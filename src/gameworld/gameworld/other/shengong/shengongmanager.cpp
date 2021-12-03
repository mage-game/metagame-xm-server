#include "shengongmanager.hpp"
#include "gamelog.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "protocal/msgshengong.hpp"
#include "scene/scene.h"
#include "shengongconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/shuxingdanconfig.hpp"
#include "servercommon/vipdef.hpp"
#include "other/shengong/shengongfunction.hpp"
#include "globalconfig/globalconfig.h"
#include "serverlogic.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effecthurtinc.hpp"
#include "task/taskrecorder.h"
#include "other/capability/capability.hpp"
#include "other/xunzhang/xunzhangmanager.hpp"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include "other/imgfuling/imgfuling.hpp"
#include "servercommon/imgfulingdef.hpp"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "other/talent/talent.hpp"
#include "gstr.h"
#include "other/jinjiesysreward/jinjiesysrewardmgr.hpp"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "other/dressingroom/dressingroomconfig.hpp"
#include "other/dressingroom/dressingroommgr.hpp"
#include "common/bintohex.h"
#include "servercommon/serverconfig/crossconfig.hpp"


ShengongManager::ShengongManager()
: m_role(NULL),m_other_capability(0)
{
}

ShengongManager::~ShengongManager()
{

}

void ShengongManager::Init(Role *role, const ShengongParam &shengong_param)
{
	m_role = role;
	m_shengong_param = shengong_param;
}

void ShengongManager::GetOtherInitParam(ShengongParam *param)
{
	*param = m_shengong_param;
}

void ShengongManager::GetRoleAppearance(RoleAppearance *appearance)
{
	if (NULL != appearance)
	{
		appearance->shengong_used_imageid = m_shengong_param.used_imageid;
	}
}

void ShengongManager::OnLogin()
{
	this->CheckActive();
}

void ShengongManager::OnRoleLevelChange(int level)
{
	m_shengong_param.level = level;

	if (m_shengong_param.IsShengongActived())
	{
		this->SendShengongInfo();
	}
}

void ShengongManager::Update(time_t now_second)
{
 	if (0 != m_shengong_param.clear_upgrade_time && now_second > m_shengong_param.clear_upgrade_time)
 	{
 		m_shengong_param.grade_bless_val = 0;
 		m_shengong_param.clear_upgrade_time = 0;

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGONG);

 		this->SendShengongInfo();
 	}
}

void ShengongManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->CheckActive();
	}

//	if (!m_shengong_param.IsShengongActived())
//	{
//		unsigned int now = (unsigned int)EngineAdapter::Instance().Time();
//		unsigned int opengame_timestamp = static_cast<unsigned int>(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime());
//		int opengame_day = GetDayIndex(opengame_timestamp, now) + 1;
//
//		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_SHENGONG);
//		if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day)
//		{
//			if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
//			{
//				ShengongFunction::ActiveShengong(m_role);
//			}
//		}
//	}
}

