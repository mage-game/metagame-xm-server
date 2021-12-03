#ifndef __MSG_WUSHANG_EQUIP_HPP__
#define __MSG_WUSHANG_EQUIP_HPP__

#include "servercommon/wushangequipdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum WUSHANG_EQUIP_REQ_TYPE
	{
		WUSHANG_REQ_TYPE_ALL_INFO = 0,						// 所有信息请求
		WUSHANG_REQ_TYPE_PUT_ON_EQUIP = 1,					// 穿装备
		WUSHANG_REQ_TYPE_TAKE_OFF_EQUIP = 2,				// 脱装备
		WUSHANG_REQ_TYPE_JIFEN_EXCHANGE = 3,				// 积分兑换
		WUSHANG_REQ_TYPE_STRENGTHEN = 4,					// 强化
		WUSHANG_REQ_TYPE_UP_STAR = 5,						// 升星
		WUSHANG_REQ_TYPE_GLORY_EXCHANGE = 6,				// 荣耀兑换
	};

	struct WushangEquipStarInfo
	{
		WushangEquipStarInfo() : star_level(0), jinhua_val(0) {}

		short star_level;	// 星级
		short jinhua_val;	// 进化值
	};

	class CSWushangEquipOpearReq							// 无上神兵操作请求 6132
	{
	public:
		CSWushangEquipOpearReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
		int param_1;
		int param_2;
		int param_3;
	};

	class SCWushangEquipAllInfo								// 无上神兵所有信息 6133
	{
	public:
		SCWushangEquipAllInfo();
		MessageHeader header;
		
		int wushang_jifen;													// 积分
		unsigned short equip_id_list[WUSHANGEQUIP_MAX_TYPE_COUNT];
		char strong_level_list[WUSHANGEQUIP_MAX_TYPE_COUNT];				// 强化等级
		WushangEquipStarInfo star_list[WUSHANGEQUIP_MAX_TYPE_COUNT];		// 星级
		int glory;															// 荣耀
		int jy_glory;														// 精英荣耀
		int cross_honor;													// 跨服荣誉
	};
}

#pragma pack(pop)

#endif

