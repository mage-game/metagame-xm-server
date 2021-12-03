#include "shenshou.hpp"
#include "shenshouconfig.hpp"

#include "engineadapter.h"
#include "config/logicconfigmanager.hpp"
#include "protocal/msgshenshou.h"
#include "obj/character/role.h"
#include "gamelog.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "scene/scene.h"
#include "other/capability/capability.hpp"
#include "skill/skillbase.hpp"
#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "item/knapsack.h"
#include "effect/skilleffect/effectabsorbhpbeenattack.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "item/itempool.h"
#include "item/expense/norexitem.hpp"
#include "other/talent/talent.hpp"
#include "config/joinlimitconfig.hpp"
#include "other/rolebigsmallgoal/rolebigsmallgoal.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "other/event/eventhandler.hpp"
#include "task/taskrecorder.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "other/tianshuxunzhu/tianshuxunzhu.hpp"

Shenshou::Shenshou() : m_role(NULL), m_cur_zhuzhan_count(0), m_cd_reduce_percent(0)
{
}

Shenshou::~Shenshou()
{
}

void Shenshou::Init(Role *role, const ShenshouParam &Shenshou_param)
{
	m_role = role;
	m_param = Shenshou_param;

	this->RecalcSkillList();

	// 统计助战神兽个数
	m_cur_zhuzhan_count = 0;
	for (int shou_id = 1; shou_id <= SHENSHOU_MAX_ID; ++shou_id)
	{
		if (m_param.shenshou_list[shou_id].has_zhuzhan > 0)
		{
			++m_cur_zhuzhan_count;
		}
	}
}

void Shenshou::GetInitParam(ShenshouParam *shenshou_param) const
{
	*shenshou_param = m_param;
}

void Shenshou::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (!this->CheckOpengameDay())
	{
		return;
	}

	if (is_recalc)
	{
		m_attrs_add.Reset();

		this->ReCalcSpecialRoleBigGoalAttr();        // 激活大目标属性加成

		CharIntAttrs all_shou_extra_attr; // 所有神兽的额外属性。不参与作为基数计算的属性都临时加到这里，不影响技能的百分比计算（策划的需求，让程序的代码开始想吐了）
		all_shou_extra_attr.Reset();

		CharIntAttrs cur_shou_base_attr; // 当前神兽的基础属性（除了随机属性跟技能）
		CharIntAttrs cur_shou_extra_attrs_add; // 当前神兽技能或随机属性的百分比属性加成，以基础属性为基数来计算

		for (int shou_id = 1; shou_id <= SHENSHOU_MAX_ID; ++shou_id)
		{
			if (m_param.shenshou_list[shou_id].has_zhuzhan <= 0) continue; // 过滤未助战的神兽

			cur_shou_base_attr.Reset();
			cur_shou_extra_attrs_add.Reset();
			const ShenshouInfoCfg *shenshou_info = LOGIC_CONFIG->GetShenshouConfig().GetShenshouInfo(shou_id);
			if (NULL == shenshou_info) continue;

			// 基础属性
			{
				cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += Attribute(shenshou_info->maxhp);
				cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += Attribute(shenshou_info->gongji);
				//cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FA_GONGJI] += Attribute(shenshou_info->gongji);
				cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += Attribute(shenshou_info->fangyu);
				//cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += Attribute(shenshou_info->shanbi);
				//cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += Attribute(shenshou_info->mingzhong);

				{
					// 天赋技能 - 神兽精通
					//long long add_fa_fangyu = m_role->GetModuleMgr()->GetTalent().GetTalentExtraValue(TALENT_ATTR_TYPE_SHENSHOU_MAXHP_PER, cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP]);
					//cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += add_fa_fangyu;
				}
				m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_SHENSHOU, cur_shou_base_attr, m_attrs_add);
			}

			// 计算装备属性
			for (int equip_slot = 0; equip_slot <= SHENSHOU_MAX_EQUIP_SLOT_INDEX; ++equip_slot)
			{
				ShenshouBackpackItem &item = m_param.shenshou_list[shou_id].equip_list[equip_slot];
				const ShenshouItemCfg *item_cfg = LOGIC_CONFIG->GetShenshouConfig().GetItemInfo(item.item_id);

				if (NULL == item_cfg || !item_cfg->is_equip) continue;
				
				// 基础属性
				const ShenshouEquipQualityCfg::BaseAttrduCfg *base_attr_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipBaseAttrCfg(item_cfg->quality, equip_slot);
				if (NULL != base_attr_cfg)
				{
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += Attribute(base_attr_cfg->maxhp);
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += Attribute(base_attr_cfg->gongji);
					//cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FA_GONGJI] += Attribute(base_attr_cfg->gongji);
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += Attribute(base_attr_cfg->fangyu);
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += Attribute(base_attr_cfg->shanbi);
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += Attribute(base_attr_cfg->mingzhong);
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += Attribute(base_attr_cfg->baoji);
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += Attribute(base_attr_cfg->jianren);
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += Attribute(base_attr_cfg->constant_zengshang);
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += Attribute(base_attr_cfg->constant_mianshang);
				}

				// 装备等级属性
				const ShenshouEquipQualityCfg::LevelShulianduCfg *tmp_shuliandu_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipShulianduCfg(equip_slot, item.strength_level);
				if (NULL != tmp_shuliandu_cfg)
				{
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += Attribute(tmp_shuliandu_cfg->maxhp);
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += Attribute(tmp_shuliandu_cfg->gongji);
					//cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FA_GONGJI] += Attribute(tmp_shuliandu_cfg->gongji);
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += Attribute(tmp_shuliandu_cfg->fangyu);
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += Attribute(tmp_shuliandu_cfg->shanbi);
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += Attribute(tmp_shuliandu_cfg->mingzhong);
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += Attribute(tmp_shuliandu_cfg->baoji);
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += Attribute(tmp_shuliandu_cfg->jianren);
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += Attribute(tmp_shuliandu_cfg->constant_zengshang);
					cur_shou_base_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += Attribute(tmp_shuliandu_cfg->constant_mianshang);
				}
			}

			// 随机属性以神兽基础属性来计算
			for (int equip_slot = 0; equip_slot <= SHENSHOU_MAX_EQUIP_SLOT_INDEX; ++equip_slot)
			{
				ShenshouBackpackItem &item = m_param.shenshou_list[shou_id].equip_list[equip_slot];
				for (int attr_index = 0; attr_index < SHENSHOU_MAX_EQUIP_ATTR_COUNT; ++attr_index)
				{
					ShenshouBackpackItem::AttrItem &attr_item = item.attr_list[attr_index];
					this->CalcRandomAttr(cur_shou_base_attr, cur_shou_extra_attrs_add, all_shou_extra_attr, attr_item.attr_type, attr_item.attr_value);
				}
			}

			// 计算技能产生的战力加成
			for (int skill_index = 0; skill_index < ShenshouInfoCfg::MAX_SKILL_COUNT; ++skill_index)
			{
				const ShenshouInfoCfg::SkillItem &skill_cfg_item = shenshou_info->skill_list[skill_index];
				const ShenshouSkillCfg::LevelCfg *skill_level_cfg = LOGIC_CONFIG->GetShenshouConfig().GetSkillLevelCfg(skill_cfg_item.skill_type, skill_cfg_item.skill_level);
				if (NULL != skill_level_cfg)
				{
					this->RecalcSkillAttrForSingleShou(cur_shou_base_attr, cur_shou_extra_attrs_add, skill_cfg_item.skill_type, skill_level_cfg->param_list);

					// 加给人物的属性，放这里把，反正只加到all_shou_extra_attr里，不影响计算基数
					this->RecalcSkillAttrForRoleBaseAttr(all_shou_extra_attr, skill_cfg_item.skill_type, skill_level_cfg->param_list);

					// 增加全身属性的技能
					this->RecalcSkillAttrForRoleAttr(m_attrs_add, skill_cfg_item.skill_type, skill_level_cfg->param_list);
				}
			}

			m_attrs_add.Add(cur_shou_base_attr.m_attrs);
			all_shou_extra_attr.Add(cur_shou_extra_attrs_add.m_attrs);
		}

		// 重新遍历所有助阵神兽的所有技能，为了计算那些针对全体助阵神兽的属性加成的技能，蛋蛋不在了
		for (int shou_id = 1; shou_id <= SHENSHOU_MAX_ID; ++shou_id)
		{
			if (m_param.shenshou_list[shou_id].has_zhuzhan <= 0) continue; // 过滤未助战的神兽

			const ShenshouInfoCfg *shenshou_info = LOGIC_CONFIG->GetShenshouConfig().GetShenshouInfo(shou_id);
			if (NULL == shenshou_info) continue;

			for (int skill_index = 0; skill_index < ShenshouInfoCfg::MAX_SKILL_COUNT; ++skill_index)
			{
				const ShenshouInfoCfg::SkillItem &skill_cfg_item = shenshou_info->skill_list[skill_index];
				const ShenshouSkillCfg::LevelCfg *skill_level_cfg = LOGIC_CONFIG->GetShenshouConfig().GetSkillLevelCfg(skill_cfg_item.skill_type, skill_cfg_item.skill_level);
				if (NULL != skill_level_cfg)
				{
					this->RecalcSkillAttrForAllZhuzhanShou(m_attrs_add, all_shou_extra_attr, skill_cfg_item.skill_type, skill_level_cfg->param_list);
				}
			}
		}

		// 人物大目标固定属性
		m_role->GetRoleBigSmallGoal()->RoleBigSmallGoalAddSpecialAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENSHOU, m_attrs_add);

		m_attrs_add.Add(all_shou_extra_attr.m_attrs);
		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_SHENSHOU, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

void Shenshou::ReCalcSpecialRoleBigGoalAttr()
{
	int add_value = m_role->GetRoleBigSmallGoal()->GetRoleBigSmallGoalAddAttr(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENSHOU);
	if (add_value <= 0)    // 未激活大目标
	{
		return;
	}

	CharIntAttrs special_big_goal_attr;

	for (int shou_id = 1; shou_id <= SHENSHOU_MAX_ID; ++shou_id)
	{
		if (m_param.shenshou_list[shou_id].has_zhuzhan <= 0) continue; // 过滤未助战的神兽

		const ShenshouInfoCfg *shenshou_info = LOGIC_CONFIG->GetShenshouConfig().GetShenshouInfo(shou_id);
		if (NULL == shenshou_info) continue;

		special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += Attribute(shenshou_info->maxhp);
		special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += Attribute(shenshou_info->gongji);
		special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += Attribute(shenshou_info->fangyu);

		// 计算装备属性
		for (int equip_slot = 0; equip_slot <= SHENSHOU_MAX_EQUIP_SLOT_INDEX; ++equip_slot)
		{
			ShenshouBackpackItem &item = m_param.shenshou_list[shou_id].equip_list[equip_slot];
			const ShenshouItemCfg *item_cfg = LOGIC_CONFIG->GetShenshouConfig().GetItemInfo(item.item_id);

			if (NULL == item_cfg || !item_cfg->is_equip) continue;

			// 基础属性
			const ShenshouEquipQualityCfg::BaseAttrduCfg *base_attr_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipBaseAttrCfg(item_cfg->quality, equip_slot);
			if (NULL != base_attr_cfg)
			{
				special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += Attribute(base_attr_cfg->maxhp);
				special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += Attribute(base_attr_cfg->gongji);
				special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += Attribute(base_attr_cfg->fangyu);
				special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += Attribute(base_attr_cfg->shanbi);
				special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += Attribute(base_attr_cfg->mingzhong);
				special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += Attribute(base_attr_cfg->baoji);
				special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += Attribute(base_attr_cfg->jianren);
				special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += Attribute(base_attr_cfg->constant_zengshang);
				special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += Attribute(base_attr_cfg->constant_mianshang);
			}
		}
	}

	// 算大目标加成
	{
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(round(special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (add_value * ROLE_ATTR_PER)));
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(round(special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (add_value * ROLE_ATTR_PER)));
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(round(special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (add_value * ROLE_ATTR_PER)));
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(round(special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (add_value * ROLE_ATTR_PER)));
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(round(special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (add_value * ROLE_ATTR_PER)));
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(round(special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (add_value * ROLE_ATTR_PER)));
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(round(special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (add_value * ROLE_ATTR_PER)));
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += static_cast<Attribute>(round(special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] * (add_value * ROLE_ATTR_PER)));
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += static_cast<Attribute>(round(special_big_goal_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] * (add_value * ROLE_ATTR_PER)));
	}
}

