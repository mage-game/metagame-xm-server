#ifndef __WORLD_STATUS_PARAM_HPP__
#define __WORLD_STATUS_PARAM_HPP__

#include "common/tlvprotocol.h"
#include "servercommon/serverdef.h"
#include "servercommon/basedef.h"

#include "servercommon/activitydef.hpp"
#include "servercommon/rankdef.hpp"
#include "servercommon/vipdef.hpp"

#include "servercommon/marrydef.hpp"
#include "servercommon/challengefielddef.hpp"
#include "servercommon/worldeventdef.hpp"
#include "servercommon/campdef.hpp"
#include "servercommon/chatserverdef.h"
#include "servercommon/crossrandactivitydef.hpp"

static const int MAX_UPGRADE_RECORD_COUNT = 10;			// 最大进阶记录数
static const int MAX_CHONGJIHAOLI_RECORD_COUNT = 30;	// 冲级豪礼各个等级全服的领取记录最大数
static const int MAX_CHEST_SHOP_RECORD_COUNT = 30;		// 寻宝记录最大数

static const int MAX_SHENGXINGZHULI_SPECIAL_DAYS_COUNT = 7;	// 升星助力前7天要特殊处理

struct YsgcSytTopUserInfo
{
	YsgcSytTopUserInfo() 
		: quanfu_topscore(0), quanfu_topscore_uid(0)
	{
		memset(quanfu_topscore_name, 0, sizeof(quanfu_topscore_name));
	}

	int quanfu_topscore;
	int	quanfu_topscore_uid;
	GameName quanfu_topscore_name;
};

struct WorldStatusCommonDataParam
{
	WorldStatusCommonDataParam() { this->Reset(); }

	struct ChestRecord
	{
		ChestRecord() : uid(0), item_id(0), reserve(0)
		{}

		int uid;
		ItemID item_id;
		short reserve;
	};
	
