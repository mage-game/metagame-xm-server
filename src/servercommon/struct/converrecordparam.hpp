#ifndef __CONVER_RECORD_PARAM_HPP__
#define __CONVER_RECORD_PARAM_HPP__

#include "servercommon/servercommon.h"

#pragma pack(push) 
#pragma pack(4)

static const int MAX_CONVERT_RECORD_ITEM_COUNT = 60;
static const int MAX_TIME_CONVERT_RECORD_ITEM_COUNT = 120;

struct ConvertRecordItem
{
	ConvertRecordItem() { this->Reset(); }

	void Reset()
	{
		reserve = 0;
		convert_type = 0;
		reserve_1 = 0;
		convert_count = 0;
		seq = 0;
	}

	short reserve;
	char convert_type;
	char reserve_1;
	short convert_count;		
	short seq;
};

// 限时存储
struct ConvertRecordItemLimitTime
{
	ConvertRecordItemLimitTime() { this->Reset(); }

	void Reset()
	{
		convert_type = 0;
		seq = -1;
		timestamp = 0;
	}

	short convert_type;
	short seq;
	unsigned int timestamp;  // 开始时间
};

struct ConvertRecordParam
{
	void OnResetData()
	{
		record_count = 0;

		for (int i = 0; i < MAX_CONVERT_RECORD_ITEM_COUNT; ++i)
		{
			record_list[i].Reset();
		}
	}

	void Reset()
	{			
		lifetime_record_count = 0;
		record_count = 0;

		for (int i = 0; i < MAX_CONVERT_RECORD_ITEM_COUNT; ++i)
		{
			record_list[i].Reset();
			lifetime_record_list[i].Reset();
		}

		limittime_record_count = 0;
		for (int i = 0; i < MAX_TIME_CONVERT_RECORD_ITEM_COUNT; ++i)
		{
			limit_time_record_list[i].Reset();
		}
	}

	short lifetime_record_count;
	short record_count;
	ConvertRecordItem record_list[MAX_CONVERT_RECORD_ITEM_COUNT];
	ConvertRecordItem lifetime_record_list[MAX_CONVERT_RECORD_ITEM_COUNT];
	
	int limittime_record_count;
	ConvertRecordItemLimitTime limit_time_record_list[MAX_TIME_CONVERT_RECORD_ITEM_COUNT];
};

#pragma pack(pop)

typedef char ConvertRecordParamHex[sizeof(ConvertRecordParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ConvertRecordParamHex) < 4096);

#endif // __CONVER_RECORD_PARAM_HPP__

