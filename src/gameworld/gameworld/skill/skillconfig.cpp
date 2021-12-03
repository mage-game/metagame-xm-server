#include "servercommon/configcommon.h"
#include "skill/skilldef.hpp"
#include "skill/skillimpl/commonskill.hpp"
#include "skill/skillimpl/monsterskillimpl.hpp"
#include "skill/skillimpl/roleskillimpl.hpp"
#include "skill/skillimpl/xiannvskillimpl.hpp"
#include "skill/skillimpl/jinglingskillimpl.hpp"
#include "item/itempool.h"
#include "gameworld/checkresourcecenter.hpp"

#define SKILL_CONFIG_ROOT	"Skill"

template<class T>
bool GetSkillValueList(TiXmlElement *root, const char *var_name, T v[MAX_SKILL_LEVEL + 1])
{
	TiXmlElement *var = root->FirstChildElement(var_name);
	if (NULL == var) return false;

	static const char *LEVEL_LABEL[MAX_ROLE_SKILL_LEVEL + 1] = {"L0", "L1", "L2", "L3", "L4", "L5", "L6", "L7", "L8", "L9", "L10",
														   "L11", "L12", "L13", "L14", "L15", "L16", "L17", "L18", "L19", "L20", 
														   "L21"};

	for (int i = 1; i <= MAX_ROLE_SKILL_LEVEL; ++i)
	{
		if (!GetSubNodeValue(var, LEVEL_LABEL[i], v[i]))
		{
			return false;
		}
	}

	return true;
}

template<class T>
bool GetPetSkillValueList(TiXmlElement *root, const char *var_name, T v[MAX_PET_SKILL_LEVEL + 1])
{
	TiXmlElement *var = root->FirstChildElement(var_name);
	if (NULL == var) return false;

	static const char *LEVEL_LABEL[MAX_PET_SKILL_LEVEL + 1] = {"L0", "L1", "L2", "L3", "L4", "L5", "L6", "L7", "L8", "L9", 
															   "L10", "L11", "L12", "L13", "L14", "L15", "L16", "L17", "L18"};

	for (int i = 1; i <= MAX_PET_SKILL_LEVEL; ++i)
	{
		if (!GetSubNodeValue(var, LEVEL_LABEL[i], v[i]))
		{
			return false;
		}
	}

	return true;
}
// ----------------------------------------------------------------------------------------------------------------
#define INIT_ROOT() \
	TiXmlDocument doc;\
	std::string filename = std::string(dir) + GetSkillName() + ".xml";\
	if (!doc.LoadFile(filename))\
	{\
		*err=filename+" load fail.";\
		*err += doc.ErrorDesc();\
		return false;\
	}\
	TiXmlElement *root = doc.FirstChildElement(SKILL_CONFIG_ROOT);\
	if (root == 0)\
	{\
		*err=filename+" load root fail: not root.";\
		return false;\
	}

// 怪物的技能默认都是1级 所以直接读到1的位置

