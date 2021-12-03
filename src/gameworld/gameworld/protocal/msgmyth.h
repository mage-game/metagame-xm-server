#ifndef __MSG_MYTH_H__
#define __MSG_MYTH_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/mythdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum MYTH_OPERA_TYPE
	{
		MYTH_OPERA_TYPE_INFO,					// 请求全部信息
		MYTH_OPERA_TYPE_UPLEVEL,				// 篇章升级 p1: chpater_id
		MYTH_OPERA_TYPE_INLAY,					// 镶嵌神魂 p1: chpater_id; p2:背包index; p3:格子index
		MYTH_OPERA_TYPE_TAKE_OFF,				// 取下神魂 p1: chpater_id; p2:格子index
		MYTH_OPERA_TYPE_RESONANCE,				// 共鸣 p1:chpater_id
		MYTH_OPERA_TYPE_DIGESTION,				// 领悟 p1:chpater_id
		MYTH_OPERA_TYPE_DECOMPOSE,				// 萃取 p1:背包index
		MYTH_OPERA_TYPE_COMPOSE,				// 合成 p1:item_id; p2:背包index1; p3: 背包index2; p4:背包index3
	};

	class CSMythOpera			// 操作		
	{
	public:
		CSMythOpera();
		MessageHeader header;
		
		int opera_type;
		int param1;
		int param2;
		int param3;
		int param4;
	};

	class SCMythKnapaskInfo		// 背包信息		
	{
	public:
		SCMythKnapaskInfo();
		MessageHeader header;

		struct GridItem
		{
			int index;
			MythSoulGodItem item;
		};

		short is_all;
		short count;
		GridItem knapsack_list[MAX_MYTH_KNAPSACK_GIRD_COUNT];
	};

	class SCMythChpaterInfo		// 篇章全部信息
	{
	public:
		SCMythChpaterInfo();
		MessageHeader header;

		int soul_essence;
		MythChpaterItem chpater_list[MAX_MYTH_CHAPTER_ID];
	};

	class SCMythChpaterSingleInfo		// 单个篇章信息 
	{
	public:
		SCMythChpaterSingleInfo();
		MessageHeader header;

		int soul_essence;
		int chpater_id;
		MythChpaterItem single_item;
	};
}

#pragma pack(pop)

#endif	//__MSG_MYTH_H__