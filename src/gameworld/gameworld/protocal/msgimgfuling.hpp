#ifndef __MSG_IMG_FULING_HPP__
#define __MSG_IMG_FULING_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/imgfulingdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCImgFulingInfo															// 2570形象赋灵信息
	{
	public:
		SCImgFulingInfo();
		MessageHeader header;

		ImgTypeFulingParam fuling_list[JINGJIE_TYPE_COUNT];
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum IMG_FULING_OPERATE_TYPE
	{
		IMG_FULING_OPERATE_TYPE_INFO_REQ = 0,			// 请求信息
		IMG_FULING_OPERATE_TYPE_LEVEL_UP,				// 请求升级 param_1=>进阶系统类型  param_2=>花费物品索引
	};

	class CSImgFulingOperate			//  2571形象赋灵操作
	{
	public:
		CSImgFulingOperate();
		MessageHeader header;

		short operate_type;
		short param_1;
		short param_2;
		short param_3;
	};
}

#pragma pack(pop)

#endif // __MSG_IMG_FULING_HPP__

