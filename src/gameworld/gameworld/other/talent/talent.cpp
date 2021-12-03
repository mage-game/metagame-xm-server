#include "other/talent/talent.hpp"
#include "other/talent/talentconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectinterval.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "gameworld/scene/scene.h"
#include "gameworld/scene/speciallogic/speciallogic.hpp"
#include "other/capability/capability.hpp"
#include "item/knapsack.h"
#include "skill/skillbase.hpp"
#include "engineadapter.h"
#include "effect/skilleffect/effecttalenthpstore.hpp"
#include "effect/skilleffect/effectforbidrecover.hpp"
#include "gameworld/gamecommon.h"
#include "effect/skilleffect/effectspecialstate.hpp"

Talent::Talent() : m_role(NULL)
{
	
}

Talent::~Talent()
{

}

void Talent::InitParam(Role *role, const TalentParam &param)
{
	m_role = role;
	m_param = param;
}

void Talent::GetInitParam(TalentParam *param) const
{
	*param = m_param;
}

bool Talent::GetAttrTypeFromAttrString(std::string &attr_str, short *attr_type)
{
	for (int i = 0; i < static_cast<int>(sizeof(gs_config_talent_attr_define_string_array) / sizeof(gs_config_talent_attr_define_string_array[0])); i++)
	{
		if (attr_str == gs_config_talent_attr_define_string_array[i].talent_attr_name)
		{
			if (NULL != attr_type) *attr_type = gs_config_talent_attr_define_string_array[i].talent_attr_type;
			return true;
		}
	}

	return false;
}

void Talent::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
 	if (is_recalc)
 	{
 		m_attrs_add.Reset();

		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] = static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_MAXHP));
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] = static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_GONGJI));
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] = static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_FANGYU));
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] = static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_BAOJI));
		// 角色暴击（暴击伤害率）
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] = static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_BAOJI_PER));
		// 角色坚韧(抗暴)
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] = static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_JIANREN));
		// 角色破防百分比（增伤率）
//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] = static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_POFANG));
		// pvp减伤万分比
//		m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] = static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_PVP_JIANSHANG_PER));
		// pvp增伤万分比
//		m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] = static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_PVP_ZENGSHANG_PER));
		// pve减伤万分比
//		m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] = static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_PVE_JIANSHANG_PER));
		// pve增伤万分比
//		m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] = static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_PVE_ZENGSHANG_PER));
		//技能增伤万分比
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER] = static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_SKILL_ZENGSHANG_PER));
		//技能减伤万分比
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER] = static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_SKILL_JIANSHANG_PER));
		//技能战力万分比
		m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_SKILL_CAP_PER] += this->ReCalSkillCapPer();

		// 下面特殊属性用乘法
		m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_POFANG)) );
		m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_PVP_ZENGSHANG_PER)) );
		m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_PVE_ZENGSHANG_PER)) );
		m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_PVP_JIANSHANG_PER)) );
		m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, static_cast<Attribute>(this->GetTalentAttrValue(TALENT_ATTR_TYPE_PVE_JIANSHANG_PER)) );
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_TALENT, m_attrs_add);
	this->ReCalExtraCap();

	base_add.Add(m_attrs_add.m_attrs, true);
}

void Talent::OnUpLevel(int old_level, int new_level)
{
	//const TalentOtherCfg &other_cfg = LOGIC_CONFIG->GetTalentConfig().GetTalentOtherCfg();

	int level_point = 0;
	for (int level = old_level + 1; level <= new_level; level ++)
	{
		int add_talent_point = LOGIC_CONFIG->GetTalentConfig().GetLvAddTalentPoint(level);

		level_point += add_talent_point;
	}

	m_param.talent_point += level_point;

	this->SendTalentInfo();
}


void Talent::OnZhuanzhi()
{
	int add_talent_point = LOGIC_CONFIG->GetTalentConfig().GetZhuanzhiLvAddTalentPoint(m_role->GetProfLevel());
	if (add_talent_point <= 0)
	{
		return;
	}

	m_param.talent_point += add_talent_point;

	this->SendTalentInfo();
}

bool Talent::OnUseTalenBook(int talent_point)
{
	if (talent_point <= 0) return false;
	
	m_param.talent_point += talent_point;
	m_param.talent_book_add_point += talent_point;

	this->SendTalentInfo();

	return true;
}

void Talent::Update(unsigned long interval, unsigned int now_second)
{
	for (int i = 0; i < TALENT_VIR_SKILL_TYPE_MAX; i++)
	{
		//效果持续时间
		if (m_vir_skill_info[i].skill_effect_time_ms > 0)
		{
			m_vir_skill_info[i].skill_effect_time_ms -= interval;
		}

		if (m_vir_skill_info[i].skill_effect_time_ms <= 0)
		{
			m_vir_skill_info[i].skill_effect_time_ms = 0;
			m_vir_skill_info[i].skill_stack_count = 0;
		}
	}

}

//static const int SPECIAL_TALENT_ID_107 = 107;	// 摄魂夺魄	攻击时有万分之param_a的概率使玩家直接损失当前生命值的万分之param_b
void Talent::OnHurtOtherTalent107(Character * target, int skill_id, long long injure, int fight_type)
{
	if (target->GetObjType() != Obj::OBJ_TYPE_ROLE)
	{
		return;
	}
	Role* target_role = static_cast<Role*>(target);

	const TalentLevelConifg *cfg = GetTalentLevelCfg(SPECIAL_TALENT_ID_107);
	if (NULL == cfg)
	{
		return;
	}

	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_time < m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_107].last_perform_timestamp + cfg->cool_down)
		return;

	int rand_val = RandomNum(10000);
	if (rand_val < cfg->param_a)
	{
		int dec_hp = (int)(target_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (cfg->param_b * SKILL_ATTR_PER));
		EffectOnce *dechp_effect = new EffectOnce(m_role->GetId(), 0, -dec_hp, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_TALENT_SKILL);
		target_role->AddEffect(dechp_effect);

		m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_107].last_perform_timestamp = now_time;
	}
}

void Talent::OnHurtOtherTalent108(Character * target, int skill_id, long long injure, int fight_type)
{
	if (target->GetObjType() != Obj::OBJ_TYPE_ROLE)
	{
		return;
	}

	const TalentLevelConifg *cfg = GetTalentLevelCfg(SPECIAL_TALENT_ID_108);
	if (NULL == cfg)
	{
		return;
	}

	int rand_val = RandomNum(10000);
	if (rand_val < cfg->param_d)
	{
		m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_108].skill_effect_time_ms = cfg->param_c;
		if (m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_108].skill_stack_count < cfg->param_b)
		{
			m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_108].skill_stack_count++;
		}
	}
}