// 对敌人的技能特效
void ShengongManager::OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag)
{
	if(NULL == target)
	{
		return;
	}

	unsigned long long now_time_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();

	// 攻击时有param_b概率使玩家造成伤害提高param_c%，持续param_d毫秒。冷却param_a毫秒
	static const int SKILL_ID0 = 0;
	UNSTD_STATIC_CHECK(ShengongParam::SKILL_COUNT > SKILL_ID0);
	if (m_shengong_param.skill_level_list[SKILL_ID0] > 0 && now_time_ms > m_shengong_param.skill_last_colddown_time[SKILL_ID0])
	{
		const ShengongSkillConifg *skill_cfg = LOGIC_CONFIG->GetShengongConfig().GetSkillCfg(SKILL_ID0, m_shengong_param.skill_level_list[SKILL_ID0]);
		if (NULL != skill_cfg)
		{
			if (RandomNum(10000) < skill_cfg->param_b)
			{
				EffectIncHurt *effect = new EffectIncHurt(m_role->GetId(), SKILL_ID0, skill_cfg->param_d, skill_cfg->skill_level);
				effect->SetIncPercent(skill_cfg->param_c);
				effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
				m_role->AddEffect(effect);

				m_shengong_param.skill_last_colddown_time[SKILL_ID0] = now_time_ms + skill_cfg->param_a;
			}
		}
	}

	// 光剑
	static const int SKILL_ID1 = 1;
	UNSTD_STATIC_CHECK(ShengongParam::SKILL_COUNT > SKILL_ID1);
	if (m_shengong_param.skill_level_list[SKILL_ID1] > 0)
	{
		const ShengongSkillConifg *skill_cfg = LOGIC_CONFIG->GetShengongConfig().GetSkillCfg(SKILL_ID1, m_shengong_param.skill_level_list[SKILL_ID1]);
		if (NULL != skill_cfg)
		{
			if (now_time_ms > m_shengong_param.skill_last_colddown_time[SKILL_ID1] || target_index > 0)
			{
				if (0 == target_index)
				{
					m_shengong_param.skill_last_colddown_time[SKILL_ID1] = now_time_ms + skill_cfg->param_a;
				}
				
				if (target_index < skill_cfg->param_b)
				{
					passive_flag |= (1 << EffectBase::PASSIVE_FLAG_SHENGONG_GUANGJIAN);
					injure += (Attribute)(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI) * (skill_cfg->param_c * ROLE_ATTR_PER));
				}
			}
		}
	}
	

	// 诛魔
	static const int SKILL_ID2 = 2;
	UNSTD_STATIC_CHECK(ShengongParam::SKILL_COUNT > SKILL_ID2);
	if (m_shengong_param.skill_level_list[SKILL_ID2] > 0 && now_time_ms > m_shengong_param.skill_last_colddown_time[SKILL_ID2])
	{
		const ShengongSkillConifg *skill_cfg = LOGIC_CONFIG->GetShengongConfig().GetSkillCfg(SKILL_ID2, m_shengong_param.skill_level_list[SKILL_ID2]);
		if (NULL != skill_cfg)
		{
			if (Obj::OBJ_TYPE_MONSTER == target->GetObjType())
			{
				if (RandomNum(10000) < skill_cfg->param_b)
				{
					m_shengong_param.skill_last_colddown_time[SKILL_ID2] = now_time_ms + skill_cfg->param_a;
					passive_flag |= (1 << EffectBase::PASSIVE_FLAG_SHENGONG_ZHUMO);

					EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), 2, skill_cfg->param_c, 1);
					effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
					Attribute value = (Attribute)(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI) * (skill_cfg->param_d * ROLE_ATTR_PER));
					effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI, value, skill_cfg->param_d);
					effect_buff->SetClientEffectType(EffectBase::ECT_UPGRADE_SHENGONG_ZHUMO);
					m_role->AddEffect(effect_buff);

					EffectSpecialState *effect_state = new EffectSpecialState(m_role->GetId(), 2, skill_cfg->param_c, EBT_SHENGGONG_ZHUMO, 1);
					effect_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
					m_role->AddEffect(effect_state);
				}
			}
		}
	}

	// 必杀
	static const int SKILL_ID3 = 3;
	UNSTD_STATIC_CHECK(ShengongParam::SKILL_COUNT > SKILL_ID3);
	if (m_shengong_param.skill_level_list[SKILL_ID3] > 0)
	{
		const ShengongSkillConifg *skill_cfg = LOGIC_CONFIG->GetShengongConfig().GetSkillCfg(SKILL_ID3, m_shengong_param.skill_level_list[SKILL_ID3]);
		if (NULL != skill_cfg)
		{
			Attribute current_hp = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			Attribute max_hp = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			if (current_hp < max_hp * (skill_cfg->param_b * ROLE_ATTR_PER))
			{
				injure += (Attribute)(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI) * (skill_cfg->param_c * ROLE_ATTR_PER));
			}
		}
	}
}

