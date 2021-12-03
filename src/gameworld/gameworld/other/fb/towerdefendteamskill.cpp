#include "towerdefendteamskill.hpp"
#include "skill/skilldef.hpp"
#include "obj/character/character.h"
#include "obj/character/monster.h"
#include "obj/character/role.h"
#include "ai/monsterai/monsteraibase.hpp"

#include "effect/effectbase.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectinterval.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effecthpstore.hpp"
#include "effect/skilleffect/effectreboundhurt.hpp"
#include "effect/skilleffect/effectwudiprotect.hpp"

#include "effect/sceneeffect/sceneeffect.hpp"

#include "scene/scene.h"
#include "scene/speciallogic/speciallogic.hpp"
#include "scene/speciallogic/personspecial/specialtowerdefend.hpp"

#include "protocal/msgscene.h"
#include "servercommon/errornum.h"
#include "gamecommon.h"
#include "config/logicconfigmanager.hpp"
#include "friend/teaminfo.hpp"

void TeamTowerDefendSkill::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL != chara && Obj::OBJ_TYPE_ROLE == chara->GetObjType())
	{
		Role *role = (Role *)chara;

		switch (m_cfg_skill_id)
		{
		case 1:
			this->DoSkill1(role, level, target, pos);
			break;

		case 2:
			this->DoSkill2(role, level, target, pos);
			break;

		case 3:
			this->DoSkill3(role, level, target, pos);
			break;

		case 6:
			this->DoSkill6(role, level, target, pos);
			break;

		case 7:
			this->DoSkill7(role, level, target, pos);
			break;

		case 8:
			this->DoSkill8(role, level, target, pos);
			break;

		case 9:
			this->DoSkill9(role, level, target, pos);
			break;

		case 10:
			this->DoSkill10(role, level, target, pos);
			break;

		case 11:
			this->DoSkill11(role, level, target, pos);
			break;

		case 12:
			this->DoSkill12(role, level, target, pos);
			break;
		}
	}
}

void TeamTowerDefendSkill::DoSkill1(Role *role, int level, Character *target, const Posi &pos) const
{
	// 无敌
	if (NULL != role)
	{
		SendAOEAttackMsg(role, target, role->GetPos());

		int dur_ms = GetParamA(1) * 1000;

		EffectWudiProtect *effect_wudi = new EffectWudiProtect(role->GetId(), m_skill_id, dur_ms, 100, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		effect_wudi->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
		role->AddEffect(effect_wudi);
	}
}

void TeamTowerDefendSkill::DoSkill2(Role *role, int level, Character *target, const Posi &pos) const
{
	// 群嘲
	if (NULL != role && NULL != role->GetScene())
	{
		if (!this->CheckDis(role->GetPos(), pos))
		{
			role->NoticeNum(errornum::EN_ROLE_FIGHT_DISTANCE_NOT_ENOUGH);
			return;
		}

		SendAOEAttackMsg(role, target, pos);

		static ObjID target_list[32] = {INVALID_OBJ_ID};

		int count = SkillBase::GetTargetByArea(role, pos, 15, target_list, AOE_ATK_ENEMY_COUNT);

		Scene *scene = role->GetScene();
		for (int i = 0; i < count; ++i)
		{
			Character *target = (Character*)scene->GetObj(target_list[i]);
			if (NULL == target) continue;

			if (Obj::OBJ_TYPE_MONSTER != target->GetObjType()) continue;

			// 先判断是否闪避
			if (IsShanbi(role, target)) continue;

			int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
			// 是否受技伤影响
			bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
			long long injure = CalcInjureAndType(role, target, level, fight_type, is_be_effect_skill_per);

			EffectOnce *effect = new EffectOnce(role->GetId(), m_skill_id, -injure, fight_type);
			target->AddEffect(effect);

			Monster *monster = (Monster *)target;
			if (NULL != monster->GetMonsterAI())
			{
				monster->GetMonsterAI()->OnChaofeng(role, injure * 10);
			}
		}

		float reduce_percent = static_cast<float>(GetParamA(1) + GetParamB(1) * level);
		int dur_ms = GetParamC(1) * 1000;

		EffectHpStore *effect_hp_store = new EffectHpStore(role->GetId(), m_skill_id, dur_ms, GetParamD(1), level, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		effect_hp_store->SetReplacePercent(reduce_percent);
		effect_hp_store->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
		role->AddEffect(effect_hp_store);
	}
}

void TeamTowerDefendSkill::DoSkill3(Role *role, int level, Character *target, const Posi &pos) const
{
	// 降低攻击百分比 （践踏）
	if (NULL != role)
	{
		if (!this->CheckDis(role->GetPos(), pos))
		{
			role->NoticeNum(errornum::EN_ROLE_FIGHT_DISTANCE_NOT_ENOUGH);
			return;
		}

		SendAOEAttackMsg(role, target, role->GetPos());

		static ObjID target_list[32] = {INVALID_OBJ_ID};

		int count = SkillBase::GetTargetByArea(role, pos, 15, target_list, AOE_ATK_ENEMY_COUNT);

		int dec_percent = GetParamA(1) + GetParamB(1) * level;
		int dur_ms = GetParamC(1) * 1000;

		Scene *scene = role->GetScene();
		for (int i = 0; i < count; ++i)
		{
			Character *target = (Character*)scene->GetObj(target_list[i]);
			if (NULL == target) continue;

			// 先判断是否闪避
			if (IsShanbi(role, target)) continue;

			int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
			// 是否受技伤影响
			bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);
			long long injure = CalcInjureAndType(role, target, level, fight_type, is_be_effect_skill_per);

			EffectOnce *effect = new EffectOnce(role->GetId(), m_skill_id, -injure, fight_type);
			target->AddEffect(effect);

			Attribute dec_gongji = static_cast<Attribute>(target->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (dec_percent / 100.0f));

			EffectBuff *effect_buff = new EffectBuff(role->GetId(), m_skill_id, dur_ms, level, EffectBase::PRODUCT_METHOD_SCENE_SKILL,EBT_REDUCE_GONGJI);
			effect_buff->AddGongJiEffect(-dec_gongji, -dec_percent);
			effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
			target->AddEffect(effect_buff);
		}
	}
}

void TeamTowerDefendSkill::DoSkill12(Role *role, int level, Character *target, const Posi &pos) const
{
	// 单体攻击
	if (NULL != role && NULL != target && role->IsEnemy(target))
	{
		if (!this->CheckDis(role->GetPos(), target->GetPos()))
		{
			role->NoticeNum(errornum::EN_ROLE_FIGHT_DISTANCE_NOT_ENOUGH);
			return;
		}

		SendAttackMsg(role, target);

		// 先判断是否闪避
		if (IsShanbi(role, target)) return;

		double add_hurt_percent = static_cast<double>(GetParamA(1) * level / 100.0f);

		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);

		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		long long injure = CalcInjureAndType(role, target, level, fight_type, is_be_effect_skill_per, add_hurt_percent);

		EffectOnce *effect = new EffectOnce(role->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);
	}
}

