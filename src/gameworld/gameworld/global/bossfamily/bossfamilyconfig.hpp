#ifndef _BOSSFAMILY_HPP_
#define _BOSSFAMILY_HPP_

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <map>
#include <set>
#include <vector>
#include "scene/scene.h"

static const int MAX_DABAO_BUY_COST_COUNT = 10;			// 打宝boss购买消耗最大条数
static const int MAX_BOSS_FAMILY_BOSS_RANK_CFG_COUNT = 10;	// boss之家排行配置个数
const static int YUNYOU_BOSS_MAX_NUM_PER_SCENE = 10;		// 云游boss每个场景最大数量
const static int YUNYOU_BOSS_MAX_SCENE_NUM = 20;			// 云游boss最大场景数量

struct BossFamilyOtherCfg
{
	BossFamilyOtherCfg():refresh_interval(0), weary_upper_limit(0), kill_add_weary(0),dabao_free_times(0), max_value(0), kill_boss_value(0),
		 kill_monster_value(0), leave_scene_id(0), leave_pos(0,0), dabao_kick_delay_time(0), active_kick_delay_time(0), active_max_weary(0),
		dabao_add_angry_interval_s(0), dabao_add_angry_value(0), dabao_angry_value_die(0), comeback_timeout_min(0)
	{}

	unsigned int refresh_interval;

	int weary_upper_limit;
	int kill_add_weary;

	int dabao_free_times;
	int max_value;
	int kill_boss_value;
	int kill_monster_value;
	int leave_scene_id;
	Posi leave_pos;
	unsigned int dabao_kick_delay_time;
	unsigned int active_kick_delay_time;
	int active_max_weary;

	int dabao_add_angry_interval_s;
	int dabao_add_angry_value;
	int dabao_angry_value_die;

	int comeback_timeout_min;
};

struct BossFamilyBossInfoCfg
{
	BossFamilyBossInfoCfg() :bossID(0), is_cross(0), scene_id(0), born_pos(0, 0), refresh_interval(0), min_lv(0), max_lv(0)
	{
	}

	int bossID;
	int is_cross;
	int scene_id;
	Posi born_pos;

	int refresh_interval;
	int min_lv;
	int max_lv;
};

struct BossFamilyMikuBossInfoCfg
{
	BossFamilyMikuBossInfoCfg() :bossID(0), scene_id(0), born_pos(0, 0), refresh_interval(0), min_lv(0), max_lv(0), is_peace(0)
	{
	}

	int bossID;
	int scene_id;
	Posi born_pos;

	int refresh_interval;
	int min_lv;
	int max_lv;
	int is_peace;			// 是否和平模式层
};

struct MikuMonsterInfoCfg
{
	MikuMonsterInfoCfg() : monster_id(0), scene_id(0), born_pos(0, 0),
		start_refresh_time(0), end_refresh_time(0), refresh_interval(0)
	{}

	int monster_id;
	int scene_id;
	Posi born_pos;
	int start_refresh_time;
	int end_refresh_time;
	int refresh_interval;
};

struct BossFamilyEnterConditionCfg
{
	BossFamilyEnterConditionCfg():scene_id(0), free_vip_level(0), enter_pos(0,0), need_item_id(0), need_item_num(0), cost_gold(0), min_lv(0), max_lv(0)
	{
	}
	
	int scene_id;
	short free_vip_level;
	Posi enter_pos;
	ItemID need_item_id;
	short need_item_num;
	int cost_gold;
	int min_lv;
	int max_lv;
};

struct DabaoBuyCostCfg
{
	DabaoBuyCostCfg() : times(0), cost_gold(0), consume_item_id(0), consume_item_num(0)
	{
	}

	int times;
	int cost_gold;
	ItemID consume_item_id;
	short consume_item_num;
};

struct DabaoBossInfoCfg
{
	DabaoBossInfoCfg() : bossID(0), scene_id(0), born_pos(0, 0),
	start_refresh_time(0), end_refresh_time(0), refresh_interval(0), kill_boss_value(0)
	{}

	int bossID;
	int scene_id;
	Posi born_pos;
	int start_refresh_time;
	int end_refresh_time;
	int refresh_interval;
	int kill_boss_value;
};

struct ActiveBossInfoCfg
{
	ActiveBossInfoCfg() : bossID(0), scene_id(0), born_pos(0, 0), add_weary(0), min_lv(0), max_lv(0){}

	int bossID;
	int scene_id;
	Posi born_pos;
	int add_weary;

	int min_lv;
	int max_lv;

	unsigned int refresh_interval;
};

