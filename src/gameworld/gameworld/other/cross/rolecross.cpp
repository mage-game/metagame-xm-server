#include "rolecross.h"
#include "servercommon/struct/roleallparam.hpp"
#include "obj/character/role.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "internalcomm.h"
#include "global/cross/crossactivity/crossrecordmanager.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "config/joinlimitconfig.hpp"
#include "config/crossteamfbconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "scene/scene.h"
#include "protocal/msgactivity.hpp"
#include "loginserver/engineadapter.h"
#include "servercommon/serverconfig/cross1v1commonconfig.hpp"
#include "servercommon/string/crossstr.h"
#include "other/route/mailroute.hpp"
#include "friend/teaminfo.hpp"
#include "servercommon/serverconfig/cross3v3config.hpp"
#include "global/team/team.hpp"
#include "servercommon/serverconfig/crosstuanzhanconfig.hpp"
#include "servercommon/serverconfig/crossbossconfig.hpp"
#include "servercommon/serverconfig/crossguildbattleconfig.hpp"
#include "servercommon/serverconfig/crossdarknightconfig.hpp"
#include "servercommon/serverconfig/crossfishingconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "world.h"
#include "gamecommon.h"
#include "gstr.h"
#include "servercommon/string/gameworldstr.h"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "other/shenshou/shenshou.hpp"
#include "servercommon/serverconfig/cross3v3config.hpp"
#include "servercommon/serverconfig/crossliekunfbconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "scene/speciallogic/worldspecial/specialcrossboss.hpp"
#include "crossserver/crossserver/protocal/msgcross.h"
#include "global/bossfamily/bossfamilyconfig.hpp"
#include "scene/sceneregister/sceneregister.hpp"
#include "other/vip/vip.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "protocal/msgfb.h"
#include "servercommon/serverconfig/crossmizangbossconfig.hpp"
#include "scene/speciallogic/worldspecial/specialcrossmizangboss.hpp"
#include "servercommon/serverconfig/crossyoumingbossconfig.hpp"
#include "scene/speciallogic/worldspecial/specialcrossyoumingboss.hpp"
#include "servercommon/serverconfig/crossbianjingzhidiconfig.h"



RoleCross::RoleCross() : m_role(NULL), m_last_cross_req_time(0), m_allow_to_cross_flag(0)
{
}

RoleCross::~RoleCross()
{

}

void RoleCross::InitParam(Role *role, const RoleCrossParam &param)
{
	m_role = role;
	m_param = param;

	if (m_param.cross_3v3_score_reuse_flag <= 0)
	{
		m_param.cross_3v3_score_reuse_flag = 1;

		m_param.cross_multiuser_challenge_score = CROSS_MULTIUSER_CHALLENGE_INIT_SCORE;
	}

	if (m_param.cross_original_uuid <= 0)
	{
		static const int PLAT_TYPE = LocalConfig::Instance().GetPlatType();
		m_param.cross_original_uuid = UniqueUserIDToLongLong(UniqueUserID(PLAT_TYPE, m_role->GetUserId()));
	}
	m_cross_original_unique_user_id = LongLongToUniqueUserID(m_param.cross_original_uuid);

	if (!m_role->IsInCross())
	{
		m_param.merge_server_id = LocalConfig::Instance().GetMergeServerId();
	}
}

void RoleCross::GetInitParam(RoleCrossParam *param)
{
	*param = m_param;
}

void RoleCross::ReCalcAttr(CharIntAttrs & base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		for (int i = 0; i < CROSS_ACTIVITY_1V1_RING_USE_COUNT; i++)
		{
			// 1v1戒指
			if (m_param.cross_1v1_season_reward_use[i] > 0 && m_param.cross_1v1_season_reward_use[i] <= CROSS_1V1_SEASON_MAX)
			{
				const Cross1V1SeasonRewardConfig *season_cfg = Cross1V1CommonConfig::Instance().GetSeasonCfgBySeasonGrade(m_param.cross_1v1_season_reward_use[i], m_param.cross_1v1_season_reward[m_param.cross_1v1_season_reward_use[i] - 1]);
				if (season_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += season_cfg->max_hp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += season_cfg->gong_ji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += season_cfg->fang_yu;
// 					m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += season_cfg->add_shang;
// 					m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += season_cfg->reduce_shang;
					m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, season_cfg->add_shang);
					m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, season_cfg->reduce_shang);
				}
			}
		}

		for (int i = 0; i < CROSS_ACTIVITY_3V3_RING_USE_COUNT; i++)
		{
			// 3v3令牌
			if (m_param.cross_3v3_season_reward_use[i] > 0 && m_param.cross_3v3_season_reward_use[i] <= CROSS_3V3_SEASON_MAX)
			{
				const Cross3v3SeasonRewardConfig *season_cfg = Cross3v3Config::Instance().GetSeasonCfgBySeasonGrade(m_param.cross_3v3_season_reward_use[i], m_param.cross_3v3_season_reward[m_param.cross_3v3_season_reward_use[i] - 1]);
				if (season_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += season_cfg->max_hp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += season_cfg->gong_ji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += season_cfg->fang_yu;
// 					m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += season_cfg->add_hurt;
// 					m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += season_cfg->reduce_hurt;
					m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, season_cfg->add_hurt);
					m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, season_cfg->reduce_hurt);
				}
			}	
		}
		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_ROLE_CROSS, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void RoleCross::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;
	{
		m_param.cross_xiuluo_tower_buy_realive_count = 0;
		m_param.cross_xiuluo_tower_add_gongji_per = 0;
		m_param.cross_xiuluo_tower_add_hp_per = 0;

		m_param.cross_xiuluotower_daily_drop_title_get_num = 0;
		m_param.cross_xiuluotower_daily_gold_box_title_get_num = 0;
		m_param.cross_xiuluotower_daily_drop_mount_get_num = 0;

		m_param.cross_guild_battle_daily_drop_wing_get_num = 0;

		this->SendCrossXiuluoTowerInfo();
	}

	// 1v1
	{
		this->Cross1v1GiveJoinTimesReward();
		m_param.cross_1v1_join_time_reward_flag = 0;
		m_param.cross_1v1_day_join_count = 0;
		m_param.cross_1v1_match_time = 0;
		m_param.cross_1v1_today_buy_times = 0;
		this->SendCross1V1SelfInfo();


		// 跨服1v1日志
		char tmp_buf[256] = { 0 }; memset(tmp_buf, 0, sizeof(tmp_buf));
		SNPRINTF(tmp_buf, sizeof(tmp_buf), "day_join_times:%d",
			(int)m_param.cross_1v1_day_join_count);

		ROLE_LOG_QUICK10(LOG_TYPE_CROSS_1V1, m_role, m_param.cross_1v1_score, m_role->GetCrossHonor(), NULL, NULL,
			0, 0, 0, 0);
	}

	// 跨服团战
	{
		// 全服奖励（目前叫参与奖励）每次活动期间只能领一次
		// m_param.cross_guildbattle_server_reward_flag = 0;
		// m_param.cross_guildbattle_guild_reward_flag = 0;
		m_param.cross_guildbattle_daily_reward_flag = 0;
	}

	{
		// 跨服牧场获取积分的次数每天重置
		m_param.cross_pasture_today_get_score_times = 0;
	}

	{
		//跨服boss
		m_param.today_gather_ordinary_crystal_num = 0;
		m_param.today_gather_treasure_crystal_num = 0;
		m_param.today_kill_boss_num = 0;
		this->SendCrossBossSelfInfo();
	}

	{
		// 跨服组队本
		m_param.cross_team_fb_day_count = 0;
		this->SendCrossTeamRBInfo();
	}

	{
		// 跨服帮派战
		memset(m_param.cross_guildbattle_task_reward_flag, 0, sizeof(m_param.cross_guildbattle_task_reward_flag));
		memset(m_param.cross_guildbattle_task_record, 0, sizeof(m_param.cross_guildbattle_task_record));
	}

	{
		// 跨服3v3, 补发每日参与奖励
		this->OnCrossMultiuserChallengeFetchJoinTimesReward(-1, false);
		m_param.cross_3v3_join_reward_fetch_flag = 0;
		m_param.cross_3v3_logout_flag = 0;
		this->SendCrossMultiuserChallengeSelfActivityInfo();
	}

	{
		//跨服秘藏boss
		m_param.cross_mizang_boss_today_gather_ordinary_crystal_num = 0;
		m_param.cross_mizang_boss_today_gather_treasure_crystal_num = 0;
		m_param.cross_mizang_boss_today_kill_boss_num = 0;
		this->SendCrossMizangBossSelfInfo();
	}

	{
		//跨服幽冥boss
		m_param.cross_youming_boss_today_gather_ordinary_crystal_num = 0;
		m_param.cross_youming_boss_today_gather_treasure_crystal_num = 0;
		m_param.cross_youming_boss_today_kill_boss_num = 0;
		this->SendCrossYoumingBossSelfInfo();
	}

	//ClockReturnDayItem* old_day = (ClockReturnDayItem *)&old_dayid;
	//ClockReturnDayItem* now_day = (ClockReturnDayItem *)&now_dayid;
	//if (NULL != old_day && NULL != now_day && (old_day->mon != now_day->mon))
	//{
	//	this->OnMonthChange();
	//}

	{
		m_param.cross_daily_consume_gold_num = 0;
	}
}

void RoleCross::OnWeekChange()
{

}

void RoleCross::OnCross1v1SeasonChange()
{
	// 发送上一赛季奖励
	WorldStatus2CommonDataParam &common_data_2 = WorldStatus2::Instance().GetCommonData2();
	if (m_param.cross_1v1_cur_join_season <= 0 || m_param.cross_1v1_cur_join_season_dayid == common_data_2.cross_1v1_cur_join_season_dayid)
		return;

	this->Cross1v1GiveScoreReward();

	m_param.cross_1v1_score = 0;
	m_param.cross_1v1_gongxun = 0;
	m_param.cross_1v1_fetch_score_reward_flag = 0;
	m_param.cross_1v1_total_join_times = 0;
	m_param.cross_1v1_total_win_times = 0;
	m_param.cross_1v1_dur_win_count = 0;
	m_param.cross_1v1_cur_join_season_dayid = common_data_2.cross_1v1_cur_join_season_dayid;

	this->SendCross1V1SelfInfo();
}

void RoleCross::OnCross3v3SeasonChange()
{
	// 发送上一赛季奖励
	WorldStatus2CommonDataParam &common_data_2 = WorldStatus2::Instance().GetCommonData2();
	if (m_param.cross_3v3_cur_join_season <= 0 || m_param.cross_3v3_cur_join_season_dayid == common_data_2.cross_3v3_cur_join_season_dayid)
		return;

	// 发放段位奖励
	this->Cross3v3GiveDivisionReward();
	// 补发功勋奖励
	this->OnCrossMultiuserChallengeFetchGongxunReward(-1, false);

	m_param.cross_3v3_cur_join_season = 0;

	m_param.cross_multiuser_challenge_score = 0;
	m_param.cross_multiuser_challenge_month_match_count = 0;
	m_param.cross_multiuser_challenge_win_match_count = 0;
	m_param.cross_multiuser_challenge_mvp_count = 0;

	m_param.cross_3v3_gongxun_reward_fetch_flag = 0;
	m_param.cross_3v3_gongxun_value = 0;

	m_param.cross_3v3_logout_flag = 0;
	m_param.cross_3v3_cur_join_season_dayid = common_data_2.cross_3v3_cur_join_season_dayid;

	this->SendCrossMultiuserChallengeSelfActivityInfo();
}

void RoleCross::OnLogin()
{
	// 同步基本在这里执行,OnUpdateCrossRecord里面基本执行不到
	this->OnSyncCross1V1Info();
	this->SyncCrossMultiuserChallengeRecordInfo();
	this->SyncCrossTuanzhanRecordInfo();
	this->SyncCrossPastureRecordInfo();
	this->SyncCrossBossRecordInfo();
	this->SyncCrossCommonRecordInfo();
	this->RegesiterCrossBossConcernInfo();
	this->SyncCrossMizangBossRecordInfo();
	this->RegesiterCrossMizangBossConcernInfo();
	this->SyncCrossYoumingBossRecordInfo();
	this->RegesiterCrossYoumingBossConcernInfo();

	// 赛季变更检查
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		this->OnCross1v1SeasonChange();
		this->OnCross3v3SeasonChange();
	}

	if (m_param.cross_consume_gold_num > 0 || m_param.cross_daily_consume_gold_num > 0)
	{
		EventHandler::Instance().OnCheckCrossConsumeGold(m_role, m_param.cross_consume_gold_num, m_param.cross_daily_consume_gold_num);
		m_param.cross_consume_gold_num = 0;
		m_param.cross_daily_consume_gold_num = 0;
	}
}

void RoleCross::OnLogout()
{
	this->CancelCrossMultiuserChallengeMatching(true);
	m_param.cross_multiuser_challenge_matching_state = -1; // 下线强制清除3v3匹配状态

	this->OnCrossTeamFBOption(CROSS_TEAM_FB_STATE_OPTION_QUIT, 0, 0, 1);
}

void RoleCross::Update(unsigned long interval, time_t now_second)
{

}

void RoleCross::OnRoleLeaveTeam()
{
	this->CancelCrossMultiuserChallengeMatching(true);
}

void RoleCross::OnStartCrossReq(int activity_type, bool is_force /* = false */, int param_1 /* = 0 */, int param_2 /* = 0 */, int param_3 /* = 0 */, short move_to_pos_x /* = 0 */, short move_to_pos_y /* = 0 */)
{
	if (m_role->IsInCross())
	{
		m_role->NoticeNum(errornum::EN_SPECIAL_SCENE_OPERATE_LIMIT);
		return;
	}

	if (!m_role->GetRoleStatusManager()->CanEnterFB())
	{
		return;
	}
	
	if (activity_type <= CROSS_ACTIVITY_TYPE_INVALID || activity_type >= CROSS_ACTIVITY_TYPE_RESERVED)
	{
		return;
	}

	//活动等级限制统一配置参与限制表
	//JoinLimitUnit limit_unit;
	//bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetActivityJoinLimit(activity_type, &limit_unit);

	//if (is_limit && m_role->GetLevel() < limit_unit.level)
	//{
	//	m_role->NoticeNum(errornum::EN_CROSS_ACTIVITY_LEVEL_LIMIT);
	//	return;
	//}

	//if (is_limit && m_role->GetLevel() > limit_unit.max_level)
	//{
	//	m_role->NoticeNum(errornum::EN_CROSS_ACTIVITY_LEVEL_MAX_LIMIT);
	//	return;
	//}

	//先临时这样处理，后面把3091做成真活动再把if去掉
	if (activity_type == CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE && ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE))
	{
		if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinActivity(3091, m_role))	// 3091是连服诛魔的假活动,实际没有这个活动
		{
			m_role->NoticeNum(errornum::EN_CROSS_ACTIVITY_OPEN_SERVER_DAY_LIMIT);
			return;
		}
	}
	else
	{
		//参与限制
		if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinActivity(activity_type, m_role))
		{
			m_role->NoticeNum(errornum::EN_CROSS_ACTIVITY_OPEN_SERVER_DAY_LIMIT);
			return;
		}
	}
	

	if (CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE == activity_type)
	{
 		const CrossGuildBattleOtherCfg &other = CrossGuildBattleConfig::Instance().GetOtherCfg();
// 		if (m_role->GetLevel() < other.level_limit)
// 		{
// 			return;
// 		}

		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
		if (opengame_day < other.openserver_limit)
		{
			return;
		}

		if (param_1 <= 0 || NULL == CrossGuildBattleConfig::Instance().GetBattleSceneCfg(param_1))
		{
			return;
		}

		Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
		if (NULL == guild)
		{
			m_role->NoticeNum(errornum::EN_CROSS_GUILDBATTLE_GUILD_LIMIT);
			return;
		}

		m_param.cross_guildbattle_scene_id = param_1;
		guild->GetTuanzhuangName(m_param.cross_guild_tuanzhang_name);
		this->SetCrossGuildBattleTuanzhangProf(guild->GetTuanzhangProf());
		this->SetCrossGuildBattleTuanzhangSex(guild->GetTuanzhangSex());
		this->SetCrossGuildBattleTuanzhangUuid(guild->GetTuanzhuangUuid());
	}
	else if (CROSS_ACTIVITY_TYPE_LIEKUN_FB == activity_type)
	{
		if (!this->OnEnterCrossLieKunFb((short)param_1, is_force))
		{
			return;
		}
	}
	else if (CROSS_ACTIVITY_TYPE_VIP_BOSS == activity_type)
	{
		if (!this->OnEnterCrossVipBoss(param_1, param_2, param_3))
		{
			return;
		}
	}
	else if (CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI == activity_type)
	{
		if (!this->OnEnterCrossBianJingZhiDi(is_force))
		{
			return;
		}
	}

//活动等级限制统一配置参与限制表
// 	else if (CROSS_ACTIVITY_TYPE_DARK_NIGHT == activity_type)
// 	{
// 		const CrossDarkNightOtherCfg& other_cfg = CrossDarkNightConfig::Instance().GetOtherCfg();
// 
// 		if (m_role->GetLevel() < other_cfg.open_level)
// 		{
// 			m_role->NoticeNum(errornum::EN_CROSS_BOSS_LEVEL_LIMIT);
// 			return;
// 		}
// 	}
// 
// 	else if (CROSS_ACTIVITY_TYPE_FISHING == activity_type)
// 	{
// 		const CrossFishingOtherConfig& other_cfg = CrossFishingConfig::Instance().GetOtherCfg();
// 		if (m_role->GetLevel() < other_cfg.open_level)
// 		{
// 			m_role->NoticeNum(errornum::EN_CROSS_BOSS_LEVEL_LIMIT);
// 			return;
// 		}
// 	}


	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_second < m_last_cross_req_time + 5)
	{
		return;
	}

	m_last_cross_req_time = now_second;

	if (!is_force)
	{
		if (CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE != activity_type && ActivityShadow::Instance().IsCrossActivityClose(activity_type))
		{
			return;
		}

		if (!m_role->GetRoleStatusManager()->CanStartCross())
		{
			return;
		}
	}

	if (CROSS_ACTIVITY_TYPE_CROSS_1V1 == activity_type)
	{
		EventHandler::Instance().OnEnterCross1v1(m_role);
		m_role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_DIAN_FENG_JING_JI, 1);
	}
	else if (CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE == activity_type)
	{
		// 六界战场开启状态下，进入活动才算参与
		if (ActivityShadow::Instance().IsCrossActivityOpen(activity_type))
		{
			Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
			if (NULL != guild)
			{
				guild->OnMemberEnterGuildBattle(m_role);
			}

			m_role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_CROSS_6_JIE, 1);
		}
	}
	else if (CROSS_ACTIVITY_TYPE_FISHING == activity_type)
	{
		m_role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_CROSS_FISHING, 1);
	}
	else if (CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER == activity_type)
	{
		m_role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_CROSS_XIU_LUO_TA, 1);
	}
	else if(CROSS_ACTIVITY_TYPE_HUSONG_SHUIJING == activity_type)
	{
		m_param.cross_husong_shuijing_enter_type = param_1;
	}

	// 检查设置进入跨服场景后是否直接传送到某个坐标
	{
		// 尝试清空
		this->ClearEnterCrossMovePos();

		if (move_to_pos_x > 0 && move_to_pos_y > 0)
		{
			m_param.enter_cross_move_to_pos_x = move_to_pos_x;
			m_param.enter_cross_move_to_pos_y = move_to_pos_y;
		}
	}

	static RoleAllParam all_param;
	all_param.Reset();

	m_role->GetCrossRoleParam(&all_param);

	static const int PLAT_TYPE = LocalConfig::Instance().GetPlatType();

	static crossgameprotocal::GameCrossStartCrossReq gcscr;
	gcscr.is_force = (is_force ? 1 : 0);
	gcscr.cross_activity_type = activity_type;
	gcscr.plat_type = PLAT_TYPE;
	m_role->GetPlatName(gcscr.pname);
	gcscr.uid = UidToInt(m_role->GetUserId());

	TLVSerializer outstream;
	outstream.Reset(gcscr.role_cross_param_data, sizeof(gcscr.role_cross_param_data));
	if (!all_param.Serialize(outstream, true)) 
	{
		return;
	}

	gcscr.role_cross_param_data_len = outstream.Size();

	int send_len = sizeof(gcscr) - sizeof(gcscr.role_cross_param_data) + gcscr.role_cross_param_data_len;
	InternalComm::Instance().SendToCross((const char *)&gcscr, send_len, activity_type);
}

