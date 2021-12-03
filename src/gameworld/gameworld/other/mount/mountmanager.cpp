#include "mountmanager.hpp"

#include "gstr.h"
#include "gamelog.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "protocal/msgmount.hpp"
#include "scene/scene.h"
#include "config/logicconfigmanager.hpp"
#include "other/mount/mountopconfig.hpp"
#include "config/taskconfig/husongconfig.hpp"
#include "config/joinlimitconfig.hpp"
#include "servercommon/vipdef.hpp"
#include "other/mount/mountfunction.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "serverlogic.h"
#include "config/shuxingdanconfig.hpp"
#include "task/taskrecorder.h"
#include "other/capability/capability.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/multimount/multimount.hpp"
#include "other/xunzhang/xunzhangmanager.hpp"
#include "other/zhibao/zhibaomanager.hpp"
#include "other/chinesezodiac/chinesezodiacconfig.hpp"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include "other/imgfuling/imgfuling.hpp"
#include "servercommon/imgfulingdef.hpp"
#include "other/talent/talent.hpp"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "other/jinjiesysreward/jinjiesysrewardmgr.hpp"
#include "other/dressingroom/dressingroomconfig.hpp"
#include "other/dressingroom/dressingroommgr.hpp"
#include "other/xianzunka/xianzunka.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "common/bintohex.h"
#include "servercommon/serverconfig/crossconfig.hpp"

MountManager::MountManager()
	: m_role(NULL),  m_other_capability(0)
{
}

MountManager::~MountManager()
{

}

void MountManager::Init(Role *role, const MountParam &mount_param)
{
	m_role = role;
	m_mount_param = mount_param;


	if (mount_param.old_grade_bless_val != 0)
	{
		m_mount_param.grade_bless_val = mount_param.old_grade_bless_val;
		m_mount_param.old_grade_bless_val = 0;
	}
}

void MountManager::GetOtherInitParam(MountParam *param)
{
	*param = m_mount_param;
}

void MountManager::GetRoleAppearance(RoleAppearance *appearance)
{
	if (NULL != appearance)
	{
		appearance->mount_used_imageid = m_mount_param.temporary_imageid > 0 ? m_mount_param.temporary_imageid : m_mount_param.used_imageid;
	}
}

void MountManager::OnLogin()
{
	this->CheckActive();
}

void MountManager::OnRoleLevelChange(int level)
{
	m_mount_param.level = level;
	if (m_mount_param.IsMountActived())
	{
		this->SendMountInfo();
	}
}

void MountManager::Update(time_t now_second)
{
	const MountGradeConfig *grade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(m_mount_param.grade);
 	if (nullptr != grade_cfg && 1 == grade_cfg->is_clear_bless && now_second >= m_mount_param.clear_upgrade_time)
 	{
 		m_mount_param.grade_bless_val = 0;
		m_mount_param.clear_upgrade_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + EngineAdapter::Instance().NextDayInterval(NEXTDAY_CLEAR_BLESS_VAL_TIME, 0, 0);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT); //清除临时属性
		this->SendMountInfo();
	}

	if (0 != m_mount_param.temporary_imageid_invalid_time && static_cast<unsigned int>(now_second) > m_mount_param.temporary_imageid_invalid_time)
	{
		if (0 != m_mount_param.temporary_imageid_has_select)
		{
			m_mount_param.temporary_imageid = 0;
			m_mount_param.temporary_imageid_has_select = 0;
			this->SendMountInfo();

			if (this->IsOnMount())
			{
				Protocol::SCMountAppeChange mac;
				mac.objid = m_role->GetId();
				mac.mount_appeid = m_mount_param.used_imageid;

				if (NULL != m_role->GetScene()) 
				{
					m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&mac, sizeof(mac));
				}

				m_role->RefreshApperance(false);
			}
		}
	}
}

void MountManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->CheckActive();
	}
}

