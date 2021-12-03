#include "friendblessmanager.h"
#include "friendmanager.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "obj/character/role.h"
#include "protocal/msgfriend.h"
#include "world.h"
#include "global/usercache/usercache.hpp"
#include "engineadapter.h"
#include "globalconfig/globalconfig.h"
#include "other/event/eventhandler.hpp"

FriendBlessManager & FriendBlessManager::Instance()
{
	static FriendBlessManager _instance;
	return _instance;
}

FriendBlessManager::FriendBlessManager()
{

}

FriendBlessManager::~FriendBlessManager()
{

}

void FriendBlessManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	m_user_map.clear();

	{
		static Protocol::SCFriendBlessAllInfo fbai;
		fbai.fetch_reward_times = 0;
		fbai.bless_item_count = 0;
		fbai.bless_times = 0;

		int send_len = sizeof(fbai) - sizeof(fbai.bless_item_list);
		World::GetInstWorld()->SendToAllGateway((const char *)&fbai, send_len);
	}
}

void FriendBlessManager::OnRemoveFriend(Role *role, int friend_uid)
{
	if (NULL == role)
	{
		return;
	}

	FriendBlessUser *bless_user = this->GetUser(role->GetUID());
	if (NULL == bless_user)
	{
		return;
	}

	FriendBlessStatusMapIt it = bless_user->bless_status_map.find(friend_uid);
	if (bless_user->bless_status_map.end() == it)
	{
		return;
	}

	FriendBlessStatusItem status_item = it->second;
	bless_user->bless_status_map.erase(friend_uid);

	this->SendChangeInfo(role, Protocol::FRIEND_BLESS_NOTIFY_REASON_REMOVE_STATUS, bless_user->fetch_reward_times, bless_user->bless_times, status_item);
}

void FriendBlessManager::OnBlessReqHelper(Role *role, int target_uid, bool notify)
{
	if (NULL == role)
	{
		return;
	}

	if (!role->GetFriendManager()->IsFriend(IntToUid(target_uid)))
	{
		if (notify) role->NoticeNum(errornum::EN_FRIEND_BLESS_FRIEND_LIMIT);
		return;
	}

	FriendBlessUser *bless_user = this->GetUser(role->GetUID());
	if (NULL == bless_user)
	{
		return;
	}

	FriendBlessUser *target_user = this->GetUser(target_uid);
	if (NULL == target_user)
	{
		return;
	}

	if (bless_user->bless_times >= GLOBALCONFIG->GetFriendBlessMaxCount())
	{
		if (notify) role->NoticeNum(errornum::EN_FRIEND_BLESS_BLESS_TIMES_LIMIT);
		return;
	}

	FriendBlessStatusMapIt bless_status_it = bless_user->bless_status_map.find(target_uid);
	{
		if (bless_user->bless_status_map.end() == bless_status_it)
		{
			FriendBlessStatusItem status_item;
			status_item.friend_uid = target_uid;
			bless_user->bless_status_map[target_uid] = status_item;

			bless_status_it = bless_user->bless_status_map.find(target_uid);
		}
	}
	if (bless_user->bless_status_map.end() == bless_status_it) return;				// 不可能失败

	if (0 != bless_status_it->second.has_bless)
	{
		if (notify) role->NoticeNum(errornum::EN_FRIEND_BLESS_HAS_BLESS);
		return;
	}

	FriendBlessStatusMapIt target_status_it = target_user->bless_status_map.find(role->GetUID());
	{
		if (target_user->bless_status_map.end() == target_status_it)
		{
			FriendBlessStatusItem status_item;
			status_item.friend_uid = role->GetUID();
			target_user->bless_status_map[role->GetUID()] = status_item;

			target_status_it = target_user->bless_status_map.find(role->GetUID());
		}
	}
	if (target_user->bless_status_map.end() == target_status_it) return;			// 不可能失败

	++ bless_user->bless_times;
	bless_status_it->second.has_bless = 1;
	target_status_it->second.bless_me = 1;

	this->SendChangeInfo(role, Protocol::FRIEND_BLESS_NOTIFY_REASON_UPDATE_STATUS, bless_user->fetch_reward_times, bless_user->bless_times, bless_status_it->second);

	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(target_uid));
	if (NULL != target_role)
	{
		this->SendChangeInfo(target_role, Protocol::FRIEND_BLESS_NOTIFY_REASON_UPDATE_STATUS, target_user->fetch_reward_times, target_user->bless_times, target_status_it->second);
	}

	if (notify) role->NoticeNum(noticenum::NT_FRIEND_BLESS_SUCC);

	EventHandler::Instance().OnFriendBless(role);
}

