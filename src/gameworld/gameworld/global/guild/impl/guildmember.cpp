#include "guildmember.hpp"
#include "guildmembermanager.hpp"

#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"

#include "global/guild/impl/guildauthority.hpp"

#include "servercommon/taskdef.hpp"

#include "engineadapter.h"

#include "config/guildconfig.hpp"
#include "servercommon/commondata.hpp"
#include "global/rank/rankmanager.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/guildbattleconfignew.hpp"
#include "item/knapsack.h"

GuildMember::GuildMember(GuildMemberManager *member_manager)
	: m_member_manager(member_manager), m_user_id(INVALID_USER_ID), 
	m_post(GUILD_POST_INVALID), m_gongxian(0), m_total_gongxian(0), m_is_chongzhi(false), 
	m_is_online(false), m_join_time(0), m_last_login_time(0), 
	m_last_change_notice_time(0), m_last_mail_all_time(0), m_lucky_color(GUILD_MEMBER_LUCKY_COLOR_WHITE),
	m_today_guild_pao_saizi_times(0), m_today_last_guild_pao_saizi_time(0), m_today_guild_saizi_score(0),
	m_maze_layer(0), m_maze_complete_time(0), m_maze_cd_end_time(0)
{
}

GuildMember::~GuildMember()
{

}

void GuildMember::Init(const GuildMemberParam::Member &member_param)
{
	m_member_index = member_param.index;

	m_user_id = IntToUid(member_param.uid);

	m_post = member_param.post;

	m_gongxian = member_param.gongxian;
	m_total_gongxian = member_param.total_gongxian;

	m_join_time = member_param.join_time;
	m_last_login_time = member_param.last_login_time;

	if (GUILD_POST_TUANGZHANG == m_post)
	{
		m_member_manager->GetGuild()->GetGuildRegister().OnGuildTuanZhangChanged(m_user_id);
	}

	m_lucky_color = member_param.lucky_color;

	m_today_guild_pao_saizi_times = member_param.today_guild_pao_saizi_times;
	m_today_last_guild_pao_saizi_time = member_param.today_last_guild_pao_saizi_time;
	m_today_guild_saizi_score = member_param.today_guild_saizi_score;

	m_maze_layer = member_param.maze_layer;
	m_maze_complete_time = member_param.maze_complete_time;
	m_common_param = member_param.common_data;
}

void GuildMember::OnDayChange()
{
// 	m_lucky_color = GUILD_MEMBER_LUCKY_COLOR_WHITE;
// 	m_member_manager->SetDirty(m_member_index);

	if (this->IsOnline() || m_last_login_time + 24 * 3600 >= static_cast<unsigned int>(EngineAdapter::Instance().Time()))
	{
		m_member_manager->SetDirty(m_member_index);
	}

	this->SetTodayLastGuildPaoSaiziTime(0);
	if (0 != m_today_guild_pao_saizi_times)
	{
		m_today_guild_pao_saizi_times = 0;
		m_member_manager->SetDirty(m_member_index);
	}
	if (0 != m_today_guild_saizi_score)
	{
		m_today_guild_saizi_score = 0;
		m_member_manager->SetDirty(m_member_index);
	}

	this->SetMazeLayer(0);
	this->SetMazeCompleteTime(0);
}

void GuildMember::OnMonthChange()
{
	m_common_param.signin_count_day = 0;
}

void GuildMember::GetAttr(GuildMemberParam::Member *member_attr) const
{
	member_attr->index = m_member_index;

	member_attr->uid = UidToInt(m_user_id);
	member_attr->post = m_post;

	member_attr->gongxian = m_gongxian;
	member_attr->total_gongxian = m_total_gongxian;

	member_attr->join_time = m_join_time;
	member_attr->last_login_time = m_last_login_time;

	member_attr->lucky_color = m_lucky_color;

	member_attr->today_guild_pao_saizi_times = m_today_guild_pao_saizi_times;
	member_attr->today_last_guild_pao_saizi_time = m_today_last_guild_pao_saizi_time;
	member_attr->today_guild_saizi_score = m_today_guild_saizi_score;

	member_attr->maze_layer = m_maze_layer;
	member_attr->maze_complete_time = m_maze_complete_time;

	member_attr->common_data = m_common_param;
}

