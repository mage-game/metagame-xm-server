#include "cloakmanager.hpp"
#include "cloakconfig.hpp"

#include "gamelog.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/shuxingdanconfig.hpp"
#include "servercommon/noticenum.h"
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
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "scene/scene.h"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "other/jinjiesysreward/jinjiesysrewardmgr.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"

CloakManager::CloakManager()
	: m_role(NULL), m_other_capability(0), m_last_add_fangyu(0)
{

}

CloakManager::~CloakManager()
{

}

void CloakManager::Init(Role *role, const CloakParam &cloak_param)
{
	m_role = role;
	m_cloak_param = cloak_param;
}

void CloakManager::GetOtherInitParam(CloakParam *param)
{
	*param = m_cloak_param;
}

void CloakManager::GetRoleAppearance(RoleAppearance *appearance)
{
	if (NULL != appearance)
	{
		appearance->cloak_used_imageid = m_cloak_param.used_imageid;
	}
}

void CloakManager::OnLogin()
{
	this->CheckActive();
}

void CloakManager::OnRoleLevelChange(int level)
{
	this->CheckActive();
}

void CloakManager::Update(time_t now_second)
{
}

void CloakManager::OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag)
{
	return;		// 屏蔽技能

	if (NULL == target || !target->IsAlive() || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return;
	}

	unsigned long long now_time_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();

	if (base_injure > 0)
	{
		static const int SKILL_ID3 = 3;
		UNSTD_STATIC_CHECK(CloakParam::SKILL_COUNT > SKILL_ID3);
		if (m_cloak_param.skill_level_list[SKILL_ID3] > 0 && now_time_ms > m_cloak_param.skill_last_colddown_time[SKILL_ID3])
		{
			const CloakSkillConifg *skill_cfg = LOGIC_CONFIG->GetCloakConfig().GetSkillCfg(SKILL_ID3, m_cloak_param.skill_level_list[SKILL_ID3]);
			if (NULL != skill_cfg)
			{
				if (RandomNum(10000) < skill_cfg->param_a)
				{
					m_cloak_param.skill_last_colddown_time[SKILL_ID3] = now_time_ms + skill_cfg->param_d;

					EffectSpecialState *effect_yun = new EffectSpecialState(m_role->GetId(), SKILL_ID3, skill_cfg->param_c, EBT_XUANYUN, static_cast<char>(m_cloak_param.skill_level_list[SKILL_ID3]));
					effect_yun->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
					effect_yun->SetClientEffectType(EffectBase::ECT_SKILL_XUANYUN);
					target->AddEffect(effect_yun);
				}
			}
		}
	}
}

void CloakManager::OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{
	return;		// 屏蔽技能

	if (!m_role->IsAlive())
	{
		return;
	}

	unsigned long long now_time_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();

	if (delta_hp < 0)
	{
		Obj* attack_obj = m_role->GetScene()->GetObj(attacker);
		if (NULL != attack_obj && Obj::OBJ_TYPE_ROLE == attack_obj->GetObjType())
		{
			Character* attack_cha = (Character *)attack_obj;

			static const int SKILL_ID0 = 0;
			UNSTD_STATIC_CHECK(CloakParam::SKILL_COUNT > SKILL_ID0);

			if (m_cloak_param.skill_level_list[SKILL_ID0] > 0 && now_time_ms > m_cloak_param.skill_last_colddown_time[SKILL_ID0])
			{
				const CloakSkillConifg *skill_cfg = LOGIC_CONFIG->GetCloakConfig().GetSkillCfg(SKILL_ID0, m_cloak_param.skill_level_list[SKILL_ID0]);
				if (NULL != skill_cfg)
				{
					m_cloak_param.skill_last_colddown_time[SKILL_ID0] = now_time_ms + skill_cfg->param_d;

					Attribute rebound_hurt = static_cast<Attribute>(delta_hp * (skill_cfg->param_a * ROLE_ATTR_PER));
					Attribute rebound_hurt_max = static_cast<Attribute>(attack_cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (skill_cfg->param_b * ROLE_ATTR_PER));
					if (rebound_hurt < -rebound_hurt_max)
					{
						rebound_hurt = -rebound_hurt_max;
					}

					EffectOnce* effect_once = new EffectOnce(m_role->GetId(), 0, rebound_hurt, EffectBase::FIGHT_TYPE_NORMAL, EffectBase::PRODUCT_METHOD_REBOUNDHURT);
					attack_cha->AddEffect(effect_once);
				}
			}
		}
	}
}

