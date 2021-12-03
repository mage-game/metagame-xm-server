#include "specialrunetower.hpp"

#include "config/logicconfigmanager.hpp"
#include "other/runesystem/runetowerconfig.hpp"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "engineadapter.h"
#include "gamecommon.h"
#include "world.h"
#include "gamelog.h"
#include "servercommon/string/gameworldstr.h"
#include "protocal/msgrunesystem.h"
#include "protocal/msgfight.h"
#include "protocal/msgfb.h"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "globalconfig/globalconfig.h"
#include "npc/transmit/fbmanager.hpp"

#include "skill/skillbase.hpp"
#include "other/runesystem/runesystem.hpp"
#include "other/event/eventhandler.hpp"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"
#include "effect/skilleffect/effectbuff.hpp"

SpecialRuneTower::SpecialRuneTower(Scene *scene)
	: SpecialLogic(scene), m_owner_objid(INVALID_OBJ_ID), m_owner_user_id(INVALID_USER_ID),
	m_is_finish(false), m_is_pass(0), m_is_first(false), m_fb_layer(0), m_total_monster_count(0), m_kill_monster_count(0), m_die_timestamp(0),
	m_refresh_monster(false), m_fresh_monster_timestamp(0)
{
}

SpecialRuneTower::~SpecialRuneTower()
{
}

void SpecialRuneTower::OnRoleEnterScene(Role *role)
{
	m_owner_objid = role->GetId();
	m_owner_user_id = role->GetUserId();

	if (m_is_finish)
	{
		if (!role->IsAlive())
		{
			m_die_timestamp = 0;
			role->ReAlive(100, 100);
		}
		this->DelayKickOutAllRole();
		return;
	}

	if (0 == m_fb_layer)
	{
		m_is_finish = false;
		m_is_pass = false;
		m_fb_layer = role->GetRuneSystem()->GetRuneTowerTodayLayer() + 1;
		m_is_first = m_fb_layer > role->GetRuneSystem()->GetRuneTowerPassLayer();
		static const unsigned int REFRESH_MONSTER_TIME = 10;
		m_fresh_monster_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + REFRESH_MONSTER_TIME;
	}

	// 战力达到时,加buff
	const RuneTowerLayerCfg *cfg = LOGIC_CONFIG->GetRuneTowerCfg().GetRuneTowerLayerCfg(m_fb_layer);
	if (nullptr != cfg && role->GetCapability()->GetTotalCapability() >= cfg->capability)
	{
		static const int BUFF_TIME_MS = 3600 * 1000;
		int add_gongji_per = LOGIC_CONFIG->GetRuneTowerCfg().GetOtherCfg().add_role_gongji_per;
		Attribute value = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (add_gongji_per * ROLE_ATTR_PER));

		EffectBuff *add_gongji = new EffectBuff(role->GetId(), 1, BUFF_TIME_MS, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		add_gongji->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		add_gongji->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, value, add_gongji_per);
		role->AddEffect(add_gongji);
	}

	// 进入自动满血
	role->Recover();

	this->SyncFBSceneLogicInfo();
}

void SpecialRuneTower::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (!role->IsAlive())
	{
		static const int REALIVE_RECOVER_HP_PER = 10;
		m_die_timestamp = 0;
		role->ReAlive(REALIVE_RECOVER_HP_PER);
	}

	role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_ADD_GONGJI, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SCENE_SKILL, 1);

	role->Recover();
}

void SpecialRuneTower::OnLeaveFB(Role *role)
{
	if (NULL == role) return;

	if (!m_is_finish)
	{
		this->KillAllMonster();
		m_is_finish = true;
		//this->OnFinish(false);
	}
}

void SpecialRuneTower::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead)
	{
		return;
	}

	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		m_kill_monster_count++;
		if (this->IsAllMonsterDead())
		{
			this->OnFinish(true);
		}
		else
		{
			this->SyncFBSceneLogicInfo();
		}
	}
	else if (Obj::OBJ_TYPE_ROLE == dead->GetObjType())
	{
		Role *role = (Role*)dead;
		if (!role->IsAlive())
		{
			m_die_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		}

		this->KillAllMonster();
		this->OnFinish(false);
	}
}

