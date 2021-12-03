#ifndef __NOREX_ITEM_H__
#define __NOREX_ITEM_H__

#include "item/itembase.h"

class NorexItem : public ItemBase
{
public:
	enum NOREX_ITEM_TYPE
	{
		I_NOREX_ITEM_INVALID = 0,

		I_NOREX_ITEM_JINGYAN_DAN = 1,							// 经验丹
		I_NOREX_ITEM_BIND_COIN_CARD = 2,						// 绑定铜钱卡
		I_NOREX_ITEM_TITLE_CARD = 3,							// 称号卡 
		I_NOREX_ITEM_RESERVE_1 = 4,								// 保留
		I_NOREX_ITEM_XIANHUN_DAN = 5,							// 仙魂丹 
		I_NOREX_ITEM_SHENGJI_DAN = 6,							// 升级丹 
		I_NOREX_ITEM_YUANLI_DAN = 7,							// 元力丹 
		I_NOREX_ITEM_GOLD_BIND_CARD = 8,						// 绑定元宝卡
		I_NOREX_ITEM_WING_BLESS_CARD = 9,						// 羽翼经验卡
		I_NOREX_ITEM_TILI_CARD = 10,							// 体力卡
		I_NOREX_ITEM_FB_EQUIP_CARD = 11,						// 新装备本卡
		I_NOREX_ITEM_WING_SPECIAL_IMG_CARD = 12,				// 羽翼特殊形象卡
		I_NOREX_ITEM_SPECIAL_MOUNT_IMG_CARD = 13,				// 坐骑特殊形象卡
		I_NOREX_ITEM_SPECIAL_APPEARANCE_CARD = 14,				// 特殊形象卡
		I_NOREX_ITEM_MOUNT_JINJIE_CARD = 15,					// 坐骑进阶卡
		I_NOREX_ITEM_REDUCE_EVIL_CARD = 16,						// 减少罪恶卡
		I_NOREX_ITEM_QIBING_JINJIE_CARD = 17,					// 骑兵进阶卡
		I_NOREX_ITEM_SHIZHUANG_CARD = 18,						// 时装卡
		I_NOREX_ITEM_RED_PAPER_CARD = 19,						// 红包卡
		I_NOREX_ITEM_SHUXINGDAN = 20,							// 属性丹
		I_NOREX_ITEM_HUNLI = 21,								// 魂力
		I_NOREX_ITEM_LINGJING = 22,								// 灵精
		I_NOREX_ITEM_MOJING = 23,								// 魔晶
		I_NOREX_ITEM_COIN_CARD = 24,							// 非绑铜钱卡
		I_NOREX_ITEM_SHENGWANG = 25,							// 声望丹
		I_NOREX_ITEM_CHENGJIU = 26,								// 成就丹
		I_NOREX_ITEM_HP = 27,									// 气血瞬回
		I_NOREX_ITEM_HP_CONTINUE = 28,							// 气血持续回复
		I_NOREX_ITEM_MP_CONTINUE = 29,							// 魔法持续回复
		I_NOREX_ITEM_ADDJINGZHUN = 30,							// 幸运药水 增加精准率
		I_NOREX_ITEM_ADDBAOJI = 31,								// 会心药水 增加暴击概率
		I_NOREX_ITEM_INC_PUTON_LEVEL = 32,						// 高阶装备药水
		I_NOREX_ITEM_INC_EXP_LEVEL = 33,						// 经验药水 增长百分比 
		I_NOREX_ITEM_EXP_BOTTLE = 34,							// 经验瓶
		I_NOREX_ITEM_FUMOFU = 35,								// 附魔符
		I_NOREX_ITEM_HUOLI = 36,								// 活力药水
		I_NOREX_ITEM_HP_DAN = 37,								// 气血丹
		I_NOREX_ITEM_SZ_PROTECT = 38,							// 神装保护令
		I_NOREX_ITEM_XIANZUNKA_CARD = 39,                       // 仙尊卡
		I_NOREX_ITEM_JINGLING_HUNLI = 40,					    // 仙宠魂力
		I_NOREX_ITEM_LIEMING = 41,								// 猎命
		I_NOREX_ITEM_ANY_MONSTER_DROP_ACTIVITY_ITEM = 42,		// 可增加固定属性的物品（限全场景打怪掉落活动时间内）
		I_NOREX_ITEM_PET_SPECIAL_IMG_CARD = 43,				    // 宠物特殊形象卡
		I_NOREX_ITEM_HUASHEN_CARD = 44,							// 化神卡
		I_NOREX_ITEM_MULTI_MOUNT_CARD = 45,						// 双人坐骑激活卡(使用后激活双人坐骑跟对应形象，并升到指定阶数-不能最大阶)
		I_NOREX_ITEM_MULTI_MOUNT_SPECIAL_IMG_CARD = 46,			// 双人坐骑特殊形象卡
		I_NOREX_ITEM_MAGIC_CARD = 47,							// 魔卡
		I_NOREX_ITEM_WUSHANG_EQUIP = 48,						// 无上神兵
		I_NOREX_ITEM_CROSS_BOSS_SCORE_CARD = 49,				// 跨服boss积分卡
		I_NOREX_ITEM_MAGIC_EQUIPMENT = 50,						// 魔器装备
		I_NOREX_ITEM_SPECIAL_HALO_IMG_CARD = 51,				// 光环特殊形象卡
		I_NOREX_ITEM_SPECIAL_SHENGONG_IMG_CARD = 52,			// 神弓特殊形象卡
		I_NOREX_ITEM_SPECIAL_SHENYI_IMG_CARD = 53,				// 神翼特殊形象卡
		I_NOREX_ITEM_CHENGZHANGDAN = 54,						// 成长丹
		I_NOREX_ITEM_ZHUANSHENG_EQUIP = 55,						// 转生装备
		I_NOREX_ITEM_ZHUANSHENG_XIUWEI_DAN = 56,				// 转生修为丹
		I_NOREX_ITEM_XIAO_CHONG_WU	 = 57,						// 小宠物
		I_NOREX_ITEM_CHINESE_ZODIAC = 58,                       // 星座
		I_NOREX_ITEM_SUPPLY_XUEBAO = 59,                        // 补给血包
		I_NOREX_ITEM_SPECIAL_FIGHTMOUNT_IMG_CARD = 60,          // 战斗坐骑特殊形象卡
		I_NOREX_ITEM_JINGLINGFAZHEN_IMG_CARD = 61,				// 精灵法阵特殊形象卡
		I_NOREX_ITEM_JINGLINGGUANGHUAN_IMG_CARD = 62,			// 精灵光环特殊形象卡
		I_NOREX_ITEM_XIANNV_HUANHUA_ACTIVE_CARD = 63,			// 仙女幻化形象激活卡
		I_NOREX_ITEM_ZHIBAO_HUANHUA_ACTIVE_CARD = 64,			// 至宝幻化卡
		I_NOREX_ITEM_REDUCE_WEARY = 65,				            // 减少boss疲劳值
		I_NOREX_ITEM_SPECIAL_JINGLING_IMG_CARD = 66,			// 精灵特殊形象卡
		I_NOREX_ITEM_MOUNT_UP_ONE_GRADE_CARD = 67,				// 坐骑直升一阶进阶卡
		I_NOREX_ITEM_FIGHT_MOUNT_UP_ONE_GRADE_CARD = 68,		// 战斗坐骑直升一阶进阶卡
		I_NOREX_ITEM_WING_UP_ONE_GRADE_CARD = 69,				// 羽翼直升一阶进阶卡
		I_NOREX_ITEM_HALO_UP_ONE_GRADE_CARD = 70,				// 光环直升一阶进阶卡
		I_NOREX_ITEM_SHENGONG_UP_ONE_GRADE_CARD = 71,			// 神弓直升一阶进阶卡
		I_NOREX_ITEM_SHENYI_UP_ONE_GRADE_CARD = 72,				// 神翼直升一阶进阶卡
		I_NOREX_ITEM_RUNE_BOX = 73,								// 符文宝箱
		I_NOREX_ITEM_RUNE = 74,									// 符文
		I_NOREX_ITEM_RUNE_TOWER_OFFLINE_TIME = 75,				// 符文塔离线时间卡
		I_NOREX_ITEM_PUT_SHENGE = 76,							// 神格离线时间卡
		I_NOREX_ITEM_XINHUN = 77,								// 星魂
		I_NOREX_ITEM_SHENGE_SUIPIAN = 78,						// 神格碎片
		I_NOREX_ITEM_LUCKY_RED_PAPER = 79,						// 幸运红包
		I_NOREX_ITEM_GOLD_CAR = 80,								// 元宝卡，使用后类似充值，并变成内玩
		I_NOREX_ITEM_FB_TOWER_DEFEND_CARD = 81,					// 单人塔防卡
		I_NOREX_ITEM_XIANNV_SHENGWU_ADD_LINGYE = 82,			// 女神圣物-增加灵液
		I_NOREX_ITEM_SPECIAL_FOOTPRINT_IMG_CARD = 83,			// 足迹特殊形象卡
		I_NOREX_ITEM_FOOTPRINT_UP_ONE_GRADE_CARD = 84,			// 足迹直升一阶进阶卡
		I_NOREX_ITEM_SPECIAL_CLOAK_IMG_CARD = 85,				// 披风特殊形象卡
		I_NOREX_ITEM_MARRY_EXP = 86,							// 增加结婚等级经验
		I_NOREX_ITEM_SHENSHOU = 87,								// 神兽
		I_NOREX_ITEM_SHENYIN = 88,								// 神印系统
		I_NOREX_ITEM_TIAN_XIANG = 89,							// 天象物品
		I_NOREX_ITEM_FABAO = 90,							    // 法宝特殊形象激活物品
		I_NOREX_ITEM_HUNYAN_YANHUA = 91,						// 烟花
		I_NOREX_ITEM_SHIZHUANG_UP_ONE_GRADE_CARD = 92,			// 时装直升一阶进阶卡
		I_NOREX_ITEM_SHENBING_UP_ONE_GRADE_CARD = 93,			// 神兵直升一阶进阶卡
		I_NOREX_ITEM_FABAO_UP_ONE_GRADE_CARD = 94,				// 法宝直升一阶进阶卡
		I_NOREX_ITEM_PVP_REWARD_ITEM = 95,                      // 1v1、3v3赛季奖励
		I_NOREX_ITEM_ITEM_SEAL = 96,							// 圣印
		I_NOREX_ITEM_YAOSHI_UP_ONE_GRADE_CARD = 97,				// 腰饰直升一阶进阶卡
		I_NOREX_ITEM_TOUSHI_UP_ONE_GRADE_CARD = 98,				// 头饰直升一阶进阶卡
		I_NOREX_ITEM_QILINBI_UP_ONE_GRADE_CARD = 99,			// 麒麟臂直升一阶进阶卡
		I_NOREX_ITEM_MASK_UP_ONE_GRADE_CARD = 100,				// 面具直升一阶进阶卡
		I_NOREX_ITEM_SPECIAL_YAOSHI_IMG_CARD = 101,				// 腰饰特殊形象卡
		I_NOREX_ITEM_SPECIAL_TOUSHI_IMG_CARD = 102,				// 头饰特殊形象卡
		I_NOREX_ITEM_SPECIAL_MASK_IMG_CARD = 103,				// 面具特殊形象卡
		I_NOREX_ITEM_SPECIAL_QILINBI_IMG_CARD = 104,			// 麒麟臂特殊形象卡
		I_NOREX_ITEM_PLANTING_TREE_SEED = 105,                  // 植树种子        
		I_NOREX_ITEM_UPGRADE_SYS_IMG_CARD = 106,				// 进阶系统特殊形象卡
		I_NOREX_ITEM_UPGRADE_SYS_UP_ONE_GRADE_CARD = 107,		// 进阶系统直升一阶进阶卡
		I_NOREX_ITEM_BIG_SMALL_GOAL_SPECIAL_ATTR = 108,         // 人物大小目标特殊属性激活卡
		I_NOREX_ITEM_CROSS_HONOR = 109,							// 跨服荣耀
		I_NOREX_ITEM_TALENT_SKILL_BOOK = 110,					// 天赋技能书
		I_NOREX_ITEM_GREATE_SOLDIER_CARD = 111,					// 名将卡
		I_NOREX_ITEM_GREATE_SOLDIER_HUANHUA_CARD = 112,			// 名將幻化卡
		I_NOREX_ITEM_GREATE_SOLDIER_EXPE_CARD = 113,			// 名将体验卡
		I_NOREX_ITEM_SHENGWU_JINGHUA = 114,						// 仙器精华
		I_NOREX_ITEM_GUILD_GONGZI = 115,						// 仙盟工资
		I_NOREX_ITEM_BOSS_REFRESH_CARD = 116,					// boss刷新卡
		I_NOREX_ITEM_DOUQIDAN = 117,							// 斗气丹使用
		I_NOREX_ITEM_TIANSHENHUTI_EQUIP = 118,					// 周末装备使用增加装备到虚拟背包
		I_NOREX_ITEM_MYTH = 119,								// 神话物品(这个取消了，可以用的。修改后记得删除备注)
		I_NOREX_ITEM_ZODIAC = 120,								// 星象

		I_NOREX_ITEM_MAX,
	};

	NorexItem();
	~NorexItem();

	virtual bool Init(const char* path, std::string *err);
	virtual int Use(Role *role, const ItemGridData &item_data, short &num, short equip_index = -1) const;
	int GetParam1() const { return m_param1; }
	int GetParam2() const { return m_param2; }
	int GetParam3() const { return m_param3; }
	int GetParam4() const { return m_param4; }
	int GetParam5() const { return m_param5; }
	short GetUseType() const { return m_use_type; }

private:
	short m_use_type;
	short m_use_daytimes;
	int m_colddown_id;
	int m_server_colddown;
	int m_param1;
	int m_param2;
	int m_param3;
	int m_param4;
	int m_param5;
};

#endif //__NOREX_ITEM_H__