long long CloakManager::CalcBaojiExtraHurt(Character *target)
{
	return 0;		// 屏蔽技能

	if (NULL == target || !target->IsAlive() || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return 0;
	}

	unsigned long long now_time_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();

	static const int SKILL_ID1 = 1;
	UNSTD_STATIC_CHECK(CloakParam::SKILL_COUNT > SKILL_ID1);
	if (m_cloak_param.skill_level_list[SKILL_ID1] > 0 && now_time_ms > m_cloak_param.skill_last_colddown_time[SKILL_ID1])
	{
		const CloakSkillConifg *skill_cfg = LOGIC_CONFIG->GetCloakConfig().GetSkillCfg(SKILL_ID1, m_cloak_param.skill_level_list[SKILL_ID1]);
		if (NULL != skill_cfg)
		{
			m_cloak_param.skill_last_colddown_time[SKILL_ID1] = now_time_ms + skill_cfg->param_d;
			return static_cast<long long>(target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (skill_cfg->param_a * ROLE_ATTR_PER));
		}
	}

	return 0;
}

void CloakManager::CalcShanbiExtraEffect()
{
	return;		// 屏蔽技能

	if (!m_role->IsAlive())
	{
		return;
	}

	unsigned long long now_time_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();

	static const int SKILL_ID2 = 2;
	UNSTD_STATIC_CHECK(CloakParam::SKILL_COUNT > SKILL_ID2);
	if (m_cloak_param.skill_level_list[SKILL_ID2] > 0 && now_time_ms > m_cloak_param.skill_last_colddown_time[SKILL_ID2])
	{
		const CloakSkillConifg *skill_cfg = LOGIC_CONFIG->GetCloakConfig().GetSkillCfg(SKILL_ID2, m_cloak_param.skill_level_list[SKILL_ID2]);
		if (NULL != skill_cfg)
		{
			Attribute current_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			Attribute max_hp = m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			if (current_hp < max_hp && RandomNum(10000) < skill_cfg->param_a)
			{
				m_cloak_param.skill_last_colddown_time[SKILL_ID2] = now_time_ms + skill_cfg->param_d;

				Attribute change_hp = static_cast<int>(max_hp * (skill_cfg->param_b * ROLE_ATTR_PER));
				if (change_hp > max_hp - current_hp)
				{
					change_hp = max_hp - current_hp;
				}

				m_role->ChangeHp(&change_hp, true);
			}
		}
	}
}

