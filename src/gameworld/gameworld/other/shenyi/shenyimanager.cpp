#include "shenyimanager.hpp"
#include "gamelog.h"
#include "gstr.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "protocal/msgshenyi.hpp"
#include "scene/scene.h"
#include "shenyiconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/shuxingdanconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "servercommon/vipdef.hpp"
#include "other/shenyi/shenyifunction.hpp"
#include "globalconfig/globalconfig.h"
#include "other/camp/campconfig.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "serverlogic.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/effectbase.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "task/taskrecorder.h"
#include "other/capability/capability.hpp"
#include "other/xunzhang/xunzhangmanager.hpp"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include "other/imgfuling/imgfuling.hpp"
#include "servercommon/imgfulingdef.hpp"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "other/jinjiesysreward/jinjiesysrewardmgr.hpp"
#include "other/dressingroom/dressingroomconfig.hpp"
#include "other/dressingroom/dressingroommgr.hpp"
#include "common/bintohex.h"
#include "servercommon/serverconfig/crossconfig.hpp"

ShenyiManager::ShenyiManager()
: m_role(NULL),m_other_capability(0)
{

}

ShenyiManager::~ShenyiManager()
{

}

void ShenyiManager::Init(Role *role, const ShenyiParam &shenyi_param)
{
	m_role = role;
	m_shenyi_param = shenyi_param;
	/*if (!m_shenyi_param.IsShenyiActived())
	{
		unsigned int now = (unsigned int)EngineAdapter::Instance().Time();
		unsigned int opengame_timestamp = static_cast<unsigned int>(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime());
		int opengame_day = GetDayIndex(opengame_timestamp, now) + 1;

		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_SHENYI);
		if (nullptr != fun_open_cfg)// && opengame_day >= fun_open_cfg->opengame_day)
		{
			if (m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id) || m_role->GetLevel() >= fun_open_cfg->role_level)
			{
				ShenyiFunction::ActiveShenyi(m_role, false);
			}
		}
	}*/
}

void ShenyiManager::GetOtherInitParam(ShenyiParam *param)
{
	*param = m_shenyi_param;
}

void ShenyiManager::GetRoleAppearance(RoleAppearance *appearance)
{
	if (NULL != appearance)
	{
		appearance->shenyi_used_imageid = m_shenyi_param.used_imageid;
	}
}

void ShenyiManager::OnLogin()
{
	this->CheckActive();
}

void ShenyiManager::OnRoleLevelChange(int level)
{
	m_shenyi_param.level = level;
	if (m_shenyi_param.IsShenyiActived())
	{
		this->SendShenyiInfo();
	}
}

void ShenyiManager::Update(time_t now_second)
{
 	if (0 != m_shenyi_param.clear_upgrade_time && now_second > m_shenyi_param.clear_upgrade_time)
 	{
 		m_shenyi_param.grade_bless_val = 0;
 		m_shenyi_param.clear_upgrade_time = 0;
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);
 		this->SendShenyiInfo();
 	}
}

void ShenyiManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->CheckActive();
	}

//	if (!m_shenyi_param.IsShenyiActived())
//	{
//		unsigned int now = (unsigned int)EngineAdapter::Instance().Time();
//		unsigned int opengame_timestamp = static_cast<unsigned int>(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime());
//		int opengame_day = GetDayIndex(opengame_timestamp, now) + 1;
//
//		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_SHENYI);
//		if (nullptr != fun_open_cfg) //&& opengame_day >= fun_open_cfg->opengame_day)
//		{
//			if (m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id) || m_role->GetLevel() >= fun_open_cfg->role_level)
//			{
//				ShenyiFunction::ActiveShenyi(m_role, false);
//			}
//		}
//	}
}

