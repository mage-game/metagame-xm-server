#include "yaoshimanager.hpp"
#include "gamelog.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "protocal/msgyaoshi.hpp"
#include "scene/scene.h"
#include "config/logicconfigmanager.hpp"
#include "other/yaoshi/yaoshiconfig.hpp"
#include "config/joinlimitconfig.hpp"
#include "servercommon/vipdef.hpp"
#include "other/yaoshi/yaoshifunction.hpp"
#include "serverlogic.h"
#include "config/shuxingdanconfig.hpp"
#include "task/taskrecorder.h"
#include "other/capability/capability.hpp"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "gstr.h"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "other/jinjiesysreward/jinjiesysrewardmgr.hpp"
#include "other/dressingroom/dressingroomconfig.hpp"
#include "other/dressingroom/dressingroommgr.hpp"
#include "other/talent/talent.hpp"
#include "common/bintohex.h"
#include "servercommon/serverconfig/crossconfig.hpp"

YaoShiManager::YaoShiManager()
	: m_role(nullptr),  m_other_capability(0)
{
}

YaoShiManager::~YaoShiManager()
{

}

void YaoShiManager::Init(Role *role, const YaoShiParam &yaoshi_param)
{
	m_role = role;
	m_yaoshi_param = yaoshi_param;

}

void YaoShiManager::GetOtherInitParam(YaoShiParam *param)
{
	*param = m_yaoshi_param;
}

void YaoShiManager::GetRoleAppearance(RoleAppearance *appearance)
{
	if (nullptr != appearance)
	{
		appearance->yaoshi_used_imageid = m_yaoshi_param.temporary_imageid > 0 ? m_yaoshi_param.temporary_imageid : m_yaoshi_param.used_imageid;
	}
}

void YaoShiManager::OnRoleLevelChange(int level)
{
	m_yaoshi_param.level = level;

	if (m_yaoshi_param.IsYaoShiActived())
	{
		this->SendYaoShiInfo();
	}
}

void YaoShiManager::Update(time_t now_second)
{
	if (0 != m_yaoshi_param.clear_upgrade_timestamp && now_second > m_yaoshi_param.clear_upgrade_timestamp)
	{
		m_yaoshi_param.grade_bless_val = 0;
		m_yaoshi_param.clear_upgrade_timestamp = 0;
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_YAOSHI);
		this->SendYaoShiInfo();
	}

	if (0 != m_yaoshi_param.temporary_imageid_invalid_time && static_cast<unsigned int>(now_second) > m_yaoshi_param.temporary_imageid_invalid_time)
	{
		if (0 != m_yaoshi_param.temporary_imageid_has_select)
		{
			m_yaoshi_param.temporary_imageid = 0;
			m_yaoshi_param.temporary_imageid_has_select = 0;
			this->SendYaoShiInfo();

			{
				Protocol::SCYaoShiAppeChange mac;
				mac.objid = m_role->GetId();
				mac.yaoshi_appeid = m_yaoshi_param.used_imageid;

				if (nullptr != m_role->GetScene()) 
				{
					m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&mac, sizeof(mac));
				}

				m_role->RefreshApperance(false);
			}
		}
	}
}

void YaoShiManager::OnRoleLogin()
{
	this->CheckActive();
}

void YaoShiManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->CheckActive();
	}
}

void YaoShiManager::CheckActive()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (!m_yaoshi_param.IsYaoShiActived())
	{
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_YAOSHI);
		if (nullptr != fun_open_cfg &&  opengame_day >= fun_open_cfg->opengame_day)
		{
			if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
			{
				YaoShiFunction::ActiveYaoShi(m_role);
			}
		}
	}
}

void YaoShiManager::OnHurtOther(Character *target, int skill_id, long long &injure, int target_index, int &passive_flag)
{

}


