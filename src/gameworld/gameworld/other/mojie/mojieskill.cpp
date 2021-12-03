#include "mojieskill.hpp"
#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"
#include "servercommon/commondata.hpp"
#include "skill/skillbase.hpp"
#include "engineadapter.h"
#include "obj/character/monster.h"
#include "scene/scene.h"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectotherbuff.hpp"
#include "other/capability/capability.hpp"

MojieSkill::MojieSkill() : m_role(nullptr), m_next_update_time(0), m_other_capability(0), m_add_exp_buff_percent(0),
m_baoji_hurt_per(0), m_kangbao_per(0), m_kangbao_hurt_per(0),
m_huixinyiji(0), m_huixinyiji_hurt_per(0),
m_mianshang_per(0), m_pve_zengshang_per(0), m_pvp_jianshang_per(0)
{
	memset(m_next_perfrom_skill_time, 0, sizeof(m_next_perfrom_skill_time));
	memset(m_skill_type_max_skill_id, -1, sizeof(m_skill_type_max_skill_id));
}

MojieSkill::~MojieSkill()
{

}

void MojieSkill::CheckActiveSkill(int pata_layer)
{
	LOGIC_CONFIG->GetMojieSkillConfig().SetActiveSkillFlag(m_role->GetCommonDataParam()->mojie_skill_active_flag, pata_layer);

	int skill_type = LOGIC_CONFIG->GetMojieSkillConfig().GetSkillTypeByLayer(pata_layer);    
	if (skill_type >= SKILL_TYPE_0 && skill_type < SKILL_TYPE_MAX)
	{
		m_skill_type_max_skill_id[skill_type] = this->CaclMaxSkillIdBySkillType(skill_type);

		if (m_skill_type_max_skill_id[skill_type] >= 0)
		{
			this->AddExpBuff(skill_type, m_skill_type_max_skill_id[skill_type]);
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOJIE_SKILL);
		}
	}
}

bool MojieSkill::CanPerfromSkill(unsigned int now_sec, int skill_type, int &skill_id)
{
	if (skill_type < SKILL_TYPE_0 || skill_type >= SKILL_TYPE_MAX)
	{
		return false;
	}

	skill_id = this->GetMaxActiveSkillIdByType(skill_type);
	if (skill_id < 0 || skill_id >= MojieSkillConfig::MOJIE_SKILL_COUNT)
	{
		return false;
	}

	return now_sec > m_next_perfrom_skill_time[skill_id];
}

void MojieSkill::InitSkillTypeMaxSkillId()
{
	for (int skill_type = 0; skill_type < SKILL_TYPE_MAX; ++skill_type)
	{
		const int skill_old = m_skill_type_max_skill_id[skill_type];
		m_skill_type_max_skill_id[skill_type] = this->CaclMaxSkillIdBySkillType(skill_type);

		if (skill_old != m_skill_type_max_skill_id[skill_type])
		{
			this->AddExpBuff(skill_type, m_skill_type_max_skill_id[skill_type]);
		}
	}

//	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOJIE_SKILL);
}

void MojieSkill::OnInitFinish()
{
	this->InitSkillTypeMaxSkillId();
	//m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOJIE_SKILL);
}

int MojieSkill::CaclMaxSkillIdBySkillType(int skill_type)
{
	if (skill_type < SKILL_TYPE_0 || skill_type >= SKILL_TYPE_MAX)
	{
		return -1;
	}

	for (int id = MojieSkillConfig::MOJIE_SKILL_COUNT - 1; id >= 0; --id)
	{
		const MojieSkillCfg *skill_cfg = LOGIC_CONFIG->GetMojieSkillConfig().GetSkillCfg(id);
		if (skill_cfg != nullptr && skill_cfg->skill_type == skill_type && this->IsActiveSkill(id))
		{
			return id;
		}
	}

	return -1;
}

int MojieSkill::GetMaxActiveSkillIdByType(int skill_type)
{
	if (skill_type < SKILL_TYPE_0 || skill_type >= SKILL_TYPE_MAX)
	{
		return -1;
	}

	return m_skill_type_max_skill_id[skill_type];
}

