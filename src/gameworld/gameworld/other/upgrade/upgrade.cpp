#include "upgrade.hpp"

#include "gamelog.h"
#include "world.h"
#include "gstr.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "protocal/msgupgrade.hpp"
#include "scene/scene.h"

#include "config/logicconfigmanager.hpp"
#include "config/upgradeconfig/upgradeconfigmanager.hpp"
#include "config/upgradeconfig/upgradeconfig.hpp"
#include "config/taskconfig/husongconfig.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/shuxingdanconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"

#include "servercommon/serverconfig/crossconfig.hpp"
#include "other/mount/mountfunction.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "serverlogic.h"

#include "task/taskrecorder.h"
#include "other/capability/capability.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/event/eventhandler.hpp"
#include "other/upgrade/upgrademanager.hpp"
#include "other/upgrade/upgradeskill.hpp"
#include "other/upgradesyscrit/upgradesyscrit.hpp"

#include "other/sublock/sublock.hpp"

#include "item/knapsack.h"
#include "item/itempool.h"
#include "other/roleactivity/roleactivity.hpp"
#include "other/talent/talent.hpp"
#include "servercommon/activitydef.hpp"
#include "other/jinjiesysreward/jinjiesysrewardmgr.hpp"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"


Upgrade::Upgrade(UpgradeManager *upgrade_manager, int upgrade_type)
	:m_upgrade_manager(upgrade_manager), m_upgrade_type(upgrade_type)
{
}

Upgrade::~Upgrade()
{

}

void Upgrade::Init(Role *role, const UpgradeBaseData &upgrade_base_data)
{
	m_role = role;
	m_base_data = upgrade_base_data;
}

void Upgrade::GetInitParam(UpgradeBaseData *upgrade_base_data)
{
	*upgrade_base_data = m_base_data;
}

void Upgrade::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->CheckActiveSys();
	}
}

void Upgrade::Update(time_t now_second)
{
	if (0 != m_base_data.clear_upgrade_time && now_second > m_base_data.clear_upgrade_time)
	{
		m_base_data.grade_bless_val = 0;
		m_base_data.clear_upgrade_time = 0;

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_UPGRADE_SYS_BEGIN + m_upgrade_type);
		this->SendUpgradeInfo();
	}

	if (0 != m_base_data.temporary_imageid_invalid_time && static_cast<unsigned int>(now_second) > m_base_data.temporary_imageid_invalid_time)
	{
		if (0 != m_base_data.temporary_imageid_has_select)
		{
			m_base_data.temporary_imageid = 0;
			m_base_data.temporary_imageid_has_select = 0;
			this->SendUpgradeInfo();
			this->OnUpgradeAppeChange();
		}
	}
}

void Upgrade::OnLogin()
{
	this->CheckActiveSys();
}

