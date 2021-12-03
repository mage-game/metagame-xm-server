#ifndef __MSG_ZODIAC_HPP__
#define __MSG_ZODIAC_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/zodiacparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCZodiacInfo									// 8972 ��Ф��Ϣ
	{
	public:
		SCZodiacInfo();
		MessageHeader header;

		struct SCZodiacItem
		{
			SCZodiacItem()
				: level(0), activate_flag(0), reserve_ch(0) {}

			short level;								// �ȼ�
			char activate_flag;							// ��Ƭ�������־
			char reserve_ch;
		};

		SCZodiacItem zodiac_item[ZODIAC_MAX_NUM];		// ʮ����Ф��Ϣ�б�
	};

	class SCZodiacBackpackInfo							// 8973 ������Ϣ
	{
	public:
		struct GridItem
		{
			unsigned short item_id;
			short zodiac_index;
			short suipian_index;
			short reserve_sh;
		};

		SCZodiacBackpackInfo();
		MessageHeader header;

		char reserve_ch1;
		char reserve_ch2;

		short grid_num;									// ��������
		GridItem grid_list[ZODIAC_BACKPACK_MAX_GRIDS];	// ������Ϣ�б�
	};

	enum ZODIAC_OPERA_TYPE
	{
		ZODIAC_OPERA_TYPE_ALL_INFO = 0,				// ʮ����Ф��������Ϣ
		ZODIAC_OPERA_TYPE_ACTIVATE,					// ���� param1 ��������index 
		ZODIAC_OPERA_TYPE_LEVELUP,					// ���� param1 ��Ф����
		ZODIAC_OPERA_TYPE_DECOMPOSE,				// �ֽ� param1 �ֽ�������param2��������index

		ZODIAC_OPERA_TYPE_MAX,
	};

	class CSZodiacReq									// 8974 ��������
	{
	public:
		CSZodiacReq();
		MessageHeader header;

		int req_type;
		int param1;
		int param2;
	};

	class CSZodiacDecomposeReq							// 8975 �ֽ�����
	{
	public:
		CSZodiacDecomposeReq();
		MessageHeader header;

		int decompose_num;														// �ֽ����
		int decompose_backpack_index_list[ZODIAC_BACKPACK_MAX_GRIDS];			// �ֽ��±��б�
	};

	class SCZodiacBaseInfo								// 8976 ������Ϣ
	{
	public:
		SCZodiacBaseInfo();
		MessageHeader header;

		int jinghua_num;							// ������
	};
}

#pragma pack(pop)

#endif // 