void GuildMember::SetLastLoginTime(time_t now_second)
{
	if (m_last_login_time != now_second)
	{
		m_last_login_time = now_second;
		m_member_manager->SetDirty(m_member_index);
	}
}

void GuildMember::SetLuckyColor(int color)
{
	if (color != m_lucky_color)
	{
		m_lucky_color = color;
		m_member_manager->SetDirty(m_member_index);
	}
}

void GuildMember::BeAppoint(int post)
{
	if (GUILD_POST_TUANGZHANG != m_post)
	{
		if (GUILD_POST_TUANGZHANG == post)	// 非团长 -> 团长
		{
			m_member_manager->GetGuild()->GetGuildRegister().OnGuildTuanZhangChanged(m_user_id);
		}
	}
	else if (GUILD_POST_TUANGZHANG == m_post)
	{
		if (GUILD_POST_TUANGZHANG != post)	// 团长 -> 非团长
		{
			m_member_manager->GetGuild()->GetGuildRegister().OnGuildTuanZhangChanged(INVALID_USER_ID);
		}
	}

	m_post = post;

	m_member_manager->SetDirty(m_member_index);

	Guild *guild = m_member_manager->GetGuild();
	if (NULL != guild)
	{
		m_member_manager->GetGuild()->GetGuildManager()->SyncToRole(m_user_id,  guild->GetGuildID(), guild->GetGuildName(), m_post);
	}
}

bool GuildMember::CheckNotifyPrincipal(int notify_type)
{
	UNSTD_STATIC_CHECK(30 == Protocol::GUILD_NOTIFY_TYPE_MAX);

	switch (notify_type)
	{
	case Protocol::GUILD_NOTIFY_TYPE_APPLYFOR:
		return GuildAuthority::CanVerify(this);

	case Protocol::GUILD_NOTIFY_TYPE_UNION_APPLYFOR:
		return GuildAuthority::CanUnion(this);

	case Protocol::GUILD_NOTIFY_TYPE_UNION_JOIN:
		return GuildAuthority::CanUnion(this);

	case Protocol::GUILD_NOTIFY_TYPE_UNION_QUIT:
		return GuildAuthority::CanUnion(this);

	case Protocol::GUILD_NOTIFY_TYPE_UNION_REJECT:
		return GuildAuthority::CanUnion(this);

	case Protocol::GUILD_NOTIFY_TYPE_UNION_APPLYFOR_SUCC:
		return GuildAuthority::CanUnion(this);

	case Protocol::GUILD_NOTIFY_TYPE_MEMBER_ADD:
	case Protocol::GUILD_NOTIFY_TYPE_MEMBER_REMOVE:
	case Protocol::GUILD_NOTIFY_TYPE_MEMBER_SOS:
	case Protocol::GUILD_NOTIFY_TYPE_MEMBER_HUNYAN:
	case Protocol::GUILD_NOTIFY_TYPE_REP_PAPER:
	case Protocol::GUILD_NOTIFY_TYPE_GUILD_FB:
	case Protocol::GUILD_NOTIFY_TYPE_GUILD_LUCKY:
	case Protocol::GUILD_NOTIFY_TYPE_BOSS:
	case Protocol::GUILD_NOTIFY_TYPE_GUILD_BONFIRE:
	case Protocol::GUILD_NOTIFY_TYPE_TOTEM_UPLEVEL:	
	case Protocol::GUILD_NOTIFY_TYPE_BOSS_CALL:				
	case Protocol::GUILD_NOTIFY_TYPE_CONSUME_RELIVE_TIMES:
	case Protocol::GUILD_NOTIFY_TYPE_GUILD_BIAOCHE_START:
	case Protocol::GUILD_NOTIFY_TYPE_GUILD_BIAOCHE_END:
	case Protocol::GUILD_NOTIFY_TYPE_GUILD_BOX_ASSIST:
	case Protocol::GUILD_NOTIFY_TYPE_TIANCI_TONGBI_OPEN:
	case Protocol::GUILD_NOTIFY_TYPE_GUILD_BIAOCHE_CUR_POS:
	case Protocol::GUILD_NOTIFY_TYPE_TIANCI_TONGBI_CLOSE:
	case Protocol::GUILD_NOTIFY_TYPE_TOTAL_GONGZI_CHNAGE:
	case Protocol::GUILD_NOTIFY_TYPE_GUILD_GOAL:
	case Protocol::GUILD_NOTIFY_TYPE_ENEMY_CHANGE:
	case Protocol::GUILD_NOTIFY_TYPE_STORAGE_LOG:

		return true;
	}

	return false;
}

