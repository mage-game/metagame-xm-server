#ifndef __FRIEND_BLESS_MANAGER_H__
#define __FRIEND_BLESS_MANAGER_H__

#include <map>

class Role;

class FriendBlessManager
{
public:
	struct FriendBlessStatusItem
	{
		FriendBlessStatusItem() : friend_uid(0), has_bless(0), bless_me(0), has_fetch_reward(0) {}

		int friend_uid;
		char has_bless;
		char bless_me;
		char has_fetch_reward;
	};

	typedef std::map<int, FriendBlessStatusItem> FriendBlessStatusMap;
	typedef std::map<int, FriendBlessStatusItem>::iterator FriendBlessStatusMapIt;

	struct FriendBlessUser
	{
		FriendBlessUser() : user_id(0), fetch_reward_times(0), bless_times(0) {}

		int user_id;
		int fetch_reward_times;
		int bless_times;

		FriendBlessStatusMap bless_status_map;
	};

	typedef std::map<int, FriendBlessUser> FriendBlessUserMap;
	typedef std::map<int, FriendBlessUser>::iterator FriendBlessUserMapIt;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	static FriendBlessManager & Instance();

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnRemoveFriend(Role *role, int friend_uid);
	void OnBlessReqHelper(Role *role, int target_uid, bool notify);
	void OnBlessReq(Role *role, int target_uid);
	void OnFetchReward(Role *role, int target_uid);
	void SendAllInfo(Role *role);
	void OnInviteBlessReq(Role *invite_role);

	int GetUserBlessTimes(int uid);

private:
	FriendBlessManager();
	~FriendBlessManager();

	FriendBlessUser * GetUser(int uid);
	void SendChangeInfo(Role *role, int notify_reason, int fetch_reward_times, int bless_times, const FriendBlessStatusItem &status_item);

	FriendBlessUserMap m_user_map;
};

#endif // __FRIEND_BLESS_MANAGER_H__

