#include <cmath>

#include "skillbase.hpp"
#include "skillpool.hpp"

#include "servercommon/errornum.h"
#include "obj/character/character.h"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "scene/scene.h"
#include "scene/map.h"
#include "protocal/msgfight.h"

#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectdoublehurt.hpp"

#include "globalconfig/globalconfig.h"

#include "skill/skillimpl/roleskillimpl.hpp"
#include "scene/speciallogic/speciallogic.hpp"

#include "engineadapter.h"
#include "gamecommon.h"
#include "servercommon/struct/xunzhangparam.hpp"
#include "config/logicconfigmanager.hpp"
#include "global/bossfamily/bossfamilyconfig.hpp"
#include "global/rank/topranker.hpp"
#include "equipment/equipmentmanager.h"
#include "other/jingling/jinglingmanager.h"
#include "other/capability/capability.hpp"
#include "other/rolegoal/rolegoal.hpp"
#include "other/xunzhang/xunzhangmanager.hpp"
#include "other/shenbing/shenbingmanager.hpp"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include "other/footprint/footprint.hpp"
#include "other/cloak/cloakmanager.hpp"
#include "other/shenshou/shenshou.hpp"
#include "other/fabao/fabaomanager.hpp"
#include "other/talent/talent.hpp"
#include "other/rolezhuanzhi/rolezhuanzhi.hpp"
#include "other/zhuanzhiequip/zhuanzhiequip.h"
#include "other/toushi/toushimanager.hpp"
#include "other/shizhuang/shizhuang.h"
#include "other/shenzhouweapon/shenzhouweapon.hpp"
#include "other/impguard/impguard.hpp"
#include "other/greatesoldier/greatesoldier.hpp"
#include "other/mojie/mojieskill.hpp"
#include "config/monster_hurt_hp_config.h"
#include "other/tianshuxunzhu/tianshuxunzhu.hpp"
#include "other/tianshenhuti/tianshenhuti.hpp"


SkillBase::SkillBase(SkillType type, bool is_attack_character, bool is_target_enemy)
	: m_skill_type(type), m_skill_id(0), m_max_level(0), m_attack_dis(0), m_skill_range_type(0), m_is_attack_character(is_attack_character), m_attack_enemy(is_target_enemy),
	m_cost_mp(0), m_prof_limit(0), m_is_repel(false), m_reading_s(0), m_continue_perform_s(0), m_trigger_qte(0), m_valid_in_save_area(0), m_fight_status_policy(0),
	m_tips_way(TIPS_WAY_NONE), m_check_shanbi(1)
{
	memset(m_attack_range_list, 0, sizeof(m_attack_range_list));
	memset(m_attack_range2_list, 0, sizeof(m_attack_range2_list));
	memset(m_attack_range_enemy_num_list, 0, sizeof(m_attack_range_enemy_num_list));

	memset(m_cooldown_time_list, 0, sizeof(m_cooldown_time_list));

	memset(m_is_be_effect_list, 0, sizeof(m_is_be_effect_list));
	memset(m_hurt_percent_list, 0, sizeof(m_hurt_percent_list));
	memset(m_fix_hurt_list, 0, sizeof(m_fix_hurt_list));

	memset(m_param_a, 0, sizeof(m_param_a));
	memset(m_param_b, 0, sizeof(m_param_b));
	memset(m_param_c, 0, sizeof(m_param_c));
	memset(m_param_d, 0, sizeof(m_param_d));
	memset(m_param_e, 0, sizeof(m_param_e));
	memset(m_param_f, 0, sizeof(m_param_f));
	memset(m_param_g, 0, sizeof(m_param_g));

	memset(m_mojing_cost_list, 0, sizeof(m_mojing_cost_list));
	memset(m_item_cost_id_list, 0, sizeof(m_item_cost_id_list));
	memset(m_item_cost_list, 0, sizeof(m_item_cost_list));
	memset(m_coin_cost_list, 0, sizeof(m_coin_cost_list));
	memset(m_capability_list, 0, sizeof(m_capability_list));
	memset(m_skill_cap_per_list, 0, sizeof(m_skill_cap_per_list));

	memset(m_calc_hurt_by_hp_per, 0, sizeof(m_calc_hurt_by_hp_per));
	memset(m_calc_baodi_hurt_by_hp_per, 0, sizeof(m_calc_baodi_hurt_by_hp_per));
	memset(m_effect_to_other_target, 0, sizeof(m_effect_to_other_target));

	memset(m_prob, 0, sizeof(m_prob));

	memset(m_learn_level_limit, 0, sizeof(m_learn_level_limit));
}

SkillBase::~SkillBase()
{

}


bool SkillBase::CheckTarget(Character *chara, Character *target) const
{
	if (!m_is_attack_character) return true;

	if (target->IsAlive())
	{
		return m_attack_enemy ^ (!chara->IsEnemy(target));
	}

	return false;
}

// 通知客户端发生普通攻击
void SkillBase::SendAttackMsg(Character *chara, Character *target, bool is_force_broadcast) const
{
	if (NULL == chara || NULL == target)
	{
		return;
	}

	if (is_force_broadcast || Obj::OBJ_TYPE_ROLE == chara->GetObjType())
	{
		Protocol::SCPerformSkill ps;
		ps.character = chara->GetId();
		ps.skill = m_skill_id;
		ps.target = target->GetId();
		ps.skill_data = chara->GetSkillManager()->GetNextSkillData();
		
		if (is_force_broadcast)
		{
			if (NULL != chara->GetScene()) chara->GetScene()->GetZoneMatrix()->NotifyAreaMsg(chara->GetObserHandle(), &ps, sizeof(Protocol::SCPerformSkill));
		}
		else if (Obj::OBJ_TYPE_ROLE == chara->GetObjType())
		{
			Role *role = (Role *)chara;
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&ps, sizeof(Protocol::SCPerformSkill));
		}
	}
}

void SkillBase::SendAOEAttackMsgHelper(Character *chara, ObjID target_id, UInt16 skill_id, const Posi &pos, short aoe_reason)
{
	if (NULL == chara) return;

	Protocol::SCPerformAOESkill psaoe;
	psaoe.character = chara->GetId();
	psaoe.skill = skill_id;
	psaoe.pos_x = pos.x;
	psaoe.pos_y = pos.y;
	psaoe.aoe_reason = aoe_reason;
	psaoe.target = target_id;
	psaoe.skill_data = chara->GetSkillManager()->GetNextSkillData();
	if (NULL != chara->GetScene()) chara->GetScene()->GetZoneMatrix()->NotifyAreaMsg(chara->GetObserHandle(), &psaoe, sizeof(Protocol::SCPerformAOESkill));
}

void SkillBase::SendAOEAttackMsgHelper(Scene *scene, ObjID deliver_id, ObjID target_id, UInt32 obser_handle, UInt16 skill_id, const Posi &pos, short aoe_reason)
{
	if (NULL == scene) return;

	Protocol::SCPerformAOESkill psaoe;
	psaoe.character = deliver_id;
	psaoe.skill = skill_id;
	psaoe.pos_x = pos.x;
	psaoe.pos_y = pos.y;
	psaoe.aoe_reason = aoe_reason;
	psaoe.target = target_id;
	psaoe.skill_data = 0;

	scene->GetZoneMatrix()->NotifyAreaMsg(obser_handle, &psaoe, sizeof(Protocol::SCPerformAOESkill));
}