void Shenshou::OnRoleBeenAttack(ObjID attacker)
{
	if (!this->CheckOpengameDay())
	{
		return;
	}

	unsigned int now_second = (unsigned int)EngineAdapter::Instance().Time();
	Obj *attack_obj = m_role->GetScene()->GetObj(attacker);
	if (NULL == attack_obj)
	{
		return;
	}

	std::map<int, std::vector<SkillItem*> > skill_type_map; // 转换数据格式为：以技能类型作为key，因为技能触发是按照技能类型来，同类型的技能一次只触发一个

	for (std::map<int, std::vector<SkillItem> >::iterator skill_list_it = m_shenshou_active_skill_list.begin(); skill_list_it != m_shenshou_active_skill_list.end(); ++skill_list_it)
	{
		for (std::vector<SkillItem>::iterator skill_item_it = skill_list_it->second.begin(); skill_item_it != skill_list_it->second.end(); ++ skill_item_it)
		{
			skill_type_map[skill_item_it->skill_type].push_back(&(*skill_item_it));
		}
	}

	for (std::map<int, std::vector<SkillItem*> >::iterator skill_list_it = skill_type_map.begin(); skill_list_it != skill_type_map.end(); ++skill_list_it)
	{
		for (std::vector<SkillItem*>::iterator skill_item_it = skill_list_it->second.begin(); skill_item_it != skill_list_it->second.end(); ++skill_item_it)
		{
			SkillItem *skill_item = *skill_item_it;

			const ShenshouSkillCfg::LevelCfg *skill_level_cfg = LOGIC_CONFIG->GetShenshouConfig().GetSkillLevelCfg(skill_item->skill_type, skill_item->level);
			if (NULL == skill_level_cfg) continue;

			if (SHENSHOU_SKILL_TYPE_8 == skill_item->skill_type)
			{
				if (Obj::OBJ_TYPE_ROLE != attack_obj->GetObjType())
				{
					continue;
				}

				if (now_second - skill_item->last_perform_time <= (unsigned int)skill_level_cfg->param_list[PARAM4]) // CD时间未到
				{
					continue;
				}
				
				if (RandomNum(10000) < skill_level_cfg->param_list[PARAM1]) // N% 的概率触发
				{
					skill_item->last_perform_time = now_second;

					int add_mianshang = skill_level_cfg->param_list[PARAM2]; // 增加当前免伤的 M 万分比
					int last_time_ms = skill_level_cfg->param_list[PARAM3] * 1000; // 持续 X 秒

					EffectBuff *effect_buff = new EffectBuff(INVALID_OBJ_ID, SHENSHOU_SKILL_TYPE_8, last_time_ms, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
					effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG, add_mianshang);
					effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_6);
					effect_buff->SetSave(false);
					m_role->AddEffect(effect_buff);
				}
			}
			else if (SHENSHOU_SKILL_TYPE_9 == skill_item->skill_type)
			{
				if (Obj::OBJ_TYPE_ROLE != attack_obj->GetObjType())
				{
					continue;
				}

				if (now_second - skill_item->last_perform_time <= (unsigned int)skill_level_cfg->param_list[PARAM6]) // CD时间未到
				{
					continue;
				}

				Attribute max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				Attribute cur_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				if ((cur_hp * 1.0 / max_hp) < skill_level_cfg->param_list[PARAM1] * SKILL_ATTR_PER) // 血量百分比小于 N
				{
					if (RandomNum(10000) < skill_level_cfg->param_list[PARAM2]) // M% 的概率触发
					{
						skill_item->last_perform_time = now_second;

						int add_mianshang_per = (skill_level_cfg->param_list[PARAM3]); // 增加当前免伤的 M%
						int add_shanbi = (skill_level_cfg->param_list[PARAM4]); // 增加当前闪避的 M%
						int last_time_ms = skill_level_cfg->param_list[PARAM5] * 1000; // 持续 X 秒

						EffectBuff *effect_buff = new EffectBuff(INVALID_OBJ_ID, SHENSHOU_SKILL_TYPE_9, last_time_ms, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
						effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI, add_shanbi);
						effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG, add_mianshang_per);
						effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_6);
						effect_buff->SetSave(false);
						m_role->AddEffect(effect_buff);
					}
				}
			}
			else if (SHENSHOU_SKILL_TYPE_11 == skill_item->skill_type)
			{
				if (now_second - skill_item->last_perform_time <= (unsigned int)skill_level_cfg->param_list[PARAM4]) // CD时间未到
				{
					continue;
				}

				if (RandomNum(10000) < skill_level_cfg->param_list[PARAM1]) // M% 的概率触发
				{
					skill_item->last_perform_time = now_second;

					int last_time_ms = skill_level_cfg->param_list[PARAM2] * 1000; // 持续 X 毫秒
					int percent = skill_level_cfg->param_list[PARAM3] / 100;
					// 被击回血150%
					EffectAbsorbHPBeenAttack *effect_recover = new EffectAbsorbHPBeenAttack(m_role->GetId(), SHENSHOU_SKILL_TYPE_11, last_time_ms , percent, 1);
					m_role->AddEffect(effect_recover);
				}
			}
			else if (SHENSHOU_SKILL_TYPE_15 == skill_item->skill_type)
			{
				if (now_second - skill_item->last_perform_time <= (unsigned int)skill_level_cfg->param_list[PARAM5]) // CD时间未到
				{
					continue;
				}

				Attribute max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				Attribute cur_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				if ((cur_hp * 1.0 / max_hp) < skill_level_cfg->param_list[PARAM1] * SKILL_ATTR_PER) // 血量百分比小于 N
				{
					skill_item->last_perform_time = now_second;

					//int add_baoji_per = skill_level_cfg->param_list[PARAM2];			// 增加暴击率 N%
					float add_gongji_per = skill_level_cfg->param_list[PARAM2] * SKILL_ATTR_PER;		// 攻击了 n%
					float add_speed_per = skill_level_cfg->param_list[PARAM3] * SKILL_ATTR_PER;		// 加速 M%
					int last_time_ms = skill_level_cfg->param_list[PARAM4] * 1000;						// 持续 X 毫秒

					if (add_speed_per > 0)
					{
						int add_movespeed = static_cast<int>(m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * add_speed_per);

						EffectMoveSpeed *effect_speed = new EffectMoveSpeed(m_role->GetId(), SHENSHOU_SKILL_TYPE_15, last_time_ms, EBT_JIASU, skill_item->level);
						effect_speed->SetMoveSpeed(add_movespeed, (short)(add_speed_per * 100));
						effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_6);
						m_role->AddEffect(effect_speed);
					}

					Attribute add_gongji = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI) * add_gongji_per);
					EffectBuff *effect_buff = new EffectBuff(INVALID_OBJ_ID, SHENSHOU_SKILL_TYPE_15, last_time_ms, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
					//effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI, add_baoji_per);
					effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, add_gongji);
					effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_6);
					effect_buff->SetSave(false);
					m_role->AddEffect(effect_buff);
				}
			}
		}
	}
}

void Shenshou::OnHurtOther(Character *target, long long injure)
{
	if (!this->CheckOpengameDay())
	{
		return;
	}

	unsigned int now_second = (unsigned int)EngineAdapter::Instance().Time();

	std::map<int, std::vector<SkillItem*> > skill_type_map; // 转换数据格式为：以技能类型作为key，因为技能触发是按照技能类型来，同类型的技能一次只触发一个

	for (std::map<int, std::vector<SkillItem> >::iterator skill_list_it = m_shenshou_active_skill_list.begin(); skill_list_it != m_shenshou_active_skill_list.end(); ++skill_list_it)
	{
		for (std::vector<SkillItem>::iterator skill_item_it = skill_list_it->second.begin(); skill_item_it != skill_list_it->second.end(); ++skill_item_it)
		{
			skill_type_map[skill_item_it->skill_type].push_back(&(*skill_item_it));
		}
	}

	for (std::map<int, std::vector<SkillItem*> >::iterator skill_list_it = skill_type_map.begin(); skill_list_it != skill_type_map.end(); ++skill_list_it)
	{
		for (std::vector<SkillItem*>::iterator skill_item_it = skill_list_it->second.begin(); skill_item_it != skill_list_it->second.end(); ++skill_item_it)
		{
			SkillItem *skill_item = *skill_item_it;

			const ShenshouSkillCfg::LevelCfg *skill_level_cfg = LOGIC_CONFIG->GetShenshouConfig().GetSkillLevelCfg(skill_item->skill_type, skill_item->level);
			if (NULL == skill_level_cfg) continue;

			if (SHENSHOU_SKILL_TYPE_17 == skill_item->skill_type)
			{
				if (now_second - skill_item->last_perform_time <= (unsigned int)skill_level_cfg->param_list[PARAM4]) // CD时间未到
				{
					continue;
				}
				skill_item->last_perform_time = now_second;

				if (RandomNum(10000) < skill_level_cfg->param_list[PARAM1]) // N% 的概率触发
				{
					Attribute cur_gongji = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI);
					Attribute dec_gongji = static_cast<Attribute>(cur_gongji * (skill_level_cfg->param_list[PARAM2] * SKILL_ATTR_PER)); // 减少当前攻击的 M%
					//Attribute cur_fa_gongji = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FA_GONGJI);
					//int dec_fa_gongji = static_cast<int>(cur_fa_gongji * (skill_level_cfg->param_list[PARAM2] * SKILL_ATTR_PER)); // 减少当前攻击的 M%
					int last_time_ms = skill_level_cfg->param_list[PARAM3] * 1000; // 持续 X 豪秒

					EffectBuff *effect_buff = new EffectBuff(INVALID_OBJ_ID, SHENSHOU_SKILL_TYPE_17, last_time_ms, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
					effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, -dec_gongji);
					//effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FA_GONGJI, -dec_fa_gongji);
					effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_6);
					effect_buff->SetSave(false);
					target->AddEffect(effect_buff);

					m_skill_last_perform_time_map[SHENSHOU_SKILL_TYPE_17] = now_second;
				}
			}
		}
	}
}

int Shenshou::GetHurtDecPer()
{
	std::map<int, std::vector<SkillItem*> > skill_type_map; // 转换数据格式为：以技能类型作为key，因为技能触发是按照技能类型来，同类型的技能一次只触发一个

	for (std::map<int, std::vector<SkillItem> >::iterator skill_list_it = m_shenshou_active_skill_list.begin(); skill_list_it != m_shenshou_active_skill_list.end(); ++skill_list_it)
	{
		for (std::vector<SkillItem>::iterator skill_item_it = skill_list_it->second.begin(); skill_item_it != skill_list_it->second.end(); ++skill_item_it)
		{
			skill_type_map[skill_item_it->skill_type].push_back(&(*skill_item_it));
		}
	}

	int hurt_dec_per = 0;

	for (std::map<int, std::vector<SkillItem*> >::iterator skill_list_it = skill_type_map.begin(); skill_list_it != skill_type_map.end(); ++skill_list_it)
	{
		for (std::vector<SkillItem*>::iterator skill_item_it = skill_list_it->second.begin(); skill_item_it != skill_list_it->second.end(); ++skill_item_it)
		{
			SkillItem *skill_item = *skill_item_it;

			const ShenshouSkillCfg::LevelCfg *skill_level_cfg = LOGIC_CONFIG->GetShenshouConfig().GetSkillLevelCfg(skill_item->skill_type, skill_item->level);
			if (NULL == skill_level_cfg) continue;

			if (SHENSHOU_SKILL_TYPE_18 == skill_item->skill_type)
			{
				Attribute max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				Attribute cur_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				if ((cur_hp * 1.0 / max_hp) * 10000 < skill_level_cfg->param_list[0]) // 血量百分比小于 N
				{
					hurt_dec_per += skill_level_cfg->param_list[1] / 100;
				}
			}
		}
	}

	return hurt_dec_per;
}

