#include "mysteriousshop.hpp"
#include "engineadapter.h"
#include "item/knapsack.h"
#include "item/itempool.h"
#include "item/itembase.h"
#include "item/itemextern.hpp"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "gamelog.h"

#include "servercommon/configcommon.h"
#include "other/event/eventhandler.hpp"
#include "internalcomm.h"
#include "checkresourcecenter.hpp"

#include "world.h"
#include "servercommon/string/gameworldstr.h"

Mysteriousshop::Mysteriousshop () : m_max_shopitem_seq(0), m_saleitem_total_weight(0), m_next_refreshtime(0), m_dissappeartime(0), m_npc_sceneid(0), m_npc_pos(0, 0)
{

}

Mysteriousshop::~Mysteriousshop()
{

}

bool Mysteriousshop::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "Mysteriousshop", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo,"Load Mysteriousshop config [%s] Error.cannot find root node.", configname.c_str());
		*err = errinfo;
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("shopitem");
		if (root_element.empty())
		{
			*err = configname + ": no [shopitem].";
			return false;
		}

		int ret = this->InitShopItemConfig(root_element);
		if (ret)
		{
			sprintf(errinfo,"%s: InitShopItemConfig failed %d", configname.c_str(), ret);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("refreshpos");
		if (root_element.empty())
		{
			*err = configname + ": no [refreshpos].";
			return false;
		}

		int ret = this->InitRefreshPosConfig(root_element);
		if (ret)
		{
			sprintf(errinfo,"%s: InitRefreshPosConfig failed %d", configname.c_str(), ret);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		int ret = this->InitOtherConfig(root_element);
		if (ret)
		{
			sprintf(errinfo,"%s: InitOtherConfig failed %d", configname.c_str(), ret);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int Mysteriousshop::InitShopItemConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_saleitem_total_weight = 0;
	int last_seq = -1;

	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq - last_seq != 1 || seq >= MAX_SHOP_ITEM_NUM)
		{
			return -1;
		}
		last_seq = seq;

		if (seq > m_max_shopitem_seq)
		{
			m_max_shopitem_seq = seq;
		}

		ShopItem &shopitem = m_shop_item_cfglist[seq];
		shopitem.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "itemid", shopitem.item_id) || NULL == ITEMPOOL->GetItem(shopitem.item_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "isbind", shopitem.isbind) || (shopitem.isbind != 0 && shopitem.isbind != 1))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "price_type", shopitem.price_type) || shopitem.price_type < 0 || shopitem.price_type >= PRICETYPE_MAX)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "price", shopitem.price) || shopitem.price <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "count_limit", shopitem.count_limit) || shopitem.count_limit <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", shopitem.weight) || shopitem.weight <= 0)
		{
			return -7;
		}
		m_saleitem_total_weight += shopitem.weight;

		if (PRICETYPE_BIND_GOLD == shopitem.price_type && shopitem.isbind != 1)
		{
			return -100;
		}

		data_element = data_element.next_sibling();
	}

	if (m_saleitem_total_weight <= 0)
	{
		return -9999;
	}

	return 0;
}

int Mysteriousshop::InitRefreshPosConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		RefreshPosItem pos_item;

		if (!PugiGetSubNodeValue(data_element, "sceneid", pos_item.sceneid) || pos_item.sceneid <= 0)
		{
			return -1;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(pos_item.sceneid);
		
		if (!PugiGetSubNodeValue(data_element, "posx", pos_item.posx) || pos_item.posx <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "posy", pos_item.posy) || pos_item.posy <= 0)
		{
			return -3;
		}

		m_refrespos_vec.push_back(pos_item);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int Mysteriousshop::InitOtherConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "refreshtime", m_other_cfg.refreshtime) || m_other_cfg.refreshtime <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "existtime", m_other_cfg.existtime) || m_other_cfg.existtime <= 0 || m_other_cfg.existtime >= m_other_cfg.refreshtime)
	{
		return -2;
	}

	return 0;
}

void Mysteriousshop::CalcSaleItemSeqList()
{
	memset(m_sale_item_list, 0, sizeof(m_sale_item_list));

	std::set<int> randomed; // 记录已随机到的，保让不重复
	int sale_num = 0;
	int rand_value = 0;
	int temp_total_weight = m_saleitem_total_weight;

	while(sale_num < MAX_SALE_ITEM_NUM)
	{
		++ sale_num;
		if (temp_total_weight <= 0)
		{
			break;
		}

		rand_value = RandomNum(temp_total_weight);

		for (int i = 0; i <= m_max_shopitem_seq; ++ i)
		{
			const ShopItem *item_cfg = &m_shop_item_cfglist[i];

			if (NULL != item_cfg && randomed.find(item_cfg->seq) == randomed.end())
			{
				if (rand_value <= item_cfg->weight &&
					sale_num >= 1 && sale_num <= MAX_SALE_ITEM_NUM)
				{
					m_sale_item_list[sale_num - 1].seq = item_cfg->seq;
					randomed.insert(item_cfg->seq);
					temp_total_weight -= item_cfg->weight;
					break;
				}

				rand_value -= item_cfg->weight;
			}
		}
	}

	randomed.clear();
}