void Upgrade::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();
		m_other_capability = 0;

		double rate = 0.0;										//所有属性加成万分比

		CharIntAttrs base_attrs;
		base_attrs.Reset();

		CharIntAttrs other_per_add_attr;						//其他系统加成属性

		const auto Upgrade_Cfg = UPGRADE_CFG_MGR(m_upgrade_type);
		if (!Upgrade_Cfg)
		{
			return;
		}

		// 基础属性
		this->GetBaseAttr(base_attrs);

		m_attrs_add.Add(base_attrs.m_attrs);

		// 大目标特殊形象万分比
		int jinjie_type = JinjiesysFuncHelper::Instance().UpgradeManagerTypeToJinJieSysType(m_upgrade_type);
		if (JINJIE_TYPE_INVALID != jinjie_type)
		{
			int special_img_per_add = JinjieSysRewardMgr::Instance().GetJinjieSpecialImgExtraAddPer(m_role, jinjie_type);
			if (special_img_per_add > 0)
			{
				other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (ROLE_ATTR_PER * special_img_per_add));
				other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (ROLE_ATTR_PER * special_img_per_add));
				other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (ROLE_ATTR_PER * special_img_per_add));
				other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (ROLE_ATTR_PER * special_img_per_add));
				other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (ROLE_ATTR_PER * special_img_per_add));
				other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (ROLE_ATTR_PER * special_img_per_add));
				other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (ROLE_ATTR_PER * special_img_per_add));
			}
		}
		
		//天赋加成
		m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_UPGRADE_0 + m_upgrade_type, base_attrs, other_per_add_attr);

		this->CalcBaseAttrAddFromSkill(m_attrs_add);

		{
			// 装备
			int active_equip_grade = Upgrade_Cfg->GetActiveEquipGrade();
			if (active_equip_grade <= m_base_data.grade)
			{
				for (int i = 0; i < UPGRADE_SKILL_COUNT; ++i)
				{
					const EquipConifg *equip_cfg = Upgrade_Cfg->GetEquipCfg(i);
					if (nullptr != equip_cfg)
					{
						int level = m_base_data.equip_level_list[i];
						if (level >= 0 && level < equip_cfg->max_level)
						{
							rate += equip_cfg->level_cfg[level].add_percent;
						}
					}
				}
			}

			rate *= ROLE_ATTR_PER;
			if (rate > 0)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * rate);
			}

			if (active_equip_grade <= m_base_data.grade)
			{
				for (int i = 0; i < UPGRADE_SKILL_COUNT; ++i)
				{
					const EquipConifg *equip_cfg = Upgrade_Cfg->GetEquipCfg(i);
					if (nullptr != equip_cfg)
					{
						int level = m_base_data.equip_level_list[i];
						if (level >= 0 && level < equip_cfg->max_level)
						{
							m_attrs_add.AddAttrFromConfig(equip_cfg->level_cfg[level].attr_cfg);
						}
					}
				}
			}
		}

		{
			// 形象进阶
			int add_percent = 0;
			for (int i = 1; i < UPGRADE_IMAGE_MAX_COUNT; ++i)
			{
				if (m_base_data.IsImageActive(i))
				{
					const int grade = m_base_data.img_grade_list[i];
					const ImgUpgradeCfg *img_upgrade_cfg = Upgrade_Cfg->GetImgUpgradeCfg(i, grade);
					if (nullptr != img_upgrade_cfg)
					{
						m_attrs_add.AddAttrFromConfig(img_upgrade_cfg->attr_cfg);

						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += img_upgrade_cfg->extra_zengshang;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += img_upgrade_cfg->extra_mianshang;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += img_upgrade_cfg->per_jingzhun;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += img_upgrade_cfg->per_baoji;
// 						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += img_upgrade_cfg->jianshang_per;
// 						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += img_upgrade_cfg->zengshang_per;
// 
// 						m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += img_upgrade_cfg->pvp_jianshang_per;
// 						m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += img_upgrade_cfg->pvp_zengshang_per;
// 						m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += img_upgrade_cfg->pve_jianshang_per;
// 						m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += img_upgrade_cfg->pve_zengshang_per;

						// 下面特殊属性用乘法
						m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, img_upgrade_cfg->jianshang_per);
						m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, img_upgrade_cfg->zengshang_per);
						m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, img_upgrade_cfg->pvp_zengshang_per);
						m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, img_upgrade_cfg->pve_zengshang_per);
						m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, img_upgrade_cfg->pvp_jianshang_per);
						m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, img_upgrade_cfg->pve_jianshang_per);

						add_percent += img_upgrade_cfg->add_per;
					}
					this->SuperSpecialCapAdd(i, grade, m_attrs_add);		//超强战力加成
				}
			}

			// 万分比加成
			for (int i = CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX; ++i)
			{
//				m_attrs_add.m_attrs[i] += static_cast<Attribute>(base_attrs.m_attrs[i] * (ROLE_ATTR_PER * add_percent));
				m_attrs_add.AddValue(i, static_cast<Attribute>(base_attrs.m_attrs[i] * (ROLE_ATTR_PER * add_percent)));
			}
		}

		// 属性丹
		{
			int shuxingdan_attr_per = 0;
			for (int slot_index = 0; slot_index < SHUXINGDAN_MAX_TYPE; slot_index++)
			{
				const int num = m_base_data.shuxingdan_list[slot_index];
				if (num > 0)
				{
					const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_UPGRADE_BEGIN + m_upgrade_type, slot_index);
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
//				m_attrs_add.m_attrs[i] += static_cast<Attribute>(base_attrs.m_attrs[i] * (ROLE_ATTR_PER * shuxingdan_attr_per));
				m_attrs_add.AddValue(i, static_cast<Attribute>(base_attrs.m_attrs[i] * (ROLE_ATTR_PER * shuxingdan_attr_per)) );
			}
		}

		{
			// 技能属性
			for (int i = 0; i < UPGRADE_SKILL_COUNT; i++)
			{
				if (m_base_data.skill_level_list[i] > 0)
				{
					const SkillConifg *skill_cfg = Upgrade_Cfg->GetSkillCfg(i, m_base_data.skill_level_list[i]);
					if (nullptr != skill_cfg)
					{
						m_attrs_add.AddAttrFromConfig(skill_cfg->attr_cfg);

						this->OnAddSkillAttr(m_attrs_add, skill_cfg);

						m_other_capability += skill_cfg->capability;
					}
				}
			}
		}

		// 阶数百分比加成
		{
			const UpgradeOtherConfig &other_cfg = Upgrade_Cfg->GetOtherCfg();
			if (m_base_data.grade >= other_cfg.extra_attrs_per_grade && other_cfg.extra_attrs_per > 0)
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

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + m_upgrade_type, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + m_upgrade_type, m_other_capability);

	m_base_data.capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_UPGRADE_SYS_BEGIN + m_upgrade_type);

	base_add.Add(m_attrs_add.m_attrs, true);
}

bool Upgrade::OnFightOut(bool is_fight_out, bool notify, bool is_force)
{
	if (Role::MOVE_MODE_FLY == m_role->GetMoveMode())
	{
		if (notify)
		{
			gstr::SendError(m_role, "opeara_limit_in_cur_status");
		}

		return false;
	}

	bool old_is_mount = this->IsFightOut();

	if (is_fight_out) // 出战
	{
		if (old_is_mount)
		{
			return false;
		}

		if (!is_force && !m_role->GetRoleStatusManager()->CanMountOn())
		{
			return false;
		}

		m_base_data.OnFightOut();
	}
	else // 不出战
	{
		if (!old_is_mount)
		{
			return false;
		}

		m_base_data.OffFightOut();
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_UPGRADE_SYS_BEGIN + m_upgrade_type);

	this->SendUpgradeInfo();
	this->OnUpgradeAppeChange();

	return true;
}

bool Upgrade::IsFightOut()
{
	return m_base_data.IsFightOut();
}

int Upgrade::GetUpgradeAppeID()
{
	return m_base_data.temporary_imageid > 0 ? m_base_data.temporary_imageid : m_base_data.used_imageid;
}

UpgradeBaseData * Upgrade::GetBaseData()
{
	return &m_base_data;
}

bool Upgrade::CheckCanOperate()
{
	if (m_role->GetSublock()->CheckHasLock())
	{
		return false;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!m_base_data.IsSysActive())
	{
		gstr::SendError(m_role, "upgrade_sys_not_active", m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN);
		return false;
	}

	return true;
}

