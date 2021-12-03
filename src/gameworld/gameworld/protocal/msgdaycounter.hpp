
#ifndef __MSG_DAYCOUNTER_HPP__
#define __MSG_DAYCOUNTER_HPP__

#include "gamedef.h"
#include "config/fbconfig.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	/*
		当天计数相关
	*/
	
	class SCDayCounterInfo
	{
	public:
		SCDayCounterInfo();
		MessageHeader		header;
		
		unsigned char		day_counter_list[MAX_DAYCOUNT_COUNT_ID];
	};
	
	class SCDayCounterItemInfo
	{
	public:
		SCDayCounterItemInfo();
		MessageHeader		header;
		
		short				day_counter_id;
		short				day_counter_value;
	};
}

#pragma pack(pop)

#endif	//__MSG_DAYCOUNTER_HPP__

