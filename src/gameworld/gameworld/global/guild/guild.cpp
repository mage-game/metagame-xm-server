#include "guild.hpp"

#include "engineadapter.h"
#include "global/rmibackobjdef.h"
#include "internalcomm.h"
#include <algorithm>

#include "guildmanager.hpp"
#include "global/rank/rankmanager.hpp"

#include "servercommon/string/globalstr.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/serverconfig/localconfig.hpp"

#include "obj/character/role.h"
#include "world.h"
#include "gamecommon.h"
#include "gamelog.h"
#include "gstr.h"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/guildbonfireconfig.hpp"
#include "global/bossfamily/bossfamilyconfig.hpp"

#include "scene/activityshadow/activityshadow.hpp"

#include "other/route/guildroute.hpp"
#include "other/route/mailroute.hpp"
#include "protocal/msgguild.hpp"
#include "config/guildactivityconfig.hpp"
#include "servercommon/string/gameworldstr.h"

#include "scene/speciallogic/guildspecial/specialguildstation.hpp"
#include "scene/speciallogic/guildspecial/specialguildboss.hpp"

#include "effect/skilleffect/effectpercentbuff.hpp"

#include "global/guild/impl/guildauthority.hpp"
#include "global/usercache/usercache.hpp"
#include "global/activity/activitymanager.hpp"
#include "protocal/msgchatmsg.h"
#include "servercommon/userprotocal/chatmsgcode.h"
#include "config/activityconfig/questionconfig.hpp"
#include "config/activityconfig/guildfbconfig.hpp"
#include "roleguild.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "item/knapsack.h"
#include "other/vip/vip.hpp"
#include "other/sublock/sublock.hpp"
#include "other/tuhaojin/tuhaojin.hpp"
#include "other/bigchatface/bigchatface.hpp"
#include "other/personalizewindow/personalizewindow.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "servercommon/serverconfig/crossguildbattleconfig.hpp"
#include "scene/guildshadow/guildshadow.hpp"
#include "other/itemlevelreward/itemlevelreward.hpp"
#include "other/event/eventhandler.hpp"
#include "other/cross/rolecross.h"
#include "servercommon/serverconfig/openserverconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "other/crossgoal/crossgoal.hpp"
#include "global/cross/crossmanager.h"
#include "servercommon/serverconfig/crossrankrewardconfig.hpp"

Guild::Guild(GuildManager *guild_manager)
	: m_guild_manager(guild_manager), m_guild_id(0), m_camp(0), m_create_time(0), m_guild_level(0), m_guild_exp(0), m_guild_last_uplevel_timestamp(0),
	m_last_update_dayid(0), m_last_active_time(0), m_next_get_cabablity_time(0), m_next_clear_member_time(0), m_next_sync_to_cross_time(0), m_warning_dismiss_days(0), m_applyfor_setup(GUILD_APPLYFOR_SETUP_NEED_AGREE),
	m_applyfor_need_capability(0), m_applyfor_need_level(0), m_auto_kickout_setup(0), m_callin_times(0),
	m_is_change(false), m_next_save_time(0), m_next_check_auto_kickout_time(0),
	m_guild_max_capability(0), m_rank(0), m_monster_finish_seq(0), m_active_degree(0),
	m_guild_fb_finish_timestamp(0), m_guild_beast_finish_timestamp(0), m_guild_bonfire_finish_timestamp(0),
	m_question_id(0), m_question_answer(-1), m_question_cur_begin(0), m_question_cur_end(0), m_question_next_begin(0), m_question_cur_right_num(0), m_open_biaoche_role_id(0),
	m_today_biaoche_isstart(0), m_today_rob_count(0), m_biaoche_status(gglobalprotocal::YUNBIAO_STATUS_INVALID), m_biaoche_pos(0, 0),
	m_guild_tianci_tongbi_day_count(0), m_guild_tianci_tongbi_is_open(0), m_today_kill_cross_boss_num(0),
	m_cross_goal_kill_cross_boss(0),m_cross_goal_kill_baizhan_boss(0),m_cross_goal_guild_flag(0), m_enemy_list_dirty(false), m_today_cross_goal_guild_notify(false)
{
	memset(m_guild_name, 0, sizeof(m_guild_name));
	memset(m_guild_notice, 0, sizeof(m_guild_notice));
	memset(m_monster_finish_username, 0, sizeof(m_monster_finish_username));
	memset(m_guild_red_paper_list, 0, sizeof(m_guild_red_paper_list));
	memset(m_question_fast_uid, 0, sizeof(m_question_fast_uid));

	m_apply_for_manager.SetGuild(this);
	m_member_manager.SetGuild(this);
	m_register.SetGuild(this);
	m_event_manager.SetGuild(this);
	m_guild_storge.SetGuild(this);
}

Guild::~Guild()
{
	
}

void Guild::Init(const AllGuildParam::GuildParam &guild_param)
{
	m_guild_id = guild_param.base_param.guild_id;
	m_camp = guild_param.base_param.camp;

	F_STRNCPY(m_guild_name, guild_param.base_param.guild_name, sizeof(m_guild_name));
	F_STRNCPY(m_guild_notice, guild_param.base_param.notice, sizeof(m_guild_notice));

	m_create_time = static_cast<unsigned int>(guild_param.base_param.create_time);
	m_guild_level = guild_param.base_param.guild_level;
	m_guild_exp = guild_param.base_param.guild_exp;
	m_guild_last_uplevel_timestamp = guild_param.base_param.guild_last_uplevel_timestamp;

	m_last_update_dayid = guild_param.base_param.last_update_dayid;
	m_last_active_time = guild_param.base_param.last_active_time;
	m_warning_dismiss_days = guild_param.base_param.warning_dismiss_days;
	m_applyfor_setup = guild_param.base_param.applyfor_setup;
	m_applyfor_need_capability = guild_param.base_param.applyfor_need_capability;
	m_applyfor_need_level = guild_param.base_param.applyfor_need_level;
	m_auto_kickout_setup = guild_param.base_param.auto_kickout_setup;
	m_guild_max_capability = guild_param.base_param.guild_max_capability;
	m_callin_times = guild_param.base_param.callin_times;
	m_monster_finish_seq = guild_param.base_param.monster_finish_seq;
	m_active_degree = guild_param.base_param.active_degree;
	F_STRNCPY(m_monster_finish_username, guild_param.base_param.monster_finish_username, sizeof(m_monster_finish_username));
	m_today_biaoche_isstart = guild_param.base_param.common_data_param.today_yunbiao_isstart;
	m_today_rob_count = guild_param.base_param.common_data_param.today_yunbiao_rob_count;

	m_common_data = guild_param.base_param.common_data_param;
	m_guild_storge.Init(guild_param.base_param);

	m_apply_for_manager.Init(guild_param.apply_for_param);
	m_member_manager.Init(guild_param.member_param);
	m_event_manager.Init(guild_param.event_param);

	this->InitGuildEnemy(&guild_param.base_param);

	m_guild_tianci_tongbi_day_count = m_common_data.guild_tianci_tongbi_day_count;
	m_today_kill_cross_boss_num = m_common_data.today_kill_cross_boss_num;

	m_cross_goal_kill_cross_boss = m_common_data.cross_goal_kill_cross_boss;
	m_cross_goal_kill_baizhan_boss = m_common_data.cross_goal_kill_baizhan_boss;
	m_cross_goal_guild_flag = m_common_data.corss_goal_guild_flag;

	m_next_save_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	m_next_send_saizi_rank_msg = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + GUILD_SEND_SAIZI_RANK_MSG_INTERVAL;

	if (0 == m_common_data.maze_layer_list[0])
	{
		this->ResetMaze();
	}

	this->RecalcMazeRank();
}

void Guild::OnWeekChange()
{
	
}

void Guild::OnDayChange()
{
	this->CheckMazeRankReward();
	m_member_manager.OnDayChange();
	m_callin_times = 0;

	m_monster_finish_seq = 0;
	memset(m_monster_finish_username, 0, sizeof(m_monster_finish_username));

	// 这里不要清理finishtime 因为他有可能是跨天的时候举行活动
	m_common_data.guild_fb_open_times = 0;
	this->SyncGuildFbStatus();

	m_common_data.guild_beast_today_open_times = 0;
	this->SyncGuildBeastStatus();

	m_common_data.guild_bonfire_open_times = 0;
	this->SyncGuildBonfireStatus();

	// 仙盟战力按自然天计算
	m_guild_max_capability = this->GetCapability();

	// 仙盟boss重置
	if (m_common_data.boss_normal_call_count > 0)
	{
		m_common_data.boss_level = 0;
		m_common_data.boss_exp = 0;
	}

	m_common_data.boss_normal_call_count = 0;
	m_common_data.boss_super_call_count = 0;
	m_common_data.totem_exp_today = 0;

	m_common_data.daily_kill_redbag_boss_times = 0;
	m_common_data.daily_guild_relive_times = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig().daily_relive_modulus * m_member_manager.GetMemberCount();
	m_daily_fetch_user_info.clear();

	m_open_biaoche_role_id = 0;
	m_today_biaoche_isstart = 0;									
	m_today_rob_count = 0;											// 每天劫镖次数重置
//	m_biaoche_status = gglobalprotocal::YUNBIAO_STATUS_INVALID;		// 一定要保证护送的镖车不会超过12点 不然就跪，配置要检查

	m_guild_tianci_tongbi_day_count = 0;
	m_today_cross_goal_guild_notify = 0;

	this->ResetMaze();

	this->SyncGuildBossInfo();
	this->SyncGulidReliveTimesInfo();

	m_is_change = true;

	this->CalcQuestionRankReward();

	const tm *tm_now = EngineAdapter::Instance().LocalTime();
	if (NULL != tm_now && 1 == tm_now->tm_wday)
	{
		this->OnWeekChange();
	}

	if (NULL != tm_now && 1 == tm_now->tm_mday)
	{
		this->OnMonthChange();
	}

// 	if (EngineAdapter::Instance().Time() - m_last_active_time >= 24 * 3600 && !m_member_manager.HasOnlineMember())
// 	{
// 		const int old_guild_level = m_guild_level;
// 		const int old_guild_exp = m_guild_exp;
// 
// 		m_guild_exp -= LOGIC_CONFIG->GetGuildConfig().GetGuildReduceExpDay(m_guild_level);
// 		if (m_guild_exp < 0)
// 		{
// 			m_guild_level--;
// 
// 			const GuildLevelConfig::ConfigItem *level_cfg = LOGIC_CONFIG->GetGuildConfig().GetLevelConfig(m_guild_level);
// 			if (NULL != level_cfg)
// 			{
// 				m_guild_exp += level_cfg->max_exp;
// 			}
// 			else
// 			{
// 				m_guild_exp = 0;
// 			}
// 
// 			m_is_change = true;
// 			this->OnLevelChange(false);
// 
// 			if (m_guild_level > 0)
// 			{
// 				// 降级提醒
// 				static MailSubject mail_subject = {0};
// 				static MailContentTxt contenttxt = {0};
// 
// 				int length1 = SNPRINTF(mail_subject, sizeof(mail_subject), gamestring::g_guild_downgrade_subject);
// 				int length2 = SNPRINTF(contenttxt, sizeof(contenttxt), gamestring::g_guild_downgrade_content, old_guild_level, m_guild_level);
// 				if (length1 > 0 && length2 > 0)
// 				{	
// 					this->GetGuildMemberManager().SystemMailToAll(mail_subject, contenttxt);
// 				}
// 			}
// 			else
// 			{
// 				// 自动解散
// 				GameName tuanzhang_name = {0};
// 				UserID tuanzhang_userid = m_register.GetGuildTuanZhangInfo(tuanzhang_name);
// 				m_guild_manager->DismissGuild(this, tuanzhang_userid, GUILD_DISMISS_TYPE_AUTO);
// 			}
// 		}
// 
// 		gamelog::g_log_guild.printf(LL_INFO, "[befor auto dismiss guild_exp] : [%d, %d] -> [%d, %d]", old_guild_level, old_guild_exp, m_guild_level, m_guild_exp);
// 	}
}

void Guild::OnMonthChange()
{
	m_member_manager.OnMonthChange();
}

void Guild::Update(unsigned long interval, time_t now_second)
{	
	if (0 == m_last_active_time)
	{
		this->SetLastActiveTime(EngineAdapter::Instance().Time());
	}

	if (now_second >= m_next_get_cabablity_time)
	{
		m_next_get_cabablity_time = static_cast<unsigned int>(now_second + GUILD_SAVE_INTERVAL);
		
		m_guild_max_capability = this->GetCapability();
		m_is_change = true;
	}

	unsigned int today_id = EngineAdapter::Instance().DayID();
	if (today_id != m_last_update_dayid)
	{
		if (0 != m_last_update_dayid)
		{
			this->OnDayChange();
		}

		m_last_update_dayid = today_id;
		m_is_change = true;
	}

	//if (now_second >= m_next_check_auto_kickout_time && m_auto_kickout_setup > 0)  // 需要客户端请求设置踢僵尸玩家m_auto_kickout_setup
	if (now_second >= m_next_check_auto_kickout_time)  // 不需要客户端请求设置踢僵尸玩家m_auto_kickout_setup
	{
		m_member_manager.CheckAutoKickout();

		m_next_check_auto_kickout_time = static_cast<unsigned int>(now_second) + 60;
	}

	this->UpdateFbStatus(interval, now_second);
	this->UpdateBonfireStatus(interval, now_second);

	if (now_second >= m_next_save_time)
	{
		m_next_save_time = static_cast<unsigned int>(now_second + GUILD_SAVE_INTERVAL);
		this->Save();
	}

	
	if (now_second >= m_next_send_saizi_rank_msg)
	{
		m_next_send_saizi_rank_msg = static_cast<unsigned int>(now_second) + GUILD_SEND_SAIZI_RANK_MSG_INTERVAL;
		
		m_member_manager.SendSaiziRankMsg();
	}

	if (!CrossConfig::Instance().IsHiddenServer() && 0 == m_question_next_begin)
	{
		this->CalcNextQuestionTime();
	}

	if (m_question_next_begin > 0 && static_cast<unsigned int>(now_second) > m_question_next_begin)
	{
		const WGQuestionCfg *question_cfg = LOGIC_CONFIG->GetQuestionConfig().GetWGQuestionCfg(QUESTION_TYPE_GUILD);
		if (NULL == question_cfg)
		{
			m_question_next_begin = 0;
			return;
		}

		m_question_id = LOGIC_CONFIG->GetQuestionConfig().RandQuestionID(&m_question_answer, QUESTION_TYPE_GUILD);
		m_question_cur_begin = static_cast<unsigned int>(now_second);
		m_question_cur_end = static_cast<unsigned int>(now_second) + question_cfg->answer_continue_time;
		this->CalcNextQuestionTime();

		this->BrocastGuildQuestion(NULL);
	}

	if (m_question_cur_end > 0 && static_cast<unsigned int>(now_second) > m_question_cur_end)
	{
		m_question_id = 0;
		m_question_answer = -1;
		m_question_cur_begin = 0;
		m_question_cur_end = 0;
		m_question_cur_right_num = 0;
		this->SendGuildQuestionRankInfo(NULL);

		UserCacheNode *first_user_node = UserCacheManager::Instance().GetUserNode(m_question_fast_uid[0]);
		UserCacheNode *second_user_node = UserCacheManager::Instance().GetUserNode(m_question_fast_uid[1]);
		UserCacheNode *third_user_node = UserCacheManager::Instance().GetUserNode(m_question_fast_uid[2]);
		if (NULL != first_user_node && NULL != second_user_node && NULL != third_user_node)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_question_fast_guild_reward,
				first_user_node->uid, first_user_node->user_name, (int)first_user_node->camp,
				second_user_node->uid, second_user_node->user_name, (int)second_user_node->camp,
				third_user_node->uid, third_user_node->user_name, (int)third_user_node->camp);
			if (length)
			{
				this->GuildSystemMsg(length, gamestring::GAMESTRING_BUF);
			}
		}

		memset(m_question_fast_uid, 0, sizeof(m_question_fast_uid));
	}
	
	this->AutoClearGuildMember(static_cast<unsigned int>(now_second));

	this->SyncGuildRankDataToCross(now_second);
	
	this->CheckCrossGoalGuildNotify(static_cast<unsigned int>(now_second));
}

int Guild::GetGuildExp(int *guild_max_exp)
{
	if (NULL != guild_max_exp) 
	{
		const GuildLevelConfig::ConfigItem *level_cfg = LOGIC_CONFIG->GetGuildConfig().GetLevelConfig(m_guild_level);
		if (NULL != level_cfg)
		{
			*guild_max_exp = level_cfg->max_exp;
		}
	}

	return m_guild_exp;
}

void Guild::GetTuanzhuangName(GameName name)
{
	this->GetGuildRegister().GetGuildTuanZhangInfo(name, NULL);
}

char Guild::GetTuanzhangProf()
{
	return this->GetGuildRegister().GetGuildTuanzhangProf();
}

char Guild::GetTuanzhangSex()
{
	return this->GetGuildRegister().GetGuildTuanzhangSex();
}

int Guild::GetTuanzhuangRoleId()
{
	return UidToInt(this->GetGuildRegister().GetGuildTuanZhangInfo(NULL, NULL));
}

long long Guild::GetTuanzhuangUuid()
{
	return this->GetGuildRegister().GetGuildTuanzhangUuid();
}

