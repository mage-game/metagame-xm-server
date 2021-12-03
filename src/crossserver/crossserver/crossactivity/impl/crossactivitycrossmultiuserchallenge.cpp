#include "crossactivitycrossmultiuserchallenge.hpp"

#include "engineadapter.h"
#include "internalcomm.h"

#include "servercommon/string/crossstr.h"

#include "servercommon/serverconfig/crosscommonconfig.hpp"

#include "crossserver.h"
#include "crossserver/crossuser/crossuser.h"
#include "crossserver/crosslog.hpp"

#include "servercommon/noticenum.h"
#include "servercommon/errornum.h"

#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/internalprotocal/ggprotocal.h"

#include "servercommon/serverconfig/cross3v3config.hpp"

union LongLongMultiuserChallengeMatchingMapKey
{
	struct
	{
		int inc_num;
		int user_count : 4;
		int score : 28;
	};

	long long val;
};

static long long MultiuserChallengeSideInfoToLongLong(int inc_num, int score, char user_count)
{
	LongLongMultiuserChallengeMatchingMapKey ll;

	ll.inc_num = inc_num;
	ll.score = score;
	ll.user_count = user_count;

	return ll.val;
}

void CrossMultiuserChallengeSideUserList::CalcAverageScore()
{
	average_score = 0;
	for (int i = 0; i < user_count && i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT; ++i) average_score += user_list[i].challenge_score;
	average_score /= (user_count > 0 ? user_count : 1);

	const Cross3V3GradeScoreConfig *grade_cfg = Cross3v3Config::Instance().GetDivisionConfigWithScore(average_score);
	if (NULL != grade_cfg)
	{
		average_division = grade_cfg->grade;
	}
}

CrossActivityCrossMultiuserChallenge::CrossActivityCrossMultiuserChallenge(CrossActivityManager *cross_activity_manager)
	: CrossActivity(cross_activity_manager, CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE), m_inc_num(0), m_last_matching_time(0), m_last_sync_data_time(0), m_open_cfg_idx(0)
{
	m_match_register.SetCrossMultiuserChallengeActivity(this);
}

CrossActivityCrossMultiuserChallenge::~CrossActivityCrossMultiuserChallenge()
{

}

void CrossActivityCrossMultiuserChallenge::Init(const CrossActivityData &data)
{
	CrossActivity::Init(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data.data_list));

	ActivityDataParam* pTemp = (ActivityDataParam*)data.data_list;
	m_data_param = *pTemp;

	m_status_end_time = this->CalcActivityNextOpenTime();

	if (m_data_param.activity_begin_day_index == 0 && m_status_end_time != 0)
	{
		const tm* tm_now = EngineAdapter::Instance().LocalTime();
		int day_count = 0;
		if (tm_now->tm_mday >= 15)
		{
			day_count = tm_now->tm_mday - 15;
		}
		else
		{
			day_count = tm_now->tm_mday - 1;
		}
		m_data_param.activity_begin_day_index = GetZeroTime(EngineAdapter::Instance().Time()) / SECOND_PER_DAY - day_count;
	}
	m_is_init_finish = true;
}

void CrossActivityCrossMultiuserChallenge::GetInitParam(CrossActivityData *data)
{
	CrossActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data->data_list));

	ActivityDataParam* pTemp = (ActivityDataParam*)data->data_list;
	*pTemp = m_data_param;
}

void CrossActivityCrossMultiuserChallenge::Update(unsigned long interval, time_t now_second)
{
	if (ACTIVITY_STATUS_OPEN == m_activity_status)
	{
		// 匹配规则
		if (now_second >= m_last_matching_time + CROSS_MULTIUSER_CHALLENGE_MATCHING_INTERVAL)
		{
			this->TeammateMatching(false);		// 先匹配2人队伍的，在匹配单个人的
			this->TeammateMatching(true);
			this->OpponentMatching();

			m_last_matching_time = static_cast<unsigned int>(now_second);
		}

		// 同步积分
		static const int SYNC_DATA_INTERVAL_S = 60;
		if (now_second >= m_last_sync_data_time + SYNC_DATA_INTERVAL_S)
		{
			this->SyncDataToGameworld();
			m_last_sync_data_time = static_cast<unsigned int>(now_second);
		}
	}

	m_match_register.Update(interval, now_second);

	CrossActivity::Update(interval, now_second);
}

void CrossActivityCrossMultiuserChallenge::OnRegisterGameServer(NetID global_netid)
{
	CrossActivity::OnRegisterGameServer(global_netid);

	crossgameprotocal::CrossGameWorldCommonDataChange cg_msg;
	cg_msg.change_type = crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_3V3;
	cg_msg.sub_type = crossgameprotocal::WORLD_COMMONDATA_SUB_TYPE_INIT;
	//if (m_data_param.season_count <= 0)
	//{
	//	m_data_param.season_count = 1;
	//	m_data_param.day_index = 0;
	//}
	cg_msg.param_2 = m_data_param.activity_begin_day_index;
	//cg_msg.param_3 = m_data_param.day_index;
	//CrossServer::GetInstCrossServer()->SendToAllGameWorld((char*)&cg_msg, sizeof(cg_msg));
	CrossServer::GetInstCrossServer()->SendToGameWorld(global_netid, (char*)&cg_msg, sizeof(cg_msg));
}

void CrossActivityCrossMultiuserChallenge::OnGSDisconnect(NetID global_netid, const UniqueServerID &unique_server_id)
{
	CrossActivity::OnGSDisconnect(global_netid, unique_server_id);

	// 原服断开连接，清除匹配信息
	this->ClearMatchInfoByUniqueServerID(unique_server_id);
}

//static int RankSortCmp(const void *v1, const void *v2)
//{
//	CrossMultiuserChallengeUserRankItem *item1 = *((CrossMultiuserChallengeUserRankItem**)v1);
//	CrossMultiuserChallengeUserRankItem *item2 = *((CrossMultiuserChallengeUserRankItem**)v2);
//
//	if (NULL == item1 || NULL == item2) return 0;
//
//	if (item2->rank_value == item1->rank_value)
//	{
//		if (item2->match_total_count == item1->match_total_count)
//		{
//			return item2->win_percent > item1->win_percent ? 1 : -1;
//		}
//		else
//		{
//			return item2->match_total_count > item1->match_total_count ? 1 : -1;
//		}
//	}
//
//	return item2->rank_value > item1->rank_value ? 1 : -1;
//}