// 受到攻击时，有param_b%概率获得param_c层伤害减免param_d%的效果。每次受到伤害减少1层，3层BUFF可以叠加生效,冷却param_a毫秒
void YaoShiManager::OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{
	if (!m_role->IsAlive())
	{
		return;
	}

	if (delta_hp >= 0) return;

	const YaoShiSkillConifg *skill_cfg = YAOSHI_CFG().GetSkillCfg(YaoShiParam::YAOSHI_SKILL_INDEX_0, m_yaoshi_param.skill_level_list[YaoShiParam::YAOSHI_SKILL_INDEX_0]);
	if (nullptr == skill_cfg) return;

	unsigned long long now_sec_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();

	if (now_sec_ms < m_yaoshi_param.skill0_last_colddown_time)
	{
		// 持续技能效果,每次攻击效果递减
		if (m_yaoshi_param.skill0_stack_count > 0)
		{
			double dec_hurt_per = static_cast<double>(m_yaoshi_param.skill0_stack_count * (skill_cfg->param_d * ROLE_ATTR_PER));
			delta_hp -= static_cast<Attribute>(delta_hp * dec_hurt_per);
			if (delta_hp > 0) delta_hp = 0;

			m_yaoshi_param.skill0_stack_count--;
		}
	}
	else
	{
		// 触发技能新效果
		int rate = RandomNum(10000);
		if (rate >= skill_cfg->param_b)
		{
			return;
		}

		m_yaoshi_param.skill0_stack_count = skill_cfg->param_c;  // 技能层数

		m_yaoshi_param.skill0_last_colddown_time = now_sec_ms + skill_cfg->param_a;
	}
}

void YaoShiManager::SendYaoShiInfo()
{
	Protocol::SCYaoShiInfo mi;
	mi.yaoshi_level = m_yaoshi_param.level;
	mi.grade = m_yaoshi_param.grade;
	mi.grade_bless_val = m_yaoshi_param.grade_bless_val;
	mi.clear_upgrade_time = m_yaoshi_param.clear_upgrade_timestamp;
	mi.used_imageid = m_yaoshi_param.used_imageid;
	UNSTD_STATIC_CHECK(sizeof(mi.active_image_flag) == sizeof(m_yaoshi_param.active_image_flag));
	memcpy(mi.active_image_flag, m_yaoshi_param.active_image_flag, sizeof(mi.active_image_flag));
	UNSTD_STATIC_CHECK(sizeof(mi.active_special_image_flag) == sizeof(m_yaoshi_param.active_special_image_flag));
	memcpy(mi.active_special_image_flag, m_yaoshi_param.active_special_image_flag, sizeof(mi.active_special_image_flag));
	mi.temporary_imageid = m_yaoshi_param.temporary_imageid;
	mi.temporary_imageid_has_select = m_yaoshi_param.temporary_imageid_has_select;
	mi.temporary_imageid_invalid_time = m_yaoshi_param.temporary_imageid_invalid_time;
	mi.star_level = m_yaoshi_param.star_level;

	ARRAYCPY(mi.shuxingdan_list, m_yaoshi_param.shuxingdan_list);
	ARRAYCPY(mi.skill_level_list, m_yaoshi_param.skill_level_list);
	memcpy(mi.special_img_grade_list, m_yaoshi_param.special_img_grade_list, sizeof(mi.special_img_grade_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&mi, sizeof(mi));
}

int YaoShiManager::GetYaoShiAppeID()
{
	return m_yaoshi_param.used_imageid;
}

bool YaoShiManager::ActiveImage(int imageid, bool recalc)
{
	if (imageid <= 0 || imageid >= YaoShiParam::MAX_IMAGE_COUNT) return false;

	m_yaoshi_param.ActiveImage(imageid);

	if (recalc)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_YAOSHI);
	}

	return true;
}

