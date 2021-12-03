#ifndef __MSG_CLOAK_H__
#define __MSG_CLOAK_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/cloakdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCCloakInfo				// 6525披风信息
	{
	public:
		SCCloakInfo();
		MessageHeader header;

		int cloak_level;															// 等级
		int cur_exp;																// 当前经验
		int used_imageid;															// 使用的形象

		int active_image_flag;														// 激活的形象列表  & (1 << imageid) != 0，则激活imageid
		unsigned long long active_special_image_flag;								// 激活的特殊形象列表
		int equip_skill_level;														// 装备技能等级

		unsigned short shuxingdan_list[CloakParam::SHUXINGDAN_MAX_TYPE];			// 属性丹数量
		short equip_level_list[CloakParam::EQUIP_COUNT];							// 装备信息
		short skill_level_list[CloakParam::SKILL_COUNT];							// 技能等级
		char special_img_grade_list[CloakParam::MAX_SPECIAL_IMAGE_ID + 1];			// 特殊形象阶数
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum CLOAK_OPERATE_TYPE
	{
		CLOAK_OPERATE_TYPE_INFO_REQ = 0,			// 请求信息
		CLOAK_OPERATE_TYPE_UP_LEVEL,				// 请求升级 param_1=>stuff_index param_2=>is_auto_buy param_3=>loop_times
		CLOAK_OPERATE_TYPE_USE_IMAGE,				// 请求使用形象 param_1=>image_id

		CLOAK_OPERATE_TYPE_UP_SPECIAL_IMAGE,		// 请求升特殊形象进阶 param_1=>special_image_id
		CLOAK_OPERATE_TYPE_UP_LEVEL_EQUIP,			// 请求升级装备 param_1=>equip_idx
		CLOAK_OPERATE_TYPE_UP_LEVEL_SKILL,			// 请求升级技能 param_1=>skill_idx param_2=>auto_buy
		
	};

	class CSCloakOperate			//  6526 披风操作
	{
	public:
		CSCloakOperate();
		MessageHeader header;

		short operate_type;
		short param_1;
		short param_2;
		short param_3;
	};
}

#pragma pack(pop)

#endif // 