void Guild::AddTotemExpToday(int value)
{
	if (value <= 0) return;

	m_common_data.totem_exp_today += value;
}


int Guild::GetGuildMaxMemberCount()
{
	return m_common_data.default_member_count + m_common_data.extend_member_count;
}

bool Guild::SetApplyforSetup(int applyfor_setup, int need_capability, int need_level)
{
	if (applyfor_setup < GUILD_APPLYFOR_SETUP_NEED_AGREE || applyfor_setup >= GUILD_APPLYFOR_TYPE_MAX) return false;

	m_applyfor_setup = applyfor_setup;
	m_applyfor_need_capability = need_capability;
	m_applyfor_need_level = need_level;

	m_is_change = true;

	return true;
}

void Guild::SetAutoKickoutSetup(int auto_kickout_setup)
{
	m_auto_kickout_setup = auto_kickout_setup;

	m_is_change = true;
}

void Guild::AddCallinTimes(int add_val)
{
	m_callin_times += add_val;
	
	if (m_callin_times < 0)
	{
		m_callin_times = 0;
	}
}

int Guild::GetCapability()
{
	return m_member_manager.GetCapability();
}

bool Guild::IsGuildFull()
{
	return (this->GetGuildRegister().GetGuildMemberCount() >= this->GetGuildMaxMemberCount());
}

void Guild::Save()
{
	static AllGuildParam::GuildParam guild_param;

	m_apply_for_manager.GetChangeApplyForList(&guild_param.apply_for_param);
	m_member_manager.GetChangeMemberList(&guild_param.member_param);
	m_event_manager.GetChangeEventList(&guild_param.event_param);

	if (guild_param.apply_for_param.count <= 0 && guild_param.event_param.count <= 0 && 
		guild_param.member_param.count <= 0 && !m_is_change)
	{
		return;
	}

	guild_param.base_param.guild_id = m_guild_id;
	guild_param.base_param.camp = m_camp;
	F_STRNCPY(guild_param.base_param.guild_name, m_guild_name, sizeof(guild_param.base_param.guild_name));
	F_STRNCPY(guild_param.base_param.notice, m_guild_notice, sizeof(guild_param.base_param.notice));
	guild_param.base_param.create_time = m_create_time;
	guild_param.base_param.guild_level = m_guild_level;
	guild_param.base_param.guild_exp = m_guild_exp;
	guild_param.base_param.guild_last_uplevel_timestamp = m_guild_last_uplevel_timestamp;

	guild_param.base_param.last_update_dayid = m_last_update_dayid;
	guild_param.base_param.last_active_time = static_cast<unsigned int>(m_last_active_time);
	guild_param.base_param.warning_dismiss_days = m_warning_dismiss_days;
	guild_param.base_param.applyfor_setup = m_applyfor_setup;
	guild_param.base_param.applyfor_need_capability = m_applyfor_need_capability;
	guild_param.base_param.applyfor_need_level = m_applyfor_need_level;
	guild_param.base_param.callin_times = m_callin_times;
	guild_param.base_param.monster_finish_seq = m_monster_finish_seq;
	guild_param.base_param.active_degree = m_active_degree;
	F_STRNCPY(guild_param.base_param.monster_finish_username, m_monster_finish_username, sizeof(guild_param.base_param.monster_finish_username));
	guild_param.base_param.common_data_param = m_common_data;
	m_guild_storge.GetStorgeItemList(&guild_param.base_param);

	this->SaveGuildEnemy(&guild_param.base_param);

	guild_param.base_param.auto_kickout_setup = m_auto_kickout_setup;
	guild_param.base_param.guild_max_capability = m_guild_max_capability;

	guild_param.base_param.common_data_param.today_yunbiao_isstart = static_cast<char>(m_today_biaoche_isstart);
	guild_param.base_param.common_data_param.today_yunbiao_rob_count = m_today_rob_count;

	guild_param.base_param.common_data_param.guild_tianci_tongbi_day_count = m_guild_tianci_tongbi_day_count;
	guild_param.base_param.common_data_param.today_kill_cross_boss_num = m_today_kill_cross_boss_num;
	guild_param.base_param.common_data_param.cross_goal_kill_cross_boss = m_cross_goal_kill_cross_boss;
	guild_param.base_param.common_data_param.cross_goal_kill_baizhan_boss = m_cross_goal_kill_baizhan_boss;
	guild_param.base_param.common_data_param.corss_goal_guild_flag = m_cross_goal_guild_flag;
	
	RMISaveGuildBackObjectImpl *impl = new RMISaveGuildBackObjectImpl();

	RMIGlobalClient ac;
	ac.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	if (ac.SaveGuildAsyn(m_guild_id, guild_param, impl))
	{
		m_is_change = false;
		m_apply_for_manager.ClearDirtyMark();
		m_member_manager.ClearDirtyMark();
		m_event_manager.ClearDirtyMark();
		m_guild_storge.ClearDirtyMark();
	}
}

void Guild::OnServerStart()
{

}

void Guild::OnServerStop()
{

}

void Guild::ChangeNotice(GuildNotice notice)
{
	STRNCPY(m_guild_notice, notice, sizeof(m_guild_notice));
	m_is_change = true;
}

void Guild::NotifyMsgToGuild(int notice_type, GuildMember *member, int notify_param, int notify_param1)
{
	m_member_manager.NotifyMsgToPrincipal(notice_type, member, notify_param, notify_param1);
}

void Guild::SendMessageToGuild(int notice_type, const char *data, int len, GuildMember *member)
{
	if (NULL == data || len <= 0) return;

	m_member_manager.SendMessageToPrincipal(notice_type, data, len, member);
}

void Guild::SendMessageToGuildAll(int notice_type, const char *data, int len, GuildMember *member)
{
	if (NULL == data || len <= 0) return;

	m_member_manager.SendMessageToAll(notice_type, data, len, member);
}

void Guild::GuildSystemMsg(int length, const char *msg,  int filter_member_count, const UserID *filter_member_list, int msg_type)
{
	if (NULL == msg || length <= 0)
	{
		return;
	}

	static Protocol::SCSystemMsg sm;
	int sendlen = 0;

	if (World::GetInstWorld()->GetSceneManager()->CreateSystemMsg(&sm, &sendlen, msg, length, msg_type))
	{
		m_member_manager.OnSystemMsg((const char *)&sm, sendlen, filter_member_count, filter_member_list);	
	}
}

void Guild::OnMemberLogin(GuildMember *member, Role *role)
{	
	if (NULL == role)
	{
		return;
	}
	
	this->SetLastActiveTime(EngineAdapter::Instance().Time());
	this->SyncGulidReliveTimesInfo(role);
	role->SyncSCGulidBossRedbagInfo();
	this->GetGuildMemberManager().SyncSCGulidSaiziInfo(role);
	this->RemoveGuildEnemy(role->GetUniqueUserID());

	if (!this->GetGuildApplyForManager().ApplyForListIsEmpty())
	{
		this->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_APPLYFOR, member);
	}

	if (gglobalprotocal::YUNBIAO_STATUS_START == m_biaoche_status)
	{
		int biaoche_objid = GuildShadow::Instance().GetGuildBiaocheObjId(m_guild_id);
		this->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_GUILD_BIAOCHE_START, member, biaoche_objid, m_open_biaoche_role_id);
	}
}

void Guild::OnMemberLogout(GuildMember *member)
{
	this->SetLastActiveTime(EngineAdapter::Instance().Time());
}

void Guild::OnNewApplyFor(Role *role)
{
	if (NULL == role) return;

	this->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_APPLYFOR);
	role->GetCrossGoal()->CheckGuildGoalStatus(true);
}

void Guild::AddGuildExp(int add_exp)
{
// 	if (add_exp <= 0) return;
// 
// 	if (INT_MAX32 - m_guild_exp < add_exp)
// 	{
// 		m_guild_exp = INT_MAX32;
// 	}
// 	else
// 	{
// 		m_guild_exp += add_exp;
// 	}
// 	
// 	m_is_change = true;
// 
// 	const GuildLevelConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetGuildConfig().GetLevelConfig(m_guild_level);
// 	if (NULL == cfg_item || m_guild_exp < cfg_item->max_exp)
// 	{
// 		return;
// 	}
// 
// 	const GuildLevelConfig::ConfigItem *next_cfg_item = LOGIC_CONFIG->GetGuildConfig().GetLevelConfig(m_guild_level + 1);
// 	if (NULL == next_cfg_item)
// 	{
// 		return;
// 	}
// 
// 	for(int i = 0; i <= GUILD_LEVEL_MAX; i++)
// 	{
// 		if (m_guild_exp < cfg_item->max_exp)
// 		{
// 			break;
// 		}
// 
// 		m_guild_exp -= cfg_item->max_exp;
// 		m_guild_level++;
// 
// 		cfg_item = LOGIC_CONFIG->GetGuildConfig().GetLevelConfig(m_guild_level);
// 		if (NULL == cfg_item || m_guild_exp < cfg_item->max_exp)
// 		{
// 			break;
// 		}
// 
// 		next_cfg_item = LOGIC_CONFIG->GetGuildConfig().GetLevelConfig(m_guild_level + 1);
// 		if (NULL == next_cfg_item)
// 		{
// 			break;;
// 		}
// 	}
// 
// 	m_guild_last_uplevel_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 
// 	this->OnLevelChange(true);
// 
// 	gamelog::g_log_guild.printf(LL_INFO, "guild->uplevel guild_id[%d], guild_name[%s], guild_level[%d]",
// 		m_guild_id, m_guild_name, m_guild_level);
// 
// 	{
// 		static MailSubject mail_subject = {0};
// 		static MailContentTxt contenttxt = {0};
// 
// 		int length1 = SNPRINTF(mail_subject, sizeof(mail_subject), gamestring::g_guild_upgrade_level_subject);
// 		int length2 = SNPRINTF(contenttxt, sizeof(contenttxt), gamestring::g_guild_upgrade_level_content, m_guild_level);
// 		if (length1 > 0 && length2 > 0)
// 		{
// 			this->GetGuildMemberManager().SystemMailToAll(mail_subject, contenttxt);
// 		}
// 	}
// 
// 	if (m_guild_level >= 3)
// 	{
// 		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_guilduplevel,
// 			m_guild_name, m_guild_level);
// 		if (length > 0)
// 		{
// 			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_AND_ROLL, 0, 0);
// 		}
// 	}
}

bool Guild::DecGuildExp(int dec_exp)
{
// 	if (dec_exp <= 0) return false;
// 
// 	m_guild_exp -= dec_exp;
// 	m_is_change = true;

	return true;
}

void Guild::AddTotemExp(int totem_exp)
{
 	if(totem_exp <= 0) return;
 
 	m_common_data.totem_exp += totem_exp;
 	m_is_change = true;
}

void Guild::GuildUpTotemLevel(Role *role)
{
	if (NULL == role) return;

	if(GUILD_POST_TUANGZHANG != role->GetGuildPost() && GUILD_POST_FU_TUANGZHANG != role->GetGuildPost())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
		return;
	}
	const GuildTotemCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetGuildConfig().GetTutemConfig(m_common_data.totem_level);
	if(NULL == cfg_item)
	{
		return;
	}

	if(m_common_data.totem_exp < cfg_item->uplevel_exp)
	{
		role->NoticeNum(errornum::EN_GUILD_TOTEM_EXP_NOT_ENOGHT);
		return;
	}

	const GuildTotemCfg::ConfigItem *next_cfg_item = LOGIC_CONFIG->GetGuildConfig().GetTutemConfig(m_common_data.totem_level + 1);
	if (NULL == next_cfg_item)
	{
		return;
	}

	m_common_data.totem_exp -= cfg_item->uplevel_exp;
	m_common_data.totem_level++;

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GUILD);
	
	{
		Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
		if (NULL == guild)
		{
			return;
		}

		static Protocol::SCGuildBaseInfo gi;

		gi.guild_id = guild->GetGuildID();
		guild->GetGuildName(gi.guild_name);
		gi.guild_level = guild->GetGuildLevel();
		gi.guild_exp = guild->GetGuildExp(&gi.guild_max_exp);
		gi.totem_level = guild->GetTotemLevel();
		gi.totem_exp = guild->GetTotemExp();
		gi.cur_member_count = guild->GetGuildRegister().GetGuildMemberCount();
		gi.max_member_count = guild->GetGuildMaxMemberCount();
		UserID tuanzhang_user_id = guild->GetGuildRegister().GetGuildTuanZhangInfo(gi.tuanzhang_name);	
		gi.tuanzhang_uid = UidToInt(tuanzhang_user_id);
		gi.create_time = static_cast<unsigned int>(guild->GetGuildCreateTime());
		gi.camp = guild->GetCamp();
		gi.vip_level = guild->GetGuildRegister().GetGuildTuanzhangVipLevel();
		gi.applyfor_setup = guild->GetApplyforSetup();
		guild->GetNotice(gi.guild_notice);
		gi.auto_kickout_setup = guild->GetAutoKickoutSetup();
		gi.applyfor_need_capability = guild->GetApplyforNeedCapability();
		gi.applyfor_need_level = guild->GetApplyforNeedLevel();
		gi.callin_times = guild->GetCallinTimes();
		gi.active_degree = guild->GetActiveDegree();
		gi.total_capability = guild->GetGuildMaxCapability();
		gi.rank = guild->GetRank();
		gi.totem_exp_today = guild->GetTotemExpToday();
		gi.is_auto_clear = guild->GetIsAutoClear();
		gi.is_today_biaoche_start = (guild->IsBiaoCheHadEnd() ? 1 : 0);
		gi.avatar_timestamp = guild->GetGuildAvatar();

		guild->SendMessageToGuild(Protocol::GUILD_NOTIFY_TYPE_TOTEM_UPLEVEL, (const char *)&gi, sizeof(gi));
	}
}

bool Guild::IsGuildStationHasActivity()
{
	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (now_time < m_guild_beast_finish_timestamp)
	{
		return true;
	}

	return false;
}

void Guild::OnBaseInfoChange()
{
	RankManager::Instance().GetGuildRank()->OnBaseInfoChange(this);
}

void Guild::OnMemberSos(Role *member, gglobalprotocal::GuildMemberSos *gg_gms)
{
	if (NULL == member)
	{
		return;
	}

	static Protocol::SCGuildMemberSos gms;
	gms.sos_type = gg_gms->sos_type;
	gms.member_uid = UidToInt(member->GetUserId());
	member->GetName(gms.member_name);
	gms.member_scene_id = gg_gms->scene_id;
	gms.member_pos_x = gg_gms->pos_x;
	gms.member_pos_y = gg_gms->pos_y;

	if (gg_gms->enemy_uid > 0)
	{
		Role *enemy_user = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(gg_gms->enemy_uid));
		if (NULL == enemy_user) return;
		
		gms.enemy_uid = UidToInt(enemy_user->GetUserId());
		enemy_user->GetName(gms.enemy_name);
		gms.enemy_camp = enemy_user->GetCamp();
		gms.enemy_guild_id = enemy_user->GetGuildID();
		enemy_user->GetGuildName(gms.enemy_guild_name);
	}
	else
	{
		gms.enemy_uid = 0;
		memset(gms.enemy_name, 0, sizeof(gms.enemy_name));
		gms.enemy_camp = 0;
		gms.enemy_guild_id = INVALID_GUILD_ID;
		memset(gms.enemy_guild_name, 0, sizeof(gms.enemy_guild_name));
	}

	this->SendMessageToGuild(Protocol::GUILD_NOTIFY_TYPE_MEMBER_SOS, (const char *)&gms, sizeof(gms));

	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_sos_husong, member->GetUID(), member->GetName(), (int)member->GetCamp(), gg_gms->scene_id, (int)gg_gms->pos_x, (int)gg_gms->pos_y);
	if (length > 0)
	{
		this->GuildSystemMsg(length, gamestring::GAMESTRING_BUF);
	}

}

void Guild::OnResetName(GuildName new_name)
{
	static GuildName old_name = {0};
	F_STRNCPY(old_name, m_guild_name, sizeof(old_name));

	F_STRNCPY(m_guild_name, new_name, sizeof(m_guild_name));

	m_member_manager.OnSyncGuildInfoToAllMember();

	{
		Guild *guild = m_member_manager.GetGuild();
		if (NULL != guild)
		{
			char guild_camp = guild->GetCamp();
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_rename_notice,
				IntToUid(m_guild_id).db_index, old_name, guild_camp, IntToUid(m_guild_id).db_index, m_guild_name, guild_camp);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2, 0, 0);
			}
		}
	}

	{
		static MailSubject subject;
		static MailContentTxt contenttxt;

		int length1 = SNPRINTF(subject, sizeof(subject), gamestring::g_guild_rename_subject);
		int length2 = SNPRINTF(contenttxt, sizeof(contenttxt), gamestring::g_guild_rename_content, old_name, m_guild_name);
		if (length1 > 0 && length2 > 0)
		{
			m_member_manager.SystemMailToAll(subject, contenttxt);
		}
	}
}

int Guild::GetGuildMaxCapability()
{
	return m_guild_max_capability;
}

void Guild::GetMonsterFinishUsername(GameName username)
{
	if (NULL != username)
	{
		F_STRNCPY(username, m_monster_finish_username, sizeof(GameName));
	}
}