bool YaoShiManager::UseImage(int imageid)
{
	if (imageid < SPECIAL_IMG_START_ID)
	{
		if (!m_yaoshi_param.IsImageActive(imageid))
		{
			m_role->NoticeNum(errornum::EN_YAOSHI_IMG_NOT_ACTIVE);
			return false;
		}
	}
	else
	{
		if (!m_yaoshi_param.IsSpecialImageActive(imageid - SPECIAL_IMG_START_ID))
		{
			m_role->NoticeNum(errornum::EN_YAOSHI_IMG_NOT_ACTIVE);
			return false;
		}
	}

	m_yaoshi_param.used_imageid = imageid;

	if (0 != m_yaoshi_param.temporary_imageid)
	{
		m_yaoshi_param.temporary_imageid = 0;
	}

	{
		Protocol::SCYaoShiAppeChange mac;
		mac.objid = m_role->GetId();
		mac.yaoshi_appeid = m_yaoshi_param.used_imageid;
		
		if (nullptr != m_role->GetScene()) 
		{
			m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&mac, sizeof(mac));
		}

		m_role->RefreshApperance(false);
	}

	this->SendYaoShiInfo();

	return true;
}

bool YaoShiManager::UnUseImage(int imageid)
{
	if (imageid != m_yaoshi_param.used_imageid)
	{
		return false;
	}

	m_yaoshi_param.used_imageid = 0;

	{
		Protocol::SCYaoShiAppeChange mac;
		mac.objid = m_role->GetId();
		mac.yaoshi_appeid = m_yaoshi_param.used_imageid;

		if (nullptr != m_role->GetScene()) 
		{
			m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&mac, sizeof(mac));
		}
	}

	this->SendYaoShiInfo();

	return true;
}

bool YaoShiManager::ActiveSpecialImage(int imageid, bool is_notify)
{
	if (m_yaoshi_param.IsSpecialImageActive(imageid))
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_SPECIAL_IMG_ALREADY_ACTIVE);
		}

		return false;
	}

	const YaoShiSpecialImgCfg *cfg_item = YAOSHI_CFG().GetSpecialImgCfg(imageid);
	if (nullptr == cfg_item)
	{
		return false;
	}

	m_yaoshi_param.ActiveSpecialImage(imageid);
	if (0 == m_yaoshi_param.special_img_grade_list[imageid])
	{
		m_yaoshi_param.special_img_grade_list[imageid] = 1;
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_YAOSHI);
	JinjiesysFuncHelper::Instance().OnActiveSpecialImg(m_role, JINJIE_TYPE_YAOSHI, imageid);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_YAOSHI, imageid, true);
	this->UseImage(SPECIAL_IMG_START_ID + imageid);
	return true;
}

bool YaoShiManager::UnActiveSpecialImage(int imageid)
{
	if (!m_yaoshi_param.IsSpecialImageActive(imageid))
	{
		return false;
	}

	const YaoShiSpecialImgCfg *cfg_item = YAOSHI_CFG().GetSpecialImgCfg(imageid);
	if (nullptr == cfg_item)
	{
		return false;
	}

	BitmapFunction::ClearBit(m_yaoshi_param.active_special_image_flag, sizeof(m_yaoshi_param.active_special_image_flag), imageid);
	if (1 == m_yaoshi_param.special_img_grade_list[imageid])
	{
		m_yaoshi_param.special_img_grade_list[imageid] = 0;
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_YAOSHI);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_YAOSHI, imageid, false);

	if (this->UnUseImage(SPECIAL_IMG_START_ID + imageid))
	{
		this->UseImage(1); //默认再给一个
	}

	return true;
}

void YaoShiManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();
		m_other_capability = 0;

		double rate = 0.0;										//所有属性加成万分比

		CharIntAttrs base_attrs;
		base_attrs.Reset();
		CharIntAttrs other_per_add_attr;
		{
			// 等级加成
			const YaoShiLevelConfig *level_cfg = YAOSHI_CFG().GetLevelConfig(m_yaoshi_param.level);
			if (nullptr != level_cfg)
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
			const YaoShiGradeConfig * grade_config = YAOSHI_CFG().GetGradeConfig(m_yaoshi_param.grade);
			if (nullptr != grade_config) 
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
				int special_img_per_add = JinjieSysRewardMgr::Instance().GetJinjieSpecialImgExtraAddPer(m_role, JINJIE_TYPE_YAOSHI);
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
				//if (m_yaoshi_param.grade_bless_val > 0)
				//{
				//	const YaoShiGradeConfig * next_grade_config = YAOSHI_CFG().GetGradeConfig(m_yaoshi_param.grade + 1);
				//	const YaoShiGradeConfig * curr_grade_cfg = YAOSHI_CFG().GetGradeConfig(m_yaoshi_param.grade);
				//	if (nullptr != next_grade_config && nullptr != curr_grade_cfg && curr_grade_cfg->bless_val_limit > 0) 
				//	{
				//		double rate = (((double)m_yaoshi_param.grade_bless_val) / curr_grade_cfg->bless_val_limit) * (next_grade_config->bless_addition * ROLE_ATTR_PER);
				//		//base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)((next_grade_config->maxhp - grade_config->maxhp) * rate);
				//		base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)((next_grade_config->gongji - grade_config->gongji) * rate);
				//		//base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)((next_grade_config->fangyu - grade_config->fangyu) * rate);
				//		//base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (Attribute)((next_grade_config->mingzhong - grade_config->mingzhong) * rate);
				//		//base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (Attribute)((next_grade_config->shanbi - grade_config->shanbi) * rate);
				//		//base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (Attribute)((next_grade_config->baoji - grade_config->baoji) * rate);
				//		//base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (Attribute)((next_grade_config->jianren - grade_config->jianren) * rate);
				//	}
				//}
				base_attrs.Add(grade_attrs.m_attrs);

				//天赋加成
				m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_YAOSHI_BASE, grade_attrs, other_per_add_attr);
			}
		}

		m_attrs_add.Add(base_attrs.m_attrs);

		{
			// 腰饰装备
			int active_equip_grade = YAOSHI_CFG().GetActiveEquipGrade();
			if (active_equip_grade <= m_yaoshi_param.grade)
			{
				for (int i = 0; i < YaoShiParam::EQUIP_COUNT; ++ i)
				{
					const YaoShiEquipConifg *equip_cfg = YAOSHI_CFG().GetEquipCfg(i);
					if (nullptr != equip_cfg)
					{
						int level = m_yaoshi_param.equip_level_list[i];
						if (level >= 0 && level < equip_cfg->max_level)
						{
							rate += equip_cfg->level_cfg[level].add_percent;
						}
					}
				}
			}

			//{
			//	// 至宝加成
			//	rate += m_role->GetZhiBaoManager()->GetYaoShiAddPer();

			//	// 勋章加成
			//	rate += m_role->GetXunZhangManager()->GetYaoShiAddPer();

			//	// 星座星魂加成
			//	int xinhun_add_per = 0;
			//	m_role->GetChineseZodiac()->GetXingHunTotalAddPer(&xinhun_add_per, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
			//	rate += xinhun_add_per;

			//}

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

			if (active_equip_grade <= m_yaoshi_param.grade)
			{
				for (int i = 0; i < YaoShiParam::EQUIP_COUNT; ++ i)
				{
					const YaoShiEquipConifg *equip_cfg = YAOSHI_CFG().GetEquipCfg(i);
					if (nullptr != equip_cfg)
					{
						int level = m_yaoshi_param.equip_level_list[i];
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
			for (int i = 0; i < YaoShiParam::MAX_SPECIAL_IMAGE_COUNT; ++ i)
			{
				if (m_yaoshi_param.IsSpecialImageActive(i))
				{
	
					// 特殊形象进阶
					const int grade = m_yaoshi_param.special_img_grade_list[i];

					const YaoShiSpecialImgUpgradeCfg *upgrade_cfg = YAOSHI_CFG().GetSpecialImgUpgradeCfg(i, grade);
					if (nullptr != upgrade_cfg)
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

		// 属性丹
		{
			int shuxingdan_attr_per = 0;
			for (int slot_index = 0; slot_index < SHUXINGDAN_MAX_TYPE; slot_index++)
			{
				const int num = m_yaoshi_param.shuxingdan_list[slot_index];
				if (num > 0)
				{
					const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_YAOSHI, slot_index);
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
			for (int i = 0; i < YaoShiParam::SKILL_COUNT; i++)
			{
				if (m_yaoshi_param.skill_level_list[i] > 0)
				{
					const YaoShiSkillConifg *skill_cfg = YAOSHI_CFG().GetSkillCfg(i, m_yaoshi_param.skill_level_list[i]);
					if (nullptr != skill_cfg)
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
		{
			const YaoShiOtherConfig &other_cfg = LOGIC_CONFIG->GetYaoShiConfig().GetOtherCfg();
			if (m_yaoshi_param.grade >= other_cfg.extra_attrs_per_grade && other_cfg.extra_attrs_per > 0)
			{
				for (int i = CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX; ++i)
				{
					//m_attrs_add.m_attrs[i] += static_cast<Attribute>(m_attrs_add.m_attrs[i] * (ROLE_ATTR_PER * other_cfg.extra_attrs_per));
					m_attrs_add.AddValue(i, static_cast<Attribute>(m_attrs_add.m_attrs[i] * (ROLE_ATTR_PER * other_cfg.extra_attrs_per)));
				}
			}
		}
		// 其他百分比属性加成，与上面的全属性加成加算
		{
			m_attrs_add.AddAttrs(&other_per_add_attr);
		}
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_YAOSHI, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_YAOSHI, m_other_capability);

	m_yaoshi_param.capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_YAOSHI);

	base_add.Add(m_attrs_add.m_attrs, true);
}


YaoShiParam * YaoShiManager::GetYaoShiParam()
{
	return &m_yaoshi_param;
}

void YaoShiManager::PrintYaoShi(const char *reason, int param1, int param2, int param3, int param4, int param5)
{
	UNSTD_STATIC_CHECK(4 == YaoShiParam::EQUIP_COUNT);
	UNSTD_STATIC_CHECK(4 == YaoShiParam::SKILL_COUNT);
	const int image_hex_len = YaoShiParam::MAX_ACTIVE_IMAGE_BYTE * 2 + 1;
	const int special_image_hex_len = YaoShiParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE * 2 + 1;
	char active_image_hex[image_hex_len] = { 0 };
	char active_special_image_hex[special_image_hex_len] = { 0 };
	BinToHex((const char*)m_yaoshi_param.active_image_flag, sizeof(m_yaoshi_param.active_image_flag), active_image_hex);
	BinToHex((const char*)m_yaoshi_param.active_special_image_flag, sizeof(m_yaoshi_param.active_special_image_flag), active_special_image_hex);
	gamelog::g_log_yaoshi.printf(LL_INFO, "YaoShi role(%d,%s) %s(%d,%d,%d,%d,%d) "
		"flag:%d, level:%d, grade:%d,%d, used_imageid:%d shuxingdan_count:%d chengzhangdan_count:%d, image_flag:%s, special_image_flag:%s"
		"equip_level_list:%d, %d, %d, %d"
		"skill_level_list:%d, %d, %d, %d",
		m_role->GetUID(), m_role->GetName(),
		reason, param1, param2, param3, param4, param5,
		(int)m_yaoshi_param.yaoshi_flag, (int)m_yaoshi_param.level,
		(int)m_yaoshi_param.grade, (int)m_yaoshi_param.grade_bless_val,
		(int)m_yaoshi_param.used_imageid,
		(int)m_yaoshi_param.shuxingdan_list[0], (int)m_yaoshi_param.shuxingdan_list[1],
		active_image_hex, active_special_image_hex,
		(int)m_yaoshi_param.equip_level_list[0], (int)m_yaoshi_param.equip_level_list[1], (int)m_yaoshi_param.equip_level_list[2], (int)m_yaoshi_param.equip_level_list[3],
		(int)m_yaoshi_param.skill_level_list[0], (int)m_yaoshi_param.skill_level_list[1], (int)m_yaoshi_param.skill_level_list[2], (int)m_yaoshi_param.skill_level_list[3]);
}

void YaoShiManager::OnCompleteTask(TaskID task_id)
{
	this->CheckActive();
}

void YaoShiManager::OnRoleUpLevel(int level)
{
	this->CheckActive();
}


bool YaoShiManager::OnUseShuxingdan(int slot_idx, short &use_count)
{
	if (slot_idx < 0 || slot_idx >= SHUXINGDAN_MAX_TYPE || use_count <= 0)
	{
		return false;
	}

	const YaoShiGradeConfig *grade_cfg = YAOSHI_CFG().GetGradeConfig(m_yaoshi_param.grade);
	if (nullptr == grade_cfg)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_YAOSHI, slot_idx);
	if (nullptr == cfg_item)
	{
		return false;
	}

	if (m_yaoshi_param.grade < cfg_item->order_limit)
	{
		gstr::SendError(m_role, "shuxingdan_use_order_limit_1", SHUXINGDAN_TYPE_YAOSHI, cfg_item->order_limit - 1, slot_idx); // 客户端阶数-1
		return false;
	}

	if (m_yaoshi_param.shuxingdan_list[slot_idx] + use_count > cfg_item->use_limit_num)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	int shuxingdan_limit = (slot_idx == SHUXINGDAN_SLOT_TYPE_ZIZHI) ? grade_cfg->shuxingdan_limit : grade_cfg->chengzhangdan_limit;
	int special_img_add_count = 0;
	for (int i = 0;i < YaoShiParam::MAX_SPECIAL_IMAGE_COUNT; i++)
	{
		if (!m_yaoshi_param.IsSpecialImageActive(i))
			continue;
		
		const YaoShiSpecialImgUpgradeCfg *special_img_grade_cfg = YAOSHI_CFG().GetSpecialImgUpgradeCfg(i,m_yaoshi_param.special_img_grade_list[i]);
		if (nullptr != special_img_grade_cfg)
		{
			int shuxingdan_add_count = (slot_idx == SHUXINGDAN_SLOT_TYPE_ZIZHI) ? special_img_grade_cfg->shuxingdan_count : special_img_grade_cfg->chengzhangdan_count;
			special_img_add_count += shuxingdan_add_count;
		}
	}

	// 策划需求，修正真正使用数量
	int can_use_count = shuxingdan_limit + special_img_add_count - m_yaoshi_param.shuxingdan_list[slot_idx];
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

	m_yaoshi_param.shuxingdan_list[slot_idx] += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_YAOSHI);
	this->SendYaoShiInfo();

	return true;
}

int YaoShiManager::RefreshYaoShiMinEquipLevel()
{
	int min = m_yaoshi_param.equip_level_list[0];

	for (int i = 1; i < YaoShiParam::EQUIP_COUNT; ++i)
	{
		min = m_yaoshi_param.equip_level_list[i] < min ? m_yaoshi_param.equip_level_list[i] : min;
	}

	return min;
}

// 幻化特殊战力加成
void YaoShiManager::HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add)
{
	if (!LOGIC_CONFIG->GetYaoShiConfig().IsAddSpecialCap())
	{
		return;
	}

	auto cfg_item = LOGIC_CONFIG->GetYaoShiConfig().GetHuanHuaSpecialCapAddCfg(huanhua_id);
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

int YaoShiManager::GetSpecialImageLevelById(int imageid)
{
	if (imageid < 0 || imageid >= YaoShiParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return 0;
	}

	if (!m_yaoshi_param.IsSpecialImageActive(imageid)) return 0;

	return m_yaoshi_param.special_img_grade_list[imageid];
}