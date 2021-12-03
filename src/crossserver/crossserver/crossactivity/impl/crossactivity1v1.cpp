#include "crossactivity1v1.hpp"

#include "engineadapter.h"
#include "internalcomm.h"

#include "servercommon/serverconfig/crosscommonconfig.hpp"
#include "servercommon/serverconfig/cross1v1commonconfig.hpp"

#include "crossserver.h"
#include "crossserver/crossuser/crossuser.h"
#include "crossserver/crosslog.hpp"

#include "servercommon/noticenum.h"
#include "servercommon/errornum.h"

#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/string/crossstr.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "crossuser/crossusermanager.hpp"

#include <algorithm>


CrossActivity1V1::CrossActivity1V1(CrossActivityManager *cross_activity_manager)
	: CrossActivity(cross_activity_manager, CROSS_ACTIVITY_TYPE_CROSS_1V1), m_next_match_time(0), m_next_rank_time(0), m_next_sync_min_score_time(0), m_fight_seq(0), m_open_cfg_idx(0)
{

}

CrossActivity1V1::~CrossActivity1V1()
{

}

void CrossActivity1V1::Init(const CrossActivityData &data)
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

void CrossActivity1V1::GetInitParam(CrossActivityData *data)
{
	CrossActivity::GetInitParam(data);

	UNSTD_STATIC_CHECK(sizeof(m_data_param) <= sizeof(data->data_list));

	ActivityDataParam* pTemp = (ActivityDataParam*)data->data_list;
	*pTemp = m_data_param;

}

void CrossActivity1V1::OnRegisterGameServer(NetID global_netid)
{
	CrossActivity::OnRegisterGameServer(global_netid);

	crossgameprotocal::CrossGameWorldCommonDataChange cgwcdc;
	cgwcdc.change_type = crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_1V1;
	cgwcdc.sub_type = crossgameprotocal::WORLD_COMMONDATA_SUB_TYPE_INIT;
	//if (m_data_param.season_count <= 0)
	//{
	//	m_data_param.season_count = 1;
	//}
	cgwcdc.param_2 = m_data_param.activity_begin_day_index;
	//CrossServer::GetInstCrossServer()->SendToAllGameWorld((const char *)&cgwcdc, sizeof(cgwcdc));
	CrossServer::GetInstCrossServer()->SendToGameWorld(global_netid, (char*)&cgwcdc, sizeof(cgwcdc));
}

void CrossActivity1V1::OnGSDisconnect(NetID global_netid, const UniqueServerID &unique_server_id)
{
	CrossActivity::OnGSDisconnect(global_netid, unique_server_id);

	// 原服断开连接，清除对应服的玩家匹配信息
	this->ClearMatchInfoByUniqueServerID(unique_server_id);
}

void CrossActivity1V1::Update(unsigned long interval, time_t now_second)
{
	CrossActivity::Update(interval, now_second);

	// 跨月前提前5s回传数据   这个逻辑有问题，会连续执行15次
	//if (m_this_month_end_time != 0 && now_second >= m_this_month_end_time)
	//{
	//	this->OnSendCross1v1MonthInfo();
	//	m_this_month_end_time = EngineAdapter::Instance().Time() + EngineAdapter::Instance().NextMouthInterval(1, 0, 0, 0) - 5;
	//}

	if (ACTIVITY_STATUS_OPEN != m_activity_status)
	{
		return;
	}

	if (0 != m_next_match_time && now_second >= m_next_match_time)
	{
		const Cross1V1OtherConfig &other_cfg = Cross1V1CommonConfig::Instance().GetOtherCfg();
		m_next_match_time = (unsigned int)now_second + other_cfg.match_time_s;

		this->DoMatch();
	}

	// 活动开启时，每隔1分钟同步一次最低积分
	if (now_second >= m_next_sync_min_score_time)
	{
		this->OnSendCross1v1MonthInfo();
		m_next_sync_min_score_time = now_second + SECOND_PER_MIN;
	}
}

void CrossActivity1V1::OnDayChange()
{
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

bool CrossActivity1V1::CheckCanStartCross(const UniqueUserID &unique_user_id, int merge_server_id)
{
	std::map<long long, CrossActivity1v1FightUnit>::iterator iter = m_match_map.find(UniqueUserIDToLongLong(unique_user_id));
	if (iter != m_match_map.end())
	{
		return true;
	}

	return false;
}

void CrossActivity1V1::OnCrossUserLogin(CrossUser *cross_user)
{
	long long uuid = UniqueUserIDToLongLong(cross_user->GetOriginalUniqueUserID());

	std::map<long long, CrossActivity1v1FightUnit>::iterator iter = m_match_map.find(uuid);
	if (iter != m_match_map.end())
	{
		crossgameprotocal::CrossGame1V1GotoScene cgs;
		cgs.uuid = UniqueUserIDToLongLong(cross_user->GetOriginalUniqueUserID());
		cgs.cross_uid = cross_user->GetCrossUserID();

		CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char*)&cgs, sizeof(cgs));
	}
	else
	{
		crosslog::g_log_cross_activity.printf(LL_INFO, "CrossActivity1V1: Plat_type:%d Role_id:%d frist enter scene can not find fight unit", 
			cross_user->GetOriginalUniqueUserID().plat_type, UidToInt(cross_user->GetOriginalUniqueUserID().user_id));
	}
}

void CrossActivity1V1::OnWeekChange()
{
	
}

