#ifndef _ZHUAGUI_CONFIG_HPP_
#define _ZHUAGUI_CONFIG_HPP_

#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

#include <vector>

class TiXmlElement;

static const int ZHUAGUI_SCENE_COUNT = 30;
static const int ZHUAGUI_SCENE_POS_COUNT = 40;

struct ZhuaGuiNpcSceneCfg
{
	ZhuaGuiNpcSceneCfg() : scene_id(0), npc_count(0), flush_min_count(0), pos_count(0)
	{
		memset(pos_list, 0, sizeof(pos_list));
	}

	int scene_id;
	int npc_count;
	int flush_min_count;
	
	int pos_count;
	Posi pos_list[ZHUAGUI_SCENE_POS_COUNT];
};

static const int ZHUAGUI_MONSTER_FLUSH_COUNT = 96;

struct ZhuaGuiMonsterFlushCfg
{
	ZhuaGuiMonsterFlushCfg() : monster_id(0), flush_pos(0, 0), flush_num(0), flush_range(0), flush_prob(0), is_boss(0), give_hunli(0),
		team_level_min(0), team_level_max(0)
	{
	}

	bool IsBoss() { return 0 != is_boss; }

	int monster_id;

	Posi flush_pos;
	int flush_num;
	int flush_range;

	int flush_prob;
	
	int is_boss;
	int give_hunli;
	int team_level_min;
	int team_level_max;
	std::vector<UInt16> drop_id_list;
	std::vector<UInt16> couple_drop_id_list;
};

static const int ZHUAGUI_REWARD_DEC_COUNT = 5;

struct ZhuaGuiRewardDecCfg
{
	ZhuaGuiRewardDecCfg() : kill_monster(0), reward_per(0)
	{
	}

	int kill_monster;
	int reward_per;
};

struct ZhuaGuiTeamAdd
{
	ZhuaGuiTeamAdd():team_player_num(0), hunli_add_per(0), add_boss_per(0)
	{}

	int team_player_num;
	int hunli_add_per;
	int add_boss_per;
};

class ZhuaGuiConfig : public ConfigBase
{
public:
	ZhuaGuiConfig();
	~ZhuaGuiConfig();

	bool Init(const std::string &configname, std::string *err);

	ZhuaGuiNpcSceneCfg * GetZhuaGuiNpcSceneCfgByIdx(int idx); 
	ZhuaGuiNpcSceneCfg * GetZhuaGuiNpcSceneCfgBySceneId(int scene_id);
	bool IsNeedFlush(int scene_id, int curr_count);
	bool GetRandomFlushInfo(int scene_id, int *npc_count, Posi randpos_list[ZHUAGUI_SCENE_POS_COUNT]);
	bool CanEnterZhuaguFb(int role_lv) { return role_lv >= m_limit_lv; }
	bool GetEnterFbInfo(int *scene_id, Posi *enter_pos);
	bool IsBoss(int monster_id);

	const int GetCoupleHunliAddPer(){return m_married_hunli_add_per;}
	const int GetCoupleBossAddPer(){return m_married_add_boss_per;}
	const unsigned int GetKickDelayTime(){return m_kick_delay_time;}
	int GetBossDropLimit() { return m_drop_time_limit; }
	int GetBossCoupleDropLimit() { return m_couple_drop_time_limit; }

	int GetMojingRewardTime(){return m_mojing_reward_time;}
	int GetMojingReward(){return m_mojing_reward;}
	const ZhuaGuiTeamAdd * GetTeamAddPer(int team_num);
	
	ZhuaGuiMonsterFlushCfg * GetFlushMonsterInfoByIdx(int idx);
	ZhuaGuiMonsterFlushCfg * GetFlushMonsterInfoByMonsterId(int monster_id);
	int GetHunliByMonsterID(int monset_id);
	int GetHunliFix(int fetch_guicount, int give_huoli, int team_player_num, bool is_has_couple);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitNpcFlushCfg(PugiXmlNode RootElement);
	int InitNpcPosCfg(PugiXmlNode RootElement);
	int InitMonsterCfg(PugiXmlNode RootElement);
	int InitRewardDecCfg(PugiXmlNode RootElement);
	int InitTeamAddCfg(PugiXmlNode RootElement);

	int m_limit_lv;

	int m_scene_id;
	Posi m_enter_pos;
	int m_married_hunli_add_per;
	int m_married_add_boss_per;
	int m_drop_time_limit;
	int m_couple_drop_time_limit;
	unsigned int m_kick_delay_time;

	int m_mojing_reward_time;
	int m_mojing_reward;

	ZhuaGuiNpcSceneCfg m_scene_npc_list[ZHUAGUI_SCENE_COUNT];
	ZhuaGuiMonsterFlushCfg m_monster_flush_list[ZHUAGUI_MONSTER_FLUSH_COUNT];
	ZhuaGuiRewardDecCfg m_reward_dec_list[ZHUAGUI_REWARD_DEC_COUNT];
	ZhuaGuiTeamAdd m_team_add_list[MAX_TEAM_MEMBER_NUM];
};

#endif // _ZHUAGUI_CONFIG_HPP_