void Talent::OnHurtOtherTalent110(Character * target, int skill_id, long long injure, int fight_type)
{
	if (target->GetObjType() != Obj::OBJ_TYPE_ROLE)
	{
		return;
	}
	Role* target_role = static_cast<Role*>(target);

	const TalentLevelConifg *cfg = GetTalentLevelCfg(SPECIAL_TALENT_ID_110);
	if (NULL == cfg)
	{
		return;
	}

	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_time >= m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_110].last_perform_timestamp + cfg->cool_down)
	{
		int rand_val = RandomNum(10000);
		if (rand_val < cfg->param_a)
		{
			//持续流血
			long long dec_hp = (long long)(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (cfg->param_b * SKILL_ATTR_PER));
			EffectInterval *effect_interval = new EffectInterval(m_role->GetId(), TALENT_VIR_SKILL_INDEX_ID_110, EBT_DROP_BLOOD, 1, EffectBase::PRODUCT_METHOD_SKILL);  // 参数skill设置为非角色技能ID.防止PVP血量上下限修正
			effect_interval->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
			effect_interval->SetParam(1000, cfg->param_c / 1000, static_cast<int>(-dec_hp));
			effect_interval->SetSave(true);
			effect_interval->SetClientEffectType(EffectBase::ECT_SKILL_DROP_BLOOD);
			target_role->AddEffect(effect_interval);

			m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_110].last_perform_timestamp = now_time;
		}
	}
}

void Talent::OnHurtOtherTalent414(Character * target, int skill_id, long long injure, int fight_type)
{
	const TalentLevelConifg *cfg = GetTalentLevelCfg(SPECIAL_TALENT_ID_414);
	if (NULL == cfg)
	{
		return;
	}

	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_time < m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_414].last_perform_timestamp + cfg->cool_down)
		return;


	int rand_val = RandomNum(10000);
	if (rand_val < cfg->param_a)
	{
		EffectOnce *dechp_effect = new EffectOnce(m_role->GetId(), 0, static_cast<Attribute>(injure * (cfg->param_b * SKILL_ATTR_PER)), EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_TALENT_SKILL);
		m_role->AddEffect(dechp_effect);
		m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_414].last_perform_timestamp = now_time;
	}
}

void Talent::OnHurtOtherTalent412(Character * target, int skill_id, long long injure, int fight_type)
{
	if (NULL == target || target->GetObjType() != Obj::OBJ_TYPE_ROLE)
	{
		return;
	}
	Role* target_role = static_cast<Role*>(target);

	const TalentLevelConifg *cfg = GetTalentLevelCfg(SPECIAL_TALENT_ID_412);
	if (NULL == cfg)
	{
		return;
	}

	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_time < m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_412].last_perform_timestamp + cfg->cool_down)
		return;

	int rand_val = RandomNum(10000);
	if (rand_val < cfg->param_a)
	{
		//target禁疗debuf
		EffectForbidRecover *effect_forbid = new EffectForbidRecover(INVALID_OBJ_ID, 0, cfg->param_b, 1, EffectBase::PRODUCT_METHOD_TALENT_SKILL);
		effect_forbid->SetClearOnDie(true);
		target_role->AddEffect(effect_forbid);

		m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_412].last_perform_timestamp = now_time;
	}
}

void Talent::OnHurtOtherTalent115(Character * target, int skill_id, long long injure, int fight_type)
{
	if (target->GetObjType() != Obj::OBJ_TYPE_ROLE)
	{
		return;
	}
	Role* target_role = static_cast<Role*>(target);

	const TalentLevelConifg *cfg = GetTalentLevelCfg(SPECIAL_TALENT_ID_115);
	if (NULL == cfg)
	{
		return;
	}

	int rand_val = RandomNum(10000);
	if (rand_val < cfg->param_a)
	{
		EffectOnce *dechp_effect = new EffectOnce(m_role->GetId(), 0, -cfg->param_b, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_TALENT_SKILL);
		target_role->AddEffect(dechp_effect);
	}
}

void Talent::OnHurtOtherTalent310(Character * target, int skill_id, long long injure, int fight_type)
{
	const TalentLevelConifg *cfg = GetTalentLevelCfg(SPECIAL_TALENT_ID_310);
	if (NULL == cfg)
	{
		return;
	}

	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_time >= m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_310].last_perform_timestamp + cfg->cool_down)
	{
		int rand_val = RandomNum(10000);
		if (rand_val < cfg->param_a)
		{
			Attribute add_gongji = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (cfg->param_b * SKILL_ATTR_PER));

			EffectBuff *effect = new EffectBuff(m_role->GetId(), 1, cfg->param_c, 1, EffectBase::PRODUCT_METHOD_TALENT_SKILL);

			effect->AddGongJiEffect(add_gongji, cfg->param_b);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
			effect->SetClientEffectType(EffectBase::ECT_SKILL_ADD_GONGJI);
			m_role->AddEffect(effect);
			m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_310].last_perform_timestamp = now_time;
		}
	}
}

void Talent::OnHurtOther(Character * target, int skill_id, long long injure, int fight_type)
{
	OnHurtOtherTalent107(target, skill_id, injure, fight_type);
	OnHurtOtherTalent108(target, skill_id, injure, fight_type);
	OnHurtOtherTalent110(target, skill_id, injure, fight_type);
	OnHurtOtherTalent414(target, skill_id, injure, fight_type);
	OnHurtOtherTalent412(target, skill_id, injure, fight_type);
	OnHurtOtherTalent115(target, skill_id, injure, fight_type);
	OnHurtOtherTalent310(target, skill_id, injure, fight_type);
	OnHurtOtherTalent306();
}

void Talent::OnRoleBeenAttack(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill)
{
	OnRoleBeenAttackTalent206(attacker, skill_id, delta_hp, from_skill);
	OnRoleBeenAttackTalent207(attacker, skill_id, delta_hp, from_skill);
	OnRoleBeenAttackTalent208(attacker, skill_id, delta_hp, from_skill);
	OnRoleBeenAttackTalent209(attacker, skill_id, delta_hp, from_skill);
	OnRoleBeenAttackTalent210(attacker, skill_id, delta_hp, from_skill);
	OnRoleBeenAttackTalent311(attacker, skill_id, delta_hp, from_skill);
	OnRoleBeenAttackTalent312(attacker, skill_id, delta_hp, from_skill);
	OnRoleBeenAttackTalent417(attacker, skill_id, delta_hp, from_skill);
	
}

