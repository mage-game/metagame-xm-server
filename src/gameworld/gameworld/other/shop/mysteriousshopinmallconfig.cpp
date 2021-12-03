#include "mysteriousshopinmallconfig.hpp"

#include "world.h"
#include "common/tinyxml/tinyxml.h"
#include "servercommon/configcommon.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/mysteriousshopinmalldef.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "obj/character/role.h"
#include "item/itempool.h"
#include "other/shop/mysteriousshopinmall.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "other/lotterycount/lotterycount.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"


MysteriousShopInMallConfig::MysteriousShopInMallConfig() : max_seq(0)
{

}

MysteriousShopInMallConfig::~MysteriousShopInMallConfig()
{

}

bool MysteriousShopInMallConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;

	PUGI_XML_LOAD_AND_CHECK(document, configname, "MysteriousShopInMallConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo,"Load MysteriousshopinMall config [%s] Error.cannot find root node.", configname.c_str());
		*err = errinfo;
		return false;
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

	// 神秘商店刷新时间
	{
		PugiXmlNode root_element = RootElement.child("mysterious_flush_time");
		if (root_element.empty())
		{
			*err = configname + ": no [mysterious_flush_time].";
			return false;
		}

		int ret = this->InitMysteriousShopFlushConfig(root_element);
		if (ret)
		{
			sprintf(errinfo, "%s: InitMysteriousShopFlushConfig failed %d", configname.c_str(), ret);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("mysterious_shop_item");
		if (root_element.empty())
		{
			*err = configname + ": no [mysterious_shop_item].";
			return false;
		}

		int ret = this->InitMysteriousShopItemConfig(root_element);
		if (ret)
		{
			sprintf(errinfo,"%s: InitMysteriousShopItemConfig failed %d", configname.c_str(), ret);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("tehui");
		if (root_element.empty())
		{
			*err = configname + ": no [tehui].";
			return false;
		}

		int ret = this->InitDiscountShopItemConfig(root_element);
		if (ret)
		{
			sprintf(errinfo, "%s: InitDiscountShopItemConfig failed %d", configname.c_str(), ret);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const MysteriousShopFlushTimeCfg * MysteriousShopInMallConfig::GetFlushTimeCfg(time_t now)
{
	if (m_myste_shop_flush_time_vec.empty())
	{
		return nullptr;
	}

	unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(now));
	unsigned int today_sec = static_cast<unsigned int>(now) - today_zero_time;

	for (auto &it : m_myste_shop_flush_time_vec)
	{
		if (today_sec < static_cast<unsigned int>(it.refresh_time))
		{
			return &it;
		}
	}

	return &m_myste_shop_flush_time_vec[0];
}

const MysteriousShopItem * MysteriousShopInMallConfig::GetMysteriousShopItemCfg(int seq)
{
	if (seq < 0 || seq >= static_cast<int>(m_mysterious_shop_item_cfg.mysterious_shop_item_vec.size()))
	{
		return NULL;
	}

	return &m_mysterious_shop_item_cfg.mysterious_shop_item_vec[seq];
}

void MysteriousShopInMallConfig::GetRandSeqList(Role *role, int seq_list[], int count)
{
	if (NULL == role)
	{
		return;
	}

	if (m_mysterious_shop_item_cfg.mysterious_shop_item_vec.empty())
	{
		return;
	}

	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

	//筛选
	std::vector<MysteriousShopItem> tmp_vec;
	for (auto &cfg_iter : m_mysterious_shop_item_cfg.mysterious_shop_item_vec)
	{
		if (cfg_iter.levelopen_min <= role->GetLevel() && role->GetLevel() <= cfg_iter.levelopen_max &&
			cfg_iter.openday_min <= opengame_day && opengame_day <= cfg_iter.openday_max)
		{
			tmp_vec.emplace_back(cfg_iter);
		}
	}

	//计算总权重
	int total_weight = 0;
	for (auto &tmp_iter : tmp_vec)
	{
		total_weight += tmp_iter.refresh_weight;
	}

	if (total_weight <= 0)
	{
		return;
	}

	//开始刷新
	for (int i = 0; i < count; i++)
	{
		int rand_weight = RandomNum(total_weight);
		size_t tem_vec_size = tmp_vec.size();
		for (size_t j = 0; j < tem_vec_size; ++j)
		{
			if (rand_weight < tmp_vec[j].refresh_weight)
			{
				LotterySequence replace_seq = tmp_vec[j].seq;
				role->GetLotteryCount()->ReplaceLotterySequence(LOTTERY_REPLACE_TYPE_MYSTERIOUS_SHOP, replace_seq);
				const MysteriousShopItem* check_item_cfg = this->GetMysteriousShopItemCfg(replace_seq);
				if (nullptr == check_item_cfg)
				{
					replace_seq = tmp_vec[j].seq;
				}
				const MysteriousShopItem* item_cfg = this->GetMysteriousShopItemCfg(replace_seq);
				if (nullptr == item_cfg)
				{
					continue;
				}
				seq_list[i] = replace_seq;
				if (item_cfg->is_broadcast)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mysteriousshop_refresh_good_item,
						role->GetUID(), role->GetName(), role->GetCamp(), item_cfg->dicount / 1000, item_cfg->item.item_id);
					if (length > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
					}

				}

				break;
			}
			else
			{
				rand_weight -= tmp_vec[j].refresh_weight;
			}
		}
	}
}

int MysteriousShopInMallConfig::InitOtherConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "item_count", m_other_cfg.item_count) || m_other_cfg.item_count <= 0 || m_other_cfg.item_count >= MYSTERIOUSSHOP_ITEM_MAX_COUNT)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "consume_diamond", m_other_cfg.consume_diamond) || m_other_cfg.consume_diamond < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "all_consume_diamond", m_other_cfg.all_consume_diamond) || m_other_cfg.all_consume_diamond < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "replacement_id", m_other_cfg.replacement_id) || (m_other_cfg.replacement_id > 0 && NULL == ITEMPOOL->GetItem(m_other_cfg.replacement_id)))
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElement, "consume_replacement", m_other_cfg.consume_replacement) || m_other_cfg.consume_replacement < 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElement, "all_consume_replacement", m_other_cfg.all_consume_replacement) || m_other_cfg.all_consume_replacement < 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(dataElement, "can_use_item", m_other_cfg.can_use_item_refresh))
	{
		return -12;
	}
	
	return 0;
}

