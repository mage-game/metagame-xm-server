#include <memory.h>
#ifdef max
#undef max
#undef min
#endif // max
#include <algorithm>


#include "servercommon/struct/itemlistparam.h"

#include "engineadapter.h"
#include "internalcomm.h"
#include "protocal/msgitem.h"
#include "protocal/msgequipment.h"

#include "obj/character/role.h"

#include "knapsack.h"
#include "itembase.h"
#include "itempool.h"
#include "itemextern.hpp"

#include "servercommon/errornum.h"
#include "globalconfig/globalconfig.h"
#include "gamelog.h"
#include "gstr.h"

#include "servercommon/struct/structcommon.h"
#include "servercommon/string/gameworldstr.h"

#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"

#include "config/logicconfigmanager.hpp"
#include "global/globalrole/litttlepet/littlepetconfig.hpp"
#include "config/zhuanzhiconfig.hpp"
#include "other/shop/roleshop.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/monitor/monitor.hpp"
#include "other/sublock/sublock.hpp"
#include "other/zhuanshengequip/zhuanshengequip.hpp"
#include "other/cross/rolecross.h"
#include "global/globalrole/globalrolemanager.hpp"
#include "global/globalrole/qingyuanglobal/qingyuanglobal.hpp"
#include "other/qingyuan/qingyuanconfig.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "servercommon/commondata.hpp"
#include "other/impguard/impguardconfig.hpp"
#include "other/vip/vipconfig.hpp"
#include "other/vip/vip.hpp"
#include "global/globalrole/litttlepet/littlepet.hpp"
#include "other/lieming/liemingmanager.h"
#include "config/giftreplaceconfig.hpp"
#include "expense/norexitem.hpp"
#include "other/runesystem/runesystem.hpp"
#include "other/shenyin/shenyin.hpp"
#include "other/shengesystem/shengesystem.hpp"
#include "global/droplog/droplogmanager.hpp"
#include "other/zodiac/zodiac.hpp"
#include "other/crossequip/crossequip.hpp"


using namespace ItemNamespace;

const char *ItemGridDataLog(const ItemBase *itemimp, const ItemGridData &grid_data)
{
	if (itemimp->GetItemType() == ItemBase::I_TYPE_EQUIPMENT)
	{
		static ItemDataWrapper tmp_item_wrapper;
		grid_data.WrapTo(&tmp_item_wrapper);
		return ItemDataWrapperLog(&tmp_item_wrapper);
	}

	return NULL;
}

const char *ItemGridQuickLog(const ItemBase *itemimp, const ItemGridData &grid_data)
{
	if (itemimp->GetItemType() == ItemBase::I_TYPE_EQUIPMENT)
	{
		static ItemDataWrapper tmp_item_wrapper;
		grid_data.WrapTo(&tmp_item_wrapper);
		return ItemDataQuickLog(&tmp_item_wrapper);
	}

	return NULL;
}

struct SortStruct
{
	void Set(int _prior_id, int _src_index, bool _is_bind) { prior_id = _prior_id; src_index = _src_index; is_bind = _is_bind; }

	int prior_id;
	int src_index;
	bool is_bind;
};

static const int MAX_KNAPSACK_OR_STOREAGE_ARRAY_SIZE = MAX_KNAPSACK_GRID_NUM > MAX_STORAGE_GRID_NUM ? MAX_KNAPSACK_GRID_NUM: MAX_STORAGE_GRID_NUM;

int SortCompare(const void *arg1, const void *arg2)
{
	SortStruct *s1 = (SortStruct*)arg1;
	SortStruct *s2 = (SortStruct*)arg2;
	int result = s1->prior_id - s2->prior_id;
	if (result != 0) return result;

	return (char)s1->is_bind - (char)s2->is_bind;
}

//////////////////////////////////////////////////////////////////////////

Knapsack::Knapsack()
	: m_role(NULL), 
	m_knapsack_is_change(false),
	m_is_lock(false),
	m_curr_knapsack_valid_num(0),	
	m_curr_storage_valid_num(0),
	m_valid_grid_index_end(0),
	m_valid_grid_auto_add_count(0),
	m_knapsack_last_inorder_time(0),
	m_storage_last_inorder_time(0),
	m_last_inorder_merge_time(0)
{
	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));
	memset(m_old_state, 0, sizeof(m_old_state));
}

Knapsack::~Knapsack()
{
	for (int i = 0; i < MAX_KNAPSACK_STORAGE_GRID_NUM; ++i)
	{
		m_grid_list[i].Clear();
	}
}

void Knapsack::Init(Role *role, int param_list_num, const ItemListParam::ItemListData *param_data_list[], int valid_knapsack_num, int valid_storage_num,
					int valid_grid_auto_add_count, Int64 gold, Int64 gold_bind, Int64 coin, Int64 coin_bind)
{
	m_role = role;

	m_money.Init(role, gold, gold_bind, coin, coin_bind);

	m_curr_knapsack_valid_num = valid_knapsack_num;
	if (m_curr_knapsack_valid_num > MAX_KNAPSACK_GRID_NUM)
	{
		m_curr_knapsack_valid_num = MAX_KNAPSACK_GRID_NUM;
	}

	m_curr_storage_valid_num = valid_storage_num;
	if (m_curr_storage_valid_num > MAX_STORAGE_GRID_NUM)
	{
		m_curr_storage_valid_num = MAX_STORAGE_GRID_NUM;
	}

	m_valid_grid_auto_add_count = valid_grid_auto_add_count;
	if (m_valid_grid_auto_add_count > MAX_KNAPSACK_GRID_NUM)
	{
		m_valid_grid_auto_add_count = MAX_KNAPSACK_GRID_NUM;
	}

	int biggest_knapsack_index = -1;

	for (int i = param_list_num - 1; i >= 0; --i)
	{
		if (NULL == param_data_list[i]) break;

		int index = param_data_list[i]->index;
		if (index < 0 || index >= MAX_KNAPSACK_STORAGE_GRID_NUM) 
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[Knapsack::Init][[%s %d] index invalid:%d]", 
				m_role->GetName(), UidToInt(role->GetUserId()), index);
			continue;
		}

 		if (!m_grid_list[index].Invalid() || NULL != m_grid_list[index].param)
 		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[Knapsack::Init Duplicate][[%s %d] index:%d old_item_id:%d new_item_id:%d old_has_param:%d]", m_role->GetName(), UidToInt(role->GetUserId()),
				index, (int)m_grid_list[index].item_id, (int)param_data_list[i]->item_wrapper.item_id, (int)(NULL != m_grid_list[index].param));
			continue;
		}

		ItemID item_id = param_data_list[i]->item_wrapper.item_id;
		int num = param_data_list[i]->item_wrapper.num;

		const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
		if (NULL == itembase || num <= 0 || num > itembase->GetPileLimit())
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[Knapsack::Init Error][[%s %d] index:%d item_id:%d num:%d]", m_role->GetName(), UidToInt(role->GetUserId()),
				index, (int)item_id, (int)num);
			continue;
		}

		if (0 == param_data_list[i]->item_wrapper.has_param)
		{
			m_grid_list[index].param = NULL;
		}
		else
		{
			const Equipment *equip = (const Equipment *)itembase;
			if (equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT) // 传入的item_id不是装备
			{
				gamelog::g_log_serious_error.printf(LL_CRITICAL, "[Knapsack::Init HasParamButIsNotEquipment][[%s %d] index:%d item_id:%d]", m_role->GetName(), UidToInt(role->GetUserId()),
					index, (int)item_id);
				continue;
			}

			EquipmentParam *param = equip->CreateParamFromStruct(&param_data_list[i]->item_wrapper.param_data);
			if (NULL == param) // 创建param出错
			{
				gamelog::g_log_serious_error.printf(LL_CRITICAL, "[Knapsack::Init CreateParamError][[%s %d] index:%d item_id:%d]", m_role->GetName(), UidToInt(role->GetUserId()),
					index, (int)item_id);
				continue;
			}

			m_grid_list[index].param = param;
		}

		m_grid_list[index].item_id = item_id;
		m_grid_list[index].num = num;
		m_grid_list[index].is_bind = (0 != param_data_list[i]->item_wrapper.is_bind ? true : false);
		m_grid_list[index].invalid_time = param_data_list[i]->item_wrapper.invalid_time;
		m_grid_list[index].gold_price =  param_data_list[i]->item_wrapper.gold_price;
		
		if (m_grid_list[index].invalid_time > 0 && !itembase->HasInvalidTime())
		{
			m_grid_list[index].invalid_time = 0;
		}

		m_old_state[index] = true;

		if (biggest_knapsack_index < index && index < MAX_KNAPSACK_GRID_NUM) // 记录下背包有效格子的最大序号
		{
			biggest_knapsack_index = index;
		}
	}

	m_valid_grid_index_end = biggest_knapsack_index + 1;
}

void Knapsack::SetRole(Role *role)
{
	m_role = role;

	m_money.SetRole(role);
	m_item_codldown_manager.SetRole(role);
}

void Knapsack::Update(time_t now_second)
{
	UInt32 curr_time = static_cast<UInt32>(now_second);
	for (int i = 0; i < m_valid_grid_index_end; i++) // 仅轮询背包的，仓库不管
	{
		if (m_grid_list[i].Invalid()) continue;

		const ImpConfig *imp_cfg = LOGIC_CONFIG->GetImpGuardConfig().GetImpCfgByItemId(m_grid_list[i].item_id);
		if (nullptr != imp_cfg) continue;          // 守护小鬼失效不删除

		if (m_grid_list[i].invalid_time > 0 && m_grid_list[i].invalid_time <= curr_time)
		{
			const ItemBase *item = ITEMPOOL->GetItem(m_grid_list[i].item_id);
			if (NULL != item)
			{
				static MailContentParam contentparam; contentparam.Reset();
				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_item_invalid_subiect);
				int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_item_invalid_content, item->GetItemName());
				if (length1 > 0 && length2 > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}

			m_grid_list[i].Clear();

			this->SetItemDirty(i, true);
		}
	}

	// 更新背包格子数
	if (GLOBALCONFIG->CanAutoExtendKnapsack(m_valid_grid_auto_add_count, static_cast<unsigned int>(m_role->GetRealTimeOnlineS())) && m_curr_knapsack_valid_num < MAX_KNAPSACK_GRID_NUM)
	{
		this->ExtendValidNum(false, false, 1, true);
	}
}

void Knapsack::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		for (int i = 0; i < m_valid_grid_index_end && i < MAX_KNAPSACK_GRID_NUM; i++) // 背包
		{
			if (m_grid_list[i].Invalid()) continue;

			const ItemBase *item = ITEMPOOL->GetItem(m_grid_list[i].item_id);
			if (NULL != item && item->IsCurDayValid())
			{
				static MailContentParam contentparam; contentparam.Reset();
				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_item_invalid_subiect);
				int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_item_invalid_content, item->GetItemName());
				if (length1 > 0 && length2 > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}

				m_grid_list[i].Clear();

				this->SetItemDirty(i, true);
			}
		}

		for (int i = MAX_KNAPSACK_GRID_NUM; i < MAX_KNAPSACK_GRID_NUM + m_curr_storage_valid_num && i < MAX_KNAPSACK_STORAGE_GRID_NUM; i++) // 仓库
		{
			if (m_grid_list[i].Invalid()) continue;

			const ItemBase *item = ITEMPOOL->GetItem(m_grid_list[i].item_id);
			if (NULL != item && item->IsCurDayValid())
			{
				static MailContentParam contentparam; contentparam.Reset();
				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_item_invalid_subiect);
				int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_item_invalid_content, item->GetItemName());
				if (length1 > 0 && length2 > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}

				m_grid_list[i].Clear();

				this->SetItemDirty(i, true);
			}
		}
	}
}

void Knapsack::OnResetData()
{
}

bool Knapsack::UseItem(ItemID item_id, short num)
{
	if (num <= 0)
	{
		return false;
	}
	short bind_item_id_list[ItemNamespace::MAX_KNAPSACK_STORAGE_GRID_NUM];
	short bind_item_num_list[ItemNamespace::MAX_KNAPSACK_STORAGE_GRID_NUM];
	short bind_item_count = 0;
	short unbind_item_id_list[ItemNamespace::MAX_KNAPSACK_STORAGE_GRID_NUM];
	short unbind_item_num_list[ItemNamespace::MAX_KNAPSACK_STORAGE_GRID_NUM];
	short unbind_item_count;
	GetListOfItem(item_id, num, bind_item_id_list, bind_item_num_list, &bind_item_count,
		unbind_item_id_list, unbind_item_num_list, &unbind_item_count);

	short used_count = num;

	for (short i = 0; i < ItemNamespace::MAX_KNAPSACK_STORAGE_GRID_NUM && i < bind_item_count; i++)
	{
		short use_count = std::min(used_count, bind_item_num_list[i]);
		if (use_count <= 0)
		{
			break;
		}
		
		if (!Use(bind_item_id_list[i], use_count))
		{
			break;
		}
		used_count -= use_count;
	}

	for (short i = 0; i < ItemNamespace::MAX_KNAPSACK_STORAGE_GRID_NUM && i < unbind_item_count; i++)
	{
		short use_count = std::min(used_count, unbind_item_num_list[i]);
		if (use_count <= 0)
		{
			break;
		}
		
		if (!Use(unbind_item_id_list[i], use_count))
		{
			break;
		}
		used_count -= use_count;
	}

	return used_count == 0;
}