void CrossActivity1V1::OnSeasonChange()
{
	if (!Cross1V1CommonConfig::Instance().GetOtherCfg().is_open)
	{
		return;
	}

	long long uuid_list[CROSS_ACTIVITY_1V1_USER_RANK_MAX] = { 0 };
	int top_user_count = CrossServer::GetInstCrossServer()->GetCrossPersonRank()->GetRankTopUser(CROSS_PERSON_RANK_TYPE_1V1_SCORE, CROSS_ACTIVITY_1V1_USER_RANK_MAX, uuid_list);
	for (int i = 0; i < top_user_count; ++i)
	{
		int score = static_cast<int>(CrossServer::GetInstCrossServer()->GetCrossPersonRank()->GetRankValue(CROSS_PERSON_RANK_TYPE_1V1_SCORE, uuid_list[i]));
		int grade = Cross1V1CommonConfig::Instance().GetGradeByScore(score);

		crosslog::g_log_cross_activity.printf(LL_INFO, "CrossActivity1V1 OnSeasonChange uuid:%lld score:%d grade:%d rank_pos:%d", uuid_list[i], score, grade, i);

		if (grade < Cross1V1CommonConfig::Instance().GetOtherCfg().rank_reward_min_grade)
		{
			continue;
		}

		crossgameprotocal::CrossGameCross1V1ScoreRankReward cgcsrr;
		cgcsrr.uid = UidToInt(LongLongToUniqueUserID(uuid_list[i]).user_id);
		cgcsrr.rank_pos = i;    // 注意！第一名的排行位置为0
		cgcsrr.score = score;

		CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(uuid_list[i]), (const char*)&cgcsrr, sizeof(cgcsrr));
	}

	CrossServer::GetInstCrossServer()->GetCrossPersonRank()->OnSeasonChange(CROSS_RANK_TOP_TYPE_1V1);

	CrossServer::GetInstCrossServer()->GetCrossPersonRank()->ClearRank(CROSS_PERSON_RANK_TYPE_1V1_SCORE);

	{
		// 新赛季开始
		// ++m_data_param.season_count;
		m_data_param.day_index = 0;
		m_data_param.activity_begin_day_index = GetZeroTime(EngineAdapter::Instance().Time()) / SECOND_PER_DAY;
		crossgameprotocal::CrossGameWorldCommonDataChange cgwcdc;
		cgwcdc.change_type = crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_1V1;
		cgwcdc.sub_type = crossgameprotocal::WORLD_COMMONDATA_SUB_TYPE_SEANSON;
		cgwcdc.param_2 = m_data_param.activity_begin_day_index; // m_data_param.season_count;
		CrossServer::GetInstCrossServer()->SendToAllGameWorld((const char *)&cgwcdc, sizeof(cgwcdc));
	}
}

void CrossActivity1V1::OnSendCross1v1MonthInfo()
{
	int min_score = 0;
	{  // 下发排行榜最低分到各个服，各个服判断大于等于这个分数则表示在排行榜上
		long long uuid_list[CROSS_ACTIVITY_1V1_USER_RANK_MAX] = { 0 };
		int top_user_count = CrossServer::GetInstCrossServer()->GetCrossPersonRank()->GetRankTopUser(CROSS_PERSON_RANK_TYPE_1V1_SCORE, CROSS_ACTIVITY_1V1_USER_RANK_MAX, uuid_list);
		for (int i = 0; i < top_user_count; ++i)
		{
			int score = static_cast<int>(CrossServer::GetInstCrossServer()->GetCrossPersonRank()->GetRankValue(CROSS_PERSON_RANK_TYPE_1V1_SCORE, uuid_list[i]));
			int grade = Cross1V1CommonConfig::Instance().GetGradeByScore(score);
			if (grade < Cross1V1CommonConfig::Instance().GetOtherCfg().rank_reward_min_grade)
			{
				continue;
			}

			if (min_score == 0 || min_score > score)
			{
				min_score = score;
			}
		}

		crossgameprotocal::CrossGameWorldCommonDataChange cgwcdc;
		cgwcdc.change_type = crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_1V1;
		cgwcdc.sub_type = crossgameprotocal::WORLD_COMMONDATA_SUB_TYPE_RANK_MIN_SCORE;
		cgwcdc.param_2 = min_score;
		cgwcdc.param_3 = 0; // m_data_param.season_count;
		CrossServer::GetInstCrossServer()->SendToAllGameWorld((const char *)&cgwcdc, sizeof(cgwcdc));
	}
}

void CrossActivity1V1::OnMatchReq(NetID global_netid, crossgameprotocal::GameCrossActivity1V1MatchReq *gcamr)
{
	std::set<long long>::iterator iter = m_uuid_set.find(gcamr->uuid);
	if (iter != m_uuid_set.end())
	{
		return;
	}

	//对于匹配成功后掉线的情况，检查匹配记录是否清除
	this->OnMatchCancel(gcamr->uuid);

	m_uuid_set.insert(gcamr->uuid);

	MatchUnit match_unit;
	match_unit.uuid = gcamr->uuid;
	match_unit.prof = gcamr->prof;
	match_unit.sex = gcamr->sex;
	match_unit.camp = gcamr->camp;
	match_unit.level = gcamr->level;
	match_unit.jingjie = gcamr->jingjie;
	match_unit.score = gcamr->score;
	match_unit.capability = gcamr->capability;
	match_unit.appearance = gcamr->appearance;
	match_unit.day_join_count = gcamr->day_join_count;
	match_unit.had_match_times = 0;
	match_unit.grade = Cross1V1CommonConfig::Instance().GetGradeByScore(gcamr->score);
	match_unit.dur_win_count = gcamr->dur_win_count;
	match_unit.total_join_times = gcamr->total_join_times;
	match_unit.total_win_times = gcamr->total_win_times;
	match_unit.gongxun = gcamr->cross_1v1_gongxun;

	SNPRINTF(match_unit.name, sizeof(match_unit.name), "%s", gcamr->name);

	m_matcher_list.push_back(match_unit);

	Protocol::SCCross1v1MatchAck cam;
	cam.result = 1;
	cam.match_end_left_time = m_next_match_time - (unsigned int)(EngineAdapter::Instance().Time());
	CrossServer::GetInstCrossServer()->SendToUser(LongLongToUniqueUserID(gcamr->uuid), (const char*)&cam, sizeof(cam));

	m_join_activity_uuid.insert(gcamr->uuid);
}

void CrossActivity1V1::OnMatchCancel(long long unique_user_id)
{
	std::set<long long>::iterator iter = m_uuid_set.find(unique_user_id);
	if (iter != m_uuid_set.end())
	{
		m_uuid_set.erase(iter++);
	}

	for (std::vector<MatchUnit>::iterator it = m_matcher_list.begin(); it != m_matcher_list.end();)
	{
		if (it->uuid == unique_user_id)
		{
			it = m_matcher_list.erase(it);
		}
		else
		{
			++it;
		}
	}

	std::map<long long, MatchUnit>::iterator unit_it = m_last_fight_matcher_map.find(unique_user_id);
	if (unit_it != m_last_fight_matcher_map.end())
	{
		m_last_fight_matcher_map.erase(unique_user_id);
	}

	std::map<long long, CrossActivity1v1FightUnit>::iterator match_it = m_match_map.find(unique_user_id);
	if (match_it != m_match_map.end())
	{
		m_match_map.erase(unique_user_id);
	}
}