void RoleCross::OnStartCrossAck(crossgameprotocal::CrossGameStartCrossAck *cgsca)
{
	if (0 != cgsca->result) return;

	this->CheckCreateCrossRecord();

	static Protocol::SCCrossEnterServer ecs;
	ecs.cross_activity_type = cgsca->cross_activity_type;
	STRNCPY(ecs.login_server_ip, cgsca->login_server_ip, sizeof(ecs.login_server_ip));
	ecs.login_server_port = cgsca->login_server_port;
	F_STRNCPY(ecs.pname, cgsca->pname, sizeof(ecs.pname));
	ecs.login_time = cgsca->login_time;

	UNSTD_STATIC_CHECK(sizeof(ecs.login_str) == sizeof(cgsca->login_str));
	memcpy(ecs.login_str, cgsca->login_str, sizeof(ecs.login_str));

	ecs.anti_wallow = cgsca->anti_wallow;
	ecs.reserved1 = 0;
	ecs.server = cgsca->server;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&ecs, sizeof(ecs));

	m_allow_to_cross_flag = 1;

	gamelog::g_log_debug.printf(LL_DEBUG, "StartCrossAck:: ip:%s port:%d  pname:%s server:%d", 
		ecs.login_server_ip, ecs.login_server_port, ecs.pname, ecs.server);

	ROLE_LOG_QUICK6(LOG_TYPE_CROSS_JOIN, m_role, m_role->GetLevel(), cgsca->cross_activity_type, "Join", NULL);
}

void RoleCross::CheckCreateCrossRecord()
{
	CrossRecordManager::Instance().GetCrossRecord(m_role, true);
}


void RoleCross::CrossNameToOriginName(GameName name)
{
	int fix_pos = -1;
	for (int i = 0; i < static_cast<int>(sizeof(GameName)); i++)
	{
		if (0 == name[i])
		{
			break;
		}
		else if ('_' == name[i])
		{
			fix_pos = i;
		}
	}

	if (fix_pos >= 0 && fix_pos < static_cast<int>(sizeof(GameName)))
	{
		name[fix_pos] = 0;
	}
}

void RoleCross::CrossPlatNameToOrigin(PlatName name)
{
	// fuck|dev_name_2_2046 -> dev_name_3

	std::string name_to_fix = name;

	auto beg_pos = name_to_fix.find("|");
	auto end_pos = name_to_fix.rfind("_" + std::to_string(CROSS_USER_DB_INDEX));

	if (beg_pos + 1 <= name_to_fix.size())
	{
		auto real_name = name_to_fix.substr(beg_pos + 1, end_pos - beg_pos - 1);
		STRNCPY(name, real_name.c_str(), sizeof(PlatName));
	}
}

bool RoleCross::IsFromSameGameServer(Role *role)
{
	if (nullptr == role)
	{
		return false;
	}

	if (this->GetOriginMergeServerID() == role->GetRoleCross()->GetOriginMergeServerID() &&
		(this->GetOriginPlatType() == role->GetRoleCross()->GetOriginPlatType()))
	{
		return true;
	}

	return false;
}

void RoleCross::OnCrossXiuluoTowerBuyBuff(bool is_buy_realive)
{
	return;
// 	if (ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER))	
// 	{
// 		return;
// 	}
// 
// 	if (is_buy_realive)
// 	{
// 		if (m_param.cross_xiuluo_tower_buy_realive_count >= 5)
// 		{
// 			m_role->NoticeNum(errornum::EN_CROSS_XIULUOTA_GUWU_LIMIT_TIMES);
// 			return;
// 		}
// 
// 		if (!m_role->GetKnapsack()->GetMoney()->UseGold(30, "CrossXiuluoTowerBuyRealive"))
// 		{
// 			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 			return;
// 		}
// 
// 		++ m_param.cross_xiuluo_tower_buy_realive_count;
// 		m_role->NoticeNum(noticenum::NT_CROSS_BUY_REALIVE_GUWU_SUCC);
// 
// 		gamelog::g_log_cross.printf(LL_INFO, "CrossXiuluota BuyRealiveCount user %d %s, realive_count %d",
// 			m_role->GetUID(), m_role->GetName(), m_param.cross_xiuluo_tower_buy_realive_count);
// 	}
// 	else
// 	{
// 		static const int GOLD_ADD_ATTR_PER_MAX = 50;
// 
// 		if (m_param.cross_xiuluo_tower_add_gongji_per >= GOLD_ADD_ATTR_PER_MAX && m_param.cross_xiuluo_tower_add_hp_per >= GOLD_ADD_ATTR_PER_MAX)
// 		{
// 			m_role->NoticeNum(errornum::EN_CROSS_XIULUOTA_BUY_REALIVE_LIMIT_TIMES);
// 			return;
// 		}
// 
// 		if (!m_role->GetKnapsack()->GetMoney()->UseGold(20, "CrossXiuluoTowerBuyBuff"))
// 		{
// 			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 			return;
// 		}
// 
// 		m_param.cross_xiuluo_tower_add_gongji_per += 5;
// 		m_param.cross_xiuluo_tower_add_hp_per += 5;
// 
// 		if (m_param.cross_xiuluo_tower_add_gongji_per > GOLD_ADD_ATTR_PER_MAX)
// 		{
// 			m_param.cross_xiuluo_tower_add_gongji_per = GOLD_ADD_ATTR_PER_MAX;
// 		}
// 
// 		if (m_param.cross_xiuluo_tower_add_hp_per > GOLD_ADD_ATTR_PER_MAX)
// 		{
// 			m_param.cross_xiuluo_tower_add_hp_per = GOLD_ADD_ATTR_PER_MAX;
// 		}
// 
// 		m_role->NoticeNum(noticenum::NT_CROSS_XIULUOTA_GUWU_SUCC);
// 
// 		gamelog::g_log_cross.printf(LL_INFO, "CrossXiuluota BuyBuff user %d %s, add_hp %d, add_gongji %d",
// 			m_role->GetUID(), m_role->GetName(), m_param.cross_xiuluo_tower_add_hp_per, m_param.cross_xiuluo_tower_add_gongji_per);
// 	}
// 
// 	this->SendCrossXiuluoTowerInfo();
}

void RoleCross::SendCrossXiuluoTowerInfo()
{
	static Protocol::SCCrossXiuluoTowerInfo cxti;
	cxti.buy_realive_count = m_param.cross_xiuluo_tower_buy_realive_count;
	cxti.add_gongji_per = m_param.cross_xiuluo_tower_add_gongji_per;
	cxti.add_hp_per = m_param.cross_xiuluo_tower_add_hp_per;
	cxti.uuid = UniqueUserIDToLongLong(m_role->GetUniqueUserID());

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cxti, sizeof(cxti));
}

void RoleCross::OnCrossMatch1v1Req()
{
	if (m_role->IsInCross())
	{
		return;
	}

	if (NULL != m_role->GetScene() && !m_role->GetScene()->InStaticScene())
	{
		m_role->NoticeNum(errornum::EN_FB_ENTER_CAN_NOT_IN_FB);
		return;
	}

	const Cross1V1OtherConfig& other_cfg = Cross1V1CommonConfig::Instance().GetOtherCfg();
	if (m_param.cross_1v1_day_join_count >= other_cfg.free_times + m_param.cross_1v1_today_buy_times)
	{
		gstr::SendError(m_role, "cross_1v1_times_limit");
		return;
	}

	if (!ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_CROSS_1V1))
	{
		m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
		return;
	}

	if (m_role->GetLevel() < other_cfg.limit_join_level)
	{
		m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_FB_LEVEL_LIMIT);
		return;
	}

	if (m_role->GetCapability()->GetTotalCapability() < CROSS_ACTIVITY_1V1_ROLE_CAPABILITY)
	{
		m_role->NoticeNum(errornum::EN_CROSS_1V1_CAPABILITY_LIMIT);
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (NULL != m_role->GetScene() && Scene::SCENE_TYPE_CROSS_1V1 != m_role->GetScene()->GetSceneType())
	{
		if (!m_role->GetRoleStatusManager()->CanEnterFB(false, false))
		{
			m_role->NoticeNum(errornum::EN_1V1_CANNOT_MATCH_STATUS);
			return;
		}
	}

	m_param.cross_1v1_match_time = (unsigned int)(EngineAdapter::Instance().Time());

	WorldStatus2CommonDataParam &common_data_2 = WorldStatus2::Instance().GetCommonData2();
	m_param.cross_1v1_cur_join_season = common_data_2.cross1v1_season_count;
	m_param.cross_1v1_cur_join_season_dayid = common_data_2.cross_1v1_cur_join_season_dayid;

	crossgameprotocal::GameCrossActivity1V1MatchReq gcamr;
	gcamr.uuid = UniqueUserIDToLongLong(m_role->GetUniqueUserID());
	gcamr.prof = m_role->GetProf();
	gcamr.sex = m_role->GetSex();
	gcamr.camp = m_role->GetCamp();
	gcamr.level = m_role->GetLevel();
	gcamr.jingjie = m_role->GetJingJieLevel();
	gcamr.score = m_param.cross_1v1_score;
	gcamr.day_join_count = m_param.cross_1v1_day_join_count;
	gcamr.capability = m_role->GetCapability()->GetTotalCapability();
	gcamr.dur_win_count = m_param.cross_1v1_dur_win_count;
	gcamr.total_join_times = m_param.cross_1v1_total_join_times;
	gcamr.total_win_times = m_param.cross_1v1_total_win_times;
	gcamr.cross_1v1_gongxun = m_param.cross_1v1_gongxun;

	m_role->GetName(gcamr.name);
	gcamr.appearance = m_role->GetAppearance();

	InternalComm::Instance().SendToCross((const char*)&gcamr, sizeof(gcamr), CROSS_ACTIVITY_TYPE_CROSS_1V1);

	EventHandler::Instance().OnJoinActivity(m_role, CROSS_ACTIVITY_TYPE_CROSS_1V1);

	this->CheckCreateCrossRecord();
}

void RoleCross::OnSyncCross1V1Info()
{
	CrossRecord *cross_record = CrossRecordManager::Instance().GetCrossRecord(m_role->GetUserId());
	if (NULL == cross_record || cross_record->user_record_data.has_sync_cross_1v1_record_to_role <= 0)
	{
		return;
	}

	unsigned int activity_begin_timestamp = ActivityShadow::Instance().GetCrossActivityBeginTime(CROSS_ACTIVITY_TYPE_CROSS_1V1);
	if (m_param.cross_1v1_curr_activity_begin_timestap != activity_begin_timestamp)
	{
		m_param.cross_1v1_curr_activity_begin_timestap = activity_begin_timestamp;
		m_param.cross_1v1_curr_activity_add_score = 0;
	}

	int add_score = cross_record->user_record_data.cross_score_1v1 - m_param.cross_1v1_score;
	if (add_score < 0) add_score = 0;

	m_param.cross_1v1_curr_activity_add_score += add_score;
	this->SetCross1v1Score(cross_record->user_record_data.cross_score_1v1, 1);
	m_param.cross_1v1_day_join_count = cross_record->user_record_data.cross_1v1_day_join_count;
	m_param.cross_1v1_dur_win_count = cross_record->user_record_data.cross_1v1_dur_win_count;

	if (m_param.cross_1v1_dur_win_count > m_param.cross_1v1_dur_win_max_count)
	{
		m_param.cross_1v1_dur_win_max_count = m_param.cross_1v1_dur_win_count;

		EventHandler::Instance().OnCross1v1DurWin(m_role, m_param.cross_1v1_dur_win_max_count);
	}

	cross_record->user_record_data.has_sync_cross_1v1_record_to_role = 0;
	m_param.cross_1v1_total_join_times = cross_record->user_record_data.cross_1v1_total_join_times;
	m_param.cross_1v1_total_win_times = cross_record->user_record_data.cross_1v1_total_wim_times;
	m_param.cross_1v1_gongxun = cross_record->user_record_data.cross_1v1_gongxun;

	m_param.cross_1v1_match_time = 0;

	this->SendCross1V1SelfInfo();

	char tmp_buf[256] = { 0 }; memset(tmp_buf, 0, sizeof(tmp_buf));
	SNPRINTF(tmp_buf, sizeof(tmp_buf), "day_join_times:%d",
		m_param.cross_1v1_day_join_count);

	ROLE_LOG_QUICK10(LOG_TYPE_CROSS_1V1, m_role, m_param.cross_1v1_score, m_role->GetCrossHonor(), NULL, NULL,
		0, 0, 0, 0);

	CrossRecordManager::Instance().SaveCrossRecord(cross_record, structcommon::CS_UPDATE);
}

void RoleCross::OnSyncCross1V1Info(crossgameprotocal::CrossGame1V1Info *sci)
{
	this->SetCross1v1Score(sci->cross_score_1v1, 2);
	m_param.cross_1v1_day_join_count = sci->cross_day_join_1v1_count;
	m_param.cross_1v1_match_time = 0;

	this->SendCross1V1SelfInfo();


	char tmp_buf[256] = { 0 }; memset(tmp_buf, 0, sizeof(tmp_buf));
	SNPRINTF(tmp_buf, sizeof(tmp_buf), "day_join_count:%d",
		(int)m_param.cross_1v1_day_join_count);

	ROLE_LOG_QUICK10(LOG_TYPE_CROSS_1V1, m_role, m_param.cross_1v1_score, m_role->GetCrossHonor(), NULL, NULL,
		0, 0, 0, 0);
}

void RoleCross::SendCross1V1SelfInfo()
{
	WorldStatus2CommonDataParam &common_data_2 = WorldStatus2::Instance().GetCommonData2();
	static Protocol::SCCrossActivity1V1SelfInfo casi;
	casi.cross_score_1v1 = m_param.cross_1v1_score;
	casi.cross_day_join_1v1_count = m_param.cross_1v1_day_join_count;
	casi.cross_1v1_join_time_reward_flag = m_param.cross_1v1_join_time_reward_flag;
	casi.cross_1v1_fetch_score_reward_flag = m_param.cross_1v1_fetch_score_reward_flag;
	casi.today_buy_times = m_param.cross_1v1_today_buy_times;
	casi.cur_season = common_data_2.cross1v1_season_count;
	casi.cross_1v1_curr_activity_add_score = m_param.cross_1v1_curr_activity_add_score;
	casi.cross_1v1_total_join_times = m_param.cross_1v1_total_join_times;
	casi.cross_1v1_total_win_times = m_param.cross_1v1_total_win_times;
	casi.cross_1v1_dur_win_times = m_param.cross_1v1_dur_win_count;
	casi.cross_1v1_gongxun = m_param.cross_1v1_gongxun;
	memcpy(casi.cross_1v1_have_ring, m_param.cross_1v1_season_reward, sizeof(m_param.cross_1v1_season_reward));
	memcpy(casi.cross_1v1_use_ring, m_param.cross_1v1_season_reward_use, sizeof(m_param.cross_1v1_season_reward_use));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&casi, sizeof(casi));
}

bool RoleCross::IsInCross1v1Matching()
{
	if (ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_CROSS_1V1))
	{
		unsigned int last_match_time = ActivityShadow::Instance().GetCross1v1LastMatchTime();
		unsigned int next_match_time = ActivityShadow::Instance().GetCross1v1NextMatchTime();
		unsigned int fight_start_time = next_match_time + Cross1V1CommonConfig::Instance().GetFightWaitTime();

		if (m_param.cross_1v1_match_time > 0)
		{
			if (m_param.cross_1v1_match_time > last_match_time && m_param.cross_1v1_match_time < fight_start_time)
			{
				return false;
			}
		}
	}

	return true;
}

void RoleCross::OnCross1v1FecthRewardReq(short fetch_type, int seq)
{
	if (fetch_type <= 0 || fetch_type >= Protocol::CROSS_1V1_FETCH_REWARD_TYPE_MAX)
	{
		return;
	}

	switch (fetch_type)
	{
	case Protocol::CROSS_1V1_FETCH_REWARD_TYPE_JOIN_TIMES:
	{
		this->Cross1v1FetchJoinTimesReward(seq);
	}
	break;

	case Protocol::CROSS_1V1_FETCH_REWARD_TYPE_SCORE:
	{
		this->Cross1v1FetchScoreReward(seq);
	}
	break;

	default:
		break;
	}
}

void RoleCross::Cross1v1FetchJoinTimesReward(int seq)
{
	if (seq < 0 || seq >= CROSS_ACTIVITY_1V1_JOIN_TIME_CFG_MAX_COUNT)
	{
		return;
	}

	if ((m_param.cross_1v1_join_time_reward_flag & (1 << seq)) != 0)
	{
		return;
	}

	const Cross1V1JoinTimesRewardConfig *reward_cfg = Cross1V1CommonConfig::Instance().GetJoinTimeRewardCfgBySeq(seq);
	if (NULL == reward_cfg)
	{
		return;
	}

	if (m_param.cross_1v1_day_join_count < reward_cfg->jion_times)
	{
		return;
	}

	if (m_role->GetLevel() <= 370)
	{
		if (!m_role->GetKnapsack()->PutListOrMail(reward_cfg->reward_item_l_list, PUT_REASON_CROSS_1V1))
		{
			return;
		}
	}
	else
	{
		if (!m_role->GetKnapsack()->PutListOrMail(reward_cfg->reward_item_h_list, PUT_REASON_CROSS_1V1))
		{
			return;
		}
	}

	m_param.cross_1v1_join_time_reward_flag |= (1 << seq);

	this->SendCross1V1SelfInfo();
}

void RoleCross::Cross1v1GiveJoinTimesReward()
{
	// 发送参与次数奖励
	for (int i = CROSS_ACTIVITY_1V1_JOIN_TIME_CFG_MAX_COUNT - 1; i >= 0; --i)
	{
		if ((m_param.cross_1v1_join_time_reward_flag & (1 << i)) != 0)
		{
			break;
		}

		const Cross1V1JoinTimesRewardConfig *reward_cfg = Cross1V1CommonConfig::Instance().GetJoinTimeRewardCfgBySeq(i);
		if (NULL == reward_cfg)
		{
			continue;
		}

		if (m_param.cross_1v1_day_join_count < reward_cfg->jion_times)
		{
			continue;
		}

		static MailContentParam content_param; content_param.Reset();

		if (m_role->GetLevel() <= 370)
		{
			for (int i = 0; i < reward_cfg->reward_item_l_count && i < MAX_ATTACHMENT_ITEM_NUM; ++i)
			{
				const ItemBase *item_base = ITEMPOOL->GetItem(reward_cfg->reward_item_l_list[i].item_id);
				if (NULL == item_base)
				{
					continue;
				}
				content_param.item_list[i].item_id = reward_cfg->reward_item_l_list[i].item_id;
				content_param.item_list[i].num = reward_cfg->reward_item_l_list[i].num;
				content_param.item_list[i].is_bind = reward_cfg->reward_item_l_list[i].is_bind;
				content_param.item_list[i].invalid_time = item_base->CalInvalidTime();
			}
		}
		else
		{
			for (int i = 0; i < reward_cfg->reward_item_h_count && i < MAX_ATTACHMENT_ITEM_NUM; ++i)
			{
				const ItemBase *item_base = ITEMPOOL->GetItem(reward_cfg->reward_item_h_list[i].item_id);
				if (NULL == item_base)
				{
					continue;
				}
				content_param.item_list[i].item_id = reward_cfg->reward_item_h_list[i].item_id;
				content_param.item_list[i].num = reward_cfg->reward_item_h_list[i].num;
				content_param.item_list[i].is_bind = reward_cfg->reward_item_h_list[i].is_bind;
				content_param.item_list[i].invalid_time = item_base->CalInvalidTime();
			}
		}

		int length = gstr::MailContent(content_param.contenttxt, sizeof(content_param.contenttxt), "cross_1v1_join_times_reward_subject", reward_cfg->jion_times);
		if (length > 0)
		{
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, content_param);
		}

		m_param.cross_1v1_join_time_reward_flag |= (1 << i);
	}
}

void RoleCross::Cross1v1FetchScoreReward(int seq)
{
	if (seq < 0 || seq >= CROSS_ACTIVITY_1V1_SCORE_CFG_MAX_COUNT)
	{
		return;
	}

	if ((m_param.cross_1v1_fetch_score_reward_flag & (1 << seq)) != 0)
	{
		return;
	}

	const Cross1V1ScoreRewardConfig *reward_cfg = Cross1V1CommonConfig::Instance().GetScoreRewardCfgBySeq(seq);
	if (NULL == reward_cfg)
	{
		return;
	}

	if (m_param.cross_1v1_gongxun < reward_cfg->need_score)
	{
		return;
	}
	m_role->GetKnapsack()->PutListOrMail(reward_cfg->reward_item_list, PUT_REASON_1V1_GONGXUN_REWARD);

	m_param.cross_1v1_fetch_score_reward_flag |= (1 << seq);

	this->SendCross1V1SelfInfo();
}

