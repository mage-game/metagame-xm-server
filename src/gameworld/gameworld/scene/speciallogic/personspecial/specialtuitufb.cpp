#include "specialtuitufb.h"
#include "gameworld/other/fb/rolefbtuitu.h"
#include "gameworld/config/logicconfigmanager.hpp"
#include "gameworld/other/fb/fbtuituconfig.h"
#include "gameworld/effect/skilleffect/effecthurtinc.hpp"
#include "gameworld/effect/skilleffect/effecthurtdec.hpp"
#include "gameworld/monster/drop/droppool.hpp"
#include "gameworld/world.h"
#include "gameworld/obj/otherobj/fallingitem.h"
#include "gameworld/globalconfig/globalconfig.h"
#include "obj/character/role.h"
#include "monster/monsterpool.h"
#include "other/capability/capability.hpp"
#include "protocal/msgfb.h"

SpecialTuitu::SpecialTuitu(Scene *scene) : SpecialLogic(scene), m_is_finish(false), m_is_pass(false), m_pass_star(0),
	m_owner_uid(0), m_fb_type(0), m_chapter(0), m_level(0), m_start_timestamp(0), m_total_boss_num(0), m_cur_boss_num(0),
	m_total_monster_num(0), m_cur_monster_num(0), m_die_timestamp(0)
{
	
}

SpecialTuitu::~SpecialTuitu()
{
	
}

void SpecialTuitu::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	static const unsigned int DEATH_KICK_TIME = 10;
	if (0 != m_die_timestamp && m_die_timestamp + DEATH_KICK_TIME < now_second)
	{
		Role *role = m_scene->GetRoleByUID(m_owner_uid);
		if (NULL != role && !role->IsAlive())
		{
			static const int REALIVE_RECOVER_HP_PER = 10;
			m_die_timestamp = 0;
			this->DelayKickOutAllRole();
			role->ReAlive(REALIVE_RECOVER_HP_PER);
		}
	}

	this->CheckSceneMonsterStatus(interval, now_second);
}

void SpecialTuitu::OnAddObj(Obj *obj)
{
	if (obj != NULL && Obj::OBJ_TYPE_FALLINGITEM == obj->GetObjType())
	{
		ItemConfigData item;

		FallingItem *falling_item = (FallingItem*)obj;
		item.item_id = falling_item->GetItemId();
		item.num = falling_item->GetItemNum();
		item.is_bind = falling_item->GetItemIsBind();

		fall_item_vec.push_back(item);
	}
}

void SpecialTuitu::OnRoleEnterScene(Role *role)
{
	if (NULL == role) return;
	
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

	if (0 == m_owner_uid)
	{
		this->Reset();

		role->GetTuituFb()->GetEnterFbParam(m_fb_type, m_chapter, m_level);

		const TuituFbInfoConfig::Layer *layer_cfg = LOGIC_CONFIG->GetTuituFbConfig().GetLayerCfg(m_fb_type, m_chapter, m_level);
		if (NULL == layer_cfg)
		{
			this->DelayKickOutAllRole();
			return;
		}

		const TuituFbOtherConfig &other_cfg = LOGIC_CONFIG->GetTuituFbConfig().GetOtherCfg();

		for (int i = 0; i < layer_cfg->monster_count && i < TUITU_FB_GUANQIA_MONSTER_NUM_MAX; ++i)
		{
			Monster *monster = MonsterPool::Instance()->CreateMonster(layer_cfg->monster_list[i].monster_id, m_scene, layer_cfg->monster_list[i].pos);
			if (monster != NULL)
			{
				//达不到推荐战力，boss加buff
				if (monster->IsBossMonster())
				{
					if (role->GetCapability()->GetTotalCapability() < layer_cfg->capability)
					{
						EffectIncHurt *effect_inchurt = new EffectIncHurt(monster->GetId(), 0, layer_cfg->time_limit_1_star * 1000, 1, EffectBase::PRODUCT_METHOD_SYSTEM);
						effect_inchurt->SetIncPercent(other_cfg.inchurt_per);
						monster->AddEffect(effect_inchurt);

						EffectDecHurt *effect_dechurt = new EffectDecHurt(monster->GetId(), 0, layer_cfg->time_limit_1_star * 1000, 1, EffectBase::PRODUCT_METHOD_SYSTEM);
						effect_dechurt->SetDecPercent(other_cfg.mianshang_per);
						monster->AddEffect(effect_dechurt);
					}

					++m_total_boss_num;
					++m_cur_boss_num;
				}

				++m_total_monster_num;
				++m_cur_monster_num;
			}
		}

		m_owner_uid = role->GetUID();
		m_start_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	}

	this->SyncFBSceneLogicInfo(role);
}

