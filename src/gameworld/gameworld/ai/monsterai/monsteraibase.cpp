#include "monsteraibase.hpp"

#include "globalconfig/globalconfig.h"
#include "scene/scene.h"
#include "scene/map.h"
#include "gamecommon.h"
#include "obj/character/monster.h"

#include "config/logicconfigmanager.hpp"
#include "skill/bossskillconditionconfig.hpp"
#include "skill/bossskillconditionpool.hpp"
#include "skill/skillbase.hpp"
#include "skill/skillpool.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"

#include "engineadapter.h"
#include "world.h"
#include "protocal/msgfight.h"

#include "scene/speciallogic/speciallogic.hpp"
#include "effect/sceneeffect/sceneeffect.hpp"
#include "friend/teaminfo.hpp"
#include <functional>

MonsterAIBase::MonsterAIBase(Character *me, int aitype)
	: AIBase(me), m_aitype(aitype), m_current_runtime(0), m_follow_interval(0), m_run_range(0), m_aoi_range(0),
	m_enemy(INVALID_OBJ_ID), m_is_out_range(false), m_birth_pos(0, 0),
	m_bossskill_cond_id(0), 
	m_release_skillcond_hplow_per(false), m_release_skillcond_hplow_per_idx(-1),
	m_fight_time(0), m_born_time(0), m_has_dps_drop(false), m_top_hurt_user_id(INVALID_USER_ID), m_top_hurt(0), m_dps_end_time(0), m_has_first_hit_drop(false),
	m_first_hit_user_uuid(INVALID_UNIQUE_USER_ID), m_last_check_hate_drive_time(0), m_last_auto_recover_time(0), m_need_search(false), m_search_enemy_interval(0),
	m_chaofenger_objid(INVALID_OBJ_ID), m_chaofeng_invalid_time(0), m_last_fight_time(0), m_dis_next_check_time(0), m_is_static(false), m_static_end_time(0)
{
}

MonsterAIBase::~MonsterAIBase()
{

}

void MonsterAIBase::Init(const MonsterAIParam &mp)
{
	const unsigned int MONSTER_FOLLOW_INTERVAL = GLOBALCONFIG->GetOtherConfig_Monster_Follow_Interval();
	const int FOLLOW_ADJUST_INTERVAL = 500;		// MONSTER_FOLLOW_INTERVAL在一个范围上下调整

	m_follow_interval = MONSTER_FOLLOW_INTERVAL;
	int rand_intervel = FOLLOW_ADJUST_INTERVAL - RandomNum(FOLLOW_ADJUST_INTERVAL * 2);
	if (m_follow_interval + rand_intervel > 0) m_follow_interval += rand_intervel;

	m_run_range = mp.run_range;
	m_aoi_range = mp.aoi_range;
	m_birth_pos = m_me->GetPos();

	m_bossskill_cond_id = mp.bossskill_cond_id;
}

void MonsterAIBase::OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp)
{
	m_last_fight_time = (unsigned int)EngineAdapter::Instance().Time();

	if (this->IsAttackWhenOnAttack(attacker))
	{
		m_enemy = attacker;
		this->StartAttack(m_enemy, false);
	}

	if (delta_hp < 0)
	{
		Role *attacker_role = m_me->GetScene()->GetPrincipal(attacker);
		if (NULL != attacker_role)
		{
			// 伤害列表
			long long &hurt = m_hurt_map[attacker_role->GetUserId()];
			hurt += (-delta_hp);

			if (m_has_dps_drop)
			{
				if (hurt > m_top_hurt || m_top_hurt_user_id == attacker_role->GetUserId())
				{
					m_top_hurt = hurt;
					m_dps_end_time = (unsigned int)EngineAdapter::Instance().Time() + CHANGE_DPS_TIME;
					if (m_top_hurt_user_id != attacker_role->GetUserId())
					{
						m_top_hurt_user_id = attacker_role->GetUserId();
					}
				}
			}
			// 首刀处理
			if (INVALID_UNIQUE_USER_ID == m_first_hit_user_uuid && m_has_first_hit_drop)
			{
				this->SetFirstHitUser(attacker_role->GetUniqueUserID());
			}
		}
	}

	if (0 != m_bossskill_cond_id)
	{
		// 检查血量释放技能
		if (!m_release_skillcond_hplow_per)
		{
			const BossSkillConditionConfig *bossskill_cond_cfg = LOGIC_CONFIG->GetBossSkillConditionPool().GetBossSkillConditionConfig(m_bossskill_cond_id);
			if (NULL != bossskill_cond_cfg)
			{
				const BossSkillTypeCond &typecond = bossskill_cond_cfg->m_typecond_list[BOSS_SKILL_COND_HPLOW_PER];
				if (typecond.count > 0)
				{
					long long curr_hp = m_me->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
					long long max_hp = m_me->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
					long long old_hp = curr_hp + (-delta_hp);

					int old_hp_per = static_cast<int>(max_hp > 0 ? old_hp * 100 / max_hp : 0);
					int hp_per = static_cast<int>(max_hp > 0 ? curr_hp * 100 / max_hp : 0);

					if (old_hp_per > hp_per)
					{
						for (int i = 0; i < typecond.count; i++)
						{
							const BossSkillCondParam &cond_param = typecond.lists[i];
							if (old_hp_per >= cond_param.cond_param_list[0] && hp_per < cond_param.cond_param_list[0])
							{
								 //printf("[DEBUG] set BOSS_SKILL_COND_HPLOW_PER flag, param=%d, hp_per %d->%d\n", cond_param.cond_param_list[0], old_hp_per, hp_per);

								m_release_skillcond_hplow_per = true; m_release_skillcond_hplow_per_idx = i;
								break;
							}
						}
					}
				}
			}
		}
	
		{
			// 更新仇恨和伤害
			const BossSkillConditionConfig *bossskill_cond_cfg = LOGIC_CONFIG->GetBossSkillConditionPool().GetBossSkillConditionConfig(m_bossskill_cond_id);
			if (NULL != bossskill_cond_cfg)
			{
				if (bossskill_cond_cfg->m_hate_drive && NULL != m_me->GetScene())
				{
					Role *attacker_role = m_me->GetScene()->GetPrincipal(attacker);

					if (NULL != attacker_role)
					{

						{
							// 仇恨列表
							if (INVALID_OBJ_ID == m_chaofenger_objid) // 有嘲讽者时，被打时不增加仇恨
							{
								long long hate = this->CalcHate(attacker_role, skill_id, -delta_hp);

								HateMapIt it = m_hate_drive_map.find(attacker_role->GetId());
								if (it != m_hate_drive_map.end() && it->second.user_id == attacker_role->GetUserId())
								{
									it->second.hate += hate;
								}
								else 
								{
									m_hate_drive_map[attacker_role->GetId()] = HateInfo(attacker_role->GetUserId(), hate);
								}
							}
						}
					}
				}
			}
		}
	}	
}

