#ifndef __MSG_YAOSHI_H__
#define __MSG_YAOSHI_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/yaoshiparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCYaoShiInfo																// 8730 腰饰信息
	{
	public:
		SCYaoShiInfo();
		MessageHeader header;

		short yaoshi_level;															// 等级
		short grade;																// 阶
		short star_level;															// 星级
		short used_imageid;															// 使用的形象
		int grade_bless_val;														// 进阶祝福值
		unsigned char active_image_flag[YaoShiParam::MAX_ACTIVE_IMAGE_BYTE];		// 激活的形象列表
		unsigned char active_special_image_flag[YaoShiParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];	// 激活的特殊形象列表
		unsigned int clear_upgrade_time;											// 清空祝福值的时间
		short temporary_imageid;													// 当前使用临时形象
		short temporary_imageid_has_select;											// 已选定的临时形象
		unsigned int temporary_imageid_invalid_time;								// 临时形象有效时间

		unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];						// 属性丹数量
		short skill_level_list[YaoShiParam::SKILL_COUNT];							// 技能等级
		char special_img_grade_list[YaoShiParam::MAX_SPECIAL_IMAGE_COUNT];			// 特殊形象阶数

	};

	class SCYaoShiAppeChange														// 8731 腰饰外观改变
	{
	public:
		SCYaoShiAppeChange();
		MessageHeader header;

		ObjID objid;
		unsigned short yaoshi_appeid;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum YAOSHI_OPERA_TYPE
	{
		YAOSHI_OPERA_TYPE_INFO = 0,								// 信息	
		YAOSHI_OPERA_TYPE_UPGRADE,								// 进阶 p1:repeat_times p2:auto_buy
		YAOSHI_OPERA_TYPE_SPECIAL_IMAGE_UPGRADE,				// 特殊形象进阶 p1:special_image_id
		YAOSHI_OPERA_TYPE_USE_IMAGE,							// 使用形象 p1:image_id
		YAOSHI_OPERA_TYPE_SKILL_UPGRADE,						// 技能进阶 p1:skill_idx p2:is_auto_buy
	};

	class CSYaoShiOperaReq															// 8732 请求操作
	{
	public:
		CSYaoShiOperaReq();
		MessageHeader header;

		short opera_type;
		short param_1;
		short param_2;
		short param_3;
	};

}

#pragma pack(pop)

#endif // 