bool SpecialRuneTower::SpecialRelive(Role *role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		const int percent = GLOBALCONFIG->GetRealiveRecoverCommonScenePercent();
		role->ReAlive(percent, percent, role->GetPos(), true);
		return true;
	}
	return false;
}

bool SpecialRuneTower::OnTimeout()
{
	if (!m_is_finish)
	{
		this->KillAllMonster();
		this->OnFinish(false);
	}

	return m_is_pass;
}

void SpecialRuneTower::Update(unsigned long interval, time_t now_second)
{
	static const unsigned int DEATH_KICK_TIME = 2;
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

	if (m_fresh_monster_timestamp != 0 && now_second >= m_fresh_monster_timestamp)
	{
		m_fresh_monster_timestamp = 0;
		this->RefreshMonster();
	}
}

void SpecialRuneTower::OnReqNextLevel()
{
	if (!m_is_pass)
	{
		return;
	}

	Role *role = this->GetOwnerRole();
	if (NULL == role)
	{
		return;
	}

	int next_fb_layer = role->GetRuneSystem()->GetRuneTowerTodayLayer() + 1;
	const RuneTowerLayerCfg *cfg = LOGIC_CONFIG->GetRuneTowerCfg().GetRuneTowerLayerCfg(next_fb_layer);
	if (NULL == cfg || role->GetLevel() < cfg->level_limit)
	{
		return;
	}

	role->Recover();
	if (cfg->scene_id == m_scene->GetSceneID())
	{
		if (!m_scene->ResetScene(role, false))
		{
			return;
		}

		m_is_finish = false;
		m_is_pass = false;
		m_fb_layer = next_fb_layer;
		m_is_first = m_fb_layer > role->GetRuneSystem()->GetRuneTowerPassLayer();

		m_fresh_monster_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

		role->ResetPos(cfg->enter_pos.x, cfg->enter_pos.y);
		this->SyncFBSceneLogicInfo();
	}
	else
	{
		World::GetInstWorld()->GetSceneManager()->GoTo(role, cfg->scene_id, role->GetUID(), cfg->enter_pos);
	}
}

void SpecialRuneTower::OnReqRefreshMonster()
{
	m_fresh_monster_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
}

Role * SpecialRuneTower::GetOwnerRole()
{
	Obj *obj = m_scene->GetObj(m_owner_objid);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		if (role->GetUserId() == m_owner_user_id) 
		{
			return role;
		}
	}

	return NULL;
}

void SpecialRuneTower::OnFinish(bool is_pass)
{
	if (m_is_finish)
	{
		return;
	}

	m_is_finish = true;
	m_is_pass = is_pass;

	int reward_list[RUNE_TOWER_RAND_REWARD_MAX_COUNT];
	memset(reward_list, 0, sizeof(reward_list));
	int reward_count = 0;

	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		if (is_pass)
		{
			const RuneTowerLayerCfg *cfg = LOGIC_CONFIG->GetRuneTowerCfg().GetRuneTowerLayerCfg(m_fb_layer);
			if (NULL != cfg)
			{
				if (m_fb_layer > role->GetRuneSystem()->GetRuneTowerPassLayer())    // 首通
				{	
					role->GetRuneSystem()->AddRuneJingHua(cfg->first_reward_rune_exp);
					LOGIC_CONFIG->GetRuneTowerCfg().GetRandRewardList(m_fb_layer, reward_list, reward_count);
					if (reward_count > 0)
					{
						if (role->GetRuneSystem()->GetBagSpaceNum() >= reward_count)
						{
							for (int i = 0; i < reward_count; ++i)
							{
								role->GetRuneSystem()->PutRuneTowerRewardByItemId(reward_list[i]);
							}
						}
						else
						{
							ItemConfigData mail_list[RUNE_TOWER_RAND_REWARD_MAX_COUNT];
							memset(mail_list, 0, sizeof(mail_list));
							for (int i = 0; i < reward_count && i < RUNE_TOWER_RAND_REWARD_MAX_COUNT; i++)
							{
								mail_list[i].item_id = reward_list[i];
								mail_list[i].num = 1;
								mail_list[i].is_bind = 1;
							}
							gamecommon::SendMultiMailToRole(role->GetUserId(), mail_list, reward_count, NULL, true, gamestring::g_runetower_first_reward, m_fb_layer);
						}
					}
				}
				else
				{
					role->GetRuneSystem()->AddRuneJingHua(cfg->normal_reward_rune_exp);
					if (0 != cfg->normal_reward_item_list[0].item_id)
					{
						role->GetKnapsack()->PutListOrMail(cfg->normal_reward_item_list, PUT_REASON_RUNE_TOWER);
					}
				}
			}
			role->GetRuneSystem()->SendVirtualBagAllInfo();
			role->GetRuneSystem()->OnRuneTowerPassLayer(m_fb_layer);
			EventHandler::Instance().OnPassRuneTowerFB(role, m_fb_layer);
		}
		else if(role->IsAlive())
		{
			this->DelayKickOutRole(role);
		}

		ROLE_LOG_QUICK6(LOG_TYPE_RUNE_TOWER, role, role->GetLevel(), m_fb_layer, is_pass ? "succ" : "fail", NULL);
	}
	
	this->SyncFBPassRewardInfo(reward_count, reward_list);
	this->SyncFBSceneLogicInfo();

	m_fb_layer = 0;
	m_total_monster_count = 0;
	m_kill_monster_count = 0;
	m_refresh_monster = false;
}

