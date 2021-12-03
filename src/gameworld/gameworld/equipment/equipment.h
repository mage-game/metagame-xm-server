#ifndef __EQUIPMENT_H__
#define __EQUIPMENT_H__

#include "gamedef.h"
#include "item/itembase.h"
#include "servercommon/struct/itemlistparam.h"
#include "obj/character/attribute.hpp"
#include "servercommon/stonedef.hpp"
#include "servercommon/struct/itemlistparam.h"

#define EQUIP_ATTR_PER 0.0001f //装备相关的加成百分比，配置里配成万分比，
// ------------------------------------------------------------------------------------------------------------
class EquipmentParam : public ItemParam	
{
public:
	EquipmentParam();

	virtual bool SetInStructData(ItemParamDataStruct *equip_data) const;

	void *	operator new(size_t c);
	void	operator delete(void *m);

	short quality;														// 品质
	short strengthen_level;												// 强化等级
	short shen_level;													// 神铸等级
	short fuling_level;													// 附灵等级
	
	short star_level;													// 星星等级
	short has_lucky;													// 是否有幸运属性
	short fumo_id;														// 附魔id
	short xianpin_equip;                                                // 仙品装备,1100-1110属于仙品装备

	unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];	// 仙品属性类型

	int param1;
	int param2;
	int param3;

	unsigned char rand_attr_type_1;											// 随机属性类型1
	unsigned char rand_attr_type_2;											// 随机属性类型2
	unsigned char rand_attr_type_3;											// 随机属性类型3
	unsigned char reserve_type;

	unsigned short rand_attr_val_1;											// 随机属性1
	unsigned short rand_attr_val_2;											// 随机属性2
	unsigned short rand_attr_val_3;											// 随机属性3
	unsigned short rand_attr_val_reserve;

	JingLingSkillInfo jing_ling_skill_list[JING_LING_SKILL_COUNT_MAX];
	unsigned short really;													// 真*装备
	unsigned short xianpin_star_num;                                        // 仙品星星数
};

class Equipment : public ItemBase
{
public:

	static const int BASE_ATTR_NUM = 12;							// 基础属性数量

	enum EQUIPMENT_TYPE												// 装备类型
	{
		E_TYPE_MIN = 100,

		E_TYPE_TOUKUI = E_TYPE_MIN,									// 头盔
		E_TYPE_YIFU,												// 衣服
		E_TYPE_KUZI,												// 裤子
		E_TYPE_XIEZI,												// 鞋子
		E_TYPE_HUSHOU,												// 护手
		E_TYPE_XIANGLIAN,											// 项链
		E_TYPE_WUQI,												// 武器
		E_TYPE_JIEZHI,												// 戒指
		E_TYPE_YAODAI,												// 腰带
		E_TYPE_YUPEI,												// 玉佩
		E_TYPE_SHOUZHUO,											// 手镯
		E_TYPE_MAX,

		E_TYPE_SPECIAL_MIN = 200,									// 特殊装备类型（如精灵)。使用equipment结构体，数据管理不在equipmentmanager，而在各个模块
		E_TYPE_SPECIAL_JINGLING ,									// 精灵
		E_TYPE_SPECIAL_HUNJIE,										// 婚戒
		E_TYPE_SPECIAL_IMP_GUARD,                                   // 小鬼
		E_TYPE_SPECIAL_MINGHUN,										// 命魂
		E_TYPE_SPECIAL_LITTLEPET,									// 小宠物
		E_TYPE_SPECIAL_MAX,

		E_TYPE_FIGHT_MOUNT = 300,									// 战斗坐骑
		E_TYPE_MOUNT = 400,											// 坐骑
		E_TYPE_WING = 500,											// 羽翼
		E_TYPE_HALO = 600,											// 光环
		E_TYPE_SHEN_GONG = 700,										// 神弓
		E_TYPE_SHEN_YI = 800,										// 神翼

