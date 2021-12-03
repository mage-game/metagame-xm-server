#include "dailyfindlistparam.hpp"

bool DailyFindListParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret)
	{
		return false;
	}

	for (int i = 0; i < count && i < DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS; ++i)
	{
		ret = outstream.Push(daily_find_list[i].index) && 
			  outstream.Push(daily_find_list[i].change_state) &&
			  outstream.Push(daily_find_list[i].find_time) && 
			  outstream.Push(daily_find_list[i].role_level) &&
			  outstream.Push(daily_find_list[i].param) && 
			  outstream.Push(daily_find_list[i].param1) &&
			  outstream.Push(daily_find_list[i].find_count);
		if (!ret)
		{
			return false;
		}
	}

	return ret;
}

bool DailyFindListParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret)
	{
		return false;
	}

	for (int i = 0; i < count && i < DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS; ++i)
	{
		ret = instream.Pop(&daily_find_list[i].index) && 
			instream.Pop(&daily_find_list[i].change_state) && 
			instream.Pop(&daily_find_list[i].find_time) && 
			instream.Pop(&daily_find_list[i].role_level) &&
			instream.Pop(&daily_find_list[i].param) && 
			instream.Pop(&daily_find_list[i].param1) && 
			instream.Pop(&daily_find_list[i].find_count);
		if (!ret)
		{
			return false;
		}
	}

	return ret;
}