void MonsterAIBase::Update(unsigned long interval, unsigned int now_second)
{
	if (0 != m_bossskill_cond_id)
	{
		const BossSkillConditionConfig *bossskill_cond_cfg = LOGIC_CONFIG->GetBossSkillConditionPool().GetBossSkillConditionConfig(m_bossskill_cond_id);
		if (NULL != bossskill_cond_cfg)
		{
			if (bossskill_cond_cfg->m_hate_drive && this->IsHateDrive())
			{
				if (now_second >= m_last_check_hate_drive_time + 3)
				{
					m_last_check_hate_drive_time = now_second;

					if (!this->CheckHateDrive(true))
					{
						this->ClearBossInfo();
						return;
					}
				}
			}

			if (INVALID_OBJ_ID != m_enemy && SPECIAL_STATUS_TYPE_NONE == m_special_status.status_type)
			{
				int old_fight_time = m_fight_time; m_fight_time += static_cast<int>(interval);

				// printf("[DEBUG] fight_time=%d\n", m_fight_time);

				const BossSkillTypeCond &typecond = bossskill_cond_cfg->m_typecond_list[BOSS_SKILL_COND_FIGHTTIMES];
				for (int i = 0; i < typecond.count; i++)
				{
					const BossSkillCondParam &cond_param = typecond.lists[i];
					if (old_fight_time < cond_param.cond_param_list[0] && m_fight_time >= cond_param.cond_param_list[0])
					{
						//printf("[DEBUG] set BOSS_SKILL_COND_FIGHTTIMES flag, param=%d, fight_time=%d\n", cond_param.cond_param_list[0] / 1000, m_fight_time / 1000);

						if (cond_param.cond_param_skill_count > 0)
						{
							m_special_status.status_type = SPECIAL_STATUS_TYPE_PERFORM_SKILL_ANYWAY;
							m_special_status.skill_index = cond_param.cond_param_skill_index_list[0];
						}

						break;
					}
				}
			}

			int old_born_time = m_born_time; m_born_time += static_cast<int>(interval);

			if (SPECIAL_STATUS_TYPE_NONE == m_special_status.status_type)
			{
				const BossSkillTypeCond &typecond = bossskill_cond_cfg->m_typecond_list[BOSS_SKILL_COND_BORNTIME];
				for (int i = 0; i < typecond.count; i++)
				{
					const BossSkillCondParam &cond_param = typecond.lists[i];

					if (old_born_time < cond_param.cond_param_list[0] && m_born_time >= cond_param.cond_param_list[0])
					{
						//printf("[DEBUG] set BOSS_SKILL_COND_BORNTIME flag, param=%d, m_born_time=%d\n", cond_param.cond_param_list[0] / 1000, m_born_time / 1000);

						if (cond_param.cond_param_skill_count > 0)
						{
							m_special_status.status_type = SPECIAL_STATUS_TYPE_PERFORM_SKILL_ANYWAY;
							m_special_status.skill_index = cond_param.cond_param_skill_index_list[0];
						}
						
						break;
					}
				}				
			}

			if (m_last_fight_time > 0 && now_second >= m_last_fight_time + 7)
			{
				if (Obj::OBJ_TYPE_MONSTER == m_me->GetObjType())
				{
					Monster *monster = (Monster*)m_me;
					if (!monster->IsForbidRecover())
					{
						m_me->Recover();
					}
				}

				if (m_me->IsFullHp())
				{
					m_last_fight_time = 0;
				}
			}
		}
	}
	
	//时刻自动回血
	{
		if (m_me != NULL && Obj::OBJ_TYPE_MONSTER == m_me->GetObjType() && m_me->IsAlive() && INVALID_USER_ID == m_top_hurt_user_id)
		{
			Monster *monster = (Monster*)m_me;

			if (monster->GetAutoRecoverPer() > 0 && 
				monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP) < monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) &&
				now_second >= m_last_auto_recover_time + monster->GetAutoRecoverTimeGap())
			{
				m_last_auto_recover_time = now_second;
				Attribute maxhp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				int recoverhp = monster->GetAutoRecoverPer();
				Attribute addhp = static_cast<Attribute>((double)maxhp * (double)recoverhp * 0.0001);
				addhp = addhp < 0 ? 1 : addhp;
				monster->ChangeHp(&addhp, true);
			}
		}
	}

	{
		if (INVALID_OBJ_ID != m_chaofenger_objid && now_second >= m_chaofeng_invalid_time)
		{
			m_chaofenger_objid = INVALID_OBJ_ID;
			m_chaofeng_invalid_time = 0;
		}
	}

	// dps切换
// 	if (m_dps_end_time > 0 && now_second > m_dps_end_time)
// 	{
// 		Role *old_dps_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_top_hurt_user_id);
// 		if (NULL != old_dps_role)
// 		{
// 			old_dps_role->SetTopDpsFlag(false);
// 		}
// 		m_hurt_map.erase(m_top_hurt_user_id);
// 
// 		m_top_hurt_user_id = INVALID_USER_ID;
// 		m_top_hurt = 0;
// 		m_dps_end_time = 0;
// 
// 		for (HurtMapIt iter = m_hurt_map.begin(); m_hurt_map.end() != iter; ++iter)
// 		{
// 			if (iter->second > m_top_hurt)
// 			{
// 				m_top_hurt_user_id = iter->first;
// 				m_top_hurt = iter->second;
// 				m_dps_end_time = now_second + CHANGE_DPS_TIME;
// 			}
// 		}
// 
// 		Role *dps_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_top_hurt_user_id);
// 		if (NULL != dps_role)
// 		{
// 			dps_role->SetTopDpsFlag(true);
// 		}
// 	}

	if (now_second > m_dis_next_check_time)
	{
		m_dis_next_check_time = now_second + DIS_CHECK_INTERVAL_TIME;

		for (HurtMapIt iter = m_hurt_map.begin(); m_hurt_map.end() != iter; ++iter)
		{	
			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(iter->first);
			if (NULL != role && NULL != role->GetScene() && NULL != m_me->GetScene())
			{
				unsigned int x_dis = abs(role->GetPos().x - m_me->GetPos().x);
				unsigned int y_dis = abs(role->GetPos().y - m_me->GetPos().y);
				if (role->GetScene()->GetIndex() != m_me->GetScene()->GetIndex() || (x_dis > m_run_range || y_dis > m_run_range))
				{
					iter->second = 0;
				}
			}
		}

		// 归属者要单独处理
		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(m_top_hurt_user_id);
		if (NULL != role && NULL != role->GetScene() && NULL != m_me->GetScene())
		{	
			unsigned int x_dis = abs(role->GetPos().x - m_me->GetPos().x);
			unsigned int y_dis = abs(role->GetPos().y - m_me->GetPos().y);
			if (role->GetScene()->GetIndex() != m_me->GetScene()->GetIndex() || (x_dis > m_run_range || y_dis > m_run_range))
			{
				m_hurt_map.erase(m_top_hurt_user_id);

				m_top_hurt_user_id = INVALID_USER_ID;
				m_top_hurt = 0;

				for (HurtMapIt iter = m_hurt_map.begin(); m_hurt_map.end() != iter; ++iter)
				{
					if (iter->second > m_top_hurt)
					{
						m_top_hurt_user_id = iter->first;
						m_top_hurt = iter->second;
					}
				}
			}
		}

		//首刀掉落主人检测
		if (Obj::OBJ_TYPE_MONSTER == m_me->GetObjType())
		{
			Monster* monster = (Monster*)m_me;
			if (MonsterInitParam::MONSTER_TYPE_BOSS == monster->GetMonsterType())
			{
				//不在同一场景或者离线
				Role* first_hit_role = monster->GetScene()->GetRoleByUUID(m_first_hit_user_uuid);

				if (NULL == first_hit_role)
				{
					this->SetFirstHitUser(INVALID_UNIQUE_USER_ID);
				}
				else if (first_hit_role->GetFirstHitHaveDead())
				{
					this->SetFirstHitUser(INVALID_UNIQUE_USER_ID);
				}
				else
				{
					//const int FIRST_HIT_USER_RANGE = 60;
					unsigned int range = m_aoi_range + m_run_range;
					if ((first_hit_role->GetPos() - m_me->GetPos()).IsLongerThan(range))
					{
						this->SetFirstHitUser(INVALID_UNIQUE_USER_ID);
					}
				}
			}
		}
	}

	if (m_static_end_time > 0 && now_second > m_static_end_time)
	{
		m_static_end_time = 0;
		m_is_static = false;
	}
}

