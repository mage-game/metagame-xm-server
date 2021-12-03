#ifndef __TALENT_DEF_HPP__
#define __TALENT_DEF_HPP__

#include "servercommon.h"

enum TALENT_ATTR_TYPE
{
	TALENT_ATTR_TYPE_INVALID = 0,

	TALENT_ATTR_TYPE_MAXHP,						// 角色气血
	TALENT_ATTR_TYPE_GONGJI,					// 角色物攻/法攻
	TALENT_ATTR_TYPE_FANGYU,					// 角色防御
	TALENT_ATTR_TYPE_BAOJI,						// 角色暴击
	TALENT_ATTR_TYPE_BAOJI_PER,					// 角色暴击（暴击伤害率）
	TALENT_ATTR_TYPE_JIANREN,					// 角色坚韧(抗暴)
	TALENT_ATTR_TYPE_POFANG,					// 角色破防百分比（增伤率）

	TALENT_ATTR_TYPE_PVP_JIANSHANG_PER,			// pvp减伤万分比
	TALENT_ATTR_TYPE_PVP_ZENGSHANG_PER,         // pvp增伤万分比
	TALENT_ATTR_TYPE_PVE_JIANSHANG_PER,         // pve减伤万分比
	TALENT_ATTR_TYPE_PVE_ZENGSHANG_PER,         // pve增伤万分比

	TALENT_ATTR_TYPE_FABAO_GONGJI_PER,			// 法宝攻击万分比
	TALENT_ATTR_TYPE_FABAO_FANGYU_PER,			// 法宝防御万分比
	TALENT_ATTR_TYPE_FIGHT_MOUNT_GONGJI_PER,	// 战斗坐骑攻击万分比
	TALENT_ATTR_TYPE_FIGHT_MOUNT_FANGYU_PER,	// 战斗坐骑防御万分比
	TALENT_ATTR_TYPE_MOUNT_GONGJI_PER,			// 坐骑攻击万分比
	TALENT_ATTR_TYPE_MOUNT_FANGYU_PER,			// 坐骑防御万分比
	TALENT_ATTR_TYPE_SHENBING_GONGJI_PER,		// 神兵攻击万分比（时装武器）
	TALENT_ATTR_TYPE_SHENBING_FANGYU_PER,		// 神兵防御万分比（时装武器）
	TALENT_ATTR_TYPE_WING_GONGJI_PER,			// 羽翼攻击万分比
	TALENT_ATTR_TYPE_WING_FANGYU_PER,			// 羽翼防御万分比
	TALENT_ATTR_TYPE_HALO_GONGJI_PER,			// 光环攻击万分比
	TALENT_ATTR_TYPE_HALO_FANGYU_PER,			// 光环防御万分比
	TALENT_ATTR_TYPE_SHENYI_GONGJI_PER,			// 神翼攻击万分比
	TALENT_ATTR_TYPE_SHENYI_FANGYU_PER,			// 神翼防御万分比
	TALENT_ATTR_TYPE_SHENGONG_GONGJI_PER,		// 神弓攻击万分比
	TALENT_ATTR_TYPE_SHENGONG_FANGYU_PER,		// 神弓防御万分比
	TALENT_ATTR_TYPE_SHIZHUANG_GONGJI_PER,		// 时装攻击万分比
	TALENT_ATTR_TYPE_SHIZHUANG_FANGYU_PER,		// 时装防御万分比
	TALENT_ATTR_TYPE_FOOTPRINT_GONGJI_PER,		// 足迹攻击万分比
	TALENT_ATTR_TYPE_FOOTPRINT_FANGYU_PER,		// 足迹防御万分比
	TALENT_ATTR_TYPE_SHENSHOU_GONGJI_PER,		// 神兽攻击万分比(龙器)
	TALENT_ATTR_TYPE_SHENSHOU_FANGYU_PER,		// 神兽防御万分比(龙器)
	TALENT_ATTR_TYPE_SHENSHOU_ALL_PER,			// 神兽全属性万分比(龙器)
	TALENT_ATTR_TYPE_YUSHI_GONGJI_PER,			// 玉石攻击万分比
	TALENT_ATTR_TYPE_YUSHI_FANGYU_PER,			// 玉石防御万分比
	TALENT_ATTR_TYPE_YUSHI_ALL_PER,				// 玉石全属性万分比
	TALENT_ATTR_TYPE_STONE_GONGJI_PER,			// 宝石攻击万分比
	TALENT_ATTR_TYPE_STONE_FANGYU_PER,			// 宝石防御万分比
	TALENT_ATTR_TYPE_STONE_ALL_PER,				// 宝石全属性万分比
	TALENT_ATTR_TYPE_NORMAL_EQUIP_GONGJI_PER,	// 普通装备攻击万分比
	TALENT_ATTR_TYPE_NORMAL_EQUIP_FANGYU_PER,	// 普通装备防御万分比
	TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_GONGJI_PER,	// 转职装备攻击万分比
	TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_FANGYU_PER,	// 转职装备防御万分比
	TALENT_ATTR_TYPE_YONGHENG_SUIT_PER,			// 永恒套装属性万分比
	TALENT_ATTR_TYPE_ZHUXIAN_SUIT_PER,			// 诛仙套装属性万分比
	TALENT_ATTR_TYPE_ZHUSHEN_SUIT_PER,			// 诛神套装属性万分比

