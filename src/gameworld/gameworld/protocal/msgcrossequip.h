#ifndef _MSG_CROSS_EQUIP_H_
#define _MSG_CROSS_EQUIP_H_

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/crossequipdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum CROSS_EQUIP_REQ_TYPE
	{
		CROSS_EQUIP_REQ_TYPE_INFO = 0,					// 请求所有信息
		CROSS_EQUIP_REQ_TYPE_DOUQI_GRADE_UP = 1,		// 斗气进阶请求
		CROSS_EQUIP_REQ_TYPE_DOUQI_XIULIAN = 2,			// 斗气修炼请求

		CROSS_EQUIP_REQ_TYPE_ROLL = 3,					// 抽奖请求 param1--roll_type  param_2--roll_times_type

		CROSS_EQUIP_REQ_TAKEOFF = 4,					// 脱下装备 param1--equipment_index(参考普通装备)
		CROSS_EQUIP_REQ_LIANZHI = 5,					// 炼制装备 param1--equipment_index param2--douqi_grade
		CROSS_EQUIP_REQ_ONE_EQUIP_INFO = 6,				// 单个装备信息 param1--equipment_index
		CROSS_EQUIP_REQ_ALL_EQUIP_INFO = 7,				// 所有装备信息 

		CROSS_EQUIP_REQ_TYPE_MAX,
	};

	//enum CROSS_EQUIP_ROLL_TYPE
	//{
	//	CROSS_EQUIP_ROLL_TYPE_LOW_LEVEL = 1,			// 初级
	//	CROSS_EQUIP_ROLL_TYPE_MID_LEVEL = 2,			// 中级
	//	CROSS_EQUIP_ROLL_TYPE_HIGHT_LEVEL = 3,			// 高级
	//};

	//enum CROSS_EQUIP_ROLL_TIMES_TYPE
	//{
	//	CROSS_EQUIP_ROLL_TIMES_TYEP_BEGIN,

	//	CROSS_EQUIP_ROLL_TYPE_ONE_TIMES = 1,			// 一抽
	//	CROSS_EQUIP_ROLL_TYPE_TEN_TIMES = 2,			// 十抽
	//	CROSS_EQUIP_ROLL_TYPE_THIRTY_TIMES = 3,			// 三十抽

	//	CROSS_EQUIP_ROLL_TIMES_TYEP_MAX,
	//};


	// 操作请求，8961
	class CSCrossEquipOpera
	{
	public:
		CSCrossEquipOpera();
		MessageHeader header;

		unsigned short req_type;
		unsigned short param_1;
		unsigned short param_2;
		unsigned short param_3;
	};

	// 信息下发 8962
	class SCCrossEquipAllInfo
	{
	public:
		SCCrossEquipAllInfo();
		MessageHeader header;

		unsigned short douqi_grade;			// 当前斗气阶级
		unsigned short xiulian_times;		// 当天修炼次数
		long long douqi_exp;				// 当前斗气经验

		unsigned int chuanshi_fragment;		// 传世碎片

		unsigned short douqidan_used_count[CROSS_EQUIP_DOUQIDAN_TYPE_MAX];	// 斗气丹使用次数
		unsigned short reserve_sh;
	};

	// 抽奖返回 8963
	const static int CROSS_EQUIP_ROLL_TIMES_MAX = 30;
	class SCCrossEquipRollResult
	{
	public:
		SCCrossEquipRollResult();
		MessageHeader header;

		unsigned int __unused;

		struct RewardResult
		{
			ItemID item_id;
			short reward_num;
		};

		int reward_count;										// 抽奖奖励
		RewardResult reward_list[CROSS_EQUIP_ROLL_TIMES_MAX];	// 抽奖奖励
	};

	class SCCrossEquipOneEquip				// 8964 单个装备信息 -- 穿脱
	{
	public:
		SCCrossEquipOneEquip();
		MessageHeader	header;

		int equip_index;
		ItemDataWrapper	item_wrapper;
	};

	class SCCrossEquipAllEquip				// 8965 所有装备信息 
	{
	public:
		SCCrossEquipAllEquip();
		MessageHeader header;

		ItemDataWrapper equipment_list[CROSS_EQUIP_INDEX_MAX];
	};

	// 8966 传世碎片改变 右下角显示
	class SCCrossEquipChuanshiFragmentChange
	{
	public:
		SCCrossEquipChuanshiFragmentChange();
		MessageHeader header;

		enum CHUANSHI_FRAG_CHANGE_TYPE
		{
			CHUANSHI_FRAG_CHANGE_TYPE_ADD,
			CHUANSHI_FRAG_CHANGE_TYPE_DEC,
		};

		short change_type;					// 0 增加 1 减少
		short reserve_sh;					// 保留
		unsigned int change_fragment;		// 传世碎片改变数量
	};

	// 8967 斗气经验改变 右下角显示
	class SCCrossEquipDouqiExpChange
	{
	public:
		SCCrossEquipDouqiExpChange();
		MessageHeader header;

		int add_exp;						// 增加的斗气经验
	};

}

#pragma pack(pop)

#endif

