#ifndef _ARMORDEFENDCONFIG_HPP_
#define _ARMORDEFENDCONFIG_HPP_

#include "gamedef.h"
#include "servercommon/servercommon.h"
#include "towerdefendteamskill.hpp"
#include "servercommon/fbdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/maildef.hpp"
#include <map>
#include <set>

static const int ARMORDEFEND_PATH_POINT_MAX = 20;					// ·���ϵĹյ�����
static const int ARMORDEFEND_HOME_REALIVE_LIMIT_INTERVAL = 0;		// ��Ѹ�������ʱ���� ����
static const int ARMORDEFEND_GOLD_REALIVE_LIMIT_INTERVAL = 0;		// Ԫ����������ʱ���� ����
static const int PERSONAL_ARMORDEFEND_MAX_WAVE_PER_LEVEL = 10;		// ��������ÿ�������
static const int ARMORDEFEND_WARNING_PERCENT_COUNT = 2;				// ������������Ѫ���ٷֱȷ�ֵ
static const int PERSONAL_ARMORDEFEND_SKILL_MAX_COUNT = 2;			// ���������������

enum ARMORDEFEND_PATH												// ˢ��·��
{
	ARMORDEFEND_PATH_INVALID = -1,
	ARMORDEFEND_PATH_LEFT,											// ��
	ARMORDEFEND_PATH_MID,											// ��
	ARMORDEFEND_PATH_RIGHT,											// ��

	ARMORDEFEND_PATH_MAX,
};

enum ARMORDEFEND_WARNING_PERCENT
{
	ARMORDEFEND_WARNING_PERCENT_50 = 50,
	ARMORDEFEND_WARNING_PERCENT_10 = 10,
};

// ���ֲ�������
struct NewPlayerArmorDefendWaveCfg
{
	NewPlayerArmorDefendWaveCfg() : wave(0), next_wave_s(0), total_monster_count(0), kill_score(0)
	{
		memset(monster_id_list, 0, sizeof(monster_id_list));
		memset(monster_count_list, 0, sizeof(monster_count_list));
	}

	int wave;
	int next_wave_s;													// ��ô���������һ��
	int monster_id_list[ARMORDEFEND_PATH_MAX];							// ÿ��·�߳��Ĺ�ID
	int monster_count_list[ARMORDEFEND_PATH_MAX];						// ÿ��·�߳��Ĺ�����
	int total_monster_count;											// ������
	int kill_score;														// ��ɱ����ӻ���
};

struct ArmorDefendWaveCfg
{
	ArmorDefendWaveCfg() : wave(0), next_wave_s(0), total_monster_count(0), kill_score(0)
	{
		memset(monster_id_list, 0, sizeof(monster_id_list));
		memset(monster_count_list, 0, sizeof(monster_count_list));
	}

	int wave;
	int next_wave_s;													// ��ô���������һ��
	int monster_id_list[ARMORDEFEND_PATH_MAX];							// ÿ��·�߳��Ĺ�ID
	int monster_count_list[ARMORDEFEND_PATH_MAX];						// ÿ��·�߳��Ĺ�����
	int total_monster_count;											// ������
	int kill_score;														// ��ɱ����ӻ���
};

struct ArmorDefendPassLevelCfg
{
	ArmorDefendPassLevelCfg() : req_wave(0), req_passtime_s(0) {}

	int req_wave;
	int req_passtime_s;
};

struct ArmorDefendLevelCfg
{
	static const int CONFIG_ITEM_MAX_COUNT = 30;
	UNSTD_STATIC_CHECK(CONFIG_ITEM_MAX_COUNT < 128);

	struct ConfigItem
	{
		ConfigItem() : level(0), role_level(0), scene_id(0), birth_pos(0, 0), wave_count(0), new_player_wave_count(0)
		{
			
		}

		int level;
		int role_level;
		int scene_id;
		Posi birth_pos;

		int wave_count;					
		ArmorDefendWaveCfg wave_list[PERSONAL_ARMORDEFEND_MAX_WAVE_PER_LEVEL];
		int new_player_wave_count;
		ArmorDefendWaveCfg new_player_wave_list[PERSONAL_ARMORDEFEND_MAX_WAVE_PER_LEVEL];
		ItemConfigData find_reward[MAX_ATTACHMENT_ITEM_NUM];
	};