// 发送AOE群攻消息
void SkillBase::SendAOEAttackMsg(Character *chara, Character *target, const Posi &pos, short aoe_reason) const
{
	if (NULL == chara) return;

	Protocol::SCPerformAOESkill psaoe;
	psaoe.character = chara->GetId();
	psaoe.skill = m_skill_id;
	psaoe.pos_x = pos.x;
	psaoe.pos_y = pos.y;
	psaoe.aoe_reason = aoe_reason;
	psaoe.target = INVALID_OBJ_ID;
	if (NULL != target)
	{
		psaoe.target = target->GetId();
	}
	psaoe.skill_data = chara->GetSkillManager()->GetNextSkillData();
	if (NULL != chara->GetScene()) chara->GetScene()->GetZoneMatrix()->NotifyAreaMsg(chara->GetObserHandle(), &psaoe, sizeof(Protocol::SCPerformAOESkill));
}

// 发送技能段（怪物技能使用）
void SkillBase::SendSkillPhase(Character *cha, UInt16 skill_id, int skill_phase) const
{
	Protocol::SCSkillPhase sp;
	sp.character = cha->GetId();
	sp.skill = skill_id;
	sp.phase = skill_phase;
	
	if (NULL != cha->GetScene()) cha->GetScene()->GetZoneMatrix()->NotifyAreaMsg(cha->GetObserHandle(), &sp, sizeof(Protocol::SCSkillPhase));
}

// 发送技能提示
void SkillBase::SendSkillTip(Character *cha) const
{
	if (NULL != cha)
	{
		Protocol::SCSkillTip sp;
		sp.character = cha->GetId();
		sp.skill = m_skill_id;

		if (NULL != cha->GetScene()) cha->GetScene()->GetZoneMatrix()->NotifyAreaMsg(cha->GetObserHandle(), &sp, sizeof(Protocol::SCSkillTip));
	}
}

// 根据范围获取场景所有目标
int SkillBase::GetSceneObjByArea(Scene *scene, const Posi &point, Axis range, ObjID *target_list, int max_num)
{
	static ObjID obj_list[256] = {0};

	int count = scene->GetZoneMatrix()->GetObjByArea(point, range, range, obj_list, 256);

	int num = 0;
	for (int i = 0; num < max_num && i < count; ++i)
	{
		Obj *obj = scene->GetObj(obj_list[i]);
		if (NULL != obj && Obj::IsCharactor(obj->GetObjType()))
		{
			target_list[num++] = obj_list[i];
		}
	}

	return num;
}

int SkillBase::GetTargetList(Scene *scene, Character *chara, const Posi &point, Axis range, Axis range2, ObjID *target_list, int max_num, int range_type, int effect_target_type)
{
	if (NULL == scene) return 0;

	int count = 0;

	switch (range_type)
	{
	case AOE_RANGE_TYPE_SELF_CENTERED_QUADRATE:
		if (NULL != chara)
		{
			count = SkillBase::GetTargetByQuadrate(scene, chara->GetPos(), range, target_list, max_num);
		}
		break;

	case AOE_RANGE_TYPE_TARGET_CENTERED_QUADRATE:

		count = SkillBase::GetTargetByQuadrate(scene, point, range, target_list, max_num);
		break;

	case AOE_RANGE_TYPE_SELF_CENTERED_CIRCLE:
		if (NULL != chara)
		{
			count = SkillBase::GetTargetByCircle(scene, chara->GetPos(), range, target_list, max_num);
		}
		break;

	case AOE_RANGE_TYPE_TARGET_CENTERED_CIRCLE:
		count = SkillBase::GetTargetByCircle(scene, point, range, target_list, max_num);
		break;

	case AOE_RANGE_TYPE_SELF_BEGINNING_RECT:
		if (NULL != chara)
		{
			count = SkillBase::GetTargetByRect(scene, chara->GetPos(), point, range, range2, target_list, max_num);
		}
		break;

	case AOE_RANGE_TYPE_SELF_BEGINNING_SECTOR:
		if (NULL != chara)
		{
			count = SkillBase::GetTargetBySector(scene, chara->GetPos(), point, range, range2, target_list, max_num);
		}
		break;

	default:
		count = SkillBase::GetTargetByQuadrate(scene, point, range, target_list, max_num);
		break;
	}

	for (int i = count - 1; i >= 0 && i < max_num && count > 0; i--)
	{
		Obj *obj = scene->GetObj(target_list[i]);
		if (NULL == obj)
		{
			target_list[i] = target_list[--count];
			continue;
		}

		if (SKILL_ATTACH_EFFECT_TARGET_BOSS == effect_target_type)
		{
			if (Obj::OBJ_TYPE_MONSTER != obj->GetObjType() || !((Monster *)obj)->IsBossMonster())
			{
				target_list[i] = target_list[--count];
			}
		}
		else if (SKILL_ATTACH_EFFECT_TARGET_MONSTER == effect_target_type)
		{
			if (Obj::OBJ_TYPE_MONSTER != obj->GetObjType() || ((Monster *)obj)->IsBossMonster())
			{
				target_list[i] = target_list[--count];
			}
		}
		else if (SKILL_ATTACH_EFFECT_TARGET_ALL_MONSTER == effect_target_type)
		{
			if (Obj::OBJ_TYPE_MONSTER != obj->GetObjType())
			{
				target_list[i] = target_list[--count];
			}
		}
		else if (SKILL_ATTACH_EFFECT_TARGET_ROLE_PET == effect_target_type)
		{
			if (Obj::OBJ_TYPE_ROLE != obj->GetObjType())
			{
				target_list[i] = target_list[--count];
			}
		}
		else if (SKILL_ATTACH_EFFECT_TARGET_ALL_CHARACTER == effect_target_type)
		{
			if (!Obj::IsFightCharactor(obj->GetObjType()))
			{
				target_list[i] = target_list[--count];
			}
		}
		else if (SKILL_ATTACH_EFFECT_TARGET_ENEMY == effect_target_type)
		{
			if (NULL == chara || !chara->IsEnemy(obj))
			{
				target_list[i] = target_list[--count];
			}
		}
	}

	return count;
}

// 根据范围获取目标
int SkillBase::GetTargetByArea(Character *chara, Character *target, Axis range, ObjID *target_list, int max_num, bool check_enemy)
{
	return GetTargetByArea(chara, target->GetPos(), range, target_list, max_num, check_enemy);
}

// 根据范围获取目标
int SkillBase::GetTargetByArea(Character *chara, const Posi &point, Axis range, ObjID *target_list, int max_num, bool check_enemy)
{
	if (NULL == chara) return 0;

	Scene *scene = chara->GetScene();
	int count = GetTargetByQuadrate(scene, point, range, target_list, max_num);

	if (check_enemy)
	{
		for (int i = count - 1; i >= 0 && i < max_num && count > 0; i--)
		{
			Obj *obj = scene->GetObj(target_list[i]);
			if (NULL == obj || !chara->IsEnemy(obj))
			{
				target_list[i] = target_list[--count];
			}
		}
	}

	return count;
}

