#include "crossguildbattleconfig.hpp"
#include "servercommon/configcommon.h"
#include <set>

CrossGuildBattleConfig::CrossGuildBattleConfig() : m_activity_open_time_cfg_list_count(0), m_flag_num(0), m_drop_total_weight(0), m_warning_interval_s(0)
{
	m_warning_percent_list[0] = CROSS_GUILD_BATTLE_FLAG_HP_WARNING_20;
	m_warning_percent_list[1] = CROSS_GUILD_BATTLE_FLAG_HP_WARNING_50;
}

CrossGuildBattleConfig::~CrossGuildBattleConfig()
{

}

CrossGuildBattleConfig & CrossGuildBattleConfig::Instance()
{
	static CrossGuildBattleConfig _instance;
	return _instance;
}

bool CrossGuildBattleConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossGuildBattleConfig", *err);
	
	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// 其他配置
		PugiXmlNode OtherElement = RootElement.child("other");
		if (OtherElement.empty())
		{
			*err = path + ": xml node error in [config->other]";
			return false;
		}

		iRet = this->InitOtherCfg(OtherElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode OtherElement = RootElement.child("activity_open_time");
		if (OtherElement.empty())
		{
			*err = path + ": xml node error in [config->activity_open_time]";
			return false;
		}

		iRet = this->InitActivityOpenTimeCfg(OtherElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitActivityOpenTimeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode OtherElement = RootElement.child("city");
		if (OtherElement.empty())
		{
			*err = path + ": xml node error in [config->city]";
			return false;
		}

		iRet = this->InitSceneCfg(OtherElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitSceneCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode OtherElement = RootElement.child("flag");
		if (OtherElement.empty())
		{
			*err = path + ": xml node error in [config->flag]";
			return false;
		}

		iRet = this->InitFlagCfg(OtherElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitFlagCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode OtherElement = RootElement.child("own_reward");
		if (OtherElement.empty())
		{
			*err = path + ": xml node error in [config->own_reward]";
			return false;
		}

		iRet = this->InitOwnRewardCfg(OtherElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOwnRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode OtherElement = RootElement.child("score_reward");
		if (OtherElement.empty())
		{
			*err = path + ": xml node error in [config->score_reward]";
			return false;
		}

		iRet = this->InitScoreRewardCfg(OtherElement);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitScoreRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode TaskElement = RootElement.child("task_cfg");
		if (TaskElement.empty())
		{
			*err = path + ": xml node error in [config->task_cfg]";
			return false;
		}

		iRet = this->InitTaskCfg(TaskElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitTaskCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode BossElement = RootElement.child("boss_cfg");
		if (BossElement.empty())
		{
			*err = path + ": xml node error in [config->boss_cfg]";
			return false;
		}

		iRet = this->InitBossCfg(BossElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitBossCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode Element = RootElement.child("monster_convert_credit");
		if ( Element.empty())
		{
			*err = path + ": xml node error in [config->monster_convert_credit]";
			return false;
		}

		iRet = this->InitMonsterConvertCreditCfg(Element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitMonsterConvertCreditCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode Element = RootElement.child("drop_num_cfg");
		if ( Element.empty())
		{
			*err = path + ": xml node error in [config->drop_num_cfg]";
			return false;
		}

		iRet = this->InitDropNumWeightCfg(Element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitDropNumWeightCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode Element = RootElement.child("special_img_drop_cfg");
		if (Element.empty())
		{
			*err = path + ": xml node error in [config->special_img_drop_cfg]";
			return false;
		}

		iRet = this->InitSpecialImgCfg(Element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitSpecialImgCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{	// 每日奖励
		PugiXmlNode root_element = RootElement.child("daily_reward");
		if (root_element.empty())
		{
			*err = path + "xml not daily_reward node";
			return false;
		}

		iRet = this->InitDailyRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitDailyRewardCfg fail %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode gather_element = RootElement.child("gather");
		if (gather_element.empty())
		{
			*err = path + "xml not gather node";
			return false;
		}

		iRet = this->InitGatherCfg(gather_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitGatherCfg fail %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode elem = RootElement.child("sos_cfg");
		if (elem.empty())
		{
			sprintf(errinfo, "%s:xml node error in node [config->sos_cfg]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitSosCfg(elem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitSosCfg fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int CrossGuildBattleConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int is_open = 0;
	if (!PugiGetSubNodeValue(dataElement, "is_open", is_open))
	{
		return -1;
	}

	m_other_cfg.is_open = (is_open != 0);

	if (!PugiGetSubNodeValue(dataElement, "relive_time", m_other_cfg.relive_time) || m_other_cfg.relive_time < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "kill_add_score", m_other_cfg.kill_add_score) || m_other_cfg.kill_add_score < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "assist_time", m_other_cfg.assist_time) || m_other_cfg.assist_time < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "assist_add_score", m_other_cfg.assist_add_score) || m_other_cfg.assist_add_score < 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "add_score_interval", m_other_cfg.add_score_interval) || m_other_cfg.add_score_interval <= 0)
	{
		return -6;
	}
	
	if (!PugiGetSubNodeValue(dataElement, "add_score_once", m_other_cfg.add_score_once) || m_other_cfg.add_score_once < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "level_limit", m_other_cfg.level_limit) || m_other_cfg.level_limit < 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement, "openserver_limit", m_other_cfg.openserver_limit) || m_other_cfg.openserver_limit < 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElement, "skip_task_consume", m_other_cfg.skip_task_consume) || m_other_cfg.skip_task_consume <= 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElement, "skip_task_level_limit", m_other_cfg.skip_task_level_limit) || m_other_cfg.skip_task_level_limit < 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(dataElement, "wing_drop_id", m_other_cfg.wing_drop_id) || m_other_cfg.wing_drop_id < 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(dataElement, "wing_drop_rate", m_other_cfg.wing_drop_rate) || m_other_cfg.wing_drop_rate < 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(dataElement, "wing_drop_daily_num_limit", m_other_cfg.wing_drop_daily_num_limit) || m_other_cfg.wing_drop_daily_num_limit < 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(dataElement, "wing_drop_total_num", m_other_cfg.wing_drop_total_num) || m_other_cfg.wing_drop_total_num < 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(dataElement, "warning_interval_s", m_warning_interval_s) || m_warning_interval_s <= 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(dataElement, "extra_drop_times", m_other_cfg.extra_drop_times) || m_other_cfg.extra_drop_times < 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(dataElement, "main_scene_id", m_other_cfg.main_scene_id) || m_other_cfg.main_scene_id <= 0)
	{
		return -18;
	}

	if (!PugiGetSubNodeValue(dataElement, "special_open_time", m_other_cfg.special_open_time) || m_other_cfg.special_open_time <= 0)
	{
		return -19;
	}

	if (!PugiGetSubNodeValue(dataElement, "special_end_time", m_other_cfg.special_end_time) || m_other_cfg.special_end_time <= 0)
	{
		return -20;
	}

	if (!PugiGetSubNodeValue(dataElement, "effective_range", m_other_cfg.effective_range) || m_other_cfg.effective_range <= 0)
	{
		return -21;
	}

	return 0;
}

int CrossGuildBattleConfig::InitActivityOpenTimeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int pre_activity_week_day = 0;
	int pre_activity_end_time = 0;

	while (!dataElement.empty())
	{
		if (m_activity_open_time_cfg_list_count >= CROSS_PASTURE_OPEN_TIME_MAX_COUNT)
		{
			return -1;
		}

		CrossGuildBattleActivityOpenTimeInfo &open_cfg = m_activity_open_time_cfg_list[m_activity_open_time_cfg_list_count];

		if (!PugiGetSubNodeValue(dataElement, "activity_week_day", open_cfg.activity_week_day) || open_cfg.activity_week_day < SUNDAY || open_cfg.activity_week_day > SATURDAY)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "activity_ready_time", open_cfg.activity_ready_time) || open_cfg.activity_ready_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_ready_time))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "activity_start_time", open_cfg.activity_start_time) || open_cfg.activity_start_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_start_time))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "activity_end_time", open_cfg.activity_end_time) || open_cfg.activity_end_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_end_time))
		{
			return -5;
		}

		if (open_cfg.activity_week_day < pre_activity_week_day)
		{
			return -6;
		}
		else if (open_cfg.activity_week_day == pre_activity_week_day)
		{
			if (open_cfg.activity_ready_time <= pre_activity_end_time)
			{
				return -7;
			}
		}

		pre_activity_end_time = open_cfg.activity_end_time;
		pre_activity_week_day = open_cfg.activity_week_day;

		int ready_time_sec = int(open_cfg.activity_ready_time / 100) * 3600 + (open_cfg.activity_ready_time % 100) * 60;
		int start_time_sec = int(open_cfg.activity_start_time / 100) * 3600 + (open_cfg.activity_start_time % 100) * 60;
		int end_time_sec = int(open_cfg.activity_end_time / 100) * 3600 + (open_cfg.activity_end_time % 100) * 60;

		open_cfg.ready_dur_s = start_time_sec - ready_time_sec;
		open_cfg.open_dur_s = end_time_sec - start_time_sec;

		if (open_cfg.ready_dur_s < 0 || open_cfg.open_dur_s <= 0)
		{
			return -9;
		}

		++m_activity_open_time_cfg_list_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossGuildBattleConfig::InitSceneCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int index = 0;
	while (!dataElement.empty())
	{
		if (index >= CROSS_GUILDBATTLE_MAX_SCENE_NUM)
		{
			return -100;
		}

		CrossGuildBattleSceneCfg &scene_cfg = m_scene_cfgs[index ++];

		if (!PugiGetSubNodeValue(dataElement, "scene_id", scene_cfg.scene_id) || scene_cfg.scene_id <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_score", scene_cfg.max_score) || scene_cfg.max_score <= 0)
		{
			return -2;
		}

		char config_str[32] = { 0 };
		for (int i = 0; i < CrossGuildBattleSceneCfg::ENTER_POS_COUNT; ++i)
		{
			SNPRINTF(config_str, sizeof(config_str), "relive_pos_%d_x", i);
			if (!PugiGetSubNodeValue(dataElement, config_str, scene_cfg.enter_pos_list[i].x) || scene_cfg.enter_pos_list[i].x <= 0)
			{
				return -3;
			}

			SNPRINTF(config_str, sizeof(config_str), "relive_pos_%d_y", i);
			if (!PugiGetSubNodeValue(dataElement, config_str, scene_cfg.enter_pos_list[i].y) || scene_cfg.enter_pos_list[i].y <= 0)
			{
				return -3;
			}
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossGuildBattleConfig::InitFlagCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_flag_num = 0;
	while (!dataElement.empty())
	{
		if (m_flag_num >= CrossGuildBattleConfig::MAX_FALG_CFG_NUM)
		{
			return -100;
		}

		CrossGuildBattleFlagCfg &scene_cfg = m_flag_cfgs[m_flag_num ++];

		if (!PugiGetSubNodeValue(dataElement, "scene_id", scene_cfg.scene_id) || scene_cfg.scene_id <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_id", scene_cfg.flag_id) || scene_cfg.flag_id <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_x", scene_cfg.flush_pos.x) || scene_cfg.flush_pos.x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_y", scene_cfg.flush_pos.y) || scene_cfg.flush_pos.y <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_score_interval", scene_cfg.add_score_interval) || scene_cfg.add_score_interval <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_score_once", scene_cfg.add_score_once) || scene_cfg.add_score_once <= 0)
		{
			return -6;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossGuildBattleConfig::InitOwnRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int index = 0;
	while (!dataElement.empty())
	{
		if (index >= CROSS_GUILDBATTLE_MAX_SCENE_NUM)
		{
			return -100;
		}
		
		CrossGuildBattleOwnRewardCfg &reward_cfg = m_own_reward_cfgs[index ++];
		int item_count = 0;
		{
			item_count = ItemConfigData::ReadConfigListNoCheck(dataElement, "server_reward_item", reward_cfg.server_reward_item, MAX_ATTACHMENT_ITEM_NUM);
			if (item_count < 0)
			{
				return -100 - item_count;
			}
		}

		{
			item_count = ItemConfigData::ReadConfigListNoCheck(dataElement, "guild_reward_item", reward_cfg.guild_reward_item, MAX_ATTACHMENT_ITEM_NUM);
			if (item_count < 0)
			{
				return -200 - item_count;
			}
		}

		{
			reward_cfg.guild_boss_reward_count = ItemConfigData::ReadConfigListNoCheck(dataElement, "guild_boss_reward_item", reward_cfg.guild_boss_reward_item, MAX_CROSS_GUILD_BATTLE_OWNREWARD_NUM);
			if (reward_cfg.guild_boss_reward_count < 0)
			{
				return -300 - reward_cfg.guild_boss_reward_count;
			}
		}

		{
			reward_cfg.team_reward_item_count = ItemConfigData::ReadConfigListNoCheck(dataElement, "team_reward_item", reward_cfg.team_reward_item, MAX_CROSS_GUILD_BATTLE_OWNREWARD_NUM);
			if (reward_cfg.team_reward_item_count < 0)
			{
				return -400 - reward_cfg.team_reward_item_count;
			}
		}

		if (!PugiGetSubNodeValue(dataElement, "part_0", reward_cfg.part_0) || reward_cfg.part_0 < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "index_0", reward_cfg.index_0) || reward_cfg.index_0 < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "part_1", reward_cfg.part_1) || reward_cfg.part_1 < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "index_1", reward_cfg.index_1) || reward_cfg.index_1 < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "guild_reward_gongzi", reward_cfg.guild_reward_gongzi) || reward_cfg.guild_reward_gongzi < 0)
		{
			return -5;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossGuildBattleConfig::InitScoreRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int index = 0;
	int last_score = 0;

	while (!dataElement.empty())
	{
		if (index >= CROSS_GUILDBATTLE_MAX_SCENE_NUM)
		{
			return -100;
		}

		CrossGuildBattleScoreRewardCfg reward_cfg;

		if (!PugiGetSubNodeValue(dataElement, "score", reward_cfg.score) || reward_cfg.score <= 0 || reward_cfg.score <= last_score)
		{
			return -1;
		}
		last_score = reward_cfg.score;

		if (!PugiGetSubNodeValue(dataElement, "exp", reward_cfg.exp) || reward_cfg.exp < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "convert_credit", reward_cfg.convert_credit) || reward_cfg.convert_credit < 0)
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(dataElement, "cross_honor", reward_cfg.cross_honor) || reward_cfg.cross_honor < 0)
		{
			return -21;
		}
// 		{
// 			PugiXmlNode item_element = dataElement.child("reward_item");
// 			if (item_element.empty())
// 			{
// 				return -3;
// 			}
// 
// 			if (!reward_cfg.reward_item.ReadConfigNoCheck(item_element))
// 			{
// 				return -4;
// 			}
// 		}
		int item_count = ItemConfigData::ReadConfigListNoCheck(dataElement, "reward_item", reward_cfg.reward_item, MAX_ATTACHMENT_ITEM_NUM);
		if (item_count < 0)
		{
			return -100 - item_count;
		}

		m_score_reward_vec.push_back(reward_cfg);

		dataElement = dataElement.next_sibling();
	}
	return 0;
}

