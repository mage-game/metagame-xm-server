#ifndef __LITTLEPET_MSG_HPP__
#define __LITTLEPET_MSG_HPP__

#include "servercommon/littlepetdef.hpp"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/frienddef.hpp"

#pragma  pack(push, 4)

namespace Protocol
{
	enum LITTLE_PET_REQ_TYPE
	{
		LITTLE_PET_REQ_INTENSIFY_SELF,						// 强化自己宠物 param1 宠物索引，param2 强化点索引，param3格子索引 
		LITTLE_PET_REQ_INTENSIFY_LOVER,						// 强化爱人宠物 param1 宠物索引，param2 强化点索引，param3格子索引 
		LITTLE_PET_REQ_CHOUJIANG,							// 抽奖	param1  1:10
		LITTLE_PET_REQ_RECYCLE,								// 回收	param1 物品id ，param2 物品数量， param3 是否绑定 1：0 默认绑定 
		LITTLE_PET_REQ_RELIVE,								// 放生	param1 宠物索引												
		LITTLE_PET_REQ_FEED,								// 喂养自己宠物	param1 宠物索引 , param2 自己：伴侣 1：0 param3:是否自动购买
		LITTLE_PET_REQ_PET_FRIEND_INFO,						// 宠友信息			
		LITTLE_PET_REQ_INTERACT,							// 互动 param1 宠物索引 param2 目标role uid param3 自己：伴侣 1：0	
		LITTLE_PET_REQ_EXCHANGE,							// 兑换 param1 兑换物品索引 param2 数量；			
		LITTLE_PET_REQ_CHANGE_PET,                          // 换宠 param1 宠物索引 param2 使用的物品id							
		LITTLE_PET_REQ_USING_PET,							// 使用形象 param1 形象id；
		LITTLE_PET_REQ_FRIEND_PET_LIST,						// 好友小宠物列表；param1 朋友uid
		LITTLE_PET_REQ_INTERACT_LOG,						// 互动记录；														
		LITTLE_PET_PUTON,									// 装备小宠物 param1:宠物下标 param2:背包index					
		LITTLE_PET_TAKEOFF,									// 卸下小宠物 param1:宠物下标
		LITTLE_PET_REQ_EQUIPMENT_PUTON,						// 小宠物穿戴装备 param1:宠物下标 param2:背包index
		LITTLE_PET_REQ_EQUIPMENT_TAKEOFF,					// 小宠物脱下装备 param1:宠物下标 param2:装备index
		LITTLE_PET_REQ_EQUIPMENT_UPLEVEL_SELF,				// 自己小宠物装备升级 param1 宠物下标 param2 装备下标 param3 是否自动购买
		LITTLE_PET_REQ_EQUIPMENT_UPLEVEL_LOVER,				// 爱人小宠物装备升级 param1 宠物下标 param2 装备下标 param3 是否自动购买
		LITTEL_PET_REQ_WALK,								// 溜宠物 param1 玩家是否idle 0不是 1是
	};

	class CSLittlePetREQ			// 操作请求 8001
	{
	public:
		CSLittlePetREQ();
		MessageHeader header;

		int opera_type;
		int param1;
		int param2;
		int param3;
	};

	struct LittlePetGridValue
	{
		short arrt_type;														//强化格子的数值类型
		short grid_index;														//格子索引
		int attr_value;															//数值
		
	};

	struct LittlePetPointValue
	{
		LittlePetGridValue gridvaluelist[LITTLEPET_QIANGHUAGRID_MAX_NUM];		//格子列表
	};

	struct LittlePetEquipInfo
	{
		unsigned short equipment_id;
		short level;
	};

	struct LittlePetSingleInfo
	{
		int index;																//宠物索引
		short id;																//宠物id
		short info_type;														//自己的，伴侣的 1: 0
		GameName pet_name;														//小宠物名字

		int maxhp;																//属性
		int gongji;
		int fangyu;
		int mingzhong;
		int shanbi;
		int baoji;
		int kangbao;

		unsigned int baoshi_active_time;										//上次饱食度满的时间戳
		short feed_degree;														//饱食度
		short interact_times;													//互动次数
		short feed_level;														//喂养等级
		short reserve_sh;

		LittlePetPointValue point_list[LITTLEPET_QIANGHUAPOINT_CURRENT_NUM];			//强化点列表
		LittlePetEquipInfo equipment_llist[LITTLEPET_EQUIP_INDEX_MAX_NUM];				// 装备列表
	};

