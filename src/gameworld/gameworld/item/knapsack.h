#ifndef __KNAPSACK_H__
#define __KNAPSACK_H__

#include "item/itembase.h"
#include "item/itemgriddata.h"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"
#include "money.h"
#include "itemcolddownmanager.hpp"
#include "servercommon/maildef.hpp"
#include "itemextern.hpp"
#include "gameworld/gameworld/protocal/msgitem.h"

#include <map>
#include <vector>

class Role;


enum PUT_REASON_TYPE
{
	PUT_REASON_INVALID = 0,								// 无效
	PUT_REASON_NO_NOTICE = 1,							// 不通知
	PUT_REASON_GM = 2,									// GM命令
	PUT_REASON_PICK = 3,								// 捡取掉落				
	PUT_REASON_GIFT = 4,								// 礼包打开
	PUT_REASON_COMPOSE = 5,								// 合成产生
	PUT_REASON_TASK_REWARD = 6,							// 任务奖励
	PUT_REASON_MAIL_RECEIVE = 7,						// 邮件
	PUT_REASON_CHEST_SHOP = 8,							// 宝箱
	PUT_REASON_RANDOM_CAMP = 9,							// 听天由命礼包
	PUT_REASON_SHOP_BUY = 10,							// 商城购买
	PUT_REASON_WELFARE = 11,							// 福利
	PUT_REASON_ACTIVE_DEGREE = 12,						// 活跃度
	PUT_REASON_CONVERT_SHOP = 13,						// 兑换商店
	PUT_REASON_ZHUXIE_ACTIVITY_REWARD = 14,				// 诛邪战场奖励
	PUT_REASON_FB_TOWERDEFEND_TEAM = 15,				// 多人塔防副本
	PUT_REASON_SEVEN_DAY_LOGIN_REWARD = 16,				// 七天登录活动奖励
	PUT_REASON_YAOJIANG = 17,							// 摇奖
	PUT_REASON_ACTIVITY_FIND = 18,						// 活动找回
	PUT_REASON_NEQ_STAR_REWARD = 19,					// 新装备本星星奖励
	PUT_REASON_NEQ_AUTO = 20,							// 新装备本扫荡
	PUT_REASON_STORY_ROLL = 21,							// 剧情本翻牌
	PUT_REASON_MAZE = 22,								// 迷宫寻宝
	PUT_REASON_STORY_FB = 23,							// 剧情副本
	PUT_REASON_CHALLENGE_FB = 24,						// 挑战副本
	PUT_REASON_VIP_LEVEL_REWARD = 25,					// VIP等级奖励
	PUT_REASON_GUILD_TASK_REWARD = 26,					// 仙盟任务奖励
	PUT_REASON_CHONGZHI_REWARD = 27,					// 充值奖励
	PUT_REASON_OPENGAME_ACTIVITY = 28,					// 开服活动
	PUT_REASON_DISCOUNT_BUY = 29,						// 一折抢购
	PUT_REASON_PHASE_AUTO = 30,							// 阶段本扫荡奖励
	PUT_REASON_TURNTABLE = 31,							// 转盘奖励
	PUT_REASON_WABAO = 32,								// 挖宝奖励
	PUT_REASON_DAILY_TASK_REWARD = 33,					// 日常任务奖励
	PUT_REASON_CISHA_TASK_REWARD = 34,					// 刺杀任务奖励
	PUT_REASON_JILIAN = 35,								// 祭炼奖励
	PUT_REASON_GUILD_TASK_ALL_COMPLETE = 36,			// 仙盟任务完成奖励
	PUT_REASON_P2PTRADE = 37,							// P2P交易
	PUT_REASON_CHALLENGE_FIELD = 38,					// 1v1竞技场
	PUT_REASON_LUCKYROLL = 39,							// 幸运转盘
	PUT_REASON_LUCKYROLL_EXTRAL = 40,					// 幸运转盘额外奖励
	PUT_REASON_CLOSE_BETA_ACTIVITY = 41,				// 封测活动
	PUT_REASON_UPDATE_NOTICE = 42,						// 更新公告奖励
	PUT_REASON_FETCH_ROLE_UPLEVEL_REWARD = 43,			// 领取升级奖励
	PUT_REASON_FETCH_SIGN_IN_REWARD = 44,				// 领取登录奖励
	PUT_REASON_SIGN_IN_FIND_BACK = 45,					// 领取找回登录的奖励
	PUT_REASON_GET_DAILY_FIND = 46,						// 日常找回获得物品
	PUT_REASON_FISH_POOL_HARVEST = 47,					// 鱼池收获
	PUT_REASON_FISH_POOL_STEAL = 48,					// 鱼池偷取
	PUT_REASON_ZHANSHENDIAN_DAY_REWARD = 49,			// 战神殿每日奖励
	PUT_REASON_ZHANSHENDIAN_PASS_REWARD = 50,			// 战神殿通关奖励
	PUT_REASON_WORLD_EVENT_REWARD = 51,					// 世界事件奖励
	PUT_REASON_RA_SERVER_PANIC_BUY = 52,				// 随机活动全民疯抢
	PUT_REASON_RA_PERSONAL_PANIC_BUY = 53,				// 随机活动个人疯抢
	PUT_REASON_RA_CHESTSHOP_REWARD = 54,				// 随机活动奇珍异宝活动奖励
	PUT_REASON_RA_STONE_UPLEVEL_REWARD = 55,			// 随机活动宝石升级活动奖励
	PUT_REASON_RA_DAY_CHONGZHI_FANLI_REWARD = 56,		// 随机活动每日充值返利奖励
	PUT_REASON_RA_DAY_CONSUME_GOLD_REWARD = 57,			// 随机活动每日消费奖励
	PUT_REASON_RA_XN_CHANMIAN_UPLEVEL_REWARD = 58,		// 随机活动仙女缠绵升级活动奖励
	PUT_REASON_RA_MOUNT_UPGRADE_REWARD = 59,			// 随机活动坐骑进阶奖励
	PUT_REASON_RA_QIBING_UPGRADE_REWARD = 60,			// 随机活动骑兵进阶奖励
	PUT_REASON_RA_MENTALITY_TOTAL_LEVEL_REWARD = 61,	// 随机活动根骨全身等级奖励
	PUT_REASON_RA_WING_UPGRADE_REWARD = 62,				// 随机活动羽翼进阶奖励
	PUT_REASON_RA_QUANMIN_QIFU_REWARD = 63,				// 随机活动全民祈福奖励
	PUT_REASON_ROLE_GOAL_REWARD = 64,					// 角色目标奖励
	PUT_REASON_RA_TOTAL_CONSUME_GOLD_REWARD = 65,		// 随机活动累计消费奖励
	PUT_REASON_RA_DAY_ACTIVE_DEGREE_REWARD = 66, 		// 随机活动活跃奖励奖励
	PUT_REASON_RA_KILL_BOSS_REWARD = 67, 				// 随机活动击杀boss奖励
	PUT_REASON_RA_SHOUYOU_YUXIANG_REWARD = 68, 			// 随机活动手有余香奖励
	PUT_REASON_RA_LOGIN_GIFT_REWARD = 69,				// 随机活动登录送礼
	PUT_REASON_LINGYUSHUISHOU = 70,						// 领域税收
	PUT_REASON_USE_MOUNT_JINJIE_CARD = 71,				// 使用坐骑阶数卡
	PUT_REASON_WUXINGGUAJI_GETREWARD = 72,				// 五行挂机获取奖励
	PUT_REASON_USE_QIBING_JINJIE_CARD = 73,				// 使用坐骑骑兵卡
	PUT_REASON_ZAI_CHONGZHI_REWARD = 74,				// 再充值奖励
	PUT_REASON_RA_BIPIN_CAPABILITY = 75,				// 比拼战力
	PUT_REASON_USE_WING_BLESS_CARD = 76,				// 使用羽翼进阶卡片
	PUT_REASON_RA_DANBI_CHONGZHI_REWARD = 77,			// 单笔充值奖励
	PUT_REASON_CSA_RANK_QIANGGOU = 78,					// 合服抢购排行
	PUT_REASON_CSA_ROLL = 79,							// 合服转盘
	PUT_REASON_CSA_KILL_BOSS_REWARD = 80,				// 合服击杀boss奖励
	PUT_REASON_CSA_LOGIN_GIFT = 81,						// 合服登录送礼
	PUT_REASON_CSA_PERSONAL_PANIC_BUY = 82,				// 合服随机活动个人疯抢
	PUT_REASON_CSA_SERVER_PANIC_BUY = 83,				// 合服活动全民疯抢
	PUT_REASON_RA_TOTAL_CHARGE_DAY = 84,				// 随机活动-累计充值（天数）奖励
	PUT_REASON_WUSHAUNG_FB_DAY_REWARD = 85,				// 无双副本，每日奖励
	PUT_REASON_WUSHAUNG_FB_PASS_REWARD = 86,			// 无双副本，通关奖励
	PUT_REASON_RA_TOMORROW_REWARD = 87,					// 随机活动-次日福利奖励
	PUT_REASON_RA_TOTAL_CHARGE = 88,					// 随机活动-累计充值
	PUT_REASON_WUSHUANG_FUMO_TAKEOFF = 89,				// 无双装备脱下附魔
	PUT_REASON_PATAFB_PASS_REWARD = 90,					// 爬塔副本通关奖励
	PUT_REASON_PATAFB_GLOBAL_FIRSTPASS_REWARD = 91,		// 爬塔副本全服首次通关奖励
	PUT_REASON_TOWERDEFENDTEAM_PASS_REWARD = 92,		// 组队塔防副本通关奖励
	PUT_REASON_YAOSHOUJITAN_PASS_REWARD = 93,			// 妖兽祭坛副本通关奖励
	PUT_REASON_MIGONGXIANFU_REWARD = 94,				// 迷宫仙府副本通关奖励
	PUT_REASON_ZHUXIE_KILL_MONSTER = 95,				// 诛邪击杀怪物获得
	PUT_REASON_ZHUXIE_GATHER = 96,						// 诛邪采集获得
	PUT_REASON_EXP_BOTTLE = 97,							// 凝聚经验
	PUT_REASON_GCZ_DAILY_REWARD = 98,					// 攻城战每日奖励
	PUT_REASON_LIFE_SKILL_MAKE= 99,						// 生活技能制造
	PUT_REASON_PAOHUAN_ROLL = 100,						// 跑环任务翻牌
	PUT_REASON_PUTOFF_GUILDSOTRGE = 101,				// 仙盟仓库
	PUT_REASON_SHENZHUANG_KILLROLE = 102,				// 神装击杀玩家
	PUT_REASON_RA_CONVERT_SHOP = 103,					// 随机活动积分兑换
	PUT_REASON_MYSTERIOUSSHOP = 104,					// 神秘商城购买
	PUT_REASON_RA_LEVEL_LOTTERY = 105,					// 随机活动金银塔
	PUT_REASON_RA_CHONGZHI_NIUEGG_REWARD = 106,			// 随机活动充值扭蛋 
	PUT_REASON_DIFF_WEEKDAY_CHONGZHI_REWARD = 107,		// 每日累充
	PUT_REASON_RA_ZHENBAOGE_REWARD = 108,				// 随机活动-珍宝阁活动奖励
	PUT_REASON_SHENZHOU_WEAPON_TAKE_OUT = 109,			// 神州六器-取出物品
	PUT_REASON_SHENZHOU_WEAPON_GATHER = 110,			// 神州六器-采集
	PUT_REASON_RA_MONEY_TREE_REWARD = 111,				// 随机活动--摇钱树
	PUT_REASON_PET_RECYCLE = 112,						// 宠物蛋回收	
	PUT_REASON_PET_REWARD = 113,						// 宠物奖励	
	PUT_REASON_MINE_FETCH_SERVER_REWARD = 114,			// 趣味挖矿全服奖励
	PUT_REASON_MINE_EXCANGE = 115,						// 趣味挖矿-兑换锦囊
	PUT_REASON_FANFAN_EXCHANGE_WORD_REWARD = 116,		// 翻翻转-兑换
	PUT_REASON_CONTINUE_CHONGZHI_FETCH_REWARD = 117,	// 连续充值奖励
	PUT_REASON_CONTINUE_CHONGZHI_FETCH_EXTRA_REWARD = 118,	// 连续充值额外奖励
	PUT_REASON_CONTINUE_CONSUME_FETCH_REWARD = 119,		// 连续消费奖励
	PUT_REASON_CONTINUE_CONSUME_FETCH_EXTRA_REWARD = 120,	// 连续消费额外奖励
	PUT_REASON_ARMY_DAY_EXCHANGE_REWARD = 121,			// 军歌嘹亮兑换
	PUT_REASON_ARMY_DAY_WIN_AYMY_SIDE_REWARD = 122,		// 军歌嘹亮--胜方奖励
	PUT_REASON_PASTURE_SPIRIT_LUCKY_DRAW_REWARD = 123,	// 牧场精灵抽奖奖励
	PUT_REASON_CIRCULATION_CHONGZHI_FETCH_REWARD = 124,	// 连续充值
	PUT_REASON_EXTREME_LUCKY_DRAW_REWARD = 125,			// 至尊幸运抽奖奖励
	PUT_REASON_RA_MONEY_TREE2_REWARD = 126,				// 摇钱树2
	PUT_REASON_WUSHANG_EQUIP_TAKE_OUT = 127,			// 无上神兵取出
	PUT_REASON_WUSHANG_EQUIP_EXCHANGE = 128,			// 无上神兵兑换
	PUT_REASON_RA_NATIONAL_DAY_EXCHANGE = 129,			// 随机活动--欢度国庆
	PUT_REASON_RA_ZHENBAOGE2_REWARD = 130,				// 珍宝阁2
	PUT_REASON_MITAMA_TASK_REWARD = 131,				// 御魂任务奖励
	PUT_REASON_MITAMA_EXCHANGE_ITEM = 132,				// 温泉积分兑换
	PUT_REASON_FAIRY_TREE_FETCH_GIFT = 133,				// 仙树奖励
	PUT_REASON_FAIRY_TREE_DRAW_REWARD = 134,			// 仙树抽奖
	PUT_REASON_MAGIC_EQUIPMENT_TAKEOFF_STONE = 135,		// 魔器取下宝石
	PUT_REASON_TREASURES_MALL_BUY = 136,				// 珍宝商城购买
	PUT_REASON_TREASURES_MALL_EXCHANGE = 137,			// 珍宝商城兑换
	PUT_REASON_EXP_FB_FIRST_REWARD = 138,				// 经验本首通奖励
	PUT_REASON_ONLINE_REWARD = 139,						// 领取在线奖励
	PUT_REASON_HAPPY_TREE_REWARD = 140,					// 领取欢乐果树奖励
	PUT_REASON_EXCHANG_REWARD = 141,					// 福利兑换物品
	PUT_REASON_CROSS_1V1 = 142,							// 跨服1v1
	PUT_REASON_XIULUOTOWER = 143,						// 跨服修罗塔
	PUT_REASON_QINGYUAN_BLESS_REWARD = 144,				// 情缘祝福奖励
	PUT_REASON_HUNYAN_GATHER = 145,						// 婚宴采集
	PUT_REASON_HUNYAN_ZHUFU = 146,						// 婚宴祝福
	PUT_REASON_QINGYUAN_FB_REWARD = 147,				// 情缘副本奖励
	PUT_REASON_QUESTION_ACTIVY = 148,					// 答题活动-新版
	PUT_REASON_TIANJIANGCAIBAO_REWARD = 149,			// 天降财宝奖励
	PUT_REASON_MOVE_CHESS = 150,						// 走棋子
	PUT_REASON_MIllIONAIRE = 151,						// 大富豪
	PUT_REASON_WANGLINGEXPLORE_ACTIVITY_REWARD = 152,	// 王陵探险奖励
	PUT_REASON_TOUZIHIHUA = 153,						// 投资计划奖励
	PUT_REASON_NEW_CORNUCOPIA = 154,					// 新聚宝盆奖励
	PUT_REASON_CROSS_TEAM = 155,						// 跨服组队
	PUT_REASON_GUILD_REWARD = 156,						// 公会奖励
	PUT_REASON_TERRITORY_WAR = 157,						// 领土战
	PUT_REASON_VIP_WEEK_REWARD = 158,					// VIP周奖励
	PUT_REASON_ZHUANSHENG_EQUIP = 159,					// 转生装备
	PUT_REASON_LITTLE_PET_EXCHANGE_ITEM = 160,			// 小宠物兑换
	PUT_REASON_LITTLE_PET_RECYCLE = 161,				// 小宠物回收
	PUT_REASON_LITTLE_PET_CHOUJIANG_ONE = 162,			// 小宠物抽奖1连
	PUT_REASON_LITTLE_PET_CHOUJIANG_TEN = 163,			// 小宠物抽奖10连
	PUT_REASON_GUILD_TERRITORY_WELF = 164,				// 公会领地福利
	PUT_REASON_VIP_PASS_REWARD = 165,					// VIP副本通关奖励
	PUT_REASON_SHENZHOU_WEAPON_IDENTIFY = 166,			// 神州六器-鉴定
	PUT_REASON_GUILD_BONFIRE_GATHER = 167,				// 采集仙盟篝火
	PUT_REASON_GUILD_BONFIRE_ADD_MUCAI = 168,			// 仙盟篝加木材
	PUT_REASON_GUILD_BATTEL = 169,						// 公会争霸奖励
	PUT_REASON_HUSONG_TASK = 170,						// 护送任务奖励
	PUT_REASON_HOTSPRING_ACTIVITY = 171,				// 温泉活动奖励
	PUT_REASON_GCZ_ACTIVITY_ZHANGONG = 172,				// 攻城战战功奖励
	PUT_REASON_TAKEOFF_SUIT_RETURN = 173,				// 脱下套装返还材料
	PUT_REASON_MAGICAL_PRECIOUS = 174,					// 魔龙秘宝奖励
	PUT_REASON_ITEM_COLLECTION_REWARD = 175,			// 集字活动兑换
	PUT_REASON_ITEM_LOVE_CONTRACT_REWARD = 176,			// 情缘爱情契约
	PUT_REASON_MARRY_REWARD = 177,						// 情缘结婚
	PUT_REASON_FB_GUIDE = 178,							// 副本引导
	PUT_REASON_QUNXIANLUANDOU = 179,					// 元素战场
	PUT_REASON_GOLD_VIP_DAY_REWARD = 180,				// 黄金会员每日奖励
	PUT_REASON_GOLD_VIP_CONVERT_ITEM = 181,				// 黄金会员道具兑换
	PUT_REASON_EQUI_COMPOUND = 182,						// 装备合成
	PUT_REASON_RUNE_TOWER = 183,						// 符文塔
	PUT_REASON_TEAM_EQUIP_FB = 184,						// 组队装备副本
	PUT_REASON_GUILD_BOX_ASSIST = 185,					// 公会宝箱协助
	PUT_REASON_GUILD_BOX_OPEN = 186,					// 公会宝箱开启
	PUT_REASON_GCZ_LUCK_REWARD = 187,					// 攻城战幸运奖励
	PUT_REASON_LTZ_LUCK_REWARD = 188,					// 领土战幸运奖励
	PUT_REASON_GHZB_LUCK_REWARD = 189,					// 公会争霸幸运奖励
	PUT_REASON_YSZC_LUCK_REWARD = 190,					// 元素战场幸运奖励
	PUT_REASON_SZLQ_OPEN_BOX_REWARD = 191,				// 神州六器打开宝箱奖励
	PUT_REASON_SZLQ_HELP_BOX_REWARD = 192,				// 神州六器协助宝箱奖励
	PUT_REASON_ZODIAC_GGL_REWARD = 193,					// 星座滚滚乐
	PUT_REASON_ZODIAC_BEAD_UNLOAD = 194,				// 星途珠卸载
	PUT_REASON_XZYJ_HELP_BOX_REWARD = 195,				// 星座遗迹打开宝箱奖励
	PUT_REASON_XINGZUO_MIJI_COMPOUND = 196,				// 秘籍合成
	PUT_REASON_ZERO_GIFT = 197,							// 零元礼包
	PUT_REASON_ZHIXIAN_TASK_REWARD = 198,				// 支线任务奖励
	PUT_REASON_MARRY_ZHUFU_REWARD = 199,				// 结婚祝福获得物品
	PUT_REASON_MARRY_SONGHUA_REWARD = 200,				// 结婚送花获得物品
	PUT_REASON_JING_LING_SKILL = 201,					// 精灵技能
	PUT_REASON_DISCARD_EQUIP = 202,						// 分解装备
	PUT_REASON_GUILD_MAZE = 203,						// 公会迷宫
	PUT_REASON_SHENZHOU_HUNYIN_INLAY = 204,				// 神州六器-魂印镶嵌
	PUT_REASON_YUANBAO_ZHUANPAN = 205,					// 转盘奖励
	PUT_REASON_TUITU_FB_STAR_REWARD = 206,				// 推图副本星级奖励
	PUT_REASON_TUITU_FB_SAODANG = 207,					// 推图副本扫荡
	PUT_REASON_JING_LING_HOME = 208,					// 精灵家园
	PUT_REASON_CONTINUE_CHONGZHI_FETCH_REWARD_CHU = 209,// 连充特惠初奖励
	PUT_REASON_CONTINUE_CHONGZHI_FETCH_REWARD_GAO = 210,// 连充特惠高奖励
	PUT_REASON_FIGHTING_MININE_ROB = 211,				// 决斗场 - 挖矿 - 抢劫奖励
	PUT_REASON_FIGHTING_MININE_REVENGE = 212,			// 决斗场 - 挖矿 - 复仇奖励
	PUT_REASON_FIGHTING_SAILINE_ROB = 213,				// 决斗场 - 航海 - 抢劫奖励
	PUT_REASON_FIGHTING_SAILINE_REVENGE = 214,			// 决斗场 - 航海 - 复仇奖励
	PUT_REASON_JING_LING_EXPLORE = 215,					// 精灵探险
	PUT_REASON_GOLDEN_PIG_RANDOM_REWARD = 216,			// 金猪召唤随机奖励
	PUT_REASON_OPEN_SERVER_INVEST = 217,				// 开服投资奖励
	PUT_REASON_TUITU_FB_FIRST_PASS_REWARD = 218,		// 推图副本首通奖励
	PUT_REASON_ADVANCE_NOTICE_REWARD = 219,				// 功能预告奖励
	PUT_REASON_SINGLE_CHONGZHI_REWAED = 220,			// 单返豪礼
	PUT_REASON_RA_XIANYUAN_TREAS_REWARD = 221,			// 聚划算
	PUT_REASON_RA_RUSH_BUYING_BUY_ITEM = 222,			// 限时拍卖
	PUT_REASON_EXTREME_LUCKY_RETURN_REWARD = 223,		// 至尊幸运星返利奖励
	PUT_REASON_PROMOTING_POSITION_REWARD = 224,			// 步步高升返利奖励
	PUT_REASON_MAP_HUNT_EXTERN_REWARD = 225,			// 地图寻宝奖励
	PUT_REASON_MAP_HUNT_BASE_REWARD = 226,				// 地图寻宝基础奖励
	PUT_REASON_MAP_HUNT_RETURN_REWARD = 227,			// 地图寻宝返利奖励
	PUT_REASON_KING_DRAW_RETURN_REWARD = 228,			// 陛下请翻牌累抽返利
	PUT_REASON_FIGHTING_CHALLENGE = 229,				// 决斗场 - 附近的人
	PUT_REASON_JINGLING_ADVANTAGE = 230,				// 精灵奇遇
	PUT_REASON_PAOHUAN = 231,							// 跑环任务
	PUT_REASON_QING_YUAN_SHENG_DI = 232,				// 情缘圣地
	PUT_REASON_DISCARD_JINGLING = 233,					// 回收精灵
	PUT_REASON_SHENSHOU_HUANLING = 234,					// 神兽唤灵	
	PUT_REASON_CROSS_GUILDBATTLE_SCORE = 235,			// 跨服帮派战
	PUT_REASON_CROSS_GUILDBATTLE_OWN = 236,				// 跨服帮派战
	PUT_REASON_LIGHT_TOWER_EXPLORE_EXTERN_REWARD = 237,	// 灯塔寻宝
	PUT_REASON_LIGHT_TOWER_EXPLORE_SERVER_REWARD = 238, // 灯塔寻宝返利
	PUT_REASON_MIJINGXUNBAO_REWARD = 239,				// 秘境寻宝返利
	PUT_REASON_GUILD_SIGNIN = 240,						// 仙盟签到
	PUT_REASON_CSA_BOSS_GATHER = 241,					// 合服boss采集
	PUT_REASON_MAGIC_SHOP_BUY = 242,					// 幻装商店购买
	PUT_REASON_MAGIC_SHOP_FETCH = 243,					// 幻装商店领取
	PUT_REASON_SHEN_YIN = 244,							// 神印精炼
	PUT_REASON_CROSS_TUANZHAN_FETCH = 245,				// 跨服团战领取
	PUT_REASON_FISHING_EXCHANGE = 246,					// 钓鱼兑换
	PUT_REASON_ELEMENT_HEART_TRANST = 247,				// 元素虚拟物品转换
	PUT_REASON_ELEMENT_HEART_SHOP_BUY = 248,			// 元素商店购买
	PUT_REASON_ELEMENT_EQUIP_RECYCLE = 249,				// 元素装备分解
	PUT_REASON_ELEMENT_HEART_PRODUCT = 250,				// 元素产出
	PUT_REASON_UPGRADE_RUSH_RETURN = 251,				// 快速进阶返还
	PUT_REASON_FEIXIAN_UP = 252,						// 飞仙装备升级
	PUT_REASON_FABAO_UP = 253,						    // 法宝进阶返还
	PUT_REASON_SHIZHUANG_UP = 254,                      // 时装或神兵进阶
	PUT_REASON_CHALLENGE_FIELD_BEST_RANK_BREAK=255,     // 竞技场奖励
	PUT_REASON_NEQ_ROLL = 256,							// 新装备本翻牌
	PUT_REASON_SHENYI_UPGRADE_ONE_RETURN = 257,			// 神翼直升一阶返还
	PUT_REASON_CROSS_DARK_NIGHT_REWARD = 258,			// 月黑风高奖励
	PUT_REASON_SHENGONG_UPGRADE_ONE_RETURN = 259,		// 神弓直升一阶返还
	PUT_REASON_LITTLE_PET_TAKEOFF = 260,				// 小宠物卸下
	PUT_REASON_LITTLE_PET_EQUIPMENT_TAKEOFF = 261,		// 小宠物装备脱下
	PUT_REASON_YUYUE_HUNYAN_REWARD = 262,				// 婚宴预约发放奖励
	PUT_REASON_QINGYUAN_GIFT = 263,						// 购买情缘礼包
	PUT_REASON_TALENT_GIRD_PUTOFF = 264,				// 天赋格子中卸下
	PUT_REASON_ARMOT_DEFEND_AUTO = 265,					// 防具装备本扫荡
	PUT_REASON_YAOSHI_UPGRADE_ONE_RETURN = 266,			// 腰饰直升一阶返还
	PUT_REASON_TOUSHI_UPGRADE_ONE_RETURN = 267,			// 头饰直升一阶返还
	PUT_REASON_QILINBI_UPGRADE_ONE_RETURN = 268,		// 麒麟臂直升一阶返还
	PUT_REASON_MASK_UPGRADE_ONE_RETURN = 270,			// 面具直升一阶返还
	PUT_REASON_GUILD_BOSS_GATHER = 271,					// 仙盟BOSS采集
	PUT_REASON_ZHAUNGZHI_SRTONE_UNLAY= 272,				// 玉石卸下
	PUT_REASON_ZHUANZHI_SUIT_STUFF = 273,				// 返还转职套装材料
	PUT_REASON_FISHING_GIVE_BAIT = 274,					// 钓鱼活动首次进入发放鱼饵
	PUT_REASON_FISHING_GIVE_TREASURE_REWARD = 275,		// 钓鱼活动宝箱奖励
	PUT_REASON_FISHING_BUY_BAIT = 276,					// 钓鱼活动鱼饵购买
	PUT_REASON_FISH_SCORE_REWARD = 277,					// 跨服钓鱼积分奖励
	PUT_REASON_NIGHT_FIGHT = 278,						// 夜战王城
	PUT_REASON_SHANGGU_BOSS_GATHER_REWARD = 279,        // 上古boss采集物奖励
	PUT_REASON_BOSSCARD_REWARD = 280,				    // boss图鉴集齐奖励
	PUT_REASON_GUILD_QUESTION_GATHER = 281,				// 帮派答题采集
	PUT_REASON_1V1_GONGXUN_REWARD = 282,                // 跨服1v1-功勋奖励
	PUT_REASON_ZHIZUN_EQUIP_COMPOSE = 283,              // 至尊装备合成
	PUT_REASON_MESS_BATTLE = 284,						// 乱斗战场
	PUT_REASON_CROSS_PASTURE = 285,						// 跨服牧场
	PUT_REASON_CROSS_BOSS_GATHER = 286,					// 跨服boss采集获得
	PUT_REASON_3V3_GONGXUN_REWARD = 287,				// 跨服3v3-功勋奖励
	PUT_REASON_3V3_JOIN_TIMES_REWARD = 288,				// 跨服3v3-参与次数奖励
	PUT_REASON_CROSS_FB_LIEKUN_GATHER = 289,			// 跨服副本-猎鲲地带采集
	PUT_REASON_EQUIPMENT_DECOMPOSE = 290,				// 普通装备分解
	PUT_REASON_CONSUM_GIFT_REWARD = 291,                // 消费领奖
	PUT_REASON_CONSUM_GIFT_ROLL_REWARD = 292,           // 消费领奖摇奖奖励
	PUT_REASON_LIMIT_BUY_BUY_ITEM = 293,                // 每日限购购买物品
	PUT_REASON_RA_UPGRADE_REWARD = 294, 				// 随机活动全民进阶奖励
	PUT_REASON_HAPPY_CUMUL_CHONGZHI_REWARD = 295,       // 欢乐累充奖励
	PUT_REASON_JINJIE_RETURN_REWARD = 296,              // 进阶返还奖励
	PUT_REASON_SHENGQI_STUFF_DECOMPOSE = 297,           // 圣器材料分解
	PUT_REASON_RA_CONSUME_FOR_GIFT_EXCHANGE_ITEM = 298,	// 消费有礼
	PUT_RA_LIMIT_TIME_REBATE_REWARD = 299,              // 限时反馈
	PUT_REASON_TIME_LIMIT_GIFT = 300,                   // 限时礼包
	PUT_REASON_CIRCULATION_CHONGZHI = 301,              // 循环充值2
	PUT_REASON_TIME_LIMIT_LIUXURY_GIFT_BAG = 302,       // 限时豪礼
	PUT_REASON_RA_RMB_BUY_CHEST_SHOP = 303,             // 神秘商店
	PUT_REASON_RA_CONSUME_GOLD_REWARD = 304,            // 消费返礼
	PUT_REASON_ITEM_COLLECTION_SECOND_REWARD = 305,     // 集月饼活动
	PUT_REASON_VERSION_GRAND_TOTAL_CHARGE = 306,        // 版本累计充值
	PUT_REASON_HUANLE_YAOJIANG2 = 307,                  // 欢乐摇奖2
	PUT_RA_BUY_ONE_GET_ONE_FREE_REWARD = 308,			// 买一送一
	PUT_REASON_EVERYDAY_NICE_GIFT_ERWARD = 309,			// 随机活动每日好礼
	PUT_REASON_GUAGUA = 310,                            // 随机活动顶呱呱奖励
	PUT_REASON_HUNJIE_DECOMPOSE = 311,                  // 婚戒分解
	PUT_REASON_BUILD_TOWER_AUTO = 312,					// 建塔本扫荡
	PUT_REASON_HOLIDAY_GUARD_REWARD = 313,				// 节日守护奖励
	PUT_REASON_EXTREME_CHALLENGE_REWARD = 314,          // 极限挑战奖励
	PUT_REASON_DRESSING_ROOM_EXCHANGE = 315,			// 衣柜兑换
	PUT_REASON_JINJIESYS_REWARD = 316,					// 进阶系统奖励
	PUT_ROLE_BIG_SMALL_GOAL_REWARD = 317,               // 人物大小目标
	PUT_REASON_SHENQI_DECOMPOSE = 318,					// 神器分解
	PUT_REASON_SHENQI_INLAY_GIVE_BACK = 319,			// 神器镶嵌返回
	PUT_REASON_YIZHANDAODI_KILL_REWARD = 320,			// 一站到底击杀奖励
	PUT_REASON_ZHUANZHI_SOTNE_CONVERT = 321,			// 转职玉石兑换
	PUT_REASON_HUANLEYAOJIANG_REWARD = 322,             // 欢乐摇奖
	PUT_REASON_HUANLEZADAN_REWARD = 323,                // 欢乐砸蛋
	PUT_REASON_ARMORDEFEND_NEW_PLAYER_REWARD = 324,		// 防具本新手奖励
	PUT_REASON_RED_EQUIP_COLLECT = 325,                 // 神兵图录--红装收集
	PUT_REASON_JINJIE_RETURN2_REWARD = 326,             // 进阶返还2
	PUT_REASON_RA_COMBINE_BUY = 327,                    // 组合购买 
	PUT_REASON_DAY_CHONGZHI_REWARD = 328,               // 天天返利
	PUT_REASON_XIANZUNKA_FIRST_ACTIVE_REWARD = 329,     // 仙尊卡
	PUT_REASON_SPOUSE_HOME_BUY = 330,                   // 夫妻家园
	PUT_REASON_GUILD_FB_WAVE_REWARD = 331,              // 仙盟试炼波次奖励
	PUT_REASON_CROSS_GUILDBATTLE_TASK = 332,			// 跨服帮派战
	PUT_REASON_CROSS_MIZANG_BOSS_GATHER = 333,			// 跨服秘藏boss采集获得
	PUT_REASON_CROSS_YOUMING_BOSS_GATHER = 334,			// 跨服幽冥boss采集获得
	PUT_REASON_EQUIP_TEAM_LAYER_JUMP = 335,				// 组队爬塔进入后给予之前层的掉落包奖励
	PUT_REASON_SHENYIN_DECOMPOSE = 336,					// 神印分解
	PUT_REASON_CROSS_GUILDBATTLE_BOSS = 337,			// 跨服帮派战除魔
	PUT_REASON_GREATESOLDIER_FETCH_DRAW_REWARD = 338,	// 名将领取保底奖励
	PUT_REASON_GREATESOLDIER = 339,						// 名将
	PUT_REASON_EQUIP_TEAM_BUY_MULTIPLE_REWARD = 340,	// 组队爬塔购买多倍奖励
	PUT_REASON_TOWER_DEFEND_TEAM_BUY_MULTIPLE_REWARD = 341,	// 组队守护购买多倍奖励
	PUT_REASON_MOUNT_UPGRADE_ONE_RETURN = 342,          // 坐骑直升一阶返回
	PUT_REASON_WING_UPGRADE_ONE_RETURN = 343,           // 羽翼直升一阶返回 
	PUT_REASON_FIGHTMOUNT_UPGRADE_ONE_RETURN = 344,     // 战骑直升一阶返回
	PUT_REASON_FOOTPRINT_UPGRADE_ONE_RETURN = 345,      // 足迹直升一阶返回
	PUT_REASON_HALO_UPGRADE_ONE_RETURN = 346,           // 光环直升一阶返回
	PUT_REASON_SHIZHUANG_UPGRADE_ONE_RETURN = 347,		// 时装直升一阶返还
	PUT_REASON_FABAO_UPGRADE_ONE_RETURN = 348,			// 法宝直升一阶返还
	PUT_REASON_RECYCLE_LITTLEPET_RETURN_FEED_STUFF = 349,// 回收小宠物返还喂养材料
	PUT_REASON_TOWER_DEFEND_TEAM_WAVE_JUMP = 350,		// 组队守护跳波奖励
	PUT_REASON_CRA_CLOUD_PURCHASE_BUY_TICKET = 351,		// 一元云购购买抢购券
	PUT_REASON_CLOUD_PURCHASE_CONVERT_REWARD = 352,		// 一元夺宝兑换奖励
	PUT_REASON_UPGRADE_CARD_BUY = 353,					// 直升丹购买
	PUT_REASON_CONTINUE_CHONGZHI_FETCH_REWARD_2 = 354,	// 限时连充奖励
	PUT_REASON_LOTTERY_1_SERVER_REWARD = 355,			// 周末狂欢全服奖励
	PUT_REASON_GODMAGIC_BOSS_GATHER = 356,				// 神魔boss
	PUT_REASON_HUNYAN_USE_YANHUA = 357,					// 婚宴使用烟花奖励
	PUT_REASON_TOWERDEFEND_FIRST_REWARD = 358,			// 防具本新手奖励
	PUT_REASON_TODAY_THEME_REWARD = 359,				// 今日主题奖励
	PUT_REASON_CROSS_GOAL_REWARD = 360,					// 跨服目标奖励
	PUT_REASON_ZERO_GIFT_GOD_COSTUME = 361,				// 零元礼包四阶红装
	PUT_REASON_ZERO_BUY_RETURN_REWARD = 362,			// 零元购购买物品
	PUT_REASON_RA_KUANG_HAI_REWARD = 363,				// 狂嗨庆典 嗨点兑换奖励
	PUT_REASON_CROSS_BIANJINGZHIDI_ACTIVITY_REWARD = 364,// 跨服边境之地奖励
	PUT_REASON_TIANSHUXZ_REWARD = 365,					// 天书寻主奖励
	PUT_REASON_LITTLE_HELPER_BOSS = 366,				// 小助手，自动击杀boss
	PUT_REASON_ZHIZUN_EQUIP_COMPOSE2 = 367,				//转职装备合成
	PUT_REASON_DISCOUNTSHOP_BUY = 367,					// 特惠商城购买
	PUT_REASON_LUCKY_CLOUD_BUY_MIN_REWARD = 368,		// 幸运云购保底奖励
	PUT_REASON_TOWERDEFENF_TEAM_FB_EXTRA_REWARD = 369,	// 组队守护副本额外奖励
	PUT_REASON_EQUIP_TEAM_FB_EXTRA_REWARD = 370,		// 组队爬塔副本额外奖励
	PUT_REASON_CRACYBUY_REWARD = 371,					// 狂欢大乐购
	PUT_REASON_WEEKEND_EQUIP_ACCUMULATE_REWARD = 372,	// 周末装备累抽奖励
	PUT_REASON_CROSS_EQUIP_ROLL = 373,					// 斗气装备抽奖
	PUT_REASON_CROSS_EQUIP_BOSS_GATHER = 374,			// 斗气装备boss采集
	PUT_REASON_CROSS_EQUIP_LIANZHI = 375,				// 斗气装备炼制
	PUT_REASON_CROSS_HUSONG_SHUIJING = 376,				// 跨服护送水晶

