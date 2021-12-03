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
	struct TaskColorFlushProbItem														// ������ɫˢ�¸�����Ϣ
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

		int color_problist[TASK_COLOR_MAX];												// ˢ�µ�������ɫ�ĸ���
	};

	struct TaskRewardCfg
	{
		TaskRewardCfg() : min_limitlevel(0), max_limitlevel(0), commit_exp(0), commit_bind_coin(0), item_count(0) {}

		bool InLimitLevel(int level) { return level >= min_limitlevel && level <= max_limitlevel; }

		int min_limitlevel;																// ��С�ȼ�
		int max_limitlevel;																// ���ȼ�
		int commit_exp;																	// ��������
		int commit_bind_coin;															// ������ͭ��
		int item_count;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];							// ��������Ʒ����
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
	int m_flush_itemid;																	// ˢ�µ���id
	int m_free_refresh_times;															// ���ˢ�´���

	int m_realive_scene_id;																// �����ID
	Posi m_realive_pos;																	// ��������

	int m_pretask_id;																	// ��������ID
	int m_max_rob_count;																// ������ٴ���
	int m_rob_no_reward_level_limit;													// �����޽����ȼ���

	int m_shield_continue_times;														// ���ܳ���ʱ��
	int m_dec_hurt_per;																	// �����˺��������

	int m_dec_speed_percent;

	TaskColorFlushProbItem m_taskcolor_problist[TASK_COLOR_MAX];						// ����������ɫ��ˢ�¸���
	int m_refresh_tocolor_maxtimes[TASK_COLOR_MAX];										// ˢ��ָ����ɫ�ı��״���

	int m_task_rewardcfg_count;															// �������õĸ���
	TaskRewardCfg m_task_rewardcfg_list[HUSONG_LEVEL_REWARD_CFG_MAX_COUNT];				// �����������б�

	int m_task_reward_factor_list[TASK_COLOR_MAX];										// ����ϵ��


	int m_buytimes_cfgcount;															// �������� ������
	BuyConfigItem m_buytimes_cfglist[HUSONG_BUY_TIMES_CFG_MAX_COUNT];					// ���������б�

	double m_exp_factor;																	// ����ϵ��

	int m_rob_exp_percent;																// ���ٻ�þ���ٷֱ�

	int m_be_rob_exp_percent;															// �����ٻ�þ���ٷֱ�
};

#endif // __HU_SONG_CONFIG_HPP__




