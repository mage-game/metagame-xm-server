#ifndef __PUBLICNOTICELIST_H__
#define __PUBLICNOTICELIST_H__

#include <string>

#include "common/tlvprotocol.h"

class PublicNoticeList
{
public:
	static const int MAX_PUBLICNOTICE_NUM = 32;

	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct NoticeAttr
	{
		int notice_id;
	
		std::string creator;
		std::string notice;
		int type;
		int color;
		int urgent;

		long long begin_time;
		long long end_time;
		long long send_interval;
	};

	int count;
	NoticeAttr notice_list[MAX_PUBLICNOTICE_NUM];

};

#endif // __PUBLICNOTICELIST_H__