bool Knapsack::Use(short index, short num, short equip_index)
{
	if (num <= 0 || index < 0 || index >= MAX_KNAPSACK_GRID_NUM) return false;

	if (m_is_lock)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	ItemGridData &item_data = m_grid_list[index];	
	if (item_data.Invalid())
	{
		item_data.Clear();
		this->SetItemDirty(index, true); 
		m_role->NoticeNum(errornum::EN_KNAPSACK_USE_ITEM_ERROR);
		return false;
	}

	ItemID use_item_id = item_data.item_id;
	if (LOGIC_CONFIG->GetGiftReplaceConfig().GetReplaceGiftId(use_item_id, m_role->GetLevel())) // 礼包替换
	{
		gamelog::g_log_knapsack.printf(LL_INFO, "[Knapsack::Use ChangeGiftId][user[%s %d] {before:item_id:%d --> after:item_id:%d}]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), (int)item_data.item_id, (int)use_item_id);
	}

	if (!m_role->GetRoleStatusManager()->CanUseItem(use_item_id))
	{
		return false;
	}

	if (m_grid_list[index].num < num)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_USE_ITEM_NUM_NOT_ENOUGH);
		return false;
	}

	const ItemBase *itemimp = ITEMPOOL->GetItem(use_item_id);
	if (NULL == itemimp)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_LOAD_ERROR);
		return false;
	}

	if (m_role->GetSublock()->CheckHasLock(false))
	{
		bool to_forbid = true;
		short item_type = itemimp->GetItemType();
		switch (item_type)
		{
		case ItemBase::I_TYPE_EXPENSE:
		case ItemBase::I_TYPE_GIFT:
			to_forbid = false;
			break;
		}

		if (to_forbid)
		{
			m_role->NoticeNum(errornum::EN_SUBLOCK_IS_LOCKED);
			return false;
		}
	}

	int use_level = m_role->GetLevel(); 

	if (NULL != item_data.param) // 如果有装备param 则检查物品是否确实是装备
	{
		if (itemimp->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_USE_ITEM_ERROR);
			return false;
		}
		else
		{	
			//尝试增加可穿戴等级
			Equipment *equipment = (Equipment *)itemimp;
			if (equipment->GetEquipType() >= Equipment::E_TYPE_MIN && equipment->GetEquipType() < Equipment::E_TYPE_MAX)
			{
				use_level = m_role->GetPutOnLevel();
			}
		}

		item_data.num = 1; // 此处置为1  是为了下文装备使用后数量确实为0
		this->SetItemDirty(index, false);
	}
	else // 如果没有装备参数 必须保证它一定不能是装备 
	{
		if (itemimp->GetItemType() == ItemBase::I_TYPE_EQUIPMENT)
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_USE_ITEM_ERROR);
			return false;
		}
	}

	if (use_level < itemimp->GetLimitLevel())
	{
		m_role->NoticeNum(errornum::EN_ITEM_LEVEL_LIMIT);
		return false;
	}

	if (!m_role->IsProfSuitable((char)itemimp->GetLimitProf()))
	{
		m_role->NoticeNum(errornum::EN_ITEM_PROF_LIMIT);
		return false;
	}

	if (!m_role->IsSexSuitable((char)itemimp->GetLimitSex()))
	{
		m_role->NoticeNum(errornum::EN_ITEM_SEX_LIMIT);
		return false;
	}

	bool item_is_bind = m_grid_list[index].is_bind;

	int ret = itemimp->Use(m_role, item_data, num, equip_index);	// 实际使用数量会在传出后修正
	if (ret != 0)
	{
		int special_use_ret = -1;

		bool succ_msg = true;
		// 铭文装备的特殊处理,避免客户端刷新闪现问题
		if (ItemBase::I_TYPE_EXPENSE == itemimp->GetItemType())
		{
			const NorexItem *norex_item = dynamic_cast<const NorexItem *>(itemimp);
			if (norex_item && NorexItem::I_NOREX_ITEM_SHENYIN == norex_item->GetUseType())
			{
				special_use_ret = m_role->GetShenYin()->PutOnItem(item_data);  // 注意！！该函数会修改item_data
			}
			else if (norex_item && NorexItem::I_NOREX_ITEM_PUT_SHENGE == norex_item->GetUseType() && norex_item->GetColor() <= ItemBase::I_COLOR_PURPLE){
				succ_msg = false;
			}
		}


		if (special_use_ret >= 0)
		{
			// 使用成功
			if (succ_msg)
			{
				m_role->NoticeNum(noticenum::NT_ITEM_USE_SUCC);
			}

			if (special_use_ret != 0)
			{
				// 使用成功并且产生了替换, 更新格子信息

				if (itemimp->IsRecord())
				{
					// use log
					gamelog::g_log_knapsack.printf(LL_INFO, "[Knapsack::Use Succ][user[%s %d] item_id:%d item_name:%s(%d) use_num:%d fail_result:0]",
						m_role->GetName(), UidToInt(m_role->GetUserId()), (int)use_item_id, itemimp->GetItemName(), (int)item_is_bind, (int)num);

					ROLE_LOG_QUICK6(LOG_TYPE_ITEM_USE, m_role, use_item_id, num, NULL, NULL);

					// put log
					const ItemBase* p_replace_item = ITEMPOOL->GetItem(item_data.item_id);
					if (p_replace_item)
					{
						gamelog::g_log_knapsack.printf(LL_INFO, "[Knapsack::Replace Succ][user[%s %d] item_id:%d item_name:%s(%d) use_num:%d fail_result:0]",
							m_role->GetName(), UidToInt(m_role->GetUserId()), (int)item_data.item_id, p_replace_item->GetItemName(), item_data.is_bind ? 1 : 0, (int)item_data.num);
					}
					
					ROLE_LOG_QUICK6(LOG_TYPE_ITEM_PUT, m_role, item_data.item_id, item_data.num, NULL, NULL);
				}

				this->SetItemDirty(index);

				Protocol::SCUseItemSuc uis;
				uis.item_id = use_item_id;
				EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&uis, sizeof(Protocol::SCUseItemSuc));

				return true;
			}
		}
		else
		{
			// 使用失败
			{
				gamelog::g_log_knapsack.printf(LL_INFO, "[Knapsack::Use Fail][user[%s %d] item_id:%d item_name:%s(%d) use_num:%d fail_result:%d]",
					m_role->GetName(), UidToInt(m_role->GetUserId()), (int)itemimp->GetItemId(), itemimp->GetItemName(), (int)item_is_bind, (int)num, ret);
			}

			return false;
		}
	}

	if (item_data.gold_price > 0)
	{
		int consume_gold_price = ItemExtern::GetGoldPriceByNum(item_data.item_id, item_data.num, item_data.gold_price, num);
		if (consume_gold_price > 0)
		{
			if (consume_gold_price > item_data.gold_price) consume_gold_price = item_data.gold_price;

			item_data.gold_price -= consume_gold_price;
		}
	}

	ItemID item_id = item_data.item_id;

	item_data.num -= num;
	if (item_data.num <= 0) // 消耗数量 注意这个num已经修正为实际使用数量
	{
		item_data.Clear();	// 指针已无用 删除之
		this->RefreshValidGridIndexEnd(index);
	}

	if (itemimp->IsRecord())
	{
		gamelog::g_log_knapsack.printf(LL_INFO,"[Knapsack::Use Succ][user[%s %d] item_id:%d item_name:%s(%d) use_num:%d fail_result:0]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), (int)item_id, itemimp->GetItemName(), (int)item_is_bind, (int)num);

		ROLE_LOG_QUICK6(LOG_TYPE_ITEM_USE, m_role, item_id, num, NULL, NULL);
	}

	this->SetItemDirty(index);

	Protocol::SCUseItemSuc uis; 
	uis.item_id = item_id;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&uis, sizeof(Protocol::SCUseItemSuc));

	return true;
}

bool Knapsack::Discard(short index, short discard_num, int discard_method, ItemID item_id_in_client, int item_num_in_client, bool is_notice_client)
{
	if (!this->CheckCanDiscard(index, discard_num, discard_method, item_id_in_client, item_num_in_client))
	{
		return false;
	}

	std::vector<KnapsackDiscardItem> index_list;

	KnapsackDiscardItem discard_item;
	discard_item.index = index;
	discard_item.discard_method = discard_method;
	discard_item.discard_num = discard_num;
	discard_item.is_succ = false;

	index_list.push_back(discard_item);
	
	bool is_discard_succ = true;
	//出售
	if (DISCARD_METHOD_SELL == discard_method)
	{
		is_discard_succ = this->DiscardSell(index_list, is_notice_client);
	}
	
	//回收
	if (DISCARD_METHOD_RECYCL == discard_method)
	{
		is_discard_succ = this->DiscardRecycleNone(index_list, is_notice_client)
						&& this->DiscardRecycleLingjing(index_list, is_notice_client)
						&& this->DiscardRecycleXiuwei(index_list, is_notice_client)
						&& this->DiscardRecycleMojing(index_list, is_notice_client)
						&& this->DiscardRecycleXuantie(index_list, is_notice_client)
						&& this->DiscardRecycleMiyin(index_list, is_notice_client)
						&& this->DiscardRecycleLittlePet(index_list, is_notice_client)
						&& this->DiscardRecycleMarrylevelExp(index_list, is_notice_client)
						&& this->DiscardRecycleHunjieDecompose(index_list, is_notice_client)
						&& this->DiscardRecyclePetScore(index_list, is_notice_client)
						&& this->DiscardRecycleHunli(index_list, is_notice_client)
						&& this->DiscardRecycleChuanshiFragment(index_list, is_notice_client);
	}

	if (!is_discard_succ)
	{
		return false;
	}

	return true;
}

bool Knapsack::BatchDiscard(Protocol::CSBatchDiscardItem *discard_item_list)
{
	if (nullptr == discard_item_list)
	{
		return false;
	}

	std::vector<KnapsackDiscardItem> index_list;
	std::set<int> index_set;
	for (unsigned int i = 0; i < (unsigned int)discard_item_list->index_count && i < static_array_size(discard_item_list->index_list); i++)
	{
		int index = discard_item_list->index_list[i].index;
		int discard_num = discard_item_list->index_list[i].discard_num;
		int discard_method = discard_item_list->index_list[i].discard_medthod;
		int item_id_in_client = discard_item_list->index_list[i].item_id_in_client;
		int item_num_in_client = discard_item_list->index_list[i].item_num_in_client;

		if (!this->CheckCanDiscard(index, discard_num, discard_method, item_id_in_client, item_num_in_client))
		{
			return false;
		}

		//筛选重复index
		auto it = index_set.find(index);
		if (it != index_set.end())
		{
			continue;
		}

		KnapsackDiscardItem discard_item;
		discard_item.index = index;
		discard_item.discard_method = discard_method;
		discard_item.discard_num = discard_num;
		discard_item.is_succ = false;
		
		index_list.push_back(discard_item);
		index_set.insert(index);
	}

	bool is_discard_succ = true;

	is_discard_succ = this->DiscardSell(index_list, false)
					&& this->DiscardRecycleNone(index_list, false)
					&& this->DiscardRecycleLingjing(index_list, false)
					&& this->DiscardRecycleXiuwei(index_list, false)
					&& this->DiscardRecycleMojing(index_list, false)
					&& this->DiscardRecycleXuantie(index_list, false)
					&& this->DiscardRecycleMiyin(index_list, false)
					&& this->DiscardRecycleLittlePet(index_list, false)
					&& this->DiscardRecycleMarrylevelExp(index_list, false)
					&& this->DiscardRecycleHunjieDecompose(index_list, false)
					&& this->DiscardRecyclePetScore(index_list, false)
					&& this->DiscardRecycleHunli(index_list, false)
					&& this->DiscardRecycleChuanshiFragment(index_list, false);

	if (!is_discard_succ)
	{
		m_role->GetKnapsack()->SendKnapsackInfoToRole();
		gstr::SendError(m_role, "discard_fail");
		return false;
	}

	m_role->GetKnapsack()->SendKnapsackInfoToRole();
	gstr::SendNotice(m_role, "discard_succ");
	return true;
}

bool Knapsack::OneKeyDiscardJiling(int color, int discard_method)
{
	if (color <= ItemBase::I_COLOR_INVALID || color >= ItemBase::I_COLOR_MAX)
	{
		return false;
	}

	if (m_role->GetSublock()->CheckHasLock()) return false;

	if (m_is_lock)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	for (int index = 0; index < m_curr_knapsack_valid_num; index++)
	{
		if (m_grid_list[index].Invalid())
		{
			continue;
		}

		const ItemBase* item_base = ITEMPOOL->GetItem(m_grid_list[index].item_id);
		if (NULL == item_base || item_base->GetColor() >= color || ItemBase::I_TYPE_EQUIPMENT != item_base->GetItemType())
		{
			continue;
		}

		const Equipment *equip = (const Equipment *)item_base;
		if (Equipment::E_TYPE_SPECIAL_JINGLING != equip->GetEquipType())
		{
			continue;
		}

		this->Discard(index, m_grid_list[index].num, discard_method);

	}

	return true;
}

bool Knapsack::CheckCanDiscard(short index, short discard_num, int discard_method /*= 0*/, ItemID item_id_in_client /*= 0*/, int item_num_in_client /*= 0*/)
{
	if (m_role->GetSublock()->CheckHasLock()) return false;

	if (index < 0 || index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return false;
	if (discard_num <= 0 || discard_num > m_grid_list[index].num) return false;
	if (discard_method != DISCARD_METHOD_SELL && discard_method != DISCARD_METHOD_RECYCL) return false;

	if (m_is_lock)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	ItemID item_id = m_grid_list[index].item_id;

	if (ItemBase::INVALID_ITEM_ID == item_id) return false;

	// 加强检查
	if (ItemBase::INVALID_ITEM_ID != item_id_in_client && item_id_in_client != item_id) return false;
	if (0 != item_num_in_client && m_grid_list[index].num != item_num_in_client) return false;

	const ItemBase *itemimp = ITEMPOOL->GetItem(item_id);
	if (NULL == itemimp) return false;

	//int num = discard_num;
	//bool item_is_bind = m_grid_list[index].is_bind;

	int reason = 0;
	if (!itemimp->CanDiscard(m_grid_list[index].param, &reason))
	{
		if (ItemBase::NDR_INLAY == reason)
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_CANNOT_DISCARD_INLAY);
		}
		else
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_CANNOT_DISCARD);
		}

		return false;
	}

	return true;
}

void Knapsack::DoneDiscard(std::vector<KnapsackDiscardItem> &index_list, bool is_notice)
{
	for (auto it = index_list.begin(); it != index_list.end();)
	{
		if (!it->is_succ)
		{
			++it;
			continue;
		}
	
		if (it->index < 0 || it->index >= MAX_KNAPSACK_STORAGE_GRID_NUM)
		{
			++it;
			continue;
		}

		ItemID item_id = m_grid_list[it->index].item_id;
		bool item_is_bind = m_grid_list[it->index].is_bind;
		int num = it->discard_num >= m_grid_list[it->index].num ? m_grid_list[it->index].num : it->discard_num;

		const ItemBase *itemimp = ITEMPOOL->GetItem(item_id);
		if (NULL == itemimp) 
		{
			++it;
			continue;
		}

		EventHandler::Instance().OnRecycleItem(m_role, item_id);

		const char *equip_data_string = ItemGridDataLog(itemimp, m_grid_list[it->index]);
		const char *item_param_string = ItemGridQuickLog(itemimp, m_grid_list[it->index]);

		if (it->discard_num >= m_grid_list[it->index].num)
		{
			m_grid_list[it->index].Clear();				// 如果丢弃的数量大于等于拥有数，就清理格子
		}
		else
		{
			m_grid_list[it->index].num -= it->discard_num;	// 如果丢弃数量小于拥有数，就减去丢弃数
		}

		if (it->index < MAX_KNAPSACK_GRID_NUM) // 如果是背包 
		{
			this->RefreshValidGridIndexEnd(it->index);
		}

		this->SetItemDirty(it->index, is_notice, ITEM_CHANGE_TYPE_OUT);

		if (NULL != equip_data_string && NULL != item_param_string)
		{
			gamelog::g_log_knapsack.printf(LL_INFO, "[Knapsack::DiscardEquipment Succ][user[%s %d] equipment_data:%s]",
				m_role->GetName(), UidToInt(m_role->GetUserId()), equip_data_string);

			ROLE_LOG_QUICK6(LOG_TYPE_ITEM_DISCARD, m_role, itemimp->GetItemId(), it->discard_num, NULL, item_param_string);
		}
		else if (itemimp->IsRecord())
		{
			gamelog::g_log_knapsack.printf(LL_INFO, "[Knapsack::Discard Succ][user[%s %d] item_id:%d item_name:%s(%d) color:%d discard_num:%d]",
				m_role->GetName(), UidToInt(m_role->GetUserId()), (int)itemimp->GetItemId(), itemimp->GetItemName(), (int)item_is_bind, (int)itemimp->GetColor(), num);

			ROLE_LOG_QUICK6(LOG_TYPE_ITEM_DISCARD, m_role, itemimp->GetItemId(), num, NULL, NULL);
		}

		//已经分解的去掉
		it = index_list.erase(it);

		EventHandler::Instance().OnGetItem(m_role, item_id, -num);
	}
}