void Guild::OnMonsterFinish(short seq, const GameName user_name)
{
	if (seq > m_monster_finish_seq)
	{
		m_monster_finish_seq = seq;

		if (NULL != user_name)
		{
			F_STRNCPY(m_monster_finish_username, user_name, sizeof(m_monster_finish_username));
		}

		m_is_change = true;
	}
}

void Guild::OnMemberKillWorldBoss()
{
	m_common_data.kill_world_boss_count += 1;

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_XIANMENG_BIPIN))
	{
		m_common_data.xianmeng_bipin_kill_boss_count += 1;
	}

	m_is_change = true;
}

int Guild::GetKillWorldBossCount()
{
	return m_common_data.kill_world_boss_count;
}

void Guild::ClearKillWorldBossCount()
{
	m_common_data.kill_world_boss_count = 0;
	m_is_change = true;
}

int Guild::GetXianmengBipinKillWorldBoss()
{
	return m_common_data.xianmeng_bipin_kill_boss_count;
}

void Guild::ClearXianmengBipinKillWorldBossCount()
{
	m_common_data.xianmeng_bipin_kill_boss_count = 0;
}

int Guild::GetDayIncreaseCapability()
{
	return m_guild_max_capability - m_common_data.guild_yesterday_capability;
}

void Guild::RecordYesterdayCapability()
{
	m_common_data.guild_yesterday_capability = this->GetCapability();
	m_is_change = true;
}

void Guild::SyncGuildRankDataToCross(time_t now)
{
	if (now > m_next_sync_to_cross_time)
	{
		m_next_sync_to_cross_time = now + SECOND_PER_MIN;

		static crossgameprotocal::GameCrossSyncCrossPersonRankInfo rank_info;

		rank_info.plat_type = LocalConfig::Instance().GetPlatType();
		rank_info.role_id = this->GetGuildID();
		rank_info.exp = 0;
		rank_info.level = 0;
		this->GetTuanzhuangName(rank_info.name);
		rank_info.sex = this->GetTuanzhangSex();
		rank_info.prof = this->GetTuanzhangProf();

		int item_count = 0;

		int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), EngineAdapter::Instance().Time()) + 1;
		int limit_open_day = 0;

		if (item_count < crossgameprotocal::SYNC_CROSS_PERSON_RANK_INFO_SIZE)
		{
			limit_open_day = CrossRankRewardConfig::Instance().GetCrossRankLimitOpenDay(CROSS_PERSON_RANK_TYPE_GUILD_KILL_BOSS);
			if (limit_open_day <= 0 || opengame_day >= limit_open_day)
			{
				rank_info.items[item_count].rank_type = CROSS_PERSON_RANK_TYPE_GUILD_KILL_BOSS;
				rank_info.items[item_count].rank_value = this->GetGuildKillBossNum();
				rank_info.items[item_count].record_index = 0;
				rank_info.items[item_count].flexible_int = this->GetTuanzhuangRoleId();
				rank_info.items[item_count].flexible_ll = this->GetGuildAvatar();
				this->GetGuildName(rank_info.items[item_count].flexible_name);
				++item_count;
			}
		}

		rank_info.item_count = item_count;
		if (rank_info.item_count > 0)
		{
			// 发往跨服排行榜所在战区，正常为1战区
			int sendlen = sizeof(rank_info) - (crossgameprotocal::SYNC_CROSS_PERSON_RANK_INFO_SIZE - rank_info.item_count) * sizeof(rank_info.items[0]);
			InternalComm::Instance().SendToCross((const char *)&rank_info, sendlen, CROSS_FUNCTION_TYPE_CROSS_RANK);
		}
	}
}

void Guild::AddGuildGongzi(int gongzi)
{
	m_common_data.guild_total_gongzi += gongzi;

	if (m_common_data.guild_total_gongzi < 0)
	{
		m_common_data.guild_total_gongzi = 0;
	}

	this->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_TOTAL_GONGZI_CHNAGE, nullptr, m_common_data.guild_total_gongzi);
}

void Guild::AddMemberGongzi(int member_uid, int gongzi)
{
	GuildMember *member = m_member_manager.GetMember(IntToUid(member_uid));
	if (NULL == member)
	{
		return;
	}

	member->AddGongzi(gongzi);

	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid));
	if (NULL != role)
	{
		role->SendAddGuildGongziInfo(gongzi);
	}
}

int Guild::GetMemberGongzi(int member_uid)
{
	GuildMember *member = m_member_manager.GetMember(IntToUid(member_uid));
	if (NULL == member)
	{
		return 0;
	}

	return member->GetGongzi();
}

void Guild::OnGiveGongzi(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	GuildMember *member = m_member_manager.GetMember(role->GetUserId());
	if (NULL == member)
	{
		return;
	}

	// 权限
	if (GUILD_POST_TUANGZHANG != member->GetPost())
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
		return;
	}

	// 工资是否足够
	GuildOtherConfig &other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();
	if (m_common_data.guild_total_gongzi < other_cfg.need_gongzi)
	{
		gstr::SendError(role, "guild_gongzi_not_enought");
		return;
	}

	// 排序
	int total_gold_bind = this->SortMemberGongziRank();

	// 判断人数补齐
	int min_give_gongzi_member_num = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig().min_give_gongzi_member_num;
	int delta_member_num = min_give_gongzi_member_num - m_member_manager.GetMemberCount();
	int post_rate = LOGIC_CONFIG->GetGuildConfig().GetGongziRate(GUILD_POST_CHENG_YUAN);
	int total_gongzi_rate = m_member_manager.GetRealGongziTotalRate();
	float gongzi_rate = total_gongzi_rate > 0 ? post_rate * 1.0f / total_gongzi_rate : 0;

	if (delta_member_num > 0)
	{
		for (int i = 0; i < delta_member_num; i ++)
		{
			MemberGongziInfo info;
			info.gold_bind = static_cast<int>(m_common_data.guild_total_gongzi * gongzi_rate);

			m_gongzi_rank_vec.push_back(info);

			total_gold_bind += info.gold_bind;
		}
	}

	int left_gold_bind = m_common_data.guild_total_gongzi - total_gold_bind;

	while (left_gold_bind > 0)
	{
		for (auto &it : m_gongzi_rank_vec)
		{
			if (left_gold_bind <= 0)
			{
				break;
			}

			it.gold_bind++;
			left_gold_bind--;
		}
	}

	// 发送邮件
	for (auto &it : m_gongzi_rank_vec)
	{
		if (it.uid <= 0)
		{
			continue;
		}

		static MailContentParam contentparam; contentparam.Reset();

		contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BIND_GOLD, it.gold_bind);

		int length = gstr::MailContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "guild_gongzi_give_mail");
		if (length > 0)
		{
			MailRoute::MailToUser(IntToUid(it.uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}

	// 发送聊天
	{
		static char buff[1024]; memset(buff, 0, sizeof(buff));

		Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)buff;
		cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
		cc->msg_length = gstr::SysMsgContent(buff + sizeof(Protocol::CSChannelChat), sizeof(buff) - sizeof(Protocol::CSChannelChat), "guild_gongzi_give_chat", m_common_data.guild_total_gongzi);

		m_member_manager.GuildleaderGuildChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat));
	}

	// 清除工资相关信息
	static int member_uid_list[MAX_MEMBER_COUNT];
	memset(member_uid_list, 0, sizeof(member_uid_list));

	{
		int member_count = m_member_manager.GetMemberList(member_uid_list);
		for (int i = 0; i < member_count && i < MAX_MEMBER_COUNT; ++i)
		{
			GuildMember *member = m_member_manager.GetMember(IntToUid(member_uid_list[i]));
			if (NULL != member)
			{
				member->ResetGongzi();
			}
		}

		m_common_data.guild_total_gongzi = 0;

		m_gongzi_rank_vec.clear();
	}

	this->SendMemberGongziList(role);
}

int Guild::SortMemberGongziRank()
{
	int total_gold_bind = 0;
	m_gongzi_rank_vec.clear();

	static int member_uid_list[MAX_MEMBER_COUNT];
	memset(member_uid_list, 0, sizeof(member_uid_list));

	int member_count = m_member_manager.GetMemberList(member_uid_list);
	for (int i = 0; i < member_count && i < MAX_MEMBER_COUNT; ++i)
	{
		GuildMember *member = m_member_manager.GetMember(IntToUid(member_uid_list[i]));
		if (NULL != member)
		{
			MemberGongziInfo info;

			info.uid = member_uid_list[i];
			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(info.uid);
			if (NULL != user_node)
			{
				user_node->GetName(info.role_name);
			}

			int gongzi_rate = m_member_manager.GetMemberGongziRate(member_uid_list[i]);

			info.post = member->GetPost();
			info.gongzi = member->GetGongzi();
			info.gongzi_rate = gongzi_rate / 100;
			info.gold_bind = static_cast<int>(m_common_data.guild_total_gongzi * (gongzi_rate * 0.0001f));

			total_gold_bind += info.gold_bind;

			m_gongzi_rank_vec.push_back(info);
		}
	}

	std::stable_sort(m_gongzi_rank_vec.begin(), m_gongzi_rank_vec.end());

	return total_gold_bind;
}

void Guild::SendMemberGongziList(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	this->SortMemberGongziRank();

	static Protocol::SCGuildGongziRankList ggrl;
	ggrl.guild_id = m_guild_id;
	ggrl.count = 0;

	for (auto &it : m_gongzi_rank_vec)
	{
		if (ggrl.count >= MAX_MEMBER_COUNT)
		{
			break;
		}

// 		if (it.gongzi <= 0)
// 		{
// 			continue;
// 		}

		MemberGongziInfo &info = ggrl.member_list[ggrl.count];
		info.uid = it.uid;
		F_STRNCPY(info.role_name, it.role_name, sizeof(GameName));
		info.post = it.post;
		info.gongzi = it.gongzi;
		info.gongzi_rate = it.gongzi_rate;
		info.gold_bind = it.gold_bind;

		ggrl.count++;
	}

	int sendlen = sizeof(ggrl) - (MAX_MEMBER_COUNT - ggrl.count) * sizeof(ggrl.member_list[0]);

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&ggrl, sendlen);
}

void Guild::StartGuildFb(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	GuildRoute::Instance().OnGuildFbOpen(this, role);

	int guild_fb_interval_s = LOGIC_CONFIG->GetGuildFbConfig().GetGuildFbIntervalS();

	++ m_common_data.guild_fb_open_times;
	if (ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILD_FB))
	{
		m_guild_fb_finish_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time() + guild_fb_interval_s);
	}
	
	this->SyncGuildFbStatus();
	this->SetGuildFbCurIndex(0);

	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_open_guild_fb);
	if (length > 0) 
	{
		this->GuildSystemMsg(length, gamestring::GAMESTRING_BUF);
	}

 	m_is_change = true;
}

bool Guild::IsGuildFbOpen(unsigned int check_second)
{
 	if (m_guild_fb_finish_timestamp > 0 && check_second < m_guild_fb_finish_timestamp)
 	{
 		return true;
 	}
 
 	return false;
}

void Guild::SyncGuildFbStatus(const GSNetID net_id)
{
 	static Protocol::SCGuildFbStatus gfs;
 	gfs.open_times = m_common_data.guild_fb_open_times;
 	gfs.reserve_sh = 0;
 	gfs.finish_timestamp = m_guild_fb_finish_timestamp;
 
 	if (INVALID_GSNETID != net_id)
 	{
 		EngineAdapter::Instance().NetSend(net_id, (const char*)&gfs, sizeof(gfs));
 	}
 	else
 	{
 		this->SendMessageToGuild(Protocol::GUILD_NOTIFY_TYPE_GUILD_FB, (const char *)&gfs, sizeof(gfs));
 	}
}

void Guild::UpdateFbStatus(unsigned long interval, time_t now_second)
{
 	if (m_guild_fb_finish_timestamp > 0 && now_second >= m_guild_fb_finish_timestamp)
 	{
 		GuildRoute::Instance().OnGuildFbClose(this);
 
 		m_guild_fb_finish_timestamp = 0;
		m_guild_fb_cur_index = -1;
		m_guild_fb_guard_boss_hp = 0;
		m_hurt_monster_info_vec.clear();
		m_history_reward_record_map.clear();

 		this->SyncGuildFbStatus();
 	}
}

void Guild::CalcRoleHurtMonsterVal(Role *role, Attribute delta_hp)
{
	if (nullptr == role)
	{
		return;
	}

	int role_id = role->GetUID();
	auto it = std::find_if(m_hurt_monster_info_vec.begin(), m_hurt_monster_info_vec.end(), [role_id](const HurtMonsterInfo &cmp_item) {
		return role_id == cmp_item.role_id; });

	if (it != m_hurt_monster_info_vec.end())
	{
		(*it).hurt_val += -delta_hp;
	}
	else
	{
		HurtMonsterInfo hurt_info;
		hurt_info.role_id = role->GetUID();
		role->GetName(hurt_info.user_name);
		hurt_info.hurt_val += -delta_hp;
		m_hurt_monster_info_vec.emplace_back(hurt_info);
	}

	this->SortHurtRank();
}

void Guild::SortHurtRank()
{
	std::stable_sort(m_hurt_monster_info_vec.begin(), m_hurt_monster_info_vec.end());
}

void Guild::GetRoleRankInfo(int role_id, int &rank_pos, long long &rank_val)
{
	rank_pos = 0, rank_val = 0;

	if (0 == role_id) return;

	for (int i = 0; i < (int)m_hurt_monster_info_vec.size(); ++i)
	{
		if (m_hurt_monster_info_vec[i].role_id == role_id)
		{
			if (m_hurt_monster_info_vec[i].hurt_val > 0)
			{
				rank_pos = i + 1;
				rank_val = m_hurt_monster_info_vec[i].hurt_val;
			}
			return;
		}
	}
}

void Guild::GetHurtRank(Protocol::SCGuildFBInfo &msg)
{
	msg.rank_count = 0;
	int max_rank_count = static_array_size(msg.rank_info_list);

	for (std::vector<HurtMonsterInfo>::const_iterator it = m_hurt_monster_info_vec.begin(); it != m_hurt_monster_info_vec.end() && msg.rank_count < max_rank_count; ++it)
	{
		if (it->hurt_val <= 0)
		{
			continue;
		}

		msg.rank_info_list[msg.rank_count].user_id = it->role_id;
		F_STRNCPY(msg.rank_info_list[msg.rank_count].user_name, it->user_name, sizeof(msg.rank_info_list[msg.rank_count].user_name));
		msg.rank_info_list[msg.rank_count].hurt_val = it->hurt_val;

		msg.rank_count++;
	}
}

void Guild::CheckPutHurtRankInfo(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	bool is_find = false;
	for (auto &it : m_hurt_monster_info_vec)
	{
		if (it.role_id == role->GetUID())
		{
			is_find = true;
			break;
		}
	}

	if (!is_find)
	{
		HurtMonsterInfo hurt_info;
		hurt_info.role_id = role->GetUID();
		role->GetName(hurt_info.user_name);
		m_hurt_monster_info_vec.emplace_back(hurt_info);
	}
}

void Guild::AddHistoryRewardRecord(Role *role, const Protocol::SCGuildFBRewardRecordInfo::ItemInfo &reward_item)
{
	if (nullptr == role)
	{
		return;
	}

	auto it = m_history_reward_record_map.find(role->GetUID());
	if (it != m_history_reward_record_map.end())
	{
		// 装备不叠加
		if (reward_item.item_color > 0)
		{
			it->second.emplace_back(reward_item);
		}
		else
		{
			auto vec_it = std::find_if(it->second.begin(), it->second.end(), [reward_item](const Protocol::SCGuildFBRewardRecordInfo::ItemInfo &cmp_item) {
				return cmp_item.reward_item_id == reward_item.reward_item_id;
			});

			if (vec_it != it->second.end())
			{
				(*vec_it).reward_item_num += reward_item.reward_item_num;
			}
			else
			{
				it->second.emplace_back(reward_item);
			}
		}

	}
	else
	{
		std::vector<Protocol::SCGuildFBRewardRecordInfo::ItemInfo> vec;
		vec.emplace_back(reward_item);
		m_history_reward_record_map[role->GetUID()] = vec;
	}
}

void Guild::GetHistoryRewardRecord(Role *role, Protocol::SCGuildFBRewardRecordInfo &msg)
{
	if (nullptr == role)
	{
		return;
	}

	auto it = m_history_reward_record_map.find(role->GetUID());
	if (it == m_history_reward_record_map.end() || it->second.empty())
	{
		return;
	}

	msg.item_count = 0;
	int max_item_count = static_array_size(msg.item_list);
	for (auto &item : it->second)
	{
		if (msg.item_count >= max_item_count)
		{
			break;
		}

		Protocol::SCGuildFBRewardRecordInfo::ItemInfo &item_info = msg.item_list[msg.item_count];
		item_info.reward_item_id = item.reward_item_id;
		item_info.reward_item_num = item.reward_item_num;
		item_info.item_color = item.item_color;

		msg.item_count++;
	}
}

void Guild::StartGuildBonfire(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	GuildRoute::Instance().OnGuildBonfireOpen(this, role);

	const int GUILD_BONFIRE_EXIST_TIME_S = LOGIC_CONFIG->GetGuildBonfireCfg().GetGatharExistTimeS();

	++ m_common_data.guild_bonfire_open_times;
	m_guild_bonfire_finish_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time() + GUILD_BONFIRE_EXIST_TIME_S);

	this->SyncGuildBonfireStatus();

	m_is_change = true;
}