void MonsterAIBase::ForceSetEnemy(ObjID obj_id)
{
	if (obj_id != m_me->GetId())
	{
		m_enemy = obj_id;
		this->StartAttack(m_enemy, false);
	}
}

void MonsterAIBase::OnChaofeng(Role *enemy_role, long long hate)
{
	if (NULL != enemy_role)
	{
		const BossSkillConditionConfig *bossskill_cond_cfg = LOGIC_CONFIG->GetBossSkillConditionPool().GetBossSkillConditionConfig(m_bossskill_cond_id);
		if (NULL != bossskill_cond_cfg && bossskill_cond_cfg->m_hate_drive)
		{
			m_chaofenger_objid = enemy_role->GetId();						 // 记录嘲讽者
			m_chaofeng_invalid_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 10; // 10秒后失效

			long long max_hate = 0;

			for (HateMapIt it = m_hate_drive_map.begin(); it != m_hate_drive_map.end(); ++it)
			{
				if (it->second.hate > max_hate)
				{
					max_hate = it->second.hate;
				}
			}

			HateMapIt it = m_hate_drive_map.find(enemy_role->GetId());
			if (it != m_hate_drive_map.end() && it->second.user_id == enemy_role->GetUserId())
			{
				if (max_hate > 0)
				{
					it->second.hate = max_hate + 1;
				}
				else
				{
					it->second.hate += hate;
				}				
			}
			else 
			{
				m_hate_drive_map[enemy_role->GetId()] = HateInfo(enemy_role->GetUserId(), hate);
			}
		}

		this->ForceSetEnemy(enemy_role->GetId());
	}	
}

struct HurtObj
{
	enum HURT_OBJ_TYPE
	{
		HURT_OBJ_TYPE_OBJ = 0,			// 玩家
		HURT_OBJ_TYPE_TEAM,				// 队伍
	};

	HurtObj() : obj_type(HURT_OBJ_TYPE_OBJ), obj_id(INVALID_OBJ_ID) {}
	HurtObj(int type, int id) : obj_type(type), obj_id(id) {}

	bool operator < (const HurtObj &r) const
	{
		return (obj_type < r.obj_type) || (obj_type == r.obj_type && obj_id < r.obj_id);
	}

	int obj_type;
	int obj_id;
};

struct HurtInfo
{
	HurtInfo() : hurt(0) {}

	long long hurt;
	RewarderUnit rewarder_unit;
};

typedef std::map<HurtObj, HurtInfo> HurtInfoMap;
typedef std::map<HurtObj, HurtInfo>::iterator HurtInfoMapIt;

void MonsterAIBase::GetTopHurtRewarder(RewarderUnit *top_reward_unit, RewarderUnit rand_reward_unit_list[BOSS_RAND_REWARD_RAND_COUNT])
{
	HurtInfoMap tmp_hurt_info_map;

	{
		// 1. 对伤害列表中的角色按有无队伍进行归并
		for (HurtMapIt iter = m_hurt_map.begin(); m_hurt_map.end() != iter; ++iter)
		{
			UserID user_id = iter->first;
			long long user_hurt = iter->second;

			Role *hurt_role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
			if (NULL != hurt_role && NULL != hurt_role->GetScene() && NULL != m_me->GetScene() &&
				hurt_role->GetScene()->GetIndex() == m_me->GetScene()->GetIndex())
			{
				if (hurt_role->GetTeamInfo()->InTeam())									// 角色有队伍 则加入到队伍做key的映射中
				{
					HurtObj key(HurtObj::HURT_OBJ_TYPE_TEAM, hurt_role->GetTeamInfo()->GetTeamIndex());

					HurtInfoMapIt hurt_iter = tmp_hurt_info_map.find(key);
					if (hurt_iter != tmp_hurt_info_map.end())
					{
						hurt_iter->second.hurt += user_hurt;
						hurt_iter->second.rewarder_unit.AddObj(hurt_role->GetId());
					}
					else
					{
						HurtInfo hurt_info;
						hurt_info.hurt = user_hurt;
						hurt_info.rewarder_unit.AddObj(hurt_role->GetId());

						tmp_hurt_info_map[key] = hurt_info;
					}
				}
				else																	// 角色无队伍 则加入到角色做key的映射中
				{
					HurtObj key(HurtObj::HURT_OBJ_TYPE_OBJ, hurt_role->GetId());

					HurtInfo hurt_info;
					hurt_info.hurt = user_hurt;
					hurt_info.rewarder_unit.AddObj(hurt_role->GetId());

					tmp_hurt_info_map[key] = hurt_info;
				}
			}
		}

		if (tmp_hurt_info_map.empty())
		{
			return;
		}
	}

	{
		// 2. 从归并后的列表选出第1名单位和2-9名的随机单位
	
		typedef std::map<long long, RewarderUnit, std::greater<long long> > HurtRewarderMap;
		typedef std::map<long long, RewarderUnit, std::greater<long long> >::iterator HurtRewarderMapIt;
		HurtRewarderMap hurt_valid_target_map; //int hurt_key = 99999999;

		{
			// 排序
			for (HurtInfoMapIt iter = tmp_hurt_info_map.begin(); tmp_hurt_info_map.end() != iter; ++iter)
			{
				//long long key = TranslateInt64(iter->second.hurt, --hurt_key);
				long long key = iter->second.hurt;
				hurt_valid_target_map[key] = iter->second.rewarder_unit;
			}
		}

		{
			// 第1
			HurtRewarderMapIt top_iter = hurt_valid_target_map.begin();
			if (top_iter != hurt_valid_target_map.end() && NULL != top_reward_unit)
			{
				*top_reward_unit = top_iter->second;
			}
		}

		{
			// 第2-9随机
			UNSTD_STATIC_CHECK(BOSS_RAND_REWARD_RAND_RANGE > BOSS_RAND_REWARD_RAND_COUNT + 1);

			for (int i = 0; i < BOSS_RAND_REWARD_RAND_COUNT; ++i) rand_reward_unit_list[i].Clear();

			if (hurt_valid_target_map.size() > 1)
			{
				hurt_valid_target_map.erase(hurt_valid_target_map.begin());

				int rand_max = ((int)hurt_valid_target_map.size() > BOSS_RAND_REWARD_RAND_RANGE - 1 ? (BOSS_RAND_REWARD_RAND_RANGE - 1) : (int)hurt_valid_target_map.size());
				for (int i = 0; i < BOSS_RAND_REWARD_RAND_COUNT && rand_max > 0; ++ i)
				{
					int rand_index = RandomNum(rand_max);
					if (rand_index >= 0 && rand_index < (int)hurt_valid_target_map.size())
					{
						HurtRewarderMapIt it = hurt_valid_target_map.begin();
						std::advance(it, rand_index);

						rand_reward_unit_list[i] = it->second;

						hurt_valid_target_map.erase(it);
						-- rand_max;
					}
				}
			}
		}
	}
}