	void Reset() 
	{
		buy_month_card_times = 0;
		xunbao_times = 0;
		
		memset(vip_level_role_num, 0, sizeof(vip_level_role_num));
		memset(reserve_int_list, 0, sizeof(reserve_int_list));

		gcz_chengzhu_id = 0;
		gcz_win_guild = INVALID_GUILD_ID;
		gcz_series_win = 0;

		last_reset_role_module_dayid = 0;
		last_reset_role_modlue_timestamp = 0;

		memset(oga_putonequipment_fetch_times, 0, sizeof(oga_putonequipment_fetch_times));
		memset(oga_capability_fetch_times, 0, sizeof(oga_capability_fetch_times));
		memset(oga_rolelevel_fetch_times, 0, sizeof(oga_rolelevel_fetch_times));

		memset(camp_toprank3_uid, 0, sizeof(camp_toprank3_uid));
		memset(camp_diaoxiang_status, 0, sizeof(camp_diaoxiang_status));

		today_chongzhi_role_count = 0;
		memset(total_upgrade_mount_record_list, 0, sizeof(total_upgrade_mount_record_list));
		memset(total_upgrade_holo_record_list, 0, sizeof(total_upgrade_holo_record_list));
		memset(total_upgrade_wing_record_list, 0, sizeof(total_upgrade_wing_record_list));
		memset(total_upgrade_shengong_record_list, 0, sizeof(total_upgrade_shengong_record_list));
		memset(total_upgrade_shenyi_record_list, 0, sizeof(total_upgrade_shenyi_record_list));

		new_cornucopia_total_value = 0;

		open_server_qunxian_uid = 0;
		open_server_guildbatte_uid = 0;
		open_server_gongchengzhan_uid = 0;
		open_server_territorywar_uid = 0;

		memset(total_chongjihaoli_record_list, 0, sizeof(total_chongjihaoli_record_list));

		guildbattle_mengzhu_guild_id = 0;

		famous_man_type_1_uid = 0;
		famous_man_type_2_uid = 0;
		famous_man_type_3_uid = 0;
		famous_man_type_4_uid = 0;
		famous_man_type_5_uid = 0;
		famous_man_type_6_uid = 0;

		free_create_guild_times = 0;
		role_create_count = 0;
		last_send_fake_recharge_msg_time = 0;
		charm_top1_title_uid = 0;

		yizhandaodi_win_user_id = 0;

		chestshop_index = 0;
		chestshop_index1 = 0;
		chestshop_index2 = 0;
		chestshop_jl_index = 0;

		memset(chestshop_record_list, 0, sizeof(chestshop_record_list));
		memset(chestshop_jl_record_list, 0, sizeof(chestshop_jl_record_list));

		marry_all_times = 0;
		marry_zhufu_count = 0;

		world_question_top1_title_uid = 0;
		gold_turntable_pool = 0;

		yesterday_chongzhi_role_count = 0;

		is_gongchengzhan_first = 0;

		total_join_darknight_role_level = 0;
		total_join_darknight_role_count = 0;
		total_join_darknight_average_level = 0;

		memset(qunxian_luandou_user_id_list, 0, sizeof(qunxian_luandou_user_id_list));

		qingyuan_cap_top_male_uid = 0;						// 情缘男战力第一
		qingyuan_cap_top_female_uid = 0;					// 情缘女战力第一
		baby_cap_top_male_uid = 0;							// 宝宝男战力第一
		baby_cap_top_female_uid = 0;						// 宝宝女战力第一
		little_pet_cap_rank_top_male_uid = 0;					// 小宠物战力男第一
		little_pet_cap_rank_top_female_uid = 0;					// 小宠物战力女第一
		
		memset(cross_fishing_top_rank_uid_list, 0, sizeof(cross_fishing_top_rank_uid_list));
		memset(cross_pasture_top_rank_uid_list, 0, sizeof(cross_pasture_top_rank_uid_list));

		yizhandaodi_second_user_id = 0;						// 一战到底第二名id
		yizhandaodi_third_user_id = 0;						// 一战到底第三名id

		reserve_int = 0;
		memset(chestshop_record3_list, 0, sizeof(chestshop_record3_list));
		chestshop_index3 = 0;

		chat_limit_condition_type_flag = 0;
		set_chat_limit_cond_type_flag = 0;
		is_forbid_audio_chat = 0;
		reserve_ch = 0;
		reserve_sh = 0;

		memset(channel_vip_level_limit_list, 0, sizeof(channel_vip_level_limit_list));
		memset(channel_level_limit_list, 0, sizeof(channel_level_limit_list));

		adjust_server_role_num = 0;
		kill_cross_boss_rank_mengzhu = 0;
		kill_corss_boss_rank_guild = 0;
		first_charge_fake_num = 0;
	}

	int buy_month_card_times;						// 月卡购买人数
	int xunbao_times;								// 寻宝次数

	UNSTD_STATIC_CHECK(20 == MAX_VIP_LEVEL);

	int vip_level_role_num[MAX_VIP_LEVEL + 1];		// 各个等级vip人数
	int reserve_int_list[1];						// 保留字节，预防以后vip等级增加

	int gcz_chengzhu_id;							// 攻城战城主id
	int gcz_win_guild;								// 攻城战胜利仙盟
	int gcz_series_win;								// 攻城战连胜次数

	unsigned int last_reset_role_module_dayid;		// 服务器角色最后重置数据的dayid
	unsigned int last_reset_role_modlue_timestamp;	// 服务器角色最后充值数据的时间
	
	YsgcSytTopUserInfo ysgc_topuserinfo;			// 妖兽广场最高积分
	YsgcSytTopUserInfo syt_topuserinfo;				// 锁妖塔最高积分
	
	unsigned short oga_putonequipment_fetch_times[OGA_PUTONEQUIPMENT_REWARD_MAX_SEQ];	// 全服收集装备领取奖励数量
	unsigned short oga_capability_fetch_times[OGA_CAPABILITY_REWARD_MAX_SEQ];			// 全服战力冲刺领取奖励数量
	unsigned short oga_rolelevel_fetch_times[OGA_ROLELEVEL_REWARD_MAX_SEQ];				// 全服等级冲刺领取奖励数量

