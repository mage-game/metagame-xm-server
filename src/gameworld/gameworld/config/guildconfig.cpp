#include "guildconfig.hpp"

#include "item/itempool.h"
#include "monster/monsterpool.h"

#include "checkresourcecenter.hpp"

#include "item/itembase.h"
#include <stdlib.h>
#include "servercommon/maildef.hpp"

GuildConfig::GuildConfig() : m_guild_box_total_rate(0), m_guild_extend_member_cfg_count(0), 
m_biaoche_sceneid(0), m_pos_count(0), 
m_biaoche_report_pos_interval(0), m_husong_exp_interval(0), m_biaoche_be_hurt_report_interval(0), m_husong_exp_range(0), m_guild_rob_count_max(0),
m_member_rob_count_max(0), m_biaoche_monsterid(0)
{
	memset(m_post_relive_times, 0, sizeof(m_post_relive_times));
	memset(m_goal_percent, 0, sizeof(m_goal_percent));
}

GuildConfig::~GuildConfig()
{

}

bool GuildConfig::Init(const std::string &configname, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "GuildConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		sprintf(errinfo,"Load Guild config [%s] Error.cannot find root node.", configname.c_str());
		*err = errinfo;
		return false;
	}

	{
		PugiXmlNode otherElement = RootElement.child("other_config");
		if (otherElement.empty())
		{
			*err = configname + ": no xml node [config->other_config]";
			return false;
		}

		iRet = this->InitOtherCfg(otherElement);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode otherElement = RootElement.child("juanxian_config");
		if (otherElement.empty())
		{
			*err = configname + ": no xml node [config->juanxian_config]";
			return false;
		}

		iRet = this->InitJuanXianCfg(otherElement);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitJuanXianCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode hallElement = RootElement.child("level_config");
		if (hallElement.empty())
		{
			*err = configname + ": no xml node [config->level_config]";
			return false;
		}

		iRet = this->InitLevelCfg(hallElement);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode otherElement = RootElement.child("exchange_config");
		if (otherElement.empty())
		{
			*err = configname + ": no xml node [config->exchange_config]";
			return false;
		}

		iRet = this->InitExchangeCfg(otherElement);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitExchangeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode luckyElement = RootElement.child("member_lucky");
		if (luckyElement.empty())
		{
			*err = configname + ": no xml node [config->member_lucky]";
			return false;
		}

		iRet = this->InitMemberLuckyCfg(luckyElement);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitMemberLuckyCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode storgeElement = RootElement.child("storge");
		if (storgeElement.empty())
		{
			*err = configname + ": no xml node [config->storge]";
			return false;
		}

		iRet = this->InitStorgeCfg(storgeElement);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitStorgeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("skill_config");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->skill_config]";
			return false;
		}

		iRet = this->InitSkillCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitSkillCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("totem_config");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->totem_config]";
			return false;
		}

		iRet = this->InitTotemCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitTutemCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("box_config");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->box_config]";
			return false;
		}

		iRet = this->InitBoxCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitBoxCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("territory_welf_config");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->territory_welf_config]";
			return false;
		}

		iRet = this->InitTerrityoryWelfCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitTerrityoryWelfCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("extend_member_cfg");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->extend_member_cfg]";
			return false;
		}

		iRet = this->InitExtendMemberCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitExtendMemberCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("post_relive_times");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->post_relive_times]";
			return false;
		}

		iRet = this->InitPostReliveTimesCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitPostReliveTimesCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("boss_guild_redbag_cfg");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->boss_guild_redbag_cfg]";
			return false;
		}

		iRet = this->InitBossGuildRedbagCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitBossGuildRedbagCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("red_pocket");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->red_pocket]";
			return false;
		}

		iRet = this->InitGuildRedPaperCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo,"%s : InitGuildRedPaperCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}


	{
		// 迷宫
		PugiXmlNode element = RootElement.child("maze");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->maze]";
			return false;
		}

		iRet = this->InitGuildMazeCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitGuildMazeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 迷宫排行奖励
		PugiXmlNode element = RootElement.child("maze_rank_reward");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->maze_rank_reward]";
			return false;
		}

		iRet = this->InitGuildMazeRankRewardCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitGuildMazeRankRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 公会骰子
		PugiXmlNode element = RootElement.child("saizi");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->saizi]";
			return false;
		}

		iRet = this->InitGuildSaiziCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitGuildSaiziCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("plat_menber_count_cfg");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->plat_menber_count_cfg]";
			return false;
		}

		iRet = this->InitGuilInitMemberCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitGuilInitMemberCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("plat_name_cfg");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->plat_name_cfg]";
			return false;
		}

		iRet = this->InitPlatNameCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitPlatNameCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 仙盟镖车---------------------------------------------------

	{
		PugiXmlNode element = RootElement.child("guild_yunbiao_other_config");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->guild_yunbiao_other_config]";
			return false;
		}

		iRet = this->InitYunbiaoOtherCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitYunbiaoOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("guild_biaoche_info");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->guild_biaoche_info]";
			return false;
		}

		iRet = this->InitYunbiaoInfoCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitYunbiaoInfoCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("guild_biaoche_add_exp");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->plat_menber_count_cfg]";
			return false;
		}

		iRet = this->InitYunbiaoExpCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitYunbiaoExpCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("guild_biaoche_attr");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->plat_menber_count_cfg]";
			return false;
		}

		iRet = this->InitYunbiaoAttrCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitYunbiaoAttrCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("guild_biaoche_pos_list");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->guild_biaoche_pos_list]";
			return false;
		}

		iRet = this->InitYunbiaoPathCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitYunbiaoPathCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("jianghu_rumors");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->jianghu_rumors]";
			return false;
		}

		iRet = this->InitGuildRumorCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitGuildRumorCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 仙盟工资----------------------------------------------
	{
		PugiXmlNode element = RootElement.child("gongzi_rate");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->gongzi_rate]";
			return false;
		}

		iRet = this->InitGuilGongziCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitGuilGongziCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("gongzi_give_type");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->gongzi_give_type]";
			return false;
		}

		iRet = this->InitGuildGongziGiveCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitGuildGongziGiveCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode element = RootElement.child("rare_item_id");
		if (element.empty())
		{
			*err = configname + ": no xml node [config->rare_item_id]";
			return false;
		}

		iRet = this->InitRareItemCfg(element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitRareItemCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int GuildConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	if (!PugiGetSubNodeValue(dataElement, "create_coin_bind", m_other_cfg.guild_create_coin) || m_other_cfg.guild_create_coin < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "create_item_id", m_other_cfg.guild_create_item_id) || 0 == m_other_cfg.guild_create_item_id || 
		NULL == ITEMPOOL->GetItem(m_other_cfg.guild_create_item_id))
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "jianshe_item_id", m_other_cfg.guild_jianshe_item_id) || 0 == m_other_cfg.guild_jianshe_item_id || 
		NULL == ITEMPOOL->GetItem(m_other_cfg.guild_jianshe_item_id))
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "delate_item_id", m_other_cfg.guild_delate_item_id) || 0 == m_other_cfg.guild_delate_item_id || 
		NULL == ITEMPOOL->GetItem(m_other_cfg.guild_delate_item_id))
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "station_scene_id", m_other_cfg.guild_station_scene_id) || m_other_cfg.guild_station_scene_id <= 0)
	{
		return -5;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.guild_station_scene_id);

	if (!PugiGetSubNodeValue(dataElement, "back_station_posx", m_other_cfg.guild_back_station_pos.x) || m_other_cfg.guild_back_station_pos.x <= 0)
	{
		return -6;
	}
	if (!PugiGetSubNodeValue(dataElement, "back_station_posy", m_other_cfg.guild_back_station_pos.y) ||  m_other_cfg.guild_back_station_pos.y <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "guild_lucky_zhufu_times", m_other_cfg.guild_lucky_zhufu_times) || m_other_cfg.guild_lucky_zhufu_times <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement, "guild_init_exp_1", m_other_cfg.guild_init_exp_1) || m_other_cfg.guild_init_exp_1 < 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElement, "creator_get_gongxian1", m_other_cfg.creator_get_gongxian1) || m_other_cfg.creator_get_gongxian1 < 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElement, "guild_init_exp_2", m_other_cfg.guild_init_exp_2) || m_other_cfg.guild_init_exp_2 < 0 || m_other_cfg.guild_init_exp_2 < m_other_cfg.guild_init_exp_1)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(dataElement, "creator_get_gongxian2", m_other_cfg.creator_get_gongxian2) || m_other_cfg.creator_get_gongxian2 < 0 || m_other_cfg.creator_get_gongxian2 < m_other_cfg.creator_get_gongxian1)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(dataElement, "box_free_up_count", m_other_cfg.box_free_up_count) || m_other_cfg.box_free_up_count < 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(dataElement, "box_up_gold", m_other_cfg.box_up_gold) || m_other_cfg.box_up_gold < 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(dataElement, "box_start_time", m_other_cfg.box_start_time) || m_other_cfg.box_start_time < 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(dataElement, "box_assist_max_count", m_other_cfg.box_assist_max_count) || m_other_cfg.box_assist_max_count < 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(dataElement, "box_assist_cd", m_other_cfg.box_assist_cd) || m_other_cfg.box_assist_cd < 0)
	{
		return -19;
	}

	if (!PugiGetSubNodeValue(dataElement, "box_assist_cd_limit", m_other_cfg.box_assist_cd_limit) || m_other_cfg.box_assist_cd_limit < 0)
	{
		return -20;
	}

	if (!PugiGetSubNodeValue(dataElement, "box_assist_cd_gold", m_other_cfg.box_assist_cd_gold) || m_other_cfg.box_assist_cd_gold < 0)
	{
		return -21;
	}

	if (!PugiGetSubNodeValue(dataElement, "box_limit_time", m_other_cfg.box_limit_time) || m_other_cfg.box_limit_time < 0)
	{
		return -22;
	}

	if (!PugiGetSubNodeValue(dataElement, "totem_bless_interval", m_other_cfg.totem_bless_interval) || m_other_cfg.totem_bless_interval < 0)
	{
		return -23;
	}

	if (!PugiGetSubNodeValue(dataElement, "finish_task_totem_exp_reward", m_other_cfg.finish_task_totem_exp_reward) || m_other_cfg.finish_task_totem_exp_reward < 0)
	{
		return -24;
	}

	if (!PugiGetSubNodeValue(dataElement, "join_guild_level_limit", m_other_cfg.join_guild_level_limit) || m_other_cfg.join_guild_level_limit <= 0)
	{
		return -25;
	}

	if (!PugiGetSubNodeValue(dataElement, "guild_call_in_level_limit", m_other_cfg.guild_call_in_level_limit) || m_other_cfg.guild_call_in_level_limit <= 0)
	{
		return -26;
	}

	if (!PugiGetSubNodeValue(dataElement, "guild_call_in_free_time", m_other_cfg.guild_call_in_free_time) || m_other_cfg.guild_call_in_free_time < 0)
	{
		return -27;
	}

	if (!PugiGetSubNodeValue(dataElement, "guild_call_in_cost_gold", m_other_cfg.guild_call_in_cost_gold) || m_other_cfg.guild_call_in_cost_gold < 0)
	{
		return -28;
	}

	if (!PugiGetSubNodeValue(dataElement, "atuo_join_guild_cd", m_other_cfg.atuo_join_guild_cd) || m_other_cfg.atuo_join_guild_cd < 0)
	{
		return -29;
	}

	if (!PugiGetSubNodeValue(dataElement, "free_create_guild_times", m_other_cfg.free_create_guild_times) || m_other_cfg.free_create_guild_times < 0)
	{
		return -30;
	}

	if (!PugiGetSubNodeValue(dataElement, "extend_member_item", m_other_cfg.extend_member_itemID) || m_other_cfg.extend_member_itemID < 0 || NULL == ITEMPOOL->GetItem(m_other_cfg.extend_member_itemID))
	{
		return -31;
	}

	if (!PugiGetSubNodeValue(dataElement, "daily_relive_modulus", m_other_cfg.daily_relive_modulus) || m_other_cfg.daily_relive_modulus < 0)
	{
		return -32;
	}

	if (!PugiGetSubNodeValue(dataElement, "craete_guild_init_relive_times", m_other_cfg.craete_guild_init_relive_times) || m_other_cfg.craete_guild_init_relive_times < 0)
	{
		return -33;
	}

	if (!PugiGetSubNodeValue(dataElement, "singal_chat_max_time", m_other_cfg.singal_chat_max_time) || m_other_cfg.singal_chat_max_time < 0)
	{
		return -34;
	}

	if (!PugiGetSubNodeValue(dataElement, "today_saizi_count", m_other_cfg.today_saizi_count) || m_other_cfg.today_saizi_count < 0)
	{
		return -35;
	}

	if (!PugiGetSubNodeValue(dataElement, "siai_cold_down", m_other_cfg.siai_cold_down) || m_other_cfg.siai_cold_down < 0)
	{
		return -36;
	}

	if (!PugiGetSubNodeValue(dataElement, "singal_chat_interval", m_other_cfg.singal_chat_interval) || m_other_cfg.singal_chat_interval < 0)
	{
		return -37;
	}

	if (!PugiGetSubNodeValue(dataElement, "siai_mojing", m_other_cfg.siai_mojing) || m_other_cfg.siai_mojing < 0)
	{
		return -38;
	}
	if (!PugiGetSubNodeValue(dataElement, "chengyuan_event_notice_vip_level", m_other_cfg.chengyuan_event_notice_vip_level) || m_other_cfg.chengyuan_event_notice_vip_level < 0)
	{
		return -39;
	}
	if (!PugiGetSubNodeValue(dataElement, "first_change_avatar_give_bind_coin", m_other_cfg.first_change_avatar_give_bind_coin) || m_other_cfg.first_change_avatar_give_bind_coin < 0)
	{
		return -40;
	}

	if (!PugiGetSubNodeValue(dataElement, "exp_factor", m_other_cfg.exp_factor) || m_other_cfg.exp_factor < 0)
	{
		return -41;
	}

	if (!PugiGetSubNodeValue(dataElement, "day_juanxian_gold_limit", m_other_cfg.day_juanxian_gold_limit) || m_other_cfg.day_juanxian_gold_limit < 0)
	{
		return -42;
	}

	if (!PugiGetSubNodeValue(dataElement, "juanxian_gold_add_exp", m_other_cfg.juanxian_gold_add_exp) || m_other_cfg.juanxian_gold_add_exp <= 0)
	{
		return -43;
	}

	if (!PugiGetSubNodeValue(dataElement, "auto_kickout_level_limit", m_other_cfg.auto_kickout_level_limit) || m_other_cfg.auto_kickout_level_limit <= 0)
	{
		return -44;
	}

	if (!PugiGetSubNodeValue(dataElement, "auto_kickout_vip_limit", m_other_cfg.auto_kickout_vip_limit) || m_other_cfg.auto_kickout_vip_limit < 0)
	{
		return -45;
	}

	if (!PugiGetSubNodeValue(dataElement, "auto_kickout_time_s_limit", m_other_cfg.auto_kickout_time_s_limit) || m_other_cfg.auto_kickout_time_s_limit <= 0)
	{
		return -46;
	}

	if (!PugiGetSubNodeValue(dataElement, "auto_kickout_member_critical_per", m_other_cfg.auto_kickout_member_critical_per) ||
		m_other_cfg.auto_kickout_member_critical_per <= 0 || m_other_cfg.auto_kickout_member_critical_per > 100)
	{
		return -47;
	}

	if (!PugiGetSubNodeValue(dataElement, "cross_boss_show_day", m_other_cfg.kill_cross_boss_open_day) || m_other_cfg.kill_cross_boss_open_day <= 0)
	{
		return -48;
	}

	if (!PugiGetSubNodeValue(dataElement, "kill_cross_boss_mengzhu_skill", m_other_cfg.kill_cross_boss_mengzhu_skill) || m_other_cfg.kill_cross_boss_mengzhu_skill < 0)
	{
		return -49;
	}

	if (!PugiGetSubNodeValue(dataElement, "need_gongzi", m_other_cfg.need_gongzi) || m_other_cfg.need_gongzi <= 0)
	{
		return -50;
	}

	if (!PugiGetSubNodeValue(dataElement, "min_give_gongzi_member_num", m_other_cfg.min_give_gongzi_member_num) || m_other_cfg.min_give_gongzi_member_num <= 0)
	{
		return -51;
	}

	if (!PugiGetSubNodeValue(dataElement, "storage_constant_item_id", m_other_cfg.storage_constant_item_id) || 
		m_other_cfg.storage_constant_item_id <= 0 || !ITEMPOOL->GetItem(m_other_cfg.storage_constant_item_id))
	{
		return -52;
	}

	if (!PugiGetSubNodeValue(dataElement, "constant_item_storage_score", m_other_cfg.constant_item_storage_score) || m_other_cfg.constant_item_storage_score <= 0)
	{
		return -53;
	}

	int count = ItemConfigData::ReadConfigList(dataElement, "kill_cross_boss_reward", m_other_cfg.kill_cross_boss_reward, MAX_ATTACHMENT_ITEM_NUM);
	if (count < 0)
	{
		return -100 + count;
	}

	if (!PugiGetSubNodeValue(dataElement, "broadcast_grade", m_other_cfg.broadcast_grade) || m_other_cfg.broadcast_grade <= 0)
	{
		return -54;
	}
	
	return 0;
}

