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
	COND_TYPE_DAYLY_TASK = 1,						// ����ճ�����X��
	COND_TYPE_EXP_FB = 2,							// ���鸱������X��
	COND_TYPE_PHASE_FB = 3,							// ���׸�����սX��
	COND_TYPE_GUILD_TASK = 4,						// ��ɹ�������X��
	COND_TYPE_CROSS_TEAM_FB = 5,					// ͨ��������Ӹ���X��
	COND_TYPE_HUSONG_TASK = 6,						// ����X��
	COND_TYPE_RECYCL_EQUIP = 7,						// ����װ��X��
	COND_TYPE_PATA_FB = 8,							// ����֮��ͨ��X��
	COND_TYPE_ROLE_LEVEL = 9,						// �ȼ��ﵽX
	COND_TYPE_GUILD_DONATE = 10,					// �����������X��
	COND_TYPE_GUILD_STORAGE = 11,					// �������װ��X��
	COND_TYPE_GUILD_BOX_OPEN = 12,					// ���ᱦ�俪��X��
	COND_TYPE_TUO_DAN = 13,							// �����������ԣ��Ѿ���������ɣ�
	COND_TYPE_ZHI_BAO_LEVEL = 14,					// ʥ��ﵽX��
	COND_TYPE_XUN_ZHANG_LEVEL = 15,					// ѫ�´ﵽX��
	COND_TYPE_KILL_BOSS = 16,						// ��ɱX��ָ��ID��BOSS
	COND_TYPE_TOTAL_STRENGTHEN = 17,				// ȫ��ǿ���ﵽX
	COND_TYPE_STRENGTHEN = 18,						// ����װ��ǿ���ﵽX
	COND_TYPE_TOTAL_UPSTAR = 19,					// ȫ�����ǴﵽX
	COND_TYPE_UPSTAR = 20,							// ����װ�����ǴﵽX
	COND_TYPE_TOTAL_STONE = 21,						// ȫ��ʯ�ﵽX
	COND_TYPE_STONE = 22,							// ��Ƕһ��X����ʯ
	COND_TYPE_EQUIP_SUIT = 23,						// ����X��ָ����װID����װ
	COND_TYPE_UPGRADE_MOUNT = 24,					// ������׵�X��
	COND_TYPE_UPGRADE_WING = 25,					// ������׵�X��
	COND_TYPE_UPGRADE_HALO = 26,					// �⻷���׵�X��
	COND_TYPE_UPGRADE_FIGHT_MOUNT = 27,				// ħ����׵�X��
	COND_TYPE_UPGRADE_SHENGONG = 28,				// �񹭽��׵�X��
	COND_TYPE_UPGRADE_SHENYI = 29,					// ������׵�X��
	COND_TYPE_ACTIVE_XIANNV = 30,					// ����X��Ů��
	COND_TYPE_CHENG_JIU_LEVEL = 31,					// �ɾʹﵽX��
	COND_TYPE_GUILD_BOX_DIG = 32,					// ���ᱦ���ھ�X��
	COND_TYPE_KILL_BOSS_TYPE = 33,					// ��ɱX��X��boss
	COND_TYPE_FB_GUIDE_YUBIAO_TYPE = 34,			// ������������������
	COND_TYPE_FB_GUIDE_SHUIJING_TYPE = 35,			// ����������ˮ���þ�
	COND_TYPE_CONVERT_SHOP = 36,					// �һ��̵�һ�X����Ʒ����X����
	COND_TYPE_RUNE_TOWER = 37,						// ������ͨ�ص�X��
	COND_TYPE_FB_GUIDE_BOSS_TYPE = 38,				// ����������boss��	
	COND_TYPE_CAUSE_HARM_ON_BOSS = 39,				// ���X���˺���X��boss
	COND_TYPE_CLIENT_NOTICE = 40,					// �ͻ���֪ͨ���
	COND_TYPE_UPGRADE_FOOTPRINT = 41,				// �㼣���׵�X��

	PERSONAL_GOAL_COND_MAX_TYPE,
};

enum COND_BOSS_TYPE
{
	COND_BOSS_INVALID_TYPE = 0,
	CON_WORLD_BOSS,					// ����boss
	COND_FAMILY_BOSS,				// boss֮��boss
	COND_MIKU_BOSS,					// �ؿ�boss
	COND_DABAO_BOSS,				// ��boss
	COND_ACTIVE_BOSS,				// ��Ծboss

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