void Talent::OtherModuleReCalcAttr(int typ, const CharIntAttrs &base_attr, CharIntAttrs &target_attr)
{
	//属性类型, 加成万分比
	std::map<CharIntAttrs::CHARINTATTR_TYPE, Attribute> add_per;
	switch (typ)
	{
	case TALENT_OTHER_MODULE_FABAO:
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, GetTalentAttrValue(TALENT_ATTR_TYPE_FABAO_GONGJI_PER)));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, GetTalentAttrValue(TALENT_ATTR_TYPE_FABAO_FANGYU_PER)));
		break;
	case TALENT_OTHER_MODULE_FIGHT_MOUNT:
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, GetTalentAttrValue(TALENT_ATTR_TYPE_FIGHT_MOUNT_GONGJI_PER)));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, GetTalentAttrValue(TALENT_ATTR_TYPE_FIGHT_MOUNT_FANGYU_PER)));
		break;
	case TALENT_OTHER_MODULE_MOUNT:
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, GetTalentAttrValue(TALENT_ATTR_TYPE_MOUNT_GONGJI_PER)));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, GetTalentAttrValue(TALENT_ATTR_TYPE_MOUNT_FANGYU_PER)));
		break;
	case TALENT_OTHER_MODULE_WING:
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, GetTalentAttrValue(TALENT_ATTR_TYPE_WING_GONGJI_PER)));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, GetTalentAttrValue(TALENT_ATTR_TYPE_WING_FANGYU_PER)));
		break;
	case TALENT_OTHER_MODULE_HALO:
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, GetTalentAttrValue(TALENT_ATTR_TYPE_HALO_GONGJI_PER)));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, GetTalentAttrValue(TALENT_ATTR_TYPE_HALO_FANGYU_PER)));
		break;
	case TALENT_OTHER_MODULE_SHENBING:
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, GetTalentAttrValue(TALENT_ATTR_TYPE_SHENBING_GONGJI_PER)));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, GetTalentAttrValue(TALENT_ATTR_TYPE_SHENBING_FANGYU_PER)));
		break;
	case TALENT_OTHER_MODULE_SHENYI:
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, GetTalentAttrValue(TALENT_ATTR_TYPE_SHENYI_GONGJI_PER)));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, GetTalentAttrValue(TALENT_ATTR_TYPE_SHENYI_FANGYU_PER)));
		break;
	case TALENT_OTHER_MODULE_SHENGONG:
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, GetTalentAttrValue(TALENT_ATTR_TYPE_SHENGONG_GONGJI_PER)));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, GetTalentAttrValue(TALENT_ATTR_TYPE_SHENGONG_FANGYU_PER)));
		break;
	case TALENT_OTHER_MODULE_SHIZHUANG:
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, GetTalentAttrValue(TALENT_ATTR_TYPE_SHIZHUANG_GONGJI_PER)));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, GetTalentAttrValue(TALENT_ATTR_TYPE_SHIZHUANG_FANGYU_PER)));
		break;
	case TALENT_OTHER_MODULE_FOOTPRINT:
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, GetTalentAttrValue(TALENT_ATTR_TYPE_FOOTPRINT_GONGJI_PER)));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, GetTalentAttrValue(TALENT_ATTR_TYPE_FOOTPRINT_FANGYU_PER)));
		break;
	case TALENT_OTHER_MODULE_SHENSHOU:
		{
			auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_SHENSHOU_ALL_PER);
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, GetTalentAttrValue(TALENT_ATTR_TYPE_SHENSHOU_GONGJI_PER) + all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, GetTalentAttrValue(TALENT_ATTR_TYPE_SHENSHOU_FANGYU_PER) + all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
		}
		break;
	case TALENT_OTHER_MODULE_YUSHI:
		{
			auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_YUSHI_ALL_PER);
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, GetTalentAttrValue(TALENT_ATTR_TYPE_YUSHI_GONGJI_PER) + all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, GetTalentAttrValue(TALENT_ATTR_TYPE_YUSHI_FANGYU_PER) + all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
		}
		break;
	case TALENT_OTHER_MODULE_STONE:
		{
			auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_STONE_ALL_PER);
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, GetTalentAttrValue(TALENT_ATTR_TYPE_STONE_GONGJI_PER) + all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, GetTalentAttrValue(TALENT_ATTR_TYPE_STONE_FANGYU_PER) + all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, all_per));
		}
		break;
	case TALENT_OTHER_MODULE_YONGHENG_SUIT:
		{
			auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_YONGHENG_SUIT_PER);
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
		}
		break;
	case TALENT_OTHER_MODULE_ZHUXIAN_SUIT:
		{
			auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_ZHUXIAN_SUIT_PER);
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
		}
		break;
	case TALENT_OTHER_MODULE_ZHUSHEN_SUIT:
		{
			auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_ZHUSHEN_SUIT_PER);
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
		}
		break;
	case TALENT_OTHER_MODULE_NORMAL_EQUIP:
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, GetTalentAttrValue(TALENT_ATTR_TYPE_NORMAL_EQUIP_GONGJI_PER)));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, GetTalentAttrValue(TALENT_ATTR_TYPE_NORMAL_EQUIP_FANGYU_PER)));
		break;
	case TALENT_OTHER_MODULE_ZHUANZHI_EQUIP:
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, GetTalentAttrValue(TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_GONGJI_PER)));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, GetTalentAttrValue(TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_FANGYU_PER)));
		break;

	case TALENT_OTHER_MODULE_FABAO_BASE:
		{
			auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_FABAO_UPGRADE_PER);
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
			add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
		}
		break;

	case TALENT_OTHER_MODULE_WING_BASE:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_WING_UPGRADE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}

		break;

	case TALENT_OTHER_MODULE_MOUNT_BASE:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_MOUNT_UPGRADE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}
		break;

	case TALENT_OTHER_MODULE_SHENBING_BASE:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_SHENGBING_UPGRADE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}
		break;

	case TALENT_OTHER_MODULE_SHIZHUANG_BASE:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_SHIZHUANG_UPGRADE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}
		break;

	case TALENT_OTHER_MODULE_FIGHT_MOUNT_BASE:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_FIGHT_MOUNT_UPGRADE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}
		break;

	case TALENT_OTHER_MODULE_SHENGONG_BASE:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_SHENGONG_UPGRADE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}
		break;

	case TALENT_OTHER_MODULE_HALO_BASE:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_HALO_UPGRADE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}
		break;

	case TALENT_OTHER_MODULE_TOUSHI_BASE:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_TOUSHI_UPGRADE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}
		break;

	case TALENT_OTHER_MODULE_YAOSHI_BASE:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_YAOSHI_UPGRADE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}
		break;

	case TALENT_OTHER_MODULE_MASK_BASE:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_MASK_UPGRADE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}
		break;

	case TALENT_OTHER_MODULE_QILINBI_BASE:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_QILINBI_UPGRADE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}
		break;

	case TALENT_OTHER_MODULE_UPGRADE_2:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_LINGTONG_UPGRADE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}
		break;

	case TALENT_OTHER_MODULE_UPGRADE_3:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_LINGGONG_UPGRADE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}
		break;

	case TALENT_OTHER_MODULE_UPGRADE_4:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_LINGQI_UPGRADE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}
		break;

	case TALENT_OTHER_MODULE_UPGRADE_5:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_WEIYAN_UPGRADE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}
		break;

	case TALENT_OTHER_MODULE_NORMAL_EQUIP_BASE:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_EQUIP_BASE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}
		break;

	case TALENT_OTHER_MODULE_ZHUANZHI_EQUIP_BASE:
	{
		auto all_per = GetTalentAttrValue(TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_BASE_PER);
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, all_per));
		add_per.insert(std::make_pair(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, all_per));
	}
		break;

	default:
		break;
	}

	for (auto& value : add_per)
	{
		target_attr.AddValue(value.first, static_cast<Attribute>(base_attr.GetAttr(value.first) * (value.second * ROLE_ATTR_PER)));
	}
}


