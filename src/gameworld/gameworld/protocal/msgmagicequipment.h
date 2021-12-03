#ifndef __MSG_MAGIC_EQUIPMENT_H__
#define __MSG_MAGIC_EQUIPMENT_H__

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/magiccarddef.hpp"
#include "servercommon/magicequipmentdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum MAGIC_EQUIPMENT_REQ_TYPE
	{
		MAGIC_EQUIPMENT_REQ_TYPE_UPGRADE = 0,								// 吞噬进阶：param1 魔器类型，param2 消耗数量
		MAGIC_EQUIPMENT_REQ_TYPE_STRENGTHEN,								// 锻造强化：param1 魔器类型，param2 是否自动强化， param3 是否自动购买
		MAGIC_EQUIPMENT_REQ_TYPE_EMBED,										// 镶嵌魔石：param1 魔器类型，param2 镶嵌孔位，param3 魔石下标（配置里的）
		MAGIC_EQUIPMENT_REQ_TYPE_TAKE_OFF_STONE,							// 卸下魔石：param1 魔器类型，param2 镶嵌孔位

		MAGIC_EQUIPMENT_REQ_TYPE_MAX,
	};

	class CSMagicEquipmentOperaReq											// 魔器操作请求   5690
	{
	public:
		CSMagicEquipmentOperaReq();
		MessageHeader header;

		short opera_type;
		short param_1;
		short param_2;
		short param_3;
	};

	class SCMagicEquipmentAllInfo											// 魔器所有信息  6141
	{
	public:
		struct SubItem
		{
			short quality_progress;											// 吞噬进度
			short quality_level;											// 品质等级

			int strengthen_bless;											// 强化祝福值
			short strengthen_level;											// 强化等级
			short stone_list[MAGIC_EQUIP_STONE_SLOT_COUNT];					// 镶嵌的魔石列表
			short reserve_sh;
		};

		SCMagicEquipmentAllInfo();
		MessageHeader header;

		SubItem equip_list[MAGIC_EQUIP_MAX_COUNT];
	};

	enum MAGIC_EQUIPMENT_CHANGE_NOTIFY_TYPE
	{
		MAGIC_EQUIPMENT_CHANGE_NOTIFY_TYPE_QUALITY_LEVEL = 0,				// 品质等级改变：param1 魔器类型，param2 魔器品质等级， param3 吞噬进度
		MAGIC_EQUIPMENT_CHANGE_NOTIFY_TYPE_STRENGTHEN_LEVEL,				// 锻造等级改变：param1 魔器类型，param2 魔器锻造等级， param3 锻造值（祝福值）
		MAGIC_EQUIPMENT_CHANGE_NOTIFY_TYPE_EMBED,							// 镶嵌魔石：param1 魔器类型，param2 魔石孔位， param3 魔石下标（配置里的）
		MAGIC_EQUIPMENT_CHANGE_NOTIFY_TYPE_TAKE_OFF,						// 脱下魔石：param1 魔器类型，param2 魔石孔位
		
		MAGIC_EQUIPMENT_CHANGE_NOTIFY_TYPE_MAX,
	};

	class SCMagicEquipmentChangeNotify										// 魔器信息增量更新  6142
	{
	public:
		SCMagicEquipmentChangeNotify();
		MessageHeader header;

		short notify_type;
		short reserve_sh;
		int param_1;
		int param_2;
		int param_3;
	};
}

#pragma pack(pop)

#endif