UniqueUserID MonsterAIBase::GetFirstHitUser()
{
	return m_first_hit_user_uuid;
}

void MonsterAIBase::SetFirstHitUser(const UniqueUserID &user_uuid)
{
	if (m_first_hit_user_uuid != user_uuid)
	{
		if (Obj::OBJ_TYPE_MONSTER == m_me->GetObjType())
		{
			Monster *monster = (Monster *)m_me;

			//之前的第一刀用户清除状态
			if (m_first_hit_user_uuid != INVALID_UNIQUE_USER_ID)
			{
				Role* old_first_hit_role = monster->GetScene()->GetRoleByUUID(m_first_hit_user_uuid);
				if (old_first_hit_role != nullptr)
				{
					old_first_hit_role->SetFirstHitFlag(false);
				}
			}

			m_first_hit_user_uuid = user_uuid;
			monster->BroadcastFirstHitInfo();

			//新的首刀用户设置状态
			if (m_first_hit_user_uuid != INVALID_UNIQUE_USER_ID)
			{
				Role* new_first_hit_role = monster->GetScene()->GetRoleByUUID(m_first_hit_user_uuid);
				if (new_first_hit_role != nullptr)
				{
					new_first_hit_role->SetFirstHitFlag(true);
				}
			}
		}
	}
}

Role * MonsterAIBase::RandGetHurtRole()
{
	const int MAX_RAND_COUNT = 20;
	int rand_count = 0;
	Role *role_list[MAX_RAND_COUNT];
	memset(role_list, 0, sizeof(role_list));

	for (HurtMapIt iter = m_hurt_map.begin(); m_hurt_map.end() != iter && rand_count < MAX_RAND_COUNT; ++iter)
	{
		UserID user_id = iter->first;

		Role *hurt_role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
		if (NULL != hurt_role && NULL != hurt_role->GetScene() && NULL != m_me->GetScene() &&
			hurt_role->GetScene()->GetIndex() == m_me->GetScene()->GetIndex())
		{
			role_list[rand_count ++] = hurt_role;
		}
	}

	if (rand_count > 0 && rand_count <= MAX_RAND_COUNT)
	{
		int rand_val = RandomNum(rand_count);
		return role_list[rand_val];
	}

	return NULL;
}

Role *MonsterAIBase::GetTopHurtRole()
{
	UserID top_uid = INVALID_USER_ID; long long top_hurt = 0;

	for (HurtMapIt iter = m_hurt_map.begin(); m_hurt_map.end() != iter; ++iter)
	{
		if (iter->second > top_hurt)
		{
			top_hurt = iter->second;
			top_uid = iter->first;
		}
	}

	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(top_uid);
	return role;
}

bool MonsterAIBase::GetRewarderByHurtMoreThen(int hurt, int max_list_count, int *list_count, UserID *userid_list)
{
	if (hurt < 0 || max_list_count <= 0 || NULL == list_count || NULL == userid_list) return false;

	int count = 0;

	for (HurtMapIt iter = m_hurt_map.begin(); m_hurt_map.end() != iter; ++iter)
	{
		if (count >= max_list_count) break;

		UserID user_id = iter->first;
		long long user_hurt = iter->second;

		if (user_hurt >= hurt)
		{
			Role *hurt_role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
			if (NULL != hurt_role && NULL != hurt_role->GetScene() && NULL != m_me->GetScene() &&
				hurt_role->GetScene()->GetIndex() == m_me->GetScene()->GetIndex())
			{
				userid_list[count++] = user_id;
			}
		}
	}

	*list_count = count;

	return true;
}

bool MonsterAIBase::IsEnemy(Obj *obj)
{
	if (obj->GetObjType() != Obj::OBJ_TYPE_MONSTER && Obj::IsCharactor(obj->GetObjType()))
	{
		Character *cha = (Character*)obj;
		if (cha->IsAlive())
		{
			Role *role = NULL;

			if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
			{
				role = (Role *)obj;
			}
			
			if (NULL != role)
			{
				if (Role::MOVE_MODE_FLY == role->GetMoveMode()) 
				{
					return false;
				}

				if (Obj::OBJ_TYPE_MONSTER == m_me->GetObjType())
				{
					Monster *monster = (Monster*)m_me;
					char my_camp = GLOBALCONFIG->GetCampOfMonster(monster->GetMonsterId());

					if (CAMP_TYPE_INVALID != my_camp)	// 我是阵营神将
					{
						if (my_camp == role->GetCamp())
						{
							return false;
						}
					}
				}

				return true;
			}
		}
	}

	return false;
}

bool MonsterAIBase::IsFriend(Obj *obj, bool check_alive)
{
	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster *)obj;
		return (!check_alive || monster->IsAlive());
	}

	return false;
}

void MonsterAIBase::StartAttack(ObjID enemy, bool perform_skill)
{
	m_need_search = true;

	m_me->StartAttack(enemy);

	if (perform_skill)
	{
		this->SetNextSkill(enemy);
	}	
}

