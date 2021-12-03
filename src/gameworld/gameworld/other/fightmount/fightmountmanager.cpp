#include "fightmountmanager.hpp"

#include "gamelog.h"
#include "engineadapter.h"

#include "obj/character/role.h"

#include "servercommon/errornum.h"
#include "protocal/msgfightmount.hpp"

#include "scene/scene.h"
#include "fightmountconfig.hpp"
#include "gstr.h"

#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/shuxingdanconfig.hpp"
#include "servercommon/vipdef.hpp"
#include "other/fightmount/fightmountfunction.hpp"
#include "globalconfig/globalconfig.h"
#include "global/worldstatus/worldstatus.hpp"
#include "serverlogic.h"
#include "task/taskrecorder.h"
#include "other/capability/capability.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/multimount/multimount.hpp"
#include "other/chinesezodiac/chinesezodiacconfig.hpp"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include "other/imgfuling/imgfuling.hpp"
#include "other/talent/talent.hpp"
#include "servercommon/imgfulingdef.hpp"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "other/jinjiesysreward/jinjiesysrewardmgr.hpp"
#include "other/dressingroom/dressingroomconfig.hpp"
#include "other/dressingroom/dressingroommgr.hpp"
#include "other/mount/mountmanager.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "common/bintohex.h"
#include "servercommon/serverconfig/crossconfig.hpp"

FightMountManager::FightMountManager()
	: m_role(NULL), m_last_skill_time(0),m_other_capability(0)
{
}

FightMountManager::~FightMountManager()
{

}

void FightMountManager::Init(Role *role, const FightMountParam &mount_param)
{
	m_role = role;
	m_mount_param = mount_param;

	if (mount_param.old_grade_bless_val != 0)
	{
		m_mount_param.grade_bless_val = mount_param.old_grade_bless_val;
		m_mount_param.old_grade_bless_val = 0;
	}
}

void FightMountManager::GetOtherInitParam(FightMountParam *param)
{
	*param = m_mount_param;
}

void FightMountManager::GetRoleAppearance(RoleAppearance *appearance)
{
	if (NULL != appearance)
	{
		appearance->fight_mount_used_imageid = m_mount_param.used_imageid;
	}
}

void FightMountManager::OnLogin()
{
	this->CheckActive();
}

void FightMountManager::OnRoleLevelChange(int level)
{
	m_mount_param.level = level;

	if (m_mount_param.IsFightMountActived())
	{
		this->SendFightMountInfo();
	}
}

void FightMountManager::Update(time_t now_second)
{
	//如果清零时间大于等于第二天的5点且战斗坐骑阶级大于等于5就清零
	const FightMountGradeConfig *grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(m_mount_param.grade);
	if (nullptr != grade_cfg && 1 == grade_cfg->is_clear_bless && now_second >= m_mount_param.clear_upgrade_time)
	{
		m_mount_param.grade_bless_val = 0;
		m_mount_param.clear_upgrade_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + EngineAdapter::Instance().NextDayInterval(5, 0, 0);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);
		this->SendFightMountInfo();
	}
}

void FightMountManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->CheckActive();
	}
}

// void FightMountManager::OnHurtOther(Character *target, int skill_id, long long &injure, int target_index, int &passive_flag)
// {
// 	if (m_mount_param.skill_level_list[0] > 0)
// 	{
// 		if (0 == target_index)
// 		{
// 			const FightMountSkillConifg *skill_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSkillCfg(0, m_mount_param.skill_level_list[0]);
// 			if (NULL != skill_cfg)
// 			{
// 				if (ServerLogic::GetInstServerLogic()->GetPiTime() < m_last_skill_time + skill_cfg->param_a)
// 				{
// 					return;
// 				}
// 
// 				m_last_skill_time = ServerLogic::GetInstServerLogic()->GetPiTime();
// 
// 				if (RandomNum(100) >= (skill_cfg->param_b * ROLE_ATTR_PER) * 100)
// 				{
// 					return;
// 				}
// 
// 				int rate = skill_cfg->param_c;
// 				passive_flag |= (1 << EffectBase::PASSIVE_FLAG_MOUNT_JIANTA);
// 
// 				UNSTD_STATIC_CHECK(FightMountParam::SKILL_COUNT >= 4);
// 				const FightMountSkillConifg *skill3_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSkillCfg(3, m_mount_param.skill_level_list[3]);
// 				if (NULL != skill3_cfg)
// 				{
// 					rate += skill3_cfg->param_a;
// 					passive_flag |= (1 << EffectBase::PASSIVE_FLAG_MOUNT_NUTA);
// 				}
// 
// 				injure += (int)(injure * (rate * ROLE_ATTR_PER));
// 			}
// 		}
// 	}
// }