void CrossActivityCrossMultiuserChallenge::OnDayChange()
{
	m_data_param.day_index++;
// 	const Cross3v3OtherConfig & other_cfg = Cross3v3Config::Instance().GetOtherCfg();
// 	if (m_data_param.day_index >= other_cfg.season_days)

	// 每月1号，15号赛季变更
	const tm *tm_now = EngineAdapter::Instance().LocalTime();
	if (NULL != tm_now)
	{
		if (1 == tm_now->tm_mday || 15 == tm_now->tm_mday)
		{
			this->OnSeasonChange();
		}

		// 处理关服的时候跨赛季的情况
		else if (m_data_param.mon_day_index > 0 &&
			abs(tm_now->tm_mday - m_data_param.mon_day_index) > 1)
		{
			bool is_season_change = false;
			if (1 < tm_now->tm_mday  && tm_now->tm_mday < 15 && m_data_param.mon_day_index > 15)
			{
				is_season_change = true;
			}
			else if (15 < tm_now->tm_mday  && m_data_param.mon_day_index < 15)
			{
				is_season_change = true;
			}

			if (is_season_change)
			{
				this->OnSeasonChange();
			}
		}

		m_data_param.mon_day_index = tm_now->tm_mday;
	}
}

void CrossActivityCrossMultiuserChallenge::OnSeasonChange()
{
	if (!Cross3v3Config::Instance().GetOtherCfg().is_open)
	{
		return;
	}

	// 每赛季结算积分排名奖励(排行前100)
	long long uuid_list[CROSS_MULTIUSER_CHALLENGE_USER_RANK_MAX] = { 0 };
	int top_user_count = CrossServer::GetInstCrossServer()->GetCrossPersonRank()->GetRankTopUser(CROSS_PERSON_RANK_TYPE_3V3_SCORE, CROSS_ACTIVITY_1V1_USER_RANK_MAX, uuid_list);
	for (int index = 0; index < top_user_count && index < CROSS_MULTIUSER_CHALLENGE_USER_RANK_MAX; ++index)
	{
		int score = static_cast<int>(CrossServer::GetInstCrossServer()->GetCrossPersonRank()->GetRankValue(CROSS_PERSON_RANK_TYPE_3V3_SCORE, uuid_list[index]));
		
		// 大于配置里的等级才上榜
		if (score < Cross3v3Config::Instance().GetOtherCfg().rank_score_limit)
		{
			continue;
		}

		crossgameprotocal::CrossGameMultiuserChallengeScoreRankReward cgcsrr;
		cgcsrr.uid = static_cast<int>(uuid_list[index] & 0xffffffff);
		cgcsrr.rank_pos = index + 1;  // 注意！！ （加1）
		cgcsrr.socre = score;

		CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(uuid_list[index]), (const char*)&cgcsrr, sizeof(cgcsrr));
	}

	CrossServer::GetInstCrossServer()->GetCrossPersonRank()->OnSeasonChange(CROSS_RANK_TOP_TYPE_3V3);

	// 清空3v3赛季排行榜
	CrossServer::GetInstCrossServer()->GetCrossPersonRank()->ClearRank(CROSS_PERSON_RANK_TYPE_3V3_SCORE);

	{
		// 新赛季开始
		//++m_data_param.season_count;
		m_data_param.day_index = 0;
		m_data_param.activity_begin_day_index = GetZeroTime(EngineAdapter::Instance().Time()) / SECOND_PER_DAY;
		crossgameprotocal::CrossGameWorldCommonDataChange cg_msg;
		cg_msg.change_type = crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_3V3;
		cg_msg.sub_type = crossgameprotocal::WORLD_COMMONDATA_SUB_TYPE_SEANSON;
		cg_msg.param_2 = m_data_param.activity_begin_day_index; // m_data_param.season_count;
		cg_msg.param_3 = 0; // m_data_param.day_index;
		CrossServer::GetInstCrossServer()->SendToAllGameWorld((char*)&cg_msg, sizeof(cg_msg));
	}
}

void CrossActivityCrossMultiuserChallenge::OnWeekChange()
{
	
}

time_t CrossActivityCrossMultiuserChallenge::CalcActivityNextOpenTime()
{
	if (!Cross3v3Config::Instance().GetOtherCfg().is_open)
	{
		return 0;
	}

	Cross3v3ActivityOpenTimeInfo cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];
	int cfg_count = Cross3v3Config::Instance().GetOpenActivityConfig(cfg_list);

	int min_interval = INT_MAX;
	int min_locate_idx = 0;
	for (int index = 0; index < cfg_count; ++index)
	{
		Cross3v3ActivityOpenTimeInfo &open_cfg = cfg_list[index];
		int interval = EngineAdapter::Instance().NextWeekInterval(open_cfg.activity_week_day,
			open_cfg.activity_ready_time / 100, open_cfg.activity_ready_time % 100, 0);

		if (interval < min_interval)
		{
			min_interval = interval;
			min_locate_idx = index;
		}
	}

	m_open_cfg_idx = min_locate_idx;

	return EngineAdapter::Instance().Time() + min_interval;
}

void CrossActivityCrossMultiuserChallenge::OnChangeToNextStatus()
{
	if (!Cross3v3Config::Instance().GetOtherCfg().is_open)
	{
		m_activity_status = ACTIVITY_STATUS_CLOSE;
		m_status_end_time = 0;
		return;
	}

	//int max_season = Cross3v3Config::Instance().GetMaxSeason();
	//if (max_season > 0 && m_data_param.season_count > max_season)
	//{
	//	m_activity_status = ACTIVITY_STATUS_CLOSE;
	//	m_status_end_time = 0;
	//	return;
	//}

	// 设置默认的等待时间跟开启时间
	int ready_dur_s = 60;
	int open_dur_s = 1200;

	Cross3v3ActivityOpenTimeInfo *open_cfg = Cross3v3Config::Instance().GetCrossActivityOpenTimeInfo(m_open_cfg_idx);
	if (NULL != open_cfg)
	{
		ready_dur_s = open_cfg->ready_dur_s;
		open_dur_s = open_cfg->open_dur_s;
	}

	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		int standby_time = ready_dur_s;
		if (standby_time > 0)
		{
			m_activity_status = ACTIVITY_STATUS_STANDY;
			m_status_end_time = EngineAdapter::Instance().Time() + ready_dur_s;
		}
		else
		{
			m_activity_status = ACTIVITY_STATUS_OPEN;
			m_status_end_time = EngineAdapter::Instance().Time() + open_dur_s;
		}
	}
	else if (ACTIVITY_STATUS_STANDY == m_activity_status)
	{
		m_activity_status = ACTIVITY_STATUS_OPEN;
		m_status_end_time = EngineAdapter::Instance().Time() + open_dur_s;
	}
	else
	{
		m_activity_status = ACTIVITY_STATUS_CLOSE;
		this->OnActivityClose();
		m_status_end_time = this->CalcActivityNextOpenTime();
	}
}