// 生命值低于param_b%时，自身受到伤害降低param_c%，持续param_d毫秒。 冷却param_a毫秒
void MountManager::OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{
	if (delta_hp >= 0) return;
	if (m_mount_param.skill_level_list[0] <= 0) return;

	const MountSkillConifg *skill_cfg = LOGIC_CONFIG->GetMountOpConfig().GetSkillCfg(0, m_mount_param.skill_level_list[0]);
	if (NULL == skill_cfg)
	{
		return;
	}

	unsigned long long now_sec_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();

	if (now_sec_ms < m_mount_param.skill0_last_colddown_time)
	{
		return;
	}

	Attribute cur_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	Attribute target_hp = static_cast<Attribute>(m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (skill_cfg->param_b * ROLE_ATTR_PER));

	if (cur_hp < target_hp)
	{
		EffectDecHurt* dec_hurt_effect = new EffectDecHurt(m_role->GetId(), 0, skill_cfg->param_d, skill_cfg->skill_level,
			EffectBase::PRODUCT_METHOD_SKILL, EBT_DEC_HURT);
		dec_hurt_effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
		dec_hurt_effect->SetDecPercent(skill_cfg->param_c);
		dec_hurt_effect->SetClientEffectType(EffectBase::ECT_SKILL_DEC_ON_HURT);
		m_role->AddEffect(dec_hurt_effect);

		delta_hp -= (Attribute)(delta_hp * skill_cfg->param_c * ROLE_ATTR_PER);

		m_mount_param.skill0_last_colddown_time = now_sec_ms + skill_cfg->param_a;
	}
}

// void MountManager::OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag)
// {
// 	if (m_mount_param.skill_level_list[0] <= 0) return;
// 
// 	const MountSkillConifg *skill_cfg = LOGIC_CONFIG->GetMountOpConfig().GetSkillCfg(0, m_mount_param.skill_level_list[0]);
// 	if (NULL == skill_cfg)
// 	{
// 		return;
// 	}
// 
// 	unsigned long long now_sec_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();
// 
// 	if (now_sec_ms < m_mount_param.skill0_last_colddown_time)
// 	{
// 		return;
// 	}
// 
// 	if (RandomNum(10000) >= skill_cfg->param_b)
// 	{
// 		return;
// 	}
// 
// 	m_mount_param.skill0_last_colddown_time = now_sec_ms + skill_cfg->param_a;
// 
// 	passive_flag |= (1 << EffectBase::PASSIVE_FLAG_MOUNT_JIANTA);
// 
// 	int rate = skill_cfg->param_c;
// 	
// 	if (m_mount_param.skill_level_list[3] > 0)
// 	{
// 		const MountSkillConifg *skill3_cfg = LOGIC_CONFIG->GetMountOpConfig().GetSkillCfg(3, m_mount_param.skill_level_list[3]);
// 		if (NULL != skill3_cfg)
// 		{
// 			rate += skill3_cfg->param_a;
// 			passive_flag |= (1 << EffectBase::PASSIVE_FLAG_MOUNT_NUTA);
// 		}
// 	}
// 
// 	injure += (long long)(base_injure * (rate * ROLE_ATTR_PER));
// }

bool MountManager::GoonMount(bool goon_mount, bool notify, bool is_force)
{
	if (Role::MOVE_MODE_FLY == m_role->GetMoveMode())
	{
		//if (notify) m_role->NoticeNum(errornum::EN_MOUNT_CANNOT_GOON_IN_FLY);
		return false;
	}

	bool old_is_mount = this->IsOnMount();

	if (goon_mount) // 上坐骑
	{
		if (old_is_mount)
		{
			//if (notify) m_role->NoticeNum(errornum::EN_MOUNT_ALREADY_ON_MOUNT);
			return false;
		}

		if (!is_force && !m_role->GetRoleStatusManager()->CanMountOn())
		{
			//if (notify) m_role->NoticeNum(errornum::EN_MOUNT_CANNOT_MOUNTON);
			return false;
		}

		if (m_role->GetMultiMount()->IsOnMount())
		{
			m_role->GetMultiMount()->MountOn(false);
		}

		if (m_role->GetFightMountManager()->IsOnFightMount())
		{
			m_role->GetFightMountManager()->GoonFightMount(false, false);
		}

		m_mount_param.OnMount();
	}
	else // 下坐骑
	{
		if (!old_is_mount)
		{
			//if (notify) m_role->NoticeNum(errornum::EN_MOUNT_NOT_ON_MOUNT);
			return false;
		}

		m_mount_param.OffMount();
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);

	this->SendMountInfo();

	{
		Protocol::SCMountAppeChange mac;
		mac.objid = m_role->GetId();
		mac.mount_appeid = this->GetMountAppeID();

		if (NULL != m_role->GetScene()) 
		{
			m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&mac, sizeof(mac));
		}
	}

	return true;
}