void ShengongManager::SendShengongInfo()
{
	Protocol::SCShengongInfo mi;
	mi.star_level = m_shengong_param.star_level;
	mi.shengong_level = m_shengong_param.level;
	mi.grade = m_shengong_param.grade;
	mi.grade_bless_val = m_shengong_param.grade_bless_val;
	mi.clear_upgrade_time = m_shengong_param.clear_upgrade_time;
	mi.used_imageid = m_shengong_param.used_imageid;
	UNSTD_STATIC_CHECK(sizeof(mi.active_image_flag) == sizeof(m_shengong_param.active_image_flag));
	memcpy(mi.active_image_flag, m_shengong_param.active_image_flag, sizeof(mi.active_image_flag));
	UNSTD_STATIC_CHECK(sizeof(mi.active_special_image_flag) == sizeof(m_shengong_param.active_special_image_flag));
	memcpy(mi.active_special_image_flag, m_shengong_param.active_special_image_flag, sizeof(mi.active_special_image_flag));
	mi.shuxingdan_count = m_shengong_param.shuxingdan_count;
	mi.chengzhangdan_count = m_shengong_param.chengzhangdan_count;
	mi.equip_skill_level = m_shengong_param.equip_skill_level;
	memcpy(mi.equip_level_list, m_shengong_param.equip_level_list, sizeof(mi.equip_level_list));
	memcpy(mi.skill_level_list, m_shengong_param.skill_level_list, sizeof(mi.skill_level_list));
	memcpy(mi.special_img_grade_list, m_shengong_param.special_img_grade_list, sizeof(mi.special_img_grade_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&mi, sizeof(mi));
}

int ShengongManager::GetShengongAppeID()
{
	return m_shengong_param.used_imageid;
}

bool ShengongManager::ActiveImage(int imageid, bool recalc)
{
	if (imageid <= 0 || imageid >= ShengongParam::MAX_IMAGE_COUNT) return false;

	m_shengong_param.ActiveImage(imageid);

	if (recalc)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGONG);
	}

	return true;
}

bool ShengongManager::UseImage(int imageid)
{
	if (imageid < SPECIAL_IMG_START_ID)
	{
		if (!m_shengong_param.IsImageActive(imageid))
		{
			m_role->NoticeNum(errornum::EN_SHENGONG_IMG_NOT_ACTIVE);
			return false;
		}
	}
	else
	{
		if (!m_shengong_param.IsSpecialImageActive(imageid - SPECIAL_IMG_START_ID))
		{
			m_role->NoticeNum(errornum::EN_SHENGONG_IMG_NOT_ACTIVE);
			return false;
		}
	}

	m_shengong_param.used_imageid = imageid;

	m_role->RefreshApperance(true);

	this->SendShengongInfo();

	return true;
}

bool ShengongManager::UnUseImage(int imageid)
{
	if (imageid != m_shengong_param.used_imageid)
	{
		return false;
	}

	m_shengong_param.used_imageid = 0;

	m_role->RefreshApperance(true);

	this->SendShengongInfo();

	return true;
}

bool ShengongManager::ActiveSpecialImage(int imageid, bool is_notify)
{
	if (m_shengong_param.IsSpecialImageActive(imageid))
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_SPECIAL_IMG_ALREADY_ACTIVE);
		}

		return false;
	}

	const ShengongSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetShengongConfig().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	m_shengong_param.ActiveSpecialImage(imageid);
	m_role->GetImgFuling()->OnPutOnImg(JINGJIE_TYPE_SHENGONG, imageid);
	if (0 == m_shengong_param.special_img_grade_list[imageid])
	{
		m_shengong_param.special_img_grade_list[imageid] = 1;
	}
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGONG);
	JinjiesysFuncHelper::Instance().OnActiveSpecialImg(m_role, JINJIE_TYPE_SHENGONG, imageid);
	this->UseImage(SPECIAL_IMG_START_ID + imageid);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_SHENGONG, imageid, true);

	return true;
}