void MonsterAIBase::SetNextSkill(ObjID enemy)
{
	if (!m_me->CheckGlobalCD()) return;

	/************************************************************************/
	/* 这里的逻辑是满足条件的技能轮流尝试释放，都不成功则放默认技能 */
	/************************************************************************/

	if (0 != m_bossskill_cond_id)
	{
		/*if (0 == m_fight_time)
		{
			if (NULL != m_me->GetScene() && Scene::IsPersonalScene(m_me->GetScene()->GetSceneType(), m_me->GetScene()->GetSceneID()))
			{
				static Protocol::SCMonsterFightStatus mfs;
				mfs.obj_id = m_me->GetId();
				mfs.fight_status = Protocol::SCMonsterFightStatus::FIGHT_STATUS_START_FIGHT;
				m_me->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_me->GetObserHandle(), &mfs, sizeof(mfs));
			}
		}*/

		// hp低于某值
		if (m_release_skillcond_hplow_per)
		{
			const BossSkillConditionConfig *bossskill_cond_cfg = LOGIC_CONFIG->GetBossSkillConditionPool().GetBossSkillConditionConfig(m_bossskill_cond_id);
			if (NULL != bossskill_cond_cfg)
			{
				const BossSkillTypeCond &typecond = bossskill_cond_cfg->m_typecond_list[BOSS_SKILL_COND_HPLOW_PER];
				if (m_release_skillcond_hplow_per_idx >= 0 && m_release_skillcond_hplow_per_idx < typecond.count)
				{
					const BossSkillCondParam &cond_param = typecond.lists[m_release_skillcond_hplow_per_idx];
					if (this->PerformNextSkillFromList(cond_param.cond_param_skill_count, &cond_param.cond_param_skill_index_list[0]))
					{
						 //printf("[DEBUG] Perform for BOSS_SKILL_COND_HPLOW_PER param[%d]!\n", cond_param.cond_param_list[0]);

						m_release_skillcond_hplow_per = false; m_release_skillcond_hplow_per_idx = -1;
						return;
					}
				}
			}
		}

		{
			// hp处于某范围
			const BossSkillConditionConfig *bossskill_cond_cfg = LOGIC_CONFIG->GetBossSkillConditionPool().GetBossSkillConditionConfig(m_bossskill_cond_id);
			if (NULL != bossskill_cond_cfg)
			{
				const BossSkillTypeCond &typecond = bossskill_cond_cfg->m_typecond_list[BOSS_SKILL_COND_HPLOW_RANGE];
				for (int i = 0; i < typecond.count; i++)
				{
					UNSTD_STATIC_CHECK(BOSS_SKILL_PARAM_COUNT >= 2);

					const BossSkillCondParam &cond_param = typecond.lists[i];

					long long curr_hp = m_me->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
					long long max_hp = m_me->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);

					int per = static_cast<int>(max_hp > 0 ? curr_hp * 100 / max_hp : 100);
					if (per >= cond_param.cond_param_list[0] && per < cond_param.cond_param_list[1])
					{
						if (this->PerformNextSkillFromList(cond_param.cond_param_skill_count, &cond_param.cond_param_skill_index_list[0]))
						{
							//printf("[DEBUG] Perform for BOSS_SKILL_COND_HPLOW_RANGE param[%d, %d), hp_per=%d\n", cond_param.cond_param_list[0], cond_param.cond_param_list[1], per);

							return;
						}
					}
				}
			}
		}

		{
			// 无条件释放
			const BossSkillConditionConfig *bossskill_cond_cfg = LOGIC_CONFIG->GetBossSkillConditionPool().GetBossSkillConditionConfig(m_bossskill_cond_id);
			if (NULL != bossskill_cond_cfg)
			{
				const BossSkillTypeCond &typecond = bossskill_cond_cfg->m_typecond_list[BOSS_SKILL_COND_ANYWAY];
				for (int i = 0; i < typecond.count; i++)
				{
					const BossSkillCondParam &cond_param = typecond.lists[i];
					if (this->PerformNextSkillFromList(cond_param.cond_param_skill_count, &cond_param.cond_param_skill_index_list[0]))
					{
						//printf("[DEBUG] Perform for BOSS_SKILL_COND_ANYWAY \n");

						return;
					}
				}
			}
		}
	}
	
	int default_skill_index = m_me->GetSkillManager()->GetDefaultSkill();
	this->PerformSkillByIndex(default_skill_index, SKILL_PHASE_READING);
}

bool MonsterAIBase::PerformNextSkillFromList(int count, const int *skill_index_list)
{
	for (int i = 0; i < count; i++)
	{
		int skill_index = skill_index_list[i];

		if (this->PerformSkillByIndex(skill_index, SKILL_PHASE_READING))
		{
			return true;
		}
	}

	return false;
}

bool MonsterAIBase::CheckHateDrive(bool is_update)
{
	const BossSkillConditionConfig *bossskill_cond_cfg = LOGIC_CONFIG->GetBossSkillConditionPool().GetBossSkillConditionConfig(m_bossskill_cond_id);
	if (NULL != bossskill_cond_cfg && bossskill_cond_cfg->m_hate_drive && NULL != m_me->GetScene())
	{
		long long enemy_hate = 0, valid_top_hate = 0;
		ObjID valid_top_objid = INVALID_OBJ_ID;

		for (HateMapIt it = m_hate_drive_map.begin(); it != m_hate_drive_map.end();)
		{
			ObjID hate_target_objid = it->first;

			Obj *obj = m_me->GetScene()->GetObj(hate_target_objid);
			if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
			{
				Role *role = (Role*)obj;
				if (role->GetUserId() == it->second.user_id)
				{
					if (role->IsAlive() && !(obj->GetPos()-m_birth_pos).IsLongerThan(m_run_range) && it->second.hate > valid_top_hate)
					{
						valid_top_objid = hate_target_objid; 
						valid_top_hate = it->second.hate;
					}
					
					if (m_enemy == hate_target_objid) 
					{
						enemy_hate = it->second.hate;
					}

					++ it;
				}
				else 
				{
					m_hate_drive_map.erase(it++);
				}
			}
			else 
			{
				m_hate_drive_map.erase(it++);
			}
		}

		//printf("[DEBUG] CheckHateDrive: m_enemy=%d enemy_hate=%d,  valid_top_objid=%d valid_top_hate=%d\n", m_enemy, enemy_hate, valid_top_objid, valid_top_hate);

		if (m_enemy != INVALID_OBJ_ID && valid_top_objid != INVALID_OBJ_ID && m_enemy != valid_top_objid && valid_top_hate > enemy_hate)
		{
			int morethan_hurt = (int)(enemy_hate * 0.01f);
			if (valid_top_hate >= morethan_hurt + enemy_hate)
			{
				m_enemy = INVALID_OBJ_ID;
			}
		}

		// 当目标不再是嘲讽者时（如嘲讽者有可能下线），认为此次嘲讽失效
		if (INVALID_OBJ_ID != m_chaofenger_objid && m_chaofenger_objid != m_enemy)
		{
			m_chaofenger_objid = 0;
			m_chaofeng_invalid_time = 0;
		}

		// 这里设计上有bug，因为怪物仅仅保存了角色的仇恨，在仙盟密境里面有怪打怪
		// 怪本来是设置的攻击目标，但是怪对怪的伤害没有保存，所以怪又会继续打人 

		if (INVALID_OBJ_ID == m_enemy && INVALID_OBJ_ID != valid_top_objid)
		{
			//printf("[DEBUG] CheckHateDrive, Change enemy to %d\n", valid_top_objid);

			m_enemy = valid_top_objid;
			this->StartAttack(m_enemy, false);
		}

		/*if (is_update && Obj::OBJ_TYPE_MONSTER == m_me->GetObjType())
		{
			Monster *monster = (Monster*)m_me;
			if (monster->IsWorldBossMonster())
			{
				int count = 1;
				for (HateMapIt it = m_hate_drive_map.begin(); it != m_hate_drive_map.end() && count <= 10; ++it, ++count)
				{
					ObjID hate_target_objid = it->first;
					Obj *obj = m_me->GetScene()->GetObj(hate_target_objid);
					if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
					{
						Role *role = (Role*)obj;

						gamelog::g_log_strengthen_test.printf(LL_INFO, "MeObj:%d scene:%d TOP(%d) user[%s %d] obj:%d hate:%d",
							m_me->GetId(), m_me->GetScene()->GetSceneID(), count,
							role->GetName(), UidToInt(role->GetUserId()), role->GetId(),
							it->second.hate);
					}
				}

				Obj *target_obj = m_me->GetScene()->GetObj(m_enemy);
				if (NULL != target_obj && Obj::OBJ_TYPE_ROLE == target_obj->GetObjType())
				{
					Role *target_role = (Role*)target_obj;

					gamelog::g_log_strengthen_test.printf(LL_INFO, "MeObj:%d scene:%d TARGET user[%s %d] obj:%d",
						m_me->GetId(), m_me->GetScene()->GetSceneID(), 
						target_role->GetName(), UidToInt(target_role->GetUserId()), target_role->GetId());
				}
				else
				{
					gamelog::g_log_strengthen_test.printf(LL_INFO, "MeObj:%d scene:%d TARGET NULL obj:%d",
						m_me->GetId(), m_me->GetScene()->GetSceneID(), m_enemy);
				}

				gamelog::g_log_strengthen_test.printf(LL_INFO, "-----------------------");
			}
		}*/

		return (INVALID_OBJ_ID != m_enemy);
	}
	
	return false;
}

