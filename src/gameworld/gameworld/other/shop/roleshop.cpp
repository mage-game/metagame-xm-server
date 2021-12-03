#include "roleshop.hpp"

#include "gamecommon.h"
#include "protocal/msgshop.h"
#include "obj/character/role.h"
#include "engineadapter.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "other/chestshop/rolechestshop.hpp"
#include "global/worldstatus/worldstatus.hpp"

#include <stdio.h>
#include "other/roleactivity/roleactivity.hpp"

RoleShop::RoleShop()
	: m_role(NULL)
{

}

RoleShop::~RoleShop()
{

}

void RoleShop::Init(Role *role, const RoleShopParam &param)
{
	m_role = role;
	m_param = param;
}

void RoleShop::GetInitParam(RoleShopParam *param)
{
	*param = m_param;
}

void RoleShop::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		JoinLimitUnit limit_unit;
		LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_CHESTSHOP, &limit_unit);

		if (m_role->GetLevel() >= limit_unit.level)
		{
			unsigned int now_s = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			m_param.chest_shop_next_free_time_1 = now_s;
			m_param.chest_shop_1_next_free_time_1 = now_s;
			m_param.chest_shop_2_next_free_time_1 = now_s;
			m_role->GetRoleChestShop()->SendFreeInfo();
		}
	}
}

void RoleShop::OnRoleLevelUp(int old_level, int role_level)
{
	JoinLimitUnit limit_unit;
	LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_CHESTSHOP, &limit_unit);

	if (old_level < limit_unit.level && role_level >= limit_unit.level)
	{
		unsigned int now_s = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_param.chest_shop_next_free_time_1 = now_s;
		m_param.chest_shop_1_next_free_time_1 = now_s;
		m_param.chest_shop_2_next_free_time_1 = now_s;
		m_param.chest_shop_jl_next_free_time_1 = now_s;

		m_role->GetRoleChestShop()->SendFreeInfo();
	}
}

unsigned int RoleShop::GetChestShopNextFreeTime(int mode)
{
	if (CHEST_SHOP_MODE_1 == mode)
	{
		return m_param.chest_shop_next_free_time_1;
	}

	else if (CHEST_SHOP_MODE1_1 == mode)
	{
		return m_param.chest_shop_1_next_free_time_1;
	}

	else if (CHEST_SHOP_MODE2_1 == mode)
	{
		return m_param.chest_shop_2_next_free_time_1;
	}

	else if(CHEST_SHOP_JL_MODE_1 == mode)
	{
		return m_param.chest_shop_jl_next_free_time_1;
	}

	return 0;
}

void RoleShop::SetChestShopNextFreeTime(int mode ,unsigned int next_time)
{
	if (CHEST_SHOP_MODE_1 == mode)
	{
		m_param.chest_shop_next_free_time_1 = next_time;
	}

	else if (CHEST_SHOP_MODE1_1 == mode)
	{
		m_param.chest_shop_1_next_free_time_1 = next_time;
	}

	else if (CHEST_SHOP_MODE2_1 == mode)
	{
		m_param.chest_shop_2_next_free_time_1 = next_time;
	}

	else if(CHEST_SHOP_JL_MODE_1 == mode)
	{
		m_param.chest_shop_jl_next_free_time_1 = next_time;
	}
}