int GuildConfig::InitJuanXianCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	int item_num = 0;
	while (!dataElement.empty())
	{
		if (item_num >= GUILD_MAX_UPLEVEL_STUFF_COUNT)
		{
			return -100;
		}

		GuildJuanXianConfig::ConfigItem config_item;

		ItemID item_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "item_id", item_id) ||  NULL == ITEMPOOL->GetItem(item_id))
		{
			return -1;
		}
		
		if (m_juanxian_cfg.juanxian_map.end() != m_juanxian_cfg.juanxian_map.find(item_id))
		{
			return -2;
		}

		config_item.item_id = item_id;
		if (!PugiGetSubNodeValue(dataElement, "add_gongxian", config_item.add_gongxian) || config_item.add_gongxian <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_guild_exp", config_item.add_guild_exp) || config_item.add_guild_exp < 0)
		{
			return -4;
		}

		m_juanxian_cfg.juanxian_map[config_item.item_id] = config_item;
		++item_num;
		dataElement = dataElement.next_sibling();
	}
	
	return 0;
}

int GuildConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	int last_level = 0;
	int last_max_exp = 0;
	int last_max_member = 0;
	
	while (!dataElement.empty())
	{
		int guild_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", guild_level) ||  guild_level != last_level + 1)
		{
			return -1;
		}

		GuildLevelConfig::ConfigItem &cfg_item = m_level_cfg.cfg_list[guild_level];
		cfg_item.level = guild_level;
		last_level = cfg_item.level;

		if (!PugiGetSubNodeValue(dataElement, "max_exp", cfg_item.max_exp) || cfg_item.max_exp <= 0 || cfg_item.max_exp <= last_max_exp)
		{
			return -2;
		}
		last_max_exp = cfg_item.max_exp;

		if (!PugiGetSubNodeValue(dataElement, "max_member", cfg_item.max_member) || cfg_item.max_member < last_max_member)
		{
			return -3;
		}
		last_max_member = cfg_item.max_member;

		if (!PugiGetSubNodeValue(dataElement, "consume_exp_eachday", cfg_item.comsume_exp_perday) || cfg_item.comsume_exp_perday < 0)
		{
			return -4;
		}

		{
			PugiXmlNode item_element = dataElement.child("daily_reward_item");
			if (item_element.empty() || !cfg_item.daily_reward_item.ReadConfig(item_element))
			{
				return -101;
			}
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitExchangeCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}
	
	int item_num = 0;
	while (!dataElement.empty())
	{
		GuildExchangeConfig::ConfigItem config_item;

		if (item_num >= GUILD_MAX_EXCHANGE_ITEM_COUNT)
		{
			return -100;
		}

		ItemID item_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "item_id", item_id) ||  NULL == ITEMPOOL->GetItem(item_id))
		{
			return -1;
		}

		if (m_exchange_cfg.exchange_map.end() != m_exchange_cfg.exchange_map.find(item_id))
		{
			return -2;
		}

		config_item.item_id = item_id;
		if (!PugiGetSubNodeValue(dataElement, "guild_level_limit", config_item.guild_level_limit) || config_item.guild_level_limit <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "cost_gongxian", config_item.cost_gongxian) || config_item.cost_gongxian <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_day_times", config_item.max_day_exchange_num) || config_item.max_day_exchange_num <= 0)
		{
			return -5;
		}

		++ item_num;
		m_exchange_cfg.exchange_map[config_item.item_id] = config_item;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitMemberLuckyCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	int last_lucky_color = 0;
	while (!dataElement.empty())
	{
		int lucky_color = 0;
		if (!PugiGetSubNodeValue(dataElement, "lucky_color", lucky_color) ||  lucky_color != last_lucky_color + 1 || lucky_color <= GUILD_MEMBER_LUCKY_COLOR_INVALID || 
			lucky_color >= GUILD_MEMBER_LUCKY_COLOR_MAX)
		{
			return -1;
		}
		last_lucky_color = lucky_color;

 		MemberLuckBlessingCfg &bless_cfg = m_luck_blessing_cfg_list[lucky_color];

		{
			int total_prob = 0;

			char temp_tag[16] = {0};
			for (int i = GUILD_MEMBER_LUCKY_COLOR_INVALID + 1; i < GUILD_MEMBER_LUCKY_COLOR_MAX; ++i)
			{
				SNPRINTF(temp_tag, sizeof(temp_tag), "lucky_color_%d", i);
				if (!PugiGetSubNodeValue(dataElement, temp_tag, bless_cfg.upgrade_prob_list[i]))
				{
					return -10;
				}

				total_prob += bless_cfg.upgrade_prob_list[i];
			}
			
			if (100 != total_prob)
			{
				return -105;
			}
		}

		{
			if (!PugiGetSubNodeValue(dataElement, "task_factor", bless_cfg.task_factor) || bless_cfg.task_factor < 0)
			{
				return -101;
			}
			if (!PugiGetSubNodeValue(dataElement, "boss_drop_factor", bless_cfg.boss_drop_factor) || bless_cfg.boss_drop_factor < 0)
			{
				return -103;
			}
			if (!PugiGetSubNodeValue(dataElement, "fb_drop_factor", bless_cfg.fb_drop_factor) || bless_cfg.fb_drop_factor < 0)
			{
				return -104;
			}
		}

		dataElement = dataElement.next_sibling();
	}

	
	return 0;
}

