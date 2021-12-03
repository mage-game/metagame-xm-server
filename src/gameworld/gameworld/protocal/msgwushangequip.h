#ifndef __MSG_WUSHANG_EQUIP_HPP__
#define __MSG_WUSHANG_EQUIP_HPP__

#include "servercommon/wushangequipdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum WUSHANG_EQUIP_REQ_TYPE
	{
		WUSHANG_REQ_TYPE_ALL_INFO = 0,						// ������Ϣ����
		WUSHANG_REQ_TYPE_PUT_ON_EQUIP = 1,					// ��װ��
		WUSHANG_REQ_TYPE_TAKE_OFF_EQUIP = 2,				// ��װ��
		WUSHANG_REQ_TYPE_JIFEN_EXCHANGE = 3,				// ���ֶһ�
		WUSHANG_REQ_TYPE_STRENGTHEN = 4,					// ǿ��
		WUSHANG_REQ_TYPE_UP_STAR = 5,						// ����
		WUSHANG_REQ_TYPE_GLORY_EXCHANGE = 6,				// ��ҫ�һ�
	};

	struct WushangEquipStarInfo
	{
		WushangEquipStarInfo() : star_level(0), jinhua_val(0) {}

		short star_level;	// �Ǽ�
		short jinhua_val;	// ����ֵ
	};

	class CSWushangEquipOpearReq							// ��������������� 6132
	{
	public:
		CSWushangEquipOpearReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
		int param_1;
		int param_2;
		int param_3;
	};

	class SCWushangEquipAllInfo								// �������������Ϣ 6133
	{
	public:
		SCWushangEquipAllInfo();
		MessageHeader header;
		
		int wushang_jifen;													// ����
		unsigned short equip_id_list[WUSHANGEQUIP_MAX_TYPE_COUNT];
		char strong_level_list[WUSHANGEQUIP_MAX_TYPE_COUNT];				// ǿ���ȼ�
		WushangEquipStarInfo star_list[WUSHANGEQUIP_MAX_TYPE_COUNT];		// �Ǽ�
		int glory;															// ��ҫ
		int jy_glory;														// ��Ӣ��ҫ
		int cross_honor;													// �������
	};
}

#pragma pack(pop)

#endif

