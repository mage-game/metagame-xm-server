#include "fishpoolconfig.hpp"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "item/itembase.h"

FishPoolConfig::FishPoolConfig() : m_fake_fish_quality_total_weight(0)
{
}

FishPoolConfig::~FishPoolConfig()
{

}

bool FishPoolConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "FishPoolConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// ÔÓÏîÅäÖÃ
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOtherCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ÓãÀàÆ·ÖÊ
		PugiXmlNode root_element = RootElement.child("fish_quality");
		if (root_element.empty())
		{
			*err = path + " xml not fish_quality node ";
			return false;
		}

		iRet = this->InitFishQualityTypeCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitFishQualityTypeCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ²¶Óã×Óµ¯¹ºÂò
		PugiXmlNode root_element = RootElement.child("buy_bullet");
		if (root_element.empty())
		{
			*err = path + " xml not buy_bullet node ";
			return false;
		}

		iRet = this->InitBuyBulletCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitBuyBulletCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ²¶Óã´ÎÊý¹ºÂò
		PugiXmlNode root_element = RootElement.child("buy_fang_fish_times");
		if (root_element.empty())
		{
			*err = path + " xml not buy_fang_fish_times node ";
			return false;
		}

		iRet = this->InitBuyFangFishTimesCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitBuyFangFishTimesCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ²¶Óã´ÎÊý¹ºÂò
		PugiXmlNode root_element = RootElement.child("fake_fish_quality_weight");
		if (root_element.empty())
		{
			*err = path + " xml not fake_fish_quality_weight node ";
			return false;
		}

		iRet = this->InitFakeFishQualityWeightCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitFakeFishQualityWeightCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// É¨µ´ÅäÖÃ
		PugiXmlNode root_element = RootElement.child("skip_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not skip_cfg node ";
			return false;
		}

		iRet = this->InitFishPoolSkipCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitFishPoolSkipCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int FishPoolConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "base_bullet_num", m_other_cfg.base_bullet_num) || m_other_cfg.base_bullet_num < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "buy_bullet_price", m_other_cfg.buy_bullet_price) || m_other_cfg.buy_bullet_price <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "give_bullet_per_buy", m_other_cfg.give_bullet_per_buy) || m_other_cfg.give_bullet_per_buy <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "day_buy_bullet_limit_times", m_other_cfg.day_buy_bullet_limit_times) || m_other_cfg.day_buy_bullet_limit_times <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "guard_fish_last_time_s", m_other_cfg.guard_fish_last_time_s) || m_other_cfg.guard_fish_last_time_s <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "today_free_fang_fish_times", m_other_cfg.today_free_fang_fish_times) || m_other_cfg.today_free_fang_fish_times <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "today_buy_fang_fish_times_limit", m_other_cfg.today_buy_fang_fish_times_limit) || m_other_cfg.today_buy_fang_fish_times_limit <= 0)
	{
		return -7;
	}

	return 0;
}