// 根据范围获取友方目标
int SkillBase::GetFriendByArea(Character *chara, const Posi &point, Axis range, ObjID *friend_list, int max_num, bool check_alive)
{
	static ObjID obj_list[256] = {0};
	Scene *scene = chara->GetScene();
	int count = scene->GetZoneMatrix()->GetObjByArea(point, range, range, obj_list, 256);

	int num = 0;
	for (int i = 0; num < max_num && i < count; ++i)
	{
		Obj *obj = scene->GetObj(obj_list[i]);
		if (NULL != obj && Obj::IsCharactor(obj->GetObjType()) && chara->IsFriend(obj, check_alive))
		{
			Character *target_cha = (Character *)obj;
			if (target_cha->GetEffectList()->IsInvisible()) continue;

			friend_list[num++] = obj_list[i];
		}
	}

	return num;
}

struct ObjDis
{
	ObjDis() { obj_id = INVALID_OBJ_ID; dis = 0; }

	ObjID obj_id;
	short dis;
};

int SortFunc(const void *arg1, const void *arg2)
{
	ObjDis *item_1 = (ObjDis*)arg1;
	ObjDis *item_2 = (ObjDis*)arg2;

	return item_1->dis - item_2->dis;
}

// 是否暴击
bool SkillBase::IsBaoji(Character *chara, Character *target, int &fight_type) const
{
	if (NULL == chara || NULL == target)
	{
		return false;
	}

	bool is_baoji = false;

	//基础暴击率 = 0.4 * 自身暴击/ (自身暴击 + 目标抗暴)
	Attribute baoji = chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI);
	double baoji_per = chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI) * ROLE_ATTR_PER;
	Attribute jianren = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_JIANREN);
	double kangbao_per = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_KANGBAO) * ROLE_ATTR_PER;
	kangbao_per += this->GetAddKangbaoPer(target);

	double base_baoji_rate = 0;
	if (0 != baoji + jianren)
	{
		base_baoji_rate = (double)(0.4 * baoji / (baoji + jianren));
	}

	//实际暴击率 = 基础暴击率 + 自身增加暴击率 - 目标减少暴击率
	double real_baoji_rate = base_baoji_rate + baoji_per - kangbao_per;
	
	if (RandomNum(100) <= (int)(real_baoji_rate * 100))
	{
		is_baoji = true;
	}

	else if (Obj::OBJ_TYPE_ROLE == chara->GetObjType())
	{
		// 计算其他系统是否产生暴击
		Role *role = (Role *)chara;
		is_baoji = role->GetJingLingMgr()->IsBaoJi();
		is_baoji = is_baoji || role->GetGreateSoldier()->IsGreateSoldierBaoji();
	}

	if (is_baoji)
	{
		fight_type = EffectBase::FIGHT_TYPE_BAOJI;

		if (Obj::OBJ_TYPE_ROLE == target->GetObjType())
		{
			Role *target_role = (Role *)target;
			target_role->GetShizhuang()->CheckShizhuangSkill();
		}
	}

	return is_baoji;
}

// 是否闪避
bool SkillBase::IsShanbi(Character *chara, Character *target, int target_add_shanbi_percent ) const
{
	if (NULL == chara || NULL == target || SKILL_TYPE_NUQI == this->GetSkillType())
	{
		return false;
	}

	//基础闪避率 = 0.3 * 目标闪避 / (攻击者命中 + 目标闪避)
	Attribute self_mingzhong = chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG);
	double self_mingzhong_per = chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG_PER) * ROLE_ATTR_PER;
//	double self_shanbi_per = chara->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI_PER) * ROLE_ATTR_PER;

	Attribute target_shanbi = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI);
	double target_shanbi_per = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI_PER) * ROLE_ATTR_PER;
//	double target_mingzhong_per = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG_PER) * ROLE_ATTR_PER;

	double base_shanbi_rate = 0;
	if (0 != self_mingzhong + target_shanbi)
	{
		base_shanbi_rate = (0.3 * target_shanbi / (self_mingzhong + target_shanbi));
	}

	//目标实际闪避率 = 基础闪避率 + 目标闪避率 - 攻击者命中率
	double real_shanbi_rate = base_shanbi_rate + target_shanbi_per - self_mingzhong_per;

	if (RandomNum(100) >= (int)(real_shanbi_rate * 100))
	{
		return false;
	}

	EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, 0, EffectBase::FIGHT_TYPE_SHANBI);
	target->AddEffect(effect);

	if (Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		Role *target_role = (Role *)target;

		// 披风技能
		target_role->GetCloakManager()->CalcShanbiExtraEffect();

		// 头饰技能
		target_role->GetTouShiManager()->CalcShanbiExtraEffect();
	}


	return true;
}

