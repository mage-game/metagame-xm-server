#include "specialchallenge.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/fb/fbchallengeconfig.hpp"
#include "servercommon/errornum.h"
#include "engineadapter.h"
#include "protocal/msgfb.h"
#include "protocal/msgfight.h"
#include "monster/monsterpool.h"
#include "servercommon/fbdef.hpp"
#include "obj/otherobj/fallingitem.h"
#include "gameworld/item/itempool.h"
#include "other/event/eventhandler.hpp"
#include "other/fb/rolefbchallenge.hpp"
#include "other/capability/capability.hpp"
#include "scene/challengefbmanager/challengefbmanager.hpp"
#include "effect/skilleffect/effectbuff.hpp"

SpecialChallenge::SpecialChallenge(Scene *scene)
: SpecialLogic(scene), is_first(true), m_is_finish(false), m_is_passed(false), m_owner_objid(INVALID_OBJ_ID), m_owner_user_id(INVALID_USER_ID), m_boss_id(0),
	m_die_timestamp(0), m_get_coin(0), m_get_item_count(0),m_pass_kick_out_time(0)
{
	memset(m_reward_item_list, 0, sizeof(m_reward_item_list));
}

SpecialChallenge::~SpecialChallenge()
{

}

void SpecialChallenge::Update(unsigned long interval, time_t now_second)
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
			role->ReAlive(REALIVE_RECOVER_HP_PER);
			this->DelayKickOutAllRole();
		}
	}

	if (is_first)
	{
		is_first = false;

		// 刷怪物
		const ChallengeFBLevelCfg::Layer *layer_cfg = LOGIC_CONFIG->GetChallengeFBConfig().GetLayerCfgBySceneID(m_scene->GetSceneID());
		if (NULL != layer_cfg)
		{
			if(layer_cfg->boss_pos.x > 0 && layer_cfg->boss_pos.y > 0)
			{
				m_boss_id = layer_cfg->boss_id;
				MONSTERPOOL->CreateMonster(layer_cfg->boss_id, m_scene, layer_cfg->boss_pos);
			}
		}
	}

	if (m_is_finish)
	{
		if (m_pass_kick_out_time > 0 && now_second >= m_pass_kick_out_time)
		{
			this->DelayKickOutAllRole(CHANGE_SCENE_REASON_FB_FINISH);
		}
		return;
	}

	this->CheckSceneMonsterStatus(interval, now_second);
}

void SpecialChallenge::OnRoleEnterScene(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	ChallengeFBManager::Instance().OnEnter(role);

	if (m_is_finish)
	{
		const ChallengeFBLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetChallengeFBConfig().GetLevelCfgByEndSceneID(m_scene->GetSceneID());
		if (NULL != lvl_cfg)
		{
			if (!role->IsAlive())
			{
				static const int REALIVE_RECOVER_HP_PER = 10;
				m_die_timestamp = 0;
				role->ReAlive(REALIVE_RECOVER_HP_PER);
			}

			this->DelayKickOutRole(role, CHANGE_SCENE_REASON_FB_FINISH);
			return;
		}
	}

	// 战力达到时,加buff
	const ChallengeFBLevelCfg::Layer *layer_cfg = LOGIC_CONFIG->GetChallengeFBConfig().GetLayerCfgBySceneID(m_scene->GetSceneID());
	if (nullptr != layer_cfg && role->GetCapability()->GetTotalCapability() >= layer_cfg->zhanli)
	{
		static const int BUFF_TIME_MS = 3600 * 1000;
		int add_gongji_per = LOGIC_CONFIG->GetChallengeFBConfig().GetCapAddGongjiPer();
		Attribute value = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (add_gongji_per * ROLE_ATTR_PER));

		EffectBuff *add_gongji = new EffectBuff(role->GetId(), 1, BUFF_TIME_MS, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		add_gongji->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		add_gongji->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, value, add_gongji_per);
		role->AddEffect(add_gongji);
	}

	m_owner_user_id = role->GetUserId(); 
	m_owner_objid = role->GetId();

	role->GetFBChallenge()->SyncLevelInfo(m_scene->GetSceneID());

	this->SendLayerInfo();
	role->GetFBChallenge()->SendInfo();

	{
		int belong_layer = -1;
		const ChallengeFBLevelCfg::ConfigItem *cfg = LOGIC_CONFIG->GetChallengeFBConfig().GetLevelCfgBySceneID(m_scene->GetSceneID(), &belong_layer);
		if (NULL != cfg) 
		{
			EventHandler::Instance().OnEnterChallengeFB(role, cfg->level, belong_layer);
		}
	}
}