void TeamTowerDefendSkill::DoSkill6(Role *role, int level, Character *target, const Posi &pos) const
{
	// 群攻
	if (NULL != role && NULL != role->GetScene())
	{
		if (!this->CheckDis(role->GetPos(), pos))
		{
			role->NoticeNum(errornum::EN_ROLE_FIGHT_DISTANCE_NOT_ENOUGH);
			return;
		}

		SendAOEAttackMsg(role, target, pos);

		double add_hurt_percent = static_cast<double>(GetParamA(1) * level / 100.0f);

		static ObjID target_list[32] = {INVALID_OBJ_ID};

		int count = SkillBase::GetTargetByArea(role, pos, DEFAULT_AOE_ATTACK_RANGE, target_list, AOE_ATK_ENEMY_COUNT);

		Scene *scene = role->GetScene();
		for (int i = 0; i < count; ++i)
		{
			Character *target = (Character*)scene->GetObj(target_list[i]);
			if (NULL == target) continue;

			// 先判断是否闪避
			if (IsShanbi(role, target)) continue;

			// 是否受技伤影响
			bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);

			int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
			long long injure = CalcInjureAndType(role, target, level, fight_type, is_be_effect_skill_per, add_hurt_percent);

			EffectOnce *effect = new EffectOnce(role->GetId(), m_skill_id, -injure, fight_type);
			target->AddEffect(effect);
		}
	}
}

void TeamTowerDefendSkill::DoSkill7(Role *role, int level, Character *target, const Posi &pos) const
{
	// 法阵 范围伤害
	if (NULL != role && NULL != role->GetScene())
	{
		if (!this->CheckDis(role->GetPos(), pos))
		{
			role->NoticeNum(errornum::EN_ROLE_FIGHT_DISTANCE_NOT_ENOUGH);
			return;
		}

		SendAOEAttackMsg(role, target, pos);

		int hurt = static_cast<int>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * ((GetParamA(1) + GetParamB(1) * level) / 100.0f));
		int count = GetParamC(level) / 1;

		SceneEffectTowerDefendFaZhen *scene_effect = new SceneEffectTowerDefendFaZhen(role->GetScene(), role->GetId(), m_skill_id, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		scene_effect->SetFaZhenInfo(pos, DEFAULT_AOE_ATTACK_RANGE, 1000, count, hurt);

		role->GetScene()->GetSpecialLogic()->AddSceneEffect(scene_effect);
	}
}