void Talent::OnKillOtherRole(Role *target_role)
{
	const int talent_id_list[2] = { SPECIAL_TALENT_ID_109, SPECIAL_TALENT_ID_209};
	for (int i = 0; i < 2; i++)
	{
		int talent_id = talent_id_list[i];
		int talent_level = this->GetTalentLevel(talent_id);
		if (talent_level <= 0) continue;

		const TalentLevelConifg *cfg = LOGIC_CONFIG->GetTalentConfig().GetTalentLevelCfg(talent_id, talent_level);
		if (NULL == cfg) continue;

		unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		if (SPECIAL_TALENT_ID_109 == talent_id)
		{
			if (now_time >= m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_109].last_perform_timestamp + cfg->cool_down)
			{
				m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_109].last_perform_timestamp = now_time;
				m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_109].skill_effect_time_ms = cfg->param_b;
				m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_109].skill_stack_count = 1;

				EffectSpecialState *effect = new EffectSpecialState(m_role->GetId(), 1, cfg->param_b, EBT_INVALID, 1);
				effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
				effect->SetParam(cfg->param_a * 1000);    // 客户端将定义CommonBuff = 1（并且除EffectBuff以外）的BUFF的参数0和参数2除以1000
				effect->SetClientEffectType(EffectBase::ECT_TALENT_INC_HURT);
				m_role->AddEffect(effect);
			}
		}
		else if (SPECIAL_TALENT_ID_209 == talent_id)
		{
			if (now_time >= m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_209].last_perform_timestamp + cfg->cool_down)
			{
				m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_209].last_perform_timestamp = now_time;
				m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_209].skill_effect_time_ms = cfg->param_b;
				m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_209].skill_stack_count = 1;

				EffectSpecialState *effect = new EffectSpecialState(m_role->GetId(), 1, cfg->param_b, EBT_INVALID, 1);
				effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
				effect->SetParam(cfg->param_a * 1000);    // 客户端将定义CommonBuff = 1（并且除EffectBuff以外）的BUFF的参数0和参数2除以1000
				effect->SetClientEffectType(EffectBase::ECT_TALENT_DEC_ON_HURT);
				m_role->AddEffect(effect);
			}
		}
	}
}
/*
long long Talent::GetTalentExtraValue(int talent_attr_type, long long base_value)
{
	if (talent_attr_type <= TALENT_ATTR_TYPE_INVALID || talent_attr_type >= TALENT_ATTR_TYPE_MAX)
	{
		return 0;
	}

	const long long attr_value = this->GetTalentAttrValue(talent_attr_type);

	//if (TALENT_ATTR_TYPE_ATTACK_FUJIA_SHANGHAI == talent_attr_type)
	//{
	//	return attr_value;
	//}

	return static_cast<long long>(base_value * (attr_value * ROLE_ATTR_PER));
}
*/
long long Talent::GetTalentFixInjure(Character *target, long long injure, const int skill_id)
{
	long long add_injure = 0;

	// 目标天赋
	const int target_talent_id_list[2] = { SPECIAL_TALENT_ID_208, SPECIAL_TALENT_ID_209 };
	const int target_virskill_id_list[2] = { TALENT_VIR_SKILL_INDEX_ID_208, TALENT_VIR_SKILL_INDEX_ID_209 };

	if (NULL != target && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		Role *target_role = (Role *)target;

		for (int i = 0; i < 2; i++)
		{
			int talent_id = target_talent_id_list[i];
			int virskill_id = target_virskill_id_list[i];

			int talent_level = target_role->GetTalent()->GetTalentLevel(talent_id);
			if (talent_level <= 0) continue;

			const TalentLevelConifg *cfg = LOGIC_CONFIG->GetTalentConfig().GetTalentLevelCfg(talent_id, talent_level);
			if (NULL == cfg) continue;

			const TalentVirSkillInfo *vir_skill_info = target_role->GetTalent()->GetTalentVirSkillInfo(virskill_id);
			if (NULL == vir_skill_info) continue;

			add_injure -= static_cast<long long>(injure * (cfg->param_a * (vir_skill_info->skill_stack_count * SKILL_ATTR_PER)));
		}
	}

	// 自身天赋
	const int talent_id_list[2] = { SPECIAL_TALENT_ID_108, SPECIAL_TALENT_ID_109 };
	const int virskill_id_list[2] = { TALENT_VIR_SKILL_INDEX_ID_108, TALENT_VIR_SKILL_INDEX_ID_109 };
	for (int i = 0; i < 2; i++)
	{
		int talent_id = talent_id_list[i];
		int virskill_id = virskill_id_list[i];

		int talent_level = this->GetTalentLevel(talent_id);
		if (talent_level <= 0) continue;

		const TalentLevelConifg *cfg = LOGIC_CONFIG->GetTalentConfig().GetTalentLevelCfg(talent_id, talent_level);
		if (NULL == cfg) continue;

		add_injure += static_cast<long long>(injure * (cfg->param_a * (m_vir_skill_info[virskill_id].skill_stack_count * SKILL_ATTR_PER)));
	}

	// 如果是技能
	if (!m_role->GetSkillManager()->IsCommonSkill(skill_id))
	{
		// 天赋技能 - 技能精通 - 增加伤害
		long long attr_value = this->GetTalentAttrValue(TALENT_ATTR_TYPE_SKILL_HURT_ADD_PER);


		if (NULL != target && Obj::OBJ_TYPE_ROLE == target->GetObjType())
		{
			// 天赋技能 - 技能精通 - 减免伤害
			attr_value -= this->GetTalentAttrValue(TALENT_ATTR_TYPE_SKILL_HURT_MINUS_PER);
		}

		add_injure += static_cast<long long>(injure * (attr_value * ROLE_ATTR_PER));
	}
	return add_injure;
}

long long Talent::DecHurtOnRoleBeenAttack(long long injure)
{
	long long add_injure = 0;

	int talent_level = this->GetTalentLevel(SPECIAL_TALENT_ID_412);
	if (talent_level > 0)
	{
		const TalentLevelConifg *cfg = LOGIC_CONFIG->GetTalentConfig().GetTalentLevelCfg(SPECIAL_TALENT_ID_412, talent_level);
		if (NULL != cfg)
		{
			add_injure -= static_cast<long long>(injure * (cfg->param_a * SKILL_ATTR_PER));
		}
	}

	return add_injure;
}

