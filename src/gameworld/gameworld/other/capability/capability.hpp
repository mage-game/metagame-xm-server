#ifndef __CAPABILITY_HPP__
#define __CAPABILITY_HPP__

#include "obj/character/attribute.hpp"

class Role;

enum CAPABILITY_TYPE 
{
	CAPABILITY_TYPE_INVALID = 0,

	CAPABILITY_TYPE_BASE,				// 基础属性战斗力
	CAPABILITY_TYPE_MENTALITY,			// 元神属性战斗力
	CAPABILITY_TYPE_EQUIPMENT,			// 装备属性战斗力
	CAPABILITY_TYPE_WING,				// 羽翼属性战斗力
	CAPABILITY_TYPE_MOUNT,				// 坐骑属性战斗力
	CAPABILITY_TYPE_TITLE,				// 称号属性战斗力
	CAPABILITY_TYPE_SKILL,				// 技能属性战斗力
	CAPABILITY_TYPE_XIANJIAN,			// 仙剑属性战斗力
	CAPABILITY_TYPE_XIANSHU,			// 仙盟仙术属性战斗力
	CAPABILITY_TYPE_GEM,				// 宝石战斗力
	CAPABILITY_TYPE_XIANNV,				// 仙女属性战斗力
	CAPABILITY_TYPE_FOOTPRINT,			// 足迹属性战斗力
	CAPABILITY_TYPE_QINGYUAN,			// 情缘属性战斗力
	CAPABILITY_TYPE_ZHANSHENDIAN,		// 战神殿属性战斗力
	CAPABILITY_TYPE_SHIZHUANG,			// 时装属性战斗力
	CAPABILITY_TYPE_ATTR_PER,			// 基础属性百分比加的战斗力
	CAPABILITY_TYPE_LIEMING,			// 猎命装备战力
	CAPABILITY_TYPE_JINGLING,			// 精灵战力
	CAPABILITY_TYPE_VIPBUFF,			// vipbuff战力
	CAPABILITY_TYPE_SHENGWANG,			// 声望
	CAPABILITY_TYPE_CHENGJIU,			// 成就
	CAPABILITY_TYPE_WASH,				// 洗练
	CAPABILITY_TYPE_SHENZHUANG,			// 神装
	CAPABILITY_TYPE_TUHAOJIN ,			// 土豪金战力
	CAPABILITY_TYPE_BIG_CHATFACE,		// 大表情战力
	CAPABILITY_TYPE_SHENZHOU_WEAPON,	// 神州六器战斗力
	CAPABILITY_TYPE_BABY,				// 宝宝属性战斗力
	CAPABILITY_TYPE_PET,				// 宠物战力
	CAPABILITY_TYPE_ACTIVITY,			// 活动相关提升的战力
	CAPABILITY_TYPE_HUASHEN,			// 化神战力
	CAPABILITY_TYPE_MULTIMOUNT,			// 双人坐骑战力
	CAPABILITY_TYPE_PERSONALIZE_WINDOW,	//个性聊天框战力
	CAPABILITY_TYPE_MAGIC_CARD,			// 魔卡战斗力
	CAPABILITY_TYPE_MITAMA,				// 御魂战力
	CAPABILITY_TYPE_XUNZHANG,			// 勋章战力
	CAPABILITY_TYPE_ZHIBAO,				// 至宝战力
	CAPABILITY_TYPE_HALO,				// 光环属性战斗力
	CAPABILITY_TYPE_SHENGONG,			// 神弓属性战斗力
	CAPABILITY_TYPE_SHENYI,				// 神翼属性战斗力
	CAPABILITY_TYPE_GUILD,				// 仙盟战斗力
	CAPABILITY_TYPE_CHINESE_ZODIAC,		// 星座系统战斗力
	CAPABILITY_TYPE_XIANNV_SHOUHU,		// 仙女守护战斗力
	CAPABILITY_TYPE_JINGLING_GUANGHUAN,	// 精灵光环战斗力
	CAPABILITY_TYPE_JINGLING_FAZHEN,	// 精灵法阵战斗力
	CAPABILITY_TYPE_CARDZU,				// 卡牌组合战力
	CAPABILITY_TYPE_ZHUANSHENGEQUIP,    // 转生属性战斗力
	CAPABILITY_TYPE_LITTLE_PET,			// 小宠物战力
	CAPABILITY_TYPE_ZHUANSHEN_RAND_ATTR ,// 转生装备随机属性
	CAPABILITY_TYPE_FIGHT_MOUNT,		// 战斗坐骑战斗力
	CAPABILITY_TYPE_MOJIE,				// 魔戒
	CAPABILITY_TYPE_LOVE_TREE,			// 相思树
	CAPABILITY_TYPE_EQUIPSUIT,			// 锻造套装战斗力
	CAPABILITY_TYPE_RUNE_SYSTEM,		// 符文系统
	CAPABILITY_TYPE_SHENGE_SYSTEM,		// 神格系统
	CAPABILITY_TYPE_SHENBING,			// 神兵系统
	CAPABILITY_TYPE_FABAO,			    // 法宝属性战斗力
	CAPABILITY_TYPE_ROLE_GOAL,			// 角色目标
	CAPABILITY_TYPE_CLOAK,				// 披风属性战斗力
	CAPABILITY_TYPE_SHENSHOU,			// 神兽战斗力
	CAPABILITY_TYPE_IMG_FULING,			// 形象赋灵
	CAPABILITY_TYPE_CSA_EQUIP,			// 合服装备
	CAPABILITY_TYPE_SHEN_YIN,			// 神印
	CAPABILITY_TYPE_ELEMENT_HEART,		// 元素之心战力
	CAPABILITY_TYPE_FEIXIAN,			// 飞仙战斗力
	CAPABILITY_TYPE_SHIZHUANG_WUQI,		// 时装武器（神兵）属性战斗力
	CAPABILITY_TYPE_CF_BEST_RANK_BREAK,	// 竞技场历史最高排名突破
	CAPABILITY_TYPE_SHENGQI,			// 圣器系统
	CAPABILITY_TYPE_JINGJIE,            // 境界系统
	CAPABILITY_TYPE_TALENT,				// 天赋系统
	CAPABILITY_TYPE_JINJIE_TALENT,		// 进阶天赋系统
	CAPABILITY_TYPE_HESHENLUOSHU,		// 河神洛书
	CAPABILITY_TYPE_IMP_GUARD,          // 小鬼守护
	CAPABILITY_TYPE_ZHUANZHI,           // 人物转职
	CAPABILITY_TYPE_BOSS_CARD,          // boss图鉴
	CAPABILITY_TYPE_YAOSHI,				// 腰饰战力
	CAPABILITY_TYPE_TOUSHI,				// 头饰战力
	CAPABILITY_TYPE_QILINBI,			// 麒麟臂战力
	CAPABILITY_TYPE_MASK,				// 面具战力
	CAPABILITY_TYPE_ZHUANZHI_EQUIP,		// 转职装备战力
	CAPABILITY_TYPE_ROLE_CROSS,			// 人物跨服
	CAPABILITY_TYPE_SEAL,				// 圣印系统
	CAPABILITY_TYPE_DRESSING_ROOM,		// 衣柜战力
	CAPABILITY_TYPE_SHENQI,				// 神器系统
	CAPABILITY_TYPE_RED_EQUIP_COLLECT,  // 红装收集
	CAPABILITY_TYPE_ORANGE_EQUIP_COLLET,// 橙装收集
	CAPABILITY_TYPE_MOLONG,             // 魔龙
	CAPABILITY_TYPE_SPOUSEHOME,         // 夫妻家园
	CAPABILITY_TYPE_EQUIP_BAPTIZE,      // 装备洗炼
	CAPABILITY_TYPE_MOJIE_SKILL,		// 魔戒技能(class MojieSkill)
	CAPABILITY_TYPE_GREATE_SOLDIER,		// 名将
	CAPABILITY_TYPE_BAIZHAN_EQUIP,		// 百战装备战力
	CAPABILITY_TYPE_XIANZUNKA,			// 仙尊卡
	CAPABILITY_TYPE_MYTH,				// 神话系统
	CAPABILITY_TYPE_ZODIAC,				// 十二生肖
	CAPABILITY_TYPE_CROSSEQUIP,			// 斗气装备
	CAPABILITY_TYPE_TIANSHENHUTI,		// 周末装备(天神护体)战力
	CAPABILITY_TYPE_TIANSHUXUNZHU,		// 天书寻主

