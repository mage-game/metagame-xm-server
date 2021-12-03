#ifndef __CROSS_NIGHT_FIGHT_CONFIG_HPP__
#define __CROSS_NIGHT_FIGHT_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/servercommon.h"

struct NightFightOtherConfig
{
	NightFightOtherConfig() : is_open(false) {}

	bool is_open;
};

struct CrossNightFightActivityOpenTimeInfo
{
	CrossNightFightActivityOpenTimeInfo() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;													// ������ܼ�
	int activity_ready_time;												// ��ȴ�ʱ���
	int activity_start_time;												// ���ʼʱ���
	int activity_end_time;													// �����ʱ���

	int ready_dur_s;														// ��ȴ�ʱ��
	int open_dur_s;															// ��������ʱ��
};

class CrossNightFightConfig
{
public:
	CrossNightFightConfig();
	~CrossNightFightConfig();

	static CrossNightFightConfig & Instance();
	bool Init(const std::string &path, std::string *err);

	const NightFightOtherConfig & GetOtherCfg() { return m_other_cfg; }
	CrossNightFightActivityOpenTimeInfo *GetActivityOpenTimeInfo(int index);
	int GetOpenActivityConfigList(CrossNightFightActivityOpenTimeInfo cfg_list[CROSS_PASTURE_OPEN_TIME_MAX_COUNT]);

	bool DayTimeFormatInvalid(int day_time);

private:
	int InitOtherCfg(TiXmlElement *RootElement);
	int InitActivityOpenTimeCfg(TiXmlElement *RootElement);

	NightFightOtherConfig m_other_cfg;
	int m_activity_open_time_cfg_list_count;								// �����ʱ����������
	CrossNightFightActivityOpenTimeInfo m_activity_open_time_cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];	// �����ʱ��
};

#endif // __CROSS_NIGHT_FIGHT_CONFIG_HPP__