bool Upgrade::OnConsumeUpgradeStuff(ItemID stuff_id, ItemID stuff2_id, int total_consume_count, bool is_auto_buy, std::string reason)
{
	short bag_stuff2_num = m_role->GetKnapsack()->Count(stuff2_id);
	short need_stuff2_num = bag_stuff2_num >= total_consume_count ? total_consume_count : bag_stuff2_num;
	short need_stuff_num = bag_stuff2_num < total_consume_count ? total_consume_count - bag_stuff2_num : 0;

	// 消耗
	ItemExtern::ItemConsumeConfigList stuff_list;

	if (need_stuff2_num > 0)
	{
		stuff_list.add_item(ItemExtern::ItemConsumeConfig(stuff2_id, is_auto_buy, need_stuff2_num));
	}

	if (need_stuff_num > 0)
	{
		stuff_list.add_item(ItemExtern::ItemConsumeConfig(stuff_id, is_auto_buy, need_stuff_num));
	}

	std::string consume_reason = "UpgradeType:" + std::to_string(m_upgrade_type) + reason;
	if (!stuff_list.done(*m_role, consume_reason.c_str()))
	{
		return false;
	}

	return true;
}

bool Upgrade::OnUpgrade(bool is_auto_buy, int repeat_times)
{
	if (!CheckCanOperate())
	{
		return false;
	}

	const auto upgrade_cfg = UPGRADE_CFG_MGR(m_upgrade_type);
	if (!upgrade_cfg)
	{
		return false;
	}

	const GradeConfig *curr_grade_cfg = upgrade_cfg->GetGradeConfig(m_base_data.grade);
	if (nullptr == curr_grade_cfg)
	{
		gstr::SendError(m_role, "upgrade_sys_upgrade_max", m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN);
		return false;
	}

	const GradeConfig *target_grade_cfg = upgrade_cfg->GetGradeConfig(m_base_data.grade + 1);
	if (nullptr == target_grade_cfg)
	{
		gstr::SendError(m_role, "upgrade_sys_upgrade_max", m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN);
		return false;
	}

	short old_grade = m_base_data.grade;
	short oldest_bless_val = m_base_data.grade_bless_val;
	bool succeeded = false;

	for (int i = 0; i < repeat_times; ++i)
	{
		short old_bless_val = m_base_data.grade_bless_val;
		const UpGradeProbConfig *upgrade_prob_cfg = upgrade_cfg->GetUpGradeProbConfig(old_grade, old_bless_val);
		if (nullptr == upgrade_prob_cfg)
		{
			gstr::SendError(m_role, "upgrade_sys_upgrade_max", m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN);
			//return false;
			succeeded = true;
			break;
		}

		//检查、消耗
		if (!OnConsumeUpgradeStuff(curr_grade_cfg->upgrade_stuff_id, curr_grade_cfg->upgrade_stuff2_id, curr_grade_cfg->upgrade_stuff_count, is_auto_buy, "::OnUpgrade"))
		{
			//return false;
			succeeded = true;
			break;
		}

		// 生效
		int rand_val = RandomNum(10000);
		
		if (rand_val < upgrade_prob_cfg->succ_rate || m_base_data.grade_bless_val >= curr_grade_cfg->bless_val_limit)
		{
			succeeded = true;
		}

		if (!succeeded)
		{
			// 记录进阶第一次增加祝福值的时间，用于判断清0祝福值,大于X阶时使用
			if (1 == curr_grade_cfg->is_clear_bless && 0 == m_base_data.clear_upgrade_time)
			{
				unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_base_data.clear_upgrade_time = now_sec + EngineAdapter::Instance().NextDayInterval(NEXTDAY_CLEAR_BLESS_VAL_TIME, 0, 0);
			}

			int crit_value = UpgradeSysCrit::Instance().CalculateCritValue(m_role, this->GetUpgradeCritType());
			m_base_data.grade_bless_val += upgrade_prob_cfg->upgrade_lucky_cfg.Random() * crit_value;

			if (m_base_data.grade_bless_val > curr_grade_cfg->bless_val_limit)
			{
				m_base_data.grade_bless_val = curr_grade_cfg->bless_val_limit;
			}

			//m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_UPGRADE_SYS_BEGIN + m_upgrade_type);
		}
		else
		{
			m_base_data.grade++;
			m_base_data.grade_bless_val = 0;
			m_base_data.clear_upgrade_time = 0;

			const SkillConifg *skill_cfg = upgrade_cfg->GetSkillCfg(0, m_base_data.skill_level_list[0] + 1);
			if (nullptr != skill_cfg && m_base_data.grade >= skill_cfg->grade)
			{
				m_base_data.skill_level_list[0]++;
				m_upgrade_manager->GetUpgradeSkill().RecalcActivedSkill();
			}

			ActiveImage(target_grade_cfg->active_image_id, false);
			UseImage(target_grade_cfg->active_image_id);

			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_UPGRADE_SYS_BEGIN + m_upgrade_type);

			EventHandler::Instance().OnUpgradeSysUpgrade(m_role, m_upgrade_type, old_grade, m_base_data.grade);

			if (target_grade_cfg->is_broadcast)
			{
				int sendlen = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "upgrade_sys_upgrade_notice", UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN, target_grade_cfg->client_grade, m_upgrade_type, target_grade_cfg->grade, this->GetMedalSkillLevel(), 200 + m_upgrade_type);
				if (sendlen > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
				}
			}
			break;
		}
	}
	SendUpgradeInfo();
	PrintUpgradeLog("Upgrade::OnUpgrade", old_grade, oldest_bless_val, m_base_data.grade, m_base_data.grade_bless_val, m_base_data.skill_level_list[0]);

	return !succeeded;
}

int Upgrade::GetUpgradeCritType()
{
	switch (m_upgrade_type)
	{
	case UPGRADE_TYPE_2: 
	{
		return UPGRADE_SYS_CRIT_TYPE_LINGTONG;
	}
	break;
	case UPGRADE_TYPE_3: 
	{
		return UPGRADE_SYS_CRIT_TYPE_LINGGONG;
	}
	break;

	case UPGRADE_TYPE_8:
	{
		return UPGRADE_SYS_CRIT_TYPE_FLYPET;
	}
	break;

	case UPGRADE_TYPE_5:
	{
		return UPGRADE_SYS_CRIT_TYPE_WEIYAN;
	}
	break;

	default: return UPGRADE_SYS_CRIT_TYPE_INVALID;
	}
}