		E_TYPE_ZHUANSHENG_MIN = 900,
		E_TYPE_ZHUANSHENG_1 = E_TYPE_ZHUANSHENG_MIN,				// 转生1
		E_TYPE_ZHUANSHENG_2,										// 转生2
		E_TYPE_ZHUANSHENG_3,										// 转生3
		E_TYPE_ZHUANSHENG_4,										// 转生4
		E_TYPE_ZHUANSHENG_5,										// 转生5
		E_TYPE_ZHUANSHENG_6,										// 转生6
		E_TYPE_ZHUANSHENG_7,										// 转生7
		E_TYPE_ZHUANSHENG_8,										// 转生8
		E_TYPE_ZHUANSHENG_MAX,

		E_TYPE_QINGYUAN_MIN = 1000,
		E_TYPE_QINGYUAN_1 = E_TYPE_QINGYUAN_MIN,					// 结婚1
		E_TYPE_QINGYUAN_2,											// 结婚2
		E_TYPE_QINGYUAN_3,											// 结婚3
		E_TYPE_QINGYUAN_4,											// 结婚4
		E_TYPE_QINGYUAN_MAX,

// 		E_TYPE_FEIXIAN_MIN = 1100,									// 飞仙 
// 		E_TYPE_FEIXIAN_WUQI = E_TYPE_FEIXIAN_MIN,					// 武器
// 		E_TYPE_FEIXIAN_YIFU,										// 铠甲
// 		E_TYPE_FEIXIAN_HUSHOU,										// 护手
// 		E_TYPE_FEIXIAN_YAODAI,										// 腰带
// 		E_TYPE_FEIXIAN_HUTUI,										// 护腿
// 		E_TYPE_FEIXIAN_TOUKUI,										// 头盔
// 		E_TYPE_FEIXIAN_XIANGLIAN,									// 项链
// 		E_TYPE_FEIXIAN_SHOUZHUO,									// 手镯
// 		E_TYPE_FEIXIAN_JIEZHI,										// 戒指
// 		E_TYPE_FEIXIAN_XIEZI,										// 鞋子
// 		E_TYPE_FEIXIAN_YUPEI,										// 玉佩
// 		E_TYPE_FEIXIAN_MAX,

		E_TYPE_ZHUANZHI_MIN = 1100,									// 转职 
		E_TYPE_ZHUANZHI_WUQI = E_TYPE_ZHUANZHI_MIN,					// 武器
		E_TYPE_ZHUANZHI_YIFU,										// 铠甲
		E_TYPE_ZHUANZHI_HUSHOU,										// 护手
		E_TYPE_ZHUANZHI_YAODAI,										// 腰带
//		E_TYPE_ZHUANZHI_HUTUI,										// 护腿
		E_TYPE_ZHUANZHI_TOUKUI,										// 头盔
		E_TYPE_ZHUANZHI_XIANGLIAN,									// 项链
		E_TYPE_ZHUANZHI_SHOUZHUO,									// 手镯
		E_TYPE_ZHUANZHI_JIEZHI,										// 戒指
		E_TYPE_ZHUANZHI_XIEZI,										// 鞋子
		E_TYPE_ZHUANZHI_YUPEI,										// 玉佩
		E_TYPE_ZHUANZHI_MAX,

		E_TYPE_LITTLE_PET_MIN = 1200,								// 小宠物装备
		E_TYPE_LITTLE_PET_1 = E_TYPE_LITTLE_PET_MIN,
		E_TYPE_LITTLE_PET_2,
		E_TYPE_LITTLE_PET_3,
		E_TYPE_LITTLE_PET_4,
		E_TYPE_LITTLE_PET_MAX,

		E_TYPE_CHINESE_ZODIAC_MIN = 1300,							//生肖装备
		E_TYPE_CHINESE_ZODIAC_1 = E_TYPE_CHINESE_ZODIAC_MIN,
		E_TYPE_CHINESE_ZODIAC_2,
		E_TYPE_CHINESE_ZODIAC_3,
		E_TYPE_CHINESE_ZODIAC_4,
		E_TYPE_CHINESE_ZODIAC_5,
		E_TYPE_CHINESE_ZODIAC_MAX,

		E_TYPE_SHENSHOU_MIN = 1400,								   //龙器装备
		E_TYPE_SHENSHOU_1 = E_TYPE_SHENSHOU_MIN,
		E_TYPE_SHENSHOU_2,
		E_TYPE_SHENSHOU_3,
		E_TYPE_SHENSHOU_4,
		E_TYPE_SHENSHOU_5,
		E_TYPE_SHENSHOU_MAX,

