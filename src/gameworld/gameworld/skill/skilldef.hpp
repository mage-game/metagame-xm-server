#ifndef __SKILLDEF_HPP__
#define __SKILLDEF_HPP__

#include <memory>

#include "servercommon/servercommon.h"
#include "servercommon/serverdef.h"
#include "gamedef.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 4)

static const int EFFECT_INFO_PARAM_MAX = 12;				// Effect信息最多参数个数

struct EffectInfo											// Effect信息 会用在协议中 注意对齐
{
	char			effect_type;							// EffectBase.EFFECT_TYPE
	char			product_method;							// EffectBase::PRODUCT_METHOD
	UInt16			product_id;								// 技能ID 物品ID 等
	unsigned int	unique_key;								// 唯一key
	int				client_effect_type;						// 客户端效果类型
	char			merge_layer;							// 叠加层数
	char			reserve;
	short			param_count;
	long long		param_list[EFFECT_INFO_PARAM_MAX];		// 参数
};

#pragma pack(pop)

static const int MAX_SKILL_LEVEL = 200;						// 最大的技能等级 
static const int MAX_PET_SKILL_LEVEL = 18;					// 宠物最大技能等级
static const int MAX_ROLE_SKILL_LEVEL = 21;

enum COMMONSKILL_ID											// 普通攻击技能ID
{
	COMMONSKILL_ID_INVALID = 0,

	COMMONSKILL_ID_JIANLING,
	COMMONSKILL_ID_CIKE,
	COMMONSKILL_ID_BIXIAN,
	COMMONSKILL_ID_FAZUN,

	COMMONSKILL_ID_MAX,
};

static const int PET_COMMON_SKILL_ID = 500;					// 宠物普通攻击ID
static const int NUQI_SKILL_ID = 5;							// 怒气技能ID
static const int MOSHOU_CHONGFENG_SKILL_ID = 520;			// 墨兽冲锋技能ID

static const int GREATE_SOLDIER_SKILL_1_ID = 600;			// 名将1技能ID
static const int GREATE_SOLDIER_SKILL_2_ID = 601;			// 名将2技能ID
static const int GREATE_SOLDIER_SKILL_3_ID = 602;			// 名将3技能ID
static const int GREATE_SOLDIER_SKILL_4_ID = 603;			// 名将特殊技能ID603
static const int GREATE_SOLDIER_SKILL_5_ID = 604;			// 名将特殊技能ID604
static const int GREATE_SOLDIER_SKILL_6_ID = 605;			// 名将特殊技能ID605
static const int GREATE_SOLDIER_SKILL_7_ID = 606;			// 名将特殊技能ID606

static const int WEDDINGRING_SKILL_ID_OFFSET = 80;			// 婚戒技能ID偏移量

static const int DEFAULT_AOE_ATTACK_RANGE = 10;				// 群攻默认攻击范围
static const int DEFAULT_AOE_ENEMY_NUM = 7;					// 群攻默认敌人数量

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum ROLE_SHADOW_SKILL_COND
{
	ROLE_SHADOW_SKILL_COND_0,								// 技能优先级0
	ROLE_SHADOW_SKILL_COND_1,								// 技能优先级1
	ROLE_SHADOW_SKILL_COND_2,								// 技能优先级2

	ROLE_SHADOW_SKILL_COND_MAX,
};

enum BOSS_SKILL_COND
{
	BOSS_SKILL_COND_HPLOW_PER = 0,							// 血量低于百分比
	BOSS_SKILL_COND_HPLOW_RANGE,							// 处于一定的血量范围
	BOSS_SKILL_COND_FIGHTTIMES,								// 战斗时间触发
	BOSS_SKILL_COND_ON_DIE,									// 死亡时触发
	BOSS_SKILL_COND_TOUCH,									// 触碰目标
	BOSS_SKILL_COND_NEAR_ROLE_DIE,							// 附近玩家死亡
	BOSS_SKILL_COND_BORNTIME,								// 出生时间触发
	BOSS_SKILL_COND_ANYWAY,									// 随时释放
	BOSS_SKILL_COND_MAX,
};

static const int BOSS_SKILL_TYPE_COND_COUNT = 8;			// 同样条件 配置的最大个数