bool ShengongManager::UnActiveSpecialImage(int imageid)
{
	if (!m_shengong_param.IsSpecialImageActive(imageid))
	{
		return false;
	}

	const ShengongSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetShengongConfig().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	BitmapFunction::ClearBit(m_shengong_param.active_special_image_flag, sizeof(m_shengong_param.active_special_image_flag), imageid);
	if (1 == m_shengong_param.special_img_grade_list[imageid])
	{
		m_shengong_param.special_img_grade_list[imageid] = 0;
	}
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGONG);

	if (this->UnUseImage(SPECIAL_IMG_START_ID + imageid))
	{
		this->UseImage(1); //默认再给一个
	}
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_SHENGONG, imageid, false);
	return true;
}

void ShengongManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();
		m_other_capability = 0;
		CharIntAttrs other_per_add_attr;
		double rate = 0.0;						// 属性加成万分比

		{
			// 等级加成
			const ShengongLevelConfig *level_cfg = LOGIC_CONFIG->GetShengongConfig().GetLevelConfig(m_shengong_param.level);
			if (NULL != level_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += level_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += level_cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += level_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += level_cfg->jianren;
			}
		}

 		{
			CharIntAttrs grade_attrs;
 			// 阶数加成  
 			const ShengongGradeConfig * grade_config = LOGIC_CONFIG->GetShengongConfig().GetGradeConfig(m_shengong_param.grade);
 			if (NULL != grade_config) 
 			{
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += grade_config->maxhp;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += grade_config->gongji;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += grade_config->fangyu;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += grade_config->mingzhong;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += grade_config->shanbi;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += grade_config->baoji;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += grade_config->jianren;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += grade_config->per_jingzhun;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += grade_config->per_baoji;
// 				grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += grade_config->jianshang_per;
// 				grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += grade_config->zengshang_per;
// 				grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += grade_config->jianshang_per;
// 				grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += grade_config->zengshang_per;

				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, grade_config->zengshang_per);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, grade_config->zengshang_per);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, grade_config->jianshang_per);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, grade_config->jianshang_per);

				// 大目标特殊形象万分比
				int special_img_per_add = JinjieSysRewardMgr::Instance().GetJinjieSpecialImgExtraAddPer(m_role, JINJIE_TYPE_SHENGONG);
				if (special_img_per_add > 0)
				{
					other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (ROLE_ATTR_PER * special_img_per_add));
					other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (ROLE_ATTR_PER * special_img_per_add));
					other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (ROLE_ATTR_PER * special_img_per_add));
					other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (ROLE_ATTR_PER * special_img_per_add));
					other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (ROLE_ATTR_PER * special_img_per_add));
					other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (ROLE_ATTR_PER * special_img_per_add));
					other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (ROLE_ATTR_PER * special_img_per_add));
				}
 				// 祝福值加成
 				//if (m_shengong_param.grade_bless_val > 0)
 				//{
 				//	const ShengongGradeConfig * next_grade_config = LOGIC_CONFIG->GetShengongConfig().GetGradeConfig(m_shengong_param.grade + 1);
 				//	const ShengongGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetShengongConfig().GetGradeConfig(m_shengong_param.grade);
 				//	if (NULL != next_grade_config && NULL != curr_grade_cfg) 
 				//	{
 				//		double rate = (((double)m_shengong_param.grade_bless_val) / curr_grade_cfg->bless_val_limit) * (next_grade_config->bless_addition * ROLE_ATTR_PER);
 				//		//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)((next_grade_config->maxhp - grade_config->maxhp) * rate);
 				//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)((next_grade_config->gongji - grade_config->gongji) * rate);
 				//	}
 				//}
				m_attrs_add.Add(grade_attrs.m_attrs);

				//天赋加成
				m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_SHENGONG_BASE, grade_attrs, m_attrs_add);
 			}
 		}

		// 天赋加成
		m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_SHENGONG, m_attrs_add, m_attrs_add);

		//{
		//	// 星级加成  
		//	const ShengongUpStarExpCfg * star_level_config = LOGIC_CONFIG->GetShengongConfig().GetUpStarExpCfg(m_shengong_param.star_level);
		//	if (NULL != star_level_config) 
		//	{
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += star_level_config->maxhp;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += star_level_config->gongji;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += star_level_config->fangyu;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += star_level_config->mingzhong;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += star_level_config->shanbi;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += star_level_config->baoji;
		//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += star_level_config->jianren;
		//	}
		//}

		{
			// 装备加成
			int active_equip_grade = LOGIC_CONFIG->GetShengongConfig().GetActiveEquipGrade();
			if (active_equip_grade <= m_shengong_param.grade)
			{
				for (int i = 0; i < ShengongParam::EQUIP_COUNT; ++i)
				{
					const ShengongEquipConifg *old_equip_cfg = LOGIC_CONFIG->GetShengongConfig().GetOldEquipCfg(i);

					if (NULL != old_equip_cfg)
					{
						int level = m_shengong_param.old_equip_level_list[i];
						if (level >= 0 && level < old_equip_cfg->max_level)
						{
							rate += old_equip_cfg->equip_level_cfg_list[level].add_percent;
						}
					}
				}
				for (int i = 0; i < ShengongParam::EQUIP_COUNT; ++ i)
				{
					const ShengongEquipConifg *equip_cfg = LOGIC_CONFIG->GetShengongConfig().GetEquipCfg(i);

					if (NULL != equip_cfg)
					{
						int level = m_shengong_param.equip_level_list[i];
						if (level >= 0 && level < equip_cfg->max_level)
						{
							rate += equip_cfg->equip_level_cfg_list[level].add_percent;
						}
					}	
				}
			}

			{
				// 勋章加成
				rate += m_role->GetXunZhangManager()->GetShengongAddPer();

				// 星座星魂加成
				int xinhun_add_per = 0;
				m_role->GetChineseZodiac()->GetXingHunTotalAddPer(NULL, NULL, NULL, &xinhun_add_per, NULL, NULL, NULL);
				rate += xinhun_add_per;

				// 成长丹加成，只对阶数和等级属性有效
				// 			if (m_shengong_param.chengzhangdan_count > 0)
				// 			{
				// 				int percent = LOGIC_CONFIG->GetShengongConfig().GetChengZhangDanPercent() * m_shengong_param.chengzhangdan_count;
				// 				if (percent > 0)
				// 				{
				// 					rate += percent;
				// 				}
				// 			}
			}

			rate *= ROLE_ATTR_PER;
			if (rate > 0)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * rate);
			}

			if (active_equip_grade <= m_shengong_param.grade)
			{
				for (int i = 0; i < ShengongParam::EQUIP_COUNT; ++i)
				{
					const ShengongEquipConifg *old_equip_cfg = LOGIC_CONFIG->GetShengongConfig().GetOldEquipCfg(i);
					if (NULL != old_equip_cfg)
					{
						int level = m_shengong_param.old_equip_level_list[i];
						if (level >= 0 && level < old_equip_cfg->max_level)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += old_equip_cfg->equip_level_cfg_list[level].maxhp;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += old_equip_cfg->equip_level_cfg_list[level].gongji;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += old_equip_cfg->equip_level_cfg_list[level].fangyu;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += old_equip_cfg->equip_level_cfg_list[level].mingzhong;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += old_equip_cfg->equip_level_cfg_list[level].shanbi;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += old_equip_cfg->equip_level_cfg_list[level].baoji;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += old_equip_cfg->equip_level_cfg_list[level].jianren;
						}
					}
				}
				for (int i = 0; i < ShengongParam::EQUIP_COUNT; ++ i)
				{
					const ShengongEquipConifg *equip_cfg = LOGIC_CONFIG->GetShengongConfig().GetEquipCfg(i);
					if (NULL != equip_cfg)
					{
						int level = m_shengong_param.equip_level_list[i];
						if (level >= 0 && level < equip_cfg->max_level)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += equip_cfg->equip_level_cfg_list[level].maxhp;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += equip_cfg->equip_level_cfg_list[level].gongji;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += equip_cfg->equip_level_cfg_list[level].fangyu;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += equip_cfg->equip_level_cfg_list[level].mingzhong;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += equip_cfg->equip_level_cfg_list[level].shanbi;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += equip_cfg->equip_level_cfg_list[level].baoji;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += equip_cfg->equip_level_cfg_list[level].jianren;
						}
					}	
				}
			}
		}

		{
			// 特殊形象
			for (int i = 0; i < ShengongParam::MAX_SPECIAL_IMAGE_COUNT; ++ i)
			{
				if (m_shengong_param.IsSpecialImageActive(i))
				{
					// 特殊形象进阶
					const int grade = m_shengong_param.special_img_grade_list[i];

					const ShengongSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetShengongConfig().GetSpecialImgUpgradeCfg(i, grade);
					if (NULL != upgrade_cfg)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += upgrade_cfg->maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += upgrade_cfg->gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += upgrade_cfg->fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += upgrade_cfg->mingzhong ;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += upgrade_cfg->shanbi;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += upgrade_cfg->baoji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += upgrade_cfg->jianren;
					}
					this->HuanHuaSpecialCapAdd(i, grade, m_attrs_add);
				}
			}
		}

		{
			// 属性丹
			if (m_shengong_param.shuxingdan_count > 0)
			{
				const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_SHENGONG, 0);
				if (NULL != cfg_item)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp * m_shengong_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji * m_shengong_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu * m_shengong_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg_item->mingzhong * m_shengong_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg_item->shanbi * m_shengong_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg_item->baoji * m_shengong_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg_item->jianren * m_shengong_param.shuxingdan_count;
				}
			}
		}

		{
			// 技能属性
			for (int i = 0; i < ShengongParam::SKILL_COUNT; i++)
			{
				if (m_shengong_param.skill_level_list[i] > 0)
				{
					const ShengongSkillConifg *skill_cfg = LOGIC_CONFIG->GetShengongConfig().GetSkillCfg(i, m_shengong_param.skill_level_list[i]);
					if (NULL != skill_cfg)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += skill_cfg->maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += skill_cfg->gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += skill_cfg->fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += skill_cfg->mingzhong ;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += skill_cfg->shanbi;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += skill_cfg->baoji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += skill_cfg->jianren;
						m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_SKILL_CAP_PER] += skill_cfg->skill_cap_per;
						m_other_capability += skill_cfg->capability;
					}
				}
			}
		}
		// 其他百分比属性加成，与上面的全属性加成加算
		{
			m_attrs_add.AddAttrs(&other_per_add_attr);
		}
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_SHENGONG, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_SHENGONG, m_other_capability);

	//策划需求，神弓的战力算入女神战力中
	m_role->GetCapability()->SetCap(CAPABILITY_TYPE_XIANNV, m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_XIANNV) + m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENGONG));

	m_shengong_param.capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENGONG);

	base_add.Add(m_attrs_add.m_attrs, true);
}