		E_TYPE_GREATESOLDIER_MIN = 1500,							//神魔装备(名将变身系统)
		E_TYPE_GREATESOLDIER_1 = E_TYPE_GREATESOLDIER_MIN,
		E_TYPE_GREATESOLDIER_2,
		E_TYPE_GREATESOLDIER_3,
		E_TYPE_GREATESOLDIER_4,
		E_TYPE_GREATESOLDIER_MAX,

		E_TYPE_BAIZHAN_MIN = 1600,									// 百战 
		E_TYPE_BAIZHAN_WUQI = E_TYPE_BAIZHAN_MIN,					// 武器
		E_TYPE_BAIZHAN_YIFU,										// 衣服
		E_TYPE_BAIZHAN_HUSHOU,										// 护手
		E_TYPE_BAIZHAN_YAODAI,										// 腰带
		E_TYPE_BAIZHAN_TOUKUI,										// 头盔
		E_TYPE_BAIZHAN_XIANGLIAN,									// 项链
		E_TYPE_BAIZHAN_SHOUZHUO,									// 手镯
		E_TYPE_BAIZHAN_JIEZHI,										// 戒指
		E_TYPE_BAIZHAN_XIEZI,										// 鞋子
		E_TYPE_BAIZHAN_YUPEI,										// 玉佩
		E_TYPE_BAIZHAN_MAX,

		E_TYPE_CHUANSHI_MIN = 1700,
		E_TYPE_CHUANSHI_WUQI,										// 斗气武器 
		E_TYPE_CHUANSHI_TOUKUI,										// 斗气战盔 
		E_TYPE_CHUANSHI_YIFU,										// 斗气战衣 
		E_TYPE_CHUANSHI_HUSHOU,										// 斗气战腕  
		E_TYPE_CHUANSHI_YAODAI,										// 斗气腰带
		E_TYPE_CHUANSHI_HUTUI,										// 斗气护腿
		E_TYPE_CHUANSHI_XIEZI,										// 斗气战靴
		E_TYPE_CHUANSHI_ZHUOZI,										// 斗气灵镯
		E_TYPE_CHUANSHI_XIANGLIAN,									// 斗气灵链
		E_TYPE_CHUANSHI_JIEZHI,										// 斗气战戒
		E_TYPE_CHUANSHI_MAX,

	};

	const static int EQUIPMENT_TYPE_COUNT = E_TYPE_MAX - E_TYPE_MIN;// 装备类型数量

	enum EQUIPMENT_INDEX											// 装备部位
	{
		E_INDEX_TOUKUI = 0,											// 头盔
		E_INDEX_YIFU,												// 衣服
		E_INDEX_KUZI,												// 裤子
		E_INDEX_XIEZI,												// 鞋子
		E_INDEX_HUSHOU,												// 护手
		E_INDEX_XIANGLIAN,											// 项链
		E_INDEX_WUQI,												// 武器
		E_INDEX_JIEZHI,												// 戒指
		E_INDEX_YAODAI,												// 腰带
		E_INDEX_YUPEI,												// 玉佩
		E_INDEX_SHOUZHUO,											// 手镯
		E_INDEX_MAX,
	};
	UNSTD_STATIC_CHECK(MAX_STONE_EQUIP_PART <= E_INDEX_MAX);
	UNSTD_STATIC_CHECK(MAX_EQUIP_PART == E_INDEX_MAX);

	enum EQUIPMENT_XIANPIN_ATTRIBUTE								// 装备仙品属性类型
	{
		E_XIANPIN_INVALID = 0,

		E_XIANPIN_ADD_BASE_MAXHP_PER = 1,								// +X%气血上限
		E_XIANPIN_ADD_BASE_GONGJI_PER = 2,								// +X%攻击力
		E_XIANPIN_ADD_BASE_FANGYU_PER = 3,								// +X%防御力
		E_XIANPIN_ADD_BASE_MINGZHAONG_PER = 4,							// +X%命中
		E_XIANPIN_ADD_BASE_SHANBI_PER = 5,								// +X%闪避
		E_XIANPIN_ADD_BASE_BAOJI_PER = 6,								// +X%暴击
		E_XIANPIN_ADD_BASE_JIANREN_PER = 7,								// +X%坚韧

