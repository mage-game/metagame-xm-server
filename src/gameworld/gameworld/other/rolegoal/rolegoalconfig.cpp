#include "rolegoalconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "engineadapter.h"
#include "servercommon/serverconfig/crossconfig.hpp"

RoleGoalConfig::RoleGoalConfig()
{

}

RoleGoalConfig::~RoleGoalConfig()
{

}


bool RoleGoalConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "RoleGoalConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

// 	{
// 		// 个人目标配置
// 		PugiXmlNode root_element = RootElement.child("personal_goal");
// 		if (root_element.empty())
// 		{
// 			*err = path + " xml not personal_goal node ";
// 			return false;
// 		}
// 
// 		iRet = this->InitPersonalGoalChapterCfg(root_element);
// 		if (0 != iRet)
// 		{
// 			sprintf(err_info, "%s InitPersonalGoalChapterCfg fail %d ", path.c_str(), iRet);
// 			*err = err_info;
// 			return false;
// 		}
// 	}
// 
// 	{
// 		// 战场目标配置
// 		PugiXmlNode root_element = RootElement.child("battlefield_goal");
// 		if (root_element.empty())
// 		{
// 			*err = path + " xml not battlefield_goal node ";
// 			return false;
// 		}
// 
// 		iRet = this->InitBattleFieldCfg(root_element);
// 		if (0 != iRet)
// 		{
// 			sprintf(err_info, "%s InitBattleFieldCfg fail %d ", path.c_str(), iRet);
// 			*err = err_info;
// 			return false;
// 		}
// 	}

	{
		// 战场目标技能配置
		PugiXmlNode root_element = RootElement.child("skill");
		if (root_element.empty())
		{
			*err = path + " xml not skill node ";
			return false;
		}

		iRet = this->InitBattleFieldSkillCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitBattleFieldSkillCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

const PersonalGoalChapterCfg * RoleGoalConfig::GetPersonalGoalChapterCfg(int chapter) const
{
	if (chapter <= 0 || chapter >= PERSONAL_GOAL_CHAPTER_MAX || m_personal_goal_chapter_cfg[chapter].chapter <= 0)
	{
		return NULL;
	}

	if (m_personal_goal_chapter_cfg[chapter].chapter <= 0)
	{
		return NULL;
	}

	return &m_personal_goal_chapter_cfg[chapter];
}

int RoleGoalConfig::GetChapterCfgByPersonalGoal(int cond_type)
{
	for (int i = 0; i < PERSONAL_GOAL_CHAPTER_MAX + 1; i++)
	{
		for (int j = 0; j < PERSONAL_GOAL_COND_MAX; j++)
		{
			if (m_personal_goal_chapter_cfg[i].cond_list[j].cond_type == cond_type)
			{
				return i;
			}
		}
	}
	return -1;
}

const BattleFieldGoalCfg * RoleGoalConfig::GetBattleFieldGoalCfg(int field_type)
{
	if (field_type <= ACTIVITY_TYPE_INVALID || field_type >= ACTIVITY_TYPE_MAX)
	{
		return NULL;
	}

	if (CrossConfig::Instance().IsHiddenServer())
	{
		return NULL;
	}

	std::map<int, BattleFieldGoalCfg>::iterator iter = m_field_type_goal_cfg_map.find(field_type);
	if (iter == m_field_type_goal_cfg_map.end())
	{
		return NULL;
	}

	if (iter->second.field_type != field_type || iter->second.field_type == 0)
	{
		return NULL;
	}

	int open_day_index = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time());
	if (open_day_index >= iter->second.open_server_day)
	{
		return NULL;
	}

	return &iter->second;
}

BattleFieldGoalSkillCfg::ConfigItem *RoleGoalConfig::GetBattleFieldGoalSkillCfg(int skill_type, int skill_level)
{
	if (skill_type <= 0 || skill_type >= FIELD_GOAL_MAX_SKILL_TYPE
		|| skill_level < 0 || skill_level > BATTLE_FIELD_GOAL_SKILL_LEVEL_MAX
		|| skill_type != m_skill_cfg.cfg_list[skill_type][skill_level].skill_type)
	{
		return NULL;
	}

	return &m_skill_cfg.cfg_list[skill_type][skill_level];
}

