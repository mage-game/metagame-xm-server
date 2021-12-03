#include "halomanager.hpp"

#include "gamelog.h"
#include "engineadapter.h"

#include "obj/character/role.h"

#include "servercommon/errornum.h"
#include "protocal/msghalo.hpp"

#include "scene/scene.h"
#include "haloconfig.hpp"
#include "gstr.h"

#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/shuxingdanconfig.hpp"
#include "servercommon/vipdef.hpp"
#include "other/halo/halofunction.hpp"
#include "globalconfig/globalconfig.h"
#include "other/camp/campconfig.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "serverlogic.h"

#include "effect/skilleffect/effectbuff.hpp"
#include "task/taskrecorder.h"
#include "other/capability/capability.hpp"
#include "other/xunzhang/xunzhangmanager.hpp"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include "other/imgfuling/imgfuling.hpp"
#include "servercommon/imgfulingdef.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "other/talent/talent.hpp"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "other/jinjiesysreward/jinjiesysrewardmgr.hpp"
#include "other/dressingroom/dressingroomconfig.hpp"
#include "other/dressingroom/dressingroommgr.hpp"
#include "common/bintohex.h"
#include "servercommon/serverconfig/crossconfig.hpp"

HaloManager::HaloManager()
	: m_role(NULL),m_other_capability(0), m_last_add_fangyu(0), m_skill0_last_colddown_time(0)
{
	
}

HaloManager::~HaloManager()
{

}

void HaloManager::Init(Role *role, const HaloParam &halo_param)
{
	m_role = role;
	m_halo_param = halo_param;

	if (halo_param.old_grade_bless_val != 0)
	{
		m_halo_param.grade_bless_val = halo_param.old_grade_bless_val;
		m_halo_param.old_grade_bless_val = 0;
	}
}

void HaloManager::GetOtherInitParam(HaloParam *param)
{
	*param = m_halo_param;
}

void HaloManager::GetRoleAppearance(RoleAppearance *appearance)
{
	if (NULL != appearance)
	{
		appearance->halo_used_imageid = m_halo_param.used_imageid;
	}
}

void HaloManager::OnLogin()
{
	this->CheckActive();
}

void HaloManager::OnRoleLevelChange(int level)
{
	m_halo_param.level = level;

	if (m_halo_param.IsHaloActived())
	{
		this->SendHaloInfo();
	}
}

void HaloManager::Update(time_t now_second)
{
	const HaloGradeConfig *grade_cfg = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(m_halo_param.grade);
	if (nullptr != grade_cfg && 1 == grade_cfg->is_clear_bless && 0 != m_halo_param.clear_upgrade_time && now_second > m_halo_param.clear_upgrade_time)
	{
		m_halo_param.grade_bless_val = 0;
		//m_halo_param.clear_upgrade_time = 0;
		m_halo_param.clear_upgrade_time = static_cast<unsigned int>(now_second) + EngineAdapter::Instance().NextDayInterval(5, 0, 0);
		//this->sendhaloinfo();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);
		this->SendHaloInfo();
	}

	if (0 != m_halo_param.reward_limit_time && now_second > m_halo_param.reward_limit_time)
	{
		m_halo_param.clear_upgrade_time = 0;
		this->SendHaloInfo();
	}

	this->OnHaloSkillEffect(now_second);
}

