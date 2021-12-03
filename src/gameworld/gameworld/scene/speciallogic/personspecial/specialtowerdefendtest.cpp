#include "specialtowerdefendtest.hpp"
#include "engineadapter.h"
#include "gamecommon.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/otherobj/gatherobj.h"
#include "monster/monsterpool.h"

#include "scene/scenemanager.h"
#include "world.h"
#include "servercommon/errornum.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "protocal/msgfight.h"
#include "other/event/eventhandler.hpp"
#include "internalcomm.h"
#include "servercommon/string/gameworldstr.h"

#include "ai/monsterai/specialai/monsteraitowerdefendteam.hpp"

#include "config/logicconfigmanager.hpp"

#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/sceneeffect/sceneeffect.hpp"
#include "effect/effectbase.hpp"

#include "other/fb/towerdefendteamskill.hpp"
#include "other/fb/towerdefendteamfunc.hpp"
#include "obj/otherobj/fallingitem.h"
#include "gameworld/item/itempool.h"
#include "npc/transmit/fbmanager.hpp"
#include "effect/skilleffect/effecthpstore.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "other/capability/capability.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "gameworld/item/knapsack.h"

SpecialTowerDefendTest::SpecialTowerDefendTest(Scene *scene)
	: SpecialLogic(scene), m_is_inited(false), m_is_finish(false), m_is_pass(false), m_is_last_wave(false), m_msg_need_sync(false),
	m_curr_wave_index(-1),  m_next_refresh_one_time(0),  m_life_tower_left_hp(0), m_life_tower_left_maxhp(0), m_owner_objid(0),
	 m_get_item_count(0), m_send_drop_info_timestamp(0), m_need_refresh_monster(true)
{
	memset(m_refresh_left_count, 0, sizeof(m_refresh_left_count));
	memset(m_reward_item_list, 0, sizeof(m_reward_item_list));
}

SpecialTowerDefendTest::~SpecialTowerDefendTest()
{

}

void SpecialTowerDefendTest::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	{
		// 刚启动
		if (!m_is_inited)
		{
			m_is_inited = true;

			UInt32 monster_num = m_scene->MonsterNum();
			for (int i = 0; i < (int)monster_num; ++i)
			{
				Monster *monster = m_scene->GetMonsterByIndex(i);
				if (NULL != monster && monster->IsAlive() &&
					monster->GetMonsterId() == LOGIC_CONFIG->GetTowerDefendConfig().GetLifeTowerMonsterIDTest())
				{
					m_life_tower_left_maxhp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
					m_life_tower_left_hp = m_life_tower_left_maxhp;

					break;
				}
			}
			m_curr_wave_index = 1;
			this->SyncToClient(Protocol::SCTowerDefendInfo::NOTIFY_REASON_INIT); 
		}
	}

	{
		// 新一波开始
		if (m_next_refresh_one_time == 0)
		{
			const TowerDefendWaveCfg * wave_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetWaveTestCfgRand();
			if (NULL != wave_cfg) 
			{
				m_curr_wave_index = wave_cfg->wave;
				for (int i = 0; i < TOWERDEFEND_PATH_MAX; ++i)
				{
					m_refresh_left_count[i] = wave_cfg->monster_count_list[i];
				}

				this->RefreshOne(now_second);
			}

			this->SyncToClient(Protocol::SCTowerDefendInfo::NOTIFY_REASON_NEW_WAVE_START); 
		}
	}

	{
		// 按频率在各路线出怪
		if (static_cast<unsigned int>(now_second) > m_next_refresh_one_time &&
			(m_scene->MonsterNum() < TOWERDEFEND_TEST_MONSTER_MINE || m_need_refresh_monster))
		{
			this->RefreshOne(now_second);
			m_need_refresh_monster = true;
		}

		if (m_scene->MonsterNum() > TOWERDEFEND_TEST_MONSTER_MAX)
		{
			m_need_refresh_monster = false;
		}
	}

	{
		if (m_msg_need_sync)
		{
			this->SyncToClient();
		}
	}
}