	CAPABILITY_TYPE_UPGRADE_SYS_BEGIN = 200,	// 进阶系统战力 (200 + upgrade_type)
	CAPABILITY_TYPE_UPGRADE_SYS_END = 230,

	CAPABILITY_TYPE_TOTAL,				// 总战斗力，(战斗力计算方式改为所有属性算好后再套公式计算，取消各个模块分别计算再加起来的方式）

	CAPABILITY_TYPE_MAX,
};

class Capability
{
public:
	Capability();
	~Capability();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, int max_cap);

	void OnRoleReCalcAttrBegin(bool recalc_all);
	void OnRoleReCalcAttrEnd(bool notify = true, bool is_time_limit = false);
	void OnGetRoleCapa();

	void ReCalcCap(int cap_type, const CharIntAttrs &diff_int_attrs);
	void AddCap(int cap_type, int cap);
	void SetCap(int cap_type, int cap);

	int GetRoleCapability(int cap_type);
	int GetTotalCapability();
	int GetMaxCapablity() { return m_max_cap; }
	int GetOtherCapablity() { return m_other_cap; }

	static int GetCapByAttr(const CharIntAttrs &diff_int_attrs, bool is_total = false);

	void GmSetCapability(int capability);
	void SendRoleCapability();

	static int GetHistoryType(int cap_type);

	void OnDayChange();
	void CheckClearAddCap();

private:
	Role *m_role;
	
	int m_total_cap;
	int m_gm_total_cap;
	int m_max_cap;
	int m_other_cap;
	int m_cap_list[CAPABILITY_TYPE_MAX]; 
};

#endif // __CAPABILITY_HPP__

