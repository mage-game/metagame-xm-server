#ifndef __MSG_XIANNVSHOUHU_H__
#define __MSG_XIANNVSHOUHU_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/xiannvshouhudef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCXiannvShouhuInfo				// 6825 仙女守护信息
	{
	public:
		SCXiannvShouhuInfo();
		MessageHeader header;

		short star_level;															// 星级
		short grade;																// 阶
		short used_imageid;															// 使用的形象
		short reserve;
		int active_image_flag;														// 激活的形象列表  & (1 << imageid) != 0，则激活imageid
		int grade_bless_val;														// 进阶祝福值
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CSUseXiannvShouhuImage			// 6801 请求使用形象
	{
	public:
		CSUseXiannvShouhuImage();
		MessageHeader header;

		short reserve_sh;
		short image_id;
	};

	class CSXiannvShouhuGetInfo				// 6802 请求仙女守护信息
	{
	public:
		CSXiannvShouhuGetInfo();	
		MessageHeader header;
	};

	class CSXiannvShouhuUpStarLevel			// 6800 升星级请求
	{
	public:
		CSXiannvShouhuUpStarLevel();
		MessageHeader header;

		short stuff_index;
		short is_auto_buy;
	};

}

#pragma pack(pop)

#endif // 

