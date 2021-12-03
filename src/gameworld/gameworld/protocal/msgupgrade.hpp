#ifndef __MSG_UPGRADE_H__
#define __MSG_UPGRADE_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/upgradeparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum UPGRADE_OPERA_TYPE
	{
		UPGRADE_OPERA_TYPE_INFO = 0,							// 信息	
		UPGRADE_OPERA_TYPE_USE_IMAGE,							// 使用形象 p1:is_temporary_image p2:image_id
		UPGRADE_OPERA_TYPE_FIGHT_OUT,							// 是否出战 p1:is_fight_out 
		UPGRADE_OPERA_TYPE_UPGRADE,								// 进阶 p1:repeat_times p2:auto_buy
		UPGRADE_OPERA_TYPE_IMAGE_UPGRADE,						// 形象进阶 p1:image_id
		UPGRADE_OPERA_TYPE_SKILL_UPGRADE,						// 技能进阶 p1:skill_idx p2:is_auto_buy
		UPGRADE_OPERA_TYPE_EQUIP_UPGRADE,						// 装备升级 p1:equip_idx

		UPGRADE_OPERA_TYPE_MAX,
	};

	class CSUpgradeOperaReq										// 8735 请求操作
	{
	public:
		CSUpgradeOperaReq();
		MessageHeader header;

		short upgrade_type;
		short opera_type;
		short param_1;
		short param_2;
		short param_3;
		short param_4;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class SCUpgradeInfo				// 8733 进阶系统信息
	{
	public:
		SCUpgradeInfo();
		MessageHeader header;

		short upgrade_type;															// 进阶系统类型
		short fightout_flag;														// fightout_flag  -------- & 1 != 0 则出战状态
		short level;																// 等级
		short grade;																// 阶
		short star_level;															// 星级
		short used_imageid;															// 使用的形象
		int grade_bless_val;														// 进阶祝福值
		unsigned int clear_upgrade_time;											// 清空祝福值的时间
		short temporary_imageid;													// 当前使用临时形象
		short temporary_imageid_has_select;											// 已选定的临时形象
		unsigned int temporary_imageid_invalid_time;								// 临时形象有效时间
		int equip_skill_level;														// 装备技能等级
		unsigned int last_upgrade_time;                                             // 上一次进阶成功时间

		UInt16 shuxingdan_list[SHUXINGDAN_MAX_TYPE];								// 属性丹数量
		short equip_level_list[UPGRADE_EQUIP_COUNT];								// 装备信息
		short skill_level_list[UPGRADE_SKILL_COUNT];								// 技能等级
		UInt8 active_img_flag[UPGRADE_MAX_IMAGE_BYTE];								// 形象激活标记
		UInt8 img_grade_list[UPGRADE_IMAGE_MAX_COUNT];								// 形象阶数列表

	};

	class SCUpgradeAppeChange			// 8734 进阶外观改变
	{
	public:
		SCUpgradeAppeChange();
		MessageHeader header;

		short upgrade_type;													// 进阶系统类型
		ObjID objid;														// 对象obj_id
		unsigned short upgrade_appeid;										// 外观信息id
		short reserve_sh;
	};
}

#pragma pack(pop)

#endif // 

