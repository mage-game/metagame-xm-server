#include "other/fabao/fabaomanager.hpp"
#include "other/fabao/fabaoconfig.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/gongchengzhanconfig.hpp"
#include "item/itemextern.hpp"
#include "servercommon/errornum.h"
#include "global/worldstatus/worldstatus.hpp"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"
#include "other/achieve/title/titleconfig.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "scene/scene.h"
#include "global/guild/guild.hpp"
#include "servercommon/string/crossstr.h"
#include "servercommon/string/gameworldstr.h"
#include "serverlogic.h"
#include "world.h"
#include "gstr.h"
#include "other/event/eventhandler.hpp"
#include "netmsg/protocol/protocol_110.hpp"
#include "config/joinlimitconfig.hpp"
#include "task/taskrecorder.h"
#include "config/shuxingdanconfig.hpp"
#include "other/upgraderush/upgraderushconfig.hpp"
#include "other/sublock/sublock.hpp"
#include "other/imgfuling/imgfuling.hpp"
#include "servercommon/imgfulingdef.hpp"
#include "other/upgradeskill/upgradeskillconfig.hpp"
#include "other/talent/talent.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "other/jinjiesysreward/jinjiesysrewardmgr.hpp"
#include "other/dressingroom/dressingroomconfig.hpp"
#include "other/dressingroom/dressingroommgr.hpp"
#include "servercommon/activitydef.hpp"
#include "other/upgradesyscrit/upgradesyscrit.hpp"
#include "common/bintohex.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"

FabaoManager::FabaoManager() : m_role(NULL), m_skill0_last_colddown_time(0), m_other_capability(0)
{
	
}

FabaoManager::~FabaoManager()
{
	
}

void FabaoManager::InitParam(Role* role, const FabaoParam& param)
{
	m_role = role;
	m_param = param;
}

void FabaoManager::GetInitParam(FabaoParam* param)
{
	*param = m_param;
}

void FabaoManager::OnLogin()
{
	this->CheckActive();
	m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_FABAO_GRADE);//嘉年华
}

void FabaoManager::Update(unsigned int now_second)
{
	//如果清零时间大于等于第二天的5点且阶级大于等于5就清零
	const FabaoGradeConfig *grade_cfg = LOGIC_CONFIG->GetFabaoConfig().GetGradeConfig(m_param.grade);
	if (nullptr != grade_cfg && 1 == grade_cfg->is_clear_bless && static_cast<unsigned int>(now_second) >= m_param.clear_bless_time)
	{
		m_param.grade_bless_val = 0;
		m_param.clear_bless_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + EngineAdapter::Instance().NextDayInterval(5, 0, 0);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FABAO);	//清除临时属性
		this->SendInfo();
	}
}

void FabaoManager::OnRoleUpLevel(int level)
{
	this->CheckActive();
//	if (m_param.IsFabaoActived()) return;
//
//	if (level >= LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenRoleLevel(FUN_OPEN_TYPE_FABAO))
//	{
//		ActiveFabao();
//	}
}

bool FabaoManager::ActiveFabao(bool is_recal)
{
	if (!m_param.IsFabaoActived())
	{
		m_param.grade = 1;
		m_param.grade_bless_val = 0;

		const FabaoGradeConfig *grade_cfg = LOGIC_CONFIG->GetFabaoConfig().GetGradeConfig(1);
		if (NULL != grade_cfg)
		{
			m_param.ActiveImage(grade_cfg->active_image_id);
			UseImg(grade_cfg->active_image_id);
		}

		if (is_recal)
		{
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FABAO);
		}
		JinjiesysFuncHelper::Instance().OnActiveJinjieSys(m_role, JINJIE_TYPE_FABAO);
		//role->GetRoleGoal()->CheckCond(COND_TYPE_UPGRADE_FIGHT_MOUNT, 0, 0);   // 不知道拿来干嘛的
	}

	return true;
}

void FabaoManager::SendInfo()
{
	static Protocol::SCSendFabaoInfo fabaoinfo;
	fabaoinfo.grade = m_param.grade;
	fabaoinfo.used_imageid = m_param.used_imageid;
	fabaoinfo.used_special_id = m_param.used_special_id;
	fabaoinfo.active_image_flag.clear();
	for (int i = 0; i < FabaoParam::MAX_ACTIVE_IMAGE_BYTE; ++i)
	{
		fabaoinfo.active_image_flag.push_back(m_param.active_image_flag[i]);
	}
	fabaoinfo.active_special_image_flag.clear();
	for (int i = 0; i < FabaoParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE; ++i)
	{
		fabaoinfo.active_special_image_flag.push_back(m_param.active_special_image_flag[i]);
	}
	fabaoinfo.grade_bless_val = m_param.grade_bless_val;
	fabaoinfo.shuxingdan_count = m_param.shuxingdan_count;
	fabaoinfo.clear_bless_time = m_param.clear_bless_time;
	fabaoinfo.last_upgrade_succ_time = m_param.last_upgrade_succ_time;
	fabaoinfo.special_img_grade_list.clear();
	fabaoinfo.is_used_special_img = m_param.is_used_special_imaged;
	for (int i = 0;i < FabaoParam::MAX_SPECIAL_IMAGE_COUNT; i++)
	{
		fabaoinfo.special_img_grade_list.push_back(m_param.special_img_grade_list[i]);
	}
	fabaoinfo.skill_level_list.clear();
	for (int j = 0; j < FabaoParam::SKILL_COUNT; j++)
	{
		fabaoinfo.skill_level_list.push_back(m_param.skill_level_list[j]);
	}
	fabaoinfo.equip_level_list.clear();
	for (int k = 0; k < FabaoParam::SKILL_COUNT; k++)
	{
		fabaoinfo.equip_level_list.push_back(m_param.equip_level_list[k]);
	}
	fabaoinfo.equip_skill_level = m_param.equip_skill_level;

	SerializeMgr.NetSend(m_role->GetNetId(), fabaoinfo);
}