void RoleCross::Cross1v1GiveScoreReward()
{
	WorldStatus2CommonDataParam &common_data_2 = WorldStatus2::Instance().GetCommonData2();

	gamelog::g_log_cross.printf(LL_INFO, "RoleCross::Cross1v1GiveScoreReward: user(%d, %s), season:%d, score:%d, join_times:%d, win_times:%d, m_dayid:%d, s_dayid:%d", 
		m_role->GetUID(), m_role->GetName(), m_param.cross_1v1_cur_join_season, m_param.cross_1v1_score, m_param.cross_1v1_total_join_times, m_param.cross_1v1_total_win_times,
		m_param.cross_1v1_cur_join_season_dayid, common_data_2.cross_1v1_cur_join_season_dayid);

	if (m_param.cross_1v1_total_join_times >= Cross1V1CommonConfig::Instance().GetOtherCfg().atleast_join_times)
	{
		int rank_min_score = 0;  // 获取玩家当前参加的赛季的最低排行积分
		int last_join_season = m_param.cross_1v1_cur_join_season % CROSS_1V1_SEASON_MAX;
		if (0 <= last_join_season && last_join_season < CROSS_1V1_SEASON_MAX && m_param.cross_1v1_cur_join_season > 0)
		{
			rank_min_score = common_data_2.cross1v1_rank_min_score[last_join_season];
		}
			
		if (rank_min_score == 0 || (rank_min_score > 0 && m_param.cross_1v1_score < rank_min_score))
		{
			const Cross1V1GradeScoreConfig *grade_score_cfg = Cross1V1CommonConfig::Instance().GetGradeScoreCfgByScore(m_param.cross_1v1_score);
			if (NULL != grade_score_cfg)
			{

				static MailContentParam contentparam; contentparam.Reset();
				int reward_count = 0;
				for (int i = 0; i < grade_score_cfg->reward_count && reward_count < MAX_ATTACHMENT_ITEM_NUM; i++)
				{
					const ItemConfigData &item_cfg = grade_score_cfg->reward_item_list[i];
					const ItemBase *item_base = ITEMPOOL->GetItem(item_cfg.item_id);
					if (NULL == item_base) return;

					contentparam.item_list[reward_count].item_id = item_cfg.item_id;
					contentparam.item_list[reward_count].num = item_cfg.num;
					contentparam.item_list[reward_count].is_bind = item_cfg.is_bind;
					contentparam.item_list[reward_count].invalid_time = item_base->CalInvalidTime();
					++reward_count;
				}
				contentparam.gold_bind = grade_score_cfg->bind_gold;
				contentparam.coin = grade_score_cfg->coin;

				int length = gstr::MailContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "cross_1v1_season_score_reward_content");
				if (length > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

					gamelog::g_log_cross.printf(LL_INFO, "Cross1v1GiveScoreReward : user(%d, %s), score:%d, reward_item_num:%d, coin:%d, gold_bind:%d",
						m_role->GetUID(), m_role->GetName(), m_param.cross_1v1_score, reward_count, grade_score_cfg->coin, grade_score_cfg->bind_gold);
				}
			}
			else
			{
				gamelog::g_log_cross.printf(LL_INFO, "Cross1v1GiveScoreReward Fail: user(%d, %s), score:%d",
					m_role->GetUID(), m_role->GetName(), m_param.cross_1v1_score);
			}

			// 戒指
			if (m_param.cross_1v1_cur_join_season <= 0)
			{
				gamelog::g_log_cross.printf(LL_INFO, "Cross1v1GiveScoreReward[Ring] Fail: user(%d, %s), season:%d, score:%d",
					m_role->GetUID(), m_role->GetName(), m_param.cross_1v1_cur_join_season, m_param.cross_1v1_score);

				return;
			}
			int grade = Cross1V1CommonConfig::Instance().GetGradeByScore(m_param.cross_1v1_score);
			const int MAX_OPEN_SEASON = Cross1V1CommonConfig::Instance().GetMaxSeason();
			const Cross1V1SeasonRewardConfig *season_cfg = NULL;
			if (common_data_2.cross1v1_season_count <= MAX_OPEN_SEASON)
			{
				season_cfg = Cross1V1CommonConfig::Instance().GetSeasonCfgBySeasonGrade(m_param.cross_1v1_cur_join_season, grade);
			}
			else
			{
				season_cfg = Cross1V1CommonConfig::Instance().GetAfterSeasonCfgBySeasonGrade(m_param.cross_1v1_cur_join_season, grade);
			}

			if (season_cfg != NULL)
			{
				static MailContentParam contentparam; contentparam.Reset();
				const ItemConfigData &item_cfg = season_cfg->reward_item;
				const ItemBase *item_base = ITEMPOOL->GetItem(item_cfg.item_id);
				if (NULL == item_base) return;

				contentparam.item_list[0].item_id = item_cfg.item_id;
				contentparam.item_list[0].num = item_cfg.num;
				contentparam.item_list[0].is_bind = item_cfg.is_bind;
				contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

				int length = gstr::MailContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "cross_1v1_season_ring_reward_content");
				if (length > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

					gamelog::g_log_cross.printf(LL_INFO, "Cross1v1GiveScoreReward[Ring] : user(%d, %s), season:%d, score:%d, grade:%d, reward_item_id:%d, reward_item_num:%d",
						m_role->GetUID(), m_role->GetName(), m_param.cross_1v1_cur_join_season, m_param.cross_1v1_score, grade, item_cfg.item_id, item_cfg.num);
				}
			}
			else
			{
				gamelog::g_log_cross.printf(LL_INFO, "Cross1v1GiveScoreReward[Ring] Fail: user(%d, %s), season:%d, score:%d, grade:%d",
					m_role->GetUID(), m_role->GetName(), m_param.cross_1v1_cur_join_season, m_param.cross_1v1_score, grade);
			}
		}
	}

	// 清掉参加赛季标志
	m_param.cross_1v1_cur_join_season = 0;

	// 功勋奖励
	for (int i = 0; i < CROSS_ACTIVITY_1V1_SCORE_CFG_MAX_COUNT; i++)
	{
		if ((m_param.cross_1v1_fetch_score_reward_flag & (1 << i)) != 0)
		{
			continue;
		}

		const Cross1V1ScoreRewardConfig *reward_cfg = Cross1V1CommonConfig::Instance().GetScoreRewardCfgBySeq(i);
		if (NULL == reward_cfg)
		{
			continue;
		}

		if (m_param.cross_1v1_gongxun < reward_cfg->need_score)
		{
			break;
		}
		gamecommon::SendMultiMailToRole(m_role->GetUserId(), reward_cfg->reward_item_list, reward_cfg->reward_count, NULL, false, "cross_1v1_season_gongxun_reward_content");
		m_param.cross_1v1_fetch_score_reward_flag |= (1 << i);
	}
}

void RoleCross::OnCross1v1RingOper(int oper_type, int seq)
{
	const Cross1V1SeasonRewardConfig * season_cfg = Cross1V1CommonConfig::Instance().GetSeasonCfgBySeq(seq);
	if (NULL == season_cfg)
	{
		return;
	}

	if (oper_type == CROSS_RING_CARD_OPER_WEAR)
	{
		int use_count = 0;
		for (int i = 0; i < CROSS_ACTIVITY_1V1_RING_USE_COUNT; ++i)
		{
			if (m_param.cross_1v1_season_reward_use[i] > 0)
			{
				++use_count;
			}

			if (m_param.cross_1v1_season_reward_use[i] == season_cfg->season)
			{
				return;
			}
		}

		if (use_count >= CROSS_ACTIVITY_1V1_RING_USE_COUNT)
		{
			gstr::SendError(m_role, "cross_1v1_wear_ring_limit");
			return;
		}

		//int index = m_param.cross_1v1_season_reward_use[0] <= 0 ? 0 : 1;
		for (int i = 0; i < CROSS_ACTIVITY_1V1_RING_USE_COUNT; ++i)
		{
			if (m_param.cross_1v1_season_reward_use[i] <= 0)
			{
				if (m_param.cross_1v1_season_reward[season_cfg->season - 1] == season_cfg->grade)
				{
					m_param.cross_1v1_season_reward_use[i] = season_cfg->season;
				}
				break;
			}
		}
	}

	if (oper_type == CROSS_RING_CARD_OPER_OFF)
	{
		for (int i = 0; i < CROSS_ACTIVITY_1V1_RING_USE_COUNT; ++i)
		{
			if (m_param.cross_1v1_season_reward_use[i] == season_cfg->season)
			{
				m_param.cross_1v1_season_reward_use[i] = 0;
			}
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ROLE_CROSS);

	this->SendCross1V1SelfInfo();
}

void RoleCross::OnCross1v1GM(int type, int score)
{
	switch (type)
	{
	case CROSS1V1_GM_OPER_TYPE_SCORE:
	{
		this->SetCross1v1Score(score, 3);
	}
	break;
	case CROSS1V1_GM_OPER_TYPE_GONGXUN:
	{
		m_param.cross_1v1_gongxun = score;
	}
	break;
	case CROSS1V1_GM_OPER_TYPE_JOIN_TIMES:
	{
		m_param.cross_1v1_total_join_times = score;
	}
	break;
	}

	this->SendCross1V1SelfInfo();
}

void RoleCross::OnCross1v1MatchResultReq(int type)
{
	crossgameprotocal::CrossGameCross1V1MatchResult cgcmr;
	cgcmr.uuid = UniqueUserIDToLongLong(m_role->GetUniqueUserID());
	cgcmr.req_type = type;
	InternalComm::Instance().SendToCross((const char *)&cgcmr, sizeof(cgcmr), CROSS_ACTIVITY_TYPE_CROSS_1V1);
}

bool RoleCross::OnActivePVPRewardItem(int item_type, int item_id, int item_seq)
{
	switch (item_type)
	{
	case CROSS_ACTIVITY_PVP_REWARD_TYPE_RING:
	{
		const Cross1V1SeasonRewardConfig *season_cfg = Cross1V1CommonConfig::Instance().GetSeasonCfgBySeq(item_seq);
		if (NULL == season_cfg)
		{
			return false;
		}

		if (season_cfg->reward_item.item_id != item_id)
		{
			return false;
		}

		m_param.cross_1v1_season_reward[season_cfg->season - 1] = season_cfg->grade;

		this->SendCross1V1SelfInfo();
		return true;
	}
	break;

	case CROSS_ACTIVITY_PVP_REWARD_TYPE_CARD:
	{
		const Cross3v3SeasonRewardConfig *season_cfg = Cross3v3Config::Instance().GetSeasonCfgBySeq(item_seq);
		if (NULL == season_cfg)
		{
			return false;
		}

		if (season_cfg->reward_item.item_id != item_id)
		{
			return false;
		}

		m_param.cross_3v3_season_reward[season_cfg->season - 1] = season_cfg->grade;

		this->SendCrossMultiuserChallengeSelfActivityInfo();
		return true;
	}
	break;


	default:
		return false;
	}
}

void RoleCross::Cross1v1GiveJoinReward()
{
	if (m_param.cross_1v1_score > 0)
	{
		const Cross1V1OtherConfig& other_cfg = Cross1V1CommonConfig::Instance().GetOtherCfg();
		static MailContentParam content_param; content_param.Reset();
		const ItemBase *item_base = ITEMPOOL->GetItem(other_cfg.join_item.item_id);
		if (NULL == item_base) return;

		content_param.item_list[0].item_id = other_cfg.join_item.item_id;
		content_param.item_list[0].num = other_cfg.join_item.num;
		content_param.item_list[0].is_bind = other_cfg.join_item.is_bind;
		content_param.item_list[0].invalid_time = item_base->CalInvalidTime();

		int length = SNPRINTF(content_param.contenttxt, sizeof(content_param.contenttxt), gamestring::g_cross_1v1_join_reward_content);

		if (length > 0)
		{
			MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, content_param);
		}
	}
}

void RoleCross::Cross1v1BuyTime()
{
	const Cross1V1OtherConfig& other_cfg = Cross1V1CommonConfig::Instance().GetOtherCfg();
	if (m_param.cross_1v1_today_buy_times >= other_cfg.max_buy_times)
	{
		m_role->NoticeNum(errornum::EN_1V1_DAILYTIMES_BUY_TIMES_MAX);
		return;
	}

	if (other_cfg.buy_time_cost > 0 && !m_role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.buy_time_cost, "Cross1v1BuyTime"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_param.cross_1v1_today_buy_times++;

	this->SendCross1V1SelfInfo();
}

void RoleCross::SetCross1v1Score(int score, int reason)
{
	if (m_param.cross_1v1_score > 0 && score <= 0)
	{
		ROLE_LOG_QUICK10(LOG_TYPE_CROSS_1V1, m_role, m_param.cross_1v1_score, m_role->GetCrossHonor(), NULL, NULL,
			reason, 0, 0, 0);
	}
	else
	{
		m_param.cross_1v1_score = score;
		if (reason == 3)
		{
			m_param.cross_1v1_total_win_times += 5;
			this->SendCross1V1SelfInfo();
		}
	}
}

int RoleCross::GetCross1v1Score()
{
	return m_param.cross_1v1_score;
}

void RoleCross::OnClearCross1v1Matching(int cross_type)
{
	if (!ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_CROSS_1V1))
	{
		return;
	}

	// 通知客户端取消匹配
	static Protocol::SCCross1v1MatchAck cam;
	cam.result = 0;
	cam.reserve = 0;
	cam.match_end_left_time = 0;

	if (cross_type <= 0)
	{
		// 全部跨服断开，比如重读配置
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cam, sizeof(cam));
		
		// TODO...
		m_param.cross_1v1_match_time = 0;
	}
	else if (cross_type > 0 && CrossConfig::Instance().CheckCrossServerHasCrossActivity(CROSS_ACTIVITY_TYPE_CROSS_1V1, cross_type))
	{
		// 单个跨服断开
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cam, sizeof(cam));

		// TODO...
		m_param.cross_1v1_match_time = 0;
	}

	// log
	gamelog::g_log_cross.printf(LL_WARNING, "Clear role(%d, %s) cross1v1 match on crossserver[cross_type:%d] disconnect.", 
		m_role->GetUID(), m_role->GetName(), cross_type);
}

void RoleCross::OnSyncCrossMultiuserChallengeMatchingState(crossgameprotocal::CrossGameMultiuserChallengeMatchingStateNotify *scmcms)
{
	m_param.cross_multiuser_challenge_matching_state = scmcms->matching_state;

	if (CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_CANCEL == m_param.cross_multiuser_challenge_matching_state)
	{
		m_param.cross_multiuser_challenge_matching_state = -1;
		this->SendCrossMultiuserChallengeSelfActivityInfo();

		if (1 == scmcms->notice_match)  // 自己正在冷却
		{
			m_role->NoticeNum(errornum::EN_MULTIUSER_CHALLENGE_SELF_IN_MATCH);
		}
		else if (2 == scmcms->notice_match)  // 其他成员正在冷却
		{
			m_role->NoticeNum(errornum::EN_MULTIUSER_CHALLENGE_MEMBER_IN_MATCH);
		}
		else if (3 == scmcms->notice_match)  // 有成员匹配中
		{
			m_role->NoticeNum(errornum::EN_MULTIUSER_CHALLENGE_MEMBER_IN_MATCHING);
		}
	}
	else if (CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_FINISH == m_param.cross_multiuser_challenge_matching_state)
	{
		m_param.cross_multiuser_challenge_matching_state = -1;

		// crossserver返回匹配完成状态，这里gameworld要判断是否主动拉进场景
		if ((1 == scmcms->auto_start_cross) && !m_role->IsInCross() && (1 == m_param.cross_3v3_logout_flag))
		{
			m_param.cross_3v3_logout_flag = 0;
			this->OnStartCrossReq(CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE, false, 0);
		}
		else
		{
			m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_CROSS_MULTIUSER_CHALLENGE);
		}
	}

	// log
	gamelog::g_log_cross.printf(LL_MAINTANCE, "SyncCross3v3MatchingState from CS. Role(%d, %s), cur_matching_state[%d], matching_state[%d], auto_start_cross[%d], notice_match[%d], 3v3_logout_flag[%d]",
		m_role->GetUID(), m_role->GetName(), 
		m_param.cross_multiuser_challenge_matching_state, scmcms->matching_state, (int)scmcms->auto_start_cross, (int)scmcms->notice_match, (int)m_param.cross_3v3_logout_flag);
}

void RoleCross::OnCrossMultiuserChallengeOpera(bool is_matching_req)
{
	if (!ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE)) return;
	if (m_role->IsInCross()) return;

	TeamInfo *team_info = m_role->GetTeamInfo();
	Role *role_list[16] = { NULL };
	int team_member_role_num = 0;

	if (team_info != nullptr && team_info->InTeam())
	{
		if (is_matching_req)
		{
			Team *team = team_info->GetMyTeam();
			if (NULL != team)
			{
				if (!team->IsLeader(m_role->GetUserId()))
				{
					m_role->NoticeNum(errornum::EN_MULTIUSER_CHALLENGE_MEMBER_IS_NOT_TEAM_LEADER);
					return;
				}

				team_member_role_num = team_info->GetOtherMemberInTheSameScene(role_list, 16);
				if (team_member_role_num != team->GetMemberNum() - 1)
				{
					m_role->NoticeNum(errornum::EN_MULTIUSER_CHALLENGE_MEMBER_NOT_IN_SAME_SCENE);
					return;
				}
			}
		}
		else
		{
			// 请求信息要显示所有成员的
			if (team_info->GetMyTeam())
			{
				int member_uid_list[MAX_TEAM_MEMBER_NUM];
				int member_count = team_info->GetMyTeam()->GetMemberUidList(member_uid_list, MAX_TEAM_MEMBER_NUM);
				for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM && team_member_role_num < 16; ++i)
				{
					if (m_role->GetUID() != member_uid_list[i])  // 自己在外面添加
					{
						Role *member_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
						if (NULL != member_role)
						{
							role_list[team_member_role_num] = member_role;
							++team_member_role_num;
						}
					}
				}
			}
		}
	}

	if (team_member_role_num < 0 || 16 <= team_member_role_num) return;

	// 把请求者自己加入列表
	role_list[team_member_role_num] = m_role; ++team_member_role_num;

	// 请求匹配
	if (is_matching_req)
	{
		// 检查友方状态
		for (int i = 0; i < team_member_role_num; ++i)
		{
			if (NULL == role_list[i])
			{
				continue;
			}

			if (NULL != role_list[i]->GetScene() && !role_list[i]->GetScene()->InStaticScene())
			{
				m_role->NoticeNum(errornum::EN_MULTIUSER_CHALLENGE_MEMBER_IN_FB);
				return;
			}

			if (role_list[i]->GetRoleCross()->GetCrossMultiuserChallengeMatchingState() >= 0)
			{
				m_role->NoticeNum(errornum::EN_MULTIUSER_CHALLENGE_MEMBER_IN_MATCHING);
				return;
			}

			if (!role_list[i]->GetRoleStatusManager()->CanStartCross())
			{
				m_role->NoticeNum(errornum::EN_MULTIUSER_CHALLENGE_MEMBER_CAN_NOT_START_CROSS);
				return;
			}

			int CROSS_MULTIUSER_CHALLENGE_JOIN_MIN_LEVEL = Cross3v3Config::Instance().GetJoinMinLevel();
			if (role_list[i]->GetLevel() < CROSS_MULTIUSER_CHALLENGE_JOIN_MIN_LEVEL)
			{
				m_role->NoticeNum(errornum::EN_MULTIUSER_CHALLENGE_MEMBER_LEVEL_LIMIT);
				return;
			}

			int CROSS_MULTIUSER_CHALLENGE_JOIN_MIN_CAPABILITY = Cross3v3Config::Instance().GetJoinMinCapability();
			if (role_list[i]->GetCapability()->GetTotalCapability() < CROSS_MULTIUSER_CHALLENGE_JOIN_MIN_CAPABILITY)
			{
				m_role->NoticeNum(errornum::EN_MULTIUSER_CHALLENGE_MEMBER_CAPABILITY_LIMIT);
				return;
			}

			int CROSS_MULTIUSER_CHALLENGE_DAY_COUNT_LIMIT = Cross3v3Config::Instance().GetJoinDayCountLimit();
			if (!role_list[i]->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_CROSS_MULTIUSER_CHALLENGE, CROSS_MULTIUSER_CHALLENGE_DAY_COUNT_LIMIT))
			{
				m_role->NoticeNum(errornum::EN_MULTIUSER_CHALLENGE_MEMBER_NO_DAYCOUNT);
				return;
			}
		}

		// 发送crossserver进行匹配
		static crossgameprotocal::GameCrossCrossMuliuserChallengeMatchingReq gccmcmr;
		gccmcmr.req_uuid = UniqueUserIDToLongLong(m_role->GetUniqueUserID());
		gccmcmr.count = 0;

		for (int i = 0; i < team_member_role_num && i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT; i++)
		{
			if (NULL == role_list[i])
			{
				continue;
			}
			gccmcmr.user_info_list[gccmcmr.count].uuid = UniqueUserIDToLongLong(role_list[i]->GetUniqueUserID());
			gccmcmr.user_info_list[gccmcmr.count].sex = role_list[i]->GetSex();
			gccmcmr.user_info_list[gccmcmr.count].prof = static_cast<char>(role_list[i]->GetBaseProf());
			gccmcmr.user_info_list[gccmcmr.count].camp = role_list[i]->GetCamp();
			gccmcmr.user_info_list[gccmcmr.count].level = role_list[i]->GetLevel();
			gccmcmr.user_info_list[gccmcmr.count].challenge_score = role_list[i]->GetRoleCross()->GetCrossMultiuserChallengeScore();
			gccmcmr.user_info_list[gccmcmr.count].win_percent = role_list[i]->GetRoleCross()->GetCrossMultiuserChallengeWinPercent();
			gccmcmr.user_info_list[gccmcmr.count].capability = role_list[i]->GetCapability()->GetTotalCapability();
			gccmcmr.user_info_list[gccmcmr.count].match_total_count = role_list[i]->GetRoleCross()->GetCrossMultiuserChallengeMatchCount();
			gccmcmr.user_info_list[gccmcmr.count].match_win_count = role_list[i]->GetRoleCross()->GetCrossMultiuserChallengeMatchWinCount();
			gccmcmr.user_info_list[gccmcmr.count].dur_win_count = role_list[i]->GetRoleCross()->GetCrossMultiuserChallengeDurWinCount();
			gccmcmr.user_info_list[gccmcmr.count].mvp_count = role_list[i]->GetRoleCross()->GetCrossMultiuserChallengeMvpCount();
			role_list[i]->GetName(gccmcmr.user_info_list[gccmcmr.count].user_name);

			++gccmcmr.count;
		}

		bool send_to_buff_succ = false;
		if (gccmcmr.count > 0)
		{
			send_to_buff_succ = InternalComm::Instance().SendToCross((const char *)&gccmcmr, sizeof(gccmcmr), CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE);
		}

		if (send_to_buff_succ)
		{
			WorldStatus2CommonDataParam &common_data_2 = WorldStatus2::Instance().GetCommonData2();

			// 设置友方数据
			for (int i = 0; i < team_member_role_num && i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT; ++i)
			{
				if (NULL == role_list[i])
				{
					continue;
				}

				role_list[i]->GetRoleCross()->CheckCreateCrossRecord();

				//role_list[i]->GetRoleCross()->m_param.cross_multiuser_challenge_matching_state = CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_TEAMMATE;

				role_list[i]->GetRoleCross()->m_param.cross_3v3_cur_join_season = common_data_2.cross3v3_season_count;
				role_list[i]->GetRoleCross()->m_param.cross_3v3_cur_join_season_dayid = common_data_2.cross_3v3_cur_join_season_dayid;
			}
		}
	}
	else
	{
		if (CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_TEAMMATE == m_param.cross_multiuser_challenge_matching_state
			|| CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_OPPONENT == m_param.cross_multiuser_challenge_matching_state)
		{
			// 已经在匹配了就不需要再下发初始信息
			return;
		}

		// 获取初始队伍信息
		static Protocol::SCCrossMultiuserChallengeBaseSelfSideInfo cmcbssi;
		cmcbssi.user_count = 0;

		for (int i = 0; i < team_member_role_num && i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT; i++)
		{
			if (NULL == role_list[i])
			{
				continue;
			}
			cmcbssi.user_list[cmcbssi.user_count].plat_type = role_list[i]->GetUniqueUserID().plat_type;
			cmcbssi.user_list[cmcbssi.user_count].server_id = role_list[i]->GetUniqueUserID().user_id.db_index;
			cmcbssi.user_list[cmcbssi.user_count].uid = UidToInt(role_list[i]->GetUserId());
			cmcbssi.user_list[cmcbssi.user_count].sex = role_list[i]->GetSex();
			cmcbssi.user_list[cmcbssi.user_count].prof = static_cast<char>(role_list[i]->GetBaseProf());
			cmcbssi.user_list[cmcbssi.user_count].camp = role_list[i]->GetCamp();
			cmcbssi.user_list[cmcbssi.user_count].level = role_list[i]->GetLevel();
			cmcbssi.user_list[cmcbssi.user_count].challenge_score = role_list[i]->GetRoleCross()->GetCrossMultiuserChallengeScore();
			cmcbssi.user_list[cmcbssi.user_count].win_percent = role_list[i]->GetRoleCross()->GetCrossMultiuserChallengeWinPercent();
			cmcbssi.user_list[cmcbssi.user_count].mvp_count = role_list[i]->GetRoleCross()->GetCrossMultiuserChallengeMvpCount();
			cmcbssi.user_list[cmcbssi.user_count].capability = role_list[i]->GetCapability()->GetTotalCapability();
			role_list[i]->GetName(cmcbssi.user_list[cmcbssi.user_count].user_name);

			++cmcbssi.user_count;
		}

		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cmcbssi, sizeof(cmcbssi));
	}
}