	TALENT_ATTR_TYPE_SKILL_HURT_ADD_PER,		// 技能伤害增加万分比
	TALENT_ATTR_TYPE_SKILL_HURT_MINUS_PER,		// 技能伤害减少万分比

	TALENT_ATTR_TYPE_SKILL_ZENGSHANG_PER,		// 技能增伤万分比
	TALENT_ATTR_TYPE_SKILL_JIANSHANG_PER,		// 技能减伤万分比

	TALENT_ATTR_TYPE_QILINBI_UPGRADE_PER,		// 麒麟臂进阶属性万分比
	TALENT_ATTR_TYPE_WING_UPGRADE_PER,			// 羽翼进阶属性万分比
	TALENT_ATTR_TYPE_FABAO_UPGRADE_PER,			// 法宝进阶属性万分比
	TALENT_ATTR_TYPE_LINGTONG_UPGRADE_PER,		// 灵童进阶属性万分比
	TALENT_ATTR_TYPE_MOUNT_UPGRADE_PER,			// 坐骑进阶属性万分比
	TALENT_ATTR_TYPE_SHENGBING_UPGRADE_PER,		// 神兵进阶属性万分比
	TALENT_ATTR_TYPE_SHIZHUANG_UPGRADE_PER,		// 时装进阶属性万分比
	TALENT_ATTR_TYPE_FIGHT_MOUNT_UPGRADE_PER,	// 战骑进阶属性万分比
	TALENT_ATTR_TYPE_SHENGONG_UPGRADE_PER,		// 神弓进阶属性万分比（仙女光环）
	TALENT_ATTR_TYPE_LINGGONG_UPGRADE_PER,		// 灵弓进阶属性万分比
	TALENT_ATTR_TYPE_TOUSHI_UPGRADE_PER,		// 头饰进阶属性万分比
	TALENT_ATTR_TYPE_LINGQI_UPGRADE_PER,		// 灵骑进阶属性万分比
	TALENT_ATTR_TYPE_YAOSHI_UPGRADE_PER,		// 腰饰进阶属性万分比
	TALENT_ATTR_TYPE_HALO_UPGRADE_PER,			// 光环进阶属性万分比
	TALENT_ATTR_TYPE_MASK_UPGRADE_PER,			// 面饰进阶属性万分比
	TALENT_ATTR_TYPE_WEIYAN_UPGRADE_PER,		// 尾焰进阶属性万分比