int Shenshou::GetHurtIncPercent()
{
	std::map<int, std::vector<SkillItem*> > skill_type_map; // 转换数据格式为：以技能类型作为key，因为技能触发是按照技能类型来，同类型的技能一次只触发一个

	for (std::map<int, std::vector<SkillItem> >::iterator skill_list_it = m_shenshou_active_skill_list.begin(); skill_list_it != m_shenshou_active_skill_list.end(); ++skill_list_it)
	{
		for (std::vector<SkillItem>::iterator skill_item_it = skill_list_it->second.begin(); skill_item_it != skill_list_it->second.end(); ++skill_item_it)
		{
			skill_type_map[skill_item_it->skill_type].push_back(&(*skill_item_it));
		}
	}

	int inc_dec_per = 0;

	for (std::map<int, std::vector<SkillItem*> >::iterator skill_list_it = skill_type_map.begin(); skill_list_it != skill_type_map.end(); ++skill_list_it)
	{
		for (std::vector<SkillItem*>::iterator skill_item_it = skill_list_it->second.begin(); skill_item_it != skill_list_it->second.end(); ++skill_item_it)
		{
			SkillItem *skill_item = *skill_item_it;

			const ShenshouSkillCfg::LevelCfg *skill_level_cfg = LOGIC_CONFIG->GetShenshouConfig().GetSkillLevelCfg(skill_item->skill_type, skill_item->level);
			if (NULL == skill_level_cfg) continue;

			bool skill_active = false;

			if (SHENSHOU_SKILL_TYPE_13 == skill_item->skill_type)
			{
				inc_dec_per += skill_level_cfg->param_list[PARAM1] / 100;
				skill_active = true;
			}
			else if (SHENSHOU_SKILL_TYPE_19 == skill_item->skill_type)
			{
				Attribute max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				Attribute cur_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				if ((cur_hp * 1.0 / max_hp) * 10000 < skill_level_cfg->param_list[PARAM1]) // 血量百分比小于 N
				{
					inc_dec_per += skill_level_cfg->param_list[PARAM2] / 100;
					skill_active = true;
				}
			}

			if (skill_active)
			{
				break;
			}
		}
	}

	return inc_dec_per;
}