		E_XIANPIN_ADD_BASE_MAXHP_ROLE_LEVEL = 8,						// +角色等级*X气血上限
		E_XIANPIN_ADD_BASE_GONGJI_ROLE_LEVEL = 9,						// +角色等级*X攻击力
		E_XIANPIN_ADD_BASE_FANGYU_ROLE_LEVEL = 10,						// +角色等级*X防御力
		E_XIANPIN_ADD_BASE_MINGZHONG_ROLE_LEVEL = 11,					// +角色等级*X命中
		E_XIANPIN_ADD_BASE_SHANBI_ROLE_LEVEL = 12,						// +角色等级*X闪避
		E_XIANPIN_ADD_BASE_BAOJI_ROLE_LEVEL = 13,						// +角色等级*X暴击
		E_XIANPIN_ADD_BASE_JIANREN_ROLE_LEVEL = 14,						// +角色等级*X坚韧

		E_XIANPIN_ADD_KILL_MONSTER_EXP_REWARD_PER = 15,					// 杀怪经验：+X%
		E_XIANPIN_ADD_HURT_TO_ROLE_PER = 16,							// 玩家对其他玩家造成伤害：+X%
		E_XIANPIN_ADD_HURT_TO_MONSTER_PER = 17,							// 玩家对怪物造成伤害：+X%
		E_XIANPIN_DEC_HURT_FROM_ROLE_PER = 18,							// 其他玩家对自己造成伤害：-X%
		E_XIANPIN_DEC_HURT_FROM_MONSTER_PER = 19,						// 怪物对玩家造成伤害：-X%

		E_XIANPIN_ADD_ALL_EQUIP_BASE_FANGYU_PER= 20,					// +X%全身装备防御
		E_XIANPIN_ADD_ALL_EQUIP_BASE_MAXHP_PER = 21,					// +X%全身装备生命
		E_XIANPIN_ADD_ALL_EQUIP_BASE_GONGJI_PER = 22,					// +X%全身装备攻击

		E_XIANPIN_ADD_GONGJI_OF_ROLE_LEVEL = 23,						// 增加（等级 * X）点攻击
		E_XIANPIN_ADD_FANGYU_OF_ROLE_LEVEL = 24,						// 增加（等级 * X）点防御
		E_XIANPIN_ADD_MAXHP_OF_ROLE_LEVEL = 25,							// 增加（等级 * X）点气血
		E_XIANPIN_ADD_MONSTER_DROP_COIN_PER = 26,						// 野怪掉落金币增加  X %  （对副本、活动内的怪物无效）
		E_XIANPIN_ADD_DAMAGE_TO_MONSTER_OF_ROLE_LEVEL = 27,				// 对怪物攻击额外造成（等级 * X）点伤害
		E_XIANPIN_DEC_DAMAGE_FROM_MONSTER_OF_ROLE_LEVEL = 28,			// 怪物对玩家伤害额外减少（等级 * X）点伤害
		E_XIANPIN_DEC_DAMAGE_FROM_ROLE_PER = 29,						// 被玩家造成伤害减少 X % （玩家造成的最终伤害 * （1 - X %）才是最终伤害）
		E_XIANPIN_ADD_DAMAGE_TO_ROLE_PER = 30,							// 对玩家造成伤害增加 X % （玩家造成的最终伤害 * （1 - X %）才是最终伤害）
		E_XIANPIN_ADD_MONSTER_DROP_EXP_PER = 31,						// 击杀野怪经验额外增加  X %

		E_XIANPIN_MAX,
	};

	enum EQUIPMENT_FIX_GETWAY
	{
		EQUIPMENT_FIX_GETWAY_MIN = 0,

