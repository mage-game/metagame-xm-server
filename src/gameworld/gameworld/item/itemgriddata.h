#ifndef __ITEMGRIDDATA_H__
#define __ITEMGRIDDATA_H__

#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"
#include "servercommon/struct/itemlistparam.h"

class ItemParam;


enum ITEM_MERGE_RESULT_TYPE
{
	ITEM_MERGE_CANNOT = 0,						// 不可以合并 des的数量本来就为99 或者其他原因
	ITEM_MERGE_COMPLETE,						// 完全合并 src将变为空
	ITEM_MERGE_NOT_COMPLETE,					// 不完全合并 des数量变为99 但src还有剩
};

struct ItemGridData									// 这个结构里面的param指针不要通过结构本身进行释放
{
private:
	ItemGridData & operator = (const ItemGridData &rh)	// 本结构不允许使用等号运算符
	{
		item_id = rh.item_id;
		num = rh.num;
		is_bind = rh.is_bind;
		invalid_time = rh.invalid_time;
		gold_price = rh.gold_price;
		param = rh.param;
		return *this; 
	}

public:

	ItemGridData() : item_id(0), num(0), is_bind(true), invalid_time(0), gold_price(0), param(0) {}
	
	bool Set(const ItemDataWrapper &item_data_wrapper, int rand_attr_put_type = -1);
	void Set(const ItemGridData &data) { *this = data; }

	bool WrapTo(ItemDataWrapper *item_data_wrapper) const;

	bool Invalid() const { return 0 == item_id; }

	void Reset() { item_id = 0; num = 0; is_bind = false; invalid_time = 0; gold_price = 0; param = 0; }

	void Clear();

	bool CanBeMerge(unsigned int merge_invalid_time = 0) const;
	int Merge(const ItemDataWrapper &iteminfo, int *rest_num, int *merged_gold_price, bool ignore_bind = false);
	int Merge(const ItemGridData &itemdata, int *rest_num, int *merged_gold_price, bool ignore_bind = false);

	ItemID		item_id;
	short		num;
	bool		is_bind;
	UInt32		invalid_time;
	int			gold_price;
	ItemParam	*param;
};

#endif
