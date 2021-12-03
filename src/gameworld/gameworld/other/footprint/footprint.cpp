#include "footprint.hpp"
#include "footprintconfig.hpp"

#include "gamelog.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/shuxingdanconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "other/capability/capability.hpp"
#include "serverlogic.h"
#include "task/taskrecorder.h"
#include "other/xunzhang/xunzhangmanager.hpp"
#include "protocal/msgrole.h"
#include "item/knapsack.h"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "gstr.h"
#include "other/sublock/sublock.hpp"
#include "item/itempool.h"
#include "other/rolegoal/rolegoal.hpp"
#include "other/rolegoal/rolegoalconfig.hpp"
#include "servercommon/commondata.hpp"
#include "other/upgradeskill/upgradeskillconfig.hpp"

#include "effect/skilleffect/effectbuff.hpp"
#include "other/chinesezodiac/chinesezodiacconfig.hpp"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include "other/imgfuling/imgfuling.hpp"
#include "servercommon/imgfulingdef.hpp"
#include "other/talent/talent.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "other/jinjiesysreward/jinjiesysrewardmgr.hpp"
#include "other/dressingroom/dressingroomconfig.hpp"
#include "other/dressingroom/dressingroommgr.hpp"
#include "servercommon/activitydef.hpp"
#include "other/upgradesyscrit/upgradesyscrit.hpp"
#include "common/bintohex.h"
#include "servercommon/serverconfig/crossconfig.hpp"

FootPrint::FootPrint()
	: m_role(NULL), m_other_capability(0), m_last_add_fangyu(0)
{

}

FootPrint::~FootPrint()
{

}

void FootPrint::Init(Role *role, const FootprintParam &halo_param)
{
	m_role = role;
	m_footprint_param = halo_param;
}

void FootPrint::GetOtherInitParam(FootprintParam *param)
{
	*param = m_footprint_param;
}

void FootPrint::GetRoleAppearance(RoleAppearance *appearance)
{
	if (NULL != appearance)
	{
		appearance->footprint_used_imageid = m_footprint_param.used_imageid;
	}
}

void FootPrint::OnLogin()
{
	this->CheckActive();
}

void FootPrint::OnRoleLevelChange(int level)
{
	m_footprint_param.level = level;

	if (m_footprint_param.IsFootprintActived())
	{
		this->SendFootprintInfo();
	}
}

void FootPrint::Update(time_t now_second)
{
	if (!m_footprint_param.IsFootprintActived() || 0 == m_footprint_param.clear_upgrade_time) {
		return;
	}

	const FootprintGradeConfig *grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetGradeConfig(m_footprint_param.grade);
 	if (NULL != grade_cfg && 1 == grade_cfg->is_clear_bless && now_second > m_footprint_param.clear_upgrade_time)
 	{
 		m_footprint_param.grade_bless_val = 0;
		//m_footprint_param.clear_upgrade_time = 0;
 		m_footprint_param.clear_upgrade_time = static_cast<unsigned int>(now_second) + static_cast<unsigned int>(EngineAdapter::Instance().NextDayInterval(5, 0, 0));
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT); //清除临时属性
 		this->SendFootprintInfo();
 	}
}

void FootPrint::SendFootprintInfo()
{
	Protocol::SCFootprintInfo mi;
	mi.footprint_level = m_footprint_param.level;
	mi.grade = m_footprint_param.grade;
	mi.star_level = m_footprint_param.star_level;
	mi.clear_upgrade_time = m_footprint_param.clear_upgrade_time;
	mi.used_imageid = m_footprint_param.used_imageid;

	UNSTD_STATIC_CHECK(sizeof(mi.active_image_flag) == sizeof(m_footprint_param.active_image_flag));
	memcpy(mi.active_image_flag, m_footprint_param.active_image_flag, sizeof(mi.active_image_flag));

	mi.shuxingdan_count = m_footprint_param.shuxingdan_count;
	mi.chengzhangdan_count = m_footprint_param.chengzhangdan_count;
	mi.grade_bless_val = m_footprint_param.grade_bless_val;

	UNSTD_STATIC_CHECK(sizeof(mi.active_special_image_flag) == sizeof(m_footprint_param.active_special_image_flag));
	memcpy(mi.active_special_image_flag, m_footprint_param.active_special_image_flag, sizeof(mi.active_special_image_flag));

	mi.equip_skill_level = m_footprint_param.equip_skill_level;
	memcpy(mi.equip_level_list, m_footprint_param.equip_level_list, sizeof(mi.equip_level_list));
	memcpy(mi.skill_level_list, m_footprint_param.skill_level_list, sizeof(mi.skill_level_list));
	memcpy(mi.special_img_grade_list, m_footprint_param.special_img_grade_list, sizeof(mi.special_img_grade_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&mi, sizeof(mi));
}

int FootPrint::GetFootprintAppeID()
{
	return m_footprint_param.used_imageid;
}

int FootPrint::GetHurtDecPercent()
{
// 	static const int SKILL_ID0 = 0;
// 	UNSTD_STATIC_CHECK(FootprintParam::SKILL_COUNT > SKILL_ID0);
// 
// 	const FootprintSkillConifg *skill0_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetSkillCfg(SKILL_ID0, m_footprint_param.skill_level_list[SKILL_ID0]);
// 	if (NULL != skill0_cfg && skill0_cfg->param_a > 0)
// 	{
// 		return skill0_cfg->param_a;
// 	}

	return 0;
}

// 受到攻击时，有param_b概率使自身闪避几率提高param_c%，持续param_d毫秒。 冷却param_a毫秒
void FootPrint::OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp, bool from_skill)
{
	static const int SKILL_ID0 = 0;
	UNSTD_STATIC_CHECK(FootprintParam::SKILL_COUNT > SKILL_ID0);

	const FootprintSkillConifg *skill_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetSkillCfg(SKILL_ID0, m_footprint_param.skill_level_list[SKILL_ID0]);
	if (nullptr == skill_cfg)
	{
		return;
	}

	unsigned long long now_sec_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();

	if (now_sec_ms < m_footprint_param.skill_last_colddown_time[SKILL_ID0])
	{
		return;
	}

	int rand_val = RandomNum(10000);
	if (rand_val >= skill_cfg->param_b)
	{
		return;
	}

	EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), SKILL_ID0, skill_cfg->param_d, skill_cfg->skill_level);
	effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI_PER, skill_cfg->param_c, skill_cfg->param_c);
	effect_buff->SetClientEffectType(EffectBase::ECT_SKILL_ADD_SHANBI_PER);
	m_role->AddEffect(effect_buff);

	m_footprint_param.skill_last_colddown_time[SKILL_ID0] = now_sec_ms + skill_cfg->param_a;
}

