#ifndef __MSG_ZODIAC_HPP__
#define __MSG_ZODIAC_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/zodiacparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCZodiacInfo									// 8972 生肖信息
	{
	public:
		SCZodiacInfo();
		MessageHeader header;

		struct SCZodiacItem
		{
			SCZodiacItem()
				: level(0), activate_flag(0), reserve_ch(0) {}

			short level;								// 等级
			char activate_flag;							// 碎片激活开启标志
			char reserve_ch;
		};

		SCZodiacItem zodiac_item[ZODIAC_MAX_NUM];		// 十二生肖信息列表
	};

	class SCZodiacBackpackInfo							// 8973 背包信息
	{
	public:
		struct GridItem
		{
			unsigned short item_id;
			short zodiac_index;
			short suipian_index;
			short reserve_sh;
		};

		SCZodiacBackpackInfo();
		MessageHeader header;

		char reserve_ch1;
		char reserve_ch2;

		short grid_num;									// 格子数量
		GridItem grid_list[ZODIAC_BACKPACK_MAX_GRIDS];	// 格子信息列表
	};

	enum ZODIAC_OPERA_TYPE
	{
		ZODIAC_OPERA_TYPE_ALL_INFO = 0,				// 十二生肖的所有信息
		ZODIAC_OPERA_TYPE_ACTIVATE,					// 激活 param1 背包格子index 
		ZODIAC_OPERA_TYPE_LEVELUP,					// 升级 param1 生肖索引
		ZODIAC_OPERA_TYPE_DECOMPOSE,				// 分解 param1 分解数量，param2背包格子index

		ZODIAC_OPERA_TYPE_MAX,
	};

	class CSZodiacReq									// 8974 操作请求
	{
	public:
		CSZodiacReq();
		MessageHeader header;

		int req_type;
		int param1;
		int param2;
	};

	class CSZodiacDecomposeReq							// 8975 分解请求
	{
	public:
		CSZodiacDecomposeReq();
		MessageHeader header;

		int decompose_num;														// 分解个数
		int decompose_backpack_index_list[ZODIAC_BACKPACK_MAX_GRIDS];			// 分解下标列表
	};

	class SCZodiacBaseInfo								// 8976 精华信息
	{
	public:
		SCZodiacBaseInfo();
		MessageHeader header;

		int jinghua_num;							// 精华数
	};
}

#pragma pack(pop)

#endif // 

