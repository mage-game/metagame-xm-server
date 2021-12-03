#ifndef __ROLE_GOAL_CONFIG_HPP__
#define __ROLE_GOAL_CONFIG_HPP__

#include <string>
#include "servercommon/struct/rolegoalparam.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

enum PERSONAL_GOAL_COND_TYPE
{
	PERSONAL_GOAL_COND_INVALID_TYPE = 0,
	COND_TYPE_DAYLY_TASK = 1,						// 完成日常任务X次
	COND_TYPE_EXP_FB = 2,							// 经验副本进入X次
	COND_TYPE_PHASE_FB = 3,							// 进阶副本挑战X次
	COND_TYPE_GUILD_TASK = 4,						// 完成公会任务X次
	COND_TYPE_CROSS_TEAM_FB = 5,					// 通关任意组队副本X次
	COND_TYPE_HUSONG_TASK = 6,						// 护送X次
	COND_TYPE_RECYCL_EQUIP = 7,						// 回收装备X件
	COND_TYPE_PATA_FB = 8,							// 勇者之塔通关X层
	COND_TYPE_ROLE_LEVEL = 9,						// 等级达到X
	COND_TYPE_GUILD_DONATE = 10,					// 公会捐献物资X个
	COND_TYPE_GUILD_STORAGE = 11,					// 公会捐献装备X个
	COND_TYPE_GUILD_BOX_OPEN = 12,					// 公会宝箱开启X个
	COND_TYPE_TUO_DAN = 13,							// 发布单身宣言（已经结婚就算完成）
	COND_TYPE_ZHI_BAO_LEVEL = 14,					// 圣物达到X级
	COND_TYPE_XUN_ZHANG_LEVEL = 15,					// 勋章达到X级
	COND_TYPE_KILL_BOSS = 16,						// 击杀X次指定ID的BOSS
	COND_TYPE_TOTAL_STRENGTHEN = 17,				// 全身强化达到X
	COND_TYPE_STRENGTHEN = 18,						// 单件装备强化达到X
	COND_TYPE_TOTAL_UPSTAR = 19,					// 全身升星达到X
	COND_TYPE_UPSTAR = 20,							// 单件装备升星达到X
	COND_TYPE_TOTAL_STONE = 21,						// 全身宝石达到X
	COND_TYPE_STONE = 22,							// 镶嵌一个X级宝石
	COND_TYPE_EQUIP_SUIT = 23,						// 集齐X件指定套装ID的套装
	COND_TYPE_UPGRADE_MOUNT = 24,					// 坐骑进阶到X阶
	COND_TYPE_UPGRADE_WING = 25,					// 羽翼进阶到X阶
	COND_TYPE_UPGRADE_HALO = 26,					// 光环进阶到X阶
	COND_TYPE_UPGRADE_FIGHT_MOUNT = 27,				// 魔骑进阶到X阶
	COND_TYPE_UPGRADE_SHENGONG = 28,				// 神弓进阶到X阶
	COND_TYPE_UPGRADE_SHENYI = 29,					// 神翼进阶到X阶
	COND_TYPE_ACTIVE_XIANNV = 30,					// 激活X个女神
	COND_TYPE_CHENG_JIU_LEVEL = 31,					// 成就达到X级
	COND_TYPE_GUILD_BOX_DIG = 32,					// 公会宝箱挖掘X个
	COND_TYPE_KILL_BOSS_TYPE = 33,					// 击杀X个X类boss
	COND_TYPE_FB_GUIDE_YUBIAO_TYPE = 34,			// 副本引导，运镖引导
	COND_TYPE_FB_GUIDE_SHUIJING_TYPE = 35,			// 副本引导，水晶幻境
	COND_TYPE_CONVERT_SHOP = 36,					// 兑换商店兑换X个物品消耗X货币
	COND_TYPE_RUNE_TOWER = 37,						// 符文塔通关到X层
	COND_TYPE_FB_GUIDE_BOSS_TYPE = 38,				// 副本引导，boss引	
	COND_TYPE_CAUSE_HARM_ON_BOSS = 39,				// 造成X点伤害对X类boss
	COND_TYPE_CLIENT_NOTICE = 40,					// 客户端通知完成
	COND_TYPE_UPGRADE_FOOTPRINT = 41,				// 足迹进阶到X阶

	PERSONAL_GOAL_COND_MAX_TYPE,
};

