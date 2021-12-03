// 升星助力协议

#ifndef MSG_SHENGXINGZHULI_H
#define MSG_SHENGXINGZHULI_H

#include "servercommon/userprotocal/msgheader.h"
// #include "config/gameconfigcommon/gameconfigcommon.h"

#pragma pack(push) 
#pragma pack(4)


namespace Protocol
{
	class CSGetShengxingzhuliInfoReq	// 8700 客户端升星助力的请求
	{
	public:
		CSGetShengxingzhuliInfoReq();
		MessageHeader header;
	};

	class SCGetShengxingzhuliInfoAck	// 8701 服务端升星助力的回复
	{
	public:
		SCGetShengxingzhuliInfoAck();
		MessageHeader header;

		int today_reward_fetch_stage;		// 是否已经领取今日份奖励,为1表示已领取
		int chognzhi_today;				// 今天氪的金
		int func_level;					// 对应系统的等级
		int func_type;					// 对应的系统
		int is_max_level;				// 对应系统是否最高级，0表示不是最高级，1表示是最高级
		int today_reward_stage;						// 当前档位
	};

	class CSGetShengxingzhuliRewardReq	// 8710 客户端请求领取升星助力的奖励
	{
	public:
		CSGetShengxingzhuliRewardReq();
		MessageHeader header;
	};

	class SCGetShengxingzhuliRewardAck	// 8711 服务端回复领取升星助力的奖励
	{
	public:
		SCGetShengxingzhuliRewardAck();
		MessageHeader header;

		int is_succ;					// 是否成功领取，为1表示已领取
	};
}

#pragma pack(pop)
#endif