const long long Talent::GetTalentAttrValue(int talent_attr_type)
{
	if (talent_attr_type <= TALENT_ATTR_TYPE_INVALID || talent_attr_type >= TALENT_ATTR_TYPE_MAX)
	{
		return 0;
	}

	long long talent_attr_value = 0;
	for (int type_index = 0; type_index < MAX_TELENT_TYPE_COUT; type_index++)
	{
		for (int talent_index = 0; talent_index < MAX_TELENT_INDEX_COUT; talent_index++)
		{
			int talent_type = type_index + 1;
			int talent_id = talent_type * 100 + talent_index;

			const TalentLevelConifg *cfg = LOGIC_CONFIG->GetTalentConfig().GetTalentLevelCfg(talent_id, m_param.talent_level_list[type_index][talent_index]);
			if (NULL == cfg) continue;

			if (cfg->effect_type != TALENT_EFFECT_TYPE_ATTR) continue;

			for (int i = 0; i < TalentLevelConifg::TALENT_MAX_ATTR_TYPE; i++)
			{
				if (cfg->talent_attr_type_list[i] == talent_attr_type)
				{
					talent_attr_value += cfg->talent_attr_value_list[i];
				}
			}
		}
	}

	return talent_attr_value;
}

long long Talent::ReCalExtraCap()
{
	long long add_cap = 0;
	for (int type_index = 0; type_index < MAX_TELENT_TYPE_COUT; type_index++)
	{
		for (int talent_index = 0; talent_index < MAX_TELENT_INDEX_COUT; talent_index++)
		{
			int talent_type = type_index + 1;
			int talent_id = talent_type * 100 + talent_index;

			const TalentLevelConifg *cfg = LOGIC_CONFIG->GetTalentConfig().GetTalentLevelCfg(talent_id, m_param.talent_level_list[type_index][talent_index]);
			if (NULL == cfg) continue;

			add_cap += cfg->add_fix_cap;
		}
	}

	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_TALENT, static_cast<int>(add_cap));

	return add_cap;
}


long long Talent::ReCalSkillCapPer()
{
	long long skill_cap_per = 0;
	for (int type_index = 0; type_index < MAX_TELENT_TYPE_COUT; type_index++)
	{
		for (int talent_index = 0; talent_index < MAX_TELENT_INDEX_COUT; talent_index++)
		{
			int talent_type = type_index + 1;
			int talent_id = talent_type * 100 + talent_index;

			const TalentLevelConifg *cfg = LOGIC_CONFIG->GetTalentConfig().GetTalentLevelCfg(talent_id, m_param.talent_level_list[type_index][talent_index]);
			if (NULL == cfg) continue;

			skill_cap_per += cfg->skill_cap_per;
		}
	}

	return skill_cap_per;
}

int Talent::GetTalentLevel(int talent_id)
{
	int type = talent_id / 100;
	int type_index = type - 1;
	int talent_index = talent_id % 100;
	
	return m_param.talent_level_list[type_index][talent_index];
}

const TalentLevelConifg* Talent::GetTalentLevelCfg(int talent_id)
{
	auto level = GetTalentLevel(talent_id);
	if (level <= 0)
	{
		return nullptr;
	}
	return LOGIC_CONFIG->GetTalentConfig().GetTalentLevelCfg(talent_id, level);
}

void Talent::OnTalentOperate(Protocol::CSRoleTelentOperate *rto)
{
	if (Protocol::ROLE_TALENT_OPERATE_TYPE_INFO == rto->opera_type)
	{
		this->SendTalentInfo();
	}
	else if (Protocol::ROLE_TALENT_OPERATE_TYPE_UPLEVEL == rto->opera_type)
	{
		this->OnTalentUpLevel(rto->param_1);
	}
	else if (Protocol::ROLE_TALENT_OPERATE_TYPE_RESET == rto->opera_type)
	{
		this->OnResetTalentPoint( 1 == rto->param_1);
	}
}

const TalentVirSkillInfo * Talent::GetTalentVirSkillInfo(int vir_skill_index)
{
	if (vir_skill_index < 0 || vir_skill_index >= TALENT_VIR_SKILL_TYPE_MAX)
	{
		return NULL;
	}

	return &m_vir_skill_info[vir_skill_index];
}

void Talent::GMAddTianFuUpLevel(int talent_id)
{
	this->OnTalentUpLevel(talent_id);
}

void Talent::OnTalentUpLevel(int talent_id)
{
	const TalentOtherCfg &other_cfg = LOGIC_CONFIG->GetTalentConfig().GetTalentOtherCfg();
	if (m_role->GetLevel() < other_cfg.open_talent_level)
	{
		m_role->NoticeNum(errornum::EN_ITEM_LEVEL_LIMIT);
		return;
	}

	int type = talent_id / 100;
	int index = talent_id % 100;

	if (type <= TALENT_TYPE_INVALID || type >= TALENT_TYPE_MAX || index < 0 || index >= MAX_TELENT_INDEX_COUT)
	{
		return;
	}

	int cur_type_index = type - 1;
	char &cur_talent_level = m_param.talent_level_list[cur_type_index][index];

	const int talent_level_max = LOGIC_CONFIG->GetTalentConfig().GetTalentLevelMax(talent_id);
	if (talent_level_max <= 0) return;

	if (cur_talent_level >= talent_level_max)
	{
		m_role->NoticeNum(errornum::EN_TALENT_IS_MAX_LEVEL);
		return;
	}

	if (m_param.talent_point <= 0)
	{
		m_role->NoticeNum(errornum::EN_TALENT_NO_TALENT_POINT);
		return;
	}

	const TalentLevelConifg *cfg = LOGIC_CONFIG->GetTalentConfig().GetTalentLevelCfg(talent_id, cur_talent_level + 1);
	if (NULL == cfg)
	{
		return;
	}

	short total_level_list[MAX_TELENT_TYPE_COUT] = { 0 }, total_talent_point = 0;
	for (int type_index = 0; type_index < MAX_TELENT_TYPE_COUT; type_index++)
	{
		for (int index = 0; index < MAX_TELENT_INDEX_COUT; index++)
		{
			total_level_list[type_index] += m_param.talent_level_list[type_index][index];
		}
		total_talent_point += total_level_list[type_index];
	}

	if (cfg->pre_talent_type > TALENT_TYPE_INVALID && cfg->pre_talent_type < TALENT_TYPE_MAX)
	{
		if (total_level_list[cfg->pre_talent_type - 1] < cfg->pre_talent_type_level)
		{
			m_role->NoticeNum(errornum::EN_TALENT_OTHER_TALENT_LEVEL_LIMIT);	
			return;
		}
	}

	if (cfg->pre_talent_id > 0)
	{
		int temp_type = cfg->pre_talent_id / 100;
		int temp_index = cfg->pre_talent_id % 100;

		if (temp_type > TALENT_TYPE_INVALID && temp_type < TALENT_TYPE_MAX && temp_index >= 0 && temp_index < MAX_TELENT_INDEX_COUT)
		{
			if (m_param.talent_level_list[temp_type - 1][temp_index] < cfg->pre_talent_level)
			{
				m_role->NoticeNum(errornum::EN_TALENT_OTHER_TALENT_LEVEL_LIMIT);
				return;
			}
		}
	}

	// 精通类天赋
	if(TALENT_TYPE_PROFICIENT == cfg->talent_type)
	{
		// 需要总投入检测
		if (total_talent_point < other_cfg.proficient_talent_limit)
		{
			m_role->NoticeNum(errornum::EN_TALENT_OTHER_TALENT_LEVEL_LIMIT);
			return;
		}

		// 需要五转之后
	//	if(m_role->GetProfLevel() < PROF_LEVEL_TYPE_FIVE_ZHUAN)
	//	{
	//		m_role->NoticeNum(errornum::EN_TALENT_OTHER_TALENT_PROFLEVEL_LIMIT);
	//		return;
	//	}
	}

	// 天赋加点时，第一次加点要扣除一定元宝(优先绑元). 重置后再次加点无需扣除
	if (cur_talent_level <= 0 && !IS_BIT_SET(m_param.talent_first_point_flag[cur_type_index], index))
	{
		if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(other_cfg.first_point_consume_gold))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		if (other_cfg.first_point_consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.first_point_consume_gold, "TALENT_FIRST_POINT_CONSUME_GOLD"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		SET_BIT(m_param.talent_first_point_flag[cur_type_index], index);	// 设置扣除元宝标记
	}

	std::set<CharIntAttrs::RECALC_REASON> attr_change_modules;
	for (auto typ : cfg->talent_attr_type_list)
	{
		auto mod_typ = GetOtherModuleReason(TALENT_ATTR_TYPE(typ));
		if (mod_typ != CharIntAttrs::RECALC_REASON_INVALID)
		{
			attr_change_modules.insert(mod_typ);
		}
	}

	m_param.talent_point--;
	cur_talent_level++;

	this->SendTalentInfo();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TALENT);

	// 其他相关系统增加对应属性值
	ReCalcOtherModule(attr_change_modules);
}

