#include "wantbuyparam.hpp"

#include "servercommon/servercommon.h"

OLD_USE_INTERNAL_NETWORK();

bool WantBuyParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_WANT_BUY_COUNT; ++i)
	{
		ret = outstream.Push(wantbuy_list[i].change_state);
		if (!ret) return false;

		const char * buyer_name = wantbuy_list[i].wantbuy_item.buyer_name;

		ret = outstream.Push(wantbuy_list[i].wantbuy_item.buyer_db_index)
			&& outstream.Push(wantbuy_list[i].wantbuy_item.buyer_role_id)
			&& outstream.Push(buyer_name)
			&& outstream.Push(wantbuy_list[i].wantbuy_item.buy_index)
			&& outstream.Push(wantbuy_list[i].wantbuy_item.item_id)
			&& outstream.Push(wantbuy_list[i].wantbuy_item.item_num)
			&& outstream.Push(wantbuy_list[i].wantbuy_item.color)
			&& outstream.Push(wantbuy_list[i].wantbuy_item.level)
			&& outstream.Push(wantbuy_list[i].wantbuy_item.prof)
			&& outstream.Push(wantbuy_list[i].wantbuy_item.gold_per)
			&& outstream.Push(wantbuy_list[i].wantbuy_item.wantbuy_time)
			&& outstream.Push(wantbuy_list[i].wantbuy_item.due_time);
		if (!ret) return false;
	}

	return true;
}

bool WantBuyParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_WANT_BUY_COUNT; ++i)
	{
		ret = instream.Pop(&wantbuy_list[i].change_state);
		if (!ret) return false;

		const char * buyer_name = 0;

		ret = instream.Pop(&wantbuy_list[i].wantbuy_item.buyer_db_index)
			&& instream.Pop(&wantbuy_list[i].wantbuy_item.buyer_role_id)
			&& instream.Pop(&buyer_name)
			&& instream.Pop(&wantbuy_list[i].wantbuy_item.buy_index)
			&& instream.Pop(&wantbuy_list[i].wantbuy_item.item_id)
			&& instream.Pop(&wantbuy_list[i].wantbuy_item.item_num)
			&& instream.Pop(&wantbuy_list[i].wantbuy_item.color)
			&& instream.Pop(&wantbuy_list[i].wantbuy_item.level)
			&& instream.Pop(&wantbuy_list[i].wantbuy_item.prof)
			&& instream.Pop(&wantbuy_list[i].wantbuy_item.gold_per)
			&& instream.Pop(&wantbuy_list[i].wantbuy_item.wantbuy_time)
			&& instream.Pop(&wantbuy_list[i].wantbuy_item.due_time);
		if (!ret) return false;

		STRNCPY(wantbuy_list[i].wantbuy_item.buyer_name, buyer_name, sizeof(wantbuy_list[0].wantbuy_item.buyer_name));
	}

	return true;
}