// void RoleCross::OnCrossMultiuserChallengeFetchDaycountReward()
// {
// 	int CROSS_MULTIUSER_CHALLENGE_REWARD_DAY_COUNT = Cross3v3Config::Instance().GetRewardDayCount();
// 
// 	if (!m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_CROSS_MULTIUSER_CHALLENGE_DAYCOUNT_REWARD, 1) || 
// 		m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_CROSS_MULTIUSER_CHALLENGE, CROSS_MULTIUSER_CHALLENGE_REWARD_DAY_COUNT))
// 	{
// 		return;
// 	}
// 
// 	{
// 		// 发邮件奖励
// 		int reward_honor = Cross3v3Config::Instance().GetDayRewardHonor();
// 		if (reward_honor > 0)
// 		{
// 			static MailContentParam contentparam; contentparam.Reset();
// 			contentparam.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_CROSS_HONOR] = reward_honor;
// 			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cross_multiuser_challenge_day_reward_mail_content, reward_honor);
// 			if (length > 0)
// 			{
// 				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
// 			}
// 			m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_CROSS_MULTIUSER_CHALLENGE_DAYCOUNT_REWARD);
// 			this->SendCrossMultiuserChallengeSelfActivityInfo();
// 		}
// 	}
// 	//m_role->NoticeNum(noticenum::NT_FETCH_CROSS_MULTIUSER_CHALLENGE_DAYCOUNT_REWARD_SUCCESS);
// }

void RoleCross::CancelCrossMultiuserChallengeMatching(bool can_cancel_self_in_team /* = false */)
{
	if (CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_TEAMMATE == m_param.cross_multiuser_challenge_matching_state 
		|| CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_OPPONENT == m_param.cross_multiuser_challenge_matching_state)
	{
		TeamInfo *team_info = m_role->GetTeamInfo();

		if (NULL != team_info && team_info->InTeam() && NULL != team_info->GetMyTeam())
		{
			if (team_info->GetMyTeam()->IsLeader(m_role->GetUserId()))
			{
				// 所有队员都要取消
				int member_uid_list[MAX_TEAM_MEMBER_NUM];
				int member_count = team_info->GetMyTeam()->GetMemberUidList(member_uid_list, MAX_TEAM_MEMBER_NUM);
				for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
				{
					Role *member_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
					if (NULL != member_role)
					{
						// 通知crossserver取消匹配
						crossgameprotocal::GameCrossCrossMuliuserChallengeMatchingCancel gccmcmc;
						gccmcmc.uuid = UniqueUserIDToLongLong(member_role->GetUniqueUserID());
						InternalComm::Instance().SendToCross((const char *)&gccmcmc, sizeof(gccmcmc), CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE);
					}
				}

				return;
			}
			else
			{
				// 身在队伍，但不是队长，不可以取消自己的匹配状态
				if (!can_cancel_self_in_team)
				{
					return;
				}
			}
		}

		// 通知crossserver取消匹配
		crossgameprotocal::GameCrossCrossMuliuserChallengeMatchingCancel gccmcmc;
		gccmcmc.uuid = UniqueUserIDToLongLong(m_role->GetUniqueUserID());
		InternalComm::Instance().SendToCross((const char *)&gccmcmc, sizeof(gccmcmc), CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE);
	}
}

void RoleCross::AddCrossMultiuserChallengeScore(int add_score, const char *reason)
{
	m_param.cross_multiuser_challenge_score += add_score;
	if (m_param.cross_multiuser_challenge_score < CROSS_MULTIUSER_CHALLENGE_MIN_SCORE)
	{
		m_param.cross_multiuser_challenge_score = CROSS_MULTIUSER_CHALLENGE_MIN_SCORE;
	}

	gamelog::g_log_cross.printf(LL_INFO, "AddCrossMultiuserChallengeScore user[%d,%s] challenge_score[%d] add_score[%d] reason[%s]",
		m_role->GetUID(), m_role->GetName(), m_param.cross_multiuser_challenge_score, add_score, reason);
}

// 跨服3v3个人信息下发
void RoleCross::SendCrossMultiuserChallengeSelfActivityInfo()
{
	WorldStatus2CommonDataParam &common_data_2 = WorldStatus2::Instance().GetCommonData2();
	static Protocol::SCCrossMultiuserChallengeSelfActicityInfo cmcsai;
	cmcsai.challenge_score = m_param.cross_multiuser_challenge_score;
	cmcsai.challenge_mvp_count = m_param.cross_multiuser_challenge_mvp_count;
	cmcsai.challenge_total_match_count = m_param.cross_multiuser_challenge_month_match_count;
	cmcsai.challenge_win_match_count = m_param.cross_multiuser_challenge_win_match_count;
	cmcsai.win_percent = this->GetCrossMultiuserChallengeWinPercent();
	cmcsai.today_match_count = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_CROSS_MULTIUSER_CHALLENGE);
	cmcsai.matching_state = m_param.cross_multiuser_challenge_matching_state;
	cmcsai.join_reward_fetch_flag = m_param.cross_3v3_join_reward_fetch_flag;
	cmcsai.gongxun_reward_fetch_flag = m_param.cross_3v3_gongxun_reward_fetch_flag;
	cmcsai.gongxun_value = m_param.cross_3v3_gongxun_value;
	memcpy(cmcsai.cross_3v3_season_reward, m_param.cross_3v3_season_reward, sizeof(m_param.cross_3v3_season_reward));
	memcpy(cmcsai.cross_3v3_season_reward_use, m_param.cross_3v3_season_reward_use, sizeof(m_param.cross_3v3_season_reward_use));
	cmcsai.season_count = common_data_2.cross3v3_season_count;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cmcsai, sizeof(cmcsai));
}

int RoleCross::GetCrossMultiuserChallengeWinPercent()
{
	if (m_param.cross_multiuser_challenge_month_match_count <= 0) return 0;

	return m_param.cross_multiuser_challenge_win_match_count * 100 / m_param.cross_multiuser_challenge_month_match_count;
}

void RoleCross::SyncCrossMultiuserChallengeRecordInfo()
{
	CrossRecord *cross_record = CrossRecordManager::Instance().GetCrossRecord(m_role->GetUserId());
	if (NULL == cross_record || cross_record->user_record_data.has_sync_cross_multiuser_challenge_to_role <= 0)
	{
		return;
	}

	this->AddCrossMultiuserChallengeScore(cross_record->user_record_data.cross_multiuser_challenge_score, "CrossMultiuserChallengeFightReward");
	m_param.cross_multiuser_challenge_total_match_count = cross_record->user_record_data.cross_multiuser_challenge_match_total_count;
	++m_param.cross_multiuser_challenge_month_match_count;

	m_param.cross_multiuser_challenge_win_match_count = cross_record->user_record_data.cross_multiuser_challenge_match_win_count;
	m_param.cross_multiuser_challenge_mvp_count = cross_record->user_record_data.cross_multiuser_challenge_mvp_count;
	m_role->AddCrossHonor(cross_record->user_record_data.cross_multiuser_challenge_add_honor, "CrossMultiuserChallengeFightReward");
	m_role->GetRoleCross()->Add3v3Gongxun(cross_record->user_record_data.cross_3v3_add_gongxun, "CrossMultiuserChallengeFightReward");

	cross_record->user_record_data.has_sync_cross_multiuser_challenge_to_role = 0;

	this->SendCrossMultiuserChallengeSelfActivityInfo();
}

void RoleCross::SyncCrossTuanzhanRecordInfo()
{
	/*
	CrossRecord *cross_record = CrossRecordManager::Instance().GetCrossRecord(m_role->GetUserId());
	if (NULL == cross_record || cross_record->user_record_data.cross_tuanzhan_has_sync_to_role <= 0)
	{
		return;
	}

	const CrossTuanzhanOtherCfg &other_cfg = CrossTuanzhanConfig::Instance().GetOtherCfg();

	int join_reward_honor = 0;
	{
		// 参与奖励荣誉
		join_reward_honor = static_cast<int>(cross_record->user_record_data.cross_tuanzhan_score * other_cfg.join_honor_factor_percent * 1.0 / 100);

		if (join_reward_honor < other_cfg.join_honor_min) join_reward_honor = other_cfg.join_honor_min;
		if (join_reward_honor > other_cfg.join_honor_max) join_reward_honor = other_cfg.join_honor_max;
	}

	static MailContentParam reward_mail;  reward_mail.Reset();

	int mail_content_result = SNPRINTF(reward_mail.contenttxt, sizeof(reward_mail.contenttxt), gamestring::g_cross_tuanzhan_reward_loser_content);
	reward_mail.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_CROSS_HONOR] = join_reward_honor;

	if (cross_record->user_record_data.cross_tuanzhan_is_win > 0) // 胜利方额外奖励
	{
		mail_content_result = SNPRINTF(reward_mail.contenttxt, sizeof(reward_mail.contenttxt), gamestring::g_cross_tuanzhan_reward_winner_content);
		reward_mail.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_CROSS_HONOR] += other_cfg.winner_cross_honor;

		if (cross_record->user_record_data.cross_tuanzhan_side_rank <= other_cfg.winner_rank_num) // 胜利方前N名额外物品奖励
		{
			mail_content_result = SNPRINTF(reward_mail.contenttxt, sizeof(reward_mail.contenttxt), gamestring::g_cross_tuanzhan_reward_rank_winner_content,
				cross_record->user_record_data.cross_tuanzhan_side_rank);

			int mail_reward_index = 0;
			for (int reward_index = 0; reward_index < other_cfg.winner_rank_reward_item_list_count && reward_index < CROSS_TUANZHAN_WINNER_REWARD_ITEM_MAX_COUNT; ++ reward_index)
			{
				const ItemBase *itembase = ITEMPOOL->GetItem(other_cfg.winner_rank_reward_item_list[reward_index].item_id);
				if (NULL != itembase)
				{
					reward_mail.item_list[mail_reward_index].item_id = other_cfg.winner_rank_reward_item_list[reward_index].item_id;
					reward_mail.item_list[mail_reward_index].num = other_cfg.winner_rank_reward_item_list[reward_index].num;
					reward_mail.item_list[mail_reward_index].is_bind = (other_cfg.winner_rank_reward_item_list[reward_index].is_bind ? 1 : 0);
					reward_mail.item_list[mail_reward_index].invalid_time = itembase->CalInvalidTime();

					++ mail_reward_index;
				}
			}
		}
	}

	if (mail_content_result > 0)
	{
		MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, reward_mail);
		cross_record->user_record_data.cross_tuanzhan_has_sync_to_role = 0;
		CrossRecordManager::Instance().SaveCrossRecord(cross_record, structcommon::CS_UPDATE);
	}
	*/
}

void RoleCross::OnCrossMultiuserChallengeFetchJoinTimesReward(int seq /* = -1 */, bool need_notify /* = true */)
{
	const Cross3V3JoinTimesRewardConfig *join_times_reward_cfg_list = Cross3v3Config::Instance().GetJoinTimesRewardCfg();
	if (NULL == join_times_reward_cfg_list) return;

	if (seq < 0) // 全部领取
	{
		for (int i = 0; i < CROSS_ACTIVITY_3V3_JOIN_TIME_CFG_MAX_COUNT; i++)
		{
			const Cross3V3JoinTimesRewardConfig &reward_cfg = join_times_reward_cfg_list[i];
			if (m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_CROSS_MULTIUSER_CHALLENGE, reward_cfg.jion_times))
			{
				continue;
			}

			if ((m_param.cross_3v3_join_reward_fetch_flag & (1 << i)) != 0) // 已领取
			{
				continue;
			}

			m_param.cross_3v3_join_reward_fetch_flag |= (1 << i);
			gamecommon::SendMultiMailToRole(m_role->GetUserId(), reward_cfg.reward_item_list, reward_cfg.reward_count, NULL, true, gamestring::g_cross_multiuser_challenge_day_reward_mail_content, reward_cfg.jion_times);
		}
	}
	else if (seq < CROSS_ACTIVITY_3V3_JOIN_TIME_CFG_MAX_COUNT)
	{
		const Cross3V3JoinTimesRewardConfig &reward_cfg = join_times_reward_cfg_list[seq];
		if (m_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_CROSS_MULTIUSER_CHALLENGE, reward_cfg.jion_times))
		{
			return;
		}

		if ((m_param.cross_3v3_join_reward_fetch_flag & (1 << seq)) != 0) // 已领取
		{
			return;
		}

		m_param.cross_3v3_join_reward_fetch_flag |= (1 << seq);
		if (!m_role->GetKnapsack()->PutList(reward_cfg.reward_count, reward_cfg.reward_item_list, PUT_REASON_3V3_JOIN_TIMES_REWARD))
		{
			gamecommon::SendMultiMailToRole(m_role->GetUserId(), reward_cfg.reward_item_list, reward_cfg.reward_count, NULL, true, gamestring::g_cross_multiuser_challenge_day_reward_mail_content, reward_cfg.jion_times);
		}
	}

	if (need_notify)
	{
		this->SendCrossMultiuserChallengeSelfActivityInfo();
	}
	//m_role->NoticeNum(noticenum::NT_FETCH_CROSS_MULTIUSER_CHALLENGE_DAYCOUNT_REWARD_SUCCESS);
}

void RoleCross::OnCrossMultiuserChallengeFetchGongxunReward(int seq /* = -1 */, bool need_notify /* = true */)
{
	if (seq < 0) // 全部领取
	{
		for (int tmp_seq = 0; tmp_seq < CROSS_ACTIVITY_3V3_GONGXUN_CFG_MAX_COUNT; ++tmp_seq)
		{
			const Cross3V3GongxunRewardConfig *gongxun_cfg = Cross3v3Config::Instance().GetGongxunRewardCfgBySeq(tmp_seq);
			if (NULL == gongxun_cfg) continue;

			if (m_param.cross_3v3_gongxun_value < gongxun_cfg->need_gongxun) continue; // 未达到条件

			if ((m_param.cross_3v3_gongxun_reward_fetch_flag & (1 << tmp_seq)) != 0) continue; // 已领取

			m_param.cross_3v3_gongxun_reward_fetch_flag |= (1 << tmp_seq);

			// 给予奖励
			gamecommon::SendMultiMailToRole(m_role->GetUserId(), gongxun_cfg->reward_item_list, gongxun_cfg->reward_count, NULL, 
				true, gamestring::g_cross_multiuser_challenge_gongxun_reward_mail_content);
		}
	}
	else if (seq < CROSS_ACTIVITY_3V3_GONGXUN_CFG_MAX_COUNT)
	{
		const Cross3V3GongxunRewardConfig *gongxun_cfg = Cross3v3Config::Instance().GetGongxunRewardCfgBySeq(seq);
		if (NULL == gongxun_cfg) return;

		if (m_param.cross_3v3_gongxun_value < gongxun_cfg->need_gongxun) return; // 未达到条件

		if ((m_param.cross_3v3_gongxun_reward_fetch_flag & (1 << seq)) != 0) return; // 已领取

																					 // 消耗
		m_param.cross_3v3_gongxun_reward_fetch_flag |= (1 << seq);

		// 给予奖励 - 背包放入失败则发邮件
		if (!m_role->GetKnapsack()->PutList(gongxun_cfg->reward_count, gongxun_cfg->reward_item_list, PUT_REASON_3V3_GONGXUN_REWARD))
		{
			gamecommon::SendMultiMailToRole(m_role->GetUserId(), gongxun_cfg->reward_item_list, gongxun_cfg->reward_count, NULL, 
				true, gamestring::g_cross_multiuser_challenge_gongxun_reward_mail_content);
		}
	}

	if (need_notify)
	{
		this->SendCrossMultiuserChallengeSelfActivityInfo();
	}
}

bool RoleCross::Add3v3Gongxun(int add_gongxun, const char *reason)
{
	if (0 == add_gongxun || NULL == reason)
	{
		return false;
	}

	long long tmp_cross_gongxun = m_param.cross_3v3_gongxun_value + add_gongxun;
	if (tmp_cross_gongxun < 0)
	{
		tmp_cross_gongxun = 0;
	}
	else if (tmp_cross_gongxun > UINT_MAX)
	{
		tmp_cross_gongxun = UINT_MAX;
	}

	m_param.cross_3v3_gongxun_value = static_cast<unsigned int>(tmp_cross_gongxun);
	this->SendCrossMultiuserChallengeSelfActivityInfo();

	gamelog::g_log_cross.printf(LL_INFO, "Add3v3Gongxun user[%d,%s] 3v3_gongxun[%d] add_gongxun[%d] reason[%s]",
		m_role->GetUID(), m_role->GetName(), m_param.cross_3v3_gongxun_value, add_gongxun, reason);

	return true;
}

void RoleCross::Cross3v3AddMatchTimes(int add_times, const char* reason)
{
	long long tmp_match_times = m_param.cross_multiuser_challenge_month_match_count + add_times;
	if (tmp_match_times < 0)
	{
		tmp_match_times = 0;
	}
	else if (tmp_match_times > USHRT_MAX)
	{
		tmp_match_times = USHRT_MAX;
	}

	m_param.cross_multiuser_challenge_month_match_count = static_cast<unsigned short>(tmp_match_times);

	// 增加今日次数
	int CROSS_MULTIUSER_CHALLENGE_DAY_COUNT_LIMIT = Cross3v3Config::Instance().GetJoinDayCountLimit();
	int daycount = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_CROSS_MULTIUSER_CHALLENGE);
	if (daycount + add_times > CROSS_MULTIUSER_CHALLENGE_DAY_COUNT_LIMIT)
	{
		m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_CROSS_MULTIUSER_CHALLENGE, CROSS_MULTIUSER_CHALLENGE_DAY_COUNT_LIMIT);
	}
	else
	{
		m_role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_CROSS_MULTIUSER_CHALLENGE, daycount + add_times);
	}

	this->SendCrossMultiuserChallengeSelfActivityInfo();

	//gamelog::g_log_cross.printf(LL_INFO, "Cross3v3AddMatchTimes user[%d,%s] match_count[%d] add_count[%d] reason[%s]",
	//	m_role->GetUID(), m_role->GetName(), m_param.cross_multiuser_challenge_month_match_count, add_times, reason);
}

