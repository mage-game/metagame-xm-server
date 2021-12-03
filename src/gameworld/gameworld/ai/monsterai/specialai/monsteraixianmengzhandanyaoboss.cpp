#include "monsteraixianmengzhandanyaoboss.hpp"
#include "obj/character/monster.h"
#include "scene/scene.h"
#include "globalconfig/globalconfig.h"
#include "gamecommon.h"
#include "engineadapter.h"
#include "config/logicconfigmanager.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "skill/skillbase.hpp"

MonsterAIXianMengZhanDanYaoBoss::MonsterAIXianMengZhanDanYaoBoss(Character *me)
	: MonsterAIBase(me, MonsterInitParam::AITYPE_SPECIALAI_XIANMENGZHAN_DANYAOBOSS),
	m_aoi_handle(-1), m_aoi_range(2), m_aoi_cur_pos(0, 0), m_stop(false), m_guild_id(INVALID_GUILD_ID), 
	m_center_pos(0, 0), m_skill_id(0), m_skill_interval(0), m_next_perform_time(0), 
	m_atk_range(0), m_add_gongji_percent(0), m_add_fangyu_percent(0), m_buff_dur_ms(0), m_enemy_size(0)
{
	memset(m_enemy_list, 0 ,sizeof(m_enemy_list));
}

MonsterAIXianMengZhanDanYaoBoss::~MonsterAIXianMengZhanDanYaoBoss()
{
}

void MonsterAIXianMengZhanDanYaoBoss::Init(const MonsterAIParam &mp)
{
	MonsterAIBase::Init(mp);
	m_aoi_range = mp.aoi_range;
}

void MonsterAIXianMengZhanDanYaoBoss::OnEnterScene()
{
	m_aoi_cur_pos = m_me->GetPos();
	m_aoi_handle = m_me->GetScene()->GetZoneMatrix()->CreateCircleAOI(m_me->GetId(), m_birth_pos, m_aoi_range, m_aoi_range);
}

void MonsterAIXianMengZhanDanYaoBoss::OnLeaveScene()
{
	m_me->GetScene()->GetZoneMatrix()->DestroyAOI(m_aoi_handle);
	m_enemy = INVALID_OBJ_ID;
}

void MonsterAIXianMengZhanDanYaoBoss::OnAOIEnter(ObjID obj_id)
{
	Obj *obj = m_me->GetScene()->GetObj(obj_id);
	if (NULL == obj) return;

	if (m_me->IsEnemy(obj))
	{
		for (int i = 0; i < m_enemy_size; i++)
		{
			if (m_enemy_list[i] == obj_id) return;
		}

		if (m_enemy_size < MAX_ENEMY_NUM)
		{
			m_enemy_list[m_enemy_size] = obj_id;
			++ m_enemy_size;

			if (m_enemy == INVALID_OBJ_ID)
			{
				m_enemy = obj_id;
				StartAttack(m_enemy, false);
			}
		}
	}
}

void MonsterAIXianMengZhanDanYaoBoss::OnAOILeave(ObjID obj_id)
{
	Obj *obj = m_me->GetScene()->GetObj(obj_id);
	if (NULL == obj) return;

	if (m_me->IsEnemy(obj))
	{
		for (int i = 0; i < m_enemy_size; ++i)
		{
			if (m_enemy_list[i] == obj_id)
			{
				m_enemy_list[i] = m_enemy_list[m_enemy_size - 1];
				--m_enemy_size;

				if (m_enemy == obj_id)
				{
					m_me->StopAttack();
					m_enemy = INVALID_OBJ_ID;
				}
				break;
			}
		}
	}
}

void MonsterAIXianMengZhanDanYaoBoss::AI(unsigned long interval)
{
	if (!CheckSpecialStatus())
	{
		return;
	}

	switch (m_me->GetLogicResult())
	{
	case Character::MOVE_STATIC:
	case Character::ATTACK_NONE:
	case Character::LOGIC_NONE:
	case Character::MOVE_RUN:
		{
			unsigned int now_sec = (unsigned int)(EngineAdapter::Instance().Time());

			if (!m_stop)		// 在路上
			{
				if (!(m_me->GetPos() - m_center_pos).IsLongerThan(2))
				{
					m_stop = true;
				}

				if (!m_stop)
				{
					if (m_me->IsStatic() && NULL != m_me->GetScene())
					{
						Posi delta_pos = m_center_pos - m_me->GetPos();

						Scalar move_dist = 0;
						Dir move_dir = gamecommon::GetMoveDir(delta_pos, &move_dist);

						m_me->SetDir(move_dir, move_dist);
					}

					if (Character::MOVE_RUN == m_me->GetLogicResult()) 
					{
						MoveAOI();
					}
				}

				if (now_sec >= m_next_perform_time)
				{
					this->PerformBuffSkill();
					m_next_perform_time = now_sec + m_skill_interval;
				}
			}
			else				// 中心点
			{
				if (now_sec >= m_next_perform_time)
				{
					this->PerformBuffSkill();
					m_next_perform_time = now_sec + m_skill_interval;
				}
				else
				{
					if (m_enemy != INVALID_OBJ_ID || FindEnemy(interval))
					{
						if (CanStartAttack())
						{
							StartAttack(m_enemy, true);
						}
						else 
						{
							m_me->StopAttack();
							m_enemy = INVALID_OBJ_ID;
							m_need_search = true;
						}
					}
				}
			}
		}
		break;
	}
}