void Guild::SyncGuildBonfireStatus(const GSNetID net_id)
{
	static Protocol::SCGuildBonfireStatus gbs;
	gbs.open_times = m_common_data.guild_bonfire_open_times;
	gbs.reserve_sh = 0;
	gbs.finish_timestamp = m_guild_bonfire_finish_timestamp;

	if (INVALID_GSNETID != net_id)
	{
		EngineAdapter::Instance().NetSend(net_id, (const char*)&gbs, sizeof(gbs));
	}
	else
	{
		this->SendMessageToGuild(Protocol::GUILD_NOTIFY_TYPE_GUILD_BONFIRE, (const char *)&gbs, sizeof(gbs));
	}
}

void Guild::UpdateBonfireStatus(unsigned long interval, time_t now_second)
{
	if (m_guild_bonfire_finish_timestamp > 0 && now_second >= m_guild_bonfire_finish_timestamp)
	{
		GuildRoute::Instance().OnGuildBonfireClose(this);

		m_guild_bonfire_finish_timestamp = 0;
		this->SyncGuildBonfireStatus();
	}
}

bool Guild::IsGuildBonfireOpen(unsigned int check_second)
{
	if (m_guild_bonfire_finish_timestamp > 0 && check_second < m_guild_bonfire_finish_timestamp)
	{
		return true;
	}

	return false;
}


void Guild::OnGuildFbFinish()
{
	// 由于仙盟boss 或者所有敌人 被干掉，所以要提前结束 仙盟副本

 	m_guild_fb_finish_timestamp = 0;
	m_guild_fb_cur_index = -1;
	m_guild_fb_guard_boss_hp = 0;
	m_hurt_monster_info_vec.clear();
	m_history_reward_record_map.clear();

 	this->SyncGuildFbStatus();
}

int Guild::GetMemberLuckyColor(const UserID &user_id)
{
	GuildMember *member = m_member_manager.GetMember(user_id);
	if (NULL != member)
	{
		return member->GetLuckyColor();
	}

	return GUILD_MEMBER_LUCKY_COLOR_WHITE;
}

bool Guild::CanDismiss(Role *role)
{
	if (NULL == role)
	{
		return false;
	}

	if (this->IsGuildStationHasActivity())
	{
		role->NoticeNum(errornum::EN_GUILD_STATION_BUSY_CANNOT_DISMISS);
		return false;
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (this->IsGuildFbOpen(now_second))
	{
		role->NoticeNum(errornum::EN_GUILD_FB_IS_OPEN_CANNOT_DISMISS);
		return false;
	}

	if (this->IsGuildBonfireOpen(now_second))
	{
		role->NoticeNum(errornum::EN_GUILD_BONFIRE_IS_OPEN_CANNOT_DISMISS);
		return false;
	}

	if (ActivityManager::Instance().IsActivityOpen(ACTIVITY_TYPE_GUILD_YUNBIAO))
	{
		gstr::SendError(role, "en_guild_yunbiao_limit");
		return false;
	}

	if (!ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_LIEKUN_FB))
	{
		role->NoticeNum(errornum::EN_GUILD_LIEKUN_ZHUMO_LIMIT);
		return false;
	}

	if (!ActivityShadow::Instance().IsCrossActivityClose(CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE))
	{
		role->NoticeNum(errornum::EN_CROSS_GUILD_BATTLE_LIMIT);
		return false;
	}

	if (CrossGuildBattleConfig::Instance().IsZhuMoTime(now_second))
	{
		role->NoticeNum(errornum::EN_GUILD_LIEKUN_ZHUMO_LIMIT);
		return false;
	}

	return true;
}

bool Guild::CanQuit(Role *role)
{
	if (NULL == role)
	{
		return false;
	}

	if (this->IsTianCiTongBiOpen(role))  // 天赐铜币活动
	{
		gstr::SendError(role, "en_guild_tianci_tongbi_limit");
		return false;
	}

	if (ActivityManager::Instance().IsActivityOpen(ACTIVITY_TYPE_GUILD_YUNBIAO))		// 仙盟镖车
	{
		gstr::SendError(role, "en_guild_yunbiao_limit");
		return false;
	}

	return true;
}

void Guild::SyncGuildBeastStatus(const GSNetID net_id)
{
	Protocol::SCGuildActiveDegreeInfo scgadi;
	scgadi.today_open_times = m_common_data.guild_beast_today_open_times;
	scgadi.finish_timestamp = m_guild_beast_finish_timestamp;
	scgadi.active_degree = m_active_degree;
	
	if (INVALID_GSNETID != net_id)
	{
		EngineAdapter::Instance().NetSend(net_id, (const char*)&scgadi, sizeof(scgadi));
	}
	else
	{
		this->SendMessageToGuild(Protocol::GUILD_NOTIFY_TYPE_GUILD_FB, (const char *)&scgadi, sizeof(scgadi));
	}
}

void Guild::SyncGulidReliveTimesInfo(Role *role)
{
	Protocol::SCGulidReliveTimes pro;
	pro.daily_guild_all_relive_times = m_common_data.daily_guild_relive_times;
	pro.daily_guild_all_kill_boss_times = m_common_data.daily_kill_redbag_boss_times;

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pro, sizeof(pro));
	}
	else
	{
		this->SendMessageToGuild(Protocol::GUILD_NOTIFY_TYPE_CONSUME_RELIVE_TIMES, (const char *)&pro, sizeof(pro));
	}
}

void Guild::StartGuildBeast(Role *role)
{
// 	if (NULL == role) return;
// 
// 	if (this->IsGuildStationHasActivity())
// 	{
// 		role->NoticeNum(errornum::EN_GUILD_CURRENT_GUILD_STATION_BUSY);
// 		return;
// 	}
// 
// 	SpecialGuildStation *guild_station = World::GetInstWorld()->GetSceneManager()->GetGuildStationSpecialLogic(this->GetGuildID(), true);
// 	if (NULL != guild_station)
// 	{
// 		int world_level = WorldShadow::Instance().GetWorldLevel(); 
// 		const GuildBeastConfig::ConfigItem *beast_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetBeastCfg(world_level, m_active_degree);
// 
// 		if (NULL == beast_cfg)
// 		{
// 			role->NoticeNum(errornum::EN_GUILD_ACTIVE_DEGREE_LIMIT);
// 			return;
// 		}
// 
// 		if (m_active_degree < beast_cfg->guild_active_degree_min)
// 		{
// 			role->NoticeNum(errornum::EN_GUILD_ACTIVE_DEGREE_LIMIT);
// 			return;
// 		}
// 
// 		const GuildActivityOther &other_cfg = LOGIC_CONFIG->GetGuildActivityCfg().OtherCfg();
// 
// 		if (guild_station->OnGuildBeastOpen(beast_cfg->beast_id, other_cfg.beast_born_position))
// 		{
// 			int old_active_degree = m_active_degree;
// 
// 			++ m_common_data.guild_beast_today_open_times;
// 			m_guild_beast_finish_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.beast_max_live_minute * 60;
// 			m_active_degree -= beast_cfg->guild_active_degree_min;
// 
// 			m_is_change = true;
// 
// 			gamelog::g_log_guild.printf(LL_INFO, "CallBeast guild_id[%d], guild_name[%s], old_active_degree[%d], new_active_degree[%d] beast_id[%d]",
// 				m_guild_id, m_guild_name, old_active_degree, m_active_degree, beast_cfg->beast_id);
// 
// 			this->SyncGuildBeastStatus();
// 
// 			// 开启活动的人直接传入
// 			static Protocol::CSGuildBackToStation gbts;
// 			gbts.guild_id = this->GetGuildID();
// 			GuildRoute::Instance().GuildBackToStationCS(role, &gbts);
// 		}
// 	}
}

void Guild::SetXianmengzhanRankValue(long long rank_value)
{
	m_common_data.xianmengzhan_rank_value = rank_value;
	m_is_change = true;
}

void Guild::ClearXianmengzhanRankValue()
{
	m_common_data.xianmengzhan_rank_value = 0;
	m_is_change = true;
}

void Guild::SetGuildBattleRank(int rank)
{
	m_common_data.guildbattle_rank = rank;
	m_is_change = true;
}

void Guild::ClearGuildBattleRank()
{
	m_common_data.guildbattle_rank = 0;
	m_is_change = true;
}

void Guild::SetTerritoryWarRank(int rank)
{
	m_common_data.territorywar_rank = rank;
	m_is_change = true;
}

void Guild::ClearTerritoryWarRank()
{
	m_common_data.territorywar_rank = 0;
	m_is_change = true;
}

void Guild::OnUplevelBoss(Role *role)
{
	if (role->GetSublock()->CheckHasLock()) return;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (m_common_data.boss_normal_call_count > 0)
	{
		role->NoticeNum(errornum::EN_GUILD_BOSS_CALLED);
		return;
	}

	const GuildBossConfig *boss_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetBossCfg(m_common_data.boss_level);
	if (NULL == boss_cfg)
	{
		return;
	}

	// 公会boss最后一个boss作为特殊等级boss
	if (m_common_data.boss_level + 2 >= LOGIC_CONFIG->GetGuildActivityCfg().GetGuildBossMaxCount())
	{
		role->NoticeNum(errornum::EN_GUILD_BOSS_UPLEVEL_MAX);
		return;
	}

	const GuildBossConfig *next_boss_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetBossCfg(m_common_data.boss_level + 1);
	if (NULL == next_boss_cfg || boss_cfg->uplevel_exp <= 0)
	{
		role->NoticeNum(errornum::EN_GUILD_BOSS_UPLEVEL_MAX);
		return;
	}

	const GuildActivityOther &other_cfg = LOGIC_CONFIG->GetGuildActivityCfg().OtherCfg();
	if (!knapsack->ConsumeItem(other_cfg.boss_uplevel_item_id, 1, "GuildBossUplevel"))
	{
		return;
	}

	m_common_data.boss_exp += other_cfg.boss_uplevel_item_exp;
	if (m_common_data.boss_exp >= boss_cfg->uplevel_exp)
	{
		m_common_data.boss_exp -= boss_cfg->uplevel_exp;
		m_common_data.boss_level++;
	}

	this->SyncGuildBossInfo(role->GetNetId());
}

void Guild::CallGuildBoss(Role *role, bool is_super_boss)
{
	if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILD_BOSS))
	{
		role->NoticeNum(errornum::EN_GUILD_BOSS_ACTIVITY_NOT_OPEN);
		return;
	}

	SpecialGuildStation *guild_logic = (SpecialGuildStation *)role->GetScene()->GetSpecialLogic();
	if (NULL == guild_logic || Scene::SCENE_TYPE_GUILD_STATION != guild_logic->GetSceneType())
	{
		role->NoticeNum(errornum::EN_GUILD_BOSS_NOT_IN_STATION);
		return;
	}

	if (guild_logic->GetGuildBossId() > 0)
	{
		role->NoticeNum(errornum::EN_GUILD_BOSS_BE_BEING);
		return;
	}

	if (m_guild_tianci_tongbi_is_open)
	{
		gstr::SendError(role, "en_guild_guild_boss_tianci_tongbi_limit");
		return;
	}

	const GuildActivityOther &other_cfg = LOGIC_CONFIG->GetGuildActivityCfg().OtherCfg();
	int boss_level = m_common_data.boss_level;
	if (is_super_boss)
	{
		if (m_common_data.boss_normal_call_count <= 0)
		{
			role->NoticeNum(errornum::EN_GUILD_BOSS_SUPER_CALL_LIMIT);
			return;
		}

		if (m_common_data.boss_super_call_count >= other_cfg.boss_super_call_max_count)
		{
			role->NoticeNum(errornum::EN_GUILD_BOSS_SUPER_CALL_COUNT_LIMIT);
			return;
		}

		if (!role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.boss_super_call_gold, "GuildBossSuperCall"))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		boss_level = m_common_data.boss_level + 1;

		// 召唤奖励
		const GuildBossConfig *boss_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetBossCfg(boss_level);
		if (NULL == boss_cfg)
		{
			return;
		}

		static MailContentParam contentparam; contentparam.Reset();
		const ItemBase *item_base = ITEMPOOL->GetItem(boss_cfg->super_call_item_reward.item_id);
		if (NULL != item_base)
		{
			contentparam.item_count = 1;
			contentparam.item_list[0].item_id = boss_cfg->super_call_item_reward.item_id;
			contentparam.item_list[0].num = boss_cfg->super_call_item_reward.num;
			contentparam.item_list[0].is_bind = (boss_cfg->super_call_item_reward.is_bind) ? 1 : 0;
			contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_guild_boss_call_content);
			if (length > 0)
			{
				MailRoute::MailToUser(role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}

		m_common_data.boss_super_call_count++;
		m_common_data.boss_super_call_uid = role->GetUID();
	}
	else
	{
		GuildMember *member = this->GetGuildMemberManager().GetMember(role->GetUserId());
		if (NULL == member || !GuildAuthority::CanCallBoss(member))
		{
			role->NoticeNum(errornum::EN_GUILD_BOSS_CALL_AUTORITY_LIMIT);
			return;
		}

		if (m_common_data.boss_normal_call_count >= other_cfg.boss_normal_call_max_count)
		{
			role->NoticeNum(errornum::EN_GUILD_BOSS_NORMAL_CALL_COUNT_LIMIT);
			return;
		}

		m_common_data.boss_normal_call_count++;
	}

	if (guild_logic->CallGuildBoss(role, boss_level, is_super_boss))
	{
		if (is_super_boss)
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_supper_boss_create);
			if (sendlen > 0)
			{
				this->GuildSystemMsg(sendlen, gamestring::GAMESTRING_BUF);
			}
		}
		else
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_boss_create);
			if (sendlen > 0)
			{
				this->GuildSystemMsg(sendlen, gamestring::GAMESTRING_BUF);
			}
		}
	}
	
	this->SyncGuildBossInfo();

	role->NoticeNum(noticenum::NT_GUILD_BOSS_CALL_SUCC);
}

void Guild::SyncGuildBossInfo(const GSNetID net_id)
{
	Protocol::SCGuildBossInfo msg;
	msg.boss_normal_call_count = m_common_data.boss_normal_call_count;
	msg.boss_super_call_count = m_common_data.boss_super_call_count;
	msg.boss_level = m_common_data.boss_level;
	msg.boss_exp = m_common_data.boss_exp;
	msg.boss_super_call_uid = m_common_data.boss_super_call_uid;
	msg.boss_super_call_name[0] = '\0';

	if (msg.boss_super_call_uid > 0)
	{
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(msg.boss_super_call_uid);
		if (NULL != user_node)
		{
			user_node->GetName(msg.boss_super_call_name);
		}
	}
 	
	if (INVALID_GSNETID != net_id)
 	{
 		EngineAdapter::Instance().NetSend(net_id, (const char*)&msg, sizeof(msg));
 	}
 	else
 	{
 		this->SendMessageToGuild(Protocol::GUILD_NOTIFY_TYPE_BOSS, (const char *)&msg, sizeof(msg));
 	}
}

void Guild::OnMemberKillMonter(Role *role, int monster_id)
{
	const BossFamilyMikuBossInfoCfg * miku_boss_info_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetMikuBossInfo(monster_id);
	const ActiveBossInfoCfg * active_boss_info_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetActiveBossInfo(monster_id);
	// 世界（秘窟）boss  和  活跃boss
	if(NULL != miku_boss_info_cfg || NULL != active_boss_info_cfg)
	{
		m_common_data.daily_kill_redbag_boss_times++;
		BossGuildRedbagCfg * boss_redbag_cfg = LOGIC_CONFIG->GetGuildConfig().GetBossGuildRedbagCfgByKillBossTimes(m_common_data.daily_kill_redbag_boss_times);
		if (boss_redbag_cfg != NULL)
		{
			m_common_data.daily_guild_relive_times += boss_redbag_cfg->guild_relive_times;

			int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_boss_red_bag, m_common_data.daily_kill_redbag_boss_times);
			if (len > 0) 
			{
				Guild *guild = GuildManager::Instance().GetGuild(m_guild_id);
				if (NULL != guild)
				{
					//guild->GuildSystemMsg(len, gamestring::GAMESTRING_BUF);
				}
			}
		}
		this->SyncGulidReliveTimesInfo();
	}
}

void Guild::OnMemberFetchRedbag(Role *role, int gold_num)
{
	static Protocol::FetchUserInfo info;
	info.gold_num = gold_num;
	info.uid = role->GetUID();
	role->GetName(info.name);
	m_daily_fetch_user_info.push_back(info);
}