bool MojieSkill::IsActiveSkill(int skill_id)
{
	if (skill_id < 0 || skill_id >= MojieSkillConfig::MOJIE_SKILL_COUNT)
	{
		return false;
	}
	 
	unsigned long long flag = m_role->GetCommonDataParam()->mojie_skill_active_flag;
	return 0 != (flag & (LL_ONE_BIT << skill_id));
}

void MojieSkill::Update(unsigned int now_sec, unsigned int interval)
{
	// ÿ����һ�Σ����Ḻ��
	if (m_next_update_time >= 1000)
	{
		int skill_id = -1;
		//����2��ÿparam_1��ظ����param_2%������
		if (CanPerfromSkill(now_sec, SKILL_TYPE_2, skill_id) && m_role->IsAlive())
		{
			const MojieSkillCfg *skill_cfg_2 = LOGIC_CONFIG->GetMojieSkillConfig().GetSkillCfg(skill_id);
			if (nullptr != skill_cfg_2)
			{
				m_next_perfrom_skill_time[skill_id] = now_sec + skill_cfg_2->param_list[MojieSkillCfg::SKILL_PARAM_1];

				Attribute now_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				Attribute max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);

				Attribute add_hp = static_cast<Attribute>(max_hp * (skill_cfg_2->param_list[MojieSkillCfg::SKILL_PARAM_2] * SKILL_ATTR_PER));
				if (now_hp + add_hp > max_hp)
				{
					add_hp = max_hp - now_hp;
				}

				if (add_hp > 0)
				{
					EffectSpecialState *special_effect = new EffectSpecialState(m_role->GetId(), 1, 1000, EBT_MOJIESKILL_RESTORE_HP, 1, EffectBase::PRODUCT_METHOD_MOJIE_SKILL);	//֪ͨ�ͻ��˲�����Ч
					m_role->AddEffect(special_effect);
					m_role->ChangeHp(&add_hp, true);
				}
			}
		}

		m_next_update_time = 0;
	}
	else
	{
		m_next_update_time += interval;
	}
}

void MojieSkill::OnHurtOther(Character *target, long long &injure, int fight_type, int target_index)
{
	if (NULL == target)
	{
		return;
	}

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	int skill_id = -1;
	// ����0�� ����ʱ��param_1%�����ٻ����ף�����Ŀ�����param_2%�������˺���CDΪparam_0��
	if (0 == target_index && CanPerfromSkill(now_sec, SKILL_TYPE_0, skill_id))
	{
		const MojieSkillCfg *skill_cfg_1 = LOGIC_CONFIG->GetMojieSkillConfig().GetSkillCfg(skill_id);
		if (nullptr != skill_cfg_1)
		{
			if (RandomNum(10000) < skill_cfg_1->param_list[MojieSkillCfg::SKILL_PARAM_1])
			{
				m_next_perfrom_skill_time[skill_id] = now_sec + skill_cfg_1->param_list[MojieSkillCfg::SKILL_PARAM_0];

				int add_hurt_per = skill_cfg_1->param_list[MojieSkillCfg::SKILL_PARAM_2];
				Attribute gongji = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI);

				injure += static_cast<Attribute>(gongji * (add_hurt_per * SKILL_ATTR_PER));
				EffectSpecialState *special_effect = new EffectSpecialState(target->GetId(), 1, 1000, EBT_MOJIESKILL_THUNDER, 1, EffectBase::PRODUCT_METHOD_MOJIE_SKILL);	//֪ͨ�ͻ��˲�����Ч
				target->AddEffect(special_effect);
			}
		}
	}

	// ����10������ʱ��param_1%����ʹĿ�꽵���ƶ��ٶ�param_2%������param_3���롣CDΪparam_0��
	if (CanPerfromSkill(now_sec, SKILL_TYPE_10, skill_id))
	{
		const MojieSkillCfg *skill_cfg_10 = LOGIC_CONFIG->GetMojieSkillConfig().GetSkillCfg(skill_id);
		if (nullptr != skill_cfg_10)
		{
			if (RandomNum(10000) < skill_cfg_10->param_list[MojieSkillCfg::SKILL_PARAM_1])
			{
				m_next_perfrom_skill_time[skill_id] = now_sec + skill_cfg_10->param_list[MojieSkillCfg::SKILL_PARAM_0];

				int dec_speed_percent = skill_cfg_10->param_list[MojieSkillCfg::SKILL_PARAM_2];
				int effect_time = skill_cfg_10->param_list[MojieSkillCfg::SKILL_PARAM_3];
				int value = static_cast<int>(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED) * (dec_speed_percent * ROLE_ATTR_PER));
				
				EffectMoveSpeed *effect_speed = new EffectMoveSpeed(m_role->GetId(), skill_id, effect_time, EBT_MOJIESKILL_SLOW, 1);
				effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
				effect_speed->SetMoveSpeed(-value, -dec_speed_percent / 100);
				effect_speed->SetClientEffectType(EffectBase::ECT_SKILL_DEC_SPEED);
				target->AddEffect(effect_speed);
			}
		}
	}
}

