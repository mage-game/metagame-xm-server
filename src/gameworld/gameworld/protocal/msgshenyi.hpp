#ifndef __MSG_SHENYI_H__
#define __MSG_SHENYI_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/shenyidef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCShenyiInfo				// 6500 神翼信息
	{
	public:
		SCShenyiInfo();
		MessageHeader header;

		short star_level;															// 星级
		short shenyi_level;															// 等级
		short grade;																// 阶
		short used_imageid;															// 使用的形象
		int grade_bless_val;														// 进阶祝福值
		short shuxingdan_count;														// 属性丹数量 
		short chengzhangdan_count;													// 成长丹数量
		unsigned char active_image_flag[ShenyiParam::MAX_ACTIVE_IMAGE_BYTE];					// 激活的形象列表  & (1 << imageid) != 0，则激活imageid
		unsigned char active_special_image_flag[ShenyiParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];	// 激活的特殊形象列表
		unsigned int clear_upgrade_time;											// 清空祝福值的时间
		int equip_skill_level;														// 装备技能等级

		short equip_level_list[ShenyiParam::EQUIP_COUNT];							// 装备信息
		short skill_level_list[ShenyiParam::SKILL_COUNT];							// 技能等级
		char special_img_grade_list[ShenyiParam::MAX_SPECIAL_IMAGE_COUNT];			// 特殊形象阶数
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CSUpgradeShenyi				// 6502 请求神翼进阶
	{
	public:
		CSUpgradeShenyi();
		MessageHeader header;

		short repeat_times;
		short auto_buy;
	};

	class CSUseShenyiImage			// 6503 请求使用形象
	{
	public:
		CSUseShenyiImage();
		MessageHeader header;

		short reserve_sh;
		short image_id;
	};

	class CSShenyiGetInfo				// 6504 请求神翼信息
	{
	public:
		CSShenyiGetInfo();	
		MessageHeader header;
	};

	class CSShenyiUplevelEquip		// 6505 神翼升级装备请求
	{
	public:
		CSShenyiUplevelEquip();	
		MessageHeader header;

		short equip_idx;
		short reserve;
	};

	class CSShenyiSkillUplevelReq
	{
	public:
		CSShenyiSkillUplevelReq();	// 6506 神翼技能升级请求
		MessageHeader header;

		short skill_idx;
		short auto_buy;
	};

	class CSShenyiSpecialImgUpgrade	// 6507 神翼特殊形象进阶
	{
	public:
		CSShenyiSpecialImgUpgrade();
		MessageHeader header;

		short special_image_id;
		short reserve_sh;
	};

	class CSShenyiUpStarLevel	// 6508神翼升星级请求
	{
	public:
		CSShenyiUpStarLevel();
		MessageHeader header;

		short stuff_index;
		short is_auto_buy;

		int loop_times;
	};

	class CSUnUseShenyiImage			// 6509 请求取消使用形象
	{
	public:
		CSUnUseShenyiImage();
		MessageHeader header;

		short image_id;
		short reserve_sh;
	};
}

#pragma pack(pop)

#endif // 

