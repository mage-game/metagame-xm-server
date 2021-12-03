#include "other/roleactivity/roleactivity.hpp"
#include "world.h"
#include "engineadapter.h"
#include "gamecommon.h"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "scene/activityshadow/activityshadow.hpp"

#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"

#include "servercommon/noticenum.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/string/globalstr.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "globalconfig/globalconfig.h"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "global/team/team.hpp"
#include "other/vip/vip.hpp"

// 玩家上线时检查上次记录的活动时间是不是处在当开启的活动时间内（即是不是同一场活动）
// 如果不需不是则 活动需要重置数据，并且看需要补发奖励
void RoleActivity::RandActivityOnRoleLogin()
{
	if (m_role->IsInCross())
	{
		return;
	}

	// 尝试清空上一场的活动，且补发奖励
	{
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_CONSUME_GOLD_FANLI, RACheckConsumeGoldFanliGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_CHESTSHOP, RACheckChestshopGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_STONE_UPLEVEL, RACheckStoneTotalLevelGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_MOUNT_UPGRADE, RACheckMountUpgradeGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_MENTALITY_TOTAL_LEVEL, RACheckMentalityTotalLevelGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_WING_UPGRADE, RACheckWingUpgradeGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_QUANMIN_QIFU, RACheckQuanminQifuGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_TOTAL_CONSUME_GOLD, RACheckTotalConsumeGoldGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_KILL_BOSS, RACheckKillBossReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_LOGIN_GIFT, RACheckLoginGiftGiveReward);
 		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL, RACheckBipinCapTotalGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING, RACheckBipinCapWingGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT, RACheckBipinCapMountGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP, RACheckBipinCapEquipGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV, RACheckBipinCapXiaNvGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI, RACheckBipinCapJingMaiGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG, RACheckBipinCapShenzhuangGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING, RACheckBipinCapJinglingGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH, RACheckBipinCapWashGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_DAY, RACheckTotalChargeDayGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_TOTAL_CHARGE, RACheckTotalChargeGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_PASSIVE_RANK, RACheckSpecialAppearacePassiveJoinReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_RANK, RACheckSpecialAppearaceJoinReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_FANFAN, RACheckFanFanGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI, RACheckContinueChongzhiGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU, RACheckContinueChongzhiGiveRewardChu);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO, RACheckContinueChongzhiGiveRewardGao);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2, RACheckContinueChongzhiGiveReward2);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_CONTINUE_CONSUME, RACheckContinueConsumeGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_ARMY_DAY, OnRaCheckArmyDayWinnerGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI, RACheckCirculationChongzhiGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_ROLE_UPLEVEL, RACheckRoleUplevelGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_PATA, RACheckPataGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_EXP_FB, RACheckExpFbGiveReweard);
		//RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_MOUNT, RACheckUpgradeMountGiveReweard);
		//RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_HALO, RACheckUpgradeHaloGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK, RACheckShizhuangUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK, RACheckShenbinUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_WING, RACheckUpgradeWingGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG, RACheckUpgradeShengongGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_SHENYI, RACheckUpgradeShenyiGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN, RACheckFirstChargeTuanUpgradeGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_DAY_TOTAL_CHARGE, RACheckDayTotalChargeUpgradeGiveReweard);
		//RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_TOTAL, RACheckMountUpgradeTotalGiveReweard);
		//RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_HALO_TOTAL, RACheckHaloUpgradeTotalGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK, RACheckFabaoUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK, RACheckFootprintUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_WING_TOTAL, RACheckWingUpgradeTotalGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_TOTAL, RACheckShengongUpgradeTotalGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_TOTAL, RACheckShenyiUpgradeTotalGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK, RACheckMountUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK, RACheckHaloUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK, RACheckWingUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK, RACheckShengongUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK, RACheckShenyiUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK, RACheckQilinbiUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK, RACheckToushiUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK, RACheckYaoshiUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK, RACheckLingtongUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK, RACheckLinggongUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK, RACheckLingqiUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL, RACheckEquipStrengthTotalLevelGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL, RACheckStoneTotalLevelGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK, RACheckEquipStrengthTotalLevelRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK, RACheckStoneTotalLevelRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_BOSS_KILLER, RACheckBossKillerGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_XIANNV_RANK, RACheckXianNvTotalRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_JINGLING_RANK, RACheckJingLingTotalRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK, RACheckFightMountTotalRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK, RACheckRoleTotalCapRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_SINGLE_CHONGZHI, OnRASingleChongZhiGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_KING_DRAW, RACheckKingdrawReturnReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_LEVEL_LOTTERY, RACheckLevelLotteryReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_ZHENBAOGE, RACheckZhenbaogeReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_MONEY_TREE, RACheckMoneyTreeReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE, RACheckNewTotalChargeGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1, RACheckNewTotalCharge1GiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2, RACheckNewTotalCharge2GiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3, RACheckNewTotalCharge3GiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_PLANTING_TREE, RACheckPlantingTreeCanyuReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_GUILDBATTLE, OnOGACheckGuildBattleReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_MIJINGXUNBAO3, ReissueMiJingXunBao3FetchPersonalReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_LIMIT_TIME_REBATE, RACheckLimitTimeRebateReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_TIME_LIMIT_GIFT, RACheckTimeLimitGiftReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI_2, RACheckCirculationChongzhiSecondReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_SHAKE_MONEY, RACheckShakeMoneyGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD, RACheckConsumeGoldRewardAutoFetch);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_VERSIONS_CONTINUE_CHARGE, RACheckVersionContinueChongzhiGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_VERSIONS_GRAND_TOTAL_CHARGE, RACheckVersionTotalChargeGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2, RACheckHuanLeYaoJiang2FetchPersonalReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_OFFLINE_SINGLE_CHARGE_0, CheckRAOfflineSingleChargeReward0);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_BUY_ONE_GET_ONE_FREE, RACheckBuyOneGetOneFreeReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_LOGIN_GIFT_0, RACheckLoginGiftGiveReward_0);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT, OnRAEverydayNiceGiftReissueReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_GUAGUA, RACheckGuaGuaAccReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_HOLIDAY_GUARD, OnHolidayGuardKillMonsterGiveReward)
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE, OnRAExtremeChallengeGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_CONSUM_GIFT, RACheckConsumGiftReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_HAPPY_ACCUMUL_RECHARGE, RACheckHappyCumulChongzhiReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_CHONGZHI_GIFT, RACheckChongZhiGiftGiveReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG, ReissueHuanLeYaoJiangFetchPersonalReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_HUANLE_ZADAN, ReissueHuanLeZaDanFetchPersonalReward);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_CHARGE_REPAYMENT, RACheckChangeRepayment);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK, RACheckFlypetUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK, RACheckWeiyanUpgradeRankGiveReweard);
		RAND_CHECKINIT_AND_GIVE_REWRAD(RAND_ACTIVITY_TYPE_KUANG_HAI_QING_DIAN, CheckRAKuangHaiQingDianGiveReward);
		
	}

	// 尝试清空上一场的活动，不用补发奖励
	{
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CORNUCOPIA);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DANBI_CHONGZHI);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DAY_CONSUME_RANK);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DAY_CHONGZHI_RANK);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MONEY_TREE);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_KING_DRAW);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_3);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_EXTREME_LUCKY);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_MONEY_TREE2);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_GOLDEN_PIG);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_FISHING);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CRYSTAL_DALARAN);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CRACYBUY);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_PROFESS_RANK);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DAILY_LOVE_2);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CHONGZHI_RANK);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_CHONGZHI_RANK2);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_EQUIP);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_JL);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DAILY_LOVE);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_RED_ENVELOPE_GIFT);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_DOUBLE_GET);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SINGLE_CHARGE1);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SINGLE_CHARGE2);
		this->CheckInitRandActivity(RAND_ACTIVITY_TYPE_SINGLE_CHARGE3);
	}
}