//技能特效
void HaloManager::OnHaloSkillEffect(time_t now_sec)
{
	if(!m_role->IsAlive())
	{
		return;
	}

	unsigned long long now_time_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();
	// 回春
	static const int SKILL_ID2 = 2;
	UNSTD_STATIC_CHECK(HaloParam::SKILL_COUNT > SKILL_ID2);
	if (m_halo_param.skill_level_list[SKILL_ID2] > 0 && static_cast<unsigned int>(now_time_ms) > m_halo_param.skill_last_colddown_time[SKILL_ID2])
	{
		const HaloSkillConifg *skill_cfg = LOGIC_CONFIG->GetHaloConfig().GetSkillCfg(SKILL_ID2, m_halo_param.skill_level_list[SKILL_ID2]);
		if (NULL != skill_cfg)
		{
			m_halo_param.skill_last_colddown_time[SKILL_ID2] = static_cast<unsigned int>(now_time_ms) + skill_cfg->param_a;

			Attribute current_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			Attribute max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			if (current_hp < max_hp)
			{
				Attribute change_hp = skill_cfg->param_b;
				if (skill_cfg->param_b > max_hp - current_hp)
				{
					change_hp = max_hp - current_hp;
				}
				
				m_role->ChangeHp(&change_hp, true);
			}
		}
	}

	// 不屈
	static const int SKILL_ID3 = 3;
	UNSTD_STATIC_CHECK(HaloParam::SKILL_COUNT > SKILL_ID3);
	if (m_halo_param.skill_level_list[SKILL_ID3] > 0)
	{
		const HaloSkillConifg *skill_cfg = LOGIC_CONFIG->GetHaloConfig().GetSkillCfg(SKILL_ID3, m_halo_param.skill_level_list[SKILL_ID3]);
		if (NULL != skill_cfg)
		{
			Attribute current_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			Attribute max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			
			int add_fangyu_per = (int)(max_hp * (skill_cfg->param_a * ROLE_ATTR_PER));
			if (add_fangyu_per > 0)
			{
				Attribute add_fangyu = (int)(((max_hp - current_hp) / add_fangyu_per) * skill_cfg->param_b);

				if (m_last_add_fangyu != add_fangyu)
				{
					m_last_add_fangyu = add_fangyu;

					if (add_fangyu <= 0)
					{
						m_role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_ADD_FANGYU, m_role->GetId(), EffectBase::PRODUCT_METHOD_SKILL, SKILL_ID3);
					}
					else
					{
						EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), SKILL_ID3, 7200000, (char)m_halo_param.skill_level_list[SKILL_ID3]);
						effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_5);
						effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU, add_fangyu, 0);
						m_role->AddEffect(effect_buff);
					}
				}
			}
		}
	}
}

// 攻击生命值低于param_b%的目标时，造成伤害提高param_c%，持续param_d毫秒。 冷却param_a毫秒
void HaloManager::OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure)
{
	static const int SKILL_ID0 = 0;
	UNSTD_STATIC_CHECK(HaloParam::SKILL_COUNT > SKILL_ID0);

	if (nullptr == target || base_injure <= 0) return;

	const HaloSkillConifg *skill_cfg = LOGIC_CONFIG->GetHaloConfig().GetSkillCfg(SKILL_ID0, m_halo_param.skill_level_list[SKILL_ID0]);
	if (nullptr == skill_cfg) return;

	unsigned long long now_sec_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();
	Attribute cur_hp = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	Attribute target_hp = static_cast<Attribute>(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (skill_cfg->param_b * ROLE_ATTR_PER));

	base_injure = static_cast<Attribute>(base_injure * (skill_cfg->param_c * ROLE_ATTR_PER));

	if (now_sec_ms < m_halo_param.skill_last_colddown_time[SKILL_ID0] + skill_cfg->param_a)
	{
		if (now_sec_ms < m_halo_param.skill_last_colddown_time[SKILL_ID0] + skill_cfg->param_d)
		{
			if (cur_hp < target_hp)
			{
				injure += base_injure;
			}
		}
	}
	else
	{
		if (cur_hp < target_hp)
		{
			injure += base_injure;
			m_halo_param.skill_last_colddown_time[SKILL_ID0] = now_sec_ms;
		}
	}
}

