#ifndef __WORLD_EVENT_CONFIG_HPP__
#define __WORLD_EVENT_CONFIG_HPP__

class WorldEventObj;
class Scene;

#include "servercommon/servercommon.h"
#include "servercommon/worldeventdef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <set>

struct WorldEventSceneConfigItem
{
	WorldEventSceneConfigItem() : scene_id(0), pos_count(0) {}

	void RandPos();
	bool TopPos(Posi *pos);
	bool PushPos(const Posi &pos);
	void PopPos();

	int scene_id;

	int pos_count;
	Posi pos_list[WORLD_EVENT_POS_CONFIG_MAX_COUNT_PER_SCENE];
};

struct WorldEventNpcPosConfig
{
	WorldEventNpcPosConfig() {}

	WorldEventSceneConfigItem * FindSceneCfg(int scene_id);

	WorldEventSceneConfigItem scene_list[WORLD_EVENT_NPC_LOCATE_SCENE_MAX_COUNT];
};

struct WorldEventPosItem
{
	WorldEventPosItem() : scene_id(0), pos(0, 0) {}
	WorldEventPosItem(int _scene_id, Posi _pos) : scene_id(_scene_id), pos(_pos) {}

	int scene_id;
	Posi pos;
};

struct WorldEventPosCfg
{
	WorldEventPosCfg() : pos_item_count(0) {}

	void RandPos();
	bool Top(WorldEventPosItem *pos_item);
	bool Push(const WorldEventPosItem &pos_item);
	void Pop();

	int pos_item_count;
	WorldEventPosItem pos_item_list[WORLD_EVENT_POS_CONFIG_MAX_COUNT];
};

struct WorldEventLogicOpenConfig
{
	struct LogicStateTime
	{
		LogicStateTime() : hour(0), minute(0) {}

		int hour;
		int minute;
	};

	struct ConfigItem
	{
		ConfigItem() {}

		LogicStateTime logic_state_time[WORLD_EVENT_LOGIC_TYPE_MAX];
	};

	WorldEventLogicOpenConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[WORLD_EVENT_LOGIC_OPEN_MAX_TIMES_PER_DAY];
};

struct WorldEventFallingItemConfig
{
	struct ConfigItem
	{
		ConfigItem() : bind_gold(0) {}

		int bind_gold;
	};

	WorldEventFallingItemConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[WORLD_EVENT_TIANNVSHANHUA_FALLING_ITEM_MAX_COUNT];
};

struct WorldEventOtherConfig
{
	WorldEventOtherConfig() : shenmozhixi_fb_scene_id(0), shenmozhixi_fb_birth_pos(0, 0), shenmozhixi_world_event_id(0), tiannnvshanhua_monster_id(0), tiannvshanhua_scene_id(0),
		tiannvshanhua_drop_range(0)
	{
	}

	int shenmozhixi_fb_scene_id;
	Posi shenmozhixi_fb_birth_pos;
	int shenmozhixi_world_event_id;
	int tiannnvshanhua_monster_id;
	int tiannvshanhua_scene_id;
	int tiannvshanhua_drop_range;
};

struct WorldEventTiannvshanhuaMoveCfg
{
	struct ConfigItem
	{
		ConfigItem() {}

		Posi pos_list[WORLD_EVENT_TIANNVSHANHUA_MOVE_POINT_MAX_COUNT];
	};

	WorldEventTiannvshanhuaMoveCfg() {}

	ConfigItem cfg_list[WORLD_EVENT_TIANNVSHANHUA_MONSTER_COUNT];
};

struct WorldEventObjConfig
{
	struct ConfigItem
	{
		ConfigItem() : world_event_id(0), obj_type(WORLD_EVENT_OBJ_INVALID), param(0),  last_time_s(0),
			max_hp(0), touch_distance(0) {}

		int world_event_id;						// 世界对象ID 策划配置唯一
		int obj_type;							// 世界对象 类型 加强检查用
		int param;								// 参数 
		int last_time_s;						// 持续时间 
		int max_hp;								// 最大血量
		int touch_distance;						// touch最小距离
	};

	WorldEventObjConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[WORLD_EVENT_OBJ_TYPE_MAX_COUNT];
};

//-----------------------------------------------------------------------------------------
struct KillRandomBossConfig
{
	KillRandomBossConfig()
		: flush_num(0), boss_id_count(0), pos_count(0)
	{
		memset(boss_id_list, 0, sizeof(boss_id_list));
	}

	int RandomBossID()
	{
		if (boss_id_count > 0)
		{
			return boss_id_list[RandomNum(boss_id_count)];
		}

		return 0;
	}

	struct FlushPosCfgItem
	{
		FlushPosCfgItem() : scene_id(0), pos(0, 0) {}

		int scene_id;
		Posi pos;

		bool IsVaild() { return 0 != scene_id; }
	};

	void RandPos()
	{
		for (int i = 0; i < pos_count; ++ i)
		{
			int rand_pos = RandomNum(pos_count);

			FlushPosCfgItem tmp_pos = pos_cfg_list[i];
			pos_cfg_list[i] = pos_cfg_list[rand_pos];
			pos_cfg_list[rand_pos] = tmp_pos;
		}
	}

	int flush_num;

	int boss_id_count;
	int boss_id_list[WORLD_EVENT_RANDOM_BOSS_TYPE_MAX];

	int pos_count;
	FlushPosCfgItem pos_cfg_list[WORLD_EVENT_RANDOM_BOSS_POS_COUNT];
};

//-----------------------------------------------------------------------------------------
struct JiqingPKConfig
{
	JiqingPKConfig()
		: monster_id(0), pos_count(0)
	{
		
	}

	struct FlushPosCfgItem
	{
		FlushPosCfgItem() : scene_id(0), pos(0, 0) {}

		int scene_id;
		Posi pos;

		bool IsVaild() { return 0 != scene_id; }
	};

	void RandPos()
	{
		for (int i = 0; i < pos_count; ++ i)
		{
			int rand_pos = RandomNum(pos_count);

			FlushPosCfgItem tmp_pos = pos_cfg_list[i];
			pos_cfg_list[i] = pos_cfg_list[rand_pos];
			pos_cfg_list[rand_pos] = tmp_pos;
		}
	}

	int monster_id;					// 用来代替PKrole的怪物ID

	int pos_count;
	FlushPosCfgItem pos_cfg_list[WORLD_EVENT_PKROLE_POS_COUNT];
};

//-----------------------------------------------------------------------------------------
struct XukongzhimengConfig
{
	XukongzhimengConfig()
		: door_event_id(0), flush_monster_interval(0), monster_idcount(0), doorcfg_count(0)
	{
		memset(monster_idlist, 0, sizeof(monster_idlist));
	}

	struct DoorCfgItem
	{
		DoorCfgItem() : scene_id(0), start_pos(0, 0), disapper_count(0)
		{

		}

		int scene_id;
		Posi start_pos;

		int disapper_count;
		Posi disapper_pos_list[WORLD_EVENT_XUKONGZHIMENG_DISAPPER_POS_MAX];

		bool IsVaild() { return 0 != scene_id; }
	};

	int RandomMonsterID()
	{
		if (monster_idcount > 0)
		{
			return monster_idlist[RandomNum(monster_idcount)];
		}

		return 0;
	}


	int door_event_id;

	int flush_monster_interval;
	
	int monster_idcount;
	int monster_idlist[WORLD_EVENT_XUKONGZHIMENG_MONSTER_MAX];

	int doorcfg_count;
	DoorCfgItem doorcfg_list[WORLD_EVENT_XUKONGZHIMENG_DOOR_MAX];
};

//-----------------------------------------------------------------------------------------
struct QuanmintafangConfig
{
	QuanmintafangConfig()
		: tower_event_id(0), door_event_id(0), flush_monster_interval(0), monster_idcount(0), towercfg_count(0), 
		kill_monster_score(0), tower_addscore(0), tower_addscore_interval(0), tower_addscore_range(0),
		monster_disapper_hp(0)
	{
		memset(monster_idlist, 0, sizeof(monster_idlist));
	}

