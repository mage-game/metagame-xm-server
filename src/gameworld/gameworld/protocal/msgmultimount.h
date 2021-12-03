#ifndef _MSG_MULTI_MOUNT_H_
#define _MSG_MULTI_MOUNT_H_

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/multimountdef.hpp"
#include "servercommon/newmultimountdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum MULTI_MOUNT_REQ_TYPE
	{
		MULTI_MOUNT_REQ_TYPE_SELECT_MOUNT = 0,								// 选择使用坐骑：param1 坐骑id
		MULTI_MOUNT_REQ_TYPE_UPGRADE,										// 坐骑进阶：param1 坐骑id, param2 重复次数，param3 是否自动购买
		MULTI_MOUNT_REQ_TYPE_RIDE,											// 上坐骑
		MULTI_MOUNT_REQ_TYPE_UNRIDE,										// 下坐骑
		MULTI_MOUNT_REQ_TYPE_INVITE_RIDE,									// 邀请骑乘：param1 玩家id
		MULTI_MOUNT_REQ_TYPE_INVITE_RIDE_ACK,								// 回应邀请骑乘：param1 玩家id，param2 是否同意
		MULTI_MOUNT_REQ_TYPE_USE_SPECIAL_IMG,								// 请求使用幻化形象：param1 特殊形象ID
		MULTI_MOUNT_REQ_TYPE_UPLEVEL_SPECIAL_IMG,							// 请求升级特殊形象：param1 特殊形象ID
	};

	class CSMultiMountOperaReq												// 双人坐骑操作请求 5686
	{
	public:
		CSMultiMountOperaReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
		int param_1;
		int param_2;
		int param_3;
	};

	class SCMultiMountAllInfo												// 所有坐骑信息 6123
	{
	public:
		struct MountItem
		{
			char is_mount_active;											// 坐骑是否已激活
			char is_img_active;												// 坐骑形象是否已激活
			short reserve_sh;

			short grade_bless;												// 进阶祝福值
			short star_level;												// 星级
		};

		SCMultiMountAllInfo();
		MessageHeader header;

		short cur_use_mount_id;
		short cur_use_special_image_id;										// 当前使用的特殊形象id,0不使用任何特殊形象

		MountItem mount_list[COUPLEMOUNT_MAX_ID + 1];

		unsigned int special_img_active_flag;								// 特殊坐骑激活位标记, 0未激活，1激活

		char special_img_level_list[COUPLEMOUNT_SPECIAL_IMG_MAX_ID_LIMIT];
		char reserve_ch;
	};

	enum MULTI_MOUNT_CHANGE_NOTIFY_TYPE
	{
		MULTI_MOUNT_CHANGE_NOTIFY_TYPE_SELECT_MOUNT = 0,					// 当前使用中的坐骑改变, param1 坐骑id
		MULTI_MOUNT_CHANGE_NOTIFY_TYPE_UPGRADE,								// 进阶数据改变, param1 坐骑id，param2 阶数，param3 祝福值
		MULTI_MOUNT_CHANGE_NOTIFY_TYPE_INVITE_RIDE,							// 收到别人坐骑邀请, param1 玩家ID，param2 坐骑ID
		MULTI_MOUNT_CHANGE_NOTIFY_TYPE_ACTIVE_SPECIAL_IMG,					// 激活双人坐骑特殊形象 param1 特殊形象激活标记 param2 形象id param3 形象等级
		MULTI_MOUNT_CHANGE_NOTIFY_TYPE_USE_SPECIAL_IMG,						// 使用特殊形象	param1 特殊形象id，param2 特殊形象等级
		MULTI_MOUNT_CHANGE_NOTIFY_TYPE_UPGRADE_EQUIP,						// 升级装备	param1 装备类型， param2 装备等级
		MULTI_MOUNT_CHANGE_NOTIFY_TYPE_UPLEVEL_SPECIAL_IMG,					// 升级特殊形象	param1 特殊形象id， param2 特殊形象等级
	};

	class SCMultiMountChangeNotify											// 坐骑信息增量更新 6124
	{
	public:
		SCMultiMountChangeNotify();
		MessageHeader header;

		short notify_type;
		short reserve_sh;
		int param_1;
		int param_2;
		int param_3;
	};

	class SCMultiMountNotifyArea											// 坐骑信息区域广播 6125
	{
	public:
		SCMultiMountNotifyArea();
		MessageHeader header;

		int owner_uid;														// 主人玩家id
		int follower_uid;													// 跟随者玩家id
		ObjID owner_obj_id;													// 主人id
		ObjID folower_obj_id;												// 跟随者id
		int owner_multimount_res_id;										// 主人坐骑资源id
		int folower_multimount_res_id;										// 跟随者坐骑资源id
	};
}

#pragma pack(pop)

#endif

