#ifndef __ITEM_FUNCTION_H__
#define __ITEM_FUNCTION_H__

#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"

#include "servercommon/struct/itemlistparam.h"

class Role;
class Knapsack;
class ItemBase;

class ItemFunction
{
public:

	// 物品合成
	static bool ItemCompose(Role *role, int product_seq, int product_num, int compose_type = 0);

private:
	static bool Composable(Role *role, const ItemBase *itembase);
};

#endif
