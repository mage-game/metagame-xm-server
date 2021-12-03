

#include "greatesoldier_skill.hpp"
#include "greatesoldier.hpp"
#include "greatesoldierconfig.hpp"
#include "gamecommon.h"
#include "effect/skilleffect/effectpercentbuff.hpp"
#include "skill/skilldef.hpp"
#include "skill/skillpool.hpp"
#include "skill/skillbase.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectinterval.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "config/logicconfigmanager.hpp"
#include "friend/teaminfo.hpp"
#include "scene/scene.h"
#include "scene/map.h"
#include "effect/skilleffect/effectreboundhurt.hpp"

void GreateSoldierSkill::Init(Role * role, GreateSoldier *greate_soldier)
{
	m_role = role;
	m_greate_soldier = greate_soldier;

	is_greatesoldier_baoji_skill = false;
}

void GreateSoldierSkill::CheckActiveSkill()
{
	//CapabilityNotifyLocker locker(m_role);

	int cur_used_seq = m_greate_soldier->m_param.other_param.cur_used_seq;

	// 检查主动技能
	{
		// 设置变身后技能
		int skill_level = 1;
		auto level_cfg = GREATE_SOLDIER_LEVEL_CFG(cur_used_seq);
		if (nullptr != level_cfg)
		{
			skill_level = level_cfg->active_skill_level;

			if (m_greate_soldier->GetHuanhuaID() > 0)  // 幻化后使用幻化的技能等级
			{
				const GreateSoldierHuanhuaConfig* huanhua_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetGreateSoldierHuanhuaConfig(m_greate_soldier->GetHuanhuaID());
				if (huanhua_cfg)
				{
					skill_level = huanhua_cfg->active_skill_level;
				}
			}
		}

		m_role->GetSkillManager()->RemoveSkill(GREATE_SOLDIER_SKILL_1_ID, false);
		m_role->GetSkillManager()->RemoveSkill(GREATE_SOLDIER_SKILL_2_ID, false);
		m_role->GetSkillManager()->RemoveSkill(GREATE_SOLDIER_SKILL_3_ID, false);

		if (m_greate_soldier->IsBianshenStatus())
		{
			m_role->GetSkillManager()->AddSkill(GREATE_SOLDIER_SKILL_1_ID, skill_level, nullptr, false, false);
			m_role->GetSkillManager()->AddSkill(GREATE_SOLDIER_SKILL_2_ID, skill_level, nullptr, false, false);
			m_role->GetSkillManager()->AddSkill(GREATE_SOLDIER_SKILL_3_ID, skill_level, nullptr, false, false);
		}

		m_role->GetSkillManager()->SendSkillListInfo();
	}

	// 检查特殊技能
	{
		// 位标记范围检查
		UNSTD_STATIC_CHECK(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_MAX <= int(sizeof(m_special_skill_flag) * CHAR_BIT))

		m_old_special_skill_flag = m_special_skill_flag;
		m_special_skill_flag = 0;
		if (m_greate_soldier->IsBianshenStatus())
		{
			for (int i = 0; i < ARRAY_ITEM_COUNT(m_greate_soldier->m_param.item_param.item_list); ++i)
			{
				auto item = &m_greate_soldier->m_param.item_param.item_list[i];
				if (item->HasActive())
				{
					auto level_cfg = GREATE_SOLDIER_LEVEL_CFG(i);
					if (level_cfg && 
						level_cfg->active_skill_type > GREATE_SOLDIER_SPECIAL_SKILL_TYPE_INVALID && 
						level_cfg->active_skill_type < GREATE_SOLDIER_SPECIAL_SKILL_TYPE_MAX)
					{
						SET_BIT(m_special_skill_flag, level_cfg->active_skill_type);
					}
				}
			}
		}

		if (m_old_special_skill_flag != m_special_skill_flag)
		{
			this->OnChangeSpecialSkill();
		}
	}

	// 当前使用的名将是否变异
	/*bool is_bianyi = false;
	if (GREATE_SOLDIER_CFG().IsValidSeq(cur_used_seq))
	{
		auto item = &m_greate_soldier->m_param.item_param.item_list[cur_used_seq];
		is_bianyi = GREATE_SOLDIER_CFG().IsBianyi(cur_used_seq, item->level, item->wash_attr_points);
	}*/

	// 检查被动技能
	{
		auto buff_type = EBT_INVALID;
		auto deliverer = m_role->GetId();
		auto product_method = EffectBase::PRODUCT_METHOD_SKILL;
		auto product_id = EffectBase::PRODUCT_ID_GREATE_SOLDIER_PASSIVE_SKILL;

		m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTR_PER, buff_type, INVALID_OBJ_ID, product_method, product_id);
		m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, buff_type, INVALID_OBJ_ID, product_method, product_id);

		auto level_cfg = GREATE_SOLDIER_LEVEL_CFG(cur_used_seq);
		if (level_cfg)
		{
			auto passive_skill_cfg = GREATE_SOLDIER_PASSIVE_SKILL_CFG(level_cfg->active_passive_skill_seq);
			if (nullptr != passive_skill_cfg)
			{
				auto duration_ms = 1000 * m_greate_soldier->GetBianshenRemainingTime();
				auto product_level = 1;

				// 变异加成，只有原本就有加成时，才会附加变异加成
				//int bianyi_add_per = is_bianyi ? GREATE_SOLDIER_OTHER_CFG().bianyi_add_passive_skill_per : 0;

				int passive_strength = 0;
				// 潜能等级加成
				{
					int cur_level = m_greate_soldier->GetParam().item_param.item_list[cur_used_seq].potential_level;
					for(int level = 1; level <= cur_level; level++)
					{
						auto potential_cfg = GREATE_SOLDIER_POTENTIAL_CFG(cur_used_seq, level - 1);
						if (potential_cfg && potential_cfg->passive_strength > 0)
						{
							passive_strength += potential_cfg->passive_strength;
						}
					}
				}

				// 加属性
				{
					auto effect = new EffectBuff(deliverer, product_id, duration_ms, product_level, product_method);

					if (passive_skill_cfg->add_gongji_per > 0)
					{
						double rate = static_cast<double>((passive_skill_cfg->add_gongji_per + passive_strength) * 0.01);
						Attribute add_gongji = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * rate);
						effect->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, add_gongji);
					}

					if (passive_skill_cfg->add_fangyu_per > 0)
					{
						double rate = static_cast<double>((passive_skill_cfg->add_fangyu_per + passive_strength) * 0.01);
						Attribute add_fangyu = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU) * rate);
						effect->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU, add_fangyu);
					}

					if (passive_skill_cfg->add_maxhp_per > 0)
					{
						double rate = static_cast<double>((passive_skill_cfg->add_maxhp_per + passive_strength) * 0.01);
						Attribute add_maxhp = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * rate);
						effect->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, add_maxhp);
					}

					effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
					effect->SetClearOnDie(false);
					m_role->AddEffect(effect);
				}

				// 加属性
