#include "friendparam.h"

OLD_USE_INTERNAL_NETWORK();

bool FriendListParam::Serialize(TLVSerializer &outstream)const
{
	if (!outstream.Push(count))
	{
		return false;
	}

	bool ret = true;

	for (int i = 0;i < count && i < MAX_FRIEND_NUM; ++i)
	{
		ret = outstream.Push(friend_list[i].change_state) && outstream.Push(friend_list[i].index);

		if (!ret)
		{
			return false;
		}

		if (friend_list[i].change_state == structcommon::CS_DELETE)
		{
			continue;
		}

		ret = outstream.Push(friend_list[i].friend_uid) &&
			outstream.Push(friend_list[i].intimacy) &&
			outstream.Push(friend_list[i].team_today_intimacy) &&
			outstream.Push(friend_list[i].gift_count) &&
			outstream.Push(friend_list[i].favorable_impression) &&
			outstream.Push(friend_list[i].day_chat_count) &&
			outstream.Push(friend_list[i].add_timestamp);

		if (!ret)
		{
			return false;
		}
	}

	return ret;
};

bool FriendListParam::Unserialize(TLVUnserializer &instream)
{
	if (!instream.Pop(&count))
	{
		return false;
	}

	bool ret = true;

	for (int i = 0; i < count && i < MAX_FRIEND_NUM; ++i)
	{
		ret = instream.Pop(&friend_list[i].change_state) && instream.Pop(&friend_list[i].index);

		if (!ret)
		{
			return false;
		}

		if (friend_list[i].change_state == structcommon::CS_DELETE)
		{
			continue;
		}

		ret = instream.Pop(&friend_list[i].friend_uid) &&
			instream.Pop(&friend_list[i].intimacy) &&
			instream.Pop(&friend_list[i].team_today_intimacy) &&
			instream.Pop(&friend_list[i].gift_count) &&
			instream.Pop(&friend_list[i].favorable_impression) &&
			instream.Pop(&friend_list[i].day_chat_count) &&
			instream.Pop(&friend_list[i].add_timestamp);

		if (!ret)
		{
			return false;
		}
	}

	return ret;
}

