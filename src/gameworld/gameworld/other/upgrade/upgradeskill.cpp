#include "upgradeskill.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/event/eventhandler.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"

#include "servercommon/noticenum.h"
#include "servercommon/errornum.h"
#include "servercommon/struct/upgradeparam.hpp"
#include "scene/scene.h"
#include "scene/speciallogic/speciallogic.hpp"
#include "obj/character/monster.h"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectinterval.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effecthurtinc.hpp"
#include "effect/skilleffect/effecthpstore.hpp"
#include "effect//skilleffect/effectwudiprotect.hpp"
#include "skill/skillpool.hpp"
#include "item/knapsack.h"
#include "skill/skillbase.hpp"
#include "friend/teaminfo.hpp"
#include "protocal/msgrole.h"
#include "engineadapter.h"

#include "config/upgradeconfig/upgradeconfigmanager.hpp"
#include "config/upgradeconfig/upgradeconfig.hpp"
#include "other/upgrade/upgrademanager.hpp"

#include <algorithm>

UpgradeSkill::UpgradeSkill() : m_role(nullptr)
{

}

UpgradeSkill::~UpgradeSkill()
{

}

void UpgradeSkill::Init(Role *role, UpgradeManager *upgrade_manager)
{
	m_role = role;
	m_upgrade_manager = upgrade_manager;
}


void UpgradeSkill::OnInitFinish()
{
	this->RecalcActivedSkill();
}

void UpgradeSkill::RecalcActivedSkill()
{
	for (int upgrade_type = 0; upgrade_type < UPGRADE_TYPE_MAX; upgrade_type ++)
	{
		if (!m_upgrade_manager->IsValidUpgradeType(upgrade_type))
		{
			continue;
		}

		int skill_level = m_upgrade_manager->GetSkill0Level(upgrade_type);
		if (skill_level <= 0)
		{
			continue;
		}

		const auto upgrade_cfg = UPGRADE_CFG_MGR(upgrade_type);
		if (!upgrade_cfg)
		{
			continue;
		}

		const SkillConifg *skill_cfg = upgrade_cfg->GetSkillCfg(0, skill_level);
		if (nullptr == skill_cfg)
		{
			continue;
		}

		auto it = m_cur_active_skill_map.find(upgrade_type);
		if (it == m_cur_active_skill_map.end())
		{
			VirSkillItem skill_item;
			skill_item.upgrade_type = upgrade_type;
			skill_item.level = skill_level;
			skill_item.param_a = skill_cfg->param_a;
			skill_item.param_b = skill_cfg->param_b;
			skill_item.param_c = skill_cfg->param_c;
			skill_item.param_d = skill_cfg->param_d;

			m_cur_active_skill_map[upgrade_type] = skill_item;
		}
	}
}

void UpgradeSkill::OnHurtOther(Character * target, int skill_id, long long injure, int fight_type)
{

}

void UpgradeSkill::OnRoleBeenAttack(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill)
{
	Obj* obj = m_role->GetScene()->GetObj(attacker);
	if (NULL == obj)
	{
		return;
	}
}

void UpgradeSkill::OnAfterHurtOther(Character *target, int skill_id, long long injure, bool from_skill, int fight_type)
{
	OnHurtOtherUpgrade2(target, injure, from_skill);
}

// 攻击时有param_b%概率造成流血，每秒造成param_c%攻击的无视防御伤害，持续param_d毫秒。冷却param_a秒
void UpgradeSkill::OnHurtOtherUpgrade2(Character * target, long long injure, bool from_skill)
{
	if (nullptr == target)
	{
		return;
	}

	auto it = m_cur_active_skill_map.find(UPGRADE_TYPE_2);
	if (it == m_cur_active_skill_map.end())
	{
		return;
	}

	// 2019-03-21 策划需求，当角色伤害小于20点伤害的时候，灵童不造成伤害
	if (injure < 20)
	{
		return;
	}

	if (!from_skill)
	{
		return;
	}

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (now_sec < it->second.last_perform_skill_timestamp) return;

	if (RandomNum(10000) < it->second.param_b)
	{
		Attribute hurt_hp = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * it->second.param_c * ROLE_ATTR_PER);
		int count = it->second.param_d / 1000;
		if (count < 0) count = 1;

		EffectInterval *effect_interval = new EffectInterval(m_role->GetId(), 1, EBT_DROP_BLOOD, 1, EffectBase::PRODUCT_METHOD_LINGCHONG);
		effect_interval->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
		effect_interval->SetParam(1000, count, -hurt_hp);
		effect_interval->SetFightType(EffectBase::FIGHT_TYPE_LINGCHONG);
		effect_interval->SetSave(false);
		effect_interval->SetClientEffectType(EffectBase::ECT_SKILL_DROP_BLOOD);
		target->AddEffect(effect_interval);

		it->second.last_perform_skill_timestamp = now_sec + (it->second.param_a);
	}
}
