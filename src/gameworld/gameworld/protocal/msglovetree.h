#ifndef __LOVETREE_MSG_HPP__
#define __LOVETREE_MSG_HPP__

#include "servercommon/lovetreedef.hpp"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/frienddef.hpp"

#pragma  pack(push, 4)

namespace Protocol
{

	class CSLoveTreeWaterReq			// 请求浇水 6650
	{
	public:
		CSLoveTreeWaterReq();
		MessageHeader header;

		char is_auto_buy;
		char is_water_other;			// 自己：别人 0：1
		short reserve;
	};

	class CSLoveTreeInfoReq				// 请求相思树信息 6651
	{	
	public:
		CSLoveTreeInfoReq();
		MessageHeader header;

		char is_self;					// 自己：别人 1：0
		char reserve_1;
		short reserve_2;
	};


	class  SCLoveTreeInfo				// 发送相思树信息 6675
	{
	public:
		SCLoveTreeInfo();
		MessageHeader header;

		char is_self;							//自己：伴侣 1:0
		char reserve_1;	
		short reserve_2;					
		
		int love_tree_star_level;
		int love_tree_cur_exp;
		int free_water_self;
		int free_water_other;

		GameName tree_name;
		int other_love_tree_star_level;
	};
}

#pragma  pack(pop)
#endif