bool Knapsack::DiscardSell(std::vector<KnapsackDiscardItem> &index_list, bool is_notice)
{
	if (index_list.empty())
	{
		return true;
	}

	Int64 add_coin_bind = 0;
	for (auto &it : index_list)
	{
		if (DISCARD_METHOD_SELL != it.discard_method)
		{
			continue;
		}

		if (it.index < 0 || it.index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return false;

		ItemID item_id = m_grid_list[it.index].item_id;
		// 修正数量
		int discard_num = it.discard_num >= m_grid_list[it.index].num ? m_grid_list[it.index].num : it.discard_num;

		const ItemBase *itemimp = ITEMPOOL->GetItem(item_id);
		if (NULL == itemimp) return false;
		
		if (itemimp->CanSell(nullptr))
		{
			add_coin_bind += itemimp->GetSellPrice(nullptr) * discard_num;	
			it.is_succ = true;
		}
	}

	// 给予
	{
		m_money.AddCoinBind(add_coin_bind, "Discard item");
	}

	// 消耗
	this->DoneDiscard(index_list, is_notice);

	return true;
}

bool Knapsack::DiscardRecycleNone(std::vector<KnapsackDiscardItem> &index_list, bool is_notice)
{
	if (index_list.empty())
	{
		return true;
	}

	for (auto &it : index_list)
	{
		if (it.index < 0 || it.index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return false;

		ItemID item_id = m_grid_list[it.index].item_id;

		const ItemBase *itemimp = ITEMPOOL->GetItem(item_id);
		if (NULL == itemimp) return false;

		if (DISCARD_METHOD_RECYCL != it.discard_method || RECYCL_NONE != itemimp->GetRecyclType(nullptr))
		{
			continue;
		}

		it.is_succ = true;
	}

	// 消耗
	this->DoneDiscard(index_list, is_notice);

	return true;
}

bool Knapsack::DiscardRecycleLingjing(std::vector<KnapsackDiscardItem> &index_list, bool is_notice)
{
	if (index_list.empty())
	{
		return true;
	}

	int add_lingjing = 0;
	for (auto &it : index_list)
	{
		if (it.index < 0 || it.index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return false;

		ItemID item_id = m_grid_list[it.index].item_id;
		// 修正数量
		int discard_num = it.discard_num >= m_grid_list[it.index].num ? m_grid_list[it.index].num : it.discard_num;

		const ItemBase *item_base = ITEMPOOL->GetItem(item_id);
		if (NULL == item_base) return false;

		if (DISCARD_METHOD_RECYCL != it.discard_method || RECYCL_LINGJING != item_base->GetRecyclType(nullptr))
		{
			continue;
		}

		add_lingjing += item_base->GetRecyclGet(NULL) * discard_num;
		it.is_succ = true;
	}

	if (add_lingjing > 0)
	{
		m_role->AddLingJing(add_lingjing, "Knapsack::DiscardRecycleLingjing");
	}

	// 消耗
	this->DoneDiscard(index_list, is_notice);

	return true;
}

bool Knapsack::DiscardRecycleXiuwei(std::vector<KnapsackDiscardItem> &index_list, bool is_notice)
{
	if (index_list.empty())
	{
		return true;
	}

	int add_xiuwei = 0;
	for (auto &it : index_list)
	{
		if (it.index < 0 || it.index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return false;

		ItemID item_id = m_grid_list[it.index].item_id;
		// 修正数量
		int discard_num = it.discard_num >= m_grid_list[it.index].num ? m_grid_list[it.index].num : it.discard_num;

		const ItemBase *itemimp = ITEMPOOL->GetItem(item_id);
		if (NULL == itemimp) return false;

		if (DISCARD_METHOD_RECYCL != it.discard_method || RECYCL_XIUWEEI != itemimp->GetRecyclType(nullptr))
		{
			continue;
		}

		add_xiuwei += itemimp->GetRecyclGet(nullptr) * discard_num;
		it.is_succ = true;
	}

	// 给予
	{
		m_role->GetZhuanShengEquip()->AddXiuwei(add_xiuwei);
	}

	// 消耗
	this->DoneDiscard(index_list, is_notice);

	return true;
}

bool Knapsack::DiscardRecycleMojing(std::vector<KnapsackDiscardItem> &index_list, bool is_notice)
{
	if (index_list.empty())
	{
		return true;
	}

	int add_shengwang = 0;
	std::vector<ItemConfigData> put_item_list;
	for (auto &it : index_list)
	{
		if (it.index < 0 || it.index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return false;

		ItemID item_id = m_grid_list[it.index].item_id;
		// 修正数量
		int discard_num = it.discard_num >= m_grid_list[it.index].num ? m_grid_list[it.index].num : it.discard_num;

		const ItemBase *itemimp = ITEMPOOL->GetItem(item_id);
		if (NULL == itemimp) return false;

		if (DISCARD_METHOD_RECYCL != it.discard_method || RECYCL_MOJING != itemimp->GetRecyclType(nullptr))
		{
			continue;
		}

		if (ItemBase::I_TYPE_EQUIPMENT == itemimp->GetItemType())
		{
			EquipmentParam *equip_param = static_cast<EquipmentParam*>(m_grid_list[it.index].param);
			if (NULL != equip_param)
			{
				short xianpin_count = 0;
				for (int i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; i++)
				{
					const EquipXianPinConfig *xianpin_cfg = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(equip_param->xianpin_type_list[i]);
					if (NULL == xianpin_cfg)
					{
						continue;
					}

					xianpin_count++;
				}

				const EquipmentCompoundCfg *compound_cfg = GLOBALCONFIG->GetEquipConfig().GetEquipCompoundConfig(m_grid_list[it.index].item_id, xianpin_count);
				if (NULL != compound_cfg)
				{
					for (unsigned int index = 0; index < (unsigned int)compound_cfg->discard_return_count && index < static_array_size(compound_cfg->discard_return); index++)
					{
						ItemConfigData return_item;
						return_item.item_id = compound_cfg->discard_return[index].item_id;
						return_item.num = compound_cfg->discard_return[index].num;
						return_item.is_bind = m_grid_list[it.index].is_bind;

						auto item_it = std::find_if(put_item_list.begin(), put_item_list.end(), [return_item](const ItemConfigData &tmp_item) {
							return return_item.item_id == tmp_item.item_id && return_item.is_bind == tmp_item.is_bind;
						});
						if (put_item_list.end() != item_it)
						{
							item_it->num += compound_cfg->discard_return[index].num;
						}
						else
						{
							put_item_list.push_back(return_item);
						}
					}
				}
			}
		}

		add_shengwang += itemimp->GetRecyclGet(nullptr) * discard_num;
		it.is_succ = true;
	}

	// 给予
	{
		m_role->GetRoleShop()->AddChestShopMojing(add_shengwang, "Discard");

		for (auto &it : put_item_list)
		{
			this->Put(it, PUT_REASON_DISCARD_EQUIP);
		}
	}

	// 消耗
	this->DoneDiscard(index_list, is_notice);

	return true;
}

bool Knapsack::DiscardRecycleXuantie(std::vector<KnapsackDiscardItem> &index_list, bool is_notice)
{
	if (index_list.empty())
	{
		return true;
	}

	std::vector<ItemConfigData> put_item_list;
	for (auto &it : index_list)
	{
		if (it.index < 0 || it.index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return false;

		ItemID item_id = m_grid_list[it.index].item_id;

		const ItemBase *itemimp = ITEMPOOL->GetItem(item_id);
		if (NULL == itemimp) return false;

		if (DISCARD_METHOD_RECYCL != it.discard_method || RECYCL_XUANTIE != itemimp->GetRecyclType(nullptr))
		{
			continue;
		}

		const DecomposeCfg *decompose_cfg = GLOBALCONFIG->GetEquipConfig().GetEquipDecomposeCfg(RECYCL_XUANTIE);
		if (decompose_cfg == NULL)
		{
			return false;
		}

		ItemConfigData item = decompose_cfg->return_item;
		item.num = itemimp->GetRecyclGet(nullptr);

		// 检查背包空间是否足够
		const ItemBase *itembase = ITEMPOOL->GetItem(item.item_id);
		item.is_bind = m_grid_list[it.index].is_bind;
		int stack_num = itembase->GetPileLimit();
		int need_grid = (stack_num + item.num) / stack_num;

		if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_grid))
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return false;
		}

		auto item_it = std::find_if(put_item_list.begin(), put_item_list.end(), [item](const ItemConfigData &tmp_item) {
			return item.item_id == tmp_item.item_id && item.is_bind == tmp_item.is_bind;
		});
		if (put_item_list.end() != item_it)
		{
			item_it->num += item.num;
		}
		else
		{
			put_item_list.push_back(item);
		}

		it.is_succ = true;
	}

	// 给予
	{
		for (auto &it : put_item_list)
		{
			this->Put(it, PUT_REASON_EQUIPMENT_DECOMPOSE);
		}
	}

	// 消耗
	this->DoneDiscard(index_list, is_notice);

	return true;
}

bool Knapsack::DiscardRecycleMiyin(std::vector<KnapsackDiscardItem> &index_list, bool is_notice)
{
	if (index_list.empty())
	{
		return true;
	}

	std::vector<ItemConfigData> put_item_list;
	for (auto &it : index_list)
	{
		if (it.index < 0 || it.index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return false;

		ItemID item_id = m_grid_list[it.index].item_id;

		const ItemBase *itemimp = ITEMPOOL->GetItem(item_id);
		if (NULL == itemimp) return false;

		if (DISCARD_METHOD_RECYCL != it.discard_method || RECYCL_MIYIN != itemimp->GetRecyclType(nullptr))
		{
			continue;
		}

		const DecomposeCfg *decompose_cfg = GLOBALCONFIG->GetEquipConfig().GetEquipDecomposeCfg(RECYCL_MIYIN);
		if (decompose_cfg == NULL)
		{
			return false;
		}

		ItemConfigData item = decompose_cfg->return_item;
		item.num = itemimp->GetRecyclGet(nullptr);

		// 检查背包空间是否足够
		const ItemBase *itembase = ITEMPOOL->GetItem(item.item_id);
		item.is_bind = m_grid_list[it.index].is_bind;
		int stack_num = itembase->GetPileLimit();
		int need_grid = (stack_num + item.num) / stack_num;

		if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_grid))
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return false;
		}

		auto item_it = std::find_if(put_item_list.begin(), put_item_list.end(), [item](const ItemConfigData &tmp_item) {
			return item.item_id == tmp_item.item_id && item.is_bind == tmp_item.is_bind;
		});
		if (put_item_list.end() != item_it)
		{
			item_it->num += item.num;
		}
		else
		{
			put_item_list.push_back(item);
		}

		it.is_succ = true;
	}

	// 给予
	{
		for (auto &it : put_item_list)
		{
			this->Put(it, PUT_REASON_EQUIPMENT_DECOMPOSE);
		}
	}

	// 消耗
	this->DoneDiscard(index_list, is_notice);

	return true;
}

bool Knapsack::DiscardRecycleLittlePet(std::vector<KnapsackDiscardItem> &index_list, bool is_notice)
{
	if (index_list.empty())
	{
		return true;
	}

	std::vector<ItemConfigData> put_item_list;
	for (auto &it : index_list)
	{
		if (it.index < 0 || it.index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return false;

		ItemID item_id = m_grid_list[it.index].item_id;

		const ItemBase *itemimp = ITEMPOOL->GetItem(item_id);
		if (NULL == itemimp) return false;

		if (DISCARD_METHOD_RECYCL != it.discard_method || RECYCL_LITTLE_PET != itemimp->GetRecyclType(nullptr))
		{
			continue;
		}

		const LittlePetBaseCfg *base_cfg = LOGIC_CONFIG->GetLittlePetCfg().GetBaseCfgByItemID(item_id);
		if (NULL == base_cfg)
		{
			return false;
		}

		ItemConfigData item = base_cfg->return_item;
		item.num = base_cfg->return_item.num * it.discard_num;

		// 检查背包空间是否足够
		const ItemBase *itembase = ITEMPOOL->GetItem(item.item_id);
		item.is_bind = m_grid_list[it.index].is_bind;
		int stack_num = itembase->GetPileLimit();
		int need_grid = (stack_num + item.num) / stack_num;

		if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_grid))
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return false;
		}

		auto item_it = std::find_if(put_item_list.begin(), put_item_list.end(), [item](const ItemConfigData &tmp_item) {
			return item.item_id == tmp_item.item_id && item.is_bind == tmp_item.is_bind;
		});
		if (put_item_list.end() != item_it)
		{
			item_it->num += item.num;
		}
		else
		{
			put_item_list.push_back(item);
		}

		it.is_succ = true;
	}

	// 给予
	{
		for (auto &it : put_item_list)
		{
			this->Put(it, PUT_REASON_LITTLE_PET_EXCHANGE_ITEM);
		}
	}

	// 消耗
	this->DoneDiscard(index_list, is_notice);

	return true;
}

bool Knapsack::DiscardRecycleMarrylevelExp(std::vector<KnapsackDiscardItem> &index_list, bool is_notice)
{
	if (index_list.empty())
	{
		return true;
	}

	int add_marry_exp = 0;
	for (auto &it : index_list)
	{
		if (it.index < 0 || it.index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return false;

		ItemID item_id = m_grid_list[it.index].item_id;
		// 修正数量
		int discard_num = it.discard_num >= m_grid_list[it.index].num ? m_grid_list[it.index].num : it.discard_num;

		const ItemBase *itemimp = ITEMPOOL->GetItem(item_id);
		if (NULL == itemimp) return false;

		if (DISCARD_METHOD_RECYCL != it.discard_method || RECYCL_MARRY_LEVEL_EXP != itemimp->GetRecyclType(nullptr))
		{
			continue;
		}

		add_marry_exp += itemimp->GetRecyclGet(nullptr) * discard_num;
		it.is_succ = true;
	}

	// 给予
	{
		QingyuanGlobal * qingyuan_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_role->GetUID());
		if (NULL == qingyuan_global)
		{
			return false;
		}

		qingyuan_global->AddMarryLevelExp(m_role, add_marry_exp);
	}
	
	// 消耗
	this->DoneDiscard(index_list, is_notice);

	return true;
}

