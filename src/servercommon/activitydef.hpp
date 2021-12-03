#ifndef __ACTIVITY_DEF_HPP__
#define __ACTIVITY_DEF_HPP__

#include <map>
#include <vector>
#include <set>
#include <deque>

#include "servercommon/servercommon.h"
#include "servercommon/guilddef.hpp"
#include "servercommon/struct/itemlistparam.h"

#include "campdef.hpp"
#include "servercommon/maildef.hpp"
#include "gameworld/gameworld/gamedef.h"

enum ACTIVITY_TYPE
{
	ACTIVITY_TYPE_INVALID = 0,

	ACTIVITY_TYPE_ZHUXIE = 1,												// 诛邪战场
	ACTIVITY_TYPE_RESERVE_2 = 2,											// 
	ACTIVITY_TYPE_HUSONG = 3,												// 护送活动	
	ACTIVITY_TYPE_MONSTER_INVADE = 4,										// 怪物入侵
	ACTIVITY_TYPE_QUNXIANLUANDOU = 5,										// 群仙乱斗
	ACTIVITY_TYPE_GONGCHENGZHAN = 6,										// 攻城战
	ACTIVITY_TYPE_XIANMENGZHAN = 7,                                         // 仙盟战
	ACTIVITY_TYPE_NATIONAL_BOSS = 8,										// 全民boss
	ACTIVITY_TYPE_YIZHANDAODI = 9,											// 一站到底
	ACTIVITY_TYPE_MOSHENJIANGLIN = 10,										// 魔神降临
	ACTIVITY_TYPE_CISHA_TASK = 11,											// 刺杀任务
	ACTIVITY_TYPE_GUAJI = 12,												// 挂机活动
	ACTIVITY_TYPE_WUXING_GUAJI = 13,										// 五行挂机
	ACTIVITY_TYPE_SHUIJING = 14,											// 水晶活动
	ACTIVITY_TYPE_HUANGCHENGHUIZHAN = 15,									// 皇城会战
	ACTIVITY_TYPE_CAMP_DEFEND1 = 16,										// 守卫雕像1
	ACTIVITY_TYPE_CAMP_DEFEND2 = 17,										// 守卫雕像2
	ACTIVITY_TYPE_CAMP_DEFEND3 = 18,										// 守卫雕像3
	ACTIVITY_TYPE_TERRITORY_WAR = 19,										// 领土战
	ACTIVITY_TYPE_TIANJIANGCAIBAO = 20,										// 天降财宝
	ACTIVITY_TYPE_GUILDBATTLE = 21,											// 公会争霸
	ACTIVITY_TYPE_HAPPYTREE_GROW_EXCHANGE = 22,								// 欢乐果树成长值兑换
	ACTIVITY_TYPE_NIGHT_FIGHT_FB = 23,										// 夜战王城											
	ACTIVITY_TYPE_GUILD_BOSS = 24,											// 公会boss
	ACTIVITY_TYPE_MILLIONAIRE = 25,											// 大富豪
	ACTIVITY_TYPE_WANGLINGEXPLORE = 26,										// 王陵探险
	ACTIVITY_TYPE_GUILD_YUNBIAO = 27,										// 仙盟运镖
	ACTIVITY_TYPE_XINGZUOYIJI = 28,											// 星座遗迹
	ACTIVITY_TYPE_WEDDING = 29,												// 婚礼
	ACTIVITY_TYPE_GUILD_QUESTION = 30,										// 帮派答题
	ACTIVITY_TYPE_MESS_BATTLE_FB = 31,										// 乱斗战场
	ACTIVITY_TYPE_WORSHIP = 32,												// 膜拜城主
	                                                                        // 33客户端 用作仙盟摇钱树
	ACTIVITY_TYPE_GUILD_FB = 34,                                            // 帮派试炼
	ACTIVITY_TYPE_GUILD_BONFIRE = 35,										// 仙盟篝火
	ACTIVITY_TYPE_MAX,
};

static const int SPECIAL_ACTIVITY_FREE_REALIVE_HP_PER = 70;					// 战场免费复活的血量百分比

enum SPECIAL_ACTIVITY_TYPE
{
	SPECIAL_ACTIVITY_TYPE_INVALID = 1024,

	SPECIAL_ACTIVITY_TYPE_OPENGAME,										// 开服活动
	SPECIAL_ACTIVITY_TYPE_CLOSEDBETA,									// 封测活动
	SPECIAL_ACTIVITY_TYPE_COMMON_ACTIVITY,								// 通用活动
	SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER,								// 合服活动

	SPECIAL_ACTIVITY_TYPE_RESVERED,
};

static const int SPECIAL_ACTIVITY_TYPE_MAX = SPECIAL_ACTIVITY_TYPE_RESVERED - SPECIAL_ACTIVITY_TYPE_INVALID;

// 随机活动
enum RAND_ACTIVITY_TYPE
{
	RAND_ACTIVITY_TYPE_BEGIN = 2048,
	RAND_ACTIVITY_TYPE_LUCKY_ROLL = RAND_ACTIVITY_TYPE_BEGIN,			// 幸运转盘
	RAND_ACTIVITY_TYPE_DAY_CHONGZHI_FANLI = 2049,						// 单日充值返利
	RAND_ACTIVITY_TYPE_DAY_CONSUME_GOLD = 2050,						    // 单日消费
	RAND_ACTIVITY_TYPE_TOTAL_CONSUME_GOLD = 2051,						// 累计消费
	RAND_ACTIVITY_TYPE_DAY_ACTIVIE_DEGREE = 2052,						// 单日活跃奖励
	RAND_ACTIVITY_TYPE_CHONGZHI_RANK = 2053,							// 充值排行
	RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK = 2054,						// 消费排行
	RAND_ACTIVITY_TYPE_SERVER_PANIC_BUY = 2055,							// 全服疯狂抢购
	RAND_ACTIVITY_TYPE_PERSONAL_PANIC_BUY = 2056,						// 个人疯狂抢购
	RAND_ACTIVITY_TYPE_CONSUME_GOLD_FANLI = 2057,						// 消费返利
	RAND_ACTIVITY_TYPE_EQUIP_STRENGTHEN = 2058,							// 装备强化
	RAND_ACTIVITY_TYPE_CHESTSHOP = 2059,								// 奇珍异宝（已删）
	RAND_ACTIVITY_TYPE_STONE_UPLEVEL = 2060,							// 宝石升级（已删）
	RAND_ACTIVITY_TYPE_XN_CHANMIAN_UPLEVEL = 2061,						// 仙女缠绵（已删）
	RAND_ACTIVITY_TYPE_MOUNT_UPGRADE = 2062,							// 坐骑进阶
	RAND_ACTIVITY_TYPE_QIBING_UPGRADE = 2063,							// 骑兵升级（已删）
	RAND_ACTIVITY_TYPE_MENTALITY_TOTAL_LEVEL = 2064,					// 根骨全身等级
	RAND_ACTIVITY_TYPE_WING_UPGRADE = 2065,								// 羽翼进化
	RAND_ACTIVITY_TYPE_QUANMIN_QIFU = 2066,								// 全民祈福（已删）
	RAND_ACTIVITY_TYPE_SHOUYOU_YUXIANG = 2067,							// 手有余香
	RAND_ACTIVITY_TYPE_XIANMENG_JUEQI = 2068,							// 仙盟崛起
	RAND_ACTIVITY_TYPE_XIANMENG_BIPIN = 2069,							// 仙盟比拼
	RAND_ACTIVITY_TYPE_DAY_ONLINE_GIFT = 2070,							// 每日在线好礼
	RAND_ACTIVITY_TYPE_KILL_BOSS = 2071,								// BOSS击杀
	RAND_ACTIVITY_TYPE_DOUFA_KUANGHUAN = 2072,							// 斗法狂欢
	RAND_ACTIVITY_TYPE_ZHANCHANG_FANBEI = 2073,							// 战场翻倍（已删）
	RAND_ACTIVITY_TYPE_LOGIN_GIFT = 2074,								// 登录奖励
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL = 2075,						// 比拼综合战力
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP = 2076,						// 比拼装备战力
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV = 2077,						// 比拼仙女战力(已删)
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI = 2078,					// 比拼经脉战力(已删)
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT = 2079,						// 比拼坐骑战力
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING = 2080,						// 比拼羽翼战力
	RAND_ACTIVITY_TYPE_CHARGE_REPAYMENT = 2081,							// 充值回馈
	RAND_ACTIVITY_TYPE_SINGLE_CHARGE = 2082,							// 单次充值
	RAND_ACTIVITY_TYPE_CORNUCOPIA = 2083,								// 聚宝盆（开服）
	RAND_ACTIVITY_TYPE_DAY_CHONGZHI_DOUBLE = 2084,						// 充值双倍奖励(已删)
	RAND_ACTIVITY_TYPE_DANBI_CHONGZHI = 2085,							// 单笔充值（不同天数奖励不同）
	RAND_ACTIVITY_TYPE_TOTAL_CHARGE_DAY = 2086,							// 累计充值（天数）活动
	RAND_ACTIVITY_TYPE_TOMORROW_REWARD = 2087,							// 次日福利活动
	RAND_ACTIVITY_TYPE_DAY_CHONGZHI_DOUBLE_2 = 2088,					// 狂送元宝(可配置返回百分比)
	RAND_ACTIVITY_TYPE_DAY_CHONGZHI_RANK = 2089,						// 每日充值排行榜
	RAND_ACTIVITY_TYPE_DAY_CONSUME_RANK = 2090,							// 每日消费排行榜
	RAND_ACTIVITY_TYPE_TOTAL_CHARGE = 2091,								// 累计充值
	RAND_ACTIVITY_TYPE_DOUBLE_SCORE = 2092,								// 双倍积分
	RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_EQUIP = 2093,				// 限时积分兑换稀有装备
	RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_JL = 2094,					// 限时积分兑换精灵
	RAND_ACTIVITY_TYPE_LEVEL_LOTTERY = 2095,							// 金银塔抽奖活动
	RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG = 2096,							// 充值扭蛋
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG = 2097,				// 比拼神装战力
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING = 2098,					// 比拼精灵战力
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH = 2099,						// 比拼精炼战力
	RAND_ACTIVITY_TYPE_ZHENBAOGE = 2100,								// 珍宝阁
	RAND_ACTIVITY_TYPE_MIJINGXUNBAO = 2101,								// 秘境寻宝
	RAND_ACTIVITY_TYPE_JISUCHPONGZHAN = 2102,							// 急速冲战
	RAND_ACTIVITY_TYPE_MONEY_TREE = 2103,								// 摇钱树活动
	RAND_ACTIVITY_TYPE_DAILY_LOVE = 2104,								// 每日爱一次
	RAND_ACTIVITY_TYPE_KING_DRAW = 2105,								// 陛下请翻牌
	RAND_ACTIVITY_TYPE_TOTAL_CHARGE_3 = 2106,							// 累计充值3(三件套活动)
	RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_PASSIVE_RANK = 2107,			// 被动变身排行榜
	RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_RANK = 2108,					// 特殊形象变身排行榜
	RAND_ACTIVITY_TYPE_ANY_MONSTER_DROP = 2109,							// 全场怪物掉落
	RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4 = 2110,							// 累计充值4(新三件套活动)
	RAND_ACTIVITY_TYPE_MINE = 2111,										// 趣味挖矿
	RAND_ACTIVITY_TYPE_GUAGUA = 2112,									// 顶刮刮
	RAND_ACTIVITY_TYPE_TIANMING_DIVINATION = 2113,						// 天命卜卦
	RAND_ACTIVITY_TYPE_FANFAN = 2114,									// 翻翻转
	RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI = 2115,						// 连续充值
	RAND_ACTIVITY_TYPE_CONTINUE_CONSUME = 2116,							// 连续消费
	RAND_ACTIVITY_TYPE_ARMY_DAY = 2117,									// 军号嘹亮
	RAND_ACTIVITY_TYPE_ANY_MONSTER_DROP2 = 2118,						// 打怪掉落2
	RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI = 2119,						// 循环充值
	RAND_ACTIVITY_TYPE_EXTREME_LUCKY = 2120,							// 至尊幸运
	RAND_ACTIVITY_TYPE_MIJINGXUNBAO2 = 2121,							// 秘境寻宝2
	RAND_ACTIVITY_TYPE_MONEY_TREE2 = 2122,								// 摇钱树2
	RAND_ACTIVITY_TYPE_NATIONAL_DAY = 2123,								// 欢度国庆
	RAND_ACTIVITY_TYPE_ZHENBAOGE2 = 2124,								// 珍宝阁2
	RAND_ACTIVITY_TYPE_PROMOTING_POSITION = 2125,						// 步步高升
	RAND_ACTIVITY_TYPE_BLACK_MARKET = 2126,								// 黑市拍卖
	RAND_ACTIVITY_TYPE_TREASURES_MALL = 2127,							// 珍宝商城

	RAND_ACTIVITY_TYPE_OPEN_SERVER_BEGIN = 2128,
	RAND_ACTIVITY_TYPE_ROLE_UPLEVEL = RAND_ACTIVITY_TYPE_OPEN_SERVER_BEGIN,// 冲级大礼(开服活动)
	RAND_ACTIVITY_TYPE_PATA = 2129,										// 勇者之塔(开服活动)
	RAND_ACTIVITY_TYPE_EXP_FB = 2130,									// 经验副本(开服活动)
	//RAND_ACTIVITY_TYPE_UPGRADE_MOUNT = 2131,							// 坐骑进阶(开服活动)
	//RAND_ACTIVITY_TYPE_UPGRADE_HALO = 2132,							// 光环进阶(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK = 2131,					//时装进阶榜(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK = 2132,						//神兵进阶榜(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_WING = 2133,								// 羽翼进阶(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG = 2134,							// 神弓进阶(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_SHENYI = 2135,							// 神翼进阶(开服活动)
	RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN = 2136,						// 首充团购(开服活动)
	RAND_ACTIVITY_TYPE_DAY_TOTAL_CHARGE = 2137,							// 每日累计充值(开服活动)
	//RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_TOTAL = 2138,					// 全服坐骑进阶(开服活动)
	//RAND_ACTIVITY_TYPE_UPGRADE_HALO_TOTAL = 2139,						// 全服光环进阶(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK = 2138,						//法宝进阶榜(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK = 2139,					//足迹进阶榜(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_WING_TOTAL = 2140,						// 全服羽翼进阶(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_TOTAL = 2141,					// 全服神弓进阶(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_TOTAL = 2142,						// 全服神翼进阶(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK = 2143,						// 坐骑进阶榜(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK = 2144,						// 光环进阶榜(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK = 2145,						// 羽翼进阶榜(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK = 2146,					// 神弓进阶榜(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK = 2147,						// 神翼进阶榜(开服活动)
	RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL = 2148,				// 装备全身强化总等级（开服活动）
	RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL = 2149,						// 装备全身宝石总等级（开服活动）
	RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK = 2150,			// 装备全身强化总等级榜(开服活动)
	RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK = 2151,					// 装备全身宝石总等级榜(开服活动)
	RAND_ACTIVITY_TYPE_BOSS_KILLER = 2152,								// boss猎手
	RAND_ACTIVITY_TYPE_OPEN_SERVER_BATTLE = 2153,						// 开服争霸
	RAND_ACTIVITY_TYPE_XIANNV_RANK = 2154,								// 女神战力排行榜
	RAND_ACTIVITY_TYPE_JINGLING_RANK = 2155,							// 精灵战力排行榜
	RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK = 2156,							// 战斗坐骑进阶排行榜
	RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK = 2157,							// 人物总战力排行榜
	RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK = 2158,						// 麒麟臂进阶榜(开服活动)	
	RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK = 2159,						// 头饰进阶榜(开服活动)	
	RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK = 2160,						// 腰饰进阶榜(开服活动)	
	RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK= 2161,						// 灵童进阶榜(开服活动)	
	RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK = 2162,					// 灵弓进阶榜(开服活动)	
	RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK = 2163,						// 灵骑进阶榜(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK = 2164,						// 飞宠进阶榜(开服活动)
	RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK = 2165,						// 尾焰进阶榜(开服活动)
	RAND_ACTIVITY_TYPE_OPEN_SERVER_END = 2166,

