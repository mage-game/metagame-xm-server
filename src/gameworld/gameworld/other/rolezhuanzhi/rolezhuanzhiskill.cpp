#include "rolezhuanzhiskill.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "servercommon/errornum.h"
#include "other/event/eventhandler.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "servercommon/noticenum.h"
#include "scene/scene.h"
#include "scene/speciallogic/speciallogic.hpp"
#include "obj/character/monster.h"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectinterval.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effecthurtinc.hpp"
#include "effect/skilleffect/effecthpstore.hpp"
#include "effect//skilleffect/effectwudiprotect.hpp"
#include "effect/skilleffect/effectreboundhurt.hpp"
#include "skill/skillpool.hpp"
#include "item/knapsack.h"
#include "skill/skillbase.hpp"
#include "friend/teaminfo.hpp"
#include "protocal/msgrole.h"
#include "protocal/msgzhuanzhi.h"
#include "engineadapter.h"

#include <algorithm>

RoleZhuanZhiSkill::RoleZhuanZhiSkill()
{

}

RoleZhuanZhiSkill::~RoleZhuanZhiSkill()
{

}

void RoleZhuanZhiSkill::Init(Role *role, RoleZhuanZhi *rolezhuanzhi)
{
	m_role = role;
	m_rolezhuanzhi = rolezhuanzhi;

	{
		m_max_cfg_skill_count = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiSkillMAxCount();
	}
}


void RoleZhuanZhiSkill::OnInitFinish()
{
	//need role scene
	this->RecalcActivedSkill(true);
}

void RoleZhuanZhiSkill::RecalcActivedSkill(bool is_init)
{
	m_special_attr_pvp_jianshang = 0;
	m_special_attr_pvp_zengshang = 0;
	m_special_attr_pve_jianshang = 0;
	m_special_attr_pve_zengshang = 0;

	int prof_level = m_role->GetProfLevel();

	for (; prof_level >= 1; prof_level --)
	{
		const ZhuanZhiLevelSkillConfig::ConfigItem *cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiSkillCfg(prof_level, m_role->GetBaseProf());
		if (NULL == cfg)
		{
			continue;
		}

		m_normal_aoe_add_target_num += cfg->add_target_num;

		m_special_attr_pvp_jianshang += cfg->pvp_jianshang_per;
		m_special_attr_pvp_zengshang += cfg->pvp_zengshang_per;
		m_special_attr_pve_jianshang += cfg->pve_jianshang_per;
		m_special_attr_pve_zengshang += cfg->pve_zengshang_per;

		for (unsigned int i = 0; i < static_array_size(cfg->add_skill_list); i++)
		{
			int add_skill_id = cfg->add_skill_list[i];

			auto it = m_cur_active_skill_map.find(add_skill_id);
			if (it == m_cur_active_skill_map.end())
			{
				const ZhuanzhiVirSkillCfg *cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(add_skill_id);
				if (nullptr == cfg)
				{
					// 添加激活的转职技能id
					if (0 != add_skill_id && NULL != SKILLPOOL->GetSkill(add_skill_id))
					{
						SkillManager::SkillItem real_skill_item;
						if (!m_role->GetSkillManager()->GetSkillByID(add_skill_id, &real_skill_item))
						{
							m_role->GetSkillManager()->AddSkill(add_skill_id, 1, NULL, is_init);
						}
					}
				}
				else
				{
					VirSkillItem skill_item;
					skill_item.vir_skill_id = add_skill_id;
					skill_item.vir_skill_type = cfg->vir_skill_type;
					skill_item.follow_perform_skill_id = cfg->follow_perform_skill_id;
					skill_item.level = 0;
					skill_item.last_perform_skill_timestamp = 0;

					m_cur_active_skill_map[add_skill_id] = skill_item;
				}
			}
		}
	}
}

bool RoleZhuanZhiSkill::IsActiveSkill(int skill_id)
{
	auto it = m_cur_active_skill_map.find(skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return false;
	}

	return true;
}