void MountManager::SendMountInfo()
{
	Protocol::SCMountInfo mi;
	mi.mount_flag = m_mount_param.mount_flag;
	mi.mount_level = m_mount_param.level;
	mi.grade = m_mount_param.grade;
	mi.grade_bless_val = m_mount_param.grade_bless_val;
	mi.clear_upgrade_time = m_mount_param.clear_upgrade_time;
	mi.used_imageid = m_mount_param.used_imageid;

	UNSTD_STATIC_CHECK(sizeof(mi.active_image_flag) == sizeof(m_mount_param.active_image_flag));
	memcpy(mi.active_image_flag, m_mount_param.active_image_flag, sizeof(mi.active_image_flag));

	UNSTD_STATIC_CHECK(sizeof(mi.active_special_image_flag) == sizeof(m_mount_param.active_special_image_flag));
	memcpy(mi.active_special_image_flag, m_mount_param.active_special_image_flag, sizeof(mi.active_special_image_flag));
	
	mi.temporary_imageid = m_mount_param.temporary_imageid;
	mi.temporary_imageid_has_select = m_mount_param.temporary_imageid_has_select;
	mi.temporary_imageid_invalid_time = m_mount_param.temporary_imageid_invalid_time;
	mi.star_level = m_mount_param.star_level;
	mi.equip_skill_level = m_mount_param.equip_skill_level;
	mi.last_upgrade_time = m_mount_param.last_upgrade_time;

	ARRAYCPY(mi.shuxingdan_list, m_mount_param.shuxingdan_list);
	memcpy(mi.equip_level_list, m_mount_param.equip_level_list, sizeof(mi.equip_level_list));
	memcpy(mi.skill_level_list, m_mount_param.skill_level_list, sizeof(mi.skill_level_list));
	memcpy(mi.special_img_grade_list, m_mount_param.special_img_grade_list, sizeof(mi.special_img_grade_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&mi, sizeof(mi));
}

int MountManager::GetMountAppeID()
{
	if (this->IsOnMount())
	{
		return m_mount_param.temporary_imageid > 0 ? m_mount_param.temporary_imageid : m_mount_param.used_imageid;
	}

	return 0;
}

bool MountManager::ActiveImage(int imageid, bool recalc)
{
	if (imageid <= 0 || imageid >= MountParam::MAX_IMAGE_COUNT) return false;

	m_mount_param.ActiveImage(imageid);

	if (recalc)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);
	}

	return true;
}

bool MountManager::UseImage(int imageid)
{
	if (imageid < SPECIAL_IMG_START_ID)
	{
		if (!m_mount_param.IsImageActive(imageid))
		{
			m_role->NoticeNum(errornum::EN_MOUNT_IMG_NOT_ACTIVE);
			return false;
		}
	}
	else
	{
		if (!m_mount_param.IsSpecialImageActive(imageid - SPECIAL_IMG_START_ID))
		{
			m_role->NoticeNum(errornum::EN_MOUNT_IMG_NOT_ACTIVE);
			return false;
		}
	}

	m_mount_param.used_imageid = imageid;

	if (0 != m_mount_param.temporary_imageid)
	{
		m_mount_param.temporary_imageid = 0;
	}

	if (this->IsOnMount())
	{
		Protocol::SCMountAppeChange mac;
		mac.objid = m_role->GetId();
		mac.mount_appeid = m_mount_param.used_imageid;
		
		if (NULL != m_role->GetScene()) 
		{
			m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&mac, sizeof(mac));
		}

		m_role->RefreshApperance(false);
	}

	this->SendMountInfo();

	return true;
}

bool MountManager::UnUseImage(int imageid)
{
	if (imageid != m_mount_param.used_imageid)
	{
		return false;
	}

	m_mount_param.used_imageid = 0;

	if (this->IsOnMount())
	{
		Protocol::SCMountAppeChange mac;
		mac.objid = m_role->GetId();
		mac.mount_appeid = m_mount_param.used_imageid;

		if (NULL != m_role->GetScene()) 
		{
			m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&mac, sizeof(mac));
		}
	}

	this->SendMountInfo();

	return true;
}