void Guild::SendBossRedbagInfo(Role *role, int index)
{
	BossGuildRedbagCfg * boss_redbag_cfg = LOGIC_CONFIG->GetGuildConfig().GetBossGuildRedbagCfg(index);
	if (NULL == boss_redbag_cfg) return;

	UserCacheNode *leader_node = UserCacheManager::Instance().GetUserNode(m_member_manager.GetTuanZhangUid());
	if (leader_node == NULL) return;

	static Protocol::SCGuildBossRedBagInfo pro;

	pro.total_gold_num = boss_redbag_cfg->max_diamond;
	pro.leader_uid = leader_node->uid;
	leader_node->GetName(pro.leader_name);
	pro.creater_prof = leader_node->prof;
	pro.creater_sex = leader_node->sex;
	pro.creater_avator_timestamp = leader_node->avatar_timestamp;

	pro.guild_id = m_guild_id;
	F_STRNCPY(pro.guild_name, m_guild_name, sizeof(GameName));
	pro.fetch_user_count = 0;

	for (std::vector<Protocol::FetchUserInfo>::iterator it = m_daily_fetch_user_info.begin(); it != m_daily_fetch_user_info.end(); it++)
	{
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(it->uid);
		if (user_node != NULL)
		{
			Protocol::FetchUserInfo &user_info = pro.fetch_user_info_list[pro.fetch_user_count];

			user_info.uid = it->uid;
			user_node->GetName(user_info.name);
			user_info.gold_num = it->gold_num;
			pro.fetch_user_count++;
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pro, sizeof(pro));
}

void Guild::GetGuildRedPaperList(Role *role, Protocol::SCGuildRedPaperListInfo *cmd)
{
	if (NULL == role || NULL == cmd) return;

	int count = 0;
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	std::map<int, GuildRedPaperCfg> &red_paper_map =  LOGIC_CONFIG->GetGuildConfig().GetGuildRedPaperCfgMap();

	for(int i = 0; i < MAX_GUILD_RED_PAPER && count < Protocol::SCGuildRedPaperListInfo::MAX_PAPER_COUNT; i++)
	{
		GuildRedPaperItem &paper = m_guild_red_paper_list[i];
		
		if (RED_PAPER_INVALID == paper.red_paper_state)
		{
			continue;
		}

		if (now_sec > paper.invalid_time || red_paper_map.find(paper.red_paper_seq) == red_paper_map.end())
		{
			paper.role_uid = 0;
			paper.red_paper_state = RED_PAPER_INVALID;
			paper.red_paper_seq = 0;
			paper.red_paper_id = 0;
			paper.create_time = 0;
			paper.invalid_time = 0;
			paper.avator_timestamp = 0;
			paper.sex = 0;
			paper.prof = 0;
			paper.reserve = 0;

			continue;
		}

		if (RED_PAPER_CAN_CREATE == paper.red_paper_state)
		{
			cmd->paper_list[count].uid = paper.role_uid;
			F_STRNCPY(cmd->paper_list[count].role_name, paper.role_name, sizeof(cmd->paper_list[count].role_name));
			cmd->paper_list[count].red_paper_id = paper.red_paper_id;
			cmd->paper_list[count].invalid_time = paper.invalid_time;
			cmd->paper_list[count].red_paper_state = paper.red_paper_state;
			cmd->paper_list[count].red_paper_seq = paper.red_paper_seq;
			cmd->paper_list[count].red_paper_index = paper.red_paper_index;
			cmd->paper_list[count].all_fetch = 0;
			cmd->paper_list[count].avator_timestamp = paper.avator_timestamp;
			cmd->paper_list[count].sex = paper.sex;
			cmd->paper_list[count].prof = paper.prof;
			cmd->paper_list[count].reserve = paper.reserve;
			count++;
		}
		else if (RED_PAPER_CAN_FETCH == paper.red_paper_state)
		{
			std::map<int, RedPaper> &red_paper_map = RedPaperManager::Instance().GetRedPaperMap();
			std::map<int, RedPaper>::iterator red_paper_it = red_paper_map.find(paper.red_paper_id);
			if (red_paper_it == red_paper_map.end())
			{
				paper.role_uid = 0;
				paper.red_paper_state = RED_PAPER_INVALID;
				paper.red_paper_seq = 0;
				paper.red_paper_id = 0;
				paper.create_time = 0;
				paper.invalid_time = 0;
				paper.avator_timestamp = 0;
				paper.sex = 0;
				paper.prof = 0;
				paper.reserve = 0;
				continue;
			}

			if (paper.create_time > 0 && paper.create_time > role->GetRoleGuild()->GetLastJoinGuildTime())
			{
				cmd->paper_list[count].uid = paper.role_uid;
				F_STRNCPY(cmd->paper_list[count].role_name, paper.role_name, sizeof(cmd->paper_list[count].role_name));
				cmd->paper_list[count].red_paper_id = paper.red_paper_id;
				cmd->paper_list[count].invalid_time = paper.invalid_time;
				cmd->paper_list[count].red_paper_state = paper.red_paper_state;
				cmd->paper_list[count].red_paper_seq = paper.red_paper_seq;
				cmd->paper_list[count].red_paper_index = paper.red_paper_index;
				cmd->paper_list[count].all_fetch = 0;
				cmd->paper_list[count].avator_timestamp = paper.avator_timestamp;
				cmd->paper_list[count].sex = paper.sex;
				cmd->paper_list[count].prof = paper.prof;
				cmd->paper_list[count].reserve = paper.reserve;

				if (red_paper_it->second.fetch_user_map.find(role->GetUID()) != red_paper_it->second.fetch_user_map.end())
				{
					cmd->paper_list[count].red_paper_state = RED_PAPER_HAS_FETCH;
				}

				if ((red_paper_it->second.can_fetch_times - static_cast<int>(red_paper_it->second.fetch_user_map.size())) <= 0)
				{
					cmd->paper_list[count].all_fetch = 1;
				}

				count++;
			}
		}
	}

	cmd->count = count;
}

void Guild::AddGuildRedPaper(Role *role, int red_paper_seq, int red_paper_state, int red_paper_id, unsigned int invalid_time)
{
	if (NULL == role || red_paper_id < 0 || red_paper_state <= RED_PAPER_INVALID || red_paper_state >= RED_PAPER_HAS_FETCH) return;

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_sec > invalid_time) return;

	unsigned int min_invalid_time = 0;
	int replace_index = -1;

	for(int i = 0; i < MAX_GUILD_RED_PAPER; i++)
	{
		GuildRedPaperItem &paper = m_guild_red_paper_list[i];
		if (RED_PAPER_INVALID == paper.red_paper_state || now_sec > paper.invalid_time)
		{
			replace_index = i;
			break;
		}
		else
		{
			if (0 == min_invalid_time)
			{
				min_invalid_time = paper.invalid_time;
				replace_index = i;
			}
			else
			{
				if (min_invalid_time > paper.invalid_time)
				{
					min_invalid_time = paper.invalid_time;
					replace_index = i;
				}
			}
		}
	}

	if (replace_index >= 0 && replace_index < MAX_GUILD_RED_PAPER)
	{
		m_guild_red_paper_list[replace_index].role_uid = role->GetUID();
		F_STRNCPY(m_guild_red_paper_list[replace_index].role_name, role->GetName(), sizeof(m_guild_red_paper_list[replace_index].role_name));
		m_guild_red_paper_list[replace_index].red_paper_state = red_paper_state;
		m_guild_red_paper_list[replace_index].red_paper_seq = red_paper_seq;
		m_guild_red_paper_list[replace_index].red_paper_id = red_paper_id;
		m_guild_red_paper_list[replace_index].create_time = 0;
		m_guild_red_paper_list[replace_index].invalid_time = invalid_time;
		m_guild_red_paper_list[replace_index].red_paper_index = replace_index;
		m_guild_red_paper_list[replace_index].avator_timestamp = role->GetAvatarTimeStamp();
		m_guild_red_paper_list[replace_index].sex = role->GetSex();
		m_guild_red_paper_list[replace_index].prof = role->GetProf();
		m_guild_red_paper_list[replace_index].reserve = 0;

		if (RED_PAPER_CAN_FETCH == red_paper_state)
		{
			m_guild_red_paper_list[replace_index].create_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		}
	}
}

void Guild::SignalChatGuildPaperRole(Role *role, int red_paper_index)
{
	if (NULL == role) return;

	if (red_paper_index < 0 || red_paper_index >= MAX_GUILD_RED_PAPER) return;

	GuildRedPaperItem &paper = m_guild_red_paper_list[red_paper_index];

	if (RED_PAPER_CAN_CREATE != paper.red_paper_state || 0 == paper.role_uid) return;

	Role *be_notice_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(paper.role_uid));
	if (NULL == be_notice_role) return;

	CommonDataParam *common_data = be_notice_role->GetCommonDataParam();
	if(NULL == common_data) return;

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_sec > common_data->red_paper_be_singal_last_be_chat_time)
	{
		common_data->red_paper_be_singal_chat_count = 0;
	}

	if (common_data->red_paper_be_singal_chat_count >= LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig().singal_chat_max_time) return;

	common_data->red_paper_be_singal_chat_count++;
	common_data->red_paper_be_singal_last_be_chat_time = now_sec + LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig().singal_chat_interval;

	Protocol::SCSingleChat *single_chat = (Protocol::SCSingleChat*)gamestring::GAMESTRING_BUF;
	int length = SNPRINTF(gamestring::GAMESTRING_BUF + sizeof(Protocol::SCSingleChat), sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::SCSingleChat), 
		gamestring::g_red_paper_singal_chat);

	if (length <= 0) return;

	single_chat->header.msg_type = Protocol::MT_SINGLE_CHAT_SC;
	single_chat->from_uid = role->GetUID();
	single_chat->from_uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	single_chat->msg_length = length;
	role->GetName(single_chat->username);
	single_chat->sex = role->GetSex();
	single_chat->camp = role->GetCamp();
	single_chat->vip_level = role->GetVip()->GetVipLevel();
	single_chat->prof = role->GetProf();
	single_chat->authority_type = role->GetAuthority();
	single_chat->level = (short)role->GetLevel(); 
	single_chat->tuhaojin_color = role->GetTuHaoJin()->GetTuhaojinColor();
	single_chat->bigchatface_status = (char)role->GetBigChatFace()->GetBigChatFaceStatus();
	single_chat->personalize_window_bubble_type = role->GetPersonalizeWindow()->GetCurUseBubbleWindow();
	single_chat->personalize_window_avatar_type = role->GetPersonalizeWindow()->GetCurUseAvatarWindowForClient();
	single_chat->has_xianzunka_flag = role->GetHasXianzunkaFlag();
	single_chat->is_echo = 0;
	single_chat->content_type = 0;
	single_chat->avatar_timestamp = role->GetAvatarTimeStamp();
	single_chat->msg_timestamp = 0;

	int total_len = sizeof(Protocol::SCSingleChat) + length;

	EngineAdapter::Instance().NetSend(be_notice_role->GetNetId(), (const char*)gamestring::GAMESTRING_BUF, total_len);
}

bool Guild::CanCreateGuildPaper(Role *role, int red_paper_index)
{
	if (NULL == role || red_paper_index < 0 || red_paper_index >= MAX_GUILD_RED_PAPER)
	{
		return false;
	}

	GuildRedPaperItem &paper = m_guild_red_paper_list[red_paper_index];
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_sec > paper.invalid_time)
	{
		paper.role_uid = 0;
		paper.red_paper_state = RED_PAPER_INVALID;
		paper.red_paper_seq = 0;
		paper.red_paper_id = 0;
		paper.create_time = 0;
		paper.invalid_time = 0;
		paper.avator_timestamp = 0;
		paper.sex = 0;
		paper.prof = 0;
		paper.reserve = 0;
	}

	if (RED_PAPER_INVALID == paper.red_paper_state || role->GetUID() != paper.role_uid)
	{
		return false;
	}

	return true;
}

void Guild::ClearRedPaperByIndex(Role *role ,int red_paper_index)
{
	if (NULL == role || red_paper_index < 0 || red_paper_index >= MAX_GUILD_RED_PAPER) return;

	GuildRedPaperItem &paper = m_guild_red_paper_list[red_paper_index];
	if (paper.role_uid != role->GetUID()) return;

	paper.role_uid = 0;
	paper.red_paper_state = RED_PAPER_INVALID;
	paper.red_paper_seq = 0;
	paper.red_paper_id = 0;
	paper.create_time = 0;
	paper.invalid_time = 0;
	paper.avator_timestamp = 0;
	paper.sex = 0;
	paper.prof = 0;
	paper.reserve =0;
}

void Guild::RemoveMemberSyncPaperInfo(int uid)
{
	if (m_guild_red_paper_set.find(uid) != m_guild_red_paper_set.end())
	{
		m_guild_red_paper_set.erase(uid);

		for (int i = 0; i < MAX_GUILD_RED_PAPER; i++)
		{
			GuildRedPaperItem &paper = m_guild_red_paper_list[i];
			if (RED_PAPER_CAN_CREATE == paper.red_paper_state && paper.role_uid == uid)
			{
				paper.role_uid = 0;
				paper.red_paper_state = RED_PAPER_INVALID;
				paper.red_paper_seq = 0;
				paper.red_paper_id = 0;
				paper.create_time = 0;
				paper.invalid_time = 0;
				paper.avator_timestamp = 0;
				paper.sex = 0;
				paper.prof = 0;
				paper.reserve = 0;
			}
		}
	}
}

void Guild::CalcNextQuestionTime()
{
	const WGQuestionCfg *question_cfg = LOGIC_CONFIG->GetQuestionConfig().GetWGQuestionCfg(QUESTION_TYPE_GUILD);
	if (NULL == question_cfg) return;

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(static_cast<time_t>(now_sec)));
	unsigned today_sec = now_sec - today_zero_time;

	if (today_sec < static_cast<unsigned int>(question_cfg->begin_time))
	{
		m_question_next_begin = today_zero_time + question_cfg->begin_time;
	}
	else if (today_sec > static_cast<unsigned int>(question_cfg->end_time))
	{
		int next_day_refresh_interval = EngineAdapter::Instance().NextDayInterval(0, 0, 0) + question_cfg->begin_time;
		m_question_next_begin = now_sec + next_day_refresh_interval;
	}
	else
	{
		m_question_next_begin = now_sec + (question_cfg->question_interval - (today_sec - question_cfg->begin_time) % question_cfg->question_interval);
	}
}

void Guild::BrocastGuildQuestion(Role *role)
{
	if (m_question_id <= 0) return;

	static Protocol::SCQuestionBroadcast question_info;
	question_info.question_type = QUESTION_TYPE_GUILD;
	question_info.curr_question_id = m_question_id;

	question_info.curr_question_begin_time = m_question_cur_begin;
	question_info.curr_question_end_time = m_question_cur_end;
	question_info.next_question_begin_time = m_question_next_begin;

	LOGIC_CONFIG->GetQuestionConfig().GetQuestionStr(m_question_id, question_info.curr_question_str, QUESTION_TYPE_GUILD);
	LOGIC_CONFIG->GetQuestionConfig().GetAnswer0DescStr(m_question_id, question_info.curr_answer0_desc_str, QUESTION_TYPE_GUILD);
	LOGIC_CONFIG->GetQuestionConfig().GetAnswer1DescStr(m_question_id, question_info.curr_answer1_desc_str, QUESTION_TYPE_GUILD);
	LOGIC_CONFIG->GetQuestionConfig().GetAnswer2DescStr(m_question_id, question_info.curr_answer2_desc_str, QUESTION_TYPE_GUILD);
	LOGIC_CONFIG->GetQuestionConfig().GetAnswer3DescStr(m_question_id, question_info.curr_answer3_desc_str, QUESTION_TYPE_GUILD);

	if (NULL != role)
	{
		if (role->GetCommonDataParam()->guild_question_last_answer_time < m_question_cur_begin)
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&question_info, sizeof(Protocol::SCQuestionBroadcast));
		}
	}
	else
	{
		this->SendMessageToGuild(Protocol::GUILD_NOTIFY_TYPE_REP_PAPER, (const char *)&question_info, sizeof(question_info));

		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_question_system_message_guild,
			question_info.curr_question_str);
		if (length > 0)
		{
			this->GuildSystemMsg(length, gamestring::GAMESTRING_BUF, 0, NULL, SYS_MSG_ONLY_GUILD_QUESTION);
		}
	}
}