bool Knapsack::DiscardRecycleHunjieDecompose(std::vector<KnapsackDiscardItem> &index_list, bool is_notice)
{
	if (index_list.empty())
	{
		return true;
	}

	std::vector<ItemConfigData> put_item_list;
	for (auto &it : index_list)
	{
		if (it.index < 0 || it.index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return false;

		ItemID item_id = m_grid_list[it.index].item_id;

		const ItemBase *itemimp = ITEMPOOL->GetItem(item_id);
		if (NULL == itemimp) return false;

		if (DISCARD_METHOD_RECYCL != it.discard_method || RECYCL_HUNJIE_DECOMPOSE != itemimp->GetRecyclType(nullptr))
		{
			continue;
		}

		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return false;
		}

		const QingyuanOtherConfig &qingyuan_other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();

		ItemConfigData item = qingyuan_other_cfg.hunjie_decompose_item;
		item.item_id = qingyuan_other_cfg.hunjie_decompose_item.item_id;
		item.num = qingyuan_other_cfg.hunjie_decompose_item.num;
		item.is_bind = m_grid_list[it.index].is_bind;

		auto item_it = std::find_if(put_item_list.begin(), put_item_list.end(), [item](const ItemConfigData &tmp_item) {
			return item.item_id == tmp_item.item_id && item.is_bind == tmp_item.is_bind;
		});
		if (put_item_list.end() != item_it)
		{
			item_it->num += item.num;
		}
		else
		{
			put_item_list.push_back(item);
		}

		it.is_succ = true;
	}

	for (auto &it : put_item_list)
	{
		this->Put(it, PUT_REASON_HUNJIE_DECOMPOSE);
	}

	// 消耗
	this->DoneDiscard(index_list, is_notice);

	return true;
}

bool Knapsack::DiscardRecyclePetScore(std::vector<KnapsackDiscardItem> &index_list, bool is_notice)
{
	if (index_list.empty())
	{
		return true;
	}

	int score = 0;
	std::vector<ItemConfigData> put_item_list;
	for (auto &it : index_list)
	{
		if (it.index < 0 || it.index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return false;

		ItemID item_id = m_grid_list[it.index].item_id;
		// 修正数量
		int discard_num = it.discard_num >= m_grid_list[it.index].num ? m_grid_list[it.index].num : it.discard_num;

		const ItemBase *itemimp = ITEMPOOL->GetItem(item_id);
		if (NULL == itemimp) return false;

		if (DISCARD_METHOD_RECYCL != it.discard_method || RECYCL_LITTLE_PET_SCORE != itemimp->GetRecyclType(nullptr))
		{
			continue;
		}

		LittlePet *little_pet = GlobalRoleManager::Instance().GetLittlePet(m_role->GetUID());
		if (nullptr == little_pet) return false;

		score += itemimp->GetRecyclGet(nullptr) * discard_num;

		// 如果是装备
		if (ItemBase::I_TYPE_EQUIPMENT == itemimp->GetItemType())
		{
			Equipment *little_equip = (Equipment *)itemimp;
			if (NULL == little_equip || (Equipment::E_TYPE_SPECIAL_LITTLEPET != little_equip->GetEquipType() &&
				(little_equip->GetEquipType() < Equipment::E_TYPE_LITTLE_PET_MIN || little_equip->GetEquipType() >= Equipment::E_TYPE_LITTLE_PET_MAX)))
			{
				return false;
			}

			// 小宠物的回收需要返还喂养的材料
			EquipmentParam *equip_param = (EquipmentParam*)m_grid_list[it.index].param;
			if (NULL == equip_param)
			{
				return false;
			}

			std::vector<ItemConfigData> tmp_item_list;
			little_pet->RecycleReturnStuffList(equip_param->param1, item_id, tmp_item_list);  // param1用于存储小宠物喂养等级

			for (auto &tmp_it : tmp_item_list)
			{
				auto item_it = std::find_if(put_item_list.begin(), put_item_list.end(), [tmp_it](const ItemConfigData &tmp_item) {
					return tmp_it.item_id == tmp_item.item_id && tmp_it.is_bind == tmp_item.is_bind;
				});
				if (put_item_list.end() != item_it)
				{
					item_it->num += tmp_it.num;
				}
				else
				{
					put_item_list.push_back(tmp_it);
				}
			}
		}

		it.is_succ = true;
	}

	// 给予
	{
		LittlePet *little_pet = GlobalRoleManager::Instance().GetLittlePet(m_role->GetUID());
		if (nullptr != little_pet)
		{
			little_pet->AddScore(score, "Knapsack::Discard");
		}

		for (auto &it : put_item_list)
		{
			this->Put(it, PUT_REASON_RECYCLE_LITTLEPET_RETURN_FEED_STUFF);
		}
	}

	// 消耗
	this->DoneDiscard(index_list, is_notice);

	return true;
}

bool Knapsack::DiscardRecycleHunli(std::vector<KnapsackDiscardItem> &index_list, bool is_notice)
{
	if (index_list.empty())
	{
		return true;
	}

	int add_exp = 0;
	// 筛选
	for (auto &it : index_list)
	{
		if (it.index < 0 || it.index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return false;

		ItemID item_id = m_grid_list[it.index].item_id;

		const ItemBase *itemimp = ITEMPOOL->GetItem(item_id);
		if (NULL == itemimp) return false;

		if (DISCARD_METHOD_RECYCL != it.discard_method || RECYCL_HUNLI != itemimp->GetRecyclType(nullptr))
		{
			continue;
		}

		EquipmentParam *equip_param = (EquipmentParam *)m_grid_list[it.index].param;
		if (equip_param != NULL)
		{
			add_exp += m_role->GetLieMingMgr()->GetDecomposeExp(itemimp->GetColor(), equip_param->strengthen_level);	//返还前面升级消耗的经验
		}

		it.is_succ = true;
	}

	// 给予
	m_role->GetLieMingMgr()->AddExp(add_exp);

	// 消耗
	this->DoneDiscard(index_list, is_notice);

	return true;
}

bool Knapsack::DiscardRecycleChuanshiFragment(std::vector<KnapsackDiscardItem> &index_list, bool is_notice)
{
	if (index_list.empty())
	{
		return true;
	}

	int add_score = 0;
	// 筛选
	for (auto &it : index_list)
	{
		if (it.index < 0 || it.index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return false;

		ItemID item_id = m_grid_list[it.index].item_id;

		// 修正数量
		int discard_num = it.discard_num >= m_grid_list[it.index].num ? m_grid_list[it.index].num : it.discard_num;

		const ItemBase *itemimp = ITEMPOOL->GetItem(item_id);
		if (NULL == itemimp) return false;

		if (DISCARD_METHOD_RECYCL != it.discard_method || RECYCL_CHUANSHI_FRAGMENT != itemimp->GetRecyclType(nullptr))
		{
			continue;
		}

		add_score += itemimp->GetRecyclGet(NULL) * discard_num;

		it.is_succ = true;
	}

	// 给予
	m_role->GetCrossEquip()->AddChuanshiFragment(add_score);

	// 消耗
	this->DoneDiscard(index_list, is_notice);

	return true;
}

bool Knapsack::Move(short from_index, short to_index)
{
	if (from_index == to_index) return true;
	if (from_index < 0 || from_index >= MAX_KNAPSACK_STORAGE_GRID_NUM || to_index < 0 || to_index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return false;

	if (m_is_lock)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (to_index >= m_curr_knapsack_valid_num && to_index < MAX_KNAPSACK_GRID_NUM) // 在背包未开启范围内，出错
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}
						
	if (to_index >= MAX_KNAPSACK_GRID_NUM + m_curr_storage_valid_num && to_index < MAX_KNAPSACK_STORAGE_GRID_NUM) // 在仓库未开启范围内，出错
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_STORAGE_NO_SPACE);
		return false;	
	}

	if (from_index < MAX_KNAPSACK_GRID_NUM && to_index >= STORAGE_INDEX_BEGIN)
	{
		if (m_grid_list[from_index].invalid_time > 0)
		{
			m_role->NoticeNum(errornum::EN_ITEM_TIME_NO_IN_STORAGE);
			return false;
		}
	}

	ItemID item_id = m_grid_list[from_index].item_id;
	int item_num = m_grid_list[from_index].num;

	if (!this->MoveHelper(from_index, to_index)) 
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_MOVE_ERROR);
		return false;
	}

	if (from_index < MAX_KNAPSACK_GRID_NUM && to_index >= STORAGE_INDEX_BEGIN) // 从背包放到仓库 触发失去
	{
		EventHandler::Instance().OnGetItem(m_role, item_id, -item_num);
	}

	if (from_index >= STORAGE_INDEX_BEGIN && to_index < MAX_KNAPSACK_GRID_NUM) // 从仓库拿出 触发得到
	{
		EventHandler::Instance().OnGetItem(m_role, item_id, item_num);
	}

	return true;
}

bool Knapsack::Split(short src_index, short num)
{
	if (num <= 0 || src_index < 0 || src_index >= ItemNamespace::MAX_KNAPSACK_GRID_NUM) return false;	// 仓库里不可拆分

	if (m_is_lock)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (m_grid_list[src_index].Invalid()) return false;

	const ItemBase *item = ITEMPOOL->GetItem(m_grid_list[src_index].item_id);
	if (NULL == item || item->GetItemType() == ItemBase::I_TYPE_EQUIPMENT || NULL != m_grid_list[src_index].param)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_SPLIT_CANNOT);
		return false;
	}

	if (m_grid_list[src_index].num <= num)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_SPLIT_NUM_ERROR);
		return false;
	}

	short to_index = 0;
	for (; to_index < m_curr_knapsack_valid_num; ++to_index)
	{
		if (m_grid_list[to_index].Invalid()) break;
	}

	if (to_index >= m_curr_knapsack_valid_num)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	int split_gold_price = 0;
	if (m_grid_list[src_index].gold_price > 0)
	{
		split_gold_price = m_grid_list[src_index].gold_price - ItemExtern::GetGoldPriceByNum(m_grid_list[src_index].item_id, 
			m_grid_list[src_index].num, m_grid_list[src_index].gold_price, m_grid_list[src_index].num - num);
		if (split_gold_price > m_grid_list[src_index].gold_price) split_gold_price = m_grid_list[src_index].gold_price;

		m_grid_list[src_index].gold_price -= split_gold_price;
	}

	m_grid_list[src_index].num -= num;

	ItemDataWrapper item_wrapper(m_grid_list[src_index].item_id, num, m_grid_list[src_index].is_bind);
	item_wrapper.invalid_time = m_grid_list[src_index].invalid_time; // 有失效时间的物品允许拆分 不允许合并
	item_wrapper.gold_price = split_gold_price;
	
	m_grid_list[to_index].Set(item_wrapper);

	this->RefreshValidGridIndexEnd(to_index >= src_index ? to_index : src_index);

	m_dirty_mark[src_index] = true;
	m_dirty_mark[to_index] = true;
	m_knapsack_is_change = true;

	this->SendknapsackGridInfoToRole(src_index, ITEM_CHANGE_TYPE_CHANGE);
	this->SendknapsackGridInfoToRole(to_index, src_index);

	return true;
}

bool Knapsack::InOrder(bool is_storage, bool ignore_bind)
{
	const UInt32 sortinterval = GLOBALCONFIG->GetOtherConfig_SortInOrder_Interval();
	UInt32 now_time = static_cast<UInt32>(EngineAdapter::Instance().Time()); // 背包/仓库整理不能频繁调用

	if (ignore_bind)
	{
		if (now_time < m_last_inorder_merge_time + sortinterval) return false;
		m_last_inorder_merge_time = now_time;
	}
	else
	{
		if (is_storage)
		{
			if (now_time < m_storage_last_inorder_time + sortinterval) return false;
			m_storage_last_inorder_time = now_time;
		}
		else
		{
			if (now_time < m_knapsack_last_inorder_time + sortinterval) return false;
			m_knapsack_last_inorder_time = now_time;
		}
	}

	if (m_is_lock)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	int to_check_max_num = -1;
	if (is_storage) 
	{
		int tmp_i = MAX_KNAPSACK_GRID_NUM + m_curr_storage_valid_num - 1;
		for (; tmp_i >= MAX_KNAPSACK_GRID_NUM; tmp_i--)
		{
			if (!m_grid_list[tmp_i].Invalid()) break;
		}
		to_check_max_num = tmp_i + 1;
	}
	else
	{
		to_check_max_num = m_valid_grid_index_end;
	}

	int begin = (is_storage) ? MAX_KNAPSACK_GRID_NUM : 0;
	
	bool empty_mark[MAX_KNAPSACK_OR_STOREAGE_ARRAY_SIZE] = {0}; // 标记排序前在begin ~ begin + check_num范围内哪些格子是空的

	if (to_check_max_num - begin > MAX_KNAPSACK_OR_STOREAGE_ARRAY_SIZE) return false;

	for (int i = begin ; i < to_check_max_num; i++)
	{
		empty_mark[i - begin] = m_grid_list[i].Invalid();
	}

	int merge_result = -1;
	int rest_num = -1;
	for (int i = to_check_max_num - 1; i >= begin; i--) // 先将所有能合并的都合并起来
	{
		const ItemBase *itembase = ITEMPOOL->GetItem(m_grid_list[i].item_id);
		if (NULL == itembase) continue;

		if (!m_grid_list[i].CanBeMerge()) continue;

		for (int j = begin; j < i; j++)
		{
			int merged_gold_price = 0;
			merge_result = m_grid_list[i].Merge(m_grid_list[j], &rest_num, &merged_gold_price);
			if (ITEM_MERGE_CANNOT != merge_result)
			{
				if (ITEM_MERGE_COMPLETE == merge_result && 0 == rest_num)
				{
					m_grid_list[j].Reset();
				}
				else
				{
					m_grid_list[j].num = rest_num;
					m_grid_list[j].gold_price -= merged_gold_price;
				}

				this->SetItemDirty(i, false);
				this->SetItemDirty(j, false);

				if (m_grid_list[i].num >= itembase->GetPileLimit()) break;
			}
		}
	}

	if (ignore_bind)	// 把非绑合到绑定里面
	{
		for (int i = to_check_max_num - 1; i >= begin; i--)
		{
			const ItemBase *itembase = ITEMPOOL->GetItem(m_grid_list[i].item_id);
			if (NULL == itembase) continue;

			if (!m_grid_list[i].is_bind || !m_grid_list[i].CanBeMerge()) continue;

			for (int j = begin; j <= to_check_max_num - 1; j++)
			{
				if (i == j || m_grid_list[j].is_bind) continue;

				int merged_gold_price = 0;
				merge_result = m_grid_list[i].Merge(m_grid_list[j], &rest_num, &merged_gold_price, true);
				if (ITEM_MERGE_CANNOT != merge_result)
				{
					if (ITEM_MERGE_COMPLETE == merge_result && 0 == rest_num)
					{
						m_grid_list[j].Reset();
					}
					else
					{
						m_grid_list[j].num = rest_num;
						m_grid_list[j].gold_price -= merged_gold_price;
					}

					this->SetItemDirty(i, false);
					this->SetItemDirty(j, false);

					break;
				}
			}
		}
	}

	static SortStruct sort_stru[MAX_KNAPSACK_OR_STOREAGE_ARRAY_SIZE];
	memset(sort_stru, 0, sizeof(sort_stru));

	int count = 0;

	for (int i = begin; i < to_check_max_num; i++)
	{
		if (!m_grid_list[i].Invalid())
		{
			const ItemBase *itembase = ITEMPOOL->GetItem(m_grid_list[i].item_id);
			if (NULL == itembase) continue;

			if (count >= MAX_KNAPSACK_OR_STOREAGE_ARRAY_SIZE) return false;

			sort_stru[count].Set(m_grid_list[i].item_id * 10000 + m_grid_list[i].num, i, m_grid_list[i].is_bind);
			
			++ count;
		}
	}

	if (count > MAX_KNAPSACK_OR_STOREAGE_ARRAY_SIZE) return false;

	qsort(sort_stru, count, sizeof(SortStruct), SortCompare);

	static ItemGridData tmp_grid_list[MAX_KNAPSACK_OR_STOREAGE_ARRAY_SIZE];

	if (!is_storage) m_valid_grid_index_end = count;

	for (int i = 0; i < count; i++) // 将排序后的物品存到临时空间里
	{
		int src_index = sort_stru[i].src_index;
		tmp_grid_list[i].Set(m_grid_list[src_index]);
	}

	for (int i = 0; i < count; i++) // 从临时空间拷贝回来
	{
		m_grid_list[begin + i].Set(tmp_grid_list[i]);
		this->SetItemDirty(begin + i, false);
	}

	for (int i = count; i < to_check_max_num - begin; i++) // 将后面的全部设为空
	{
		if (empty_mark[i]) continue;

		m_grid_list[begin + i].Reset();
		this->SetItemDirty(begin + i, false);
	}
	
	this->SendKnapsackInfoToRole(); 

	return true;
}

