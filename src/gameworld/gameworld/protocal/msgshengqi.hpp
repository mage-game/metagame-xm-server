#ifndef __MSG_SHENGQI_HPP__
#define __MSG_SHENGQI_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/shengqiparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCShengqiInfo									// 8704
	{
	public:
		SCShengqiInfo();
		MessageHeader header;

		struct SCShengqiItem
		{
			SCShengqiItem()
				: level(0), reserve_sh(0), spirit_flag(0)
			{
				memset(spirit_value, 0, sizeof(spirit_value));
				memset(per_spirit_value, 0, sizeof(per_spirit_value));
			}
			short level;										// 等级
			short reserve_sh;
			int spirit_flag;									// 开启的附灵列表
			int spirit_value[SHENGQI_SPIRIT_MAX_NUM];			// 附灵属性值
			int per_spirit_value[SHENGQI_SPIRIT_MAX_NUM];		// 附灵属性万分比
		};

		int activate_flag;									// 激活的圣器列表
		SCShengqiItem shengqi_item[SHENGQI_MAX_NUM];		// 圣器信息列表
	};

	enum SHENGQI_OPERA_TYPE
	{
		SHENGQI_OPERA_TYPE_ALL_INFO = 0,				// 圣器的所有信息
		SHENGQI_OPERA_TYPE_ACTIVATE,					// param1 圣器id
		SHENGQI_OPERA_TYPE_OPEN_SPIRIT,					// param1 圣器id param2 附灵属性索引
		SHENGQI_OPERA_TYPE_SPIRIT,						// param1 圣器id param2 附灵物品id
		SHENGQI_OPERA_TYPE_STRENGTH,					// param1 圣器id param2 强化物品id
		SHENGQI_DECOMPOSE,								// 材料分解操作,param1 材料id , param2 数量

		SHENGQI_OPERA_TYPE_MAX,
	};

	class CSShengqiReq									// 8705
	{
	public:
		CSShengqiReq();
		MessageHeader header;

		int req_type;
		int param1;
		int param2;
	};
}

#pragma pack(pop)

#endif // 


