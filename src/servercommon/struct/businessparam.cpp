
#include "businessparam.h"

bool BusinessListParam::Serialize(TLVSerializer &outstream)const
{
	bool ret = outstream.Push(count); 
	if(!ret)
	{
		return false;
	}

	for(int i = 0;i < count && i < MAX_BUSINESS_GOOD_NUMS; ++i)
	{
		ret = outstream.Push(business_list[i].change_state)
			&& outstream.Push(business_list[i].index)
			&& outstream.Push(business_list[i].business_good_id)
			&& outstream.Push(business_list[i].bury_prince);
		if (!ret)
		{
			return false;
		}
	}

	return true;
};
bool BusinessListParam::Unserialize(TLVUnserializer &instream)
{

	bool ret = instream.Pop(&count);
	if(!ret)
	{
		return false;
	}

	for(int i = 0; i < count && i < MAX_BUSINESS_GOOD_NUMS; ++i)
	{
		ret =instream.Pop(&business_list[i].change_state)
			&& instream.Pop(&business_list[i].index)
			&& instream.Pop(&business_list[i].business_good_id)
			&& instream.Pop(&business_list[i].bury_prince);
		if (!ret)
		{
			return false;
		}
	}
	return true;
}