void Guild::OnAnswerGuildQuestion(Role *role, int choose)
{
	if (NULL == role) return;

	const WGQuestionCfg *question_cfg = LOGIC_CONFIG->GetQuestionConfig().GetWGQuestionCfg(QUESTION_TYPE_GUILD);
	if (NULL == question_cfg) return;

	if (role->GetLevel() < question_cfg->open_level)
	{
		role->NoticeNum(errornum::EN_QUESTION_LEVEL_LIMIT);
		return;
	}

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (m_question_id <= 0 || now_sec < m_question_cur_begin || now_sec > m_question_cur_end)
	{
		role->NoticeNum(errornum::EN_QUESTION_TIME_LIMIT);
		return;
	}

	CommonDataParam *common_data = role->GetCommonDataParam();
	if (NULL == common_data) return;

	if (common_data->guild_question_last_answer_time >= m_question_cur_begin)
	{
		role->NoticeNum(errornum::EN_QUESTION_HAS_ANSWER_LIMIT);
		return;
	}

	common_data->guild_question_last_answer_time = now_sec;

	static Protocol::SCQuestionAnswerResult result;
	result.answer_type = QUESTION_TYPE_GUILD;
	result.right_result = choose == m_question_answer ? 1 : 0;
	result.result = m_question_answer;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&result, sizeof(result));

	if (1 == result.right_result)
	{
		common_data->guild_question_right_num++;
		role->AddExp(static_cast<long long>((role->GetLevel() + 50) *question_cfg->right_exp_reward), "GuildQuestiong", Role::EXP_ADD_REASON_DEFAULT);
		this->AddQuestionRankItem(role, common_data->guild_question_right_num, now_sec);
	}
	else
	{
		role->AddExp(static_cast<long long>((role->GetLevel() + 50) *question_cfg->wrong_exp_reward), "GuildQuestiong", Role::EXP_ADD_REASON_DEFAULT);
	}

	if (m_question_cur_right_num < question_cfg->fast_reward_num && 1 == result.right_result)
	{
		if (m_question_cur_right_num >= 0 && m_question_cur_right_num < 3)
		{
			m_question_fast_uid[m_question_cur_right_num] = role->GetUID();
		}

		m_question_cur_right_num++;

		static MailContentParam contentparam;
		contentparam.Reset();
		int item_count = 0;

		for (; item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
		{
			if (question_cfg->fast_reward[item_count].item_id <= 0) break;

			contentparam.item_list[item_count].item_id = question_cfg->fast_reward[item_count].item_id;
			contentparam.item_list[item_count].num = question_cfg->fast_reward[item_count].num;
			contentparam.item_list[item_count].is_bind = (question_cfg->fast_reward[item_count].is_bind) ? 1 : 0;
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_question_fast_guild_reward_mail, m_question_cur_right_num);
		if (length > 0 && item_count > 0)
		{
			MailRoute::MailToUser(role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_question.printf(LL_INFO, "WorldQusestionReward::user[%d, %s], reward_item_1[%d],num_1[%d],reward_item_2[%d],num_2[%d],reward_item_3[%d],num_3[%d]",
				role->GetUID(), role->GetName(), contentparam.item_list[0].item_id, contentparam.item_list[0].num,
				contentparam.item_list[1].item_id, contentparam.item_list[1].num, contentparam.item_list[2].item_id, contentparam.item_list[2].num);
		}
	}

	role->SendQuestionRightAnswerNum();
}

void Guild::AddQuestionRankItem(Role *role, int right_answer_num, unsigned int change_timshamp)
{
	if (NULL == role) return;

	bool insert_item = true;

	std::vector<QuestionRankItem>::iterator begin_iter = m_question_rank_vec.begin();
	for (; begin_iter != m_question_rank_vec.end(); begin_iter++)
	{
		if (begin_iter->uid == role->GetUID())
		{
			insert_item = false;
			begin_iter->right_answer_num = right_answer_num;
			begin_iter->last_num_change_time = change_timshamp;
			return;
		}
	}

	if (insert_item)
	{
		QuestionRankItem item;
		item.uid = role->GetUID();
		item.right_answer_num = right_answer_num;
		item.last_num_change_time = change_timshamp;

		m_question_rank_vec.push_back(item);
	}
}

bool SortFuncByScore(const Guild::QuestionRankItem rankitem_1, const Guild::QuestionRankItem rankitem_2)
{
	if (rankitem_1.right_answer_num != rankitem_2.right_answer_num)
	{
		return rankitem_1.right_answer_num > rankitem_2.right_answer_num;
	}

	return rankitem_1.last_num_change_time < rankitem_2.last_num_change_time;
}

void Guild::CalcQuestionRankReward()
{
	if (m_question_rank_vec.empty()) return;

	std::sort(m_question_rank_vec.begin(), m_question_rank_vec.end(), &SortFuncByScore);

	int rank_size = static_cast<unsigned int> (m_question_rank_vec.size());
	for (int i = 0; i < rank_size; i++)
	{
		static MailContentParam contentparam;
		contentparam.Reset();
		int item_count = 0;
		int length = 0;

		QuestionGuildRankRewardCfg *rank_cfg = LOGIC_CONFIG->GetQuestionConfig().GetQuestionGuildRankReward(i + 1);
		if (NULL != rank_cfg)
		{
			for (; item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
			{
				if (rank_cfg->rank_reward[item_count].item_id <= 0) break;

				contentparam.item_list[item_count].item_id = rank_cfg->rank_reward[item_count].item_id;
				contentparam.item_list[item_count].num = rank_cfg->rank_reward[item_count].num;
				contentparam.item_list[item_count].is_bind = (rank_cfg->rank_reward[item_count].is_bind) ? 1 : 0;
			}

			length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_question_guild_rank_reward, i + 1);
		}
		else
		{
			const QuestionOtherCfg *other_cfg = LOGIC_CONFIG->GetQuestionConfig().GetOtherCfg();
			if (NULL != other_cfg)
			{
				for (; item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
				{
					if (other_cfg->guild_rank_other_reward[item_count].item_id <= 0) break;

					contentparam.item_list[item_count].item_id = other_cfg->guild_rank_other_reward[item_count].item_id;
					contentparam.item_list[item_count].num = other_cfg->guild_rank_other_reward[item_count].num;
					contentparam.item_list[item_count].is_bind = (other_cfg->guild_rank_other_reward[item_count].is_bind) ? 1 : 0;
				}
			}

			length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_question_guild_rank_other_reward);
		}

		if (length > 0 && item_count > 0)
		{
			MailRoute::MailToUser(IntToUid(m_question_rank_vec[i].uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}
	m_question_rank_vec.clear();
}

void Guild::SendGuildQuestionRankInfo(Role *role)
{
	static Protocol::SCQuestionGuildRankInfo rank_info;
	rank_info.rank_count = 0;

	for (int i = 0; i < static_cast<int>(m_question_rank_vec.size()) && i < Protocol::SCQuestionGuildRankInfo::RANK_NUM; ++i)
	{
		Protocol::SCQuestionGuildRankInfo::RankItem &rank_item = rank_info.rank_list[i];

		QuestionRankItem item = m_question_rank_vec[i];
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(item.uid);
		if (NULL == user_node) continue;
		user_node->GetName(rank_item.name);
		rank_item.uid = item.uid;
		rank_item.right_answer_num = item.right_answer_num;

		++rank_info.rank_count;
	}

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&rank_info, sizeof(Protocol::SCQuestionGuildRankInfo)
			- sizeof(Protocol::SCQuestionGuildRankInfo::RankItem) * (Protocol::SCQuestionGuildRankInfo::RANK_NUM - rank_info.rank_count));
	}
	else
	{
		this->SendMessageToGuild(Protocol::GUILD_NOTIFY_TYPE_REP_PAPER, (const char *)&rank_info, sizeof(Protocol::SCQuestionGuildRankInfo)
			- sizeof(Protocol::SCQuestionGuildRankInfo::RankItem) * (Protocol::SCQuestionGuildRankInfo::RANK_NUM - rank_info.rank_count));
	}
}

void Guild::RemoveMemberSyncQuestionRankInfo(int uid)
{
	std::vector<QuestionRankItem>::iterator begin_iter = m_question_rank_vec.begin();
	for (; begin_iter != m_question_rank_vec.end(); begin_iter++)
	{
		if (begin_iter->uid == uid)
		{
			m_question_rank_vec.erase(begin_iter);
			break;
		}
	}

	this->SendGuildQuestionRankInfo(NULL);
}

void Guild::OnGetMazeInfo(Role *role)
{
	this->SendMazeInfo(role);
	this->SendMazeRankInfo(role);
}

void Guild::OnMazeSelectDoor(Role *role, int door_id)
{
	if (role == nullptr) return;

	GuildMember *member = this->GetGuildMemberManager().GetMember(role->GetUserId());
	if (NULL == member)
	{
		return;
	}

	int layer = member->GetMazeLayer();
	if (layer < 0 || layer >= GUILD_MAZE_LAYER_MAX)
	{
		return;
	}

	GuildMazeCfg *maze_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildMazeCfg(layer + 1);
	if (NULL == maze_cfg)
	{
		return;
	}

	time_t now_time = EngineAdapter::Instance().Time();
	if (member->GetMazeCdEndTime() > now_time)
	{
		return;
	}

	int reason = Protocol::GUILD_MAZE_INFO_REASON_DEF;

	if (door_id == m_common_data.maze_layer_list[layer])
	{
		layer++;
		reason = Protocol::GUILD_MAZE_INFO_REASON_SUCC;

		if (layer > role->GetRoleGuild()->GetMazeRawardLayer())
		{
			role->GetRoleGuild()->SetMazeRawardLayer(layer);
			if (0 != maze_cfg->reward_item_list[0].item_id)
			{
				role->GetKnapsack()->PutListOrMail(maze_cfg->reward_item_list, PUT_REASON_GUILD_MAZE);
			}

			reason = Protocol::GUILD_MAZE_INFO_REASON_FIRST_SUCC;
		}
	}
	else
	{
		layer -= maze_cfg->fail_down_layer;
		if (layer < 0) layer = 0;

		reason = Protocol::GUILD_MAZE_INFO_REASON_FAIL;

		if (maze_cfg->fail_cd > 0)
		{
			member->SetMazeCdEndTime(now_time + maze_cfg->fail_cd);
		}
	}

	member->SetMazeLayer(layer);
	if (layer >= GUILD_MAZE_LAYER_MAX)
	{
		member->SetMazeCompleteTime(now_time);
		this->RecalcMazeRank();
		this->SendMazeRankInfo(role);
	}

	this->SendMazeInfo(role, reason);
}

void Guild::SetAutoClear(Role *role, short is_auto_clear)
{
	if (NULL == role) return;

	GuildMember *member = this->GetGuildMemberManager().GetMember(role->GetUserId());
	if (NULL == member) // 2：必须是该军团成员
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		return;
	}

	if (!GuildAuthority::CanSetAutoClear(member)) // 3：必须是有权限
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
		return;
	}

	m_common_data.is_auto_clear = is_auto_clear;

	GuildManager::Instance().SyncGuildBaseInfo(role);
}

void Guild::AutoClearGuildMember(unsigned int now_sec)
{
	if (now_sec <= m_next_clear_member_time) return;
	
	m_next_clear_member_time = static_cast<unsigned int>(now_sec + GUILD_CLEAR_MEMBER_INTERVAL);

	if (0 != m_common_data.is_auto_clear)
	{
		m_member_manager.AutoClearMember(now_sec);
	}
}

void Guild::OnMemberEnterGuildBattle(Role *role)
{
	if (this->GetJoinGuildBattleFlag() <= 0)
	{
		this->SetJoinGuildBattleFlag(1);
	}

	GuildMember *member = m_member_manager.GetMember(role->GetUserId());
	if (NULL != member)
	{
		member->SetHasJoinGuildBattle(true);
	}
}

void Guild::ClearGuildBattleJoinFlag()
{
	int member_uid_list[MAX_MEMBER_COUNT];
	int member_count = m_member_manager.GetMemberList(member_uid_list);
	for (int i = 0; i < member_count; ++i)
	{
		GuildMember *member = m_member_manager.GetMember(IntToUid(member_uid_list[i]));
		if (NULL != member && member->GetHasJoinGuildBattle())
		{
			member->SetHasJoinGuildBattle(false);
		}
	}
}

void Guild::SendBattleWinReward(int scene_index, int scene_id)
{
	const CrossGuildBattleOwnRewardCfg *rewrd_cfg = CrossGuildBattleConfig::Instance().GetOwnServerRewardCfg(scene_index);
	if (NULL == rewrd_cfg)
	{
		return;
	}

	// 工资
	{
		this->AddGuildGongzi(rewrd_cfg->guild_reward_gongzi);

		int length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSChannelChat), sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSChannelChat), "guild_gongzi_cross_guildbattle_chat",
			scene_id, rewrd_cfg->guild_reward_gongzi);
		if (length > 0)
		{
			Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)gamestring::GAMESTRING_BUF;
			cc->channel_type = chatdef::CHANNEL_TYPE_GUILD_SYSTEM;
			cc->msg_length = length;

			this->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat), chatdef::ORIGIN_TYPE_GUILD_SYSTEM_MSG);
		}
	}

	int member_uid_list[MAX_MEMBER_COUNT];
	int member_count = m_member_manager.GetMemberList(member_uid_list);
	for (int i = 0; i < member_count; ++i)
	{
		GuildMember *member = m_member_manager.GetMember(IntToUid(member_uid_list[i]));
		if (NULL != member && member->GetHasJoinGuildBattle()) // 参与过六界战场的成员才允许发邮件奖励
		{
			if (GUILD_POST_TUANGZHANG == member->GetPost())
			{
				gamecommon::SendMultiMailToRole(IntToUid(member_uid_list[i]), rewrd_cfg->guild_boss_reward_item, rewrd_cfg->guild_boss_reward_count, NULL, true, gamestring::g_guildbattle_guild_reward_content);
			}
			else
			{
				MailRoute::FastMailList(member_uid_list[i], rewrd_cfg->guild_reward_item, gamestring::g_guildbattle_guild_reward_content);
			}
		}
	}

	{
		//集体奖励（盟主无需参与也可获得奖励，高文均需求）
		gamecommon::SendMultiMailToRole(IntToUid(this->GetTuanzhuangRoleId()), rewrd_cfg->team_reward_item, rewrd_cfg->team_reward_item_count, NULL, true, gamestring::g_guildbattle_team_reward_content, scene_id);

		if (rewrd_cfg->team_reward_item_count > 0)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSChannelChat), sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSChannelChat),
				gamestring::g_guild_activity_team_reward_notice, CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE, rewrd_cfg->team_reward_item[0].item_id,
				rewrd_cfg->team_reward_item[0].num);

			if (length > 0)
			{
				Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)(gamestring::GAMESTRING_BUF);
				cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
				cc->msg_length = length;

				this->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat));
			}
		}
	}
}

void Guild::SendBattleJoinReward()
{
	const CrossGuildBattleOwnRewardCfg *rewrd_cfg = CrossGuildBattleConfig::Instance().GetOwnServerRewardCfg(0); // 默认拿第一个奖励作为参与奖
	if (NULL == rewrd_cfg)
	{
		return;
	}

	int member_uid_list[MAX_MEMBER_COUNT];
	int member_count = m_member_manager.GetMemberList(member_uid_list);
	for (int i = 0; i < member_count; ++i)
	{
		GuildMember *member = m_member_manager.GetMember(IntToUid(member_uid_list[i]));
		if (NULL != member && member->GetHasJoinGuildBattle()) // 参与过六界战场的成员才允许发邮件奖励
		{
			MailRoute::FastMailList(member_uid_list[i], rewrd_cfg->server_reward_item, gamestring::g_guildbattle_server_reward_content);
		}
	}
}

bool Guild::IsHadBiaoChe()
{
	if (m_biaoche_status != gglobalprotocal::YUNBIAO_STATUS_INVALID)
	{
		if (m_biaoche_status != gglobalprotocal::YUNBIAO_STATUS_COMPLETE && m_biaoche_status != gglobalprotocal::YUNBIAO_STATUS_FAIL)
		{
			return true;
		}
	}

	return false;
}

bool Guild::IsBiaoCheHadEnd()
{
	if (m_biaoche_status != gglobalprotocal::YUNBIAO_STATUS_INVALID)
	{
		if (m_biaoche_status != gglobalprotocal::YUNBIAO_STATUS_COMPLETE && m_biaoche_status != gglobalprotocal::YUNBIAO_STATUS_FAIL)
		{
			return false;
		}
	}
	else
	{
		return IsBiaoCheHadStart();
	}

	return true;
}

void Guild::OnYunBiaoStatus(gglobalprotocal::GuildYunBiaoStatus *gybs)
{
	switch (gybs->status)
	{
	case gglobalprotocal::YUNBIAO_STATUS_START:
	{
		m_biaoche_status = gybs->status;
	}
	break;

	case gglobalprotocal::YUNBIAO_STATUS_COMPLETE:
	{
		this->OnYunBiaoComplete(gybs);
		m_biaoche_status = gglobalprotocal::YUNBIAO_STATUS_INVALID;
		m_biaoche_pos.x = 0;
		m_biaoche_pos.y = 0;
	}
	break;

	case gglobalprotocal::YUNBIAO_STATUS_FAIL:
	{
		this->OnYunBiaoFail(gybs);
		m_biaoche_status = gglobalprotocal::YUNBIAO_STATUS_INVALID;
		m_biaoche_pos.x = 0;
		m_biaoche_pos.y = 0;
	}
	break;

	case gglobalprotocal::YUNBIAO_STATUS_UPDATE:
	{
		m_biaoche_pos.x = gybs->biaoche_pos_x;
		m_biaoche_pos.y = gybs->biaoche_pos_y;
	}
	break;

	case gglobalprotocal::YUNBIAO_STATUS_BE_HURT:
	{
		int biaoche_sceneid = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheSceneID();

		int length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "guild_biaoche_be_hurt",
			biaoche_sceneid, gybs->biaoche_pos_x, gybs->biaoche_pos_y);
		if (length > 0)
		{
			this->GuildSystemMsg(length, gamestring::GAMESTRING_BUF, 0, NULL, SYS_MSG_ONLY_CHAT_GUILD);
		}
	}
	break;

	case gglobalprotocal::YUNBIAO_STATUS_ACTIVITY_END_FAIL:
	{
		this->OnYunBiaoActivityEndFail();
		m_biaoche_status = gglobalprotocal::YUNBIAO_STATUS_INVALID;
		m_biaoche_pos.x = 0;
		m_biaoche_pos.y = 0;
	}
	break;

	}
}

