#ifndef __ITEM_POOL_H__
#define __ITEM_POOL_H__

#include <limits.h>
#include <string>

#include "servercommon/serverdef.h"

class ItemBase;

#define ITEMPOOL ItemPool::Instance()

class ItemPool 
{
public:
	static const int MAX_ITEM_ID = 65535;
	static const int MAX_SUIT_ID = 1024;
	static const int MAX_EQUIPMENT_ORDER = 35;

	enum RELOAD_ITEM_TYPE
	{
		RELOAD_I_TYPE_EXPENSE = 0,
		RELOAD_I_TYPE_OTHER,
		RELOAD_I_TYPE_DEFGIFT,
		RELOAD_I_TYPE_RANDGIFT,
	};

	static ItemPool * Instance();

	bool Init(const std::string &dir, const std::string &filename, std::string *err);	
	static bool Reload(const std::string &dir, const std::string &filename, std::string *err);
	bool ReloadOneItem(const std::string &dir, int item_type, ItemID item_id, std::string *err);

	const ItemBase * GetItem(ItemID item_id);	

private:
	ItemPool();
	~ItemPool();

	void Release();

	long long ConvertToLongLong(short param1, short param2, short param3, char param4, char param5);

	ItemBase *m_item_pool[MAX_ITEM_ID];						// ŒÔ∆∑≥ÿ
};

#endif // __ITEM_POOL_H__