int FishPoolConfig::InitFishQualityTypeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int quality = 0;

	while (!dataElement.empty())
	{
		if (quality < 0 || quality >= FISH_QUALITY_COUNT)
		{
			return -10001;
		}

		FishQualityTypeCfg & quality_cfg = m_quality_typ_cfg[quality];

		if (!PugiGetSubNodeValue(dataElement, "quality", quality_cfg.quality) || quality_cfg.quality != quality)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "count", quality_cfg.count) || quality_cfg.count < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_time", quality_cfg.need_time) || quality_cfg.need_time < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold", quality_cfg.gold) || quality_cfg.gold < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "up_quality_pro", quality_cfg.up_quality_pro) || quality_cfg.up_quality_pro < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "exp", quality_cfg.exp) || quality_cfg.exp < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "rune_score", quality_cfg.rune_score) || quality_cfg.rune_score < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "steal_limit", quality_cfg.steal_limit) || quality_cfg.steal_limit < 0)
		{
			return -8;
		}

		PugiXmlNode Itemelement = dataElement.child("reward_item");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid))
		{
			if (!quality_cfg.reward_item.ReadConfig(Itemelement) && 0 != quality_cfg.reward_item.item_id)
			{
				return -9;
			}
		}
		else
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "be_steal_pro", quality_cfg.be_steal_pro) || quality_cfg.be_steal_pro < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "up_quality_gold", quality_cfg.up_quality_gold) || quality_cfg.up_quality_gold < 0)
		{
			return -12;
		}

		quality++;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FishPoolConfig::InitBuyBulletCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		BuyBulletCfg buy_bullet_cfg;

		if (!PugiGetSubNodeValue(dataElement, "bullet_count", buy_bullet_cfg.bullet_count) || buy_bullet_cfg.bullet_count < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold", buy_bullet_cfg.gold) || buy_bullet_cfg.gold < 0)
		{
			return -2;
		}

		m_buy_buttle_cfg.push_back(buy_bullet_cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FishPoolConfig::InitBuyFangFishTimesCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		BuyFangFishTimesCfg buy_fang_fish_cfg;

		if (!PugiGetSubNodeValue(dataElement, "buy_fang_fish_count", buy_fang_fish_cfg.buy_fang_fish_count) || buy_fang_fish_cfg.buy_fang_fish_count < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "gold", buy_fang_fish_cfg.gold) || buy_fang_fish_cfg.gold < 0)
		{
			return -2;
		}

		m_buy_fang_fish_times_cfg.push_back(buy_fang_fish_cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FishPoolConfig::InitFakeFishQualityWeightCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		FakeFishQualityWeightCfg quality_weight_cfg;

		if (!PugiGetSubNodeValue(dataElement, "quality", quality_weight_cfg.quality) || quality_weight_cfg.quality < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "weight", quality_weight_cfg.weight) || quality_weight_cfg.weight < 0)
		{
			return -2;
		}

		m_fake_fish_quality_total_weight += quality_weight_cfg.weight;

		m_fake_fish_quality_cfg.push_back(quality_weight_cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FishPoolConfig::InitFishPoolSkipCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		FakeFishQualityWeightCfg quality_weight_cfg;

		int type = -1;
		if (!PugiGetSubNodeValue(dataElement, "type", type) || type < 0)
		{
			return -1;
		}

		if (0 == type)
		{
			int quality = -1;
			if (!PugiGetSubNodeValue(dataElement, "quality", quality) || quality < 0 || quality >= FISH_QUALITY_COUNT)
			{
				return -2;
			}

			FishSkipCfg &cfg = m_fish_pool_skip_cfg.fish_skip_cfg[quality];
			cfg.skip_fish_quality = quality;

			if (!PugiGetSubNodeValue(dataElement, "limit_level", cfg.limit_level) || cfg.limit_level <= 0)
			{
				return -3;
			}

			if (!PugiGetSubNodeValue(dataElement, "consume", cfg.consume) || cfg.consume <= 0)
			{
				return -4;
			}
		}
		else if (1 == type)
		{
			if (!PugiGetSubNodeValue(dataElement, "limit_level", m_fish_pool_skip_cfg.skip_steal_level_limit) || m_fish_pool_skip_cfg.skip_steal_level_limit <= 0)
			{
				return -4;
			}

			if (!PugiGetSubNodeValue(dataElement, "consume", m_fish_pool_skip_cfg.skip_steal_consume) || m_fish_pool_skip_cfg.skip_steal_consume <= 0)
			{
				return -5;
			}
		}
		else
		{
			return -6;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

FishQualityTypeCfg * FishPoolConfig::GetFishQualityCfg(int quality)
{
	if (quality < 0 || quality >= FISH_QUALITY_COUNT)
	{
		return NULL;
	}
	return &m_quality_typ_cfg[quality];
}

int FishPoolConfig::GetBuyBulletGold(int times)
{
	for (int i = 0; i < static_cast<int>(m_buy_buttle_cfg.size()); i++)
	{
		if(m_buy_buttle_cfg[i].bullet_count == times)
		{
			return m_buy_buttle_cfg[i].gold;
		}
	}
	return -1;
}

int FishPoolConfig::GetFakeFishQuality()
{
	if (m_fake_fish_quality_total_weight <= 0)
	{
		return 0;
	}

	int rand_num = RandomNum(m_fake_fish_quality_total_weight);

	for (int i = 0; i < static_cast<int>(m_fake_fish_quality_cfg.size()); ++i)
	{
		if (rand_num < m_fake_fish_quality_cfg[i].weight)
		{
			return m_fake_fish_quality_cfg[i].quality;
		}

		rand_num -= m_fake_fish_quality_cfg[i].weight;
	}

	return 0;
}

int FishPoolConfig::GetBuyFangFishGold(int times)
{
	for (int i = 0; i < static_cast<int>(m_buy_fang_fish_times_cfg.size()); i++)
	{
		if (m_buy_fang_fish_times_cfg[i].buy_fang_fish_count == times)
		{
			return m_buy_fang_fish_times_cfg[i].gold;
		}
	}
	return -1;
}