void FootPrint::OperationReq(const Protocol::CSFootprintOperate *req)
{
	if (NULL == req) return;
	
	switch (req->operate_type)
	{
	case Protocol::FOOTPRINT_OPERATE_TYPE_INFO_REQ:
	{
		this->SendFootprintInfo();
	}
	break;

	case Protocol::FOOTPRINT_OPERATE_TYPE_UP_GRADE:
	{
		//int old_level = m_footprint_param.grade;
		//bool need_continue_upgrade = true;
		bool ret = this->Upgrade(1 == req->param_2, req->param_1);
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_FOOTPRINT_UPGRADE, ret);
	}
	break;

	case Protocol::FOOTPRINT_OPERATE_TYPE_USE_IMAGE:
	{
		this->UseImage(req->param_1);
	}
	break;

	case Protocol::FOOTPRINT_OPERATE_TYPE_UP_LEVEL_EQUIP:
	{
		 this->UplevelEquip(req->param_1);
	}
	break;

	case Protocol::FOOTPRINT_OPERATE_TYPE_UP_STAR:
	{
		/*
		int old_grade = m_footprint_param.grade;
		const FootprintGradeConfig * grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetGradeConfig(old_grade);
		if (NULL == grade_cfg) return;

		int max_up_star_stuff_times = grade_cfg->pack_num;

		for (int i = 0; i < req->param_3 && i < max_up_star_stuff_times; i++)
		{
			bool ret = this->UpStarLevel(req->param_1, req->param_2, req->param_3);
			if (!ret || old_grade != m_footprint_param.grade)
			{
				m_role->SendOperateResult(Protocol::SCOperateResult::OP_FOOTPRINT_UPSTAR, 0);
				return;
			}
		}

		m_role->SendOperateResult(Protocol::SCOperateResult::OP_FOOTPRINT_UPSTAR, 1);
		*/
	}
	break;

	case Protocol::FOOTPRINT_OPERATE_TYPE_UP_LEVEL_SKILL:
	{
		this->UplevelSkill(req->param_1, 0 != req->param_2);
	}
	break;

	case Protocol::FOOTPRINT_OPERATE_TYPE_UP_SPECIAL_IMAGE:
	{
		this->FootprintSpecialImgUpgrade(req->param_1);
	}
	break;

	default:
		break;
	}
}

bool FootPrint::ActiveImage(int imageid, bool recalc)
{
	if (imageid <= 0 || imageid >= FootprintParam::MAX_IMAGE_COUNT) return false;

	m_footprint_param.ActiveImage(imageid);

	if (recalc)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT);
	}

	return true;
}

bool FootPrint::UseImage(int imageid)
{
	if (imageid < SPECIAL_IMG_START_ID)
	{
		if (!m_footprint_param.IsImageActive(imageid))
		{
			m_role->NoticeNum(errornum::EN_FOOTPRINT_IMG_NOT_ACTIVE);
			return false;
		}
	}
	else
	{
		if (!m_footprint_param.IsSpecialImageActive(imageid - SPECIAL_IMG_START_ID))
		{
			m_role->NoticeNum(errornum::EN_FOOTPRINT_IMG_NOT_ACTIVE);
			return false;
		}
	}

	m_footprint_param.used_imageid = imageid;

	m_role->RefreshApperance(true);

	this->SendFootprintInfo();

	return true;
}

bool FootPrint::UnUseImage(int imageid)
{
	if (imageid != m_footprint_param.used_imageid)
	{
		return false;
	}

	m_footprint_param.used_imageid = 0;

	m_role->RefreshApperance(true);

	this->SendFootprintInfo();

	return true;
}

bool FootPrint::ActiveSpecialImage(int imageid, bool is_notify)
{
	if (m_footprint_param.IsSpecialImageActive(imageid))
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_SPECIAL_IMG_ALREADY_ACTIVE);
		}

		return false;
	}

	const FootprintSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetFootPrintConfig().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	m_footprint_param.ActiveSpecialImage(imageid);
	m_role->GetImgFuling()->OnPutOnImg(JINGJIE_TYPE_FOOT_PRINT, imageid);
	if (0 == m_footprint_param.special_img_grade_list[imageid])
	{
		m_footprint_param.special_img_grade_list[imageid] = 1;
	}
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT);
	JinjiesysFuncHelper::Instance().OnActiveSpecialImg(m_role, JINJIE_TYPE_FOOTPRINT, imageid);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_FOOTPRINT,imageid,true);
	this->UseImage(SPECIAL_IMG_START_ID + imageid);
	return true;
}