void FriendBlessManager::OnBlessReq(Role *role, int target_uid)
{
	if (NULL == role)
	{
		return;
	}

	if (target_uid > 0)
	{
		this->OnBlessReqHelper(role, target_uid, true);
	}
	else
	{
		FriendListParam friend_list_param;
		role->GetFriendManager()->GetFriendList(&friend_list_param);

		for (int i = 0; i < friend_list_param.count && i < 100; ++ i)
		{
			this->OnBlessReqHelper(role, friend_list_param.friend_list[i].friend_uid, false);
		}

		role->NoticeNum(noticenum::NT_FRIEND_BLESS_SUCC);
	}
}

void FriendBlessManager::OnFetchReward(Role *role, int target_uid)
{
	if (NULL == role)
	{
		return;
	}

	FriendBlessUser *bless_user = this->GetUser(role->GetUID());
	if (NULL == bless_user)
	{
		return;
	}

	if (bless_user->fetch_reward_times >= GLOBALCONFIG->GetFriendBlessRewardMaxCount())
	{
		role->NoticeNum(errornum::EN_FRIEND_BLESS_FETCH_REWARD_TIMES_LIMIT);
		return;
	}

	FriendBlessStatusMapIt bless_status_it = bless_user->bless_status_map.find(target_uid);
	if (bless_user->bless_status_map.end() == bless_status_it) 
	{
		role->NoticeNum(errornum::EN_FRIEND_BLESS_NOT_BLESS);
		return;
	}

	if (0 == bless_status_it->second.bless_me)
	{
		role->NoticeNum(errornum::EN_FRIEND_BLESS_NOT_BLESS);
		return;
	}

	if (0 != bless_status_it->second.has_fetch_reward)
	{
		role->NoticeNum(errornum::EN_FRIEND_BLESS_HAS_FETCH_REWARD);
		return;
	}

	++ bless_user->fetch_reward_times;
	bless_status_it->second.has_fetch_reward = 1;

	role->AddEnergyForce(GLOBALCONFIG->GetFriendBlessRewardTili(), "FriendBless");

	this->SendChangeInfo(role, Protocol::FRIEND_BLESS_NOTIFY_REASON_UPDATE_REWARD_TIMES, bless_user->fetch_reward_times, bless_user->bless_times, bless_status_it->second);

	role->NoticeNum(noticenum::NT_FRIEND_FETCH_BLESS_REWARD);
}

void FriendBlessManager::SendAllInfo(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	FriendBlessUser *bless_user = this->GetUser(role->GetUID());
	if (NULL == bless_user)
	{
		return;
	}

	static Protocol::SCFriendBlessAllInfo fbai;
	fbai.fetch_reward_times = bless_user->fetch_reward_times;
	fbai.bless_times = bless_user->bless_times;
	fbai.bless_item_count = 0;

	for (FriendBlessStatusMapIt it = bless_user->bless_status_map.begin(); it != bless_user->bless_status_map.end(); ++ it)
	{
		if (fbai.bless_item_count >= MAX_FRIEND_NUM)
		{
			break;
		}

		Protocol::FriendBlessStatusItem &bless_item = fbai.bless_item_list[fbai.bless_item_count];
		bless_item.uid = it->second.friend_uid;
		bless_item.has_bless = it->second.has_bless;
		bless_item.bless_me = it->second.bless_me;
		bless_item.has_fetch_reward = it->second.has_fetch_reward;
		bless_item.reserve_ch = 0;
		++ fbai.bless_item_count;
	}

	int send_len = sizeof(fbai) - sizeof(fbai.bless_item_list) + sizeof(fbai.bless_item_list[0]) * fbai.bless_item_count;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&fbai, send_len);
}

