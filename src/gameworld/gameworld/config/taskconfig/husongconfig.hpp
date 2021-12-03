#ifndef __HU_SONG_CONFIG_HPP__
#define __HU_SONG_CONFIG_HPP__

#include <string>
#include "servercommon/taskdef.hpp"
#include "servercommon/maildef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

class HuSongConfig : public ConfigBase
{
public:
	struct TaskColorFlushProbItem														// 任务颜色刷新概率信息
	{
		TaskColorFlushProbItem() { this->Reset(); }

		void Reset() { memset(color_problist, 0, sizeof(color_problist)); }

		bool CheckCfgValid()
		{
			int sum_prob = 0;
			for (int i = TASK_COLOR_WHITE; i < TASK_COLOR_MAX; ++i) 
			{
				sum_prob += color_problist[i];
			}

			return 100 == sum_prob;
		}

		int color_problist[TASK_COLOR_MAX];												// 刷新到各种颜色的概率
	};

	struct TaskRewardCfg
	{
		TaskRewardCfg() : min_limitlevel(0), max_limitlevel(0), commit_exp(0), commit_bind_coin(0), item_count(0) {}

		bool InLimitLevel(int level) { return level >= min_limitlevel && level <= max_limitlevel; }

		int min_limitlevel;																// 最小等级
		int max_limitlevel;																// 最大等级
		int commit_exp;																	// 交任务经验
		int commit_bind_coin;															// 交任务铜币
		int item_count;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];							// 交任务物品奖励
	};

	struct BuyConfigItem
	{
		BuyConfigItem() : buy_times(0), need_gold(0) {}

		int buy_times;
		int need_gold;
	};

	HuSongConfig();
	~HuSongConfig();

	bool Init(const std::string &path, std::string *err);

	int GetFlushItemId() { return m_flush_itemid; }

	const HuSongConfig::TaskColorFlushProbItem * GetTaskColorFlushProb(int task_color);
	int GetRefreshToColorMaxTimes(int color);
	int GetRealivePos(Posi *pos);
	int GetPreTaskID() { return m_pretask_id; }
	int GetMaxRobCount() { return m_max_rob_count; }
	int GetRobNoRewardLevelLimit() { return m_rob_no_reward_level_limit; }
	int GetShieldTime(){return m_shield_continue_times;}
	int GetShieldDecHurtPer(){return m_dec_hurt_per;}
	int GetSpeedDecPer(){return m_dec_speed_percent;}

	const TaskRewardCfg * GetTaskRewardCfg(int level);
	int GetTaskRewardFactor(int task_color);
	int GetBuyTimesCost(int times);
	int GetFreeRefreshTimes() { return m_free_refresh_times; }
	double GetExpFactor() { return m_exp_factor; }
	int GetRobExpFactor() { return m_rob_exp_percent; }
	int GetBeRobExpFactor() { return m_be_rob_exp_percent; }
	double GetExpFactorFind();

private:
	int m_flush_itemid;																	// 刷新道具id
	int m_free_refresh_times;															// 免费刷新次数

	int m_realive_scene_id;																// 复活场景ID
	Posi m_realive_pos;																	// 复活坐标

	int m_pretask_id;																	// 新手任务ID
	int m_max_rob_count;																// 最大抢劫次数
	int m_rob_no_reward_level_limit;													// 抢劫无奖励等级差

	int m_shield_continue_times;														// 护盾持续时间
	int m_dec_hurt_per;																	// 护盾伤害减免比率

	int m_dec_speed_percent;

	TaskColorFlushProbItem m_taskcolor_problist[TASK_COLOR_MAX];						// 护送任务颜色的刷新概率
	int m_refresh_tocolor_maxtimes[TASK_COLOR_MAX];										// 刷到指定颜色的保底次数

	int m_task_rewardcfg_count;															// 奖励配置的个数
	TaskRewardCfg m_task_rewardcfg_list[HUSONG_LEVEL_REWARD_CFG_MAX_COUNT];				// 任务奖励配置列表

	int m_task_reward_factor_list[TASK_COLOR_MAX];										// 奖励系数


	int m_buytimes_cfgcount;															// 购买配置 配置数
	BuyConfigItem m_buytimes_cfglist[HUSONG_BUY_TIMES_CFG_MAX_COUNT];					// 购买配置列表

	double m_exp_factor;																	// 经验系数

	int m_rob_exp_percent;																// 抢劫获得经验百分比

	int m_be_rob_exp_percent;															// 被抢劫获得经验百分比
};

#endif // __HU_SONG_CONFIG_HPP__




