#include "specialarmordefend.hpp"
#include "engineadapter.h"
#include "gamecommon.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/servercommon.h"

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

//#include "ai/monsterai/specialai/monsteraitowerdefendteam.hpp"
#include "ai/monsterai/specialai/monsterairunningmonster.hpp"

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
#include "other/mount/mountmanager.hpp"
#include "gameworld/config/logicconfigmanager.hpp"
#include "other/fb/armordefendconfig.hpp"
#include "other/fb/rolefbarmordefend.hpp"
#include "npc/transmit/fbmanager.hpp"
#include "gameworld/skill/skillbase.hpp"
#include "item/knapsack.h"

SpecialArmorDefend::SpecialArmorDefend(Scene *scene)
	: SpecialLogic(scene), m_init_time(0), m_is_finish(false), m_is_pass(false), m_is_last_wave(false), m_msg_need_sync(false), m_is_new_player(false),
	m_curr_wave_index(-1), m_next_wave_start_time(0), m_next_refresh_one_time(0),m_owner_objid(0), m_energy(0), m_clear_wave_count(0),
	m_get_coin(0), m_get_item_count(0), m_escape_monster_count(0),m_escape_boss_count(0),m_refresh_when_clean(0), m_pass_kick_out_time(0)
{
	memset(m_refresh_left_count, 0, sizeof(m_refresh_left_count));
	memset(m_kill_count, 0, sizeof(m_kill_count));
	memset(m_next_perform_time_list, 0, sizeof(m_next_perform_time_list));
	memset(m_reward_item_list, 0, sizeof(m_reward_item_list));
}

SpecialArmorDefend::~SpecialArmorDefend()
{

}

void SpecialArmorDefend::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_is_finish)
	{
		if (m_pass_kick_out_time > 0 && now_second >= m_pass_kick_out_time)
		{
			this->DelayKickOutAllRole(CHANGE_SCENE_REASON_FB_FINISH);
		}
		return;
	}

	{
		// 刚启动
		if (0 == m_init_time)
		{
			m_init_time = static_cast<int>(EngineAdapter::Instance().Time());

			m_next_wave_start_time = static_cast<unsigned int>(now_second) + LOGIC_CONFIG->GetArmorDefendConfig().GetRefreshStartDelay();
			m_refresh_when_clean = 0;

			this->SyncToClient(Protocol::SCTowerDefendInfo::NOTIFY_REASON_INIT); 
		}
	}

	{
		// 新一波开始
		if (m_next_wave_start_time > 0 && now_second >= m_next_wave_start_time)
		{
			++ m_curr_wave_index;

			const  ArmorDefendWaveCfg * wave_cfg = LOGIC_CONFIG->GetArmorDefendConfig().GetWaveCfg(m_scene->GetSceneID(), m_curr_wave_index, m_is_new_player);
			if (NULL != wave_cfg) 
			{
				m_next_wave_start_time = static_cast<unsigned int>(now_second) + wave_cfg->next_wave_s;

				for (int i = 0; i < TOWERDEFEND_PATH_MAX; ++i)
				{
					m_refresh_left_count[i] = wave_cfg->monster_count_list[i];
				}

				this->RefreshOne(now_second);
			}

			if (m_curr_wave_index + 1 >= LOGIC_CONFIG->GetArmorDefendConfig().GetWaveCountBySceneID(m_scene->GetSceneID(), m_is_new_player))	// 最后一波了
			{
				m_is_last_wave = true;
				m_next_wave_start_time = 0;
			}

			this->SyncToClient(Protocol::SCTowerDefendInfo::NOTIFY_REASON_NEW_WAVE_START); 
		}
	}

	{
		// 按频率在各路线出怪
		if (m_next_refresh_one_time > 0 && now_second >= m_next_refresh_one_time)
		{
			this->RefreshOne(now_second);
		}
	}

	if (!m_is_finish)
	{
		this->CheckMonsterEscape();
	}

	{
		if (m_curr_wave_index >= 0 && m_msg_need_sync)
		{
			this->SyncToClient();
		}
	}
}

void SpecialArmorDefend::OnRoleEnterScene(Role *role)
{
	if (m_is_finish)
	{
		this->DelayKickOutRole(role, CHANGE_SCENE_REASON_FB_FINISH);
	}
	else
	{
		bool is_first_enter = (INVALID_USER_ID == m_owner_user_id);

		m_owner_user_id = role->GetUserId(); 
		m_owner_objid = role->GetId();

		EventHandler::Instance().OnEnterArmorDefendFB(role,is_first_enter);
		
		this->SyncToClient();
	}
}

