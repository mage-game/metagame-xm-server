#ifndef __MSG_SHENGONG_H__
#define __MSG_SHENGONG_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/shengongdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCShengongInfo				// 6450 神弓信息
	{
	public:
		SCShengongInfo();
		MessageHeader header;

		short star_level;															// 星级
		short shengong_level;														// 等级
		short grade;																// 阶
		short used_imageid;															// 使用的形象
		int grade_bless_val;														// 进阶祝福值
		short shuxingdan_count;														// 属性丹数量 
		short chengzhangdan_count;													// 成长丹数量
		unsigned char active_image_flag[ShengongParam::MAX_ACTIVE_IMAGE_BYTE];		// 激活的形象列表
		unsigned char active_special_image_flag[ShengongParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];	// 激活的特殊形象列表
		unsigned int clear_upgrade_time;											// 清空祝福值的时间
		int equip_skill_level;														// 装备技能等级

		short equip_level_list[ShengongParam::EQUIP_COUNT];							// 装备信息
		short skill_level_list[ShengongParam::SKILL_COUNT];							// 技能等级
		char special_img_grade_list[ShengongParam::MAX_SPECIAL_IMAGE_COUNT];			// 特殊形象阶数
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CSUpgradeShengong				// 6452 请求神弓进阶
	{
	public:
		CSUpgradeShengong();
		MessageHeader header;

		short repeat_times;
		short auto_buy;
	};

	class CSUseShengongImage			// 6453 请求使用形象
	{
	public:
		CSUseShengongImage();
		MessageHeader header;

		short reserve_sh;
		short image_id;
	};

	class CSShengongGetInfo				// 6454 请求神弓信息
	{
	public:
		CSShengongGetInfo();	
		MessageHeader header;
	};


	class CSShengongUplevelEquip		// 6455 神弓升级装备请求
	{
	public:
		CSShengongUplevelEquip();	
		MessageHeader header;

		char equip_idx;
		short reserve;
	};

	class CSShengongSkillUplevelReq
	{
	public:
		CSShengongSkillUplevelReq();	// 6456 神弓技能升级请求
		MessageHeader header;

		short skill_idx;
		short auto_buy;
	};

	class CSShengongSpecialImgUpgrade	// 6457 神弓特殊形象进阶
	{
	public:
		CSShengongSpecialImgUpgrade();
		MessageHeader header;

		short special_image_id;
		short reserve_sh;
	};


	class CSShengongUpStarLevel		// 6458神弓升星级请求
	{
	public:
		CSShengongUpStarLevel();
		MessageHeader header;

		short stuff_index;
		short is_auto_buy;

		int loop_times;
	};

	class CSUnUseShengongImage			// 6459 请求取消使用形象
	{
	public:
		CSUnUseShengongImage();
		MessageHeader header;

		short image_id;
		short reserve_sh;
	};
}

#pragma pack(pop)

#endif // 