void CloakManager::SendCloakInfo()
{
	Protocol::SCCloakInfo mi;
	mi.cloak_level = m_cloak_param.level;
	mi.cur_exp = m_cloak_param.cur_exp;
	mi.used_imageid = m_cloak_param.used_imageid;
	mi.active_image_flag = m_cloak_param.reserve;
	mi.active_special_image_flag = m_cloak_param.active_special_image_flag;
	mi.equip_skill_level = m_cloak_param.equip_skill_level;

	ARRAYCPY(mi.shuxingdan_list, m_cloak_param.shuxingdan_list);
	memcpy(mi.equip_level_list, m_cloak_param.equip_level_list, sizeof(mi.equip_level_list));
	memcpy(mi.skill_level_list, m_cloak_param.skill_level_list, sizeof(mi.skill_level_list));
	memcpy(mi.special_img_grade_list, m_cloak_param.special_img_grade_list, sizeof(mi.special_img_grade_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&mi, sizeof(mi));
}

int CloakManager::GetFootprintAppeID()
{
	return m_cloak_param.used_imageid;
}

void CloakManager::OperationReq(const Protocol::CSCloakOperate *req)
{
	if (NULL == req) return;
	
	switch (req->operate_type)
	{
	case Protocol::CLOAK_OPERATE_TYPE_INFO_REQ:
	{
		this->SendCloakInfo();
	}
	break;

	case Protocol::CLOAK_OPERATE_TYPE_UP_LEVEL:
	{
		int old_level = m_cloak_param.level;
		const CloakUpLevelCfg * level_cfg = LOGIC_CONFIG->GetCloakConfig().GetUpLevelCfg(old_level);
		if (NULL == level_cfg) return;

		int max_up_star_stuff_times = level_cfg->pack_num;

		for (int i = 0; i < req->param_3 && i < max_up_star_stuff_times; i++)
		{
			bool ret = this->UpLevel(req->param_1, req->param_2);
			if (!ret || old_level != m_cloak_param.level)
			{
				m_role->SendOperateResult(Protocol::SCOperateResult::OP_CLOAK_UP_LEVEL, 0);
				return;
			}
		}

		m_role->SendOperateResult(Protocol::SCOperateResult::OP_CLOAK_UP_LEVEL, 1);
	}
	break;

	case Protocol::CLOAK_OPERATE_TYPE_USE_IMAGE:
	{
		this->UseImage(req->param_1);
	}
	break;

	case Protocol::CLOAK_OPERATE_TYPE_UP_SPECIAL_IMAGE:
	{
		//this->CloakSpecialImgUpgrade(req->param_1);
	}
	break;

	case Protocol::CLOAK_OPERATE_TYPE_UP_LEVEL_EQUIP:
	{
		// this->UplevelEquip(req->param_1);
	}
	break;

	case Protocol::CLOAK_OPERATE_TYPE_UP_LEVEL_SKILL:
	{
		//this->UplevelSkill(req->param_1, 0 != req->param_2);
	}
	break;

	default:
		break;
	}
}


bool CloakManager::IsImageActive(int attr_id)
{
	const CloakUpLevelCfg * curr_level_cfg = LOGIC_CONFIG->GetCloakConfig().GetUpLevelCfg(m_cloak_param.level);
	if (NULL == curr_level_cfg) return false;

	if (curr_level_cfg->active_image < attr_id) return false;

	return true;
}

bool CloakManager::UseImage(int imageid)
{
	if (imageid < SPECIAL_IMG_START_ID)
	{
		if (!this->IsImageActive(imageid))
		{
			m_role->NoticeNum(errornum::EN_CLOAK_IMG_NOT_ACTIVE);
			return false;
		}
	}
	else
	{
		if (!m_cloak_param.IsSpecialImageActive(imageid - SPECIAL_IMG_START_ID))
		{
			m_role->NoticeNum(errornum::EN_CLOAK_IMG_NOT_ACTIVE);
			return false;
		}
	}

	m_cloak_param.used_imageid = imageid;

	m_role->RefreshApperance(true);

	this->SendCloakInfo();

	return true;
}

bool CloakManager::UnUseImage(int imageid)
{
	if (imageid != m_cloak_param.used_imageid)
	{
		return false;
	}

	m_cloak_param.used_imageid = 0;

	m_role->RefreshApperance(true);

	this->SendCloakInfo();

	return true;
}

bool CloakManager::ActiveSpecialImage(int imageid, bool is_notify)
{
	if (0 != (m_cloak_param.active_special_image_flag & ((UInt64)1 << imageid)))
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_SPECIAL_IMG_ALREADY_ACTIVE);
		}

		return false;
	}

	const CloakSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetCloakConfig().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	m_cloak_param.active_special_image_flag |= ((UInt64)1 << imageid);
	if (0 == m_cloak_param.special_img_grade_list[imageid])
	{
		m_cloak_param.special_img_grade_list[imageid] = 1;
	}
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CLOAK);
	JinjiesysFuncHelper::Instance().OnActiveSpecialImg(m_role, JINJIE_TYPE_CLOAK, imageid);
	this->UseImage(SPECIAL_IMG_START_ID + imageid);

	return true;
}

bool CloakManager::UnActiveSpecialImage(int imageid)
{
	if (0 == (m_cloak_param.active_special_image_flag & ((UInt64)1 << imageid)))
	{
		return false;
	}

	const CloakSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetCloakConfig().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	UInt64 temp_flag = ~(1 << imageid);
	m_cloak_param.active_special_image_flag &= temp_flag;
	if (1 == m_cloak_param.special_img_grade_list[imageid])
	{
		m_cloak_param.special_img_grade_list[imageid] = 0;
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CLOAK);

	if (this->UnUseImage(SPECIAL_IMG_START_ID + imageid))
	{
		this->UseImage(1); //默认再给一个
	}

	return true;
}

void CloakManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (!m_cloak_param.IsCloakActived())
	{
		return;
	}

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
			const CloakUpLevelCfg * star_level_config = LOGIC_CONFIG->GetCloakConfig().GetUpLevelCfg(m_cloak_param.level);
			if (NULL != star_level_config)
			{
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += star_level_config->maxhp;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += star_level_config->gongji;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += star_level_config->fangyu;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += star_level_config->mingzhong;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += star_level_config->shanbi;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += star_level_config->baoji;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += star_level_config->jianren;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += star_level_config->extra_zengshang;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += star_level_config->extra_mianshang;
				// 特殊属性
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += star_level_config->per_jingzhun;
				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += star_level_config->per_baoji;

				// 下面特殊属性用乘法
				base_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, star_level_config->per_pofang);
				base_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, star_level_config->per_mianshang);
				base_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, star_level_config->pvp_zengshang_per);
				base_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, star_level_config->pve_zengshang_per);
				base_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, star_level_config->pvp_jianshang_per);
				base_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, star_level_config->pve_jianshang_per);

//				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += star_level_config->per_mianshang;
//				base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += star_level_config->per_pofang;
// 				base_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += star_level_config->pvp_zengshang_per;
// 				base_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += star_level_config->pve_zengshang_per;
// 				base_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += star_level_config->pvp_jianshang_per;
// 				base_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += star_level_config->pve_jianshang_per;
			}
			// 大目标特殊形象万分比
			int special_img_per_add = JinjieSysRewardMgr::Instance().GetJinjieSpecialImgExtraAddPer(m_role, JINJIE_TYPE_CLOAK);
			if (special_img_per_add > 0)
			{
				other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * ROLE_ATTR_PER * special_img_per_add);
				other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * ROLE_ATTR_PER * special_img_per_add);
				other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * ROLE_ATTR_PER * special_img_per_add);
				other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * ROLE_ATTR_PER * special_img_per_add);
				other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * ROLE_ATTR_PER * special_img_per_add);
				other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * ROLE_ATTR_PER * special_img_per_add);
				other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (Attribute)(base_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * ROLE_ATTR_PER * special_img_per_add);
			}
		}

		m_attrs_add.Add(base_attrs.m_attrs);

		{
			// 装备
			int active_equip_level = LOGIC_CONFIG->GetCloakConfig().GetActiveEquipLevel();
			if (active_equip_level <= m_cloak_param.level)
			{
				for (int i = 0; i < CloakParam::EQUIP_COUNT; ++i)
				{
					const CloakEquipConifg *equip_cfg = LOGIC_CONFIG->GetCloakConfig().GetEquipCfg(i);
					if (NULL != equip_cfg)
					{
						int level = m_cloak_param.equip_level_list[i];
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
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * rate);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * rate);
			}

			if (active_equip_level <= m_cloak_param.level)
			{
				for (int i = 0; i < CloakParam::EQUIP_COUNT; ++i)
				{
					const CloakEquipConifg *equip_cfg = LOGIC_CONFIG->GetCloakConfig().GetEquipCfg(i);
					if (NULL != equip_cfg)
					{
						int level = m_cloak_param.equip_level_list[i];
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
			for (int i = 0; i <= CloakParam::MAX_SPECIAL_IMAGE_ID; ++i)
			{
				if (0 != (m_cloak_param.active_special_image_flag & ((UInt64)1 << i)))
				{
					// 特殊形象进阶
					const int grade = m_cloak_param.special_img_grade_list[i];

					const CloakSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetCloakConfig().GetSpecialImgUpgradeCfg(i, grade);
					if (NULL != upgrade_cfg)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += upgrade_cfg->maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += upgrade_cfg->gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += upgrade_cfg->fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += upgrade_cfg->mingzhong;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += upgrade_cfg->shanbi;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += upgrade_cfg->baoji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += upgrade_cfg->jianren;
						//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += upgrade_cfg->extra_zengshang;
						//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += upgrade_cfg->extra_mianshang;
						//// 特殊属性
						//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += upgrade_cfg->per_jingzhun;
						//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI] += upgrade_cfg->per_baoji;
						//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += upgrade_cfg->per_mianshang;
						//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += upgrade_cfg->per_pofang;
						//m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += upgrade_cfg->pvp_zengshang_per;
						//m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += upgrade_cfg->pve_zengshang_per;
						//m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += upgrade_cfg->pvp_jianshang_per;
						//m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += upgrade_cfg->pve_jianshang_per;
					}
				}
			}
		}

		// 属性丹,屏蔽属性丹
		/*{
			int shuxingdan_attr_per = 0;
			for (int slot_index = 0; slot_index < CloakParam::SHUXINGDAN_MAX_TYPE; slot_index++)
			{
				const int num = m_cloak_param.shuxingdan_list[slot_index];
				if (num > 0)
				{
					const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_CLOAK, slot_index);
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
				m_attrs_add.m_attrs[i] += static_cast<Attribute>(base_attrs.m_attrs[i] * (ROLE_ATTR_PER * shuxingdan_attr_per));
			}
		}*/
		// 屏蔽技能
		//{
		//	// 技能属性
		//	for (int i = 0; i < CloakParam::SKILL_COUNT; i++)
		//	{
		//		if (m_cloak_param.skill_level_list[i] > 0)
		//		{
		//			const CloakSkillConifg *skill_cfg = LOGIC_CONFIG->GetCloakConfig().GetSkillCfg(i, m_cloak_param.skill_level_list[i]);
		//			if (NULL != skill_cfg)
		//			{
		//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += skill_cfg->maxhp;
		//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += skill_cfg->gongji;
		//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += skill_cfg->fangyu;
		//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += skill_cfg->mingzhong;
		//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += skill_cfg->shanbi;
		//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += skill_cfg->baoji;
		//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += skill_cfg->jianren;
		//				m_other_capability += skill_cfg->capability;
		//			}
		//		}
		//	}
		//}
		// 其他百分比属性加成，与上面的全属性加成加算
		{
			m_attrs_add.AddAttrs(&other_per_add_attr);
		}
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_CLOAK, m_attrs_add);
	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_CLOAK, m_other_capability);

	m_cloak_param.capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_CLOAK);

	base_add.Add(m_attrs_add.m_attrs, true);
}