	TALENT_ATTR_TYPE_EQUIP_BASE_PER,			// 普通装备属性万分比
	TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_BASE_PER,	// 转职装备属性万分比

	TALENT_ATTR_TYPE_MAX,
};

struct ConfigTalentAttrDefineTypeString
{
	char talent_attr_name[32];
	short talent_attr_type;
};

const static ConfigTalentAttrDefineTypeString gs_config_talent_attr_define_string_array[] =
{
	{ "role_maxhp", TALENT_ATTR_TYPE_MAXHP },								// 角色最大血量
	{ "role_gongji", TALENT_ATTR_TYPE_GONGJI },								// 角色攻击
	{ "role_fangyu", TALENT_ATTR_TYPE_FANGYU },								// 角色防御
	{ "role_baoji", TALENT_ATTR_TYPE_BAOJI },								// 角色暴击
	{ "role_baoji_per", TALENT_ATTR_TYPE_BAOJI_PER },						// 角色暴击伤害
	{ "role_jianren", TALENT_ATTR_TYPE_JIANREN },							// 角色坚韧(抗暴)
	{ "role_pofang", TALENT_ATTR_TYPE_POFANG},								// 破防百分比(增伤率)

	{ "pvp_mianshang_per", TALENT_ATTR_TYPE_PVP_JIANSHANG_PER},			// pvp减伤万分比
	{ "pvp_zengshang_per", TALENT_ATTR_TYPE_PVP_ZENGSHANG_PER},         // pvp增伤万分比
	{ "pve_mianshang_per", TALENT_ATTR_TYPE_PVE_JIANSHANG_PER},			// pve减伤万分比
	{ "pve_zengshang_per", TALENT_ATTR_TYPE_PVE_ZENGSHANG_PER},         // pve增伤万分比

	{ "fight_mount_gongji_per", TALENT_ATTR_TYPE_FIGHT_MOUNT_GONGJI_PER},	// 战斗坐骑攻击万分比
	{ "fight_mount_fangyu_per", TALENT_ATTR_TYPE_FIGHT_MOUNT_FANGYU_PER},	// 战斗坐骑防御万分比
	{ "wing_gongji_per", TALENT_ATTR_TYPE_WING_GONGJI_PER},					// 羽翼攻击万分比
	{ "wing_fangyu_per", TALENT_ATTR_TYPE_WING_FANGYU_PER},					// 羽翼防御万分比
	{ "halo_gongji_per", TALENT_ATTR_TYPE_HALO_GONGJI_PER},					// 光环攻击万分比
	{ "halo_fangyu_per", TALENT_ATTR_TYPE_HALO_FANGYU_PER},					// 光环防御万分比
	{ "fabao_gongji_per", TALENT_ATTR_TYPE_FABAO_GONGJI_PER },				// 法宝攻击万分比
	{ "fabao_fangyu_per", TALENT_ATTR_TYPE_FABAO_FANGYU_PER },				// 法宝防御万分比
	{ "mount_gongji_per", TALENT_ATTR_TYPE_MOUNT_GONGJI_PER },				// 坐骑攻击万分比
	{ "mount_fangyu_per", TALENT_ATTR_TYPE_MOUNT_FANGYU_PER },				// 坐骑防御万分比
	{ "shenbing_gongji_per", TALENT_ATTR_TYPE_SHENBING_GONGJI_PER },		// 神兵攻击万分比
	{ "shenbing_fangyu_per", TALENT_ATTR_TYPE_SHENBING_FANGYU_PER },		// 神兵防御万分比
	{ "shenyi_gongji_per", TALENT_ATTR_TYPE_SHENYI_GONGJI_PER },			// 神翼攻击万分比(仙女系统法阵)
	{ "shenyi_fangyu_per", TALENT_ATTR_TYPE_SHENYI_FANGYU_PER },			// 神翼防御万分比
	{ "shengong_gongji_per", TALENT_ATTR_TYPE_SHENGONG_GONGJI_PER },		// 神弓攻击万分比(仙女系统光环)
	{ "shengong_fangyu_per", TALENT_ATTR_TYPE_SHENGONG_FANGYU_PER },		// 神弓防御万分比
	{ "shizhuang_gongji_per", TALENT_ATTR_TYPE_SHIZHUANG_GONGJI_PER },		// 时装攻击万分比
	{ "shizhuang_fangyu_per", TALENT_ATTR_TYPE_SHIZHUANG_FANGYU_PER },		// 时装防御万分比
	{ "footprint_gongji_per", TALENT_ATTR_TYPE_FOOTPRINT_GONGJI_PER },		// 足迹攻击万分比
	{ "footprint_fangyu_per", TALENT_ATTR_TYPE_FOOTPRINT_FANGYU_PER },		// 足迹防御万分比
	{ "shenshou_gongji_per", TALENT_ATTR_TYPE_SHENSHOU_GONGJI_PER },		// 神兽攻击万分比(龙器)
	{ "shenshou_fangyu_per", TALENT_ATTR_TYPE_SHENSHOU_FANGYU_PER },		// 神兽防御万分比(龙器)
	{ "shenshou_all_per", TALENT_ATTR_TYPE_SHENSHOU_ALL_PER },				// 神兽全属性万分比(龙器)
	{ "yushi_gongji_per", TALENT_ATTR_TYPE_YUSHI_GONGJI_PER },				// 玉石攻击万分比
	{ "yushi_fangyu_per", TALENT_ATTR_TYPE_YUSHI_FANGYU_PER },				// 玉石防御万分比
	{ "yushi_all_per", TALENT_ATTR_TYPE_YUSHI_ALL_PER },					// 玉石全属性万分比
	{ "stone_gongji_per", TALENT_ATTR_TYPE_STONE_GONGJI_PER },				// 宝石攻击万分比
	{ "stone_fangyu_per", TALENT_ATTR_TYPE_STONE_FANGYU_PER },				// 宝石防御万分比
	{ "stone_all_per", TALENT_ATTR_TYPE_STONE_ALL_PER },					// 宝石全属性万分比
	{ "normal_equip_gongji_per", TALENT_ATTR_TYPE_NORMAL_EQUIP_GONGJI_PER },		// 普通装备攻击万分比
	{ "normal_equip_fangyu_per", TALENT_ATTR_TYPE_NORMAL_EQUIP_FANGYU_PER },		// 普通装备防御万分比
	{ "zhuanzhi_equip_gongji_per", TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_GONGJI_PER },	// 转职装备攻击万分比
	{ "zhuanzhi_equip_fangyu_per", TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_FANGYU_PER },	// 转职装备防御万分比
	{ "yushi_gongji_per", TALENT_ATTR_TYPE_YUSHI_GONGJI_PER },						// 玉石攻击万分比
	{ "yushi_fangyu_per", TALENT_ATTR_TYPE_YUSHI_FANGYU_PER },						// 玉石防御万分比
	{ "yongheng_suit_per", TALENT_ATTR_TYPE_YONGHENG_SUIT_PER},						// 永恒套装属性万分比
	{ "zhuxian_suit_per", TALENT_ATTR_TYPE_ZHUXIAN_SUIT_PER },						// 诛仙套装属性万分比
	{ "zhushen_suit_per", TALENT_ATTR_TYPE_ZHUSHEN_SUIT_PER },						// 诛神套装属性万分比

	{ "skill_hurt_add_per", TALENT_ATTR_TYPE_SKILL_HURT_ADD_PER },					// 技能伤害增加万分比
	{ "skill_hurt_minus_per", TALENT_ATTR_TYPE_SKILL_HURT_MINUS_PER },				// 技能伤害减少万分比

	{ "skill_zengshang_per", TALENT_ATTR_TYPE_SKILL_ZENGSHANG_PER },				// 技能增伤万分比
	{ "skill_jianshang_per", TALENT_ATTR_TYPE_SKILL_JIANSHANG_PER },				// 技能减伤万分比

	{ "qilingbi_upgrade_per", TALENT_ATTR_TYPE_QILINBI_UPGRADE_PER },				// 麒麟臂进阶属性万分比
	{ "wing_upgrade_per", TALENT_ATTR_TYPE_WING_UPGRADE_PER },						// 羽翼进阶属性万分比
	{ "fabao_upgrade_per", TALENT_ATTR_TYPE_FABAO_UPGRADE_PER },					// 法宝进阶属性万分比
	{ "lingtong_upgrade_per", TALENT_ATTR_TYPE_LINGTONG_UPGRADE_PER },				// 灵童进阶属性万分比
	{ "mount_upgrade_per", TALENT_ATTR_TYPE_MOUNT_UPGRADE_PER },					// 坐骑进阶属性万分比
	{ "shenbing_upgrade_per", TALENT_ATTR_TYPE_SHENGBING_UPGRADE_PER },				// 神兵进阶属性万分比
	{ "shizhuang_upgrade_per", TALENT_ATTR_TYPE_SHIZHUANG_UPGRADE_PER },			// 时装进阶属性万分比
	{ "fight_mount_upgrade_per", TALENT_ATTR_TYPE_FIGHT_MOUNT_UPGRADE_PER },		// 战骑进阶属性万分比
	{ "shengong_upgrade_per", TALENT_ATTR_TYPE_SHENGONG_UPGRADE_PER },				// 神弓进阶属性万分比（仙女光环）
	{ "linggong_upgrade_per", TALENT_ATTR_TYPE_LINGGONG_UPGRADE_PER },				// 灵弓进阶属性万分比
	{ "toushi_upgrade_per", TALENT_ATTR_TYPE_TOUSHI_UPGRADE_PER },				    // 头饰进阶属性万分比
	{ "lingqi_upgrade_per", TALENT_ATTR_TYPE_LINGQI_UPGRADE_PER },				    // 灵骑进阶属性万分比
	{ "yaoshi_upgrade_per", TALENT_ATTR_TYPE_YAOSHI_UPGRADE_PER },				    // 腰饰进阶属性万分比
	{ "halo_upgrade_per", TALENT_ATTR_TYPE_HALO_UPGRADE_PER },					    // 光环进阶属性万分比
	{ "mask_upgrade_per", TALENT_ATTR_TYPE_MASK_UPGRADE_PER },						// 面饰进阶属性万分比
	{ "weiyan_upgrade_per", TALENT_ATTR_TYPE_WEIYAN_UPGRADE_PER },					// 尾焰进阶属性万分比

	{ "equip_base_per", TALENT_ATTR_TYPE_EQUIP_BASE_PER },							// 普通装备属性万分比
	{ "zhuanzhi_equip_base_per", TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_BASE_PER },		// 转职装备属性万分比
	
	{ "0", TALENT_ATTR_TYPE_INVALID }
};