void SpecialTuitu::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (!role->IsAlive())
	{
		static const int REALIVE_RECOVER_HP_PER = 10;
		m_die_timestamp = 0;
		role->ReAlive(REALIVE_RECOVER_HP_PER);
	}
}

bool SpecialTuitu::OnTimeout()
{
	this->OnFinish(0);
	return m_is_pass;
}

void SpecialTuitu::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE == dead->GetObjType())
	{
		m_die_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		this->KillAllMonster();
		this->OnFinish(0);
	}
	else if(Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = (Monster*)dead;

		if (m_cur_monster_num > 0)
			--m_cur_monster_num;

		if (monster->IsBossMonster() && m_cur_boss_num > 0)
			--m_cur_boss_num;

		Role *role = m_scene->GetRoleByUID(m_owner_uid);
		if (role != NULL)
		{
			//策划需求 boss死掉算次数但未算通关
			if (m_cur_boss_num <= 0)
			{
				role->GetTuituFb()->OnEnterFb(m_fb_type, m_chapter, m_level);
			}

			this->SyncFBSceneLogicInfo(role);
		}
	}
}

void SpecialTuitu::Reset()
{
	m_is_finish = false;
	m_is_pass = false;
	m_pass_star = 0;
	m_owner_uid = 0;
	m_fb_type = 0;
	m_chapter = 0;
	m_level = 0;
	m_start_timestamp = 0;
	m_total_boss_num = 0;
	m_cur_boss_num = 0;
	m_total_monster_num = 0;
	m_cur_monster_num = 0;
	fall_item_vec.clear();
}

void SpecialTuitu::OnReqNextLevel()
{
	if (!m_is_pass)
	{
		return;
	}

	Role *role = m_scene->GetRoleByUID(m_owner_uid);
	if (role != NULL)
	{
		if (!m_scene->ResetScene(role, false))
		{
			return;
		}

		if (!role->GetTuituFb()->GetNextLayerParam(m_fb_type, m_chapter, m_level))
		{
			return;
		}

		if (!role->GetTuituFb()->CanEnterFb(m_fb_type, m_chapter, m_level))
		{
			return;
		}

		const TuituFbInfoConfig::Layer* layer_cfg = LOGIC_CONFIG->GetTuituFbConfig().GetLayerCfg(m_fb_type, m_chapter, m_level);
		if (NULL == layer_cfg)
		{
			this->DelayKickOutAllRole();
			return;
		}

		this->AutoPickFallingItem(role);


		//同一场景，副本处理
		if (layer_cfg->scene_id == m_scene->GetSceneID())
		{
			const TuituFbOtherConfig &other_cfg = LOGIC_CONFIG->GetTuituFbConfig().GetOtherCfg();

			for (int i = 0; i < layer_cfg->monster_count && i < TUITU_FB_GUANQIA_MONSTER_NUM_MAX; ++i)
			{
				Monster *monster = MonsterPool::Instance()->CreateMonster(layer_cfg->monster_list[i].monster_id, m_scene, layer_cfg->monster_list[i].pos);

				if (monster != NULL)
				{
					//达不到推荐战力，boss加buff
					if (monster->IsBossMonster())
					{
						if (role->GetCapability()->GetTotalCapability() < layer_cfg->capability)
						{
							EffectIncHurt *effect_inchurt = new EffectIncHurt(monster->GetId(), 0, layer_cfg->time_limit_1_star, 1, EffectBase::PRODUCT_METHOD_SYSTEM);
							effect_inchurt->SetIncPercent(other_cfg.inchurt_per);
							monster->AddEffect(effect_inchurt);

							EffectDecHurt *effect_dechurt = new EffectDecHurt(monster->GetId(), 0, layer_cfg->time_limit_1_star, 1, EffectBase::PRODUCT_METHOD_SYSTEM);
							effect_dechurt->SetDecPercent(other_cfg.mianshang_per);
							monster->AddEffect(effect_dechurt);
						}

						++m_total_boss_num;
						++m_cur_boss_num;
					}

					++m_total_monster_num;
					++m_cur_monster_num;
				}
			}

			m_owner_uid = role->GetUID();
			m_start_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		}

		World::GetInstWorld()->GetSceneManager()->GoTo(role, layer_cfg->scene_id, role->GetUID(), layer_cfg->enter_pos);

		this->SyncFBSceneLogicInfo(role);
	}
}