void RoleCross::OnCrossMultiuserChallengeCardOper(int oper_type, int seq)
{
	const Cross3v3SeasonRewardConfig *season_cfg = Cross3v3Config::Instance().GetSeasonCfgBySeq(seq);
	if (NULL == season_cfg)
	{
		return;
	}

	switch (oper_type)
	{
	case CROSS_RING_CARD_OPER_WEAR:
	{
		int use_count = 0;
		for (int i = 0; i < CROSS_ACTIVITY_3V3_RING_USE_COUNT; ++i)
		{
			if (m_param.cross_3v3_season_reward_use[i] > 0)
			{
				++use_count;
			}

			if (m_param.cross_3v3_season_reward_use[i] == season_cfg->season)
			{
				return;
			}
		}
		if (use_count >= CROSS_ACTIVITY_3V3_RING_USE_COUNT)
		{
			m_role->NoticeNum(errornum::EN_MULTIUSER_CHALLENGE_WAER_CARD_LIMIT);
			return;
		}

		//int index = m_param.cross_3v3_season_reward_use[0] <= 0 ? 0 : 1;
		for (int i = 0; i < CROSS_ACTIVITY_3V3_RING_USE_COUNT; ++i)
		{
			if (m_param.cross_3v3_season_reward_use[i] <= 0)
			{
				if (m_param.cross_3v3_season_reward[season_cfg->season - 1] == season_cfg->grade)
				{
					m_param.cross_3v3_season_reward_use[i] = season_cfg->season;
				}
				break;
			}
		}
	}
	break;
	case CROSS_RING_CARD_OPER_OFF:
	{
		for (int i = 0; i < CROSS_ACTIVITY_3V3_RING_USE_COUNT; ++i)
		{
			if (m_param.cross_3v3_season_reward_use[i] == season_cfg->season)
			{
				m_param.cross_3v3_season_reward_use[i] = 0;
			}
		}
	}
	break;
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ROLE_CROSS);

	this->SendCrossMultiuserChallengeSelfActivityInfo();
}

void RoleCross::Cross3v3GiveDivisionReward()
{
	WorldStatus2CommonDataParam &common_data_2 = WorldStatus2::Instance().GetCommonData2();

	gamelog::g_log_cross.printf(LL_INFO, "RoleCross::Cross3v3GiveDivisionReward: user(%d, %s), season:%d, score:%d, join_times:%d, win_times:%d, m_dayid:%d, s_dayid:%d", 
		m_role->GetUID(), m_role->GetName(), m_param.cross_3v3_cur_join_season, m_param.cross_multiuser_challenge_score, m_param.cross_multiuser_challenge_month_match_count, m_param.cross_multiuser_challenge_win_match_count,
		m_param.cross_3v3_cur_join_season_dayid, common_data_2.cross_3v3_cur_join_season_dayid);

	int rank_last_score = 0;  // 获取玩家当前参加的赛季的最低排行积分
	int last_join_season = m_param.cross_3v3_cur_join_season % CROSS_3V3_SEASON_MAX;
	if (0 <= last_join_season && last_join_season < CROSS_3V3_SEASON_MAX && m_param.cross_3v3_cur_join_season > 0)
	{
		rank_last_score = common_data_2.cross3v3_rank_min_score[last_join_season];
	}

	// 大于排行榜最低积分则说明自己“应该”在榜上，会获得排行奖励，就不给发段位奖励了。这个需求挺蛋碎的，这样同步的方式来判断，只能保证正常情况下没问题~~（比如有玩家跨越一个赛季才登陆就有问题了）
	if (m_param.cross_multiuser_challenge_score >= rank_last_score && rank_last_score > 0)
	{
		return;
	}

	if (m_param.cross_multiuser_challenge_month_match_count < Cross3v3Config::Instance().GetOtherCfg().division_need_match_count)
	{
		return;
	}

	const Cross3V3GradeScoreConfig* reward_cfg = Cross3v3Config::Instance().GetDivisionConfigWithScore(m_param.cross_multiuser_challenge_score);
	if (NULL != reward_cfg)
	{
		// 段位奖励
		int v_item_list[MAIL_VIRTUAL_ITEM_MAX]; memset(v_item_list, 0, sizeof(v_item_list));
		v_item_list[MAIL_VIRTUAL_ITEM_BIND_GOLD] = reward_cfg->bind_gold;
		v_item_list[MAIL_VIRTUAL_ITEM_CONVERTSHOP_MOJING] = reward_cfg->mojing;

		gamecommon::SendMultiMailToRole(m_role->GetUserId(), reward_cfg->reward_item_list, reward_cfg->reward_count, v_item_list, 
			true, gamestring::g_cross_multiuser_challenge_season_score_reward_content);
		
		gamelog::g_log_cross.printf(LL_INFO, "Cross3v3GiveDivisionReward: user(%d, %s), division_grade:%d, bind_gold:%d, mojing:%d", 
			m_role->GetUID(), m_role->GetName(), reward_cfg->grade, reward_cfg->bind_gold, reward_cfg->mojing);

		// 段位令牌
		if (m_param.cross_3v3_cur_join_season <= 0)
		{
			gamelog::g_log_cross.printf(LL_INFO, "Cross3v3GiveDivisionReward[Card] Fail: user(%d, %s), join_season[%d], score:%d",
				m_role->GetUID(), m_role->GetName(), m_param.cross_3v3_cur_join_season, m_param.cross_multiuser_challenge_score);

			return;
		}

		const int MAX_OPEN_SEASON = Cross3v3Config::Instance().GetMaxSeason();
		const Cross3v3SeasonRewardConfig * season_cfg = NULL;

		if (m_param.cross_3v3_cur_join_season <= MAX_OPEN_SEASON)
		{
			season_cfg = Cross3v3Config::Instance().GetSeasonCfgBySeasonGrade(m_param.cross_3v3_cur_join_season, reward_cfg->grade);
		}
		else
		{
			season_cfg = Cross3v3Config::Instance().GetAfterSeasonCfgBySeasonGrade(m_param.cross_3v3_cur_join_season, reward_cfg->grade);
		}
		
		if (NULL != season_cfg)
		{
			static MailContentParam contentparam; contentparam.Reset();
			const ItemConfigData &item_cfg = season_cfg->reward_item;
			const ItemBase *item_base = ITEMPOOL->GetItem(item_cfg.item_id);
			if (NULL == item_base) return;

			contentparam.item_list[0].item_id = item_cfg.item_id;
			contentparam.item_list[0].num = item_cfg.num;
			contentparam.item_list[0].is_bind = item_cfg.is_bind;
			contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cross_multiuser_challenge_season_card_reward_content);
			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

				gamelog::g_log_cross.printf(LL_INFO, "Cross3v3GiveDivisionReward[Card]: user(%d, %s), join_season[%d], score:%d, reward_item_id:%d, reward_item_num:%d",
					m_role->GetUID(), m_role->GetName(), m_param.cross_3v3_cur_join_season, m_param.cross_multiuser_challenge_score, item_cfg.item_id, item_cfg.num);
			}
		}
	}
	else
	{
		gamelog::g_log_cross.printf(LL_INFO, "Cross3v3GiveDivisionReward Fail : user(%d, %s), score:%d",
			m_role->GetUID(), m_role->GetName(), m_param.cross_multiuser_challenge_score);
	}
}

bool RoleCross::IsInCross3v3Matching()
{
	if (ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE))
	{
		int cross3v3_match_state = m_param.cross_multiuser_challenge_matching_state;
		if (CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_TEAMMATE == cross3v3_match_state
			|| CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_OPPONENT == cross3v3_match_state)
		{
			return true;
		}
	}

	return false;
}

void RoleCross::CheckCrossServerCross3v3MatchingState()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (!ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE))
	{
		m_param.cross_3v3_logout_flag = 0;
		return;
	}

	if (1 != m_param.cross_3v3_logout_flag)
	{
		return;
	}

	// 向crossserver查询匹配状态
	crossgameprotocal::GameCrossCrossMuliuserChallengeGetMatchingState gccmcgms;
	gccmcgms.uuid = UniqueUserIDToLongLong(m_role->GetUniqueUserID());
	InternalComm::Instance().SendToCross((const char *)&gccmcgms, sizeof(gccmcgms), CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE);

	gamelog::g_log_cross.printf(LL_MAINTANCE, "CheckCrossServerCross3v3MatchingState Role(%d, %s), 3v3_logout_flag[%d]",
		m_role->GetUID(), m_role->GetName(), (int)m_param.cross_3v3_logout_flag);
}

void RoleCross::OnClearCross3v3Matching(int cross_type)
{
	if (!this->IsInCross3v3Matching())
	{
		return;
	}

	if (cross_type <= 0)
	{
		// 全部跨服断开，比如重读配置
		m_param.cross_multiuser_challenge_matching_state = -1;
		this->SendCrossMultiuserChallengeSelfActivityInfo();
	}
	else if (cross_type > 0 && CrossConfig::Instance().CheckCrossServerHasCrossActivity(CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE, cross_type))
	{
		// 单个跨服断开
		m_param.cross_multiuser_challenge_matching_state = -1;
		this->SendCrossMultiuserChallengeSelfActivityInfo();
	}

	// log
	gamelog::g_log_cross.printf(LL_WARNING, "Clear role(%d, %s) cross3v3 match on crossserver[cross_type:%d] disconnect.",
		m_role->GetUID(), m_role->GetName(), cross_type);
}

void RoleCross::AddPastureScore(int add_score, const char *reason)
{
	if (0 == add_score || NULL == reason)
	{
		return;
	}

	long long tmp_cross_pasture_score = m_param.cross_pasture_score + add_score;
	if (tmp_cross_pasture_score < 0)
	{
		tmp_cross_pasture_score = 0;
	}
	else if (tmp_cross_pasture_score > UINT_MAX)
	{
		tmp_cross_pasture_score	= UINT_MAX;
	}

	m_param.cross_pasture_score = static_cast<unsigned int>(tmp_cross_pasture_score);

	gamelog::g_log_cross.printf(LL_INFO, "AddPastureScore user[%d,%s] pasture_score[%d] add_score[%d] reason[%s]",
		m_role->GetUID(), m_role->GetName(), m_param.cross_pasture_score, add_score, reason);

	return;
}

bool RoleCross::ConsumePastureScore(unsigned int score, const char *reason)
{
	if (0 == score || NULL == reason)
	{
		return false;
	}

	if (m_param.cross_pasture_score < score)
	{
		m_role->NoticeNum(errornum::EN_PASTURE_SPIRIT_POINT_NOT_ENOUGH);
		return false;
	}

	m_param.cross_pasture_score -= score;

	gamelog::g_log_cross.printf(LL_INFO, "ConsumePastureScore user[%d,%s] pasture_score[%d] score[%d] reason[%s]",
		m_role->GetUID(), m_role->GetName(), m_param.cross_pasture_score, score, reason);

	return true;
}

bool RoleCross::AddPastureGetScoreTimes(int add_score_times, const char *reason)
{
	if (0 == add_score_times || NULL == reason)
	{
		return false;
	}

	long long tmp_cross_pasture_score = m_param.cross_pasture_today_get_score_times + add_score_times;
	if (tmp_cross_pasture_score < 0)
	{
		tmp_cross_pasture_score = 0;
	}
	else if (tmp_cross_pasture_score > SHRT_MAX)
	{
		tmp_cross_pasture_score	= SHRT_MAX;
	}

	m_param.cross_pasture_today_get_score_times = static_cast<short>(tmp_cross_pasture_score);

	gamelog::g_log_cross.printf(LL_INFO, "AddPastureGetScoreTimes user[%d,%s] today_get_score_times[%d] add_times[%d] reason[%s]",
		m_role->GetUID(), m_role->GetName(), m_param.cross_pasture_today_get_score_times, add_score_times, reason);

	return true;
}

void RoleCross::SyncCrossPastureRecordInfo()
{
	CrossRecord *cross_record = CrossRecordManager::Instance().GetCrossRecord(m_role->GetUserId());
	if (NULL == cross_record || cross_record->user_record_data.cross_pasture_need_sync_to_role <= 0)
	{
		return;
	}

	m_role->GetRoleCross()->AddPastureScore(cross_record->user_record_data.cross_pasture_get_score, "SyncCrossPastureRecordInfo");
	m_role->GetRoleCross()->AddPastureGetScoreTimes(cross_record->user_record_data.cross_pasture_get_score_times, "SyncCrossPastureRecordInfo");

	cross_record->user_record_data.cross_pasture_need_sync_to_role = 0;
	CrossRecordManager::Instance().SaveCrossRecord(cross_record, structcommon::CS_UPDATE);
}


void RoleCross::SendCrossBossSelfInfo()
{
	const CrossBossOtherCfg &other_cfg = CrossBossConfig::Instance().GetOtherCfg();
	int left_ordinary_crystal_gather_times = other_cfg.ordinary_crystal_gather_times - m_param.today_gather_ordinary_crystal_num;
	int left_treasure_crystal_gather_times = other_cfg.treasure_crystal_gather_times - m_param.today_gather_treasure_crystal_num;

	static Protocol::SCCrossBossPlayerInfo cbpi;
	cbpi.left_ordinary_crystal_gather_times = left_ordinary_crystal_gather_times > 0 ? left_ordinary_crystal_gather_times : 0;
	cbpi.left_treasure_crystal_gather_times = left_treasure_crystal_gather_times > 0 ? left_treasure_crystal_gather_times : 0;
	cbpi.left_can_kill_boss_num = m_param.today_kill_boss_num < 0 ? 0 : m_param.today_kill_boss_num;

	m_role->GetRoleCross()->GetCrossBossConcernFlag(cbpi.concern_flag);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cbpi, sizeof(cbpi));
}

void RoleCross::SyncCrossBossRecordInfo()
{
	CrossRecord *cross_record = CrossRecordManager::Instance().GetCrossRecord(m_role->GetUserId());
	if (NULL == cross_record || cross_record->user_record_data.cross_boss_need_sync_to_role <= 0)
	{
		return;
	}

	m_role->GetRoleCross()->AddCrossBossReliveTimes(-cross_record->user_record_data.cross_boss_relive_times, "SyncCrossBossRecordInfo", false);
	m_param.today_gather_ordinary_crystal_num = cross_record->user_record_data.today_gather_ordinary_crystal_times;
	m_param.today_gather_treasure_crystal_num = cross_record->user_record_data.today_gather_treasure_crystal_times;
	m_param.today_kill_boss_num = cross_record->user_record_data.tire_value;

	this->SendCrossBossSelfInfo();

	cross_record->user_record_data.cross_boss_need_sync_to_role = 0;
	CrossRecordManager::Instance().SaveCrossRecord(cross_record, structcommon::CS_UPDATE);
}

void RoleCross::GetCrossBossConcernFlag(unsigned int *flag)
{
	memcpy(flag, m_param.cross_boss_concern_flag, sizeof(m_param.cross_boss_concern_flag));
}

void RoleCross::RegesiterCrossBossConcernInfo()
{
	if (m_role->IsInCross())
	{
		return;
	}

	for (int i = 0; i < CROSS_BOSS_SCENE_MAX; i++)
	{
		for (int j = 0; j < MAX_CROSS_BOSS_PER_SCENE; j++)
		{
			if (0 != (m_param.cross_boss_concern_flag[i] & (1 << j)))
			{
				const CrossBossBossCfg::BossCfg *boss_cfg = CrossBossConfig::Instance().GetCrossBossBossCfgByLayer(i, j);
				if (NULL == boss_cfg)
				{
					continue;
				}

				static crossgameprotocal::GameGameBossInfoReq ggbir;

				ggbir.cross_boss_type = crossgameprotocal::CROSS_BOSS_TYPE_CROSS;
				ggbir.uuid = UniqueUserIDToLongLong(this->GetCrossOriginalUniqueUserID());
				ggbir.req_type = Protocol::CROSS_BOSS_OPERA_TYPE_CONCERN_BOSS;
				ggbir.param_1 = i;
				ggbir.param_2 = boss_cfg->boss_id;

				InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_HIDDEN, 0, (const char *)&ggbir, sizeof(ggbir));
				return;
			}
		}
	}
}

void RoleCross::SetCrossBossConcernFlag(unsigned int flag, int index)
{
	if (index < 0 || index >= CROSS_BOSS_SCENE_MAX)
	{
		return;
	}

	m_param.cross_boss_concern_flag[index] = flag;
}

bool RoleCross::OnCrossBossDecTireValue(int tire_value)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		if (m_param.today_kill_boss_num < tire_value)
		{
			return false;
		}

		m_param.today_kill_boss_num -= tire_value;

		this->SendCrossBossSelfInfo();
		return true;
	}
	else
	{
		if (NULL == m_role->GetScene())
		{
			return false;
		}

		if (m_role->GetScene()->GetSceneType() != Scene::SCENE_TYPE_CROSS_BOSS)
		{
			return false;
		}

		SpecialCrossBoss *cross_boss = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossBoss(m_role->GetScene()->GetSceneID(), CROSS_PUBLIC_SCENE_KEY);
		if (NULL == cross_boss)
		{
			return false;
		}

		if (!cross_boss->DecCrossBossTire(m_role, tire_value))
		{
			return false;
		}

		return true;
	}

	return false;
}

void RoleCross::OnCrossBossReq(int opera_type, int layer, int param2)
{
	if (opera_type < 0 || opera_type >= Protocol::CROSS_BOSS_OPERA_TYPE_MAX || layer > CROSS_BOSS_SCENE_MAX)
	{
		return;
	}

	if (opera_type != Protocol::CROSS_BOSS_OPERA_TYPE_GET_FLUSH_INFO && opera_type != Protocol::CROSS_BOSS_OPERA_TYPE_DROP_RECORD && layer <= 0)
	{
		return;
	}

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		if (opera_type == Protocol::CROSS_BOSS_OPERA_TYPE_CONCERN_BOSS)
		{
			int index = CrossBossConfig::Instance().GetIndexByBossId(layer, param2);
			if (index < 0 || index >= MAX_CROSS_BOSS_PER_SCENE)
			{
				return;
			}

			if (0 != (m_param.cross_boss_concern_flag[layer - 1] & (1 << index)))
			{
				return;
			}

			m_param.cross_boss_concern_flag[layer - 1] |= (1 << index);
			this->SendCrossBossSelfInfo();
			m_role->NoticeNum(noticenum::NT_WORLD_BOSS_CONCERN_SUCC);
		}
		else if (opera_type == Protocol::CROSS_BOSS_OPERA_TYPE_UNCONCERN_BOSS)
		{
			int index = CrossBossConfig::Instance().GetIndexByBossId(layer, param2);
			if (index < 0 || index >= MAX_CROSS_BOSS_PER_SCENE)
			{
				return;
			}

			if (0 == (m_param.cross_boss_concern_flag[layer - 1] & (1 << index)))
			{
				return;
			}

			m_param.cross_boss_concern_flag[layer - 1] &= (~(1 << index));
			this->SendCrossBossSelfInfo();
			m_role->NoticeNum(noticenum::NT_WORLD_BOSS_CONCERN_SUCC);
		}

		static crossgameprotocal::GameGameBossInfoReq ggbir;

		ggbir.cross_boss_type = crossgameprotocal::CROSS_BOSS_TYPE_CROSS;
		ggbir.uuid = UniqueUserIDToLongLong(this->GetCrossOriginalUniqueUserID());
		ggbir.req_type = opera_type;
		ggbir.param_1 = layer;
		ggbir.param_2 = param2;

		InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_HIDDEN, 0, (const char *)&ggbir, sizeof(ggbir));
		return;
	}

	if (NULL == m_role->GetScene())
	{
		return;
	}

	if (m_role->GetScene()->GetSceneType() != Scene::SCENE_TYPE_CROSS_BOSS)
	{
		return;
	}

	const CrossBossLayerCfg *layer_cfg = CrossBossConfig::Instance().GetCrossBossLayerCfg(layer);
	if (NULL == layer_cfg)
	{
		return;
	}

	SpecialCrossBoss *cross_boss = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossBoss(layer_cfg->scene_id, CROSS_PUBLIC_SCENE_KEY);
	if (NULL == cross_boss)
	{
		return;
	}

	switch (opera_type)
	{
	case Protocol::CROSS_BOSS_OPERA_TYPE_GET_FLUSH_INFO:
	{
		cross_boss->SyncSceneInfo(m_role);
	}
	break;

	case Protocol::CROSS_BOSS_OPERA_TYPE_BOSS_KILL_RECORD:
	{
		cross_boss->SendBossKillerRecordInfo(m_role, param2);
	}
	break;

	case Protocol::CROSS_BOSS_OPERA_TYPE_DROP_RECORD:
	{
		cross_boss->SendDropRecordInfo(m_role);
	}
	break;

	case Protocol::CROSS_BOSS_OPERA_TYPE_CONCERN_BOSS:
	{
		int index = CrossBossConfig::Instance().GetIndexByBossId(layer, param2);
		if (index < 0 || index >= MAX_CROSS_BOSS_PER_SCENE)
		{
			return;
		}

		if (0 != (m_param.cross_boss_concern_flag[layer - 1] & (1 << index)))
		{
			return;
		}


		if (!cross_boss->ConcernBoss(UniqueUserIDToLongLong(m_role->GetUniqueUserID()), param2))
		{
			return;
		}

		m_param.cross_boss_concern_flag[layer - 1] |= (1 << index);
		cross_boss->SendInfoToRole(m_role);
		m_role->NoticeNum(noticenum::NT_WORLD_BOSS_CONCERN_SUCC);
	}
	break;

	case Protocol::CROSS_BOSS_OPERA_TYPE_UNCONCERN_BOSS:
	{
		int index = CrossBossConfig::Instance().GetIndexByBossId(layer, param2);
		if (index < 0 || index >= MAX_CROSS_BOSS_PER_SCENE)
		{
			return;
		}

		if (0 == (m_param.cross_boss_concern_flag[layer - 1] & (1 << index)))
		{
			return;
		}

		if (!cross_boss->UnConcernBoss(UniqueUserIDToLongLong(m_role->GetUniqueUserID()), param2))
		{
			return;
		}

		m_param.cross_boss_concern_flag[layer - 1] &= ~(1 << index);
		cross_boss->SendInfoToRole(m_role);
		m_role->NoticeNum(noticenum::NT_WORLD_BOSS_CONCERN_SUCC);
	}

	default:
		break;
	}
}

