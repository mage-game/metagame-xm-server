#include "wingmanager.hpp"

#include "gamelog.h"
#include "engineadapter.h"

#include "obj/character/role.h"

#include "servercommon/errornum.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "protocal/msgwing.hpp"

#include "scene/scene.h"
#include "wingconfig.hpp"
#include "gamecommon.h"
#include "gstr.h"



#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/shuxingdanconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "servercommon/vipdef.hpp"
#include "other/wing/wingfunction.hpp"
#include "globalconfig/globalconfig.h"
#include "other/camp/campconfig.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "serverlogic.h"

#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "task/taskrecorder.h"
#include "other/capability/capability.hpp"
#include "other/xunzhang/xunzhangmanager.hpp"
#include "other/zhibao/zhibaomanager.hpp"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include "other/imgfuling/imgfuling.hpp"
#include "servercommon/imgfulingdef.hpp"
#include "global/challengefield/challengefieldconfig.hpp"
#include "global/challengefield/challengefield.hpp"
#include "other/talent/talent.hpp"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "other/jinjiesysreward/jinjiesysrewardmgr.hpp"
#include "other/dressingroom/dressingroomconfig.hpp"
#include "other/dressingroom/dressingroommgr.hpp"
#include "servercommon/serverconfig/crossrankrewardconfig.hpp"
#include "global/worldstatus3/worldstatus3.hpp"
#include "other/xianzunka/xianzunka.hpp"
#include "common/bintohex.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/serverconfig/localconfig.hpp"

WingManager::WingManager()
: m_role(NULL), m_other_capability(0), m_skill_shouhu_loss_effect_time(0), m_skill_shouhu_count(0)
{

}

WingManager::~WingManager()
{

}

void WingManager::Init(Role *role, const WingParam &wing_param)
{
	m_role = role;
	m_wing_param = wing_param;


	if (wing_param.old_grade_bless_val != 0)
	{
		m_wing_param.grade_bless_val = wing_param.old_grade_bless_val;
		m_wing_param.old_grade_bless_val = 0;
	}
}
void WingManager::OnLogin()
{
	this->CheckActive();
	if (CrossConfig::Instance().IsHiddenServer())
	{//跨服中不根据排行榜删除形象
		return;
	}
	// 改为结算时通过邮件发放 modify 2019-03-10
	// this->OnChallengeFieldRankInfo(ChallengeField::Instance().GetRoleRewardRankPos(m_role->GetUID()));

	this->CheckShowChallengeFieldRank();
	CheckActiveSpecialImg();
}

void WingManager::GetOtherInitParam(WingParam *param)
{
	*param = m_wing_param;
}

void WingManager::GetRoleAppearance(RoleAppearance *appearance)
{
	if (NULL != appearance)
	{
		appearance->wing_used_imageid = m_wing_param.temporary_imageid > 0 ? m_wing_param.temporary_imageid : m_wing_param.used_imageid;
	}
}

void WingManager::OnRoleLevelChange(int level)
{
	m_wing_param.level = level;

	if (m_wing_param.IsWingActived())
	{
		this->SendWingInfo();
	}
}

void WingManager::Update(time_t now_second)
{
	const WingGradeConfig *grade_cfg = LOGIC_CONFIG->GetWingConfig().GetGradeConfig(m_wing_param.grade);
 	if (nullptr != grade_cfg && 1 == grade_cfg->is_clear_bless &&  now_second > m_wing_param.clear_upgrade_time)
 	{
 		m_wing_param.grade_bless_val = 0;
 		m_wing_param.clear_upgrade_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + EngineAdapter::Instance().NextDayInterval(5, 0, 0);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);   //清除临时属性
 		this->SendWingInfo();
 	}

	if (0 != m_wing_param.temporary_imageid_invalid_time && static_cast<unsigned int>(now_second) > m_wing_param.temporary_imageid_invalid_time)
	{
		if (0 != m_wing_param.temporary_imageid_has_select)
		{
			m_wing_param.temporary_imageid = 0;
			m_wing_param.temporary_imageid_has_select = 0;
			m_role->RefreshApperance(true);
			this->SendWingInfo();
		}
	}
}