	UNSTD_STATIC_CHECK(9 == MAX_CAMP_RECORD_RANK3_NUM);
	int camp_toprank3_uid[MAX_CAMP_RECORD_RANK3_NUM];									// 前3三名. 3个阵营
	int camp_diaoxiang_status[MAX_CAMP_RECORD_RANK3_NUM];								// 师门争夺战结果, 前3个雕象， 3个阵营 ((0.碎掉，1保留))

	int today_chongzhi_role_count;										// 今日首次充值人数
	int total_upgrade_mount_record_list[MAX_UPGRADE_RECORD_COUNT];		// 坐骑进阶人数记录
	int total_upgrade_holo_record_list[MAX_UPGRADE_RECORD_COUNT];		// 光环进阶人数记录
	int total_upgrade_wing_record_list[MAX_UPGRADE_RECORD_COUNT];		// 羽翼进阶人数记录
	int total_upgrade_shengong_record_list[MAX_UPGRADE_RECORD_COUNT];	// 神弓进阶人数记录
	int total_upgrade_shenyi_record_list[MAX_UPGRADE_RECORD_COUNT];		// 神翼进阶人数记录

	int new_cornucopia_total_value;

	int open_server_qunxian_uid;							// 开服争霸活动-元素战场
	int open_server_guildbatte_uid;							// 开服争霸活动-公会争霸
	int open_server_gongchengzhan_uid;						// 开服争霸活动-攻城战
	int open_server_territorywar_uid;						// 开服争霸活动-领土战

	int total_chongjihaoli_record_list[MAX_CHONGJIHAOLI_RECORD_COUNT];	// 冲级豪礼各个等级全服的领取记录

	GuildID guildbattle_mengzhu_guild_id;				//公会争霸霸主id

	int famous_man_type_1_uid;							// 名人堂-类型1
	int famous_man_type_2_uid;							// 名人堂-类型2
	int famous_man_type_3_uid;							// 名人堂-类型3
	int famous_man_type_4_uid;							// 名人堂-类型4
	int famous_man_type_5_uid;							// 名人堂-类型5
	int famous_man_type_6_uid;							// 名人堂-类型6

	int free_create_guild_times;						// 免费创帮总次数
	int role_create_count;								// 创建角色数
	unsigned int last_send_fake_recharge_msg_time;		// 最后一次发送假首冲传闻的时间
	int charm_top1_title_uid;							// 当天获得魅力值排名称号的角色uid

	int yizhandaodi_win_user_id;						// 一战到底第一名id

	unsigned char chestshop_index;
	unsigned char chestshop_index1;
	unsigned char chestshop_index2;
	unsigned char chestshop_jl_index;
	ChestRecord chestshop_record_list[MAX_CHEST_SHOP_RECORD_COUNT];			// 装备寻宝记录
	ChestRecord chestshop_record1_list[MAX_CHEST_SHOP_RECORD_COUNT];		// 装备寻宝1记录
	ChestRecord chestshop_record2_list[MAX_CHEST_SHOP_RECORD_COUNT];		// 装备寻宝2记录
	ChestRecord chestshop_jl_record_list[MAX_CHEST_SHOP_RECORD_COUNT];		// 精灵寻宝记录

	int marry_all_times;													// 结婚总次数
	int marry_zhufu_count;													// 结婚可祝福数量
	long long gold_turntable_pool;											// 元宝转盘

	int world_question_top1_title_uid;					// 当天世界答题排名第一

	int yesterday_chongzhi_role_count;					// 昨天充值人数

	unsigned int is_gongchengzhan_first;				// 已经首次攻城战（实际为否）

	int total_join_darknight_role_level;				// 跨服月黑风高当次活动参与玩家总等级
	short total_join_darknight_role_count;				// 跨服月黑风高当次活动参与玩家总人数
	short total_join_darknight_average_level;			// 跨服月黑风高玩家平均等级
	
	int qunxian_luandou_user_id_list[QUNXIANLUANDOU_SIDE_MAX];				// 群仙乱斗各边排名第一