int CrossGuildBattleConfig::InitTaskCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int scene_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "scene_id", scene_id) || scene_id <= 0)
		{
			return -1;
		}

		CrossGuildBattleTaskCfg cfg;
		cfg.scene_id = scene_id;

		if (!PugiGetSubNodeValue(dataElement, "task_index", cfg.task_index) || cfg.task_index < 0 || cfg.task_index >= CROSS_GUILDBATTLE_MAX_SCENE_NUM)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "task_id", cfg.task_id) || cfg.task_id < 0 || cfg.task_id >= CROSS_GUILDBATTLE_MAX_SCENE_NUM)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "task_type", cfg.task_type) || (cfg.task_type != 0 && cfg.task_type != 1))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "task_param", cfg.task_param) || cfg.task_param < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "task_count", cfg.task_count) || cfg.task_count < 0)
		{
			return -7;
		}

// 		if (!PugiGetSubNodeValue(dataElement, "reward_credit", cfg.reward_credit) || cfg.reward_credit < 0)
// 		{
// 			return -8;
// 		}
// 
// 		if (!PugiGetSubNodeValue(dataElement, "extra_credit", cfg.extra_credit) || cfg.extra_credit < 0)
// 		{
// 			return -10;
// 		}

		if (!PugiGetSubNodeValue(dataElement, "cross_honor", cfg.cross_honor) || cfg.cross_honor < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "extra_cross_honor", cfg.extra_cross_honor) || cfg.extra_cross_honor < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "auto_complete_need_gold", cfg.auto_complete_need_gold) || cfg.auto_complete_need_gold <= 0)
		{
			return -13;
		}