// 受击技能特效
void WingManager::OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{
	if (!m_role->IsAlive())
	{
		return;
	}

	unsigned long long now_time_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();

	// 守护
// 	static const int SKILL_ID0 = 0;
// 	UNSTD_STATIC_CHECK(WingParam::SKILL_COUNT > SKILL_ID0);
// 	if (m_wing_param.skill_level_list[SKILL_ID0] > 0)
// 	{
// 		const WingSkillConifg *skill_cfg = LOGIC_CONFIG->GetWingConfig().GetSkillCfg(SKILL_ID0, m_wing_param.skill_level_list[SKILL_ID0]);
// 		if (NULL != skill_cfg)
// 		{
// 			if (m_skill_shouhu_count > 0 && now_time_ms < m_skill_shouhu_loss_effect_time)
// 			{
// 				delta_hp -= (Attribute)(delta_hp * (skill_cfg->param_c * ROLE_ATTR_PER));
// 				--m_skill_shouhu_count;
// 			}
// 			else if (now_time_ms > m_wing_param.skill_last_colddown_time[SKILL_ID0])
// 			{
// 				m_skill_shouhu_count = 0;
// 				m_skill_shouhu_loss_effect_time = 0;
// 
// 				Attribute current_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
// 				Attribute max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
// 
// 				if(current_hp < max_hp * (skill_cfg->param_a * ROLE_ATTR_PER))
// 				{
// 					m_wing_param.skill_last_colddown_time[SKILL_ID0] = now_time_ms + skill_cfg->param_d;
// 					m_skill_shouhu_loss_effect_time = now_time_ms + 30000;
// 					m_skill_shouhu_count = skill_cfg->param_b;
// 
// 					delta_hp -= (Attribute)(delta_hp * (skill_cfg->param_c * ROLE_ATTR_PER));
// 
// 					EffectSpecialState *effect_state = new EffectSpecialState(m_role->GetId(), 0, 2000, EBT_WING_SHOUHU, 1);
// 					effect_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
// 					m_role->AddEffect(effect_state);
// 
// 					--m_skill_shouhu_count;
// 				}
// 			}
// 		}
// 	}
	
	// 风盾
// 	static const int SKILL_ID2 = 2;
// 	UNSTD_STATIC_CHECK(WingParam::SKILL_COUNT > SKILL_ID2);
// 	if (m_wing_param.skill_level_list[SKILL_ID2] > 0 && now_time_ms > m_wing_param.skill_last_colddown_time[SKILL_ID2])
// 	{
// 		const WingSkillConifg *skill_cfg = LOGIC_CONFIG->GetWingConfig().GetSkillCfg(SKILL_ID2, m_wing_param.skill_level_list[SKILL_ID2]);
// 		if (NULL != skill_cfg)
// 		{
// 			if (-delta_hp > m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (skill_cfg->param_a * ROLE_ATTR_PER))
// 			{
// 				m_wing_param.skill_last_colddown_time[SKILL_ID2] = now_time_ms + skill_cfg->param_d;
// 				// 自身加防
// 				EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), SKILL_ID2, skill_cfg->param_c, 1);
// 				effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
// 				effect_buff->AddFangYuEffect(skill_cfg->param_b);
// 				effect_buff->SetClientEffectType(EffectBase::ECT_UPGRADE_WING_FENGDUN);
// 				m_role->AddEffect(effect_buff);
// 
// 				EffectSpecialState *effect_state = new EffectSpecialState(m_role->GetId(), SKILL_ID2 , skill_cfg->param_c, EBT_WING_FENGDUN, 1);
// 				effect_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
// 				m_role->AddEffect(effect_state);
// 			}
// 		}
// 	}

	// 结界
	static const int SKILL_ID3 = 3;
	UNSTD_STATIC_CHECK(WingParam::SKILL_COUNT > SKILL_ID3);
	if (m_wing_param.skill_level_list[SKILL_ID3] > 0 && now_time_ms > m_wing_param.skill_last_colddown_time[SKILL_ID3])
	{
		const WingSkillConifg *skill_cfg = LOGIC_CONFIG->GetWingConfig().GetSkillCfg(SKILL_ID3,m_wing_param.skill_level_list[SKILL_ID3]);
		if (NULL != skill_cfg)
		{
			if (RandomNum(10000) < skill_cfg->param_a)
			{
				m_wing_param.skill_last_colddown_time[SKILL_ID3] = now_time_ms + skill_cfg->param_d;

				// 自身伤害减免
				EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), SKILL_ID3, skill_cfg->param_c, 1);
				effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
				effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG, skill_cfg->param_b , skill_cfg->param_b);
				effect_buff->SetClientEffectType(EffectBase::ECT_UPGRADE_WING_JIEJIE);
				m_role->AddEffect(effect_buff);

				EffectSpecialState *effect_state = new EffectSpecialState(m_role->GetId(), SKILL_ID3 , skill_cfg->param_c, EBT_WING_JIEJIE, 1);
				effect_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
				m_role->AddEffect(effect_state);
			}
		}
	}
}

