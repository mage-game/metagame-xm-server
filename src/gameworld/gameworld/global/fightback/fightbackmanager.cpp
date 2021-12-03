
#include "fightbackmanager.hpp"

#include "obj/character/role.h"
#include "protocal/msgrole.h"
#include "scene/scenemanager.h"
#include "world.h"


FightBackManager::FightBackManager() : m_next_check_timestamp(0)
{
}

FightBackManager::~FightBackManager()
{
}


FightBackManager & FightBackManager::Instance()
{
	static FightBackManager fbm;
	return fbm;
}

void FightBackManager::Update(unsigned long interval, time_t now_second)
{
	if (m_fight_back_role_map.empty()) return;

	static const int CHECK_INTERVAL_S = 5;  // 检测间隔
	static const int DEL_ROLE_TIME_S = 60;  // 反击对象存在时间

	if (now_second >= m_next_check_timestamp)
	{
		for (auto role_it = m_fight_back_role_map.begin(); role_it != m_fight_back_role_map.end();)
		{
			int uid = role_it->first;
			AttackRoleMap &attack_role_map = role_it->second;

			bool is_modify = false; std::set<int> del_list;
			for (AttackRoleMapIt list_it = attack_role_map.begin(); list_it != attack_role_map.end();)
			{
				if ((unsigned int)now_second >= list_it->second + DEL_ROLE_TIME_S)
				{
					// 从攻击者攻击列表缓存中删除uid
					//auto attack_it = m_role_attack_list.find(list_it->first);
					//if (m_role_attack_list.end() != attack_it)
					//{
					//	attack_it->second.erase(uid);
					//}

					// 删除反击对象
					del_list.insert(list_it->first);
					attack_role_map.erase(list_it++);
					is_modify = true;
				}
				else
				{
					++list_it;
				}
			}

			// 下发删除后的列表给客户端
			if (is_modify)
			{
				Role *tmp_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
				if (nullptr != tmp_role)
				{
					this->SendAddOrDelRoleList(tmp_role, del_list, Protocol::SCFightBackRoleList::NOTIFY_LIST_DEL);  // 通知角色
				}
			}

			// 如果角色的反击对象列表为空就删除
			if (0 == attack_role_map.size())
			{
				m_fight_back_role_map.erase(role_it++);
			}
			else
			{
				++role_it;
			}
		}

		m_next_check_timestamp = now_second + CHECK_INTERVAL_S;  // 5秒检测
	}
}

void FightBackManager::OnRoleDie(Role *role)
{
	return;  // 屏蔽
	if (nullptr == role)
	{
		return;
	}

	int attacker_uid = role->GetUID();

	// 攻击者死亡，则查找所有角色的反击对象表，遍历删除这个攻击者
	auto attack_it = m_role_attack_list.find(attacker_uid);
	if (m_role_attack_list.end() == attack_it) return;

	for (auto role_uid : attack_it->second)
	{
		auto role_it = m_fight_back_role_map.find(role_uid);
		if (m_fight_back_role_map.end() == role_it) continue;

		AttackRoleMap &attack_role_map = role_it->second;

		if (attack_role_map.end() != attack_role_map.find(attacker_uid))
		{
			attack_role_map.erase(attacker_uid);

			Role *tmp_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role_uid));
			if (nullptr != tmp_role)
			{
				std::set<int> del_list; del_list.insert(attacker_uid);
				this->SendAddOrDelRoleList(tmp_role, del_list, Protocol::SCFightBackRoleList::NOTIFY_LIST_DEL);  // 通知角色
			}
		}
	}

	m_role_attack_list.erase(attacker_uid);
}

bool FightBackManager::IsFightBackObj(Role * role, Role *target_role)
{
	if (nullptr == role || nullptr == target_role)
	{
		return false;
	}

	int uid = role->GetUID();

	if (uid == target_role->GetUID())
	{
		return false;
	}

	auto role_it = m_fight_back_role_map.find(uid);
	if (m_fight_back_role_map.end() == role_it)
	{
		return false;
	}

	AttackRoleMap &attack_role_map = role_it->second;

	return (attack_role_map.end() != attack_role_map.find(target_role->GetUID()));
}