	RAND_ACTIVITY_TYPE_NEW_CORNUCOPIA = 2167,							// 新聚宝盆
	RAND_ACTIVITY_TYPE_ITEM_COLLECTION = 2168,							// 集字活动
	RAND_ACTIVITY_TYPE_MARRY_ME = 2169,									// 我们结婚吧
	RAND_ACTIVITY_TYPE_RED_ENVELOPE_GIFT = 2170,						// 红包好礼
	RAND_ACTIVITY_TYPE_SUPPER_GIFT = 2171,								// 礼包限购
	RAND_ACTIVITY_TYPE_EXP_REFINE = 2172,								// 经验炼制
	RAND_ACTIVITY_TYPE_GOLDEN_PIG =	2173,								// 金猪召唤
	RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU = 2174,					// 连充特惠初
	RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO = 2175,					// 连充特惠高
	RAND_ACTIVITY_TYPE_OPEN_SERVER_INVEST = 2176,						// 开服投资
	RAND_ACTIVITY_TYPE_TREASURE_CONVERT = 2177,							// 珍宝兑换
	RAND_ACTIVITY_TYPE_SINGLE_CHONGZHI = 2178,							// 单返豪礼
	RAND_ACTIVITY_TYPE_XIANYUAN_TREAS = 2179,							// 聚划算
	RAND_ACTIVITY_TYPE_RUSH_BUYING = 2180,								// 限时秒杀
	RAND_ACTIVITY_TYPE_SINGLE_CHARGE_2 = 2181,							// 急速冲战2
	RAND_ACTIVITY_TYPE_SINGLE_CHARGE_3 = 2182,							// 急速冲战3
	RAND_ACTIVITY_TYPE_SINGLE_CHARGE_4 = 2183,							// 急速冲战4
	RAND_ACTIVITY_TYPE_SINGLE_CHARGE_5 = 2184,							// 急速冲战5
	RAND_ACTIVITY_TYPE_MAP_HUNT = 2185,									// 地图寻宝
	RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE = 2186,						// 灯塔寻宝
	RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE = 2187,							// 新累计充值
	RAND_ACTIVITY_TYPE_MAGIC_SHOP = 2188,								// 幻装商店
	RAND_ACTIVITY_TYPE_PLANTING_TREE = 2189,							// 趣味植树(单身狗排行榜、虐狗排行榜  虐狗排行榜客户端自己处理跟着单身狗排行榜一起开启)
	RAND_ACTIVITY_TYPE_FISHING = 2190,									// 钓鱼
	RAND_ACTIVITY_TYPE_CRYSTAL_DALARAN = 2191,							// 达拉然水晶
	RAND_ACTIVITY_TYPE_CHONGZHI_CRAZY_REBATE = 2192,					// 狂返元宝
	RAND_ACTIVITY_TYPE_PERFECT_LOVER = 2193,							// 完美情人
	RAND_ACTIVITY_TYPE_CONSUM_GIFT = 2194,                              // 消费领奖
	RAND_ACTIVITY_TYPE_BUY_LIMIT = 2195,                                // 每日限购
	RAND_ACTIVITY_TYPE_COLLECT_TREASURE = 2196,                         // 聚宝盆新
	RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY = 2197,                      // 暴击日
	RAND_ACTIVITY_TYPE_HAPPY_ACCUMUL_RECHARGE = 2198,                   // 欢乐累充
	RAND_ACTIVITY_TYPE_UPLEVEL_RETURN = 2199,                           // 进阶返还
	RAND_ACTIVITY_TYPE_QUANMINJINJIE = 2200,							// 全民进阶
	RAND_ACTIVITY_TYPE_QUANMINZONGDONGYUAN = 2201,						// 全民总动员
	RAND_ACTIVITY_TYPE_GUILDBATTLE = 2202,								// 帮派争霸
	RAND_ACTIVITY_TYPE_LIMIT_TIME_REBATE = 2203,						// 限时回馈
	RAND_ACTIVITY_TYPE_TIME_LIMIT_GIFT = 2204,							// 限时礼包
	RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI_2 = 2205,					// 循环充值2
	RAND_ACTIVITY_TYPE_SHAKE_MONEY = 2206,								// 疯狂摇钱树
	RAND_ACTIVITY_TYPE_LIMIT_LUXURY_GIFT_BAG_GIFT = 2207,				// 限时豪礼
	RAND_ACTIVITY_TYPE_DOUBLE_CHONGZHI = 2208,							// 普天同庆
	RAND_ACTIVITY_TYPE_RMB_BUT_CHEST_SHOP = 2209,						// 神秘商店
	RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD = 2210,						// 消费返礼
	RAND_ACTIVITY_TYPE_ITEM_COLLECTION_2 = 2211,                        // 集月饼活动
	RAND_ACTIVITY_TYPE_VERSIONS_CONTINUE_CHARGE = 2212,					// 版本连续充值
	RAND_ACTIVITY_TYPE_VERSIONS_GRAND_TOTAL_CHARGE = 2213,				// 版本累计充值
	RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2 = 2214,							// 欢乐摇奖2
	RAND_ACTIVITY_TYPE_OFFLINE_SINGLE_CHARGE_0 = 2215,					// 线下单笔充值0
	RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE = 2216,					    // 极限挑战
	RAND_ACTIVITY_TYPE_LOGIN_GIFT_0 = 2217,								// 登录奖励0
	RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT = 2218,					    // 每日好礼
	RAND_ACTIVITY_TYPE_BUY_ONE_GET_ONE_FREE = 2219,						// 买一送一
	RAND_ACTIVITY_TYPE_CONSUME_FOR_GIFT = 2220,							// 消费有礼
	RAND_ACTIVITY_TYPE_MIJINGXUNBAO3 = 2221,							// 秘境寻宝3
	RAND_ACTIVITY_TYPE_HUANLE_ZADAN = 2222,								// 欢乐砸蛋
	RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG = 2223,							// 欢乐摇奖
	RAND_ACTIVITY_TYPE_TOTAL_CHARGE_FIVE = 2224,						// 累计充值5(版本吉祥三宝)
	RAND_ACTIVITY_TYPE_HOLIDAY_GUARD = 2225,							// 节日守护(吃鸡)
	RAND_ACTIVITY_TYPE_SPECIAL_IMG_SUIT = 2226,							// 限定套装
	RAND_ACTIVITY_TYPE_CHONGZHI_GIFT = 2227,							// 你充我送
	RAND_ACTIVITY_TYPE_PROFESS_RANK = 2228,								// 表白排行榜
	RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2 = 2229,                         // 进阶返还2
	RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY_2 = 2230,                    // 暴击日2
	RAND_ACTIVITY_TYPE_COMBINE_BUY = 2231,								// 组合购买
	RAND_ACTIVITY_TYPE_LUCKY_WISH = 2232,								// 幸运许愿
	RAND_ACTIVITY_TYPE_IMAGE_COMPETITION = 2233,						// 形象比拼
	RAND_ACTIVITY_TYPE_DISCOUNT_BUY_HOME = 2234,						// 折扣买房(夫妻家园)
	RAND_ACTIVITY_TYPE_DISCOUNT_BUY_FURNITURE= 2235,					// 买一送一(夫妻家园)
	RAND_ACTIVITY_TYPE_COUPLE_HALO_SELL = 2236,							// 夫妻光环特购
	RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2 = 2237,						// 限时连充
	RAND_ACTIVITY_TYPE_WEST_WEDDING = 2238,                				// 欧式婚礼半价
	RAND_ACTIVITY_TYPE_LOTTERY_1 = 2239,                				// 周末狂欢（抽奖）
	RAND_ACTIVITY_TYPE_DISCOUNT_GET_BABY = 2240,                		// 五折抱娃
	RAND_ACTIVITY_TYPE_SUPPER_GIFT_2 = 2241,							// 礼包限购2
	RAND_ACTIVITY_TYPE_ZERO_BUY_RETURN = 2242,                			// 零元购(先购买再返还)
	RAND_ACTIVITY_TYPE_DAILY_LOVE_2 = 2243,								// 每日一爱2
	RAND_ACTIVITY_TYPE_KUANG_HAI_QING_DIAN = 2244,						// 狂嗨庆典
	RAND_ACTIVITY_TYPE_GIFT_HARVEST = 2245,								// 礼物收割
	RAND_ACTIVITY_TYPE_LUCKY_CLOUD_BUY = 2246,							// 幸运云购
	RAND_ACTIVITY_TYPE_CRACYBUY = 2247,									// 狂欢大乐购
	RAND_ACTIVITY_TYPE_XIAN_ZUN_KA = 2248,								// 仙尊卡活动
	RAND_ACTIVITY_TYPE_SINGLE_CHARGE1 = 2249,							// 单次充值1
	RAND_ACTIVITY_TYPE_SINGLE_CHARGE2 = 2250,							// 单次充值2
	RAND_ACTIVITY_TYPE_SINGLE_CHARGE3 = 2251,							// 单次充值3
	RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1 = 2252,						// 新累计充值1
	RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2 = 2253,						// 新累计充值2
	RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3 = 2254,						// 新累计充值3
	RAND_ACTIVITY_TYPE_DOUBLE_GET = 2255,								// 双倍元宝
	RAND_ACTIVITY_TYPE_CHONGZHI_RANK2 = 2256,							// 充值排行2

	RAND_ACTIVITY_TYPE_END,
};

static const int RAND_ACTIVITY_TYPE_MAX = RAND_ACTIVITY_TYPE_END - RAND_ACTIVITY_TYPE_BEGIN;

// 随机活动开启类型
enum RAND_ACTIVITY_OPEN_TYPE
{
	RAND_ACTIVITY_OPEN_TYPE_BEGIN = 0,

	RAND_ACTIVITY_OPEN_TYPE_NORMAL = RAND_ACTIVITY_OPEN_TYPE_BEGIN,		// 普通随机活动
	RAND_ACTIVITY_OPEN_TYPE_VERSION,									// 版本活动

	RAND_ACTIVITY_OPEN_TYPE_END,
};

enum JINJIE_ACT_THEME_TYPE                  // 进阶活动主题
{
	JINJIE_ACT_THEME_TYPE_INVALID = 0,
	JINJIE_ACT_THEME_TYPE_MOUNT = 1,       // 坐骑
	JINJIE_ACT_THEME_TYPE_WING = 2,        // 羽翼
	JINJIE_ACT_THEME_TYPE_FABAO = 3,       // 法宝
	JINJIE_ACT_THEME_TYPE_SHENBING = 4,    // 神兵
	JINJIE_ACT_THEME_TYPE_FOOTSPRINT = 5,  // 足迹
	JINJIE_ACT_THEME_TYPE_GUANGHUAN = 6 ,   // 光环
	JINJIE_ACT_THEME_TYPE_FASHION = 7,     // 时装
	JINJIE_ACT_THEME_TYPE_FIGHT_MOUNT = 8, // 战骑
	JINJIE_ACT_THEME_TYPE_TOUSHI = 9,      // 头饰
	JINJIE_ACT_THEME_TYPE_MASK = 10,        // 面饰
	JINJIE_ACT_THEME_TYPE_YAOSHI = 11,      // 腰饰
	JINJIE_ACT_THEME_TYPE_QILINBI = 12,     // 麒麟臂
	JINJIE_ACT_THEME_TYPE_LINGTONG = 13,    // 灵童
	JINJIE_ACT_THEME_TYPE_LINGGONG = 14,    // 灵弓
	JINJIE_ACT_THEME_TYPE_LINGQI = 15,		// 灵骑
	JINJIE_ACT_THEME_TYPE_SHENGONG = 16,	// 仙女光环
	JINJIE_ACT_THEME_TYPE_SHENYI = 17,		// 仙女法阵
	JINJIE_ACT_THEME_TYPE_FLYPET = 18,		// 飞宠
	JINJIE_ACT_THEME_TYPE_WEIYAN = 19,		// 尾焰

	JINJIE_ACT_THEME_TYPE_MAX,
};

struct RandActivityData
{
	static const int DATA_LENGTH = 127;

	RandActivityData() : begin_time(0), end_time(0), open_type(RAND_ACTIVITY_OPEN_TYPE_BEGIN)
	{
		memset(data, 0, sizeof(data));
	}

	unsigned int begin_time;
	unsigned int end_time;
	char data[DATA_LENGTH];
	char open_type;
};

struct RandActivityParam
{
	RandActivityParam() {}

	void Reset()
	{
		memset(data_list, 0, sizeof(data_list));
	}

	RandActivityData data_list[RAND_ACTIVITY_TYPE_MAX];
};

//////////////////////////////////////////////// 通用活动 ////////////////////////////////////////////////
static const int ACTIVITY_DEFAULT_READY_TIME_S = 300;						// 活动默认等待时间

union RewardState
{
	RewardState() : data(0) {}

	char reward_state_list[8];
	long long data;
};

static const int COMMON_ACTIVITY_UNIT_MAX_NUM = 32;						// 通用活动子活动最大数量

enum COMMON_ACTIVITY_UNIT_TYPE											// 通用活动子活动定义
{
	COMMON_ACTIVITY_UNIT_TYPE_INVALID = 0,
	COMMON_ACTIVITY_UNIT_TYPE_FOR_TEST = 1,								// 为了测试

	COMMON_ACTIVITY_UNIT_TYPE_MAX,
};

UNSTD_STATIC_CHECK(COMMON_ACTIVITY_UNIT_TYPE_MAX <= COMMON_ACTIVITY_UNIT_MAX_NUM);

//////////////////////////////////////////////// 跨服活动 ////////////////////////////////////////////////
enum CROSS_ACTIVITY_TYPE
{
	CROSS_ACTIVITY_TYPE_INVALID = 3072,
	CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER = 3073,								// 跨服修罗塔
	CROSS_ACTIVITY_TYPE_CROSS_1V1 = 3074,										// 跨服1v1
	CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE = 3075,						// 跨服3V3
	CROSS_ACTIVITY_TYPE_TUANZHAN = 3076,										// 跨服团战
	CROSS_ACTIVITY_TYPE_PASTURE = 3077,											// 跨服牧场
	CROSS_ACTIVITY_TYPE_CROSS_BOSS = 3078,										// 跨服boss
	CROSS_ACTIVITY_TYPE_TEAM_FB = 3079,											// 跨服组队（长期开放场景）
	CROSS_ACTIVITY_TYPE_HOT_SPRING = 3080,										// 跨服温泉
	CROSS_ACTIVITY_TYPE_SHUIJING = 3081,										// 跨服水晶
	CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE = 3082,								// 跨服帮派战
	CROSS_ACTIVITY_TYPE_DARK_NIGHT = 3083,										// 跨服月黑风高
	CROSS_ACTIVITY_TYPE_FISHING = 3084,											// 跨服钓鱼
	CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB = 3085,									// 夜战王城
	CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB = 3086,									// 乱斗战场
	CROSS_ACTIVITY_TYPE_LIEKUN_FB = 3087,										// 猎鲲地带
	CROSS_ACTIVITY_TYPE_VIP_BOSS = 3088,                                        // 跨服vipBoss    
	CROSS_ACTIVITY_TYPE_CROSS_MIZANG_BOSS = 3089,								// 跨服秘藏boss
	CROSS_ACTIVITY_TYPE_CROSS_YOUMING_BOSS = 3090,								// 跨服幽冥boss
										// 3091,								// 跨服诛魔,假活动,在下面加枚举的时候不要用3091,直接跳过这个活动号
	CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI = 3092,								// 跨服边境之地
	CROSS_ACTIVITY_TYPE_CROSS_CHALLENGEFIELD = 3093,							// 跨服竞技场
	CROSS_ACTIVITY_TYPE_HUSONG_SHUIJING = 3094,									// 跨服护送水晶

	CROSS_ACTIVITY_TYPE_RESERVED
};

static const int CROSS_ACTIVITY_TYPE_MAX = CROSS_ACTIVITY_TYPE_RESERVED - CROSS_ACTIVITY_TYPE_INVALID;

enum ACTIVITY_STATUS
{
	ACTIVITY_STATUS_CLOSE = 0,											// 活动关闭状态
	ACTIVITY_STATUS_STANDY,												// 活动准备状态
	ACTIVITY_STATUS_OPEN,												// 活动开始中 
	ACTIVITY_STATUS_MAX,
};

struct ActivityStatus
{
	short activity_type;
	short status;
	unsigned int activity_begin_time;
	unsigned int activity_end_time;
	int data;
};

static const int MAX_ACTIVIYT_TIME_CFG_NUM = 5;							// 每个活动最多的配置数
static const int MAX_ACTIVITY_OPEN_WEEKDAY = 7;							// 每个活动开启的最大星期天数

struct StatusTimeCfg
{
	StatusTimeCfg() : status(0), start_hour(0), start_min(0), start_sec(0), dur_s(0) {}

	bool Invalid() { return 0 == dur_s; }

	int status;															// 状态
	int start_hour;														// 开始 H
	int start_min;														// 开始 M
	int start_sec;														// 开始 S
	int dur_s;															// 持续多少S
};

struct StatusTimeCfgList
{
	StatusTimeCfg statustime[ACTIVITY_STATUS_MAX];
};

struct ActivityCfg
{
	ActivityCfg() : type(ACTIVITY_TYPE_INVALID), is_open(false), statustime_list_count(0) 
	{
		memset(open_weekday_list, 0, sizeof(open_weekday_list));
	}

	bool Invalid() { return ACTIVITY_TYPE_INVALID == type; }

	int type;
	bool is_open;

	int open_weekday_list[MAX_ACTIVITY_OPEN_WEEKDAY];

	int statustime_list_count;
	StatusTimeCfgList statustime_list[MAX_ACTIVIYT_TIME_CFG_NUM];
};

struct SpecialActivityCfg
{
	SpecialActivityCfg() : type(SPECIAL_ACTIVITY_TYPE_INVALID), is_open(false), begin_time(0), end_time(0) {}

	bool Invalid() { return SPECIAL_ACTIVITY_TYPE_INVALID == type; }

	int type;
	bool is_open;

	unsigned int begin_time;
	unsigned int end_time;
};