// 受到伤害时，有param_b万分比概率反弹该次伤害的param_c万分比。 冷却param_a毫秒
void FightMountManager::OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{
	static const int SKILL_ID0 = 0;
	UNSTD_STATIC_CHECK(SKILL_ID0 < FightMountParam::SKILL_COUNT);
	if (delta_hp >= 0) return;
	
	Scene *scene = m_role->GetScene();
	if (NULL == scene) return;

	Obj* obj = scene->GetObj(attacker);
	if (NULL == obj) return;
	if (Obj::OBJ_TYPE_ROLE != obj->GetObjType() && Obj::OBJ_TYPE_MONSTER != obj->GetObjType()) return;

	Character *target = dynamic_cast<Character*>(obj);
	if (NULL == target) return;

	if (m_mount_param.skill_level_list[SKILL_ID0] > 0)
	{
		const FightMountSkillConifg *skill_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSkillCfg(SKILL_ID0, m_mount_param.skill_level_list[SKILL_ID0]);
		if (NULL != skill_cfg)
		{
			unsigned long long now_sec_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();
			if (now_sec_ms < m_mount_param.skill0_colldown_time)
			{
				return;
			}

			if (RandomNum(10000) >= skill_cfg->param_b)
			{
				return;
			}

			// 反伤
			if (from_skill)
			{
				Attribute rebound_hp = static_cast<Attribute>(delta_hp * (skill_cfg->param_c * ROLE_ATTR_PER));
				EffectOnce *effect = new EffectOnce(m_role->GetId(), 0, rebound_hp, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_REBOUNDHURT);
				target->AddEffect(effect);
			}

			m_mount_param.skill0_colldown_time = now_sec_ms + skill_cfg->param_a;
		}
	}
}

bool FightMountManager::GoonFightMount(bool goon_mount, bool notify, bool is_force)
{
	if (Role::MOVE_MODE_FLY == m_role->GetMoveMode())
	{
		//if (notify) m_role->NoticeNum(errornum::EN_FIGHT_MOUNT_CANNOT_GOON_IN_FLY);
		return false;
	}

	bool old_is_mount = this->IsOnFightMount();

	if (goon_mount) // 上坐骑
	{
		if (old_is_mount)
		{
			//if (notify) m_role->NoticeNum(errornum::EN_MOUNT_ALREADY_ON_MOUNT);
			return false;
		}

		if (!is_force && !m_role->GetRoleStatusManager()->CanFightMountOn())
		{
			//if (notify) m_role->NoticeNum(errornum::EN_MOUNT_CANNOT_MOUNTON);
			return false;
		}

		if (m_role->GetMultiMount()->IsOnMount())
		{
			m_role->GetMultiMount()->MountOn(false);
		}

		if (m_role->GetMountManager()->IsOnMount())
		{
			m_role->GetMountManager()->GoonMount(false);
		}

		m_mount_param.OnFightMount();
	}
	else // 下坐骑
	{
		if (!old_is_mount)
		{
			//if (notify) m_role->NoticeNum(errornum::EN_MOUNT_NOT_ON_MOUNT);
			return false;
		}

		m_mount_param.OffFightMount();
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);

	this->SendFightMountInfo();

	{
		Protocol::SCFightMountAppeChange mac;
		mac.objid = m_role->GetId();
		mac.mount_appeid = this->GetFightMountAppeID();

		if (NULL != m_role->GetScene()) 
		{
			m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&mac, sizeof(mac));
		}
	}

	return true;
}