bool Upgrade::CheckCanUpOneGrade(int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num)
{
	if (!m_base_data.IsSysActive())
	{
		gstr::SendError(m_role, "upgrade_sys_not_active", m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN);
		return false;
	}

	if (m_base_data.grade < valid_min_grade)
	{
		gstr::SendError(m_role, "upgrade_sys_upgrade_max", m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN);
		return false;
	}
	else if (m_base_data.grade > valid_max_grade)
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(return_item_id);
		if (nullptr != item_base)
		{
			for (int loop_time = 0; return_item_num > 0 && loop_time < 1000; ++loop_time)
			{
				int put_num = return_item_num > item_base->GetPileLimit() ? item_base->GetPileLimit() : return_item_num;
				return_item_num -= put_num;

				ItemConfigData return_item(return_item_id, true, put_num);
				m_role->GetKnapsack()->PutOrMail(return_item, PUT_REASON_UPGRADE_ONE_RETURN + m_upgrade_type);
			}

			return true;
		}

		return false;
	}

	if (!OnUpOneGrade())
	{
		return false;
	}

	return true;
}

bool Upgrade::OnUpOneGrade()
{
	int old_grade = m_base_data.grade;
	int old_bless_val = m_base_data.grade_bless_val;

	const auto upgrade_cfg = UPGRADE_CFG_MGR(m_upgrade_type);
	if (!upgrade_cfg)
	{
		return false;
	}

	const GradeConfig *curr_grade_cfg = upgrade_cfg->GetGradeConfig(m_base_data.grade);
	if (nullptr == curr_grade_cfg)
	{
		gstr::SendError(m_role, "upgrade_sys_upgrade_max", m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN);
		return false;
	}

	const GradeConfig *target_grade_cfg = upgrade_cfg->GetGradeConfig(m_base_data.grade + 1);
	if (nullptr == target_grade_cfg)
	{
		gstr::SendError(m_role, "upgrade_sys_upgrade_max", m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN);
		return false;
	}

	{
		m_base_data.grade++;
		m_base_data.grade_bless_val = 0;
		m_base_data.clear_upgrade_time = 0;

		const SkillConifg *skill_cfg = upgrade_cfg->GetSkillCfg(0, m_base_data.skill_level_list[0] + 1);
		if (nullptr != skill_cfg && m_base_data.grade >= skill_cfg->grade)
		{
			m_base_data.skill_level_list[0]++;
			m_upgrade_manager->GetUpgradeSkill().RecalcActivedSkill();
		}

		ActiveImage(target_grade_cfg->active_image_id, false);
		UseImage(target_grade_cfg->active_image_id);

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_UPGRADE_SYS_BEGIN + m_upgrade_type); 

		if (target_grade_cfg->is_broadcast)
		{
			int sendlen = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "upgrade_sys_upgrade_notice", UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN, target_grade_cfg->client_grade, m_upgrade_type, target_grade_cfg->grade, this->GetMedalSkillLevel(), 200 + m_upgrade_type);
			if (sendlen > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	SendUpgradeInfo();

	EventHandler::Instance().OnUpgradeSysUpgrade(m_role, m_upgrade_type, old_grade, m_base_data.grade);

	PrintUpgradeLog("Upgrade::OnUpgrade", old_grade, old_bless_val, m_base_data.grade, m_base_data.grade_bless_val, m_base_data.skill_level_list[0]);

	return true;
}

bool Upgrade::ActiveImage(int imageid, bool recalc)
{
	if (imageid <= 0 || imageid >= UPGRADE_IMAGE_MAX_COUNT) return false;

	m_base_data.ActiveImage(imageid);

	if (recalc)
	{
		if (IsSpecialImg(imageid) && 0 == m_base_data.img_grade_list[imageid])
		{
			m_base_data.img_grade_list[imageid] = 1;
			this->UseImage(imageid);
		}

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_UPGRADE_SYS_BEGIN + m_upgrade_type);
		this->SendUpgradeInfo();
	}
	EventHandler::Instance().OnUpgradeSysActiveSpecialImage(m_role, m_upgrade_type, imageid);
	return true;
}

bool Upgrade::UnActiveImage(int imageid)
{
	if (imageid <= 0 || imageid >= UPGRADE_IMAGE_MAX_COUNT) return false;

	m_base_data.UnActiveImage(imageid);
	if (IsSpecialImg(imageid) && 1 == m_base_data.img_grade_list[imageid])
	{
		m_base_data.img_grade_list[imageid] = 0;
		this->UnUseImage(imageid);
	}
	SendUpgradeInfo();
	return true;
}

bool Upgrade::IsImageActive(int imageid)
{
	return m_base_data.IsImageActive(imageid);
}

bool Upgrade::UseImage(int imageid)
{
	if (!m_base_data.IsImageActive(imageid))
	{
		return false;
	}

	int old_used_imageid = m_base_data.used_imageid;
	m_base_data.used_imageid = imageid;

	if (0 != m_base_data.temporary_imageid)
	{
		m_base_data.temporary_imageid = 0;
	}

	// 刷新外观
	OnUpgradeAppeChange();

	SendUpgradeInfo();

	PrintUpgradeLog("Upgrade::UseImage", old_used_imageid, imageid);
	return true;
}

bool Upgrade::UnUseImage(int imageid)
{
	if (imageid != m_base_data.used_imageid)
	{
		return false;
	}

	m_base_data.used_imageid = 0;

	// 刷新外观
	OnUpgradeAppeChange();

	SendUpgradeInfo();

	PrintUpgradeLog("Upgrade::UnUseImage", imageid);

	return true;
}