void Talent::OnResetTalentPoint(bool is_auto_buy)
{
	const TalentOtherCfg &other_cfg = LOGIC_CONFIG->GetTalentConfig().GetTalentOtherCfg();
	if(!m_role->GetKnapsack()->ConsumeItem(other_cfg.reset_consume_item, 1, "Talent::OnResetTalentPoint"))
	{
		if (!is_auto_buy)
		{
			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}
		//处理自动购买
		static ItemExtern::ItemConsumeList consume_list;
		consume_list.Reset();
		{
			int stuff_count = 0;
			ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
			short upgrade_stuff_num = m_role->GetKnapsack()->Count(other_cfg.reset_consume_item);
			if (upgrade_stuff_num < 1 && stuff_count < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM)
			{
				stuff_list[stuff_count].item_id = other_cfg.reset_consume_item;
				stuff_list[stuff_count].num = 1;
				stuff_list[stuff_count].buyable = is_auto_buy;

				++stuff_count;
			}
			if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, false))
			{
				return;
			}
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "Talent::OnResetTalentPoint"))
		{
			return;
		}
	}

	int extra_talent_point = 0;
	for (int level = other_cfg.open_talent_level + 1; level <= m_role->GetLevel(); level++)
	{
		int add_talent_point = LOGIC_CONFIG->GetTalentConfig().GetLvAddTalentPoint(level);

		extra_talent_point += add_talent_point;
	}

	for (int zhuanzhi_level = 1; zhuanzhi_level <= m_role->GetProfLevel(); zhuanzhi_level++)
	{
		int add_talent_point = LOGIC_CONFIG->GetTalentConfig().GetZhuanzhiLvAddTalentPoint(zhuanzhi_level);
		extra_talent_point += add_talent_point;
	}

//	int extra_talent_point = m_role->GetLevel() - other_cfg.open_talent_level > 0 ? m_role->GetLevel() - other_cfg.open_talent_level : 0;
	m_param.talent_point = other_cfg.base_talent_point + extra_talent_point + m_param.talent_book_add_point;

	for (int i = 0; i < MAX_TELENT_TYPE_COUT; i++)
	{
		memset(m_param.talent_level_list[i], 0, sizeof(m_param.talent_level_list[i]));
	}

	this->SendTalentInfo();

	//m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TALENT);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_INVALID);  // 重置天赋，其他所有系统也全部重算。 有部分系统依赖天赋
}

void Talent::ReCalcOtherModule(const std::set<CharIntAttrs::RECALC_REASON>& other_md)
{
	for (auto typ:other_md)
	{
		if (typ != CharIntAttrs::RECALC_REASON_INVALID)
		{
			m_role->ReCalcAttr(typ);
		}
	}
}