void FabaoManager::GetRoleAppearance(RoleAppearance* appearance)
{
	// 这里的外观是至宝的，所以赋值给至宝外观
	if (NULL != appearance)
	{
		if (m_param.is_used_special_imaged)
		{
			appearance->fabao_used_imageid = m_param.used_special_id;
		}
		else
		{
			appearance->fabao_used_imageid = m_param.used_imageid;
		}
	}
}

void FabaoManager::ReCalcAttr(CharIntAttrs& base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();
		double rate = 0.0;						// 属性加成万分比
		CharIntAttrs other_per_add_attr;
		CharIntAttrs grade_attrs;
		// 阶数加成
		const FabaoGradeConfig * grade_config = LOGIC_CONFIG->GetFabaoConfig().GetGradeConfig(m_param.grade);
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

			// 下面特殊属性用乘法
			grade_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, grade_config->per_mianshang);
			grade_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, grade_config->per_pofang);
			grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, grade_config->pvp_zengshang_per);
			grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, grade_config->pve_zengshang_per);
			grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, grade_config->pvp_jianshang_per);
			grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, grade_config->pve_jianshang_per);
// 			grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += grade_config->per_mianshang;
// 			grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += grade_config->per_pofang;
// 			grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += grade_config->pvp_zengshang_per;
// 			grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += grade_config->pve_zengshang_per;
// 			grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += grade_config->pvp_jianshang_per;
// 			grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += grade_config->pve_jianshang_per;
			// 大目标特殊形象万分比
			int special_img_per_add = JinjieSysRewardMgr::Instance().GetJinjieSpecialImgExtraAddPer(m_role, JINJIE_TYPE_FABAO);
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
			//if (m_param.grade_bless_val > 0)
			//{
			//	const FabaoGradeConfig * next_grade_config = LOGIC_CONFIG->GetFabaoConfig().GetGradeConfig(m_param.grade + 1);
			//	if (NULL != next_grade_config && NULL != grade_config && grade_config->bless_val_limit > 0)
			//	{
			//		double rate = (((double)m_param.grade_bless_val) / grade_config->bless_val_limit) * (next_grade_config->bless_addition * ROLE_ATTR_PER);
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

			// 天赋
			m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_FABAO_BASE, grade_attrs, m_attrs_add);

			this->CalcBaseAttrAddFromSkill(m_attrs_add);
		}

		// 天赋
		m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_FABAO, m_attrs_add, m_attrs_add);

		//特殊形象加成
		for (int special_img_id = 1; special_img_id < FabaoParam::MAX_SPECIAL_IMAGE_COUNT;)
		{
			if (!m_param.IsSpecialImageActive(special_img_id))
			{
				special_img_id += 1;
				continue;
			}

			const FabaoSpecialImgUpGradeCfg *special_img_cfg = LOGIC_CONFIG->GetFabaoConfig().GetSpecialImgUpGradeCfg(special_img_id,m_param.special_img_grade_list[special_img_id]);
			if (NULL != special_img_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += special_img_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += special_img_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += special_img_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += special_img_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += special_img_cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += special_img_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += special_img_cfg->jianren;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += special_img_cfg->extra_zengshang;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += special_img_cfg->extra_mianshang;
				// 特殊属性
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += special_img_cfg->per_jingzhun;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += special_img_cfg->per_baoji;

				// 下面特殊属性用乘法
//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += special_img_cfg->per_mianshang;
// 				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += special_img_cfg->per_pofang;
// 				m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += special_img_cfg->pvp_zengshang_per;
// 				m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += special_img_cfg->pve_zengshang_per;
// 				m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += special_img_cfg->pvp_jianshang_per;
// 				m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += special_img_cfg->pve_jianshang_per;
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, special_img_cfg->per_mianshang);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, special_img_cfg->per_pofang);
				m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, special_img_cfg->pvp_zengshang_per);
				m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, special_img_cfg->pve_zengshang_per);
				m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, special_img_cfg->pvp_jianshang_per);
				m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, special_img_cfg->pve_jianshang_per);
			}

			this->HuanHuaSpecialCapAdd(special_img_id, m_param.special_img_grade_list[special_img_id], m_attrs_add);

			special_img_id += 1;
		}

		{
			// 属性丹
			if (m_param.shuxingdan_count > 0)
			{
				const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_FABAO, 0);
				if (NULL != cfg_item)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp * m_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji * m_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu * m_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg_item->mingzhong * m_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg_item->shanbi * m_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg_item->baoji * m_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg_item->jianren * m_param.shuxingdan_count;
				}
			}
		}

		{
			// 技能属性
			for (int i = 0; i < FootprintParam::SKILL_COUNT; i++)
			{
				if (m_param.skill_level_list[i] > 0)
				{
					const FabaoSkillConifg *skill_cfg = LOGIC_CONFIG->GetFabaoConfig().GetSkillCfg(i, m_param.skill_level_list[i]);
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

		{
			// 法宝装备
			int active_equip_grade = LOGIC_CONFIG->GetFabaoConfig().GetActiveEquipGrade();
			if (active_equip_grade <= m_param.grade)
			{
				for (int i = 0; i < FabaoParam::EQUIP_COUNT; ++i)
				{
					const FabaoEquipConifg* equip_cfg = LOGIC_CONFIG->GetFabaoConfig().GetEquipCfg(i);
					if (NULL != equip_cfg)
					{
						int level = m_param.equip_level_list[i];
						if (level >= 0 && level < equip_cfg->max_level)
						{
							rate += equip_cfg->level_cfg[level].add_percent;
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
		}
		// 其他百分比属性加成，与上面的全属性加成加算
		{
			m_attrs_add.AddAttrs(&other_per_add_attr);
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_FABAO, m_attrs_add);
		m_role->GetCapability()->AddCap(CAPABILITY_TYPE_FABAO, m_other_capability);
		m_param.capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_FABAO);
	}
	base_add.Add(m_attrs_add.m_attrs, true);
}


bool FabaoManager::UpOneGrade(int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num)
// 使用进阶丹直接进阶
{
	if (m_param.grade < valid_min_grade)
	{//阶数不可使用
		m_role->NoticeNum(errornum::EN_FABAO_UP_ONE_GRADE_LIMIT);
		return false;
	}
	else if (m_param.grade > valid_max_grade)
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(return_item_id);
		if (item_base != nullptr)
		{
			for (int loop_time = 0; return_item_num > 0 && loop_time < 1000; ++loop_time)
			{
				int put_num = return_item_num > item_base->GetPileLimit() ? item_base->GetPileLimit() : return_item_num;
				return_item_num -= put_num;

				ItemConfigData return_item(return_item_id, true, put_num);
				m_role->GetKnapsack()->PutOrMail(return_item, PUT_REASON_FABAO_UPGRADE_ONE_RETURN);
			}

			return true;
		}
		return false;
	}

	short old_grade = m_param.grade;
	short old_bless_val = m_param.grade_bless_val;

	const FabaoGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetFabaoConfig().GetGradeConfig(old_grade);
	if (NULL == curr_grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_FABAO_CANNOT_UPGRADE);
		return false;
	}

	const FabaoGradeConfig * target_grade_cfg = LOGIC_CONFIG->GetFabaoConfig().GetGradeConfig(old_grade + 1);
	if (NULL == target_grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_FABAO_CANNOT_UPGRADE);
		return false;
	}

	const FabaoUpGradeProbConfig * upgrade_prob_cfg = LOGIC_CONFIG->GetFabaoConfig().GetUpGradeProbConfig(old_grade,old_bless_val);
	if (NULL == upgrade_prob_cfg)
	{
		m_role->NoticeNum(errornum::EN_FABAO_CANNOT_UPGRADE);
		return false;
	}

	m_param.grade += 1;
	m_param.grade_bless_val = 0;
	EventHandler::Instance().OnFaBaoUpgrade(m_role, old_grade, m_param.grade);

	unsigned int now_time = (unsigned int)EngineAdapter::Instance().Time();
	const UpgradeRushReturnCfg* upgrade_rush_cfg = LOGIC_CONFIG->GetUpgradeRushConfig().GetUpgradeRushReturnCfg(UPGRADE_RUSH_TYPE_FABAO, m_param.grade);
	if (NULL != upgrade_rush_cfg)
	{
		if ((now_time - m_param.last_upgrade_succ_time) <= (unsigned int)(upgrade_rush_cfg->limit_time * SECOND_PER_HOUR))
		{
			m_role->GetKnapsack()->PutOrMail(upgrade_rush_cfg->reward_item, PUT_REASON_FABAO_UP);
		}
	}
	m_param.last_upgrade_succ_time = now_time;

	const FabaoSkillConifg *skill_cfg = LOGIC_CONFIG->GetFabaoConfig().GetSkillCfg(0, m_param.skill_level_list[0] + 1);
	if (NULL != skill_cfg && m_param.grade >= skill_cfg->fabao_grade)
	{
		m_param.skill_level_list[0]++;
	}

	m_param.ActiveImage(target_grade_cfg->active_image_id);
	this->UseImg(target_grade_cfg->active_image_id);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FABAO);

	if (target_grade_cfg->is_broadcast)
	{
		int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_fabao_upgrade,
			UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade, this->GetMedalSkillLevel());
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
	gamelog::g_log_other.printf(LL_INFO, "Fabao::OnUpGrade user[%d, %s], grade_bless[%d, %d] -> [%d, %d]",
		m_role->GetUID(), m_role->GetName(), old_grade, old_bless_val, m_param.grade, m_param.grade_bless_val);
	return true;
}

