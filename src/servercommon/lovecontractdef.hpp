#ifndef __LOVECONTRACT_DEF_HPP__
#define __LOVECONTRACT_DEF_HPP__

#include "servercommon.h"
#include "servercommon/serverdef.h"

#pragma pack(push, 4)

const static int MAX_NOTICE_COUNT = 30;						// 最大留言数
const static int LOVE_CONTRACT_DAY_COUNT = 7;				// 爱情契约总天数

struct LoveContractData
{
	struct LoverNotice
	{
		LoverNotice() { this->Reset(); }
		void Reset()
		{
			notice_timestamp = 0;
			memset(contract_notice, 0, sizeof(LoveContractNotice));
		}
	
		unsigned int notice_timestamp;
		LoveContractNotice contract_notice;
	};
	
	LoveContractData(){ this->Reset(); }
	void Reset()
	{
		love_contract_reward_falg = 0;
		love_contract_start_timestamp = 0;
		can_receive_day_num = -1;

		for (int i = 0; i < MAX_NOTICE_COUNT; i++)
		{
			love_contract_notice_list[i].Reset();
		}
	}
	
	int love_contract_reward_falg;
	unsigned int love_contract_start_timestamp;
	int can_receive_day_num;
	LoverNotice love_contract_notice_list[MAX_NOTICE_COUNT];
};

typedef char LoveContractDataHex[sizeof(LoveContractData) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(LoveContractDataHex) < 5000);

#pragma pack(pop)

#endif // __LOVECONTRACT_DEF_HPP__