void FightMountManager::SendFightMountInfo()
{
	Protocol::SCFightMountInfo mi;
	mi.mount_flag = m_mount_param.mount_flag;
	mi.mount_level = m_mount_param.level;
	mi.grade = m_mount_param.grade;
	mi.star_level = m_mount_param.star_level;
	mi.clear_upgrade_time = m_mount_param.clear_upgrade_time;
	mi.used_imageid = m_mount_param.used_imageid;
	UNSTD_STATIC_CHECK(sizeof(mi.active_image_flag) == sizeof(m_mount_param.active_image_flag));
	memcpy(mi.active_image_flag, m_mount_param.active_image_flag, sizeof(mi.active_image_flag));
	mi.shuxingdan_count = m_mount_param.shuxingdan_count;
	mi.chengzhangdan_count = m_mount_param.chengzhangdan_count;
	mi.grade_bless_val = m_mount_param.grade_bless_val;
	UNSTD_STATIC_CHECK(sizeof(mi.active_special_image_flag) == sizeof(m_mount_param.active_special_image_flag));
	memcpy(mi.active_special_image_flag, m_mount_param.active_special_image_flag, sizeof(mi.active_special_image_flag));
	mi.equip_skill_level = m_mount_param.equip_skill_level;
	memcpy(mi.equip_level_list, m_mount_param.equip_level_list, sizeof(mi.equip_level_list));
	memcpy(mi.skill_level_list, m_mount_param.skill_level_list, sizeof(mi.skill_level_list));
	memcpy(mi.special_img_grade_list, m_mount_param.special_img_grade_list, sizeof(mi.special_img_grade_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&mi, sizeof(mi));
}

int FightMountManager::GetFightMountAppeID()
{
	if (this->IsOnFightMount())
	{
		return m_mount_param.used_imageid;
	}

	return 0;
}

bool FightMountManager::ActiveImage(int imageid, bool recalc)
{
	if (imageid <= 0 || imageid >= FightMountParam::MAX_IMAGE_COUNT) return false;

	m_mount_param.ActiveImage(imageid);

	if (recalc)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);
	}

	return true;
}

bool FightMountManager::UseImage(int imageid)
{
	if (imageid < SPECIAL_IMG_START_ID)
	{
		if (!m_mount_param.IsImageActive(imageid))
		{
			m_role->NoticeNum(errornum::EN_FIGHT_MOUNT_IMG_NOT_ACTIVE);
			return false;
		}
	}
	else
	{
		if (!m_mount_param.IsSpecialImageActive(imageid - SPECIAL_IMG_START_ID))
		{
			m_role->NoticeNum(errornum::EN_FIGHT_MOUNT_IMG_NOT_ACTIVE);
			return false;
		}
	}

	m_mount_param.used_imageid = imageid;

	if (this->IsOnFightMount())
	{
		Protocol::SCFightMountAppeChange mac;
		mac.objid = m_role->GetId();
		mac.mount_appeid = m_mount_param.used_imageid;
		
		if (NULL != m_role->GetScene()) 
		{
			m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&mac, sizeof(mac));
		}

		m_role->RefreshApperance(false);
	}

	this->SendFightMountInfo();

	return true;
}

bool FightMountManager::UnUseImage(int imageid)
{
	if (imageid != m_mount_param.used_imageid)
	{
		return false;
	}

	m_mount_param.used_imageid = 0;

	if (this->IsOnFightMount())
	{
		Protocol::SCFightMountAppeChange mac;
		mac.objid = m_role->GetId();
		mac.mount_appeid = m_mount_param.used_imageid;

		if (NULL != m_role->GetScene()) 
		{
			m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&mac, sizeof(mac));
		}
	}

	this->SendFightMountInfo();

	return true;
}

