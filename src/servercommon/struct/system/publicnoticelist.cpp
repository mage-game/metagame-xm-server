#include "publicnoticelist.h"


bool PublicNoticeList::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < PublicNoticeList::MAX_PUBLICNOTICE_NUM; ++i)
	{
		ret = outstream.Push(notice_list[i].notice_id) 
			&& outstream.Push(notice_list[i].creator.c_str()) 
			&& outstream.Push(notice_list[i].notice.c_str()) 
			&& outstream.Push(notice_list[i].type) 
			&& outstream.Push(notice_list[i].color)
			&& outstream.Push(notice_list[i].urgent)
			&& outstream.Push(notice_list[i].begin_time)
			&& outstream.Push(notice_list[i].end_time)
			&& outstream.Push(notice_list[i].send_interval);
		if (!ret) return false;
	}
	
	if (!ret) return false;

	return true;
}

bool PublicNoticeList::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret || count > PublicNoticeList::MAX_PUBLICNOTICE_NUM) return false;

	for (int i = 0; i < count; ++i)
	{
		const char *creator_tmp = 0;
		const char *notice_tmp = 0;

		ret = instream.Pop(&notice_list[i].notice_id) 
			&& instream.Pop(&creator_tmp) 
			&& instream.Pop(&notice_tmp) 
			&& instream.Pop(&notice_list[i].type) 
			&& instream.Pop(&notice_list[i].color)
			&& instream.Pop(&notice_list[i].urgent)
			&& instream.Pop(&notice_list[i].begin_time)
			&& instream.Pop(&notice_list[i].end_time)
			&& instream.Pop(&notice_list[i].send_interval);
		if (!ret) return false;

		if (creator_tmp != 0) notice_list[i].creator = creator_tmp;
		if (notice_tmp != 0) notice_list[i].notice = notice_tmp;
	}

	return true;
}