bool Upgrade::UseTemporaryImage(int imageid)
{
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (0 != m_base_data.temporary_imageid_invalid_time && now_sec > m_base_data.temporary_imageid_invalid_time)
	{
		gstr::SendError(m_role, "upgrade_sys_temporary_image_time_limit", m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN, m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN);
		return false;
	}

	if (0 != imageid && 0 != m_base_data.temporary_imageid_has_select && imageid != m_base_data.temporary_imageid_has_select)
	{
		return false;
	}

	m_base_data.temporary_imageid = imageid;

	if (0 != imageid && 0 == m_base_data.temporary_imageid_has_select)
	{
		m_base_data.temporary_imageid_has_select = imageid;
	}

	const auto upgrade_cfg = UPGRADE_CFG_MGR(m_upgrade_type);
	if (!upgrade_cfg)
	{
		return false;
	}

	if (0 != imageid && 0 == m_base_data.temporary_imageid_invalid_time)
	{
		m_base_data.temporary_imageid_invalid_time = now_sec + upgrade_cfg->GetTemporaryImageValidTime();
	}

	if (m_base_data.IsFightOut())
	{
		OnUpgradeAppeChange();
	}

	SendUpgradeInfo();

	PrintUpgradeLog("Upgrade::UseTemporaryImage", m_base_data.temporary_imageid);

	return true;
}

bool Upgrade::CheckCanImgUpgrade(int img_id)
{
	if (img_id <= 0 || img_id >= UPGRADE_IMAGE_MAX_COUNT)
	{
		return false;
	}

	if (!CheckCanOperate())
	{
		return false;
	}

	if (!m_base_data.IsImageActive(img_id))
	{
		gstr::SendError(m_role, "upgrade_sys_image_not_active");
		return false;
	}

	const int old_grade = m_base_data.img_grade_list[img_id];
	if (old_grade >= UPGRADE_MAX_IMAGE_GRADE)
	{
		gstr::SendError(m_role, "upgrade_sys_image_upgrade_max", m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN);
		return false;
	}

	const auto upgrade_cfg = UPGRADE_CFG_MGR(m_upgrade_type);
	if (!upgrade_cfg)
	{
		return false;
	}

	const ImgUpgradeCfg *image_grade_cfg = upgrade_cfg->GetImgUpgradeCfg(img_id, old_grade + 1);
	if (nullptr == image_grade_cfg)
	{
		gstr::SendError(m_role, "upgrade_sys_image_upgrade_max", m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN);
		return false;
	}

	//检查、消耗
	if (!OnConsumeUpgradeStuff(image_grade_cfg->stuff_id, 0, image_grade_cfg->stuff_num, false, "::ImgUpgrade"))
	{
		return false;
	}

	// 生效
	this->OnImgUpgrade(img_id);

	return true;
}

bool Upgrade::OnImgUpgrade(int img_id)
{
	++m_base_data.img_grade_list[img_id];

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_UPGRADE_SYS_BEGIN + m_upgrade_type); 

	SendUpgradeInfo();

	PrintUpgradeLog("Upgrade::OnImgUpgrade", img_id, m_base_data.img_grade_list[img_id]); 

	return true;
}

void Upgrade::OnUpgradeAppeChange()
{
	Protocol::SCUpgradeAppeChange msg;
	msg.upgrade_type = m_upgrade_type;
	msg.objid = m_role->GetId();
	msg.upgrade_appeid = m_base_data.temporary_imageid > 0 ? m_base_data.temporary_imageid : m_base_data.used_imageid;

	if (nullptr != m_role->GetScene())
	{
		m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&msg, sizeof(msg));
	}

	m_role->RefreshApperance(false);
}

bool Upgrade::CheckCanUplevelEquip(int equip_idx)
{
	if (equip_idx < 0 || equip_idx >= UPGRADE_EQUIP_COUNT)
	{
		return false;
	}

	if (!CheckCanOperate())
	{
		return false;
	}

	const auto upgrade_cfg = UPGRADE_CFG_MGR(m_upgrade_type);
	if (!upgrade_cfg)
	{
		return false;
	}

	short old_level = m_base_data.equip_level_list[equip_idx];
	if (old_level < 0)
	{
		return false;
	}

	//装备升级最大级数由系统阶数决定
	const GradeConfig * cur_grade_cfg = upgrade_cfg->GetGradeConfig(m_base_data.grade);
	if (nullptr == cur_grade_cfg)
	{
		return false;
	}

	if (old_level >= cur_grade_cfg->equip_level_toplimit)
	{
		gstr::SendError(m_role, "upgrade_sys_equip_upgrade_max", m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN);
		return false;
	}

	if (m_base_data.grade < upgrade_cfg->GetActiveEquipGrade())
	{
		return false;
	}

	const EquipConifg *equip_cfg = upgrade_cfg->GetEquipCfg(equip_idx);
	if (nullptr == equip_cfg)
	{
		return false;
	}

	// 消耗
	if (!OnConsumeUpgradeStuff(equip_cfg->level_cfg[old_level].item.item_id, 0, equip_cfg->level_cfg[old_level].item.num, false, "::OnUplevelEquip"))
	{
		return false;
	}

	// 生效
	this->OnUplevelEquip(equip_idx);

	return true;
}

bool Upgrade::OnUplevelEquip(int equip_idx)
{
	m_base_data.equip_level_list[equip_idx]++;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_UPGRADE_SYS_BEGIN + m_upgrade_type); 
	SendUpgradeInfo();

	PrintUpgradeLog("Upgrade::OnUplevelEquip", equip_idx, m_base_data.equip_level_list[equip_idx]); 

	return true;
}

