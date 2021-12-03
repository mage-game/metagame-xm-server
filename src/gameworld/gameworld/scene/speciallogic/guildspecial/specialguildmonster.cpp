#include "specialguildmonster.hpp"
#include "engineadapter.h"
#include "gamecommon.h"

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/otherobj/gatherobj.h"
#include "obj/otherobj/fallingitem.h"
#include "monster/monsterpool.h"
#include "item/itempool.h"
#include "npc/transmit/fbmanager.hpp"

#include "scene/scenemanager.h"
#include "world.h"
#include "protocal/msgfight.h"
#include "other/event/eventhandler.hpp"
#include "internalcomm.h"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/string/gameworldstr.h"

#include "config/logicconfigmanager.hpp"
#include "config/guildactivityconfig.hpp"

#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/sceneeffect/sceneeffect.hpp"
#include "effect/effectbase.hpp"

#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"
#include "other/roleactivity/roleactivity.hpp"

SpecialGuildMonster::SpecialGuildMonster(Scene *scene)
	: SpecialLogic(scene), m_owner_user_id(INVALID_USER_ID), m_owner_objid(INVALID_OBJ_ID), m_is_inited(false), m_is_finish(false),
	m_monster_id(0), m_target_seq(0), m_get_coin(0), m_get_item_count(0), m_send_drop_info_timestamp(0)
{
}

SpecialGuildMonster::~SpecialGuildMonster()
{

}

void SpecialGuildMonster::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

// 	if (!m_is_finish)
// 	{
// 		if (!m_is_inited)
// 		{
// 			if (m_monster_id > 0)
// 			{
// 				m_is_inited = true;
// 
// 				Posi born_pos = LOGIC_CONFIG->GetGuildActivityCfg().OtherCfg().monster_boss_born_pos;
// 				MONSTERPOOL->CreateMonster(m_monster_id, m_scene, born_pos);
// 			}
// 		}
// 	}
// 
// 	if (0 != m_send_drop_info_timestamp && EngineAdapter::Instance().Time() >= m_send_drop_info_timestamp)
// 	{
// 		m_send_drop_info_timestamp = 0;
// 
// 		Role *role = this->GetOwnerRole();
// 		if (NULL != role)
// 		{
// 			Protocol::SCFBDropInfo fbdi;
// 			fbdi.get_coin = m_get_coin;
// 			fbdi.get_item_count = 0;
// 
// 			for (int i = 0;i < m_get_item_count && i < Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM; ++ i)
// 			{
// 				fbdi.drop_items[i].item_id = m_reward_item_list[i].item_id;
// 				fbdi.drop_items[i].num = static_cast<short>(m_reward_item_list[i].num);
// 
// 				++ fbdi.get_item_count;
// 			}
// 
// 			int len = sizeof(fbdi) - sizeof(fbdi.drop_items[0]) * (Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM - fbdi.get_item_count);
// 			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fbdi, len);
// 
// 			Protocol::SCGuildMonsterFBResult scret;
// 			scret.is_pass = (m_is_pass ? 1 : 0);
// 
// 			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&scret, sizeof(scret));
// 		}
// 	}
// }
// 
// void SpecialGuildMonster::OnRoleEnterScene(Role *role)
// {
// 	bool is_first_enter = (INVALID_USER_ID == m_owner_user_id);
// 	if (is_first_enter)
// 	{
// 	}
// 
// 	GuildID guild_id = role->GetGuildID();
// 	if (guild_id <= 0)
// 	{
// 		return;
// 	}
// 
// 	Guild *guild = GuildManager::Instance().GetGuild(guild_id);
// 	if (NULL == guild)
// 	{
// 		return;
// 	}
// 
// 	m_owner_user_id = role->GetUserId(); 
// 	m_owner_objid = role->GetId();
// 
// 	m_target_seq = 1 + role->GetRoleActivity()->GetGuildMonsterFinishSeq();
// 
// 	if (m_monster_id <= 0)
// 	{
// 		const GuildMonsterConfig *monster_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetGuildMonsterCfg(m_target_seq);
// 		if (NULL != monster_cfg)
// 		{
// 			m_monster_id = monster_cfg->monster_id;
// 		}
// 	}
// 
// 	this->CheckBuffStatus();
// 
// 	EventHandler::Instance().OnEnterGuildMonsterFB(role, is_first_enter);
}

void SpecialGuildMonster::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{
// 	if (NULL != role && NULL != falling_item)
// 	{
// 		const ItemBase *item = ITEMPOOL->GetItem(falling_item->GetItemId());
// 		if (NULL != item)
// 		{
// 			bool is_find = false;
// 
// 			for (int i = 0; i < m_get_item_count && i < REWARD_ITEM_MAX_TYPE; ++ i)
// 			{
// 				if (m_reward_item_list[i].item_id == falling_item->GetItemId())
// 				{
// 					m_reward_item_list[i].num += falling_item->GetItemNum();
// 					is_find = true;
// 
// 					break;
// 				}	
// 			}
// 
// 			if (!is_find && m_get_item_count < REWARD_ITEM_MAX_TYPE)
// 			{
// 				m_reward_item_list[m_get_item_count].item_id = falling_item->GetItemId();
// 				m_reward_item_list[m_get_item_count].num = falling_item->GetItemNum();
// 
// 				++ m_get_item_count;
// 			}
// 		}
// 		else
// 		{
// 			m_get_coin += falling_item->GetCoinOrGold();
// 		}
// 	}
}

bool SpecialGuildMonster::OnTimeout()
{
	this->OnFinish(false);

	return m_is_pass;
}

void SpecialGuildMonster::OnLeaveFB(Role *role)
{
	this->OnFinish(false, true);
}