		EQUIPMENT_FIX_GETWAY_DEFAULT_GIFT = 1,						// 从礼包中获得
		EQUIPMENT_FIX_GETWAY_CHEST_SHOP = 2,						// 从寻宝、兑换中获得
		EQUIPMENT_FIX_GETWAY_MONSTER_ID_DROP = 3,					// 从掉落中获得(根据怪物id)
		EQUIPMENT_FIX_GETWAY_SCENE_ID_DROP = 4,						// 从掉落中获得(根据场景id)
		EQUIPMENT_FIX_GETWAY_COLOR_DROP = 5,						// 从掉落中获得(根据装备颜色)
		EQUIPMENT_FIX_GETWAY_COMPOSE = 6,							// 装备兑换
		EQUIPMENT_FIX_GETWAY_COMPOUND = 7,							// 装备合成
		EQUIPMENT_FIX_GETWAY_ZHUANZHI_EQUIP_COMPOSE = 8,			// 转职装备合成

		EQUIPMENT_FIX_GETWAY_MAX,
	};

	enum EQUIPMENT_STRENGTHEN_IS_DOWNLEVEL_ON_FAIL
	{
		E_STRENGTHEN_NOT_DOWNLEVEL_ON_FAIL = 0,						// 强化失败不掉级
		E_STRENGTHEN_DOWNLEVEL_ON_FAIL								// 强化失败掉级
	};

	UNSTD_STATIC_CHECK(E_INDEX_MAX == ItemNamespace::MAX_EQUIPMENT_GRID_NUM);
	
	Equipment(short equip_type);
	virtual ~Equipment() {}

	short GetEquipType() const { return m_equipment_type; }
	//short GetQuality() const { return m_quality; } 

	bool CanStrengthen() const { return m_can_strengthen; }
	bool CanFlush() const { return m_can_flush; }
	UInt16 GetSuitId() const { return m_suit_id; }
	short GetHoleNum() const { return m_hole_num; }
	char IsShen() const { return m_is_shen; }
	short GetActShizhuangType() const { return m_act_shizhung_type; }
	int GetOrder() const { return m_order; }
	int GetSubType() const { return m_sub_type; }
	int GetGuildStorageScore() const { return m_guild_storage_score; }
	int GetGodAttr()const { return god_attr; }

	virtual bool Init(const char* path, std::string *err);
	virtual int Use(Role *role, const ItemGridData &item_data, short &num, short equip_index = -1) const;

	virtual int GetSellPrice(ItemParam *param) const;
	virtual bool CanDiscard(ItemParam *param, int *reason) const;

	virtual EquipmentParam * CreateParam() const;
	virtual bool SetParamFromStruct(ItemParam *param, const ItemParamDataStruct *equip_data) const;
	virtual EquipmentParam * CreateParamFromStruct(const ItemParamDataStruct *equip_data) const;

	void GetBaseAttr(CharIntAttrs &int_attrs) const;
	void GetAttr(const EquipmentParam *param, CharIntAttrs &int_attrs, int role_level) const;

	int GetAttrIndexByAttrType(short attr_type) const;

	static bool IsWuQiIndex(short equip_index);
	static bool IsFangJuIndex(short equip_index);
	static bool IsShouShiIndex(short equip_index);
	static bool IsCommonEquipIndex(short equip_index);
	static bool IsMatchIndex(short equip_type, short equip_index);
	static bool IsValidType(short equip_type);
	static bool IsFangJuType(short equip_type);
	static bool IsShipinType(short equip_type);
	static bool IsWuqiType(short equip_type);
	static bool IsHuShouType(short equip_type);
	static bool IsGongjiType(short equip_type);

	virtual bool CheckIsSameItemExceptSex(const ItemBase *other_item) const;

protected:

	const short	m_equipment_type;									// 装备类型EQUIPMENT_TYPE 武器 衣服等
		
	//short m_quality;												// 品质
	bool m_can_strengthen;											// 可否强化
	bool m_can_flush;												// 可否洗炼
	char m_is_shen;													// 是否神装
	short m_hole_num;												// 孔数量
	UInt16 m_suit_id;												// 套装id
	short m_act_shizhung_type;										// 激活时装类型
	int m_order;													// 阶数
	int m_sub_type;													// 子类型
	int m_guild_storage_score;										// 仙盟仓库职分

	short m_base_attr_type_list[BASE_ATTR_NUM];						// 基础属性类型
	int m_base_attr_value_list[BASE_ATTR_NUM];						// 基础属性值
	short god_attr;                                                 // 天神属性
};

#endif
