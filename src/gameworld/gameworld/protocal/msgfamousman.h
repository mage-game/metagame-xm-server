#ifndef MSG_FAMOUS_MAN_H
#define MSG_FAMOUS_MAN_H

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum FAMOUS_MAN_OPERA_TYPE
	{
		FAMOUS_MAN_INFO_REQ = 0,	//	 ����������������Ϣ	
	};

	class CSFamousManOpera			// 1876�����ò�������
	{
	public:
		CSFamousManOpera();
		MessageHeader header;

		int opera_type;
	};

	class SCSendFamousManInfo							// 1877����������������Ϣ
	{
	public:
		SCSendFamousManInfo();
		MessageHeader header;

		int famous_man_type_1_uid;							// ������-�ȼ����ȴﵽX��
		int famous_man_type_2_uid;							// ������-�������ȴﵽX��
		int famous_man_type_3_uid;							// ������-�������ȴﵽX��
		int famous_man_type_4_uid;							// ������-���ȼ���һ��X�׺�ɫװ��
		int famous_man_type_5_uid;							// ������-����֮������ͨ��X��
		int famous_man_type_6_uid;							// ������-���ȼ���Ů��XXX
	};
}

#pragma pack(pop)

#endif