int GuildConfig::InitStorgeCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	int last_level = 0;
	int last_count = 0;
	while (!dataElement.empty())
	{
		int level = 0;
		int open_count = 0;

		if (!PugiGetSubNodeValue(dataElement, "level", level) || 1 != level - last_level || level > GUILD_LEVEL_MAX)
		{
			return -1;
		}
		last_level = level;

		if (!PugiGetSubNodeValue(dataElement, "open_count", open_count) || open_count < last_count || open_count > GUILD_STORAGE_MAX_GRID_NUM)
		{
			return -2;
		}
		last_count = open_count;

		m_storge_cfg.storge_open_grid_list[level] = open_count;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitSkillCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		int skill_index = 0;
		int skill_level = 0;

		if (!PugiGetSubNodeValue(dataElement, "skill_idx", skill_index) || skill_index < 0 || skill_index > GUILD_SKILL_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "level", skill_level) || skill_level < 0 || skill_level > MAX_GUILD_SKILL_LEVEL)
		{
			return -2;
		}

		GuildSkillCfg::ConfigItem &cfg = m_skill_cfg.item_list[skill_index][skill_level];
		if (cfg.uplevel_gongxian > 0)
		{
			return -3;
		}

		cfg.skill_index = skill_index;
		cfg.level = skill_level;

		if (!PugiGetSubNodeValue(dataElement, "uplevel_gongxian", cfg.uplevel_gongxian) || cfg.uplevel_gongxian < 0)
		{
			return -11;
		}
		if (!PugiGetSubNodeValue(dataElement, "uplevel_stuff_id", cfg.uplevel_stuff_id))
		{
			return -111;
		}
		if (!PugiGetSubNodeValue(dataElement, "uplevel_stuff_count", cfg.uplevel_stuff_count) || cfg.uplevel_stuff_count < 0)
		{
			return -112;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0)
		{
			return -18;
		}

		if (!PugiGetSubNodeValue(dataElement, "zhufuyiji_per", cfg.zhufuyiji_per) || cfg.zhufuyiji_per < 0)
		{
			return -19;
		}

		if (!PugiGetSubNodeValue(dataElement, "guild_level_limit", cfg.guild_level_limit) || cfg.guild_level_limit <= 0)
		{
			return -20;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitTotemCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level < 0 || level > MAX_GUILD_TOTEM_LEVEL)
		{
			return -1;
		}

		GuildTotemCfg::ConfigItem &cfg = m_totem_cfg.item_list[level];
		if (cfg.uplevel_exp > 0)
		{
			return -2;
		}

		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "max_exp", cfg.uplevel_exp) || cfg.uplevel_exp < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0)
		{
			return -18;
		}

		if (!PugiGetSubNodeValue(dataElement, "leader_maxhp", cfg.leader_maxhp) || cfg.leader_maxhp < 0)
		{
			return -22;
		}

		if (!PugiGetSubNodeValue(dataElement, "leader_gongji", cfg.leader_gongji) || cfg.leader_gongji < 0)
		{
			return -23;
		}

		if (!PugiGetSubNodeValue(dataElement, "leader_fangyu", cfg.leader_fangyu) || cfg.leader_fangyu < 0)
		{
			return -24;
		}

		if (!PugiGetSubNodeValue(dataElement, "leader_mingzhong", cfg.leader_mingzhong) || cfg.leader_mingzhong < 0)
		{
			return -25;
		}

		if (!PugiGetSubNodeValue(dataElement, "leader_shanbi", cfg.leader_shanbi) || cfg.leader_shanbi < 0)
		{
			return -26;
		}

		if (!PugiGetSubNodeValue(dataElement, "leader_baoji", cfg.leader_baoji) || cfg.leader_baoji < 0)
		{
			return -27;
		}

		if (!PugiGetSubNodeValue(dataElement, "leader_jianren", cfg.leader_jianren) || cfg.leader_jianren < 0)
		{
			return -28;
		}

		if (!PugiGetSubNodeValue(dataElement, "bless_exp", cfg.bless_exp) || cfg.bless_exp < 0)
		{
			return -29;
		}

		if (!PugiGetSubNodeValue(dataElement, "totem_hp", cfg.totem_hp) || cfg.totem_hp < 0)
		{
			return -30;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitBoxCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	m_guild_box_total_rate = 0;

	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level < 0 || level > MAX_GUILD_BOX_LEVEL)
		{
			return -1;
		}

		GuildBoxCfg::ConfigItem &cfg = m_box_cfg.item_list[level];
		cfg.level = level;

		//{
		//	PugiXmlNode element = dataElement.child("item_reward");
		//	if (element.empty() || !cfg.item_reward.ReadConfig(element))
		//	{
		//		return -2;
		//	}
		//}

		{
			cfg.item_reward_count = ItemConfigData::ReadConfigList(dataElement, std::string("item_reward"),cfg.item_reward_list, MAX_ATTACHMENT_ITEM_NUM);
			if (cfg.item_reward_count < 0)
			{
				return -200 + cfg.item_reward_count;
			}
		}

		{
			/*PugiXmlNode element = dataElement.child("assist_reward");
			if (element.empty() || !cfg.assist_reward.ReadConfig(element))
			{
				return -3;
			}*/
		}

		if (!PugiGetSubNodeValue(dataElement, "be_assist_reward_bind_gold", cfg.be_assist_reward_bind_gold) || cfg.be_assist_reward_bind_gold < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "init_rate", cfg.init_rate) || cfg.init_rate < 0)
		{
			return -12;
		}

		m_guild_box_total_rate += cfg.init_rate;

		if (!PugiGetSubNodeValue(dataElement, "uplevel_rate", cfg.uplevel_rate) || cfg.uplevel_rate < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "box_open_time", cfg.box_open_time) || cfg.box_open_time < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "box_assist_dec_time", cfg.box_assist_dec_time) || cfg.box_assist_dec_time < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement, "can_be_assist_max_count", cfg.can_be_assist_max_count) || cfg.can_be_assist_max_count < 0 || cfg.can_be_assist_max_count > MAX_GUILD_BOX_BE_ASSIST_COUNT)
		{
			return -16;
		}

		{
			PugiXmlNode element = dataElement.child("random_reward");
			if (element.empty() || !cfg.random_reward.ReadConfig(element))
			{
				return -17;
			}
		}

		if (!PugiGetSubNodeValue(dataElement, "probability", cfg.probability) || cfg.probability < 0 )
		{
			return -18;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitTerrityoryWelfCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		int territory_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "territory_index", territory_index) || territory_index < 0 || territory_index > MAX_TERRITORY_WELF_COUNT)
		{
			return -1;
		}

		GuildTerritoryWelfCfg::ConfigItem &cfg = m_territory_welf_cfg.item_list[territory_index];
		cfg.territory_index = territory_index;

		{
			PugiXmlNode element = dataElement.child("phase_one_reward");
			if (element.empty() || !cfg.phase_one_reward.ReadConfig(element))
			{
				return -2;
			}
		}

		{
			PugiXmlNode element = dataElement.child("phase_two_reward");
			if (element.empty() || !cfg.phase_two_reward.ReadConfig(element))
			{
				return -3;
			}
		}

		{
			PugiXmlNode element = dataElement.child("phase_three_reward");
			if (element.empty() || !cfg.phase_three_reward.ReadConfig(element))
			{
				return -4;
			}
		}

		{
			PugiXmlNode element = dataElement.child("phase_four_reward");
			if (element.empty() || !cfg.phase_four_reward.ReadConfig(element))
			{
				return -5;
			}
		}

		{
			PugiXmlNode element = dataElement.child("guild_president_extra_reward");
			if (element.empty() || !cfg.guild_president_extra_reward.ReadConfig(element))
			{
				return -6;
			}
		}

		if (!PugiGetSubNodeValue(dataElement, "banggong_one_limit", cfg.banggong_one_limit) || cfg.banggong_one_limit < 0 )
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "banggong_two_limit", cfg.banggong_two_limit) || cfg.banggong_two_limit < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "banggong_three_limit", cfg.banggong_three_limit) || cfg.banggong_three_limit < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "banggong_four_limit", cfg.banggong_four_limit) || cfg.banggong_four_limit < 0)
		{
			return -10;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitExtendMemberCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	m_guild_extend_member_cfg_count = 0;
	int last_min_extend_member_num = -1;

	while (!dataElement.empty())
	{
		if (m_guild_extend_member_cfg_count >= GUILD_EXTEND_MEMBER_CFG_MAX_COUNT)
		{
			return - 1;
		}

		GuildExtendMemberCfg &cfg_item = m_extend_member_cfg_list[m_guild_extend_member_cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "member_count", cfg_item.min_extend_member_num) || cfg_item.min_extend_member_num <= last_min_extend_member_num || cfg_item.min_extend_member_num > MAX_MEMBER_COUNT)
		{
			return -2;
		}
		last_min_extend_member_num = cfg_item.min_extend_member_num;

		if (!PugiGetSubNodeValue(dataElement, "need_item_count", cfg_item.need_item_count) || cfg_item.need_item_count <= 0)
		{
			return -3;
		}

		++ m_guild_extend_member_cfg_count;
		dataElement = dataElement.next_sibling();
	}

	if (0 != m_extend_member_cfg_list[0].min_extend_member_num)
	{
		return -3;
	}
	
	return 0;
}