// 		if (!PugiGetSubNodeValue(dataElement, "special_time_extra_credit", cfg.special_time_extra_credit) || cfg.special_time_extra_credit < 0)
// 		{
// 			return -14;
// 		}

		int item_count = ItemConfigData::ReadConfigListNoCheck(dataElement, "reward_item", cfg.reward_item, MAX_ATTACHMENT_ITEM_NUM);
		if (item_count < 0)
		{
			return -100 - item_count;
		}

		item_count = ItemConfigData::ReadConfigListNoCheck(dataElement, "extra_item", cfg.extra_item, MAX_ATTACHMENT_ITEM_NUM);
		if (item_count < 0)
		{
			return -200 - item_count;
		}

		item_count = ItemConfigData::ReadConfigListNoCheck(dataElement, "special_time_item", cfg.special_time_item, MAX_ATTACHMENT_ITEM_NUM);
		if(item_count < 0)
		{
			return -300 - item_count;
		}

		if (0 >= cfg.task_param && 0 >= cfg.task_count)
		{
			return -11;
		}

		if (m_task_cfg_map[scene_id].find(cfg.task_id) != m_task_cfg_map[scene_id].end())
		{
			return -12;
		}

		m_task_cfg_map[scene_id][cfg.task_id] = cfg;

		dataElement = dataElement.next_sibling();
	}
	return 0;
}