int SkillBase::GetTotalHurtPercent(Character *cha, int level) const
{
	int hurt_per = GetHurtPercent(level);

	if (cha != NULL && Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	{
		Role *role = (Role*)cha;

		if (m_skill_id == 5 || 
			m_skill_id == 111 || 
			m_skill_id == 121 || 
			m_skill_id == 131 || 
			m_skill_id == 141 || 
			m_skill_id == 211 || 
			m_skill_id == 221 || 
			m_skill_id == 231 || 
			m_skill_id == 241 || 
			m_skill_id == 311 || 
			m_skill_id == 321 || 
			m_skill_id == 331 || 
			m_skill_id == 341)
		{
			// 星座秘籍加成
			hurt_per += role->GetChineseZodiac()->GetSkillXishuUp();
		}

		// 神兽加成
		if (m_skill_id == 121 || m_skill_id == 221 || m_skill_id == 321)
		{
			hurt_per += role->GetShenshou()->GetSkill10HurtPercent();
		}

		if (m_skill_id == 131 || m_skill_id == 231 || m_skill_id == 331)
		{
			hurt_per += role->GetShenshou()->GetSkill12HurtPercent();
		}

	}

	return hurt_per;
}

long long SkillBase::CalcInjureAndType(Character *cha, Character *target, int level, int &fight_type, bool is_be_effect_skill_per, double add_hurt_percent) const
{
	if (NULL == cha || NULL == target)
	{
		return 0;
	}

	// 固定伤害
	if (target->IsFixHurtOnAttack()) return target->GetFixHurtOnAttack();
	
	// 获取人物的面板属性
	double gongji = this->GetGongji(cha, target);
	double skill_per = (double)(this->GetTotalHurtPercent(cha, level) * SKILL_ATTR_PER);
	double target_fangyu = (double)target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU);
//	double baoji_per = (double)(cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI) * ROLE_ATTR_PER);
	double pofang_per = (double)(cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_POFANG) * ROLE_ATTR_PER);
	double mianshang_per = (double)(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG) * ROLE_ATTR_PER);
	double huixinyiji_hurt_per = (double)(cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER) * ROLE_ATTR_PER);
	double skill_zengshang_per = (double)(cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER) * ROLE_ATTR_PER);
	double skill_jianshang_per = (double)(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER) * ROLE_ATTR_PER);
	double baoji_hurt_per = (double)(cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI_HURT) * ROLE_ATTR_PER);
	double kangbao_hurt_per = (double)(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_KANGBAO_HURT) * ROLE_ATTR_PER);
	double gedang_jianshang_per = (double)(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GEDANG_JIANSHANG_PER) * ROLE_ATTR_PER);

	// 不受技伤影响，则为0
	if (!is_be_effect_skill_per)
	{
		skill_zengshang_per = 0;
		skill_jianshang_per = 0;
	}

	//伤害0-------------------------------------------------------------------------------------------------------------------------
	long long injure_0 = 0;

	// 是否破甲一击
	bool is_pojia = this->GetIsPojia(cha);

	// 伤害0 = 是否破甲一击 ? 自身攻击 : max(自身攻击 - 目标防御, 0.1 * 自身攻击);
	injure_0 = static_cast<long long>(is_pojia ? gongji : (std::max)((gongji - target_fangyu), 0.1 * gongji));

	// 基础伤害值，用于系统的加成计算
	long long base_injure = injure_0;

	// 女神攻击
	injure_0 += static_cast<long long>(cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI) - target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_DIKANG_SHANGHAI));

	// 伤害计算加入玩家其他系统的加成
	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	{
		injure_0 += this->GetOtherSysInjure(cha, target, base_injure);
	}

	// 伤害加入固定增伤和固定免伤
	injure_0 += cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG);
	injure_0 -= target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_CONSTANT_MIANSHANG);

	//伤害1-------------------------------------------------------------------------------------------------------------------------
	long long injure_1 = 0;

	double add_hurt_per = 1.0;
	double dec_hurt_per = 1.0;
	// 伤害加成，攻击者的技能buff带来的加成
	add_hurt_per *= this->GetHurtIncPercent(cha, target, add_hurt_percent, pofang_per);

	// 伤害加成，计算加入攻击者自身系统的加成
	add_hurt_per *= this->GetObjTypeAddHurtPer(cha, target);

	// 伤害减免，攻击者的技能buff带来的减免
	dec_hurt_per *= this->GetHurtDecPercent(cha, target, mianshang_per);

	// 伤害减免，计算加入攻击者自身系统的减免
	dec_hurt_per *= this->GetObjTypeDecHurtPer(cha, target);

	// 总技能加成 = 技能加成 + 自己技能增伤 - 目标技能减伤
	double total_skill_per = skill_per + skill_zengshang_per - skill_jianshang_per;

	// 伤害1 = （总技能加成 * 伤害0 * 自身伤害加成 * 目标伤害减免）
	injure_1 = static_cast<long long>(total_skill_per * injure_0 * add_hurt_per * dec_hurt_per);

	//伤害2-------------------------------------------------------------------------------------------------------------------------
	long long injure_2 = 0;
	// 是否祝福一击
	bool is_zhufuyiji = this->GetIsZhufuyiji(cha);

	// 伤害2 = 是否祝福一击 ? 1.2 * 伤害1 : (random(0, 0.4) + 0.8) * 伤害1;
	injure_2 = static_cast<long long>(is_zhufuyiji ? 1.2 * injure_1 : (RandomNum(400) * 0.001 + 0.800) * injure_1);

	//伤害3-------------------------------------------------------------------------------------------------------------------------
	long long injure_3 = 0;

	// 是否暴击
	bool is_baoji = this->IsBaoji(cha, target, fight_type);
	if (!is_baoji)
	{
		// 伤害3 = 伤害2
		injure_3 = injure_2;
	}
	else
	{
		// 是否会心一击
		bool is_huixinyiji = this->GetIsHuixinyiji(cha, is_baoji, fight_type);
		if (is_huixinyiji)
		{
			// 伤害3 = 伤害2 * (2 + 会心一击伤害率)
			huixinyiji_hurt_per += this->GetAddHuixinyijiHurtPer(cha);
			injure_3 = static_cast<long long>(injure_2 * (2 + huixinyiji_hurt_per) );
		}
		else
		{
			// 伤害3 = 伤害2 * （1.5 + 自身暴击伤害率 - 目标暴击抵抗伤害率）
			baoji_hurt_per += this->GetAddBaojiHurtPer(cha);
			kangbao_hurt_per += this->GetAddKangbaoHurtPer(target);
			injure_3 = static_cast<long long>(injure_2 * (1.5 + baoji_hurt_per - kangbao_hurt_per));
		}

		// 暴击额外伤害(暴击、会心一击都会触发)
		long long baoji_extra_hurt = this->GetBaojiExtraHurt(cha, target);
		injure_3 += baoji_extra_hurt;
	}

	//伤害4-------------------------------------------------------------------------------------------------------------------------
	long long injure_4 = 0;

	// 是否格挡
	bool is_gedang = this->GetIsGedang(cha, target, fight_type);

	// 伤害4 = 是否格挡 ? 伤害3 * (1 - 目标格挡减伤率) : 伤害3;
	injure_4 = static_cast<long long>( is_gedang ? injure_3 * (1 - gedang_jianshang_per) : injure_3 );

	//总伤害-------------------------------------------------------------------------------------------------------------------------
	long long total_injure = injure_4;

	// 伤害过低，进行伤害修正
	this->CalcFixInjure(cha, target, total_injure);

	// 致命一击：伤害=被攻击者的固定百分比血量
	this->CalcFixHuretInjure(cha, target, total_injure);

	return total_injure;
}

// 闪避
bool SkillBase::CheckShanbi() const
{
	return 1 == m_check_shanbi;
}

int SkillBase::HurtTrigger(Character *cha, Character *target, long long &injure, int fight_type, int target_index) const
{
	if (NULL == cha || NULL == target)
	{
		return 0;
	}

	return cha->OnHurtOther(target, m_skill_id, injure, fight_type, target_index);
}

// 计算AOE的攻击目标
int SkillBase::GetAOESkillTarget(Character *chara, int level, Character *target, const Posi &pos, ObjID *target_list, int max_num) const
{
	if (nullptr == chara)
	{
		return 0;
	}
	const int enemy_num = GetAttackRangeEnemyNum(level);	// 受击最大数量
	int add_num = 0;

	// 转职增加目标人数
	if (Obj::OBJ_TYPE_ROLE == chara->GetObjType())
	{
		Role *role = (Role*)chara;

		if (SkillManager::IsNormalAttackSkill(m_skill_id))
		{
			add_num = role->GetRoleZhuanZhi()->GetRoleZhuanZhiSkill().GetAddRangeEnemyNum();
		}
	}

	max_num = (std::min)(enemy_num + add_num, max_num);

	const int range = GetAttackRange(level);				// 攻击距离
	const int range2 = GetAttackRange2(level);				// 矩形高度、扇形角度

	int count = SkillBase::GetTargetList(chara->GetScene(), chara, pos, range, range2, target_list, max_num, m_skill_range_type, SKILL_ATTACH_EFFECT_TARGET_ENEMY);
	if (NULL != target && chara->IsEnemy(target))
	{
        if (0 == count)
        {
            target_list[0] = target->GetId();
            count = 1;
            return count;
        }
		// 第1个一定是主目标
		for (int i = 0; i < count; ++i)
		{
			if (target->GetId() == target_list[i] || i == count - 1)
			{
				target_list[i] = target_list[0];
				target_list[0] = target->GetId();
				break;
			}
		}

	}

	return count;
}

