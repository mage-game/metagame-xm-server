#ifndef __MONSTERINITPARAM_H__
#define __MONSTERINITPARAM_H__

#include <vector>
#include <string>

#include "gamedef.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"


class MonsterInitParam
{
public:
	MonsterInitParam() : monster_type(0), monster_id(0), prof(0), ai_type(0), cold_down(0), capability(0), hp(0), boss_type(0),
		gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), per_jingzhun(0), per_baoji(0), per_kangbao(0), per_pofang(0), per_mianshang(0),
		level(0), move_speed(0), skillid(0), 
		run_range(0), aoi_range(0), boss_skill_cond_id(0), drop_type(0),
		drop_money_count(0), drop_money_prob(0),
		exp_reward(0), yuanli_reward(0), disappear_time(0), die_notice(false), dead_notice(false), forbid_capture_by_skill(false), is_fix_hurt_on_attack(false),
		is_forbid_recover(false), flush_notice(false), walk_point_num(0), walk_range(0), auto_reover_per(0), auto_reover_time_gap(0)
	{
		memset(monster_name, 0, sizeof(monster_name));
	}

	bool Init(const std::string &configname, std::string &err);

	enum MONSTER_TYPE
	{
		MONSTER_TYPE_NORMAL = 0,								// 普通怪物
		MONSTER_TYPE_BOSS,										// BOSS怪物 
		MONSTER_TYPE_MAX,
	};

	enum BOSS_TYPE
	{
		BOSS_TYPE_INVALID = 0,
		BOSS_TYPE_NORMAL = 1,									// 普通boss
		BOSS_TYPE_WORLD = 2,									// 世界boss
		BOSS_TYPE_SPECIAL = 3,                                  // 珍惜boss
		BOSS_TYPE_CROSS_BOSS = 4,                               // 跨服boss
		BOSS_TYPE_MAX,
	};

	// 任务的boss类型
	enum BOSS_TASK_TYPE
	{
		BOSS_TYPE_TASK_INVALID_TYPE = -1,
		BOSS_TYPE_TASK_NORMAL,								// 普通怪
		BOSS_TYPE_TASK_MIKU,								// 秘窟boss
		BOSS_TYPE_TASK_ACTIVE,								// 活跃boss
		BOSS_TYPE_TASK_VIP,									// VIPboss
		BOSS_TYPE_TASK_PERSON,								// 个人boss

		BOSS_TYPE_TASK_MAX_TYPE,
	};

	enum BOSS_DROP_OWN_TYPE
	{
		BOSS_DROP_OWN_TYPE_NONE = 0,							// 无归属（谁都可以检）
		BOSS_DROP_OWN_TYPE_MAX_HURT_ROLE = 1,					// 最高伤害 个人归属掉落
		BOSS_DROP_OWN_TYPE_MAX_HURT_TEAM = 2,					// 最高伤害 队伍归属掉落(必须造成伤害，所有人随机分一份)
		BOSS_DROP_OWN_TYPE_KILLER = 3,                          // 击杀掉落 谁杀给谁
		BOSS_DROP_OWN_TYPE_ALL_KILL_TEAM = 4,                   // 击杀掉落 全队
		BOSS_DROP_OWN_TYPE_FIRST_ATTACK = 5,					// 首刀掉落（必须造成伤害，所有人随机分一份)
		BOSS_DROP_OWN_TYPE_FIRST_ALL_TEAM = 6,					// 首刀掉落 全队每人一份（必须造成伤害）
		BOSS_DROP_OWN_TYPE_MAX,
	};

	enum AITYPE
	{
		AITYPE_INVALID = -1,			

		AITYPE_FOOL = 0,										// 不走动 不还手 
		AITYPE_STATIC,											// 静止怪 非主动怪
		AITYPE_PASSIVE,											// 巡逻非主动怪
		AITYPE_INITIATIVE,										// 巡逻主动怪
		AITYPE_STAT_INITIAT,									// 静止主动怪
		AITYPE_JUST_RUN,										// 随即乱跑怪
		AITYPE_JUST_FLEE,										// 被打逃跑怪
		AITYPE_STATIC_ADD_MIANSHANG_BUFF,						// 加免伤buff怪
		AITYPE_STATCI_ADD_RECOVER_BUFF,							// 加血怪

		AITYPE_SPECIALAI_LINGSHANXIANQI = 10000,				// 灵山仙器 灵器AI
		AITYPE_SPECIALAI_BATTLEFIELD,							// 战场 怪物AI
		AITYPE_SPECIALAI_GONGCHENGZHAN,							// 攻城战

		AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK = 10100,		// 多人塔防进攻怪
		AITYPE_SPECIALAI_TOWERDEFEND_TEAM_SHOUHU = 10101,		// 塔防守护怪

		AITYPE_SPECIALAI_XIANMENGZHAN_GUARDBOSS = 10200,		// 仙盟战 - 守卫灵兽
		AITYPE_SPECIALAI_XIANMENGZHAN_AREABOSS  = 10201,        // 仙盟战 - 据点怪
		AITYPE_SPECIALAI_XIANMENGZHAN_DANYAOBOSS  = 10202,      // 仙盟战 - 丹药怪

		AITYPE_SPECIALAI_GUILD_TOWERDEFEND_ATTACK = 10300,		// 仙盟塔防 - 进攻怪
		AITYPE_SPECIALAI_GUILD_TOWERDEFEND_SHOUHU = 10301,		// 仙盟塔防 - 守护怪
		AITYPE_SPECIALAI_GUILD_TOWERDEFEND_MT = 10302,			// 仙盟塔防 - MT怪

		AITYPE_CISHA_CAMP1 = 10401,								// 刺杀任务怪 阵营1
		AITYPE_CISHA_CAMP2 = 10402,								// 刺杀任务怪 阵营2
		AITYPE_CISHA_CAMP3 = 10403,								// 刺杀任务怪 阵营3

		AITYPE_GUILDFB_GUARD = 10501,							// 仙盟副本 守卫boss
		AITYPE_GUILDFB_ENEMY = 10502,							// 仙盟副本 敌人

		AITYPE_COIN = 10601,									// 铜币本怪物AI
		AITYPE_CROSS_PASTURE_ANIMAL = 10602,					// 跨服牧场 动物

		AITYPE_BUILD_TOWER_TOWER = 10604,						// 塔防本 塔怪
		AITYPE_BUILD_TOWER_RUNNING = 10605,						// 塔防本 跑怪

		AITYPE_SPECIALAI_TERRITORYWAR_TOWER = 10701,			// 领土战-防御塔
		AITYPE_SPECIALAI_GIFT_HARVEST = 10702,					// 礼物收割ai
		
		AITYPE_MAX,
	};

	static const int DROP_LIST_MAX_COUNT = 64;					// 掉落列表不允许超过16个
	static const int MAX_DROP_ITEM_COUNT = 64;

	int				monster_type;								// 类型
	UInt16			monster_id;									// ID

	int				prof;										// 职业
	GameName		monster_name;								// 名字
	int				ai_type;									// AI类型
	long long		cold_down;									// 冷却时间

	int				capability;									// 战斗力
	Attribute		hp;											// 血量
	int				boss_type;									// BOSS类型
	Attribute		gongji;										// 攻击
	Attribute		fangyu;										// 防御

	Attribute		mingzhong;									// 命中
	Attribute		shanbi;										// 闪避
	Attribute		baoji;										// 暴击
	Attribute		jianren;									// 坚韧
	
	Attribute		per_jingzhun;								// 精准万分比
	Attribute		per_baoji;									// 暴击万分比
	Attribute		per_kangbao;								// 抗暴万分比
	Attribute		per_pofang;									// 破防万分比
	Attribute		per_mianshang;								// 免伤万分比

	int				level;										// 等级
	Attribute		move_speed;									// 移动速度

	unsigned short	skillid;									// 普通技能ID

	unsigned int	run_range;									// 可移动范围
	unsigned int	aoi_range;									// 主动怪探测范围
	unsigned int	boss_skill_cond_id;							// Boss技能条件ID

	int				drop_type;									// 掉落类型
	std::vector<UInt16>	dropid_list;							// 掉落ID列表
	std::vector<UInt16> firsthit_dropid_list;					// 第一刀掉落ID列表
	std::vector<UInt16> dps_dropid_list;						// DPS掉落ID列表
	std::vector<UInt16> rand_dropid_list;						// 随机奖励掉落ID列表
	std::vector<UInt16> task_dropid_list;                       // 任务定向掉落
	int					drop_money_count;						// 掉钱数量
	int					drop_money_prob;						// 掉钱概率

	long long		exp_reward;									// 经验奖励
	int				yuanli_reward;								// 元力奖励
	unsigned int	disappear_time;								// 0表示不消失 单位秒

	bool			die_notice;									// 死亡是否公告
	bool            dead_notice;                                // 抢杀提示
	bool			forbid_capture_by_skill;					// 不能被技能拉扯或击退
	bool			is_fix_hurt_on_attack;						// 被打时是否固定为1伤害
	bool			is_forbid_recover;							// 禁止自动回血
	bool			flush_notice;								// 是否刷新公告

	unsigned int	walk_point_num;								// 怪物随机散步地点数
	unsigned int	walk_range;									// 怪物散步范围

	int				auto_reover_per;							// 一直自动回血万分比
	int				auto_reover_time_gap;						// 自动回血间隔时间（毫秒）

private:
	bool ReadDropIDList(PugiXmlNode dropListElement, std::vector<UInt16> &dropid_list);
};

#endif