static const int BOSS_SKILL_PARAM_COUNT = 4;				// 技能条件参数
static const int BOSS_SKILL_PARAM_SKILL_COUNT = 4;			// 技能条件 触发技能列表参数

struct BossSkillCondParam
{
	BossSkillCondParam()
		: cond_param_skill_count(0)
	{
		memset(cond_param_list, 0, sizeof(cond_param_list));
		memset(cond_param_skill_index_list, 0, sizeof(cond_param_skill_index_list));
	}

	int cond_param_list[BOSS_SKILL_PARAM_COUNT];							// 条件参数

	int cond_param_skill_count;												// 条件触发技能个数
	int cond_param_skill_index_list[BOSS_SKILL_PARAM_SKILL_COUNT];			// 条件触发技能列表
};

struct BossSkillTypeCond
{
	BossSkillTypeCond() : count(0) {}

	int count;
	BossSkillCondParam lists[BOSS_SKILL_TYPE_COND_COUNT];
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum SKILL_ATTACH_EFFECT_TARGET
{
	SKILL_ATTACH_EFFECT_TARGET_INVALID = 0,

	SKILL_ATTACH_EFFECT_TARGET_BOSS,						// BOSS
	SKILL_ATTACH_EFFECT_TARGET_MONSTER,						// 小怪
	SKILL_ATTACH_EFFECT_TARGET_ALL_MONSTER,					// 全部怪物
	SKILL_ATTACH_EFFECT_TARGET_ROLE_PET,					// 玩家及其宠物
	SKILL_ATTACH_EFFECT_TARGET_ALL_CHARACTER,				// 全部对象
	SKILL_ATTACH_EFFECT_TARGET_ENEMY,						// 敌人

	SKILL_ATTACH_EFFECT_TARGET_MAX,
};

enum SKILL_ATTACH_EFFECT
{
	SKILL_ATTACH_EFFECT_INVALID = 0,

	SKILL_ATTACH_EFFECT_XUANYUN,							// 眩晕
	SKILL_ATTACH_EFFECT_JITUI,								// 击退
	SKILL_ATTACH_EFFECT_CHENMO,								// 沉默
	SKILL_ATTACH_EFFECT_CHIHUAN,							// 迟缓

	SKILL_ATTACH_EFFECT_CALL_SAME_MONSTER,					// 召唤相同怪
	SKILL_ATTACH_EFFECT_CALL_DIFF_MONSTER,					// 召唤不同怪

	SKILL_ATTACH_EFFECT_INC_GONGJI,							// 增加攻击
	SKILL_ATTACH_EFFECT_DEC_GONGJI,							// 减少攻击
	SKILL_ATTACH_EFFECT_INC_FANGYU,							// 增加防御
	SKILL_ATTACH_EFFECT_DEC_FANGYU,							// 减少防御
	SKILL_ATTACH_EFFECT_INC_BAOJI,							// 增加暴击

	SKILL_ATTACH_EFFECT_ADD_HP,								// 回血
	SKILL_ATTACH_EFFECT_DUR_ADD_HP,							// 持续回血

	SKILL_ATTACH_EFFECT_WUDI_PROTECT,						// 无敌保护 使范围内的指定对象受到的所有伤害都为1

	SKILL_ATTACH_EFFECT_HURT_MAXHP_PERCENT,					// 对指定对象造成其最大气血值A%+B点伤害

	SKILL_ATTACH_EFFECT_KILL_SELF,							// 自杀

	SKILL_ATTACH_EFFECT_CHG_GLOBAL_CD,						// 改全局CD

	SKILL_ATTACH_EFFECT_HURT,								// 单次伤害

	SKILL_ATTACH_EFFECT_DUR_BLOOD_NOMERGE,					// 附加持续流血 不可叠加
	SKILL_ATTACH_EFFECT_DUR_BLOOD_MERGE,					// 附加持续流血 可叠加

	SKILL_ATTACH_EFFECT_DUR_GRADUAL_BLOOD,					// 附加持续渐增流血
	SKILL_ATTACH_EFFECT_FAZHEN_GRADUAL_BLOOD,				// 法阵渐增流血

	SKILL_ATTACH_EFFECT_REDUCE_MP,							// 减少内力点

	SKILL_ATTACH_EFFECT_RECOVER_ON_ATKED,					// 被击回血
	SKILL_ATTACH_EFFECT_REBOUND_HURT,						// 伤害反弹