void CrossActivityCrossMultiuserChallenge::OnActivityOpen()
{
	this->ClearAllData();
	//int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_multiuser_challenge_open_notice);
	//if (length > 0)
	//{
	//	m_cross_activity_manager->GetCrossServer()->SystemMsgToAllGlobalUser(gamestring::GAMESTRING_BUF, length);
	//}
}

void CrossActivityCrossMultiuserChallenge::OnActivityClose()
{
	this->ClearAllData();
	this->SyncDataToGameworld();
}

void CrossActivityCrossMultiuserChallenge::ClearAllData()
{
	//int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_multiuser_challenge_close_notice);
	//if (length > 0)
	//{
	//	m_cross_activity_manager->GetCrossServer()->SystemMsgToAllGlobalUser(gamestring::GAMESTRING_BUF, length);
	//}

	for (UserMatchingMapIt it = m_teammate_matching_map.begin(); it != m_teammate_matching_map.end(); ++ it)
	{
		this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_CANCEL, &it->second);
	}
	m_teammate_matching_map.clear();

	for (UserMatchingMapIt it = m_opponent_matching_map.begin(); it != m_opponent_matching_map.end(); ++ it)
	{
		this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_CANCEL, &it->second);
	}
	m_opponent_matching_map.clear();

	m_last_matching_time = 0;

	m_user_skill_map.clear();
}

void CrossActivityCrossMultiuserChallenge::OnMatchingReq(NetID global_netid, crossgameprotocal::GameCrossCrossMuliuserChallengeMatchingReq *gccmcmr)
{
	if (gccmcmr->count <= 0 || gccmcmr->count > CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT) return;
	if (ACTIVITY_STATUS_CLOSE == m_activity_status) return;
	
	CrossMultiuserChallengeSideUserList side_user_list;
	for (int i = 0; i < gccmcmr->count && i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT; i++)
	{
		if (gccmcmr->user_info_list[i].uuid <= 0) return;

		side_user_list.user_list[i].unique_user_id = LongLongToUniqueUserID(gccmcmr->user_info_list[i].uuid);
		side_user_list.user_list[i].sex = gccmcmr->user_info_list[i].sex;
		side_user_list.user_list[i].prof = gccmcmr->user_info_list[i].prof;
		side_user_list.user_list[i].camp = gccmcmr->user_info_list[i].camp;
		side_user_list.user_list[i].level = gccmcmr->user_info_list[i].level;
		side_user_list.user_list[i].capability = gccmcmr->user_info_list[i].capability;
		side_user_list.user_list[i].challenge_score = gccmcmr->user_info_list[i].challenge_score;
		side_user_list.user_list[i].challenge_exp = gccmcmr->user_info_list[i].challenge_exp;
		side_user_list.user_list[i].win_percent = gccmcmr->user_info_list[i].win_percent;
		side_user_list.user_list[i].challenge_level = gccmcmr->user_info_list[i].challenge_level;
		side_user_list.user_list[i].match_total_count = gccmcmr->user_info_list[i].match_total_count;
		side_user_list.user_list[i].match_win_count = gccmcmr->user_info_list[i].match_win_count;
		side_user_list.user_list[i].dur_win_count= gccmcmr->user_info_list[i].dur_win_count;
		side_user_list.user_list[i].mvp_count = gccmcmr->user_info_list[i].mvp_count;
		F_STRNCPY(side_user_list.user_list[i].user_name, gccmcmr->user_info_list[i].user_name, sizeof(side_user_list.user_list[i].user_name));
	}
	side_user_list.user_count = gccmcmr->count;
	side_user_list.CalcAverageScore();
	side_user_list.CalcAverageCapability();

	for (int i = 0; i < side_user_list.user_count; ++ i)
	{
		// 如果玩家处于正在比赛状态
		if (NULL != m_match_register.GetMatch(side_user_list.user_list[i].unique_user_id))
		{
			// 通知gameworld该玩家是匹配取消状态
			crossgameprotocal::CrossGameMultiuserChallengeMatchingStateNotify scmcms;
			scmcms.matching_state = CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_CANCEL;
			scmcms.cross_uid = UidToInt(side_user_list.user_list[i].unique_user_id.user_id);
			scmcms.auto_start_cross = 0;
			scmcms.notice_match = 1;
			CrossServer::GetInstCrossServer()->SendToGameWorld(side_user_list.user_list[i].unique_user_id, (const char *)&scmcms, sizeof(scmcms));

			// 通知客户端该玩家是匹配取消状态
			static Protocol::SCCrossMultiuserChallengeMatchingState cmcms;
			cmcms.matching_state = CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_CANCEL;
			cmcms.user_count = 0;
			CrossServer::GetInstCrossServer()->SendToUser(side_user_list.user_list[i].unique_user_id, (const char*)&cmcms, sizeof(cmcms));

			// 通知gameworld本方其他人员取消匹配
			side_user_list.user_list[i].unique_user_id = INVALID_UNIQUE_USER_ID;
			this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_CANCEL, &side_user_list, NULL, 2);

			return;
		}

		// 如果查询到玩家正在匹配
		if (this->IsUserInMatching(side_user_list.user_list[i].unique_user_id))
		{
			// 通知gameworld本方人员全部取消匹配
			//CrossServer::GetInstCrossServer()->NoticeNum(global_netid, LongLongToUniqueUserID(gccmcmr->req_uuid).user_id, errornum::EN_MULTIUSER_CHALLENGE_MEMBER_IN_MATCHING);
			this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_CANCEL, &side_user_list, NULL, 3);
			return;
		}
	}

	long long map_key = MultiuserChallengeSideInfoToLongLong(++ m_inc_num, side_user_list.average_score, side_user_list.user_count);

	if (side_user_list.user_count < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT)
	{
//		for (UserMatchingMapIt it = m_teammate_matching_map.begin(); it != m_teammate_matching_map.end(); ++ it)
//		{
//			if (it->second.is_matched) continue;
//
//			if (it->second.user_count + side_user_list.user_count == CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT && 
//				(int)(fabs(static_cast<float>(it->second.average_score - side_user_list.average_score))) <= CROSS_MULTIUSER_CHALLENGE_MATCHING_DELTA_SCORE)
//			{
//				it->second.is_matched = true;
//
//				side_user_list.AddUserList(it->second);
//				side_user_list.start_match_time = EngineAdapter::Instance().Time();
//				m_opponent_matching_map[map_key] = side_user_list;
//
//				this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_OPPONENT, &side_user_list);
//
//				m_teammate_matching_map.erase(it);
//				return;
//			}
//		}

		side_user_list.start_match_time = EngineAdapter::Instance().Time();
		m_teammate_matching_map[map_key] = side_user_list;

		this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_TEAMMATE, &side_user_list);
	}
	else
	{
//		for (UserMatchingMapIt it = m_opponent_matching_map.begin(); it != m_opponent_matching_map.end(); ++ it)
//		{
//			if (it->second.is_matched) continue;
//
//			if ((int)(fabs(static_cast<float>(it->second.average_score - side_user_list.average_score))) <= CROSS_MULTIUSER_CHALLENGE_MATCHING_DELTA_SCORE || 
//				(int)(fabs(static_cast<float>(it->second.average_capability - side_user_list.average_capability))) <= CROSS_MULTIUSER_CHALLENGE_MATCHING_DELTA_CAPABILITY)
//			{
//				it->second.is_matched = true;
//
//				m_match_register.CreateMatch(&it->second, &side_user_list);
//
//				this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_FINISH, &it->second, &side_user_list);
//
//				m_opponent_matching_map.erase(it);
//				return;
//			}
//		}

		++ side_user_list.matching_count;
		side_user_list.start_match_time = EngineAdapter::Instance().Time();
		m_opponent_matching_map[map_key] = side_user_list;

		this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_OPPONENT, &side_user_list);
	}
}