struct RandActivityCfg
{
	RandActivityCfg() : type(0), is_open(false), duration_day(0) {}

	bool Invalid() { return 0 == type; }

	int type;
	bool is_open;

	int duration_day;
};

//////////////////////////////////////////////// 活动 相关定义 ////////////////////////////////////////////////
struct ActivityRoomStatus
{
	ActivityRoomStatus() : is_open(0), role_num(0) {}

	bool IsOpen() { return 0 != is_open; }
	void Open() { is_open = 1; }

	int is_open;													
	int role_num;
};

static const int ACTIVITY_ROOM_MAX = 8;												// 开房间的活动 统一为8个
static const int ACTIVITY_ROOM_DEFAULT_USER_MAX = 120;								// 开房间的活动默认最大人数
static const int ZHANCHANG_LUACY_ROLE_MAX = 5;										// 攻城战、领土战、公会争霸、元素战场幸运玩家最大值

inline const int GetSkillIDBySceneTypeAndIndex(int scene_type, int skill_index)
{
	return 1000 + scene_type * 50 + skill_index;
}

//////////////////////////////////////////////// 战场活动 相关定义 ////////////////////////////////////////////////
static const unsigned int FIRST_GUILD_INTERVAL = 60;
static const unsigned int BEST_KILLER_INTERVAL = 30;
static const unsigned int ASSIST_LIMIT_TIME = 30;
static const int GUILD_BATTLE_NEW_POINT_NUM = 5;									// 公会争霸据点数量
static const int GUILD_BATTLE_NOTICE_INTERVAL = 30;									// 发送据点血量过低提示的时间间隔
static const int GUILD_BATTLE_NEW_REWARD_CITY_COUNT = 5;							// 公会争霸奖励城市数量

//////////////////////////////////////////////// 诛邪战场 相关定义 ////////////////////////////////////////////////
enum ZHUXIE_TASK_TYPE
{
	ZHUXIE_TASK_TYPE_INVALID = 0,
	ZHUXIE_TASK_TYPE_GATHER,														// 诛邪任务类型 采集
	ZHUXIE_TASK_TYPE_KILL_BOSS,														// 诛邪任务类型 打BOSS
	ZHUXIE_TASK_TYPE_KILL_MONSTER,													// 诛邪任务类型 打怪物
	ZHUXIE_TASK_TYPE_KILL_ROLE,														// 诛邪任务类型 击杀角色
	ZHUXIE_TASK_TYPE_MAX,
};

struct ZhuXieTaskInfo																// 角色身上拥有的诛邪任务信息 
{
	ZhuXieTaskInfo() : task_id(0), task_world_min_level(0), task_world_max_level(0), task_type(ZHUXIE_TASK_TYPE_INVALID), param_id(0), param_value(0), max_value(0), is_fetch_reward(0),
		shengwang(0) {}

	bool IsFinish() { return param_value >= max_value; }
	bool IsFetchReward() { return 0 != is_fetch_reward; }
	bool IsMatch(short _task_type, int _param_id)
	{
		if (task_type == _task_type && 0 == param_id)  //只区分类型不区分目标id
		{
			return true;
		}

		return task_type == _task_type && param_id == _param_id; 
	}

	short task_id;																	// 任务ID
	short task_world_min_level;
	short task_world_max_level;
	short task_type;																// 任务类型
	int param_id;																	// 关联任务ID 采集物ID或怪物ID
	short param_value;																// 当前值
	short max_value;																// 最大值
	int is_fetch_reward;															// 是否领取奖励
	int shengwang;

	ItemConfigData reward_item1;
	ItemConfigData reward_item2;
	ItemConfigData reward_item3;
};

struct SpecialZhuXieUser
{
	SpecialZhuXieUser() : user_id(INVALID_USER_ID), kills(0), role_level(0), kill_role_get_total_shengwang(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UserID user_id;
	int kills;
	int role_level;
	GameName user_name;

	int kill_role_get_total_shengwang;
	std::vector<ZhuXieTaskInfo> zhuxietask_list;									// 诛邪战场 玩家的任务信息 
};
typedef std::map<UserID, SpecialZhuXieUser> SpecialZhuXieUserMap;
typedef std::map<UserID, SpecialZhuXieUser>::iterator SpecialZhuXieUserMapIt;

static const int ZHUXIE_TASK_MAX = 4;												// 诛邪战场任务最大个数 

static const int ZHUXIE_REALIVE_POS_MAX = 7;										// 诛邪战场 复活点个数
static const int ZHUXIE_LEVEL_REWARD_MAX = 100;										// 诛邪任务奖励 等级最大分段数
static const int ZHUXIE_BOSS_NUM = 5;												// 诛邪战场BOSS数量
static const int ZHUXIE_MONSTER_NUM = 4;											// 诛邪战场怪物种类数量
static const int ZHUXIE_GATHER_NUM = 3;												// 诛邪战场采集物种类数量
static const int ZHUXIE_FIRST_FLUSH_BOSS_TIME = 600;								// 诛邪战场第一次刷新BOSS的时间

static const int ZHUXIE_MAX_RELACE_BOSS_NUM = 10;									// 诛邪战场替换BOSS的最大数量
static const int ZHUXIE_MAX_RELACE_MONSTER_NUM = 10;								// 诛邪战场替换怪物的最大数量
static const int ZHUXIE_MAX_RELACE_GATHER_NUM = 10;									// 诛邪战场采集物的最大数量

static const int ZHUXIE_MONSTER_BORN_LESS_NUM = 10;									// 怪物少于10个时刷新
static const int ZHUXIE_GATHER_BORN_LESS_NUM = 5;									// 采集少于10个时刷新

//////////////////////////////////////////////// 天降财宝 相关定义 ////////////////////////////////////////////////
enum TIANJIANGCAIBAO_TASK_TYPE
{
	TIANJIANGCAIBAO_TYPE_INVALID = 0,
	TIANJIANGCAIBAO_TYPE_GATHER,													// 天降财宝类型 采集
	TIANJIANGCAIBAO_TYPE_KILL_MONSTER,												// 天降财宝类型 击杀野怪
	TIANJIANGCAIBAO_TYPE_MAX,
};

static const int TIANJIANGCAIBAO_MAX_REWARD_ITEM_COUNT = 10;						// 奖励物品最大类型数
static const int TIANJIANGCAIBAO_GOLD_RANK_MAX = 10;								// 天降财宝元宝排行最大数
static const int MAX_REWARD_CFG_COUNT = 4;											// 额外奖励配置数

static const int TIANJIANGCAIBAO_TASK_MAX = 20;										// 天降财宝 任务最大个数 
static const int TIANJIANGCAIBAO_BIGQIAN_NUM = 1;									// 天降财宝 巨型钱多多数量
static const int TIANJIANGCAIBAO_REFLUSH_NOTICE_TIME = 10;							// 天降财宝 刷新提示时间

struct TianJiangCaiBaoTaskInfo														// 角色身上拥有的天降财宝任务信息 
{
	TianJiangCaiBaoTaskInfo() : task_id(0), task_type(0), param_id(0), param_count(0), cur_param_value(0), is_finish(0), reward_exp(0), exp_factor_task(0){}

	bool IsFinish() { return is_finish != 0; }
	bool IsMatch(short _task_type, int _param_id)
	{
		return task_type == _task_type && param_id == _param_id; 
	}

	void AddParamVal() 
	{
		++ cur_param_value; 
		
		if (cur_param_value >= param_count)
		{
			is_finish = 1;
		}
	}

	short task_id;																	// 任务ID

	int task_type;
	int param_id;																	// 任务关联的id(采集物，怪物)
	int param_count;																// 任务关联的采集物或怪物的数量
	int cur_param_value;															// 当前值
	int is_finish;																	// 是否完成任务

	int reward_exp;
	int exp_factor_task;
	ItemConfigData reward_list[MAX_ATTACHMENT_ITEM_NUM];							// 奖励物品
};

struct RewardItemInfo																// 获得的物品信息
{
	RewardItemInfo() : item_id(0), num(0) {}

	ItemID item_id;
	short num;
};

struct RankGold
{
	RankGold() : user_id(INVALID_USER_ID), rank_value(0), get_total_gold(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UserID user_id;
	GameName user_name;

	int rank_value;
	int get_total_gold;	
};

struct TianJiangCaiBaoUser
{
	TianJiangCaiBaoUser() : user_id(INVALID_USER_ID), task_finish_num(0), get_total_gold(0), curr_task_id(-1), curr_task_param(0), lianzhan(0), assist(0)
	{
		memset(is_fetch_reward, 0, sizeof(is_fetch_reward));
	}

	UserID user_id;

	int task_finish_num;
	int get_total_gold;
	int curr_task_id;
	int curr_task_param;
	int lianzhan;
	int assist;

	char is_fetch_reward[MAX_REWARD_CFG_COUNT];
	std::map<int, int> role_get_item_map;												// 天降财宝 玩家获得物品信息
	std::vector<int> task_list;															// 天降财宝 任务信息，用于记录未完成的任务
	std::map<UserID, time_t> hurt_map;													// 伤害我的 用户列表	
};

typedef std::map<UserID, TianJiangCaiBaoUser> TianJiangCaiBaoUserMap;
typedef std::map<UserID, TianJiangCaiBaoUser>::iterator TianJiangCaiBaoUserMapIt;

//////////////////////////////////////////////// 王陵探险 相关定义 ////////////////////////////////////////////////
enum WANGLINGEXPLORE_TASK_TYPE
{
	WANGLINGEXPLORE_TYPE_INVALID = 0,
	WANGLINGEXPLORE_TYPE_GATHER,														// 王陵探险 采集
	WANGLINGEXPLORE_TYPE_KILL_MONSTER,													// 王陵探险 击杀野怪
	WANGLINGEXPLORE_TYPE_MAX,
};

static const int WANGLINGEXPLORE_TASK_MAX = 5;											// 王陵探险 任务最大个数
static const int WANGLINGEXPLORE_REFLUSH_NOTICE_TIME = 10;								// 王陵探险 刷新提示时间

static const int WANGLINGEXPLORE_MAX_REWARD_ITEM_COUNT = 10;							// 王陵探险 奖励物品最大数
static const int WANGLINGEXPLORE_OWNER_ROLE_AND_BOSS_DISTANCE = 200;					// 王陵探险 归属权最大距离

struct WangLingExploreTaskInfo															// 角色身上拥有的王陵探险任务信息 
{
	WangLingExploreTaskInfo() : task_id(0), task_type(0), param_id(0), param_count(0), cur_param_value(0), is_finish(0), is_double_reward(false), is_fetch_reward(false), item_count(0), reward_exp(0), exp_factor_task(0){}

	bool IsFinish() { return is_finish != 0; }
	bool IsMatch(short _task_type, int _param_id)
	{
		return task_type == _task_type && param_id == _param_id;
	}

	void AddParamVal() 
	{
		++ cur_param_value; 

		if (cur_param_value >= param_count)
		{
			is_finish = 1;
		}
	}

	bool IsDoubleReward() { return is_double_reward; }
	bool IsFetchReward() { return is_fetch_reward; }

	short task_id;																	// 任务ID

	int task_type;
	int param_id;																	// 任务关联的id(采集物，怪物)
	int param_count;																// 任务关联的采集物或怪物的数量
	int cur_param_value;															// 当前完成值
	int is_finish;																	// 是否完成任务
	bool is_double_reward;
	bool is_fetch_reward;

	int item_count;
	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
	int reward_exp;
	double exp_factor_task;
};

struct WangLingExploreUser
{
	WangLingExploreUser() : user_id(INVALID_USER_ID), is_send_item_info(false), task_finish_num(0), is_fetch_reward(false), lianzhan(0), assist(0),
	is_brocast_gather_buff(0),wangling_buff_timestamp(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UserID user_id;
	GameName user_name;

	bool is_send_item_info;															// 王陵探险 完成全部任务显示结算面板
	int task_finish_num;
	bool is_fetch_reward;															// 王陵探险 完成任务额外奖励
	int lianzhan;
	int assist;
	bool is_brocast_gather_buff;	
	unsigned int wangling_buff_timestamp;										// 王陵探险 玩家buff时间
	std::vector<WangLingExploreTaskInfo> wanglingexplore_task_list;					// 王陵探险 玩家的任务信息
	std::map<int, int> role_get_item_map;											// 王陵探险 玩家获得物品信息
	std::map<UserID, time_t> hurt_map;												// 伤害我的 用户列表	
};

typedef std::map<UserID, WangLingExploreUser> WangLingExploreUserMap;
typedef std::map<UserID, WangLingExploreUser>::iterator WangLingExploreUserMapIt;

//////////////////////////////////////////////// 答题活动-旧版 相关定义 ////////////////////////////////////////////////
enum QUESTION_STATE
{
	QUESTION_STATE_INVALID = 0,
	QUESTION_STATE_READY,
	QUESTION_STATE_READ,
	QUESTION_STATE_ANSWER,
	QUESTION_STATE_MAX,
};

static const int QUESTION_LIST_NUM = 30;											// 答题活动 列表大小

static const int QUESTION_STRING_MAX = 128;											// 题目最大字符		
typedef char QuestionStr[QUESTION_STRING_MAX];										// 题目定义

static const int QUESTION_START_READY_TIME_S = 60;									// 答题开始准备时间
static const int QUESTION_READY_TIME_S = 3;											// 下一道题 准备时间 
static const int QUESTION_READ_TIME_S = 7;											// 读题时间 
static const int QUESTION_ANSWER_TIME_S = 15;										// 答题时间

static const int QUESTION_RANK_MAX = 10;											// 答题排行榜

struct QuestionRankUser
{
	QuestionRankUser() : uid(0), score(0), right_num(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	int uid;
	GameName user_name;
	int score;
	int right_num;
};

typedef std::vector<QuestionRankUser> QuestionRankUserVec;
typedef std::vector<QuestionRankUser>::iterator QuestionRankUserVecIt;

struct QuestionUser
{
	QuestionUser() : user_id(INVALID_USER_ID), role_level(0), right_num(0), score(0), rankpos(0), answer_num(0), reward_exp(0), reward_shengwang(0), answer(-1), answer_time_s(0), kick_cd_time(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	bool AlreadyAnswer() { return -1 != answer; }
	void ClearAnswerInfo() { answer = -1; answer_time_s = 0; }

	UserID user_id;
	GameName user_name;

	int role_level;
	int right_num;
	int score;
	int rankpos;
	int answer_num;
	int reward_exp;
	int reward_shengwang;

	int answer;
	int answer_time_s;

	unsigned int kick_cd_time;
};
typedef std::map<UserID, QuestionUser> QuestionUserMap;
typedef std::map<UserID, QuestionUser>::iterator QuestionUserMapIt;

//////////////////////////////////////////////// 温泉活动 相关定义 ////////////////////////////////////////////////
enum HOTSPRING_ACTION
{
	HOTSPRING_ACTION_INVALID = 0,
	HOTSPRING_ACTION_SANDBALL,														// 扔沙球
	HOTSPRING_ACTION_HOTWATER,														// 泼热水
	HOTSPRING_ACTION_MAX,
};

enum HOTSPRING_STATUS
{
	HOTSPRING_STATUS_INVALID = 0,													// 正常
	HOTSPRING_STATUS_SAND,															// 沙人
	HOTSPRING_STATUS_HOT,															// 热晕
	HOTSPRING_STATUS_MAX,
};

//////////////////////////////////////////////////  跨服帮派战 /////////////////////////////
const static int CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE = 3;				// 最大旗子数在场景中
const static int CROSS_GUILDBATTLE_MAX_SCENE_NUM = 6;					// 帮派场景个数
const static int CROSS_GUILDBATTLE_MAX_TASK_NUM = 6;					// 场景任务个数
UNSTD_STATIC_CHECK(CROSS_GUILDBATTLE_MAX_SCENE_NUM == 6);
const static int CROSS_GUILDBATTLE_MAX_GUILD_RANK_NUM = 5;				// 跨服帮派战前5
const static int CROSS_GUILDBATTLE_MAX_LOG_RANK_NUM = 10;				// 跨服帮派战活动结束时存下前10名的数据
const static int CROSS_GUILDBATTLE_MAX_USER_SCORE_RANK_NUM = 100;		// 跨服帮派战活动玩家积分排行最大数量

///////////////////////////////////////////////////////////  王城灵脉相关  /////////////////////////////////////////////////////////////
static const int LINGMAI_LINGZHU_NUM_MAX = 4;										// 一个玩家最大同时种植的灵珠数
static const int LINGMAI_RAND_ITEM_GET_TIME_S = 12 * 3600;							// 12小时可获得稀有物品
static const int LINGMAI_LINGZHU_REWARD_TIME_MAX_HOUR = 24;							// 灵珠最多计算24小时的奖励
static const int LINGMAI_DISAPPEAR_TIME_S = LINGMAI_LINGZHU_REWARD_TIME_MAX_HOUR * 3600;	// 24消失后灵脉消失
static const int LINGMAI_ZHADAN_ROLE_EFFECT_LEVEL = 48;								// 炸弹可以炸死的角色所需等级

enum LINGMAI_TYPE
{
	LINGMAI_TYPE_INVALID = 0,
	LINGMAI_TYPE_LINGZHU,															// 灵珠
	LINGMAI_TYPE_ZHADAN,															// 炸弹
	LINGMAI_TYPE_SOULINGFU,															// 搜灵符
	LINGMAI_TYPE_MAX,
}; 

struct LingMaiItem
{
	LingMaiItem() 
		: unique_id(0), owner_uid(0), owner_level(0), owner_camp(0), lingmai_type(LINGMAI_TYPE_INVALID), reward_index(-1), 
		scene_id(0), pos_x(0), pos_y(0), plant_timestamp(0)
	{
		memset(owner_name, 0, sizeof(owner_name));
	}