void MojieSkill::OnHurtBoss(Character *target, long long &injure)
{
	
}

void MojieSkill::OnAttackCalcHurt(ObjID attacker, long long &delta_hp, int fight_type)
{
	
}

void MojieSkill::CheckAddDecControlBuffTimeBuff(EffectBase *effect)
{
	//����9���ܵ�����Ч�����param_1���ڣ����ܵ���������Ч����Ч��ʱ�����param_2%����ȴparam_0��...��������һ��buff
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	int skill_id = -1;
	if (CanPerfromSkill(now_sec, SKILL_TYPE_9, skill_id))
	{
		const MojieSkillCfg *skill_cfg_9 = LOGIC_CONFIG->GetMojieSkillConfig().GetSkillCfg(skill_id);
		if (nullptr != skill_cfg_9)
		{
			if (nullptr != effect && m_role->GetEffectList()->IsControlBuffType(effect->GetBuffType()) && m_role->GetEffectList()->CanClearByFuncClearAllDeBuffEffect(effect))
			{
				m_next_perfrom_skill_time[skill_id] = now_sec + skill_cfg_9->param_list[MojieSkillCfg::SKILL_PARAM_0];

				int buff_time_ms = skill_cfg_9->param_list[MojieSkillCfg::SKILL_PARAM_1] * 1000;

				EffectSpecialState *special_effect = new EffectSpecialState(m_role->GetId(), EffectBase::PRODUCT_ID_MOJIE_SKILL,
					buff_time_ms, EBT_DEC_CONTROL_BUFF_TIME, 1, EffectBase::PRODUCT_METHOD_SKILL);

				special_effect->SetClientEffectType(EffectBase::ECT_MOJIE_SKILL_9);
				special_effect->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
				special_effect->SetParam(skill_cfg_9->param_list[MojieSkillCfg::SKILL_PARAM_2] * 1000);			//�ͻ������󣬱������1000������ʹ�õ�ʱ���ٳ�����

				m_role->AddEffect(special_effect);
			}
		}
	}
}

void MojieSkill::CheckDecControlBuffTime(EffectBase *effect)
{
	//����9���ܵ�����Ч�����param_1���ڣ����ܵ���������Ч����Ч��ʱ�����param_2%����ȴparam_0��...������ʽ��buffʱ�����
	EffectBase *dec_effect = m_role->GetEffectList()->FindEffectByType(EffectBase::EFFECT_TYPE_SPECIAL_STATE, EBT_DEC_CONTROL_BUFF_TIME);
	if (nullptr != dec_effect)	//����buff
	{
		EffectSpecialState *special_effect = static_cast<EffectSpecialState *>(dec_effect);

		int dec_per = special_effect->GetParam();

		if (nullptr != effect && m_role->GetEffectList()->IsControlBuffType(effect->GetBuffType()) && m_role->GetEffectList()->CanClearByFuncClearAllDeBuffEffect(effect))
		{
			int left_time_ms = effect->GetLeftTimeMS();
			left_time_ms -= static_cast<int>(left_time_ms * (dec_per / 1000 * SKILL_ATTR_PER));

			effect->SetLeftTimeMS(left_time_ms);
		}
	}
}

void MojieSkill::GmResetActiveFlag(int skill_id)
{
#ifndef _DEBUG
	return;
#endif
	
	unsigned long long &flag = m_role->GetCommonDataParam()->mojie_skill_active_flag;
	if (skill_id < 0 || skill_id >= MojieSkillConfig::MOJIE_SKILL_COUNT)
	{
		return;
	}

	if (0 != (flag & (LL_ONE_BIT << skill_id)))
	{
		flag &= ~(LL_ONE_BIT << skill_id);
	}
	else
	{
		flag |= (LL_ONE_BIT << skill_id);
	}
}

