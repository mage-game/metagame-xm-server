#ifndef __MSG_FEIXIAN_H__
#define __MSG_FEIXIAN_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/feixianparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{

	class SCFeixianEquipInfo
	{
	public:
		SCFeixianEquipInfo();
		MessageHeader header;

		ItemDataWrapper equip_list[FeiXianParam::E_INDEX_MAX];
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// ����װ����������
	enum XIANYUAN_EQUIP_OPERATE_TYPE
	{
		FEIXIAN_EQUIP_OPERATE_TYPE_REQ_INFO = 0,		// ������Ϣ
		FEIXIAN_EQUIP_OPERATE_TYPE_TAKE_OFF,			// ����(param1 = 'װ������')
		FEIXIAN_EQUIP_OPERATE_TYPE_COMPOSE,				// �ϳ�(param1 = 'װ������',param2 = '����װ������',param3 = '0(��һ�������Ǳ�����װ������),1(��һ�������Ǵ������ϵ�װ������)')
		FEIXIAN_EQUIP_OPERATE_TYPE_LEVELUP,				// ����(param1 = 'װ������' param2 = '0(��һ�������Ǳ�����װ������),1(��һ�������Ǵ������ϵ�װ������)')
	};

	class CSFeixianEquipOpe
	{
	public:
		CSFeixianEquipOpe();
		MessageHeader header;						

		short operate;                                  // �������ͣ�0 = ������Ϣ��1 = ����...��
		short param1;
		short param2;
		short param3;
	};
}

#pragma pack(pop)

#endif