bool FootPrint::UnActiveSpecialImage(int imageid)
{
	if (!m_footprint_param.IsSpecialImageActive(imageid))
	{
		return false;
	}

	const FootprintSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetFootPrintConfig().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	BitmapFunction::ClearBit(m_footprint_param.active_special_image_flag, sizeof(m_footprint_param.active_special_image_flag), imageid);
	if (1 == m_footprint_param.special_img_grade_list[imageid])
	{
		m_footprint_param.special_img_grade_list[imageid] = 0;
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_FOOTPRINT,imageid,false);
	if (this->UnUseImage(SPECIAL_IMG_START_ID + imageid))
	{
		this->UseImage(1); //默认再给一个
	}

	return true;
}

void FootPrint::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
//	if (!m_footprint_param.IsFootprintActived())
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
			const FootprintLevelConfig *level_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetLevelConfig(m_footprint_param.level);
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
			const FootprintGradeConfig * grade_config = LOGIC_CONFIG->GetFootPrintConfig().GetGradeConfig(m_footprint_param.grade);
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
				int special_img_per_add = JinjieSysRewardMgr::Instance().GetJinjieSpecialImgExtraAddPer(m_role, JINJIE_TYPE_FOOTPRINT);
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
				//if (m_footprint_param.grade_bless_val > 0)
				//{
				//	const FootprintGradeConfig * next_grade_config = LOGIC_CONFIG->GetFootPrintConfig().GetGradeConfig(m_footprint_param.grade + 1);
				//	if (NULL != next_grade_config && NULL != grade_config && grade_config->bless_val_limit > 0)
				//	{
				//		double rate = (((double)m_footprint_param.grade_bless_val) / grade_config->bless_val_limit) * (next_grade_config->bless_addition * ROLE_ATTR_PER);
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
			}
		}

		m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_FOOTPRINT, m_attrs_add, m_attrs_add);
		/*
		{
			// 星级加成  
			const FootprintUpStarExpCfg * star_level_config = LOGIC_CONFIG->GetFootPrintConfig().GetUpStarExpCfg(m_footprint_param.star_level);
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
		*/

		{
			// 装备
			int active_equip_grade = LOGIC_CONFIG->GetFootPrintConfig().GetActiveEquipGrade();
			if (active_equip_grade <= m_footprint_param.grade)
			{
				for (int i = 0; i < FootprintParam::EQUIP_COUNT; ++i)
				{
					const FootprintEquipConifg *equip_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetEquipCfg(i);
					if (NULL != equip_cfg)
					{
						int level = m_footprint_param.equip_level_list[i];
						if (level >= 0 && level < equip_cfg->max_level)
						{
							rate += equip_cfg->level_cfg[level].add_percent;
						}
					}
				}
			}

			{
				// 勋章加成
				rate += m_role->GetXunZhangManager()->GetFootprintAddPer();

				// 星座星魂加成
				int xinhun_add_per = 0;
				m_role->GetChineseZodiac()->GetXingHunTotalAddPer(NULL, NULL, NULL, NULL, NULL, NULL, &xinhun_add_per);
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

			if (active_equip_grade <= m_footprint_param.grade)
			{
				for (int i = 0; i < FootprintParam::EQUIP_COUNT; ++i)
				{
					const FootprintEquipConifg *equip_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetEquipCfg(i);
					if (NULL != equip_cfg)
					{
						int level = m_footprint_param.equip_level_list[i];
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
			for (int i = 0; i < FootprintParam::MAX_SPECIAL_IMAGE_COUNT; ++i)
			{
				if (m_footprint_param.IsSpecialImageActive(i))
				{
					// 特殊形象进阶
					const int grade = m_footprint_param.special_img_grade_list[i];

					const FootprintSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetSpecialImgUpgradeCfg(i, grade);
					if (NULL != upgrade_cfg)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += upgrade_cfg->maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += upgrade_cfg->gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += upgrade_cfg->fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += upgrade_cfg->mingzhong;
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
			if (m_footprint_param.shuxingdan_count > 0)
			{
				const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_FOOTPRINT, 0);
				if (NULL != cfg_item)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp * m_footprint_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji * m_footprint_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu * m_footprint_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg_item->mingzhong * m_footprint_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg_item->shanbi * m_footprint_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg_item->baoji * m_footprint_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg_item->jianren * m_footprint_param.shuxingdan_count;
				}
			}
		}

		{
			// 技能属性
			for (int i = 0; i < FootprintParam::SKILL_COUNT; i++)
			{
				if (m_footprint_param.skill_level_list[i] > 0)
				{
					const FootprintSkillConifg *skill_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetSkillCfg(i, m_footprint_param.skill_level_list[i]);
					if (NULL != skill_cfg)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += skill_cfg->maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += skill_cfg->gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += skill_cfg->fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += skill_cfg->mingzhong;
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

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_FOOTPRINT, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_FOOTPRINT, m_other_capability);

	m_footprint_param.capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_FOOTPRINT);

	base_add.Add(m_attrs_add.m_attrs, true);
}

FootprintParam * FootPrint::GetFootprintParam()
{
	return &m_footprint_param;
}

void FootPrint::PrintFootprint(const char *reason, int param1, int param2, int param3, int param4, int param5)
{
	UNSTD_STATIC_CHECK(4 == FootprintParam::EQUIP_COUNT);
	UNSTD_STATIC_CHECK(4 == FootprintParam::SKILL_COUNT);
	const int image_hex_len = FootprintParam::MAX_ACTIVE_IMAGE_BYTE * 2 + 1;
	const int special_image_hex_len = FootprintParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE * 2 + 1;
	char active_image_hex[image_hex_len] = { 0 };
	char active_special_image_hex[special_image_hex_len] = { 0 };
	BinToHex((const char*)m_footprint_param.active_image_flag, sizeof(m_footprint_param.active_image_flag), active_image_hex);
	BinToHex((const char*)m_footprint_param.active_special_image_flag, sizeof(m_footprint_param.active_special_image_flag), active_special_image_hex);
	gamelog::g_log_footprint.printf(LL_INFO, "Footprint role(%d,%s) %s(%d,%d,%d,%d,%d) "
		"level:%d, grade:%d,%d, used_imageid:%d shuxingdan_count:%d chengzhangdan_count:%d, image_flag:%s, special_image_flag:%s"
		"equip_level_list:%d, %d, %d, %d"
		"skill_level_list:%d, %d, %d, %d",
		m_role->GetUID(), m_role->GetName(),
		reason, param1, param2, param3, param4, param5,
		(int)m_footprint_param.level,
		(int)m_footprint_param.grade, (int)m_footprint_param.grade_bless_val,
		(int)m_footprint_param.used_imageid,
		(int)m_footprint_param.shuxingdan_count, (int)m_footprint_param.chengzhangdan_count,
		active_image_hex, active_special_image_hex, 
		(int)m_footprint_param.equip_level_list[0], (int)m_footprint_param.equip_level_list[1], (int)m_footprint_param.equip_level_list[2], (int)m_footprint_param.equip_level_list[3],
		(int)m_footprint_param.skill_level_list[0], (int)m_footprint_param.skill_level_list[1], (int)m_footprint_param.skill_level_list[2], (int)m_footprint_param.skill_level_list[3]);
}

void FootPrint::OnCompleteTask(TaskID task_id)
{
	this->CheckActive();
//	if (LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenTaskID(FUN_OPEN_TYPE_FOOTPRINT) == task_id)
//	{
//		this->ActiveFootprint();
//	}
}

void FootPrint::OnRoleUpLevel(int level)
{
	this->CheckActive();
//	if (m_footprint_param.IsFootprintActived()) return;
//
//	if (level >= LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenRoleLevel(FUN_OPEN_TYPE_FOOTPRINT))
//	{
//		this->ActiveFootprint();
//	}
}

bool FootPrint::OnUseShuxingdan(int slot_idx, short &use_count)
{
	if (slot_idx < 0 || slot_idx >= SHUXINGDAN_MAX_TYPE || use_count <= 0)
	{
		return false;
	}

	const FootprintGradeConfig *grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetGradeConfig(m_footprint_param.grade);
	if (NULL == grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_FOOTPRINT_SHUXINGDAN_USE_LIMIT_NUM);
		return false;
	}

	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_FOOTPRINT, slot_idx);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_footprint_param.grade < cfg_item->order_limit)
	{
		gstr::SendError(m_role, "shuxingdan_use_order_limit_1", SHUXINGDAN_TYPE_FOOTPRINT, cfg_item->order_limit - 1, slot_idx); // 客户端阶数-1
		return false;
	}

	if (m_footprint_param.shuxingdan_count + use_count > cfg_item->use_limit_num)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	int special_img_add_count = 0;
	for (int i = 0; i < FootprintParam::MAX_SPECIAL_IMAGE_COUNT; i++)
	{
		if (!m_footprint_param.IsSpecialImageActive(i))
			continue;

		const FootprintSpecialImgUpgradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetSpecialImgUpgradeCfg(i, m_footprint_param.special_img_grade_list[i]);
		if (NULL != special_img_grade_cfg)
		{
			special_img_add_count += special_img_grade_cfg->shuxingdan_count;
		}
	}

	// 策划需求，修正真正使用数量
	int can_use_count = grade_cfg->shuxingdan_limit + special_img_add_count - m_footprint_param.shuxingdan_count;
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

	m_footprint_param.shuxingdan_count += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT);
	this->SendFootprintInfo();

	return true;
}