	SKILL_ATTACH_EFFECT_JUHUAPIG_REBOUND,					// 菊花猪式反弹

	SKILL_ATTACH_EFFECT_CLEAR_FIGHT_EFFECT,					// 随机清除战斗buff
	SKILL_ATTACH_EFFECT_CLEAR_ALL_FIGHT_EFFECT,				// 清除所有战斗buff

	SKILL_ATTACH_EFFECT_ADD_HP_ON_ATTACK_OTHER,				// 攻击回血

	SKILL_ATTACH_EFFECT_BLAST,								// 自爆
	SKILL_ATTACH_EFFECT_SPECIAL_LOGIC,						// 回调场景特殊逻辑

	SKILL_ATTACH_SUPER_MIANYI,								// 免疫定身，昏迷，沉默，击退

	SKILL_ATTACH_DOUBLE_ATTR_BUFF,							// 双属性buff

	SKILL_ATTACH_ADD_HP,									// 攻击时增加被攻击者最大生命A%血量

	SKILL_ATTACH_ADD_SPECIAL_SHIELD,						// 增加固伤护盾

	SKILL_ATTACH_EFFECT_MAX,
};

enum SKILL_FAZHEN_EFFECT_TYPE
{
	SKILL_FAZHEN_EFFECT_TYPE_INVALID = 0,

	SKILL_FAZHEN_EFFECT_TYPE_FAZHEN,						// 法阵
	SKILL_FAZHEN_EFFECT_TYPE_HALO,							// 光环

	SKILL_FAZHEN_EFFECT_TYPE_MAX,
};

enum SKILL_EFFECT_SRC
{
	SKILL_EFFECT_SRC_SKILL = 0,								// 效果来源 技能
	SKILL_EFFECT_SRC_FAZHEN,								// 效果来源 法阵
	SKILL_EFFECT_SRC_HALO,									// 效果来源 光环
};

enum SKILL_PHASE
{
	SKILL_PHASE_READING = 0,								// 读条
	SKILL_PHASE_PERFORM,									// 释放
	SKILL_PHASE_CONTINUE_PERFORM,							// 持续施法
	SKILL_PHASE_END,										// 技能结束
};

enum SKILL_CHECK_FLAG
{
	SKILL_CHECK_FLAG_CD				= (0x1 << 0),			// 检查CD
	SKILL_CHECK_FLAG_TARGET			= (0x1 << 1),			// 检查目标
	SKILL_CHECK_FLAG_DISTANCE		= (0x1 << 2),			// 检查距离
};

enum SKILL_SPECIAL_LOGIC_TYPE
{
	SKILL_SPECIAL_LOGIC_TYPE_INVALID = 0,

	SKILL_SPECIAL_LOGIC_TYPE_TOWERDEFEND_SCORE = 1,			// 塔防积分

	SKILL_SPECIAL_LOGIC_TYPE_MAX,
};

static const int SKILL_ATTACH_MAX_EFFECT = 2;				// 技能附加效果
static const int SKILL_ATTACH_EFFECT_PARAM_COUNT = 6;		// 技能附加效果 参数个数
static const int SKILL_ATTACH_EFFECT_MAX_TARGET = 32;		// 技能效果目标最大数量

class Character;
class Scene;

struct SkillAttachEffectParam
{
	static const int FAZHEN_BUFF_DUR_MS = 1500;				// 法阵加的buff持续时间

	SkillAttachEffectParam() : effect_type(SKILL_ATTACH_EFFECT_INVALID)
	{
		memset(effect_param_list, 0, sizeof(effect_param_list));
	}

	bool CheckParamConfig(UInt16 skill_id, bool used_in_fazhen, bool is_range_skill, int effect_target, std::string &err);

	void Attach(Scene *scene, ObjID deliver_objid, const Posi &pos, short target_count, Character *target_list[SKILL_ATTACH_EFFECT_MAX_TARGET], 
				UInt16 skill_id, int effect_src = SKILL_EFFECT_SRC_SKILL, int param = 0) const;

	int effect_type;
	long long effect_param_list[SKILL_ATTACH_EFFECT_PARAM_COUNT];
};

#endif // __SKILLDEF_HPP__