bool FabaoManager::OnUpGrade(bool is_auto_buy, int repeat_times)
{
	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	short old_grade = m_param.grade;
	short oldest_bless_val = m_param.grade_bless_val;
	bool upgrade_succ = false;

	const FabaoGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetFabaoConfig().GetGradeConfig(old_grade);
	if (NULL == curr_grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_FABAO_CANNOT_UPGRADE);
		return false;
	}

	const FabaoGradeConfig * target_grade_cfg = LOGIC_CONFIG->GetFabaoConfig().GetGradeConfig(old_grade + 1);
	if (NULL == target_grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_FABAO_CANNOT_UPGRADE);
		return false;
	}

	for (int i = 0; i < repeat_times; ++i)
	{
		short old_bless_val = m_param.grade_bless_val;
		const FabaoUpGradeProbConfig * upgrade_prob_cfg = LOGIC_CONFIG->GetFabaoConfig().GetUpGradeProbConfig(old_grade, old_bless_val);
		if (NULL == upgrade_prob_cfg)
		{
			m_role->NoticeNum(errornum::EN_FABAO_CANNOT_UPGRADE);
			//return false;
			upgrade_succ = true;
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
				stuff_list[stuff_count].buyable = is_auto_buy ? true : false;

				++stuff_count;
			}

			// 如果限时进阶丹用完了，还不够一次批量使用，就使用普通进阶丹
			if (upgrade_stuff_num < curr_grade_cfg->upgrade_stuff_count && stuff_count < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM)
			{
				stuff_list[stuff_count].item_id = curr_grade_cfg->upgrade_stuff_id;
				stuff_list[stuff_count].num = curr_grade_cfg->upgrade_stuff_count - upgrade_stuff_num;
				stuff_list[stuff_count].buyable = is_auto_buy ? true : false;

				++stuff_count;
			}

			if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, false))
			{
				//return false;
				upgrade_succ = true;
				break;
			}
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "FabaoManager::OnUpGrade"))
		{
			upgrade_succ = true;
			break;
		}
		
		{
			// 生效

			int rand_val = RandomNum(10000);
			if (rand_val < upgrade_prob_cfg->succ_rate || m_param.grade_bless_val >= curr_grade_cfg->bless_val_limit)
			{
				upgrade_succ = true;
			}

			if (false == upgrade_succ)
			{
				int crit_value = UpgradeSysCrit::Instance().CalculateCritValue(m_role, UPGRADE_SYS_CRIT_TYPE_FABAO);
				short new_bless_val = old_bless_val + (upgrade_prob_cfg->RandAddBlessVal() * crit_value);

				if (new_bless_val > curr_grade_cfg->bless_val_limit) new_bless_val = curr_grade_cfg->bless_val_limit;

				m_param.grade_bless_val = new_bless_val;

				//m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FABAO);	//计算临时属性

			}
			else
			{
				m_param.grade += 1;
				m_param.grade_bless_val = 0;
				upgrade_succ = true;

				unsigned int now_time = (unsigned int)EngineAdapter::Instance().Time();
				const UpgradeRushReturnCfg* upgrade_rush_cfg = LOGIC_CONFIG->GetUpgradeRushConfig().GetUpgradeRushReturnCfg(UPGRADE_RUSH_TYPE_FABAO, m_param.grade);
				if (NULL != upgrade_rush_cfg)
				{
					if ((now_time - m_param.last_upgrade_succ_time) <= (unsigned int)(upgrade_rush_cfg->limit_time * SECOND_PER_HOUR))
					{
						m_role->GetKnapsack()->PutOrMail(upgrade_rush_cfg->reward_item, PUT_REASON_FABAO_UP);
					}
				}
				m_param.last_upgrade_succ_time = now_time;

				const FabaoSkillConifg *skill_cfg = LOGIC_CONFIG->GetFabaoConfig().GetSkillCfg(0, m_param.skill_level_list[0] + 1);
				if (NULL != skill_cfg && m_param.grade >= skill_cfg->fabao_grade)
				{
					m_param.skill_level_list[0]++;
				}

				m_param.ActiveImage(target_grade_cfg->active_image_id);
				this->UseImg(target_grade_cfg->active_image_id);

				m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FABAO);

				if (target_grade_cfg->is_broadcast)
				{
					int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_fabao_upgrade,
						UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade, this->GetMedalSkillLevel());
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

				//m_role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_FABAO, m_param.grade);   // 进阶丹返还活动
				break;
			}
		}
	}
	this->SendInfo();
	EventHandler::Instance().OnFaBaoUpgrade(m_role, old_grade, m_param.grade);
	gamelog::g_log_other.printf(LL_INFO, "Fabao::OnUpGrade user[%d, %s], grade_bless[%d, %d] -> [%d, %d]",
		m_role->GetUID(), m_role->GetName(), old_grade, oldest_bless_val, m_param.grade, m_param.grade_bless_val);

	return !upgrade_succ;
}

