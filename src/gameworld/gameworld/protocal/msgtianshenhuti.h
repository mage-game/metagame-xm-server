#ifndef _MSG_TIANSHENHUTI_H_
#define _MSG_TIANSHENHUTI_H_

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/tianshenhutidef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCTianshenhutiALlInfo												// 周末装备装备信息  8628
	{
	public:
		SCTianshenhutiALlInfo();
		MessageHeader header;

		TianshenEquipID equip_list[TIANSHENHUTI_EQUIP_MAX_COUNT];			// 每个部位对应装备ID

		short used_free_times;												// 已经使用的免费次数
		unsigned short backpack_num;										// 背包里装备数量（有效数组长度）
		int roll_score;														// 抽奖积分
		unsigned int next_free_roll_time;									// 下次可以免费抽奖时间 （先判断次数 看是否可以免费 再判断时间 是否显示倒计时）
		short accumulate_roll_times;										// 累计抽奖次数
		short reserved_sh;													// 预留位
		TianshenEquipID backpack_list[TIANSHENHUTI_BACKPACK_MAX_COUNT];		// 背包里拥有的所有装备列表

	};

	const static int TIANSHENHUTI_BATCH_ROLL_TIMES = 5;

	class SCTianshenhutiRollResult											// 周末装备抽奖结果  8629
	{
	public:
		SCTianshenhutiRollResult();
		MessageHeader header;

		short reward_count;
		TianshenEquipID reward_list[TIANSHENHUTI_BATCH_ROLL_TIMES];

		UNSTD_STATIC_CHECK(5 == TIANSHENHUTI_BATCH_ROLL_TIMES);
	};

	enum CS_TIANSHENHUTI_REQ_TYPE
	{
		CS_TIANSHENHUTI_REQ_TYPE_PUT_ON = 0,								// 穿上装备-背包下标
		CS_TIANSHENHUTI_REQ_TYPE_TAKE_OFF,									// 脱下装备-装备部位下标
		CS_TIANSHENHUTI_REQ_TYPE_TRANSFORM,									// 转化装备-2个背包下标
		CS_TIANSHENHUTI_REQ_TYPE_COMBINE,									// 合成装备-3个背包下标
		CS_TIANSHENHUTI_REQ_TYPE_ROLL,										// 天神附体抽奖
		CS_TIANSHENHUTI_REQ_TYPE_COMBINE_ONE_KEY,							// 一键合成装备 p1 选中等级

		CS_TIANSHENHUTI_REQ_TYPE_MAX,
	};

	class SCTianshenhutiReqResult											// 周末装备相关请求结果 8630
	{
	public:
		SCTianshenhutiReqResult();
		MessageHeader header;

		unsigned short req_type;											// CS_TIANSHENHUTI_REQ_TYPE
		unsigned short param_1;
		TianshenEquipID new_equip_id;
		unsigned short reserve_sh;
	};

	class CSTianshenhutiReq													// 周末装备装备请求  8631
	{
	public:
		CSTianshenhutiReq();
		MessageHeader header;

		unsigned short req_type;											// CS_TIANSHENHUTI_REQ_TYPE
		unsigned short param_1;
		unsigned short param_2;
		unsigned short param_3;

		unsigned short param_4;
		unsigned short reserve_sh;
	};

	class SCTianshenhutiScoreChange											//  8632 周末装备积分变动
	{
	public:
		SCTianshenhutiScoreChange();
		MessageHeader header;

		int roll_score;
	};

	class SCTianshenhutiCombineOneKeyResult									// 周末装备一键合成返回 8633
	{
	public:
		SCTianshenhutiCombineOneKeyResult();
		MessageHeader header;

		static const int MAX_COMBINE_NUM = 40;

		int combine_count;
		TianshenEquipID new_equip_id[MAX_COMBINE_NUM];
	};

}

#pragma pack(pop)

#endif