enum TALENT_OTHER_MODULE
{
	TALENT_OTHER_MODULE_INVALID,
	TALENT_OTHER_MODULE_FABAO,
	TALENT_OTHER_MODULE_FIGHT_MOUNT,
	TALENT_OTHER_MODULE_MOUNT,
	TALENT_OTHER_MODULE_WING,			// 羽翼
	TALENT_OTHER_MODULE_HALO,			// 光环
	TALENT_OTHER_MODULE_SHENBING,		// 神兵
	TALENT_OTHER_MODULE_SHENYI,
	TALENT_OTHER_MODULE_SHENGONG,
	TALENT_OTHER_MODULE_SHIZHUANG,
	TALENT_OTHER_MODULE_FOOTPRINT,
	TALENT_OTHER_MODULE_SHENSHOU,
	TALENT_OTHER_MODULE_YUSHI,
	TALENT_OTHER_MODULE_STONE,
	TALENT_OTHER_MODULE_YONGHENG_SUIT,
	TALENT_OTHER_MODULE_ZHUXIAN_SUIT,
	TALENT_OTHER_MODULE_ZHUSHEN_SUIT,
	TALENT_OTHER_MODULE_NORMAL_EQUIP,
	TALENT_OTHER_MODULE_ZHUANZHI_EQUIP,
	TALENT_OTHER_MODULE_FABAO_BASE,
	TALENT_OTHER_MODULE_FIGHT_MOUNT_BASE,
	TALENT_OTHER_MODULE_MOUNT_BASE,
	TALENT_OTHER_MODULE_WING_BASE,
	TALENT_OTHER_MODULE_HALO_BASE,
	TALENT_OTHER_MODULE_SHENBING_BASE,
	TALENT_OTHER_MODULE_SHENGONG_BASE,
	TALENT_OTHER_MODULE_SHIZHUANG_BASE,
	TALENT_OTHER_MODULE_QILINBI_BASE,
	TALENT_OTHER_MODULE_MASK_BASE,
	TALENT_OTHER_MODULE_TOUSHI_BASE,
	TALENT_OTHER_MODULE_YAOSHI_BASE,
	TALENT_OTHER_MODULE_NORMAL_EQUIP_BASE,
	TALENT_OTHER_MODULE_ZHUANZHI_EQUIP_BASE,