	long long unique_id;															// 灵脉唯一标示ID 时间戳+自增值

	int owner_uid;																	// 主人ID
	GameName owner_name;															// 主人名字
	int owner_level;																// 主人等级

	char owner_camp;																// 主人阵营
	char lingmai_type;																// 灵脉类型：灵珠或炸弹（崩天玉）
	short reward_index;																// 奖励物品的下标

	int scene_id;																	// 场景ID
	int pos_x;																		// 灵脉坐标X
	int pos_y;																		// 灵脉坐标Y

	unsigned int plant_timestamp;													// 灵脉种植时间戳
};

//////////////////////////////////////////////// 群仙乱斗 相关定义 ////////////////////////////////////////////////

static const int MAX_MULTI_BATTLEFILED_SKILL_COUNT = 4;								// 多人战场技能数量

static const int ZHANCHANG_RESERVE_INFO_TIME_S = 30 * 60;							// 战场保留用户信息 时间

static const int QUNXIANLUANDOU_RANK_NUM= 10;										// 群仙乱斗 实时排行榜信息个数
static const int QUNXIANLUANDOU_SHENSHI_REFRESH_TIME_S = 5 * 60;					// 神石刷新时间 （神石死亡后，5分钟后再刷一个）

static const int QUNXIANLUANDOU_KILL_VALID_TIME_S = 30;								// 击杀有效间隔时间
static const int QUNXIANLUANDOU_ASSIST_VALID_TIME_S = 5;							// 助攻有效时间
static const unsigned int QUNXIANLUANDOU_REALIVE_HERE_TIME_S = 0;					// 原地复活时间间隔0s

static const int BATTLEFIELD_SHENSHI_MAX_OWNER_DISTENCE = 2;						// 战场神石 跟随距离
static const int BATTLEFIELD_SHENSHI_DEST_POINT_DISTANCE = 10;						// 战场神石 到达目标点的允许范围
static const int BATTLEFIELD_SHENSHI_MAX_PICK_UP_DISTANCE = 10;						// 神石拾取最远距离

enum QUNXIANLUANDOU_SIDE
{
	QUNXIANLUANDOU_SIDE_1,															// 群仙乱斗 边1							
	QUNXIANLUANDOU_SIDE_2,															// 群仙乱斗 边2
	QUNXIANLUANDOU_SIDE_3,															// 群仙乱斗 边3
	QUNXIANLUANDOU_SIDE_MAX,
};

struct QunXianLuanDouSideInfo														// 群仙乱斗 边信息
{
	QunXianLuanDouSideInfo() : role_num(0), score(0), robot_num(0), is_weak_side_protect(0) {}

	int role_num;
	int score;																		// 积分
	int robot_num;
	bool is_weak_side_protect;														// 是否弱阵营保护
};

struct QunXianLuanDouUserRankItem													// 群仙乱斗 排行榜信息
{
	QunXianLuanDouUserRankItem() : user_id(INVALID_USER_ID), side(0), score(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UserID user_id;	
	GameName user_name;																// 角色名
	int side;																		// 角色在战场中的阵营
	int score;																		// 斩杀人数
};

class SpecialQunXianLuanDou;
class Role;
class Scene;

struct QunXianLuanDouUser															// 群仙乱斗 用户信息
{
	QunXianLuanDouUser() : user_id(INVALID_USER_ID), vip(0), sex(0), prof(0), camp(0), level(0), gsnetid(INVALID_GSNETID),
		side(-1), kills(0), lianzhan(0), assists(0), lianzhan_max(0), rob_shenshi(0), 
		obj_id(INVALID_OBJ_ID), kill_honor(0), assist_honor(0), rob_shenshi_honor(0), free_reward_honor(0), extra_honor(0), last_leavetime(0), dead_time(0),
		last_realive_here_timestamp(0), score_reward_flag(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	bool GSNetIDValid() { return INVALID_GSNETID != gsnetid; }

	void SetSide(int _side) { side = _side; }
	bool InvalidSide() { return (side < QUNXIANLUANDOU_SIDE_1 || side >= QUNXIANLUANDOU_SIDE_MAX); }
	int GetSide() { return side; }

	void OnEnterActivity(Role *role); 
	void OnLeaveActivity(time_t now_second) { gsnetid = INVALID_GSNETID; obj_id = INVALID_OBJ_ID; last_leavetime = now_second; }

	void OnHurt(const UserID &attack_userid, time_t now_second);
	void OnKill(SpecialQunXianLuanDou *special_logic, bool is_reward);
	void OnDead(SpecialQunXianLuanDou *special_logic, time_t _dead_time, bool reset_lianzhan);
	void OnAssist(SpecialQunXianLuanDou *special_logic);
	void OnRobShenShi(SpecialQunXianLuanDou *special_logic);
	void OnSideMemberRobShenShi(SpecialQunXianLuanDou *special_logic);
	void OnFreeReward(SpecialQunXianLuanDou *special_logic, int reward_honor);
	void ChangeZhanyi(int add_zhanyi);
	void OnExtraHonor(SpecialQunXianLuanDou *special_logic, int reward_honor);

	int TotalHonor() const { return kill_honor + assist_honor + rob_shenshi_honor + free_reward_honor + extra_honor; } 
	int TotalHonor() { return kill_honor + assist_honor + rob_shenshi_honor + free_reward_honor + extra_honor; } 
	bool IsLeaveBattleField() { return 0 != last_leavetime; }

	bool IsFetchScoreRewardFlag(int reward_index)
	{
		return 0 != (score_reward_flag & (1 << reward_index));
	}

	void SetFetchScoreRewardFlag(int reward_index)
	{
		score_reward_flag |= (1 << reward_index);
	}

	Role * GetRole(Scene *scene);

	UserID user_id;																	// 用户ID
	GameName user_name;																// 用户名
	char vip;
	char sex;
	char prof;
	char camp;
	int level;

	GSNetID gsnetid;																// 用户的网络层ID 

	short side;																		// 角色在战场中的阵营
	short kills;																	// 杀人次数
	short lianzhan;																	// 连杀次数
	short assists;																	// 助攻次数
	short lianzhan_max;																// 最高连杀
	short rob_shenshi;																// 夺石次数
	ObjID obj_id;																	// 用户在场景上的ID

	int kill_honor;																	// 杀人荣誉
	int assist_honor;																// 助攻荣誉
	int rob_shenshi_honor;															// 夺石荣誉
	int free_reward_honor;															// 免费奖励荣誉
	int extra_honor;																// 额外荣誉

	time_t last_leavetime;															// 最后离开时间
	time_t dead_time;																// 战场中死亡时间

	unsigned int last_realive_here_timestamp;										// 上次原地复活时间

	int score_reward_flag;															// 个人奖励标记

	std::map<UserID, time_t> hurt_map;												// 伤害我的 用户列表

	std::map<UserID, time_t> kill_user_map;											// 击杀的玩家列表 
};
typedef std::map<UserID, QunXianLuanDouUser> QunXianLuanDouUserMap;
typedef std::map<UserID, QunXianLuanDouUser>::iterator QunXianLuanDouUserMapIt;

//////////////////////////////////////////////// 仙盟战 相关定义 ////////////////////////////////////////////////
static const int XIANMENGZHAN_REALIVE_AREA_NUM    = 5;                              // 复活点数量
static const int XIANMENGZHAN_DEFEND_AREA_NUM     = 5;                              // 守卫点数量

static const unsigned int XIANMENGZHAN_ROLE_REALIVE__HERE_TIME_S = 20;				// 玩家原地复活CD
static const int XIANMENGZHAN_SYNC_USER_INFO_TIME_S = 1;                            // 同步玩家信息间隔
static const int XIANMENGZHAN_SYNC_GUILD_INFO_TIME_S = 3;                           // 同步仙盟信息间隔
static const int XIANMENGZHAN_SYNC_DEFEND_AREA_LIST_TIME_S = 5;                     // 同步据点信息间隔
static const int XIANMENGZHAN_SYNC_GUILD_RANK_TIME_S = 3;							// 同步仙盟排行信息间隔
static const int XIANMENGZHAN_KILL_VALID_TIME_S = 10;								// 仙盟战击杀有效间隔时间
static const int XIANMENGZHAN_ASSIST_VALID_TIME_S = 5;								// 仙盟战助攻有效间隔时间

static const int XIANMENGZHAN_REWARD_GUILD_RANK_MAX = 10;							// 仙盟战排行奖励
static const int XIANMENGZHAN_REWARD_GUILD_RANK_GIFT_MAX = 10;						// 仙盟战排行礼包奖励

struct XianMengZhanUser
{
	XianMengZhanUser() 
		: dirty(false), user_id(INVALID_USER_ID), net_id(INVALID_GSNETID), obj_id(INVALID_OBJ_ID), level(0),
		guild_id(INVALID_GUILD_ID), call_allow(false), call_count(0), score(0), assist_count(0), free_score(0), kill_count(0), kill_score(0), lianzhan(0), last_realive_here_timestamp(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	bool dirty;

	UserID user_id;
	GSNetID net_id;
	ObjID obj_id;
	int level;
	GameName user_name;

	GuildID guild_id;
	bool call_allow;
	int call_count;

	int score;
	int assist_count;
	int free_score;
	int kill_count;
	int kill_score;
	int lianzhan;

	unsigned int last_realive_here_timestamp;

	std::map<UserID, time_t> hurt_map;							// 伤害我的 用户列表
	std::map<UserID, time_t> kill_user_map;						// 击杀的玩家列表 
};

typedef std::map<UserID, XianMengZhanUser> XianMengZhanUserMap;
typedef XianMengZhanUserMap::iterator XianMengZhanUserMapIt;

struct XianMengZhanData
{
	XianMengZhanData()
		: guild_id(INVALID_GUILD_ID), guild_score(0), camp(0)
	{
		memset(guild_name, 0, sizeof(guild_name));
	}
	void Clear()
	{
		guild_id = INVALID_GUILD_ID;
		memset(guild_name, 0, sizeof(guild_name));
		guild_score = 0;
		camp = CAMP_TYPE_INVALID;
	}

	GuildID guild_id;
	GuildName guild_name;
	int guild_score;
	int camp;
};

struct XianMengZhanGuild
{
	XianMengZhanGuild()
		: dirty(false), defend_area(-1), caller_pos_x(0), caller_pos_y(0), last_call_time(0), enter_area(-1)
	{
	}
	bool HasDefendArea() { return defend_area >= 0 && defend_area < XIANMENGZHAN_DEFEND_AREA_NUM; }

	bool dirty;

	int defend_area;

	int caller_pos_x;
	int caller_pos_y;
	time_t last_call_time;

	int enter_area;

	XianMengZhanData data;
};

typedef std::map<GuildID, XianMengZhanGuild> XianMengZhanGuildMap;
typedef XianMengZhanGuildMap::iterator XianMengZhanGuildMapIt;

struct XianMengZhanDefendArea
{
	XianMengZhanDefendArea()
		: dirty(false), gather_obj_id(INVALID_OBJ_ID)
	{
	}

	void Clear()
	{
		dirty = false;
		gather_obj_id = INVALID_OBJ_ID;
		data.Clear();
	}
	bool HasGuild() { return INVALID_GUILD_ID != data.guild_id; }

	bool dirty;
	ObjID gather_obj_id;
	XianMengZhanData data;
};

/////////////////////////////////////////////////  乱斗战场 /////////////////////////////
const static int MESS_BATTLE_TOP_RANK_MAX = 3;						// 乱斗战场总榜人数
const static int CROSS_MESS_BATTLE_MAX_SCENE_NUM = 10;					// 乱斗战场最大房间个数

////////////////////////////////////////////////// 全民 boss ////////////////////////
struct NationalBossRankItem													
{
	NationalBossRankItem() : uid(0), camp(0), hurt(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	int uid;	
	char camp;
	GameName user_name;
	long long hurt;																	
};

static const int NATIONALBOSS_FRESH_CONFIG_COUNT = 16;
static const int NATIONALBOSS_FRESH_BOSS_COUNT = 4;

//////////////////////////////////////////////// 开服活动 /////////////////////////////////////////////////////////
static const int OGA_PUTONEQUIPMENT_REWARD_MAX_SEQ = 32;
static const int OGA_CAPABILITY_REWARD_MAX_SEQ = 32;
static const int OGA_ROLELEVEL_REWARD_MAX_SEQ = 32;
static const int OGA_BOSS_MAX_SEQ = 64;								// BOSS序号最大值(不能大于32)
static const int OGA_BOSS_REWARD_MAX_SEQ = 20;						// BOSS序号最大值(不能大于32)
static const int OGA_GIFT_MAX_TYPE = 8;								// 礼包限购种类最大值
static const int OGA_GIFT_MAX_SEQ = 20;								// 礼包限购序号最大值(不能大于32)

//////////////////////////////////////////////// 随机活动 /////////////////////////////////////////////////////////

//////////////////////////////////////////////// 随机活动 幸运摇奖 /////////////////////////////////////////////////////////
static const int RAND_ACTIVITY_TEN_COUNT_ROLL = 10;
struct RandActivityLuckyRollNewsItem
{
	RandActivityLuckyRollNewsItem() : news_id(0), role_id(0), role_camp(0), buy_item_id(0) 
	{
		memset(role_name, 0, sizeof(role_name));
	}

	int news_id;
	int role_id;
	GameName role_name;
	int role_camp;
	int buy_item_id;
};

//////////////////////////////////////////////// 随机活动 全民抢购 /////////////////////////////////////////////////////////
static const int RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT = 16;// 全服抢购物品种类数量

//////////////////////////////////////////////// 随机活动 个人抢购 /////////////////////////////////////////////////////////
static const int RAND_ACTIVITY_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT = 8;// 个人抢购物品种类数量

//////////////////////////////////////////////// 随机活动 斗法狂欢 /////////////////////////////////////////////////////////
static const int RAND_ACTIVITY_DOUFA_KUANGHUAN_REWARD_TIMES = 2;	// 斗法狂欢 奖励倍数

//////////////////////////////////////////////// 随机活动 战场翻倍 /////////////////////////////////////////////////////////
static const int ZHANCHANG_FANGBEI_MAX_REWARD_TIMES = 2;	// 战场翻倍 奖励倍数

//////////////////////////////////////////////// 随机活动 红包好礼 /////////////////////////////////////////////////////////
static const int RAND_ACTIVITY_HONGBAO_HAOLI_MAX_COUNT = 7;// 红包好礼，红包个数

//////////////////////////////////////////////// 随机活动 每日限购 /////////////////////////////////////////////////////////
static const int RAND_ACTIVITY_DAILY_LIMIT_BUY_MAX_SEQ = 4;

//////////////////////////////////////////////// 随机活动 聚宝盆 /////////////////////////////////////////////////////////
static const int RAND_ACTIVITY_COLLECT_TREASURE_MAX_RECORD_COUNT = 10;

struct CollectTreasureRollRecord
{
	CollectTreasureRollRecord() : role_id(0), roll_mul(0)
	{
		memset(role_name, 0, sizeof(role_name));
	}

	int role_id;
	GameName role_name;
	int roll_mul;     //倍数
};

////////////////////////////////////////////////// 随机活动-限时回馈 ////////////////////////////////////////////////
static const int RA_LIMIT_TIME_REBATE_MAX_DAY = 9;

////////////////////////////////////////////////// 随机活动-神秘商店 ////////////////////////////////////////////////
static constexpr int RA_RMB_BUY_CHEST_SHOP_MAX_BUY_COUNT = 64;

////////////////////////////////////////////////集月饼活动//////////////////////////////////////////
static const int RAND_ACTIVITY_ITEM_COLLECTION_SECOND_REWARD_MAX_COUNT = 5;	// 集月饼活动配置最大个数

////////////////////////////////////////////////// 随机活动-消费换大礼 ////////////////////////////////////////////////
static const int RA_CONSUME_FOR_GIFT_MAX_ITEM_COUNT = 20;				// 最多兑换物品数

/////////////////////////////////////////////////  随机活动-秘境寻宝3/欢乐砸蛋/欢乐摇奖  /////////////////////////////
enum RA_MIJINGXUNBAO3_CHOU_TYPE							//淘宝类型
{
	RA_MIJINGXUNBAO3_CHOU_TYPE_1 = 0,					//淘宝一次
	RA_MIJINGXUNBAO3_CHOU_TYPE_10,						//淘宝十次						
	RA_MIJINGXUNBAO3_CHOU_TYPE_30,						//淘宝三十次

