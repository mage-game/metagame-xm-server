
#ifndef __MSGACTIVEDEGREE_HPP__
#define __MSGACTIVEDEGREE_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/activedegreedef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	//	C --> S

	class CSActiveFetchReward
	{
	public:
		CSActiveFetchReward();
		MessageHeader		header;

		enum FETCH_ACTIVE_REWARD_OPERATE_TYEP
		{
			FETCH_ACTIVE_DEGREE_REWARD = 0,
			FETCHE_TOTAL_ACTIVE_DEGREE_REWARD,
			FETCHE_TOTAL_ACTIVE_DEGREE_REWARD_ON_DAY,
		};

		int					operate_type;
		int					param;
	};

	//	S --> C

	class SCActiveDegreeInfo
	{
	public:
		SCActiveDegreeInfo();
		MessageHeader		header;
		int					total_degree;
		unsigned long long	activedegree_fetch_flag;
		char				reward_flag_list[ACTIVEDEGREE_REWARD_ITEM_MAX_NUM];
		char				degree_list[ACTIVEDEGREE_TYPE_MAX];
		char				reward_on_day_flag_list[ACTIVEDEGREE_REWARD_ITEM_MAX_NUM];
	};
}

#pragma pack(pop)

#endif