void CrossActivity1V1::OnMatchQuery(NetID global_netid, const UniqueUserID &unique_user_id, int req_type)
{
	Protocol::SCCross1v1MatchAck cam;
	cam.result = 0;
	cam.match_end_left_time = 0;

	switch (req_type)
	{
	case crossgameprotocal::CROSS1V1_MATCH_RESULT_REQ_TYPE_QUERY:
		{
		std::set<long long>::iterator iter = m_uuid_set.find(UniqueUserIDToLongLong(unique_user_id));
		if (iter != m_uuid_set.end())
		{
			cam.result = 1;
			cam.match_end_left_time = m_next_match_time - (unsigned int)(EngineAdapter::Instance().Time());
		}
		}
		break;
	case crossgameprotocal::CROSS1V1_MATCH_RESULT_REQ_TYPE_CANCEL:
		{
			this->OnMatchCancel(UniqueUserIDToLongLong(unique_user_id));
		}
		break;
	}

	CrossServer::GetInstCrossServer()->SendToUser(unique_user_id, (const char*)&cam, sizeof(cam));
}

void CrossActivity1V1::OnQueryWeekRecord(NetID global_netid, const UniqueUserID &unique_user_id)
{
	static Protocol::SCCross1v1WeekRecord cwr;
	cwr.win_this_week = 0;
	cwr.lose_this_week = 0;
	cwr.record_count = 0;

	std::map<long long, CrossWeekRecord1v1List>::iterator iter = m_cross_week_record_1v1_map.find(UniqueUserIDToLongLong(unique_user_id));
	if (iter != m_cross_week_record_1v1_map.end())
	{
		CrossWeekRecord1v1List &record_list = iter->second;

		cwr.win_this_week = record_list.win_this_week;
		cwr.lose_this_week = record_list.lose_this_week;

		for (int i = 0; i < record_list.record_count && cwr.record_count < CROSS_ACTIVITY_1V1_MAX_RECORD_COUNT; ++i)
		{
			cwr.record_list[cwr.record_count].result = record_list.record_list[i].result;
			cwr.record_list[cwr.record_count].oppo_plat_type = record_list.record_list[i].oppo_plat_type;
			cwr.record_list[cwr.record_count].oppo_server_id = record_list.record_list[i].oppo_server_id;
			cwr.record_list[cwr.record_count].oppo_role_uid = record_list.record_list[i].oppo_role_uid;
			cwr.record_list[cwr.record_count].oppo_capability = record_list.record_list[i].oppo_capability;
			cwr.record_list[cwr.record_count].add_score = record_list.record_list[i].add_score;
			F_STRNCPY(cwr.record_list[cwr.record_count].oppo_name, record_list.record_list[i].oppo_name, sizeof(cwr.record_list[cwr.record_count].oppo_name));

			++ cwr.record_count;
		}
	}

	CrossServer::GetInstCrossServer()->SendToUser(unique_user_id, (const char*)&cwr, sizeof(cwr));
}

