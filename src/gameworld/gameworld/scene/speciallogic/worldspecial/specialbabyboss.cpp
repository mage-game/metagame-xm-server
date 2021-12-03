#include "specialbabyboss.hpp"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "world.h"
#include "servercommon/commondata.hpp"
#include "servercommon/noticenum.h"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/babybossconfig.hpp"
#include "obj/otherobj/fallingitem.h"
#include "global/bossfamily/bossfamilymanager.hpp"
#include "scene/generater/monstergenerater.h"
#include "servercommon/errornum.h"

SpecialBabyBoss::SpecialBabyBoss(Scene *scene) : SpecialLogic(scene)
{

}

SpecialBabyBoss::~SpecialBabyBoss()
{

}

void SpecialBabyBoss::OnRoleEnterScene(Role* role)
{
	auto kick_out_iter = m_user_logout_auto_kick_time_map.find(role->GetUID());
	if (kick_out_iter != m_user_logout_auto_kick_time_map.end())
	{
		if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) >= kick_out_iter->second)
		{
			this->DelayKickOutRole(role);
		}

		m_user_logout_auto_kick_time_map.erase(kick_out_iter);
	}

// 	auto iter = m_user_auto_add_angry_value_map.find(role->GetUID());
// 	if (iter == m_user_auto_add_angry_value_map.end())
// 	{
// 		m_user_auto_add_angry_value_map[role->GetUID()] = 0;
// 	}

	m_user_auto_add_angry_value_map[role->GetUID()] = 0;
}

void SpecialBabyBoss::OnRoleLeaveScene(Role* role, bool is_logout)
{
	if (is_logout)
	{
		unsigned int &kick_out_time = m_user_logout_auto_kick_time_map[role->GetUID()];
		kick_out_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetBabyBossConfig().GetOtherCfg().logout_kick_out_time;
	}
	else
	{
		role->GetCommonDataParam()->baby_boss_angry_value = 0;
		role->GetCommonDataParam()->baby_boss_kick_time = 0;
	}
}

void SpecialBabyBoss::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	this->CheckAutoAddAngerValue(static_cast<unsigned int>(interval), static_cast<unsigned int>(now_second));
	this->CheckKickOut(static_cast<unsigned int>(now_second));
}

void SpecialBabyBoss::OnCharacterDie(Character *dead_cha, ObjID killer_objid)
{
	if (nullptr == dead_cha) return;


	if (Obj::OBJ_TYPE_ROLE == dead_cha->GetObjType())   //»÷É±Íæ¼Ò
	{
		Role *dead_role = (Role *)dead_cha;
		this->AddAngryValue(dead_role, LOGIC_CONFIG->GetBabyBossConfig().GetOtherCfg().angry_value_die);
	}

	Obj *killer_obj = m_scene->GetObj(killer_objid);
	if (nullptr == killer_obj || Obj::OBJ_TYPE_ROLE != killer_obj->GetObjType()) return;
	Role *killer_role = (Role *)killer_obj;


	if (Obj::OBJ_TYPE_MONSTER == dead_cha->GetObjType())  //Íæ¼Ò»÷É±boss
	{
		Monster *monster = (Monster *)dead_cha;
		if (LOGIC_CONFIG->GetBabyBossConfig().IsBabyBoss(monster->GetMonsterId()))
		{
			//»÷É±¼ÇÂ¼
			Protocol::BabyBossInfo::KillerInfo killer_info;
			killer_role->GetName(killer_info.killer_name);
			killer_info.killer_uid = killer_role->GetUID();
			killer_info.killier_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			KillerInfoList &killer_list = m_boss_kill_info_map[monster->GetMonsterId()];
			killer_list.Push(killer_info);

			//bossËÀÍöÍ¨Öª
			BossFamilyManager::Instance().OnBabyMonsterDie(monster, killer_role);
			BossFamilyManager::Instance().SendBabyBossRoleInfo(killer_role);
		}
		else
		{
			this->AddAngryValueToKiller(killer_role, monster->GetMonsterId());
		}
	}
}