void Guild::OnYunBiaoComplete(gglobalprotocal::GuildYunBiaoStatus *gybs)
{
// 	const int total_exp = LOGIC_CONFIG->GetGuildConfig().GetTotalBiaoCheGuildExp(gybs->guild_lv);
// 	this->AddGuildExp(total_exp);

	const BiaoCheLvInfo *biaoche_cfg = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheInfoCfg(gybs->guild_lv);
	if (nullptr != biaoche_cfg)
	{
		static MailContentParam reward_mail;  reward_mail.Reset();

		for (int i = 0; i < biaoche_cfg->succ_reward_count && i < MAX_ATTACHMENT_ITEM_NUM; i++)
		{
			reward_mail.item_list[i].item_id = biaoche_cfg->succ_reward_item_list[i].item_id;
			reward_mail.item_list[i].num = biaoche_cfg->succ_reward_item_list[i].num;
			reward_mail.item_list[i].is_bind = (biaoche_cfg->succ_reward_item_list[i].is_bind ? 1 : 0);
			const ItemBase *itembase = ITEMPOOL->GetItem(biaoche_cfg->succ_reward_item_list[i].item_id);
			if (NULL != itembase) reward_mail.item_list[i].invalid_time = itembase->CalInvalidTime();
		}

		reward_mail.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GUILDGONGXIAN, biaoche_cfg->succ_gongxian);

		int length1 = gstr::MailContent(reward_mail.subject, sizeof(reward_mail.subject), "guild_biaoche_complete_subject");
		int length2 = gstr::MailContent(reward_mail.contenttxt, sizeof(reward_mail.contenttxt), "guild_biaoche_complete_content");

		if (length1 > 0 && length2 > 0)
		{
//			m_member_manager.SystemMailToAll(reward_mail);
			this->SendYunBiaoRewardMail(reward_mail, nullptr);
		}

		{
			//策划需求，镖车运送成功奖励经验
			int member_uid_list[MAX_MEMBER_COUNT]; memset(member_uid_list, 0, sizeof(member_uid_list));
			int member_count = 0;

			member_count = m_member_manager.GetMemberList(member_uid_list);

			for (int i = 0; i < member_count && i < MAX_MEMBER_COUNT; ++i)
			{
				Role *temp_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
				if (NULL == temp_role)
				{
					continue;
				}

				int biaoche_sceneid = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheSceneID();
				if (biaoche_sceneid != temp_role->GetScene()->GetSceneID())
				{
					continue;
				}

				long long add_exp = static_cast<long long>(biaoche_cfg->succ_reward_exp_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(temp_role->GetLevel(), 0)));
				temp_role->AddExp(add_exp, "guild_biaoche_complete_subject", Role::EXP_ADD_REASON_DEFAULT);
				EventHandler::Instance().OnEnterGuildYunBiao(temp_role);
			}

			EventHandler::Instance().OnAddGuildRumorEvent(this->GetGuildID(), NULL, NULL, GUILD_EVENT_TYPE_RUMOR_YUNBIAO_SUCC);
		}
	}

	this->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_GUILD_BIAOCHE_END, NULL, GUILD_YUNBIAO_RESULT_TYPE_SUCC, m_open_biaoche_role_id);

	int length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "guild_biaoche_succ_notice");
	if (length > 0)
	{
		this->GuildSystemMsg(length, gamestring::GAMESTRING_BUF);
	}
}

void Guild::OnYunBiaoFail(gglobalprotocal::GuildYunBiaoStatus *gybs)
{
// 	const int total_exp = LOGIC_CONFIG->GetGuildConfig().GetTotalBiaoCheGuildExp(gybs->guild_lv);
// 	const int rob_exp = LOGIC_CONFIG->GetGuildConfig().GetRobBiaoCheGuildExp(gybs->guild_lv);

	{
// 		int own_exp = total_exp - rob_exp;
// 		this->AddGuildExp(own_exp);

		Guild *rob_guild = m_guild_manager->GetGuild(gybs->rob_guild_id);
		if (NULL != rob_guild)
		{
			// 运镖失败奖励
			this->OnRewardYunBiaoFail(rob_guild->GetGuildName());

			// 抢劫成功奖励
			{
				const int guild_rob_count_max = LOGIC_CONFIG->GetGuildConfig().GetGuildRobBiaoCheCountMax();
				if (rob_guild->GetRobBiaoCheCount() < guild_rob_count_max) // 一天最多截3次其他帮派镖车
				{
//					rob_guild->AddGuildExp(rob_exp);

					const BiaoCheLvInfo *biaoche_cfg = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheInfoCfg(1);
					if (nullptr != biaoche_cfg)
					{
						static MailContentParam reward_mail;  reward_mail.Reset();

						for (int i = 0; i < biaoche_cfg->rob_reward_count && i < MAX_ATTACHMENT_ITEM_NUM; i ++)
						{
							reward_mail.item_list[i].item_id = biaoche_cfg->rob_reward_item_list[i].item_id;
							reward_mail.item_list[i].num = biaoche_cfg->rob_reward_item_list[i].num;
							reward_mail.item_list[i].is_bind = (biaoche_cfg->rob_reward_item_list[i].is_bind ? 1 : 0);
							const ItemBase *itembase = ITEMPOOL->GetItem(biaoche_cfg->rob_reward_item_list[i].item_id);
							if (NULL != itembase) reward_mail.item_list[i].invalid_time = itembase->CalInvalidTime();
						}

						reward_mail.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GUILDGONGXIAN, biaoche_cfg->rob_gongxian);

						int length1 = gstr::MailContent(reward_mail.subject, sizeof(reward_mail.subject), "guild_biaoche_rob_subject");
						int length2 = gstr::MailContent(reward_mail.contenttxt, sizeof(reward_mail.contenttxt), "guild_biaoche_rob_content", this->GetGuildName());

						if (length1 > 0 && length2 > 0)
						{
							this->SendYunBiaoRewardMail(reward_mail, rob_guild);
						}
					}
				}

				rob_guild->AddRobBiaoCheCount();  // 增加次数
			}

			// 抢劫成功传闻
			{
				int length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "guild_biaoche_rob_notice",
					rob_guild->GetGuildName(), this->GetGuildName());
				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVITY_SPECIAL);
				}
			}

			rob_guild->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_GUILD_BIAOCHE_END, NULL, GUILD_YUNBIAO_RESULT_TYPE_ROB_SUCC, rob_guild->GetRobBiaoCheCount());
		}
	}

	this->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_GUILD_BIAOCHE_END, NULL, GUILD_YUNBIAO_RESULT_TYPE_FAIL, m_open_biaoche_role_id);
}

void Guild::OnYunBiaoActivityEndFail()
{
	this->OnRewardYunBiaoFail(nullptr);
	this->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_GUILD_BIAOCHE_END, NULL, GUILD_YUNBIAO_RESULT_TYPE_FAIL, m_open_biaoche_role_id);
}

void Guild::OnTransToBiaoCheReq(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	if (gglobalprotocal::YUNBIAO_STATUS_START != m_biaoche_status)
	{
		return;
	}

	if (!role->GetRoleStatusManager()->CanTransmit())
	{
		return;
	}

	int scene_id = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheSceneID();

	if (!role->GetRoleStatusManager()->CanFlyByShoe(scene_id))
	{
		return;
	}

	if (m_biaoche_pos.x > 0 && m_biaoche_pos.y > 0)
	{

		World::GetInstWorld()->GetSceneManager()->GoTo(role, scene_id, 0, Posi(m_biaoche_pos.x, m_biaoche_pos.y));
	}
}

void Guild::OnGetBiaoChePos(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	Protocol::SCNotifyGuildSuper ngs;
	ngs.notify_type = Protocol::GUILD_NOTIFY_TYPE_GUILD_BIAOCHE_CUR_POS;
	ngs.notify_param = m_biaoche_pos.x;
	ngs.notify_param1 = m_biaoche_pos.y;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&ngs, sizeof(ngs));
}

void Guild::OnRewardYunBiaoFail(GuildName rob_guild_name)
{
	const BiaoCheLvInfo *biaoche_cfg = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheInfoCfg(1);
	if (nullptr == biaoche_cfg)
	{
		return;
	}

	static MailContentParam reward_mail;  reward_mail.Reset();

	for (int i = 0; i < biaoche_cfg->fail_reward_count && i < MAX_ATTACHMENT_ITEM_NUM; i++)
	{
		reward_mail.item_list[i].item_id = biaoche_cfg->fail_reward_item_list[i].item_id;
		reward_mail.item_list[i].num = biaoche_cfg->fail_reward_item_list[i].num;
		reward_mail.item_list[i].is_bind = (biaoche_cfg->fail_reward_item_list[i].is_bind ? 1 : 0);
		const ItemBase *itembase = ITEMPOOL->GetItem(biaoche_cfg->fail_reward_item_list[i].item_id);
		if (NULL != itembase) reward_mail.item_list[i].invalid_time = itembase->CalInvalidTime();
	}

	reward_mail.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GUILDGONGXIAN, biaoche_cfg->fail_gongxian);

	int length1 = gstr::MailContent(reward_mail.subject, sizeof(reward_mail.subject), "guild_biaoche_fail_subject");
	int length2 = 0;

	if (nullptr != rob_guild_name)
	{
		length2 = gstr::MailContent(reward_mail.contenttxt, sizeof(reward_mail.contenttxt), "guild_biaoche_fail_content", rob_guild_name);
	}
	else
	{
		length2 = gstr::MailContent(reward_mail.contenttxt, sizeof(reward_mail.contenttxt), "guild_biaoche_activity_end_fail_content");
	}

	if (length1 > 0 && length2 > 0)
	{
		this->SendYunBiaoRewardMail(reward_mail, nullptr);
	}

	{
		//策划需求，镖车运送失败奖励经验
		int member_uid_list[MAX_MEMBER_COUNT]; memset(member_uid_list, 0, sizeof(member_uid_list));
		int member_count = 0;

		member_count = m_member_manager.GetMemberList(member_uid_list);

		for (int i = 0; i < member_count && i < MAX_MEMBER_COUNT; ++i)
		{
			Role *temp_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
			if (NULL == temp_role)
			{
				continue;
			}

			int biaoche_sceneid = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheSceneID();
			if (biaoche_sceneid != temp_role->GetScene()->GetSceneID())
			{
				continue;
			}

			long long add_exp = static_cast<long long>(biaoche_cfg->fail_reward_exp_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(temp_role->GetLevel(), 0)));
			temp_role->AddExp(add_exp, "OnRewardYunBiaoFail", Role::EXP_ADD_REASON_DEFAULT);
			EventHandler::Instance().OnEnterGuildYunBiao(temp_role);
		}

		EventHandler::Instance().OnAddGuildRumorEvent(this->GetGuildID(), NULL, rob_guild_name, GUILD_EVENT_TYPE_RUMOR_YUNBIAO_ROBBED);
	}
}

void Guild::SendYunBiaoRewardMail(const MailContentParam &contentparam, Guild *guild)
{
	int member_uid_list[MAX_MEMBER_COUNT]; memset(member_uid_list, 0, sizeof(member_uid_list));
	int member_count = 0;

	if (nullptr == guild)
	{
		member_count = m_member_manager.GetMemberList(member_uid_list);
	}
	else
	{
		member_count = guild->GetGuildMemberManager().GetMemberList(member_uid_list);
	}
	
	for (int i = 0; i < member_count && i < MAX_MEMBER_COUNT; ++i)
	{
		Role *temp_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
		if (NULL == temp_role)
		{
			continue;
		}

		int biaoche_sceneid = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheSceneID();
		if (biaoche_sceneid != temp_role->GetScene()->GetSceneID())
		{
			continue;
		}

		MailRoute::MailToUser(temp_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		EventHandler::Instance().OnEnterGuildYunBiao(temp_role);
	}
}

void Guild::OnSignin(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (!m_member_manager.IsSigninSucc(role))
	{
		role->NoticeNum(errornum::EN_GUILD_TOTAY_IS_SIGNIN);
		return;
	}

	// 奖励
	const GuildActivityOther &other_cfg = LOGIC_CONFIG->GetGuildActivityCfg().OtherCfg();

	role->GetKnapsack()->GetMoney()->AddGoldBind(other_cfg.signin_gold_bind, "Welfare::FetchGuildSigninReward");
	role->AddGuildGongxian(other_cfg.signin_gongxian, "Welfare::FetchGuildSigninReward");
	role->GetKnapsack()->PutListOrMail(other_cfg.signin_item, PUT_REASON_GUILD_SIGNIN);

	int cfg_count = LOGIC_CONFIG->GetGuildActivityCfg().GetSignInCfgCount();
	const GuildSigninCfg *signin_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetSigninCfg(cfg_count - 1);
	if (nullptr != signin_cfg)
	{
		int length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "guildsignup",
			role->GetUID(), role->GetName(), role->GetCamp(), m_member_manager.GetMemberSigninTotalCountToday(), signin_cfg->need_count);

		if (length > 0)
		{
			this->GuildSystemMsg(length, gamestring::GAMESTRING_BUF);
		}
	}

	this->SendSigninAllInfo(role);
}

void Guild::FethSinginReward(Role *role, int index)
{
	if (NULL == role)
	{
		return;
	}

	GuildMember *invite_member = m_member_manager.GetMember(role->GetUserId());
	if (NULL == invite_member)
	{
		return;
	}

	const GuildSigninCfg  *signin_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetSigninCfg(index);
	if (NULL == signin_cfg)
	{
		return;
	}

	char &fetch_flag = role->GetCommonDataParam()->guild_signin_fetch_reward_flag;
	if (0 != (fetch_flag & (1 << index)))
	{
		return;
	}

	int member_signin_count = m_member_manager.GetMemberSigninTotalCountToday();
	if (member_signin_count < signin_cfg->need_count)
	{
		return;
	}

	fetch_flag |= (1 << index);
	role->GetKnapsack()->PutOrMail(signin_cfg->reward_item, PUT_REASON_GUILD_SIGNIN);

	this->SendSigninAllInfo(role);
}

void Guild::SendSigninAllInfo(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	GuildMember *member = m_member_manager.GetMember(role->GetUserId());
	if (NULL == member)
	{
		return;
	}

	CommonDataParam *common_data = role->GetCommonDataParam();

	static Protocol::SCGuildSinginAllInfo gsai;

	unsigned int zero_timestamp = static_cast<unsigned int>(GetZeroTime(EngineAdapter::Instance().Time()));

	gsai.is_signin_today = common_data->last_signin_timestamp > zero_timestamp ? 1 : 0;
	gsai.guild_signin_fetch_reward_flag = common_data->guild_signin_fetch_reward_flag;
	gsai.signin_count_month = member->GetCommonData().signin_count_day;
	gsai.guild_signin_count_today = m_member_manager.GetMemberSigninTotalCountToday();

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gsai, sizeof(gsai));
}

bool Guild::IsTianCiTongBiOpen(Role *role)
{
	if (nullptr == role)
	{
		return false;
	}

	if (m_guild_tianci_tongbi_is_open)
	{
		SpecialGuildStation *special_logic = World::GetInstWorld()->GetSceneManager()->GetGuildStationSpecialLogic(role->GetGuildID(), true);
		if (NULL != special_logic)
		{
			special_logic->SendTianCiTongBiRankInfo(role);
		}
	}

	return m_guild_tianci_tongbi_is_open;
}

int Guild::OnTianCiTongBiReq(GuildMember *member)
{
	if (NULL == member)
	{
		return -1;
	}

	if (!(m_guild_tianci_tongbi_day_count >= GUILD_TIANCI_TONGBI_DAY_COUNT_MAX))
	{
		//if (m_guild_level < 2)
		//{
		//	World::GetInstWorld()->GetSceneManager()->NoticeToUser(member->GetUserID(), errornum::EN_GUILD_TIANCI_TONGBI_GUILD_LEVEL_LIMIT);
		//	return -3;
		//}

		++m_guild_tianci_tongbi_day_count;
		m_guild_tianci_tongbi_is_open = true;

		//Protocol::SCGuildSyncTianCiTongBi gstctb;
		//gstctb.guild_id = m_guild_id;
		//gstctb.is_open = 1;
		//World::GetInstWorld()->SendToAllGateway((const char*)&gstctb, sizeof(gstctb));


		int length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "guild_tiancitongbi_notice", m_guild_name);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
		}

		return 0;
	}
	else
	{
		gstr::SendError(member->GetUserID(), "en_guild_tianci_tongbi_daycount_limit");
		return -3;
	}
}

void Guild::SetTianCiTongBiOpentatus(bool parameter)
{
	m_guild_tianci_tongbi_is_open = parameter; 

	if (!parameter)
	{
		this->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_TIANCI_TONGBI_CLOSE, nullptr, 1);
	}
}

void Guild::OnGuildTianCiTongBiOpenReq(const UserID & user_id, Protocol::CSGuildTianCiTongBiReq * gtctbq)
{
	Guild *guild = GuildManager::Instance().GetGuild(user_id);

	if (NULL != guild) // 1：军团必须存在
	{
		GuildMember *member = guild->GetGuildMemberManager().GetMember(user_id);
		if (NULL != member) // 2：必须是该军团成员
		{
			if (GuildAuthority::CanOpenTianCiTongBi(member)) // 3：必须是有权限
			{
				Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
				if (nullptr == role)
				{
					return;
				}

				if (INVALID_GUILD_ID == gtctbq->param_1 || role->GetGuildID() != gtctbq->param_1)
				{
					role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
					return;
				}

				if (Scene::SCENE_TYPE_GUILD_STATION != role->GetScene()->GetSceneType())
				{
					gstr::SendError(role, "en_guild_tianci_tongbi_scene_limit");
					return;
				}

				if (ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILD_BOSS))
				{
					gstr::SendError(role, "en_guild_tianci_tongbi_guild_boss_limit");
					return;
				}

				int ret = guild->OnTianCiTongBiReq(member);
				if (ret == 0)
				{
					SpecialGuildStation *guild_logic = (SpecialGuildStation *)role->GetScene()->GetSpecialLogic();
					if (NULL != guild_logic)
					{
						guild_logic->OnOpenTianCiTongBi();

						this->NotifyMsgToGuild(Protocol::GUILD_NOTIFY_TYPE_TIANCI_TONGBI_OPEN, nullptr, 1);
					}
				}
				else
				{
					return;
				}
			}
			else
			{
				World::GetInstWorld()->GetSceneManager()->NoticeToUser(user_id, errornum::EN_GUILD_YOU_NOT_HAVE_POWER);
			}
		}
		else
		{
			World::GetInstWorld()->GetSceneManager()->NoticeToUser(user_id, errornum::EN_GUILD_YOU_NOT_IN_GUILD);
		}
	}
	else
	{
		World::GetInstWorld()->GetSceneManager()->NoticeToUser(user_id, errornum::EN_GUILD_NO_EXIST);
	}
}