void CrossActivity1V1::OnFightResult(NetID global_netid, crossgameprotocal::GameCrossCross1V1FightResult *gccfr)
{
	static crossgameprotocal::CrossGameUpdateCrossRecord cgucr;
	cgucr.update_cross_record_type = crossgameprotocal::CrossGameUpdateCrossRecord::UPDATE_CROSS_RECORD_CROSS_1V1;
	const Cross1V1OtherConfig &other_cfg = Cross1V1CommonConfig::Instance().GetOtherCfg();

	std::map<long long, MatchUnit>::iterator it1 = m_last_fight_matcher_map.find(gccfr->uuid1);
	std::map<long long, MatchUnit>::iterator it2 = m_last_fight_matcher_map.find(gccfr->uuid2);

	static Protocol::SCCross1v1FightResult cfr;
	if (it1 != m_last_fight_matcher_map.end() && it2 != m_last_fight_matcher_map.end())
	{
		int reward_score = 0;
		int reward_gongxun = 0;

		// it1
		{
			const Cross1V1GradeScoreConfig *grade_cfg = Cross1V1CommonConfig::Instance().GetGradeScoreCfgByGrade(it1->second.grade);
			if (NULL == grade_cfg)
			{
				return;
			}

			if (0 != gccfr->user1_is_win)
			{
				//reward_score += grade_cfg->victory_score;
				//reward_score += Cross1V1CommonConfig::Instance().GetExtraScoreByWinTimes(it1->second.dur_win_count + 1);
				// 修改积分公式 单场获得积分 = 本段位胜利积分 * （MAX（连胜场次 - 2，0） * 0.5 + 1）
				reward_score += (int)(grade_cfg->victory_score * (((std::max)(it1->second.dur_win_count + 1 - 2, 0)) * 0.5f + 1));
				
				reward_gongxun = other_cfg.winner_score;
			}
			else
			{
				if (Cross1V1CommonConfig::Instance().GetIsAddFailScoreByGrade(it1->second.grade))
				{
					reward_score += grade_cfg->fail_score;
				}
				else
				{
					reward_score -= grade_cfg->fail_score;
				}
				reward_score += Cross1V1CommonConfig::Instance().GetExtraScoreByGap(it1->second.grade, it2->second.grade);
				reward_gongxun = other_cfg.loser_score;
			}

			{
				cgucr.uid = UidToInt(LongLongToUniqueUserID(gccfr->uuid1).user_id);
				cgucr.record_param_list[0] = (it1->second.score + reward_score) > 0 ? it1->second.score + reward_score : 0;
				cgucr.record_param_list[1] = it1->second.day_join_count + 1;
				cgucr.record_param_list[2] = static_cast<int>(it1->second.capability);
				cgucr.record_param_list[3] = gccfr->user1_is_win ? it1->second.dur_win_count + 1 : 0;
				cgucr.record_param_list[4] = Cross1V1CommonConfig::Instance().GetGradeByScore(static_cast<int>(cgucr.record_param_list[0]));
				cgucr.record_param_list[5] = it1->second.total_join_times + 1;
				cgucr.record_param_list[6] = gccfr->user1_is_win ? it1->second.total_win_times + 1 : it1->second.total_win_times;
				cgucr.record_param_list[7] = it1->second.gongxun + reward_gongxun;
				cgucr.appearance = it1->second.appearance;

				CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(gccfr->uuid1), (const char *)&cgucr, sizeof(cgucr));
			}

			{
				cfr.result = gccfr->user1_is_win ? 1 : 0;
				cfr.award_score = reward_score;

				CrossServer::GetInstCrossServer()->SendToCrossUser(LongLongToUniqueUserID(gccfr->uuid1), (const char*)&cfr, sizeof(cfr));
			}

			{
				CrossWeekRecord1v1List &record_list = m_cross_week_record_1v1_map[gccfr->uuid1];

				record_list.win_this_week = 0;
				record_list.lose_this_week = 0;

				CrossWeekRecord1v1 record;
				record.result = cfr.result;
				record.uuid = gccfr->uuid1;
				record.oppo_plat_type = LongLongToUniqueUserID(gccfr->uuid2).plat_type;
				record.oppo_server_id = LongLongToUniqueUserID(gccfr->uuid2).user_id.db_index;
				record.oppo_role_uid = UidToInt(LongLongToUniqueUserID(gccfr->uuid2).user_id);
				record.add_score = reward_score;
				record.oppo_capability = it2->second.capability;
				F_STRNCPY(record.oppo_name, it2->second.name, sizeof(record.oppo_name));

				if (record_list.record_count >= 0 && record_list.record_count < CROSS_ACTIVITY_1V1_MAX_RECORD_COUNT)
				{
					record_list.record_list[record_list.record_count++] = record;
				}
				else
				{
					for (int i = 1; i < CROSS_ACTIVITY_1V1_MAX_RECORD_COUNT; ++i)
					{
						record_list.record_list[i - 1] = record_list.record_list[i];
					}

					record_list.record_list[CROSS_ACTIVITY_1V1_MAX_RECORD_COUNT - 1] = record;
				}
			}

			{
				CrossActivity1V1RankItem rank_item;
				rank_item.plat_type = LongLongToUniqueUserID(gccfr->uuid1).plat_type;
				rank_item.role_id = UidToInt(LongLongToUniqueUserID(gccfr->uuid1).user_id);
				rank_item.level = it1->second.level;
				rank_item.prof = it1->second.prof;
				rank_item.sex = it1->second.sex;
				rank_item.score = (it1->second.score + reward_score) > 0 ? it1->second.score + reward_score : 0;
				rank_item.capability = it1->second.capability;
				F_STRNCPY(rank_item.name, it1->second.name, sizeof(rank_item.name));

				this->UpdataUserRank(rank_item);
			}
		}

		// it2
		{
			reward_score = 0;
			reward_gongxun = 0;
			const Cross1V1GradeScoreConfig *grade_cfg = Cross1V1CommonConfig::Instance().GetGradeScoreCfgByGrade(it2->second.grade);
			if (NULL == grade_cfg)
			{
				return;
			}
			if (0 != gccfr->user2_is_win)
			{
				//reward_score += grade_cfg->victory_score;
				//reward_score += Cross1V1CommonConfig::Instance().GetExtraScoreByWinTimes(it2->second.dur_win_count + 1);
				// 修改积分公式 单场获得积分 = 本段位胜利积分 * （MAX（连胜场次 - 2，0） * 0.5 + 1）
				reward_score += (int)(grade_cfg->victory_score * (((std::max)(it2->second.dur_win_count + 1 - 2, 0)) * 0.5f + 1));

				reward_gongxun = other_cfg.winner_score;
			}
			else
			{
				if (Cross1V1CommonConfig::Instance().GetIsAddFailScoreByGrade(it2->second.grade))
				{
					reward_score += grade_cfg->fail_score;
				}
				else
				{
					reward_score -= grade_cfg->fail_score;
				}
				reward_score += Cross1V1CommonConfig::Instance().GetExtraScoreByGap(it2->second.grade, it1->second.grade);
				reward_gongxun = other_cfg.loser_score;
			}

			{
				cgucr.uid = UidToInt(LongLongToUniqueUserID(gccfr->uuid2).user_id);
				cgucr.record_param_list[0] = (it2->second.score + reward_score) > 0 ? it2->second.score + reward_score : 0;
				cgucr.record_param_list[1] = it2->second.day_join_count + 1;
				cgucr.record_param_list[2] = static_cast<int>(it2->second.capability);
				cgucr.record_param_list[3] = gccfr->user2_is_win ? it2->second.dur_win_count + 1 : 0;
				cgucr.record_param_list[4] = Cross1V1CommonConfig::Instance().GetGradeByScore(static_cast<int>(cgucr.record_param_list[0]));
				cgucr.record_param_list[5] = it2->second.total_join_times + 1;
				cgucr.record_param_list[6] = gccfr->user2_is_win ? it2->second.total_win_times + 1 : it2->second.total_win_times;
				cgucr.record_param_list[7] = it2->second.gongxun + reward_gongxun;
				cgucr.appearance = it2->second.appearance;

				CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(gccfr->uuid2), (const char *)&cgucr, sizeof(cgucr));
			}

			{
				cfr.result = gccfr->user2_is_win ? 1 : 0;
				cfr.award_score = reward_score;


				CrossServer::GetInstCrossServer()->SendToCrossUser(LongLongToUniqueUserID(gccfr->uuid2), (const char*)&cfr, sizeof(cfr));
			}

			{
				CrossWeekRecord1v1List &record_list = m_cross_week_record_1v1_map[gccfr->uuid2];

				CrossWeekRecord1v1 record;
				record.result = cfr.result;
				record.uuid = gccfr->uuid2;
				record.oppo_plat_type = LongLongToUniqueUserID(gccfr->uuid1).plat_type;
				record.oppo_server_id = LongLongToUniqueUserID(gccfr->uuid1).user_id.db_index;
				record.oppo_role_uid = UidToInt(LongLongToUniqueUserID(gccfr->uuid1).user_id);
				record.add_score = reward_score;
				record.oppo_capability = it1->second.capability;
				F_STRNCPY(record.oppo_name, it1->second.name, sizeof(record.oppo_name));

				if (record_list.record_count >= 0 && record_list.record_count < CROSS_ACTIVITY_1V1_MAX_RECORD_COUNT)
				{
					record_list.record_list[record_list.record_count++] = record;
				}
				else
				{
					for (int i = 1; i < CROSS_ACTIVITY_1V1_MAX_RECORD_COUNT; ++i)
					{
						record_list.record_list[i - 1] = record_list.record_list[i];
					}

					record_list.record_list[CROSS_ACTIVITY_1V1_MAX_RECORD_COUNT - 1] = record;
				}
			}

			{
				CrossActivity1V1RankItem rank_item;
				rank_item.plat_type = LongLongToUniqueUserID(gccfr->uuid2).plat_type;
				rank_item.role_id = UidToInt(LongLongToUniqueUserID(gccfr->uuid2).user_id);
				rank_item.level = it2->second.level;
				rank_item.prof = it2->second.prof;
				rank_item.sex = it2->second.sex;
				rank_item.score = (it2->second.score + reward_score) > 0 ? it2->second.score + reward_score : 0;
				rank_item.capability = it2->second.capability;
				F_STRNCPY(rank_item.name, it2->second.name, sizeof(rank_item.name));

				this->UpdataUserRank(rank_item);
			}
		}

		m_match_map.erase(gccfr->uuid1);
		m_match_map.erase(gccfr->uuid2);
		m_last_fight_matcher_map.erase(gccfr->uuid1);
		m_last_fight_matcher_map.erase(gccfr->uuid2);
	}
	else if (it1 != m_last_fight_matcher_map.end() && it2 == m_last_fight_matcher_map.end())
	{
		int reward_score = 0;
		int reward_gongxun = 0;
		const Cross1V1GradeScoreConfig *grade_cfg = Cross1V1CommonConfig::Instance().GetGradeScoreCfgByGrade(it1->second.grade);
		if (NULL == grade_cfg)
		{
			return;
		}
		if (0 != gccfr->user1_is_win)
		{
			//reward_score += grade_cfg->victory_score;
			// 修改积分公式 单场获得积分 = 本段位胜利积分 * （MAX（连胜场次 - 2，0） * 0.5 + 1）
			reward_score += (int)(grade_cfg->victory_score * (((std::max)(it1->second.dur_win_count + 1 - 2, 0)) * 0.5f + 1));

			reward_gongxun += other_cfg.winner_score;
		}
		else
		{
			if (Cross1V1CommonConfig::Instance().GetIsAddFailScoreByGrade(it1->second.grade))
			{
				reward_score += grade_cfg->fail_score;
			}
			else
			{
				reward_score -= grade_cfg->fail_score;
			}
			reward_gongxun += other_cfg.loser_score;
		}

		{
			cgucr.uid = UidToInt(LongLongToUniqueUserID(gccfr->uuid1).user_id);
			cgucr.record_param_list[0] = (it1->second.score + reward_score) > 0 ? it1->second.score + reward_score : 0;
			cgucr.record_param_list[1] = it1->second.day_join_count + 1;
			cgucr.record_param_list[2] = static_cast<int>(it1->second.capability);
			cgucr.record_param_list[3] = gccfr->user1_is_win ? it1->second.dur_win_count + 1 : 0;
			cgucr.record_param_list[4] = Cross1V1CommonConfig::Instance().GetGradeByScore(static_cast<int>(cgucr.record_param_list[0]));
			cgucr.record_param_list[5] = it1->second.total_join_times + 1;
			cgucr.record_param_list[6] = gccfr->user1_is_win ? it1->second.total_win_times + 1 : it1->second.total_win_times;
			cgucr.record_param_list[7] = it1->second.gongxun + reward_gongxun;
			
			cgucr.appearance = it1->second.appearance;

			CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(gccfr->uuid1), (const char *)&cgucr, sizeof(cgucr));
		}

		{
			cfr.result = gccfr->user1_is_win ? 1 : 0;
			cfr.award_score = reward_score;
			CrossServer::GetInstCrossServer()->SendToCrossUser(LongLongToUniqueUserID(gccfr->uuid1), (const char*)&cfr, sizeof(cfr));
		}

		{
			CrossWeekRecord1v1List &record_list = m_cross_week_record_1v1_map[gccfr->uuid1];

			record_list.win_this_week = 0;
			record_list.lose_this_week = 0;

			CrossWeekRecord1v1 record;
			record.result = cfr.result;
			record.uuid = gccfr->uuid1;
			record.oppo_plat_type = 0;

			record.oppo_server_id = 0;
			record.oppo_role_uid = 0;
			record.add_score = reward_score;
			record.oppo_capability = 0;
			F_STRNCPY(record.oppo_name, it1->second.oppo_name, sizeof(record.oppo_name));

			if (record_list.record_count >= 0 && record_list.record_count < CROSS_ACTIVITY_1V1_MAX_RECORD_COUNT)
			{
				record_list.record_list[record_list.record_count++] = record;
			}
			else
			{
				for (int i = 1; i < CROSS_ACTIVITY_1V1_MAX_RECORD_COUNT; ++i)
				{
					record_list.record_list[i - 1] = record_list.record_list[i];
				}

				record_list.record_list[CROSS_ACTIVITY_1V1_MAX_RECORD_COUNT - 1] = record;
			}
		}

		{
			CrossActivity1V1RankItem rank_item;
			rank_item.plat_type = LongLongToUniqueUserID(gccfr->uuid1).plat_type;
			rank_item.role_id = UidToInt(LongLongToUniqueUserID(gccfr->uuid1).user_id);
			rank_item.level = it1->second.level;
			rank_item.prof = it1->second.prof;
			rank_item.sex = it1->second.sex;
			rank_item.score = (it1->second.score + reward_score) > 0 ? it1->second.score + reward_score : 0;
			rank_item.capability = it1->second.capability;
			F_STRNCPY(rank_item.name, it1->second.name, sizeof(rank_item.name));

			this->UpdataUserRank(rank_item);
		}

		m_match_map.erase(gccfr->uuid1);
		m_last_fight_matcher_map.erase(gccfr->uuid1);
	}
}

