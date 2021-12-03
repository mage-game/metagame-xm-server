#ifndef __MSG_TUHAOJIN_HPP__
#define __MSG_TUHAOJIN_HPP__

#include "servercommon/tuhaojindef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCTuHaoJinInfo			// 2405									
	{
	public:
		SCTuHaoJinInfo();
		MessageHeader header;
		
		short tuhaojin_level;
		short reserve;
		char cur_tuhaojin_color; 
		char max_tuhaojin_color;
	};

	class CSUseTuHaoJinReq			// 2399
	{
	public:
		CSUseTuHaoJinReq();
		MessageHeader header;

		char use_tuhaojin_color;	
		char reserve_1;
		short reserve_2;
	};

	class CSTuhaojinUpLevelReq		// 8276
	{
	public:
		CSTuhaojinUpLevelReq();
		MessageHeader header;
	};
}

#pragma pack(pop)

#endif 