void CrossActivityCrossMultiuserChallenge::OnMatchingCancel(NetID global_netid, UniqueUserID unique_user_id)
{
	if (NULL != m_match_register.GetMatch(unique_user_id)) return;

	for (UserMatchingMapIt it = m_teammate_matching_map.begin(); it != m_teammate_matching_map.end(); ++ it)
	{
		CrossMultiuserChallengeUser *cancel_user = it->second.GetUser(unique_user_id);
		if (NULL != cancel_user)
		{
			CrossMultiuserChallengeSideUserList cancel_user_list; cancel_user_list.AddUser(*cancel_user);
			it->second.RemoveUser(unique_user_id);

			// 其他队员保持匹配队友状态
			this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_TEAMMATE, &it->second);
			// 自己设置为取消匹配状态
			this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_CANCEL, &cancel_user_list);

			if (it->second.user_count <= 0) m_teammate_matching_map.erase(it);

			return;
		}
	}

	for (UserMatchingMapIt it = m_opponent_matching_map.begin(); it != m_opponent_matching_map.end(); ++ it)
	{
		CrossMultiuserChallengeUser *cancel_user = it->second.GetUser(unique_user_id);
		if (NULL != cancel_user)
		{
			CrossMultiuserChallengeSideUserList cancel_user_list; cancel_user_list.AddUser(*cancel_user);
			it->second.RemoveUser(unique_user_id);

			// 其他队员回到匹配队友状态
			this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_TEAMMATE, &it->second);
			// 自己设置为取消状态
			this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_CANCEL, &cancel_user_list);

			if (it->second.user_count > 0) m_teammate_matching_map[it->first] = it->second;
			m_opponent_matching_map.erase(it);

			return;
		}
	}

	// 前端要求取消状态，上面都没进去，说明跨服里是没有在匹配，前端本地数据有误，取消他的匹配状态
	crossgameprotocal::CrossGameMultiuserChallengeMatchingStateNotify scmcms;
	scmcms.matching_state = CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_CANCEL;
	scmcms.cross_uid = UidToInt(unique_user_id.user_id);
	scmcms.auto_start_cross = 0;
	scmcms.notice_match = 0;
	CrossServer::GetInstCrossServer()->SendToGameWorld(unique_user_id, (const char *)&scmcms, sizeof(scmcms));
}

void CrossActivityCrossMultiuserChallenge::OnGetMatchingState(NetID global_netid, UniqueUserID unique_user_id)
{
	if (ACTIVITY_STATUS_CLOSE == m_activity_status) return;

	// 如果查找到了玩家的匹配完成信息
	if (NULL != m_match_register.GetMatch(unique_user_id))
	{
		// 通知gameworld该玩家是匹配完成状态
		crossgameprotocal::CrossGameMultiuserChallengeMatchingStateNotify scmcms;
		scmcms.matching_state = CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_FINISH;
		scmcms.cross_uid = UidToInt(unique_user_id.user_id);
		scmcms.auto_start_cross = 1;
		scmcms.notice_match = 0;
		CrossServer::GetInstCrossServer()->SendToGameWorld(unique_user_id, (const char *)&scmcms, sizeof(scmcms));

		return;
	}

	// 如果查询到玩家正在匹配
	//if (this->IsUserInMatching(unique_user_id))
	//{
		// 通知gameworld本方人员全部取消匹配
	//	return;
	//}
}

bool CrossActivityCrossMultiuserChallenge::IsUserInMatching(const UniqueUserID &unique_user_id)
{
	for (UserMatchingMapIt it = m_teammate_matching_map.begin(); it != m_teammate_matching_map.end(); ++ it)
	{
		if (it->second.HasUser(unique_user_id)) return true;
	}

	for (UserMatchingMapIt it = m_opponent_matching_map.begin(); it != m_opponent_matching_map.end(); ++ it)
	{
		if (it->second.HasUser(unique_user_id)) return true;
	}

	return false;
}