void CrossActivity1V1::OnGlobalSyncRankList(NetID global_netid, crossgameprotocal::GameCrossCross1V1RankList *gccrl)
{
	for (int i = 0; i < gccrl->count && crossgameprotocal::MAX_CROSS_1V1_RANK_COUNT; i++)
	{
		CrossActivity1V1RankItem rank_item;
		rank_item.plat_type = gccrl->rank_list[i].plat_type;
		rank_item.role_id = gccrl->rank_list[i].role_id;
		F_STRNCPY(rank_item.name, gccrl->rank_list[i].name, sizeof(rank_item.name));
		rank_item.level = gccrl->rank_list[i].level;
		rank_item.prof = gccrl->rank_list[i].prof;
		rank_item.sex = gccrl->rank_list[i].sex;
		rank_item.score = gccrl->rank_list[i].score;
		rank_item.capability = gccrl->rank_list[i].capability;

		this->UpdataUserRank(rank_item);
	}
}

void CrossActivity1V1::OnSyncRoleRankBaseInfo(NetID global_netid, crossgameprotocal::GameCrossActivity1V1RoleRankBaseInfo *gcarrbi)
{
	CrossActivity1V1RankItem *user_rank_item = m_user_rank.GetUserRankItem(gcarrbi->uuid);
	if (NULL != user_rank_item)
	{
		user_rank_item->level = gcarrbi->level;
		user_rank_item->capability = gcarrbi->capability;
	}
}