	PUT_REASON_UPGRADE_ONE_RETURN = 1000,				// 进阶系统升一阶返回(1000 + upgrade_type)
};

enum DisCardMethod
{
	DISCARD_METHOD_SELL = 0,						// 出售
	DISCARD_METHOD_RECYCL = 1,						// 回收
};

enum RecyclType
{
	RECYCL_NONE = 0,								// 回收无东西
	RECYCL_COIN = 1,								// 铜钱(暂无用)
	RECYCL_NVWASHI = 2,								// 女娲石(暂无用)
	RECYCL_LINGJING = 3,							// 灵精
	RECYCL_BEAST_EXP = 4,							// 师门神兽经验(暂无用)
	RECYCL_XIUWEEI = 5,								// 修为
	RECYCL_MOJING = 6,								// 魔晶
	RECYCL_LITTLE_PET = 7,							// 宠物强化丹
	RECYCL_MARRY_LEVEL_EXP = 8,						// 结婚经验值
	RECYCL_XUANTIE = 9,								// 玄铁
	RECYCL_MIYIN = 10,								// 秘银
	RECYCL_HUNJIE_DECOMPOSE = 11,					// 婚戒分解
	RECYCL_LITTLE_PET_SCORE = 12,					// 小宠物回收积分
	RECYCL_HUNLI = 13,								// 仙宠灵魂分解
	RECYCL_CHUANSHI_FRAGMENT = 14,					// 传世碎片
};

