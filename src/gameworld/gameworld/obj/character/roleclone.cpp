#include "roleclone.hpp"
#include "servercommon/serverconfig/localconfig.hpp"

#include "other/capability/capability.hpp"
#include "other/vip/vip.hpp"
#include "engineadapter.h"

RoleClone::RoleClone(int role_shadow_type)
	: RoleShadow(role_shadow_type)
	, m_end_time(0), m_enemy_obj_id(0)
{
}

void RoleClone::Init(Role * role,float per,int continue_time, ObjID enemy_id)
{
	if (continue_time > 0)
	{
		m_end_time = (unsigned int)EngineAdapter::Instance().Time() + continue_time;
	}
	RoleAppearance role_appearance;
	ChallengeParam param = ConstructChallengeParam(role, role_appearance, per);
	RoleShadow::Init(param, role_appearance, 0, 0);
	m_enemy_obj_id = enemy_id;
}

void RoleClone::Update(unsigned long interval, time_t now_second, unsigned int now_dayid)
{
	if (m_end_time > 0 && now_second > m_end_time)
	{
		m_int_attrs.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP, 0);	//先致死在base去更新  防止致死后技能会加血
	}
	RoleShadow::Update(interval, now_second, now_dayid);

	if (m_enemy_obj_id > 0)
	{
		this->ForceSetEnemy(m_enemy_obj_id);
	}
}

ChallengeParam RoleClone::ConstructChallengeParam(Role * role, RoleAppearance &role_appearance, float per)
{
	ChallengeParam param;

	if (NULL != role)
	{
		param.plat_type = LocalConfig::Instance().GetPlatType();
		param.role_id = role->GetUID();
		role->GetName(param.role_name);
		param.avatar = role->GetAvatar();
		param.sex = role->GetSex();
		param.prof = role->GetProf();
		param.camp = role->GetCamp();
		param.level = role->GetLevel();
		param.capability = role->GetCapability()->GetTotalCapability();

		param.max_hp = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP) * per);
		param.gongji = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI)* per);
		param.fangyu = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU)* per);
		param.mingzhong = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG)* per);
		param.shanbi = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI)* per);
		param.baoji = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI)* per);
		param.jianren = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN)* per);
		param.move_speed = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED)* per);
		param.fujiashanghai = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI)* per);
		param.dikangshanghai = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI)* per);

		param.jingzhun_per = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN)* per);
		param.baoji_per = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT)* per);
		param.kangbao_per = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO)* per);
		param.pofang_per = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG)* per);
		param.mianshang_per = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG)* per);
		param.constant_zengshang = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG)* per);
		param.constant_mianshang = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG)* per);

		param.vip_level = static_cast<char>(role->GetVip()->GetVipLevel());

		param.guild_post = role->GetGuildPost();
		param.guild_id = role->GetGuildID();
		role->GetGuildName(param.guild_name);

		role_appearance = role->GetAppearance();
		param.avatar_timestamp = role->GetAvatarTimeStamp();

		{
			// 技能信息
			SkillListParam skill_list_param;
			role->GetSkillManager()->GetSkillList(&skill_list_param);

			for (int i = 0; i < skill_list_param.count && i < SkillListParam::MAX_SKILL_NUM; ++i)
			{
				param.skill_list[i].skill_id = skill_list_param.skill_list[i].skill_id;
				param.skill_list[i].level = skill_list_param.skill_list[i].level;
			}
		}
	}

	return param;
}