CloakParam * CloakManager::GetCloakParam()
{
	return &m_cloak_param;
}

void CloakManager::Cloakprint(const char *reason, int param1, int param2, int param3, int param4, int param5)
{
	UNSTD_STATIC_CHECK(4 == CloakParam::EQUIP_COUNT);
	UNSTD_STATIC_CHECK(4 == CloakParam::SKILL_COUNT);

	gamelog::g_log_cloak.printf(LL_INFO, "Cloak role(%d,%s) %s(%d,%d,%d,%d,%d) "
		"old_level:%d, cur_level:%d,%d, used_imageid:%d zizhidan_count:%d chengzhangdan_count:%d, image_flag:%d, special_image_flag:%d"
		"equip_level_list:%d, %d, %d, %d"
		"skill_level_list:%d, %d, %d, %d",
		m_role->GetUID(), m_role->GetName(),
		reason, param1, param2, param3, param4, param5,
		(int)param1,
		(int)m_cloak_param.level, (int)m_cloak_param.cur_exp,
		(int)m_cloak_param.used_imageid,
		(int)m_cloak_param.shuxingdan_list[0], (int)m_cloak_param.shuxingdan_list[1],
		(int)m_cloak_param.reserve, (int)m_cloak_param.active_special_image_flag,
		(int)m_cloak_param.equip_level_list[0], (int)m_cloak_param.equip_level_list[1], (int)m_cloak_param.equip_level_list[2], (int)m_cloak_param.equip_level_list[3],
		(int)m_cloak_param.skill_level_list[0], (int)m_cloak_param.skill_level_list[1], (int)m_cloak_param.skill_level_list[2], (int)m_cloak_param.skill_level_list[3]);
}

void CloakManager::OnCompleteTask(TaskID task_id)
{
	this->CheckActive();
//	if (LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenTaskID(FUN_OPEN_TYPE_CLOAK) == task_id)
//	{
//		this->ActiveCloak();
//	}
}

void CloakManager::OnRoleUpLevel(int level)
{
	this->CheckActive();
//	if (m_cloak_param.IsCloakActived()) return;
//
//	if (level >= LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenRoleLevel(FUN_OPEN_TYPE_CLOAK))
//	{
//		this->ActiveCloak();
//	}
}