void CrossActivityCrossMultiuserChallenge::CalcDivisionRange(std::map<long long, CrossMultiuserChallengeSideUserList> &side_user_map)
{
	const std::vector<Cross3V3PriorityConfig> &priority_vec_cfg = Cross3v3Config::Instance().GetPriorityConfig();
	time_t now_second = EngineAdapter::Instance().Time();

	for (UserMatchingMapIt it = side_user_map.begin(); it != side_user_map.end(); ++it)
	{
		if (it->second.is_matched) continue;

		int match_time_s = int(now_second - it->second.start_match_time);
		it->second.max_division_range = 0;
		for (std::vector<Cross3V3PriorityConfig>::const_reverse_iterator priority_it = priority_vec_cfg.rbegin(); priority_it != priority_vec_cfg.rend(); ++priority_it)
		{
			if (match_time_s >= priority_it->match_time_s) // 记住配置里配的是下限
			{
				it->second.max_division_range = priority_it->interval;
				break;
			}
		}
	}
}

void CrossActivityCrossMultiuserChallenge::TeammateMatching(bool is_matching_singel_user)
{
	// 遍历一遍匹配队友列表，生成每个队伍允许匹配的最大段位差距
	this->CalcDivisionRange(m_teammate_matching_map);

	if (is_matching_singel_user)
	{
		CrossMultiuserChallengeSideUserList side_user_list;
		long long matched_map_key_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT] = {0};
		const std::vector<Cross3V3PriorityConfig> &priority_vec_cfg = Cross3v3Config::Instance().GetPriorityConfig();
		time_t now_second = EngineAdapter::Instance().Time();
		
		for (UserMatchingMapIt it = m_teammate_matching_map.begin(); it != m_teammate_matching_map.end(); ++ it)
		{
			if (it->second.is_matched) continue;

			if (it->second.user_count == 1)
			{
				if (side_user_list.user_count > 0) // 临时列表里有玩家才开始做段位检测
				{
					int division_diff = (int)(fabs(it->second.average_division - side_user_list.average_division));
					if (division_diff > it->second.max_division_range || division_diff > side_user_list.max_division_range)
					{
						continue;
					}
				}

				if (0 <= side_user_list.user_count && side_user_list.user_count < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT)
				{
					matched_map_key_list[side_user_list.user_count] = it->first;
					side_user_list.AddUserList(it->second);
					{ // 每次新加玩家，都要重算一次允许最大差距，因为这个是临时生成的玩家列表，前面没有算

						int match_time_s = int(now_second - side_user_list.start_match_time);
						side_user_list.max_division_range = 0;
						for (std::vector<Cross3V3PriorityConfig>::const_reverse_iterator priority_it = priority_vec_cfg.rbegin(); priority_it != priority_vec_cfg.rend(); ++priority_it)
						{
							if (match_time_s >= priority_it->match_time_s) // 记住配置里配的是下限
							{
								side_user_list.max_division_range = priority_it->interval;
								break;
							}
						}
					}

					if (CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT == side_user_list.user_count)
					{
						for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT; i++)
						{
							UserMatchingMapIt matched_map_it = m_teammate_matching_map.find(matched_map_key_list[i]);
							if (matched_map_it != m_teammate_matching_map.end())
							{
								matched_map_it->second.is_matched = true;
							}
						}

						long long map_key = MultiuserChallengeSideInfoToLongLong(++ m_inc_num, side_user_list.average_score, side_user_list.user_count);
						side_user_list.start_match_time = EngineAdapter::Instance().Time();
						m_opponent_matching_map[map_key] = side_user_list;

						this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_OPPONENT, &side_user_list);

						side_user_list.Reset();
					}
				}
			}
		}
	}
	else
	{
		for (UserMatchingMapIt it = m_teammate_matching_map.begin(); it != m_teammate_matching_map.end(); ++ it)
		{
			if (it->second.is_matched) continue;

			UserMatchingMapIt other_it = it; ++ other_it;
			for (; other_it != m_teammate_matching_map.end(); ++ other_it)
			{
				if (other_it->second.is_matched) continue;

				int division_diff = (int)(fabs(it->second.average_division - other_it->second.average_division));
				if (division_diff > it->second.max_division_range || division_diff > other_it->second.max_division_range)
				{
					continue;
				}

				if (it->second.user_count + other_it->second.user_count == CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT)
				{
					it->second.is_matched = true; other_it->second.is_matched = true;

					CrossMultiuserChallengeSideUserList side_user_list;
					side_user_list.AddUserList(it->second); side_user_list.AddUserList(other_it->second);

					long long map_key = MultiuserChallengeSideInfoToLongLong(++ m_inc_num, side_user_list.average_score, side_user_list.user_count);
					side_user_list.start_match_time = EngineAdapter::Instance().Time();
					m_opponent_matching_map[map_key] = side_user_list;

					this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_OPPONENT, &side_user_list);

					break;
				}
			}
		}
	}

	for (UserMatchingMapIt it = m_teammate_matching_map.begin(); it != m_teammate_matching_map.end();)
	{
		if (it->second.is_matched)
		{
			m_teammate_matching_map.erase(it++);
		}
		else
		{
			++ it;
		}
	}
}

void CrossActivityCrossMultiuserChallenge::OpponentMatching()
{
	// 编译一遍匹配对手列表，生成每个队伍允许匹配的最大段位差距
	this->CalcDivisionRange(m_opponent_matching_map);

	// 双重循环做匹配动作，要两个队伍都在各自允许的段位差距才能匹配得到
	for (UserMatchingMapIt it = m_opponent_matching_map.begin(); it != m_opponent_matching_map.end(); ++ it)
	{
		if (it->second.is_matched) continue;
		
		UserMatchingMapIt other_it = it; ++ other_it;
		for (; other_it != m_opponent_matching_map.end(); ++ other_it)
		{
			if (other_it->second.is_matched) continue;

			int division_diff = (int)(fabs(it->second.average_division - other_it->second.average_division));
			if (division_diff > it->second.max_division_range || division_diff > other_it->second.max_division_range)
			{
				continue;
			}

			it->second.is_matched = true; other_it->second.is_matched = true;

			m_match_register.CreateMatch(&it->second, &other_it->second);

			this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_FINISH, &it->second, &other_it->second);

			break;
		}

		++ it->second.matching_count;
	}

	// 删除已匹配到的队伍列表
	for (UserMatchingMapIt it = m_opponent_matching_map.begin(); it != m_opponent_matching_map.end();)
	{
		if (it->second.is_matched)
		{
			m_opponent_matching_map.erase(it++);
		}
		else
		{
			++ it;
		}
	}
}