void FabaoManager::UseImg(short img_id)
{
	if (img_id < 0 || img_id >= FabaoParam::MAX_IMAGE_COUNT)
	{
		return;
	}

	FabaoParam &fabaoparam = m_param;

	if (0 != img_id && !m_param.IsImageActived(img_id))
	{
		m_role->NoticeNum(errornum::EN_FABAO_IMG_NOT_ACTIVE);
		return;
	}
	m_param.is_used_special_imaged = 0;
	fabaoparam.used_imageid = img_id;
	m_role->RefreshApperance(true);
	this->SendInfo();
}

void FabaoManager::UseSpecialImg(short special_img_id)
{
	if ( special_img_id < 0 || special_img_id >= FabaoParam::MAX_SPECIAL_IMAGE_COUNT + FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM)
	{
		return;
	}

	FabaoParam &fabaoparam = m_param;

	if (0 != special_img_id && !m_param.IsSpecialImageActive(special_img_id - FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM))
	{
		m_role->NoticeNum(errornum::EN_FABAO_IMG_NOT_ACTIVE);
		return;
	}

	m_param.is_used_special_imaged = 1;
	fabaoparam.used_special_id = special_img_id;
	m_role->RefreshApperance(true);
	this->SendInfo();
}

void FabaoManager::UnUseSpecialImg(short special_img_id)
{
	if (special_img_id < 0 || special_img_id >= FabaoParam::MAX_SPECIAL_IMAGE_COUNT + FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM)
	{
		return;
	}

	m_param.used_special_id = 0;
	m_role->RefreshApperance(true);
	this->SendInfo();
}