	struct TowerCfgItem
	{
		TowerCfgItem() : scene_id(0), disapper_pos(0, 0), start_count(0)
		{

		}

		int scene_id;
		Posi disapper_pos;

		int start_count;
		Posi start_pos_list[WORLD_EVENT_QUANMINGTAFANG_START_POS_MAX];

		bool IsVaild() { return 0 != scene_id; }
	};

	int RandomMonsterID()
	{
		if (monster_idcount > 0)
		{
			return monster_idlist[RandomNum(monster_idcount)];
		}

		return 0;
	}

	int tower_event_id;
	int door_event_id;

	int flush_monster_interval;

	int monster_idcount;
	int monster_idlist[WORLD_EVENT_QUANMINGTAFANG_MONSTER_MAX];

	int towercfg_count;
	TowerCfgItem towercfg_list[WORLD_EVENT_QUANMINGTAFANG_TOWER_MAX];

	int kill_monster_score;
	int tower_addscore;
	int tower_addscore_interval;
	int tower_addscore_range;

	int monster_disapper_hp;
};
//-----------------------------------------------------------------------------------------

struct WorldEventLightWordConfig
{
	struct ConfigItem
	{
		ConfigItem() : word_seq(0), stuff_id(0), stuff_count(0) {}

		int word_seq;
		ItemID stuff_id;
		int stuff_count;
	};

	WorldEventLightWordConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[WORLD_EVENT_TITLE_WORD_MAX_NUM];
};

struct WorldEventTypeConfig
{
	struct ConfigItem
	{
		ConfigItem() : event_type(WORLD_EVENT_TYPE_INVALID), complete_event_score(0), 
			base_coin(0), base_xianhun(0), base_zhenqi(0) {}

		int event_type;
		int complete_event_score;
		int base_coin;
		int base_xianhun;
		int base_zhenqi;
		ItemConfigData base_item;
	};

	WorldEventTypeConfig() {}

	ConfigItem cfg_list[WORLD_EVENT_TYPE_MAX];
};

struct WorldEventRewardConfig
{
	struct ConfigItem
	{
		ConfigItem()
		{
			memset(need_score_list, 0, sizeof(need_score_list));
			memset(reward_factor_list, 0, sizeof(reward_factor_list));
		}

		int need_score_list[WORLD_EVENT_REWARD_GRADE_COUNT];
		int reward_factor_list[WORLD_EVENT_REWARD_GRADE_COUNT];
	};

	WorldEventRewardConfig() {}

	ConfigItem cfg_list[WORLD_EVENT_TYPE_MAX];
};

class WorldEventConfig : public ConfigBase
{
public:
	WorldEventConfig();
	~WorldEventConfig();

	bool Init(std::string path, std::string *err);

	const WorldEventNpcPosConfig & GetNpcPosCfg() { return m_npc_pos_cfg; }
	bool GetNPCPos(int scene_cfg_index, int pos_cfg_index, int *scene_id, Posi *pos);
	
	const WorldEventLogicOpenConfig * GetLogicOpenCfg(int event_type);
	void GetNpcIDList(int npc_idlist[WORLD_EVENT_NPC_TYPE_MAX_COUNT]);
	bool GetNpcIDIndex(int npc_id, int *locate_index);
	const WorldEventObjConfig::ConfigItem * GetWorldEventObjCfg(int world_event_obj_id);
	WorldEventObj * CreateWorldEventObj(int world_event_id, Scene *scene, const Posi &posi);

	KillRandomBossConfig & GetKillRandomBossConfig() { return m_kill_randomboss_cfg; }
	JiqingPKConfig & GetJiqingPKConfig() { return m_jiqing_pk_cfg; }
	XukongzhimengConfig & GetXukongzhimengConfig() { return m_xukongzhimeng_cfg; }
	QuanmintafangConfig & GetQuanmintafangConfig() { return m_quanmingtafang_cfg; }

	const WorldEventPosCfg & GetShenmozhijiPosCfg() { return m_shenmozhiji_pos_cfg; }
	const WorldEventLightWordConfig::ConfigItem * GetLightWordCfg(int word_seq);
	int GetLightWordCount() { return m_light_word_cfg.cfg_count; }

