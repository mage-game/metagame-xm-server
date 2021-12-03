#ifndef _CHALLENGEFB_MANAGER_HPP_
#define _CHALLENGEFB_MANAGER_HPP_

#include "servercommon/servercommon.h"

#include <map>
#include <set>
#include "protocal/msgfb.h"

class Role;
class FallingItem;
class ItemDataWrapper;

class ChallengeFBManager
{
public:

	struct Challenge_data
	{
		Challenge_data() { this->Reset(); }

		void Reset()
		{
			is_all_finish = false;
			drop_item_count = 0;
			memset(drop_list, 0, sizeof(drop_list));
		}

		bool is_all_finish;
		int drop_item_count;
		ItemDataWrapper drop_list[Protocol::SCFBDropInfo::FB_DROP_ITEM_MAX_NUM];
	};

	static ChallengeFBManager & Instance();

	void Update(unsigned long interval);

	void OnEnter(Role *role);

	void OnFbFinish(Role *role);

	void OnPickFallingItem(Role *role, FallingItem *falling_item);

	int GetTotalFallItem(Role *role, ItemDataWrapper *total_fall_item_list);

private:
	ChallengeFBManager();
	~ChallengeFBManager();

	typedef std::map<UserID, Challenge_data> ChallengeFBUserMap;
	typedef std::map<UserID, Challenge_data>::iterator ChallengeFBUserMapIt;
	ChallengeFBUserMap m_challengefb_user_map;

};

#endif