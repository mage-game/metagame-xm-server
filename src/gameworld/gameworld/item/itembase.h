#ifndef __ITEMBASE_H__
#define __ITEMBASE_H__

#include <string>
#include <map>

#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"

struct ItemParamDataStruct;
class Role;
struct ItemDataWrapper;
struct ItemGridData;
struct EquipSpecialParamStruct;

class ItemParam
{
public:
	enum ITEMPARAM_TYPE
	{
		PARAM_TYPE_INVALID = 0,
		PARAM_TYPE_NORMAL_EQUIP,					// 一般装备
	};

	ItemParam(short _param_type) : param_type(_param_type) {}
	virtual ~ItemParam() {}

	virtual bool SetInStructData(ItemParamDataStruct *param_data) const { return false; }
	virtual bool GetSpecialParam(EquipSpecialParamStruct *_special_param) const { return false; }

	short GetParamType() const { return param_type; }

protected:

	short param_type;
};

class ItemBase
{
public:
	static const UInt16 INVALID_ITEM_ID = 0;
	static const int MAX_EQUIP_LEVEL = 200;			// 最大装备等级
	static const int QUALITY_PER_COLOR = 3;			//每个品质之间的等级数

	enum ITEM_COLOR
	{
		I_COLOR_INVALID = 0,

		I_COLOR_GREEN = 1,								// 绿
		I_COLOR_BLUE = 2,								// 蓝
		I_COLOR_PURPLE = 3,								// 紫
		I_COLOR_ORANGE = 4,								// 橙
		I_COLOR_RED = 5,								// 红
		I_COLOR_PINK = 6,								// 粉

		I_COLOR_MAX,
	};

	enum ITEM_QUALITY
	{
		I_QUALITY_MIN = 0,
		I_QUALITY_GREEN = I_QUALITY_MIN,		// 绿
		I_QUALITY_BLUE,							// 蓝
		I_QUALITY_PURPLE,						// 紫
		I_QUALITY_ORANGE,						// 橙
		I_QUALITY_RED,							// 红
		//I_QUALITY_GOLD,							// 金

		I_QUALITY_MAX,
	};

	enum ITEM_TYPE
	{
		I_TYPE_INVALID = -1,						// -1 无效

		I_TYPE_EXPENSE = 0,							// 0 消耗类 能直接使用
		I_TYPE_OTHER,								// 1 被动使用类 type value 最好不要直接使用
		I_TYPE_GIFT,								// 2 礼包类

		I_TYPE_EQUIPMENT = 100,						// 装备类型
	};


	enum ITEM_SEARCH_TYPE							// 没用到 仅用于对照
	{
		I_SEARCH_TYPE_WEAPON_1 = 1,					// 
		I_SEARCH_TYPE_WEAPON_2,						// 
		I_SEARCH_TYPE_WEAPON_3,						// 
		I_SEARCH_TYPE_WEAPON_4,						// 

		I_SEARCH_TYPE_TOUKUI = 20,					// 头盔
		I_SEARCH_TYPE_YIFU,							// 衣服
		I_SEARCH_TYPE_HUJIAN,						// 护肩
		I_SEARCH_TYPE_YAODAI,						// 腰带
		I_SEARCH_TYPE_HUTUI,						// 护腿
		I_SEARCH_TYPE_XIEZI,						// 鞋子
		I_SEARCH_TYPE_HUWAN,						// 护腕
		I_SEARCH_TYPE_HUSHOU,						// 护手
		
		I_SEARCH_TYPE_XIANGLIANG = 30,				// 项链
		I_SEARCH_TYPE_HUFU,							// 护符
		I_SEARCH_TYPE_JIEZHI,						// 戒指
		I_SEARCH_TYPE_SHIZHUANG,					// 时装

		I_SEARCH_TYPE_QIANGHUASHI = 40,				// 强化石
		I_SEARCH_TYPE_XILIANSHI,					// 洗练石
		I_SEARCH_TYPE_JINGLIAN,						// 精炼材料
		I_SEARCH_TYPE_JINJIE,						// 进阶材料
		I_SEARCH_TYPE_SUIPIAN,						// 碎片
		I_SEARCH_TYPE_QIANZHUIPINZHI,				// 前缀品质
		I_SEARCH_TYPE_XIANGQIANSHI,					// 镶嵌石

		I_SEARCH_TYPE_XUANTIESUIPIAN = 50,			// 强化符
		I_SEARCH_TYPE_BAOHUFU,						// 保护符

		I_SEARCH_TYPE_NO_USE = 60,					// 暂时无用
		I_SEARCH_TYPE_CHONGWUDAN,					// 宠物蛋
		I_SEARCH_TYPE_CHONGWUJINENG,				// 宠物技能
		I_SEARCH_TYPE_CHONGWULIANGSHI,				// 宠物粮食
		I_SEARCH_TYPE_CHONGWUCHENGZHANG,			// 宠物成长
		I_SEARCH_TYPE_CHONGWUQIANNENG,				// 宠物潜能