int MysteriousShopInMallConfig::InitMysteriousShopFlushConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1;
	}

	while (!dataElement.empty())
	{
		MysteriousShopFlushTimeCfg flush_time_item;
		if (!PugiGetSubNodeValue(dataElement, "seq", flush_time_item.seq) || flush_time_item.seq < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_auto_refresh", flush_time_item.is_auto_refresh))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "refresh_time", flush_time_item.refresh_time) || flush_time_item.refresh_time <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "give_free_refesh_count", flush_time_item.give_free_refesh_count) || flush_time_item.give_free_refesh_count < 0 || 
			(!flush_time_item.is_auto_refresh && flush_time_item.give_free_refesh_count <= 0))
		{
			return -6;
		}

		m_myste_shop_flush_time_vec.emplace_back(flush_time_item);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int MysteriousShopInMallConfig::InitMysteriousShopItemConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1;
	}

	while (!dataElement.empty())
	{
		MysteriousShopItem shop_item;
		if (!PugiGetSubNodeValue(dataElement, "seq", shop_item.seq) || shop_item.seq < 0)
		{
			return -2;
		}

		PugiXmlNode item_element = dataElement.child("item");
		if (NULL != item_element)
		{
			if (!shop_item.item.ReadConfig(item_element))
			{
				return -3;
			}
		}

		if (!PugiGetSubNodeValue(dataElement, "price", shop_item.price) || shop_item.price < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "buy_limit", shop_item.buy_limit) || shop_item.buy_limit <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "refresh_weight", shop_item.refresh_weight) || shop_item.refresh_weight < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_broadcast", shop_item.is_broadcast))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "dicount", shop_item.dicount) || shop_item.dicount < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "levelopen_min", shop_item.levelopen_min) || shop_item.levelopen_min <= 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "levelopen_max", shop_item.levelopen_max) || shop_item.levelopen_max <= 0 || shop_item.levelopen_min >= shop_item.levelopen_max)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "openday_min", shop_item.openday_min) || shop_item.openday_min <= 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "openday_max", shop_item.openday_max) || shop_item.openday_max <= 0 || shop_item.openday_min >= shop_item.openday_max)
		{
			return -12;
		}

		m_mysterious_shop_item_cfg.mysterious_shop_item_vec.push_back(shop_item);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int MysteriousShopInMallConfig::InitDiscountShopItemConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1;
	}

	while (!dataElement.empty())
	{
		DiscountShopItem shop_item;
		if (!PugiGetSubNodeValue(dataElement, "seq", shop_item.seq) || shop_item.seq < 0 || shop_item.seq >= 128)
		{
			return -2;
		}

		PugiXmlNode item_element = dataElement.child("item");
		if (NULL != item_element)
		{
			if (!shop_item.item.ReadConfig(item_element))
			{
				return -3;
			}
		}

		if (!PugiGetSubNodeValue(dataElement, "price", shop_item.price) || shop_item.price < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "buy_limit", shop_item.buy_limit) || shop_item.buy_limit <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "levelopen_min", shop_item.levelopen_min) || shop_item.levelopen_min <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "levelopen_max", shop_item.levelopen_max) || shop_item.levelopen_max <= 0 || shop_item.levelopen_min >= shop_item.levelopen_max)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "openday_min", shop_item.openday_min) || shop_item.openday_min <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "openday_max", shop_item.openday_max) || shop_item.openday_max <= 0 /*|| shop_item.openday_min >= shop_item.openday_max*/)
		{
			return -9;
		}

		m_discount_shop_item_vec.push_back(shop_item);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

bool MysteriousShopInMallConfig::CheckSeq(int seq)
{
	if (seq < 0 || seq >= (int)m_mysterious_shop_item_cfg.mysterious_shop_item_vec.size())
	{
		return false;
	}
	return true;
}

int MysteriousShopInMallConfig::GetDiscountShopSeqList(int seq_list[], int role_level)
{
	if (m_discount_shop_item_vec.empty())
	{
		return 0;
	}

	int item_count = 0;

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

	for (auto &it : m_discount_shop_item_vec)
	{
		if (it.levelopen_min <= role_level && role_level <= it.levelopen_max &&
			it.openday_min <= opengame_day && opengame_day <= it.openday_max)
		{
			if(item_count < 0 || item_count >= DISCOUNTSHOP_ITEM_MAX_COUNT)
			{
				continue;
			}

			seq_list[item_count++] = it.seq;
		}
	}

	return item_count;
}

const DiscountShopItem * MysteriousShopInMallConfig::GetDiscountShopItemCfg(int seq)
{
	if (seq < 0 || seq >= static_cast<int>(m_discount_shop_item_vec.size()))
	{
		return NULL;
	}

	return &m_discount_shop_item_vec[seq];
}