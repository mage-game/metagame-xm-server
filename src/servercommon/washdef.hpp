#ifndef __WASH_DEF_HPP__
#define __WASH_DEF_HPP__

#include "servercommon.h"

#pragma pack(push, 4)

static const int MAX_WASH_ATTR_COUNT = 4;
static const int MAX_WASH_EQUIP_PART = 12;

struct WashAttrItem
{
	WashAttrItem() { this->Reset(); }

	void Reset()
	{
		attr_type = 0;
		reserve = 0;
		attr_value = 0;
	}

	short attr_type;
	short reserve;

	int attr_value;
};

struct WashOnPart
{
	WashOnPart()
	{
		this->Reset();
	}

	void Reset()
	{
		lucky = 0;
		capability = 0;

		for (int i = 0; i < MAX_WASH_ATTR_COUNT; ++ i)
		{
			attr_list[i].Reset();
		}
	}

	int lucky;
	int capability;
	WashAttrItem attr_list[MAX_WASH_ATTR_COUNT];
};

struct WashParam
{
	WashParam() { this->Reset(); }

	void Reset()
	{
		reserve_1 = 0;
		reserve_2 = 0;
		
		for (int i = 0; i < MAX_WASH_EQUIP_PART; ++ i)
		{
			part_list[i].Reset();
		}

		for (int i = 0; i < MAX_WASH_EQUIP_PART; ++ i)
		{
			notsave_part_list[i].Reset();
		}
	}

	short reserve_1;	
	short reserve_2;		

	WashOnPart part_list[MAX_WASH_EQUIP_PART];
	WashOnPart notsave_part_list[MAX_WASH_EQUIP_PART];
};

typedef char WashParamHex[sizeof(WashParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(WashParamHex) < 2048);

#pragma pack(pop)

#endif 