void Mysteriousshop::Update(unsigned long interval, time_t now_second)
{
// 	if (0 != m_dissappeartime && static_cast<int>(now_second) >= m_dissappeartime)
// 	{
// 		memset(m_sale_item_list, 0, sizeof(m_sale_item_list));
// 		m_npc_sceneid = 0;
// 		m_npc_pos.x = 0;
// 		m_npc_pos.y = 0;
// 		m_dissappeartime = 0;
// 
// 		this->SyncRefreshNpcPosToWrold();
// 	}
// 
// 	if (static_cast<int>(now_second) >= m_next_refreshtime)
// 	{
// 		int size = static_cast<int>(m_refrespos_vec.size());
// 		if (size > 0)
// 		{
// 			int index = RandomNum(size);
// 			const RefreshPosVec::const_iterator iter = m_refrespos_vec.begin() + index;
// 			if (m_refrespos_vec.end() == iter)
// 			{
// 				return;
// 			}
// 
// 			if (iter->sceneid == m_npc_sceneid && iter->posx == m_npc_pos.x && iter->posy == m_npc_pos.y)  //确保不在同个点
// 			{
// 				return;
// 			}
// 
// 			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(iter->sceneid, 0); 
// 			if (NULL == scene)
// 			{
// 				return;
// 			}
// 
// 			m_next_refreshtime = static_cast<int>(now_second) + m_other_cfg.refreshtime;
// 			m_dissappeartime = static_cast<int>(now_second) + m_other_cfg.existtime;
// 			m_npc_sceneid = iter->sceneid;
// 			m_npc_pos.x = iter->posx;
// 			m_npc_pos.y = iter->posy;
// 
// 			this->CalcSaleItemSeqList();
// 		
// 			this->SyncRefreshNpcPosToWrold();
// 			this->SyncShopItemInfo();
// 
// 			// 传闻
// 			{
// 				int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mysteriousrefresh_notice, 
// 					m_npc_sceneid);
// 				if (len > 0) 
// 				{
// 					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_AND_ROLL);
// 				}
// 			}
// 		}
// 	}
}

