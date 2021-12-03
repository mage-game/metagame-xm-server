#ifndef __GAMELOG_H__
#define __GAMELOG_H__

#include "ilogmodule.h"
#include "servercommon/servercommon.h"
#include "servercommon/logdef.h"

class ILogModule;
struct ItemParamDataStruct;

namespace gamelog
{
	extern LogAgent g_log_world;
	extern LogAgent g_log_scenemanager;
	extern LogAgent g_log_scene;
	extern LogAgent g_log_messagehandler;

	extern LogAgent g_log_scenestatus;			// 场景状态日志
	extern LogAgent g_log_save_error;			// RMI存储错误
	extern LogAgent g_log_msgmoniter;			// 内存池检测

	extern LogAgent g_log_moneygold;			// 元宝日志
	extern LogAgent g_log_moneycoin;			// 铜钱日志
	extern LogAgent g_log_knapsack;				// 角色背包仓库日志
	extern LogAgent g_log_item;					// 物品功能日志
	extern LogAgent g_log_xiannv;				// 仙女日志
	extern LogAgent g_log_wing;					// 羽翼日志
	extern LogAgent g_log_halo;					// 光环日志
	extern LogAgent g_log_shengong;				// 神弓日志
	extern LogAgent g_log_shenyi;				// 神翼日志
	extern LogAgent g_log_footprint;			// 足迹日志
	extern LogAgent g_log_cloak;				// 披风日志
	extern LogAgent g_log_equipment;			// 装备功能日志
	extern LogAgent g_log_task;					// 任务日志
	extern LogAgent g_log_task_stats;			// 任务统计日志
	extern LogAgent g_log_vip;					// VIP日志
	extern LogAgent g_log_chestshop;			// 宝箱商店
	extern LogAgent g_log_movechess;			// 走棋子
	extern LogAgent g_log_mount;				// 坐骑日志
	extern LogAgent g_log_publicsale;			// 拍卖日志
	extern LogAgent g_log_mail;					// 邮件日志
	extern LogAgent g_log_guild_route;			// 军团日志
	extern LogAgent g_log_npcshop;				// npc商店
	extern LogAgent g_log_shop;					// 商城
	extern LogAgent g_log_convertshop;			// 兑换商店
	extern LogAgent g_log_roleactivity;			// 角色活动
	extern LogAgent g_log_rand_activity;		// 随机活动
	extern LogAgent g_log_qingyuan;				// 情缘
	extern LogAgent g_log_roledata;				// 仙魂
	extern LogAgent g_log_marry;				// 结婚

	extern LogAgent g_log_mentality;			// 将心
	extern LogAgent g_log_function_stats;		// 功能统计日志
	extern LogAgent g_log_role_rename;			// 角色改名
	extern LogAgent g_log_role_upgrade;			// 角色升级
	extern LogAgent g_log_friends;				// 好友系统日志
	extern LogAgent g_log_drop;					// 掉落日志

	extern LogAgent g_log_welfare;				// 福利日志
	extern LogAgent g_log_monitor;				// 监控日志
	extern LogAgent g_log_sundry;				// 杂项日志

	extern LogAgent g_log_cmd;					// 外部系统命令日志
	extern LogAgent g_log_debug;				// 用于一些调试日志输出
	extern LogAgent g_log_serious_error;		// 严重错误 
	extern LogAgent g_log_disconnect;			// 断掉连接日志
	extern LogAgent g_log_login;				// 登录登出日志
	extern LogAgent g_log_gm;					// GM命令日志
	extern LogAgent g_log_strengthen_test;		// 强化统计日志
	extern LogAgent g_log_forbid;				// 禁言封号日志
	extern LogAgent g_log_quick;				// 快速日志
		
	extern LogAgent g_log_guild;				// 仙盟日志
	extern LogAgent g_log_activity;				// 活动日志
	extern LogAgent g_log_p2ptrade;				// p2p交易日志
	extern LogAgent g_log_challengefield;		// 竞技场日志
	extern LogAgent g_log_other;				// 一些杂项
	extern LogAgent g_log_fb;					// 副本日志
	extern LogAgent g_log_worldboss;			// 世界boss日志
	extern LogAgent g_log_wabao;				// 挖宝日志
	extern LogAgent g_log_question;				// 答题日志

	extern LogAgent g_log_role_num;				// 在线人数日志 从login服移过来的
	extern LogAgent	g_log_global_user_state;	// 用户上下线日志 从login服移过来的