	TALENT_OTHER_MODULE_UPGRADE_0 = 200,  // 灵珠
	TALENT_OTHER_MODULE_UPGRADE_1,		  // 仙宝
	TALENT_OTHER_MODULE_UPGRADE_2,		  // 灵童
	TALENT_OTHER_MODULE_UPGRADE_3,		  // 灵弓
	TALENT_OTHER_MODULE_UPGRADE_4,        // 灵骑
	TALENT_OTHER_MODULE_UPGRADE_5,        // 尾焰
	TALENT_OTHER_MODULE_UPGRADE_6,        // 手环
	TALENT_OTHER_MODULE_UPGRADE_7,        // 尾巴
	TALENT_OTHER_MODULE_UPGRADE_8,        // 飞宠
};

enum TALENT_TYPE
{
	TALENT_TYPE_INVALID = 0,

	TALENT_TYPE_ROLE_GONGJI,            // 角色攻击系
	TALENT_TYPE_ROLE_FANGYU,			// 角色防御系
	TALENT_TYPE_LINGTONG,				// 灵宠系
	TALENT_TYPE_PROFICIENT,				// 精通系

	TALENT_TYPE_MAX,
};

enum TALENT_EFFECT_TYPE
{
	TALENT_EFFECT_TYPE_ATTR = 0,		// 属性类
	TALENT_EFFECT_TYPE_SKILL,           // 技能类
};