void MojieSkill::AddExpBuff(int skill_type, int skill_id)
{
	if (SKILL_TYPE_3 == skill_type)
	{
		// ����3 �����ȡ+param_1%
		
		const MojieSkillCfg *skill_cfg = LOGIC_CONFIG->GetMojieSkillConfig().GetSkillCfg(skill_id);
		if (NULL == skill_cfg || skill_cfg->skill_type != skill_type) return;

		if (!this->IsActiveSkill(skill_id)) return;
		
		//������BUFF����Ϊ��Ӱ�쾭��ҩˮBUFF������ 
		//m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFEECT_TYPE_OTHER, EBT_INVALID, INVALID_OBJ_ID,
		//	EffectBase::PRODUCT_METHOD_MOJIE_SKILL, EffectBase::PRODUCT_ID_INC_EXTRAL_EXP_PER);
		//
		//if (skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1] > 0)
		//{
		//	EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_INC_EXTRAL_EXP_PER, 0, 1, EffectBase::PRODUCT_METHOD_MOJIE_SKILL);
		//	effect->IncExtraExpPer(skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1]);
		//	effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_5);
		//	effect->SetClientEffectType(EffectBase::ECT_MOJIE_SKILL_3);
		//	effect->SetSave(true);
		//	m_role->AddEffect(effect);
		//}

		if (skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1] > 0)
		{
			m_add_exp_buff_percent = skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1];
		}
	}
}

void MojieSkill::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		m_baoji_hurt_per = 0;
		m_kangbao_per = 0;
		m_kangbao_hurt_per = 0;

		m_huixinyiji = 0;
		m_huixinyiji_hurt_per = 0;

		m_mianshang_per = 0;
		m_pve_zengshang_per = 0;
		m_pvp_jianshang_per = 0;

		for (int skill_type = 0; skill_type < SKILL_TYPE_MAX; ++skill_type)
		{
			int skill_id = GetMaxActiveSkillIdByType(skill_type);
			if (this->IsActiveSkill(skill_id))
			{
				const MojieSkillCfg *skill_cfg = LOGIC_CONFIG->GetMojieSkillConfig().GetSkillCfg(skill_id);
				if (NULL == skill_cfg || skill_cfg->skill_type != skill_type) continue;

				switch (skill_type)
				{
				case SKILL_TYPE_1:
				{
					// ����1 �ƶ��ٶ�+param_1%
					float speed_per = static_cast<float>(skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1] * ROLE_ATTR_PER);
					m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED] += speed_per;
				}
				break;

				case SKILL_TYPE_4:
				{
					// ����4 PVE�˺��ӳ�+param_1%
//					m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1];
					// m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1]);
					m_pve_zengshang_per += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1];
				}
				break;

				case SKILL_TYPE_5:
				{
					// ����5 �����˺� + param_1%�������˺� + param_2%
					// m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1];
					// m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER] += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_2];
					m_baoji_hurt_per += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1];
					m_huixinyiji_hurt_per += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_2];
				}
				break;

				case SKILL_TYPE_6:
				{
					// ����6 ����param_1%��PVP����param_2%
// 					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1];
// 					m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_2];
					// m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1]);
					// m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_2]);
					m_mianshang_per += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1];
					m_pvp_jianshang_per += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_2];
				}
				break;

				case SKILL_TYPE_7:
				{
					// ����7 ����һ�������param_1%�������˺����param_2%
					// m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI] += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1];
					// m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER] += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_2];
					m_huixinyiji += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1];
					m_huixinyiji_hurt_per += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_2];
				}
				break;

				case SKILL_TYPE_8:
				{
					// ����8 ��������+param_1%�������ֿ�+param_2%
					// m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO] += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1];
					// m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO_HURT] += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_2];
					m_kangbao_per += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_1];
					m_kangbao_hurt_per += skill_cfg->param_list[MojieSkillCfg::SKILL_PARAM_2];
				}
				break;

				default:
					break;
				}
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_MOJIE_SKILL, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