void SpecialArmorDefend::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = (Monster*)dead;

		if (NULL != monster && MonsterInitParam::AITYPE_BUILD_TOWER_RUNNING == monster->GetAIType())
		{
			// 杀怪增加能量
			int old_energy = m_energy;

			m_energy += LOGIC_CONFIG->GetArmorDefendConfig().GetEnergyByMonsterId( monster->GetMonsterId());

			if (m_energy > ENERGY_MAX_NUM)
			{
				m_energy = ENERGY_MAX_NUM;
			}

			if (old_energy != m_energy)
			{
				m_msg_need_sync = true;
			}

			this->RemoveFromMoveList(monster->GetId());
		}
	}
}

void SpecialArmorDefend::OnLeaveFB(Role *role)
{
	if (!m_is_finish)
	{
		bool is_pass = m_is_new_player ? true : false;
		this->OnFinish(is_pass);
	}

	role->GetMountManager()->GoonMount(true, false, true);
}


bool SpecialArmorDefend::SpecialRelive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		Posi realive_pos;
		int target_scene_id = m_scene->GetTownPoint(role, &realive_pos);
		if (target_scene_id == m_scene->GetSceneID())
		{
			if (role->ReAlive(10, 100, realive_pos)) return true;
		}
	}
	
	return false;
}

bool SpecialArmorDefend::SpecialIsEnemy(Role *role, Obj *obj)
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
		return monster->IsAlive() && MonsterInitParam::AITYPE_BUILD_TOWER_RUNNING == monster->GetAIType();
	}
	break;
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