// 攻击时有param_b概率造成param_c%的额外伤害。 冷却param_a毫秒
void WingManager::OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure)
{
	static const int SKILL_ID0 = 0;
	UNSTD_STATIC_CHECK(WingParam::SKILL_COUNT > SKILL_ID0);
	
	if (m_wing_param.skill_level_list[SKILL_ID0] <= 0) return;
	
	const WingSkillConifg *skill_cfg = LOGIC_CONFIG->GetWingConfig().GetSkillCfg(SKILL_ID0, m_wing_param.skill_level_list[SKILL_ID0]);
	if (NULL == skill_cfg) return;

	unsigned long long now_time_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();
	if (now_time_ms < m_wing_param.skill_last_colddown_time[SKILL_ID0])
	{
		return;
	}

	int rate = RandomNum(10000);
	if (rate >= skill_cfg->param_b)
	{
		return;
	}

	base_injure = static_cast<Attribute>(base_injure * (skill_cfg->param_c * ROLE_ATTR_PER));
	injure += base_injure;

	m_wing_param.skill_last_colddown_time[SKILL_ID0] = now_time_ms + skill_cfg->param_a;
			
}

void WingManager::SendWingInfo()
{
	Protocol::SCWingInfo mi;
	mi.wing_level = m_wing_param.level;
	mi.grade = m_wing_param.grade;
	mi.grade_bless_val = m_wing_param.grade_bless_val;
	mi.clear_upgrade_time = m_wing_param.clear_upgrade_time;
	mi.used_imageid = m_wing_param.used_imageid;
	ARRAYCPY(mi.active_image_flag, m_wing_param.active_image_flag);
	ARRAYCPY(mi.active_special_image_flag, m_wing_param.active_special_image_flag);
	mi.temporary_imageid = m_wing_param.temporary_imageid;
	mi.temporary_imageid_has_select = m_wing_param.temporary_imageid_has_select;
	mi.temporary_imageid_invalid_time = m_wing_param.temporary_imageid_invalid_time;
	mi.star_level = m_wing_param.star_level;
	mi.equip_skill_level = m_wing_param.equip_skill_level;

	ARRAYCPY(mi.shuxingdan_list, m_wing_param.shuxingdan_list);
	memcpy(mi.equip_level_list, m_wing_param.equip_level_list, sizeof(mi.equip_level_list));
	memcpy(mi.skill_level_list, m_wing_param.skill_level_list, sizeof(mi.skill_level_list));
	memcpy(mi.special_img_grade_list, m_wing_param.special_img_grade_list, sizeof(mi.special_img_grade_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&mi, sizeof(mi));
}

int WingManager::GetWingAppeID()
{
	return m_wing_param.temporary_imageid > 0 ? m_wing_param.temporary_imageid : m_wing_param.used_imageid;
}

bool WingManager::ActiveImage(int imageid, bool recalc)
{
	if (imageid <= 0 || imageid >= WingParam::MAX_IMAGE_COUNT) return false;

	m_wing_param.ActiveImage(imageid);

	if (recalc)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);
	}

	return true;
}

bool WingManager::UseImage(int imageid)
{
	if (imageid < SPECIAL_IMG_START_ID)
	{
		if (!m_wing_param.IsImageActive(imageid))
		{
			m_role->NoticeNum(errornum::EN_WING_IMG_NOT_ACTIVE);
			return false;
		}
	}
	else
	{
		if (!m_wing_param.IsSpecialImageActive(imageid - SPECIAL_IMG_START_ID))
		{
			m_role->NoticeNum(errornum::EN_WING_IMG_NOT_ACTIVE);
			return false;
		}
	}

	m_wing_param.used_imageid = imageid;

	if (0 != m_wing_param.temporary_imageid)
	{
		m_wing_param.temporary_imageid = 0;
	}

	m_role->RefreshApperance(true);

	this->SendWingInfo();

	return true;
}

bool WingManager::UnUseImage(int imageid)
{
	if (imageid != m_wing_param.used_imageid)
	{
		return false;
	}

	m_wing_param.used_imageid = 0;

	m_role->RefreshApperance(true);

	this->SendWingInfo();

	return true;
}