void SpecialTowerDefendTest::OnRoleEnterScene(Role *role)
{
	if (role == NULL) return;

	m_scene->GoToTownPoint(role);
	m_owner_user_id = role->GetUserId();
	m_owner_objid = role->GetId();

	this->SyncToClient();
}

void SpecialTowerDefendTest::OnRoleLeaveScene(Role *role, bool is_logout)
{

}

bool SpecialTowerDefendTest::CanDestroy()
{
	return m_scene->RoleNum() == 0;
}

void SpecialTowerDefendTest::OnRemoveObj(Obj *obj)
{

}

void SpecialTowerDefendTest::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead) return;

	if (Obj::OBJ_TYPE_ROLE == dead->GetObjType())
	{
		Role *death_role = (Role*)dead;

		if (death_role->GetUserId() == m_owner_user_id)
		{
			this->SyncToClient();
		}
	}
}

void SpecialTowerDefendTest::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (NULL == cha) return;

	if (Obj::OBJ_TYPE_MONSTER == cha->GetObjType())
	{
		Monster *monster = (Monster*)cha;

		if (monster->GetMonsterId() == LOGIC_CONFIG->GetTowerDefendConfig().GetLifeTowerMonsterIDTest() && monster->IsAlive())
		{
			
			long long now_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			long long max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			long long old_hp = now_hp + (-delta_hp);

			m_life_tower_left_hp = now_hp;

			int old_percent = static_cast<int>(max_hp > 0 ? old_hp * 100 / max_hp : 0); //血量百分比
			int now_percent = static_cast<int>(max_hp > 0 ? now_hp * 100 / max_hp : 0);

			if (old_percent > now_percent)
			{
				int warning_percent = 0;
				if (LOGIC_CONFIG->GetTowerDefendConfig().CheckWarning(old_percent, now_percent, &warning_percent))
				{
					this->NotifyWarning(Protocol::SCTowerDefendWarning::WARNING_TYPE_LOW_PERCENT, warning_percent);

					monster->Recover();
					m_life_tower_left_hp = max_hp;
				}
			}

			m_msg_need_sync = true;
		}
	}
}

bool SpecialTowerDefendTest::OnTimeout()
{
	if (!m_is_finish)
	{
		m_is_finish = true;

		this->OnFinish(false);
	}

	return m_is_pass;
}

void SpecialTowerDefendTest::OnLeaveFB(Role *role)
{
	if (!m_is_finish)
	{
		this->OnFinish(false);
	}

	// role->GetSpecialAppearance()->SetFbAppearance(SPECIAL_APPEARANCE_TYPE_NORMAL, 0, false);
	role->GetMountManager()->GoonMount(true, false, true);
}

bool SpecialTowerDefendTest::SpecialRelive(Role *role, int realive_type)
{
	const TowerDefendOtherCfg & other_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetOtherCfg();
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		if (role->GetDeadTime() < other_cfg.relive_back_home_interval)
		{
			return true;
		}
		Posi realive_pos;
		int target_scene_id = m_scene->GetTownPoint(role, &realive_pos);
		if (target_scene_id == m_scene->GetSceneID())
		{
			if (role->ReAlive(50, 100, realive_pos)) return true;
		}
	}
	else if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_HERE_GOLD == realive_type)
	{
		if (role->GetDeadTime() < other_cfg.relive_gold_interval)
		{
			return true;
		}
	}
	
	return false;
}