ShengongParam * ShengongManager::GetShengongParam()
{
	return &m_shengong_param;
}

void ShengongManager::PrintShengong(const char *reason, int param1, int param2, int param3, int param4, int param5)
{
	UNSTD_STATIC_CHECK(4 == ShengongParam::EQUIP_COUNT);
	UNSTD_STATIC_CHECK(4 == ShengongParam::SKILL_COUNT);
	const int image_hex_len = ShengongParam::MAX_ACTIVE_IMAGE_BYTE * 2 + 1;
	const int special_image_hex_len = ShengongParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE * 2 + 1;
	char active_image_hex[image_hex_len] = { 0 };
	char active_special_image_hex[special_image_hex_len] = { 0 };
	BinToHex((const char*)m_shengong_param.active_image_flag, sizeof(m_shengong_param.active_image_flag), active_image_hex);
	BinToHex((const char*)m_shengong_param.active_special_image_flag, sizeof(m_shengong_param.active_special_image_flag), active_special_image_hex);
	gamelog::g_log_shengong.printf(LL_INFO, "Shengong role(%d,%s) %s(%d,%d,%d,%d,%d) "
		"level:%d, grade:%d,%d, used_imageid:%d shuxingdan_count:%d chengzhangdan_count:%d, image_flag:%s, special_image_flag:%s"
		"equip_info_list:%d, %d, %d, %d"
		"skill_level_list:%d, %d, %d, %d",
		m_role->GetUID(), m_role->GetName(),
		reason, param1, param2, param3, param4, param5,
		(int)m_shengong_param.level,
		(int)m_shengong_param.grade, (int)m_shengong_param.grade_bless_val,
		(int)m_shengong_param.used_imageid,
		(int)m_shengong_param.shuxingdan_count, (int)m_shengong_param.chengzhangdan_count,
		active_image_hex, active_special_image_hex,
		(int)m_shengong_param.equip_level_list[0], (int)m_shengong_param.equip_level_list[1],
		(int)m_shengong_param.equip_level_list[2], (int)m_shengong_param.equip_level_list[3],
		(int)m_shengong_param.skill_level_list[0], (int)m_shengong_param.skill_level_list[1], (int)m_shengong_param.skill_level_list[2], (int)m_shengong_param.skill_level_list[3]);
}