bool Knapsack::TakeOutAllItemFromStorage()
{
	if (m_is_lock)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	int end_index = m_curr_storage_valid_num < MAX_STORAGE_GRID_NUM ? STORAGE_INDEX_BEGIN + m_curr_storage_valid_num : STORAGE_INDEX_BEGIN + MAX_STORAGE_GRID_NUM;

	for (int i = STORAGE_INDEX_BEGIN; i < end_index; ++i)
	{
		if (m_grid_list[i].Invalid()) continue;
		if (!this->HaveSpace()) break;

		static ItemDataWrapper item_wrapper;

		m_grid_list[i].WrapTo(&item_wrapper);

		this->CertainPut(item_wrapper, PUT_REASON_NO_NOTICE, NULL);

		m_grid_list[i].Clear();

		this->SetItemDirty(i);
	}

	return true;
}

#define PUT_HELPER(item_wrapper, drop_monster_id) \
if (m_is_lock)\
{\
	m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);\
	return false;\
}\
if (item_wrapper.Invalid()) return false;\
const ItemBase *item = ITEMPOOL->GetItem(item_wrapper.item_id);\
if (NULL == item) \
{\
	m_role->NoticeNum(errornum::EN_KNAPSACK_PUT_ERROR);\
	return false;\
}\
this->CertainPut(item_wrapper, reason_type, out_index, drop_monster_id);

bool Knapsack::Put(const ItemConfigData &item_config_data, int reason_type, int *out_index, UInt16 drop_monster_id, int gold_price)
{
	const ItemBase *itembase = ITEMPOOL->GetItem(item_config_data.item_id);
	if (NULL == itembase) return false;

	static ItemDataWrapper temp_item_wrapper;
	temp_item_wrapper.item_id = item_config_data.item_id;
	temp_item_wrapper.num = (short)item_config_data.num;
	temp_item_wrapper.is_bind = (itembase->IsBind() || item_config_data.is_bind) ? 1 : 0;
	temp_item_wrapper.invalid_time = itembase->CalInvalidTime();
	temp_item_wrapper.gold_price = gold_price;
	PUT_HELPER(temp_item_wrapper, drop_monster_id);
	return true;
}

bool Knapsack::Put(const ItemDataWrapper &item_wrapper, int reason_type, int *out_index, UInt16 drop_monster_id)
{
	PUT_HELPER(item_wrapper, drop_monster_id);
	return true;
}

#define PUT_LIST_HELPER(item_list) \
if (NULL == item_list || list_num <= 0 || list_num > MAX_KNAPSACK_GRID_NUM) return false;\
if (m_is_lock)\
{\
	m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);\
	return false;\
}\
for (int i = 0; i < list_num; ++i)\
{\
	if (item_list[i].num <= 0)\
	{\
		m_role->NoticeNum(errornum::EN_KNAPSACK_PUT_ERROR);\
		return false;\
	}\
	const ItemBase *itembase = ITEMPOOL->GetItem(item_list[i].item_id);\
	if (NULL == itembase) \
	{\
		m_role->NoticeNum(errornum::EN_KNAPSACK_PUT_ERROR);\
		return false;\
	}\
}

bool Knapsack::PutList(short list_num, const ItemConfigData *item_configdata_list, short reason_type, int *out_index_list)
{
	PUT_LIST_HELPER(item_configdata_list);

	if (this->CheckEmptyGridNoLessThan(list_num))
	{
		static ItemDataWrapper temp_item_wrapper;
		for (int i = 0; i < list_num; ++i)
		{
			const ItemBase *itembase = ITEMPOOL->GetItem(item_configdata_list[i].item_id);
			if (NULL == itembase) return false;

			int *out_index = (NULL == out_index_list) ? NULL : &out_index_list[i];

			temp_item_wrapper.item_id = item_configdata_list[i].item_id;
			temp_item_wrapper.num = (short)item_configdata_list[i].num;
			temp_item_wrapper.is_bind = (itembase->IsBind() || item_configdata_list[i].is_bind) ? 1 : 0;
			temp_item_wrapper.invalid_time = itembase->CalInvalidTime();
			this->CertainPut(temp_item_wrapper, reason_type, out_index);
		}
	}
	else
	{
		if (CrossConfig::Instance().IsHiddenServer())
		{
			gamecommon::SendMultiMailToCrossRole(m_role->GetRoleCross()->GetCrossOriginalUniqueUserID(), item_configdata_list, list_num, NULL, true, gamestring::g_knapsack_put_content);
		}
		else
		{
			gamecommon::SendMultiMailToRole(m_role->GetUserId(), item_configdata_list, list_num, NULL, true, gamestring::g_knapsack_put_content);
		}

		gstr::SendError(m_role, "knapsack_full_and_send_mail");
	}

	return true;
}

bool Knapsack::PutList(short list_num, const ItemDataWrapper *item_wrapper_list, short reason_type, int *out_index_list)
{
	PUT_LIST_HELPER(item_wrapper_list);

	if (!this->CheckEmptyGridNoLessThan(list_num))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	for (int i = 0; i < list_num; ++i)
	{
		int *out_index = (NULL == out_index_list) ? NULL : &out_index_list[i];

		this->CertainPut(item_wrapper_list[i], reason_type, out_index);
	}

	return true;
}

bool Knapsack::PutOrMail(const ItemConfigData &item_configdata, short reason_type)
{
	if (this->HaveSpace())
	{
		return this->Put(item_configdata, reason_type);
	}

	const ItemBase *itembase = ITEMPOOL->GetItem(item_configdata.item_id);
	if (NULL == itembase) return false;

	static MailContentParam contentparam; contentparam.Reset();
	contentparam.item_list[0].item_id = item_configdata.item_id;
	contentparam.item_list[0].is_bind = (short)item_configdata.is_bind;
	contentparam.item_list[0].num = item_configdata.num;
	contentparam.item_list[0].invalid_time = itembase->CalInvalidTime();

	int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_knapsack_put_subiect);
	int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_knapsack_put_content);
	if (length1 > 0 && length2 > 0)
	{
		MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
		gstr::SendError(m_role, "knapsack_full_and_send_mail");
		return true;
	}

	return false;
}

bool Knapsack::PutListOrMail(const ItemConfigData item_configdata_list[MAX_ATTACHMENT_ITEM_NUM], short reason_type)
{
	if (m_is_lock)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	int real_count = 0;
	for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
	{
		if (0 != item_configdata_list[i].item_id && item_configdata_list[i].num > 0)
		{
			const ItemBase *itembase = ITEMPOOL->GetItem(item_configdata_list[i].item_id);
			if (NULL == itembase) return false;

			++ real_count;
		}
	}

	if (this->CheckEmptyGridNoLessThan(real_count))
	{
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			if (0 != item_configdata_list[i].item_id && item_configdata_list[i].num > 0) 
			{
				this->Put(item_configdata_list[i], reason_type);
			}
		}
	}
	else
	{
		static MailContentParam contentparam; contentparam.Reset();
		
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			contentparam.item_list[i].Reset();
		}

		int attach_num = 0;

		for (int i = 0; attach_num < MAX_ATTACHMENT_ITEM_NUM && i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			if (0 != item_configdata_list[i].item_id && item_configdata_list[i].num > 0) 
			{
				const ItemBase *itembase = ITEMPOOL->GetItem(item_configdata_list[i].item_id);
				if (NULL == itembase) return false;

				contentparam.item_list[attach_num].item_id = item_configdata_list[i].item_id;
				contentparam.item_list[attach_num].is_bind = (short)item_configdata_list[i].is_bind;
				contentparam.item_list[attach_num].num = item_configdata_list[i].num;
				contentparam.item_list[attach_num].invalid_time = itembase->CalInvalidTime();

				++ attach_num;
			}
		}		

		int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_knapsack_put_subiect);
		int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_knapsack_put_content);
		if (length1 > 0 && length2 > 0)
		{
			MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
			gstr::SendError(m_role, "knapsack_full_and_send_mail");
			return true;
		}
	}

	return true;
}

bool Knapsack::PutOrSendToOtherSystem(const ItemConfigData &item_configdata, short reason_type, int *out_index)
{
	const ItemBase* item_base = ITEMPOOL->GetItem(item_configdata.item_id);
	if (NULL == item_base) return false;

	short use_type = NorexItem::I_NOREX_ITEM_INVALID;

	const NorexItem* norex_item_cfg = dynamic_cast<const NorexItem*>(item_base);
	if (NULL != norex_item_cfg)
	{
		use_type = norex_item_cfg->GetUseType();
	}

	bool ret = false;
	switch (use_type)
	{
	case NorexItem::I_NOREX_ITEM_RUNE:
	{
		ret = m_role->GetRuneSystem()->AddRuneByItemId(item_configdata.item_id, item_configdata.num);
	}
	break;
	case NorexItem::I_NOREX_ITEM_PUT_SHENGE:
	{
		ret = m_role->GetShengeSystem()->AddShengeByItemIdByColor(item_base->GetColor(), item_configdata.item_id, item_configdata.num);
	}
	break;
	case NorexItem::I_NOREX_ITEM_ZODIAC:
	{
		ret = m_role->GetZodiac()->AddItemsToBackpack(item_configdata.item_id, item_configdata.num);
	}
	break;
	default:break;
	}
	

	if (ret)
	{
		return true;
	}
	else // 上面的操作没有成功，则还是要put到背包里，增加容错率
	{
		return this->Put(item_configdata, reason_type, out_index);
	}
}

void Knapsack::ChangeAllItemToOtherSex()
{
	for (int i = 0; i < ItemNamespace::MAX_GRID_NUM; i++)
	{
		if (!m_grid_list[i].Invalid())
		{
			const ItemBase *itembase = ITEMPOOL->GetItem(m_grid_list[i].item_id);
			if (NULL != itembase && 0 != itembase->GetOtherSexItemId())
			{
				const ItemBase *other_sex_item = ITEMPOOL->GetItem(itembase->GetOtherSexItemId());
				if (NULL != other_sex_item) 
				{
					m_grid_list[i].item_id = other_sex_item->GetItemId();
					this->SetItemDirty(i);
				}
			}
		}
	}
}

void Knapsack::OnChangeProf()
{
	for (int i = 0; i < ItemNamespace::MAX_GRID_NUM; i++)
	{
		if (!m_grid_list[i].Invalid())
		{
			const ItemBase *itembase = ITEMPOOL->GetItem(m_grid_list[i].item_id);
			if (NULL != itembase)
			{
				ItemID change_itemid = 0;
				if (LOGIC_CONFIG->GetZhuanzhiCfg().GetChangeItemID(itembase->GetItemId(), m_role->GetProf() % PROF_TYPE_PROF_ZHUANZHI_BASE, &change_itemid))
				{
					const ItemBase *change_item = ITEMPOOL->GetItem(change_itemid);
					if (NULL != change_item) 
					{
						m_grid_list[i].item_id = change_item->GetItemId();
						this->SetItemDirty(i);
					}
				}
			}
		}
	}
}

int Knapsack::Count(ItemID item_id)
{
	if (ItemBase::INVALID_ITEM_ID == item_id) return 0;

	int count = 0;

	for (int i = 0; i < m_valid_grid_index_end; ++i)
	{
		if (!m_grid_list[i].Invalid() && m_grid_list[i].item_id == item_id)
		{
			count += m_grid_list[i].num;
		}
	}

	return count;
}

bool Knapsack::CheckEmptyGridNoLessThan(int check_num)
{
	if (check_num < 0 || m_is_lock) return false;		// 这里要加上交易锁的检查 防止交易时放入背包失败又没有处理的情况
	if (0 == check_num) return true;

	if (m_curr_knapsack_valid_num - m_valid_grid_index_end >= check_num) return true;
	
	int count = 0;

	for (int i = 0; i < m_curr_knapsack_valid_num; ++i)
	{
		if (m_grid_list[i].Invalid())
		{
			if (++ count >= check_num) return true;
		}
	}

	return false;
}

short Knapsack::GetCurrKnapscakEmptyGridNum()
{
	int count = 0;

	for (int i = 0; i < m_curr_knapsack_valid_num; ++i)
	{
		if (m_grid_list[i].Invalid())
		{
			++ count;
		}
	}

	return count;
}

bool Knapsack::HasItem(ItemID item_id)
{
	if (m_is_lock)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (ItemBase::INVALID_ITEM_ID == item_id) return false;

	for (int i = 0; i < m_valid_grid_index_end; ++i)
	{
		if (m_grid_list[i].item_id == item_id) return true;
	}

	return false;
}