time_t CrossActivity1V1::CalcActivityNextOpenTime()
{
	if (!Cross1V1CommonConfig::Instance().GetOtherCfg().is_open)
	{
		return 0;
	}

	Cross1V1OpenTimeConfig cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];
	int cfg_count = Cross1V1CommonConfig::Instance().GetOpenActivityConfigList(cfg_list);

	int min_interval = INT_MAX;
	int min_locate_idx = 0;
	for (int index = 0; index < cfg_count; ++index)
	{
		Cross1V1OpenTimeConfig &open_cfg = cfg_list[index];
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

void CrossActivity1V1::OnChangeToNextStatus()
{
	if (!Cross1V1CommonConfig::Instance().GetOtherCfg().is_open)
	{
		m_activity_status = ACTIVITY_STATUS_CLOSE;
		m_status_end_time = 0;
		return;
	}

	//int max_season = Cross1V1CommonConfig::Instance().GetMaxSeason();
	//if (max_season > 0 && m_data_param.season_count > max_season)
	//{
	//	m_activity_status = ACTIVITY_STATUS_CLOSE;
	//	m_status_end_time = 0;
	//	return;
	//}

	const Cross1V1OtherConfig &other_cfg = Cross1V1CommonConfig::Instance().GetOtherCfg();
	if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		int standby_time = Cross1V1CommonConfig::Instance().GetActivityStandbyTime(m_open_cfg_idx);
		if (standby_time > 0)
		{
			m_activity_status = ACTIVITY_STATUS_STANDY;
			m_status_end_time = EngineAdapter::Instance().Time() + standby_time;
		}
		else
		{
			m_activity_status = ACTIVITY_STATUS_OPEN;
			m_status_end_time = EngineAdapter::Instance().Time() + Cross1V1CommonConfig::Instance().GetActivityOpenTime(m_open_cfg_idx);
		}
	}
	else if (ACTIVITY_STATUS_STANDY == m_activity_status)
	{
		m_activity_status = ACTIVITY_STATUS_OPEN;

		m_fight_seq = 0;
		m_matcher_list.clear();
		m_uuid_set.clear();
		m_next_match_time = (unsigned int)(EngineAdapter::Instance().Time()) + other_cfg.match_time_s;
		m_next_rank_time = (unsigned int)(EngineAdapter::Instance().Time()) + this->GetRankInterval();
		
		m_status_end_time = EngineAdapter::Instance().Time() + Cross1V1CommonConfig::Instance().GetActivityOpenTime(m_open_cfg_idx);
	}
	else
	{
		m_activity_status = ACTIVITY_STATUS_CLOSE;

		m_fight_seq = 0;
		m_matcher_list.clear();
		m_match_map.clear();
		m_uuid_set.clear();

		m_status_end_time = this->CalcActivityNextOpenTime();
	}

	if (ACTIVITY_STATUS_OPEN == m_activity_status)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_1v1_activity_open_notice);
		if (length > 0)
		{
			m_cross_activity_manager->GetCrossServer()->SystemMsgToAllGlobalUser(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
		}
	}
	else if (ACTIVITY_STATUS_CLOSE == m_activity_status)
	{
		const static int TOP_RANK_ITEM_COUNT = 5;
		CrossActivity1V1RankItem rank_item_list[TOP_RANK_ITEM_COUNT];
		int count = m_user_rank.GetTopRankItem(TOP_RANK_ITEM_COUNT, rank_item_list);

		char names[256] = {0}; memset(names, 0, sizeof(names));
		int names_len = 0;

		for (int i = 0; i < count && i < TOP_RANK_ITEM_COUNT; i ++)
		{
			int tmp_len = SNPRINTF(names + names_len, sizeof(names) - names_len, "%s ", rank_item_list[i].name);
			names_len += tmp_len;
		}

		if (names_len > 0)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_1v1_activity_close_notice, names);
			if (length > 0)
			{
				//m_cross_activity_manager->GetCrossServer()->SystemMsgToAllGlobalUser(gamestring::GAMESTRING_BUF, length);
				m_cross_activity_manager->GetCrossServer()->SystemMsgToAllGlobalUser(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
			}
		}

		{
			// 统计数据
			std::map<int, int> plat_join_num;
			for (std::set<long long>::iterator it = m_join_activity_uuid.begin(); m_join_activity_uuid.end() != it; ++ it)
			{
				UniqueUserID uuid = LongLongToUniqueUserID(*it);

				if (plat_join_num.end() == plat_join_num.find(uuid.plat_type))
				{
					plat_join_num[uuid.plat_type] = 1;
				}
				else
				{
					++ plat_join_num[uuid.plat_type];
				}
			}

			for (std::map<int, int>::iterator it = plat_join_num.begin(); plat_join_num.end() != it; ++ it)
			{
				PlatName plat_name;

				LocalConfig::GetPlatName(it->first, plat_name);
				plat_name[sizeof(plat_name) - 1] = '\0';

				crosslog::g_log_cross_activity.printf(LL_INFO, "CrossActivity1V1 Join Number Plat_type:%s, num:%d", plat_name, it->second);
			}

			m_join_activity_uuid.clear();
		}
	}
}

static bool MatchSortFunc(const CrossActivity1V1::MatchUnit &item1, const CrossActivity1V1::MatchUnit &item2)
{
	return item1.score > item2.score;
}

int CrossActivity1V1::GetOppoIndexByUnit(MatchUnit match_unit, int match_index)
{
	const Cross1V1PriorityConfig *priority_cfg = Cross1V1CommonConfig::Instance().GetPriorityCfgByMatchTimes(match_unit.had_match_times);
	if (NULL == priority_cfg)
	{
		return match_index;
	}

	// 筛选符合匹配规则的玩家
	std::vector<int> temp_vec;
	int list_size = static_cast<int>(m_matcher_list.size());
	for (int i = 0; i < list_size; ++i)
	{
		if (m_last_fight_matcher_map.find(m_matcher_list[i].uuid) != m_last_fight_matcher_map.end())
		{
			continue;
		}

		if (m_matcher_list[i].uuid == match_unit.uuid)
		{
			continue;
		}

		if (m_matcher_list[i].grade < match_unit.grade - priority_cfg->interval || m_matcher_list[i].grade > match_unit.grade + priority_cfg->interval)
		{
			continue;
		}

		temp_vec.push_back(i);
	}

	if (temp_vec.size() == 0)
	{
		return match_index;
	}
	return temp_vec[RandomNum(static_cast<int>(temp_vec.size()))];
}

