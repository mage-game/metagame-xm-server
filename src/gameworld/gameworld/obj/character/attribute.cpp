#include "attribute.hpp"

#include "character.h"
#include "role.h"

#include "protocal/msgrole.h"

#include "scene/scene.h"
#include "scene/speciallogic/speciallogic.hpp"

#include "engineadapter.h"

#include "globalconfig/globalconfig.h"
#include "global/globalrole/globalrole.hpp"
#include "global/globalrole/globalrolemanager.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "other/role_module.hpp"
#include "other/tianshuxunzhu/tianshuxunzhu.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ConfigAttrDefineTypeString
{
	//ConfigAttrDefineTypeString() : attr_type(CharIntAttrs::CHARINTATTR_TYPE_INVALID)
	//{
	//	memset(attr_str, 0, sizeof(attr_str));
	//}

	char attr_str[32];
	short attr_type;
};

const static ConfigAttrDefineTypeString gs_config_attr_define_string_array[] = 
{
	{"maxhp", CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP},
	{"maxmp", CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXMP},
	{"gongji", CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI},
	{"fangyu", CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU},
	{"mingzhong", CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG},
	{"shanbi", CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI},
	{"baoji", CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI},
	{"jianren", CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN},
	{"movespeed", CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED},
	{"fujia_shanghai", CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI},
	{"dikang_shanghai", CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI},
	{"per_jingzhun", CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN},
	{"per_baoji_hurt", CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT},
	{"per_kangbao", CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO},
	{"per_pofang", CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG},
	{"per_mianshang", CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG},
	{"constant_zengshang", CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG},
	{"constant_mianshang", CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG},
	{"per_shanbi", CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI_PER },
	{"per_didang", CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_DIKANG_PER },
	{"per_mingzhong", CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG_PER },

	{"per_pvp_zengshenang", CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER },
	{"per_pvp_mianshang", CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER },
	{"per_pve_zengshang", CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER },
	{"per_pve_mianshang", CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER },
	{"per_gedang", CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_PER },

	{ "per_kangbao_hurt", CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO_HURT },
	
	{"0", CharIntAttrs::CHARINTATTR_TYPE_INVALID}
};

bool CharIntAttrs::GetAttrTypeFromAttrString(std::string &attr_str, short *attr_type)
{
	for (int i = 0; i < static_cast<int>(sizeof(gs_config_attr_define_string_array) / sizeof(gs_config_attr_define_string_array[0])); i++)
	{
		if (attr_str == gs_config_attr_define_string_array[i].attr_str)
		{
			if (NULL != attr_type) *attr_type = gs_config_attr_define_string_array[i].attr_type;
			return true;
		}
	}

	return false;
}

void CharIntAttrs::AddAttrFromConfig(const AttrConfig &cfg, double factor)
{
	m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(round(factor * cfg.maxhp));
	m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(round(factor * cfg.gongji));
	m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(round(factor * cfg.fangyu));
	m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(round(factor * cfg.mingzhong));
	m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(round(factor * cfg.shanbi));
	m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(round(factor * cfg.baoji));
	m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(round(factor * cfg.jianren));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Character::ChangeHp(Attribute *value, bool notfiy, unsigned char attr_notify_reason, bool is_ignore_forbidrecover)
{
	// 角色是否禁止回血（复活逻辑不触发）
	if (!is_ignore_forbidrecover && *value > 0 && IsInForbidRcover())
	{
		return;
	}

	this->ChangeHpMpHelper(&m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP], &m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP], value,
		CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP, notfiy, attr_notify_reason);
}

void Character::ChangeMp(Attribute *value, bool notfiy, unsigned char attr_notify_reason)
{
	this->ChangeHpMpHelper(&m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP], &m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP], value, 
		CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP, notfiy, attr_notify_reason);
}

void Character::Recover(bool recover_hp, bool recover_mp, bool notify, unsigned char attr_notify_reason)
{
	if (this->IsAlive())
	{
		if (recover_hp)
		{
			Attribute addhp = this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			this->ChangeHp(&addhp, notify, attr_notify_reason);
		}

		if (recover_mp)
		{
			Attribute addmp = this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP);
			this->ChangeMp(&addmp, notify, attr_notify_reason);
		}
	}
}