bool Knapsack::ExtendValidNum(bool can_use_gold, bool is_storage, short num, bool is_auto_extend)
{
	if (num <= 0) return false;

	if (m_role->GetSublock()->CheckHasLock()) return false;

	short *curr_grid_num = is_storage ? &m_curr_storage_valid_num : &m_curr_knapsack_valid_num;
	int max_num = is_storage ? MAX_STORAGE_GRID_NUM : MAX_KNAPSACK_GRID_NUM;

	if (*curr_grid_num >= max_num)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_STORAGE_EXTEND_MAX);
		return false;
	}

	if (*curr_grid_num + num > max_num)
	{
		num = max_num - *curr_grid_num;
	}

	// 检查消耗
	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	ItemID consume_item_id = (is_storage ? GLOBALCONFIG->GetOtherConfig_KnapsackExtendStorageItemID() : GLOBALCONFIG->GetOtherConfig_KnapsackExtendBagItemID());

	if (is_auto_extend)
	{
		++m_valid_grid_auto_add_count;
	}
	else
	{
		int total_need_item_num = 0;
		{
			for (int i = 0; i < num; ++ i)
			{
				int extend_index = *curr_grid_num + i;
				int need_item_count = GLOBALCONFIG->GetOtherConfig_KnapsackExtendNeedItemCount(extend_index, is_storage);

				if (need_item_count <= 0)
				{
					return false;
				}

				total_need_item_num += need_item_count;
			} 
		}

		{
			ItemExtern::ItemConsumeConfig comsume_stuff;
			comsume_stuff.item_id = consume_item_id;
			comsume_stuff.num = total_need_item_num;
			comsume_stuff.buyable = can_use_gold;

			if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, &comsume_stuff, &consume_list))
			{
				m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
				return false;
			}

			/*if (!m_money.GoldMoreThan(consume_list.need_gold_num))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return false;
			}*/
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "KnapsackStorageExtendGridNum"))
		{
			return false;
		}
	}

	*curr_grid_num += num;

	if (is_storage)
	{
		Protocol::SCStorageMaxGridNum smgn;
		smgn.max_grid_num = *curr_grid_num;
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&smgn, sizeof(smgn));
	}
	else
	{
		Protocol::SCKnapsackMaxGridNum kmgn;
		kmgn.max_grid_num = *curr_grid_num;
		kmgn.is_auto_extend = is_auto_extend;
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&kmgn, sizeof(kmgn));
	}

	if (!is_storage && is_auto_extend)
	{
		this->SendKnapsackGridExtendAuto();
	}

	gamelog::g_log_knapsack.printf(LL_INFO, "ExtendValidNum user[%d, %s], extern %s, extern_num[%d], curr_grid_num[%d], consume_all_gold[%d], consume_item[id:%d, num:%d]",
		m_role->GetUID(), m_role->GetName(), (is_storage ? "storage" : "knapsack"), num, *curr_grid_num, consume_list.need_gold_num,
		consume_item_id, consume_list.bind_item_num + consume_list.nobind_item_num);

	return true;
}
void Knapsack::VipLevelExtendValidNum(Role * role)
{
	short level_grid_num =  LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_VIP_OPEN_KNAPSACK_GRID);
	if (level_grid_num >= MAX_KNAPSACK_GRID_NUM|| level_grid_num <= 0)
	{
		return;
	}

	m_curr_knapsack_valid_num = level_grid_num;

	Protocol::SCKnapsackMaxGridNum kmgn;
	kmgn.max_grid_num = m_curr_knapsack_valid_num;
	kmgn.is_auto_extend = 0;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&kmgn, sizeof(kmgn));
	
	gamelog::g_log_knapsack.printf(LL_INFO, "ExtendValidNum user[%d, %s], extern %s, vip_level[%d],curr_grid_num[%d]",m_role->GetUID(), m_role->GetName(),"VIPExtend", 
		role->GetVip()->GetVipLevel(),m_curr_knapsack_valid_num);
}
void Knapsack::ClearAll()
{
	if (m_role->GetSublock()->CheckHasLock()) return;

	static short consume_index_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
	static short consume_num_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];

	int count = 0;
	for (int i = 0; i < m_valid_grid_index_end; i++)
	{
		if (m_grid_list[i].Invalid()) continue;

		consume_index_list[count] = i;
		consume_num_list[count] = m_grid_list[i].num;
		count ++;
	}

	this->ConsumeItemByIndexList(count, consume_index_list, consume_num_list, "ClearAll");
}