bool MountManager::ActiveSpecialImage(int imageid, bool is_notify)
{
	if (m_mount_param.IsSpecialImageActive(imageid))
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_SPECIAL_IMG_ALREADY_ACTIVE);
		}

		return false;
	}

	const MountSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetMountOpConfig().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}
	if (m_role->GetProfLevel() < cfg_item->zhuanzhi_prof)
	{
		m_role->NoticeNum(errornum::EN_MOUNT_PROF_NOT_ENOUGH);
		return false;
	}

	m_mount_param.ActiveSpecialImage(imageid);
	m_role->GetImgFuling()->OnPutOnImg(JINGJIE_TYPE_MOUNT, imageid);
	if (0 == m_mount_param.special_img_grade_list[imageid])
	{
		m_mount_param.special_img_grade_list[imageid] = 1;
	}
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);
	JinjiesysFuncHelper::Instance().OnActiveSpecialImg(m_role, JINJIE_TYPE_MOUNT, imageid);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_MOUNT,imageid,true);
	this->UseImage(SPECIAL_IMG_START_ID + imageid);

	m_role->GetMountManager()->PrintMount("ActiveSpecialImage", imageid);
	return true;
}

bool MountManager::UnActiveSpecialImage(int imageid)
{
	if (!m_mount_param.IsSpecialImageActive(imageid))
	{
		return false;
	}

	const MountSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetMountOpConfig().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	BitmapFunction::ClearBit(m_mount_param.active_special_image_flag, sizeof(m_mount_param.active_special_image_flag), imageid);
	if (1 == m_mount_param.special_img_grade_list[imageid])
	{
		m_mount_param.special_img_grade_list[imageid] = 0;
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);

	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_MOUNT, imageid, false);
	
	if (this->UnUseImage(SPECIAL_IMG_START_ID + imageid))
	{
		this->UseImage(1); //默认再给一个
	}

	m_role->GetMountManager()->PrintMount("UnActiveSpecialImage", imageid);
	return true;
}

bool MountManager::IsActiveSpecialImage(int imageid)
{
	if (m_mount_param.IsSpecialImageActive(imageid))
	{
		return true;
	}

	return false;
}

void MountManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
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
			const MountLevelConfig *level_cfg = LOGIC_CONFIG->GetMountOpConfig().GetLevelConfig(m_mount_param.level);
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
			const MountGradeConfig * grade_config = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(m_mount_param.grade);
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
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += grade_config->per_jingzhun;
				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += grade_config->per_baoji;