bool WingManager::ActiveSpecialImage(int imageid, bool is_notify)
{
	if (m_wing_param.IsSpecialImageActive(imageid))
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_SPECIAL_IMG_ALREADY_ACTIVE);
		}

		return false;
	}

	const WingSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetWingConfig().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}
	if (m_role->GetProfLevel() < cfg_item->zhuanzhi_prof)
	{
		m_role->NoticeNum(errornum::EN_WING_PROF_NOT_ENOUGH);
		return false;
	}
	m_wing_param.ActiveSpecialImage(imageid);
	m_role->GetImgFuling()->OnPutOnImg(JINGJIE_TYPE_WING, imageid);
	if (0 == m_wing_param.special_img_grade_list[imageid])
	{
		m_wing_param.special_img_grade_list[imageid] = 1;
	}
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);
	JinjiesysFuncHelper::Instance().OnActiveSpecialImg(m_role, JINJIE_TYPE_WING, imageid);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_WING,imageid,true);
	this->UseImage(SPECIAL_IMG_START_ID + imageid);
	return true;
}

bool WingManager::UnActiveSpecialImage(int imageid, bool is_activity_img)
{
	if (!m_wing_param.IsSpecialImageActive(imageid))
	{
		return false;
	}

	const WingSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetWingConfig().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	BitmapFunction::ClearBit(m_wing_param.active_special_image_flag, sizeof(m_wing_param.active_special_image_flag), imageid);
	if (1 == m_wing_param.special_img_grade_list[imageid])
	{
		m_wing_param.special_img_grade_list[imageid] = 0;
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_WING, imageid, false);
	if (this->UnUseImage(SPECIAL_IMG_START_ID + imageid))
	{
		this->UseImage(1); //默认再给一个
	}

	if (is_activity_img)
	{
		this->SendUnActiveSpecialImgMail(imageid);
	}

	return true;
}

void WingManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
//	if (!m_wing_param.IsWingActived())
//	{
//		return;
//	}

	if (is_recalc)
	{
		m_attrs_add.Reset();
		m_other_capability = 0;

		double rate = 0.0;						// 属性加成万分比

		CharIntAttrs base_attrs;
		base_attrs.Reset();
		CharIntAttrs other_per_add_attr;
		{
			// 等级加成
			const WingLevelConfig *level_cfg = LOGIC_CONFIG->GetWingConfig().GetLevelConfig(m_wing_param.level);
			if (NULL != level_cfg)
			{
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg->maxhp;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg->gongji;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg->fangyu;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += level_cfg->mingzhong;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += level_cfg->shanbi;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += level_cfg->baoji;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += level_cfg->jianren;
			}
		}

		{
			CharIntAttrs grade_attrs;
			// 阶数加成  
			const WingGradeConfig * grade_config = LOGIC_CONFIG->GetWingConfig().GetGradeConfig(m_wing_param.grade);
			if (NULL != grade_config) 
			{
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += grade_config->maxhp;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += grade_config->gongji;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += grade_config->fangyu;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += grade_config->mingzhong;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += grade_config->shanbi;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += grade_config->baoji;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += grade_config->jianren;

				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += grade_config->extra_zengshang;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += grade_config->extra_mianshang;
				// 特殊属性
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += grade_config->per_jingzhun;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += grade_config->per_baoji;
// 				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += grade_config->per_mianshang;
// 				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += grade_config->per_pofang;
// 				m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += grade_config->pvp_zengshang_per;
// 				m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += grade_config->pve_zengshang_per;
// 				m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += grade_config->pvp_jianshang_per;
// 				m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += grade_config->pve_jianshang_per;

				// 下面特殊属性用乘法
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, grade_config->per_mianshang);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, grade_config->per_pofang);
				m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, grade_config->pvp_zengshang_per);
				m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, grade_config->pve_zengshang_per);
				m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, grade_config->pvp_jianshang_per);
				m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, grade_config->pve_jianshang_per);

				// 大目标特殊形象万分比
				int special_img_per_add = JinjieSysRewardMgr::Instance().GetJinjieSpecialImgExtraAddPer(m_role, JINJIE_TYPE_WING);
				if (special_img_per_add > 0)
				{
					double d_special_img_per_add = special_img_per_add * ROLE_ATTR_PER;
					other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * d_special_img_per_add);
					other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * d_special_img_per_add);
					other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * d_special_img_per_add);
					other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * d_special_img_per_add);
					other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * d_special_img_per_add);
					other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * d_special_img_per_add);
					other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * d_special_img_per_add);
				}
				
				// 仙尊卡加成
				{
					const int add_grade_attrs_per = m_role->GetXianZunKa()->GetAddWingGradeAttrsPer();
					for (int i = CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX; ++i)
					{
						if (CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED == i) continue;

						//grade_attrs.m_attrs[i] += static_cast<Attribute>(grade_attrs.m_attrs[i] * (ROLE_ATTR_PER * add_grade_attrs_per));
						grade_attrs.AddValue(i, static_cast<Attribute>(grade_attrs.m_attrs[i] * (ROLE_ATTR_PER * add_grade_attrs_per)));
					}
				}

				base_attrs.Add(grade_attrs.m_attrs);

				// 天赋加成
				m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_WING_BASE, grade_attrs, m_attrs_add);

				this->CalcBaseAttrAddFromSkill(base_attrs);
			}
		}

		m_attrs_add.Add(base_attrs.m_attrs);

		{
			// 天赋加成
			m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_WING, base_attrs, m_attrs_add);
		}

		{
			// 星级加成  
			const WingUpStarExpCfg * star_level_config = LOGIC_CONFIG->GetWingConfig().GetUpStarExpCfg(m_wing_param.star_level);
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
			// 羽翼装备
			int active_equip_grade = LOGIC_CONFIG->GetWingConfig().GetActiveEquipGrade();
			if (active_equip_grade <= m_wing_param.grade)
			{
				for (int i = 0; i < WingParam::EQUIP_COUNT; ++i)
				{
					const WingEquipConifg *equip_cfg = LOGIC_CONFIG->GetWingConfig().GetEquipCfg(i);
					if (NULL != equip_cfg)
					{
						int level = m_wing_param.equip_level_list[i];
						if (level > 0 && level < equip_cfg->max_level)
						{
							rate += equip_cfg->level_cfg[level].add_percent;
						}
					}
				}
			}

			{
				// 至宝加成
				rate += m_role->GetZhiBaoManager()->GetWingAddPer();

				// 勋章加成
				rate += m_role->GetXunZhangManager()->GetWingAddPer();

				// 星座星魂加成
				int xinhun_add_per = 0;
				m_role->GetChineseZodiac()->GetXingHunTotalAddPer(NULL, &xinhun_add_per, NULL, NULL, NULL, NULL, NULL);
				rate += xinhun_add_per;

				// 成长丹加成，只对阶数和等级属性有效
				// 			if (m_wing_param.chengzhangdan_count > 0)
				// 			{
				// 				int percent = LOGIC_CONFIG->GetWingConfig().GetChengZhangDanPercent() * m_wing_param.chengzhangdan_count;
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

			if (active_equip_grade <= m_wing_param.grade)
			{
				for (int i = 0; i < WingParam::EQUIP_COUNT; ++i)
				{
					const WingEquipConifg *equip_cfg = LOGIC_CONFIG->GetWingConfig().GetEquipCfg(i);
					if (NULL != equip_cfg)
					{
						int level = m_wing_param.equip_level_list[i];
						if (level > 0 && level < equip_cfg->max_level)
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
			for (int i = 0; i < WingParam::MAX_SPECIAL_IMAGE_COUNT; ++ i)
			{
				if (m_wing_param.IsSpecialImageActive(i))
				{
					// 特殊形象进阶
					const int grade = m_wing_param.special_img_grade_list[i];

					const WingSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetWingConfig().GetSpecialImgUpgradeCfg(i, grade);
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
						m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<float>(upgrade_cfg->per_maxhp * ROLE_ATTR_PER);

					}

					this->HuanHuaSpecialCapAdd(i, grade, m_attrs_add);
				}
			}
		}

		// 属性丹
		{
			int shuxingdan_attr_per = 0;
			for (int slot_index = 0; slot_index < SHUXINGDAN_MAX_TYPE; slot_index++)
			{
				const int num = m_wing_param.shuxingdan_list[slot_index];
				if (num > 0)
				{
					const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_WING, slot_index);
					if (nullptr != cfg_item)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp * num;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji * num;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu * num;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg_item->mingzhong * num;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg_item->shanbi * num;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg_item->baoji * num;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg_item->jianren * num;
						shuxingdan_attr_per += cfg_item->attr_per * num;
					}
				}
			}

			for (int i = CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX; ++i)
			{
				//m_attrs_add.m_attrs[i] += static_cast<Attribute>(base_attrs.m_attrs[i] * (ROLE_ATTR_PER * shuxingdan_attr_per));
				m_attrs_add.AddValue(i, static_cast<Attribute>(base_attrs.m_attrs[i] * (ROLE_ATTR_PER * shuxingdan_attr_per)));
			}
		}

		{
			// 技能属性
			for (int i = 0; i < WingParam::SKILL_COUNT; i++)
			{
				if (m_wing_param.skill_level_list[i] > 0)
				{
					const WingSkillConifg *skill_cfg = LOGIC_CONFIG->GetWingConfig().GetSkillCfg(i, m_wing_param.skill_level_list[i]);
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

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_WING, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_WING, m_other_capability);

	m_wing_param.capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_WING);

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

WingParam * WingManager::GetWingParam()
{
	return &m_wing_param;
}

void WingManager::PrintWing(const char *reason, int param1, int param2, int param3, int param4, int param5)
{
	UNSTD_STATIC_CHECK(4 == WingParam::EQUIP_COUNT);
	UNSTD_STATIC_CHECK(4 == WingParam::SKILL_COUNT);
	const int image_hex_len = WingParam::MAX_ACTIVE_IMAGE_BYTE * 2 + 1;
	const int special_image_hex_len = WingParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE * 2 + 1;
	char active_image_hex[image_hex_len] = { 0 };
	char active_special_image_hex[special_image_hex_len] = { 0 };
	BinToHex((const char*)m_wing_param.active_image_flag, sizeof(m_wing_param.active_image_flag), active_image_hex);
	BinToHex((const char*)m_wing_param.active_special_image_flag, sizeof(m_wing_param.active_special_image_flag), active_special_image_hex);
	gamelog::g_log_wing.printf(LL_INFO, "Wing role(%d,%s) %s(%d,%d,%d,%d,%d) "
		"level:%d, grade:%d,%d, used_imageid:%d zizhidan_count:%d chengzhangdan_count:%d image_flag:%s, special_image_flag:%s"
		"equip_level_list:%d, %d, %d, %d"
		"skill_level_list:%d, %d, %d, %d",
		m_role->GetUID(), m_role->GetName(),
		reason, param1, param2, param3, param4, param5,
		(int)m_wing_param.level,
		(int)m_wing_param.grade, (int)m_wing_param.grade_bless_val,
		(int)m_wing_param.used_imageid,
		(int)m_wing_param.shuxingdan_list[0], (int)m_wing_param.shuxingdan_list[1],
		active_image_hex, active_special_image_hex, 
		(int)m_wing_param.equip_level_list[0], (int)m_wing_param.equip_level_list[1], (int)m_wing_param.equip_level_list[2], (int)m_wing_param.equip_level_list[3],
		(int)m_wing_param.skill_level_list[0], (int)m_wing_param.skill_level_list[1], (int)m_wing_param.skill_level_list[2], (int)m_wing_param.skill_level_list[3]);
}

void WingManager::OnCompleteTask(TaskID task_id)
{
//	if (LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenTaskID(FUN_OPEN_TYPE_WING) == task_id)
//	{
//		WingFunction::ActiveWing(m_role);
//	}
	this->CheckActive();
}

void WingManager::OnRoleUpLevel(int level)
{
//	if (m_wing_param.IsWingActived()) return;

//	if (level >= LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenRoleLevel(FUN_OPEN_TYPE_WING))
//	{
//		WingFunction::ActiveWing(m_role);
//	}
	this->CheckActive();
}

void WingManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->CheckActive();
	}
}

