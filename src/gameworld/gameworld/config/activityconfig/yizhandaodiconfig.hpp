#ifndef __YIZHANDAODI_CONFIG_HPP__
#define __YIZHANDAODI_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/activitydef.hpp"
#include <vector>
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct YiZhanDaoDiDeadConfig
{
	const static int MAX_DEAD_TIMES = 32;

	struct ConfigItem
	{
		ConfigItem() : dead_times(0), add_maxhp_percent(0), has_item_reward(0) {}
		
		int dead_times;
		int add_maxhp_percent;
		int has_item_reward;
		ItemConfigData reward_item;
	};

	YiZhanDaoDiDeadConfig() : max_dead_times(0) {}

	int max_dead_times;
	ConfigItem cfg_list[MAX_DEAD_TIMES];
};

enum YIZHAN_GATHER_BUFF_TYPE
{
	YIZHAN_BUFF_ADD_SPEED = 0,			// 加速
	YIZHAN_BUFF_DEC_SPEED,				// 减速
	YIZHAN_BUFF_ADD_GONGJI,				// 加攻
	YIZHAN_BUFF_DEC_GONGJI,				// 减攻
	YIZHAN_BUFF_ADD_FANGYU,				// 加防
	YIZHAN_BUFF_DEC_FANGYU,				// 减防
	YIZHAN_BUFF_RECOVER,				// 回复
	YIZHAN_BUFF_WUDI,					// 无敌
	YIZHAN_BUFF_BIANSHEN,				// 变身
	YIZHAN_BUFF_DEADLY_ATTACK,			// 致命一击

	YIZHAN_BUFF_MAX_TYPE,
};

struct YiZhanDaoDiGatherBuffCfg
{
	YiZhanDaoDiGatherBuffCfg(): buff_seq(0), buff_pro(0), buff_type(-1), is_notice(0)
	{
		memset(param, 0, sizeof(param));
	}

	short buff_seq;
	int	buff_pro;
	short buff_type;
	int param[4];
	short is_notice;
};

class YiZhanDaoDiConfig : public ConfigBase
{
public:
	YiZhanDaoDiConfig();
	~YiZhanDaoDiConfig();

	bool Init(const std::string &path, std::string *err);

	struct JiShaReward
	{
		JiShaReward() : reward_item_count(0) {}

		int reward_item_count;
		ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	};

	struct JiShaTitle
	{
		JiShaTitle() : jisha_count(0), title_id(0) {}

		int jisha_count;
		int title_id;
	};
	
	struct KillNumReward
	{
		KillNumReward():kill_count(0), reward_count(0)
		{
			memset(reward_item, 0, sizeof(reward_item));
		}
		int kill_count;
		int reward_count;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
	};

	struct JishaScoreReward
	{
		JishaScoreReward() : jisha_score_min(0), jisha_score_max(0), jisha_shengwang(0) {}

		int jisha_score_min;
		int jisha_score_max;
		int jisha_shengwang;
	};

	int GetOpenLevel() { return m_open_level; }
	int GetEnterPos(Posi *enter_pos);

	int GetDeadMaxCount() { return m_dead_max_count; }
	bool RandomRealivePos(Posi *pos);
	int GetJiShaTitle(int jisha_count);
	int GetLuckyInterval() { return m_lucky_interval; }
	const ItemConfigData & GetLuckyItem() { return m_lucky_item; }
	const ItemConfigData * GetJishaRankRewardItem(int rank_idx);

	const int GetJishaRankRewardItemCount(int rank_idx);

	int GetKillHonor() { return m_kill_honor; }

	void GetGongjiGuWuInfo(int *need_gold, int *max_gpngji_per, int *add_gongji_per);
	void GetMaxHPGuWuInfo(int *need_gold, int *max_hp_per, int *add_maxhp_per);
	int GetJishaShengwangReward(int jiasha_score);

	int GetMaxBeKillTimes() { return m_dead_cfg.max_dead_times; }
	const YiZhanDaoDiDeadConfig::ConfigItem * GetDeadCfg(int dead_times);

	int GetGatherID() { return m_gather_id;}
	int GetGatherNum() { return m_gather_num;}
	int GetGatherFirstInterval() { return m_first_refresh_interval;}
	int GetGatherInterval() { return m_refresh_interval;}
	int GetGatherTime() { return m_gather_time;}
	int GetGatherExitTime() { return m_gather_exit_time;}
	std::vector<Posi> & GetGatherPosiVec(){return m_gather_pos_list;}

	int GetGatherBuffToatalPro() { return m_gather_buff_total_pro;}
	std::map<int, YiZhanDaoDiGatherBuffCfg> & GetGatherBuffMap() { return m_gather_buff_map;}

	int GetBroactRoleNumInterval() {return m_broact_role_num_interval;}
	int GetBroactTitleID(){return m_broact_title_id;}

	int GetRankTitle(int rank);

	const KillNumReward* GetKillNumReward(int kill_count, int & index);

private:
	int InitKillNumReward(PugiXmlNode RootElement);

	int m_open_level;															// 开放等级
	int m_sceneid;																// 一站到底 场景ID

	int m_realive_pos_count;													// 一站到底 复活点个数
	Posi m_realive_poslist[YIZHANDAODI_REALIVE_POS_MAX];						// 一站到底 复活点坐标

	int m_dead_max_count;														// 复活最大次数（一次活动）

	int m_lucky_interval;														// 幸运奖励间隔
	ItemConfigData m_lucky_item;												// 幸运物品
	
	int m_reward_count;															// 击杀奖励个数
	JiShaReward m_jisha_reward_list[YIZHANDAODI_RANK_NUM + 1];					// 击杀榜奖励 前面100配置 最后一个统一 (100+1)

	int m_jisha_title_count;													// 击杀称号数量
	JiShaTitle m_jisha_title_list[YIZHANDAODI_JISHA_TITLE_MAX];					// 击杀称号列表
	
	int m_jisha_score_reward_count;
	JishaScoreReward m_jisha_score_reward_list[YIZHANDAODI_JISHA_SCORE_REWARD_MAX];// 击杀积分兑换荣誉列表

	int m_guwu_gongji_gold;														// 鼓舞攻击所需元宝
	int m_guwu_gongji_max_per;													// 鼓舞攻击最大次数
	int m_guwu_gongji_percent;													// 鼓舞加成攻击百分比

	int m_guwu_maxhp_gold;														// 鼓舞气血所需元宝
	int m_guwu_maxhp_max_per;													// 鼓舞气血最大次数
	int m_guwu_maxhp_percent;													// 鼓舞加成气血百分比

	int m_kill_honor;															// 击杀奖励荣誉

	YiZhanDaoDiDeadConfig m_dead_cfg;											// 被杀配置

	int m_gather_id;															// 采集物ID
	int m_gather_num;															// 采集物刷新数量
	int m_first_refresh_interval;												// 首次刷新间隔
	int m_refresh_interval;														// 刷新间隔
	int m_gather_time;															// 采集时间
	int m_gather_exit_time;														// 采集物存在时间
	std::vector<Posi> m_gather_pos_list;										// 采集物坐标列表

	int m_gather_buff_total_pro;
	std::map<int, YiZhanDaoDiGatherBuffCfg> m_gather_buff_map;

	int m_broact_role_num_interval;
	int m_broact_title_id;

	int m_rank_user_reward_title_id_list[YIZHANDAODI_REWARD_TITLE_RANK_COUNT];

	int m_reward_kill_list_count;
	KillNumReward m_reward_kill_num[YIZHANDAODI_JISHA_REWARD_NUM];

};

#endif // __YIZHANDAODI_CONFIG_HPP__