long long SkillBase::PerformSpecial(Character *chara, int level, Character *target, bool is_main_target,int &fight_type, double add_hurt_percent) const
{
	return 0;
}

// 技能单体攻击
void SkillBase::PerformSingleSkill(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara) return;

	SendAttackMsg(chara, target);

	if (NULL == target) return;

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;

	if (IsShanbi(chara, target)) return;

	long long injure = PerformSpecial(chara, level, target, true, fight_type, 0.0);
	if (0 == injure)
	{
		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);

		injure = CalcInjureAndType(chara, target, level, fight_type, is_be_effect_skill_per);
	}

	// 目标伤害
	int passive_flag = this->HurtTrigger(chara, target, injure, fight_type, 0);
	EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
	effect->SetPassiveFlag(passive_flag);
	effect->SetMainTarget(true);
	target->AddEffect(effect);

	if (m_is_repel && Obj::OBJ_TYPE_ROLE != target->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW != target->GetObjType())
	{
		Posi deltaP = chara->GetPos() - target->GetPos();
		Scalar distance = (Scalar)GetParamG(level);
		Dir dir = gamecommon::GetMoveDir(deltaP, NULL);
		if (dir != Dir_S)
		{
			Posi diff_pos((Axis)(distance * cos(dir)), (Axis)(distance * sin(dir)));
			Posi dest_pos = target->GetPos() + diff_pos;
			target->SkillResetPos(dest_pos, Character::SKILL_RESET_POS_TYPE_JITUI, m_skill_id, true, chara->GetId());
		}
	}
}

// 技能群体攻击
void SkillBase::PerformAOESkill(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == chara) return;

	SendAOEAttackMsg(chara, target, pos);

	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
	double add_hurt_percent = 0.0f;
	//int add_hurt_static = 0;

	static ObjID target_list[32] = {INVALID_OBJ_ID};
	const int count = GetAOESkillTarget(chara, level, target, pos, target_list, sizeof(target_list) / sizeof(target_list[0]));
	Scene *scene = chara->GetScene();

	// 容错
	if ((chara == target) && count > 0)
	{
		Character *target_cha = (Character*)scene->GetObj(target_list[0]);
		if (NULL != target_cha)
		{
			target = target_cha;
		}
	}

	for (int i = 0; i < count; ++i)
	{
		Character *target_cha = (Character*)scene->GetObj(target_list[i]);
		if (NULL == target_cha) continue;

		// 闪避计算
		if (IsShanbi(chara, target_cha)) continue;

		if (target_cha == target)
		{
			add_hurt_percent = 0.0f;
		}
		else if (Obj::OBJ_TYPE_ROLE == target_cha->GetObjType())
		{
			add_hurt_percent = GetParamE(level) * ROLE_ATTR_PER - 1.0;
		}
		else
		{
			add_hurt_percent = GetParamF(level) * ROLE_ATTR_PER - 1.0;
		}

		// 周末装备套装加成伤害,暂时屏蔽
		//if (Obj::OBJ_TYPE_ROLE == chara->GetObjType() && SkillManager::IsRoleSkill(this->GetSkillId()))
		//{
		//	Role *role = static_cast<Role *>(chara);
		//	int skill_index = (this->GetSkillId() / 10) % 10;  // 第几个技能
		//	role->GetTianshenhuti()->GetTaozhuangInjureAdd(skill_index, &add_hurt_percent, &add_hurt_static);
		//}

		// 计算伤害
		long long injure = PerformSpecial(chara, level, target_cha, target_cha == target, fight_type, add_hurt_percent);
		if (0 == injure)
		{
			// 是否受技伤影响
			bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);

			injure = CalcInjureAndType(chara, target_cha, level, fight_type, is_be_effect_skill_per, add_hurt_percent);
		}

		//injure += add_hurt_static; // 周末装备套装固定加成伤害,暂时屏蔽

		int passive_flag = this->HurtTrigger(chara, target_cha, injure, fight_type, i);
		EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
		effect->SetMainTarget(target_cha == target);
		effect->SetPassiveFlag(passive_flag);
		target_cha->AddEffect(effect);

		if (m_is_repel && Obj::OBJ_TYPE_ROLE != target_cha->GetObjType() && Obj::OBJ_TYPE_ROLE_SHADOW != target_cha->GetObjType())
		{
			Posi deltaP = chara->GetPos() - target_cha->GetPos();
			Scalar distance = (Scalar)GetParamG(level);
			Dir dir = gamecommon::GetMoveDir(deltaP, NULL);
			if (dir != Dir_S)
			{
				Posi diff_pos((Axis)(distance * cos(dir)), (Axis)(distance * sin(dir)));
				Posi dest_pos = target_cha->GetPos() + diff_pos;
				target_cha->SkillResetPos(dest_pos, Character::SKILL_RESET_POS_TYPE_JITUI, m_skill_id, true, chara->GetId());
			}
		}
	}
}

int SkillBase::GetTargetByQuadrate(Scene *scene, const Posi &point, Axis range, ObjID *target_list, int max_num)
{
	static ObjID obj_list[256] = {0};
	int count = scene->GetZoneMatrix()->GetObjByArea(point, range, range, obj_list, sizeof(obj_list) / sizeof(obj_list[0]));

	int num = 0;
	for (int i = 0; num < max_num && i < count; ++i)
	{
		Obj *obj = scene->GetObj(obj_list[i]);
		if (NULL != obj && Obj::IsCharactor(obj->GetObjType()))
		{
			Character *target_cha = (Character *)obj;
			if (target_cha->GetEffectList()->IsInvisible()) continue;

			if (!scene->GetMap()->SaveArea(obj->GetPos().x, obj->GetPos().y))
			{
				target_list[num++] = obj_list[i];
			}
		}
	}

	return num;
}

// 根据长宽获取矩形（有斜率）范围目标，center 为矩形的中心点
int SkillBase::GetTargetByRect(Scene *scene, const Posi &pos_self, const Posi &pos_target, Axis width, Axis height, ObjID *target_list, int max_num)
{
	int cx = pos_self.x;
	int cy = pos_self.y;

	// U = V - C
	float ux = static_cast<float>(pos_target.x - cx);
	float uy = static_cast<float>(pos_target.y - cy);
	float lenght_u = std::sqrt(ux*ux + uy*uy);
	float qx = (lenght_u == 0.0f) ? 0.0f : ((ux * width) / lenght_u);
	float qy = (lenght_u == 0.0f) ? 0.0f : ((uy * width) / lenght_u);

	// 矩形中心点
	float ctx = cx + qx;
	float cty = cy + qy;
	Posi center(static_cast<int>(ctx), static_cast<int>(cty));

	static ObjID obj_list[64] = {0};
	int count = scene->GetZoneMatrix()->GetObjByArea(center, width+height, width+height, obj_list, sizeof(obj_list) / sizeof(obj_list[0]));

	int num = 0;
	for (int i = 0; i < count && num < max_num; ++ i)
	{
		Obj *obj = scene->GetObj(obj_list[i]);
		if (NULL != obj && Obj::IsCharactor(obj->GetObjType()))
		{
			Character *target_cha = (Character *)obj;
			if (target_cha->GetEffectList()->IsInvisible()) continue;  // 隐身
			if (scene->GetMap()->SaveArea(obj->GetPos().x, obj->GetPos().y)) continue; // 在安全区

			int px = obj->GetPos().x;
			int py = obj->GetPos().y;
			// D = P - C
			float dx = static_cast<float>(px - cx);
			float dy = static_cast<float>(py - cy);

			// sin(α) = sin(|A - B|) = |sin(A)cos(B) - cos(A)sin(B)|
			float dis_y = (lenght_u == 0.0f) ? 0.0f : (std::abs(dy * ux - dx * uy) / lenght_u);

			// T = P - Center
			float tx = px - ctx;
			float ty = py - cty;
			float dis_x = std::sqrt(tx*tx + ty*ty - dis_y*dis_y);

			// 在矩形范围
			if (dis_y <= height && dis_x <= width)
			{
				target_list[num++] = obj_list[i];
			}
		}
	}
	return num;
}