// 攻击技能特效
void ShenyiManager::OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag)
{
	if (NULL == target || !target->IsAlive())
	{
		return;
	}

	// 攻击有param_b概率造成相当于目标param_c%最大生命值的伤害。（PVP） 冷却param_a毫秒
	static const int SKILL_ID0 = 0;
	UNSTD_STATIC_CHECK(ShenyiParam::SKILL_COUNT > SKILL_ID0);
	if (m_shenyi_param.skill_level_list[SKILL_ID0] > 0 && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		const ShenyiSkillConifg *skill_cfg = LOGIC_CONFIG->GetShenyiConfig().GetSkillCfg(SKILL_ID0, m_shenyi_param.skill_level_list[SKILL_ID0]);
		if (NULL != skill_cfg)
		{
			unsigned long long now_time_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();
			if (now_time_ms > m_shenyi_param.skill_last_colddown_time[SKILL_ID0])
			{
				if (RandomNum(10000) < skill_cfg->param_b)
				{
					// 目标最大百分比血量
					Attribute hurt_injure = static_cast<Attribute>(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * skill_cfg->param_c * ROLE_ATTR_PER);
					if (hurt_injure > base_injure)  // 如果基础伤害较小，则修正。如果基础伤害较大，则取基础伤害
					{
						injure += (hurt_injure - base_injure);
					}

					m_shenyi_param.skill_last_colddown_time[SKILL_ID0] = now_time_ms + skill_cfg->param_a;
				}
			}
		}
	}

	// 幻影
	static const int SKILL_ID2 = 2;
	UNSTD_STATIC_CHECK(ShenyiParam::SKILL_COUNT > SKILL_ID2);
	if (m_shenyi_param.skill_level_list[SKILL_ID2] > 0)
	{
		const ShenyiSkillConifg *skill_cfg = LOGIC_CONFIG->GetShenyiConfig().GetSkillCfg(SKILL_ID2, m_shenyi_param.skill_level_list[SKILL_ID2]);
		if (NULL != skill_cfg)
		{
			if (RandomNum(10000) < skill_cfg->param_a)
			{
				passive_flag |= (1 << EffectBase::PASSIVE_FLAG_SHENYI_HUANYING);

				for (int i = 0; i < skill_cfg->param_b && i < 10; i++)
				{
					injure += (long long)(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI) * 0.5f + skill_cfg->param_c);
				}
			}
		}
	}

	// 审判
	static const int SKILL_ID3 = 3;
	UNSTD_STATIC_CHECK(ShenyiParam::SKILL_COUNT > SKILL_ID3);
	if (m_shenyi_param.skill_level_list[SKILL_ID3] > 0)
	{
		const ShenyiSkillConifg *skill_cfg = LOGIC_CONFIG->GetShenyiConfig().GetSkillCfg(SKILL_ID3, m_shenyi_param.skill_level_list[SKILL_ID3]);
		if (NULL != skill_cfg)
		{
			unsigned long long now_time_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();
			if (now_time_ms > m_shenyi_param.skill_last_colddown_time[SKILL_ID3])
			{
				m_shenyi_param.skill_last_colddown_time[SKILL_ID3] = now_time_ms + skill_cfg->param_d;

				if (RandomNum(10000) < skill_cfg->param_a)
				{
					// 自己加攻
					EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), 3, skill_cfg->param_c, 1);
					effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
					long long value = (long long)(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (skill_cfg->param_b * ROLE_ATTR_PER));
					effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, value, skill_cfg->param_b);
					effect_buff->SetClientEffectType(EffectBase::ECT_UPGRADE_SHENYI_SHENPAN);
					m_role->AddEffect(effect_buff);

					EffectSpecialState *effect_state = new EffectSpecialState(m_role->GetId(), 3, skill_cfg->param_c, EBT_SHENYI_SHENPAN, 1);
					effect_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
					m_role->AddEffect(effect_state);
				}
			}
		}
	}
}

