#include "discountbuyconfig.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"

DiscountBuyConfig::DiscountBuyConfig() 
{

}

DiscountBuyConfig::~DiscountBuyConfig()
{

}

bool DiscountBuyConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "DiscountBuyConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// 阶段配置
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
		// 物品配置
		PugiXmlNode root_element = RootElement.child("item_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not item_cfg node ";
			return false;
		}

		iRet = this->InitItemCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitItemCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

const DiscountBuyPhaseConfig::ConfigItem * DiscountBuyConfig::GetPhaseCfg(int phase)
{
	if (phase >= 0 && phase < m_pahse_cfg.cfg_count && phase < DISCOUNT_BUY_PHASE_MAX_COUNT)
	{
		return &m_pahse_cfg.cfg_list[phase];
	}

	return NULL;
}

const DiscountBuyItemConfig::ConfigItem * DiscountBuyConfig::GetItemCfg(int seq)
{
	if (seq >= 0 && seq < m_item_cfg.cfg_count && seq < DiscountBuyItemConfig::CONFIG_ITEM_MAX_COUNT)
	{
		return &m_item_cfg.cfg_list[seq];
	}

	return NULL;
}

int DiscountBuyConfig::InitPhaseCfg(PugiXmlNode RootElement)
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

	m_pahse_cfg.cfg_count = 0;

	int last_active_level = 0;

	while (!data_element.empty())
	{
		int phase = 0;
		if (!PugiGetSubNodeValue(data_element, "phase", phase) || phase != m_pahse_cfg.cfg_count || phase >= DISCOUNT_BUY_PHASE_MAX_COUNT)
		{
			return -1;
		}

		DiscountBuyPhaseConfig::ConfigItem &cfg_item = m_pahse_cfg.cfg_list[m_pahse_cfg.cfg_count];
		cfg_item.phase = phase;

		if (!PugiGetSubNodeValue(data_element, "active_level", cfg_item.active_level) || cfg_item.active_level < last_active_level)
		{
			return -2;
		}
		last_active_level = cfg_item.active_level;

		if (!PugiGetSubNodeValue(data_element, "last_time", cfg_item.last_time_s) || cfg_item.last_time_s <= 0)
		{
			return -3;
		}

		PugiXmlNode item_element = data_element.child("notice_mail_item");
		if (NULL == item_element)
		{
			return -4;
		}

		if (!cfg_item.notice_mail_item.ReadConfig(item_element))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "begin_open_day", cfg_item.begin_open_day) || cfg_item.begin_open_day < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "end_open_day", cfg_item.end_open_day) || cfg_item.end_open_day < 0)
		{
			return -7;
		}

		if (cfg_item.end_open_day < cfg_item.begin_open_day)
		{
			return -8;
		}

		++ m_pahse_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int DiscountBuyConfig::InitItemCfg(PugiXmlNode RootElement)
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

	m_item_cfg.cfg_count = 0;

	int last_item_seq[DISCOUNT_BUY_PHASE_MAX_COUNT];
	
	for (int i = 0; i < DISCOUNT_BUY_PHASE_MAX_COUNT; ++ i)
	{
		last_item_seq[i] = -1;
	}

	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_item_cfg.cfg_count || seq >= DiscountBuyItemConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}

		DiscountBuyItemConfig::ConfigItem &cfg_item = m_item_cfg.cfg_list[m_item_cfg.cfg_count];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "phase", cfg_item.phase) || cfg_item.phase < 0 || 
			cfg_item.phase >= DISCOUNT_BUY_PHASE_MAX_COUNT || cfg_item.phase >= m_pahse_cfg.cfg_count)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "item_seq", cfg_item.item_seq) || cfg_item.item_seq != last_item_seq[cfg_item.phase] + 1)
		{
			return -3;
		}
		last_item_seq[cfg_item.phase] = cfg_item.item_seq;

		if (!PugiGetSubNodeValue(data_element, "price", cfg_item.price) || cfg_item.price < 0)			//策划需求，一折抢购可配置免费，2019.3.29
		{
			return -4;
		}

		//if (!PugiGetSubNodeValue(data_element, "buy_limit_count", cfg_item.buy_limit_count) || cfg_item.buy_limit_count <= 0 || cfg_item.buy_limit_count > 100)
		if (!PugiGetSubNodeValue(data_element, "buy_limit_count", cfg_item.buy_limit_count) || cfg_item.buy_limit_count <= 0 || cfg_item.buy_limit_count > SHRT_MAX)
		{
			return -5;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -6;
		}

		if (!cfg_item.item.ReadConfig(item_element))
		{
			return -7;
		}

		++ m_item_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	for (int i = 0; i < DISCOUNT_BUY_PHASE_MAX_COUNT && i < m_pahse_cfg.cfg_count; ++ i)
	{
		if (DISCOUNT_BUY_ITEM_PER_PHASE <= last_item_seq[i])
		{
			return -30000;
		}
	}

	return 0;
}