void CrossActivity1V1::DoMatch()
{
	std::sort(m_matcher_list.begin(), m_matcher_list.end(), MatchSortFunc);

	for (std::vector<MatchUnit>::iterator it = m_matcher_list.begin(); it != m_matcher_list.end(); ++it)
	{
		++(it->had_match_times);
	}

	static crossgameprotocal::CrossGameSyncCross1v1MatchInfo match_info;
	match_info.m_next_match_left_time = m_next_match_time - (unsigned int)(EngineAdapter::Instance().Time());
	match_info.unit_count = 0;

	const Cross1V1OtherConfig &other_cfg = Cross1V1CommonConfig::Instance().GetOtherCfg();
	if (m_matcher_list.empty())
	{
		return;
	}

	int list_size = static_cast<int>(m_matcher_list.size());
	for (int match_index = 0; match_index < list_size; match_index++)
	{
		if (m_last_fight_matcher_map.find(m_matcher_list[match_index].uuid) != m_last_fight_matcher_map.end())
		{
			continue;
		}

		CrossActivity1v1FightUnit fight_unit;
		if (m_matcher_list[match_index].had_match_times > Cross1V1CommonConfig::Instance().GetMaxPriority())
		{
			fight_unit.fight_seq = this->GetFightSeq();
			MatchUnit robot_unit;
			const Cross1V1RandNameConfig *rand_name_cfg = Cross1V1CommonConfig::Instance().GetRandNameConfig();
			if (NULL == rand_name_cfg)
			{
				continue;
			}

			robot_unit.uuid = 0;
			robot_unit.sex = rand_name_cfg->sex;

			if (MALE == (int)rand_name_cfg->sex)
			{
				robot_unit.prof = PROF_TYPE_PROF_1;
			}
			else
			{
				robot_unit.prof = PROF_TYPE_PROF_3;
			}
			robot_unit.camp = RandomNum(1, CAMP_TYPE_MAX);

			int rand_num = RandomNum(80, 120);
			robot_unit.level = static_cast<short>(m_matcher_list[match_index].level * (rand_num / 100.0f)) > MAX_ROLE_LEVEL ? MAX_ROLE_LEVEL : static_cast<short>(m_matcher_list[match_index].level * (rand_num / 100.0f));
			robot_unit.level = robot_unit.level < other_cfg.limit_join_level ? other_cfg.limit_join_level : robot_unit.level;
			robot_unit.capability = static_cast<long long>(m_matcher_list[match_index].capability * (rand_num / 100.0f));
			F_STRNCPY(robot_unit.name, rand_name_cfg->name, sizeof(GameName));
			F_STRNCPY(m_matcher_list[match_index].oppo_name, rand_name_cfg->name, sizeof(GameName));
			m_last_fight_matcher_map[m_matcher_list[match_index].uuid] = m_matcher_list[match_index];

			this->PutMatchToFightUnit(m_matcher_list[match_index], fight_unit, 1);
			m_last_fight_matcher_map[m_matcher_list[match_index].uuid] = m_matcher_list[match_index];
			m_matcher_list[match_index].had_match_times = 0;

			this->PutMatchToFightUnit(robot_unit, fight_unit, 2);
			fight_unit.is_with_robot1 = 1;
			fight_unit.rand_per_attr = rand_num;

			m_match_map[fight_unit.uuid1] = fight_unit;

			unsigned int fight_start_left_time = Cross1V1CommonConfig::Instance().GetFightWaitTime();
			unsigned int fight_end_left_time = other_cfg.fight_time_s + other_cfg.fight_wait_s;

			Protocol::SCCross1v1MatchResult cmr;
			cmr.result = 0;
			cmr.fight_start_left_time = fight_start_left_time;
			cmr.side = 1;
			cmr.oppo_plat_type = 0;
			cmr.oppo_server_id = 0;
			cmr.oppo_role_id = 0;
			cmr.oppo_prof = fight_unit.prof2;
			cmr.oppo_sex = fight_unit.sex2;
			cmr.oppo_camp = fight_unit.camp2;
			cmr.oppo_level = fight_unit.level2;
			cmr.oppo_capability = fight_unit.capability2;
			cmr.fight_end_left_time = fight_end_left_time;
			SNPRINTF(cmr.oppo_name, sizeof(cmr.oppo_name), "%s", fight_unit.user_name2);
			CrossServer::GetInstCrossServer()->SendToUser(LongLongToUniqueUserID(fight_unit.uuid1), (const char*)&cmr, sizeof(cmr));
		}
		else
		{
			int oppo_index = this->GetOppoIndexByUnit(m_matcher_list[match_index], match_index);
			if (oppo_index != match_index)
			{
				fight_unit.fight_seq = this->GetFightSeq();

				this->PutMatchToFightUnit(m_matcher_list[match_index], fight_unit, 1);
				m_last_fight_matcher_map[m_matcher_list[match_index].uuid] = m_matcher_list[match_index];
				m_matcher_list[match_index].had_match_times = 0;

				this->PutMatchToFightUnit(m_matcher_list[oppo_index], fight_unit, 2);
				m_last_fight_matcher_map[m_matcher_list[oppo_index].uuid] = m_matcher_list[oppo_index];
				m_matcher_list[oppo_index].had_match_times = 0;


				if (fight_unit.uuid1 > 0 && fight_unit.uuid2 > 0)
				{
					m_match_map[fight_unit.uuid1] = fight_unit;
					m_match_map[fight_unit.uuid2] = fight_unit;

					unsigned int fight_start_left_time = Cross1V1CommonConfig::Instance().GetFightWaitTime();
					unsigned int fight_end_left_time = other_cfg.fight_time_s + other_cfg.fight_wait_s;

					Protocol::SCCross1v1MatchResult cmr;
					cmr.result = 0;
					cmr.fight_start_left_time = fight_start_left_time;
					cmr.side = 1;
					cmr.oppo_plat_type = LongLongToUniqueUserID(fight_unit.uuid2).plat_type;
					cmr.oppo_server_id = LongLongToUniqueUserID(fight_unit.uuid2).user_id.db_index;
					cmr.oppo_role_id = UidToInt(LongLongToUniqueUserID(fight_unit.uuid2).user_id);
					cmr.oppo_prof = fight_unit.prof2;
					cmr.oppo_sex = fight_unit.sex2;
					cmr.oppo_camp = fight_unit.camp2;
					cmr.oppo_level = fight_unit.level2;
					cmr.oppo_capability = fight_unit.capability2;
					cmr.fight_end_left_time = fight_end_left_time;
					SNPRINTF(cmr.oppo_name, sizeof(cmr.oppo_name), "%s", fight_unit.user_name2);
					CrossServer::GetInstCrossServer()->SendToUser(LongLongToUniqueUserID(fight_unit.uuid1), (const char*)&cmr, sizeof(cmr));

					cmr.result = 0;
					cmr.fight_start_left_time = fight_start_left_time;
					cmr.side = 2;
					cmr.oppo_plat_type = LongLongToUniqueUserID(fight_unit.uuid1).plat_type;
					cmr.oppo_server_id = LongLongToUniqueUserID(fight_unit.uuid1).user_id.db_index;
					cmr.oppo_role_id = UidToInt(LongLongToUniqueUserID(fight_unit.uuid1).user_id);
					cmr.oppo_prof = fight_unit.prof1;
					cmr.oppo_sex = fight_unit.sex1;
					cmr.oppo_camp = fight_unit.camp1;
					cmr.oppo_level = fight_unit.level1;
					cmr.oppo_capability = fight_unit.capability1;
					cmr.fight_end_left_time = fight_end_left_time;
					SNPRINTF(cmr.oppo_name, sizeof(cmr.oppo_name), "%s", fight_unit.user_name1);
					CrossServer::GetInstCrossServer()->SendToUser(LongLongToUniqueUserID(fight_unit.uuid2), (const char*)&cmr, sizeof(cmr));
				}
			}
		}

		if (fight_unit.capability2 > 0 &&  match_info.unit_count < crossgameprotocal::CrossGameSyncCross1v1MatchInfo::MAX_FIGHT_UNIT_COUNT)
		{
			match_info.fight_unit_list[match_info.unit_count++] = fight_unit;

			// 拿到循环外面执行，否则隐藏服会执行多次创建副本
			//int sendlen = sizeof(match_info) - (crossgameprotocal::CrossGameSyncCross1v1MatchInfo::MAX_FIGHT_UNIT_COUNT - match_info.unit_count) * sizeof(match_info.fight_unit_list[0]);
			//CrossServer::GetInstCrossServer()->SendToAllGameWorld((const char *)&match_info, sendlen, 0);
		}
	}

	// 通知跨服创建副本，匹配成功的战斗组等待传送进入跨服
	if (match_info.unit_count > 0)
	{
		int sendlen = sizeof(match_info) - (crossgameprotocal::CrossGameSyncCross1v1MatchInfo::MAX_FIGHT_UNIT_COUNT - match_info.unit_count) * sizeof(match_info.fight_unit_list[0]);
		CrossServer::GetInstCrossServer()->SendToAllGameWorld((const char *)&match_info, sendlen);
	}

	for (std::vector<MatchUnit>::iterator it = m_matcher_list.begin(); it != m_matcher_list.end();)
	{
		if (m_last_fight_matcher_map.find(it->uuid) != m_last_fight_matcher_map.end())
		{
			it = m_matcher_list.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (std::set<long long>::iterator it = m_uuid_set.begin(); it != m_uuid_set.end();)
	{
		if (m_last_fight_matcher_map.find(*it) != m_last_fight_matcher_map.end())
		{
			m_uuid_set.erase(it++);
		}
		else
		{
			++it;
		}
	}

}

int CrossActivity1V1::GetFightSeq()
{
	return ++ m_fight_seq;
}

int CrossActivity1V1::GetRankInterval()
{
	const Cross1V1OtherConfig &other_cfg = Cross1V1CommonConfig::Instance().GetOtherCfg();
	return other_cfg.match_time_s + other_cfg.fight_wait_s + other_cfg.fight_time_s + 1;
}

void CrossActivity1V1::PutMatchToFightUnit(MatchUnit &match_unit, CrossActivity1v1FightUnit &fight_unit, int side, bool is_with_robot)
{
	if (1 == side)
	{
		fight_unit.uuid1 = match_unit.uuid;
		fight_unit.prof1 = match_unit.prof;
		fight_unit.sex1 = match_unit.sex;
		fight_unit.camp1 = match_unit.camp;
		fight_unit.level1 = match_unit.level;
		fight_unit.capability1 = match_unit.capability;
		fight_unit.is_with_robot1 = is_with_robot ? 1 : 0;
		SNPRINTF(fight_unit.user_name1, sizeof(fight_unit.user_name1), "%s", match_unit.name);
	}
	else
	{
		fight_unit.uuid2 = match_unit.uuid;
		fight_unit.prof2 = match_unit.prof;
		fight_unit.sex2 = match_unit.sex;
		fight_unit.camp2 = match_unit.camp;
		fight_unit.level2 = match_unit.level;
		fight_unit.capability2 = match_unit.capability;
		fight_unit.is_with_robot2 = is_with_robot ? 1 : 0;
		SNPRINTF(fight_unit.user_name2, sizeof(fight_unit.user_name2), "%s", match_unit.name);
	}
}

void CrossActivity1V1::UpdataUserRank(const CrossActivity1V1RankItem &rank_item)
{
	CrossrPersonRankItem person_rank_item;

	person_rank_item.rank_type = CROSS_PERSON_RANK_TYPE_1V1_SCORE;
	person_rank_item.plat_type = rank_item.plat_type;
	person_rank_item.role_id = rank_item.role_id;
	person_rank_item.sex = rank_item.sex;
	person_rank_item.prof = rank_item.prof;
	person_rank_item.level = rank_item.level;
	F_STRNCPY(person_rank_item.name, rank_item.name, sizeof(GameName));
	person_rank_item.rank_value = rank_item.score;
	person_rank_item.flexible_ll = rank_item.capability;

	CrossServer::GetInstCrossServer()->GetCrossPersonRank()->Cross1v1UpdateRankList(person_rank_item);
}

void CrossActivity1V1::ClearMatchInfoByUniqueServerID(const UniqueServerID &unique_server_id)
{
	if (ACTIVITY_STATUS_OPEN != m_activity_status)
	{
		return;
	}

	for (std::set<long long>::iterator iter = m_uuid_set.begin(); iter != m_uuid_set.end();)
	{
		UniqueUserID unique_user_id = LongLongToUniqueUserID(*iter);
		UniqueServerID tmp_server_id(unique_user_id.plat_type, unique_user_id.user_id.db_index);
		if (unique_server_id == tmp_server_id)
		{
			m_join_activity_uuid.erase(*iter);

			m_uuid_set.erase(iter++);
		}
		else
		{
			++iter;
		}
	}

// 	for (std::vector<MatchUnit>::iterator it = m_matcher_list.begin(); it != m_matcher_list.end();)
// 	{
// 		UniqueUserID &unique_user_id = LongLongToUniqueUserID(it->uuid);
// 		UniqueServerID tmp_server_id(unique_user_id.plat_type, unique_user_id.user_id.db_index);
// 		if (unique_server_id == tmp_server_id)
// 		{
// 			it = m_matcher_list.erase(it);
// 		}
// 		else
// 		{
// 			++it;
// 		}
// 	}

	auto match_iter = std::remove_if(m_matcher_list.begin(), m_matcher_list.end(), [unique_server_id](MatchUnit &mu)
	{
		UniqueUserID unique_user_id = LongLongToUniqueUserID(mu.uuid);
		UniqueServerID tmp_server_id(unique_user_id.plat_type, unique_user_id.user_id.db_index);
		return unique_server_id == tmp_server_id;
	});

	m_matcher_list.erase(match_iter, m_matcher_list.end());

	// log
	crosslog::g_log_cross_activity.printf(LL_WARNING, "Clear 1v1 match on gameserver[serverID:%d][plat_type:%d] disconnect..",
		unique_server_id.server_id, unique_server_id.plat_type);
}