struct BossSpecialRefreshCfg
{
	BossSpecialRefreshCfg() : boss_id(0), min_role_level(0), max_role_level(0), item_count(0) {}

	struct RefreshItem
	{
		RefreshItem() : role_num(0), flush_time_s(0) {}
		int role_num;
		int flush_time_s;
	};

	static const int ITEM_COUNT_MAX = 10;

	int boss_id;
	int min_role_level;
	int max_role_level;
	int item_count;
	RefreshItem item_list[ITEM_COUNT_MAX];
};

struct PreciousBossOtherCfg
{
	PreciousBossOtherCfg() : precious_boss_limit_level(0), precious_boss_scene_id(0), team_member_reward_precent(0), boss_type_count(0),
		monster_type_count(0), gather_refresh_interval(0), gather_id(0), gather_cost_time(0), gather_pos_start_seq(-1), gather_pos_end_seq(0),
		monster_pos_start_seq(-1), monster_pos_end_seq(0), boss_pos_start_seq(-1), boss_pos_end_seq(0), forbid_pk_pos(0, 0), forbid_pk_radius(0),
		gather_num(0), skip_task_consume(0), skip_task_limit_level(0)
	{}

	short precious_boss_limit_level;
	short precious_boss_scene_id;

	short team_member_reward_precent;

	short boss_type_count;
	short monster_type_count;

	short gather_refresh_interval;
	int gather_id;
	int gather_cost_time;
	short gather_pos_start_seq;
	short gather_pos_end_seq;

	short monster_pos_start_seq;
	short monster_pos_end_seq;

	short boss_pos_start_seq;
	short boss_pos_end_seq;

	Posi forbid_pk_pos;
	int forbid_pk_radius;

	int gather_num;

	int skip_task_consume;
	int skip_task_limit_level;
};

struct PreciousMonsterCfg
{
	PreciousMonsterCfg() : monster_type(-1), monster_id(0), seq(0), refresh_interval(0), refresh_max_count(0)
	{}

	enum PRECIOUS_MONSTER_TYPE
	{
		PRECIOUS_MONSTER_TYPE_BOSS = 0,
		PRECIOUS_MONSTER_TYPE_COMMON_MONSTER,
		PRECIOUS_MONSTER_TYPE_GREAT_MONSTER,

		PRECIOUS_MONSTER_TYPE_MAX_TYPE,
	};

	int monster_type;
	int monster_id;
	int seq;

	unsigned int refresh_interval;
	int refresh_max_count;
};

struct PreciousMonsterWorldLevelCfg
{
	PreciousMonsterWorldLevelCfg() : world_level(0)
	{}

	int world_level;
	std::vector<PreciousMonsterCfg> world_level_cfg_vec;
};

struct PreciousBossTaskCfg
{
	PreciousBossTaskCfg() : task_id(0), task_type(-1), task_param(0), task_condition(0)
	{}

	enum PRECIOUS_BOSS_TASK_TYPE
	{
		PRECIOUS_BOSS_TASK_TYPE_BOSS = 0,
		PRECIOUS_BOSS_TASK_TYPE_COMMON_MONSTER,
		PRECIOUS_BOSS_TASK_TYPE_GATHER,
		PRECIOUS_BOSS_TASK_TYPE_GREAT_MONSTER,

		PRECIOUS_BOSS_TASK_TYPE_MAX,
	};

	int task_id;
	int task_type;
	int task_param;
	int task_condition;
};

struct PreciousBossRewardCfg
{
	PreciousBossRewardCfg() : reward_type(-1), reward_param(0), reward_score(0)
	{}

	enum PRECIOUS_BOSS_REWARD_TYPE
	{
		PRECIOUS_BOSS_REWARD_TYPE_TASK = 0,
		PRECIOUS_BOSS_REWARD_TYPE_KILL_MONSTER,
		PRECIOUS_BOSS_REWARD_TYPE_GATHER,

		PRECIOUS_BOSS_REWARD_TYPE_MAX,
	};

	int reward_type;
	int reward_param;
	int reward_score;
};

struct PreciousBossRewardLevelCfg
{
	PreciousBossRewardLevelCfg() : level(0)
	{}

	int level;
	std::map<int, PreciousBossRewardCfg> reward_cfg_map;
};

struct PreciousBossRefreshPosCfg
{
	PreciousBossRefreshPosCfg() : pos_type(-1), pos(0,0)
	{}

	enum PRECIOUS_BOSS_POS_TYPE
	{
		PRECIOUS_BOSS_POS_TYPE_BOSS = 0,
		PRECIOUS_BOSS_POS_TYPE_COMMON_MONSTER,
		PRECIOUS_BOSS_POS_TYPE_GATHER,
		PRECIOUS_BOSS_POS_TYPE_GREAT_MONSTER,