void RoleCross::OnEnterCrossBossScene(int scene_level, short move_to_pos_x /* = 0 */, short move_to_pos_y /* = 0 */)
{
	if (ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_CROSS_BOSS))
	{
		return;
	}

	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetActivityJoinLimit(CROSS_ACTIVITY_TYPE_CROSS_BOSS, &limit_unit);

	if (is_limit && m_role->GetLevel() < limit_unit.level)
	{
		m_role->NoticeNum(errornum::EN_CROSS_ACTIVITY_LEVEL_LIMIT);
		return;
	}

	if (is_limit && m_role->GetLevel() > limit_unit.max_level)
	{
		m_role->NoticeNum(errornum::EN_CROSS_ACTIVITY_LEVEL_MAX_LIMIT);
		return;
	}

	int level_limit = CrossBossConfig::Instance().GetLevelLimitBySceneLevel(scene_level);
	if (level_limit <= 0 || m_role->GetLevel() < level_limit)
	{
		m_role->NoticeNum(errornum::EN_SCENE_TRANSPORT_LEVEL_LIMIT);
		return;
	}

	//unsigned int now_time = (unsigned int)(EngineAdapter::Instance().Time());
	//if (!CrossBossConfig::Instance().CanEnterCrossBoss(now_time))
	//{
	//	gstr::SendError(m_role, "cross_boss_enter_limit");
	//	return;
	//}

	const CrossBossLayerCfg *layer_cfg = CrossBossConfig::Instance().GetCrossBossLayerCfg(scene_level);
	if (NULL == layer_cfg)
	{
		return;
	}

	m_param.cross_boss_scene_level = scene_level;

	this->OnStartCrossReq(CROSS_ACTIVITY_TYPE_CROSS_BOSS, false, 0, 0, 0, move_to_pos_x, move_to_pos_y);

	gamelog::g_log_cross.printf(LL_INFO, "RoleCross OnEnterCrossBossScene user %d %s, scene_level %d",
		m_role->GetUID(), m_role->GetName(), m_param.cross_boss_scene_level);

	{
		// 传闻
		int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_boss_enter_notice, 
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), layer_cfg->scene_id);
		if (sendlen > 0) 
		{
			//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE);
		}
	}
}

bool RoleCross::AddCrossBossReliveTimes(int add_times, const char *reason, bool send_info)
{
	if (0 == add_times || NULL == reason)
	{
		return false;
	}

	int temp_relive_times = m_param.cross_boss_relive_times + add_times;
	if (temp_relive_times < 0 || temp_relive_times > SHRT_MAX)
	{
		return false;
	}

	m_param.cross_boss_relive_times = static_cast<short>(temp_relive_times);

	if (send_info)
	{
		this->SendCrossBossSelfInfo();
	}

	gamelog::g_log_cross.printf(LL_INFO, "AddCrossBossReliveTimes user[%d,%s] cur_relive_times[%d] add_times[%d] reason[%s]",
		m_role->GetUID(), m_role->GetName(), m_param.cross_boss_relive_times, add_times, reason);

	return true;
}

void RoleCross::OnCrossBossBuyReliveTimes(int buy_times)
{
	if (buy_times <= 0)
	{
		return;
	}

	if (m_param.cross_boss_relive_times + buy_times > SHRT_MAX)
	{
		buy_times = SHRT_MAX - m_param.cross_boss_relive_times;
	}

	int need_comsume_gold = buy_times * CrossBossConfig::Instance().GetOtherCfg().buy_relive_times_need_gold;
	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_comsume_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 消耗
	m_role->GetKnapsack()->GetMoney()->UseGold(need_comsume_gold, "OnCrossBossBuyReliveTimes");

	// 给予
	m_param.cross_boss_relive_times += buy_times;

	this->SendCrossBossSelfInfo();
	m_role->NoticeNum(noticenum::NT_CROSS_BOSS_BUY_RELIVE_TIME_SUCC);

	gamelog::g_log_cross.printf(LL_INFO, "OnCrossBossBuyReliveTimes user[%d,%s] buy_times[%d] need_comsume_gold[%d] cur_relive_times[%d]",
		m_role->GetUID(), m_role->GetName(), buy_times, need_comsume_gold, m_param.cross_boss_relive_times);
}

void RoleCross::SyncCrossCommonRecordInfo()
{
	CrossRecord *cross_record = CrossRecordManager::Instance().GetCrossRecord(m_role->GetUserId());
	if (NULL == cross_record || cross_record->user_record_data.cross_common_need_sync_to_role <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(cross_record->user_record_data.cross_common_use_gold, "SyncCrossCommonRecordInfo"))
	{
		// 消耗失败，输出日志

		gamelog::g_log_cross.printf(LL_INFO, "SyncCrossCommonRecordInfo UseGold Fail user[%d,%s] use_gold[%d]",
			m_role->GetUID(), m_role->GetName(), cross_record->user_record_data.cross_common_use_gold);
	}

	cross_record->user_record_data.cross_common_need_sync_to_role = 0;
	CrossRecordManager::Instance().SaveCrossRecord(cross_record, structcommon::CS_UPDATE);
}

void RoleCross::SyncCrossCommonAttendRecordInfo()
{
	CrossRecord *cross_record = CrossRecordManager::Instance().GetCrossRecord(m_role->GetUserId());
	if (NULL == cross_record || cross_record->user_record_data.cross_attend_need_sync_to_role <= 0)
	{
		return;
	}

	//EventHandler::Instance().OnFinishHighMission(m_role, 0, cross_record->user_record_data.attend_activity_num);

	cross_record->user_record_data.cross_attend_need_sync_to_role = 0;
	cross_record->user_record_data.attend_activity_num = 0;
}

void RoleCross::OnCrossTeamFBOption(int option_type, int layer, int room, int param, int param2, int param3)
{
	if (CROSS_TEAM_FB_STATE_OPTION_CREATE == option_type ||
		CROSS_TEAM_FB_STATE_OPTION_JOIN == option_type)
	{
		if (!m_role->GetRoleStatusManager()->CanEnterFB())
		{
			return;
		}

		const CrossTeamFBLayerCfg *cfg = LOGIC_CONFIG->GetCrossTeamFBConfig().GetCrossTeamFBLayerCfg(layer);
		if (NULL == cfg) return;

		if (m_role->GetLevel() < cfg->level_limit)
		{
			m_role->NoticeNum(errornum::EN_CROSS_TEAM_LEVEL_LIMIT);
			return;
		}
	}

	int temp_state = CROSS_TEAM_FB_STATE_TYPE_READY;
	if (CROSS_TEAM_FB_STATE_OPTION_JOIN == option_type && param2 >= CROSS_TEAM_FB_STATE_TYPE_READY && param2 <= CROSS_TEAM_FB_STATE_TYPE_START)
	{
		temp_state = param2;
	}

	static crossgameprotocal::CrossTeamFBOptionReq qctfb;
	qctfb.option_type = option_type;
	qctfb.user_info.uuid = UniqueUserIDToLongLong(m_role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	qctfb.user_info.prof = m_role->GetProf();
	qctfb.user_info.sex = m_role->GetSex();
	qctfb.user_info.camp = m_role->GetCamp();
	qctfb.user_info.user_state = temp_state;
	F_STRNCPY(qctfb.user_info.name, m_role->GetName(), sizeof(GameName));
	qctfb.user_info.capability = m_role->GetCapability()->GetTotalCapability();
	qctfb.user_info.layer = layer;
	qctfb.user_info.room = room;
	qctfb.param = param;
	qctfb.param2 = param2;
	qctfb.param3 = param3;

	InternalComm::Instance().SendToCross((const char *)&qctfb, sizeof(qctfb), CROSS_ACTIVITY_TYPE_TEAM_FB);
}

void RoleCross::OnCrossTeamFBEnter()
{
	m_param.cross_team_fb_day_count++;
	this->SendCrossTeamRBInfo();
}

bool RoleCross::IsCrossTeamFBPass(int layer)
{
	return (m_param.cross_team_fb_pass_flag & (1 << layer)) != 0;
}

int RoleCross::GetCrossTeamFBDayCount()
{
	return m_param.cross_team_fb_day_count;
}

void RoleCross::OnCrossTeamFBPassLayer(int layer)
{
	m_param.cross_team_fb_pass_flag |= (1 << layer);
	this->SendCrossTeamRBInfo();
}

void RoleCross::SendCrossTeamRBInfo()
{
	Protocol::SCCrossTeamFBSelfInfo pro;
	pro.cross_team_fb_pass_flag = m_param.cross_team_fb_pass_flag;
	pro.cross_team_fb_day_count = m_param.cross_team_fb_day_count;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pro, sizeof(pro));
}

void RoleCross::CheckCrossGuildBattleClacTime()
{
	if (m_role->IsInCross())
	{
		return;
	}

	int cross_guildbattle_reward_calc_time = WorldStatus::Instance().GetCrossGuildBattleRewardCalctime();
	if (cross_guildbattle_reward_calc_time > 0 && m_param.cross_guildbattle_recalc_time != cross_guildbattle_reward_calc_time)
	{
		m_param.cross_guildbattle_recalc_time = cross_guildbattle_reward_calc_time;
		m_param.cross_guildbattle_server_reward_flag = 0;
		m_param.cross_guildbattle_guild_reward_flag = 0;
		m_param.cross_guildbattle_daily_reward_flag = 0;
	}
}

