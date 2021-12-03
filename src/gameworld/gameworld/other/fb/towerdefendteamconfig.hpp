#ifndef _TOWERDEFENDTEAMCONFIG_HPP_
#define _TOWERDEFENDTEAMCONFIG_HPP_

#include "gamedef.h"
#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "towerdefendteamskill.hpp"
#include "servercommon/fbdef.hpp"
#include <map>
#include <set>

#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "monster/drop/droppool.hpp"

static const int TEAM_TOWERDEFEND_PATH_POINT_COUNT = 4;				// ��Ӹ���·���յ�����
static const int TEAM_TOWERDEFEND_NORMAL_SKILL_COUNT = 4;			// Ĭ�ϼ�������		
static const int TEAM_TOWERDEFEND_EXTRA_SKILL_COUNT = 0;			// ���⼼������
static const int TEAM_TOWERDEFEND_FROZEN_MAX_COUNT = 0;				// ����������
static const int TEAN_TOWERDEFEND_WAVE_COUNT_PER_LEVEL = 10;		// �������ÿ�������
static const int TEAM_TOWERDEFEND_WAVE_COUNT = 80;					// �����
static const int TEAM_TOWERDEFEND_HOME_REALIVE_LIMIT_INTERVAL = 0;	// ��Ѹ�������ʱ���� ����
static const int TEAM_TOWERDEFEND_GOLD_REALIVE_LIMIT_INTERVAL = 0;	// Ԫ����������ʱ���� ����
static const int TEAM_TOWERDEFEND_MAX_SKILL_LEVEL = 30;				// �������ȼ�
static const int TEAM_TOWERDEFEND_MAX_SKILL_COUNT = 12;				// ���������
static const int TEAM_TOWERDEFEND_WARNING_PERCENT_COUNT = 2;		// ������������Ѫ���ٷֱȷ�ֵ

enum TEAM_TOWERDEFEND_PATH											// ˢ��·��
{
	TEAM_TOWERDEFEND_PATH_INVALID = -1,
	TEAM_TOWERDEFEND_PATH_LEFT,											// ��
	TEAM_TOWERDEFEND_PATH_MID,											// ��
	TEAM_TOWERDEFEND_PATH_RIGHT,										// ��

	TEAM_TOWERDEFEND_PATH_MAX,
};

enum TEAM_TOWERDEFEND_WARNING_PERCENT
{
	TEAM_TOWERDEFEND_WARNING_PERCENT_50 = 50,
	TEAM_TOWERDEFEND_WARNING_PERCENT_10 = 10,
};

enum TEAM_TOWERDEFEND_ATTRTYPE											// �ӳ�����
{
	TEAM_TOWERDEFEND_ATTRTYPE_INVALID = 0,
	TEAM_TOWERDEFEND_ATTRTYPE_GONGJI,									// �ӹ� ��ȸ
	TEAM_TOWERDEFEND_ATTRTYPE_FANGYU,									// �ӷ� ����
	TEAM_TOWERDEFEND_ATTRTYPE_ASSIST,									// ���� ����

	TEAM_TOWERDEFEND_ATTRTYPE_MAX,
};

struct TeamTowerDefendWaveCfg
{
	TeamTowerDefendWaveCfg() : wave(0), next_wave_s(0), total_monster_count(0), kill_score(0),sweep_calue(0)
	{
		memset(monster_id_list, 0, sizeof(monster_id_list));
		memset(monster_count_list, 0, sizeof(monster_count_list));
	}

	int wave;
	int next_wave_s;													// ��ô���������һ��
	int monster_id_list[TEAM_TOWERDEFEND_PATH_MAX];						// ÿ��·�߳��Ĺ�ID
	int monster_count_list[TEAM_TOWERDEFEND_PATH_MAX];					// ÿ��·�߳��Ĺ�����
	int total_monster_count;											// ������
	int kill_score;														// ��ɱ����ӻ���

	std::vector<UInt16> dropid_list;									// ����ID�б�
	int sweep_calue;													// ɨ������ֵ
};

struct TeamTowerDefendLevelCfg
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
		TeamTowerDefendWaveCfg wave_list[TEAN_TOWERDEFEND_WAVE_COUNT_PER_LEVEL];
	};

	TeamTowerDefendLevelCfg() : cfg_count(0)
	{
	}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct TeamTowerDefendOtherCfg
{
	TeamTowerDefendOtherCfg() : free_join_times(0), auto_fb_empty_grid(0), auto_item_id(0), auto_item_count(0), team_enter_sceneid(0), team_enter_pos(0, 0), team_wave_count(0),
		buy_double_reward_need_gold(0), buy_triple_reward_need_gold(0), double_reward_need_vip_level(0), triple_reward_need_vip_level(0)
	{

	}

	int free_join_times;
	int auto_fb_empty_grid;
	ItemID auto_item_id;
	int auto_item_count;

	int team_enter_sceneid;
	Posi team_enter_pos;

	int team_wave_count;
	int buy_double_reward_need_gold;
	int buy_triple_reward_need_gold;
	int double_reward_need_vip_level;
	int triple_reward_need_vip_level;
	TeamTowerDefendWaveCfg team_wave_list[TEAM_TOWERDEFEND_WAVE_COUNT];

	std::vector<UInt16> dropid_list;									// ���⽱��
};

