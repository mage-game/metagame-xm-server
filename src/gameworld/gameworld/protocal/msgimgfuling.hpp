#ifndef __MSG_IMG_FULING_HPP__
#define __MSG_IMG_FULING_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/imgfulingdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCImgFulingInfo															// 2570��������Ϣ
	{
	public:
		SCImgFulingInfo();
		MessageHeader header;

		ImgTypeFulingParam fuling_list[JINGJIE_TYPE_COUNT];
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum IMG_FULING_OPERATE_TYPE
	{
		IMG_FULING_OPERATE_TYPE_INFO_REQ = 0,			// ������Ϣ
		IMG_FULING_OPERATE_TYPE_LEVEL_UP,				// �������� param_1=>����ϵͳ����  param_2=>������Ʒ����
	};

	class CSImgFulingOperate			//  2571���������
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

