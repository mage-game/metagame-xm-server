#include "specialcoinfb.hpp"

#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "engineadapter.h"
#include "gamecommon.h"
#include "protocal/msgfight.h"
#include "obj/otherobj/bigcoinobj.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "other/event/eventhandler.hpp"
#include "globalconfig/globalconfig.h"
#include "protocal/msgfb.h"
#include "obj/otherobj/fallingitem.h"
#include "gameworld/item/itempool.h"
#include "gameworld/other/fb/dailyfbconfig.hpp"
#include "ai/monsterai/specialai/monsteraicoin.hpp"
#include "other/fb/roledailyfb.hpp"
#include <vector>

SpecialCoinFB::SpecialCoinFB(Scene *scene)
: SpecialLogic(scene), m_dailyfb_type(0), m_max_wave(0), m_is_first(true), m_is_finish(false), m_is_pass(false),
m_get_exp(0), m_get_coin(0), m_wave(0), m_next_wave_start_time(0), m_create_monster_count(0), m_create_monster_id(0), m_next_create_time(0), m_owner_objid(INVALID_OBJ_ID), m_owner_user_id(INVALID_USER_ID)
{

}

SpecialCoinFB::~SpecialCoinFB()
{

}

void SpecialCoinFB::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_is_first)
	{
		m_dailyfb_type = LOGIC_CONFIG->GetDailyFbCfg().GetDailyFbTypeBySceneId(m_scene->GetSceneID());
		m_max_wave = LOGIC_CONFIG->GetDailyFbCfg().GetCoinMaxWave();
		m_is_first = false;
	}

	if (m_is_finish) return;

	if (m_wave < m_max_wave && m_next_wave_start_time >= 0 && now_second >= m_next_wave_start_time && m_create_monster_count <= 0)
	{
		if (this->StartCreateWaveMonsters(m_wave + 1))
		{
			++ m_wave;

			const CoinFbWaveItem * wave_item = LOGIC_CONFIG->GetDailyFbCfg().GetCoinFbWaveItemCfg(m_wave);
			if (NULL != wave_item)
			{
				m_next_wave_start_time = static_cast<unsigned int>(now_second) + wave_item->next_wave_time_gap;
			}
			else
			{
				m_next_wave_start_time = -1;
			}
			
			this->SyncFBSceneLogicInfo(false);
		}
	}

	//怪一个个创建
	if (m_wave <= m_max_wave && m_create_monster_count > 0 && m_create_monster_id > 0 && m_next_create_time >= 0 && now_second >= m_next_create_time)
	{
		m_next_create_time = static_cast<unsigned int>(now_second) + 2;
		this->CreateOneMonster(m_create_monster_id);
	}

	this->CheckSceneMonsterStatus(interval, now_second);
};

bool SpecialCoinFB::StartCreateWaveMonsters(short wave)
{
	if (m_dailyfb_type < 0)
	{
		return false;
	}

	Role *role = this->GetOwnerRole();
	if (NULL == role)
	{
		return false;
	}

	if (wave > m_max_wave)
	{
		return false;
	}

	const CoinFbWaveItem *wave_item = LOGIC_CONFIG->GetDailyFbCfg().GetCoinFbWaveItemCfg(wave);
	if (NULL == wave_item)
	{
		return false;
	}
	
	m_create_monster_id = wave_item->monster_id;
	m_create_monster_count = wave_item->monster_count;

	return true;
}

void SpecialCoinFB::CreateOneMonster(int monster_id)
{
	if (monster_id <= 0 || m_create_monster_count <= 0) return;

	std::vector<Posi> &path = LOGIC_CONFIG->GetDailyFbCfg().GetCoinPath();
	if (static_cast<int>(path.size()) <= 0)
	{
		return;
	}

	Monster *monster = MONSTERPOOL->CreateMonster(monster_id, m_scene, gamecommon::GetDisRandPos(path[0], 4));
	if (NULL != monster)
	{
		MonsterAIBase *aibase = monster->GetMonsterAI();
		if (NULL != aibase && MonsterInitParam::AITYPE_COIN == aibase->GetAIType())
		{
			MonsterAICoin *aiattack = (MonsterAICoin *)aibase;
			aiattack->SetMovePath(path);
		}
	}

	-- m_create_monster_count;
	if (m_create_monster_count <= 0)
	{
		m_create_monster_id = 0;
	}
}