CharIntAttrs::RECALC_REASON Talent::GetOtherModuleReason(TALENT_ATTR_TYPE typ)
{
	switch (typ)
	{
	case TALENT_ATTR_TYPE_FABAO_GONGJI_PER:
	case TALENT_ATTR_TYPE_FABAO_FANGYU_PER:
	case TALENT_ATTR_TYPE_FABAO_UPGRADE_PER:
		return CharIntAttrs::RECALC_REASON_FABAO;

	case TALENT_ATTR_TYPE_FIGHT_MOUNT_GONGJI_PER:
	case TALENT_ATTR_TYPE_FIGHT_MOUNT_FANGYU_PER:
	case TALENT_ATTR_TYPE_FIGHT_MOUNT_UPGRADE_PER:
		return CharIntAttrs::RECALC_REASON_FIGHT_MOUNT;

	case TALENT_ATTR_TYPE_MOUNT_GONGJI_PER:
	case TALENT_ATTR_TYPE_MOUNT_FANGYU_PER:
	case TALENT_ATTR_TYPE_MOUNT_UPGRADE_PER:
		return CharIntAttrs::RECALC_REASON_MOUNT;

	case TALENT_ATTR_TYPE_SHENBING_GONGJI_PER:
	case TALENT_ATTR_TYPE_SHENBING_FANGYU_PER:
	case TALENT_ATTR_TYPE_SHENGBING_UPGRADE_PER:
		return CharIntAttrs::RECALC_REASON_SHIZHUANG;

	case TALENT_ATTR_TYPE_WING_GONGJI_PER:
	case TALENT_ATTR_TYPE_WING_FANGYU_PER:
	case TALENT_ATTR_TYPE_WING_UPGRADE_PER:
		return CharIntAttrs::RECALC_REASON_WING;

	case TALENT_ATTR_TYPE_HALO_GONGJI_PER:
	case TALENT_ATTR_TYPE_HALO_FANGYU_PER:
	case TALENT_ATTR_TYPE_HALO_UPGRADE_PER:
		return CharIntAttrs::RECALC_REASON_HALO;

	case TALENT_ATTR_TYPE_SHENYI_GONGJI_PER:
	case TALENT_ATTR_TYPE_SHENYI_FANGYU_PER:
		return CharIntAttrs::RECALC_REASON_SHENYI;

	case TALENT_ATTR_TYPE_SHENGONG_GONGJI_PER:
	case TALENT_ATTR_TYPE_SHENGONG_FANGYU_PER:
	case TALENT_ATTR_TYPE_SHENGONG_UPGRADE_PER:
		return CharIntAttrs::RECALC_REASON_SHENGONG;

	case TALENT_ATTR_TYPE_SHIZHUANG_GONGJI_PER:
	case TALENT_ATTR_TYPE_SHIZHUANG_FANGYU_PER:
	case TALENT_ATTR_TYPE_SHIZHUANG_UPGRADE_PER:
		return CharIntAttrs::RECALC_REASON_SHIZHUANG;

	case TALENT_ATTR_TYPE_FOOTPRINT_GONGJI_PER:
	case TALENT_ATTR_TYPE_FOOTPRINT_FANGYU_PER:
		return CharIntAttrs::RECALC_REASON_FOOTPRINT;

	case TALENT_ATTR_TYPE_SHENSHOU_GONGJI_PER:
	case TALENT_ATTR_TYPE_SHENSHOU_FANGYU_PER:
	case TALENT_ATTR_TYPE_SHENSHOU_ALL_PER:
		return CharIntAttrs::RECALC_REASON_SHENSHOU;

	case TALENT_ATTR_TYPE_YUSHI_GONGJI_PER:
	case TALENT_ATTR_TYPE_YUSHI_FANGYU_PER:
	case TALENT_ATTR_TYPE_YUSHI_ALL_PER:
		return CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP;

	case TALENT_ATTR_TYPE_STONE_GONGJI_PER:
	case TALENT_ATTR_TYPE_STONE_FANGYU_PER:
	case TALENT_ATTR_TYPE_STONE_ALL_PER:
		return CharIntAttrs::RECALC_REASON_STONE;

	case TALENT_ATTR_TYPE_NORMAL_EQUIP_GONGJI_PER:
	case TALENT_ATTR_TYPE_NORMAL_EQUIP_FANGYU_PER:
		return CharIntAttrs::RECALC_REASON_EQUIPMENT;

	case TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_GONGJI_PER:
	case TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_FANGYU_PER:
		return CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP;

	case TALENT_ATTR_TYPE_YONGHENG_SUIT_PER:
		return CharIntAttrs::RECALC_REASON_EQUIPMENT;

	case TALENT_ATTR_TYPE_ZHUXIAN_SUIT_PER:
	case TALENT_ATTR_TYPE_ZHUSHEN_SUIT_PER:
		return CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP;

	case TALENT_ATTR_TYPE_QILINBI_UPGRADE_PER:
		return CharIntAttrs::RECALC_REASON_QILINBI;

	case TALENT_ATTR_TYPE_TOUSHI_UPGRADE_PER:
		return CharIntAttrs::RECALC_REASON_TOUSHI;

	case TALENT_ATTR_TYPE_YAOSHI_UPGRADE_PER:
		return CharIntAttrs::RECALC_REASON_YAOSHI;

	case TALENT_ATTR_TYPE_MASK_UPGRADE_PER:
		return CharIntAttrs::RECALC_REASON_MASK;

	case TALENT_ATTR_TYPE_LINGTONG_UPGRADE_PER:
		return CharIntAttrs::RECALC_REASON_UPGRADE_SYS_2;

	case TALENT_ATTR_TYPE_LINGGONG_UPGRADE_PER:
		return CharIntAttrs::RECALC_REASON_UPGRADE_SYS_3;

	case TALENT_ATTR_TYPE_LINGQI_UPGRADE_PER:
		return CharIntAttrs::RECALC_REASON_UPGRADE_SYS_4;

	case TALENT_ATTR_TYPE_WEIYAN_UPGRADE_PER:
		return CharIntAttrs::RECALC_REASON_UPGRADE_SYS_5;

	case TALENT_ATTR_TYPE_EQUIP_BASE_PER:
		return CharIntAttrs::RECALC_REASON_EQUIPMENT;

	case TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_BASE_PER:
		return CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP;

	default:
		return CharIntAttrs::RECALC_REASON_INVALID;
	}
}

void Talent::SendTalentInfo()
{
	static Protocol::SCRoleTelentInfo rti;

	memcpy(rti.talent_level_list, m_param.talent_level_list, sizeof(rti.talent_level_list));
	rti.talent_point = m_param.talent_point;
	
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rti, sizeof(Protocol::SCRoleTelentInfo));
}

bool Talent::OnPerformSkill(Character *target, long long injure, int max_target_num)
{
	Scene *scene = m_role->GetScene();
	if (NULL == scene)
	{
		return false;
	}

	static const int RANGE = 15;
	static ObjID targer_list[32] = { INVALID_OBJ_ID };
	int count = 0;
	if (target)
	{
		count = scene->GetZoneMatrix()->GetObjByArea(target->GetPos(), RANGE, RANGE, targer_list, sizeof(targer_list) / sizeof(targer_list[0]));
	}
	else 
	{
		count = scene->GetZoneMatrix()->GetObjByArea(m_role->GetPos(), RANGE, RANGE, targer_list, sizeof(targer_list) / sizeof(targer_list[0]));
	}

	int target_num = 0;
	Protocol::SCTalentSkillTargetList lcstl;
	lcstl.deliver_obj_id = m_role->GetId();
	lcstl.injure = injure;

	long long tmp_injure = injure;
	if (max_target_num > 0)
	{
		if (target && m_role->IsEnemy(target))
		{
			EffectOnce *effect = new EffectOnce(m_role->GetId(), 0, static_cast<Attribute>(-tmp_injure), EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_TALENT_SKILL);
			target->AddEffect(effect);

			lcstl.target_objid_list[target_num] = target->GetId();

			++target_num;
		}
		for (int i = 0; i < count && target_num < max_target_num; i++)
		{
			Obj *tmp_target_obj = scene->GetObj(targer_list[i]);
			if (NULL == tmp_target_obj || !tmp_target_obj->IsCharactor(tmp_target_obj->GetObjType()) || !m_role->IsEnemy(tmp_target_obj) || tmp_target_obj == target)
			{
				continue;
			}

			Character *tmp_target = (Character*)tmp_target_obj;
			EffectOnce *effect = new EffectOnce(m_role->GetId(), 0, static_cast<Attribute>(-tmp_injure), EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_TALENT_SKILL);
			tmp_target->AddEffect(effect);

			lcstl.target_objid_list[target_num] = targer_list[i];

			++target_num;
		}
		lcstl.target_count = target_num;
	}

	m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void*)&lcstl, sizeof(lcstl));

	return lcstl.target_count > 0;
}

void Talent::OnHurtOtherTalent306()
{
	auto talent_level = GetTalentLevel(SPECIAL_TALENT_ID_306);
	if (talent_level <= 0)
	{
		return;
	}

	const TalentLevelConifg *cfg = LOGIC_CONFIG->GetTalentConfig().GetTalentLevelCfg(SPECIAL_TALENT_ID_306, talent_level);
	if (NULL == cfg)
	{
		return;
	}

	int rand_val = RandomNum(10000);
	if (rand_val >= cfg->param_a)
	{
		return;
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_second < m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_306].last_perform_timestamp + cfg->cool_down)
		return;

	m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_306].last_perform_timestamp = now_second;
	auto injure = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI);
	OnPerformSkill(nullptr, (long long)(injure * (cfg->param_c * 0.0001f)), cfg->param_b);
}