// 根据半径获取扇形范围目标
int SkillBase::GetTargetBySector(Scene *scene, const Posi &pos_self, const Posi &pos_target, Axis radius, int angle, ObjID *target_list, int max_num)
{
	static ObjID obj_list[256] = {0};
	int count = scene->GetZoneMatrix()->GetObjByArea(pos_self, radius, radius, obj_list, sizeof(obj_list) / sizeof(obj_list[0]));

	const float pi = std::acos(-1.0f);
	float square_r = static_cast<float>(radius * radius);
	float theta = ((angle / 2.0f) / 180.0f ) * pi;
	float cos_theta = std::cos(theta);
	int num = 0;
	for (int i = 0; num < max_num && i < count; ++i)
	{
		Obj *obj = scene->GetObj(obj_list[i]);
		if (NULL != obj && Obj::IsCharactor(obj->GetObjType()))
		{
			Character *target_cha = (Character *)obj;
			if (target_cha->GetEffectList()->IsInvisible()) continue;
			if (scene->GetMap()->SaveArea(obj->GetPos().x, obj->GetPos().y)) continue;

			// D = P - C
			float dx = static_cast<float>(obj->GetPos().x - pos_self.x);
			float dy = static_cast<float>(obj->GetPos().y - pos_self.y);
			// U = V - C
			float ux = static_cast<float>(pos_target.x - pos_self.x);
			float uy = static_cast<float>(pos_target.y - pos_self.y);
			float square_len = dx*dx + dy*dy;

			// 在圆形区域内
			// |D|^2 > |U|^2
			if (square_len > square_r) continue;

			// 在扇形区域内
			float length_d = std::sqrt(square_len);
			float length_u = std::sqrt(ux*ux + uy*uy);
			float dot = (dx*ux + dy*uy);

			// D dot U > |D||U| cos(theta)
			if (dot > length_d * length_u * cos_theta)
			{
				target_list[num++] = obj_list[i];
			}
		}
	}
	return num;
}

// 根据半径获取圆形范围目标
int SkillBase::GetTargetByCircle(Scene *scene, const Posi &point, Axis radius, ObjID *target_list, int max_num)
{
	static ObjID obj_list[256] = {0};
	int count = scene->GetZoneMatrix()->GetObjByArea(point, radius, radius, obj_list, sizeof(obj_list) / sizeof(obj_list[0]));

	int num = 0;
	float square_r = static_cast<float>(radius * radius);
	for (int i = 0; num < max_num && i < count; ++i)
	{
		Obj *obj = scene->GetObj(obj_list[i]);
		if (NULL != obj && Obj::IsCharactor(obj->GetObjType()))
		{
			Character *target_cha = (Character *)obj;
			if (target_cha->GetEffectList()->IsInvisible()) continue;
			if (scene->GetMap()->SaveArea(obj->GetPos().x, obj->GetPos().y)) continue;

			float dx = static_cast<float>(obj->GetPos().x - point.x);
			float dy = static_cast<float>(obj->GetPos().y - point.y);
			float square_len = dx*dx + dy*dy;

			// |D|^2 > |U|^2
			if (square_len > square_r) continue;

			// 在圆内区域内
			target_list[num++] = obj_list[i];
		}
	}
	return num;
}

double SkillBase::GetGongji(Character *cha, Character *target) const
{
	double gongji = (double)cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI);

	// 怪物战力比玩家高，玩家的攻击有加成
	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType() && Obj::OBJ_TYPE_MONSTER == target->GetObjType())
	{
		Role *role = (Role*)cha;
		Monster *monster = (Monster *)target;

		double monster_capability = static_cast<double>(monster->GetCapability());
		double role_capability = static_cast<double>(role->GetCapability()->GetTotalCapability());
		if (monster_capability > 0 && role_capability > 0 && monster_capability > role_capability)
		{
			gongji = gongji * (monster_capability / role_capability);
		}
	}
	
	return gongji;
}

bool SkillBase::GetIsPojia(Character *cha) const
{
	// 破甲：根据破甲率触发破甲
	double pojia_per = (double)(cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_JINGZHUN) * ROLE_ATTR_PER);
	return (RandomNum(100) <= (int)(pojia_per * 100));
}

bool SkillBase::GetIsHuixinyiji(Character *cha, bool is_baoji, int &fight_type) const
{
	// 会心一击：必须在暴击的基础上，如果触发会心一击，则用会心一击伤害率代替暴击伤害率
	bool is_huixinyiji = false;
	if (is_baoji)
	{
		Attribute huixinyiji = cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HUIXINYIJI);
		huixinyiji += this->GetAddHuixinyiji(cha);
		if (RandomNum(10000) < huixinyiji)
		{
			is_huixinyiji = true;
			fight_type = EffectBase::FIGHT_TYPE_HUIXINYIJI;
		}
	}

	return is_huixinyiji;
}

bool SkillBase::GetIsGedang(Character *cha, Character *target, int &fight_type) const
{
	if (nullptr == cha || nullptr == target)
	{
		return false;
	}

	// 格挡概率 = 格挡率(目标) - 对方格挡抵抗率(攻击者)
	Attribute gedang_dikang_per = cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GEDANG_DIKANG_PER);
	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	{
		//Role * role = dynamic_cast<Role*>(cha);
	}
	Attribute gedang_per = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GEDANG_PER);
	if (Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		//Role * target_role = dynamic_cast<Role*>(target);
	}

	Attribute gedang_rate = gedang_per - gedang_dikang_per;
	if (gedang_rate <= 0)
	{
		return false;
	}

	bool is_gedang = false;
	if (RandomNum(10000) < gedang_rate)
	{
		is_gedang = true;

		// 如果暴击和会心一击都没触发，则显示格挡
		if (fight_type != EffectBase::FIGHT_TYPE_BAOJI && fight_type != EffectBase::FIGHT_TYPE_HUIXINYIJI)
		{
			fight_type = EffectBase::FIGHT_TYPE_GEDANG;
		}
	}

	return is_gedang;
}

bool SkillBase::GetIsZhufuyiji(Character *cha) const
{
	if (nullptr == cha)
	{
		return false;
	}

	// 祝福一击
	bool is_zhufu = false;
	Attribute zhufuyiji_per = cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_ZHUFUYIJI_PER);
	if (RandomNum(10000) < zhufuyiji_per)
	{
		is_zhufu = true;
	}

	return is_zhufu;
}