void SpecialArmorDefend::OnNextWaveReq(Role *role)
{
	const ArmorDefendWaveCfg * wave_cfg = LOGIC_CONFIG->GetArmorDefendConfig().GetWaveCfg(m_scene->GetSceneID(), m_curr_wave_index + 1, m_is_new_player);
	if (NULL == wave_cfg|| role ==NULL) return;


	if (!IsCurrWaveRefreshAll())
	{
		role->NoticeNum(errornum::EN_TOWER_DEFEND_CURR_WAVE_NOT_DONE);
		return;
	}

	if (m_moving_monster_step_map.size() != 0)
	{
		role->NoticeNum(errornum::EN_FB_REST_MONSTER_NUM_LIMIT);
		return;
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (m_next_wave_start_time > 0 && now_second < m_next_wave_start_time)
	{
		m_next_wave_start_time = now_second;

		this->SyncToClient();

		role->NoticeNum(noticenum::NT_TOWERDEFEND_NEXT_WAVE_SUCC);
	}
}

void SpecialArmorDefend::SetRefreshWhenClean(short parm1)
{
	if (m_refresh_when_clean == parm1)
	{
		return;
	}

	if (parm1 == 0)
	{
		m_refresh_when_clean = 0;
		this->SyncToClient();
	}
	else if (parm1 == 1)
	{
		m_refresh_when_clean = 1;
		this->SyncToClient();
	}
}

void SpecialArmorDefend::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{
	if (NULL != role && NULL != falling_item)
	{
		const ItemBase *item = ITEMPOOL->GetItem(falling_item->GetItemId());
		if (NULL != item)
		{
			bool is_find = false;

			for (int i = 0; i < m_get_item_count && i < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM; ++ i)
			{
				if (m_reward_item_list[i].item_id == falling_item->GetItemId())
				{
					m_reward_item_list[i].num += falling_item->GetItemNum();
					is_find = true;

					break;
				}	
			}

			if (!is_find && m_get_item_count < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM)
			{
				m_reward_item_list[m_get_item_count].item_id = falling_item->GetItemId();
				m_reward_item_list[m_get_item_count].num = falling_item->GetItemNum();

				++ m_get_item_count;
			}
		}
		else
		{
			m_get_coin += falling_item->GetCoinOrGold();
		}
	}

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

Role* SpecialArmorDefend::GetOwnerRole()
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

void SpecialArmorDefend::RefreshOne(time_t now_second)
{
	const ArmorDefendWaveCfg * wave_cfg = LOGIC_CONFIG->GetArmorDefendConfig().GetWaveCfg(m_scene->GetSceneID(), m_curr_wave_index, m_is_new_player);
	if (NULL == wave_cfg) return;

	bool is_left = false;

	for (int path = ARMORDEFEND_PATH_LEFT; path < ARMORDEFEND_PATH_MAX; ++path)
	{
		if (m_refresh_left_count[path] > 0)
		{
			Posi birth_pos = LOGIC_CONFIG->GetArmorDefendConfig().GetBirthPos(path);

			Monster *monster = MONSTERPOOL->CreateMonster(wave_cfg->monster_id_list[path], m_scene, gamecommon::GetDisRandPos(birth_pos, 4));
			if (NULL!=monster && monster->GetAIType() == MonsterInitParam::AITYPE_BUILD_TOWER_RUNNING)
			{
				MonsterAIRunningMonster *ai = (MonsterAIRunningMonster *)monster->GetMonsterAI();

				Posi walk_pos_list[MAX_RUNNING_MONSTER_AI_RUNNING_STEP];
				memset(walk_pos_list, 0, sizeof(walk_pos_list));
				const ArmorDefendMonsterPath* ampath = LOGIC_CONFIG->GetArmorDefendConfig().GetArmorDefendPathByMonsterID(m_scene->GetSceneID(), monster->GetMonsterId(), m_is_new_player);
				if (NULL != ampath && ampath->cfg_count < MAX_RUNNING_MONSTER_AI_RUNNING_STEP)
				{
					memcpy(walk_pos_list, ampath->pos_list, sizeof(Posi)*ampath->cfg_count);
					ai->SetWalkPosList(ampath->cfg_count, walk_pos_list);
				}

				 this->AddToMoveList(monster->GetId());
			}

			--m_refresh_left_count[path];
			if (m_refresh_left_count[path] > 0) is_left = true;
		}
	}

	if (is_left)
	{
		m_next_refresh_one_time = static_cast<unsigned int>(now_second) + LOGIC_CONFIG->GetArmorDefendConfig().GetRefreshInterval();
	}
	else
	{
		m_next_refresh_one_time = 0;
	}

}

void SpecialArmorDefend::OnFinish(bool is_pass)
{
	m_is_pass = is_pass;
	m_is_finish = true;
	m_next_wave_start_time = 0;

	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		role->GetRoleFbArmorDefend()->OnPersonalFBFinish(is_pass, m_scene->GetSceneID(), m_clear_wave_count);
		role->NotifyFBFinish();
		this->AutoPickFallingItem(role);

		if (m_is_new_player)
		{
			const ArmorDefendOtherCfg& other_cfg = LOGIC_CONFIG->GetArmorDefendConfig().GetOtherCfg();

			role->GetKnapsack()->PutList(other_cfg.reward_count, other_cfg.reward_item_list, PUT_REASON_ARMORDEFEND_NEW_PLAYER_REWARD);

			for (int i = 0; i < other_cfg.reward_count; i ++)
			{
				m_reward_item_list[i].item_id = other_cfg.reward_item_list[i].item_id;
				m_reward_item_list[i].num = other_cfg.reward_item_list[i].num;
			}
		}
	}
	UInt32 monster_num = m_scene->MonsterNum();
	for (int i = 0; i < static_cast<int>(monster_num); i++)
	{
		Monster *monster = m_scene->GetMonsterByIndex(i);
		if (NULL != monster && monster->GetMonsterId() != LOGIC_CONFIG->GetArmorDefendConfig().GetLifeTowerMonsterID())
		{
			monster->ForceSetDead();
		}
	}

	this->SyncToClient(Protocol::SCArmorDefendInfo::NOTIFY_REASON_FINISHED);
	this->SendArmorDefendDropInfo();
	if (NULL != role)
	{
		if (m_is_pass)
		{
			m_pass_kick_out_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 9;
		}
		else
		{
			this->DelayKickOutRole(role, CHANGE_SCENE_REASON_FB_FINISH);
		}
	}
}

void SpecialArmorDefend::SyncToClient(int reason)
{
	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		static Protocol::SCArmorDefendInfo ttdi;
		ttdi.reason = static_cast<short>(reason);
		ttdi.escape_monster_count = m_escape_monster_count;
		ttdi.curr_wave = m_curr_wave_index;
		ttdi.energy = m_energy;
		ttdi.next_wave_refresh_time = m_next_wave_start_time;
		ttdi.clear_wave_count = m_clear_wave_count;
		ttdi.refresh_when_clear = m_refresh_when_clean;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&ttdi, sizeof(ttdi));
	}

	m_msg_need_sync = false;
}