void SpecialChallenge::OnLeaveFB(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (!role->IsAlive())
	{
		static const int REALIVE_RECOVER_HP_PER = 10;
		m_die_timestamp = 0;
		role->ReAlive(REALIVE_RECOVER_HP_PER);
	}

	if (!m_is_finish)
	{
		if (role->GetUserId() == m_owner_user_id)
		{
			this->OnLayerFinish(false, true);
		}
	}

	role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_ADD_GONGJI, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SCENE_SKILL, 1);
}

bool SpecialChallenge::OnTimeout()
{
	this->OnLayerFinish(false);
	return false;
}

void SpecialChallenge::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL != dead)
	{
		if (Obj::OBJ_TYPE_ROLE == dead->GetObjType())
		{
			m_die_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			this->OnLayerFinish(false);
			this->KillAllMonster();
		}
	}
}

void SpecialChallenge::OnRemoveObj(Obj *obj)
{
	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Role *role = this->GetOwnerRole();
		if (NULL != role)
		{
			m_scene->SyncSceneTimeOut(role);
		}
	}
}

bool SpecialChallenge::SpecialRelive(Role *role, int realive_type)
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

bool SpecialChallenge::SpecialCanTransport(Role *role)
{
	if (m_is_passed) return true;

	return false;
}

void SpecialChallenge::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{
	ChallengeFBManager::Instance().OnPickFallingItem(role, falling_item);

	m_get_item_count = ChallengeFBManager::Instance().GetTotalFallItem(role, m_reward_item_list);

	//用于客户端掉落统计
	{
		Protocol::SCFBDropInfo fbdi;
		fbdi.get_coin = m_get_coin;
		fbdi.get_item_count = 0;

		memset(fbdi.drop_items, 0, sizeof(Protocol::SCFBDropInfo::DropItem) * Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM);
		for (int i = 0; i < m_get_item_count && i < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM; ++i)
		{
			fbdi.drop_items[i].item_id = m_reward_item_list[i].item_id;
			fbdi.drop_items[i].num = static_cast<short>(m_reward_item_list[i].num);

			++fbdi.get_item_count;
		}

		int len = sizeof(fbdi) - sizeof(fbdi.drop_items[0]) * (Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM - fbdi.get_item_count);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fbdi, len);
	}
}

void SpecialChallenge::OnLayerFinish(bool is_passed, bool is_active_leave_fb)
{
	if (!m_is_finish)
	{
		m_is_finish = true;
		m_is_passed = is_passed;
		
		if (!is_active_leave_fb)
		{
			this->NotifyBossHPPercent(m_boss_id);
		}

		Role *role = this->GetOwnerRole();
		if (NULL != role)
		{
			this->AutoPickFallingItem(role);
			role->GetFBChallenge()->OnFBFinish(is_passed, m_scene->GetSceneID(), is_active_leave_fb, m_scene->GetCreateTime());

			if (!is_active_leave_fb)
			{
				this->SendLayerInfo();
			}

			int belong_layer = -1;
			const ChallengeFBLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetChallengeFBConfig().GetLevelCfgBySceneID(m_scene->GetSceneID(), &belong_layer);
			if (NULL != lvl_cfg)
			{
				// 日志
				int use_time = static_cast<int>(EngineAdapter::Instance().Time() - m_scene->GetCreateTime());
				
				gamelog::g_log_fb.printf(LL_INFO, "ChallengeFB::OnLayerFinish user[%d, %s], role_level[%d], is_pass[%d], use_time[%d], level[%d], layer[%d]", 
					role->GetUID(), role->GetName(), role->GetLevel(), (is_passed ? 1 : 0), use_time, lvl_cfg->level, belong_layer);

				if (!is_passed || (is_passed && belong_layer >= (LOGIC_CONFIG->GetChallengeFBConfig().GetMaxLayer() - 1)))
				{
					ChallengeFBManager::Instance().OnFbFinish(role);
				}

				if (!is_passed)
				{
					this->DelayKickOutRole(role, CHANGE_SCENE_REASON_FB_FINISH);
				}
				else if (is_passed && belong_layer >= (LOGIC_CONFIG->GetChallengeFBConfig().GetMaxLayer() - 1))
				{
					m_pass_kick_out_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 9;
				}
			}
		}
	}
}

Role * SpecialChallenge::GetOwnerRole()
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

void SpecialChallenge::OnAllMonsterDead()
{
	this->OnLayerFinish(true);
}

void SpecialChallenge::SendLayerInfo()
{
	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		Protocol::SCChallengeLayerInfo cpl;
		cpl.is_pass = (m_is_passed ? 1 : 0);
		cpl.is_finish = (m_is_finish ? 1 : 0);
		cpl.reserve_sh = 0;
		cpl.time_out_stamp = m_scene->GetSceneTimeOutStamp();
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cpl, sizeof(cpl));
	}
}