void ShengongManager::OnCompleteTask(TaskID task_id)
{
	this->CheckActive();
//	if (m_shengong_param.IsShengongActived()) return;
//
//	unsigned int now = (unsigned int)EngineAdapter::Instance().Time();
//	unsigned int opengame_timestamp = static_cast<unsigned int>(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime());
//	int opengame_day = GetDayIndex(opengame_timestamp, now) + 1;
//
//	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_SHENGONG);
//
//	if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day && fun_open_cfg->task_id == task_id)
//	{
//		ShengongFunction::ActiveShengong(m_role);
//	}
}

void ShengongManager::OnRoleUpLevel(int level)
{
	this->CheckActive();
//	if (m_shengong_param.IsShengongActived()) return;
//
//	unsigned int now = (unsigned int)EngineAdapter::Instance().Time();
//	unsigned int opengame_timestamp = static_cast<unsigned int>(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime());
//	int opengame_day = GetDayIndex(opengame_timestamp, now) + 1;
//
//	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_SHENGONG);
//
//	if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day && level >= fun_open_cfg->role_level)
//	{
//		ShengongFunction::ActiveShengong(m_role);
//	}
}

bool ShengongManager::OnUseShuxingdan(int slot_idx, short &use_count)
{
	if (slot_idx < 0 || slot_idx >= SHUXINGDAN_MAX_TYPE || use_count <= 0)
	{
		return false;
	}

	const ShengongGradeConfig *grade_cfg = LOGIC_CONFIG->GetShengongConfig().GetGradeConfig(m_shengong_param.grade);
	if (NULL == grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_SHENGONG_SHUXINGDAN_USE_LIMIT_NUM);
		return false;
	}

	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_SHENGONG, slot_idx);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_shengong_param.grade < cfg_item->order_limit)
	{
		gstr::SendError(m_role, "shuxingdan_use_order_limit_1", SHUXINGDAN_TYPE_SHENGONG, cfg_item->order_limit - 1, slot_idx); // 客户端阶数-1
		return false;
	}

	if (m_shengong_param.shuxingdan_count + use_count > cfg_item->use_limit_num)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	int special_img_add_count = 0;
	for (int i = 0;i < ShengongParam::MAX_SPECIAL_IMAGE_COUNT; i++)
	{
		if (!m_shengong_param.IsSpecialImageActive(i))
			continue;

		const ShengongSpecialImgUpgradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetShengongConfig().GetSpecialImgUpgradeCfg(i,m_shengong_param.special_img_grade_list[i]);
		if (NULL != special_img_grade_cfg)
		{
			special_img_add_count += special_img_grade_cfg->shuxingdan_count;
		}
	}

	// 策划需求，修正真正使用数量
	int can_use_count = grade_cfg->shuxingdan_limit + special_img_add_count - m_shengong_param.shuxingdan_count;
	if (can_use_count > 0)
	{
		if (can_use_count < use_count)
		{
			use_count = can_use_count;
		}
	}
	else
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	m_shengong_param.shuxingdan_count += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGONG);
	this->SendShengongInfo();

	return true;
}

