#ifndef __CROSS_PASTURE_CONFIG_HPP__
#define __CROSS_PASTURE_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/activitydef.hpp"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct CrossPastureOtherCfg
{
	CrossPastureOtherCfg() : is_open(false), scene_id(0), day_max_get_score_times(0), monster_create_interval_s(0), player_relive_time_s(0), dec_speed_percent(0), rank_title_id(0){}

	bool is_open;															// �Ƿ񿪷ű�����-�ܿ���

	int scene_id;															// ��������ID
	Posi role_pos;															// ��ҳ�����

	int day_max_get_score_times;											// ÿ������ȡ���ִ���	
	int monster_create_interval_s;											// �����������ɼ�����룩
	int player_relive_time_s;												// ��ɱ��һ�û���

	int dec_speed_percent;													// ���ٰٷֱ�
	int rank_title_id;													    
};

struct CrossPastureActivityOpenTimeInfo
{
	CrossPastureActivityOpenTimeInfo() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;													// ������ܼ�
	int activity_ready_time;												// ��ȴ�ʱ���
	int activity_start_time;												// ���ʼʱ���
	int activity_end_time;													// �����ʱ���

	int ready_dur_s;														// ��ȴ�ʱ��
	int open_dur_s;															// ��������ʱ��
};

struct CrossPastureMonsterInfoConfig
{
	CrossPastureMonsterInfoConfig() : monster_id(0), score(0), max_num(0), rand_pos_index_begin(0), rand_pos_index_end(0), weight(0), is_count(1), is_control_refresh(0){}

	int monster_id;
	int score;
	int max_num;
	int rand_pos_index_begin;
	int rand_pos_index_end;
	ItemConfigData reward_item;
	int weight;
	short is_count;
	short is_control_refresh;
};

struct CrossPastureFenceConfig
{
	CrossPastureFenceConfig() : fence_id(0), x1(0), x2(0), y1(0), y2(0) {}

	int fence_id;
	int x1;
	int x2;
	int y1;
	int y2;
};

struct CrossPastureSkillConfig
{
	CrossPastureSkillConfig() : index(0), skill_id(0) {}

	int index;
	int skill_id;
};

struct CrossPastureRankReward
{
	CrossPastureRankReward():min_rank(0),max_rank(0){}
	int min_rank;
	int max_rank;
	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
};

class CrossPastureConfig
{
public:
	CrossPastureConfig();
	~CrossPastureConfig();

	static CrossPastureConfig & Instance();
	bool Init(const std::string &path, std::string *err);

	const CrossPastureOtherCfg & GetOtherCfg() { return m_other_cfg; }
	CrossPastureActivityOpenTimeInfo *GetCrossPastureActivityOpenTimeInfo(int index);// ��ȡ��Ŀ�������
	Posi GetRandomMonsterCreatePosByMonsterId(int monster_id);									 // ��ȡһ������Ĺ��������

	int GetOpenActivityConfig(CrossPastureActivityOpenTimeInfo cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT]);
	int GetMonsterInfoCfgCount() { return m_monster_info_cfg_count; }
	const CrossPastureMonsterInfoConfig *GetMonsterInfoCfgWithIndex(int index);
	const CrossPastureMonsterInfoConfig *GetMonsterInfoCfgWithMonsterID(int monster_id);

	int GetFenceCfgCount() { return m_fence_cfg_count; }
	const CrossPastureFenceConfig *GetFenceCfg(int index);

	const CrossPastureSkillConfig *GetSkillCfgList() { return m_skill_cfg_list; }

	const CrossPastureMonsterInfoConfig *GetRandRewardWithMonsterID(int monster_id);

	int GetSpecialMonsterId() { return spical_monster_id; }

	const CrossPastureRankReward * GetRankRewardByRank(int rank) const;
private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitActivityOpenTimeCfg(PugiXmlNode RootElement);
	int InitReliveCfg(PugiXmlNode RootElement);
	int InitMonsterInfoCfg(PugiXmlNode RootElement);
	int InitFenceInfoCfg(PugiXmlNode RootElement);
	int InitSkillInfoCfg(PugiXmlNode RootElement);
	int InitRankReward(PugiXmlNode RootElement);

	bool DayTimeFormatInvalid(int day_time);

	CrossPastureOtherCfg m_other_cfg;

	int m_activity_open_time_cfg_list_count;								// �����ʱ����������
	CrossPastureActivityOpenTimeInfo m_activity_open_time_cfg_list[CROSS_PASTURE_OPEN_TIME_MAX_COUNT];	// �����ʱ��

	int m_monster_create_pos_list_count;									// ���������
	Posi m_monster_create_pos_list[CROSS_PASTURE_RELIVE_POS_MAX_COUNT];

	int m_monster_info_cfg_count;
	CrossPastureMonsterInfoConfig m_monster_info_cfg_list[CROSS_PASTURE_MONSTER_CFG_MAX_COUNT];// ������Ϣ

	int m_fence_cfg_count;
	CrossPastureFenceConfig m_fence_cfg_list[CROSS_PASTURE_FENCE_CFG_MAX_COUNT];// դ����Ϣ

	CrossPastureSkillConfig m_skill_cfg_list[CROSS_PASTURE_SKILL_CFG_MAX_COUNT];// ������Ϣ

	int spical_monster_id;														//����monster id
	std::vector<CrossPastureRankReward> m_rank_reward;
};

#endif // __CROSS_1V1_COMMON_CONFIG_HPP__

