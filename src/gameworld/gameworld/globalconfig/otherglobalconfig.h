#ifndef __OTHERGLOBALCONFIG_H__
#define __OTHERGLOBALCONFIG_H__

#include <string>
#include <vector>

#include "gamedef.h"

#include "servercommon/servercommon.h"
#include "servercommon/frienddef.hpp"
#include "servercommon/vipdef.hpp"
#include "servercommon/guilddef.hpp"

#include "other/chestshop/chestshop.hpp"

#include "obj/character/attribute.hpp"

#include "item/itemextern.hpp"
#include "servercommon/campdef.hpp"

namespace globalconfig
{
	/*
	全局配置
	itemdrop_interval_owner:自己掉落物品的拾取时间，秒
	itemdrop_interval_delete:掉落物品消失的时间，秒
	itemdrop_length:掉落物品拾取距离

	monster_dead_delay：怪物死亡延迟删除时间,毫秒
	monster_follow_interval: 怪物跟随间隔，毫秒

	bag_sortinorder_interval：整理背包最小时间间隔限制，秒

	fight_status_leave_interval:脱离战斗状态的时间，秒

	check_move_valid:用于检查客户端发送移动协议是否合法
	is_check_valid:是否进行防挂测试
	max_move_speed:游戏中可能的最大移动速度
	move_check_length:移动的允许非法距离，单位：格子

	max_transport_length：传送点可请求的最远距离，单位：格子

	max_gather_distance：角色与采集地点的最长距离，单位格子
	max_npc_touch_distance：角色与npc的最长距离，单位格子
	max_friend_num: 最大好友数

	server_max_role_num 服务器容纳的最大玩家数

	friend_bless_reward_max_count 好友祝福奖励次数
	friend_bless_reward_tili 好友祝福奖励体力
	first_change_avatar_reward_gold 第一次改变头像元宝奖励
	*/
	class OtherGlobalConfig
	{
	public:
		OtherGlobalConfig();
		~OtherGlobalConfig() {}

		bool Init(const std::string& path, std::string* err);

		unsigned int itemdrop_interval_owner;
		unsigned int itemdrop_interval_delete;
		unsigned int itemdrop_interval_delete_no_static_scene;
		unsigned int itemdrop_length;

		unsigned int monster_dead_delay;
		unsigned int monster_follow_interval;
		unsigned int bag_sortinorder_interval;
		unsigned int fight_status_leave_interval_s;
		bool is_check_valid;
		unsigned int max_move_speed;
		unsigned int move_check_length;
		unsigned int max_transport_length;

		unsigned int max_gather_distance;
		unsigned int max_npc_touch_distance;

		int gm_switch;			

		int random_camp_itemid;
		int random_camp_gold_bind;
		int ignore_level_limit_chongzhi;		// 充值X元宝后不限制聊天等级

		const static int KNAPSACK_EXTEND_COST_CFG_MAX_COUNT = 32;
		struct KnapsackExtendCostCfgItem
		{
			KnapsackExtendCostCfgItem() : min_extend_index(0), need_item_count(0),
			min_extend_index_storage(0), need_item_count_storage(0){}

			int min_extend_index;
			int need_item_count;
			int min_extend_index_storage;
			int need_item_count_storage;
		};

		int knapsack_extend_cost_cfgcount;
		KnapsackExtendCostCfgItem knapsack_extend_cost_cfglist[KNAPSACK_EXTEND_COST_CFG_MAX_COUNT];

		std::vector<unsigned int> knapsack_grid_auto_add_time_vec;		// 自动拓展格子配置

		ItemID knapsack_extend_bag_item_id;
		ItemID knapsack_extend_storage_item_id;

		int speaker_level;							// 喇叭开启等级
		int speaker_item_id;						// 本服喇叭消耗物品id
		int speaker_need_gold;						// 本服喇叭需要元宝
		int cross_speaker_item_id;					// 跨服喇叭消耗物品id
		int cross_speaker_need_gold;				// 跨服喇叭需要元宝

		int clear_mentality_cd_interval_per_gold;
		int gengu_baohufu_item_id;

		int rest_limit_level;
		int rest_recovery_interval;
		int rest_recovery_hp_per;
		int rest_reward_interval;

		int shuangxiu_add_per;
		int shuangxiu_intimacy_interval;
		int shuangxiu_intimacy_add;

		int namecolor_reduce_exp_percent[NAME_COLOR_MAX];
		int namecolor_reduce_hp_per_percent[NAME_COLOR_MAX];
		int namecolor_reduce_gongji_per_percent[NAME_COLOR_MAX];
		int namecolor_evil_list[NAME_COLOR_MAX];
		int namecolor_max_evil;
		int namecolor_update_interval_s;
		int namecolor_update_dec_evil;

		ItemID realive_item_id;
		int realive_recover_common_scene_percent;
		int realive_recover_fb_percent;

		TaskID role_birth_task_id;

		int team_today_intimacy_limit;
		int team_intimacy_reward;
		int intimacy_reward_monster_level;

		struct FriendNumForLevel
		{
			FriendNumForLevel() : level(0), number(0) {}

			int level;
			int number;
		};
		FriendNumForLevel friendnum_for_level[FRIENDNUM_FOR_LEVEL_COUNT];

		int honour_init_value;
		int kill_other_camp_get_honour_per;
		int kill_other_camp_max_level_diff;
		int double_honour_kill_count;
		
		int sceneid_of_camp1;
		int sceneid_of_camp2;
		int sceneid_of_camp3;

