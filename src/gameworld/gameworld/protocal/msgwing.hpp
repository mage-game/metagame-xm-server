#ifndef __MSG_WING_H__
#define __MSG_WING_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/wingdef.hpp"
#include "config/shuxingdanconfig.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCWingInfo				// 4703 羽翼信息
	{
	public:
		SCWingInfo();
		MessageHeader header;

		short wing_level;															// 等级
		short grade;																// 阶
		short star_level;															// 星级
		short used_imageid;															// 使用的形象
		unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];						// 属性丹数量
		int grade_bless_val;														// 进阶祝福值
		unsigned char active_image_flag[WingParam::MAX_ACTIVE_IMAGE_BYTE];					// 激活的形象列表
		unsigned char active_special_image_flag[WingParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];	// 激活的特殊形象列表
		unsigned int clear_upgrade_time;											// 清空祝福值的时间
		short temporary_imageid;													// 当前使用临时形象
		short temporary_imageid_has_select;											// 已选定的临时形象
		unsigned int temporary_imageid_invalid_time;								// 临时形象有效时间
		int equip_skill_level;														// 装备技能等级

		short equip_level_list[WingParam::EQUIP_COUNT];								// 装备信息
		short skill_level_list[WingParam::SKILL_COUNT];								// 技能等级
		char special_img_grade_list[WingParam::MAX_SPECIAL_IMAGE_COUNT];			// 特殊形象阶数
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CSUpgradeWing				// 4757 请求羽翼进阶
	{
	public:
		CSUpgradeWing();
		MessageHeader header;

		short repeat_times;
		short auto_buy;
	};

	class CSUseWingImage			// 4774 请求使用形象
	{
	public:
		CSUseWingImage();
		MessageHeader header;

		short is_temporary_image;
		short image_id;
	};

	class CSWingSpecialImgUpgrade	// 4789 羽翼特殊形象进阶
	{
	public:
		CSWingSpecialImgUpgrade();
		MessageHeader header;

		short special_image_id;
		short reserve_sh;
	};

	class CSWingUplevelEquip		// 4786 羽翼升级装备请求
	{
	public:
		CSWingUplevelEquip();	
		MessageHeader header;

		short equip_idx;			// 装备索引
		short reserve;				
	};

	class CSWingSkillUplevelReq
	{
	public:
		CSWingSkillUplevelReq();	// 4787 羽翼技能升级请求
		MessageHeader header;

		short skill_idx;
		short auto_buy;
	};

	class CSWingGetInfo				// 4775 请求羽翼信息
	{
	public:
		CSWingGetInfo();	
		MessageHeader header;
	};	
	
	class CSWingUpStarLevel		    // 4776 羽翼升星级请求
	{
	public:
		CSWingUpStarLevel();
		MessageHeader header;

		short stuff_index;
		short is_auto_buy;

		int loop_times;
	};
}

#pragma pack(pop)

#endif // 