int GuildConfig::InitPostReliveTimesCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}
	
	int last_post = 0;
	int post = 0;

	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "guild_post", post) || post != last_post + 1)
		{
			return -1;
		}

		last_post = post;

		if (!PugiGetSubNodeValue(dataElement, "daily_relive_times", m_post_relive_times[post]) || m_post_relive_times[post] < 0)
		{
			return -2;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitBossGuildRedbagCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		BossGuildRedbagCfg config_item;

		if (!PugiGetSubNodeValue(dataElement, "kill_boss_times", config_item.kill_boss_times) || config_item.kill_boss_times <= 0)
		{
			return -1;
		}
		
		if (!PugiGetSubNodeValue(dataElement, "max_diamond", config_item.max_diamond) || config_item.max_diamond <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "guild_relive_times", config_item.guild_relive_times) || config_item.guild_relive_times <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "index", config_item.index) || config_item.index < 0)
		{
			return -4;
		}
		
		m_boss_guild_redbag_cfg.push_back(config_item);
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitGuildRedPaperCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	int last_seq = 0;
	while (!dataElement.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(dataElement, "seq", seq) || seq <= 0 || seq != last_seq + 1 || m_guild_red_paper_map.find(seq) != m_guild_red_paper_map.end())
		{
			return -1;
		}

		GuildRedPaperCfg paper_cfg;
		paper_cfg.paper_seq = seq;
		last_seq = seq;

		if (!PugiGetSubNodeValue(dataElement, "paper_type", paper_cfg.paper_type) || paper_cfg.paper_type < 0)
		{
			return -2;
		}
		
		if (!PugiGetSubNodeValue(dataElement, "paper_level", paper_cfg.paper_level) || paper_cfg.paper_level <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "param", paper_cfg.cond_param) || paper_cfg.cond_param < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "bind_gold", paper_cfg.bind_gold) || paper_cfg.bind_gold <= 0)
		{
			return -5;
		}

		m_guild_red_paper_map[seq] = paper_cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitGuildSaiziCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	int index = 0;
	while (!dataElement.empty())
	{
		if (index < 0 || index >= MAX_GUILD_SAIZI_REWARD)
		{
			return -1000;
		}

		GuildSaiziCfg & cfg = m_guild_saizi_cfg[index];

		if (!PugiGetSubNodeValue(dataElement, "rank1", cfg.rank1) || cfg.rank1 < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "rank2", cfg.rank2) || cfg.rank2 <= 0)
		{
			return -2;
		}

		{
			PugiXmlNode element = dataElement.child("reward_item_list");
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

				if (!cfg.reward_item_list[i].ReadConfig(item_element))
				{
					return -151 - i;
				}

				i++;
				cfg.reward_item_count++;
				item_element = item_element.next_sibling();
			}
		}

		index++;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitGuildMazeCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		int layer = 0;
		if (!PugiGetSubNodeValue(dataElement, "layer", layer) || layer <= 0 || layer > GUILD_MAZE_LAYER_MAX)
		{
			return -1;
		}

		GuildMazeCfg &cfg = m_maze_cfg_list[layer];
		cfg.layer = layer;

		if (!PugiGetSubNodeValue(dataElement, "fail_down_layer", cfg.fail_down_layer) || cfg.fail_down_layer < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "fail_cd", cfg.fail_cd) || cfg.fail_cd < 0)
		{
			return -3;
		}

		int item_count = ItemConfigData::ReadConfigList(dataElement, "reward_item", cfg.reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
		if (item_count < 0)
		{
			return -100 + item_count;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitGuildMazeRankRewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	int last_rank = 0;
	while (!dataElement.empty())
	{
		GuildMazeRankRewardCfg cfg;
		if (!PugiGetSubNodeValue(dataElement, "rank", cfg.rank) || cfg.rank != last_rank + 1)
		{
			return -1;
		}
		last_rank++;

		int item_count = ItemConfigData::ReadConfigList(dataElement, "reward_item", cfg.reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
		if (item_count < 0)
		{
			return -100 + item_count;
		}

		m_maze_rank_reward_vec.push_back(cfg);
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitGuilInitMemberCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		std::string plat_name;
		if (!PugiGetSubNodeValue(dataElement, "plat_name", plat_name) || plat_name.empty())
		{
			return -1;
		}

		if (m_plat_init_member_map.find(plat_name) != m_plat_init_member_map.end())
		{
			return -2;
		}

		int guild_count = 0;
		if (!PugiGetSubNodeValue(dataElement, "guild_count", guild_count) || guild_count < 0)
		{
			return -3;
		}

		m_plat_init_member_map[plat_name] = guild_count;

		if (m_plat_init_member_map.find("default") == m_plat_init_member_map.end())
		{
			return -20;
		}
		
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitPlatNameCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	std::string guild_default_plat_name = "default";
	while (!dataElement.empty())
	{
		std::string plat_name;
		if (!PugiGetSubNodeValue(dataElement, "plat_name", plat_name) || plat_name.empty())
		{
			return -1;
		}

		if (m_plat_name_consume_map.find(plat_name) != m_plat_name_consume_map.end())
		{
			return -2;
		}

		GuildPlatNameCfg guild_plat_name_cfg;
		/*if (!PugiGetSubNodeValue(dataElement, "currency_type", guild_plat_name_cfg.currency_type) || guild_plat_name_cfg.currency_type < 0)
		{
			return -3;
		}*/

		if (!PugiGetSubNodeValue(dataElement, "gold", guild_plat_name_cfg.gold) || guild_plat_name_cfg.gold < 0)
		{
			return -4;
		}

		m_plat_name_consume_map[plat_name] = guild_plat_name_cfg;

		if (m_plat_name_consume_map.find(guild_default_plat_name) == m_plat_name_consume_map.end())
		{
			return -20;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitYunbiaoOtherCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

// 	if (!PugiGetSubNodeValue(dataElement, "yunbiao_start_hour", m_start_limit.hour) || m_start_limit.hour <= 0)
// 	{
// 		return -1;
// 	}
// 	if (!PugiGetSubNodeValue(dataElement, "yunbiao_start_min", m_start_limit.minute) || m_start_limit.minute < 0)
// 	{
// 		
// 		return -2;
// 	}
// 
// 	if (!PugiGetSubNodeValue(dataElement, "yunbiao_end_hour", m_end_limit.hour) || m_end_limit.hour <= 0 || m_end_limit.hour <= m_start_limit.hour) // 结束的小时一定要比开始的小时大 24小时制
// 	{
// 		return -3;
// 	}
// 	if (!PugiGetSubNodeValue(dataElement, "yunbiao_end_min", m_end_limit.minute) || m_end_limit.minute < 0)
// 	{
// 		return -4;
// 	}
// 
// 	if (m_end_limit.hour >= 23 || m_start_limit.hour <= 8)  // guild ondaychange 有依赖，不得修改数字 （因为guild的ondaychange直接修改了镖车的状态）
// 	{
// 		return -5;
// 	}

	if (!PugiGetSubNodeValue(dataElement, "biaoche_scene_id", m_biaoche_sceneid) || m_biaoche_sceneid <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "biaoche_report_pos_interval", m_biaoche_report_pos_interval) || m_biaoche_report_pos_interval <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "biaoche_be_hurt_report_interval", m_biaoche_be_hurt_report_interval) || m_biaoche_be_hurt_report_interval <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement, "husong_exp_interval", m_husong_exp_interval) || m_husong_exp_interval <= 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElement, "husong_exp_range", m_husong_exp_range) || m_husong_exp_range <= 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElement, "guild_rob_count_max", m_guild_rob_count_max) || m_guild_rob_count_max <= 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(dataElement, "member_rob_count_max", m_member_rob_count_max) || m_member_rob_count_max <= 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(dataElement, "biaoche_monster_id", m_biaoche_monsterid) || m_biaoche_monsterid <= 0)
	{
		return -13;
	}

	return 0;
}