bool FightMountManager::ActiveSpecialImage(int imageid, bool is_notify)
{
	if (m_mount_param.IsSpecialImageActive(imageid))
	{
		if (is_notify)
		{
			FightMountFunction::FightMountSpecialImgUpgrade(m_role, imageid);
		}
		
		return false;
	}

	const FightMountSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetFightMountOpConfig().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_role->GetProfLevel() < cfg_item->zhuanzhi_prof)
	{
		m_role->NoticeNum(errornum::EN_FIGHT_MOUNT_PROF_NOT_ENOUGH);
		return false;
	}

	m_mount_param.ActiveSpecialImage(imageid);
	m_role->GetImgFuling()->OnPutOnImg(JINGJIE_TYPE_FIGHT_MOUNT, imageid);
	if (0 == m_mount_param.special_img_grade_list[imageid])
	{
		m_mount_param.special_img_grade_list[imageid] = 1;
	}
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);
	JinjiesysFuncHelper::Instance().OnActiveSpecialImg(m_role, JINJIE_TYPE_FIGHT_MOUNT, imageid);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_FIGHT_MOUNT, imageid, true);
	this->UseImage(SPECIAL_IMG_START_ID + imageid);
	return true;
}

bool FightMountManager::UnActiveSpecialImage(int imageid)
{
	if (!m_mount_param.IsSpecialImageActive(imageid))
	{
		return false;
	}

	const FightMountSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetFightMountOpConfig().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	BitmapFunction::ClearBit(m_mount_param.active_special_image_flag, sizeof(m_mount_param.active_special_image_flag), imageid);
	if (1 == m_mount_param.special_img_grade_list[imageid])
	{
		m_mount_param.special_img_grade_list[imageid] = 0;
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_FIGHT_MOUNT, imageid, false);
	if (this->UnUseImage(SPECIAL_IMG_START_ID + imageid))
	{
		this->UseImage(1); //默认再给一个
	}

	return true;
}

void FightMountManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
//	if (!m_mount_param.IsFightMountActived())
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
			const FightMountLevelConfig *level_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetLevelConfig(m_mount_param.level);
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
			const FightMountGradeConfig * grade_config = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(m_mount_param.grade);
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

				// 下面特殊属性用乘法
// 				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += grade_config->per_mianshang;
// 				grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += grade_config->per_pofang;
// 				grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += grade_config->pvp_zengshang_per;
// 				grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += grade_config->pve_zengshang_per;
// 				grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += grade_config->pvp_jianshang_per;
// 				grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += grade_config->pve_jianshang_per;
				grade_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, grade_config->per_mianshang);
				grade_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, grade_config->per_pofang);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, grade_config->pvp_zengshang_per);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, grade_config->pve_zengshang_per);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, grade_config->pvp_jianshang_per);
				grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, grade_config->pve_jianshang_per);

				//天赋加成
				m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_FIGHT_MOUNT_BASE, grade_attrs, m_attrs_add);

				this->CalcBaseAttrAddFromSkill(m_attrs_add);

				if (this->IsOnFightMount()) // 没有上双人坐骑，才按这边坐骑的速度来算
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED] += grade_config->movespeed;
				}

				// 大目标特殊形象万分比
				int special_img_per_add = JinjieSysRewardMgr::Instance().GetJinjieSpecialImgExtraAddPer(m_role, JINJIE_TYPE_FIGHT_MOUNT);
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
				//if (m_mount_param.grade_bless_val > 0)
				//{
				//	const FightMountGradeConfig * next_grade_config = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(m_mount_param.grade + 1);
				//	const FightMountGradeConfig * curr_grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(m_mount_param.grade);
				//	if (NULL != next_grade_config && NULL != curr_grade_cfg && curr_grade_cfg->bless_val_limit > 0) 
				//	{
				//		double rate = (((double)m_mount_param.grade_bless_val) / curr_grade_cfg->bless_val_limit) * (next_grade_config->bless_addition * ROLE_ATTR_PER);
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

