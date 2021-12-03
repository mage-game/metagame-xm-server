#ifndef __ITEMCOLDDOWNMANAGER_HPP__
#define __ITEMCOLDDOWNMANAGER_HPP__

#include "servercommon/basedef.h"
#include "servercommon/itemcolddowndef.hpp"

class Role;

class ItemColddownManager
{
public:
	ItemColddownManager();
	~ItemColddownManager();

	void Init(Role *role, const ItemColddownParam &param);
	void SetRole(Role *role) { m_role = role; }

	void GetItemColddownParam(ItemColddownParam *param);

	bool AddColddown(UInt16 colddown_id, int colddown_second);
	bool HasColddown(UInt16 colddown_id);

	void RefreshCD();

	void SendItemColddown();

private:
	Role *m_role;

	short m_colddown_num;
	ItemColddownInfo m_colddown_info_list[MAX_COLDDOWN_NUM];
};

#endif
