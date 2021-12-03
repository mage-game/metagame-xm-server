#ifndef __MSG_JINGLINGFAZHEN_H__
#define __MSG_JINGLINGFAZHEN_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/jinglingfazhendef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCJinglingFazhenInfo				// 6925 精灵法阵信息
	{
	public:
		SCJinglingFazhenInfo();
		MessageHeader header;

		short reserve;															
		short grade;																// 阶
		short used_imageid;															// 使用的形象
		short reserve_2;
		int active_image_flag;														// 激活的形象列表  & (1 << imageid) != 0，则激活imageid
		int grade_bless_val;														// 进阶祝福值
		int active_special_image_flag;												// 激活的特殊形象列表

		char special_img_grade_list[JinglingFazhenParam::MAX_SPECIAL_IMAGE_ID + 1];	// 特殊形象阶数
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CSUseJinglingFazhenImage			// 6901 请求使用形象
	{
	public:
		CSUseJinglingFazhenImage();
		MessageHeader header;

		short reserve_sh;
		short image_id;
	};

	class CSJinglingFazhenGetInfo				// 6902 请求精灵法阵护信息
	{
	public:
		CSJinglingFazhenGetInfo();	
		MessageHeader header;
	};

	class CSJinglingFazhenUpStarLevel			// 6900 升星级请求
	{
	public:
		CSJinglingFazhenUpStarLevel();
		MessageHeader header;

		int is_auto_buy;
	};

	class CSJinglingFazhenSpecialImgUpgrade			// 6903 精灵法阵特殊形象进阶
	{
	public:
		CSJinglingFazhenSpecialImgUpgrade();
		MessageHeader header;

		short special_image_id;
		short reserve_sh;
	};

}

#pragma pack(pop)

#endif // 

