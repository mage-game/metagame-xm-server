#ifndef __MSG_FIGHT_MOUNT_H__
#define __MSG_FIGHT_MOUNT_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/fightmountparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCFightMountInfo				//  战斗坐骑信息  2550
	{
	public:
		SCFightMountInfo();
		MessageHeader header;

		short mount_flag;															// MOUNTFLAG  -------- & 1 != 0 在坐骑上，否则不在
		short reserved;
		short mount_level;															// 等级
		short grade;																// 阶
		short star_level;															// 星级
		short used_imageid;															// 使用的形象
		short shuxingdan_count;														// 属性丹数量 
		short chengzhangdan_count;													// 成长丹数量
		int grade_bless_val;														// 进阶祝福值
		unsigned char active_image_flag[FightMountParam::MAX_ACTIVE_IMAGE_BYTE];	// 激活的形象列表  & (1 << imageid) != 0，则激活imageid
		unsigned char active_special_image_flag[FightMountParam::MAX_ACTIVE_SPECIAL_IMAGE_BYTE];	// 激活的特殊形象列表
		unsigned int clear_upgrade_time;											// 清空祝福值的时间
		int equip_skill_level;														// 装备技能等级

		short equip_level_list[FightMountParam::EQUIP_COUNT];	// 装备信息
		short skill_level_list[FightMountParam::SKILL_COUNT];						// 技能等级
		char special_img_grade_list[FightMountParam::MAX_SPECIAL_IMAGE_COUNT];		// 特殊形象阶数
	};

	class SCFightMountAppeChange			//  坐骑外观改变
	{
	public:
		SCFightMountAppeChange();
		MessageHeader header;

		ObjID objid;
		unsigned short mount_appeid;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CSGoonFightMount				//  请求上/下坐骑
	{
	public:
		CSGoonFightMount();
		MessageHeader header;

		int goon_mount;				// 1上，0下
	};

	class CSUpgradeFightMount			//  请求坐骑进阶
	{
	public:
		CSUpgradeFightMount();
		MessageHeader header;

		short repeat_times;
		short auto_buy;
	};

	class CSUseFightMountImage			//  请求使用形象
	{
	public:
		CSUseFightMountImage();
		MessageHeader header;

		short reserve_sh;
		short image_id;
	};

	class CSFightMountSpecialImgUpgrade	//  坐骑特殊形象进阶
	{
	public:
		CSFightMountSpecialImgUpgrade();
		MessageHeader header;

		short special_image_id;
		short reserve_sh;
	};

	class CSFightMountUplevelEquip		//  坐骑升级装备请求
	{
	public:
		CSFightMountUplevelEquip();	
		MessageHeader header;

		char equip_idx;
		short reserve;
	};

	class CSFightMountSkillUplevelReq
	{
	public:
		CSFightMountSkillUplevelReq();	//  坐骑技能升级请求
		MessageHeader header;

		short skill_idx;
		short auto_buy;
	};

	class CSFightMountGetInfo			//  请求坐骑信息
	{
	public:
		CSFightMountGetInfo();	
		MessageHeader header;
	};

	class CSFightMountUpStarLevel		// 战斗坐骑升星级请求
	{
	public:
		CSFightMountUpStarLevel();
		MessageHeader header;

		short stuff_index;
		short is_auto_buy;

		int loop_times;
	};
}

#pragma pack(pop)

#endif // 