bool FabaoManager::ActiveSpecialImg(short special_img_id, bool is_notify)
{
	if ( special_img_id <= FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM || special_img_id >= FabaoParam::MAX_SPECIAL_IMAGE_COUNT + FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM)
	{
		return false;
	}

	int tmp = special_img_id;

	FabaoParam &fabaoparam = m_param;

	if (fabaoparam.IsSpecialImageActive(tmp - FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM))
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_FABAO_IMG_HAS_ACTIVE);
		}
		return false;
	}

	const FabaoSpecialImgCfg * special_img_cfg = NULL;
	special_img_cfg = LOGIC_CONFIG->GetFabaoConfig().GetSpecialImgCfg(tmp - FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM);
	if (NULL == special_img_cfg)
	{
		return false;
	}

	char old_active_flag_hex[FabaoParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE * 2 + 1] = { 0 };
	BinToHex((const char*)m_param.active_special_image_flag, sizeof(m_param.active_special_image_flag), old_active_flag_hex);

	fabaoparam.ActiveSpecialImage(tmp - FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM);

	char new_active_flag_hex[FabaoParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE * 2 + 1] = { 0 };
	BinToHex((const char*)m_param.active_special_image_flag, sizeof(m_param.active_special_image_flag), new_active_flag_hex);

	m_role->GetImgFuling()->OnPutOnImg(JINGJIE_TYPE_FABAO, tmp - FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM);
	if (0 == m_param.special_img_grade_list[tmp - FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM])
	{
		m_param.special_img_grade_list[tmp - FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM] = 1;
	}
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FABAO);
	JinjiesysFuncHelper::Instance().OnActiveSpecialImg(m_role, JINJIE_TYPE_FABAO, special_img_id);
	this->UseSpecialImg(special_img_id);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_FABAO,special_img_id,true);
	gamelog::g_log_other.printf(LL_INFO, "Fabao::UseSpecialImg user[%d, %s], level[%d], special_img_id[%d], old_flag[%s], new_flag[%s]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), special_img_id, old_active_flag_hex, new_active_flag_hex);

	return true;
}

bool FabaoManager::UnActiveSpecialImg(short special_img_id)
{
	if ( special_img_id <= FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM || special_img_id >= FabaoParam::MAX_SPECIAL_IMAGE_COUNT + FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM)
	{
		return false;
	}

	FabaoParam &fabaoparam = m_param;

	if (!fabaoparam.IsSpecialImageActive(special_img_id - FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM))
	{
		return false;
	}

	BitmapFunction::ClearBit(fabaoparam.active_special_image_flag, sizeof(fabaoparam.active_special_image_flag), special_img_id - FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM);
	if (1 == m_param.special_img_grade_list[special_img_id - FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM])
	{
		m_param.special_img_grade_list[special_img_id - FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM] = 0;
	}
	this->UnUseSpecialImg(special_img_id);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FABAO);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_FABAO, special_img_id, false);
	return true;
}

