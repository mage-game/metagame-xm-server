#ifndef __MAGICAL_PRECIOUS_DEF_H__
#define __MAGICAL_PRECIOUS_DEF_H__

#include "servercommon.h"

#pragma pack(push, 4)

static const int MAGICAL_PRECIOUS_CHAPTER_COUNT = 16;																// 总章节数
static const int MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT = 16;													// 每章的奖励索引数
static const int MAGICAL_PRECIOUS_CHAPTER_FLAG_COUNT = 5;															// 奖励标记数组数量
static const int MAGICAL_PRECIOUS_REWARD_COUNT_PER_CHAPTER = MAGICAL_PRECIOUS_CHAPTER_FLAG_COUNT * 32;				// 每章节总任务数量

enum MAGICAL_PRECIOUS_REWARD_TYPE
{
	MAGICAL_PRECIOUS_REWARD_TYPE_INVALID = -1,

	MAGICAL_PRECIOUS_REWARD_TYPE_UNDONE = 0,					// 未完成
	MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH = 1,					// 可领取
	MAGICAL_PRECIOUS_REWARD_TYPE_FINISH = 2,					// 已领取

	MAGICAL_PRECIOUS_REWARD_TYPE_MAX
};

struct MagicalPreciousParam
{
	MagicalPreciousParam() { this->Reset(); }

	void Reset()
	{
		current_chapter = -1;
		reserve_ch = 0;
		reserve_sh = 0;
		chapter_invalid_time = 0;
		chapter_fetch_finish_reward_flag = 0;
		memset(chapter_score_list, 0, sizeof(chapter_score_list));
		memset(chapter_fetch_reward_flag, 0, sizeof(chapter_fetch_reward_flag));

		finish_guild_task_times = 0;
		finish_daily_task_times = 0;
		wabao_times = 0;
		open_guild_box_times = 0;
		hotspring_join_times = 0;
		hotspring_answer_right_times = 0;
		millionaire_box_times = 0;
		husong_times = 0;
		enter_exp_fb_times = 0;
		pass_phase_fb_times = 0;
		wangling_task_times = 0;
		tianjiangcaibao_task_times = 0;
		shuijing_task_times = 0;
		shuijing_gather_times = 0;
		kill_miku_boss = 0;
		kill_world_boss = 0;
		kill_active_boss = 0;
		kill_dabao_boss = 0;
		kill_other_role_in_fighting = 0;
		mine_times = 0;
		fish_num = 0;
		sailing_num = 0;
		kill_monster_num = 0;
		active_degree = 0;
		kill_vip_boss = 0;
		join_gongcheng_zhan_times = 0;
		reserve_sh1 = 0;

		kill_boss_count_1 = 0;
		kill_boss_count_2 = 0;
		kill_boss_count_3 = 0;
	}

	char current_chapter;									// 当前章节
	char reserve_ch;
	short reserve_sh;
	unsigned int chapter_invalid_time;						// 章节失效时间
	unsigned int chapter_fetch_finish_reward_flag;			// 章节完成奖励标志
	unsigned short chapter_score_list[MAGICAL_PRECIOUS_CHAPTER_COUNT]; // 每个章节的积分列表
	char chapter_fetch_reward_flag[MAGICAL_PRECIOUS_CHAPTER_COUNT][MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT];	// 章节积分状态标志，0是未完成，1是已完成且可领取积分，2是已领取积分

	unsigned short finish_guild_task_times;					// 完成公会任务次数
	unsigned short finish_daily_task_times;					// 完成日常任务次数
	unsigned short wabao_times;								// 挖宝次数
	unsigned short open_guild_box_times;					// 开启公会宝箱个数
	unsigned short hotspring_join_times;					// 参与跨服温泉次数
	unsigned short hotspring_answer_right_times;			// 跨服温泉回答正确次数
	unsigned short millionaire_box_times;					// 大富豪采集宝箱次数
	unsigned short husong_times;							// 护送次数
	unsigned short enter_exp_fb_times;						// 进入经验副本次数
	unsigned short pass_phase_fb_times;						// 通关阶段副本次数
	unsigned short wangling_task_times;						// 完成王陵探险任务总次数
	unsigned short tianjiangcaibao_task_times;				// 完成天降财宝任务总次数
	unsigned short shuijing_task_times;						// 完成水晶幻境任务总次数
	unsigned short shuijing_gather_times;					// 水晶幻境采集总次数
	unsigned short kill_miku_boss;							// 击杀精英boss总次数
	unsigned short kill_world_boss;							// 击杀世界boss总次数
	unsigned short kill_active_boss;						// 击杀活跃boss总次数
	unsigned short kill_dabao_boss;							// 击杀打宝boss总次数
	unsigned short kill_other_role_in_fighting;				// 击杀陌生人次数
	unsigned short mine_times;								// 挖矿次数
	unsigned short fish_num;								// 养鱼数
	unsigned short sailing_num;								// 航海
	unsigned short kill_monster_num;						// 击杀怪物数量
	unsigned short active_degree;							// 活跃度
	unsigned short kill_vip_boss;							// 击杀VIPboss总次数
	unsigned short join_gongcheng_zhan_times;				// 参加攻城战次数
	unsigned short reserve_sh1;								// 
	unsigned short kill_boss_count_1;						// 击杀boss X等级Y次数(精英+vip)
	unsigned short kill_boss_count_2;						// 击杀boss X等级Y次数次数(精英+vip)
	unsigned short kill_boss_count_3;						// 击杀boss X等级Y次数次数(精英+vip)
};

typedef char MagicalPreciousParamHex[sizeof(MagicalPreciousParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MagicalPreciousParamHex) < 2048);

#pragma pack(pop)

#endif

