#ifndef __CROSS_BIANJINGZHIDI_CONFIG_HPP__
#define __CROSS_BIANJINGZHIDI_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"

#include <vector>
#include <map>


// ��������
struct CrossBianJingZhiDiOtherCfg
{
	CrossBianJingZhiDiOtherCfg() : 
		is_open(false), server_open_day(0), m_min_level(0), 
		m_sceneid(0), relive_pos(0, 0),
		double_reward_limit_time(0), gather_buff_gold(0), gather_buff_time(0), 
		start_reflush_time_s(0), reflush_time_s(0) {}

	bool is_open;
	int server_open_day;	// ����߾�֮�� �����ڼ��쿪��
	int m_min_level;		// ����߾�֮�� ���Ƶȼ�

	int m_sceneid;			// ����߾�֮�� ����ID
	Posi relive_pos;		// ��ɫ������

	int double_reward_limit_time;
	int gather_buff_gold;
	int gather_buff_time;

	int start_reflush_time_s;
	int reflush_time_s;
};

// ����ʱ��
struct CrossBianJingZhiDiActivityOpenTimeCfg
{
	CrossBianJingZhiDiActivityOpenTimeCfg() : activity_week_day(0), activity_ready_time(0), activity_start_time(0), activity_end_time(0), ready_dur_s(0), open_dur_s(0) {}

	int activity_week_day;													// ������ܼ�
	int activity_ready_time;												// ��ȴ�ʱ���
	int activity_start_time;												// ���ʼʱ���
	int activity_end_time;													// �����ʱ���

	int ready_dur_s;														// ��ȴ�ʱ��
	int open_dur_s;															// ��������ʱ��
};

// BOSS����
struct CrossBianJingZhiDiBossCfg
{
	CrossBianJingZhiDiBossCfg() : boss_type(0), count(0), start_reflush_time_s(0), reflush_time_s(0), pos_id_start(0), pos_id_len(0) {}

	int boss_type;
	int count;
	int start_reflush_time_s;
	int reflush_time_s;
	int pos_id_start;
	int pos_id_len;
};

// ����ȼ�BOSS����
struct CrossBianJingZhiDiBossPool
{
	static const int MAX_BOSS_ID_COUNT = 20;

	CrossBianJingZhiDiBossPool() : count(0) {}

	struct BossItem
	{
		BossItem() : world_level(0), boss_id(0) { }

		int world_level;
		int boss_id;
	};

	int count;
	BossItem item_list[MAX_BOSS_ID_COUNT];
};

// С������
struct CrossBianJingZhiDiMonsterCfg
{
	CrossBianJingZhiDiMonsterCfg() : monster_id(0), count(0), start_reflush_time_s(0), reflush_time_s(0), pos_id_start(0), pos_id_len(0) {}

	int monster_id;
	int count;
	int start_reflush_time_s;
	int reflush_time_s;
	int pos_id_start;
	int pos_id_len;
};

// �ɼ�������
struct CrossBianJingZhiDiGatherCfg
{
	CrossBianJingZhiDiGatherCfg() { }

	struct GatherItem
	{
		GatherItem() : boss_type(0), gather_id(0), gather_time(0), pos_id_start(0), pos_id_len(0), disappear_s(0), item_count(0) {}

		int boss_type;
		int gather_id;
		int gather_time;
		int pos_id_start;
		int pos_id_len;
		int disappear_s;

		int item_count;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
	};

	GatherItem gather_list[CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE];
};


//////////////////////////////////////////////////////////////////////////
class CrossBianJingZhiDiConfig
{
public:
	CrossBianJingZhiDiConfig();
	~CrossBianJingZhiDiConfig();

	static CrossBianJingZhiDiConfig & Instance();

	bool Init(const std::string &path, std::string *err);
	int InitOther(PugiXmlNode RootElement);
	int InitTaskList(PugiXmlNode RootElement);
	int InitFlushPoint(PugiXmlNode RootElement);
	int InitBoss(PugiXmlNode RootElement);
	int InitMonster(PugiXmlNode RootElement);
	int InitGather(PugiXmlNode RootElement);
	int InitBoosIdByWorldLevel(PugiXmlNode RootElement);
	int InitActivityOpenTimeCfg(PugiXmlNode RootElement);
	int InitSosCfg(PugiXmlNode RootElemen);

	bool DayTimeFormatInvalid(int day_time);

	inline bool IsCrossBianJingZhiDiScene(int scene_id) { return (0 != scene_id && scene_id == m_bianjingzhidi_other_cfg.m_sceneid); };

	const CrossBianJingZhiDiOtherCfg & GetCrossBianJingZhiDiOtherCfg() { return m_bianjingzhidi_other_cfg; }
	CrossBianJingZhiDiActivityOpenTimeCfg *GetActivityOpenTimeInfo(int index);
	int GetOpenActivityConfigList(CrossBianJingZhiDiActivityOpenTimeCfg cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT]) const;

	void GetCrossBianJingZhiDiTaskList(CrossBianJingZhiDiTaskInfo bianjingzhidi_task_list[CROSS_BIANJINGZHIDI_TASK_MAX]);
	const CrossBianJingZhiDiTaskInfo *GetCrossBianJingZhiDiTaskCfg(int task_id);

	const CrossBianJingZhiDiBossCfg* GetCrossBianJingZhiDiBossCfg(int boss_type);
	const int GetBossTypeByBossId(int boss_id);
	const CrossBianJingZhiDiMonsterCfg &GetCrossBianJingZhiDiMonsterCfg() { return m_bianjingzhidi_monster_cfg; }

	const CrossBianJingZhiDiGatherCfg::GatherItem *GetCrossBianJingZhiDiGatherCfg(int boss_type);
	const CrossBianJingZhiDiGatherCfg::GatherItem *GetCrossBianJingZhiDiGatherCfgByID(int gather_id);
	int GetSosCost(int times) const;

	bool GetRandomFlushPos(Posi &flush_pos, int id_star, int id_len);
	bool GetRandomFlushPos(std::vector<Posi>& flush_pos, int count, int id_star, int id_len);//����idȡ����

	int GetBoosIdByWorldLevel(int world_level, int boss_type);

private:

	std::map<int, CrossBianJingZhiDiTaskInfo> m_bianjingzhidi_task_map;	// ����߾�֮�� �������������Ϣ<task_id, info>

	CrossBianJingZhiDiOtherCfg m_bianjingzhidi_other_cfg;
	CrossBianJingZhiDiBossCfg m_bianjingzhidi_boss_cfg[CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE]; // ����߾�֮�� BOSS��Ϣ
	CrossBianJingZhiDiMonsterCfg m_bianjingzhidi_monster_cfg;			// ����߾�֮�� ����
	CrossBianJingZhiDiGatherCfg m_gather_cfg;							// ����߾�֮�� �ɼ�����Ϣ
	std::map<int, Posi> m_flush_point_map;								// ˢ�µ�

	CrossBianJingZhiDiBossPool m_boss_pool[CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE];	// ����ȼ���Ӧ��boos id

	int m_activity_open_time_cfg_list_count;							// �����ʱ����������
	CrossBianJingZhiDiActivityOpenTimeCfg m_activity_open_time_cfg_list[CROSS_ACTIVITY_OPEN_TIME_CFG_MAX_COUNT];	// �����ʱ��

	std::vector<int> m_sos_cost_cfg_vec;
};
#endif // __CROSS_BIANJINGZHIDI_CONFIG_HPP__

