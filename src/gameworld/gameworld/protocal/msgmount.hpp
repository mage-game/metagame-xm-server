#ifndef __MSG_MOUNT_H__
#define __MSG_MOUNT_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/mountparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCMountInfo				// 4700 坐骑信息
	{
	public:
		SCMountInfo();
		MessageHeader header;

		short mount_flag;															// MOUNTFLAG  -------- & 1 != 0 在坐骑上，否则不在
		short reserved;
		short mount_level;															// 等级
		short grade;																// 阶
		short star_level;															// 星级
		short used_imageid;															// 使用的形象
		unsigned short shuxingdan_list[SHUXINGDAN_MAX_TYPE];						// 属性丹数量
		int grade_bless_val;														// 进阶祝福值
		unsigned char active_image_flag[MountParam::MAX_ACTIVE_IMAGE_BYTE];			// 激活的形象列表  & (1 << imageid) != 0，则激活imageid
		unsigned char active_special_image_flag[MountParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];	// 激活的特殊形象列表
		unsigned int clear_upgrade_time;											// 清空祝福值的时间
		short temporary_imageid;													// 当前使用临时形象
		short temporary_imageid_has_select;											// 已选定的临时形象
		unsigned int temporary_imageid_invalid_time;								// 临时形象有效时间
		int equip_skill_level;														// 装备技能等级

		short equip_level_list[MountParam::EQUIP_COUNT];							// 装备信息
		short skill_level_list[MountParam::SKILL_COUNT];							// 技能等级
		char special_img_grade_list[MountParam::MAX_SPECIAL_IMAGE_COUNT];			// 特殊形象阶数
		unsigned int last_upgrade_time;                                             //上一次进阶成功时间

	};

	class SCMountAppeChange			// 4701 坐骑外观改变
	{
	public:
		SCMountAppeChange();
		MessageHeader header;

		ObjID objid;
		unsigned short mount_appeid;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CSGoonMount				// 4750 请求上/下坐骑
	{
	public:
		CSGoonMount();
		MessageHeader header;

		int goon_mount;				// 1上，0下
	};

	class CSUpgradeMount			// 4751 请求坐骑进阶
	{
	public:
		CSUpgradeMount();
		MessageHeader header;

		short repeat_times;
		short auto_buy;
	};

	class CSUseMountImage			// 4752 请求使用形象
	{
	public:
		CSUseMountImage();
		MessageHeader header;

		short is_temporary_image;
		short image_id;
	};

	class CSMountSpecialImgUpgrade	// 4753 坐骑特殊形象进阶
	{
	public:
		CSMountSpecialImgUpgrade();
		MessageHeader header;

		short special_image_id;
		short reserve_sh;
	};

	class CSMountUplevelEquip		// 4754 坐骑升级装备请求
	{
	public:
		CSMountUplevelEquip();	
		MessageHeader header;

		short equip_idx;			// 装备索引
		short reserve;				
	};

	class CSMountSkillUplevelReq
	{
	public:
		CSMountSkillUplevelReq();	// 4755 坐骑技能升级请求
		MessageHeader header;

		short skill_idx;
		short auto_buy;
	};

	class CSMountGetInfo			// 4756 请求坐骑信息
	{
	public:
		CSMountGetInfo();	
		MessageHeader header;
	};

	class CSMountUpStarLevel		// 4758 坐骑升星级请求
	{	
	public:
		CSMountUpStarLevel();
		MessageHeader header;

		short stuff_index;
		short is_auto_buy;

		int loop_times;
	};
}

#pragma pack(pop)

#endif // 

