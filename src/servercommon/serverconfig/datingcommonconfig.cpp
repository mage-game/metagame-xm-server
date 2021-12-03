#include "datingcommonconfig.hpp"

DatingCommonConfig::DatingCommonConfig()
	: m_dating_scene_id(0), m_gold_refresh_target_list_cost(0), m_present_more_exp_cost_coin(0), m_present_double_exp_cost_gold(0), m_invite_cost_gold(0), 
	m_reward_exp_base(0), m_reward_coin_bind_base(0), m_reward_intimacy_base(0), m_update_add_exp_pos(0, 0), m_update_add_exp_range(0), m_update_add_exp(0), 
	m_question_num(0), m_update_add_exp_count(0)
{
}

DatingCommonConfig::~DatingCommonConfig()
{

}

bool DatingCommonConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "DatingCommonConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo, "Load XianBang config [%s] Error.cannot find root node.", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "dating_scene_id", m_dating_scene_id) || m_dating_scene_id <= 0)
	{
		sprintf(errinfo,"%s:xml node error in node [config->other->data->dating_scene_id]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "update_add_exp_pos_x", m_update_add_exp_pos.x) || m_update_add_exp_pos.x <= 0)
	{
		sprintf(errinfo,"%s:xml node error in node [config->other->data->update_add_exp_pos_x]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "update_add_exp_pos_y", m_update_add_exp_pos.y) || m_update_add_exp_pos.y <= 0)
	{
		sprintf(errinfo,"%s:xml node error in node [config->other->data->update_add_exp_pos_y]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "update_add_exp_range", m_update_add_exp_range) || m_update_add_exp_range <= 0)
	{
		sprintf(errinfo,"%s:xml node error in node [config->other->data->update_add_exp_range]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "update_add_exp", m_update_add_exp) || m_update_add_exp <= 0)
	{
		sprintf(errinfo,"%s:xml node error in node [config->other->data->update_add_exp]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "gold_refresh_target_list_cost", m_gold_refresh_target_list_cost) || m_gold_refresh_target_list_cost <= 0)
	{
		sprintf(errinfo,"%s:xml node error in node [config->other->data->gold_refresh_target_list_cost]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "present_more_exp_cost_coin", m_present_more_exp_cost_coin) || m_present_more_exp_cost_coin <= 0)
	{
		sprintf(errinfo,"%s:xml node error in node [config->other->data->present_more_exp_cost_coin]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "present_double_exp_cost_gold", m_present_double_exp_cost_gold) || m_present_double_exp_cost_gold <= 0)
	{
		sprintf(errinfo,"%s:xml node error in node [config->other->data->present_double_exp_cost_gold]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "invite_cost_gold", m_invite_cost_gold) || m_invite_cost_gold <= 0)
	{
		sprintf(errinfo,"%s:xml node error in node [config->other->data->invite_cost_gold]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "reward_exp_base", m_reward_exp_base) || m_reward_exp_base < 0)
	{
		sprintf(errinfo,"%s:xml node error in node [config->other->data->reward_exp_base]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "reward_coin_bind_base", m_reward_coin_bind_base) || m_reward_coin_bind_base < 0)
	{
		sprintf(errinfo,"%s:xml node error in node [config->other->data->reward_coin_bind_base]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "reward_intimacy_base", m_reward_intimacy_base) || m_reward_intimacy_base < 0)
	{
		sprintf(errinfo,"%s:xml node error in node [config->other->data->reward_intimacy_base]", path.c_str());
		*err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "question_num", m_question_num) || m_question_num <= 0)
	{
		sprintf(errinfo,"%s:xml node error in node [config->other->data->question_num]", path.c_str());
		*err = errinfo;
		return false;
	}

	{
		PugiXmlNode more_exp_present_item_element = RootElement.child("more_exp_present_item");
		if (more_exp_present_item_element.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->more_exp_present_item]", path.c_str());
			*err = errinfo;
			return false;
		}

		if (!m_more_exp_present_item.ReadConfigNoCheck(more_exp_present_item_element))
		{
			sprintf(errinfo,"%s:xml node error in node [config->more_exp_present_item]", path.c_str());
			*err = errinfo;
			return false;
		}

		PugiXmlNode double_exp_present_item_element = RootElement.child("double_exp_present_item");
		if (double_exp_present_item_element.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->double_exp_present_item]", path.c_str());
			*err = errinfo;
			return false;
		}

		if (!m_double_exp_present_item.ReadConfigNoCheck(double_exp_present_item_element))
		{
			sprintf(errinfo,"%s:xml node error in node [config->double_exp_present_item]", path.c_str());
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode update_add_exp_list_element = RootElement.child("update_add_exp_list");
		if (update_add_exp_list_element.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->update_add_exp_list]", path.c_str());
			*err = errinfo;
			return false;
		}

		PugiXmlNode add_exp_item_element = update_add_exp_list_element.child("add_exp_item");
		if (add_exp_item_element.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->update_add_exp_list->add_exp_item]", path.c_str());
			*err = errinfo;
			return false;
		}

		int count = 0;
		while (!add_exp_item_element.empty())
		{
			if (count >= DATING_UPDATE_ADD_EXP_ITEM_COUNT)
			{
				sprintf(errinfo,"%s:xml node error in node [config->update_add_exp_list->add_exp_item]", path.c_str());
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(add_exp_item_element, "low_level", m_update_add_exp_list[count].low_level) || m_update_add_exp_list[count].low_level <= 0)
			{
				sprintf(errinfo,"%s:xml node error in node [config->update_add_exp_list->low_level]", path.c_str());
				*err = errinfo;
				return false;
			}

			if (count > 0 && m_update_add_exp_list[count].low_level <= m_update_add_exp_list[count - 1].high_level)
			{
				sprintf(errinfo,"%s:xml node error in node [config->update_add_exp_list->low_level]", path.c_str());
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(add_exp_item_element, "high_level", m_update_add_exp_list[count].high_level) || m_update_add_exp_list[count].high_level < m_update_add_exp_list[count].low_level)
			{
				sprintf(errinfo,"%s:xml node error in node [config->update_add_exp_list->high_level]", path.c_str());
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(add_exp_item_element, "add_exp", m_update_add_exp_list[count].add_exp) || m_update_add_exp_list[count].add_exp <= 0)
			{
				sprintf(errinfo,"%s:xml node error in node [config->update_add_exp_list->add_exp]", path.c_str());
				*err = errinfo;
				return false;
			}

			++ count;
			add_exp_item_element = add_exp_item_element.next_sibling();
		}
		m_update_add_exp_count = count;
	}

	return true;
}

const ItemConfigData * DatingCommonConfig::GetDatingPresentItem(int present_type)
{
	if (DATING_PRESENT_TYPE_MORE_EXP == present_type)
	{
		return &m_more_exp_present_item;
	}
	else if (DATING_PRESENT_TYPE_DOUBLE_EXP == present_type)
	{
		return &m_double_exp_present_item;
	}

	return NULL;
}

bool DatingCommonConfig::GetDatingUpdateAddExpPos(Posi &pos, int *range)
{
	if (NULL == range) return false;

	pos = m_update_add_exp_pos;
	*range = m_update_add_exp_range;

	return true;
}

int DatingCommonConfig::GetDatingUpdateAddExp(int level)
{
	for (int i = m_update_add_exp_count - 1; i >= 0; i--)
	{
		if (m_update_add_exp_list[i].low_level <= level && level <= m_update_add_exp_list[i].high_level)
		{
			return m_update_add_exp_list[i].add_exp;
		}
	}

	return 0;
}