void SpecialTuitu::OnAllMonsterDead()
{
	const TuituFbInfoConfig::Layer *layer_cfg = LOGIC_CONFIG->GetTuituFbConfig().GetLayerCfg(m_fb_type, m_chapter, m_level);
	if (NULL == layer_cfg)
	{
		return;
	}

	int star = 0;
	const int cost_time = static_cast<int>(static_cast<unsigned int>(EngineAdapter::Instance().Time()) - m_start_timestamp);
	if (cost_time < layer_cfg->time_limit_3_star)
	{
		star = 3;
	}
	else if (cost_time < layer_cfg->time_limit_2_star)
	{
		star = 2;
	}
	else if (cost_time < layer_cfg->time_limit_1_star)
	{
		star = 1;
	}

	this->OnFinish(star);
}

void SpecialTuitu::OnFinish(int star)
{
	if (m_is_finish) return;

	Role *role = m_scene->GetRoleByUID(m_owner_uid);
	if (NULL == role) return;

	this->AutoPickFallingItem(role);

	m_is_finish = true;
	m_is_pass = star > 0 ? true : false;
	m_pass_star = star;

	Protocol::SCTuituFbResultInfo ttfri;

	ttfri.star = star;
	ttfri.reserve_ch = 0;
	ttfri.item_count = 0;

	if (m_is_pass)
	{
		for (std::vector<ItemConfigData>::iterator it = fall_item_vec.begin(); it != fall_item_vec.end(); ++it)
		{
			ttfri.reward_item_list[ttfri.item_count].item_id = it->item_id;
			ttfri.reward_item_list[ttfri.item_count].num = it->num;
			ttfri.reward_item_list[ttfri.item_count].is_bind = it->is_bind ? 1 : 0;
			++ttfri.item_count;

			if (ttfri.item_count >= Protocol::SCTuituFbResultInfo::MAX_REWARD_ITEM_NUM)
			{
				break;
			}
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&ttfri, sizeof(ttfri));

	this->SyncFBSceneLogicInfo(role);

	if (role->IsAlive())
	{
		if (!m_is_pass)
		{
			this->DelayKickOutAllRole();
		}
		else
		{
			role->Recover();
		}
	}

	role->GetTuituFb()->OnFbFinish(m_fb_type, m_chapter, m_level, star);
}

void SpecialTuitu::SyncFBSceneLogicInfo(Role *role)
{
	if (NULL == role) return;

	Protocol::SCFBSceneLogicInfo fbsli; fbsli.reset();
	
	fbsli.time_out_stamp = m_scene->GetSceneTimeOutStamp();
	fbsli.scene_type = m_scene->GetSceneType();
	fbsli.is_finish = m_is_finish ? 1 :0;
	fbsli.is_pass = m_is_pass ? 1 :0;

	fbsli.total_boss_num = m_total_boss_num;
	fbsli.total_allmonster_num = m_total_monster_num;
	fbsli.kill_boss_num = m_total_boss_num - m_cur_boss_num;
	fbsli.kill_allmonster_num = m_total_monster_num - m_cur_monster_num;
	fbsli.pass_time_s = this->GetPassTime();
	fbsli.param1 = m_fb_type;
	fbsli.param2 = m_chapter;
	fbsli.param3 = m_level;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fbsli, sizeof(fbsli));
}