void MonsterAIBase::ClearBossInfo()
{
	/*if (m_fight_time > 0)
	{
		if (NULL != m_me->GetScene() && Scene::IsPersonalScene(m_me->GetScene()->GetSceneType(), m_me->GetScene()->GetSceneID()))
		{
			static Protocol::SCMonsterFightStatus mfs;
			mfs.obj_id = m_me->GetId();
			mfs.fight_status = Protocol::SCMonsterFightStatus::FIGHT_STATUS_LEAVE_FIGHT;
			m_me->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_me->GetObserHandle(), &mfs, sizeof(mfs));
		}
	}*/

	m_release_skillcond_hplow_per = false; m_release_skillcond_hplow_per_idx = -1;
	m_fight_time = 0; 
	m_born_time = 0; 

	m_hate_drive_map.clear();
	m_hurt_map.clear();
	this->SetFirstHitUser(INVALID_UNIQUE_USER_ID);

	if (NULL != m_me->GetScene())
	{
// 		if (m_aitype !=  MonsterInitParam::AITYPE_SPECIALAI_TERRITORYWAR_TOWER) // 领土战的防御塔无敌状态不能在没有仇恨驱动时就清掉
// 		{
// 			m_me->ClearFightBuffEffect(EffectBase::CLEAR_REASON_SKILL);
// 		}

		m_me->ClearAllDeBuffEffect();
	}

	if (Obj::OBJ_TYPE_MONSTER == m_me->GetObjType())
	{
		Monster *monster = (Monster*)m_me;
		monster->SetBossMonsterBuff();

		monster->GetSkillManager()->ClearAllCD();
	}
}

void MonsterAIBase::OnEnemyLost()
{
	if (0 != m_bossskill_cond_id)
	{
		this->ClearBossInfo();
	}
}

bool MonsterAIBase::CanStartAttack()
{
	if (NULL == m_me->GetScene())
	{
		return false;
	}

	Obj *obj = m_me->GetScene()->GetObj(m_enemy);
	if (NULL == obj || !Obj::IsCharactor(obj->GetObjType()))
	{
		m_enemy = INVALID_OBJ_ID;
		m_me->StopAttack();
		return false;
	}

	Character *target = (Character*)obj;
	if (!m_me->IsEnemy(target))
	{
		m_enemy = INVALID_OBJ_ID;
		m_me->StopAttack();
		return false;
	}

	if (!target->IsAlive())
	{
		m_enemy = INVALID_OBJ_ID;
		m_me->StopAttack();
		return false;
	}

	if (!m_me->GetScene()->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, m_me->GetPos().x, m_me->GetPos().y))
	{
		m_me->StopAttack();
		return false;
	}

	if (!m_me->CanCharacterAction())
	{
		m_me->StopAttack();
		return false;
	}

	Posi target_pos = target->GetPos();
	return m_me->CheckNextSkillDistance(target, &target_pos);
}

bool MonsterAIBase::IsOutRange()
{
	bool outof_range = (m_me->GetPos()-m_birth_pos).IsLongerThan(m_run_range);
	if (outof_range)
	{
		//if (NULL != m_bossskill_cond_cfg && m_bossskill_cond_cfg->m_goback) // 设置无敌 超出距离 并且设置返回的情况下 满血满魔
		//{
		//	// printf("[DEBUG] OutOfRange, ClearBossInfo!\n");

		//	this->AttachWudiState();
		//	this->ClearBossInfo();
		//}
	}

	return outof_range;
}

void MonsterAIBase::Follow(ObjID obj_id, unsigned long interval)
{
	static const int DIR_ADJUST_FACTOR = 60;		// 追踪角度调整系数

	UNSTD_STATIC_CHECK(DIR_ADJUST_FACTOR > 0);

	if (NULL == m_me->GetScene())
	{
		return;
	}

	Obj *obj = m_me->GetScene()->GetObj(obj_id);
	if (NULL == obj)
	{
		m_enemy = INVALID_OBJ_ID;
		return;
	}

	// 全局cd的时候不追人，防止动作没做完就移动
	if (!m_me->CheckGlobalCD())
	{
		return;
	}

	if (m_me->IsStatic())
	{
		Posi deltaP = obj->GetPos() - m_me->GetPos();
		Scalar distance = 0;

		Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
		if (0 == m_bossskill_cond_id) 
		{
			dir += (DIR_ADJUST_FACTOR * 1.0f - RandomNum(DIR_ADJUST_FACTOR * 2)) / 100;
		}

		m_me->SetDir(dir, distance);
		m_current_runtime = 0;
		return;
	}

	m_current_runtime += interval; 

	if (m_current_runtime >= m_follow_interval)
	{
		Posi deltaP = obj->GetPos() - m_me->GetPos();
		Scalar distance = 0;

		Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
		if (0 == m_bossskill_cond_id)
		{
			dir += (DIR_ADJUST_FACTOR * 1.0f - RandomNum(DIR_ADJUST_FACTOR * 2)) / 100;
		}

		if (m_me->GetDir() != dir)
		{
			m_me->SetDir(dir, distance);
		}

		m_current_runtime = 0;
		return;
	}
}