#define INIT_MONSTER_SKILL_COMMON_VAR()\
	if (!GetSubNodeValue(root, "SkillID", m_skill_id))\
	{\
		*err=filename+" node[SkillID] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "Distance", m_attack_dis))\
	{\
		*err=filename+" node[Distance] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "RangeType", m_skill_range_type))\
	{\
		*err=filename+" node[RangeType] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "Range", m_attack_range_list[1]))\
	{\
		*err=filename+" node[Range] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "Range2", m_attack_range2_list[1]))\
	{\
		*err=filename+" node[Range2] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "CoolDown", m_cooldown_time_list[1]))\
	{\
		*err=filename+" node[CoolDown] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "CheckShanbi", m_check_shanbi))\
	{\
		*err=filename+" node[CheckShanbi] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "HurtPercent", m_hurt_percent_list[1]))\
	{\
		*err=filename+" node[HurtPercent] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "FixHurt", m_fix_hurt_list[1]))\
	{\
		*err=filename+" node[FixHurt] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "EffectTarget", m_effect_target) || m_effect_target <= SKILL_ATTACH_EFFECT_TARGET_INVALID || m_effect_target >= SKILL_ATTACH_EFFECT_TARGET_MAX)\
	{\
		*err=filename+" node[EffectTarget] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "ReadingTime", m_reading_s) || m_reading_s < 0 || m_reading_s > 10)\
	{\
		*err=filename+" node[ReadingTime] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "ContinueTime", m_continue_perform_s) || m_continue_perform_s < 0 || m_continue_perform_s > 20)\
	{\
		*err=filename+" node[ContinueTime] error.";\
		return false;\
	}\
	

#define INIT_FAZHEN_VAR() \
	if (!GetSubNodeValue(root, "FaZhenType", m_fazhen_type) || m_fazhen_type <= SKILL_FAZHEN_EFFECT_TYPE_INVALID || m_fazhen_type >= SKILL_FAZHEN_EFFECT_TYPE_MAX)\
	{\
		*err=filename+" node[FaZhenType] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "FaZhenInterval", m_fazhen_interval) || m_fazhen_interval < 1)\
	{\
		*err=filename+" node[FaZhenInterval] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "FaZhenLifeTime", m_fazhen_lifetime) || m_fazhen_lifetime <= 0)\
	{\
		*err=filename+" node[FaZhenLifeTime] error.";\
		return false;\
	}\
	int effect_now = 0;\
	if (!GetSubNodeValue(root, "EffectNow", effect_now))\
	{\
		*err=filename+" node[EffectNow] error.";\
		return false;\
	}\
	m_effect_now = (0 != effect_now);

#define INIT_RAND_ZONE_VAR() \
	if (!GetSubNodeValue(root, "RandZoneCount", m_rand_zone_count) || m_rand_zone_count < 1)\
	{\
		*err=filename+" node[RandZoneCount] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "SubZoneRange", m_sub_zone_range) || m_sub_zone_range < 1)\
	{\
		*err=filename+" node[SubZoneRange] error.";\
		return false;\
	}

#define INIT_MUT_RECT_ZONE_VAR() \
	if (!GetSubNodeValue(root, "RectLength", m_rect_length) || m_rect_length < 1)\
	{\
		*err=filename+" node[RectLength] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "RectWidth", m_rect_width) || m_rect_width < 1)\
	{\
		*err=filename+" node[RectWidth] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "DirType", m_dir_type) || m_dir_type < DIR_TYPE_SINGLE || m_dir_type > DIR_TYPE_CROSS)\
	{\
		*err=filename+" node[DirType] error.";\
		return false;\
	}

#define INIT_EFFECT_LIST_VAR() \
	TiXmlElement *effect_listElement = root->FirstChildElement("effect_list");\
	if (!effect_listElement)\
	{\
		return true;\
	}\
	TiXmlElement *effectElement = effect_listElement->FirstChildElement("effect");\
	if (!effectElement)\
	{\
		*err =filename+" node error in [config->effect_list->effect]";\
		return false;\
	}\
	m_effect_count = 0;\
	while (effectElement)\
	{\
		if (m_effect_count >= SKILL_ATTACH_MAX_EFFECT)\
		{\
			*err =filename+" node error in [config->effect_list->effect_count too much]";\
			return false;\
		}\
		int effect_type = 0;\
		if (!GetSubNodeValue(effectElement, "effect_type", effect_type) || effect_type <= SKILL_ATTACH_EFFECT_INVALID || effect_type >= SKILL_ATTACH_EFFECT_MAX)\
		{\
			*err=filename+" node[effect_type] error.";\
			return false;\
		}\
		m_effect_list[m_effect_count].effect_type = effect_type;\
		int param0 = 0;\
		if (!GetSubNodeValue(effectElement, "param0", param0))\
		{\
			*err=filename+" node[param0] error.";\
			return false;\
		}\
		m_effect_list[m_effect_count].effect_param_list[0] = param0;\
		int param1 = 0;\
		if (!GetSubNodeValue(effectElement, "param1", param1))\
		{\
			*err=filename+" node[param1] error.";\
			return false;\
		}\
		m_effect_list[m_effect_count].effect_param_list[1] = param1;\
		int param2 = 0;\
		if (!GetSubNodeValue(effectElement, "param2", param2))\
		{\
			*err=filename+" node[param2] error.";\
			return false;\
		}\
		m_effect_list[m_effect_count].effect_param_list[2] = param2;\
		int param3 = 0;\
		if (!GetSubNodeValue(effectElement, "param3", param3))\
		{\
			*err=filename+" node[param3] error.";\
			return false;\
		}\
		m_effect_list[m_effect_count].effect_param_list[3] = param3;\
		int param4 = 0;\
		if (!GetSubNodeValue(effectElement, "param4", param4))\
		{\
		*err=filename+" node[param4] error.";\
		return false;\
		}\
		m_effect_list[m_effect_count].effect_param_list[4] = param4;\
		int param5 = 0;\
		if (!GetSubNodeValue(effectElement, "param5", param5))\
		{\
		*err=filename+" node[param5] error.";\
		return false;\
		}\
		m_effect_list[m_effect_count].effect_param_list[5] = param5;\
		\
		++ m_effect_count;\
		effectElement = effectElement->NextSiblingElement();\
	}\