void SpecialArmorDefend::NotifyWarning()
{
	static Protocol::SCArmorDefendWarning ttdw;
	ttdw.escape_num = m_escape_monster_count;
	ttdw.reserve_sh = 0;

	m_scene->SendToRole((const char *)&ttdw, sizeof(ttdw));
}

void SpecialArmorDefend::SyncPerformSkill(int skill_index)
{
	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		static Protocol::SCArmorDefendPerformSkill adps;
		adps.skill_index = skill_index;
		memcpy(adps.next_perform_time, m_next_perform_time_list, sizeof(m_next_perform_time_list));
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&adps, sizeof(adps));
	}
}

bool SpecialArmorDefend::PerformSkill(Role *role, int skill_index, const Posi &pos, ObjID target_id)
{
	if (NULL == role) return false;

	if (skill_index < 0 || skill_index >= PERSONAL_ARMORDEFEND_SKILL_MAX_COUNT) return false;

	const ArmorDefendSkillCfg::ConfigItem *skill_cfg = LOGIC_CONFIG->GetArmorDefendConfig().GetSkillCfg(skill_index);
	if (NULL == skill_cfg)
	{
		return false;
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	
	if (now_second < m_next_perform_time_list[skill_index])
	{
		role->NoticeNum(errornum::EN_ROLE_FIGHT_CD_NOT_ENOUGH);
		return false;
	}

	if (m_energy < skill_cfg->energy_cost)
	{
		role->NoticeNum(errornum::EN_ROLE_HUNZHI_NOT_ENOUGH);
		return false;
	}

	UInt16 scene_skill_id = GetSkillIDBySceneTypeAndIndex(m_scene->GetSceneType(), skill_index);

	switch(skill_index)
	{
	case 0:
		{
			// 增加攻击防御A%，持续B秒
			//SkillBase::SendAOEAttackMsgHelper(role, target_id,scene_skill_id, role->GetPos());

			int dur_ms = skill_cfg->param_b * 1000;
			EffectBuff *effect_buff = new EffectBuff(role->GetId(), scene_skill_id, dur_ms, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL,EBT_ADD_GONGJI);

			effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);

			int delta_gongji = (int)(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * skill_cfg->param_a / 100.0f);
			effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, delta_gongji, skill_cfg->param_a);

			role->AddEffect(effect_buff);
		}
		break;

	case 1:
		{
			// 群攻
			if ((pos - role->GetPos()).IsLongerThan(skill_cfg->distance + 2))
			{
				role->NoticeNum(errornum::EN_ROLE_FIGHT_DISTANCE_NOT_ENOUGH);
				return false;
			}

			SkillBase::SendAOEAttackMsgHelper(role, target_id,scene_skill_id, pos);

			static ObjID target_list[32] = {INVALID_OBJ_ID};
			int count = SkillBase::GetTargetByArea(role, pos, 8, target_list, 10);

			for (int i = 0; i < count && i < 10; ++ i)
			{
				Obj *target_obj = m_scene->GetObj(target_list[i]);
				if (NULL == target_obj || Obj::OBJ_TYPE_MONSTER != target_obj->GetObjType() || !role->IsEnemy(target_obj))
				{
					continue;
				}

				Monster * target_monster = (Monster*)target_obj;

				Attribute injure = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * skill_cfg->param_a / 100);
				if (injure <= 0)
				{
					injure = 1;
				}

				EffectOnce *tmp_effect = new EffectOnce(role->GetId(), scene_skill_id, -injure, EffectBase::FIGHT_TYPE_NORMAL);
				target_monster->AddEffect(tmp_effect);
			}
		}
		break;
	}

	// 消耗掉能量
	m_energy -= skill_cfg->energy_cost;
	m_next_perform_time_list[skill_index] = now_second + skill_cfg->cd_s;
	this->SyncPerformSkill(skill_index);
	this->SyncToClient();
	return true;
}

void SpecialArmorDefend::GmAddEnergy(int value)
{
	m_energy += value;
	
	if (m_energy < 0)
	{
		m_energy = 0;
	}

	if (m_energy > ENERGY_MAX_NUM)
	{
		m_energy = ENERGY_MAX_NUM;
	}

	this->SyncToClient();
}
void SpecialArmorDefend::AddToMoveList(ObjID obj_id)
{
	MovingMonsterInfoMapIt it = m_moving_monster_step_map.find(obj_id);
	if (it != m_moving_monster_step_map.end())
	{
		return;
	}

	MovingMonsterInfo info;
	info.obj_id = obj_id;
	info.cur_pos_index = 0;

	m_moving_monster_step_map[obj_id] = info;
}
void SpecialArmorDefend::RemoveFromMoveList(ObjID obj_id)
{
	MovingMonsterInfoMapIt it = m_moving_monster_step_map.find(obj_id);
	if (it != m_moving_monster_step_map.end())
	{
		m_moving_monster_step_map.erase(it);
	}
}