int CrossGuildBattleConfig::InitBossCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int boss_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "boss_id", boss_id) || boss_id <= 0)
		{
			return -1;
		}

		if (m_boss_map.find(boss_id) != m_boss_map.end())
		{
			return -2;
		}

		CrossGuildBattleBossCfg cfg;
		cfg.boss_id = boss_id;

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "born_x", cfg.born_pos.x) || cfg.born_pos.x < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "born_y", cfg.born_pos.y) || cfg.born_pos.y < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "kill_credit", cfg.kill_credit) || cfg.kill_credit < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "start_refresh_time", cfg.start_refresh_time) || cfg.start_refresh_time < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "end_refresh_time", cfg.end_refresh_time) || cfg.end_refresh_time < 0 || cfg.end_refresh_time <= cfg.start_refresh_time)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "refresh_interval", cfg.refresh_interval) || cfg.refresh_interval <= 0)
		{
			return -9;
		}

// 		if (!PugiGetSubNodeValue(dataElement, "start_refresh_time1", cfg.start_refresh_time1) || cfg.start_refresh_time1 < 0)
// 		{
// 			return -10;
// 		}
// 
// 		if (!PugiGetSubNodeValue(dataElement, "end_refresh_time1", cfg.end_refresh_time1) || cfg.end_refresh_time1 < 0 || cfg.end_refresh_time1 <= cfg.start_refresh_time1)
// 		{
// 			return -11;
// 		}
// 
// 		if (!PugiGetSubNodeValue(dataElement, "refresh_interval1", cfg.refresh_interval1) || cfg.refresh_interval1 <= 0)
// 		{
// 			return -12;
// 		}
// 
// 		if (cfg.start_refresh_time1 < cfg.end_refresh_time)
// 		{
// 			return -13;
// 		}

		m_boss_map[boss_id] = cfg;

		dataElement = dataElement.next_sibling();
	}
	return 0;
}

