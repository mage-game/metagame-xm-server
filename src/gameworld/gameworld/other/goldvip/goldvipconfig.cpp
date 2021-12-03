#include "goldvipconfig.hpp"

#include "servercommon/configcommon.h"
#include "item/itempool.h"

GoldVipConfig::GoldVipConfig()
{
	
}

GoldVipConfig::~GoldVipConfig()
{

}

bool GoldVipConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "GoldVipConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// ¼¤»îÅäÖÃ
		PugiXmlNode root_element = RootElement.child("goldvip_active");
		if (root_element.empty())
		{
			*err = path + " xml not goldvip_active node ";
			return false;
		}

		iRet = this->InitGoldVipActiveCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitGoldVipActiveCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ÉÌµêÅäÖÃ
		PugiXmlNode root_element = RootElement.child("goldvip_shop");
		if (root_element.empty())
		{
			*err = path + " xml not goldvip_shop node ";
			return false;
		}

		iRet = this->InitGoldVipShopCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitGoldVipShopCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("multiple_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not multiple_cfg node ";
			return false;
		}

		iRet = this->InitGoldVipShopMulCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitGoldVipShopMulCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int GoldVipConfig::InitGoldVipActiveCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{

		if (!PugiGetSubNodeValue(data_element, "need_level", m_active_cfg.need_level) || m_active_cfg.need_level < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "convert_rate", m_active_cfg.convert_rate) || m_active_cfg.convert_rate < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "need_gold", m_active_cfg.need_gold) || m_active_cfg.need_gold < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "continue_days", m_active_cfg.continue_days) || m_active_cfg.continue_days < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "active_convert_gold", m_active_cfg.active_convert_gold) || m_active_cfg.active_convert_gold < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "return_gold", m_active_cfg.return_gold) || m_active_cfg.return_gold < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "kill_monster_exp_add_per", m_active_cfg.kill_monster_exp_add_per) || m_active_cfg.kill_monster_exp_add_per < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "gold_vip_title_id", m_active_cfg.gold_vip_title_id) || m_active_cfg.gold_vip_title_id < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "count_down", m_active_cfg.count_down) || m_active_cfg.count_down < 0)
		{
			return -9;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int GoldVipConfig::InitGoldVipShopCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_shop_cfg.cfg_count = 0;
	while (!data_element.empty())
	{
		if (m_shop_cfg.cfg_count >= GOLD_VIP_CONVERT_SHOP_MAX_COUNT)
		{
			return -1000;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_shop_cfg.cfg_count || seq >= GOLD_VIP_CONVERT_SHOP_MAX_COUNT)
		{
			return -10;
		}

		GoldVipShopCfg::ConfigItem &reward_cfg = m_shop_cfg.cfg_list[seq];
		reward_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "limit_times", reward_cfg.limit_times) || reward_cfg.limit_times < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "consume_type", reward_cfg.consume_type) || reward_cfg.consume_type < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "consume_val", reward_cfg.consume_val) || reward_cfg.consume_val < 0)
		{
			return -13;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -100;
		}

		if (!reward_cfg.reward_item.ReadConfig(item_element))
		{
			return -101;
		}

		++ m_shop_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int GoldVipConfig::InitGoldVipShopMulCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_multiple_cost_id = 0;
	int last_max_times = 0;
	int last_price_multile = 0;
	int index = 0;

	while (!dataElement.empty())
	{
		int shop_seq = 0;
		int times_min = 0;
		int times_max = 0;
		int price_multile = 0;

		if (!PugiGetSubNodeValue(dataElement, "shop_seq", shop_seq) || (shop_seq < 0))
		{
			return -1;
		}

		if (shop_seq != last_multiple_cost_id)
		{
			last_max_times = 0;
			last_price_multile = 0;
			index = 0;
		}
		last_multiple_cost_id = shop_seq;

		if (!PugiGetSubNodeValue(dataElement, "times_min", times_min) || (last_max_times != 0 && times_min - last_max_times != 1))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "times_max", times_max) || (times_max < times_min))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "price_multile", price_multile) || (price_multile <= last_price_multile))
		{
			return -4;
		}

		last_max_times = times_max;

		if (index >= GOLD_VIP_CONVERT_SHOP_MUL_MAX_COUNT)
		{
			return -21;
		}

		GoldVipMultiCostList &multi_cost = m_multiplecost_map[shop_seq];
		GoldVipMultiCostList::CostItem *item = &multi_cost.item_list[index];
		item->multiple_cost_id = shop_seq;
		item->times_min = times_min;
		item->times_max = times_max;

		if (!PugiGetSubNodeValue(dataElement, "price_multile", item->price_multile) || item->price_multile < last_price_multile)
		{
			return -22;
		}
		last_price_multile = item->price_multile;

		++index;
		multi_cost.multiple_count = index;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const GoldVipActiveCfg * GoldVipConfig::GetGoldVipActiveCfg()
{
	return &m_active_cfg;
}

const GoldVipShopCfg::ConfigItem * GoldVipConfig::GetGoldVipShopRewardCfg(int seq)
{

	if (seq < 0 || seq > m_shop_cfg.cfg_count || seq >= GOLD_VIP_CONVERT_SHOP_MAX_COUNT)
	{
		return NULL;
	}

	return &m_shop_cfg.cfg_list[seq];
}

int GoldVipConfig::GetMultipleCost(int shop_seq, int times)
{
	std::map<int, GoldVipMultiCostList>::iterator iter = m_multiplecost_map.find(shop_seq);
	if (m_multiplecost_map.end() == iter)
	{
		return 1;
	}

	for (int i = 0; i < iter->second.multiple_count && i < GOLD_VIP_CONVERT_SHOP_MUL_MAX_COUNT; ++i)
	{
		GoldVipMultiCostList::CostItem &item = iter->second.item_list[i];
		if (times >= item.times_min && times <= item.times_max)
		{
			return item.price_multile;
		}
	}

	return 1;
}