bool FootPrint::OnUseChengzhangdan(int use_count)
{
	const FootprintGradeConfig *grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetGradeConfig(m_footprint_param.grade);
	if (NULL == grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_CHENGZHANGDAN_USE_LIMIT_NUM);
		return false;
	}

	int special_img_add_count = 0;
	for (int i = 0; i < FootprintParam::MAX_SPECIAL_IMAGE_COUNT; i++)
	{
		if (!m_footprint_param.IsSpecialImageActive(i))
			continue;

		const FootprintSpecialImgUpgradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetSpecialImgUpgradeCfg(i, m_footprint_param.special_img_grade_list[i]);
		if (NULL != special_img_grade_cfg)
		{
			special_img_add_count += special_img_grade_cfg->chengzhangdan_count;
		}
	}

	if (m_footprint_param.chengzhangdan_count + use_count > grade_cfg->chengzhangdan_limit + special_img_add_count)
	{
		m_role->NoticeNum(errornum::EN_CHENGZHANGDAN_USE_LIMIT_NUM);
		return false;
	}

	m_footprint_param.chengzhangdan_count += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT);
	this->SendFootprintInfo();

	return true;
}

int FootPrint::RefreshFootprintMinEquipLevel()
{
	int min = m_footprint_param.equip_level_list[0];

	for (int i = 1; i < FootprintParam::EQUIP_COUNT; ++i)
	{
		min = m_footprint_param.equip_level_list[i] < min ? m_footprint_param.equip_level_list[i] : min;
	}

	return min;
}

