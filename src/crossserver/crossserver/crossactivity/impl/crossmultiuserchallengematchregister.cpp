#include "crossmultiuserchallengematchregister.hpp"

#include "engineadapter.h"

#include "servercommon/internalprotocal/ggprotocal.h"

#include "crossactivitycrossmultiuserchallenge.hpp"
#include "crossserver.h"

#include "servercommon/serverconfig/cross3v3config.hpp"

CrossMultiuserChallengeMatchRegister::CrossMultiuserChallengeMatchRegister()
	: m_cross_activity_multiuser_challenge(NULL), m_inc_key(0)
{

}

CrossMultiuserChallengeMatchRegister::~CrossMultiuserChallengeMatchRegister()
{

}

void CrossMultiuserChallengeMatchRegister::Clear()
{
	m_match_map.clear();
	m_user_match_map.clear();
}

void CrossMultiuserChallengeMatchRegister::Update(unsigned long interval, time_t now_second)
{
	for (CrossMultiuserChallengeMatchMapIt it = m_match_map.begin(); it != m_match_map.end(); )
	{
		if (CROSS_MULTIUSER_CHALLENGE_MATCH_STATUS_FINISH != it->second.status && 0 != it->second.next_status_time && now_second >= it->second.next_status_time)
		{
			it->second.next_status_time = 0;
			++ it->second.status;

			CrossMultiuserChallengeMatchInfo *match_info = &it->second;
			++ it;

			this->OnMatchStatusChange(match_info);
		}
		else
		{
			++ it;
		}
	}
}

CrossMultiuserChallengeMatchInfo * CrossMultiuserChallengeMatchRegister::CreateMatch(CrossMultiuserChallengeSideUserList *side_user_list, CrossMultiuserChallengeSideUserList *other_side_user_list)
{
	UniqueUserID unique_user_id_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX];
	long long capability_list[CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX] = {0};

	UNSTD_STATIC_CHECK(CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT > 0);
	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; i++)
	{
		if (i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT)
		{
			unique_user_id_list[i] = side_user_list->user_list[i].unique_user_id;
			capability_list[i] = side_user_list->user_list[i].capability;
		}
		else
		{
			unique_user_id_list[i] = other_side_user_list->user_list[i % CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT].unique_user_id;
			capability_list[i] = other_side_user_list->user_list[i % CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT].capability;
		}
	}

	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; i++)
	{
		if (INVALID_UNIQUE_USER_ID == unique_user_id_list[i] || NULL != this->GetMatch(unique_user_id_list[i]))
		{
			return NULL;
		}
	}

	CrossMultiuserChallengeMatchInfo match_info;
	match_info.key = ++ m_inc_key;
	match_info.create_time = EngineAdapter::Instance().Time();
	match_info.status = CROSS_MULTIUSER_CHALLENGE_MATCH_STATUS_READY;
	match_info.next_status_time = EngineAdapter::Instance().Time() + Cross3v3Config::Instance().GetReadyTime();
	
	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; i++)
	{
		match_info.unique_user_id_list[i] = unique_user_id_list[i];
		m_user_match_map[unique_user_id_list[i]] = match_info.key;
		match_info.capability_list[i] = capability_list[i];
	}

	m_match_map[match_info.key] = match_info;
	
	CrossMultiuserChallengeMatchInfo *new_match_info = &m_match_map[match_info.key];

	{
		// 立刻创建比赛副本
		static crossgameprotocal::CrossGameCrossMultiuserChallengeStartMatch cgcmcsm;
		cgcmcsm.match_key = match_info.key;
		
		for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; ++ i)
		{
			UNSTD_STATIC_CHECK(CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT > 0);
			CrossMultiuserChallengeUser *user = (i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT ? &(side_user_list->user_list[i]) : &(other_side_user_list->user_list[i % CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT]));

			cgcmcsm.user_info_list[i].uuid = UniqueUserIDToLongLong(user->unique_user_id);
			F_STRNCPY(cgcmcsm.user_info_list[i].user_name, user->user_name, sizeof(GameName));
			cgcmcsm.user_info_list[i].camp = user->camp;
			cgcmcsm.user_info_list[i].sex = user->sex;
			cgcmcsm.user_info_list[i].prof = user->prof;
			cgcmcsm.user_info_list[i].level = user->level;
			cgcmcsm.user_info_list[i].capability = user->capability;
			cgcmcsm.user_info_list[i].challenge_score = user->challenge_score;
			cgcmcsm.user_info_list[i].challenge_exp = user->challenge_exp;
			cgcmcsm.user_info_list[i].win_percent = user->win_percent;
			cgcmcsm.user_info_list[i].challenge_level = user->challenge_level;
			cgcmcsm.user_info_list[i].match_total_count = user->match_total_count;
			cgcmcsm.user_info_list[i].match_win_count = user->match_win_count;
			cgcmcsm.user_info_list[i].mvp_count = user->mvp_count;
		}

		CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char *)&cgcmcsm, sizeof(cgcmcsm));

		m_cross_activity_multiuser_challenge->OnMatchStart(new_match_info);
	}

	return new_match_info;
}

CrossMultiuserChallengeMatchInfo * CrossMultiuserChallengeMatchRegister::GetMatch(int key)
{
	CrossMultiuserChallengeMatchMapIt it = m_match_map.find(key);
	if (it != m_match_map.end()) return &it->second;

	return NULL;
}

CrossMultiuserChallengeMatchInfo * CrossMultiuserChallengeMatchRegister::GetMatch(const UniqueUserID &unique_user_id)
{
	UserMatchMapIt it = m_user_match_map.find(unique_user_id);
	if (it != m_user_match_map.end()) return this->GetMatch(it->second);

	return NULL;
}

void CrossMultiuserChallengeMatchRegister::RemoveMatch(int key)
{
	CrossMultiuserChallengeMatchInfo *match_info = this->GetMatch(key);
	if (NULL == match_info) return;

	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; ++ i)
	{
		m_user_match_map.erase(match_info->unique_user_id_list[i]);
	}

	m_match_map.erase(key);
}

void CrossMultiuserChallengeMatchRegister::OnMatchStatusChange(CrossMultiuserChallengeMatchInfo *match_info)
{
	switch (match_info->status)
	{
	case CROSS_MULTIUSER_CHALLENGE_MATCH_STATUS_GOING:
		{
			match_info->next_status_time = EngineAdapter::Instance().Time() + Cross3v3Config::Instance().GetFightingTime() + 100;
		}
		break;

	case CROSS_MULTIUSER_CHALLENGE_MATCH_STATUS_FINISH:
		{
			m_cross_activity_multiuser_challenge->OnMatchTimeoutFinish(match_info);
		}
		break;
	}
}