UNSTD_STATIC_CHECK(SKILL_ATTACH_EFFECT_PARAM_COUNT == 6); // 上面直接用下标 改这个值 必须改上面代码和使用地方的代码

#define INIT_PET_SKILL_VAR()\
	if (!GetSubNodeValue(root, "SkillID", m_skill_id))\
	{\
		*err=filename+" node[SkillID] error.";\
		return false;\
	}\
	if (!GetSubNodeValue(root, "Distance", m_attack_dis))\
	{\
		*err=filename+" node[Distance] error.";\
		return false;\
	}\
	if (!GetPetSkillValueList(root, "prob", m_prob))\
	{\
		*err=filename+" node[prob] error.";\
		return false;\
	}\
	if (!GetPetSkillValueList(root, "AttackRangeList", m_attack_range_list))\
	{\
		*err=filename+" node[GetSkillValueList AttackRangeList] error.";\
		return false;\
	}\
	if (!GetPetSkillValueList(root, "AttackRangeEnemyNumList", m_attack_range_enemy_num_list))\
	{\
		*err=filename+" node[GetSkillValueList AttackRangeList] error.";\
		return false;\
	}\
	if (!GetPetSkillValueList(root, "CoolDownList", m_cooldown_time_list))\
	{\
		*err=filename+" node[GetSkillValueList CoolDownList] error.";\
		return false;\
	}\
	if (!GetPetSkillValueList(root, "HurtPercentList", m_hurt_percent_list))\
	{\
		*err=filename+" node[GetSkillValueList HurtPercentList] error.";\
		return false;\
	}\
	if (!GetPetSkillValueList(root, "FixHurtList", m_fix_hurt_list))\
	{\
		*err=filename+" node[GetSkillValueList FixHurtList] error.";\
		return false;\
	}\
	if (!GetPetSkillValueList(root, "param_a", m_param_a))\
	{\
		*err=filename+" node[GetSkillValueList param_a] error.";\
		return false;\
	}\
	if (!GetPetSkillValueList(root, "param_b", m_param_b))\
	{\
		*err=filename+" node[GetSkillValueList param_b] error.";\
		return false;\
	}

#define GET_VALUE(NodeName, FieldName) \
	if (!GetSubNodeValue(root, NodeName, FieldName) )\
	{\
		*err=filename+" node["NodeName"] error.";\
		return false;\
	}

#define CLEAR_ROOT()\
	root->Clear();

// ----------------------------------------------------------------------------------------------------------------