int FabaoManager::GetHasActiveFabao()
{
	int count = 0;

	FabaoParam &fabaoparam = m_param;

	for (int i = 0; i < FabaoParam::MAX_SPECIAL_IMAGE_COUNT; ++i)
	{
		if (fabaoparam.IsSpecialImageActive(i))
		{
			++count;
		}
	}

	return count;
}

bool FabaoManager::OnUseShuxingdan(int slot_idx, short &use_count)
{
	if (slot_idx < 0 || slot_idx >= SHUXINGDAN_MAX_TYPE || use_count <= 0)
	{
		return false;
	}

	const FabaoGradeConfig *grade_cfg = LOGIC_CONFIG->GetFabaoConfig().GetGradeConfig(m_param.grade);
	if (NULL == grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_FABAO_SHUXINGDAN_USE_LIMIT_NUM);
		return false;
	}

	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_FABAO, slot_idx);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_param.grade < cfg_item->order_limit)
	{
		gstr::SendError(m_role, "shuxingdan_use_order_limit_1", SHUXINGDAN_TYPE_FABAO, cfg_item->order_limit - 1, slot_idx); // 客户端阶数-1
		return false;
	}

	if (m_param.shuxingdan_count + use_count > cfg_item->use_limit_num)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	int special_img_add_count = 0;
	for (int i = 0; i < FabaoParam::MAX_SPECIAL_IMAGE_COUNT; i++)
	{
		if (!m_param.IsSpecialImageActive(i))
			continue;

		const FabaoSpecialImgUpGradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetFabaoConfig().GetSpecialImgUpGradeCfg(i, m_param.special_img_grade_list[i]);
		if (NULL != special_img_grade_cfg)
		{
			special_img_add_count += special_img_grade_cfg->shuxingdan_count;
		}
	}

	// 策划需求，修正真正使用数量
	int can_use_count = grade_cfg->shuxingdan_limit + special_img_add_count - m_param.shuxingdan_count;
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

	m_param.shuxingdan_count += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FABAO);
	this->SendInfo();

	return true;
}

bool FabaoManager::UplevelSkill(int skill_idx, bool is_auto_buy)
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

	if (!m_param.IsFabaoActived())
	{
		m_role->NoticeNum(errornum::EN_FABAO_IMG_NOT_ACTIVE);
		return false;
	}

	const FabaoSkillConifg *target_skill_cfg = LOGIC_CONFIG->GetFabaoConfig().GetSkillCfg(skill_idx, m_param.skill_level_list[skill_idx] + 1);
	if (NULL == target_skill_cfg)
	{
		return false;
	}

	if (0 == target_skill_cfg->skill_type)
	{
		return false;
	}

	if (target_skill_cfg->fabao_grade > m_param.grade)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

	{
		// 升级材料
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = target_skill_cfg->uplevel_stuff_id;
		stuff_list[0].num = target_skill_cfg->uplevel_stuff_num;
		stuff_list[0].buyable = is_auto_buy ? true : false;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
		{
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "FabaoManager::UplevelSkill"))
		{
			return false;
		}
	}

	m_param.skill_level_list[skill_idx]++;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FABAO);
	this->SendInfo();

	return true;
}

int FabaoManager::GetHurtDecPercent()
{
// 	static const int SKILL_ID0 = 0;
// 	UNSTD_STATIC_CHECK(FabaoParam::SKILL_COUNT > SKILL_ID0);
// 
// 	const FabaoSkillConifg *skill0_cfg = LOGIC_CONFIG->GetFabaoConfig().GetSkillCfg(SKILL_ID0, m_param.skill_level_list[SKILL_ID0]);
// 	if (NULL != skill0_cfg && skill0_cfg->param_a > 0)
// 	{
// 		return skill0_cfg->param_a;
// 	}

	return 0;
}