void Talent::OnRoleBeenAttackTalent206(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill)
{
	Character *attacker_char = (Character*)m_role->GetScene()->GetObj(attacker);
	if (!attacker_char)
	{
		return;
	}

	int talent_level = this->GetTalentLevel(SPECIAL_TALENT_ID_206);
	if (talent_level <= 0)
	{
		return;
	}

	const TalentLevelConifg *cfg = LOGIC_CONFIG->GetTalentConfig().GetTalentLevelCfg(SPECIAL_TALENT_ID_206, talent_level);
	if (!cfg)
	{
		return;
	}

	int rand_val = RandomNum(10000);
	if (rand_val < cfg->param_a)
	{
		SpecialLogic *logic = m_role->GetScene()->GetSpecialLogic();
		if (!logic)
		{
			return;
		}
		if (!logic->SpecialCanRebound(m_role, attacker_char))
		{
			return;
		}

		if (from_skill)
		{
			EffectOnce *dec_effect = new EffectOnce(m_role->GetId(), 0, -cfg->param_b, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_TALENT_SKILL);
			attacker_char->AddEffect(dec_effect);
		}
	}
}
void Talent::OnRoleBeenAttackTalent207(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill)
{
	Character *attacker_char = (Character*)m_role->GetScene()->GetObj(attacker);
	if (!attacker_char)
	{
		return;
	}
	const TalentLevelConifg *cfg = GetTalentLevelCfg(SPECIAL_TALENT_ID_207);
	if (!cfg)
	{
		return;
	}

	int rand_val = RandomNum(10000);
	if (rand_val < cfg->param_a)
	{
		if (Obj::OBJ_TYPE_ROLE != attacker_char->GetObjType())
		{
			return;
		}
		Role *attacker_role = (Role *)attacker_char;

		if (from_skill)
		{
			int dec_hp = (int)(attacker_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (cfg->param_b * SKILL_ATTR_PER));
			EffectOnce *dec_effect = new EffectOnce(m_role->GetId(), 0, -dec_hp, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_TALENT_SKILL);
			attacker_role->AddEffect(dec_effect);
		}
	}
}
void Talent::OnRoleBeenAttackTalent208(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill)
{
	Character *attacker_char = (Character*)m_role->GetScene()->GetObj(attacker);
	if (!attacker_char)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != attacker_char->GetObjType())
	{
		return;

	}

	const TalentLevelConifg *cfg = GetTalentLevelCfg(SPECIAL_TALENT_ID_208);
	if (!cfg)
	{
		return;
	}

	int rand_val = RandomNum(10000);
	if (rand_val < cfg->param_d)
	{
		m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_208].skill_effect_time_ms = cfg->param_c;;
		if (m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_208].skill_stack_count < cfg->param_b)
		{
			m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_208].skill_stack_count++;
		}
	}
}
void Talent::OnRoleBeenAttackTalent209(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill)
{
}

void Talent::OnRoleBeenAttackTalent210(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill)
{
	Character *attacker_char = (Character*)m_role->GetScene()->GetObj(attacker);
	if (!attacker_char)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != attacker_char->GetObjType())
	{
		return;

	}

	if (!m_role->IsAlive())
	{
		return;
	}

	const TalentLevelConifg *cfg = GetTalentLevelCfg(SPECIAL_TALENT_ID_210);
	if (!cfg)
	{
		return;
	}

	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_time >= m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_210].last_perform_timestamp + cfg->cool_down)
	{
		int rand_val = RandomNum(10000);
		if (rand_val < cfg->param_a)
		{
			Attribute cur_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			Attribute max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			if (max_hp > cur_hp)
			{
				long long inc_hp = (long long)(max_hp * (cfg->param_b * SKILL_ATTR_PER));
				inc_hp = inc_hp <= 0 ? 1 : inc_hp;

				EffectOnce *addhp_effect = new EffectOnce(INVALID_OBJ_ID, 0, static_cast<Attribute>(inc_hp), EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_TALENT_SKILL);
				m_role->AddEffect(addhp_effect);

				m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_210].last_perform_timestamp = now_time;
			}
		}
	}
}
void Talent::OnRoleBeenAttackTalent311(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill)
{
	Character *attacker_char = (Character*)m_role->GetScene()->GetObj(attacker);
	if (!attacker_char)
	{
		return;
	}

	const TalentLevelConifg *cfg = GetTalentLevelCfg(SPECIAL_TALENT_ID_311);
	if (!cfg)
	{
		return;
	}

	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_time >= m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_311].last_perform_timestamp + cfg->cool_down)
	{
		int rand_val = RandomNum(10000);
		if (rand_val < cfg->param_a)
		{
			Attribute add_fangyu = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * ( cfg->param_b * SKILL_ATTR_PER));

			EffectBuff *effect = new EffectBuff(m_role->GetId(), 1, cfg->param_c, 1, EffectBase::PRODUCT_METHOD_TALENT_SKILL);

			effect->AddFangYuEffect(add_fangyu, cfg->param_b);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
			effect->SetClientEffectType(EffectBase::ECT_SKILL_ADD_FANGYU);
			m_role->AddEffect(effect);
			m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_311].last_perform_timestamp = now_time;
		}
	}
}
void Talent::OnRoleBeenAttackTalent312(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill)
{
	if (delta_hp >= 0) return;

	if (!m_role->IsAlive())
	{
		return;
	}

	const TalentLevelConifg *cfg = GetTalentLevelCfg(SPECIAL_TALENT_ID_312);
	if (NULL == cfg)
	{
		return;
	}

	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_time < m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_312].last_perform_timestamp + cfg->cool_down)
		return;


	int rand_val = RandomNum(10000);
	if (rand_val < cfg->param_a)
	{
		Attribute add_hp = static_cast<Attribute>(-delta_hp * (cfg->param_b * SKILL_ATTR_PER));
		EffectOnce *addhp_effect = new EffectOnce(INVALID_OBJ_ID, 0, add_hp, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_TALENT_SKILL);
		m_role->AddEffect(addhp_effect);

		m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_312].last_perform_timestamp = now_time;
	}

}
void Talent::OnRoleBeenAttackTalent417(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill)
{
	const TalentLevelConifg *cfg = GetTalentLevelCfg(SPECIAL_TALENT_ID_417);
	if (NULL == cfg)
	{
		return;
	}

	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_time < m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_417].last_perform_timestamp + cfg->cool_down)
		return;


	int rand_val = RandomNum(10000);
	if (rand_val < cfg->param_a)
	{
		EffectTalentHpStore *effect = new EffectTalentHpStore(INVALID_OBJ_ID, 0, 10000, 1, EffectBase::PRODUCT_METHOD_TALENT_SKILL);
		effect->SetReplacePercent(50.0f);
		effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
		m_role->AddEffect(effect);
		m_vir_skill_info[TALENT_VIR_SKILL_INDEX_ID_417].last_perform_timestamp = now_time;
	}
}