void HaloManager::SendHaloInfo()
{
	Protocol::SCHaloInfo mi;
	mi.halo_level = m_halo_param.level;
	mi.grade = m_halo_param.grade;
	mi.star_level = m_halo_param.star_level;
	mi.clear_upgrade_time = m_halo_param.clear_upgrade_time;
	mi.reward_limit_time = m_halo_param.reward_limit_time;
	mi.used_imageid = m_halo_param.used_imageid;
	UNSTD_STATIC_CHECK(sizeof(mi.active_image_flag) == sizeof(m_halo_param.active_image_flag));
	memcpy(mi.active_image_flag, m_halo_param.active_image_flag, sizeof(mi.active_image_flag));
	mi.shuxingdan_count = m_halo_param.shuxingdan_count;
	mi.chengzhangdan_count = m_halo_param.chengzhangdan_count;
	mi.grade_bless_val = m_halo_param.grade_bless_val;
	UNSTD_STATIC_CHECK(sizeof(mi.active_special_image_flag) == sizeof(m_halo_param.active_special_image_flag));
	memcpy(mi.active_special_image_flag, m_halo_param.active_special_image_flag, sizeof(mi.active_special_image_flag));
	mi.equip_skill_level = m_halo_param.equip_skill_level;
	memcpy(mi.equip_level_list, m_halo_param.equip_level_list, sizeof(mi.equip_level_list));
	memcpy(mi.skill_level_list, m_halo_param.skill_level_list, sizeof(mi.skill_level_list));
	memcpy(mi.special_img_grade_list, m_halo_param.special_img_grade_list, sizeof(mi.special_img_grade_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&mi, sizeof(mi));
}

int HaloManager::GetHaloAppeID()
{
	return m_halo_param.used_imageid;
}

bool HaloManager::ActiveImage(int imageid, bool recalc)
{
	if (imageid <= 0 || imageid >= HaloParam::MAX_IMAGE_COUNT) return false;

	m_halo_param.ActiveImage(imageid);

	if (recalc)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);
	}

	return true;
}

bool HaloManager::UseImage(int imageid)
{
	if (imageid < SPECIAL_IMG_START_ID)
	{
		if (!m_halo_param.IsImageActive(imageid))
		{
			m_role->NoticeNum(errornum::EN_HALO_IMG_NOT_ACTIVE);
			return false;
		}
	}
	else
	{
		if (!m_halo_param.IsSpecialImageActive(imageid - SPECIAL_IMG_START_ID))
		{
			m_role->NoticeNum(errornum::EN_HALO_IMG_NOT_ACTIVE);
			return false;
		}
	}

	m_halo_param.used_imageid = imageid;

	m_role->RefreshApperance(true);

	this->SendHaloInfo();

	return true;
}

bool HaloManager::UnUseImage(int imageid)
{
	if (imageid != m_halo_param.used_imageid)
	{
		return false;
	}

	m_halo_param.used_imageid = 0;

	m_role->RefreshApperance(true);

	this->SendHaloInfo();

	return true;
}

bool HaloManager::ActiveSpecialImage(int imageid, bool is_notify)
{
	if (m_halo_param.IsSpecialImageActive(imageid))
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_SPECIAL_IMG_ALREADY_ACTIVE);
		}

		return false;
	}

	const HaloSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetHaloConfig().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	m_halo_param.ActiveSpecialImage(imageid);
	m_role->GetImgFuling()->OnPutOnImg(JINGJIE_TYPE_HALO, imageid);
	if (0 == m_halo_param.special_img_grade_list[imageid])
	{
		m_halo_param.special_img_grade_list[imageid] = 1;
	}
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);
	JinjiesysFuncHelper::Instance().OnActiveSpecialImg(m_role, JINJIE_TYPE_HALO, imageid);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_HALO,imageid,true);
	this->UseImage(SPECIAL_IMG_START_ID + imageid);
	return true;
}

bool HaloManager::UnActiveSpecialImage(int imageid)
{
	if (!m_halo_param.IsSpecialImageActive(imageid))
	{
		return false;
	}

	const HaloSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetHaloConfig().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	BitmapFunction::ClearBit(m_halo_param.active_special_image_flag, sizeof(m_halo_param.active_special_image_flag), imageid);
	if (1 == m_halo_param.special_img_grade_list[imageid])
	{
		m_halo_param.special_img_grade_list[imageid] = 0;
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_HALO, imageid, false);
	if (this->UnUseImage(SPECIAL_IMG_START_ID + imageid))
	{
		this->UseImage(1); //默认再给一个
	}

	return true;
}

void HaloManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
//	if (!m_halo_param.IsHaloActived())
//	{
//		return;
//	}

	if (is_recalc)
	{
		m_attrs_add.Reset();
		m_other_capability = 0;
		CharIntAttrs other_per_add_attr;
		double rate = 0.0;						// 属性加成万分比

		{
			// 等级加成
			const HaloLevelConfig *level_cfg = LOGIC_CONFIG->GetHaloConfig().GetLevelConfig(m_halo_param.level);
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
			const HaloGradeConfig * grade_config = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(m_halo_param.grade);
			if (NULL != grade_config) 
			{
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += grade_config->maxhp;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += grade_config->gongji;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += grade_config->fangyu;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += grade_config->mingzhong;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += grade_config->shanbi;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += grade_config->baoji;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += grade_config->jianren;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += grade_config->extra_zengshang;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += grade_config->extra_mianshang;
				// 特殊属性
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += grade_config->per_jingzhun;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += grade_config->per_baoji;

// 				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += grade_config->per_mianshang;
// 				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += grade_config->per_pofang;
// 				grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += grade_config->pvp_zengshang_per;
// 				grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += grade_config->pve_zengshang_per;
// 				grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += grade_config->pvp_jianshang_per;
// 				grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += grade_config->pve_jianshang_per;

				// 下面特殊属性用乘法
				grade_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, grade_config->per_mianshang);
				grade_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, grade_config->per_pofang);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, grade_config->pvp_zengshang_per);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, grade_config->pve_zengshang_per);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, grade_config->pvp_jianshang_per);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, grade_config->pve_jianshang_per);
				// 大目标特殊形象万分比
				int special_img_per_add = JinjieSysRewardMgr::Instance().GetJinjieSpecialImgExtraAddPer(m_role, JINJIE_TYPE_HALO);
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
				//if (m_halo_param.grade_bless_val > 0)
				//{
				//	const HaloGradeConfig * next_grade_config = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(m_halo_param.grade + 1);
				//	const HaloGradeConfig * curr_grade_config = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(m_halo_param.grade);
				//	if (NULL != next_grade_config && NULL != curr_grade_config && curr_grade_config->bless_val_limit > 0) 
				//	{
				//		double rate = (((double)m_halo_param.grade_bless_val) / curr_grade_config->bless_val_limit) * (next_grade_config->bless_addition * ROLE_ATTR_PER);
				//		//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)((next_grade_config->maxhp - grade_config->maxhp) * rate);
				//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)((next_grade_config->gongji - grade_config->gongji) * rate);
				//		//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)((next_grade_config->fangyu - grade_config->fangyu) * rate);
				//		//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (Attribute)((next_grade_config->mingzhong - grade_config->mingzhong) * rate);
				//		//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (Attribute)((next_grade_config->shanbi - grade_config->shanbi) * rate);
				//		//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (Attribute)((next_grade_config->baoji - grade_config->baoji) * rate);
				//		//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (Attribute)((next_grade_config->jianren - grade_config->jianren) * rate);
				//	}
				//}
				m_attrs_add.Add(grade_attrs.m_attrs);

				//天赋加成
				m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_HALO_BASE, grade_attrs, m_attrs_add);

				this->CalcBaseAttrAddFromSkill(m_attrs_add);
			}
		}

		{
			// 天赋加成
			m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_HALO, m_attrs_add, m_attrs_add);
		}

		{
			// 星级加成  
			const HaloUpStarExpCfg * star_level_config = LOGIC_CONFIG->GetHaloConfig().GetUpStarExpCfg(m_halo_param.star_level);
			if (NULL != star_level_config) 
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += star_level_config->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += star_level_config->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += star_level_config->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += star_level_config->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += star_level_config->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += star_level_config->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += star_level_config->jianren;
			}
		}

		{
			// 光环装备
			int active_equip_grade = LOGIC_CONFIG->GetHaloConfig().GetActiveEquipGrade();
			if (active_equip_grade <= m_halo_param.grade)
			{
				for (int i = 0; i < HaloParam::EQUIP_COUNT; ++ i)
				{
					const HaloEquipConifg *equip_cfg = LOGIC_CONFIG->GetHaloConfig().GetEquipCfg(i);
					if (NULL != equip_cfg)
					{
						int level = m_halo_param.equip_level_list[i];
						if (level >= 0 && level < equip_cfg->max_level)
						{
							rate += equip_cfg->level_cfg[level].add_percent;
						}
					}
				}
			}

			{
				// 勋章加成
				rate += m_role->GetXunZhangManager()->GetHaloAddPer();

				// 星座星魂加成
				int xinhun_add_per = 0;
				m_role->GetChineseZodiac()->GetXingHunTotalAddPer(NULL, NULL, &xinhun_add_per, NULL, NULL, NULL, NULL);
				rate += xinhun_add_per;

				// 成长丹加成，只对阶数和等级属性有效
				// 			if (m_halo_param.chengzhangdan_count > 0)
				// 			{
				// 				int percent = LOGIC_CONFIG->GetHaloConfig().GetChengZhangDanPercent() * m_halo_param.chengzhangdan_count;
				// 				if (percent > 0)
				// 				{
				// 					rate += percent;
				// 				}
				// 				
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

			if (active_equip_grade <= m_halo_param.grade)
			{
				for (int i = 0; i < HaloParam::EQUIP_COUNT; ++ i)
				{
					const HaloEquipConifg *equip_cfg = LOGIC_CONFIG->GetHaloConfig().GetEquipCfg(i);
					if (NULL != equip_cfg)
					{
						int level = m_halo_param.equip_level_list[i];
						if (level >= 0 && level < equip_cfg->max_level)
						{
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += equip_cfg->level_cfg[level].maxhp;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += equip_cfg->level_cfg[level].gongji;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += equip_cfg->level_cfg[level].fangyu;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += equip_cfg->level_cfg[level].mingzhong;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += equip_cfg->level_cfg[level].shanbi;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += equip_cfg->level_cfg[level].baoji;
							m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += equip_cfg->level_cfg[level].jianren;
						}
					}
				}
			}
		}

		{
			// 特殊形象
			for (int i = 0; i < HaloParam::MAX_SPECIAL_IMAGE_COUNT; ++ i)
			{
				if (m_halo_param.IsSpecialImageActive(i))
				{
					// 特殊形象进阶
					const int grade = m_halo_param.special_img_grade_list[i];

					const HaloSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetHaloConfig().GetSpecialImgUpgradeCfg(i, grade);
					if (NULL != upgrade_cfg)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += upgrade_cfg->maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += upgrade_cfg->gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += upgrade_cfg->fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += upgrade_cfg->mingzhong ;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += upgrade_cfg->shanbi;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += upgrade_cfg->baoji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += upgrade_cfg->jianren;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += upgrade_cfg->extra_zengshang;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += upgrade_cfg->extra_mianshang;
						// 特殊属性
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += upgrade_cfg->per_jingzhun;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += upgrade_cfg->per_baoji;
// 						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += upgrade_cfg->per_mianshang;
// 						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += upgrade_cfg->per_pofang;
// 						m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += upgrade_cfg->pvp_zengshang_per;
// 						m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += upgrade_cfg->pve_zengshang_per;
// 						m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += upgrade_cfg->pvp_jianshang_per;
// 						m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += upgrade_cfg->pve_jianshang_per;

						// 下面特殊属性用乘法
						m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, upgrade_cfg->per_mianshang);
						m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, upgrade_cfg->per_pofang);
						m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, upgrade_cfg->pvp_zengshang_per);
						m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, upgrade_cfg->pve_zengshang_per);
						m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, upgrade_cfg->pvp_jianshang_per);
						m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, upgrade_cfg->pve_jianshang_per);
					}

					this->HuanHuaSpecialCapAdd(i, grade, m_attrs_add);
				}
			}
		}

		{
			// 属性丹
			if (m_halo_param.shuxingdan_count > 0)
			{
				const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_HALO, 0);
				if (NULL != cfg_item)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp * m_halo_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji * m_halo_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu * m_halo_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg_item->mingzhong * m_halo_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg_item->shanbi * m_halo_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg_item->baoji * m_halo_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg_item->jianren * m_halo_param.shuxingdan_count;
				}
			}
		}

		{
			// 技能属性
			for (int i = 0; i < HaloParam::SKILL_COUNT; i++)
			{
				if (m_halo_param.skill_level_list[i] > 0)
				{
					const HaloSkillConifg *skill_cfg = LOGIC_CONFIG->GetHaloConfig().GetSkillCfg(i, m_halo_param.skill_level_list[i]);
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

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_HALO, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_HALO, m_other_capability);

	m_halo_param.capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_HALO);

	base_add.Add(m_attrs_add.m_attrs, true);
}

