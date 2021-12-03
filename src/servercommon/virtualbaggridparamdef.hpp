#ifndef __VIRTUAL_BAG_GRID_PARAM_H__
#define __VIRTUAL_BAG_GRID_PARAM_H__

/*************************************************
Copyright:

Author:Luyaoyuan

Date:2018-03-19

LastModifyDate:2018-03-21

Description: ���ⱳ���Ļ�����ӽṹ��,����ʹ�����ⱳ���Ķ�Ӧ�ü̳������;
			ͬʱ,�������޸�&��ӽṹ���Ա;��Ǳ�ҪҲ��Ӧ�����ӽӿ�;���ڼ̳���ʵ���Լ�����Ľӿ�
};
**************************************************/

#include "servercommon.h"

struct VirtualBagGridParam
{
	static const int INVALID_V_ITEM_ID = -1;
	VirtualBagGridParam() { this->Reset(); }

	bool IsEmpty() const { return v_item_id == INVALID_V_ITEM_ID; }
	bool operator<(const VirtualBagGridParam &r) const;
	void Reset();

	short v_item_id;
	short num;
	char is_bind;
	char param1;
	short param2;
};

inline bool VirtualBagGridParam::operator<(const VirtualBagGridParam &r) const
{
	if (this->v_item_id > r.v_item_id) return true;
	if (this->v_item_id < r.v_item_id) return false;
	if (this->num > r.num) return true;
	if (this->num < r.num) return false;

	return this->is_bind < r.is_bind;
}

inline void VirtualBagGridParam::Reset()
{
	v_item_id = INVALID_V_ITEM_ID;
	num = 0;
	is_bind = 0;
	param1 = 0;
	param2 = 0;
}

// static_assert(sizeof(VirtualBagGridParam) == 8, "sql Check Error,did not allow to modify or change");
UNSTD_STATIC_CHECK(sizeof(VirtualBagGridParam) == 8);

#endif // __SERVERCOMMON_H__