//				{
// 					auto effect = new EffectBuff(deliverer, product_id, duration_ms, product_level, product_method);
// 
// 					if (passive_skill_cfg->add_hurt_increase > 0)
// 					{
// 						effect->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_POFANG, passive_skill_cfg->add_hurt_increase);
// 					}
// 
// 					if (passive_skill_cfg->add_hurt_reduce > 0)
// 					{
// 						effect->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG, passive_skill_cfg->add_hurt_reduce);
// 					}
// 
// 					effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
// 					effect->SetClearOnDie(false);
// 					m_role->AddEffect(effect);
//				}
			}
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GREATE_SOLDIER);
}

void GreateSoldierSkill::Update(time_t now_second, unsigned long interval)
{
	if (nullptr == m_role) return;
	Scene *scene = m_role->GetScene();
	if (nullptr == scene) return;
	
	int skill_level = 1;
	int cur_used_seq = m_greate_soldier->m_param.other_param.cur_used_seq;
	int cur_potential_level = m_greate_soldier->GetPotentialLevelBySeq(cur_used_seq);
	if (cur_potential_level > 0)
	{
		auto potential_cfg = GREATE_SOLDIER_POTENTIAL_CFG(cur_used_seq, cur_potential_level - 1);
		if (potential_cfg)
		{
			skill_level = potential_cfg->special_skill_level;
		}
	}

	if (m_greate_soldier->IsBianshenStatus())
	{
		// 特殊技能11，变身期间，每param_a秒对圆形范围(10)内x个目标造成80%攻击的伤害，走公式（触发间隔param_a是名将特殊技能表参数。其他参数都是技能配置参数）
		if (IS_BIT_SET(m_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_11))
		{
			auto skill_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetSpecialSkillCfg(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_11, skill_level);
			if (skill_cfg)
			{
				int cd_s = skill_cfg->param_a > 0 ? skill_cfg->param_a : 1;
				if (now_second >= m_skill_perform_time_11 + cd_s)
				{
					const SkillBase *skill = SKILLPOOL->GetSkill(GREATE_SOLDIER_SKILL_4_ID);  // 603
					if (skill)
					{
						skill->Perform(m_role, skill_cfg->skill_level, m_role, m_role->GetPos(), 0);
					}
					m_skill_perform_time_11 = now_second;
				}
			}
		}

		// 特殊技能13：变身期间，自己和队友每param_a秒恢复param_b%最大生命值(距离param_c内)
		if (IS_BIT_SET(m_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_13))
		{
			auto skill_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetSpecialSkillCfg(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_13, skill_level);
			if (skill_cfg)
			{
				int cd_s = skill_cfg->param_a > 0 ? skill_cfg->param_a : 1;
				if (now_second >= m_skill_perform_time_13 + cd_s)
				{
					const double recover_percent = (double)skill_cfg->param_b * ROLE_ATTR_PER;

					// 给队友加
					if (GreateSoldierSpecialSkillConfig::SPECIAL_SKILL_MODE_TEAM == skill_cfg->skill_mode)
					{
						Role *member_list[MAX_TEAM_MEMBER_NUM] = { 0 };
						int member_count = m_role->GetTeamInfo()->GetOtherMemberInTeam(member_list, sizeof(member_list) / sizeof(member_list[0]));
						for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
						{
							Role *tmp_role = member_list[i];
							if (tmp_role && tmp_role->IsAlive() && !(m_role->GetPos() - tmp_role->GetPos()).IsLongerThan(skill_cfg->param_c) )
							{
								Attribute add_hp = (Attribute)(recover_percent * tmp_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP));
								tmp_role->ChangeHp(&add_hp, true);
							}
						}
					}

					// 给自己加
					{
						if (m_role->IsAlive())
						{
							Attribute add_hp = (Attribute)(recover_percent * m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP));
							m_role->ChangeHp(&add_hp, true);
						}
					}

					m_skill_perform_time_13 = now_second;
				}
			}
		}
	}
}