int CrossGuildBattleConfig::InitMonsterConvertCreditCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		unsigned short monster_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "monster_id", monster_id) || monster_id <= 0)
		{
			return -1;
		}

		CrossGuildBattleMonsterCredit cfg;
		if (!PugiGetSubNodeValue(dataElement, "convert_credit", cfg.credit) || cfg.credit <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "extra_credit", cfg.extra_credit) || cfg.extra_credit <= 0)
		{
			return -3;
		}

		if (!m_monster_convert_credit_cfg_map.insert(std::make_pair(monster_id, cfg)).second)
		{
			return -4;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossGuildBattleConfig::InitSpecialImgCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		unsigned short monster_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "monster_id", monster_id) || monster_id <= 0)
		{
			return -1;
		}

		int wing_drop_rate = 0;
		if (!PugiGetSubNodeValue(dataElement, "wing_drop_rate", wing_drop_rate) || wing_drop_rate <= 0)
		{
			return -2;
		}

		if (!m_special_img_cfg_map.insert(std::make_pair(monster_id, wing_drop_rate)).second)
		{
			return -3;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossGuildBattleConfig::InitDropNumWeightCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		CrossGuildBattleDropNumWeight drop_cfg;
		if (!PugiGetSubNodeValue(dataElement, "drop_times", drop_cfg.drop_times) || drop_cfg.drop_times < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "weight", drop_cfg.weight) || drop_cfg.weight < 0)
		{
			return -2;
		}

		m_drop_total_weight += drop_cfg.weight;
		
		m_drop_weight_cfg_vec.push_back(drop_cfg);

		dataElement = dataElement.next_sibling();
	}

	if (m_drop_total_weight <= 0)
	{
		return -3;
	}

	return 0;
}