	int qingyuan_cap_top_male_uid;						// 情缘男战力第一
	int qingyuan_cap_top_female_uid;					// 情缘女战力第一
	int baby_cap_top_male_uid;							// 宝宝男战力第一
	int baby_cap_top_female_uid;						// 宝宝女战力第一
	int little_pet_cap_rank_top_male_uid;				// 小宠物战力男第一
	int little_pet_cap_rank_top_female_uid;				// 小宠物战力女第一

	UNSTD_STATIC_CHECK(3 == CROSS_FISHING_TOP_RANK_COUNT_MAX);
	Int32 cross_fishing_top_rank_uid_list[CROSS_FISHING_TOP_RANK_COUNT_MAX];			// 跨服钓鱼活动前三名
	Int32 cross_pasture_top_rank_uid_list[CROSS_PASTURE_TOP_RANK_COUNT_MAX];			// 跨服牧场活动前三名

	int yizhandaodi_second_user_id;						// 一战到底第二名id
	int yizhandaodi_third_user_id;						// 一战到底第三名id
	unsigned int reserve_int;							// 保留

	ChestRecord chestshop_record3_list[MAX_CHEST_SHOP_RECORD_COUNT];		// 装备寻宝3记录
	int chestshop_index3;											// 装备寻宝3索引

	short chat_limit_condition_type_flag;							// 聊天限制条件类型
	short set_chat_limit_cond_type_flag;							// 是否后台设置过聊天限制条件类型
	char is_forbid_audio_chat;										// 是否禁止语音聊天
	char reserve_ch;
	short reserve_sh;

	UNSTD_STATIC_CHECK(8 == OPENLEVEL_LIMIT_TYPE_MAX);
	char channel_vip_level_limit_list[OPENLEVEL_LIMIT_TYPE_MAX];				// 频道vip等级限制
	unsigned short channel_level_limit_list[OPENLEVEL_LIMIT_TYPE_MAX];			// 频道等级限制
	unsigned int adjust_server_role_num;								// 动态导入玩家数
	int kill_cross_boss_rank_mengzhu;									// 击杀神域boss第一仙盟盟主技能
	int kill_corss_boss_rank_guild;										// 击杀神域boss第一仙盟
	int first_charge_fake_num;											// 首充团购假人数
};

struct BossSpecialRefreshParam
{
	struct RefreshItem
	{
		RefreshItem() : boss_id(0), refresh_interval(0) {}
		int boss_id;
		int refresh_interval;
	};

	static const int REFRESH_ITEM_MAX = 50;
	RefreshItem refresh_item_list[REFRESH_ITEM_MAX];
};

struct CrossGuildBattleGuildItem
{
	CrossGuildBattleGuildItem() { this->Reset(); }

	void Reset()
	{
		plat_type = 0;
		guild_id = 0;
		memset(guild_name, 0, sizeof(guild_name));
		memset(guild_tuanzhang_name, 0, sizeof(guild_tuanzhang_name));
	}

	int plat_type;
	int guild_id;
	GuildName guild_name;
	GuildName guild_tuanzhang_name;
};

struct CrossGuildBattleTuanzhangInfo
{
	CrossGuildBattleTuanzhangInfo() { this->Reset(); }

	void Reset()
	{
		tuanzhang_prof = 0;
		tuanzhang_sex = 0;

		reserve_sh = 0;
	}
	char tuanzhang_prof;
	char tuanzhang_sex;

	short reserve_sh;
};
struct CrossGuildBattleApperanceInfo //领主外观信息
{
	CrossGuildBattleApperanceInfo() { this->Reset(); }
	void Reset()
	{
		wuqi_id = 0;
		shizhuang_wuqi = 0;
		shizhuang_wuqi_is_special = 0;
		shizhuang_body = 0;
		shizhuang_body_is_special = 0;
		halo_used_imageid = 0;
		toushi_used_imageid = 0;
		qilinbi_used_imageid = 0;
		mask_used_imageid = 0;
		reserve_sh = 0;
	}
	ItemID wuqi_id;              // 武器ID
	char shizhuang_wuqi;          // 时装武器
	char shizhuang_wuqi_is_special;      // 时装武器
	char shizhuang_body;          // 时装身体
	char shizhuang_body_is_special;      // 时装身体
	short halo_used_imageid;        // 光环形象
	short toushi_used_imageid;        // 头饰形象id
	short qilinbi_used_imageid;        // 麒麟臂形象id
	short mask_used_imageid;        // 面具形象id
	short reserve_sh;
};
struct CrossGuildBattleParam
{
	CrossGuildBattleParam() { this->Reset(); }

