#ifndef MSG_CHENGJIU_H
#define MSG_CHENGJIU_H

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/shengwangdef.hpp"
#include "servercommon/chengjiudef.hpp"

#pragma pack(push, 4) 

namespace Protocol
{
	enum CHENGJIU_OPER_TYPE
	{
		CHENGJIU_REQ_INFO = 0,			// 请求成就信息
		CHENGJIU_OPER_TITLE_UPLEVEL,	// 提升称号
		CHENGJIU_OPER_FETCH_REWARD,		// 领取奖励
		CHENGJIU_OPER_FUWEN_UPLEVEL,	// 提升符文
	};

	class CSChengJiuOpera
	{
	public:
		CSChengJiuOpera();
		MessageHeader header;

		short opera_type;
		short param1;
	};

	class SCChengJiuInfo
	{
	public:
		SCChengJiuInfo();
		MessageHeader header;

		short title_level;
		short reverse_sh1;

		short fuwen_level;
		short process_times;
		int need_gold_on_uplevel_next_fuwen;

		int shuxingdan_list[ChengJiuParam::CHENGJIU_SHUXINGDAN_MAX_TYPE];
		
		short reverse_sh2;

		struct InfoType 
		{
			short	reward_id;
			char	flag;
			char	reserve;

			unsigned int		process;
		};

		short count;
		InfoType reward_list[CHENGJIU_REWARD_MAX_NUM];
	};

	class SCChengJiuTitleViewChange
	{
	public:
		SCChengJiuTitleViewChange();
		MessageHeader header;

		ObjID obj_id;
		short title_level;
	};

	class SCChengJiuRewardChange
	{
	public:
		SCChengJiuRewardChange();
		MessageHeader header;

		struct InfoType 
		{
			short	reward_id;
			char	flag;
			char	reserve;

			unsigned int		process;
		};
		
		short reserver;
		short count;
		InfoType reward_list[CHENGJIU_REWARD_MAX_CHANGE_NUM_INBATCH];
	};

	// 成就信息  5691
	class SCChengJiuRewardInfo
	{
	public:
		SCChengJiuRewardInfo();
		MessageHeader header;

		short reward_id;
		short reserve_sh;
	};
}

#pragma pack(pop)

#endif