	RA_MIJINGXUNBAO3_CHOU_TYPE_MAX,
};

/////////////////////////////////////////////////  随机活动-单笔充值  /////////////////////////////
static const int RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT = 10;							// 单笔充值配置最大数

////////////////////////////////////////////////// 随机活动-极限挑战 ////////////////////////////////////////////////
static const int RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM = 5;			// 个人最大任务数量		

enum RA_EXREME_CHALLENGE_TASK_TYPE
{
	RA_EXTREME_CHALLENGE_ACTIVE,					// 活跃任务
	RA_EXTREME_CHALLENGE_KILL_MONSTER,				// 杀怪任务
	RA_EXTREME_CHALLENGE_ON_LINE,					// 在线任务
	RA_EXTREME_CHALLENGE_KILL_BOSS,					// BOSS任务
	RA_EXTREME_CHALLENGE_REPUTATION,				// 声望任务
	RA_EXTREME_CHALLENGE_COMPOUND,					// 合成任务

	RA_EXTREME_CHALLENGE_MAX_NUM,
};

struct RAExremeChallengeTaskInfo
{
	RAExremeChallengeTaskInfo() { this->Reset(); }

	void Reset()
	{
		task_id = 0;
		task_type = -1;
		is_finish = 0;
		is_already_fetch = 0;
		task_plan = 0;
	}

	char task_id;				// 任务id
	char task_type;				// 任务类型
	char is_finish;				// 是否完成
	char is_already_fetch;		// 是否领取
	unsigned int task_plan;		// 任务进度
};

/////////////////////////////////////	组合购买		/////////////////////////
static const int RA_COMBINE_BUY_BUCKET_ITEM_COUNT = 6;				// 购物车最大数量
static const int RA_COMBINE_BUY_MAX_ITEM_COUNT = 96;				// 配置物品种类数
static const int RA_COMBINE_BUY_LIMIT_BUY_ITEM_COUNT = 255;			// 限购物品数量

//////////////////////////////////////////////// 活动找回 ////////////////////////////////////////////////
enum ACTIVITY_FIND_TYPE
{
	ACTIVITY_FIND_TYPE_GONGCHENGZHAN = 0,				// 攻城战 6
	ACTIVITY_FIND_TYPE_TIANJIANGCAIBAO = 1,				// 天降财宝
	ACTIVITY_FIND_TYPE_QUESTION_2 = 2,					// 答题-新版
	ACTIVITY_FIND_TYPE_MILLIONAIRE = 3,					// 大富豪	25
	ACTIVITY_FIND_TYPE_WANGLINGEXPLORE = 4,				// 王陵探险	26
	ACTIVITY_FIND_TYPE_QUNXIANLUANDOU = 5,				// 元素战场	5
	ACTIVITY_FIND_TYPE_GUILDBATTLE = 6,					// 公会争霸	21
	ACTIVITY_FIND_TYPE_TERRITORYWAR = 7,				// 领土战
	ACTIVITY_FIND_TYPE_FISHING = 8,						// 钓鱼		3084
	ACTIVITY_FIND_TYPE_BANGPAIDATI = 9,					// 帮派答题	30
	ACTIVITY_FIND_TYPE_SHUIJING = 10,					// 水晶		14
	ACTIVITY_FIND_TYPE_WORSHIP = 11,					// 膜拜城主	32
	ACTIVITY_FIND_TYPE_NIGHT_FIGHT_FB = 12,				// 夜战王城	23
	ACTIVITY_FIND_TYPE_MESS_BATTLE_FB = 13,				// 乱斗战场	31
	ACTIVITY_FIND_TYPE_GUILD_YUNBIAO = 14,				// 仙盟运镖	27
	ACTIVITY_FIND_TYPE_GUILD_FB = 15,					// 帮派试炼  34
	ACTIVITY_FIND_TYPE_GUILD_YANHUI = 16,				// 仙盟宴会	33
	ACTIVITY_FIND_TYPE_CROSS_1V1 = 17,					// 跨服1v1 3074
	ACTIVITY_FIND_TYPE_CROSS_MULTIUSER_CHALLENGE = 18,	// 跨服3V3 3075
	ACTIVITY_FIND_TYPE_HOT_SPRING = 19,					// 跨服温泉 3080
	ACTIVITY_FIND_TYPE_CROSS_SHUIJING = 20,				// 跨服水晶	3081
	ACTIVITY_FIND_TYPE_CROSS_GUILDBATTLE = 21,			// 跨服帮派战  3082
	ACTIVITY_FIND_TYPE_DARK_NIGHT = 22,					// 跨服月黑风高 3083
	ACTIVITY_FIND_TYPE_CROSS_NIGHT_FIGHT_FB = 23,		// 跨服夜战王城 3085
	ACTIVITY_FIND_TYPE_CROSS_MESS_BATTLE_FB = 24,		// 跨服乱斗战场	3086
	ACTIVITY_FIND_TYPE_LIEKUN_FB = 25,					// 猎鲲地带	3087
	ACTIVITY_FIND_TYPE_CHU_MO = 26,						// 六界除魔
	ACTIVITY_FIND_TYPE_XIULUOTOWER = 27,				// 跨服修罗塔

	ACTIVITY_FIND_TYPE_MAX,
};

/////////////////////////////////////	天天返利		/////////////////////////
static const int DAY_CHONGZHI_REWARD_FLAG_LIST_LEN = 4;

static const int ACTIVITY_FIND_CONFIG_LEVEL_INTERVAL_COUNT = 200;	// 活动找回配置 最大等级间隔数量
static const int ACTIVITY_FIND_CONFIG_REWARD_ITEM_MAX_COUNT = 3;	// 活动找回配置 最大奖励物品数
static const int ACTIVITY_OPEN_RECORD_MAX_COUNT = 3;				// 活动开启记录最大天数

struct ActivityOpenRecords											// 活动开启记录
{
	ActivityOpenRecords() 
	{
		this->Reset();
	}

	void Reset()
	{
		memset(record_list, 0, sizeof(record_list));
	}

	void OnResetData()
	{
		for (int i = ACTIVITY_OPEN_RECORD_MAX_COUNT - 1; i > 0 ; -- i)
		{
			record_list[i] = record_list[i - 1];
		}

		record_list[0] = 0;
	}

	bool IsActivityOpen(int pass_day, int activity_find_type)
	{
		if (pass_day < 0 || pass_day >= ACTIVITY_OPEN_RECORD_MAX_COUNT || activity_find_type < 0 || activity_find_type >= (int)sizeof(record_list[0]) * 8)
		{
			return false;
		}

		bool is_open = 0 != (record_list[pass_day] & (1 << activity_find_type));

		return is_open;
	}

	int record_list[ACTIVITY_OPEN_RECORD_MAX_COUNT];
};

//////////////////////////////////////////////// 七天挑战活动 ////////////////////////////////////////////////
static const int SEVEN_DAY_CHALLENGE_SUB_ACTIVITY_REWARD_CFG_MAX_COUNT  = 10;	// 七天挑战子活动可配置的最大奖励数

enum SEVEN_DAY_CHALLENGE_SUB_TYPE
{
	SDCST_INVALID = 0,
	SDCST_XUYUTIAOZHAN,
	SDCST_JINGYINGXUYU,
	SDCST_SHENMOZHIJING,
	SDCST_JINGYINGSHENMO,
	SDCST_XIANHUNTASHEN,
	SDCST_BOSSTIAOZHAN,
	SDCST_XIEMEITONGYOU,

	SDCST_MAX
};

///////////////////////////////////////////////////////////  幸运转盘相关  /////////////////////////////////////////////////////////////
const static int LUCKY_ROLL_REWARD_RECORD_MAX_COUNT = 10;

enum LUCKY_ROLL_REWARD_TYPE
{
	LUCKY_ROLL_REWARD_TYPE_INVALID = 0,
	LUCKY_ROLL_REWARD_TYPE_ITEM, 
	LUCKY_ROLL_REWARD_TYPE_GOLD, 

	LUCKY_ROLL_REWARD_TYPE_MAX,
};
struct LuckyRollRewardRecord										// 幸运转盘获奖记录
{
	LuckyRollRewardRecord() { this->Reset(); }

	void Reset() 
	{ 
		user_id = 0; 
		memset(user_name, 0, sizeof(user_name));
		reward_type = LUCKY_ROLL_REWARD_TYPE_INVALID; 
		num = 0; 
		item_id = 0; 
		gold = 0; 
		time_stamp = 0; 
	}

	int user_id;
	GameName user_name;
	char reward_type;
	char num;
	ItemID item_id;
	int gold;
	unsigned int time_stamp;
};

/////////////////////////////////////////////////////////// 限时兑换装备活动 //////////////////////////////////////////////////////////
static const int TIME_LIMIT_EXCHANGE_ITEM_COUNT = 10;

///////////////////////////////////////////////////////////  封测活动相关  /////////////////////////////////////////////////////////////
const static int CLOSE_BETA_CHALLENGE_FIELD_REWARD_TIMES = 2;		// 封测活动期间，竞技场额外奖励倍数


//////////////////////////////////////////////// 一站到底 相关定义 ////////////////////////////////////////////////

static const int YIZHANDAODI_REALIVE_POS_MAX = 16;								// 复活点个数
static const int YIZHANDAODI_JISHA_REWARD_NUM = 16;								// 击杀奖励个数
static const int YIZHANDAODI_JISHA_TITLE_MAX = 16;								// 击杀称号配置
static const int YIZHANDAODI_LUACY_ROLE_MAX = 5;								// 幸运玩家个数
static const int YIZHANDAODI_RANK_NUM = 100;									// 一站到底 实时排行榜信息个数
static const int YIZHANDAODI_JISHA_SCORE_REWARD_MAX = 16;						// 一站到底 击杀积分兑换荣誉最大个数
static const int YIZHANDAODI_REWARD_TITLE_RANK_COUNT = 3;						// 一战到底，前n名领取称号奖励
UNSTD_STATIC_CHECK(YIZHANDAODI_REWARD_TITLE_RANK_COUNT <= YIZHANDAODI_RANK_NUM);


struct YiZhanDaoDiUser
{
	YiZhanDaoDiUser() 
		: user_id(INVALID_USER_ID),
		jisha_count(0), jisha_title_id(0), dead_count(0), lucky_count(0), jisha_score(0), gongji_guwu_per(0), maxhp_guwu_per(0),lianzhan(0),
		assist(0),uid(0), kill_num_reward_flag(0), hp_when_leave(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UserID user_id;
	GameName user_name;

	int jisha_count;
	int jisha_title_id;
	int dead_count;
	int lucky_count;
	int jisha_score;

	int gongji_guwu_per;
	int maxhp_guwu_per;
	int lianzhan;
	int assist;
	int uid;
	int kill_num_reward_flag;
	Attribute hp_when_leave;
	std::vector<ItemConfigData> reward_item_vec;
	std::map<UserID, time_t> hurt_map;
};

typedef std::map<UserID, YiZhanDaoDiUser> YiZhanDaoDiUserMap;
typedef std::map<UserID, YiZhanDaoDiUser>::iterator YiZhanDaoDiUserMapIt;

struct YiZhanDaoDiUserRankItem
{
	YiZhanDaoDiUserRankItem() : user_id(INVALID_USER_ID), jisha_count(0), dead_count(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UserID user_id;	
	GameName user_name;																// 角色名
	short jisha_count;																// 角色在战场中的阵营
	short dead_count;																// 斩杀人数
};

//////////////////////////////////////////////// 五行挂机 相关定义 ////////////////////////////////////////////////
static const int WUXINGGUAJI_STUFF_MAX = 5;											// 材料个数
static const int WUXINGGUAJI_TARGET_MAX = 5;										// 目标个数
static const int WUXINGGUAJI_MAX_RELACE_BOSS_NUM = 10;								// 因世界等级替换BOSS的最大数量
static const int WUXINGGUAJI_BOSS_NUM = 1;											// BOSS的最大数量
static const int WUXINGGUAJI_FLUSH_BOSS_INTERVAL = 4 * 60;							// BOSS刷新间隔
static const int WUXINGGUAJI_LUACY_ROLE_MAX = 5;									// 幸运玩家个数

struct WuXingGuaJiTargetInfo
{
	WuXingGuaJiTargetInfo() : is_finish(0), is_getreward(0) 
	{
		memset(stuff_id_list, 0, sizeof(stuff_id_list));
		memset(stuff_num_list, 0, sizeof(stuff_num_list));
	}

	bool IsFinish() { return 0 != is_finish; }
	bool IsGetReward() { return 0 != is_getreward; }

	int stuff_id_list[WUXINGGUAJI_STUFF_MAX];
	int stuff_num_list[WUXINGGUAJI_STUFF_MAX];
	short is_finish;
	short is_getreward;
};

struct WuXingGuaJiUser
{
	WuXingGuaJiUser() 
		: user_id(INVALID_USER_ID), lucky_count(0)
	{
	}

	UserID user_id;
	int lucky_count;
	GSNetID net_id;

	WuXingGuaJiTargetInfo target_list[WUXINGGUAJI_TARGET_MAX];
};

typedef std::map<UserID, WuXingGuaJiUser> WuXingGuaJiUserMap;
typedef std::map<UserID, WuXingGuaJiUser>::iterator WuXingGuaJiUserMapIt;

//////////////////////////////////////////////// 随机活动-聚宝盆 相关定义 ////////////////////////////////////////////////

struct RACornucopiaRewardRecord										// 聚宝盆记录
{
	RACornucopiaRewardRecord() { this->Reset(); }

	void Reset() 
	{ 
		user_id = 0; 
		memset(user_name, 0, sizeof(user_name));
		reward_gold = 0;
		camp = 0;
		reserve_ch = 0;
		need_put_gold = 0;
	}

	int user_id;
	GameName user_name;
	short reward_gold;
	char camp;
	char reserve_ch;
	int need_put_gold;
};

const static int RAND_ACTIVITY_CORNUCOPIA_RECORD_MAX_COUNT = 100;	// 聚宝盆记录最大数量

//////////////////////////////////////////////// 合服活动 ////////////////////////////////////////////////
static const int COMBINE_SERVER_ACTIVITY_RANK_REWARD_ROLE_NUM = 3;		// 排名奖励 奖励人数
static const int COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE = 3;			// 排行抢购物品类型
static const int COMBINE_SERVER_ROLL_CFG_MAX_COUNT = 16;				// 摇奖配置最大数
static const int COMBINE_SERVER_SINGLE_CHARGE_CONFIG_MAX_COUNT = 10;	// 单笔充值配置最大数
static const int COMBINE_SERVER_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT = 32;// 登录送礼奖励最大配置个数
static const int COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT = 16;	// 全服抢购物品种类数量
static const int COMBINE_SERVER_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT = 8;	// 个人抢购物品种类数量
static const int COMBINE_SERVER_BOSS_RANK_NUM = 10;						// 合服boss排行榜上榜数
static const int COMBINE_SERVER_BOSS_MAX_COUNT = 15;					// 合服boss最大数量
static const int COMBINE_SERVER_CONSUME_RANK_COUNT = 10;				// 合服消费排行数量(暂时是消费和充值排行人数一致,不需在排序那里分类)
static const int COMBINE_SERVER_CHONGZHI_RANK_COUNT = 10;				// 合服充值排行数量
static const int COMBINE_SERVER_MAX_FOUNDATION_TYPE = 10;				// 成长基金最大数量
static const int COMBINE_SERVER_MAX_LOGIN_GIFT_NUM = 20;				// 登录奖励最多数量

struct CSAFoundationStatus
{
	CSAFoundationStatus() : buy_level(0), reward_phase(0)
	{}

	short buy_level;
	short reward_phase;
};

enum COMBINE_SERVER_ACTIVITY_SUB_ACTIVITY_STATE
{
	CSA_SUB_ACTIVITY_STATE_NO_START = 0,								// 未开始
	CSA_SUB_ACTIVITY_STATE_OPEN,										// 进行中
	CSA_SUB_ACTIVITY_STATE_FINISH,										// 结束
};

enum COMBINE_SERVER_ACTIVITY_SUB_TYPE									// 合服活动子类型
{
	CSA_SUB_TYPE_INVALID = 0,
	CSA_SUB_TYPE_RANK_QIANGGOU,											// 抢购
	CSA_SUB_TYPE_ROLL,													// 转盘
	CSA_SUB_TYPE_GONGCHENGZHAN,											// 攻城战
	CSA_SUB_TYPE_XIANMENGZHAN,											// 仙盟战
	CSA_SUB_TYPE_CHONGZHI_RANK,											// 充值排行
	CSA_SUB_TYPE_CONSUME_RANK,											// 消费排行
	CSA_SUB_TYPE_KILL_BOSS,												// 击杀boss
	CSA_SUB_TYPE_SINGLE_CHARGE,											// 单笔充值
	CSA_SUB_TYPE_LOGIN_Gift,											// 登录奖励
	CSA_SUB_TYPE_PERSONAL_PANIC_BUY,									// 个人抢购
	CSA_SUB_TYPE_SERVER_PANIC_BUY,										// 全服抢购
	CSA_SUB_TYPE_ZHANCHANG_FANBEI,										// 战场翻倍
	CSA_SUB_TYPE_CHARGE_REWARD_DOUBLE,									// 充值双倍返利
	CSA_SUB_TYPE_BOSS,													// 合服boss
	CSA_SUB_TYPE_TOUZIJIHUA,											// 合服投资计划
	CSA_SUB_TYPE_FOUNDATION,											// 合服成长基金
	CSA_SUB_TYPE_BUY_EXP,												// 合服经验炼制

