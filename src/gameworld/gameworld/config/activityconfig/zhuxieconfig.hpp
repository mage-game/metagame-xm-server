#ifndef __ZHU_XIE_CONFIG_HPP__
#define __ZHU_XIE_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

struct ZhuXieReward
{
	ZhuXieReward() : shengwang(0) {}

	int shengwang;
	ItemConfigData item;
};

struct ZhuXieReplaceBossCfg
{
	ZhuXieReplaceBossCfg() : replace_boss_id(0), replace_need_world_level(0), flush_time_on_die(0), pos_id_start(0), pos_id_len(0) {}

	int replace_boss_id;
	int replace_need_world_level;
	int flush_time_on_die;
	int pos_id_start;
	int pos_id_len;
};

struct ZhuXieBossCfg
{
	ZhuXieBossCfg() : replace_boss_count(0) {}

	int replace_boss_count;
	ZhuXieReplaceBossCfg replace_boss_list[ZHUXIE_MAX_RELACE_BOSS_NUM];
};

struct ZhuXieReplaceMonsterCfg
{
	ZhuXieReplaceMonsterCfg() : replace_monster_id(0), replace_need_world_level(0), count(0), pos_id_start(0), pos_id_len(0), shengwang(0) {}

	int replace_monster_id;
	int replace_need_world_level;
	int count;
	int pos_id_start;
	int pos_id_len;
	int shengwang;

	ItemConfigData reward_item;
};

struct ZhuXieMonsterCfg
{
	ZhuXieMonsterCfg() : replace_monster_count(0) {}

	int replace_monster_count;
	ZhuXieReplaceMonsterCfg replace_monster_list[ZHUXIE_MAX_RELACE_MONSTER_NUM];
};

struct ZhuXieReplaceGatherCfg
{
	ZhuXieReplaceGatherCfg() : replace_gather_id(0), replace_need_world_level(0), gather_time(0), count(0), pos_id_start(0), pos_id_len(0), shengwang(0) {}

	int replace_gather_id;
	int replace_need_world_level;
	int gather_time;
	int count;
	int pos_id_start;
	int pos_id_len;
	int shengwang;

	ItemConfigData reward_item;
};

struct ZhuXieGatherCfg
{
	ZhuXieGatherCfg() : replace_gather_count(0) {}

	int replace_gather_count;
	ZhuXieReplaceGatherCfg replace_gather_list[ZHUXIE_MAX_RELACE_GATHER_NUM];
};

class ZhuXieConfig : public ConfigBase
{
public:
	ZhuXieConfig();
	~ZhuXieConfig();

	bool Init(const std::string &path, std::string *err);
	int InitOther(PugiXmlNode RootElement);
	int InitTaskList(PugiXmlNode RootElement);
	int InitRealivePoint(PugiXmlNode RootElement);
	int InitFlushPoint(PugiXmlNode RootElement);
	int InitBoss(PugiXmlNode RootElement);
	int InitMonster(PugiXmlNode RootElement);
	int InitGather(PugiXmlNode RootElement);

	int GetZhuXieFBEnterInfo(Posi *enterpos);
	int GetKillRoleGetShengWang() { return m_kill_role_get_shengwang; }
	int GetKillRoleGetMaxShengWang() { return m_kill_role_get_max_shengwang; }

	void GetZhuXieTaskList(ZhuXieTaskInfo zhuxie_task_list[ZHUXIE_TASK_MAX], int world_level);
	const ZhuXieTaskInfo *GetZhuXieTaskCfg(int task_id);
	const ZhuXieReplaceBossCfg *GetReplaceBossCfg(int boss_index, int world_level);
	const ZhuXieReplaceBossCfg *GetZhuXieReplacBossCfgById(int boss_id);
	const ZhuXieReplaceMonsterCfg *GetReplaceMonsterCfg(int monster_index, int world_level);
	const ZhuXieReplaceGatherCfg *GetReplaceGatherCfg(int gather_index, int world_level);
	bool GetRandomFlushPos(Posi &flush_pos, int id_star, int id_len);

	const ZhuXieReward *GetKillMonsterReward(int monster_id);
	const ZhuXieReward *GetGatherReward(int gather_id);

private:
	int m_zhuxie_sceneid;													// 诛邪场景ID
	int m_kill_role_get_shengwang;
	int m_kill_role_get_max_shengwang;

	int m_zhuexie_realive_pos_count;										// 诛邪复活点数量
	Posi m_zhuxie_realive_pos_list[ZHUXIE_REALIVE_POS_MAX];					// 诛邪复活点列表

	std::map<int, ZhuXieTaskInfo> m_zhuxie_task_map;						// 诛邪任务 相关配置信息

	std::map<int, Posi> m_flush_point_map;

	ZhuXieBossCfg m_zhuxie_boss_cfg_list[ZHUXIE_BOSS_NUM];					// 诛邪战场BOSS列表
	ZhuXieMonsterCfg m_zhuxie_monster_cfg_list[ZHUXIE_MONSTER_NUM];			// 诛邪战场怪物列表
	ZhuXieGatherCfg m_zhuxie_gather_cfg_list[ZHUXIE_GATHER_NUM];			// 诛邪战场采集物列表

	std::map<int, ZhuXieReward> m_monster_reward_map;
	std::map<int, ZhuXieReward> m_gather_reward_map;
};

#endif // __ZHU_XIE_CONFIG_HPP__