	void Reset()
	{
		reward_calc_time = 0;
		average_level = 0;

		for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
		{
			cross_guildbattle_list[i].Reset();
			info_list[i].Reset();
			apperance_info_list[i].Reset();
		}
	}

	CrossGuildBattleGuildItem cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	int reward_calc_time;
	int average_level;
	CrossGuildBattleTuanzhangInfo info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	CrossGuildBattleApperanceInfo apperance_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
};

class WorldStatusParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	long long id_system;							// 仅仅初始化的时候有用 

	unsigned int start_time;
	unsigned int world_time;
	unsigned int last_update_dayid;
	int xianmengzhan_mengzhu_id;

	ActivityOpenRecords activity_open_records;

	WorldStatusCommonDataParam common_data;
	ChallengeFieldDataParam challenge_field_data;
	WorldEventParam world_event_param;

	CombineServerActivityRankParam csa_activity_rank_param;
	CombineServerActivityOtherParam csa_activity_other_param;

	BossSpecialRefreshParam boss_refresh_param;

	CrossGuildBattleParam cross_guildbattle_param;

	CombineServerActivityBossRankParam csa_boss_rank_param;

	CrossRandActivityParam cross_rand_activity_param;
	CrossRandActivityBigParam cross_rand_activity_big_param;
	CrossRandActivityBigParam2 cross_rand_activity_big_param2;
	CrossCloudPurchaseParam cross_cloudpurchase_param;
};

typedef char WorldStatusCommonDataParamHex[sizeof(WorldStatusCommonDataParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(WorldStatusCommonDataParamHex) < 4800);

typedef char ActivityOpenRecordsHex[sizeof(ActivityOpenRecords) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ActivityOpenRecordsHex) < 96);

typedef char TotalRoleProfCountListHex[sizeof(int) * 5 * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(TotalRoleProfCountListHex) < 64);

typedef char MarryReserveDataHex[sizeof(MarryReserveParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MarryReserveDataHex) < 5120);

typedef char WorldEventParamHex[sizeof(WorldEventParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(WorldEventParamHex) < 512);

typedef char CombineServerActivityRankParamHex[sizeof(CombineServerActivityRankParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CombineServerActivityRankParamHex) < 2048);

typedef char CombineServerActivityOtherParamHex[sizeof(CombineServerActivityOtherParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CombineServerActivityOtherParamHex) < 256);

typedef char BossSpecialRefreshParamHex[sizeof(BossSpecialRefreshParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(BossSpecialRefreshParamHex) < 1024);

typedef char CrossGuildBattleParamHex[sizeof(CrossGuildBattleParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CrossGuildBattleParamHex) < 2048);

typedef char CombineServerActivityBossRankParamHex[sizeof(CombineServerActivityBossRankParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CombineServerActivityBossRankParamHex) < 512);

typedef char CrossRandActivityParamHex[sizeof(CrossRandActivityParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CrossRandActivityParamHex) < 1024);

typedef char CrossRandActivityBigParamHex[sizeof(CrossRandActivityBigParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CrossRandActivityBigParamHex) < 1024);

typedef char CrossRandActivityBigParam2Hex[sizeof(CrossRandActivityBigParam2) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CrossRandActivityBigParam2Hex) < 4096);

typedef char CrossCloudPurchaseParamHex[sizeof(CrossCloudPurchaseParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CrossCloudPurchaseParamHex) < 30720);

#endif // __WORLD_STATUS_PARAM_HPP__

