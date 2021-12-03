#ifndef __GIFT_ITEM_H__
#define __GIFT_ITEM_H__

#include "item/itembase.h"
#include "item/itemextern.hpp"

class GiftItem : public ItemBase
{
public:
	static const int GIFT_ITEM_MAX_NUM = 40;

	enum GIFT_TYPE
	{
		GIFT_TYPE_INVALID,

		GIFT_TYPE_DEF,							// 固定礼包
		GIFT_TYPE_RAND,							// 随机礼包
		GIFT_TYPE_SELECT,						// 选择礼包

		GIFT_TYPE_MAX,
	};

	enum IS_BOX_TYPE
	{
		IS_BOX_TYPE_INVALID = -1,

		IS_BOX_TYPE_SAME_AS_THE_GIFT,			//开出的物品绑定属性受礼包绑定属性影响,并且不通知客户端
		IS_BOX_TYPE_SAME_AS_THE_ITEM_CONFIG,	//开出的物品绑定属性不受礼包绑定属性影响,并通知客户端

		IS_BOX_TYPE_MAX,
	};

	GiftItem(short sub_type) : ItemBase(I_TYPE_GIFT), m_sub_type(sub_type), m_coin_bind(0), m_gold_bind(0), m_gold(0), m_is_check_prof(false), m_item_num(0), m_day_usetimes(0), is_box(0) {}
	virtual ~GiftItem() {}
	short GetSubType(){ return m_sub_type; }

protected:
	short m_sub_type;
	int m_coin_bind;							// 绑定钱币
	int m_gold_bind;							// 绑定元宝
	int m_gold;									// 元宝
	bool m_is_check_prof;						// 是否区分职业
	short m_item_num;							// 开出数量
	short m_day_usetimes;
	int is_box;									// 是否为箱子. 1开出的物品绑定属性不受礼包绑定属性影响， 0开出的物品绑定属性受礼包绑定属性影响
	ItemConfigData m_gift_item_list[GIFT_ITEM_MAX_NUM];
};

#endif //__GIFT_ITEM_H__