HaloParam * HaloManager::GetHaloParam()
{
	return &m_halo_param;
}

void HaloManager::PrintHalo(const char *reason, int param1, int param2, int param3, int param4, int param5)
{
	UNSTD_STATIC_CHECK(4 == HaloParam::EQUIP_COUNT);
	UNSTD_STATIC_CHECK(4 == HaloParam::SKILL_COUNT);
	const int image_hex_len = HaloParam::MAX_ACTIVE_IMAGE_BYTE * 2 + 1;
	const int special_image_hex_len = HaloParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE * 2 + 1;
	char active_image_hex[image_hex_len] = { 0 };
	char active_special_image_hex[special_image_hex_len] = { 0 };
	BinToHex((const char*)m_halo_param.active_image_flag, sizeof(m_halo_param.active_image_flag), active_image_hex);
	BinToHex((const char*)m_halo_param.active_special_image_flag, sizeof(m_halo_param.active_special_image_flag), active_special_image_hex);
	gamelog::g_log_halo.printf(LL_INFO, "Halo role(%d,%s) %s(%d,%d,%d,%d,%d) "
		"level:%d, grade:%d,%d, used_imageid:%d shuxingdan_count:%d chengzhangdan_count:%d, image_flag:%s, special_image_flag:%s"
		"equip_level_list:%d, %d, %d, %d"
		"skill_level_list:%d, %d, %d, %d",
		m_role->GetUID(), m_role->GetName(),
		reason, param1, param2, param3, param4, param5,
		(int)m_halo_param.level,
		(int)m_halo_param.grade, (int)m_halo_param.grade_bless_val,
		(int)m_halo_param.used_imageid,
		(int)m_halo_param.shuxingdan_count, (int)m_halo_param.chengzhangdan_count,
		active_image_hex, active_special_image_hex,
		(int)m_halo_param.equip_level_list[0], (int)m_halo_param.equip_level_list[1], (int)m_halo_param.equip_level_list[2], (int)m_halo_param.equip_level_list[3],
		(int)m_halo_param.skill_level_list[0], (int)m_halo_param.skill_level_list[1], (int)m_halo_param.skill_level_list[2], (int)m_halo_param.skill_level_list[3]);
}

