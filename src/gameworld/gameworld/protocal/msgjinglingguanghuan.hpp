#ifndef __MSG_JINGLINGGUANGHUAN_H__
#define __MSG_JINGLINGGUANGHUAN_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/jinglingguanghuandef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCJinglingGuanghuanInfo				// 6875 精灵光环信息
	{
	public:
		SCJinglingGuanghuanInfo();
		MessageHeader header;

		short reserve;															
		short grade;																// 阶
		short used_imageid;															// 使用的形象
		short reserve_2;
		int active_image_flag;														// 激活的形象列表  & (1 << imageid) != 0，则激活imageid
		int grade_bless_val;														// 进阶祝福值
		int active_special_image_flag;												// 激活的特殊形象列表

		char special_img_grade_list[JinglingGuanghuanParam::MAX_SPECIAL_IMAGE_ID + 1];	// 特殊形象阶数
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CSUseJinglingGuanghuanImage			// 6851 请求使用形象
	{
	public:
		CSUseJinglingGuanghuanImage();
		MessageHeader header;

		short reserve_sh;
		short image_id;
	};

	class CSJinglingGuanghuanGetInfo				// 6852 请求精灵光环信息
	{
	public:
		CSJinglingGuanghuanGetInfo();	
		MessageHeader header;
	};

	class CSJinglingGuanghuanUpStarLevel			// 6850 升星级请求
	{
	public:
		CSJinglingGuanghuanUpStarLevel();
		MessageHeader header;

		int is_auto_buy;
	};

	class CSJinglingGuanghuanSpecialImgUpgrade			// 6853 精灵光环特殊形象进阶
	{
	public:
		CSJinglingGuanghuanSpecialImgUpgrade();
		MessageHeader header;

		short special_image_id;
		short reserve_sh;
	};

}

#pragma pack(pop)

#endif // 

