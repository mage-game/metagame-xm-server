#include "effectbase.hpp"

#include "obj/character/character.h"
#include "obj/character/role.h"

#include "scene/scene.h"
#include "scene/speciallogic/speciallogic.hpp"

#include "protocal/msgfight.h"

#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effecthpstore.hpp"
#include "effect/skilleffect/effectinterval.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectcounter.hpp"
#include "effect/skilleffect/effecthurtinc.hpp"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "effect/skilleffect/effectdoublehurt.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "effect/skilleffect/effectbomb.hpp"
#include "effect/skilleffect/effectwudiprotect.hpp"
#include "effect/skilleffect/effectmodifyglobalcd.hpp"
#include "effect/skilleffect/effectreboundhurt.hpp"
#include "effect/skilleffect/effectrecoveronatked.hpp"
#include "effect/skilleffect/effectgradualhurt.hpp"
#include "effect/skilleffect/effectjuhuapig.hpp"
#include "effect/skilleffect/effectinvisible.hpp"
#include "effect/skilleffect/effectattrhurtrandinc.hpp"
#include "effect/skilleffect/effectbianshen.hpp"
#include "effect/skilleffect/effectmpnocost.hpp"
#include "effect/skilleffect/effectshenbing.hpp"
#include "effect/skilleffect/effectabsorbhp.hpp"
#include "effect/skilleffect/effectpercentbuff.hpp"
#include "effect/skilleffect/effectotherbuff.hpp"
#include "effect/skilleffect/effectattrcontainer.hpp"
#include "effect/skilleffect/effecthspecialshield.hpp"
#include "effect/skilleffect/effectattrstore.hpp"
#include "effect/sceneeffect/sceneeffect.hpp"
#include "effect/skilleffect/effectabsorbhpbeenattack.hpp"
#include "effect/skilleffect/effecttalenthpstore.hpp"
#include "effect/skilleffect/effectforbidrecover.hpp"
#include "engineadapter.h"
#include "config/logicconfigmanager.hpp"
#include "config/monster_hurt_hp_config.h"
#include "obj/obj.h"
#include "obj/character/monster.h"

bool EffectBase::CanEffect(Character *cha)
{
	return true;
}

