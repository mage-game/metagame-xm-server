#include "upgradecardbuy.hpp"
#include "upgradecardbuyconfig.hpp"
#include "gamecommon.h"
#include "protocal/msgother.h"
#include "config/logicconfigmanager.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "servercommon/errornum.h"
#include "config/sharedconfig/sharedconfig.h"
#include "other/mount/mountmanager.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/halo/halomanager.hpp"
#include "other/footprint/footprint.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/yaoshi/yaoshimanager.hpp"
#include "other/toushi/toushimanager.hpp"
#include "other/qilinbi/qilinbimanager.hpp"
#include "other/mask/maskmanager.hpp"
#include "item/knapsack.h"
#include "other/fabao/fabaomanager.hpp"
#include "other/shizhuang/shizhuang.h"
#include "other/upgrade/upgrademanager.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"


#define ACTIVITY_RALATED_DEF(activity_id, index, func) \
{ \
	(activity_id), ActivityRelatedInfo((index),  [](Role *role) -> int { return (func); } )\
}

std::map<int, ActivityRelatedInfo> UpgradeCardBuy::m_activity_bitflag_related_map =
{
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_MOUNT,				ACTIVITY_RELATED_INDEX_1,	role->GetMountManager()->GetMountParam()->grade),						//坐骑
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_WING,				ACTIVITY_RELATED_INDEX_2,	role->GetWing()->GetWingParam()->grade),								//羽翼
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_FABAO,				ACTIVITY_RELATED_INDEX_3,	role->GetFabaoManager()->GetParam()->grade),							//法宝 
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_SHENBING,			ACTIVITY_RELATED_INDEX_4,	role->GetShizhuang()->GetShiZhuangGrade(SHIZHUANG_TYPE_WUQI)),			//神兵
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_FOOTSPRINT,			ACTIVITY_RELATED_INDEX_5,	role->GetFootPrint()->GetGrade()),										//足迹
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_GUANGHUAN,			ACTIVITY_RELATED_INDEX_6,	role->GetHalo()->GetHaloParam()->grade),								//光环
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_FASHION,				ACTIVITY_RELATED_INDEX_7,	role->GetShizhuang()->GetShiZhuangGrade(SHIZHUANG_TYPE_BODY)),			//时装
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_FIGHT_MOUNT,			ACTIVITY_RELATED_INDEX_8,	role->GetFightMountManager()->GetFightMountParam()->grade),				//战骑
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_TOUSHI,				ACTIVITY_RELATED_INDEX_9,	role->GetTouShiManager()->GetTouShiParam()->grade),						//头饰
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_MASK,				ACTIVITY_RELATED_INDEX_10,	role->GetMaskManager()->GetMaskParam()->grade),							//面饰
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_YAOSHI,				ACTIVITY_RELATED_INDEX_11,	role->GetYaoShiManager()->GetYaoShiParam()->grade),						//腰饰
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_QILINBI,				ACTIVITY_RELATED_INDEX_12,	role->GetQiLinBiManager()->GetQiLinBiParam()->grade),					//麒麟臂
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_LINGTONG,			ACTIVITY_RELATED_INDEX_13,	role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_2)),					//灵童
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_LINGGONG,			ACTIVITY_RELATED_INDEX_14,	role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_3)),					//灵弓
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_LINGQI,				ACTIVITY_RELATED_INDEX_15,	role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_4)),					//灵骑
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_SHENGONG,			ACTIVITY_RELATED_INDEX_16,	role->GetShengong()->GetShengongParam()->grade),						//神弓
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_SHENYI,				ACTIVITY_RELATED_INDEX_17,	role->GetShenyi()->GetShenyiParam()->grade),							//神翼
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_FLYPET,				ACTIVITY_RELATED_INDEX_18,	role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_8)),					//飞宠
	ACTIVITY_RALATED_DEF(JINJIE_ACT_THEME_TYPE_WEIYAN,				ACTIVITY_RELATED_INDEX_19,	role->GetUpgradeManager()->GetGrade(UPGRADE_TYPE_5)),					//尾焰
};