bool ShengongManager::OnUseChengzhangdan(int use_count)
{
	const ShengongGradeConfig *grade_cfg = LOGIC_CONFIG->GetShengongConfig().GetGradeConfig(m_shengong_param.grade);
	if (NULL == grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_CHENGZHANGDAN_USE_LIMIT_NUM);
		return false;
	}

	int special_img_add_count = 0;
	for (int i = 0;i < ShengongParam::MAX_SPECIAL_IMAGE_COUNT; i++)
	{
		if (!m_shengong_param.IsSpecialImageActive(i))
			continue;

		const ShengongSpecialImgUpgradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetShengongConfig().GetSpecialImgUpgradeCfg(i,m_shengong_param.special_img_grade_list[i]);
		if (NULL != special_img_grade_cfg)
		{
			special_img_add_count += special_img_grade_cfg->chengzhangdan_count;
		}
	}

	if (m_shengong_param.chengzhangdan_count + use_count > grade_cfg->chengzhangdan_limit + special_img_add_count)
	{
		m_role->NoticeNum(errornum::EN_CHENGZHANGDAN_USE_LIMIT_NUM);
		return false;
	}

	m_shengong_param.chengzhangdan_count += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGONG);
	this->SendShengongInfo();

	return true;
}

int ShengongManager::RefreshShengongMinEquipLevel()
{
	int min = m_shengong_param.equip_level_list[0];

	for (int i = 0; i < ShengongParam::EQUIP_COUNT; ++i)
	{
		min = m_shengong_param.equip_level_list[i] < min ? m_shengong_param.equip_level_list[i] : min;
	}

	return min;
}

void ShengongManager::CheckActive()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (!m_shengong_param.IsShengongActived())
	{
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_SHENGONG);
		if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day)
		{
			if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
			{
				ShengongFunction::ActiveShengong(m_role);
			}
		}
	}
}

// 幻化特殊战力加成
void ShengongManager::HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add)
{
	if (!LOGIC_CONFIG->GetShengongConfig().IsAddSpecialCap())
	{
		return;
	}

	auto cfg_item = LOGIC_CONFIG->GetShengongConfig().GetHuanHuaSpecialCapAddCfg(huanhua_id);
	if (nullptr == cfg_item)
	{
		return;
	}

	if (huanhua_level >= cfg_item->huanhua_level)
	{
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp;
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji;
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu;
	}
}

int ShengongManager::GetSpecialImageLevelById(int imageid)
{
	if (imageid < 0 || imageid >= ShengongParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return 0;
	}

	if (!m_shengong_param.IsSpecialImageActive(imageid)) return 0;

	return m_shengong_param.special_img_grade_list[imageid];
}

int ShengongManager::GetGrade()
{
	if (!m_shengong_param.IsShengongActived())
	{
		return 0;
	}
	return m_shengong_param.grade;
}
