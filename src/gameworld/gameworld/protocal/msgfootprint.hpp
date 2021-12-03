#ifndef __MSG_FOOTPRINT_H__
#define __MSG_FOOTPRINT_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/footprintdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCFootprintInfo				//  6520足迹信息
	{
	public:
		SCFootprintInfo();
		MessageHeader header;

		short footprint_level;														// 等级
		short grade;																// 阶
		short star_level;															// 星级
		short used_imageid;															// 使用的形象
		short shuxingdan_count;														// 属性丹数量 
		short chengzhangdan_count;													// 成长丹数量
		int grade_bless_val;														// 进阶祝福值
		unsigned char active_image_flag[FootprintParam::MAX_ACTIVE_IMAGE_BYTE];		// 激活的形象列表
		unsigned char active_special_image_flag[FootprintParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];		// 激活的特殊形象列表
		unsigned int clear_upgrade_time;											// 清空祝福值的时间
		int equip_skill_level;														// 装备技能等级

		short equip_level_list[FootprintParam::EQUIP_COUNT];						// 装备信息
		short skill_level_list[FootprintParam::SKILL_COUNT];						// 技能等级
		char special_img_grade_list[FootprintParam::MAX_SPECIAL_IMAGE_COUNT];		// 特殊形象阶数
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum FOOTPRINT_OPERATE_TYPE
	{
		FOOTPRINT_OPERATE_TYPE_INFO_REQ = 0,			// 请求信息
		FOOTPRINT_OPERATE_TYPE_UP_GRADE,				// 请求进阶 param_1=>repeat_times  param_2=>auto_buy
		FOOTPRINT_OPERATE_TYPE_USE_IMAGE,				// 请求使用形象 param_1=>image_id
		FOOTPRINT_OPERATE_TYPE_UP_LEVEL_EQUIP,			// 请求升级装备 param_1=>equip_idx
		FOOTPRINT_OPERATE_TYPE_UP_STAR,					// 请求升星 param_1=>stuff_index param_2=>is_auto_buy param_3=>loop_times
		FOOTPRINT_OPERATE_TYPE_UP_LEVEL_SKILL,			// 请求升级技能 param_1=>skill_idx param_2=>auto_buy
		FOOTPRINT_OPERATE_TYPE_UP_SPECIAL_IMAGE,		// 请求升特殊形象进阶 param_1=>special_image_id
	};

	class CSFootprintOperate			//  6521足迹操作
	{
	public:
		CSFootprintOperate();
		MessageHeader header;

		short operate_type;
		short param_1;
		short param_2;
		short param_3;
	};
}

#pragma pack(pop)

#endif // 

