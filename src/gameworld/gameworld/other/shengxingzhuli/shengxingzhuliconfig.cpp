
#include "shengxingzhuliconfig.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "item/itempool.h"
#include "item/itembase.h"
#include <vector>

ShengxingzhuliConfig::ShengxingzhuliConfig()
{

}

bool ShengxingzhuliConfig::Init(const std::string &configname, std::string *err)
{ 
	char errinfo[1024] = { 0 };
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ShengxingzhuliConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 其他
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = InitShengxingzhuliOtherConfig(other_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitShengxingzhuliOtherConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 周几对应系统
		PugiXmlNode other_element = RootElement.child("weekday_to_system");
		if (other_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = InitShengxingzhuliWeekdaySystemConfig(other_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitShengxingzhuliWeekdaySystemConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const int ShengxingzhuliConfig::GetStageByChongzhi(int chongzhi)
{
	int stage = 0;
	for (int i = 0; i < NEED_CHONGZHI_FIRST_COUNT; ++i)
	{
		chongzhi -= m_other_cfg.need_chongzhi[i];
		if (chongzhi < 0)
		{
			return stage;
		}

		stage++;
	}

	stage = (chongzhi / m_other_cfg.add_value) + NEED_CHONGZHI_FIRST_COUNT;
	return stage;
}

const int ShengxingzhuliConfig::GetNeedChongZhiByStage(int stage)
{
	if (stage <= 0)
	{
		return 0;
	}

	int chongzhi = 0;
	for (int i = 0; i < NEED_CHONGZHI_FIRST_COUNT && i < stage; ++i)
	{
		chongzhi += m_other_cfg.need_chongzhi[i];
	}

	if (stage > NEED_CHONGZHI_FIRST_COUNT)
	{
		chongzhi = chongzhi + (stage - NEED_CHONGZHI_FIRST_COUNT) * m_other_cfg.add_value;
	}

	return chongzhi;
}

int ShengxingzhuliConfig::InitShengxingzhuliOtherConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");

	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "need_chongzhi_0", m_other_cfg.need_chongzhi[0]) || m_other_cfg.need_chongzhi[0] <= 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "need_chongzhi_1", m_other_cfg.need_chongzhi[1]) || m_other_cfg.need_chongzhi[1] <= 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "need_chongzhi_2", m_other_cfg.need_chongzhi[2]) || m_other_cfg.need_chongzhi[2] <= 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(data_element, "open_day", m_other_cfg.open_day) || m_other_cfg.open_day < 0)
		{
			return -2;
		}

		PugiXmlNode  item_list_element = data_element.child("rewards_item_list");
		if (item_list_element.empty()) return -3;

		std::vector<ItemConfigData> reward_list;
		PugiXmlNode rewardItemElement = item_list_element.child("rewards_item");
		while (!rewardItemElement.empty())
		{
			ItemConfigData item_config;
			if (!item_config.ReadConfig(rewardItemElement))
			{
				return -4;
			}
			reward_list.push_back(item_config);

			rewardItemElement = rewardItemElement.next_sibling();
		}

		int item_count = 0;
		for (std::vector<ItemConfigData>::iterator it = reward_list.begin(); reward_list.end() != it; ++it)
		{
			if (item_count >= SHENGXINGZHULI_SYSTEM_TYPE_COUNT)
			{
				break;
			}

			m_other_cfg.reward_item_list[item_count].item_id = it->item_id;
			m_other_cfg.reward_item_list[item_count].num = it->num;
			m_other_cfg.reward_item_list[item_count].is_bind = it->is_bind;

			++item_count;
		}

		if (!PugiGetSubNodeValue(data_element, "add_valus", m_other_cfg.add_value) || m_other_cfg.add_value < 0)
		{
			return -5;
		}

		if (item_count != SHENGXINGZHULI_SYSTEM_TYPE_COUNT)
		{
			return -6;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ShengxingzhuliConfig::InitShengxingzhuliWeekdaySystemConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");

	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "monday_sys", m_weekday_system_cfg.monday_sys) || m_weekday_system_cfg.monday_sys < 0 || m_weekday_system_cfg.monday_sys >= SHENGXINGZHULI_SYSTEM_TYPE_COUNT)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "tuesday_sys", m_weekday_system_cfg.tuesday_sys) || m_weekday_system_cfg.tuesday_sys < 0 || m_weekday_system_cfg.tuesday_sys >= SHENGXINGZHULI_SYSTEM_TYPE_COUNT)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "wednesday_sys", m_weekday_system_cfg.wednesday_sys) || m_weekday_system_cfg.wednesday_sys < 0 || m_weekday_system_cfg.wednesday_sys >= SHENGXINGZHULI_SYSTEM_TYPE_COUNT)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "thursday_sys", m_weekday_system_cfg.thursday_sys) || m_weekday_system_cfg.thursday_sys < 0 || m_weekday_system_cfg.thursday_sys >= SHENGXINGZHULI_SYSTEM_TYPE_COUNT)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "friday_sys", m_weekday_system_cfg.friday_sys) || m_weekday_system_cfg.friday_sys < 0 || m_weekday_system_cfg.friday_sys >= SHENGXINGZHULI_SYSTEM_TYPE_COUNT)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "saturday_sys", m_weekday_system_cfg.saturday_sys) || m_weekday_system_cfg.saturday_sys < 0 || m_weekday_system_cfg.saturday_sys >= SHENGXINGZHULI_SYSTEM_TYPE_COUNT)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "sunday_sys", m_weekday_system_cfg.sunday_sys) || m_weekday_system_cfg.sunday_sys < 0 || m_weekday_system_cfg.sunday_sys >= SHENGXINGZHULI_SYSTEM_TYPE_COUNT)
	{
		return -7;
	}

	return 0;
}