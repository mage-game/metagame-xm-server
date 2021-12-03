#include "capability.hpp"

#include "obj/character/role.h"
#include "protocal/msgrole.h"
#include "engineadapter.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/struct/historycapparam.hpp"
#include "servercommon/serverconfig/crossrankrewardconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/commondata.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "global/cross/crossmanager.h"
#include "other/cross/rolecross.h"

Capability::Capability() : m_role(NULL), m_total_cap(0), m_gm_total_cap(0), m_max_cap(0), m_other_cap(0)
{
	memset(m_cap_list, 0, sizeof(m_cap_list));
}

Capability::~Capability()
{

}

void Capability::Init(Role *role, int max_cap)
{
	m_role = role;
	m_max_cap = max_cap;

	if (0 == m_role->GetCommonDataParam()->next_clear_add_cap_time)
	{
		this->CheckClearAddCap();
	}
}

void Capability::OnRoleReCalcAttrBegin(bool recalc_all)
{
	m_total_cap = 0;
	m_other_cap = 0;

	if (recalc_all) 
	{
		memset(m_cap_list, 0, sizeof(m_cap_list));
	}
}

void Capability::OnRoleReCalcAttrEnd(bool notify, bool is_time_limit)
{
	int old_total_cap = m_total_cap;

// 	for (int i = CAPABILITY_TYPE_INVALID + 1; i < CAPABILITY_TYPE_MAX; i++)
// 	{
// 		m_total_cap += m_cap_list[i];
// 	}
	//战力改为统一计算
	m_cap_list[CAPABILITY_TYPE_TOTAL] += m_other_cap;
	m_total_cap = m_cap_list[CAPABILITY_TYPE_TOTAL];

	if (m_total_cap > m_max_cap)
	{
		m_max_cap = m_total_cap;
	}

	if (old_total_cap != m_total_cap)
	{
		m_role->OnRoleBaseInfoChange();
	}

	{ // 跨服增战榜
		int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), EngineAdapter::Instance().Time()) + 1;
		if (CrossConfig::Instance().IsHiddenServer())
		{
			const UniqueServerID unique_server_id = m_role->GetRoleCross()->GetOriginalUniqueServerID();
			opengame_day = CrossManager::Instance().GetGameServerDayIndex(unique_server_id) + 1;
		}

		int limi_open_day = CrossRankRewardConfig::Instance().GetCrossRankLimitOpenDay(CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY);
		if (limi_open_day <= 0 || opengame_day >= limi_open_day)
		{
			int &new_max_cap = m_role->GetCommonDataParam()->new_max_cap;
			int &add_cap = m_role->GetCommonDataParam()->add_cap_for_rank;

			if (0 == m_role->GetCommonDataParam()->had_reset_new_max_cap)
			{
				new_max_cap = 0;
				m_role->GetCommonDataParam()->had_reset_new_max_cap = 1;
			}

			if (0 == new_max_cap)
			{
				new_max_cap = m_total_cap;
			}

			if (m_total_cap > new_max_cap)
			{
				if (notify)
				{
					add_cap += (m_total_cap - new_max_cap);
					if (!is_time_limit)
					{
						new_max_cap = m_total_cap;
					}
				}
			}
		}
	}

	if (notify) this->SendRoleCapability();

	EventHandler::Instance().OnChangeCapability(m_role, CAPABILITY_TYPE_INVALID);
}

void Capability::OnGetRoleCapa()
{
	this->SendRoleCapability();
}

void Capability::ReCalcCap(int cap_type, const CharIntAttrs &diff_int_attrs)
{
	if (cap_type <= CAPABILITY_TYPE_INVALID || cap_type >= CAPABILITY_TYPE_MAX) return;

	bool is_total = (cap_type == CAPABILITY_TYPE_TOTAL);

	m_cap_list[cap_type] = Capability::GetCapByAttr(diff_int_attrs, is_total);

	EventHandler::Instance().OnChangeCapability(m_role, cap_type);
}

void Capability::AddCap(int cap_type, int cap)
{
	if (cap_type <= CAPABILITY_TYPE_INVALID || cap_type >= CAPABILITY_TYPE_MAX) return;

	m_cap_list[cap_type] += cap;
	m_other_cap += cap;

	EventHandler::Instance().OnChangeCapability(m_role, cap_type);
}

void Capability::SetCap(int cap_type, int cap)
{
	if (cap_type <= CAPABILITY_TYPE_INVALID || cap_type >= CAPABILITY_TYPE_MAX) return;

	m_cap_list[cap_type] = cap;

	EventHandler::Instance().OnChangeCapability(m_role, cap_type);
}

int Capability::GetRoleCapability(int cap_type)
{
	if (cap_type <= CAPABILITY_TYPE_INVALID || cap_type >= CAPABILITY_TYPE_MAX) return 0;

	return m_cap_list[cap_type];
}

int Capability::GetTotalCapability()
{
	if (m_gm_total_cap > 0)
	{
		return m_gm_total_cap;
	}

	return m_total_cap;
}