static const int MAX_TELENT_TYPE_COUT = 4;
static const int MAX_TELENT_INDEX_COUT = 20;
static const int ROLE_TALENT_LEVEL_MAX = 50;
const static int RECALC_REASON_STONE = 10;

static const int SPECIAL_TALENT_ID_107 = 107;	// 摄魂夺魄	攻击时有万分之param_a的概率使玩家直接损失最大生命值的万分之param_b
static const int SPECIAL_TALENT_ID_108 = 108;	// 战争热忱	攻击时有万分之param_d的概率对玩家造成攻击时增加万分之param_a的伤害，最大叠加param_b层，持续param_c毫秒
static const int SPECIAL_TALENT_ID_109 = 109;	// 杀意波动	击杀玩家后param_b秒内伤害提升万分之param_a，冷却时间20秒
static const int SPECIAL_TALENT_ID_110 = 110;	// 烈焰噬骨	攻击时有万分之param_a对玩家造成流血效果，每秒损失我方攻击力万分之param_b的血量，持续param_c毫秒，冷却时间5秒
static const int SPECIAL_TALENT_ID_414 = 414; 	// 吸星：攻击时，有万分之param_a的几率使当次攻击所造成伤害的万分之param_b转化为自己的生命值，冷却时间5S。
static const int SPECIAL_TALENT_ID_412 = 412; 	// 摄心：攻击时有万分之param_a几率使对方进入禁疗状态，param_b毫秒之内无法使用任何方法回复气血，冷却时间60秒。
static const int SPECIAL_TALENT_ID_115 = 115;	// 炙心：攻击时有万分之param_a概率附带param_b点真实伤害
static const int SPECIAL_TALENT_ID_310 = 310;	// 追魂夺魄：攻击时有万分之param_a的几率增加万分之param_b攻击，持续param_c毫秒，冷却时间X秒