void SpecialBabyBoss::AddAngryValueToDropOwner(Role *role, unsigned short monster_id)
{
	if (nullptr == role)
	{
		return;
	}

	CommonDataParam *common_p = role->GetCommonDataParam();
	if (NULL == common_p) return;

	const BabyBossOtherCfg &other_cfg = LOGIC_CONFIG->GetBabyBossConfig().GetOtherCfg();
	if (common_p->baby_boss_angry_value >= other_cfg.angry_value_limit) return;

	// ÏûºÄ
	int add_value = LOGIC_CONFIG->GetBabyBossConfig().GetKillAngryValue(monster_id);
	if (add_value > 0)
	{
		this->AddAngryValue(role, add_value);
	}
}

void SpecialBabyBoss::AddAngryValueToKiller(Role *role, unsigned short monster_id)
{
	this->AddAngryValueToDropOwner(role, monster_id);
}

bool SpecialBabyBoss::GetKillerInfo(unsigned short boss_id, Protocol::BabyBossInfo::KillerInfo temp_kill_info_list[], int list_len)
{
	auto map_iter = m_boss_kill_info_map.find(boss_id);
	if (map_iter == m_boss_kill_info_map.end())
	{
		return false;
	}

	map_iter->second.GetKillerList(temp_kill_info_list, list_len);
	return true;
}

void SpecialBabyBoss::CheckKickOut(unsigned int now_sec)
{
	for (int i = 0; i < static_cast<int>(m_scene->RoleNum()); ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			CommonDataParam *common_p = role->GetCommonDataParam();
			if (0 < common_p->baby_boss_kick_time && now_sec >= common_p->baby_boss_kick_time)
			{
				Posi target_pos(0, 0);
				int target_scene_id = role->GetLastScene(&target_pos);
				ChangeSceneDelayer *timer = new ChangeSceneDelayer(World::GetInstWorld()->GetSceneManager(), m_scene->GetIndex(), role->GetId(),
					role->GetUserId(), target_scene_id, 0, target_pos, CHANGE_SCENE_REASON_INVALID);

				EngineAdapter::Instance().CreateTimer(1, timer);
				role->NoticeNum(noticenum::NT_KICK_OUT_FROM_BABY_BOSS);
			}
		}
	}
}

void SpecialBabyBoss::CheckAutoAddAngerValue(unsigned int interval, unsigned int now_sec)
{
	for (int i = 0; i < static_cast<int>(m_scene->RoleNum()); ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (nullptr != role)
		{
			auto it = m_user_auto_add_angry_value_map.find(role->GetUID());
			if (it != m_user_auto_add_angry_value_map.end())
			{
				if (0 == it->second)
				{
					it->second = now_sec + SECOND_PER_MIN;
				}

				if (now_sec >= it->second && it->second > 0)
				{
					int add_angry_value_times = 1;
					add_angry_value_times += (now_sec - it->second) / SECOND_PER_MIN;

					const BabyBossOtherCfg &other_cfg = LOGIC_CONFIG->GetBabyBossConfig().GetOtherCfg();
					int add_angry_value = other_cfg.angry_value_add_per_minute * add_angry_value_times;
					
					this->AddAngryValue(role, add_angry_value);

					it->second = now_sec + SECOND_PER_MIN;
				}
			}
		}
	}
}

void SpecialBabyBoss::AddAngryValue(Role *role, int value)
{
	if (role == nullptr) return;

	const BabyBossOtherCfg &other_cfg = LOGIC_CONFIG->GetBabyBossConfig().GetOtherCfg();
	short &angry_value = role->GetCommonDataParam()->baby_boss_angry_value;
	angry_value += value;

	if (angry_value >= other_cfg.angry_value_limit && role->GetCommonDataParam()->baby_boss_kick_time == 0)
	{
		role->GetCommonDataParam()->baby_boss_kick_time = (unsigned int)EngineAdapter::Instance().Time() + other_cfg.delay_kick_out_time;
	}

	BossFamilyManager::Instance().SendBabyBossRoleInfo(role);
}
