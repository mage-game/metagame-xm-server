#ifndef __MSG_IMP_GUARD_H_
#define __MSG_IMP_GUARD_H_

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/cardzudef.hpp"
#include "other/impguard/impguard.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum IMP_GUARD_REQ_TYPE
	{
		IMP_GUARD_REQ_TYPE_RENEW_PUTON = 0,			// 续费穿在身上的小鬼 param1:身上小鬼格子index  param2:是否使用绑元
		IMP_GUARD_REQ_TYPE_RENEW_KNAPSACK,			// 续费背包中的小鬼 param1:背包index param2:是否使用绑元
		IMP_GUARD_REQ_TYPE_TAKEOFF,					// 脱下小鬼 param1:身上小鬼格子index  0 1
		IMP_GUARD_REQ_TYPE_ALL_INFO,

		IMP_GUARD_REQ_TYPE_MAX,
	}; 

	class CSImpGuardOperaReq						// 8805
	{
	public:
		CSImpGuardOperaReq();
		MessageHeader header;

		int opera_type;
		short param1;
		short param2;
	};

	class SCImpGuardInfo							// 8806
	{
	public:
		SCImpGuardInfo();
		MessageHeader header;

		struct ImpGuardData
		{
			char grid_index;
			char used_imp_type;
			char is_expire;			// 是否过期。 1过期，0没有
			char res_ch;
			ItemDataWrapper item_wrapper;
		};

		ImpGuardData imp_guard_data[ImpGuard::IMP_GUARD_GRID_INDEX_MAX];   // 2
	};

	class SCRoleImpAppeChange         //8807    小鬼守护外观变换通知范围内的玩家
	{
	public:
		SCRoleImpAppeChange();
		MessageHeader header;

		ObjID objid;                    // 发生变换小鬼的主人的objid
		short appe_index;               // 发生变化的小鬼index   0或1
		short appe_id;
	};

	class SCRoleImpExpireTime         // 8811    通知小鬼到期   （又不用了，先不删了，放在这，以后可以利用）
	{
	public:
		SCRoleImpExpireTime();
		MessageHeader header;

		short grid_index;               // 发生变化的小鬼index   0或1
		short res_sh;
	};
}

#pragma pack(pop)

#endif

