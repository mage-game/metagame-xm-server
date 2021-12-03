#ifndef __MSG_PET_HPP__
#define __MSG_PET_HPP__

#include "servercommon/petdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum PET_REQ_TYPE
	{
		PET_REQ_TYPE_INFO = 0,									// 宠物基础信息请求
		PET_REQ_TYPE_BACKPACK_INFO,								// 宠物背包信息请求
		PET_REQ_TYPE_SELECT_PET,								// 宠物出战请求
		PET_REQ_TYPE_CHANGE_NAME,								// 宠物改名请求	
		PET_REQ_TYPE_UP_LEVEL,									// 宠物升级请求
		PET_REQ_TYPE_UP_GRADE,									// 宠物升阶请求(作废，已经使用独立协议)
		PET_REQ_TYPE_CHOU,										// 宠物抽取请求
		PET_REQ_TYPE_RECYCLE_EGG,								// 宠物蛋回收请求
		PET_REQ_TYPE_PUT_REWARD_TO_KNAPSACK,					// 宠物领取奖励请求
		PET_REQ_TYPE_ACTIVE,									// 宠物激活请求
		PET_REQ_TYPE_LEARN_SKILL,								// 学习技能
		PET_REQ_TYPE_UPGRADE_SKILL,								// 升级技能
		PET_REQ_TYPE_FORGET_SKILL,								// 遗忘技能
		PET_REQ_TYPE_QINMI_PROMOTE,								// 提升亲密度
		PET_REQ_TYPE_QINMI_AUTO_PROMOTE,						// 一键升亲密等级
		PET_REQ_TYPE_FOOD_MARKET_CHOU_ONCE,						// 吃货市场单次抽奖
		PET_REQ_TYPE_FOOD_MARKET_CHOU_TIMES,					// 吃货市场多次抽奖
		PET_REQ_TYPE_UPLEVL_SPECIAL_IMG,						// 提升幻化等级
	};

	class CSPetOperaReq											// 宠物操作请求  6110
	{
	public:
		CSPetOperaReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
		int param_1;
		int param_2;
		GameName newname;
	};

	class CSPetUpgradeReq										// 宠物升阶请求  6111
	{
	public:
		CSPetUpgradeReq();
		MessageHeader header;

		short repeat_times;
		short auto_buy;
	};

	class SCPetBackpackInfo			 							// 宠物背包信息 6106
	{
	public:
		SCPetBackpackInfo();
		MessageHeader header;

		unsigned int last_free_chou_timestamp;					// 上一次免费抽宠物的时间戳
		PetStoreItem backpack_item_list[PET_MAX_STORE_COUNT];
		int food_market_free_times;								// 吃货市场免费次数
	};
	
	class CSPetUseSpecialImg									//使用宠物特殊形象 6112
	{
	public:
		CSPetUseSpecialImg();
		MessageHeader header;

		short special_img_id;
		short reserve_sh;
	};

	class SCPetInfo												// 宠物基础信息 6107
	{
	public:
		SCPetInfo();
		MessageHeader header;

		int cur_on_use_pet_id;
		PetParamItem own_pet_list[PET_MAX_COUNT_LIMIT];
		int special_img_active_flag;
		char special_img_level_list[MAX_SPECIAL_IMG_COUNT];
	};

	class SCFightOutPetInfo												// 宠物出战信息 6109
	{
	public:
		SCFightOutPetInfo();
		MessageHeader header;

		int cur_on_use_pet_id;											// 当前出战的宠物id
	};


	class SCPetViewChangeNotify									// 宠物属性改变广播 6105
	{
	public:
		SCPetViewChangeNotify();
		MessageHeader header;

		ObjID obj_id;											// 玩家对象ID
		short pet_id;											// 出战宠物ID
		short pet_level;										// 出战宠物等级
		short pet_grade;										// 出战宠物阶数
		GameName pet_name;										// 出战宠物名
		short use_img_id;                                       // 使用幻化ID
		short reserve_sh;
	};


	static const int SC_CHOU_PET_MAX_TIMES = 10;
	class SCPetChouResult														// 宠物抽奖结果信息 6108
	{
	public:
		SCPetChouResult();
		MessageHeader header;

		short reward_list_count;												// 抽奖获取的奖励数量
		char all_reward_index_list[SC_CHOU_PET_MAX_TIMES];						// 抽奖获取的奖励索引列表
	};

	class SCPetQinmiAllInfo										// 6117 所有宠物亲密度信息
	{
	public:
		SCPetQinmiAllInfo();
		MessageHeader header;

		PetQinmiParamItem pet_qinmi_list[PET_MAX_COUNT_LIMIT];		// PET_MAX_COUNT_LIMIT = 12
		int ignore_fangyu_percent_level;
	};

	class SCPetQinmiSingleInfo									// 6118 单个宠物亲密度信息
	{
	public:
		SCPetQinmiSingleInfo();
		MessageHeader header;

		PetQinmiParamItem pet_qinmi_item;
		int cur_pet_id;
		int ignore_fangyu_percent_level;
	};

	static const int SC_CHOU_PET_FOOD_MARKET_MAX_TIMES = 50;
	class SCPetFoodMarcketChouResult							// 6119 吃货市场抽奖信息
	{
	public:
		SCPetFoodMarcketChouResult();
		MessageHeader header;

		short reward_list_count;
		char reward_seq_list[SC_CHOU_PET_FOOD_MARKET_MAX_TIMES];
	};
}

#pragma pack(pop)

#endif // __MSG_PET_HPP__