bool Shenshou::IsMianYiXuanYun()
{
	if (!this->CheckOpengameDay())
	{
		return false;
	}

	std::map<int, std::vector<SkillItem*> > skill_type_map; // 转换数据格式为：以技能类型作为key，因为技能触发是按照技能类型来，同类型的技能一次只触发一个

	for (std::map<int, std::vector<SkillItem> >::iterator skill_list_it = m_shenshou_active_skill_list.begin(); skill_list_it != m_shenshou_active_skill_list.end(); ++skill_list_it)
	{
		for (std::vector<SkillItem>::iterator skill_item_it = skill_list_it->second.begin(); skill_item_it != skill_list_it->second.end(); ++skill_item_it)
		{
			skill_type_map[skill_item_it->skill_type].push_back(&(*skill_item_it));
		}
	}

	for (std::map<int, std::vector<SkillItem*> >::iterator skill_list_it = skill_type_map.begin(); skill_list_it != skill_type_map.end(); ++skill_list_it)
	{
		for (std::vector<SkillItem*>::iterator skill_item_it = skill_list_it->second.begin(); skill_item_it != skill_list_it->second.end(); ++skill_item_it)
		{
			SkillItem *skill_item = *skill_item_it;

			const ShenshouSkillCfg::LevelCfg *skill_level_cfg = LOGIC_CONFIG->GetShenshouConfig().GetSkillLevelCfg(skill_item->skill_type, skill_item->level);
			if (NULL == skill_level_cfg) continue;

			if (SHENSHOU_SKILL_TYPE_20 == skill_item->skill_type)
			{
				if (RandomNum(10000) < skill_level_cfg->param_list[0]) // N% 的概率触发
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool Shenshou::IsMianYiChenMo()
{
	if (!this->CheckOpengameDay())
	{
		return false;
	}

	std::map<int, std::vector<SkillItem*> > skill_type_map; // 转换数据格式为：以技能类型作为key，因为技能触发是按照技能类型来，同类型的技能一次只触发一个

	for (std::map<int, std::vector<SkillItem> >::iterator skill_list_it = m_shenshou_active_skill_list.begin(); skill_list_it != m_shenshou_active_skill_list.end(); ++skill_list_it)
	{
		for (std::vector<SkillItem>::iterator skill_item_it = skill_list_it->second.begin(); skill_item_it != skill_list_it->second.end(); ++skill_item_it)
		{
			skill_type_map[skill_item_it->skill_type].push_back(&(*skill_item_it));
		}
	}

	for (std::map<int, std::vector<SkillItem*> >::iterator skill_list_it = skill_type_map.begin(); skill_list_it != skill_type_map.end(); ++skill_list_it)
	{
		for (std::vector<SkillItem*>::iterator skill_item_it = skill_list_it->second.begin(); skill_item_it != skill_list_it->second.end(); ++skill_item_it)
		{
			SkillItem *skill_item = *skill_item_it;

			const ShenshouSkillCfg::LevelCfg *skill_level_cfg = LOGIC_CONFIG->GetShenshouConfig().GetSkillLevelCfg(skill_item->skill_type, skill_item->level);
			if (NULL == skill_level_cfg) continue;

			if (SHENSHOU_SKILL_TYPE_20 == skill_item->skill_type)
			{
				if (RandomNum(10000) < skill_level_cfg->param_list[0]) // N% 的概率触发
				{
					return true;
				}
			}
		}
	}

	return false;
}

// bool Shenshou::TransformFromKnapsackItem(UInt16 item_base_id, int item_base_num, short item_id, short strength_level, bool is_rand_start, int start_num, const char *reason)
// {
// 	if (!this->CheckOpengameDay())
// 	{
// 		return false;
// 	}
// 
// 	if (!this->EmptyBackpackNumMoreThan(item_base_num))
// 	{
// 		m_role->NoticeNum(errornum::EN_SHENSHOU_BACKPACK_NOT_ENOUGH);
// 		return false;
// 	}
// 
// 	bool is_succ = true;
// 	bool is_send = false;
// 	bool is_full_backpack = 0;
// 	for (int index = 0; index < item_base_num; index++)
// 	{
// 		if (index == item_base_num - 1) is_send = true;
// 		if (item_base_num != 1) is_full_backpack = 1;
// 		const ShenshouItemCfg *item_cfg = LOGIC_CONFIG->GetShenshouConfig().GetItemInfo(item_id);
// 		if (NULL == item_cfg)
// 		{
// 			return false;
// 		}
// 		const ShenshouEquipQualityCfg *quality_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipQualityCfg(item_cfg->quality);
// 		if (NULL == quality_cfg)
// 		{
// 			return false;
// 		}
// 
// 		static ShenshouBackpackItem tmp_item; tmp_item.Reset();
// 		tmp_item.item_id = item_id;
// 		tmp_item.strength_level = strength_level;
// 
// 		int star_attr_num = is_rand_start ? quality_cfg->GetRandomStartNum() : start_num;
// 		int attr_num = 0;
// 		// 生成随机属性
// 		std::set<int> start_attr_type_set;
// 		std::set<int> normal_attr_type_set;
// 		for (int i = 0; item_cfg->is_equip && i < quality_cfg->rand_attr_num && attr_num < SHENSHOU_MAX_EQUIP_ATTR_COUNT; ++i)
// 		{
// 			const ShenshouAttrCfg *attr_cfg;
// 			if (star_attr_num > 0)
// 			{
// 				--star_attr_num;
// 
// 				attr_cfg = quality_cfg->GetRandomStarAttr(start_attr_type_set);
// 				if (NULL != attr_cfg)
// 				{
// 					start_attr_type_set.insert(attr_cfg->attr_type);
// 					tmp_item.attr_list[attr_num].is_start = 1;
// 				}
// 			}
// 			else
// 			{
// 				attr_cfg = quality_cfg->GetRandomNormalAttr(normal_attr_type_set);
// 				if (NULL != attr_cfg) normal_attr_type_set.insert(attr_cfg->attr_type);
// 			}
// 
// 			if (NULL == attr_cfg) continue;
// 
// 			tmp_item.attr_list[attr_num].attr_type = attr_cfg->attr_type;
// 			tmp_item.attr_list[attr_num].attr_value = attr_cfg->attr_value;
// 			++attr_num;
// 		}
// 
// 		is_succ = is_succ && this->AddItem(tmp_item, reason, 1, is_send, is_full_backpack);
// 	}
// 	
// 	return is_succ;
// }

int Shenshou::PutOnEquip(ItemDataWrapper &item_wrapper, int shou_id)
{
	const ShenshouInfoCfg * info_cfg = LOGIC_CONFIG->GetShenshouConfig().GetShenshouInfo(shou_id);
	const ShenshouItemCfg * shouitem_cfg = LOGIC_CONFIG->GetShenshouConfig().GetItemInfo(item_wrapper.item_id,0);
	if (nullptr == info_cfg || nullptr == shouitem_cfg)
	{
		return -1;
	}

	const Equipment * equip= dynamic_cast<const Equipment*>(ITEMPOOL->GetItem(item_wrapper.item_id));
	if(nullptr == equip)
	{
		return -2;
	}

	int equip_type = equip->GetEquipType() - Equipment::E_TYPE_SHENSHOU_MIN;
	if (equip_type < 0 || equip_type >SHENSHOU_MAX_EQUIP_SLOT_INDEX)
	{
		return -3;
	}
	
	if (shouitem_cfg->quality < info_cfg->slot_need_quality[equip_type]) // 资质不够，不允许装备
	{
		m_role->NoticeNum(errornum::EN_SHENSHOU_QUALITY_NOT_ENOUGH);
		return -4;
	}

	if (m_param.shenshou_list[shou_id].equip_list[equip_type].item_id >0)
	{
		if (!this->TakeOffEquip(shou_id, equip_type, true))
		{
			return -5;
		}
	}

	ShenshouBackpackItem &slot_item = m_param.shenshou_list[shou_id].equip_list[equip_type];

	slot_item.item_id = shouitem_cfg->item_id;
	slot_item.strength_level = item_wrapper.param_data.strengthen_level;
	slot_item.shuliandu = item_wrapper.param_data.param1;

	gamelog::g_log_shenshou.printf(LL_INFO, "Shenshou::PutOnEquip user[%d, %s] shou_id[%d] slot[%d, %d] ",
		m_role->GetUID(), m_role->GetName(), shou_id, equip_type, m_param.shenshou_list[shou_id].equip_list[equip_type].item_id);

	// 人物大小目标
	if (this->IsActiveShenshou(shou_id))
	{
		TianShuXunZhu::CheckShenShouTianshu(m_role);
		m_role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENSHOU, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_3, shou_id);
	}

	this->SendShenshouInfo(shou_id);

	m_role->NoticeNum(noticenum::NT_JUHUN_PUTON_SUCC);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENSHOU);

	EventHandler::Instance().OnPutonShenshouEquip(m_role, shou_id, slot_item.item_id);

	return 0;
}

bool Shenshou::TakeOffEquip(int shou_id, int slot_index, bool is_puton)
{
	if (shou_id <= 0 || shou_id > SHENSHOU_MAX_ID || slot_index < 0 || slot_index > SHENSHOU_MAX_EQUIP_SLOT_INDEX)
	{
		return false;
	}

	ShenshouInfo &shenshou_info = m_param.shenshou_list[shou_id];
	ShenshouBackpackItem &tmp_item = m_param.shenshou_list[shou_id].equip_list[slot_index];
	if (tmp_item.item_id <= 0)
	{
		return false;
	}

	const ShenshouItemCfg * shou_item_cfg = LOGIC_CONFIG->GetShenshouConfig().GetItemInfo(tmp_item.item_id);
	if (nullptr == shou_item_cfg)
	{
		return false;
	}

	const Equipment * equip = dynamic_cast<const Equipment*>(ITEMPOOL->GetItem(shou_item_cfg->real_id));
	if (nullptr == equip)
	{
		return false;
	}

	int equip_type = equip->GetEquipType() - Equipment::E_TYPE_SHENSHOU_MIN;
	if (equip_type != slot_index)
	{
		return false;
	}

	Knapsack* knapsack = m_role->GetKnapsack();
	if(!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	ItemDataWrapper put_item;
	put_item.item_id = shou_item_cfg->real_id;
	//put_item.is_bind = false;
	put_item.is_bind = true;		//策划需求，装备穿上后变为绑定物品
	put_item.num = 1;
	put_item.has_param = 1;
	put_item.param_data.strengthen_level = tmp_item.strength_level;
	put_item.param_data.param1 = tmp_item.shuliandu;

	if (!knapsack->Put(put_item, PUT_REASON_NO_NOTICE))
	{
		return false;
	}

	tmp_item.Reset();

	if (!is_puton)
	{
		if (shenshou_info.has_zhuzhan > 0) // 脱下装备，变成未助战状态
		{
			shenshou_info.has_zhuzhan = 0;
			--m_cur_zhuzhan_count;

			// 删除助阵技能
			this->RecalcSkillList(shou_id, false);

			m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_SHENSHOU_COUNT); // 嘉年华
		}

		m_role->NoticeNum(noticenum::NT_JUHUN_TAKE_OFF_SUCC);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENSHOU);

		this->SendShenshouInfo(shou_id);
	}
	gamelog::g_log_shenshou.printf(LL_INFO, "Shenshou::TakeOffEquip user[%d, %s] shou_id[%d] slot[%d],real_id[%d],level[%d],shuliandu[%d] ",
		m_role->GetUID(), m_role->GetName(), shou_id, slot_index, put_item.item_id, put_item.param_data.strengthen_level, put_item.param_data.param1);
	return true;
}
void Shenshou::OnHandlerOpera(short opera_type, short param_1, short param_2, short param_3, int param_4)
{
	if (!this->CheckOpengameDay())
	{
		return;
	}

	switch (opera_type)
	{
	case Protocol::SHENSHOU_REQ_TYPE_ALL_INFO:
		{
			//this->SendBackpackInfo();
			this->SendShenshouInfo();
			this->SendShenshouBaseInfo();
		}
		break;

	case Protocol::SHENSHOU_REQ_TYPE_PUT_ON:
		{
			//this->PutOnEquip(param_1, param_2, param_3);
		}
		break;

	case Protocol::SHENSHOU_REQ_TYPE_TAKE_OFF:
		{
			this->TakeOffEquip(param_1, param_2,false);
		}
		break;

	case Protocol::SHENSHOU_REQ_TYPE_ZHUZHAN:
		{
			this->Zhuzhan(param_1);
		}
		break;

	case Protocol::SHENSHOU_REQ_TYPE_ADD_ZHUZHAN:
		{
			this->AddZhuzhan();
		}
		break;

	case Protocol::SHENSHOU_REQ_TYPE_COMPOSE:
		{
			this->Compose(param_1, param_2, param_3, param_4);
		}
		break;

	case Protocol::SHENSHOU_REQ_TYPE_HUANLING_INFO:
		{
			this->SendShenshouHuanlingInfo();
		}
		break;

	case Protocol::SHENSHOU_REQ_TYPE_HUANLING_REFRESH:
		{
			this->HuanlingRefresh();
		}
		break;

	case Protocol::SHENSHOU_REQ_TYPE_HUANLING_DRAW:
		{
			this->HuanlingDraw();
		}
		break;

	default: break;
	}
}

void Shenshou::OnGMReq(int type, int param1, int param2, int param3)
{
	/*
	/gm shenshou:1 id num				增加神兽装备 param1 装备ID， param2 数量
	 */
	enum Shenshou_GM_TYPE
	{
		Shenshou_GM_TYPE_AUTO_TEST = 0,			// 服务端自动测试
		Shenshou_GM_TYPE_ADD_EQUIP,				// 增加神兽装备
		Shenshou_GM_TYPE_CLEAR_BACKPACK,		// 清空神兽背包
		Shenshou_GM_TYPE_ADD_SCORE,				// 增加唤灵积分
		Shenshou_GM_TYPE_HUANLING_REFRESH,		// 唤灵刷新
		Shenshou_GM_TYPE_HUANLING_DRAW,			// 唤灵抽奖
	};

	switch (type)
	{
	case Shenshou_GM_TYPE_AUTO_TEST:
		{
#ifdef _DEBUG
			if (this->AutoTest())
			{
				gamelog::g_log_world.printf(LL_INFO, "Shenshou AutoTest Success.");
			}
			else
			{
				gamelog::g_log_world.printf(LL_INFO, "Shenshou AutoTest Fail.");
			}
#endif
		}
		break;

	case Shenshou_GM_TYPE_ADD_EQUIP:
		{
			//this->TransformFromKnapsackItem(0, 1, param1, param2, 0 == param3, param3, "NorexItem::Use");
		}
		break;

	case Shenshou_GM_TYPE_CLEAR_BACKPACK:
		{
// 			for (int i = 0; i < SHENSHOU_MAX_BACKPACK_COUNT; ++i)
// 			{
// 				m_param.backpack_item_list[i].Reset();
// 			}
// 			this->SendBackpackInfo();
		}
		break;

	case Shenshou_GM_TYPE_ADD_SCORE:
		{
			this->AddHunalingScore(param1);
		}
		break;

	case Shenshou_GM_TYPE_HUANLING_REFRESH:
		{
			this->HuanlingRefresh();
		}
		break;

	case Shenshou_GM_TYPE_HUANLING_DRAW:
		{
			this->HuanlingDraw();
		}
		break;

	default: break;
	}
}

// void Shenshou::UpgradeEquip(short shenshou_id, short equip_index, short destroy_num, short destroy_backpack_index_list[SHENSHOU_MAX_BACKPACK_COUNT], bool use_double_shuliandu)
// {
// 	if (shenshou_id <= 0 || shenshou_id > SHENSHOU_MAX_ID || equip_index < 0 || equip_index > SHENSHOU_MAX_EQUIP_SLOT_INDEX)
// 	{
// 		return;
// 	}
// 	if (destroy_num <= 0 || destroy_num > SHENSHOU_MAX_BACKPACK_COUNT)
// 	{
// 		return;
// 	}
// 
// 	int MAX_LEVEL = LOGIC_CONFIG->GetShenshouConfig().GetMaxEquipStrengthLevel();
// 	ShenshouBackpackItem &equip_item = m_param.shenshou_list[shenshou_id].equip_list[equip_index];
// 	const ShenshouItemCfg *item_cfg = LOGIC_CONFIG->GetShenshouConfig().GetItemInfo(equip_item.item_id);
// 	if (NULL == item_cfg)
// 	{
// 		return;
// 	}
// 
// 	if (equip_item.strength_level >= MAX_LEVEL)
// 	{
// 		m_role->NoticeNum(errornum::EN_SHENSHOU_EQUIP_STRENGTH_LEVEL_MAX);
// 		return;
// 	}
// 
// 	long long total_add_shuliandu = 0;	// 计算出消耗所有物品得到的总熟练度
// 	const ShenshouEquipQualityCfg::LevelShulianduCfg *now_shuliandu_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipShulianduCfg(equip_index, equip_item.strength_level);
// 	if (NULL == now_shuliandu_cfg)
// 	{
// 		return;
// 	}
// 	int now_contain_shulian = now_shuliandu_cfg->contain_shulian;	// 上级当强化材料时提升熟练度值contain_shulian
// 
// 	int shuliandu_max = LOGIC_CONFIG->GetShenshouConfig().GetEquipShulianduMax(equip_index);
// 	if (shuliandu_max < 0)
// 	{
// 		return;
// 	}
// 
// 	int consume_num = 0;
// 	bool is_overstep = false;
// 	for (; consume_num < destroy_num && consume_num < SHENSHOU_MAX_BACKPACK_COUNT; ++consume_num)
// 	{
// 		int backpack_index = destroy_backpack_index_list[consume_num];
// 		if (backpack_index < 0 || backpack_index >= SHENSHOU_MAX_BACKPACK_COUNT)
// 		{
// 			continue;
// 		}
// 		ShenshouBackpackItem &tmp_equip_item = m_param.backpack_item_list[backpack_index];
// 
// 		const ShenshouItemCfg *tmp_item_cfg = LOGIC_CONFIG->GetShenshouConfig().GetItemInfo(tmp_equip_item.item_id);
// 		if (NULL == tmp_item_cfg) continue;
// 
// 		if (tmp_item_cfg->is_equip)
// 		{
// 			const ShenshouEquipQualityCfg::LevelShulianduCfg *tmp_shuliandu_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipShulianduCfg(equip_index, tmp_equip_item.strength_level);
// 			if (NULL != tmp_shuliandu_cfg)
// 			{
// 				total_add_shuliandu += tmp_shuliandu_cfg->contain_shulian;
// 			}
// 
// 			total_add_shuliandu += tmp_item_cfg->contain_shulian;
// 			total_add_shuliandu += m_param.backpack_item_list[backpack_index].shuliandu; // 当前级的熟练度
// 		}
// 		else
// 		{
// 			total_add_shuliandu += tmp_item_cfg->contain_shulian;
// 		}
// 
// 		// 判断熟练度上限
// 		if (use_double_shuliandu)
// 		{
// 			if (total_add_shuliandu * 2 >= (shuliandu_max - (equip_item.shuliandu + now_contain_shulian)))
// 			{
// 				is_overstep = true;
// 				break;
// 			}
// 		}
// 		else
// 		{
// 			if (total_add_shuliandu >= (shuliandu_max - (equip_item.shuliandu + now_contain_shulian)))
// 			{
// 				is_overstep = true;
// 				break;
// 			}
// 		}
// 	}
// 
// 	if (is_overstep)
// 	{
// 		++consume_num;
// 	}
// 
// 	if (total_add_shuliandu <= 0)
// 	{
// 		return;
// 	}
// 
// 	long long left_shuliandu = total_add_shuliandu;
// 
// 	if (use_double_shuliandu)
// 	{
// 		left_shuliandu *= 2;
// 
// 		int need_gold = int(ceil(left_shuliandu * 1.0 / LOGIC_CONFIG->GetShenshouConfig().GetOtherCfg().equip_double_shulian_per_gold));
// 		if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "Shenshou::UpgradeEquip"))
// 		{
// 			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 			return;
// 		}
// 	}
// 
// 	ShenshouBackpackItem target_equip_item = equip_item; // 临时变量里升级
// 
// 	while (left_shuliandu > 0)
// 	{
// 		const ShenshouEquipQualityCfg::LevelShulianduCfg *cur_shuliandu_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipShulianduCfg(equip_index, target_equip_item.strength_level);
// 		const ShenshouEquipQualityCfg::LevelShulianduCfg *next_shuliandu_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipShulianduCfg(equip_index, target_equip_item.strength_level + 1);
// 		if (NULL == cur_shuliandu_cfg || NULL == next_shuliandu_cfg) // 若没有下一级配置，说明是最高级了
// 		{
// 			break;
// 		}
// 
// 		int upgrade_need_shuliandu = cur_shuliandu_cfg->upgrade_need_shulian - target_equip_item.shuliandu;
// 		if (left_shuliandu >= upgrade_need_shuliandu)
// 		{
// 			left_shuliandu -= upgrade_need_shuliandu;
// 
// 			++target_equip_item.strength_level;
// 			target_equip_item.shuliandu = 0;
// 
// 			if (target_equip_item.strength_level >= MAX_LEVEL)
// 			{
// 				target_equip_item.strength_level = MAX_LEVEL;
// 				break;
// 			}
// 		}
// 		else
// 		{
// 			target_equip_item.shuliandu += int(left_shuliandu);
// 			left_shuliandu = 0;
// 		}
// 	}
// 
// 	// 消耗物品
// 	for (int i = 0; i < consume_num && i < SHENSHOU_MAX_BACKPACK_COUNT; ++i)
// 	{
// 		int backpack_index = destroy_backpack_index_list[i];
// 		if (backpack_index < 0 || backpack_index >= SHENSHOU_MAX_BACKPACK_COUNT)
// 		{
// 			continue;
// 		}
// 		m_param.backpack_item_list[backpack_index].Reset();
// 	}
// 
// 	// 给予奖励升级
// 	equip_item = target_equip_item;
// 
// 	this->SendBackpackInfo();
// 	this->SendShenshouInfo(shenshou_id);
// 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENSHOU);
// }

void Shenshou::UpgradeEquip(short shenshou_id, short equip_index, short destroy_num, short destroy_backpack_index_list[SHENSHOU_MAX_BACKPACK_COUNT], bool use_double_shuliandu)
{
	if (shenshou_id <= 0 || shenshou_id > SHENSHOU_MAX_ID || equip_index < 0 || equip_index > SHENSHOU_MAX_EQUIP_SLOT_INDEX)
	{
		return;
	}	
	if (destroy_num <= 0 || destroy_num > SHENSHOU_MAX_BACKPACK_COUNT)
	{
		return;
	}

	int MAX_LEVEL = LOGIC_CONFIG->GetShenshouConfig().GetMaxEquipStrengthLevel();
	ShenshouBackpackItem &equip_item = m_param.shenshou_list[shenshou_id].equip_list[equip_index];
	const ShenshouItemCfg *item_cfg = LOGIC_CONFIG->GetShenshouConfig().GetItemInfo(equip_item.item_id);
	if (NULL == item_cfg)
	{
		return;
	}

	if (equip_item.strength_level >= MAX_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_SHENSHOU_EQUIP_STRENGTH_LEVEL_MAX);
		return;
	}

	long long total_add_shuliandu = 0;	// 计算出消耗所有物品得到的总熟练度
	const ShenshouEquipQualityCfg::LevelShulianduCfg *now_shuliandu_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipShulianduCfg(equip_index, equip_item.strength_level);
	if (NULL == now_shuliandu_cfg)
	{
		return;
	}
	int now_contain_shulian = now_shuliandu_cfg->contain_shulian;	// 上级当强化材料时提升熟练度值contain_shulian
	
	int shuliandu_max = LOGIC_CONFIG->GetShenshouConfig().GetEquipShulianduMax(equip_index);
	if (shuliandu_max < 0)
	{
		return;
	}

	Knapsack * knapasck = m_role->GetKnapsack();
	if (knapasck->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	// 客户端发送的背包索引不能重复，重复会造成数据被刷。所以直接存入set表，取一个有效的就行
	std::set<short> destroy_backpack_index_set;
	for (int i = 0; i < destroy_num && i < SHENSHOU_MAX_BACKPACK_COUNT; ++i)
	{
		int backpack_idx = destroy_backpack_index_list[i];
		if (backpack_idx < 0 || backpack_idx >= ItemNamespace::MAX_KNAPSACK_GRID_NUM)
		{
			continue;
		}
		destroy_backpack_index_set.insert(backpack_idx);
	}


	bool is_overstep = false;
	std::map<int,int> consum_index;
	//for (int consume_num = 0; consume_num < destroy_num && consume_num < SHENSHOU_MAX_BACKPACK_COUNT; ++consume_num)
	for (auto &it : destroy_backpack_index_set)
	{
		//int backpack_index = destroy_backpack_index_list[consume_num];
		int backpack_index = it;
		if (backpack_index < 0 || backpack_index >= ItemNamespace::MAX_KNAPSACK_GRID_NUM)
		{
			continue;
		}

		ItemGridData * tmp_equip_item = knapasck->GetItemGridData(backpack_index);
		if (nullptr == tmp_equip_item){
			continue;
		}

		
		EquipmentParam* equip_param = dynamic_cast<EquipmentParam*>(tmp_equip_item->param);
		if (nullptr == equip_param)
		{
			const NorexItem* nore_item = dynamic_cast<const NorexItem*>(ITEMPOOL->GetItem(tmp_equip_item->item_id));
			if (nullptr == nore_item || NorexItem::I_NOREX_ITEM_SHENSHOU != nore_item->GetUseType())
			{
				continue;
			}
		}

		const ShenshouItemCfg *tmp_item_cfg = LOGIC_CONFIG->GetShenshouConfig().GetItemInfo(tmp_equip_item->item_id,0);
		if (nullptr == tmp_item_cfg) continue;

		if (tmp_item_cfg->is_equip)
		{
			const ShenshouEquipQualityCfg::LevelShulianduCfg *tmp_shuliandu_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipShulianduCfg(equip_index, equip_param->strengthen_level);
			if (NULL != tmp_shuliandu_cfg)
			{
				total_add_shuliandu += tmp_shuliandu_cfg->contain_shulian;
			}

			//total_add_shuliandu += tmp_item_cfg->contain_shulian;
			total_add_shuliandu += equip_param->param1; // 当前级的熟练度
		}

		consum_index[backpack_index] = 0;
		// 判断熟练度上限
		for (int i = 0; i < tmp_equip_item->num; ++i)
		{
			consum_index[backpack_index]++;

			total_add_shuliandu += tmp_item_cfg->contain_shulian;
			if (use_double_shuliandu)
			{
				if (total_add_shuliandu * 2 >= (shuliandu_max - (equip_item.shuliandu + now_contain_shulian)))
				{
					is_overstep = true;
					break;
				}
			}
			else
			{
				if (total_add_shuliandu >= (shuliandu_max - (equip_item.shuliandu + now_contain_shulian)))
				{
					is_overstep = true;
					break;
				}
			}
		}

		if (is_overstep)
		{
			break;
		}
	}

	if (total_add_shuliandu <= 0)
	{
		return;
	}

	long long left_shuliandu = total_add_shuliandu;

	if (use_double_shuliandu)
	{
		left_shuliandu *= 2;

		int need_gold = int(ceil(left_shuliandu * 1.0 / LOGIC_CONFIG->GetShenshouConfig().GetOtherCfg().equip_double_shulian_per_gold));
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "Shenshou::UpgradeEquip"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}

	ShenshouBackpackItem target_equip_item = equip_item; // 临时变量里升级
	
	while (left_shuliandu > 0)
	{
		const ShenshouEquipQualityCfg::LevelShulianduCfg *cur_shuliandu_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipShulianduCfg(equip_index, target_equip_item.strength_level);
		const ShenshouEquipQualityCfg::LevelShulianduCfg *next_shuliandu_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipShulianduCfg(equip_index, target_equip_item.strength_level + 1);
		if (NULL == cur_shuliandu_cfg || NULL == next_shuliandu_cfg) // 若没有下一级配置，说明是最高级了
		{
			break;
		}

		int upgrade_need_shuliandu = cur_shuliandu_cfg->upgrade_need_shulian - target_equip_item.shuliandu;
		if (left_shuliandu >= upgrade_need_shuliandu)
		{
			left_shuliandu -= upgrade_need_shuliandu;

			++target_equip_item.strength_level;
			target_equip_item.shuliandu = 0;

			if (target_equip_item.strength_level >= MAX_LEVEL)
			{
				target_equip_item.strength_level = MAX_LEVEL;
				break;
			}
		}
		else
		{
			target_equip_item.shuliandu += int(left_shuliandu);
			left_shuliandu = 0;
		}
	}

	// 消耗物品
	for (auto it = consum_index.begin(); it != consum_index.end();++it )
	{
		//knapasck->Discard(it->first, it->second, DISCARD_METHOD_RECYCL);
		knapasck->ConsumeItemByIndex(it->first, it->second, "Shenshou::UpgradeEquip");
	}

	// 给予奖励升级
	equip_item = target_equip_item;

	//this->SendBackpackInfo();
	this->SendShenshouInfo(shenshou_id);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENSHOU);
}

