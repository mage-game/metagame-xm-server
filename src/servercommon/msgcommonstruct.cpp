

#include "msgcommonstruct.h"
#include "servercommon/struct/itemlistparam.h"

void Protocol::ItemInfo::Copy(const ItemConfigData & item)
{
	item_id = item.item_id;
	item_num = item.num;
	is_bind = item.is_bind;
}

void Protocol::ItemInfoList::Copy1(const std::vector<ItemConfigData> &item_list)
{
	item_count = 0;
	for (const auto &item : item_list)
	{
		item_info_list[item_count++].Copy(item);
		if (item_count >= MAX_ITEM_COUNT) break;
	}
}

void Protocol::ItemInfoList::Copy2(const ItemConfigData item_list[], int count)
{
	item_count = 0;
	for (int i = 0; i < count; ++i)
	{
		item_info_list[item_count++].Copy(item_list[i]);
		if (item_count >= MAX_ITEM_COUNT) break;
	}
}