int RoleGoalConfig::InitPersonalGoalChapterCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int chapter = -1;
		if (!PugiGetSubNodeValue(dataElement, "chapter", chapter) || chapter <= 0 || chapter > PERSONAL_GOAL_CHAPTER_MAX)
		{
			return -1;
		}

		PersonalGoalChapterCfg &chapter_cfg = m_personal_goal_chapter_cfg[chapter];
		chapter_cfg.chapter = chapter;

		char temp_str[32] = {0};
		for (int i = 0; i < PERSONAL_GOAL_COND_MAX; i++)
		{
			sprintf(temp_str, "cond_type%d", i + 1);
			if (!PugiGetSubNodeValue(dataElement, temp_str, chapter_cfg.cond_list[i].cond_type) || chapter_cfg.cond_list[i].cond_type < 0)
			{
				return -100 - i;
			}

			sprintf(temp_str, "cond_param%d_1", i + 1);
			if (!PugiGetSubNodeValue(dataElement, temp_str, chapter_cfg.cond_list[i].cond_param1) || chapter_cfg.cond_list[i].cond_param1 < 0)
			{
				return -200 - i;
			}

			sprintf(temp_str, "cond_param%d_2", i + 1);
			if (!PugiGetSubNodeValue(dataElement, temp_str, chapter_cfg.cond_list[i].cond_param2) || chapter_cfg.cond_list[i].cond_param2 < 0)
			{
				return -300 - i;
			}
		}

		{
			PugiXmlNode element = dataElement.child("prof_reward1_list");
			if (element.empty())
			{
				return -100;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("prof_reward1");
			while (!item_element.empty())
			{
				if (i >= MAX_REWARD_LIST_COUNT)
				{
					return -100 - i;
				}

				if (!chapter_cfg.prof_reward_1[i].ReadConfig(item_element))
				{
					if (chapter_cfg.prof_reward_1[i].item_id == 0)
					{
						break;
					}

					return - 150 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}

			element = dataElement.child("prof_reward2_list");
			if (element.empty())
			{
				return -200;
			}

			i = 0;
			item_element = element.child("prof_reward2");
			while (!item_element.empty())
			{
				if (i >= MAX_REWARD_LIST_COUNT)
				{
					return -200 - i;
				}

				if (!chapter_cfg.prof_reward_2[i].ReadConfig(item_element))
				{
					if (chapter_cfg.prof_reward_2[i].item_id == 0)
					{
						break;
					}

					return - 250 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}

			element = dataElement.child("prof_reward3_list");
			if (element.empty())
			{
				return -300;
			}

			i = 0;
			item_element = element.child("prof_reward3");
			while (!item_element.empty())
			{
				if (i >= MAX_REWARD_LIST_COUNT)
				{
					return -300 - i;
				}

				if (!chapter_cfg.prof_reward_3[i].ReadConfig(item_element))
				{
					if (chapter_cfg.prof_reward_3[i].item_id == 0)
					{
						break;
					}
					return - 350 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}

			element = dataElement.child("prof_reward4_list");
			if (element.empty())
			{
				return -400;
			}

			i = 0;
			item_element = element.child("prof_reward4");
			while (!item_element.empty())
			{
				if (i >= MAX_REWARD_LIST_COUNT)
				{
					return -400 - i;
				}

				if (!chapter_cfg.prof_reward_4[i].ReadConfig(item_element))
				{
					if (chapter_cfg.prof_reward_4[i].item_id == 0)
					{
						break;
					}
					return - 450 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int RoleGoalConfig::InitBattleFieldCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int field_type = -1;
		if (!PugiGetSubNodeValue(dataElement, "field_type", field_type) || field_type <= ACTIVITY_TYPE_INVALID || field_type >= ACTIVITY_TYPE_MAX)
		{
			return -2;
		}

		if (m_field_type_goal_cfg_map.find(field_type) != m_field_type_goal_cfg_map.end())
		{
			return -3;
		}

		BattleFieldGoalCfg field_goal_cfg;
		field_goal_cfg.field_type = field_type;

		if (!PugiGetSubNodeValue(dataElement, "open_server_day", field_goal_cfg.open_server_day) || field_goal_cfg.open_server_day <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "complete_score", field_goal_cfg.complete_score) || field_goal_cfg.complete_score <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_has_reward", field_goal_cfg.is_has_reward) || (field_goal_cfg.is_has_reward != 0 && field_goal_cfg.is_has_reward != 1))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "skill_type", field_goal_cfg.skill_type) || field_goal_cfg.skill_type < FIELD_GOAL_INVALID_SKILL_TYPE || field_goal_cfg.skill_type >= FIELD_GOAL_MAX_SKILL_TYPE)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_1", field_goal_cfg.param_1) || field_goal_cfg.param_1 < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_2", field_goal_cfg.param_2) || field_goal_cfg.param_2 < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_3", field_goal_cfg.param_3) || field_goal_cfg.param_3 < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_4", field_goal_cfg.param_4) || field_goal_cfg.param_4 < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "level_limit", field_goal_cfg.level_limit) || field_goal_cfg.level_limit <= 0 || field_goal_cfg.level_limit > MAX_ROLE_LEVEL)
		{
			return -12;
		}

		{
			PugiXmlNode element = dataElement.child("item_reward_list");
			if (element.empty())
			{
				return -100;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("item_reward");
			while (!item_element.empty())
			{
				if (i >= MAX_REWARD_LIST_COUNT)
				{
					return -100 - i;
				}

				if (!field_goal_cfg.item_reward[i].ReadConfig(item_element))
				{
					if (field_goal_cfg.item_reward[i].item_id == 0)
					{
						break;
					}
					return - 150 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}
		
		m_field_type_goal_cfg_map[field_type] = field_goal_cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int RoleGoalConfig::InitBattleFieldSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int skill_type = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_type", skill_type) || skill_type < FIELD_GOAL_INVALID_SKILL_TYPE || skill_type >= FIELD_GOAL_MAX_SKILL_TYPE)
		{
			return -1;
		}

		int skill_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_level", skill_level) || skill_level < 0 || skill_level > BATTLE_FIELD_GOAL_SKILL_LEVEL_MAX)
		{
			return -2;
		}

		BattleFieldGoalSkillCfg::ConfigItem &cfg = m_skill_cfg.cfg_list[skill_type][skill_level];
		cfg.skill_type = skill_type;
		cfg.skill_level = skill_level;

		if (!PugiGetSubNodeValue(dataElement, "level_limit", cfg.level_limit) || cfg.level_limit <= 0 || cfg.level_limit > MAX_ROLE_LEVEL)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_1", cfg.param_1) || cfg.param_1 < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_2", cfg.param_2) || cfg.param_2 < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_3", cfg.param_3) || cfg.param_3 < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_4", cfg.param_4) || cfg.param_4 < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "capability", cfg.capability) || cfg.capability < 0)
		{
			return -8;
		}

		{
			PugiXmlNode itemelement = dataElement.child("uplevel_stuff_prof1");
			if (itemelement.empty() || !cfg.uplevel_stuff_prof1.ReadConfig(itemelement))
			{
				return -9;
			}
		}

		{
			PugiXmlNode itemelement = dataElement.child("uplevel_stuff_prof2");
			if (itemelement.empty() || !cfg.uplevel_stuff_prof2.ReadConfig(itemelement))
			{
				return -10;
			}
		}

		{
			PugiXmlNode itemelement = dataElement.child("uplevel_stuff_prof3");
			if (itemelement.empty() || !cfg.uplevel_stuff_prof3.ReadConfig(itemelement))
			{
				return -11;
			}
		}

		{
			PugiXmlNode itemelement = dataElement.child("uplevel_stuff_prof4");
			if (itemelement.empty() || !cfg.uplevel_stuff_prof4.ReadConfig(itemelement))
			{
				return -12;
			}
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