int CrossGuildBattleConfig::InitDailyRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int city_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "city_index", city_index) || city_index < 0 || city_index >= CROSS_GUILDBATTLE_MAX_SCENE_NUM)
		{
			return -1;
		}

		int item_count = ItemConfigData::ReadConfigListNoCheck(dataElement, "reward_item", m_daily_reward_item_list[city_index], MAX_ATTACHMENT_ITEM_NUM);
		if (item_count < 0)
		{
			return -100 - item_count;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossGuildBattleConfig::InitGatherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");

	while (!dataElement.empty())
	{
		CrossGuildBattleGather gather;
		if (!PugiGetSubNodeValue(dataElement, "gather_id", gather.gather_id) || gather.gather_id <= 0)
		{
			return -1;
		}
		if (!PugiGetSubNodeValue(dataElement, "boss_id", gather.boss_id) || gather.boss_id < 0)
		{
			return -2;
		}
		if (!PugiGetSubNodeValue(dataElement,"gather_time",gather.gather_time) || gather.gather_time <= 0)
		{
			return -3;
		}
		if (!PugiGetSubNodeValue(dataElement, "disappear_gather_max", gather.disappear_gather_max) || gather.disappear_gather_max <= 0)
		{
			return -4;
		}
		if (!PugiGetSubNodeValue(dataElement, "disappear_s", gather.disappear_s) || gather.disappear_s < 0 )
		{
			return -5;
		}

		PugiXmlNode itemElement = dataElement.child("guild_reward_item");
		if (itemElement.empty() || !gather.guild_reward_item.ReadConfigNoCheck(itemElement))
		{
			return -6;
		}

		m_boss_gather_map[gather.gather_id] = gather;
		dataElement = dataElement.next_sibling();
	}
	return 0;
}

int CrossGuildBattleConfig::InitSosCfg(PugiXmlNode RootElemen)
{
	PugiXmlNode dataElement = RootElemen.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int times = -1;
		if (!PugiGetSubNodeValue(dataElement, "times", times) || times < 0 || times != static_cast<int>(m_sos_cost_cfg_vec.size()))
		{
			return -1;
		}

		int cost = -1;
		if (!PugiGetSubNodeValue(dataElement, "cost", cost) || cost < 0)
		{
			return -2;
		}

		m_sos_cost_cfg_vec.push_back(cost);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

CrossGuildBattleActivityOpenTimeInfo *CrossGuildBattleConfig::GetActivityOpenTimeInfo(int index)
{
	if (index >= 0 && index < m_activity_open_time_cfg_list_count && index < CROSS_PASTURE_OPEN_TIME_MAX_COUNT)
	{
		return &m_activity_open_time_cfg_list[index];
	}

	return NULL;
}

int CrossGuildBattleConfig::GetOpenActivityConfigList(CrossGuildBattleActivityOpenTimeInfo cfg_list[CROSS_PASTURE_OPEN_TIME_MAX_COUNT])
{
	memcpy(cfg_list, m_activity_open_time_cfg_list, sizeof(m_activity_open_time_cfg_list));
	return m_activity_open_time_cfg_list_count;
}

const CrossGuildBattleSceneCfg *CrossGuildBattleConfig::GetBattleSceneCfg(int scene_id)
{
	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++ i)
	{
		if (m_scene_cfgs[i].scene_id == scene_id)
		{
			return &m_scene_cfgs[i];
		}
	}
	
	return NULL;
}

