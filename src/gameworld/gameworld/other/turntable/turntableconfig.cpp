#include "other/turntable/turntableconfig.h"
#include "servercommon/servercommon.h"
#include "globalconfig/equipconfig.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "obj/character/attribute.hpp"
#include "gameworld/checkresourcecenter.hpp"

TurnTableConfig::TurnTableConfig() : m_roll_cfg_count(0)
{

}

TurnTableConfig::~TurnTableConfig()
{
	
}

bool TurnTableConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "TurnTableConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	//读取幸运值和所需条件
	PugiXmlNode roll_cfg_element = RootElement.child("roll_cfg");
	if (roll_cfg_element.empty())
	{
		sprintf(errinfo, "%s: xml node error in [config->roll_cfg]", path.c_str());
		*err = errinfo;
		return false;
	}

	iRet = this->InitTurnTableRollCfg(roll_cfg_element);
	if (0 != iRet)
	{
		sprintf(errinfo,"%s: InitTurnTableRollCfg failed %d", path.c_str(), iRet);
		*err = errinfo;
		return false;
	}

	//读取转盘物品
	PugiXmlNode reward_cfg_element = RootElement.child("reward_cfg");
	if (reward_cfg_element.empty())
	{
		sprintf(errinfo, "%s: xml node error in [config->reward_cfg]", path.c_str());
		*err = errinfo;
		return false;
	}

	iRet = this->InitTurnTableRewardsCfg(reward_cfg_element);
	if (0 != iRet)
	{
		sprintf(errinfo,"%s: InitTurnTableRewardsCfg failed %d", path.c_str(), iRet);
		*err = errinfo;
		return false;
	}

	return true;
}

int TurnTableConfig::InitTurnTableRewardsCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");

	int last_item_index_list[TURNTABLE_TYPE_MAX_COUNT];

	for (int i = 0; i < TURNTABLE_TYPE_MAX_COUNT; ++ i)
	{
		last_item_index_list[i] = -1;
	}

	while (!data_element.empty())
	{
		int type = 0;
		if (!PugiGetSubNodeValue(data_element, "turntable_type", type) || type < 0 || type >= m_roll_cfg_count)
		{
			return -1;
		}

		int item_index = 0;
		if (!PugiGetSubNodeValue(data_element, "item_index", item_index) || item_index < 0 || item_index >= TURNTABLE_REWARDS_MAX_COUNT)
		{
			return -2;
		}

		if (item_index != last_item_index_list[type] + 1)
		{
			return -100;
		}
		last_item_index_list[type] = item_index;

		m_roll_cfg_list[type].rewards[item_index].item_index = item_index;

		PugiXmlNode reward_item_list_element = data_element.child("reward_item");
		if (reward_item_list_element.empty())
		{
			return -3;
		}

		if (!m_roll_cfg_list[type].rewards[item_index].item.ReadConfig(reward_item_list_element))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", m_roll_cfg_list[type].rewards[item_index].weight) || m_roll_cfg_list[type].rewards[item_index].weight <= 0)
		{
			return -5;
		}

		int tmp_notice = 0;
		if (!PugiGetSubNodeValue(data_element, "is_notice", tmp_notice) || tmp_notice < 0)
		{
			return -6;
		}

		m_roll_cfg_list[type].rewards[item_index].is_notice = (0 != tmp_notice);

		data_element = data_element.next_sibling();
	}

	for (int i = 0; i < m_roll_cfg_count; ++ i)
	{
		if (last_item_index_list[i] != TURNTABLE_REWARDS_MAX_COUNT - 1)
		{
			return -10;
		}
	}

	return 0;
}

int TurnTableConfig::InitTurnTableRollCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");

	int last_type = -1;

	while (!data_element.empty())
	{
		int type = 0;
		if (!PugiGetSubNodeValue(data_element, "turntable_type", type) || type < 0 || type >= TURNTABLE_TYPE_MAX_COUNT || type != last_type + 1)
		{
			return -1;
		}
		last_type = type;

		m_roll_cfg_list[type].type = type;

		if (!PugiGetSubNodeValue(data_element, "lucky_max", m_roll_cfg_list[type].lucky_max) || m_roll_cfg_list[type].lucky_max <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "great_item_index", m_roll_cfg_list[type].great_index) || m_roll_cfg_list[type].great_index < 0
			|| m_roll_cfg_list[type].great_index >= TURNTABLE_REWARDS_MAX_COUNT)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "need_item_id", m_roll_cfg_list[type].need_item) || NULL == ItemPool::Instance()->GetItem(m_roll_cfg_list[type].need_item))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "need_item_num", m_roll_cfg_list[type].need_num) || m_roll_cfg_list[type].need_num <= 0)
		{
			return -5;
		}

		++ m_roll_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const TurnTableRollCfg * TurnTableConfig::GetRollCfgList(int req_type)
{
	if (req_type >= 0 && req_type < m_roll_cfg_count)
	{
		return &m_roll_cfg_list[req_type];
	}

	return NULL;
}