bool SpecialTowerDefendTest::SpecialIsEnemy(Role *role, Obj *obj)
{
	switch (obj->GetObjType())	
	{
	case Obj::OBJ_TYPE_ROLE:
		{
			return false;
		}
		break;

	case Obj::OBJ_TYPE_MONSTER:
		{
			Monster *monster = (Monster*)obj;
			return monster->IsAlive() && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType();
		}
		break;
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

bool SpecialTowerDefendTest::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
{
	switch (obj->GetObjType())	
	{
	case Obj::OBJ_TYPE_ROLE:
		{
			Role *friend_role = (Role*)obj;
			return (!check_alive || friend_role->IsAlive());
		}
		break;

	case Obj::OBJ_TYPE_MONSTER:
		{
			Monster *monster = (Monster*)obj;
			return (!check_alive || monster->IsAlive()) && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType();
		}
		break;
	}

	return SpecialLogic::SpecialIsFriend(role, obj, check_alive);
}

bool SpecialTowerDefendTest::SpecialCanReAlive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		return role->GetDeadTime() >= TOWERDEFEND_HOME_REALIVE_LIMIT_INTERVAL;
	}

	return role->GetDeadTime() >= TOWERDEFEND_GOLD_REALIVE_LIMIT_INTERVAL;
}

void SpecialTowerDefendTest::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{

}

void SpecialTowerDefendTest::OnNextWaveReq(Role *role)
{
	if (NULL == role) return;

	for (int i = 0; i < TOWERDEFEND_PATH_MAX; ++i)
	{
		if (m_refresh_left_count[i] > 0)
		{
			role->NoticeNum(errornum::EN_TOWER_DEFEND_CURR_WAVE_NOT_DONE);
			return;
		}
	}

	int monster_num = m_scene->MonsterNum();
	for (int i = 0; i < monster_num; ++i)
	{
		Monster *monster = m_scene->GetMonsterByIndex(i);
		if (monster != NULL && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType() && monster->IsAlive())
		{
			role->NoticeNum(errornum::EN_FB_REST_MONSTER_NUM_LIMIT);
			return;
		}
	}

	//unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());


	this->SyncToClient();

	role->NoticeNum(noticenum::NT_TOWERDEFEND_NEXT_WAVE_SUCC);
	
}

Role* SpecialTowerDefendTest::GetOwnerRole()
{
	Obj *obj = m_scene->GetObj(m_owner_objid);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		if (m_owner_user_id == role->GetUserId()) 
		{
			return role;
		}
	}

	return NULL;
}

void SpecialTowerDefendTest::RefreshOne(time_t now_second)
{
	bool is_left = false;

	const TowerDefendWaveCfg * wave_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetWaveTestCfgByWave(m_curr_wave_index);
	if (NULL == wave_cfg)
	{
		m_next_refresh_one_time = 0;
		return;
	}

	for (int path = TOWERDEFEND_PATH_LEFT; path < TOWERDEFEND_PATH_MAX; ++path)
	{
		if (m_refresh_left_count[path] > 0)
		{
			Posi birth_pos = LOGIC_CONFIG->GetTowerDefendConfig().GetBirthPosTest(path);

			// 未达到推荐战力时
			Attribute add_hp = 0, add_gongji = 0;
			Role *role = this->GetOwnerRole();
			const TowerDefendLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetLevelCfgBySceneID(m_scene->GetSceneID());
			if (nullptr != lvl_cfg)
			{
				if (nullptr != role && role->GetCapability()->GetTotalCapability() < lvl_cfg->capability)
				{
					const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(wave_cfg->monster_id_list[path]);
					if (NULL != monster_param)
					{
						const TowerDefendOtherCfg &other_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetOtherCfg();
						add_hp = (Attribute)(monster_param->hp * (other_cfg.add_hp_per * SKILL_ATTR_PER));
						add_gongji = (Attribute)(monster_param->gongji * (other_cfg.add_gongji_per * SKILL_ATTR_PER));
					}
				}
			}
			
			Monster *monster = MONSTERPOOL->CreateMonster(wave_cfg->monster_id_list[path], m_scene, gamecommon::GetDisRandPos(birth_pos, 4), add_hp, add_gongji);
			if (NULL != monster)
			{
				MonsterAIBase *aibase = monster->GetMonsterAI();
				if (NULL != aibase && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == aibase->GetAIType())
				{
					MonsterAITowerDefendTeam *aiattack = (MonsterAITowerDefendTeam *)aibase;
					aiattack->SetBirthPath(path);
				}

				-- m_refresh_left_count[path];
			}
			//printf("------refresh wave:%d ---path:%d,left:%d\n",m_curr_wave_index, path, m_refresh_left_count[path]);
			if (m_refresh_left_count[path] > 0) is_left = true;
		}
	}

	if (is_left)
	{
		m_next_refresh_one_time = static_cast<unsigned int>(now_second) + LOGIC_CONFIG->GetTowerDefendConfig().GetOtherCfg().refresh_interval_test;
	}
	else
	{
		m_next_refresh_one_time = 0;
	}
}