int Capability::GetCapByAttr(const CharIntAttrs &diff_int_attrs, bool is_total)
{
	double jingzhun_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] * ROLE_ATTR_PER;
	double baoji_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI] * ROLE_ATTR_PER;
	double kangbao_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO] * ROLE_ATTR_PER;
	double pofang_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] * ROLE_ATTR_PER;
	double mianshang_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] * ROLE_ATTR_PER;
	double huixinyiji_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI] * ROLE_ATTR_PER;
	double huixinyiji_hurt_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER] * ROLE_ATTR_PER;

	double baoji_hurt_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] * ROLE_ATTR_PER;
	double kangbao_hurt_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO_HURT] * ROLE_ATTR_PER;
	double zhufuyiji_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_ZHUFUYIJI_PER] * ROLE_ATTR_PER;
	double gedang_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_PER] * ROLE_ATTR_PER;
	//double gedang_dikang_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_DIKANG_PER] * ROLE_ATTR_PER;
	double gedang_jianshang_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_JIANSHANG_PER] * ROLE_ATTR_PER;
	double mingzhong_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG_PER] * ROLE_ATTR_PER;
	double shanbi_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI_PER] * ROLE_ATTR_PER;
	double skill_zengshang_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER] * ROLE_ATTR_PER;
	double skill_jianshang_per = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER] * ROLE_ATTR_PER;

	Attribute max_hp = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP];
	Attribute gongji = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI];
	Attribute fangyu = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU];
	Attribute mingzhong = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG];
	Attribute shanbi = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI];
	Attribute baoji = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI];
	Attribute jianren = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN];

	//fujia_shanghai 与 constant_zengshang 功能相同
	Attribute fujia_shanghai = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI];
	//dikang_shanghai 与 constant_mianshang 功能相同
	Attribute dikang_shanghai = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI];
	Attribute constant_zengshang = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG];
	Attribute constant_mianshang = diff_int_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG];

	double pvp_jianshang_per = diff_int_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] * ROLE_ATTR_PER;
	double pvp_zengshang_per = diff_int_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] * ROLE_ATTR_PER;
	double pve_jianshang_per = diff_int_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] * ROLE_ATTR_PER;
	double pve_zengshang_per = diff_int_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] * ROLE_ATTR_PER;
	double skill_cap_per = diff_int_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_SKILL_CAP_PER] * ROLE_ATTR_PER;

	if (!is_total)
	{
		// 计算单系统战力时，不计算特殊属性，否则会过大
		pofang_per = 0;
		mianshang_per = 0;
		pvp_jianshang_per = 0;
		pvp_zengshang_per = 0;
		pve_jianshang_per = 0;
		pve_zengshang_per = 0;
	}

	// 战力A = 攻击 * 2 + 防御 * 2 + 生命 * 0.1 + 暴击 * 2 + 抗暴 * 2 + 命中 * 2 + 闪避 * 2 + 附加伤害 * 2.5 + 抵抗伤害 * 2.5
	double CapA = gongji * 2 + fangyu * 2 + max_hp * 0.1 + baoji * 2 + jianren * 2 + mingzhong * 2 + shanbi * 2 + (fujia_shanghai + constant_zengshang) * 2.5 + (dikang_shanghai + constant_mianshang) * 2.5;

	// 战力B = 战力A + 战力A * (1/3 * （0.4 * 技能增伤 + 0.4 * 技能减伤 + 0.2 * 祝福一击 + 命中几率 + 闪避几率 + 0.5 * 暴击几率 + 0.5 * 抗暴几率 + 0.5 * 暴击伤害 + 0.5 * 暴击抵抗
	// + 0.2 * 会心几率 + 0.2 * 会心伤害 + 0.3 * 格挡几率 + 0.4 * 格挡减伤 + 伤害加成 + ((1/ ( 1 - 伤害减免) - 1) + 0.5 * PVE伤害加成 + 0.5 * ((1/ ( 1 - PVE伤害减免) - 1) + 0.5 * PVP伤害加成 + 0.5 * ((1/ ( 1 - PVP伤害减免) - 1) + 0.5 * 破甲几率）
	// + 技能1战力万分比 + 技能2战力万分比 + … + 技能N战力万分比）

	double real_mianshang_per = 0;
	double real_pve_jianshang_per = 0;
	double real_pvp_jianshang_per = 0;

	if ((1 - mianshang_per) != 0)
	{
		real_mianshang_per = (1 / (1 - mianshang_per) - 1) > 0 ? (1 / (1 - mianshang_per) - 1) : 0;
	}

	if ((1 - pve_jianshang_per) != 0)
	{
		real_pve_jianshang_per = (1 / (1 - pve_jianshang_per) - 1) > 0 ? (1 / (1 - pve_jianshang_per) - 1) : 0;
	}
	
	if ((1 - pvp_jianshang_per) != 0)
	{
		real_pvp_jianshang_per = (1 / (1 - pvp_jianshang_per) - 1) > 0 ? (1 / (1 - pvp_jianshang_per) - 1) : 0;
	}

	double CapB = CapA + CapA / 3 * 
		(skill_zengshang_per * 0.4 +
		skill_jianshang_per * 0.4 +
		zhufuyiji_per * 0.2 + 
		mingzhong_per + 
		shanbi_per + 
		baoji_per * 0.5 + 
		kangbao_per * 0.5 +
		baoji_hurt_per * 0.5 +
		kangbao_hurt_per * 0.5 +
		huixinyiji_per * 0.2 +
		huixinyiji_hurt_per * 0.2 +
		gedang_per * 0.3 +
		gedang_jianshang_per * 0.4 +
		pofang_per +
		real_mianshang_per +
		pve_zengshang_per * 0.5 +
		real_pve_jianshang_per * 0.5 +
		pvp_zengshang_per * 0.5 +
		real_pvp_jianshang_per * 0.5 +
		jingzhun_per * 0.5) + 
		CapA * skill_cap_per;

	// 战力C = 战力B
	double CapC = CapB;

	return static_cast<int>(CapC);
}


