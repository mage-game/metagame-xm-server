#ifndef __MSG_MASK_H__
#define __MSG_MASK_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/maskparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCMaskInfo																// 8760 面具信息
	{
	public:
		SCMaskInfo();
		MessageHeader header;

		short mask_level;															// 等级
		short grade;																// 阶
		short star_level;															// 星级
		short used_imageid;															// 使用的形象
		int grade_bless_val;														// 进阶祝福值
		unsigned char active_image_flag[MaskParam::MAX_ACTIVE_IMAGE_BYTE];			// 激活的形象列表
		unsigned char active_special_image_flag[MaskParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];	// 激活的特殊形象列表
		unsigned int clear_upgrade_time;											// 清空祝福值的时间
		short temporary_imageid;													// 当前使用临时形象
		short temporary_imageid_has_select;											// 已选定的临时形象
		unsigned int temporary_imageid_invalid_time;								// 临时形象有效时间

		unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];						// 属性丹数量
		short skill_level_list[MaskParam::SKILL_COUNT];								// 技能等级
		char special_img_grade_list[MaskParam::MAX_SPECIAL_IMAGE_COUNT];			// 特殊形象阶数

	};

	class SCMaskAppeChange														// 8761 面具外观改变
	{
	public:
		SCMaskAppeChange();
		MessageHeader header;

		ObjID objid;
		unsigned short mask_appeid;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum MASK_OPERA_TYPE
	{
		MASK_OPERA_TYPE_INFO = 0,								// 信息	
		MASK_OPERA_TYPE_UPGRADE,								// 进阶 p1:repeat_times p2:auto_buy
		MASK_OPERA_TYPE_SPECIAL_IMAGE_UPGRADE,					// 特殊形象进阶 p1:special_image_id
		MASK_OPERA_TYPE_USE_IMAGE,								// 使用形象 p1:image_id
		MASK_OPERA_TYPE_SKILL_UPGRADE,							// 技能进阶 p1:skill_idx p2:is_auto_buy
	};

	class CSMaskOperaReq															// 8762 请求操作
	{
	public:
		CSMaskOperaReq();
		MessageHeader header;

		short opera_type;
		short param_1;
		short param_2;
		short param_3;
	};

}

#pragma pack(pop)

#endif // 

