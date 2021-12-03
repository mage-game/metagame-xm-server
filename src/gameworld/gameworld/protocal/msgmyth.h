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
		MYTH_OPERA_TYPE_INFO,					// ����ȫ����Ϣ
		MYTH_OPERA_TYPE_UPLEVEL,				// ƪ������ p1: chpater_id
		MYTH_OPERA_TYPE_INLAY,					// ��Ƕ��� p1: chpater_id; p2:����index; p3:����index
		MYTH_OPERA_TYPE_TAKE_OFF,				// ȡ����� p1: chpater_id; p2:����index
		MYTH_OPERA_TYPE_RESONANCE,				// ���� p1:chpater_id
		MYTH_OPERA_TYPE_DIGESTION,				// ���� p1:chpater_id
		MYTH_OPERA_TYPE_DECOMPOSE,				// ��ȡ p1:����index
		MYTH_OPERA_TYPE_COMPOSE,				// �ϳ� p1:item_id; p2:����index1; p3: ����index2; p4:����index3
	};

	class CSMythOpera			// ����		
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

	class SCMythKnapaskInfo		// ������Ϣ		
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

	class SCMythChpaterInfo		// ƪ��ȫ����Ϣ
	{
	public:
		SCMythChpaterInfo();
		MessageHeader header;

		int soul_essence;
		MythChpaterItem chpater_list[MAX_MYTH_CHAPTER_ID];
	};

	class SCMythChpaterSingleInfo		// ����ƪ����Ϣ 
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