	class SCLittlePetAllInfo		//8050
	{
	public:
		SCLittlePetAllInfo();
		MessageHeader header;
		int score;																//积分
		unsigned int last_free_chou_timestamp;									//免费抽奖时间戳
		short interact_times;													//玩家互动次数	

		short pet_count;														//宠物个数
		LittlePetSingleInfo pet_list[LITTLE_PET_COUPLE_MAX_SHARE_NUM];
	};

	class SCLittlePetSingleInfo		//8051
	{
	public:
		SCLittlePetSingleInfo();
		MessageHeader header;

		LittlePetSingleInfo pet_single;
	};

	class  SCLittlePetChangeInfo	//8052
	{
	public:
		SCLittlePetChangeInfo();
		MessageHeader header;

		char pet_index;															//宠物索引
		char is_self;															//自己：伴侣 1:0
		char point_type;														//强化点
		char grid_index;														//格子索引

		LittlePetGridValue gridvalue;											//格子的数值
	};

	struct LittlePetReWarValue
	{
		int item_id;															//物品id
		short item_num;															//物品数量
		short is_bind;															//是否绑定
	};

	class SCLittlePetChouRewardList		//8053
	{
	public:
		SCLittlePetChouRewardList();
		MessageHeader header;

		int list_count;															//奖励物品列表数量
		int final_reward_seq;													//最后一个抽奖结果seq
		LittlePetReWarValue reward_list[LITTLE_PET_MAX_CHOU_COUNT];				//奖励物品列表
	};

	enum LITTLE_PET_NOTIFY_INFO_TYPE
	{
		LITTLE_PET_NOTIFY_INFO_SCORE,											//param1 积分信息							
		LITTLE_PET_NOTIFY_INFO_FREE_CHOU_TIMESTAMP,								//param1 免费抽奖时间戳
		LITTLE_PET_NOTIFY_INFO_INTERACT_TIMES,									//param1 玩家互动次数
		LITTLE_PET_NOTIFY_INFO_FEED_DEGREE,										//param1 宠物索引 ， param2 饱食度，param3 自己：伴侣 ？ 1：0
		LITTLE_PET_NOTIFY_INFO_PET_INTERACT_TIMES,								//param1 宠物互动次数
	};


	class SCLittlePetNotifyInfo			//8054
	{
	public:
		SCLittlePetNotifyInfo();
		MessageHeader header;

		int param_type;
		unsigned int param1;
		int param2;
		int param3;
		unsigned int param4;
	};

	struct LittlePetFriendInfo
	{
		int friend_uid;															//朋友id
		short prof;																//头像
		short sex;																//朋友的sex
		GameName owner_name;													//朋友的名字
		int pet_num;															//宠物的数量
	};

	class SCLittlePetFriendInfo			//8055
	{
	public:
		SCLittlePetFriendInfo();
		MessageHeader header;

		int count;
		LittlePetFriendInfo pet_friend_list[MAX_FRIEND_NUM];
	};

	class SCLittlePetUsingImg			//8056
	{
	public:
		SCLittlePetUsingImg();
		MessageHeader header;

		int role_id;
		int using_pet_id;
		GameName pet_name;
	};

	struct LittlePetFriendPet
	{
		int index;
		int pet_id;
		int info_type;
		GameName pet_name;
		short interact_times;													//互动次数
		short reserve;				
	};

	class SCLittlePetFriendPetListInfo	//8057
	{
	public:
		SCLittlePetFriendPetListInfo();
		MessageHeader header;

		int count;
		LittlePetFriendPet pet_list[LITTLE_PET_COUPLE_MAX_SHARE_NUM];
	};

	struct LittlePetInteractLogStruct
	{
		GameName name;
		int pet_id;
		unsigned int timestamp;
		GameName pet_name;
	};

	class SCLittlePetInteractLog		//8058
	{
	public:
		SCLittlePetInteractLog();
		MessageHeader header;

		int count;
		LittlePetInteractLogStruct log_list[LITTLE_PET_SHARE_MAX_LOG_NUM];
	};

	class CSLittlePetRename				//8002
	{
	public:
		CSLittlePetRename();
		MessageHeader header;

		int index;
		GameName pet_name;
	};

	class SCLittlePetRename				//8059
	{
	public:
		SCLittlePetRename();
		MessageHeader header;

		short index;
		short info_type;
		GameName pet_name;
	};

	class SCLittlePetWalk				//8060
	{
	public:
		SCLittlePetWalk();
		MessageHeader header;

		ObjID obj_id;
		short reserve_sh;
		int pet_id;
		GameName pet_name;
	};
}

#pragma  pack(pop)
#endif