bool CloakManager::OnUseShuxingdan(int slot_idx, short &use_count)
{
	return false; // 屏蔽资质丹，成长丹

	if (slot_idx < 0 || slot_idx >= SHUXINGDAN_MAX_TYPE || use_count <= 0)
	{
		return false;
	}

	const CloakUpLevelCfg *grade_cfg = LOGIC_CONFIG->GetCloakConfig().GetUpLevelCfg(m_cloak_param.level);
	if (NULL == grade_cfg)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_CLOAK, slot_idx);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_cloak_param.level < cfg_item->order_limit)
	{
		gstr::SendError(m_role, "shuxingdan_use_level_limit", cfg_item->order_limit, slot_idx);
		return false;
	}

	if (m_cloak_param.shuxingdan_list[slot_idx] + use_count > cfg_item->use_limit_num)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	int shuxingdan_limit = (slot_idx == SHUXINGDAN_SLOT_TYPE_ZIZHI) ? grade_cfg->shuxingdan_limit : grade_cfg->chengzhangdan_limit;
	int special_img_add_count = 0;
	for (int i = 0; i <= CloakParam::MAX_SPECIAL_IMAGE_ID; i++)
	{
		if (!m_cloak_param.IsSpecialImageActive(i))
			continue;

		const CloakSpecialImgUpgradeCfg *special_img_grade_cfg = LOGIC_CONFIG->GetCloakConfig().GetSpecialImgUpgradeCfg(i, m_cloak_param.special_img_grade_list[i]);
		if (NULL != special_img_grade_cfg)
		{
			int shuxingdan_add_count = (slot_idx == SHUXINGDAN_SLOT_TYPE_ZIZHI) ? special_img_grade_cfg->shuxingdan_count : special_img_grade_cfg->chengzhangdan_count;
			special_img_add_count += shuxingdan_add_count;
		}
	}

	// 策划需求，修正真正使用数量
	int can_use_count = shuxingdan_limit + special_img_add_count - m_cloak_param.shuxingdan_list[slot_idx];
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

	m_cloak_param.shuxingdan_list[slot_idx] += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CLOAK);
	this->SendCloakInfo();

	return true;
}

int CloakManager::RefreshCloakMinEquipLevel()
{
	int min = m_cloak_param.equip_level_list[0];

	for (int i = 1; i < CloakParam::EQUIP_COUNT; ++i)
	{
		min = m_cloak_param.equip_level_list[i] < min ? m_cloak_param.equip_level_list[i] : min;
	}

	return min;
}

bool CloakManager::UplevelEquip(int equip_idx)
{
	if (equip_idx < 0 || equip_idx >= CloakParam::EQUIP_COUNT)
	{
		return false;
	}

	if (m_role->GetKnapsack()->IsLock() || m_role->GetSublock()->CheckHasLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!m_cloak_param.IsCloakActived())
	{
		return false;
	}

	const CloakEquipConifg *equip_cfg = LOGIC_CONFIG->GetCloakConfig().GetEquipCfg(equip_idx);
	if (NULL == equip_cfg)
	{
		return false;
	}
	
	short level = m_cloak_param.equip_level_list[equip_idx];
	if (level >= equip_cfg->max_level - 1)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_LEVEL_MAX);
		return false;
	}

	short old_level = level;

	if (m_cloak_param.level < LOGIC_CONFIG->GetCloakConfig().GetActiveEquipLevel())
	{
		return false;
	}

	// 消耗
	if (!m_role->GetKnapsack()->ConsumeItem(equip_cfg->level_cfg[level].item.item_id, equip_cfg->level_cfg[level].item.num, "CloakEquipUplevel"))
	{
		return false;
	}

	// 结算

	m_cloak_param.equip_level_list[equip_idx]++;

	//bool is_active_upgrade_skill = false;
	int min_equip_level = this->RefreshCloakMinEquipLevel();
	if (LOGIC_CONFIG->GetCloakConfig().GetActiveEquipSkillLevel() <= min_equip_level)
	{
		int equip_skill_level = LOGIC_CONFIG->GetCloakConfig().GetEquipSkillLevel(min_equip_level);
		if (equip_skill_level > 0)
		{
			m_cloak_param.equip_skill_level = equip_skill_level;
			//is_active_upgrade_skill = true;
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CLOAK);
	this->SendCloakInfo();

// 	if (is_active_upgrade_skill)
// 	{
// 		CommonDataParam *common_data = m_role->GetCommonDataParam();
// 		if (NULL != common_data)
// 		{
// 			if (0 == (common_data->upgrade_skill_skill_active_flag & (1 << UpGradeSkillConfig::SKILL_TYPE_FOOTPRINT)))
// 			{
// 				common_data->upgrade_skill_skill_active_flag |= (1 << UpGradeSkillConfig::SKILL_TYPE_FOOTPRINT);
// 				if (-1 == common_data->upgrade_skill_next_skill_index)
// 				{
// 					common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_FOOTPRINT;
// 				}
// 
// 				m_role->SendGradeSkillInfo(m_role);
// 			}
// 		}
// 	}

	this->Cloakprint("UplevelEquip", old_level);

	return true;
}