//		{
//			// 星级加成  
//			const FightMountUpStarExpCfg * star_level_config = LOGIC_CONFIG->GetFightMountOpConfig().GetUpStarExpCfg(m_mount_param.star_level);
//			if (NULL != star_level_config) 
//			{
//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += star_level_config->maxhp;
//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += star_level_config->gongji;
//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += star_level_config->fangyu;
//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += star_level_config->mingzhong;
//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += star_level_config->shanbi;
//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += star_level_config->baoji;
//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += star_level_config->jianren;
//			}
//
//			if (!m_role->GetMultiMount()->IsOnMount()) // 没有上双人坐骑，才按这边坐骑的速度来算
//			{
//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED] += star_level_config->movespeed;
//			}

//		}

		{
			// 装备加成
			int active_equip_grade = LOGIC_CONFIG->GetFightMountOpConfig().GetActiveEquipGrade();
			if (active_equip_grade <= m_mount_param.grade)
			{
				for (int i = 0; i < FightMountParam::EQUIP_COUNT; ++ i)
				{
					const FightMountEquipConifg *equip_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetEquipCfg(i);
					if (NULL != equip_cfg)
					{
						int level = m_mount_param.equip_level_list[i];
						if (level >= 0 && level < equip_cfg->max_level)
						{
							rate += equip_cfg->equip_level_cfg_list[level].add_percent;
						}
					}	
				}

				{
					// 星座星魂加成
					int xinhun_add_per = 0;
					m_role->GetChineseZodiac()->GetXingHunTotalAddPer(NULL, NULL, NULL, NULL, NULL, &xinhun_add_per, NULL);
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

				for (int i = 0; i < FightMountParam::EQUIP_COUNT; ++ i)
				{
					const FightMountEquipConifg *equip_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetEquipCfg(i);
					if (NULL != equip_cfg)
					{
						int level = m_mount_param.equip_level_list[i];
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
			for (int i = 0; i < FightMountParam::MAX_SPECIAL_IMAGE_COUNT; ++ i)
			{
				if (m_mount_param.IsSpecialImageActive(i))
				{
	
					// 特殊形象进阶
					const int grade = m_mount_param.special_img_grade_list[i];

					const FightMountSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSpecialImgUpgradeCfg(i, grade);
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
			if (m_mount_param.shuxingdan_count > 0)
			{
				const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_MOUNT, 0);
				if (NULL != cfg_item)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp * m_mount_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji * m_mount_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu * m_mount_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg_item->mingzhong * m_mount_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg_item->shanbi * m_mount_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg_item->baoji * m_mount_param.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg_item->jianren * m_mount_param.shuxingdan_count;
				}
			}
		}

		{
			// 技能属性
			for (int i = 0; i < FightMountParam::SKILL_COUNT; i++)
			{
				if (m_mount_param.skill_level_list[i] > 0)
				{
					const FightMountSkillConifg *skill_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSkillCfg(i, m_mount_param.skill_level_list[i]);
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

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_FIGHT_MOUNT, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_FIGHT_MOUNT, m_other_capability);

	m_mount_param.capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_FIGHT_MOUNT);

	base_add.Add(m_attrs_add.m_attrs, true);
	//if (this->IsOnFightMount())		// 如果骑乘就全加
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
}

bool FightMountManager::IsOnFightMount()
{
	return m_mount_param.IsOnFightMount();
}

FightMountParam * FightMountManager::GetFightMountParam()
{
	return &m_mount_param;
}

void FightMountManager::PrintFightMount(const char *reason, int param1, int param2, int param3, int param4, int param5)
{
	UNSTD_STATIC_CHECK(4 == FightMountParam::EQUIP_COUNT);
	UNSTD_STATIC_CHECK(4 == FightMountParam::SKILL_COUNT);
	const int image_hex_len = FightMountParam::MAX_ACTIVE_IMAGE_BYTE * 2 + 1;
	const int special_image_hex_len = FightMountParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE * 2 + 1;
	char active_image_hex[image_hex_len] = { 0 };
	char active_special_image_hex[special_image_hex_len] = { 0 };
	BinToHex((const char*)m_mount_param.active_image_flag, sizeof(m_mount_param.active_image_flag), active_image_hex);
	BinToHex((const char*)m_mount_param.active_special_image_flag, sizeof(m_mount_param.active_special_image_flag), active_special_image_hex);
	gamelog::g_log_mount.printf(LL_INFO, "FightMount role(%d,%s) %s(%d,%d,%d,%d,%d) "
		"flag:%d, level:%d, grade:%d,%d, used_imageid:%d shuxingdan_count:%d chengzhangdan_count:%d, image_flag:%s, special_image_flag:%s"
		"equip_info_list:%d, %d, %d, %d"
		"skill_level_list:%d, %d, %d, %d",
		m_role->GetUID(), m_role->GetName(),
		reason, param1, param2, param3, param4, param5,
		(int)m_mount_param.mount_flag, (int)m_mount_param.level,
		(int)m_mount_param.grade, (int)m_mount_param.grade_bless_val,
		(int)m_mount_param.used_imageid,
		(int)m_mount_param.shuxingdan_count, (int)m_mount_param.chengzhangdan_count,
		active_image_hex, active_special_image_hex,
		(int)m_mount_param.equip_level_list[0], (int)m_mount_param.equip_level_list[1],
		(int)m_mount_param.equip_level_list[2], (int)m_mount_param.equip_level_list[3],
		(int)m_mount_param.skill_level_list[0], (int)m_mount_param.skill_level_list[1], (int)m_mount_param.skill_level_list[2], (int)m_mount_param.skill_level_list[3]);
}

void FightMountManager::OnCompleteTask(TaskID task_id)
{
	this->CheckActive();
//	if (LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenTaskID(FUN_OPEN_TYPE_FIGHT_MOUNT) == task_id)
//	{
//		FightMountFunction::ActiveFightMount(m_role);
//	}
}

void FightMountManager::OnRoleUpLevel(int level)
{
	this->CheckActive();
//	if (m_mount_param.IsFightMountActived()) return;
//
//	if (level >= LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenRoleLevel(FUN_OPEN_TYPE_FIGHT_MOUNT))
//	{
//		FightMountFunction::ActiveFightMount(m_role);
//	}
}

bool FightMountManager::OnUseShuxingdan(int slot_idx, short &use_count)
{
	if (slot_idx < 0 || slot_idx >= SHUXINGDAN_MAX_TYPE || use_count <= 0)
	{
		return false;
	}

	const FightMountGradeConfig *grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(m_mount_param.grade);
	if (NULL == grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_FIGHTMOUNT_SHUXINGDAN_USE_LIMIT_NUM);
		return false;
	}

	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_MOUNT, 0);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_mount_param.grade < cfg_item->order_limit)
	{
		gstr::SendError(m_role, "shuxingdan_use_order_limit_1", SHUXINGDAN_TYPE_FIGHTMOUNT, cfg_item->order_limit - 1, slot_idx); // 客户端阶数-1
		return false;
	}

	if (m_mount_param.shuxingdan_count + use_count > cfg_item->use_limit_num)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	int special_img_add_count = 0;
	for (int i = 0;i < FightMountParam::MAX_SPECIAL_IMAGE_COUNT; i++)
	{
		if (!m_mount_param.IsSpecialImageActive(i))
			continue;
		
		const FightMountSpecialImgUpgradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSpecialImgUpgradeCfg(i,m_mount_param.special_img_grade_list[i]);
		if (NULL != special_img_grade_cfg)
		{
			special_img_add_count += special_img_grade_cfg->shuxingdan_count;
		}
	}

	// 策划需求，修正真正使用数量
	int can_use_count = grade_cfg->shuxingdan_limit + special_img_add_count - m_mount_param.shuxingdan_count;
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

	m_mount_param.shuxingdan_count += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);
	this->SendFightMountInfo();

	return true;
}