bool FootPrint::Upgrade(bool is_auto_buy, int repeat_times)
{
	Knapsack *knapsack = m_role->GetKnapsack();
	if (NULL == knapsack) return false;

	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!m_footprint_param.IsFootprintActived())
	{
		m_role->NoticeNum(errornum::EN_FOOTPRINT_NOT_EXIST);
		return false;
	}

	short old_grade = m_footprint_param.grade;
	short oldest_bless_val = m_footprint_param.grade_bless_val;
	bool succeeded = false;

	const FootprintGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetGradeConfig(old_grade);
	if (NULL == curr_grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_FOOTPRINT_CANNOT_UPGRADE);
		return false;
	}

	const FootprintGradeConfig * target_grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetGradeConfig(old_grade + 1);
	if (NULL == target_grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_FOOTPRINT_CANNOT_UPGRADE);
		return false;
	}

	for (int i = 0; i < repeat_times; ++i)
	{
		short old_bless_val = m_footprint_param.grade_bless_val;
		const FootprintUpGradeProbConfig * upgrade_prob_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetUpGradeProbConfig(old_grade, old_bless_val);
		if (NULL == upgrade_prob_cfg)
		{
			m_role->NoticeNum(errornum::EN_FOOTPRINT_CANNOT_UPGRADE);
			//return false;
			succeeded = true;
			break;
		}

		static ItemExtern::ItemConsumeList consume_list;
		consume_list.Reset();

		{
			// 进阶符
			int stuff_count = 0;
			ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

			short upgrade_stuff_num = m_role->GetKnapsack()->Count(curr_grade_cfg->upgrade_stuff2_id);
			// 如果有限时进阶丹, 则优先使用
			if (upgrade_stuff_num > 0)
			{
				stuff_list[stuff_count].item_id = curr_grade_cfg->upgrade_stuff2_id;
				stuff_list[stuff_count].num = upgrade_stuff_num >= curr_grade_cfg->upgrade_stuff_count ? curr_grade_cfg->upgrade_stuff_count : upgrade_stuff_num;
				stuff_list[stuff_count].buyable = is_auto_buy;

				++stuff_count;
			}

			// 如果限时进阶丹用完了，还不够一次批量使用，就使用普通进阶丹
			if (upgrade_stuff_num < curr_grade_cfg->upgrade_stuff_count && stuff_count < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM)
			{
				stuff_list[stuff_count].item_id = curr_grade_cfg->upgrade_stuff_id;
				stuff_list[stuff_count].num = curr_grade_cfg->upgrade_stuff_count - upgrade_stuff_num;
				stuff_list[stuff_count].buyable = is_auto_buy;

				++stuff_count;
			}

			if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, false))
			{
				//return false;
				succeeded = true;
				break;
			}
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "FootPrint::Upgrade"))
		{
			succeeded = true;
			break;
		}
		
		{
			// 生效
			int rand_val = RandomNum(10000);

			if (rand_val < upgrade_prob_cfg->succ_rate || m_footprint_param.grade_bless_val >= curr_grade_cfg->bless_val_limit)
			{
				succeeded = true;
			}

			if (!succeeded)
			{
				int crit_value = UpgradeSysCrit::Instance().CalculateCritValue(m_role, UPGRADE_SYS_CRIT_TYPE_FOOTPRINT);
				m_footprint_param.grade_bless_val += (upgrade_prob_cfg->RandAddBlessVal() * crit_value);
				//m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT);				//计算祝福值临时属性

				if (m_footprint_param.grade_bless_val > curr_grade_cfg->bless_val_limit)
				{
					m_footprint_param.grade_bless_val = curr_grade_cfg->bless_val_limit;
				}

			}
			else
			{
				m_footprint_param.grade++;
				m_footprint_param.grade_bless_val = 0;

				// 			// 进阶成功清空时间
				// 			m_footprint_param.clear_upgrade_time = 0;

				const FootprintSkillConifg *skill_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetSkillCfg(0, m_footprint_param.skill_level_list[0] + 1);
				if (NULL != skill_cfg && m_footprint_param.grade >= skill_cfg->halo_grade)
				{
					m_footprint_param.skill_level_list[0]++;
				}
				//注释无用代码
				/*
				const FootprintGradeConfig *grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetGradeConfig(m_footprint_param.grade);
				if(NULL == grade_cfg)
				{
					//return false;
					succeeded = true;
					break;
				}
				else
				{
					for(int i = 0; i < FootprintParam::EQUIP_COUNT; i++)
					{
						int special_img_add = 0;
						for (int j = 0;j <= FootprintParam::MAX_SPECIAL_IMAGE_ID; j++)
						{
							const FootprintSpecialImgUpgradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetSpecialImgUpgradeCfg(j, m_footprint_param.special_img_grade_list[j]);
							if (NULL != special_img_grade_cfg)
							{
								special_img_add += special_img_grade_cfg->equip_level;
							}
						}
					}
				}
				*/
				this->ActiveImage(target_grade_cfg->active_image_id, false);
				this->UseImage(target_grade_cfg->active_image_id);

				m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT);

				if (target_grade_cfg->is_broadcast)
				{
					int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_footprint_upgrade,
						UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade);
					if (sendlen > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
					}
				}

				//m_role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_FOOTSPRINT, m_footprint_param.grade);   // 进阶丹返还活动
				break;
			}
		}
	}
	this->SendFootprintInfo();
	this->PrintFootprint("FootprintUpgrade", old_grade, oldest_bless_val);

	EventHandler::Instance().OnFootprintUpgrade(m_role, old_grade, m_footprint_param.grade);

	return !succeeded;
}