void EffectBase::EffectHP(Attribute *real_delta_hp, Attribute *view_delta_hp, Character *cha, bool *die, bool is_from_skill, int fight_type)
{
	if (NULL == cha || NULL == die) return;
	if (NULL == real_delta_hp || NULL == view_delta_hp) return;

	bool is_fix_hurt = false;

	if (*real_delta_hp < 0)
	{
		int recover_percent_on_atked = cha->GetEffectList()->GetRecoverPercentOnAtked();

		if (is_from_skill && recover_percent_on_atked > 0)
		{
			*real_delta_hp = static_cast<Attribute>(-1 * (*real_delta_hp) * (recover_percent_on_atked / 100.0f));
		}
		else if (cha->GetEffectList()->IsInWudiProtect() || cha->IsFixHurtOnAttack())
		{
			*real_delta_hp = -cha->GetFixHurtOnAttack();
			
			// 人无敌，则掉-1血
			if (Obj::OBJ_TYPE_ROLE == cha->GetObjType()) *real_delta_hp = -1;

			is_fix_hurt = true;
		}
		else
		{
			EffectSpecialShield *effect_special_shield = cha->GetEffectList()->GetEffectSpecialShield();
			if (NULL != effect_special_shield)
			{
				effect_special_shield->ReplaceHurt(real_delta_hp, cha);
			}
		}
	}

	static Protocol::SCFightSpecialFloat injure_msg;
	injure_msg.obj_id = cha->GetId();
	injure_msg.deliver_obj_id = m_deliverer;

	if (!is_fix_hurt && *real_delta_hp < 0)
	{
		cha->OnAttackCalcHurt(m_deliverer, m_product_id, *real_delta_hp, is_from_skill, fight_type);
		
		EffectHpStore *effect_hpstore = cha->GetEffectList()->GetEffectHpStore();
		if (NULL != effect_hpstore)
		{
			Attribute old_real_delta_hp = *real_delta_hp;
			effect_hpstore->ReplaceHurt(real_delta_hp);

			//特殊飘字
			Attribute float_value = old_real_delta_hp - *real_delta_hp;
			if (-float_value < 0)
			{
				injure_msg.float_type = Protocol::SCFightSpecialFloat::EFFECT_HPSTORE;
				injure_msg.float_value = -float_value;
				injure_msg.skill_special_effect = Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_NON;
				cha->GetScene()->GetZoneMatrix()->NotifyAreaMsg(cha->GetObserHandle(), &injure_msg, sizeof(Protocol::SCFightSpecialFloat));
			}
		}
	}

	Attribute other_delta_hp = 0;
	Attribute baodi_delta_hp = 0;
	if (*real_delta_hp < 0)
	{

		if (SkillManager::IsRoleSkill(m_product_id) || SkillManager::IsGreateSoldierSkill(m_product_id))
		{
			if (!cha->GetEffectList()->IsInWudiProtect())
			{
				Obj *obj = cha->GetScene()->GetObj(m_deliverer);
				if (nullptr != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
				{
					Role *deliverer_role = static_cast<Role *>(obj);
					if (nullptr != deliverer_role)
					{
						// 人物技能在pvp时扣血量上限
						other_delta_hp = deliverer_role->GetSkillManager()->CalcSkillHurtByHpPer(cha, m_product_id, m_is_main_target);
						if (other_delta_hp > 0)
						{
							if (*real_delta_hp < -other_delta_hp)
							{
								*real_delta_hp = -other_delta_hp;
							}
						}

						// 人物技能在pvp时扣血量下限
						baodi_delta_hp = deliverer_role->GetSkillManager()->CalcSkillBaodiHurtByHpPer(cha, m_product_id, m_is_main_target);
						if (baodi_delta_hp > 0)
						{
							if (*real_delta_hp > -baodi_delta_hp)
							{
								*real_delta_hp = -baodi_delta_hp;
							}
						}
					}
				}
			}
		}
	}

	*view_delta_hp = *real_delta_hp;

	if (*real_delta_hp < 0) // 受到伤害的飘字/特效通知
	{
		Attribute holy_delta_hp = 0;
		EffectSpecialShield *effect_special_shield = cha->GetEffectList()->GetEffectSpecialShield();
		if (NULL != effect_special_shield)
		{
			holy_delta_hp = -1;
			effect_special_shield->ReplaceHurt(&holy_delta_hp, cha);
		}

		int special_effect_type = cha->CalcUpGradeSkillEffect(m_deliverer, m_product_id, *real_delta_hp, holy_delta_hp, is_from_skill, is_fix_hurt);

		if (holy_delta_hp < 0 && special_effect_type != Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_NON)
		{
			injure_msg.float_type = Protocol::SCFightSpecialFloat::EFFECT_UP_GRADE_SKILL;
			injure_msg.float_value = holy_delta_hp;
			injure_msg.skill_special_effect = special_effect_type;
			cha->GetScene()->GetZoneMatrix()->NotifyAreaMsg(cha->GetObserHandle(), &injure_msg, sizeof(Protocol::SCFightSpecialFloat));
		}
		else // 其他正常情况下的伤害
		{
			if (EffectBase::PRODUCT_METHOD_REBOUNDHURT == m_product_method)
			{
				injure_msg.float_type = Protocol::SCFightSpecialFloat::EFFECT_REBOUNDHURT;
				injure_msg.float_value = *real_delta_hp;
				injure_msg.skill_special_effect = Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_REBOUNDHURT;
				cha->GetScene()->GetZoneMatrix()->NotifyAreaMsg(cha->GetObserHandle(), &injure_msg, sizeof(Protocol::SCFightSpecialFloat));
			}
			else if (PRODUCT_METHOD_XIANNV_SHENGWU_SKILL == m_product_method)
			{
				injure_msg.float_type = Protocol::SCFightSpecialFloat::EFFECT_NORMAL_HURT;
				injure_msg.float_value = *real_delta_hp;
				injure_msg.skill_special_effect = Protocol::SCFightSpecialFloat::SPECIAL_EFFECT_XIANNV_SHENGWU_HURT;
				cha->GetScene()->GetZoneMatrix()->NotifyAreaMsg(cha->GetObserHandle(), &injure_msg, sizeof(Protocol::SCFightSpecialFloat));
			}
		}

		// 怪物配置最大伤害万分比
		if (cha->GetObjType() == Obj::OBJ_TYPE_MONSTER)
		{
			Monster * monster = (Monster *)cha;
			if (NULL != monster)
			{
				HurtConfig* max_hurt_cfg = LOGIC_CONFIG->GetMonsterHurtHpConfig().GetConfig(monster->GetMonsterId());
				if (max_hurt_cfg != NULL) // 不配就是打多少扣多少
				{
					Attribute max_hp = cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
					Attribute max_delta_hp = (Attribute)(-1 * max_hp * (max_hurt_cfg->percent / 10000.0));
					if (*real_delta_hp < max_delta_hp) *real_delta_hp = max_delta_hp;
				}
			}
		}
	}

	{
		//检查是否是致命的一击，注意：这个检查需要在ChangeHp之前
		Attribute cur_hp = cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		if (cur_hp <= -(*real_delta_hp))
		{
			if (cha->IsMianyiDeadlyAttack())
			{
				*real_delta_hp = 0;
			}
		}
	}

	if (0 != *real_delta_hp)
	{
		cha->ChangeHp(real_delta_hp);

		if (*real_delta_hp < 0)
		{
			// 受击者被伤害之后
			cha->OnAttack(m_deliverer, m_product_id, *real_delta_hp, is_from_skill);

			// 攻击者（角色）伤害别人之后
			Obj *obj = cha->GetScene()->GetObj(m_deliverer);
			if (nullptr != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
			{
				Role *deliverer_role = (Role *)obj;
				deliverer_role->OnAfterHurtOther(cha, m_product_id, -(*real_delta_hp), is_from_skill, fight_type);
			}
		}
	}

	if (!cha->IsAlive()) *die = true;
}

void EffectBase::SendAttackBlock(Character *cha, Attribute real_delta_hp, Attribute delta_hp, unsigned short fight_type, unsigned char blood_seq, bool is_send_product_id, int passive_flag)
{
	if (NULL == cha || NULL == cha->GetScene()) return;
	/*[-Wunused-variable]
	unsigned short tmp_attack_seq = 0;

	Obj *deliver_obj = cha->GetScene()->GetObj(m_deliverer);
	if (NULL != deliver_obj && Obj::IsCharactor(deliver_obj->GetObjType()))
	{
		Character *deliver_cha = (Character *)deliver_obj;
		tmp_attack_seq = deliver_cha->GetAttackSeq();
	}
	*/
	Protocol::SCObjChangeBlood oba;
	oba.obj_id = cha->GetId();
	oba.deliverer = m_deliverer;
	oba.product_method = static_cast<unsigned char>(m_product_method);
	oba.fighttype = static_cast<unsigned char>(fight_type);
	oba.real_blood = real_delta_hp;
	oba.blood = delta_hp;
	oba.skill = (is_send_product_id ? m_product_id : 0);
	oba.passive_flag = passive_flag;

	cha->GetScene()->GetZoneMatrix()->NotifyAreaMsg(cha->GetObserHandle(), &oba, sizeof(Protocol::SCObjChangeBlood));
}

void EffectBase::SendMPChangeMsg(Character *cha, Attribute delta_mp)
{
	Protocol::SCObjChangeMP oba;
	oba.obj_id = cha->GetId();
	oba.mp = delta_mp;
	if (NULL != cha->GetScene()) cha->GetScene()->GetZoneMatrix()->NotifyAreaMsg(cha->GetObserHandle(), &oba, sizeof(Protocol::SCObjChangeMP));
}

EffectBase * EffectBase::UnserializeCreateEffect(char effect_type)
{
	switch (effect_type)
	{
	//////////////////////////////////////////// 技能buff ////////////////////////////////////////
	case EFFECT_TYPE_ATTRBUFF:
		return new EffectBuff();

	case EFFECT_TYPE_SPECIAL_STATE:
		return new EffectSpecialState();

	case EFFECT_TYPE_INTERVAL_HP:
		return new EffectInterval();

	case EFFECT_TYPE_COUNTER:
		return new EffectCounter();

	case EFFECT_TYPE_INC_HURT:
		return new EffectIncHurt();

	case EFFECT_TYPE_DEC_HURT:
		return new EffectDecHurt();

	case EFFECT_TYPE_DOUBLE_HURT:
		return new EffectDoubleHurt();

	case EFFECT_TYPE_MOVESPEED:
		return new EffectMoveSpeed();

	case EFFECT_TYPE_HPSTORE:
		return new EffectHpStore();

	case EFFECT_TYPE_BOMB:
		return new EffectBomb();

	case EFFECT_TYPE_WUDI_PROTECT:
		return new EffectWudiProtect();

	case EFFECT_TYPE_MODIFY_GLOBAL_CD:
		return new EffectModifyGlobalCD();

	case EFFECT_TYPE_REBOUND_HURT:
		return new EffectReboundHurt();

	case EFFECT_TYPE_RECOVER_ON_ATKED:
		return new EffectRecoverOnAtked();

	case EFFECT_TYPE_GRADUAL_HURT:
		return new EffectGradualHurt();

	case EFFECT_TYPE_JUHUAPIG:
		return new EffectJuhuaPig();

	case EFFECT_TYPE_INVISIBLE:
		return new EffectInvisible();

	case EFFECT_TYPE_ATTR_RAND_INC_HURT:
		return new EffectAttrRandIncHurt();

	case EFFECT_TYPE_BIANSHEN:
		return new EffectBianshen();

	case EFFECT_TYPE_MP_NO_COST:
		return new EffectMPNoCost();

	case EFFECT_TYPE_SHENBING:
		return new EffectShenbing();

	case EFFECT_TYPE_ABSORB_HP:
		return new EffectAbsorbHP();

	case EFFECT_TYPE_ATTR_PER:
		return new EffectPercentBuff();

	case EFEECT_TYPE_OTHER:
		return new EffectOtherBuff();

	case EFFECT_TYPE_ATTR_CONTAINER:
		return new EffectAttrContainer();

	case EFFECT_TYPE_ATTR_STORE:
		return new EffectAttrStore();

	case EFFECT_TYPE_ABSORB_HP_BEEN_ATTACK:
		return new EffectAbsorbHPBeenAttack();

	case EFFECT_TYPE_TALENT_HPSTORE:
		return new EffectTalentHpStore();

	case EFFECT_TYPE_FORBID_RECOVER:
		return new EffectForbidRecover();
	}

	return NULL;
}

void EffectBase::GetEffectInfo(EffectInfo *effect_info)
{
	if (NULL != effect_info)
	{
		effect_info->effect_type = this->GetEffectType();
		effect_info->product_method = m_product_method;
		effect_info->product_id = m_product_id;
		effect_info->unique_key = m_unique_key;
		effect_info->merge_layer = m_merge_layer;
		effect_info->param_count = 0;
		effect_info->client_effect_type = m_client_effect_type;
		this->GetEffectParam(&effect_info->param_count, effect_info->param_list);
	}
}

bool EffectBase::SerializeBase(TLVSerializer &s) const
{
	return s.Push(m_deliverer) && s.Push(m_product_id) && s.Push(m_product_method) && s.Push(m_product_level) && s.Push(m_merge_rule) &&
			s.Push(m_merge_layer) && s.Push(m_max_merge_layer) && s.Push(m_client_effect_type) && s.Push(m_unique_key);
}

bool EffectBase::UnserializeBase(TLVUnserializer &s)
{
	return s.Pop(&m_deliverer) && s.Pop(&m_product_id) && s.Pop(&m_product_method) && s.Pop(&m_product_level) && s.Pop(&m_merge_rule) &&
			s.Pop(&m_merge_layer) && s.Pop(&m_max_merge_layer) && s.Pop(&m_client_effect_type) && s.Pop(&m_unique_key);
}

void EffectBase::HurtOthers(Character *cha, short target_count, ObjID *target_objid_list, long long injure, int fight_type)
{
	if (NULL != cha && NULL != cha->GetScene() && NULL != target_objid_list && target_count > 0)
	{
		SceneEffectHurt *effect_hurt = new SceneEffectHurt(cha->GetScene(), cha->GetId(), 0, EffectBase::PRODUCT_METHOD_SKILL);
		effect_hurt->SetHurtInfo(injure, fight_type);
		for (int i = 0; i < target_count; ++i)
		{
			effect_hurt->AddTarget(target_objid_list[i]);
		}

		cha->GetScene()->GetSpecialLogic()->AddSceneEffect(effect_hurt);
	}
}