	CSA_SUB_TYPE_MAX,
};

enum COMBINE_SERVER_ACTIVITY_RANK_TYPE
{
	CSA_RANK_TYPE_INVALID = 0,
	CSA_RANK_TYPE_QIANGGOU,
	CSA_RANK_TYPE_CHONGZHI,
	CSA_RANK_TYPE_CONSUME,

	CSA_RANK_TYPE_MAX,
};

struct CombineServerActivityOtherParam
{
	CombineServerActivityOtherParam() { this->Reset(); }

	void Reset()
	{
		memset(rank_qianggou_buynum_list, 0, sizeof(rank_qianggou_buynum_list));

		reserve_int = 0;
		csa_xmz_winner_roleid = 0;
		csa_gcz_winner_roleid = 0;

		memset(server_panic_buy_num_list, 0, sizeof(server_panic_buy_num_list));
		cas_gcz_winner_lover_id = 0;
	}

	UNSTD_STATIC_CHECK(3 == COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE);

	int rank_qianggou_buynum_list[COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE];
	int reserve_int;

	int csa_xmz_winner_roleid;
	int csa_gcz_winner_roleid;

	UNSTD_STATIC_CHECK(16 == COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT);

	int server_panic_buy_num_list[COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT];
	int cas_gcz_winner_lover_id;
};

struct CombineServerActivityRankUser
{
	CombineServerActivityRankUser() { this->Reset(); }

	void Reset() 
	{
		role_id = 0;
		rank_value = 0;
		rank_reserve = 0;
	}

	bool IsFree() { return 0 == role_id; }

	int role_id;
	long long rank_value;
	long long rank_reserve;
};

struct CombineServerActivityRankItem
{
	CombineServerActivityRankItem() { this->Reset(); }

	void Reset() 
	{
		is_finish = 0;
		reserve_ch = 0;
		reserve_sh = 0;

		for (int i = 0; i < COMBINE_SERVER_CONSUME_RANK_COUNT; ++ i)
		{
			user_list[i].Reset();
		}
	}

	char is_finish;
	char reserve_ch;
	short reserve_sh;

	CombineServerActivityRankUser user_list[COMBINE_SERVER_CONSUME_RANK_COUNT];
};

struct CombineServerActivityRankParam
{
	CombineServerActivityRankParam() { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < CSA_RANK_TYPE_MAX; ++ i)
		{
			rank_item_list[i].Reset();
		}
	}

	CombineServerActivityRankItem rank_item_list[CSA_RANK_TYPE_MAX];
};

struct CombineServerActivityBossPersonalRankItem
{
	CombineServerActivityBossPersonalRankItem()
	{
		this->Reset();
	}

	void Reset()
	{
		id = 0;
		rank_value = 0;
	}

	int id;
	int rank_value;
};

struct CombineServerActivityBossGuildRankItem
{
	CombineServerActivityBossGuildRankItem()
	{
		this->Reset();
	}

	void Reset()
	{
		id = 0;
		rank_value = 0;
	}

	int id;
	int rank_value;
};

struct CombineServerActivityBossRankParam
{
	CombineServerActivityBossRankParam()
	{
		this->Reset();
	}

	void Reset()
	{
		for (int i = 0; i < COMBINE_SERVER_BOSS_RANK_NUM; i++)
		{
			personal_rank[i].Reset();
			guild_rank[i].Reset();
		}
	}

	CombineServerActivityBossPersonalRankItem personal_rank[COMBINE_SERVER_BOSS_RANK_NUM];
	CombineServerActivityBossGuildRankItem guild_rank[COMBINE_SERVER_BOSS_RANK_NUM];
};

//////////////////////////////////////////////// 帮派答题 相关定义////////////////////////////////////////////////
static const int GUILD_QUESTION_SCORE_RANK_MAX = 10;		// 帮派答题的最大排行数

static const int GUILD_QUESTION_STRING_MAX = 256;											// 题目最大字符		
typedef char GuildQuestionStr[GUILD_QUESTION_STRING_MAX];									// 题目定义

enum GUILD_QUESTION_STATE
{
	GUILD_QUESTION_STATE_STANDY = 0,	// 准备
	GUILD_QUESTION_STATE_START,			// 开始答题
	GUILD_QUESTION_STATE_CLOSE,			// 将要关闭（也还在答题）
	GUILD_QUESTION_STATE_MAX,
};

struct GuildQuestionPlayerInfo
{
	GuildQuestionPlayerInfo() : user_id(INVALID_USER_ID), right_num(0), exp(0), guild_gongxian(0), gather_count(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UserID user_id;
	GameName user_name;

	int right_num;			// 答题正确数
	long long exp;			// 答题获得经验
	int guild_gongxian;		// 为帮派获得贡献
	int gather_count;		// 采集次数
};

typedef std::map<UserID, GuildQuestionPlayerInfo> GuildQuestionPlayerInfoMap;
typedef std::map<UserID, GuildQuestionPlayerInfo>::iterator GuildQuestionPlayerInfoMapIt;

struct GuildQuestionPersonRank
{
	GuildQuestionPersonRank() : user_id(0), right_num(0)
	{
		memset(user_name, 0, sizeof(user_name));
		memset(guild_name, 0, sizeof(guild_name));
	}

	bool operator <(const GuildQuestionPersonRank &other) const
	{
		return right_num > other.right_num;
	}

	int user_id;
	GameName user_name;					// 玩家名字
	GuildName guild_name;				// 帮派名字

	int right_num;			// 答题正确数
};

// 个人正确答题数排行
typedef std::vector<GuildQuestionPersonRank> GuilldQuestionPersonRankVec;
typedef std::vector<GuildQuestionPersonRank>::iterator GuilldQuestionPersonRankVecIt;

struct GuildQuestionGuildRank
{
	GuildQuestionGuildRank() : guild_id(0), answer_true_timestamp(0), guild_score(0)
	{
		memset(guild_name, 0, sizeof(guild_name));
	}

	GuildID guild_id;					// 帮派ID
	GuildName guild_name;				// 帮派名字
	unsigned int answer_true_timestamp;	// 答对时间戳
	int guild_score;					// 帮派积分
};

typedef std::vector<GuildQuestionGuildRank> GuildQuestionGuildRankVec;
typedef std::vector<GuildQuestionGuildRank>::iterator GuildQuestionGuildRankVecIt;

struct GuildQuestionQuestion
{
	GuildQuestionQuestion() : guild_id(0), question_number(0), question_right_num(0), question_id(-1), cur_question_timestamp(0), next_question_timestamp(0), is_aready_broadcast(false), is_already_answer_true(false) {}

	GuildID guild_id;					// 帮派ID
	int question_number;
	int question_right_num;				// 答对题目数量
	int question_id;
	std::set<int> question_set;			// 题目集合
	time_t cur_question_timestamp;		// 当前题出题时间戳
	time_t next_question_timestamp;		// 出下题时间戳
	bool is_aready_broadcast;			// 是否已经广播
	bool is_already_answer_true;
};
typedef std::map<GuildID, GuildQuestionQuestion> GuildQuestionQuestionMap;
typedef std::map<GuildID, GuildQuestionQuestion>::iterator GuildQuestionQuestionMapIt;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////// 随机活动-金银塔抽奖活动 ////////////////////////////////////////////////
static const int RA_LEVEL_LOTTERY_MAX_LEVEL_LIMIT = 6;							// 随机活动-金银塔抽奖活动，最大的层数限制
static const int RA_LEVEL_LOTTERY_MAX_REWARD_ID_LIMIT = 27;						// 随机活动-金银塔抽奖活动，最大的奖励ID限制
static const int RA_LEVEL_LOTTERY_REWARD_MAX_COUNT = 52;						// 抽奖允许最大次数
static const int RA_LEVEL_LOTTERY_HISTORY_MAX_COUNT = 10;						// 抽奖历史纪录最大数量

struct RALevelLotteryHistoryItem
{
	RALevelLotteryHistoryItem() : uid(0), reward_index(-1) { memset(user_name, 0, sizeof(user_name)); }

	GameName user_name;
	int uid;
	int reward_index; // 玩家抽取到的奖励id
};

////////////////////////////////////////////////// 随机活动-充值扭蛋活动 ////////////////////////////////////////////////
static const int RA_NIU_EGG_CHOU_REWARD_CFG_COUNT_LIMIT = 26;					// 扭蛋抽奖物品配置数量
static const int RA_NIU_EGG_SERVER_REWARD_CFG_COUNT_LIMIT = 6;					// 扭蛋全服奖励物品列表
static const int RA_NIU_EGG_CHOU_HISTORY_MAX_COUNT = 10;						// 抽奖历史纪录最大数量

struct RAChongzhiNiuEggHistoryItem
{
	RAChongzhiNiuEggHistoryItem() { this->Reset(); }
	
	void Reset()
	{
		uid = 0;
		reward_req = -1;
		memset(user_name, 0, sizeof(user_name));
	}
		
	GameName user_name;
	int uid;
	int reward_req; // 玩家抽取到的奖励下标
};

////////////////////////////////////////////////// 珍宝阁活动 ////////////////////////////////////////////////
static const int RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT = 9;				// 显示可买物品列表数量
static const int RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT = 5;			// 保存个人增加权重数量

UNSTD_STATIC_CHECK((RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT + 1) % 2 == 0);

struct ZhenbaogeAddWeightInfo
{
	ZhenbaogeAddWeightInfo() : seq(0), weight(0){}

	short seq;
	short weight;
};

////////////////////////////////////////////////// 秘境寻宝活动 ///////////////////////////////////////////////
static const int RAND_ACTIVITY_MIJINGXUNBAO_ADD_WEIGHT_COUNT = 10;			// 保存个人增加权重数量

struct MiJingXunBaoAddWeightInfo
{
	MiJingXunBaoAddWeightInfo() : seq(0), weight(0), tao_none_rare_times(0){}

	short seq;
	short weight;
	int tao_none_rare_times;
};

enum RA_MIJINGXUNBAO_CHOU_TYPE							//淘宝类型
{
	RA_MIJINGXUNBAO_CHOU_TYPE_1 = 0,					//淘宝一次
	RA_MIJINGXUNBAO_CHOU_TYPE_10,						//淘宝十次						
	RA_MIJINGXUNBAO_CHOU_TYPE_50,						//淘宝五十次

	RA_MIJINGXUNBAO_CHOU_TYPE_MAX,
};

////////////////////////////////////////////////// 随机活动-摇钱树活动 ////////////////////////////////////////////////
static const int RA_MONEY_TREE_REWARD_CFG_COUNT = 40;						// 摇钱树奖品配置数量
static const int RA_MONEY_TREE_SERVER_REWARD_CFG_COUNT = 6;					// 摇钱树全服奖励物品列表

////////////////////////////////////////////////// 随机活动-陛下请翻牌活动 ////////////////////////////////////////////////
static const int RA_KING_DRAW_LEVEL_COUNT = 3;									// 有三个等级的翻牌：初级，中级，高级
static const int RA_KING_DRAW_MAX_REWARD_COUNT = 50;							// 活动最多的奖励数（发给客户端）
UNSTD_STATIC_CHECK(RA_KING_DRAW_MAX_REWARD_COUNT <= MAX_ROLE_CHEST_GRID);
static const int RA_KING_DRAW_MAX_SHOWED_COUNT = 9;								// 活动显示的可翻牌数
static const int RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT = 5;						// 可以自增权重的奖励配置数
static const int RA_KING_DRAW_MAX_RETURN_REWARD = 10;							// 累抽奖励最大数目
UNSTD_STATIC_CHECK(1 == RA_KING_DRAW_MAX_SHOWED_COUNT % 2);

// 某一等级的翻牌信息
struct RAKingDrawCardList
{
	RAKingDrawCardList() { this->Reset(); }

	void Reset()
	{
		reserve_sh = 0;

		for (int i = 0; i < RA_KING_DRAW_MAX_SHOWED_COUNT; ++i)
		{
			reward_list[i] = -1;										// -1代表没翻牌
		}
	}

	short reward_list[RA_KING_DRAW_MAX_SHOWED_COUNT];					// 值是奖励seq
	short reserve_sh;
};

// 某一等级的翻牌的自增权重情况
struct RAKingDrawSelfWeight
{
	RAKingDrawSelfWeight() { this->Reset(); }

	void Reset()
	{
		seq = -1;
		auto_weight_add = 0;
	}

	short seq;															// 奖励的seq
	short auto_weight_add;												// 当前的weight自增值
};


////////////////////////////////////////////////// 被动变身活动 ////////////////////////////////////////////////

static const int RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT = 10;						// 随机活动-特殊形象变身卡排行最大数量

struct RASpecialAppearacePlayerRankInfo
{
	RASpecialAppearacePlayerRankInfo() : uid(0), change_num(0), total_capablity(0), avatar(0), sex(0), prof(0), reserve_1(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	int uid;
	GameName user_name;
	int change_num;
	int total_capablity;
	char avatar;
	char sex;
	char prof;
	char reserve_1;
};

static const int RA_SPECIAL_APPEARANCE_PASSIVE_RANK_MAX_COUNT = 10;						// 随机活动-特殊形象被变身排行最大数量

struct RASpecialAppearacePassivePlayerRankInfo
{
	RASpecialAppearacePassivePlayerRankInfo() : uid(0), change_num(0), total_capablity(0) , avatar(0), sex(0), prof(0), reserve_1(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	int uid;
	GameName user_name;
	int change_num;
	int total_capablity;
	char avatar;
	char sex;
	char prof;
	char reserve_1;
};

////////////////////////////////////////////////// 随机活动-挖矿活动 ////////////////////////////////////////////////
static const int RA_MINE_MAX_REFRESH_COUNT = 8;							// 一次刷新出的最大矿石数目
static const int RA_MINE_MAX_TYPE_COUNT = 12;							// 矿石类型最大数目
static const int RA_MINE_REWARD_MAX_COUNT = 900;						// 全服礼包最大数
static const int RA_MINE_REFRESH_TIME_INTERVAL = SECOND_PER_HOUR * 4;	// 系统自动刷矿时间间隔
UNSTD_STATIC_CHECK(0 == RA_MINE_MAX_TYPE_COUNT % 2)
static const int RA_MINE_MAX_CFG_COUNT = 900;							// 最大配置数量

enum RA_MINE_TYPES
{
	RA_MINE_TYPES_INVALID = 0,

	RA_MINE_TYPES_BEGIN = 10,

	RA_MINE_TYPES_END = RA_MINE_TYPES_BEGIN,
};

////////////////////////////////////////////////// 随机活动-顶刮刮活动 ////////////////////////////////////////////////
static const int RA_GUAGUA_MAX_REWARD_COUNT = 50;						// 最多奖励数目
static const int RA_GUAGUA_ICON_TYPE_COUNT = 10;						// 图标类型数目
static const int RA_GUAGUA_REWARD_AREA_COUNT = 5;						// 刮奖区域数目
static const int RA_GUAGUA_REWARD_COUNT_PER_AREA = 3;					// 单个刮奖区域的奖励数
static const int RA_GUAGUA_ACC_REWARD_COUNT = 16;						// 累计抽奖的奖励数

enum RA_GUAGUA_PLAY_MULTI_TYPES											//刮奖多次的类型
{
	RA_GUAGUA_PLAY_ONE_TIME = 0,										// 刮奖1次
	RA_GUAGUA_PLAY_TEN_TIMES = 1,										// 刮奖10次
	RA_GUAGUA_PLAY_THIRTY_TIMES = 2,									// 刮奖30次

	RA_GUAGUA_TYPE_MAX
};

////////////////////////////////////////////////// 随机活动-天命卜卦活动 ////////////////////////////////////////////////
static const int RA_TIANMING_LOT_COUNT = 6;								// 可加注标签数量
static const int RA_TIANMING_REWARD_HISTORY_COUNT = 20;					// 奖励历史记录数量

struct RATianMingRewardHistoryItem
{
	RATianMingRewardHistoryItem() { this->Reset(); }

	void Reset()
	{
		seq = 0;
		add_lot = 0;
	}

	char seq;															// 奖池下标
	char add_lot;														// 获奖加注倍数
};

////////////////////////////////////////////////// 随机活动-翻翻转活动 ////////////////////////////////////////////////
static const int RA_FANFAN_MAX_ITEM_COUNT = 50;						// 最大奖励数量
static const int RA_FANFAN_MAX_WORD_COUNT = 10;						// 最大字组数量
static const int RA_FANFAN_CARD_COUNT = 40;							// 可翻牌数
static const int RA_FANFAN_LETTER_COUNT_PER_WORD = 4;				// 每个字组的字数
static const int RA_FANFAN_BAODI_REWARD_CFG_COUNT = 10;				// 累抽返利配置最大数量
UNSTD_STATIC_CHECK(0 == RA_FANFAN_MAX_WORD_COUNT % 2)
UNSTD_STATIC_CHECK(RA_FANFAN_CARD_COUNT == 40)

enum RA_FANFAN_CARD_TYPE
{
	RA_FANFAN_CARD_TYPE_BEGIN,