void CrossActivityCrossMultiuserChallenge::SyncDataToGameworld()
{
	int rank_last_score = 0;
	long long uuid_list[CROSS_ACTIVITY_1V1_USER_RANK_MAX] = { 0 };
	int top_user_count = CrossServer::GetInstCrossServer()->GetCrossPersonRank()->GetRankTopUser(CROSS_PERSON_RANK_TYPE_3V3_SCORE, CROSS_ACTIVITY_1V1_USER_RANK_MAX, uuid_list);
	for (int i = 0; i < top_user_count; i++)
	{
		int score = static_cast<int>(CrossServer::GetInstCrossServer()->GetCrossPersonRank()->GetRankValue(CROSS_PERSON_RANK_TYPE_3V3_SCORE, uuid_list[i]));
		if (score < Cross3v3Config::Instance().GetOtherCfg().rank_score_limit)
		{
			continue;
		}

		if (rank_last_score == 0 || score < rank_last_score)
		{
			rank_last_score = score;
		}
	}

	crossgameprotocal::CrossGameWorldCommonDataChange cg_msg;
	cg_msg.change_type = crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_3V3;
	cg_msg.sub_type = crossgameprotocal::WORLD_COMMONDATA_SUB_TYPE_RANK_MIN_SCORE;
	cg_msg.param_2 = rank_last_score;
	cg_msg.param_3 = 0; // m_data_param.season_count;
	CrossServer::GetInstCrossServer()->SendToAllGameWorld((char*)&cg_msg, sizeof(cg_msg));
}

void CrossActivityCrossMultiuserChallenge::UpdateUserRank(int rank_type, const CrossMultiuserChallengeUserRankItem &rank_item)
{
	if (rank_type <= CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_INVALID || rank_type >= CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_MAX) return;
	if (m_user_rank_list[rank_type].count < 0 || m_user_rank_list[rank_type].count > CROSS_MULTIUSER_CHALLENGE_USER_RANK_MAX) return;

	UniqueUserID unique_user_id(rank_item.plat_type, IntToUid(rank_item.role_id));

	CrossMultiuserChallengeUserRankItem *user_rank_item = m_user_rank_list[rank_type].GetUserRankItem(unique_user_id);
	if (NULL != user_rank_item)
	{
		*user_rank_item = rank_item;
		return;
	}

	if (m_user_rank_list[rank_type].count < CROSS_MULTIUSER_CHALLENGE_USER_RANK_MAX)
	{
		int index = m_user_rank_list[rank_type].count;
		m_user_rank_list[rank_type].user_rank_list[index] = rank_item;
		m_user_rank_list[rank_type].user_rank_map[unique_user_id] = index;

		++ m_user_rank_list[rank_type].count;
	}
	else
	{
		int min_rank_slot = 0;
		for (int i = 0; i < m_user_rank_list[rank_type].count; i++)
		{
			if (m_user_rank_list[rank_type].user_rank_list[i].rank_value < m_user_rank_list[rank_type].user_rank_list[min_rank_slot].rank_value)
			{
				min_rank_slot = i;
			}
		}

		if (rank_item.rank_value > m_user_rank_list[rank_type].user_rank_list[min_rank_slot].rank_value)
		{
			UniqueUserID min_rank_unique_user_id(m_user_rank_list[rank_type].user_rank_list[min_rank_slot].plat_type, IntToUid(m_user_rank_list[rank_type].user_rank_list[min_rank_slot].role_id));

			m_user_rank_list[rank_type].user_rank_map.erase(min_rank_unique_user_id);
			m_user_rank_list[rank_type].user_rank_list[min_rank_slot] = rank_item;
			m_user_rank_list[rank_type].user_rank_map[unique_user_id] = min_rank_slot;
		}
	}
}