void Shenshou::AddHunalingScore(int add_score)
{
	if (add_score > 0)
	{
		m_param.huanling_score += add_score;
		this->SendShenshouHuanlingInfo();
	}
}

// bool Shenshou::AddItem(ShenshouBackpackItem &tmp_item, const char *reason, int num, bool is_send, char is_full_backpack)
// {
// 	if (NULL == reason || num <= 0)
// 	{
// 		return false;
// 	}
// 
// 	if (!this->EmptyBackpackNumMoreThan(num))
// 	{
// 		m_role->NoticeNum(errornum::EN_SHENSHOU_BACKPACK_NOT_ENOUGH);
// 		return false;
// 	}
// 
// 	if (NULL == LOGIC_CONFIG->GetShenshouConfig().GetItemInfo(tmp_item.item_id))
// 	{
// 		return false;
// 	}
// 
// 	gamelog::g_log_shenshou.buff_printf("Shenshou::AddItem user[%d, %s] item_id[%d] reason[%s] backpack_index_list[",
// 		m_role->GetUID(), m_role->GetName(), tmp_item.item_id, reason);
// 
// 	int left_num = num;
// 	for (int b_index = 0; b_index < SHENSHOU_MAX_BACKPACK_COUNT; ++b_index)
// 	{
// 		if (m_param.backpack_item_list[b_index].item_id <= 0) // 找到空格子，放进去
// 		{
// 			m_param.backpack_item_list[b_index] = tmp_item;
// 			if (is_send)
// 			{
// 				if (is_full_backpack == 0)
// 				{
// 					this->SendBackpackAdditionInfo(b_index);
// 				}
// 				else
// 				{
// 					this->SendBackpackInfo();
// 				}	
// 			}
// 			
// 			gamelog::g_log_shenshou.buff_printf("%d, ", b_index);
// 
// 			--left_num;
// 			if (left_num <= 0)
// 			{
// 				break;
// 			}
// 		}
// 	}
// 
// 	gamelog::g_log_shenshou.commit_buff(LL_INFO);
// 	
// 	//this->SendBackpackInfo();
// 	return true; // 前面检查过背包格子数足够的了，一般不会出现剩余物品没放入的情况
// }

#define SHENSHOU_SKILL_ADD_ATTR_PER(ATTR_TYPE, PERCENT) \
char_extra_attrs.m_attrs[ATTR_TYPE] += static_cast<Attribute>(char_int_attrs.m_attrs[ATTR_TYPE] * (PERCENT / 10000.0));

#define SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(ATTR_TYPE, ATTR_VALUE) \
role_attrs.m_attrs[ATTR_TYPE] += ATTR_VALUE;
 
void Shenshou::RecalcSkillAttrForSingleShou(const CharIntAttrs & char_int_attrs, CharIntAttrs & char_extra_attrs, int skill_id, const int param_list[])
{
	UNSTD_STATIC_CHECK(Shenshou::PARAM_COUNT == ShenshouSkillCfg::PARAM_COUNT);

	switch (skill_id)
	{
	case SHENSHOU_SKILL_TYPE_1:
		{
			// 1级当前神兽（包含装备）防御力+20%；
			SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, param_list[PARAM1]);
			SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, param_list[PARAM2]);
		}
		break;

	case SHENSHOU_SKILL_TYPE_2:
		{
			SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, param_list[PARAM1]);
		}
		break;

	case SHENSHOU_SKILL_TYPE_3:
		{
			SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, param_list[PARAM1]);
			//SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_FA_GONGJI, param_list[PARAM1]);
		}
		break;

	case SHENSHOU_SKILL_TYPE_21:
	{
		SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, param_list[PARAM1]);
	}
	break;

	default: break;
	}
}

void Shenshou::RecalcSkillAttrForAllZhuzhanShou(const CharIntAttrs &char_int_attrs, CharIntAttrs & char_extra_attrs, int skill_id, const int param_list[])
{
	switch (skill_id)
	{
	case SHENSHOU_SKILL_TYPE_4:
		{
			SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, param_list[PARAM1]);
			SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, param_list[PARAM1]);
		}
		break;

	case SHENSHOU_SKILL_TYPE_5:
		{
			SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, param_list[PARAM1]);
		}
		break;

	case SHENSHOU_SKILL_TYPE_6:
		{
			SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, param_list[PARAM1]);
			//SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_FA_GONGJI, param_list[PARAM1]);
		}
		break;

	case SHENSHOU_SKILL_TYPE_7:
		{
			SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, param_list[PARAM1]);
			SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, param_list[PARAM2]);
			SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, param_list[PARAM3]);
			//SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_FA_GONGJI, param_list[PARAM3]);
			SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, param_list[PARAM4]);
			SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, param_list[PARAM5]);
		}
		break;

	default: break;
	}
}

void Shenshou::RecalcSkillAttrForRoleBaseAttr(CharIntAttrs &role_attrs, int skill_id, const int param_list[])
{
	switch (skill_id)
	{
	case SHENSHOU_SKILL_TYPE_14:
		{
			SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT, param_list[PARAM1]);
		}
		break;

	default: break;
	}
}

void Shenshou::RecalcSkillAttrForRoleAttr(CharIntAttrs & role_attrs, int skill_id, const int param_list[])
{
	switch (skill_id)
	{
	case SHENSHOU_SKILL_TYPE_22:  // 全身攻击 + X%
	{
		role_attrs.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<float>(param_list[PARAM1] * ROLE_ATTR_PER);
	}
	break;

	case SHENSHOU_SKILL_TYPE_23:  // 全身生命 + X%
	{
		role_attrs.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(param_list[PARAM1] * ROLE_ATTR_PER);
	}
	break;

	case SHENSHOU_SKILL_TYPE_24:  // 技能增伤 + X%
	{
		SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER, param_list[PARAM1]);
	}
	break;

	case SHENSHOU_SKILL_TYPE_25:  // 技能减伤 + X%
	{
		SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER, param_list[PARAM1]);
	}
	break;

	case SHENSHOU_SKILL_TYPE_26:  // 伤害加成 + X%
	{
//		SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, param_list[PARAM1]);
		role_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, param_list[PARAM1]);
	}
	break;

	case SHENSHOU_SKILL_TYPE_27:  // 伤害减免 + X%
	{
//		SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, param_list[PARAM1]);
		role_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, param_list[PARAM1]);
	}
	break;

	case SHENSHOU_SKILL_TYPE_28:  // 格挡几率 + X%
	{
		SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_PER, param_list[PARAM1]);
	}
	break;

	case SHENSHOU_SKILL_TYPE_29:  // 命中几率 + X%
	{
		SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG_PER, param_list[PARAM1]);
	}
	break;

	case SHENSHOU_SKILL_TYPE_30:  // 伤害加成 + X%
	{
//		SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, param_list[PARAM1]);
		role_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, param_list[PARAM1]);
	}
	break;

	default: break;
	}
}

void Shenshou::CalcRandomAttr(const CharIntAttrs & char_int_attrs, CharIntAttrs & char_extra_attrs, CharIntAttrs & role_attrs, int attr_type, int attr_value)
{
	switch (attr_type)
	{
	case SHENSHOU_ATTR_TYPE_PER_HP:
		SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_PER_GONGJI:
		SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, attr_value);
		//SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_FA_GONGJI, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_PER_FANGYU:
		SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_PER_SHANBI:
		SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_PER_MINGZHONG:
		SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_PER_BAOJI:
		SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_PER_JIANREN:
		SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_PER_GUSHANG:
		SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_PER_JIANSHANG:
		SHENSHOU_SKILL_ADD_ATTR_PER(CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_MAX_HP:
		SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_GONGJI:
		SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, attr_value);
		//SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_FA_GONGJI, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_FANGYU:
		SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_SHANBI:
		SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_MINGZHONG:
		SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_BAOJI:
		SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_JIANREN:
		SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_GUSHANG:
		SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG, attr_value);
		break;

	case SHENSHOU_ATTR_TYPE_JIANSHANG:
		SHENSHOU_SKILL_ADD_ATTR_TO_ROLE(CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG, attr_value);
		break;

	default: break;
	}

}