void Character::ChangeHpMpHelper(Attribute *which_cur, Attribute *which_max, Attribute *value, int attr_type, bool notity, unsigned char attr_notify_reason)
{
	Attribute old_attr = *which_cur;

	if (NULL != which_max && *value > *which_max - *which_cur) *value = *which_max - *which_cur;
	*which_cur += *value;

	if (*which_cur < 0) *which_cur = 0;
	if (NULL != which_max && *which_cur > *which_max) *which_cur = *which_max;

	*value = *which_cur - old_attr;

	if (notity && 0 != *value)
	{
		Protocol::SCRoleAttributeValue msg;
		msg.obj_id = m_obj_id;
		msg.attr_notify_reason = attr_notify_reason;
		msg.count = 1;
		msg.attr_list[0].attr_type = attr_type;
		msg.attr_list[0].attr_value = *which_cur;
		int sendlen = sizeof(msg) - (Protocol::SC_ATTR_CHANGE_COUNT - msg.count) * sizeof(msg.attr_list[0]);

		if (NULL != m_scene)
		{
			m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&msg, sendlen);
		}
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------
void Character::GetCharaIntAttrPercents(CharIntAttrs &attrs)
{
	if (sizeof(attrs.m_percent_attrs) == sizeof(m_int_attrs.m_percent_attrs))
	{
		memcpy(attrs.m_percent_attrs, m_int_attrs.m_percent_attrs, sizeof(attrs.m_percent_attrs));
	}
}

void Character::ChangeCharAttrHelper(int attr_type, Attribute attr_value, unsigned char attr_notify_reason)
{
	CharIntAttrs::AttrPair attr_pair(attr_type, attr_value);
	this->ChangeCharAttrHelper(&attr_pair, 1, attr_notify_reason);
}

void Character::ChangeCharAttrHelper(CharIntAttrs::AttrPair *attr_list, int count, unsigned char attr_notify_reason)
{
	if (NULL == attr_list || count <= 0) return;

	Attribute movespeed_old = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED];
	Attribute hp_old = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP];
	Attribute maxhp_old = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP];
	/*Attribute mp_old = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP];
	Attribute maxmp_old = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP];*/

	for (int i = 0; i < count; i++)
	{
		int attr_type = attr_list[i].attr_type;
		Attribute attr_value = attr_list[i].attr_value;
		if (!CharIntAttrs::IsFightAttr(attr_type, false) || ((Attribute)0 == attr_value))
		{
			continue;
		}
		
		m_int_attrs.m_attrs[attr_type] += attr_value;

		//if (CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP == attr_type || CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP == attr_type)
		//{
		//	// 这里会出bug的 比如攻击是500 减少1000 结果buff到期的时候 把1000补回来 发现被刷了

		//	if (m_int_attrs.m_attrs[attr_type] <= 0) m_int_attrs.m_attrs[attr_type] = 0;
		//}
	}

	{
		Protocol::SCRoleAttributeValue broadcast_msg;
		broadcast_msg.attr_notify_reason = attr_notify_reason;
		broadcast_msg.count = 0;
		broadcast_msg.obj_id = m_obj_id;

		{
			Attribute movespeed_new = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED];
			if (movespeed_old != movespeed_new && broadcast_msg.count < Protocol::SC_ATTR_CHANGE_COUNT)
			{
				broadcast_msg.attr_list[broadcast_msg.count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED;
				broadcast_msg.attr_list[broadcast_msg.count].attr_value = movespeed_new;
				++ broadcast_msg.count;
			}
		}

		{
			if (m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP] >= m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP])
			{
				m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP] = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP];
			}
			Attribute hp_new = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP];
			if (hp_new != hp_old && broadcast_msg.count < Protocol::SC_ATTR_CHANGE_COUNT)
			{
				broadcast_msg.attr_list[broadcast_msg.count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP;
				broadcast_msg.attr_list[broadcast_msg.count].attr_value = hp_new;
				++ broadcast_msg.count;
			}
		}

		{
			Attribute maxhp_new = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP];
			if (maxhp_new != maxhp_old && broadcast_msg.count < Protocol::SC_ATTR_CHANGE_COUNT)
			{
				broadcast_msg.attr_list[broadcast_msg.count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP;
				broadcast_msg.attr_list[broadcast_msg.count].attr_value = maxhp_new;
				++ broadcast_msg.count;
			}
		}

		{
			int sendlen = sizeof(broadcast_msg) - (Protocol::SC_ATTR_CHANGE_COUNT - broadcast_msg.count) * sizeof(broadcast_msg.attr_list[0]);
			if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&broadcast_msg, sendlen);
		}
	}

	if (Obj::OBJ_TYPE_ROLE == GetObjType()) // 通知给玩家自己
	{
		Role *role = (Role *)this;

		{
			Protocol::SCRoleAttributeValue attr_msg;
			attr_msg.attr_notify_reason = attr_notify_reason;
			attr_msg.count = 0;
			attr_msg.obj_id = m_obj_id;

			for (int i = 0; i < count; i++)
			{
				int attr_type = attr_list[i].attr_type;
				Attribute attr_value = attr_list[i].attr_value;
				if (!CharIntAttrs::IsFightAttr(attr_type, false) || ((Attribute)0 == attr_value))
				{
					continue;
				}

				if (attr_msg.count >= Protocol::SC_ATTR_CHANGE_COUNT) break;

				attr_msg.attr_list[attr_msg.count].attr_type = attr_type;
				attr_msg.attr_list[attr_msg.count].attr_value = m_int_attrs.m_attrs[attr_type];
				++ attr_msg.count;
			}

			// 修改最大的MP 可能导致MP发生变更
			if (m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP] > m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP])
			{
				m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP] = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP];

				if (attr_msg.count < Protocol::SC_ATTR_CHANGE_COUNT)
				{
					attr_msg.attr_list[attr_msg.count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP;
					attr_msg.attr_list[attr_msg.count].attr_value = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP];
					++ attr_msg.count;
				}
			}

			int sendlen = sizeof(attr_msg) - sizeof(attr_msg.attr_list[0]) * (Protocol::SC_ATTR_CHANGE_COUNT - attr_msg.count);
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&attr_msg, sendlen);
		}
	}
}

// -----------------------------------------------------------------------------------------------------------------------------------

