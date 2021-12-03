#include "specialpatafb.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/event/eventhandler.hpp"
#include "other/fb/patafbconfig.hpp"
#include "protocal/msgfight.h"
#include "protocal/msgfb.h"
#include "npc/transmit/fbmanager.hpp"
#include "task/newtaskmanager.h"
#include "other/capability/capability.hpp"
#include "other/fb/rolepatafb.hpp"
#include "gamelog.h"
#include "engineadapter.h"
#include "skill/skillbase.hpp"
#include "effect/skilleffect/effectbuff.hpp"

SpecialPataFb::SpecialPataFb(Scene *scene)
	: SpecialLogic(scene), m_owner_objid(INVALID_OBJ_ID), m_owner_user_id(INVALID_USER_ID), 
	m_is_finish(false), m_is_pass(false), m_monster_id(0), m_die_timestamp(0)
{
}

SpecialPataFb::~SpecialPataFb()
{
}

void SpecialPataFb::OnReqNextLevel()
{
	if (!m_is_finish || !m_is_pass)
	{
		return;
	}

	if (!this->IsAllMonsterDead())
	{
		return;
	}

	Role *role = this->GetOwnerRole();
	if (NULL == role)
	{
		return;
	}

	int pass_level = role->GetRolePataFB()->GetTodayLevel();
	const PataLevelConfig *patafb_cfg = LOGIC_CONFIG->GetPataFbCfg().GetLevelCfgByLevel(pass_level + 1);
	if (NULL == patafb_cfg)
	{
		return;
	}

	if (!m_scene->ResetScene(role, false))
	{
		return;
	}

	role->ResetPos(patafb_cfg->birth_pos.x, patafb_cfg->birth_pos.y);

	// 未达到推荐战力时
	if ( role->GetCapability()->GetTotalCapability() < patafb_cfg->capability)
	{
		Attribute add_hp = 0, add_gongji = 0;
		const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(patafb_cfg->boss_id);
		if (NULL != monster_param)
		{
			const PataOtherConfig &other_cfg = LOGIC_CONFIG->GetPataFbCfg().GetOtherCfg();
			add_hp = (Attribute)(monster_param->hp * (other_cfg.add_hp_per * SKILL_ATTR_PER));
			add_gongji = (Attribute)(monster_param->gongji * (other_cfg.add_gongji_per * SKILL_ATTR_PER));
		}

		if (NULL != MONSTERPOOL->CreateMonster(patafb_cfg->boss_id, role->GetScene(), patafb_cfg->boss_pos, add_hp, add_gongji))
		{
			m_is_finish = false;
			m_is_pass = false;
			m_monster_id = patafb_cfg->boss_id;
		}

		// 战力没达到时,把buff消除了
		role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_ADD_GONGJI, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SCENE_SKILL, 1);
	}
	else
	{
		if (NULL != MONSTERPOOL->CreateMonster(patafb_cfg->boss_id, role->GetScene(), patafb_cfg->boss_pos))
		{
			m_is_finish = false;
			m_is_pass = false;
			m_monster_id = patafb_cfg->boss_id;
		}
	}

	{
		m_owner_user_id = role->GetUserId();
		m_owner_objid = role->GetId();
		this->SyncFBSceneLogicInfo();
	}
}

void SpecialPataFb::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	static const unsigned int DEATH_KICK_TIME = 10;
	if (0 != m_die_timestamp && m_die_timestamp + DEATH_KICK_TIME < now_second)
	{
		Role *role = this->GetOwnerRole();
		if (NULL != role && !role->IsAlive())
		{
			static const int REALIVE_RECOVER_HP_PER = 10;
			m_die_timestamp = 0;
			this->DelayKickOutAllRole();
			role->ReAlive(REALIVE_RECOVER_HP_PER);
		}
	}

	if (m_is_finish || INVALID_USER_ID == m_owner_user_id) return;

	this->CheckSceneMonsterStatus(interval, now_second);
}

void SpecialPataFb::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead)
	{
		return;
	}

	Role *role = GetOwnerRole();
	if (NULL != role)
	{
		if(!role->IsAlive())
		{
			m_die_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			this->OnFinish(false);
			this->SyncFBSceneLogicInfo();
			this->KillAllMonster();
			return;
		}
	}
}