void SpecialGuildMonster::OnReqNextLevel()
{
// 	if (!m_is_finish || !m_is_pass)
// 	{
// 		return;
// 	}
// 
// 	if (!this->IsAllMonsterDead())
// 	{
// 		return;
// 	}
// 
// 	Role *role = this->GetOwnerRole();
// 	if (NULL == role)
// 	{
// 		return;
// 	}
// 
// 	int guild_id = role->GetGuildID();
// 	if (guild_id <= 0)
// 	{
// 		return;
// 	}
// 
// 	if (!m_scene->ResetScene(role))
// 	{
// 		return;
// 	}
// 
// 	Guild *guild = GuildManager::Instance().GetGuild(guild_id);
// 	if (NULL == guild)
// 	{
// 		return;
// 	}
// 
// 	int tmp_target_seq = 1 + role->GetRoleActivity()->GetGuildMonsterFinishSeq();
// 	const GuildMonsterConfig *monster_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetGuildMonsterCfg(tmp_target_seq);
// 	if (NULL == monster_cfg)
// 	{
// 		return;
// 	}
// 
// 	Posi born_pos = LOGIC_CONFIG->GetGuildActivityCfg().OtherCfg().monster_boss_born_pos;
// 	if (NULL != MONSTERPOOL->CreateMonster(monster_cfg->monster_id, m_scene, born_pos))
// 	{
// 		m_is_finish = false;
// 		m_is_pass = false;
// 		m_monster_id = monster_cfg->monster_id;
// 		m_target_seq = tmp_target_seq;
// 		m_get_coin = 0;
// 		m_get_item_count = 0;
// 		memset(m_reward_item_list, 0, sizeof(m_reward_item_list));
// 
// 		this->CheckBuffStatus();
// 	}
}

Role* SpecialGuildMonster::GetOwnerRole()
{
	Obj *obj = World::GetInstWorld()->GetSceneManager()->GetRole(m_owner_user_id);
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

void SpecialGuildMonster::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead)
	{
		return;
	}

	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *guild_boss = (Monster*)dead;

		{
			Role *owner_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_owner_user_id);
			if (NULL != owner_role)
			{
				int guildboss_dropfactor = LOGIC_CONFIG->GetGuildConfig().GetGuildLuckyFactor(owner_role->GetGuildLuckyColor(), GUILD_LUCKY_FACTOR_TYPE_BOSSDROP);
				if (RandomNum(100) < guildboss_dropfactor)
				{
					guild_boss->GuildBossLuckyExtraDrop(owner_role);
				}
			}
		}

		this->OnFinish(true);
	}
}

void SpecialGuildMonster::OnFinish(bool is_pass, bool is_active_leave_fb)
{
	if (m_is_finish)
	{
		return;
	}

	m_is_finish = true;
	m_is_pass = is_pass;

	if (!is_active_leave_fb)
	{
		this->NotifyBossHPPercent(m_monster_id);
	}

	if (is_pass)
	{
		m_send_drop_info_timestamp = EngineAdapter::Instance().Time() + DROP_INFO_SEND_DELAY_TIME_GUILD_MONSTER;
	}
	else
	{
		m_send_drop_info_timestamp = EngineAdapter::Instance().Time();
	}

	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		if (is_pass)
		{
			role->GetRoleActivity()->SetGuildMonsterFinishSeq(m_target_seq);

			Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
			if (NULL != guild)
			{
				guild->OnMonsterFinish(m_target_seq, role->GetName());				
			}

			role->GetRoleActivity()->SendGuildMonsterInfo(); 

// 			int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_kill_boss, 
// 				role->GetUID(), role->GetName(), (int)role->GetCamp(), m_monster_id);
// 
// 			if (len > 0) 
// 			{
// 				World::GetInstWorld()->GetSceneManager()->SystemMsgToGuild(role->GetGuildID(), gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER);
// 			}
		}

		{
			// »’÷æ
			int use_time = static_cast<int>(EngineAdapter::Instance().Time() - m_scene->GetCreateTime());

			gamelog::g_log_fb.printf(LL_INFO, "GuildMOnster::OnFinish user[%d, %s], level[%d], use_time[%d]", 
				role->GetUID(), role->GetName(), role->GetLevel(), use_time);
		}
	}
}

void SpecialGuildMonster::CheckBuffStatus()
{
// 	Role *role = this->GetOwnerRole();
// 	if (NULL == role)
// 	{
// 		return;
// 	}
// 
// 	int guild_id = role->GetGuildID();
// 	if (guild_id <= 0)
// 	{
// 		return;
// 	}
// 
// 	Guild *guild = GuildManager::Instance().GetGuild(guild_id);
// 	if (NULL == guild)
// 	{
// 		return;
// 	}
// 
// 	int skill_id = GetSkillIDBySceneTypeAndIndex(Scene::SCENE_TYPE_GUILD_MONSTER, 1);
// 	role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_INVALID, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SCENE_SKILL, skill_id);
// 
// 	int guild_finish_seq = guild->GetMonsterFinishSeq();
// 	if (guild_finish_seq >= m_target_seq)
// 	{
// 		int add_percent = LOGIC_CONFIG->GetGuildActivityCfg().OtherCfg().monster_buff_percent;
// 		int add_gongji = (int)(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (add_percent / 100.0f));
// 		int add_fangyu = (int)(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * (add_percent / 100.0f));
// 
// 		EffectBuff *buff = new EffectBuff(INVALID_OBJ_ID, skill_id, 3600 * 1000, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
// 		buff->AddGongJiEffect(add_gongji, add_percent);
// 		buff->AddFangYuEffect(add_fangyu, add_percent);
// 		buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
// 
// 		role->AddEffect(buff);
// 	}
}