// 				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += grade_config->jianshang_per;
// 				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += grade_config->zengshang_per;
// 				grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += grade_config->pvp_jianshang_per;
// 				grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += grade_config->pvp_zengshang_per;
// 				grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += grade_config->pve_jianshang_per;
// 				grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += grade_config->pve_zengshang_per;

				// 下面特殊属性用乘法
				grade_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, grade_config->jianshang_per);
				grade_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, grade_config->zengshang_per);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, grade_config->pvp_zengshang_per);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, grade_config->pve_zengshang_per);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, grade_config->pvp_jianshang_per);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, grade_config->pve_jianshang_per);

				// 大目标特殊形象万分比
				int special_img_per_add = JinjieSysRewardMgr::Instance().GetJinjieSpecialImgExtraAddPer(m_role, JINJIE_TYPE_MOUNT);
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
				if (this->IsOnMount()) // 上了坐骑才加移动速度
				{
					base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED] += grade_config->movespeed;
				}

				// 仙尊卡加成
				{
					const int add_grade_attrs_per = m_role->GetXianZunKa()->GetAddMountGradeAttrsPer();
					for (int i = CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX; ++i)
					{
						if (CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED == i) continue;

						//grade_attrs.m_attrs[i] += static_cast<Attribute>(grade_attrs.m_attrs[i] * (ROLE_ATTR_PER * add_grade_attrs_per));
						grade_attrs.AddValue(i, static_cast<Attribute>(grade_attrs.m_attrs[i] * (ROLE_ATTR_PER * add_grade_attrs_per)));
					}
				}

				base_attrs.Add(grade_attrs.m_attrs);

				//天赋加成
				m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_MOUNT_BASE, grade_attrs, m_attrs_add);

				//勋章增加形象进阶基础属性
				this->CalcBaseAttrAddFromSkill(m_attrs_add);
			}
		}

		m_attrs_add.Add(base_attrs.m_attrs);

		//天赋加成
		m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_MOUNT, m_attrs_add, m_attrs_add);

		{
			// 坐骑装备
			int active_equip_grade = LOGIC_CONFIG->GetMountOpConfig().GetActiveEquipGrade();
			if (active_equip_grade <= m_mount_param.grade)
			{
				for (int i = 0; i < MountParam::EQUIP_COUNT; ++ i)
				{
					const MountEquipConifg *equip_cfg = LOGIC_CONFIG->GetMountOpConfig().GetEquipCfg(i);
					if (NULL != equip_cfg)
					{
						int level = m_mount_param.equip_level_list[i];
						if (level >= 0 && level < equip_cfg->max_level)
						{
							rate += equip_cfg->level_cfg[level].add_percent;
						}
					}
				}
			}

			{
				// 至宝加成
				rate += m_role->GetZhiBaoManager()->GetMountAddPer();

				// 勋章加成
				rate += m_role->GetXunZhangManager()->GetMountAddPer();

				// 星座星魂加成
				int xinhun_add_per = 0;
				m_role->GetChineseZodiac()->GetXingHunTotalAddPer(&xinhun_add_per, NULL, NULL, NULL, NULL, NULL, NULL);
				rate += xinhun_add_per;

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

			if (active_equip_grade <= m_mount_param.grade)
			{
				for (int i = 0; i < MountParam::EQUIP_COUNT; ++ i)
				{
					const MountEquipConifg *equip_cfg = LOGIC_CONFIG->GetMountOpConfig().GetEquipCfg(i);
					if (NULL != equip_cfg)
					{
						int level = m_mount_param.equip_level_list[i];
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
			for (int i = 0; i < MountParam::MAX_SPECIAL_IMAGE_COUNT; ++ i)
			{
				if (m_mount_param.IsSpecialImageActive(i))
				{
	
					// 特殊形象进阶
					const int grade = m_mount_param.special_img_grade_list[i];

					const MountSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetSpecialImgUpgradeCfg(i, grade);
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
// 						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += upgrade_cfg->jianshang_per;
// 						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += upgrade_cfg->zengshang_per;
// 						m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += upgrade_cfg->pvp_jianshang_per;
// 						m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += upgrade_cfg->pvp_zengshang_per;
// 						m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += upgrade_cfg->pve_jianshang_per;
// 						m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += upgrade_cfg->pve_zengshang_per;

						// 下面特殊属性用乘法
						m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, upgrade_cfg->jianshang_per);
						m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, upgrade_cfg->zengshang_per);
						m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, upgrade_cfg->pvp_zengshang_per);
						m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, upgrade_cfg->pve_zengshang_per);
						m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, upgrade_cfg->pvp_jianshang_per);
						m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, upgrade_cfg->pve_jianshang_per);
						m_attrs_add.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] +=  static_cast<float>(upgrade_cfg->per_gongji * ROLE_ATTR_PER);
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
				const int num = m_mount_param.shuxingdan_list[slot_index];
				if (num > 0)
				{
					const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_MOUNT, slot_index);
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
			for (int i = 0; i < MountParam::SKILL_COUNT; i++)
			{
				if (m_mount_param.skill_level_list[i] > 0)
				{
					const MountSkillConifg *skill_cfg = LOGIC_CONFIG->GetMountOpConfig().GetSkillCfg(i, m_mount_param.skill_level_list[i]);
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

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_MOUNT, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_MOUNT, m_other_capability);

	m_mount_param.capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_MOUNT);

	//if (this->IsOnMount())		// 如果骑乘就全加
	//{
	//	base_add.Add(m_attrs_add.m_attrs);
	//}
	//else
	//{
	//	for (int i = CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP; i <= CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN; ++i)
	//	{
	//		base_add.m_attrs[i] += m_attrs_add.m_attrs[i];
	//	}
	//}
	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);

}

bool MountManager::IsOnMount()
{
	return m_mount_param.IsOnMount();
}

MountParam * MountManager::GetMountParam()
{
	return &m_mount_param;
}