void HaloManager::OnCompleteTask(TaskID task_id)
{
 //	if (LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenTaskID(FUN_OPEN_TYPE_HALO) == task_id)
 //	{
 //		HaloFunction::ActiveHalo(m_role);
 //	}
	this->CheckActive();
}

void HaloManager::OnRoleUpLevel(int level)
{
//	if (m_halo_param.IsHaloActived()) return;

//	if (level >= LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenRoleLevel(FUN_OPEN_TYPE_HALO))
//	{
//		HaloFunction::ActiveHalo(m_role);
//	}
	this->CheckActive();
}

bool HaloManager::OnUseShuxingdan(int slot_idx, short &use_count)
{
	if (slot_idx < 0 || slot_idx >= SHUXINGDAN_MAX_TYPE || use_count <= 0)
	{
		return false;

	}

	const HaloGradeConfig *grade_cfg = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(m_halo_param.grade);
	if (NULL == grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_HALO_SHUXINGDAN_USE_LIMIT_NUM);
		return false;
	}

	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_HALO, 0);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_halo_param.grade < cfg_item->order_limit)
	{
		gstr::SendError(m_role, "shuxingdan_use_order_limit_1", SHUXINGDAN_TYPE_HALO, cfg_item->order_limit - 1, slot_idx); // 客户端阶数-1
		return false;
	}

	if (m_halo_param.shuxingdan_count + use_count > cfg_item->use_limit_num)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	int special_img_add_count = 0;
	for (int i = 0;i < HaloParam::MAX_SPECIAL_IMAGE_COUNT; i++)
	{
		if (!m_halo_param.IsSpecialImageActive(i))
			continue;

		const HaloSpecialImgUpgradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetHaloConfig().GetSpecialImgUpgradeCfg(i,m_halo_param.special_img_grade_list[i]);
		if (NULL != special_img_grade_cfg)
		{
			special_img_add_count += special_img_grade_cfg->shuxingdan_count;
		}
	}

	// 策划需求，修正真正使用数量
	int can_use_count = grade_cfg->shuxingdan_limit + special_img_add_count - m_halo_param.shuxingdan_count;
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

	m_halo_param.shuxingdan_count += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);
	this->SendHaloInfo();

	return true;
}