bool FootPrint::UplevelEquip(int equip_idx)
{
	if (equip_idx < 0 || equip_idx >= FootprintParam::EQUIP_COUNT)
	{
		return false;
	}

	if (m_role->GetKnapsack()->IsLock() || m_role->GetSublock()->CheckHasLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!m_footprint_param.IsFootprintActived())
	{
		return false;
	}

	FootPrint *footprint = m_role->GetFootPrint();
	if (NULL == footprint)
	{
		return false;
	}
	FootprintParam *footprintparam = footprint->GetFootprintParam();
	if (NULL == footprintparam)
	{
		return false;
	}

	const FootprintEquipConifg *equip_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetEquipCfg(equip_idx);
	if (NULL == equip_cfg)
	{
		return false;
	}
	
	short level = m_footprint_param.equip_level_list[equip_idx];
	//if (level >= equip_cfg->max_level - 1)
	//{
	//	m_role->NoticeNum(errornum::EN_EQUIP_LEVEL_MAX);
	//	return false;
	//}

	//策划需求，装备升级级数由足迹阶数决定
	const FootprintGradeConfig * cur_grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetGradeConfig(footprintparam->grade);
	if (cur_grade_cfg == NULL)
	{
		return false;
	}

	if (level >= cur_grade_cfg->equip_level_toplimit)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_LEVEL_MAX);
		return false;
	}

	short old_level = level;

	if (m_footprint_param.grade < LOGIC_CONFIG->GetFootPrintConfig().GetActiveEquipGrade())
	{
		return false;
	}

	// 消耗
	if (!m_role->GetKnapsack()->ConsumeItem(equip_cfg->level_cfg[level].item.item_id, equip_cfg->level_cfg[level].item.num, "FootprintEquipUplevel"))
	{
		return false;
	}

	// 结算

	m_footprint_param.equip_level_list[equip_idx]++;

	bool is_active_upgrade_skill = false;
	int min_equip_level = this->RefreshFootprintMinEquipLevel();
	if (LOGIC_CONFIG->GetFootPrintConfig().GetActiveEquipSkillLevel() <= min_equip_level)
	{
		int equip_skill_level = LOGIC_CONFIG->GetFootPrintConfig().GetEquipSkillLevel(min_equip_level);
		if (equip_skill_level > 0)
		{
			m_footprint_param.equip_skill_level = equip_skill_level;
			is_active_upgrade_skill = true;
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT);
	this->SendFootprintInfo();

	if (is_active_upgrade_skill)
	{
		CommonDataParam *common_data = m_role->GetCommonDataParam();
		if (NULL != common_data)
		{
			if (0 == (common_data->upgrade_skill_skill_active_flag & (1 << UpGradeSkillConfig::SKILL_TYPE_FOOTPRINT)))
			{
				common_data->upgrade_skill_skill_active_flag |= (1 << UpGradeSkillConfig::SKILL_TYPE_FOOTPRINT);
				if (-1 == common_data->upgrade_skill_next_skill_index)
				{
					common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_FOOTPRINT;
				}

				m_role->SendGradeSkillInfo(m_role);
			}
		}
	}

	this->PrintFootprint("UplevelEquip", old_level);

	return true;
}

bool FootPrint::UpStarLevel(short stuff_index, short is_auto_buy, int loop_times)
{
	if (stuff_index < 0 || stuff_index >= FootprintParam::MAX_UP_STAR_ITEM_COUNT)
		return false;

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock() || m_role->GetSublock()->CheckHasLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!m_footprint_param.IsFootprintActived())
	{
		m_role->NoticeNum(errornum::EN_FOOTPRINT_NOT_EXIST);
		return false;
	}

	short old_star_level = m_footprint_param.star_level;
	short old_grade = m_footprint_param.grade;
	int old_bless_val = m_footprint_param.grade_bless_val;

	const FootprintUpStarExpCfg * curr_star_level_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetUpStarExpCfg(old_star_level);
	if (NULL == curr_star_level_cfg)
	{
		return false;
	}

	const FootprintUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetUpStarExpCfg(old_star_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return false;
	}

	const FootprintUpStarStuffCfg * up_star_stuff_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetUpStarStuffCfg(stuff_index);
	if (NULL == up_star_stuff_cfg)
	{
		return false;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(up_star_stuff_cfg->up_star_item_id);
	if (NULL == item_base)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();

	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[0].item_id = up_star_stuff_cfg->up_star_item_id;
	stuff_list[0].num = 1;
	stuff_list[0].buyable = 1 == is_auto_buy ? true : false;

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
	{
		return false;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "FootPrint::UpStarLevel"))
	{
		return false;
	}

	// 结算
	{
		int rand_value = RandomNum(10000);
		if (rand_value < up_star_stuff_cfg->baoji_per)
		{
			m_footprint_param.grade_bless_val += up_star_stuff_cfg->star_exp * up_star_stuff_cfg->baoji_mul;
		}
		else
		{
			m_footprint_param.grade_bless_val += up_star_stuff_cfg->star_exp;
		}

		int up_star_need_exp = curr_star_level_cfg->up_star_level_exp;
		while (m_footprint_param.grade_bless_val >= up_star_need_exp)
		{
			const FootprintUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetUpStarExpCfg(m_footprint_param.star_level + 1);
			if (NULL == next_star_level_cfg || up_star_need_exp <= 0)
			{
				m_footprint_param.grade_bless_val = 0;
				break;
			}

			m_footprint_param.grade_bless_val -= up_star_need_exp;
			m_footprint_param.star_level++;

			if ((m_footprint_param.grade - 1) != (m_footprint_param.star_level / 10) && (m_footprint_param.grade - 1) < (m_footprint_param.star_level / 10))
			{
				m_footprint_param.grade = (m_footprint_param.star_level / 10) + 1;

				const FootprintGradeConfig * grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetGradeConfig(m_footprint_param.grade);
				if (NULL != grade_cfg)
				{
					this->ActiveImage(grade_cfg->active_image_id, false);
					this->UseImage(grade_cfg->active_image_id);
					if (grade_cfg->is_broadcast)
					{
						int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_footprint_upgrade,
							UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), grade_cfg->client_grade, grade_cfg->grade);
						if (sendlen > 0)
						{
							World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
						}
					}
				}

				const FootprintSkillConifg *skill_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetSkillCfg(0, m_footprint_param.skill_level_list[0] + 1);
				if (NULL != skill_cfg && m_footprint_param.grade >= skill_cfg->halo_grade)
				{
					m_footprint_param.skill_level_list[0]++;
				}
			}

			up_star_need_exp = next_star_level_cfg->up_star_level_exp;
		}

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMG_FULING);			//形象赋灵增加百分比进阶属性
	}

	this->SendFootprintInfo();

	this->PrintFootprint("FootprintUpgrade", old_star_level, old_bless_val);

	EventHandler::Instance().OnFootprintUpgrade(m_role, old_grade, m_footprint_param.grade);
	EventHandler::Instance().OnFootprintUpStar(m_role, old_star_level, m_footprint_param.star_level);

	return true;
}