bool Upgrade::CheckCanUpLevelSkill(int skill_idx, bool is_auto_buy)
{
	if (skill_idx < 0 || skill_idx >= UPGRADE_SKILL_COUNT)
	{
		return false;
	}

	if (!CheckCanOperate())
	{
		return false;
	}

	const auto upgrade_cfg = UPGRADE_CFG_MGR(m_upgrade_type);
	if (!upgrade_cfg)
	{
		return false;
	}

	const SkillConifg *target_skill_cfg = upgrade_cfg->GetSkillCfg(skill_idx, m_base_data.skill_level_list[skill_idx] + 1);
	if (nullptr == target_skill_cfg)
	{
		gstr::SendError(m_role, "upgrade_sys_skill_upgrade_max", m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN);
		return false;
	}

	if (0 == target_skill_cfg->skill_type)
	{
		return false;
	}

	if (target_skill_cfg->grade > m_base_data.grade)
	{
		return false;
	}

	// 消耗
	if (!OnConsumeUpgradeStuff(target_skill_cfg->uplevel_stuff_id, 0, target_skill_cfg->uplevel_stuff_num, is_auto_buy, "::OnUplevelSkill"))
	{
		return false;
	}

	// 升级
	this->OnUplevelSkill(skill_idx);

	return true;
}

bool Upgrade::OnUplevelSkill(int skill_idx)
{
	int old_skill_level = m_base_data.skill_level_list[skill_idx];
	m_base_data.skill_level_list[skill_idx]++;

	m_upgrade_manager->GetUpgradeSkill().RecalcActivedSkill();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_UPGRADE_SYS_BEGIN + m_upgrade_type); 

	SendUpgradeInfo();
	PrintUpgradeLog("UplevelSkill", skill_idx, old_skill_level, m_base_data.skill_level_list[skill_idx]);

	return true;
}

void Upgrade::OnAddSkillAttr(CharIntAttrs &attrs, const SkillConifg *skill_cfg)
{
	if (NULL == skill_cfg)
	{
		return;
	}

	switch (m_upgrade_type)
	{
	case UPGRADE_TYPE_0:
	{
		// 灵珠 伤害减免提高param_a%
//		attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += skill_cfg->param_a;
		attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, skill_cfg->param_a);
	}
	break;
	case UPGRADE_TYPE_1:
	{
		// 仙宝 伤害加成提高param_a%
//		attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += skill_cfg->param_a;
		// 下面特殊属性用乘法
		attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, skill_cfg->param_a);
	}
	break;
	case UPGRADE_TYPE_3:
	{
		// 灵弓 暴击几率提高param_a%
		attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI] += skill_cfg->param_a;
	}
	break;
	case UPGRADE_TYPE_4:
	{
		// 灵骑 格挡几率提高param_a%
		attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_PER] += skill_cfg->param_a;
	}
	break;
	case UPGRADE_TYPE_5:
	{
		// 尾焰 闪避几率提高param_a%
		attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI_PER] += skill_cfg->param_a;
	}
	break;
	case UPGRADE_TYPE_6:
	{
		// 手环 技能增伤提高param_a%
		attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER] += skill_cfg->param_a;
	}
	break;
	case UPGRADE_TYPE_7:
	{
		// 尾巴 PVP伤害减免提高param_a%
//		attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += skill_cfg->param_a;
		attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, skill_cfg->param_a);
	}
	break;
	case UPGRADE_TYPE_8:
	{
		// 飞宠 PVP伤害加成提高param_a%
//		attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += skill_cfg->param_a;
		attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, skill_cfg->param_a);
	}
	break;

	default:
		break;
	}

	attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_SKILL_CAP_PER] += skill_cfg->skill_cap_per;
}

void Upgrade::OnCompleteTask(TaskID task_id)
{
	this->CheckActiveSys();
}

void Upgrade::OnRoleUpLevel(int level)
{
	this->CheckActiveSys();
}

void Upgrade::CheckActiveSys()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (m_base_data.IsSysActive())
	{
		return;
	}

	const UpgradeMgrCfg *mgr_cfg = LOGIC_CONFIG->GetUpgradeCfgManager().GetUpgradeMgrCfg(m_upgrade_type);
	if (nullptr != mgr_cfg)
	{
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

		if (m_role->GetTaskRecorder()->IsFinished(mgr_cfg->task_id) &&
			m_role->GetLevel() >= mgr_cfg->role_level && 
			opengame_day >= mgr_cfg->opengame_day)
		{
			ActiveSys();
		}
	}
}

bool Upgrade::ActiveSys(bool is_recal)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return false;
	}

	if (m_base_data.IsSysActive())
	{
		return false;
	}

	const auto upgrade_cfg = UPGRADE_CFG_MGR(m_upgrade_type);
	if (!upgrade_cfg)
	{
		return false;
	}

	m_base_data.grade = 1;

	const GradeConfig *grade_cfg = upgrade_cfg->GetGradeConfig(1);
	if (nullptr != grade_cfg)
	{
		ActiveImage(grade_cfg->active_image_id, false);
		UseImage(grade_cfg->active_image_id);
	}

	const SkillConifg *skill_cfg = upgrade_cfg->GetSkillCfg(0, 1);
	if (nullptr != skill_cfg)
	{
		if (m_base_data.grade >= skill_cfg->grade && (0 == m_base_data.skill_level_list[0]))
		{
			m_base_data.skill_level_list[0] = 1;
		}
	}

	if (is_recal)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_UPGRADE_SYS_BEGIN + m_upgrade_type);
	}

	SendUpgradeInfo();

	PrintUpgradeLog("Upgrade::ActiveSys", m_base_data.grade, m_base_data.skill_level_list[0]);
	EventHandler::Instance().OnUpgradeSysActive(m_role, m_upgrade_type, is_recal);
	return true;
}