bool RoleShop::AddChestShopMojing(int score, const char *type)
{
	m_param.chest_shop_mojing += score;
	if (m_param.chest_shop_mojing < 0)
	{
		m_param.chest_shop_mojing = 0;
	}

	m_role->GetRoleChestShop()->SendScoreInfoNotice(score);
	m_role->GetRoleChestShop()->SendScoreInfo();
	
	gamelog::g_log_chestshop.printf(LL_INFO, "ChestShopMojing: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, score, m_param.chest_shop_mojing);
	return true;
}

bool RoleShop::AddChestShopShengwang(int score , const char *type)
{
	m_param.chest_shop_shengwang += score;
	if (m_param.chest_shop_shengwang < 0)
	{
		m_param.chest_shop_shengwang = 0;
	}

	m_role->GetRoleChestShop()->SendScoreInfoNotice(0, score);
	m_role->GetRoleChestShop()->SendScoreInfo();
	m_role->GetRoleActivity()->OnRAExtremeChallengeAddPlan(RA_EXTREME_CHALLENGE_REPUTATION, score);

	gamelog::g_log_chestshop.printf(LL_INFO, "ChestShopShengwang: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, score, m_param.chest_shop_shengwang);
	return true;
}

bool RoleShop::AddChestShopGongXun(int score, const char *type)
{
	m_param.chest_shop_gongxun += score;
	if (m_param.chest_shop_gongxun < 0)
	{
		m_param.chest_shop_gongxun = 0;
	}

	m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, score);
	m_role->GetRoleChestShop()->SendScoreInfo();

	gamelog::g_log_chestshop.printf(LL_INFO, "ChestShopGongXun: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, score, m_param.chest_shop_gongxun);
	return true;
}

bool RoleShop::AddChestShopWeiWang(int score, const char *type)
{
	m_param.chest_shop_weiwang += score;
	if (m_param.chest_shop_weiwang < 0)
	{
		m_param.chest_shop_weiwang = 0;
	}

	m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, score);
	m_role->GetRoleChestShop()->SendScoreInfo();

	gamelog::g_log_chestshop.printf(LL_INFO, "ChestShopWeiWang: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, score, m_param.chest_shop_weiwang);
	return true;
}

bool RoleShop::AddChestShopHunjing(int score, const char *type)
{
	m_param.chest_shop_hunjing += score;
	if (m_param.chest_shop_hunjing < 0)
	{
		m_param.chest_shop_hunjing = 0;
	}

	m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, score);
	m_role->GetRoleChestShop()->SendScoreInfo();

	gamelog::g_log_chestshop.printf(LL_INFO, "ChestShopHunjing: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, score, m_param.chest_shop_hunjing);
	return true;
}

bool RoleShop::AddChestShopTreasureCredit(int score, const char *type)
{
	m_param.chest_shop_treasure_credit += score;
	if (m_param.chest_shop_treasure_credit < 0)
	{
		m_param.chest_shop_treasure_credit = 0;
	}

	m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, score);
	m_role->GetRoleChestShop()->SendScoreInfo();

	gamelog::g_log_chestshop.printf(LL_INFO, "ChestShopTreasureHuntCredit: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, score, m_param.chest_shop_treasure_credit);
	return true;
}

bool RoleShop::AddChestShopTreasureCredit1(int score, const char *type)
{
	m_param.chest_shop_treasure_credit1 += score;
	if (m_param.chest_shop_treasure_credit1 < 0)
	{
		m_param.chest_shop_treasure_credit1 = 0;
	}

	m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, score);
	m_role->GetRoleChestShop()->SendScoreInfo();

	gamelog::g_log_chestshop.printf(LL_INFO, "ChestShopTreasureHuntCredit1: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, score, m_param.chest_shop_treasure_credit1);
	return true;
}

bool RoleShop::AddChestShopTreasureCredit2(int score, const char *type)
{
	m_param.chest_shop_treasure_credit2 += score;
	if (m_param.chest_shop_treasure_credit2 < 0)
	{
		m_param.chest_shop_treasure_credit2 = 0;
	}

	m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, score);
	m_role->GetRoleChestShop()->SendScoreInfo();

	gamelog::g_log_chestshop.printf(LL_INFO, "ChestShopTreasureHuntCredit2: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, score, m_param.chest_shop_treasure_credit2);
	return true;
}

bool RoleShop::AddChestShopTreasureCredit3(int score, const char *type)
{
	m_param.chest_shop_treasure_credit3 += score;
	if (m_param.chest_shop_treasure_credit3 < 0)
	{
		m_param.chest_shop_treasure_credit3 = 0;
	}

	m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, score);
	m_role->GetRoleChestShop()->SendScoreInfo();

	gamelog::g_log_chestshop.printf(LL_INFO, "ChestShopTreasureHuntCredit3: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, score, m_param.chest_shop_treasure_credit3);
	return true;
}

bool RoleShop::AddChestShopJingLingCredit(int score, const char *type)
{
	m_param.chest_shop_jingling_credit += score;
	if (m_param.chest_shop_jingling_credit < 0)
	{
		m_param.chest_shop_jingling_credit = 0;
	}

	m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, 0, score);
	m_role->GetRoleChestShop()->SendScoreInfo();

	gamelog::g_log_chestshop.printf(LL_INFO, "ChestShopJingLingCredit: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, score, m_param.chest_shop_jingling_credit);
	return true;
}

bool RoleShop::AddChestShopHappyTreeGrow(int score, const char *type)
{
	m_param.chest_shop_happytree_grow += score;
	if (m_param.chest_shop_happytree_grow < 0)
	{
		m_param.chest_shop_happytree_grow = 0;
	}

	m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, 0, 0, score);
	m_role->GetRoleChestShop()->SendScoreInfo();

	gamelog::g_log_chestshop.printf(LL_INFO, "ChestShopHappyTreeGrow: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, score, m_param.chest_shop_happytree_grow);
	return true;
}

bool RoleShop::AddChestShopMysteriousShopCredit(int score, const char *type)
{
	m_param.chest_shop_mysterious_credit += score;
	if (m_param.chest_shop_mysterious_credit < 0)
	{
		m_param.chest_shop_mysterious_credit = 0;
	}

	m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, 0, 0, 0, score);
	m_role->GetRoleChestShop()->SendScoreInfo();

	gamelog::g_log_chestshop.printf(LL_INFO, "ChestMysteriousShopCredit: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, score, m_param.chest_shop_mysterious_credit);
	return true;
}

bool RoleShop::AddChestShopHunyinLingzhi(int blue_lingzhi, int purple_lingzhi, int orange_lingzhi, const char *type)
{
	if (0 != blue_lingzhi)
	{
		m_param.chest_shop_bule_lingzhi += blue_lingzhi;
		if (m_param.chest_shop_bule_lingzhi < 0)
		{
			m_param.chest_shop_bule_lingzhi = 0;
		}

		m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, 0, 0, 0, 0, blue_lingzhi);
		m_role->GetRoleChestShop()->SendScoreInfo();

		gamelog::g_log_chestshop.printf(LL_INFO, "ChestShopHappyTreeGrow: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, blue_lingzhi, m_param.chest_shop_bule_lingzhi);
	}

	if (0 != purple_lingzhi)
	{
		m_param.chest_shop_purple_linzhi += purple_lingzhi;
		if (m_param.chest_shop_purple_linzhi < 0)
		{
			m_param.chest_shop_purple_linzhi = 0;
		}

		m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, 0, 0, 0, 0, 0, purple_lingzhi);
		m_role->GetRoleChestShop()->SendScoreInfo();

		gamelog::g_log_chestshop.printf(LL_INFO, "ChestShopHappyTreeGrow: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, purple_lingzhi, m_param.chest_shop_bule_lingzhi);
	}

	if (0 != orange_lingzhi)
	{
		m_param.chest_shop_orange_linzhi += orange_lingzhi;
		if (m_param.chest_shop_orange_linzhi < 0)
		{
			m_param.chest_shop_orange_linzhi = 0;
		}

		m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, orange_lingzhi);
		m_role->GetRoleChestShop()->SendScoreInfo();

		gamelog::g_log_chestshop.printf(LL_INFO, "ChestShopHappyTreeGrow: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, orange_lingzhi, m_param.chest_shop_orange_linzhi);
	}

	return true;
}

bool RoleShop::AddChestShopGuanghui(int score, const char *type)
{
	m_param.chest_shop_guanghui += score;
	if (m_param.chest_shop_guanghui < 0)
	{
		m_param.chest_shop_guanghui = 0;
	}

	m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0 , score);
	m_role->GetRoleChestShop()->SendScoreInfo();

	gamelog::g_log_chestshop.printf(LL_INFO, "ChestMysteriousShopCredit: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(),type, score, m_param.chest_shop_guanghui);
	return true;
}

bool RoleShop::AddChestShopPreciousBossScore(int score, const char *type)
{
	m_param.chest_shop_precious_score += score;
	if (m_param.chest_shop_precious_score < 0)
	{
		m_param.chest_shop_precious_score = 0;
	}

	m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, score);
	m_role->GetRoleChestShop()->SendScoreInfo();

	gamelog::g_log_chestshop.printf(LL_INFO, "ChestPreciousBossScore: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, score, m_param.chest_shop_precious_score);
	return true;
}

bool RoleShop::AddChestShopShenZhouWeaponCredit(int score, const char *type)
{
	m_param.chest_shop_shenzhouweapon_score += score;
	if (m_param.chest_shop_shenzhouweapon_score < 0)
	{
		m_param.chest_shop_shenzhouweapon_score = 0;
	}

	m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, score);
	m_role->GetRoleChestShop()->SendScoreInfo();

	gamelog::g_log_chestshop.printf(LL_INFO, "ChestShenZhouWeaponCredit: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]", m_role->GetUID(), m_role->GetName(), type, score, m_param.chest_shop_shenzhouweapon_score);
	return true;
}

bool RoleShop::IsFirstEquipBeHited(int shop_type)
{
	return IS_BIT_SET(m_param.first_equip_is_behited_flag, shop_type);
}

void RoleShop::SetFirstEquipBeHited(int shop_type)
{
	SET_BIT(m_param.first_equip_is_behited_flag, shop_type);
}

bool RoleShop::AddCrossEquipChuanshiScore(int score, const char *type)
{
// 	m_param.cross_equip_chuanshi_score += score;
// 	if (m_param.cross_equip_chuanshi_score < 0)
// 	{
// 		m_param.cross_equip_chuanshi_score = 0;
// 	}
// 
// 	m_role->GetRoleChestShop()->SendScoreInfoNotice(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, score);
// 	m_role->GetRoleChestShop()->SendScoreInfo();
// 
// 	gamelog::g_log_chestshop.printf(LL_INFO, "AddCrossEquipChuanshiScore: uid:%d, name:%s, type:%s, change_value[%d], now_value[%d]",
// 		m_role->GetUID(), m_role->GetName(), type, score, m_param.cross_equip_chuanshi_score);
	return true;
}