bool CrossGuildBattleConfig::IsCrossGuildBattleScene(int scene_id)
{
	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
	{
		if (m_scene_cfgs[i].scene_id == scene_id)
		{
			return true;
		}
	}

	return false;
}

Posi CrossGuildBattleConfig::GetRandEnterPosBySceneID(int scene_id)
{
	int rand_index = RandomNum(CrossGuildBattleSceneCfg::ENTER_POS_COUNT);

	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
	{
		if (m_scene_cfgs[i].scene_id == scene_id)
		{
			return m_scene_cfgs[i].enter_pos_list[rand_index];
		}
	}

	return Posi(0, 0);
}

Posi CrossGuildBattleConfig::GetStandyEnterPosBySceneID(int scene_id)
{
	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
	{
		if (m_scene_cfgs[i].scene_id == scene_id)
		{
			return m_scene_cfgs[i].enter_pos_list[0];
		}
	}

	return Posi(0, 0);
}

int CrossGuildBattleConfig::GetRandEnterPosByIndex(int index, Posi *pos)
{
	if (nullptr == pos)
	{
		return 0;
	}

	if (index < 0 || index >= CROSS_GUILDBATTLE_MAX_SCENE_NUM)
	{
		return 0;
	}

	int rand_index = RandomNum(CrossGuildBattleSceneCfg::ENTER_POS_COUNT);
	*pos = m_scene_cfgs[index].enter_pos_list[rand_index];

	return m_scene_cfgs[index].scene_id;
}

void CrossGuildBattleConfig::GetGuildBattleSceneIdList(int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM])
{
	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++ i)
	{
		scene_id_list[i] = m_scene_cfgs[i].scene_id;
	}
}

int CrossGuildBattleConfig:: GetCityIndex(const int scene_id)
{
	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
	{
		if(scene_id == m_scene_cfgs[i].scene_id)
		{
			return i;
		}
	}
	return  CROSS_GUILDBATTLE_MAX_SCENE_NUM;
}

void CrossGuildBattleConfig::GetFlagIdList(int scene_id, int flag_id_list[CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE])
{
	int num = 0;
	for (int i = 0; i < m_flag_num && i < CrossGuildBattleConfig::MAX_FALG_CFG_NUM; ++ i)
	{
		if (m_flag_cfgs[i].scene_id == scene_id && num < CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE)
		{
			flag_id_list[num ++] = m_flag_cfgs[i].flag_id;
		}
	}
}

const CrossGuildBattleFlagCfg *CrossGuildBattleConfig::GetFlagCfg(int scene_id, int flag_id)
{
	for (int i = 0; i < m_flag_num && i < CrossGuildBattleConfig::MAX_FALG_CFG_NUM; ++ i)
	{
		if (m_flag_cfgs[i].scene_id == scene_id && m_flag_cfgs[i].flag_id == flag_id)
		{
			return &m_flag_cfgs[i];
		}
	}

	return NULL;
}

bool CrossGuildBattleConfig::IsFlag(int flag_id)
{
	for (int i = 0; i < m_flag_num && i < CrossGuildBattleConfig::MAX_FALG_CFG_NUM; ++i)
	{
		if (m_flag_cfgs[i].flag_id == flag_id)
		{
			return true;
		}
	}

	return false;
}

const CrossGuildBattleScoreRewardCfg *CrossGuildBattleConfig::GetScoreRewardCfg(int old_score, int new_score)
{
	if (old_score >= new_score)
	{
		return NULL;
	}

	ScoreRewardVec::iterator iter = m_score_reward_vec.begin();
	for (; m_score_reward_vec.end() != iter; ++ iter)
	{
		if (old_score < iter->score && new_score >= iter->score)
		{
			return &(*iter);
		}
	}

	return NULL;
}