bool Shenshou::IsActiveShenshou(int shenshou_id) const
{
	if (!this->CheckOpengameDay())
	{
		return false;
	}

	if (shenshou_id <= 0 || shenshou_id > SHENSHOU_MAX_ID)
	{
		return false;
	}

	const ShenshouInfo &shenshou_info = m_param.shenshou_list[shenshou_id];
	for (int index = 0; index <= SHENSHOU_MAX_EQUIP_SLOT_INDEX; ++index)
	{
		if (shenshou_info.equip_list[index].item_id <= 0) // 有一个未装备就不能助战
		{
			return false;
		}
	}

	return true;
}

// bool Shenshou::EmptyBackpackNumMoreThan(int check_num) const
// {
// 	if (check_num <= 0)
// 	{
// 		return true;
// 	}
// 
// 	for (int index = 0; index < SHENSHOU_MAX_BACKPACK_COUNT; ++index)
// 	{
// 		if (m_param.backpack_item_list[index].item_id <= 0)
// 		{
// 			if (--check_num <= 0)
// 			{
// 				return true;
// 			}
// 		}
// 	}
// 
// 	return false;
// }

int Shenshou::GetAttrType(int Shenshou_attr_type)
{
	int attr_type = CharIntAttrs::CHARINTATTR_TYPE_INVALID;

	//switch (Shenshou_attr_type)
	//{
	//case SHENSHOU_ATTR_TYPE_PER_HP:
	//	attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MAXHP;
	//	break;

	//case SHENSHOU_ATTR_TYPE_PER_GONGJI:
	//	attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_GONGJI;
	//	break;

	//case SHENSHOU_ATTR_TYPE_PER_FANGYU:
	//	attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_FANGYU;
	//	break;

	//case SHENSHOU_ATTR_TYPE_PER_FA_FANGYU:
	//	attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_FA_FANGYU;
	//	break;

	//case SHENSHOU_ATTR_TYPE_PER_POJIA:
	//	attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POJIA;
	//	break;

	//case SHENSHOU_ATTR_TYPE_PER_WUXING_GONGJI:
	//	attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI;
	//	break;

	//case SHENSHOU_ATTR_TYPE_PER_WUXING_FANGYU:
	//	attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN;
	//	break;

	//case SHENSHOU_ATTR_TYPE_MAX_HP:
	//	attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP;
	//	break;

	//case SHENSHOU_ATTR_TYPE_GONGJI:
	//	attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI;
	//	break;

	//case SHENSHOU_ATTR_TYPE_FANGYU:
	//	attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU;
	//	break;

	//case SHENSHOU_ATTR_TYPE_FA_FANGYU:
	//	attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI;
	//	break;

	//case SHENSHOU_ATTR_TYPE_POJIA:
	//	attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG;
	//	break;

	//case SHENSHOU_ATTR_TYPE_WUXING_GONGJI:
	//	attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI;
	//	break;

	//case SHENSHOU_ATTR_TYPE_WUXING_FANGYU:
	//	attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN;
	//	break;

	//case SHENSHOU_ATTR_TYPE_PER_COINT_DROP:
	//	attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG;
	//	break;


	//default: break;
	//}

	return attr_type;
}

// void Shenshou::PutOnEquip(int shou_id, int backpack_index, int slot_index)
// {
// 	if (shou_id <= 0 || shou_id > SHENSHOU_MAX_ID || slot_index < 0 || slot_index > SHENSHOU_MAX_EQUIP_SLOT_INDEX || backpack_index < 0 || backpack_index >= SHENSHOU_MAX_BACKPACK_COUNT)
// 	{
// 		return;
// 	}
// 
// 	ShenshouBackpackItem &backpack_item = m_param.backpack_item_list[backpack_index];
// 	const ShenshouItemCfg *item_cfg = LOGIC_CONFIG->GetShenshouConfig().GetItemInfo(backpack_item.item_id);
// 	if (NULL == item_cfg)
// 	{
// 		return;
// 	}
// 
// 	if (!item_cfg->is_equip)
// 	{
// 		m_role->NoticeNum(errornum::EN_SHENSHOU_ITEM_IS_NOT_EQUIP);
// 		return;
// 	}
// 
// 	if (item_cfg->slot_index != slot_index)
// 	{
// 		return;
// 	}
// 
// 	const ShenshouInfoCfg *shou_cfg = LOGIC_CONFIG->GetShenshouConfig().GetShenshouInfo(shou_id);
// 	if (NULL == shou_cfg)
// 	{
// 		return;
// 	}
// 
// 	if (item_cfg->quality < shou_cfg->slot_need_quality[slot_index]) // 资质不够，不允许装备
// 	{
// 		m_role->NoticeNum(errornum::EN_SHENSHOU_QUALITY_NOT_ENOUGH);
// 		return;
// 	}
// 
// 	gamelog::g_log_shenshou.printf(LL_INFO, "Shenshou::PutOnEquip user[%d, %s] shou_id[%d] slot[%d, %d] backpack_index[%d, %d]",
// 		m_role->GetUID(), m_role->GetName(), shou_id, slot_index, m_param.shenshou_list[shou_id].equip_list[slot_index].item_id, backpack_index, m_param.backpack_item_list[backpack_index].item_id);
// 
// 	// 装备槽跟背包格子是一样的，直接交换位置就行
// 	ShenshouBackpackItem tmp_item = m_param.backpack_item_list[backpack_index];
// 	m_param.backpack_item_list[backpack_index] = m_param.shenshou_list[shou_id].equip_list[slot_index];
// 	m_param.shenshou_list[shou_id].equip_list[slot_index] = tmp_item;
// 
// 	// 人物大小目标
// 	if (this->IsActiveShenshou(shou_id))
// 	{
// 		m_role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENSHOU, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_3, shou_id);
// 	}
// 	
// 	this->SendBackpackInfo();
// 	this->SendShenshouInfo(shou_id);
// 
// 	m_role->NoticeNum(noticenum::NT_JUHUN_PUTON_SUCC);
// 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENSHOU);
// }

// void Shenshou::TakeOffEquip(int shou_id, int slot_index)
// {
// 	if (shou_id <= 0 || shou_id > SHENSHOU_MAX_ID || slot_index < 0 || slot_index > SHENSHOU_MAX_EQUIP_SLOT_INDEX)
// 	{
// 		return;
// 	}
// 
// 	ShenshouInfo &shenshou_info = m_param.shenshou_list[shou_id];
// 	ShenshouBackpackItem &tmp_item = m_param.shenshou_list[shou_id].equip_list[slot_index];
// 	if (tmp_item.item_id <= 0)
// 	{
// 		return;
// 	}
// 
// 	if (!this->AddItem(tmp_item, "Shenshou::TakeOffEquip"))
// 	{
// 		m_role->NoticeNum(errornum::EN_SHENSHOU_BACKPACK_NOT_ENOUGH);
// 		return;
// 	}
// 
// 	tmp_item.Reset();
// 
// 	if (shenshou_info.has_zhuzhan > 0) // 脱下装备，变成未助战状态
// 	{
// 		shenshou_info.has_zhuzhan = 0;
// 		--m_cur_zhuzhan_count;
// 
// 		// 删除助阵技能
// 		this->RecalcSkillList(shou_id, false);
// 
// 		m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_SHENSHOU_COUNT); // 嘉年华
// 	}
// 
// 	m_role->NoticeNum(noticenum::NT_JUHUN_TAKE_OFF_SUCC);
// 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENSHOU);
// 
// 	//this->SendBackpackInfo();
// 	this->SendShenshouInfo(shou_id);
// }

void Shenshou::Zhuzhan(int shou_id)
{
	if (shou_id <= 0 || shou_id > SHENSHOU_MAX_ID)
	{
		return;
	}

	ShenshouInfo &shenshou_info = m_param.shenshou_list[shou_id];

	int max_zhuzhan_count = LOGIC_CONFIG->GetShenshouConfig().GetOtherCfg().default_zhuzhan_count + m_param.extra_zhuzhan_count;
	if (shenshou_info.has_zhuzhan <= 0 && m_cur_zhuzhan_count >= max_zhuzhan_count)
	{
		m_role->NoticeNum(errornum::EN_SHENSHOU_ZHUZHAN_COUNT_LIMIT);
		return;
	}

	if (!this->IsActiveShenshou(shou_id))
	{
		m_role->NoticeNum(errornum::EN_SHENSHOU_NOT_ACTIVE);
		return;
	}

	if (shenshou_info.has_zhuzhan > 0)
	{
		shenshou_info.has_zhuzhan = 0;
		--m_cur_zhuzhan_count;
		this->RecalcSkillList(shou_id, false);
	}
	else
	{
		shenshou_info.has_zhuzhan = 1;
		++m_cur_zhuzhan_count;
		this->RecalcSkillList(shou_id, true);
	}
	this->SendShenshouInfo(shou_id);

	m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_SHENSHOU_COUNT); // 嘉年华

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENSHOU);
}

void Shenshou::AddZhuzhan()
{
	const ShenshouZhuzhanCfg *zhuzhan_cfg = LOGIC_CONFIG->GetShenshouConfig().GetExtraZhuzhanCfg(m_param.extra_zhuzhan_count + 1);
	if (NULL == zhuzhan_cfg)
	{
		return;
	}

	if (m_role->GetKnapsack()->Count(zhuzhan_cfg->stuff_id) < zhuzhan_cfg->stuff_num)
	{
		m_role->NoticeNum(errornum::EN_ITEM_NUM_NOT_ENOUGH);
		return;
	}

	m_role->GetKnapsack()->ConsumeItem(zhuzhan_cfg->stuff_id, zhuzhan_cfg->stuff_num, "Shenshou::AddZhuzhan");
	++m_param.extra_zhuzhan_count;
	
	//EventHandler::Instance().OnShenshouAddZhuzhan(m_role, m_param.extra_zhuzhan_count);
	this->SendShenshouBaseInfo();
}

