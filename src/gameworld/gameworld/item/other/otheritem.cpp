#include "otheritem.hpp"

#include "obj/character/role.h"

#include "servercommon/errornum.h"

int OtherItem::Use(Role *role, const ItemGridData &item_data, short &num, short equip_index) const
{
	role->NoticeNum(errornum::EN_KNAPSACK_USE_NOT_DIRECT);

	return -1;
}