void RoleCross::SyncCrossGuildBattleInfo()
{
	this->CheckCrossGuildBattleClacTime();

	static CrossGuildBattleGuildItem cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	WorldStatus::Instance().GetCrossGuildBattleInfo(cross_guildbattle_list);

	static CrossGuildBattleTuanzhangInfo cross_guildbattle_tuanzhang_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	WorldStatus::Instance().GetCrossGuildBattleTuanzhangInfo(cross_guildbattle_tuanzhang_info_list);

	static CrossGuildBattleApperanceInfo cross_guildbattle_apperance_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	WorldStatus::Instance().GetCrossGuildBattleApperanceInfo(cross_guildbattle_apperance_info_list);

	static const int PLAT_TYPE = LocalConfig::Instance().GetPlatType();
	const GuildID role_guild_id = m_role->GetGuildID();
	int guild_id = INVALID_GUILD_ID;
	int plat_type = 0;

	Protocol::SCCrossGuildBattleInfo cmd;
	cmd.server_reward_flag = m_param.cross_guildbattle_server_reward_flag;
	cmd.guild_reward_flag = m_param.cross_guildbattle_guild_reward_flag;
	cmd.daily_reward_flag = m_param.cross_guildbattle_daily_reward_flag;
	cmd.role_reward_flag = 0;
	if (INVALID_GUILD_ID != role_guild_id)
	{
		Guild *guild = GuildManager::Instance().GetGuild(role_guild_id);
		if (NULL != guild)
		{
			cmd.role_reward_flag = (guild->GetJoinGuildBattleFlag() == 1) ? 1 : 0;
		}
	}

	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
	{
		guild_id = cross_guildbattle_list[i].guild_id;
		plat_type = cross_guildbattle_list[i].plat_type;

		cmd.guild_list[i].guild_id = guild_id;
		cmd.guild_list[i].server_id = IntToUid(guild_id).db_index;
		cmd.guild_list[i].plat_type = plat_type;
		cmd.guild_list[i].is_our_guild = (role_guild_id == guild_id && INVALID_GUILD_ID != role_guild_id && PLAT_TYPE == plat_type) ? 1 : 0;
		F_STRNCPY(cmd.guild_list[i].guild_name, cross_guildbattle_list[i].guild_name, sizeof(GuildName));
		F_STRNCPY(cmd.guild_list[i].guild_tuanzhang_name, cross_guildbattle_list[i].guild_tuanzhang_name, sizeof(GameName));
		cmd.guild_list[i].prof = cross_guildbattle_tuanzhang_info_list[i].tuanzhang_prof;
		cmd.guild_list[i].sex = cross_guildbattle_tuanzhang_info_list[i].tuanzhang_sex;
		cmd.guild_list[i].wuqi_id = cross_guildbattle_apperance_info_list[i].wuqi_id;
		cmd.guild_list[i].shizhuang_wuqi = cross_guildbattle_apperance_info_list[i].shizhuang_wuqi;
		cmd.guild_list[i].shizhuang_wuqi_is_special = cross_guildbattle_apperance_info_list[i].shizhuang_wuqi_is_special;
		cmd.guild_list[i].shizhuang_body = cross_guildbattle_apperance_info_list[i].shizhuang_body;
		cmd.guild_list[i].shizhuang_body_is_special = cross_guildbattle_apperance_info_list[i].shizhuang_body_is_special;
		cmd.guild_list[i].halo_used_imageid = cross_guildbattle_apperance_info_list[i].halo_used_imageid;
		cmd.guild_list[i].toushi_used_imageid = cross_guildbattle_apperance_info_list[i].toushi_used_imageid;
		cmd.guild_list[i].qilinbi_used_imageid = cross_guildbattle_apperance_info_list[i].qilinbi_used_imageid;
		cmd.guild_list[i].mask_used_imageid = cross_guildbattle_apperance_info_list[i].mask_used_imageid;

	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

void RoleCross::OnFetchCrossGuildBattleReward(int reward_type, int index)
{
	return; // 奖励改成打完直接发了，不需要手动领取
	int cross_guildbattle_reward_calc_time = WorldStatus::Instance().GetCrossGuildBattleRewardCalctime();
	if (m_role->IsInCross() || cross_guildbattle_reward_calc_time <= 0)
	{
		return;
	}

	if ((0 != reward_type && 1 != reward_type) || index < 0 || index >= CROSS_GUILDBATTLE_MAX_SCENE_NUM)
	{
		return;
	}

	const CrossGuildBattleOwnRewardCfg *rewrd_cfg = CrossGuildBattleConfig::Instance().GetOwnServerRewardCfg(index);
	if (NULL == rewrd_cfg)
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (NULL == guild)
	{
		m_role->NoticeNum(errornum::EN_CROSS_GUILDBATTLE_REWARD_FETCH_LIMIT);
		return;
	}

	// 	GuildMember *member = guild->GetGuildMemberManager().GetMember(m_role->GetUserId());
	// 	if (NULL == member)
	// 	{
	// 		m_role->NoticeNum(errornum::EN_CROSS_GUILDBATTLE_REWARD_FETCH_LIMIT);
	// 		return;
	// 	}

	// 进帮派当天不能领取奖励
	// 	int join_days = GetDayIndex(static_cast<time_t>(member->GetJoinTime()), EngineAdapter::Instance().Time());
	// 	if (join_days <= 0)
	// 	{
	// 		m_role->NoticeNum(errornum::EN_CROSS_GUILD_BATTLE_REWARD_DAY_LIMIT);
	// 		return;
	// 	}

	this->CheckCrossGuildBattleClacTime();

	CrossGuildBattleGuildItem cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	WorldStatus::Instance().GetCrossGuildBattleInfo(cross_guildbattle_list);

	if (0 == reward_type) // 全服奖励
	{
		// 全服奖励只能领一次
		index = 0;

		if (0 != (m_param.cross_guildbattle_server_reward_flag & (1 << index)))
		{
			m_role->NoticeNum(errornum::EN_CROSS_GUILDBATTLE_REWARD_HAD_FETCHED);
			return;
		}

		if (guild->GetJoinGuildBattleFlag() <= 0)
		{
			m_role->NoticeNum(errornum::EN_CROSS_GUILDBATTLE_REWARD_FETCH_LIMIT);
			return;
		}

		GuildID role_guild_id = m_role->GetGuildID();
		if (role_guild_id <= 0)
		{
			m_role->NoticeNum(errornum::EN_CROSS_GUILDBATTLE_REWARD_FETCH_LIMIT);
			return;
		}

		// 占领过任何一个城池的帮派成员，不能领取参与奖
		for (int t_index = 0; t_index < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++t_index)
		{
			if (cross_guildbattle_list[t_index].guild_id > 0 && 
				cross_guildbattle_list[t_index].plat_type == LocalConfig::Instance().GetPlatType() &&
				role_guild_id == cross_guildbattle_list[t_index].guild_id)
			{
				m_role->NoticeNum(errornum::EN_CROSS_GUILDBATTLE_REWARD_FETCH_LIMIT);
				return;
			}
		}

		m_param.cross_guildbattle_server_reward_flag |= (1 << index);

		m_role->GetKnapsack()->PutListOrMail(rewrd_cfg->server_reward_item, PUT_REASON_CROSS_GUILDBATTLE_OWN);
	}
	else if (1 == reward_type) // 帮派奖励
	{
		if (0 != (m_param.cross_guildbattle_guild_reward_flag & (1 << index)))
		{
			m_role->NoticeNum(errornum::EN_CROSS_GUILDBATTLE_REWARD_HAD_FETCHED);
			return;
		}

		int count = 0;
		for (int seq = 0; seq < CROSS_GUILDBATTLE_MAX_SCENE_NUM; seq++)
		{
			if (0 != (m_param.cross_guildbattle_guild_reward_flag & (1 << seq)))
			{
				count++;
			}

			if (count >= 2) // 一个帮派最多只能占领2个城，故最多只能领2次
			{
				m_role->NoticeNum(errornum::EN_EXP_JADE_FETCH_TIME_LIMIT);
				return;
			}
		}

		if (m_role->GetGuildID() <= 0
			|| cross_guildbattle_list[index].guild_id <= 0
			|| cross_guildbattle_list[index].plat_type != LocalConfig::Instance().GetPlatType()
			|| m_role->GetGuildID() != cross_guildbattle_list[index].guild_id)
		{
			m_role->NoticeNum(errornum::EN_CROSS_GUILDBATTLE_REWARD_FETCH_LIMIT);
			return;
		}

		m_param.cross_guildbattle_guild_reward_flag |= (1 << index);
		m_role->GetKnapsack()->PutListOrMail(rewrd_cfg->guild_reward_item, PUT_REASON_CROSS_GUILDBATTLE_OWN);
	}

	this->SyncCrossGuildBattleInfo();

	gamelog::g_log_cross.printf(LL_INFO, "OnFetchCrossGuildBattleReward user[%d,%s] reward_type[%d] index[%d]",
		m_role->GetUID(), m_role->GetName(), reward_type, index);
}

void RoleCross::OnGetCrossGuildBattleDailyReward(int index)
{
	int cross_guildbattle_reward_calc_time = WorldStatus::Instance().GetCrossGuildBattleRewardCalctime();
	if (m_role->IsInCross() || cross_guildbattle_reward_calc_time <= 0)
	{
		return;
	}

	if (index < 0 || index >= CROSS_GUILDBATTLE_MAX_SCENE_NUM)
	{
		return;
	}
	const ItemConfigData * reward_item = CrossGuildBattleConfig::Instance().GetDailyDailyRewardItem(index);
	if (reward_item == nullptr)
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (NULL == guild)
	{
		m_role->NoticeNum(errornum::EN_CROSS_GUILDBATTLE_REWARD_FETCH_LIMIT);
		return;
	}

	this->CheckCrossGuildBattleClacTime();

	CrossGuildBattleGuildItem cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	WorldStatus::Instance().GetCrossGuildBattleInfo(cross_guildbattle_list);


	if (0 != (m_param.cross_guildbattle_daily_reward_flag & (1 << index)))
	{
		m_role->NoticeNum(errornum::EN_CROSS_GUILDBATTLE_REWARD_HAD_FETCHED);
		return;
	}

	int count = 0;
	for (int seq = 0; seq < CROSS_GUILDBATTLE_MAX_SCENE_NUM; seq++)
	{
		if (0 != (m_param.cross_guildbattle_daily_reward_flag & (1 << seq)))
		{
			count++;
		}

		if (count >= 2) // 一个帮派最多只能占领2个城，故最多只能领2次
		{
			m_role->NoticeNum(errornum::EN_EXP_JADE_FETCH_TIME_LIMIT);
			return;
		}
	}

	if (m_role->GetGuildID() <= 0
		|| cross_guildbattle_list[index].guild_id <= 0
		|| cross_guildbattle_list[index].plat_type != LocalConfig::Instance().GetPlatType()
		|| m_role->GetGuildID() != cross_guildbattle_list[index].guild_id)
	{
		m_role->NoticeNum(errornum::EN_CROSS_GUILDBATTLE_REWARD_FETCH_LIMIT);
		return;
	}
	
	m_param.cross_guildbattle_daily_reward_flag |= (1 << index);
	m_role->GetKnapsack()->PutListOrMail(reward_item, PUT_REASON_CROSS_GUILDBATTLE_OWN);

	this->SyncCrossGuildBattleInfo();

	gamelog::g_log_cross.printf(LL_INFO, "OnGetCrossGuildBattleDailyReward user[%d,%s] ",m_role->GetUID(), m_role->GetName());
}

void RoleCross::ReqCrossGuildBattleGetRankLog()
{
	static crossgameprotocal::GameCrossGetGuildBattleRankLogReq qcggbrl;
	qcggbrl.uni_user_id = this->m_role->GetUniqueUserID();

	InternalComm::Instance().SendToCross((const char *)&qcggbrl, sizeof(qcggbrl), CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE);
}

void RoleCross::RespCrossGuildBattleGetRankLog(CrossGuildBattleSceneLogInfo scene_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM])
{
	Protocol::SCCrossGuildBattleGetRankInfoResp cmd;
	memcpy(cmd.scene_list, scene_list, sizeof(cmd.scene_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

void RoleCross::SendGuildBattleTaskInfo()
{
	Protocol::SCCrossGuildBattleTaskInfo cmd;
	memcpy(cmd.task_finish_flag, m_param.cross_guildbattle_task_reward_flag, sizeof(cmd.task_finish_flag));
	memcpy(cmd.task_record, m_param.cross_guildbattle_task_record, sizeof(cmd.task_record));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

void RoleCross::SkipCrossGuildBattleTask()
{
	if (NULL == m_role) return;

	const CrossGuildBattleOtherCfg &other = CrossGuildBattleConfig::Instance().GetOtherCfg();
	if (m_role->GetLevel() < other.skip_task_level_limit)
	{
		return;
	}

	int task_count = 0;
	std::map<int, std::map<int, CrossGuildBattleTaskCfg> > &task_map = CrossGuildBattleConfig::Instance().GetGuildBattleTaskMap();
	for (std::map<int, std::map<int, CrossGuildBattleTaskCfg> >::iterator scene_iter = task_map.begin();
	scene_iter != task_map.end(); scene_iter++)
	{
		std::map<int, CrossGuildBattleTaskCfg> &task_cfg_map = scene_iter->second;
		for (std::map<int, CrossGuildBattleTaskCfg>::iterator task_iter = task_cfg_map.begin(); task_iter != task_cfg_map.end(); ++task_iter)
		{
			int scene_index = task_iter->second.task_index;
			int task_id = task_iter->second.task_id;
			if (0 == (m_param.cross_guildbattle_task_reward_flag[scene_index] & (1 << task_id)))
			{
				++task_count;
			}
		}
	}

	int consume = task_count * other.skip_task_consume;
	if (consume <= 0)
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(consume, "Skip CrossGuildBattle Task"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int add_score = 0;
	//std::map<int, std::map<int, CrossGuildBattleTaskCfg> > &task_map = CrossGuildBattleConfig::Instance().GetGuildBattleTaskMap();
	for (std::map<int, std::map<int, CrossGuildBattleTaskCfg> >::iterator scene_iter = task_map.begin();
			scene_iter != task_map.end(); scene_iter++)
	{
		std::map<int, CrossGuildBattleTaskCfg> &task_cfg_map = scene_iter->second;
		for (std::map<int, CrossGuildBattleTaskCfg>::iterator task_iter = task_cfg_map.begin(); task_iter != task_cfg_map.end(); ++task_iter)
		{
			int scene_index = task_iter->second.task_index;
			int task_id = task_iter->second.task_id;

			if (scene_index >= 0 && scene_index < CROSS_GUILDBATTLE_MAX_SCENE_NUM
				&& task_id >= 0 && task_id < CROSS_GUILDBATTLE_MAX_TASK_NUM)
			{
				if (0 == (m_param.cross_guildbattle_task_reward_flag[scene_index] & (1 << task_id)))
				{
					m_param.cross_guildbattle_task_reward_flag[scene_index] |= (1 << task_id);
					m_param.cross_guildbattle_task_record[scene_index][task_id] = task_iter->second.task_count;
					//add_score += task_iter->second.reward_credit;
					m_role->GetKnapsack()->PutListOrMail(task_iter->second.reward_item, PUT_REASON_CROSS_GUILDBATTLE_TASK);

					static CrossGuildBattleGuildItem cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
					memset(cross_guildbattle_list, 0, sizeof(cross_guildbattle_list));
					WorldStatus::Instance().GetCrossGuildBattleInfo(cross_guildbattle_list);
					if (cross_guildbattle_list[scene_index].guild_id > 0
						&& cross_guildbattle_list[scene_index].plat_type == m_cross_original_unique_user_id.plat_type
						&& m_role->GetGuildID() == cross_guildbattle_list[scene_index].guild_id)
					{
						//add_score += task_iter->second.extra_credit;
						m_role->GetKnapsack()->PutListOrMail(task_iter->second.extra_item, PUT_REASON_CROSS_GUILDBATTLE_TASK);
					}
				}
			}
		}
	}
	
	m_role->GetShenshou()->AddHunalingScore(add_score);
	this->SendGuildBattleTaskInfo();
}

bool RoleCross::CheckCanUpdateLieKunRank()
{
	if (ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_LIEKUN_FB))
	{
		return false;
	}

	return true;
}

bool RoleCross::OnEnterCrossLieKunFb(short zone, bool is_force)
{
	if (zone < LIEKUN_ZONE_TYPE_MAIN || zone >= LIEKUN_ZONE_TYPE_COUNT)
	{
		return false;
	}

	if (ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_LIEKUN_FB))
	{
		return false;
	}

	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (NULL == guild)
	{
		m_role->NoticeNum(errornum::EN_CROSS_GUILDBATTLE_GUILD_LIMIT);
		return false;
	}

 	const CrossLieKunFbOtherCfg &other_cfg = CrossLieKunFbConfig::Instance().GetOtherCfg();
// 等级限制统一配到参与限制表
// 	// 等级限制
// 	int role_limit_level = other_cfg.open_level;
// 	if (m_role->GetLevel() < role_limit_level)
// 	{

// 		m_role->NoticeNum(errornum::EN_CROSS_ACTIVITY_LEVEL_LIMIT);
// 		return false;
// 	}

	// 时间限制
	if (zone != LIEKUN_ZONE_TYPE_MAIN)	// 副区域
	{
		if (!is_force)
		{
			unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			const unsigned int begin_time = ActivityShadow::Instance().GetCrossActivityBeginTime(CROSS_ACTIVITY_TYPE_LIEKUN_FB);
			if (now_time - begin_time >= static_cast<unsigned int>(other_cfg.enter_time_limit_s))
			{
				gstr::SendError(m_role, "cross_liekunfb_enter_time_out");
				return false;
			}
		}
	}
	else
	{
		GuildID guild_id = guild->GetGuildID();

		bool is_enter = false;
		GuildID liekun_guild_id_list[LIEKUN_ZONE_TYPE_COUNT];
		WorldStatus2::Instance().GetCrossLieKunFBGuildId(liekun_guild_id_list);
		for (int i = LIEKUN_ZONE_TYPE_MINOR_1; i < LIEKUN_ZONE_TYPE_COUNT; ++i)
		{
			if (guild_id == liekun_guild_id_list[i])
			{
				is_enter = true;
				break;
			}
		}

		if (!is_enter)
		{
			gstr::SendError(m_role, "cross_liekunfb_guild_no_kill_boss");
			return false;
		}
	}

	m_param.cross_liekun_scene_zone = zone; // 设置要进入的区域

	return true;
}


//////////////////////////////////////////////////////////////////////////
// 
// 检查跨服掉线重连。不包括3V3，3v3目前是异步检测，需要先获取匹配状态
// 只检查一次，进入跨服不成功就算了
//
//////////////////////////////////////////////////////////////////////////
void RoleCross::CheckCanRetransferToCrossOnRoleLogin()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		ClearCrossLogoutRecord();
		return;
	}

	int activity_type = m_param.role_cross_logout_record.cross_activity_type;

	if (activity_type <= CROSS_ACTIVITY_TYPE_INVALID || activity_type >= CROSS_ACTIVITY_TYPE_RESERVED)
	{
		return;
	}

	if (!ActivityShadow::Instance().IsCrossActivityOpen(activity_type))
	{
		ClearCrossLogoutRecord();
		return;
	}

	if (1 != m_param.role_cross_logout_record.logout_flag)
	{
		ClearCrossLogoutRecord();
		return;
	}

	// 30秒超时
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now >= (m_param.role_cross_logout_record.logout_timestamp + CROSS_ROLE_LOGOUT_LIMIT_TIME_S))
	{
		gamelog::g_log_cross.printf(LL_WARNING, 
			"CheckCanRetransferToCrossOnRoleLogin timeout! Role(%d, %s), activity_type[%d], logout_time[%u]",
			m_role->GetUID(), m_role->GetName(), (int)activity_type, m_param.role_cross_logout_record.logout_timestamp);

		ClearCrossLogoutRecord();
		return;
	}

	// 尝试重进跨服
	switch (activity_type)
	{
	case CROSS_ACTIVITY_TYPE_LIEKUN_FB:  // 猎鲲副本
	{
		this->OnStartCrossReq(activity_type, true, (int)m_param.role_cross_logout_record.param);
		break;
	}

	default:
		break;
	}

	// 日志
	gamelog::g_log_cross.printf(LL_MAINTANCE, 
		"CheckCanRetransferToCrossOnRoleLogin try transfer! Role(%d, %s), corss_activity[%d], scene[%d,%d], param[%lld]",
		m_role->GetUID(), m_role->GetName(), 
		activity_type, m_param.role_cross_logout_record.sceneId, m_param.role_cross_logout_record.sceneKey, m_param.role_cross_logout_record.param);

	// 清除记录
	ClearCrossLogoutRecord();
}

bool RoleCross::IsTransferToCrossIng()
{
	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	return m_last_cross_req_time + CROSS_CONNECT_ASYN_TIME_OUT_MS / 1000 > now_second;
}

bool RoleCross::OnEnterCrossVipBoss(int enter_type, int scene_id, int boss_id)
{
	if (enter_type <= Protocol::BOSS_ENTER_TYPE_IVNALID || enter_type >= Protocol::BOSS_ENTER_TYPE_MAX)
	{
		return false;
	}

	if (m_role->GetRoleStatusManager()->IsInFB())
	{
		m_role->NoticeNum(errornum::EN_CAN_NOT_ENTER_BOSS);
		return false;
	}

	const BossFamilyEnterConditionCfg *enter_condition_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetEnterConditionInfo(m_role, scene_id);
	if (NULL == enter_condition_cfg) return false;

	if (!SceneRegister::Instance().CheckSceneLevelLimit(m_role, scene_id))
	{
		m_role->NoticeNum(errornum::EN_SCENE_TRANSPORT_LEVEL_LIMIT);
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();

	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[0].item_id = enter_condition_cfg->need_item_id;
	stuff_list[0].num = enter_condition_cfg->need_item_num;
	stuff_list[0].buyable = false;
	int use_gold = 0;

	Knapsack *knapsack = m_role->GetKnapsack();

	if (enter_condition_cfg->free_vip_level == 0 || m_role->GetVip()->GetVipLevel() < enter_condition_cfg->free_vip_level)
	{
		if (0 == enter_condition_cfg->need_item_id)
		{
			use_gold = enter_condition_cfg->cost_gold;
			if (use_gold > 0 && !knapsack->GetMoney()->AllGoldMoreThan(use_gold))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return false;
			}
		}
		else
		{
			if (0 == enter_condition_cfg->need_item_num || NULL == ITEMPOOL->GetItem(enter_condition_cfg->need_item_id))
			{
				m_role->NoticeNum(errornum::EN_VIP_LEVEL_NOT_ENOUGH);
				return false;
			}
			else
			{
				if (NULL == knapsack || knapsack->IsLock() || !ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
				{
					return false;
				}
			}
		}
	}

	switch (enter_type)
	{
	case Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY:
	{
		if (!Scene::IsBossFamilyScene(scene_id))
		{
			return false;
		}

		if (m_role->GetLevel() < enter_condition_cfg->min_lv || m_role->GetLevel() > enter_condition_cfg->max_lv) return false;

		const BossFamilyBossInfoCfg *boss_info_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyBossInfo(boss_id);
		if (nullptr == boss_info_cfg || boss_info_cfg->is_cross <= 0)
		{
			return false;
		}

		if (use_gold > 0 && !knapsack->GetMoney()->UseAllGold(use_gold, "EnterBossFamily"))
		{
			return false;
		}

		if (!ItemExtern::ConsumeItem(m_role, consume_list, "EnterBossFamily"))
		{
			return false;
		}
	}
	break;
	}

	m_param.cross_common_boss_enter_type = enter_type;
	m_param.cross_common_boss_enter_scene_id = scene_id;
	m_param.cross_common_boss_enter_boss_id = boss_id;

	return true;
}

bool RoleCross::GetCrossVipBossEnterInfo(Protocol::CSEnterBossFamily *ebf)
{
	if (nullptr == ebf) return false;

	ebf->enter_type = m_param.cross_common_boss_enter_type;
	ebf->scene_id = m_param.cross_common_boss_enter_scene_id;
	ebf->boss_id = m_param.cross_common_boss_enter_boss_id;

	return true;
}

void RoleCross::OnEnterCrossMizangBossScene(int scene_level, short move_to_pos_x /* = 0 */, short move_to_pos_y /* = 0 */)
{
	if (ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_CROSS_MIZANG_BOSS))
	{
		return;
	}

	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetActivityJoinLimit(CROSS_ACTIVITY_TYPE_CROSS_MIZANG_BOSS, &limit_unit);

	if (is_limit && m_role->GetLevel() < limit_unit.level)
	{
		m_role->NoticeNum(errornum::EN_CROSS_ACTIVITY_LEVEL_LIMIT);
		return;
	}

	if (is_limit && m_role->GetLevel() > limit_unit.max_level)
	{
		m_role->NoticeNum(errornum::EN_CROSS_ACTIVITY_LEVEL_MAX_LIMIT);
		return;
	}

	int level_limit = CrossMizangBossConfig::Instance().GetLevelLimitBySceneLevel(scene_level);
	if (level_limit <= 0 || m_role->GetLevel() < level_limit)
	{
		m_role->NoticeNum(errornum::EN_SCENE_TRANSPORT_LEVEL_LIMIT);
		return;
	}

	//unsigned int now_time = (unsigned int)(EngineAdapter::Instance().Time());
	//if (!CrossMizangBossConfig::Instance().CanEnterCrossBoss(now_time))
	//{
	//	gstr::SendError(m_role, "cross_boss_enter_limit");
	//	return;
	//}

	const CrossMizangBossLayerCfg *layer_cfg = CrossMizangBossConfig::Instance().GetCrossBossLayerCfg(scene_level);
	if (NULL == layer_cfg)
	{
		return;
	}

	m_param.cross_mizang_boss_scene_level = scene_level;

	this->OnStartCrossReq(CROSS_ACTIVITY_TYPE_CROSS_MIZANG_BOSS, false, 0, 0, 0, move_to_pos_x, move_to_pos_y);

	gamelog::g_log_cross.printf(LL_INFO, "RoleCross OnEnterCrossMizangBossScene user %d %s, scene_level %d",
		m_role->GetUID(), m_role->GetName(), m_param.cross_mizang_boss_scene_level);

	{
		// 传闻
		int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_boss_enter2_notice,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), layer_cfg->scene_id);
		if (sendlen > 0)
		{
			//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE);
		}
	}
}

void RoleCross::OnCrossMizangBossReq(int opera_type, int layer, int param2)
{
	if (opera_type < 0 || opera_type >= Protocol::CROSS_MIZANG_BOSS_OPERA_TYPE_MAX || layer > CROSS_MIZANG_BOSS_SCENE_MAX)
	{
		return;
	}

	if (opera_type != Protocol::CROSS_MIZANG_BOSS_OPERA_TYPE_GET_FLUSH_INFO && opera_type != Protocol::CROSS_MIZANG_BOSS_OPERA_TYPE_DROP_RECORD && layer <= 0)
	{
		return;
	}

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		if (opera_type == Protocol::CROSS_MIZANG_BOSS_OPERA_TYPE_CONCERN_BOSS)
		{
			int index = CrossMizangBossConfig::Instance().GetIndexByBossId(layer, param2);
			if (index < 0 || index >= MAX_CROSS_MIZANG_BOSS_PER_SCENE)
			{
				return;
			}

			if (0 != (m_param.cross_mizang_boss_concern_flag[layer - 1] & (1 << index)))
			{
				return;
			}

			m_param.cross_mizang_boss_concern_flag[layer - 1] |= (1 << index);
			this->SendCrossMizangBossSelfInfo();
			m_role->NoticeNum(noticenum::NT_WORLD_BOSS_CONCERN_SUCC);
		}
		else if (opera_type == Protocol::CROSS_MIZANG_BOSS_OPERA_TYPE_UNCONCERN_BOSS)
		{
			int index = CrossMizangBossConfig::Instance().GetIndexByBossId(layer, param2);
			if (index < 0 || index >= MAX_CROSS_MIZANG_BOSS_PER_SCENE)
			{
				return;
			}

			if (0 == (m_param.cross_mizang_boss_concern_flag[layer - 1] & (1 << index)))
			{
				return;
			}

			m_param.cross_mizang_boss_concern_flag[layer - 1] &= (~(1 << index));
			this->SendCrossMizangBossSelfInfo();
			m_role->NoticeNum(noticenum::NT_WORLD_BOSS_CONCERN_SUCC);
		}

		static crossgameprotocal::GameGameBossInfoReq ggbir;

		ggbir.cross_boss_type = crossgameprotocal::CROSS_BOSS_TYPE_MIZANG;
		ggbir.uuid = UniqueUserIDToLongLong(this->GetCrossOriginalUniqueUserID());
		ggbir.req_type = opera_type;
		ggbir.param_1 = layer;
		ggbir.param_2 = param2;
		InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_HIDDEN, 0, (const char *)&ggbir, sizeof(ggbir));
		return;
	}

	if (NULL == m_role->GetScene())
	{
		return;
	}

	if (m_role->GetScene()->GetSceneType() != Scene::SCENE_TYPE_CROSS_MIZANG_BOSS)
	{
		return;
	}

	const CrossMizangBossLayerCfg *layer_cfg = CrossMizangBossConfig::Instance().GetCrossBossLayerCfg(layer);
	if (NULL == layer_cfg)
	{
		return;
	}

	SpecialCrossMiZangBoss *cross_boss = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossMizangBoss(layer_cfg->scene_id, CROSS_PUBLIC_SCENE_KEY);
	if (NULL == cross_boss)
	{
		return;
	}

	switch (opera_type)
	{
	case Protocol::CROSS_MIZANG_BOSS_OPERA_TYPE_GET_FLUSH_INFO:
	{
		cross_boss->SyncSceneInfo(m_role);
	}
	break;

	case Protocol::CROSS_MIZANG_BOSS_OPERA_TYPE_BOSS_KILL_RECORD:
	{
		cross_boss->SendBossKillerRecordInfo(m_role, param2);
	}
	break;

	case Protocol::CROSS_MIZANG_BOSS_OPERA_TYPE_DROP_RECORD:
	{
		cross_boss->SendDropRecordInfo(m_role);
	}
	break;

	case Protocol::CROSS_MIZANG_BOSS_OPERA_TYPE_CONCERN_BOSS:
	{
		int index = CrossMizangBossConfig::Instance().GetIndexByBossId(layer, param2);
		if (index < 0 || index >= MAX_CROSS_MIZANG_BOSS_PER_SCENE)
		{
			return;
		}

		if (0 != (m_param.cross_mizang_boss_concern_flag[layer - 1] & (1 << index)))
		{
			return;
		}


		if (!cross_boss->ConcernBoss(UniqueUserIDToLongLong(m_role->GetUniqueUserID()), param2))
		{
			return;
		}

		m_param.cross_mizang_boss_concern_flag[layer - 1] |= (1 << index);
		cross_boss->SendInfoToRole(m_role);
		m_role->NoticeNum(noticenum::NT_WORLD_BOSS_CONCERN_SUCC);
	}
	break;

	case Protocol::CROSS_MIZANG_BOSS_OPERA_TYPE_UNCONCERN_BOSS:
	{
		int index = CrossMizangBossConfig::Instance().GetIndexByBossId(layer, param2);
		if (index < 0 || index >= MAX_CROSS_MIZANG_BOSS_PER_SCENE)
		{
			return;
		}

		if (0 == (m_param.cross_mizang_boss_concern_flag[layer - 1] & (1 << index)))
		{
			return;
		}

		if (!cross_boss->UnConcernBoss(UniqueUserIDToLongLong(m_role->GetUniqueUserID()), param2))
		{
			return;
		}

		m_param.cross_mizang_boss_concern_flag[layer - 1] &= ~(1 << index);
		cross_boss->SendInfoToRole(m_role);
		m_role->NoticeNum(noticenum::NT_WORLD_BOSS_CONCERN_SUCC);
	}

	default:
		break;
	}
}

