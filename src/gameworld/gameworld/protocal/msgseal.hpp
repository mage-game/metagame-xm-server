#ifndef __MSG_SEAL_HPP__
#define __MSG_SEAL_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/sealparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum SEAL_OPERA_TYPE
	{
		SEAL_OPERA_TYPE_ALL_INFO = 0,				// 请求所有信息
		SEAL_OPERA_TYPE_PUT_ON,						// 装备， param1 背包格子index
		SEAL_OPERA_TYPE_UPLEVLE,					// 升级， param1 圣印孔索引index
		SEAL_OPERA_TYPE_RECYCLE,					// 分解单个， param1 分解数量，param2背包格子index
		SEAL_OPERA_TYPE_USE_SOUL,					// 使用圣魂 param1 类型，param2 数量

		SEAL_OPERA_TYPE_MAX,
	};

	class CSSealReq									// 8706
	{
	public:
		CSSealReq();
		MessageHeader header;

		int req_type;
		int param1;
		int param2;
	};

	class SCSealBackpackInfo						// 8707
	{
	public:
		struct GridItem
		{
			short index;
			short slot_index;
			short order;
			short level;
			unsigned short item_id;
			unsigned short reserve_sh;
		};

		SCSealBackpackInfo();
		MessageHeader header;

		char reserve_1;
		char reserve_2;

		short grid_num;															// 格子数量
		GridItem grid_list[SEAL_BACKPACK_MAX_GRIDS];							// 格子信息列表
	};

	class SCSealSlotInfo							// 8708
	{
	public:
		SCSealSlotInfo();
		MessageHeader header;

		SealItem grid_list[SEAL_SLOT_MAX_NUM];		// 圣印孔列表
	};

	class SCSealBaseInfo							// 8709
	{
	public:
		SCSealBaseInfo();
		MessageHeader header;

		int hun_score;								// 灵魂数
		int soul_list[SEAL_SOUL_MAX_TYPE];			// 圣魂列表
	};

	class CSSealRecycleReq							// 分解  8710
	{
	public:
		CSSealRecycleReq();
		MessageHeader header;

		int recycle_num;														// 分解个数
		int recycle_backpack_index_list[SEAL_BACKPACK_MAX_GRIDS];				// 分解下标列表
	};
}

#pragma pack(pop)

#endif // 