bool Upgrade::OnUseShuxingdan(int slot_idx, short &use_count)
{
	if (slot_idx < 0 || slot_idx >= SHUXINGDAN_MAX_TYPE || use_count <= 0)
	{
		return false;
	}

	if (!m_base_data.IsSysActive())
	{
		gstr::SendError(m_role, "upgrade_sys_not_active", m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN);
		return false;
	}

	const auto upgrade_cfg = UPGRADE_CFG_MGR(m_upgrade_type);
	if (!upgrade_cfg)
	{
		return false;
	}

	const GradeConfig *grade_cfg = upgrade_cfg->GetGradeConfig(m_base_data.grade);
	if (nullptr == grade_cfg)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_UPGRADE_BEGIN + m_upgrade_type, slot_idx);
	if (nullptr == cfg_item)
	{
		return false;
	}

	if (m_base_data.grade < cfg_item->order_limit)
	{
		gstr::SendError(m_role, "shuxingdan_use_order_limit_1", m_upgrade_type + SHUXINGDAN_TYPE_UPGRADE_BEGIN, cfg_item->order_limit - 1, slot_idx); // 客户端阶数-1
		return false;
	}

	if (m_base_data.shuxingdan_list[slot_idx] + use_count > cfg_item->use_limit_num)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	int shuxingdan_limit = (slot_idx == SHUXINGDAN_SLOT_TYPE_ZIZHI) ? grade_cfg->shuxingdan_limit : grade_cfg->chengzhangdan_limit;
	int special_img_add_count = 0;
	for (int i = 1; i < UPGRADE_IMAGE_MAX_COUNT; i++)
	{
		if (!m_base_data.IsImageActive(i))
			continue;

		const ImgUpgradeCfg *img_grade_cfg = upgrade_cfg->GetImgUpgradeCfg(i, m_base_data.img_grade_list[i]);
		if (nullptr != img_grade_cfg)
		{
			int shuxingdan_add_count = (slot_idx == SHUXINGDAN_SLOT_TYPE_ZIZHI) ? img_grade_cfg->shuxingdan_count : img_grade_cfg->chengzhangdan_count;
			special_img_add_count += shuxingdan_add_count;
		}
	}

	// 修正真正使用数量
	int can_use_count = shuxingdan_limit + special_img_add_count - m_base_data.shuxingdan_list[slot_idx];
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

	m_base_data.shuxingdan_list[slot_idx] += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_UPGRADE_SYS_BEGIN + m_upgrade_type); 
	this->SendUpgradeInfo();

	return true;
}

void Upgrade::SendUpgradeInfo()
{
	Protocol::SCUpgradeInfo msg;
	msg.upgrade_type = m_upgrade_type;
	msg.level = m_base_data.level;
	msg.grade = m_base_data.grade;
	msg.grade_bless_val = m_base_data.grade_bless_val;
	msg.clear_upgrade_time = m_base_data.clear_upgrade_time;
	msg.used_imageid = m_base_data.used_imageid;
	msg.temporary_imageid = m_base_data.temporary_imageid;
	msg.temporary_imageid_has_select = m_base_data.temporary_imageid_has_select;
	msg.temporary_imageid_invalid_time = m_base_data.temporary_imageid_invalid_time;
	msg.star_level = m_base_data.star_level;

	UNSTD_STATIC_CHECK(sizeof(msg.shuxingdan_list) == sizeof(m_base_data.shuxingdan_list));
	memcpy(msg.shuxingdan_list, m_base_data.shuxingdan_list, sizeof(msg.shuxingdan_list));

	UNSTD_STATIC_CHECK(sizeof(msg.equip_level_list) == sizeof(m_base_data.equip_level_list));
	memcpy(msg.equip_level_list, m_base_data.equip_level_list, sizeof(msg.equip_level_list));

	UNSTD_STATIC_CHECK(sizeof(msg.skill_level_list) == sizeof(m_base_data.skill_level_list));
	memcpy(msg.skill_level_list, m_base_data.skill_level_list, sizeof(msg.skill_level_list));

	UNSTD_STATIC_CHECK(sizeof(msg.active_img_flag) == sizeof(m_base_data.active_img_flag));
	memcpy(msg.active_img_flag, m_base_data.active_img_flag, sizeof(msg.active_img_flag));

	UNSTD_STATIC_CHECK(sizeof(msg.img_grade_list) == sizeof(m_base_data.img_grade_list));
	memcpy(msg.img_grade_list, m_base_data.img_grade_list, sizeof(msg.img_grade_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&msg, sizeof(msg));
}

void Upgrade::PrintUpgradeLog(const char *reason, int param1 /*= 0*/, int param2 /*= 0*/, int param3 /*= 0*/, int param4 /*= 0*/, int param5 /*= 0*/)
{
	UNSTD_STATIC_CHECK(2 == SHUXINGDAN_MAX_TYPE);
	UNSTD_STATIC_CHECK(4 == UPGRADE_EQUIP_COUNT);
	UNSTD_STATIC_CHECK(4 == UPGRADE_SKILL_COUNT);

	gamelog::g_log_upgrade.printf(LL_INFO, "UpgradeType(%d, cap:%d) role(%d,%s) %s(%d,%d,%d,%d,%d) "
		"fightout_flag:%d, level:%d, grade:%d,%d, used_imageid:%d shuxingdan_count:%d chengzhangdan_count:%d"
		"equip_level_list:%d, %d, %d, %d"
		"skill_level_list:%d, %d, %d, %d",
		m_upgrade_type, m_base_data.capability, m_role->GetUID(), m_role->GetName(),
		reason, param1, param2, param3, param4, param5,
		(int)m_base_data.fight_out_flag, (int)m_base_data.level,
		(int)m_base_data.grade, (int)m_base_data.grade_bless_val,
		(int)m_base_data.used_imageid,
		(int)m_base_data.shuxingdan_list[0], (int)m_base_data.shuxingdan_list[1],
		(int)m_base_data.equip_level_list[0], (int)m_base_data.equip_level_list[1], (int)m_base_data.equip_level_list[2], (int)m_base_data.equip_level_list[3],
		(int)m_base_data.skill_level_list[0], (int)m_base_data.skill_level_list[1], (int)m_base_data.skill_level_list[2], (int)m_base_data.skill_level_list[3]);
}

void Upgrade::GetBaseAttr(CharIntAttrs &tmp_attr)
{
	const auto Upgrade_Cfg = UPGRADE_CFG_MGR(m_upgrade_type);
	if (!Upgrade_Cfg)
	{
		return;
	}

	{
		// 阶数加成
		const GradeConfig * grade_config = Upgrade_Cfg->GetGradeConfig(m_base_data.grade);
		if (nullptr != grade_config)
		{
			tmp_attr.AddAttrFromConfig(grade_config->attr_cfg);

			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += grade_config->extra_zengshang;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += grade_config->extra_mianshang;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += grade_config->per_jingzhun;
			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += grade_config->per_baoji;
// 			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += grade_config->jianshang_per;
// 			tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += grade_config->zengshang_per;
// 
// 			tmp_attr.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += grade_config->pvp_jianshang_per;
// 			tmp_attr.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += grade_config->pvp_zengshang_per;
// 			tmp_attr.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += grade_config->pve_jianshang_per;
// 			tmp_attr.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += grade_config->pve_zengshang_per;

			// 下面特殊属性用乘法
			tmp_attr.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, grade_config->jianshang_per);
			tmp_attr.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, grade_config->zengshang_per);
			tmp_attr.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, grade_config->pvp_zengshang_per);
			tmp_attr.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, grade_config->pve_zengshang_per);
			tmp_attr.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, grade_config->pvp_jianshang_per);
			tmp_attr.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, grade_config->pve_jianshang_per);

			// 祝福值加成
			/*if (m_base_data.grade_bless_val > 0)
			{
				const GradeConfig * next_grade_config = Upgrade_Cfg->GetGradeConfig(m_base_data.grade + 1);
				const GradeConfig * curr_grade_cfg = Upgrade_Cfg->GetGradeConfig(m_base_data.grade);
				if (nullptr != next_grade_config && nullptr != curr_grade_cfg && curr_grade_cfg->bless_val_limit > 0)
				{
					double rate = (((double)m_base_data.grade_bless_val) / curr_grade_cfg->bless_val_limit) * (next_grade_config->bless_addition * ROLE_ATTR_PER);
					tmp_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)((next_grade_config->attr_cfg.gongji - grade_config->attr_cfg.gongji) * rate);
				}
			}*/
		}
	}
}

