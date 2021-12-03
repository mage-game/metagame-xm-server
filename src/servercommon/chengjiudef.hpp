#ifndef __CHENGJIU_DEF_HPP__
#define __CHENGJIU_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)

enum CHENGJIU_CONDITION
{
	CJ_CONDITION_INVALID = 0,

	CJ_CONDITION_KILL_MONSTER,					// 成功击杀n只怪物(普通怪）
	CJ_CONDITION_ADD_FRIEND,					// 成功添加n个好友
	CJ_CONDITION_JOIN_TEAM,						// 第n次加入队伍
	CJ_CONDITION_COMPOSE,						// 第n次合成
	CJ_CONDITION_JOIN_GUILD,					// 第n次加入公会
	CJ_CONDITION_EQUIP_STRENGTH,				// 第n次装备强化
	CJ_CONDITION_EQUIP_UPSTAR,					// 第n次装备升星
	CJ_CONDITION_EQUIP_SHENZHU,					// 第n次装备神铸(无用)
	CJ_CONDITION_EQUIP_UPLEVEL,					// 第n次装备升级
	CJ_CONDITION_ROLE_LEVEL,					// 人物等级达n级
	CJ_CONDITION_LOGIN_LAST_DAY,				// 累计登陆达n天
	CJ_CONDITION_MONEY_GOLD_BIND,				// 绑定钻石数量达
	CJ_CONDITION_KILL_BOSS,						// 成功击杀n只boss
	CJ_CONDITION_PUTON_EQUIP_STRENGTH,			// 穿戴一件强化+x的装备
	CJ_CONDITION_PUTON_EQUIP_STAR,				// 穿戴一件xxx星的装备
	CJ_CONDITION_CHENGJIU_TITLE,				// 成就达到“xxx”
	CJ_CONDITION_MOUNT_JIE,						// 坐骑达到xx阶
	CJ_CONDITION_SKILL_TOTAL_LEVEL,				// 技能总等级
	CJ_CONDITION_XIANJIE_LEVEL,					// 仙阶等级(无用)
	CJ_CONDITION_WING_JIE_CHANGE,				// 羽翼达到n阶
	CJ_CONDITION_FABAO_LEVEL,					// 法宝达到xx级
	CJ_CONDITION_EQU_1_STAR_AND_COLOR,			// 穿戴橙色一星或者红色一星
	CJ_CONDITION_EQU_2_STAR_AND_COLOR,			// 穿戴橙色二星或者红色二星
	CJ_CONDITION_EQU_3_STAR_AND_COLOR,			// 穿戴红色三星
	CJ_CONDITION_EQU_JIEZHI,					// 首次穿戴戒指
	CJ_CONDITION_YSZC_KILL_ROLE,				// 元素战场击杀n名玩家
	CJ_CONDITION_XLT_KILL_ROLE,					// 跨服修罗塔击杀n名玩家
	CJ_CONDITION_GCZ_WIN_TIMES,					// 攻城战获得n次胜利
	CJ_CONDITION_1V1_WIN_TIMES,					// 跨服1v1获得n次胜利
	CJ_CONDITION_HUSONG_TIMES,					// 护送马车n次
	CJ_CONDITION_EXP_FB_PASS_LEVEL,				// 经验副本进入次数
	CJ_CONDITION_PATA_PASS_LEVEL,				// 通关勇者之塔xx层
	CJ_CONDITION_GUILD_FINISH_TASK,				// 完成n个公会任务
	CJ_CONDITION_GUILD_DONATION_EQU,			// 公会捐献n件装备
	CJ_CONDITION_LTZ_SKILL_TA,					// 领土战摧毁xx座防御塔
	CJ_CONDITION_SKILL_ROLE,					// 在任意场景击杀n名玩家
	CJ_CONDITION_SKILL_RED_NAME_ROLE,			// 在任意场景击杀n名红名玩家
	CJ_CONDITION_GUANG_HUAN_LEVEL,				// 光环达到xx阶
	CJ_CONDITION_MO_QI_LEVEL,					// 魔骑达到xx阶
	CJ_CONDITION_ACTIVITE_MOJIE,				// 激活魔戒xx
	CJ_CONDITION_RESERVE1,						// 预留1
	CJ_CONDITION_RESERVE2,						// 预留2
	CJ_CONDITION_RESERVE3,						// 预留3
	CJ_CONDITION_ACTIVITE_NVSHEN,				// 激活女神xx
	CJ_CONDITION_SHEN_GONG_LEVEL,				// 神弓达到xx阶
	CJ_CONDITION_SHEN_WING_LEVEL,				// 神翼达到xx阶
	CJ_CONDITION_XUN_ZHANG_TOTLE_LEVEL,			// 勋章总等级至xxx级
	CJ_CONDITION_SJHJ_SHUI_JIN_COUNT,			// 水晶幻境采集xx个水晶
	CJ_CONDITION_SKILL_GUILD_BOSS,				// 参与击杀n只公会boss
	CJ_CONDITION_LIEQU_JINGLING_TOTAL,			// 猎取xx只精灵
	CJ_CONDITION_ZHUANGBEI_JINGLING_LEVEL,		// 装备一只xx级的精灵
	CJ_CONDITION_LIEQU_JINGLING,				// 成功猎取精灵xx
	CJ_CONDITION_NVSHEN_LEVEL,					// 一位女神资质等级升至xx级
	CJ_CONDITION_FOOTPRINT_LEVEL,				// 足迹达到xx阶