bool FabaoManager::UplevelEquip(int equip_idx)
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

	if (!m_param.IsFabaoActived())
	{
		return false;
	}

	FabaoManager *fabao = m_role->GetFabaoManager();
	if (NULL == fabao)
	{
		return false;
	}
	FabaoParam *fabaoparam = fabao->GetParam();
	if (NULL == fabaoparam)
	{
		return false;
	}

	const FabaoEquipConifg *equip_cfg = LOGIC_CONFIG->GetFabaoConfig().GetEquipCfg(equip_idx);
	if (NULL == equip_cfg)
	{
		return false;
	}

	short level = m_param.equip_level_list[equip_idx];
	//if (level >= equip_cfg->max_level - 1)
	//{
	//	m_role->NoticeNum(errornum::EN_FABAO_EQUIP_LEVEL_MAX);
	//	return false;
	//}

	if (m_param.grade < LOGIC_CONFIG->GetFabaoConfig().GetActiveEquipGrade())
	{
		return false;
	}

	const FabaoGradeConfig * cur_grade_cfg = LOGIC_CONFIG->GetFabaoConfig().GetGradeConfig(fabaoparam->grade);
	if (cur_grade_cfg == NULL)
	{
		return false;
	}

	if (level >= cur_grade_cfg->equip_level_toplimit)
	{
		m_role->NoticeNum(errornum::EN_FABAO_EQUIP_LEVEL_MAX);
		return false;
	}

	// 消耗
	if (!m_role->GetKnapsack()->ConsumeItem(equip_cfg->level_cfg[level].item.item_id, equip_cfg->level_cfg[level].item.num, "FabaoEquipUplevel"))
	{
		return false;
	}

	// 结算

	m_param.equip_level_list[equip_idx]++;

	bool is_active_upgrade_skill = false;
	int min_equip_level = this->RefreshFabaoMinEquipLevel();
	if (LOGIC_CONFIG->GetFabaoConfig().GetActiveEquipSkillLevel() <= min_equip_level)
	{
		int equip_skill_level = LOGIC_CONFIG->GetFabaoConfig().GetEquipSkillLevel(min_equip_level);
		if (equip_skill_level > 0)
		{
			m_param.equip_skill_level = equip_skill_level;
			is_active_upgrade_skill = true;
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FABAO);
	this->SendInfo();

	if (is_active_upgrade_skill)
	{
		CommonDataParam *common_data = m_role->GetCommonDataParam();
		if (NULL != common_data)
		{
			if (0 == (common_data->upgrade_skill_skill_active_flag & (1 << UpGradeSkillConfig::SKILL_TYPE_FABAO)))
			{
				common_data->upgrade_skill_skill_active_flag |= (1 << UpGradeSkillConfig::SKILL_TYPE_FABAO);
				if (-1 == common_data->upgrade_skill_next_skill_index)
				{
					common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_FABAO;
				}

				m_role->SendGradeSkillInfo(m_role);
			}
		}
	}

	return true;
}

int FabaoManager::RefreshFabaoMinEquipLevel()
{
	int min = m_param.equip_level_list[0];

	for (int i = 1; i < FootprintParam::EQUIP_COUNT; ++i)
	{
		min = m_param.equip_level_list[i] < min ? m_param.equip_level_list[i] : min;
	}

	return min;
}


//void FabaoManager::OnRoleBeenAttack(ObjID attacker)
//{
//	Obj* obj = m_role->GetScene()->GetObj(attacker);
//	if (NULL == obj || (obj->GetObjType() != Obj::OBJ_TYPE_ROLE && obj->GetObjType() != Obj::OBJ_TYPE_MONSTER))
//	{
//		return;
//	}
//
//	Character *target = (Character *)obj;
//
//	const FabaoSkillConifg *skill_cfg = LOGIC_CONFIG->GetFabaoConfig().GetSkillCfg(0, m_param.skill_level_list[0]);
//	if (nullptr == skill_cfg)
//	{
//		return;
//	}
//
//	unsigned long long now_sec_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();
//
//	if (now_sec_ms < m_skill0_last_colddown_time)
//	{
//		return;
//	}
//
//	Attribute cur_target_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
//	Attribute max_target_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
//	int hp_per = static_cast<int>(cur_target_hp / max_target_hp * 10000.0f);
//	if (hp_per >= skill_cfg->param_a)
//	{
//		return;
//	}
//
//	int rand_val = RandomNum(10000);
//	if (skill_cfg->param_b > rand_val)
//	{
//		return;
//	}
//
//	Attribute gongji = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI);
//	int param_1 = static_cast<int>(gongji * (skill_cfg->param_c / 10000.0f));
//
//	EffectOnce *dec_effect = new EffectOnce(m_role->GetId(), 0, param_1, EffectBase::FIGHT_TYPE_NORMAL);
//	target->AddEffect(dec_effect);
//
//	m_skill0_last_colddown_time = now_sec_ms + skill_cfg->param_d;
//}