void FriendBlessManager::OnInviteBlessReq(Role *invite_role)
{
	if (NULL == invite_role)
	{
		return;
	}

	FriendBlessUser *invite_user = this->GetUser(invite_role->GetUID());
	if (NULL == invite_user)
	{
		return;
	}

	if (invite_user->fetch_reward_times >= GLOBALCONFIG->GetFriendBlessRewardMaxCount())
	{
		invite_role->NoticeNum(errornum::EN_FRIEND_BLESS_FETCH_REWARD_TIMES_LIMIT);
		return;
	}

	static FriendListParam friend_list_param; 
	friend_list_param.count = 0;
	invite_role->GetFriendManager()->GetFriendList(&friend_list_param);

	if (friend_list_param.count <= 0)
	{
		return;
	}

	int netidlist_count = 0;
	GSNetID netidlist[MAX_FRIEND_NUM];

	for (int i = 0; i < friend_list_param.count && i < MAX_FRIEND_NUM; ++ i)
	{
		UserID friend_user_id = IntToUid(friend_list_param.friend_list[i].friend_uid);

		Role *friend_role = World::GetInstWorld()->GetSceneManager()->GetRole(friend_user_id);
		if (NULL != friend_role)
		{
			if (FriendBlessManager::Instance().GetUserBlessTimes(friend_role->GetUID()) < GLOBALCONFIG->GetFriendBlessMaxCount())
			{
				FriendBlessUser *friend_user = this->GetUser(friend_role->GetUID());
				if (NULL != friend_user)
				{
					FriendBlessStatusMapIt target_status_it = friend_user->bless_status_map.find(invite_role->GetUID());
					if (friend_user->bless_status_map.end() == target_status_it || 0 == target_status_it->second.has_bless)
					{
						if (netidlist_count < MAX_FRIEND_NUM)
						{
							netidlist[netidlist_count ++] = friend_role->GetNetId();
						}
					}
				}
			}
		}
	}

	if (netidlist_count > 0)
	{
		Protocol::SCFriendBlessInviteBless fbib;
		fbib.invite_uid = invite_role->GetUID();
		invite_role->GetName(fbib.invite_name);
		EngineAdapter::Instance().NetMulSend(netidlist, netidlist_count, (const char *)&fbib, sizeof(fbib));
	}

	invite_role->NoticeNum(noticenum::NT_FRIEND_BLESS_INVATE_BLESS_SUCC);
}

int FriendBlessManager::GetUserBlessTimes(int uid)
{
	FriendBlessUserMapIt it = m_user_map.find(uid);
	if (m_user_map.end() == it)
	{
		return 0;
	}

	return it->second.bless_times;
}

FriendBlessManager::FriendBlessUser * FriendBlessManager::GetUser(int uid)
{
	UserCacheNode * user_node = UserCacheManager::Instance().GetUserNode(uid); // 角色不存在于服务器
	if (NULL == user_node)
	{
		return NULL;
	}
	
	FriendBlessUserMapIt it = m_user_map.find(uid);
	if (m_user_map.end() == it)
	{
		FriendBlessUser bless_user;
		bless_user.user_id = uid;
		bless_user.fetch_reward_times = 0;
		m_user_map[uid] = bless_user;

		it = m_user_map.find(uid);
	}
	if (m_user_map.end() == it)
	{
		return NULL;
	}

	return &it->second;
}

void FriendBlessManager::SendChangeInfo(Role *role, int notify_reason, int fetch_reward_times, int bless_times, const FriendBlessManager::FriendBlessStatusItem &status_item)
{
	if (NULL == role)
	{
		return;
	}

	static Protocol::SCFriendBlessChangeInfo fbci;
	fbci.notify_reason = notify_reason;
	fbci.fetch_reward_times = fetch_reward_times;
	fbci.bless_times = bless_times;
	fbci.bless_item.uid = status_item.friend_uid;
	fbci.bless_item.has_bless = status_item.has_bless;
	fbci.bless_item.bless_me = status_item.bless_me;
	fbci.bless_item.has_fetch_reward = status_item.has_fetch_reward;
	fbci.bless_item.reserve_ch = 0;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&fbci, sizeof(fbci));
}