		PRECIOUS_BOSS_POS_TYPE_MAX
	};

	int pos_type;
	Posi pos;
};


struct WearyCfg
{
	WearyCfg() :weary_tpye(0), weary_limit(0), kill_add_weary(0), buy_give_weary(0) {}
	int weary_tpye;
	int weary_limit;
	int kill_add_weary;
	int buy_give_weary;
};

struct WearyCostCfg
{
	WearyCostCfg() :buy_times(0), cost(0), cost_tpye(0) {}
	int buy_times;
	int cost;
	int cost_tpye;
};

struct BossFamilyBossHurtRankCfg
{
	BossFamilyBossHurtRankCfg() : monster_id(0), rank_cfg_count(0) {}

	struct RankItem
	{
		RankItem() : rank(0)
		{
			memset(item_count_list, 0, sizeof(item_count_list));
		}

		int rank;
		int item_count_list[PROF_TYPE_PROF_NO_LIMIT];
		ItemConfigData item_list[PROF_TYPE_PROF_NO_LIMIT][MAX_ATTACHMENT_ITEM_NUM];
	};

	int monster_id;
	int rank_cfg_count;
	RankItem rank_item_list[MAX_BOSS_FAMILY_BOSS_RANK_CFG_COUNT];
};

// 云游boss配置
struct YunyouBossBornCfg
{
	int scene_id = 0;
	int boss_id = 0;

	PointConfig born_pos;
};


class BossFamilyConfig : public ConfigBase
{
public:
	BossFamilyConfig();
	~BossFamilyConfig();

	bool Init(const std::string &configname, std::string *err);

	const BossFamilyOtherCfg& GetBossFamilyOtherCfg(){return m_other_cfg;}

	const std::map<int, BossFamilyBossInfoCfg> & GetBossFamilyBossMap(){return m_bossfamily_boss_map;}
	const std::map<int, BossFamilyMikuBossInfoCfg> & GetBossFamilyMikuBossMap(){return m_miku_boss_map;}
	const std::map<int, MikuMonsterInfoCfg> & GetMikuMonsterMap() { return m_miku_monster_map; }
	const std::map<int, DabaoBossInfoCfg> & GetDabaoBossMap(){return m_dabao_boss_map;}
	const std::map<int, ActiveBossInfoCfg> & GetActiveBossMap(){return m_active_boss_map;}
	const std::set<int> & GetFamilyBossSceneIDSet() {return m_family_boss_scene_id_set;}
	const std::set<int> & GetMikuBossSceneIDSet() {return m_miku_boss_scene_id_set;}
	const std::set<int> & GetDaobaoBossSceneIDSet() {return m_dabao_boss_scene_id_set;}
	const std::set<int> & GetActiveBossSceneIDSet() {return m_active_boss_scene_id_set;}
	const std::vector<PreciousMonsterCfg> & GetPreciousMonsterWorldLevelCfg();
	const std::map<int, PreciousBossRewardCfg> & GetPreciousBossRewardCfg(int level);

	const BossFamilyBossInfoCfg * GetBossFamilyBossInfo(int boss_id);
	const BossFamilyMikuBossInfoCfg * GetMikuBossInfo(int boss_id);
	const BossFamilyEnterConditionCfg * GetEnterConditionInfo(Role *role, int scene_id);
	int GetDabaoBossBuyCountCost(int today_buy_count);
	const DabaoBossInfoCfg * GetDabaoBossInfo(int boss_id);
	const DabaoBuyCostCfg * GetDabaoBuyCostCfg(int times);
	const ActiveBossInfoCfg * GetActiveBossInfo(int boss_id);
	const MikuMonsterInfoCfg * GetMikuMonsterInfo(int monster_id);

	const std::map<int, BossSpecialRefreshCfg> & GetBossSpecialRefreshCfg() { return m_boss_special_refresh_cfg_map; }
	const BossSpecialRefreshCfg* GetBossSpecialRefreshCfgByBossId(int boss_id);

	const PreciousBossOtherCfg & GetPreciousBossOtherCfg() { return m_precious_boss_other_cfg; }
	std::map<int, std::vector<Posi> > & GetPreciousBossPosCfg() { return m_precious_boss_pos_cfg; }
	const PreciousBossTaskCfg * GetPreciousBossTaskCfg(int task_param);

	const WearyCfg * GetWearyCfg(int weary_tpye);
	const WearyCostCfg * GetBuyWearyCostCfg(int buy_times);

	const BossFamilyBossHurtRankCfg::RankItem *GetBossHurtRankCfg(int monster_id, int rank);