struct KnapsackDiscardItem
{
	int index = -1;
	int discard_num = 0;
	int discard_method = 0;
	bool is_succ = false;
};

const char *ItemGridDataLog(const ItemBase *itemimp, const ItemGridData &grid_data);
const char *ItemGridQuickLog(const ItemBase *itemimp, const ItemGridData &grid_data);

class Knapsack
{
public:
	Knapsack();
	~Knapsack();

	friend class ItemExtern::ItemConsumeConfigList;

	void SetRole(Role *role);

	void Init(Role *role, int param_num, const ItemListParam::ItemListData *param_list[], int valid_knapsack_num, int valid_storage_num,
		int valid_grid_auto_add_count, Int64 gold, Int64 gold_bind, Int64 coin, Int64 coin_bind);
	void Update(time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnResetData();			

	//使用固定背包格子的物品
	bool Use(short index, short num, short equip_index = -1);
	//根据item_id, num 自动搜索物品使用(优先绑定)
	bool UseItem(ItemID item_id, short num);
	
	// 分解--------------------------------------------------------------------------------------------------------
	bool Discard(short index, short discard_num, int discard_method = 0, ItemID item_id_in_client = 0, int item_num_in_client = 0, bool is_notice_client = true);
	bool BatchDiscard(Protocol::CSBatchDiscardItem *discard_item_list);
	bool OneKeyDiscardJiling(int color,int discard_method = DISCARD_METHOD_RECYCL);

	bool CheckCanDiscard(short index, short discard_num, int discard_method = 0, ItemID item_id_in_client = 0, int item_num_in_client = 0);
	void DoneDiscard(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);				// 执行背包物品消耗
	bool DiscardSell(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleNone(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleLingjing(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleXiuwei(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleMojing(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleXuantie(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleMiyin(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleLittlePet(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleMarrylevelExp(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleHunjieDecompose(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecyclePetScore(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleHunli(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleChuanshiFragment(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);

	// ------------------------------------------------------------------------------------------------------------

	bool Move(short from_index, short to_index);
	bool Split(short index, short num);
	bool InOrder(bool is_storage, bool ignore_bind);
	bool TakeOutAllItemFromStorage();			
	
	bool Put(const ItemConfigData &item_configdata, int reason_type, int *out_index = NULL, UInt16 drop_monster_id = 0, int gold_price = 0);
	bool Put(const ItemDataWrapper &item_wrapper, int reason_type, int *out_index = NULL, UInt16 drop_monster_id = 0);
	bool PutList(short list_num, const ItemConfigData *item_configdata_list, short reason_type, int *out_index_list = NULL);		// 有净值物品不可用此接口
	bool PutList(short list_num, const ItemDataWrapper *item_wrapper_list, short reason_type, int *out_index_list = NULL);
	bool PutOrMail(const ItemConfigData &item_configdata, short reason_type);
	bool PutListOrMail(const ItemConfigData item_configdata_list[MAX_ATTACHMENT_ITEM_NUM], short reason_type);
	void PutInKnapsack(const ItemConfigData item_list[MAX_ATTACHMENT_ITEM_NUM]);
	bool PutOrSendToOtherSystem(const ItemConfigData &item_configdata, short reason_type, int *out_index = NULL);					// 放入背包，如果是特殊类型则放入相应系统虚拟背包
	
	void ChangeAllItemToOtherSex();
	void OnChangeProf();

	int  Count(ItemID item_id);
	bool CheckEmptyGridNoLessThan(int check_num);												// 检查当前背包空位是否不少于这个值
	bool HaveSpace() { return CheckEmptyGridNoLessThan(1); }
	bool HasItem(ItemID item_id);
	bool ExtendValidNum(bool can_use_gold, bool is_storage, short num, bool is_auto_extend = false); // 扩展背包/仓库格子
	void VipLevelExtendValidNum(Role * level);														 // vip等级扩展背包格子

	void ClearAll();																				 // 清空背包，仅供GM命令使用
	
	short GetCurrKnapsackValidNum() { return m_curr_knapsack_valid_num; }
	short GetCurrStorageValidNum() { return m_curr_storage_valid_num; }
	short GetValidGridIndexEnd() { return m_valid_grid_index_end; }
	int GetValidGridAutoAddCount() { return m_valid_grid_auto_add_count; }

	short GetCurrKnapscakEmptyGridNum();
	
	bool Lock() { if (m_is_lock) return false; return (m_is_lock = true); }
	void UnLock() { m_is_lock = false; }
	bool IsLock() { return m_is_lock; }

	Money * GetMoney() { return &m_money; }
	ItemColddownManager * GetItemColddownManager() { return &m_item_codldown_manager; }

	bool ConsumeItem(ItemID item_id, const int num, const char *reason, bool need_audit_log = true);
	bool ConsumeItemByIndex(int knapsack_index, int num, const char *reason, bool is_real_consume = true, bool need_audit_log = true);
	bool ConsumeItemByIndexList(int list_num, const short *knapsack_index_list, const short *consume_num_list, const char *reason, bool is_real_consume = true, bool need_audit_log = true);		// 允许index重复

	void SendKnapsackInfoToRole();
	void SendKnapsackGridExtendAuto();							// 在线时长自动拓展格子信息
	
	bool GetItemGridData(int index, ItemGridData *item_data);
	ItemGridData *GetItemGridData(short index);
	void SetItemDirty(int index, bool to_notice_client = true, short protocol_change_type = ITEM_CHANGE_TYPE_CHANGE, short reason_type = PUT_REASON_INVALID);			
	void RefreshValidGridIndexEnd(int change_index);											// 修正非空格子最终序号
	void GridChangeEquipId(int index, ItemID new_equip_id, bool need_set_dirty);
	void GridSetBind(int index, bool need_set_dirty);
	void GridClearInvalidTime(int index, bool need_set_dirty);

	void GetItemList(int *count, ItemListParam::ItemListData *save_list);
	void GetChangeItemList(int *count, ItemListParam::ItemListData *save_list);
	void ClearDirtyMark();

	bool GetListOfItem(ItemID item_id, short max_num,
					   short *bind_index_list, short *bind_num_list, short *bind_count,
					   short *nobind_index_list, short *nobind_num_list, short *nobind_count);
	
	int GetItemNum(ItemID item_id);

	void GMExtendAll();

	void CheckImpGuardMailInKnapsack(time_t time);

	bool ConsumeItemOrUseGold(ItemID item_id, const int num, const char *reason, bool need_audit_log = true, bool is_use_bind_gold = false);

	void PushNoHaveSpaceWrapper(const MailContentParam &mail_content);
	bool UpdateToMail();

private:

	enum ITEM_CHANGE_TYPE
	{
		ITEM_CHANGE_TYPE_SNEAKY_IN = -4,	// 偷偷的放入 不需要通知玩家获得 当脱下装备和宝石镶嵌惩罚时使用这个
		ITEM_CHANGE_TYPE_CHANGE = -3,		// 发生改变
		ITEM_CHANGE_TYPE_OUT = -2,			// 从背包进入外部
		ITEM_CHANGE_TYPE_IN = -1,			// 从外部进入背包
		// 0以上表示是从背包/仓库的其他格子里移动过来/去 值表示原来的下标
	};

	void SendknapsackGridInfoToRole(int index, int change_type, short reason_type = PUT_REASON_INVALID);

	void CertainPut(const ItemDataWrapper &item_info, int reason_type, int *out_index, UInt16 drop_monster_id = 0);
	bool MoveHelper(int from_index, int to_index);

	Role *m_role;

	bool m_old_state[ItemNamespace::MAX_KNAPSACK_STORAGE_GRID_NUM];
	bool m_dirty_mark[ItemNamespace::MAX_KNAPSACK_STORAGE_GRID_NUM];
	bool m_knapsack_is_change;

	bool m_is_lock;												// 摆摊时锁定
	
	short m_curr_knapsack_valid_num;							// 当前背包最大格数 
	short m_curr_storage_valid_num;								// 当前仓库最大格数 
	short m_valid_grid_index_end;								// 当前背包中非空格子的再下一个格子序号
	int m_valid_grid_auto_add_count;							// 当前背包格子自增数

	ItemGridData m_grid_list[ItemNamespace::MAX_KNAPSACK_STORAGE_GRID_NUM];		// 背包仓库数据

	UInt32 m_knapsack_last_inorder_time;						// 上次背包整理时间
	UInt32 m_storage_last_inorder_time;							// 上次仓库整理时间
	UInt32 m_last_inorder_merge_time;							// 上次整理并合并时间

	Money m_money;												// 钱
	ItemColddownManager m_item_codldown_manager;				// 冷却时间
	std::vector<MailContentParam> m_no_have_space_mail_vec;		// 执行CertainPut时背包没有空间了，在发邮件前先放这里，最小化发邮件数量
};

#endif