void Role::ReCalcAttr(int recalc_reason, bool is_init, bool is_recover, unsigned char attr_notify_reason, bool is_time_limit)
{
	bool recalc_all = (CharIntAttrs::RECALC_REASON_INVALID == recalc_reason || is_init);

	int old_total_cap = this->GetCapability()->GetTotalCapability();

	this->GetCapability()->OnRoleReCalcAttrBegin(recalc_all);

	Attribute movespeed_old = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED];
	Attribute hp_old = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP];
	Attribute maxhp_old = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP];
	Attribute mp_old = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP];
	Attribute maxmp_old = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP];
	
	Attribute gongji_add = 0, fangyu_add = 0, mingzhong_add = 0, shanbi_add = 0, baoji_add = 0, jianren_add = 0,
		maxhp_add = 0, maxmp_add = 0, movespeed_add = 0, fujia_shanghai_add = 0, dikang_shanghai_add = 0,
		per_jingzhun_add = 0, per_baoji_add = 0, per_kangbao_add = 0, per_baoji_hurt_add = 0, per_kangbao_hurt_add = 0, per_pofang_add = 0, per_mian_shang_add = 0,
		constant_zengshang_add = 0, constant_mianshang_add = 0, global_cd_add = 0, hp_add = 0, mp_add = 0, huixinyiji_add = 0, huixinyiji_hurt_per_add = 0,
		zhufuyiji_per_add = 0, gedang_per_add = 0, gedang_dikang_per_add = 0, gedang_jianshang_add = 0, skill_zengshang_add = 0, skill_jianshang_add = 0,
		mingzhong_per_add = 0, shanbi_per_add = 0;

	{
		global_cd_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GLOBAL_COOLDOWN];
		hp_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP];
		mp_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP];
	}

 	if (!is_init) // 若不是初始化 记录之前战斗物品加成的战斗属性
	{
		maxhp_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP];
		maxmp_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXMP];

		gongji_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI];
		fangyu_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU];
		mingzhong_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG];
		shanbi_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI];
		baoji_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI];
		jianren_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_JIANREN] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN];
		movespeed_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED];
		fujia_shanghai_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI];
		dikang_shanghai_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_DIKANG_SHANGHAI] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI];
		per_jingzhun_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_JINGZHUN] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN];
		per_baoji_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI];
		per_kangbao_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_KANGBAO] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO];
		per_baoji_hurt_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI_HURT] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT];
		per_kangbao_hurt_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_KANGBAO_HURT] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO_HURT];
		per_pofang_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_POFANG] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG];
		per_mian_shang_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG];
		constant_zengshang_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG];
		constant_mianshang_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG];
		huixinyiji_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HUIXINYIJI] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI];
		huixinyiji_hurt_per_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER];
		zhufuyiji_per_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_ZHUFUYIJI_PER] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_ZHUFUYIJI_PER];

		gedang_per_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GEDANG_PER] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_PER];
		gedang_dikang_per_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GEDANG_DIKANG_PER] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_DIKANG_PER];
		gedang_jianshang_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GEDANG_JIANSHANG_PER] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_JIANSHANG_PER];

		skill_zengshang_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER];
		skill_jianshang_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER];

		mingzhong_per_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG_PER] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG_PER];
		shanbi_per_add = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI_PER] - m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI_PER];
	}

	if (CharIntAttrs::RECALC_REASON_INVALID == recalc_reason)
	{
		maxhp_add += m_gm_change_maxhp;
		gongji_add += m_gm_change_gongji;
		movespeed_add += m_gm_change_movespeed;
		fangyu_add += m_gm_change_fangyu;
		mingzhong_add += m_gm_change_mingzhong;
		shanbi_add += m_gm_change_shanbi;
		baoji_add += m_gm_change_baoji;
		jianren_add += m_gm_change_jianren;
		fujia_shanghai_add += m_gm_change_fujia_shanghai;
		dikang_shanghai_add += m_gm_change_dikang_shanghai;
		per_jingzhun_add += m_gm_change_per_jingzhun;
		per_baoji_add += m_gm_change_per_baoji;
		per_kangbao_add += m_gm_change_per_kangbao;
		per_pofang_add += m_gm_change_per_pofang;
		per_mian_shang_add += m_gm_change_per_mianshang;
		constant_zengshang_add += m_gm_change_constant_zengshang;
		constant_mianshang_add += m_gm_change_constant_mianshang;
	}

	{
		// 先初始化
		m_int_attrs.Reset();
	}
	
	{
		// 天赋 其他系统依赖于天赋系统
		m_role_module->talent.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_TALENT == recalc_reason || recalc_all);

		// 装备系统
		m_role_module->equip_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_EQUIPMENT == recalc_reason
			|| CharIntAttrs::RECALC_REASON_SHEN_ZHUANG == recalc_reason || recalc_all);

		// 精灵
		m_role_module->jingling_mgr.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_JINGLING == recalc_reason || recalc_all);

		// 符文系统
		m_role_module->runesystem_manger.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_RESON_RUNE_SYSTEM == recalc_reason || recalc_all);

		// 神州六器战力计算
		m_role_module->shenzhou_weapon_manger.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_SHENZHOU_WEAPON == recalc_reason || recalc_all);

		// 仙女
		m_role_module->xiannv_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_XIANNV == recalc_reason || recalc_all);

		// 元神系统
		m_role_module->mentality.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_MENTALITY == recalc_reason || recalc_all);

		// 坐骑系统
		m_role_module->mount_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_MOUNT == recalc_reason 
			|| CharIntAttrs::RECALC_REASON_ZHIBAO == recalc_reason
			|| CharIntAttrs::RECALC_REASON_XUNZHANG == recalc_reason || recalc_all);

		// 战斗坐骑系统
		m_role_module->fightmount_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_FIGHT_MOUNT == recalc_reason || recalc_all);

		// 技能系统
		m_skill_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_SKILL == recalc_reason || recalc_all);

		// 宝石
		m_role_module->stone_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_STONE == recalc_reason || recalc_all);

		// 称号
		m_role_module->title_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_TITLE == recalc_reason || recalc_all);

		// 仙盟仙术
		m_role_module->guild_xianshu.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_GUILD_XIANSHU == recalc_reason || recalc_all);

		// 羽翼
		m_role_module->wing.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_WING == recalc_reason 
			|| CharIntAttrs::RECALC_REASON_ZHIBAO == recalc_reason 
			|| CharIntAttrs::RECALC_REASON_XUNZHANG == recalc_reason || recalc_all);

		// 足迹
		m_role_module->footprint.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_FOOTPRINT == recalc_reason || recalc_all);

		// 披风
		m_role_module->cloak_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_CLOAK == recalc_reason || recalc_all);

		// 情缘
		m_role_module->qingyuan.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_QINGYUAN == recalc_reason || recalc_all);

		//宝宝战力计算
		m_role_module->baby.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_BABY == recalc_reason || recalc_all);

		// 时装
		m_role_module->shizhuang.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_SHIZHUANG == recalc_reason || recalc_all);

		// 卡牌组合
		m_role_module->cardzu.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_CARDZU == recalc_reason || recalc_all);

		// 猎命	注意，猎命的战力算入精灵战力，所以猎命的战力计算必须在精灵之后
		m_role_module->lieming_mgr.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_LIEMING == recalc_reason || recalc_all);

		// vipbuff
		m_role_module->vip.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_VIPBUFF == recalc_reason || recalc_all);

		// 声望
		m_role_module->shengwang_mgr.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_SHENGWANG == recalc_reason || recalc_all);

		// 成就
		m_role_module->chengjiu_mgr.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_CHENGJIU == recalc_reason || recalc_all);

		// 土豪金战力计算
		m_role_module->tuhaojin.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_TUHAOJIN  == recalc_reason || recalc_all);

		// 聊天大表情战力计算
		m_role_module->bigchatface.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_BIG_CHATFACE == recalc_reason || recalc_all);

		// 宠物战力计算
		m_role_module->pet.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_PET == recalc_reason || recalc_all);

		// 活动相关
		m_role_module->role_activity.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_RAND_ACTIVITY == recalc_reason || recalc_all);

		// 化神
		m_role_module->huashen.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_HUASHEN == recalc_reason || recalc_all);

		// 双人坐骑
		m_role_module->multi_mount.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_MULTI_MOUNT == recalc_reason || recalc_all);

		// 个性聊天框
		m_role_module->personalize_window.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_PERSONALIZE_WINDOW == recalc_reason || recalc_all);

		// 魔卡
		m_role_module->magic_card.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_MAGIC_CARD == recalc_reason || recalc_all);

		// 御魂
		m_role_module->mitama.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_MITAMA == recalc_reason || recalc_all);

		// 勋章
		m_role_module->xunzhang_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_XUNZHANG == recalc_reason || recalc_all);

		// 至宝
		m_role_module->zhibao_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_ZHIBAO == recalc_reason || recalc_all);

		// 神兵
		m_role_module->shenbing_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_SHENBING == recalc_reason || recalc_all);

		// 光环
		m_role_module->halo.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_HALO == recalc_reason 
			|| CharIntAttrs::RECALC_REASON_XUNZHANG == recalc_reason || recalc_all);

		// 神弓	注意，神弓的战力算入仙女战力，所以神弓的战力计算必须在仙女之后
		m_role_module->shengong.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_SHENGONG == recalc_reason 
			|| CharIntAttrs::RECALC_REASON_XUNZHANG == recalc_reason || recalc_all);

		// 神翼	注意，神翼的战力算入仙女战力，所以神翼的战力计算必须在仙女之后
		m_role_module->shenyi.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_SHENYI == recalc_reason 
			|| CharIntAttrs::RECALC_REASON_XUNZHANG == recalc_reason || recalc_all);

		// 仙盟
		m_role_module->role_guild.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_GUILD == recalc_reason || recalc_all);

		// 仙女守护
		m_role_module->xiannvshouhu_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_XIANNV_SHOUHU == recalc_reason || recalc_all);

		// 精灵光环	注意，精灵光环的战力算入精灵战力，所以精灵光环的战力计算必须在精灵之后
		m_role_module->jinglingguanghuan_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_JINGLING_GUANGHUAN == recalc_reason || recalc_all);

		// 精灵法阵	注意，精灵法阵的战力算入精灵战力，所以精灵法阵的战力计算必须在精灵之后
		m_role_module->jinglingfazhen_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_JINGLING_FAZHEN == recalc_reason || recalc_all);

		// 魔戒
		m_role_module->mojie_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_MOJIE == recalc_reason || recalc_all);

		// 转生战力计算
		m_role_module->zhuansheng_equip.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_ZHUANSHENGEQUIP  == recalc_reason || recalc_all);

		// 锻造套装
		m_role_module->equip_suit.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_EQUIP_SUIT == recalc_reason || recalc_all);

		// 神装
		m_role_module->shen_zhuang.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_EQUIPMENT == recalc_reason
			|| CharIntAttrs::RECALC_REASON_SHEN_ZHUANG == recalc_reason || recalc_all);

		// buff增加基础属性
		m_effect_list.ReCalcBaseAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_BUFF == recalc_reason || recalc_all);

		// buff增加基础属性百分比
		m_effect_list.ReCalcPercentAttr(m_int_attrs);

		// 生肖系统战力计算
		m_role_module->chinese_zodiac.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_CHINESE_ZODIAC == recalc_reason || recalc_all);
		
		// 神格系统
		m_role_module->shenge_system.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_SHENGE_SYSTEM == recalc_reason || recalc_all);

		// 神兽
		m_role_module->shenshou.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_SHENSHOU == recalc_reason || recalc_all);

		// 形象赋灵
		m_role_module->img_fuling.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_IMG_FULING == recalc_reason || recalc_all);

		// 合服装备
		m_role_module->csa_equip.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_CSA_EQUIP == recalc_reason || recalc_all);

		// 神印系统
		m_role_module->shenyin.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_SHEN_YIN == recalc_reason || recalc_all);

		// 角色目标
		m_role_module->role_goal.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_ROLE_GOAL == recalc_reason || recalc_all);

		// 元素之心
		m_role_module->element_heart.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_ELEMENT_HEART == recalc_reason || recalc_all);

		//法宝
		m_role_module->fabao.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_FABAO == recalc_reason || recalc_all);

		// 飞仙
		m_role_module->feixian_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_FEIXIAN == recalc_reason || recalc_all);

		// 竞技场历史排名突破
		m_role_module->role_challenge_field.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_CHALLENGE_FIELD_BEST_RANK_BREAK == recalc_reason || recalc_all);

		// 境界
		m_role_module->jingjie.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_JINGJIE == recalc_reason || recalc_all);

		// 进阶天赋
		m_role_module->jinjie_talent.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_JINJIE_TALENT == recalc_reason || recalc_all);

		// 河神洛书
		m_role_module->heshen_luoshu.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_HESHEN_LUOSHU == recalc_reason || recalc_all);

		//小鬼守护
		m_role_module->m_imp_guard.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_IMP_GUARD == recalc_reason || recalc_all);

		//boss图鉴
		m_role_module->boss_card.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_BOSS_CARD == recalc_reason || recalc_all);

		//人物转职
		m_role_module->role_zhuanzhi.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_ZHUANZHI == recalc_reason || recalc_all);

		// 进阶系统管理
		m_role_module->upgrade_manager.ReCalcAttr(m_int_attrs, recalc_reason, recalc_all);

		//腰饰
		m_role_module->yaoshi_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_YAOSHI == recalc_reason || recalc_all);

		//头饰
		m_role_module->toushi_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_TOUSHI == recalc_reason || recalc_all);

		//麒麟臂
		m_role_module->qilinbi_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_QILINBI == recalc_reason || recalc_all);

		//面具
		m_role_module->mask_manager.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_MASK == recalc_reason || recalc_all);

		//转职装备
		m_role_module->zhuanzhi_equip.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_ZHUANZHI_EQUIP == recalc_reason || recalc_all);

		//人物跨服
		m_role_module->role_cross.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_ROLE_CROSS == recalc_reason || recalc_all);

		//圣器系统
		m_role_module->shengqi.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_SHENGQI == recalc_reason || recalc_all);
		
		// 圣印
		m_role_module->seal.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_SEAL == recalc_reason || recalc_all);

		// 衣柜
		m_role_module->dressing_room_mgr.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_DRESSING_ROOM == recalc_reason || recalc_all);

		//神器
		m_role_module->shen_qi.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_SHENQI == recalc_reason || recalc_all);

		// 红装收集
		m_role_module->redequip_collect.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_RED_EQUIP_COLLECT == recalc_reason || recalc_all);

		// 橙装收集
		m_role_module->orang_equip_collect.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_ORANGE_EQUIP_COLLECT == recalc_reason || recalc_all);

		//魔龙
		m_role_module->molong.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_MOLONG == recalc_reason || recalc_all);

		//装备洗炼
		m_role_module->equip_baptize.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_EQUIP_BAPTIZE == recalc_reason || recalc_all);

		// 魔戒技能
		m_role_module->mojie_skill.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_MOJIE_SKILL == recalc_reason || recalc_all);

		// 名将
		m_role_module->m_greate_soldier.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_GREATE_SOLDIER == recalc_reason || recalc_all);

		//百战装备
		m_role_module->baizhan_equip.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_BAIZHAN_EQUIP == recalc_reason || recalc_all);

		//仙尊卡
		m_role_module->xianzunka.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_XIANZUNKA == recalc_reason || recalc_all);

		// 神话系统
		m_role_module->myth.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_MYTH == recalc_reason || recalc_all);

		// 星象
		m_role_module->zodiac.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_ZODIAC == recalc_reason || recalc_all);

		//斗气装备
		m_role_module->cross_equip.ReCalcAttr(m_int_attrs, CharIntAttrs::RECALC_REASON_CROSSEQUIP == recalc_reason || recalc_all);

		//天书寻主
		TianShuXunZhu::ReCalcAttr(this, m_int_attrs, CharIntAttrs::RECALC_REASON_TIANSHUXUNZHU == recalc_reason || recalc_all);

		//////////////////////////////////////////////////////////////////////////
		// 其他系统
		m_role_other_mgr->ReCalcAttr(m_int_attrs, recalc_reason, recalc_all);
		//////////////////////////////////////////////////////////////////////////

		if (CrossConfig::Instance().IsHiddenServer())
		{
			// 跨服个人全局属性
			m_role_module->cross_role_global.ReCalcAttr(m_int_attrs);
		}
		else
		{
			// 情缘属性必须在这个模块计算前
			// 全局性的角色数据，战力重算
			GlobalRole *g_role = GlobalRoleManager::Instance().GetGlobalRole(m_uid);
			if (NULL != g_role)
			{
				g_role->ReCalcAttr(m_int_attrs, recalc_reason, recalc_all, this);
			}
		}

		// 角色本身
		this->ReCalcSelfAttr(m_int_attrs);
	}
	
	//基础属性百分比处理
	{
		static CharIntAttrs attr_add_from_per;
		attr_add_from_per.Reset();
		
		float maxhp_per = m_int_attrs.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP];
		attr_add_from_per.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] = static_cast<Attribute>(m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * maxhp_per);

		float gongji_per = m_int_attrs.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI];
		attr_add_from_per.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] = static_cast<Attribute>(m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * gongji_per);

		float fangyu_per = m_int_attrs.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU];
		attr_add_from_per.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] = static_cast<Attribute>(m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * fangyu_per);

		float mingzhong_per = m_int_attrs.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG];
		attr_add_from_per.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] = static_cast<Attribute>(m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * mingzhong_per);

		float shanbi_per = m_int_attrs.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI];
		attr_add_from_per.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] = static_cast<Attribute>(m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * shanbi_per);

		float baoji_per = m_int_attrs.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI];
		attr_add_from_per.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] = static_cast<Attribute>(m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * baoji_per);

		float jianren_per = m_int_attrs.m_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN];
		attr_add_from_per.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] = static_cast<Attribute>(m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * jianren_per);

		// 2019.03.15 策划要求，特定万分比属性用乘法
		{
			// (1 + 伤害加成) * ...  *(1 + 伤害加成) - 1
			if (m_int_attrs.m_special_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] > 0)
			{
				m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] = static_cast<Attribute>((m_int_attrs.m_special_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] - 1) * 10000.0f);
			}

			if (m_int_attrs.m_special_percent_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] > 0)
			{
				m_int_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] = static_cast<Attribute>((m_int_attrs.m_special_percent_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] - 1) * 10000.0f);
			}

			if (m_int_attrs.m_special_percent_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] > 0)
			{
				m_int_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] = static_cast<Attribute>((m_int_attrs.m_special_percent_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] - 1) * 10000.0f);
			}

			// 1 - (1 - 伤害减免) * ...  *(1 - 伤害减免)
			if (m_int_attrs.m_special_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] > 0)
			{
				m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] = static_cast<Attribute>((1 - m_int_attrs.m_special_percent_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG]) * 10000.0f);
			}

			if (m_int_attrs.m_special_percent_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] > 0)
			{
				m_int_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] = static_cast<Attribute>((1 - m_int_attrs.m_special_percent_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER]) * 10000.0f);
			}

			if (m_int_attrs.m_special_percent_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] > 0)
			{
				m_int_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] = static_cast<Attribute>((1 - m_int_attrs.m_special_percent_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER])* 10000.0f);
			}
		}

		this->GetCapability()->ReCalcCap(CAPABILITY_TYPE_ATTR_PER, attr_add_from_per);
		m_int_attrs.Add(attr_add_from_per.m_attrs);
	}
	
	{
		// 如果使用GM变更了战力，则重新设置基础属性值
		if (m_gm_change_capability_attr > 0)
		{
			//攻击  0.2778 //防御  0.0926 //生命  1.8519
			//命中  0.0093 //闪避  0.0093 //暴击  0.0093 //抗暴  0.0093

			// 缓存不用重置的属性
			Attribute tmp_move_speed = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED];

			// 重置属性
			m_int_attrs.Reset();

			// 设置战力属性
			m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] = static_cast<Attribute>(m_gm_change_capability_attr * 0.2778);
			m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] = static_cast<Attribute>(m_gm_change_capability_attr * 0.0926);
			m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] = static_cast<Attribute>(m_gm_change_capability_attr * 1.8519);
			m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] = static_cast<Attribute>(m_gm_change_capability_attr * 0.0093);
			m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] = static_cast<Attribute>(m_gm_change_capability_attr * 0.0093);
			m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] = static_cast<Attribute>(m_gm_change_capability_attr * 0.0093);
			m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] = static_cast<Attribute>(m_gm_change_capability_attr * 0.0093);

			m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED] = tmp_move_speed;

			// 恢复血量
			is_recover = true;

			// 重置属性差值
			gongji_add = 0;
			fangyu_add = 0;
			mingzhong_add = 0;
			shanbi_add = 0;
			baoji_add = 0;
			jianren_add = 0;

			maxhp_add = 0;
			maxmp_add = 0;
			//movespeed_add = 0;  // 移动速度不用重置

			fujia_shanghai_add = 0;
			dikang_shanghai_add = 0;
			per_jingzhun_add = 0;
			per_baoji_add = 0;
			per_kangbao_add = 0;
			per_pofang_add = 0;
			per_mian_shang_add = 0;
			constant_zengshang_add = 0;
			constant_mianshang_add = 0;
			huixinyiji_add = 0;
			huixinyiji_hurt_per_add = 0;

			per_baoji_hurt_add = 0;
			per_kangbao_hurt_add = 0;

			zhufuyiji_per_add = 0;

			gedang_per_add = 0;
			gedang_dikang_per_add = 0;
			gedang_jianshang_add = 0;

			skill_zengshang_add = 0;
			skill_jianshang_add = 0;

			mingzhong_per_add = 0;
			shanbi_per_add = 0;
		}
	}

	// 同步到Character. 计算后的BASE属性同步到FIGHT属性
	{
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] + gongji_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] + fangyu_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] + mingzhong_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] + shanbi_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] + baoji_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_JIANREN] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] + jianren_add;

		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] + maxhp_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXMP] + maxmp_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED] + movespeed_add;

		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI] + fujia_shanghai_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_DIKANG_SHANGHAI] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI] + dikang_shanghai_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_JINGZHUN] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] + per_jingzhun_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI_HURT] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] + per_baoji_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_KANGBAO] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO] + per_kangbao_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_POFANG] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] + per_pofang_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] + per_mian_shang_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] + constant_zengshang_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] + constant_mianshang_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HUIXINYIJI] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI] + huixinyiji_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER] + huixinyiji_hurt_per_add;

		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI_HURT] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] + per_baoji_hurt_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_KANGBAO_HURT] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO_HURT] + per_kangbao_hurt_add;

		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_ZHUFUYIJI_PER] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_ZHUFUYIJI_PER] + zhufuyiji_per_add;

		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GEDANG_PER] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_PER] + gedang_per_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GEDANG_DIKANG_PER] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_DIKANG_PER] + gedang_dikang_per_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GEDANG_JIANSHANG_PER] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_JIANSHANG_PER] + gedang_jianshang_add;

		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER] + skill_zengshang_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER] + skill_jianshang_add;

		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG_PER] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG_PER] + mingzhong_per_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI_PER] = m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI_PER] + shanbi_per_add;

		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GLOBAL_COOLDOWN] = global_cd_add;
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP] = hp_add;	// 不能少，否则，hp为0的在下边effect通不过
		m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP] = mp_add;
	}

	{
		bool is_die = false;
		m_effect_list.Effect(0, this, &is_die);		// 这里可能会对hp进行修正，所以下边重新赋值

		if (m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP] >= m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP])
		{
			m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP] = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP];
		}

		if (m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP] >= m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP])
		{
			m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP] = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP];
		}
	}

	if (!is_init)
	{
		m_effect_list.Recalc();
	}

	{
// 		// 新手保护期，装备属性重算恢复满血
// 		if (!is_init && hp_old == maxhp_old)
// 		{
// 			if (CharIntAttrs::RECALC_REASON_EQUIPMENT == recalc_reason)
// 			{
// 				int xinshou_protect_level = GLOBALCONFIG->GetRoleXinShouProtectedLevel();
// 				if (m_level < xinshou_protect_level)
// 				{
// 					is_recover = true;
// 				}
// 			}
// 		}

		if (is_recover) // 恢复血量魔法到最大
		{
			m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP] = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP];
			m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP] = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP];
		}

		if (!is_init) 
		{
			{
				Protocol::SCRoleAttributeValue broadcast_msg;
				broadcast_msg.attr_notify_reason = attr_notify_reason;
				broadcast_msg.count = 0;
				broadcast_msg.obj_id = m_obj_id;
				
				// speed
				{
					Attribute movespeed_new = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED];
					if (movespeed_old != movespeed_new && broadcast_msg.count < Protocol::SC_ATTR_CHANGE_COUNT)
					{
						broadcast_msg.attr_list[broadcast_msg.count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED;
						broadcast_msg.attr_list[broadcast_msg.count].attr_value = movespeed_new;
						++ broadcast_msg.count;
					}
				}
				
				// hp
				{
					{
						if (m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP] >= m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP])
						{
							m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP] = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP];
						}
						Attribute hp_new = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP];
						if (hp_new != hp_old && broadcast_msg.count < Protocol::SC_ATTR_CHANGE_COUNT)
						{
							broadcast_msg.attr_list[broadcast_msg.count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP;
							broadcast_msg.attr_list[broadcast_msg.count].attr_value = hp_new;
							++ broadcast_msg.count;
						}
					}

					{
						Attribute maxhp_new = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP];
						if (maxhp_new != maxhp_old && broadcast_msg.count < Protocol::SC_ATTR_CHANGE_COUNT)
						{
							broadcast_msg.attr_list[broadcast_msg.count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP;
							broadcast_msg.attr_list[broadcast_msg.count].attr_value = maxhp_new;
							++ broadcast_msg.count;
						}
					}
				}
			
				// mp
				{
					{
						if (m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP] >= m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP])
						{
							m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP] = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP];
						}
						Attribute mp_new = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP];
						if (mp_new != mp_old && broadcast_msg.count < Protocol::SC_ATTR_CHANGE_COUNT)
						{
							broadcast_msg.attr_list[broadcast_msg.count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP;
							broadcast_msg.attr_list[broadcast_msg.count].attr_value = mp_new;
							++ broadcast_msg.count;
						}
					}

					{
						Attribute maxmp_new = m_int_attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP];
						if (maxmp_new != maxmp_old && broadcast_msg.count < Protocol::SC_ATTR_CHANGE_COUNT)
						{
							broadcast_msg.attr_list[broadcast_msg.count].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP;
							broadcast_msg.attr_list[broadcast_msg.count].attr_value = maxmp_new;
							++ broadcast_msg.count;
						}
					}
				}

				{
					int sendlen = sizeof(broadcast_msg) - (Protocol::SC_ATTR_CHANGE_COUNT - broadcast_msg.count) * sizeof(broadcast_msg.attr_list[0]);
					if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&broadcast_msg, sendlen);
				}
			}
	
			{
				m_need_send_attrinfo = true;
//				this->SendAttributeInfo(attr_notify_reason);
			}
		}
	}
	
	this->GetCapability()->ReCalcCap(CAPABILITY_TYPE_TOTAL, m_int_attrs);  //计算战力由以前的各个模块战力加起来改为所有属性算好后再套公式算战力
	this->GetCapability()->OnRoleReCalcAttrEnd(!is_init, is_time_limit);

	int total_cap = this->GetCapability()->GetTotalCapability();
	if (total_cap != old_total_cap && !is_init)
	{
		char attr_str[128] = {0};
		SNPRINTF(attr_str, sizeof(attr_str), "%d|%d|%d|%lld|%lld|%lld|%lld|%lld|%lld|%lld|%lld|%lld|%lld|%lld|%lld", recalc_reason
			, this->GetCapability()->GetMaxCapablity()
			, this->GetCapability()->GetOtherCapablity()
			, m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP]
			, m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI]
			, m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU]
			, m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG]
			, m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI]
			, m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI]
			, m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN]
			, m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI]
			, m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN]
			, m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT]
			, m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG]
			, m_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG]);
		ROLE_LOG_QUICK6(LOG_TYPE_CAPABILITY, this, total_cap, old_total_cap, attr_str, NULL);
	}

	EventHandler::Instance().OnReCalcAttr(this, is_init);
}

