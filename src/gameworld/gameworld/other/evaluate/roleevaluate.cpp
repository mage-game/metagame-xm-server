#include "roleevaluate.hpp"

#include "global/offlineregister/offlineregistermanager.hpp"
#include "internalcomm.h"
#include "engineadapter.h"
#include "servercommon/errornum.h"
#include "world.h"
#include "obj/character/role.h"
#include "global/usercache/usercache.hpp"
#include "global/rank/rankmanager.hpp"
#include "servercommon/noticenum.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "protocal/msgfriend.h"
#include "friend/friendmanager.h"
#include "other/daycounter/daycounter.hpp"

RoleEvaluate::RoleEvaluate() : m_role(NULL)
{
	memset(m_uid_list, 0, sizeof(m_uid_list));
}

RoleEvaluate::~RoleEvaluate()
{

}

void RoleEvaluate::Init(Role *role, const EvaluateParam &param)
{
	m_role = role;
	memcpy(m_uid_list, param.uid_list, sizeof(m_uid_list));
}

void RoleEvaluate::GetEvaluateParam(EvaluateParam *param)
{
	memcpy(param->uid_list, m_uid_list, sizeof(param->uid_list));
}

void RoleEvaluate::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		memset(m_uid_list, 0, sizeof(m_uid_list));
	}
}

void RoleEvaluate::OnEvaluateRole(int target_uid)
{
	if (UidToInt(m_role->GetUserId()) == target_uid)
	{
		m_role->NoticeNum(errornum::EN_OTHER_CANNOT_EVALUATE_SELF);
		return;
	}

	if (NULL == UserCacheManager::Instance().GetUserNode(target_uid))
	{
		m_role->NoticeNum(errornum::EN_ROLE_NOT_IN_CACHE);
		return;
	}

	UserID target_user_id = IntToUid(target_uid);

	int evaluate_count = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_EVALUATE);
	
	if (m_role->GetLevel() < EVALUSTE_NEED_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_OTHER_EVALUATE_LEVEL_LIMIT);
		return;
	}

	if (evaluate_count < 0 || evaluate_count >= MAX_EVALUSTE_NUM)
	{
		m_role->NoticeNum(errornum::EN_OTHER_NO_EVALUATE_NUM);
		return;
	}

	for (int i = 0; i < evaluate_count; ++i)
	{
		if (m_uid_list[i] == target_uid)
		{
			m_role->NoticeNum(errornum::EN_OTHER_ROLE_EVALUATEED);
			return;
		}
	}

	m_uid_list[evaluate_count] = target_uid;
	m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_EVALUATE);
	//UserID user_id = m_role->GetUserId();

	const static int ADMIRE_ADD_CHARM = 1;

	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(target_user_id);
	if (NULL != target_role)
	{
		target_role->GetFriendManager()->AddAllCharm(ADMIRE_ADD_CHARM);
		
		{
			Protocol::SCAllCharmChange acc;
			acc.uid = target_role->GetUID();
			acc.all_charm = target_role->GetFriendManager()->GetAllCharm();
			acc.day_charm = target_role->GetFriendManager()->GetDayAddCharm();
			EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&acc, sizeof(acc));
		}
	}
	else
	{
		OfflineRegisterManager::Instance().OnEvaluateRole(target_user_id, ADMIRE_ADD_CHARM);
	}

	m_role->NoticeNum(noticenum::NT_PRAISE_SUCC);
}