void SpecialPataFb::OnRoleEnterScene(Role *role)
{
	if (NULL != role)
	{
		if (m_is_finish)
		{
			if (!role->IsAlive())
			{
				static const int REALIVE_RECOVER_HP_PER = 10;
				m_die_timestamp = 0;
				role->ReAlive(REALIVE_RECOVER_HP_PER);
			}
			this->DelayKickOutAllRole();
			return;
		}

		bool is_first_enter = (INVALID_USER_ID == m_owner_user_id);
		if (is_first_enter)
		{
			int pass_level = role->GetRolePataFB()->GetTodayLevel();
			const PataLevelConfig *level_cfg = LOGIC_CONFIG->GetPataFbCfg().GetLevelCfgByLevel(pass_level + 1);
			if (NULL != level_cfg)
			{
				// 未达到推荐战力时
				if ( role->GetCapability()->GetTotalCapability() < level_cfg->capability)
				{
					Attribute add_hp = 0, add_gongji = 0;
					const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(level_cfg->boss_id);
					if (NULL != monster_param)
					{
						const PataOtherConfig &other_cfg = LOGIC_CONFIG->GetPataFbCfg().GetOtherCfg();
						add_hp = (Attribute)(monster_param->hp * (other_cfg.add_hp_per * SKILL_ATTR_PER));
						add_gongji = (Attribute)(monster_param->gongji * (other_cfg.add_gongji_per * SKILL_ATTR_PER));
					}

					m_monster_id = level_cfg->boss_id;
					MONSTERPOOL->CreateMonster(level_cfg->boss_id, role->GetScene(), level_cfg->boss_pos, add_hp, add_gongji);
				}
				else
				{
					m_monster_id = level_cfg->boss_id;
					MONSTERPOOL->CreateMonster(level_cfg->boss_id, role->GetScene(), level_cfg->boss_pos);
				}
			}
		}

		// 战力达到时,加buff
		int pass_level = role->GetRolePataFB()->GetTodayLevel();
		const PataLevelConfig *cfg = LOGIC_CONFIG->GetPataFbCfg().GetLevelCfgByLevel(pass_level + 1);

		if (nullptr != cfg && role->GetCapability()->GetTotalCapability() >= cfg->capability)
		{
			static const int BUFF_TIME_MS = 7200 * 1000;
			int add_gongji_per = LOGIC_CONFIG->GetPataFbCfg().GetOtherCfg().add_role_hurt_buff_percent;
			if (add_gongji_per > 0)
			{
				EffectBuff *add_gongji = new EffectBuff(role->GetId(), 1, BUFF_TIME_MS, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
				add_gongji->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
				Attribute value = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (add_gongji_per * ROLE_ATTR_PER));
				add_gongji->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, value, add_gongji_per);

				role->AddEffect(add_gongji);
			}
		}

		m_owner_user_id = role->GetUserId();
		m_owner_objid = role->GetId();
		this->SyncFBSceneLogicInfo();
	}
}

void SpecialPataFb::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (!role->IsAlive())
	{
		static const int REALIVE_RECOVER_HP_PER = 10;
		m_die_timestamp = 0;
		role->ReAlive(REALIVE_RECOVER_HP_PER);
	}

	role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_ADD_GONGJI, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SCENE_SKILL, 1);
}

bool SpecialPataFb::OnTimeout()
{
	if (!m_is_finish)
	{
		this->KillAllMonster();
		this->OnFinish(false);
	}

	return m_is_pass;
}

bool SpecialPataFb::SpecialRelive(Role *role, int realive_type)
{
	if (m_is_finish) return false;

	this->OnFinish(false);
	this->DelayKickOutRole(role);

	return false;
}

Role * SpecialPataFb::GetOwnerRole()
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

void SpecialPataFb::OnAllMonsterDead()
{
	this->OnFinish(true);
}

void SpecialPataFb::OnFinish(bool is_pass)
{
	if (!m_is_finish)
	{
		m_is_finish = true;
		m_is_pass = is_pass;

		this->SyncFBSceneLogicInfo();

		Role *role = this->GetOwnerRole();
		if (NULL != role)
		{
			this->NotifyBossHPPercent(m_monster_id);

			if (m_is_pass)
			{
				const PataLevelConfig *level_cfg = LOGIC_CONFIG->GetPataFbCfg().GetLevelCfgByMonsterID(m_monster_id);
				if (NULL != level_cfg)
				{
					role->Recover();
					role->GetRolePataFB()->OnFinish(level_cfg->level);
					role->GetTaskManager()->OnPassFB(FBManager::FBCT_PATAFB);
					role->GetTaskManager()->OnPassFBLayer(FBManager::FBCT_PATAFB);

					ROLE_LOG_QUICK6(LOG_TYPE_PATA_FB, role, role->GetLevel(), level_cfg->level, NULL, NULL);
				}

				EventHandler::Instance().OnPassPataFB(role);
			}
		}
	}
}

void SpecialPataFb::SyncFBSceneLogicInfo(bool is_active_leave_fb)
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

		cmd.total_boss_num = 1;
		cmd.total_allmonster_num = 1;
		cmd.kill_boss_num = m_is_pass ? 1 : 0;
		cmd.kill_allmonster_num = m_is_pass ? 1 : 0;

		cmd.pass_time_s = this->GetPassTime();

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
	}
}