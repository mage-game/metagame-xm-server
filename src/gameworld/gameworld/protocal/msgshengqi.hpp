#ifndef __MSG_SHENGQI_HPP__
#define __MSG_SHENGQI_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/shengqiparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCShengqiInfo									// 8704
	{
	public:
		SCShengqiInfo();
		MessageHeader header;

		struct SCShengqiItem
		{
			SCShengqiItem()
				: level(0), reserve_sh(0), spirit_flag(0)
			{
				memset(spirit_value, 0, sizeof(spirit_value));
				memset(per_spirit_value, 0, sizeof(per_spirit_value));
			}
			short level;										// �ȼ�
			short reserve_sh;
			int spirit_flag;									// �����ĸ����б�
			int spirit_value[SHENGQI_SPIRIT_MAX_NUM];			// ��������ֵ
			int per_spirit_value[SHENGQI_SPIRIT_MAX_NUM];		// ����������ֱ�
		};

		int activate_flag;									// �����ʥ���б�
		SCShengqiItem shengqi_item[SHENGQI_MAX_NUM];		// ʥ����Ϣ�б�
	};

	enum SHENGQI_OPERA_TYPE
	{
		SHENGQI_OPERA_TYPE_ALL_INFO = 0,				// ʥ����������Ϣ
		SHENGQI_OPERA_TYPE_ACTIVATE,					// param1 ʥ��id
		SHENGQI_OPERA_TYPE_OPEN_SPIRIT,					// param1 ʥ��id param2 ������������
		SHENGQI_OPERA_TYPE_SPIRIT,						// param1 ʥ��id param2 ������Ʒid
		SHENGQI_OPERA_TYPE_STRENGTH,					// param1 ʥ��id param2 ǿ����Ʒid
		SHENGQI_DECOMPOSE,								// ���Ϸֽ����,param1 ����id , param2 ����

		SHENGQI_OPERA_TYPE_MAX,
	};

	class CSShengqiReq									// 8705
	{
	public:
		CSShengqiReq();
		MessageHeader header;

		int req_type;
		int param1;
		int param2;
	};
}

#pragma pack(pop)

#endif // 