void Shenshou::Compose(int v_item_id, int index_1, int index_2, int index_3)
{
// 	if (index_1 < 0 || index_1 >= SHENSHOU_MAX_BACKPACK_COUNT)
// 	{
// 		return;
// 	}
// 
// 	if (index_2 < 0 || index_2 >= SHENSHOU_MAX_BACKPACK_COUNT)
// 	{
// 		return;
// 	}
// 
// 	if (index_3 < 0 || index_3 >= SHENSHOU_MAX_BACKPACK_COUNT)
// 	{
// 		return;
// 	}
// 
// 	if (index_1 == index_2 || index_1 == index_3 || index_2 == index_3)
// 	{
// 		return;
// 	}
// 
// 	const ShenshouComposeCfg *compose_cfg = LOGIC_CONFIG->GetShenshouConfig().GetComposeCfg(v_item_id);
// 	const ShenshouItemCfg *target_equip = LOGIC_CONFIG->GetShenshouConfig().GetItemInfo(v_item_id);
// 	if (NULL == compose_cfg || NULL == target_equip)
// 	{
// 		return;
// 	}
// 
// 	
// 	ShenshouBackpackItem &tmp_equip_item1 = m_param.backpack_item_list[index_1];
// 	ShenshouBackpackItem &tmp_equip_item2 = m_param.backpack_item_list[index_2];
// 	ShenshouBackpackItem &tmp_equip_item3 = m_param.backpack_item_list[index_3];
// 
// 	// 检查星数
// 	int start_num = 0;
// 	for (int i = 0; i < SHENSHOU_MAX_EQUIP_ATTR_COUNT; ++i)
// 	{
// 		if (0 == tmp_equip_item1.attr_list[i].is_start)
// 		{
// 			break;
// 		}
// 
// 		if (0 == tmp_equip_item2.attr_list[i].is_start)
// 		{
// 			break;
// 		}
// 
// 		if (0 == tmp_equip_item3.attr_list[i].is_start)
// 		{
// 			break;
// 		}
// 
// 		++start_num;
// 	}
// 
// 	if (start_num != compose_cfg->need_start_num)
// 	{
// 		return;
// 	}
// 
// 	// 检查配置
// 	const ShenshouItemCfg *tmp_item_cfg1 = LOGIC_CONFIG->GetShenshouConfig().GetItemInfo(tmp_equip_item1.item_id);
// 	if (NULL == tmp_item_cfg1 || !tmp_item_cfg1->is_equip) return;
// 
// 	const ShenshouItemCfg *tmp_item_cfg2 = LOGIC_CONFIG->GetShenshouConfig().GetItemInfo(tmp_equip_item2.item_id);
// 	if (NULL == tmp_item_cfg2 || !tmp_item_cfg2->is_equip) return;
// 
// 	const ShenshouItemCfg *tmp_item_cfg3 = LOGIC_CONFIG->GetShenshouConfig().GetItemInfo(tmp_equip_item3.item_id);
// 	if (NULL == tmp_item_cfg3 || !tmp_item_cfg3->is_equip) return;
// 
// 	// 获取装备部位并确定上一级所得熟练度
// 	const ShenshouEquipQualityCfg::LevelShulianduCfg *item1_shuliandu_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipShulianduCfg(tmp_item_cfg1->slot_index, tmp_equip_item1.strength_level);		// 上级当强化材料时提升熟练度值contain_shulian
// 	const ShenshouEquipQualityCfg::LevelShulianduCfg *item2_shuliandu_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipShulianduCfg(tmp_item_cfg2->slot_index, tmp_equip_item2.strength_level);
// 	const ShenshouEquipQualityCfg::LevelShulianduCfg *item3_shuliandu_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipShulianduCfg(tmp_item_cfg3->slot_index, tmp_equip_item3.strength_level);
// 	if (NULL == item1_shuliandu_cfg || NULL == item2_shuliandu_cfg || NULL == item3_shuliandu_cfg)
// 	{
// 		return;
// 	}
// 
// 	// 统计要增加的熟练度
// 	long long total_add_shuliandu = 0; // 计算出消耗所有物品得到的总熟练度
// 	total_add_shuliandu += (tmp_equip_item1.shuliandu + item1_shuliandu_cfg->contain_shulian); // 当前级获得的熟练度
// 	total_add_shuliandu += (tmp_equip_item2.shuliandu + item2_shuliandu_cfg->contain_shulian); // 当前级获得的熟练度
// 	total_add_shuliandu += (tmp_equip_item3.shuliandu + item3_shuliandu_cfg->contain_shulian); // 当前级获得的熟练度
// 
// 	long long left_shuliandu = total_add_shuliandu;
// 	ShenshouBackpackItem target_equip_item; 
// 	target_equip_item.Reset();
// 	target_equip_item.item_id = v_item_id;
// 
// 	// 生成属性
// 	const ShenshouEquipQualityCfg *quality_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipQualityCfg(compose_cfg->give_quality);
// 	if (NULL == quality_cfg) return;
// 
// 	int star_attr_num = compose_cfg->give_start_num;
// 	int attr_num = 0;
// 	std::set<int> start_attr_type_set;
// 	std::set<int> normal_attr_type_set;
// 	for (int i = 0; i < quality_cfg->rand_attr_num && attr_num < SHENSHOU_MAX_EQUIP_ATTR_COUNT; ++i)
// 	{
// 		const ShenshouAttrCfg *attr_cfg;
// 		if (star_attr_num > 0)
// 		{
// 			--star_attr_num;
// 
// 			attr_cfg = quality_cfg->GetRandomStarAttr(start_attr_type_set);
// 			if (NULL != attr_cfg)
// 			{
// 				start_attr_type_set.insert(attr_cfg->attr_type);
// 				target_equip_item.attr_list[attr_num].is_start = 1;
// 			}
// 		}
// 		else
// 		{
// 			attr_cfg = quality_cfg->GetRandomNormalAttr(normal_attr_type_set);
// 			if (NULL != attr_cfg) normal_attr_type_set.insert(attr_cfg->attr_type);
// 		}
// 
// 		if (NULL == attr_cfg) continue;
// 
// 		target_equip_item.attr_list[attr_num].attr_type = attr_cfg->attr_type;
// 		target_equip_item.attr_list[attr_num].attr_value = attr_cfg->attr_value;
// 		++attr_num;
// 	}
// 
// 	// 临时变量里升级
// 	int MAX_LEVEL = LOGIC_CONFIG->GetShenshouConfig().GetMaxEquipStrengthLevel();
// 	while (left_shuliandu > 0)
// 	{
// 		const ShenshouEquipQualityCfg::LevelShulianduCfg *cur_shuliandu_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipShulianduCfg(target_equip->slot_index, target_equip_item.strength_level);
// 		const ShenshouEquipQualityCfg::LevelShulianduCfg *next_shuliandu_cfg = LOGIC_CONFIG->GetShenshouConfig().GetEquipShulianduCfg(target_equip->slot_index, target_equip_item.strength_level + 1);
// 		if (NULL == cur_shuliandu_cfg || NULL == next_shuliandu_cfg) // 若没有下一级配置，说明是最高级了
// 		{
// 			break;
// 		}
// 
// 		int upgrade_need_shuliandu = cur_shuliandu_cfg->upgrade_need_shulian - target_equip_item.shuliandu;
// 		if (left_shuliandu >= upgrade_need_shuliandu)
// 		{
// 			left_shuliandu -= upgrade_need_shuliandu;
// 
// 			++target_equip_item.strength_level;
// 			target_equip_item.shuliandu = 0;
// 			
// 			if (target_equip_item.strength_level >= MAX_LEVEL)
// 			{
// 				target_equip_item.strength_level = MAX_LEVEL;
// 				break;
// 			}
// 		}
// 		else
// 		{
// 			target_equip_item.shuliandu += int(left_shuliandu);
// 			left_shuliandu = 0;
// 		}
// 	}
// 
// 	// 消耗物品
// 	if (compose_cfg->is_need_item && !m_role->GetKnapsack()->ConsumeItem(compose_cfg->item_id, compose_cfg->item_num, __FUNCTION__))
// 	{
// 		m_role->NoticeNum(errornum::EN_ITEM_NOT_ENOUGH);
// 		return;
// 	}
// 	tmp_equip_item1.Reset();
// 	tmp_equip_item2.Reset();
// 	tmp_equip_item3.Reset();
// 
// 	// 给予奖励升级
// 	this->AddItem(target_equip_item, "Shenshou::Compose", 1);
// //	this->SendBackpackInfo();
// 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENSHOU);
// 
// 	m_role->NoticeNum(noticenum::NT_EQUIP_COMPOSE_SUCC);
// 
// 	if (compose_cfg->is_broatcast)
// 	{
// 		// 传闻
// 		static char equip_param_string[1024] = { 0 };
// 		memset(equip_param_string, 0, sizeof(equip_param_string));
// 
// 		{
// 			SNPRINTF(equip_param_string, sizeof(equip_param_string),
// 				"%d:%d:%d",
// 				target_equip_item.attr_list[0].attr_type, target_equip_item.attr_list[1].attr_type, target_equip_item.attr_list[2].attr_type);
// 		}
// 
// 		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenshou_compose_content,
// 			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), compose_cfg->v_item_id);
// 		if (length > 0)
// 		{
// 			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL, 0, 0);
// 		}
// 	}
}

void Shenshou::CheckFirstHuanlingRefresh()
{
	if (m_role->GetLevel() >= LOGIC_CONFIG->GetShenshouConfig().GetOtherCfg().start_limit_level && 0 == m_param.first_huanling)
	{
		this->HuanlingRefresh(true);
		m_param.first_huanling = 1;
	}
}

void Shenshou::HuanlingRefresh(bool is_free)
{
	if (m_param.cur_huanling_draw_times >= LOGIC_CONFIG->GetShenshouConfig().GetOtherCfg().huanling_draw_limit
		|| m_param.cur_huanling_draw_times >= SHENSHOU_MAX_DRAW_TIMES)
	{
		is_free = true;
	}

	// 消耗积分
	if (!is_free)
	{
		int consume = LOGIC_CONFIG->GetShenshouConfig().GetOtherCfg().huanling_refresh_consume;
		if (consume <= 0)
		{
			return;
		}

		if (m_param.huanling_score < consume)
		{
			m_role->NoticeNum(errornum::EN_SHENSHOU_SCORE_NOT_ENOUGH);
			return;
		}

		m_param.huanling_score -= consume;
	}

	// 刷新
	m_param.cur_huanling_draw_times = 0;
	memset(m_param.huanling_list, 0, sizeof(m_param.huanling_list));
	bool iRet = LOGIC_CONFIG->GetShenshouConfig().GetHuanlingRefreshItem(m_param.refresh_huanling_times, m_param.huanling_list);
	if (iRet)
	{
		this->SendShenshouHuanlingInfo();
	}

	if (0 == m_param.first_huanling)
	{
		m_param.refresh_huanling_times = 0;
	}
}

void Shenshou::HuanlingDraw()
{
	// 检查次数
	if (m_param.cur_huanling_draw_times >= LOGIC_CONFIG->GetShenshouConfig().GetOtherCfg().huanling_draw_limit
		|| m_param.cur_huanling_draw_times >= SHENSHOU_MAX_DRAW_TIMES)
	{
		//m_role->NoticeNum(errornum::EN_SHENSHOU_TIMES_LIMIT);
		return;
	}

	// 消耗积分
	int consume = LOGIC_CONFIG->GetShenshouConfig().GetHuanlingDrawConsumeScore(m_param.cur_huanling_draw_times);
	if (consume <= 0)
	{
		return;
	}

	if (m_param.huanling_score < consume)
	{
		m_role->NoticeNum(errornum::EN_SHENSHOU_SCORE_NOT_ENOUGH);
		return;
	}

	m_param.huanling_score -= consume;

	// 给予物品
	int item_seq = -1;
	bool is_broadcast = false;
	const ItemConfigData *item = LOGIC_CONFIG->GetShenshouConfig().GetHuanlingDrawItem(m_param.cur_huanling_draw_times, m_param.huanling_list, item_seq, is_broadcast);
	if (NULL != item)
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(item->item_id);
		if (NULL == item_base)
		{
			return;
		}

		//抽到神兽装备，直接使用
		bool is_put_in_knapsack = true;
// 		if (ItemBase::I_TYPE_EXPENSE == item_base->GetItemType())
// 		{
// 			const NorexItem *norex_item = dynamic_cast<const NorexItem *>(item_base);
// 			if (NorexItem::I_NOREX_ITEM_SHENSHOU == norex_item->GetUseType())
// 			{
// 				if (!m_role->GetShenshou()->TransformFromKnapsackItem(item->item_id, item->num,
// 					static_cast<short>(norex_item->GetParam1()), static_cast<short>(norex_item->GetParam2()),
// 					norex_item->GetParam3() > 0, norex_item->GetParam4(), "Shenshou Huanling Draw and "))
// 				{
// 					return;
// 				}
// 
// 				is_put_in_knapsack = false;
// 			}
// 		}

		if (is_put_in_knapsack)
		{
			m_role->GetKnapsack()->PutOrMail(*item, PUT_REASON_SHENSHOU_HUANLING);
		}

		++m_param.cur_huanling_draw_times;
		this->SendShenshouHuanlingDrawResult(item_seq);
		this->SendShenshouHuanlingInfo();

		if (is_broadcast)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenshou_huanling_draw,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), item->item_id);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE, 0, 0);
			}
		}
	}
}

bool Shenshou::CheckOpengameDay() const
{
	//int open_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;

	//return open_day >= LOGIC_CONFIG->GetShenshouConfig().GetOtherCfg().opengame_day;
	return true;
}

#ifdef _DEBUG
bool Shenshou::AutoTest()
{
	m_param.Reset();
	//static short HUN_ID = 1;
	//static short HUN_LEVEL = 1;


	return true;
}
#endif

// void Shenshou::SendBackpackAdditionInfo(int backpack_index)
// {
// 	if (m_param.backpack_item_list[backpack_index].item_id <= 0 || m_param.backpack_item_list[backpack_index].item_id > SHENSHOU_MAX_EQUIP_ID) // 有效ID才下发
// 	{
// 		return;
// 	}
// 
// 	static Protocol::SCShenshouBackpackInfo jbpi;
// 	jbpi.is_full_backpack = 0; // 本来去掉增量下发机制，现在客户端又需要增加物品时增量下发。。。
// 	jbpi.grid_num = 0;
// 
// 	jbpi.grid_list[jbpi.grid_num].index = backpack_index;
// 	jbpi.grid_list[jbpi.grid_num].iteminfo = m_param.backpack_item_list[backpack_index];
// 	++jbpi.grid_num;
// 
// 	int send_len = int(sizeof(jbpi) - sizeof(jbpi.grid_list[0]) * (SHENSHOU_MAX_BACKPACK_COUNT - jbpi.grid_num));
// 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&jbpi, send_len);
// }