bool HaloManager::OnUseChengzhangdan(int use_count)
{
	const HaloGradeConfig *grade_cfg = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(m_halo_param.grade);
	if (NULL == grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_CHENGZHANGDAN_USE_LIMIT_NUM);
		return false;
	}

	int special_img_add_count = 0;
	for (int i = 0;i < HaloParam::MAX_SPECIAL_IMAGE_COUNT; i++)
	{
		if (!m_halo_param.IsSpecialImageActive(i))
			continue;

		const HaloSpecialImgUpgradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetHaloConfig().GetSpecialImgUpgradeCfg(i,m_halo_param.special_img_grade_list[i]);
		if (NULL != special_img_grade_cfg)
		{
			special_img_add_count += special_img_grade_cfg->chengzhangdan_count;
		}
	}

	if (m_halo_param.chengzhangdan_count + use_count > grade_cfg->chengzhangdan_limit + special_img_add_count)
	{
		m_role->NoticeNum(errornum::EN_CHENGZHANGDAN_USE_LIMIT_NUM);
		return false;
	}

	m_halo_param.chengzhangdan_count += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);
	this->SendHaloInfo();

	return true;
}

int HaloManager::RefreshHaloMinEquipLevel()
{
	int min = m_halo_param.equip_level_list[0];

	for (int i = 1; i < HaloParam::EQUIP_COUNT; ++i)
	{
		min = m_halo_param.equip_level_list[i] < min ? m_halo_param.equip_level_list[i] : min;
	}

	return min;
}

void HaloManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->CheckActive();
	}
}

void HaloManager::CheckActive()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (!m_halo_param.IsHaloActived())
	{
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_HALO);
		if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day)
		{
			if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
			{
				HaloFunction::ActiveHalo(m_role);
			}
		}
	}
}

// 幻化特殊战力加成
void HaloManager::HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add)
{
	if (!LOGIC_CONFIG->GetHaloConfig().IsAddSpecialCap())
	{
		return;
	}

	auto cfg_item = LOGIC_CONFIG->GetHaloConfig().GetHuanHuaSpecialCapAddCfg(huanhua_id);
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

int HaloManager::GetSpecialImageLevelById(int imageid)
{
	if (imageid < 0 || imageid >= HaloParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return 0;
	}

	if (!m_halo_param.IsSpecialImageActive(imageid)) return 0;

	return m_halo_param.special_img_grade_list[imageid];
}

void HaloManager::CalcBaseAttrAddFromSkill(CharIntAttrs & base_add)
{
	// 阶数加成
	const HaloGradeConfig * grade_config = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(m_halo_param.grade);
	if (NULL == grade_config)
	{
		return;
	}

	int total_add_per = 0;
	for (int i = 0; i < HaloParam::SKILL_COUNT; i++)
	{
		if (m_halo_param.skill_level_list[i] <= 0)
		{
			continue;
		}
		const HaloSkillConifg *skill_cfg = LOGIC_CONFIG->GetHaloConfig().GetSkillCfg(i, m_halo_param.skill_level_list[i]);
		if (NULL == skill_cfg || skill_cfg->base_attr_add_per <= 0)
		{
			continue;
		}
		total_add_per += skill_cfg->base_attr_add_per;
	}
	if (total_add_per > 0)
	{
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(grade_config->maxhp * (total_add_per * ROLE_ATTR_PER));
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(grade_config->gongji * (total_add_per * ROLE_ATTR_PER));
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(grade_config->fangyu * (total_add_per * ROLE_ATTR_PER));
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(grade_config->mingzhong * (total_add_per * ROLE_ATTR_PER));
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(grade_config->shanbi * (total_add_per * ROLE_ATTR_PER));
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(grade_config->baoji * (total_add_per * ROLE_ATTR_PER));
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(grade_config->jianren * (total_add_per * ROLE_ATTR_PER));
	}
}

int HaloManager::GetMedalSkillLevel()
{
	short medal_skill_index = LOGIC_CONFIG->GetHaloConfig().GetMedalSkillIndex();
	if (medal_skill_index < 0 || medal_skill_index >= (short)static_array_size(m_halo_param.skill_level_list))
	{
		return 0;
	}
	return m_halo_param.skill_level_list[medal_skill_index];
}

int HaloManager::GetGrade()
{
	if (!m_halo_param.IsHaloActived())
	{
		return 0;
	}
	return m_halo_param.grade;
}
