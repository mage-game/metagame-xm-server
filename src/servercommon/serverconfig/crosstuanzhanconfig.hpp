#ifndef __CROSS_TUANZHAN_CONFIG_HPP__
#define __CROSS_TUANZHAN_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/activitydef.hpp"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct CrossTuanzhanOtherCfg
{
	CrossTuanzhanOtherCfg() : is_open(false), scene_id(0), scene_standby_dur_s(0), kill_score(0), assist_score(0), scene_add_score_time(0), 
		static_score(0), pillar_time(0), pillar_player_score(0), pillar_teammate_score(0), weak_buff_need_been_kill(0), weak_buff_hp_percent(0), 
		winner_cross_honor(0), winner_rank_num(0), join_honor_factor_percent(0), join_honor_max(0), join_honor_min(0), rand_user_side_time(0){}

	bool is_open;															// �Ƿ񿪷ű�����-�ܿ���

	int scene_id;															// ��ս����ID

	int scene_standby_dur_s;												// ����׼��ʱ�䣨�룩
	
	int kill_score;															// ��ɱ��һ�û���
	int assist_score;														// ������û���

	int scene_add_score_time;												// ս����û��ּ�����룩
	int static_score;														// ս����ʱ��û���
	int pillar_time;														// ���ӻ�û��ּ�����룩
	int pillar_player_score;												// ���Ӷ�ʱ�Ӹ��˻���
	int pillar_teammate_score;												// ���Ӷ�ʱ�Ӷ��ѻ���

	int weak_buff_need_been_kill;											// �������buff����������ɱ��
	int weak_buff_hp_percent;												// ������Ѫ�ӳɰٷֱ�

	int winner_cross_honor;													// ʤ����������
	int winner_rank_num;													// ʤ����Ӫǰ����
	
	int winner_rank_reward_item_list_count;
	ItemConfigData winner_rank_reward_item_list[CROSS_TUANZHAN_WINNER_REWARD_ITEM_MAX_COUNT];// ʤ����Ӫǰ����������Ʒ

	int join_honor_factor_percent;											// ��������ϵ���ٷֱ�
	int join_honor_max;														// ������������
	int join_honor_min;														// ������������
	int rand_user_side_time;												// �����Ӫ�ļ��ʱ��
};

struct CrossTuanzhanPillaCfg
{
	CrossTuanzhanPillaCfg() : pilla_count(0) {}

	struct CrossTuanzhanPillaConfigItem
	{
		CrossTuanzhanPillaConfigItem() : index(0), pilla_id(0), pilla_side_0(0), pilla_side_1(0) {}

		int index;
		int pilla_id;															// ���ӹ���id
		int pilla_side_0;														// ���ӹ���id 0��
		int pilla_side_1;														// ���ӹ���id 1��
		Posi pilla_pos;															// ���ӳ�����
	};

	int pilla_count;														// ��������
	CrossTuanzhanPillaConfigItem pilla_list[CROSS_TUANZHAN_PILLA_MAX_COUNT];
};

struct ActivityOpenTimeInfo
{
	ActivityOpenTimeInfo() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;													// ������ܼ�
	int activity_ready_time;												// ��ȴ�ʱ���
	int activity_start_time;												// ���ʼʱ���
	int activity_end_time;													// �����ʱ���

	int ready_dur_s;														// ��ȴ�ʱ��
	int open_dur_s;															// ��������ʱ��
};


struct CrossTuanzhanScoreRewardCfg
{
	CrossTuanzhanScoreRewardCfg() : seq(0), need_score(0), reward_currency(0), cross_honor(0), reward_item_count(0) {}

	int seq;
	int need_score;
	int reward_currency;
	int cross_honor;
	int reward_item_count;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct CrossTuanzhanScoreRewardLevelCfg
{
	CrossTuanzhanScoreRewardLevelCfg() : level(0), score_reward_cfg_list_count(0) {}

	int level;
	int score_reward_cfg_list_count;
	CrossTuanzhanScoreRewardCfg score_reward_cfg_list[CROSS_TUANZHAN_SCORE_REWARD_CFG_MAX];
};

struct CrossTuanzhanRankRewardCfg
{
	CrossTuanzhanRankRewardCfg() : rank(0), reward_currency(0), cross_honor(0), reward_item_count(0) {}

	int rank;
	int reward_currency;
	int cross_honor;
	int reward_item_count;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct CrossTuanzhanRankRewardLevelCfg
{
	CrossTuanzhanRankRewardLevelCfg() : level(0), rank_reward_cfg_list_count(0) {}

	int level;
	int rank_reward_cfg_list_count;
	CrossTuanzhanRankRewardCfg rank_reward_cfg_list[CROSS_TUANZHAN_RANK_REWARD_CFG_MAX];
};

class CrossTuanzhanConfig
{
public:
	CrossTuanzhanConfig();
	~CrossTuanzhanConfig();

	static CrossTuanzhanConfig & Instance();
	bool Init(const std::string &path, std::string *err);

	const CrossTuanzhanOtherCfg & GetOtherCfg() { return m_other_cfg; }
	const CrossTuanzhanPillaCfg & GetPillaCfg() { return m_pilla_cfg; }
	ActivityOpenTimeInfo *GetActivityOpenTimeInfo(int index);				// ��ȡ��Ŀ�������
	Posi GetRandomRelivePos();												// ��ȡһ������ĸ����

	int GetOpenActivityConfig(ActivityOpenTimeInfo cfg_list[CROSS_TUANZHAN_OPEN_TIME_MAX_COUNT]);
	const CrossTuanzhanScoreRewardCfg *GetCrossTuanzhanScoreRewardCfg(int role_level, int score);
	const CrossTuanzhanScoreRewardCfg *GetCrossTuanzhanScoreRewardCfgBySeq(int role_level, int seq);
	const CrossTuanzhanRankRewardCfg *GetCrossTuanzhanRankRewardCfg(int role_level, int score);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitActivityOpenTimeCfg(PugiXmlNode RootElement);
	int InitPillaCfg(PugiXmlNode RootElement);
	int InitReliveCfg(PugiXmlNode RootElement);
	int InitScoreRewardCfg(PugiXmlNode RootElement);
	int InitRankRewardCfg(PugiXmlNode RootElement);

	bool DayTimeFormatInvalid(int day_time);

	CrossTuanzhanOtherCfg m_other_cfg;
	CrossTuanzhanPillaCfg m_pilla_cfg;										// ������Ϣ

	int m_activity_open_time_cfg_list_count;								// �����ʱ����������
	ActivityOpenTimeInfo m_activity_open_time_cfg_list[CROSS_TUANZHAN_OPEN_TIME_MAX_COUNT];	// �����ʱ��

	int m_player_relive_pos_list_count;										// ���������
	Posi m_player_relive_pos_list[CROSS_TUANZHAN_RELIVE_POS_MAX_COUNT];

	int m_score_reward_level_cfg_count;
	CrossTuanzhanScoreRewardLevelCfg m_score_level_reward_cfg_list[CROSS_TUANZHAN_SCORE_REWARD_LEVEL_CFG_MAX];

	int m_rank_reward_level_cfg_count;
	CrossTuanzhanRankRewardLevelCfg m_rank_reward_level_cfg_list[CROSS_TUANZHAN_SCORE_REWARD_LEVEL_CFG_MAX];
};

#endif // __CROSS_1V1_COMMON_CONFIG_HPP__