// 受攻击技能特效
void ShenyiManager::OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{
	// 救赎
	//static const int SKILL_ID0 = 0;
	//UNSTD_STATIC_CHECK(ShenyiParam::SKILL_COUNT > SKILL_ID0);
	//if (m_shenyi_param.skill_level_list[SKILL_ID0] > 0)
	//{
	//	const ShenyiSkillConifg *skill_cfg = LOGIC_CONFIG->GetShenyiConfig().GetSkillCfg(SKILL_ID0, m_shenyi_param.skill_level_list[SKILL_ID0]);
	//	if (NULL != skill_cfg)
	//	{
	//		unsigned long now_time_ms = ServerLogic::GetInstServerLogic()->GetPiTime();
	//		if (now_time_ms > m_shenyi_param.skill_last_colddown_time[SKILL_ID0])
	//		{
	//			Attribute current_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	//			if(current_hp + delta_hp <= 0)
	//			{
	//				m_shenyi_param.skill_last_colddown_time[SKILL_ID0] = now_time_ms + skill_cfg->param_c;

	//				delta_hp = 5 - current_hp;

	//				Attribute value = (Attribute)(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (skill_cfg->param_b * ROLE_ATTR_PER));
	//				m_role->ChangeHp(&value, true);
	//			}
	//		}
	//	}
	//}
}

void ShenyiManager::SendShenyiInfo()
{
	Protocol::SCShenyiInfo mi;
	mi.star_level = m_shenyi_param.star_level;
	mi.shenyi_level = m_shenyi_param.level;
	mi.grade = m_shenyi_param.grade;
	mi.grade_bless_val = m_shenyi_param.grade_bless_val;
	mi.clear_upgrade_time = m_shenyi_param.clear_upgrade_time;
	mi.used_imageid = m_shenyi_param.used_imageid;
	UNSTD_STATIC_CHECK(sizeof(mi.active_image_flag) == sizeof(m_shenyi_param.active_image_flag));
	memcpy(mi.active_image_flag, m_shenyi_param.active_image_flag, sizeof(mi.active_image_flag));
	UNSTD_STATIC_CHECK(sizeof(mi.active_special_image_flag) == sizeof(m_shenyi_param.active_special_image_flag));
	memcpy(mi.active_special_image_flag, m_shenyi_param.active_special_image_flag, sizeof(mi.active_special_image_flag));
	mi.shuxingdan_count = m_shenyi_param.shuxingdan_count;
	mi.chengzhangdan_count = m_shenyi_param.chengzhangdan_count;
	mi.equip_skill_level = m_shenyi_param.equip_skill_level;
	memcpy(mi.equip_level_list, m_shenyi_param.equip_level_list, sizeof(mi.equip_level_list));
	memcpy(mi.skill_level_list, m_shenyi_param.skill_level_list, sizeof(mi.skill_level_list));
	memcpy(mi.special_img_grade_list, m_shenyi_param.special_img_grade_list, sizeof(mi.special_img_grade_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&mi, sizeof(mi));
}

int ShenyiManager::GetShenyiAppeID()
{
	return m_shenyi_param.used_imageid;
}

bool ShenyiManager::ActiveImage(int imageid, bool recalc)
{
	if (imageid <= 0 || imageid >= ShenyiParam::MAX_IMAGE_COUNT) return false;

	m_shenyi_param.ActiveImage(imageid);

	if (recalc)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);
	}

	return true;
}

bool ShenyiManager::UseImage(int imageid)
{
	if (imageid < SPECIAL_IMG_START_ID)
	{
		if (!m_shenyi_param.IsImageActive(imageid))
		{
			m_role->NoticeNum(errornum::EN_SHENYI_IMG_NOT_ACTIVE);
			return false;
		}
	}
	else
	{
		if (!m_shenyi_param.IsSpecialImageActive(imageid - SPECIAL_IMG_START_ID))
		{
			m_role->NoticeNum(errornum::EN_SHENYI_IMG_NOT_ACTIVE);
			return false;
		}
	}

	m_shenyi_param.used_imageid = imageid;

	m_role->RefreshApperance(true);

	this->SendShenyiInfo();

	return true;
}

bool ShenyiManager::UnUseImage(int imageid)
{
	if (imageid != m_shenyi_param.used_imageid)
	{
		return false;
	}

	m_shenyi_param.used_imageid = 0;

	m_role->RefreshApperance(true);

	this->SendShenyiInfo();

	return true;
}