struct TeamTowerDefendSkillCfg
{
	static const int CONFIG_ITEM_MAX_COUNT = TEAM_TOWERDEFEND_MAX_SKILL_COUNT;

	struct ConfigItem
	{
		ConfigItem() : skill_index(0), attr_type(0), skill_seq(0), skill_id(0), hurt_percent(0), fix_hurt(0), cd_factor(0), cd_s(0), distance(0), energy_cost(0), param_a(0), param_b(0), param_c(0), param_d(0)
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
		int param_d;
	};

	TeamTowerDefendSkillCfg()
	{

	}

	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct TeamTowerDefendBuyJoinTimesCfg
{
	const static int CONFIG_ITEM_MAX_COUNT = 16;

	struct ConfigItem
	{
		ConfigItem() : buy_times(0), gold_cost(0) {}

		int buy_times;
		int gold_cost;
	};

	TeamTowerDefendBuyJoinTimesCfg() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

class TeamTowerDefendConfig : public ConfigBase
{
public:
	TeamTowerDefendConfig();
	~TeamTowerDefendConfig();

	bool Init(const std::string &configname, std::string *err);

	int GetRefreshStartDelay() const { return m_wave_refresh_start_delay; }
	int GetRefreshInterval() const { return m_wave_refresh_interval; }

	int GetWaveCountBySceneID(int scene_id) const;
	const TeamTowerDefendWaveCfg * GetWaveCfg(int scene_id, int wave) const;
	const TeamTowerDefendWaveCfg * GetWaveCfgByMonsterID(int scene_id, int monster_id) const;

	int GetTeamDefendWaveCount() { return m_other_cfg.team_wave_count; }
	const TeamTowerDefendWaveCfg * GetTeamWaveCfg(int wave) const;
	const TeamTowerDefendWaveCfg * GetTeamWaveCfgByMonsterID(int monster_id) const;
	int GetMonsterAngryValue(int monster_id);
	Posi GetBirthPos(int path) const;

	int GetLifeTowerMonsterID() const { return m_life_tower_monster_id; }
	int GetTeamLifeTowerMonsterID() const { return m_team_life_tower_monster_id; }
	bool GetNextPosi(int birth_path, const Posi &curr_pos, Posi *pos);

	bool CheckWarning(int old_percent, int now_percent, int *warning_percent);

	int GetWarningInterval() const { return m_warning_interval_s; }
	int GetSyncInterval() const { return m_sync_interval_s; }

	const TeamTowerDefendLevelCfg::ConfigItem * GetLevelCfgBySceneID(int scene_id) const;
	const TeamTowerDefendLevelCfg::ConfigItem * GetLevelCfg(int level) const;
	const TeamTowerDefendOtherCfg & GetOtherCfg() { return m_other_cfg; }
	const TeamTowerDefendSkillCfg::ConfigItem * GetSkillCfg(int skill_index);
	int GetBuyJoinTimesCfg(int buy_times);

	const TeamTowerDefendSkillCfg::ConfigItem * GetTeamSkillCfg(int skill_id);
	void GetTeamSkillListByAttr(int attr_type, int skill_id_list[TEAM_TOWERDEFEND_NORMAL_SKILL_COUNT]) const;
	int GetUpTeamSkillCost(int skill_id, int skill_level) const;
	const TeamTowerDefendSkill * GetTeamSkillBase(int skill_id) const;


private:
	int InitTeamWaveCfg(PugiXmlNode RootElement);
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitPathCfg(PugiXmlNode RootElement);
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);
	int InitBuyJoinTimesCfg(PugiXmlNode RootElement);
	int InitSkillLearnCfg(PugiXmlNode RootElement);

	bool CheckLevelMonsterID(const TeamTowerDefendLevelCfg::ConfigItem &cfg_item);

	int m_wave_refresh_start_delay;										// ��һ�����ӳ���
	int m_wave_refresh_interval;										// ���ּ��

	Posi wave_pathway_list[TEAM_TOWERDEFEND_PATH_MAX][TEAM_TOWERDEFEND_PATH_POINT_COUNT];

	int m_life_tower_monster_id;										// ��������ID
	int m_team_life_tower_monster_id;									// ������� ��������ID

	int m_warning_interval_s;											// ����������ı������
	int m_warning_percent_list[TEAM_TOWERDEFEND_WARNING_PERCENT_COUNT];	// ������������Ѫ���ٷֱ��б�

	int m_sync_interval_s;												// ��ʱͬ����Ϣ�ļ��

	TeamTowerDefendLevelCfg m_level_cfg;									// �ؿ�����
	TeamTowerDefendOtherCfg m_other_cfg;									// ��������
	TeamTowerDefendSkillCfg m_skill_cfg;									// ��������
	TeamTowerDefendBuyJoinTimesCfg m_buy_join_times_cfg;					// ��������������

	int m_skill_up_cost_list[TEAM_TOWERDEFEND_MAX_SKILL_COUNT][TEAM_TOWERDEFEND_MAX_SKILL_LEVEL + 1];	// ���������б�
	TeamTowerDefendSkill m_team_skill_base_list[TEAM_TOWERDEFEND_MAX_SKILL_COUNT+1];

	std::map<int, int> m_monster_angry_value;								//����ֵ��Ӧ��

};

#endif