void CrossActivityCrossMultiuserChallenge::SendMatchingStateToUser(int matching_state, CrossMultiuserChallengeSideUserList *side_user_list, CrossMultiuserChallengeSideUserList *other_side_user_list /* = NULL */, short notice_match /* = 0 */)
{
	static Protocol::SCCrossMultiuserChallengeMatchingState cmcms;
	cmcms.matching_state = matching_state;
	cmcms.user_count = 0;

	if (NULL != side_user_list)
	{
		for (int i = 0; i < side_user_list->user_count && cmcms.user_count < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; i++)
		{
			cmcms.user_list[cmcms.user_count].plat_type = side_user_list->user_list[i].unique_user_id.plat_type;
			cmcms.user_list[cmcms.user_count].server_id = side_user_list->user_list[i].unique_user_id.user_id.db_index;
			cmcms.user_list[cmcms.user_count].role_id = UidToInt(side_user_list->user_list[i].unique_user_id.user_id);
			cmcms.user_list[cmcms.user_count].mvp_count =side_user_list->user_list[i].mvp_count;
			cmcms.user_list[cmcms.user_count].prof = side_user_list->user_list[i].prof;
			cmcms.user_list[cmcms.user_count].camp = side_user_list->user_list[i].camp;
			cmcms.user_list[cmcms.user_count].level = side_user_list->user_list[i].level;
			cmcms.user_list[cmcms.user_count].challenge_score = side_user_list->user_list[i].challenge_score;
			cmcms.user_list[cmcms.user_count].win_percent = side_user_list->user_list[i].win_percent;
			cmcms.user_list[cmcms.user_count].capability = side_user_list->user_list[i].capability;
			F_STRNCPY(cmcms.user_list[cmcms.user_count].role_name, side_user_list->user_list[i].user_name, sizeof(cmcms.user_list[cmcms.user_count].role_name));

			++ cmcms.user_count;
		}
	}

	if (NULL != other_side_user_list)
	{
		for (int i = 0; i < other_side_user_list->user_count && cmcms.user_count < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; i++)
		{
			cmcms.user_list[cmcms.user_count].plat_type = other_side_user_list->user_list[i].unique_user_id.plat_type;
			cmcms.user_list[cmcms.user_count].server_id = other_side_user_list->user_list[i].unique_user_id.user_id.db_index;
			cmcms.user_list[cmcms.user_count].role_id = UidToInt(other_side_user_list->user_list[i].unique_user_id.user_id);
			cmcms.user_list[cmcms.user_count].mvp_count = other_side_user_list->user_list[i].mvp_count;
			cmcms.user_list[cmcms.user_count].prof = other_side_user_list->user_list[i].prof;
			cmcms.user_list[cmcms.user_count].camp = other_side_user_list->user_list[i].camp;
			cmcms.user_list[cmcms.user_count].level = other_side_user_list->user_list[i].level;
			cmcms.user_list[cmcms.user_count].challenge_score = other_side_user_list->user_list[i].challenge_score;
			cmcms.user_list[cmcms.user_count].win_percent = other_side_user_list->user_list[i].win_percent;
			cmcms.user_list[cmcms.user_count].capability = other_side_user_list->user_list[i].capability;
			F_STRNCPY(cmcms.user_list[cmcms.user_count].role_name, other_side_user_list->user_list[i].user_name, sizeof(cmcms.user_list[cmcms.user_count].role_name));

			++ cmcms.user_count;
		}
	}

	crossgameprotocal::CrossGameMultiuserChallengeMatchingStateNotify scmcms;
	scmcms.matching_state = matching_state;
	scmcms.auto_start_cross = 0;
	scmcms.notice_match = notice_match;

	for (int i = 0; i < cmcms.user_count; i++)
	{
		UniqueUserID unique_user_id = UniqueUserID(cmcms.user_list[i].plat_type, IntToUid(cmcms.user_list[i].role_id));

		scmcms.cross_uid = cmcms.user_list[i].role_id;
		CrossServer::GetInstCrossServer()->SendToUser(unique_user_id, (const char*)&cmcms, sizeof(cmcms));
		CrossServer::GetInstCrossServer()->SendToGameWorld(unique_user_id, (const char *)&scmcms, sizeof(scmcms));
	}
}

void CrossActivityCrossMultiuserChallenge::OnMatchStart(CrossMultiuserChallengeMatchInfo *match_info)
{
	// 匹配完成会有通知 这里并不需要额外通知
}

void CrossActivityCrossMultiuserChallenge::OnMatchTimeoutFinish(CrossMultiuserChallengeMatchInfo *match_info)
{
	m_match_register.RemoveMatch(match_info->key);
	match_info = NULL;
}

void CrossActivityCrossMultiuserChallenge::OnMatchResult(crossgameprotocal::GameCrossCrossMultiuserChallengeMatchResult *gccmcmr)
{
	if (-1 != gccmcmr->win_side && (gccmcmr->win_side < 0 || gccmcmr->win_side >= CROSS_MULTIUSER_CHALLENGE_SIDE_MAX)) return;		// -1 表示平局

	CrossMultiuserChallengeMatchInfo *match_info = m_match_register.GetMatch(gccmcmr->match_key);
	if (NULL == match_info) return;

	match_info->win_side = gccmcmr->win_side;

	{
		static crossgameprotocal::CrossGameUpdateCrossRecord cgucr; 
		cgucr.update_cross_record_type = crossgameprotocal::CrossGameUpdateCrossRecord::UPDATE_CROSS_RECORD_CROSS_MULTIUSER_CHALLENGE;

		for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; i++)
		{
			UniqueUserID unique_user_id = LongLongToUniqueUserID(gccmcmr->sync_user_info_list[i].uuid);

			{
				cgucr.uid = UidToInt(unique_user_id.user_id);

				UNSTD_STATIC_CHECK(sizeof(cgucr.record_param_list) / sizeof(cgucr.record_param_list[0]) >= 5);

				cgucr.record_param_list[0] = gccmcmr->sync_user_info_list[i].add_challenge_score;
				cgucr.record_param_list[1] = (i / CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT == gccmcmr->win_side ? 1 : 0);
				cgucr.record_param_list[2] = (gccmcmr->sync_user_info_list[i].is_mvp ? 1 : 0);
				cgucr.record_param_list[3] = gccmcmr->sync_user_info_list[i].add_honor;
				cgucr.record_param_list[4] = static_cast<int>(match_info->capability_list[i]);
				cgucr.record_param_list[5] = static_cast<int>(gccmcmr->sync_user_info_list[i].add_gongxun);

				CrossServer::GetInstCrossServer()->SendToGameWorld(unique_user_id, (const char *)&cgucr, sizeof(cgucr));
			}

			{
				CrossMultiuserChallengeUserRankItem rank_item;
				rank_item.plat_type = unique_user_id.plat_type;
				rank_item.server_id = unique_user_id.user_id.db_index;
				rank_item.role_id = cgucr.uid;
				rank_item.level = gccmcmr->sync_user_info_list[i].level;
				rank_item.prof = gccmcmr->sync_user_info_list[i].prof;
				rank_item.sex = gccmcmr->sync_user_info_list[i].sex;
				rank_item.jingjie = gccmcmr->sync_user_info_list[i].jingjie;
				rank_item.match_total_count = gccmcmr->sync_user_info_list[i].match_total_count;
				rank_item.dur_win_count = gccmcmr->sync_user_info_list[i].dur_win_count;
				rank_item.win_percent = 0;
				rank_item.capability = static_cast<int>(match_info->capability_list[i]);
				if (rank_item.match_total_count > 0)
				{
					rank_item.win_percent = gccmcmr->sync_user_info_list[i].match_win_count * 100 / rank_item.match_total_count;
				}
				F_STRNCPY(rank_item.name, gccmcmr->sync_user_info_list[i].user_name, sizeof(rank_item.name));
				
				for (int rank_type = CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_INVALID + 1; rank_type < CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_MAX; rank_type++)
				{
					switch (rank_type)
					{
					case CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_SCORE:
						{
							rank_item.rank_value = gccmcmr->sync_user_info_list[i].challenge_score;
						}
						break;

					case CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_MVP:
						{
							rank_item.rank_value = gccmcmr->sync_user_info_list[i].mvp_count;
						}
						break;
					}

					this->UpdateUserRank(rank_type, rank_item);
				}
			}
		}
	}

	{
		m_match_register.RemoveMatch(gccmcmr->match_key);
		match_info = NULL;
	}
}

