#include "randactivityconfig.hpp"
#include "gamecommon.h"
#include "servercommon/servercommon.h"
#include "item/itempool.h"
#include "servercommon/configcommon.h"
#include "config/sharedconfig/sharedconfig.h"
#include "engineadapter.h"

#include <string>
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/chongzhiconfig.hpp"
#include "monster/monsterpool.h"

#include "gameworld/scene/activityshadow/activityshadow.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "global/randactivity/impl/randactivitymijingxunbao3.hpp"
#include "global/randactivity/impl/randactivityhuanlezadan.hpp"
#include "global/randactivity/impl/randactivityhuanleyaojiang.hpp"
#include <algorithm>
#include "global/randactivity/impl/randactivityhuanleyaojiang2.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "randactivityconfig.hpp"

RandActivityConfig::RandActivityConfig() : m_consume_gold_fanli_rate(0), m_chestshop_discount(0), m_quanmin_qifu_discount(0),
	m_rank_bipin_cfg_count(0), m_all_bipin_cfg_count(0), m_cornucopia_max_lun(0), m_total_charge_day_max_seq(0),
	m_tomorrow_reward_need_active_degree(0), m_tomorrow_reward_cfg_total_weight(0), m_tomorrow_reward_cfg_count(0),
	m_level_lottery_max_level(0), m_level_lottery_consume_item(0),m_level_lottery_cfg_list_count(0), m_niu_egg_need_charge(0), m_niu_egg_chou_total_weight(0),
	m_niu_egg_chou_reward_cfg_count(0), m_niu_egg_server_reward_cfg_count(0), m_zhenbaoge_reflush_gold(0), m_zhenbaoge_flush_item(0), m_zhenbaoge_auto_flush_times(0),
	m_zhenbaoge_auto_add_cfg_count(0), m_zhenbaoge_max_seq(0), m_zhenbaoge_reward_max_seq(0), m_mijingxunbao_once_gold(0), m_mijingxunbao_tentimes_gold(0),
	m_mijingxunbao_fifthtimes_gold(0), m_mijingxunbao_interval(0), m_add_weight_tao_times(0), m_mijingxunbao_fifthtimes_item_id(0), m_mijingxunbao_reward_cfg_count(0),
	m_rock_money_tree_need_gold(0), m_money_tree_init_pool_gold(0), m_money_tree_into_pool(0), m_money_tree_free_times(0), m_money_tree_free_interval(0), m_money_tree_consume_item(0), m_money_tree_max_stage(0),
	m_kingdraw_level_chuji_consume(0), m_kingdraw_level_zhongji_consume(0), m_kingdraw_level_gaoji_consume(0), m_king_draw_gaoji_consume_item(0), m_any_monster_drop_limit_drop_time(0),
	m_any_monster_drop_id(0), m_any_monster_drop_day_limit_rate(0), m_any_monster_drop_cfg_list_count(0), m_mine_refresh_gold(0), m_mine_refresh_count(0), m_mine_free_times(0), m_mine_server_reward_level_limit(0), m_mine_info_cfg_count(0), 
	m_mine_server_reward_cfg_count(0), m_guagua_free_time_interval(0), m_guagua_roll_item_id(0), m_tianming_free_chou_times(0),
	m_tianming_reward_total_weight(0), m_tianming_reward_max_seq(0), m_tianming_reward_add_lot_seq_count(0), m_tianming_gold_consume_max_add_lot_times(0), m_fanfan_free_fan_times_per_day(0),
	m_fanfan_once_need_gold(0), m_fanfan_refresh_need_gold(0), m_fanfan_auto_refresh_interval(0), m_fanfan_word_increase_rate(0), m_fanfan_item_info_cfg_count(0), m_fanfan_word_info_cfg_count(0), m_fanfan_baodi_reward_cfg_count(0),
	m_continue_chongzhi_fetch_extra_reward_need_days(0), m_continue_chongzhi_max_day_index(0), m_continue_chongzhi_fetch_extra_reward_need_days_chu(0), m_continue_chongzhi_fetch_extra_reward_need_days_gao(0),
	m_continue_consume_other_day_cfg_count(0), m_continue_consume_max_reward_day_cfg_count(0),
	m_circulation_chongzhi_max_reward_count(0), m_extreme_lucky_flush_consume_glod(0), m_extreme_lucky_free_draw_times(0), m_extreme_lucky_return_reward_cfg_count(0), m_mijingxunbao2_once_gold(0), m_mijingxunbao2_tentimes_gold(0), m_mijingxunbao2_fifthtimes_gold(0),
	m_mijingxunbao2_interval(0), m_add_weight_tao2_times(0), m_mijingxunbao2_fifthtimes_item_id(0),
	m_rock_money_tree2_need_gold(0), m_money_tree2_init_pool_gold(0), m_money_tree2_into_pool(0), m_money_tree2_max_stage(0), m_national_day_cfg_count(0), m_zhenbaoge2_reflush_gold(0),
	m_zhenbaoge2_auto_flush_times(0), m_zhenbaoge2_auto_add_cfg_count(0), m_zhenbaoge2_max_seq(0), m_zhenbaoge2_reward_max_seq(0), m_promoting_position_free_play_interval(0),
	m_promoting_position_10_times_use_item(0), m_promoting_position_extra_time_per_chongzhi_gold(0), m_promoting_position_play_once_gold(0), m_promoting_position_10_times_gold(0),
	m_promoting_position_outside_max_seq(0), m_promoting_position_inside_max_seq(0), m_promoting_position_reward_cfg_count(0), m_new_cornucopia_percent(0), m_xianyuan_treas_buy_limit_day(0), m_xianyuan_treas_cfg_count(0),
	m_rush_buying_duration(0), m_map_hunt_free_xunbao_count(0), m_map_hunt_free_flush_interval(0), m_map_hunt_xunbao_glod(0), m_map_hunt_flush_gold(0), m_light_tower_explore_one_gold(0), m_light_tower_explore_ten_gold(0),
	m_planting_tree_tree_living_time(0), m_planting_tree_max_watering_times(0), m_planting_tree_gather_id(0), m_planting_tree_watering_time(0), m_planting_tree_scene_id(0), m_planting_tree_norexitem_id(0),
	m_be_stealed_fish_time_limit(0), m_crystal_dalaran_initial_count(0), m_steal_crystal_dalaran_time_limit(0), m_crystal_dalaran_be_stealed_time_limit(0), m_steal_crystal_dalaran_one_people_time_limit(0), m_steal_crystal_dalaran_percent(0),
	m_broadcast_percent(0), m_steal_crystal_dalaran_limit(0), m_roll_reward_gold(0), m_roll_times_limit(0), m_collect_treasure_gold(0), m_collect_treasure_total_rate(0), m_collect_treasure_join_times(0), m_consume_for_gift_points_per_gold(0),
	m_mijingxunbao3_once_gold(0), m_mijingxunbao3_tentimes_gold(0), m_mijingxunbao3_thirtytimes_gold(0), m_mijingxunbao3_interval(0), m_add_weight_tao3_times(0), m_mijingxunbao3_thirtytimes_item_id(0), m_mijingxunbao3_free_times(0), m_mijingxunbao3_reward_cfg_count(0), m_mijingxunbao3_daily_cfg_max_count(0),
	m_huanlezadan_once_gold(0), m_huanlezadan_tentimes_gold(0), m_huanlezadan_thirtytimes_gold(0), m_huanlezadan_interval(0), m_huanlezadan_add_weight_tao_times(0), m_huanlezadan_thirtytimes_item_id(0), m_huanlezadan_free_times(0), m_huanlezadan_reward_cfg_count(0), m_huanlezadan_daily_cfg_max_count(0),
	m_huanleyaojiang_once_gold(0), m_huanleyaojiang_tentimes_gold(0), m_huanleyaojiang_thirtytimes_gold(0), m_huanleyaojiang_interval(0), m_huanleyaojiang_add_weight_tao_times(0), m_huanleyaojaing_thirtytimes_item_id(0), m_huanleyaojiang_free_times(0), m_huanleyaojiang_reward_cfg_count(0), m_huanleyaojiang_stage_cfg_max_count(0),
	m_circulation_chongzhi_2_need_chongzhi(0), m_consume_reward_stage_cfg_count(0), m_huanleyaojiang_2_once_gold(0), m_huanleyaojiang_2_tentimes_gold(0), m_huanleyaojiang_2_thirtytimes_gold(0), m_huanleyaojiang_2_interval(0),
	m_huanleyaojiang_2_add_weight_tao_times(0), m_huanleyaojaing_2_thirtytimes_item_id(0), m_huanleyaojiang_2_free_times(0), m_huanleyaojiang_2_reward_cfg_count(0), m_huanleyaojiang_2_stage_cfg_max_count(0), m_everyday_nice_gift_cfg_count(0), m_holiday_guard_kill_rank_num(0),
	m_extreme_challenge_rand_give_role_task_num(0), m_extreme_challenge_refresh_task_need_gold(0), m_extreme_challenge_reward_count(0), m_combine_buy_discount_cfg_list_count(0), m_combine_buy_item_cfg_list_count(0), m_image_competition_max_opengame_day(0), m_lucky_wish_30_times_use_item(0)
{
	memset(m_person_rank_bipin_activity_map, 0, sizeof(m_person_rank_bipin_activity_map));
	memset(m_level_lottery_consume_gold_level_list, 0, sizeof(m_level_lottery_consume_gold_level_list));
	//memset(m_kingdraw_level_cfg_list, 0, sizeof(m_kingdraw_level_cfg_list));
	memset(m_total_charge_3_cfglist, 0, sizeof(m_total_charge_3_cfglist));
	memset(m_any_monster_drop_cfg_list, 0, sizeof(m_any_monster_drop_cfg_list));
	memset(m_mijingxunbao_max_seq_list, 0, sizeof(m_mijingxunbao_max_seq_list));
	memset(m_mijingxunbao_auto_add_cfg_count_list, 0, sizeof(m_mijingxunbao_auto_add_cfg_count_list));
	memset(m_money_tree_total_weight_list, 0, sizeof(m_money_tree_total_weight_list));
	memset(m_money_tree_item_reward_total_weight_list, 0, sizeof(m_money_tree_item_reward_total_weight_list));
	memset(m_money_tree_reward_cfg_count_list, 0, sizeof(m_money_tree_reward_cfg_count_list));
	memset(m_money_tree_server_reward_cfg_count_list, 0, sizeof(m_money_tree_server_reward_cfg_count_list));
	memset(m_kingdraw_return_reward_cfg_count, 0, sizeof(m_kingdraw_return_reward_cfg_count));
	memset(m_guagua_need_gold_list, 0, sizeof(m_guagua_need_gold_list));
	memset(m_tianming_reward_cfg_list, 0, sizeof(m_tianming_reward_cfg_list));
	memset(m_tianming_reward_add_lot_seq_list, 0, sizeof(m_tianming_reward_add_lot_seq_list));
	memset(m_tianming_gold_consume_cfg_list, 0, sizeof(m_tianming_gold_consume_cfg_list));
	memset(m_mijingxunbao2_auto_add_cfg_count_list, 0, sizeof(m_mijingxunbao2_auto_add_cfg_count_list));
	memset(m_mijingxunbao2_max_seq_list, 0, sizeof(m_mijingxunbao2_max_seq_list));
	memset(m_extreme_lucky_reward_cfg_count_list, 0, sizeof(m_extreme_lucky_reward_cfg_count_list));
	memset(m_extreme_lucky_total_weight_list, 0, sizeof(m_extreme_lucky_total_weight_list));
	memset(m_money_tree2_total_weight_list, 0, sizeof(m_money_tree2_total_weight_list));
	memset(m_money_tree2_item_reward_total_weight_list, 0, sizeof(m_money_tree2_item_reward_total_weight_list));
	memset(m_money_tree2_reward_cfg_count_list, 0, sizeof(m_money_tree2_reward_cfg_count_list));
	memset(m_money_tree2_server_reward_cfg_count_list, 0, sizeof(m_money_tree2_server_reward_cfg_count_list));
	memset(m_map_hunt_total_city_weight_list, 0, sizeof(m_map_hunt_total_city_weight_list));
	memset(m_mijingxunbao3_auto_add_cfg_count_list, 0, sizeof(m_mijingxunbao3_auto_add_cfg_count_list));
	memset(m_mijingxunbao3_max_seq_list, 0, sizeof(m_mijingxunbao3_max_seq_list));
	memset(m_huanlezadan_auto_add_cfg_count_list, 0, sizeof(m_huanlezadan_auto_add_cfg_count_list));
	memset(m_huanlezadan_max_seq_list, 0, sizeof(m_huanlezadan_max_seq_list));
	memset(m_huanleyaojiang_auto_add_cfg_count_list, 0, sizeof(m_huanleyaojiang_auto_add_cfg_count_list));
	memset(m_huanleyaojiang_max_seq_list, 0, sizeof(m_huanleyaojiang_max_seq_list));
	memset(m_huanleyaojiang_2_auto_add_cfg_count_list, 0, sizeof(m_huanleyaojiang_2_auto_add_cfg_count_list));
	memset(m_huanleyaojiang_2_max_seq_list, 0, sizeof(m_huanleyaojiang_2_max_seq_list));
	memset(m_total_charge_5_cfg, 0, sizeof(m_total_charge_5_cfg));
	memset(m_extreme_challenge_finish_all_task_reward_cfg, 0, sizeof(m_extreme_challenge_finish_all_task_reward_cfg));
	
	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP1] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL;
	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP1] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP;
	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP1] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV;
	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP1] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI;
	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP1] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT;
	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP1] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING;

	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP2] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL;
	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP2] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP;
	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP2] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV;
	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP2] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI;
	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP2] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT;
	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP2] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING;

	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP3] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL;
	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP3] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP;
	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP3] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV;
	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP3] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI;
	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP3] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT;
	m_person_rank_bipin_activity_map[PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP3] = RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING;
}

RandActivityConfig::~RandActivityConfig()
{

}

bool RandActivityConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "RandActivityConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// 其他配置
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + ": xml no other node";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 幸运转盘配置
		PugiXmlNode root_element = RootElement.child("lucky_roll");
		if (root_element.empty())
		{
			*err = path + ": xml no lucky_roll node";
			return false;
		}

		iRet = this->InitLuckyRollCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLuckRollCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 全民疯抢配置
		PugiXmlNode root_element = RootElement.child("server_panic_buy");
		if (root_element.empty())
		{
			*err = path + ": xml no server_panic_buy node";
			return false;
		}

		iRet = this->InitServerPanicBuyCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitServerPanicBuyCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 个人疯抢配置
		PugiXmlNode root_element = RootElement.child("personal_panic_buy");
		if (root_element.empty())
		{
			*err = path + ": xml no personal_panic_buy node";
			return false;
		}

		iRet = this->InitPersonalPanicBuyCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPersonalPanicBuyCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 充值排行榜奖励配置
		PugiXmlNode root_element = RootElement.child("chongzhi_rank");
		if (root_element.empty())
		{
			*err = path + ": xml no chongzhi_rank node";
			return false;
		}

		iRet = this->InitChongzhiRankRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitChongzhiRankRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 充值排行榜2奖励配置
		PugiXmlNode root_element = RootElement.child("extreme_recharge");
		if (root_element.empty())
		{
			*err = path + ": xml no extreme_recharge node";
			return false;
		}

		iRet = this->InitChongzhiRank2RewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitChongzhiRank2RewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 消费排行榜奖励配置
		PugiXmlNode root_element = RootElement.child("consume_gold_rank");
		if (root_element.empty())
		{
			*err = path + ": xml no consume_gold_rank node";
			return false;
		}

		iRet = this->InitConsumeGoldRankRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitConsumeGoldRankRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 装备强化奖励配置
		PugiXmlNode root_element = RootElement.child("equipstrengthen");
		if (root_element.empty())
		{
			*err = path + ": xml no equipstrengthen node";
			return false;
		}

		iRet = this->InitEquipStrengthenRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitStrengthenRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 奇珍异宝奖励配置
		PugiXmlNode root_element = RootElement.child("chestshop");
		if (root_element.empty())
		{
			*err = path + ": xml no chestshop node";
			return false;
		}

		iRet = this->InitChestshopRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitChestshopRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 全身宝石总等级奖励配置
		PugiXmlNode root_element = RootElement.child("stone_total_level_uplevel");
		if (root_element.empty())
		{
			*err = path + ": xml no stone_total_level_uplevel node";
			return false;
		}

		iRet = this->InitStoneUplevelRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitStoneUplevelRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 仙女缠绵等级奖励配置
		PugiXmlNode root_element = RootElement.child("xiannv_chanmian_uplevel");
		if (root_element.empty())
		{
			*err = path + ": xml no xiannv_chanmian_uplevel node";
			return false;
		}

		iRet = this->InitXiannvChanmianUplevelRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitXiannvChanmianUplevelRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 坐骑进阶奖励配置
		PugiXmlNode root_element = RootElement.child("mount_upgrade");
		if (root_element.empty())
		{
			*err = path + ": xml no mount_upgrade node";
			return false;
		}

		iRet = this->InitMountUpgradeRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMountUpgradeRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 骑兵进阶奖励配置
		PugiXmlNode root_element = RootElement.child("qibing_upgrade");
		if (root_element.empty())
		{
			*err = path + ": xml no qibing_upgrade node";
			return false;
		}

		iRet = this->InitQibingUpgradeRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitQibingUpgradeRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}


	{
		// 根骨全身等级奖励配置
		PugiXmlNode root_element = RootElement.child("mentality_total_level");
		if (root_element.empty())
		{
			*err = path + ": xml no mentality_total_level node";
			return false;
		}

		iRet = this->InitMentalityTotalLevelRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMentalityTotalLevelRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 羽翼进化等级奖励配置
		PugiXmlNode root_element = RootElement.child("wing_upgrade");
		if (root_element.empty())
		{
			*err = path + ": xml no wing_upgrade node";
			return false;
		}

		iRet = this->InitWingUpgradeRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitWingUpgradeRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 全民祈福奖励配置
		PugiXmlNode root_element = RootElement.child("quanmin_qifu");
		if (root_element.empty())
		{
			*err = path + ": xml no quanmin_qifu node";
			return false;
		}

		iRet = this->InitQuanminQifuRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitVipQifuRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 仙盟比拼奖励配置
		PugiXmlNode root_element = RootElement.child("xianmeng_bipin");
		if (root_element.empty())
		{
			*err = path + ": xml no xianmeng_bipin node";
			return false;
		}

		iRet = this->InitXianmengBipinRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitXianmengBipinRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 仙盟崛起奖励配置
		PugiXmlNode root_element = RootElement.child("xianmeng_jueqi");
		if (root_element.empty())
		{
			*err = path + ": xml no xianmeng_jueqi node";
			return false;
		}

		iRet = this->InitXianmengJueQiRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitXianmengJueQiRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 登录送礼奖励配置
		PugiXmlNode root_element = RootElement.child("login_gift");
		if (root_element.empty())
		{
			*err = path + ": xml no login_gift node";
			return false;
		}

		iRet = this->InitLoginGiftRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLoginGiftRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 登录送礼奖励配置
		PugiXmlNode root_element = RootElement.child("login_gift_0");
		if (root_element.empty())
		{
			*err = path + ": xml no login_gift_0 node";
			return false;
		}

		iRet = this->InitLoginGiftRewardCfg_0(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitLoginGiftRewardCfg_0 failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 手有余香奖励配置
		PugiXmlNode root_element = RootElement.child("shouyou_yuxiang");
		if (root_element.empty())
		{
			*err = path + ": xml no shouyou_yuxiang node";
			return false;
		}

		iRet = this->InitShouYouYuXiangRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitShouYouYuXiangRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 比拼排名配置
		PugiXmlNode root_element = RootElement.child("bipin_rank");
		if (root_element.empty())
		{
			*err = path + ": xml no bipin_rank node";
			return false;
		}

		iRet = this->InitRankBipinRewordCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitRankBipinRewordCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 比拼排全民配置
		PugiXmlNode root_element = RootElement.child("bipin_all");
		if (root_element.empty())
		{
			*err = path + ": xml no bipin_all node";
			return false;
		}

		iRet = this->InitAllBipinRewordCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitAllBipinRewordCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
		{
			if (m_luckyroll_cfg[i].cfg.replace_gold_reward_seq >= RAND_ACTIVITY_LUCKROOL_REWARD_CONFIG_MAX_COUNT)
			{
				*err = path + ": lucky_roll_replace_gold_reward_seq is overflow";
				return false;
			}

			if (LUCKY_ROLL_REWARD_TYPE_ITEM != m_luckyroll_cfg[i].cfg.reward_cfg_list[m_luckyroll_cfg[i].cfg.replace_gold_reward_seq].reward_type)
			{
				*err = path + ": lucky_roll_replace_gold_reward_seq is not itemreward seq";
				return false;
			}
		}

	}

	{
		// 充值返利配置
		PugiXmlNode root_element = RootElement.child("day_chongzhi_fanli");
		if (root_element.empty())
		{
			*err = path + ": xml no day_chongzhi_fanli node";
			return false;
		}

		iRet = this->InitDayChongZhiFanLiCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDayChongZhiFanLiCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//每日消费活动配置
		PugiXmlNode root_element = RootElement.child("day_gold_consume");
		if (root_element.empty())
		{
			*err = path + ": xml no day_gold_consume node";
			return false;
		}

		iRet = this->InitDayConsumeGoldCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDayConsumeGoldCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//累计消费活动配置
		PugiXmlNode root_element = RootElement.child("total_gold_consume");
		if (root_element.empty())
		{
			*err = path + ": xml no total_gold_consume node";
			return false;
		}

		iRet = this->InitTotalConsumeGoldRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTotalConsumeGoldRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//活跃奖励活动配置
		PugiXmlNode root_element = RootElement.child("day_active_degree");
		if (root_element.empty())
		{
			*err = path + ": xml no day_active_degree node";
			return false;
		}

		iRet = this->InitDayActiveDegreeRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDayActiveDegreeRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//在线奖励活动配置
		PugiXmlNode root_element = RootElement.child("day_online_gift");
		if (root_element.empty())
		{
			*err = path + ": xml no day_online_gift node";
			return false;
		}

		iRet = this->InitDayOnlineGiftCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitDayOnlineGiftCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//充值回馈活动配置
		PugiXmlNode root_element = RootElement.child("charge_repayment");
		if (root_element.empty())
		{
			*err = path + ": xml no charge_repayment node";
			return false;
		}
		iRet = this->InitChargeRepaymentCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitChargeRepaymentCfg faild %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//单笔充值活动配置
		PugiXmlNode root_element = RootElement.child("single_charge");
		if (root_element.empty())
		{
			*err = path + ": xml no single_charge node";
			return false;
		}
		iRet = this->InitSingleChargeCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitSingleChargeCfg faild %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 单笔充值1配置
		PugiXmlNode root_element = RootElement.child("single_charge1");
		if (root_element.empty())
		{
			*err = path + " xml not single_charge1 node ";
			return false;
		}

		iRet = this->InitSingleCharge1Cfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitSingleCharge1Cfg fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
	{
		// 单笔充值2配置
		PugiXmlNode root_element = RootElement.child("single_charge2");
		if (root_element.empty())
		{
			*err = path + " xml not single_charge2 node ";
			return false;
		}

		iRet = this->InitSingleCharge2Cfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitSingleCharge2Cfg fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
	{
		// 单笔充值3配置
		PugiXmlNode root_element = RootElement.child("single_charge3");
		if (root_element.empty())
		{
			*err = path + " xml not single_charge3 node ";
			return false;
		}

		iRet = this->InitSingleCharge3Cfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitSingleCharge3Cfg fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//聚宝盆轮数总配置
		PugiXmlNode root_element = RootElement.child("cornucopia");
		if (root_element.empty())
		{
			*err = path + ": xml no cornucopia node";
			return false;
		}
		iRet = this->InitCornucopiaCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitCornucopiaCfg faild %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 单笔充值配置
		PugiXmlNode root_element = RootElement.child("danbichongzhi");
		if (root_element.empty())
		{
			*err = path + ": xml no danbichongzhi node";
			return false;
		}
		iRet = this->InitDanbiChongzhiCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitDanbiChongzhiCfg faild %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
	
	{
		// 累计充值（天数）配置
		PugiXmlNode root_element = RootElement.child("total_chongzhi_day");
		if (root_element.empty())
		{
			*err = path + ": xml no total_chongzhi_day node";
			return false;
		}
		iRet = this->InitTotalChargeDayCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitTotalChargeDayCfg faild %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 次日福利配置
		PugiXmlNode root_element = RootElement.child("tomorrow_reward");
		if (root_element.empty())
		{
			*err = path + ": xml no tomorrow_reward node";
			return false;
		}
		iRet = this->InitTomorrowRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitTomorrowRewardCfg faild %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 每日充值排行配置
		PugiXmlNode root_element = RootElement.child("day_chongzhi_rank");
		if (root_element.empty())
		{
			*err = path + ": xml no day_chongzhi_rank node";
			return false;
		}
		iRet = this->InitDayChongzhiRankCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitDayChongzhiRankCfg faild %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 每日消费排行配置
		PugiXmlNode root_element = RootElement.child("day_consume_rank");
		if (root_element.empty())
		{
			*err = path + ": xml no day_consume_rank node";
			return false;
		}
		iRet = this->InitDayConsumeRankCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitDayConsumeRankCfg faild %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 累计充值
		PugiXmlNode root_element = RootElement.child("rand_total_chongzhi");
		if (root_element.empty())
		{
			*err = path + ": xml no total_chongzhi node";
			return false;
		}

		iRet = this->InitTotalChongzhiCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTotalChongzhiCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//限时积分兑换稀有装备
		PugiXmlNode root_element = RootElement.child("time_limit_exchange_equi");
		if (root_element.empty())
		{
			*err = path + ":xml no time_limit_exchange_equi node";
			return false;
		}

		iRet = this->InitTimeLimitExchangeEquiCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTimeLimitExchangeEquiCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//限时积分兑换精灵
		PugiXmlNode root_element = RootElement.child("time_limit_exchange_jl");

		if (root_element.empty())
		{
			*err = path + ":xml no time_limit_exchange_jl node";
			return false;
		}

		iRet = this->InitTimeLimitExchangeJLCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTimeLimitExchangeJLCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//金银塔其他配置	
		PugiXmlNode root_element = RootElement.child("level_lottery_other");
		if (root_element.empty())
		{
			*err = path + ": xml no level_lottery_other node";
			return false;
		}

		iRet = this->InitLevelLotteryOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitLevelLotteryOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}

		//金银塔抽奖活动配置	
		root_element = RootElement.child("level_lottery_consume");
		if (root_element.empty())
		{
			*err = path + ": xml no level_lottery_consume node";
			return false;
		}

		iRet = this->InitLevelLotteryConsumeCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLevelLotteryConsumeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}

		root_element = RootElement.child("level_lottery_reward");
		if (root_element.empty())
		{
			*err = path + ": xml no level_lottery_reward node";
			return false;
		}

		iRet = this->InitLevelLotteryRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLevelLotteryRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}

		//金银塔累计次数奖励配置	
		root_element = RootElement.child("level_lottery_total_reward");
		if (root_element.empty())
		{
			*err = path + ": xml no level_lottery_total_reward node";
			return false;
		}

		iRet = this->InitLevelLotteryTotalTimesCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitLevelLotteryTotalTimesCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//充值扭蛋
		PugiXmlNode root_element = RootElement.child("charge_niu_egg");
		if (root_element.empty())
		{
			*err = path + ": xml no charge_niu_egg node";
			return false;
		}

		iRet = this->InitNiuEggCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitNiuEggCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 珍宝阁
		PugiXmlNode root_element = RootElement.child("zhenbaoge");
		if (root_element.empty())
		{
			*err = path + ": xml no zhenbaoge_cfg node";
			return false;
		}

		iRet = this->InitZhenbaogeCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitZhenbaogeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 充值返元宝
		PugiXmlNode root_element = RootElement.child("charge_reward_gold");
		if (root_element.empty())
		{
			*err = path + ": xml no charge_reward_gold node";
			return false;
		}

		iRet = this->InitChargeRewardGold(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitChargeRewardGold failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 秘境寻宝
		PugiXmlNode root_element = RootElement.child("mijingxunbao");
		if (root_element.empty())
		{
			*err = path + ": xml no mijingxunbao node";
			return false;
		}

		iRet = this->InitMiJingXunBaoCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMiJingXunBaoCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}

		//秘境寻宝累抽奖励配置
		root_element = RootElement.child("mijingxunbao_reward");
		if (root_element.empty())
		{
			*err = path + ": xml no mijingxunbao_reward node";
			return false;
		}

		iRet = this->InitMiJingXunBaoRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitMiJingXunBaoRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 秘境寻宝2
		PugiXmlNode root_element = RootElement.child("mijingxunbao2");
		if (root_element.empty())
		{
			*err = path + ": xml no mijingxunbao2 node";
			return false;
		}

		iRet = this->InitMiJingXunBao2Cfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMiJingXunBao2Cfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 急速冲战
		PugiXmlNode root_element = RootElement.child("jisuchongzhan");
		if (root_element.empty())
		{
			*err = path + ": xml no jisuchongzhan node";
			return false;
		}

		iRet = this->InitJiSuChongZhanCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitJiSuChongZhanCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 摇钱树配置
		PugiXmlNode root_element = RootElement.child("yaoqianshu");
		if (root_element.empty())
		{
			*err = path + ": xml no yaoqianshu node";
			return false;
		}

		iRet = this->InitMoneyTreeCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMoneyTreeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}

		PugiXmlNode baodi_element = RootElement.child("yaoqianshu");
		if (baodi_element.empty())
		{
			*err = path + ":xml no yaoqianshu_server_reward";
			return false;
		}

		iRet = this->InitMoneyTreeBaodiCfg(baodi_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitMoneyTreeBaodiCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 摇钱树2配置
		PugiXmlNode root_element = RootElement.child("yaoqianshu2");
		if (root_element.empty())
		{
			*err = path + ": xml no yaoqianshu2 node";
			return false;
		}

		iRet = this->InitMoneyTree2Cfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMoneyTree2Cfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 陛下请翻牌活动，初始化配置
		PugiXmlNode root_element = RootElement.child("king_draw");
		if (root_element.empty())
		{
			*err = path + ": xml no king_draw node";
			return false;
		}

		iRet = this->InitKingDrawLevelCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitKingDrawLevelCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 陛下请翻牌活动，初始化配置
		PugiXmlNode root_element = RootElement.child("king_draw_return_reward");
		if (root_element.empty())
		{
			*err = path + ": xml no king_draw_return_reward node";
			return false;
		}

		iRet = this->InitKingDrawReturnRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitKingDrawReturnRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 累计充值3
		PugiXmlNode root_element = RootElement.child("total_charge_3");
		if (root_element.empty())
		{
			*err = path + ": xml no total_charge_3 node";
			return false;
		}

		iRet = this->InitTotalChongzhi3Cfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTotalChongzhi3Cfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 被动特殊形象变身排行
		PugiXmlNode root_element = RootElement.child("special_appearance_passive_rank");
		if (root_element.empty())
		{
			*err = path + ": xml no special_appearance_passive_rank node";
			return false;
		}

		iRet = this->InitAppearacePassiveRankCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitAppearacePassiveRankCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 特殊形象变身卡排行
		PugiXmlNode root_element = RootElement.child("special_appearance_rank");
		if (root_element.empty())
		{
			*err = path + ": xml no special_appearance_rank node";
			return false;
		}

		iRet = this->InitAppearaceRankCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitAppearaceRankCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 全场景打怪掉落活动配置
		PugiXmlNode root_element = RootElement.child("any_monster_drop");
		if (root_element.empty())
		{
			*err = path + ": xml no any_monster_drop node";
			return false;
		}

		iRet = this->InitAnyMonsterDropCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitAnyMonsterDropCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 全场景累计充值4(新三件套)活动配置
		PugiXmlNode root_element = RootElement.child("total_charge4");
		if (root_element.empty())
		{
			*err = path + ": xml no total_charge4 node";
			return false;
		}

		iRet = this->InitTotalChongzhi4Cfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTotalChongzhi4Cfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 挖宝活动，初始化矿石信息配置
		PugiXmlNode root_element = RootElement.child("mine_info");
		if (root_element.empty())
		{
			*err = path + ": xml no mine_info node";
			return false;
		}

		iRet = this->InitMineInfoCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMineInfoCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}

		// 挖宝活动，初始化全服礼包配置
		root_element = RootElement.child("mine_reward");
		if (root_element.empty())
		{
			*err = path + ": xml no mine_reward node";
			return false;
		}

		iRet = this->InitMineServerRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMineServerRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 顶刮刮活动，初始化配置
		PugiXmlNode root_element = RootElement.child("guagua");
		if (root_element.empty())
		{
			*err = path + ": xml no guagua node";
			return false;
		}

		iRet = this->InitGuaGuaCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitGuaGuaCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}

		root_element = RootElement.child("guagua_acc_reward");
		if (root_element.empty())
		{
			*err = path + ": xml no guagua_acc_reward node";
			return false;
		}

		iRet = this->InitGuaGuaAccRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitGuaGuaAccRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}

		PugiXmlNode baodi_element = RootElement.child("guagua");
		if (baodi_element.empty())
		{
			*err = path + "xml no guagua_server_reward";
			return false;
		}

		iRet = this->InitGuaGuaBaodiRewardCfg(baodi_element);
		if (iRet)
		{
			sprintf(errinfo, "%s:InitGuaGuaBaodiRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 天命卜卦活动-初始化奖池配置
		PugiXmlNode root_element = RootElement.child("tianming_jiangchi");
		if (root_element.empty())
		{
			*err = path + ": xml no tianming_jiangchi node";
			return false;
		}

		iRet = this->InitTianMingDivinationRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTianMingDivinationRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 天命卜卦活动-初始化消耗配置
		PugiXmlNode root_element = RootElement.child("tianming_consume");
		if (root_element.empty())
		{
			*err = path + ": xml no tianming_consume node";
			return false;
		}

		iRet = this->InitTianMingDivinationGoldConsumeCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTianMingDivinationGoldConsumeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 翻翻转活动-初始化物品配置
		PugiXmlNode root_element = RootElement.child("fanfan_item_info");
		if (root_element.empty())
		{
			*err = path + ": xml no fanfan_item_info node";
			return false;
		}

		iRet = this->InitFanFanItemInfoCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitFanFanItemInfoCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}

		// 翻翻转活动-初始化字组信息
		root_element = RootElement.child("fanfan_word_info");
		if (root_element.empty())
		{
			*err = path + ": xml no fanfan_word_info node";
			return false;
		}

		iRet = this->InitFanFanWordInfoCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitFanFanWordInfoCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}


		//翻翻转累抽奖励
		root_element = RootElement.child("fanfan_reward");
		if (root_element.empty())
		{
			*err = path + ": xml no fanfan_reward node";
			return false;
		}

		iRet = this->InitFanFanBaodiCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitFanFanBaodiCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 连续充值-奖励信息配置
		PugiXmlNode root_element = RootElement.child("continue_chonghzi");
		if (root_element.empty())
		{
			*err = path + ": xml no continue_chonghzi node";
			return false;
		}

		iRet = this->InitContinueChongzhiRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitContinueChongzhiRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 连充特惠初-奖励信息配置
		PugiXmlNode root_element = RootElement.child("continue_chonghzi_chu");
		if (root_element.empty())
		{
			*err = path + ": xml no continue_chonghzi_chu node";
			return false;
		}

		iRet = this->InitContinueChongzhiRewardChuCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitContinueChongzhiRewardChuCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 连充特惠高-奖励信息配置
		PugiXmlNode root_element = RootElement.child("continue_chonghzi_gao");
		if (root_element.empty())
		{
			*err = path + ": xml no continue_chonghzi_gao node";
			return false;
		}

		iRet = this->InitContinueChongzhiRewardGaoCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitContinueChongzhiRewardGaoCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 限时连充-奖励信息配置
		PugiXmlNode root_element = RootElement.child("continue_chonghzi_2");
		if (root_element.empty())
		{
			*err = path + ": xml no continue_chonghzi_2 node";
			return false;
		}

		iRet = this->InitContinueChongzhiReward2Cfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitContinueChongzhiReward2Cfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{

		// 连续消费-奖励信息配置
		PugiXmlNode root_element = RootElement.child("continue_consume");
		if (root_element.empty())
		{
			*err = path + ": xml no continue_consume node";
			return false;
		}

		iRet = this->InitContinueConsumeRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitContinueConsumeRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 军歌嘹亮 -- 活跃度兑换旗帜
		PugiXmlNode root_element = RootElement.child("army_day_exchange_flag");
		if (root_element.empty())
		{
			*err = path + ": xml no army_day_exchange_flag node";
			return false;
		}

		iRet = this->InitArmyDayExchangeFlagCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitArmyDayExchangeFlagCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 军歌嘹亮 -- 旗帜兑换物品
		PugiXmlNode root_element = RootElement.child("army_day_exchange_item");
		if (root_element.empty())
		{
			*err = path + ": xml no army_day_exchange_item node";
			return false;
		}

		iRet = this->InitArmyDayExchangeItemCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitArmyDayExchangeItemCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 循环充值-奖励信息配置
		PugiXmlNode root_element = RootElement.child("circulation_chongzhi");
		if (root_element.empty())
		{
			*err = path + ": xml no circulation_chongzhi node";
			return false;
		}

		iRet = this->InitCirculationChongzhiRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitCirculationChongzhiRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 至尊幸运-消耗配置
		PugiXmlNode root_element = RootElement.child("extreme_lucky_draw_consume");
		if (root_element.empty())
		{
			*err = path + ": xml no extreme_lucky_draw_consume node";
			return false;
		}

		iRet = this->InitExtremeLuckyDrawConsumeCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitExtremeLuckyDrawConsumeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 至尊幸运-物品配置
		PugiXmlNode root_element = RootElement.child("extreme_lucky_draw");
		if (root_element.empty())
		{
			*err = path + ": xml no extreme_lucky_draw node";
			return false;
		}

		iRet = this->InitExtremeLuckyDrawRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitExtremeLuckyDrawRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 至尊幸运-物品配置
		PugiXmlNode root_element = RootElement.child("extreme_lucky_return_reward");
		if (root_element.empty())
		{
			*err = path + ": xml no extreme_lucky_return_reward node";
			return false;
		}

		iRet = this->InitExtremeLuckyDrawReturnRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitExtremeLuckyDrawReturnRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 欢度国庆-兑换配置
		PugiXmlNode root_element = RootElement.child("national_day");
		if (root_element.empty())
		{
			*err = path + ": xml no national_day node";
			return false;
		}

		iRet = this->InitNationalDayCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitNationalDayCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 珍宝阁2
		PugiXmlNode root_element = RootElement.child("zhenbaoge2");
		if (root_element.empty())
		{
			*err = path + ": xml no zhenbaoge2_cfg node";
			return false;
		}

		iRet = this->InitZhenbaoge2Cfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitZhenbaoge2Cfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 步步高升-外圈配置
		PugiXmlNode root_element = RootElement.child("promoting_position_outside_reward");
		if (root_element.empty())
		{
			*err = path + ": xml no promoting_position_outside_reward node";
			return false;
		}

		iRet = this->InitPromotingPositionOutsideCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPromotingPositionOutsideCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 步步高升-内圈配置
		PugiXmlNode root_element = RootElement.child("promoting_position_inside_reward");
		if (root_element.empty())
		{
			*err = path + ": xml no promoting_position_inside_reward node";
			return false;
		}

		iRet = this->InitPromotingPositionInsideCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPromotingPositionInsideCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 步步高升-抽奖返利
		PugiXmlNode root_element = RootElement.child("promoting_position_return_reward");
		if (root_element.empty())
		{
			*err = path + ": xml no promoting_position_return_reward node";
			return false;
		}

		iRet = this->InitPromotingReturnRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitPromotingReturnRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 黑市拍卖配置
		PugiXmlNode root_element = RootElement.child("black_market");
		if (root_element.empty())
		{
			*err = path + ": xml no black_market node";
			return false;
		}

		iRet = this->InitBlackMarketCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBlackMarketCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 珍宝商城配置
		PugiXmlNode root_element = RootElement.child("treasure_mall");
		if (root_element.empty())
		{
			*err = path + ": xml no treasure_mall node";
			return false;
		}

		iRet = this->InitTreasuresMallCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTreasuresMallCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 珍宝商城兑换配置
		PugiXmlNode root_element = RootElement.child("treasure_mall_exchange");
		if (root_element.empty())
		{
			*err = path + ": xml no treasure_mall_exchange node";
			return false;
		}

		iRet = this->InitTreasuresMallExchangeCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTreasuresMallExchangeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 开服活动配置
		PugiXmlNode root_element = RootElement.child("openserver_reward");
		if (root_element.empty())
		{
			*err = path + ": xml no openserver_reward node";
			return false;
		}

		iRet = this->InitOpenServerRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOpenServerRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 新聚宝盆
		PugiXmlNode root_element = RootElement.child("new_cornucopia");
		if (root_element.empty())
		{
			*err = path + ": xml no new_cornucopia node";
			return false;
		}

		iRet = this->InitNewCornucopiaCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitNewCornucopiaCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 新聚宝盆全服奖励
		PugiXmlNode root_element = RootElement.child("new_cornucopia_total");
		if (root_element.empty())
		{
			*err = path + ": xml no new_cornucopia_total node";
			return false;
		}

		iRet = this->InitNewCornucopiaTotalCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitNewCornucopiaTotalCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 集字活动-兑换奖励配置
		PugiXmlNode root_element = RootElement.child("item_collection");
		if (root_element.empty())
		{
			*err = path + ": xml no item_collection node";
			return false;
		}

		iRet = this->InitItemCollectionRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitItemCollectionRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 经验炼制
		PugiXmlNode root_element = RootElement.child("exp_refine");
		if (root_element.empty())
		{
			*err = path + ": xml no exp_refine node";
			return false;
		}

		iRet = this->InitExpRefineRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitExpRefineRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 红包好礼
		PugiXmlNode root_element = RootElement.child("red_envelope_gift");
		if (root_element.empty())
		{
			*err = path + ": xml no red_envelope_gift node";
			return false;
		}

		iRet = this->InitRedEnvelopeGift(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitRedEnvelopeGift failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 我们结婚吧
		PugiXmlNode root_element = RootElement.child("marry_me");
		if (root_element.empty())
		{
			*err = path + ": xml no marry_me node";
			return false;
		}

		iRet = this->InitMarryMeCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMarryMeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 金猪召唤
		PugiXmlNode root_element = RootElement.child("golden_pig_summon_basis");
		if (root_element.empty())
		{
			*err = path + ": xml no golden_pig_summon_basis node";
			return false;
		}

		iRet = this->InitGoldenPigOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitGoldenPigOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}

		root_element = RootElement.child("golden_pig_reward");
		if (root_element.empty())
		{
			*err = path + ": xml no golden_pig_reward node";
			return false;
		}

		iRet = this->InitGoldenPigRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitGoldenPigRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 开服投资
		PugiXmlNode root_element = RootElement.child("openserver_invest_basis");
		if (root_element.empty())
		{
			*err = path + ": xml no openserver_invest_basis node";
			return false;
		}

		iRet = this->InitOpenServerInvestOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitOpenServerInvestOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}

		root_element = RootElement.child("openserver_return_reward");
		if (root_element.empty())
		{
			*err = path + ": xml no openserver_return_reward node";
			return false;
		}

		iRet = this->InitOpenServerInvestRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitOpenServerInvestRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 单返豪礼
		PugiXmlNode root_element = RootElement.child("single_chongzhi");
		if (root_element.empty())
		{
			*err = path + ": xml no single_chongzhi node";
			return false;
		}

		iRet = this->InitSingleChongZhiRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitSingleChongZhiRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 聚划算
		PugiXmlNode root_element = RootElement.child("xianyuan_treas");
		if (root_element.empty())
		{
			*err = path + ": xml no xianyuan_treas node";
			return false;
		}

		iRet = this->InitXianyuanTreasCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitXianyuanTreasCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 限时秒杀-阶段配置
		PugiXmlNode root_element = RootElement.child("rush_buying");
		if (root_element.empty())
		{
			*err = path + ": xml no rush_buying node";
			return false;
		}

		iRet = this->InitRushBuyingPhaseCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitRushBuyingPhaseCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//单笔充值活动配置(2)
		PugiXmlNode root_element = RootElement.child("single_charge_2");
		if (root_element.empty())
		{
			*err = path + ": xml no single_charge_2 node";
			return false;
		}
		iRet = this->InitSingleChargeCfg2(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitSingleChargeCfg2 faild %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//单笔充值活动配置(3)、极速冲战2
		PugiXmlNode root_element = RootElement.child("single_charge_3");
		if (root_element.empty())
		{
			*err = path + ": xml no single_charge_2 node";
			return false;
		}
		iRet = this->InitSingleChargeCfg3(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitSingleChargeCfg2 faild %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//单笔充值活动配置(4)、极速冲战3
		PugiXmlNode root_element = RootElement.child("single_charge_4");
		if (root_element.empty())
		{
			*err = path + ": xml no single_charge_2 node";
			return false;
		}
		iRet = this->InitSingleChargeCfg4(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitSingleChargeCfg2 faild %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//单笔充值活动配置(5)、极速冲战4
		PugiXmlNode root_element = RootElement.child("single_charge_5");
		if (root_element.empty())
		{
			*err = path + ": xml no single_charge_2 node";
			return false;
		}
		iRet = this->InitSingleChargeCfg5(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitSingleChargeCfg5 faild %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 每日好礼
		PugiXmlNode root_element = RootElement.child("everyday_nice_gift");
		if (root_element.empty())
		{
			*err = path + ": xml no everyday_nice_gift node";
			return false;
		}

		iRet = this->InitEverydayNiceGiftCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitEverydayNiceGiftCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 地图寻宝
		{
			// 城市配置
			PugiXmlNode root_element = RootElement.child("map_hunt_city");
			if (root_element.empty())
			{
				*err = path + ": xml no map_hunt_city node";
				return false;
			}

			iRet = this->InitMapHuntCityCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitMapHuntCityCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 路线配置
			PugiXmlNode root_element = RootElement.child("map_hunt_route");
			if (root_element.empty())
			{
				*err = path + ": xml no map_hunt_route node";
				return false;
			}

			iRet = this->InitMapHuntRouteCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitMapHuntRouteCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 全服奖励配置
			PugiXmlNode root_element = RootElement.child("map_hunt_server_reward");
			if (root_element.empty())
			{
				*err = path + ": xml no map_hunt_server_reward node";
				return false;
			}

			iRet = this->InitMapHuntServerRewardCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitMapHuntServerRewardCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 灯塔寻宝-物品配置
			PugiXmlNode root_element = RootElement.child("light_tower_explore");
			if (root_element.empty())
			{
				*err = path + ": xml no light_tower_explore node";
				return false;
			}

			iRet = this->InitLightTowerExploreCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitLightTowerExploreCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 灯塔寻宝额外奖励配置
			PugiXmlNode root_element = RootElement.child("light_tower_explore_extern_reward");
			if (root_element.empty())
			{
				*err = path + ": xml no light_tower_explore_extern_reward node";
				return false;
			}

			iRet = this->InitLightTowerExploreExternCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitLightTowerExploreExternCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 灯塔寻宝返利配置
			PugiXmlNode root_element = RootElement.child("light_tower_explore_return_reward");
			if (root_element.empty())
			{
				*err = path + ": xml no light_tower_explore_return_reward node";
				return false;
			}

			iRet = this->InitLightTowerExploreServerCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitLightTowerExploreServerCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 新累计充值
			PugiXmlNode root_element = RootElement.child("new_rand_total_chongzhi");
			if (root_element.empty())
			{
				*err = path + ": xml no new_rand_total_chongzhi node";
				return false;
			}

			iRet = this->InitNewTotalChongzhiCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitNewTotalChongzhiCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 新累计充值
			PugiXmlNode root_element = RootElement.child("new_rand_total_chongzhi1");
			if (root_element.empty())
			{
				*err = path + ": xml no new_rand_total_chongzhi1 node";
				return false;
			}

			iRet = this->InitNewTotalChongzhi1Cfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitNewTotalChongzhi1Cfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 新累计充值
			PugiXmlNode root_element = RootElement.child("new_rand_total_chongzhi2");
			if (root_element.empty())
			{
				*err = path + ": xml no new_rand_total_chongzhi2 node";
				return false;
			}

			iRet = this->InitNewTotalChongzhi2Cfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitNewTotalChongzhi2Cfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 新累计充值
			PugiXmlNode root_element = RootElement.child("new_rand_total_chongzhi3");
			if (root_element.empty())
			{
				*err = path + ": xml no new_rand_total_chongzhi3 node";
				return false;
			}

			iRet = this->InitNewTotalChongzhi3Cfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitNewTotalChongzhi3Cfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 幻装商店
			PugiXmlNode root_element = RootElement.child("magic_shop");
			if (root_element.empty())
			{
				*err = path + ": xml no magic_shop node";
				return false;
			}

			iRet = this->InitMagicShopCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitMagicShopCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			//钓鱼基本信息
			PugiXmlNode root_element = RootElement.child("fish_info");
			if (root_element.empty())
			{
				*err = path + ": xml no fish_info node";
				return false;
			}

			iRet = this->InitFishInfoCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitFishInfoCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}

			//钓鱼排名奖励
			PugiXmlNode root_element2 = RootElement.child("fish_rank_reward");
			if (root_element2.empty())
			{
				*err = path + ": xml no fish_rank_reward node";
				return false;
			}

			iRet = this->InitFishRankRewardCfg(root_element2);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitFishRankRewardCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 趣味植树配置
			PugiXmlNode root_element = RootElement.child("planting_tree");
			if (root_element.empty())
			{
				*err = path + ": xml no planting_tree node";
				return false;
			}

			iRet = this->InitPlantingTreeRankRewardCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitPlantingTreeRankRewardCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 达拉然水晶
			PugiXmlNode root_element = RootElement.child("crtstal_dalaran_rank_reward");
			if (root_element.empty())
			{
				*err = path + ": xml no crtstal_dalaran_rank_reward node";
				return false;
			}

			iRet = this->InitCrtstalDalaranRewardCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitCrtstalDalaranRewardCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 狂返元宝
			PugiXmlNode root_element = RootElement.child("crazy_rebate");
			if (root_element.empty())
			{
				*err = path + ": xml no crazy_rebate node";
				return false;
			}

			iRet = this->InitChongzhiCrazyRabateCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitChongzhiCrazyRabateCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{ // 进阶主题活动
			PugiXmlNode root_element = RootElement.child("jinjie_act_theme");
			if (root_element.empty())
			{
				*err = path + ": xml no jinjie_act_theme node";
				return false;
			}

			iRet = this->InitJinJieThemeAct(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitJinJieThemeAct failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 消费领奖
			
			PugiXmlNode root_element = RootElement.child("consum_gift");
			if (root_element.empty())
			{
				*err = path + ": xml no consum_gift node";
				return false;
			}

			iRet = this->InitConsumGiftCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitConsumGiftCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
			//
			root_element = RootElement.child("consum_gift_roll_reward_pool");
			if (root_element.empty())
			{
				*err = path + ": xml no consum_gift_roll_reward_pool node";
				return false;
			}

			iRet = this->InitConsumGiftRollPoolCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitConsumGiftRollPoolCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
			//
			root_element = RootElement.child("consum_gift_roll");
			if (root_element.empty())
			{
				*err = path + ": xml no consum_gift_roll node";
				return false;
			}

			iRet = this->InitConsumGiftRollCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitConsumGiftRollCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 每日限购
			PugiXmlNode root_element = RootElement.child("daily_limit_buy");
			if (root_element.empty())
			{
				*err = path + ": xml no daily_limit_buy node";
				return false;
			}

			iRet = this->InitDailyLimitBuyConfig(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitDailyLimitBuyConfig failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 聚宝盆
			
			PugiXmlNode root_element = RootElement.child("collect_treasure");
			if (root_element.empty())
			{
				*err = path + ": xml no collect_treasure node";
				return false;
			}

			iRet = this->InitCollectTreasureConfig(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitCollectTreasureConfig failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{//欢乐累充
			
			PugiXmlNode root_element = RootElement.child("happy_cumul_chongzhi");
			if (root_element.empty())
			{
				*err = path + ": xml no happy_cumul_chongzhi node";
				return false;
			}

			iRet = this->InitHappyCumulChongzhiConfig(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitHappyCumulChongzhiConfig failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 暴击日
			PugiXmlNode root_element = RootElement.child("baoji_day");
			if (root_element.empty())
			{
				*err = path + ": xml no baoji_day node";
				return false;
			}

			iRet = this->InitCriticalStrikeDayConfig(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitCriticalStrikeDayConfig failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 进阶返还
			PugiXmlNode root_element = RootElement.child("jinjie_return");
			if (root_element.empty())
			{
				*err = path + ": xml no jinjie_return node";
				return false;
			}

			iRet = this->InitJinJieReturnConfig(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitJinJieReturnConfig failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 消费换大礼-物品配置
			PugiXmlNode root_element = RootElement.child("consume_for_gift");
			if (root_element.empty())
			{
				*err = path + ": xml no consume_for_gift node";
				return false;
			}

			iRet = this->InitConsumeForGiftCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitConsumeForGiftCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 秘境寻宝
			PugiXmlNode root_element = RootElement.child("mijingxunbao3");
			if (root_element.empty())
			{
				*err = path + ": xml no mijingxunbao3 node";
				return false;
			}

			iRet = this->InitMiJingXunBao3Cfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitMiJingXunBao3Cfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}

			//秘境寻宝累抽奖励配置
			root_element = RootElement.child("mijingxunbao3_reward");
			if (root_element.empty())
			{
				*err = path + ": xml no mijingxunbao3_reward node";
				return false;
			}

			iRet = this->InitMiJingXunBao3RewardCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitMiJingXunBao3RewardCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}

			root_element = RootElement.child("mijingxunbao3");
			if (root_element.empty())
			{
				*err = path + ":xml no mijingxunbao3_server_reward";
				return false;
			}

			iRet = this->InitMiJingXunBao3BaodiCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s:InitMiJingXunBao3Baodicfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 欢乐砸蛋
			PugiXmlNode root_element = RootElement.child("huanlezadan");
			if (root_element.empty())
			{
				*err = path + ": xml no huanlezadan node";
				return false;
			}

			iRet = this->InitHuanLeZaDanCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitHuanLeZaDanCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}

			//欢乐砸蛋累抽奖励配置
			root_element = RootElement.child("huanlezadan_reward");
			if (root_element.empty())
			{
				*err = path + ": xml no huanlezadan_reward node";
				return false;
			}

			iRet = this->InitHuanLeZaDanRewardCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitHuanLeZaDanRewardCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 欢乐摇奖
			PugiXmlNode root_element = RootElement.child("huanleyaojiang");
			if (root_element.empty())
			{
				*err = path + ": xml no huanleyaojiang node";
				return false;
			}

			iRet = this->InitHuanLeYaoJiangCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitHuanLeYaoJiangCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}

			// 欢乐摇奖累抽奖励配置
			root_element = RootElement.child("huanleyaojiang_reward");
			if (root_element.empty())
			{
				*err = path + ": xml no huanleyaojiang_reward node";
				return false;
			}

			iRet = this->InitHuanLeYaoJiangRewardCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitHuanLeYaoJiangRewardCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}

			root_element = RootElement.child("huanleyaojiang");
			if (root_element.empty())
			{
				*err = path + "xml no huanleyaojiang node";
				return false;
			}

			iRet = this->InitHuanLeYaoJiangBaodiCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s:InitHuanLeYaoJiangBaodiCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 限时回馈
			PugiXmlNode root_element = RootElement.child("limit_time_rebate");
			if (root_element.empty())
			{
				*err = path + ": xml no limit_time_rebate node";
				return false;
			}

			iRet = this->InitLimitTimeRebateCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitLimitTimeRebateCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 限时礼包
			PugiXmlNode root_element = RootElement.child("timelimit_gift");
			if (root_element.empty())
			{
				*err = path + ": xml no timelimit_gift node";
				return false;
			}

			iRet = this->InitLimitTimeGiftCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitLimitTimeGiftCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 循环充值2
			PugiXmlNode root_element = RootElement.child("circulation_chongzhi_2");
			if (root_element.empty())
			{
				*err = path + ": xml no circulation_chongzhi_2 node";
				return false;
			}

			iRet = this->InitCirculationChongzhiSecondRewardCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitCirculationChongzhiSecondRewardCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 疯狂摇钱树
			PugiXmlNode root_element = RootElement.child("shake_money");
			if (root_element.empty())
			{
				*err = path + ": xml no shake_money node";
				return false;
			}

			iRet = this->InitShakeMoneyCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitShakeMoneyCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 限时豪礼
			PugiXmlNode root_element = RootElement.child("timelimit_luxury_gift_bag");
			if (root_element.empty())
			{
				*err = path + ": xml no timelimit_luxury_gift_bag node";
				return false;
			}

			iRet = this->InitTimeLimitLuxuryGiftBag(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitTimeLimitLuxuryGiftBag failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 消费返礼
			PugiXmlNode root_element = RootElement.child("consume_gold_reward");
			if (root_element.empty())
			{
				*err = path + ": xml no consume_gold_reward node";
				return false;
			}

			iRet = this->InitConsumeRewardCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitConsumeRewardCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 神秘商店
			PugiXmlNode root_element = RootElement.child("rmb_buy_chest_shop");
			if (root_element.empty())
			{
				*err = path + ": xml no rmb_buy_chest_shop node";
				return false;
			}

			iRet = this->InitRmbBuyChestShopCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitRmbBuyChestShopCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 集月饼活动
			PugiXmlNode root_element = RootElement.child("item_collection_2");
			if (root_element.empty())
			{
				*err = path + ": xml no item_collection_2 node";
				return false;
			}

			iRet = this->InitItemCollectionSecondRewardCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitItemCollectionSecondRewardCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 版本累计充值
			PugiXmlNode root_element = RootElement.child("versions_grand_total_charge");
			if (root_element.empty())
			{
				*err = path + ": xml no versions_grand_total_charge node";
				return false;
			}

			iRet = this->InitGrandTotalChargeCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitGrandTotalChargeCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 版本连续充值
			PugiXmlNode root_element = RootElement.child("versions_continuous_charge");
			if (root_element.empty())
			{
				*err = path + ": xml no versions_continuous_charge node";
				return false;
			}

			iRet = this->InitContinueChargeRewardChuCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitContinueChargeRewardChuCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}	
		{// 欢乐摇奖2
			PugiXmlNode root_element = RootElement.child("huanleyaojiang2");
			if (root_element.empty())
			{
				*err = path + ": xml no huanleyaojiang2 node";
				return false;
			}

			iRet = this->InitHuanLeYaoJiang2Cfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitHuanLeYaoJiang2Cfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}

			root_element = RootElement.child("huanleyaojiang2_reward");
			if (root_element.empty())
			{
				*err = path + ": xml no huanleyaojiang2_reward node";
				return false;
			}

			iRet = this->InitHuanLeYaoJiang2RewardCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitHuanLeYaoJiang2RewardCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{ // 线下单笔充值
			PugiXmlNode root_element = RootElement.child("offline_single_charge_0");
			if (root_element.empty())
			{
				*err = path + ": xml no offline_single_charge_0 node";
				return false;
			}

			iRet = this->InitOfflineSingleChargeCfg0(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitOfflineSingleChargeCfg0 failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 买一送一
			PugiXmlNode root_element = RootElement.child("buy_one_get_one_free");
			if (root_element.empty())
			{
				*err = path + ": xml no buy_one_get_one_free node";
				return false;
			}

			iRet = this->InitBuyOneGetOneFreeCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitBuyOneGetOneFreeCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 击杀排行奖励
			PugiXmlNode kill_rank_element = RootElement.child("holiday_guard_kill_rank_reward");
			if (kill_rank_element.empty())
			{
				*err = path + ": no [holiday_guard_kill_rank_reward].";
				return false;
			}

			iRet = this->InitHolidayGuardKillRankRewardCfg(kill_rank_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitHolidayGuardKillRankReward failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 累计充值5（版本吉祥三宝）
			PugiXmlNode root_element = RootElement.child("grand_total_charge_five");
			if (root_element.empty())
			{
				*err = path + ": xml no grand_total_charge_five node";
				return false;
			}


			iRet = this->InitTotalChongzhi5Cfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitTotalChongzhi5Cfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 极限挑战 
			PugiXmlNode extreme_challenge_element = RootElement.child("extreme_challenge");
			if (extreme_challenge_element.empty())
			{
				*err = path + ": no [extreme_challenge].";
				return false;
			}

			iRet = this->InitExtremeChallengeCfg(extreme_challenge_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitExtremeChallengeCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 你充我送
			
			PugiXmlNode chongzhi_gift_element = RootElement.child("chongzhi_gift");
			if (chongzhi_gift_element.empty())
			{
				*err = path + ": no [chongzhi_gift].";
				return false;
			}

			iRet = this->InitChongZhiGiftCfg(chongzhi_gift_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitChongZhiGiftCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}
		
		{
			// 表白排行榜奖励配置
			PugiXmlNode extreme_challenge_element = RootElement.child("profess_rank");
			if (extreme_challenge_element.empty())
			{
				*err = path + ": no [profess_rank].";
				return false;
			}

			iRet = this->InitProfessRankRewardCfg(extreme_challenge_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitProfessRankRewardCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 进阶返还2
			PugiXmlNode root_element = RootElement.child("jinjie_return2");
			if (root_element.empty())
			{
				*err = path + ": xml no jinjie_return2 node";
				return false;
			}

			iRet = this->InitJinJieReturn2Config(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitJinJieReturn2Config failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 暴击日2
			PugiXmlNode root_element = RootElement.child("baoji_day2");
			if (root_element.empty())
			{
				*err = path + ": xml no baoji_day2 node";
				return false;
			}

			iRet = this->InitCriticalStrikeDay2Config(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitCriticalStrikeDay2Config failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 组合购买
			PugiXmlNode root_element = RootElement.child("combine_buy_discount");
			if (root_element.empty())
			{
				*err = path + ": xml no combine_buy_discount node";
				return false;
			}

			iRet = this->InitCombineBuyDiscountCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitCombineBuyDiscountCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}

			root_element = RootElement.child("combine_buy_item");
			if (root_element.empty())
			{
				*err = path + ": xml no combine_buy_item node";
				return false;
			}

			iRet = this->InitCombineBuyItemCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitCombineBuyItemCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 幸运许愿每日奖励配置
			PugiXmlNode root_element = RootElement.child("lucky_wish_everyday");
			if (root_element.empty())
			{
				*err = path + ": xml no lucky_wish_everyday node";
				return false;
			}

			iRet = this->InitLuckyWishLotteryCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitLuckyWishLotteryCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 幸运许愿大奖配置
			PugiXmlNode root_element = RootElement.child("lucky_wish_big_reward");
			if (root_element.empty())
			{
				*err = path + ": xml no lucky_wish_big_reward node";
				return false;
			}

			iRet = this->InitLuckyWishLuckyCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitLuckyWishLuckyCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 形象比拼
			PugiXmlNode root_element = RootElement.child("image_competition_type");
			if (root_element.empty())
			{
				*err = path + ": xml no image_competition_type node";
				return false;
			}

			iRet = this->InitImageCompetitionCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitImageCompetitionCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{// 形象比拼奖励
			PugiXmlNode root_element = RootElement.child("image_competition_reward");
			if (root_element.empty())
			{
				*err = path + ": xml no image_competition_reward node";
				return false;
			}

			iRet = this->InitImageCompetitionRewardCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitImageCompetitionRewardCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		// 周末狂欢奖励
		{
			PugiXmlNode root_element = RootElement.child("lottery_1");
			if (root_element.empty())
			{
				*err = path + ": xml no lottery_1 node";
				return false;
			}

			iRet = this->InitLottery1RewardCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitLottery1RewardCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		// 周末狂欢个人奖励
		{
			PugiXmlNode root_element = RootElement.child("lottery_1_person_reward");
			if (root_element.empty())
			{
				*err = path + ": xml no lottery_1_person_reward node";
				return false;
			}

			iRet = this->InitLottery1PersonRewardCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitLottery1PersonRewardCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		// 周末狂欢全服奖励
		{
			PugiXmlNode root_element = RootElement.child("lottery_1_server_reward");
			if (root_element.empty())
			{
				*err = path + ": xml no lottery_1_server_reward node";
				return false;
			}

			iRet = this->InitLottery1ServerRewardCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitLottery1ServerRewardCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

				{//每日一爱
			PugiXmlNode root_element = RootElement.child("daily_love_reward_percent");
			if (root_element.empty())
			{
				*err = path + ": xml no daily_love_reward_percent node";
				return false;
			}

			iRet = this->InitDailyLoveRewardPercentCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitDailyLoveRewardPercentCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}
		
		{
			// 零元购
			PugiXmlNode root_element = RootElement.child("zero_buy_return");
			if (root_element.empty())
			{
				*err = path + ": xml no zero_buy_return node";
				return false;
			}

			iRet = this->InitZeroBuyReturnCfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitZeroBuyReturnCfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		// 每日一爱2 (单笔返利)配置
		{
			PugiXmlNode root_element = RootElement.child("daily_love2");
			if (root_element.empty())
			{
				*err = path + ": xml no daily_love2 node";
				return false;
			}

			iRet = this->InitDailyLove2Cfg(root_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitDailyLove2Cfg failed %d", path.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}
	}

	// 狂嗨庆典
	{
		PugiXmlNode kuang_hai_element = RootElement.child("kuang_hai");
		if (kuang_hai_element.empty())
		{
			*err = path + ": xml no kuang_hai node";
			return false;
		}

		iRet = this->InitKuangHaiQingDianCfg(kuang_hai_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitKuangHaiQingDianCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 狂嗨庆典奖励
	{
		PugiXmlNode kuang_hai_reward_element = RootElement.child("kuang_hai_reward");
		if (kuang_hai_reward_element.empty())
		{
			*err = path + ": xml no kuang_hai_reward node";
			return false;
		}

		iRet = this->InitKuangHaiQingDianRewardCfg(kuang_hai_reward_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitKuangHaiQingDianRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 礼物收割排行奖励
	{
		PugiXmlNode gift_harvest_reward_element = RootElement.child("gift_harvest_rank_reward");
		if (gift_harvest_reward_element.empty())
		{
			*err = path + ": xml no gift_harvest_rank_reward node";
			return false;
		}

		iRet = this->InitGiftHarvestRankRwardCfg(gift_harvest_reward_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitGiftHarvestRankRwardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 幸运云购其他配置
		PugiXmlNode lucky_cloud_buy_other_element = RootElement.child("lucky_cloud_buy_other");
		if (lucky_cloud_buy_other_element.empty())
		{
			*err = path + ": xml no lucky_cloud_buy_other node";
			return false;
		}

		iRet = this->InitLuckyCloudBuyOtherCfg(lucky_cloud_buy_other_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitLuckyCloudBuyOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 幸运云购配置
		PugiXmlNode lucky_cloud_buy_element = RootElement.child("lucky_cloud_buy");
		if (lucky_cloud_buy_element.empty())
		{
			*err = path + ": xml no lucky_cloud_buy node";
			return false;
		}

		iRet = this->InitLuckyCloudBuyCfg(lucky_cloud_buy_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitLuckyCloudBuyCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 首充团购假人数
		PugiXmlNode first_charge_fix_cfg_element = RootElement.child("num_first_group");
		if (first_charge_fix_cfg_element.empty())
		{
			*err = path + ": xml no num_first_group node";
			return false;
		}

		iRet = this->InitFirstChargeTuanCfg(first_charge_fix_cfg_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitFirstChargeTuanCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}


	{
		// 狂欢大乐购
		PugiXmlNode root_element = RootElement.child("happy_shopping");
		if (root_element.empty())
		{
			*err = path + ": xml no [ happy_shopping ] node\n";
			return false;
		}

		iRet = this->InitCracyBuyCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitCracyBuyCfg faild %d \n", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 双倍元宝
		PugiXmlNode root_element = RootElement.child("double_get");
		if (root_element.empty())
		{
			*err = path + ": xml no [ double_get ] node\n";
			return false;
		}

		iRet = this->InitDoubleGetCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitDoubleGetCfg faild %d \n", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int RandActivityConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;
	m_continue_consume_other_day_cfg_count = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}
		
		if (opengame_day != last_opengame_day)
		{
			++ stage_num;
			++m_continue_consume_other_day_cfg_count;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (!PugiGetSubNodeValue(data_element, "lucky_roll_get_gold_min_gold_poll", m_luckyroll_cfg[stage_num].cfg.get_gold_min_gold_poll) || m_luckyroll_cfg[stage_num].cfg.get_gold_min_gold_poll < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "lucky_roll_replace_gold_reward_seq", m_luckyroll_cfg[stage_num].cfg.replace_gold_reward_seq) || m_luckyroll_cfg[stage_num].cfg.replace_gold_reward_seq < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "lucky_roll_extra_reward_need_times", m_luckyroll_cfg[stage_num].cfg.extra_reward_need_times) || m_luckyroll_cfg[stage_num].cfg.extra_reward_need_times <= 0)
		{
			return -3;
		}

		PugiXmlNode lucky_roll_reward_item_element = data_element.child("lucky_roll_reward_item");
		if (lucky_roll_reward_item_element.empty())
		{
			return -4;
		}

		if (!m_luckyroll_cfg[stage_num].cfg.extra_reward_item.ReadConfig(lucky_roll_reward_item_element))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "lucky_roll_unit_roll_consume_gold", m_luckyroll_cfg[stage_num].cfg.unit_roll_consume_gold) || m_luckyroll_cfg[stage_num].cfg.unit_roll_consume_gold <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "lucky_roll_unit_enter_gold_pool_num", m_luckyroll_cfg[stage_num].cfg.unit_enter_gold_pool_num) || m_luckyroll_cfg[stage_num].cfg.unit_enter_gold_pool_num <= 0 || 
			m_luckyroll_cfg[stage_num].cfg.unit_enter_gold_pool_num > m_luckyroll_cfg[stage_num].cfg.unit_roll_consume_gold)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "fanli_rate", m_consume_gold_fanli_rate) || m_consume_gold_fanli_rate <= 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "chestshop_discount", m_chestshop_discount) || m_chestshop_discount <= 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "quanmin_qifu_discount", m_quanmin_qifu_discount) || m_quanmin_qifu_discount <= 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "need_kill_boss_count", m_kill_boss_reward_cfg.need_kill_boss_count) || m_kill_boss_reward_cfg.need_kill_boss_count <= 0)
		{
			return -12;
		}

		PugiXmlNode reward_item = data_element.child("kill_boss_reward");
		if (reward_item.empty())
		{
			return -13;
		}

		if (!m_kill_boss_reward_cfg.reward_item.ReadConfig(reward_item))
		{
			return -14;
		}

		{
			PugiXmlNode day_online_reward_item = data_element.child("day_online_gift_reward");
			if (day_online_reward_item.empty())
			{
				return -15;
			}

			if (!m_day_online_gift_cfg[stage_num].reward_item.ReadConfig(day_online_reward_item))
			{
				return -16;
			}
		}

		{
			// 登录送礼
			if (!PugiGetSubNodeValue(data_element, "login_gift_accumulate_reward_need_days", m_login_gift_cfg[stage_num].need_accumulate_days) || m_login_gift_cfg[stage_num].need_accumulate_days <= 0)
			{
				return -17;
			}

			PugiXmlNode reward_item = data_element.child("login_gift_accumulate_reward");
			if (reward_item.empty())
			{
				return -18;
			}

			if (!m_login_gift_cfg[stage_num].accumulate_reward.ReadConfig(reward_item))
			{
				return -19;
			}
		}

		{
			if (!PugiGetSubNodeValue(data_element, "tomorrow_reward_need_active_degree", m_tomorrow_reward_need_active_degree) || m_tomorrow_reward_need_active_degree <= 0)
			{
				return -21;
			}
		}

		{
			//充值扭蛋所需额度
			if (!PugiGetSubNodeValue(data_element, "niu_egg_need_charge", m_niu_egg_need_charge) || m_niu_egg_need_charge <= 0)
			{
				return -22;
			}
		}

		{
			//珍宝阁
			if (!PugiGetSubNodeValue(data_element, "zhenbaoge_reflush_gold", m_zhenbaoge_reflush_gold) || m_zhenbaoge_reflush_gold <= 0)
			{
				return -23;
			}

			if (!PugiGetSubNodeValue(data_element, "zhenbaoge_flush_item", m_zhenbaoge_flush_item) || NULL == ITEMPOOL->GetItem(m_zhenbaoge_flush_item))
			{
				return -2300;
			}

			if (!PugiGetSubNodeValue(data_element, "zhenbaoge_auto_flush_times", m_zhenbaoge_auto_flush_times) || m_zhenbaoge_auto_flush_times <= 0 || m_zhenbaoge_auto_flush_times >= 200)
			{
				return -24;
			}

			PugiXmlNode reward_item_element = data_element.child("zhenbaoge_all_buy_reward");
			if (reward_item_element.empty())
			{
				return -25;
			}

			if (!m_zhenbaoge_all_buy_reward.ReadConfig(reward_item_element))
			{
				return -26;
			}
		}

		{
			//摇钱树
			if (!PugiGetSubNodeValue(data_element, "money_tree_need_gold", m_rock_money_tree_need_gold) || m_rock_money_tree_need_gold <= 0)
			{
				return -27;
			}

			if (!PugiGetSubNodeValue(data_element, "money_tree_into_pool", m_money_tree_into_pool) || m_money_tree_into_pool <= 0)
			{
				return -28;
			}

			if (!PugiGetSubNodeValue(data_element, "money_tree_init_gold", m_money_tree_init_pool_gold) || m_money_tree_init_pool_gold < 0)
			{
				return -29;
			}

			if (!PugiGetSubNodeValue(data_element, "money_tree_free_times", m_money_tree_free_times) || m_money_tree_free_times < 0)
			{
				return -30;
			}

			if (!PugiGetSubNodeValue(data_element, "money_tree_free_interval", m_money_tree_free_interval) || m_money_tree_free_interval < 0)
			{
				return -31;
			}
		}

		{
			//摇钱树2
			if (!PugiGetSubNodeValue(data_element, "money_tree2_need_gold", m_rock_money_tree2_need_gold) || m_rock_money_tree2_need_gold <= 0)
			{
				return -270;
			}

			if (!PugiGetSubNodeValue(data_element, "money_tree2_into_pool", m_money_tree2_into_pool) || m_money_tree2_into_pool <= 0)
			{
				return -280;
			}

			if (!PugiGetSubNodeValue(data_element, "money_tree2_init_gold", m_money_tree2_init_pool_gold) || m_money_tree2_init_pool_gold < 0)
			{
				return -290;
			}
		}

		{
			if (!PugiGetSubNodeValue(data_element, "mijingxunbao_once_gold", m_mijingxunbao_once_gold) || m_mijingxunbao_once_gold <= 0)
			{
				return -30;
			}

			if (!PugiGetSubNodeValue(data_element, "mijingxunbao_tentimes_gold", m_mijingxunbao_tentimes_gold) || m_mijingxunbao_tentimes_gold <= 0)
			{
				return -31;
			}

			if (!PugiGetSubNodeValue(data_element, "mijingxunbao_fifthtimes_gold", m_mijingxunbao_fifthtimes_gold) || m_mijingxunbao_fifthtimes_gold <= 0)
			{
				return -32;
			}

			if (!PugiGetSubNodeValue(data_element, "mijingxunbao_interval", m_mijingxunbao_interval) || m_mijingxunbao_interval <= 0)
			{
				return -33;
			}

			if (!PugiGetSubNodeValue(data_element, "add_weight_tao_times", m_add_weight_tao_times) || m_add_weight_tao_times <= 0)
			{
				return -34;
			}

			if (!PugiGetSubNodeValue(data_element, "mijingxunbao_fifthtimes_item_id", m_mijingxunbao_fifthtimes_item_id) || NULL == ITEMPOOL->GetItem(m_mijingxunbao_fifthtimes_item_id))
			{
				return -35;
			}

		}

		{
			//秘境寻宝2
			if (!PugiGetSubNodeValue(data_element, "mijingxunbao2_once_gold", m_mijingxunbao2_once_gold) || m_mijingxunbao2_once_gold <= 0)
			{
				return -300;
			}

			if (!PugiGetSubNodeValue(data_element, "mijingxunbao2_tentimes_gold", m_mijingxunbao2_tentimes_gold) || m_mijingxunbao2_tentimes_gold <= 0)
			{
				return -310;
			}

			if (!PugiGetSubNodeValue(data_element, "mijingxunbao2_fifthtimes_gold", m_mijingxunbao2_fifthtimes_gold) || m_mijingxunbao2_fifthtimes_gold <= 0)
			{
				return -320;
			}

			if (!PugiGetSubNodeValue(data_element, "mijingxunbao2_interval", m_mijingxunbao2_interval) || m_mijingxunbao2_interval <= 0)
			{
				return -330;
			}

			if (!PugiGetSubNodeValue(data_element, "add_weight_tao2_times", m_add_weight_tao2_times) || m_add_weight_tao2_times <= 0)
			{
				return -340;
			}

			if (!PugiGetSubNodeValue(data_element, "mijingxunbao2_fifthtimes_item_id", m_mijingxunbao2_fifthtimes_item_id) || NULL == ITEMPOOL->GetItem(m_mijingxunbao2_fifthtimes_item_id))
			{
				return -350;
			}
		}

		{
			//陛下请翻牌
			if (!PugiGetSubNodeValue(data_element, "king_draw_chuji_once_gold", m_kingdraw_level_chuji_consume) || m_kingdraw_level_chuji_consume <= 0)
			{
				return -37;
			}

			if (!PugiGetSubNodeValue(data_element, "king_draw_zhongji_once_gold", m_kingdraw_level_zhongji_consume) || m_kingdraw_level_zhongji_consume <= 0)
			{
				return -38;
			}

			if (!PugiGetSubNodeValue(data_element, "king_draw_gaoji_once_gold", m_kingdraw_level_gaoji_consume) || m_kingdraw_level_gaoji_consume <= 0)
			{
				return -39;
			}

			if (!PugiGetSubNodeValue(data_element, "king_draw_gaoji_consume_item", m_king_draw_gaoji_consume_item) || NULL == ITEMPOOL->GetItem(m_king_draw_gaoji_consume_item))
			{
				return -40;
			}

			if (!PugiGetSubNodeValue(data_element, "jinyinta_consume_item", m_level_lottery_consume_item) || NULL == ITEMPOOL->GetItem(m_level_lottery_consume_item))
			{
				return -1000;
			}

			if (!PugiGetSubNodeValue(data_element, "money_tree_consume_item", m_money_tree_consume_item) || NULL == ITEMPOOL->GetItem(m_money_tree_consume_item))
			{
				return -1001;
			}
		}

		{	
			//特殊形象变身排行榜
			PugiXmlNode reward_item = data_element.child("special_appearance_rank_join_reward");
			if (reward_item.empty())
			{
				return -41;
			}

			if (!m_appearance_rank_join_reward.ReadConfig(reward_item))
			{
				return -42;
			}
		}

		{
			//被动变身排行榜
			PugiXmlNode reward_item = data_element.child("special_appearance_passive_rank_join_reward");
			if (reward_item.empty())
			{
				return -43;
			}

			if (!m_appearance_passive_rank_join_reward.ReadConfig(reward_item))
			{
				return -44;
			}
		}

		{
			//全场怪物掉落
			if (!PugiGetSubNodeValue(data_element, "any_monster_drop_limit_drop_time", m_any_monster_drop_limit_drop_time) || m_any_monster_drop_limit_drop_time <= 0)
			{
				return -45;
			}

			if (!PugiGetSubNodeValue(data_element, "any_monster_drop_id", m_any_monster_drop_id) || m_any_monster_drop_id <= 0)
			{
				return -46;
			}

			if (!PugiGetSubNodeValue(data_element, "any_monster_drop_day_limit_rate", m_any_monster_drop_day_limit_rate) || m_any_monster_drop_day_limit_rate <= 0)
			{
				return -47;
			}
		}

		{
			if (!PugiGetSubNodeValue(data_element, "mine_refresh_gold", m_mine_refresh_gold) || m_mine_refresh_gold <= 0)
			{
				return -48;
			}

			if (!PugiGetSubNodeValue(data_element, "mine_refresh_num", m_mine_refresh_count) || m_mine_refresh_count > RA_MINE_MAX_REFRESH_COUNT || m_mine_refresh_count < 0)
			{
				return -49;
			}

			if (!PugiGetSubNodeValue(data_element, "mine_free_times", m_mine_free_times) || m_mine_free_times < 0)
			{
				return -50;
			}

			if (!PugiGetSubNodeValue(data_element, "mine_server_reward_level", m_mine_server_reward_level_limit) || m_mine_server_reward_level_limit < 0)
			{
				return -51;
			}
		}

		{
			PugiXmlNode item_element_list = data_element.child("guagua_anwei_reward_list");
			if (item_element_list.empty())
			{
				return -52;
			}

			PugiXmlNode item_element = item_element_list.child("guagua_anwei_reward");
			if (item_element.empty())
			{
				return -53;
			}

			int anwei_reward_count = 0;
			while (!item_element.empty())
			{
				if (anwei_reward_count >= RA_GUAGUA_REWARD_COUNT_PER_AREA)
				{
					return -54;
				}

				/*if (!m_guagua_anwei_reward.reward_item_list[anwei_reward_count].ReadConfig(item_element))
				{
					return -55;
				}*/

				++ anwei_reward_count;

				item_element = item_element.next_sibling();
			}

			if (!PugiGetSubNodeValue(data_element, "guagua_once_gold", m_guagua_need_gold_list[RA_GUAGUA_PLAY_ONE_TIME]) || m_guagua_need_gold_list[RA_GUAGUA_PLAY_ONE_TIME] <= 0)
			{
				return -56;
			}

			if (!PugiGetSubNodeValue(data_element, "guagua_tentimes_gold", m_guagua_need_gold_list[RA_GUAGUA_PLAY_TEN_TIMES]) || m_guagua_need_gold_list[RA_GUAGUA_PLAY_TEN_TIMES] <= 0)
			{
				return -57;
			}

			if (!PugiGetSubNodeValue(data_element, "guagua_fiftytimes_gold", m_guagua_need_gold_list[RA_GUAGUA_PLAY_THIRTY_TIMES]) || m_guagua_need_gold_list[RA_GUAGUA_PLAY_THIRTY_TIMES] <= 0)
			{
				return -58;
			}

			if (!PugiGetSubNodeValue(data_element, "guagua_free_time_interval", m_guagua_free_time_interval) || m_guagua_free_time_interval <= 0)
			{
				return -59;
			}

			if (!PugiGetSubNodeValue(data_element, "guagua_roll_item_id", m_guagua_roll_item_id) || m_guagua_roll_item_id <= 0)
			{
				return -1159;
			}
		}

		{
			if (!PugiGetSubNodeValue(data_element, "tianming_free_chou_times", m_tianming_free_chou_times) || m_tianming_free_chou_times < 0)
			{
				return -60;
			}		
		}

		{
			// 翻翻转
			if (!PugiGetSubNodeValue(data_element, "fanfan_free_fan_times_per_day", m_fanfan_free_fan_times_per_day) || m_fanfan_free_fan_times_per_day < 0)
			{
				return -61;
			}

			if (!PugiGetSubNodeValue(data_element, "fanfan_once_need_gold", m_fanfan_once_need_gold) || m_fanfan_once_need_gold <= 0)
			{
				return -62;
			}

			if (!PugiGetSubNodeValue(data_element, "fanfan_refresh_need_gold", m_fanfan_refresh_need_gold) || m_fanfan_refresh_need_gold <= 0)
			{
				return -63;
			}

			if (!PugiGetSubNodeValue(data_element, "fanfan_auto_refresh_interval", m_fanfan_auto_refresh_interval) || m_fanfan_auto_refresh_interval <= 0)
			{
				return -64;
			}

			if (!PugiGetSubNodeValue(data_element, "fanfan_word_increase_rate", m_fanfan_word_increase_rate) || m_fanfan_word_increase_rate < 0)
			{
				return -65;
			}
		}
			
		// 连续充值、消费
		{
			if (!PugiGetSubNodeValue(data_element, "continue_chongzhi_fetch_extra_reward_need_days", m_continue_chongzhi_fetch_extra_reward_need_days) || m_continue_chongzhi_fetch_extra_reward_need_days <= 0)
			{
				return -66;
			}	

			PugiXmlNode continue_chongzhi_extra_reward_item_element = data_element.child("continue_chongzhi_extra_reward");
			if (continue_chongzhi_extra_reward_item_element.empty())
			{
				return -67;
			}

			if (!m_continue_chongzhi_extra_reward.ReadConfig(continue_chongzhi_extra_reward_item_element))
			{
				return -68;
			}
		}

		{
			RandActivityContinueConsumeOtherDayCfg &day_cfg = m_continue_consume_other_cfg_list[stage_num];
			++day_cfg.cfg_count;
			day_cfg.opengame_day = opengame_day;

			if (!PugiGetSubNodeValue(data_element, "continue_consume_fetch_extra_reward_need_days", day_cfg.continue_consume_fetch_extra_reward_need_days) ||
				day_cfg.continue_consume_fetch_extra_reward_need_days <= 0)
			{
				return -69;
			}

			PugiXmlNode continue_consume_extra_reward_item_element = data_element.child("continue_consume_extra_reward");
			if (continue_consume_extra_reward_item_element.empty())
			{
				return -70;
			}

			if (!day_cfg.continue_consume_extra_reward.ReadConfig(continue_consume_extra_reward_item_element))
			{
				return -71;
			}
		}

		{
			// 军歌嘹亮
			PugiXmlNode army_day_win_reward_item = data_element.child("army_day_win_reward");
			if (army_day_win_reward_item.empty())
			{
				return -72;
			}

			if (!m_army_day_win_side_reward.ReadConfig(army_day_win_reward_item))
			{
				return -73;
			}
		}

		{
			// 幸运至尊 --刷新消耗
			if (!PugiGetSubNodeValue(data_element, "extreme_lucky_flush_consume", m_extreme_lucky_flush_consume_glod) || m_extreme_lucky_flush_consume_glod <= 0)
			{
				return -74;
			}
			// 每日免费次数
			if (!PugiGetSubNodeValue(data_element, "extreme_lucky_free_draw_times", m_extreme_lucky_free_draw_times) || m_extreme_lucky_free_draw_times <= 0)
			{
				return -10074;
			}
		}

		{
			//珍宝阁2
			if (!PugiGetSubNodeValue(data_element, "zhenbaoge2_reflush_gold", m_zhenbaoge2_reflush_gold) || m_zhenbaoge2_reflush_gold <= 0)
			{
				return -75;
			}

			if (!PugiGetSubNodeValue(data_element, "zhenbaoge2_auto_flush_times", m_zhenbaoge2_auto_flush_times) || m_zhenbaoge2_auto_flush_times <= 0 || m_zhenbaoge2_auto_flush_times >= 200)
			{
				return -76;
			}

			PugiXmlNode reward_item_element = data_element.child("zhenbaoge2_all_buy_reward");
			if (reward_item_element.empty())
			{
				return -77;
			}

			if (!m_zhenbaoge2_all_buy_reward.ReadConfig(reward_item_element))
			{
				return -78;
			}
		}

		{
			// 步步高升
			if (!PugiGetSubNodeValue(data_element, "promoting_position_free_play_interval", m_promoting_position_free_play_interval) || m_promoting_position_free_play_interval <= 0)
			{
				return -79;
			}

			if (!PugiGetSubNodeValue(data_element, "promoting_position_10_times_use_item", m_promoting_position_10_times_use_item) || NULL == ITEMPOOL->GetItem(m_promoting_position_10_times_use_item))
			{
				return -80;
			}

			if (!PugiGetSubNodeValue(data_element, "promoting_position_extra_time_per_chongzhi_gold", m_promoting_position_extra_time_per_chongzhi_gold) || m_promoting_position_extra_time_per_chongzhi_gold <= 0)
			{
				return -81;
			}

			if (!PugiGetSubNodeValue(data_element, "promoting_position_play_once_gold", m_promoting_position_play_once_gold) || m_promoting_position_play_once_gold <= 0)
			{
				return -82;
			}

			if (!PugiGetSubNodeValue(data_element, "promoting_position_10_times_gold", m_promoting_position_10_times_gold) || m_promoting_position_10_times_gold <= 0)
			{
				return -83;
			}
		}

		{
			// 新聚宝盆
			if (!PugiGetSubNodeValue(data_element, "new_cornucopia_percent", m_new_cornucopia_percent) || m_new_cornucopia_percent <= 0)
			{
				return -84;
			}
		}

		// 连充特惠初
		{
// 			if (!PugiGetSubNodeValue(data_element, "continue_chongzhi_fetch_extra_reward_need_days_chu", m_continue_chongzhi_fetch_extra_reward_need_days_chu) || m_continue_chongzhi_fetch_extra_reward_need_days_chu <= 0)
// 			{
// 				return -85;
// 			}
// 
// 			PugiXmlNode continue_chongzhi_extra_reward_item_element = data_element.child("continue_chongzhi_extra_reward_chu");
// 			if (continue_chongzhi_extra_reward_item_element.empty())
// 			{
// 				return -86;
// 			}
// 
// 			if (!m_continue_chongzhi_extra_reward_chu.ReadConfig(continue_chongzhi_extra_reward_item_element))
// 			{
// 				return -87;
// 			}
		}

		// 连充特惠高
		{
// 			if (!PugiGetSubNodeValue(data_element, "continue_chongzhi_fetch_extra_reward_need_days_gao", m_continue_chongzhi_fetch_extra_reward_need_days_gao) || m_continue_chongzhi_fetch_extra_reward_need_days_gao <= 0)
// 			{
// 				return -89;
// 			}
// 
// 			PugiXmlNode continue_chongzhi_extra_reward_item_element = data_element.child("continue_chongzhi_extra_reward_gao");
// 			if (continue_chongzhi_extra_reward_item_element.empty())
// 			{
// 				return -90;
// 			}
// 
// 			if (!m_continue_chongzhi_extra_reward_gao.ReadConfig(continue_chongzhi_extra_reward_item_element))
// 			{
// 				return -90;
// 			}
		}

		{
			// 聚划算
			if (!PugiGetSubNodeValue(data_element, "xianyuan_treas_buy_limit_day", m_xianyuan_treas_buy_limit_day) || m_xianyuan_treas_buy_limit_day <= 0)
			{
				return -101;
			}

			PugiXmlNode reward_item = data_element.child("xianyuan_treas_all_buy_reward");
			if (reward_item.empty())
			{
				return -102;
			}

			ItemConfigData tmp_item_config_data;
			if (!tmp_item_config_data.ReadConfig(reward_item))
			{
				return -103;
			}
			m_xianyuan_treas_all_buy_reward[opengame_day] = tmp_item_config_data;
		}

		{
			// 限时秒杀
			if (!PugiGetSubNodeValue(data_element, "rush_buying_duration", m_rush_buying_duration) || m_rush_buying_duration <= 0)
			{
				return -104;
			}
		}

		{
			// 地图寻宝
			if (!PugiGetSubNodeValue(data_element, "map_hunt_free_count", m_map_hunt_free_xunbao_count) || m_map_hunt_free_xunbao_count <= 0)
			{
				return -105;
			}

			if (!PugiGetSubNodeValue(data_element, "free_flush_interval", m_map_hunt_free_flush_interval) || m_map_hunt_free_flush_interval <= 0)
			{
				return -106;
			}

			if (!PugiGetSubNodeValue(data_element, "map_hunt_flush_gold", m_map_hunt_flush_gold) || m_map_hunt_flush_gold <= 0)
			{
				return -107;
			}

			if (!PugiGetSubNodeValue(data_element, "map_hunt_xunbao_glod", m_map_hunt_xunbao_glod) || m_map_hunt_xunbao_glod <= 0)
			{
				return -108;
			}
		}

		{
			if (!PugiGetSubNodeValue(data_element, "planting_tree_tree_living_time", m_planting_tree_tree_living_time) || m_planting_tree_tree_living_time <= 0)
			{
				return -109;
			}

			if (!PugiGetSubNodeValue(data_element, "planting_tree_max_watering_times", m_planting_tree_max_watering_times) || m_planting_tree_max_watering_times <= 0)
			{
				return -110;
			}

			if (!PugiGetSubNodeValue(data_element, "planting_tree_gather_id", m_planting_tree_gather_id) || m_planting_tree_gather_id <= 0)
			{
				return -111;
			}

			PugiXmlNode reward_item_element = data_element.child("planting_tree_canyu_reward");
			if (reward_item_element.empty())
			{
				return -112;
			}

			if (!m_planting_tree_canyu_reward.ReadConfig(reward_item_element))
			{
				return -113;
			}

			if (!PugiGetSubNodeValue(data_element, "planting_tree_max_watering_times", m_planting_tree_max_watering_times) || m_planting_tree_max_watering_times <= 0)
			{
				return -1131;
			}

			if (!PugiGetSubNodeValue(data_element, "planting_tree_scene_id", m_planting_tree_scene_id) || m_planting_tree_scene_id <= 0)
			{
				return -1132;
			}

			if (!PugiGetSubNodeValue(data_element, "planting_tree_seed_id", m_planting_tree_norexitem_id) || m_planting_tree_norexitem_id <= 0 || nullptr == ITEMPOOL->GetItem(m_planting_tree_norexitem_id))
			{
				return -1133;
			}

			if (!PugiGetSubNodeValue(data_element, "planting_tree_watering_time", m_planting_tree_watering_time) || m_planting_tree_watering_time <= 0)
			{
				return -1134;
			}
		}

		{
			if (!PugiGetSubNodeValue(data_element, "fishing_times_limit", m_fish_time_limit) || m_fish_time_limit <= 0)
			{
				return -114;
			}

			if (!PugiGetSubNodeValue(data_element, "steal_fish_time_limit", m_steal_fish_time_limit) || m_steal_fish_time_limit <= 0)
			{
				return -115;
			}

			if (!PugiGetSubNodeValue(data_element, "be_stealed_fish_times_limit", m_be_stealed_fish_time_limit) || m_be_stealed_fish_time_limit <= 0)
			{
				return -116;
			}

			if (!PugiGetSubNodeValue(data_element, "steal_fish_persent", m_steal_fish_percent) || m_steal_fish_percent <= 0)
			{
				return -117;
			}
		}

		{
			if (!PugiGetSubNodeValue(data_element, "crystal_dalaran_initial_cout", m_crystal_dalaran_initial_count) || m_crystal_dalaran_initial_count <= 0)
			{
				return -118;
			}

			if (!PugiGetSubNodeValue(data_element, "steal_crystal_dalaran_day_times", m_steal_crystal_dalaran_time_limit) || m_steal_crystal_dalaran_time_limit <= 0)
			{
				return -119;
			}

			if (!PugiGetSubNodeValue(data_element, "be_stealed_crystal_dalaran_day_times", m_crystal_dalaran_be_stealed_time_limit) || m_crystal_dalaran_be_stealed_time_limit <= 0)
			{
				return -120;
			}

			if (!PugiGetSubNodeValue(data_element, "steal_one_people_day_times", m_steal_crystal_dalaran_one_people_time_limit) || m_steal_crystal_dalaran_one_people_time_limit <= 0)
			{
				return -121;
			}

			if (!PugiGetSubNodeValue(data_element, "steal_crystal_dalaran_percent", m_steal_crystal_dalaran_percent) || m_steal_crystal_dalaran_percent <= 0)
			{
				return -122;
			}

			if (!PugiGetSubNodeValue(data_element, "broadcast_percent", m_broadcast_percent) || m_broadcast_percent <= 0)
			{
				return -123;
			}

			if (!PugiGetSubNodeValue(data_element, "steal_crystal_dalaran_max_num", m_steal_crystal_dalaran_limit) || m_steal_crystal_dalaran_limit <= 0)
			{
				return -124;
			}
		}

		{ // 消费领奖
			if (!PugiGetSubNodeValue(data_element, "consum_gift_roll_reward_gold", m_roll_reward_gold) || m_roll_reward_gold <= 0)
			{
				return -125;
			}

			if (!PugiGetSubNodeValue(data_element, "consume_gift_roll_times_limit", m_roll_times_limit) || m_roll_times_limit <= 0)
			{
				return -1251;
			}
		}

		{// 聚宝盆新
			if (!PugiGetSubNodeValue(data_element, "collect_treasure_gold", m_collect_treasure_gold) || m_collect_treasure_gold <= 0)
			{
				return -126;
			}

			if (!PugiGetSubNodeValue(data_element, "collect_treasure_times", m_collect_treasure_join_times) || m_collect_treasure_join_times <= 0)
			{
				return -127;
			}
		}
		//{
		//	//陛下请翻牌（新增）
		//	if (!PugiGetSubNodeValue(data_element, "king_draw_primary_level_score", m_kingdraw_level_primary_score) || m_kingdraw_level_primary_score <= 0)
		//	{
		//		return -128;
		//	}

		//	if (!PugiGetSubNodeValue(data_element, "king_draw_middle_level_score", m_kingdraw_level_middle_score) || m_kingdraw_level_middle_score <= 0)
		//	{
		//		return -129;
		//	}

		//	if (!PugiGetSubNodeValue(data_element, "king_draw_high_level_score", m_kingdraw_level_high_score) || m_kingdraw_level_high_score <= 0)
		//	{
		//		return -130;
		//	}
		//}

		// 秘境寻宝3
		{
			if (!PugiGetSubNodeValue(data_element, "mijingxunbao3_once_gold", m_mijingxunbao3_once_gold) || m_mijingxunbao3_once_gold <= 0)
			{
				return -128;
			}

			if (!PugiGetSubNodeValue(data_element, "mijingxunbao3_tentimes_gold", m_mijingxunbao3_tentimes_gold) || m_mijingxunbao_tentimes_gold <= 0)
			{
				return -129;
			}

			if (!PugiGetSubNodeValue(data_element, "mijingxunbao3_thirtytimes_gold", m_mijingxunbao3_thirtytimes_gold) || m_mijingxunbao3_thirtytimes_gold <= 0)
			{
				return -130;
			}

			if (!PugiGetSubNodeValue(data_element, "mijingxunbao3_interval", m_mijingxunbao3_interval) || m_mijingxunbao_interval <= 0)
			{
				return -131;
			}

			if (!PugiGetSubNodeValue(data_element, "add_weight_tao3_times", m_add_weight_tao3_times) || m_add_weight_tao_times <= 0)
			{
				return -132;
			}

			if (!PugiGetSubNodeValue(data_element, "mijingxunbao3_thirtytimes_item_id", m_mijingxunbao3_thirtytimes_item_id) || NULL == ITEMPOOL->GetItem(m_mijingxunbao_fifthtimes_item_id))
			{
				return -133;
			}

			if (!PugiGetSubNodeValue(data_element, "mijingxunbao3_free_times", m_mijingxunbao3_free_times) || m_mijingxunbao3_free_times <= 0)
			{
				return -134;
			}

		}

		// 欢乐砸蛋
		{
			if (!PugiGetSubNodeValue(data_element, "huanlezadan_once_gold", m_huanlezadan_once_gold) || m_huanlezadan_once_gold <= 0)
			{
				return -135;
			}

			if (!PugiGetSubNodeValue(data_element, "huanlezadan_tentimes_gold", m_huanlezadan_tentimes_gold) || m_huanlezadan_tentimes_gold <= 0)
			{
				return -136;
			}

			if (!PugiGetSubNodeValue(data_element, "huanlezadan_thirtytimes_gold", m_huanlezadan_thirtytimes_gold) || m_huanlezadan_thirtytimes_gold <= 0)
			{
				return -137;
			}

			if (!PugiGetSubNodeValue(data_element, "huanlezadan_interval", m_huanlezadan_interval) || m_huanlezadan_interval <= 0)
			{
				return -138;
			}

			if (!PugiGetSubNodeValue(data_element, "add_huanlezadan_weight_tao_times", m_huanlezadan_add_weight_tao_times) || m_huanlezadan_add_weight_tao_times <= 0)
			{
				return -139;
			}

			if (!PugiGetSubNodeValue(data_element, "huanlezadan_thirtytimes_item_id", m_huanlezadan_thirtytimes_item_id) || NULL == ITEMPOOL->GetItem(m_huanlezadan_thirtytimes_item_id))
			{
				return -140;
			}

			if (!PugiGetSubNodeValue(data_element, "huanlezadan_free_times", m_huanlezadan_free_times) || m_huanlezadan_free_times <= 0)
			{
				return -141;
			}
		}

		// 欢乐摇奖
		{
			if (!PugiGetSubNodeValue(data_element, "huanleyaojiang_once_gold", m_huanleyaojiang_once_gold) || m_huanleyaojiang_once_gold <= 0)
			{
				return -142;
			}

			if (!PugiGetSubNodeValue(data_element, "huanleyaojiang_tentimes_gold", m_huanleyaojiang_tentimes_gold) || m_huanleyaojiang_tentimes_gold <= 0)
			{
				return -143;
			}

			if (!PugiGetSubNodeValue(data_element, "huanleyaojiang_thirtytimes_gold", m_huanleyaojiang_thirtytimes_gold) || m_huanleyaojiang_thirtytimes_gold <= 0)
			{
				return -144;
			}

			if (!PugiGetSubNodeValue(data_element, "huanleyaojiang_interval", m_huanleyaojiang_interval) || m_huanleyaojiang_interval <= 0)
			{
				return -145;
			}

			if (!PugiGetSubNodeValue(data_element, "huanleyaojiang_add_weight_tao_times", m_huanleyaojiang_add_weight_tao_times) || m_huanleyaojiang_add_weight_tao_times <= 0)
			{
				return -146;
			}

			if (!PugiGetSubNodeValue(data_element, "huanleyaojaing_thirtytimes_item_id", m_huanleyaojaing_thirtytimes_item_id) || NULL == ITEMPOOL->GetItem(m_huanleyaojaing_thirtytimes_item_id))
			{
				return -147;
			}

			if (!PugiGetSubNodeValue(data_element, "huanleyaojiang_free_times", m_huanleyaojiang_free_times) || m_huanleyaojiang_free_times <= 0)
			{
				return -148;
			}

		}

		{
			//循环充值2
			if (!PugiGetSubNodeValue(data_element, "circulation_chongzhi_2_need_chongzhi", m_circulation_chongzhi_2_need_chongzhi) || m_circulation_chongzhi_2_need_chongzhi <= 0)
			{
				return -149;
			}
		}

		// 欢乐摇奖2
		{
			if (!PugiGetSubNodeValue(data_element, "huanleyaojiang2_once_gold", m_huanleyaojiang_2_once_gold) || m_huanleyaojiang_2_once_gold <= 0)
			{
				return -150;
			}

			if (!PugiGetSubNodeValue(data_element, "huanleyaojiang2_tentimes_gold", m_huanleyaojiang_2_tentimes_gold) || m_huanleyaojiang_2_tentimes_gold <= 0)
			{
				return -151;
			}

			if (!PugiGetSubNodeValue(data_element, "huanleyaojiang2_thirtytimes_gold", m_huanleyaojiang_2_thirtytimes_gold) || m_huanleyaojiang_2_thirtytimes_gold <= 0)
			{
				return -152;
			}

			if (!PugiGetSubNodeValue(data_element, "huanleyaojiang2_interval", m_huanleyaojiang_2_interval) || m_huanleyaojiang_2_interval <= 0)
			{
				return -153;
			}

			if (!PugiGetSubNodeValue(data_element, "huanleyaojiang2_add_weight_tao_times", m_huanleyaojiang_2_add_weight_tao_times) || m_huanleyaojiang_2_add_weight_tao_times <= 0)
			{
				return -154;
			}

			if (!PugiGetSubNodeValue(data_element, "huanleyaojaing2_thirtytimes_item_id", m_huanleyaojaing_2_thirtytimes_item_id) || NULL == ITEMPOOL->GetItem(m_huanleyaojaing_2_thirtytimes_item_id))
			{
				return -155;
			}

			if (!PugiGetSubNodeValue(data_element, "huanleyaojiang2_free_times", m_huanleyaojiang_2_free_times) || m_huanleyaojiang_2_free_times <= 0)
			{
				return -156;
			}

			{// 极限挑战

				if (!PugiGetSubNodeValue(data_element, "extreme_challenge_rand_task_num", m_extreme_challenge_rand_give_role_task_num) || m_extreme_challenge_rand_give_role_task_num <= 0)
				{
					return -157;
				}

				if (!PugiGetSubNodeValue(data_element, "extreme_challenge_refresh_task_need_gold", m_extreme_challenge_refresh_task_need_gold) || m_extreme_challenge_refresh_task_need_gold <= 0)
				{
					return -158;
				}

				PugiXmlNode reward_item_list = data_element.child("extreme_challenge_finish_all_task_reward_list");
				if (reward_item_list.empty())
				{
					return -159;
				}

				PugiXmlNode reward_item = reward_item_list.child("extreme_challenge_finish_all_task_reward");
				if (reward_item_list.empty())
				{
					return -160;
				}

				int i = 0;

				while (!reward_item.empty())
				{
					if (i >= MAX_ATTACHMENT_ITEM_NUM)
					{
						return -162;
					}

					if (!m_extreme_challenge_finish_all_task_reward_cfg[i].ReadConfig(reward_item))
					{
						return -161;
					}

					++i;
					reward_item = reward_item.next_sibling();
				}
			}
		}

		// 节日守护
		{
			PugiXmlNode reward_item = data_element.child("holiday_guard_participation_reward");
			if (reward_item.empty())
			{
				return -157;
			}

			if (!m_holiday_guard_participation_reward.ReadConfig(reward_item))
			{
				return -158;
			}
		}

		// 周末狂欢
		{
			if (!PugiGetSubNodeValue(data_element, "lottery1_consume_gold", m_lottery1_other_cfg.lottery1_consume_gold) || m_lottery1_other_cfg.lottery1_consume_gold <= 0)
			{
				return -162;
			}

			if (!PugiGetSubNodeValue(data_element, "lottery1_free_interval_s", m_lottery1_other_cfg.lottery1_free_interval_s) || m_lottery1_other_cfg.lottery1_free_interval_s <= 0)
			{
				return -163;
			}

			if (!PugiGetSubNodeValue(data_element, "lottery1_one_consume_item", m_lottery1_other_cfg.lottery1_one_consume_item) || m_lottery1_other_cfg.lottery1_one_consume_item <= 0 ||
				NULL == ITEMPOOL->GetItem(m_lottery1_other_cfg.lottery1_one_consume_item))
			{
				return -164;
			}

			if (!PugiGetSubNodeValue(data_element, "lottery1_ten_consume_gold", m_lottery1_other_cfg.lottery1_ten_consume_gold) || m_lottery1_other_cfg.lottery1_ten_consume_gold <= 0)
			{
				return -165;
			}

			if (!PugiGetSubNodeValue(data_element, "lottery1_ten_consume_item", m_lottery1_other_cfg.lottery1_ten_consume_item) || m_lottery1_other_cfg.lottery1_ten_consume_item <= 0 ||
				NULL == ITEMPOOL->GetItem(m_lottery1_other_cfg.lottery1_ten_consume_item))
			{
				return -166;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "lucky_wish_30_times_use_item", m_lucky_wish_30_times_use_item) || NULL == ITEMPOOL->GetItem(m_lucky_wish_30_times_use_item))
		{
			return -167;
		}

		data_element = data_element.next_sibling();
	}

	// 加强配置检查 -- 幸运转盘检查
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (m_luckyroll_cfg[stage_num].cfg.unit_roll_consume_gold <= 0)
		{
			return -100001;
		}

		if (m_login_gift_cfg[stage_num].need_accumulate_days <= 0)
		{
			return -100002;
		}
	}

	return 0;
}

int RandActivityConfig::InitLuckyRollCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int m_reward_count = 0;
	int last_opengame_day = 0;
	int opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) ||  opengame_day <= 0)
		{
			return -100;
		}

		if (last_opengame_day != opengame_day)
		{
			++ stage_num;
			m_reward_count = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (m_reward_count >= RAND_ACTIVITY_LUCKROOL_REWARD_CONFIG_MAX_COUNT)
		{
			break;
		}

		m_luckyroll_cfg[stage_num].opengame_day = opengame_day;
		RandActivityLuckyRollRewardCfg &cfg_item = m_luckyroll_cfg[stage_num].cfg.reward_cfg_list[m_reward_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_reward_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_type", cfg_item.reward_type) || cfg_item.reward_type <= LUCKY_ROLL_REWARD_TYPE_INVALID || cfg_item.reward_type >= LUCKY_ROLL_REWARD_TYPE_MAX)
		{
			return -2;
		}

		int is_broadcast = 0;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast))
		{
			return -3;
		}
		cfg_item.is_broadcast = (0 != is_broadcast);

		if (!PugiGetSubNodeValue(data_element, "weight", cfg_item.weight) || cfg_item.weight <= 0)
		{
			return -4;
		}
		
		if (!PugiGetSubNodeValue(data_element, "gold_percent", cfg_item.gold_percent) || cfg_item.gold_percent < 0 || cfg_item.gold_percent >= 100)
		{
			return -5;
		}

		if (LUCKY_ROLL_REWARD_TYPE_GOLD == cfg_item.reward_type && cfg_item.gold_percent <= 0)
		{
			return -6;
		}
		
		if (LUCKY_ROLL_REWARD_TYPE_ITEM == cfg_item.reward_type)
		{
			PugiXmlNode reward_item = data_element.child("reward_item");
			if (reward_item.empty())
			{
				return -7;
			}

			if (!cfg_item.reward_item.ReadConfig(reward_item))
			{
				return -8;
			}
		}

		++ m_reward_count;
		++ m_luckyroll_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	{
		if (m_luckyroll_cfg[stage_num].cfg.replace_gold_reward_seq >= RAND_ACTIVITY_LUCKROOL_REWARD_CONFIG_MAX_COUNT)
		{
			return -20000;
		}

		if (LUCKY_ROLL_REWARD_TYPE_ITEM != m_luckyroll_cfg[stage_num].cfg.reward_cfg_list[m_luckyroll_cfg[stage_num].cfg.replace_gold_reward_seq].reward_type)
		{
			return -20001;
		}
	}

	return 0;
}

int RandActivityConfig::InitDayChongZhiFanLiCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}
	
	int last_need_gold = 0;
	int opengame_day = 0;
	int last_opengame_day =0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}
		if (opengame_day != last_opengame_day)
		{
			++ stage_num;
			last_need_gold = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		m_day_chongzhi_fanli_cfg[stage_num].opengame_day = opengame_day;

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq >= RAND_ACTIVITY_DAY_CHONGZHI_FANLI_REWARD_CONFIG_MAX_COUNT || seq != m_day_chongzhi_fanli_cfg[stage_num].cfg_count)
		{
			return -1;
		}

		RandActivityDayChongZhiFanLiCfg::ConfigItem &cfg_item = m_day_chongzhi_fanli_cfg[stage_num].cfg_list[m_day_chongzhi_fanli_cfg[stage_num].cfg_count];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "need_gold", cfg_item.need_gold) || cfg_item.need_gold <= last_need_gold)
		{
			return -2;
		}
		last_need_gold = cfg_item.need_gold;

		int valid_reward_count = 0;

		if (!PugiGetSubNodeValue(data_element, "reward_gold", cfg_item.reward_gold) || cfg_item.reward_gold < 0)
		{
			return -3;
		}
		if (cfg_item.reward_gold > 0)
		{
			++ valid_reward_count;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_bind_gold", cfg_item.reward_bind_gold) || cfg_item.reward_bind_gold < 0)
		{
			return -4;
		}
		if (cfg_item.reward_bind_gold > 0)
		{
			++ valid_reward_count;
		}
		
		PugiXmlNode reward_item = data_element.child("reward_item");
		if (!reward_item.empty())
		{
			if (cfg_item.reward_item.ReadConfig(reward_item))
			{
				++ valid_reward_count;
			}
			else
			{
				cfg_item.reward_item.item_id = 0;
			}
		}

		if (1 != valid_reward_count)
		{
			return -20000;
		}
		
		++ m_day_chongzhi_fanli_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitDayConsumeGoldCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_need_consume_gold = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++ stage_num;
			last_need_consume_gold = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}
		m_day_consume_gold_cfg[stage_num].opengame_day = opengame_day;

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq >= RAND_ACTIVITY_DAY_CONSUME_GOLD_REWARD_CONFIG_MAX_COUNT || seq != m_day_consume_gold_cfg[stage_num].cfg_count)
		{
			return -1;
		}

		RandActivityDayConsumeGoldCfg::ConfigItem &cfg_item = m_day_consume_gold_cfg[stage_num].cfg_list[m_day_consume_gold_cfg[stage_num].cfg_count];
		cfg_item.seq = seq;
		
		if (!PugiGetSubNodeValue(data_element, "need_consume_gold", cfg_item.need_consume_gold) || cfg_item.need_consume_gold <= last_need_consume_gold)
		{
			return -2;
		}
		last_need_consume_gold = cfg_item.need_consume_gold;
	
		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -3;
		}

		if (!cfg_item.reward_item.ReadConfig(reward_item))
		{
			return -4;
		}

		++ m_day_consume_gold_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitServerPanicBuyCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		int seq = 0;

		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++ stage_num;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 ||  stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		m_server_panic_buy_cfg[stage_num].opengame_day = opengame_day;

		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_server_panic_buy_cfg[stage_num].cfg_count || seq >= RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT)
		{
			return -1;
		}

		RandActivityServerPanicBuyCfg::ConfigItem &cfg_item = m_server_panic_buy_cfg[stage_num].cfg_list[m_server_panic_buy_cfg[stage_num].cfg_count];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "gold_price", cfg_item.gold_price) || cfg_item.gold_price <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "server_limit_buy_count", cfg_item.server_limit_buy_count) || cfg_item.server_limit_buy_count <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "personal_limit_buy_count", cfg_item.personal_limit_buy_count) || cfg_item.personal_limit_buy_count <= 0)
		{
			return -4;
		}

		PugiXmlNode rewardItemElement = data_element.child("reward_item");
		if (rewardItemElement.empty())
		{
			return -5;
		}

		if (!cfg_item.reward_item.ReadConfig(rewardItemElement))
		{
			return -6;
		}

		++ m_server_panic_buy_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitPersonalPanicBuyCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int opengame_day = -1;
	int last_opengame_day = -1;
	int stage_num = -1;

	while (!data_element.empty())
	{
		
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++ stage_num;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		m_personal_panic_buy_cfg[stage_num].opengame_day = opengame_day;

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_personal_panic_buy_cfg[stage_num].cfg_count || seq >= RAND_ACTIVITY_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT)
		{
			return -1;
		}

		RandActivityPersonalPanicBuyCfg::ConfigItem &cfg_item = m_personal_panic_buy_cfg[stage_num].cfg_list[m_personal_panic_buy_cfg[stage_num].cfg_count];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "gold_price", cfg_item.gold_price) || cfg_item.gold_price <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "limit_buy_count", cfg_item.limit_buy_count) || cfg_item.limit_buy_count <= 0)
		{
			return -3;
		}

		PugiXmlNode rewardItemElement = data_element.child("reward_item");
		if (rewardItemElement.empty())
		{
			return -4;
		}

		if (!cfg_item.reward_item.ReadConfig(rewardItemElement))
		{
			return -5;
		}

		++ m_personal_panic_buy_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitChongzhiRankRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_rank_index = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{

		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++ stage_num;
			last_rank_index = -1;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}
		m_chongzhi_rank_reward_cfg[stage_num].opengame_day = opengame_day;

		if (m_chongzhi_rank_reward_cfg[stage_num].cfg_count >= RAND_ACTIVITY_CHONGZHI_RANK_REWARD_CONFIG_MAX_COUNT)
		{
			return -300;
		}

		RandActivityChongzhiRankCfg::ConfigItem &cfg_item = m_chongzhi_rank_reward_cfg[stage_num].cfg_list[m_chongzhi_rank_reward_cfg[stage_num].cfg_count];

		if (!PugiGetSubNodeValue(data_element, "rank_index", cfg_item.rank) || cfg_item.rank <= last_rank_index)
		{
			return -1;
		}
		last_rank_index = cfg_item.rank;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -2;
		}

		if (!cfg_item.item.ReadConfig(reward_item))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "limit_chongzhi", cfg_item.limit_chongzhi) || cfg_item.limit_chongzhi < 0)
		{
			return -4;
		}

		++ m_chongzhi_rank_reward_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitChongzhiRank2RewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_rank_index = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{

		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++stage_num;
			last_rank_index = -1;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}
		m_chongzhi_rank2_reward_cfg[stage_num].opengame_day = opengame_day;

		if (m_chongzhi_rank2_reward_cfg[stage_num].cfg_count >= RAND_ACTIVITY_CHONGZHI_RANK_REWARD_CONFIG_MAX_COUNT)
		{
			return -300;
		}

		RandActivityChongzhiRankCfg::ConfigItem &cfg_item = m_chongzhi_rank2_reward_cfg[stage_num].cfg_list[m_chongzhi_rank2_reward_cfg[stage_num].cfg_count];

		if (!PugiGetSubNodeValue(data_element, "rank_index", cfg_item.rank) || cfg_item.rank <= last_rank_index)
		{
			return -1;
		}
		last_rank_index = cfg_item.rank;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -2;
		}

		if (!cfg_item.item.ReadConfig(reward_item))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "limit_chongzhi", cfg_item.limit_chongzhi) || cfg_item.limit_chongzhi < 0)
		{
			return -4;
		}

		++m_chongzhi_rank2_reward_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitConsumeGoldRankRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_rank_index = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{

		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++ stage_num;
			last_rank_index = -1;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}
		m_consume_gold_rand_cfg[stage_num].opengame_day = opengame_day;

		if (m_consume_gold_rand_cfg[stage_num].cfg_count >= RAND_ACTIVITY_CONSUME_GOLD_RANK_REWARD_CONFIG_MAX_COUNT)
		{
			return -300;
		}

		RandActivityConsumeGoldRankCfg::ConfigItem &cfg_item = m_consume_gold_rand_cfg[stage_num].cfg_list[m_consume_gold_rand_cfg[stage_num].cfg_count];

		if (!PugiGetSubNodeValue(data_element, "rank_index", cfg_item.rank) || cfg_item.rank <= last_rank_index)
		{
			return -1;
		}
		last_rank_index = cfg_item.rank;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -2;
		}

		if (!cfg_item.item.ReadConfig(reward_item))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "limit_comsume", cfg_item.limit_comsume) || cfg_item.limit_comsume < 0)
		{
			return -4;
		}

		++ m_consume_gold_rand_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitEquipStrengthenRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_strengthen_reward_cfg.cfg_count = 0;
	int last_level = 0;

	while (!data_element.empty())
	{
		if (m_strengthen_reward_cfg.cfg_count >= RAND_ACTIVITY_EQUIP_STRENGTHEN_REWARD_CONFIG_MAX_COUNT)
		{
			return -100;
		}

		RandActivityEquipStrengthenCfg::ConfigItem &cfg_item = m_strengthen_reward_cfg.cfg_list[m_strengthen_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "level", cfg_item.level) || cfg_item.level <= last_level)
		{
			return -1;
		}
		last_level = cfg_item.level;

		PugiXmlNode reward_item_list_element = data_element.child("reward_item_list");
		if (reward_item_list_element.empty())
		{
			return -2;
		}

		PugiXmlNode reward_item_element = reward_item_list_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -3;
		}

		cfg_item.item_count = 0;

		while (!reward_item_element.empty())
		{
			if (cfg_item.item_count >= RAND_ACTIVITY_EQUIP_STRENGTHEN_REWARD_ITEM_MAX_COUNT)
			{
				return -4;
			}

			if (!cfg_item.item_list[cfg_item.item_count].ReadConfig(reward_item_element))
			{
				return -5;
			}

			++ cfg_item.item_count;
			reward_item_element = reward_item_element.next_sibling();
		}

		++ m_strengthen_reward_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitChestshopRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_chestshop_reward_cfg.cfg_count = 0;

	int last_need_chestshop_times = 0;

	while (!data_element.empty())
	{
		if (m_chestshop_reward_cfg.cfg_count >= RAND_ACTIVITY_CHESTSHOP_REWARD_CONFIG_MAX_COUNT)
		{
			return -100;
		}

		RandActivityChestShopCfg::ConfigItem &cfg_item = m_chestshop_reward_cfg.cfg_list[m_chestshop_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_chestshop_reward_cfg.cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "need_chestshop_times", cfg_item.need_chestshop_times) || cfg_item.need_chestshop_times <= last_need_chestshop_times)
		{
			return -2;
		}
		last_need_chestshop_times = cfg_item.need_chestshop_times;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -3;
		}

		if (!cfg_item.reward_item.ReadConfig(reward_item))
		{
			return -4;
		}

		++ m_chestshop_reward_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitTotalConsumeGoldRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_need_consume_gold = 0;
	int last_opengame_day = 0;
	int diff_num = -1;

	while (!data_element.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < last_opengame_day || opengame_day <= 0)
		{
			return -100;
		}

		if (diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (last_opengame_day != opengame_day)
		{
			++ diff_num;
			last_opengame_day = opengame_day;
			last_need_consume_gold = 0;
		}

		RandActivityTotalConsumeGoldCfg &total_consume_gold_reward_cfg = m_diff_total_consume_gold_reward_list[diff_num];
		total_consume_gold_reward_cfg.opengame_day = opengame_day;

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq >= RAND_ACTIVITY_TOTAL_CONSUME_GOLD_REWARD_CONFIG_MAX_COUNT || seq != total_consume_gold_reward_cfg.cfg_count)
		{
			return -1;
		}

		RandActivityTotalConsumeGoldCfg::ConfigItem &cfg_item = total_consume_gold_reward_cfg.cfg_list[total_consume_gold_reward_cfg.cfg_count];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "need_consume_gold", cfg_item.need_consume_gold) || cfg_item.need_consume_gold <= last_need_consume_gold)
		{
			return -2;
		}
		last_need_consume_gold = cfg_item.need_consume_gold;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -3;
		}

		if (!cfg_item.reward_item.ReadConfig(reward_item))
		{
			return -4;
		}

		++ total_consume_gold_reward_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}
	return 0;
}

int RandActivityConfig::InitDayActiveDegreeRewardCfg(PugiXmlNode RootElement)
{

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_need_active = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (last_opengame_day != opengame_day)
		{
			++ stage_num;
			last_need_active= 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num > MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}
		m_day_active_degree_reward_cfg[stage_num].opengame_day = opengame_day;

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq >= RAND_ACTIVITY_DAY_ACTIVE_DEGREE_REWARD_CONFIG_MAX_COUNT || seq != m_day_active_degree_reward_cfg[stage_num].cfg_count)
		{
			return -1;
		}

		RandActivityDayActiveDegreeCfg::ConfigItem &cfg_item = m_day_active_degree_reward_cfg[stage_num].cfg_list[m_day_active_degree_reward_cfg[stage_num].cfg_count];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "need_active", cfg_item.need_active) || cfg_item.need_active <= last_need_active)
		{
			return -2;
		}
		last_need_active = cfg_item.need_active;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -3;
		}

		if (!cfg_item.reward_item.ReadConfig(reward_item))
		{
			return -4;
		}

		++ m_day_active_degree_reward_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}
	return 0;
}

int RandActivityConfig::InitStoneUplevelRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_stone_uplevel_cfg.cfg_count = 0;
	
	int last_need_stone_total_level = 0;

	while (!data_element.empty())
	{
		if (m_stone_uplevel_cfg.cfg_count >= RAND_ACTIVITY_STONE_UPLEVEL_REWARD_CONFIG_MAX_COUNT)
		{
			return -100;
		}

		RandActivityStoneUplevelCfg::ConfigItem &cfg_item = m_stone_uplevel_cfg.cfg_list[m_stone_uplevel_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_stone_uplevel_cfg.cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "need_total_stone_level", cfg_item.need_total_stone_level) || cfg_item.need_total_stone_level <= last_need_stone_total_level)
		{
			return -2;
		}
		last_need_stone_total_level = cfg_item.need_total_stone_level;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -3;
		}

		if (!cfg_item.reward_item.ReadConfig(reward_item))
		{
			return -4;
		}

		++ m_stone_uplevel_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitXiannvChanmianUplevelRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_xiannv_chanmian_uplevel_cfg.cfg_count = 0;

	int last_need_chanmian_level = 0;

	while (!data_element.empty())
	{
		if (m_xiannv_chanmian_uplevel_cfg.cfg_count >= RAND_ACTIVITY_XIANNV_CHANMIAN_UPLEVEL_REWARD_CONFIG_MAX_COUNT)
		{
			return -100;
		}

		RandActivityXiannvChanmianUplevelCfg::ConfigItem &cfg_item = m_xiannv_chanmian_uplevel_cfg.cfg_list[m_xiannv_chanmian_uplevel_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_xiannv_chanmian_uplevel_cfg.cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "need_chanmian_level", cfg_item.need_chanmian_level) || cfg_item.need_chanmian_level <= last_need_chanmian_level)
		{
			return -2;
		}
		last_need_chanmian_level = cfg_item.need_chanmian_level;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -3;
		}

		if (!cfg_item.reward_item.ReadConfig(reward_item))
		{
			return -4;
		}

		++ m_xiannv_chanmian_uplevel_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitMountUpgradeRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_mount_upgrade_cfg.cfg_count = 0;

	int last_need_mount_grade = 0;

	while (!data_element.empty())
	{
		if (m_mount_upgrade_cfg.cfg_count >= RAND_ACTIVITY_MOUNT_UPGRADE_REWARD_CONFIG_MAX_COUNT)
		{
			return -100;
		}

		RandActivityMountUpgradeCfg::ConfigItem &cfg_item = m_mount_upgrade_cfg.cfg_list[m_mount_upgrade_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_mount_upgrade_cfg.cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "need_mount_grade", cfg_item.need_mount_grade) || cfg_item.need_mount_grade <= last_need_mount_grade)
		{
			return -2;
		}
		last_need_mount_grade = cfg_item.need_mount_grade;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -3;
		}

		if (!cfg_item.reward_item.ReadConfig(reward_item))
		{
			return -4;
		}

		++ m_mount_upgrade_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitQibingUpgradeRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_qibing_upgrade_cfg.cfg_count = 0;

	int last_need_qibing_grade = 0;

	while (!data_element.empty())
	{
		if (m_qibing_upgrade_cfg.cfg_count >= RAND_ACTIVITY_QIBING_UPGRADE_REWARD_CONFIG_MAX_COUNT)
		{
			return -100;
		}

		RandActivityQibingUpgradeCfg::ConfigItem &cfg_item = m_qibing_upgrade_cfg.cfg_list[m_qibing_upgrade_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_qibing_upgrade_cfg.cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "need_qibing_grade", cfg_item.need_qibing_grade) || cfg_item.need_qibing_grade <= last_need_qibing_grade)
		{
			return -2;
		}
		last_need_qibing_grade = cfg_item.need_qibing_grade;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -3;
		}

		if (!cfg_item.reward_item.ReadConfig(reward_item))
		{
			return -4;
		}

		++ m_qibing_upgrade_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitMentalityTotalLevelRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_mentality_total_level_cfg.cfg_count = 0;

	int last_need_mentality_total_level = 0;

	while (!data_element.empty())
	{
		if (m_mentality_total_level_cfg.cfg_count >= RAND_ACTIVITY_MENTALITY_TOTAL_LEVEL_REWARD_CONFIG_MAX_COUNT)
		{
			return -100;
		}

		RandActivityMentalityTotalLevelCfg::ConfigItem &cfg_item = m_mentality_total_level_cfg.cfg_list[m_mentality_total_level_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_mentality_total_level_cfg.cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "need_mentality_total_level", cfg_item.need_mentality_total_level) || cfg_item.need_mentality_total_level <= last_need_mentality_total_level)
		{
			return -2;
		}
		last_need_mentality_total_level = cfg_item.need_mentality_total_level;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -3;
		}

		if (!cfg_item.reward_item.ReadConfig(reward_item))
		{
			return -4;
		}

		++ m_mentality_total_level_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitWingUpgradeRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_wing_upgrade_cfg.cfg_count = 0;

	int last_need_wing_grade = 0;

	while (!data_element.empty())
	{
		if (m_wing_upgrade_cfg.cfg_count >= RAND_ACTIVITY_WING_UPGRADE_REWARD_CONFIG_MAX_COUNT)
		{
			return -100;
		}

		RandActivityWingUpgradeCfg::ConfigItem &cfg_item = m_wing_upgrade_cfg.cfg_list[m_wing_upgrade_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_wing_upgrade_cfg.cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "need_wing_grade", cfg_item.need_wing_grade) || cfg_item.need_wing_grade <= last_need_wing_grade)
		{
			return -2;
		}
		last_need_wing_grade = cfg_item.need_wing_grade;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -3;
		}

		if (!cfg_item.reward_item.ReadConfig(reward_item))
		{
			return -4;
		}

		++ m_wing_upgrade_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitDayOnlineGiftCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_send_reward_hour = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++ stage_num;
			last_send_reward_hour = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}
		m_day_online_gift_cfg[stage_num].opengame_day = opengame_day;

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq >= RAND_ACTIVITY_DAY_ONLINE_GIFT_CONFIG_MAX_COUNT || seq != m_day_online_gift_cfg[stage_num].cfg_count)
		{
			return -1;
		}

		RandActivityDayOnlineGiftCfg::ConfigItem &cfg_item = m_day_online_gift_cfg[stage_num].cfg_list[m_day_online_gift_cfg[stage_num].cfg_count];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "send_reward_hour", cfg_item.send_reward_hour) || cfg_item.send_reward_hour <= last_send_reward_hour)
		{
			return -2;
		}
		last_send_reward_hour = cfg_item.send_reward_hour;

		++ m_day_online_gift_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitQuanminQifuRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_quanmin_qifu_cfg.cfg_count = 0;

	int last_need_qifu_times = 0;

	while (!data_element.empty())
	{
		if (m_quanmin_qifu_cfg.cfg_count >= RAND_ACTIVITY_VIP_QIFU_REWARD_CONFIG_MAX_COUNT)
		{
			return -100;
		}

		RandActivityQuanminQifuCfg::ConfigItem &cfg_item = m_quanmin_qifu_cfg.cfg_list[m_quanmin_qifu_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_quanmin_qifu_cfg.cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "need_qifu_times", cfg_item.need_qifu_times) || cfg_item.need_qifu_times <= last_need_qifu_times)
		{
			return -2;
		}
		last_need_qifu_times = cfg_item.need_qifu_times;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -3;
		}

		if (!cfg_item.reward_item.ReadConfig(reward_item))
		{
			return -4;
		}

		++ m_quanmin_qifu_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitXianmengBipinRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_xianmeng_bipin_cfg.cfg_count = 0;

	int last_need_rank = -1;

	while (!data_element.empty())
	{
		if (m_xianmeng_bipin_cfg.cfg_count >= RAND_ACTIVITY_XIANMENG_BIPIN_REWARD_CONFIG_MAX_COUNT)
		{
			return -1;
		}

		RandActivityXianmengBipinCfg::ConfigItem &cfg_item = m_xianmeng_bipin_cfg.cfg_list[m_xianmeng_bipin_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "need_rank", cfg_item.need_rank) || cfg_item.need_rank <= last_need_rank)
		{
			return -2;
		}
		last_need_rank = cfg_item.need_rank;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -3;
		}

		if (!cfg_item.reward_item.ReadConfig(reward_item))
		{
			return -4;
		}

		++ m_xianmeng_bipin_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitXianmengJueQiRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_xianmeng_jueqi_cfg.cfg_count = 0;

	int last_need_rank = -1;

	while (!data_element.empty())
	{
		if (m_xianmeng_jueqi_cfg.cfg_count >= RAND_ACTIVITY_XIANMENG_JUEQI_REWARD_CONFIG_MAX_COUNT)
		{
			return -1;
		}

		RandActivityXianmengJueQiCfg::ConfigItem &cfg_item = m_xianmeng_jueqi_cfg.cfg_list[m_xianmeng_jueqi_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "need_rank", cfg_item.need_rank) || cfg_item.need_rank <= last_need_rank)
		{
			return -2;
		}
		last_need_rank = cfg_item.need_rank;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -3;
		}

		if (!cfg_item.reward_item.ReadConfig(reward_item))
		{
			return -4;
		}

		++ m_xianmeng_jueqi_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitLoginGiftRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_need_login_days = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++ stage_num;
			last_need_login_days = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}
		m_login_gift_cfg[stage_num].opengame_day = opengame_day;

		if (m_login_gift_cfg[stage_num].cfg_count >= RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT)
		{
			return -1;
		}

		RandActivityLoginGiftCfg::ConfigItem &cfg_item = m_login_gift_cfg[stage_num].cfg_list[m_login_gift_cfg[stage_num].cfg_count];

		if (!PugiGetSubNodeValue(data_element, "need_login_days", cfg_item.need_login_days) || cfg_item.need_login_days <= last_need_login_days)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_login_gift_cfg[stage_num].cfg_count)
		{
			return -3;
		}
		last_need_login_days = cfg_item.need_login_days;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -4;
		}

		if (!cfg_item.common_reward_item.ReadConfig(reward_item))
		{
			return -5;
		}

		PugiXmlNode vip_reward_item = data_element.child("vip_reward_item");
		if (vip_reward_item.empty())
		{
			return -6;
		}

		if (!cfg_item.vip_reward_item.ReadConfig(vip_reward_item))
		{
			return -7;
		}

		++ m_login_gift_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitLoginGiftRewardCfg_0(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_need_login_days = 0;

	while (!data_element.empty())
	{
		if (m_login_gift_cfg_0.cfg_count >= RAND_ACTIVITY_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT)
		{
			return -1;
		}

		RandActivityLoginGiftCfg::ConfigItem &cfg_item = m_login_gift_cfg_0.cfg_list[m_login_gift_cfg_0.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "need_login_days", cfg_item.need_login_days) || cfg_item.need_login_days <= last_need_login_days)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != m_login_gift_cfg_0.cfg_count)
		{
			return -3;
		}
		last_need_login_days = cfg_item.need_login_days;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -4;
		}

		if (!cfg_item.common_reward_item.ReadConfig(reward_item))
		{
			return -5;
		}

		++m_login_gift_cfg_0.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitEverydayNiceGiftCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int opengame_day = 0;

	while (!data_element.empty())
	{

		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (m_everyday_nice_gift_cfg_count < 0 || m_everyday_nice_gift_cfg_count >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		RandActivityEverydayNiceGiftCfg &tmp_cfg = m_everyday_nice_gift_cfg[m_everyday_nice_gift_cfg_count];
		tmp_cfg.opengame_day = opengame_day;

		if (!PugiGetSubNodeValue(data_element, "index", tmp_cfg.everyday_reward.index) || tmp_cfg.everyday_reward.index < 0)
		{
			return -201;
		}

		if (!PugiGetSubNodeValue(data_element, "recharge_gold", tmp_cfg.everyday_reward.recharge_gold) || tmp_cfg.everyday_reward.recharge_gold <= 0)
		{
			return -203;
		}

		for (int i = 0; i < RAND_ACTIVITY_EVERYDAY_NICE_GIFT_MAX_REWARD_COUNT; ++i)
		{
			char node_name[13] = "";
			sprintf(node_name, "reward_item%d", i + 1);

			PugiXmlNode reward_item = data_element.child(node_name);
			if (reward_item.empty())
			{
				return -205;
			}

			if (tmp_cfg.everyday_reward.reward_count >= RAND_ACTIVITY_EVERYDAY_NICE_GIFT_MAX_REWARD_COUNT)
			{
				break;
			}

			if (!tmp_cfg.everyday_reward.reward_item_list[tmp_cfg.everyday_reward.reward_count].ReadConfig(reward_item))
			{
				if (tmp_cfg.everyday_reward.reward_item_list[tmp_cfg.everyday_reward.reward_count].item_id == 0)
				{
					break;
				}
				else
				{
					return -206;
				}
			}

			++tmp_cfg.everyday_reward.reward_count;
		}

		if (tmp_cfg.everyday_reward.reward_count < 1)
		{
			return -207;
		}

		++m_everyday_nice_gift_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitShouYouYuXiangRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_shouyou_yuxiang_cfg.cfg_count = 0;

	int last_need_flower = 0;

	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq >= RAND_ACTIVITY_SHOUYOU_YUXIANG_REWARD_CONFIG_MAX_COUNT || seq != m_shouyou_yuxiang_cfg.cfg_count)
		{
			return -1;
		}

		RandActivityShouYouYuXiangCfg::ConfigItem &cfg_item = m_shouyou_yuxiang_cfg.cfg_list[m_shouyou_yuxiang_cfg.cfg_count];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "need_flower", cfg_item.need_flower) || cfg_item.need_flower <= last_need_flower)
		{
			return -2;
		}
		last_need_flower = cfg_item.need_flower;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -3;
		}

		if (!cfg_item.reward_item.ReadConfig(reward_item))
		{
			return -4;
		}

		++ m_shouyou_yuxiang_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}
	return 0;
}

int RandActivityConfig::InitRankBipinRewordCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_rank_bipin_cfg_count = 0;

	std::set<int> repeat_checker;

	while (!data_element.empty())
	{
		if (m_rank_bipin_cfg_count >= RAND_ACTIVITY_BIPIN_ACTIVITY_MAX_COUNT)
		{
			return -1000;
		}

		RandActivityRankBipinCfg &cfg_item = m_rank_bipin_cfglist[m_rank_bipin_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "activity_type", cfg_item.activity_type) || 
			cfg_item.activity_type < RAND_ACTIVITY_TYPE_BEGIN || cfg_item.activity_type >= RAND_ACTIVITY_TYPE_END)
		{
			return -1;
		}
		
		if (repeat_checker.end() != repeat_checker.find(cfg_item.activity_type))
		{
			return -2;
		}
		repeat_checker.insert(cfg_item.activity_type);

		if (!PugiGetSubNodeValue(data_element, "reward_day", cfg_item.give_reward_day) || cfg_item.give_reward_day <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "need_capability", cfg_item.need_capability) || cfg_item.need_capability <= 0)
		{
			return -4;
		}

		{ 
			// 不区分阵营
			PugiXmlNode reward_item = data_element.child("reward_item");
			if (reward_item.empty())
			{
				return -5;
			}
			if (!cfg_item.reward_item.ReadConfig(reward_item))
			{
				return -6;
			}
		}

		//{ 
		//	// 第二个阵营 - 神农
		//	PugiXmlNode reward_item = data_element.child("camp2_item");
		//	if (NULL == reward_item)
		//	{
		//		return -7;
		//	}
		//	if (!cfg_item.reward_item_list[CAMP_TYPE_ZHURI].ReadConfig(reward_item))
		//	{
		//		return -8;
		//	}
		//}

		//{ 
		//	// 第三个阵营 - 蚩尤
		//	PugiXmlNode reward_item = data_element.child("camp3_item");
		//	if (NULL == reward_item)
		//	{
		//		return -9;
		//	}
		//	if (!cfg_item.reward_item_list[CAMP_TYPE_ZHUIYUE].ReadConfig(reward_item))
		//	{
		//		return -10;
		//	}
		//}

		++ m_rank_bipin_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitAllBipinRewordCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	std::map<int, int> activity_index_map;
	int last_need_capability_list[RAND_ACTIVITY_BIPIN_ACTIVITY_MAX_COUNT]; 
	memset(last_need_capability_list, 0, sizeof(last_need_capability_list));

	while (!data_element.empty())
	{
		int activity_type = 0;
		if (!PugiGetSubNodeValue(data_element, "activity_type", activity_type) ||
			activity_type < RAND_ACTIVITY_TYPE_BEGIN || activity_type >= RAND_ACTIVITY_TYPE_END )
		{
			return -1;
		}

		int locate_idx = -1;
		{
			std::map<int, int>::iterator it = activity_index_map.find(activity_type);
			if (activity_index_map.end() == it)
			{
				if (m_all_bipin_cfg_count >= RAND_ACTIVITY_BIPIN_ACTIVITY_MAX_COUNT)
				{
					return -2;
				}

				locate_idx = m_all_bipin_cfg_count;
				++ m_all_bipin_cfg_count;
				activity_index_map[activity_type] = locate_idx;
			}
			else
			{
				locate_idx = it->second;
			}
		}
		if (locate_idx < 0 || locate_idx >= RAND_ACTIVITY_BIPIN_ACTIVITY_MAX_COUNT)
		{
			return -3;
		}
		
		RandActivityAllBipinCfg *cfg_item = &m_all_bipin_cfglist[locate_idx];
		cfg_item->activity_type = activity_type;

		if (cfg_item->cfg_count >= RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT)
		{
			return -4;
		}

		RandActivityAllBipinCfg::ConfigItem &cfg_sub_item = cfg_item->cfg_list[cfg_item->cfg_count];
		cfg_sub_item.activity_type = activity_type;

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_sub_item.seq) || cfg_sub_item.seq != cfg_item->cfg_count)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "need_capability", cfg_sub_item.need_capability) || cfg_sub_item.need_capability <= 0)
		{
			return -6;
		}

		if (cfg_sub_item.need_capability < last_need_capability_list[locate_idx])
		{
			return -7;
		}
		last_need_capability_list[locate_idx] = cfg_sub_item.need_capability;

		bool has_reward_bin_gold = true;
		// 奖励绑定元宝
		if (!PugiGetSubNodeValue(data_element, "reward_bind_gold", cfg_sub_item.reward_gold_bind))
		{
			has_reward_bin_gold = false;
		}

		bool has_item = false;

		// 奖励物品
		PugiXmlNode reward_item = data_element.child("reward_item");
		if (!reward_item.empty())
		{
			if (cfg_sub_item.reward_item.ReadConfig(reward_item))
			{
				has_item = true;
			}
		}

		if (!has_item)
		{
			cfg_sub_item.reward_item.item_id = 0;
		}

		if (!has_item && !has_reward_bin_gold)
		{
			return -8;
		}

		cfg_item->cfg_count ++;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitChargeRepaymentCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1;
	}

	int last_charge_value = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++ stage_num;
			last_charge_value = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}
		m_charge_repayment_cfg[stage_num].opengame_day = opengame_day;

		if (m_charge_repayment_cfg[stage_num].cfg_count >= RAND_ACTIVITY_CHARGE_REPAYMENT_CONFIG_MAX_COUNT)
		{
			return -2;
		}

		RandActivityChargeRepaymentCfg::ConfigItem *cfg_item = &m_charge_repayment_cfg[stage_num].cfg_list[m_charge_repayment_cfg[stage_num].cfg_count];
		
		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item->seq) || cfg_item->seq != m_charge_repayment_cfg[stage_num].cfg_count)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "charge_value", cfg_item->charge_value) || cfg_item->charge_value <= last_charge_value)
		{
			return -4;
		}
		last_charge_value = cfg_item->charge_value;

		if (!PugiGetSubNodeValue(data_element, "bind_gold_repayment", cfg_item->bind_gold_repayment) || cfg_item->bind_gold_repayment <= 0)
		{
			return -5;
		}

		++ m_charge_repayment_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitSingleChargeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1;
	}

	int last_charge_value = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++ stage_num;
			last_charge_value = 0;
		}
		last_opengame_day =opengame_day;

		if (stage_num < 0 || stage_num >= RAND_ACTIVITY_SINGLE_CHAR_CFG_COUNT)
		{
			return -200;
		}
		m_single_charge_cfg[stage_num].opengame_day = opengame_day;

		if (m_single_charge_cfg[stage_num].cfg_count >= RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT)
		{
			return -2;
		}

		RandActivitySingleChargeCfg::ConfigItem *cfg_item = &m_single_charge_cfg[stage_num].cfg_list[m_single_charge_cfg[stage_num].cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item->seq) || cfg_item->seq != m_single_charge_cfg[stage_num].cfg_count)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "charge_value", cfg_item->charge_value) || cfg_item->charge_value <= last_charge_value)
		{
			return -4;
		}
		last_charge_value = cfg_item->charge_value;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -5;
		}
		if (!cfg_item->reward_item.ReadConfig(reward_item))
		{
			return -6;
		}

		const ChongzhiReward *extra_reward_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetExtraRewardByNum(cfg_item->charge_value);
		if (NULL == extra_reward_cfg)
		{
			return -44;
		}

		++ m_single_charge_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitSingleCharge1Cfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1;
	}

	int last_charge_value = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++stage_num;
			last_charge_value = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= RAND_ACTIVITY_SINGLE_CHAR_CFG_COUNT)
		{
			return -200;
		}
		m_single_charge1_cfg[stage_num].opengame_day = opengame_day;

		if (m_single_charge1_cfg[stage_num].cfg_count >= RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT)
		{
			return -2;
		}

		RandActivitySingleChargeCfg::ConfigItem *cfg_item = &m_single_charge1_cfg[stage_num].cfg_list[m_single_charge1_cfg[stage_num].cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item->seq) || cfg_item->seq != m_single_charge1_cfg[stage_num].cfg_count)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "charge_value", cfg_item->charge_value) || cfg_item->charge_value <= last_charge_value)
		{
			return -4;
		}
		last_charge_value = cfg_item->charge_value;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -5;
		}
		if (!cfg_item->reward_item.ReadConfig(reward_item))
		{
			return -6;
		}

		const ChongzhiReward *extra_reward_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetExtraRewardByNum(cfg_item->charge_value);
		if (NULL == extra_reward_cfg)
		{
			return -44;
		}

		++m_single_charge1_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}
int RandActivityConfig::InitSingleCharge2Cfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1;
	}

	int last_charge_value = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++stage_num;
			last_charge_value = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= RAND_ACTIVITY_SINGLE_CHAR_CFG_COUNT)
		{
			return -200;
		}
		m_single_charge2_cfg[stage_num].opengame_day = opengame_day;

		if (m_single_charge2_cfg[stage_num].cfg_count >= RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT)
		{
			return -2;
		}

		RandActivitySingleChargeCfg::ConfigItem *cfg_item = &m_single_charge2_cfg[stage_num].cfg_list[m_single_charge2_cfg[stage_num].cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item->seq) || cfg_item->seq != m_single_charge2_cfg[stage_num].cfg_count)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "charge_value", cfg_item->charge_value) || cfg_item->charge_value <= last_charge_value)
		{
			return -4;
		}
		last_charge_value = cfg_item->charge_value;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -5;
		}
		if (!cfg_item->reward_item.ReadConfig(reward_item))
		{
			return -6;
		}

		const ChongzhiReward *extra_reward_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetExtraRewardByNum(cfg_item->charge_value);
		if (NULL == extra_reward_cfg)
		{
			return -44;
		}

		++m_single_charge2_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}
int RandActivityConfig::InitSingleCharge3Cfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1;
	}

	int last_charge_value = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++stage_num;
			last_charge_value = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= RAND_ACTIVITY_SINGLE_CHAR_CFG_COUNT)
		{
			return -200;
		}
		m_single_charge3_cfg[stage_num].opengame_day = opengame_day;

		if (m_single_charge3_cfg[stage_num].cfg_count >= RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT)
		{
			return -2;
		}

		RandActivitySingleChargeCfg::ConfigItem *cfg_item = &m_single_charge3_cfg[stage_num].cfg_list[m_single_charge3_cfg[stage_num].cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item->seq) || cfg_item->seq != m_single_charge3_cfg[stage_num].cfg_count)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "charge_value", cfg_item->charge_value) || cfg_item->charge_value <= last_charge_value)
		{
			return -4;
		}
		last_charge_value = cfg_item->charge_value;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -5;
		}
		if (!cfg_item->reward_item.ReadConfig(reward_item))
		{
			return -6;
		}

		const ChongzhiReward *extra_reward_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetExtraRewardByNum(cfg_item->charge_value);
		if (NULL == extra_reward_cfg)
		{
			return -44;
		}

		++m_single_charge3_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}
int RandActivityConfig::InitCornucopiaCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1;
	}

	m_cornucopia_max_lun = 0;

	while (!data_element.empty())
	{
		int lun = 0;
		if (!PugiGetSubNodeValue(data_element, "lun", lun) || lun != m_cornucopia_max_lun + 1)
		{
			return -2;
		}
		m_cornucopia_max_lun = lun;

		if (m_cornucopia_max_lun > RAND_ACTIVITY_CORNUCOPIA_MAX_LUN)
		{
			return -3;
		}

		RandActivityCornucopiaCfg *cfg_item = &m_cornucopia_cfglist[lun];

		if (!PugiGetSubNodeValue(data_element, "need_total_charge", cfg_item->need_total_charge) || cfg_item->need_total_charge <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "need_put_gold", cfg_item->need_put_gold) || cfg_item->need_put_gold <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "min_reward_gold", cfg_item->min_reward_gold) || cfg_item->min_reward_gold < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "max_reward_gold", cfg_item->max_reward_gold) || cfg_item->max_reward_gold < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", cfg_item->is_broadcast))
		{
			return -7;
		}
		
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitDanbiChongzhiCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_opengame_day = 0;
	int diff_num = -1;

	while (!data_element.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < last_opengame_day || opengame_day <= 0)
		{
			return -100;
		}

		if (diff_num >= RAND_ACTIVITY_DANBI_CHONGZHI_DAY_CFG_COUNT)
		{
			return -200;
		}

		if (last_opengame_day != opengame_day)
		{
			++ diff_num;
			last_opengame_day = opengame_day;
		}

		RandActivityDanbiChongzhiDayDiffCfg &diff_cfg = m_diff_danbi_chongzhi_day_cfglist[diff_num];
		diff_cfg.opengame_day = opengame_day;


		int activity_day = 0;
		if (!PugiGetSubNodeValue(data_element, "activity_day", activity_day) || activity_day < 0 || activity_day >= RAND_ACTIVITY_DANBI_CHONGZHI_DAY_CFG_MAX_COUNT)
		{
			return -1;
		}

		if (activity_day > diff_cfg.reward_max_day)
		{
			diff_cfg.reward_max_day = activity_day;
		}

		RandActivityDanbiChongzhiDayCfg &day_cfg = diff_cfg.day_cfglist[activity_day];
		day_cfg.activity_day = activity_day;

		int reward_seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", reward_seq) || reward_seq != day_cfg.reward_count || reward_seq >= RAND_ACTIVITY_DANBI_CHONGZHI_REWARD_CFG_MAX_COUNT_PER_DAY)
		{
			return -2;
		}

		RandActivityDanbiChongzhiRewardCfg &reward_cfg = day_cfg.reward_list[reward_seq];
		reward_cfg.activity_day = activity_day;
		reward_cfg.reward_seq = reward_seq;

		if (!PugiGetSubNodeValue(data_element, "need_chongzhi_num", reward_cfg.need_chongzhi_num))
		{
			return -3;
		}

		const ChongzhiReward *extra_reward_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetExtraRewardByNum(reward_cfg.need_chongzhi_num);
		if (NULL == extra_reward_cfg)
		{
			return -4;
		}

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -5;
		}

		if (!reward_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -6;
		}

		++ day_cfg.reward_count;
		data_element = data_element.next_sibling();
	}
	return 0;
}

int RandActivityConfig::InitTotalChargeDayCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int last_need_charge_day = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day))
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++ stage_num;
			m_total_charge_day_max_seq = -1;
			last_need_charge_day = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		m_total_charge_day_cfglist[stage_num].opengame_day = opengame_day;

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq))
		{
			return -1;
		}

		if (seq != m_total_charge_day_max_seq + 1 || seq > RAND_ACTIVITY_TOTAL_CHARGE_DAY_CFG_MAX_SEQ)
		{
			return -2;
		}

		RandActivityTotalChargeDayCfg::TotalChargeDayItem &cfg = m_total_charge_day_cfglist[stage_num].cfg_list[seq];
		cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "need_recharge_day", cfg.need_recharge_day) || cfg.need_recharge_day < last_need_charge_day)
		{
			return -3;
		}
		last_need_charge_day = cfg.need_recharge_day;

		{
			PugiXmlNode item_list_element = data_element.child("reward_item_list");
			if (item_list_element.empty())
			{
				return -100;
			}
			cfg.reward_count = 0;

			PugiXmlNode item_element = item_list_element.child("reward_item");
			if (item_element.empty())
			{
				return -101;
			}

			while (!item_element.empty())
			{
				if (cfg.reward_count >= RandActivityTotalChargeDayCfg::TotalChargeDayItem::REWARD_ITEM_MAX_COUT)
				{
					return -102;
				}

				if (!cfg.reward_item_list[cfg.reward_count].ReadConfig(item_element))
				{
					return -103;
				}

				++ cfg.reward_count;
				item_element = item_element.next_sibling();
			}
		}

		++ m_total_charge_day_max_seq;
		++ m_total_charge_day_cfglist[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitTomorrowRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	m_tomorrow_reward_cfg_count = 0;
	m_tomorrow_reward_cfg_total_weight = 0;

	while (!data_element.empty())
	{
		if (m_tomorrow_reward_cfg_count >= RAND_ACTIVITY_TOMORROW_REWARD_CFG_MAX_COUNT)
		{
			return -1;
		}
		RandActivityTomorrowRewardCfg &tomorrow_reward_cfg = m_tomorrow_reward_cfglist[m_tomorrow_reward_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "index", tomorrow_reward_cfg.index) || tomorrow_reward_cfg.index != m_tomorrow_reward_cfg_count + 1)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", tomorrow_reward_cfg.weight) || tomorrow_reward_cfg.weight < 0)
		{
			return -3;
		}
		m_tomorrow_reward_cfg_total_weight += tomorrow_reward_cfg.weight;

		int is_broadcast = 0;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast) || is_broadcast < 0)
		{
			return -4;
		}
		tomorrow_reward_cfg.is_broadcast = (0 != is_broadcast);

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -5;
		}

		if (!tomorrow_reward_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -6;
		}

		++ m_tomorrow_reward_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitDayChongzhiRankCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_opengame_day = 0;
	int diff_num = -1;

	while (!data_element.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < last_opengame_day || opengame_day <= 0)
		{
			return -100;
		}

		if (diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (last_opengame_day != opengame_day)
		{
			++ diff_num;
			last_opengame_day = opengame_day;
		}

		RandActivityDayChongzhiRankRewardDiffConfig &daycfg = m_diff_daychongzhirank_reward_list[diff_num];
		daycfg.opengame_day = opengame_day;

		int day;
		if (!PugiGetSubNodeValue(data_element, "activity_day", day) || day <= 0 || day > RAND_ACTIVITY_DAY_CHONGZHI_RANK_REWARD_MAX_DAY)
		{
			return -1;
		}
		if (day > daycfg.reward_max_day)
		{
			daycfg.reward_max_day = day;
		}

		RandActivityDayChongzhiRankRewardConfig &day_cfg = daycfg.rank_reward_cfglist[day];
		if (day_cfg.cfg_count >= RandActivityDayChongzhiRankRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -2;
		}

		RandActivityDayChongzhiRankRewardConfig::ConfigItem &cfg_item = day_cfg.cfg_list[day_cfg.cfg_count];
		cfg_item.day = day;

		if (!PugiGetSubNodeValue(data_element, "rank", cfg_item.limit_rank))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "min_gold", cfg_item.limit_gold) || cfg_item.limit_gold <= 0)
		{
			return -5;
		}

		PugiXmlNode item_list_element = data_element.child("reward_item_list");
		if (item_list_element.empty())
		{
			return -100;
		}

		PugiXmlNode item_element = item_list_element.child("reward_item");
		if (item_element.empty())
		{
			return -101;
		}

		while (item_element)
		{
			if (cfg_item.item_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -102;
			}

			if (!cfg_item.reward_item_list[cfg_item.item_count].ReadConfig(item_element))
			{
				return -103;
			}

			++ cfg_item.item_count;
			item_element = item_element.next_sibling();
		}

		 ++ day_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitDayConsumeRankCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}
	
	int last_opengame_day = 0;
	int diff_num = -1;

	while (!data_element.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < last_opengame_day || opengame_day <= 0)
		{
			return -100;
		}

		if (diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (last_opengame_day != opengame_day)
		{
			++ diff_num;
			last_opengame_day = opengame_day;
		}

		RandActivityDayConsumeRankRewardDiffConfig &daycfg = m_diff_dayconsume_rank_reward_cfglist[diff_num];
		daycfg.opengame_day = opengame_day;

		int day;
		if (!PugiGetSubNodeValue(data_element, "activity_day", day) || day <= 0 || day > RAND_ACTIVITY_DAY_CONSUME_GOLD_REWARD_CONFIG_MAX_COUNT)
		{
			return -1;
		}
		if (day > daycfg.reward_max_day)
		{
			daycfg.reward_max_day = day;
		}

		RandActivityDayConsumeRankRewardConfig &day_cfg = daycfg.rank_reward_cfglist[day];
		if (day_cfg.cfg_count >= RandActivityDayConsumeRankRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -2;
		}

		RandActivityDayConsumeRankRewardConfig::ConfigItem &cfg_item = day_cfg.cfg_list[day_cfg.cfg_count];
		cfg_item.day = day;

		if (!PugiGetSubNodeValue(data_element, "rank", cfg_item.limit_rank))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "min_gold", cfg_item.limit_gold) || cfg_item.limit_gold <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "fanli_rate", cfg_item.fanli_rate) || cfg_item.fanli_rate < 0)
		{
			return -6;
		}

		PugiXmlNode item_list_element = data_element.child("reward_item_list");
		if (item_list_element.empty())
		{
			return -100;
		}

		PugiXmlNode item_element = item_list_element.child("reward_item");
		if (item_element.empty())
		{
			return -101;
		}

		while (item_element)
		{
			if (cfg_item.item_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -102;
			}

			if (!cfg_item.reward_item_list[cfg_item.item_count].ReadConfig(item_element))
			{
				return -103;
			}

			++ cfg_item.item_count;
			item_element = item_element.next_sibling();
		}

		++ day_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitTotalChongzhiCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_opengame_day = 0;
	int last_need_chongzhi = 0;
	int diff_num = -1;

	while (!data_element.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < last_opengame_day || opengame_day <= 0)
		{
			return -100;
		}
		
		if (diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (last_opengame_day != opengame_day)
		{
			++ diff_num;
			last_opengame_day = opengame_day;
			last_need_chongzhi = 0;
		}

		RandActivityTotalChargeRewardConfig &total_chognzhi_reward_cfg = m_diff_totalcharge_reward_list[diff_num];
		total_chognzhi_reward_cfg.opengame_day = opengame_day;
		
		if (total_chognzhi_reward_cfg.cfg_count >= RandActivityTotalChargeRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -30000;
		}

		RandActivityTotalChargeRewardConfig::ConfigItem &cfg_item = total_chognzhi_reward_cfg.cfg_list[total_chognzhi_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != total_chognzhi_reward_cfg.cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "need_chognzhi", cfg_item.need_chongzhi) || cfg_item.need_chongzhi <= last_need_chongzhi)
		{
			return -2;
		}
		last_need_chongzhi = cfg_item.need_chongzhi;

		{
			// 读物品奖励
			PugiXmlNode item_list_element = data_element.child("reward_item_list");
			if (item_list_element.empty())
			{
				return -1001;
			}

			PugiXmlNode item_element = item_list_element.child("reward_item");
			if (item_element.empty())
			{
				return -1002;
			}

			cfg_item.item_count = 0;

			while (!item_element.empty())
			{
				if (cfg_item.item_count >= RandActivityTotalChargeRewardConfig::REWARD_ITEM_MAX_COUNT)
				{
					return -1003;
				}

				if (!cfg_item.item_list[cfg_item.item_count].ReadConfig(item_element))
				{
					return -1004;
				}

				++ cfg_item.item_count;
				item_element = item_element.next_sibling();
			}
		}

		++ total_chognzhi_reward_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitTimeLimitExchangeEquiCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_seq = -1;

	while (!data_element.empty())
	{
		int seq = -1;

		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= TIME_LIMIT_EXCHANGE_ITEM_COUNT || seq != last_seq + 1)
		{
			return -100;
		}
		last_seq = seq;

		RandActivityTimeLimitExchangeConfig &cfg_item = m_time_limit_exchange_equi_list[seq];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "need_score", cfg_item.need_score) || cfg_item.need_score <= 0)
		{
			return -200;
		}

		if (!PugiGetSubNodeValue(data_element, "double_count", cfg_item.double_count) || cfg_item.double_count <= 0)
		{
			return -300;
		}

		{
			// 读物品奖励
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				return -1001;
			}

			if (!cfg_item.item.ReadConfig(item_element))
			{
				return -1002;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig ::InitTimeLimitExchangeJLCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_seq = -1;

	while (!data_element.empty())
	{
		int seq = -1;

		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= TIME_LIMIT_EXCHANGE_ITEM_COUNT || seq != last_seq + 1)
		{
			return -100;
		}

		last_seq = seq;

		RandActivityTimeLimitExchangeConfig &cfg_item = m_time_limit_exchange_jl_list[seq];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "need_score", cfg_item.need_score) || cfg_item.need_score <= 0)
		{
			return -200;
		}

		if (!PugiGetSubNodeValue(data_element, "double_count", cfg_item.double_count) || cfg_item.double_count <= 0)
		{
			return -300;
		}

		{
			// 读物品奖励
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				return -1001;
			}

			if (!cfg_item.item.ReadConfig(item_element))
			{
				return -1002;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityLuckyRollCfg * RandActivityConfig:: GetLuckyRollCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_LUCKY_ROLL);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_luckyroll_cfg[i].opengame_day)
		{
			return &m_luckyroll_cfg[i];
		}
	}

	return NULL;
}

const RandActivityLuckyRollRewardCfg * RandActivityConfig::RandLuckyRollYaojiangRewardCfg(long long gold_poll, int *result_index)
{
	if (NULL == result_index)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_LUCKY_ROLL);
	int stage = MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM - 1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_luckyroll_cfg[i].opengame_day)
		{
			stage = i;
			break;
		}
	}

	int total_weight = 0;
	{
		for (int i = 0; i < RAND_ACTIVITY_LUCKROOL_REWARD_CONFIG_MAX_COUNT; ++ i)
		{
			total_weight += m_luckyroll_cfg[stage].cfg.reward_cfg_list[i].weight;
		}

		if (total_weight <= 0)
		{
			return NULL;
		}
	}

	int roll_index = -1;
	{
		int rand_value = RandomNum(total_weight);
		for (int i = 0; i < RAND_ACTIVITY_LUCKROOL_REWARD_CONFIG_MAX_COUNT; ++ i)
		{
			if (rand_value < m_luckyroll_cfg[stage].cfg.reward_cfg_list[i].weight)
			{
				roll_index = i;
				break;
			}

			rand_value -= m_luckyroll_cfg[stage].cfg.reward_cfg_list[i].weight;
		}
	}

	if (roll_index < 0 || roll_index >= RAND_ACTIVITY_LUCKROOL_REWARD_CONFIG_MAX_COUNT)
	{
		return NULL;
	}

	const RandActivityLuckyRollRewardCfg &reward_cfg = m_luckyroll_cfg[stage].cfg.reward_cfg_list[roll_index];

	if (LUCKY_ROLL_REWARD_TYPE_GOLD == reward_cfg.reward_type)
	{
		if (gold_poll < m_luckyroll_cfg[stage].cfg.get_gold_min_gold_poll)
		{
			roll_index = m_luckyroll_cfg[stage].cfg.replace_gold_reward_seq;
		}
	}

	*result_index = roll_index;
	return &m_luckyroll_cfg[stage].cfg.reward_cfg_list[roll_index];
}

const RandActivityDayChongZhiFanLiCfg::ConfigItem * RandActivityConfig::GetDayChongZhiFanLiRewardCfg(int reward_seq)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_DAY_CHONGZHI_FANLI);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if ( opengame_day > m_day_chongzhi_fanli_cfg[i].opengame_day)
		{
			continue;
		}

		if (reward_seq >= 0 && reward_seq < m_day_chongzhi_fanli_cfg[i].cfg_count)
		{
			return &m_day_chongzhi_fanli_cfg[i].cfg_list[reward_seq];
		}
	}

	return NULL;
}

const RandActivityDayConsumeGoldCfg::ConfigItem * RandActivityConfig::GetDayConsumeGoldRewardCfg(int reward_seq)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_DAY_CONSUME_GOLD);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_day_consume_gold_cfg[i].opengame_day)
		{
			continue;
		}

		if (reward_seq >= 0 && reward_seq < m_day_consume_gold_cfg[i].cfg_count)
		{
			return &m_day_consume_gold_cfg[i].cfg_list[reward_seq];
		}
	}

	return NULL;
}

const RandActivityServerPanicBuyCfg::ConfigItem * RandActivityConfig::GetServerPanicBuyItemCfg(int item_idx)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_SERVER_PANIC_BUY);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_server_panic_buy_cfg[i].opengame_day)
		{
			continue;
		}

		if (item_idx >= 0 && item_idx < m_server_panic_buy_cfg[i].cfg_count)
		{
			return &m_server_panic_buy_cfg[i].cfg_list[item_idx];
		}
	}

	return NULL;
}

const RandActivityPersonalPanicBuyCfg::ConfigItem * RandActivityConfig::GetPersonalPanicBuyItemCfg(int item_idx)
{
	time_t now_sec = EngineAdapter::Instance().Time();
	time_t server_real_start_time = SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime();
	int opengame_day = GetDayIndex(server_real_start_time, now_sec) + 1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{

		if (opengame_day != m_personal_panic_buy_cfg[i].opengame_day)
		{
			continue;
		}

		if (item_idx >= 0 && item_idx <= m_personal_panic_buy_cfg[i].cfg_count)
		{
			return &m_personal_panic_buy_cfg[i].cfg_list[item_idx];
		}
	}

	return NULL;
}

const RandActivityChestShopCfg::ConfigItem * RandActivityConfig::GetChestshopCfgBySeq(int seq)
{
	if (seq >= 0 && seq < m_chestshop_reward_cfg.cfg_count)
	{
		return &m_chestshop_reward_cfg.cfg_list[seq];
	}

	return NULL;
}

const RandActivityStoneUplevelCfg::ConfigItem * RandActivityConfig::GetStoneUplevelCfgByLevel(int total_stone_level)
{
	for (int i = m_stone_uplevel_cfg.cfg_count - 1; i >= 0; -- i)
	{
		if (total_stone_level >= m_stone_uplevel_cfg.cfg_list[i].need_total_stone_level)
		{
			return &m_stone_uplevel_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const RandActivityStoneUplevelCfg::ConfigItem * RandActivityConfig::GetStoneUplevelCfgBySeq(int seq)
{
	if (seq >= 0 && seq < m_stone_uplevel_cfg.cfg_count)
	{
		return &m_stone_uplevel_cfg.cfg_list[seq];
	}

	return NULL;
}

const RandActivityXiannvChanmianUplevelCfg::ConfigItem * RandActivityConfig::GetXiannvChanmianUplevelCfgByLevel(int level)
{
	for (int i = m_xiannv_chanmian_uplevel_cfg.cfg_count - 1; i >= 0; -- i)
	{
		if (level >= m_xiannv_chanmian_uplevel_cfg.cfg_list[i].need_chanmian_level)
		{
			return &m_xiannv_chanmian_uplevel_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const RandActivityXiannvChanmianUplevelCfg::ConfigItem * RandActivityConfig::GetXiannvChanmianUplevelCfgBySeq(int seq)
{
	if (seq >= 0 && seq < m_xiannv_chanmian_uplevel_cfg.cfg_count)
	{
		return &m_xiannv_chanmian_uplevel_cfg.cfg_list[seq];
	}

	return NULL;
}

const RandActivityMountUpgradeCfg::ConfigItem * RandActivityConfig::GetMountUpgradeCfgBySeq(int seq)
{
	if (seq >= 0 && seq < m_mount_upgrade_cfg.cfg_count)
	{
		return &m_mount_upgrade_cfg.cfg_list[seq];
	}

	return NULL;
}

const RandActivityMountUpgradeCfg::ConfigItem * RandActivityConfig::GetMountUpgradeCfgByGrade(int grade)
{
	for (int i = m_mount_upgrade_cfg.cfg_count - 1; i >= 0; -- i)
	{
		if (grade >= m_mount_upgrade_cfg.cfg_list[i].need_mount_grade)
		{
			return &m_mount_upgrade_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const RandActivityQibingUpgradeCfg::ConfigItem * RandActivityConfig::GetQibingUpgradeCfgBySeq(int seq)
{
	if (seq >= 0 && seq < m_qibing_upgrade_cfg.cfg_count)
	{
		return &m_qibing_upgrade_cfg.cfg_list[seq];
	}

	return NULL;
}

const RandActivityQibingUpgradeCfg::ConfigItem * RandActivityConfig::GetQibingUpgradeCfgByGrade(int grade)
{
	for (int i = m_qibing_upgrade_cfg.cfg_count - 1; i >= 0; -- i)
	{
		if (grade >= m_qibing_upgrade_cfg.cfg_list[i].need_qibing_grade)
		{
			return &m_qibing_upgrade_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const RandActivityMentalityTotalLevelCfg::ConfigItem * RandActivityConfig::GetMentalityTotalLevelCfgBySeq(int seq)
{
	if (seq >= 0 && seq < m_mentality_total_level_cfg.cfg_count)
	{
		return &m_mentality_total_level_cfg.cfg_list[seq];
	}

	return NULL;
}

const RandActivityMentalityTotalLevelCfg::ConfigItem * RandActivityConfig::GetMentalityTotalLevelCfgByLevel(int level)
{
	for (int i = m_mentality_total_level_cfg.cfg_count - 1; i >= 0; -- i)
	{
		if (level >= m_mentality_total_level_cfg.cfg_list[i].need_mentality_total_level)
		{
			return &m_mentality_total_level_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const RandActivityWingUpgradeCfg::ConfigItem * RandActivityConfig::GetWingUpgradeCfgBySeq(int seq)
{
	if (seq >= 0 && seq < m_wing_upgrade_cfg.cfg_count)
	{
		return &m_wing_upgrade_cfg.cfg_list[seq];
	}

	return NULL;
}

const RandActivityWingUpgradeCfg::ConfigItem * RandActivityConfig::GetWingUpgradeCfgByGrade(int grade)
{
	for (int i = m_wing_upgrade_cfg.cfg_count - 1; i >= 0; -- i)
	{
		if (grade >= m_wing_upgrade_cfg.cfg_list[i].need_wing_grade)
		{
			return &m_wing_upgrade_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const RandActivityDayOnlineGiftCfg::ConfigItem * RandActivityConfig::GetNextDayOnlineGiftCfg(int now_hour)
{
	int opemgame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_DAY_ONLINE_GIFT);

	
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opemgame_day > m_day_online_gift_cfg[i].opengame_day)
		{
			continue;
		}

		for (int j = 0; j < m_day_online_gift_cfg[i].cfg_count; ++ j)
		{
			if (m_day_online_gift_cfg[i].cfg_list[j].send_reward_hour > now_hour)
			{
				return &m_day_online_gift_cfg[i].cfg_list[j];
			}
		}

		return &m_day_online_gift_cfg[i].cfg_list[0];
	}

	return NULL;
}

const ItemConfigData * RandActivityConfig::GetDayOnlineGiftCfgRewardItem()
{
	int opengame_day =  this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_DAY_ONLINE_GIFT);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_day_online_gift_cfg[i].opengame_day)
		{
			return &m_day_online_gift_cfg[i].reward_item;
		}
	}

	return NULL;
}

const RandActivityLoginGiftCfg * RandActivityConfig::GetLoginGiftCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_LOGIN_GIFT);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_login_gift_cfg[i].opengame_day)
		{
			return &m_login_gift_cfg[i];
		}
	}
	
	return NULL;
}

const RandActivityLoginGiftCfg * RandActivityConfig::GetLoginGiftCfg_0()
{
	return &m_login_gift_cfg_0;
}

const RandActivityQuanminQifuCfg::ConfigItem * RandActivityConfig::GetQuanminQifuCfgBySeq(int seq)
{
	if (seq >= 0 && seq < m_quanmin_qifu_cfg.cfg_count)
	{
		return &m_quanmin_qifu_cfg.cfg_list[seq];
	}

	return NULL;
}

const RandActivityChongzhiRankCfg::ConfigItem * RandActivityConfig::GetChongzhiRankRewardCfg(int user_rank, int chongzhi)
{
	if (user_rank < 0) return NULL;

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CHONGZHI_RANK);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_chongzhi_rank_reward_cfg[i].opengame_day)
		{
			continue;
		}
		
		RandActivityChongzhiRankCfg *cfg = &m_chongzhi_rank_reward_cfg[i];
		for (int j = 0; j < cfg->cfg_count; ++ j)
		{
			if (user_rank <= cfg->cfg_list[j].rank && chongzhi >= cfg->cfg_list[j].limit_chongzhi)
			{
				return &cfg->cfg_list[j];
			}
		}
	}

	return NULL;
}

int RandActivityConfig::GetChongzhiRankCfgCount()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CHONGZHI_RANK);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_chongzhi_rank_reward_cfg[i].opengame_day)
		{
			continue;
		}

		RandActivityChongzhiRankCfg *cfg = &m_chongzhi_rank_reward_cfg[i];

		return cfg->cfg_count;
	}

	return 0;
}

const RandActivityChongzhiRankCfg::ConfigItem * RandActivityConfig::GetChongzhiRank2RewardCfg(int user_rank, int chongzhi)
{
	if (user_rank < 0) return NULL;

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CHONGZHI_RANK2);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_chongzhi_rank2_reward_cfg[i].opengame_day)
		{
			continue;
		}

		RandActivityChongzhiRankCfg *cfg = &m_chongzhi_rank2_reward_cfg[i];
		for (int j = 0; j < cfg->cfg_count; ++j)
		{
			if (user_rank <= cfg->cfg_list[j].rank && chongzhi >= cfg->cfg_list[j].limit_chongzhi)
			{
				return &cfg->cfg_list[j];
			}
		}
	}

	return NULL;
}

int RandActivityConfig::GetChongzhiRank2CfgCount()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CHONGZHI_RANK2);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_chongzhi_rank2_reward_cfg[i].opengame_day)
		{
			continue;
		}

		RandActivityChongzhiRankCfg *cfg = &m_chongzhi_rank2_reward_cfg[i];

		return cfg->cfg_count;
	}

	return 0;
}

int RandActivityConfig::GetConsumeRankCfgCount()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_consume_gold_rand_cfg[i].opengame_day)
		{
			continue;
		}

		return m_consume_gold_rand_cfg[i].cfg_count;
	}

	return 0;
}

const RandActivityConsumeGoldRankCfg::ConfigItem * RandActivityConfig::GetConsumeGoldRankRewardCfg(int user_rank, int consume)
{
	if (user_rank < 0) return NULL;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_consume_gold_rand_cfg[i].opengame_day)
		{
			continue;
		}

		for (int j = 0; j < m_consume_gold_rand_cfg[i].cfg_count; ++ j)
		{
			if (user_rank <= m_consume_gold_rand_cfg[i].cfg_list[j].rank && consume >= m_consume_gold_rand_cfg[i].cfg_list[j].limit_comsume)
			{
				return &m_consume_gold_rand_cfg[i].cfg_list[j];
			}
		}	
	}

	return NULL;
}

const int RandActivityConfig::GetConsumeGoldRankNeedChonghzi(int user_rank)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_consume_gold_rand_cfg[i].opengame_day)
		{
			continue;
		}

		for (int j = 0; j < m_consume_gold_rand_cfg[i].cfg_count; ++j)
		{
			if (m_consume_gold_rand_cfg[i].cfg_list[j].rank >= user_rank)
			{
				return m_consume_gold_rand_cfg[i].cfg_list[j].limit_comsume;
			}
		}
	}

	return 99999999;
}


const RandActivityEquipStrengthenCfg::ConfigItem * RandActivityConfig::GetEquipStrengthRewardCfg(int strengthen_level)
{
	if (strengthen_level <= 0) return NULL;

	for (int i = m_strengthen_reward_cfg.cfg_count - 1; i >= 0; -- i)
	{
		if (strengthen_level >= m_strengthen_reward_cfg.cfg_list[i].level)
		{
			return &m_strengthen_reward_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const RandActivityTotalConsumeGoldCfg::ConfigItem * RandActivityConfig::GetTotalConsumeGoldRewardCfg(int reward_seq)
{
	const RandActivityTotalConsumeGoldCfg *total_consume_gold_reward_cfg = this->GetDiffTotalConsumeGoldRewardCfg();
	if (NULL == total_consume_gold_reward_cfg)
	{
		return NULL;
	}

	if (reward_seq >= 0 && reward_seq < total_consume_gold_reward_cfg->cfg_count && reward_seq < RAND_ACTIVITY_TOTAL_CONSUME_GOLD_REWARD_CONFIG_MAX_COUNT)
	{
		return &total_consume_gold_reward_cfg->cfg_list[reward_seq];
	}

	return NULL;
}

const RandActivityTotalConsumeGoldCfg * RandActivityConfig::GetDiffTotalConsumeGoldRewardCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_TOTAL_CONSUME_GOLD);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_diff_total_consume_gold_reward_list[i].opengame_day)
		{
			return &m_diff_total_consume_gold_reward_list[i];
		}
	}

	return NULL;
}

const RandActivityDayActiveDegreeCfg::ConfigItem * RandActivityConfig::GetDayActiveRewardRewardCfg(int reward_seq)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_DAY_ACTIVIE_DEGREE);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_day_active_degree_reward_cfg[i].opengame_day)
		{
			continue;
		}

		if (reward_seq >= 0 && reward_seq < m_day_active_degree_reward_cfg[i].cfg_count)
		{
			return &m_day_active_degree_reward_cfg[i].cfg_list[reward_seq];
		}
	}

	return NULL;
}

const RandActivityXianmengBipinCfg::ConfigItem * RandActivityConfig::GetXianmengBipinCfg(int rank)
{
	for (int i = 0; i < m_xianmeng_bipin_cfg.cfg_count; ++ i)
	{
		if (rank <= m_xianmeng_bipin_cfg.cfg_list[i].need_rank)
		{
			return &m_xianmeng_bipin_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const RandActivityXianmengJueQiCfg::ConfigItem * RandActivityConfig::GetXianmengJueQiCfg(int rank)
{
	for (int i = 0; i < m_xianmeng_jueqi_cfg.cfg_count; ++ i)
	{
		if (rank <= m_xianmeng_jueqi_cfg.cfg_list[i].need_rank)
		{
			return &m_xianmeng_jueqi_cfg.cfg_list[i];
		}
	}

	return NULL;
}

const RandActivityLoginGiftCfg::ConfigItem * RandActivityConfig::GetLoginGiftRewardCfg(int seq)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_LOGIN_GIFT);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if ( opengame_day > m_login_gift_cfg[i].opengame_day)
		{
			continue;
		}

		if (seq >= 0 && seq < m_login_gift_cfg[i].cfg_count)
		{
			return &m_login_gift_cfg[i].cfg_list[seq];
		}
	}

	return NULL;
}

const RandActivityShouYouYuXiangCfg::ConfigItem * RandActivityConfig::GetShouYouYuXiangRewardCfg(int reward_seq)
{
	if (reward_seq >= 0 && reward_seq < m_shouyou_yuxiang_cfg.cfg_count)
	{
		return &m_shouyou_yuxiang_cfg.cfg_list[reward_seq];
	}

	return NULL;
}

const RandActivityRankBipinCfg * RandActivityConfig::GetRankBipinCfg(int activity_type)
{
	for (int i = 0; i < m_rank_bipin_cfg_count; ++ i)
	{
		if (activity_type == m_rank_bipin_cfglist[i].activity_type)
		{
			return &m_rank_bipin_cfglist[i];
		}
	}

	return NULL;
}

const RandActivityAllBipinCfg::ConfigItem * RandActivityConfig::GetAllBipinCfg(int activity_type, int seq)
{
	if (seq >= 0 && seq < RAND_ACTIVITY_ALL_BIPIN_REWARD_CONFIG_MAX_COUNT)
	{
		for (int i = 0; i < m_all_bipin_cfg_count; ++ i)
		{
			if (activity_type == m_all_bipin_cfglist[i].activity_type)
			{
				if (seq < m_all_bipin_cfglist[i].cfg_count)
				{
					return &m_all_bipin_cfglist[i].cfg_list[seq];
				}
			}
		}
	}

	return NULL;
}

const RandActivityChargeRepaymentCfg::ConfigItem * RandActivityConfig::GetChargeRepaymentCfgWithSeq(int seq)
{
	if (seq < 0)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CHARGE_REPAYMENT);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_charge_repayment_cfg[i].opengame_day)
		{
			continue;
		}

		for (int index = m_charge_repayment_cfg[i].cfg_count - 1; index >= 0; index --)
		{
			RandActivityChargeRepaymentCfg::ConfigItem *cfg_item = &m_charge_repayment_cfg[i].cfg_list[index];
			if (seq == cfg_item->seq)
			{
				return cfg_item;
			}
		}

		return NULL;
	}

	return NULL;
}

const RandActivityChargeRepaymentCfg::ConfigItem * RandActivityConfig::GetChargeRepaymentCfgWithCharge(long long charge_value)
{
	if (charge_value <= 0)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CHARGE_REPAYMENT);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day >  m_charge_repayment_cfg[i].opengame_day)
		{
			continue;
		}

		for (int index = m_charge_repayment_cfg[i].cfg_count - 1; index >= 0; index --)
		{
			RandActivityChargeRepaymentCfg::ConfigItem *cfg_item = &m_charge_repayment_cfg[i].cfg_list[index];
			if (charge_value >= cfg_item->charge_value)
			{
				return cfg_item;
			}
		}

		return NULL;
	}

	return NULL;
}

const RandActivitySingleChargeCfg::ConfigItem * RandActivityConfig::GetSingleChargeCfg(long long charge_value)
{
	if (charge_value <= 0)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_SINGLE_CHARGE);

	for (int i = 0; i < RAND_ACTIVITY_SINGLE_CHAR_CFG_COUNT; ++ i)
	{
		if (opengame_day > m_single_charge_cfg[i].opengame_day)
		{
			continue;
		}

		for (int index = m_single_charge_cfg[i].cfg_count - 1; index >= 0; index --)
		{
			RandActivitySingleChargeCfg::ConfigItem *cfg_item = &m_single_charge_cfg[i].cfg_list[index];
			if (charge_value >= cfg_item->charge_value)
			{
				return cfg_item;
			}
		}
	}

	return NULL;
}

const RandActivitySingleChargeCfg::ConfigItem * RandActivityConfig::GetSingleCharge1Cfg(long long charge_value,int flag)
{
	if (charge_value <= 0)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_SINGLE_CHARGE1);

	for (int i = 0; i < RAND_ACTIVITY_SINGLE_CHAR_CFG_COUNT; ++i)
	{
		if (opengame_day > m_single_charge1_cfg[i].opengame_day)
		{
			continue;
		}

		for (int index = m_single_charge1_cfg[i].cfg_count - 1; index >= 0; index--)
		{
			RandActivitySingleChargeCfg::ConfigItem *cfg_item = &m_single_charge1_cfg[i].cfg_list[index];
			if (charge_value >= cfg_item->charge_value && (flag & (1 << cfg_item->seq)) == 0)
			{
				return cfg_item;
			}
		}
	}

	return NULL;
}

const RandActivitySingleChargeCfg::ConfigItem * RandActivityConfig::GetSingleCharge2Cfg(long long charge_value,int flag)
{
	if (charge_value <= 0)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_SINGLE_CHARGE2);

	for (int i = 0; i < RAND_ACTIVITY_SINGLE_CHAR_CFG_COUNT; ++i)
	{
		if (opengame_day > m_single_charge2_cfg[i].opengame_day)
		{
			continue;
		}

		for (int index = m_single_charge2_cfg[i].cfg_count - 1; index >= 0; index--)
		{
			RandActivitySingleChargeCfg::ConfigItem *cfg_item = &m_single_charge2_cfg[i].cfg_list[index];
			if (charge_value >= cfg_item->charge_value && (flag & (1 << cfg_item->seq)) == 0)
			{
				return cfg_item;
			}
		}
	}
	return NULL;
}

const RandActivitySingleChargeCfg::ConfigItem * RandActivityConfig::GetSingleCharge3Cfg(long long charge_value,int flag)
{
	if (charge_value <= 0)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_SINGLE_CHARGE3);

	for (int i = 0; i < RAND_ACTIVITY_SINGLE_CHAR_CFG_COUNT; ++i)
	{
		if (opengame_day > m_single_charge3_cfg[i].opengame_day)
		{
			continue;
		}

		for (int index = m_single_charge3_cfg[i].cfg_count - 1; index >= 0; index--)
		{
			RandActivitySingleChargeCfg::ConfigItem *cfg_item = &m_single_charge3_cfg[i].cfg_list[index];
			if (charge_value >= cfg_item->charge_value && (flag & (1 << cfg_item->seq)) == 0)
			{
				return cfg_item;
			}
		}
	}

	return NULL;
}

const RandActivityCornucopiaCfg *RandActivityConfig::GetCornucopiaCfg(int lun)
{
	if (lun > 0 && lun <= m_cornucopia_max_lun)
	{
		return &m_cornucopia_cfglist[lun];
	}

	return NULL;
}

const RandActivityDanbiChongzhiRewardCfg * RandActivityConfig::GetDanbiChongzhiRewardBySeq(int activity_day, int seq)
{
	const RandActivityDanbiChongzhiDayDiffCfg *diff_cfg = this->GetDanbiChongzhiRewardDiffCfg();
	if (NULL == diff_cfg)
	{
		return NULL;
	}

	if (seq < 0 || seq >= RAND_ACTIVITY_DANBI_CHONGZHI_REWARD_CFG_MAX_COUNT_PER_DAY)
	{
		return NULL;
	}

	if (activity_day > diff_cfg->reward_max_day)
	{
		activity_day = diff_cfg->reward_max_day;
	}

	if (activity_day >= 0 && activity_day < RAND_ACTIVITY_DANBI_CHONGZHI_DAY_CFG_MAX_COUNT)
	{
		const RandActivityDanbiChongzhiDayCfg &day_cfg = diff_cfg->day_cfglist[activity_day];

		if (seq < day_cfg.reward_count)
		{
			return &day_cfg.reward_list[seq];
		}
	}

	return NULL;
}

const RandActivityDanbiChongzhiDayDiffCfg * RandActivityConfig::GetDanbiChongzhiRewardDiffCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_DANBI_CHONGZHI);

	for (int i = 0; i < RAND_ACTIVITY_DANBI_CHONGZHI_DAY_CFG_COUNT; ++ i)
	{
		if (opengame_day <= m_diff_danbi_chongzhi_day_cfglist[i].opengame_day)
		{
			return &m_diff_danbi_chongzhi_day_cfglist[i];
		}
	}

	return NULL;
}

const RandActivityDanbiChongzhiRewardCfg * RandActivityConfig::GetDanbiChongzhiRewardByChongzhi(int activity_day, int chongzhi)
{
	const RandActivityDanbiChongzhiDayDiffCfg *diff_cfg = this->GetDanbiChongzhiRewardDiffCfg();
	if (NULL == diff_cfg)
	{
		return NULL;
	}

	if (activity_day > diff_cfg->reward_max_day)
	{
		activity_day = diff_cfg->reward_max_day;
	}

	if (activity_day >= 0 && activity_day < RAND_ACTIVITY_DANBI_CHONGZHI_DAY_CFG_MAX_COUNT)
	{
		const RandActivityDanbiChongzhiDayCfg &day_cfg = diff_cfg->day_cfglist[activity_day];

		for (int i = day_cfg.reward_count - 1; i >= 0; -- i)
		{
			if (chongzhi >= day_cfg.reward_list[i].need_chongzhi_num)
			{
				return &day_cfg.reward_list[i];
			}
		}
	}

	return NULL;
}

const RandActivityTotalChargeDayCfg::TotalChargeDayItem *RandActivityConfig::GetTotalChargeDayCfgWithSeq(int seq)
{
	if (seq < 0 || seq > RAND_ACTIVITY_TOTAL_CHARGE_DAY_CFG_MAX_SEQ)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_DAY);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && seq < m_total_charge_day_cfglist[i].cfg_count; ++ i)
	{
		if (opengame_day <= m_total_charge_day_cfglist[i].opengame_day)
		{
			return &m_total_charge_day_cfglist[i].cfg_list[seq];
		}	
	}
	
	return NULL;
}

const RandActivityTotalChargeDayCfg::TotalChargeDayItem *RandActivityConfig::GetTotalChargeDayCfgWithDayCount(int charge_day_count)
{
	if (charge_day_count <= 0)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_DAY);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_total_charge_day_cfglist[i].opengame_day)
		{
			continue;
		}

		for (int seq = 0; seq <= RAND_ACTIVITY_TOTAL_CHARGE_DAY_CFG_MAX_SEQ && seq < m_total_charge_day_cfglist[i].cfg_count; ++ seq)
		{
			if (charge_day_count == m_total_charge_day_cfglist[i].cfg_list[seq].need_recharge_day)
			{
				return &m_total_charge_day_cfglist[i].cfg_list[seq];
			}
		}
	}

	return NULL;
}

const RandActivityTomorrowRewardCfg *RandActivityConfig::GetTomorrowRandomRewardCfg()
{
	if (m_tomorrow_reward_cfg_total_weight <= 0) return NULL;

	int rand_weight = RandomNum(m_tomorrow_reward_cfg_total_weight);

	for (int index = 0; index < m_tomorrow_reward_cfg_count; ++ index)
	{
		if (rand_weight < m_tomorrow_reward_cfglist[index].weight)
		{
			return &m_tomorrow_reward_cfglist[index];
		}

		rand_weight -= m_tomorrow_reward_cfglist[index].weight;
	}

	return NULL;
}

const RandActivityTotalChargeRewardConfig::ConfigItem * RandActivityConfig::GetTotalChargeRewardWithSeq(int seq)
{
	const RandActivityTotalChargeRewardConfig *reward_cfg = this->GetTotalChargeRewardCfg();
	if (NULL == reward_cfg)
	{
		return NULL;
	}

	if (seq >= 0 && seq < RandActivityTotalChargeRewardConfig::CONFIG_ITEM_MAX_COUNT && seq < reward_cfg->cfg_count)
	{
		return &reward_cfg->cfg_list[seq];
	}

	return NULL;
}

const RandActivityTotalChargeRewardConfig * RandActivityConfig::GetTotalChargeRewardCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_TOTAL_CHARGE);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		RandActivityTotalChargeRewardConfig *total_chognzhi_reward_cfg = &m_diff_totalcharge_reward_list[i];
		if (opengame_day <= total_chognzhi_reward_cfg->opengame_day)
		{
			return total_chognzhi_reward_cfg;
		}
	}

	return NULL;
}

int RandActivityConfig::GetBiPinActivityTypeByOpenDayIndex(int openday_idx)
{
	for (int i = 0; i < m_rank_bipin_cfg_count; ++ i)
	{
		if (openday_idx == m_rank_bipin_cfglist[i].give_reward_day - 1)  // give_reward_day从2开始 
		{
			return m_rank_bipin_cfglist[i].activity_type;
		}
	}

	return -1;
}

int RandActivityConfig::GetRelateBipinActivity(int person_rank_type)
{
	if (person_rank_type < 0 || person_rank_type >= PERSON_RANK_TYPE_MAX)
	{
		return 0;
	}

	return m_person_rank_bipin_activity_map[person_rank_type];
}

int RandActivityConfig::GetTotalChargeSeqWithCharge(int total_charge)
{
	const RandActivityTotalChargeRewardConfig *reward_cfg = this->GetTotalChargeRewardCfg();
	if (NULL == reward_cfg)
	{
		return -1;
	}

	for (int seq = reward_cfg->cfg_count - 1; seq >= 0; -- seq)
	{
		if (seq >= 0 && reward_cfg->cfg_list[seq].need_chongzhi <= total_charge)
		{
			return seq;
		}
	}

	return -1;
}

int RandActivityConfig::GetNewTotalChargeSeqWithCharge(int total_charge)
{
	const RandActivityNewTotalChargeRewardConfig *reward_cfg = this->GetNewTotalChargeRewardCfg();
	if (NULL == reward_cfg)
	{
		return -1;
	}

	for (int seq = reward_cfg->cfg_count - 1; seq >= 0; --seq)
	{
		if (seq >= 0 && reward_cfg->cfg_list[seq].need_chongzhi <= total_charge)
		{
			return seq;
		}
	}

	return -1;
}

int RandActivityConfig::GetNewTotalCharge1SeqWithCharge(int total_charge)
{
	const RandActivityNewTotalChargeRewardConfig *reward_cfg = this->GetNewTotalCharge1RewardCfg();
	if (NULL == reward_cfg)
	{
		return -1;
	}

	for (int seq = reward_cfg->cfg_count - 1; seq >= 0; --seq)
	{
		if (seq >= 0 && reward_cfg->cfg_list[seq].need_chongzhi <= total_charge)
		{
			return seq;
		}
	}

	return -1;
}

int RandActivityConfig::GetNewTotalCharge2SeqWithCharge(int total_charge)
{
	const RandActivityNewTotalChargeRewardConfig *reward_cfg = this->GetNewTotalCharge2RewardCfg();
	if (NULL == reward_cfg)
	{
		return -1;
	}

	for (int seq = reward_cfg->cfg_count - 1; seq >= 0; --seq)
	{
		if (seq >= 0 && reward_cfg->cfg_list[seq].need_chongzhi <= total_charge)
		{
			return seq;
		}
	}

	return -1;
}

int RandActivityConfig::GetNewTotalCharge3SeqWithCharge(int total_charge)
{
	const RandActivityNewTotalChargeRewardConfig *reward_cfg = this->GetNewTotalCharge3RewardCfg();
	if (NULL == reward_cfg)
	{
		return -1;
	}

	for (int seq = reward_cfg->cfg_count - 1; seq >= 0; --seq)
	{
		if (seq >= 0 && reward_cfg->cfg_list[seq].need_chongzhi <= total_charge)
		{
			return seq;
		}
	}

	return -1;
}

const RandActivityDayChongzhiRankRewardConfig::ConfigItem * RandActivityConfig::GetDayChongzhiRankRewardCfg(int day, int rank, int gold_num)
{
	const RandActivityDayChongzhiRankRewardDiffConfig  *diff_cfg = this->GetDayChongzhiRankRewardDayCfg();

	if (NULL == diff_cfg)
	{
		return NULL;
	}

	if (day > 0)
	{
		if (day > diff_cfg->reward_max_day)
		{
			day = diff_cfg->reward_max_day;
		}

		const RandActivityDayChongzhiRankRewardConfig &day_reward_cfg = diff_cfg->rank_reward_cfglist[day];

		for (int i = 0; i < day_reward_cfg.cfg_count; ++ i)
		{
			if (rank <= day_reward_cfg.cfg_list[i].limit_rank && gold_num >= day_reward_cfg.cfg_list[i].limit_gold)
			{
				return &day_reward_cfg.cfg_list[i];
			}
		}
	}

	return NULL;
}

const RandActivityDayChongzhiRankRewardDiffConfig * RandActivityConfig::GetDayChongzhiRankRewardDayCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_DAY_CHONGZHI_RANK);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_diff_daychongzhirank_reward_list[i].opengame_day)
		{
			return &m_diff_daychongzhirank_reward_list[i];
		}
	}

	return NULL;
}

const RandActivityDayConsumeRankRewardConfig::ConfigItem * RandActivityConfig::GetDayConsumeRankRewardCfg(int day, int rank, int gold_num)
{
	const RandActivityDayConsumeRankRewardDiffConfig *diff_cfg = this->GetDayConsumeRankRewardDayCfg();
	if (NULL == diff_cfg)
	{
		return NULL;
	}

	if (day > 0)
	{
		if (day > diff_cfg->reward_max_day)
		{
			day = diff_cfg->reward_max_day;
		}

		const RandActivityDayConsumeRankRewardConfig &day_reward_cfg = diff_cfg->rank_reward_cfglist[day];

		for (int i = 0; i < day_reward_cfg.cfg_count; ++ i)
		{
			if (rank <= day_reward_cfg.cfg_list[i].limit_rank && gold_num >= day_reward_cfg.cfg_list[i].limit_gold)
			{
				return &day_reward_cfg.cfg_list[i];
			}
		}
	}

	return NULL;
}

const RandActivityDayConsumeRankRewardDiffConfig * RandActivityConfig::GetDayConsumeRankRewardDayCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_DAY_CONSUME_RANK);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		
		if (opengame_day <= m_diff_dayconsume_rank_reward_cfglist[i].opengame_day)
		{
			return &m_diff_dayconsume_rank_reward_cfglist[i];
		}
	}

	return NULL;
}

int RandActivityConfig::GetIntervalOpenGameDay(int activity_type)
{
	unsigned int begin_time = ActivityShadow::Instance().GetRandActivityBeginTime(activity_type);
	if (activity_type == RAND_ACTIVITY_TYPE_DOUBLE_GET)
	{
		begin_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()); //双倍元宝以当前时间给奖励
	}

	time_t zero_time = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime();
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return 0;
	}
	
	if (begin_time < zero_time) return 0;

	int interval_day = (unsigned int)((begin_time - zero_time) / (3600 * 24)) + 1;

	return interval_day;
}

const RandActivityTimeLimitExchangeConfig *RandActivityConfig::GetTimeLimitExchangeEquiCfg(int seq)
{
	if (seq < 0 || seq >= TIME_LIMIT_EXCHANGE_ITEM_COUNT)
	{
		return NULL;
	}

	return &m_time_limit_exchange_equi_list[seq];
}

const RandActivityTimeLimitExchangeConfig *RandActivityConfig::GetTimeLimitExchangeJLCfg (int seq)
{
	if (seq < 0 || seq >= TIME_LIMIT_EXCHANGE_ITEM_COUNT)
	{
		return NULL;
	}

	return &m_time_limit_exchange_jl_list[seq];
}

int RandActivityConfig::InitLevelLotteryOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "free_time", m_level_lottery_other_cfg.free_times) || m_level_lottery_other_cfg.free_times < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "free_interval", m_level_lottery_other_cfg.free_interval) || m_level_lottery_other_cfg.free_interval < 0)
	{
		return -2;
	}

	return 0;
}

int RandActivityConfig::InitLevelLotteryConsumeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_level_lottery_max_level = -1;

	while (!data_element.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level != m_level_lottery_max_level + 1)
		{
			return -1;
		}
		m_level_lottery_max_level = level;

		if (level > RA_LEVEL_LOTTERY_MAX_LEVEL_LIMIT)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "lottery_consume_gold", m_level_lottery_consume_gold_level_list[level]) || 
			m_level_lottery_consume_gold_level_list[level] <= 0)
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitLevelLotteryRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int opengame_day = 0;
	int last_opengamed_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{

		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengamed_day)
		{
			++ stage_num;
			m_level_lottery_cfg_list_count = 0;
		}
		last_opengamed_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		m_level_lottery_cfg_list[stage_num].opengame_day =opengame_day;

		if (m_level_lottery_cfg_list_count > RA_LEVEL_LOTTERY_MAX_REWARD_ID_LIMIT)
		{
			return -1;
		}

		RandActivityAdditionLotteryConfig::ConfigItem &level_lottery_cfg = m_level_lottery_cfg_list[stage_num].cfg_list[m_level_lottery_cfg_list_count];

		if (!PugiGetSubNodeValue(data_element, "reward_index", level_lottery_cfg.reward_index) || level_lottery_cfg.reward_index != m_level_lottery_cfg_list_count)
		{
			return -2;
		}

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -3;
		}

		if (!level_lottery_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "level", level_lottery_cfg.level) || level_lottery_cfg.level < 0 || level_lottery_cfg.level > m_level_lottery_max_level)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", level_lottery_cfg.weight) || level_lottery_cfg.weight <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "to_level", level_lottery_cfg.to_level) || level_lottery_cfg.to_level < 0 || level_lottery_cfg.to_level > m_level_lottery_max_level)
		{
			return -7;
		}

		int is_broadcast = 0;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast) || is_broadcast < 0 || is_broadcast > 1)
		{
			return -7;
		}
		level_lottery_cfg.is_broadcast = (is_broadcast == 1);

		int is_record_history = 0;
		if (!PugiGetSubNodeValue(data_element, "is_record_history", is_record_history) || is_record_history < 0 || is_record_history > 1)
		{
			return -8;
		}
		level_lottery_cfg.is_record_history = (is_record_history == 1);

		int baodi = 0;
		if (PugiGetSubNodeValue(data_element, "baodi", baodi) && baodi > 0)
		{
			// 保底次数必须不同,因为保底必出物品（除非保底物品也随机）
			if (m_level_lottery_cfg_list[stage_num].baodi_map.find(baodi) != m_level_lottery_cfg_list[stage_num].baodi_map.end()
				|| m_level_lottery_cfg_list[stage_num].baodi_map.size() > 0)
			{
				return -9;
			}

			m_level_lottery_cfg_list[stage_num].baodi_map.insert(std::make_pair(baodi, level_lottery_cfg.reward_index));
		}

		++ m_level_lottery_cfg_list_count;
		++ m_level_lottery_cfg_list[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitLevelLotteryTotalTimesCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int opengame_day = -1;

		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -1;
		}

		std::map<int, RandActivityLevelLotteryTotalTimesCfg> &open_day_cfg_map = m_level_lottery_total_times_map[opengame_day];

		int total_times = 0;
		if (!PugiGetSubNodeValue(data_element, "total_times", total_times) || total_times < 0 || 
			open_day_cfg_map.find(total_times) != open_day_cfg_map.end())
		{
			return -2;
		}

		RandActivityLevelLotteryTotalTimesCfg cfg;
		cfg.total_times = total_times;
		
		if (!PugiGetSubNodeValue(data_element, "vip_level_limit", cfg.vip_level_limit) || cfg.vip_level_limit < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_index", cfg.reward_index) || cfg.reward_index < 0 || cfg.reward_index > 31)
		{
			return -4;
		}

		{
			PugiXmlNode element = data_element.child("reward_list");
			if (element.empty())
			{
				return -100;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("reward");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -101 - i;
				}

				if (!cfg.reward[i].ReadConfig(item_element))
				{
					return -151 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}

		open_day_cfg_map[total_times] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityAdditionLotteryConfig::ConfigItem * RandActivityConfig::GetLevelLotteryRandomRewardCfg(int level)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_LEVEL_LOTTERY);

	if (level >= 0 && level <= m_level_lottery_max_level && level < RA_LEVEL_LOTTERY_MAX_LEVEL_LIMIT)
	{
		static RandActivityAdditionLotteryConfig::ConfigItem tmp_level_reward_cfg_list[RA_LEVEL_LOTTERY_MAX_REWARD_ID_LIMIT + 1];

		int level_cfg_count = 0;
		for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
		{
			if (opengame_day > m_level_lottery_cfg_list[i].opengame_day)
			{
				continue;
			}

			for (int reward_id = 0; reward_id < m_level_lottery_cfg_list_count && reward_id <= RA_LEVEL_LOTTERY_MAX_REWARD_ID_LIMIT; ++ reward_id)
			{
				if (m_level_lottery_cfg_list[i].cfg_list[reward_id].level == level && level_cfg_count <= RA_LEVEL_LOTTERY_MAX_REWARD_ID_LIMIT)
				{
					tmp_level_reward_cfg_list[level_cfg_count] = m_level_lottery_cfg_list[i].cfg_list[reward_id];
					level_cfg_count++;
				}
			}

			break;
		}


		if (level_cfg_count > 0)
		{
			int total_weight = 0;
			for (int index = 0; index < level_cfg_count; ++ index)
			{
				total_weight += tmp_level_reward_cfg_list[index].weight;
			}
			
			if (total_weight <= 0)
			{
				return NULL;
			}

			int rand_weight = RandomNum(total_weight);
			for (int index = 0; index < level_cfg_count; ++ index)
			{
				if (rand_weight < tmp_level_reward_cfg_list[index].weight)
				{
					return &tmp_level_reward_cfg_list[index];
				}

				rand_weight -= tmp_level_reward_cfg_list[index].weight;
			}
		}
	}

	return NULL;
}

const RandActivityAdditionLotteryConfig::ConfigItem * RandActivityConfig::GetLevelLotteryBaodiRewardCfg(int server_chou_times)
{
	if (server_chou_times <= 0) return nullptr;

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_LEVEL_LOTTERY);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_level_lottery_cfg_list[i].opengame_day)
		{
			continue;
		}

		RandActivityAdditionLotteryConfig &cur_stage_cfg = m_level_lottery_cfg_list[i];

		for (std::map<int, int>::iterator it = cur_stage_cfg.baodi_map.begin(); it != cur_stage_cfg.baodi_map.end(); ++it)
		{
			const int baodi_times = it->first;
			const int reward_seq = it->second;

			if (baodi_times <= 0 || 0 != server_chou_times % baodi_times) continue;

			if (reward_seq < 0 || reward_seq >= m_level_lottery_cfg_list_count || reward_seq >= RA_LEVEL_LOTTERY_MAX_REWARD_ID_LIMIT) continue;

			return &m_level_lottery_cfg_list[i].cfg_list[reward_seq];
		}
		break;
	}

	return nullptr;
}

const RandActivityLevelLotteryTotalTimesCfg * RandActivityConfig::GetLevelLotteryTotalTimesRewardCfg(int total_times)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_LEVEL_LOTTERY);
	std::map<int, std::map<int, RandActivityLevelLotteryTotalTimesCfg> >::const_iterator opengame_day_iter = m_level_lottery_total_times_map.begin();
	for (; opengame_day_iter != m_level_lottery_total_times_map.end(); ++opengame_day_iter)
	{
		if (opengame_day > opengame_day_iter->first)
		{
			continue;
		}

		const std::map<int, RandActivityLevelLotteryTotalTimesCfg > &reward_iter = opengame_day_iter->second;
		std::map<int, RandActivityLevelLotteryTotalTimesCfg >::const_iterator iter = reward_iter.find(total_times);
		if (iter != reward_iter.end() && iter->second.total_times == total_times)
		{
			return &iter->second;
		}

		return NULL;
	}

	return NULL;
}

const std::map<int, RandActivityLevelLotteryTotalTimesCfg > & RandActivityConfig::GetLevelLotteryOpengameDayRewardCfg(int join_day)
{
	static std::map<int, RandActivityLevelLotteryTotalTimesCfg > empty_map;
	for (std::map<int, std::map<int, RandActivityLevelLotteryTotalTimesCfg> >::iterator iter = m_level_lottery_total_times_map.begin();
		iter != m_level_lottery_total_times_map.end(); ++iter)
	{
		if (join_day > iter->first)
		{
			continue;
		}

		return iter->second;
	}

	return empty_map;
}

int RandActivityConfig::GetLevelLotteryNeedGold(int level)
{
	if (level >= 0 && level <= m_level_lottery_max_level && level <= RA_LEVEL_LOTTERY_MAX_LEVEL_LIMIT)
	{
		return m_level_lottery_consume_gold_level_list[level];
	}

	return 0;
}

int RandActivityConfig::GetZhenbaogeAddWeightSeq(int index)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_ZHENBAOGE);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_zhenbaoge_auto_add_cfg_count; ++i)
	{
		if (m_zhenbaoge_add_weight_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	RandActivityZhenBaoGeAddWeightDayCfg &day_cfg = m_zhenbaoge_add_weight_cfg_list[idx];

	if (index < 0 || index >= day_cfg.cfg_count)
	{
		return 0;
	}

	return day_cfg.cfg_list[index].seq;
}

int RandActivityConfig::InitNiuEggCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_niu_egg_chou_total_weight = 0;
	m_niu_egg_chou_reward_cfg_count = 0;
	m_niu_egg_server_reward_cfg_count = 0;
	
	int last_server_niu_times = 0;

	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++ stage_num;
			m_niu_egg_chou_reward_cfg_count = 0;
			m_niu_egg_server_reward_cfg_count = 0;
			last_server_niu_times = 0;
			m_niu_egg_chou_total_weight = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0  || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM )
		{
			return -200;
		}

		m_niu_egg_chou_reward_cfg_list[stage_num].opengame_day = opengame_day;
		m_niu_egg_server_reward_cfg_list[stage_num].opengame_day = opengame_day;

		int cfg_type = 0;
		if (!PugiGetSubNodeValue(data_element, "cfg_type", cfg_type) || cfg_type < 0 || cfg_type > 1)
		{
			return -1;
		}

		int req = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", req))
		{
			return -2;
		}

		if (0 == cfg_type) // 抽奖配置
		{
			if (m_niu_egg_chou_reward_cfg_count >= RA_NIU_EGG_CHOU_REWARD_CFG_COUNT_LIMIT)
			{
				return -11;
			}

			if (req != m_niu_egg_chou_reward_cfg_count)
			{
				return -12;
			}

			RandActivityNiuEggChouRewardConfig::RANiuEggConfigItem &chou_cfg = m_niu_egg_chou_reward_cfg_list[stage_num].cfg_list[req];
			chou_cfg.req = req;

			if (!PugiGetSubNodeValue(data_element, "weight", chou_cfg.weight) || chou_cfg.weight <= 0)
			{
				return -13;
			}
			m_niu_egg_chou_total_weight += chou_cfg.weight;

			int is_broadcast = 0;
			if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast) || is_broadcast < 0 || is_broadcast > 1)
			{
				return -14;
			}
			chou_cfg.is_broadcast = (is_broadcast > 0);

			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -15;
			}

			if (!chou_cfg.reward_item.ReadConfig(reward_item_element))
			{
				return -16;
			}

			int baodi = 0;
			if (PugiGetSubNodeValue(data_element, "baodi", baodi) && baodi > 0)
			{
				// 保底次数必须不同,因为保底必出物品（除非保底物品也随机）
				if (m_niu_egg_chou_reward_cfg_list[stage_num].baodi_map.find(baodi) != m_niu_egg_chou_reward_cfg_list[stage_num].baodi_map.end()
					|| m_niu_egg_chou_reward_cfg_list[stage_num].baodi_map.size() > 0)
				{
					return -9;
				}

				m_niu_egg_chou_reward_cfg_list[stage_num].baodi_map.insert(std::make_pair(baodi, chou_cfg.req));
			}

			++ m_niu_egg_chou_reward_cfg_count;
			++ m_niu_egg_chou_reward_cfg_list[stage_num].cfg_count;
		}
		else if (1 == cfg_type) // 累计全服扭蛋奖励
		{
			if (m_niu_egg_server_reward_cfg_count >= RA_NIU_EGG_SERVER_REWARD_CFG_COUNT_LIMIT)
			{
				return -21;
			}

			if (req != m_niu_egg_server_reward_cfg_count)
			{
				return -22;
			}

			RandActivityNiuEggServerRewardConfig::RANiuEggConfigItem &server_reward_cfg = m_niu_egg_server_reward_cfg_list[stage_num].cfg_list[req];
			server_reward_cfg.req = req;

			if (!PugiGetSubNodeValue(data_element, "server_niu_times", server_reward_cfg.server_niu_times) || server_reward_cfg.server_niu_times <= 0 || server_reward_cfg.server_niu_times < last_server_niu_times)
			{
				return -23;
			}
			last_server_niu_times = server_reward_cfg.server_niu_times;

			if (!PugiGetSubNodeValue(data_element, "vip_limit", server_reward_cfg.vip_limit) || server_reward_cfg.vip_limit < 0 || server_reward_cfg.vip_limit > MAX_VIP_LEVEL)
			{
				return -14;
			}

			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -15;
			}

			if (!server_reward_cfg.reward_item.ReadConfig(reward_item_element))
			{
				return -16;
			}

			++ m_niu_egg_server_reward_cfg_count;
			++ m_niu_egg_server_reward_cfg_list[stage_num].cfg_count;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityNiuEggChouRewardConfig::RANiuEggConfigItem * RandActivityConfig::GetRandomNiuEggRewardCfg()
{
	if (m_niu_egg_chou_total_weight <= 0)
	{
		return  NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG);

	int random_weight = RandomNum(m_niu_egg_chou_total_weight);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_niu_egg_chou_reward_cfg_list[i].opengame_day)
		{
			continue;
		}

		for (int req = 0; req < m_niu_egg_chou_reward_cfg_count && req < RA_NIU_EGG_CHOU_REWARD_CFG_COUNT_LIMIT && req < m_niu_egg_chou_reward_cfg_list[i].cfg_count; ++ req)
		{
			RandActivityNiuEggChouRewardConfig::RANiuEggConfigItem &cfg = m_niu_egg_chou_reward_cfg_list[i].cfg_list[req];
			if (random_weight < cfg.weight)
			{
				return &cfg;
			}

			random_weight -= cfg.weight;
		}
	}

	return NULL;
}

const RandActivityNiuEggChouRewardConfig::RANiuEggConfigItem * RandActivityConfig::GetNiuEggBaodiRewardCfg(const int server_chou_times)
{
	if (server_chou_times <= 0) return nullptr;

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG);

	int stage = -1;
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_niu_egg_chou_reward_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage >= 0 && stage < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		RandActivityNiuEggChouRewardConfig &cur_cfg = m_niu_egg_chou_reward_cfg_list[stage];

		for (std::map<int, int>::iterator it = cur_cfg.baodi_map.begin(); it != cur_cfg.baodi_map.end(); ++it)
		{
			const int baodi_times = it->first;
			const int reward_seq = it->second;

			if (baodi_times <= 0 || 0 != server_chou_times % baodi_times) continue;

			if (reward_seq < 0 || reward_seq >= RA_NIU_EGG_CHOU_REWARD_CFG_COUNT_LIMIT || cur_cfg.cfg_list[reward_seq].req != reward_seq) continue;

			return &cur_cfg.cfg_list[reward_seq];
		}
	}

	return nullptr;
}

const RandActivityNiuEggServerRewardConfig::RANiuEggConfigItem * RandActivityConfig::GetNiuServerCfgWithServerNiuTimes(int server_total_niu_times)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_niu_egg_server_reward_cfg_list[i].opengame_day)
		{
			continue;
		}

		for (int req = m_niu_egg_server_reward_cfg_count - 1; req >= 0; -- req)
		{
			if (req < RA_NIU_EGG_SERVER_REWARD_CFG_COUNT_LIMIT)
			{
				RandActivityNiuEggServerRewardConfig::RANiuEggConfigItem *cfg = &m_niu_egg_server_reward_cfg_list[i].cfg_list[req];
				if (server_total_niu_times >= cfg->server_niu_times)
				{
					return cfg;
				}
			}
		}
	}

	return NULL;
}

const RandActivityNiuEggChouRewardConfig::RANiuEggConfigItem * RandActivityConfig::GetNiuRewardCfgWithReq(int req)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_niu_egg_chou_reward_cfg_list[i].opengame_day)
		{
			continue;	
		}

		if (req >= 0 && req < m_niu_egg_chou_reward_cfg_count && req < RA_NIU_EGG_CHOU_REWARD_CFG_COUNT_LIMIT && req < m_niu_egg_chou_reward_cfg_list[i].cfg_count)
		{
			return &m_niu_egg_chou_reward_cfg_list[i].cfg_list[req];
		}
	}

	return NULL;
}

const RandActivityNiuEggServerRewardConfig::RANiuEggConfigItem * RandActivityConfig::GetNiuServerCfgWithReq(int req)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG);

	for (int i= 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_niu_egg_server_reward_cfg_list[i].opengame_day)
		{
			if (req >= 0 && req < m_niu_egg_server_reward_cfg_count && req < RA_NIU_EGG_SERVER_REWARD_CFG_COUNT_LIMIT)
			{
				return &m_niu_egg_server_reward_cfg_list[i].cfg_list[req];
			}

		}
	}

	return NULL;
}

int RandActivityConfig::GetDayChongZhiFanLiRewardCfgCount()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_DAY_CHONGZHI_FANLI);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i) 
	{
		if (opengame_day <= m_day_chongzhi_fanli_cfg[i].opengame_day)
		{
			return m_day_chongzhi_fanli_cfg[i].cfg_count;
		}
	}

	return  -1;
}

int RandActivityConfig::GetDayConsumeGoldRewardCfgCount()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_DAY_CONSUME_GOLD);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_day_consume_gold_cfg[i].opengame_day)
		{
			return m_day_consume_gold_cfg[i].cfg_count;
		}
	}

	return -1;
}

int RandActivityConfig::GetDayActiveDegreeCfgRewardCfgCount()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_DAY_ACTIVIE_DEGREE);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_day_active_degree_reward_cfg[i].opengame_day)
		{
			return m_day_active_degree_reward_cfg[i].cfg_count;
		}
	}

	return -1;
}

int RandActivityConfig::GetDayOnlineGiftCfgCount()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_DAY_ONLINE_GIFT);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_day_online_gift_cfg[i].opengame_day)
		{
			return m_day_online_gift_cfg[i].cfg_count;
		}
	}

	return -1;
}

int RandActivityConfig::InitZhenbaogeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_zhenbaoge_max_seq = 0;
	m_zhenbaoge_reward_max_seq = 0;
	m_zhenbaoge_auto_add_cfg_count = 0;

	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			stage++;
			last_opengame_day = opengame_day;
			m_zhenbaoge_max_seq = stage + 1;
			m_zhenbaoge_reward_max_seq = stage + 1;
			m_zhenbaoge_auto_add_cfg_count = stage + 1;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		int type = 0;
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "cfg_type", type) || type < 0 || type > 1)
		{
			return -1;
		}

		if (type <= 0)		// 刷新物品	
		{
			RandActivityZhenBaoGeDayCfg &day_cfg = m_zhenbaoge_cfg_list[stage];
			++day_cfg.cfg_count;
			day_cfg.opengame_day = opengame_day;

			if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq <= 0 || seq > RAND_ACTIVITY_ZHENBAOGE_MAX_SEQ)
			{
				return -11;
			}

			RandActivityZhenBaoGeConfig &zhenbaoge_cfg = day_cfg.cfg_list[seq];
			zhenbaoge_cfg.seq = seq;
			m_zhenbaoge_max_seq = seq;

			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -12;
			}

			if (!zhenbaoge_cfg.reward_item.ReadConfig(reward_item_element))
			{
				return -13;
			}

			if (!PugiGetSubNodeValue(data_element, "is_rare", zhenbaoge_cfg.is_rare) || zhenbaoge_cfg.is_rare < 0 || zhenbaoge_cfg.is_rare > 1)
			{
				return -14;
			}

			if (!PugiGetSubNodeValue(data_element, "weight", zhenbaoge_cfg.weight) || zhenbaoge_cfg.weight < 0)
			{
				return -15;
			}

			if (!PugiGetSubNodeValue(data_element, "auto_add_weight", zhenbaoge_cfg.auto_add_weight) || zhenbaoge_cfg.auto_add_weight < 0)
			{
				return -16;
			}

			if (!PugiGetSubNodeValue(data_element, "buy_consume_gold", zhenbaoge_cfg.buy_consume_gold) || zhenbaoge_cfg.buy_consume_gold <= 0)
			{
				return -17;
			}

			if (zhenbaoge_cfg.auto_add_weight > 0)
			{
				RandActivityZhenBaoGeAddWeightDayCfg &weight_day_cfg = m_zhenbaoge_add_weight_cfg_list[stage];
				weight_day_cfg.opengame_day = opengame_day;

				if (weight_day_cfg.cfg_count >= RAND_ACTIVITY_ZHENBAOGE_AUTO_ADD_WEIGHT_CFG_COUNT || weight_day_cfg.cfg_count < 0)
				{
					return -18;
				}

				RandActivityZhenBaoGeAddWeightConfig &weight_cfg = weight_day_cfg.cfg_list[weight_day_cfg.cfg_count];
				weight_cfg.add_weight = zhenbaoge_cfg.auto_add_weight;
				weight_cfg.seq = zhenbaoge_cfg.seq;

				++weight_day_cfg.cfg_count;
			}

			int baodi = 0;
			if (PugiGetSubNodeValue(data_element, "baodi", baodi) || baodi > 0)
			{
				if (day_cfg.baodi_map.size() > 0 || day_cfg.baodi_map.find(baodi) != day_cfg.baodi_map.end())
				{
					return -19;
				}

				day_cfg.baodi_map.insert(std::make_pair(baodi, seq));
			}
		}
		else	// 全服奖励
		{
			RandActivityZhenBaoGeServerDayCfg &day_cfg = m_zhenbaoge_reward_cfg_list[stage];
			++day_cfg.cfg_count;
			day_cfg.opengame_day = opengame_day;

			if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq <= 0 || seq > RAND_ACTIVITY_ZHENBAOGE_MAX_RETURN_REWARD_COUNT)
			{
				return -51;
			}

			if (seq < 0 || seq > RAND_ACTIVITY_ZHENBAOGE_SERVER_MAX_SEQ)
			{
				return -52;
			}

			RandActivityZhenBaoGeServerConfig &server_cfg = day_cfg.cfg_list[seq];
			server_cfg.seq = seq;

			if (!PugiGetSubNodeValue(data_element, "can_fetch_times", server_cfg.can_fetch_times) || server_cfg.can_fetch_times <= 0)
			{
				return -53;
			}

			if (!PugiGetSubNodeValue(data_element, "vip_limit", server_cfg.vip_limit) || server_cfg.vip_limit < 0 || server_cfg.vip_limit > MAX_VIP_LEVEL)
			{
				return -54;
			}

			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -55;
			}

			if (!server_cfg.reward_item.ReadConfig(reward_item_element))
			{
				return -56;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityZhenBaoGeConfig * RandActivityConfig::GetZhenbaogeCfg(int seq)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_ZHENBAOGE);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_zhenbaoge_max_seq; ++i)
	{
		if (m_zhenbaoge_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityZhenBaoGeDayCfg &day_cfg = m_zhenbaoge_cfg_list[idx];

	if (seq <= 0 || seq > day_cfg.cfg_count || seq > RAND_ACTIVITY_ZHENBAOGE_MAX_SEQ)
	{
		return NULL;
	}

	return &day_cfg.cfg_list[seq];
}

const RandActivityZhenBaoGeAddWeightConfig * RandActivityConfig::GetZhenbaogeAddWeight(int index)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_ZHENBAOGE);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_zhenbaoge_auto_add_cfg_count; ++i)
	{
		if (m_zhenbaoge_add_weight_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityZhenBaoGeAddWeightDayCfg &day_cfg = m_zhenbaoge_add_weight_cfg_list[idx];

	if (index < 0 || index >= day_cfg.cfg_count || index >= RAND_ACTIVITY_ZHENBAOGE_AUTO_ADD_WEIGHT_CFG_COUNT)
	{
		return NULL;
	}

	return &day_cfg.cfg_list[index];
}

const RandActivityZhenBaoGeServerConfig * RandActivityConfig::GetZhenbaogeServerCfg(int seq)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_ZHENBAOGE);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_zhenbaoge_reward_max_seq; ++i)
	{
		if (m_zhenbaoge_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityZhenBaoGeServerDayCfg &day_cfg = m_zhenbaoge_reward_cfg_list[idx];
	if (seq <= 0 || seq > day_cfg.cfg_count || seq > RAND_ACTIVITY_ZHENBAOGE_SERVER_MAX_SEQ)
	{
		return NULL;
	}

	return &day_cfg.cfg_list[seq];
}

bool RandActivityConfig::GetZhenbaogeRandomResult(const ZhenbaogeAddWeightInfo *self_weight, int length, int is_free, RandActivityZhenBaoGeConfig *result_list, int result_list_length, int server_chou_times)
{
	if (NULL == self_weight || length < 0 || NULL == result_list || result_list_length <= 0)
	{
		return false;
	}

	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_ZHENBAOGE);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_zhenbaoge_max_seq; ++i)
	{
		if (m_zhenbaoge_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityZhenBaoGeDayCfg &day_cfg = m_zhenbaoge_cfg_list[idx];

	int all_weight = 0;
	int temp_list[RAND_ACTIVITY_ZHENBAOGE_MAX_SEQ + 1] = { 0 };

	for (int i = 0; i <= day_cfg.cfg_count && i <= RAND_ACTIVITY_ZHENBAOGE_MAX_SEQ; ++i)
	{
		temp_list[i] = day_cfg.cfg_list[i].weight;

		if (!is_free)	// 非免费刷新 增加自己权重
		{
			for (int self_index = 0; self_index < length; ++self_index)
			{
				if (self_weight[self_index].seq == day_cfg.cfg_list[i].seq)
				{
					temp_list[i] += self_weight[self_index].weight;
				}
			}
		}

		all_weight += temp_list[i];
	}

	if (all_weight <= 0)
	{
		return false;
	}

	int get_count = 0;

	// 保底次数
	if (server_chou_times > 0)
	{
		for (std::map<int, int>::iterator it = day_cfg.baodi_map.begin(); it != day_cfg.baodi_map.end(); ++it)
		{
			const int baodi_times = it->first;
			const int reward_seq = it->second;

			if (baodi_times > 0 && 0 == server_chou_times % baodi_times && reward_seq >= 0 && reward_seq <= day_cfg.cfg_count && reward_seq <= RAND_ACTIVITY_ZHENBAOGE2_MAX_SEQ)
			{
				result_list[get_count] = day_cfg.cfg_list[reward_seq];
				++get_count;
				break;
			}
		}
	}

	for (; get_count < result_list_length; ++get_count)
	{
		int rand_weight = RandomNum(all_weight);

		for (int i = 0; i <= day_cfg.cfg_count && i <= RAND_ACTIVITY_ZHENBAOGE_MAX_SEQ; ++i)
		{
			if (rand_weight < temp_list[i])
			{
				result_list[get_count] = day_cfg.cfg_list[i];
				all_weight -= temp_list[i];
				temp_list[i] = 0;

				break;
			}

			rand_weight -= temp_list[i];
		}
	}

	return true;
}


int RandActivityConfig::InitChargeRewardGold(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_type = 0;
	int last_high = 0;

	while (!data_element.empty())
	{
		RandActivityChargeRewardGoldConfig cfg;

		if (!PugiGetSubNodeValue(data_element, "type", cfg.type) ||  cfg.type < 0 || cfg.type > 1)
		{
			return -1;
		}

		if (last_type != cfg.type)
		{
			last_type = cfg.type;
			last_high = 0;
		}
		

		if (!PugiGetSubNodeValue(data_element, "gold_low_limit", cfg.gold_low_limit) ||  cfg.gold_low_limit < 0 || cfg.gold_low_limit != last_high + 1)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "gold_high_limit", cfg.gold_high_limit) ||  cfg.gold_high_limit < cfg.gold_low_limit)
		{
			return -3;
		}
		last_high = cfg.gold_high_limit;

		if (!PugiGetSubNodeValue(data_element, "reward_precent", cfg.reward_precent) ||  cfg.reward_precent < 0 || cfg.type > 100)
		{
			return -4;
		}

		m_charge_reward_gold_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitMiJingXunBaoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int is_rare_temp = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (last_opengame_day != opengame_day)
		{
			last_opengame_day = opengame_day;
			++ stage;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (m_mijingxunbao_max_seq_list[stage] >= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ)
		{
			return -1;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_mijingxunbao_max_seq_list[stage] + 1)
		{
			return -11;
		}

		m_mijingxunbao_cfg_list[stage].opengame_day = opengame_day;
		RandActivityMiJingXunBaoConfig &mijingxunbao_cfg = m_mijingxunbao_cfg_list[stage].cfg_list[seq];
		mijingxunbao_cfg.seq = seq;
		m_mijingxunbao_max_seq_list[stage] = seq;

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -12;
		}

		if (!mijingxunbao_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -13;
		}

		is_rare_temp = 0;
		if (!PugiGetSubNodeValue(data_element, "is_rare", is_rare_temp) || is_rare_temp < 0 || is_rare_temp > 1)
		{
			return -14;
		}
		mijingxunbao_cfg.is_rare = (0 != is_rare_temp);

		if (!PugiGetSubNodeValue(data_element, "weight", mijingxunbao_cfg.weight) || mijingxunbao_cfg.weight < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(data_element, "auto_add_weight", mijingxunbao_cfg.auto_add_weight) || mijingxunbao_cfg.auto_add_weight < 0)
		{
			return -16;
		}

		if (mijingxunbao_cfg.auto_add_weight > 0)
		{
			if (m_mijingxunbao_auto_add_cfg_count_list[stage] >= RAND_ACTIVITY_MIJINGXUNBAO_AUTO_ADD_WEIGHT_CFG_COUNT)
			{
				return -18;
			}

			m_mijingxunbao_add_weight_cfg_list[stage].opengame_day = opengame_day;
			m_mijingxunbao_add_weight_cfg_list[stage].add_weight_cfg_list[m_mijingxunbao_auto_add_cfg_count_list[stage]].add_weight = mijingxunbao_cfg.auto_add_weight;
			m_mijingxunbao_add_weight_cfg_list[stage].add_weight_cfg_list[m_mijingxunbao_auto_add_cfg_count_list[stage]].seq = mijingxunbao_cfg.seq;

			++ m_mijingxunbao_auto_add_cfg_count_list[stage];
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitMiJingXunBaoRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int max_index = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			stage++;
			last_opengame_day = opengame_day;
			m_mijingxunbao_reward_cfg_count = stage + 1;
			max_index = -1;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		int index = 0;
		if (!PugiGetSubNodeValue(data_element, "index", index) || index != max_index + 1 || index >= RAND_ACTIVITY_MIJINGXUNBAO_REWARD_MAX_COUNT)
		{
			return -1;
		}

		RandActivityMiJingXunBaoRewardDayCfg &day_cfg = m_mijingxunbao_reward_list_cfg[stage];
		++day_cfg.cfg_count;
		day_cfg.opengame_day = opengame_day;

		RandActivityMiJingXunBaoRewardConfig &reward_list_cfg = day_cfg.cfg_list[index];
		reward_list_cfg.index = index;
		max_index = index;

		if (!PugiGetSubNodeValue(data_element, "choujiang_times", reward_list_cfg.choujiang_times) || reward_list_cfg.choujiang_times < 0)
		{
			return -2;
		}

		{
			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -3;
			}

			if (!reward_list_cfg.reward_item.ReadConfig(reward_item_element))
			{
				return -4;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

//type = 0 为首冲 1 为非首冲
int RandActivityConfig::GetChargeRewardPrecent(const int type, const int chongzhi_num)
{
	if (chongzhi_num <= 0)
	{
		return 0;
	}

	ChargeRewardGoldVec::const_iterator iter = m_charge_reward_gold_vec.begin();
	for (; iter != m_charge_reward_gold_vec.end();  iter ++)
	{
		if (iter->gold_low_limit <= chongzhi_num && chongzhi_num <= iter->gold_high_limit && type == iter->type)
		{
			return iter->reward_precent;
		}
	}

	//这里处理的是：首冲但金额超出首冲返100%的额度
	iter = m_charge_reward_gold_vec.begin();
	for (; iter != m_charge_reward_gold_vec.end();  iter ++)
	{
		if (iter->gold_low_limit <= chongzhi_num && chongzhi_num <= iter->gold_high_limit && type != iter->type)
		{
			return iter->reward_precent;
		}
	}

	return -1;
}

int RandActivityConfig::GetMiJingXunBaoAddWeightSeq(int index)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MIJINGXUNBAO);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_mijingxunbao_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	if (index < 0 || index >= m_mijingxunbao_auto_add_cfg_count_list[stage])
	{
		return 0;
	}

	return m_mijingxunbao_add_weight_cfg_list[stage].add_weight_cfg_list[index].seq;
}

int RandActivityConfig::GetMiJingXunBaoCfgAddWeight(int seq)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MIJINGXUNBAO);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_mijingxunbao_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	if (seq <= 0 || seq > m_mijingxunbao_max_seq_list[stage] || seq > RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ)
	{
		return 0;
	}

	for (int index = 0; index < m_mijingxunbao_auto_add_cfg_count_list[stage]; index++)
	{
		if (m_mijingxunbao_add_weight_cfg_list[stage].add_weight_cfg_list[index].seq == seq)
		{
			return m_mijingxunbao_add_weight_cfg_list[stage].add_weight_cfg_list[index].add_weight;
		}
	}

	return 0;
}

int RandActivityConfig::GetMiJingXunBaoAutoWeightAddCount()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MIJINGXUNBAO);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_mijingxunbao_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	return m_mijingxunbao_auto_add_cfg_count_list[stage];
}

int RandActivityConfig::GetMiJingXunBaoTaoGold(int type)
{
	if (type < RA_MIJINGXUNBAO_CHOU_TYPE_1 || type >= RA_MIJINGXUNBAO_CHOU_TYPE_MAX)
	{
		return 0;
	}

	switch(type)
	{
	case RA_MIJINGXUNBAO_CHOU_TYPE_1 : 
		{
			return m_mijingxunbao_once_gold;
		}
		break;

	case RA_MIJINGXUNBAO_CHOU_TYPE_10 : 
		{
			return m_mijingxunbao_tentimes_gold;
		}
		break;

	case RA_MIJINGXUNBAO_CHOU_TYPE_50 : 
		{
			return m_mijingxunbao_fifthtimes_gold;
		}
		break;

	default:
		return 0;
	}
}

bool RandActivityConfig::GetMiJingXunBaoRandomResult(MiJingXunBaoAddWeightInfo *self_weight, int length, int is_free, RandActivityMiJingXunBaoConfig *result_list, int result_list_length)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MIJINGXUNBAO);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_mijingxunbao_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return false;
	}

	if (NULL == self_weight || length < 0 || NULL == result_list || result_list_length <= 0)
	{
		return false;
	}

	int all_weight = 0;
	int all_cfg_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ + 1] = {0};

	for (int i = 1; i <= m_mijingxunbao_max_seq_list[stage] && i <= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ; ++ i)
	{
		all_cfg_weight_list[i] = m_mijingxunbao_cfg_list[stage].cfg_list[i].weight;

		if (!is_free)	// 非免费刷新 增加自己权重
		{
			for (int self_index = 0; self_index < length; ++ self_index)
			{
				if (self_weight[self_index].seq == m_mijingxunbao_cfg_list[stage].cfg_list[i].seq)
				{
					all_cfg_weight_list[i] += self_weight[self_index].weight;
				}
			}
		}

		all_weight += all_cfg_weight_list[i];
	}

	if (all_weight <= 0)
	{
		return false;
	}

	for (int result_index = 0; result_index < result_list_length && all_weight > 0; ++ result_index)
	{
		int rand_weight = RandomNum(all_weight);
		for (int i = 1; i <= m_mijingxunbao_max_seq_list[stage] && i <= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ; ++ i)
		{
			if (rand_weight < all_cfg_weight_list[i])
			{
				result_list[result_index] = m_mijingxunbao_cfg_list[stage].cfg_list[i];

				for (int self_index = 0; self_index < length; ++ self_index)
				{
					if (result_list[result_index].seq == self_weight[self_index].seq) //1 抽到珍稀，把该奖励权重置0
					{
						all_cfg_weight_list[self_weight[self_index].seq] -= self_weight[self_index].weight;
						all_weight -= self_weight[self_index].weight;
						if (all_weight <= 0)
						{
							return false;
						}

						self_weight[self_index].weight = 0;
						self_weight[self_index].tao_none_rare_times = 0;
					}
					else
					{
						self_weight[self_index].tao_none_rare_times ++;
						int add_weight = this->GetMiJingXunBaoCfgAddWeight(self_weight[self_index].seq);

						if (self_weight[self_index].tao_none_rare_times >= this->GetMiJingXunBaoAddWeightTaoTimes())
						{
							all_weight += add_weight;

							self_weight[self_index].weight += add_weight;
							all_cfg_weight_list[self_weight[self_index].seq] += self_weight[self_index].weight;

							self_weight[self_index].tao_none_rare_times -= this->GetMiJingXunBaoAddWeightTaoTimes();
						}
					}
				}
				break;
			}

			rand_weight -= all_cfg_weight_list[i];
		}
	}

	return true;
}

const RandActivityMiJingXunBaoRewardConfig * RandActivityConfig::GetRandActivityMiJingXunBaoPersonalRewardCfg(int seq, int chou_times)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MIJINGXUNBAO);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_mijingxunbao_reward_cfg_count; ++i)
	{
		if (m_mijingxunbao_reward_list_cfg[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityMiJingXunBaoRewardDayCfg &day_cfg = m_mijingxunbao_reward_list_cfg[idx];

	if (seq < 0 || seq >= RAND_ACTIVITY_MIJINGXUNBAO_REWARD_MAX_COUNT || chou_times < 0)
	{
		return NULL;
	}

	if (chou_times >= day_cfg.cfg_list[seq].choujiang_times && day_cfg.cfg_list[seq].choujiang_times != 0)
	{
		return &day_cfg.cfg_list[seq];
	}

	return NULL;
}

int RandActivityConfig::InitJiSuChongZhanCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int stage_num = -1;
	int seq = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int last_seq = -1;
	int last_charge_value = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -1;
		}

		if (last_opengame_day != opengame_day)
		{
			last_opengame_day = opengame_day;
			++ stage_num;
			last_seq = -1;
			last_charge_value = 0;
		}

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -100;
		}

		RandActivityJiSuChongZhanCfg *cfg = &m_jisuchongzhan_cfg_list[stage_num];
		cfg->opengame_day = opengame_day;


		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq != last_seq + 1 || seq >= RAND_ACTIVITY_JISUCHONGZHAN_CFG_NUM)
		{
			return -2;
		}
		last_seq = seq;

		RandActivityJiSuChongZhanConfig &reward_cfg = cfg->cfg_list[seq];
		reward_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "charge_value",reward_cfg.charge_value) || reward_cfg.charge_value < 0 || reward_cfg.charge_value <= last_charge_value)
		{
			return -3;
		}
		last_charge_value = reward_cfg.charge_value;
		
		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -4;
		}

		if (!reward_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -5;
		}

		data_element = data_element.next_sibling();

	}

	return 0;
}

const RandActivityJiSuChongZhanConfig * RandActivityConfig::GetJiSuChongZhanRewardCfg(const int charge_value)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_JISUCHPONGZHAN);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_jisuchongzhan_cfg_list[i].opengame_day)
		{
			continue;
		}

		for (int j = RAND_ACTIVITY_JISUCHONGZHAN_CFG_NUM - 1; j >= 0; -- j)
		{
			const int num = m_jisuchongzhan_cfg_list[i].cfg_list[j].charge_value;

			if (charge_value < num || 0 == num)
			{
				continue;
			}

			return &m_jisuchongzhan_cfg_list[i].cfg_list[j];
		}
	}

	return NULL;
}

int RandActivityConfig::InitMoneyTreeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int stage = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	m_money_tree_max_stage = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			last_opengame_day = opengame_day;
			++ stage;
			++ m_money_tree_max_stage;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		int cfg_type = -1;
		if (!PugiGetSubNodeValue(data_element, "cfg_type", cfg_type) || cfg_type < 0 || cfg_type > 1)
		{
			return -20;
		}

		if (0 == cfg_type)
		{
			if (m_money_tree_reward_cfg_count_list[stage] >= RA_MONEY_TREE_REWARD_CFG_COUNT)
			{
				return -21;
			}

			int seq = -1;
			if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_money_tree_reward_cfg_count_list[stage])
			{
				return -1;
			}

			m_money_tree_reward_cfg_list[stage].opengame_day = opengame_day;
			RandActivityMoneyTreeRewardConfig &reward_cfg = m_money_tree_reward_cfg_list[stage].reward_cfg_list[seq];
			reward_cfg.seq = seq;

			if (!PugiGetSubNodeValue(data_element, "weight", reward_cfg.weight) || reward_cfg.weight < 0)
			{
				return -2;
			}

			int is_broadcast = 0;
			if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast) || is_broadcast < 0 || is_broadcast > 1)
			{
				return -3;
			}
			reward_cfg.is_broadcast = (is_broadcast > 0);

			int has_reward = false;
			if (PugiGetSubNodeValue(data_element, "prize_pool_percent", reward_cfg.prize_pool_percent))
			{
				if (reward_cfg.prize_pool_percent < 0 || reward_cfg.prize_pool_percent > 100)
				{
					return -5;
				}

				has_reward = true;
			}

			m_money_tree_total_weight_list[stage] += reward_cfg.weight;

			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -4;
			}

			if (reward_cfg.reward_item.ReadConfig(reward_item_element))
			{
				if (has_reward)
				{
					return -6;
				}

				has_reward = true;
				m_money_tree_item_reward_total_weight_list[stage] += reward_cfg.weight;
			}

			if (!has_reward)
			{
				return -7;
			}

// 			int baodi = 0;
// 			if (PugiGetSubNodeValue(data_element, "baodi", baodi) && baodi > 0)
// 			{
// 				// 保底次数必须不同,因为保底必出物品（除非保底物品也随机）
// 				if (m_money_tree_reward_cfg_list[stage].baodi_map.find(baodi) != m_money_tree_reward_cfg_list[stage].baodi_map.end()
// 					|| m_money_tree_reward_cfg_list[stage].baodi_map.size() > 0)
// 				{
// 					return -9;
// 				}
// 
// 				m_money_tree_reward_cfg_list[stage].baodi_map.insert(std::make_pair(baodi, reward_cfg.seq));
// 			}

			++m_money_tree_reward_cfg_count_list[stage];
		}
		else if (1 == cfg_type)		// 全服奖励
		{
			if (m_money_tree_server_reward_cfg_count_list[stage] >= RA_MONEY_TREE_SERVER_REWARD_CFG_COUNT)
			{
				return -22;
			}

			int seq = -1;
			if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_money_tree_server_reward_cfg_count_list[stage])
			{
				return -10;
			}

			m_money_tree_server_reward_cfg_list[stage].opengame_day = opengame_day;
			RandActivityMoneyTreeServerRewardConfig &server_reward_cfg = m_money_tree_server_reward_cfg_list[stage].server_reward_cfg_list[seq];
			server_reward_cfg.seq = seq;

			if (!PugiGetSubNodeValue(data_element, "server_rock_times", server_reward_cfg.server_rock_times) || server_reward_cfg.server_rock_times <= 0)
			{
				return -11;
			}

			if (!PugiGetSubNodeValue(data_element, "vip_limit", server_reward_cfg.vip_limit) || server_reward_cfg.vip_limit < 0 || server_reward_cfg.vip_limit > MAX_VIP_LEVEL)
			{
				return -14;
			}

			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -15;
			}

			if (!server_reward_cfg.reward_item.ReadConfig(reward_item_element))
			{
				return -16;
			}

			++ m_money_tree_server_reward_cfg_count_list[stage];
		}

		data_element = data_element.next_sibling();
	}

	if (m_money_tree_max_stage < 1)
	{
		return -1111111;
	}

	for (int i = 0; i < m_money_tree_max_stage; ++ i)
	{
		if (m_money_tree_total_weight_list[stage] <= 0)
		{
			return -1000;
		}

		if (m_money_tree_item_reward_total_weight_list[stage] <= 0)
		{
			return -2000;
		}
	}

	return 0;
}

int RandActivityConfig::InitMoneyTreeBaodiCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100;
	}

	int last_opengame_day = -1;
	int stage = -1;
	int last_seq = 0;
	while (!data_element.empty())
	{
		int baodi = 0;
		if (!PugiGetSubNodeValue(data_element, "baodi", baodi) || baodi <= 0)
		{
			data_element = data_element.next_sibling();
			continue;
		}

		int open_game_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", open_game_day) || open_game_day <= 0)
		{
			return -1;
		}

		if (open_game_day != last_opengame_day)
		{
			last_opengame_day = open_game_day;
			stage++;
			last_seq = 0;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -100;
		}

		RandActivityServerBaodiCfgList & baodi_cfg = m_money_tree_baodi_cfg_list[stage];

		if (baodi_cfg.opengame_day != 0 && baodi_cfg.opengame_day != open_game_day)
		{
			return -101;
		}

		if (baodi_cfg.baodi != 0 && baodi_cfg.baodi != baodi)
		{
			return -102;
		}

		baodi_cfg.opengame_day = open_game_day;
		baodi_cfg.baodi = baodi;

		RandActivityServerBaodiConfig baodi_item;

		if (!PugiGetSubNodeValue(data_element, "seq", baodi_item.seq) || baodi_item.seq < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "baodi_weight", baodi_item.weight) || baodi_item.weight < 0)
		{
			return -4;
		}
		
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", baodi_item.is_rare))
		{
			return -5;
		}

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -6;
		}

		if (!baodi_item.reward_item.ReadConfig(reward_item_element))
		{
			return -7;
		}
		
		if (last_seq != 0 && last_seq + 1 != baodi_item.seq)
		{
			return -103;
		}
		last_seq = baodi_item.seq;

		baodi_cfg.server_baodi_vec.push_back(baodi_item);
		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityMoneyTreeRewardConfig * RandActivityConfig::GetRandomMoneyTreeRewardCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MONEY_TREE);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_money_tree_reward_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	if (m_money_tree_total_weight_list[stage] <= 0) return NULL;

	int random_weight = RandomNum(m_money_tree_total_weight_list[stage]);
	for (int req = 0; req < m_money_tree_reward_cfg_count_list[stage] && req < RA_MONEY_TREE_REWARD_CFG_COUNT; ++ req)
	{
		RandActivityMoneyTreeRewardConfig &cfg = m_money_tree_reward_cfg_list[stage].reward_cfg_list[req];
		if (random_weight < cfg.weight)
		{
			return &cfg;
		}

		random_weight -= cfg.weight;
	}

	return NULL;
}

const RandActivityMoneyTreeRewardConfig * RandActivityConfig::GetRandomMoneyTreeItemRewardCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MONEY_TREE);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_money_tree_reward_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	if (m_money_tree_item_reward_total_weight_list[stage] <= 0) return NULL;

	int random_weight = RandomNum(m_money_tree_item_reward_total_weight_list[stage]);
	for (int req = 0; req < m_money_tree_reward_cfg_count_list[stage] && req < RA_MONEY_TREE_REWARD_CFG_COUNT; ++ req)
	{
		RandActivityMoneyTreeRewardConfig &cfg = m_money_tree_reward_cfg_list[stage].reward_cfg_list[req];
		if (cfg.prize_pool_percent > 0)
		{
			continue;
		}

		if (random_weight < cfg.weight)
		{
			return &cfg;
		}

		random_weight -= cfg.weight;
	}

	return NULL;
}

const RandActivityMoneyTreeRewardConfig * RandActivityConfig::GetMoneyTreeBaodiRewardCfg(int server_chou_times)
{
	if (server_chou_times <= 0) return nullptr;

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MONEY_TREE);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_money_tree_reward_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	for (std::map<int, int>::iterator it = m_money_tree_reward_cfg_list[stage].baodi_map.begin(); it != m_money_tree_reward_cfg_list[stage].baodi_map.end(); ++it)
	{
		const int baodi_times = it->first;
		const int reward_seq = it->second;

		if (baodi_times <= 0 || 0 != server_chou_times % baodi_times) continue;

		if (reward_seq < 0 || reward_seq >= RA_MONEY_TREE_REWARD_CFG_COUNT || m_money_tree_reward_cfg_list[stage].reward_cfg_list[reward_seq].seq != reward_seq) continue;

		return &m_money_tree_reward_cfg_list[stage].reward_cfg_list[reward_seq];
	}

	return NULL;
}

const RandActivityServerBaodiConfig*RandActivityConfig::GetMoneyTreeBaodiRandReward(int server_chou_times)
{
	if (server_chou_times <= 0) return nullptr;

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MONEY_TREE);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day <= m_money_tree_baodi_cfg_list[i].opengame_day)
		{
			if (server_chou_times >= m_money_tree_baodi_cfg_list[i].baodi)
			{
				stage = i;
				break;
			}
			else
			{
				return NULL;
			}
		}
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	return gamecommon::RandListItem(m_money_tree_baodi_cfg_list[stage].server_baodi_vec);
}

const RandActivityMoneyTreeServerRewardConfig * RandActivityConfig::GetMoneyTreeServerCfgWithReq(int req, int join_day)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MONEY_TREE);
	if (join_day > 0)
	{
		opengame_day = join_day;
	}

	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_money_tree_server_reward_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	if (req >= 0 && req < m_money_tree_server_reward_cfg_count_list[stage] && req < RA_MONEY_TREE_SERVER_REWARD_CFG_COUNT)
	{
		return &m_money_tree_server_reward_cfg_list[stage].server_reward_cfg_list[req];
	}

	return NULL;
}

const RandActivityMoneyTreeServerRewardConfig * RandActivityConfig::GetTreeServerCfgWithServerMoneyTreeTimes(int server_total_money_tree_times, int join_day)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MONEY_TREE);
	if (join_day > 0)
	{
		opengame_day = join_day;
	}

	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_money_tree_server_reward_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	for (int req = m_money_tree_server_reward_cfg_count_list[stage] - 1; req >= 0; -- req)
	{
		RandActivityMoneyTreeServerRewardConfig &cfg = m_money_tree_server_reward_cfg_list[stage].server_reward_cfg_list[req];
		if (server_total_money_tree_times >= cfg.server_rock_times)
		{
			return &cfg;
		}
	}

	return NULL;
}

int RandActivityConfig::InitKingDrawLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;
	int level = 0;
	int old_level = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			stage++;
			last_opengame_day = opengame_day;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (level != old_level)
		{
			stage = 0;
			old_level = level;
		}

		RandActivityKingDrawLevelConfig &reward_cfg_list = m_kingdraw_level_cfg_list[level][stage];
		reward_cfg_list.opengame_day = opengame_day;

		RandActivityKingDrawLevelStageConfig &level_cfg = reward_cfg_list.level_cfg;

		if (level_cfg.m_reward_count < 0 || level_cfg.m_reward_count >= RandActivityKingDrawLevelStageConfig::KING_DRAW_REWARD_COUNT_PER_STAGE)
		{
			return -1001;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != level_cfg.m_reward_count)
		{
			return -2;
		}

		RandActivityKingDrawRewardConfig &reward_cfg = level_cfg.reward_list[level_cfg.m_reward_count];
		reward_cfg.seq = seq;

		int baodi = 0;
		if (PugiGetSubNodeValue(data_element, "baodi", baodi) && baodi > 0)
		{
			// 保底次数必须不同,因为保底必出物品（除非保底物品也随机）
			if (reward_cfg_list.baodi_map.find(baodi) != reward_cfg_list.baodi_map.end()
				|| reward_cfg_list.baodi_map.size() > 0)
			{
				return -9;
			}

			reward_cfg_list.baodi_map.insert(std::make_pair(baodi, reward_cfg.seq));
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -3;
		}

		if (!reward_cfg.reward_item.ReadConfig(item_element))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", reward_cfg.weight) || reward_cfg.weight < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "auto_add_weight", reward_cfg.auto_add_weight) || reward_cfg.auto_add_weight < 0)
		{
			return -6;
		}

		if (reward_cfg.auto_add_weight > 0)
		{
			if (level_cfg.m_kingdraw_self_weigth_count >= RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT)
			{
				return -1002;
			}

			level_cfg.m_kingdraw_self_weight_cfg_list[level_cfg.m_kingdraw_self_weigth_count].seq = reward_cfg.seq;
			level_cfg.m_kingdraw_self_weight_cfg_list[level_cfg.m_kingdraw_self_weigth_count].auto_add_weight = reward_cfg.auto_add_weight;

			++ level_cfg.m_kingdraw_self_weigth_count;
		}

		int is_rare = -1;
		if (!PugiGetSubNodeValue(data_element, "is_rare", is_rare) || is_rare < 0 || is_rare > 1)
		{
			return -8;
		}
		reward_cfg.is_rare = (0 != is_rare);

		++ level_cfg.m_reward_count;
		data_element = data_element.next_sibling();
	}

	for (int draw_level = 0; draw_level < RA_KING_DRAW_LEVEL_COUNT; ++draw_level)
	{
		for (int s = 0; s < stage && s < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++s)
		{
			if (m_kingdraw_level_cfg_list[draw_level][s].level_cfg.m_reward_count <= 0)
			{
				return -2000;
			}
		}
	}

	return 0;
}

int RandActivityConfig::InitKingDrawReturnRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			stage++;
			last_opengame_day = opengame_day;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		int level = -1;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT)
		{
			return -1;
		}

		RandActivityKingDrawReturnRewardCfg &return_cfg_list = m_kingdraw_return_reward_cfg_list[level][stage];
		return_cfg_list.opengame_day = opengame_day;

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= RandActivityKingDrawReturnRewardCfg::KING_DRAW_RETURN_REWARD_COUNT_PER_STAGE || seq != return_cfg_list.cfg_count)
		{
			return -2;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -3;
		}

		if (!return_cfg_list.cfg_list[seq].reward_item.ReadConfig(item_element))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "draw_times", return_cfg_list.cfg_list[seq].draw_times)
			|| return_cfg_list.cfg_list[seq].draw_times < 0)
		{
			return -5;
		}

		++return_cfg_list.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::GetDailyLoveRewardPrecent(int opengame_day)
{
	for (size_t i = m_daily_love_reward_precent_vec.size() - 1; i >= 0; --i)
	{
		if (opengame_day >= m_daily_love_reward_precent_vec[i].opengame_day)
		{
			return m_daily_love_reward_precent_vec[i].gold_percent;
		}
	}
	return 0;
}

int RandActivityConfig::GetKingDrawPlayOnceGold(int level)
{
	int consume_gold = 0;
	switch(level)
	{
	case 0 :
		consume_gold = m_kingdraw_level_chuji_consume;
		break;

	case 1:
		consume_gold = m_kingdraw_level_zhongji_consume;
		break;

	case 2:
		consume_gold = m_kingdraw_level_gaoji_consume;
		
		break;

	default:
		consume_gold = -1;
		break;
	}

	return consume_gold;
}

//int RandActivityConfig::GetKingDrawPlayOnceScore(int level)
//{
//	int score = 0;
//	switch (level)
//	{
//	case 0:
//		score = m_kingdraw_level_primary_score;
//		break;
//
//	case 1:
//		score = m_kingdraw_level_middle_score;
//		break;
//
//	case 2:
//		score = m_kingdraw_level_high_score;
//		break;
//
//	default:
//		score = 0;
//		break;
//	}
//	return score;
//}


int RandActivityConfig::GetKingDrawSelfWeigthCfgCount(int level)
{
	if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT)
	{
		return 0;
	}

	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_KING_DRAW);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (m_kingdraw_level_cfg_list[level][i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	RandActivityKingDrawLevelStageConfig &cfg_list = m_kingdraw_level_cfg_list[level][idx].level_cfg;

	return cfg_list.m_kingdraw_self_weigth_count;
}

int RandActivityConfig::GetKingDrawReturnRewardCfgCount(int level)
{
	if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT)
	{
		return 0;
	}

	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_KING_DRAW);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (m_kingdraw_return_reward_cfg_list[level][i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	return m_kingdraw_return_reward_cfg_list[level][idx].cfg_count;
}

const RandActivityKingDrawReturnRewardStageCfg * RandActivityConfig::GetKingDrawReturnRewardCfg(int level, int seq, int join_day)
{
	if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT)
	{
		return NULL;
	}

	int idx = -1;
	int opengame_day = 0;
	if (join_day > 0)
	{
		opengame_day = join_day;
	}
	else
	{
		opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_KING_DRAW);
	}

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (m_kingdraw_return_reward_cfg_list[level][i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityKingDrawReturnRewardCfg &return_cfg_list = m_kingdraw_return_reward_cfg_list[level][idx];
	if (seq >= 0 && seq < return_cfg_list.cfg_count && seq < RandActivityKingDrawReturnRewardCfg::KING_DRAW_RETURN_REWARD_COUNT_PER_STAGE)
	{
		return &return_cfg_list.cfg_list[seq];;
	}

	return NULL;
}

const RandActivityKingDrawRewardConfig* RandActivityConfig::GetKingDrawRandomRewardCfg(int level, const RAKingDrawSelfWeight self_weight_list[])
{
	if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT || NULL == self_weight_list) // level: [0, 2]
	{
		return NULL;
	}

	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_KING_DRAW);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (m_kingdraw_level_cfg_list[level][i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	const RandActivityKingDrawLevelStageConfig &level_cfg = m_kingdraw_level_cfg_list[level][idx].level_cfg;	// 当前级别的奖励信息

	if (level_cfg.m_reward_count <= 0)
	{
		return NULL;
	}

	// 临时权重表，普通权重加上自增权重
	int total_weight = 0;
	int temp_weight_list[RandActivityKingDrawLevelStageConfig::KING_DRAW_REWARD_COUNT_PER_STAGE];
	memset(temp_weight_list, 0, sizeof(temp_weight_list));

	for (int seq = 0; seq < level_cfg.m_reward_count && seq < RandActivityKingDrawLevelStageConfig::KING_DRAW_REWARD_COUNT_PER_STAGE; ++ seq)
	{
		temp_weight_list[seq] = level_cfg.reward_list[seq].weight;

		for (int self_index = 0; self_index < RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT; ++self_index)
		{
			if (self_weight_list[self_index].seq == seq)
			{
				temp_weight_list[seq] += self_weight_list[self_index].auto_weight_add;
			}
		}

		total_weight += temp_weight_list[seq];
	}

	// 计算
	double fix_real_rate = RandomNum(10000) * 0.0001;
	int rand_weight = (int)(fix_real_rate * total_weight);

	for (int seq = 0; seq < level_cfg.m_reward_count && seq < RandActivityKingDrawLevelStageConfig::KING_DRAW_REWARD_COUNT_PER_STAGE; ++seq)
	{
		if (temp_weight_list[seq] > rand_weight)
		{
			return &level_cfg.reward_list[seq];
		}

		rand_weight -= temp_weight_list[seq];
	}

	return NULL;
}

const RandActivityKingDrawRewardConfig * RandActivityConfig::GetKingDrawBaodiRewardCfg(int level, const int server_chou_times)
{
	if (server_chou_times <= 0) return nullptr;

	if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT) // level: [0, 2]
	{
		return NULL;
	}

	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_KING_DRAW);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (m_kingdraw_level_cfg_list[level][i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return nullptr;
	}

	RandActivityKingDrawLevelConfig &cur_cfg = m_kingdraw_level_cfg_list[level][idx];

	for (std::map<int, int>::iterator it = cur_cfg.baodi_map.begin(); it != cur_cfg.baodi_map.end(); ++it)
	{
		const int baodi_times = it->first;
		const int reward_seq = it->second;

		if (baodi_times <= 0 || 0 != server_chou_times % baodi_times) continue;

		if (reward_seq < 0 || reward_seq >= RandActivityKingDrawLevelStageConfig::KING_DRAW_REWARD_COUNT_PER_STAGE || cur_cfg.level_cfg.reward_list[reward_seq].seq != reward_seq) continue;

		return &cur_cfg.level_cfg.reward_list[reward_seq];
	}

	return NULL;
}

const RandActivitySelfWeightConfig * RandActivityConfig::GetKingDrawSelfWeightCfg(int level, int index)
{
	if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT || index < 0 || index >= RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT)
	{
		return NULL;
	}

	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_KING_DRAW);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (m_kingdraw_level_cfg_list[level][i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	return &m_kingdraw_level_cfg_list[level][idx].level_cfg.m_kingdraw_self_weight_cfg_list[index];
}

int RandActivityConfig::GetKingDrwaRewardCount(int level)
{
	if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT)
	{
		return 0;
	}

	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_KING_DRAW);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (m_kingdraw_level_cfg_list[level][i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	return m_kingdraw_level_cfg_list[level][idx].level_cfg.m_reward_count;
}

int RandActivityConfig::GetKingDrawOpenGameDay(int level, int index)
{
	//if (level < 0 || level >= RA_KING_DRAW_LEVEL_COUNT || index < 0 || index >= RandActivityKingDrawLevelConfig::KING_DRAW_REWARD_COUNT_PER_LEVEL)
	//{
	//	return 0;
	//}

	//if (index >= m_kingdraw_level_cfg_list[level].m_reward_count)
	//{
	//	return 0;
	//}

	//return m_kingdraw_level_cfg_list[level].reward_list[index].opengame_day;
	return 0;
}

int RandActivityConfig::InitTotalChongzhi3Cfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		int start_day = 0;
		int end_day = 0;
		int opengame_day = 0;

		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			if (opengame_day <= last_opengame_day)
			{
				return -200;
			}

			++ stage;
			last_opengame_day =opengame_day;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -300;
		}

		RandActivityTotalCharge3Config &cfg = m_total_charge_3_cfglist[stage];
		cfg.opengame_day = opengame_day;

		if (!PugiGetSubNodeValue(data_element, "start_day", start_day) || start_day < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "end_day", end_day) || end_day < 0 || end_day < start_day)
		{
			return -2;
		}

		if (cfg.total_cfg_count >= RAND_ACTIVITY_TOTAL_CHARGE_3_CFG_COUNT)
		{
			return -3;
		}

		bool same_with_pre = false; // 是否跟上一个配置是同一个时间段

		if (cfg.total_cfg_count > 0)
		{
			RandActivityTotalCharge3Cfg &pre_total_charge3_cfg = cfg.total_charge_3_cfglist[cfg.total_cfg_count - 1];
			if (pre_total_charge3_cfg.start_day == start_day && pre_total_charge3_cfg.end_day == end_day)
			{
				same_with_pre = true;
			}
			else if(start_day > pre_total_charge3_cfg.end_day)
			{
				same_with_pre = false;
			}
			else
			{
				return -4;
			}
		}

		int cur_index = cfg.total_cfg_count;
		if (same_with_pre)
		{
			cur_index = cfg.total_cfg_count - 1;
		}

		RandActivityTotalCharge3Cfg &total_charge3_cfg = cfg.total_charge_3_cfglist[cur_index];
		total_charge3_cfg.start_day = start_day;
		total_charge3_cfg.end_day = end_day;

		if (total_charge3_cfg.chongzhi_list_count >= RandActivityTotalCharge3Cfg::MAX_CHONGZHI_CFG_COUNT)
		{
			return -5;
		}

		RandActivityTotalCharge3Cfg::ConfigItem &sub_cfg = total_charge3_cfg.chongzhi_list[total_charge3_cfg.chongzhi_list_count];
		sub_cfg.seq = total_charge3_cfg.chongzhi_list_count;

		if (!PugiGetSubNodeValue(data_element, "need_chongzhi_num", sub_cfg.need_chongzhi_num) || sub_cfg.need_chongzhi_num <= 0)
		{
			return -6;
		}

		PugiXmlNode item_element_list = data_element.child("reward_item_list");
		if (item_element_list.empty())
		{
			return -1001;
		}

		PugiXmlNode reward_item_element = item_element_list.child("reward_item");
		if (reward_item_element.empty())
		{
			return -7;
		}

		int reward_count = 0;

		while (!reward_item_element.empty())
		{
			if (reward_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -20;
			}

			if (!sub_cfg.reward_item[reward_count].ReadConfig(reward_item_element))
			{
				return -8;
			}

			++ reward_count;
			reward_item_element = reward_item_element.next_sibling();
		}

		++ total_charge3_cfg.chongzhi_list_count;

		if (!same_with_pre)
		{
			++ cfg.total_cfg_count;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::GetTotalCharge3CfgCount(int day_idx)
{
	int opengame_day = this ->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_3);
	int stage = 0;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_total_charge_3_cfglist[i].opengame_day)
		{
			stage = i;
			break;
		}
	}
	
	RandActivityTotalCharge3Config &cfg = m_total_charge_3_cfglist[stage];

	for (int index = 0; index < cfg.total_cfg_count; ++ index)
	{
		RandActivityTotalCharge3Cfg &charge_cfg = cfg.total_charge_3_cfglist[index];
		if (day_idx >= charge_cfg.start_day && day_idx <= charge_cfg.end_day)
		{
			return charge_cfg.chongzhi_list_count;
		}
	}

	return 0;
}

bool RandActivityConfig::CheckIsResetTotalCharge3Day(int day_idx)
{
	int opengame_day = this ->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_3);
	int stage = 0;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_total_charge_3_cfglist[i].opengame_day)
		{
			stage = i;
			break;
		}
	}

	RandActivityTotalCharge3Config &cfg = m_total_charge_3_cfglist[stage];

	for (int index = 0; index < cfg.total_cfg_count; ++ index)
	{
		if (day_idx == cfg.total_charge_3_cfglist[index].start_day)
		{
			return true;
		}
	}

	return false;
}

const RandActivityTotalCharge3Cfg::ConfigItem * RandActivityConfig::GetTotalCharge3Cfg(int day_idx, int seq)
{
	int opengame_day = this ->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_3);
	int stage = 0;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_total_charge_3_cfglist[i].opengame_day)
		{
			stage = i;
			break;
		}
	}

	RandActivityTotalCharge3Config &cfg = m_total_charge_3_cfglist[stage];

	for (int index = 0; index < cfg.total_cfg_count; ++ index)
	{
		RandActivityTotalCharge3Cfg &charge_cfg = cfg.total_charge_3_cfglist[index];
		if (day_idx >= charge_cfg.start_day && day_idx <= charge_cfg.end_day)
		{
			if (seq >= 0 && seq < charge_cfg.chongzhi_list_count && seq < RandActivityTotalCharge3Cfg::MAX_CHONGZHI_CFG_COUNT)
			{
				return &cfg.total_charge_3_cfglist[index].chongzhi_list[seq];
			}
		}
	}

	return NULL;
}

int RandActivityConfig::InitAppearacePassiveRankCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int pre_rank = 0;
	int rank = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "rank", rank) || rank != pre_rank + 1)
		{
			return -1;
		}

		if (rank <= 0 || rank > RA_SPECIAL_APPEARANCE_PASSIVE_RANK_MAX_COUNT)
		{
			return -2;
		}
		pre_rank = rank;

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -7;
		}

		if (!m_appearance_passive_rank_reward_list[rank].ReadConfig(reward_item_element))
		{
			return -8;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitAppearaceRankCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int pre_rank = 0;
	int rank = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "rank", rank) || rank != pre_rank + 1)
		{
			return -1;
		}

		if (rank <= 0 || rank > RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT)
		{
			return -2;
		}
		pre_rank = rank;

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -7;
		}

		if (!m_appearance_rank_reward_list[rank].ReadConfig(reward_item_element))
		{
			return -8;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const ItemConfigData * RandActivityConfig::GetAppearaceRankReward(int rank)
{
	if (rank > 0 && rank <= RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT)
	{
		return &m_appearance_rank_reward_list[rank];
	}

	return NULL;
}

const ItemConfigData * RandActivityConfig::GetAppearacePassiveRankReward(int rank)
{
	if (rank > 0 && rank <= RA_SPECIAL_APPEARANCE_PASSIVE_RANK_MAX_COUNT)
	{
		return &m_appearance_passive_rank_reward_list[rank];
	}

	return NULL;
}

int RandActivityConfig::InitAnyMonsterDropCfg(PugiXmlNode RootElement)
{

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while(!data_element.empty())
	{
		if (m_any_monster_drop_cfg_list_count >= RAND_ACTIVITY_ANY_MONSTER_DROP_CFG_COUNT)
		{
			return -1;
		}

		RandActivityAnyMonsterDropConfig &drop_cfg = m_any_monster_drop_cfg_list[m_any_monster_drop_cfg_list_count];

		if (!PugiGetSubNodeValue(data_element, "add_attr_item_id", drop_cfg.add_attr_item_id) || NULL == ITEMPOOL->GetItem(drop_cfg.add_attr_item_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "add_attr_rate", drop_cfg.add_attr_rate) || drop_cfg.add_attr_rate <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", drop_cfg.maxhp) || drop_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", drop_cfg.gongji) || drop_cfg.gongji < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", drop_cfg.fangyu) || drop_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", drop_cfg.mingzhong) || drop_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", drop_cfg.shanbi) || drop_cfg.shanbi < 0)
		{
			return -104;
		}

		++ m_any_monster_drop_cfg_list_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityAnyMonsterDropConfig * RandActivityConfig::GetAndyMonsterDropCfg(ItemID item_id)
{
	// 不带参数，则返回第一个配置（只需要固定加的属性）
	if (item_id == ItemBase::INVALID_ITEM_ID && m_any_monster_drop_cfg_list_count > 0)
	{
		return &m_any_monster_drop_cfg_list[0];
	}

	for (int index = 0; index < m_any_monster_drop_cfg_list_count; ++ index)
	{
		if (m_any_monster_drop_cfg_list[index].add_attr_item_id == item_id)
		{
			return &m_any_monster_drop_cfg_list[index];
		}
	}

	return NULL;
}

int RandActivityConfig::InitTotalChongzhi4Cfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		int start_day = 0;
		int end_day = 0;
		int opengame_day = 0;

		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -1;
		}

		if (opengame_day != last_opengame_day)
		{
			if (opengame_day <= last_opengame_day)
			{
				return -200;
			}

			++ stage;
			last_opengame_day = opengame_day;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -300;
		}

		RandActivityTotalCharge4Config &stage_cfg = m_total_charge_4_cfg_list[stage];
		stage_cfg.opengame_day = opengame_day;

		if (!PugiGetSubNodeValue(data_element, "start_day", start_day) || start_day < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "end_day", end_day) || end_day < 0 || end_day < start_day)
		{
			return -2;
		}

		if (stage_cfg.total_charge_4_cfglist_count >= RAND_ACTIVITY_TOTAL_CHARGE_4_CFG_COUNT)
		{
			return -3;
		}

		bool same_with_pre = false; // 是否跟上一个配置是同一个时间段

		RandActivityTotalCharge4Cfg &pre_total_charge4_cfg = stage_cfg.total_charge_4_cfglist[stage_cfg.total_charge_4_cfglist_count - 1];

		if (stage_cfg.total_charge_4_cfglist_count > 0)
		{
			if (pre_total_charge4_cfg.start_day == start_day && pre_total_charge4_cfg.end_day == end_day)
			{
				same_with_pre = true;
			}
			else if(start_day > pre_total_charge4_cfg.end_day)
			{
				same_with_pre = false;
			}
			else
			{
				return -4;
			}
		}

		int cur_index = stage_cfg.total_charge_4_cfglist_count;
		if (same_with_pre)
		{
			cur_index = stage_cfg.total_charge_4_cfglist_count - 1;
		}

		RandActivityTotalCharge4Cfg &total_charge4_cfg = stage_cfg.total_charge_4_cfglist[cur_index];
		total_charge4_cfg.start_day = start_day;
		total_charge4_cfg.end_day = end_day;

		if (total_charge4_cfg.chongzhi_list_count >= RandActivityTotalCharge4Cfg::MAX_CHONGZHI_CFG_COUNT)
		{
			return -5;
		}

		RandActivityTotalCharge4Cfg::ConfigItem &sub_cfg = total_charge4_cfg.chongzhi_list[total_charge4_cfg.chongzhi_list_count];
		sub_cfg.seq = total_charge4_cfg.chongzhi_list_count;

		if (!PugiGetSubNodeValue(data_element, "need_chongzhi_num", sub_cfg.need_chongzhi_num) || sub_cfg.need_chongzhi_num <= 0)
		{
			return -6;
		}

		PugiXmlNode item_element_list = data_element.child("reward_item_list");
		if (item_element_list.empty())
		{
			return -1001;
		}

		PugiXmlNode reward_item_element = item_element_list.child("reward_item");
		if (reward_item_element.empty())
		{
			return -7;
		}

		int reward_count = 0;

		while (!reward_item_element.empty())
		{
			if (reward_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -20;
			}

			if (!sub_cfg.reward_item[reward_count].ReadConfig(reward_item_element))
			{
				return -8;
			}

			++ reward_count;
			reward_item_element = reward_item_element.next_sibling();
		}

		++ total_charge4_cfg.chongzhi_list_count;

		if (!same_with_pre)
		{
			++ stage_cfg.total_charge_4_cfglist_count;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityTotalCharge4Cfg::ConfigItem * RandActivityConfig::GetTotalCharge4Cfg(int day_idx, int seq)
{
	int opengame_day = this ->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_total_charge_4_cfg_list[i].opengame_day)
		{
			stage = i;
			break;
		}
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityTotalCharge4Config &cfg = m_total_charge_4_cfg_list[stage];

	for (int index = 0; index < cfg.total_charge_4_cfglist_count; ++ index)
	{
		RandActivityTotalCharge4Cfg &charge_cfg = cfg.total_charge_4_cfglist[index];
		if (day_idx >= charge_cfg.start_day && day_idx <= charge_cfg.end_day)
		{
			if (seq >= 0 && seq < charge_cfg.chongzhi_list_count && seq < RandActivityTotalCharge4Cfg::MAX_CHONGZHI_CFG_COUNT)
			{
				return &charge_cfg.chongzhi_list[seq];
			}
		}
	}

	return NULL;
}

int RandActivityConfig::GetTotalCharge4CfgCount(int day_idx)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_total_charge_4_cfg_list[i].opengame_day)
		{
			stage = i;
			break;
		}
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	RandActivityTotalCharge4Config &cfg = m_total_charge_4_cfg_list[stage];

	for (int index = 0; index < cfg.total_charge_4_cfglist_count; ++ index)
	{
		RandActivityTotalCharge4Cfg &charge_cfg = cfg.total_charge_4_cfglist[index];
		if (day_idx >= charge_cfg.start_day && day_idx <= charge_cfg.end_day)
		{
			return charge_cfg.chongzhi_list_count;
		}
	}

	return 0;
}

bool RandActivityConfig::CheckIsResetTotalCharge4Day(int day_idx)
{
	int opengame_day = this ->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_total_charge_4_cfg_list[i].opengame_day)
		{
			stage = i;
			break;
		}
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return false;
	}

	RandActivityTotalCharge4Config &cfg = m_total_charge_4_cfg_list[stage];

	for (int index = 0; index < cfg.total_charge_4_cfglist_count; ++ index)
	{
		if (day_idx == cfg.total_charge_4_cfglist[index].start_day)
		{
			return true;
		}
	}

	return false;
}

int RandActivityConfig::InitMineInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_mine_info_cfg_count = 0;

	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			last_opengame_day = opengame_day;
			++stage;

			if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
			{
				return -200;
			}

			m_mine_info_cfg_list[stage].opengame_day = opengame_day;
		}
		RandActivityMineInfoConfig &mine_info_cfg = m_mine_info_cfg_list[stage];

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != mine_info_cfg.stage_list_count)
		{
			return -1;
		}

		RandActivityMineInfoStageConfig &mine_stage_info_cfg = mine_info_cfg.stage_list[seq];
		mine_stage_info_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "need_gold", mine_stage_info_cfg.need_gold) || mine_stage_info_cfg.need_gold <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "exchange_need_num", mine_stage_info_cfg.exchange_need_num) || mine_stage_info_cfg.exchange_need_num <= 0)
		{
			return -3;
		}

		PugiXmlNode item_element = data_element.child("exchange_item");
		if (item_element.empty())
		{
			return -100;
		}

		if (!mine_stage_info_cfg.exchange_item.ReadConfig(item_element))
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "show_weight", mine_stage_info_cfg.show_weight) || mine_stage_info_cfg.show_weight < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "show_need_refresh_times", mine_stage_info_cfg.show_need_refresh_times) || mine_stage_info_cfg.show_need_refresh_times < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "show_certain_refresh_times", mine_stage_info_cfg.show_certain_refresh_beishu) || mine_stage_info_cfg.show_certain_refresh_beishu < 0)
		{
			return -6;
		}

		int is_broadcast = -1;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast) || is_broadcast < 0 || is_broadcast > 1)
		{
			return -8;
		}
		mine_stage_info_cfg.is_broadcast = (0 != is_broadcast);

		int baodi = 0;
		if (PugiGetSubNodeValue(data_element, "baodi", baodi) && baodi > 0)
		{
			if (mine_info_cfg.baodi_map.size() > 0 || mine_info_cfg.baodi_map.find(seq) != mine_info_cfg.baodi_map.end())
			{
				return -9;
			}

			mine_info_cfg.baodi_map.insert(std::make_pair(baodi, seq));
		}

		++mine_info_cfg.stage_list_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitMineServerRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_mine_server_reward_cfg_count = 0;

	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			last_opengame_day = opengame_day;
			++stage;

			if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
			{
				return -200;
			}

			m_mine_server_reward_cfg_list[stage].opengame_day = opengame_day;
		}

		RandActivityMineServerRewardConfig &cfg_list = m_mine_server_reward_cfg_list[stage];
		cfg_list.opengame_day = opengame_day;

		if (cfg_list.stage_list_count >= RA_MINE_REWARD_MAX_COUNT)
		{
			return -1000;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != cfg_list.stage_list_count)
		{
			return -1;
		}

		RandActivityMineServerRewardStageConfig &mine_reward_stage_cfg = cfg_list.stage_list[seq];
		mine_reward_stage_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "total_refresh_times", mine_reward_stage_cfg.total_refresh_times) || mine_reward_stage_cfg.total_refresh_times < 0)
		{
			return -2;
		}
		if (seq > 0)
		{
			if (mine_reward_stage_cfg.total_refresh_times <= cfg_list.stage_list[seq - 1].total_refresh_times)
			{
				return -3;
			}
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -100;
		}

		if (!mine_reward_stage_cfg.reward_item.ReadConfig(item_element))
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "mine_server_reward_vip_limit", mine_reward_stage_cfg.vip_limit) || mine_reward_stage_cfg.vip_limit < 0 ||
			mine_reward_stage_cfg.vip_limit > MAX_VIP_LEVEL)
		{
			return -4;
		}

		++cfg_list.stage_list_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

bool RandActivityConfig::GetMineRefreshResult(char result_list[], int result_list_len, int role_refresh_times, int server_chou_times)
{
	if (NULL == result_list || result_list_len <= 0 || result_list_len > RA_MINE_MAX_REFRESH_COUNT || result_list_len > m_mine_refresh_count)
	{
		return false;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MINE);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_mine_info_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityMineInfoConfig &mine_info_cfg_list = m_mine_info_cfg_list[stage];

	char mine_type_list[RA_MINE_MAX_TYPE_COUNT]; memset(mine_type_list, 0, sizeof(mine_type_list));

	// 获取必刷矿列表
	int cur_refresh_count = 0;

	if (role_refresh_times > 0)
	{
		long long mine_beishu_map[RA_MINE_MAX_TYPE_COUNT]; memset(mine_beishu_map, 0, sizeof(mine_beishu_map));

		for (int seq = 0; seq < mine_info_cfg_list.stage_list_count && seq < RA_MINE_MAX_TYPE_COUNT; ++ seq)
		{
			int cur_refresh_beishu = mine_info_cfg_list.stage_list[seq].show_certain_refresh_beishu;

			if (cur_refresh_beishu > 0 && (0 == role_refresh_times % cur_refresh_beishu))
			{	
				mine_type_list[cur_refresh_count ++] = seq + RA_MINE_TYPES_BEGIN;
				mine_beishu_map[seq] = ConvertParamToLongLong(seq + RA_MINE_TYPES_BEGIN, cur_refresh_beishu);
			}
		}

		// 把倍数高的seq返还
		if (cur_refresh_count >= result_list_len)
		{
			// 选择排序，从大到小
			for (int i = 0; i < RA_MINE_MAX_TYPE_COUNT - 1; ++ i)
			{
				for (int j = i + 1; j < RA_MINE_MAX_TYPE_COUNT; ++ j)
				{
					if (mine_beishu_map[i] < mine_beishu_map[j])
					{
						long long tmp = mine_beishu_map[i];
						mine_beishu_map[i] = mine_beishu_map[j];
						mine_beishu_map[j] = tmp;
					}
				}
			}

			// 返还
			for (int count = 0; count < result_list_len && count < RA_MINE_MAX_TYPE_COUNT; ++ count)
			{
				result_list[count] = (int)mine_beishu_map[count];
			}

			return true;
		}
	}

	// 全服保底次数刷
	if (server_chou_times > 0)
	{
		for (std::map<int, int>::iterator it = mine_info_cfg_list.baodi_map.begin(); it != mine_info_cfg_list.baodi_map.end(); ++it)
		{
			const int baodi_times = it->first;
			const int reward_seq = it->second;

			if (baodi_times > 0 && 0 == server_chou_times % baodi_times && reward_seq >= 0 && reward_seq < RandActivityMineInfoConfig::RA_MINE_INFO_STAGE_LIST_MAX_LEN)
			{
				mine_type_list[cur_refresh_count++] = reward_seq + RA_MINE_TYPES_BEGIN;
				break;
			}
		}
	}

	// 普通矿总权重
	int normal_total_weight = 0;

	// 获取普通矿列表
	for (int seq = 0; seq < mine_info_cfg_list.stage_list_count && seq < RA_MINE_MAX_TYPE_COUNT; ++seq)
	{
		if (mine_info_cfg_list.stage_list[seq].show_need_refresh_times >= 0 && role_refresh_times >= mine_info_cfg_list.stage_list[seq].show_need_refresh_times)
		{
			normal_total_weight += mine_info_cfg_list.stage_list[seq].show_weight;
		}
	}

	if (normal_total_weight <= 0)
	{
		return false;
	}

	for (int count = cur_refresh_count; count < result_list_len; ++ count)
	{
		int rand_weight = RandomNum(normal_total_weight);

		for (int seq = 0; seq < mine_info_cfg_list.stage_list_count && seq < RA_MINE_MAX_TYPE_COUNT; ++seq)
		{
			if (rand_weight < mine_info_cfg_list.stage_list[seq].show_weight)
			{
				mine_type_list[count] = seq + RA_MINE_TYPES_BEGIN;
				break;
			}
			else
			{
				rand_weight -= mine_info_cfg_list.stage_list[seq].show_weight;
			}
		}
	}

	memcpy(result_list, mine_type_list, result_list_len);

	return true;
}

const RandActivityMineInfoStageConfig * RandActivityConfig::GetMineInfoCfg(int seq)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MINE);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_mine_info_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	if (seq < 0 || seq >= m_mine_info_cfg_list[stage].stage_list_count || seq >= RandActivityMineInfoConfig::RA_MINE_INFO_STAGE_LIST_MAX_LEN)
	{
		return NULL;
	}

	return &m_mine_info_cfg_list[stage].stage_list[seq];
}

const RandActivityMineServerRewardStageConfig * RandActivityConfig::GetMineServerRewardCfg(int seq)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MINE);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_mine_info_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	if (seq < 0 || seq >= m_mine_server_reward_cfg_list[stage].stage_list_count || seq >= RandActivityMineServerRewardConfig::RA_MINE_REWARD_STAGE_LIST_MAX_LEN)
	{
		return NULL;
	}

	return &m_mine_server_reward_cfg_list[stage].stage_list[seq];
}

int RandActivityConfig::InitGuaGuaCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}
	int last_opengame_day = 0;
	int opengame_day_count = 0;

	while (!data_element.empty())
	{
		int baodi = 0;
		if (PugiGetSubNodeValue(data_element, "baodi", baodi) && baodi > 0)
		{
			data_element = data_element.next_sibling();
			continue;
		}

		RandActivityGuaGuaConfig cfg;

		if (!PugiGetSubNodeValue(data_element, "opengame_day", cfg.opengame_day) || cfg.opengame_day <= 0 || cfg.opengame_day < last_opengame_day)
		{
			return -100;
		}

		if (cfg.opengame_day != last_opengame_day)
		{
			++opengame_day_count;
			last_opengame_day = cfg.opengame_day;
		}

		if (opengame_day_count < 0 || opengame_day_count >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", cfg.seq) || cfg.seq < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight < 0)
		{
			return -2;
		}
		m_guagua_total_weight[cfg.opengame_day] += cfg.weight;

		// 读奖励列表
		PugiXmlNode item_element_list = data_element.child("reward_item_list");
		if (item_element_list.empty())
		{
			return -1001;
		}

		PugiXmlNode reward_item = item_element_list.child("reward_item");
		if (reward_item.empty())
		{
			return -1002;
		}

		if (!cfg.reward_item.ReadConfig(reward_item))
		{
			return -3;
		}

		// 特别奖
		if (!PugiGetSubNodeValue(data_element, "is_special", cfg.is_special) || cfg.is_special < 0)
		{
			return -4;
		}

		m_guagua_cfg_list.push_back(cfg);
		data_element = data_element.next_sibling();
	}

	//检查每天的权重
	for (auto& v : m_guagua_total_weight)
	{
		if (v.second <= 0)
		{
			return -1111111;
		}
	}

	return 0;
}

int RandActivityConfig::InitGuaGuaAccRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_opengame_day = 0;
	int opengame_day_count = 0;

	while (!data_element.empty())
	{
		RandActivityGuaGuaAccConfig cfg;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", cfg.opengame_day) || cfg.opengame_day <= 0)
		{
			return -100;
		}

		if (cfg.opengame_day != last_opengame_day)
		{
			++opengame_day_count;
			last_opengame_day = cfg.opengame_day;
		}

		if (opengame_day_count < 0 || opengame_day_count >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (!PugiGetSubNodeValue(data_element, "index", cfg.index) || cfg.index < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "acc_count", cfg.acc_count) || cfg.acc_count <= 0)
		{
			return -3;
		}

		// 读奖励列表
		PugiXmlNode item_element_list = data_element.child("reward_item_list");
		if (item_element_list.empty())
		{
			return -1001;
		}

		PugiXmlNode item_element = item_element_list.child("reward_item");
		if (item_element.empty())
		{
			return -4;
		}

		if (!cfg.reward_item.ReadConfig(item_element))
		{
			return -5;
		}

		m_guagua_acc_cfg_list.push_back(cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitGuaGuaBaodiRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100;
	}

	int last_opengame_day = -1;
	int stage = -1;
	int last_seq = 0;
	while (!data_element.empty())
	{
		int baodi = 0;
		if (!PugiGetSubNodeValue(data_element, "baodi", baodi) || baodi <= 0)
		{
			data_element = data_element.next_sibling();
			continue;
		}

		int open_game_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", open_game_day) || open_game_day <= 0)
		{
			return -1;
		}

		if (open_game_day != last_opengame_day)
		{
			last_opengame_day = open_game_day;
			stage++;
			last_seq = 0;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -100;
		}

		RandActivityServerBaodiCfgList & baodi_cfg = m_guagua_baodi_reward_cfg[stage];

		if (baodi_cfg.opengame_day != 0 && baodi_cfg.opengame_day != open_game_day)
		{
			return -101;
		}

		if (baodi_cfg.baodi != 0 && baodi_cfg.baodi != baodi)
		{
			return -102;
		}

		baodi_cfg.opengame_day = open_game_day;
		baodi_cfg.baodi = baodi;

		RandActivityServerBaodiConfig baodi_item;

		if (!PugiGetSubNodeValue(data_element, "seq", baodi_item.seq) || baodi_item.seq < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "baodi_weight", baodi_item.weight) || baodi_item.weight < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "is_special", baodi_item.is_rare))
		{
			return -5;
		}

		PugiXmlNode item_element_list = data_element.child("reward_item_list");
		if (item_element_list.empty())
		{
			return -1001;
		}

		PugiXmlNode reward_item_element = item_element_list.child("reward_item");
		if (reward_item_element.empty())
		{
			return -6;
		}

		if (!baodi_item.reward_item.ReadConfig(reward_item_element))
		{
			return -7;
		}

		if (last_seq != 0 &&  last_seq +1  != baodi_item.seq)
		{
			return -103;
		}
		last_seq = baodi_item.seq;

		baodi_cfg.server_baodi_vec.push_back(baodi_item);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::GetGuaGuaPlayTimesGold(int type)
{
	if (type < RA_GUAGUA_PLAY_ONE_TIME || type >= RA_GUAGUA_TYPE_MAX)
	{
		return 0;
	}

	return m_guagua_need_gold_list[type];
}

int RandActivityConfig::GetGuaGuaFreeTimeInterval() // 返回秒
{
	return m_guagua_free_time_interval * SECOND_PER_MIN;
}

bool RandActivityConfig::GetGuaGuaRandomRewardCfg(RandActivityGuaGuaConfig *guagua_cfg, int *is_win, int server_roll_times) // 参数2是是否获得了奖励（否则为安慰奖）
{
	if (NULL == guagua_cfg || NULL == is_win)
	{
		return false;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_GUAGUA);

	RandActivityGuaGuaConfig temp_cfg;
	temp_cfg.opengame_day = opengame_day;

	// 找到第一个大于当前开服天数的配置
	auto begin_iter = std::upper_bound(m_guagua_cfg_list.begin(), m_guagua_cfg_list.end(), temp_cfg);
	if (begin_iter == m_guagua_cfg_list.end())
	{
		return false;
	}

	// 检查总权重
	auto total_weight_iter = m_guagua_total_weight.find(begin_iter->opengame_day);
	if (total_weight_iter == m_guagua_total_weight.end() || 0 == total_weight_iter->second)
	{
		return false;
	}

	// 找到最后一个配置
	auto end_iter = begin_iter + 1;
	while (end_iter != m_guagua_cfg_list.end())
	{
		if (begin_iter->opengame_day == end_iter->opengame_day)
		{
			++end_iter;
		}
		else
		{
			break;
		}
	}

	// 全服保底次数刷
//	if (server_roll_times > 0)
//	{
// 		for (auto iter = begin_iter; iter != end_iter; ++iter)
// 		{
// 			int baodi_times = iter->baodi;
// 			if (baodi_times > 0)
// 			{
// 				if (0 == server_roll_times % baodi_times)
// 				{
// 					*is_win = 1;
// 					*guagua_cfg = *iter;
// 					return true;
// 				}
// 			}
// 		}
//	}

	// 全服保底次数刷
	const RandActivityServerBaodiConfig *  baodi_cfg = GetGuaGuaBaodiRewardCfg(server_roll_times);
	if (NULL != baodi_cfg)
	{
		*is_win = 1;
		guagua_cfg->opengame_day = opengame_day;
		guagua_cfg->reward_item = baodi_cfg->reward_item;
		guagua_cfg->seq = baodi_cfg->seq;
		guagua_cfg->baodi = 1;
		guagua_cfg->weight = 0;
		guagua_cfg->is_special = baodi_cfg->is_rare;
		return true;
	}

	// 由于需求更改,目前安慰奖不需要
	int rand_weight = RandomNum(total_weight_iter->second);
	if (rand_weight >= total_weight_iter->second)
	{
		*is_win = 0;
		//*guagua_cfg = m_guagua_anwei_reward;
		return true;
	}

	// 随机物品
	for (auto iter = begin_iter; iter != end_iter; ++iter)
	{
		if (rand_weight < iter->weight)
		{
			*is_win = 1;
			*guagua_cfg = *iter;
			return true;
		}

		rand_weight -= iter->weight;
	}

	return false;
}

const RandActivityGuaGuaAccConfig *RandActivityConfig::GetGuaGuaAccRewardByIndex(int index)
{
	if (index < 0)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_GUAGUA);
	for (int i = 0; i < (int)m_guagua_acc_cfg_list.size(); ++i)
	{
		if (opengame_day < m_guagua_acc_cfg_list[i].opengame_day && m_guagua_acc_cfg_list[i].index == index)
		{
			return &m_guagua_acc_cfg_list[i];
		}
	}

	return NULL;
}

const RandActivityServerBaodiConfig * RandActivityConfig::GetGuaGuaBaodiRewardCfg(int server_chou_times)
{
	if (server_chou_times <= 0) return nullptr;

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_GUAGUA);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day <= m_guagua_baodi_reward_cfg[i].opengame_day)
		{
			if (server_chou_times >= m_guagua_baodi_reward_cfg[i].baodi)
			 {
				 stage = i;
				 break;
			}
			else
			{
				return NULL;
			}
		}
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	return gamecommon::RandListItem(m_guagua_baodi_reward_cfg[stage].server_baodi_vec);
}

int RandActivityConfig::InitTianMingDivinationRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_tianming_reward_max_seq = -1;
	m_tianming_reward_total_weight = 0;
	m_tianming_reward_add_lot_seq_count = 0;

	int last_opengame_day = 0;

	static const int GRID_MAX_COUNT = 23;
	int grid_count = 0;

	while (!data_element.empty())
	{
		if (m_tianming_reward_max_seq >= RAND_ACTIVITY_TIANMING_DIVINATION_REWARD_MAX_SEQ)
		{
			return -1000;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_tianming_reward_max_seq + 1 )
		{
			return -1;
		}

		RandActivityTianMingDivinationRewardConfig & tianming_reward_cfg = m_tianming_reward_cfg_list[seq];
		tianming_reward_cfg.seq = seq;
		m_tianming_reward_max_seq = seq;

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -2;
		}

		if (!tianming_reward_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", tianming_reward_cfg.weight) || tianming_reward_cfg.weight <= 0)
		{
			return -4;
		}

		m_tianming_reward_total_weight += tianming_reward_cfg.weight;

		if (!PugiGetSubNodeValue(data_element, "opengame_day", tianming_reward_cfg.opengame_day) || tianming_reward_cfg.opengame_day <= 0)
		{
			return -5;
		}

		if (GRID_MAX_COUNT == grid_count)
		{
			if (last_opengame_day == tianming_reward_cfg.opengame_day)
			{
				return -1010;
			}
			else
			{
				last_opengame_day = tianming_reward_cfg.opengame_day;
				grid_count = 0;
			}
		}

		int can_add_lot = -1;
		if (!PugiGetSubNodeValue(data_element, "can_add_lot", can_add_lot) || can_add_lot < 0 || can_add_lot > 1)
		{
			return -6;
		}
		tianming_reward_cfg.can_add_lot = (0 != can_add_lot);

		if (tianming_reward_cfg.can_add_lot)
		{

			++ m_tianming_reward_add_lot_seq_count;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", tianming_reward_cfg.is_broadcast))
		{
			return -7;
		}

		if (last_opengame_day != tianming_reward_cfg.opengame_day)
		{
			last_opengame_day = tianming_reward_cfg.opengame_day;
			++grid_count;
		}

		if (grid_count < 0 || grid_count >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -10001;
		}
		m_tianming_baodi_cfg_list[grid_count].opengame_day = tianming_reward_cfg.opengame_day;

		int baodi = 0;
		if (PugiGetSubNodeValue(data_element, "baodi", baodi) || baodi > 0)
		{
			if (m_tianming_baodi_cfg_list[grid_count].baodi_map.find(baodi) != m_tianming_baodi_cfg_list[grid_count].baodi_map.end()
				|| m_tianming_baodi_cfg_list[grid_count].baodi_map.size() > 0)
			{
				return -1002;
			}

			m_tianming_baodi_cfg_list[grid_count].baodi_map.insert(std::make_pair(baodi, tianming_reward_cfg.seq));
		}

		data_element = data_element.next_sibling();
	}
	++m_tianming_reward_max_seq;

	if (m_tianming_reward_total_weight <= 0)
	{
		return -2000;
	}

	if (0 != m_tianming_reward_add_lot_seq_count % 6)
	{
		return -3000;
	}

	return 0;
}

int RandActivityConfig::InitTianMingDivinationGoldConsumeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_tianming_gold_consume_max_add_lot_times = -1;
	int last_add_consume_gold = 0;
	int last_chou_consume_gold = 0;

	while (!data_element.empty())
	{
		if (m_tianming_gold_consume_max_add_lot_times >= RAND_ACTIVITY_TIANMING_DIVINATION_TOTAL_MAX_ADD_LOT_TIMES)
		{
			return -2000;
		}

		int times = -1;
		if (!PugiGetSubNodeValue(data_element, "lot_add_times", times) || times != m_tianming_gold_consume_max_add_lot_times + 1)
		{
			return -1;
		}

		RandActivityTianMingDivinationGoldConsumeConfig & tianming_gold_consume_cfg = m_tianming_gold_consume_cfg_list[times];
		tianming_gold_consume_cfg.lot_add_times = times;
		m_tianming_gold_consume_max_add_lot_times = times;

		if (!PugiGetSubNodeValue(data_element, "add_consume_gold", tianming_gold_consume_cfg.add_consume_gold) || tianming_gold_consume_cfg.add_consume_gold < 0 ||
			tianming_gold_consume_cfg.add_consume_gold < last_add_consume_gold)
		{
			return -2;
		}
		last_add_consume_gold = tianming_gold_consume_cfg.add_consume_gold;

		if (!PugiGetSubNodeValue(data_element, "chou_consume_gold", tianming_gold_consume_cfg.chou_consume_gold) || tianming_gold_consume_cfg.chou_consume_gold <= 0 ||
			tianming_gold_consume_cfg.chou_consume_gold < last_chou_consume_gold)
		{
			return -3;
		}
		last_chou_consume_gold = tianming_gold_consume_cfg.chou_consume_gold;

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityTianMingDivinationRewardConfig * RandActivityConfig::GetTianMingDivinationRewardCfg(int seq)
{
	if (seq < 0 || seq >= m_tianming_reward_max_seq || seq >= RAND_ACTIVITY_TIANMING_DIVINATION_REWARD_MAX_SEQ)
	{
		return NULL;
	}

	return &m_tianming_reward_cfg_list[seq];
}

const RandActivityTianMingDivinationRewardConfig * RandActivityConfig::GetTianMingDivinationBaodiRewardCfg(const int server_chou_times)
{
	if (server_chou_times <= 0) return nullptr;

	const int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_TIANMING_DIVINATION);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_tianming_baodi_cfg_list[i].opengame_day)
		{
			continue;
		}

		for (std::map<int, int>::iterator it = m_tianming_baodi_cfg_list[i].baodi_map.begin(); it != m_tianming_baodi_cfg_list[i].baodi_map.end(); ++it)
		{
			const int baodi_times = it->first;
			const int reward_seq = it->second;

			if (baodi_times <= 0 || 0 != server_chou_times % baodi_times) continue;

			if (reward_seq < 0 || reward_seq >= RAND_ACTIVITY_TIANMING_DIVINATION_REWARD_MAX_SEQ || m_tianming_reward_cfg_list[reward_seq].seq != reward_seq)
			{
				continue;
			}

			return &m_tianming_reward_cfg_list[reward_seq];
		}
		break;
	}

	return nullptr;
}

const RandActivityTianMingDivinationGoldConsumeConfig * RandActivityConfig::GetTianMingDivinationGoldConsumeCfg(int times)
{
	if (times < 0 || times > m_tianming_gold_consume_max_add_lot_times || times > RAND_ACTIVITY_TIANMING_DIVINATION_TOTAL_MAX_ADD_LOT_TIMES)
	{
		return NULL;
	}

	return &m_tianming_gold_consume_cfg_list[times];
}

int RandActivityConfig::GetTianMingDivinationSeqWithLotIndex(int lot_index)
{
	if (lot_index < 0 || lot_index >= m_tianming_reward_add_lot_seq_count || lot_index >= RA_TIANMING_LOT_COUNT)
	{
		return 0;
	}

	return m_tianming_reward_add_lot_seq_list[lot_index];
}

int RandActivityConfig::GetTianMingDivinationLotIndexWithSeq(int seq)
{
	for(int lot_index = 0; lot_index < RA_TIANMING_LOT_COUNT; ++lot_index)
	{
		if (m_tianming_reward_add_lot_seq_list[lot_index] == seq)
		{
			return lot_index;
		}
	}

	return -1;
}

int RandActivityConfig::InitFanFanItemInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_fanfan_item_info_cfg_count = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			stage++;
			last_opengame_day = opengame_day;
			m_fanfan_item_info_cfg_count = stage + 1;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq >= RA_FANFAN_MAX_ITEM_COUNT)
		{
			return -1;
		}

		RandActivityFanFanItemDayCfg &day_cfg = m_fanfan_item_info_cfg_list[stage];
		++day_cfg.cfg_count;
		day_cfg.opengame_day = opengame_day;

		RandActivityFanFanItemInfo &item_info_cfg = day_cfg.cfg_list[seq];
		item_info_cfg.seq = seq;
		{
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				return -100;
			}

			if (!item_info_cfg.reward_item.ReadConfig(item_element))
			{
				return -101;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "weight", item_info_cfg.weight) || item_info_cfg.weight <= 0)
		{
			return -2;
		}

		int is_broadcast = 0;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast) || is_broadcast < 0 || is_broadcast > 1)
		{
			return -3;
		}
		item_info_cfg.is_broadcast = (0 != is_broadcast);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitFanFanWordInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_fanfan_word_info_cfg_count = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			stage++;
			last_opengame_day = opengame_day;
			m_fanfan_word_info_cfg_count = stage + 1;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq >= RA_FANFAN_MAX_WORD_COUNT)
		{
			return -1;
		}

		RandActivityFanFanWordDayCfg &day_cfg = m_fanfan_word_info_cfg_list[stage];
		++day_cfg.cfg_count;
		day_cfg.opengame_day = opengame_day;

		RandActivityFanFanWordInfo &word_info_cfg = day_cfg.cfg_list[seq];
		word_info_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "weight", word_info_cfg.weight) || word_info_cfg.weight < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "floor_refresh_times", word_info_cfg.floor_refresh_times) || word_info_cfg.floor_refresh_times < 0)
		{
			return -3;
		}

		{
			PugiXmlNode item_element = data_element.child("exchange_item");
			if (item_element.empty())
			{
				return -100;
			}

			if (!word_info_cfg.exchange_item.ReadConfig(item_element))
			{
				return -101;
			}
		}

		int baodi = 0;
		if (PugiGetSubNodeValue(data_element, "baodi", baodi) && baodi > 0)
		{
			if (day_cfg.baodi_map.size() > 0 || day_cfg.baodi_map.find(baodi) != day_cfg.baodi_map.end())
			{
				return -102;
			}

			day_cfg.baodi_map.insert(std::make_pair(baodi, seq));
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitFanFanBaodiCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int index = -1;
	int last_index = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			stage++;
			last_opengame_day = opengame_day;
			m_fanfan_baodi_reward_cfg_count = stage + 1;
			last_index = -1;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (!PugiGetSubNodeValue(data_element, "index", index) || index < 0 || index >= RA_FANFAN_BAODI_REWARD_CFG_COUNT)
		{
			return -1;
		}

		if (index != last_index + 1)
		{
			return -2;
		}
		last_index = index;

		RandActivityFanFanBaodiDayConfig &day_cfg = m_fanfan_baodi_reward_cfg_list[stage];
		++day_cfg.cfg_count;
		day_cfg.opengame_day = opengame_day;

		RandActivityFanFanBaodiConfig &cfg = day_cfg.cfg_list[index];

		cfg.index = index;

		if (!PugiGetSubNodeValue(data_element, "choujiang_times", cfg.choujiang_times) || cfg.choujiang_times <= 0)
		{
			return -3;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -4;
		}

		if (!cfg.reward_item.ReadConfig(item_element))
		{
			return -5;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityFanFanItemInfo * RandActivityConfig::GetFanFanItemInfoCfg(int seq)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_FANFAN);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_fanfan_item_info_cfg_count; ++i)
	{
		if (m_fanfan_word_info_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityFanFanItemDayCfg &day_cfg = m_fanfan_item_info_cfg_list[idx];

	if (seq < 0 || seq >= day_cfg.cfg_count || seq >= RA_FANFAN_MAX_ITEM_COUNT)
	{
		return NULL;
	}

	return &day_cfg.cfg_list[seq];
}

const RandActivityFanFanWordInfo * RandActivityConfig::GetFanFanWordInfoCfg(int seq)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_FANFAN);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_fanfan_word_info_cfg_count; ++i)
	{
		if (m_fanfan_word_info_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityFanFanWordDayCfg &day_cfg = m_fanfan_word_info_cfg_list[idx];

	if (seq < 0 || seq >= day_cfg.cfg_count || seq >= RA_FANFAN_MAX_WORD_COUNT)
	{
		return NULL;
	}

	return &day_cfg.cfg_list[seq];
}

int RandActivityConfig::GetFanFanRandomRewardSeq(int letter_occur_rate, int *is_reward_word)
{
	// 先计算是否出现letter
	bool is_occur_letter = false;

	int rand_value = RandomNum(100);
	if (rand_value < letter_occur_rate)
	{
		is_occur_letter = true;
	}

	int ret_seq = -1;

	if (!is_occur_letter)
	{
		int idx = -1;
		int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_FANFAN);
		for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_fanfan_item_info_cfg_count; ++i)
		{
			if (m_fanfan_word_info_cfg_list[i].opengame_day >= opengame_day)
			{
				idx = i;
				break;
			}
		}

		if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -1;
		}

		RandActivityFanFanItemDayCfg &day_cfg = m_fanfan_item_info_cfg_list[idx];

		// 总权重
		int total_weight = 0;
		for (int i = 0; i < day_cfg.cfg_count && i < RA_FANFAN_MAX_ITEM_COUNT; ++ i)
		{
			total_weight += day_cfg.cfg_list[i].weight;
		}

		if (total_weight > 0)
		{
			// 计算
			int rand_weight = RandomNum(total_weight);
			for (int i = 0; i < day_cfg.cfg_count && i < RA_FANFAN_MAX_ITEM_COUNT; ++ i)
			{
				if (rand_weight < day_cfg.cfg_list[i].weight)
				{
					ret_seq = day_cfg.cfg_list[i].seq + RA_FANFAN_CARD_TYPE_ITEM_BEGIN;
					break;
				}

				rand_weight -= day_cfg.cfg_list[i].weight;
			}
		}
	}
	else
	{
		ret_seq = 0;
	}

	if (is_reward_word != NULL)
	{
		*is_reward_word = (int)is_occur_letter;
	}

	return ret_seq;
}

RAFanFanHiddenWordInfo RandActivityConfig::GetFanFanRandomHiddenWord(int role_refresh_times, int server_chou_times)
{
	RAFanFanHiddenWordInfo hidden_word_info; hidden_word_info.Reset();

	// 检查是否获得保底字组
	bool is_get_floor_word = false;

	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_FANFAN);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_fanfan_word_info_cfg_count; ++i)
	{
		if (m_fanfan_word_info_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return hidden_word_info;
	}

	RandActivityFanFanWordDayCfg &day_cfg = m_fanfan_word_info_cfg_list[idx];

	if (role_refresh_times > 0)
	{
		int floor_refresh_times_count = 0;
		long long floor_refresh_times_map[RA_FANFAN_MAX_WORD_COUNT]; memset(floor_refresh_times_map, 0, sizeof(floor_refresh_times_map));

		for (int seq = 0; seq < day_cfg.cfg_count && seq < RA_FANFAN_MAX_WORD_COUNT; ++seq)
		{
			int floor_refresh_times = day_cfg.cfg_list[seq].floor_refresh_times;

			if (floor_refresh_times > 0 && (0 == role_refresh_times % floor_refresh_times))
			{
				floor_refresh_times_map[seq] = ConvertParamToLongLong(seq, floor_refresh_times);
				++floor_refresh_times_count;

				is_get_floor_word = true;
			}
		}

		if (floor_refresh_times_count > 0)
		{
			for (int i = 0; i < RA_FANFAN_MAX_WORD_COUNT - 1; ++i)
			{
				for (int j = i + 1; j < RA_FANFAN_MAX_WORD_COUNT; ++j)
				{
					if (floor_refresh_times_map[i] < floor_refresh_times_map[j])
					{
						long long tmp = floor_refresh_times_map[i];
						floor_refresh_times_map[i] = floor_refresh_times_map[j];
						floor_refresh_times_map[j] = tmp;
					}
				}
			}

			hidden_word_info.hidden_word_seq = (int)floor_refresh_times_map[0] + RA_FANFAN_CARD_TYPE_WORD_BEGIN;
		}
	}

	if (!is_get_floor_word && server_chou_times > 0)
	{
		for (std::map<int, int>::iterator it = day_cfg.baodi_map.begin(); it != day_cfg.baodi_map.end(); ++it)
		{
			const int baodi_times = it->first;
			const int reward_seq = it->second;

			if (baodi_times > 0 && 0 == server_chou_times % baodi_times && reward_seq >= 0 && reward_seq < day_cfg.cfg_count)
			{
				hidden_word_info.hidden_word_seq = day_cfg.cfg_list[reward_seq].seq + RA_FANFAN_CARD_TYPE_WORD_BEGIN;

				is_get_floor_word = true;
				break;
			}
		}
	}

	if (!is_get_floor_word)
	{
		int total_weight = 0;
		for (int i = 0; i < day_cfg.cfg_count && i < RA_FANFAN_MAX_WORD_COUNT; ++i)
		{
			total_weight += day_cfg.cfg_list[i].weight;
		}

		if (total_weight > 0)
		{
			int rand_weight = RandomNum(total_weight);
			for (int i = 0; i < day_cfg.cfg_count && i < RA_FANFAN_MAX_WORD_COUNT; ++i)
			{
				if (rand_weight < day_cfg.cfg_list[i].weight)
				{
					hidden_word_info.hidden_word_seq = day_cfg.cfg_list[i].seq + RA_FANFAN_CARD_TYPE_WORD_BEGIN;
					break;
				}

				rand_weight -= day_cfg.cfg_list[i].weight;
			}
		}
	}

	return hidden_word_info;
}

const RandActivityFanFanBaodiConfig * RandActivityConfig::GetFanFanBaodi(int index)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_FANFAN);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_fanfan_baodi_reward_cfg_count; ++i)
	{
		if (m_fanfan_baodi_reward_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityFanFanBaodiDayConfig &day_cfg = m_fanfan_baodi_reward_cfg_list[idx];

	if (index < 0 || index >= day_cfg.cfg_count || index >= RA_FANFAN_BAODI_REWARD_CFG_COUNT)
	{
		return NULL;
	}

	return &m_fanfan_baodi_reward_cfg_list[idx].cfg_list[index];
}

int RandActivityConfig::InitContinueChongzhiRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_day_index = 0;
	m_continue_chongzhi_max_day_index = 0;

	while (!data_element.empty())
	{
		if (last_day_index >= RA_CONTINUE_CHONGZHI_MAX_DAY_INDEX)
		{
			return -2000;
		}

		int day_index = 0;
		if (!PugiGetSubNodeValue(data_element, "day_index", day_index) || day_index <= 0 || day_index > RA_CONTINUE_CHONGZHI_MAX_DAY_INDEX || day_index != last_day_index + 1)
		{
			return -1;
		}
		last_day_index = day_index;

		RandActivityContinueChongzhiRewardConfig & continue_chongzhi_reward_cfg = m_continue_chongzhi_reward_cfg_list[day_index];
		m_continue_chongzhi_max_day_index = day_index;

		if (!PugiGetSubNodeValue(data_element, "need_chongzhi", continue_chongzhi_reward_cfg.need_chongzhi) || continue_chongzhi_reward_cfg.need_chongzhi <=0)
		{
			return -2;
		}

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -3;
		}

		if (!continue_chongzhi_reward_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -4;
		}

		data_element = data_element.next_sibling();
	}

	if (m_continue_chongzhi_fetch_extra_reward_need_days > m_continue_chongzhi_max_day_index)
	{
		return -5;
	}

	return 0;
}

// 连充特惠初
int RandActivityConfig::InitContinueChongzhiRewardChuCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_seq = 0;
	int last_open_server_day = 0;
	int last_need_chongzhi = 0;

	while (!data_element.empty())
	{
		RandActivityContinueChongzhiRewardConfig continue_chongzhi_reward_cfg;

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0)
		{
			return -1000;
		}

		if (seq == last_seq + 1)
		{
			last_seq = seq;
			last_open_server_day = 0;
			last_need_chongzhi = 0;
		}

		if (!PugiGetSubNodeValue(data_element, "day_index", continue_chongzhi_reward_cfg.lei_chong_day) || continue_chongzhi_reward_cfg.lei_chong_day <= 0)
		{
			return -1;
		}
		
		if (!PugiGetSubNodeValue(data_element, "open_server_day", continue_chongzhi_reward_cfg.open_server_day) || continue_chongzhi_reward_cfg.open_server_day <= 0 || (0 != last_open_server_day && continue_chongzhi_reward_cfg.open_server_day != last_open_server_day))
		{
			return -2;
		}
		last_open_server_day = continue_chongzhi_reward_cfg.open_server_day;

		if (!PugiGetSubNodeValue(data_element, "need_chongzhi", continue_chongzhi_reward_cfg.need_chongzhi) || continue_chongzhi_reward_cfg.need_chongzhi <= 0 || (0 != last_need_chongzhi && continue_chongzhi_reward_cfg.need_chongzhi != last_need_chongzhi))
		{
			return -3;
		}
		last_need_chongzhi = continue_chongzhi_reward_cfg.need_chongzhi;

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -4;
		}

		if (!continue_chongzhi_reward_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -5;
		}

		continue_chongzhi_reward_cfg.seq = seq;
		m_continue_chongzhi_reward_cfg_vec_chu.push_back(continue_chongzhi_reward_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

// 连充特惠高
int RandActivityConfig::InitContinueChongzhiRewardGaoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_seq = 0;
	int last_open_server_day = 0;
	int last_need_chongzhi = 0;

	while (!data_element.empty())
	{
		RandActivityContinueChongzhiRewardConfig continue_chongzhi_reward_cfg;

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0)
		{
			return -1000;
		}

		if (seq == last_seq + 1)
		{
			last_seq = seq;
			last_open_server_day = 0;
			last_need_chongzhi = 0;
		}

		if (!PugiGetSubNodeValue(data_element, "day_index", continue_chongzhi_reward_cfg.lei_chong_day) || continue_chongzhi_reward_cfg.lei_chong_day <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "open_server_day", continue_chongzhi_reward_cfg.open_server_day) || continue_chongzhi_reward_cfg.open_server_day <= 0 || (0 != last_open_server_day && continue_chongzhi_reward_cfg.open_server_day != last_open_server_day))
		{
			return -2;
		}
		last_open_server_day = continue_chongzhi_reward_cfg.open_server_day;

		if (!PugiGetSubNodeValue(data_element, "need_chongzhi", continue_chongzhi_reward_cfg.need_chongzhi) || continue_chongzhi_reward_cfg.need_chongzhi <= 0 || (0 != last_need_chongzhi && continue_chongzhi_reward_cfg.need_chongzhi != last_need_chongzhi))
		{
			return -3;
		}
		last_need_chongzhi = continue_chongzhi_reward_cfg.need_chongzhi;

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -4;
		}

		if (!continue_chongzhi_reward_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -5;
		}

		continue_chongzhi_reward_cfg.seq = seq;
		m_continue_chongzhi_reward_cfg_vec_gao.push_back(continue_chongzhi_reward_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

// 限时连充
int RandActivityConfig::InitContinueChongzhiReward2Cfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_seq = 0;
	int last_open_server_day = 0;

	while (!data_element.empty())
	{
		RandActivityContinueChongzhiRewardConfig continue_chongzhi_reward_cfg;

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0)
		{
			return -1000;
		}

		if (seq == last_seq + 1)
		{
			last_seq = seq;
			last_open_server_day = 0;
		}

		if (!PugiGetSubNodeValue(data_element, "day_index", continue_chongzhi_reward_cfg.lei_chong_day) || continue_chongzhi_reward_cfg.lei_chong_day <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "open_server_day", continue_chongzhi_reward_cfg.open_server_day) || continue_chongzhi_reward_cfg.open_server_day <= 0 || (0 != last_open_server_day && continue_chongzhi_reward_cfg.open_server_day != last_open_server_day))
		{
			return -2;
		}
		last_open_server_day = continue_chongzhi_reward_cfg.open_server_day;

		if (!PugiGetSubNodeValue(data_element, "need_chongzhi", continue_chongzhi_reward_cfg.need_chongzhi) || continue_chongzhi_reward_cfg.need_chongzhi <= 0)
		{
			return -3;
		}

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -4;
		}

		if (!continue_chongzhi_reward_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -5;
		}
		if (!PugiGetSubNodeValue(data_element, "continue_chongzhi_fetch_extra_reward_need_days_2", continue_chongzhi_reward_cfg.fetch_extra_reward_need_days) || continue_chongzhi_reward_cfg.fetch_extra_reward_need_days <= 0)
		{
			return -6;
		}

		PugiXmlNode continue_chongzhi_extra_reward_item_element = data_element.child("continue_chongzhi_extra_reward_2");
		if (continue_chongzhi_extra_reward_item_element.empty())
		{
			return -7;
		}

		if (!continue_chongzhi_reward_cfg.extra_reward.ReadConfig(continue_chongzhi_extra_reward_item_element))
		{
			return -8;
		}

		continue_chongzhi_reward_cfg.seq = seq;
		m_continue_chongzhi_reward_cfg_vec_2.push_back(continue_chongzhi_reward_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitContinueConsumeRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_day_index = 0;
	m_continue_consume_max_reward_day_cfg_count = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			stage++;
			last_opengame_day = opengame_day;
			++m_continue_consume_max_reward_day_cfg_count;
			last_day_index = 0;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (last_day_index >= RA_CONTINUE_CONSUME_MAX_DAY_INDEX)
		{
			return -2000;
		}

		int day_index = 0;
		if (!PugiGetSubNodeValue(data_element, "day_index", day_index) || day_index <= 0 || day_index > RA_CONTINUE_CONSUME_MAX_DAY_INDEX || day_index != last_day_index + 1)
		{
			return -1;
		}
		last_day_index = day_index;

		RandActivityContinueConsumeRewardDayCfg &day_cfg = m_continue_consume_reward_cfg_list[stage];
		day_cfg.opengame_day = opengame_day;
		++day_cfg.cfg_count;

		RandActivityContinueConsumeRewardConfig &continue_consume_reward_cfg = day_cfg.cfg_list[day_index];

		if (!PugiGetSubNodeValue(data_element, "need_consume_gold", continue_consume_reward_cfg.need_consume_gold) || continue_consume_reward_cfg.need_consume_gold <= 0)
		{
			return -2;
		}

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -3;
		}

		if (!continue_consume_reward_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -4;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityContinueChongzhiRewardConfig * RandActivityConfig::GetContinueChongzhiRewardInfoWithdayindex(int day_index)
{
	if (day_index <= 0 || day_index > m_continue_chongzhi_max_day_index || day_index > RA_CONTINUE_CHONGZHI_MAX_DAY_INDEX)
	{
		return NULL;
	}

	return &m_continue_chongzhi_reward_cfg_list[day_index];
}

const RandActivityContinueChongzhiRewardConfig * RandActivityConfig::GetContinueChongzhiRewardInfoWithdayindexChu(int day_index, bool is_chongzhi, bool is_bu_fa_reward)
{
	int open_day_index = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;

	if (is_chongzhi)
	{
		for (int i = 0; i < static_cast<int>(m_continue_chongzhi_reward_cfg_vec_chu.size()); i++)
		{
			if (open_day_index <= m_continue_chongzhi_reward_cfg_vec_chu[i].open_server_day)
			{
				return &m_continue_chongzhi_reward_cfg_vec_chu[i];
			}
		}
	}
	else
	{
		if (is_bu_fa_reward) open_day_index--;
		int seq = -1;
		for (int i = 0; i < static_cast<int>(m_continue_chongzhi_reward_cfg_vec_chu.size()); i++)
		{
			if (-1 != seq && seq != m_continue_chongzhi_reward_cfg_vec_chu[i].seq) return NULL;

			if (-1 == seq && open_day_index <= m_continue_chongzhi_reward_cfg_vec_chu[i].open_server_day)
			{
				seq = m_continue_chongzhi_reward_cfg_vec_chu[i].seq;
			}

			if (open_day_index <= m_continue_chongzhi_reward_cfg_vec_chu[i].open_server_day && m_continue_chongzhi_reward_cfg_vec_chu[i].lei_chong_day == day_index)
			{
				return &m_continue_chongzhi_reward_cfg_vec_chu[i];
			}
		}
	}

	return NULL;
}

const RandActivityContinueChongzhiRewardConfig * RandActivityConfig::GetContinueChongzhiRewardInfoWithdayindexGao(int day_index, bool is_chongzhi, bool is_bu_fa_reward)
{
	int open_day_index = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;

	if (is_chongzhi)
	{
		for (int i = 0; i < static_cast<int>(m_continue_chongzhi_reward_cfg_vec_gao.size()); i++)
		{
			if (open_day_index <= m_continue_chongzhi_reward_cfg_vec_gao[i].open_server_day)
			{
				return &m_continue_chongzhi_reward_cfg_vec_gao[i];
			}
		}
	}
	else
	{
		if (is_bu_fa_reward) open_day_index--;
		int seq = -1;
		for (int i = 0; i < static_cast<int>(m_continue_chongzhi_reward_cfg_vec_gao.size()); i++)
		{
			if (-1 != seq && seq != m_continue_chongzhi_reward_cfg_vec_gao[i].seq) return NULL;

			if (-1 == seq && open_day_index <= m_continue_chongzhi_reward_cfg_vec_gao[i].open_server_day)
			{
				seq = m_continue_chongzhi_reward_cfg_vec_gao[i].seq;
			}

			if (open_day_index <= m_continue_chongzhi_reward_cfg_vec_gao[i].open_server_day && m_continue_chongzhi_reward_cfg_vec_gao[i].lei_chong_day == day_index)
			{
				return &m_continue_chongzhi_reward_cfg_vec_gao[i];
			}
		}
	}

	return NULL;
}

const RandActivityContinueChongzhiRewardConfig * RandActivityConfig::GetContinueChongzhiRewardInfoWithdayindex2(int day_index, bool is_chongzhi, bool is_bu_fa_reward)
{
	const unsigned int timestamp = RandActivityManager::Instance().GetRanadActivityOpenTime(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2);
	if (timestamp <= 0)
	{
		return NULL;
	}

	int open_day_index = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), timestamp) + 1;

	if (is_chongzhi)
	{
		for (int i = 0; i < static_cast<int>(m_continue_chongzhi_reward_cfg_vec_2.size()); i++)
		{
			if (open_day_index <= m_continue_chongzhi_reward_cfg_vec_2[i].open_server_day)
			{
				return &m_continue_chongzhi_reward_cfg_vec_2[i];
			}
		}
	}
	else
	{
		if (is_bu_fa_reward) open_day_index--;
		int seq = -1;
		for (int i = 0; i < static_cast<int>(m_continue_chongzhi_reward_cfg_vec_2.size()); i++)
		{
			if (-1 != seq && seq != m_continue_chongzhi_reward_cfg_vec_2[i].seq) return NULL;

			if (-1 == seq && open_day_index <= m_continue_chongzhi_reward_cfg_vec_2[i].open_server_day)
			{
				seq = m_continue_chongzhi_reward_cfg_vec_2[i].seq;
			}

			if (open_day_index <= m_continue_chongzhi_reward_cfg_vec_2[i].open_server_day && m_continue_chongzhi_reward_cfg_vec_2[i].lei_chong_day == day_index)
			{
				return &m_continue_chongzhi_reward_cfg_vec_2[i];
			}
		}
	}

	return NULL;
}

int RandActivityConfig::GetContinueConsumeFetchExtraRewardNeedDays()
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CONTINUE_CONSUME);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_continue_consume_other_day_cfg_count; ++i)
	{
		if (m_continue_consume_other_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			return m_continue_consume_other_cfg_list[idx].continue_consume_fetch_extra_reward_need_days;
		}
	}

	return 0;
}

const ItemConfigData * RandActivityConfig::GetContinueConsumeExtraReward()
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CONTINUE_CONSUME);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_continue_consume_other_day_cfg_count; ++i)
	{
		if (m_continue_consume_other_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			return &m_continue_consume_other_cfg_list[idx].continue_consume_extra_reward;
		}
	}

	return NULL;
}

const RandActivityContinueConsumeRewardConfig * RandActivityConfig::GetContinueConsumeRewardInfoWithdayindex(int day_index)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CONTINUE_CONSUME);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_continue_consume_max_reward_day_cfg_count; ++i)
	{
		if (m_continue_consume_reward_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityContinueConsumeRewardDayCfg &day_cfg = m_continue_consume_reward_cfg_list[idx];

	if (day_index <= 0 || day_index > day_cfg.cfg_count + 1 || day_index > RA_CONTINUE_CONSUME_MAX_DAY_INDEX)
	{
		return NULL;
	}

	return &day_cfg.cfg_list[day_index];
}

int RandActivityConfig::InitArmyDayExchangeFlagCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_seq = -1;
	int last_nedd_active_degree = 0;
	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= RA_ARMY_DAY_ACTIVE_DEGREE_EXCHANGE_NUM || seq != last_seq + 1)
		{
			return -1;
		}
		last_seq = seq;

		RandActivityArmyDayExchangeFlagConfig &cfg = m_army_day_exchange_flag_cfg_list[seq];

		if (!PugiGetSubNodeValue(data_element, "need_activity_degree", cfg.need_active_degree) || cfg.need_active_degree <= 0 || cfg.need_active_degree <= last_nedd_active_degree)
		{
			return -2;
		}
		last_nedd_active_degree = cfg.need_active_degree;

		if (!PugiGetSubNodeValue(data_element, "flags_num", cfg.reward_flag_num) || cfg.reward_flag_num <= 0)
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitArmyDayExchangeItemCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int side_type = 0;
		if (!PugiGetSubNodeValue(data_element, "side_type", side_type) || side_type < 0 || side_type >= RA_ARMY_DAY_ARMY_SIDE_NUM)
		{
			return -1;
		}

		RandActivityArmyDayExchangeItemConfig &cfg = m_army_day_exchange_item_cfg_list[side_type];

		if (!PugiGetSubNodeValue(data_element, "need_flags", cfg.need_flags) || cfg.need_flags <= 0)
		{
			return -2;
		}

		PugiXmlNode exchange_item_element = data_element.child("reward_item");
		if (exchange_item_element.empty())
		{
			return -3;
		}
	
		if (!cfg.reward_item.ReadConfig(exchange_item_element))
		{
			return -4;
		}

		data_element = data_element.next_sibling();
	}

	for (int i = 0; i < RA_ARMY_DAY_ARMY_SIDE_NUM; ++ i)
	{
		if (NULL == ITEMPOOL->GetItem(m_army_day_exchange_item_cfg_list[i].reward_item.item_id))
		{
			return -5;
		}
	}

	return 0;
}

const RandActivityArmyDayExchangeFlagConfig * RandActivityConfig::GetArmyDayExhangeFlagCfg(const int seq)
{
	if (seq < 0 || seq >= RA_ARMY_DAY_ACTIVE_DEGREE_EXCHANGE_NUM)
	{
		return NULL;
	}

	if (m_army_day_exchange_flag_cfg_list[seq].need_active_degree > 0)
	{
		return &m_army_day_exchange_flag_cfg_list[seq];
	}

	return NULL;
}

const RandActivityArmyDayExchangeItemConfig * RandActivityConfig::GetArmyDayExhangeItemCfg(const int side_type)
{
	if (side_type < 0 || side_type >= RA_ARMY_DAY_ARMY_SIDE_NUM)
	{
		return NULL;
	}

	return &m_army_day_exchange_item_cfg_list[side_type];
}

int RandActivityConfig::InitCirculationChongzhiRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_circulation_chongzhi_max_reward_count = 0;

	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			stage++;
			last_opengame_day = opengame_day;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		RandActivityCirculationChongzhiRewardConfig &reward_cfg_list = m_circulation_chongzhi_reward_cfg_list[stage];
		reward_cfg_list.cfg_count = 0;
		reward_cfg_list.opengame_day = opengame_day;

		if (reward_cfg_list.cfg_count >= RA_CIRCULATION_CHONGZHI_MAX_REWARD_COUNT)
		{
			return -1000;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "day_index", seq) || seq != reward_cfg_list.cfg_count)
		{
			return -1;
		}

		RandActivityCirculationChongzhiStageRewardConfig &reward_cfg = reward_cfg_list.cfg_list[seq];
		reward_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "need_chongzhi_gold", reward_cfg.need_chongzhi_gold) || reward_cfg.need_chongzhi_gold <= 0)
		{
			return -2;
		}

		{
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				return -100;
			}
			if (!reward_cfg.reward_item.ReadConfig(item_element))
			{
				return -101;
			}
		}

		++reward_cfg_list.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityCirculationChongzhiStageRewardConfig * RandActivityConfig::GetCirculationChongzhiRewardCfg(int reward_seq)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (m_circulation_chongzhi_reward_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityCirculationChongzhiRewardConfig &cfg_list = m_circulation_chongzhi_reward_cfg_list[idx];

	if (reward_seq < 0 || reward_seq >= cfg_list.cfg_count || reward_seq >= RA_CIRCULATION_CHONGZHI_MAX_REWARD_COUNT)
	{
		return NULL;
	}

	return &m_circulation_chongzhi_reward_cfg_list[idx].cfg_list[reward_seq];
}

int RandActivityConfig::InitExtremeLuckyDrawConsumeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}
 
	int seq = 0;
	int last_draw_consume_gold = 0;

	while (!data_element.empty())
	{
	
		if (seq >= RA_EXTREME_LUCKY_DRAW_CONSUME_GOLD_CFG_COUNT)
		{
			return -100;
		}

		RandActivityExtremeLuckyDrawConsumeConfig &cfg = m_extreme_lucky_draw_consume_gold_list[seq];
		
		if (!PugiGetSubNodeValue(data_element, "draw_time", cfg.draw_times) || cfg.draw_times != seq + 1)	//draw_times = seq + 1; 方便下标取值
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "draw_consume", cfg.consume_gold) || cfg.consume_gold <= last_draw_consume_gold)
		{
			return -2;
		}
		last_draw_consume_gold = cfg.consume_gold;

		++ seq;
		data_element = data_element.next_sibling();
	}
	
	return 0;
}

int RandActivityConfig::InitExtremeLuckyDrawRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int is_rare = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{

		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			last_opengame_day = opengame_day;
			++ stage;

			if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
			{
				return -200;
			}

			m_extreme_lucky_reward_cfg_count_list[stage] = 0;
			m_extreme_lucky_total_weight_list[stage] = 0;
			m_extreme_lucky_draw_reward_list[stage].opengame_day = opengame_day;
		}

		if (m_extreme_lucky_reward_cfg_count_list[stage] >= RA_EXTREME_LUCKY_DRAW_REWARD_CFG_COUNT)
		{
			return -300;
		}

		RandActivityExtremeLuckyDrawRewardConfig &cfg = m_extreme_lucky_draw_reward_list[stage].reward_list[m_extreme_lucky_reward_cfg_count_list[stage]];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg.seq) || cfg.seq != m_extreme_lucky_reward_cfg_count_list[stage])
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "min_flush_times", cfg.min_flush_times) || cfg.min_flush_times < 0)
		{
			return -2;
		}
		
		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight <= 0)
		{
			return -3;
		}
		m_extreme_lucky_total_weight_list[stage] += cfg.weight;

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -4;
		}

		if (!cfg.reward_item.ReadConfig(item_element))
		{
			return -5;
		}

		is_rare = 0;
		if (!PugiGetSubNodeValue(data_element, "is_rare", is_rare) || is_rare < 0 || is_rare > 1)
		{
			return -6;
		}

		cfg.is_rare = (0 != is_rare);

		int baodi = 0;
		if (PugiGetSubNodeValue(data_element, "baodi", baodi) && baodi > 0)
		{
			// 保底次数必须不同,因为保底必出物品（除非保底物品也随机）
			if (m_extreme_lucky_draw_reward_list[stage].baodi_map.find(baodi) != m_extreme_lucky_draw_reward_list[stage].baodi_map.end()
				|| m_extreme_lucky_draw_reward_list[stage].baodi_map.size() > 0)
			{
				return -9;
			}

			m_extreme_lucky_draw_reward_list[stage].baodi_map.insert(std::make_pair(baodi, cfg.seq));
		}

		++m_extreme_lucky_reward_cfg_count_list[stage];
		data_element = data_element.next_sibling();
	}

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < stage; ++ i)
	{
		if (m_extreme_lucky_total_weight_list[i] <= 0)
		{
			return -1200;
		}
	}

	return 0;
}

int RandActivityConfig::InitExtremeLuckyDrawReturnRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			last_opengame_day = opengame_day;
			++stage;

			if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
			{
				return -200;
			}

			m_extreme_lucky_return_reward_list[stage].opengame_day = opengame_day;
		}

		RandActivityExtremeLuckyReturnRewardCfg &cfg_list = m_extreme_lucky_return_reward_list[stage];
		cfg_list.opengame_day = opengame_day;

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= RA_EXTREME_LUCKY_RETURN_REWARD_ONCE_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "draw_times", cfg_list.stage_cfg_list[seq].draw_times) ||
			cfg_list.stage_cfg_list[seq].draw_times < 0)
		{
			return -3;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -4;
		}

		if (!cfg_list.stage_cfg_list[seq].reward_item.ReadConfig(item_element))
		{
			return -5;
		}

		++cfg_list.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::GetExtremeLuckyDrawConsumeByDrawTimes(int draw_times) const
{
	if (draw_times > 0 && draw_times <= RA_EXTREME_LUCKY_DRAW_CONSUME_GOLD_CFG_COUNT)
	{
		return m_extreme_lucky_draw_consume_gold_list[draw_times - 1].consume_gold;
	}

	return 0;
}

bool RandActivityConfig::GetExtremLuckyFlushRewardList(int flush_times, RAExtremeLuckyRewardInfo *reward_list, const int list_length, int server_chou_times)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_EXTREME_LUCKY);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_extreme_lucky_draw_reward_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return false;
	}

	if (list_length > m_extreme_lucky_reward_cfg_count_list[stage] || NULL == reward_list)
	{
		return false;
	}

	if (m_extreme_lucky_total_weight_list[stage] <= 0)
	{
		return false;
	}

	int rand_weight = 0;
	int cur_get_cfg_count = 0;
	bool is_repeat = false;
	int cycle_times = 0;		// 防止死循环

	// 保底物品
	if (server_chou_times > 0)
	{
		for (std::map<int, int>::iterator it = m_extreme_lucky_draw_reward_list[stage].baodi_map.begin(); it != m_extreme_lucky_draw_reward_list[stage].baodi_map.end(); ++it)
		{
			const int baodi_times = it->first;
			const int reward_seq = it->second;

			if (baodi_times <= 0 || 0 != server_chou_times % baodi_times) continue;
			if (reward_seq < 0 || reward_seq >= RA_EXTREME_LUCKY_DRAW_REWARD_CFG_COUNT) continue;

			if (m_extreme_lucky_draw_reward_list[stage].reward_list[reward_seq].seq == reward_seq)
			{
				reward_list[cur_get_cfg_count].seq = m_extreme_lucky_draw_reward_list[stage].reward_list[reward_seq].seq;
				reward_list[cur_get_cfg_count].weight = m_extreme_lucky_draw_reward_list[stage].reward_list[reward_seq].weight;
				++cur_get_cfg_count;
				break;
			}
		}
	}

	for (; cur_get_cfg_count < list_length && cycle_times < 100;)
	{
		rand_weight = RandomNum(m_extreme_lucky_total_weight_list[stage]);
		++ cycle_times;
		
		for (int i = 0; i < m_extreme_lucky_reward_cfg_count_list[stage]; ++ i)
		{
			is_repeat = false;

			if (rand_weight < m_extreme_lucky_draw_reward_list[stage].reward_list[i].weight &&
				flush_times >= m_extreme_lucky_draw_reward_list[stage].reward_list[i].min_flush_times)
			{
				//查重
				for (int j = 0; j < cur_get_cfg_count; ++ j)
				{
					if(reward_list[j].seq == m_extreme_lucky_draw_reward_list[stage].reward_list[i].seq)
					{
						is_repeat = true;
						break;
					}
				}

				if (!is_repeat)
				{
					reward_list[cur_get_cfg_count].seq = m_extreme_lucky_draw_reward_list[stage].reward_list[i].seq;
					reward_list[cur_get_cfg_count].weight = m_extreme_lucky_draw_reward_list[stage].reward_list[i].weight;
					 ++ cur_get_cfg_count;
					break;
				}
			}

			//如果重复也减去其权重，剩余权重将会是负数
			if (!is_repeat)
			{
				rand_weight -= m_extreme_lucky_draw_reward_list[stage].reward_list[i].weight;
			}
		}
	}

	//for (int i = 0; i < list_length; ++ i)
	//{
	//	if (m_extreme_lucky_draw_reward_list[stage].reward_list[reward_list[i].seq].is_rare)
	//	{
	//		reward_list[i].invalid = 1;			// 第一珍稀不可抽取
	//		break;
	//	}
	//}

	return true;
}

const RandActivityExtremeLuckyDrawRewardConfig * RandActivityConfig::GetExtremeLuckyRewardCfg(const int seq)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_EXTREME_LUCKY);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_extreme_lucky_draw_reward_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	if (seq < 0 || seq >= m_extreme_lucky_reward_cfg_count_list[stage] || seq >= RA_EXTREME_LUCKY_DRAW_REWARD_CFG_COUNT)
	{
		return NULL;
	}

	return &m_extreme_lucky_draw_reward_list[stage].reward_list[seq];
}

const RandActivityExtremeLuckyReturnRewardStageCfg * RandActivityConfig::GetExtremeLuckyReturnRewardCfg(const int seq)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_EXTREME_LUCKY);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (m_extreme_lucky_return_reward_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityExtremeLuckyReturnRewardCfg &cfg_list = m_extreme_lucky_return_reward_list[idx];
	if (seq < 0 || seq >= MAX_RA_MAP_HUNT_SERVER_REWARD_COUNT || seq >= cfg_list.cfg_count)
	{
		return NULL;
	}

	return &cfg_list.stage_cfg_list[seq];
}

int RandActivityConfig::InitMiJingXunBao2Cfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int is_rare_temp = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (last_opengame_day != opengame_day)
		{
			last_opengame_day = opengame_day;
			++ stage;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (m_mijingxunbao2_max_seq_list[stage] >= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ)
		{
			return -1;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_mijingxunbao2_max_seq_list[stage] + 1)
		{
			return -11;
		}

		m_mijingxunbao2_cfg_list[stage].opengame_day = opengame_day;
		RandActivityMiJingXunBaoConfig &mijingxunbao2_cfg = m_mijingxunbao2_cfg_list[stage].cfg_list[seq];
		mijingxunbao2_cfg.seq = seq;
		m_mijingxunbao2_max_seq_list[stage] = seq;

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -12;
		}

		if (!mijingxunbao2_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -13;
		}

		is_rare_temp = 0;
		if (!PugiGetSubNodeValue(data_element, "is_rare", is_rare_temp) || is_rare_temp < 0 || is_rare_temp > 1)
		{
			return -14;
		}
		mijingxunbao2_cfg.is_rare = (0 != is_rare_temp);

		if (!PugiGetSubNodeValue(data_element, "weight", mijingxunbao2_cfg.weight) || mijingxunbao2_cfg.weight < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(data_element, "auto_add_weight", mijingxunbao2_cfg.auto_add_weight) || mijingxunbao2_cfg.auto_add_weight < 0)
		{
			return -16;
		}

		if (mijingxunbao2_cfg.auto_add_weight > 0)
		{
			if (m_mijingxunbao2_auto_add_cfg_count_list[stage] >= RAND_ACTIVITY_MIJINGXUNBAO_AUTO_ADD_WEIGHT_CFG_COUNT)
			{
				return -18;
			}

			m_mijingxunbao2_add_weight_cfg_list[stage].opengame_day = opengame_day;
			m_mijingxunbao2_add_weight_cfg_list[stage].add_weight_cfg_list[m_mijingxunbao2_auto_add_cfg_count_list[stage]].add_weight = mijingxunbao2_cfg.auto_add_weight;
			m_mijingxunbao2_add_weight_cfg_list[stage].add_weight_cfg_list[m_mijingxunbao2_auto_add_cfg_count_list[stage]].seq = mijingxunbao2_cfg.seq;

			++ m_mijingxunbao2_auto_add_cfg_count_list[stage];
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::GetMiJingXunBao2AddWeightSeq(int index)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MIJINGXUNBAO2);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_mijingxunbao2_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	if (index < 0 || index >= m_mijingxunbao2_auto_add_cfg_count_list[stage])
	{
		return 0;
	}

	return m_mijingxunbao2_add_weight_cfg_list[stage].add_weight_cfg_list[index].seq;
}

int RandActivityConfig::GetMiJingXunBao2CfgAddWeight(int seq)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MIJINGXUNBAO2);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_mijingxunbao2_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	if (seq <= 0 || seq > m_mijingxunbao2_max_seq_list[stage] || seq > RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ)
	{
		return 0;
	}

	for (int index = 0; index < m_mijingxunbao2_auto_add_cfg_count_list[stage]; index++)
	{
		if (m_mijingxunbao2_add_weight_cfg_list[stage].add_weight_cfg_list[index].seq == seq)
		{
			return m_mijingxunbao2_add_weight_cfg_list[stage].add_weight_cfg_list[index].add_weight;
		}
	}

	return 0;
}

int RandActivityConfig::GetMiJingXunBao2TaoGold(int type)
{
	if (type < RA_MIJINGXUNBAO_CHOU_TYPE_1 || type >= RA_MIJINGXUNBAO_CHOU_TYPE_MAX)
	{
		return 0;
	}

	switch(type)
	{
	case RA_MIJINGXUNBAO_CHOU_TYPE_1 : 
		{
			return m_mijingxunbao2_once_gold;
		}
		break;

	case RA_MIJINGXUNBAO_CHOU_TYPE_10 : 
		{
			return m_mijingxunbao2_tentimes_gold;
		}
		break;

	case RA_MIJINGXUNBAO_CHOU_TYPE_50 : 
		{
			return m_mijingxunbao2_fifthtimes_gold;
		}
		break;

	default:
		return 0;
	}
}

int RandActivityConfig::GetMiJingXunBao2AutoWeightAddCount()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MIJINGXUNBAO2);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_mijingxunbao2_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	return m_mijingxunbao2_auto_add_cfg_count_list[stage];
}

bool RandActivityConfig::GetMiJingXunBao2RandomResult(MiJingXunBaoAddWeightInfo *self_weight, int length, int is_free, RandActivityMiJingXunBaoConfig *result_list, int result_list_length)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MIJINGXUNBAO2);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_mijingxunbao2_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return false;
	}

	if (NULL == self_weight || length < 0 || NULL == result_list || result_list_length <= 0)
	{
		return false;
	}

	int all_weight = 0;
	int all_cfg_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ + 1] = {0};

	for (int i = 1; i <= m_mijingxunbao2_max_seq_list[stage] && i <= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ; ++ i)
	{
		all_cfg_weight_list[i] = m_mijingxunbao2_cfg_list[stage].cfg_list[i].weight;

		if (!is_free)	// 非免费刷新 增加自己权重
		{
			for (int self_index = 0; self_index < length; ++ self_index)
			{
				if (self_weight[self_index].seq == m_mijingxunbao2_cfg_list[stage].cfg_list[i].seq)
				{
					all_cfg_weight_list[i] += self_weight[self_index].weight;
				}
			}
		}

		all_weight += all_cfg_weight_list[i];
	}

	if (all_weight <= 0)
	{
		return false;
	}

	for (int result_index = 0; result_index < result_list_length && all_weight > 0; ++ result_index)
	{
		int rand_weight = RandomNum(all_weight);
		for (int i = 1; i <= m_mijingxunbao2_max_seq_list[stage] && i <= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ; ++ i)
		{
			if (rand_weight < all_cfg_weight_list[i])
			{
				result_list[result_index] = m_mijingxunbao2_cfg_list[stage].cfg_list[i];

				for (int self_index = 0; self_index < length; ++ self_index)
				{
					if (result_list[result_index].seq == self_weight[self_index].seq) //1 抽到珍稀，把该奖励权重置0
					{
						all_cfg_weight_list[self_weight[self_index].seq] -= self_weight[self_index].weight;
						all_weight -= self_weight[self_index].weight;
						if (all_weight <= 0)
						{
							return false;
						}

						self_weight[self_index].weight = 0;
						self_weight[self_index].tao_none_rare_times = 0;
					}
					else
					{
						self_weight[self_index].tao_none_rare_times ++;
						int add_weight = this->GetMiJingXunBao2CfgAddWeight(self_weight[self_index].seq);

						if (self_weight[self_index].tao_none_rare_times >= this->GetMiJingXunBao2AddWeightTaoTimes())
						{
							all_weight += add_weight;

							self_weight[self_index].weight += add_weight;
							all_cfg_weight_list[self_weight[self_index].seq] += self_weight[self_index].weight;

							self_weight[self_index].tao_none_rare_times -= this->GetMiJingXunBao2AddWeightTaoTimes();
						}
					}
				}
				break;
			}

			rand_weight -= all_cfg_weight_list[i];
		}
	}

	return true;
}

int RandActivityConfig::InitMoneyTree2Cfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int stage = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	m_money_tree2_max_stage = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			last_opengame_day = opengame_day;
			++ stage;
			++ m_money_tree2_max_stage;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		int cfg_type = -1;
		if (!PugiGetSubNodeValue(data_element, "cfg_type", cfg_type) || cfg_type < 0 || cfg_type > 1)
		{
			return -20;
		}

		if (0 == cfg_type)
		{
			if (m_money_tree2_reward_cfg_count_list[stage] >= RA_MONEY_TREE_REWARD_CFG_COUNT)
			{
				return -21;
			}

			int seq = -1;
			if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_money_tree2_reward_cfg_count_list[stage])
			{
				return -1;
			}

			m_money_tree2_reward_cfg_list[stage].opengame_day = opengame_day;
			RandActivityMoneyTreeRewardConfig &reward_cfg = m_money_tree2_reward_cfg_list[stage].reward_cfg_list[seq];
			reward_cfg.seq = seq;

			if (!PugiGetSubNodeValue(data_element, "weight", reward_cfg.weight) || reward_cfg.weight <= 0)
			{
				return -2;
			}

			int is_broadcast = 0;
			if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast) || is_broadcast < 0 || is_broadcast > 1)
			{
				return -3;
			}
			reward_cfg.is_broadcast = (is_broadcast > 0);

			int has_reward = false;
			if (PugiGetSubNodeValue(data_element, "prize_pool_percent", reward_cfg.prize_pool_percent))
			{
				if (reward_cfg.prize_pool_percent < 0 || reward_cfg.prize_pool_percent > 100)
				{
					return -5;
				}

				has_reward = true;
			}

			m_money_tree2_total_weight_list[stage] += reward_cfg.weight;

			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -4;
			}

			if (reward_cfg.reward_item.ReadConfig(reward_item_element))
			{
				if (has_reward)
				{
					return -6;
				}

				has_reward = true;
				m_money_tree2_item_reward_total_weight_list[stage] += reward_cfg.weight;
			}

			if (!has_reward)
			{
				return -7;
			}

			++ m_money_tree2_reward_cfg_count_list[stage];
		}
		else if (1 == cfg_type)		// 全服奖励
		{
			if (m_money_tree2_server_reward_cfg_count_list[stage] >= RA_MONEY_TREE_SERVER_REWARD_CFG_COUNT)
			{
				return -22;
			}

			int seq = -1;
			if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_money_tree2_server_reward_cfg_count_list[stage])
			{
				return -10;
			}

			m_money_tree2_server_reward_cfg_list[stage].opengame_day = opengame_day;
			RandActivityMoneyTreeServerRewardConfig &server_reward_cfg = m_money_tree2_server_reward_cfg_list[stage].server_reward_cfg_list[seq];
			server_reward_cfg.seq = seq;

			if (!PugiGetSubNodeValue(data_element, "server_rock_times", server_reward_cfg.server_rock_times) || server_reward_cfg.server_rock_times <= 0)
			{
				return -11;
			}

			if (!PugiGetSubNodeValue(data_element, "vip_limit", server_reward_cfg.vip_limit) || server_reward_cfg.vip_limit < 0 || server_reward_cfg.vip_limit > MAX_VIP_LEVEL)
			{
				return -14;
			}

			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -15;
			}

			if (!server_reward_cfg.reward_item.ReadConfig(reward_item_element))
			{
				return -16;
			}

			++ m_money_tree2_server_reward_cfg_count_list[stage];
		}

		data_element = data_element.next_sibling();
	}


	for (int i = 0; i < m_money_tree2_max_stage; ++ i)
	{
		if (m_money_tree2_total_weight_list[stage] <= 0)
		{
			return -1000;
		}

		if (m_money_tree2_item_reward_total_weight_list[stage] <= 0)
		{
			return -2000;
		}
	}

	return 0;
}

const RandActivityMoneyTreeRewardConfig * RandActivityConfig::GetRandomMoneyTree2RewardCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MONEY_TREE2);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_money_tree2_reward_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	if (m_money_tree2_total_weight_list[stage] <= 0) return NULL;

	int random_weight = RandomNum(m_money_tree2_total_weight_list[stage]);
	for (int req = 0; req < m_money_tree2_reward_cfg_count_list[stage] && req < RA_MONEY_TREE_REWARD_CFG_COUNT; ++ req)
	{
		RandActivityMoneyTreeRewardConfig &cfg = m_money_tree2_reward_cfg_list[stage].reward_cfg_list[req];
		if (random_weight < cfg.weight)
		{
			return &cfg;
		}

		random_weight -= cfg.weight;
	}

	return NULL;
}

const RandActivityMoneyTreeRewardConfig * RandActivityConfig::GetRandomMoneyTree2ItemRewardCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MONEY_TREE2);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_money_tree2_reward_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	if (m_money_tree2_item_reward_total_weight_list[stage] <= 0) return NULL;

	int random_weight = RandomNum(m_money_tree2_item_reward_total_weight_list[stage]);
	for (int req = 0; req < m_money_tree2_reward_cfg_count_list[stage] && req < RA_MONEY_TREE_REWARD_CFG_COUNT; ++ req)
	{
		RandActivityMoneyTreeRewardConfig &cfg = m_money_tree2_reward_cfg_list[stage].reward_cfg_list[req];
		if (cfg.prize_pool_percent > 0)
		{
			continue;
		}

		if (random_weight < cfg.weight)
		{
			return &cfg;
		}

		random_weight -= cfg.weight;
	}

	return NULL;
}

const RandActivityMoneyTreeServerRewardConfig * RandActivityConfig::GetMoneyTree2ServerCfgWithReq(int req)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MONEY_TREE2);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_money_tree2_server_reward_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	if (req >= 0 && req < m_money_tree2_server_reward_cfg_count_list[stage] && req < RA_MONEY_TREE_SERVER_REWARD_CFG_COUNT)
	{
		return &m_money_tree2_server_reward_cfg_list[stage].server_reward_cfg_list[req];
	}

	return NULL;
}

const RandActivityMoneyTreeServerRewardConfig * RandActivityConfig::GetTreeServerCfgWithServerMoneyTree2Times(int server_total_money_tree_times)
{

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MONEY_TREE2);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day > m_money_tree2_server_reward_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	for (int req = m_money_tree2_server_reward_cfg_count_list[stage] - 1; req >= 0; -- req)
	{
		RandActivityMoneyTreeServerRewardConfig &cfg = m_money_tree2_server_reward_cfg_list[stage].server_reward_cfg_list[req];
		if (server_total_money_tree_times >= cfg.server_rock_times)
		{
			return &cfg;
		}
	}

	return NULL;
}

int RandActivityConfig::InitNationalDayCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_national_day_cfg_count = 0;

	while (!data_element.empty())
	{
		RandActivityNationalDayConfig &tmp_cfg = m_national_day_exchange_cfg_list[m_national_day_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", tmp_cfg.seq) ||  tmp_cfg.seq < 0 || tmp_cfg.seq != m_national_day_cfg_count)
		{
			return -1;
		}

		PugiXmlNode consume_item1 = data_element.child("consume_item1");
		if (!consume_item1.empty())
		{
			if (tmp_cfg.consume_item_list[tmp_cfg.consume_count].ReadConfig(consume_item1))
			{
				++ tmp_cfg.consume_count;
			}
		}

		PugiXmlNode consume_item2 = data_element.child("consume_item2");
		if (!consume_item2.empty())
		{
			if (tmp_cfg.consume_item_list[tmp_cfg.consume_count].ReadConfig(consume_item2))
			{
				++ tmp_cfg.consume_count;
			}
		}

		PugiXmlNode consume_item3 = data_element.child("consume_item3");
		if (!consume_item3.empty())
		{
			if (tmp_cfg.consume_item_list[tmp_cfg.consume_count].ReadConfig(consume_item3))
			{
				++ tmp_cfg.consume_count;
			}
		}

		PugiXmlNode consume_item4 = data_element.child("consume_item4");
		if (!consume_item4.empty())
		{
			if (tmp_cfg.consume_item_list[tmp_cfg.consume_count].ReadConfig(consume_item4))
			{
				++ tmp_cfg.consume_count;
			}
		}

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (!reward_item.empty())
		{
			if (!tmp_cfg.reward_item.ReadConfig(reward_item))
			{
				return -2;
			}
		}

		if (tmp_cfg.consume_count <= 0 || tmp_cfg.consume_count > ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM)
		{
			return -3;
		}

		++ m_national_day_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityNationalDayConfig * RandActivityConfig::GetNationalDayRewardCfg(const int seq)
{
	if (seq < 0|| seq >= m_national_day_cfg_count || seq >= RA_NATIONAL_DAY_EXCHANGE_CFG_COUNT)
	{
		return NULL;
	}

	return &m_national_day_exchange_cfg_list[seq];
}

const RandActivityNewTotalChargeRewardConfig::ConfigItem * RandActivityConfig::GetNewTotalChargeRewardWithSeq(int seq)
{
	const RandActivityNewTotalChargeRewardConfig *reward_cfg = this->GetNewTotalChargeRewardCfg();
	if (NULL == reward_cfg)
	{
		return NULL;
	}

	if (seq >= 0 && seq < RandActivityNewTotalChargeRewardConfig::CONFIG_ITEM_MAX_COUNT && seq < reward_cfg->cfg_count)
	{
		return &reward_cfg->cfg_list[seq];
	}

	return NULL;
}

const RandActivityNewTotalChargeRewardConfig * RandActivityConfig::GetNewTotalChargeRewardCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		RandActivityNewTotalChargeRewardConfig *total_chognzhi_reward_cfg = &m_diff_new_totalcharge_reward_list[i];
		if (opengame_day <= total_chognzhi_reward_cfg->opengame_day)
		{
			return total_chognzhi_reward_cfg;
		}
	}

	return NULL;
}

const RandActivityNewTotalChargeRewardConfig::ConfigItem * RandActivityConfig::GetNewTotalCharge1RewardWithSeq(int seq)
{
	const RandActivityNewTotalChargeRewardConfig *reward_cfg = this->GetNewTotalCharge1RewardCfg();
	if (NULL == reward_cfg)
	{
		return NULL;
	}

	if (seq >= 0 && seq < RandActivityNewTotalChargeRewardConfig::CONFIG_ITEM_MAX_COUNT && seq < reward_cfg->cfg_count)
	{
		return &reward_cfg->cfg_list[seq];
	}

	return NULL;
}

const RandActivityNewTotalChargeRewardConfig * RandActivityConfig::GetNewTotalCharge1RewardCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		RandActivityNewTotalChargeRewardConfig *total_chognzhi_reward_cfg = &m_diff_new_totalcharge1_reward_list[i];
		if (opengame_day <= total_chognzhi_reward_cfg->opengame_day)
		{
			return total_chognzhi_reward_cfg;
		}
	}

	return NULL;
}

const RandActivityNewTotalChargeRewardConfig::ConfigItem * RandActivityConfig::GetNewTotalCharge2RewardWithSeq(int seq)
{
	const RandActivityNewTotalChargeRewardConfig *reward_cfg = this->GetNewTotalCharge2RewardCfg();
	if (NULL == reward_cfg)
	{
		return NULL;
	}

	if (seq >= 0 && seq < RandActivityNewTotalChargeRewardConfig::CONFIG_ITEM_MAX_COUNT && seq < reward_cfg->cfg_count)
	{
		return &reward_cfg->cfg_list[seq];
	}

	return NULL;
}
const RandActivityNewTotalChargeRewardConfig * RandActivityConfig::GetNewTotalCharge2RewardCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		RandActivityNewTotalChargeRewardConfig *total_chognzhi_reward_cfg = &m_diff_new_totalcharge2_reward_list[i];
		if (opengame_day <= total_chognzhi_reward_cfg->opengame_day)
		{
			return total_chognzhi_reward_cfg;
		}
	}

	return NULL;
}

const RandActivityNewTotalChargeRewardConfig::ConfigItem * RandActivityConfig::GetNewTotalCharge3RewardWithSeq(int seq)
{
	const RandActivityNewTotalChargeRewardConfig *reward_cfg = this->GetNewTotalCharge3RewardCfg();
	if (NULL == reward_cfg)
	{
		return NULL;
	}

	if (seq >= 0 && seq < RandActivityNewTotalChargeRewardConfig::CONFIG_ITEM_MAX_COUNT && seq < reward_cfg->cfg_count)
	{
		return &reward_cfg->cfg_list[seq];
	}

	return NULL;
}

const RandActivityNewTotalChargeRewardConfig * RandActivityConfig::GetNewTotalCharge3RewardCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		RandActivityNewTotalChargeRewardConfig *total_chognzhi_reward_cfg = &m_diff_new_totalcharge3_reward_list[i];
		if (opengame_day <= total_chognzhi_reward_cfg->opengame_day)
		{
			return total_chognzhi_reward_cfg;
		}
	}

	return NULL;
}

const RandActivityMagicShopConfig * RandActivityConfig::GetMagicShopCfg(int index)
{
	if (index < 0 || index >= RA_MAGIC_SHOP_ITEM_MAX_INDEX) return NULL;

	time_t activity_begin_time = ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_MAGIC_SHOP);
	time_t now_time = EngineAdapter::Instance().Time();
	int act_real_open_day = GetDayIndex(activity_begin_time, now_time) + 1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MAGIC_SHOP);

	for (int i = 0; i < (int)m_magic_shop_cfg_list.size(); ++ i)
	{
		if (opengame_day <= m_magic_shop_cfg_list[i].opengame_day && act_real_open_day == m_magic_shop_cfg_list[i].activity_day &&
			index == m_magic_shop_cfg_list[i].index)
		{
			return &m_magic_shop_cfg_list[i];
		}
	}

	return NULL;
}

int RandActivityConfig::InitZhenbaoge2Cfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_zhenbaoge2_max_seq = 0;
	m_zhenbaoge2_reward_max_seq = 0;
	m_zhenbaoge2_auto_add_cfg_count = 0;

	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			stage++;
			last_opengame_day = opengame_day;
			m_zhenbaoge2_max_seq = stage + 1;
			m_zhenbaoge2_reward_max_seq = stage + 1;
			m_zhenbaoge2_auto_add_cfg_count = stage + 1;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		int type = 0;
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "cfg_type", type) || type < 0 || type > 1)
		{
			return -1;
		}

		if (type <= 0)		// 刷新物品	
		{
			RandActivityZhenBaoGeDayCfg &day_cfg = m_zhenbaoge2_cfg_list[stage];
			++day_cfg.cfg_count;
			day_cfg.opengame_day = opengame_day;

			if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq <= 0 || seq > RAND_ACTIVITY_ZHENBAOGE2_MAX_SEQ)
			{
				return -11;
			}

			RandActivityZhenBaoGeConfig &zhenbaoge_cfg = day_cfg.cfg_list[seq];
			zhenbaoge_cfg.seq = seq;
			m_zhenbaoge2_max_seq = seq;

			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -12;
			}

			if (!zhenbaoge_cfg.reward_item.ReadConfig(reward_item_element))
			{
				return -13;
			}

			if (!PugiGetSubNodeValue(data_element, "is_rare", zhenbaoge_cfg.is_rare) || zhenbaoge_cfg.is_rare < 0 || zhenbaoge_cfg.is_rare > 1)
			{
				return -14;
			}

			if (!PugiGetSubNodeValue(data_element, "weight", zhenbaoge_cfg.weight) || zhenbaoge_cfg.weight < 0)
			{
				return -15;
			}

			if (!PugiGetSubNodeValue(data_element, "auto_add_weight", zhenbaoge_cfg.auto_add_weight) || zhenbaoge_cfg.auto_add_weight < 0)
			{
				return -16;
			}

			if (!PugiGetSubNodeValue(data_element, "buy_consume_gold", zhenbaoge_cfg.buy_consume_gold) || zhenbaoge_cfg.buy_consume_gold <= 0)
			{
				return -17;
			}

			if (zhenbaoge_cfg.auto_add_weight > 0)
			{
				RandActivityZhenBaoGeAddWeightDayCfg &weight_day_cfg = m_zhenbaoge2_add_weight_cfg_list[stage];
				weight_day_cfg.opengame_day = opengame_day;

				if (weight_day_cfg.cfg_count >= RAND_ACTIVITY_ZHENBAOGE_AUTO_ADD_WEIGHT_CFG_COUNT || weight_day_cfg.cfg_count < 0)
				{
					return -18;
				}

				RandActivityZhenBaoGeAddWeightConfig &weight_cfg = weight_day_cfg.cfg_list[weight_day_cfg.cfg_count];
				weight_cfg.add_weight = zhenbaoge_cfg.auto_add_weight;
				weight_cfg.seq = zhenbaoge_cfg.seq;

				++weight_day_cfg.cfg_count;
			}

			int baodi = 0;
			if (PugiGetSubNodeValue(data_element, "baodi", baodi) && baodi > 0)
			{
				if (day_cfg.baodi_map.size() > 0 || day_cfg.baodi_map.find(baodi) != day_cfg.baodi_map.end())
				{
					return -19;
				}

				day_cfg.baodi_map.insert(std::make_pair(baodi, seq));
			}
		}
		else	// 全服奖励
		{
			RandActivityZhenBaoGeServerDayCfg &day_cfg = m_zhenbaoge2_reward_cfg_list[stage];
			++day_cfg.cfg_count;
			day_cfg.opengame_day = opengame_day;

			if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq <= 0 || seq > RAND_ACTIVITY_ZHENBAOGE2_MAX_RETURN_REWARD_COUNT)
			{
				return -51;
			}

			if (seq < 0 || seq > RAND_ACTIVITY_ZHENBAOGE_SERVER_MAX_SEQ)
			{
				return -52;
			}

			RandActivityZhenBaoGeServerConfig &server_cfg = day_cfg.cfg_list[seq];
			server_cfg.seq = seq;

			if (!PugiGetSubNodeValue(data_element, "can_fetch_times", server_cfg.can_fetch_times) || server_cfg.can_fetch_times <= 0)
			{
				return -53;
			}

			if (!PugiGetSubNodeValue(data_element, "vip_limit", server_cfg.vip_limit) || server_cfg.vip_limit < 0 || server_cfg.vip_limit > MAX_VIP_LEVEL)
			{
				return -54;
			}

			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -55;
			}

			if (!server_cfg.reward_item.ReadConfig(reward_item_element))
			{
				return -56;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::GetZhenbaoge2AddWeightSeq(int index)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_ZHENBAOGE2);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_zhenbaoge2_auto_add_cfg_count; ++i)
	{
		if (m_zhenbaoge2_add_weight_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	RandActivityZhenBaoGeAddWeightDayCfg &day_cfg = m_zhenbaoge2_add_weight_cfg_list[idx];

	if (index < 0 || index >= day_cfg.cfg_count)
	{
		return 0;
	}

	return day_cfg.cfg_list[index].seq;
}

const RandActivityZhenBaoGeConfig * RandActivityConfig::GetZhenbaoge2Cfg(int seq)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_ZHENBAOGE2);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_zhenbaoge2_max_seq; ++i)
	{
		if (m_zhenbaoge2_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityZhenBaoGeDayCfg &day_cfg = m_zhenbaoge2_cfg_list[idx];

	if (seq <= 0 || seq > day_cfg.cfg_count || seq > RAND_ACTIVITY_ZHENBAOGE_MAX_SEQ)
	{
		return NULL;
	}

	return &day_cfg.cfg_list[seq];
}

const RandActivityZhenBaoGeAddWeightConfig * RandActivityConfig::GetZhenbaoge2AddWeight(int index)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_ZHENBAOGE2);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_zhenbaoge2_auto_add_cfg_count; ++i)
	{
		if (m_zhenbaoge2_add_weight_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityZhenBaoGeAddWeightDayCfg &day_cfg = m_zhenbaoge2_add_weight_cfg_list[idx];

	if (index < 0 || index >= day_cfg.cfg_count || index >= RAND_ACTIVITY_ZHENBAOGE_AUTO_ADD_WEIGHT_CFG_COUNT)
	{
		return NULL;
	}

	return &day_cfg.cfg_list[index];
}

const RandActivityZhenBaoGeServerConfig * RandActivityConfig::GetZhenbaoge2ServerCfg(int seq)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_ZHENBAOGE2);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_zhenbaoge2_reward_max_seq; ++i)
	{
		if (m_zhenbaoge2_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityZhenBaoGeServerDayCfg &day_cfg = m_zhenbaoge2_reward_cfg_list[idx];

	if (seq <= 0 || seq > day_cfg.cfg_count || seq > RAND_ACTIVITY_ZHENBAOGE_SERVER_MAX_SEQ)
	{
		return NULL;
	}

	return &day_cfg.cfg_list[seq];
}

bool RandActivityConfig::GetZhenbaoge2RandomResult(const ZhenbaogeAddWeightInfo *self_weight, int length, int is_free, RandActivityZhenBaoGeConfig *result_list, int result_list_length, int server_chou_times)
{
	if (NULL == self_weight || length < 0 || NULL == result_list || result_list_length <= 0)
	{
		return false;
	}

	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_ZHENBAOGE2);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_zhenbaoge2_max_seq; ++i)
	{
		if (m_zhenbaoge2_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityZhenBaoGeDayCfg &day_cfg = m_zhenbaoge2_cfg_list[idx];

	int all_weight = 0;
	int temp_list[RAND_ACTIVITY_ZHENBAOGE_MAX_SEQ + 1] = {0};

	for (int i = 0; i <= day_cfg.cfg_count && i <= RAND_ACTIVITY_ZHENBAOGE_MAX_SEQ; ++ i)
	{
		temp_list[i] = day_cfg.cfg_list[i].weight;

		if (!is_free)	// 非免费刷新 增加自己权重
		{
			for (int self_index = 0; self_index < length; ++ self_index)
			{
				if (self_weight[self_index].seq == day_cfg.cfg_list[i].seq)
				{
					temp_list[i] += self_weight[self_index].weight;
				}
			}
		}

		all_weight += temp_list[i];
	}

	if (all_weight <= 0)
	{
		return false;
	}

	int get_count = 0;

	// 保底次数
	if (server_chou_times > 0)
	{
		for (std::map<int, int>::iterator it = day_cfg.baodi_map.begin(); it != day_cfg.baodi_map.end(); ++it)
		{
			const int baodi_times = it->first;
			const int reward_seq = it->second;

			if (baodi_times > 0 && 0 == server_chou_times % baodi_times && reward_seq >= 0 && reward_seq <= day_cfg.cfg_count && reward_seq <= RAND_ACTIVITY_ZHENBAOGE2_MAX_SEQ)
			{
				result_list[get_count] = day_cfg.cfg_list[reward_seq];
				++get_count;
				break;
			}
		}
	}

	for (; get_count < result_list_length; ++get_count)
	{
		int rand_weight = RandomNum(all_weight);

		for (int i = 0; i <= day_cfg.cfg_count && i <= RAND_ACTIVITY_ZHENBAOGE_MAX_SEQ; ++ i)
		{
			if (rand_weight < temp_list[i])
			{
				result_list[get_count] = day_cfg.cfg_list[i];
				all_weight -= temp_list[i];
				temp_list[i] = 0;

				break;
			}

			rand_weight -= temp_list[i];
		}
	}

	return true;
}

int RandActivityConfig::InitPromotingPositionOutsideCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_promoting_position_outside_max_seq = -1;

	int last_opengame_day = 0;

	//static const int GRID_MAX_COUNT = 25;
	int grid_count = 0;

	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_promoting_position_outside_max_seq + 1)
		{
			return -1;
		}

		RandActivityPromotingPositionConfig &outside_cfg = m_promoting_position_outside_cfg_list[seq];
		outside_cfg.seq = seq;
		m_promoting_position_outside_max_seq = seq;

		if (!PugiGetSubNodeValue(data_element, "reward_type", outside_cfg.reward_type) || outside_cfg.reward_type < RA_PROMOTING_POSITION_REWARD_TYPE_GOLD || outside_cfg.reward_type > RA_PROMOTING_POSITION_REWARD_TYPE_SPLIT)
		{
			return -2;
		}

		switch (outside_cfg.reward_type)
		{
		case RA_PROMOTING_POSITION_REWARD_TYPE_GOLD:
			{
				if (!PugiGetSubNodeValue(data_element, "reward_gold_rate", outside_cfg.reward_gold_rate) || outside_cfg.reward_gold_rate < 0)
				{
					return -3;
				}
			}
			break;

		case RA_PROMOTING_POSITION_REWARD_TYPE_ITEM:
			{
				PugiXmlNode item_element = data_element.child("reward_item");
				if (item_element.empty())
				{
					return -100;
				}

				if (!outside_cfg.reward_item.ReadConfig(item_element))
				{
					return -101;
				}
			}
			break;
		}

		if (!PugiGetSubNodeValue(data_element, "free_play_weight", outside_cfg.free_play_weight) || outside_cfg.free_play_weight < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "gold_play_weight", outside_cfg.gold_play_weight) || outside_cfg.gold_play_weight < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "extra_time_play_weight", outside_cfg.extra_time_play_weight) || outside_cfg.extra_time_play_weight < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "item_play_weight", outside_cfg.item_play_weight) || outside_cfg.item_play_weight < 0)
		{
			return -7;
		}

		int is_rare = 0;
		if (!PugiGetSubNodeValue(data_element, "is_rare", is_rare) || is_rare < 0 || is_rare > 1)
		{
			return -8;
		}
		outside_cfg.is_rare = (0 != is_rare);

		int is_broadcast = 0;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast) || is_broadcast < 0 || is_broadcast > 1)
		{
			return -9;
		}
		outside_cfg.is_broadcast = (0 != is_broadcast);

		if (!PugiGetSubNodeValue(data_element, "opengame_day", outside_cfg.opengame_day) || outside_cfg.opengame_day <= 0)
		{
			return -10;
		}

		if (last_opengame_day != outside_cfg.opengame_day)
		{
			last_opengame_day = outside_cfg.opengame_day;
			++grid_count;
		}

		if (grid_count < 0 || grid_count >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -1001;
		}

		m_promoting_position_outside_baodi_cfg_list[grid_count].opengame_day = outside_cfg.opengame_day;

		int baodi = 0;
		if (PugiGetSubNodeValue(data_element, "baodi", baodi) || baodi > 0)
		{
			if (m_promoting_position_outside_baodi_cfg_list[grid_count].baodi_map.find(baodi) != m_promoting_position_outside_baodi_cfg_list[grid_count].baodi_map.end()
				|| m_promoting_position_outside_baodi_cfg_list[grid_count].baodi_map.size() > 0)
			{
				return -1002;
			}

			m_promoting_position_outside_baodi_cfg_list[grid_count].baodi_map.insert(std::make_pair(baodi, outside_cfg.seq));
		}

		data_element = data_element.next_sibling();
	}

	++m_promoting_position_outside_max_seq;
	if (m_promoting_position_outside_max_seq >= RA_PROMOTING_POSITION_OUTSIDE_REWARD_MAX_SEQ)
	{
		return -1000;
	}

	return 0;
}

int RandActivityConfig::InitPromotingPositionInsideCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_promoting_position_inside_max_seq = -1;

	int last_opengame_day = 0;

	//static const int GRID_MAX_COUNT = 23;
	int grid_count = 0;

	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_promoting_position_inside_max_seq + 1)
		{
			return -1;
		}

		if (seq > 255)
		{
			return -10000;
		}

		RandActivityPromotingPositionConfig &inside_cfg = m_promoting_position_inside_cfg_list[seq];
		inside_cfg.seq = seq;
		m_promoting_position_inside_max_seq = seq;

		if (!PugiGetSubNodeValue(data_element, "reward_type", inside_cfg.reward_type) || (inside_cfg.reward_type != RA_PROMOTING_POSITION_REWARD_TYPE_GOLD && inside_cfg.reward_type != RA_PROMOTING_POSITION_REWARD_TYPE_ITEM))
		{
			return -2;
		}

		switch (inside_cfg.reward_type)
		{
		case RA_PROMOTING_POSITION_REWARD_TYPE_GOLD:
			{
				if (!PugiGetSubNodeValue(data_element, "reward_gold_rate", inside_cfg.reward_gold_rate) || inside_cfg.reward_gold_rate < 0)
				{
					return -3;
				}
			}
			break;

		case RA_PROMOTING_POSITION_REWARD_TYPE_ITEM:
			{
				PugiXmlNode item_element = data_element.child("reward_item");
				if (item_element.empty())
				{
					return -100;
				}

				if (!inside_cfg.reward_item.ReadConfig(item_element))
				{
					return -101;
				}
			}
			break;
		}

		if (!PugiGetSubNodeValue(data_element, "free_play_weight", inside_cfg.free_play_weight) || inside_cfg.free_play_weight < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "gold_play_weight", inside_cfg.gold_play_weight) || inside_cfg.gold_play_weight < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "extra_time_play_weight", inside_cfg.extra_time_play_weight) || inside_cfg.extra_time_play_weight < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "item_play_weight", inside_cfg.item_play_weight) || inside_cfg.item_play_weight < 0)
		{
			return -7;
		}

		int is_rare = 0;
		if (!PugiGetSubNodeValue(data_element, "is_rare", is_rare) || is_rare < 0 || is_rare > 1)
		{
			return -8;
		}
		inside_cfg.is_rare = (0 != is_rare);

		int is_broadcast = 0;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast) || is_broadcast < 0 || is_broadcast > 1)
		{
			return -9;
		}
		inside_cfg.is_broadcast = (0 != is_broadcast);

		if (!PugiGetSubNodeValue(data_element, "opengame_day", inside_cfg.opengame_day) || inside_cfg.opengame_day <= 0)
		{
			return -10;
		}

		if (last_opengame_day != inside_cfg.opengame_day)
		{
			last_opengame_day = inside_cfg.opengame_day;
			++grid_count;
		}

		if (grid_count < 0 || grid_count >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -1001;
		}

		m_promoting_position_inside_baodi_cfg_list[grid_count].opengame_day = inside_cfg.opengame_day;

		int baodi = 0;
		if (PugiGetSubNodeValue(data_element, "baodi", baodi) || baodi > 0)
		{
			if (m_promoting_position_inside_baodi_cfg_list[grid_count].baodi_map.find(baodi) != m_promoting_position_inside_baodi_cfg_list[grid_count].baodi_map.end()
				|| m_promoting_position_inside_baodi_cfg_list[grid_count].baodi_map.size() > 0)
			{
				return -1002;
			}

			m_promoting_position_inside_baodi_cfg_list[grid_count].baodi_map.insert(std::make_pair(baodi, inside_cfg.seq));
		}

		data_element = data_element.next_sibling();
	}

	++m_promoting_position_inside_max_seq;
	if (m_promoting_position_inside_max_seq >= RA_PROMOTING_POSITION_INSIDE_REWARD_MAX_SEQ)
	{
		return -1000;
	}

	return 0;
}

int RandActivityConfig::InitPromotingReturnRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_promoting_position_reward_cfg_count = -1;
	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_promoting_position_reward_cfg_count + 1)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "play_times", m_promoting_position_reward_cfg_list[seq].play_times)
			|| m_promoting_position_reward_cfg_list[seq].play_times < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "vip_limit", m_promoting_position_reward_cfg_list[seq].vip_limit)
			|| m_promoting_position_reward_cfg_list[seq].vip_limit < 0)
		{
			return -3;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -4;
		}

		if (!m_promoting_position_reward_cfg_list[seq].reward_item.ReadConfig(item_element))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "opengame_day", m_promoting_position_reward_cfg_list[seq].opengame_day)
			|| m_promoting_position_reward_cfg_list[seq].opengame_day < 0)
		{
			return -6;
		}

		++m_promoting_position_reward_cfg_count;
		data_element = data_element.next_sibling();
	}

	++m_promoting_position_reward_cfg_count;
	if (m_promoting_position_reward_cfg_count >= RA_PROMOTION_POSITION_REWARD_CFG_MAX_COUNT)
	{
		return -1000;
	}

	return 0;
}

const RandActivityPromotingPositionConfig * RandActivityConfig::GetPromotingPositionOutsideCfg(int seq)
{
	if (seq < 0 || seq >= m_promoting_position_outside_max_seq || seq >= RA_PROMOTING_POSITION_OUTSIDE_REWARD_MAX_SEQ)
	{
		return NULL;
	}
	else
	{
		return &m_promoting_position_outside_cfg_list[seq];
	}
}

const RandActivityPromotingPositionConfig * RandActivityConfig::GetPromotingPositionInsideCfg(int seq)
{
	if (seq < 0 || seq >= m_promoting_position_inside_max_seq || seq >= RA_PROMOTING_POSITION_INSIDE_REWARD_MAX_SEQ)
	{
		return NULL;
	}
	else
	{
		return &m_promoting_position_inside_cfg_list[seq];
	}
}

const RandActivityPromotingPositionConfig * RandActivityConfig::GetRandomPromotingPositionReward(int begin_index, int end_index, int play_type, int circle_type, bool is_split_mode)
{
	const RandActivityPromotingPositionConfig *circle_cfg_list = NULL;
	int max_seq = 0;

	if (RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE == circle_type)
	{
		circle_cfg_list = m_promoting_position_outside_cfg_list;

		max_seq = m_promoting_position_outside_max_seq;
		if (max_seq > RA_PROMOTING_POSITION_OUTSIDE_REWARD_MAX_SEQ)
		{
			return NULL;
		}
	}
	else
	{
		circle_cfg_list = m_promoting_position_inside_cfg_list;

		max_seq = m_promoting_position_inside_max_seq;
		if (max_seq > RA_PROMOTING_POSITION_INSIDE_REWARD_MAX_SEQ)
		{
			return NULL;
		}
	}

	if (NULL == circle_cfg_list)
	{
		return NULL;
	}

	std::vector<int> reward_seq_list;

	// 总权重
	int total_weight = 0;
	for (int seq = begin_index; seq < end_index && seq <= max_seq; ++ seq)
	{
		int reward_type = circle_cfg_list[seq].reward_type;

		// 双开只会抽中元宝或物品
		if (is_split_mode && (reward_type != RA_PROMOTING_POSITION_REWARD_TYPE_GOLD && reward_type != RA_PROMOTING_POSITION_REWARD_TYPE_ITEM))
		{
			continue;
		}

		reward_seq_list.push_back(seq);

		int weight = 0;
		switch(play_type)
		{
		case RA_PROMOTING_POSITION_PLAY_TYPE_FREE:
			{
				weight = circle_cfg_list[seq].free_play_weight;
			}
			break;

		case RA_PROMOTING_POSITION_PLAY_TYPE_GOLD:
			{
				weight = circle_cfg_list[seq].gold_play_weight;
			}
			break;

		case RA_PROMOTING_POSITION_PLAY_TYPE_EXTRA_TIMES:
			{
				weight = circle_cfg_list[seq].extra_time_play_weight;
			}
			break;

		case RA_PROMOTING_POSITION_PLAY_TYPE_USE_ITEM:
			{
				weight = circle_cfg_list[seq].item_play_weight;
			}
			break;
		}

		total_weight += weight;
	}

	if (total_weight <= 0 || 0 == reward_seq_list.size())
	{
		return NULL;
	}

	// 计算
	int rand_val = RandomNum(total_weight);
	for (std::vector<int>::iterator it = reward_seq_list.begin(); it != reward_seq_list.end(); ++ it)
	{
		const int seq = *it;
		int weight = 0;
		switch(play_type)
		{
		case RA_PROMOTING_POSITION_PLAY_TYPE_FREE:
			{
				weight = circle_cfg_list[seq].free_play_weight;
			}
			break;

		case RA_PROMOTING_POSITION_PLAY_TYPE_GOLD:
			{
				weight = circle_cfg_list[seq].gold_play_weight;
			}
			break;

		case RA_PROMOTING_POSITION_PLAY_TYPE_EXTRA_TIMES:
			{
				weight = circle_cfg_list[seq].extra_time_play_weight;
			}
			break;

		case RA_PROMOTING_POSITION_PLAY_TYPE_USE_ITEM:
			{
				weight = circle_cfg_list[seq].item_play_weight;
			}
			break;
		}

		if (rand_val < weight)
		{
			return &circle_cfg_list[seq];
		}

		rand_val -= weight;
	}

	return NULL;
}

const RandActivityPromotingPositionRewardCfg * RandActivityConfig::GetPromotingPositionRewardCfg(int seq)
{
	if (seq < 0 || seq >= m_promoting_position_reward_cfg_count || seq >= RA_PROMOTION_POSITION_REWARD_CFG_MAX_COUNT)
	{
		return NULL;
	}

	return &m_promoting_position_reward_cfg_list[seq];
}

const RandActivityPromotingPositionConfig * RandActivityConfig::GetPromotingPositionBaodiReward(const int server_chou_times)
{
	if (server_chou_times <= 0) return nullptr;

	const int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_PROMOTING_POSITION);

	RandActivityPromotingPositionBaodiCfg *baodi_cfg = nullptr;
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day <= m_promoting_position_inside_baodi_cfg_list[i].opengame_day)
		{
			baodi_cfg = &m_promoting_position_inside_baodi_cfg_list[i];
			break;
		}
	}

	if (nullptr == baodi_cfg)
	{
		return nullptr;
	}

	for (std::map<int, int>::iterator it = baodi_cfg->baodi_map.begin(); it != baodi_cfg->baodi_map.end(); ++it)
	{
		const int baodi_times = it->first;
		const int reward_seq = it->second;

		if (baodi_times <= 0 || 0 != server_chou_times % baodi_times) continue;
		if (reward_seq < 0 || reward_seq >= RA_PROMOTING_POSITION_INSIDE_REWARD_MAX_SEQ) continue;

		if (m_promoting_position_inside_cfg_list[reward_seq].seq == reward_seq)
		{
			return &m_promoting_position_inside_cfg_list[reward_seq];
		}
	}

	return nullptr;
}

int RandActivityConfig::InitBlackMarketCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;	
	}

	int opengame_day = 0;
	int last_opengame_day = 0;
	int diff_num = -1;
	int last_day = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			last_opengame_day = opengame_day;
			last_day = 0;
			++ diff_num;
		}

		if (diff_num < 0 || diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		m_black_market_cfg_list[diff_num].opengame_day = opengame_day;

		int day = 0;
		if (!PugiGetSubNodeValue(data_element, "day", day) || (day != last_day && day != last_day + 1))
		{
			return -1;
		}

		if (day >= RA_BLACK_MARKET_MAX_DAY_COUNT)
		{
			return -1000;
		}

		RandActivityBlackMarketItemCfg &diff_day_cfg = m_black_market_cfg_list[diff_num].black_market_cfg_list[day];

		diff_day_cfg.day = day;
		last_day = day;

		if (diff_day_cfg.max_item_count >= RA_BLACK_MARKET_MAX_ITEM_COUNT)
		{
			return -2000;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != diff_day_cfg.max_item_count)
		{
			return -2;
		}

		RandActivityBlackMarketItemCfg::ItemConfig &item_cfg = diff_day_cfg.item_cfg_list[seq];
		item_cfg.seq = seq;

		{
			PugiXmlNode item_element = data_element.child("item");
			if (item_element.empty())
			{
				return -100;
			}
			if (!item_cfg.item.ReadConfig(item_element))
			{
				return -101;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "init_gold", item_cfg.init_gold) || item_cfg.init_gold < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "min_add_gold", item_cfg.min_add_gold) || item_cfg.min_add_gold <= 0)
		{
			return -4;
		}

		++ diff_day_cfg.max_item_count;
		m_black_market_cfg_list[diff_num].black_market_max_day_count = last_day + 1;

		data_element = data_element.next_sibling();
	}


	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		for (int day = 0; day < m_black_market_cfg_list[i].black_market_max_day_count; ++ day)
		{
			if (m_black_market_cfg_list[i].black_market_cfg_list[day].max_item_count != RA_BLACK_MARKET_MAX_ITEM_COUNT)
			{
				return -3000;
			}
		}
	}

	return 0;
}

const RandActivityBlackMarketItemCfg::ItemConfig * RandActivityConfig::GetBlackMarketItemCfg(int day, int seq)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_BLACK_MARKET);
	int diff_num = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_black_market_cfg_list[i].opengame_day)
		{
			diff_num = i;
			break;
		}
	}

	if (diff_num < 0 || diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}


	if (day < 0 || day >= m_black_market_cfg_list[diff_num].black_market_max_day_count || day >= RA_BLACK_MARKET_MAX_DAY_COUNT)
	{
		return NULL;
	}

	RandActivityBlackMarketItemCfg &cfg = m_black_market_cfg_list[diff_num].black_market_cfg_list[day];

	if (seq < 0 || seq >= cfg.max_item_count || seq >= RA_BLACK_MARKET_MAX_ITEM_COUNT)
	{
		return NULL;
	}

	return &cfg.item_cfg_list[seq];
}

int RandActivityConfig::InitTreasuresMallCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;	
	}

	int opengame_day = 0;
	int last_opengame_day = 0;
	int diff_num = -1;
	int index = 0;
	int last_index = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -1;
		}

		if (opengame_day != last_opengame_day)
		{
			last_opengame_day = opengame_day;
			last_index = -1;
			++ diff_num;
		}

		if (diff_num < 0 || diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -2;
		}

		RandActivityTreasuresMallBuyConfig &diff_cfg = m_treasures_mall_buy_cfg_list[diff_num];
		diff_cfg.opengame_day = opengame_day;

		if (!PugiGetSubNodeValue(data_element, "index", index) || index < 0 || index >= RA_TREASURES_MALL_MAX_BUY_ITEM_COUNT || index != last_index + 1)
		{
			return -3;
		}
		last_index = index;

		RandActivityTreasuresMallBuyConfig::BuyItemConfig &buy_item_cfg = diff_cfg.item_list[index];
		buy_item_cfg.index = index;

		{
			// 读取物品
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				return -100;
			}

			if (!buy_item_cfg.reward_item.ReadConfig(item_element))
			{
				return -200;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "cost_gold", buy_item_cfg.cost_gold) || buy_item_cfg.cost_gold <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "get_score", buy_item_cfg.get_score) || buy_item_cfg.get_score <= 0)
		{
			return -5;
		}

		++ diff_cfg.item_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitTreasuresMallExchangeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;	
	}

	int opengame_day = 0;
	int last_opengame_day = 0;
	int diff_num = -1;
	int index = 0;
	int last_index = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -1;
		}

		if (opengame_day != last_opengame_day)
		{
			last_opengame_day = opengame_day;
			last_index = -1;
			++ diff_num;
		}

		if (diff_num < 0 || diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -2;
		}

		RandActivityTreasuresMallExchangeConfig &diff_cfg = m_treasures_mall_exchange_cfg_list[diff_num];
		diff_cfg.opengame_day = opengame_day;

		if (!PugiGetSubNodeValue(data_element, "index", index) || index < 0 || index >= RA_TREASURES_MALL_MAX_EXCHANGE_ITEM_COUNT || index != last_index + 1)
		{
			return -3;
		}
		last_index = index;

		RandActivityTreasuresMallExchangeConfig::ExchangeItemConfig &exchange_item_cfg = diff_cfg.exchange_item_cfg_list[index];
		exchange_item_cfg.index = index;

		{
			// 读取物品
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				return -100;
			}

			if (!exchange_item_cfg.reward_item.ReadConfig(item_element))
			{
				return -200;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "need_score", exchange_item_cfg.need_score) || exchange_item_cfg.need_score <= 0)
		{
			return -4;
		}

		++ diff_cfg.exchange_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityTreasuresMallBuyConfig::BuyItemConfig * RandActivityConfig::GetTreasuresMallBuyRewardCfg(int buy_index)
{
	if (buy_index < 0 || buy_index >= RA_TREASURES_MALL_MAX_BUY_ITEM_COUNT)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_TREASURES_MALL);
	int opengame_day_index = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_treasures_mall_buy_cfg_list[i].opengame_day)
		{
			opengame_day_index = i;
			break;
		}
	}

	if (opengame_day_index < 0 || opengame_day_index >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	if (buy_index < m_treasures_mall_buy_cfg_list[opengame_day_index].item_cfg_count)
	{
		return &m_treasures_mall_buy_cfg_list[opengame_day_index].item_list[buy_index];
	}

	return NULL;
}

const RandActivityTreasuresMallExchangeConfig::ExchangeItemConfig * RandActivityConfig::GetTreasuresMallExchangeRewardCfg(int exchange_index)
{
	if (exchange_index < 0 || exchange_index >= RA_TREASURES_MALL_MAX_EXCHANGE_ITEM_COUNT)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_TREASURES_MALL);
	int opengame_day_index = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++ i)
	{
		if (opengame_day <= m_treasures_mall_exchange_cfg_list[i].opengame_day)
		{
			opengame_day_index = i;
			break;
		}
	}

	if (opengame_day_index < 0 || opengame_day_index >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	if (exchange_index < m_treasures_mall_exchange_cfg_list[opengame_day_index].exchange_cfg_count)
	{
		return &m_treasures_mall_exchange_cfg_list[opengame_day_index].exchange_item_cfg_list[exchange_index];
	}

	return NULL;
}

int RandActivityConfig::InitOpenServerRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int activity_type = 0;
		if (!PugiGetSubNodeValue(data_element, "activity_type", activity_type) 
			|| activity_type < RAND_ACTIVITY_TYPE_BEGIN || activity_type >= RAND_ACTIVITY_TYPE_END)
		{
			return -1;
		}

		RandActivityOpenServerRewardConfig &cfg = m_openserver_reward_map[activity_type];
		cfg.rand_activity_type = activity_type;

		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -2;
		}

		RandActivityOpenServerRewardConfig::RewardDayConfig *day_cfg = NULL;
		for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; i++)
		{
			if (0 == cfg.reward_day_list[i].opengame_day || cfg.reward_day_list[i].opengame_day == opengame_day)
			{
				day_cfg = &cfg.reward_day_list[i];
				break;
			}
		}

		if (NULL == day_cfg)
		{
			return -3;
		}

		day_cfg->opengame_day = opengame_day;

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq <= 0 || seq >= RAND_ACTIVITY_OPEN_SERVER_REWARD_COUNT_MAX)
		{
			return -4;
		}

		RandActivityOpenServerRewardItemConfig &reward_cfg = day_cfg->item_list[seq];
		if (reward_cfg.seq > 0)
		{
			return -5;
		}

		reward_cfg.seq = seq;
		
		if (!PugiGetSubNodeValue(data_element, "cond1", reward_cfg.cond1) || reward_cfg.cond1 < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "cond2", reward_cfg.cond2) || reward_cfg.cond2 < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "cond3", reward_cfg.cond3) || reward_cfg.cond3 < 0)
		{
			return -8;
		}

		{
			PugiXmlNode list_element = data_element.child("reward_item_list");
			if (list_element.empty())
			{
				return -10;
			}

			PugiXmlNode item_element = list_element.child("reward_item");
			if (item_element.empty())
			{
				return -11;
			}

			reward_cfg.reward_item_count = 0;
			while (!item_element.empty())
			{
				if (reward_cfg.reward_item_count >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -12;
				}

				if (!reward_cfg.reward_item_list[reward_cfg.reward_item_count].ReadConfig(item_element))
				{
					return -13;
				}

				++ reward_cfg.reward_item_count;
				item_element = item_element.next_sibling();
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityOpenServerRewardItemConfig * RandActivityConfig::GetRandActivityOpenServerRewardConfig(int activity_type, int seq, bool is_reward_yesterday)
{
	if (seq <= 0 || seq >= RAND_ACTIVITY_OPEN_SERVER_REWARD_COUNT_MAX)
	{
		return NULL;
	}

	std::map<int, RandActivityOpenServerRewardConfig>::iterator it = m_openserver_reward_map.find(activity_type);
	if (it == m_openserver_reward_map.end())
	{
		return NULL;
	}

	int day_index = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;
	if (is_reward_yesterday)
	{
		day_index--;
	}

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; i++)
	{
		if (day_index <= it->second.reward_day_list[i].opengame_day)
		{
			if (it->second.reward_day_list[i].item_list[seq].seq <= 0)
			{
				return NULL;
			}

			return &it->second.reward_day_list[i].item_list[seq];
		}
	}

	return NULL;
}

const int RandActivityConfig::GetRandActivityOpenServerMaxCon3(int activity_type, bool is_reward_yesterday)
{
	int max_con3 = -1;
	std::map<int, RandActivityOpenServerRewardConfig>::iterator it = m_openserver_reward_map.find(activity_type);
	if (it == m_openserver_reward_map.end())
	{
		return max_con3;
	}

	int day_index = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;
	if (is_reward_yesterday)
	{
		day_index--;
	}

	int tage = -1;
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; i++)
	{
		if (day_index <= it->second.reward_day_list[i].opengame_day)
		{
			tage = i;
			break;
		}
	}

	if (tage < 0 || tage >= RAND_ACTIVITY_OPEN_SERVER_REWARD_COUNT_MAX)
	{
		return max_con3;
	}

	for (int j = 0; j < RAND_ACTIVITY_OPEN_SERVER_REWARD_COUNT_MAX; ++j)
	{
		if (it->second.reward_day_list[tage].item_list[j].cond3 > max_con3)
		{
			max_con3 = it->second.reward_day_list[tage].item_list[j].cond3;
		}
	}

	return max_con3;
}

const RandActivityOpenServerRewardConfig::RewardDayConfig * RandActivityConfig::GetOpenServerRewardConfigByTypeAndOGDay(int activity_type)
{
	std::map<int, RandActivityOpenServerRewardConfig>::iterator it = m_openserver_reward_map.find(activity_type);
	if (it == m_openserver_reward_map.end())
	{
		return NULL;
	}

	int day_index = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; i++)
	{
		if (day_index <= it->second.reward_day_list[i].opengame_day)
		{
			return &it->second.reward_day_list[i];
		}
	}

	return NULL;
}

int RandActivityConfig::InitNewCornucopiaCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0 || opengame_day >= RAND_ACTIVITY_NEW_CORNUCOPIA_CFG_COUNT_MAX)
		{
			return -1;
		}

		RandActivityNewCornucopiaConfig &cfg = m_new_cornucopia_cfg_list[opengame_day];

		int task_type = 0;
		if (!PugiGetSubNodeValue(data_element, "task_type", task_type) || task_type < 0 || task_type >= RA_NEW_CORNUCOPIA_TASK_TYPE_MAX)
		{
			return -2;
		}

		RandActivityNewCornucopiaConfig::ConfigItem &cfg_item = cfg.item_list[task_type];

		if (!PugiGetSubNodeValue(data_element, "task_value", cfg_item.task_value) || cfg_item.task_value < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "add_percent", cfg_item.add_percent) || cfg_item.add_percent <= 0)
		{
			return -4;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityNewCornucopiaConfig * RandActivityConfig::GetRandActivityNewCornucopiaConfig(int opengame_day)
{
	if (opengame_day < 0 || opengame_day >= RAND_ACTIVITY_NEW_CORNUCOPIA_CFG_COUNT_MAX)
	{
		return NULL;
	}

	return &m_new_cornucopia_cfg_list[opengame_day];
}

int RandActivityConfig::InitNewCornucopiaTotalCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= RAND_ACTIVITY_NEW_CORNUCOPIA_TOTAL_CFG_COUNT_MAX)
		{
			return -1;
		}

		RandActivityNewCornucopiaTotalConfig &cfg = m_new_cornucopia_total_cfg_list[seq];
		if (0 != cfg.seq)
		{
			return -2;
		}

		cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "cornucopia_value", cfg.cornucopia_value) || cfg.cornucopia_value <= 0)
		{
			return -3;
		}

		{
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty() || !cfg.reward_item.ReadConfig(item_element))
			{
				return -4;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityNewCornucopiaTotalConfig * RandActivityConfig::GetRandActivityNewCornucopiaTotalConfig(int seq)
{
	if (seq < 0 || seq >= RAND_ACTIVITY_NEW_CORNUCOPIA_TOTAL_CFG_COUNT_MAX)
	{
		return NULL;
	}

	return &m_new_cornucopia_total_cfg_list[seq];
}

int RandActivityConfig::InitItemCollectionRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_item_collection_reward_cfg.cfg_count = 0;
	while (!data_element.empty())
	{
		if (m_item_collection_reward_cfg.cfg_count >= RAND_ACTIVITY_ITEM_COLLECTION_REWARD_MAX_COUNT)
		{
			return -1000;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_item_collection_reward_cfg.cfg_count)
		{
			return -10;
		}

		RandActivityItemCollectionRewardCfg::ConfigItem &reward_cfg = m_item_collection_reward_cfg.cfg_list[seq];
		reward_cfg.seq = seq;

		char temp_str[32] = {0};
		for (int i = 0; i < RandActivityItemCollectionRewardCfg::MAX_COLLECTION_STUFF_COUNT && reward_cfg.stuff_count < RandActivityItemCollectionRewardCfg::MAX_COLLECTION_STUFF_COUNT; i++)
		{
			sprintf(temp_str, "stuff_id%d", i + 1);
			PugiXmlNode item_element = data_element.child(temp_str);
			if (item_element.empty())
			{
				return -100;
			}

			if (!reward_cfg.stuff_id_list[reward_cfg.stuff_count].ReadConfig(item_element) && reward_cfg.stuff_id_list[reward_cfg.stuff_count].item_id != 0)
			{
				return -101;
			}

			reward_cfg.stuff_count++;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -200;
		}

		if (!reward_cfg.reward_item.ReadConfig(item_element))
		{
			return -201;
		}

		if (!PugiGetSubNodeValue(data_element, "exchange_times_limit", reward_cfg.exchange_times_limit) || reward_cfg.exchange_times_limit < 0)
		{
			return -20;
		}

		++m_item_collection_reward_cfg.cfg_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityItemCollectionRewardCfg::ConfigItem * RandActivityConfig::GetItemCollectionRewardCfg(int seq)
{
	if (seq < 0 || seq >= m_item_collection_reward_cfg.cfg_count || seq >= RAND_ACTIVITY_ITEM_COLLECTION_REWARD_MAX_COUNT)
	{
		return NULL;
	}

	return &m_item_collection_reward_cfg.cfg_list[seq];
}

int RandActivityConfig::InitExpRefineRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_seq = -1;
	int last_opengame_day = -1;
	while (!data_element.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "activity_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}
		if (opengame_day != last_opengame_day)
		{
			last_opengame_day = opengame_day;
			last_seq = -1;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= RAND_ACTIVITY_EXP_REFINE_REWARD_MAX_COUNT)
		{
			return -1;
		}

		if (seq != last_seq + 1)
		{
			return -2;
		}

		RandActivityExpRefineRewardCfg &cfg = m_exp_refine_cfg_map[opengame_day].m_exp_refine_cfg_list[seq];
		cfg.seq = seq;
		last_seq = seq;

		if (!PugiGetSubNodeValue(data_element, "consume_gold", cfg.consume_gold) || cfg.consume_gold <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_exp", cfg.reward_exp) || cfg.reward_exp <= 0)
		{
			return -4;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityExpRefineRewardCfg * RandActivityConfig::GetRandActivityExpRefineRewardCfg(int seq)
{
	if (seq < 0 || seq >= RAND_ACTIVITY_EXP_REFINE_REWARD_MAX_COUNT)
	{
		return NULL;
	}

	time_t activity_begin_time = ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_EXP_REFINE);
	time_t now_time = EngineAdapter::Instance().Time();
	int act_real_open_day = GetDayIndex(activity_begin_time, now_time) + 1;

	for (auto & iter : m_exp_refine_cfg_map)
	{
		if (act_real_open_day == iter.first)
		{
			if (iter.second.m_exp_refine_cfg_list[seq].seq == seq)
			{
				return &iter.second.m_exp_refine_cfg_list[seq];
			}
		}
	}

	//return &m_exp_refine_cfg_list[seq];
	return NULL;
}

ItemConfigData * RandActivityConfig::GetRAGoldenPigRewardCfg(int summon_type, int reward_type, int &boss_id)
{
	if (summon_type < RandActivityGoldenPigRewardCfg::GOLDEN_PIG_REWARD_TYPE_SUMMON ||
		summon_type >= RandActivityGoldenPigRewardCfg::GOLDEN_PIG_REWARD_TYPE_MAX_COUNT)
	{
		return NULL;
	}

	if (reward_type < 0 || reward_type >= RandActivityGoldenPigRewardCfg::GOLDEN_PIG_REWARD_TYPE_MAX_COUNT)
	{
		return NULL;
	}

	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;

	for (std::vector<RandActivityGoldenPigRewardCfg>::iterator iter = m_golden_pig_reward_vec_list[summon_type].begin();
			iter != m_golden_pig_reward_vec_list[summon_type].end(); ++iter)
	{
		if (opengame_day < iter->opengame_day)
		{
			boss_id = iter->monster_id;
			return &iter->reward_list[reward_type];
		}
	}

	return NULL;
}

int RandActivityConfig::IsGoldenPigBoss(int boss_id)
{
	for (int i = 0; i < RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_MAX_COUNT; ++i)
	{
		for (std::vector<RandActivityGoldenPigRewardCfg>::iterator iter = m_golden_pig_reward_vec_list[i].begin();
		iter != m_golden_pig_reward_vec_list[i].end(); iter++)
		{
			if (iter->monster_id == boss_id)
			{
				return i;
			}
		}
	}

	return -1;
}

int RandActivityConfig::GetGoldenPigJoinerRewardFlag(int boss_id)
{
	for (int i = 0; i < RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_MAX_COUNT; ++i)
	{
		for (std::vector<RandActivityGoldenPigRewardCfg>::iterator iter = m_golden_pig_reward_vec_list[i].begin();
		iter != m_golden_pig_reward_vec_list[i].end(); iter++)
		{
			if (iter->monster_id == boss_id)
			{
				return iter->joiner_reward_flag;
			}
		}
	}

	return -1;
}

void RandActivityConfig::SetGoldenPigJoinerRewardFlag(int flag, int boss_id)
{
	for (int i = 0; i < RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_MAX_COUNT; ++i)
	{
		for (std::vector<RandActivityGoldenPigRewardCfg>::iterator iter = m_golden_pig_reward_vec_list[i].begin();
		iter != m_golden_pig_reward_vec_list[i].end(); iter++)
		{
			if (iter->monster_id == boss_id)
			{
				iter->joiner_reward_flag = flag;
			}
		}
	}
}

const RandActivityOpenServerInvestOtherCfg * RandActivityConfig::GetRAServerInvestOtherCfg(int invest_type)
{
	if (invest_type >= RandActivityOpenServerInvestOtherCfg::INVEST_TYPE_BOSS && invest_type < RandActivityOpenServerInvestOtherCfg::MAX_INVEST_TYPE)
	{
		return &m_openserver_invest_other_cfg[invest_type];
	}

	return NULL;
}

const RandActivityOpenServerInvestRewardCfg * RandActivityConfig::GetRAServerInvestRewardCfg(int invest_type, int seq)
{
	if (invest_type >= RandActivityOpenServerInvestOtherCfg::INVEST_TYPE_BOSS && invest_type < RandActivityOpenServerInvestOtherCfg::MAX_INVEST_TYPE)
	{
		if (seq >= 0 || seq < RAND_ACTIVITY_OPEN_SERVER_INVEST_MAX_COUNT)
		{
			return &m_openserver_invest_reward_cfg[invest_type][seq];
		}
	}

	return NULL;
}

const RandActivitySingleChongZhiRewardCfg::ItemConfig *RandActivityConfig::GetSingleChongZhiCfgByGoldNum(int chongzhi)
{
	//for (int i = RAND_ACTIVITY_SINGLE_CHONGZHI_MAX_CFG - 1; i >= 0; --i)
	//{
	//	if (chongzhi == m_single_chongzhi_cfg.item_cfg_list[i].need_gold)
	//	{
	//		return &m_single_chongzhi_cfg.item_cfg_list[i];
	//	}
	//}

	for (int i = 0; i < RAND_ACTIVITY_SINGLE_CHONGZHI_MAX_CFG; i++)
	{
		if (i <= RAND_ACTIVITY_SINGLE_CHONGZHI_MAX_CFG - 2)
		{
			if (chongzhi >= m_single_chongzhi_cfg.item_cfg_list[i].need_gold && chongzhi < m_single_chongzhi_cfg.item_cfg_list[i + 1].need_gold)
			{
				return &m_single_chongzhi_cfg.item_cfg_list[i];
			}
		}

		if (i == RAND_ACTIVITY_SINGLE_CHONGZHI_MAX_CFG - 1 && chongzhi >= m_single_chongzhi_cfg.item_cfg_list[i].need_gold)
		{
			return &m_single_chongzhi_cfg.item_cfg_list[i];
		}
	}

	return NULL;
}

const RandActivitySingleChongZhiRewardCfg::ItemConfig *RandActivityConfig::GetSingleChongZhiCfgBySeq(int seq)
{
	if (seq < 0 || seq >= RAND_ACTIVITY_SINGLE_CHONGZHI_MAX_CFG)
	{
		return NULL;
	}

	return &m_single_chongzhi_cfg.item_cfg_list[seq];
}

const RandActivityXianyuanTreasConfig * RandActivityConfig::GetXianyuanTreasCfg(int seq)
{
	if (seq < 0 || seq >= m_xianyuan_treas_cfg_count || seq >= RAND_ACTIVITY_XIANYUAN_TREAS_CFG_COUNT)
	{
		return NULL;
	}

	return &m_xianyuan_treas_cfg_list[seq];

}

const ItemConfigData* RandActivityConfig::GetXianyuanTreasAllBuyReward()
{
	if (m_xianyuan_treas_all_buy_reward.empty())
	{
		return nullptr;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;	

	auto it = std::find_if(m_xianyuan_treas_all_buy_reward.begin(), m_xianyuan_treas_all_buy_reward.end(), [opengame_day](decltype(m_xianyuan_treas_all_buy_reward)::value_type& value) {
		if (opengame_day <= value.first)
		{
			return true;
		}
		return false;
	});

	if (it != m_xianyuan_treas_all_buy_reward.end())
	{
		return &it->second;
	}

	return &(m_xianyuan_treas_all_buy_reward.rbegin()->second);
}

int RandActivityConfig::GetActivityLastedDays(int activity_type)
{
	time_t activity_begin_time = ActivityShadow::Instance().GetRandActivityBeginTime(activity_type);
	time_t now_time = EngineAdapter::Instance().Time();
	int act_real_open_day = GetDayIndex(activity_begin_time, now_time) + 1; // 活动开启时的天数，1~N

	if (act_real_open_day <= 0)
	{
		act_real_open_day = 1;
	}

	return act_real_open_day;
}

int RandActivityConfig::GetRushBuyingPhase(time_t timestamp)
{
	struct tm *local_time = ::localtime(&timestamp);
	int tm_hour = local_time->tm_hour;

	if (CrossConfig::Instance().IsHiddenServer())
	{
		return -1;
	}
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		RandActivityRushBuyingPhaseDayConfig &day_cfg = m_rush_buying_phase_cfg_list[i];
		if (opengame_day > day_cfg.opengame_day)
		{
			continue;
		}

		for (int k = 0; k < day_cfg.cfg_count && k < RA_RUSH_BUYING_PHASES_COUNT; ++k)
		{
			int begin_time_h = day_cfg.cfg_list[k].buying_begin_time;
			int next_begin_time_h = 0;

			if (k < day_cfg.cfg_count - 1)
			{
				next_begin_time_h = day_cfg.cfg_list[k + 1].buying_begin_time;
			}
			else
			{
				next_begin_time_h = 24;
			}

			if (tm_hour >= begin_time_h && tm_hour < next_begin_time_h)
			{
				return day_cfg.cfg_list[k].seq;
			}
		}

		break;
	}

	return -1;
}

int RandActivityConfig::GetRushBuyingSaleItemsCount(int phase_seq)
{
	if (phase_seq < 0 || phase_seq >= RA_RUSH_BUYING_PHASES_COUNT)
	{
		return 0;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return 0;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		RandActivityRushBuyingPhaseDayConfig &day_cfg = m_rush_buying_phase_cfg_list[i];
		if (opengame_day > day_cfg.opengame_day)
		{
			continue;
		}

		if (phase_seq >= day_cfg.cfg_count)
		{
			return 0;
		}

		return day_cfg.cfg_list[phase_seq].sale_items_count;
	}

	return 0;
}

unsigned int RandActivityConfig::GetRushBuyingPhaseBeginTimestamp(int phase_seq)
{
	if (phase_seq < 0 || phase_seq >= RA_RUSH_BUYING_PHASES_COUNT)
	{
		return 0;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return 0;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		RandActivityRushBuyingPhaseDayConfig &day_cfg = m_rush_buying_phase_cfg_list[i];
		if (opengame_day > day_cfg.opengame_day)
		{
			continue;
		}

		if (phase_seq >= day_cfg.cfg_count)
		{
			return 0;
		}

		int buying_begin_time_h = day_cfg.cfg_list[phase_seq].buying_begin_time;

		return (unsigned int)GetZeroTime(EngineAdapter::Instance().Time()) + buying_begin_time_h * SECOND_PER_HOUR;
	}

	return 0;
}

const RandActivityRushBuyingPhaseConfig::SaleItemConfig * RandActivityConfig::GetRushBuyingSaleItemCfg(int phase_seq, int item_index)
{
	if (phase_seq < 0 || phase_seq >= RA_RUSH_BUYING_PHASES_COUNT)
	{
		return NULL;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return NULL;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		RandActivityRushBuyingPhaseDayConfig &day_cfg = m_rush_buying_phase_cfg_list[i];
		if (opengame_day > day_cfg.opengame_day)
		{
			continue;
		}

		if (phase_seq >= day_cfg.cfg_count || item_index < 0 || item_index >= day_cfg.cfg_list[phase_seq].sale_items_count || item_index >= RA_RUSH_BUYING_SALES_ITEMS_COUNT)
		{
			return NULL;
		}

		return &day_cfg.cfg_list[phase_seq].sale_item_cfg_list[item_index];
	}

	return NULL;
}

const RandActivitySingleChargeCfg::ConfigItem * RandActivityConfig::GetSingleChargeCfg2(long long charge_value)
{
	if (charge_value <= 0)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_SINGLE_CHARGE_2);

	for (int i = 0; i < RAND_ACTIVITY_SINGLE_CHARGE2_CFG_COUNT; ++i)
	{
		if (opengame_day > m_single_charge_cfg2[i].opengame_day)
		{
			continue;
		}

		for (int index = m_single_charge_cfg2[i].cfg_count - 1; index >= 0; index--)
		{
			RandActivitySingleChargeCfg::ConfigItem *cfg_item = &m_single_charge_cfg2[i].cfg_list[index];
			if (charge_value >= cfg_item->charge_value)
			{
				return cfg_item;
			}
		}
	}

	return NULL;
}

const RandActivitySingleChargeCfg::ConfigItem * RandActivityConfig::GetSingleChargeCfg3(long long charge_value)
{
	if (charge_value <= 0)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_SINGLE_CHARGE_3);

	for (int i = 0; i < RAND_ACTIVITY_SINGLE_CHARGE3_CFG_COUNT; ++i)
	{
		if (opengame_day > m_single_charge_cfg3[i].opengame_day)
		{
			continue;
		}

		for (int index = m_single_charge_cfg3[i].cfg_count - 1; index >= 0; index--)
		{
			RandActivitySingleChargeCfg::ConfigItem *cfg_item = &m_single_charge_cfg3[i].cfg_list[index];
			if (charge_value >= cfg_item->charge_value)
			{
				return cfg_item;
			}
		}
	}

	return NULL;
}

const RandActivitySingleChargeCfg::ConfigItem * RandActivityConfig::GetSingleChargeCfg4(long long charge_value)
{
	if (charge_value <= 0)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_SINGLE_CHARGE_4);

	for (int i = 0; i < RAND_ACTIVITY_SINGLE_CHARGE4_CFG_COUNT; ++i)
	{
		if (opengame_day > m_single_charge_cfg4[i].opengame_day)
		{
			continue;
		}

		for (int index = m_single_charge_cfg4[i].cfg_count - 1; index >= 0; index--)
		{
			RandActivitySingleChargeCfg::ConfigItem *cfg_item = &m_single_charge_cfg4[i].cfg_list[index];
			if (charge_value >= cfg_item->charge_value)
			{
				return cfg_item;
			}
		}
	}

	return NULL;
}

const RandActivitySingleChargeCfg::ConfigItem * RandActivityConfig::GetSingleChargeCfg5(long long charge_value)
{
	if (charge_value <= 0)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_SINGLE_CHARGE_5);

	for (int i = 0; i < RAND_ACTIVITY_SINGLE_CHARGE5_CFG_COUNT; ++i)
	{
		if (opengame_day > m_single_charge_cfg5[i].opengame_day)
		{
			continue;
		}

		for (int index = m_single_charge_cfg5[i].cfg_count - 1; index >= 0; index--)
		{
			RandActivitySingleChargeCfg::ConfigItem *cfg_item = &m_single_charge_cfg5[i].cfg_list[index];
			if (charge_value >= cfg_item->charge_value)
			{
				return cfg_item;
			}
		}
	}

	return NULL;
}

bool RandActivityConfig::GetMapHuntRandCityCfg(int rand_count, RandActivityMapHuntCityConfig::CityCfg *rand_list[MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE],int server_chou_times)
{
	if (MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE != rand_count)
	{
		return false;
	}

	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MAP_HUNT);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (m_map_hunt_city_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return false;
	}

	RandActivityMapHuntCityConfig &cfg_list = m_map_hunt_city_cfg_list[idx];

	int city_flag = 0;
	int total_weight = m_map_hunt_total_city_weight_list[idx];
	if (total_weight <= 0) return false;

	int get_count = 0;

	// 保底必出
	if (server_chou_times > 0)
	{
		for (std::map<int, int>::iterator it = cfg_list.baodi_map.begin(); it != cfg_list.baodi_map.end(); ++it)
		{
			const int baodi_times = it->first;
			const int reward_seq = it->second;

			if (baodi_times > 0 && 0 == server_chou_times % baodi_times && reward_seq >= 0 && reward_seq < cfg_list.city_count)
			{
				rand_list[get_count] = &cfg_list.city_cfg_list[reward_seq];
				++get_count;
				break;
			}
		}
	}

	for (; get_count < MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE; ++get_count)
	{
		int rand_val = RandomNum(total_weight);
		for (int i = 0; i < cfg_list.city_count; ++i)
		{
			if (rand_val < cfg_list.city_cfg_list[i].weight)
			{
				if (0 != (city_flag & (1 << i)))
				{
					continue;
				}
				else
				{
					city_flag |= (1 << i);
					total_weight -= cfg_list.city_cfg_list[i].weight;
					rand_list[get_count] = &cfg_list.city_cfg_list[i];
					break;
				}
			}

			rand_val -= cfg_list.city_cfg_list[i].weight;
		}
	}

	return true;
}

const ItemConfigData *RandActivityConfig::GetMapHuntBaseReward(const int city_id)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MAP_HUNT);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (m_map_hunt_city_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityMapHuntCityConfig &cfg_list = m_map_hunt_city_cfg_list[idx];
	if (city_id <= 0 || city_id > cfg_list.city_count)
	{
		return NULL;
	}

	for (int i = 0; i < cfg_list.city_count; ++i)
	{
		if (cfg_list.city_cfg_list[i].city_id == city_id)
		{
			return &cfg_list.city_cfg_list[i].base_reward_item;
		}
	}

	return NULL;
}

const RandActivityMapHuntRouteConfig::RouteCfg * RandActivityConfig::GetMapHuntRouteCfg(const int route_index)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MAP_HUNT);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (m_map_hunt_route_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityMapHuntRouteConfig &cfg_list = m_map_hunt_route_cfg_list[idx];
	if (route_index <= 0 || route_index > cfg_list.route_count)
	{
		return NULL;
	}

	for (int i = 0; i < cfg_list.route_count; ++i)
	{
		if (cfg_list.route_cfg_list[i].route_index == route_index)
		{
			return &cfg_list.route_cfg_list[i];
		}
	}

	return NULL;
}

const int RandActivityConfig::GetMapHuntRouteIndex(const int first_city_id)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MAP_HUNT);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (m_map_hunt_route_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return -1;
	}

	RandActivityMapHuntRouteConfig &cfg_list = m_map_hunt_route_cfg_list[idx];
	for (int i = 0; i < cfg_list.route_count; ++i)
	{
		if (cfg_list.route_cfg_list[i].city_list[0] == first_city_id)
		{
			return cfg_list.route_cfg_list[i].route_index;
		}
	}

	return -1;
}

const RandActivityMapHuntServerRewardConfig::RewardCfg* RandActivityConfig::GetMapHuntServerRewardCfg(const int index)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MAP_HUNT);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (m_map_hunt_server_cfg_list[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityMapHuntServerRewardConfig &cfg_list = m_map_hunt_server_cfg_list[idx];
	if (index < 0 || index >= MAX_RA_MAP_HUNT_SERVER_REWARD_COUNT || index >= cfg_list.server_reward_cout)
	{
		return NULL;
	}

	if (cfg_list.server_reward_cfg_list[index].index == index)
	{
		return &cfg_list.server_reward_cfg_list[index];
	}

	return NULL;
}

const RandActivityLightTowerExploreCfg::LayerConfig * RandActivityConfig::GetLightTowerExploreReward(const int cur_layer, bool is_fixed)
{
	if (cur_layer < 0 || cur_layer >= RA_LIGHT_TOWER_EXPLORE_MAX_LAYER)
	{
		return NULL;
	}

	int index = -1;
	unsigned int begin_time = ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE);
	int opengame_day = this->GetIntervalOpenGameDay(begin_time);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_light_tower_explore_list[i].opengame_day)
		{
			continue;
		}
		index = i;
		break;
	}

	if (index < 0 || index >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	RandActivityLightTowerExploreCfg &cfg = m_light_tower_explore_list[index].layer_reward_list[cur_layer];
	if (cfg.layer != cur_layer)
	{
		return NULL;
	}

	if (is_fixed)
	{
		return &cfg.reward_list[0];
	}
	else
	{
		if (cfg.layer_total_weight <= 0)
		{
			return NULL;
		}

		int rand_val = RandomNum(cfg.layer_total_weight);
		for (int i = 0; i < RA_LIGHT_TOWER_EXPLORE_MAX_PER_LAYER_REWAED_COUNT; ++i)
		{
			if (rand_val < cfg.reward_list[i].weight)
			{
				return &cfg.reward_list[i];
			}

			rand_val -= cfg.reward_list[i].weight;
		}
	}

	return NULL;
}

const RandActivityLightTowerExploreCfg * RandActivityConfig::GetLightTowerExploreLayerCfg(const int layer)
{
	if (layer < 0 || layer >= RA_LIGHT_TOWER_EXPLORE_MAX_LAYER)
	{
		return NULL;
	}

	int index = -1;
	unsigned int begin_time = ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE);
	int opengame_day = this->GetIntervalOpenGameDay(begin_time);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_light_tower_explore_list[i].opengame_day)
		{
			continue;
		}
		index = i;
		break;
	}

	if (index < 0 || index >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	return &m_light_tower_explore_list[index].layer_reward_list[layer];
}

const RandActivityLightTowerExploreExternCfg * RandActivityConfig::GetLightTowerExploreExternReward(const int layer)
{
	if (layer < 0 || layer >= RA_LIGHT_TOWER_EXPLORE_MAX_LAYER)
	{
		return NULL;
	}

	int index = -1;
	unsigned int begin_time = ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE);
	int opengame_day = this->GetIntervalOpenGameDay(begin_time);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_light_tower_explore_extern_reward_list[i].opengame_day)
		{
			continue;
		}
		index = i;
		break;
	}

	if (index < 0 || index >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	if (m_light_tower_explore_extern_reward_list[index].layer_extern_reward_list[layer].layer == layer)
	{
		return &m_light_tower_explore_extern_reward_list[index].layer_extern_reward_list[layer];
	}

	return NULL;
}

const RandActivityLightTowerExploreReturnCfg * RandActivityConfig::GetLightTowerExploreReturnReward(const int index)
{
	if (index < 0 || index >= RA_LIGHT_TOWER_EXPLORE_MAX_SERVER_REWARD_COUNT)
	{
		return NULL;
	}

	int opengame_day_index = -1;
	unsigned int begin_time = ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE);
	int opengame_day = this->GetIntervalOpenGameDay(begin_time);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_light_tower_explore_server_reward_list[i].opengame_day)
		{
			continue;
		}
		opengame_day_index = i;
		break;
	}

	if (opengame_day_index < 0 || opengame_day_index >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	if (m_light_tower_explore_server_reward_list[opengame_day_index].server_reward_list[index].index == index)
	{
		return &m_light_tower_explore_server_reward_list[opengame_day_index].server_reward_list[index];
	}

	return NULL;
}

int RandActivityConfig::InitRedEnvelopeGift(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "percent", m_red_envelope_gift_cfg.percent) || m_red_envelope_gift_cfg.percent <= 0)
	{
		return -1;
	}
	
	return 0;
}

int RandActivityConfig::InitMarryMeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	RandActivityMarryMeCfg &cfg = m_marryme_cfg;

	if (!PugiGetSubNodeValue(data_element, "couple_count", cfg.reward_couple_count) || cfg.reward_couple_count <= 0 || cfg.reward_couple_count > RA_MARRY_REWARD_COUPLE_COUNT_MAX)
	{
		return -1;
	}

	{
		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -2;
		}

		if (!cfg.reward_item.ReadConfig(item_element))
		{
			return -3;
		}
	}	

	return 0;
}

int RandActivityConfig::InitGoldenPigOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "senior_summon_consume", m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_SENIOR].summon_consume) ||
		m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_SENIOR].summon_consume <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "medium_summon_consume", m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_MEDIUM].summon_consume) ||
		m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_MEDIUM].summon_consume <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "junior_summon_consume", m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_JUNIOR].summon_consume) ||
		m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_JUNIOR].summon_consume <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "senior_summon_pos_x", m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_SENIOR].summon_pos.x) ||
		m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_SENIOR].summon_pos.x < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "medium_summon_pos_x", m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_MEDIUM].summon_pos.x) ||
		m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_MEDIUM].summon_pos.x < 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "junior_summon_pos_x", m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_JUNIOR].summon_pos.x) ||
		m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_JUNIOR].summon_pos.x < 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "senior_summon_pos_y", m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_SENIOR].summon_pos.y) ||
		m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_SENIOR].summon_pos.y < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "medium_summon_pos_y", m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_MEDIUM].summon_pos.y) ||
		m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_MEDIUM].summon_pos.y < 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "junior_summon_pos_y", m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_JUNIOR].summon_pos.y) ||
		m_golden_pig_other_cfg.summon_cfg_list[RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_JUNIOR].summon_pos.y < 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(data_element, "gold_consume", m_golden_pig_other_cfg.gold_consume) || m_golden_pig_other_cfg.gold_consume <= 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(data_element, "scene_id", m_golden_pig_other_cfg.scene_id) || m_golden_pig_other_cfg.scene_id <= 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "per_hp_reward", m_golden_pig_other_cfg.per_hp_reward) || m_golden_pig_other_cfg.per_hp_reward < 100 ||
		m_golden_pig_other_cfg.per_hp_reward > 10000)
	{
		return -12;
	}

	return 0;
}

int RandActivityConfig::InitGoldenPigRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		RandActivityGoldenPigRewardCfg reward_cfg;

		if (!PugiGetSubNodeValue(data_element, "opengame_day", reward_cfg.opengame_day) || reward_cfg.opengame_day <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_id", reward_cfg.monster_id) || reward_cfg.monster_id <= 0 || !MONSTERPOOL->IsMonsterExist(reward_cfg.monster_id))
		{
			return -2;
		}

		int summon_type = -1;
		if (!PugiGetSubNodeValue(data_element, "summon_type", summon_type) || summon_type < RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_JUNIOR ||
			summon_type >= RandActivityGoldenPigOtherCfg::GOLDEN_PIG_SUMMON_TYPE_MAX_COUNT)
		{
			return -3;
		}

		PugiXmlNode item_element = data_element.child("summon_reward");
		if (item_element.empty())
		{
			return -4;
		}

		if (!reward_cfg.reward_list[RandActivityGoldenPigRewardCfg::GOLDEN_PIG_REWARD_TYPE_SUMMON].ReadConfig(item_element))
		{
			return -5;
		}

		item_element = data_element.child("joiner_reward");
		if (item_element.empty())
		{
			return -6;
		}

		if (!reward_cfg.reward_list[RandActivityGoldenPigRewardCfg::GOLDEN_PIG_REWARD_TYPE_JOINER].ReadConfig(item_element))
		{
			return -7;
		}

		item_element = data_element.child("killer_reward");
		if (item_element.empty())
		{
			return -8;
		}

		if (!reward_cfg.reward_list[RandActivityGoldenPigRewardCfg::GOLDEN_PIG_REWARD_TYPE_KILLER].ReadConfig(item_element))
		{
			return -9;
		}

		m_golden_pig_reward_vec_list[summon_type].push_back(reward_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitOpenServerInvestOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int invest_type = 0;
		if (!PugiGetSubNodeValue(data_element, "invest_type", invest_type) || invest_type < RandActivityOpenServerInvestOtherCfg::INVEST_TYPE_BOSS || invest_type >= RandActivityOpenServerInvestOtherCfg::MAX_INVEST_TYPE)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "consume", m_openserver_invest_other_cfg[invest_type].consume) ||
			m_openserver_invest_other_cfg[invest_type].consume < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "invest_limit_time", m_openserver_invest_other_cfg[invest_type].invest_limit_time) ||
			m_openserver_invest_other_cfg[invest_type].invest_limit_time < 0)
		{
			return -3;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -4;
		}

		if (!m_openserver_invest_other_cfg[invest_type].reward_item.ReadConfig(item_element))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "active_reward_limit", m_openserver_invest_other_cfg[invest_type].active_reward_limit) ||
			m_openserver_invest_other_cfg[invest_type].active_reward_limit < 0)
		{
			return -6;
		}

		if (0 != m_openserver_invest_other_cfg[invest_type].active_reward_limit && invest_type != RandActivityOpenServerInvestOtherCfg::INVEST_TYPE_ACTIVE)
		{
			return -7;
		}
		
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitOpenServerInvestRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int invest_type = 0;
		if (!PugiGetSubNodeValue(data_element, "invest_type", invest_type) || invest_type < RandActivityOpenServerInvestOtherCfg::INVEST_TYPE_BOSS || invest_type >= RandActivityOpenServerInvestOtherCfg::MAX_INVEST_TYPE)
		{
			return -1;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= RAND_ACTIVITY_OPEN_SERVER_INVEST_MAX_COUNT)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "param", m_openserver_invest_reward_cfg[invest_type][seq].param) ||
			m_openserver_invest_reward_cfg[invest_type][seq].param < 0)
		{
			return -3;
		}

		int item_count = ItemConfigData::ReadConfigList(data_element, std::string("reward_item"), m_openserver_invest_reward_cfg[invest_type][seq].reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
		if (item_count < 0)
		{
			return -10 + item_count;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitSingleChongZhiRewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -20000;
	}


	int seq = 0;
	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "seq", seq) || seq < 0 || seq >= RAND_ACTIVITY_SINGLE_CHONGZHI_MAX_CFG)
		{
			return -1;
		}
		RandActivitySingleChongZhiRewardCfg::ItemConfig &reward_cfg = m_single_chongzhi_cfg.item_cfg_list[seq];
		reward_cfg.seq = seq;
		if (!PugiGetSubNodeValue(dataElement, "need_gold", reward_cfg.need_gold) || reward_cfg.need_gold <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_gold", reward_cfg.reward_gold) || reward_cfg.reward_gold <= 0)
		{
			return -3;
		}

		PugiXmlNode item_element = dataElement.child("reward_item");
		if (item_element.empty())
		{
			return -4;
		}

		if (!reward_cfg.reward_item.ReadConfig(item_element))
		{
			return -5;
		}
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitXianyuanTreasCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_xianyuan_treas_cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_xianyuan_treas_cfg_count >= RAND_ACTIVITY_XIANYUAN_TREAS_CFG_COUNT)
		{
			return -20;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_xianyuan_treas_cfg_count)
		{
			return -1;
		}

		RandActivityXianyuanTreasConfig &xianyuan_treas_cfg = m_xianyuan_treas_cfg_list[seq];
		xianyuan_treas_cfg.seq = seq;

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -3;
		}

		if (!xianyuan_treas_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "max_reward_day", xianyuan_treas_cfg.max_reward_day) || xianyuan_treas_cfg.max_reward_day <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "consume_gold", xianyuan_treas_cfg.consume_gold) || xianyuan_treas_cfg.consume_gold <= 0)
		{
			return -6;
		}

		++m_xianyuan_treas_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitRushBuyingPhaseCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_seq = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++stage_num;
			last_seq = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		RandActivityRushBuyingPhaseDayConfig &day_cfg = m_rush_buying_phase_cfg_list[stage_num];
		day_cfg.opengame_day = opengame_day;

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || (seq != last_seq && seq != last_seq + 1))
		{
			return -1;
		}
		if (seq >= RA_RUSH_BUYING_PHASES_COUNT)
		{
			return -1000;
		}

		day_cfg.cfg_count = seq + 1;

		RandActivityRushBuyingPhaseConfig &cfg = day_cfg.cfg_list[seq];
		cfg.seq = seq;
		last_seq = seq;

		if (cfg.sale_items_count >= RA_RUSH_BUYING_SALES_ITEMS_COUNT)
		{
			return -2000;
		}

		int buying_begin_time = -1;
		if (!PugiGetSubNodeValue(data_element, "buying_begin_time", buying_begin_time) || buying_begin_time < 0 || buying_begin_time >= 24)		// [0, 23]
		{
			return -2;
		}

		if (seq > 0)
		{
			int pre_buying_begin_time = day_cfg.cfg_list[seq - 1].buying_begin_time;
			if (pre_buying_begin_time >= buying_begin_time)
			{
				return -3;
			}
		}

		cfg.buying_begin_time = buying_begin_time;

		int index = -1;
		if (!PugiGetSubNodeValue(data_element, "index", index) || index != cfg.sale_items_count)
		{
			return -4;
		}

		RandActivityRushBuyingPhaseConfig::SaleItemConfig &sale_item_cfg = cfg.sale_item_cfg_list[index];
		sale_item_cfg.index = index;

		{
			PugiXmlNode item_element = data_element.child("sale_item");
			if (item_element.empty())
			{
				return -100;
			}

			if (!sale_item_cfg.sale_item.ReadConfig(item_element))
			{
				return -101;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "sale_price", sale_item_cfg.sale_price) || sale_item_cfg.sale_price <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "original_price", sale_item_cfg.original_price) || sale_item_cfg.original_price <= 0 || sale_item_cfg.original_price < sale_item_cfg.sale_price)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "server_buy_times_limit", sale_item_cfg.server_buy_times_limit) || sale_item_cfg.server_buy_times_limit <= 0 || sale_item_cfg.server_buy_times_limit > RA_RUSH_BUYING_MAX_SERVER_BUYING_TIMES)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "role_buy_times_limit", sale_item_cfg.role_buy_times_limit) || sale_item_cfg.role_buy_times_limit <= 0 || sale_item_cfg.role_buy_times_limit > RA_RUSH_BUYING_MAX_ROLE_BUYING_TIMES)
		{
			return -8;
		}

		++cfg.sale_items_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitSingleChargeCfg2(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1;
	}

	int last_charge_value = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++stage_num;
			last_charge_value = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= RAND_ACTIVITY_SINGLE_CHARGE2_CFG_COUNT)
		{
			return -200;
		}
		m_single_charge_cfg2[stage_num].opengame_day = opengame_day;

		if (m_single_charge_cfg2[stage_num].cfg_count >= RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT)
		{
			return -2;
		}

		RandActivitySingleChargeCfg::ConfigItem *cfg_item = &m_single_charge_cfg2[stage_num].cfg_list[m_single_charge_cfg2[stage_num].cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item->seq) || cfg_item->seq != m_single_charge_cfg2[stage_num].cfg_count)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "charge_value", cfg_item->charge_value) || cfg_item->charge_value <= last_charge_value)
		{
			return -4;
		}
		last_charge_value = cfg_item->charge_value;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -5;
		}
		if (!cfg_item->reward_item.ReadConfig(reward_item))
		{
			return -6;
		}

		const ChongzhiReward *extra_reward_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetExtraRewardByNum(cfg_item->charge_value);
		if (NULL == extra_reward_cfg)
		{
			return -44;
		}

		++m_single_charge_cfg2[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitSingleChargeCfg3(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1;
	}

	int last_charge_value = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++stage_num;
			last_charge_value = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= RAND_ACTIVITY_SINGLE_CHARGE3_CFG_COUNT)
		{
			return -200;
		}
		m_single_charge_cfg3[stage_num].opengame_day = opengame_day;

		if (m_single_charge_cfg3[stage_num].cfg_count >= RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT)
		{
			return -2;
		}

		RandActivitySingleChargeCfg::ConfigItem *cfg_item = &m_single_charge_cfg3[stage_num].cfg_list[m_single_charge_cfg3[stage_num].cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item->seq) || cfg_item->seq != m_single_charge_cfg3[stage_num].cfg_count)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "charge_value", cfg_item->charge_value) || cfg_item->charge_value <= last_charge_value)
		{
			return -4;
		}
		last_charge_value = cfg_item->charge_value;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -5;
		}
		if (!cfg_item->reward_item.ReadConfig(reward_item))
		{
			return -6;
		}

		const ChongzhiReward *extra_reward_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetExtraRewardByNum(cfg_item->charge_value);
		if (NULL == extra_reward_cfg)
		{
			return -44;
		}

		++m_single_charge_cfg3[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitSingleChargeCfg4(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1;
	}

	int last_charge_value = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++stage_num;
			last_charge_value = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= RAND_ACTIVITY_SINGLE_CHARGE4_CFG_COUNT)
		{
			return -200;
		}
		m_single_charge_cfg4[stage_num].opengame_day = opengame_day;

		if (m_single_charge_cfg4[stage_num].cfg_count >= RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT)
		{
			return -2;
		}

		RandActivitySingleChargeCfg::ConfigItem *cfg_item = &m_single_charge_cfg4[stage_num].cfg_list[m_single_charge_cfg4[stage_num].cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item->seq) || cfg_item->seq != m_single_charge_cfg4[stage_num].cfg_count)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "charge_value", cfg_item->charge_value) || cfg_item->charge_value <= last_charge_value)
		{
			return -4;
		}
		last_charge_value = cfg_item->charge_value;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -5;
		}
		if (!cfg_item->reward_item.ReadConfig(reward_item))
		{
			return -6;
		}

		const ChongzhiReward *extra_reward_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetExtraRewardByNum(cfg_item->charge_value);
		if (NULL == extra_reward_cfg)
		{
			return -44;
		}

		++m_single_charge_cfg4[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitSingleChargeCfg5(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1;
	}

	int last_charge_value = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++stage_num;
			last_charge_value = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= RAND_ACTIVITY_SINGLE_CHARGE5_CFG_COUNT)
		{
			return -200;
		}
		m_single_charge_cfg5[stage_num].opengame_day = opengame_day;

		if (m_single_charge_cfg5[stage_num].cfg_count >= RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT)
		{
			return -2;
		}

		RandActivitySingleChargeCfg::ConfigItem *cfg_item = &m_single_charge_cfg5[stage_num].cfg_list[m_single_charge_cfg5[stage_num].cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item->seq) || cfg_item->seq != m_single_charge_cfg5[stage_num].cfg_count)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "charge_value", cfg_item->charge_value) || cfg_item->charge_value <= last_charge_value)
		{
			return -4;
		}
		last_charge_value = cfg_item->charge_value;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -5;
		}
		if (!cfg_item->reward_item.ReadConfig(reward_item))
		{
			return -6;
		}

		const ChongzhiReward *extra_reward_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetExtraRewardByNum(cfg_item->charge_value);
		if (NULL == extra_reward_cfg)
		{
			return -44;
		}

		++m_single_charge_cfg5[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitMapHuntCityCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int city_id = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int diff_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			diff_num++;
			last_opengame_day = opengame_day;
		}

		if (diff_num < 0 || diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		RandActivityMapHuntCityConfig &cfg_list = m_map_hunt_city_cfg_list[diff_num];
		cfg_list.opengame_day = opengame_day;

		if (!PugiGetSubNodeValue(data_element, "city_id", city_id) || city_id != cfg_list.city_count + 1)
		{
			return -1;
		}

		if (cfg_list.city_count >= MAX_RA_MAP_HUNT_CITY_COUNT)
		{
			return -2;
		}

		RandActivityMapHuntCityConfig::CityCfg &cfg = cfg_list.city_cfg_list[cfg_list.city_count];
		cfg.city_id = city_id;

		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight <= 0)
		{
			return -3;
		}
		m_map_hunt_total_city_weight_list[diff_num] += cfg.weight;

		PugiXmlNode item_element = data_element.child("base_reward_item");
		if (item_element.empty())
		{
			return -4;
		}

		if (!cfg.base_reward_item.ReadConfig(item_element))
		{
			return -5;
		}

		int is_broadcast = 0;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast))
		{
			return -6;
		}

		cfg.is_broadcast = (is_broadcast != 0) ? true : false;

		int baodi = 0;
		if (PugiGetSubNodeValue(data_element, "baodi", baodi) || baodi > 0)
		{
			if (cfg_list.baodi_map.size() > 0 || cfg_list.baodi_map.find(cfg_list.city_count) != cfg_list.baodi_map.end())
			{
				return -7;
			}

			cfg_list.baodi_map.insert(std::make_pair(baodi, cfg_list.city_count));
		}

		++cfg_list.city_count;
		data_element = data_element.next_sibling();
	}

	//for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	//{
	//	if (m_map_hunt_total_city_weight_list[i] <= 0)
	//	{
	//		return -300;
	//	}
	//}

	return 0;
}

int RandActivityConfig::InitMapHuntRouteCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int route_index = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int diff_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			diff_num++;
			last_opengame_day = opengame_day;
		}

		if (diff_num < 0 || diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		RandActivityMapHuntRouteConfig &cfg_list = m_map_hunt_route_cfg_list[diff_num];
		cfg_list.opengame_day = opengame_day;

		if (!PugiGetSubNodeValue(data_element, "route_index", route_index) || route_index != cfg_list.route_count + 1)
		{
			return -1;
		}

		if (cfg_list.route_count >= MAX_RA_MAP_HUNT_ROUTE_COUNT)
		{
			return -300;
		}

		RandActivityMapHuntRouteConfig::RouteCfg &cfg = cfg_list.route_cfg_list[cfg_list.route_count];
		cfg.route_index = route_index;

		for (int i = 0; i < MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE; ++i)
		{
			char temp[32] = { 0 };
			SNPRINTF(temp, sizeof(temp), "city_%d", (i + 1));
			if (!PugiGetSubNodeValue(data_element, temp, cfg.city_list[i]) || cfg.city_list[i] <= 0 || cfg.city_list[i] > m_map_hunt_city_cfg_list[diff_num].city_count)
			{
				return -2;
			}

			//检查一个城市在开头的必须只有一条路线,路线索引用第一个城市的id方便检查
			if (0 == i)
			{
				if (cfg.city_list[i] != route_index)
				{
					return -100;
				}
			}
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -5;
		}

		if (!cfg.reward_item.ReadConfig(item_element))
		{
			return -6;
		}

		++cfg_list.route_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitMapHuntServerRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int index = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int diff_num = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			diff_num++;
			last_opengame_day = opengame_day;
		}

		if (diff_num < 0 || diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		RandActivityMapHuntServerRewardConfig &cfg_list = m_map_hunt_server_cfg_list[diff_num];
		cfg_list.opengame_day = opengame_day;

		if (!PugiGetSubNodeValue(data_element, "index", index) || index != cfg_list.server_reward_cout)
		{
			return -1;
		}

		if (index >= MAX_RA_MAP_HUNT_SERVER_REWARD_COUNT)
		{
			return -2;
		}

		RandActivityMapHuntServerRewardConfig::RewardCfg &cfg = cfg_list.server_reward_cfg_list[index];
		cfg.index = index;

		if (!PugiGetSubNodeValue(data_element, "need_flush_count", cfg.need_flush_count) || cfg.need_flush_count < 0)
		{
			return -3;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -4;
		}

		if (!cfg.reward_item.ReadConfig(item_element))
		{
			return -5;
		}

		++cfg_list.server_reward_cout;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitLightTowerExploreCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int diff_num = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int layer = 0;
	int last_layer = -1;
	int index = 0;
	int last_index = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			last_opengame_day = opengame_day;
			++diff_num;
			last_layer = -1;
		}

		if (diff_num < 0 || diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}
		m_light_tower_explore_list[diff_num].opengame_day = opengame_day;

		if (!PugiGetSubNodeValue(data_element, "layer", layer) || layer < 0 || layer >= RA_LIGHT_TOWER_EXPLORE_MAX_LAYER)
		{
			return -1;
		}

		if (layer != last_layer && layer != last_layer + 1)
		{
			return -2;
		}

		if (layer != last_layer)
		{
			last_layer = layer;
			last_index = -1;
		}

		RandActivityLightTowerExploreCfg &temp_cfg = m_light_tower_explore_list[diff_num].layer_reward_list[layer];
		temp_cfg.layer = layer;

		if (!PugiGetSubNodeValue(data_element, "index", index) || index != last_index + 1 || index < 0 || index >= RA_LIGHT_TOWER_EXPLORE_MAX_PER_LAYER_REWAED_COUNT)
		{
			return -3;
		}
		last_index = index;

		RandActivityLightTowerExploreCfg::LayerConfig &cfg = temp_cfg.reward_list[index];
		cfg.index = index;

		if (!PugiGetSubNodeValue(data_element, "reward_type", cfg.reward_type) || (LIGHT_TOWER_EXPLORE_REWAED_TYPE_ITEM != cfg.reward_type &&
			LIGHT_TOWER_EXPLORE_REWAED_TYPE_GOTO_LAYER != cfg.reward_type))
		{
			return -4;
		}
		//保证每层第一个为物品，跳层固定给
		if (0 == index && LIGHT_TOWER_EXPLORE_REWAED_TYPE_ITEM != cfg.reward_type)
		{
			return -40;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", cfg.weight) || cfg.weight <= 0)
		{
			return -5;
		}

		temp_cfg.layer_total_weight += cfg.weight;

		if (LIGHT_TOWER_EXPLORE_REWAED_TYPE_ITEM == cfg.reward_type)
		{
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				return -1000;
			}

			if (!cfg.reward_item.ReadConfig(item_element))
			{
				return -6;
			}
		}
		else
		{
			if (!PugiGetSubNodeValue(data_element, "goto_layer", cfg.goto_layer) || cfg.goto_layer < 0 || cfg.goto_layer >= RA_LIGHT_TOWER_EXPLORE_MAX_LAYER)
			{
				return -7;
			}
		}

		int is_broadcast = 0;

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast) || is_broadcast < 0)
		{
			return -8;
		}

		cfg.is_broadcast = (1 == is_broadcast) ? true : false;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitLightTowerExploreExternCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int diff_num = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int layer = 0;
	int last_layer = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			last_opengame_day = opengame_day;
			++diff_num;
			last_layer = -1;
		}

		if (diff_num < 0 || diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}
		m_light_tower_explore_extern_reward_list[diff_num].opengame_day = opengame_day;

		if (!PugiGetSubNodeValue(data_element, "layer", layer) || layer < 0 || layer >= RA_LIGHT_TOWER_EXPLORE_MAX_LAYER || layer != last_layer + 1)
		{
			return -1;
		}
		last_layer = layer;

		RandActivityLightTowerExploreExternCfg &cfg = m_light_tower_explore_extern_reward_list[diff_num].layer_extern_reward_list[layer];
		cfg.layer = layer;

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -1000;
		}

		if (!cfg.reward_item.ReadConfig(item_element))
		{
			return -2;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitLightTowerExploreServerCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int diff_num = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int index = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			last_opengame_day = opengame_day;
			++diff_num;
		}

		if (diff_num < 0 || diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}
		m_light_tower_explore_server_reward_list[diff_num].opengame_day = opengame_day;

		if (!PugiGetSubNodeValue(data_element, "index", index) || index < 0 || index >= RA_LIGHT_TOWER_EXPLORE_MAX_SERVER_REWARD_COUNT)
		{
			return -1;
		}

		RandActivityLightTowerExploreReturnCfg &cfg = m_light_tower_explore_server_reward_list[diff_num].server_reward_list[index];
		cfg.index = index;

		if (!PugiGetSubNodeValue(data_element, "need_count", cfg.need_count) || cfg.need_count <= 0)
		{
			return -2;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -1000;
		}

		if (!cfg.reward_item.ReadConfig(item_element))
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitNewTotalChongzhiCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_opengame_day = 0;
	int last_need_chongzhi = 0;
	int diff_num = -1;

	while (!data_element.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < last_opengame_day || opengame_day <= 0)
		{
			return -100;
		}

		if (diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (last_opengame_day != opengame_day)
		{
			++diff_num;
			last_opengame_day = opengame_day;
			last_need_chongzhi = 0;
		}

		RandActivityNewTotalChargeRewardConfig &total_chognzhi_reward_cfg = m_diff_new_totalcharge_reward_list[diff_num];
		total_chognzhi_reward_cfg.opengame_day = opengame_day;

		if (total_chognzhi_reward_cfg.cfg_count >= RandActivityTotalChargeRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -30000;
		}

		RandActivityNewTotalChargeRewardConfig::ConfigItem &cfg_item = total_chognzhi_reward_cfg.cfg_list[total_chognzhi_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != total_chognzhi_reward_cfg.cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "need_chognzhi", cfg_item.need_chongzhi) || cfg_item.need_chongzhi <= last_need_chongzhi)
		{
			return -2;
		}
		last_need_chongzhi = cfg_item.need_chongzhi;

		{
			// 读物品奖励
			PugiXmlNode item_list_element = data_element.child("reward_item_list");
			if (item_list_element.empty())
			{
				return -1001;
			}

			PugiXmlNode item_element = item_list_element.child("reward_item");
			if (item_element.empty())
			{
				return -1002;
			}

			cfg_item.item_count = 0;

			while (!item_element.empty())
			{
				if (cfg_item.item_count >= RandActivityTotalChargeRewardConfig::REWARD_ITEM_MAX_COUNT)
				{
					return -1003;
				}

				if (!cfg_item.item_list[cfg_item.item_count].ReadConfig(item_element))
				{
					return -1004;
				}

				++cfg_item.item_count;
				item_element = item_element.next_sibling();
			}
		}

		++total_chognzhi_reward_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitNewTotalChongzhi1Cfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_opengame_day = 0;
	int last_need_chongzhi = 0;
	int diff_num = -1;

	while (!data_element.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < last_opengame_day || opengame_day <= 0)
		{
			return -100;
		}

		if (diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (last_opengame_day != opengame_day)
		{
			++diff_num;
			last_opengame_day = opengame_day;
			last_need_chongzhi = 0;
		}

		RandActivityNewTotalChargeRewardConfig &total_chognzhi_reward_cfg = m_diff_new_totalcharge1_reward_list[diff_num];
		total_chognzhi_reward_cfg.opengame_day = opengame_day;

		if (total_chognzhi_reward_cfg.cfg_count >= RandActivityTotalChargeRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -30000;
		}

		RandActivityNewTotalChargeRewardConfig::ConfigItem &cfg_item = total_chognzhi_reward_cfg.cfg_list[total_chognzhi_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != total_chognzhi_reward_cfg.cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "need_chognzhi", cfg_item.need_chongzhi) || cfg_item.need_chongzhi <= last_need_chongzhi)
		{
			return -2;
		}
		last_need_chongzhi = cfg_item.need_chongzhi;

		{
			// 读物品奖励
			PugiXmlNode item_list_element = data_element.child("reward_item_list");
			if (item_list_element.empty())
			{
				return -1001;
			}

			PugiXmlNode item_element = item_list_element.child("reward_item");
			if (item_element.empty())
			{
				return -1002;
			}

			cfg_item.item_count = 0;

			while (!item_element.empty())
			{
				if (cfg_item.item_count >= RandActivityTotalChargeRewardConfig::REWARD_ITEM_MAX_COUNT)
				{
					return -1003;
				}

				if (!cfg_item.item_list[cfg_item.item_count].ReadConfig(item_element))
				{
					return -1004;
				}

				++cfg_item.item_count;
				item_element = item_element.next_sibling();
			}
		}

		++total_chognzhi_reward_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitNewTotalChongzhi2Cfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_opengame_day = 0;
	int last_need_chongzhi = 0;
	int diff_num = -1;

	while (!data_element.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < last_opengame_day || opengame_day <= 0)
		{
			return -100;
		}

		if (diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (last_opengame_day != opengame_day)
		{
			++diff_num;
			last_opengame_day = opengame_day;
			last_need_chongzhi = 0;
		}

		RandActivityNewTotalChargeRewardConfig &total_chognzhi_reward_cfg = m_diff_new_totalcharge2_reward_list[diff_num];
		total_chognzhi_reward_cfg.opengame_day = opengame_day;

		if (total_chognzhi_reward_cfg.cfg_count >= RandActivityTotalChargeRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -30000;
		}

		RandActivityNewTotalChargeRewardConfig::ConfigItem &cfg_item = total_chognzhi_reward_cfg.cfg_list[total_chognzhi_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != total_chognzhi_reward_cfg.cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "need_chognzhi", cfg_item.need_chongzhi) || cfg_item.need_chongzhi <= last_need_chongzhi)
		{
			return -2;
		}
		last_need_chongzhi = cfg_item.need_chongzhi;

		{
			// 读物品奖励
			PugiXmlNode item_list_element = data_element.child("reward_item_list");
			if (item_list_element.empty())
			{
				return -1001;
			}

			PugiXmlNode item_element = item_list_element.child("reward_item");
			if (item_element.empty())
			{
				return -1002;
			}

			cfg_item.item_count = 0;

			while (!item_element.empty())
			{
				if (cfg_item.item_count >= RandActivityTotalChargeRewardConfig::REWARD_ITEM_MAX_COUNT)
				{
					return -1003;
				}

				if (!cfg_item.item_list[cfg_item.item_count].ReadConfig(item_element))
				{
					return -1004;
				}

				++cfg_item.item_count;
				item_element = item_element.next_sibling();
			}
		}

		++total_chognzhi_reward_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitNewTotalChongzhi3Cfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_opengame_day = 0;
	int last_need_chongzhi = 0;
	int diff_num = -1;

	while (!data_element.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < last_opengame_day || opengame_day <= 0)
		{
			return -100;
		}

		if (diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (last_opengame_day != opengame_day)
		{
			++diff_num;
			last_opengame_day = opengame_day;
			last_need_chongzhi = 0;
		}

		RandActivityNewTotalChargeRewardConfig &total_chognzhi_reward_cfg = m_diff_new_totalcharge3_reward_list[diff_num];
		total_chognzhi_reward_cfg.opengame_day = opengame_day;

		if (total_chognzhi_reward_cfg.cfg_count >= RandActivityTotalChargeRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -30000;
		}

		RandActivityNewTotalChargeRewardConfig::ConfigItem &cfg_item = total_chognzhi_reward_cfg.cfg_list[total_chognzhi_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != total_chognzhi_reward_cfg.cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "need_chognzhi", cfg_item.need_chongzhi) || cfg_item.need_chongzhi <= last_need_chongzhi)
		{
			return -2;
		}
		last_need_chongzhi = cfg_item.need_chongzhi;

		{
			// 读物品奖励
			PugiXmlNode item_list_element = data_element.child("reward_item_list");
			if (item_list_element.empty())
			{
				return -1001;
			}

			PugiXmlNode item_element = item_list_element.child("reward_item");
			if (item_element.empty())
			{
				return -1002;
			}

			cfg_item.item_count = 0;

			while (!item_element.empty())
			{
				if (cfg_item.item_count >= RandActivityTotalChargeRewardConfig::REWARD_ITEM_MAX_COUNT)
				{
					return -1003;
				}

				if (!cfg_item.item_list[cfg_item.item_count].ReadConfig(item_element))
				{
					return -1004;
				}

				++cfg_item.item_count;
				item_element = item_element.next_sibling();
			}
		}

		++total_chognzhi_reward_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitMagicShopCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_opengame_day = 0;
	int diff_num = -1;
	while (!data_element.empty())
	{
		RandActivityMagicShopConfig cfg;

		if (!PugiGetSubNodeValue(data_element, "opengame_day", cfg.opengame_day) || cfg.opengame_day < last_opengame_day || cfg.opengame_day <= 0)
		{
			return -1;
		}

		if (diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (last_opengame_day != cfg.opengame_day)
		{
			++diff_num;
			last_opengame_day = cfg.opengame_day;
		}

		if (!PugiGetSubNodeValue(data_element, "activity_day", cfg.activity_day) || cfg.activity_day <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "index", cfg.index) || cfg.index < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "need_gold", cfg.need_gold) || cfg.need_gold < 0)
		{
			return -5;
		}

		{
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				return -6;
			}

			if (!cfg.reward_item.ReadConfig(item_element))
			{
				return -7;
			}
		}

		m_magic_shop_cfg_list.push_back(cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitPlantingTreeRankRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_planting_tree_rank_reward_count = 0;

	while (!data_element.empty())
	{
		if (m_planting_tree_rank_reward_count >= RA_TREE_PLANTING_RANK_MAX_COUNT)
		{
			return -2000;
		}

		int rank = 0;
		if (!PugiGetSubNodeValue(data_element, "rank", rank) || rank != m_planting_tree_rank_reward_count + 1)
		{
			return -1;
		}

		RandActivityPlantingTreeRankRewardConfig &reward_cfg = m_planting_tree_rank_reward_cfg_list[rank];
		reward_cfg.rank = rank;

		{
			int reward_count = 0;

			PugiXmlNode reward_list_element = data_element.child("planting_rank_reward_list");
			if (reward_list_element.empty())
			{
				return -2;
			}

			PugiXmlNode reward_element = reward_list_element.child("planting_rank_reward");
			if (reward_element.empty())
			{
				return -3;
			}

			while (!reward_element.empty())
			{
				if (reward_count >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -4;
				}

				if (!reward_cfg.planting_rank_reward[reward_count].ReadConfig(reward_element))
				{
					return -5;
				}

				++reward_count;
				reward_element = reward_element.next_sibling();
			}

			reward_count = 0;

			reward_list_element = data_element.child("watering_rank_reward_list");
			if (reward_list_element.empty())
			{
				return -6;
			}

			reward_element = reward_list_element.child("watering_rank_reward");
			if (reward_element.empty())
			{
				return -7;
			}

			while (!reward_element.empty())
			{
				if (reward_count >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -8;
				}

				if (!reward_cfg.watering_rank_reward[reward_count].ReadConfig(reward_element))
				{
					return -9;
				}

				++reward_count;
				reward_element = reward_element.next_sibling();
			}
		}

		++m_planting_tree_rank_reward_count;
		data_element = data_element.next_sibling();
	}

	if (m_planting_tree_rank_reward_count != RA_TREE_PLANTING_RANK_MAX_COUNT)
	{
		return -3000;
	}

	return 0;
}

const RandActivityPlantingTreeRankRewardConfig * RandActivityConfig::GetPlantingTreeRankRewardCfg(int rank)
{
	if (rank <= 0 || rank > m_planting_tree_rank_reward_count || rank > RA_TREE_PLANTING_RANK_MAX_COUNT)
	{
		return NULL;
	}

	return &m_planting_tree_rank_reward_cfg_list[rank];
}

int RandActivityConfig::InitFishInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int index = 0;
	RandActivityFishingInfoCfg fish_cfg;
	int total_weight = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "fish_type", index) || index < 0 || index > RA_FISHING_FISH_TYPE_COUNT)
		{
			return -1;
		}

		fish_cfg.fish_type = index;

		if (!PugiGetSubNodeValue(data_element, "fish_score", fish_cfg.fish_score) || fish_cfg.fish_score <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "steal_fish_score", fish_cfg.be_steal_fish_score) || fish_cfg.be_steal_fish_score <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", fish_cfg.is_broadcast))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "fish_num", fish_cfg.fish_num) || fish_cfg.fish_num <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "fish_weight", fish_cfg.fish_weight) || fish_cfg.fish_weight <= 0)
		{
			return -6;
		}
		total_weight += fish_cfg.fish_weight;

		PugiXmlNode item_element = data_element.child("change_reward");
		if (item_element.empty())
		{
			return -7;
		}

		if (!fish_cfg.reward_item.ReadConfig(item_element))
		{
			return -8;
		}

		m_fish_info_list[index] = fish_cfg;

		data_element = data_element.next_sibling();
	}

	if (100 != total_weight)
	{
		return -9;
	}

	return 0;
}

int RandActivityConfig::InitFishRankRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	RandActivityFishingRankCfg reward_cfg;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "rank_num", reward_cfg.rank_num) || reward_cfg.rank_num <= 0 || reward_cfg.rank_num > RA_FISHING_RANK_MAX_COUNT)
		{
			return -1;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -2;
		}

		if (!reward_cfg.reward_item.ReadConfig(item_element))
		{
			return -3;
		}

		PugiXmlNode item_element2 = data_element.child("be_steal_reward");
		if (item_element2.empty())
		{
			return -4;
		}

		if (!reward_cfg.be_stealed_reward_item.ReadConfig(item_element2))
		{
			return -5;
		}

		m_fish_rank_reward_list[reward_cfg.rank_num] = reward_cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

RandActivityFishingInfoCfg *RandActivityConfig::GetFishinfo()
{
	int rand_num = RandomNum(100);
	for (int i = 0; i < RA_FISHING_FISH_TYPE_COUNT; i++)
	{
		if (rand_num <= m_fish_info_list[i].fish_weight)
		{
			return &m_fish_info_list[i];
		}

		rand_num -= m_fish_info_list[i].fish_weight;
	}

	return NULL;
}

RandActivityFishingInfoCfg *RandActivityConfig::GetFishinfobyType(short fish_type)
{
	if (fish_type < 0 || fish_type > RA_FISHING_FISH_TYPE_COUNT)
	{
		return NULL;
	}

	return &m_fish_info_list[fish_type];
}

RandActivityFishingRankCfg *RandActivityConfig::GetFishingRewardInfo(short rank_num)
{
	if (rank_num <= 0 || rank_num > RA_FISHING_RANK_MAX_COUNT)
	{
		return NULL;
	}

	return &m_fish_rank_reward_list[rank_num];
}

int RandActivityConfig::InitCrtstalDalaranRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int index = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "rank_num", index) || index < 1 || index > RA_CRYSTAL_DALARAN_RANK_REWARD_COUNT)
		{
			return -1;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -2;
		}

		if (!m_steal_crystal_dalaran_reward[index].reward_item.ReadConfig(item_element))
		{
			return -3;
		}

		PugiXmlNode item_element2 = data_element.child("be_stealed_reward_item");
		if (item_element2.empty())
		{
			return -4;
		}

		if (!m_steal_crystal_dalaran_reward[index].be_stealed_reward_item.ReadConfig(item_element2))
		{
			return -5;
		}

		m_steal_crystal_dalaran_reward[index].rank_num = index;

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityStealCrtstalDalaranRewardCfg *RandActivityConfig::GetStealCrystalDalaranRewardCfg(int rank_num)
{
	if (rank_num <= 0 || rank_num > RA_CRYSTAL_DALARAN_RANK_REWARD_COUNT)
	{
		return NULL;
	}

	return &m_steal_crystal_dalaran_reward[rank_num];
}

int RandActivityConfig::InitChongzhiCrazyRabateCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_high = 0;
	while (!data_element.empty())
	{
		RandActivityChongzhiCrazyRebateConfig cfg;
		if (!PugiGetSubNodeValue(data_element, "gold_low_limit", cfg.gold_low_limit) || cfg.gold_low_limit < 0 || cfg.gold_low_limit != last_high)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "gold_high_limit", cfg.gold_high_limit) || cfg.gold_high_limit < cfg.gold_low_limit)
		{
			return -2;
		}
		last_high = cfg.gold_high_limit;

		if (!PugiGetSubNodeValue(data_element, "reward_precent", cfg.reward_precent) || cfg.reward_precent < 0 || cfg.reward_precent > 100)
		{
			return -3;
		}

		m_chongzhi_crazy_rebate_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::GetChongzhiCrazyRebateRewardGold(int total_chongzhi_count, int had_rabate_count)
{
	if (total_chongzhi_count <= 0)
	{
		return 0;
	}

	int rebate_cout = 0;
	ChongzhiCrazyRebateVec::const_iterator iter = m_chongzhi_crazy_rebate_vec.begin();

	for (; iter != m_chongzhi_crazy_rebate_vec.end(); iter++)
	{
		if (total_chongzhi_count >= iter->gold_low_limit && had_rabate_count < iter->gold_high_limit)
		{
			int cur_state_max_rebate = 0;
			if (total_chongzhi_count >= iter->gold_high_limit)
			{
				cur_state_max_rebate = iter->gold_high_limit - iter->gold_low_limit;
			}
			else
			{
				cur_state_max_rebate = total_chongzhi_count - iter->gold_low_limit;
			}

			if (had_rabate_count <= iter->gold_low_limit)
			{
				rebate_cout += (int)(cur_state_max_rebate * (iter->reward_precent / 100.0));
			}
			else
			{
				rebate_cout += (int)((cur_state_max_rebate - (had_rabate_count - iter->gold_low_limit)) * (iter->reward_precent / 100.0));
			}
		}
	}

	return rebate_cout;
}

int RandActivityConfig::InitJinJieThemeAct(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int act_id = 0;
	int opengame_day = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "act_id", act_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -2;
		}

		long long key = ConvertParamToLongLong(act_id, opengame_day);
		if (m_jinjie_theme_act_map.find(key) != m_jinjie_theme_act_map.end())
		{
			return -10001;
		}

		JinjieThemeActConfig &cfg = m_jinjie_theme_act_map[key];
		cfg.act_id = act_id;
		cfg.opengame_day = opengame_day;

		if (!PugiGetSubNodeValue(data_element, "act_theme", cfg.act_theme) || cfg.act_theme <= JINJIE_ACT_THEME_TYPE_INVALID || cfg.act_theme >= JINJIE_ACT_THEME_TYPE_MAX)
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const int RandActivityConfig::GetJinJieThemeActTheme(int act_id)
{
	int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) % RAND_ACTIVITY_JINJIE_THEME_ACT_MAX_OPENGAME_DAY + 1;
	long long key = ConvertParamToLongLong(act_id, opengame_day);
	std::map<long long, JinjieThemeActConfig>::iterator it = m_jinjie_theme_act_map.find(key);
	if (it != m_jinjie_theme_act_map.end())
	{
		return it->second.act_theme;
	}

	return 0;
}

int RandActivityConfig::InitConsumGiftCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_seq = -1;
	int last_theme = 0;
	while (!data_element.empty())
	{
		ConsumGiftRewardConfig cfg;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", cfg.opengame_day) || cfg.opengame_day <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "act_theme", cfg.act_theme) || cfg.act_theme <= JINJIE_ACT_THEME_TYPE_INVALID || cfg.act_theme >= JINJIE_ACT_THEME_TYPE_MAX)
		{
			return -2;
		}

		if (cfg.act_theme != last_theme)
		{
			last_seq = -1;
		}

		last_theme = cfg.act_theme;

		if (!PugiGetSubNodeValue(data_element, "seq", cfg.seq) || cfg.seq < 0 || cfg.seq >= RAND_ACTIVITY_CONSUM_GIFT_MAX_SEQ || cfg.seq != last_seq + 1)
		{
			return -3;
		}

		last_seq = cfg.seq;

		if (!PugiGetSubNodeValue(data_element, "need_gold", cfg.need_gold) || cfg.need_gold <= 0)
		{
			return -4;
		}

		{
			cfg.item_count = 0;
			PugiXmlNode item_list_element = data_element.child("reward_item_list");
			if (item_list_element.empty())
			{
				return -5;
			}

			PugiXmlNode item_element = item_list_element.child("reward_item");
			if (item_element.empty())
			{
				return -6;
			}

			while (!item_element.empty())
			{
				if (cfg.item_count >= RAND_ACTIVITY_CONSUM_GIFT_MAX_REWARD_COUNT)
				{
					return -7;
				}

				if (!cfg.reward_item_list[cfg.item_count].ReadConfig(item_element))
				{
					return -8;
				}

				++cfg.item_count;
				item_element = item_element.next_sibling();
			}
		}

		m_consum_gift_vec.push_back(cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitConsumGiftRollPoolCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		ConsumGiftRollRewardPool cfg;

		if (!PugiGetSubNodeValue(data_element, "opengame_day", cfg.opengame_day) || cfg.opengame_day <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "act_theme", cfg.act_theme) || cfg.act_theme <= JINJIE_ACT_THEME_TYPE_INVALID || cfg.act_theme >= JINJIE_ACT_THEME_TYPE_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", cfg.seq) || cfg.seq < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "ratio", cfg.num_rate) || cfg.num_rate < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "num_type", cfg.num_type) || cfg.num_type <= 0)
		{
			return -5;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -6;
		}

		if (!cfg.roll_reward_item.ReadConfig(item_element))
		{
			return -7;
		}

		m_consum_gift_roll_reward_pool.push_back(cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitConsumGiftRollCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		ConsumGiftRollRewardConfig cfg;
		if (!PugiGetSubNodeValue(data_element, "num_type", cfg.num_type) || cfg.num_type <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "decade", cfg.decade_num) || cfg.decade_num < 0 || cfg.decade_num > 9)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "decade_ratio", cfg.decade_rate) || cfg.decade_rate < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "units_digit", cfg.units_digit_num) || cfg.units_digit_num < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "units_digit_ratio", cfg.units_digit_rate) || cfg.units_digit_rate < 0)
		{
			return -5;
		}

		m_consum_gift_roll_reward_vec.push_back(cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

const ConsumGiftRewardConfig *RandActivityConfig::GetConsumGiftRewardCfg(int opengame_day, int theme, int seq)
{
	if ((theme <= JINJIE_ACT_THEME_TYPE_INVALID || theme >= JINJIE_ACT_THEME_TYPE_MAX) || (seq < 0 || seq >= RAND_ACTIVITY_CONSUM_GIFT_MAX_SEQ))
	{
		return nullptr;
	}

	for (ConsumGiftRewardVecIt it = m_consum_gift_vec.begin(); it != m_consum_gift_vec.end(); ++it)
	{
		if (opengame_day <= it->opengame_day && it->act_theme == theme && it->seq == seq)
		{
			return &(*it);
		}
	}
	return nullptr;
}

const ConsumGiftRollRewardPool *RandActivityConfig::GetConsumGiftRollNumType(int theme)
{
	int total_rate = 0;
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return nullptr;
	}
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	for (int i = 0; i < static_cast<int>(m_consum_gift_roll_reward_pool.size()); ++i)
	{
		if (m_consum_gift_roll_reward_pool[i].act_theme != theme || opengame_day > m_consum_gift_roll_reward_pool[i].opengame_day)
		{
			continue;
		}

		total_rate += m_consum_gift_roll_reward_pool[i].num_rate;
	}

	if (total_rate <= 0)
	{
		return nullptr;
	}

	int rand_num = RandomNum(total_rate);
	for (int i = 0; i < static_cast<int>(m_consum_gift_roll_reward_pool.size()); ++i)
	{
		if (m_consum_gift_roll_reward_pool[i].act_theme != theme || opengame_day > m_consum_gift_roll_reward_pool[i].opengame_day)
		{
			continue;
		}

		if (rand_num <= m_consum_gift_roll_reward_pool[i].num_rate)
		{
			return &m_consum_gift_roll_reward_pool[i];
		}
		rand_num -= m_consum_gift_roll_reward_pool[i].num_rate;
	}

	return nullptr;
}

const ConsumGiftRollRewardPool *RandActivityConfig::GetConsumGiftRollReward(int theme, int seq)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return nullptr;
	}
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	for (int i = 0; i < static_cast<int>(m_consum_gift_roll_reward_pool.size()); ++i)
	{
		if (opengame_day <= m_consum_gift_roll_reward_pool[i].opengame_day && m_consum_gift_roll_reward_pool[i].act_theme == theme && m_consum_gift_roll_reward_pool[i].seq == seq)
		{
			return &m_consum_gift_roll_reward_pool[i];
		}
	}

	return nullptr;
}

const bool RandActivityConfig::GetConsumGiftRollRewardNumByType(int type, int &decade, int &units_digit)
{
	int total_decade_rate = 0;
	int units_digit_total_rate = 0;
	for (int i = 0; i < static_cast<int>(m_consum_gift_roll_reward_vec.size()); ++i)
	{
		if (m_consum_gift_roll_reward_vec[i].num_type != type)
		{
			continue;
		}
		total_decade_rate += m_consum_gift_roll_reward_vec[i].decade_rate;
		units_digit_total_rate += m_consum_gift_roll_reward_vec[i].units_digit_rate;
	}

	if (0 == total_decade_rate || 0 == units_digit_total_rate)
	{
		return false;
	}

	int decade_rand_rate = RandomNum(total_decade_rate);
	int units_digit_rand_rate = RandomNum(units_digit_total_rate);
	for (int i = 0; i < static_cast<int>(m_consum_gift_roll_reward_vec.size()); ++i)
	{
		if (m_consum_gift_roll_reward_vec[i].num_type != type)
		{
			continue;
		}

		if (decade < 0)
		{
			if (decade_rand_rate < m_consum_gift_roll_reward_vec[i].decade_rate)
			{
				decade = m_consum_gift_roll_reward_vec[i].decade_num;
			}

			decade_rand_rate -= m_consum_gift_roll_reward_vec[i].decade_rate;
		}
		
		if (units_digit < 0)
		{
			if (units_digit_rand_rate < m_consum_gift_roll_reward_vec[i].units_digit_rate)
			{
				units_digit = m_consum_gift_roll_reward_vec[i].units_digit_num;
			}

			units_digit_rand_rate -= m_consum_gift_roll_reward_vec[i].units_digit_rate;
		}

		if (decade >= 0 && units_digit >= 0)
		{
			return true;
		}
	}

	return false;
}

int RandActivityConfig::InitDailyLimitBuyConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int act_type = 0;
	int last_act_type = -1;
	int seq = 0;
	//int last_seq = -1;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "act_type", act_type) || act_type <= JINJIE_ACT_THEME_TYPE_INVALID || act_type >= JINJIE_ACT_THEME_TYPE_MAX)
		{
			return -1;
		}

		if (last_act_type != act_type)
		{
			//last_seq = -1;
			last_act_type = act_type;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= RAND_ACTIVITY_DAILY_LIMIT_BUY_MAX_SEQ)
		{
			return -2;
		}

		DailyLimitBuyConfig &cfg = m_daily_limit_buy_cfg[act_type][seq];
		cfg.act_type = act_type;
		cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "gold_price", cfg.gold_price) || cfg.gold_price <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "limit_buy_count", cfg.limit_buy_count) || cfg.limit_buy_count <= 0)
		{
			return -4;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -5;
		}

		if (!cfg.reward_item.ReadConfig(item_element))
		{
			return -6;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const DailyLimitBuyConfig *RandActivityConfig::GetLimitBuyCfg(int theme, int seq)
{
	if ((theme <= JINJIE_ACT_THEME_TYPE_INVALID || theme >= JINJIE_ACT_THEME_TYPE_MAX) || (seq < 0 || seq >= RAND_ACTIVITY_DAILY_LIMIT_BUY_MAX_SEQ))
	{
		return nullptr;
	}

	return &m_daily_limit_buy_cfg[theme][seq];
}

int RandActivityConfig::InitCollectTreasureConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int seq = 0;
	int last_seq = -1;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= RAND_ACTIVITY_COLLECT_TREASURE_MAX_CFG || seq != last_seq + 1)
		{
			return -1;
		}

		last_seq = seq;

		CollectTreasureConfig &cfg = m_collect_treasure_cfg[seq];
		cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "units_digit", cfg.units_digit) || cfg.units_digit < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "units_digit_ratio", cfg.units_rate) || cfg.units_rate < 0)
		{
			return -3;
		}

		m_collect_treasure_total_rate += cfg.units_rate;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const CollectTreasureConfig *RandActivityConfig::GetCollectTreasureCfg()
{
	if (m_collect_treasure_total_rate <= 0)
	{
		return nullptr;
	}

	int rand_num = RandomNum(m_collect_treasure_total_rate);

	for (int i = 0; i < static_cast<int>(static_array_size(m_collect_treasure_cfg)); ++i)
	{
		if (rand_num < m_collect_treasure_cfg[i].units_rate)
		{
			return &m_collect_treasure_cfg[i];
		}

		rand_num -= m_collect_treasure_cfg[i].units_rate;
	}

	return nullptr;
}

const CollectTreasureConfig *RandActivityConfig::GetCollectTreasureCfgBySeq(int seq)
{
	if (seq < 0 || seq >= static_cast<int>(static_array_size(m_collect_treasure_cfg)))
	{
		return nullptr;
	}

	return &m_collect_treasure_cfg[seq];
}

int RandActivityConfig::InitCriticalStrikeDayConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int act_type = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "act_type", act_type) || act_type <= JINJIE_ACT_THEME_TYPE_INVALID || act_type >= JINJIE_ACT_THEME_TYPE_MAX)
		{
			return -1;
		}

		CriticalStrikeDayConfig &cfg = m_critical_strike_day_cfg[act_type];

		cfg.act_type = act_type;

		if (!PugiGetSubNodeValue(data_element, "baoji_rate", cfg.baoji_rate) || cfg.baoji_rate < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji_mul", cfg.baoji_mul) || cfg.baoji_mul <= 0)
		{
			return -3;
		}
		
		data_element = data_element.next_sibling();
	}

	return 0;
}

const int RandActivityConfig::GetCriticalStrikeDayMul(int type)
{
	int rand_num = RandomNum(100);
	for (int i = 1; i < static_cast<int>(static_array_size(m_critical_strike_day_cfg)); ++i)
	{
		if (m_critical_strike_day_cfg[i].act_type == type && rand_num <= m_critical_strike_day_cfg[i].baoji_rate)
		{
			return m_critical_strike_day_cfg[i].baoji_mul;
		}
	}

	return 1;
}

const int RandActivityConfig::GetCriticalStrikeDayRate(int type)
{
	for (int i = 1; i < static_cast<int>(static_array_size(m_critical_strike_day_cfg)); ++i)
	{
		if (m_critical_strike_day_cfg[i].act_type == type)
		{
			return m_critical_strike_day_cfg[i].baoji_rate;
		}
	}
	return 0;
}

const int RandActivityConfig::GetConsumeForGiftItemCount()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CONSUME_FOR_GIFT);

	for (int i = 0; i < (int)m_consume_for_gift_item_cfg_list.size(); ++i)
	{
		if (opengame_day <= m_consume_for_gift_item_cfg_list[i].opengame_day)
		{
			std::map<int, int>::iterator iter = m_consume_for_gift_item_count_map.find(m_consume_for_gift_item_cfg_list[i].opengame_day);
			if (iter == m_consume_for_gift_item_count_map.end())
				return 0;

			return m_consume_for_gift_item_count_map[m_consume_for_gift_item_cfg_list[i].opengame_day];
		}
	}

	return 0;
}

int RandActivityConfig::GetConsumeForGiftPointsPerGold()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CONSUME_FOR_GIFT);
	for (int index = 0; index < (int)m_consume_for_gift_item_cfg_list.size(); ++index)
	{
		if (opengame_day <= m_consume_for_gift_item_cfg_list[index].opengame_day)
		{
			return m_consume_for_gift_item_cfg_list[index].point_per_gold;
		}
	}

	return -1;
}

const RandActivityConsumeForGiftItemConfig * RandActivityConfig::GetConsumeForGiftItemCfg(int seq)
{
	if (seq < 0 || seq >= RA_CONSUME_FOR_GIFT_MAX_ITEM_COUNT)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CONSUME_FOR_GIFT);
	for (int index = 0; index < (int)m_consume_for_gift_item_cfg_list.size(); ++index)
	{
		if (opengame_day <= m_consume_for_gift_item_cfg_list[index].opengame_day && m_consume_for_gift_item_cfg_list[index].seq == seq)
		{
			return &m_consume_for_gift_item_cfg_list[index];
		}
	}
	return NULL;
}

int RandActivityConfig::GetMiJingXunBao3AutoWeightAddCount()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MIJINGXUNBAO3);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_mijingxunbao3_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	return m_mijingxunbao3_auto_add_cfg_count_list[stage];
}

int RandActivityConfig::GetMiJingXunBao3CfgAddWeight(int seq)
{
	if (seq <= 0 || seq > RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ)
	{
		return 0;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MIJINGXUNBAO3);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_mijingxunbao3_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	if (seq > m_mijingxunbao3_max_seq_list[stage])
	{
		return 0;
	}

	for (int index = 0; index < m_mijingxunbao3_auto_add_cfg_count_list[stage]; index++)
	{
		if (m_mijingxunbao3_add_weight_cfg_list[stage].add_weight_cfg_list[index].seq == seq)
		{
			return m_mijingxunbao3_add_weight_cfg_list[stage].add_weight_cfg_list[index].add_weight;
		}
	}

	return 0;
}

int RandActivityConfig::GetMiJingXunBao3AddWeightSeq(int index)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MIJINGXUNBAO3);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_mijingxunbao3_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	if (index < 0 || index >= m_mijingxunbao3_auto_add_cfg_count_list[stage])
	{
		return 0;
	}

	return m_mijingxunbao3_add_weight_cfg_list[stage].add_weight_cfg_list[index].seq;
}

int RandActivityConfig::GetMiJingXunBao3TaoGold(int type)
{
	if (type < RA_MIJINGXUNBAO3_CHOU_TYPE_1 || type >= RA_MIJINGXUNBAO3_CHOU_TYPE_MAX)
	{
		return 0;
	}

	switch (type)
	{
	case RA_MIJINGXUNBAO3_CHOU_TYPE_1:
	{
		return m_mijingxunbao3_once_gold;
	}
	break;

	case RA_MIJINGXUNBAO3_CHOU_TYPE_10:
	{
		return m_mijingxunbao3_tentimes_gold;
	}
	break;

	case RA_MIJINGXUNBAO3_CHOU_TYPE_30:
	{
		return m_mijingxunbao3_thirtytimes_gold;
	}
	break;

	default:
		return 0;
	}
}

bool RandActivityConfig::GetMiJingXunBao3RandomResult(MiJingXunBaoAddWeightInfo * self_weight, int length, int is_free, RandActivityMiJingXunBaoConfig * result_list, int result_list_length)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MIJINGXUNBAO3);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_mijingxunbao3_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return false;
	}

	if (NULL == self_weight || length < 0 || NULL == result_list || result_list_length <= 0)
	{
		return false;
	}

	int all_weight = 0;
	int all_cfg_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ + 1] = { 0 };

	for (int i = 1; i <= m_mijingxunbao3_max_seq_list[stage] && i <= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ; ++i)
	{
		all_cfg_weight_list[i] = m_mijingxunbao3_cfg_list[stage].cfg_list[i].weight;

		if (!is_free)	// 非免费刷新 增加自己权重
		{
			for (int self_index = 0; self_index < length; ++self_index)
			{
				if (self_weight[self_index].seq == m_mijingxunbao3_cfg_list[stage].cfg_list[i].seq)
				{
					all_cfg_weight_list[i] += self_weight[self_index].weight;
				}
			}
		}

		all_weight += all_cfg_weight_list[i];
	}


	if (all_weight <= 0)
	{
		return false;
	}

	RandActivityMiJingXunBao3 *mijing_xunbao3_activity = RandActivityManager::Instance().GetRandActivityMiJingXunBao3();
	if (nullptr == mijing_xunbao3_activity)
	{
		return false;
	}

	for (int result_index = 0; result_index < result_list_length && all_weight > 0; ++result_index)
	{
		if (!is_free)
		{
			mijing_xunbao3_activity->AddServerChouTimes(1);
		}

		int server_chou_times = mijing_xunbao3_activity->GetServerChouTimes();
		// 如果没有保底奖励才进行随机奖励
		const RandActivityServerBaodiConfig *baodi_cfg = this->GetMiJingXunBao3BaodiRandreward(server_chou_times);
		if (nullptr != baodi_cfg)
		{
			mijing_xunbao3_activity->ClearServerChouTimes();
			result_list[result_index].reward_item = baodi_cfg->reward_item;
			result_list[result_index].is_rare = baodi_cfg->is_rare;
			result_list[result_index].seq = baodi_cfg->seq;
			result_list[result_index].weight = 0;
			continue;
		}
		
		int rand_weight = RandomNum(all_weight);
		for (int i = 1; i <= m_mijingxunbao3_max_seq_list[stage] && i <= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ; ++i)
		{
			if (rand_weight < all_cfg_weight_list[i])
			{
				result_list[result_index] = m_mijingxunbao3_cfg_list[stage].cfg_list[i];

				for (int self_index = 0; self_index < length; ++self_index)
				{
					if (result_list[result_index].seq == self_weight[self_index].seq) //1 抽到珍稀，把该奖励权重置0
					{
						all_cfg_weight_list[self_weight[self_index].seq] -= self_weight[self_index].weight;
						all_weight -= self_weight[self_index].weight;
						if (all_weight <= 0)
						{
							return false;
						}

						self_weight[self_index].weight = 0;
						self_weight[self_index].tao_none_rare_times = 0;
					}
					else
					{
						self_weight[self_index].tao_none_rare_times++;
						int add_weight = this->GetMiJingXunBao3CfgAddWeight(self_weight[self_index].seq);

						if (self_weight[self_index].tao_none_rare_times >= this->GetMiJingXunBao3AddWeightTaoTimes())
						{
							all_weight += add_weight;

							self_weight[self_index].weight += add_weight;
							all_cfg_weight_list[self_weight[self_index].seq] += add_weight;

							self_weight[self_index].tao_none_rare_times -= this->GetMiJingXunBao3AddWeightTaoTimes();
						}
					}
				}//for self_index
				
				break;
			}
			rand_weight -= all_cfg_weight_list[i];
		}

	}

	return true;
}

const RandActivityMiJingXunBaoRewardConfig * RandActivityConfig::GetRandActivityMiJingXunBao3PersonalRewardCfg(int seq, int chou_times)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MIJINGXUNBAO3);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_mijingxunbao3_reward_cfg_count; ++i)
	{
		if (m_mijingxunbao3_reward_list_cfg[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return nullptr;
	}

	RandActivityMiJingXunBaoRewardDayCfg &day_cfg = m_mijingxunbao3_reward_list_cfg[idx];

	if (seq < 0 || seq >= RAND_ACTIVITY_MIJINGXUNBAO_REWARD_MAX_COUNT || chou_times < 0)
	{
		return nullptr;
	}

	if (chou_times >= day_cfg.cfg_list[seq].choujiang_times && day_cfg.cfg_list[seq].choujiang_times != 0)
	{
		return &day_cfg.cfg_list[seq];
	}

	return nullptr;
}
const RandActivityMiJingXunBaoConfig * RandActivityConfig::GetMiJingXUnBao3BaodiRewardCfg(int server_chou_times)
{
	if (server_chou_times <= 0) return nullptr;

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MIJINGXUNBAO3);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_mijingxunbao3_cfg_list[i].opengame_day)
		{
			continue;
		}

		RandActivityMiJingXunBaoCfgList &cur_stage_cfg = m_mijingxunbao3_cfg_list[i];

		for (std::map<int, int>::iterator it = cur_stage_cfg.baodi_map.begin(); it != cur_stage_cfg.baodi_map.end(); ++it)
		{
			const int baodi_times = it->first;
			const int reward_seq = it->second;

			if (baodi_times <= 0 || 0 != server_chou_times % baodi_times) continue;

			if (reward_seq < 0 || reward_seq > m_mijingxunbao3_daily_cfg_max_count) continue;

			return &m_mijingxunbao3_cfg_list[i].cfg_list[reward_seq];
		}
		break;
	}

	return nullptr;
}

const RandActivityServerBaodiConfig * RandActivityConfig::GetMiJingXunBao3BaodiRandreward(int server_chou_times)
{
	if (server_chou_times <= 0) return nullptr;

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_MIJINGXUNBAO3);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day <= m_mijingxunbao3_baodi_cfg[i].opengame_day)
		{
			if (server_chou_times >= m_mijingxunbao3_baodi_cfg[i].baodi)
			{
				stage = i;
				break;
			}
			else
			{
				return NULL;
			}
		}
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	return gamecommon::RandListItem(m_mijingxunbao3_baodi_cfg[stage].server_baodi_vec);
}

int RandActivityConfig::GetHuanLeZaDanAutoWeightAddCount()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_ZADAN);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_huanlezadan_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	return m_huanlezadan_auto_add_cfg_count_list[stage];
}

int RandActivityConfig::GetHuanLeZaDanAddWeightSeq(int index)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_ZADAN);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_huanlezadan_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	if (index < 0 || index >= m_huanlezadan_auto_add_cfg_count_list[stage])
	{
		return 0;
	}

	return m_huanlezadan_add_weight_cfg_list[stage].add_weight_cfg_list[index].seq;
}

int RandActivityConfig::GetHuanLeZaDanCfgAddWeight(int seq)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_ZADAN);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_huanlezadan_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	if (seq <= 0 || seq > m_huanlezadan_max_seq_list[stage] || seq > RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ)
	{
		return 0;
	}

	for (int index = 0; index < m_huanlezadan_auto_add_cfg_count_list[stage]; index++)
	{
		if (m_huanlezadan_add_weight_cfg_list[stage].add_weight_cfg_list[index].seq == seq)
		{
			return m_huanlezadan_add_weight_cfg_list[stage].add_weight_cfg_list[index].add_weight;
		}
	}

	return 0;
}

int RandActivityConfig::GetHuanLeZaDanTaoGold(int type)
{
	if (type < RA_MIJINGXUNBAO3_CHOU_TYPE_1 || type >= RA_MIJINGXUNBAO3_CHOU_TYPE_MAX)
	{
		return 0;
	}

	switch (type)
	{
	case RA_MIJINGXUNBAO3_CHOU_TYPE_1:
	{
		return m_huanlezadan_once_gold;
	}
	break;

	case RA_MIJINGXUNBAO3_CHOU_TYPE_10:
	{
		return m_huanlezadan_tentimes_gold;
	}
	break;

	case RA_MIJINGXUNBAO3_CHOU_TYPE_30:
	{
		return m_huanlezadan_thirtytimes_gold;
	}
	break;

	default:
		return 0;
	}
}

bool RandActivityConfig::GetHuanLeZaDanRandomResult(MiJingXunBaoAddWeightInfo * self_weight, int length, int is_free, RandActivityMiJingXunBaoConfig * result_list, int result_list_length)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_ZADAN);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_huanlezadan_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return false;
	}

	if (NULL == self_weight || length < 0 || NULL == result_list || result_list_length <= 0)
	{
		return false;
	}

	int all_weight = 0;
	int all_cfg_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ + 1] = { 0 };

	for (int i = 1; i <= m_huanlezadan_max_seq_list[stage] && i <= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ; ++i)
	{
		all_cfg_weight_list[i] = m_huanlezadan_cfg_list[stage].cfg_list[i].weight;

		if (!is_free)	// 非免费刷新 增加自己权重
		{
			for (int self_index = 0; self_index < length; ++self_index)
			{
				if (self_weight[self_index].seq == m_huanlezadan_cfg_list[stage].cfg_list[i].seq)
				{
					all_cfg_weight_list[i] += self_weight[self_index].weight;
				}
			}
		}

		all_weight += all_cfg_weight_list[i];
	}

	if (all_weight <= 0)
	{
		return false;
	}

	RandActivityHuanLeZaDan *huanle_zadan_activity = RandActivityManager::Instance().GetRandActivityHuanLeZaDan();
	if (nullptr == huanle_zadan_activity)
	{
		return false;
	}

	for (int result_index = 0; result_index < result_list_length && all_weight > 0; ++result_index)
	{
		int rand_weight = RandomNum(all_weight);
		for (int i = 1; i <= m_huanlezadan_max_seq_list[stage] && i <= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ; ++i)
		{
			if (rand_weight < all_cfg_weight_list[i])
			{
				int server_chou_times = huanle_zadan_activity->GetServerChouTimes();

				// 如果没有保底奖励才进行随机奖励
				const RandActivityMiJingXunBaoConfig *lottery_cfg = this->GetHuanLeZaDanBaodiRewardCfg(server_chou_times);
				if (nullptr != lottery_cfg)
				{
					huanle_zadan_activity->ClearServerChouTimes();
					result_list[result_index] = *lottery_cfg;
				}
				else
				{
					result_list[result_index] = m_huanlezadan_cfg_list[stage].cfg_list[i];
				}

				for (int self_index = 0; self_index < length; ++self_index)
				{
					if (result_list[result_index].seq == self_weight[self_index].seq) //1 抽到珍稀，把该奖励权重置0
					{
						all_cfg_weight_list[self_weight[self_index].seq] -= self_weight[self_index].weight;
						all_weight -= self_weight[self_index].weight;
						if (all_weight <= 0)
						{
							return false;
						}

						self_weight[self_index].weight = 0;
						self_weight[self_index].tao_none_rare_times = 0;
					}
					else
					{
						self_weight[self_index].tao_none_rare_times++;
						int add_weight = this->GetHuanLeZaDanCfgAddWeight(self_weight[self_index].seq);

						if (self_weight[self_index].tao_none_rare_times >= this->GetHuanLeZaDanAddWeightTaoTimes())
						{
							all_weight += add_weight;

							self_weight[self_index].weight += add_weight;
							all_cfg_weight_list[self_weight[self_index].seq] += self_weight[self_index].weight;

							self_weight[self_index].tao_none_rare_times -= this->GetHuanLeZaDanAddWeightTaoTimes();
						}
					}
				}
				break;
			}

			rand_weight -= all_cfg_weight_list[i];
		}

		huanle_zadan_activity->AddServerChouTimes(1);
	}

	return true;
}

const RandActivityMiJingXunBaoRewardConfig * RandActivityConfig::GetRandActivityHuanLeZaDanPersonalRewardCfg(int seq, int chou_times)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_ZADAN);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_huanlezadan_reward_cfg_count; ++i)
	{
		if (m_huanlezadan_reward_list_cfg[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return nullptr;
	}

	RandActivityMiJingXunBaoRewardDayCfg &day_cfg = m_huanlezadan_reward_list_cfg[idx];

	if (seq < 0 || seq >= RAND_ACTIVITY_MIJINGXUNBAO_REWARD_MAX_COUNT || chou_times < 0)
	{
		return nullptr;
	}

	if (chou_times >= day_cfg.cfg_list[seq].choujiang_times && day_cfg.cfg_list[seq].choujiang_times != 0)
	{
		return &day_cfg.cfg_list[seq];
	}

	return nullptr;
}

int RandActivityConfig::GetHuanLeYaoJiangAutoWeightAddCount()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_huanleyaojiang_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	return m_huanleyaojiang_auto_add_cfg_count_list[stage];
}

int RandActivityConfig::GetHuanLeYaoJiangAddWeightSeq(int index)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_huanleyaojiang_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	if (index < 0 || index >= m_huanleyaojiang_auto_add_cfg_count_list[stage])
	{
		return 0;
	}

	return m_huanleyaojiang_add_weight_cfg_list[stage].add_weight_cfg_list[index].seq;
}

int RandActivityConfig::GetHuanLeYaoJiangCfgAddWeight(int seq)
{
	if (seq <= 0 || seq >RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ)
	{
		return 0;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_huanleyaojiang_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	if (seq > m_huanleyaojiang_max_seq_list[stage])
	{
		return 0;
	}

	for (int index = 0; index < m_huanleyaojiang_auto_add_cfg_count_list[stage]; index++)
	{
		if (m_huanleyaojiang_add_weight_cfg_list[stage].add_weight_cfg_list[index].seq == seq)
		{
			return m_huanleyaojiang_add_weight_cfg_list[stage].add_weight_cfg_list[index].add_weight;
		}
	}

	return 0;
}

int RandActivityConfig::GetHuanLeYaoJiangTaoGold(int type)
{
	if (type < RA_MIJINGXUNBAO3_CHOU_TYPE_1 || type >= RA_MIJINGXUNBAO3_CHOU_TYPE_MAX)
	{
		return 0;
	}

	switch (type)
	{
	case RA_MIJINGXUNBAO3_CHOU_TYPE_1:
	{
		return m_huanleyaojiang_once_gold;
	}
	break;

	case RA_MIJINGXUNBAO3_CHOU_TYPE_10:
	{
		return m_huanleyaojiang_tentimes_gold;
	}
	break;

	case RA_MIJINGXUNBAO3_CHOU_TYPE_30:
	{
		return m_huanleyaojiang_thirtytimes_gold;
	}
	break;

	default:
		return 0;
	}
}

bool RandActivityConfig::GetHuanLeYaoJiangRandomResult(MiJingXunBaoAddWeightInfo * self_weight, int length, int is_free, RandActivityMiJingXunBaoConfig * result_list, int result_list_length)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_huanleyaojiang_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return false;
	}

	if (NULL == self_weight || length < 0 || NULL == result_list || result_list_length <= 0)
	{
		return false;
	}

	int all_weight = 0;
	int all_cfg_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ + 1] = { 0 };

	for (int i = 1; i <= m_huanleyaojiang_max_seq_list[stage] && i <= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ; ++i)
	{
		all_cfg_weight_list[i] = m_huanleyaojiang_cfg_list[stage].cfg_list[i].weight;

		if (!is_free)	// 非免费刷新 增加自己权重
		{
			for (int self_index = 0; self_index < length; ++self_index)
			{
				if (self_weight[self_index].seq == m_huanleyaojiang_cfg_list[stage].cfg_list[i].seq)
				{
					all_cfg_weight_list[i] += self_weight[self_index].weight;
				}
			}
		}
		
		all_weight += all_cfg_weight_list[i];
	}
	
	if (all_weight <= 0)
	{
		return false;
	}

	RandActivityHuanLeYaoJiang *huanle_yaojiang_activity = RandActivityManager::Instance().GetRandActivityHuanLeYaoJiang();
	if (nullptr == huanle_yaojiang_activity)
	{
		return false;
	}

	for (int result_index = 0; result_index < result_list_length && all_weight > 0; ++result_index)
	{
		if (!is_free)
		{
			huanle_yaojiang_activity->AddServerChouTimes(1);
		}

		// 如果没有保底奖励才进行随机奖励
		int server_chou_times = huanle_yaojiang_activity->GetServerChouTimes();

		const RandActivityServerBaodiConfig * baodi_config = this->GetHuanLeYaoJiangBaodiRandReward(server_chou_times);
		if (nullptr != baodi_config)
		{
			huanle_yaojiang_activity->ClearServerChouTimes();
			result_list[result_index].reward_item = baodi_config->reward_item;
			result_list[result_index].seq = baodi_config->seq;
			result_list[result_index].is_rare = baodi_config->is_rare;
			result_list[result_index].auto_add_weight = 0;
			continue;
		}
		
		int rand_weight = RandomNum(all_weight);
		for (int i = 1; i <= m_huanleyaojiang_max_seq_list[stage] && i <= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ; ++i)
		{
			if (rand_weight < all_cfg_weight_list[i])
			{
				result_list[result_index] = m_huanleyaojiang_cfg_list[stage].cfg_list[i];

				for (int self_index = 0; self_index < length; ++self_index)
				{
					if (result_list[result_index].seq == self_weight[self_index].seq) //1 抽到珍稀，把该奖励权重置0
					{
						all_cfg_weight_list[self_weight[self_index].seq] -= self_weight[self_index].weight;
						all_weight -= self_weight[self_index].weight;
						if (all_weight <= 0)
						{
							return false;
						}

						self_weight[self_index].weight = 0;
						self_weight[self_index].tao_none_rare_times = 0;
					}
					else
					{
						self_weight[self_index].tao_none_rare_times++;
						int add_weight = this->GetHuanLeYaoJiangCfgAddWeight(self_weight[self_index].seq);

						if (self_weight[self_index].tao_none_rare_times >= this->GetHuanLeYaoJiangAddWeightTaoTimes())
						{
							all_weight += add_weight;

							self_weight[self_index].weight += add_weight;
							all_cfg_weight_list[self_weight[self_index].seq] += add_weight;

							self_weight[self_index].tao_none_rare_times -= this->GetHuanLeYaoJiangAddWeightTaoTimes();
						}
					}
				}

				break;
			}

			rand_weight -= all_cfg_weight_list[i];
		}

	}

	return true;
}

const RandActivityMiJingXunBaoRewardConfig * RandActivityConfig::GetRandActivityHuanLeYaoJiangPersonalRewardCfg(int seq, int chou_times)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_huanleyaojiang_reward_cfg_count; ++i)
	{
		if (m_huanleyaojiang_reward_list_cfg[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return nullptr;
	}

	RandActivityMiJingXunBaoRewardDayCfg &day_cfg = m_huanleyaojiang_reward_list_cfg[idx];

	if (seq < 0 || seq >= RAND_ACTIVITY_MIJINGXUNBAO_REWARD_MAX_COUNT || chou_times < 0)
	{
		return nullptr;
	}

	if (chou_times >= day_cfg.cfg_list[seq].choujiang_times && day_cfg.cfg_list[seq].choujiang_times != 0)
	{
		return &day_cfg.cfg_list[seq];
	}

	return nullptr;
}

const RandActivityMiJingXunBaoConfig * RandActivityConfig::GetHuanLeYaoJiangBaodiRewardCfg(int server_chou_times)
{
	if (server_chou_times <= 0) return nullptr;

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_huanleyaojiang_cfg_list[i].opengame_day)
		{
			continue;
		}

		RandActivityMiJingXunBaoCfgList &cur_stage_cfg = m_huanleyaojiang_cfg_list[i];

		for (std::map<int, int>::iterator it = cur_stage_cfg.baodi_map.begin(); it != cur_stage_cfg.baodi_map.end(); ++it)
		{
			const int baodi_times = it->first;
			const int reward_seq = it->second;

			if (baodi_times <= 0 || 0 != server_chou_times % baodi_times) continue;

			if (reward_seq < 0 || reward_seq > m_huanleyaojiang_stage_cfg_max_count) continue;

			return &m_huanleyaojiang_cfg_list[i].cfg_list[reward_seq];
		}
		break;
	}

	return nullptr;
}

const RandActivityServerBaodiConfig * RandActivityConfig::GetHuanLeYaoJiangBaodiRandReward(int server_chou_times)
{
	if (server_chou_times <= 0) return nullptr;

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day <= m_huanleyaojiang_baodi_cfg[i].opengame_day)
		{
			if (server_chou_times >= m_huanleyaojiang_baodi_cfg[i].baodi)
			{
				stage = i;
				break;
			}
			else
			{
				return NULL;
			}
		}
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return NULL;
	}

	return gamecommon::RandListItem(m_huanleyaojiang_baodi_cfg[stage].server_baodi_vec);
}

const RandActivityMiJingXunBaoConfig * RandActivityConfig::GetHuanLeZaDanBaodiRewardCfg(int server_chou_times)
{
	if (server_chou_times <= 0) return nullptr;

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_ZADAN);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_huanlezadan_cfg_list[i].opengame_day)
		{
			continue;
		}

		RandActivityMiJingXunBaoCfgList &cur_stage_cfg = m_huanlezadan_cfg_list[i];

		for (std::map<int, int>::iterator it = cur_stage_cfg.baodi_map.begin(); it != cur_stage_cfg.baodi_map.end(); ++it)
		{
			const int baodi_times = it->first;
			const int reward_seq = it->second;

			if (baodi_times <= 0 || 0 != server_chou_times % baodi_times) continue;

			if (reward_seq < 0 || reward_seq > m_huanlezadan_daily_cfg_max_count) continue;

			return &m_huanlezadan_cfg_list[i].cfg_list[reward_seq];
		}
		break;
	}

	return nullptr;
}

int RandActivityConfig::InitHappyCumulChongzhiConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int act_type = 0;
	int last_act_type = 0;
	int seq = 0;
	int last_seq = -1;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "act_type", act_type) || act_type <= JINJIE_ACT_THEME_TYPE_INVALID || act_type >= JINJIE_ACT_THEME_TYPE_MAX)
		{
			return -1;
		}

		if (act_type != last_act_type)
		{
			last_seq = -1;
			last_act_type = act_type;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= RAND_ACTIVITY_HAPPY_CUMUL_CHONGZHI_MAX_CFG || seq != last_seq + 1)
		{
			return -2;
		}

		last_seq = seq;

		HappyCumulChongzhiConfig &cfg = m_happy_cumul_chongzhi_cfg[act_type][seq];
		cfg.act_type = act_type;
		cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "need_chongzhi", cfg.need_chongzhi) || cfg.need_chongzhi <= 0)
		{
			return -3;
		}

		{
			cfg.reward_count = 0;
			PugiXmlNode item_list_element = data_element.child("reward_item_list");
			if (item_list_element.empty())
			{
				return -4;
			}

			PugiXmlNode item_element = item_list_element.child("reward_item");
			if (item_element.empty())
			{
				return -5;
			}

			while (!item_element.empty())
			{
				if (cfg.reward_count >= RAND_ACTIVITY_HAPPY_CUMUL_CHONGZHI_MAX_REWARD_COUNT)
				{
					return -6;
				}

				if (!cfg.reward_item_list[cfg.reward_count].ReadConfig(item_element))
				{
					return -7;
				}

				++cfg.reward_count;
				item_element = item_element.next_sibling();
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const HappyCumulChongzhiConfig *RandActivityConfig::GetHappyCumulChongzhiCfg(int theme, int seq)
{
	if ((theme <= JINJIE_ACT_THEME_TYPE_INVALID || theme >= JINJIE_ACT_THEME_TYPE_MAX) || (seq < 0 || seq >= RAND_ACTIVITY_HAPPY_CUMUL_CHONGZHI_MAX_CFG))
	{
		return nullptr;
	}

	return &m_happy_cumul_chongzhi_cfg[theme][seq];
}

int RandActivityConfig::InitJinJieReturnConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int act_type = 0;
	int last_act_type = 0;
	int seq = 0;
	int last_seq = -1;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "act_type", act_type) || act_type <= JINJIE_ACT_THEME_TYPE_INVALID || act_type >= JINJIE_ACT_THEME_TYPE_MAX)
		{
			return -1;
		}

		if (act_type != last_act_type)
		{
			last_seq = -1;
			last_act_type = act_type;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= RAND_ACTIVITY_JINJIE_RETURN_MAX_CFG || seq != last_seq + 1)
		{
			return -2;
		}

		last_seq = seq;

		JinJieReturnConfig & cfg = m_jinjie_return_cfg[act_type][seq];

		if (!PugiGetSubNodeValue(data_element, "need_grade", cfg.need_grade) || cfg.need_grade <= 0)
		{
			return -3;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -4;
		}

		if (!cfg.reward_item.ReadConfig(item_element))
		{
			return -5;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitConsumeForGiftCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_opengame_day = 0;
	int opengame_day_count = 0;

	while (!data_element.empty())
	{
		RandActivityConsumeForGiftItemConfig cfg;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", cfg.opengame_day) || cfg.opengame_day <= 0)
		{
			return -1;
		}

		if (cfg.opengame_day != last_opengame_day)
		{
			++opengame_day_count;
			m_consume_for_gift_item_count_map[cfg.opengame_day] = 0;
		}
		last_opengame_day = cfg.opengame_day;

		if (opengame_day_count < 0 || opengame_day_count >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", cfg.seq) || cfg.seq < 0)
		{
			return -3;
		}

		PugiXmlNode exchange_item = data_element.child("exchange_item");
		if (exchange_item.empty())
		{
			return -4;
		}

		if (!cfg.exchange_item.ReadConfig(exchange_item) || cfg.exchange_item.item_id <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "need_points", cfg.need_points) || cfg.need_points <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "double_points_need_ex_times", cfg.double_points_need_ex_times) || cfg.double_points_need_ex_times < 0 || cfg.double_points_need_ex_times >= UCHAR_MAX)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "points_per_gold", cfg.point_per_gold) || cfg.point_per_gold < 0)
		{
			return -8;
		}

		++m_consume_for_gift_item_count_map[cfg.opengame_day];
		m_consume_for_gift_item_cfg_list.push_back(cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitMiJingXunBao3Cfg(PugiXmlNode  RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int is_rare_temp = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (last_opengame_day != opengame_day)
		{
			last_opengame_day = opengame_day;
			++stage;
			m_mijingxunbao3_daily_cfg_max_count = 0;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (m_mijingxunbao3_max_seq_list[stage] >= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ)
		{
			return -1;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_mijingxunbao3_max_seq_list[stage] + 1)
		{
			return -11;
		}

		m_mijingxunbao3_cfg_list[stage].opengame_day = opengame_day;
		RandActivityMiJingXunBaoConfig &mijingxunbao3_cfg = m_mijingxunbao3_cfg_list[stage].cfg_list[seq];
		mijingxunbao3_cfg.seq = seq;
		m_mijingxunbao3_max_seq_list[stage] = seq;

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -12;
		}

		if (!mijingxunbao3_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -13;
		}

		is_rare_temp = 0;
		if (!PugiGetSubNodeValue(data_element, "is_rare", is_rare_temp) || is_rare_temp < 0 || is_rare_temp > 1)
		{
			return -14;
		}
		mijingxunbao3_cfg.is_rare = (0 != is_rare_temp);

		if (!PugiGetSubNodeValue(data_element, "weight", mijingxunbao3_cfg.weight) || mijingxunbao3_cfg.weight < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(data_element, "auto_add_weight", mijingxunbao3_cfg.auto_add_weight) || mijingxunbao3_cfg.auto_add_weight < 0)
		{
			return -16;
		}

		if (mijingxunbao3_cfg.auto_add_weight > 0)
		{
			if (m_mijingxunbao3_auto_add_cfg_count_list[stage] >= RAND_ACTIVITY_MIJINGXUNBAO_AUTO_ADD_WEIGHT_CFG_COUNT)
			{
				return -18;
			}

			m_mijingxunbao3_add_weight_cfg_list[stage].opengame_day = opengame_day;
			m_mijingxunbao3_add_weight_cfg_list[stage].add_weight_cfg_list[m_mijingxunbao3_auto_add_cfg_count_list[stage]].add_weight = mijingxunbao3_cfg.auto_add_weight;
			m_mijingxunbao3_add_weight_cfg_list[stage].add_weight_cfg_list[m_mijingxunbao3_auto_add_cfg_count_list[stage]].seq = mijingxunbao3_cfg.seq;

			++m_mijingxunbao3_auto_add_cfg_count_list[stage];
		}

// 		int baodi = 0;
// 		if (PugiGetSubNodeValue(data_element, "baodi", baodi) && baodi > 0)
// 		{
// 			// 保底次数必须不同,因为保底必出物品（除非保底物品也随机）
// 			if (m_mijingxunbao3_cfg_list[stage].baodi_map.find(baodi) != m_mijingxunbao3_cfg_list[stage].baodi_map.end()
// 				|| m_mijingxunbao3_cfg_list[stage].baodi_map.size() > 0)
// 			{
// 				return -9;
// 			}
// 
// 			m_mijingxunbao3_cfg_list[stage].baodi_map.insert(std::make_pair(baodi, seq));
// 		}

		m_mijingxunbao3_daily_cfg_max_count++;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitMiJingXunBao3RewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int max_index = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			stage++;
			last_opengame_day = opengame_day;
			m_mijingxunbao3_reward_cfg_count = stage + 1;
			max_index = -1;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		int index = 0;
		if (!PugiGetSubNodeValue(data_element, "index", index) || index != max_index + 1 || index >= RAND_ACTIVITY_MIJINGXUNBAO_REWARD_MAX_COUNT)
		{
			return -1;
		}

		RandActivityMiJingXunBaoRewardDayCfg &day_cfg = m_mijingxunbao3_reward_list_cfg[stage];
		++day_cfg.cfg_count;
		day_cfg.opengame_day = opengame_day;

		RandActivityMiJingXunBaoRewardConfig &reward_list_cfg = day_cfg.cfg_list[index];
		reward_list_cfg.index = index;
		max_index = index;

		if (!PugiGetSubNodeValue(data_element, "choujiang_times", reward_list_cfg.choujiang_times) || reward_list_cfg.choujiang_times < 0)
		{
			return -2;
		}

		{
			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -3;
			}

			if (!reward_list_cfg.reward_item.ReadConfig(reward_item_element))
			{
				return -4;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitMiJingXunBao3BaodiCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100;
	}

	int last_opengame_day = -1;
	int stage = -1;
	int last_seq = 0;
	while (!data_element.empty())
	{
		int baodi = 0;
		if (!PugiGetSubNodeValue(data_element, "baodi", baodi) || baodi <= 0)
		{
			data_element = data_element.next_sibling();
			continue;
		}

		int open_game_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", open_game_day) || open_game_day <= 0)
		{
			return -1;
		}

		if (open_game_day != last_opengame_day)
		{
			last_opengame_day = open_game_day;
			stage++;
			last_seq = 0;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -100;
		}

		RandActivityServerBaodiCfgList & baodi_cfg = m_mijingxunbao3_baodi_cfg[stage];

		if (baodi_cfg.opengame_day != 0 && baodi_cfg.opengame_day != open_game_day)
		{
			return -101;
		}

		if (baodi_cfg.baodi != 0 && baodi_cfg.baodi != baodi)
		{
			return -102;
		}

		baodi_cfg.opengame_day = open_game_day;
		baodi_cfg.baodi = baodi;

		RandActivityServerBaodiConfig baodi_item;

		if (!PugiGetSubNodeValue(data_element, "seq", baodi_item.seq) || baodi_item.seq < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "baodi_weight", baodi_item.weight) || baodi_item.weight < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "is_rare", baodi_item.is_rare))
		{
			return -5;
		}

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -6;
		}

		if (!baodi_item.reward_item.ReadConfig(reward_item_element))
		{
			return -7;
		}

		if (last_seq != 0 && last_seq + 1 != baodi_item.seq)
		{
			return -103;
		}
		last_seq = baodi_item.seq;

		baodi_cfg.server_baodi_vec.push_back(baodi_item);
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitHuanLeZaDanCfg(PugiXmlNode  RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int is_rare_temp = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (last_opengame_day != opengame_day)
		{
			last_opengame_day = opengame_day;
			++stage;
			m_huanlezadan_daily_cfg_max_count = 0;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (m_huanlezadan_max_seq_list[stage] >= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ)
		{
			return -1;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_huanlezadan_max_seq_list[stage] + 1)
		{
			return -11;
		}

		m_huanlezadan_cfg_list[stage].opengame_day = opengame_day;
		RandActivityMiJingXunBaoConfig &huanlezadan_cfg = m_huanlezadan_cfg_list[stage].cfg_list[seq];
		huanlezadan_cfg.seq = seq;
		m_huanlezadan_max_seq_list[stage] = seq;

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -12;
		}

		if (!huanlezadan_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -13;
		}

		is_rare_temp = 0;
		if (!PugiGetSubNodeValue(data_element, "is_rare", is_rare_temp) || is_rare_temp < 0 || is_rare_temp > 1)
		{
			return -14;
		}
		huanlezadan_cfg.is_rare = (0 != is_rare_temp);

		if (!PugiGetSubNodeValue(data_element, "weight", huanlezadan_cfg.weight) || huanlezadan_cfg.weight < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(data_element, "auto_add_weight", huanlezadan_cfg.auto_add_weight) || huanlezadan_cfg.auto_add_weight < 0)
		{
			return -16;
		}

		if (huanlezadan_cfg.auto_add_weight > 0)
		{
			if (m_huanlezadan_auto_add_cfg_count_list[stage] >= RAND_ACTIVITY_MIJINGXUNBAO_AUTO_ADD_WEIGHT_CFG_COUNT)
			{
				return -18;
			}

			m_huanlezadan_add_weight_cfg_list[stage].opengame_day = opengame_day;
			m_huanlezadan_add_weight_cfg_list[stage].add_weight_cfg_list[m_huanlezadan_auto_add_cfg_count_list[stage]].add_weight = huanlezadan_cfg.auto_add_weight;
			m_huanlezadan_add_weight_cfg_list[stage].add_weight_cfg_list[m_huanlezadan_auto_add_cfg_count_list[stage]].seq = huanlezadan_cfg.seq;

			++m_huanlezadan_auto_add_cfg_count_list[stage];
		}

		int baodi = 0;
		if (PugiGetSubNodeValue(data_element, "baodi", baodi) && baodi > 0)
		{
			// 保底次数必须不同,因为保底必出物品（除非保底物品也随机）
			if (m_huanlezadan_cfg_list[stage].baodi_map.find(baodi) != m_huanlezadan_cfg_list[stage].baodi_map.end()
				|| m_huanlezadan_cfg_list[stage].baodi_map.size() > 0)
			{
				return -9;
			}

			m_huanlezadan_cfg_list[stage].baodi_map.insert(std::make_pair(baodi, seq));
		}

		++m_huanlezadan_daily_cfg_max_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitHuanLeZaDanRewardCfg(PugiXmlNode  RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int max_index = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			stage++;
			last_opengame_day = opengame_day;
			m_huanlezadan_reward_cfg_count = stage + 1;
			max_index = -1;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		int index = 0;
		if (!PugiGetSubNodeValue(data_element, "index", index) || index != max_index + 1 || index >= RAND_ACTIVITY_MIJINGXUNBAO_REWARD_MAX_COUNT)
		{
			return -1;
		}

		RandActivityMiJingXunBaoRewardDayCfg &day_cfg = m_huanlezadan_reward_list_cfg[stage];
		++day_cfg.cfg_count;
		day_cfg.opengame_day = opengame_day;

		RandActivityMiJingXunBaoRewardConfig &reward_list_cfg = day_cfg.cfg_list[index];
		reward_list_cfg.index = index;
		max_index = index;

		if (!PugiGetSubNodeValue(data_element, "choujiang_times", reward_list_cfg.choujiang_times) || reward_list_cfg.choujiang_times < 0)
		{
			return -2;
		}

		{
			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -3;
			}

			if (!reward_list_cfg.reward_item.ReadConfig(reward_item_element))
			{
				return -4;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitHuanLeYaoJiangCfg(PugiXmlNode  RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int is_rare_temp = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (last_opengame_day != opengame_day)
		{
			last_opengame_day = opengame_day;
			++stage;
			m_huanleyaojiang_stage_cfg_max_count = 0;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (m_huanleyaojiang_max_seq_list[stage] >= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ)
		{
			return -1;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_huanleyaojiang_max_seq_list[stage] + 1)
		{
			return -11;
		}

		m_huanleyaojiang_cfg_list[stage].opengame_day = opengame_day;
		RandActivityMiJingXunBaoConfig &huanleyaojaing_cfg = m_huanleyaojiang_cfg_list[stage].cfg_list[seq];
		huanleyaojaing_cfg.seq = seq;
		m_huanleyaojiang_max_seq_list[stage] = seq;

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -12;
		}

		if (!huanleyaojaing_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -13;
		}

		is_rare_temp = 0;
		if (!PugiGetSubNodeValue(data_element, "is_rare", is_rare_temp) || is_rare_temp < 0 || is_rare_temp > 1)
		{
			return -14;
		}
		huanleyaojaing_cfg.is_rare = (0 != is_rare_temp);

		if (!PugiGetSubNodeValue(data_element, "weight", huanleyaojaing_cfg.weight) || huanleyaojaing_cfg.weight < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(data_element, "auto_add_weight", huanleyaojaing_cfg.auto_add_weight) || huanleyaojaing_cfg.auto_add_weight < 0)
		{
			return -16;
		}

		if (huanleyaojaing_cfg.auto_add_weight > 0)
		{
			if (m_huanleyaojiang_auto_add_cfg_count_list[stage] >= RAND_ACTIVITY_MIJINGXUNBAO_AUTO_ADD_WEIGHT_CFG_COUNT)
			{
				return -18;
			}

			m_huanleyaojiang_add_weight_cfg_list[stage].opengame_day = opengame_day;
			m_huanleyaojiang_add_weight_cfg_list[stage].add_weight_cfg_list[m_huanleyaojiang_auto_add_cfg_count_list[stage]].add_weight = huanleyaojaing_cfg.auto_add_weight;
			m_huanleyaojiang_add_weight_cfg_list[stage].add_weight_cfg_list[m_huanleyaojiang_auto_add_cfg_count_list[stage]].seq = huanleyaojaing_cfg.seq;

			++m_huanleyaojiang_auto_add_cfg_count_list[stage];
		}

// 		int baodi = 0;
// 		if (PugiGetSubNodeValue(data_element, "baodi", baodi) && baodi > 0)
// 		{
// 			// 保底次数必须不同,因为保底必出物品（除非保底物品也随机）
// 			if (m_huanleyaojiang_cfg_list[stage].baodi_map.find(baodi) != m_huanleyaojiang_cfg_list[stage].baodi_map.end()
// 				|| m_huanleyaojiang_cfg_list[stage].baodi_map.size() > 0)
// 			{
// 				return -9;
// 			}
// 
// 			m_huanleyaojiang_cfg_list[stage].baodi_map.insert(std::make_pair(baodi, seq));
// 		}

		++m_huanleyaojiang_stage_cfg_max_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitHuanLeYaoJiangRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int max_index = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			stage++;
			last_opengame_day = opengame_day;
			m_huanleyaojiang_reward_cfg_count = stage + 1;
			max_index = -1;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		int index = 0;
		if (!PugiGetSubNodeValue(data_element, "index", index) || index != max_index + 1 || index >= RAND_ACTIVITY_MIJINGXUNBAO_REWARD_MAX_COUNT)
		{
			return -1;
		}

		RandActivityMiJingXunBaoRewardDayCfg &day_cfg = m_huanleyaojiang_reward_list_cfg[stage];
		++day_cfg.cfg_count;
		day_cfg.opengame_day = opengame_day;

		RandActivityMiJingXunBaoRewardConfig &reward_list_cfg = day_cfg.cfg_list[index];
		reward_list_cfg.index = index;
		max_index = index;

		if (!PugiGetSubNodeValue(data_element, "choujiang_times", reward_list_cfg.choujiang_times) || reward_list_cfg.choujiang_times < 0)
		{
			return -2;
		}

		{
			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -3;
			}

			if (!reward_list_cfg.reward_item.ReadConfig(reward_item_element))
			{
				return -4;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitHuanLeYaoJiangBaodiCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100;
	}

	int last_opengame_day = -1;
	int stage = -1;
	int last_seq = 0;
	while (!data_element.empty())
	{
		int baodi = 0;
		if (!PugiGetSubNodeValue(data_element, "baodi", baodi) || baodi <= 0)
		{
			data_element = data_element.next_sibling();
			continue;
		}

		int open_game_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", open_game_day) || open_game_day <= 0)
		{
			return -1;
		}

		if (open_game_day != last_opengame_day)
		{
			last_opengame_day = open_game_day;
			stage++;
			last_seq = 0;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -100;
		}

		RandActivityServerBaodiCfgList & baodi_cfg = m_huanleyaojiang_baodi_cfg[stage];

		if (baodi_cfg.opengame_day != 0 && baodi_cfg.opengame_day != open_game_day)
		{
			return -101;
		}

		if (baodi_cfg.baodi != 0 && baodi_cfg.baodi != baodi)
		{
			return -102;
		}

		baodi_cfg.opengame_day = open_game_day;
		baodi_cfg.baodi = baodi;

		RandActivityServerBaodiConfig baodi_item;

		if (!PugiGetSubNodeValue(data_element, "seq", baodi_item.seq) || baodi_item.seq < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "baodi_weight", baodi_item.weight) || baodi_item.weight < 0)
		{
			return -4;
		}
		
		if (!PugiGetSubNodeValue(data_element, "is_rare", baodi_item.is_rare))
		{
			return -5;
		}

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -6;
		}

		if (!baodi_item.reward_item.ReadConfig(reward_item_element))
		{
			return -7;
		}
		
		if (last_seq != 0 && last_seq + 1 != baodi_item.seq)
		{
			return -103;
		}

		last_seq = baodi_item.seq;

		baodi_cfg.server_baodi_vec.push_back(baodi_item);
		data_element = data_element.next_sibling();
	}

	return 0;
}

const JinJieReturnConfig *RandActivityConfig::GetJinJieReturnCfg(int theme, int seq)
{
	if ((theme <= JINJIE_ACT_THEME_TYPE_INVALID || theme >= JINJIE_ACT_THEME_TYPE_MAX) || (seq < 0 || seq >= RAND_ACTIVITY_JINJIE_RETURN_MAX_CFG))
	{
		return nullptr;
	}

	return &m_jinjie_return_cfg[theme][seq];
}

int RandActivityConfig::InitLimitTimeRebateCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_opengame_day = -1;
	int day_state_index = -1;
	int opengame_day = 0;
	int last_seq = -1;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -1;
		}

		if (opengame_day != last_opengame_day)
		{
			++day_state_index;
			last_seq = -1;
		}
		last_opengame_day = opengame_day;

		if (day_state_index < 0 || day_state_index >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -2;
		}

		RandActivityLimitTimeRebateCfg &cfg = m_limit_time_rebate_list[day_state_index];
		cfg.opengame_day = opengame_day;

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != last_seq + 1 || seq != cfg.cfg_count || seq >= RA_LIMIT_TIME_REBATE_MAX_CFG_COUNT)
		{
			return -3;
		}
		last_seq = seq;

		if (!PugiGetSubNodeValue(data_element, "chongzhi_count", cfg.cfg_list[cfg.cfg_count].chongzhi_cout) || cfg.cfg_list[cfg.cfg_count].chongzhi_cout <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "chongzhi_day", cfg.cfg_list[cfg.cfg_count].need_day) || cfg.cfg_list[cfg.cfg_count].need_day <= 0)
		{
			return -5;
		}

		{
			PugiXmlNode reward_item_element = data_element.child("reward");
			if (reward_item_element.empty())
			{
				return -6;
			}

			if (!cfg.cfg_list[cfg.cfg_count].reward_item.ReadConfig(reward_item_element))
			{
				return -7;
			}
		}

		cfg.cfg_count++;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityLimitTimeRebateCfg *RandActivityConfig::GetLimitTimeRebateCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_LIMIT_TIME_REBATE);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; i++)
	{
		if (m_limit_time_rebate_list[i].opengame_day >= opengame_day)
		{
			return &m_limit_time_rebate_list[i];
		}
	}

	return NULL;
}

const LimitTimeRebateItem *RandActivityConfig::GetLimitTimeRebateItemCfg(int seq)
{
	if (seq < 0 || seq >= RA_LIMIT_TIME_REBATE_MAX_CFG_COUNT)
	{
		return NULL;
	}

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_LIMIT_TIME_REBATE);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; i++)
	{
		if (m_limit_time_rebate_list[i].opengame_day >= opengame_day)
		{
			return &m_limit_time_rebate_list[i].cfg_list[seq];
		}
	}

	return NULL;
}

int RandActivityConfig::InitLimitTimeGiftCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int stage_count = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int last_limit_vip_level = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0 || opengame_day < last_opengame_day)
		{
			return -1;
		}

		if (opengame_day != last_opengame_day)
		{
			++stage_count;
			last_limit_vip_level = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_count < 0 || stage_count >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -100;
		}

		RandActivityLimitTimeGiftCfg &limit_time_gift_cfg = m_limit_time_gift_cfg_list[stage_count];
		limit_time_gift_cfg.opengame_day = opengame_day;

		if (limit_time_gift_cfg.sub_item_cfg_count < 0 || limit_time_gift_cfg.sub_item_cfg_count >= RAND_ACTIVITY_LIMIT_TIME_GIFT_MAX_SEQ)
		{
			return -3;
		}

		RandActivityLimitTimeGiftCfg::SubItemCfg &sub_cfg = limit_time_gift_cfg.sub_item_cfg_list[limit_time_gift_cfg.sub_item_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", sub_cfg.seq) || sub_cfg.seq != limit_time_gift_cfg.sub_item_cfg_count + 1
			|| sub_cfg.seq >= RAND_ACTIVITY_LIMIT_TIME_GIFT_MAX_SEQ)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "limit_time", sub_cfg.limit_time) || sub_cfg.limit_time <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "limit_vip_level", sub_cfg.limit_vip_level) || sub_cfg.limit_vip_level < 0 || sub_cfg.limit_vip_level < last_limit_vip_level)
		{
			return -5;
		}
		last_limit_vip_level = sub_cfg.limit_vip_level;

		if (!PugiGetSubNodeValue(data_element, "charge_value", sub_cfg.charge_value1) || sub_cfg.charge_value1 <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "gift_value", sub_cfg.gift_value1) || sub_cfg.gift_value1 <= 0)
		{
			return -7;
		}

		sub_cfg.item_count1 = ItemConfigData::ReadConfigList(data_element, "reward_item", sub_cfg.reward_item_list1, MAX_ATTACHMENT_ITEM_NUM);
		if (sub_cfg.item_count1 <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "charge_value2", sub_cfg.charge_value2) || sub_cfg.charge_value2 <= 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "gift_value2", sub_cfg.gift_value2) || sub_cfg.gift_value2 <= 0)
		{
			return -10;
		}

		sub_cfg.item_count2 = ItemConfigData::ReadConfigList(data_element, "reward_item2", sub_cfg.reward_item_list2, MAX_ATTACHMENT_ITEM_NUM);
		if (sub_cfg.item_count2 <= 0)
		{
			return -11;
		}
		
		if (!PugiGetSubNodeValue(data_element, "limit_charge_min", sub_cfg.limit_charge_min) || sub_cfg.limit_charge_min < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "limit_charge_max", sub_cfg.limit_charge_max) || sub_cfg.limit_charge_max < -1)
		{
			return -13;
		}

		//if (!PugiGetSubNodeValue(data_element, "charge_value3", sub_cfg.charge_value3) || sub_cfg.charge_value3 < 0)
		//{
		//	return -14;
		//}

		//if (!PugiGetSubNodeValue(data_element, "gift_value3", sub_cfg.gift_value3) || sub_cfg.gift_value3 < 0)
		//{
		//	return -15;
		//}

		//sub_cfg.item_count3 = ItemConfigData::ReadConfigList(data_element, "reward_item3", sub_cfg.reward_item_list3, MAX_ATTACHMENT_ITEM_NUM);
		//if (sub_cfg.item_count3 <= 0)
		//{
		//	return -16;
		//}

		++limit_time_gift_cfg.sub_item_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityLimitTimeGiftCfg::SubItemCfg * RandActivityConfig::GetLimitTimeGiftRewardCfg(int opengame_day, const int vip_level, long long recharge_gold)
{
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day <= m_limit_time_gift_cfg_list[i].opengame_day)
		{
			for (int j = 0; j < m_limit_time_gift_cfg_list[i].sub_item_cfg_count && j < RAND_ACTIVITY_LIMIT_TIME_GIFT_MAX_SEQ; ++j)
			{
				//填-1代表最大值不限
				if (m_limit_time_gift_cfg_list[i].sub_item_cfg_list[j].limit_charge_max == -1 &&
					vip_level <= m_limit_time_gift_cfg_list[i].sub_item_cfg_list[j].limit_vip_level &&
					recharge_gold >= m_limit_time_gift_cfg_list[i].sub_item_cfg_list[j].limit_charge_min)
				{
					return &m_limit_time_gift_cfg_list[i].sub_item_cfg_list[j];
				}
				else if (m_limit_time_gift_cfg_list[i].sub_item_cfg_list[j].limit_charge_max != -1 && 
					vip_level <= m_limit_time_gift_cfg_list[i].sub_item_cfg_list[j].limit_vip_level && 
					recharge_gold >= m_limit_time_gift_cfg_list[i].sub_item_cfg_list[j].limit_charge_min &&
					recharge_gold < m_limit_time_gift_cfg_list[i].sub_item_cfg_list[j].limit_charge_max)	//左闭右开
				{
					return &m_limit_time_gift_cfg_list[i].sub_item_cfg_list[j];
				}
			}
		}
	}

	return nullptr;
}

const RandActivityLimitTimeGiftCfg::SubItemCfg * RandActivityConfig::GetLimitTimeGiftRewardCfgBySeq(int opengame_day, const int seq)
{
	if (seq < 0 || seq > RAND_ACTIVITY_LIMIT_TIME_GIFT_MAX_SEQ) return nullptr;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day <= m_limit_time_gift_cfg_list[i].opengame_day)
		{
			for (int j = 0; j < m_limit_time_gift_cfg_list[i].sub_item_cfg_count && j <= RAND_ACTIVITY_LIMIT_TIME_GIFT_MAX_SEQ; ++j)
			{
				if (seq == m_limit_time_gift_cfg_list[i].sub_item_cfg_list[j].seq)
				{
					return &m_limit_time_gift_cfg_list[i].sub_item_cfg_list[j];
				}
			}
		}
	}

	return nullptr;
}

int RandActivityConfig::InitCirculationChongzhiSecondRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_level = 0;
	while (!dataElement.empty())
	{
		RandActivityCirculationChongzhiSecondRewardCfg reward_cfg;

		if (!PugiGetSubNodeValue(dataElement, "level", reward_cfg.level) || reward_cfg.level <= 0 || reward_cfg.level <= last_level)
		{
			return -2;
		}

		last_level = reward_cfg.level;

		{
			PugiXmlNode reward_item_element = dataElement.child("reward_item");
			if (reward_item_element.empty())
			{
				return -3;
			}

			if (!reward_cfg.reward_item.ReadConfig(reward_item_element))
			{
				return -4;
			}
		}

		m_circulation_chongzhi_2_reward_cfg_vec.push_back(reward_cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const ItemConfigData * RandActivityConfig::GetCirculationChongzhiSecondRewardItem(int level)
{
	for (auto it = m_circulation_chongzhi_2_reward_cfg_vec.begin(); it != m_circulation_chongzhi_2_reward_cfg_vec.end(); ++it)
	{
		if (level <= it->level)
		{
			return &(it->reward_item);
		}
	}

	return nullptr;
}

// 疯狂摇钱树
int RandActivityConfig::InitShakeMoneyCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int seq = 0;
	int last_seq = 0;
	int stage_num = -1;

	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "seq", seq) || seq <= 0)
		{
			return -2;
		}

		if (seq != last_seq)
		{
			++stage_num;
		}
		last_seq = seq;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -3;
		}

		RandActivityShakeMoneyConfig &config = m_shakemoney_cfg_list[stage_num];
		config.seq = seq;

		if (!PugiGetSubNodeValue(dataElement, "section_start", config.section_start) || config.section_start <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "section_end", config.section_end) || config.section_end < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "chongzhi_return", config.chongzhi_return) || config.chongzhi_return <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "return_max", config.return_max) || config.return_max <= 0)
		{
			return -7;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const RandActivityShakeMoneyConfig *RandActivityConfig::GetShakeMoneyCfg(int opengame_day)
{
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if ((opengame_day >= m_shakemoney_cfg_list[i].section_start && opengame_day <= m_shakemoney_cfg_list[i].section_end) || m_shakemoney_cfg_list[i].section_end == 0)
		{
			return &m_shakemoney_cfg_list[i];
		}
	}

	return NULL;
}

int RandActivityConfig::InitTimeLimitLuxuryGiftBag(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int stage_count = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int last_limit_vip_level = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0 || opengame_day < last_opengame_day)
		{
			return -1;
		}

		if (opengame_day != last_opengame_day)
		{
			++stage_count;
			last_limit_vip_level = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_count < 0 || stage_count >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -2;
		}

		RandActivityLimitTimeLuxuryGiftBagCfg &limit_time_gift_cfg = m_limit_time_luxury_gift_bag_cfg_list[stage_count];
		limit_time_gift_cfg.opengame_day = opengame_day;

		if (limit_time_gift_cfg.sub_item_cfg_count < 0 || limit_time_gift_cfg.sub_item_cfg_count >= RAND_ACTIVITY_LIMIT_TIME_GIFT_MAX_SEQ)
		{
			return -3;
		}

		RandActivityLimitTimeLuxuryGiftBagCfg::SubItemCfg &sub_cfg = limit_time_gift_cfg.sub_item_cfg_list[limit_time_gift_cfg.sub_item_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", sub_cfg.seq) || sub_cfg.seq != limit_time_gift_cfg.sub_item_cfg_count + 1
			|| sub_cfg.seq >= RAND_ACTIVITY_LIMIT_TIME_GIFT_MAX_SEQ)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "limit_time", sub_cfg.limit_time) || sub_cfg.limit_time <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "limit_vip_level", sub_cfg.limit_vip_level) || sub_cfg.limit_vip_level < 0 || sub_cfg.limit_vip_level < last_limit_vip_level)
		{
			return -6;
		}
		last_limit_vip_level = sub_cfg.limit_vip_level;

		if (!PugiGetSubNodeValue(data_element, "gift_value", sub_cfg.gift_value) || sub_cfg.gift_value <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "need_gold", sub_cfg.need_gold) || sub_cfg.need_gold <= 0)
		{
			return -8;
		}

		{
			PugiXmlNode reward_item_list_element = data_element.child("reward_item_list");
			if (reward_item_list_element.empty())
			{
				return -9;
			}

			PugiXmlNode reward_item_element = reward_item_list_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -10;
			}

			sub_cfg.item_count = 0;

			while (!reward_item_element.empty())
			{
				if (sub_cfg.item_count >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -11;
				}

				if (!sub_cfg.reward_item_list[sub_cfg.item_count].ReadConfig(reward_item_element))
				{
					return -12;
				}

				++sub_cfg.item_count;
				reward_item_element = reward_item_element.next_sibling();
			}
		}

		++limit_time_gift_cfg.sub_item_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityLimitTimeLuxuryGiftBagCfg::SubItemCfg * RandActivityConfig::GetLimitTimeLuxuryGiftBagRewardCfg(int opengame_day, const int vip_level)
{
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day <= m_limit_time_luxury_gift_bag_cfg_list[i].opengame_day)
		{
			for (int j = 0; j < m_limit_time_luxury_gift_bag_cfg_list[i].sub_item_cfg_count && j < RAND_ACTIVITY_LIMIT_TIME_GIFT_MAX_SEQ; ++j)
			{
				if (vip_level <= m_limit_time_luxury_gift_bag_cfg_list[i].sub_item_cfg_list[j].limit_vip_level)
				{
					return &m_limit_time_luxury_gift_bag_cfg_list[i].sub_item_cfg_list[j];
				}
			}
		}
	}

	return nullptr;
}

const RandActivityLimitTimeLuxuryGiftBagCfg::SubItemCfg * RandActivityConfig::GetLimitTimeLuxuryGiftBagRewardCfgBySeq(int opengame_day, const int seq)
{
	if (seq < 0 || seq > RAND_ACTIVITY_LIMIT_TIME_GIFT_MAX_SEQ) return nullptr;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day <= m_limit_time_luxury_gift_bag_cfg_list[i].opengame_day)
		{
			for (int j = 0; j < m_limit_time_luxury_gift_bag_cfg_list[i].sub_item_cfg_count && j <= RAND_ACTIVITY_LIMIT_TIME_GIFT_MAX_SEQ; ++j)
			{
				if (seq == m_limit_time_luxury_gift_bag_cfg_list[i].sub_item_cfg_list[j].seq)
				{
					return &m_limit_time_luxury_gift_bag_cfg_list[i].sub_item_cfg_list[j];
				}
			}
		}
	}

	return nullptr;
}

int RandActivityConfig::InitConsumeRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_openserver_day = 0;
	m_consume_reward_stage_cfg_count = -1;

	while (!dataElement.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(dataElement, "opengame_day", opengame_day) || opengame_day < last_openserver_day)
		{
			return -1;
		}

		if (opengame_day != last_openserver_day)
		{
			last_openserver_day = opengame_day;
			++m_consume_reward_stage_cfg_count;
		}

		if (m_consume_reward_stage_cfg_count < 0 || m_consume_reward_stage_cfg_count >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -1000;
		}

		RandActivityConsumeRewardCfg &cfg = m_consume_reward_stage_cfg_list[m_consume_reward_stage_cfg_count];
		cfg.openserver_day = opengame_day;

		RandActivityConsumeRewardStageCfg stage_cfg;
		if (!PugiGetSubNodeValue(dataElement, "activity_day", stage_cfg.activity_day) || stage_cfg.activity_day < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "limit_vip_level", stage_cfg.limit_vip_level) || stage_cfg.limit_vip_level < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "consume_gold", stage_cfg.consume_gold) || stage_cfg.consume_gold < 0)
		{
			return -4;
		}

		int item_count = ItemConfigData::ReadConfigList(dataElement, "reward_item", stage_cfg.reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
		if (item_count < 0)
		{
			return -100 + item_count;
		}

		auto iter = cfg.stage_cfg_vec.rbegin();
		if (iter != cfg.stage_cfg_vec.rend())
		{
			if (stage_cfg.activity_day < iter->activity_day)
			{
				return -5;
			}
		}

		cfg.stage_cfg_vec.push_back(stage_cfg);

		dataElement = dataElement.next_sibling();
	}

	++m_consume_reward_stage_cfg_count;

	return 0;
}

const RandActivityConsumeRewardStageCfg * RandActivityConfig::GetConsumeRewardCfg(int vip_level, int activity_day)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD);
	int stage_index = -1;
	for (int i = 0; i < m_consume_reward_stage_cfg_count && i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day <= m_consume_reward_stage_cfg_list[i].openserver_day)
		{
			stage_index = i;
			break;
		}
	}

	if (stage_index < 0 || stage_index >= m_consume_reward_stage_cfg_count || stage_index >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return nullptr;
	}

	//跨天自动领取的时候，直接使用传过来的activity_day
	if (0 == activity_day)
	{
		time_t begin_time = ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD);
		time_t begin_zero_time = GetZeroTime(begin_time);
		activity_day = GetDayIndex(begin_zero_time, EngineAdapter::Instance().Time());
	}
	else
	{
		//传过来的天数是加了1的
		--activity_day;
	}

	RandActivityConsumeRewardStageCfg temp_stage_cfg;
	temp_stage_cfg.activity_day = activity_day;

	const RandActivityConsumeRewardCfg &cfg = m_consume_reward_stage_cfg_list[stage_index];
	auto pair = std::equal_range(cfg.stage_cfg_vec.begin(), cfg.stage_cfg_vec.end(), temp_stage_cfg);
	for (auto iter = pair.first; iter != pair.second; ++iter)
	{
		if (vip_level <= iter->limit_vip_level)
		{
			return &(*iter);
		}
	}

	return nullptr;
}

int RandActivityConfig::InitRmbBuyChestShopCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_opengame_day = 0;
	int stage = -1;
	while (!data_element.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < last_opengame_day)
		{
			return -1;
		}

		if (last_opengame_day != opengame_day)
		{
			++stage;
			last_opengame_day = opengame_day;
		}

		RandActivityRmbBuyChestShopOpenDayCfg &cfg = m_rmb_buy_ches_shop_list[stage];
		cfg.opengame_day = opengame_day;

		RandActivityRmbBuyChestShopOpenDayCfg::RmbRewardCfg item_cfg;
		if (!PugiGetSubNodeValue(data_element, "rmb_num", item_cfg.rmb_num) || item_cfg.rmb_num < 0)
		{
			return -2;
		}

		for (auto &v : cfg.reward_list)
		{
			if (v.rmb_num == item_cfg.rmb_num)
			{
				//购买rmb价格不能有相同的;
				return -3;
			}
		}

		{
			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -4;
			}

			if (!item_cfg.reward_item.ReadConfig(reward_item_element))
			{
				return -5;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "count_limit", item_cfg.count_limit) || item_cfg.count_limit <= 0 || item_cfg.count_limit >= SCHAR_MAX)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "index", item_cfg.index) || item_cfg.index < 0 || item_cfg.index != (short)cfg.reward_list.size())
		{
			return -7;
		}

		if (item_cfg.index >= RA_RMB_BUY_CHEST_SHOP_MAX_BUY_COUNT)
		{
			return -8;
		}

		cfg.reward_list.push_back(item_cfg);
		data_element = data_element.next_sibling();
	}
	return 0;
}

const RandActivityRmbBuyChestShopOpenDayCfg::RmbRewardCfg * RandActivityConfig::GetRmbBuyChestShopCfg(int rmb_num)
{
	unsigned int real_start_server_timestamp = static_cast<unsigned int>(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime());
	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	int openserver_day = GetDayIndex(real_start_server_timestamp, now_second) + 1;
	for (auto &v : m_rmb_buy_ches_shop_list)
	{
		if (openserver_day <= v.opengame_day)
		{
			for (auto &cfg : v.reward_list)
			{
				if (cfg.rmb_num == rmb_num)
				{
					return &cfg;
				}
			}

			break;
		}
	}

	return nullptr;
}

int RandActivityConfig::InitItemCollectionSecondRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_item_collection_2_reward_cfg.cfg_count = 0;
	while (!data_element.empty())
	{
		if (m_item_collection_2_reward_cfg.cfg_count >= RAND_ACTIVITY_ITEM_COLLECTION_SECOND_REWARD_MAX_COUNT)
		{
			return -1000;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_item_collection_2_reward_cfg.cfg_count)
		{
			return -10;
		}

		RandActivityItemCollectionSecondRewardCfg::ConfigItem &reward_cfg = m_item_collection_2_reward_cfg.cfg_list[seq];
		reward_cfg.seq = seq;

		char temp_str[32] = { 0 };
		for (int i = 0; i < RandActivityItemCollectionSecondRewardCfg::MAX_COLLECTION_STUFF_COUNT && reward_cfg.stuff_count < RandActivityItemCollectionSecondRewardCfg::MAX_COLLECTION_STUFF_COUNT; i++)
		{
			sprintf(temp_str, "stuff_id%d", i + 1);
			PugiXmlNode item_element = data_element.child(temp_str);
			if (item_element.empty())
			{
				return -100;
			}

			if (!reward_cfg.stuff_id_list[reward_cfg.stuff_count].ReadConfig(item_element) && reward_cfg.stuff_id_list[reward_cfg.stuff_count].item_id != 0)
			{
				return -101;
			}

			reward_cfg.stuff_count++;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -200;
		}

		if (!reward_cfg.reward_item.ReadConfig(item_element))
		{
			return -201;
		}

		if (!PugiGetSubNodeValue(data_element, "exchange_times_limit", reward_cfg.exchange_times_limit) || reward_cfg.exchange_times_limit < 0)
		{
			return -20;
		}

		++m_item_collection_2_reward_cfg.cfg_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityItemCollectionSecondRewardCfg::ConfigItem * RandActivityConfig::GetItemCollectionSecondRewardCfg(int seq)
{
	if (seq < 0 || seq >= m_item_collection_2_reward_cfg.cfg_count || seq >= RAND_ACTIVITY_ITEM_COLLECTION_SECOND_REWARD_MAX_COUNT)
	{
		return NULL;
	}

	return &m_item_collection_2_reward_cfg.cfg_list[seq];
}

int RandActivityConfig::InitGrandTotalChargeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}


	int last_opengame_day = 0;
	int last_need_chongzhi = 0;
	int diff_num = -1;

	while (!data_element.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < last_opengame_day || opengame_day <= 0)
		{
			return -100;
		}

		if (diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (last_opengame_day != opengame_day)
		{
			++diff_num;
			last_opengame_day = opengame_day;
			last_need_chongzhi = 0;
		}

		RandActivityTotalChargeRewardConfig &total_chognzhi_reward_cfg = m_grand_total_charge_reward_list[diff_num];
		total_chognzhi_reward_cfg.opengame_day = opengame_day;

		if (total_chognzhi_reward_cfg.cfg_count >= RandActivityTotalChargeRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -30000;
		}

		RandActivityTotalChargeRewardConfig::ConfigItem &cfg_item = total_chognzhi_reward_cfg.cfg_list[total_chognzhi_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item.seq) || cfg_item.seq != total_chognzhi_reward_cfg.cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "need_chognzhi", cfg_item.need_chongzhi) || cfg_item.need_chongzhi <= last_need_chongzhi)
		{
			return -2;
		}
		last_need_chongzhi = cfg_item.need_chongzhi;

		{
			// 读物品奖励
			PugiXmlNode item_list_element = data_element.child("reward_item_list");
			if (item_list_element.empty())
			{
				return -1001;
			}

			PugiXmlNode item_element = item_list_element.child("reward_item");
			if (item_element.empty())
			{
				return -1002;
			}

			cfg_item.item_count = 0;

			while (!item_element.empty())
			{
				if (cfg_item.item_count >= RandActivityTotalChargeRewardConfig::REWARD_ITEM_MAX_COUNT)
				{
					return -1003;
				}

				if (!cfg_item.item_list[cfg_item.item_count].ReadConfig(item_element))
				{
					return -1004;
				}

				++cfg_item.item_count;
				item_element = item_element.next_sibling();
			}
		}

		++total_chognzhi_reward_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityTotalChargeRewardConfig * RandActivityConfig::GetVersionGrandotalChargeRewardCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_VERSIONS_GRAND_TOTAL_CHARGE);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		RandActivityTotalChargeRewardConfig *total_chognzhi_reward_cfg = &m_grand_total_charge_reward_list[i];
		if (nullptr == total_chognzhi_reward_cfg)
		{
			continue;
		}

		if (opengame_day <= total_chognzhi_reward_cfg->opengame_day)
		{
			return total_chognzhi_reward_cfg;
		}
	}

	return nullptr;
}

int RandActivityConfig::GetVersionTotalChargeSeqWithCharge(int total_charge)
{
	const RandActivityTotalChargeRewardConfig *reward_cfg = this->GetVersionGrandotalChargeRewardCfg();
	if (nullptr == reward_cfg)
	{
		return -1;
	}

	for (int seq = reward_cfg->cfg_count - 1; seq >= 0; --seq)
	{
		if (total_charge >= reward_cfg->cfg_list[seq].need_chongzhi)
		{
			return seq;
		}
	}

	return -1;
}

const RandActivityTotalChargeRewardConfig::ConfigItem * RandActivityConfig::GetVersionTotalChargeRewardWithSeq(int seq)
{
	const RandActivityTotalChargeRewardConfig *reward_cfg = this->GetVersionGrandotalChargeRewardCfg();
	if (nullptr == reward_cfg)
	{
		return nullptr;
	}

	if (seq >= 0 && seq < RandActivityTotalChargeRewardConfig::CONFIG_ITEM_MAX_COUNT && seq < reward_cfg->cfg_count)
	{
		return &reward_cfg->cfg_list[seq];
	}

	return nullptr;
}

int RandActivityConfig::InitContinueChargeRewardChuCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_seq = 0;
	int last_open_server_day = 0;
	int last_need_chongzhi = 0;

	while (!data_element.empty())
	{
		RandActivityContinueChongzhiRewardConfig continue_chongzhi_reward_cfg;

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0)
		{
			return -1000;
		}

		if (seq == last_seq + 1)
		{
			last_seq = seq;
			last_open_server_day = 0;
			last_need_chongzhi = 0;
		}

		if (!PugiGetSubNodeValue(data_element, "day_index", continue_chongzhi_reward_cfg.lei_chong_day) || continue_chongzhi_reward_cfg.lei_chong_day <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "open_server_day", continue_chongzhi_reward_cfg.open_server_day) || continue_chongzhi_reward_cfg.open_server_day <= 0 || (0 != last_open_server_day && continue_chongzhi_reward_cfg.open_server_day != last_open_server_day))
		{
			return -2;
		}
		last_open_server_day = continue_chongzhi_reward_cfg.open_server_day;

		if (!PugiGetSubNodeValue(data_element, "need_chongzhi", continue_chongzhi_reward_cfg.need_chongzhi) || continue_chongzhi_reward_cfg.need_chongzhi <= 0 || (0 != last_need_chongzhi && continue_chongzhi_reward_cfg.need_chongzhi != last_need_chongzhi))
		{
			return -3;
		}
		last_need_chongzhi = continue_chongzhi_reward_cfg.need_chongzhi;

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -4;
		}

		if (!continue_chongzhi_reward_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -5;
		}

		continue_chongzhi_reward_cfg.seq = seq;
		m_continue_charge_reward_cfg_vec.push_back(continue_chongzhi_reward_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}


const RandActivityContinueChongzhiRewardConfig * RandActivityConfig::GetVersionContinueChargeRewardInfoWithdayindex(int day_index, bool is_chongzhi, bool is_bu_fa_reward)
{
	int open_day_index = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;

	if (is_chongzhi)
	{
		for (int i = 0; i < static_cast<int>(m_continue_charge_reward_cfg_vec.size()); i++)
		{
			if (open_day_index <= m_continue_charge_reward_cfg_vec[i].open_server_day)
			{
				return &m_continue_charge_reward_cfg_vec[i];
			}
		}
	}
	else
	{
		if (is_bu_fa_reward) open_day_index--;
		int seq = -1;
		for (int i = 0; i < static_cast<int>(m_continue_charge_reward_cfg_vec.size()); i++)
		{
			if (-1 != seq && seq != m_continue_charge_reward_cfg_vec[i].seq) return NULL;

			if (-1 == seq && open_day_index <= m_continue_charge_reward_cfg_vec[i].open_server_day)
			{
				seq = m_continue_charge_reward_cfg_vec[i].seq;
			}

			if (open_day_index <= m_continue_charge_reward_cfg_vec[i].open_server_day && m_continue_charge_reward_cfg_vec[i].lei_chong_day == day_index)
			{
				return &m_continue_charge_reward_cfg_vec[i];
			}
		}
	}

	return nullptr;
}

int RandActivityConfig::InitHuanLeYaoJiang2Cfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int is_rare_temp = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (last_opengame_day != opengame_day)
		{
			last_opengame_day = opengame_day;
			++stage;
			m_huanleyaojiang_2_stage_cfg_max_count = 0;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (m_huanleyaojiang_2_max_seq_list[stage] >= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ)
		{
			return -1;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_huanleyaojiang_2_max_seq_list[stage] + 1)
		{
			return -11;
		}

		m_huanleyaojiang_2_cfg_list[stage].opengame_day = opengame_day;
		RandActivityMiJingXunBaoConfig &huanleyaojaing_cfg = m_huanleyaojiang_2_cfg_list[stage].cfg_list[seq];
		huanleyaojaing_cfg.seq = seq;
		m_huanleyaojiang_2_max_seq_list[stage] = seq;

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -12;
		}

		if (!huanleyaojaing_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -13;
		}

		is_rare_temp = 0;
		if (!PugiGetSubNodeValue(data_element, "is_rare", is_rare_temp) || is_rare_temp < 0 || is_rare_temp > 1)
		{
			return -14;
		}
		huanleyaojaing_cfg.is_rare = (0 != is_rare_temp);

		if (!PugiGetSubNodeValue(data_element, "weight", huanleyaojaing_cfg.weight) || huanleyaojaing_cfg.weight < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(data_element, "auto_add_weight", huanleyaojaing_cfg.auto_add_weight) || huanleyaojaing_cfg.auto_add_weight < 0)
		{
			return -16;
		}

		if (huanleyaojaing_cfg.auto_add_weight > 0)
		{
			if (m_huanleyaojiang_2_auto_add_cfg_count_list[stage] >= RAND_ACTIVITY_MIJINGXUNBAO_AUTO_ADD_WEIGHT_CFG_COUNT)
			{
				return -18;
			}

			m_huanleyaojiang_2_add_weight_cfg_list[stage].opengame_day = opengame_day;
			m_huanleyaojiang_2_add_weight_cfg_list[stage].add_weight_cfg_list[m_huanleyaojiang_2_auto_add_cfg_count_list[stage]].add_weight = huanleyaojaing_cfg.auto_add_weight;
			m_huanleyaojiang_2_add_weight_cfg_list[stage].add_weight_cfg_list[m_huanleyaojiang_2_auto_add_cfg_count_list[stage]].seq = huanleyaojaing_cfg.seq;

			++m_huanleyaojiang_2_auto_add_cfg_count_list[stage];
		}

		int baodi = 0;
		if (PugiGetSubNodeValue(data_element, "baodi", baodi) && baodi > 0)
		{
			// 保底次数必须不同,因为保底必出物品（除非保底物品也随机）
			if (m_huanleyaojiang_2_cfg_list[stage].baodi_map.size() > 0)
			{
				return -9;
			}

			m_huanleyaojiang_2_cfg_list[stage].baodi_map[baodi] = seq;
		}

		++m_huanleyaojiang_2_stage_cfg_max_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitHuanLeYaoJiang2RewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int max_index = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			stage++;
			last_opengame_day = opengame_day;
			m_huanleyaojiang_2_reward_cfg_count = stage + 1;
			max_index = -1;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		int index = 0;
		if (!PugiGetSubNodeValue(data_element, "index", index) || index != max_index + 1 || index >= RAND_ACTIVITY_MIJINGXUNBAO_REWARD_MAX_COUNT)
		{
			return -1;
		}

		RandActivityMiJingXunBaoRewardDayCfg &day_cfg = m_huanleyaojiang_2_reward_list_cfg[stage];
		++day_cfg.cfg_count;
		day_cfg.opengame_day = opengame_day;

		RandActivityMiJingXunBaoRewardConfig &reward_list_cfg = day_cfg.cfg_list[index];
		reward_list_cfg.index = index;
		max_index = index;

		if (!PugiGetSubNodeValue(data_element, "choujiang_times", reward_list_cfg.choujiang_times) || reward_list_cfg.choujiang_times < 0)
		{
			return -2;
		}

		{
			PugiXmlNode reward_item_element = data_element.child("reward_item");
			if (reward_item_element.empty())
			{
				return -3;
			}

			if (!reward_list_cfg.reward_item.ReadConfig(reward_item_element))
			{
				return -4;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::GetHuanLeYaoJiang2AutoWeightAddCount()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_huanleyaojiang_2_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	return m_huanleyaojiang_2_auto_add_cfg_count_list[stage];
}

int RandActivityConfig::GetHuanLeYaoJiang2AddWeightSeq(int index)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_huanleyaojiang_2_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	if (index < 0 || index >= m_huanleyaojiang_2_auto_add_cfg_count_list[stage])
	{
		return 0;
	}

	return m_huanleyaojiang_2_add_weight_cfg_list[stage].add_weight_cfg_list[index].seq;
}

int RandActivityConfig::GetHuanLeYaoJiang2CfgAddWeight(int seq)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_huanleyaojiang_2_add_weight_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	if (seq <= 0 || seq > m_huanleyaojiang_2_max_seq_list[stage] || seq > RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ)
	{
		return 0;
	}

	for (int index = 0; index < m_huanleyaojiang_2_auto_add_cfg_count_list[stage]; index++)
	{
		if (m_huanleyaojiang_2_add_weight_cfg_list[stage].add_weight_cfg_list[index].seq == seq)
		{
			return m_huanleyaojiang_2_add_weight_cfg_list[stage].add_weight_cfg_list[index].add_weight;
		}
	}

	return 0;
}

int RandActivityConfig::GetHuanLeYaoJiang2TaoGold(int type)
{
	if (type < RA_MIJINGXUNBAO3_CHOU_TYPE_1 || type >= RA_MIJINGXUNBAO3_CHOU_TYPE_MAX)
	{
		return 0;
	}

	switch (type)
	{
	case RA_MIJINGXUNBAO3_CHOU_TYPE_1:
	{
		return m_huanleyaojiang_2_once_gold;
	}
	break;

	case RA_MIJINGXUNBAO3_CHOU_TYPE_10:
	{
		return m_huanleyaojiang_2_tentimes_gold;
	}
	break;

	case RA_MIJINGXUNBAO3_CHOU_TYPE_30:
	{
		return m_huanleyaojiang_2_thirtytimes_gold;
	}
	break;

	default:
		return 0;
	}
}

bool RandActivityConfig::GetHuanLeYaoJiang2RandomResult(MiJingXunBaoAddWeightInfo * self_weight, int length, int is_free, RandActivityMiJingXunBaoConfig * result_list, int result_list_length)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_huanleyaojiang_2_cfg_list[i].opengame_day)
		{
			continue;
		}

		stage = i;
		break;
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return false;
	}

	if (NULL == self_weight || length < 0 || NULL == result_list || result_list_length <= 0)
	{
		return false;
	}

	int all_weight = 0;
	int all_cfg_weight_list[RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ + 1] = { 0 };

	for (int i = 1; i <= m_huanleyaojiang_2_max_seq_list[stage] && i <= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ; ++i)
	{
		all_cfg_weight_list[i] = m_huanleyaojiang_2_cfg_list[stage].cfg_list[i].weight;

		if (!is_free)	// 非免费刷新 增加自己权重
		{
			for (int self_index = 0; self_index < length; ++self_index)
			{
				if (self_weight[self_index].seq == m_huanleyaojiang_2_cfg_list[stage].cfg_list[i].seq)
				{
					all_cfg_weight_list[i] += self_weight[self_index].weight;
				}
			}
		}

		all_weight += all_cfg_weight_list[i];
	}

	if (all_weight <= 0)
	{
		return false;
	}

	RandActivityHuanLeYaoJiang2 *huanle_yaojiang_activity = RandActivityManager::Instance().GetRandActivityHuanLeYaoJiang2();
	if (nullptr == huanle_yaojiang_activity)
	{
		return false;
	}

	for (int result_index = 0; result_index < result_list_length && all_weight > 0; ++result_index)
	{
		int rand_weight = RandomNum(all_weight);
		for (int i = 1; i <= m_huanleyaojiang_2_max_seq_list[stage] && i <= RAND_ACTIVITY_MIJINGXUNBAO_MAX_SEQ; ++i)
		{
			if (rand_weight < all_cfg_weight_list[i])
			{
				int server_chou_times = huanle_yaojiang_activity->GetServerChouTimes();

				// 如果没有保底奖励才进行随机奖励
				const RandActivityMiJingXunBaoConfig *lottery_cfg = this->GetHuanLeYaoJiang2BaodiRewardCfg(server_chou_times);
				if (nullptr != lottery_cfg)
				{
					huanle_yaojiang_activity->ClearServerChouTimes();
					result_list[result_index] = *lottery_cfg;
				}
				else
				{
					result_list[result_index] = m_huanleyaojiang_2_cfg_list[stage].cfg_list[i];
				}

				for (int self_index = 0; self_index < length; ++self_index)
				{
					if (result_list[result_index].seq == self_weight[self_index].seq) //1 抽到珍稀，把该奖励权重置0
					{
						all_cfg_weight_list[self_weight[self_index].seq] -= self_weight[self_index].weight;
						all_weight -= self_weight[self_index].weight;
						if (all_weight <= 0)
						{
							return false;
						}

						self_weight[self_index].weight = 0;
						self_weight[self_index].tao_none_rare_times = 0;
					}
					else
					{
						self_weight[self_index].tao_none_rare_times++;
						int add_weight = this->GetHuanLeYaoJiang2CfgAddWeight(self_weight[self_index].seq);

						if (self_weight[self_index].tao_none_rare_times >= this->GetHuanLeYaoJiang2AddWeightTaoTimes())
						{
							all_weight += add_weight;

							self_weight[self_index].weight += add_weight;
							all_cfg_weight_list[self_weight[self_index].seq] += self_weight[self_index].weight;

							self_weight[self_index].tao_none_rare_times -= this->GetHuanLeYaoJiang2AddWeightTaoTimes();
						}
					}
				}
				break;
			}

			rand_weight -= all_cfg_weight_list[i];
		}

		huanle_yaojiang_activity->AddServerChouTimes(1);
	}

	return true;
}

const RandActivityMiJingXunBaoRewardConfig * RandActivityConfig::GetRandActivityHuanLeYaoJiang2PersonalRewardCfg(int seq, int chou_times)
{
	int idx = -1;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2);
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM && i < m_huanleyaojiang_2_reward_cfg_count; ++i)
	{
		if (m_huanleyaojiang_2_reward_list_cfg[i].opengame_day >= opengame_day)
		{
			idx = i;
			break;
		}
	}

	if (idx < 0 || idx >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return nullptr;
	}

	RandActivityMiJingXunBaoRewardDayCfg &day_cfg = m_huanleyaojiang_2_reward_list_cfg[idx];

	if (seq < 0 || seq >= RAND_ACTIVITY_MIJINGXUNBAO_REWARD_MAX_COUNT || chou_times < 0)
	{
		return nullptr;
	}

	if (chou_times >= day_cfg.cfg_list[seq].choujiang_times && day_cfg.cfg_list[seq].choujiang_times != 0)
	{
		return &day_cfg.cfg_list[seq];
	}

	return nullptr;
}

const RandActivityMiJingXunBaoConfig * RandActivityConfig::GetHuanLeYaoJiang2BaodiRewardCfg(int server_chou_times)
{
	if (server_chou_times <= 0) return nullptr;

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_huanleyaojiang_2_cfg_list[i].opengame_day)
		{
			continue;
		}

		RandActivityMiJingXunBaoCfgList &cur_stage_cfg = m_huanleyaojiang_2_cfg_list[i];

		for (std::map<int, int>::iterator it = cur_stage_cfg.baodi_map.begin(); it != cur_stage_cfg.baodi_map.end(); ++it)
		{
			const int baodi_times = it->first;
			const int reward_seq = it->second;

			if (baodi_times <= 0 || 0 != server_chou_times % baodi_times) continue;

			if (reward_seq < 0 || reward_seq > m_huanleyaojiang_2_stage_cfg_max_count) continue;

			return &m_huanleyaojiang_2_cfg_list[i].cfg_list[reward_seq];
		}
		break;
	}

	return nullptr;
}

int RandActivityConfig::InitOfflineSingleChargeCfg0(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_charge_value = 0;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;
	int last_reward_type = -1;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++stage_num;
			last_charge_value = 0;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}
		m_offline_single_charge_cfg_0[stage_num].opengame_day = opengame_day;

		if (m_offline_single_charge_cfg_0[stage_num].cfg_count >= RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT)
		{
			return -2;
		}

		RandActivityOfflineSingleChargeCfg::ConfigItem *cfg_item = &m_offline_single_charge_cfg_0[stage_num].cfg_list[m_offline_single_charge_cfg_0[stage_num].cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", cfg_item->seq) || cfg_item->seq != m_offline_single_charge_cfg_0[stage_num].cfg_count)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "charge_value", cfg_item->charge_value) || cfg_item->charge_value <= last_charge_value)
		{
			return -4;
		}
		last_charge_value = cfg_item->charge_value;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -5;
		}
		if (!cfg_item->reward_item.ReadConfig(reward_item))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "reward_type", cfg_item->reward_type) ||
			cfg_item->reward_type < 0 || cfg_item->reward_type >= RandActivityOfflineSingleChargeCfg::REWARD_TYPE_MAX ||
			(last_reward_type >= 0 && last_reward_type != cfg_item->reward_type))
		{
			return -7;
		}
		last_reward_type = cfg_item->reward_type;

		if (!PugiGetSubNodeValue(data_element, "reward_limit", cfg_item->reward_limit) || cfg_item->reward_limit <= 0)
		{
			return -8;
		}

		const ChongzhiReward *extra_reward_cfg = LOGIC_CONFIG->GetChongzhiCfg().GetExtraRewardByNum(cfg_item->charge_value);
		if (NULL == extra_reward_cfg)
		{
			return -44;
		}

		++m_offline_single_charge_cfg_0[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityOfflineSingleChargeCfg::ConfigItem * RandActivityConfig::GetOfflineSingleChargeCfg0(long long charge_value, unsigned int join_timestamp)
{
	if (charge_value <= 0)
	{
		return NULL;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return NULL;
	}
	int opengame_day = 0;
	if (join_timestamp > 0)
	{
		opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), static_cast<time_t>(join_timestamp)) + 1;
	}
	else if (0 == join_timestamp)
	{
		opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_OFFLINE_SINGLE_CHARGE_0);
	}

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_offline_single_charge_cfg_0[i].opengame_day)
		{
			continue;
		}

		for (int index = m_offline_single_charge_cfg_0[i].cfg_count - 1; index >= 0; index--)
		{
			RandActivityOfflineSingleChargeCfg::ConfigItem *cfg_item = &m_offline_single_charge_cfg_0[i].cfg_list[index];
			if (charge_value >= cfg_item->charge_value)
			{
				return cfg_item;
			}
		}
	}

	return NULL;
}

int RandActivityConfig::InitBuyOneGetOneFreeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_opengame_day = -1;
	int day_state_index = -1;
	int opengame_day = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -1;
		}

		if (opengame_day != last_opengame_day)
		{
			++day_state_index;
		}
		last_opengame_day = opengame_day;

		if (day_state_index < 0 || day_state_index >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -2;
		}

		RandActivityBuyOneGetOneFreeConfig &cfg = m_buy_one_get_one_free_cfg_list[day_state_index];
		cfg.opengame_day = opengame_day;

		if (cfg.item_count >= RAND_ACTIVITY_BUY_ONE_GET_ONE_FREE_ITEM_MAX_COUNT)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "price_gold", cfg.item_list[cfg.item_count].price_gold) || cfg.item_list[cfg.item_count].price_gold <= 0)
		{
			return -4;
		}

		{
			PugiXmlNode buy_item_element = data_element.child("buy_item");
			if (buy_item_element.empty())
			{
				return -5;
			}

			if (!cfg.item_list[cfg.item_count].buy_item.ReadConfig(buy_item_element))
			{
				return -6;
			}
		}

		{
			PugiXmlNode reward_item_element = data_element.child("free_reward_item");
			if (reward_item_element.empty())
			{
				return -7;
			}

			if (!cfg.item_list[cfg.item_count].free_reward_item.ReadConfig(reward_item_element))
			{
				return -8;
			}
		}

		cfg.item_count++;

		data_element = data_element.next_sibling();
	}

	return 0;
}

const BuyOneGetOneFreeItemCfg* RandActivityConfig::GetBuyOneGetOneFreeCfg(int opengame_day, int item_index)
{
	if (opengame_day <= 0 || item_index < 0 || item_index >= RAND_ACTIVITY_BUY_ONE_GET_ONE_FREE_ITEM_MAX_COUNT)
	{
		return nullptr;
	}

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; i++)
	{
		if (m_buy_one_get_one_free_cfg_list[i].opengame_day >= opengame_day)
		{
			return &m_buy_one_get_one_free_cfg_list[i].item_list[item_index];
		}
	}

	return nullptr;
}

RandActivityEverydayNiceGiftCfg::ConfigItem * RandActivityConfig::GetEveydayNiceGiftRewardCfg()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT);

	for (int i = 0; i < m_everyday_nice_gift_cfg_count; ++i)
	{
		if (opengame_day > m_everyday_nice_gift_cfg[i].opengame_day)
		{
			continue;
		}

		return &m_everyday_nice_gift_cfg[i].everyday_reward;
	}

	return nullptr;
}

const RandActivityLoginGiftCfg::ConfigItem * RandActivityConfig::GetLoginGiftRewardCfg_0(int seq)
{
	if (seq >= 0 && seq < m_login_gift_cfg_0.cfg_count)
	{
		return &m_login_gift_cfg_0.cfg_list[seq];
	}

	return nullptr;
}

int RandActivityConfig::InitHolidayGuardKillRankRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int check_rank = -1;
	int open_server = 0;
	int last_open_server = 0;
	int stage_num = -1;
	int reward_num = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", open_server) || open_server <= 0)
		{
			return -1;
		}

		if (open_server != last_open_server)
		{
			++stage_num;
			reward_num = 0;
			++m_holiday_guard_kill_rank_num;
			check_rank = -1;
		}
		last_open_server = open_server;

		if (reward_num < 0 || reward_num >= RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX || stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -2;
		}

		m_holiday_guard_kill_rank_reward_cfg[stage_num].open_server_day = open_server;
		HolidayGuardKillRankReward::ConfigItem & item_config = m_holiday_guard_kill_rank_reward_cfg[stage_num].kill_rank_reward_list_cfg[reward_num];

		int rank = 0;
		if (!PugiGetSubNodeValue(data_element, "rank", rank) || rank < 0 || rank >= RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX || rank != check_rank + 1)
		{
			return -3;
		}

		check_rank = rank;

		PugiXmlNode item_element_list = data_element.child("reward_item_list");
		if (item_element_list.empty())
		{
			return -4;
		}

		PugiXmlNode reward_item_element = item_element_list.child("reward_item");
		if (reward_item_element.empty())
		{
			return -5;
		}

		int reward_count = 0;

		while (!reward_item_element.empty())
		{
			if (reward_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -6;
			}

			if (!item_config.reward_list[reward_count].ReadConfig(reward_item_element))
			{
				return -7;
			}

			++reward_count;
			reward_item_element = reward_item_element.next_sibling();
		}

		++reward_num;
		data_element = data_element.next_sibling();
	}

	return 0;
}

HolidayGuardKillRankReward::ConfigItem * RandActivityConfig::GetHolidayGuardKillRankRewardCfg(int role_rank)
{
	if (role_rank < 0 || role_rank >= RAND_ACTIVITY_HOLIDAYGUARD_PERSON_RANK_MAX)
	{
		return nullptr;
	}

	int opengame_day = LOGIC_CONFIG->GetRandActivityCfg().GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_HOLIDAY_GUARD);

	for (int i = 0; i < m_holiday_guard_kill_rank_num; ++i)
	{
		if (opengame_day > m_holiday_guard_kill_rank_reward_cfg[i].open_server_day)
		{
			continue;
		}

		return &m_holiday_guard_kill_rank_reward_cfg[i].kill_rank_reward_list_cfg[role_rank];
	}

	return nullptr;
}

// 累计充值5（版本吉祥三宝）
int RandActivityConfig::InitTotalChongzhi5Cfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_opengame_day = 0;
	int stage = -1;

	while (!data_element.empty())
	{
		int start_day = 0;
		int end_day = 0;
		int opengame_day = 0;

		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -1;
		}

		if (opengame_day != last_opengame_day)
		{
			if (opengame_day <= last_opengame_day)
			{
				return -200;
			}

			++stage;
			last_opengame_day = opengame_day;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -300;
		}

		RandActivityTotalCharge5Config &stage_cfg = m_total_charge_5_cfg[stage];
		stage_cfg.opengame_day = opengame_day;

		if (!PugiGetSubNodeValue(data_element, "start_day", start_day) || start_day < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "end_day", end_day) || end_day < 0 || end_day < start_day)
		{
			return -2;
		}

		if (stage_cfg.total_charge_5_cfglist_count >= RAND_ACTIVITY_TOTAL_CHARGE_5_CFG_COUNT)
		{
			return -3;
		}

		bool same_with_pre = false; // 是否跟上一个配置是同一个时间段

		RandActivityTotalCharge5Cfg &pre_total_charge5_cfg = stage_cfg.total_charge_5_cfglist[stage_cfg.total_charge_5_cfglist_count - 1];

		if (stage_cfg.total_charge_5_cfglist_count > 0)
		{
			if (pre_total_charge5_cfg.start_day == start_day && pre_total_charge5_cfg.end_day == end_day)
			{
				same_with_pre = true;
			}
			else if (start_day > pre_total_charge5_cfg.end_day)
			{
				same_with_pre = false;
			}
			else
			{
				return -4;
			}
		}

		int cur_index = stage_cfg.total_charge_5_cfglist_count;
		if (same_with_pre)
		{
			cur_index = stage_cfg.total_charge_5_cfglist_count - 1;
		}

		RandActivityTotalCharge5Cfg &total_charge5_cfg = stage_cfg.total_charge_5_cfglist[cur_index];
		total_charge5_cfg.start_day = start_day;
		total_charge5_cfg.end_day = end_day;

		if (total_charge5_cfg.chongzhi_list_count >= RandActivityTotalCharge5Cfg::MAX_CHONGZHI_CFG_COUNT)
		{
			return -5;
		}

		RandActivityTotalCharge5Cfg::ConfigItem &sub_cfg = total_charge5_cfg.chongzhi_list[total_charge5_cfg.chongzhi_list_count];
		sub_cfg.seq = total_charge5_cfg.chongzhi_list_count;

		if (!PugiGetSubNodeValue(data_element, "need_chongzhi_num", sub_cfg.need_chongzhi_num) || sub_cfg.need_chongzhi_num <= 0)
		{
			return -6;
		}

		PugiXmlNode item_element_list = data_element.child("reward_item_list");
		if (item_element_list.empty())
		{
			return -1001;
		}

		PugiXmlNode reward_item_element = item_element_list.child("reward_item");
		if (reward_item_element.empty())
		{
			return -7;
		}

		int reward_count = 0;

		while (!reward_item_element.empty())
		{
			if (reward_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -20;
			}

			if (!sub_cfg.reward_item[reward_count].ReadConfig(reward_item_element))
			{
				return -8;
			}

			++reward_count;
			reward_item_element = reward_item_element.next_sibling();
		}

		++total_charge5_cfg.chongzhi_list_count;

		if (!same_with_pre)
		{
			++stage_cfg.total_charge_5_cfglist_count;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityTotalCharge5Cfg::ConfigItem * RandActivityConfig::GetTotalCharge5Cfg(int day_idx, int seq)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_FIVE);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day <= m_total_charge_5_cfg[i].opengame_day)
		{
			stage = i;
			break;
		}
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return nullptr;
	}

	RandActivityTotalCharge5Config &cfg = m_total_charge_5_cfg[stage];

	for (int index = 0; index < cfg.total_charge_5_cfglist_count; ++index)
	{
		RandActivityTotalCharge5Cfg &charge_cfg = cfg.total_charge_5_cfglist[index];
		if (day_idx >= charge_cfg.start_day && day_idx <= charge_cfg.end_day)
		{
			if (seq >= 0 && seq < charge_cfg.chongzhi_list_count && seq < RandActivityTotalCharge5Cfg::MAX_CHONGZHI_CFG_COUNT)
			{
				return &charge_cfg.chongzhi_list[seq];
			}
		}
	}

	return nullptr;
}

int RandActivityConfig::GetTotalCharge5CfgCount(int day_idx)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_FIVE);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day <= m_total_charge_5_cfg[i].opengame_day)
		{
			stage = i;
			break;
		}
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return 0;
	}

	RandActivityTotalCharge5Config &cfg = m_total_charge_5_cfg[stage];

	for (int index = 0; index < cfg.total_charge_5_cfglist_count; ++index)
	{
		RandActivityTotalCharge5Cfg &charge_cfg = cfg.total_charge_5_cfglist[index];
		if (day_idx >= charge_cfg.start_day && day_idx <= charge_cfg.end_day)
		{
			return charge_cfg.chongzhi_list_count;
		}
	}

	return 0;
}

bool RandActivityConfig::CheckIsResetTotalCharge5Day(int day_idx)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_FIVE);
	int stage = -1;

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day <= m_total_charge_5_cfg[i].opengame_day)
		{
			stage = i;
			break;
		}
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return false;
	}

	RandActivityTotalCharge5Config &cfg = m_total_charge_5_cfg[stage];

	for (int index = 0; index < cfg.total_charge_5_cfglist_count; ++index)
	{
		if (day_idx == cfg.total_charge_5_cfglist[index].start_day)
		{
			return true;
		}
	}

	return false;
}

int RandActivityConfig::InitExtremeChallengeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		if (m_extreme_challenge_reward_count >= RAND_ACTIVITY_EXTREME_CHALLENGE_MAX_TASK_COUNT)
		{
			return -5;
		}

		RandActivityExtremeChallengeCfg &tmp_cfg = m_extreme_challenge_cfg[m_extreme_challenge_reward_count];

		if (!PugiGetSubNodeValue(data_element, "task_id", tmp_cfg.task_id) || tmp_cfg.task_id <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "task_type", tmp_cfg.task_type) || tmp_cfg.task_type < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "param1", tmp_cfg.param1) || tmp_cfg.param1 <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "param2", tmp_cfg.param2) || tmp_cfg.param2 < 0)
		{
			return -4;
		}

		PugiXmlNode item_element_list = data_element.child("reward_item_list");
		if (item_element_list.empty())
		{
			return -5;
		}

		PugiXmlNode reward_item_element = item_element_list.child("reward_item");
		if (reward_item_element.empty())
		{
			return -6;
		}

		int reward_count = 0;

		while (!reward_item_element.empty())
		{
			if (reward_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -7;
			}

			if (!tmp_cfg.reward_list[reward_count].ReadConfig(reward_item_element))
			{
				return -8;
			}

			++reward_count;
			reward_item_element = reward_item_element.next_sibling();
		}

		++m_extreme_challenge_reward_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitProfessRankRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_rank_index = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;

	while (!data_element.empty())
	{

		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -100;
		}

		if (opengame_day != last_opengame_day)
		{
			++stage_num;
			last_rank_index = -1;
		}
		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}
		m_profess_rand_cfg[stage_num].opengame_day = opengame_day;

		if (m_profess_rand_cfg[stage_num].cfg_count >= RAND_ACTIVITY_CONSUME_GOLD_RANK_REWARD_CONFIG_MAX_COUNT)
		{
			return -300;
		}

		RandActivityProfessRankCfg::ConfigItem &cfg_item = m_profess_rand_cfg[stage_num].cfg_list[m_profess_rand_cfg[stage_num].cfg_count];

		if (!PugiGetSubNodeValue(data_element, "rank_index", cfg_item.rank) || cfg_item.rank <= last_rank_index)
		{
			return -1;
		}
		last_rank_index = cfg_item.rank;

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -100;
		}

		if (!cfg_item.item.ReadConfig(reward_item))
		{
			return -101;
		}

		PugiXmlNode female_reward_item = data_element.child("female_reward_item");
		if (female_reward_item.empty())
		{
			return -110;
		}

		if (!cfg_item.female_item.ReadConfig(female_reward_item))
		{
			return -111;
		}

		if (!PugiGetSubNodeValue(data_element, "limit_score", cfg_item.limit_score) || cfg_item.limit_score < 0)
		{
			return -4;
		}

		++m_profess_rand_cfg[stage_num].cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::GetExtremeChallengeRandTaskByType(char type)
{
	if (type < RA_EXTREME_CHALLENGE_ACTIVE || type >= RA_EXTREME_CHALLENGE_MAX_NUM)
	{
		return 0;
	}

	int task_id[RAND_ACTIVITY_EXTREME_CHALLENGE_MAX_TASK_COUNT] = {0};
	int index = 0;

	for (int i = 0; i < RAND_ACTIVITY_EXTREME_CHALLENGE_MAX_TASK_COUNT && i < m_extreme_challenge_reward_count; ++i)
	{
		if (m_extreme_challenge_cfg[i].task_type == type)
		{
			task_id[index] = m_extreme_challenge_cfg[i].task_id;
			++index;
		}
	}

	return task_id[RandomNum(index)];
}

const RandActivityExtremeChallengeCfg* RandActivityConfig::GetExtremeChallengePerformTaskInfoCfg(int task_id)
{
	for (int i = 0; i < RAND_ACTIVITY_EXTREME_CHALLENGE_MAX_TASK_COUNT; ++i)
	{
		if (m_extreme_challenge_cfg[i].task_id == task_id)
		{
			return &m_extreme_challenge_cfg[i];
		}
	}

	return nullptr;
}

// 表白排行榜

int RandActivityConfig::GetProfessRankCfgCount()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_PROFESS_RANK);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_profess_rand_cfg[i].opengame_day)
		{
			continue;
		}

		return m_profess_rand_cfg[i].cfg_count;
	}

	return 0;
}

const RandActivityProfessRankCfg::ConfigItem * RandActivityConfig::GetProfessRankRewardCfg(int user_rank_index)
{
	if (user_rank_index < 0) return NULL;
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_PROFESS_RANK);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_profess_rand_cfg[i].opengame_day)
		{
			continue;
		}

		for (int j = 0; j < m_profess_rand_cfg[i].cfg_count; ++j)
		{
			if (user_rank_index <= m_profess_rand_cfg[i].cfg_list[j].rank)
			{
				return &m_profess_rand_cfg[i].cfg_list[j];
			}
		}
	}

	return NULL;
}

int RandActivityConfig::InitChongZhiGiftCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_opengame_day = 0;
	int diff_num = -1;
	while (!data_element.empty())
	{
		RandActivityChongZhiGiftConfig cfg;

		if (!PugiGetSubNodeValue(data_element, "opengame_day", cfg.opengame_day) || cfg.opengame_day < last_opengame_day || cfg.opengame_day <= 0)
		{
			return -1;
		}

		if (diff_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		if (last_opengame_day != cfg.opengame_day)
		{
			++diff_num;
			last_opengame_day = cfg.opengame_day;
		}

		if (!PugiGetSubNodeValue(data_element, "activity_day", cfg.activity_day) || cfg.activity_day <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "index", cfg.index) || cfg.index < 0 || cfg.index >= RA_CHONGZHI_GIFT_ITEM_MAX_INDEX)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "need_gold", cfg.need_gold) || cfg.need_gold < 0)
		{
			return -4;
		}

		{
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				return -5;
			}

			if (!cfg.reward_item.ReadConfig(item_element))
			{
				return -6;
			}
		}

		m_chongzhi_gift_cfg_list.push_back(cfg);
		data_element = data_element.next_sibling();
	}

	return 0;
}

RandActivityChongZhiGiftConfig *RandActivityConfig::GetChongZhiGiftByIndex(int index)
{
	if (index < 0 || index >= RA_CHONGZHI_GIFT_ITEM_MAX_INDEX) return NULL;

	time_t activity_begin_time = ActivityShadow::Instance().GetRandActivityBeginTime(RAND_ACTIVITY_TYPE_CHONGZHI_GIFT);
	time_t now_time = EngineAdapter::Instance().Time();

	int act_real_open_day = GetDayIndex(activity_begin_time, now_time) + 1;
	if (act_real_open_day <= 0) return NULL;

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CHONGZHI_GIFT);

	for (int i = 0; i < (int)m_chongzhi_gift_cfg_list.size(); ++i)
	{
		if (opengame_day <= m_chongzhi_gift_cfg_list[i].opengame_day && act_real_open_day == m_chongzhi_gift_cfg_list[i].activity_day &&
			index == m_chongzhi_gift_cfg_list[i].index)
		{
			return &m_chongzhi_gift_cfg_list[i];
		}
	}

	return NULL;
}

const RandActivityChongZhiGiftConfig * RandActivityConfig::GetChongZhiGiftCfg(int act_day, int index)
{
	if (index < 0 || index >= RA_CHONGZHI_GIFT_ITEM_MAX_INDEX) return NULL;

	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CHONGZHI_GIFT);

	for (int i = 0; i < (int)m_chongzhi_gift_cfg_list.size(); ++i)
	{
		if (opengame_day <= m_chongzhi_gift_cfg_list[i].opengame_day && act_day == m_chongzhi_gift_cfg_list[i].activity_day &&
			index == m_chongzhi_gift_cfg_list[i].index)
		{
			return &m_chongzhi_gift_cfg_list[i];
		}
	}

	return NULL;
}

int RandActivityConfig::InitJinJieReturn2Config(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int act_type = 0;
	int last_act_type = 0;
	int seq = 0;
	int last_seq = -1;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "act_type", act_type) || act_type <= JINJIE_ACT_THEME_TYPE_INVALID || act_type >= JINJIE_ACT_THEME_TYPE_MAX)
		{
			return -1;
		}

		if (act_type != last_act_type)
		{
			last_seq = -1;
			last_act_type = act_type;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= RAND_ACTIVITY_JINJIE_RETURN_MAX_CFG || seq != last_seq + 1)
		{
			return -2;
		}

		last_seq = seq;

		JinJieReturnConfig & cfg = m_jinjie_return2_cfg[act_type][seq];

		if (!PugiGetSubNodeValue(data_element, "need_grade", cfg.need_grade) || cfg.need_grade <= 0)
		{
			return -3;
		}

		PugiXmlNode item_element = data_element.child("reward_item");
		if (item_element.empty())
		{
			return -4;
		}

		if (!cfg.reward_item.ReadConfig(item_element))
		{
			return -5;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const JinJieReturnConfig *RandActivityConfig::GetJinJieReturn2Cfg(int theme, int seq)
{
	if ((theme <= JINJIE_ACT_THEME_TYPE_INVALID || theme >= JINJIE_ACT_THEME_TYPE_MAX) || (seq < 0 || seq >= RAND_ACTIVITY_JINJIE_RETURN_MAX_CFG))
	{
		return nullptr;
	}

	return &m_jinjie_return2_cfg[theme][seq];
}

int RandActivityConfig::InitCriticalStrikeDay2Config(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int act_type = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "act_type", act_type) || act_type <= JINJIE_ACT_THEME_TYPE_INVALID || act_type >= JINJIE_ACT_THEME_TYPE_MAX)
		{
			return -1;
		}

		CriticalStrikeDayConfig &cfg = m_critical_strike_day2_cfg[act_type];

		cfg.act_type = act_type;

		if (!PugiGetSubNodeValue(data_element, "baoji_rate", cfg.baoji_rate) || cfg.baoji_rate < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji_mul", cfg.baoji_mul) || cfg.baoji_mul <= 0)
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const int RandActivityConfig::GetCriticalStrikeDay2Mul(int type)
{
	int rand_num = RandomNum(100);
	for (int i = 1; i < static_cast<int>(static_array_size(m_critical_strike_day2_cfg)); ++i)
	{
		if (m_critical_strike_day2_cfg[i].act_type == type && rand_num <= m_critical_strike_day2_cfg[i].baoji_rate)
		{
			return m_critical_strike_day2_cfg[i].baoji_mul;
		}
	}

	return 1;
}

const int RandActivityConfig::GetCriticalStrikeDay2Rate(int type)
{
	for (int i = 1; i < static_cast<int>(static_array_size(m_critical_strike_day2_cfg)); ++i)
	{
		if (m_critical_strike_day2_cfg[i].act_type == type)
		{
			return m_critical_strike_day2_cfg[i].baoji_rate;
		}
	}
	return 0;
}

int RandActivityConfig::InitCombineBuyDiscountCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -200000;
	}

	int open_server = 0;
	int last_open_server = 0;
	int stage_num = -1;
	m_combine_buy_discount_cfg_list_count = 0;
	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "opengame_day", open_server) || open_server <= 0)
		{
			return -1;
		}

		if (open_server != last_open_server)
		{
			++stage_num;

			++m_combine_buy_discount_cfg_list_count;
		}
		last_open_server = open_server;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -2;
		}

		RandActivityCombineBuyDiscountCfg &cfg = m_combine_buy_discount_cfg_list[stage_num];
		cfg.opengame_day = open_server;

		int item_count = 0;
		if (!PugiGetSubNodeValue(dataElement, "item_count", item_count) || item_count <= 0 || item_count > RA_COMBINE_BUY_BUCKET_ITEM_COUNT)
		{
			return -3;
		}

		int discount = 0;
		if (!PugiGetSubNodeValue(dataElement, "discount", discount) || discount <= 0 || discount > 100)		//配置里配成百分比
		{
			return -4;
		}

		if (!cfg.item_count_to_discount_cfg_map.emplace(item_count, discount).second)
		{
			return -5;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitCombineBuyItemCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -200000;
	}

	int open_server = 0;
	int last_open_server = 0;
	int stage_num = -1;
	m_combine_buy_item_cfg_list_count = 0;
	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "opengame_day", open_server) || open_server <= 0)
		{
			return -1;
		}

		if (open_server != last_open_server)
		{
			++stage_num;
			++m_combine_buy_item_cfg_list_count;
		}
		last_open_server = open_server;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -2;
		}

		int seq = -1;
		if (!PugiGetSubNodeValue(dataElement, "seq", seq) || seq < 0 || seq >= RA_COMBINE_BUY_MAX_ITEM_COUNT)
		{
			return -3;
		}

		m_combine_buy_Item_cfg_list[stage_num].opengame_day = open_server;
		RandActivityCombineBuyItemCfg::ItemCfg &cfg = m_combine_buy_Item_cfg_list[stage_num].item_cfg_list[seq];

		PugiXmlNode reward_item = dataElement.child("item");
		if (reward_item.empty())
		{
			return -4;
		}

		if (!cfg.item.ReadConfig(reward_item))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "price", cfg.price) || cfg.price <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "buy_limit", cfg.buy_limit) || cfg.buy_limit < 0 || cfg.buy_limit >= RA_COMBINE_BUY_LIMIT_BUY_ITEM_COUNT)
		{
			return -7;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitLuckyWishLotteryCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -200000;
	}

	int last_seq = -1;
	int opengame_day_type = 0;
	int opengame_day = 0;
	while (!dataElement.empty())
	{
		RandActivityLuckyWishLotteryCfg lottery_cfg;

		if (!PugiGetSubNodeValue(dataElement, "opengame_day", lottery_cfg.opengame_day) || lottery_cfg.opengame_day < 0)
		{
			return -10;
		}
		if (opengame_day > 0 && opengame_day != lottery_cfg.opengame_day)
		{
			++opengame_day_type;
			last_seq = -1;
		}
		opengame_day = lottery_cfg.opengame_day;

		if (opengame_day_type < 0 || opengame_day_type >= RAND_ACTIVITY_LUCKY_WISH_LOTTERY_CFG_COUNT)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "seq", lottery_cfg.seq) || lottery_cfg.seq < 0)
		{
			return -1;
		}
		if (last_seq + 1 != lottery_cfg.seq)
		{
			return -2;
		}
		last_seq = lottery_cfg.seq;
		
		if (!PugiGetSubNodeValue(dataElement, "weight", lottery_cfg.weight) || lottery_cfg.weight <= 0)
		{
			return -3;
		}
		
		if (!PugiGetSubNodeValue(dataElement, "is_rare", lottery_cfg.is_rare))
		{
			return -4;
		}

		PugiXmlNode reward_item = dataElement.child("item");
		if (reward_item.empty())
		{
			return -5;
		}

		if (!lottery_cfg.item.ReadConfig(reward_item))
		{
			return -6;
		}
		RandActivityLuckyWishLotteryAndTotalWeightCfg &cfg = m_lucky_wish_lottery_and_total_weight_cfg_list[opengame_day_type];
		cfg.total_weight += lottery_cfg.weight;
		
		int baodi = 0;
		if (PugiGetSubNodeValue(dataElement, "baodi", baodi) && baodi > 0)
		{
			if (cfg.baodi_map.size() > 0)
			{
				return -100;
			}
			cfg.baodi_map.insert(std::make_pair(baodi, lottery_cfg.seq));
			lottery_cfg.is_baodi = true;
		}

		cfg.lottery_vec.push_back(lottery_cfg);
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitLuckyWishLuckyCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -200000;
	}

	int last_day = 0;
	m_lucky_wish_lucky_cfg_vec.clear();

	while (!dataElement.empty())
	{
		RandActivityLuckyWishLuckyCfg cfg;
		if (!PugiGetSubNodeValue(dataElement, "opengame_day", cfg.opengame_day) || cfg.opengame_day <= 0)
		{
			return -1;
		}

		if (last_day + 1 != cfg.opengame_day)
		{
			return -2;
		}
		last_day = cfg.opengame_day;
		if (!PugiGetSubNodeValue(dataElement, "consume_gold_1", cfg.consume_gold_1) || cfg.consume_gold_1 < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "consume_gold_30", cfg.consume_gold_30) || cfg.consume_gold_30 < 0)
		{
			return -30;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_lucky_value", cfg.add_lucky_value) || cfg.add_lucky_value < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "lucky_min", cfg.lucky_min) || cfg.lucky_min < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "lucky_max", cfg.lucky_max) || cfg.lucky_max < 0)
		{
			return -6;
		}

		if (cfg.lucky_min > cfg.lucky_max)
		{
			return -7;
		}

		PugiXmlNode reward_item = dataElement.child("item");
		if (reward_item.empty())
		{
			return -8;
		}

		if (!cfg.item.ReadConfig(reward_item))
		{
			return -9;
		}

		m_lucky_wish_lucky_cfg_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}
	return 0;
}

int RandActivityConfig::GetCombineBuyDiscount(int item_count)
{
	const int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_COMBINE_BUY);
	for (int i = 0; i < m_combine_buy_discount_cfg_list_count && i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		const RandActivityCombineBuyDiscountCfg &cfg = m_combine_buy_discount_cfg_list[i];
		if (opengame_day <= cfg.opengame_day)
		{
			auto iter = cfg.item_count_to_discount_cfg_map.find(item_count);
			if (iter != cfg.item_count_to_discount_cfg_map.end())
			{
				return iter->second;
			}
		}
	}

	// 没有配置，意味着原价购买
	return 100;
}

const RandActivityCombineBuyItemCfg::ItemCfg * RandActivityConfig::GetCombineBuyItemCfg(int seq)
{
	if (seq < 0 || seq >= RA_COMBINE_BUY_MAX_ITEM_COUNT)
	{
		return nullptr;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return nullptr;
	}

	const int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_COMBINE_BUY);
	for (int i = 0; i < m_combine_buy_item_cfg_list_count && i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		const RandActivityCombineBuyItemCfg &cfg = m_combine_buy_Item_cfg_list[i];
		if (opengame_day <= cfg.opengame_day)
		{
			return &cfg.item_cfg_list[seq];
		}
	}

	return nullptr;
}

const std::vector<RandActivityLuckyWishLotteryCfg> * RandActivityConfig::GetLuckyWishLotteryCfgVec()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return nullptr;
	}
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex();
	for (int i = 0; i < RAND_ACTIVITY_LUCKY_WISH_LOTTERY_CFG_COUNT; ++i)
	{
		if (m_lucky_wish_lottery_and_total_weight_cfg_list[i].lottery_vec.size() == 0)
		{
			continue;
		}
		if (opengame_day < m_lucky_wish_lottery_and_total_weight_cfg_list[i].lottery_vec[0].opengame_day)
		{
			return &m_lucky_wish_lottery_and_total_weight_cfg_list[i].lottery_vec;
		}
	}
	return nullptr;
}

int RandActivityConfig::GetLuckyWishLotteryTotalWeight()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return 0;
	}
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex();
	for (int i = 0; i < RAND_ACTIVITY_LUCKY_WISH_LOTTERY_CFG_COUNT; ++i)
	{
		if (m_lucky_wish_lottery_and_total_weight_cfg_list[i].lottery_vec.size() == 0)
		{
			continue;
		}
		if (opengame_day < m_lucky_wish_lottery_and_total_weight_cfg_list[i].lottery_vec[0].opengame_day)
		{
			return m_lucky_wish_lottery_and_total_weight_cfg_list[i].total_weight;
		}
	}
	return 0;
}

const RandActivityLuckyWishLuckyCfg * RandActivityConfig::GetLuckyWishLuckyCfg()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return nullptr;
	}
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex();

	if (opengame_day < 0)
	{
		opengame_day = 0;
	}
	if (0 == m_lucky_wish_lucky_cfg_vec.size())
	{
		return nullptr;
	}
	if (opengame_day >= static_cast<int>(m_lucky_wish_lucky_cfg_vec.size()))
	{
		opengame_day %= m_lucky_wish_lucky_cfg_vec.size();
	}
	return &m_lucky_wish_lucky_cfg_vec[opengame_day];
}

const RandActivityLuckyWishLotteryCfg * RandActivityConfig::GetLuckyWishLotteryBaodiCfg(int server_chou_times) const
{
	if (server_chou_times <= 0) return nullptr;

	if (CrossConfig::Instance().IsHiddenServer())
	{
		return nullptr;
	}

	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex();

	if (opengame_day < 0)
	{
		opengame_day = 0;
	}

	for (int i = 0; i < RAND_ACTIVITY_LUCKY_WISH_LOTTERY_CFG_COUNT; ++i)
	{
		if (m_lucky_wish_lottery_and_total_weight_cfg_list[i].lottery_vec.size() == 0)
		{
			continue;
		}
		if (opengame_day < m_lucky_wish_lottery_and_total_weight_cfg_list[i].lottery_vec[0].opengame_day)
		{
			for (std::map<int, int>::const_iterator itr = m_lucky_wish_lottery_and_total_weight_cfg_list[i].baodi_map.begin(); itr != m_lucky_wish_lottery_and_total_weight_cfg_list[i].baodi_map.end(); ++itr)
			{
				int baodi_times = itr->first;
				int reward_seq = itr->second;
				if(baodi_times <= 0 ||  server_chou_times < baodi_times) continue;
				if (m_lucky_wish_lottery_and_total_weight_cfg_list[i].lottery_vec[reward_seq].seq == reward_seq)
				{
					return &m_lucky_wish_lottery_and_total_weight_cfg_list[i].lottery_vec[reward_seq];
				}
			}
		}
	}

	return nullptr;
}

int RandActivityConfig::InitImageCompetitionCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -200000;
	}

	while (!dataElement.empty())
	{
		RandActivityImageCompetition cfg;

		if (!PugiGetSubNodeValue(dataElement, "opengame_day", cfg.opengame_day) || cfg.opengame_day <= 0)
		{
			return -1;
		}

		if (cfg.opengame_day != m_image_competition_max_opengame_day + 1)
		{
			return -10000;
		}

		m_image_competition_max_opengame_day = cfg.opengame_day;

		if (!PugiGetSubNodeValue(dataElement, "competition_type", cfg.rank_system_type) || cfg.rank_system_type <= 0 || cfg.rank_system_type >= IMAGE_COMPETITION_TYPE_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "competition_id", cfg.rank_system_id) || cfg.rank_system_id <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "show_id", cfg.item_id) || nullptr == ITEMPOOL->GetItem(cfg.item_id))
		{
			return -4;
		}

		m_image_competition_cfg_vec.push_back(cfg);
		dataElement = dataElement.next_sibling();
	}

	if (m_image_competition_max_opengame_day <= 0)
	{
		return -20000;
	}

	return 0;
}

int RandActivityConfig::InitImageCompetitionRewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -200000;
	}

	int opengame_day = 0;
	int last_opengame_day = 0;
	int stage_num = -1;
	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -1;
		}

		if (opengame_day != last_opengame_day)
		{
			++stage_num;
		}

		last_opengame_day = opengame_day;

		if (stage_num < 0 || stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -2;
		}

		int reward_index = -1;
		if (!PugiGetSubNodeValue(dataElement, "reward_index", reward_index) || reward_index < 0 || reward_index >= RAND_ACTIVITY_IMAGE_COMPETITION_REWARD_CFG_COUNT)
		{
			return -3;
		}

		m_image_competition_reward_cfg[stage_num].opengame_day = opengame_day;
		RandActivityImageCompetitionReward::RewardCfg &cfg = m_image_competition_reward_cfg[stage_num].reward_cfg[reward_index];
		cfg.reward_index = reward_index;

		if (!PugiGetSubNodeValue(dataElement, "rank_pos", cfg.rank_pos) || cfg.rank_pos < 0)
		{
			return -4;
		}

		PugiXmlNode reward_item_list = dataElement.child("reward_item_list");
		if (reward_item_list.empty())
		{
			return -5;
		}

		PugiXmlNode reward_item = reward_item_list.child("reward_item");
		if (reward_item_list.empty())
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "limit_grade", cfg.limit_grade) || cfg.limit_grade < 0)
		{
			return -7;
		}

		cfg.reward_count = 0;
		while (!reward_item.empty())
		{
			if (cfg.reward_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -7;
			}

			if (!cfg.reward_list[cfg.reward_count].ReadConfig(reward_item))
			{
				return -8;
			}

			++cfg.reward_count;
			reward_item = reward_item.next_sibling();
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitDailyLoveRewardPercentCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -200000;
	}

	m_daily_love_reward_precent_vec.clear();
	int last_opengame_day = 0;
	while (!dataElement.empty())
	{
		RADailyLoveOpengameDayCfg cfg;
		if (!PugiGetSubNodeValue(dataElement, "opengame_day", cfg.opengame_day) || cfg.opengame_day <= 0)
		{
			return -1;
		}
		if (last_opengame_day >= cfg.opengame_day)
		{
			return -2;
		}
		last_opengame_day = cfg.opengame_day;
		if (!PugiGetSubNodeValue(dataElement, "gold_percent", cfg.gold_percent) || cfg.gold_percent < 0)
		{
			return -3;
		}
		m_daily_love_reward_precent_vec.push_back(cfg);
		
		dataElement = dataElement.next_sibling();
	}
	return 0;
}

int RandActivityConfig::InitLottery1RewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -200000;
	}

	while (!data_element.empty())
	{

		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -1;
		}

		RALottery1RewardCfg &reward_cfg = m_lottery_reward_map[opengame_day];
		RALottery1RewardItem item_cfg;
		RALottery1RewardWeight one_weight_cfg;
		RALottery1RewardWeight ten_weight_cfg;

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0)
		{
			return -3;
		}

		int weight = 0;
		if (!PugiGetSubNodeValue(data_element, "weight", weight) || weight < 0)
		{
			return -4;
		}

		int ten_weight = 0;
		if (!PugiGetSubNodeValue(data_element, "ten_weight", ten_weight) || ten_weight < 0)
		{
			return -5;
		}

// 		if (!PugiGetSubNodeValue(data_element, "is_record_history", item_cfg.is_record_history) || item_cfg.is_record_history < 0 || item_cfg.is_record_history > 1)
// 		{
// 			return -6;
// 		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", item_cfg.is_broadcast) || item_cfg.is_broadcast < 0 || item_cfg.is_broadcast > 1)
		{
			return -7;
		}

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -100;
		}

		if (!item_cfg.reward_item.ReadConfig(reward_item))
		{
			return -101;
		}

		item_cfg.seq = seq;

		one_weight_cfg.seq = seq;
		one_weight_cfg.weight = weight;

		ten_weight_cfg.seq = seq;
		ten_weight_cfg.weight = ten_weight;

		reward_cfg.reward_item.push_back(item_cfg);
		reward_cfg.one_lottery.push_back(one_weight_cfg);
		reward_cfg.ten_lottery.push_back(ten_weight_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitDailyLove2Cfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int stage = -1;
	int opengame_day = 0;
	int last_opengame_day = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -1;
		}

		if (last_opengame_day != opengame_day)
		{
			++stage;
			last_opengame_day = opengame_day;
		}

		if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -200;
		}

		m_daily_love_2_cfg_list[stage].opengame_day = opengame_day;
		if (!PugiGetSubNodeValue(data_element, "single_rebate_reward_precent", m_daily_love_2_cfg_list[stage].reward_gold_per) || m_daily_love_2_cfg_list[stage].reward_gold_per <= 0)
		{
			return -2;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitLottery1PersonRewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -200000;
	}

	while (!data_element.empty())
	{

		RALottery1PersonRewardCfg reward_cfg;
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -1;
		}
		reward_cfg.opengame_day = opengame_day;

		if (!PugiGetSubNodeValue(data_element, "seq", reward_cfg.seq) || reward_cfg.seq < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "person_chou_times", reward_cfg.person_chou_times) || reward_cfg.person_chou_times <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "vip_limit", reward_cfg.vip_limit) || reward_cfg.vip_limit < 0)
		{
			return -4;
		}

		reward_cfg.reward_count = ItemConfigData::ReadConfigList(data_element, "reward_item", reward_cfg.reward_item_list, MULTI_MAX_ATTACHMENT_ITEM_NUM);
		if (reward_cfg.reward_count < 0)
		{
			return -100 + reward_cfg.reward_count;
		}

		m_lottery_person_reward_map[opengame_day].push_back(reward_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitLottery1ServerRewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -200000;
	}

	while (!data_element.empty())
	{

		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -1;
		}

		int server_chou_times = 0;
		if (!PugiGetSubNodeValue(data_element, "server_chou_times", server_chou_times) || server_chou_times <= 0)
		{
			return -2;
		}

		std::map<int, RALottery1ServerRewardCfg> &server_reward_map = m_lottery_server_reward_map[opengame_day];
		RALottery1ServerRewardCfg &server_reward_cfg = server_reward_map[server_chou_times];

		server_reward_cfg.opengame_day = opengame_day;
		server_reward_cfg.server_chou_times = server_chou_times;

		RALottery1ServerRewardItem item_cfg;
		RALottery1ServerRewardWeight weight_cfg;

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "weight", weight_cfg.weight) || weight_cfg.weight <= 0)
		{
			return -4;
		}

// 		if (!PugiGetSubNodeValue(data_element, "is_record_history", item_cfg.is_record_history) || item_cfg.is_record_history < 0 || item_cfg.is_record_history > 1)
// 		{
// 			return -6;
// 		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", item_cfg.is_broadcast) || item_cfg.is_broadcast < 0 || item_cfg.is_broadcast > 1)
		{
			return -7;
		}

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -100;
		}

		if (!item_cfg.reward_item.ReadConfig(reward_item))
		{
			return -101;
		}

		item_cfg.seq = seq;
		weight_cfg.seq = seq;

		server_reward_cfg.reward_item.push_back(item_cfg);
		server_reward_cfg.lottery_weight.push_back(weight_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitZeroBuyReturnCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -200000;
	}
	
	int last_fecth_day = 0;
	m_zero_buy_return_map.clear();

	while (!data_element.empty())
	{
		int buy_type = 0;
		if (!PugiGetSubNodeValue(data_element, "buy_type", buy_type) || buy_type < 0 || buy_type >= RAND_ACTICITY_ZERO_BUY_RETURN_MAX_TYPE)
		{
			return -1;
		}

		auto cfg_it = m_zero_buy_return_map.find(buy_type);
		if (m_zero_buy_return_map.end() == cfg_it)  // 读取第一个
		{
			RAZeroBuyReturnConfig &cfg = m_zero_buy_return_map[buy_type];

			if (!PugiGetSubNodeValue(data_element, "need_gold", cfg.need_gold) || cfg.need_gold < 0)
			{
				return -100;
			}

			// 读取奖励
			PugiXmlNode reward_item_element = data_element.child("buy_reward");
			if (reward_item_element.empty())
			{
				return -1000;
			}

			if (!cfg.buy_reward.ReadConfig(reward_item_element))
			{
				return -101;
			}

			int fecth_day = -1;
			if (!PugiGetSubNodeValue(data_element, "fecth_day", fecth_day) || (0 != fecth_day))  // fetch_day 必须是0
			{
				return -102;
			}

			if (!PugiGetSubNodeValue(data_element, "daily_reward_gold", cfg.daily_reward_gold[0]) || cfg.daily_reward_gold[0] <= 0)
			{
				return -103;
			}

			last_fecth_day = 0;  // 重置提取天数
		}
		else
		{
			RAZeroBuyReturnConfig &cfg = cfg_it->second;

			int fecth_day = 0;
			if (!PugiGetSubNodeValue(data_element, "fecth_day", fecth_day) || fecth_day < 0 || fecth_day >= RAND_ACTICITY_ZERO_BUY_RETURN_MAX_DAY)
			{
				return -200;
			}

			if (fecth_day != last_fecth_day + 1)
			{
				return -201;
			}
			last_fecth_day = fecth_day;

			if (!PugiGetSubNodeValue(data_element, "daily_reward_gold", cfg.daily_reward_gold[fecth_day]) || cfg.daily_reward_gold[fecth_day] <= 0)
			{
				return -202;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int RandActivityConfig::InitKuangHaiQingDianCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -200000;
	}

	int index = -1, index_check = -1;
	int item_cfg_num = 0;

	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "index", index) || index < 0)
		{
			return -1;
		}
		if (index - index_check != 1)
		{
			return -101; // index必须逐一递增
		}
		index_check = index;

		RandActivityKuangHaiCfg::ItemCfg& item_cfg = m_kuang_hai_cfg.item_cfg_list[index];
		item_cfg.index = index;

		if (!PugiGetSubNodeValue(dataElement, "task_type", item_cfg.task_type) || item_cfg.task_type <= 0 || item_cfg.task_type >= RA_KUANG_HAI_TASK_TYPE_MAX_NUM)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "task_arg", item_cfg.task_arg) || item_cfg.task_arg <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "per_score", item_cfg.per_score) || item_cfg.per_score <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_score", item_cfg.max_score) || item_cfg.max_score <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "open_level", item_cfg.open_level) || item_cfg.open_level <= 0)
		{
			return -6;
		}


		item_cfg_num++;
		if (item_cfg_num >= RA_KUANG_HAI_TASK_ITEM_MAX_NUM)
		{
			return -1000;
		}

		auto& task_vec = m_kuang_hai_cfg.id_task_map[item_cfg.task_type];
		task_vec.push_back(&item_cfg);

		dataElement = dataElement.next_sibling();
	}

	m_kuang_hai_cfg.item_num = item_cfg_num;

	return 0;
}

int RandActivityConfig::InitKuangHaiQingDianRewardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -200000;
	}

	int reward_seq = -1, reward_seq_check = -1;
	int item_cfg_num = 0;

	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "reward_seq", reward_seq) || reward_seq < 0)
		{
			return -1;
		}
		if (reward_seq - reward_seq_check != 1)
		{
			return -101; // index必须逐一递增
		}
		reward_seq_check = reward_seq;

		RandActivityKuangHaiRewardCfg::ItemCfg& item_cfg = m_kuang_hai_reward_cfg.item_cfg_list[reward_seq];
		item_cfg.reward_seq = reward_seq;

		if (!PugiGetSubNodeValue(dataElement, "need_score", item_cfg.need_score) || item_cfg.need_score <= 0)
		{
			return -2;
		}

		item_cfg.reward_num = ItemConfigData::ReadConfigList(dataElement, std::string("reward_item"), item_cfg.reward_list, RA_KUANG_HAI_REWARD_MAX_ITEM_NUM);
		if (item_cfg.reward_num < 0)
		{
			return -10 + item_cfg.reward_num;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_broadcast", item_cfg.is_broadcast) || item_cfg.is_broadcast < 0)
		{
			return -3;
		}

		item_cfg_num++;
		if (item_cfg_num > RA_KUANG_HAI_REWARD_LIST_MAX_NUM)
		{
			return -1000;
		}

		dataElement = dataElement.next_sibling();
	}

	m_kuang_hai_reward_cfg.item_num = item_cfg_num;

	return 0;
}

int RandActivityConfig::InitGiftHarvestRankRwardCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -200000;
	}

	int rank_item_num = 0;

	while (!data_element.empty())
	{
		if (rank_item_num >= GIFT_HARVEST_PERSON_RANK_MAX)
		{
			return -100;
		}

		GiftHarvestRankRewardCfg::ConfigItem &cfg_item = m_gift_harvest_rank_reward_cfg.rank_list[rank_item_num];

		if (!PugiGetSubNodeValue(data_element, "rank", cfg_item.rank) || cfg_item.rank != rank_item_num)
		{
			return -1;
		}

		cfg_item.num = ItemConfigData::ReadConfigList(data_element, std::string("reward_item"), cfg_item.reward_list, MAX_ATTACHMENT_ITEM_NUM);
		if (cfg_item.num < 0)
		{
			return -10 + cfg_item.num;
		}

		++rank_item_num;
		data_element = data_element.next_sibling();
	}

	m_gift_harvest_rank_reward_cfg.item_num = rank_item_num;
	return 0;
}

// 幸运云购
int RandActivityConfig::InitLuckyCloudBuyCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -200000;
	}

	int index = 0;
	int opengame_day = 0;
	int round_index = 0, round_index_check = -1;

	char reward_weight_str[50];
	memset(reward_weight_str, 0, sizeof(reward_weight_str));
	int opengame_day_check = opengame_day;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day < 0)
		{
			return -1;
		}
		if (opengame_day_check != opengame_day)
		{
			opengame_day_check = opengame_day;
			round_index_check = -1;
			index++;
		}

		// 不可超过 开服天数数量
		if (index < 0 || index >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -2000;
		}

		if (!PugiGetSubNodeValue(data_element, "round_index", round_index) || round_index < 0 || round_index >= LUCKY_CLOUD_BUY_ROUND_MAX_NUM)
		{
			return -2;
		}
		// 保证逐一递增
		if (round_index - round_index_check != 1)
		{
			return -200;
		}

		RandLuckyCloudBuyCfg &today_cfg = m_lucky_buy_list[index];
		today_cfg.round_num_tody++;
		if (today_cfg.round_num_tody > LUCKY_CLOUD_BUY_ROUND_MAX_NUM)
		{
			return -1000;
		}
		today_cfg.opengame_day = opengame_day;
		RandLuckyCloudBuyCfg::RoundItem& item_cfg = today_cfg.round_list[round_index];

		if (!PugiGetSubNodeValue(data_element, "seq", item_cfg.seq) || item_cfg.seq < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "sale_count", item_cfg.sale_count) || item_cfg.sale_count < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "need_gold", item_cfg.need_gold) || item_cfg.need_gold <= 0)
		{
			return -5;
		}

		int begin_time = 0;
		if (!PugiGetSubNodeValue(data_element, "begin_time", begin_time) || begin_time <= 0)
		{
			return -6;
		}
		item_cfg.begin_time_hour = int(begin_time / 100);
		item_cfg.begin_time_min = int(begin_time % 100);
		if (item_cfg.begin_time_hour >= 24 || item_cfg.begin_time_min >= 60)
		{
			return -61;
		}

		int end_time = 0;
		if (!PugiGetSubNodeValue(data_element, "end_time", end_time) || end_time <= 0)
		{
			return -7;
		}
		item_cfg.end_time_hour = int(end_time / 100);
		item_cfg.end_time_min = int(end_time % 100);
		if (item_cfg.end_time_hour >= 24 || item_cfg.end_time_min >= 60)
		{
			return -71;
		}


		if (!PugiGetSubNodeValue(data_element, "min_reward_weight", reward_weight_str))
		{
			return -8;
		}

		int reward_weight_count = 0;
		int total_reward_weight = 0;
		// 将字符串分割处理，转为权重值
		{
			int _begin = 0;
			int _end = 0;
			std::string reward_weight(reward_weight_str);
			_end = reward_weight.find_first_of(":", _begin);

			// 逐一分割
			while (_end != -1)
			{
				reward_weight_str[_end] = '\0';
				int weight = std::atoi(&reward_weight_str[_begin]);
				item_cfg.min_reward_weight_list[reward_weight_count++] = weight;
				_begin = ++_end;
				_end = reward_weight.find_first_of(":", _begin);
				total_reward_weight += weight;
			}

			// 最后一个数字
			int weight = std::atoi(&reward_weight_str[_begin]);
			item_cfg.min_reward_weight_list[reward_weight_count++] = weight;
			total_reward_weight += weight;

			if (reward_weight_count > LUCKY_CLOUD_BUY_MIN_REWARD_MAX_NUM)
			{
				return -400;
			}
		}
		item_cfg.total_reward_weight = total_reward_weight;

		PugiXmlNode big_reward_item_element = data_element.child("big_reward_item");
		if (big_reward_item_element.empty())
		{
			return -9;
		}

		if (!item_cfg.big_reward_item.ReadConfig(big_reward_item_element))
		{
			return -10;
		}

		PugiXmlNode min_reward_item_list_element = data_element.child("min_reward_item_list");
		if (min_reward_item_list_element.empty())
		{
			return -11;
		}

		PugiXmlNode min_reward_item_element = min_reward_item_list_element.child("min_reward_item");
		if (min_reward_item_element.empty())
		{
			return -12;
		}

		int reward_count = 0;
		while (!min_reward_item_element.empty())
		{
			if (reward_count >= LUCKY_CLOUD_BUY_MIN_REWARD_MAX_NUM)
			{
				return -500;
			}

			if (!item_cfg.min_reward_item_list[reward_count].ReadConfig(min_reward_item_element))
			{
				return -600;
			}

			++reward_count;
			min_reward_item_element = min_reward_item_element.next_sibling();
		}

		// 确保权重的个数与奖励物品个数保持一致
		if (reward_count <= 0 || reward_weight_count != reward_count)
		{
			return -700;
		}

		item_cfg.min_reward_item_count = reward_count;

		data_element = data_element.next_sibling();
		round_index_check++;
	}

	return 0;
}

int RandActivityConfig::InitLuckyCloudBuyOtherCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -200000;
	}

	if (!PugiGetSubNodeValue(data_element, "buy_limit", m_lucky_buy_other.buy_limit) || m_lucky_buy_other.buy_limit < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "nolimit_time_hour", m_lucky_buy_other.nolimit_time_hour) || m_lucky_buy_other.nolimit_time_hour < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "nolimit_time_minute", m_lucky_buy_other.nolimit_time_minute) || m_lucky_buy_other.nolimit_time_minute < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "is_broadcast", m_lucky_buy_other.is_broadcast) || m_lucky_buy_other.is_broadcast < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "huge_reward_need_percent", m_lucky_buy_other.huge_reward_need_percent) || m_lucky_buy_other.huge_reward_need_percent < 0)
	{
		return -5;
	}

	m_lucky_buy_other.today_time_nolimit = m_lucky_buy_other.nolimit_time_hour * SECOND_PER_HOUR + m_lucky_buy_other.nolimit_time_minute * SECOND_PER_MIN;

	return 0;
}

int RandActivityConfig::InitFirstChargeTuanCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_refresh_time = 0;
	int last_opengame_day = 0;
	while (!data_element.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengame_day) || opengame_day <= 0 || opengame_day < last_opengame_day)
		{
			return -1;
		}

		if (opengame_day != last_opengame_day)
		{
			last_opengame_day = opengame_day;
			last_refresh_time = 0;
		}

		FirstChargeTuanCfg cfg_item;

		if (!PugiGetSubNodeValue(data_element, "show_time", cfg_item.refresh_time) || cfg_item.refresh_time < last_refresh_time)
		{
			return -2;
		}

		last_refresh_time = cfg_item.refresh_time;

		if (!PugiGetSubNodeValue(data_element, "num_people", cfg_item.fix_charge_num) || cfg_item.fix_charge_num <= 0)
		{
			return -3;
		}

		m_first_charge_tuan_refresh_timp_map[opengame_day].push_back(cfg_item);

		data_element = data_element.next_sibling();
	}

	return 0;
}

void RandActivityConfig::GetImageCompetitionRankInfo(int *rank_type, int *rank_id)
{
	if (rank_type == nullptr || rank_id == nullptr || m_image_competition_max_opengame_day <= 0)
	{
		return;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() % m_image_competition_max_opengame_day + 1;
	for (int i = 0; i < (int)m_image_competition_cfg_vec.size(); ++i)
	{
		if (opengame_day <= m_image_competition_cfg_vec[i].opengame_day)
		{
			*rank_type = m_image_competition_cfg_vec[i].rank_system_type;
			*rank_id = m_image_competition_cfg_vec[i].rank_system_id;
			break;
		}
	}

	return;
}

const RandActivityImageCompetition * RandActivityConfig::GetImageCompetitionStruct(int move_day)
{
	if (m_image_competition_max_opengame_day <= 0)
	{
		return nullptr;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return nullptr;
	}
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() % m_image_competition_max_opengame_day + 1 + move_day;
	for (int i = 0; i < (int)m_image_competition_cfg_vec.size(); ++i)
	{
		if (opengame_day <= m_image_competition_cfg_vec[i].opengame_day)
		{
			return &m_image_competition_cfg_vec[i];
		}
	}

	return nullptr;
}

const RandActivityImageCompetitionReward::RewardCfg *RandActivityConfig::GetCompetitionRankReward(int rank_pos, long long rank_value)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return nullptr;
	}
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	int stage = -1;
	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day <= m_image_competition_reward_cfg[i].opengame_day)
		{
			stage = i;
			break;
		}
	}

	if (stage < 0 || stage >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
	{
		return nullptr;
	}

	for (int i = 0; i < RAND_ACTIVITY_IMAGE_COMPETITION_REWARD_CFG_COUNT; ++i)
	{
		if (rank_pos <= m_image_competition_reward_cfg[stage].reward_cfg[i].rank_pos && rank_value >= m_image_competition_reward_cfg[stage].reward_cfg[i].limit_grade)
		{
			return &m_image_competition_reward_cfg[stage].reward_cfg[i];
		}
	}

	return nullptr;
}

const RALottery1RewardCfg * RandActivityConfig::GetLottery1RewardWeightCfg(int opengame_day)
{
	for (auto &it : m_lottery_reward_map)
	{
		if (opengame_day <= it.first)
		{
			return &it.second;
		}
	}

	return nullptr;
}

const RALottery1RewardItem * RandActivityConfig::GetLottery1RewardCfg(int opengame_day, int seq)
{
	for (auto &it : m_lottery_reward_map)
	{
		if (opengame_day <= it.first)
		{
			for (auto &vec_it : it.second.reward_item)
			{
				if (seq == vec_it.seq)
				{
					return &vec_it;
				}
			}
		}
	}

	return nullptr;
}

const RALottery1PersonRewardCfg * RandActivityConfig::GetLottery1PersonRewardCfg(int opengame_day, int seq)
{
	for (auto &it : m_lottery_person_reward_map)
	{
		if (opengame_day <= it.first)
		{
			for (auto &vec_it : it.second)
			{
				if (seq == vec_it.seq)
				{
					return &vec_it;
				}
			}
		}
	}

	return nullptr;
}

const RALottery1ServerRewardItem * RandActivityConfig::GetLottery1ServerRewardCfg(int opengame_day, int total_server_times)
{
	for (auto &it : m_lottery_server_reward_map)
	{
		if (opengame_day > it.first)
		{
			continue;
		}

		for (auto &server_tims_it : it.second)
		{
			if (server_tims_it.first <= 0)
			{
				continue;
			}

			// 判断是否达到全服次数
			bool is_hit_server_times = ( (total_server_times % server_tims_it.first) == 0 ? true : false);
			if (is_hit_server_times)
			{
				const RALottery1ServerRewardWeight *weight_cfg = gamecommon::RandListItem(server_tims_it.second.lottery_weight);
				if (nullptr == weight_cfg)
				{
					break;
				}

				for (auto &reward_it : server_tims_it.second.reward_item)
				{
					if (reward_it.seq == weight_cfg->seq)
					{
						return &reward_it;
					}
				}
			}
		}
	}

	return nullptr;
}

int RandActivityConfig::GetImageCompetitionOpengameDay()
{
	if (m_image_competition_max_opengame_day <= 0)
	{
		return 0;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return 0;
	}
	return SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() % m_image_competition_max_opengame_day + 1;
}

const RAZeroBuyReturnConfig *RandActivityConfig::GetRAZeroBuyReturnConfig(int buy_type)
{
	auto it = m_zero_buy_return_map.find(buy_type);
	if (m_zero_buy_return_map.end() == it)
	{
		return nullptr;
	}

	return &it->second;
}

const RandActivityDailyLoveConfig * RandActivityConfig::GetDailyLove2RewardPrecent()
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_DAILY_LOVE_2);

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day <= m_daily_love_2_cfg_list[i].opengame_day)
		{
			return &m_daily_love_2_cfg_list[i];
		}
	}

	return nullptr;
}

// 狂嗨庆典---------------------------------
int RandActivityConfig::GetKuangHaiTaskNum()
{
	return m_kuang_hai_cfg.item_num;
}

const RandActivityKuangHaiCfg::ItemCfg* RandActivityConfig::GetKuangHaiTaskItem(int index)
{
	if (index >= m_kuang_hai_cfg.item_num || index >= RA_KUANG_HAI_TASK_ITEM_MAX_NUM || index < 0)
	{
		return nullptr;
	}
	return &m_kuang_hai_cfg.item_cfg_list[index];
}

const std::vector<const RandActivityKuangHaiCfg::ItemCfg*>* RandActivityConfig::GetKuangHaiTaskAllItems(int task_id)
{
	if (task_id >= RA_KUANG_HAI_TASK_TYPE_MAX_NUM || task_id <= RA_KUANG_HAI_TASK_TYPE_BEGIN)
	{
		return nullptr;
	}

	auto task_vec = m_kuang_hai_cfg.id_task_map.find(task_id);
	if (task_vec != m_kuang_hai_cfg.id_task_map.end())
	{
		return &(task_vec->second);
	}

	return nullptr;
	//return &m_kuang_hai_cfg.id_task_map[task_id];
}

int RandActivityConfig::GetKuangHaiRewardListNum()
{
	return m_kuang_hai_reward_cfg.item_num;
}

int RandActivityConfig::GetKuangHaiRewardNum(int seq)
{
	if (seq >= m_kuang_hai_reward_cfg.item_num || seq >= RA_KUANG_HAI_REWARD_LIST_MAX_NUM || seq < 0)
	{
		return 0;
	}
	return m_kuang_hai_reward_cfg.item_cfg_list[seq].reward_num;
}

const RandActivityKuangHaiRewardCfg::ItemCfg* RandActivityConfig::GetKuangHaiRewardItem(int seq)
{
	if (seq >= m_kuang_hai_reward_cfg.item_num || seq >= RA_KUANG_HAI_REWARD_LIST_MAX_NUM || seq < 0)
	{
		return nullptr;
	}
	return &m_kuang_hai_reward_cfg.item_cfg_list[seq];
}

const GiftHarvestRankRewardCfg::ConfigItem * RandActivityConfig::GetGiftHarvestRankItem(int rank_index)
{
	if (rank_index >= GIFT_HARVEST_PERSON_RANK_MAX || rank_index < 0 || rank_index >= m_gift_harvest_rank_reward_cfg.item_num)
	{
		return nullptr;
	}

	return &m_gift_harvest_rank_reward_cfg.rank_list[rank_index];
}

// 幸运云购
const RandLuckyCloudBuyCfg::RoundItem* RandActivityConfig::GetLuckyCloudBuyCfg(int rounds_index, unsigned int join_timestamp)
{
	if (rounds_index < 0 || rounds_index >= LUCKY_CLOUD_BUY_ROUND_MAX_NUM)
	{
		return nullptr;
	}

	int opengame_day = 0;
	if (join_timestamp > 0)
	{
		opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), static_cast<time_t>(join_timestamp)) + 1;
	}
	else if (0 == join_timestamp)
	{
		opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_LUCKY_CLOUD_BUY);
	}

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_lucky_buy_list[i].opengame_day)
		{
			continue;
		}

		if (m_lucky_buy_list[i].round_num_tody > rounds_index)
		{
			return &m_lucky_buy_list[i].round_list[rounds_index];
		}
	}

	return nullptr;
}

int RandActivityConfig::GetLuckyBuyRoundsNum(unsigned int join_timestamp)
{
	int opengame_day = 0;
	if (join_timestamp > 0)
	{
		opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), static_cast<time_t>(join_timestamp)) + 1;
	}
	else if (0 == join_timestamp)
	{
		opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_LUCKY_CLOUD_BUY);
	}

	for (int i = 0; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_lucky_buy_list[i].opengame_day)
		{
			continue;
		}

		return m_lucky_buy_list[i].round_num_tody;
	}

	return 0;
}

bool RandActivityConfig::IsLuckyBuyUnlimitTime()
{
	auto now_tm = EngineAdapter::Instance().LocalTime();
	int today_time_now = now_tm->tm_hour * SECOND_PER_HOUR + now_tm->tm_min * SECOND_PER_MIN;

	if (today_time_now > m_lucky_buy_other.today_time_nolimit)
		return true;

	return false;
}

int RandActivityConfig::GetFirstChargeTuanRefreshTime()
{
	int opengame_day = 0;
	opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;

	unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(EngineAdapter::Instance().Time()));
	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	auto it = m_first_charge_tuan_refresh_timp_map.find(opengame_day);
	if (it != m_first_charge_tuan_refresh_timp_map.end())
	{
		for (const auto &iter : it->second)
		{
			if (today_zero_time + iter.refresh_time / 100 * 3600 + iter.refresh_time % 100 * 60 == today_zero_time && now_second == today_zero_time)
			{
				return today_zero_time;		//设置下次刷新时间
			}
			else if(now_second < today_zero_time + iter.refresh_time / 100 * 3600 + iter.refresh_time % 100 * 60)
			{
				return (today_zero_time + iter.refresh_time / 100 * 3600 + iter.refresh_time % 100 * 60);		//设置下次刷新时间
			}
		}
	}

	return -1;
}

int RandActivityConfig::GetFirstChargeTuanFixChargeTotalNum(int time)
{
	int opengame_day = 0;
	opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;

	auto it = m_first_charge_tuan_refresh_timp_map.find(opengame_day);
	if (it != m_first_charge_tuan_refresh_timp_map.end())
	{
		for (const auto &iter : it->second)
		{
			if (time == (iter.refresh_time / 100 * 3600 + iter.refresh_time % 100 * 60))
			{
				return iter.fix_charge_num;
			}
		}
	}

	return -1;
}

int RandActivityConfig::GetFirstChargeTuanFixNumOnInit()
{
	int opengame_day = 0;
	opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;

	unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(EngineAdapter::Instance().Time()));
	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	int fix_num = -1;

	auto it = m_first_charge_tuan_refresh_timp_map.find(opengame_day);
	if (it != m_first_charge_tuan_refresh_timp_map.end())
	{
		for (const auto &iter : it->second)
		{
			if (now_second >= today_zero_time + iter.refresh_time / 100 * 3600 + iter.refresh_time % 100 * 60)
			{
				fix_num = iter.fix_charge_num;
			}
		}
	}

	return fix_num;
}

int RandActivityConfig::InitCracyBuyCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (nullptr == data_element)
	{
		return -10000;
	}

	int index = -1;

	while (!data_element.empty())
	{
		int opengameday = 0;

		if (!PugiGetSubNodeValue(data_element, "opengame_day", opengameday) || opengameday <= 0)
		{
			return -1;
		}

		if (index == -1)
		{
			++index;
		}
		else if (m_cracy_buy[index].opengameday != opengameday)
		{
			++index;
		}

		if (index < 0 || index >= RAND_ACTIVITY_CRACY_BUY_MAX_OF_OPENGAMEDAY)
		{
			return -100;
		}

		RandActivityCrazyBuy &cracy_buy = m_cracy_buy[index];
		cracy_buy.opengameday = opengameday;
		int seq = -1;

		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0)
		{
			return -2;
		}

		PugiXmlNode reward_item = data_element.child("reward_item");
		if (reward_item.empty())
		{
			return -6;
		}

		if (!cracy_buy.content[seq].reward_item.ReadConfig(reward_item))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "level", cracy_buy.content[seq].level) || cracy_buy.content[seq].level < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "gold_buy", cracy_buy.content[seq].gold_buy) || cracy_buy.content[seq].gold_buy <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "gold_level", cracy_buy.content[seq].gold_level) || cracy_buy.content[seq].gold_level <= 0)
		{
			return -5;
		}

		int broadcast = 0;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", broadcast))
		{
			return -5;
		}
		cracy_buy.content[seq].is_broadcast = (1 == broadcast);

		if (!PugiGetSubNodeValue(data_element, "server_buy_times_limit", cracy_buy.content[seq].server_buy_times_limit) || cracy_buy.content[seq].server_buy_times_limit <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "role_buy_times_limit", cracy_buy.content[seq].role_buy_times_limit) || cracy_buy.content[seq].role_buy_times_limit <= 0)
		{
			return -5;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const RandActivityCrazyBuy::ContentCfg * RandActivityConfig::GetCracyBuyCfg(int seq)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CRACYBUY);

	if (seq < 0 || seq >= RAND_ACTIVITY_CRACY_BUY_MAX_NUM_TO_OPENDAY)
	{
		return nullptr;
	}

	for (int i = 0; i < RAND_ACTIVITY_CRACY_BUY_MAX_OF_OPENGAMEDAY; ++i)
	{
		if (opengame_day > m_cracy_buy[i].opengameday)
		{
			continue;
		}

		return &m_cracy_buy[i].content[seq];
	}

	return nullptr;
}

int RandActivityConfig::GetCracyBuyLevelCfg(int chongzhi)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_CRACYBUY);

	int i = 0;
	for (; i < RAND_ACTIVITY_CRACY_BUY_MAX_OF_OPENGAMEDAY; ++i)
	{
		if (opengame_day > m_cracy_buy[i].opengameday)
		{
			continue;
		}

		break;
	}

	for (int index = RAND_ACTIVITY_CRACY_BUY_MAX_NUM_TO_OPENDAY - 1; index >= 0; --index)
	{
		if (m_cracy_buy[i].content[index].gold_level > 0 && m_cracy_buy[i].content[index].gold_level <= chongzhi)
		{
			return m_cracy_buy[i].content[index].level;
		}
	}

	return -1;
}

int RandActivityConfig::InitDoubleGetCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100;
	}

	int last_opengame_day = 0;
	int stage_num = 0;
	while (!dataElement.empty())
	{
		int opengame_day = 0;
		if (!PugiGetSubNodeValue(dataElement, "opengame_day", opengame_day) || opengame_day <= 0)
		{
			return -1;
		}
		if (opengame_day != last_opengame_day )
		{
			stage_num++;
		}
		last_opengame_day = opengame_day;

		if (stage_num >= MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM)
		{
			return -1001;
		}

		RandActivityDoubleGet & cfg = m_double_get_cfg_list[stage_num];
		cfg.opengame_day = opengame_day;

		RandActivityDoubleGet::ConfigItem & cfg_item = cfg.item_list[cfg.count];

		if (!PugiGetSubNodeValue(dataElement, "seq", cfg_item.seq) || cfg_item.seq != cfg.count)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "chongzhi_value", cfg_item.chongzhi_value) || cfg_item.chongzhi_value < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_gold", cfg_item.reward_gold) || cfg_item.reward_gold < 0)
		{
			return -4;
		}

		cfg.count++;
		if (cfg.count >= RAND_ACTIVITY_DOUBLE_GET_CFG_COUNT)
		{
			return -5;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const RandActivityDoubleGet::ConfigItem * RandActivityConfig::GetDoubleGetCfg(int gold_num, int flag)
{
	int opengame_day = this->GetIntervalOpenGameDay(RAND_ACTIVITY_TYPE_DOUBLE_GET);

	int i = 0;
	for (; i < MAX_RAND_ACTIVITY_DAY_OPENGAMEDAY_NUM; ++i)
	{
		if (opengame_day > m_double_get_cfg_list[i].opengame_day)
		{
			continue;
		}
		break;
	}

	for (int index = m_double_get_cfg_list[i].count - 1; index >= 0; --index)
	{
		if (gold_num >= m_double_get_cfg_list[i].item_list[index].chongzhi_value)
		{
			if ((flag & (1 << m_double_get_cfg_list[i].item_list[index].seq)) == 0)
			{
				return &m_double_get_cfg_list[i].item_list[index];
			}
		}
	}

	return NULL;
}