bool FootPrint::UplevelSkill(int skill_idx, bool is_auto_buy)
{
	if (skill_idx < 0 || skill_idx >= FootprintParam::SKILL_COUNT)
	{
		return false;
	}

	if (m_role->GetKnapsack()->IsLock() || m_role->GetSublock()->CheckHasLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!m_footprint_param.IsFootprintActived())
	{
		m_role->NoticeNum(errornum::EN_FOOTPRINT_NOT_EXIST);
		return false;
	}

	const FootprintSkillConifg *target_skill_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetSkillCfg(skill_idx, m_footprint_param.skill_level_list[skill_idx] + 1);
	if (NULL == target_skill_cfg)
	{
		return false;
	}

	if (0 == target_skill_cfg->skill_type)
	{
		return false;
	}

	if (target_skill_cfg->halo_grade > m_footprint_param.grade)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

	{
		// 升级材料
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = target_skill_cfg->uplevel_stuff_id;
		stuff_list[0].num = target_skill_cfg->uplevel_stuff_num;
		stuff_list[0].buyable = is_auto_buy;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
		{
			return false;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "FootPrint::UplevelSkill"))
	{
		return false;
	}

	short skill_level = m_footprint_param.skill_level_list[skill_idx];
	m_footprint_param.skill_level_list[skill_idx]++;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT);
	this->SendFootprintInfo();

	this->PrintFootprint("UplevelSkill", skill_idx, skill_level);

	return true;
}

bool FootPrint::FootprintSpecialImgUpgrade(short special_img_id)
{
	if (special_img_id <= 0 || special_img_id >= FootprintParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return false;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!m_footprint_param.IsSpecialImageActive(special_img_id))
	{
		m_role->NoticeNum(errornum::EN_FOOTPRINT_IMG_NOT_ACTIVE);
		return false;
	}

	const int old_grade = m_footprint_param.special_img_grade_list[special_img_id];
	if (old_grade >= FootprintParam::MAX_UPGRADE_LIMIT)
	{
		m_role->NoticeNum(errornum::EN_FOOTPRINT_SPECIAL_IMG_GRADE_LIMIT);
		return false;
	}

	const FootprintSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetSpecialImgUpgradeCfg(special_img_id, old_grade + 1);
	if (NULL == upgrade_cfg)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

	{
		// 进阶符
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = upgrade_cfg->stuff_id;
		stuff_list[0].num = upgrade_cfg->stuff_num;
		stuff_list[0].buyable = false;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
		{
			m_role->NoticeNum(errornum::EN_ITEM_FUNC_ITEM_COMPOSE_NOT_ENOUGH);
			return false;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "FootPrint::FootprintSpecialImgUpgrade"))
	{
		return false;
	}

	// 生效
	++m_footprint_param.special_img_grade_list[special_img_id];

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT);

	this->SendFootprintInfo();

	this->PrintFootprint("FootprintSpecialImgUpgrade", old_grade, m_footprint_param.special_img_grade_list[special_img_id]);

	return true;
}

bool FootPrint::UpOneGrade(int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num)
{
	if (!m_footprint_param.IsFootprintActived())
	{
		m_role->NoticeNum(errornum::EN_FOOTPRINT_NOT_EXIST);
		return false;
	}

	if (m_footprint_param.grade < valid_min_grade)
	{
		m_role->NoticeNum(errornum::EN_FOOTPRINT_UP_ONE_GRADE_LIMIT);
		return false;
	}
	else if (m_footprint_param.grade > valid_max_grade)
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(return_item_id);
		if (item_base != nullptr)
		{
			for (int loop_time = 0; return_item_num > 0 && loop_time < 1000; ++loop_time)
			{
				int put_num = return_item_num > item_base->GetPileLimit() ? item_base->GetPileLimit() : return_item_num;
				return_item_num -= put_num;

				ItemConfigData return_item(return_item_id, true, put_num);
				m_role->GetKnapsack()->PutOrMail(return_item, PUT_REASON_FOOTPRINT_UPGRADE_ONE_RETURN);
			}

			return true;
		}
		return false;
	}

	int old_grade = m_footprint_param.grade;
	int old_bless_val = m_footprint_param.grade_bless_val;

	const FootprintGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetGradeConfig(old_grade);
	if (NULL == curr_grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_FOOTPRINT_CANNOT_UPGRADE);
		return false;
	}

	const FootprintGradeConfig * target_grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetGradeConfig(old_grade + 1);
	if (NULL == target_grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_FOOTPRINT_CANNOT_UPGRADE);
		return false;
	}

	{
		m_footprint_param.grade++;
		m_footprint_param.grade_bless_val = 0;
		m_footprint_param.star_level = (m_footprint_param.grade - 1) * 10;

		// 			// 进阶成功清空时间
		// 			mount->clear_upgrade_time = 0;

		const FootprintSkillConifg *skill_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetSkillCfg(0, m_footprint_param.skill_level_list[0] + 1);
		if (NULL != skill_cfg && m_footprint_param.grade >= skill_cfg->halo_grade)
		{
			m_footprint_param.skill_level_list[0]++;
		}

		this->ActiveImage(target_grade_cfg->active_image_id, false);
		this->UseImage(target_grade_cfg->active_image_id);

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT);

		if (target_grade_cfg->is_broadcast)
		{
			int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_footprint_upgrade,
				UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade);
			if (sendlen > 0)
			{
				if (target_grade_cfg->client_grade >= 5)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
				}
				else
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}

	this->PrintFootprint("FootPrintUpgrade", old_grade, old_bless_val);

	EventHandler::Instance().OnFootprintUpgrade(m_role, old_grade, m_footprint_param.grade);

	return true;
}

