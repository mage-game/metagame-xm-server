#include "zerogiftconfig.hpp"

#include "servercommon/configcommon.h"
#include "item/itempool.h"

ZeroGifgConfig::ZeroGifgConfig(): m_zero_gift_pahse_max(0), m_zero_gift_god_costume_max(0)
{

}

ZeroGifgConfig::~ZeroGifgConfig()
{

}

bool ZeroGifgConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "ZeroGifgConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// ½×¶ÎÅäÖÃ
		PugiXmlNode root_element = RootElement.child("phase_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not phase_cfg node ";
			return false;
		}

		iRet = this->InitPhaseCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitPhaseCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ËÄ½×Éñ×°ÅäÖÃ
		PugiXmlNode root_element = RootElement.child("god_costume_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not god_costume_cfg node ";
			return false;
		}

		iRet = this->InitGodCostumeCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitGodCostumeCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

const ZeroGiftPhaseCfg * ZeroGifgConfig::GetPhaseCfg(int seq)
{
	if (seq < 0 || seq >= ZERO_GIFT_PHASE_MAX || seq >= m_zero_gift_pahse_max) return NULL;

	ZeroGiftPhaseCfg &cfg = m_phase_cfg[seq];

	if (cfg.seq != seq) return NULL;

	return &cfg;
}

const ZeroGiftGodCostumeCfg * ZeroGifgConfig::GetGodCostumeCfg(int seq)
{
	if (seq < 0 || seq >= ZERO_GIFT_GOD_COSTUME_MAX || seq >= m_zero_gift_god_costume_max) return NULL;

	ZeroGiftGodCostumeCfg &cfg = m_god_costume_cfg[seq];

	if (cfg.seq != seq) return NULL;

	return &cfg;
}

int ZeroGifgConfig::InitPhaseCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	m_zero_gift_pahse_max = 0;
	int last_seq = -1;
	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= ZERO_GIFT_PHASE_MAX || seq != last_seq + 1)
		{
			return -1;
		}

		ZeroGiftPhaseCfg &cfg = m_phase_cfg[seq];
		cfg.seq = seq;
		last_seq = seq;

		if (!PugiGetSubNodeValue(data_element, "level_limit", cfg.level_limit) || cfg.level_limit < 0 || cfg.level_limit > MAX_ROLE_LEVEL)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "vaild_buy_time", cfg.vaild_buy_time) || cfg.vaild_buy_time <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "buy_gold", cfg.buy_gold) || cfg.buy_gold < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "need_wait_time", cfg.need_wait_time) || cfg.need_wait_time < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_gold", cfg.reward_gold) || cfg.reward_gold < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "active_level", cfg.active_level) || cfg.active_level < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "is_bind_gold", cfg.is_bind_gold) || cfg.is_bind_gold < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "open_day", cfg.open_day) || cfg.open_day < 0)
		{
			return -9;
		}

		{
			PugiXmlNode element = data_element.child("gift_item_list");
			if (element.empty())
			{
				return -100;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("gift_item");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -101 - i;
				}

				if (!cfg.gift_item[i].ReadConfig(item_element))
				{
					return - 151 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}

		{
			for (int i = 0; i < ZeroGiftPhaseCfg::REWARD_DAY_COUNT; ++i)
			{
				char cfg_string[32] = { 0 };
				SNPRINTF(cfg_string, sizeof(cfg_string), "reward_item_list_%d", i);

				int count = ItemConfigData::ReadConfigList(data_element, cfg_string, cfg.reward_item_list[i], MAX_ATTACHMENT_ITEM_NUM);
				if (count < 0)
				{
					return -1000 + count;
				}
			}
		}

		++m_zero_gift_pahse_max;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ZeroGifgConfig::InitGodCostumeCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	m_zero_gift_god_costume_max = 0;
	int last_seq = -1;
	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= ZERO_GIFT_GOD_COSTUME_MAX || seq != last_seq + 1)
		{
			return -1;
		}

		ZeroGiftGodCostumeCfg &cfg = m_god_costume_cfg[seq];
		cfg.seq = seq;
		last_seq = seq;

		if (!PugiGetSubNodeValue(data_element, "open_day", cfg.open_day) || cfg.open_day < 0)
		{
			return -20;
		}

		for (int i = 0; i < ZERO_GIFT_GOD_LEVEL_PHASE; ++i)
		{
			std::string level_limit = "level_limit_" + std::to_string(i);
			if (!PugiGetSubNodeValue(data_element, level_limit, cfg.level_limit[i]) || cfg.level_limit[i] < 0 || cfg.level_limit[i] > MAX_ROLE_LEVEL)
			{
				return -2 - (i * 100);
			}
		}

		if (!PugiGetSubNodeValue(data_element, "vaild_buy_time", cfg.vaild_buy_time) || cfg.vaild_buy_time <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "buy_gold", cfg.buy_gold) || cfg.buy_gold < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "need_wait_time", cfg.need_wait_time) || cfg.need_wait_time < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_gold", cfg.reward_gold) || cfg.reward_gold < 0)
		{
			return -6;
		}

		for (int i = 0; i < ZERO_GIFT_GOD_LEVEL_PHASE; ++i)
		{
			std::string active_level = "active_level_" + std::to_string(i);
			if (!PugiGetSubNodeValue(data_element, active_level, cfg.active_level[i]) || cfg.active_level[i] < 0 || cfg.active_level[i] > MAX_ROLE_LEVEL)
			{
				return -7 - (i * 100);
			}
		}

		if (!PugiGetSubNodeValue(data_element, "is_bind_gold", cfg.is_bind_gold) || cfg.is_bind_gold < 0)
		{
			return -8;
		}

		{
			PugiXmlNode element = data_element.child("gift_item_list");
			if (element.empty())
			{
				return -100;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("gift_item");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -101 - i;
				}

				if (!cfg.gift_item[i].ReadConfig(item_element))
				{
					return -151 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}

		{
			for (int i = 0; i < ZERO_GIFT_GOD_LEVEL_PHASE; ++i)
			{
				char cfg_string[32] = { 0 };
				SNPRINTF(cfg_string, sizeof(cfg_string), "reward_item_list_%d", i);

				int count = ItemConfigData::ReadConfigList(data_element, cfg_string, cfg.reward_item_list[i], MAX_ATTACHMENT_ITEM_NUM);
				if (count < 0)
				{
					return -1000 + count;
				}
			}
		}

		++m_zero_gift_god_costume_max;
		data_element = data_element.next_sibling();
	}
	return 0;
}