// 攻击时，有param_b%概率触发天火，对目标及其周围共param_c个目标造成param_d%攻击的伤害, 冷却param_a毫秒
void FabaoManager::OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure)
{
	if (NULL == target) return;
	if (base_injure <= 0) return;
	Scene *scene = m_role->GetScene();
	if (NULL == scene) return;
	
	const FabaoSkillConifg *skill_cfg = LOGIC_CONFIG->GetFabaoConfig().GetSkillCfg(0, m_param.skill_level_list[0]);
	if (nullptr == skill_cfg)
	{
		return;
	}

	unsigned long long now_sec_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();

	if (now_sec_ms < m_skill0_last_colddown_time)
	{
		return;
	}

	if (RandomNum(10000) > skill_cfg->param_b)
	{
		return;
	}

	static const int RANGE = 10;
	static const int MAX_COUNT_OBJ = 32;
	static ObjID targer_list[MAX_COUNT_OBJ] = { INVALID_OBJ_ID };
	int count = scene->GetZoneMatrix()->GetObjByArea(target->GetPos(), RANGE, RANGE, targer_list, sizeof(targer_list) / sizeof(targer_list[0]));

	Attribute hurt_hp = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI) * (skill_cfg->param_d * ROLE_ATTR_PER));
	
	// 主目标增伤
	injure += hurt_hp;

	// 其他伤害百分比
	int taret_num = 0;
	for (int i = 0; i < count && i < MAX_COUNT_OBJ && taret_num < skill_cfg->param_c; i++)
	{
		Obj *tmp_target_obj = scene->GetObj(targer_list[i]);
		if (NULL == tmp_target_obj || !tmp_target_obj->IsFightCharactor(tmp_target_obj->GetObjType())) continue;
		if (!m_role->IsEnemy(tmp_target_obj) || tmp_target_obj == target) continue;

		Character *tmp_target = (Character*)tmp_target_obj;
		EffectOnce *effect = new EffectOnce(m_role->GetId(), 0, -hurt_hp, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_SKILL);
		tmp_target->AddEffect(effect);

		++taret_num;
	}

	m_skill0_last_colddown_time = now_sec_ms + skill_cfg->param_a;
}

bool FabaoManager::FabaoSpecialImgUpgrade(short special_img_id)
{
	special_img_id += FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM;
	if (special_img_id <= FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM || special_img_id >= FabaoParam::MAX_SPECIAL_IMAGE_COUNT + FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM)
	{
		return false;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	FabaoManager *mount_mng = m_role->GetFabaoManager();
	if (NULL == mount_mng)
	{
		return false;
	}

	FabaoParam *mount = mount_mng->GetParam();
	if (NULL == mount)
	{
		return false;
	}

	if (!mount->IsSpecialImageActive(special_img_id - FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM))
	{
		m_role->NoticeNum(errornum::EN_FABAO_IMG_NOT_ACTIVE);
		return false;
	}

	const int old_grade = mount->special_img_grade_list[special_img_id - FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM];
	if (old_grade >= FabaoParam::MAX_UPGRADE_LIMIT)
	{
		m_role->NoticeNum(errornum::EN_FABAO_CANNOT_UPGRADE);
		return false;
	}

	const FabaoSpecialImgUpGradeCfg *upgrade_cfg = LOGIC_CONFIG->GetFabaoConfig().GetSpecialImgUpGradeCfg(special_img_id - FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM,old_grade + 1);
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
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "FabaoManager::FabaoSpecialImgUpgrade"))
	{
		return false;
	}

	// 生效
	++mount->special_img_grade_list[special_img_id - FabaoParam::DISTINGUISH_IMG_SPECIALIMG_NUM];

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FABAO);

	this->SendInfo();

	return true;
}

void FabaoManager::OnCompleteTask(TaskID task_id)
{
	this->CheckActive();
}

void FabaoManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->CheckActive();
	}
}

void FabaoManager::CheckActive()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (!m_param.IsFabaoActived())
	{
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_FABAO);
		if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day)
		{
			if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
			{
				this->ActiveFabao();
			}
		}
	}
}

// 幻化特殊战力加成
void FabaoManager::HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add)
{
	if (!LOGIC_CONFIG->GetFabaoConfig().IsAddSpecialCap())
	{
		return;
	}

	auto cfg_item = LOGIC_CONFIG->GetFabaoConfig().GetHuanHuaSpecialCapAddCfg(huanhua_id);
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

int FabaoManager::GetSpecialImageLevelById(int imageid)
{
	if (imageid < 0 || imageid >= FabaoParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return 0;
	}

	if (!m_param.IsSpecialImageActive(imageid)) return 0;

	return m_param.special_img_grade_list[imageid];
}

void FabaoManager::CalcBaseAttrAddFromSkill(CharIntAttrs & base_add)
{
	// 阶数加成
	const FabaoGradeConfig * grade_config = LOGIC_CONFIG->GetFabaoConfig().GetGradeConfig(m_param.grade);
	if (NULL == grade_config)
	{
		return;
	}

	int total_add_per = 0;
	for (int i = 0; i < FabaoParam::SKILL_COUNT; i++)
	{
		if (m_param.skill_level_list[i] <= 0)
		{
			continue;
		}
		const FabaoSkillConifg *skill_cfg = LOGIC_CONFIG->GetFabaoConfig().GetSkillCfg(i, m_param.skill_level_list[i]);
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

int FabaoManager::GetMedalSkillLevel()
{
	short medal_skill_index = LOGIC_CONFIG->GetFabaoConfig().GetMedalSkillIndex();
	if (medal_skill_index < 0 || medal_skill_index >= (short)static_array_size(m_param.skill_level_list))
	{
		return 0;
	}
	return m_param.skill_level_list[medal_skill_index];
}