int MonsterAIBase::SearchNearEnemys(unsigned long interval, const Posi &pos, int range, ObjID *target_list, int max_num)
{
	if (!m_need_search) return 0;

	if (NULL == m_me->GetScene())
	{
		return 0;
	}

	range -= 2;		// 防止在临界范围突然离开的情况 搜索范围小于aoi_range
	
	int num = 0;

	if (0 == m_search_enemy_interval || m_search_enemy_interval >= 5000)
	{
		m_search_enemy_interval = 0;

		num = SkillBase::GetTargetByArea(m_me, pos, range, target_list, max_num);

		for (int i = 0; i < num; )
		{
			Obj *obj = m_me->GetScene()->GetObj(target_list[i]);
			if (NULL == obj || (pos - obj->GetPos()).IsLongerThan(range))
			{
				target_list[i] = target_list[num - 1];
				-- num;
			}
			else
			{
				++ i;
			}
		}
	}

	m_search_enemy_interval += interval;

	return num;
}

void MonsterAIBase::AttachWudiState(int dual_ms)
{
	if (dual_ms <= 0)
	{
		Posi deltaP = m_me->GetPos() - m_birth_pos;
		Scalar distance = 0; gamecommon::GetMoveDir(deltaP, &distance);
		Attribute move_speed = m_me->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED);
		dual_ms = static_cast<int>(move_speed > 0 ? (1000 * distance * 100.0f / move_speed) : 0);
	}

	if (dual_ms > 0)
	{
		EffectSpecialState *special_effect = new EffectSpecialState(INVALID_OBJ_ID, 0, dual_ms, EBT_WUDI, 1, EffectBase::PRODUCT_METHOD_SYSTEM);
		m_me->AddEffect(special_effect);
	}
}

void MonsterAIBase::OnDie(ObjID killer)
{
	if (0 != m_bossskill_cond_id)
	{
		const BossSkillConditionConfig *bossskill_cond_cfg = LOGIC_CONFIG->GetBossSkillConditionPool().GetBossSkillConditionConfig(m_bossskill_cond_id);
		if (NULL != bossskill_cond_cfg)
		{
			const BossSkillTypeCond &typecond = bossskill_cond_cfg->m_typecond_list[BOSS_SKILL_COND_ON_DIE];
			for (int i = 0; i < typecond.count; i++)
			{
				const BossSkillCondParam &cond_param = typecond.lists[i];

				if (this->PerformNextSkillFromList(cond_param.cond_param_skill_count, &cond_param.cond_param_skill_index_list[0]))
				{
					//printf("[DEBUG] Perform for BOSS_SKILL_COND_ON_DIE \n");

					return;
				}
			}
		}
	}

	if (INVALID_USER_ID != m_top_hurt_user_id)
	{
		Role *dps_role = World::GetInstWorld()->GetSceneManager()->GetRole(m_top_hurt_user_id);
		if (NULL != dps_role)
		{
			m_top_hurt_user_id = INVALID_USER_ID;
			m_top_hurt = 0;
			m_dps_end_time = 0;
		}
	}

	if (m_first_hit_user_uuid != INVALID_UNIQUE_USER_ID)
	{
		Role *first_hit_role = nullptr;
		Scene *cur_scene = m_me->GetScene();
		if (cur_scene)
		{
			first_hit_role = cur_scene->GetRoleByUUID(m_first_hit_user_uuid);
		}

		if (first_hit_role != nullptr)
		{
			first_hit_role->SetFirstHitFlag(false);
		}
	}
}

bool MonsterAIBase::PerformSkillByIndex(int skill_index, int skill_phase)
{
	SkillManager *skillmanager = m_me->GetSkillManager();

	const SkillManager::SkillItem *skill_item = skillmanager->GetSkillItem(skill_index);
	if (NULL != skill_item)
	{
		const SkillBase *skill_base = SKILLPOOL->GetSkill(skill_item->skill_id);
		if (NULL != skill_base)
		{
			if (SKILL_PHASE_READING == skill_phase)
			{
				if (!skillmanager->CheckSkillCoolDown(skill_index)) return false;

				if (skill_base->GetReadingS() > 0)
				{
					Posi target_pos; ObjID target_id = INVALID_OBJ_ID;
					if (!this->GetSkillTarget(skillmanager->CheckSkillTargetEnemy(skill_index), &target_pos, &target_id)) 
					{
						return false;
					}

					int check_flag = 0;
					check_flag |= SKILL_CHECK_FLAG_DISTANCE;
					check_flag |= SKILL_CHECK_FLAG_TARGET;

					if (m_me->PerformSkill(skill_index, target_pos, target_id, check_flag, SKILL_PHASE_READING))
					{
						m_last_fight_time = (unsigned int)EngineAdapter::Instance().Time();
						m_special_status.status_type = SPECIAL_STATUS_TYPE_READING;
						m_special_status.end_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + skill_base->GetReadingS();
						m_special_status.skill_index = skill_index;
						m_special_status.start_pos = m_me->GetPos();
						m_special_status.target_pos = target_pos;

						SceneEffectClient *scene_effect = new SceneEffectClient(m_me->GetScene(), m_me->GetId(), skill_base->GetSkillId(), EffectBase::PRODUCT_METHOD_SKILL_READDING);
						scene_effect->SetEffectParam(target_pos, m_me->GetPos(), m_special_status.end_time, skill_base->GetReadingS(), skill_base->GetAttackRange(skill_item->level));
						m_me->GetScene()->GetSpecialLogic()->AddSceneEffect(scene_effect);
						
						/*printf("[DEBUG] MonsterAIBase::PerformSkillByIndex, skill[%d] = %d reading end %d\n", 
							skill_index, skill_item->skill_id, m_special_status.end_time);*/

						return true;
					}
				}
				else
				{
					skill_phase = SKILL_PHASE_PERFORM;
				}
			}

			if (SKILL_PHASE_PERFORM == skill_phase)
			{
				Posi target_pos; ObjID target_id = INVALID_OBJ_ID;
				if (!this->GetSkillTarget(skillmanager->CheckSkillTargetEnemy(skill_index), &target_pos, &target_id))
				{
					if (SPECIAL_STATUS_TYPE_READING != m_special_status.status_type)
					{
						return false;
					}

					target_id = m_me->GetId();
				}

				// 读条技能目标坐标用一开始选择的
				if (SPECIAL_STATUS_TYPE_READING == m_special_status.status_type)
				{
					target_pos = m_special_status.target_pos;
				}

				int check_flag = 0;
				// 单体技能或者非读条技能才检查距离、敌人
				if (skill_base->GetAttackRangeEnemyNum(1) <= 1 || SPECIAL_STATUS_TYPE_READING != m_special_status.status_type)
				{
					check_flag |= SKILL_CHECK_FLAG_DISTANCE;
					check_flag |= SKILL_CHECK_FLAG_TARGET;
				}

				if (m_me->PerformSkill(skill_index, target_pos, target_id, check_flag, SKILL_PHASE_PERFORM))
				{
					m_last_fight_time = (unsigned int)EngineAdapter::Instance().Time();
					//printf("[DEBUG] MonsterAIBase::PerformSkillByIndex, skill[%d] = %d phase %d\n", skill_index, skill_item->skill_id, skill_phase);

					if (skill_base->GetContinuePerformS() > 0)
					{
						m_special_status.status_type = SPECIAL_STATUS_TYPE_CONTINUE_PERFORM;
						m_special_status.end_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + skill_base->GetContinuePerformS();
						m_special_status.skill_index = skill_index;
						m_special_status.status_param = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 1;
						m_special_status.start_pos = m_me->GetPos();

						/*printf("[DEBUG] MonsterAIBase::PerformSkillByIndex, skill[%d] = %d into continue end %d next %d\n", 
							skill_index, skill_item->skill_id, m_special_status.end_time, m_special_status.status_param);*/
					}
					else
					{
						m_special_status.Clear();
					}

					return true;
				}
			}
			else if (SKILL_PHASE_CONTINUE_PERFORM == skill_phase)
			{
				if (m_me->PerformSkill(skill_index, m_me->GetPos(), m_me->GetId(), 0, SKILL_PHASE_CONTINUE_PERFORM))
				{
					m_last_fight_time = (unsigned int)EngineAdapter::Instance().Time();
					m_special_status.status_param = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 1;

					//printf("[DEBUG] MonsterAIBase::PerformSkillByIndex, skill[%d] = %d continue next %d\n", skill_index, skill_item->skill_id, m_special_status.status_param);

					return true;
				}
			}
			else if (SKILL_PHASE_END == skill_phase)
			{
				if (m_me->PerformSkill(skill_index, m_me->GetPos(), m_me->GetId(), 0, SKILL_PHASE_END))
				{
					m_me->GetScene()->GetSpecialLogic()->DelSceneEffect(m_me->GetId(), skill_base->GetSkillId(), EffectBase::PRODUCT_METHOD_SKILL_READDING);
					//printf("[DEBUG] MonsterAIBase::PerformSkillByIndex, skill[%d] = %d skill_end\n", skill_index, skill_item->skill_id);

					return true;
				}
			}
		}
	}

	return false;
}