bool FightMountManager::OnUseChengzhangdan(int use_count)
{
	const FightMountGradeConfig *grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(m_mount_param.grade);
	if (NULL == grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_CHENGZHANGDAN_USE_LIMIT_NUM);
		return false;
	}

	int special_img_add_count = 0;
	for (int i = 0;i < FightMountParam::MAX_SPECIAL_IMAGE_COUNT; i++)
	{
		if (!m_mount_param.IsSpecialImageActive(i))
			continue;

		const FightMountSpecialImgUpgradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSpecialImgUpgradeCfg(i,m_mount_param.special_img_grade_list[i]);
		if (NULL != special_img_grade_cfg)
		{
			special_img_add_count += special_img_grade_cfg->chengzhangdan_count;
		}
	}

	if ((m_mount_param.chengzhangdan_count + use_count) > (grade_cfg->chengzhangdan_limit + special_img_add_count))
	{
		m_role->NoticeNum(errornum::EN_CHENGZHANGDAN_USE_LIMIT_NUM);
		return false;
	}

	m_mount_param.chengzhangdan_count += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);
	this->SendFightMountInfo();

	return true;
}

bool FightMountManager::FightMountCanFly()
{
	return false;
// 	if (!LOGIC_CONFIG->GetFightMountOpConfig().FightMountCanFly(m_mount_param.flyup_use_imageid))
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
// 	if (m_role->GetMultiFightMount()->IsOnFightMount())
// 	{
// 		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_CANNOT_FLY);
// 		return false;
// 	}
// 
// 	return true;
}

