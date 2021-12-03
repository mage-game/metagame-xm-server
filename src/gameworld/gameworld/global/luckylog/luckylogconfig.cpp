#include "luckylogconfig.hpp"
#include "item/itempool.h"

bool LuckyLogConfig::Init(const std::string &configname, std::string *err)
{
	PRE_LOAD_CONFIG;

	LOAD_CONFIG("log_info", InitLogInfo);

	return true;
}

int LuckyLogConfig::InitLogInfo(TiXmlElement *root_element)
{
	TiXmlElement *data_element = root_element->FirstChildElement("data");
	if (nullptr == data_element)
	{
		return -1000;
	}

	int activity_type = 0;
	ItemID item_id = 0;
	while (nullptr != data_element)
	{
		if (!GetSubNodeValue(data_element, "activity_type", activity_type) || activity_type < 0)
		{
			return -1;
		}

		NeedLogItemVector &vec = m_need_log_item_map[activity_type];

		if (!GetSubNodeValue(data_element, "item_id", item_id) || NULL == ITEMPOOL->GetItem(item_id))
		{
			return -2;
		}

		vec.push_back(item_id);

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}


bool LuckyLogConfig::CheckNeedLog(int activity_type, ItemID item_id)
{
	auto it_map = m_need_log_item_map.find(activity_type);

	if (m_need_log_item_map.end() != it_map)
	{
		for (auto &it_vec : it_map->second)
		{
			if (it_vec == item_id)
			{
				return true;
			}
		}
	}

	return false;
}