bool RoleCross::AddCrossMizangBossReliveTimes(int add_times, const char *reason, bool send_info)
{
	if (0 == add_times || NULL == reason)
	{
		return false;
	}

	int temp_relive_times = m_param.cross_mizang_boss_relive_times + add_times;
	if (temp_relive_times < 0 || temp_relive_times > SHRT_MAX)
	{
		return false;
	}

	m_param.cross_mizang_boss_relive_times = static_cast<short>(temp_relive_times);

	if (send_info)
	{
		this->SendCrossMizangBossSelfInfo();
	}

	gamelog::g_log_cross.printf(LL_INFO, "AddCrossMizangBossReliveTimes user[%d,%s] cur_relive_times[%d] add_times[%d] reason[%s]",
		m_role->GetUID(), m_role->GetName(), m_param.cross_mizang_boss_relive_times, add_times, reason);

	return true;
}

bool RoleCross::OnCrossMizangBossDecTireValue(int tire_value)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		if (m_param.cross_mizang_boss_today_kill_boss_num < tire_value)
		{
			return false;
		}

		m_param.cross_mizang_boss_today_kill_boss_num -= tire_value;

		this->SendCrossMizangBossSelfInfo();
		return true;
	}
	else
	{
		if (NULL == m_role->GetScene())
		{
			return false;
		}

		if (m_role->GetScene()->GetSceneType() != Scene::SCENE_TYPE_CROSS_MIZANG_BOSS)
		{
			return false;
		}

		SpecialCrossMiZangBoss *cross_boss = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossMizangBoss(m_role->GetScene()->GetSceneID(), CROSS_PUBLIC_SCENE_KEY);
		if (NULL == cross_boss)
		{
			return false;
		}

		if (!cross_boss->DecCrossBossTire(m_role, tire_value))
		{
			return false;
		}

		return true;
	}

	return false;
}

void RoleCross::SetCrossMizangBossConcernFlag(unsigned int flag, int index)
{
	if (index < 0 || index >= CROSS_MIZANG_BOSS_SCENE_MAX)
	{
		return;
	}

	m_param.cross_mizang_boss_concern_flag[index] = flag;
}

void RoleCross::GetCrossMizangBossConcernFlag(unsigned int *flag)
{
	if (flag == nullptr)
	{
		return;
	}
	memcpy(flag, m_param.cross_mizang_boss_concern_flag, sizeof(m_param.cross_mizang_boss_concern_flag));
}

void RoleCross::SendCrossMizangBossSelfInfo()
{
	const CrossMizangBossOtherCfg &other_cfg = CrossMizangBossConfig::Instance().GetOtherCfg();
	int left_ordinary_crystal_gather_times = other_cfg.ordinary_crystal_gather_times - m_param.cross_mizang_boss_today_gather_ordinary_crystal_num;
	int left_treasure_crystal_gather_times = other_cfg.treasure_crystal_gather_times - m_param.cross_mizang_boss_today_gather_treasure_crystal_num;

	static Protocol::SCCrossMiZangBossPlayerInfo cbpi;
	cbpi.left_ordinary_crystal_gather_times = left_ordinary_crystal_gather_times > 0 ? left_ordinary_crystal_gather_times : 0;
	cbpi.left_treasure_crystal_gather_times = left_treasure_crystal_gather_times > 0 ? left_treasure_crystal_gather_times : 0;
	cbpi.left_can_kill_boss_num = m_param.cross_mizang_boss_today_kill_boss_num < 0 ? 0 : m_param.cross_mizang_boss_today_kill_boss_num;

	m_role->GetRoleCross()->GetCrossMizangBossConcernFlag(cbpi.concern_flag);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cbpi, sizeof(cbpi));
}

void RoleCross::SyncCrossMizangBossRecordInfo()
{
	CrossRecord *cross_record = CrossRecordManager::Instance().GetCrossRecord(m_role->GetUserId());
	if (NULL == cross_record || cross_record->user_record_data.cross_mizang_boss_need_sync_to_role <= 0)
	{
		return;
	}

	m_role->GetRoleCross()->AddCrossMizangBossReliveTimes(-cross_record->user_record_data.cross_mizang_boss_relive_times, "SyncCrossMizangBossRecordInfo", false);
	m_param.cross_mizang_boss_today_gather_ordinary_crystal_num = cross_record->user_record_data.cross_mizang_boss_today_gather_ordinary_crystal_times;
	m_param.cross_mizang_boss_today_gather_treasure_crystal_num = cross_record->user_record_data.cross_mizang_boss_today_gather_treasure_crystal_times;
	m_param.cross_mizang_boss_today_kill_boss_num = cross_record->user_record_data.cross_mizang_boss_tire_value;

	this->SendCrossMizangBossSelfInfo();

	cross_record->user_record_data.cross_mizang_boss_need_sync_to_role = 0;
	CrossRecordManager::Instance().SaveCrossRecord(cross_record, structcommon::CS_UPDATE);
}

void RoleCross::RegesiterCrossMizangBossConcernInfo()
{
	if (m_role->IsInCross())
	{
		return;
	}

	for (int i = 0; i < CROSS_MIZANG_BOSS_SCENE_MAX; i++)
	{
		for (int j = 0; j < MAX_CROSS_MIZANG_BOSS_PER_SCENE; j++)
		{
			if (0 != (m_param.cross_mizang_boss_concern_flag[i] & (1 << j)))
			{
				const CrossMizangBossBossCfg::BossCfg *boss_cfg = CrossMizangBossConfig::Instance().GetCrossBossBossCfgByLayer(i, j);
				if (NULL == boss_cfg)
				{
					continue;
				}

				static crossgameprotocal::GameGameBossInfoReq ggbir;

				ggbir.cross_boss_type = crossgameprotocal::CROSS_BOSS_TYPE_MIZANG;
				ggbir.uuid = UniqueUserIDToLongLong(this->GetCrossOriginalUniqueUserID());
				ggbir.req_type = Protocol::CROSS_MIZANG_BOSS_OPERA_TYPE_CONCERN_BOSS;
				ggbir.param_1 = i;
				ggbir.param_2 = boss_cfg->boss_id;

				InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_HIDDEN, 0, (const char *)&ggbir, sizeof(ggbir));
				return;
			}
		}
	}
}

void RoleCross::OnCrossYoumingBossReq(int opera_type, int layer, int param2)
{
	if (opera_type < 0 || opera_type >= Protocol::CROSS_YOUMING_BOSS_OPERA_TYPE_MAX || layer > CROSS_YOUMING_BOSS_SCENE_MAX)
	{
		return;
	}

	if (opera_type != Protocol::CROSS_YOUMING_BOSS_OPERA_TYPE_GET_FLUSH_INFO && opera_type != Protocol::CROSS_YOUMING_BOSS_OPERA_TYPE_DROP_RECORD && layer <= 0)
	{
		return;
	}

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		if (opera_type == Protocol::CROSS_YOUMING_BOSS_OPERA_TYPE_CONCERN_BOSS)
		{
			int index = CrossYoumingBossConfig::Instance().GetIndexByBossId(layer, param2);
			if (index < 0 || index >= MAX_CROSS_YOUMING_BOSS_PER_SCENE)
			{
				return;
			}

			if (0 != (m_param.cross_youming_boss_concern_flag[layer - 1] & (1 << index)))
			{
				return;
			}

			m_param.cross_youming_boss_concern_flag[layer - 1] |= (1 << index);
			this->SendCrossYoumingBossSelfInfo();
			m_role->NoticeNum(noticenum::NT_WORLD_BOSS_CONCERN_SUCC);
		}
		else if (opera_type == Protocol::CROSS_YOUMING_BOSS_OPERA_TYPE_UNCONCERN_BOSS)
		{
			int index = CrossYoumingBossConfig::Instance().GetIndexByBossId(layer, param2);
			if (index < 0 || index >= MAX_CROSS_YOUMING_BOSS_PER_SCENE)
			{
				return;
			}

			if (0 == (m_param.cross_youming_boss_concern_flag[layer - 1] & (1 << index)))
			{
				return;
			}

			m_param.cross_youming_boss_concern_flag[layer - 1] &= (~(1 << index));
			this->SendCrossYoumingBossSelfInfo();
			m_role->NoticeNum(noticenum::NT_WORLD_BOSS_CONCERN_SUCC);
		}

		static crossgameprotocal::GameGameBossInfoReq ggbir;

		ggbir.cross_boss_type = crossgameprotocal::CROSS_BOSS_TYPE_YOUMING;
		ggbir.uuid = UniqueUserIDToLongLong(this->GetCrossOriginalUniqueUserID());
		ggbir.req_type = opera_type;
		ggbir.param_1 = layer;
		ggbir.param_2 = param2;
		InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_HIDDEN, 0, (const char *)&ggbir, sizeof(ggbir));
		return;
	}

	if (NULL == m_role->GetScene())
	{
		return;
	}

	if (m_role->GetScene()->GetSceneType() != Scene::SCENE_TYPE_CROSS_YOUMING_BOSS)
	{
		return;
	}

	const CrossYoumingBossLayerCfg *layer_cfg = CrossYoumingBossConfig::Instance().GetCrossBossLayerCfg(layer);
	if (NULL == layer_cfg)
	{
		return;
	}

	SpecialCrossYouMingBoss *cross_boss = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossYoumingBoss(layer_cfg->scene_id, CROSS_PUBLIC_SCENE_KEY);
	if (NULL == cross_boss)
	{
		return;
	}

	switch (opera_type)
	{
	case Protocol::CROSS_YOUMING_BOSS_OPERA_TYPE_GET_FLUSH_INFO:
	{
		cross_boss->SyncSceneInfo(m_role);
	}
	break;

	case Protocol::CROSS_YOUMING_BOSS_OPERA_TYPE_BOSS_KILL_RECORD:
	{
		cross_boss->SendBossKillerRecordInfo(m_role, param2);
	}
	break;

	case Protocol::CROSS_YOUMING_BOSS_OPERA_TYPE_DROP_RECORD:
	{
		cross_boss->SendDropRecordInfo(m_role);
	}
	break;

	case Protocol::CROSS_YOUMING_BOSS_OPERA_TYPE_CONCERN_BOSS:
	{
		int index = CrossYoumingBossConfig::Instance().GetIndexByBossId(layer, param2);
		if (index < 0 || index >= MAX_CROSS_YOUMING_BOSS_PER_SCENE)
		{
			return;
		}

		if (0 != (m_param.cross_youming_boss_concern_flag[layer - 1] & (1 << index)))
		{
			return;
		}


		if (!cross_boss->ConcernBoss(UniqueUserIDToLongLong(m_role->GetUniqueUserID()), param2))
		{
			return;
		}

		m_param.cross_youming_boss_concern_flag[layer - 1] |= (1 << index);
		cross_boss->SendInfoToRole(m_role);
		m_role->NoticeNum(noticenum::NT_WORLD_BOSS_CONCERN_SUCC);
	}
	break;

	case Protocol::CROSS_YOUMING_BOSS_OPERA_TYPE_UNCONCERN_BOSS:
	{
		int index = CrossYoumingBossConfig::Instance().GetIndexByBossId(layer, param2);
		if (index < 0 || index >= MAX_CROSS_YOUMING_BOSS_PER_SCENE)
		{
			return;
		}

		if (0 == (m_param.cross_youming_boss_concern_flag[layer - 1] & (1 << index)))
		{
			return;
		}

		if (!cross_boss->UnConcernBoss(UniqueUserIDToLongLong(m_role->GetUniqueUserID()), param2))
		{
			return;
		}

		m_param.cross_youming_boss_concern_flag[layer - 1] &= ~(1 << index);
		cross_boss->SendInfoToRole(m_role);
		m_role->NoticeNum(noticenum::NT_WORLD_BOSS_CONCERN_SUCC);
	}

	default:
		break;
	}
}

void RoleCross::OnEnterCrossYoumingBossScene(int scene_level, short move_to_pos_x /* = 0 */, short move_to_pos_y /* = 0 */)
{
	if (ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_CROSS_YOUMING_BOSS))
	{
		return;
	}

	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetActivityJoinLimit(CROSS_ACTIVITY_TYPE_CROSS_YOUMING_BOSS, &limit_unit);

	if (is_limit && m_role->GetLevel() < limit_unit.level)
	{
		m_role->NoticeNum(errornum::EN_CROSS_ACTIVITY_LEVEL_LIMIT);
		return;
	}

	if (is_limit && m_role->GetLevel() > limit_unit.max_level)
	{
		m_role->NoticeNum(errornum::EN_CROSS_ACTIVITY_LEVEL_MAX_LIMIT);
		return;
	}

	int level_limit = CrossYoumingBossConfig::Instance().GetLevelLimitBySceneLevel(scene_level);
	if (level_limit <= 0 || m_role->GetLevel() < level_limit)
	{
		m_role->NoticeNum(errornum::EN_SCENE_TRANSPORT_LEVEL_LIMIT);
		return;
	}

	/*unsigned int now_time = (unsigned int)(EngineAdapter::Instance().Time());
	if (!CrossYoumingBossConfig::Instance().CanEnterCrossBoss(now_time))
	{
		gstr::SendError(m_role, "cross_boss_enter_limit");
		return;
	}*/

	const CrossYoumingBossLayerCfg *layer_cfg = CrossYoumingBossConfig::Instance().GetCrossBossLayerCfg(scene_level);
	if (NULL == layer_cfg)
	{
		return;
	}

	m_param.cross_youming_boss_scene_level = scene_level;

	this->OnStartCrossReq(CROSS_ACTIVITY_TYPE_CROSS_YOUMING_BOSS, false, 0, 0, 0, move_to_pos_x, move_to_pos_y);

	gamelog::g_log_cross.printf(LL_INFO, "RoleCross OnEnterCrossYoumingBossScene user %d %s, scene_level %d",
		m_role->GetUID(), m_role->GetName(), m_param.cross_youming_boss_scene_level);

	{
		// 传闻
		int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_boss_enter3_notice,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), layer_cfg->scene_id);
		if (sendlen > 0)
		{
			//World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE);
		}
	}
}

bool RoleCross::AddCrossYoumingBossReliveTimes(int add_times, const char *reason, bool send_info)
{
	if (0 == add_times || NULL == reason)
	{
		return false;
	}

	int temp_relive_times = m_param.cross_youming_boss_relive_times + add_times;
	if (temp_relive_times < 0 || temp_relive_times > SHRT_MAX)
	{
		return false;
	}

	m_param.cross_youming_boss_relive_times = static_cast<short>(temp_relive_times);

	if (send_info)
	{
		this->SendCrossYoumingBossSelfInfo();
	}

	gamelog::g_log_cross.printf(LL_INFO, "AddCrossYoumingBossReliveTimes user[%d,%s] cur_relive_times[%d] add_times[%d] reason[%s]",
		m_role->GetUID(), m_role->GetName(), m_param.cross_youming_boss_relive_times, add_times, reason);

	return true;
}

bool RoleCross::OnCrossYoumingBossDecTireValue(int tire_value)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		if (m_param.cross_youming_boss_today_kill_boss_num < tire_value)
		{
			return false;
		}

		m_param.cross_youming_boss_today_kill_boss_num -= tire_value;

		this->SendCrossYoumingBossSelfInfo();
		return true;
	}
	else
	{
		if (NULL == m_role->GetScene())
		{
			return false;
		}

		if (m_role->GetScene()->GetSceneType() != Scene::SCENE_TYPE_CROSS_YOUMING_BOSS)
		{
			return false;
		}

		SpecialCrossYouMingBoss *cross_boss = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossYoumingBoss(m_role->GetScene()->GetSceneID(), CROSS_PUBLIC_SCENE_KEY);
		if (NULL == cross_boss)
		{
			return false;
		}

		if (!cross_boss->DecCrossBossTire(m_role, tire_value))
		{
			return false;
		}

		return true;
	}

	return false;
}

void RoleCross::SetCrossYoumingBossConcernFlag(unsigned int flag, int index)
{
	if (index < 0 || index >= CROSS_YOUMING_BOSS_SCENE_MAX)
	{
		return;
	}

	m_param.cross_youming_boss_concern_flag[index] = flag;
}

void RoleCross::GetCrossYoumingBossConcernFlag(unsigned int *flag)
{
	if (flag == nullptr)
	{
		return;
	}
	memcpy(flag, m_param.cross_youming_boss_concern_flag, sizeof(m_param.cross_youming_boss_concern_flag));
}

void RoleCross::SendCrossYoumingBossSelfInfo()
{
	const CrossYoumingBossOtherCfg &other_cfg = CrossYoumingBossConfig::Instance().GetOtherCfg();
	int left_ordinary_crystal_gather_times = other_cfg.ordinary_crystal_gather_times - m_param.cross_youming_boss_today_gather_ordinary_crystal_num;
	int left_treasure_crystal_gather_times = other_cfg.treasure_crystal_gather_times - m_param.cross_youming_boss_today_gather_treasure_crystal_num;

	static Protocol::SCCrossYouMingBossPlayerInfo cbpi;
	cbpi.left_ordinary_crystal_gather_times = left_ordinary_crystal_gather_times > 0 ? left_ordinary_crystal_gather_times : 0;
	cbpi.left_treasure_crystal_gather_times = left_treasure_crystal_gather_times > 0 ? left_treasure_crystal_gather_times : 0;
	cbpi.left_can_kill_boss_num = m_param.cross_youming_boss_today_kill_boss_num < 0 ? 0 : m_param.cross_youming_boss_today_kill_boss_num;

	m_role->GetRoleCross()->GetCrossYoumingBossConcernFlag(cbpi.concern_flag);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cbpi, sizeof(cbpi));
}

void RoleCross::SyncCrossYoumingBossRecordInfo()
{
	CrossRecord *cross_record = CrossRecordManager::Instance().GetCrossRecord(m_role->GetUserId());
	if (NULL == cross_record || cross_record->user_record_data.cross_youming_boss_need_sync_to_role <= 0)
	{
		return;
	}

	m_role->GetRoleCross()->AddCrossYoumingBossReliveTimes(-cross_record->user_record_data.cross_youming_boss_relive_times, "SyncCrossYoumingBossRecordInfo", false);
	m_param.cross_youming_boss_today_gather_ordinary_crystal_num = cross_record->user_record_data.cross_youming_boss_today_gather_ordinary_crystal_times;
	m_param.cross_youming_boss_today_gather_treasure_crystal_num = cross_record->user_record_data.cross_youming_boss_today_gather_treasure_crystal_times;
	m_param.cross_youming_boss_today_kill_boss_num = cross_record->user_record_data.cross_youming_boss_tire_value;

	this->SendCrossYoumingBossSelfInfo();

	cross_record->user_record_data.cross_youming_boss_need_sync_to_role = 0;
	CrossRecordManager::Instance().SaveCrossRecord(cross_record, structcommon::CS_UPDATE);
}

void RoleCross::RegesiterCrossYoumingBossConcernInfo()
{
	if (m_role->IsInCross())
	{
		return;
	}

	for (int i = 0; i < CROSS_YOUMING_BOSS_SCENE_MAX; i++)
	{
		for (int j = 0; j < MAX_CROSS_YOUMING_BOSS_PER_SCENE; j++)
		{
			if (0 != (m_param.cross_youming_boss_concern_flag[i] & (1 << j)))
			{
				const CrossYoumingBossBossCfg::BossCfg *boss_cfg = CrossYoumingBossConfig::Instance().GetCrossBossBossCfgByLayer(i, j);
				if (NULL == boss_cfg)
				{
					continue;
				}

				static crossgameprotocal::GameGameBossInfoReq ggbir;

				ggbir.cross_boss_type = crossgameprotocal::CROSS_BOSS_TYPE_YOUMING;
				ggbir.uuid = UniqueUserIDToLongLong(this->GetCrossOriginalUniqueUserID());
				ggbir.req_type = Protocol::CROSS_YOUMING_BOSS_OPERA_TYPE_CONCERN_BOSS;
				ggbir.param_1 = i;
				ggbir.param_2 = boss_cfg->boss_id;

				InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_HIDDEN, 0, (const char *)&ggbir, sizeof(ggbir));
				return;
			}
		}
	}
}

void RoleCross::OnCrossConsumeGold(int consume_gold_num)
{
	if (consume_gold_num <= 0) return;

	m_param.cross_consume_gold_num += consume_gold_num;
	m_param.cross_daily_consume_gold_num += consume_gold_num;
}

bool RoleCross::OnEnterCrossBianJingZhiDi(bool is_force)
{
	if (ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI))
	{
		return false;
	}

	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (NULL == guild)
	{
		m_role->NoticeNum(errornum::EN_CROSS_GUILDBATTLE_GUILD_LIMIT);
		return false;
	}

	return true;
}