	const WorldEventOtherConfig & GetOtherCfg() { return m_other_cfg; }
	Posi GetTiannvshanhuaMonsterMovePos(int monster_idx, int target_move_pos_idx);
	const WorldEventFallingItemConfig & GetTiannvshanhuaFallingItemCfg() { return m_tiannvshanhua_falling_item_cfg; }

	const WorldEventTypeConfig::ConfigItem * GetEventTypeCfg(int event_type);
	const WorldEventRewardConfig::ConfigItem * GetRewardCfg(int event_type);

	bool RandFindNpcId(short find_npc_flag, char *find_npc_id);
	bool IsFindNpcMonster(int monster_id);
	bool IsKillRandBossEventBoss(int monster_id);
	bool IsQuanmintafangEventMonster(int monster_id);
	int RandSpecialMonsterAppearance();

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitWorldEventObjCfg(PugiXmlNode RootElement);
	int InitOpenCfg(PugiXmlNode RootElement);
	int InitNpcPosCfg(PugiXmlNode RootElement);
	int InitNpcObjCfg(PugiXmlNode RootElement);
	int InitWorldEventTypeCfg(PugiXmlNode RootElement);
	int InitShenmozhijiPosCfg(PugiXmlNode RootElement);
	int InitTiannvsanhuaFallingItemCfg(PugiXmlNode RootElement);
	int InitTiannvsanhuaMoveCfg(PugiXmlNode RootElement);
	int InitLightWordCfg(PugiXmlNode RootElement);
	int InitAppearanceCfg(PugiXmlNode RootElement);

	int InitKillRandomBossConfig(PugiXmlNode RootElement);
	int InitJiqingPKConfig(PugiXmlNode RootElement);
	int InitXukongzhimengConfig(PugiXmlNode RootElement);
	int InitQuanmintafangConfig(PugiXmlNode RootElement);
	int InitRewardConfig(PugiXmlNode RootElement);

	bool TransformLogicStateTime(int time_value, WorldEventLogicOpenConfig::LogicStateTime  *logic_state_time);

	WorldEventLogicOpenConfig m_logic_open_cfglist[WORLD_EVENT_TYPE_MAX];			// 世界时间 逻辑 开启时间
	WorldEventObjConfig m_world_event_obj_cfg;										// 事件对象 配置
	WorldEventPosCfg m_shenmozhiji_pos_cfg;											// 神魔之隙配置
	WorldEventLightWordConfig m_light_word_cfg;										// 点亮字配置
	WorldEventOtherConfig m_other_cfg;												// 其他
	WorldEventTiannvshanhuaMoveCfg m_tiannvshanhua_move_cfg;						// 天女散花移动坐标配置
	WorldEventFallingItemConfig m_tiannvshanhua_falling_item_cfg;					// 天女散花掉落物配置

	WorldEventNpcPosConfig m_npc_pos_cfg;											// 寻找npc 配置
	int m_npc_idlist[WORLD_EVENT_NPC_TYPE_MAX_COUNT];								// npc id列表
	std::set<int> m_find_npc_monsterid_set;											// 寻找npc 怪物id set

	std::set<int> m_kill_randomboss_bossid_set;										// 随机boss 怪物id set
	KillRandomBossConfig m_kill_randomboss_cfg;										// 随机boss配置

	JiqingPKConfig m_jiqing_pk_cfg;													// 激情pk配置

	XukongzhimengConfig m_xukongzhimeng_cfg;										// 虚空之门配置

	std::set<int> m_quanmintafang_monster_id_set;									// 全民塔防怪物id set
	QuanmintafangConfig m_quanmingtafang_cfg;										// 全民塔防配置

	WorldEventTypeConfig m_event_type_cfg;											// 世界事件类型配置

	std::set<int> m_appearance_monster_set;											// 外观配置

	WorldEventRewardConfig m_reward_config;											// 奖励配置
};


#endif // __WORLD_EVENT_CONFIG_HPP__