	ArmorDefendLevelCfg() : cfg_count(0)
	{
	}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct ArmorDefendOtherCfg
{
	ArmorDefendOtherCfg() : free_join_times(0), auto_fb_empty_grid(0), auto_item_id(0), auto_item_count(0),
		escape_num_to_failure(0), boss_num_to_failure(0),escape_num_to_warn(0), guid_task_id(0), reward_count(0)
	{

	}

	int free_join_times;
	int auto_fb_empty_grid;
	ItemID auto_item_id;
	int auto_item_count;

	int escape_num_to_failure;
	int boss_num_to_failure;
	int escape_num_to_warn;

	int guid_task_id;
	int reward_count;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct ArmorDefendSkillCfg
{
	static const int CONFIG_ITEM_MAX_COUNT = PERSONAL_ARMORDEFEND_SKILL_MAX_COUNT ;

	struct ConfigItem
	{
		ConfigItem() : skill_index(0), attr_type(0), skill_seq(0), skill_id(0), hurt_percent(0), fix_hurt(0), cd_factor(0), cd_s(0), distance(0), energy_cost(0), param_a(0), param_b(0), param_c(0)
		{

		}

		bool Invalid() const { return 0 == skill_index; }

		int skill_index;
		int attr_type;
		int skill_seq;
		int skill_id;
		int hurt_percent;
		int fix_hurt;
		int cd_factor;
		int cd_s;
		int distance;
		int energy_cost;
		int param_a;
		int param_b;
		int param_c;
	};

	ArmorDefendSkillCfg()
	{

	}

	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct ArmorDefendMonsterPath
{
	ArmorDefendMonsterPath() : cfg_count(0) {}

	int cfg_count;
	Posi pos_list[ARMORDEFEND_PATH_POINT_MAX];
};

struct ArmorDefendBuyJoinTimesCfg
{
	const static int CONFIG_ITEM_MAX_COUNT = 16;

	struct ConfigItem
	{
		ConfigItem() : buy_times(0), gold_cost(0) {}

		int buy_times;
		int gold_cost;
	};

	ArmorDefendBuyJoinTimesCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};


class ArmorDefendConfig: public ConfigBase
{
public:
	ArmorDefendConfig();
	~ArmorDefendConfig();

	bool Init(const std::string &configname, std::string *err);

	int GetNewPlayerTaskId() { return m_other_cfg.guid_task_id; }

	int GetRefreshStartDelay() const { return m_wave_refresh_start_delay; }
	int GetRefreshInterval() const { return m_wave_refresh_interval; }

	int GetEscapeNumToFail()const { return m_other_cfg.escape_num_to_failure; }
	int GetEscapeNumToWarning()const { return m_other_cfg.escape_num_to_warn; }
	int GetWaveCountBySceneID(int scene_id, bool is_new_player) const;
	const ArmorDefendWaveCfg * GetWaveCfg(int scene_id, int wave, bool is_new_player) const;
	const ArmorDefendWaveCfg * GetWaveCfgByMonsterID(int scene_id, int monster_id, bool is_new_player) const;
	const ArmorDefendMonsterPath* GetArmorDefendPathByMonsterID(int scene_id, int monster_id, bool is_new_player) const;

	Posi GetBirthPos(int path) const;
	int GetArmorDefendPathPosCount(int path) const;
	Posi GetArmorDefendPathPos(int path, int pos_idx) ;
	

	int GetLifeTowerMonsterID() const { return m_life_tower_monster_id; }
	
	bool CheckWarning(int old_percent, int now_percent, int *warning_percent);

	int GetWarningInterval() const { return m_warning_interval_s; }
	int GetSyncInterval() const { return m_sync_interval_s; }

	const ArmorDefendLevelCfg::ConfigItem * GetLevelCfgBySceneID(int scene_id) const;
	const ArmorDefendLevelCfg::ConfigItem * GetLevelCfg(int level) const;
	const ArmorDefendOtherCfg & GetOtherCfg() { return m_other_cfg; }
	const ArmorDefendSkillCfg::ConfigItem * GetSkillCfg(int skill_index);
	int GetBuyJoinTimesCfg(int buy_times);
	int GetEnergyByMonsterId(int monster_id);

private:
	int InitNewPlayerWaveCfg(PugiXmlNode RootElement);
	int InitWaveCfg(PugiXmlNode RootElement);
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitPathCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);
	int InitBuyJoinTimesCfg(PugiXmlNode RootElement);
	int InitGetEnergyCfg(PugiXmlNode RootElement);

	bool CheckLevelMonsterID(const ArmorDefendLevelCfg::ConfigItem &cfg_item);

	int m_wave_refresh_start_delay;										// ��һ�����ӳ���
	int m_wave_refresh_interval;										// ���ּ��

	ArmorDefendMonsterPath wave_pathway_list[ARMORDEFEND_PATH_MAX];

	int m_life_tower_monster_id;										// ��������ID
	int m_team_life_tower_monster_id;									// ������� ��������ID

	int m_warning_interval_s;											// ����������ı������
	int m_warning_percent_list[ARMORDEFEND_WARNING_PERCENT_COUNT];		// ������������Ѫ���ٷֱ��б�

	int m_sync_interval_s;												// ��ʱͬ����Ϣ�ļ��

	ArmorDefendLevelCfg m_level_cfg;									// �ؿ�����
	ArmorDefendOtherCfg m_other_cfg;									// ��������
	ArmorDefendSkillCfg m_skill_cfg;									// ��������
	ArmorDefendBuyJoinTimesCfg m_buy_join_times_cfg;					// ��������������

	std::map<int,int> m_get_energy;									//������ȡ
};

#endif