bool WingManager::OnUseShuxingdan(int slot_idx, short &use_count)
{
	if (slot_idx < 0 || slot_idx >= SHUXINGDAN_MAX_TYPE || use_count <= 0)
	{
		return false;
	}

	const WingGradeConfig *grade_cfg = LOGIC_CONFIG->GetWingConfig().GetGradeConfig(m_wing_param.grade);
	if (NULL == grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_WING_SHUXINGDAN_USE_LIMIT_NUM);
		return false;
	}

	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_WING, slot_idx);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_wing_param.grade < cfg_item->order_limit)
	{
		gstr::SendError(m_role, "shuxingdan_use_order_limit_1", SHUXINGDAN_TYPE_WING, cfg_item->order_limit - 1, slot_idx); // 客户端阶数-1
		return false;
	}

	if (m_wing_param.shuxingdan_list[slot_idx] + use_count > cfg_item->use_limit_num)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	int shuxingdan_limit = (slot_idx == SHUXINGDAN_SLOT_TYPE_ZIZHI) ? grade_cfg->shuxingdan_limit : grade_cfg->chengzhangdan_limit;
	int special_img_add_count = 0;
	for (int i = 0;i < WingParam::MAX_SPECIAL_IMAGE_COUNT; i++)
	{
		if (!m_wing_param.IsSpecialImageActive(i))
			continue;

		const WingSpecialImgUpgradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetWingConfig().GetSpecialImgUpgradeCfg(i,m_wing_param.special_img_grade_list[i]);
		if (NULL != special_img_grade_cfg)
		{
			int shuxingdan_add_count = (slot_idx == SHUXINGDAN_SLOT_TYPE_ZIZHI) ? special_img_grade_cfg->shuxingdan_count : special_img_grade_cfg->chengzhangdan_count;
			special_img_add_count += shuxingdan_add_count;
		}
	}

	// 策划需求，修正真正使用数量
	int can_use_count = shuxingdan_limit + special_img_add_count - m_wing_param.shuxingdan_list[slot_idx];
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

	m_wing_param.shuxingdan_list[slot_idx] += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);
	this->SendWingInfo();

	return true;
}