bool MonsterAIBase::CheckSpecialStatus()
{
	if (SPECIAL_STATUS_TYPE_NONE != m_special_status.status_type)
	{
		this->OnSpecialStatus();
		return false;
	}

	return true;
}

void MonsterAIBase::OnSpecialStatus()
{
	{
		// 这是可以被打断的状态
		if (SPECIAL_STATUS_TYPE_READING == m_special_status.status_type ||
			SPECIAL_STATUS_TYPE_CONTINUE_PERFORM == m_special_status.status_type)
		{
			if (m_me->IsXuanYun() || m_me->IsChenMo() || 
				m_special_status.start_pos.x != m_me->GetPos().x || m_special_status.start_pos.y != m_me->GetPos().y)
			{
				this->PerformSkillByIndex(m_special_status.skill_index, SKILL_PHASE_END);

				m_special_status.Clear();
				return;
			}
		}
	}
	
	switch (m_special_status.status_type)
	{
	case SPECIAL_STATUS_TYPE_READING:
		{
			unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			if (now_second >= m_special_status.end_time)
			{
				this->PerformSkillByIndex(m_special_status.skill_index, SKILL_PHASE_PERFORM);
			}
		}
		break;

	case SPECIAL_STATUS_TYPE_CONTINUE_PERFORM:
		{
			unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			if (now_second >= m_special_status.end_time)
			{
				this->PerformSkillByIndex(m_special_status.skill_index, SKILL_PHASE_END);
				m_special_status.Clear();
			}
			else
			{
				if (now_second >= static_cast<unsigned int>(m_special_status.status_param))
				{
					this->PerformSkillByIndex(m_special_status.skill_index, SKILL_PHASE_CONTINUE_PERFORM);
				}
			}
		}
		break;

	case SPECIAL_STATUS_TYPE_PERFORM_SKILL_ANYWAY:
		{
			this->PerformSkillByIndex(m_special_status.skill_index, SKILL_PHASE_READING);
			if (SPECIAL_STATUS_TYPE_READING != m_special_status.status_type)
			{
				m_special_status.Clear();
			}
		}
		break;
	}
}

void MonsterAIBase::CancleSpcialStaus()
{
	if (NULL == m_me || !m_me->IsAlive())
	{
		return;
	}

	if (Obj::OBJ_TYPE_MONSTER == m_me->GetObjType() && ((Monster*)m_me)->IsWorldBossMonster())
	{
		return;
	}

	if (SPECIAL_STATUS_TYPE_READING == m_special_status.status_type ||
		SPECIAL_STATUS_TYPE_CONTINUE_PERFORM == m_special_status.status_type)
	{
		this->PerformSkillByIndex(m_special_status.skill_index, SKILL_PHASE_END);
		m_special_status.Clear();
	}
}

bool MonsterAIBase::GetSkillTarget(bool is_target_enemy, Posi *target_pos, ObjID *target_id)
{
	if (NULL == target_pos || NULL == target_id) return false;

	if (is_target_enemy)
	{
		Scene *scene = m_me->GetScene();
		if (NULL == scene) return false;

		Obj *target_obj = scene->GetObj(m_enemy);				// 敌人是指本怪锁定的目标
		if (NULL == target_obj) return false;

		*target_id = m_enemy;
		*target_pos = target_obj->GetPos();
	}
	else
	{
		*target_id = m_me->GetId();
		*target_pos = m_me->GetPos();
	}

	return true;
}

void MonsterAIBase::OnRoleDie(ObjID obj_id)
{
	if (0 != m_bossskill_cond_id)
	{
		const BossSkillConditionConfig *bossskill_cond_cfg = LOGIC_CONFIG->GetBossSkillConditionPool().GetBossSkillConditionConfig(m_bossskill_cond_id);
		if (NULL != bossskill_cond_cfg)
		{
			const BossSkillTypeCond &typecond = bossskill_cond_cfg->m_typecond_list[BOSS_SKILL_COND_NEAR_ROLE_DIE];
			for (int i = 0; i < typecond.count; i++)
			{
				const BossSkillCondParam &cond_param = typecond.lists[i];

				if (this->PerformNextSkillFromList(cond_param.cond_param_skill_count, &cond_param.cond_param_skill_index_list[0]))
				{
					//printf("[DEBUG] Perform for BOSS_SKILL_COND_NEAR_ROLE_DIE \n");

					return;
				}
			}
		}
	}
}

long long MonsterAIBase::CalcHate(Role *attacker_role, UInt16 skill_id, long long deltahp)
{
	if (deltahp < 0)
	{
		deltahp = -deltahp;
	}

	long long hate = deltahp;

	if (NULL != attacker_role && NULL != attacker_role->GetScene() && NULL != attacker_role->GetScene()->GetSpecialLogic())
	{
		hate = attacker_role->GetScene()->GetSpecialLogic()->SpecialRecalcHate(attacker_role, skill_id, hate);
	}	

	return hate;
}

void MonsterAIBase::ClearHate(ObjID obj_id)
{
	HateMapIt it = m_hate_drive_map.find(obj_id);
	if (it != m_hate_drive_map.end())
	{
		it->second.hate = 0;
	}

	if (NULL != m_me->GetScene())
	{
		Role *role = m_me->GetScene()->GetPrincipal(obj_id);
		if (NULL != role)
		{
			HurtMapIt iter = m_hurt_map.find(role->GetUserId());
			if (iter != m_hurt_map.end())
			{
				iter->second = 0;
			}
		}
	}
}