void CrossActivityCrossMultiuserChallenge::OnGlobalSyncRankList(crossgameprotocal::GlobalCrossMultiuserChallengeRankList *gcmcrl)
{
	return; // 这个协议已经废弃，没有地方发送，所以这里返回，测试逻辑没错后可以删除下面代码

	for (int i = 0; i < gcmcrl->count && crossgameprotocal::MAX_MULTIUSER_CHALLENGE_RANK_COUNT; i++)
	{
		{
			CrossMultiuserChallengeUserRankItem rank_item;
			rank_item.plat_type = gcmcrl->rank_list[i].plat_type;
			rank_item.role_id = gcmcrl->rank_list[i].role_id;
			F_STRNCPY(rank_item.name, gcmcrl->rank_list[i].name, sizeof(rank_item.name));
			rank_item.level = gcmcrl->rank_list[i].level;
			rank_item.sex = gcmcrl->rank_list[i].sex;
			rank_item.prof = gcmcrl->rank_list[i].prof;
			rank_item.jingjie = gcmcrl->rank_list[i].jingjie;
			rank_item.match_total_count = gcmcrl->rank_list[i].match_total_count;
			rank_item.win_percent = gcmcrl->rank_list[i].win_percent;
			rank_item.capability = gcmcrl->rank_list[i].capability;
			rank_item.rank_value = 0;

			for (int rank_type = CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_INVALID + 1; rank_type < CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_MAX; rank_type++)
			{
				switch (rank_type)
				{
				case CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_SCORE:
					{
						rank_item.rank_value = gcmcrl->rank_list[i].challenge_score;
					}
					break;

				case CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_MVP:
					{
						rank_item.rank_value = gcmcrl->rank_list[i].mvp_count;
					}
					break;
				}

				this->UpdateUserRank(rank_type, rank_item);
			}
		}
	}
}

void CrossActivityCrossMultiuserChallenge::OnGetRankList(NetID global_netid, const UniqueUserID &unique_user_id, int rank_type)
{
	if (rank_type <= CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_INVALID || rank_type >= CROSS_MULTIUSER_CHALLENGE_USER_RANK_TYPE_MAX) return;

	static Protocol::SCMultiuserChallengeRankList mcrl;
	mcrl.rank_type = rank_type;
	mcrl.count = m_user_rank_list[rank_type].count;
	memcpy(mcrl.rank_list, m_user_rank_list[rank_type].user_rank_list, sizeof(mcrl.rank_list));

	int sendlen = sizeof(mcrl) - sizeof(mcrl.rank_list[0]) * (CROSS_MULTIUSER_CHALLENGE_USER_RANK_MAX - mcrl.count);
	CrossServer::GetInstCrossServer()->SendToUser(global_netid, unique_user_id.user_id, (const char*)&mcrl, sendlen);
}

void CrossActivityCrossMultiuserChallenge::OnCheckUserHasMatch(NetID global_netid, const UniqueUserID &unique_user_id)
{
	static Protocol::SCMultiuserChallengeHasMatchNotice mchmn;

	if (NULL != m_match_register.GetMatch(unique_user_id))
	{
		mchmn.has_match = 1;
	}
	else
	{
		mchmn.has_match = 0;
	}

	CrossServer::GetInstCrossServer()->SendToUser(global_netid, unique_user_id.user_id, (const char*)&mchmn, sizeof(mchmn));
}

bool CrossActivityCrossMultiuserChallenge::CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id)
{
	CrossMultiuserChallengeMatchInfo *match_info = m_match_register.GetMatch(unique_user_id);
	return (NULL != match_info && CROSS_MULTIUSER_CHALLENGE_MATCH_STATUS_FINISH != match_info->status);
}

void CrossActivityCrossMultiuserChallenge::OnCrossUserLogin(CrossUser *cross_user)
{
	CrossMultiuserChallengeMatchInfo *match_info = m_match_register.GetMatch(cross_user->GetOriginalUniqueUserID());
	if (NULL != match_info)
	{
		int side = match_info->GetSide(cross_user->GetOriginalUniqueUserID());
		if (side >= 0 && side < CROSS_MULTIUSER_CHALLENGE_SIDE_MAX)
		{
			crossgameprotocal::CrossGameMultiuserChallengeGotoScene cmcgs;
			cmcgs.cross_uid = cross_user->GetCrossUid();
			cmcgs.fb_key = match_info->key;
			cmcgs.side = side;
			CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char *)&cmcgs, sizeof(cmcgs));
		}
	}
}

void CrossActivityCrossMultiuserChallenge::ClearMatchInfoByUniqueServerID(const UniqueServerID &unique_server_id)
{
	if (ACTIVITY_STATUS_OPEN != m_activity_status)
	{
		return;
	}

	// 清除正在匹配队友的
	for (UserMatchingMapIt it = m_teammate_matching_map.begin(); it != m_teammate_matching_map.end();)
	{
		if (it->second.RemoveUserByUniqueServerID(unique_server_id))
		{
			// 其他队员保持匹配队友状态
			this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_TEAMMATE, &it->second);

			if (it->second.user_count <= 0) m_teammate_matching_map.erase(it++);
			else ++it;
		}
		else
		{
			++it;
		}
	}

	// 清除正在匹配对手的
	for (UserMatchingMapIt it = m_opponent_matching_map.begin(); it != m_opponent_matching_map.end();)
	{
		if (it->second.RemoveUserByUniqueServerID(unique_server_id))
		{
			// 其他队员回到匹配队友状态
			this->SendMatchingStateToUser(CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_TEAMMATE, &it->second);

			if (it->second.user_count > 0) m_teammate_matching_map[it->first] = it->second;
			m_opponent_matching_map.erase(it++);
		}
		else
		{
			++it;
		}
	}

	// 队友或者对手可能是其他服务器的角色，取消后也要进行通知！！


	// log
	crosslog::g_log_cross_activity.printf(LL_WARNING, "Clear 3v3 match on gameserver[serverID:%d][plat_type:%d] disconnect..",
		unique_server_id.server_id, unique_server_id.plat_type);
}