void Role::ReCalcSelfAttr(CharIntAttrs &base_add)
{
	CharIntAttrs self_attrs;		// 自身成长属性，不能缓存，因为不知道哪个模块会加一级属性

	{
		// 初始成长
		RoleLevelCfg * lvl_cfg = GLOBALCONFIG->GetRoleLvlCfg(m_level);
		if (NULL != lvl_cfg)
		{
			self_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] = GETPROFVALUE(GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, lvl_cfg->maxhp);
			self_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXMP] = GETPROFVALUE(GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXMP, lvl_cfg->maxmp);

			self_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] = GETPROFVALUE(GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, lvl_cfg->gongji);
			self_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] = GETPROFVALUE(GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, lvl_cfg->fangyu);

			self_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] = GETPROFVALUE(GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, lvl_cfg->mingzhong);
			self_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] = GETPROFVALUE(GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, lvl_cfg->shanbi);

			self_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] = GETPROFVALUE(GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, lvl_cfg->baoji);
			self_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] = GETPROFVALUE(GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, lvl_cfg->jianren);

			self_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED] = BIRTH_MOVESPEED_BASE;
		}

	}

	if (MOVE_MODE_FLY == m_move_mode)
	{
		int FLY_INCREASE_SPEED_PERCENT = GLOBALCONFIG->GetFlyIncreaseSpeedPercent();
		self_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED] += (Attribute)(self_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED] * (FLY_INCREASE_SPEED_PERCENT / 100.0));
	}

	base_add.Add(self_attrs.m_attrs, true);

	this->GetCapability()->ReCalcCap(CAPABILITY_TYPE_BASE, self_attrs); // 计算人物基本属性战斗力
}

