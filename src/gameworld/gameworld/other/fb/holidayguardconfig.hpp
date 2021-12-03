#ifndef _HOLIDAYGUARD_CONFIG_HPP_
#define _HOLIDAYGUARD_CONFIG_HPP_

#include "servercommon/servercommon.h"
//#include "towerdefendteamskill.hpp"
#include "servercommon/configbase.h"
#include "config/activityconfig/randactivityconfig.hpp"
#include <map>
#include <unordered_map>

static const int HOLIDAYGUARD_PATH_POINT_COUNT = 4;					// ·���ϵĹյ�����
static const int HOLIDAYGUARD_HOME_REALIVE_LIMIT_INTERVAL = 0;		// ��Ѹ�������ʱ���� ����
static const int HOLIDAYGUARD_GOLD_REALIVE_LIMIT_INTERVAL = 0;		// Ԫ����������ʱ���� ����
static const int PERSONAL_HOLIDAYGUARD_MAX_WAVE_PER_LEVEL = 10;		// ��������ÿ�������
static const int HOLIDAYGUARD_WARNING_PERCENT_COUNT = 2;			// ������������Ѫ���ٷֱȷ�ֵ
static const int PERSONAL_HOLIDAYGUARD_SKILL_MAX_COUNT = 2;			// ���������������
static const int HOLIDAYGUARD_BUFF_DROP_WEIGHT_COUNT = 5;			// BUFF����Ȩ���������
static const int HOLIDAYGUARD_NPC_CFG_MAX_COUNT = 200;				// npc�����������
static const int NPC_SCENE_REFRESH_LIMIT_COUT = 5;					// npc����ˢ������

enum HOLIDAYGUARD_PATH												// ˢ��·��
{
	HOLIDAYGUARD_PATH_INVALID = -1,
	HOLIDAYGUARD_PATH_LEFT,											// ��
	HOLIDAYGUARD_PATH_MID,											// ��
	HOLIDAYGUARD_PATH_RIGHT,										// ��

	HOLIDAYGUARD_PATH_MAX,
};

enum HOLIDAYGUARD_WARNING_PERCENT
{
	HOLIDAYGUARD_WARNING_PERCENT_50 = 50,
	HOLIDAYGUARD_WARNING_PERCENT_10 = 10,
};

struct HolidayGuardOtherCfg
{
	HolidayGuardOtherCfg() : free_join_times(0), first_wave_refresh_delay(0),
		wave_refresh_interval(0), life_tower_monster_id(0), warning_interval_s(0), sync_interval_s(0), buff_exit_time(0), npc_refresh_interval(0), npc_refresh_count(0), leave_fb_reward_count(0), everyday_can_fetch_reward_count(0),
		time_within_tread_finish_refresh(0)
	{

	}

	int free_join_times;

	int first_wave_refresh_delay;
	int wave_refresh_interval;
	int life_tower_monster_id;
	int warning_interval_s;
	int sync_interval_s;
	int buff_exit_time;
	unsigned int fb_begin_time;
	unsigned int fb_end_time;
	int npc_refresh_interval;
	int npc_refresh_count;
	ItemConfigData leave_fb_reward_list_cfg[MAX_ATTACHMENT_ITEM_NUM];
	int leave_fb_reward_count;
	int everyday_can_fetch_reward_count;
	int time_within_tread_finish_refresh;
};

struct HolidayGuardWaveCfg
{
	HolidayGuardWaveCfg() : wave(0), next_wave_s(0), total_monster_count(0)
	{
		memset(monster_id_list, 0, sizeof(monster_id_list));
		memset(monster_count_list, 0, sizeof(monster_count_list));
	}

	int wave;
	int next_wave_s;													// ��ô���������һ��
	int monster_id_list[HOLIDAYGUARD_PATH_MAX];							// ÿ��·�߳��Ĺ�ID
	int monster_count_list[HOLIDAYGUARD_PATH_MAX];						// ÿ��·�߳��Ĺ�����
	int total_monster_count;											// ������
};

struct HolidayGuardLevelCfg
{
	static const int CONFIG_ITEM_MAX_COUNT = 10;
	UNSTD_STATIC_CHECK(CONFIG_ITEM_MAX_COUNT < 128);

	struct ConfigItem
	{
		ConfigItem() : level(0), role_level(0), scene_id(0), birth_pos(0, 0), wave_count(0)
		{

		}

		int level;
		int role_level;
		int scene_id;
		Posi birth_pos;

		int wave_count;
		HolidayGuardWaveCfg wave_list[PERSONAL_HOLIDAYGUARD_MAX_WAVE_PER_LEVEL];
	};