	RA_FANFAN_CARD_TYPE_HIDDEN = RA_FANFAN_CARD_TYPE_BEGIN,			// 隐藏卡牌类型
	RA_FANFAN_CARD_TYPE_ITEM_BEGIN = 100,							// 物品卡牌类型起始值
	RA_FANFAN_CARD_TYPE_WORD_BEGIN = 200,							// 字组卡牌类型起始值

	RA_FANFAN_CARD_TYPE_MAX,
};
UNSTD_STATIC_CHECK(RA_FANFAN_CARD_TYPE_WORD_BEGIN - RA_FANFAN_CARD_TYPE_ITEM_BEGIN >= RA_FANFAN_MAX_ITEM_COUNT);
UNSTD_STATIC_CHECK(UCHAR_MAX - RA_FANFAN_CARD_TYPE_WORD_BEGIN >= RA_FANFAN_MAX_WORD_COUNT);

struct RAFanFanHiddenWordInfo
{
	static const int POS_BEGIN = 100;

	RAFanFanHiddenWordInfo() { this->Reset(); }

	void Reset()
	{
		hidden_word_seq = 0;
		memset(reserve, 0, sizeof(reserve));
		memset(hidden_letter_pos_list, 0, sizeof(hidden_letter_pos_list));
	}

	bool IsValid()
	{
		int hidden_word_real_seq = hidden_word_seq - RA_FANFAN_CARD_TYPE_WORD_BEGIN;

		return (hidden_word_real_seq >= 0 && hidden_word_real_seq < RA_FANFAN_MAX_WORD_COUNT);
	}

	int GetHiddenLetterCount()
	{
		int hidden_letter_count = 0;
		for (int i = 0; i < RA_FANFAN_LETTER_COUNT_PER_WORD; ++ i)
		{
			if (0 == hidden_letter_pos_list[i])
			{
				++ hidden_letter_count;
			}
		}

		return hidden_letter_count;
	}

	unsigned char hidden_word_seq;				// 对应字组seq
	char reserve[3];
	unsigned char hidden_letter_pos_list[RA_FANFAN_LETTER_COUNT_PER_WORD];
};

struct RAFanFanWordActiveInfo
{
	static const int MAX_WORD_ACTIVE_COUNT = 99;

	RAFanFanWordActiveInfo() { this->Reset(); }

	void Reset()
	{
		active_flag = 0;
		active_count = 0;
	}

	int ActiveWord()
	{
		int active_bit = 0;
		for (int bit = 0; bit < RA_FANFAN_LETTER_COUNT_PER_WORD; ++ bit)
		{
			if (0 == ((1 << bit) & active_flag))
			{
				active_bit = bit;
				active_flag |= (1 << active_bit);

				break;
			}
		}

		UNSTD_STATIC_CHECK(RA_FANFAN_LETTER_COUNT_PER_WORD == 4);
		if (active_flag >= 0x0f)
		{
			active_flag = 0;
			++ active_count;
		}

		return active_bit;
	}

	bool IsLetterActive(int bit) { return (0 != ((1 << bit) & active_flag)); }

	void ClearActiveFlag() { active_flag = 0; }

	unsigned char active_flag;
	unsigned char active_count;
};

////////////////////////////////////////////////// 随机活动-连续充值 ////////////////////////////////////////////////
static const int RA_CONTINUE_CHONGZHI_MAX_DAY_INDEX = 15;				// 连续充值、连充特惠初、连充特惠高 活动最大天数

////////////////////////////////////////////////// 随机活动-连续消费 ////////////////////////////////////////////////
static const int RA_CONTINUE_CONSUME_MAX_DAY_INDEX = 15;				// 连续消费活动最大天数

////////////////////////////////////////////////// 随机活动-军号嘹亮 ////////////////////////////////////////////////
enum ARMY_SIDE_TYPE
{
	INVALID_ARMY_SIDE = -1,
	BEGIN_ARMY_SIDE = 0,
	BLUE_ARMY_SIDE = BEGIN_ARMY_SIDE,
	RED_ARMY_SIDE,
	YELLOW_ARMY_SIDE,

	MAX_ARMY_SIDE,
};

static const int RA_ARMY_DAY_ARMY_SIDE_NUM = MAX_ARMY_SIDE - BEGIN_ARMY_SIDE;
static const int RA_ARMY_DAY_ACTIVE_DEGREE_EXCHANGE_NUM = 20;							// 活跃度兑换奖励配置数

////////////////////////////////////////////////// 随机活动-循环充值 ////////////////////////////////////////////////
static const int RA_CIRCULATION_CHONGZHI_MAX_REWARD_COUNT = 15;			// 循环充值活动最多奖励数目

////////////////////////////////////////////////// 随机活动-至尊幸运 ////////////////////////////////////////////////
static const int RA_EXTREME_LUCKY_DRAW_CONSUME_GOLD_CFG_COUNT = 10;			// 
static const int RA_EXTREME_LUCKY_DRAW_REWARD_CFG_COUNT = 100;				// 全部抽奖奖品数
static const int RA_EXTREME_LUCKY_PER_TURN_REWARD_COUNT = 10;				// 每轮抽奖物品个数
static const int RA_EXTREME_LUCKY_NEXT_FLUSH_INTERVAL = 4;					// 下次刷新间隔(小时)
static const int RA_EXTREME_LUCKY_RETURN_REWARD_CFG_COUNT = 900;			// 最大返利配置数
static const int RA_EXTREME_LUCKY_RETURN_REWARD_ONCE_COUNT = 10;
UNSTD_STATIC_CHECK(RA_EXTREME_LUCKY_RETURN_REWARD_ONCE_COUNT < 16)

struct RAExtremeLuckyRewardInfo
{
	RAExtremeLuckyRewardInfo() : seq(0), invalid(0), has_fetch(0), weight(0)
	{
		
	}

	short seq;
	char invalid;
	char has_fetch;
	int weight;
};

static const int RA_CONSUM_GIFT_MAX_ROLL_COUNT = 10;		//随机活动-消费领奖单次最大摇奖次数

////////////////////////////////////////////////// 随机活动-欢度国庆 ////////////////////////////////////////////////
static const int RA_NATIONAL_DAY_EXCHANGE_CFG_COUNT = 10;					//欢度国庆兑换物品数

////////////////////////////////////////////////// 随机活动-步步高升 ////////////////////////////////////////////////
enum RA_PROMOTING_POSITION_REWARD_TYPE			//奖励类型 
{
	RA_PROMOTING_POSITION_REWARD_TYPE_GOLD,		// 元宝
	RA_PROMOTING_POSITION_REWARD_TYPE_ITEM,		// 物品
	RA_PROMOTING_POSITION_REWARD_TYPE_INSIDE,	// 内圈
	RA_PROMOTING_POSITION_REWARD_TYPE_SPLIT,	// 双开

	RA_PROMOTING_POSITION_REWARD_TYPE_MAX
};

enum RA_PROMOTING_POSITION_PLAY_TYPE				// 抽奖方式
{
	RA_PROMOTING_POSITION_PLAY_TYPE_INVALID = -1,
	RA_PROMOTING_POSITION_PLAY_TYPE_FREE,			// 免费抽
	RA_PROMOTING_POSITION_PLAY_TYPE_GOLD,			// 元宝抽
	RA_PROMOTING_POSITION_PLAY_TYPE_EXTRA_TIMES,	// 充值赠送次数抽
	RA_PROMOTING_POSITION_PLAY_TYPE_USE_ITEM,		// 替代物品抽

	RA_PROMOTING_POSITION_PLAY_TYPE_MAX
};

static const int RA_PROMOTING_POSITION_OUTSIDE_REWARD_MAX_SEQ = 255;		// 外圈最多奖励数
static const int RA_PROMOTING_POSITION_INSIDE_REWARD_MAX_SEQ = 255;			// 内圈最多奖励数
static const int RA_PROMOTION_POSITION_MULTIPLAY_TIMES = 30;				// 多抽次数
static const int RA_PROMOTION_POSITION_REWARD_RECORDS_MAX_COUNT = 14;		// 抽奖历史记录最大数
static const int RA_PROMOTION_POSITION_REWARD_CFG_MAX_COUNT = 200;			// 抽奖返利配置最大数
static const int RA_PROMOTION_POSITION_REWARD_CFG_INTERVAL = 10;

enum RA_PROMOTING_POSITION_CIRCLE_TYPE
{
	RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE,
	RA_PROMOTING_POSITION_CIRCLE_TYPE_INSIDE,
};

struct RAPromotingPositionInfo					// 摇奖位置
{
	RAPromotingPositionInfo() { this->Reset(); }

	void Reset()
	{
		circle_type = RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE;
		position = 0;
	}

	char circle_type;
	unsigned char position;						// 0代表起点，其余取值和配置项seq对应
};

struct RAPromotingPositionRewardInfo			// 奖励信息
{
	RAPromotingPositionRewardInfo() : circle_type(RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE), seq(0) {}

	char circle_type;
	unsigned char seq;
};

// 获奖历史记录信息
struct RAPromotingPositionRewardRecord
{
	RAPromotingPositionRewardRecord() : uid(0), reserve_sh(0) { memset(user_name, 0, sizeof(user_name)); }

	GameName user_name;
	int uid;
	RAPromotingPositionRewardInfo reward_info;
	short reserve_sh;
};

////////////////////////////////////////////////// 随机活动-黑市拍卖 ////////////////////////////////////////////////
static const int RA_BLACK_MARKET_MAX_ITEM_COUNT = 3;		// 拍卖物品最大数
static const int RA_BLACK_MARKET_MAX_DAY_COUNT = 7;			// 最多拍卖日数量

struct RABlackMarketItemInfo		// 拍卖物品信息
{
	RABlackMarketItemInfo() { this->Reset(); }

	void Reset()
	{
		seq = -1;
		is_offered = 0;
		cur_price = 0;
		buyer_uid = 0;
	}

	void SetItemInfo(int _seq, int _is_offered, int _cur_price, int _buyer_uid)
	{
		seq = _seq;
		is_offered = _is_offered;
		cur_price = _cur_price;
		buyer_uid = _buyer_uid;
	}

	int seq;						// 物品的配置seq
	int is_offered;					// 是否被要价过
	int cur_price;					// 当前拍卖价格
	int buyer_uid;					// 竞价玩家
};

////////////////////////////////////////////////// 随机活动-珍宝商城 ////////////////////////////////////////////////
static const int RA_TREASURES_MALL_MAX_BUY_ITEM_COUNT = 10;					// 珍宝商城购买物品数
static const int RA_TREASURES_MALL_MAX_EXCHANGE_ITEM_COUNT = 10;			// 珍宝商城可兑换物品数

////////////////////////////////////////////////// 随机活动-新聚宝盆 ////////////////////////////////////////////////
enum RA_NEW_CORNUCOPIA_TASK_TYPE
{
	RA_NEW_CORNUCOPIA_TASK_TYPE_MOUNT_UPGRADE,								// 坐骑达到指定阶数
	RA_NEW_CORNUCOPIA_TASK_TYPE_HALO_UPGRADE,								// 光环达到指定阶数
	RA_NEW_CORNUCOPIA_TASK_TYPE_WING_UPGRADE,								// 羽翼达到指定阶数
	RA_NEW_CORNUCOPIA_TASK_TYPE_SHENGONG_UPGRADE,							// 神弓达到指定阶数
	RA_NEW_CORNUCOPIA_TASK_TYPE_SHENYI_UPGRADE,								// 神翼达到指定阶数
	RA_NEW_CORNUCOPIA_TASK_TYPE_WORLD_BOSS,									// 参与世界BOSS
	RA_NEW_CORNUCOPIA_TASK_TYPE_DAILY_TASK,									// 完成日常任务
	RA_NEW_CORNUCOPIA_TASK_TYPE_GUILD_TASK,									// 完成公会任务
	RA_NEW_CORNUCOPIA_TASK_TYPE_CROSS_TEAM_FB,								// 参与跨服组队副本
	RA_NEW_CORNUCOPIA_TASK_TYPE_HUSONG,										// 运镖
	RA_NEW_CORNUCOPIA_TASK_TYPE_STRENGTHEN,									// 强化
	RA_NEW_CORNUCOPIA_TASK_TYPE_CROSS_1V1,									// 参与跨服1V1
	RA_NEW_CORNUCOPIA_TASK_TYPE_UPGRADE_FB,									// 参与进阶副本

	RA_NEW_CORNUCOPIA_TASK_TYPE_MAX,
};

////////////////////////////////////////////////集字活动//////////////////////////////////////////
static const int RAND_ACTIVITY_ITEM_COLLECTION_REWARD_MAX_COUNT = 5;		// 集字活动配置最大个数

////////////////////////////////////////////////我们结婚吧//////////////////////////////////////////
static const int RA_MARRY_REWARD_COUPLE_COUNT_MAX = 100;					// 我们结婚吧最多奖励对数
static const int RA_MARRY_SHOW_COUPLE_COUNT_MAX = 10;						// 我们结婚吧最多显示对数

////////////////////////////////////////////////开服投资//////////////////////////////////////////
static const int RA_OPEN_SERVER_INVEST_TYPE_MAX = 3;						// 开服投资类型数量

///////////////////////////////////////////////星座遗迹///////////////////////////////////////////
static const int XINGZUOYIJI_BOX_TYPE_MAX = 4;								// 星座遗迹箱子类型数量

struct CoupleInfo
{
	CoupleInfo() { this->Reset(); }

	void Reset()
	{
		propose_id = 0;
		accept_proposal_id = 0;
	}

	bool is_valid() { return (0 != propose_id && 0 != accept_proposal_id); }

	void SetCouple(int _accept_proposal_id, int _proposal_id)
	{
		propose_id = _proposal_id;
		accept_proposal_id = _accept_proposal_id;
	}

	int	propose_id;
	int accept_proposal_id;
};

////////////////////////////////////////////////// 聚划算 ////////////////////////////////////////////////
static const int RAND_ACTIVITY_XIANYUAN_TREAS_COUNT = 10;

struct XianyuanTreasInfo
{
	XianyuanTreasInfo() : num(0), buy_day_index(0) {}

	short num;					//	当前已领取的次数
	short buy_day_index;
};

////////////////////////////////////////////////// 限时秒杀 ////////////////////////////////////////////////
static const int RA_RUSH_BUYING_SALES_ITEMS_COUNT = 8;					// 最多可抢购的物品
static const int RA_RUSH_BUYING_PHASES_COUNT = 4;						// 抢购阶段数
static const int RA_RUSH_BUYING_MAX_SERVER_BUYING_TIMES = 1000;			// 全服最大抢购次数（单个物品）
static const int RA_RUSH_BUYING_MAX_ROLE_BUYING_TIMES = 100;			// 玩家最大抢购次数（单个物品）
UNSTD_STATIC_CHECK(RA_RUSH_BUYING_MAX_ROLE_BUYING_TIMES <= CHAR_MAX);

////////////////////////////////////////////////// 随机活动-地图寻宝 ////////////////////////////////////////////////
static const int MAX_RA_MAP_HUNT_CITY_COUNT = 20;							// 最多可探险城市20个
static const int MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE = 3;					// 一条路线最多3个城市
static const int MAX_RA_MAP_HUNT_ROUTE_COUNT = MAX_RA_MAP_HUNT_CITY_COUNT;	// 城市组合-每个城市开头的路线必须有且仅有1条
static const int MAX_RA_MAP_HUNT_SERVER_REWARD_COUNT = 10;					// 全服奖励个数

struct RAMapHuntRouteInfo
{
	RAMapHuntRouteInfo()
	{
		this->Reset();
	}

	void Reset()
	{
		route_index = 0;
		route_active_flag = 0;
		reserve_sh = 0;
		memset(city_list, 0, sizeof(city_list));
		city_fetch_flag = 0;
	}

	char route_index;													//当前路线index
	char route_active_flag;												//当前路线激活标记
	short reserve_sh;
	UNSTD_STATIC_CHECK(3 == MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE);
	char city_list[MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE];				//刷出来的城市列表
	char city_fetch_flag;
};

////////////////////////////////////////////////// 随机活动-灯塔寻宝 ////////////////////////////////////////////////
enum LIGHT_TOWER_EXPLORE_REWAED_TYPE
{
	LIGHT_TOWER_EXPLORE_REWAED_TYPE_INVALID = -1,

	LIGHT_TOWER_EXPLORE_REWAED_TYPE_ITEM = 0,				//奖励物品
	LIGHT_TOWER_EXPLORE_REWAED_TYPE_GOTO_LAYER = 1,			//跳转

