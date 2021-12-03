#ifndef __MSG_SEAL_HPP__
#define __MSG_SEAL_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/sealparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum SEAL_OPERA_TYPE
	{
		SEAL_OPERA_TYPE_ALL_INFO = 0,				// ����������Ϣ
		SEAL_OPERA_TYPE_PUT_ON,						// װ���� param1 ��������index
		SEAL_OPERA_TYPE_UPLEVLE,					// ������ param1 ʥӡ������index
		SEAL_OPERA_TYPE_RECYCLE,					// �ֽⵥ���� param1 �ֽ�������param2��������index
		SEAL_OPERA_TYPE_USE_SOUL,					// ʹ��ʥ�� param1 ���ͣ�param2 ����

		SEAL_OPERA_TYPE_MAX,
	};

	class CSSealReq									// 8706
	{
	public:
		CSSealReq();
		MessageHeader header;

		int req_type;
		int param1;
		int param2;
	};

	class SCSealBackpackInfo						// 8707
	{
	public:
		struct GridItem
		{
			short index;
			short slot_index;
			short order;
			short level;
			unsigned short item_id;
			unsigned short reserve_sh;
		};

		SCSealBackpackInfo();
		MessageHeader header;

		char reserve_1;
		char reserve_2;

		short grid_num;															// ��������
		GridItem grid_list[SEAL_BACKPACK_MAX_GRIDS];							// ������Ϣ�б�
	};

	class SCSealSlotInfo							// 8708
	{
	public:
		SCSealSlotInfo();
		MessageHeader header;

		SealItem grid_list[SEAL_SLOT_MAX_NUM];		// ʥӡ���б�
	};

	class SCSealBaseInfo							// 8709
	{
	public:
		SCSealBaseInfo();
		MessageHeader header;

		int hun_score;								// �����
		int soul_list[SEAL_SOUL_MAX_TYPE];			// ʥ���б�
	};

	class CSSealRecycleReq							// �ֽ�  8710
	{
	public:
		CSSealRecycleReq();
		MessageHeader header;

		int recycle_num;														// �ֽ����
		int recycle_backpack_index_list[SEAL_BACKPACK_MAX_GRIDS];				// �ֽ��±��б�
	};
}

#pragma pack(pop)

#endif // 