void SpecialArmorDefend::CheckMonsterEscape()
{
	std::vector<ObjID> remove_obj_list;

	for (MovingMonsterInfoMapIt it = m_moving_monster_step_map.begin(); m_moving_monster_step_map.end() != it; it++)
	{
		Obj *obj = m_scene->GetObj(it->second.obj_id);
		if (NULL == obj) continue;

		if (obj->GetObjType() != Obj::OBJ_TYPE_MONSTER) continue;

		Monster *monster = (Monster *)obj;
		if (monster->GetAIType() == MonsterInitParam::AITYPE_BUILD_TOWER_RUNNING)
		{
			MonsterAIRunningMonster *ai = (MonsterAIRunningMonster *)monster->GetMonsterAI();
			if (ai->IsAtWalkEndPos())
			{
				remove_obj_list.push_back(monster->GetId());
			}
		}
	}

	for (std::vector<ObjID>::iterator iter = remove_obj_list.begin(); remove_obj_list.end() != iter; ++iter)
	{
		Monster * monster = dynamic_cast<Monster*>(m_scene->GetObj(*iter));
		if (NULL != monster && monster->IsBossMonster())
		{
			m_escape_boss_count++;
		}
		else
		{
			m_escape_monster_count++;
		}

		this->RemoveFromMoveList(*iter);
		m_scene->DeleteObj(*iter);

		m_msg_need_sync = true;
	}
	this->CheckFbFinish();
}

void SpecialArmorDefend::CheckFbFinish()
{
	const ArmorDefendOtherCfg & other_cfg = LOGIC_CONFIG->GetArmorDefendConfig().GetOtherCfg();
	if (m_moving_monster_step_map.size() == 0 && IsCurrWaveRefreshAll())
	{
		if (m_is_last_wave && m_escape_monster_count < other_cfg.escape_num_to_failure && m_escape_boss_count < other_cfg.boss_num_to_failure)
		{
			this->OnFinish(true);
		}

		unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		if (!m_is_last_wave && m_refresh_when_clean == 1 && now_second < m_next_wave_start_time)
		{
			m_next_wave_start_time = now_second;
		}
	}

	if (m_escape_monster_count >= other_cfg.escape_num_to_failure || m_escape_boss_count >= other_cfg.boss_num_to_failure)
	{
		bool is_pass = m_is_new_player ? true : false;
		this->OnFinish(is_pass);
	}

	if (!m_is_finish && m_escape_monster_count >= other_cfg.escape_num_to_warn)
	{
		this->NotifyWarning();
	}
}

void SpecialArmorDefend::SendArmorDefendDropInfo()
{
	if (m_is_finish)
	{
		Role *role = this->GetOwnerRole();
		if (NULL != role)
		{
			Protocol::SCArmorDefendResult ttdr;
			ttdr.is_passed = m_is_pass;
			ttdr.clear_wave_count = m_clear_wave_count;
			ttdr.use_time = static_cast<int>(EngineAdapter::Instance().Time()) - m_init_time;
			ttdr.get_coin = m_get_coin;
			ttdr.get_item_count = 0;

			memset(ttdr.drop_items, 0, sizeof(Protocol::SCArmorDefendResult::DropItem) * Protocol::SCArmorDefendResult::FB_DROP_ITEM_MAX_NUM);
			for (int i = 0; i < m_get_item_count && i < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM; ++i)
			{
				ttdr.drop_items[ttdr.get_item_count].item_id = m_reward_item_list[i].item_id;
				ttdr.drop_items[ttdr.get_item_count].num = static_cast<short>(m_reward_item_list[i].num);

				++ttdr.get_item_count;
			}

			int len = sizeof(ttdr) - sizeof(ttdr.drop_items[0]) * (Protocol::SCArmorDefendResult::FB_DROP_ITEM_MAX_NUM - ttdr.get_item_count);
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&ttdr, len);
		}
	}
}
bool SpecialArmorDefend::IsCurrWaveRefreshAll()
{
	for (int i = 0; i < ARMORDEFEND_PATH_MAX; ++i)
	{
		if (m_refresh_left_count[i] > 0)
		{
			return false;
		}
	}
	return true;
}