bool MonsterAIXianMengZhanDanYaoBoss::IsEnemy(Obj *obj)
{
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;

		if (INVALID_GUILD_ID != m_guild_id && m_guild_id != role->GetGuildID())
		{
			return true;
		}
	}

	return false;
}

bool MonsterAIXianMengZhanDanYaoBoss::IsFriend(Obj *obj, bool check_alive)
{
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;

		if (INVALID_GUILD_ID != m_guild_id && m_guild_id == role->GetGuildID())
		{
			return true;
		}
	}

	return false;
}

bool MonsterAIXianMengZhanDanYaoBoss::FindEnemy(unsigned long interval)
{
	if (m_enemy != INVALID_OBJ_ID)
	{
		return false;
	}

	while (m_enemy_size > 0)
	{
		Obj *obj = m_me->GetScene()->GetObj(m_enemy_list[0]);
		if (obj == 0 || !m_me->IsEnemy(obj))
		{
			-- m_enemy_size;
			m_enemy_list[0] = m_enemy_list[m_enemy_size];

			continue;
		}

		if (m_enemy_size > 0) m_enemy = m_enemy_list[RandomNum(m_enemy_size)];
		
		return true;
	}

	m_enemy = INVALID_OBJ_ID;

	m_enemy_size = this->SearchNearEnemys(interval, m_center_pos, m_aoi_range, m_enemy_list, MAX_ENEMY_NUM);

	if (m_enemy_size > 0)
	{
		m_enemy = m_enemy_list[RandomNum(m_enemy_size)];
		return true;
	}

	return false;
}

void MonsterAIXianMengZhanDanYaoBoss::MoveAOI()
{
	const int PETAI_AOI_SENSITIVE_DISTANCE = (int)GLOBALCONFIG->GetKeyConfig_PetAoiSensitiveDistance();	// PetAi中移动距离大于这个时MoveAOI
	if ((m_me->GetPos() - m_aoi_cur_pos).IsLongerThan(PETAI_AOI_SENSITIVE_DISTANCE))
	{
		m_aoi_cur_pos = m_me->GetPos();
		m_me->GetScene()->GetZoneMatrix()->MoveAOI(m_aoi_handle);
	}
}

void MonsterAIXianMengZhanDanYaoBoss::PerformBuffSkill()
{
	if (NULL == m_me->GetScene()) return;

	SkillBase::SendAOEAttackMsgHelper(m_me, INVALID_OBJ_ID, (UInt16)m_skill_id, m_me->GetPos());

	const int MAX_OBJ_COUNT = 256;

	static ObjID obj_list[MAX_OBJ_COUNT] = {0};
	int count = m_me->GetScene()->GetZoneMatrix()->GetObjByArea(m_me->GetPos(), m_atk_range, m_atk_range, obj_list, MAX_OBJ_COUNT);

	for (int i = 0; i < count && i < MAX_OBJ_COUNT; ++i)
	{
		Obj *obj = m_me->GetScene()->GetObj(obj_list[i]);
		if (NULL != obj && Obj::IsCharactor(obj->GetObjType()) && m_me->IsFriend(obj))
		{
			Character *target_cha = (Character *)obj;

			Attribute add_gongji = static_cast<Attribute>(target_cha->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (m_add_gongji_percent / 100.0f));
			Attribute add_fangyu = static_cast<Attribute>(target_cha->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * (m_add_fangyu_percent / 100.0f));

			EffectBuff *effect = new EffectBuff(m_me->GetId(), 1, m_buff_dur_ms, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);

			effect->AddGongJiEffect(add_gongji, m_add_gongji_percent);
			effect->AddFangYuEffect(add_fangyu, m_add_fangyu_percent);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);

			target_cha->AddEffect(effect);
		}
	}
}

void MonsterAIXianMengZhanDanYaoBoss::InitAIParam(GuildID guild_id, const Posi &center_pos, int skill_interval, int skill_id, int atk_range, int add_gongji_percent, int add_fangyu_percent, int buff_dur_ms)
{
	m_guild_id = guild_id;
	m_center_pos = center_pos;
	m_skill_interval = skill_interval;
	m_skill_id = skill_id;
	m_atk_range = atk_range;
	m_add_gongji_percent = add_gongji_percent;
	m_add_fangyu_percent = add_fangyu_percent;
	m_buff_dur_ms = buff_dur_ms;

	m_next_perform_time = (unsigned int)(EngineAdapter::Instance().Time());	// 出场后立即释放
}