	extern LogAgent g_log_husong_task;			// 护送任务日志
	extern LogAgent g_log_guild_task;			// 仙盟任务日志
	extern LogAgent g_log_daily_task;			// 日常任务日志
	extern LogAgent g_log_month_card;			// 月卡
	extern LogAgent g_log_touzijihua;			// 投资计划
	extern LogAgent g_log_fish_pool;			// 鱼池
	extern LogAgent g_log_combine_server;		// 合服活动
	extern LogAgent g_log_cardzu;				// 卡牌组合
	extern LogAgent g_log_rank;					// 排行榜
	extern LogAgent g_log_cross;				// 跨服
	extern LogAgent g_log_lieming;				// 猎命
	extern LogAgent g_log_jingling;				// 精灵
	extern LogAgent g_log_chengjiu;				// 成就
	extern LogAgent g_log_shengwang;			// 声望
	extern LogAgent g_log_tuhaojin;				// 土豪金
	extern LogAgent g_log_bigchatface;			// 聊天大表情
	extern LogAgent g_log_shenzhou_weapon;		// 神州六器
	extern LogAgent g_log_baby;					// 宝宝
	extern LogAgent g_log_pet;					// 宠物
	extern LogAgent g_log_appearance;			// 玩家外观
	extern LogAgent g_log_huashen;				// 化神系统
	extern LogAgent g_log_multi_mount;			// 双人坐骑
	extern LogAgent g_log_personalize_window;	// 个性化窗口
	extern LogAgent g_log_magic_card;			// 魔卡
	extern LogAgent g_log_mitama;				// 御魂
	extern LogAgent g_log_chinese_zodiac;		// 星座系统
	extern LogAgent g_log_xiannv_shouhu;		// 仙女守护
	extern LogAgent g_log_jingling_fazhen;		// 精灵法阵
	extern LogAgent g_log_jingling_guanghuan;	// 精灵光环
	extern LogAgent g_log_zhuansheng_equip;		// 转生系统
	extern LogAgent g_log_little_pet;			// 小宠物
	extern LogAgent g_log_equip_suit;			// 锻造套装
	extern LogAgent g_log_rune_system;			// 符文系统
	extern LogAgent g_log_shenge_system;		// 神格系统
	extern LogAgent g_log_shenbing;				// 神兵系统
	extern LogAgent g_log_title;				// 称号
	extern LogAgent g_log_fighting;				// 决斗场系统
	extern LogAgent g_log_shenshou;				// 神兽
	extern LogAgent g_log_new_protocol;			// 新协议日志
	extern LogAgent g_log_tianxiang;			// 天象系统

	extern LogAgent g_log_fishing_exchange;		// 钓鱼兑换
	extern LogAgent g_log_fishing;				// 钓鱼活动日志
	extern LogAgent g_log_element_heart;		// 元素之心
	extern LogAgent g_log_upgrade;				// 进阶系统

	extern LogAgent g_log_shengqi;				// 圣器系统

	extern LogAgent g_log_yaoshi;				// 腰饰
	extern LogAgent g_log_toushi;				// 头饰
	extern LogAgent g_log_qilinbi;				// 麒麟臂
	extern LogAgent g_log_mask;					// 面具日志
	extern LogAgent g_log_zhaunzhi_equip;		// 转职装备日志
	extern LogAgent g_log_seal;					// 圣印系统

	extern LogAgent g_log_role_big_small_goal;  // 角色大小目标
	
	extern LogAgent g_log_shenqi;				// 神器

	extern LogAgent g_log_xianzunka;			// 仙尊卡

	extern LogAgent g_log_equip_baptize;        // 装备洗炼
	extern LogAgent g_log_greate_soldier;		// 名将

	extern LogAgent g_log_cross_rand_activity;	// 跨服随机活动

	extern LogAgent g_log_jinjie_sys_reward;	// 进阶系统大小目标

	extern LogAgent g_log_baizhan_equip;		// 百战装备日志

	extern LogAgent g_log_global_role;			// 全局角色日志
	extern LogAgent g_log_tianshenhuti;			// 周末装备

	extern LogAgent g_log_zodiac;				// 十二生肖

	extern bool g_open_console_log; 

	void LogInit(ILogModule *log, const std::string &logdir, bool open_console_log, int server_index);
}

#define CONSOLE_DEBUG(format, ...) \
	if (gamelog::g_open_console_log) printf("File: " __FILE__ ", Line: %05d: " format "\n", __LINE__, ##__VA_ARGS__); fflush(stdout);

#define ROLE_LOG_QUICK6(t, r, n1, n2, z1, z2) \
	LOG_QUICK8(gamelog::g_log_quick, (t), UidToInt((r)->GetUserId()), (r)->GetName(), (r)->GetPlatName(), (n1), (n2), (z1), (z2))

#define ROLE_LOG_QUICK10(t, r, n1, n2, z1, z2, n3, n4, n5, n6) \
	LOG_QUICK12(gamelog::g_log_quick, (t), UidToInt((r)->GetUserId()), (r)->GetName(), (r)->GetPlatName(), (n1), (n2), (z1), (z2), (n3), (n4), (n5), (n6))

#endif

