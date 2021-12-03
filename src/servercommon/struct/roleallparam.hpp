#ifndef __ROLE_ALL_PARAM_H__
#define __ROLE_ALL_PARAM_H__

#include "common/tlvprotocol.h"
#include "roleinitparam.h"
#include "roleotherinitparam.hpp"
#include "itemlistparam.h"
#include "hotkeylistparam.h"
#include "friendparam.h"
#include "enemyparam.h"
#include "businessparam.h"
#include "skilllistparam.h"
#include "dailyfindlistparam.hpp"
#include "blacklistsparam.h"

#pragma pack(push, 4)

class RoleAllParam
{
public:
	bool Serialize(TLVSerializer &outstream, bool ver_check) const;
	bool Unserialize(TLVUnserializer &instream, bool ver_check);
	
	void Reset();

	RoleInitParam			role_param;
	RoleOtherInitParam		other_param;
	ItemListParam			item_param;
	HotkeyListParam			hotkey_param;
	FriendListParam			friend_param;
	BlacklistsParam			black_param;
	EnemyListParam			enemy_param;
	BusinessListParam		business_param;
	SkillListParam			skill_param;
	DailyFindListParam		daily_find_param;
};

#pragma pack(pop)

#endif	// __ROLE_ALL_PARAM_H__