void SpecialTowerDefendTest::CheckAttackMonsterStatus(unsigned long interval, time_t now_second)
{
	if (now_second >= m_last_check_monster_status_time + 1)
	{
		m_last_check_monster_status_time = static_cast<unsigned int>(now_second);

		for (int i = 0; i < TOWERDEFEND_PATH_MAX; ++i)
		{
			if (m_refresh_left_count[i] > 0) return; // 怪都没出完呢
		}

		UInt32 monster_num = m_scene->MonsterNum();
		for (int i = 0; i < (int)monster_num; ++i)
		{
			Monster *monster = m_scene->GetMonsterByIndex(i);
			if (NULL != monster && monster->IsAlive() && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType())
			{
				return;
			}
		}

		this->OnAllAttackMonsterDead();
	}
}

void SpecialTowerDefendTest::OnAllAttackMonsterDead()
{
	if (!m_is_finish && m_is_last_wave)
	{
		m_is_finish = true;

		this->OnFinish(true);
	}
}

void SpecialTowerDefendTest::OnFinish(bool is_pass)
{
	
}

bool SpecialTowerDefendTest::OnNewPlayerLevelFinish()
{
	return true;
}

void SpecialTowerDefendTest::SyncToClient(int reason)
{
	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		Protocol::SCTowerDefendInfo ttdi;
		ttdi.reason = static_cast<short>(reason);
		ttdi.time_out_stamp = 0;
		ttdi.is_finish = m_is_finish ? 1 : 0;
		ttdi.is_pass = m_is_pass ? 1: 0;
		ttdi.pass_time_s = 0;
		ttdi.life_tower_left_hp = m_life_tower_left_hp;
		ttdi.life_tower_left_maxhp = m_life_tower_left_maxhp;
		ttdi.curr_wave = m_curr_wave_index;
		ttdi.curr_level = LOGIC_CONFIG->GetTowerDefendConfig().GetLevelBySceneID(m_scene->GetSceneID());
		ttdi.next_wave_refresh_time = 0;
		ttdi.clear_wave_count = 0;
		ttdi.death_count = 0;
		if (m_is_pass)
		{
			ttdi.get_coin = 0;
			ttdi.get_item_count = 0;
			memset(ttdi.drop_items, 0, sizeof(Protocol::SCTowerDefendInfo::DropItem) * FB_RECORD_DROP_ITEM_MAX_NUM);
			for (unsigned int i = 0; i < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM && ttdi.get_item_count< FB_RECORD_DROP_ITEM_MAX_NUM; i++)
			{
				if (m_reward_item_list[i].item_id > 0 && m_reward_item_list[i].num > 0 )
				{
					ttdi.drop_items[ttdi.get_item_count].item_id = m_reward_item_list[i].item_id;
					ttdi.drop_items[ttdi.get_item_count].num = m_reward_item_list[i].num;
					++ttdi.get_item_count;
				}
			}
		}
		else
		{
			ttdi.get_coin = 0;
			ttdi.get_item_count = 0;
		}

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&ttdi, sizeof(ttdi));

	}

	m_msg_need_sync = false;
}

void SpecialTowerDefendTest::NotifyWarning(short warning_type, short percent)
{
	Protocol::SCTowerDefendWarning ttdw;
	ttdw.warning_type = warning_type;
	ttdw.percent = percent;

	m_scene->SendToRole((const char *)&ttdw, sizeof(ttdw));
}