enum COND_BOSS_TYPE
{
	COND_BOSS_INVALID_TYPE = 0,
	CON_WORLD_BOSS,					// 世界boss
	COND_FAMILY_BOSS,				// boss之家boss
	COND_MIKU_BOSS,					// 秘窟boss
	COND_DABAO_BOSS,				// 打宝boss
	COND_ACTIVE_BOSS,				// 活跃boss

	COND_BOSS_MAX_TYPE,
};

enum FIELD_GOAL_SKILL_TYPE
{
	FIELD_GOAL_INVALID_SKILL_TYPE = 0,

	FIELD_GOAL_HURT_MONSTER_ADD,
	FIELD_GOAL_KILL_MONSTER_EXP_ADD,
	FIELD_GOAL_ABSORB_BLOOD,

	FIELD_GOAL_MAX_SKILL_TYPE,
};

struct PersonalGoalChapterCfg
{
	PersonalGoalChapterCfg() : chapter(0) {}

	struct CondCfg
	{
		CondCfg() : cond_type(0), cond_param1(0), cond_param2(0) {}
		int cond_type;
		int cond_param1;
		int cond_param2;
	};

	int chapter;
	CondCfg cond_list[PERSONAL_GOAL_COND_MAX];
	ItemConfigData prof_reward_1[MAX_REWARD_LIST_COUNT];
	ItemConfigData prof_reward_2[MAX_REWARD_LIST_COUNT];
	ItemConfigData prof_reward_3[MAX_REWARD_LIST_COUNT];
	ItemConfigData prof_reward_4[MAX_REWARD_LIST_COUNT];
};

struct BattleFieldGoalCfg
{
	BattleFieldGoalCfg():field_type(0), open_server_day(0), complete_score(0), is_has_reward(0), skill_type(0), param_1(0),
		param_2(0), param_3(0), param_4(0), level_limit(0){}
	
	int field_type;
	int open_server_day;
	int complete_score;
	int is_has_reward;
	int skill_type;
	int param_1;
	int param_2;
	int param_3;
	int param_4;
	int level_limit;
	ItemConfigData item_reward[MAX_REWARD_LIST_COUNT];
};

static const int BATTLE_FIELD_GOAL_SKILL_LEVEL_MAX = 100;
struct BattleFieldGoalSkillCfg
{
	struct ConfigItem
	{
		ConfigItem() : skill_type(0), skill_level(0), level_limit(0), param_1(0), param_2(0), param_3(0), param_4(0), capability(0) {}
		int skill_type;
		int skill_level;
		int level_limit;
		int param_1;
		int param_2;
		int param_3;
		int param_4;
		int capability;
		ItemConfigData uplevel_stuff_prof1;
		ItemConfigData uplevel_stuff_prof2;
		ItemConfigData uplevel_stuff_prof3;
		ItemConfigData uplevel_stuff_prof4;
	};

	ConfigItem cfg_list[FIELD_GOAL_MAX_SKILL_TYPE][BATTLE_FIELD_GOAL_SKILL_LEVEL_MAX + 1];
};

class RoleGoalConfig : public ConfigBase
{
public:
	RoleGoalConfig();
	~RoleGoalConfig();

	bool Init(std::string path, std::string *err);

	const PersonalGoalChapterCfg * GetPersonalGoalChapterCfg(int chapter) const;
	int GetChapterCfgByPersonalGoal(int cond_type);
	const BattleFieldGoalCfg * GetBattleFieldGoalCfg(int field_type);

	std::map<int, BattleFieldGoalCfg> &GetBattleFieldGoalCfgMap() { return m_field_type_goal_cfg_map; }

	BattleFieldGoalSkillCfg::ConfigItem *GetBattleFieldGoalSkillCfg(int skill_type, int skill_level);

private:
	int InitPersonalGoalChapterCfg(PugiXmlNode RootElement);
	int InitBattleFieldCfg(PugiXmlNode RootElement);
	int InitBattleFieldSkillCfg(PugiXmlNode RootElement);

	PersonalGoalChapterCfg m_personal_goal_chapter_cfg[PERSONAL_GOAL_CHAPTER_MAX + 1];
	std::map<int, BattleFieldGoalCfg> m_field_type_goal_cfg_map;
	BattleFieldGoalSkillCfg m_skill_cfg;
};

#endif

