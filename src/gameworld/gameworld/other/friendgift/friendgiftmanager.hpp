#ifndef __FRIEND_GIFT_MANAGER_HPP__
#define __FRIEND_GIFT_MANAGER_HPP__

#include "servercommon/struct/friendgiftparam.hpp"
#include "obj/character/attribute.hpp"
#include "servercommon/frienddef.hpp"
#include "gameworld/gameworld/protocal/msgfriendgift.hpp"

class Character;
class Role;

class FriendGiftManager
{
public:
	FriendGiftManager();
	~FriendGiftManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const FriendGiftParam &friend_gift_param);
	void GetInitParam(FriendGiftParam *friend_gift_param);

	void SendFriendGiftInfo();

	void OnFriendGiftSongGift(int uid, int is_all_return, int is_return_gift);
	void OnFriendGiftSongGiftSingle(int uid, int is_return_gift = -1);
	void OnFriendGiftSongGiftAll();

	void OnFriendGiftShouGift(int uid);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	
	void OnRoleLevelChange(int level);
	void ReqHeliNotice(int type, int param1 = -1, int param2 = -1);
	void OnHeliRet(int uid, int type);
	bool IsCanSendHeli(int uid);

	// 物品赠送--------------------------------------------------------
	void OnGiveItemReq(Protocol::CSGiveItemReq *req);
	bool IsCanGive(int bag_index, int num);
	void SendGiveItemInfo(Role *role, bool is_give);

private:

	Role *m_role;

	FriendGiftParam m_friend_gift_param;

	int m_friend_heli_list[MAX_FRIEND_NUM];		// 记录可以祝福自己的好友
};

#endif
