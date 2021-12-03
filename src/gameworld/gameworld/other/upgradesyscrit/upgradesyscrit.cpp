#include "upgradesyscrit.hpp"

int UpgradeSysCrit::UpgradeSysTypeToJinjieActThemeType(int type)
{
	switch (type)
	{
	case UPGRADE_SYS_CRIT_TYPE_MOUNT: return JINJIE_ACT_THEME_TYPE_MOUNT;
	case UPGRADE_SYS_CRIT_TYPE_WING: return JINJIE_ACT_THEME_TYPE_WING;
	case UPGRADE_SYS_CRIT_TYPE_FABAO: return JINJIE_ACT_THEME_TYPE_FABAO;
	case UPGRADE_SYS_CRIT_TYPE_SHENBING: return JINJIE_ACT_THEME_TYPE_SHENBING;
	case UPGRADE_SYS_CRIT_TYPE_FOOTPRINT: return JINJIE_ACT_THEME_TYPE_FOOTSPRINT;
	case UPGRADE_SYS_CRIT_TYPE_HALO: return JINJIE_ACT_THEME_TYPE_GUANGHUAN;
	case UPGRADE_SYS_CRIT_TYPE_SHIZHUANG: return JINJIE_ACT_THEME_TYPE_FASHION;
	case UPGRADE_SYS_CRIT_TYPE_FIGHTMOUNT: return JINJIE_ACT_THEME_TYPE_FIGHT_MOUNT;
	case UPGRADE_SYS_CRIT_TYPE_TOUSHI: return JINJIE_ACT_THEME_TYPE_TOUSHI;
	case UPGRADE_SYS_CRIT_TYPE_MASK: return JINJIE_ACT_THEME_TYPE_MASK;
	case UPGRADE_SYS_CRIT_TYPE_YAOSHI: return JINJIE_ACT_THEME_TYPE_YAOSHI;
	case UPGRADE_SYS_CRIT_TYPE_QILINBI: return JINJIE_ACT_THEME_TYPE_QILINBI;
	case UPGRADE_SYS_CRIT_TYPE_LINGTONG: return JINJIE_ACT_THEME_TYPE_LINGTONG;
	case UPGRADE_SYS_CRIT_TYPE_LINGGONG: return JINJIE_ACT_THEME_TYPE_LINGGONG;
	case UPGRADE_SYS_CRIT_TYPE_LINGQI: return JINJIE_ACT_THEME_TYPE_LINGQI;
	case UPGRADE_SYS_CRIT_TYPE_SHENGONG: return JINJIE_ACT_THEME_TYPE_SHENGONG;
	case UPGRADE_SYS_CRIT_TYPE_SHENYI: return JINJIE_ACT_THEME_TYPE_SHENYI;
	case UPGRADE_SYS_CRIT_TYPE_FLYPET: return JINJIE_ACT_THEME_TYPE_FLYPET;
	case UPGRADE_SYS_CRIT_TYPE_WEIYAN: return JINJIE_ACT_THEME_TYPE_WEIYAN;

	default:return UPGRADE_SYS_CRIT_TYPE_INVALID;
	}
}

UpgradeSysCrit & UpgradeSysCrit::Instance()
{
	static UpgradeSysCrit usc;
	return usc;
}

UpgradeSysCrit::UpgradeSysCrit() {}

UpgradeSysCrit::~UpgradeSysCrit() {}


int UpgradeSysCrit::CalculateCritRate(Role* role, int type)
{
	int rate = 0;
	if (nullptr == role)
	{
		return 0;
	}
	rate += role->GetRoleActivity()->GetRACriticalStrikeDayRate(this->UpgradeSysTypeToJinjieActThemeType(type)) * 100;						//暴击日活动
	rate += LOGIC_CONFIG->GetUpgradeSysCritConfig().GetUpgradeSysCritRate(type);			//进阶系统祝福值暴击配置的几率
	return rate;
}

int UpgradeSysCrit::CalculateCritValue(Role* role, int type)
{
	if (nullptr == role)
	{
		return 0;
	}
	int rate = this->CalculateCritRate(role, type);
	int value = 1;
	if (RandomNum(10000) < rate)
	{
		int cfg_crit_value = LOGIC_CONFIG->GetUpgradeSysCritConfig().GetUpgradeSysCritValue(type);
		value = cfg_crit_value == role->GetRoleActivity()->GetCriticalStrikeDayMul(this->UpgradeSysTypeToJinjieActThemeType(type)) ? cfg_crit_value : 2;
	}
	return value;
}