RoleZhuanZhiSkill::VirSkillItem * RoleZhuanZhiSkill::GetVirSkillItem(int vir_skill_type)
{
	auto it = std::find_if(m_cur_active_skill_map.begin(), m_cur_active_skill_map.end(), [vir_skill_type](const std::map<int, VirSkillItem>::value_type &cmp_item) {
		if (vir_skill_type == cmp_item.second.vir_skill_type)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != m_cur_active_skill_map.end())
	{
		return &it->second;
	}

	return nullptr;
}

void RoleZhuanZhiSkill::CheckRecover(time_t now_second)
{
	if (m_role->IsFullHp()) return;

	for (auto it = m_cur_active_skill_map.begin(); it != m_cur_active_skill_map.end(); it++)
	{
		if (it->second.vir_skill_type == ZHUANZHI_VIR_SKILL_TYPE_RECOVER_FIX)
		{
			const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(it->first);
			if (NULL == skill_cfg) continue;

			if (now_second < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
			{
				continue;
			}

			long long add_hp = skill_cfg->param_1;

			if (m_role->IsAlive())
			{
//				m_role->ChangeHp(&add_hp, true);

				EffectInterval *effect_interval = new EffectInterval(m_role->GetId(), it->second.vir_skill_id, EBT_ADD_BLOOD, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
				effect_interval->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
				effect_interval->SetParam(1000, 1, add_hp);
				effect_interval->SetSave(false);

				m_role->AddEffect(effect_interval);

				it->second.last_perform_skill_timestamp = (unsigned int)now_second;

				BroadcastSkillTrigger(it->first, m_role->GetId());
			}
		}
	}
}


bool RoleZhuanZhiSkill::CheckMainyiXuanyun()
{
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	VirSkillItem *skill_item = this->GetVirSkillItem(ZHUANZHI_VIR_SKILL_TYPE_XUANYUN_MIANYI);
	if (nullptr == skill_item)
	{
		return false;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(skill_item->vir_skill_id);
	if (NULL == skill_cfg) return false;

	if (now < skill_item->last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return false;
	}

	skill_item->last_perform_skill_timestamp = now;

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return false;
	}

	return true;
}

double RoleZhuanZhiSkill::GetAddExpPer()
{
	int add_exp_per = 0;
	for (auto it = m_cur_active_skill_map.begin(); it != m_cur_active_skill_map.end(); it++)
	{
		if (it->second.vir_skill_type == ZHUANZHI_VIR_SKILL_TYPE_UP_EXP_PER)
		{
			const ZhuanzhiVirSkillCfg *cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(it->second.vir_skill_id);
			if (NULL == cfg) continue;
			add_exp_per += cfg->param_1;
		}
	}

	return add_exp_per / 100.0;
}


void RoleZhuanZhiSkill::OnPerformSkillReq(int vir_skill_id, ObjID target_id)
{
	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		m_role->NoticeNum(errornum::EN_PET_SKILL_HAS_NOT_LEARN);
	}

	switch (it->second.vir_skill_type)
	{
	case ZHUANZHI_VIR_SKILL_TYPE_RECOVER:
		{
			this->OnPerformRecover(vir_skill_id);
		}
		break;

	case ZHUANZHI_VIR_SKILL_TYPE_HURT_3:
		{
			this->OnPerformHurt3(vir_skill_id, target_id);
		}
		break;
	}
}

void RoleZhuanZhiSkill::OnPerformRecover(int vir_skill_id)
{
	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	if (it->second.vir_skill_type != ZHUANZHI_VIR_SKILL_TYPE_RECOVER)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg) return;

	if (now_second < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	Attribute cur_target_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	Attribute max_target_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	int hp_per = static_cast<int>(cur_target_hp / max_target_hp * 10000.0f);
	Attribute recover_hp = 0;
	if (hp_per >= skill_cfg->param_1)
	{
		recover_hp = static_cast<Attribute>(max_target_hp * (skill_cfg->param_2 * SKILL_ATTR_PER));
	}
	else
	{
		recover_hp = static_cast<Attribute>(max_target_hp * (skill_cfg->param_3 * SKILL_ATTR_PER));
	}

	EffectInterval *effect_interval = new EffectInterval(m_role->GetId(), vir_skill_id, EBT_ADD_BLOOD, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_interval->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
	effect_interval->SetParam(1000, 1, recover_hp);
	effect_interval->SetSave(false);

	m_role->AddEffect(effect_interval);

	EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), vir_skill_id, skill_cfg->param_5, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	effect_buff->AddFangYuEffect(skill_cfg->param_4);
	m_role->AddEffect(effect_buff);

	it->second.last_perform_skill_timestamp = (unsigned int)now_second;

	BroadcastSkillTrigger(vir_skill_id, m_role->GetId());
}


void RoleZhuanZhiSkill::OnPerformHurt3(int vir_skill_id, ObjID target_id)
{
	Obj* obj = m_role->GetScene()->GetObj(target_id);
	if (NULL == obj)
	{
		return;
	}

	Character *target = (Character *)obj;

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	if (it->second.vir_skill_type != ZHUANZHI_VIR_SKILL_TYPE_HURT_3)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg) return;

	if (now_second < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	Attribute deta_hp = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (skill_cfg->param_1 * SKILL_ATTR_PER));
	EffectOnce *dec_effect = new EffectOnce(m_role->GetId(), vir_skill_id, -deta_hp, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	target->AddEffect(dec_effect);

	EffectSpecialState *effect_dingshen = new EffectSpecialState(m_role->GetId(), vir_skill_id, skill_cfg->param_2, EBT_DINGSHEN, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_dingshen->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
	effect_dingshen->SetClientEffectType(EffectBase::ECT_SKILL_DINGSHEN);
	target->AddEffect(effect_dingshen);

	it->second.last_perform_skill_timestamp = (unsigned int)now_second;

	BroadcastSkillTrigger(vir_skill_id, target->GetId());
}

void RoleZhuanZhiSkill::OnHurtOther(Character * target, int skill_id, long long injure, int fight_type)
{
	OnHurtOtherPVPUpShanghaiAttr(target, injure);
	OnHurtOtherPVEUpShanghaiAttr(target, injure);

	for(auto it : m_cur_active_skill_map)
	{
		switch (it.second.vir_skill_type)
		{
		case ZHUANZHI_VIR_SKILL_TYPE_PVE_UP_SHANGHAI:
			{ 
				this->OnHurtOtherPVEUpShanghai(target, it.second.vir_skill_id, skill_id);
			}
			break;

		case ZHUANZHI_VIR_SKILL_TYPE_PVP_UP_SHANGHAI:
			{			
				this->OnHurtOtherPVPUpShanghai(target, it.second.vir_skill_id, skill_id);
			}
			break;

		case ZHUANZHI_VIR_SKILL_TYPE_PVP_XUANYUN:
			{
				this->OnHurtOtherPVPXuanyun(target, it.second.vir_skill_id, skill_id);
			}
			break;

		case ZHUANZHI_VIR_SKILL_TYPE_PVP_UP_SHANGHAI_2:
			{
				this->OnHurtOtherPVPUpShanghai2(target, it.second.vir_skill_id, skill_id);
			}
			break;

		case ZHUANZHI_VIR_SKILL_TYPE_PVM_UP_SHANGHAI:
			{
				this->OnHurtOtherPVMUpShanghai(target, it.second.vir_skill_id, skill_id, injure);
			}
			break;

		case ZHUANZHI_VIR_SKILL_TYPE_PVP_UP_SHANGHAI_3:
			{
				this->OnHurtOtherPVPUpShanghai3(target, it.second.vir_skill_id, skill_id);
			}
			break;

		case ZHUANZHI_VIR_SKILL_TYPE_PVP_UP_SHANGHAI_4:
			{
				this->OnHurtOtherPVPUpShanghai4(target, it.second.vir_skill_id, skill_id);
			}
			break;

		case ZHUANZHI_VIR_SKILL_TYPE_HURT_INTERVAL:
			{
				this->OnHurtOtherHurtInterval(target, it.second.vir_skill_id, skill_id);
			}
			break;

		case ZHUANZHI_VIR_SKILL_TYPE_HURT_1:
			{
				this->OnHurtOtherHurt1(target, it.second.vir_skill_id, skill_id);
			}
			break;

		case ZHUANZHI_VIR_SKILL_TYPE_HURT_2:
			{
				this->OnHurtOtherHurt2(target, it.second.vir_skill_id, skill_id, injure);
			}
			break;

		case ZHUANZHI_VIR_SKILL_TYPE_HURT_4:
			{
				this->OnHurtOtherHurt4(target, it.second.vir_skill_id, skill_id, injure);
			}
			break;

		case ZHUANZHI_VIR_SKILL_TYPE_HURT_5:
			{
				this->OnHurtOtherHurt5(target, it.second.vir_skill_id, skill_id, injure);
			}
			break;

		case ZHUANZHI_VIR_SKILL_TYPE_PVP_CHENMO:
			{
				this->OnHurtOtherPVPChenmo(target, it.second.vir_skill_id, skill_id);
			}
			break;

		case ZHUANZHI_VIR_SKILL_TYPE_PVE_RECOVER:
			{
				this->OnHurtOtherPVERecover(target, it.second.vir_skill_id, skill_id, injure);
			}
			break;
		}
	}
}


void RoleZhuanZhiSkill::OnRoleBeenAttack(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill)
{
	Obj* obj = m_role->GetScene()->GetObj(attacker);
	if (NULL == obj)
	{
		return;
	}

	for (auto it : m_cur_active_skill_map)
	{
		switch (it.second.vir_skill_type)
		{
		case ZHUANZHI_VIR_SKILL_TYPE_PVP_REBOUND:
			{
				this->OnBeenAttackPVPRebound(attacker, it.second.vir_skill_id, skill_id, delta_hp);
			}
			break;

		case ZHUANZHI_VIR_SKILL_TYPE_PVP_DOWN_SHANGHAI:
			{
				this->OnBeenAttackPVPDownShanghai(attacker, it.second.vir_skill_id, skill_id);
			}
			break;

		case ZHUANZHI_VIR_SKILL_TYPE_NORMAL_HPSTORE:
			{
				this->OnBeenAttackNormalHpStore(it.second.vir_skill_id, skill_id);
			}
			break;

		case ZHUANZHI_VIR_SKILL_TYPE_WUDI_HPSTORE:
			{
				this->OnBeenAttackWudiHpStore(it.second.vir_skill_id, skill_id);
			}
			break;
		}
	}
}


void RoleZhuanZhiSkill::OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{
	OnBeendAttackPVPDownShanghaiAttr(attacker, delta_hp);
	OnBeendAttackPVEDownShanghaiAttr(attacker, delta_hp);
}

void RoleZhuanZhiSkill::CheckKillOtherRole(bool is_kill)
{
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (auto it = m_cur_active_skill_map.begin(); it != m_cur_active_skill_map.end(); it++)
	{
		if (it->second.vir_skill_type != ZHUANZHI_VIR_SKILL_TYPE_PVP_UP_SHANGHAI_4)
		{
			continue;
		}

		const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(it->first);
		if (NULL == skill_cfg) continue;

		unsigned int dur_s = static_cast<unsigned int>(skill_cfg->param_3 / 1000);

		if (!is_kill)
		{
			if (now >= it->second.last_perform_skill_timestamp + dur_s)
			{
				it->second.level = 0;
				continue;
			}
		}
		else if(it->second.level < skill_cfg->param_2)
		{
			it->second.level++;
		}
	}
}


void RoleZhuanZhiSkill::OnHurtOtherPVPUpShanghaiAttr(Character *target, long long base_injure)
{
	if (nullptr == target || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	if (m_special_attr_pvp_zengshang <= 0)
	{
		return;
	}

	Attribute up_shanghai = static_cast<Attribute>((m_special_attr_pvp_zengshang * SKILL_ATTR_PER) * base_injure);

	EffectOnce *dec_effect = new EffectOnce(m_role->GetId(), 0, -up_shanghai, EffectBase::FIGHT_TYPE_NORMAL);
	target->AddEffect(dec_effect);
}


void RoleZhuanZhiSkill::OnHurtOtherPVEUpShanghaiAttr(Character *target, long long base_injure)
{
	if (nullptr == target || Obj::OBJ_TYPE_MONSTER != target->GetObjType())
	{
		return;
	}

	if (m_special_attr_pve_zengshang <= 0)
	{
		return;
	}

	Attribute up_shanghai = static_cast<Attribute>((m_special_attr_pve_zengshang * SKILL_ATTR_PER) * base_injure);

	EffectOnce *dec_effect = new EffectOnce(m_role->GetId(), 0, -up_shanghai, EffectBase::FIGHT_TYPE_NORMAL);
	target->AddEffect(dec_effect);
}

void RoleZhuanZhiSkill::OnHurtOtherPVEUpShanghai(Character * target, int vir_skill_id, int skill_id)
{
	if (nullptr == target)
	{
		return;
	}

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

	int param_1 = skill_cfg->param_1;
	int param_2 = skill_cfg->param_2;

	EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), vir_skill_id, param_2, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_buff->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
	effect_buff->AddAttrEffect((char)CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, param_1);
	effect_buff->SetSave(false);
	m_role->AddEffect(effect_buff);

	it->second.last_perform_skill_timestamp = now;

	BroadcastSkillTrigger(vir_skill_id, m_role->GetId());
}


void RoleZhuanZhiSkill::OnHurtOtherPVPUpShanghai(Character * target, int vir_skill_id, int skill_id)
{
	if (nullptr == target || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

	long long param_1 = static_cast<long long>(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (skill_cfg->param_1 * SKILL_ATTR_PER));
	int param_2 = skill_cfg->param_2;

	EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), vir_skill_id, param_2, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_buff->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
	effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG, param_1);
	effect_buff->SetSave(false);
	m_role->AddEffect(effect_buff);

	it->second.last_perform_skill_timestamp = now;

	BroadcastSkillTrigger(vir_skill_id, m_role->GetId());
}


void RoleZhuanZhiSkill::OnHurtOtherPVPXuanyun(Character * target, int vir_skill_id, int skill_id)
{
	if (nullptr == target || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

	int param_1 = skill_cfg->param_1;

	EffectSpecialState *specail_state = new EffectSpecialState(m_role->GetId(), vir_skill_id, param_1, EBT_XUANYUN, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	specail_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
	specail_state->SetClientEffectType(EffectBase::ECT_SKILL_XUANYUN);
	specail_state->SetClearOnDie(true);
	target->AddEffect(specail_state);

	it->second.last_perform_skill_timestamp = now;

	BroadcastSkillTrigger(vir_skill_id, m_role->GetId());
}


void RoleZhuanZhiSkill::OnHurtOtherPVPUpShanghai2(Character * target, int vir_skill_id, int skill_id)
{
	if (nullptr == target || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

	Attribute cur_target_hp = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	Attribute max_target_hp = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	int hp_per = static_cast<int>(cur_target_hp / max_target_hp * 10000.0f);
	if (hp_per >= skill_cfg->param_1)
	{
		return;
	}

	int param_2 = skill_cfg->param_2;
	int param_3 = skill_cfg->param_3;

	EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), vir_skill_id, param_3, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_buff->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
	effect_buff->AddAttrEffect((char)CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, param_2);
	effect_buff->SetSave(false);
	m_role->AddEffect(effect_buff);

	it->second.last_perform_skill_timestamp = now;

	BroadcastSkillTrigger(vir_skill_id, m_role->GetId());
}


void RoleZhuanZhiSkill::OnHurtOtherPVPUpShanghai3(Character * target, int vir_skill_id, int skill_id)
{
	if (nullptr == target || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

	int param_1 = skill_cfg->param_1;
	int param_2 = skill_cfg->param_2;

	EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), vir_skill_id, param_2, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_buff->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
	effect_buff->AddAttrEffect((char)CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, param_1);
	effect_buff->SetSave(false);
	m_role->AddEffect(effect_buff);

	it->second.last_perform_skill_timestamp = now;

	BroadcastSkillTrigger(vir_skill_id, m_role->GetId());
}


void RoleZhuanZhiSkill::OnHurtOtherPVPUpShanghai4(Character * target, int vir_skill_id, int skill_id)
{
	if (nullptr == target || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

	if (it->second.level <= 0)
	{
		return;
	}

	Attribute param_1 = skill_cfg->param_1 * it->second.level;
	int param_3 = skill_cfg->param_3;

	EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), vir_skill_id, param_3, static_cast<char>(it->second.level), EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_buff->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
	effect_buff->AddAttrEffect((char)CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, param_1);
	effect_buff->SetSave(false);
	m_role->AddEffect(effect_buff);

	it->second.last_perform_skill_timestamp = now;

	BroadcastSkillTrigger(vir_skill_id, m_role->GetId());
}

void RoleZhuanZhiSkill::OnHurtOtherPVMUpShanghai(Character * target, int vir_skill_id, int skill_id, long long injure)
{
	if (nullptr == target || Obj::OBJ_TYPE_MONSTER != target->GetObjType())
	{
		return;
	}

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

	Attribute param_1 = static_cast<Attribute>(injure * (skill_cfg->param_1 * SKILL_ATTR_PER));

	EffectOnce *dec_effect = new EffectOnce(m_role->GetId(), vir_skill_id, -param_1, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	target->AddEffect(dec_effect);

	it->second.last_perform_skill_timestamp = now;

	BroadcastSkillTrigger(vir_skill_id, target->GetId());
}


void RoleZhuanZhiSkill::OnHurtOtherHurtInterval(Character * target, int vir_skill_id, int skill_id)
{
	if (nullptr == target)
	{
		return;
	}

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

	//持续伤害
	long long dec_hp = (long long)(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (skill_cfg->param_1 * SKILL_ATTR_PER));
	EffectInterval *effect_interval = new EffectInterval(m_role->GetId(), vir_skill_id, EBT_DROP_BLOOD, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_interval->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
	effect_interval->SetParam(1000, skill_cfg->param_2 / 1000, static_cast<Attribute>(-dec_hp));
	effect_interval->SetSave(true);
	target->AddEffect(effect_interval);

	it->second.last_perform_skill_timestamp = now;

	BroadcastSkillTrigger(vir_skill_id, target->GetId());
}


void RoleZhuanZhiSkill::OnHurtOtherHurt1(Character * target, int vir_skill_id, int skill_id)
{
	if (nullptr == target)
	{
		return;
	}

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	
	if (now >= it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		int rate = RandomNum(10000);
		if (rate <= skill_cfg->rate)
		{
			it->second.last_perform_skill_timestamp = now;
		}
	}

	unsigned int invalid_time = it->second.last_perform_skill_timestamp + skill_cfg->param_2 / 1000;
	if (it->second.last_perform_skill_timestamp <= 0 || now >= invalid_time)
	{
		return;
	}

	Attribute dec_hp = (Attribute)(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (skill_cfg->param_1 * SKILL_ATTR_PER));

	EffectOnce *dec_effect = new EffectOnce(m_role->GetId(), vir_skill_id, static_cast<Attribute>(-dec_hp), EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	target->AddEffect(dec_effect);

	BroadcastSkillTrigger(vir_skill_id, target->GetId());
}


void RoleZhuanZhiSkill::OnHurtOtherHurt2(Character * target, int vir_skill_id, int skill_id, long long injure)
{
	if (nullptr == target || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

	Attribute param_1 = static_cast<Attribute>(injure * (skill_cfg->param_1 * SKILL_ATTR_PER));

	EffectOnce *dec_effect = new EffectOnce(m_role->GetId(), vir_skill_id, -param_1, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	target->AddEffect(dec_effect);

	it->second.last_perform_skill_timestamp = now;

	BroadcastSkillTrigger(vir_skill_id, target->GetId());
}


void RoleZhuanZhiSkill::OnHurtOtherHurt4(Character * target, int vir_skill_id, int skill_id, long long injure)
{
	if (nullptr == target)
	{
		return;
	}

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

	Attribute param_1 = static_cast<Attribute>(injure * (skill_cfg->param_1 * SKILL_ATTR_PER));
	int param_2 = skill_cfg->param_2;

	EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), vir_skill_id, param_2, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_buff->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
	effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG, param_1);
	effect_buff->SetSave(false);
	m_role->AddEffect(effect_buff);

	it->second.last_perform_skill_timestamp = now;

	BroadcastSkillTrigger(vir_skill_id, m_role->GetId());
}


void RoleZhuanZhiSkill::OnHurtOtherHurt5(Character * target, int vir_skill_id, int skill_id, long long injure)
{
	if (nullptr == target)
	{
		return;
	}

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

	Attribute param_1 = static_cast<Attribute>(injure * (skill_cfg->param_1 * SKILL_ATTR_PER));
	int param_2 = skill_cfg->param_2;

	EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), vir_skill_id, param_2, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_buff->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
	effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_CONSTANT_MIANSHANG, param_1);
	effect_buff->SetSave(false);
	m_role->AddEffect(effect_buff);

	it->second.last_perform_skill_timestamp = now;

	BroadcastSkillTrigger(vir_skill_id, m_role->GetId());
}

void RoleZhuanZhiSkill::OnHurtOtherPVPChenmo(Character * target, int vir_skill_id, int skill_id)
{
	if (nullptr == target || Obj::OBJ_TYPE_MONSTER != target->GetObjType())
	{
		return;
	}

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

	EffectSpecialState *special_effect = new EffectSpecialState(target->GetId(), vir_skill_id, skill_cfg->param_1, EBT_CHENMO, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	special_effect->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
	special_effect->SetSave(true);
	target->AddEffect(special_effect);

	it->second.last_perform_skill_timestamp = now;

	BroadcastSkillTrigger(vir_skill_id, target->GetId());
}


void RoleZhuanZhiSkill::OnHurtOtherPVERecover(Character * target, int vir_skill_id, int skill_id, long long injure)
{
	if (nullptr == target)
	{
		return;
	}

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

	if (m_role->IsAlive())
	{
		Attribute hp = static_cast<Attribute>(injure * (skill_cfg->param_1 * SKILL_ATTR_PER));
		Attribute recover_hp = hp > skill_cfg->param_2 ? skill_cfg->param_2 : hp;

//		m_role->ChangeHp(&recover_hp, true);

		EffectInterval *effect_interval = new EffectInterval(m_role->GetId(), vir_skill_id, EBT_ADD_BLOOD, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
		effect_interval->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		effect_interval->SetParam(1000, 1, recover_hp);
		effect_interval->SetSave(false);

		m_role->AddEffect(effect_interval);

		it->second.last_perform_skill_timestamp = now;

		BroadcastSkillTrigger(vir_skill_id, m_role->GetId());
	}
}


void RoleZhuanZhiSkill::OnBeendAttackPVPDownShanghaiAttr(ObjID attacker, Attribute &delta_hp)
{
	Obj* obj = m_role->GetScene()->GetObj(attacker);
	if (NULL == obj || Obj::OBJ_TYPE_ROLE != obj->GetObjType())
	{
		return;
	}

	if (m_special_attr_pvp_jianshang <= 0)
	{
		return;
	}

	Attribute dec_hp = static_cast<Attribute>(delta_hp * (m_special_attr_pvp_jianshang* SKILL_ATTR_PER));
	delta_hp -= dec_hp;
}


void RoleZhuanZhiSkill::OnBeendAttackPVEDownShanghaiAttr(ObjID attacker, Attribute &delta_hp)
{
	Obj* obj = m_role->GetScene()->GetObj(attacker);
	if (NULL == obj || Obj::OBJ_TYPE_MONSTER != obj->GetObjType())
	{
		return;
	}

	if (m_special_attr_pve_jianshang <= 0)
	{
		return;
	}

	Attribute dec_hp = static_cast<Attribute>(delta_hp * (m_special_attr_pve_jianshang* SKILL_ATTR_PER));
	delta_hp -= dec_hp;
}

bool RoleZhuanZhiSkill::IsMianyiDeadlyAttack()
{
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	bool is_mianyi = false;
	for (auto it = m_cur_active_skill_map.begin(); it != m_cur_active_skill_map.end(); it++)
	{

		if (it->second.vir_skill_type != ZHUANZHI_VIR_SKILL_TYPE_DEADLY_ATTACK_MIANYI)
		{
			continue;
		}

		const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(it->first);
		if (NULL == skill_cfg) continue;

		if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
		{
			continue;
		}

		int rate = RandomNum(10000);
		if (rate <= skill_cfg->rate)
		{
			if (m_role->IsAlive())
			{
				Attribute recover_hp = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (skill_cfg->param_1 * SKILL_ATTR_PER));
//				m_role->ChangeHp(&recover_hp, true);

				EffectInterval *effect_interval = new EffectInterval(m_role->GetId(), it->second.vir_skill_id, EBT_ADD_BLOOD, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
				effect_interval->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
				effect_interval->SetParam(1000, 1, recover_hp);
				effect_interval->SetSave(false);

				m_role->AddEffect(effect_interval);
			}

			BroadcastSkillTrigger(it->first, m_role->GetId());

			is_mianyi = true;
			break;
		}

		it->second.last_perform_skill_timestamp = now;
	}

	return is_mianyi;
}


void RoleZhuanZhiSkill::OnBeenAttackNormalHpStore(int vir_skill_id, int skill_id)
{
	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

	Attribute param_1 = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP) * (skill_cfg->param_1 * SKILL_ATTR_PER));
	int param_2 = skill_cfg->param_2;
	float param_3 = static_cast<float>(skill_cfg->param_3 / 100.0f);

	EffectHpStore *effect_hp_store = new EffectHpStore(m_role->GetId(), vir_skill_id, param_2, param_1, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_hp_store->SetReplacePercent(param_3);
	effect_hp_store->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	m_role->AddEffect(effect_hp_store);

	it->second.last_perform_skill_timestamp = now;

	BroadcastSkillTrigger(vir_skill_id, m_role->GetId());
}


void RoleZhuanZhiSkill::OnBeenAttackWudiHpStore(int vir_skill_id, int skill_id)
{
	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

	int param_1 = skill_cfg->param_1;

	EffectWudiProtect *effect_wudi = new EffectWudiProtect(m_role->GetId(), vir_skill_id, param_1, 100, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_wudi->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	m_role->AddEffect(effect_wudi);

	it->second.last_perform_skill_timestamp = now;

	BroadcastSkillTrigger(vir_skill_id, m_role->GetId());
}


void RoleZhuanZhiSkill::OnBeenAttackPVPRebound(ObjID attacker, int vir_skill_id, int skill_id, long long delta_hp)
{
	Obj* obj = m_role->GetScene()->GetObj(attacker);
	if (NULL == obj || Obj::OBJ_TYPE_ROLE != obj->GetObjType())
	{
		return;
	}
	Role *attacker_role = (Role*)obj;

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	SpecialLogic *logic = m_role->GetScene()->GetSpecialLogic();
	if (!logic)
	{
		return;
	}
	if (!logic->SpecialCanRebound(m_role, (Character *)obj))
	{
		return;
	}

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

//	int param_1 = static_cast<int>(delta_hp * (skill_cfg->param_1 * SKILL_ATTR_PER));

// 	EffectOnce *dec_effect = new EffectOnce(m_role->GetId(), vir_skill_id, param_1, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
// 	attacker_role->AddEffect(dec_effect);
// 
// 	EffectOnce *add_effect = new EffectOnce(m_role->GetId(), vir_skill_id, -(Attribute)delta_hp, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
// 	m_role->AddEffect(add_effect);

	if (vir_skill_id != skill_id)
	{
		EffectReboundHurt *effect_rebound = new EffectReboundHurt(m_role->GetId(), vir_skill_id, 1000, skill_cfg->param_1 / 100, 1, EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
		m_role->AddEffect(effect_rebound);

		it->second.last_perform_skill_timestamp = now;

		BroadcastSkillTrigger(vir_skill_id, attacker_role->GetId());
	}
}


void RoleZhuanZhiSkill::OnBeenAttackPVPDownShanghai(ObjID attacker, int vir_skill_id, int skill_id)
{
	Obj* obj = m_role->GetScene()->GetObj(attacker);
	if (NULL == obj || Obj::OBJ_TYPE_ROLE != obj->GetObjType())
	{
		return;
	}
	//Role *attacker_role = (Role*)obj;

	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 如果是跟随主动技能的
	if (it->second.follow_perform_skill_id > 0 && it->second.follow_perform_skill_id != skill_id)
	{
		return;
	}

	const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(vir_skill_id);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//CD
	if (now < it->second.last_perform_skill_timestamp + skill_cfg->cd_time)
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate > skill_cfg->rate)
	{
		return;
	}

	if (it->second.level < skill_cfg->param_2)
	{
		it->second.level++;
	}

	Attribute param_1 = skill_cfg->param_1 * it->second.level;
	int param_3 = skill_cfg->param_3;

	EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), vir_skill_id, param_3, static_cast<char>(it->second.level), EffectBase::PRODUCT_METHOD_ZHUANZHI_SKILL);
	effect_buff->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);
	effect_buff->AddAttrEffect((char)CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, param_1);
	effect_buff->SetSave(false);
	m_role->AddEffect(effect_buff);

	it->second.last_perform_skill_timestamp = now;

	BroadcastSkillTrigger(vir_skill_id, m_role->GetId());
}


void RoleZhuanZhiSkill::CheckBeenAttack(unsigned int now)
{
	for (auto it = m_cur_active_skill_map.begin(); it != m_cur_active_skill_map.end(); it++)
	{
		if (it->second.vir_skill_type != ZHUANZHI_VIR_SKILL_TYPE_PVP_DOWN_SHANGHAI)
		{
			continue;
		}

		if (it->second.last_perform_skill_timestamp <= 0)
		{
			continue;
		}

		const ZhuanzhiVirSkillCfg *skill_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiVirSkillCfg(it->first);
		if (NULL == skill_cfg) continue;

		unsigned int dur_s = static_cast<unsigned int>(skill_cfg->param_3 / 1000);

		if (now >= it->second.last_perform_skill_timestamp + dur_s)
		{
			it->second.level = 0;
		}
	}
}

void RoleZhuanZhiSkill::BroadcastSkillTrigger(int vir_skill_id, ObjID target_obj_id, long long injure, bool send_to_role)
{
	auto it = m_cur_active_skill_map.find(vir_skill_id);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	static Protocol::SCZhuanzhiSkillTrigger msg;

	msg.skill_id = vir_skill_id;
	msg.target_obj_id = target_obj_id;
	msg.deliver_obj_id = m_role->GetId();
	msg.last_perform = it->second.last_perform_skill_timestamp;
	msg.injure = injure;

	if (send_to_role)
	{
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&msg, sizeof(msg));
	}
	else
	{
		m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void*)&msg, sizeof(msg));
	}
}