	HolidayGuardLevelCfg() : cfg_count(0)
	{
	}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct HolidayGuardBuyJoinTimesCfg
{
	const static int CONFIG_ITEM_MAX_COUNT = 16;

	struct ConfigItem
	{
		ConfigItem() : buy_times(0), gold_cost(0) {}

		int buy_times;
		int gold_cost;
	};

	HolidayGuardBuyJoinTimesCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct HolidayGuardStarLevelCfg
{
	HolidayGuardStarLevelCfg() :death_count(0), star_level(0)
	{}

	short death_count;
	short star_level;
};

struct HolidayGuardBuffDropWeightCfg
{
	HolidayGuardBuffDropWeightCfg() : monster_id(0), total_weight(0)
	{
		memset(weight, 0, sizeof(weight));
	}

	int monster_id;
	int weight[HOLIDAYGUARD_BUFF_DROP_WEIGHT_COUNT];
	int total_weight;
};

enum HOLIDAY_GUARD_BUff_TYPE
{
	HOLIDAY_GUARD_BUFF_TYPE_INVALID = 0,

	HOLIDAY_GUARD_BUFF_TYPE_RECOVER,		// �ظ�����Ѫ��
	HOLIDAY_GUARD_BUFF_TYPE_ADD_GONGJI,		// ����������
	HOLIDAY_GUARD_BUFF_TYPE_KILL_MONSTER,	// ��ɱ�ѳ��ֵĹ���
	HOLIDAY_GUARD_BUFF_TYPE_ADD_HUDUN,		// ������ӻ���

	HOLIDAY_GUARD_BUFF_TYPE_MAX,
};

struct HolidayGuardBuffCfg
{
	HolidayGuardBuffCfg() : buff_type(0)
	{
		memset(param, 0, sizeof(param));
	}

	int buff_type;
	int param[4];
};

struct HolidayGuardNpcRefreshCfg
{
	HolidayGuardNpcRefreshCfg() : index(0), scene_id(0), npc_id(0), coordinate_x(0), coordinate_y(0) {}

	int index;
	int scene_id;
	int npc_id;
	int coordinate_x;
	int coordinate_y;
};

struct NpcSceneLimit
{
	NpcSceneLimit() : scene_id(0), limit_count(0) {}

	int scene_id;
	int limit_count;
};

class TiXmlElement;

class HolidayGuardConfig : public ConfigBase
{
public:
	HolidayGuardConfig();
	~HolidayGuardConfig();

	bool Init(const std::string &path, std::string *err);

	int GetRefreshStartDelay() const { return m_other_cfg.first_wave_refresh_delay; }
	int GetRefreshInterval() const { return m_other_cfg.wave_refresh_interval; }
	int GetLifeHolidayMonsterID() const { return m_other_cfg.life_tower_monster_id; }
	int GetWarningInterval() const { return m_other_cfg.warning_interval_s; }
	int GetSyncInterval() const { return m_other_cfg.sync_interval_s; }
	int GetBuffDropExitTime() const { return m_other_cfg.buff_exit_time; }

	int GetWaveCountBySceneID(int scene_id) const;
	const HolidayGuardWaveCfg * GetWaveCfg(int scene_id, int wave) const;
	const HolidayGuardWaveCfg * GetWaveCfgByMonsterID(int scene_id, int monster_id) const;

	Posi GetBirthPos(int path) const;
	bool GetNextPosi(int birth_path, const Posi &curr_pos, Posi *pos);

	bool CheckWarning(int old_percent, int now_percent, int *warning_percent);

	const HolidayGuardLevelCfg::ConfigItem * GetLevelCfgBySceneID(int scene_id) const;
	const HolidayGuardLevelCfg::ConfigItem * GetLevelCfg(int level) const;
	const HolidayGuardOtherCfg & GetOtherCfg() { return m_other_cfg; }

	HolidayGuardBuffDropWeightCfg * GetBuffDropCfg(int monster_id);
	HolidayGuardBuffCfg * GetBufffCfg(int buff_type);

	bool GetFBBeginTimeAndEndTime(unsigned int *begin_time, unsigned int *end_time);

	int GetNpcCfg(HolidayGuardNpcRefreshCfg *get_npc_cfg_list, int max_get_count);
	int GetNpcRefreshTime() { return m_other_cfg.npc_refresh_interval; }
	int GetNpcNeedRefreshCount() { return m_other_cfg.npc_refresh_count; }
	int GetNpcConfigCount() { return m_npc_cfg_count; }

	int GetLeaveFbReward(ItemConfigData *leave_fb_reward, int length);
	int GetEverydayLeaveFbRewardCount() { return m_other_cfg.everyday_can_fetch_reward_count; }

	std::unordered_map<int, int> & GetSceneNpcCountMap() { return m_scene_npc_count_map; }
	int GetTimeWithinTreadFinishRefreshTime() { return m_other_cfg.time_within_tread_finish_refresh; }

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitWaveCfg(PugiXmlNode RootElement);
	int InitPathCfg(PugiXmlNode RootElement);
	int InitBuffDropWeightCfg(PugiXmlNode RootElement);
	int InitBuffCfg(PugiXmlNode RootElement);
	int InitNpcCfg(PugiXmlNode RootElement);
	int InitNpcLimitCfg(PugiXmlNode RootElement);

	bool CheckLevelMonsterID(const HolidayGuardLevelCfg::ConfigItem &cfg_item);

	HolidayGuardOtherCfg m_other_cfg;									// ��������
	HolidayGuardLevelCfg m_level_cfg;									// �ؿ�����

	Posi wave_pathway_list[HOLIDAYGUARD_PATH_MAX][HOLIDAYGUARD_PATH_POINT_COUNT];

	int m_warning_percent_list[HOLIDAYGUARD_WARNING_PERCENT_COUNT];		// ������������Ѫ���ٷֱ��б�

	std::map<int, HolidayGuardBuffDropWeightCfg> m_buff_drop_weight_map;// buff��������
	std::map<int, HolidayGuardBuffCfg> m_buff_cfg_map;					// buff����

	int m_npc_cfg_count;
	HolidayGuardNpcRefreshCfg m_npc_cfg_list[HOLIDAYGUARD_NPC_CFG_MAX_COUNT];	// npc ����
	
	std::unordered_map<int, int> m_scene_npc_count_map;							// ����npc����
};

#endif