int UpgradeCardBuy::GetIndexByThemeID(int theme)
{
	std::map<int, ActivityRelatedInfo>::iterator it = m_activity_bitflag_related_map.find(theme);
	if (m_activity_bitflag_related_map.end() != it)
	{
		return it->second.index;
	}

	return -1;
}

int UpgradeCardBuy::GetGradeByThemeID(int theme)
{
	std::map<int, ActivityRelatedInfo>::iterator it = m_activity_bitflag_related_map.find(theme);
	if (m_activity_bitflag_related_map.end() != it)
	{
		std::function<int(Role *role)> &func = it->second.get_grade_func;
		if (nullptr != func)
		{
			return func(m_role);
		}
	}

	return -1;
}

void UpgradeCardBuy::OnRoleLogin()
{
	time_t now_second = EngineAdapter::Instance().Time();

	if (m_param.last_reset_time == 0 || GetDayIndex(m_param.last_reset_time, now_second) != 0)
	{
		this->UpdateTodayGrade();
		m_param.last_reset_time = now_second;
	}

	this->SendAllInfo();
}

void UpgradeCardBuy::OnDayChange(UInt32 old_dayid, UInt32 now_dayid)
{
	m_param.ra_upgrade_card_buy_flag_1 = 0;
	m_param.ra_upgrade_card_buy_flag_2 = 0;
	this->UpdateTodayGrade();
	m_param.last_reset_time = EngineAdapter::Instance().Time();

	this->SendAllInfo();
}

void UpgradeCardBuy::UpdateTodayGrade()
{
	for (auto &it : m_activity_bitflag_related_map)
	{
		short grade = this->GetGradeByThemeID(it.first);

		if (it.second.index >= 0 && it.second.index < UPGRADE_CARD_BUY_MAX_SYSTEM_COUNT)
		{
			m_param.today_upgrade_system_grade[it.second.index] = grade;
		}
	}
}

void UpgradeCardBuy::SendAllInfo()
{
	if (RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN))
	{
		this->SendUpgradeCardBuyInfo(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN);
	}
	if (RandActivityManager::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2))
	{
		this->SendUpgradeCardBuyInfo(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2);
	}
}

void UpgradeCardBuy::SendUpgradeCardBuyInfo(int activity_id)
{
	static Protocol::SCUpgradeCardBuyInfo msg;

	msg.activity_id = activity_id;

	switch (activity_id)
	{
	case RAND_ACTIVITY_TYPE_UPLEVEL_RETURN:
	{
		if (CrossConfig::Instance().IsHiddenServer())
		{
			return;
		}
		int theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN);
		int index = UpgradeCardBuy::GetIndexByThemeID(theme);
		msg.activity_id = RAND_ACTIVITY_TYPE_UPLEVEL_RETURN;

		{//客户端显示阶数
			short opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

			if (index < 0 || index >= UPGRADE_CARD_BUY_MAX_SYSTEM_COUNT)return;

			UpgradeCardInfo *cfg = UPGRADE_CARD_BUY_CFG().GetUpgradeCardInfo(activity_id, opengame_day, m_param.today_upgrade_system_grade[index], theme);
			if (nullptr != cfg)
			{
				msg.grade = cfg->grade;
			}
			else
			{
				msg.grade = m_param.today_upgrade_system_grade[index];
			}
		}

		msg.is_already_buy = IS_BIT_SET(m_param.ra_upgrade_card_buy_flag_1, index);
	}
	break;

	case RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2:
	{
		if (CrossConfig::Instance().IsHiddenServer())
		{
			return;
		}
		int theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2);
		int index = UpgradeCardBuy::GetIndexByThemeID(theme);
		msg.activity_id = RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2;

		{//客户端显示阶数
			short opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

			if (index < 0 || index >= UPGRADE_CARD_BUY_MAX_SYSTEM_COUNT)return;

			UpgradeCardInfo *cfg = UPGRADE_CARD_BUY_CFG().GetUpgradeCardInfo(activity_id, opengame_day, m_param.today_upgrade_system_grade[index], theme);
			if (nullptr != cfg)
			{
				msg.grade = cfg->grade;
			}
			else
			{
				msg.grade = m_param.today_upgrade_system_grade[index];
			}
		}

		msg.is_already_buy = IS_BIT_SET(m_param.ra_upgrade_card_buy_flag_2, index);
	}
	break;

	default:
		return;

	}

	SEND_TO_ROLE(m_role, msg);
}