void RoleActivity::CheckInitRandActivity(int rand_act_type, bool is_force)
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(rand_act_type))	// not in activity
	{
		return;
	}

	if (RAND_ACTIVITY_TYPE_OPEN_SERVER_BEGIN <= rand_act_type && rand_act_type <= RAND_ACTIVITY_TYPE_OPEN_SERVER_END)
	{
		int sub_index = rand_act_type - RAND_ACTIVITY_TYPE_OPEN_SERVER_BEGIN;
		if (sub_index >= 0 && sub_index < OPEN_SERVER_ACTIVITY_COUNT)
		{
			OpenServerSubActivityInfo &info = m_activity_info.open_server_sub_info_list[sub_index];
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(rand_act_type, info.join_timestamp);
			if (is_force || !in_activity_time)
			{
				info.join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				info.reward_flag = 0;

				if (RAND_ACTIVITY_TYPE_BOSS_KILLER == rand_act_type)
				{
					m_activity_info.oga_kill_boss_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
					m_activity_info.oga_kill_boss_reward_flag = 0;
					m_activity_info.oga_kill_boss_flag = 0;
				}
				this->RACheckActivityOpenNotification();
			}
		}
	}

	else
	{
		switch (rand_act_type)
		{
		case RAND_ACTIVITY_TYPE_DAY_CONSUME_RANK:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_DAY_CONSUME_RANK,
				m_activity_info.ra_day_consume_rank_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_day_consume_rank_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_day_consume_rank_gold_num = 0;
				m_activity_info.ra_day_consume_rank_consume_last_timestamp = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_DAY_CHONGZHI_RANK:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_DAY_CHONGZHI_RANK,
				m_activity_info.ra_day_chongzhi_rank_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_day_chongzhi_rank_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_day_chongzhi_rank_gold_num = 0;
				m_activity_info.ra_day_chongzhi_rank_chongzhi_last_timestamp = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT,
				m_activity_info.ra_bipin_cap_mount_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckBipinCapMountGiveReweard();
				m_activity_info.ra_bipin_cap_mount_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_bipin_cap_mount_history_max_value = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING,
				m_activity_info.ra_bipin_cap_wing_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckBipinCapWingGiveReweard();
				m_activity_info.ra_bipin_cap_wing_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_bipin_cap_wing_history_max_value = 0;

			}
		}
		break;

		case RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL,
				m_activity_info.ra_bipin_cap_total_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckBipinCapTotalGiveReweard();
				m_activity_info.ra_bipin_cap_total_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_bipin_cap_total_history_max_value = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP,
				m_activity_info.ra_bipin_cap_equip_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckBipinCapEquipGiveReweard();
				m_activity_info.ra_bipin_cap_equip_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_bipin_cap_equip_history_max_value = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV,
				m_activity_info.ra_bipin_cap_xiannv_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_bipin_cap_xiannv_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				this->RACheckBipinCapXiaNvGiveReweard();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI,
				m_activity_info.ra_bipin_cap_jingmai_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckBipinCapJingMaiGiveReweard();
				m_activity_info.ra_bipin_cap_jingmai_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_bipin_cap_jingmai_history_max_value = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_LUCKY_ROLL:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_LUCKY_ROLL,
				m_activity_info.lucky_roll_activity_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.lucky_roll_activity_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.lucky_roll_activity_roll_times = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CHONGZHI_RANK:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CHONGZHI_RANK,
				m_activity_info.ra_chongzhi_rank_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_chongzhi_rank_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_chongzhi_rank_chongzhi_num = 0;
				m_activity_info.ra_chongzhi_rank_chongzhi_last_timestamp = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CHONGZHI_RANK2:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CHONGZHI_RANK2,
				m_activity_info.ra_chongzhi_rank2_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_chongzhi_rank2_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_chongzhi_rank2_chongzhi_num = 0;
				m_activity_info.ra_chongzhi_rank2_chongzhi_last_timestamp = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK,
				m_activity_info.ra_consume_gold_rank_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_consume_gold_rank_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_consume_gold_rank_consume_gold_num = 0;
				m_activity_info.ra_consume_gold_rank_consume_last_timestamp = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CONSUME_GOLD_FANLI:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CONSUME_GOLD_FANLI,
				m_activity_info.ra_consume_gold_fanli_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckConsumeGoldFanliGiveReward();

				m_activity_info.ra_consume_gold_fanli_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_consume_gold_fanli_consume_gold = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_SERVER_PANIC_BUY:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_SERVER_PANIC_BUY,
				m_activity_info.ra_server_panic_buy_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_server_panic_buy_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				memset(m_activity_info.ra_server_panic_buy_numlist, 0, sizeof(m_activity_info.ra_server_panic_buy_numlist));
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_PERSONAL_PANIC_BUY:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_PERSONAL_PANIC_BUY,
				m_activity_info.ra_personal_panic_buy_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_personal_panic_buy_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				memset(m_activity_info.ra_personal_panic_buy_numlist, 0, sizeof(m_activity_info.ra_personal_panic_buy_numlist));
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_DAY_CHONGZHI_FANLI:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_DAY_CHONGZHI_FANLI,
				m_activity_info.ra_day_chongzhi_fanli_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckDayChongZhiFanLiReward();

				m_activity_info.ra_day_chongzhi_fanli_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_day_chongzhi_fanli_chongzhi_num = 0;
				m_activity_info.ra_day_chongzhi_fanli_fetch_reward_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_DAY_CONSUME_GOLD:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_DAY_CONSUME_GOLD,
				m_activity_info.ra_day_consume_gold_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckDayConsumeGoldReward();

				m_activity_info.ra_day_consume_gold_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_day_consume_gold_consume_num = 0;
				m_activity_info.ra_day_consume_gold_fetch_reward_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CHESTSHOP:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CHESTSHOP,
				m_activity_info.ra_chestshop_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckChestshopGiveReward();

				m_activity_info.ra_chestshop_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_chestshop_fetch_reward_flag = 0;
				m_activity_info.ra_chestshop_xunbao_times = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_TOTAL_CONSUME_GOLD:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_TOTAL_CONSUME_GOLD,
				m_activity_info.ra_total_consume_gold_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckTotalConsumeGoldGiveReward();

				m_activity_info.ra_total_consume_gold_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_total_consume_gold_consume_num = 0;
				m_activity_info.ra_total_consume_gold_fetch_reward_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_DAY_ACTIVIE_DEGREE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_DAY_ACTIVIE_DEGREE,
				m_activity_info.ra_day_active_degree_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckDayActiveDegreeGiveReward();

				m_activity_info.ra_day_active_degree_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_day_active_degree_active_count = 0;
				m_activity_info.ra_day_active_degree_fetch_reward_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_STONE_UPLEVEL:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_STONE_UPLEVEL,
				m_activity_info.ra_stone_uplevel_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckStoneTotalLevelGiveReward();

				m_activity_info.ra_stone_uplevel_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_stone_uplevel_can_reward_flag = 0;

				this->CheckRAStoneUplevelReward();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_MOUNT_UPGRADE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_MOUNT_UPGRADE,
				m_activity_info.ra_mount_upgrade_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckMountUpgradeGiveReward();

				m_activity_info.ra_mount_upgrade_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_mount_upgrade_can_reward_flag = 0;

				this->CheckRAMountUpgradeReward();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_MENTALITY_TOTAL_LEVEL:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_MENTALITY_TOTAL_LEVEL,
				m_activity_info.ra_mentality_total_level_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckMentalityTotalLevelGiveReward();

				m_activity_info.ra_mentality_total_level_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_mentality_total_level_can_reward_flag = 0;

				this->CheckRAMentalityTotalLevelReward();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_WING_UPGRADE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_WING_UPGRADE,
				m_activity_info.ra_wing_upgrade_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckWingUpgradeGiveReward();

				m_activity_info.ra_wing_upgrade_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_wing_upgrade_can_reward_flag = 0;

				this->CheckRAWingUpgradeReward();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_KILL_BOSS:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_KILL_BOSS,
				m_activity_info.ra_kill_boss_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckKillBossReward();

				m_activity_info.ra_kill_boss_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_kill_boss_kill_count = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_QUANMIN_QIFU:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_QUANMIN_QIFU,
				m_activity_info.ra_quanmin_qifu_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckQuanminQifuGiveReward();

				m_activity_info.ra_quanmin_qifu_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_quanmin_qifu_fetch_reward_flag = 0;
				m_activity_info.ra_quanmin_qifu_qifu_times = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_LOGIN_GIFT:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_LOGIN_GIFT,
				m_activity_info.ra_login_gift_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckLoginGiftGiveReward();

				m_activity_info.ra_login_gift_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_login_gift_login_days = 1;
				m_activity_info.ra_login_gift_last_login_dayid = EngineAdapter::Instance().DayID();
				m_activity_info.ra_login_gift_has_fetch_accumulate_reward = 0;
				m_activity_info.ra_login_gift_fetch_common_reward_flag = 0;
				m_activity_info.ra_login_gift_fetch_vip_reward_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_LOGIN_GIFT_0:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_LOGIN_GIFT_0,
				m_activity_info.ra_login_gift_begin_timestamp_0);
			if (is_force || !in_activity_time)
			{
				this->RACheckLoginGiftGiveReward_0();

				m_activity_info.ra_login_gift_begin_timestamp_0 = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_login_gift_login_days_0 = 1;
				m_activity_info.ra_login_gift_last_login_dayid_0 = EngineAdapter::Instance().DayID();
				m_activity_info.ra_login_gift_fetch_common_reward_flag_0 = 0;
				m_activity_info.ra_login_gift_fetch_vip_reward_flag_0 = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_SHOUYOU_YUXIANG:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_SHOUYOU_YUXIANG,
				m_activity_info.ra_shouyou_yuxiang_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_shouyou_yuxiang_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_shouyou_yuxiang_fetch_reward_flag = 0;
				m_activity_info.ra_shouyou_yuxiang_give_flowers_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CHARGE_REPAYMENT:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CHARGE_REPAYMENT,
				m_activity_info.ra_charge_repayment_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_charge_repayment_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_charge_repayment_reward_active_flag = 0;
				m_activity_info.ra_charge_repayment_reward_fetch_flag = 0;
				m_activity_info.ra_charge_repayment_history_charge = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CORNUCOPIA:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CORNUCOPIA,
				m_activity_info.ra_cornucopia_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_cornucopia_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_cornucopia_history_charge = 0;
				m_activity_info.ra_cornucopia_current_lun = 1;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_DANBI_CHONGZHI:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_DANBI_CHONGZHI,
				m_activity_info.ra_danbi_chongzhi_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_danbi_chongzhi_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_danbi_chongzhi_reward_can_fetch_flag = 0;
				m_activity_info.ra_danbi_chongzhi_reward_has_fetch_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_TOTAL_CHARGE_DAY:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_DAY,
				m_activity_info.ra_total_charge_day_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckTotalChargeDayGiveReward();

				m_activity_info.ra_total_charge_day_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_total_charge_day_reward_has_fetch_flag = 0;
				m_activity_info.ra_total_charge_day_count = 0;
				m_activity_info.ra_total_charge_day_today_has_charged = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_TOMORROW_REWARD:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_TOMORROW_REWARD,
				m_activity_info.ra_tomorrow_reward_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_tomorrow_reward_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_tomorrow_reward_count = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_TOTAL_CHARGE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_TOTAL_CHARGE,
				m_activity_info.ra_total_charge_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckTotalChargeGiveReward();

				m_activity_info.ra_total_charge_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_total_charge_value = 0;
				m_activity_info.ra_total_charge_reward_has_fetch_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_DOUBLE_SCORE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_DOUBLE_SCORE,
				m_activity_info.ra_double_score_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_double_score_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_EQUIP:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_EQUIP,
				m_activity_info.ra_time_limit_exchange_equi_begin_tiemstamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_time_limit_exchange_equi_begin_tiemstamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				memset(m_activity_info.ra_time_limit_exchange_exchange_equi_times_list, 0, sizeof(m_activity_info.ra_time_limit_exchange_exchange_equi_times_list));
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_JL:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_JL,
				m_activity_info.ra_time_limit_exchange_jl_begin_tiemstamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_time_limit_exchange_jl_begin_tiemstamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				memset(m_activity_info.ra_time_limit_exchange_exchange_jl_times_list, 0, sizeof(m_activity_info.ra_time_limit_exchange_exchange_jl_times_list));
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_LEVEL_LOTTERY:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_LEVEL_LOTTERY,
				m_activity_info.ra_level_lottery_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_level_lottery_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_level_lottery_cur_level = 0;
				m_activity_info.ra_level_lottery_free_buy_times = 0;
				m_activity_info.ra_lottery_next_free_timestamp = 0;
				m_activity_info.ra_lottery_buy_total_times = 0;
				m_activity_info.ra_lottery_fetch_reward_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG,
				m_activity_info.ra_niu_egg_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_niu_egg_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_niu_egg_cur_chongzhi_value = 0;
				m_activity_info.ra_niu_egg_server_reward_has_fetch_reward_flag = 0;
				m_activity_info.ra_niu_egg_times = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG,
				m_activity_info.ra_bipin_cap_shenzhuang_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckBipinCapShenzhuangGiveReweard();
				m_activity_info.ra_bipin_cap_shenzhuang_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_bipin_cap_shenzhuang_history_max_value = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING,
				m_activity_info.ra_bipin_cap_jingling_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckBipinCapJinglingGiveReweard();
				m_activity_info.ra_bipin_cap_jingling_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_bipin_cap_jingling_history_max_value = 0;
			}
		}
		break;
		case RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH,
				m_activity_info.ra_bipin_cap_wash_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckBipinCapWashGiveReweard();
				m_activity_info.ra_bipin_cap_wash_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_bipin_cap_wash_history_max_value = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_ZHENBAOGE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_ZHENBAOGE,
				m_activity_info.ra_zhenbaoge_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_zhenbaoge_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_zhenbaoge_next_free_flush_timestamp = m_activity_info.ra_zhenbaoge_timestamp;
				m_activity_info.ra_zhenbaoge_refresh_gift_fetch_flag = 0;
				m_activity_info.ra_zhenbaoge_refresh_times = 0;
				memset(m_activity_info.ra_zhenbaoge_item_list, 0, sizeof(m_activity_info.ra_zhenbaoge_item_list));

				for (int i = 0; i < RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT; ++i)
				{
					m_activity_info.ra_zhenbaoge_add_weight_list[i].seq = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaogeAddWeightSeq(i);
					m_activity_info.ra_zhenbaoge_add_weight_list[i].weight = 0;
				}
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_DAY_CHONGZHI_DOUBLE_2:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_DAY_CHONGZHI_DOUBLE_2,
				m_activity_info.ra_charge_reward_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_charge_reward_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_charge_reward_is_first = 1;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_MIJINGXUNBAO:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_MIJINGXUNBAO,
				m_activity_info.ra_mijingxunbao_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_mijingxunbao_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_mijingxunbao_next_free_tao_timestamp = 0;
				m_activity_info.ra_mijingxunbao_personal_reward_flag = 0;
				m_activity_info.ra_mijingxunbao_role_chou_times = 0;
				int auto_add_weight_cfg_count = LOGIC_CONFIG->GetRandActivityCfg().GetMiJingXunBaoAutoWeightAddCount();

				for (int i = 0; i < auto_add_weight_cfg_count; ++i)
				{
					m_activity_info.ra_mijingxunbao_add_weight_list[i].seq = LOGIC_CONFIG->GetRandActivityCfg().GetMiJingXunBaoAddWeightSeq(i);
					m_activity_info.ra_mijingxunbao_add_weight_list[i].weight = 0;
					m_activity_info.ra_mijingxunbao_add_weight_list[i].tao_none_rare_times = 0;
				}
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_MIJINGXUNBAO2:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_MIJINGXUNBAO2,
				m_activity_info.ra_mijingxunbao2_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_mijingxunbao2_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_mijingxunbao2_next_free_tao_timestamp = 0;
				int auto_add_weight_cfg_count = LOGIC_CONFIG->GetRandActivityCfg().GetMiJingXunBao2AutoWeightAddCount();

				for (int i = 0; i < auto_add_weight_cfg_count; ++i)
				{
					m_activity_info.ra_mijingxunbao2_add_weight_list[i].seq = LOGIC_CONFIG->GetRandActivityCfg().GetMiJingXunBao2AddWeightSeq(i);
					m_activity_info.ra_mijingxunbao2_add_weight_list[i].weight = 0;
					m_activity_info.ra_mijingxunbao2_add_weight_list[i].tao_none_rare_times = 0;
				}
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_JISUCHPONGZHAN:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_JISUCHPONGZHAN,
				m_activity_info.ra_jisuchongzhan_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_jisuchongzhan_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_MONEY_TREE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_MONEY_TREE,
				m_activity_info.ra_money_tree_last_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_money_tree_last_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_money_tree_server_reward_has_fetch_reward_flag = 0;
				m_activity_info.ra_money_tree_total_times = 0;
				m_activity_info.ra_money_tree_free_timestamp = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_MONEY_TREE2:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_MONEY_TREE2,
				m_activity_info.ra_money_tree2_last_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_money_tree2_last_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_money_tree2_server_reward_has_fetch_reward_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_DAILY_LOVE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_DAILY_LOVE,
				m_activity_info.ra_daily_love_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_daily_love_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_daily_love_daily_first_flag = 1;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_KING_DRAW:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_KING_DRAW,
				m_activity_info.ra_kingdraw_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_kingdraw_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				memset(m_activity_info.ra_kingdraw_draw_times, 0, sizeof(m_activity_info.ra_kingdraw_draw_times));
				m_activity_info.ra_kingdraw_reward_flag = 0;
				memset(m_activity_info.ra_kingdraw_reward_score, 0, sizeof(m_activity_info.ra_kingdraw_reward_score));

				for (int level_index = 0; level_index < RA_KING_DRAW_LEVEL_COUNT; ++level_index)
				{
					m_activity_info.ra_kingdraw_card_list[level_index].Reset();

					const int self_weight_cfg_count = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawSelfWeigthCfgCount(level_index);
					for (int i = 0; i < self_weight_cfg_count && i < RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT; ++i)
					{
						const RandActivitySelfWeightConfig *cfg_weight = LOGIC_CONFIG->GetRandActivityCfg().GetKingDrawSelfWeightCfg(level_index, i);
						if (NULL != cfg_weight)
						{
							m_activity_info.ra_kingdraw_self_weight_list[level_index][i].seq = cfg_weight->seq;
							m_activity_info.ra_kingdraw_self_weight_list[level_index][i].auto_weight_add = 0;
						}
					}
				}
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_TOTAL_CHARGE_3:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_3,
				m_activity_info.ra_total_charge_3_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckTotalCharge3RewardCheck();

				m_activity_info.ra_total_charge_3_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_total_charge_3_cur_has_fetch_flag = 0;
				m_activity_info.ra_total_charge_3_cur_total_charge = 0;

				this->OnSendTotalCharge3Info();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_PASSIVE_RANK:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_PASSIVE_RANK,
				m_activity_info.ra_special_appearance_card_passive_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckSpecialAppearacePassiveJoinReward(); // 参与奖励

				m_activity_info.ra_special_appearance_card_passive_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_special_appearance_card_passive_use_times = 0;
				m_activity_info.ra_special_appearance_passive_has_got_join_reward = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_RANK:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_RANK,
				m_activity_info.ra_special_appearance_card_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckSpecialAppearaceJoinReward(); // 参与奖励

				m_activity_info.ra_special_appearance_card_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_special_appearance_card_use_times = 0;
				m_activity_info.ra_special_appearance_has_got_join_reward = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4,
				m_activity_info.ra_total_charge_4_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckTotalCharge4RewardCheck();

				m_activity_info.ra_total_charge_4_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_total_charge_4_cur_has_fetch_flag = 0;
				m_activity_info.ra_total_charge_4_cur_total_charge = 0;

				this->OnSendTotalCharge4Info();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_MINE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_MINE,
				m_activity_info.ra_mine_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_mine_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_mine_next_refresh_time = m_activity_info.ra_mine_join_timestamp;
				m_activity_info.ra_mine_cur_reward_fetch_flag = 0;
				m_activity_info.ra_mine_role_refresh_times = 0;
				m_activity_info.ra_mine_free_gather_times = LOGIC_CONFIG->GetRandActivityCfg().GetMineFreeGatherTimes();
				memset(m_activity_info.ra_mine_gather_count_list, 0, sizeof(m_activity_info.ra_mine_gather_count_list));
				//重置ra_mine_type_list
				this->OnRAMineRefresh(false);
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_GUAGUA:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_GUAGUA,
				m_activity_info.ra_guagua_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_guagua_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_guagua_next_free_time = 0;
				m_activity_info.ra_guagua_acc_count = 0;
				m_activity_info.ra_guagua_acc_reward_has_fetch_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_TIANMING_DIVINATION:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_TIANMING_DIVINATION,
				m_activity_info.ra_tianming_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_tianming_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_tianming_free_chou_times = 0;

				for (int i = 0; i < RA_TIANMING_LOT_COUNT; ++i)
				{
					m_activity_info.ra_tianming_add_lot_times_list[i] = 1;
				}

				m_activity_info.ra_tianming_reward_history_list_cur_index = 0;

				for (int i = 0; i < RA_TIANMING_REWARD_HISTORY_COUNT; ++i)
				{
					m_activity_info.ra_tianming_reward_history_list[i].Reset();
				}
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_FANFAN:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_FANFAN,
				m_activity_info.ra_fanfan_join_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckFanFanGiveReward();

				m_activity_info.ra_fanfan_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

				m_activity_info.ra_fanfan_cur_free_times = LOGIC_CONFIG->GetRandActivityCfg().GetFanFanFreeTimesPerDay();
				m_activity_info.ra_fanfan_next_refresh_time = 0;
				m_activity_info.ra_fanfan_cur_letter_occur_rate = 0;
				m_activity_info.ra_fanfan_role_refresh_times = 0;
				m_activity_info.ra_fanfan_hidden_word_info.Reset();

				m_activity_info.ra_fanfan_reward_flag = 0;
				m_activity_info.ra_fanfan_leichou_times = 0;

				for (int index = 0; index < RA_FANFAN_MAX_WORD_COUNT; ++index)
				{
					m_activity_info.ra_fanfan_word_active_info_list[index].Reset();
				}

				for (int index = 0; index < RA_FANFAN_CARD_COUNT; ++index)
				{
					m_activity_info.ra_fanfan_card_type_list[index] = RA_FANFAN_CARD_TYPE_HIDDEN;
				}

				this->OnRAFanFanRefresh(false);
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI,
				m_activity_info.ra_continue_chongzhi_join_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckContinueChongzhiGiveReward();

				m_activity_info.ra_continue_chongzhi_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_continue_chongzhi_today_chongzhi = 0;
				m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag = 0;
				m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag = 0;
				m_activity_info.ra_continue_chongzhi_days = 0;
				m_activity_info.ra_continue_chongzhi_is_activity_over = 0;

				if (GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI), m_activity_info.ra_continue_chongzhi_join_timestamp) >= 1)
				{
					m_activity_info.ra_continue_chongzhi_is_activity_over = 1;
				}
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU,
				m_activity_info.ra_continue_chongzhi_join_timestamp_chu);
			if (is_force || !in_activity_time)
			{
				this->RACheckContinueChongzhiGiveRewardChu();

				m_activity_info.ra_continue_chongzhi_join_timestamp_chu = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_continue_chongzhi_today_chongzhi_chu = 0;
				m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_chu = 0;
				m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_chu = 0;
				m_activity_info.ra_continue_chongzhi_days_chu = 0;
				m_activity_info.ra_is_lei_chongzhi_chu = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO,
				m_activity_info.ra_continue_chongzhi_join_timestamp_gao);
			if (is_force || !in_activity_time)
			{
				this->RACheckContinueChongzhiGiveRewardGao();

				m_activity_info.ra_continue_chongzhi_join_timestamp_gao = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_continue_chongzhi_today_chongzhi_gao = 0;
				m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_gao = 0;
				m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_gao = 0;
				m_activity_info.ra_continue_chongzhi_days_gao = 0;
				m_activity_info.ra_is_lei_chongzhi_gao = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2,
				m_activity_info.ra_continue_chongzhi_join_timestamp_2);
			if (is_force || !in_activity_time)
			{
				this->RACheckContinueChongzhiGiveReward2();

				m_activity_info.ra_continue_chongzhi_join_timestamp_2 = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_continue_chongzhi_today_chongzhi_2 = 0;
				m_activity_info.ra_continue_chongzhi_can_fetch_reward_flag_2 = 0;
				m_activity_info.ra_continue_chongzhi_has_fetch_reward_flag_2 = 0;
				m_activity_info.ra_continue_chongzhi_days_2 = 0;
				m_activity_info.ra_continue_chongzhi_2_is_lei_chongzhi = 0;

				// 			if (GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2), m_activity_info.ra_continue_chongzhi_join_timestamp_2) >= 1)
				// 			{
				// 				m_activity_info.ra_continue_chongzhi_2_is_activity_over = 1;
				// 			}
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CONTINUE_CONSUME:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CONTINUE_CONSUME,
				m_activity_info.ra_continue_consume_join_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckContinueConsumeGiveReward();

				m_activity_info.ra_continue_consume_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_continue_consume_today_consume_gold_total = 0;
				m_activity_info.ra_continue_consume_cur_consume_gold = 0;
				m_activity_info.ra_continue_consume_total_days = 0;
				m_activity_info.ra_continue_consume_days = 0;
				m_activity_info.ra_continue_consume_extra_reward_num = 0;
				m_activity_info.ra_continue_consume_the_last_time_consume_day_index = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_ARMY_DAY:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_ARMY_DAY,
				m_activity_info.ra_army_day_join_timestamp);
			if (is_force || !in_activity_time)
			{
				this->OnRaCheckArmyDayWinnerGiveReward();

				m_activity_info.ra_army_day_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_army_day_belong_army_side = INVALID_ARMY_SIDE;
				m_activity_info.ra_army_day_active_degree_exchange_flag = 0;
				memset(m_activity_info.ra_army_day_flags_list, 0, sizeof(m_activity_info.ra_army_day_flags_list));
				memset(m_activity_info.ra_army_day_score_list, 0, sizeof(m_activity_info.ra_army_day_score_list));
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI,
				m_activity_info.ra_circulation_chongzhi_join_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckCirculationChongzhiGiveReward();

				m_activity_info.ra_circulation_chongzhi_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_circulation_chongzhi_total_chongzhi = 0;
				m_activity_info.ra_circulation_chongzhi_cur_chongzhi_gold = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_EXTREME_LUCKY:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_EXTREME_LUCKY,
				m_activity_info.ra_extreme_lucky_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_extreme_lucky_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_extreme_lucky_next_flush_timestamp = 0;
				m_activity_info.ra_extreme_lucky_day_free_draw_times = 0;
				m_activity_info.ra_extreme_lucky_flush_times = 0;
				m_activity_info.ra_extreme_lucky_cur_total_draw_times = 0;
				m_activity_info.ra_extreme_lucky_cur_gold_draw_times = 0;
				m_activity_info.ra_extreme_lucky_total_draw_times = 0;
				m_activity_info.ra_extreme_lucky_return_reward_flag = 0;
				memset(m_activity_info.ra_extreme_lucky_item_list, 0, sizeof(m_activity_info.ra_extreme_lucky_item_list));

				// 依赖以上数据
				this->OnRAExtremeLuckyFlush(true);
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_NATIONAL_DAY:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_NATIONAL_DAY,
				m_activity_info.ra_national_day_last_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_national_day_last_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_ZHENBAOGE2:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_ZHENBAOGE2,
				m_activity_info.ra_zhenbaoge2_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_zhenbaoge2_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_zhenbaoge2_next_free_flush_timestamp = m_activity_info.ra_zhenbaoge2_timestamp;
				m_activity_info.ra_zhenbaoge2_server_fetch_flag = 0;
				m_activity_info.ra_zhenbaoge2_flush_times = 0;
				memset(m_activity_info.ra_zhenbaoge2_item_list, 0, sizeof(m_activity_info.ra_zhenbaoge2_item_list));

				for (int i = 0; i < RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT; ++i)
				{
					m_activity_info.ra_zhenbaoge2_add_weight_list[i].seq = LOGIC_CONFIG->GetRandActivityCfg().GetZhenbaoge2AddWeightSeq(i);
					m_activity_info.ra_zhenbaoge2_add_weight_list[i].weight = 0;
				}
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_PROMOTING_POSITION:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_PROMOTING_POSITION,
				m_activity_info.ra_promoting_position_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_promoting_position_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_promoting_position_next_free_timestamp = m_activity_info.ra_promoting_position_join_timestamp;
				m_activity_info.ra_promoting_position_extra_times = 0;
				m_activity_info.ra_promoting_position_start_pos.Reset();
				m_activity_info.ra_promoting_position_chongzhi_gold = 0;
				m_activity_info.ra_promoting_position_play_times = 0;
				m_activity_info.ra_promoting_position_reward_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_TREASURES_MALL:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_TREASURES_MALL,
				m_activity_info.ra_treasures_mall_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_treasures_mall_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_treasures_mall_score = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_RED_ENVELOPE_GIFT:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_RED_ENVELOPE_GIFT,
				m_activity_info.ra_red_envelope_gift_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_red_envelope_gift_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				memset(m_activity_info.ra_red_envelope_gift_consume_gold_num_list, 0, sizeof(m_activity_info.ra_red_envelope_gift_consume_gold_num_list));
				m_activity_info.ra_red_envelope_gift_reward_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_EXP_REFINE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_EXP_REFINE,
				m_activity_info.refine_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.refine_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.refine_today_buy_time = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_GOLDEN_PIG:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_GOLDEN_PIG,
				m_activity_info.ra_golden_pig_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_golden_pig_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_golden_pig_chongzhi = 0;
				m_activity_info.ra_golden_pig_summon_credit = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_SINGLE_CHONGZHI:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_SINGLE_CHONGZHI, m_activity_info.ra_single_chongzhi_join_timestamp);
			if (is_force || !in_activity_time)
			{
				this->OnRASingleChongZhiGiveReward();
				m_activity_info.ra_single_chongzhi_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

				m_activity_info.ra_single_chongzhi_reward_flag = 0;
				m_activity_info.ra_single_chongzhi_is_reward_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_XIANYUAN_TREAS:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_XIANYUAN_TREAS,
				m_activity_info.ra_xianyuan_treas_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckXianyuanTreasMailReward();		// 检查最后奖励

				m_activity_info.ra_xianyuan_treas_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_xianyuan_treas_all_buy_gift_fetch_flag = 0;

				for (int i = 0; i < RAND_ACTIVITY_XIANYUAN_TREAS_CFG_COUNT; ++i)
				{
					m_activity_info.ra_xianyuan_treas_list[i].buy_day_index = 0;
					m_activity_info.ra_xianyuan_treas_list[i].num = 0;
				}
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_RUSH_BUYING:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_RUSH_BUYING,
				m_activity_info.ra_rush_buying_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_rush_buying_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_rush_buying_cur_pahse = LOGIC_CONFIG->GetRandActivityCfg().GetRushBuyingPhase(m_activity_info.ra_rush_buying_join_timestamp);
				memset(m_activity_info.ra_rush_buying_item_buying_times, 0, sizeof(m_activity_info.ra_rush_buying_item_buying_times));
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_MAP_HUNT:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_MAP_HUNT,
				m_activity_info.ra_map_hunt_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_map_hunt_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_map_hunt_next_flush_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_map_hunt_cur_route_info.Reset();
				m_activity_info.ra_map_hunt_has_used_free_xunbao_count = 0;
				m_activity_info.ra_map_hunt_return_reward_fetch_flag = 0;
				m_activity_info.ra_map_hunt_play_times = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE,
				m_activity_info.ra_light_tower_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_light_tower_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				memset(m_activity_info.ra_light_tower_info, 0, sizeof(m_activity_info.ra_light_tower_info));
				m_activity_info.ra_light_tower_layer_extern_reward_fetch_flag = 0;
				m_activity_info.ra_light_tower_return_reward_fetch_flag = 0;
				m_activity_info.ra_light_tower_cur_index = 0;
				m_activity_info.ra_light_tower_cur_layer = 0;
				m_activity_info.ra_light_tower_draw_times = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE,
				m_activity_info.ra_new_total_charge_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckNewTotalChargeGiveReward();

				m_activity_info.ra_new_total_charge_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_new_total_charge_value = 0;
				m_activity_info.ra_new_total_charge_reward_has_fetch_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1,
				m_activity_info.ra_new_total_charge1_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckNewTotalCharge1GiveReward();

				m_activity_info.ra_new_total_charge1_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_new_total_charge1_value = 0;
				m_activity_info.ra_new_total_charge1_reward_has_fetch_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2,
				m_activity_info.ra_new_total_charge2_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckNewTotalCharge2GiveReward();

				m_activity_info.ra_new_total_charge2_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_new_total_charge2_value = 0;
				m_activity_info.ra_new_total_charge2_reward_has_fetch_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3,
				m_activity_info.ra_new_total_charge3_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckNewTotalCharge3GiveReward();

				m_activity_info.ra_new_total_charge3_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_new_total_charge3_value = 0;
				m_activity_info.ra_new_total_charge3_reward_has_fetch_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_MAGIC_SHOP:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_MAGIC_SHOP,
				m_activity_info.ra_magic_shop_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_magic_shop_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_magic_shop_buy_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_PLANTING_TREE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_PLANTING_TREE,
				m_activity_info.ra_planting_tree_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_planting_tree_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_planting_tree_planting_times = 0;
				m_activity_info.ra_planting_tree_watering_times = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_FISHING:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_FISHING,
				m_activity_info.ra_fishing_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_fishing_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_fishing_times = 0;
				m_activity_info.ra_steal_fish_times = 0;
				m_activity_info.ra_be_steal_fish_times = 0;
				m_activity_info.ra_fishing_score = 0;
				m_activity_info.ra_steal_fish_score = 0;
				memset(m_activity_info.ra_fish_info_list, 0, sizeof(m_activity_info.ra_fish_info_list));
				m_activity_info.ra_fishing_steal_timestamp = 0;
				m_activity_info.ra_fishing_fishing_timestamp = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CRYSTAL_DALARAN:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CRYSTAL_DALARAN,
				m_activity_info.ra_fishing_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_crystal_dalaran_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_crystal_dalaran_count = 0;
				m_activity_info.ra_steal_crystal_dalaran_times = 0;
				m_activity_info.ra_crystal_dalaran_be_stealed_times = 0;
				m_activity_info.ra_steal_crystal_dalaran_timestamp = 0;
				m_activity_info.ra_crystal_dalaran_be_stealed_timestamp = 0;

				m_activity_info.ra_steal_dalaran_count = 0;
				m_activity_info.ra_dalaran_be_stealed_count = 0;
				m_activity_info.ra_be_stealed_info->Reset();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CHONGZHI_CRAZY_REBATE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CHONGZHI_CRAZY_REBATE,
				m_activity_info.ra_chongzhi_crazy_rebate_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_chongzhi_crazy_rebate_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_chongzhi_crazy_rebate_day_chongzhi_total_count = 0;
				m_activity_info.ra_chongzhi_crazy_rebate_had_rebete_count = 0;
				this->SendRaCrazyRebateChongzhiInfo();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_PERFECT_LOVER:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_PERFECT_LOVER, m_activity_info.ra_perfect_lover_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_perfect_lover_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_perfect_lover_type_record_flag = 0;
				m_activity_info.ra_perfect_lover_type_fetch_flag = 0;
				m_activity_info.ra_perfect_lover_last_marry_role_id = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_QUANMINJINJIE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_QUANMINJINJIE, m_activity_info.ra_upgrade_reward_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_upgrade_reward_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_upgrade_reward_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_QUANMINZONGDONGYUAN:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_QUANMINZONGDONGYUAN, m_activity_info.ra_upgrade_group_reward_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_upgrade_group_reward_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_upgrade_group_reward_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CONSUM_GIFT:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CONSUM_GIFT,
				m_activity_info.ra_consume_gift_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_consume_gift_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());;
				m_activity_info.ra_consume_gift_consume_gold = 0;
				m_activity_info.ra_consume_gift_today_join_times = 0;
				m_activity_info.ra_consume_gift_reward_flag = 0;
				m_activity_info.ra_consume_gift_join_theme = 0;
				m_activity_info.ra_consume_gift_join_opengame_day = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_BUY_LIMIT:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_BUY_LIMIT,
				m_activity_info.ra_limit_buy_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_limit_buy_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				memset(m_activity_info.ra_limit_buy_had_buy_count, 0, sizeof(m_activity_info.ra_limit_buy_had_buy_count));
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_COLLECT_TREASURE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_COLLECT_TREASURE,
				m_activity_info.ra_collect_treasure_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_collect_treasure_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_collect_treasure_can_join_times = 0;
				m_activity_info.ra_collect_treasure_join_times = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_HAPPY_ACCUMUL_RECHARGE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_HAPPY_ACCUMUL_RECHARGE,
				m_activity_info.ra_happy_cumul_chongzhi_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_happy_cumul_chongzhi_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_happy_cumul_chongzhi_num = 0;
				m_activity_info.ra_happy_cumul_chongzhi_fetch_flag = 0;
				m_activity_info.ra_happy_cumul_chongzhi_join_theme = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_UPLEVEL_RETURN:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN,
				m_activity_info.ra_jinjie_return_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_jinjie_return_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_jinjie_return_fetch_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT,
				m_activity_info.ra_everyday_nice_gift_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->OnRAEverydayNiceGiftReissueReward();

				m_activity_info.ra_everyday_nice_gift_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_everyday_nice_gift_can_fetch_reward_flag = 0;
				m_activity_info.ra_everyday_nice_gift_have_fetch_reward_flag = 0;
				m_activity_info.ra_everyday_nice_gift_is_have_recharge_standard = 0;
				m_activity_info.ra_everyday_nice_gift_reserve_sh = 0;
				m_activity_info.ra_everyday_nice_gift_everydaty_recharge = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CONSUME_FOR_GIFT:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CONSUME_FOR_GIFT,
				m_activity_info.ra_consume_for_gift_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_consume_for_gift_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_consume_for_gift_total_consume_gold = 0;
				m_activity_info.ra_consume_for_gift_cur_points = 0;
				memset(m_activity_info.ra_consume_for_gift_item_exchange_times, 0, sizeof(m_activity_info.ra_consume_for_gift_item_exchange_times));
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_MIJINGXUNBAO3:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_MIJINGXUNBAO3,
				m_activity_info.ra_mijingxunbao3_timestamp);
			if (is_force || !in_activity_time)
			{
				this->ReissueMiJingXunBao3FetchPersonalReward();

				m_activity_info.ra_mijingxunbao3_daily_free_times = LOGIC_CONFIG->GetRandActivityCfg().GetMiJingXunBao3FreeTimes();
				m_activity_info.ra_mijingxunbao3_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_mijingxunbao3_next_free_tao_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_mijingxunbao3_role_chou_times = 0;
				m_activity_info.ra_mijingxunbao3_personal_reward_flag = 0;
				int auto_add_weight_cfg_count = LOGIC_CONFIG->GetRandActivityCfg().GetMiJingXunBao3AutoWeightAddCount();

				for (int i = 0; i < auto_add_weight_cfg_count; ++i)
				{
					m_activity_info.ra_mijingxunbao3_add_weight_list[i].seq = LOGIC_CONFIG->GetRandActivityCfg().GetMiJingXunBao3AddWeightSeq(i);
					m_activity_info.ra_mijingxunbao3_add_weight_list[i].weight = 0;
					m_activity_info.ra_mijingxunbao3_add_weight_list[i].tao_none_rare_times = 0;
				}
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_LIMIT_TIME_REBATE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_LIMIT_TIME_REBATE,
				m_activity_info.ra_limit_time_rebate_join_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckLimitTimeRebateReward();

				m_activity_info.ra_limit_time_rebate_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_limit_time_rebate_cur_day_is_chongzhi = 0;
				m_activity_info.ra_limit_time_rebate_total_chongzhi_days = 0;
				m_activity_info.ra_limit_time_rebate_reward_flag = 0;
				m_activity_info.ra_limit_time_rebate_cur_day_chongzhi = 0;

				memset(m_activity_info.ra_limit_time_rebate_chongzhi_list, 0, sizeof(m_activity_info.ra_limit_time_rebate_chongzhi_list));
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_TIME_LIMIT_GIFT:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_TIME_LIMIT_GIFT,
				m_activity_info.ra_time_limit_gift_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckTimeLimitGiftReward();

				m_activity_info.ra_time_limit_gift_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_time_limit_gift_join_vip_level = m_role->GetVip()->GetVipLevel();
				m_activity_info.ra_time_limit_gift_open_flag = 1;
				m_activity_info.ra_time_limit_gift_reward_can_fetch_flag1 = 0;
				m_activity_info.ra_time_limit_gift_reward_has_fetch_flag1 = 0;
				m_activity_info.ra_time_limit_gift_reward_can_fetch_flag2 = 0;
				m_activity_info.ra_time_limit_gift_reward_has_fetch_flag2 = 0;
				m_activity_info.ra_time_limit_gift_reward_can_fetch_flag3 = 0;
				m_activity_info.ra_time_limit_gift_reward_has_fetch_flag3 = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI_2:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI_2,
				m_activity_info.ra_circulation_chongzhi_2_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_circulation_chongzhi_2_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_circulation_chongzhi_2_cur_chongzhi_gold = 0;
				m_activity_info.ra_circulation_chongzhi_2_total_chongzhi_gold = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_SHAKE_MONEY:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_SHAKE_MONEY, m_activity_info.ra_shakemoney_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckShakeMoneyGiveReward();

				m_activity_info.ra_shakemoney_history_chongzhi_num = 0;
				m_activity_info.ra_shakemoney_fetched_num = 0;
				m_activity_info.ra_shakemoney_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_LIMIT_LUXURY_GIFT_BAG_GIFT:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_LIMIT_LUXURY_GIFT_BAG_GIFT,
				m_activity_info.ra_time_limit_luxury_gift_bag_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_time_limit_luxury_gift_open_flag = 1;
				m_activity_info.ra_time_limit_luxury_gift_bag_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_time_limit_luxury_gift_bag_join_vip_level = m_role->GetVip()->GetVipLevel();
				m_activity_info.ra_time_limit_luxury_gift_bag_is_already_buy = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_DOUBLE_CHONGZHI:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_DOUBLE_CHONGZHI, m_activity_info.ra_reset_double_chongzhi_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_reset_double_chongzhi_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_reset_double_chongzhi_reward_flag = 0;
				m_activity_info.ra_reset_double_chongzhi_open_flag = 1;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_RMB_BUT_CHEST_SHOP:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_RMB_BUT_CHEST_SHOP,
				m_activity_info.ra_rmb_buy_chest_shop_join_timestamp);

			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_rmb_buy_chest_shop_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				memset(m_activity_info.ra_rmb_buy_chest_shop_buy_count_list, 0, sizeof(m_activity_info.ra_rmb_buy_chest_shop_buy_count_list));
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD,
				m_activity_info.ra_consume_gold_reward_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_consume_gold_reward_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_consume_gold_reward_fetch_flag = 0;
				m_activity_info.ra_consume_gold_reward_consume_gold = 0;
				m_activity_info.ra_consume_gold_reward_activity_day = 0;
				m_activity_info.ra_consume_gold_reward_today_vip_level = m_role->GetVip()->GetVipLevel();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_ITEM_COLLECTION_2:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_ITEM_COLLECTION_2,
				m_activity_info.collection_exchange_2_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.collection_exchange_2_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				memset(m_activity_info.collection_exchange_2_times, 0, sizeof(m_activity_info.collection_exchange_2_times));
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_VERSIONS_GRAND_TOTAL_CHARGE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_VERSIONS_GRAND_TOTAL_CHARGE,
				m_activity_info.ra_version_total_charge_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckVersionTotalChargeGiveReward();

				m_activity_info.ra_version_total_charge_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_version_total_charge_value = 0;
				m_activity_info.ra_version_total_charge_reward_has_fetch_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_VERSIONS_CONTINUE_CHARGE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_VERSIONS_CONTINUE_CHARGE,
				m_activity_info.ra_version_continue_chongzhi_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_version_continue_chongzhi_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_version_continue_chongzhi_today_chongzhi = 0;
				m_activity_info.ra_version_continue_chongzhi_can_fetch_reward_flag = 0;
				m_activity_info.ra_version_continue_chongzhi_has_fetch_reward_flag = 0;
				m_activity_info.ra_version_continue_chongzhi_days = 0;
				m_activity_info.ra_version_is_lei_chongzhi = 0;
				m_activity_info.ra_version_continue_chongzhi_reserve_sh = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2,
				m_activity_info.ra_huanleyaojiang_2_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckHuanLeYaoJiang2FetchPersonalReward();

				m_activity_info.ra_huanleyaojiang_2_daily_free_times = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeYaoJiang2FreeTimes();
				m_activity_info.ra_huanleyaojiang_2_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_huanleyaojiang_2_next_free_tao_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_huanleyaojiang_2_role_chou_times = 0;
				m_activity_info.ra_huanleyaojiang_2_personal_reward_flag = 0;

				int auto_add_weight_cfg_count = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeYaoJiang2AutoWeightAddCount();

				for (int i = 0; i < auto_add_weight_cfg_count; ++i)
				{
					m_activity_info.ra_huanleyaojiang_2_add_weight_list[i].seq = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeYaoJiang2AddWeightSeq(i);
					m_activity_info.ra_huanleyaojiang_2_add_weight_list[i].weight = 0;
					m_activity_info.ra_huanleyaojiang_2_add_weight_list[i].tao_none_rare_times = 0;
				}
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_OFFLINE_SINGLE_CHARGE_0:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_OFFLINE_SINGLE_CHARGE_0,
				m_activity_info.ra_offline_single_charge_join_timestamp_0);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_offline_single_charge_join_timestamp_0 = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_offline_single_charge_max_value_0 = -1;
				memset(m_activity_info.ra_offline_single_charge_reward_times_list_0, 0, sizeof(m_activity_info.ra_offline_single_charge_reward_times_list_0));
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_BUY_ONE_GET_ONE_FREE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_BUY_ONE_GET_ONE_FREE,
				m_activity_info.ra_buy_one_get_one_free_join_timestamp);
			if (is_force || !in_activity_time)
			{
				this->RACheckBuyOneGetOneFreeReward();

				m_activity_info.ra_buy_one_get_one_free_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_buy_one_get_one_free_buy_flag = 0;
				m_activity_info.ra_buy_one_get_one_free_fetch_reward_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_HOLIDAY_GUARD:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_HOLIDAY_GUARD,
				m_activity_info.ra_holiday_guard_participate_timestamp);
			if (is_force || !in_activity_time)
			{
				this->OnHolidayGuardKillMonsterGiveReward();

				m_activity_info.ra_holiday_guard_participate_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_holiday_guard_kill_monster_count = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_HUANLE_ZADAN:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_HUANLE_ZADAN,
				m_activity_info.ra_huanlezadan_timestamp);
			if (is_force || !in_activity_time)
			{
				this->ReissueHuanLeZaDanFetchPersonalReward();

				m_activity_info.ra_huanlezadan_daily_free_times = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeZaDanFreeTimes();
				m_activity_info.ra_huanlezadan_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_huanlezadan_next_free_tao_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_huanlezadan_role_chou_times = 0;
				m_activity_info.ra_huanlezadan_personal_reward_flag = 0;
				int auto_add_weight_cfg_count = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeZaDanAutoWeightAddCount();

				for (int i = 0; i < auto_add_weight_cfg_count; ++i)
				{
					m_activity_info.ra_huanlezadan_add_weight_list[i].seq = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeZaDanAddWeightSeq(i);
					m_activity_info.ra_huanlezadan_add_weight_list[i].weight = 0;
					m_activity_info.ra_huanlezadan_add_weight_list[i].tao_none_rare_times = 0;
				}
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG,
				m_activity_info.ra_huanleyaojiang_timestamp);
			if (is_force || !in_activity_time)
			{
				this->ReissueHuanLeYaoJiangFetchPersonalReward();

				m_activity_info.ra_huanleyaojiang_daily_free_times = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeYaoJiangFreeTimes();
				m_activity_info.ra_huanleyaojiang_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_huanleyaojiang_next_free_tao_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_huanleyaojiang_role_chou_times = 0;
				m_activity_info.ra_huanleyaojiang_personal_reward_flag = 0;

				int auto_add_weight_cfg_count = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeYaoJiangAutoWeightAddCount();

				for (int i = 0; i < auto_add_weight_cfg_count; ++i)
				{
					m_activity_info.ra_huanleyaojiang_add_weight_list[i].seq = LOGIC_CONFIG->GetRandActivityCfg().GetHuanLeYaoJiangAddWeightSeq(i);
					m_activity_info.ra_huanleyaojiang_add_weight_list[i].weight = 0;
					m_activity_info.ra_huanleyaojiang_add_weight_list[i].tao_none_rare_times = 0;
				}
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_TOTAL_CHARGE_FIVE:
		{
				bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_FIVE,
					m_activity_info.ra_total_charge_5_begin_timestamp);
				if (is_force || !in_activity_time)
				{
					this->RACheckTotalCharge5RewardCheck();

					m_activity_info.ra_total_charge_5_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
					m_activity_info.ra_total_charge_5_join_day_index = GetDayIndex(ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_FIVE), m_activity_info.ra_total_charge_5_begin_timestamp);
					m_activity_info.ra_total_charge_5_cur_has_fetch_flag = 0;
					m_activity_info.ra_total_charge_5_cur_total_charge = 0;

					this->OnSendTotalCharge5Info();
				}
		}
		break;

		case RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE,
				m_activity_info.ra_extreme_challenge_timestamp);
			if (is_force || !in_activity_time)
			{
				this->OnRAExtremeChallengeGiveReward();

				m_activity_info.ra_extreme_challenge_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				memset(m_activity_info.ra_extreme_challenge_task_info_list, 0, sizeof(m_activity_info.ra_extreme_challenge_task_info_list));
				m_activity_info.ra_extreme_challenge_last_update_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_extreme_challenge_ultimate_have_fetch = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CHONGZHI_GIFT:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CHONGZHI_GIFT,
				m_activity_info.ra_chongzhi_gift_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_chongzhi_gift_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_magic_shop_chongzhi_value = 0;
				m_activity_info.ra_magic_shop_fetch_reward_flag = 0;
				m_activity_info.ra_chongzhi_gift_join_act_day = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_PROFESS_RANK:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_PROFESS_RANK,
				m_activity_info.ra_profess_rank_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_profess_rank_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_profess_score = 0;
				m_activity_info.ra_profess_to_num = 0;
				m_activity_info.ra_profess_from_num = 0;
				m_activity_info.ra_profess_rank_last_profess_timestamp = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2,
				m_activity_info.ra_jinjie_return2_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_jinjie_return2_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_jinjie_return2_fetch_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_COMBINE_BUY:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_COMBINE_BUY,
				m_activity_info.ra_combine_buy_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_combine_buy_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

				memset(m_activity_info.ra_combine_buy_item_buy_times_list, 0, sizeof(m_activity_info.ra_combine_buy_item_buy_times_list));
				memset(m_activity_info.ra_combine_buy_bucket_item_seq_list, -1, sizeof(m_activity_info.ra_combine_buy_bucket_item_seq_list));
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_LUCKY_WISH:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_LUCKY_WISH,
				m_activity_info.ra_lucky_wish_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_lucky_wish_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				this->OnRALuckyWishResetLuckyValue();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_LOTTERY_1:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_LOTTERY_1,
				m_activity_info.ra_lottery1_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_lottery1_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_lottery1_next_free_timestamp = 0;
				m_activity_info.ra_lottery1_fetch_person_reward_flag = 0;
				m_activity_info.ra_lottery1_person_chou_times = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_DAILY_LOVE_2:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_DAILY_LOVE_2, m_activity_info.ra_daily_love_2_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_daily_love_2_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_daily_love_daily_2_has_charge = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_KUANG_HAI_QING_DIAN:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_KUANG_HAI_QING_DIAN,
				m_activity_info.ra_kuang_hai_qing_dian_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->CheckRAKuangHaiQingDianGiveReward();

				m_activity_info.ra_kuang_hai_qing_dian_begin_timestamp = 0;
				m_activity_info.ra_kuang_hai_qing_dian_total_score = 0;
				m_activity_info.ra_kuang_hai_qing_dian_fetch_reward_flag = 0;
				memset(m_activity_info.ra_kuang_hai_qing_dian_score_list, 0, sizeof(m_activity_info.ra_kuang_hai_qing_dian_score_list));
				memset(m_activity_info.ra_kuang_hai_qing_dian_task_info_list, 0, sizeof(m_activity_info.ra_kuang_hai_qing_dian_task_info_list));
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_GIFT_HARVEST:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_GIFT_HARVEST,
				m_activity_info.ra_gift_harvest_begin_timestamp);
			if (is_force || !in_activity_time)
			{
				this->CheckRAGiftHarvestGiveReward();

				m_activity_info.ra_gift_harvest_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_gift_harvest_score = 0;
				m_activity_info.ra_gift_harvest_get_score_times = 0;
				m_activity_info.ra_gift_harvest_reserve_sh = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_MARRY_ME:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_MARRY_ME,
				m_activity_info.ra_marry_me_join_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_marry_me_join_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_marry_me_is_fetch_reward = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_LUCKY_CLOUD_BUY:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_LUCKY_CLOUD_BUY,
				m_activity_info.ra_lucky_cloud_buy_timestamp);
			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_lucky_cloud_buy_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				this->CheckRALuckyCloudBuyReturn();
				this->ResetLuckyCloudBuy();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_CRACYBUY:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_CRACYBUY,
				m_activity_info.ra_cracy_buy_begin_timestamp);

			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_cracy_buy_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_cracy_buy_chongzhi = 0;
				m_activity_info.ra_cracy_buy_level = -1;
				memset(m_activity_info.ra_cracy_buy_limit, 0, sizeof(m_activity_info.ra_cracy_buy_limit));
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_DOUBLE_GET:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_DOUBLE_GET,
				m_activity_info.ra_double_get_begin_timestamp);

			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_double_get_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_double_get_reward_fetch_flag = 0;
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_SINGLE_CHARGE1:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_SINGLE_CHARGE1,
				m_activity_info.ra_single_charge1_begin_timestamp);

			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_single_charge1_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_single_charge1_reward_fetch_flag = 0;
				this->SendSingleChargeInfoOne();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_SINGLE_CHARGE2:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_SINGLE_CHARGE2,
				m_activity_info.ra_single_charge2_begin_timestamp);

			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_single_charge2_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_single_charge2_reward_fetch_flag = 0;
				this->SendSingleChargeInfoTwo();
			}
		}
		break;

		case RAND_ACTIVITY_TYPE_SINGLE_CHARGE3:
		{
			bool in_activity_time = ActivityShadow::Instance().IsInRandActivityOpenTime(RAND_ACTIVITY_TYPE_SINGLE_CHARGE3,
				m_activity_info.ra_single_charge3_begin_timestamp);

			if (is_force || !in_activity_time)
			{
				m_activity_info.ra_single_charge3_begin_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_activity_info.ra_single_charge3_reward_fetch_flag = 0;
				this->SendSingleChargeInfoThree();
			}
		}
		break;

		default:
			break;
		}
	}
}