void SpecialRuneTower::RefreshMonster()
{
	if (m_refresh_monster)
	{
		return;
	}

	const RuneTowerLayerCfg *cfg = LOGIC_CONFIG->GetRuneTowerCfg().GetRuneTowerLayerCfg(m_fb_layer);
	if (NULL == cfg)
	{
		return;
	}

	Role *role = this->GetOwnerRole();
	if (NULL == role)
	{
		return;
	}

	m_refresh_monster = true;
	for (int i = 0; i < cfg->monster_num; i++)
	{
		Posi pos = cfg->monster_pos;

		for (int k = 0; k <= 20; k++)
		{
			Posi temp_pos = gamecommon::GetDisRandPos(cfg->monster_pos, cfg->monster_range);
			if (m_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, temp_pos.x, temp_pos.y))
			{
				pos = temp_pos;
				break;
			}
		}

		Attribute add_hp = 0, add_gongji = 0;
		if (role->GetCapability()->GetTotalCapability() < cfg->capability)
		{
			const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(cfg->monster_id);
			if (NULL != monster_param)
			{
				add_hp = (Attribute)(monster_param->hp * (LOGIC_CONFIG->GetRuneTowerCfg().GetOtherCfg().add_hp_per * SKILL_ATTR_PER));
				add_gongji = (Attribute)(monster_param->gongji * (LOGIC_CONFIG->GetRuneTowerCfg().GetOtherCfg().add_gongji_per * SKILL_ATTR_PER));
			}
		}

		m_total_monster_count++;
		MONSTERPOOL->CreateMonster(cfg->monster_id, m_scene, pos, add_hp, add_gongji);
	}

	this->SyncFBSceneLogicInfo();
}

void SpecialRuneTower::SyncFBSceneLogicInfo()
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

		cmd.total_allmonster_num = m_total_monster_count;
		cmd.kill_allmonster_num = m_kill_monster_count;

		cmd.param1 = m_is_first ? 1 : 0;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
	}
}

void SpecialRuneTower::SyncFBPassRewardInfo(int reward_count, int reward_list[RUNE_TOWER_RAND_REWARD_MAX_COUNT])
{
	Role *role = this->GetOwnerRole();
	if (NULL == role) return;

	static Protocol::SCRuneTowerPassRewardInfo rtpri;
	rtpri.reward_count = 0;
	for (int i = 0; i < reward_count && i < RUNE_TOWER_RAND_REWARD_MAX_COUNT; ++i)
	{
		if (reward_list[i] > 0)
		{
			rtpri.reward_list[rtpri.reward_count].item_id = reward_list[i];
			rtpri.reward_list[rtpri.reward_count].num = 1;
			rtpri.reward_list[rtpri.reward_count].is_bind = 1;

			++rtpri.reward_count;
		}
	}

	int send_len = sizeof(rtpri) - sizeof(rtpri.reward_list) + sizeof(rtpri.reward_list[0]) * rtpri.reward_count;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&rtpri, send_len);
}