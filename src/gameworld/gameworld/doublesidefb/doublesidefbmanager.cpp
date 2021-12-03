#include "world.h"
#include "doublesidefbmanager.hpp"
#include "servercommon/errornum.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "scene/scene.h"

#include "other/fb/nightfight/nightfightfbconfig.hpp"
#include "other/fb/messbattle/messbattleconfig.hpp"

DoubleSideFBManager::DoubleSideFBManager()
{
}

DoubleSideFBManager::~DoubleSideFBManager()
{
}

void DoubleSideFBManager::RequireEnterFb(Role * role, int activity_type, bool is_cross, int &scene_key)
{
	if(NULL == role && !is_cross)
	{
		return;
	}

	if(!is_cross && (activity_type <= ACTIVITY_TYPE_INVALID || activity_type >= ACTIVITY_TYPE_MAX))
	{
		return;
	}

	if(!role->GetRoleStatusManager()->CanEnterFB())
	{
		return;
	}

	scene_key = 0;

	// 获取限制人数，如果没有限制，则可以放置int类型最大值的人数
	int room_member_limit_num = INT_MAX;

	switch (activity_type)
	{
		case ACTIVITY_TYPE_NIGHT_FIGHT_FB:
		case CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB:
			{
				scene_key = Scene::SCENE_TYPE_NIGHT_FIGHT_FB;

				room_member_limit_num = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg().room_member_limit;
			}
			break;

		case ACTIVITY_TYPE_MESS_BATTLE_FB:
		case CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB:
		{
			scene_key = Scene::SCENE_TYPE_MESS_BATTLE_FB;

			room_member_limit_num = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg().room_member_limit;
		}
		break;

		default:
			break;
	}

	// 放置人员
	ActivityRoomMemberNumIt room_member_it = m_activity_room_member_num_map.find(activity_type);
	if (room_member_it == m_activity_room_member_num_map.end())
	{
		int add_room = 1;
		std::vector<int> room;
		room.push_back(add_room);
		m_activity_room_member_num_map[activity_type] = room;

		long long role_key = this->GetRoleKey(role);
		if (role_key > 0)
		{
			m_role_seat_map[this->GetRoleKey(role)] = scene_key;
		}

		// 根据天数传送玩家进入对应场景
		this->SendRoleToScene(role, activity_type, is_cross, scene_key);
	}
	else
	{
		std::vector<int>::iterator room = room_member_it->second.begin();
		for(; room != room_member_it->second.end(); ++room)
		{
			// 如果房间没有满人
			if(*room < room_member_limit_num)
			{
				// 该房间人数+1
				(*room)++;

				long long role_key = this->GetRoleKey(role);
				if (role_key > 0)
				{
					m_role_seat_map[this->GetRoleKey(role)] = scene_key;
				}

				// 根据天数传送玩家进入对应场景
				this->SendRoleToScene(role, activity_type, is_cross, scene_key);
				break;
			}

			scene_key++;
		}
		
		// 如果房间满人，则新开房间
		if(room == room_member_it->second.end())
		{
			int add_room = 1;
			room_member_it->second.push_back(add_room);

			long long role_key = this->GetRoleKey(role);
			if (role_key > 0)
			{
				m_role_seat_map[this->GetRoleKey(role)] = scene_key;
			}

			// 根据天数传送玩家进入对应场景
			this->SendRoleToScene(role, activity_type, is_cross, scene_key);
		}
	}
}

void DoubleSideFBManager::RequireLeaveFb(Role * role, int activity_type, bool is_cross)
{
	if (NULL == role && !is_cross)
	{
		return;
	}

	long long role_key = this->GetRoleKey(role);

	if (role_key > 0)
	{
		RoleSeatMapIt it = m_role_seat_map.find(role_key);
		ActivityRoomMemberNumIt room_member_it = m_activity_room_member_num_map.find(activity_type);

		if(it != m_role_seat_map.end() && room_member_it!= m_activity_room_member_num_map.end())
		{
			switch (activity_type)
			{
				case ACTIVITY_TYPE_NIGHT_FIGHT_FB:
				case CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB:
				{
					--room_member_it->second[it->second - Scene::SCENE_TYPE_NIGHT_FIGHT_FB];

					m_role_seat_map.erase(it);
				}
				break;

				case ACTIVITY_TYPE_MESS_BATTLE_FB:
				case CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB:
				{
					--room_member_it->second[it->second - Scene::SCENE_TYPE_MESS_BATTLE_FB];

					m_role_seat_map.erase(it);
				}
				break;

				default:
					break;
			}
		}
	}
}

int DoubleSideFBManager::GetRoleFbKey(Role * role)
{
	int scene_key = 1;

	if (nullptr == role)
	{
		return scene_key;
	}
	
	auto it = m_role_seat_map.find(this->GetRoleKey(role));
	if (it != m_role_seat_map.end())
	{
		scene_key = it->second;
	}

	return scene_key;
}

long long DoubleSideFBManager::GetRoleKey(Role * role)
{
	if(!CrossConfig::Instance().IsHiddenServer())
	{
		return static_cast<long long>(role->GetUID());
	}
	else
	{
		return UniqueUserIDToLongLong(role->GetUniqueUserID());
	}
	
	return -1;
}

void DoubleSideFBManager::SendRoleToScene(Role * role, int activity_type, bool is_cross, int secen_key)
{
	if (NULL == role && !is_cross)
	{
		return;
	}

	int scene_id = 0;

	switch (activity_type)
	{
		case ACTIVITY_TYPE_NIGHT_FIGHT_FB:
		case CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB:
		{
			const NightFightFbOtherConfig other_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg();
			scene_id = other_cfg.scene_id;

			SpecialNightFight *special_scene = World::GetInstWorld()->GetSceneManager()->GetSpecialNightFight(scene_id, secen_key, true);
			if (NULL == special_scene)
			{
				return;
			}

			if (!is_cross)
			{
				World::GetInstWorld()->GetSceneManager()->GoTo(role, scene_id, secen_key, gamecommon::GetDisRandPos(LOGIC_CONFIG->GetNightFightFbConfig().GetRelievePos(), 5));
			}
		}
		break;

		case ACTIVITY_TYPE_MESS_BATTLE_FB:
		case CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB:
		{
			const MessBattleFbOtherConfig other_cfg = LOGIC_CONFIG->GetMessBattleFbConfig().GetOtherCfg();
			scene_id = other_cfg.scene_id;

			SpecialMessBattle *special_scene = World::GetInstWorld()->GetSceneManager()->GetSpecialMessBattle(scene_id, secen_key, true);
			if (NULL == special_scene)
			{
				return;
			}

			if (!is_cross)
			{
				World::GetInstWorld()->GetSceneManager()->GoTo(role, scene_id, secen_key, gamecommon::GetDisRandPos(LOGIC_CONFIG->GetMessBattleFbConfig().GetRelievePos(), 5));
			}
		}
		break;

		default:
			break;
	}
}