void GuildMember::OnSyncInfo(int gongxian, int total_gongxian)
{
	m_gongxian = gongxian;
	m_total_gongxian = total_gongxian;

	m_member_manager->SetDirty(m_member_index);
}

void GuildMember::SetTodayLastGuildPaoSaiziTime(time_t value)
{
	if (value != m_today_last_guild_pao_saizi_time)
	{
		m_today_last_guild_pao_saizi_time = value;
		m_member_manager->SetDirty(m_member_index);
	}
}

void GuildMember::AddTodayGuildPaoSaiziTimes()
{
	m_today_guild_pao_saizi_times++;
	m_member_manager->SetDirty(m_member_index);
}

void GuildMember::AddTodayGuildSaiziScore(int value)
{
	m_today_guild_saizi_score += value;
	m_member_manager->SetDirty(m_member_index);
}

void GuildMember::SetMazeLayer(int maze_layer)
{
	if (maze_layer != m_maze_layer)
	{
		m_maze_layer = maze_layer;
		m_member_manager->SetDirty(m_member_index);
	}
}

void GuildMember::SetMazeCompleteTime(long long maze_complete_time)
{
	if (maze_complete_time != m_maze_complete_time)
	{
		m_maze_complete_time = maze_complete_time;
		m_member_manager->SetDirty(m_member_index);
	}
}

void GuildMember::SetHasJoinGuildBattle(bool has_join_guild_battle) 
{
	char value = has_join_guild_battle ? 1 : 0;;
	if (m_common_param.has_join_guild_battle != value)
	{
		m_common_param.has_join_guild_battle = value;
		m_member_manager->SetDirty(m_member_index);
	}
}

void GuildMember::OnFetchDailyReward(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	GuildRank *guild_rank = RankManager::Instance().GetGuildRank();
	if (nullptr != guild_rank)
	{
		int rank = guild_rank->GetGuildRankInTopRank(role->GetGuildID(), GUILD_RANK_TYPE_GUILDBATTLE, GUILD_BATTLE_NEW_REWARD_CITY_COUNT);
		if (rank != 0)
		{
			if (role->GetCommonDataParam()->guild_battle_fetch_daily_reward < 1 && nullptr != role)
			{
				const ItemConfigData *item_list = LOGIC_CONFIG->GetGuildBattleConfigNew().GetDailyReward(rank);
				if (nullptr != item_list)
				{
					role->GetKnapsack()->PutListOrMail(item_list, PUT_REASON_GUILD_BATTEL);
					++role->GetCommonDataParam()->guild_battle_fetch_daily_reward;
					this->SendDailyRewardTimes(role);
				}
			}
		}
	}
}

void GuildMember::SendDailyRewardTimes(Role *role, int rank)
{
	if (nullptr == role)
	{
		return;
	}

	GuildRank *guild_rank = RankManager::Instance().GetGuildRank();
	if (nullptr != guild_rank)
	{
		Protocol::SCSendGuildBattleDailyRewardFlag pro;
		pro.had_fetch = role->GetCommonDataParam()->guild_battle_fetch_daily_reward;
		pro.reserve_ch = 0;
		if (rank >= 0)
		{
			pro.rank = rank;
		}
		else
		{
			pro.rank = guild_rank->GetGuildRankInTopRank(role->GetGuildID(), GUILD_RANK_TYPE_GUILDBATTLE, GUILD_BATTLE_NEW_REWARD_CITY_COUNT);
		}
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, sizeof(pro));
	}
}

bool GuildMember::IsSigninSucc(Role *role)
{
	if (NULL == role) return false;

	unsigned int now_second = (unsigned int)EngineAdapter::Instance().Time();
	unsigned int zero_timestamp = static_cast<unsigned int>(GetZeroTime(now_second));
	unsigned int &timestamp = role->GetCommonDataParam()->last_signin_timestamp;
	unsigned int &member_timestamp = m_common_param.last_signin_timestamp;;
	if (timestamp >= zero_timestamp || member_timestamp >= zero_timestamp)
	{
		return  false;
	}
	// 消耗
	timestamp = now_second;
	member_timestamp = now_second;
	++m_common_param.signin_count_day;

	return true;
}