int GuildConfig::InitYunbiaoInfoCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		int guild_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "guild_level", guild_level) || guild_level <= 0 || guild_level > GUILD_LEVEL_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "total_guild_exp", m_biaoche_lvinfo_list[guild_level].total_guild_exp) || m_biaoche_lvinfo_list[guild_level].total_guild_exp <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "rob_guild_exp", m_biaoche_lvinfo_list[guild_level].rob_guild_exp) || m_biaoche_lvinfo_list[guild_level].rob_guild_exp <= 0)
		{
			return -5;
		}

		{
			m_biaoche_lvinfo_list[guild_level].succ_reward_count = ItemConfigData::ReadConfigList(dataElement, std::string("succ_reward_item"),
				m_biaoche_lvinfo_list[guild_level].succ_reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
			if (m_biaoche_lvinfo_list[guild_level].succ_reward_count < 0)
			{
				return -200 + m_biaoche_lvinfo_list[guild_level].succ_reward_count;
			}
		}

		{
			m_biaoche_lvinfo_list[guild_level].fail_reward_count = ItemConfigData::ReadConfigList(dataElement, std::string("fail_reward_item"),
				m_biaoche_lvinfo_list[guild_level].fail_reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
			if (m_biaoche_lvinfo_list[guild_level].fail_reward_count < 0)
			{
				return -300 + m_biaoche_lvinfo_list[guild_level].fail_reward_count;
			}
		}

		{
			m_biaoche_lvinfo_list[guild_level].rob_reward_count = ItemConfigData::ReadConfigList(dataElement, std::string("rob_reward_item"),
				m_biaoche_lvinfo_list[guild_level].rob_reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
			if (m_biaoche_lvinfo_list[guild_level].rob_reward_count < 0)
			{
				return -400 + m_biaoche_lvinfo_list[guild_level].rob_reward_count;
			}
		}

		if (!PugiGetSubNodeValue(dataElement, "succ_reward_exp_factor", m_biaoche_lvinfo_list[guild_level].succ_reward_exp_factor) || m_biaoche_lvinfo_list[guild_level].succ_reward_exp_factor  < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "fail_reward_exp_factor", m_biaoche_lvinfo_list[guild_level].fail_reward_exp_factor) || m_biaoche_lvinfo_list[guild_level].fail_reward_exp_factor < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "succ_gongxian", m_biaoche_lvinfo_list[guild_level].succ_gongxian) || m_biaoche_lvinfo_list[guild_level].succ_gongxian < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "fail_gongxian", m_biaoche_lvinfo_list[guild_level].fail_gongxian) || m_biaoche_lvinfo_list[guild_level].fail_gongxian < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "rob_gongxian", m_biaoche_lvinfo_list[guild_level].rob_gongxian) || m_biaoche_lvinfo_list[guild_level].rob_gongxian < 0)
		{
			return -10;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitYunbiaoExpCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{

		BiaocheHusongExpCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "role_level", cfg.role_level) || cfg.role_level <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "role_add_exp", cfg.role_add_exp) || cfg.role_add_exp <= 0)
		{
			return -2;
		}

		m_biaoche_husong_exp_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitYunbiaoAttrCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	int count = 0;
	while (!dataElement.empty())
	{

		if (count >= BIAOCHE_ATTR_MAX)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(dataElement, "world_level", m_biaoche_attr_list[count].world_minlv) || m_biaoche_attr_list[count].world_minlv <= 0)
		{
			return -1;
		}

		if (count > 0 && m_biaoche_attr_list[count].world_minlv <= m_biaoche_attr_list[count - 1].world_minlv)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_max_hp", m_biaoche_attr_list[count].max_hp) || m_biaoche_attr_list[count].max_hp <= 0)
		{
			return -3;
		}

		++count;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitYunbiaoPathCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	int count = 0;
	while (!dataElement.empty())
	{

		if (count >= BIAOCHE_POS_MAX)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x", m_pos_list[count].x) || m_pos_list[count].x <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", m_pos_list[count].y) || m_pos_list[count].y <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "goal_percent", m_goal_percent[count]) || m_goal_percent[count] < 0)
		{
			return -3;
		}

		++count;

		m_pos_count = count;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitGuildRumorCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		int rumor_type = 0;
		if (!PugiGetSubNodeValue(dataElement, "rumors_type", rumor_type) || rumor_type <= 0)
		{
			return -1;
		}

		GuildRumorCfg &cfg = m_guild_rumor_map[rumor_type];

		if (!PugiGetSubNodeValue(dataElement, "param1", cfg.param_1) || cfg.param_1 < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "param2", cfg.param_2) || cfg.param_2 < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "param3", cfg.param_3) || cfg.param_3 < 0)
		{
			return -4;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitGuilGongziCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		int guild_post = 0;
		if (!PugiGetSubNodeValue(dataElement, "guild_post", guild_post) || guild_post <= GUILD_POST_INVALID || guild_post >= GUILD_POST_MAX)
		{
			return -1;
		}

		GuildGongziCfg &cfg = m_gongzi_cfg_list[guild_post];
		cfg.guild_post = guild_post;

		if (!PugiGetSubNodeValue(dataElement, "rate", cfg.rate) || cfg.rate <= 0)
		{
			return -2;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitGuildGongziGiveCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		int boss_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "boss_id", boss_id) || boss_id <= 0 || !MONSTERPOOL->IsMonsterExist(boss_id))
		{
			return -1;
		}

		if (m_gongzi_give_map.find(boss_id) != m_gongzi_give_map.end())
		{
			return -100;
		}

		GuildGongziGiveCfg &cfg = m_gongzi_give_map[boss_id];
		
		if (!PugiGetSubNodeValue(dataElement, "gongzi_give_type", cfg.gongzi_give_type) || cfg.gongzi_give_type <= GUILD_GONGZI_GIVE_TYPE_INVALID || cfg.gongzi_give_type >= GUILD_GONGZI_GIVE_TYPE_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "give_gongzi", cfg.give_gongzi) || cfg.give_gongzi <= 0)
		{
			return -3;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildConfig::InitRareItemCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		ItemID rare_item_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "rare_item_id", rare_item_id) || NULL == ITEMPOOL->GetItem(rare_item_id))
		{
			return -1;
		}

		if (rare_item_set.find(rare_item_id) == rare_item_set.end())
		{
			rare_item_set.insert(rare_item_id);
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const GuildJuanXianConfig::ConfigItem * GuildConfig::GetJuanXianConfig(ItemID item_id)
{
	if (item_id <= 0) return NULL;

	std::map<ItemID, GuildJuanXianConfig::ConfigItem>::const_iterator iter = m_juanxian_cfg.juanxian_map.find(item_id);
	if (m_juanxian_cfg.juanxian_map.end() == iter)
	{
		return NULL;
	}
	
	return &iter->second;
}

const GuildLevelConfig::ConfigItem * GuildConfig::GetLevelConfig(int level)
{
	if (level >= 1 && level <= GUILD_LEVEL_MAX && m_level_cfg.cfg_list[level].level > 0)
	{
		return &m_level_cfg.cfg_list[level];
	}

	return NULL;
}

const GuildExchangeConfig::ConfigItem * GuildConfig::GetExchangeConfig(ItemID item_id)
{
	if (item_id <= 0) return NULL;

	std::map<ItemID, GuildExchangeConfig::ConfigItem>::const_iterator iter = m_exchange_cfg.exchange_map.find(item_id);
	if (m_exchange_cfg.exchange_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

int GuildConfig::GetGuildReduceExpDay(int guild_level)
{
	const GuildLevelConfig::ConfigItem *cfg_item = this->GetLevelConfig(guild_level);
	if (NULL == cfg_item)
	{
		return 0;
	}

	return cfg_item->comsume_exp_perday;
}

int GuildConfig::GetGuildLuckyFactor(int lucky_color, int factor_type)
{
// 	if (lucky_color <= GUILD_MEMBER_LUCKY_COLOR_INVALID || lucky_color >= GUILD_MEMBER_LUCKY_COLOR_MAX || 
// 		factor_type <= GUILD_LUCKY_FACTOR_TYPE_INVALID || factor_type >= GUILD_LUCKY_FACTOR_TYPE_MAX)
// 	{
// 		return 0;
// 	}
// 
// 	MemberLuckBlessingCfg *lucky_cfg = &m_luck_blessing_cfg_list[lucky_color];
// 
// 	if (GUILD_LUCKY_FACTOR_TYPE_GUILDTASK == factor_type)
// 	{
// 		return lucky_cfg->task_factor;
// 	}
// 	else if (GUILD_LUCKY_FACTOR_TYPE_BOSSDROP == factor_type)
// 	{
// 		return lucky_cfg->boss_drop_factor;
// 	}
// 	else if (GUILD_LUCKY_FACTOR_TYPE_FBDROP == factor_type)
// 	{
// 		return lucky_cfg->fb_drop_factor;
// 	}

	return 0;
}

int GuildConfig::GetGuildExtendMemberNeedItemCount(int cur_member_count)
{
	for (int i = m_guild_extend_member_cfg_count - 1; i >= 0; -- i)
	{
		if (cur_member_count >= m_extend_member_cfg_list[i].min_extend_member_num)
		{
			return m_extend_member_cfg_list[i].need_item_count;
		}
	}

	return 0;
}

GuildTerritoryWelfCfg::ConfigItem* GuildConfig::GetGuildTerritoryWelf(int index)
{
	if (index < 0 || index > MAX_TERRITORY_WELF_COUNT)
	{
		return NULL;
	}

	return &m_territory_welf_cfg.item_list[index];
}

MemberLuckBlessingCfg * GuildConfig::GetMemberLuckBlessingCfg(int lucky_color)
{
	if (lucky_color <= GUILD_MEMBER_LUCKY_COLOR_INVALID || lucky_color >= GUILD_MEMBER_LUCKY_COLOR_MAX)
	{
		return NULL;
	}

	return &m_luck_blessing_cfg_list[lucky_color];
}

int GuildConfig::GetGuildStorgeOpenCount(int level)
{
	if (level < 0 || level > GUILD_LEVEL_MAX)
	{
		return 0;
	}

	return m_storge_cfg.storge_open_grid_list[level];
}

const GuildSkillCfg::ConfigItem * GuildConfig::GetSkillConfig(int skill_index, int skill_level)
{
	if (skill_index < 0 || skill_index >= GUILD_SKILL_COUNT || skill_level < 0 || skill_level > MAX_GUILD_SKILL_LEVEL)
	{
		return NULL;
	}

	if (m_skill_cfg.item_list[skill_index][skill_level].level != skill_level)
	{
		return NULL;
	}

	return &m_skill_cfg.item_list[skill_index][skill_level];
}

const GuildTotemCfg::ConfigItem * GuildConfig::GetTutemConfig(int tutem_level)
{
	if (tutem_level < 0 || tutem_level > MAX_GUILD_TOTEM_LEVEL)
	{
		return NULL;
	}

	if (m_totem_cfg.item_list[tutem_level].level != tutem_level)
	{
		return NULL;
	}

	return &m_totem_cfg.item_list[tutem_level];
}

GuildBoxCfg::ConfigItem * GuildConfig::GetGuildBoxCfg(int box_level)
{
	if (box_level < 0 || box_level > MAX_GUILD_BOX_LEVEL)
	{
		return NULL;
	}

	return &m_box_cfg.item_list[box_level];
}

int GuildConfig::RandGuildBoxLevel()
{
	if (m_guild_box_total_rate <= 0) return 0;
	int rand_num = RandomNum(m_guild_box_total_rate);

	for (int i = 0; i <= MAX_GUILD_BOX_LEVEL; i++)
	{
		if (rand_num < m_box_cfg.item_list[i].init_rate)
		{
			return i;
		}

		rand_num -= m_box_cfg.item_list[i].init_rate;
	}

	return 0;
}

BossGuildRedbagCfg * GuildConfig::GetBossGuildRedbagCfg(int index)
{
	if (index < 0 || index >= static_cast<int>(m_boss_guild_redbag_cfg.size()))
	{
		return NULL;
	}

	return &m_boss_guild_redbag_cfg[index];
}

BossGuildRedbagCfg * GuildConfig::GetBossGuildRedbagCfgByKillBossTimes(int times)
{
	if (times <= 0)
	{
		return NULL;
	}

	for (int i = 0; i < static_cast<int>(m_boss_guild_redbag_cfg.size()); i++)
	{
		if (m_boss_guild_redbag_cfg[i].kill_boss_times == times)
		{
			return &m_boss_guild_redbag_cfg[i];
		}
	}

	return NULL;
}

int GuildConfig::GetPostReliveTimes(int post)
{
	if (post < 0 || post >= GUILD_POST_MAX)
	{
		return 0;
	}

	return m_post_relive_times[post];
}

int GuildConfig::GetGuildRedPaperSeq(int paper_type, int cond_parm)
{
	if (paper_type <= GUILD_RED_PAPER_INVALID || paper_type >= GUILD_RED_PAPER_MAX) return 0;

	std::map<int, GuildRedPaperCfg>::iterator begin_iter =  m_guild_red_paper_map.begin();
	for(; begin_iter != m_guild_red_paper_map.end(); begin_iter++)
	{
		if (begin_iter->second.paper_type == paper_type && begin_iter->second.cond_param == cond_parm)
		{
			return begin_iter->second.paper_seq;
		}
	}

	return 0;
}

GuildSaiziCfg * GuildConfig::GetGuildSaiziCfg(int rank)
{
	for (int i = 0; i < MAX_GUILD_SAIZI_REWARD; i++)
	{
		if (m_guild_saizi_cfg[i].rank1 == rank || m_guild_saizi_cfg[i].rank2 == rank)
		{
			return &m_guild_saizi_cfg[i];
		}
	}
	return NULL;
}

GuildMazeCfg * GuildConfig::GetGuildMazeCfg(int layer)
{
	if (layer <= 0 || layer > GUILD_MAZE_LAYER_MAX)
	{
		return NULL;
	}

	return &m_maze_cfg_list[layer];
}

GuildMazeRankRewardCfg * GuildConfig::GetGuildMazeRankRewardCfg(int rank)
{
	if (rank < 0 || rank >= (int)m_maze_rank_reward_vec.size())
	{
		return NULL;
	}

	return &m_maze_rank_reward_vec[rank];
}

int GuildConfig::GetGuildInitMemberByPlatName(std::string plat_name)
{
	std::map<std::string, int>::iterator iter = m_plat_init_member_map.find(plat_name);
	if (iter != m_plat_init_member_map.end())
	{
		return iter->second;
	}
	else
	{
		iter = m_plat_init_member_map.find("default");
		if (iter != m_plat_init_member_map.end())
		{
			return iter->second;
		}
	}

	return 0;
}

const GuildPlatNameCfg * GuildConfig::GetGuildPlatNameCfg(std::string plat_name)
{
	if (m_plat_name_consume_map.find(plat_name) == m_plat_name_consume_map.end())
	{
		return nullptr;
	}
	return &m_plat_name_consume_map[plat_name];
}

bool GuildConfig::NowCanStartGuildYunBiao(int hour, int min)
{
	if (hour > m_start_limit.hour || (hour == m_start_limit.hour && min >= m_start_limit.minute)) // 大于开始时间
	{
		if (hour < m_end_limit.hour || (hour == m_end_limit.hour && min < m_end_limit.minute)) // 小于结束时间
		{
			return true;
		}
	}

	return false;
}

bool GuildConfig::GetBiaoChePos(int seq, Posi *get_pos)
{
	if (seq < 0 || seq >= m_pos_count || seq >= BIAOCHE_POS_MAX || NULL == get_pos) return false;

	*get_pos = m_pos_list[seq];

	return true;
}

int GuildConfig::GetBiaoCheGoalPercent(int seq)
{
	if (seq < 0 || seq >= m_pos_count || seq >= BIAOCHE_POS_MAX)
	{
		return 0;
	}

	return m_goal_percent[seq];
}

int GuildConfig::GetBiaoCheAddMaxHp(int world_lv)
{
	for (int i = BIAOCHE_ATTR_MAX - 1; i >= 0; i--)
	{
		if (m_biaoche_attr_list[i].world_minlv > 0 && world_lv >= m_biaoche_attr_list[i].world_minlv)
		{
			return m_biaoche_attr_list[i].max_hp;
		}
	}

	return 0;
}

int GuildConfig::GetTotalBiaoCheGuildExp(int biaoche_lv)
{
	if (biaoche_lv <= 0 || biaoche_lv > GUILD_LEVEL_MAX) return 0;

	return m_biaoche_lvinfo_list[biaoche_lv].total_guild_exp;
}

int GuildConfig::GetRobBiaoCheGuildExp(int biaoche_lv)
{
	if (biaoche_lv <= 0 || biaoche_lv > GUILD_LEVEL_MAX) return 0;

	return m_biaoche_lvinfo_list[biaoche_lv].rob_guild_exp;
}

int GuildConfig::GetBiaoCheAddExp(int role_level)
{
	for (int i = m_biaoche_husong_exp_vec.size() - 1; i >= 0; i --)
	{
		if (role_level >= m_biaoche_husong_exp_vec[i].role_level)
		{
			return m_biaoche_husong_exp_vec[i].role_add_exp;
		}
	}

	return 0;
}

const BiaoCheLvInfo * GuildConfig::GetBiaoCheInfoCfg(int biaoche_lv)
{
	if (biaoche_lv <= 0 || biaoche_lv > GUILD_LEVEL_MAX) return nullptr;

	return &m_biaoche_lvinfo_list[biaoche_lv];
}

const GuildRumorCfg *GuildConfig::GetGuildRumorCfg(int rumor_type)
{
	auto it = m_guild_rumor_map.find(rumor_type);
	if (it != m_guild_rumor_map.end())
	{
		return &it->second;
	}

	return NULL;
}

int GuildConfig::GetGongziRate(int guild_post)
{
	if (guild_post <= GUILD_POST_INVALID || guild_post >= GUILD_POST_MAX)
	{
		return 0;
	}

	return m_gongzi_cfg_list[guild_post].rate;
}

const GuildGongziGiveCfg * GuildConfig::GetGongziGiveCfg(int boss_id)
{
	auto it = m_gongzi_give_map.find(boss_id);
	if (it != m_gongzi_give_map.end())
	{
		return &it->second;
	}

	return nullptr;
}

bool GuildConfig::CheckItemRare(ItemID item_id)
{
	if (rare_item_set.find(item_id) != rare_item_set.end())
	{
		return true;
	}

	return false;
}
