#include "otherglobalconfig.h"

#include "servercommon/configcommon.h"

#include "item/itempool.h"
#include "item/itembase.h"
#include "item/other/otheritem.hpp"
#include "item/itemgriddata.h"

#include "monster/monsterpool.h"
#include "task/taskpool.h"
#include "globalconfig/equipconfig.h"

#include "checkresourcecenter.hpp"
#include "servercommon/welfaredef.hpp"

namespace globalconfig
{
	OtherGlobalConfig::OtherGlobalConfig()
		: itemdrop_interval_owner(0), itemdrop_interval_delete(0), itemdrop_interval_delete_no_static_scene(0), itemdrop_length(0), monster_dead_delay(0), monster_follow_interval(0),
		bag_sortinorder_interval(0), fight_status_leave_interval_s(0), is_check_valid(true), max_move_speed(0), move_check_length(0), max_transport_length(0), max_gather_distance(0),
		max_npc_touch_distance(0), gm_switch(0), random_camp_itemid(0), random_camp_gold_bind(0), ignore_level_limit_chongzhi(0),
		knapsack_extend_cost_cfgcount(0), knapsack_extend_bag_item_id(0), knapsack_extend_storage_item_id(0), speaker_level(0), speaker_item_id(0), speaker_need_gold(0), cross_speaker_item_id(0), cross_speaker_need_gold(0),
		clear_mentality_cd_interval_per_gold(0), 
		rest_limit_level(0), rest_recovery_interval(0), rest_recovery_hp_per(0), rest_reward_interval(0),
		shuangxiu_add_per(0), shuangxiu_intimacy_interval(0), shuangxiu_intimacy_add(0), 
		namecolor_max_evil(0), namecolor_update_interval_s(0), namecolor_update_dec_evil(0), 
		realive_item_id(0),
		realive_recover_common_scene_percent(100), realive_recover_fb_percent(100),
		role_birth_task_id(0), team_today_intimacy_limit(0), team_intimacy_reward(0), intimacy_reward_monster_level(0),
		honour_init_value(0), kill_other_camp_get_honour_per(0), kill_other_camp_max_level_diff(0), double_honour_kill_count(0),
		sceneid_of_camp1(0), sceneid_of_camp2(0), sceneid_of_camp3(0),
		ctrl_effect_reduce_circle_time(0),
		max_nuqi(0), nuqi_atk_other(0), nuqi_atked(0), max_energy(0), give_energy_interval_s(0), give_energy_per_time(0), buy_max_energy(0),
		world_level_open_level(0), world_level_exp_add_percent_perlevel(0.0f), world_level_max_exp_add_percent(0),
		welfare_daily_find_days(0),
		jump_limit_level(0), jump_limit_times(0), jump_recover_time(0), reset_jump_pos_min_level(0),
		reset_name_item_id(0), speedup_hello_count_max(0), speedup_hello_timeout_s(0), speedup_hello_count_forbit_time_s(0),
		speedup_hello_timeout_forbit_time_s(0), speedup_hello_all_pack_percent_limit(0), speedup_hello_open_check(true), server_max_role_num(0), friend_bless_reward_max_count(0), friend_bless_reward_tili(0), friend_bless_max_count(0), first_change_avatar_reward_gold(0),
		world_talk_is_limit(0), world_talk_effect_msg_min_len(0), world_talk_torelate_max_count(0), world_talk_accumulate_count_reset_interval(0),
		fb_guwu_cost_gold(0), fb_guwu_cost_coin(0), fb_guwu_gongji_per(0), fb_guwu_time(0),
		trade_logintime_limit(0), publicsale_solt_out_time(0), publicsale_max_price(0), is_allow_gm_public_sale_buy(0)
	{
		memset(namecolor_reduce_exp_percent, 0, sizeof(namecolor_reduce_exp_percent));
		memset(namecolor_reduce_hp_per_percent, 0, sizeof(namecolor_reduce_hp_per_percent));
		memset(namecolor_reduce_gongji_per_percent, 0, sizeof(namecolor_reduce_gongji_per_percent));
		memset(namecolor_evil_list, 0, sizeof(namecolor_evil_list));
	
		memset(ctrl_effect_reduce_percent_list, 0, sizeof(ctrl_effect_reduce_percent_list));
		memset(shenjiang_list, 0, sizeof(shenjiang_list));

		memset(publicsale_once_tax_list, 0, sizeof(publicsale_once_tax_list));
	}