void Guild::SetLastActiveTime(time_t last_active_time)
{
	m_last_active_time = last_active_time;

	m_is_change = true;
}

void Guild::OnLevelChange(bool is_upgrade)
{
	m_event_manager.OnLevelChange(m_guild_level, is_upgrade);
}

void Guild::ResetMaze()
{
	if (GUILD_MAZE_LAYER_DOOR_MAX > 0)
	{
		for (int i = 0; i < GUILD_MAZE_LAYER_MAX; i++)
		{
			m_common_data.maze_layer_list[i] = RandomNum(GUILD_MAZE_LAYER_DOOR_MAX) + 1;
		}
	}

	m_maze_rank_vec.clear();
}

bool static MazeSortFunc(const Guild::MazeRankInfo &rank_item_1, const Guild::MazeRankInfo &rank_item_2)
{
	return rank_item_1.maze_complete_time < rank_item_2.maze_complete_time;
}

void Guild::RecalcMazeRank()
{
	m_maze_rank_vec.clear();

	static int member_uid_list[MAX_MEMBER_COUNT];
	memset(member_uid_list, 0, sizeof(member_uid_list));

	int member_count = m_member_manager.GetMemberList(member_uid_list);
	for (int i = 0; i < member_count && i < MAX_MEMBER_COUNT; ++i)
	{
		GuildMember *member = m_member_manager.GetMember(IntToUid(member_uid_list[i]));
		if (NULL != member && member->GetMazeCompleteTime() > 0)
		{
			m_maze_rank_vec.push_back(MazeRankInfo(member_uid_list[i], member->GetMazeCompleteTime()));
		}
	}

	std::sort(m_maze_rank_vec.begin(), m_maze_rank_vec.end(), &MazeSortFunc);
}

void Guild::CheckMazeRankReward()
{
	this->RecalcMazeRank();

	int rank = 0;
	for (std::vector<MazeRankInfo>::iterator it = m_maze_rank_vec.begin(); it != m_maze_rank_vec.end(); it++)
	{
		static MailContentParam contentparam; contentparam.Reset();

		GuildMazeRankRewardCfg *cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildMazeRankRewardCfg(rank);
		if (NULL == cfg)
		{
			break;
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_guild_maze_rank_reward, rank + 1);
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; i++)
		{
			if (0 != cfg->reward_item_list[i].item_id)
			{
				contentparam.AddItem(cfg->reward_item_list[i].item_id, cfg->reward_item_list[i].num, cfg->reward_item_list[i].is_bind);
			}
		}
		if (length > 0)
		{
			MailRoute::MailToUser(IntToUid((*it).uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}

		rank++;
	}
}

void Guild::SendMazeInfo(Role *role, int reason)
{
	if (role == nullptr) return;

	GuildMember *member = m_member_manager.GetMember(role->GetUserId());
	if (NULL == member)
	{
		return;
	}

	Protocol::SCGuildMemberMazeInfo pro;
	pro.reason = reason;
	pro.layer = member->GetMazeLayer();
	pro.complete_time = (unsigned int)member->GetMazeCompleteTime();
	pro.cd_end_time = (unsigned int)member->GetMazeCdEndTime();
	pro.max_layer = role->GetRoleGuild()->GetMazeRawardLayer();
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, sizeof(pro));
}

void Guild::SendMazeRankInfo(Role *role)
{
	if (role == nullptr) return;

	Protocol::SCGuildMazeRankInfo pro;
	pro.rank_count = 0;

	for (std::vector<MazeRankInfo>::iterator it = m_maze_rank_vec.begin(); it != m_maze_rank_vec.end(); it++)
	{
		if (pro.rank_count >= MAX_MEMBER_COUNT)
		{
			break;
		}

		MazeRankInfo &info = *it;
		pro.rank_list[pro.rank_count].uid = info.uid;
		pro.rank_list[pro.rank_count].complete_time = (unsigned int)info.maze_complete_time;

		UserCacheNode *cache_node = UserCacheManager::Instance().GetUserNode(info.uid);
		if (NULL != cache_node)
		{
			cache_node->GetName(pro.rank_list[pro.rank_count].user_name);
		}

		pro.rank_count++;
	}

	int length = sizeof(pro) - (MAX_MEMBER_COUNT - pro.rank_count) * sizeof(pro.rank_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, length);
}

void Guild::AddMemberKillCrossBoss(UserID uid,int value)
{
	GuildOtherConfig & other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();
	int open_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	if (open_day < other_cfg.kill_cross_boss_open_day)
	{
		return;
	}

	if(m_member_manager.GetMember(uid) != nullptr && value > 0)
	{
		m_today_kill_cross_boss_num += value;
	}
}

void Guild::AddCrossGoalkillCrossBoss(UserID uid, int value)
{
	const CrossGoalOtherCfg & other_cfg = LOGIC_CONFIG->GetCrossGoalConfig().GetOtherCfg();
	int open_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex();
	if (open_day < other_cfg.open_day_beg || open_day > other_cfg.open_day_end)
	{
		return;
	}

	if (m_member_manager.GetMember(uid) != nullptr && value > 0)
	{
		m_cross_goal_kill_cross_boss += value;
		CheckGuildCrossGoalFinish(CROSS_GOAL_COND_GUILD_KILL_CROSS_BOSS);
	}
}

void Guild::AddCrossGoalkillBaiZhanBoss(UserID uid, int value)
{
	const CrossGoalOtherCfg & other_cfg = LOGIC_CONFIG->GetCrossGoalConfig().GetOtherCfg();
	int open_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() ;
	if (open_day < other_cfg.open_day_beg || open_day > other_cfg.open_day_end)
	{
		return;
	}

	if (value > 0)
	{
		m_cross_goal_kill_baizhan_boss += value;
		CheckGuildCrossGoalFinish(CROSS_GOAL_COND_GUILD_KILL_BAIZHAN_BOSS);
	}
}

void Guild::CheckGuildCrossGoalFinish(int cond_type)
{
	bool have_act = false;
	if (CROSS_GOAL_COND_GUILD_KILL_CROSS_BOSS == cond_type)
	{
		have_act = LOGIC_CONFIG->GetCrossGoalConfig().HaveGuildGoalFinish(cond_type, m_cross_goal_kill_cross_boss, &m_cross_goal_guild_flag);
	}
	else if(CROSS_GOAL_COND_GUILD_KILL_BAIZHAN_BOSS == cond_type)
	{
		have_act = LOGIC_CONFIG->GetCrossGoalConfig().HaveGuildGoalFinish(cond_type, m_cross_goal_kill_baizhan_boss, &m_cross_goal_guild_flag);
	}
//	printf("--------cb:%d,bz:%d,fg:%d\n", m_cross_goal_kill_cross_boss, m_cross_goal_kill_baizhan_boss, m_cross_goal_guild_flag);
	if (have_act)
	{
		bool is_all_finish = LOGIC_CONFIG->GetCrossGoalConfig().CheckGuildFinalGoal(&m_cross_goal_guild_flag);

		int member_uid_list[MAX_MEMBER_COUNT] = { 0 };
		int count = m_member_manager.GetMemberList(member_uid_list);
		for (int i = 0; i < count; i++)
		{
			Role * role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
			if (NULL != role)
			{
				role->GetCrossGoal()->CheckGuildGoalStatus(true);
			}
		}
		GameName tuanzhang = {0};
		this->GetTuanzhuangName(tuanzhang);
		int len = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_goal_guild_one", tuanzhang);
		if (len > 0)
		{
			static Protocol::SCSystemMsg sm;
			int sendlen = 0;

			if (World::GetInstWorld()->GetSceneManager()->CreateSystemMsg(&sm, &sendlen, gstr::GSTR_BUF, len, SYS_MSG_ONLY_CHAT_GUILD))
			{
				SendMessageToGuildAll(Protocol::GUILD_NOTIFY_TYPE_GUILD_GOAL, (const char*)&sm, sendlen);
			}
		}
		
		if (is_all_finish)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_goal_guild_final", this->GetGuildName());
			if (lenth > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, lenth, SYS_MSG_CENTER_ROLL_2);
			}
		}
	}
}

void Guild::AddGuildKillBossNum(int num)
{
	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), EngineAdapter::Instance().Time()) + 1;
	int limi_open_day = CrossRankRewardConfig::Instance().GetCrossRankLimitOpenDay(CROSS_PERSON_RANK_TYPE_GUILD_KILL_BOSS);
	if (limi_open_day > 0 && opengame_day < limi_open_day)
	{
		return;
	}

	m_common_data.guild_kill_boss_num += num;
}

void Guild::CheckCrossGoalGuildNotify(unsigned int now_second)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	unsigned int  today_sec = now_second - static_cast<int>(GetZeroTime(now_second));
	const CrossGoalOtherCfg other_cfg = LOGIC_CONFIG->GetCrossGoalConfig().GetOtherCfg();
	if (today_sec >= other_cfg.guild_notify_sec && today_sec <= other_cfg.guild_notify_sec + 10 && !m_today_cross_goal_guild_notify)
	{
		int open_day_index = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time());
		const CrossGoalOtherCfg & other_cfg = LOGIC_CONFIG->GetCrossGoalConfig().GetOtherCfg();
		if (open_day_index >= other_cfg.open_day_beg && open_day_index <= other_cfg.open_day_end)
		{
			m_today_cross_goal_guild_notify = true; 
			Protocol::SCCrossGoalGuildNotify notify;
			notify.flag = m_cross_goal_guild_flag;
			notify.guild_kill_cross_boss = m_cross_goal_kill_cross_boss;
			notify.guild_kill_baizhan_boss = m_cross_goal_kill_baizhan_boss;

			SendMessageToGuildAll(Protocol::GUILD_NOTIFY_TYPE_GUILD_GOAL, (const char*)&notify, sizeof(notify));
		}
	}
}

void Guild::InitGuildEnemy(const GuildBaseParam *param)
{
	if (nullptr == param)
	{
		return;
	}

	m_enemy_list.clear();
	m_enemy_list_dirty = true;

	for (int i = 0; i < GUILD_ENEMY_RANK_MAX_COUNT; ++i)
	{
		const GuildEnemyInfo &info = param->guild_enemy_list[i];
		if (0 != info.enemy_uuid)
		{
			m_enemy_list.push_back(info);
		}
	}

	// 去重，去除旧的数据
 	std::sort(m_enemy_list.begin(), m_enemy_list.end(), [](const GuildEnemyInfo &rank_item_1, const GuildEnemyInfo &rank_item_2)->bool {
 			return rank_item_1.enemy_uuid > rank_item_2.enemy_uuid;
 	});
 
 	auto it = std::unique(m_enemy_list.begin(), m_enemy_list.end(), [](const GuildEnemyInfo &rank_item_1, const GuildEnemyInfo &rank_item_2)->bool {
 		return rank_item_1.enemy_uuid == rank_item_2.enemy_uuid;
 	});
 
 	m_enemy_list.erase(it, m_enemy_list.end());
}

void Guild::SaveGuildEnemy(GuildBaseParam *param)
{
	if (nullptr == param)
	{
		return;
	}

	// 首先清理旧数据
	memset(param->guild_enemy_list, 0, sizeof(param->guild_enemy_list));

	int vec_size = m_enemy_list.size();
	
	for (int i = 0; i < vec_size && i < GUILD_ENEMY_RANK_MAX_COUNT; ++i)
	{
		if (0 != m_enemy_list[i].enemy_uuid)
		{
			if (m_enemy_list[i].last_kill_timestamp <= 0)
			{
				m_enemy_list[i].last_kill_timestamp = (unsigned int)EngineAdapter::Instance().Time();
			}

			param->guild_enemy_list[i] = m_enemy_list[i];  // 仇人不会删除，所以有效的都塞进去
		}
	}
}

void Guild::AddGuildEnemy(const GuildEnemyInfo &enemy_info)
{
	if (0 == enemy_info.enemy_uuid || enemy_info.last_kill_timestamp <= 0)
	{
		return;
	}

	for (auto it = m_enemy_list.begin(); m_enemy_list.end() != it; ++it)
	{
		if (enemy_info.enemy_uuid == it->enemy_uuid)
		{
			it->kill_score += 1;  //增加一点仇恨值
			F_STRNCPY(it->enemy_name, enemy_info.enemy_name, sizeof(GameName));
			it->last_kill_timestamp = (unsigned int)EngineAdapter::Instance().Time();
			
			m_enemy_list_dirty = true;

			return;
		}
	}

	int size = (int)m_enemy_list.size();
	if (size >= GUILD_ENEMY_RANK_MAX_COUNT)
	{
		// 信息已满，则替换一个最旧的
		unsigned int last_time = 0; int index = -1;
		for (int i = 0; i < size; ++i)
		{
			if (0 == last_time || m_enemy_list[i].last_kill_timestamp < last_time)
			{
				last_time = m_enemy_list[i].last_kill_timestamp;
				index = i;
			}
		}

		if (index >= 0 && index < size && index < GUILD_ENEMY_RANK_MAX_COUNT)
		{
			m_enemy_list[index] = enemy_info;
			m_enemy_list[index].kill_score = 1;
			m_enemy_list[index].last_kill_timestamp = (unsigned int)EngineAdapter::Instance().Time();

			m_enemy_list_dirty = true;
		}
	}
	else
	{
		m_enemy_list.push_back(enemy_info);

		m_enemy_list_dirty = true;
	}
}

void Guild::RemoveGuildEnemy(const UniqueUserID &unqiue_user_id)
{
	const long long uuid = UniqueUserIDToLongLong(unqiue_user_id);

	for (auto it = m_enemy_list.begin(); m_enemy_list.end() != it; ++it)
	{
		if (uuid == it->enemy_uuid)
		{
			it->Reset();

			m_enemy_list_dirty = true;

			break;
		}
	}
}

void Guild::SortRankGuildEnemy()
{
	if (m_enemy_list_dirty)
	{
		auto rank_iter = std::remove_if(m_enemy_list.begin(), m_enemy_list.end(), [](const GuildEnemyInfo &rank_item)
		{
			return (0 == rank_item.enemy_uuid || rank_item.last_kill_timestamp <= 0);
		});

		m_enemy_list.erase(rank_iter, m_enemy_list.end());

		std::sort(m_enemy_list.begin(), m_enemy_list.end(), [](const GuildEnemyInfo &rank_item_1, const GuildEnemyInfo &rank_item_2)->bool {

			if (rank_item_1.kill_score == rank_item_2.kill_score)
			{
				return rank_item_1.last_kill_timestamp > rank_item_2.last_kill_timestamp;
			}
			else
			{
				return rank_item_1.kill_score > rank_item_2.kill_score;
			}
		});
		
		m_enemy_list_dirty = false;
	}
}

void Guild::SendGuildEnemy(Role *role, long long uuid /* = 0 */)
{
	this->SortRankGuildEnemy();  // 请求的时候检查排序

	static Protocol::SCGuildEnemyRankList gerl;
	gerl.count = 0;

	for (auto it = m_enemy_list.begin(); m_enemy_list.end() != it && gerl.count < GUILD_ENEMY_RANK_MAX_COUNT; ++it)
	{
		if (0 == it->enemy_uuid)
		{
			continue;
		}

		gerl.enemy_list[gerl.count].rank = gerl.count + 1;
		gerl.enemy_list[gerl.count].enemy_uuid = it->enemy_uuid;
		F_STRNCPY(gerl.enemy_list[gerl.count].enemy_name, it->enemy_name, sizeof(GameName));
		gerl.enemy_list[gerl.count].kill_score = it->kill_score;
		gerl.enemy_list[gerl.count].last_kill_timestamp = it->last_kill_timestamp;
		
		gerl.count++;
	}

	int sendlen = sizeof(gerl) - sizeof(gerl.enemy_list[0]) * (GUILD_ENEMY_RANK_MAX_COUNT - gerl.count);
	if (nullptr == role)
	{
		if (0 != uuid)
		{
			// 发送给跨服中的指定玩家
			CrossManager::Instance().UserProtocalTransferToGame(CROSS_FUNCTION_TYPE_COMMON, uuid, (const char*)&gerl, sendlen);
		}
		else
		{
			// 发送给全部成员
			this->SendMessageToGuildAll(Protocol::GUILD_NOTIFY_TYPE_ENEMY_CHANGE, (const char*)&gerl, sendlen);
		}
	}
	else
	{
		// 发送给玩家
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gerl, sendlen);
	}
}