void UpgradeCardBuy::OnUpgradeCardBuyReq(int activity_id, ItemID item_id)
{
	// 活动未开启
	if (!RandActivityManager::Instance().IsRandActivityOpen(activity_id))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_NOT_OPEN);
		return;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	int grade = 0;
	int theme = -1;
	int index = -1;

	short limit_opengame_day = 0;

	switch (activity_id)
	{
	case RAND_ACTIVITY_TYPE_UPLEVEL_RETURN:
	{
		theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN);
		index = UpgradeCardBuy::GetIndexByThemeID(theme);

		if (index < 0 || index >= UPGRADE_CARD_BUY_MAX_SYSTEM_COUNT)return;

		if (IS_BIT_SET(m_param.ra_upgrade_card_buy_flag_1, index))
		{
			m_role->NoticeNum(errornum::EN_UPGRADE_CARD_BUY_ALREADY_BUY);					
			return;
		}

		grade = m_param.today_upgrade_system_grade[index];

		limit_opengame_day = UPGRADE_CARD_BUY_CFG().GetOpengameDayLimit().opengame_day_limit;
	}
	break;

	case RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2:
	{
		theme = LOGIC_CONFIG->GetRandActivityCfg().GetJinJieThemeActTheme(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2);
		index = UpgradeCardBuy::GetIndexByThemeID(theme);

		if (index < 0 || index >= UPGRADE_CARD_BUY_MAX_SYSTEM_COUNT)return;

		if (IS_BIT_SET(m_param.ra_upgrade_card_buy_flag_2, index))
		{
			m_role->NoticeNum(errornum::EN_UPGRADE_CARD_BUY_ALREADY_BUY);				
			return;
		}

		grade = m_param.today_upgrade_system_grade[index];

		limit_opengame_day = UPGRADE_CARD_BUY_CFG().GetOpengameDayLimit().opengame_day_limit_2;
	}
	break;

	default:
		return;

	}

	short opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

	// 前X天不让购买
	//short limit_opengame_day = UPGRADE_CARD_BUY_CFG().GetOpengameDayLimit();
	if (opengame_day <= limit_opengame_day)
	{
		return;
	}

	UpgradeCardInfo *cfg = UPGRADE_CARD_BUY_CFG().GetUpgradeCardInfo(activity_id, opengame_day, grade, theme);
	if (nullptr == cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}

	// 请求购买的是否一样
	if (item_id != cfg->item_id)
	{
		m_role->NoticeNum(errornum::EN_ILLEGAL_OPERA);
		return;
	}

	// 背包空格
	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	// 检查消耗
	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(cfg->need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}
	m_role->GetKnapsack()->GetMoney()->UseGold(cfg->need_gold, "OnUpgradeCardBuyReq");

	// 给道具
	ItemConfigData card_item(cfg->item_id, true, 1);
	m_role->GetKnapsack()->Put(card_item, PUT_REASON_UPGRADE_CARD_BUY);

	switch (activity_id)
	{
	case RAND_ACTIVITY_TYPE_UPLEVEL_RETURN:
	{
		SET_BIT(m_param.ra_upgrade_card_buy_flag_1, index);
	}
	break;

	case RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2:
	{
		SET_BIT(m_param.ra_upgrade_card_buy_flag_2, index);
	}
	break;

	}

	this->SendUpgradeCardBuyInfo(activity_id);
}