bool CloakManager::UpLevel(short stuff_index, short is_auto_buy)
{
	if (stuff_index < 0 || stuff_index >= CloakParam::MAX_UP_LEVEL_ITEM_COUNT)
		return false;

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock() || m_role->GetSublock()->CheckHasLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	//if (!m_cloak_param.IsCloakActived())
	//{
	//	m_role->NoticeNum(errornum::EN_CLOAK_NOT_EXIST);
	//	return false;
	//}

	short old_level = m_cloak_param.level;

	const CloakUpLevelCfg * curr_star_level_cfg = LOGIC_CONFIG->GetCloakConfig().GetUpLevelCfg(old_level);
	if (NULL == curr_star_level_cfg)
	{
		return false;
	}

	const CloakUpLevelCfg * next_star_level_cfg = LOGIC_CONFIG->GetCloakConfig().GetUpLevelCfg(old_level + 1);
	if (NULL == next_star_level_cfg)
	{
		return false;
	}

	const CloakUpLevelStuffCfg * up_star_stuff_cfg = LOGIC_CONFIG->GetCloakConfig().GetUpLevelStuffCfg(stuff_index);
	if (NULL == up_star_stuff_cfg)
	{
		return false;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(up_star_stuff_cfg->up_level_item_id);
	if (NULL == item_base)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();

	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[0].item_id = up_star_stuff_cfg->up_level_item_id;
	stuff_list[0].num = 1;
	stuff_list[0].buyable = 1 == is_auto_buy ? true : false;

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
	{
		return false;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "CloakManager::UpLevel"))
	{
		return false;
	}

	// 结算
	{
		int rand_value = RandomNum(10000);
		if (rand_value < up_star_stuff_cfg->baoji_per)
		{
			m_cloak_param.cur_exp += up_star_stuff_cfg->add_exp * up_star_stuff_cfg->baoji_mul;
		}
		else
		{
			m_cloak_param.cur_exp += up_star_stuff_cfg->add_exp;
		}

		int up_level_need_exp = curr_star_level_cfg->up_level_exp;
		while (m_cloak_param.cur_exp >= up_level_need_exp)
		{
			const CloakUpLevelCfg * next_star_level_cfg = LOGIC_CONFIG->GetCloakConfig().GetUpLevelCfg(m_cloak_param.level + 1);
			if (NULL == next_star_level_cfg || up_level_need_exp <= 0)
			{
				m_cloak_param.cur_exp = 0;
				break;
			}

			m_cloak_param.cur_exp -= up_level_need_exp;
			m_cloak_param.level++;

			if (0 != next_star_level_cfg->active_image && this->IsImageActive(next_star_level_cfg->active_image))
			{
				this->UseImage(next_star_level_cfg->active_image);
			}

			if (0 != next_star_level_cfg->is_broadcast)
			{
				int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cloak_up_level,
					UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), m_cloak_param.level);
				if (sendlen > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
				}
			}

			if (0 != next_star_level_cfg->is_broadcast_image)
			{
				int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cloak_imge,
					UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), m_cloak_param.level, next_star_level_cfg->active_image);
				if (sendlen > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
				}
			}

			up_level_need_exp = next_star_level_cfg->up_level_exp;
		}
		//屏蔽技能
		/*for (int i = 0; i < CloakParam::SKILL_COUNT; i++)
		{
			const CloakSkillConifg *skill_cfg = LOGIC_CONFIG->GetCloakConfig().GetSkillCfg(i, 1);
			if (NULL != skill_cfg && m_cloak_param.level >= skill_cfg->cloak_level && m_cloak_param.skill_level_list[i] == 0)
			{
				m_cloak_param.skill_level_list[i] = 1;
			}
		}*/

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CLOAK);
	}

	this->SendCloakInfo();

	this->Cloakprint("CloakUpgrade", old_level);
	EventHandler::Instance().OnCloakUpLevel(m_role, old_level, m_cloak_param.level);
	return true;
}