bool WingManager::UseTemporaryImage(int imageid)
{
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (0 != m_wing_param.temporary_imageid_invalid_time && now_sec > m_wing_param.temporary_imageid_invalid_time)
	{
		m_role->NoticeNum(errornum::EN_WING_TEMPORARY_IMAGE_TIME_LIMIT);
		return false;
	}

	if(0 != imageid && 0 != m_wing_param.temporary_imageid_has_select && imageid != m_wing_param.temporary_imageid_has_select)
	{
		return false;
	}

	m_wing_param.temporary_imageid = imageid;

	if (0 != imageid && 0 == m_wing_param.temporary_imageid_has_select)
	{
		m_wing_param.temporary_imageid_has_select = imageid;
	}

	if (0 != imageid && 0 == m_wing_param.temporary_imageid_invalid_time)
	{
		m_wing_param.temporary_imageid_invalid_time = now_sec + LOGIC_CONFIG->GetWingConfig().GetTemporaryImageValidTime();
	}

	m_role->RefreshApperance(true);

	this->SendWingInfo();

	return true;
}

int WingManager::RefreshWingMinEquipLevel()
{
	int min = m_wing_param.equip_level_list[0];

	for (int i = 1; i < WingParam::EQUIP_COUNT; ++i)
	{
		min = m_wing_param.equip_level_list[i] < min ? m_wing_param.equip_level_list[i] : min;
	}

	return min;
}

void WingManager::OnChallengeFieldRankInfo(int rank_pos)
{
	return; // 改为结算时通过邮件发放 modify 2019-03-10

	int image_id = LOGIC_CONFIG->GetChallengeFieldCfg().GetRankTopRewardWingID();

	if (image_id < 0)
	{
		return;
	}

	//第一名，没激活的才激活，非第一名则去掉
	if (0 == rank_pos && !m_wing_param.IsSpecialImageActive(image_id))
	{
		this->ActiveSpecialImage(image_id);
	}
	else if (0 != rank_pos)
	{
		this->UnActiveSpecialImage(image_id);
	}
}


void WingManager::CheckShowChallengeFieldRank()
{
	int rank = ChallengeField::Instance().GetChallengeUserRankPos(m_role);
	bool is_open = ChallengeField::Instance().IsChallengeOpen();
	if (rank > 0 && !is_open)
	{
		int reward_opengame_day = ChallengeField::Instance().GetEndRankRewardOpenday();
		const ChallengeFieldRankRewardConfig::ConfigItem *reward_cfg = LOGIC_CONFIG->GetChallengeFieldCfg().GetRankRewardCfgByRank(reward_opengame_day, rank - 1);
		if (NULL != reward_cfg && reward_cfg->wing_id > 0)
		{
			bool is_active = m_wing_param.IsSpecialImageActive(reward_cfg->wing_id);
			if (!is_active)
			{
				EventHandler::Instance().OnGetSceneActivityReward(m_role, Protocol::SCSceneActivityRewardInfo::REWARD_SOURCE_ID_CHALLENGEFIELD, reward_cfg->wing_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_WING, rank);
			}
		}
	}
}

void WingManager::CheckActive()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (!m_wing_param.IsWingActived())
	{
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_WING);
		if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day)
		{
			if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
			{
				WingFunction::ActiveWing(m_role);
			}
		}
	}
}

// 幻化特殊战力加成
void WingManager::HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add)
{
	if (!LOGIC_CONFIG->GetWingConfig().IsAddSpecialCap())
	{
		return;
	}

	auto cfg_item = LOGIC_CONFIG->GetWingConfig().GetHuanHuaSpecialCapAddCfg(huanhua_id);
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

void WingManager::SendUnActiveSpecialImgMail(int img_id)
{
	const auto pCfg = LOGIC_CONFIG->GetWingConfig().GetSpecialImgCfg(img_id);
	if (nullptr != pCfg)
	{
		static MailContentParam mail_param; mail_param.Reset();
		int length = SNPRINTF(mail_param.contenttxt, sizeof(mail_param.contenttxt), gamestring::g_time_limit_cap_img_mail_context,
			pCfg->active_item_id);

		if (length > 0)
		{
			MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, mail_param);
		}
	}
}

