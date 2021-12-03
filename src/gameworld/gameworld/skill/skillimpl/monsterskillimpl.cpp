#include "monsterskillimpl.hpp"

#include "obj/character/character.h"

#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectinterval.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "effect/sceneeffect/sceneeffect.hpp"
#include "effect/skilleffect/effectwudiprotect.hpp"
#include "effect/skilleffect/effectmodifyglobalcd.hpp"
#include "effect/skilleffect/effectreboundhurt.hpp"
#include "effect/skilleffect/effectrecoveronatked.hpp"
#include "effect/skilleffect/effectgradualhurt.hpp"
#include "effect/skilleffect/effectjuhuapig.hpp"
#include "effect/skilleffect/effecthspecialshield.hpp"

#include "scene/scene.h"
#include "scene/speciallogic/speciallogic.hpp"

#include "obj/character/monsterhalo.hpp"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"

#include "checkresourcecenter.hpp"

#include "gamecommon.h"

bool SkillAttachEffectParam::CheckParamConfig(UInt16 skill_id, bool used_in_fazhen, bool is_range_skill, int effect_target, std::string &err)
{
	UNSTD_STATIC_CHECK(sizeof(effect_param_list) / sizeof(effect_param_list[0]) == 6);

	switch (effect_type)
	{
	case SKILL_ATTACH_EFFECT_CHENMO:
	case SKILL_ATTACH_EFFECT_XUANYUN:
		{
			if (effect_param_list[0] <= 0)
			{
				err = "TYPE[1/3] CHENMO/XUANYUN [param0] is dual_time, should > 0";
				return false;
			}
			
		}
		break;

	case SKILL_ATTACH_EFFECT_JITUI:
		{
			if (SKILL_ATTACH_EFFECT_TARGET_ROLE_PET != effect_target)
			{
				err = "TYPE[2] JITUI must on role";
				return false;
			}

			if (effect_param_list[0] <= 0)
			{
				err = "TYPE[2] JITUI [param0] is distance, should > 0";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_CHIHUAN:
		{
			if (effect_param_list[0] <= 0 || effect_param_list[0] >= 100)
			{
				err = "TYPE[4] [param1] is chihuan percent, should > 0";
				return false;
			}

			if (effect_param_list[1] <= 0)
			{
				err = "TYPE[4] [param1] is chihuan dual_time, should > 0";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_CALL_SAME_MONSTER:
		{
			if (effect_param_list[0] > 16)
			{
				err = "TYPE[5] [param0] is count, can't > 16";
				return false;
			}

			CheckResourceCenter::Instance().GetSkillCallMonsterCheck()->Add(skill_id, (int)effect_param_list[1]);
		}
		break;

	case SKILL_ATTACH_EFFECT_CALL_DIFF_MONSTER:
		{
			for (int i = 0; i < 4; ++i)
			{
				if (0 != effect_param_list[i])
				{
					CheckResourceCenter::Instance().GetSkillCallMonsterCheck()->Add(skill_id, (int)effect_param_list[i]);
				}				
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_INC_GONGJI:
	case SKILL_ATTACH_EFFECT_INC_FANGYU:
	case SKILL_ATTACH_EFFECT_INC_BAOJI:
	case SKILL_ATTACH_EFFECT_DEC_GONGJI:
	case SKILL_ATTACH_EFFECT_DEC_FANGYU:
		{
			if (effect_param_list[0] < 0)
			{
				err = "TYPE[7/8/9/10/11] [param0] is change gongji/fangyu/baoji/ percent, should >= 0";
				return false;
			}
			if (effect_param_list[1] < 0)
			{
				err = "TYPE[7/8/9/10/11] [param1] is change gongji/fangyu/baoji/ fix value, should >= 0";
				return false;
			}
			if (effect_param_list[2] <= 0)
			{
				err = "TYPE[7/8/9/10/11] [param2] is dual_time, should > 0";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_ADD_HP:
		{
			if (effect_param_list[0] < 0)
			{
				err = "TYPE[12] [param0] is add_hp percent, should >= 0";
				return false;
			}
			if (effect_param_list[1] < 0)
			{
				err = "TYPE[12] [param1] is add_hp fix value, should >= 0";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_DUR_ADD_HP:
		{
			if (effect_param_list[0] <= 0)
			{
				err = "TYPE[13] [param0] is dur_add_hp interval, should > 0";
				return false;
			}
			if (effect_param_list[1] < 0)
			{
				err = "TYPE[13] [param1] is dur_add_hp percent, should >= 0";
				return false;
			}
			if (effect_param_list[2] < 0)
			{
				err = "TYPE[13] [param2] is dur_add_hp fix value, should >= 0";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_WUDI_PROTECT:
		{
			if (SKILL_ATTACH_EFFECT_TARGET_BOSS != effect_target && 
				SKILL_ATTACH_EFFECT_TARGET_MONSTER != effect_target &&
				SKILL_ATTACH_EFFECT_TARGET_ALL_MONSTER != effect_target)
			{
				err = "TYPE[14] WUDI PROTECT should on monster";
				return false;
			}

			if (!used_in_fazhen)
			{
				err = "TYPE[14] WUDI PROTECT should used_in_fazhen";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_HURT_MAXHP_PERCENT:
		{
			/*if (is_range_skill)
			{
				err = "TYPE[15] only on single charactor";
				return false;
			}*/
			if (effect_param_list[0] < 0)
			{
				err = "TYPE[15] [param0] is percent_hurt percent, should >= 0";
				return false;
			}
			if (effect_param_list[1] < 0)
			{
				err = "TYPE[15] [param1] is percent_hurt fix value, should >= 0";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_KILL_SELF:
		{
			err = "TYPE[16] kill_self is no use";
			return false;
		}
		break;

	case SKILL_ATTACH_EFFECT_CHG_GLOBAL_CD:
		{
			if (0 == effect_param_list[0])
			{
				err = "TYPE[17] [param0] is percent to modify global cd, should != 0";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_HURT:
		{
			if (effect_param_list[0] <= 0)
			{
				err = "TYPE[18] [param0] is hurt, should > 0";
				return false;
			}
			if (SKILL_ATTACH_EFFECT_TARGET_ROLE_PET != effect_target && SKILL_ATTACH_EFFECT_TARGET_ENEMY != effect_target)
			{
				err = "TYPE[18] should on role or enemy";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_DUR_BLOOD_NOMERGE:
		{
			if (effect_param_list[0] <= 0)
			{
				err = "TYPE[19] [param0] is interval, should > 0";
				return false;
			}
			if (effect_param_list[1] <= 0)
			{
				err = "TYPE[19] [param1] is hurt, should > 0";
				return false;
			}
			if (effect_param_list[2] < 0 || effect_param_list[2] < effect_param_list[0])
			{
				err = "TYPE[19] [param2] is dual_time, should >= 0 && >= interval";
				return false;
			}
			if (SKILL_ATTACH_EFFECT_TARGET_ROLE_PET != effect_target && SKILL_ATTACH_EFFECT_TARGET_ENEMY != effect_target)
			{
				err = "TYPE[19] should on role or enemy";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_DUR_BLOOD_MERGE:
		{
			if (effect_param_list[0] <= 0)
			{
				err = "TYPE[20] [param0] is interval, should > 0";
				return false;
			}
			if (effect_param_list[1] <= 0)
			{
				err = "TYPE[20] [param1] is hurt, should > 0";
				return false;
			}
			if (effect_param_list[2] < 0 || effect_param_list[2] < effect_param_list[0])
			{
				err = "TYPE[20] [param2] is dual_time, should >= 0 && >= interval";
				return false;
			}
			if (effect_param_list[3] < 1)
			{
				err = "TYPE[20] [param3] max_layer > 1";
				return false;
			}
			if (SKILL_ATTACH_EFFECT_TARGET_ROLE_PET != effect_target)
			{
				err = "TYPE[20] should on role";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_DUR_GRADUAL_BLOOD:
		{
			if (effect_param_list[0] <= 0)
			{
				err = "TYPE[21] [param0] is init_hurt, should > 0";
				return false;
			}
			if (effect_param_list[1] < 0)
			{
				err = "TYPE[21] [param1] is percent_hurt percent, should >= 0";
				return false;
			}
			if (effect_param_list[2] < 0)
			{
				err = "TYPE[21] [param2] is fix value, should >= 0";
				return false;
			}
			if (effect_param_list[3] <= 0)
			{
				err = "TYPE[21] [param3] is interval value, should > 0";
				return false;
			}
			if (effect_param_list[4] <= 0)
			{
				err = "TYPE[21] [param4] is dual_time, should > 0";
				return false;
			}
			if (SKILL_ATTACH_EFFECT_TARGET_ROLE_PET != effect_target)
			{
				err = "TYPE[21] should on role";
				return false;
			}
			if (used_in_fazhen)
			{
				err = "TYPE[21] should not used in fazhen";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_FAZHEN_GRADUAL_BLOOD:
		{
			if (effect_param_list[0] <= 0)
			{
				err = "TYPE[22] [param0] is init_hurt, should > 0";
				return false;
			}
			if (effect_param_list[1] < 0)
			{
				err = "TYPE[22] [param1] is percent_hurt percent, should >= 0";
				return false;
			}
			if (effect_param_list[2] < 0)
			{
				err = "TYPE[22] [param2] is fix value, should >= 0";
				return false;
			}
			if (SKILL_ATTACH_EFFECT_TARGET_ROLE_PET != effect_target)
			{
				err = "TYPE[22] should on role";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_REDUCE_MP:
		{
			if (effect_param_list[0] <= 0)
			{
				err = "TYPE[23] [param0] is mp to reduce, should > 0";
				return false;
			}
			if (SKILL_ATTACH_EFFECT_TARGET_ROLE_PET != effect_target)
			{
				err = "TYPE[23] reduce_mp should on role";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_RECOVER_ON_ATKED:
		{
			if (effect_param_list[0] <= 0)
			{
				err = "TYPE[24] [param0] is percent, should > 0";
				return false;
			}
			if (effect_param_list[1] <= 0)
			{
				err = "TYPE[24] [param1] is dual_time, should > 0";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_REBOUND_HURT:
		{
			if (effect_param_list[0] <= 0)
			{
				err = "TYPE[25] [param0] is percent, should > 0";
				return false;
			}
			if (effect_param_list[1] <= 0)
			{
				err = "TYPE[25] [param1] is dual_time, should > 0";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_JUHUAPIG_REBOUND:
		{
			if (effect_param_list[0] <= 0)
			{
				err = "TYPE[26] [param0] is juhua injure_dealine, should > 0";
				return false;
			}
			if (effect_param_list[1] <= 0)
			{
				err = "TYPE[26] [param1] is juhua range, should > 0";
				return false;
			}
			if (effect_param_list[2] <= 0)
			{
				err = "TYPE[26] [param2] is juhua hurt percent, should > 0";
				return false;
			}
			if (effect_param_list[3] <= 0)
			{
				err = "TYPE[26] [param3] is juhua dual_time, should > 0";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_CLEAR_FIGHT_EFFECT:
		{
			if (effect_param_list[0] <= 0)
			{
				err = "TYPE[27] [param0] is clear count, should > 0";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_CLEAR_ALL_FIGHT_EFFECT:
		{
			// nothing to do
		}
		break;

	case SKILL_ATTACH_EFFECT_ADD_HP_ON_ATTACK_OTHER:
		{
			if (is_range_skill)
			{
				err = "TYPE[29] only on single charactor";
				return false;
			}
			if (effect_param_list[0] < 0)
			{
				err = "TYPE[29] [param0] is percent, should >= 0";
				return false;
			}
			if (effect_param_list[1] < 0)
			{
				err = "TYPE[29] [param1] is fix_hurt, should >= 0";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_BLAST:
		{
			if (effect_param_list[0] <= 0)
			{
				err = "TYPE[30] [param0] is injure, should > 0";
				return false;
			}
			if (!is_range_skill)
			{
				err = "TYPE[30] only on range";
				return false;
			}
			if (used_in_fazhen)
			{
				err = "TYPE[30] cannot used in fazhen";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_SPECIAL_LOGIC:
		{
			if (effect_param_list[0] <= SKILL_SPECIAL_LOGIC_TYPE_INVALID || effect_param_list[0] >= SKILL_SPECIAL_LOGIC_TYPE_MAX)
			{
				err = "TYPE[31] [param0] is skill_special_logic_type, error";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_SUPER_MIANYI:
		{
			if (effect_param_list[0] <= 0)
			{
				err = "TYPE[32] [param0] is mianyi dual_time, should > 0";
				return false;
			}

			if (used_in_fazhen)
			{
				err = "TYPE[32] cannot used in fazhen";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_DOUBLE_ATTR_BUFF:
		{
			if (CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI != effect_param_list[0] &&
				CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU != effect_param_list[0] &&
				CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI != effect_param_list[0])
			{
				err = "TYPE[33] [param0] sorry, gongji:6,fangyu:7,baoji:10";
				return false;
			}
						
			if (CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI != effect_param_list[2] &&
				CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU != effect_param_list[2] &&
				CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI != effect_param_list[2])
			{
				err = "TYPE[33] [param2] sorry, gongji:6,fangyu:7,baoji:10";
				return false;
			}
			
			if (effect_param_list[4] <= 0)
			{
				err = "TYPE[33] [param4] is dual_time, should > 0";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_ADD_HP:
		{
			if (effect_param_list[0] <= 0)
			{
				err = "TYPE[34] [param0] is percent_hurt percent, should > 0";
				return false;
			}
		}
		break;

	case SKILL_ATTACH_ADD_SPECIAL_SHIELD:
	{
		if (effect_param_list[0] <= 0)
		{
			err = "TYPE[35] [param0] is percent, should > 0";
			return false;
		}
		if (effect_param_list[1] <= 0)
		{
			err = "TYPE[35] [param1] is dual_time, should > 0";
			return false;
		}
		if (effect_param_list[1] <= 0)
		{
			err = "TYPE[35] [param2] is dual_time, should > 0";
			return false;
		}
	}
	break;

	}

	return true;
}

void SkillAttachEffectParam::Attach(Scene *scene, ObjID deliver_objid, const Posi &pos, short target_count, Character *target_list[SKILL_ATTACH_EFFECT_MAX_TARGET],
									UInt16 skill_id, int effect_src, int param) const
{
	UNSTD_STATIC_CHECK(sizeof(effect_param_list) / sizeof(effect_param_list[0]) == 6);

	bool is_buff_save = true;
	char product_method = EffectBase::PRODUCT_METHOD_SKILL;

	if (NULL != scene && !scene->InCommonScene())
	{
		product_method = EffectBase::PRODUCT_METHOD_SCENE_SKILL;
	}

	if (SKILL_EFFECT_SRC_FAZHEN == effect_src || SKILL_EFFECT_SRC_HALO == effect_src) 
	{
		is_buff_save = false;
		product_method = EffectBase::PRODUCT_METHOD_FAZHEN_HALO;
	}

	switch (effect_type)
	{
	case SKILL_ATTACH_EFFECT_XUANYUN:
	case SKILL_ATTACH_EFFECT_CHENMO:
		{
			unsigned char buff_type = EBT_INVALID;

			if (SKILL_ATTACH_EFFECT_CHENMO == effect_type) {
				buff_type = EBT_CHENMO;
			}
			else if (SKILL_ATTACH_EFFECT_XUANYUN == effect_type)
			{
				buff_type = EBT_XUANYUN;
				int yun_per = (int)effect_param_list[1];//概率眩晕
				if (yun_per > 0)
				{
					int rand_per = RandomNum(100);
					if (yun_per < rand_per)
					break;
				}
			}


			int dur_ms = (int)effect_param_list[0] * 1000;
			if (SKILL_EFFECT_SRC_FAZHEN == effect_src || SKILL_EFFECT_SRC_HALO == effect_src) dur_ms = FAZHEN_BUFF_DUR_MS;

			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					EffectSpecialState *special_effect = new EffectSpecialState(deliver_objid, skill_id, dur_ms, buff_type, 1, product_method);
					special_effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
					special_effect->SetSave(is_buff_save);
					target_list[i]->AddEffect(special_effect);
				}
			}			
		}
		break;

	case SKILL_ATTACH_EFFECT_JITUI:
		{
			if (NULL != scene)
			{
				int jitui_dis = (int)effect_param_list[0];

				for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
				{
					if (NULL != target_list[i] && target_list[i]->IsAlive())
					{
						if (target_list[i]->CanBeCaptureBySkill())
						{
							Posi deltaP = target_list[i]->GetPos() - pos;

							Scalar distance = 0;
							Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
							int x = int(target_list[i]->GetPos().x + jitui_dis * cos(dir));
							int y = int(target_list[i]->GetPos().y + jitui_dis * sin(dir));

							target_list[i]->SkillResetPos(Posi(x, y), Character::SKILL_RESET_POS_TYPE_JITUI, skill_id, false);
						}
					}
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_CHIHUAN:
		{
			int dec_percent = (int)effect_param_list[0];
			int dur_ms = (int)effect_param_list[1] * 1000;

			if (SKILL_EFFECT_SRC_FAZHEN == effect_src || SKILL_EFFECT_SRC_HALO == effect_src) dur_ms = FAZHEN_BUFF_DUR_MS;

			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					int dec_movespeed = static_cast<int>(target_list[i]->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * (dec_percent / 100.0f));

					EffectMoveSpeed *effect_speed = new EffectMoveSpeed(deliver_objid, skill_id, dur_ms, EBT_CHIHUAN, 1, product_method);
					effect_speed->SetMoveSpeed(-dec_movespeed, -dec_percent);
					effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
					effect_speed->SetClientEffectType(EffectBase::ECT_SKILL_DEC_SPEED);
					effect_speed->SetSave(is_buff_save);
					target_list[i]->AddEffect(effect_speed);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_CALL_SAME_MONSTER:
		{
			if (NULL != scene)
			{
				Obj *caller_obj = scene->GetObj(deliver_objid);
				if (NULL != caller_obj && Obj::OBJ_TYPE_MONSTER == caller_obj->GetObjType())
				{
					Monster *caller_monster = (Monster *)caller_obj;

					const int MAX_CALL_SAME_COUNT = 4;

					Posi callerpos = caller_monster->GetPos();
					Posi birth_pos_list[] = {Posi(callerpos.x + 9, callerpos.y - 9), Posi(callerpos.x + 9, callerpos.y + 9),
											 Posi(callerpos.x - 9, callerpos.y + 9), Posi(callerpos.x - 9, callerpos.y - 9)};

					int monster_count = (int)effect_param_list[0];
					int monster_id = (int)effect_param_list[1];

					bool live_on_father = (1 == (int)effect_param_list[3]);

					UNSTD_STATIC_CHECK(MAX_CALL_SAME_COUNT == (int)(sizeof(birth_pos_list) / sizeof(birth_pos_list[0])));
					int left_pos_count = MAX_CALL_SAME_COUNT;

					for (int i = 0; i < monster_count && i < MAX_CALL_SAME_COUNT; ++i)
					{
						Posi pos;

						if (left_pos_count > 0 && left_pos_count <= MAX_CALL_SAME_COUNT)
						{
							int rand_val = RandomNum(left_pos_count);
							pos = birth_pos_list[rand_val];
							birth_pos_list[rand_val] = birth_pos_list[left_pos_count - 1];
							-- left_pos_count;
						}
						else
						{
							pos = birth_pos_list[i];
						}

						Monster *child = MONSTERPOOL->CreateMonster(monster_id, scene, pos);
						if (NULL != child && live_on_father)
						{
							child->SetOwnerMonster(deliver_objid, caller_monster->GetMonsterKey());
						}
					}
				}				
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_CALL_DIFF_MONSTER:
		{
			if (NULL != scene)
			{
				Obj *caller_obj = scene->GetObj(deliver_objid);
				if (NULL != caller_obj && Obj::OBJ_TYPE_MONSTER == caller_obj->GetObjType())
				{
					Monster *caller_monster = (Monster *)caller_obj;

					const int MAX_CALL_DIFF_COUNT = 3;

					Posi callerpos = caller_monster->GetPos();
					Posi birth_pos_list[] = {Posi(callerpos.x + 9, callerpos.y - 9), Posi(callerpos.x + 9, callerpos.y + 9),
											 Posi(callerpos.x - 9, callerpos.y + 9)};

					bool live_on_father = (1 == effect_param_list[3]);

					UNSTD_STATIC_CHECK(MAX_CALL_DIFF_COUNT == (int)(sizeof(birth_pos_list) / sizeof(birth_pos_list[0])));
					int left_pos_count = MAX_CALL_DIFF_COUNT;

					for (int i = 0; i < MAX_CALL_DIFF_COUNT; ++i)
					{
						int monster_id = (int)effect_param_list[i];
						if (monster_id > 0)
						{
							Posi pos;

							if (left_pos_count > 0 && left_pos_count <= MAX_CALL_DIFF_COUNT)
							{
								int rand_val = RandomNum(left_pos_count);
								pos = birth_pos_list[rand_val];
								birth_pos_list[rand_val] = birth_pos_list[left_pos_count - 1];
								-- left_pos_count;
							}
							else
							{
								pos = birth_pos_list[i];
							}

							Monster *child = MONSTERPOOL->CreateMonster(monster_id, scene, pos);
							if (NULL != child && live_on_father)
							{
								child->SetOwnerMonster(deliver_objid, caller_monster->GetMonsterKey());
							}
						}
					}
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_INC_GONGJI:
	case SKILL_ATTACH_EFFECT_INC_FANGYU:
	case SKILL_ATTACH_EFFECT_INC_BAOJI:
		{
			int dur_ms = (int)effect_param_list[2] * 1000;
			if (SKILL_EFFECT_SRC_FAZHEN == effect_src || SKILL_EFFECT_SRC_HALO == effect_src) dur_ms = FAZHEN_BUFF_DUR_MS;

			int attr_type = 0; Attribute chg_val = 0;

			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					if (SKILL_ATTACH_EFFECT_INC_GONGJI == effect_type) 
					{
						attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI;
						chg_val = static_cast<Attribute>(target_list[i]->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (effect_param_list[0] / 100.0) + effect_param_list[1]);
					}
					else if (SKILL_ATTACH_EFFECT_INC_FANGYU == effect_type)
					{
						attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU;
						chg_val = static_cast<Attribute>(target_list[i]->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * (effect_param_list[0] / 100.0) + effect_param_list[1]);
					}
					else 
					{
						attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI;
						chg_val = static_cast<Attribute>(target_list[i]->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI) * (effect_param_list[0] / 100.0) + effect_param_list[1]);
					}

					EffectBuff *effect_buff = new EffectBuff(deliver_objid, skill_id, dur_ms, 1, product_method);
					effect_buff->AddAttrEffect(attr_type, chg_val, (int)effect_param_list[0]);
					effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
					effect_buff->SetSave(is_buff_save);
					target_list[i]->AddEffect(effect_buff);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_DEC_GONGJI:
	case SKILL_ATTACH_EFFECT_DEC_FANGYU:
		{
			int dur_ms = (int)effect_param_list[2] * 1000;
			if (SKILL_EFFECT_SRC_FAZHEN == effect_src || SKILL_EFFECT_SRC_HALO == effect_src) dur_ms = FAZHEN_BUFF_DUR_MS;

			int attr_type = 0; Attribute chg_val = 0;

			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					if (SKILL_ATTACH_EFFECT_DEC_GONGJI == effect_type) 
					{
						attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI;
						chg_val = static_cast<Attribute>(target_list[i]->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (effect_param_list[0] / 100.0) + effect_param_list[1]);
					}
					else if (SKILL_ATTACH_EFFECT_DEC_FANGYU == effect_type)
					{
						attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU;
						chg_val = static_cast<Attribute>(target_list[i]->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * (effect_param_list[0] / 100.0) + effect_param_list[1]);
					}

					EffectBuff *effect_buff = new EffectBuff(deliver_objid, skill_id, dur_ms, 1, product_method);
					effect_buff->AddAttrEffect(attr_type, -chg_val, -1 * (int)effect_param_list[0]);
					effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
					effect_buff->SetSave(is_buff_save);
					target_list[i]->AddEffect(effect_buff);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_ADD_HP:
		{
			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && 
					target_list[i]->IsAlive() &&
					target_list[i]->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP) < target_list[i]->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP))
				{
					Attribute add_hp = static_cast<Attribute>(target_list[i]->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (effect_param_list[0] / 100.0) + effect_param_list[1]);
				
					EffectOnce *effect_addhp = new EffectOnce(deliver_objid, skill_id, add_hp, EffectBase::FIGHT_TYPE_NORMAL);
					target_list[i]->AddEffect(effect_addhp);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_DUR_ADD_HP:
		{
			int interval_ms = (int)effect_param_list[0] * 1000;
			if (interval_ms <= 0) interval_ms = 1000;

			int dur_ms = (int)effect_param_list[3] * 1000;

			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					Attribute add_hp = static_cast<Attribute>(target_list[i]->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP) * (effect_param_list[1] / 100.0) + effect_param_list[2]);

					EffectInterval *effect_interval = new EffectInterval(deliver_objid, skill_id, EBT_ADD_BLOOD, 1, product_method);
					effect_interval->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
					effect_interval->SetParam(interval_ms, dur_ms / interval_ms, add_hp);
					effect_interval->SetSave(is_buff_save);
					target_list[i]->AddEffect(effect_interval);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_WUDI_PROTECT:
		{
			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					EffectWudiProtect *effect_wudi = new EffectWudiProtect(deliver_objid, skill_id, FAZHEN_BUFF_DUR_MS, 1);
					effect_wudi->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
					target_list[i]->AddEffect(effect_wudi);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_HURT_MAXHP_PERCENT:
		{
			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					long long injure = static_cast<long long>(target_list[i]->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (effect_param_list[0] / 100.0) + effect_param_list[1]);

					EffectOnce *effect_addhp = new EffectOnce(deliver_objid, skill_id, -injure, EffectBase::FIGHT_TYPE_NORMAL);
					target_list[i]->AddEffect(effect_addhp);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_KILL_SELF:
		{
			/* 这个功能取消了
			if (NULL != scene && SKILL_EFFECT_SRC_SKILL == effect_src)
			{
				Obj *obj = scene->GetObj(deliver_objid);
				if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
				{
					Monster *monster = (Monster *)obj;
					monster->ForceSetDead();
				}
			}*/
		}
		break;

	case SKILL_ATTACH_EFFECT_CHG_GLOBAL_CD:
		{
			int dur_ms = (int)effect_param_list[1] * 1000;
			if (SKILL_EFFECT_SRC_FAZHEN == effect_src || SKILL_EFFECT_SRC_HALO == effect_src) dur_ms = FAZHEN_BUFF_DUR_MS;

			int percent = (int)effect_param_list[0];

			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					EffectModifyGlobalCD *effect_modify_gcd = new EffectModifyGlobalCD(deliver_objid, skill_id, dur_ms, percent, 1, product_method);
					effect_modify_gcd->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
					target_list[i]->AddEffect(effect_modify_gcd);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_HURT:
		{
			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
					long long injure = effect_param_list[0];

					EffectOnce *effect = new EffectOnce(deliver_objid, skill_id, -injure, fight_type);
					target_list[i]->AddEffect(effect);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_DUR_BLOOD_NOMERGE:
		{
			int interval_ms = (int)effect_param_list[0] * 1000;
			if (interval_ms <= 0) interval_ms = 1000;

			long long injure = effect_param_list[1];
			int dur_ms = (int)effect_param_list[2] * 1000;

			int gongji_percent = (int)effect_param_list[3];

			if (NULL != scene)
			{
				Obj *deliver_obj = scene->GetObj(deliver_objid);
				if (NULL != deliver_obj && Obj::OBJ_TYPE_MONSTER == deliver_obj->GetObjType()) 
				{
					Monster *deliver_monster = (Monster *)deliver_obj;

					long long gongji_injure = static_cast<long long>(deliver_monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (gongji_percent / 100.0));
					injure += gongji_injure;

					for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
					{
						if (NULL != target_list[i] && target_list[i]->IsAlive())
						{
							EffectInterval *effect_interval = new EffectInterval(deliver_objid, skill_id, EBT_DROP_BLOOD, 1, product_method);
							effect_interval->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
							effect_interval->SetParam(interval_ms, dur_ms / interval_ms, -injure);
							effect_interval->SetSave(is_buff_save);
							target_list[i]->AddEffect(effect_interval);
						}
					}
				}	
			}			
		}
		break;

	case SKILL_ATTACH_EFFECT_DUR_BLOOD_MERGE:
		{
			int interval_ms = (int)effect_param_list[0] * 1000;
			if (interval_ms <= 0) interval_ms = 1000;

			long long injure = effect_param_list[1];
			int dur_ms = (int)effect_param_list[2] * 1000;
			int max_layer = (int)effect_param_list[3];

			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					EffectInterval *effect_interval = new EffectInterval(deliver_objid, skill_id, EBT_DROP_BLOOD, 1, product_method);
					effect_interval->SetMergeRule(EffectBase::MERGE_RULE_MERGE);
					effect_interval->SetMaxMergeLayer(max_layer);
					effect_interval->SetParam(interval_ms, dur_ms / interval_ms, -injure);
					effect_interval->SetSave(is_buff_save);
					target_list[i]->AddEffect(effect_interval);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_DUR_GRADUAL_BLOOD:
		{
			int interval_ms = (int)effect_param_list[3] * 1000;
			if (interval_ms <= 0) interval_ms = 1000;

			long long init_injure = effect_param_list[0];
			int dur_ms = (int)effect_param_list[4] * 1000;

			int percent = (int)effect_param_list[1];
			long long fix_hurt = effect_param_list[2];

			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					EffectGradualHurt *effect_gradual_hurt = new EffectGradualHurt(deliver_objid, skill_id, 1, product_method);
					effect_gradual_hurt->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
					effect_gradual_hurt->SetParam(interval_ms, dur_ms / interval_ms, init_injure, percent, fix_hurt);
					effect_gradual_hurt->SetSave(is_buff_save);
					target_list[i]->AddEffect(effect_gradual_hurt);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_FAZHEN_GRADUAL_BLOOD:
		{
			int jump_count = param;
			if (jump_count < 1) jump_count = 1;

			long long init_hurt = effect_param_list[0];
			int hurt_percent = (int)effect_param_list[1];
			long long fix_hurt = effect_param_list[2];

			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					long long injure = static_cast<long long>(init_hurt * (1 + (jump_count - 1) * (hurt_percent / 100.0)) + (jump_count - 1) * fix_hurt);
					EffectOnce *effect_addhp = new EffectOnce(deliver_objid, skill_id, -injure, EffectBase::FIGHT_TYPE_NORMAL);
					target_list[i]->AddEffect(effect_addhp);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_REDUCE_MP:
		{
			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				Attribute change_mp = -effect_param_list[0];

				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					target_list[i]->ChangeMp(&change_mp, true);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_RECOVER_ON_ATKED:
		{
			int dur_ms = (int)effect_param_list[1] * 1000;
			if (SKILL_EFFECT_SRC_FAZHEN == effect_src || SKILL_EFFECT_SRC_HALO == effect_src) dur_ms = FAZHEN_BUFF_DUR_MS;

			int percent = (int)effect_param_list[0];

			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					EffectRecoverOnAtked *effect_recover = new EffectRecoverOnAtked(deliver_objid, skill_id, dur_ms, percent, 1, product_method);
					effect_recover->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
					target_list[i]->AddEffect(effect_recover);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_REBOUND_HURT:
		{
			int dur_ms = (int)effect_param_list[1] * 1000;
			if (SKILL_EFFECT_SRC_FAZHEN == effect_src || SKILL_EFFECT_SRC_HALO == effect_src) dur_ms = FAZHEN_BUFF_DUR_MS;

			int percent = (int)effect_param_list[0];

			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					EffectReboundHurt *effect_rebound = new EffectReboundHurt(deliver_objid, skill_id, dur_ms, percent, 1, product_method);
					effect_rebound->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
					target_list[i]->AddEffect(effect_rebound);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_JUHUAPIG_REBOUND:
		{
			if (NULL != scene)
			{
				Obj *obj = scene->GetObj(deliver_objid);
				if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
				{
					long long injure_deadline = effect_param_list[0];
					int bomb_range = (int)effect_param_list[1];
					int percent = (int)effect_param_list[2];
					int dur_ms = (int)effect_param_list[3] * 1000;
					
					for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
					{
						if (NULL != target_list[i] && target_list[i]->IsAlive())
						{
							EffectJuhuaPig *effect_juhuapig = new EffectJuhuaPig(deliver_objid, skill_id, 1);
							effect_juhuapig->SetMergeRule(EffectBase::MERGE_RULE_IGNORE);

							Attribute init_hp = target_list[i]->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
							effect_juhuapig->SetParam(dur_ms, injure_deadline, percent, bomb_range, init_hp);
								
							target_list[i]->AddEffect(effect_juhuapig);
						}
					}
				}
			}			
		}
		break;

	case SKILL_ATTACH_EFFECT_CLEAR_FIGHT_EFFECT:
		{
			int effect_count_to_remove = (int)effect_param_list[0];

			for (int i = 0; i < effect_count_to_remove && i < 4; ++i)
			{
				for (int j = 0; j < target_count && j < SKILL_ATTACH_EFFECT_MAX_TARGET; ++j)
				{
					if (NULL != target_list[j] && target_list[j]->IsAlive())
					{
						target_list[j]->GetEffectList()->RandRemoveFightEffect();
					}
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_CLEAR_ALL_FIGHT_EFFECT:
		{
			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					target_list[i]->GetEffectList()->ClearFightEffect(EffectBase::CLEAR_REASON_SKILL);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_ADD_HP_ON_ATTACK_OTHER:
		{
			int hurt_percent = (int)effect_param_list[0];
			long long fix_hurt = effect_param_list[1];

			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					Attribute add_hp = static_cast<Attribute>(target_list[i]->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (hurt_percent / 100.0) + fix_hurt);
					EffectOnce *effect_addhp = new EffectOnce(deliver_objid, skill_id, add_hp, EffectBase::FIGHT_TYPE_NORMAL);
					target_list[i]->AddEffect(effect_addhp);
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_BLAST:
		{
			long long injure = effect_param_list[0];

			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					EffectOnce *effect = new EffectOnce(deliver_objid, skill_id, -injure, EffectBase::FIGHT_TYPE_NORMAL);
					target_list[i]->AddEffect(effect);
				}
			}

			if (NULL != scene && SKILL_EFFECT_SRC_SKILL == effect_src)
			{
				Obj *obj = scene->GetObj(deliver_objid);
				if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
				{
					Monster *monster = (Monster *)obj;
					monster->ForceSetDead();
				}
			}
		}
		break;

	case SKILL_ATTACH_EFFECT_SPECIAL_LOGIC:
		{
			if (NULL != scene && NULL != scene->GetSpecialLogic())
			{
				Obj *obj = scene->GetObj(deliver_objid);
				if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
				{
					Monster *monster = (Monster *)obj;
					
					int special_type = (int)effect_param_list[0];
					scene->GetSpecialLogic()->OnSkillEffect(monster, special_type, (int)effect_param_list[1], (int)effect_param_list[2]);
				}
			}
		}
		break;

	case SKILL_ATTACH_SUPER_MIANYI:
		{
			int dur_ms = (int)effect_param_list[0] * 1000;

			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					EffectSpecialState *effect_buff = new EffectSpecialState(deliver_objid, skill_id, dur_ms, EBT_SUPER_MIANYI, 1, product_method);
					effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
					effect_buff->SetSave(is_buff_save);
					target_list[i]->AddEffect(effect_buff);
				}
			}
		}
		break;

	case SKILL_ATTACH_DOUBLE_ATTR_BUFF:
		{
			int dur_ms = (int)effect_param_list[4] * 1000;
			if (SKILL_EFFECT_SRC_FAZHEN == effect_src || SKILL_EFFECT_SRC_HALO == effect_src) dur_ms = FAZHEN_BUFF_DUR_MS;

			int attr_type1 = 0; Attribute chg_val1 = 0;
			int attr_type2 = 0; Attribute chg_val2 = 0;

			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					if (CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI == effect_param_list[0]) 
					{
						attr_type1 = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI;
						chg_val1 = static_cast<Attribute>(target_list[i]->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (effect_param_list[1] / 100.0));
					}
					else if (CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU == effect_param_list[0])
					{
						attr_type1 = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU;
						chg_val1 = static_cast<Attribute>(target_list[i]->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * (effect_param_list[1] / 100.0));
					}
					else 
					{
						attr_type1 = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI;
						chg_val1 = static_cast<Attribute>(target_list[i]->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI) * (effect_param_list[1] / 100.0));
					}

					if (CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI == effect_param_list[2]) 
					{
						attr_type2 = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI;
						chg_val2 = static_cast<Attribute>(target_list[i]->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (effect_param_list[3] / 100.0));
					}
					else if (CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU == effect_param_list[2])
					{
						attr_type2 = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU;
						chg_val2 = static_cast<Attribute>(target_list[i]->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * (effect_param_list[3] / 100.0));
					}
					else 
					{
						attr_type2 = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI;
						chg_val2 = static_cast<Attribute>(target_list[i]->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI) * (effect_param_list[3] / 100.0));
					}

					EffectBuff *effect_buff = new EffectBuff(deliver_objid, skill_id, dur_ms, 1, product_method);
					effect_buff->AddAttrEffect(attr_type1, chg_val1, (int)effect_param_list[1]);
					effect_buff->AddAttrEffect(attr_type2, chg_val2, (int)effect_param_list[3]);
					effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
					effect_buff->SetSave(is_buff_save);
					target_list[i]->AddEffect(effect_buff);
				}
			}
		}
		break;

	case SKILL_ATTACH_ADD_HP:
		{
			for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
			{
				if (NULL != target_list[i] && target_list[i]->IsAlive())
				{
					Attribute add_hp = static_cast<Attribute>(target_list[i]->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (effect_param_list[0] / 100.0));

					if (add_hp > 0)
					{
						EffectInterval *effect_interval = new EffectInterval(deliver_objid, 1, EBT_ADD_BLOOD, 1);
						effect_interval->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
						effect_interval->SetParam(500, 1, add_hp);
						effect_interval->SetSave(false);
						target_list[i]->AddEffect(effect_interval);
					}
				}
			}
		}
		break;

	case SKILL_ATTACH_ADD_SPECIAL_SHIELD:
	{
		for (int i = 0; i < target_count && i < SKILL_ATTACH_EFFECT_MAX_TARGET; ++i)
		{
			if (NULL != target_list[i] && target_list[i]->IsAlive())
			{
				EffectSpecialShield *special_shield = new EffectSpecialShield(target_list[i]->GetId(), 1, (int)effect_param_list[1], (int)effect_param_list[0], 1);
				special_shield->SetFixHurt(-effect_param_list[2]);
				target_list[i]->AddEffect(special_shield);
			}
		}
	}
	break;

	}
}


// 单体对敌人
// ------------------------------------------------------------------------------------------------------------
void MonsterCommonSkillToEnemy::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == target) return;

	if (SKILL_PHASE_READING == special_param || SKILL_PHASE_CONTINUE_PERFORM == special_param || SKILL_PHASE_END == special_param)
	{
		SendSkillPhase(chara, m_skill_id, special_param);
	}
	else
	{
		SendAttackMsg(chara, target);

		// 先判断是否闪避
		if (CheckShanbi() && IsShanbi(chara, target)) return;

		if (GetHurtPercent(1) > 0 || GetFixHurt(1) > 0)		// 技能可能无伤，节省一次公式
		{
			int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
			long long injure = CalcInjureAndType(chara, target, level, fight_type);

			EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
			target->AddEffect(effect);
		}

		int target_count = 0;
		Character *target_list[SKILL_ATTACH_EFFECT_MAX_TARGET] = {NULL};

		target_list[target_count++] = target;

		for (int i = 0; i < m_effect_count; i++) 
		{
			m_effect_list[i].Attach(chara->GetScene(), chara->GetId(), chara->GetPos(), target_count, target_list, m_skill_id);
		}
	}
}
// ------------------------------------------------------------------------------------------------------------

// 群体对敌人坐标
// ------------------------------------------------------------------------------------------------------------
void MonsterRangeCommonSkillToEnemyPos::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (SKILL_PHASE_READING == special_param || SKILL_PHASE_CONTINUE_PERFORM == special_param || SKILL_PHASE_END == special_param)
	{
		SendSkillPhase(chara, m_skill_id, special_param);
	}
	else
	{
		SendAOEAttackMsg(chara, target, pos);

		Scene *scene = chara->GetScene();

		static ObjID target_objid_list[SKILL_ATTACH_EFFECT_MAX_TARGET] = {INVALID_OBJ_ID};
		int range = this->GetAttackRange(level);
		int range2 = this->GetAttackRange2(level);
		int obj_count = SkillBase::GetTargetList(scene, chara, pos, range, range2, target_objid_list, SKILL_ATTACH_EFFECT_MAX_TARGET, m_skill_range_type, m_effect_target);

		Character *target_cha_list[SKILL_ATTACH_EFFECT_MAX_TARGET] = {NULL};
		int target_count = 0;
		int max_count = static_cast<int>(sizeof(target_cha_list) / sizeof(target_cha_list[0]));

		for (int i = 0; i < obj_count && target_count < max_count; ++i)
		{
			Character *target = (Character*)scene->GetObj(target_objid_list[i]);
			if (NULL == target) continue;

			// 先判断是否闪避
			if (CheckShanbi() && IsShanbi(chara, target)) continue;					// 分别判断每个目标闪避，如果闪避了 不掉血，效果不起作用

			target_cha_list[target_count ++] = target;

			if (GetHurtPercent(1) > 0 || GetFixHurt(1) > 0) 								// 技能可能无伤，节省一次公式
			{
				int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
				long long injure = CalcInjureAndType(chara, target, level, fight_type);

				EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
				target->AddEffect(effect);
			}
		}

		for (int i = 0; i < m_effect_count; i++) 
		{
			m_effect_list[i].Attach(chara->GetScene(), chara->GetId(), chara->GetPos(), target_count, target_cha_list, m_skill_id);
		}
	}
}
// ------------------------------------------------------------------------------------------------------------

// 单体对自己
// ------------------------------------------------------------------------------------------------------------
void MonsterCommonSkillToSelf::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (SKILL_PHASE_READING == special_param || SKILL_PHASE_CONTINUE_PERFORM == special_param || SKILL_PHASE_END == special_param)
	{
		SendSkillPhase(chara, m_skill_id, special_param);
	}
	else
	{
		SendAttackMsg(chara, chara);

		int target_count = 0;
		Character *target_list[SKILL_ATTACH_EFFECT_MAX_TARGET] = {NULL};

		target_list[target_count++] = chara;

		for (int i = 0; i < m_effect_count; i++)
		{
			m_effect_list[i].Attach(chara->GetScene(), chara->GetId(), chara->GetPos(), target_count, target_list, m_skill_id);
		}
	}
}
// ------------------------------------------------------------------------------------------------------------

// 群体对自己坐标
// ------------------------------------------------------------------------------------------------------------
void MonsterRangeCommonSkillToSelfPos::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (SKILL_PHASE_READING == special_param || SKILL_PHASE_CONTINUE_PERFORM == special_param || SKILL_PHASE_END == special_param)
	{
		SendSkillPhase(chara, m_skill_id, special_param);
	}
	else
	{
		SendAOEAttackMsg(chara, target, pos);

		Scene *scene = chara->GetScene();

		static ObjID target_objid_list[SKILL_ATTACH_EFFECT_MAX_TARGET] = {INVALID_OBJ_ID};
		int range = this->GetAttackRange(level);
		int range2 = this->GetAttackRange2(level);
		int obj_count = SkillBase::GetTargetList(scene, chara, pos, range, range2, target_objid_list, SKILL_ATTACH_EFFECT_MAX_TARGET, m_skill_range_type, m_effect_target);

		Character *target_cha_list[SKILL_ATTACH_EFFECT_MAX_TARGET] = {NULL};
		int target_count = 0;
		int max_count = static_cast<int>(sizeof(target_cha_list) / sizeof(target_cha_list[0]));

		for (int i = 0; i < obj_count && target_count < max_count; ++i)
		{
			Character *target = (Character*)scene->GetObj(target_objid_list[i]);
			if (NULL == target) continue;

			// 先判断是否闪避
			if (CheckShanbi() && IsShanbi(chara, target)) continue;

			if (GetHurtPercent(1) > 0 || GetFixHurt(1) > 0) 								// 技能可能无伤，节省一次公式
			{
				int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
				long long injure = CalcInjureAndType(chara, target, level, fight_type);

				EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
				target->AddEffect(effect);
			}

			target_cha_list[target_count ++] = target;
		}

		for (int i = 0; i < m_effect_count; i++)
		{
			m_effect_list[i].Attach(chara->GetScene(), chara->GetId(), chara->GetPos(), target_count, target_cha_list, m_skill_id);
		}
	}
}
// ------------------------------------------------------------------------------------------------------------

// 法阵技 对敌人坐标
// ------------------------------------------------------------------------------------------------------------
void MonsterFaZhenSkillToEnemyPos::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (SKILL_PHASE_READING == special_param || SKILL_PHASE_CONTINUE_PERFORM == special_param || SKILL_PHASE_END == special_param)
	{
		SendSkillPhase(chara, m_skill_id, special_param);
	}
	else
	{
		if (SKILL_FAZHEN_EFFECT_TYPE_FAZHEN == m_fazhen_type && m_fazhen_lifetime > 0 && m_fazhen_interval > 0 && m_effect_count > 0)
		{
			SendAOEAttackMsg(chara, target, pos);

			SceneEffectSpecialFaZhen *scene_effect = new SceneEffectSpecialFaZhen(chara->GetScene(), chara->GetId(), m_skill_id, EffectBase::PRODUCT_METHOD_SKILL);
			scene_effect->SetFaZhenInfo(pos, this->GetAttackRange(level), m_fazhen_interval * 1000, m_fazhen_lifetime / m_fazhen_interval, GetContinuePerformS() > 0, m_effect_now);
			scene_effect->SetEffectParam(m_effect_target, m_effect_count, &m_effect_list[0]);

			chara->GetScene()->GetSpecialLogic()->AddSceneEffect(scene_effect);
		}
	}
}
// ------------------------------------------------------------------------------------------------------------

// 法阵技 对自己坐标
// ------------------------------------------------------------------------------------------------------------
void MonsterFaZhenSkillToSelfPos::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (SKILL_PHASE_READING == special_param || SKILL_PHASE_CONTINUE_PERFORM == special_param || SKILL_PHASE_END == special_param)
	{
		SendSkillPhase(chara, m_skill_id, special_param);
	}
	else
	{
		if (SKILL_FAZHEN_EFFECT_TYPE_FAZHEN == m_fazhen_type && m_fazhen_lifetime > 0 && m_fazhen_interval > 0 && m_effect_count > 0)
		{
			SendAOEAttackMsg(chara, target, pos);

			SceneEffectSpecialFaZhen *scene_effect = new SceneEffectSpecialFaZhen(chara->GetScene(), chara->GetId(), m_skill_id, EffectBase::PRODUCT_METHOD_SKILL);
			scene_effect->SetFaZhenInfo(pos, this->GetAttackRange(level), m_fazhen_interval * 1000, m_fazhen_lifetime / m_fazhen_interval, GetContinuePerformS() > 0, m_effect_now);
			scene_effect->SetEffectParam(m_effect_target, m_effect_count, &m_effect_list[0]);

			chara->GetScene()->GetSpecialLogic()->AddSceneEffect(scene_effect);
		}
		else if (SKILL_FAZHEN_EFFECT_TYPE_HALO == m_fazhen_type && m_fazhen_lifetime > 0 && m_fazhen_interval > 0 && m_effect_count > 0)
		{
			if (Obj::OBJ_TYPE_MONSTER == chara->GetObjType())
			{
				SendAOEAttackMsg(chara, target, pos);

				Monster *monster = (Monster *)chara;

				MonsterHalo *halo = new MonsterHalo(chara->GetId(), monster->GetMonsterKey(), m_skill_id, EffectBase::PRODUCT_METHOD_SKILL);
				halo->Init(1);
				halo->SetHaloInfo(this->GetAttackRange(level), m_fazhen_interval * 1000, m_fazhen_lifetime / m_fazhen_interval);
				halo->SetEffectParam(m_effect_target, m_effect_list[0]);

				chara->GetScene()->AddObj(halo);
			}			
		}
	}
}
// ------------------------------------------------------------------------------------------------------------

// 
// ------------------------------------------------------------------------------------------------------------
void MonsterSkillToEnemyEffectToOther::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (NULL == target) return;

	if (SKILL_PHASE_READING == special_param || SKILL_PHASE_CONTINUE_PERFORM == special_param || SKILL_PHASE_END == special_param)
	{
		SendSkillPhase(chara, m_skill_id, special_param);
	}
	else
	{
		SendAttackMsg(chara, target);

		// 先判断是否闪避
		if (CheckShanbi() && IsShanbi(chara, target)) return;

		{
			// 对技能目标伤害
			if (GetHurtPercent(1) > 0 || GetFixHurt(1) > 0)		// 技能可能无伤，节省一次公式
			{
				int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
				long long injure = CalcInjureAndType(chara, target, level, fight_type);

				EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
				target->AddEffect(effect);
			}
		}

		{
			// 对效果目标生效
			Scene *scene = chara->GetScene();

			static ObjID target_objid_list[SKILL_ATTACH_EFFECT_MAX_TARGET] = {INVALID_OBJ_ID};
			int range = this->GetAttackRange(level);
			int range2 = this->GetAttackRange2(level);
			int obj_count = SkillBase::GetTargetList(scene, chara, pos, range, range2, target_objid_list, SKILL_ATTACH_EFFECT_MAX_TARGET, m_skill_range_type, m_effect_target);

			Character *target_cha_list[SKILL_ATTACH_EFFECT_MAX_TARGET] = {NULL};
			int target_count = 0;
			int max_count = static_cast<int>(sizeof(target_cha_list) / sizeof(target_cha_list[0]));

			for (int i = 0; i < obj_count && target_count < max_count; ++i)
			{
				Character *target = (Character*)scene->GetObj(target_objid_list[i]);
				if (NULL == target) continue;

				target_cha_list[target_count ++] = target;
			}

			for (int i = 0; i < m_effect_count; i++) 
			{
				m_effect_list[i].Attach(chara->GetScene(), chara->GetId(), chara->GetPos(), target_count, target_cha_list, m_skill_id);
			}
		}
	}
}
// ------------------------------------------------------------------------------------------------------------

// 矩形群攻
// ------------------------------------------------------------------------------------------------------------
void MonsterRectRangeCommonSkillToEnemyPos::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (SKILL_PHASE_READING == special_param || SKILL_PHASE_CONTINUE_PERFORM == special_param || SKILL_PHASE_END == special_param)
	{
		SendSkillPhase(chara, m_skill_id, special_param);
	}
	else
	{
		SendAOEAttackMsg(chara, target, pos);

		const float PI = 3.1416f;

		float dir_list[4 + 1];						// 1-4象限
		memset(dir_list, 0, sizeof(dir_list));

		int rand_val = RandomNum(1, 91);
		float arc = rand_val * PI / 180;

		dir_list[1] = arc;

		if (DIR_TYPE_DOUBLE == m_dir_type)
		{
			dir_list[3] = arc + PI;
		}
		else if (DIR_TYPE_CROSS == m_dir_type)
		{
			dir_list[2] = arc + PI / 2; 
			dir_list[3] = arc + PI;
			dir_list[4] = arc + PI * 3 / 2;
		}

		Scene *scene = chara->GetScene();

		static ObjID target_objid_list[SKILL_ATTACH_EFFECT_MAX_TARGET] = {INVALID_OBJ_ID};
		int range = this->GetAttackRange(level);
		int range2 = this->GetAttackRange2(level);
		int obj_count = SkillBase::GetTargetList(scene, chara, pos, range, range2, target_objid_list, SKILL_ATTACH_EFFECT_MAX_TARGET, m_skill_range_type, m_effect_target);

		Character *target_cha_list[SKILL_ATTACH_EFFECT_MAX_TARGET] = {NULL};
		int target_count = 0;
		int max_count = static_cast<int>(sizeof(target_cha_list) / sizeof(target_cha_list[0]));

		for (int i = 0; i < obj_count && target_count < max_count; ++i)
		{
			Character *target = (Character*)scene->GetObj(target_objid_list[i]);
			if (NULL == target) continue;

			float dir = 0.0f;

			if (target->GetPos().x >= chara->GetPos().x && target->GetPos().y >= chara->GetPos().y)
			{
				dir = dir_list[1];
			}
			else if (target->GetPos().x <= chara->GetPos().x && target->GetPos().y >= chara->GetPos().y)
			{
				dir = dir_list[2];
			}
			else if (target->GetPos().x <= chara->GetPos().x && target->GetPos().y <= chara->GetPos().y)
			{
				dir = dir_list[3];
			}
			else
			{
				dir = dir_list[4];
			}

			if (0 != dir && this->IsInRange(chara->GetPos(), dir, static_cast<float>(m_rect_length), static_cast<float>(m_rect_width / 2), target->GetPos()))
			{
				target_cha_list[target_count ++] = target;
			}
		}

		for (int i = 0; i < m_effect_count; i++)
		{
			m_effect_list[i].Attach(chara->GetScene(), chara->GetId(), chara->GetPos(), target_count, target_cha_list, m_skill_id);
		}
	}
}

bool MonsterRectRangeCommonSkillToEnemyPos::IsInRange(Posi origin_pos, float dir, float distance, float width, Posi target_pos) const
{
	dir = -dir;
	Posi offofset_pos = target_pos - origin_pos;

	float x = offofset_pos.x * cos(dir) - offofset_pos.y * sin(dir);
	float y = offofset_pos.x * sin(dir) + offofset_pos.y * cos(dir);
	if (x >= -2.0f && x <= distance + 2.0f && y >= -width && y <= width)
	{
		return true;
	}

	return false;
}

// ------------------------------------------------------------------------------------------------------------

// 多区域
// ------------------------------------------------------------------------------------------------------------
void MonsterMultiRangeCommonSkillToEnemyPos::Perform(Character *chara, int level, Character *target, const Posi &pos, int special_param) const
{
	if (SKILL_PHASE_READING == special_param || SKILL_PHASE_CONTINUE_PERFORM == special_param || SKILL_PHASE_END == special_param)
	{
		SendSkillPhase(chara, m_skill_id, special_param);
	}
	else
	{
		Scene *scene = chara->GetScene();

		for (int i = 0; i < m_rand_zone_count; ++i)
		{
			Posi zone_pos = gamecommon::GetDisRandPos(pos, GetAttackRange(level));

			SendAOEAttackMsg(chara, target, zone_pos);

			static ObjID target_objid_list[SKILL_ATTACH_EFFECT_MAX_TARGET] = {INVALID_OBJ_ID};
			int range = this->GetAttackRange(level);
			int range2 = this->GetAttackRange2(level);
			int obj_count = SkillBase::GetTargetList(scene, chara, pos, range, range2, target_objid_list, SKILL_ATTACH_EFFECT_MAX_TARGET, m_skill_range_type, m_effect_target);

			Character *target_cha_list[SKILL_ATTACH_EFFECT_MAX_TARGET] = {NULL};
			int target_count = 0;
			int max_count = static_cast<int>(sizeof(target_cha_list) / sizeof(target_cha_list[0]));

			for (int i = 0; i < obj_count && target_count < max_count; ++i)
			{
				Character *target = (Character*)scene->GetObj(target_objid_list[i]);
				if (NULL == target) continue;

				// 先判断是否闪避
				if (CheckShanbi() && IsShanbi(chara, target)) continue;							// 分别判断每个目标闪避，如果闪避了 不掉血，效果不起作用

				target_cha_list[target_count ++] = target;

				if (GetHurtPercent(1) > 0 || GetFixHurt(1) > 0) 								// 技能可能无伤，节省一次公式
				{
					int fight_type = EffectBase::FIGHT_TYPE_NORMAL;
					long long injure = CalcInjureAndType(chara, target, level, fight_type);

					EffectOnce *effect = new EffectOnce(chara->GetId(), m_skill_id, -injure, fight_type);
					target->AddEffect(effect);
				}
			}

			for (int i = 0; i < m_effect_count; i++) 
			{
				m_effect_list[i].Attach(chara->GetScene(), chara->GetId(), chara->GetPos(), target_count, target_cha_list, m_skill_id);
			}
		}		
	}
}
