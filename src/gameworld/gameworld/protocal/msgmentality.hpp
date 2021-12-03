#ifndef __MSG_MENTALITY_HPP__
#define __MSG_MENTALITY_HPP__

#include "servercommon/userprotocal/msgheader.h"

#include "servercommon/mentalitydef.hpp"

#pragma pack(push)
#pragma pack(4)

namespace Protocol
{
	class CSTrainMentality											// 训练元神
	{
	public:
		CSTrainMentality();
		MessageHeader header;
	};

	class CSMentalityClearTrainCD										// 清除元神CD
	{
	public:
		CSMentalityClearTrainCD();
		MessageHeader header;
	};

	class CSUpgradeMentalityGengu									// 升级根骨
	{
	public:
		CSUpgradeMentalityGengu();
		MessageHeader header;

		short type;													// 元神类型
		short reserved;		
		int use_protect_item;										// 使用保护符
		int is_auto_buy;											// 不足是否自动购买 1购买
	};

	class CSSpeedUpMentalityTrain									// 加速元神修炼
	{
	public:
		CSSpeedUpMentalityTrain();
		MessageHeader header;
	};

	class SCMentalityList											// 发送元神信息
	{
	public:
		SCMentalityList();
		MessageHeader header;

		unsigned int cd_next_time;									// 元神训练完成时间戳
		int curr_train_type;										// 当前训练类型
		short flag;													// 标记
		short reserve_sh;											// 保留

		MentalityItem mentality_list[MENTALITY_TYPE_MAX];			// 元神列表

		UNSTD_STATIC_CHECK(35 == MENTALITY_WUXING_MAX_COUNT);

		short reserve_sh_list[5];	
		short wuxing_level_list[MENTALITY_WUXING_MAX_COUNT];		// 五行等级

		int shuxingdan_list[MENTALITY_SHUXINGDAN_MAX_TYPE];			// 属性丹
	};

	class CSMentalityUplevelWuxing									// 提升五行
	{
	public:
		CSMentalityUplevelWuxing();
		MessageHeader header;

		short wuxing_index;
		short is_auto_buy;
	};

	class CSMentalityYijianTisheng									// 一键提升
	{
	public:
		CSMentalityYijianTisheng();
		MessageHeader header;
	};

	class SCMentalityYijianTishengAck								// 一键提升结果通知
	{
	public:
		SCMentalityYijianTishengAck();
		MessageHeader header;

		int tisheng_count;											// 元神提升个数
	};
}

#pragma pack(pop)

#endif // __MSG_MENTALITY_HPP__