void SpecialCoinFB::OnRoleEnterScene(Role *role)
{
	if (m_is_finish)
	{
		this->DelayKickOutRole(role, CHANGE_SCENE_REASON_FB_FINISH);
	}
	else
	{
		m_owner_user_id = role->GetUserId(); 
		m_owner_objid = role->GetId();
		this->SyncFBSceneLogicInfo();

		EventHandler::Instance().OnEnterFBCoin(role);
	}
}

void SpecialCoinFB::OnLeaveFB(Role *role)
{
	if (!m_is_finish)
	{
		if (role->GetUserId() == m_owner_user_id)
		{
			this->OnCoinFBFinish(false, true);
		}
	}
}

void SpecialCoinFB::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead)
	{
		return;
	}

	if (killer_id == m_owner_objid && Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = (Monster *)dead;
		long long get_exp = 0;
		int get_coin = 0;
		MONSTERPOOL->GetMonsterDrop(monster->GetMonsterId(), NULL, NULL, &get_exp, &get_coin, NULL);
		m_get_exp += get_exp;
		m_get_coin += get_coin;

		this->SyncFBSceneLogicInfo();
	}
}

bool SpecialCoinFB::OnTimeout()
{
	if (!m_is_finish)
	{
		this->OnCoinFBFinish(false);
	}

	return m_is_pass;
}

bool SpecialCoinFB::SpecialRelive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		Posi realive_pos;
		int target_scene_id = m_scene->GetTownPoint(role, &realive_pos);
		if (target_scene_id == m_scene->GetSceneID())
		{
			if (role->ReAlive(10, 10, realive_pos)) return true;
		}
	}

	return false;
}

Role* SpecialCoinFB::GetOwnerRole()
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

void SpecialCoinFB::OnAllMonsterDead()
{
	if (m_is_finish) return;

	if (0 == m_create_monster_count) //因为怪是一个个出的，必须全出完才考虑
	{
		int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());  //全部打完缩减时间
		if(m_next_wave_start_time - now_time > 10)
		{
			m_next_wave_start_time = now_time + 10;
			this->SyncFBSceneLogicInfo();
		}
	}

	if (0 == m_create_monster_count && m_wave >= m_max_wave)
	{
		this->OnCoinFBFinish(true, false);
	}
}

void SpecialCoinFB::OnCoinFBFinish(bool is_pass, bool is_active_leave_fb)
{
	if (m_is_finish) return;

	m_is_pass = is_pass;
	m_is_finish = true;
	this->KillAllMonster();

	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		role->GetRoleDailyFb()->OnDailyFbFinish(m_dailyfb_type, this->GetPassTime(), m_is_pass);
		this->SyncFBSceneLogicInfo(is_active_leave_fb);
	}
}

void SpecialCoinFB::SyncFBSceneLogicInfo(bool is_active_leave_fb)
{
	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		static Protocol::SCFBSceneLogicInfo cmd; 
		cmd.reset();

		cmd.time_out_stamp = m_scene->GetSceneTimeOutStamp();
		cmd.scene_type = this->GetSceneType();

		cmd.is_finish = m_is_finish ? 1 : 0;
		cmd.is_pass = m_is_pass ? 1 : 0;
		cmd.is_active_leave_fb = is_active_leave_fb ? 1 : 0;

		cmd.pass_time_s = this->GetPassTime();
		cmd.coin = m_get_coin;
		cmd.exp = m_get_exp;

		cmd.param1 = m_wave;
		cmd.param2 = m_next_wave_start_time - static_cast<unsigned int>(EngineAdapter::Instance().Time());
		cmd.param2 = cmd.param2 < 0 ? 0 : cmd.param2;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
	}
}