bool ShenyiManager::ActiveSpecialImage(int imageid, bool is_notify)
{
	if (m_shenyi_param.IsSpecialImageActive(imageid))
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_SPECIAL_IMG_ALREADY_ACTIVE);
		}

		return false;
	}

	const ShenyiSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetShenyiConfig().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	m_shenyi_param.ActiveSpecialImage(imageid);
	m_role->GetImgFuling()->OnPutOnImg(JINGJIE_TYPE_SHENYI, imageid);
	if (0 == m_shenyi_param.special_img_grade_list[imageid])
	{
		m_shenyi_param.special_img_grade_list[imageid] = 1;
	}
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);
	JinjiesysFuncHelper::Instance().OnActiveSpecialImg(m_role, JINJIE_TYPE_SHENYI, imageid);
	this->UseImage(SPECIAL_IMG_START_ID + imageid);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_SHENYI,imageid,true);

	return true;
}

bool ShenyiManager::UnActiveSpecialImage(int imageid)
{
	if (!m_shenyi_param.IsSpecialImageActive(imageid))
	{
		return false;
	}

	const ShenyiSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetShenyiConfig().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	BitmapFunction::ClearBit(m_shenyi_param.active_special_image_flag, sizeof(m_shenyi_param.active_special_image_flag), imageid);
	if (1 == m_shenyi_param.special_img_grade_list[imageid])
	{
		m_shenyi_param.special_img_grade_list[imageid] = 0;
	}
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);

	if (this->UnUseImage(SPECIAL_IMG_START_ID + imageid))
	{
		this->UseImage(1); //默认再给一个
	}
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_SHENYI,imageid,false);
	return true;
}

void ShenyiManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();
		m_other_capability = 0;
		CharIntAttrs other_per_add_attr;
		double rate = 0.0;						// 属性加成万分比

		{
			// 等级加成
			const ShenyiLevelConfig *level_cfg = LOGIC_CONFIG->GetShenyiConfig().GetLevelConfig(m_shenyi_param.level);
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
 			const ShenyiGradeConfig * grade_config = LOGIC_CONFIG->GetShenyiConfig().GetGradeConfig(m_shenyi_param.grade);
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

				// 下面特殊属性用乘法
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, grade_config->zengshang_per);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, grade_config->zengshang_per);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, grade_config->jianshang_per);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, grade_config->jianshang_per);


				// 大目标特殊形象万分比
				int special_img_per_add = JinjieSysRewardMgr::Instance().GetJinjieSpecialImgExtraAddPer(m_role, JINJIE_TYPE_SHENYI);
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
 				//if (m_shenyi_param.grade_bless_val > 0)
 				//{
 				//	const ShenyiGradeConfig * next_grade_config = LOGIC_CONFIG->GetShenyiConfig().GetGradeConfig(m_shenyi_param.grade + 1);
 				//	const ShenyiGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetGradeConfig(m_shenyi_param.grade);
 				//	if (NULL != next_grade_config && NULL != curr_grade_cfg) 
 				//	{
 				//		double rate = (((double)m_shenyi_param.grade_bless_val) / curr_grade_cfg->bless_val_limit) * (next_grade_config->bless_addition * ROLE_ATTR_PER);
 				//		//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)((next_grade_config->maxhp - grade_config->maxhp) * rate);
 				//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)((next_grade_config->gongji - grade_config->gongji) * rate);
 				//	}
 				//}
				m_attrs_add.Add(grade_attrs.m_attrs);
 			}
 		}

		//{
		//	// 星级加成  
		//	const ShenyiUpStarExpCfg * star_level_config = LOGIC_CONFIG->GetShenyiConfig().GetUpStarExpCfg(m_shenyi_param.star_level);
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
			int active_equip_grade = LOGIC_CONFIG->GetShenyiConfig().GetActiveEquipGrade();
			if (active_equip_grade <= m_shenyi_param.grade)
			{
				for (int i = 0; i < ShenyiParam::EQUIP_COUNT; ++i)
				{
					const ShenyiEquipConifg *old_equip_cfg = LOGIC_CONFIG->GetShenyiConfig().GetOldEquipCfg(i);
					if (NULL != old_equip_cfg)
					{
						int level = m_shenyi_param.old_equip_level_list[i];
						if (level >= 0 && level < old_equip_cfg->max_level)
						{
							rate += old_equip_cfg->equip_level_cfg_list[level].add_percent;
						}
					}
				}
				for (int i = 0; i < ShenyiParam::EQUIP_COUNT; ++ i)
				{
					const ShenyiEquipConifg *equip_cfg = LOGIC_CONFIG->GetShenyiConfig().GetEquipCfg(i);
					if (NULL != equip_cfg)
					{
						int level = m_shenyi_param.equip_level_list[i];
						if (level >= 0 && level < equip_cfg->max_level)
						{
							rate += equip_cfg->equip_level_cfg_list[level].add_percent;
						}
					}	
				}
			}

			{
				// 勋章加成
				rate += m_role->GetXunZhangManager()->GetShenyiAddPer();

				// 星座星魂加成
				int xinhun_add_per = 0;
				m_role->GetChineseZodiac()->GetXingHunTotalAddPer(NULL, NULL, NULL, NULL, &xinhun_add_per, NULL, NULL);
				rate += xinhun_add_per;

				// 成长丹加成，只对阶数和等级属性有效
				// 			if (m_shenyi_param.chengzhangdan_count > 0)
				// 			{
				// 				int percent = LOGIC_CONFIG->GetShenyiConfig().GetChengZhangDanPercent() * m_shenyi_param.chengzhangdan_count;
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

			if (active_equip_grade <= m_shenyi_param.grade)
			{
				for (int i = 0; i < ShenyiParam::EQUIP_COUNT; ++i)
				{
					const ShenyiEquipConifg *old_equip_cfg = LOGIC_CONFIG->GetShenyiConfig().GetOldEquipCfg(i);
					if (NULL != old_equip_cfg)
					{
						int level = m_shenyi_param.old_equip_level_list[i];
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
				for (int i = 0; i < ShenyiParam::EQUIP_COUNT; ++ i)
				{
					const ShenyiEquipConifg *equip_cfg = LOGIC_CONFIG->GetShenyiConfig().GetEquipCfg(i);
					if (NULL != equip_cfg)
					{
						int level = m_shenyi_param.equip_level_list[i];
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
			for (int i = 0; i < ShenyiParam::MAX_SPECIAL_IMAGE_COUNT; ++ i)
			{
				if (m_shenyi_param.IsSpecialImageActive(i))
				{
					// 特殊形象进阶
					const int grade = m_shenyi_param.special_img_grade_list[i];

					const ShenyiSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetSpecialImgUpgradeCfg(i, grade);
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
			if (m_shenyi_param.shuxingdan_count > 0)
			{
				const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_SHENYI, 0);
				if (NULL != cfg_item)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp * m_shenyi_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji * m_shenyi_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu * m_shenyi_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg_item->mingzhong * m_shenyi_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg_item->shanbi * m_shenyi_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg_item->baoji * m_shenyi_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg_item->jianren * m_shenyi_param.shuxingdan_count;
				}
			}
		}

		{
			// 技能属性
			for (int i = 0; i < ShenyiParam::SKILL_COUNT; i++)
			{
				if (m_shenyi_param.skill_level_list[i] > 0)
				{
					const ShenyiSkillConifg *skill_cfg = LOGIC_CONFIG->GetShenyiConfig().GetSkillCfg(i, m_shenyi_param.skill_level_list[i]);
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

		// 阶数百分比加成
// 		{
// 			const ShenyiOtherConfig &other_cfg = LOGIC_CONFIG->GetShenyiConfig().GetOtherCfg();
// 			if (m_shenyi_param.grade >= other_cfg.extra_attrs_per_grade && other_cfg.extra_attrs_per > 0)
// 			{
// 				for (int i = CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX; ++i)
// 				{
// 					m_attrs_add.m_attrs[i] += static_cast<Attribute>(m_attrs_add.m_attrs[i] * ROLE_ATTR_PER * other_cfg.extra_attrs_per);
// 				}
// 			}
// 		}
		// 其他百分比属性加成，与上面的全属性加成加算
		{
			m_attrs_add.AddAttrs(&other_per_add_attr);
		}
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_SHENYI, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_SHENYI, m_other_capability);

	//策划需求，神翼的战力算入女神战力中
	m_role->GetCapability()->SetCap(CAPABILITY_TYPE_XIANNV, m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_XIANNV) + m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENYI));

	m_shenyi_param.capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENYI);

	base_add.Add(m_attrs_add.m_attrs, true);
}

ShenyiParam * ShenyiManager::GetShenyiParam()
{
	return &m_shenyi_param;
}

void ShenyiManager::PrintShenyi(const char *reason, int param1, int param2, int param3, int param4, int param5)
{
	UNSTD_STATIC_CHECK(4 == ShenyiParam::EQUIP_COUNT);
	UNSTD_STATIC_CHECK(4 == ShenyiParam::SKILL_COUNT);
	const int image_hex_len = ShenyiParam::MAX_ACTIVE_IMAGE_BYTE * 2 + 1;
	const int special_image_hex_len = ShenyiParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE * 2 + 1;
	char active_image_hex[image_hex_len] = { 0 };
	char active_special_image_hex[special_image_hex_len] = { 0 };
	BinToHex((const char*)m_shenyi_param.active_image_flag, sizeof(m_shenyi_param.active_image_flag), active_image_hex);
	BinToHex((const char*)m_shenyi_param.active_special_image_flag, sizeof(m_shenyi_param.active_special_image_flag), active_special_image_hex);
	gamelog::g_log_shenyi.printf(LL_INFO, "Shenyi role(%d,%s) %s(%d,%d,%d,%d,%d) "
		"level:%d, grade:%d,%d, used_imageid:%d shuxingdan_count:%d chengzhangdan_count:%d, image_flag:%s, special_image_flag:%s"
		"equip_level_list:%d, %d, %d, %d"
		"skill_level_list:%d, %d, %d, %d",
		m_role->GetUID(), m_role->GetName(),
		reason, param1, param2, param3, param4, param5,
		(int)m_shenyi_param.level,
		(int)m_shenyi_param.grade, (int)m_shenyi_param.grade_bless_val,
		(int)m_shenyi_param.used_imageid,
		(int)m_shenyi_param.shuxingdan_count, (int)m_shenyi_param.chengzhangdan_count,
		active_image_hex, active_special_image_hex,
		(int)m_shenyi_param.equip_level_list[0], (int)m_shenyi_param.equip_level_list[1],
		(int)m_shenyi_param.equip_level_list[2], (int)m_shenyi_param.equip_level_list[3],
		(int)m_shenyi_param.skill_level_list[0], (int)m_shenyi_param.skill_level_list[1], (int)m_shenyi_param.skill_level_list[2], (int)m_shenyi_param.skill_level_list[3]);
}

void ShenyiManager::OnCompleteTask(TaskID task_id)
{
	this->CheckActive();
//	if (m_shenyi_param.IsShenyiActived()) return;
//
//	unsigned int now = (unsigned int)EngineAdapter::Instance().Time();
//	unsigned int opengame_timestamp = static_cast<unsigned int>(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime());
//	int opengame_day = GetDayIndex(opengame_timestamp, now) + 1;
//
//	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_SHENYI);
//
//	if (nullptr != fun_open_cfg  && fun_open_cfg->task_id == task_id )//)&& opengame_day >= fun_open_cfg->opengame_day)
//	{
//		ShenyiFunction::ActiveShenyi(m_role);
//	}
}

void ShenyiManager::OnRoleUpLevel(int level)
{
	this->CheckActive();
//	if (m_shenyi_param.IsShenyiActived()) return;
//
//	unsigned int now = (unsigned int)EngineAdapter::Instance().Time();
//	unsigned int opengame_timestamp = static_cast<unsigned int>(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime());
//	int opengame_day = GetDayIndex(opengame_timestamp, now) + 1;
//
//	const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_SHENYI);
//
//	if (nullptr != fun_open_cfg  && level >= fun_open_cfg->role_level )//&& opengame_day >= fun_open_cfg->opengame_day)
//	{
//		ShenyiFunction::ActiveShenyi(m_role);
//	}
}

bool ShenyiManager::OnUseShuxingdan(int slot_idx, short &use_count)
{
	if (slot_idx < 0 || slot_idx >= SHUXINGDAN_MAX_TYPE || use_count <= 0)
	{
		return false;
	}

	const ShenyiGradeConfig *grade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetGradeConfig(m_shenyi_param.grade);
	if (NULL == grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_SHENYI_SHUXINGDAN_USE_LIMIT_NUM);
		return false;
	}

	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_SHENYI, 0);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_shenyi_param.grade < cfg_item->order_limit)
	{
		gstr::SendError(m_role, "shuxingdan_use_order_limit_1", SHUXINGDAN_TYPE_SHENYI, cfg_item->order_limit - 1, slot_idx); // 客户端阶数-1
		return false;
	}

	if (m_shenyi_param.shuxingdan_count + use_count > cfg_item->use_limit_num)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	int special_img_add_count = 0;
	for (int i = 0;i < ShenyiParam::MAX_SPECIAL_IMAGE_COUNT; i++)
	{
		if (!m_shenyi_param.IsSpecialImageActive(i))
			continue;

		const ShenyiSpecialImgUpgradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetSpecialImgUpgradeCfg(i,m_shenyi_param.special_img_grade_list[i]);
		if (NULL != special_img_grade_cfg)
		{
			special_img_add_count += special_img_grade_cfg->shuxingdan_count;
		}
	}

	// 策划需求，修正真正使用数量
	int can_use_count = grade_cfg->shuxingdan_limit + special_img_add_count - m_shenyi_param.shuxingdan_count;
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

	m_shenyi_param.shuxingdan_count += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);
	this->SendShenyiInfo();

	return true;
}