void Capability::GmSetCapability(int capability)
{
	if (capability < 0) return;

	m_gm_total_cap = capability;

	this->SendRoleCapability();

	EventHandler::Instance().OnChangeCapability(m_role, CAPABILITY_TYPE_INVALID);
}

void Capability::SendRoleCapability()
{
//	if (m_role->IsInCross()) 
//	{
//		return;
//	}

	static Protocol::SCRoleCapability rc;

	for (int i = CAPABILITY_TYPE_INVALID; i < CAPABILITY_TYPE_MAX; i++)
	{
		rc.capability_list[i] = m_cap_list[i];
	}

	rc.total_capability = this->GetTotalCapability();
	rc.other_capability = m_other_cap;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rc, sizeof(rc));
}

int Capability::GetHistoryType(int cap_type)
{
	int history_cap_type = HISTORY_CAPABILITY_TYPE_MAX;

	switch(cap_type)
	{
	case CAPABILITY_TYPE_TOTAL:
		history_cap_type = HISTORY_CAPABILITY_TYPE_TAOTAL;
		break;

	case CAPABILITY_TYPE_MENTALITY:
	case CAPABILITY_TYPE_GEM:
	case CAPABILITY_TYPE_WASH:
		history_cap_type = HISTORY_CAPABILITY_TYPE_WASH;
		break;

	case CAPABILITY_TYPE_EQUIPMENT:
		history_cap_type = HISTORY_CAPABILITY_TYPE_EQUIPMENT;
		break;

	case CAPABILITY_TYPE_WING:
		history_cap_type = HISTORY_CAPABILITY_TYPE_WING;
		break;

	case CAPABILITY_TYPE_MOUNT:
		history_cap_type = HISTORY_CAPABILITY_TYPE_MOUNT;
		break;

	case CAPABILITY_TYPE_JINGLING:
		history_cap_type = HISTORY_CAPABILITY_TYPE_JINGLING;
		break;

	case CAPABILITY_TYPE_SHENZHUANG:
		history_cap_type = HISTORY_CAPABILITY_TYPE_SHENZHUANG;
		break;

	case CAPABILITY_TYPE_XIANNV:
		history_cap_type = HISTORY_CAPABILITY_TYPE_XIANNV;
		break;
	case CAPABILITY_TYPE_FEIXIAN:
		history_cap_type = HISTORY_CAPABILITY_TYPE_FEIXIAN;
		break;
	}

	return history_cap_type;
}

void Capability::OnDayChange()
{
	this->CheckClearAddCap();
}

void Capability::CheckClearAddCap()
{
	unsigned int &next_clear_time = m_role->GetCommonDataParam()->next_clear_add_cap_time;
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_sec >= next_clear_time)
	{
		m_role->GetCommonDataParam()->add_cap_for_rank = 0;
		const CrossRankRewardDateCfg *date_cfg = CrossRankRewardConfig::Instance().GetDateCfg(CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY);
		if (nullptr != date_cfg)
		{
			const tm *tm_now = EngineAdapter::Instance().LocalTime();
			if (nullptr != tm_now)
			{
				int day_count = 0;
				for (int i = tm_now->tm_wday + 1; i < DAY_PER_WEEK; ++i)
				{
					if (date_cfg->CanReward(i))
					{
						day_count = i - tm_now->tm_wday;
						break;
					}
				}

				if (0 == day_count)
				{
					for (int i = 0; i <= tm_now->tm_wday; ++i)
					{
						if (date_cfg->CanReward(i))
						{
							day_count = i + DAY_PER_WEEK - tm_now->tm_wday;
							break;
						}
					}
				}

				next_clear_time = now_sec + EngineAdapter::Instance().NextDayInterval(0, 0, 0) + (day_count - 1) * SECOND_PER_DAY;
			}
		}
	}
}