bool Knapsack::ConsumeItem(ItemID item_id, const int consume_num, const char *reason, bool need_audit_log)
{
	if (consume_num < 0 || item_id == ItemBase::INVALID_ITEM_ID) return false;
	if (0 == consume_num) return true;

	if (m_is_lock)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
	if (NULL == itembase) return false;

	if (itembase->GetItemType() == ItemBase::I_TYPE_EQUIPMENT && consume_num > 1)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_USE_ITEM_NUM_NOT_ENOUGH);
		return false;
	}

	int index_list[MAX_KNAPSACK_GRID_NUM] = {0};

	int index_num = 0;
	int count = 0;

	for (int i = 0; i < m_valid_grid_index_end; ++i)
	{
		if (m_grid_list[i].Invalid()) continue; 

		if (m_grid_list[i].item_id == item_id) 
		{
			count += m_grid_list[i].num;
			index_list[index_num] = i;
			++ index_num;

			if (count >= consume_num) break;
		}
	}

	if (count < consume_num) return false;

	const char *equip_data_string = NULL;
	const char *item_param_string = NULL;

	int total_num = consume_num;

	for (int i = 0; i < index_num; ++i)
	{
		int index = index_list[i];

		int consume_num = (total_num <= m_grid_list[index].num) ? total_num : m_grid_list[index].num;
		total_num -= consume_num;

		if (m_grid_list[index].gold_price > 0)
		{
			int consume_gold_price = ItemExtern::GetGoldPriceByNum(m_grid_list[index].item_id, m_grid_list[index].num, m_grid_list[index].gold_price, consume_num);
			if (consume_gold_price > 0)
			{
				if (consume_gold_price > m_grid_list[index].gold_price) consume_gold_price = m_grid_list[index].gold_price;

				m_grid_list[index].gold_price -= consume_gold_price;
			}
		}

		equip_data_string = ItemGridDataLog(itembase, m_grid_list[index]);
		item_param_string = ItemGridQuickLog(itembase, m_grid_list[index]);

		m_grid_list[index].num -= consume_num;
		if (m_grid_list[index].num <= 0)
		{
			m_grid_list[index].Clear();
		}

		this->SetItemDirty(index);

		if (total_num <= 0) break;
	}

	if (NULL != equip_data_string)
	{
		gamelog::g_log_knapsack.printf(LL_INFO, "[Knapsack::ConsumeEquipment Succ][user[%s %d] equipment_data:%s]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), equip_data_string);

		ROLE_LOG_QUICK6(LOG_TYPE_ITEM_CONSUME, m_role, item_id, consume_num, reason, item_param_string);
	}
	else if (itembase->IsRecord())
	{
		gamelog::g_log_knapsack.printf(LL_INFO,"[Knapsack::ConsumeItem Succ][user[%s %d] item_name:%s item_id:%d consume_num:%d reason:%s]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), itembase->GetItemName(), (int)item_id, consume_num, NULL == reason ? "" : reason);

		ROLE_LOG_QUICK6(LOG_TYPE_ITEM_CONSUME, m_role, item_id, consume_num, reason, NULL);
	}

	if (index_num >= 1) this->RefreshValidGridIndexEnd(index_list[index_num - 1]);

	EventHandler::Instance().OnGetItem(m_role, item_id, -consume_num);

	return true;
}

bool Knapsack::ConsumeItemByIndex(int knapsack_index, int num, const char *reason, bool is_real_consume, bool need_audit_log)
{
	if (num < 0) return false;
	if (knapsack_index < 0 || knapsack_index >= MAX_KNAPSACK_GRID_NUM || m_grid_list[knapsack_index].Invalid()) return false;
	if (0 == num) return true; 

	if (m_is_lock)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (m_grid_list[knapsack_index].num < num)
	{
		return false;
	}

	ItemID item_id = m_grid_list[knapsack_index].item_id;
	const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
	if (NULL == itembase)
	{
		return false;
	}

	if (!is_real_consume) return true;

	if (m_grid_list[knapsack_index].gold_price > 0)
	{
		int consume_gold_price = ItemExtern::GetGoldPriceByNum(item_id, m_grid_list[knapsack_index].num, m_grid_list[knapsack_index].gold_price, num);
		if (consume_gold_price > 0)
		{
			if (consume_gold_price > m_grid_list[knapsack_index].gold_price) consume_gold_price = m_grid_list[knapsack_index].gold_price;

			m_grid_list[knapsack_index].gold_price -= consume_gold_price;
		}
	}

	bool item_is_bind = m_grid_list[knapsack_index].is_bind;

	const char *equip_data_string = ItemGridDataLog(itembase, m_grid_list[knapsack_index]);
	const char *item_param_string = ItemGridQuickLog(itembase, m_grid_list[knapsack_index]);

	m_grid_list[knapsack_index].num -= num;

	if (m_grid_list[knapsack_index].num <= 0)
	{
		m_grid_list[knapsack_index].Clear();
		this->RefreshValidGridIndexEnd(knapsack_index);
	}

	this->SetItemDirty(knapsack_index);

	EventHandler::Instance().OnGetItem(m_role, item_id, -num);

	if (NULL != equip_data_string)
	{
		gamelog::g_log_knapsack.printf(LL_INFO, "[Knapsack::ConsumeEquipmentByIndex Succ][user[%s %d] equipment_data:%s]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), equip_data_string);

		ROLE_LOG_QUICK6(LOG_TYPE_ITEM_CONSUME, m_role, item_id, num, reason, item_param_string);
	}
	else if (itembase->IsRecord())
	{
		gamelog::g_log_knapsack.printf(LL_INFO, "[Knapsack::ConsumeItemByIndex Succ][user[%s %d] item_id:%d item_name:%s(%d) consume_num:%d remain_num:%d reason:%s]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), (int)item_id, itembase->GetItemName(), (int)item_is_bind, (int)num, (int)m_grid_list[knapsack_index].num, NULL == reason ? "" : reason);

		ROLE_LOG_QUICK6(LOG_TYPE_ITEM_CONSUME, m_role, item_id, num, reason, NULL);
	}

	return true;
}

bool Knapsack::ConsumeItemByIndexList(int list_num, const short *knapsack_index_list, const short *consume_num_list, const char *reason, bool is_real_consume, bool need_audit_log)
{
	if (list_num <= 0 || list_num > MAX_KNAPSACK_GRID_NUM || NULL == knapsack_index_list || NULL == consume_num_list) return false;

	if (m_is_lock)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	int real_count = 0;
	static short real_index[MAX_KNAPSACK_GRID_NUM];
	static short real_num[MAX_KNAPSACK_GRID_NUM];
	memset(real_index, 0, sizeof(real_index));
	memset(real_num, 0, sizeof(real_num));

	for (int i = 0; i < list_num; i++)  // 检查重复 将实际不同的index分出来
	{
		if (consume_num_list[i] < 0) return false;
		if (0 == consume_num_list[i]) continue;

		bool found_duplicate = false;
		for (int k = 0; k < real_count; k++)
		{
			if (real_index[k] == knapsack_index_list[i])
			{
				real_num[k] += consume_num_list[i];
				found_duplicate = true;
				break;
			}
		}
		if (found_duplicate) continue;

		if (knapsack_index_list[i] < 0 || knapsack_index_list[i] >= MAX_KNAPSACK_GRID_NUM) return false;
		if (real_count >= MAX_KNAPSACK_GRID_NUM) return false;

		real_index[real_count] = knapsack_index_list[i];
		real_num[real_count] = consume_num_list[i];

		++real_count;
	}

	for (int i = 0; i < real_count; i++) // 检查消费
	{
		if (real_num[i] <= 0) return false;

		int real_i = real_index[i];
		if (m_grid_list[real_i].Invalid()) return false;
		
		const ItemBase *item = ITEMPOOL->GetItem(m_grid_list[real_i].item_id);
		if (NULL == item) return false;

		if (m_grid_list[real_i].num < real_num[i]) return false;
	}

	if (!is_real_consume) return true;

	for (int i = 0; i< real_count; i++) // 真正消费
	{
		int real_i = real_index[i];
		ItemID item_id = m_grid_list[real_i].item_id;
		const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
		if (NULL == itembase) continue; // 上文已经检查过了，这里需要这个指针是为了打log

		bool item_is_bind = m_grid_list[real_i].is_bind;

		if (m_grid_list[real_i].gold_price > 0)
		{
			int consume_gold_price = ItemExtern::GetGoldPriceByNum(item_id, m_grid_list[real_i].num, m_grid_list[real_i].gold_price, real_num[i]);
			if (consume_gold_price > 0)
			{
				if (consume_gold_price > m_grid_list[real_i].gold_price) consume_gold_price = m_grid_list[real_i].gold_price;

				m_grid_list[real_i].gold_price -= consume_gold_price;
			}
		}

		const char *equip_data_string = ItemGridDataLog(itembase, m_grid_list[real_i]);
		const char *item_param_string = ItemGridQuickLog(itembase, m_grid_list[real_i]);
		
		m_grid_list[real_i].num -= real_num[i];
		if (m_grid_list[real_i].num <= 0)
		{
			m_grid_list[real_i].Clear();
			this->RefreshValidGridIndexEnd(real_i);
		}
		
		this->SetItemDirty(real_i);
	
		EventHandler::Instance().OnGetItem(m_role, item_id, -real_num[i]);

		if (NULL != equip_data_string)
		{
			gamelog::g_log_knapsack.printf(LL_INFO, "[Knapsack::ConsumeEquipmentByIndexList Succ][user[%s %d] equipment_data:%s]",
				m_role->GetName(), UidToInt(m_role->GetUserId()), equip_data_string);

			ROLE_LOG_QUICK6(LOG_TYPE_ITEM_CONSUME, m_role, itembase->GetItemId(), real_num[i], reason, item_param_string);
		}
		else if (itembase->IsRecord())
		{
			gamelog::g_log_knapsack.printf(LL_INFO, "[Knapsack::ConsumeItemByIndexList Succ][user[%s %d] item_id:%d item_name:%s(%d) consume_num:%d remain_num:%d reason:%s]",
				m_role->GetName(), UidToInt(m_role->GetUserId()), (int)itembase->GetItemId(), itembase->GetItemName(), (int)item_is_bind, (int)real_num[i], (int)m_grid_list[real_i].num, NULL == reason ? "" : reason);

			ROLE_LOG_QUICK6(LOG_TYPE_ITEM_CONSUME, m_role, itembase->GetItemId(), real_num[i], reason, NULL);
		}
	}

	return true;
}

void Knapsack::SendKnapsackInfoToRole()
{
	static Protocol::SCKnapsackInfoAck kia;

	kia.item_count = 0;
	kia.max_knapsack_valid_num = m_curr_knapsack_valid_num;	
	kia.max_storage_valid_num = m_curr_storage_valid_num;	

	static Protocol::SCKnapsackInfoParam kip;
	kip.count = 0;

	for (int i = 0; i < MAX_KNAPSACK_STORAGE_GRID_NUM; ++i)
	{
		if (!m_grid_list[i].Invalid())
		{
			kia.item_list[kia.item_count].item_id = m_grid_list[i].item_id;
			kia.item_list[kia.item_count].is_bind = (char)m_grid_list[i].is_bind;
			kia.item_list[kia.item_count].index = i;
			kia.item_list[kia.item_count].num = m_grid_list[i].num;
			kia.item_list[kia.item_count].has_param = false;
			kia.item_list[kia.item_count].invalid_time = m_grid_list[i].invalid_time;
			
			if (NULL != m_grid_list[i].param) // 只发送有参数的
			{
				kia.item_list[kia.item_count].has_param = true;

				kip.data[kip.count].index = i;
				((EquipmentParam*)(m_grid_list[i].param))->SetInStructData(&(kip.data[kip.count].param));
				
				++ kip.count;
			}

			++ kia.item_count;
		}
	}

	UInt32 sendlen = sizeof(Protocol::SCKnapsackInfoAck) - sizeof(Protocol::SCKnapsackInfoAck::InfoType) * (MAX_KNAPSACK_STORAGE_GRID_NUM - kia.item_count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&kia, sendlen);

	UInt32 param_send_len = sizeof(Protocol::SCKnapsackInfoParam) - sizeof(kip.data[0]) * (MAX_KNAPSACK_STORAGE_GRID_NUM - kip.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&kip, param_send_len);

	m_money.Send();
}

void Knapsack::SendKnapsackGridExtendAuto()
{
	Protocol::SCKnapsackGridExtendAuto kgea;
	kgea.online_time = static_cast<unsigned int>(m_role->GetRealTimeOnlineS());
	kgea.auto_extend_times = m_valid_grid_auto_add_count;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&kgea, sizeof(kgea));
}

bool Knapsack::GetItemGridData(int index, ItemGridData *item_data)
{
	if (NULL == item_data || index < 0 || index >= MAX_KNAPSACK_GRID_NUM) return false;

	if (m_grid_list[index].Invalid()) return false;

	const ItemBase *itembase = ITEMPOOL->GetItem(m_grid_list[index].item_id);
	if (NULL == itembase) return false;

	if (m_grid_list[index].num <= 0 || m_grid_list[index].num > itembase->GetPileLimit()) return false;

	item_data->Set(m_grid_list[index]);

	return true;
}

ItemGridData *Knapsack::GetItemGridData(short index)
{
	if (index < 0 || index >= MAX_KNAPSACK_GRID_NUM)
	{
		return NULL;
	}

	if (m_grid_list[index].Invalid())
	{
		return NULL;
	}

	return &m_grid_list[index];
}

void Knapsack::SetItemDirty(int index, bool to_notice_client, short protocal_change_type, short reason_type)
{
	if (index < 0 || index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return;

	m_dirty_mark[index] = true;
	m_knapsack_is_change = true;

	if (to_notice_client) this->SendknapsackGridInfoToRole(index, protocal_change_type, reason_type);
}

void Knapsack::RefreshValidGridIndexEnd(int change_index)
{
	if (change_index < 0 || change_index >= MAX_KNAPSACK_GRID_NUM) return;

	if (m_grid_list[change_index].Invalid())
	{
		if (m_valid_grid_index_end < change_index)
		{
			m_valid_grid_index_end = change_index;
		}
	}
	else
	{
		if (m_valid_grid_index_end < change_index + 1)
		{
			m_valid_grid_index_end = change_index + 1;
		}
	}

	for (int i = m_valid_grid_index_end - 1; i >= 0 ; i--) // 往前搜索直到最后的非空格子
	{
		if (!m_grid_list[i].Invalid())
		{
			m_valid_grid_index_end = i + 1;
			return;
		}
	}

	m_valid_grid_index_end = 0;
}

void Knapsack::GridChangeEquipId(int index, ItemID new_equip_id, bool need_set_dirty)
{
	if (index < 0 || index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return;
	if (m_grid_list[index].Invalid()) return;

	const Equipment *old_equip = (const Equipment *)ITEMPOOL->GetItem(m_grid_list[index].item_id);
	if (NULL == old_equip || old_equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT) return;
	
	const Equipment *new_equip = (const Equipment *)ITEMPOOL->GetItem(new_equip_id);
	if (NULL == new_equip || new_equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT) return;

	if (old_equip != new_equip)
	{
		m_grid_list[index].item_id = new_equip_id;

		if (need_set_dirty) this->SetItemDirty(index);
	}
}

void Knapsack::GridSetBind(int index, bool need_set_dirty)
{
	if (index < 0 || index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return;

	if (!m_grid_list[index].Invalid() && !m_grid_list[index].is_bind)
	{
		m_grid_list[index].is_bind = true;

		if (need_set_dirty) this->SetItemDirty(index);
	}
}

void Knapsack::GridClearInvalidTime(int index, bool need_set_dirty)
{
	if (index < 0 || index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return;

	if (!m_grid_list[index].Invalid() && m_grid_list[index].invalid_time > 0)
	{
		m_grid_list[index].invalid_time = 0;

		if (need_set_dirty) this->SetItemDirty(index);
	}
}

void Knapsack::GetItemList(int *count, ItemListParam::ItemListData *save_list)
{
	int tmp_count = 0;

	for (int i = 0; i < MAX_KNAPSACK_STORAGE_GRID_NUM; ++i)
	{
		if (!m_grid_list[i].Invalid())
		{
			save_list[tmp_count].change_state_item = structcommon::CS_NONE;
			save_list[tmp_count].index = i;
			m_grid_list[i].WrapTo(&save_list[tmp_count].item_wrapper);

			if (++ tmp_count >= *count) break;
		}
	}

	*count = tmp_count;
}

void Knapsack::GetChangeItemList(int *count, ItemListParam::ItemListData *save_list)
{
	if (!m_knapsack_is_change)
	{
		*count = 0;
		return;
	}

	int tmp_count = 0;

	for (int i = 0; i < MAX_KNAPSACK_STORAGE_GRID_NUM; ++i)
	{
		if (m_dirty_mark[i])
		{
			if (m_grid_list[i].Invalid())
			{
				if (!m_old_state[i]) continue;
				
				save_list[tmp_count].change_state_item = structcommon::CS_DELETE;
			}
			else
			{
				if (m_old_state[i])
				{
					save_list[tmp_count].change_state_item = structcommon::CS_UPDATE;
				}
				else
				{
					save_list[tmp_count].change_state_item = structcommon::CS_INSERT;
				}
			}

			save_list[tmp_count].index = i;
			m_grid_list[i].WrapTo(&save_list[tmp_count].item_wrapper);

			if (++ tmp_count >= *count) break;
		}
	}

	*count = tmp_count;
}

void Knapsack::ClearDirtyMark()
{
	if (!m_knapsack_is_change) return;

	for (int i = 0; i < MAX_KNAPSACK_STORAGE_GRID_NUM; ++i)
	{
		if (m_dirty_mark[i])
		{
			if (m_grid_list[i].Invalid())
			{
				m_old_state[i] = false;
			}
			else
			{
				m_old_state[i] = true;
			}
		}
	}

	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));

	m_knapsack_is_change = false;
}

void Knapsack::SendknapsackGridInfoToRole(int index, int change_type, short reason_type)
{
	if (index < 0 || index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return;

	if (NULL == m_grid_list[index].param)
	{
		static Protocol::SCKnapsackItemChange kic;

		kic.change_type = (char)change_type;
		kic.reason_type = reason_type;
		kic.is_bind = m_grid_list[index].is_bind ? 1 : 0;
		kic.index = (short)index;
		kic.item_id = m_grid_list[index].item_id;
		kic.num = m_grid_list[index].num;
		kic.invalid_time = m_grid_list[index].invalid_time;

		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&kic, sizeof(kic));
	}
	else
	{
		static Protocol::SCKnapsackItemChangeParam kicp;

		kicp.change_type = (short)change_type;
		kicp.reason_type = reason_type;
		kicp.index = (short)index;
		m_grid_list[index].WrapTo(&kicp.item_wrapper);

		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&kicp, sizeof(kicp));
	}
}

void Knapsack::CertainPut(const ItemDataWrapper &item_info, int reason_type, int *out_index, UInt16 drop_monster_id)
{	
	const ItemBase *itembase = ITEMPOOL->GetItem(item_info.item_id);
	if (NULL == itembase) return;

	unsigned int curr_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (item_info.invalid_time > 0 && item_info.invalid_time <= curr_time)
	{
		static MailContentParam contentparam; contentparam.Reset();
		int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_item_invalid_subiect);
		int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_item_invalid_content, itembase->GetItemName());
		if (length1 > 0 && length2 > 0)
		{
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
		return;
	}

	if (m_valid_grid_index_end > m_curr_knapsack_valid_num || m_curr_knapsack_valid_num > MAX_KNAPSACK_GRID_NUM) return;

	bool is_equipment = (ItemBase::I_TYPE_EQUIPMENT == itembase->GetItemType());

	static int real_put_index_list[MAX_KNAPSACK_GRID_NUM] = {0};
	int real_put_index_count = 0;

	ItemDataWrapper tmp_item_wrapper(item_info);

	//功能预告获得的仙宠灵魂特殊处理，设置初始等级为1
	if (reason_type == PUT_REASON_ADVANCE_NOTICE_REWARD)
	{
		const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(item_info.item_id);
		if (NULL != equip_item && ItemBase::I_TYPE_EQUIPMENT == equip_item->GetItemType())
		{
			if (equip_item->GetSubType() == Equipment::E_TYPE_SPECIAL_MINGHUN)
			{
				tmp_item_wrapper.param_data.strengthen_level = 1;
			}
		}
	}

	int empty_index = -1;
	for (int i = 0; i < m_valid_grid_index_end; ++i)
	{
		if (is_equipment) // 装备只需要找到第一个空位置即可
		{
			if (m_grid_list[i].Invalid()) 
			{
				empty_index = i;
				break;
			}
		}
		else 
		{
			if (m_grid_list[i].Invalid()) 
			{
				if (-1 == empty_index) empty_index = i;
			}
			else if (m_grid_list[i].CanBeMerge(tmp_item_wrapper.invalid_time))
			{
				if (real_put_index_count >= MAX_KNAPSACK_GRID_NUM)
				{
					break;
				}
				int rest_num = 0;
				int merged_gold_price = 0;
				int merge_result = m_grid_list[i].Merge(tmp_item_wrapper, &rest_num, &merged_gold_price);
				if (ITEM_MERGE_COMPLETE == merge_result)
				{
					real_put_index_list[real_put_index_count ++] = i; // 已经放完 不需要检查count了
					tmp_item_wrapper.num = 0;
					tmp_item_wrapper.gold_price = 0;
					break;
				}
				else if (ITEM_MERGE_NOT_COMPLETE == merge_result)
				{
					real_put_index_list[real_put_index_count ++] = i;
					tmp_item_wrapper.num = rest_num;
					tmp_item_wrapper.gold_price -= merged_gold_price;
				}

			}
		}
	}

	// 前面全满 则从下一个空位开始放
	if (-1 == empty_index && m_valid_grid_index_end < m_curr_knapsack_valid_num)
	{
		empty_index = m_valid_grid_index_end;
	}

	// 容错, 避免越界操作
	if (empty_index >= 0)
	{
		// 如果还有剩余，则从当前空格子开始往后放入物品
		for (; empty_index < m_curr_knapsack_valid_num && tmp_item_wrapper.num > 0; empty_index++)
		{
			// 该格子是空的则放入物品，原本剩余的数量减去放进该格子的数量
			if (m_grid_list[empty_index].Invalid())
			{
				if (real_put_index_count >= MAX_KNAPSACK_GRID_NUM)
				{
					break;
				}
				int rand_attr_put_type = -1;
				rand_attr_put_type = itembase->GetColor();
				m_grid_list[empty_index].Set(tmp_item_wrapper, rand_attr_put_type);
				tmp_item_wrapper.num -= m_grid_list[empty_index].num;
				real_put_index_list[real_put_index_count++] = empty_index;

			}
		}
	}

	short last_item_count = tmp_item_wrapper.num;
	tmp_item_wrapper.num = item_info.num - tmp_item_wrapper.num;
	if (tmp_item_wrapper.num > 0)
	{
		DropLogManager::Instance().AddLogGetItem(tmp_item_wrapper);
	}
	tmp_item_wrapper.num = last_item_count;

	// 如果格子不够并且还有剩余，则剩下的物品数量以邮件的形式发给玩家
	if (tmp_item_wrapper.num > 0)
	{
		static MailContentParam contentparam; contentparam.Reset();
		contentparam.item_list[0].item_id = tmp_item_wrapper.item_id;
		contentparam.item_list[0].is_bind = (short)tmp_item_wrapper.is_bind;
		contentparam.item_list[0].num = tmp_item_wrapper.num;
		contentparam.item_list[0].invalid_time = itembase->CalInvalidTime();

		int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_knapsack_put_subiect);
		int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_knapsack_put_content);
		if (length1 > 0 && length2 > 0)
		{
			MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
		}

		gstr::SendError(m_role, "knapsack_full_and_send_mail");
	}

	/*
	if (tmp_item_wrapper.num > 0) // 还有放剩的
	{
		if (-1 == empty_index && m_valid_grid_index_end < m_curr_knapsack_valid_num) // 前面全满 则放到下一个空位里
		{
			empty_index = m_valid_grid_index_end;
		}

		if (empty_index < 0 || empty_index >= MAX_KNAPSACK_GRID_NUM) return;

		int rand_attr_put_type = -1;
// 		if (NULL != m_role->GetScene())
// 		{
// 			if (PUT_REASON_GM == reason_type)
// 			{
// 				rand_attr_put_type = ZHUANSHENG_EQUIP_PUT_TYPE_COMMON_SCENE;
// 			}
// 			else if (Scene::SCENE_TYPE_COMMON_SCENE == m_role->GetScene()->GetSceneType())
// 			{
// 				rand_attr_put_type = ZHUANSHENG_EQUIP_PUT_TYPE_COMMON_SCENE;
// 			}
// 			else if (Scene::SCENE_TYPE_CROSS_BOSS == m_role->GetScene()->GetSceneType())
// 			{
// 				rand_attr_put_type = ZHUANSHENG_EQUIP_PUT_TYPE_CROSS_BOSS;
// 			}
// 		}

		rand_attr_put_type = itembase->GetColor();

		m_grid_list[empty_index].Set(tmp_item_wrapper, rand_attr_put_type);

		if (real_put_index_count >= MAX_KNAPSACK_GRID_NUM) return;
		real_put_index_list[real_put_index_count ++] = empty_index;
	}
	*/

	if (real_put_index_count <= 0 || real_put_index_count > MAX_KNAPSACK_GRID_NUM) return;

	EventHandler::Instance().OnGetItem(m_role, item_info.item_id, item_info.num);

	short change_type = (PUT_REASON_NO_NOTICE == reason_type) ? ITEM_CHANGE_TYPE_SNEAKY_IN : ITEM_CHANGE_TYPE_IN;
	for (int i = 0; i < real_put_index_count; i++)
	{
		int index = real_put_index_list[i];
		this->RefreshValidGridIndexEnd(index);
		this->SetItemDirty(index, true, change_type, reason_type);
	}

	if (NULL != out_index) *out_index = real_put_index_list[0]; // 只传出第一个

	char put_reason_str[64] = {0};
	//char *broadcast_str = NULL;

	switch (reason_type)
	{
	case PUT_REASON_NO_NOTICE:
	case PUT_REASON_GM:
		{
			STRNCPY(put_reason_str, "other", sizeof(put_reason_str));
		}
		break;

	case PUT_REASON_PICK:
		{
			// 捡取掉落的公告在捡取时发
			STRNCPY(put_reason_str, "drop", sizeof(put_reason_str));
		}
		break;

	case PUT_REASON_GIFT:
		{
			// 礼包的公告在打开礼包时发
			STRNCPY(put_reason_str, "gift", sizeof(put_reason_str));

			// 添加仙品
// 			ItemGridData item_grid_data;
// 			{
// 				if(empty_index >= 0)
// 				{
// 					bool succ = this->GetItemGridData(empty_index, &item_grid_data);
// 
// 					if (succ && GLOBALCONFIG->GetEquipConfig().GetRandomXianpin(&item_grid_data))
// 					{
// 						this->SetItemDirty(empty_index);
// 					}
// 				}
// 			}
		}
		break;

	case PUT_REASON_COMPOSE:
		{
			// 合成的公告在合成时发
			STRNCPY(put_reason_str, "compose", sizeof(put_reason_str));
		}
		break;

	case PUT_REASON_TASK_REWARD:
		{
			STRNCPY(put_reason_str, "reward", sizeof(put_reason_str));
		}
		break;

	case PUT_REASON_MAIL_RECEIVE:
		{
			STRNCPY(put_reason_str, "mail_receive", sizeof(put_reason_str));
		}
		break;

	case PUT_REASON_CHEST_SHOP:
		{
			STRNCPY(put_reason_str, "chest_shop", sizeof(put_reason_str));
		}
		break;

	case PUT_REASON_RANDOM_CAMP:
		{
			STRNCPY(put_reason_str, "random_camp", sizeof(put_reason_str));
		}
		break;

	case PUT_REASON_SHOP_BUY:
		{
			STRNCPY(put_reason_str, "shop_buy", sizeof(put_reason_str));
		}
		break;

	case PUT_REASON_WELFARE:
		{
			STRNCPY(put_reason_str, "welfare", sizeof(put_reason_str));
		}
		break;

	case PUT_REASON_FETCH_ROLE_UPLEVEL_REWARD:
		{
			STRNCPY(put_reason_str, "fetch_role_up_level_reward", sizeof(put_reason_str));
		}
		break;

	case PUT_REASON_FETCH_SIGN_IN_REWARD:
		{
			STRNCPY(put_reason_str, "fetch_sign_in_reward", sizeof(put_reason_str));
		}
		break;

	case PUT_REASON_SIGN_IN_FIND_BACK:
		{
			STRNCPY(put_reason_str, "fetch_sign_in_find_back_reward", sizeof(put_reason_str));
		}
		break;

	case PUT_REASON_ACTIVITY_FIND:
		{
			STRNCPY(put_reason_str, "fetch_activity_find_reward", sizeof(put_reason_str));
		}
		break;

	case PUT_REASON_GET_DAILY_FIND:
		{
			STRNCPY(put_reason_str, "fetch_daily_find_reward", sizeof(put_reason_str));
		}
		break;

	case PUT_REASON_ACTIVE_DEGREE:
		{
			STRNCPY(put_reason_str, "active_degree", sizeof(put_reason_str));
		}
		break;

	default:
		{
			SNPRINTF(put_reason_str, sizeof(put_reason_str), "%d", (int)reason_type);
		}
		break;
	}

	if (!item_info.is_bind) m_role->GetMonitor()->AddMonitorNum(MONITOR_TYPE_DAY_NOBIND_ITEM_NUM, item_info.num);

	if (itembase->IsRecord())
	{
		gamelog::g_log_knapsack.printf(LL_INFO, "[Knapsack::CertainPut Succ][user[%s %d] item_id:%d item_name:%s(%d) color:%d is_equipment:%d put_num:%d reason:%s drop_monster_id:%d]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), (int)item_info.item_id, itembase->GetItemName(), (int)item_info.is_bind, (int)itembase->GetColor(), (int)is_equipment, (int)item_info.num, put_reason_str, (int)drop_monster_id);

		char log_quick_str[64] = {0};
		SNPRINTF(log_quick_str, sizeof(log_quick_str), "item_put_%d", (int)reason_type);
		ROLE_LOG_QUICK6(LOG_TYPE_ITEM_PUT, m_role, item_info.item_id, item_info.num, log_quick_str, NULL);
	}
	
	/*
	if (NULL != broadcast_str && itembase->IsBroadcast())
	{
		// 在这里打的公告仅允许两种类型：1.角色信息(dsd)+物品id(d)；2.角色信息(dsd)+装备信息(ddds)
		static char str_buff[1024] = {0};
		int str_len = 0;
		if (PUT_REASON_EQUIP_COMPOSE == reason_type)
		{
			const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_info.item_id);
			if (NULL != equip && equip->GetItemType() == ItemBase::I_TYPE_EQUIPMENT && equip->GetFatherEquipType() == Equipment::E_FA_T_NORMAL)
			{
				int put_index = real_put_index_list[0];
				if (put_index >= 0 && put_index < MAX_KNAPSACK_GRID_NUM && !m_grid_list[put_index].Invalid())
				{
					NormalEquipParam *equip_param = (NormalEquipParam *)m_grid_list[put_index].param;
					if (NULL != equip_param && equip_param->GetParamType() == EquipmentParam::PARAM_TYPE_NORMAL_EQUIP)
					{
						const char *equip_param_buff = EquipParamDataStruct::PrintParamStr(equip_param, item_info.item_id);
						if (NULL != equip_param_buff)
						{
							str_len = SNPRINTF(str_buff, sizeof(str_buff), broadcast_str, UidToInt(m_role->GetUserId()), m_role->GetName(), 
								(int)m_role->GetCamp(), (int)item_info.item_id, equip_param->prefix, -1, equip_param_buff); // 现时只能合成一般装备
						}
					}
				}
			}
		}
		else
		{
			str_len = SNPRINTF(str_buff, sizeof(str_buff), broadcast_str, UidToInt(m_role->GetUserId()), m_role->GetName(), 
				(int)m_role->GetCamp(), (int)item_info.item_id);
		}

		if (str_len > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(str_buff, str_len, SYS_MSG_ROLL);
		}
	}
	*/
}

bool Knapsack::UpdateToMail()
{
	const size_t size = m_no_have_space_mail_vec.size();
	if (size == 0)
	{
		return false;
	}
	MailContentParam contentparam;
	int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_knapsack_put_subiect);
	int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_knapsack_put_content);
	if (!(length1 > 0 && length2 > 0))
	{
		return false;
	}
	for (size_t i = 0; i < size; ++i)
	{
		if (contentparam.CanMerge(m_no_have_space_mail_vec[i]))
		{
			contentparam.Merge(m_no_have_space_mail_vec[i]);
		}
		else
		{
			MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);
			contentparam.ResetExceptText();
			contentparam.Set(m_no_have_space_mail_vec[i]);
		}
	}
	MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, contentparam);	//for循环内没有发最后一封
	m_no_have_space_mail_vec.clear();
	return true;
}