	LIGHT_TOWER_EXPLORE_REWAED_TYPE_MAX
};

static const int RA_LIGHT_TOWER_EXPLORE_MAX_LAYER = 5;						// 灯塔寻宝最大层数'
static const int RA_LIGHT_TOWER_EXPLORE_MAX_PER_LAYER_REWAED_COUNT = 6;		// 灯塔寻宝每层奖励数
static const int RA_LIGHT_TOWER_EXPLORE_MAX_SERVER_REWARD_COUNT = 10;		// 全服奖励最大个数
static const int MAX_LIGHT_TOWER_EXPLORE_REWARD_COUNT = 10;

struct LightTowerExploreGridInfo
{
	LightTowerExploreGridInfo() : layer(0), index(0)
	{

	}

	char layer;												//层数
	char index;												//索引
};

////////////////////////////////////////////////// 随机活动-幻装商店 ////////////////////////////////////////////////
static const int RA_MAGIC_SHOP_ITEM_MAX_INDEX = 3;

////////////////////////////////////////////////// 随机活动-你充我送 ////////////////////////////////////////////////
static const int RA_CHONGZHI_GIFT_ITEM_MAX_INDEX = 3;

////////////////////////////////////////////////// 钓鱼 //////////////////////////////////////////////////////////////
static const int RA_FISHING_FISH_TYPE_COUNT = 6;						// 鱼的最大种类
static const int RA_FISHING_RANK_MAX_COUNT = 30;

////////////////////////////////////////////////// 随机活动-趣味植树 ////////////////////////////////////////////////
static const int RA_TREE_PLANTING_RANK_MAX_COUNT = 10;					// 排行榜玩家最大数

struct RAPlantingTreeRankInfo
{
	RAPlantingTreeRankInfo() { this->Reset(); }

	void Reset()
	{
		uid = 0;
		memset(name, 0, sizeof(name));
		opera_items = 0;
		prof = 0;
		sex = 0;
		reserve_sh = 0;
	}

	int uid;
	GameName name;
	int opera_items;
	char prof;
	char sex;
	short reserve_sh;
};

//////////////////////////////////////////////////// 达拉然水晶 ///////////////////////////////////////////////////////////
struct RandActivityStealNoteCfg
{
	RandActivityStealNoteCfg() { this->Reset(); }

	void Reset()
	{
		uid = 0;
		be_steal_times = 0;
		reserve_sh = 0;
	}

	int		uid;
	short	be_steal_times;
	short   reserve_sh;
};

static const int RA_CRYSTAL_DALARAN_RANK_REWARD_COUNT = 10;							// 排行奖励最大数
static const int RA_CRYSTA_DALARAN_STEAL_USER_MAX_PER_DAY = 30;					// 最多偷取人数

////////////////////////////////////////////////// 随机活动-周末狂欢活动 ////////////////////////////////////////////////
static const int RA_LOTTERY_1_HISTORY_MAX_COUNT = 10;					// 抽奖历史纪录最大数量
static const int RA_LOTTERY_1_ONECHOU_COUNT = 1;						// 一抽次数
static const int RA_LOTTERY_1_TEN_CHOU_COUNT = 10;						// 十抽次数

enum RA_LOTTERY_1_TYPE
{
	RA_LOTTERY_1_TYPE_INVALID = 0,
	RA_LOTTERY_1_TYPE_ONE,		// 一抽
	RA_LOTTERY_1_TYPE_TEN,		// 十抽

	RA_LOTTERY_1_TYPE_MAX,
};

struct RALottery1ChouItem
{
	RALottery1ChouItem() { this->Reset(); }

	void Reset()
	{
		reward_item = 0;
		reward_num = 0;
		is_bind = 0;
		reserve3 = 0;

		memset(xianpin_type_list, 0, sizeof(xianpin_type_list));
	}

	ItemID reward_item;			// 玩家抽取到的奖励id
	short reward_num;			// 数量

	char is_bind;				// 是否绑定
	char reserve3;
	char xianpin_type_list[MAX_CHEST_XIANPIN_NUM];
};

/////////////////////////////////////	狂嗨庆典		/////////////////////////////////////////////////////////////////////////

static const int RA_KUANG_HAI_TASK_ITEM_MAX_NUM = 32;		// 最大任务item数量
static const int RA_KUANG_HAI_REWARD_LIST_MAX_NUM = 16;		// 最大的奖励列表数
static const int RA_KUANG_HAI_REWARD_MAX_ITEM_NUM = 6;		// 奖励列表最大物品数

enum RA_KUANG_HAI_TASK_TYPE
{
	RA_KUANG_HAI_TASK_TYPE_BEGIN,
	RA_KUANG_HAI_TASK_TYPE_RECHARGE = 1,				// 充值	（参数：充值元宝金额）
	RA_KUANG_HAI_TASK_TYPE_EXP_FB = 2,					// 完成经验副本	（参数：参与次数）
	RA_KUANG_HAI_TASK_TYPE_PHASE_FB = 3,				// 完成进阶副本
	RA_KUANG_HAI_TASK_TYPE_PROTECT_FB = 4,				// 通关守护副本（单人塔防）
	RA_KUANG_HAI_TASK_TYPE_CHALLENGE_FB = 5,			// 通关挑战副本
	RA_KUANG_HAI_TASK_TYPE_ARENA = 6,					// 挑战竞技场	（参数：参与次数）
	RA_KUANG_HAI_TASK_TYPE_XUN_BAO = 7,					// 寻宝
	RA_KUANG_HAI_TASK_TYPE_KILL_BOSS = 8,				// 击杀boss（精英，世界，VIP，活跃，斗气）
	RA_KUANG_HAI_TASK_TYPE_BB_BOSS = 9,					// 击杀宝宝boss
	RA_KUANG_HAI_TASK_TYPE_HU_SONG = 10,				// 仙女护送
	RA_KUANG_HAI_TASK_TYPE_EXP_TASK = 11,				// 完成经验任务（日常）
	RA_KUANG_HAI_TASK_TYPE_WL_TANGXIAN = 12,			// 参加王陵探险
	RA_KUANG_HAI_TASK_TYPE_TIAN_JIANG_CAI_BAO = 13,		// 参加天降财宝
//	RA_KUANG_HAI_TASK_TYPE_LING_SHI_HU_SONG = 14,		// 灵石护送
	RA_KUANG_HAI_TASK_TYPE_GONG_CHENG_ZHAN = 15,		// 攻城战
	RA_KUANG_HAI_TASK_TYPE_YI_ZHAN_FENG_SHEN = 16,		// 参加一战封神
//	RA_KUANG_HAI_TASK_TYPE_XIAN_MO_ZHAN_CHANG = 17,		// 参加仙魔战场
	RA_KUANG_HAI_TASK_TYPE_XIAN_MENG_ZHENG_BA = 18,		// 参加仙盟争霸
	RA_KUANG_HAI_TASK_TYPE_SHUI_JING_HUAN_JING = 19,	// 参加水晶幻境
//	RA_KUANG_HAI_TASK_TYPE_CROSS_MINING = 20,			// 参加跨服挖矿
	RA_KUANG_HAI_TASK_TYPE_CROSS_FISHING = 21,			// 跨服钓鱼
	RA_KUANG_HAI_TASK_TYPE_DIAN_FENG_JING_JI = 22,		// 参加巅峰竞技（参数：次数，打1场算1次）
//	RA_KUANG_HAI_TASK_TYPE_TIAN_SHENG_ZHONG = 23,		// 参加天神冢
	RA_KUANG_HAI_TASK_TYPE_CROSS_XIU_LUO_TA = 24,		// 跨服修罗塔
//	RA_KUANG_HAI_TASK_TYPE_6_JIE_DUO_QI = 25,			// 参加六界夺旗
	RA_KUANG_HAI_TASK_TYPE_CROSS_6_JIE = 26,			// 参加跨服六界
	RA_KUANG_HAI_TASK_TYPE_GUILD = 27,					// 完成仙盟任务
	RA_KUANG_HAI_TASK_TYPE_GUA_JI = 28,					// 完成挂机任务（跑环）
	RA_KUANG_HAI_TASK_TYPE_BANG_BEN_CHOU_JIANG = 29,	// 参加欢乐抽奖2
//	RA_KUANG_HAI_TASK_TYPE_EXPENSE_NICE_GIFT = 30,		// 消费好礼
	RA_KUANG_HAI_TASK_TYPE_LOGIN = 31,					// 登陆
	RA_KUANG_HAI_TASK_TYPE_TARGET_RECHARGE = 32,		// 充值目标金额（参数：目标元宝金额）

	RA_KUANG_HAI_TASK_TYPE_MAX_NUM,
};

/////////////////////////////////////	礼物收割 - 场景活动		//////////////////////////////////////////
static const int GIFT_HARVEST_OPEN_TIME_MAX_COUNT = 36;						// 轮次开启时间	最大数量
static const int GIFT_HARVEST_MONSTER_CFG_MAX_COUNT = 6;					// 怪物配置		最大数量
static const int GIFT_HARVEST_FENCE_CFG_MAX_COUNT = 6;						// 围栏区配置	最大数量
static const int GIFT_HARVEST_SKILL_CFG_MAX_COUNT = 3;						// 技能配置		最大数量
static const int GIFT_HARVEST_RELIVE_POS_MAX_COUNT = 100;					// 怪物复活点	最大数量
static const int GIFT_HARVEST_PERSON_RANK_MAX = 10;							// 排行榜人数	最大数量

////////////////////////////////////////////////   跨服牧场  /////////////////////////////////////
static const int CROSS_PASTURE_RANK_COUNT = 10;                //跨服牧场排行最大人数
static const int CROSS_PASTURE_TOP_RANK_COUNT_MAX = 3;		   // 跨服牧场排行个数

//////////////////////////////////////////////// 跨服挖矿、跨服钓鱼 //////////////////////////////////////////
static const int CROSS_MINING_TOP_RANK_COUNT_MAX = 3;				// 排行个数

//////////////////////////////////////////////////// 跨服钓鱼 //////////////////////////////
static const int FISHING_FISH_BAIT_CFG_MAX_COUNT = 3;			// 鱼饵配置最大数
static const int FISHING_FISH_LEAST_COUNT_CFG_MAX_COUNT = 10;	// 获取双倍积分最少需要钓鱼数目的配置的最大数
static const int FISHING_TREASURE_CFG_MAX_COUNT = 20;			// 宝箱配置最大数
static const int FISHING_RAND_ROLE_NUM = 4;						// 钓鱼随机展示角色数量
static const int FISHING_NEWS_MAX = 10;							// 钓鱼日志数量
static const int FISHING_STEAL_COUNT_BUY_MAX = 10;				// 钓鱼偷鱼次数购买最大数
static const int FISHING_SCORE_MAX_RANK = 10;				    // 钓鱼积分排行榜最大数量
static const int CROSS_FISHING_TOP_RANK_COUNT_MAX = 3;			// 钓鱼排行个数

enum FISHING_GEAR
{
	FISHING_GEAR_NET = 0,				// 渔网
	FISHING_GEAR_SPEAR,					// 鱼叉
	FISHING_GEAR_OIL,					// 香油

	FISHING_GEAR_COUNT
};
static const int FISHING_GEAR_MAX_COUNT = 3;
UNSTD_STATIC_CHECK(FISHING_GEAR_MAX_COUNT == FISHING_GEAR_COUNT);

enum FISHING_FISH_TYPE
{
	FISHING_FISH_TYPE_NOFISH = 0,		// 没鱼（没钓上）
	FISHING_FISH_TYPE_QIUDAOYU,			// 秋刀鱼
	FISHING_FISH_TYPE_HAIXIA,			// 海虾
	FISHING_FISH_TYPE_DAHUANGYU,		// 大黄鱼
	FISHING_FISH_TYPE_JINQIANGYU,		// 金枪鱼
	FISHING_FISH_TYPE_MAOXIE,			// 毛蟹
	FISHING_FISH_TYPE_HAIGUI,			// 海龟
	FISHING_FISH_TYPE_LANXUELONGXIA,	// 蓝血龙虾

	FISHING_FISH_TYPE_COUNT,
};

static const int FISHING_FISH_TYPE_MAX_COUNT = 8;
UNSTD_STATIC_CHECK(FISHING_FISH_TYPE_MAX_COUNT == FISHING_FISH_TYPE_COUNT);

enum FISHING_NEWS_TYPE
{
	FISHING_NEWS_TYPE_INVALID = 0,
	FISHING_NEWS_TYPE_STEAL = 1,		// 偷鱼日志
	FISHING_NEWS_TYPE_BE_STEAL = 2,		// 被偷日志

	FISHING_NEWS_TYPE_MAX
};

struct FishingRandUserInfo
{
	FishingRandUserInfo() : uid(0), prof(0), least_count_cfg_index(0)
	{
		memset(user_name, 0, sizeof(user_name));
		memset(fish_num_list, 0, sizeof(fish_num_list));
	}

	long long uid;
	GameName user_name;
	short prof;
	short least_count_cfg_index;
	int fish_num_list[FISHING_FISH_TYPE_MAX_COUNT];
};

struct FishingNews
{
	FishingNews() : news_type(0), fish_type(0), fish_num(0), reserve_sh(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	short news_type;
	short fish_type;
	short fish_num;
	short reserve_sh;
	GameName user_name;
};

/////////////////////////////////////////////////  夜战王城 /////////////////////////////
const static int NIGHT_FIGHT_RANK_TOP_MAX = 3;							//夜战王城结束奖励人数
const static int CROSS_NIGHT_FIGHT_MAX_SCENE_NUM = 10;					// 夜战王城最大房间个数

////////////////////////	公会争霸奖励类型   //////////////////////////////////////////
enum OGA_GUILD_BATTLE_OPERA_TYPE
{
	OGA_GUILD_BATTLE_OPERA_TYPE_INFO = 0,						// 获取信息
	OGA_GUILD_BATTLE_OPERA_TYPE_FETCH_REWARD = 1				// 领取奖励
};

enum OGA_GUILD_BATTLE_REWARD_TYPE
{
	OGA_GUILD_BATTLE_REWARD_TYPE_WINNER_BANGZHU = 1,			// 主宰盟主可领取
	OGA_GUILD_BATTLE_REWARD_TYPE_WINNER_OWENER = 2,				// 主宰帮派成员可领取
	OGA_GUILD_BATTLE_REWARD_TYPE_LOSER_BANGZHU = 3,				// 参与活动帮派帮主可领取
	OGA_GUILD_BATTLE_REWARD_TYPE_LOSER_OTHER = 4,				// 其他参与玩家可领取

	OGA_GUILD_BATTLE_REWARD_TYPE_MAX
};

const static int LUCKY_WISH_MAX_ITEM_COUNT = 30;				//幸运许愿最大物品数量

const static int TODAY_THEME_REWARD_MAX_COUNT = 128;
const static int TODAY_THEME_REWARD_MAX_BYTE = TODAY_THEME_REWARD_MAX_COUNT / BIT_COUNT_OF_BYTE;
UNSTD_STATIC_CHECK(TODAY_THEME_REWARD_MAX_COUNT == 128);

////////////////////////	零元购   //////////////////////////////////////////
static const int RAND_ACTICITY_ZERO_BUY_RETURN_MAX_TYPE = 16;	// 零元购最大类型
static const int RAND_ACTICITY_ZERO_BUY_RETURN_MAX_DAY = 20;	// 零元购最大配置天数

///////////////////////// 天书寻主  /////////////////////////////////////////////
static const int TIANSHU_XUNZHU_MAX_TYPE_COUNT = 8;			// 天书寻主最大类型数
static const int TIANSHU_XUNZHU_SIGNLE_MAX_NUM = 32;		// 天书寻主 单个天书最多类型
static const int TIANSHU_XUNZHU_TUMO_MONSTER_MAX_NUM = 3;	// 天书寻主 屠魔天书一个类型最多的怪物种类
static const int TIANSHU_XUNZHU_TUMO_MAX_BOSS_COUNT = 100;	// 天书寻主 屠魔天书最多BOSS种

/////////////////////////////////////	幸运云购		/////////////////////////
static const int  LUCKY_BUY_RECORD_MAX_NUM = 12;			// 购买记录的最大数量
static const int  LUCKY_BUY_ROLE_RECORD_MAX_NUM = 10;		// 玩家购买数量的最大记录

struct RandActivityLuckyCloudBuyParam
{
	RandActivityLuckyCloudBuyParam() { this->Reset(); }

	void Reset()
	{
		round_index = 0;
		round_buy_num = 0;
		round_sale_num = 0;
		round_end_tick = 0;
		next_round_begin_tick = 0;
		memset(role_buy, 0, sizeof(role_buy));
		memset(buy_log, 0, sizeof(buy_log));
	}

	struct BuyLogItem
	{
		int role_id = 0;
		GameName name = { 0 };
		unsigned int timestamp = 0;
	};

	struct RoleBuy
	{
		int role_id = 0;
		int buy_num = 0;
	};

	int round_index;
	int round_buy_num;
	int round_sale_num;
	unsigned int round_end_tick;
	unsigned int next_round_begin_tick;
	RoleBuy role_buy[LUCKY_BUY_ROLE_RECORD_MAX_NUM];
	BuyLogItem buy_log[LUCKY_BUY_RECORD_MAX_NUM];
};

/////////////////////////////////////  狂欢大乐购  /////////////////////////////////////
static const int RA_MAX_CRACY_BUY_NUM_LIMIT = 21;	// 物品种类数量

#endif // __ACTIVITY_DEF_HPP__