bool Upgrade::IsSpecialImg(int img_id)
{
	const auto Upgrade_Cfg = UPGRADE_CFG_MGR(m_upgrade_type);
	if (!Upgrade_Cfg)
	{
		return false;
	}

	const ImgUpgradeCfg *image_grade_cfg = Upgrade_Cfg->GetImgUpgradeCfg(img_id, 0);
	if (nullptr == image_grade_cfg)
	{
		return false;
	}

	return true;
}

void Upgrade::SuperSpecialCapAdd(int image_id, int grade, CharIntAttrs &m_attrs_add)
{
	const auto Upgrade_Cfg = UPGRADE_CFG_MGR(m_upgrade_type);
	if (!Upgrade_Cfg)
	{
		return;
	}

	if (!Upgrade_Cfg->IsAddSpecialCap())
	{
		return;
	}

	auto cfg_item = Upgrade_Cfg->GetSuperSpecialCapAddCfg(image_id);
	if (nullptr == cfg_item)
	{
		return;
	}

	if (grade >= cfg_item->grade)
	{
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp;
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji;
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu;
	}
}

int Upgrade::GetImageLevel(int imageid)
{
	if (imageid <= 0 || imageid >= UPGRADE_IMAGE_MAX_COUNT) return 0;

	if (!m_base_data.IsImageActive(imageid)) return 0;

	return m_base_data.img_grade_list[imageid];
}

void Upgrade::CalcBaseAttrAddFromSkill(CharIntAttrs & base_add)
{
	const auto Upgrade_Cfg = UPGRADE_CFG_MGR(m_upgrade_type);
	if (!Upgrade_Cfg)
	{
		return;
	}

	// 阶数加成
	const GradeConfig * grade_config = Upgrade_Cfg->GetGradeConfig(m_base_data.grade);
	if (NULL == grade_config)
	{
		return;
	}

	int total_add_per = 0;
	for (int i = 0; i < UPGRADE_SKILL_COUNT; i++)
	{
		if (m_base_data.skill_level_list[i] <= 0)
		{
			continue;
		}
		const SkillConifg *skill_cfg = Upgrade_Cfg->GetSkillCfg(i, m_base_data.skill_level_list[i]);
		if (NULL == skill_cfg || skill_cfg->base_attr_add_per <= 0)
		{
			continue;
		}
		total_add_per += skill_cfg->base_attr_add_per;
	}
	if (total_add_per > 0)
	{
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(grade_config->attr_cfg.maxhp * (total_add_per * ROLE_ATTR_PER));
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(grade_config->attr_cfg.gongji * (total_add_per * ROLE_ATTR_PER));
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(grade_config->attr_cfg.fangyu * (total_add_per * ROLE_ATTR_PER));
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(grade_config->attr_cfg.mingzhong * (total_add_per * ROLE_ATTR_PER));
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(grade_config->attr_cfg.shanbi * (total_add_per * ROLE_ATTR_PER));
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(grade_config->attr_cfg.baoji * (total_add_per * ROLE_ATTR_PER));
		base_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(grade_config->attr_cfg.jianren * (total_add_per * ROLE_ATTR_PER));
	}
}

int Upgrade::GetMedalSkillLevel()
{
	const auto upgrade_cfg = UPGRADE_CFG_MGR(m_upgrade_type);
	if (!upgrade_cfg)
	{
		return 0;
	}
	short medal_skill_index = upgrade_cfg->GetMedalSkillIndex();
	if (medal_skill_index < 0 || medal_skill_index >= (short)static_array_size(m_base_data.skill_level_list))
	{
		return 0;
	}
	return m_base_data.skill_level_list[medal_skill_index];
}