// void Shenshou::SendBackpackInfo()
// {
// 	static Protocol::SCShenshouBackpackInfo jbpi;
// 	jbpi.is_full_backpack = 1; // 去掉增量下发机制，方便客户端。。
// 	jbpi.grid_num = 0;
// 
// 	for (int index = 0; index < SHENSHOU_MAX_BACKPACK_COUNT; ++index)
// 	{
// 		if (m_param.backpack_item_list[index].item_id <= 0 || m_param.backpack_item_list[index].item_id > SHENSHOU_MAX_EQUIP_ID) // 有效ID才下发
// 		{
// 			continue;
// 		}
// 
// 		jbpi.grid_list[jbpi.grid_num].index = index;
// 		jbpi.grid_list[jbpi.grid_num].iteminfo = m_param.backpack_item_list[index];
// 		++jbpi.grid_num;
// 	}
// 
// 	int send_len = int(sizeof(jbpi) - sizeof(jbpi.grid_list[0]) * (SHENSHOU_MAX_BACKPACK_COUNT - jbpi.grid_num));
// 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&jbpi, send_len);
// }

void Shenshou::SendShenshouInfo(int shenshou_id)
{
	static Protocol::SCShenshouListInfo sli;

	sli.shenshou_num = 0;

	if (shenshou_id > 0 && shenshou_id <= SHENSHOU_MAX_ID) // 有效的神兽ID
	{
		sli.is_all_shenshou = 0;

		sli.shenshou_list[0].shou_id = shenshou_id;
		sli.shenshou_list[0].shou_info = m_param.shenshou_list[shenshou_id];
		++sli.shenshou_num;
	}
	else // 无效ID，则默认发所有信息
	{
		sli.is_all_shenshou = 1;
		
		for (int shou_id = 1; shou_id <= SHENSHOU_MAX_ID; ++shou_id)
		{
			// 子循环只要找到一个有效装备就会break，所以sli.shenshou_num最大值还是超不过SHENSHOU_MAX_ID
			for (int equip_slot_index = 0; equip_slot_index <= SHENSHOU_MAX_EQUIP_SLOT_INDEX; ++equip_slot_index)
			{
				if (m_param.shenshou_list[shou_id].equip_list[equip_slot_index].item_id > 0)
				{
					sli.shenshou_list[sli.shenshou_num].shou_id = shou_id;
					sli.shenshou_list[sli.shenshou_num].shou_info = m_param.shenshou_list[shou_id];
					++sli.shenshou_num;
					break;
				}
			}
		}
	}

	int send_len = int(sizeof(sli) - sizeof(sli.shenshou_list[0]) * (SHENSHOU_MAX_ID + 1 - sli.shenshou_num));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&sli, send_len);
}

void Shenshou::SendShenshouBaseInfo()
{
	static Protocol::SCShenshouBaseInfo ssbi;
	ssbi.extra_zhuzhan_count = m_param.extra_zhuzhan_count;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ssbi, sizeof(ssbi));
}

void Shenshou::SendShenshouHuanlingInfo()
{
	static Protocol::SCShenshouHuanlingListInfo pro;
	pro.score = m_param.huanling_score;
	pro.cur_draw_times = m_param.cur_huanling_draw_times;
	memcpy(pro.huanling_list, m_param.huanling_list, sizeof(pro.huanling_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pro, sizeof(pro));
}

void Shenshou::SendShenshouHuanlingDrawResult(int seq)
{
	static Protocol::SCShenshouHuanlingDrawInfo pro;
	pro.score = m_param.huanling_score;
	pro.seq = seq;
	pro.cur_draw_times = m_param.cur_huanling_draw_times;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pro, sizeof(pro));
}

void Shenshou::SpecialSkillDeal()
{
	std::map<int, std::vector<SkillItem*> > skill_type_map; // 转换数据格式为：以技能类型作为key，因为技能触发是按照技能类型来，同类型的技能一次只触发一个

	for (std::map<int, std::vector<SkillItem> >::iterator skill_list_it = m_shenshou_active_skill_list.begin(); skill_list_it != m_shenshou_active_skill_list.end(); ++skill_list_it)
	{
		for (std::vector<SkillItem>::iterator skill_item_it = skill_list_it->second.begin(); skill_item_it != skill_list_it->second.end(); ++skill_item_it)
		{
			skill_type_map[skill_item_it->skill_type].push_back(&(*skill_item_it));
		}
	}

	m_cd_reduce_percent = 0;
	m_stength_skill_map.clear();
	for (std::map<int, std::vector<SkillItem*> >::iterator skill_list_it = skill_type_map.begin(); skill_list_it != skill_type_map.end(); ++skill_list_it)
	{
		for (std::vector<SkillItem*>::iterator skill_item_it = skill_list_it->second.begin(); skill_item_it != skill_list_it->second.end(); ++skill_item_it)
		{
			SkillItem *skill_item = *skill_item_it;

			const ShenshouSkillCfg::LevelCfg *skill_level_cfg = LOGIC_CONFIG->GetShenshouConfig().GetSkillLevelCfg(skill_item->skill_type, skill_item->level);
			if (NULL == skill_level_cfg) continue;

			if (SHENSHOU_SKILL_TYPE_16 == skill_item->skill_type)
			{
					m_cd_reduce_percent += skill_level_cfg->param_list[PARAM1];
			}
			else if (SHENSHOU_SKILL_TYPE_10 == skill_item->skill_type || SHENSHOU_SKILL_TYPE_12 == skill_item->skill_type)
			{
				if (skill_item->level > m_stength_skill_map[skill_item->skill_type])
				{
					m_stength_skill_map[skill_item->skill_type] = skill_item->level;
				}
			}
		}
	}
}

void Shenshou::RecalcSkillList(int cur_change_shou_id, bool is_new_active)
{
	unsigned int now_second = (unsigned int)EngineAdapter::Instance().Time();

	if (cur_change_shou_id > 0 && cur_change_shou_id <= SHENSHOU_MAX_ID) // 有效的神兽ID，说明是单个神兽助战状态改变
	{
		if (is_new_active) // 新增技能
		{
			const ShenshouInfoCfg *shenshou_info = LOGIC_CONFIG->GetShenshouConfig().GetShenshouInfo(cur_change_shou_id);
			if (NULL == shenshou_info) return;

			m_shenshou_active_skill_list[cur_change_shou_id].clear();
			for (int skill_index = 0; skill_index < ShenshouInfoCfg::MAX_SKILL_COUNT; ++skill_index)
			{
				const ShenshouInfoCfg::SkillItem &skill_cfg_item = shenshou_info->skill_list[skill_index];

				SkillItem skill_item;
				skill_item.skill_type = skill_cfg_item.skill_type;
				skill_item.level = skill_cfg_item.skill_level;
				skill_item.last_perform_time = now_second;

				m_shenshou_active_skill_list[cur_change_shou_id].push_back(skill_item);
			}
		}
		else // 删除技能
		{
			m_shenshou_active_skill_list.erase(cur_change_shou_id);
		}
	}
	else // 所有技能重新计算一遍
	{
		m_shenshou_active_skill_list.clear();
		m_cd_reduce_percent = 0;

		for (int shou_id = 1; shou_id <= SHENSHOU_MAX_ID; ++shou_id)
		{
			if (0 == m_param.shenshou_list[shou_id].has_zhuzhan) continue; // 过滤未助战的神兽

			const ShenshouInfoCfg *shenshou_info = LOGIC_CONFIG->GetShenshouConfig().GetShenshouInfo(shou_id);
			if (NULL == shenshou_info) continue;

			for (int skill_index = 0; skill_index < ShenshouInfoCfg::MAX_SKILL_COUNT; ++skill_index)
			{
				const ShenshouInfoCfg::SkillItem &skill_cfg_item = shenshou_info->skill_list[skill_index];

				SkillItem skill_item;
				skill_item.skill_type = skill_cfg_item.skill_type;
				skill_item.level = skill_cfg_item.skill_level;
				skill_item.last_perform_time = now_second;

				m_shenshou_active_skill_list[shou_id].push_back(skill_item);

			}
		}
	}

	this->SpecialSkillDeal();
}

long long Shenshou::RecalcExtraInjureById(Character *target, long long injure, int skill_id)
{
	if (!this->CheckOpengameDay())
	{
		return 0;
	}

	if (NULL == target)
	{
		return 0;
	}

	//if (m_role->GetSkillManager()->IsCommonSkill(skill_id))
	//{
	//	return 0;
	//}

	int percent = this->GetHurtIncPercent();
	if (target->GetObjType() == Obj::OBJ_TYPE_ROLE)
	{
		Role *t_role = (Role*)target;
		percent -= t_role->GetShenshou()->GetHurtDecPer();
	}

	return (long long)(injure * (percent / 100.0f)) ;
}

void Shenshou::RecalcCD(int skill_id, int cold_down_times, int &reduce_cd) const
{
	if (!this->CheckOpengameDay())
	{
		return;
	}

	reduce_cd += (int)(cold_down_times * (m_cd_reduce_percent / 10000.0f));
}

int Shenshou::GetSkill12HurtPercent()
{
	if (!this->CheckOpengameDay())
	{
		return 0;
	}

	int level = m_stength_skill_map[SHENSHOU_SKILL_TYPE_12];
	const ShenshouSkillCfg::LevelCfg *skill_cfg = LOGIC_CONFIG->GetShenshouConfig().GetSkillLevelCfg(SHENSHOU_SKILL_TYPE_12, level);
	if (NULL == skill_cfg)
	{
		return 0;
	}

	return skill_cfg->param_list[PARAM1];
}

int Shenshou::GetSkill10HurtPercent()
{
	if (!this->CheckOpengameDay())
	{
		return 0;
	}

	int level = m_stength_skill_map[SHENSHOU_SKILL_TYPE_10];
	const ShenshouSkillCfg::LevelCfg *skill_cfg = LOGIC_CONFIG->GetShenshouConfig().GetSkillLevelCfg(SHENSHOU_SKILL_TYPE_10, level);
	if (NULL == skill_cfg)
	{
		return 0;
	}

	return skill_cfg->param_list[PARAM1];
}

void Shenshou::OnDayChange(int old_dayid, int now_dayid)
{
	this->CheckFuncOpen();
}

void Shenshou::OnRoleLevelChange(int level)
{
	this->CheckFuncOpen();
}

void Shenshou::OnCompleteTask(TaskID task_id)
{
	this->CheckFuncOpen();
}

int Shenshou::GetActiveCount()
{
	int count = 0;
	for (const auto &shenshou_info : m_param.shenshou_list)
	{
		if (shenshou_info.has_zhuzhan)
		{
			++count;
		}
	}

	return count;
}

int Shenshou::GetEquipCount(int quality)
{
	int count = 0;
	for (const auto &shenshou_info : m_param.shenshou_list)
	{
		for (unsigned int i = 0; i < static_array_size(shenshou_info.equip_list); i ++)
		{
			const ShenshouItemCfg *item_cfg = LOGIC_CONFIG->GetShenshouConfig().GetItemInfo(shenshou_info.equip_list[i].item_id);
			if (nullptr == item_cfg)
			{
				continue;
			}

			if (item_cfg->quality >= quality)
			{
				++count;
			}
		}
	}

	return count;
}

void Shenshou::CheckFuncOpen()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (m_role->GetRoleBigSmallGoal()->IsFuncActive(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENSHOU))
	{
		return;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_SHENSHOU);
	if (fun_open_cfg != nullptr && opengame_day >= fun_open_cfg->opengame_day)
	{
		if (m_role->GetLevel() >= fun_open_cfg->role_level || m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
		{
			m_role->GetRoleBigSmallGoal()->OnOpenFunc(ROLE_BIG_SMALL_GOAL_SYSTEM_SHENSHOU);
		}
	}
}

void Shenshou::GetSkill12HurtNum(int &num)
{
	//int level = m_stength_skill_map[SHENSHOU_SKILL_TYPE_12];
	//const ShenshouSkillCfg::LevelCfg *skill_cfg = LOGIC_CONFIG->GetShenshouConfig().GetSkillLevelCfg(SHENSHOU_SKILL_TYPE_12, level);
	//if (NULL == skill_cfg)
	//{
	//	return;
	//}

	//num = skill_cfg->param_list[PARAM2];
}

void Shenshou::GetSkill10DecSpeedInterval(int &interval)
{
	//int level = m_stength_skill_map[SHENSHOU_SKILL_TYPE_10];
	//const ShenshouSkillCfg::LevelCfg *skill_cfg = LOGIC_CONFIG->GetShenshouConfig().GetSkillLevelCfg(SHENSHOU_SKILL_TYPE_10, level);
	//if (NULL == skill_cfg)
	//{
	//	return;
	//}

	//interval = skill_cfg->param_list[PARAM3];
}