void Role::SendAttributeInfo(int notify_reason)
{
	// 通知给玩家基本属性 hp 速度在上面广播 
	Protocol::SCRoleAttributeValue base_attr_msg;
	base_attr_msg.attr_notify_reason = notify_reason;
	base_attr_msg.count = 0;
	base_attr_msg.obj_id = m_obj_id;

	{
		// 发送战斗属性变更
		for (int i = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI; i < CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAX; i++)
		{
			if (base_attr_msg.count >= Protocol::SC_ATTR_CHANGE_COUNT) break;

			base_attr_msg.attr_list[base_attr_msg.count].attr_type = i;
			base_attr_msg.attr_list[base_attr_msg.count].attr_value = m_int_attrs.m_attrs[i];
			++ base_attr_msg.count;
		}
	}

	{
		// 发送基础属性变更
		for (int i = CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN + 1; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX; i++)
		{
			if (base_attr_msg.count >= Protocol::SC_ATTR_CHANGE_COUNT) break;

			base_attr_msg.attr_list[base_attr_msg.count].attr_type = i;
			base_attr_msg.attr_list[base_attr_msg.count].attr_value = m_int_attrs.m_attrs[i];
			++ base_attr_msg.count;
		}
	}

	int sendlen = sizeof(base_attr_msg) - sizeof(base_attr_msg.attr_list[0]) * (Protocol::SC_ATTR_CHANGE_COUNT - base_attr_msg.count);
	EngineAdapter::Instance().NetSend(m_netid, (const char *)&base_attr_msg, sendlen);
}