bool FootPrint::ActiveFootprint(bool is_recal)
{
	if (!m_footprint_param.IsFootprintActived())
	{
		m_footprint_param.star_level = 0;
		m_footprint_param.level = m_role->GetLevel();
		m_footprint_param.grade = 1;
		m_footprint_param.grade_bless_val = 0;

		const FootprintGradeConfig *grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetGradeConfig(1);
		if (NULL != grade_cfg)
		{
			this->ActiveImage(grade_cfg->active_image_id, false);
			this->UseImage(grade_cfg->active_image_id);

			//初始化每日更新时间
			m_footprint_param.clear_upgrade_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + static_cast<unsigned int>(EngineAdapter::Instance().NextDayInterval(5, 0, 0));
		}

		const FootprintSkillConifg *skill_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetSkillCfg(0, 1);
		if (NULL != skill_cfg && m_footprint_param.grade >= skill_cfg->halo_grade)
		{
			m_footprint_param.skill_level_list[0] = 1;
		}

		if (is_recal)
		{
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT);
		}

		this->SendFootprintInfo();

		this->PrintFootprint("ActiveFootprint");

		m_role->GetRoleGoal()->CheckCond(COND_TYPE_UPGRADE_FOOTPRINT, 0, 0);
		JinjiesysFuncHelper::Instance().OnActiveJinjieSys(m_role, JINJIE_TYPE_FOOTPRINT);

	}

	return true;
}

bool FootPrint::GMResetGrade()
{
	m_footprint_param.grade = 1;
	m_footprint_param.grade_bless_val = 0;

	this->SendFootprintInfo();

	return true;
}

int FootPrint::UplevelAndSaveBless(Role *role, int add_star)
{
	FootPrint *footprint_mgr = role->GetFootPrint();
	if (NULL == footprint_mgr)
	{
		return -1;
	}

	FootprintParam *footprint_param = footprint_mgr->GetFootprintParam();
	if (NULL == footprint_param)
	{
		return -2;
	}

	if (!footprint_param->IsFootprintActived())
	{
		return -3;
	}

	short old_star_level = footprint_param->star_level;
	short old_grade = footprint_param->grade;
	int old_bless_val = footprint_param->grade_bless_val;

	const FootprintUpStarExpCfg * curr_star_level_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetUpStarExpCfg(old_star_level);
	if (NULL == curr_star_level_cfg)
	{
		return -4;
	}

	const FootprintUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetUpStarExpCfg(old_star_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return -5;
	}

	{
		footprint_param->star_level += add_star;
		footprint_param->grade_bless_val = old_bless_val;

		if ((footprint_param->grade - 1) != (footprint_param->star_level / 10) && (footprint_param->grade - 1) < (footprint_param->star_level / 10))
		{
			footprint_param->grade = (footprint_param->star_level / 10) + 1;

			const FootprintGradeConfig * grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetGradeConfig(footprint_param->grade);
			if (NULL != grade_cfg)
			{
				role->GetFootPrint()->ActiveImage(grade_cfg->active_image_id, false);
				role->GetFootPrint()->UseImage(grade_cfg->active_image_id);
				if (grade_cfg->is_broadcast)
				{
					int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_footprint_upgrade,
						UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), grade_cfg->client_grade, grade_cfg->grade);
					if (sendlen > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
					}
				}
			}

			const FootprintSkillConifg *skill_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetSkillCfg(0, footprint_param->skill_level_list[0] + 1);
			if (NULL != skill_cfg && footprint_param->grade >= skill_cfg->halo_grade)
			{
				footprint_param->skill_level_list[0]++;
			}
		}

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT);
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_IMG_FULING);			//形象赋灵增加百分比进阶属性
	}

	role->GetFootPrint()->SendFootprintInfo();

	role->GetFootPrint()->PrintFootprint("FootprintUpgrade", old_star_level, old_bless_val);

	EventHandler::Instance().OnFootprintUpgrade(role, old_grade, footprint_param->grade);
	EventHandler::Instance().OnFootprintUpStar(role, old_star_level, footprint_param->star_level);

	role->GetFootPrint()->PrintFootprint(__FUNCTION__, old_grade, old_bless_val);

	return footprint_param->star_level;
}

bool FootPrint::CheckCanUpGrade(Role* role)
{
	if (NULL == role)
	{
		return false;
	}

	FootprintParam* footprint_param = role->GetFootPrint()->GetFootprintParam();
	if (NULL == footprint_param)
	{
		return false;
	}

	if (!footprint_param->IsFootprintActived())
	{
		return false;
	}

	const FootprintUpStarExpCfg * next_star_level_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetUpStarExpCfg(footprint_param->star_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return false;
	}

	return true;
}

void FootPrint::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->CheckActive();
	}
}

void FootPrint::CheckActive()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (!m_footprint_param.IsFootprintActived())
	{
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_FOOTPRINT);
		if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day)
		{
			if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
			{
				this->ActiveFootprint();
			}
		}
	}
}

// 幻化特殊战力加成
void FootPrint::HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add)
{
	if (!LOGIC_CONFIG->GetFootPrintConfig().IsAddSpecialCap())
	{
		return;
	}

	auto cfg_item = LOGIC_CONFIG->GetFootPrintConfig().GetHuanHuaSpecialCapAddCfg(huanhua_id);
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

int FootPrint::GetSpecialImageLevelById(int special_image_id)
{
	if (special_image_id < 0 || special_image_id >= FootprintParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return 0;
	}
	if (!m_footprint_param.IsSpecialImageActive(special_image_id))
	{
		return 0;
	}
	return m_footprint_param.special_img_grade_list[special_image_id];
}