void FightBackManager::AddFightBackRole(Role *role, Role *attacker_role)
{
	if (nullptr == role || nullptr == attacker_role)
	{
		return;
	}

	int uid = role->GetUID();
	int attack_uid = attacker_role->GetUID();
	AttackRoleMap &attack_role_map = m_fight_back_role_map[uid];

	// 加入反击列表
	auto list_it = attack_role_map.find(attack_uid);
	if (attack_role_map.end() == list_it)
	{
		if (attack_role_map.size() >= Protocol::SCFightBackRoleList::MAX_ROLE_LIST_NUM)  // 最多50个应该也够了
		{
			// 删除最早的
			unsigned int earliest_time = 0; int earliest_uid = 0;
			for (auto tmp_it : attack_role_map)
			{
				if (0 == earliest_time || tmp_it.second < earliest_time)
				{
					earliest_time = tmp_it.second;
					earliest_uid = tmp_it.first;
				}
			}

			if (earliest_uid > 0)
			{
				attack_role_map.erase(earliest_uid);

				std::set<int> del_list; del_list.insert(earliest_uid);
				this->SendAddOrDelRoleList(role, del_list, Protocol::SCFightBackRoleList::NOTIFY_LIST_DEL);  // 通知角色
			}
		}

		attack_role_map[attack_uid] = (unsigned int)EngineAdapter::Instance().Time();

		std::set<int> add_list; add_list.insert(attack_uid);
		this->SendAddOrDelRoleList(role, add_list, Protocol::SCFightBackRoleList::NOTIFY_LIST_ADD);
	}
	else
	{
		list_it->second = (unsigned int)EngineAdapter::Instance().Time();
	}
	
	// 攻击者缓存,活哥说不要死亡直接删除，仍然保留一分钟
	//m_role_attack_list[attack_uid].insert(uid);

}

void FightBackManager::SendRoleList(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	static Protocol::SCFightBackRoleList fbrl;
	fbrl.notify = Protocol::SCFightBackRoleList::NOTIFY_LIST_ADD;
	fbrl.count = 0;

	auto role_it = m_fight_back_role_map.find(role->GetUID());
	if (m_fight_back_role_map.end() != role_it)
	{
		AttackRoleMap &attack_role_map = role_it->second;

		for (auto list_it = attack_role_map.begin(); list_it != attack_role_map.end(); ++list_it)
		{
			if (list_it->first > 0)
			{
				fbrl.role_uid_list[fbrl.count] = list_it->first;
				fbrl.count++;
				if (fbrl.count >= Protocol::SCFightBackRoleList::MAX_ROLE_LIST_NUM)
				{
					break;
				}
			}
		}
	}

	int send_len = sizeof(fbrl) - sizeof(fbrl.role_uid_list[0]) * (Protocol::SCFightBackRoleList::MAX_ROLE_LIST_NUM - fbrl.count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fbrl, send_len);
}

void FightBackManager::SendAddOrDelRoleList(Role *role, std::set<int> &role_list, int notify)
{
	if (nullptr == role)
	{
		return;
	}

	static Protocol::SCFightBackRoleList fbrl;
	fbrl.notify = notify;
	fbrl.count = 0;

	for (auto list_it = role_list.begin(); list_it != role_list.end(); ++list_it)
	{
		fbrl.role_uid_list[fbrl.count] = (*list_it);
		fbrl.count++;
		if (fbrl.count >= Protocol::SCFightBackRoleList::MAX_ROLE_LIST_NUM)
		{
			break;
		}
	}

	int send_len = sizeof(fbrl) - sizeof(fbrl.role_uid_list[0]) * (Protocol::SCFightBackRoleList::MAX_ROLE_LIST_NUM - fbrl.count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fbrl, send_len);
}
