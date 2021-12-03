/******************************************************************************************
FileName: rankdef.hpp
Author: 
Description: 排行榜系统 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreanwang    2011/08/26/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __RANK_DEF_HPP__
#define __RANK_DEF_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/campdef.hpp"

static const int RANK_DATA_SAVE_TO_DB_TIME = 60;			 // 排行榜数据保存到DB的时间间隔

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const int PERSON_RANK_LEVEL_LIMIT = 40;				 // 上榜等级限制
static const int PERSON_RANK_UPDATE_TO_GLOBAL_TIME = 1 * 60; // 同步到排行榜的时间间隔 
static const int PERSON_RANK_UPDATE_TO_CROSS_TIME = 10 * 60; // 同步到跨服排行榜的时间间隔(同步主要依赖于切地图后Init之后2秒的同步. 这个做挂机玩家的同步)
static const int PERSON_RANK_SNAPSHOT_INTERVAL = 60;		 // 个人排行榜快照时间间隔 秒

// 个人榜类型  
enum PERSON_RANK_TYPE
{
	PERSON_RANK_TYPE_CAPABILITY_ALL = 0,						// 综合战力榜
	PERSON_RANK_TYPE_LEVEL = 1,									// 等级榜
	PERSON_RANK_TYPE_XIANNV_CAPABILITY = 2,						// 仙女战力榜
	PERSON_RANK_TYPE_EQUIP = 3,									// 装备战力榜
	PERSON_RANK_TYPE_ALL_CHARM = 4,								// 魅力总榜	
	PERSON_RANK_TYPE_CAPABILITY_CAMP_1 = 5,						// 阵营1综合战力榜
	PERSON_RANK_TYPE_CAPABILITY_CAMP_2 = 6,						// 阵营2综合战力榜
	PERSON_RANK_TYPE_CAPABILITY_CAMP_3 = 7,						// 阵营3综合战力榜
	PERSON_RANK_TYPE_MOUNT = 8,									// 坐骑阶数战力榜
	PERSON_RANK_TYPE_XIANJIAN = 9,								// 仙剑战力榜
	PERSON_RANK_TYPE_MENTALITY = 10,							// 修炼战力榜
	PERSON_RANK_TYPE_WING = 11,									// 羽翼阶数战力榜
	PERSON_RANK_TYPE_CHARM = 12,								// 魅力榜
	PERSON_RANK_TYPE_CHARM_MALE = 13,							// 男性魅力榜
	PERSON_RANK_TYPE_CHARM_FEMALE = 14,							// 女性魅力榜
	PERSON_RANK_TYPE_GONGCHENGZHAN = 15,						// 攻城战排行榜
	PERSON_RANK_TYPE_QUNXIANLUANDOU = 16,						// 群仙乱斗排行榜
	PERSON_RANK_TYPE_ZHANSHENDIAN = 17,							// 战神殿排行榜
	PERSON_RANK_TYPE_KILL_WORLDBOSS = 18,						// 击杀世界boss 
	PERSON_RANK_TYPE_RA_CHONGZHI = 19,							// 充值排行榜
	PERSON_RANK_TYPE_RA_CONSUME_GOLD = 20,						// 消费排行榜
	PERSON_RANK_TYPE_RED_PAPER_CONSUME_GOLD = 21,				// 红包土豪榜
	PERSON_RANK_TYPE_RED_PAPER_LUCKY_VALUE = 22,				// 红包幸运榜
	PERSON_RANK_TYPE_EVIL = 23,									// 罪恶榜

	PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP1 = 24,				// 阵营1历史总战力榜
	PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP2 = 25,				// 阵营2历史总战力榜
	PERSON_RANK_TYPE_HISTORY_TOTAL_CAP_CAMP3 = 26,				// 阵营3历史总战力榜

	PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP1 = 27,				// 阵营1历史装备战力榜
	PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP2 = 28,				// 阵营2历史装备战力榜
	PERSON_RANK_TYPE_HISTORY_EQUIP_CAP_CAMP3 = 29,				// 阵营3历史装备战力榜

	PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP1 = 30,				// 阵营1历史仙女战力榜
	PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP2 = 31,				// 阵营2历史仙女战力榜
	PERSON_RANK_TYPE_HISTORY_XIANNV_CAP_CAMP3 = 32,				// 阵营3历史仙女战力榜

	PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP1 = 33,			// 阵营1历史经脉战力榜
	PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP2 = 34,			// 阵营2历史经脉战力榜
	PERSON_RANK_TYPE_HISTORY_JINGMAI_CAP_CAMP3 = 35,			// 阵营3历史经脉战力榜

	PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP1 = 36,				// 阵营1历史坐骑战力榜
	PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP2 = 37,				// 阵营2历史坐骑战力榜
	PERSON_RANK_TYPE_HISTORY_MOUNT_CAP_CAMP3 = 38,				// 阵营3历史坐骑战力榜

	PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP1 = 39 ,				// 阵营1历史羽翼战力榜
	PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP2 = 40,				// 阵营2历史羽翼战力榜
	PERSON_RANK_TYPE_HISTORY_WING_CAP_CAMP3 = 41,				// 阵营3历史羽翼战力榜

	PERSON_RANK_TYPE_RA_DAY_CHONGZHI_NUM= 42,					// 随机活动每日充值
	PERSON_RANK_TYPE_RA_DAY_CONSUME_NUM = 43,					// 随机活动每日消费

	PERSON_RANK_TYPE_HALO = 44,									// 光环阶数战力榜
	PERSON_RANK_TYPE_SHENGONG = 45,								// 神弓阶数战力榜
	PERSON_RANK_TYPE_SHENYI = 46,								// 神翼阶数战力榜

	PERSON_RANK_TYPE_CAPABILITY_SHENZHUANG = 47,				// 神装战力榜
	PERSON_RANK_TYPE_CAPABILITY_JINGLING = 48,					// 精灵战力榜
	PERSON_RANK_TYPE_CAPABILITY_WASH = 49,						// 精炼战力榜

	PERSON_RANK_TYPE_EQUIP_STRENGTH_LEVEL = 50,					// 全身装备强化总等级榜
	PERSON_RANK_TYPE_STONE_TOATAL_LEVEL = 51,					// 全身宝石总等级榜
	
	PERSON_RANK_TYPE_FIGHT_MOUNT = 52,							// 战骑总等级榜
	PERSON_RANK_TYPE_WORLD_RIGHT_ANSWER = 53,					// 世界题目对题榜
	PERSON_RANK_TYPE_FIGHTING_CHALLENGE_SCORE = 54,				// 决斗场 - 挑衅积分排行
	PERSON_RANK_TYPE_DAY_CHARM = 55,							// 每日魅力榜
	PERSON_RANK_FISH_PLACE = 56,								// 钓鱼名次
	PERSON_RANK_STEAL_FISH_PLACE = 57,							// 偷鱼名次
	PERSON_RANK_TYPE_PLANTING_TREE_PLANTING = 58,				// 植树活动植树次数排行
	PERSON_RANK_TYPE_PLANTING_TREE_WATERING = 59,				// 植树活动浇水次数排行
	PERSON_RANK_CRYSTA_DALARAN_STEAL_NUM = 60,					// 达拉然水晶偷取榜
	PERSON_RANK_CRYSTA_DALARAN_BE_STEALED_NUM = 61,				// 达拉然水晶被偷榜

	PERSON_RANK_TYPE_FABAO = 62,								// 法宝阶数战力榜
	PERSON_RANK_TYPE_SHIZHUANG = 63,							// 时装阶数战力榜
	PERSON_RANK_TYPE_SHIZHUANG_WUQI = 64,						// 时装阶数武器（神兵）战力榜
	PERSON_RANK_TYPE_FOOTPRINT = 65,							// 足迹阶数战力榜
	PERSON_RANK_TYPE_RUNE_TOWER_LAYER = 67,						// 符文塔层数榜

	PERSON_RANK_TYPE_YAOSHI = 68,								// 腰饰阶数战力榜
	PERSON_RANK_TYPE_TOUSHI = 69,								// 头饰阶数战力榜
	PERSON_RANK_TYPE_QILINBI = 70,								// 麒麟臂阶数战力榜
	PERSON_RANK_TYPE_MASK = 71,									// 面具阶数战力榜

	PERSON_RANK_TYPE_LINGZHU = 72,                              // 灵珠阶数战力榜
	PERSON_RANK_TYPE_XIANBAO = 73,                              // 仙宝阶数战力榜
	PERSON_RANK_TYPE_LINGTONG = 74,                             // 灵童阶数战力榜
	PERSON_RANK_TYPE_LINGGONG = 75,                             // 灵弓阶数战力榜
	PERSON_RANK_TYPE_LINGQI = 76,                               // 灵骑阶数战力榜
	PERSON_RANK_TYPE_WEIYAN = 77,                               // 尾焰阶数战力榜
	PERSON_RANK_TYPE_SHOUHUAN = 78,                             // 手环阶数战力榜
	PERSON_RANK_TYPE_WEIBA = 79,                                // 尾巴阶数战力榜
	PERSON_RANK_TYPE_FEICHONG = 80,                             // 飞宠阶数战力榜

	PERSON_RANK_TYPE_CLOAK = 81,                                // 披风排行榜
	PERSON_RANK_TYPE_SHENBING = 82,                             // 灵刃排行榜

	PERSON_RANK_TYPE_ROLE_PATA_LAYER = 83,						// 个人爬塔本层数榜

	PERSON_RANK_TYPE_RA_PROFESS_MALE = 84,						// 表白排行男榜
	PERSON_RANK_TYPE_RA_PROFESS_FEMALE = 85,					// 表白排行女榜

	PERSON_RANK_TYPE_IMAGE_COMPETITION = 86,                    // 形象比拼

	PERSON_RANK_TYPE_RA_CHONGZHI2 = 87,							// 充值排行榜2

	PERSON_RANK_TYPE_MAX
};

static const int PERSON_RANK_MAX_SIZE = 100;				// 上榜人数100 存多20个 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 军团榜类型  
enum GUILD_RANK_TYPE
{
	GUILD_RANK_TYPE_LEVEL = 0,								// 军团等级榜
	GUILD_RANK_TYPE_XIANMENGZHAN,							// 仙盟战排行
	GUILD_RANK_TYPE_KILL_WORLD_BOSS,						// 击杀世界boss数量
	GUILD_RANK_TYPE_XIANMENG_BIPIN_KILL_WORLD_BOSS,			// 仙盟比拼期间击杀世界boss数量
	GUILD_RANK_TYPE_DAY_INCREASE_CAPABILITY,				// 仙盟每日增加战力
	GUILD_RANK_TYPE_CAPABILITY,								// 仙盟战力榜
	GUILD_RANK_TYPE_GUILDBATTLE,							// 公会争霸排行榜
	GUILD_RANK_TYPE_TERRITORYWAR,							// 领土战排行榜
	GUILD_RANK_TYPE_KILL_CROSS_BOSS,						// 击杀神域boss(神域+远古)

	GUILD_RANK_TYPE_MAX,
};

static const int GUILD_RANK_MAX_SIZE = 120;					// 上榜军团 存多20个
static const int GUILD_RANK_SNAPSHOT_INTERVAL = 60;			// 军团排行榜快照时间间隔 秒

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 组队榜类型
enum TEAM_RANK_TYPE
{
	TEAM_RANK_TYPE_EQUIP_FB = 0,							// 装备
	TEAM_RANK_TYPE_TOWERDEFEND,								// 塔防
	TEAM_RANK_TYPE_MAX
};

static const int TEAM_RANK_MAX_SIZE = 120;					// 上榜记录数 存多20个

static const int TEAM_RANK_SNAPSHOT_INTERVAL = 60;			// 组队排行榜快照时间间隔 秒

#pragma pack(push, 4)

struct TeamRankMemberInfo									// 组队排行队员信息 会用在协议中 注意对齐
{
	TeamRankMemberInfo() : uid(0), camp(CAMP_TYPE_INVALID) { memset(user_name, 0, sizeof(user_name)); }

	void Reset() { uid = 0; camp = CAMP_TYPE_INVALID; memset(user_name, 0, sizeof(user_name)); }

	int uid;												// 用户ID
	GameName user_name;										// 用户名
	int camp;												// 国家
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 前十名排行类型定义
enum RANK_TOP_TYPE
{
	RANK_TOP_TYPE_MIN = 0,

	RANK_TOP_TYPE_CAPABILITY = RANK_TOP_TYPE_MIN,		// 战力前10
	RANK_TOP_TYPE_LEVEL,								// 等级前10
	RANK_TOP_TYPE_XIANNV,								// 仙女前10
	RANK_TOP_TYPE_CAPABILITY_CAMP_1,					// 阵营1战力前10
	RANK_TOP_TYPE_CAPABILITY_CAMP_2,					// 阵营2战力前10
	RANK_TOP_TYPE_CAPABILITY_CAMP_3,					// 阵营3战力前10
	RANK_TOP_TYPE_CHARM_MALE,							// 男性魅力前10
	RANK_TOP_TYPE_CHARM_FEMALE,							// 女性魅力前10
	RANK_TOP_TYPE_QUNXIANLUANDOU,						// 三界战场前10
	RANK_TOP_TYPE_ZHANSHENDIAN,							// 战神殿前10
	RANK_TOP_TYPE_KILL_WORLDBOSS,						// 击杀世界boss前10
	RANK_TOP_TYPE_RED_PAPER_CONSUME_GOLD,				// 红包土豪前10
	RANK_TOP_TYPE_RED_PAPER_LUCKY_VALUE,				// 红包幸运前10
	RANK_TOP_TYPE_MOUNT,								// 坐骑战力
	RANK_TOP_TYPE_WING,									// 羽翼战力
	RANK_TOP_TYPE_HALO,									// 光环战力
	RANK_TOP_TYPE_SHENGONG,								// 神弓战力
	RANK_TOP_TYPE_SHENYI,								// 神翼战力
	RANK_TOP_TYPE_EQUIPT_STRENGTH_LEVEL,				// 全身装备强化总等级
	RANK_TOP_TYPE_STONE_TOTAL_LEVEL,					// 全身宝石总等级
	RANK_TOP_TYPE_FIGHT_MOUNT,							// 战骑战力
	RANK_TOP_TYPE_CHARM,								// 魅力榜
	RANK_TOP_TYPE_WORLD_RIGHT_ANSWER,					// 世界题目对题榜
	RANK_TOP_TYPE_JINGLING,								// 精灵战力
	RANK_TOP_TYPE_DAY_CHARM,							// 每日魅力榜
	RANK_TOP_TYPE_QILINBI,								// 麒麟臂战力
	RANK_TOP_TYPE_TOUSHI,								// 头饰战力
	RANK_TOP_TYPE_YAOSHI,								// 腰饰战力
	RANK_TOP_TYPE_LINGTONG,								// 灵童战力
	RANK_TOP_TYPE_LINGGONG,								// 灵弓战力
	RANK_TOP_TYPE_LINGQI,								// 灵骑战力
	RANK_TOP_TYPE_FlYPET,								// 飞宠战力
	RANK_TOP_TYPE_WEIYAN,								// 尾焰战力

	RANK_TOP_TYPE_MAX,
};

static const int TOP_TEN = 10;							// 前10名

struct TopUidList
{
	TopUidList() { this->Reset(); }	

	void Reset() { memset(uid_list, 0, sizeof(uid_list)); }

	int uid_list[TOP_TEN];
};

struct TopRankParam
{
	TopUidList all_top_rank_list[RANK_TOP_TYPE_MAX];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 跨服排行榜类型
enum CROSS_PERSON_RANK_TYPE
{
	CROSS_PERSON_RANK_TYPE_CAPABILITY_ALL = 0,							// 跨服战力榜
	CROSS_PERSON_RANK_TYPE_WEEK_ADD_CHARM = 1,							// 跨服魅力榜
	CROSS_PERSON_RANK_TYPE_XIULUO_TOWER = 2,							// 跨服修罗塔
	CROSS_PERSON_RANK_TYPE_1V1_SCORE = 3,								// 跨服1v1积分排行榜

	CROSS_PERSON_RANK_TYPE_3V3_SCORE = 4 ,								// 跨服3v3积分排行榜
	CROSS_PERSON_RANK_TYPE_ROLE_LEVEL = 5,								// 跨服等级排行榜
	CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY = 6,							// 跨服增加战力榜
	CROSS_PERSON_RANK_TYPE_ADD_CHARM = 7,								// 跨服增加魅力榜
	CROSS_PERSON_RANK_TYPE_GUILD_KILL_BOSS = 8,							// 跨服公会击杀榜

	CROSS_PERSON_RANK_TYPE_MAX,									
};

static const int CROSS_COUPLE_RANK_TYPE_BEGIN = 1000;

enum CROSS_COUPLE_RANK_TYPE
{
	CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP = 0,						// 跨服情缘榜
	CROSS_COUPLE_RANK_TYPE_MAX
};

static const int CROSS_COUPLE_RANK_MAX_SIZE = 120;			// 上榜人数100 存多20个 

static const int CROSS_RANK_UPDATE_TO_GLOBAL_TIME = 3 * 60; // 同步到排行榜的时间间隔 
static const int CROSS_RANK_SNAPSHOT_INTERVAL = 60;		    // 个人排行榜快照时间间隔 秒
static const int CROSS_PERSON_RANK_MAX_SIZE = 120;			// 上榜人数100 存多20个 

static const int TOP_HUNDRED = 100;							// 前一百名
static const int CROSS_RANK_WEEK_REWARD_MAX_ROLE_COUNT = 20;// 跨服排行榜每周奖励最大人数

enum CROSS_RANK_TOP_TYPE
{
	CROSS_RANK_TOP_TYPE_MIN = 0,

	CROSS_RANK_TOP_TYPE_CAPABILITY = CROSS_RANK_TOP_TYPE_MIN,		// 战力前一百
	CROSS_RANK_TOP_TYPE_XIULUO_TOWER,								// 跨服魅力榜
	CROSS_RANK_TOP_TYPE_1V1,										// 跨服1V1积分榜
	CROSS_RANK_TOP_TYPE_3V3,										// 跨服3V3积分榜

	CROSS_RANK_TOP_TYPE_MAX,
};

struct CrossPersonRankTitleUuidInfo
{
	CrossPersonRankTitleUuidInfo() : rank_type(-1), uuid_count(0) 
	{
		memset(rank_title_uuid_list, 0, sizeof(rank_title_uuid_list));
	}

	int rank_type;
	int uuid_count;
	long long rank_title_uuid_list[CROSS_PERSON_RANK_MAX_SIZE];
};

struct CrossPersonRankTitleParam
{
	CrossPersonRankTitleUuidInfo rank_title_uuid_info_list[CROSS_RANK_TOP_TYPE_MAX];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const int COUPLE_RANK_TYPE_MAX_COUNT = 8;

// 夫妻排行
enum  COUPLE_RANK_TYPE
{
	COUPLE_RANK_TYPE_MIN = 0,
	COUPLE_RANK_TYPE_QINGYUAN_CAP = COUPLE_RANK_TYPE_MIN,				// 夫妻情缘战力榜
	COUPLE_RANK_TYPE_BABY_CAP,											// 夫妻宝宝战力榜
	COUPLE_RANL_TYPE_LITTLE_PET,										// 夫妻小宠物战力榜
	COUPLE_RANK_TYPE_TOTAL_QINGYUAN_CAP,								// 夫妻情缘总战力榜(用于跨服排行榜)

	COUPLE_RANK_TYPE_MAX
};

UNSTD_STATIC_CHECK(COUPLE_RANK_TYPE_MAX_COUNT >= COUPLE_RANK_TYPE_MAX);

static const int COUPLE_RANK_MAX_SIZE = 120;							// 夫妻排行榜最大数
static const int COUPLE_RANK_TOP_COUNT = 10;							// 夫妻排行top 数
static const int COUPLE_RANK_SNASHOT_INTERVAL = 60;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

#endif  // __RANK_DEF_HPP__