bool CloakManager::UplevelSkill(int skill_idx, bool is_auto_buy)
{
	return false; //屏蔽技能

	if (skill_idx < 0 || skill_idx >= CloakParam::SKILL_COUNT)
	{
		return false;
	}

	if (m_role->GetKnapsack()->IsLock() || m_role->GetSublock()->CheckHasLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!m_cloak_param.IsCloakActived())
	{
		m_role->NoticeNum(errornum::EN_CLOAK_NOT_EXIST);
		return false;
	}

	const CloakSkillConifg *target_skill_cfg = LOGIC_CONFIG->GetCloakConfig().GetSkillCfg(skill_idx, m_cloak_param.skill_level_list[skill_idx] + 1);
	if (NULL == target_skill_cfg)
	{
		return false;
	}

	if (0 == target_skill_cfg->skill_type)
	{
		return false;
	}

	if (target_skill_cfg->cloak_level > m_cloak_param.level)
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
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "CloakManager::UplevelSkill"))
		{
			return false;
		}
	}

	++m_cloak_param.skill_level_list[skill_idx];

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CLOAK);
	this->SendCloakInfo();

	this->Cloakprint("UplevelSkill");

	return true;
}

bool CloakManager::CloakSpecialImgUpgrade(short special_img_id)
{
	if (special_img_id <= 0 || special_img_id > CloakParam::MAX_SPECIAL_IMAGE_ID)
	{
		return false;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (!m_cloak_param.IsSpecialImageActive(special_img_id))
	{
		m_role->NoticeNum(errornum::EN_CLOAK_IMG_NOT_ACTIVE);
		return false;
	}

	const int old_grade = m_cloak_param.special_img_grade_list[special_img_id];
	if (old_grade >= CloakParam::MAX_UPGRADE_LIMIT)
	{
		m_role->NoticeNum(errornum::EN_CLOAK_SPECIAL_IMG_GRADE_LIMIT);
		return false;
	}

	const CloakSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetCloakConfig().GetSpecialImgUpgradeCfg(special_img_id, old_grade + 1);
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
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "CloakManager::CloakSpecialImgUpgrade"))
	{
		return false;
	}

	// 生效
	++m_cloak_param.special_img_grade_list[special_img_id];

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CLOAK);

	this->SendCloakInfo();

	this->Cloakprint("CloakSpecialImgUpgrade");

	return true;
}

bool CloakManager::ActiveCloak(bool is_recal)
{
	if (!m_cloak_param.IsCloakActived())
	{
		m_cloak_param.level = 1;
		m_cloak_param.cur_exp = 0;

		const CloakUpLevelCfg *level_cfg = LOGIC_CONFIG->GetCloakConfig().GetUpLevelCfg(m_cloak_param.level);
		if (NULL != level_cfg)
		{
			this->UseImage(level_cfg->active_image);
		}
		//屏蔽技能
		/*for (int i = 0; i < CloakParam::SKILL_COUNT; i++)
		{
			const CloakSkillConifg *skill_cfg = LOGIC_CONFIG->GetCloakConfig().GetSkillCfg(i, 1);
			if (NULL != skill_cfg && m_cloak_param.level >= skill_cfg->cloak_level)
			{
				m_cloak_param.skill_level_list[i] = 1;
			}
		}*/

		if (is_recal)
		{
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CLOAK);
		}

		this->SendCloakInfo();

		this->Cloakprint("ActiveCloak");
	}

	return true;
}

void CloakManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->CheckActive();
	}
}

void CloakManager::CheckActive()
{
	return;	//屏蔽自动激活

	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (!m_cloak_param.IsCloakActived())
	{
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_CLOAK);
		if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day)
		{
			if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
			{
				this->ActiveCloak();
			}
		}
	}
}
