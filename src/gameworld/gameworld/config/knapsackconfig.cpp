#include "knapsackconfig.hpp"

#include "servercommon/configcommon.h"

KnapsackConfig::KnapsackConfig() : m_extend_begin_level(0)
{
	memset(m_extend_online_time_list, 0, sizeof(m_extend_online_time_list));
}

KnapsackConfig::~KnapsackConfig()
{

}

bool KnapsackConfig::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "KnapsackConfig", *err);

	PugiXmlNode root_element = document.document_element();
	if (root_element.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode other_element = root_element.child("other");
		if (other_element.empty())
		{
			*err = path + ": xml node error in [config->other].";
			return false;
		}

		PugiXmlNode data_element = other_element.child("data");
		if (data_element.empty())
		{
			*err = path + ": xml node error in [config->other->data].";
			return false;
		}

		if (!PugiGetSubNodeValue(data_element, "extend_begin_level", m_extend_begin_level) || m_extend_begin_level <= 0)
		{
			*err = path + ": xml node error in [config->other->data->extend_begin_level].";
			return false;
		}
	}

	{
		PugiXmlNode time_extend_element = root_element.child("time_extend");
		if (time_extend_element.empty())
		{
			*err = path + ": xml node error in [config->time_extend].";
			return false;
		}

		PugiXmlNode data_element = time_extend_element.child("data");
		if (data_element.empty())
		{
			*err = path + ": xml node error in [config->time_extend->data] empty.";
			return false;
		}

		int index = 0;

		while (!data_element.empty())
		{
			if (index < 0 || index >= ItemNamespace::MAX_KNAPSACK_GRID_NUM)
			{
				*err = path + ": xml node error in [config->time_extend->data] too many.";
				return false;
			}

			int temp_index = 0;
			if (!PugiGetSubNodeValue(data_element, "grid_index", temp_index) || temp_index != index + 1)
			{
				*err = path + ": xml node error in [config->time_extend->data->grid_index].";
				return false;
			}

			if (!PugiGetSubNodeValue(data_element, "online_time", m_extend_online_time_list[index]) || m_extend_online_time_list[index] <= 0)
			{
				*err = path + ": xml node error in [config->time_extend->data->online_time].";
				return false;
			}

			++index;
			data_element = data_element.next_sibling();
		}

		if (index <= 0) return false;

		for (int i = index; i < ItemNamespace::MAX_KNAPSACK_GRID_NUM; ++i)
		{
			m_extend_online_time_list[i] = m_extend_online_time_list[i - 1];
		}
	}

	return true;
}

int KnapsackConfig::GetExtendOnlineTime(int grid_index)
{
	if (grid_index < 0 || grid_index >= ItemNamespace::MAX_KNAPSACK_GRID_NUM) return 0;

	return m_extend_online_time_list[grid_index];
}

