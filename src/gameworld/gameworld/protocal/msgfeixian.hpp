#ifndef __MSG_FEIXIAN_H__
#define __MSG_FEIXIAN_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/feixianparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{

	class SCFeixianEquipInfo
	{
	public:
		SCFeixianEquipInfo();
		MessageHeader header;

		ItemDataWrapper equip_list[FeiXianParam::E_INDEX_MAX];
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 飞仙装备操作类型
	enum XIANYUAN_EQUIP_OPERATE_TYPE
	{
		FEIXIAN_EQUIP_OPERATE_TYPE_REQ_INFO = 0,		// 请求信息
		FEIXIAN_EQUIP_OPERATE_TYPE_TAKE_OFF,			// 脱下(param1 = '装备索引')
		FEIXIAN_EQUIP_OPERATE_TYPE_COMPOSE,				// 合成(param1 = '装备索引',param2 = '背包装备索引',param3 = '0(第一个参数是背包里装备索引),1(第一个参数是穿在身上的装备索引)')
		FEIXIAN_EQUIP_OPERATE_TYPE_LEVELUP,				// 升级(param1 = '装备索引' param2 = '0(第一个参数是背包里装备索引),1(第一个参数是穿在身上的装备索引)')
	};

	class CSFeixianEquipOpe
	{
	public:
		CSFeixianEquipOpe();
		MessageHeader header;						

		short operate;                                  // 操作类型（0 = 请求信息，1 = 脱下...）
		short param1;
		short param2;
		short param3;
	};
}

#pragma pack(pop)

#endif