void Mysteriousshop::BuyShopNormalItemReq(Role *role, int shopitem_seq, int num)
{
// 	if (NULL == role || num <= 0)
// 	{
// 		return;
// 	}
// 
// 	if (shopitem_seq < 0 || shopitem_seq > m_max_shopitem_seq || shopitem_seq >= MAX_SHOP_ITEM_NUM)
// 	{
// 		role->NoticeNum(errornum::EN_MYSTERIOUSSHOP_ITEM_NOTEXIST);
// 		return;
// 	}
// 
// 	if (!role->GetKnapsack()->HaveSpace())
// 	{
// 		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
// 		return;
// 	}
// 
// 	if (static_cast<int>(EngineAdapter::Instance().Time()) >= m_dissappeartime &&
// 		static_cast<int>(EngineAdapter::Instance().Time()) < m_next_refreshtime)
// 	{
// 		role->NoticeNum(errornum::EN_MYSTERIOUSSHOP_NPC_DISSAPPEAR);
// 		return;
// 	}
// 
// 	if (role->GetScene()->GetSceneID() != m_npc_sceneid || (m_npc_pos - role->GetPos()).IsLongerThan(20))
// 	{
// 		role->NoticeNum(errornum::EN_TOO_FAR_FROM_NPC);
// 		return;
// 	}
// 
// 	const ShopItem *shop_item = &m_shop_item_cfglist[shopitem_seq];
// 	if (NULL == shop_item)
// 	{
// 		role->NoticeNum(errornum::EN_MYSTERIOUSSHOP_ITEM_NOTEXIST);
// 		return;
// 	}
// 
// 	int sale_index = -1;	// 售卖列表中的索引，不是商品序列号
// 
// 	{
// 		for (int i = 0; i < MAX_SALE_ITEM_NUM; ++ i)
// 		{
// 			if (m_sale_item_list[i].seq == shopitem_seq)
// 			{
// 				sale_index = i;
// 				break;
// 			}
// 		}
// 
// 		if (sale_index < 0 || sale_index >= MAX_SALE_ITEM_NUM)
// 		{
// 			role->NoticeNum(errornum::EN_MYSTERIOUSSHOP_ITEM_NOTEXIST);
// 			return;
// 		}
// 
// 		if (m_sale_item_list[sale_index].buynum + num > shop_item->count_limit)
// 		{
// 			role->NoticeNum(errornum::EN_MYSTERIOUSSHOP_BUY_NUM_LIMIT);
// 			return;
// 		}
// 	}
// 
// 	{
// 		if (Mysteriousshop::PRICETYPE_GOLD == shop_item->price_type && shop_item->price > 0)
// 		{
// 			if (!role->GetKnapsack()->GetMoney()->UseGold(shop_item->price * num, "MysteriousshopBuy"))
// 			{
// 				role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 				return;	
// 			}
// 		}
// 
// 		else if (Mysteriousshop::PRICETYPE_BIND_GOLD == shop_item->price_type && shop_item->price > 0)
// 		{
// 			if (!role->GetKnapsack()->GetMoney()->UseGoldBind(shop_item->price * num, "MysteriousshopBuy"))
// 			{
// 				role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
// 				return;	
// 			}
// 		}
// 
// 		else if (Mysteriousshop::PRICETYPE_COIN == shop_item->price_type && shop_item->price > 0)
// 		{
// 			if (!role->GetKnapsack()->GetMoney()->UseCoin(shop_item->price * num, "MysteriousshopBuy"))
// 			{
// 				role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
// 				return;	
// 			}
// 		}
// 
// 		else
// 		{
// 			return;
// 		}
// 	}
// 
//  	ItemConfigData item_config_data(shop_item->item_id, shop_item->isbind != 0, num);
//  
//  	if (!role->GetKnapsack()->PutOrMail(item_config_data, PUT_REASON_MYSTERIOUSSHOP))
//  	{
//  		return;
//  	}
// 
// 	m_sale_item_list[sale_index].buynum += num;
// 
// 	this->SyncShopItemInfo();
// 
// 	role->NoticeNum(noticenum::NT_SHOP_BUY_SUCC);
// 	
// 	gamelog::g_log_other.printf(LL_INFO, "BuyShopNormalItemReq user[%d, %s], level[%d], itemid[%d], is_bind[%d], num[%d] pricetype[%d], price[%d] ", 
// 		role->GetUID(), role->GetName(), role->GetLevel(), shop_item->item_id, shop_item->isbind, num, shop_item->price_type, shop_item->price);
}

void Mysteriousshop::SyncRefreshNpcPosToWrold()
{
// 	static Protocol::SCMysteriosNpcRefresh cmd;
// 	cmd.disappeartime = m_dissappeartime;
// 	cmd.nextrefreshtime = m_next_refreshtime;
// 	cmd.npc_sceneid = m_npc_sceneid;
// 	cmd.npc_x = m_npc_pos.x;
// 	cmd.npc_y = m_npc_pos.y;
// 
// 	World::GetInstWorld()->SendToAllGateway((const char *)&cmd, sizeof(cmd));
}

void Mysteriousshop::SyncRefreshNpcPosToRole(Role *role)
{
// 	if (NULL == role)
// 	{
// 		return;
// 	}
// 
// 	static Protocol::SCMysteriosNpcRefresh cmd;
// 	cmd.disappeartime = m_dissappeartime;
// 	cmd.nextrefreshtime = m_next_refreshtime;
// 	cmd.npc_sceneid = m_npc_sceneid;
// 	cmd.npc_x = m_npc_pos.x;
// 	cmd.npc_y = m_npc_pos.y;
// 
// 	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&cmd, sizeof(cmd));
}

void Mysteriousshop::SyncShopItemInfo()
{
// 	 //只要同个场景的玩家才有可能需要看到刷新、。不用全服广播，省传输字节
// 	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(m_npc_sceneid, 0); 
// 
// 	if (NULL == scene)
// 	{
// 		return;
// 	}
// 
// 	static Protocol::SCMysteriosshopInfo cmd;
// 
// 	UNSTD_STATIC_CHECK(sizeof(cmd.saleitem_list) == sizeof(m_sale_item_list));
// 	memcpy(cmd.saleitem_list, m_sale_item_list, sizeof(cmd.saleitem_list));
// 
// 	const int role_num = (int)scene->RoleNum();
// 	for (int i = 0; i < role_num; ++ i)
// 	{
// 		Role *role = scene->GetRoleByIndex(i);
// 		if (NULL != role)
// 		{
// 			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&cmd, sizeof(cmd));
// 		}
// 	}
}

void Mysteriousshop::GmNextStatus()
{
	int now_time = static_cast<int>(EngineAdapter::Instance().Time());

	if (0 != m_dissappeartime && now_time < m_dissappeartime)
	{
		m_dissappeartime = now_time;
		return;
	}

	if (now_time < m_next_refreshtime)
	{
		m_next_refreshtime = now_time;
	}
}