		static const int MAX_CTRL_EFFECT_REDUCE_COUNT = 10;			// 控制效果衰减的最大次数

		int ctrl_effect_reduce_circle_time;							// 控制效果衰减周期
		int ctrl_effect_reduce_percent_list[MAX_CTRL_EFFECT_REDUCE_COUNT + 1];

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		static const int MAX_FRIEND_ATTR_ITEM_NUM = 15;				// 好友buff配置数量
		static const int MAX_FRIEND_ATTR_TYPE_NUM = 3;				// 好友buff改变属性数量
		UNSTD_STATIC_CHECK(MAX_FRIEND_ATTR_TYPE_NUM <= 3);			// buff最多增加3个属性

		struct FriendAttrItem
		{
			FriendAttrItem() : intimacy(0), count(0) {}

			int intimacy;
			int count;
			CharIntAttrs::AttrPair attr_list[MAX_FRIEND_ATTR_TYPE_NUM];
		};

		int friend_attr_count;
		FriendAttrItem friend_attr_list[MAX_FRIEND_ATTR_ITEM_NUM];

		///////////////////////////////////////////////////////杀怪经验参数//////////////////////////////////////////////////////////
		static const int MAX_LEVEL_FACTOR_COUNT = 4;

		struct MonsterDeltaLevel
		{
			MonsterDeltaLevel() : delta_level(0), factor(0) {}

			int delta_level;
			double factor;
		};

		struct LevelFactor
		{
			LevelFactor() : role_level(0) {}

			int role_level;
			std::vector<MonsterDeltaLevel> monster_delta_level_vec;
		};

		std::vector<LevelFactor> m_low_level_factor_vec;
		std::vector<LevelFactor> m_high_level_factor_vec;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int max_nuqi;													// 怒气上限
		int nuqi_atk_other;												// 
		int nuqi_atked;													// 
		int max_energy;													// 体力上限
		int give_energy_interval_s;
		int give_energy_per_time;
		int buy_max_energy;												// 可购买的体力上限

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		int world_level_open_level;										// 世界等级加成开放等级
		double world_level_exp_add_percent_perlevel;						// 每差一级加成百分比
		int world_level_max_exp_add_percent;							// 世界等级经验最大加成比例

		int welfare_daily_find_days;									// 日常福利最大找回天数

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int jump_limit_level;
		int jump_limit_times;
		int jump_recover_time;
		int reset_jump_pos_min_level;

		//////////////////////////////////////////////////////////////////////////
		int shenjiang_list[CAMP_TYPE_MAX];

		//////////////////////////////////////////////////////////////////////////
		int reset_name_item_id;

		//////////////////////////////////////////////////////////////////////////
		int speedup_hello_count_max;			// 加速检测消息数量最大值
		int speedup_hello_timeout_s;			// 加速检测消息最大间隔
		int speedup_hello_count_forbit_time_s;	// 加速检测 消息数量不合理禁号时长
		int speedup_hello_timeout_forbit_time_s;// 加速检测 消息发送间隔超时禁号时长
		int speedup_hello_all_pack_percent_limit;// 加速检测 消息数量最大容忍百分比
		bool speedup_hello_open_check;			// 是否开启加速检测（总闸）

		//////////////////////////////////////////////////////////////////////////
		int server_max_role_num;				// 服务器容纳的最大玩家数 

		//////////////////////////////////////////////////////////////////////////
		int friend_bless_reward_max_count;		// 好友祝福奖励次数
		int friend_bless_reward_tili;			// 好友祝福奖励体力
		int friend_bless_max_count;				// 祝福好友的最大次数

		int first_change_avatar_reward_gold;	// 第一次改变头像奖励

		//////////////////////////////////////////////////////////////////////////
		int world_talk_is_limit;				// 世界聊天是否检测
		int world_talk_effect_msg_min_len;		// 世界聊天检测消息最小长度
		int world_talk_torelate_max_count;		// 世界聊天最多容忍相同消息数
		int world_talk_accumulate_count_reset_interval; // 世界聊天累计数清零时间间隔

		///////////////////////////////////////////////////////////////////////////副本鼓舞
		int fb_guwu_cost_gold;
		int fb_guwu_cost_coin;					
		int fb_guwu_gongji_per;
		int fb_guwu_time;

		///////////////////////////////////////////////////////////////////////////交易方案
		int trade_logintime_limit;
		unsigned int publicsale_solt_out_time;
		int publicsale_max_price;
		int is_allow_gm_public_sale_buy;						// 是否允许内玩市场购买

		static const int MAX_PUBLIC_SALE_TAX_NUM = 10;
		struct PublicsaleTaxCfg
		{	
			PublicsaleTaxCfg() : gold(0), tax(0) {}

			int gold;
			int tax;
		};

		PublicsaleTaxCfg publicsale_once_tax_list[MAX_PUBLIC_SALE_TAX_NUM];

		private:
			int InitCommonCfg(TiXmlElement *RootElement);
			int InitSpeedUpCfg(TiXmlElement *RootElement);
			int InitTalkCfg(TiXmlElement *RootElement);
			int InitBagCfg(TiXmlElement *RootElement);
			int InitKnapsackGridAutoAddCfg(TiXmlElement *RootElement);
			int InitRedNameCfg(TiXmlElement *RootElement);
			int InitFriendCountCfg(TiXmlElement *RootElement);
			int InitBuffDecCfg(TiXmlElement *RootElement);
			int InitFriendBuffCfg(TiXmlElement *RootElement);
			int InitPublicSaleTaxCfg(TiXmlElement *RootElement);
	};
};

#endif

