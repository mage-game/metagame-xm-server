#include "roleglobalconfig.h"
#include "servercommon/configcommon.h"

#include "skill/skillpool.hpp"

namespace globalconfig
{
	RoleGlobalConfig::RoleGlobalConfig()
		: m_cooldown(0), m_xinshuo_protected_level(0), m_upgrade_log_level(0), m_peace_mode_interval(0), m_realive_interval(0),
		m_in_fight_ms(0), m_in_team_distance(0, 0), m_recover_hp_interval(0), m_recover_hp_per(0), m_recover_hp_fight_per(0), m_fly_increase_speed_percent(0), m_forbid_neiwan_action(false)
	{
	}

	RoleGlobalConfig::~RoleGlobalConfig()
	{

	}

	bool RoleGlobalConfig::Init(const std::string &configname, std::string *err)
	{
		char errinfo[1024] = {0}; 

		TiXmlDocument document;
		if (configname == "" || !document.LoadFile(configname))
		{
			*err = configname + " Load fail: " +document.ErrorDesc();
			return false;
		}

		TiXmlElement *RootElement = document.RootElement();
		if (NULL == RootElement)
		{
			*err = configname + ": xml node error in root";
			return false;
		}

		TiXmlElement *CurrElement = NULL;

		if (!GetSubNodeValue(RootElement, "cooldown", m_cooldown) || m_cooldown <= 0)
		{
			*err = configname + ": xml node error in [config->cooldown]";
			return false;
		}

		if (!GetSubNodeValue(RootElement, "recover_hp_interval", m_recover_hp_interval) || m_recover_hp_interval < 0)
		{
			sprintf(errinfo,"%s: xml node error in  [config->recover_hp_interval] .", configname.c_str());
			*err = errinfo;
			return false;
		}

		if (!GetSubNodeValue(RootElement, "recover_hp_per", m_recover_hp_per) || m_recover_hp_per < 0)
		{
			sprintf(errinfo,"%s: xml node error in  [config->recover_hp_per] .", configname.c_str());
			*err = errinfo;
			return false;
		}

		if (!GetSubNodeValue(RootElement, "recover_hp_fight_per", m_recover_hp_fight_per) || m_recover_hp_fight_per < 0)
		{
			sprintf(errinfo,"%s: xml node error in  [config->recover_hp_fight_per] .", configname.c_str());
			*err = errinfo;
			return false;
		}

		if (!GetSubNodeValue(RootElement, "trigger_continue_kill_interval_max", m_trigger_continue_kill_interval_max) || m_trigger_continue_kill_interval_max < 0)
		{
			sprintf(errinfo, "%s: xml node error in  [config->trigger_continue_kill_interval_max] .", configname.c_str());
			*err = errinfo;
			return false;
		}

		CurrElement = RootElement->FirstChildElement("in_team_distance");
		if (!CurrElement)
		{
			sprintf(errinfo,"%s: xml node [config->in_team_distance] is not exsit.", configname.c_str());
			*err = errinfo;
			return false;
		}
		if (!GetSubNodeValue(CurrElement, "x", m_in_team_distance.x) || m_in_team_distance.x <= 0)
		{
			sprintf(errinfo,"%s: xml node error in  [config->in_team_distance->x] .", configname.c_str());
			*err = errinfo;
			return false;
		}
		if (!GetSubNodeValue(CurrElement, "y", m_in_team_distance.y) || m_in_team_distance.y <= 0)
		{
			sprintf(errinfo,"%s: xml node error in  [config->in_team_distance->y] .", configname.c_str());
			*err = errinfo;
			return false;
		}

		if (!GetSubNodeValue(RootElement, "xinshuo_protected_level", m_xinshuo_protected_level) || m_xinshuo_protected_level <= 0)
		{
			*err = configname + ": xml node error in [config->xinshuo_protected_level]";
			return false;
		}

		if (!GetSubNodeValue(RootElement, "upgrade_log_level", m_upgrade_log_level) || m_upgrade_log_level <= 0)
		{
			*err = configname + ": xml node error in [config->upgrade_log_level]";
			return false;
		}

		if (!GetSubNodeValue(RootElement, "peace_mode_interval", m_peace_mode_interval) || m_peace_mode_interval <= 0)
		{
			*err = configname + ": xml node error in [config->peace_mode_interval]";
			return false;
		}

		if (!GetSubNodeValue(RootElement, "realive_interval", m_realive_interval) || m_realive_interval <= 0)
		{
			*err = configname + ": xml node error in [config->realive_interval]";
			return false;
		}

		if (!GetSubNodeValue(RootElement, "in_fight_ms", m_in_fight_ms) || m_in_fight_ms <= 0)
		{
			*err = configname + ": xml node error in [config->in_fight_ms]";
			return false;
		}

		TiXmlElement *profcfglistElement = RootElement->FirstChildElement("profcfg_list");
		if (!profcfglistElement)
		{
			sprintf(errinfo,"%s: xml node [config->profcfg_list] is not exsit.", configname.c_str());
			*err = errinfo;
			return false;
		}

		// 职业配置，支持创建角色时可以学习多个默认技能
		TiXmlElement *profcfgElement = profcfglistElement->FirstChildElement("profcfg");
		while (NULL != profcfgElement)
		{
			int prof = 0;
			if (!GetSubNodeValue(profcfgElement, "prof", prof) || prof < PROF_TYPE_INVALID || prof >= PROF_TYPE_MAX)
			{
				sprintf(errinfo,"%s: xml node error in [config->profcfg_list->profcfg->prof].", configname.c_str());
				*err = errinfo;
				return false;
			}
			if (0 != m_prof_cfg_list[prof].default_skill_id_count)
			{
				sprintf(errinfo,"%s: xml node error in [config->profcfg_list->profcfg->prof default_skill_id repeat].", configname.c_str());
				*err = errinfo;
				return false;
			}

			ProfCfg *prof_cfg = &m_prof_cfg_list[prof];
			prof_cfg->default_skill_id_count = 0;

			TiXmlElement *defskillElement = profcfgElement->FirstChildElement("default_skill_id");
			while (NULL != defskillElement)
			{
				if (prof_cfg->default_skill_id_count >= MAX_DEFAULT_SKILL_COUNT)
				{
					sprintf(errinfo,"%s: xml node error in [config->profcfg_list->profcfg->prof default_skill_id error 1].", configname.c_str());
					*err = errinfo;
					return false;
				}

				int skill_id = 0;
				if (!GetNodeValue(defskillElement, skill_id))
				{
					sprintf(errinfo,"%s: xml node error in [config->profcfg_list->profcfg->prof default_skill_id error 2].", configname.c_str());
					*err = errinfo;
					return false;
				}

				if (skill_id > 0 && NULL == SKILLPOOL->GetSkill(skill_id))
				{
					sprintf(errinfo,"%s: xml node error in [config->profcfg_list->profcfg->prof skill_id:%d not exist].", configname.c_str(), skill_id);
					*err = errinfo;
					return false;
				}

				prof_cfg->default_skill_id_list[prof_cfg->default_skill_id_count] = skill_id;
				prof_cfg->default_skill_id_count++;

				defskillElement = defskillElement->NextSiblingElement();
			}
			
			profcfgElement = profcfgElement->NextSiblingElement();
		}

		for (int i = PROF_TYPE_PROF_1; i <= PROF_TYPE_PROF_4; i++)
		{
			if (0 == m_prof_cfg_list[i].default_skill_id_count)
			{
				sprintf(errinfo,"%s: xml node error in [config->profcfg_list->profcfg->prof:%d default_skill_id is 0].", configname.c_str(), i);
				*err = errinfo;
				return false;
			}
		}


		{
			if (!GetSubNodeValue(RootElement, "fly_increase_speed_percent", m_fly_increase_speed_percent) || m_fly_increase_speed_percent <= 0)
			{
				*err = configname + ": xml node error in [config->fly_increase_speed_percent]";
				return false;
			}
		}


		{
			int forbid_neiwan_action = 0;
			if (!GetSubNodeValue(RootElement, "forbid_neiwan_action", forbid_neiwan_action) || forbid_neiwan_action < 0 || forbid_neiwan_action > 1)
			{
				*err = configname + ": xml node error in [config->forbid_neiwan_action]";
				return false;
			}

			m_forbid_neiwan_action = (0 != forbid_neiwan_action);
		}

		TiXmlElement *forbid_neiwan_accountElement = RootElement->FirstChildElement("forbid_neiwan_account");
		if (!forbid_neiwan_accountElement)
		{
			sprintf(errinfo,"%s: xml node [config->forbid_neiwan_account] is not exsit.", configname.c_str());
			*err = errinfo;
			return false;
		}

		int is_forbid = 0;
		if (!GetSubNodeValue(forbid_neiwan_accountElement, "is_forbid", is_forbid) || is_forbid < 0 || is_forbid > 1)
		{
			*err = configname + ": xml node error in [config->forbid_neiwan_account->is_forbid]";
			return false;
		}
		m_forbid_neiwan_account.is_forbid = (0 != is_forbid);

		int no_login_day = 0;
		if (!GetSubNodeValue(forbid_neiwan_accountElement, "no_login_day", no_login_day) || no_login_day < 0)
		{
			*err = configname + ": xml node error in [config->forbid_neiwan_account->no_login_day]";
			return false;
		}
		m_forbid_neiwan_account.no_login_sec = (unsigned int)(no_login_day * SECOND_PER_DAY);

		if (!GetSubNodeValue(forbid_neiwan_accountElement, "chongzhi_max", m_forbid_neiwan_account.chongzhi_max) || m_forbid_neiwan_account.chongzhi_max < 0)
		{
			*err = configname + ": xml node error in [config->forbid_neiwan_account->chongzhi_max]";
			return false;
		}

		{
			TiXmlElement *forbid_channelElement = RootElement->FirstChildElement("forbid_channel");
			if (!forbid_channelElement)
			{
				sprintf(errinfo,"%s: xml node [config->forbid_channel] is not exsit.", configname.c_str());
				*err = errinfo;
				return false;
			}

			if (!GetSubNodeValue(forbid_channelElement, "role_level_limit", m_forbid_channel.role_level_limit) || m_forbid_channel.role_level_limit <= 0)
			{
				*err = configname + ": xml node error in [config->forbid_channel->role_level_limit]";
				return false;
			}

			if (!GetSubNodeValue(forbid_channelElement, "max_world_chat_count1", m_forbid_channel.max_world_chat_count1) || m_forbid_channel.max_world_chat_count1 < 0)
			{
				*err = configname + ": xml node error in [config->forbid_channel->max_world_chat_count1]";
				return false;
			}

			if (!GetSubNodeValue(forbid_channelElement, "max_world_chat_count2", m_forbid_channel.max_world_chat_count2) || m_forbid_channel.max_world_chat_count2 < 0)
			{
				*err = configname + ": xml node error in [config->forbid_channel->max_world_chat_count2]";
				return false;
			}
		}


		{
			// 连斩配置
			TiXmlElement *continue_kill_element = RootElement->FirstChildElement("continue_kill");
			if (NULL == continue_kill_element)
			{
				*err = configname + ": no [continue_kill].";
				return false;
			}

			int iRet = this->InitContinueKillCfg(continue_kill_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitContinueKillCfg failed %d", configname.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}
		
		return true;
	}

	int RoleGlobalConfig::InitContinueKillCfg(TiXmlElement *RootElement)
	{
		TiXmlElement *dataElement = RootElement->FirstChildElement("data");
		if (NULL == dataElement)
		{
			return -10000;
		}

		m_continue_kill_cfg_map.clear();

		while (NULL != dataElement)
		{
			int kill_count = 0;
			if (!GetSubNodeValue(dataElement, "kill_count", kill_count) || kill_count <= 0)
			{
				return -1;
			}

			if (m_continue_kill_cfg_map.find(kill_count) != m_continue_kill_cfg_map.end())
			{
				return -2;
			}

			RoleContinueKillCfg &cfg = m_continue_kill_cfg_map[kill_count];
			cfg.kill_count = kill_count;

			if (!GetSubNodeValue(dataElement, "buff_time", cfg.buff_time) || cfg.buff_time <= 0)
			{
				return -3;
			}

			if (!GetSubNodeValue(dataElement, "gongji_add_per", cfg.gongji_add_per) || cfg.gongji_add_per <= 0)
			{
				return -4;
			}

			dataElement = dataElement->NextSiblingElement();
		}

		return 0;
	}

	int RoleGlobalConfig::GetDefaultSkillID(int prof, int default_skill_id_list[MAX_DEFAULT_SKILL_COUNT])
	{
		if (prof <= PROF_TYPE_INVALID || prof >= PROF_TYPE_MAX) return 0;
		for (int i = 0; i < MAX_DEFAULT_SKILL_COUNT; i++)
		{
			default_skill_id_list[i] =  m_prof_cfg_list[prof].default_skill_id_list[i];
		}
		return m_prof_cfg_list[prof].default_skill_id_count;
	}

	int RoleGlobalConfig::GetFlyIncreaseSpeedPercent()
	{
		return m_fly_increase_speed_percent;
	}

	bool RoleGlobalConfig::GetIsForbidNeiwanAction()
	{
		return m_forbid_neiwan_action;
	}

	RoleGlobalConfig::ProfCfg * RoleGlobalConfig::GetProfCfg(int prof)
	{
		if (prof < PROF_TYPE_INVALID || prof >= PROF_TYPE_MAX) return NULL;
		return &m_prof_cfg_list[prof];
	}

	RoleContinueKillCfg * RoleGlobalConfig::GetContinueKillCfg(int kill_count)
	{
		if (kill_count < 0) return NULL;

		std::map<int, RoleContinueKillCfg>::reverse_iterator iter = m_continue_kill_cfg_map.rbegin();
		for (; iter != m_continue_kill_cfg_map.rend(); ++iter)
		{
			if (kill_count >= iter->second.kill_count)
			{
				return &iter->second;
			}
		}

		return NULL;
	}


}