	bool OtherGlobalConfig::Init(const std::string& configname, std::string* err)
	{
		char errinfo[1024] = { 0 };
		int iRet = 0;

		TiXmlDocument document;
		if (configname == "" || !document.LoadFile(configname))
		{
			*err = configname + " Load fail: " + document.ErrorDesc();
			return false;
		}

		TiXmlElement *RootElement = document.RootElement();
		if (!RootElement)
		{
			*err = configname + ": xml node error in root";
			return false;
		}

		{
			// 通用配置
			TiXmlElement *common_element = RootElement->FirstChildElement("common_cfg");
			if (NULL == common_element)
			{
				*err = configname + ": no [common_cfg].";
				return false;
			}

			iRet = this->InitCommonCfg(common_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitCommonCfg failed %d", configname.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 加速配置
			TiXmlElement *speedup_element = RootElement->FirstChildElement("speedup_cfg");
			if (NULL == speedup_element)
			{
				*err = configname + ": no [speedup_cfg].";
				return false;
			}

			iRet = this->InitSpeedUpCfg(speedup_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitSpeedUpCfg failed %d", configname.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 聊天配置
			TiXmlElement *talk_element = RootElement->FirstChildElement("talk_cfg");
			if (NULL == talk_element)
			{
				*err = configname + ": no [talk_cfg].";
				return false;
			}

			iRet = this->InitTalkCfg(talk_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitTalkCfg failed %d", configname.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 背包配置
			TiXmlElement *bag_element = RootElement->FirstChildElement("bag_cfg");
			if (NULL == bag_element)
			{
				*err = configname + ": no [bag_cfg].";
				return false;
			}

			iRet = this->InitBagCfg(bag_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitBagCfg failed %d", configname.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 背包格子自增配置
			TiXmlElement *element = RootElement->FirstChildElement("knapsack_grid_auto_add_cfg");
			if (NULL == element)
			{
				*err = configname + ": no [knapsack_grid_auto_add_cfg].";
				return false;
			}

			iRet = this->InitKnapsackGridAutoAddCfg(element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitKnapsackGridAutoAddCfg failed %d", configname.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 红名配置
			TiXmlElement *red_name_element = RootElement->FirstChildElement("red_name_cfg");
			if (NULL == red_name_element)
			{
				*err = configname + ": no [red_name_cfg].";
				return false;
			}

			iRet = this->InitRedNameCfg(red_name_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitRedNameCfg failed %d", configname.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 好友数量配置
			TiXmlElement *friend_count_element = RootElement->FirstChildElement("friend_count");
			if (NULL == friend_count_element)
			{
				*err = configname + ": no [friend_count].";
				return false;
			}

			iRet = this->InitFriendCountCfg(friend_count_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitFriendCountCfg failed %d", configname.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 控制效果衰减配置
			TiXmlElement *buff_dec_element = RootElement->FirstChildElement("buff_dec");
			if (NULL == buff_dec_element)
			{
				*err = configname + ": no [buff_dec].";
				return false;
			}

			iRet = this->InitBuffDecCfg(buff_dec_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitBuffDecCfg failed %d", configname.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 好友buff配置
			TiXmlElement *friend_buff_element = RootElement->FirstChildElement("friend_buff");
			if (NULL == friend_buff_element)
			{
				*err = configname + ": no [friend_buff].";
				return false;
			}

			iRet = this->InitFriendBuffCfg(friend_buff_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitFriendBuffCfg failed %d", configname.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		{
			// 市场税率配置
			TiXmlElement *publicsale_tax_element = RootElement->FirstChildElement("publicsale_tax");
			if (NULL == publicsale_tax_element)
			{
				*err = configname + ": no [publicsale_tax].";
				return false;
			}

			iRet = this->InitPublicSaleTaxCfg(publicsale_tax_element);
			if (iRet)
			{
				sprintf(errinfo, "%s: InitPublicSaleTaxCfg failed %d", configname.c_str(), iRet);
				*err = errinfo;
				return false;
			}
		}

		return true;
	}

	int OtherGlobalConfig::InitCommonCfg(TiXmlElement *RootElement)
	{
		TiXmlElement *dataElement = RootElement->FirstChildElement("data");
		if (NULL == dataElement)
		{
			return -10000;
		}

		if (!GetSubNodeValue(dataElement, "friend_bless_max_count", friend_bless_max_count) || friend_bless_max_count < 0)
		{
			return -1;
		}

		if (!GetSubNodeValue(dataElement, "friend_bless_reward_max_count", friend_bless_reward_max_count) || friend_bless_reward_max_count < 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "friend_bless_reward_tili", friend_bless_reward_tili) || friend_bless_reward_tili < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "first_change_avatar_reward_gold", first_change_avatar_reward_gold) || first_change_avatar_reward_gold < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "server_max_role_num", server_max_role_num) || server_max_role_num < 1000)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "itemdrop_interval_owner", itemdrop_interval_owner))
		{
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "itemdrop_interval_delete", itemdrop_interval_delete))
		{
			return -7;
		}

		if (!GetSubNodeValue(dataElement, "itemdrop_interval_delete_no_static_scene", itemdrop_interval_delete_no_static_scene))
		{
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "itemdrop_length", itemdrop_length))
		{
			return -9;
		}

		if (!GetSubNodeValue(dataElement, "monster_dead_delay", monster_dead_delay))
		{
			return -10;
		}

		if (!GetSubNodeValue(dataElement, "monster_follow_interval", monster_follow_interval))
		{
			return -11;
		}

		if (!GetSubNodeValue(dataElement, "bag_sortinorder_interval", bag_sortinorder_interval))
		{
			return -12;
		}

		if (!GetSubNodeValue(dataElement, "fight_status_leave_interval_s", fight_status_leave_interval_s))
		{
			return -13;
		}

		if (!GetSubNodeValue(dataElement, "is_check_valid", is_check_valid))
		{
			return -14;
		}

		if (!GetSubNodeValue(dataElement, "max_move_speed", max_move_speed))
		{
			return -15;
		}

		if (!GetSubNodeValue(dataElement, "move_check_length", move_check_length))
		{
			return -16;
		}

		if (!GetSubNodeValue(dataElement, "max_transport_length", max_transport_length))
		{
			return -17;
		}

		if (!GetSubNodeValue(dataElement, "max_gather_distance", max_gather_distance) || max_gather_distance <= 0)
		{
			return -18;
		}

		if (!GetSubNodeValue(dataElement, "max_npc_touch_distance", max_npc_touch_distance) || max_npc_touch_distance <= 0)
		{
			return -19;
		}

		if (!GetSubNodeValue(dataElement, "gm_switch", gm_switch))
		{
			return -20;
		}

		if (!GetSubNodeValue(dataElement, "random_camp_gold_bind", random_camp_gold_bind) || random_camp_gold_bind <= 0)
		{
			return -21;
		}

		if (!GetSubNodeValue(dataElement, "extend_bag_item_id", knapsack_extend_bag_item_id) || NULL == ITEMPOOL->GetItem(knapsack_extend_bag_item_id))
		{
			return -22;
		}

		if (!GetSubNodeValue(dataElement, "extend_storage_item_id", knapsack_extend_storage_item_id) || NULL == ITEMPOOL->GetItem(knapsack_extend_storage_item_id))
		{
			return -23;
		}

		if (!GetSubNodeValue(dataElement, "clear_cd_interval_per_gold", clear_mentality_cd_interval_per_gold) || clear_mentality_cd_interval_per_gold <= 0)
		{
			return -24;
		}

		if (!GetSubNodeValue(dataElement, "rest_limit_level", rest_limit_level) || rest_limit_level <= 0)
		{
			return -25;
		}

		if (!GetSubNodeValue(dataElement, "rest_recovery_interval", rest_recovery_interval) || rest_recovery_interval <= 0)
		{
			return -26;
		}

		if (!GetSubNodeValue(dataElement, "rest_recovery_hp_per", rest_recovery_hp_per) || rest_recovery_hp_per <= 0)
		{
			return -27;
		}

		if (!GetSubNodeValue(dataElement, "rest_reward_interval", rest_reward_interval) || rest_reward_interval <= 0)
		{
			return -28;
		}

		if (!GetSubNodeValue(dataElement, "rest_shuangxiu_add_per", shuangxiu_add_per) || shuangxiu_add_per < 0 || shuangxiu_add_per > 100)
		{
			return -29;
		}

		if (!GetSubNodeValue(dataElement, "rest_intimacy_interval", shuangxiu_intimacy_interval) || shuangxiu_intimacy_interval <= 0)
		{
			return -30;
		}

		if (!GetSubNodeValue(dataElement, "rest_intimacy_add", shuangxiu_intimacy_add) || shuangxiu_intimacy_add < 0)
		{
			return -31;
		}

		if (!GetSubNodeValue(dataElement, "namecolor_max_evil", namecolor_max_evil) || namecolor_max_evil <= 0)
		{
			return -32;
		}
		if (!GetSubNodeValue(dataElement, "namecolor_update_interval_s", namecolor_update_interval_s) || namecolor_update_interval_s <= 0)
		{
			return -33;
		}
		if (!GetSubNodeValue(dataElement, "namecolor_update_dec_evil", namecolor_update_dec_evil) || namecolor_update_dec_evil <= 0)
		{
			return -34;
		}

		if (!GetSubNodeValue(dataElement, "realive_item_id", realive_item_id) || NULL == ITEMPOOL->GetItem(realive_item_id))
		{
			return -35;
		}

		if (!GetSubNodeValue(dataElement, "realive_recover_common_scene_percent", realive_recover_common_scene_percent) || realive_recover_common_scene_percent <= 0 || realive_recover_common_scene_percent > 100)
		{
			return -36;
		}

		if (!GetSubNodeValue(dataElement, "realive_recover_fb_percent", realive_recover_fb_percent) || realive_recover_fb_percent <= 0 || realive_recover_fb_percent > 100)
		{
			return -37;
		}

		if (!GetSubNodeValue(dataElement, "role_birth_task_id", role_birth_task_id) || role_birth_task_id <= 0)
		{
			return -38;
		}
		CheckResourceCenter::Instance().GetTaskCheck()->Add(role_birth_task_id, true);

		if (!GetSubNodeValue(dataElement, "jump_limit_level", jump_limit_level))
		{
			return -39;
		}

		if (!GetSubNodeValue(dataElement, "jump_limit_times", jump_limit_times))
		{
			return -40;
		}

		if (!GetSubNodeValue(dataElement, "jump_recover_time", jump_recover_time))
		{
			return -41;
		}

		if (!GetSubNodeValue(dataElement, "reset_jump_pos_min_level", reset_jump_pos_min_level))
		{
			return -42;
		}

		if (!GetSubNodeValue(dataElement, "trade_logintime_limit", trade_logintime_limit) || trade_logintime_limit < 0)
		{
			return -43;
		}

		if (!GetSubNodeValue(dataElement, "publicsale_solt_out_time", publicsale_solt_out_time) || publicsale_solt_out_time <= 0)
		{
			return -44;
		}

		if (!GetSubNodeValue(dataElement, "publicsale_max_price", publicsale_max_price) || publicsale_max_price <= 0)
		{
			return -45;
		}

		if (!GetSubNodeValue(dataElement, "fb_guwu_cost_gold", fb_guwu_cost_gold) || fb_guwu_cost_gold < 0)
		{
			return -46;
		}

		if (!GetSubNodeValue(dataElement, "fb_guwu_cost_coin", fb_guwu_cost_coin) || fb_guwu_cost_coin <= 0)
		{
			return -47;
		}

		if (!GetSubNodeValue(dataElement, "fb_guwu_gongji_per", fb_guwu_gongji_per) || fb_guwu_gongji_per <= 0)
		{
			return -48;
		}

		if (!GetSubNodeValue(dataElement, "fb_guwu_time", fb_guwu_time) || fb_guwu_time <= 0)
		{
			return -49;
		}

		if (!GetSubNodeValue(dataElement, "max_nuqi", max_nuqi) || max_nuqi <= 0)
		{
			return -50;
		}

		if (!GetSubNodeValue(dataElement, "nuqi_atk_other", nuqi_atk_other) || nuqi_atk_other <= 0)
		{
			return -51;
		}

		if (!GetSubNodeValue(dataElement, "nuqi_atked", nuqi_atked) || nuqi_atked <= 0)
		{
			return -52;
		}

		if (!GetSubNodeValue(dataElement, "max_energy", max_energy) || max_energy <= 0)
		{
			return -53;
		}

		if (!GetSubNodeValue(dataElement, "give_energy_interval_s", give_energy_interval_s) || give_energy_interval_s <= 0)
		{
			return -54;
		}

		if (!GetSubNodeValue(dataElement, "give_energy_per_time", give_energy_per_time) || give_energy_per_time <= 0)
		{
			return -55;
		}

		if (!GetSubNodeValue(dataElement, "buy_max_energy", buy_max_energy) || buy_max_energy <= 0)
		{
			return -56;
		}

		if (!GetSubNodeValue(dataElement, "team_today_intimacy_limit", team_today_intimacy_limit) || team_today_intimacy_limit <= 0)
		{
			return -57;
		}
		if (!GetSubNodeValue(dataElement, "team_intimacy_reward", team_intimacy_reward) || team_intimacy_reward <= 0)
		{
			return -58;
		}
		if (!GetSubNodeValue(dataElement, "intimacy_reward_monster_level", intimacy_reward_monster_level) || intimacy_reward_monster_level <= 0)
		{
			return -59;
		}

		if (!GetSubNodeValue(dataElement, "camp_1_monsterid", shenjiang_list[CAMP_TYPE_FEIXING]) ||
			!MONSTERPOOL->IsMonsterExist(shenjiang_list[CAMP_TYPE_FEIXING]))
		{
			return -60;
		}

		if (!GetSubNodeValue(dataElement, "camp_2_monsterid", shenjiang_list[CAMP_TYPE_ZHURI]) ||
			!MONSTERPOOL->IsMonsterExist(shenjiang_list[CAMP_TYPE_ZHURI]))
		{
			return -61;
		}

		if (!GetSubNodeValue(dataElement, "camp_3_monsterid", shenjiang_list[CAMP_TYPE_ZHUIYUE]) ||
			!MONSTERPOOL->IsMonsterExist(shenjiang_list[CAMP_TYPE_ZHUIYUE]))
		{
			return -62;
		}

		if (!GetSubNodeValue(dataElement, "sceneid_of_camp1", sceneid_of_camp1) || sceneid_of_camp1 <= 0)
		{
			return -63;
		}

		if (!GetSubNodeValue(dataElement, "sceneid_of_camp2", sceneid_of_camp2) || sceneid_of_camp2 <= 0)
		{
			return -64;
		}

		if (!GetSubNodeValue(dataElement, "sceneid_of_camp3", sceneid_of_camp3) || sceneid_of_camp3 <= 0)
		{
			return -65;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(sceneid_of_camp1);
		CheckResourceCenter::Instance().GetSceneCheck()->Add(sceneid_of_camp2);
		CheckResourceCenter::Instance().GetSceneCheck()->Add(sceneid_of_camp3);

		if (!GetSubNodeValue(dataElement, "daily_find_days", welfare_daily_find_days) || welfare_daily_find_days <= 0 || welfare_daily_find_days > WELFARE_DAILY_FIND_MAXDAYS)
		{
			return -66;
		}

		if (!GetSubNodeValue(dataElement, "ctrl_effect_reduce_time", ctrl_effect_reduce_circle_time) || ctrl_effect_reduce_circle_time <= 0)
		{
			return -67;
		}

		if (!GetSubNodeValue(dataElement, "world_add_open_level", world_level_open_level) || world_level_open_level <= 0 || world_level_open_level > MAX_ROLE_LEVEL)
		{
			return -68;
		}

		if (!GetSubNodeValue(dataElement, "world_exp_add_percent_per_level", world_level_exp_add_percent_perlevel) || world_level_exp_add_percent_perlevel <= 0.0f)
		{
			return -69;
		}

		if (!GetSubNodeValue(dataElement, "world_max_exp_add_percent", world_level_max_exp_add_percent) || world_level_max_exp_add_percent <= 0)
		{
			return -70;
		}

		if (!GetSubNodeValue(dataElement, "honour_init_value", honour_init_value) || honour_init_value <= 0)
		{
			return -71;
		}

		if (!GetSubNodeValue(dataElement, "honour_per", kill_other_camp_get_honour_per) || kill_other_camp_get_honour_per <= 0)
		{
			return -72;
		}

		if (!GetSubNodeValue(dataElement, "max_level_diff", kill_other_camp_max_level_diff) || kill_other_camp_max_level_diff <= 0)
		{
			return -73;
		}

		if (!GetSubNodeValue(dataElement, "double_honour_kill_count", double_honour_kill_count) || double_honour_kill_count <= 0)
		{
			return -74;
		}

		if (!GetSubNodeValue(dataElement, "reset_name_item_id", reset_name_item_id) || NULL == ITEMPOOL->GetItem(reset_name_item_id))
		{
			return -75;
		}

		if (!GetSubNodeValue(dataElement, "is_allow_gm_public_sale_buy", is_allow_gm_public_sale_buy) || is_allow_gm_public_sale_buy < 0)
		{
			return -76;
		}

		return 0;
	}

	int OtherGlobalConfig::InitSpeedUpCfg(TiXmlElement *RootElement)
	{
		TiXmlElement *dataElement = RootElement->FirstChildElement("data");
		if (NULL == dataElement)
		{
			return -10000;
		}

		if (!GetSubNodeValue(dataElement, "speedup_hello_count_max", speedup_hello_count_max) || speedup_hello_count_max <= 0)
		{
			return -1;
		}
		if (!GetSubNodeValue(dataElement, "speedup_hello_timeout_s", speedup_hello_timeout_s))
		{
			return -2;
		}
		if (!GetSubNodeValue(dataElement, "speedup_hello_count_forbit_time_s", speedup_hello_count_forbit_time_s) || speedup_hello_count_forbit_time_s < 0)
		{
			return -3;
		}
		if (!GetSubNodeValue(dataElement, "speedup_hello_timeout_forbit_time_s", speedup_hello_timeout_forbit_time_s) || speedup_hello_timeout_forbit_time_s < 0)
		{
			return -4;
		}
		if (!GetSubNodeValue(dataElement, "speedup_hello_all_pack_percent_limit", speedup_hello_all_pack_percent_limit) || speedup_hello_all_pack_percent_limit < 100)
		{
			return -5;
		}
		int open_check = 0;
		if (!GetSubNodeValue(dataElement, "speedup_hello_open_check", open_check) || open_check < 0)
		{
			return -6;
		}
		speedup_hello_open_check = (0 != open_check);

		return 0;
	}

	int OtherGlobalConfig::InitTalkCfg(TiXmlElement *RootElement)
	{
		TiXmlElement *dataElement = RootElement->FirstChildElement("data");
		if (NULL == dataElement)
		{
			return -10000;
		}

		if (!GetSubNodeValue(dataElement, "is_limit", world_talk_is_limit) || world_talk_is_limit < 0)
		{
			return -1;
		}

		if (!GetSubNodeValue(dataElement, "effect_msg_min_len", world_talk_effect_msg_min_len) || world_talk_effect_msg_min_len < 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "torelate_max_count", world_talk_torelate_max_count) || world_talk_torelate_max_count < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(dataElement, "accumulate_count_reset_interval", world_talk_accumulate_count_reset_interval) || world_talk_accumulate_count_reset_interval < 0)
		{
			return -4;
		}

		if (!GetSubNodeValue(dataElement, "ignore_level_limit_chongzhi", ignore_level_limit_chongzhi) || ignore_level_limit_chongzhi < 0)
		{
			return -5;
		}

		if (!GetSubNodeValue(dataElement, "speaker_level", speaker_level) || speaker_level < 0)
		{
			return -6;
		}

		if (!GetSubNodeValue(dataElement, "speaker_item_id", speaker_item_id) || speaker_item_id <= 0)
		{
			return -7;
		}

		if (!GetSubNodeValue(dataElement, "speaker_need_gold", speaker_need_gold) || speaker_need_gold <= 0)
		{
			return -8;
		}

		if (!GetSubNodeValue(dataElement, "cross_speaker_item_id", cross_speaker_item_id) || cross_speaker_item_id <= 0)
		{
			return -9;
		}

		if (!GetSubNodeValue(dataElement, "cross_speaker_need_gold", cross_speaker_need_gold) || cross_speaker_need_gold <= 0)
		{
			return -10;
		}

		return 0;
	}

	int OtherGlobalConfig::InitBagCfg(TiXmlElement *RootElement)
	{
		TiXmlElement *dataElement = RootElement->FirstChildElement("data");
		if (NULL == dataElement)
		{
			return -10000;
		}

		knapsack_extend_cost_cfgcount = 0;
		int last_min_extend_index = -1;
		int last_min_extend_index_storage = -1;

		while (NULL != dataElement)
		{
			if (knapsack_extend_cost_cfgcount >= KNAPSACK_EXTEND_COST_CFG_MAX_COUNT)
			{
				return -1000;
			}

			KnapsackExtendCostCfgItem &cfg_item = knapsack_extend_cost_cfglist[knapsack_extend_cost_cfgcount];

			if (!GetSubNodeValue(dataElement, "min_extend_index", cfg_item.min_extend_index) || cfg_item.min_extend_index <= last_min_extend_index)
			{
				return -1;
			}
			last_min_extend_index = cfg_item.min_extend_index;

			if (!GetSubNodeValue(dataElement, "need_item_count", cfg_item.need_item_count) || cfg_item.need_item_count <= 0)
			{
				return -2;
			}

			if (!GetSubNodeValue(dataElement, "min_extend_index_storage", cfg_item.min_extend_index_storage) || cfg_item.min_extend_index_storage <= last_min_extend_index_storage)
			{
				return -3;
			}
			last_min_extend_index_storage = cfg_item.min_extend_index_storage;

			if (!GetSubNodeValue(dataElement, "need_item_count_storage", cfg_item.need_item_count_storage) || cfg_item.need_item_count_storage <= 0)
			{
				return -4;
			}

			++knapsack_extend_cost_cfgcount;
			dataElement = dataElement->NextSiblingElement();
		}

		if (0 != knapsack_extend_cost_cfglist[0].min_extend_index)
		{
			return -500;
		}

		if (0 != knapsack_extend_cost_cfglist[0].min_extend_index_storage)
		{
			return -501;
		}

		return 0;
	}

	int OtherGlobalConfig::InitKnapsackGridAutoAddCfg(TiXmlElement *RootElement)
	{
		TiXmlElement *dataElement = RootElement->FirstChildElement("data");
		if (NULL == dataElement)
		{
			return -10000;
		}

		int last_index = 0;
		int last_online_time = -1;
		while (NULL != dataElement)
		{
			int temp_index = 0;
			if (!GetSubNodeValue(dataElement, "grid_index", temp_index) || temp_index != last_index + 1 || temp_index >= ItemNamespace::MAX_KNAPSACK_GRID_NUM)
			{
				return -1;
			}

			int temp_online_time = 0;
			if (!GetSubNodeValue(dataElement, "online_time", temp_online_time) || temp_online_time < 0 || temp_online_time <= last_online_time)
			{
				return -2;
			}

			knapsack_grid_auto_add_time_vec.push_back(temp_online_time);
			++last_index;
			last_online_time = temp_online_time;
			dataElement = dataElement->NextSiblingElement();
		}

		return 0;
	}

	int OtherGlobalConfig::InitRedNameCfg(TiXmlElement *RootElement)
	{
		TiXmlElement *dataElement = RootElement->FirstChildElement("data");
		if (NULL == dataElement)
		{
			return -10000;
		}

		int evil_count = 0;
		while (dataElement)
		{
			if (evil_count >= NAME_COLOR_MAX)
			{
				return -1000;
			}

			if (!GetSubNodeValue(dataElement, "evil", namecolor_evil_list[evil_count]) || namecolor_evil_list[evil_count] < 0)
			{
				return -2;
			}

			if (!GetSubNodeValue(dataElement, "reduce_percent_hp", namecolor_reduce_hp_per_percent[evil_count]) || namecolor_reduce_hp_per_percent[evil_count] < 0)
			{
				return -3;
			}

			if (!GetSubNodeValue(dataElement, "reduce_percent_gongji", namecolor_reduce_gongji_per_percent[evil_count]) || namecolor_reduce_gongji_per_percent[evil_count] < 0)
			{
				return -4;
			}

			++evil_count;
			dataElement = dataElement->NextSiblingElement();
		}

		return 0;
	}

	int OtherGlobalConfig::InitFriendCountCfg(TiXmlElement *RootElement)
	{
		TiXmlElement *dataElement = RootElement->FirstChildElement("data");
		if (NULL == dataElement)
		{
			return -10000;
		}

		int count = 0;
		while (NULL != dataElement)
		{
			if (count >= FRIENDNUM_FOR_LEVEL_COUNT)
			{
				return -1000;
			}

			if (!GetSubNodeValue(dataElement, "level", friendnum_for_level[count].level) || friendnum_for_level[count].level <= 0 || friendnum_for_level[count].level > MAX_ROLE_LEVEL)
			{
				return -1;
			}

			if (!GetSubNodeValue(dataElement, "number", friendnum_for_level[count].number) || friendnum_for_level[count].number <= 0 || friendnum_for_level[count].number > MAX_FRIEND_NUM)
			{
				return -2;
			}

			if ((count > 0) && (friendnum_for_level[count].level < friendnum_for_level[count - 1].level || friendnum_for_level[count].number < friendnum_for_level[count - 1].number))
			{
				return -3;
			}

			++count;
			dataElement = dataElement->NextSiblingElement();
		}

		return 0;
	}

	int OtherGlobalConfig::InitBuffDecCfg(TiXmlElement *RootElement)
	{
		TiXmlElement *dataElement = RootElement->FirstChildElement("data");
		if (NULL == dataElement)
		{
			return -10000;
		}

		while (NULL != dataElement)
		{
			int count = -1;
			if (!GetSubNodeValue(dataElement, "count", count) || count <= 0 || count > MAX_CTRL_EFFECT_REDUCE_COUNT)
			{
				return -1;
			}

			if (!GetSubNodeValue(dataElement, "buff_per", ctrl_effect_reduce_percent_list[count]) || ctrl_effect_reduce_percent_list[count] < 0 || ctrl_effect_reduce_percent_list[count] > 100)
			{
				return -2;
			}

			dataElement = dataElement->NextSiblingElement();
		}

		return 0;
	}

	int OtherGlobalConfig::InitFriendBuffCfg(TiXmlElement *RootElement)
	{
		TiXmlElement *dataElement = RootElement->FirstChildElement("data");
		if (NULL == dataElement)
		{
			return -10000;
		}

		friend_attr_count = 0;
		// 好友亲密度属性加成
		int attr_item_count = 0;
		while (NULL != dataElement)
		{
			if (attr_item_count >= MAX_FRIEND_ATTR_ITEM_NUM)
			{
				return -1000;
			}

			FriendAttrItem *attr_item = &friend_attr_list[attr_item_count];

			if (!GetSubNodeValue(dataElement, "intimacy", attr_item->intimacy) || attr_item->intimacy <= 0)
			{
				return -1;
			}

			char temp_str[64] = { 0 };
			for (int i = 0; i < MAX_FRIEND_ATTR_TYPE_NUM; ++i)
			{
				sprintf(temp_str, "attr_type_%d", i);
				if (!GetSubNodeValue(dataElement, temp_str, attr_item->attr_list[i].attr_type) || !CharIntAttrs::IsFightAttr(attr_item->attr_list[i].attr_type, true))
				{
					return -100;
				}

				sprintf(temp_str, "attr_value_%d", i);
				if (!GetSubNodeValue(dataElement, temp_str, attr_item->attr_list[i].attr_value) || attr_item->attr_list[i].attr_value <= 0)
				{
					return -101;
				}
			}

			++attr_item_count;
			dataElement = dataElement->NextSiblingElement();
		}

		friend_attr_count = attr_item_count;

		return 0;
	}

	int OtherGlobalConfig::InitPublicSaleTaxCfg(TiXmlElement *RootElement)
	{
		TiXmlElement *dataElement = RootElement->FirstChildElement("data");
		if (NULL == dataElement)
		{
			return -10000;
		}

		int num = 0;
		while (NULL != dataElement)
		{
			if (num >= MAX_PUBLIC_SALE_TAX_NUM)
			{
				return -1000;
			}

			PublicsaleTaxCfg *tax_fg = &publicsale_once_tax_list[num];

			if (!GetSubNodeValue(dataElement, "gold", tax_fg->gold) || tax_fg->gold <= 0)
			{
				return -1;
			}

			if (!GetSubNodeValue(dataElement, "tax", tax_fg->tax) || tax_fg->tax <= 0 || tax_fg->tax >= 100)
			{
				return -2;
			}

			++num;
			dataElement = dataElement->NextSiblingElement();
		}

		return 0;
	}
}



