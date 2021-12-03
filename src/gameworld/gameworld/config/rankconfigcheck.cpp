#include "rankconfigcheck.hpp"

#include "servercommon/configcommon.h"

RankConfigCheck::RankConfigCheck()
{
	
}

RankConfigCheck::~RankConfigCheck()
{

}

bool RankConfigCheck::Init(const std::string &configname, std::string *err)
{
	PugiXmlDocument document;

	PUGI_XML_LOAD_AND_CHECK(document, configname, "RankConfigCheck", *err);

	PugiXmlNode root_element = document.document_element();
	if (root_element.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode honour_rank_reward_element = root_element.child("honour_rank_reward");
		if (honour_rank_reward_element.empty())
		{
			*err = configname + ": xml node error in [honour_rank_reward].";
			return false;
		}

		PugiXmlNode data_element = honour_rank_reward_element.child("data");
		if (data_element.empty())
		{
			*err = configname + ": xml node error in [honour_rank_reward->data] empty.";
			return false;
		}

		m_honour_rank_reward_count = 0;

		while (!data_element.empty())
		{
			if (m_honour_rank_reward_count >= MAX_HONOUR_RANK_REWARD_COUNT)
			{
				*err = configname + ": xml node error in [honour_rank_reward->data] too math.";
				return false;
			}

			int rank_pos = 0;
			if (!PugiGetSubNodeValue(data_element, "rank_pos", rank_pos) || rank_pos != m_honour_rank_reward_count + 1)
			{
				*err = configname + ": xml node error in [honour_rank_reward->data->rank_pos].";
				return false;
			}

			ItemConfigData &item = m_honour_rank_reward_list[m_honour_rank_reward_count];

			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty() || !item.ReadConfig(reward_item_element))
			{
				*err = configname + ": xml node error in [honour_rank_reward->data->reward_item].";
				return false;
			}

			++m_honour_rank_reward_count;

			data_element = data_element.next_sibling();
		}
	}

	return true;
}