int FightMountManager::RefreshFightMountMinEquipLevel()
{
	int min = m_mount_param.equip_level_list[0];

	for (int i = 1; i < FightMountParam::EQUIP_COUNT; ++i)
	{
		min = m_mount_param.equip_level_list[i] < min ? m_mount_param.equip_level_list[i] : min;
	}

	return min;
}

void FightMountManager::CheckActive()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (!m_mount_param.IsFightMountActived())
	{
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_FIGHT_MOUNT);
		if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day)
		{
			if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
			{
				FightMountFunction::ActiveFightMount(m_role);
			}
		}
	}
}

// 幻化特殊战力加成
void FightMountManager::HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add)
{
	if (!LOGIC_CONFIG->GetFightMountOpConfig().IsAddSpecialCap())
	{
		return;
	}

	auto cfg_item = LOGIC_CONFIG->GetFightMountOpConfig().GetHuanHuaSpecialCapAddCfg(huanhua_id);
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

int FightMountManager::GetSpecialImageLevelById(int imageid)
{
	if (imageid < 0 || imageid >= FightMountParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return 0;
	}

	if (!m_mount_param.IsSpecialImageActive(imageid))
	{
		return 0;
	}

	return m_mount_param.special_img_grade_list[imageid];
}

void FightMountManager::CalcBaseAttrAddFromSkill(CharIntAttrs & base_add)
{
	// 阶数加成
	const FightMountGradeConfig * grade_config = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(m_mount_param.grade);
	if (NULL == grade_config)
	{
		return;
	}

	int total_add_per = 0;
	for (int i = 0; i < FightMountParam::SKILL_COUNT; i++)
	{
		if (m_mount_param.skill_level_list[i] <= 0)
		{
			continue;
		}
		const FightMountSkillConifg *skill_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetSkillCfg(i, m_mount_param.skill_level_list[i]);
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

int FightMountManager::GetMedalSkillLevel()
{
	short medal_skill_index = LOGIC_CONFIG->GetFightMountOpConfig().GetMedalSkillIndex();
	if (medal_skill_index < 0 || medal_skill_index >= (short)static_array_size(m_mount_param.skill_level_list))
	{
		return 0;
	}
	return m_mount_param.skill_level_list[medal_skill_index];
}

int FightMountManager::GetGrade()
{
	if (!m_mount_param.IsFightMountActived())
	{
		return 0;
	}
	return m_mount_param.grade;
}