double SkillBase::GetHurtIncPercent(Character *cha, Character *target, const double add_hurt_percent, const double pofang_per) const
{
	// 伤害加成
	double add_hurt_per = 1.0;
	add_hurt_per *= (1.0 + add_hurt_percent);
	add_hurt_per *= (1.0 + pofang_per);
	add_hurt_per *= (1.0 + cha->GetSkillManager()->GetHurtIncPercent() * SKILL_ATTR_PER);
	add_hurt_per *= (1.0 + cha->GetEffectList()->GetHurtIncPercent(target) * SKILL_ATTR_PER);
	add_hurt_per *= (1.0 + cha->GetHurtIncPercent(target) * SKILL_ATTR_PER);

	return add_hurt_per;
}

double SkillBase::GetHurtDecPercent(Character *cha, Character *target, const double mianshang_per) const
{
	// 伤害减免
	double dec_hurt_per = 1.0;

	double real_mianshang_per = mianshang_per;
	dec_hurt_per *= (1.0 - real_mianshang_per);
	if (target->GetObjType() == Obj::OBJ_TYPE_ROLE)
	{
		Role *target_role = dynamic_cast<Role *>(target);
		Attribute impguard_mianshang_per = target_role->GetImpGuard()->GetPerMianshangVal();
		dec_hurt_per *= (1.0 - (impguard_mianshang_per * SKILL_ATTR_PER));

		// 传世名剑
		dec_hurt_per *= (1.0 - target_role->GetMojieSkill()->GetMianshangPercent() * SKILL_ATTR_PER);
	}

	dec_hurt_per *= (1.0 - target->GetSkillManager()->GetHurtDecPercent()  * SKILL_ATTR_PER);
	dec_hurt_per *= (1.0 - target->GetEffectList()->GetHurtDecPercent(cha) * SKILL_ATTR_PER);
	dec_hurt_per *= (1.0 - target->GetHurtDecPercent(cha) * SKILL_ATTR_PER);

	return dec_hurt_per;
}

double SkillBase::GetObjTypeAddHurtPer(Character *cha, Character *target) const
{
	double add_hurt_per = 1.0;
	// 玩家打怪
	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType() && Obj::OBJ_TYPE_MONSTER == target->GetObjType())
	{
		add_hurt_per *= this->GetRMAddHurtPer(cha, target);
	}

	// 怪打玩家
	if (Obj::OBJ_TYPE_MONSTER == cha->GetObjType() && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		add_hurt_per *= this->GetMRAddHurtPer(cha, target);
	}

	// 玩家打玩家
	else if (Obj::OBJ_TYPE_ROLE == cha->GetObjType() && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		add_hurt_per *= this->GetRRAddHurtPer(cha, target);
	}

	return add_hurt_per;
}

double SkillBase::GetRMAddHurtPer(Character *cha, Character *target) const
{
	Role *role = (Role*)cha;
	Monster *monster = (Monster *)target;
	
	double add_hurt_per = 1.0;
	// 装备仙品属性伤害加成
	add_hurt_per *= (1.0 + role->GetEquipmentManager()->EquipXianPinGetValue(Equipment::E_XIANPIN_ADD_HURT_TO_MONSTER_PER) * SKILL_ATTR_PER );
	add_hurt_per *= (1.0 + role->GetZhuanzhiEquip()->EquipXianPinFixValue(Equipment::E_XIANPIN_ADD_HURT_TO_MONSTER_PER) * SKILL_ATTR_PER );

	// 怪物类型为boss
	if (monster->IsBossMonster())
	{
		// 勋章伤害加成
		add_hurt_per *= (1.0 + role->GetXunZhangManager()->XunZhangGetAttributeValue(XUNZHANG_ADD_HURT_TO_BOSS_PER) * SKILL_ATTR_PER );

		//神兵伤害加成
		add_hurt_per *= (1.0 + role->GetShenBingManager()->OnHurtBoss() * SKILL_ATTR_PER );

	}
	else
	{
		// 勋章伤害加成
		add_hurt_per *= (1.0 + role->GetXunZhangManager()->XunZhangGetAttributeValue(XUNZHANG_ADD_HURT_TO_MONSTER_PER) * SKILL_ATTR_PER );
	}

	// 传世名剑
	add_hurt_per *= (1.0 + role->GetMojieSkill()->GetPVEZengshangPercent() * SKILL_ATTR_PER);

	//pve 增伤属性
	add_hurt_per *= (1.0 + role->GetCharaIntAttr(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER) * SKILL_ATTR_PER );

	// 个人目标加成
	add_hurt_per *= (1.0 + role->GetRoleGoal()->GetHurtMonsterAdd() * SKILL_ATTR_PER );

	return add_hurt_per;
}

double SkillBase::GetMRAddHurtPer(Character *cha, Character *target) const
{
	Monster *monster = dynamic_cast<Monster *>(cha);
	Role *role = dynamic_cast<Role *>(target);
	
	double add_hurt_per = 1.0;

	// 等级差碾压
	int delta_level = (monster->GetLevel() - role->GetLevel());
	if (delta_level > 0)
	{
		bool is_ignore = LOGIC_CONFIG->GetMonsterHurtHpConfig().IsIgnoreLevelDelta(role->GetScene()->GetSceneType());
		LevelDeltaCfg* level_delta_cfg = LOGIC_CONFIG->GetMonsterHurtHpConfig().GetLevelDeltaCfg(delta_level);
		if (nullptr != level_delta_cfg && !is_ignore)
		{
			add_hurt_per *= (1.0 + level_delta_cfg->add_hurt_per * SKILL_ATTR_PER);
		}
	}

	return add_hurt_per;
}

double SkillBase::GetRRAddHurtPer(Character *cha, Character *target) const
{
//	Role *role_target = (Role*)target;

	double add_hurt_per = 1.0;

	Role *role_attack = (Role*)cha;
	// 装备仙品属性伤害加成
	add_hurt_per *= ( 1.0 + role_attack->GetEquipmentManager()->EquipXianPinGetValue(Equipment::E_XIANPIN_ADD_HURT_TO_ROLE_PER) * SKILL_ATTR_PER);
	add_hurt_per *= (1.0 + role_attack->GetZhuanzhiEquip()->EquipXianPinFixValue(Equipment::E_XIANPIN_ADD_HURT_TO_ROLE_PER) * SKILL_ATTR_PER);

	// pvp 攻击者增伤
	add_hurt_per *= (1.0 + role_attack->GetCharaIntAttr(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER) * SKILL_ATTR_PER);

	return add_hurt_per;
}

long long SkillBase::GetBaojiExtraHurt(Character *cha, Character *target) const
{
	if (nullptr == cha || Obj::OBJ_TYPE_ROLE != cha->GetObjType())
	{
		return 0;
	}

	Role *role_attacker = dynamic_cast<Role *>(cha);
	long long baoji_extra_hurt = 0;

	// 披风
	baoji_extra_hurt += role_attacker->GetCloakManager()->CalcBaojiExtraHurt(target);


	return baoji_extra_hurt;
}

