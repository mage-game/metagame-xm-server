#ifndef __MSG_BABY_H__
#define __MSG_BABY_H__

#include "servercommon/babydef.hpp"
#include "servercommon/userprotocal/msgheader.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum BABY_REQ_TYPE
	{
		BABY_REQ_TYPE_INFO = 0,									// 请求单个宝宝信息  参数1 宝宝列表索引index
		BABY_REQ_TYPE_ALL_INFO = 1,								// 请求所有宝宝信息
		BABY_REQ_TYPE_UPLEVEL = 2,								// 升级请求	参数1 宝宝列表索引index
		BABY_REQ_TYPE_QIFU = 3,									// 祈福请求 参数1 祈福类型
		BABY_REQ_TYPE_QIFU_RET = 4,								// 祈福答应请求 参数1 祈福类型，参数2 是否接受
		BABY_REQ_TYPE_CHAOSHENG = 5,							// 超生请求
		BABY_REQ_TYPE_SPIRIT_INFO = 6,							// 请求单个宝宝的守护精灵的信息
		BABY_REQ_TYPE_TRAIN_SPIRIT = 7,							// 培育精灵请求
		BABY_REQ_TYPE_REMOVE_BABY = 8,							// 遗弃宝宝请求 p1:baby_index
		BABY_REQ_TYPE_REMOVE_BABY_RET = 9,						// 回应是否遗弃宝宝 p1:req_baby_index p2:1(同意)，0（不同意）p4:unique_baby_id
		BABY_REQ_TYPE_WALK = 10,								// 溜宝宝 param1 玩家是否idle 0不是 1是
	};

	struct BabySendInfo
	{
		short baby_index;

		short baby_id;
		short rename_times;
		short grade;
		short level;
		short bless;

		GameName baby_name;

		GameName lover_name;

		BabySpiritInfo baby_spirit_list[BABY_SPIRIT_COUNT];
	};

	class CSBabyOperaReq										// 操作请求  2417
	{
	public:
		CSBabyOperaReq();
		MessageHeader header;

		short opera_type;
		short param_0;
		int param_1;
		int param_2;
		int param_3;

		long long param_4;
	};

	class CSBabyUpgradeReq										// 宝宝升阶请求  2418
	{
	public:
		CSBabyUpgradeReq();
		MessageHeader header;

		short baby_index;
		short repeat_times;
		short auto_buy;
		short is_auto_upgrade;
	};

	class CSBabyRenameReq										// 宝宝改名请求 2419
	{
	public:
		CSBabyRenameReq();
		MessageHeader header;

		short baby_index;
		short reserve_sh;

		GameName newname;
	};

	enum BABY_INFO_TYPE
	{
		BABY_INFO_TYPE_REQUESET_CREATE_BABY = 0,					// 祈福树 生宝宝请求 p1:type
		BABY_INFO_TYPE_REMOVE_BABY_REQ,								// 遗弃宝宝请求 p1:req_baby_index, p2:unique_baby_id
	};

	class SCBabyBackInfo										// 宝宝 发送反馈信息  2414
	{
	public:
		SCBabyBackInfo();
		MessageHeader header;

		short info_type;
		short param_1;
		long long param_2;
	};

	class SCBabyInfo											//　发送 宝宝信息  2413
	{
	public:
		SCBabyInfo();
		MessageHeader header;

		BabySendInfo baby_send;
	};

	class SCBabyAllInfo											//　发送 所有宝宝信息  2415
	{
	public:
		SCBabyAllInfo();
		MessageHeader header;

		BabySendInfo baby_list[BABY_MAX_COUNT];

		int baby_chaosheng_count;
	};

	class SCBabySpiritInfo										// 发送 守护精灵信息 2416
	{
	public:
		SCBabySpiritInfo();
		MessageHeader header;

		int baby_index;

		BabySpiritInfo baby_spirit_list[BABY_SPIRIT_COUNT];
	};

	class SCBabyWalk				// 8061 溜宝宝
	{
	public:
		SCBabyWalk();
		MessageHeader header;

		ObjID obj_id;
		short is_special_baby;  // 是否龙凤宝宝 1是 0否
		int baby_index;
		GameName baby_name;
	};
}

#pragma pack(pop)

#endif // __MSG_BABY_H__