bool ShenyiManager::OnUseChengzhangdan(int use_count)
{
	const ShenyiGradeConfig *grade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetGradeConfig(m_shenyi_param.grade);
	if (NULL == grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_CHENGZHANGDAN_USE_LIMIT_NUM);
		return false;
	}

	int special_img_add_count = 0;
	for (int i = 0;i < ShenyiParam::MAX_SPECIAL_IMAGE_COUNT; i++)
	{
		if (!m_shenyi_param.IsSpecialImageActive(i))
			continue;

		const ShenyiSpecialImgUpgradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetSpecialImgUpgradeCfg(i,m_shenyi_param.special_img_grade_list[i]);
		if (NULL != special_img_grade_cfg)
		{
			special_img_add_count += special_img_grade_cfg->chengzhangdan_count;
		}
	}

	if (m_shenyi_param.chengzhangdan_count + use_count > grade_cfg->chengzhangdan_limit + special_img_add_count)
	{
		m_role->NoticeNum(errornum::EN_CHENGZHANGDAN_USE_LIMIT_NUM);
		return false;
	}

	m_shenyi_param.chengzhangdan_count += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);
	this->SendShenyiInfo();

	return true;
}

int ShenyiManager::RefreshShenyiMinEquipLevel()
{
	int min = m_shenyi_param.equip_level_list[0];

	for (int i = 0; i < ShenyiParam::EQUIP_COUNT; ++i)
	{
		min = m_shenyi_param.equip_level_list[i] < min ? m_shenyi_param.equip_level_list[i] : min;
	}

	return min;
}

void ShenyiManager::CheckActive()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (!m_shenyi_param.IsShenyiActived())
	{
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_SHENYI);
		if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day)
		{
			if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
			{
				ShenyiFunction::ActiveShenyi(m_role, false);
			}
		}
	}
}

// 幻化特殊战力加成
void ShenyiManager::HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add)
{
	if (!LOGIC_CONFIG->GetShenyiConfig().IsAddSpecialCap())
	{
		return;
	}

	auto cfg_item = LOGIC_CONFIG->GetShenyiConfig().GetHuanHuaSpecialCapAddCfg(huanhua_id);
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

int ShenyiManager::GetSpecialImageLevelById(int special_image_id)
{
	if (special_image_id < 0 || special_image_id >= ShenyiParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return 0;
	}
	if (!m_shenyi_param.IsSpecialImageActive(special_image_id))
	{
		return 0;
	}
	return m_shenyi_param.special_img_grade_list[special_image_id];
}

int ShenyiManager::GetGrade()
{
	if (!m_shenyi_param.IsShenyiActived())
	{
		return 0;
	}
	return m_shenyi_param.grade;
}