double SkillBase::GetObjTypeDecHurtPer(Character *cha, Character *target) const
{
	double dec_hurt_per = 1.0;

	// 怪打玩家
	if (Obj::OBJ_TYPE_MONSTER == cha->GetObjType() && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		dec_hurt_per *= this->GetMRDecHurtPer(target);
	}

	// 玩家打怪
	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType() && Obj::OBJ_TYPE_MONSTER == target->GetObjType())
	{
		dec_hurt_per *= this->GetRMDecHurtPer(cha, target);
	}

	// 玩家打玩家
	else if (Obj::OBJ_TYPE_ROLE == cha->GetObjType() && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		dec_hurt_per *= this->GetRRDecHurtPer(cha, target);
	}

	return dec_hurt_per;
}

double SkillBase::GetMRDecHurtPer(Character *target) const
{
	Role *role = (Role*)target;

	double dec_hurt_per = 1.0;
	// 装备仙品属性伤害减免
	dec_hurt_per *= (1.0 - role->GetEquipmentManager()->EquipXianPinGetValue(Equipment::E_XIANPIN_DEC_HURT_FROM_MONSTER_PER) * SKILL_ATTR_PER );
	dec_hurt_per *= (1.0 - role->GetZhuanzhiEquip()->EquipXianPinFixValue(Equipment::E_XIANPIN_DEC_HURT_FROM_MONSTER_PER) * SKILL_ATTR_PER );

	//pve 减伤属性
	dec_hurt_per *= (1.0 - role->GetCharaIntAttr(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER) * SKILL_ATTR_PER );

	return dec_hurt_per;
}

double SkillBase::GetRMDecHurtPer(Character *cha, Character *target) const
{
	Role *role = dynamic_cast<Role *>(cha);
	Monster *monster = dynamic_cast<Monster *>(target);

	double dec_hurt_per = 1.0;

	// 等级差碾压
	int delta_level = ( monster->GetLevel() - role->GetLevel() );
	if (delta_level > 0)
	{
		bool is_ignore = LOGIC_CONFIG->GetMonsterHurtHpConfig().IsIgnoreLevelDelta(role->GetScene()->GetSceneType());
		LevelDeltaCfg* level_delta_cfg = LOGIC_CONFIG->GetMonsterHurtHpConfig().GetLevelDeltaCfg(delta_level);
		if (nullptr != level_delta_cfg && !is_ignore)
		{
			dec_hurt_per *= (1.0 -  level_delta_cfg->dec_attack_per * SKILL_ATTR_PER);
		}
	}

	return dec_hurt_per;
}

double SkillBase::GetRRDecHurtPer(Character *cha, Character *target) const
{
	Role *role_target = (Role*)target;

	double dec_hurt_per = 1.0;
	// 装备仙品属性伤害减免
	dec_hurt_per *= (1.0 - role_target->GetEquipmentManager()->EquipXianPinGetValue(Equipment::E_XIANPIN_DEC_HURT_FROM_ROLE_PER) * SKILL_ATTR_PER );
	dec_hurt_per *= (1.0 - role_target->GetZhuanzhiEquip()->EquipXianPinFixValue(Equipment::E_XIANPIN_DEC_HURT_FROM_ROLE_PER) * SKILL_ATTR_PER );

	// 足迹伤害减免
	dec_hurt_per *= (1.0 - role_target->GetFootPrint()->GetHurtDecPercent() * SKILL_ATTR_PER );

	// 法宝伤害减免
	dec_hurt_per *= (1.0 - role_target->GetFabaoManager()->GetHurtDecPercent() * SKILL_ATTR_PER );

//	Role *role_attack = (Role*)cha;
	// 装备仙品属性伤害减免

	// 传世名剑
	dec_hurt_per *= (1.0 - role_target->GetMojieSkill()->GetPVPJianshangPercent() * SKILL_ATTR_PER);

	// pvp 被击者减伤
	dec_hurt_per *= (1.0 - role_target->GetCharaIntAttr(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER) * SKILL_ATTR_PER );

	return dec_hurt_per;
}

long long SkillBase::GetOtherSysInjure(Character *cha, Character *target, long long base_injure) const
{
	Role *role = (Role *)cha;

	long long injure = 0;
	//神兽
	{
		injure += role->GetShenshou()->RecalcExtraInjureById(target, base_injure, m_skill_id);
	}
	//天赋
	{
		injure += role->GetTalent()->GetTalentFixInjure(target, base_injure, m_skill_id);
	}
	// 异火技能
	{
		injure += role->GetShenzhouWeapon()->GetShenzhouSkillFixInjure(target, base_injure, m_skill_id);
	}

	return injure;
}

void SkillBase::CalcFixInjure(Character *cha, Character *target, long long &injure) const
{
	// 伤害修正
	if (injure < 10)
	{
		injure = RandomNum(10, 21);
	}

	injure = cha->GetScene()->GetSpecialLogic()->SpecialRecalcInjure(cha, target, injure);
}

void SkillBase::CalcFixHuretInjure(Character *cha, Character *target, long long &injure) const
{
	// 致命一击：打掉固定血量
	float hp_fix_hurt_per = cha->GetEffectList()->GetFixHurtPercent(target);
	if (hp_fix_hurt_per > 0)
	{
		injure = static_cast<long long>(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (hp_fix_hurt_per * SKILL_ATTR_PER));
	}
}

double SkillBase::GetAddBaojiHurtPer(Character *cha) const
{
	double add_baoji_hurt_per = 0.0;

	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	{
		Role *role = (Role*)cha;

		// 传世名剑
		add_baoji_hurt_per += (role->GetMojieSkill()->GetBaojiHurtPercent() * SKILL_ATTR_PER);
	}

	return add_baoji_hurt_per;
}

double SkillBase::GetAddKangbaoPer(Character *cha) const
{
	double add_kangbao_per = 0.0;

	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	{
		Role *role = (Role*)cha;

		// 传世名剑
		add_kangbao_per += (role->GetMojieSkill()->GetKangbaoPercent() * SKILL_ATTR_PER);
	}

	return add_kangbao_per;
}

double SkillBase::GetAddKangbaoHurtPer(Character *cha) const
{
	double add_kangbao_hurt_per = 0.0;

	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	{
		Role *role = (Role*)cha;

		// 传世名剑
		add_kangbao_hurt_per += (role->GetMojieSkill()->GetKangbaoHurtPercent() * SKILL_ATTR_PER);
	}

	return add_kangbao_hurt_per;
}

double SkillBase::GetAddHuixinyijiHurtPer(Character *cha) const
{
	double add_huixinyiji_hurt_per = 0.0;

	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	{
		Role *role = (Role*)cha;

		// 传世名剑
		add_huixinyiji_hurt_per += (role->GetMojieSkill()->GetHuiXinYiJiHurtPercent() * SKILL_ATTR_PER);
	}

	return add_huixinyiji_hurt_per;
}

Attribute SkillBase::GetAddHuixinyiji(Character *cha) const
{
	Attribute add_huixinyiji = 0;

	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	{
		Role *role = (Role*)cha;

		// 传世名剑
		add_huixinyiji += (role->GetMojieSkill()->GetHuiXinYiJi());
	}

	return add_huixinyiji;
}