void TeamTowerDefendSkill::DoSkill8(Role *role, int level, Character *target, const Posi &pos) const
{
	// 单体攻击
	if (NULL != role && NULL != target && role->IsEnemy(target))
	{
		if (!this->CheckDis(role->GetPos(), target->GetPos()))
		{
			role->NoticeNum(errornum::EN_ROLE_FIGHT_DISTANCE_NOT_ENOUGH);
			return;
		}

		SendAttackMsg(role, target);

		// 先判断是否闪避
		if (IsShanbi(role, target)) return;

		double add_hurt_percent = static_cast<double>(GetParamA(1) * level / 100.0f);

		// 是否受技伤影响
		bool is_be_effect_skill_per = GetIsBeEffectSkillPercent(level);

		int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
		long long injure = CalcInjureAndType(role, target, level, fight_type, is_be_effect_skill_per, add_hurt_percent);

		EffectOnce *effect = new EffectOnce(role->GetId(), m_skill_id, -injure, fight_type);
		target->AddEffect(effect);
	}
}

void TeamTowerDefendSkill::DoSkill9(Role *role, int level, Character *target, const Posi &pos) const
{
	// 群体加血
	if (NULL != role)
	{
		SendAOEAttackMsg(role, target, role->GetPos());

		Role *role_list[16] = {NULL};
		role_list[0] = role;

		int team_member_role_num = 1;	
		team_member_role_num += role->GetTeamInfo()->GetOtherMemberInTeam(&role_list[1], (sizeof(role_list) / sizeof(role_list[0]) - 1));

		Attribute add_hp = GetParamA(1) + GetParamB(1) * level + static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (GetParamC(1) * level / 100.0f));

		for (int i = 0; i < team_member_role_num; ++i)
		{
			if (NULL != role_list[i] && role_list[i]->IsAlive())
			{
				EffectOnce *effect_addhp = new EffectOnce(INVALID_OBJ_ID, m_skill_id, add_hp, EffectBase::FIGHT_TYPE_NORMAL);
				role_list[i]->AddEffect(effect_addhp);
			}
		}
	}
}

void TeamTowerDefendSkill::DoSkill10(Role *role, int level, Character *target, const Posi &pos) const
{
	// 群体加攻防
	if (NULL != role)
	{
		SendAOEAttackMsg(role, target, role->GetPos());

		Role *role_list[16] = {NULL};
		role_list[0] = role;

		int team_member_role_num = 1;	
		team_member_role_num += role->GetTeamInfo()->GetOtherMemberInTeam(&role_list[1], (sizeof(role_list) / sizeof(role_list[0]) - 1));

		int add_percent = GetParamA(1) + GetParamB(1) * level;
		int dur_ms = GetParamC(1) * 1000;

		for (int i = 0; i < team_member_role_num; ++i)
		{
			if (NULL != role_list[i])
			{
				Attribute add_gongji = static_cast<Attribute>(role_list[i]->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (add_percent / 100.0));
				Attribute add_fangyu = static_cast<Attribute>(role_list[i]->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * (add_percent / 100.0));

				EffectBuff *effect_buff = new EffectBuff(role->GetId(), m_skill_id, dur_ms, static_cast<char>(101), EffectBase::PRODUCT_METHOD_SCENE_SKILL, EBT_ADD_MULTI_ATTR);
				effect_buff->AddGongJiEffect(add_gongji, add_percent);
				effect_buff->AddFangYuEffect(add_fangyu, add_percent);
				effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
				role_list[i]->AddEffect(effect_buff);
			}
		}
	}
}

void TeamTowerDefendSkill::DoSkill11(Role *role, int level, Character *target, const Posi &pos) const
{
	// 群体定身
	if (NULL != role && NULL != role->GetScene())
	{
		if (!this->CheckDis(role->GetPos(), pos))
		{
			role->NoticeNum(errornum::EN_ROLE_FIGHT_DISTANCE_NOT_ENOUGH);
			return;
		}

		SendAOEAttackMsg(role, target, pos);

		static ObjID target_list[32] = {INVALID_OBJ_ID};

		int count = SkillBase::GetTargetByArea(role, pos, DEFAULT_AOE_ATTACK_RANGE, target_list, AOE_ATK_ENEMY_COUNT);

		Scene *scene = role->GetScene();
		for (int i = 0; i < count; ++i)
		{
			Character *target = (Character*)scene->GetObj(target_list[i]);
			if (NULL == target) continue;

			// 先判断是否闪避
			if (IsShanbi(role, target)) continue;

			int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
			long long injure = CalcInjureAndType(role, target, level, fight_type);

			EffectOnce *effect = new EffectOnce(role->GetId(), m_skill_id, -injure, fight_type);
			target->AddEffect(effect);

			int dur_ms = GetParamA(1) * 1000;

			EffectSpecialState *effect_dingshen = new EffectSpecialState(role->GetId(), m_skill_id, dur_ms, EBT_DINGSHEN, level, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
			effect_dingshen->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
			target->AddEffect(effect_dingshen);
		}
	}
}

bool TeamTowerDefendSkill::CheckDis(const Posi &src_pos, const Posi &dst_pos) const
{
	return !(dst_pos - src_pos).IsLongerThan(GetAttackDis());
}