	bool IsCrossVipBossScene(int scene_id);

	// 云游boss-----------------------------
	const std::vector<YunyouBossBornCfg> & GetYunyouBossAllBornCfg() { return m_yunyou_boss_vec; }
	bool GetNextRefreshTimeOfYunyouBoss(unsigned int *next_refresh_time) const;
	bool IsYunyouBoss(int boss_id);

private:

	int InitBossFamilyOtherCfg(PugiXmlNode RootElement);
	int InitBossFamilyBossCfg(PugiXmlNode RootElement);
	int InitBossFamilyMikuBossCfg(PugiXmlNode RootElement);
	int InitBossFamilyEnterCfg(PugiXmlNode RootElement);
	int InitDabaoBuyCostCfg(PugiXmlNode RootElement);
	int InitDabaoBossCfg(PugiXmlNode RootElement);
	int InitActiveBossCfg(PugiXmlNode RootElement);
	int InitMikuMonsterCfg(PugiXmlNode RootElement);
	int InitBossSpecialRefreshCfg(PugiXmlNode RootElement);

	int InitPreciousBossOtherCfg(PugiXmlNode RootElement);
	int InitPreciousMonsterCfg(PugiXmlNode RootElement);
	int InitPreciousBossTaskCfg(PugiXmlNode RootElement);
	int InitPreciousBossRewardCfg(PugiXmlNode RootElement);
	int InitPreciousBossPosCfg(PugiXmlNode RootElement);

	int InitWearyCfg(PugiXmlNode RootElement);
	int InitWearyCostCfg(PugiXmlNode RootElement);
	int InitBossHurtRankCfg(PugiXmlNode RootElement);

	BossFamilyOtherCfg m_other_cfg;

	std::map<int, BossFamilyBossInfoCfg> m_bossfamily_boss_map;		// boss之家
	std::map<int, BossFamilyMikuBossInfoCfg> m_miku_boss_map;		// 秘窟boss
	std::map<int, MikuMonsterInfoCfg> m_miku_monster_map;			// 秘窟小怪
	std::map<int, BossFamilyEnterConditionCfg> m_enter_conditon_map;
	std::map<int, DabaoBossInfoCfg> m_dabao_boss_map;				// 打宝boss
	std::map<int, ActiveBossInfoCfg> m_active_boss_map;				// 活跃boss
	std::set<int> m_family_boss_scene_id_set;						// boss之家场景id
	std::set<int> m_miku_boss_scene_id_set;							// 秘窟boss场景id
	std::set<int> m_dabao_boss_scene_id_set;						// 打宝boss场景id
	std::set<int> m_active_boss_scene_id_set;						// 活跃boss场景id

	int m_dabao_buy_cost_count;
	DabaoBuyCostCfg m_dabao_buy_cost_list[MAX_DABAO_BUY_COST_COUNT];

	std::map<int, BossSpecialRefreshCfg> m_boss_special_refresh_cfg_map;

	PreciousBossOtherCfg m_precious_boss_other_cfg;
	std::vector<PreciousMonsterWorldLevelCfg> m_precious_monster_world_level_cfg_vec;
	std::map<int, PreciousBossTaskCfg> m_precious_boss_task_cfg_map;
	std::vector<PreciousBossRewardLevelCfg> m_precious_boss_reward_level_cfg_vec;

	std::map<int, std::vector<Posi> > m_precious_boss_pos_cfg;
	std::vector<Posi> m_precious_boss_refresh_pos_cfg_vec;
	std::vector<Posi> m_precious_monster_refresh_pos_cfg_vec;
	std::vector<Posi> m_precious_great_monster_refresh_pos_cfg_vec;
	std::vector<Posi> m_precious_gather_refresh_pos_cfg_vec;

	std::map<int, WearyCfg> m_weary_map;
	std::map<int, WearyCostCfg> m_buy_weary_cost_map;

	typedef std::map<int, BossFamilyBossHurtRankCfg> BossFamilyBossHurtRankCfgMap;
	typedef std::map<int, BossFamilyBossHurtRankCfg>::iterator BossFamilyBossHurtRankCfgMapIt;
	BossFamilyBossHurtRankCfgMap m_boss_hurt_rank_cfg_map;

	// 云游boss-----------------------------
	int InitYunyouBossCfg(PugiXmlNode RootElement);
	int InitYunyouBossRefreshCfg(PugiXmlNode RootElement);

	std::vector<YunyouBossBornCfg> m_yunyou_boss_vec;
	std::vector<int> m_yunyou_refresh_time_list;

	//-------------------------------------
};

#endif