void MountManager::PrintMount(const char *reason, int param1, int param2, int param3, int param4, int param5)
{
	UNSTD_STATIC_CHECK(4 == MountParam::EQUIP_COUNT);
	UNSTD_STATIC_CHECK(4 == MountParam::SKILL_COUNT);
	const int image_hex_len = MountParam::MAX_ACTIVE_IMAGE_BYTE * 2 + 1;
	const int special_image_hex_len = MountParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE * 2 + 1;
	char active_image_hex[image_hex_len] = { 0 };
	char active_special_image_hex[special_image_hex_len] = { 0 };
	BinToHex((const char*)m_mount_param.active_image_flag, sizeof(m_mount_param.active_image_flag), active_image_hex);
	BinToHex((const char*)m_mount_param.active_special_image_flag, sizeof(m_mount_param.active_special_image_flag), active_special_image_hex);
	gamelog::g_log_mount.printf(LL_INFO, "Mount role(%d,%s) %s(%d,%d,%d,%d,%d) "
		"flag:%d, level:%d, grade:%d,%d, used_imageid:%d zizhidan_count:%d chengzhangdan_count:%d image_flag_hex:%s, special_image_flag:%s"
		"equip_level_list:%d, %d, %d, %d"
		"skill_level_list:%d, %d, %d, %d",
		m_role->GetUID(), m_role->GetName(),
		reason, param1, param2, param3, param4, param5,
		(int)m_mount_param.mount_flag, (int)m_mount_param.level,
		(int)m_mount_param.grade, (int)m_mount_param.grade_bless_val,
		(int)m_mount_param.used_imageid,
		(int)m_mount_param.shuxingdan_list[0], (int)m_mount_param.shuxingdan_list[1],
		active_image_hex, active_special_image_hex,
		(int)m_mount_param.equip_level_list[0], (int)m_mount_param.equip_level_list[1], (int)m_mount_param.equip_level_list[2], (int)m_mount_param.equip_level_list[3],
		(int)m_mount_param.skill_level_list[0], (int)m_mount_param.skill_level_list[1], (int)m_mount_param.skill_level_list[2], (int)m_mount_param.skill_level_list[3]);
}

void MountManager::OnCompleteTask(TaskID task_id)
{
//	if (LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenTaskID(FUN_OPEN_TYPE_MOUNT) == task_id)
//	{
//		MountFunction::ActiveMount(m_role);
//	}
	this->CheckActive();
}

void MountManager::OnRoleUpLevel(int level)
{
	this->CheckActive();
}

bool MountManager::OnUseShuxingdan(int slot_idx, short &use_count)
{
	if (slot_idx < 0 || slot_idx >= SHUXINGDAN_MAX_TYPE || use_count <= 0)
	{
		return false;
	}

	const MountGradeConfig *grade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(m_mount_param.grade);
	if (NULL == grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_MOUNT_SHUXINGDAN_USE_LIMIT_NUM);
		return false;
	}

	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_MOUNT, slot_idx);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_mount_param.grade < cfg_item->order_limit)
	{
		gstr::SendError(m_role, "shuxingdan_use_order_limit_1", SHUXINGDAN_TYPE_MOUNT, cfg_item->order_limit - 1, slot_idx); // 客户端阶数-1
		return false;
	}

	if (m_mount_param.shuxingdan_list[slot_idx] + use_count > cfg_item->use_limit_num)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	//int shuxingdan_limit = (slot_idx == SHUXINGDAN_SLOT_TYPE_ZIZHI) ? grade_cfg->shuxingdan_limit : grade_cfg->chengzhangdan_limit;
	int special_img_add_count = 0;
	for (int i = 0;i < MountParam::MAX_SPECIAL_IMAGE_COUNT; i++)
	{
		if (!m_mount_param.IsSpecialImageActive(i))
			continue;
		
		const MountSpecialImgUpgradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetSpecialImgUpgradeCfg(i,m_mount_param.special_img_grade_list[i]);
		if (NULL != special_img_grade_cfg)
		{
			int shuxingdan_add_count = (slot_idx == SHUXINGDAN_SLOT_TYPE_ZIZHI) ? special_img_grade_cfg->shuxingdan_count : special_img_grade_cfg->chengzhangdan_count;
			special_img_add_count += shuxingdan_add_count;
		}
	}

	// 策划需求，修正真正使用数量
	int can_use_count = grade_cfg->shuxingdan_limit + special_img_add_count - m_mount_param.shuxingdan_list[slot_idx];
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

	m_mount_param.shuxingdan_list[slot_idx] += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);
	this->SendMountInfo();

	return true;
}