bool SkillBase::LoadFromDataNode(TiXmlElement *dataElement, std::string *err)
{
	if (NULL == dataElement)
	{
		*err = "skill base null data element";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "skill_id", m_skill_id))
	{
		*err = "skill_id error";
		return false;
	}

	short skill_level = 0;
	if (!GetSubNodeValue(dataElement, "skill_level", skill_level) || skill_level < 1 || skill_level > MAX_SKILL_LEVEL)
	{
		*err = "skill_level error";
		return false;
	}

	if (skill_level > m_max_level)
	{
		m_max_level = skill_level;
	}

	if (!GetSubNodeValue(dataElement, "distance", m_attack_dis))
	{
		*err = "distance error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "range_type", m_skill_range_type))
	{
		*err = "range_type error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "attack_range", m_attack_range_list[skill_level]))
	{
		*err = "attack_range error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "attack_range2", m_attack_range2_list[skill_level]))
	{
		*err = "attack_range2 error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "enemy_num", m_attack_range_enemy_num_list[skill_level]))
	{
		*err = "enemy_num error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "cd_s", m_cooldown_time_list[skill_level]))
	{
		*err = "cd_s error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "prof_limit", m_prof_limit))
	{
		*err = "prof_limit error";
		return false;
	}

	short is_repel = 0;
	if (!GetSubNodeValue(dataElement, "is_repel", is_repel))
	{
		*err = "is_repel error";
		return false;
	}
	m_is_repel = is_repel > 0 ? true : false; 

	if (!GetSubNodeValue(dataElement, "cost_mp", m_cost_mp))
	{
		*err = "cost_mp error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "learn_level_limit", m_learn_level_limit[skill_level]))
	{
		*err = "learn_level_limit error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "hurt_percent", m_hurt_percent_list[skill_level]))
	{
		*err = "hurt_percent error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "is_be_effect", m_is_be_effect_list[skill_level]))
	{
		*err = "is_be_effect error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "fix_hurt", m_fix_hurt_list[skill_level]))
	{
		*err = "fix_hurt error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "param_a", m_param_a[skill_level]))
	{
		*err = "param_a error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "param_b", m_param_b[skill_level]))
	{
		*err = "param_b error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "param_c", m_param_c[skill_level]))
	{
		*err = "param_c error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "param_d", m_param_d[skill_level]))
	{
		*err = "param_d error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "param_e", m_param_e[skill_level]))
	{
		*err = "param_e error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "param_f", m_param_f[skill_level]))
	{
		*err = "param_f error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "param_g", m_param_g[skill_level]))
	{
		*err = "param_g error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "mojing_cost", m_mojing_cost_list[skill_level]))
	{
		*err = "mojing_cost error";
		return false;
	}

	m_item_cost_id_list[skill_level] = 0;
	GetSubNodeValue(dataElement, "item_cost_id", m_item_cost_id_list[skill_level]);

	if (!GetSubNodeValue(dataElement, "item_cost", m_item_cost_list[skill_level]))
	{
		*err = "item_cost error";
		return false;
	}

	if (m_item_cost_id_list[skill_level] > 0)
	{
		CheckResourceCenter::Instance().GetItemValidCheck()->Add(m_skill_id, m_item_cost_id_list[skill_level], 1);

		if (m_item_cost_list[skill_level] <= 0)
		{
			*err = "item_cost <= 0 error";
			return false;
		}
	}

	if (!GetSubNodeValue(dataElement, "coin_cost", m_coin_cost_list[skill_level]))
	{
		*err = "coin_cost error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "capbility", m_capability_list[skill_level]))
	{
		*err = "capbility error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "skill_cap_per", m_skill_cap_per_list[skill_level]))
	{
		*err = "skill_cap_per error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "calc_hurt_by_hp_per", m_calc_hurt_by_hp_per[skill_level]))
	{
		*err = "calc_hurt_by_hp_per error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "calc_baodi_hurt_by_hp_per", m_calc_baodi_hurt_by_hp_per[skill_level]))
	{
		*err = "calc_baodi_hurt_by_hp_per error";
		return false;
	}

	if (!GetSubNodeValue(dataElement, "effect_to_other_target", m_effect_to_other_target[skill_level]) ||
		(0 != m_effect_to_other_target[skill_level] && 1 != m_effect_to_other_target[skill_level]))
	{
		*err = "effect_to_other_target error";
		return false;
	}

	return true;
}

// ----------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------
bool MonsterCommonSkillToEnemy::LoadConfig(const char *dir, std::string *err)
{
	INIT_ROOT();
	INIT_MONSTER_SKILL_COMMON_VAR();
	if (m_continue_perform_s > 0)
	{
		*err = filename + std::string("only fazhen skill continue perform");
		return false;
	}
	INIT_EFFECT_LIST_VAR();

	for (int i = 0; i < m_effect_count; ++ i)
	{
		std::string param_err;
		if (!m_effect_list[i].CheckParamConfig(m_skill_id, false, false, m_effect_target, param_err))
		{
			*err = filename + std::string(" effect param error: ")+ param_err;
			return false;
		}
	}

	CLEAR_ROOT();
	return true;
}

bool MonsterRangeCommonSkillToEnemyPos::LoadConfig(const char *dir, std::string *err)
{
	INIT_ROOT();
	INIT_MONSTER_SKILL_COMMON_VAR();
	if (m_continue_perform_s > 0)
	{
		*err = filename + std::string("only fazhen skill continue perform");
		return false;
	}
	INIT_EFFECT_LIST_VAR();

	for (int i = 0; i < m_effect_count; ++ i)
	{
		std::string param_err;
		if (!m_effect_list[i].CheckParamConfig(m_skill_id, false, true, m_effect_target, param_err))
		{
			*err = filename + std::string(" effect param error: ")+ param_err;
			return false;
		}
	}

	CLEAR_ROOT();
	return true;
}

bool MonsterCommonSkillToSelf::LoadConfig(const char *dir, std::string *err)
{
	INIT_ROOT();
	INIT_MONSTER_SKILL_COMMON_VAR();
	if (m_continue_perform_s > 0)
	{
		*err = filename + std::string("only fazhen skill continue perform");
		return false;
	}
	INIT_EFFECT_LIST_VAR();

	for (int i = 0; i < m_effect_count; ++ i)
	{
		std::string param_err;
		if (!m_effect_list[i].CheckParamConfig(m_skill_id, false, false, m_effect_target, param_err))
		{
			*err = filename + std::string(" effect param error: ")+ param_err;
			return false;
		}
	}

	CLEAR_ROOT();
	return true;
}

bool MonsterRangeCommonSkillToSelfPos::LoadConfig(const char *dir, std::string *err)
{
	INIT_ROOT();
	INIT_MONSTER_SKILL_COMMON_VAR();
	if (m_continue_perform_s > 0)
	{
		*err = filename + std::string("only fazhen skill continue perform");
		return false;
	}
	INIT_EFFECT_LIST_VAR();

	for (int i = 0; i < m_effect_count; ++ i)
	{
		std::string param_err;
		if (!m_effect_list[i].CheckParamConfig(m_skill_id, false, true, m_effect_target, param_err))
		{
			*err = filename + std::string(" effect param error: ")+ param_err;
			return false;
		}
	}

	CLEAR_ROOT();
	return true;
}

bool MonsterFaZhenSkillToEnemyPos::LoadConfig(const char *dir, std::string *err)
{
	INIT_ROOT();
	INIT_MONSTER_SKILL_COMMON_VAR();
	INIT_FAZHEN_VAR();
	if (m_continue_perform_s > 0 && m_continue_perform_s > m_fazhen_lifetime)
	{
		*err = filename + std::string("m_continue_perform_s > m_fazhen_lifetime");
		return false;
	}
	if (SKILL_FAZHEN_EFFECT_TYPE_HALO == m_fazhen_type)
	{
		*err = filename + std::string("guang huan only apply to self pos");
		return false;
	}
	INIT_EFFECT_LIST_VAR();

	for (int i = 0; i < m_effect_count; ++ i)
	{
		std::string param_err;
		if (!m_effect_list[i].CheckParamConfig(m_skill_id, true, false, m_effect_target, param_err))
		{
			*err = filename + std::string(" effect param error: ")+ param_err;
			return false;
		}
	}

	CLEAR_ROOT();
	return true;
}

bool MonsterFaZhenSkillToSelfPos::LoadConfig(const char *dir, std::string *err)
{
	INIT_ROOT();
	INIT_MONSTER_SKILL_COMMON_VAR();
	INIT_FAZHEN_VAR();
	if (m_continue_perform_s > 0 && m_continue_perform_s > m_fazhen_lifetime)
	{
		*err = filename + std::string("m_continue_perform_s > m_fazhen_lifetime");
		return false;
	}
	INIT_EFFECT_LIST_VAR();

	for (int i = 0; i < m_effect_count; ++ i)
	{
		std::string param_err;
		if (!m_effect_list[i].CheckParamConfig(m_skill_id, true, false, m_effect_target, param_err))
		{
			*err = filename + std::string(" effect param error: ")+ param_err;
			return false;
		}
	}

	CLEAR_ROOT();
	return true;
}

bool MonsterSkillToEnemyEffectToOther::LoadConfig(const char *dir, std::string *err)
{
	INIT_ROOT();
	INIT_MONSTER_SKILL_COMMON_VAR();
	if (m_continue_perform_s > 0)
	{
		*err = filename + std::string("only fazhen skill continue perform");
		return false;
	}
	INIT_EFFECT_LIST_VAR();

	for (int i = 0; i < m_effect_count; ++ i)
	{
		std::string param_err;
		if (!m_effect_list[i].CheckParamConfig(m_skill_id, false, false, m_effect_target, param_err))
		{
			*err = filename + std::string(" effect param error: ")+ param_err;
			return false;
		}
	}

	CLEAR_ROOT();
	return true;
}

bool MonsterRectRangeCommonSkillToEnemyPos::LoadConfig(const char *dir, std::string *err)
{
	INIT_ROOT();
	INIT_MONSTER_SKILL_COMMON_VAR();
	if (m_continue_perform_s > 0)
	{
		*err = filename + std::string("only fazhen skill continue perform");
		return false;
	}
	INIT_MUT_RECT_ZONE_VAR();
	INIT_EFFECT_LIST_VAR();

	for (int i = 0; i < m_effect_count; ++ i)
	{
		std::string param_err;
		if (!m_effect_list[i].CheckParamConfig(m_skill_id, false, false, m_effect_target, param_err))
		{
			*err = filename + std::string(" effect param error: ")+ param_err;
			return false;
		}
	}

	CLEAR_ROOT();
	return true;
}

bool MonsterMultiRangeCommonSkillToEnemyPos::LoadConfig(const char *dir, std::string *err)
{
	INIT_ROOT();
	INIT_MONSTER_SKILL_COMMON_VAR();
	if (m_continue_perform_s > 0)
	{
		*err = filename + std::string("only fazhen skill continue perform");
		return false;
	}
	INIT_RAND_ZONE_VAR();
	INIT_EFFECT_LIST_VAR();

	for (int i = 0; i < m_effect_count; ++ i)
	{
		std::string param_err;
		if (!m_effect_list[i].CheckParamConfig(m_skill_id, false, true, m_effect_target, param_err))
		{
			*err = filename + std::string(" effect param error: ")+ param_err;
			return false;
		}
	}

	CLEAR_ROOT();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 精灵技能配置
bool JingLingSkill151::LoadConfig(const char *dir, std::string *err)
{
	return true;
}

bool JingLingSkill152::LoadConfig(const char *dir, std::string *err)
{
	return true;
}

bool JingLingSkill153::LoadConfig(const char *dir, std::string *err)
{
	return true;
}

bool JingLingSkill154::LoadConfig(const char *dir, std::string *err)
{
	return true;
}

bool JingLingSkill155::LoadConfig(const char *dir, std::string *err)
{
	return true;
}

bool JingLingSkill156::LoadConfig(const char *dir, std::string *err)
{
	return true;
}

bool JingLingSkill157::LoadConfig(const char *dir, std::string *err)
{
	return true;
}

bool JingLingSkill158::LoadConfig(const char *dir, std::string *err)
{
	return true;
}

bool JingLingSkill159::LoadConfig(const char *dir, std::string *err)
{
	return true;
}

bool JingLingSkill160::LoadConfig(const char *dir, std::string *err)
{
	return true;
}

bool JingLingSkill161::LoadConfig(const char *dir, std::string *err)
{
	return true;
}

bool JingLingSkill162::LoadConfig(const char *dir, std::string *err)
{
	return true;
}

bool JingLingSkill163::LoadConfig(const char *dir, std::string *err)
{
	return true;
}

bool JingLingSkill164::LoadConfig(const char *dir, std::string *err)
{
	return true;
}

bool JingLingSkill165::LoadConfig(const char *dir, std::string *err)
{
	return true;
}

bool JingLingSkill166::LoadConfig(const char *dir, std::string *err)
{
	return true;
}