bool Knapsack::MoveHelper(int from_index, int to_index)
{
	if (from_index == to_index) return false;
	if (from_index < 0 || from_index >= MAX_KNAPSACK_STORAGE_GRID_NUM || to_index < 0 || to_index >= MAX_KNAPSACK_STORAGE_GRID_NUM) return false;
	if (m_grid_list[from_index].Invalid()) return false;

	if (m_grid_list[to_index].Invalid()) // 直接放过去
	{
		m_grid_list[to_index].Set(m_grid_list[from_index]);
		m_grid_list[from_index].Reset();
	}
	else 
	{
		bool from_is_bind = m_grid_list[from_index].is_bind;

		int rest_num = -1;
		int merged_gold_price = 0;
		int merge_result = m_grid_list[to_index].Merge(m_grid_list[from_index], &rest_num, &merged_gold_price, true);

		if (ITEM_MERGE_CANNOT == merge_result)
		{
			ItemGridData tmp;
			tmp.Set(m_grid_list[to_index]);
			m_grid_list[to_index].Set(m_grid_list[from_index]);
			m_grid_list[from_index].Set(tmp);
		}
		else
		{
			if (0 == rest_num)
			{
				m_grid_list[from_index].Reset();
			}
			else
			{
				m_grid_list[from_index].num = rest_num;
				m_grid_list[from_index].gold_price -= merged_gold_price;
			}

			m_grid_list[to_index].is_bind = m_grid_list[to_index].is_bind || from_is_bind;
		}
	}

	int max_index = (from_index >= to_index) ? from_index : to_index;

	if (max_index >= MAX_KNAPSACK_GRID_NUM)
	{
		int min_index = from_index + to_index - max_index;
		if (min_index < MAX_KNAPSACK_GRID_NUM) this->RefreshValidGridIndexEnd(min_index);
	}
	else 
	{
		this->RefreshValidGridIndexEnd(max_index);
	}

	if (m_grid_list[from_index].item_id != m_grid_list[to_index].item_id)
	{
		this->SetItemDirty(from_index, true, to_index); // 注意这里的改变类型 使用大于0的值表示该格子的改变有出处
		this->SetItemDirty(to_index, true, from_index);	
	}
	else 
	{
		this->SetItemDirty(from_index);	
		this->SetItemDirty(to_index);	
	}

	return true;
}

bool Knapsack::GetListOfItem(ItemID item_id, short max_num, 
							 short *bind_index_list, short *bind_num_list, short *bind_count, 
							 short *nobind_index_list, short *nobind_num_list, short *nobind_count)
{
	if (m_is_lock)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (item_id <= 0 || max_num <= 0 ||
		NULL == bind_index_list || NULL == bind_num_list || NULL == bind_count ||
		NULL == nobind_index_list || NULL == nobind_num_list || NULL == nobind_count)
	{
		return false;
	}

	*bind_count = *nobind_count = 0;

	short tmp_bind_count = 0, tmp_nobind_count = 0;

	for (int i = 0; i < m_valid_grid_index_end; ++i)
	{
		if (tmp_bind_count >= max_num || tmp_nobind_count >= max_num)
		{
			continue;
		}

		if (m_grid_list[i].item_id == item_id) 
		{
			if (m_grid_list[i].is_bind)
			{
				bind_index_list[tmp_bind_count] = i;
				bind_num_list[tmp_bind_count] = m_grid_list[i].num;
				++ tmp_bind_count;
			}
			else
			{
				nobind_index_list[tmp_nobind_count] = i;
				nobind_num_list[tmp_nobind_count] = m_grid_list[i].num;
				++ tmp_nobind_count;
			}
		}
	}

	*bind_count = tmp_bind_count;
	*nobind_count = tmp_nobind_count;

	return true;
}

int Knapsack::GetItemNum(ItemID item_id)
{
	if (item_id == ItemBase::INVALID_ITEM_ID) return 0;

	int count = 0;

	for (int i = 0; i < m_valid_grid_index_end; ++i)
	{
		if (m_grid_list[i].Invalid()) continue; 

		if (m_grid_list[i].item_id == item_id) 
		{
			count += m_grid_list[i].num;
		}
	}

	return count;
}

void Knapsack::GMExtendAll()
{
	m_curr_knapsack_valid_num = MAX_KNAPSACK_GRID_NUM;

	Protocol::SCKnapsackMaxGridNum kmgn;
	kmgn.max_grid_num = m_curr_knapsack_valid_num;
	kmgn.is_auto_extend = 0;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&kmgn, sizeof(kmgn));
}

void Knapsack::CheckImpGuardMailInKnapsack(time_t time)
{
	for (int i = 0; i < m_valid_grid_index_end; i++)
	{
		if (m_grid_list[i].Invalid()) continue;

		const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(m_grid_list[i].item_id);
		if (NULL != equip && equip->GetEquipType() == Equipment::E_TYPE_SPECIAL_IMP_GUARD)
		{
			const ImpConfig *imp_cfg = LOGIC_CONFIG->GetImpGuardConfig().GetImpCfgByItemId(m_grid_list[i].item_id);
			if (NULL == imp_cfg) continue;

			if (0 == (m_role->GetCommonDataParam()->imp_guard_mail_flag & (1 << imp_cfg->imp_type)))
			{
				if ((m_grid_list[i].invalid_time > 0) &&
					(m_grid_list[i].invalid_time > (unsigned int)time) &&
					(m_grid_list[i].invalid_time < ((unsigned int)time + SECOND_PER_HOUR * TWELVE_HOURS)))
				{
					//邮件提醒
					static MailContentParam contentparam; contentparam.Reset();
					unsigned int left_time = m_grid_list[i].invalid_time - (unsigned int)time;
					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_impguard_remind_mail_content, imp_cfg->item_id, left_time);
					if (length > 0)
					{
						MailRoute::MailToUser(IntToUid(m_role->GetUID()), SYSTEM_MAIL_REASON_INVALID, contentparam);
						m_role->GetCommonDataParam()->imp_guard_mail_flag |= (1 << imp_cfg->imp_type);
					}
				}
			}
		}
	}
}

void Knapsack::PutInKnapsack(const ItemConfigData item_list[MAX_ATTACHMENT_ITEM_NUM])
{
	ItemConfigData temp_item_list[MAX_ATTACHMENT_ITEM_NUM];
	memcpy(temp_item_list, item_list, sizeof(temp_item_list));

	int item_count[MAX_ATTACHMENT_ITEM_NUM];
	for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
	{
		item_count[i] = 0;
		if (0 < temp_item_list[i].num)
		{
			item_count[i] = temp_item_list[i].num;
		}
	}

	while (0 < item_count[0] || 0 < item_count[1] || 0 < item_count[2])
	{
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			const ItemBase *itembase = ITEMPOOL->GetItem(temp_item_list[i].item_id);
			if (NULL == itembase)
			{
				item_count[i] = 0;
				continue;
			}

			int pile_count = itembase->GetPileLimit();
			if (pile_count <= 0)
			{
				item_count[i] = 0;
				continue;
			}

			item_count[i] = temp_item_list[i].num;
			if (item_count[i] > pile_count)
			{
				temp_item_list[i].num = pile_count;
				item_count[i] -= pile_count;
			}
			else
			{
				item_count[i] = 0;
			}
		}

		this->PutListOrMail(temp_item_list, PUT_REASON_DISCARD_EQUIP);

		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			temp_item_list[i].num = item_count[i];
			if (0 == item_count[i])
			{
				temp_item_list[i].item_id = 0;
				temp_item_list[i].is_bind = 0;
			}
		}
	}
}

bool Knapsack::ConsumeItemOrUseGold(ItemID item_id, const int num, const char *reason, bool need_audit_log, bool is_use_bind_gold)
{
	if (m_is_lock)
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (num <= 0) return false;

	int consume_num = num;

	int has_num = this->GetItemNum(item_id);
	int need_buy_num = num - has_num;
	if (need_buy_num > 0)
	{
		int gold_price = 0, bind_gold_price = 0;
		if (!LOGIC_CONFIG->GetShop().GetShopItemGoldPrice(item_id, &gold_price, &bind_gold_price))
		{
			return false;
		}

		int need_gold_num = gold_price * need_buy_num;
		if (is_use_bind_gold)
		{
			need_gold_num = bind_gold_price * need_buy_num;
		}

		if (need_gold_num <= 0) { return false; }

		if (is_use_bind_gold)
		{
			if (need_gold_num > 0 && !m_role->GetKnapsack()->GetMoney()->UseAllGold(need_gold_num, reason))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return false;
			}
		}
		else
		{
			if (need_gold_num > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(need_gold_num, reason))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return false;
			}
		}

		consume_num = has_num;
		{
			ItemExtern::BuyItemInfo buy_info;
			buy_info.item_id = item_id;
			buy_info.item_count = need_buy_num;
			buy_info.consume_gold = need_gold_num;
			EventHandler::Instance().OnShopBuyItem(m_role, 1, &buy_info, need_gold_num, need_gold_num);
		}
	}

	if (consume_num > 0 && !this->ConsumeItem(item_id, consume_num, reason, need_audit_log))
	{
		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return false;
	}

	return true;
}

void Knapsack::PushNoHaveSpaceWrapper(const MailContentParam & mail_content)
{
	m_no_have_space_mail_vec.push_back(mail_content);
}