void WingManager::CheckActiveSpecialImg(bool is_mail)
{
	if (m_role->IsInCross())
	{
		return;
	}

	std::set<ItemID> active_item_set;
	std::set<ItemID> unactive_item_set;

	int plat_type = LocalConfig::Instance().GetPlatType();
	UserID role_id = m_role->GetUserId();
	UniqueUserID unique_userid(plat_type, role_id);
	long long uuid = UniqueUserIDToLongLong(unique_userid);

	int rank_type = 0;

	// 跨服情缘榜
	{
		long long male_uuid_list[CROSS_RANK_REWARD_COUNT] = { 0 };
		WorldStatus3::Instance().GetCrossQingyuanCapRankFirstMale(male_uuid_list);
		long long female_uuid_list[CROSS_RANK_REWARD_COUNT] = { 0 };
		WorldStatus3::Instance().GetCrossQingyuanCapRankFirstFemale(female_uuid_list);

		for (int i = 0; i < CROSS_RANK_REWARD_COUNT; ++i)
		{
			const CrossRankReward *reward_cfg = CrossRankRewardConfig::Instance().GetReward(CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP + CROSS_COUPLE_RANK_TYPE_BEGIN, i + 1);
			if (nullptr != reward_cfg)
			{
				if (uuid == male_uuid_list[i] || uuid == female_uuid_list[i])
				{
					active_item_set.insert(reward_cfg->img_item_id);

					rank_type = CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP + CROSS_COUPLE_RANK_TYPE_BEGIN;
				}
				else
				{
					unactive_item_set.insert(reward_cfg->img_item_id);
				}
			}
		}
	}

	// 结算激活与不激活
	{
		// 激活
		std::set<ItemID>::iterator iter = active_item_set.begin();
		for (; iter != active_item_set.end(); ++iter)
		{
			auto pCfg = LOGIC_CONFIG->GetWingConfig().GetSpecialImgCfgByItemID(*iter);
			if (nullptr != pCfg)
			{
				if (this->ActiveSpecialImage(pCfg->image_id))
				{
					if (rank_type != 0 && rank_type == CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP + CROSS_COUPLE_RANK_TYPE_BEGIN)
					{
						EventHandler::Instance().OnGetSceneActivityReward(m_role, Protocol::SCSceneActivityRewardInfo::REWARD_SOURCE_ID_CROSS_QINGYUAN_CAP, pCfg->image_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_WING);
					}
				}
			}
		}

		// 取消激活
		for (iter = unactive_item_set.begin(); iter != unactive_item_set.end(); ++iter)
		{
			// 如果本次已经激活了，不应该取消掉
			if (active_item_set.find(*iter) == active_item_set.end())
			{
				auto pCfg = LOGIC_CONFIG->GetWingConfig().GetSpecialImgCfgByItemID(*iter);
				if (nullptr != pCfg)
				{
					this->UnActiveSpecialImage(pCfg->image_id, is_mail);
				}
			}
		}
	}
}

int WingManager::GetSpecialImageLevelById(int imageid)
{
	if (imageid < 0 || imageid >= WingParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return 0;
	}
	if (!m_wing_param.IsSpecialImageActive(imageid))
	{
		return 0;
	}

	return m_wing_param.special_img_grade_list[imageid];
}

void WingManager::CalcBaseAttrAddFromSkill(CharIntAttrs & base_add)
{
	// 阶数加成
	const WingGradeConfig * grade_config = LOGIC_CONFIG->GetWingConfig().GetGradeConfig(m_wing_param.grade);
	if (NULL == grade_config)
	{
		return;
	}

	int total_add_per = 0;
	for (int i = 0; i < WingParam::SKILL_COUNT; i++)
	{
		if (m_wing_param.skill_level_list[i] <= 0)
		{
			continue;
		}
		const WingSkillConifg *skill_cfg = LOGIC_CONFIG->GetWingConfig().GetSkillCfg(i, m_wing_param.skill_level_list[i]);
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

int WingManager::GetMedalSkillLevel()
{
	short medal_skill_index = LOGIC_CONFIG->GetWingConfig().GetMedalSkillIndex();
	if (medal_skill_index < 0 || medal_skill_index >= (short)static_array_size(m_wing_param.skill_level_list))
	{
		return 0;
	}
	return m_wing_param.skill_level_list[medal_skill_index];
}

int WingManager::GetGrade()
{
	if (!m_wing_param.IsWingActived())
	{
		return 0;
	}
	return m_wing_param.grade;
}
