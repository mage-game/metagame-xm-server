#ifndef __JOIN_LIMIT_CONFIG_HPP__
#define __JOIN_LIMIT_CONFIG_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <map>
#include "gamedef.h"

class Role;

enum JOIN_LIMIT_MAIN_TYPE
{
	JLMT_INVALID = 0,
	JLMT_ACTIVITY,
	JLMT_TYPE_FB,
	JLMT_OTHER,

	JLMT_MAX,
};

enum JOIN_LIMIT_SUB_TYPE_OTHER
{
	JLMT_OTHER_INVALID = 0, 
	JLMT_OTHER_CHESTSHOP = 1,
	JLMT_OTHER_DAILY_TASK = 2,
	JLMT_OTHER_HUSONG_TASK = 3,
	JLMT_OTHER_MOUNT_UPLEVEL_EQUIP = 4,
	JLMT_OTHER_WING_UPLEVEL_EQUIP = 5,
	JLMT_OTHER_HALO_UPLEVEL_EQUIP = 6,
	JLMT_OTHER_SHENGONG_UPLEVEL_EQUIP = 7,
	JLMT_OTHER_SHENYI_UPLEVEL_EQUIP = 8,	
	JLMT_OTHER_CHALLENGE_FIELD = 10,
	JLMT_OTHER_FISH_POOL = 11,
	JLMT_OTHER_SHENMOZHIXI = 12,
	JLMT_OTHER_CHANGE_CAMP = 13,
	JLMT_OTHER_ZHUANZHI = 14,
	JLMT_OTHER_YSGC = 15,						// 妖兽广场
	JLMT_OTHER_SYT = 16,						// 锁妖塔
	JLMT_OTHER_PAOHUAN_TASK = 17,				// 跑环 130级 
	JLMT_OTHER_JH_HUSONG_JOIN = 18,				// 精华护送参与
	JLMT_OTHER_TOUZHIJIHUA = 19,				// 投资计划
	JLMT_OTHER_BABY = 20,						// 宝宝
	JLMT_OTHER_CHENGJIU = 22,					// 成就
	JLMT_OTHER_FIGHTING = 23,					// 决斗场
	JLMT_OTHER_ZHUANZHI_TASK = 24,				// 转职任务

	JLMT_OTHER_MAX, 
};

struct JoinLimitType
{
	JoinLimitType() : main_type(0), sub_type(0) {}
	JoinLimitType(int _main_type, int _sub_type) : main_type(_main_type), sub_type(_sub_type) {}

	int main_type;
	int sub_type;
};

struct JoinLimitUnit
{
	JoinLimitUnit() : level(0), max_level(0), capability(0), is_notice(false), day_limit_switch(false) {}

	int level;
	int max_level;
	int capability;
	bool is_notice;
	bool day_limit_switch;
	std::vector<std::vector<int> > level_range_vec;
	std::vector<std::vector<int> > open_day_vec_vec;
	std::vector<std::vector<int> > open_server_day_range_vec;
	JoinLimitType join_type;
};

enum FUN_OPEN_TYPE
{
	FUN_OPEN_TYPE_MOUNT = 0,
	FUN_OPEN_TYPE_WING = 1,
	FUN_OPEN_TYPE_HALO = 2,
	FUN_OPEN_TYPE_SHENGONG = 3,
	FUN_OPEN_TYPE_SHENYI = 4,
	FUN_OPEN_TYPE_XIANNVSHOUHU = 5,
	FUN_OPEN_TYPE_JINGLINGGUANGHUAN = 6,
	FUN_OPEN_TYPE_JINGLINGFAZHEN = 7,
	FUN_OPEN_TYPE_FIGHT_MOUNT = 8,
	FUN_OPEN_TYPE_FOOTPRINT = 9,
	FUN_OPEN_TYPE_CLOAK = 10,
	FUN_OPEN_TYPE_FABAO = 11,                      // 法宝功能激活类型11
	FUN_OPEN_TYPE_SHIZHUANG = 12,                  // 时装功能激活类型12
	FUN_OPEN_TYPE_SHENBING = 13,                   // 神兵功能激活类型13
	FUN_OPEN_TYPE_YAOSHI = 14,
	FUN_OPEN_TYPE_TOUSHI = 15,
	FUN_OPEN_TYPE_QILINBI = 16,
	FUN_OPEN_TYPE_MASK = 17,
	FUN_OPEN_TYPE_RUNE = 18,                      // 战魂
	FUN_OPEN_TYPE_XIANNV = 19,                    // 仙女
	FUN_OPEN_TYPE_XIANCHONG = 20,                 // 仙宠
	FUN_OPEN_TYPE_SHENZHOU_WEAPON = 21,           // 异火
	FUN_OPEN_TYPE_SHENGE = 22,                    // 星辉
	FUN_OPEN_TYPE_SHENSHOU = 23,                  // 龙器
	FUN_OPEN_TYPE_SHENYIN = 24,                   // 铭纹
	FUN_OPEN_TYPE_EQUIPMENT_FORGE = 25,           // 锻造
	FUN_OPEN_TYPE_GREATE_SOLDIER = 26,			  // 神魔(名将)
	FUN_OPEN_TYPE_CHINESE_ZODIAC = 27,			  // 生肖
	FUN_OPEN_TYPE_SHENGQI = 28,					  // 圣器
	FUN_OPEN_TYPE_CROSS_EQUIP = 29,				  // 斗气装备

	FUN_OPEN_TYPE_MAX,
};

struct FunOpenUnit
{
	FunOpenUnit() : task_id(0), role_level(0), opengame_day(0), param1(0), param2(0) {}

	TaskID task_id;
	int role_level;
	int opengame_day;
	int param1;
	int param2;
};

class JoinLimitConfig : public ConfigBase
{
public:
	JoinLimitConfig();
	~JoinLimitConfig();

	bool Init(const std::string &path, std::string *err);

	const FunOpenUnit * GetFunOpenCfg(int open_type);
	TaskID GetFunOpenTaskID(int open_type);
	int GetFunOpenRoleLevel(int open_type);
	int GetFunOpenParam1(int open_type);
	int GetFunOpenParam2(int open_type);

	bool GetJoinLimit(int main_type, int sub_type, JoinLimitUnit *limit_unit);
	bool GetActivityJoinLimit(int activity_type, JoinLimitUnit *limit_unit);
	bool GetFBJoinLimit(int fb_type, JoinLimitUnit *limit_unit);
	bool GetOtherJoinLimit(int sub_type, JoinLimitUnit *limit_unit);

	bool CanJoinActivity(int activity_type, Role *role);
	bool CanJoinFB(int fb_type, Role *role);
	bool CanJoinOther(int sub_type, Role *role);

private:
	long long JoinLimitTypeToLongLong(JoinLimitType limit_type);
	int InitJoinLImitConfig(PugiXmlNode RootElement);
	bool CheckCanJoin(const JoinLimitUnit &limit_unit, Role *role);
	int InitFunOpenConfig(PugiXmlNode RootElement);

	typedef std::map<long long, JoinLimitUnit> JoinLimitMap;
	typedef std::map<long long, JoinLimitUnit>::iterator JoinLimitMapIt;

	JoinLimitMap m_limit_map;
	FunOpenUnit m_funopen_cfglist[FUN_OPEN_TYPE_MAX];
};

#endif // __JOIN_LIMIT_CONFIG_HPP__

