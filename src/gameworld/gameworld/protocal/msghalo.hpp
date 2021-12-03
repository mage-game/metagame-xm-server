#ifndef __MSG_HALO_H__
#define __MSG_HALO_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/halodef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCHaloInfo				// 6400 光环信息
	{
	public:
		SCHaloInfo();
		MessageHeader header;

		short halo_level;															// 等级
		short grade;																// 阶
		short star_level;															// 星级
		short used_imageid;															// 使用的形象
		short shuxingdan_count;														// 属性丹数量 
		short chengzhangdan_count;													// 成长丹数量
		int grade_bless_val;														// 进阶祝福值
		unsigned char active_image_flag[HaloParam::MAX_ACTIVE_IMAGE_BYTE];			// 激活的形象列表  & (1 << imageid) != 0，则激活imageid
		unsigned char active_special_image_flag[HaloParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];			// 激活的特殊形象列表
		unsigned int clear_upgrade_time;											// 清空祝福值的时间
		unsigned int reward_limit_time;												// 返回进阶丹的时间
		int equip_skill_level;														// 装备技能等级

		short equip_level_list[HaloParam::EQUIP_COUNT];								// 装备信息
		short skill_level_list[HaloParam::SKILL_COUNT];								// 技能等级
		char special_img_grade_list[HaloParam::MAX_SPECIAL_IMAGE_COUNT];			// 特殊形象阶数
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CSUpgradeHalo				// 6402 请求光环进阶
	{
	public:
		CSUpgradeHalo();
		MessageHeader header;

		short repeat_times;
		short auto_buy;
	};

	class CSUseHaloImage			// 6403 请求使用形象
	{
	public:
		CSUseHaloImage();
		MessageHeader header;

		short reserve_sh;
		short image_id;
	};

	class CSHaloGetInfo				// 6404 请求光环信息
	{
	public:
		CSHaloGetInfo();	
		MessageHeader header;
	};

	class CSHaloUplevelEquip		// 6405 光环升级装备请求
	{
	public:
		CSHaloUplevelEquip();	
		MessageHeader header;

		short equip_idx;			// 装备索引
		short reserve;				
	};

	class CSHaloSkillUplevelReq
	{
	public:
		CSHaloSkillUplevelReq();	// 6406 光环技能升级请求
		MessageHeader header;

		short skill_idx;
		short auto_buy;
	};

	class CSHaloSpecialImgUpgrade	// 6407 光环特殊形象进阶
	{
	public:
		CSHaloSpecialImgUpgrade();
		MessageHeader header;

		short special_image_id;
		short reserve_sh;
	};

	class CSHaloUpStarLevel		// 6408 光环升星级请求
	{
	public:
		CSHaloUpStarLevel();
		MessageHeader header;

		short stuff_index;
		short is_auto_buy;

		int loop_times;
	};
}

#pragma pack(pop)

#endif // 

