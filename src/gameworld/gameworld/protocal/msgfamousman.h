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
		FAMOUS_MAN_INFO_REQ = 0,	//	 请求名人堂名人信息	
	};

	class CSFamousManOpera			// 1876名人堂操作请求
	{
	public:
		CSFamousManOpera();
		MessageHeader header;

		int opera_type;
	};

	class SCSendFamousManInfo							// 1877发送名人堂名人信息
	{
	public:
		SCSendFamousManInfo();
		MessageHeader header;

		int famous_man_type_1_uid;							// 名人堂-等级最先达到X级
		int famous_man_type_2_uid;							// 名人堂-坐骑最先达到X阶
		int famous_man_type_3_uid;							// 名人堂-羽翼最先达到X阶
		int famous_man_type_4_uid;							// 名人堂-最先集齐一套X阶红色装备
		int famous_man_type_5_uid;							// 名人堂-勇者之塔最先通关X层
		int famous_man_type_6_uid;							// 名人堂-最先激活女神XXX
	};
}

#pragma pack(pop)

#endif

