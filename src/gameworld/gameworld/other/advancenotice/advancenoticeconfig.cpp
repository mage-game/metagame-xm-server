#include "advancenoticeconfig.hpp"

#include "servercommon/configcommon.h"
#include "servercommon/commondata.hpp"
#include "item/itempool.h"

AdvanceNoticeConfig::AdvanceNoticeConfig()
{

}

AdvanceNoticeConfig::~AdvanceNoticeConfig()
{

}

bool AdvanceNoticeConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "AdvanceNoticeConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// 引导配置
		PugiXmlNode root_element = RootElement.child("notice_list");
		if (root_element.empty())
		{
			*err = path + " xml not notice_list node ";
			return false;
		}

		iRet = this->InitAdvanceNoticeGuideCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitAdvanceNoticeGuideCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 天数预告
		PugiXmlNode root_element = RootElement.child("notice_day");
		if (root_element.empty())
		{
			*err = path + " xml not notice_day node ";
			return false;
		}

		iRet = this->InitAdvanceDayCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitAdvanceDayCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

const AdvanceNoticeGuideCfg * AdvanceNoticeConfig::GetAdvanceNoticeCfg(int id)
{
	if (id <= 0)
	{
		return NULL;
	}

	std::map<int, AdvanceNoticeGuideCfg>::iterator iter = m_advance_notice_map.find(id);
	if (iter != m_advance_notice_map.end())
	{
		return &iter->second;
	}

	return NULL;
}

const AdvanceNoticeDayCfg * AdvanceNoticeConfig::GetAdvanceNoticeDayCfg(int id)
{
	if (id <= 0)
	{
		return NULL;
	}

	std::map<int, AdvanceNoticeDayCfg>::iterator iter = m_advance_notice_day_map.find(id);
	if (iter != m_advance_notice_day_map.end())
	{
		return &iter->second;
	}

	return NULL;
}

int AdvanceNoticeConfig::InitAdvanceNoticeGuideCfg(PugiXmlNode RootElement)
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

	int last_id = 0;
	while (!data_element.empty())
	{
		AdvanceNoticeGuideCfg cfg;
		if (!PugiGetSubNodeValue(data_element, "id", cfg.id) || cfg.id <= 0 || m_advance_notice_map.find(cfg.id) != m_advance_notice_map.end() || cfg.id != last_id + 1)
		{
			return -1;
		}

		last_id = cfg.id;

		if (!PugiGetSubNodeValue(data_element, "start_level", cfg.start_level) || cfg.start_level < 0 || cfg.start_level > MAX_ROLE_LEVEL)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "end_level", cfg.end_level) || cfg.end_level < 0 || cfg.end_level > MAX_ROLE_LEVEL || cfg.end_level < cfg.start_level)
		{
			return -3;
		}

		{
			PugiXmlNode element = data_element.child("reward_item_list");
			if (element.empty())
			{
				return -100;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("reward_item");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -101 - i;
				}

				if (!cfg.reward_item[i].ReadConfig(item_element))
				{
					if (0 == cfg.reward_item[i].item_id)
					{
						break;
					}

					return - 151 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}

		m_advance_notice_map[cfg.id] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int AdvanceNoticeConfig::InitAdvanceDayCfg(PugiXmlNode RootElement)
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

	int last_id = 0;
	while (!data_element.empty())
	{
		AdvanceNoticeDayCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "id", cfg.id) || cfg.id <= 0 || cfg.id != last_id + 1
			|| m_advance_notice_day_map.find(cfg.id) != m_advance_notice_day_map.end() ||
			cfg.id >= CommonDataParam::NOTICE_DAY_MAX_ID)
		{
			return -1;
		}
		last_id = cfg.id;

		if (!PugiGetSubNodeValue(data_element, "open_day", cfg.open_day) || cfg.open_day <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "level_limit", cfg.level_limit) || cfg.level_limit < 0 || cfg.level_limit > MAX_ROLE_LEVEL)
		{
			return -3;
		}

		{
			int reward_count = ItemConfigData::ReadConfigList(data_element, std::string("reward_item"),
				cfg.reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
			if (reward_count < 0)
			{
				return -100 + reward_count;
			}
		}

		m_advance_notice_day_map[cfg.id] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}
