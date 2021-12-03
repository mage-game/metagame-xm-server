#include "followbossmanager.hpp"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "protocal/msgfb.h"

FollowBossManager::FollowBossManager(): m_role(NULL)
{

}

FollowBossManager::~FollowBossManager()
{

}

void FollowBossManager::SetRole(Role *role)
{
	m_role = role;
}

void FollowBossManager::Init(Role *role, const FollowBossParam &followboss_param)
{
	m_role = role;
	m_followboss_param = followboss_param;
}

void FollowBossManager::GetInitParam(FollowBossParam *followboss_param)
{
	if (NULL != followboss_param)
	{
		*followboss_param = m_followboss_param;
	}
}

void FollowBossManager::SendFollowBossList()
{
	Protocol::SCFollowBossInfo fbi;
	fbi.count = 0;

	for (;fbi.count < Protocol::SCFollowBossInfo::MAX_BOSS_COUNT; ++fbi.count)
	{
		if (m_followboss_param.follow_boss_list[fbi.count].boss_id == 0 
			&& m_followboss_param.follow_boss_list[fbi.count].boss_type == 0 
			&& m_followboss_param.follow_boss_list[fbi.count].scene_id == 0)
		{
			break;
		}

		fbi.follow_boss_list[fbi.count].boss_id = m_followboss_param.follow_boss_list[fbi.count].boss_id;
		fbi.follow_boss_list[fbi.count].boss_type = m_followboss_param.follow_boss_list[fbi.count].boss_type;
		fbi.follow_boss_list[fbi.count].reserve_ch = 0;
		fbi.follow_boss_list[fbi.count].scene_id = m_followboss_param.follow_boss_list[fbi.count].scene_id;
	}

	int sendlen = sizeof(fbi) - sizeof(fbi.follow_boss_list[0]) * (Protocol::SCFollowBossInfo::MAX_BOSS_COUNT - fbi.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&fbi, sendlen);
}

void FollowBossManager::FollowBoss(int boss_type, int boss_id, int scene_id)
{
	int follow_boss_count = 0;
	for (; follow_boss_count < FollowBossParam::MAX_FOLLOW_BOSS_COUNT; ++follow_boss_count)
	{
		if (m_followboss_param.follow_boss_list[follow_boss_count].boss_id == boss_id 
			&& m_followboss_param.follow_boss_list[follow_boss_count].boss_type == boss_type 
			&& m_followboss_param.follow_boss_list[follow_boss_count].scene_id == scene_id)
		{
			return;
		}

		if (m_followboss_param.follow_boss_list[follow_boss_count].boss_id == 0 
			&& m_followboss_param.follow_boss_list[follow_boss_count].boss_type == 0 
			&& m_followboss_param.follow_boss_list[follow_boss_count].scene_id == 0)
		{
			break;
		}
	}

	if ((size_t)follow_boss_count < static_array_size(m_followboss_param.follow_boss_list))
	{
		m_followboss_param.follow_boss_list[follow_boss_count].boss_id = boss_id;
		m_followboss_param.follow_boss_list[follow_boss_count].boss_type = boss_type;
		m_followboss_param.follow_boss_list[follow_boss_count].scene_id = scene_id;
	}

	this->SendFollowBossList();
}

void FollowBossManager::UnFollowBoss(int boss_type, int boss_id, int scene_id)
{
	int follow_boss_count = 0;
	int dest_index = -1;
	for (; follow_boss_count < FollowBossParam::MAX_FOLLOW_BOSS_COUNT; ++follow_boss_count)
	{
		if (m_followboss_param.follow_boss_list[follow_boss_count].boss_id == boss_id 
			&& m_followboss_param.follow_boss_list[follow_boss_count].boss_type == boss_type 
			&& m_followboss_param.follow_boss_list[follow_boss_count].scene_id == scene_id)
		{
			dest_index = follow_boss_count;
		}

		if (m_followboss_param.follow_boss_list[follow_boss_count].boss_id == 0 
			&& m_followboss_param.follow_boss_list[follow_boss_count].boss_type == 0 
			&& m_followboss_param.follow_boss_list[follow_boss_count].scene_id == 0)
		{
			break;
		}
	}

	if (0 > dest_index || 1 > follow_boss_count || (size_t)dest_index >= static_array_size(m_followboss_param.follow_boss_list))
	{
		return;
	}

	follow_boss_count--;
	m_followboss_param.follow_boss_list[dest_index].boss_id = m_followboss_param.follow_boss_list[follow_boss_count].boss_id;
	m_followboss_param.follow_boss_list[dest_index].boss_type = m_followboss_param.follow_boss_list[follow_boss_count].boss_type;
	m_followboss_param.follow_boss_list[dest_index].scene_id = m_followboss_param.follow_boss_list[follow_boss_count].scene_id;

	m_followboss_param.follow_boss_list[follow_boss_count].boss_id = 0;
	m_followboss_param.follow_boss_list[follow_boss_count].boss_type = 0;
	m_followboss_param.follow_boss_list[follow_boss_count].reserve_ch = 0;
	m_followboss_param.follow_boss_list[follow_boss_count].scene_id = 0;

	this->SendFollowBossList();
}