void GreateSoldierSkill::OnChangeSpecialSkill()
{
	if (nullptr == m_role) return;
	Scene *scene = m_role->GetScene();
	if (nullptr == scene)
	{
		return;
	}

	int skill_level = 1;
	int cur_used_seq = m_greate_soldier->m_param.other_param.cur_used_seq;
	int cur_potential_level = m_greate_soldier->GetPotentialLevelBySeq(cur_used_seq);
	if (cur_potential_level > 0)
	{
		auto potential_cfg = GREATE_SOLDIER_POTENTIAL_CFG(cur_used_seq, cur_potential_level - 1);
		if (potential_cfg)
		{
			skill_level = potential_cfg->special_skill_level;
		}
	}

	Role *member_list[MAX_TEAM_MEMBER_NUM] = { 0 };
	int member_count = m_role->GetTeamInfo()->GetOtherMemberInTeam(member_list, sizeof(member_list) / sizeof(member_list[0]));

	static const int OBJ_LIST_NUM = 256;
	static const int NEARBY_OBJ_RANGE = 35;
	ObjID nearby_obj_list[OBJ_LIST_NUM] = { 0 };
	int nearby_obj_count = SkillBase::GetSceneObjByArea(scene, m_role->GetPos(), NEARBY_OBJ_RANGE, nearby_obj_list, ARRAY_ITEM_COUNT(nearby_obj_list));

	// 特殊技能1，全队每秒回血param_a万分比，持续变身期间
	if (IS_BIT_SET(m_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_1))
	{
		auto delever = m_role->GetId();
		auto product_method = 0;
		auto product_id = EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL;
		auto buff_type = EBT_ADD_BLOOD;
		auto product_level = 1;
		auto count = m_greate_soldier->GetBianshenRemainingTime();
		auto skill_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetSpecialSkillCfg(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_1, skill_level);
		if (skill_cfg)
		{
			// 给队友加
			for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
			{
				Role *tmp_role = member_list[i];
				if (tmp_role)
				{
					Attribute add_hp = static_cast<Attribute>(skill_cfg->param_a * ROLE_ATTR_PER * tmp_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP));
					EffectInterval *effect = new EffectInterval(delever, product_id, buff_type, product_level, product_method);
					effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
					effect->SetParam(1000, count, add_hp, delever);
					tmp_role->AddEffect(effect);
				}
			}

			// 给自己加
			{
				Attribute add_hp = static_cast<Attribute>(skill_cfg->param_a * ROLE_ATTR_PER * m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP));
				EffectInterval *effect = new EffectInterval(delever, product_id, buff_type, product_level, product_method);
				effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
				effect->SetParam(1000, count, add_hp, delever);
				m_role->AddEffect(effect);
			}
		}
	}

	// 特殊技能2，免疫所有控制技能，持续变身期间
	if (IS_BIT_SET(m_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_2))
	{
		auto delever = m_role->GetId();
		auto product_id = EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL;
		auto effect_time = m_greate_soldier->GetBianshenRemainingTime() * 1000;
		auto buff_type = EBT_SUPER_MIANYI;
		auto product_level = 1;

		EffectSpecialState *effect_buff = new EffectSpecialState(delever, product_id, effect_time, buff_type, product_level);
		m_role->AddEffect(effect_buff);
	}

	// 特殊技能3，变身，晕眩周围敌人，持续param_a毫秒
	if (IS_BIT_SET(m_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_3) || 
		IS_BIT_SET(m_old_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_3))
	{
		for (int i = 0; i < nearby_obj_count && i < OBJ_LIST_NUM; ++i)
		{
			auto chara = dynamic_cast<Character*>(scene->GetObj(nearby_obj_list[i]));
			if (chara && !scene->GetMap()->SaveArea(chara->GetPos().x, chara->GetPos().y) && m_role->IsEnemy(chara))
			{
				auto delever = m_role->GetId();
				auto product_id = EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL;
				auto effect_time = 2000;
				auto buff_type = EBT_XUANYUN;
				auto product_level = 1;
				auto skill_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetSpecialSkillCfg(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_3, skill_level);
				if (skill_cfg)
				{
					effect_time = skill_cfg->param_a;
					auto special_effect = new EffectSpecialState(delever, product_id, effect_time, buff_type, product_level);
					special_effect->SetClientEffectType(EffectBase::ECT_SKILL_XUANYUN);
					chara->AddEffect(special_effect);
				}
			}
		}
	}

	// 特殊技能4：变身时，回复队友生命param_a万分比
	if (IS_BIT_SET(m_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_4))
	{
		auto skill_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetSpecialSkillCfg(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_4, skill_level);
		if (skill_cfg)
		{
			const double recover_percent = (double)skill_cfg->param_a * ROLE_ATTR_PER;

			// 给队友加
			for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
			{
				Role *tmp_role = member_list[i];
				if (tmp_role && tmp_role->IsAlive())
				{
					Attribute add_hp = (Attribute)(recover_percent * tmp_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP));
					tmp_role->ChangeHp(&add_hp, true);
				}
			}

			// 给自己加
			{
				if (m_role->IsAlive())
				{
					Attribute add_hp = (Attribute)(recover_percent * m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP));
					m_role->ChangeHp(&add_hp, true);
				}
			}
		}
	}
	
	// 特殊技能5：变身时对周围玩家造成攻击力param_a万分比的伤害
	if (IS_BIT_SET(m_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_5))
	{
		auto skill_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetSpecialSkillCfg(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_5, skill_level);
		if (skill_cfg)
		{
			Attribute hurt = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (skill_cfg->param_a * ROLE_ATTR_PER));

			for (int i = 0; i < nearby_obj_count && i < OBJ_LIST_NUM; ++i)
			{
				auto chara = dynamic_cast<Character*>(scene->GetObj(nearby_obj_list[i]));
				if (chara && chara->GetObjType() == Obj::OBJ_TYPE_ROLE && !scene->GetMap()->SaveArea(chara->GetPos().x, chara->GetPos().y) && m_role->IsEnemy(chara))
				{
					EffectOnce *effect = new EffectOnce(m_role->GetId(), 0, -hurt, EffectBase::FIGHT_TYPE_NORMAL);
					chara->AddEffect(effect);
				}
			}
		}
	}

	// 特殊技能6：变身期间，自身受到伤害降低param_a万分比
	if (IS_BIT_SET(m_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_6))
	{
		auto delever = m_role->GetId();
		auto product_id = EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL;
		auto effect_time = m_greate_soldier->GetBianshenRemainingTime() * 1000;
		auto product_level = 1;
		auto skill_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetSpecialSkillCfg(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_6, skill_level);
		if (skill_cfg)
		{
			int dec_percent = skill_cfg->param_a;	// 万分比

			// 给队友加
			if (GreateSoldierSpecialSkillConfig::SPECIAL_SKILL_MODE_TEAM == skill_cfg->skill_mode)
			{
				for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
				{
					Role *tmp_role = member_list[i];
					if (tmp_role)
					{
						EffectDecHurt *effect_dechurt = new EffectDecHurt(delever, product_id, effect_time, product_level);
						effect_dechurt->SetDecPercent(dec_percent);
						effect_dechurt->SetClientEffectType(EffectBase::ECT_SKILL_DEC_ON_HURT);
						tmp_role->AddEffect(effect_dechurt);
					}
				}
			}

			// 给自己加
			EffectDecHurt *effect_dechurt = new EffectDecHurt(delever, product_id, effect_time, product_level);
			effect_dechurt->SetDecPercent(dec_percent);
			effect_dechurt->SetClientEffectType(EffectBase::ECT_SKILL_DEC_ON_HURT);
			m_role->AddEffect(effect_dechurt);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 特殊技能7：额外暴击率增加param_a万分比
	if (IS_BIT_SET(m_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_7))
	{
		ObjID deliverer = m_role->GetId();
		int product_id = EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL;
		int product_method = EffectBase::PRODUCT_METHOD_PER_BAOJI;
		auto effect_time = m_greate_soldier->GetBianshenRemainingTime() * 1000;
		int product_level = 1;
		auto skill_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetSpecialSkillCfg(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_7, skill_level);
		if (skill_cfg)
		{
			const int add_value = skill_cfg->param_a;

			// 给队友加
			if (GreateSoldierSpecialSkillConfig::SPECIAL_SKILL_MODE_TEAM == skill_cfg->skill_mode)
			{
				for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
				{
					Role *tmp_role = member_list[i];
					if (tmp_role)
					{
						EffectBuff *buff = new EffectBuff(deliverer, product_id, effect_time, product_level, product_method);
						buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI, add_value, add_value);
						buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
						buff->SetSave(false);
						buff->SetClientEffectType(EffectBase::ECT_SKILL_ADD_BAOJI_PER);
						tmp_role->AddEffect(buff);
					}
				}
			}

			// 给自己加
			{
				EffectBuff *buff = new EffectBuff(deliverer, product_id, effect_time, product_level, product_method);
				buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI, add_value, add_value);
				buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
				buff->SetSave(false);
				buff->SetClientEffectType(EffectBase::ECT_SKILL_ADD_BAOJI_PER);
				m_role->AddEffect(buff);
			}
		}
	}

	// 特殊技能8：额外闪避率增加param_a万分比
	if (IS_BIT_SET(m_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_8))
	{
		ObjID deliverer = m_role->GetId();
		int product_id = EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL;
		int product_method = EffectBase::PRODUCT_METHOD_PER_SHANBI;
		auto effect_time = m_greate_soldier->GetBianshenRemainingTime() * 1000;
		int product_level = 1;
		auto skill_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetSpecialSkillCfg(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_8, skill_level);
		if (skill_cfg)
		{
			const int add_value = skill_cfg->param_a;

			// 给队友加
			if (GreateSoldierSpecialSkillConfig::SPECIAL_SKILL_MODE_TEAM == skill_cfg->skill_mode)
			{
				for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
				{
					Role *tmp_role = member_list[i];
					if (tmp_role)
					{
						EffectBuff *buff = new EffectBuff(deliverer, product_id, effect_time, product_level, product_method);
						buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI_PER, add_value, add_value);
						buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
						buff->SetSave(false);
						buff->SetClientEffectType(EffectBase::ECT_SKILL_ADD_SHANBI_PER);
						tmp_role->AddEffect(buff);
					}
				}
			}

			// 给自己加
			{
				EffectBuff *buff = new EffectBuff(deliverer, product_id, effect_time, product_level, product_method);
				buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI_PER, add_value, add_value);
				buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
				buff->SetSave(false);
				buff->SetClientEffectType(EffectBase::ECT_SKILL_ADD_SHANBI_PER);
				m_role->AddEffect(buff);
			}
		}
	}

	// 特殊技能9：额外伤害加成增加param_a万分比
	if (IS_BIT_SET(m_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_9))
	{
		ObjID deliverer = m_role->GetId();
		int product_id = EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL;
		int product_method = 1;
		auto effect_time = m_greate_soldier->GetBianshenRemainingTime() * 1000;
		int product_level = 1;
		auto skill_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetSpecialSkillCfg(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_9, skill_level);
		if (skill_cfg)
		{
			const int add_value = skill_cfg->param_a;

			// 给队友加
			if (GreateSoldierSpecialSkillConfig::SPECIAL_SKILL_MODE_TEAM == skill_cfg->skill_mode)
			{
				for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
				{
					Role *tmp_role = member_list[i];
					if (tmp_role)
					{
						EffectBuff *buff = new EffectBuff(deliverer, product_id, effect_time, product_level, product_method);
						buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_POFANG, add_value);
						buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
						buff->SetSave(false);
						tmp_role->AddEffect(buff);
					}
				}
			}

			// 给自己加
			{
				EffectBuff *buff = new EffectBuff(deliverer, product_id, effect_time, product_level, product_method);
				buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_POFANG, add_value);
				buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
				buff->SetSave(false);
				m_role->AddEffect(buff);
			}
		}
	}

	// 特殊技能10：额外伤害减免增加param_a万分比
	if (IS_BIT_SET(m_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_10))
	{
		ObjID deliverer = m_role->GetId();
		int product_id = EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL;
		int product_method = 1;
		auto effect_time = m_greate_soldier->GetBianshenRemainingTime() * 1000;
		int product_level = 1;
		auto skill_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetSpecialSkillCfg(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_10, skill_level);
		if (skill_cfg)
		{
			const int add_value = skill_cfg->param_a;

			// 给队友加
			if (GreateSoldierSpecialSkillConfig::SPECIAL_SKILL_MODE_TEAM == skill_cfg->skill_mode)
			{
				for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
				{
					Role *tmp_role = member_list[i];
					if (tmp_role)
					{
						EffectBuff *buff = new EffectBuff(deliverer, product_id, effect_time, product_level, product_method);
						buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG, add_value);
						buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
						buff->SetSave(false);
						tmp_role->AddEffect(buff);
					}
				}
			}

			// 给自己加
			{
				EffectBuff *buff = new EffectBuff(deliverer, product_id, effect_time, product_level, product_method);
				buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG, add_value);
				buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
				buff->SetSave(false);
				m_role->AddEffect(buff);
			}
		}
	}

	// 特殊技能12：变身期间，反弹param_a万分比受到的伤害
	if (IS_BIT_SET(m_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_12))
	{
		auto effect_time = m_greate_soldier->GetBianshenRemainingTime() * 1000;
		int product_id = EffectBase::PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL;
		auto skill_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetSpecialSkillCfg(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_12, skill_level);
		if (skill_cfg)
		{
			int rebound_percent = skill_cfg->param_a / 100;

			// 给队友加
			if (GreateSoldierSpecialSkillConfig::SPECIAL_SKILL_MODE_TEAM == skill_cfg->skill_mode)
			{
				for (int i = 0; i < member_count && i < MAX_TEAM_MEMBER_NUM; ++i)
				{
					Role *tmp_role = member_list[i];
					if (tmp_role)
					{
						EffectReboundHurt *effect_rebound = new EffectReboundHurt(INVALID_OBJ_ID, product_id, effect_time, rebound_percent, 1);
						tmp_role->AddEffect(effect_rebound);
					}
				}
			}

			// 给自己加
			EffectReboundHurt *effect_rebound = new EffectReboundHurt(INVALID_OBJ_ID, product_id, effect_time, rebound_percent, 1);
			m_role->AddEffect(effect_rebound);
		}
	}

	// 特殊技能14，变身时，对圆形范围内x个目标造成200%攻击的伤害并使其眩晕，持续2秒, 走公式（参数都是技能配置参数，持续时间是技能param_a参数：秒）
	if (IS_BIT_SET(m_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_14))
	{
		auto skill_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetSpecialSkillCfg(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_14, skill_level);
		if (skill_cfg)
		{
			const SkillBase *skill = SKILLPOOL->GetSkill(GREATE_SOLDIER_SKILL_5_ID); // 604
			if (skill)
			{
				skill->Perform(m_role, skill_cfg->skill_level, m_role, m_role->GetPos(), 0);
			}
		}
	}

	// 特殊技能15，变身时，对圆形范围（12）内6个目标拉到自己身边，造成200%的攻击力的伤害并使其移动速度降低param_a万分比，持续param_b秒,走公式（参数都是技能配置表参数）
	if (IS_BIT_SET(m_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_15))
	{
		auto skill_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetSpecialSkillCfg(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_15, skill_level);
		if (skill_cfg)
		{
			const SkillBase *skill = SKILLPOOL->GetSkill(GREATE_SOLDIER_SKILL_6_ID); // 605
			if (skill)
			{
				skill->Perform(m_role, skill_cfg->skill_level, m_role, m_role->GetPos(), 0);
			}
		}
	}

	// 特殊技能16，变身结束的一瞬间，对周围8个目标造成120%攻击的伤害并使其沉默param_a秒，走公式（参数都是技能配置表参数）
	if (IS_BIT_SET(m_old_special_skill_flag, GREATE_SOLDIER_SPECIAL_SKILL_TYPE_16) && (false == m_greate_soldier->IsBianshenStatus()))
	{
		auto skill_cfg = LOGIC_CONFIG->GetGreateSoldierConfig().GetSpecialSkillCfg(GREATE_SOLDIER_SPECIAL_SKILL_TYPE_16, skill_level);
		if (skill_cfg)
		{
			const SkillBase *skill = SKILLPOOL->GetSkill(GREATE_SOLDIER_SKILL_7_ID); // 606
			if (skill)
			{
				skill->Perform(m_role, skill_cfg->skill_level, m_role, m_role->GetPos(), 0);
			}
		}
	}
}

void GreateSoldierSkill::OnRoleBeenAttack(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill)
{
	if (!m_role->GetScene()) return;
	if (delta_hp >= 0) return;
	if (!m_greate_soldier->IsBianshenStatus()) return;
	
	//Role *member_list[MAX_TEAM_MEMBER_NUM] = { 0 };
	//int member_count = m_role->GetTeamInfo()->GetOtherMemberInTeam(member_list, sizeof(member_list) / sizeof(member_list[0]));

}