bool MountManager::UseTemporaryImage(int imageid)
{
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (0 != m_mount_param.temporary_imageid_invalid_time && now_sec > m_mount_param.temporary_imageid_invalid_time)
	{
		m_role->NoticeNum(errornum::EN_MOUNT_TEMPORARY_IMAGE_TIME_LIMIT);
		return false;
	}

	if(0 != imageid && 0 != m_mount_param.temporary_imageid_has_select && imageid != m_mount_param.temporary_imageid_has_select)
	{
		return false;
	}

	m_mount_param.temporary_imageid = imageid;

	if (0 != imageid && 0 == m_mount_param.temporary_imageid_has_select)
	{
		m_mount_param.temporary_imageid_has_select = imageid;
	}

	if (0 != imageid && 0 == m_mount_param.temporary_imageid_invalid_time)
	{
		m_mount_param.temporary_imageid_invalid_time = now_sec + LOGIC_CONFIG->GetMountOpConfig().GetTemporaryImageValidTime();
	}

	if (this->IsOnMount())
	{
		Protocol::SCMountAppeChange mac;
		mac.objid = m_role->GetId();
		mac.mount_appeid = m_mount_param.temporary_imageid > 0 ? m_mount_param.temporary_imageid : m_mount_param.used_imageid;

		if (NULL != m_role->GetScene()) 
		{
			m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&mac, sizeof(mac));
		}

		m_role->RefreshApperance(false);
	}

	this->SendMountInfo();

	return true;
}

bool MountManager::MountCanFly()
{
	return true;
	return false;
// 	if (!LOGIC_CONFIG->GetMountOpConfig().MountCanFly(m_mount_param.flyup_use_imageid))
// 	{
// 		return false;
// 	}
// 	
// 	if (!m_role->GetScene()->InCommonScene() || m_role->GetScene()->InCangBaoMiJing() || m_role->GetScene()->InShiFangShiJie() || m_role->GetScene()->InQinYuanScene())
// 	{
// 		m_role->NoticeNum(errornum::EN_MOUNT_CUR_SCENE_CAN_NOT_FLY);
// 		return false;
// 	}
// 
// 	if (m_role->GetMultiMount()->IsOnMount())
// 	{
// 		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_CANNOT_FLY);
// 		return false;
// 	}
// 
// 	return true;
}


int MountManager::RefreshMountMinEquipLevel()                          
{
	int min = m_mount_param.equip_level_list[0];

	for (int i = 1; i < MountParam::EQUIP_COUNT; ++i)
	{
		min = m_mount_param.equip_level_list[i] < min ? m_mount_param.equip_level_list[i] : min;
	}

	return min;
}

void MountManager::CheckActive()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (!m_mount_param.IsMountActived())
	{
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_MOUNT);
		if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day)
		{
			if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
			{
				MountFunction::ActiveMount(m_role,false);
			}
		}
	}
}

// 幻化特殊战力加成
void MountManager::HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add)
{
	if (!LOGIC_CONFIG->GetMountOpConfig().IsAddSpecialCap())
	{
		return;
	}

	auto cfg_item = LOGIC_CONFIG->GetMountOpConfig().GetHuanHuaSpecialCapAddCfg(huanhua_id);
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

int MountManager::GetSpecialImageLevelById(int imageid)
{
	if (imageid <= 0 || imageid >= MountParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return 0;
	}

	if (!m_mount_param.IsSpecialImageActive(imageid))
	{
		return 0;
	}

	return m_mount_param.special_img_grade_list[imageid];
}

int MountManager::GetGradeBlessVal()
{
	return m_mount_param.grade_bless_val;
}

void MountManager::CalcBaseAttrAddFromSkill(CharIntAttrs & base_add)
{
	// 阶数加成
	const MountGradeConfig * grade_config = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(m_mount_param.grade);
	if (NULL == grade_config)
	{
		return;
	}

	int total_add_per = 0;
	for (int i = 0; i < MountParam::SKILL_COUNT; i++)
	{
		if (m_mount_param.skill_level_list[i] <= 0)
		{
			continue;
		}
		const MountSkillConifg *skill_cfg = LOGIC_CONFIG->GetMountOpConfig().GetSkillCfg(i, m_mount_param.skill_level_list[i]);
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

int MountManager::GetMedalSkillLevel()
{
	short medal_skill_index = LOGIC_CONFIG->GetMountOpConfig().GetMedalSkillIndex();
	if (medal_skill_index < 0 || medal_skill_index >= (short)static_array_size(m_mount_param.skill_level_list))
	{
		return 0;
	}
	return m_mount_param.skill_level_list[medal_skill_index];
}

int MountManager::GetGrade()
{
	if (!m_mount_param.IsMountActived())
	{
		return 0;
	}
	return m_mount_param.grade;
}