		I_SEARCH_GOLD = 70,							// 元宝
		I_SEARCH_TONGQIAN,							// 铜钱

		I_SEARCH_HP = 80,							// 气血
		I_SEARCH_MP,								// 法力
		I_SEARCH_HPPACKAGE,							// 气血包
		I_SEARCH_MPPACKAGE,							// 法力包

		I_SEARCH_MOUNT_CARD = 90,					// 坐骑卡
		I_SEARCH_MOUNT_GROW,						// 坐骑成长

		I_SEARCH_TYPE_BANGPAI = 100,				// 帮派
		I_SEARCH_TYPE_TASK,							// 任务
		I_SEARCH_TYPE_HUODONG,						// 活动
		I_SEARCH_TYPE_OTHER,						// 其它
		I_SEARCH_TYPE_YUANSHEN,						// 元神

		I_SEARCH_SKILLBOOK_PROF1 = 110,				// 职业1技能书
		I_SEARCH_SKILLBOOK_PROF2,					// 职业2技能书
		I_SEARCH_SKILLBOOK_PROF3,					// 职业3技能书
		I_SEARCH_SKILLBOOK_PROF4,					// 职业4技能书
	};

	enum NOT_DISCARD_REASON							// 不可丢弃理由
	{
		NDR_DEFAULT,
		NDR_INLAY,									// 有镶嵌
	};

	ItemBase(short item_type);
	virtual ~ItemBase();

	short GetItemType() const { return m_item_type; }
	UInt16 GetItemId() const { return m_item_id; }
	const char * GetItemName() const { return m_item_name; }
	short GetSearchType() const { return m_search_type; }
	short GetColor() const { return m_color; }
	short GetLimitLevel() const { return m_limit_level; }
	short GetLimitProf() const { return m_limit_prof; }
	short GetLimitSex() const { return m_limit_sex; }
	short GetPileLimit() const { return m_pile_limit; }

	bool HasInvalidTime() const;
	bool IsCurDayValid() const { return m_is_curday_valid; }
	UInt32 GetTimeLength() const { return m_time_length; }
	UInt32 CalInvalidTime() const;

	bool IsRecord() const { return m_is_record; }
	bool IsBroadcast() const {return m_is_broadcast; }
	bool IsBind() const { return m_is_bind; }
	ItemID GetOtherSexItemId() const { return m_other_sex_itemid; }

	virtual bool Init(const char* path, std::string *err) = 0;
	virtual int Use(Role *role, const ItemGridData &item_data, short &num, short equip_index = -1) const = 0;

	virtual int GetSellPrice(ItemParam *param) const { return m_sell_price; }
	virtual int GetRecyclType(ItemParam *param) const { return m_recycl_type; }
	virtual int GetRecyclGet(ItemParam *param) const { return m_recycl_get; }
	virtual bool CanDiscard(ItemParam *param, int *reason) const;
	virtual bool CanSell(ItemParam *param) const { return m_can_sell; }

	virtual ItemParam * CreateParam() const { return 0; }
	virtual bool SetParamFromStruct(ItemParam *param, const ItemParamDataStruct *equip_data) const { return false; }
	virtual ItemParam * CreateParamFromStruct(const ItemParamDataStruct *equip_data) const { return 0; }

	static bool CanMerge(ItemID item_id);
	static bool CanTrade(const ItemGridData *item_data);

	virtual bool CheckIsSameItemExceptSex(const ItemBase *other_item) const;

protected:
	const short m_item_type;
	short m_search_type;							// 搜索类型
	ItemID m_item_id;								// 物品id
	short m_color;									// 颜色
	bool m_is_bind;									// 初始是否绑定
	bool m_can_discard;								// 是否可丢弃
	bool m_can_sell;								// 能否出售
	bool m_is_broadcast;							// 是否广播
	int m_sell_price;								// 出售价格
	int m_recycl_type;								// 回收类型
	int m_recycl_get;								// 回收获得
	bool m_is_record;								// 是否记录到日志中：掉落、使用
	UInt32 m_time_length;							// 有效时长（获得物品起算）
	UInt32 m_invalid_time;							// 失效时间（特定时间点）
	bool m_is_curday_valid;							// 是否仅获得当天有效
	short m_limit_prof;								// 职业限制
	short m_limit_level;							// 等级限制
	short m_limit_sex;								// 性别限制
	ItemID m_other_sex_itemid;						// 性别限制时对应的另一性别物品id
	short m_pile_limit;								// 堆叠上限
	GameName m_item_name;		
};


#endif