	CJ_CONDITION_MAX,
};

enum CHENGJIU_REWARD_STATUS
{
	CJ_REWARD_STATUS_CAN_NOT_FETCH = 0,			// 不可领取
	CJ_REWARD_STATUS_CAN_FETCH,					// 可领取
	CJ_RWARD_STATUS_BE_FETCHED,					// 已领取
};

static const int CHENGJIU_REWARD_MAX_NUM = 400;					//成就奖励最大数
static const int CHENGJIU_REWARD_MAX_CHANGE_NUM_INBATCH = 80;	//成就奖励批量改变最大数
UNSTD_STATIC_CHECK(CJ_CONDITION_MAX < CHENGJIU_REWARD_MAX_NUM);
UNSTD_STATIC_CHECK(CJ_CONDITION_MAX < CHENGJIU_REWARD_MAX_CHANGE_NUM_INBATCH);

struct ChengJiuRewardItem
{
	ChengJiuRewardItem()
	{
		this->Reset();
	}
	
	void Reset()
	{
		flag = 0;
		reserve_ch = 0;
		reserve_sh = 0;
		process = 0;
	}

	char  flag;
	char  reserve_ch;

	short reserve_sh;
	unsigned int process;
};

struct ChengJiuParam
{
	static const int CHENGJIU_SHUXINGDAN_MAX_TYPE = 3;

	ChengJiuParam()
	{
		this->Reset();
	}

	void Reset()
	{
		title_level = 0;
		reserve_sh1 = 0;

		fuwen_level = 0;
		process_times = 0;
		day_times = 0;

		for (int i = 0; i < CHENGJIU_REWARD_MAX_NUM; ++ i)
		{
			reward_list[i].Reset();
		}
		memset(shuxingdan_list, 0, sizeof(shuxingdan_list));
	}

	short title_level;
	short reserve_sh1;

	short fuwen_level;
	short process_times;
	int day_times;

	ChengJiuRewardItem reward_list[CHENGJIU_REWARD_MAX_NUM];

	int shuxingdan_list[CHENGJIU_SHUXINGDAN_MAX_TYPE];						//属性丹列表
};

typedef char ChengJiuParamHex[sizeof(ChengJiuParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ChengJiuParamHex) < 8192);

#pragma pack(pop)
#endif