static const int SPECIAL_TALENT_ID_206 = 206; 	// 荆棘之甲	受到伤害时万分之param_a的几率反弹param_b点真实伤害
static const int SPECIAL_TALENT_ID_207 = 207; 	// 剑结雷池	受到伤害时有万分之param_a的概率使攻击的玩家直接损失万分之param_b的最大生命值
static const int SPECIAL_TALENT_ID_210 = 210; 	// 鲜血沸腾	受到玩家攻击时，有万分之param_a的概率回复最大生命值万分之param_b的血量
static const int SPECIAL_TALENT_ID_311 = 311; 	// 登峰造极：受到攻击时万分之param_a几率增加万分之param_b防御，持续param_c毫秒，冷却时间30秒
static const int SPECIAL_TALENT_ID_312 = 312; 	// 巧夺天工：被攻击时有万分之param_a概率吸收受到伤害的万分之param_b转换成自身气血，冷却时间10秒
static const int SPECIAL_TALENT_ID_417 = 417; 	// 造化：受到攻击时，有万分之param_a几率使自己进入万物复生状态，无敌param_b毫秒并吸收这param_b毫秒内所受伤害的万分之param_c转化为自己的生命值，冷却时间60秒。

static const int SPECIAL_TALENT_ID_208 = 208; 	// 不屈意志	每受到玩家伤害时万分之param_d的概率降低万分之param_a的伤害，最大叠加param_b层，持续param_c毫秒
static const int SPECIAL_TALENT_ID_209 = 209; 	// 诸邪辟易	击杀玩家后param_b毫秒内受到的伤害减少万分之param_a，冷却时间20秒

static const int SPECIAL_TALENT_ID_306 = 306; 	// 震荡法箭 攻击时有万分之param_a几率释放强力技能，对param_b个敌人造成攻击的万分之param_c的伤害

UNSTD_STATIC_CHECK(MAX_TELENT_TYPE_COUT == TALENT_TYPE_MAX - 1)
UNSTD_STATIC_CHECK(MAX_TELENT_INDEX_COUT < 32)

#pragma pack(push, 4)

struct TalentParam
{
	TalentParam() { this->Reset(); }

	void Reset()
	{
		talent_point = 0;
		talent_book_add_point = 0;
		reserve_sh = 0;

		memset(talent_level_list, 0, sizeof(talent_level_list));
		memset(talent_first_point_flag, 0, sizeof(talent_first_point_flag));
	}

	int talent_point;														// 天赋点数
	unsigned short talent_book_add_point;									// 天赋技能书增加的点数
	short reserve_sh;

	char talent_level_list[MAX_TELENT_TYPE_COUT][MAX_TELENT_INDEX_COUT];	// 天赋等级
	int talent_first_point_flag[MAX_TELENT_TYPE_COUT];						// 天赋首次加点记录
};

typedef char TalentParamHex[sizeof(TalentParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(TalentParamHex) < 256);

#pragma pack(pop)

#endif