const CrossGuildBattleOwnRewardCfg *CrossGuildBattleConfig::GetOwnServerRewardCfg(int index)
{
	if (index < 0 || index >= CROSS_GUILDBATTLE_MAX_SCENE_NUM)
	{
		return NULL;
	}

	return &m_own_reward_cfgs[index];
}

bool CrossGuildBattleConfig::DayTimeFormatInvalid(int day_time)
{
	int hour = day_time / 100;
	int minute = day_time % 100;

	return (hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}

bool CrossGuildBattleConfig::IsZhuMoTime(unsigned int second)
{
	unsigned int zero_time = static_cast<unsigned int>(GetZeroTime(second));
	int today_sec = second - zero_time;
	for (auto itr = m_boss_map.begin(); itr != m_boss_map.end(); ++itr)
	{
		if (today_sec >= itr->second.start_refresh_time && today_sec <= itr->second.end_refresh_time)
		{
			return true;
		}
	}

	return false;
}

const CrossGuildBattleTaskCfg *CrossGuildBattleConfig::GetGuildBattleTaskCfg(int scend_id, int task_id)
{
	if (m_task_cfg_map.find(scend_id) == m_task_cfg_map.end()) return NULL;

	if (m_task_cfg_map[scend_id].find(task_id) == m_task_cfg_map[scend_id].end()) return NULL;

	return &m_task_cfg_map[scend_id][task_id];
}

const CrossGuildBattleBossCfg *CrossGuildBattleConfig::GetGuildBattleBossCfg(int boss_id)
{
	if (m_boss_map.find(boss_id) == m_boss_map.end()) return NULL;

	return &m_boss_map[boss_id];
}

const CrossGuildBattleMonsterCredit * CrossGuildBattleConfig::GetMonsterConvertCreditCfg(unsigned short monster_id)
{
	auto iter = m_monster_convert_credit_cfg_map.find(monster_id);
	if (iter != m_monster_convert_credit_cfg_map.end())
	{
		return &(iter->second);
	}

	return nullptr;
}

int CrossGuildBattleConfig::GetSpecialImgDropRate(unsigned short monster_id)
{
	auto iter = m_special_img_cfg_map.find(monster_id);
	if (iter != m_special_img_cfg_map.end())
	{
		return iter->second;
	}

	return 0;
}

int CrossGuildBattleConfig::GetRandDropWingNum()
{
	if (m_drop_total_weight <= 0)
	{
		return 1;
	}

	int rand_weight = RandomNum(0, m_drop_total_weight);
	for (const auto &cfg : m_drop_weight_cfg_vec)
	{
		if (rand_weight < cfg.weight)
		{
			return cfg.drop_times;
		}

		rand_weight -= cfg.weight;
	}

	return 1;
}

bool CrossGuildBattleConfig::CheckWarning(int old_percent, int now_percent, int *warning_percent)
{
	for (int i = 0; i < WARNING_HP_LIST_LEN; ++i)
	{
		if (old_percent >= m_warning_percent_list[i] && now_percent < m_warning_percent_list[i])
		{
			if (nullptr != warning_percent) *warning_percent = m_warning_percent_list[i];
			return true;
		}
	}

	return false;
}

const ItemConfigData * CrossGuildBattleConfig::GetDailyDailyRewardItem(int index) const
{
	if (index < 0 || index >= CROSS_GUILDBATTLE_MAX_SCENE_NUM)
	{
		return nullptr;
	}

	return m_daily_reward_item_list[index];
}

const CrossGuildBattleGather* CrossGuildBattleConfig::GetGatherCfgByBossId(int boss_id) const
{
	for (auto it = m_boss_gather_map.begin(); it != m_boss_gather_map.end(); ++it)
	{
		if (it->second.boss_id == boss_id)
		{
			return &it->second;
		}
	}
	return nullptr;
}
const CrossGuildBattleGather* CrossGuildBattleConfig::GetGatherCfgById(int gather_id) const
{
	auto it = m_boss_gather_map.find(gather_id);
	if (it != m_boss_gather_map.end())
	{
		return &it->second;
	}
	return nullptr;
}

int CrossGuildBattleConfig::GetSosCost(int times) const
{
	if (times < 0 || times >= static_cast<int>(m_sos_cost_cfg_vec.size()))
	{
		return -1;
	}

	return m_sos_cost_cfg_vec[times];
}