#ifndef __MSG_ZHUANSHENGEQUIP_H__
#define __MSG_ZHUANSHENGEQUIP_H__

#include "servercommon/zhuanshengequipdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum ZHUANSHENG_REQ_TYPE
	{
		ZHUANSHENG_REQ_TYPE_ALL_INFO = 0,
		ZHUANSHENG_REQ_TYPE_BACKPACK_INFO = 1,
		ZHUANSHENG_REQ_TYPE_OTHER_INFO = 2,
		ZHUANSHENG_REQ_TYPE_UPLEVEL = 3,					// ��������
		ZHUANSHENG_REQ_TYPE_CHANGE_XIUWEI = 4,				// �һ���Ϊ����
		ZHUANSHENG_REQ_TYPE_PUT_ON_EQUIP = 5,				// ��װ��
		ZHUANSHENG_REQ_TYPE_TAKE_OFF_EQUIP = 6,				// ��װ��
		ZHUANSHENG_REQ_TYPE_COMBINE_EQUIP = 7,				// ��װ��
		ZHUANSHENG_REQ_TYPE_CHOU_EQUIP = 8,					// ��װ��
		ZHUANSHENG_REQ_TYPE_TAKE_OUT_EQUIP = 9,				// ȡ��װ��
		ZHUANSHENG_REQ_TYPE_AUTO_COMBINE_EQUIP = 10,		// �Զ���װ��
	};

	class CSZhuanShengOpearReq		// 6950 ת����������
	{
	public:
		CSZhuanShengOpearReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
		int param1;
		int param2;
		int param3;
	};

	class SCZhuanShengAllInfo		// 6975 ת��������Ϣ
	{
	public:
		SCZhuanShengAllInfo();
		MessageHeader header;

		ItemDataWrapper zhuansheng_equip_list[ZHUANSHENG_EQUIP_TYPE_MAX];

		unsigned int last_time_free_chou_timestamp;									// �ϴ���ѳ�ʱ���
		int personal_xiuwei;														// ������Ϊ
		short zhuansheng_level;														// ��ǰת���ȼ�
		short day_change_times;														// ����һ�����
	};

	class SCZhuanShengBackpackInfo // 6976 ת��������Ϣ
	{
	public:
		SCZhuanShengBackpackInfo();
		MessageHeader header;

		ZhuanshenEuipGridItem backpack_equip_list[ZHUANSHENG_EQUIP_BACKPACK_MAX_COUNT];	// ������ӵ�е�����װ���б�
	};

	enum ZHUANSHENG_NOTICE_REASON
	{
		ZHUANSHENG_NOTICE_NORMAL,
		ZHUANSHENG_NOTICE_XIUWEI_NOT_ENOUGH,
	};
	
	class SCZhuanShengOtherInfo  // 6977 ת��������Ϣ
	{	
	public:
		SCZhuanShengOtherInfo();
		MessageHeader header;
		
		int notice_reason;
		unsigned int last_time_free_chou_timestamp;									// �ϴ���ѳ�ʱ���
		int personal_xiuwei;														// ������Ϊ
		short zhuansheng_level;														// ��ǰת���ȼ�
		short day_change_times;														// ����һ�����
	};

	class SCZhuanShengXiuweiNotice
	{
	public:
		SCZhuanShengXiuweiNotice();
		MessageHeader header;
		
		int notice_reason;
		int add_xiuwei;																// ��Ϊ
	};

	static const int SC_ZHUANSHENG_CHOU_RESULT_MAX = 10;

	class SCZhuanShengChouResult // 6978 ת���齱���
	{
	public:
		SCZhuanShengChouResult();
		MessageHeader header;

		int result_count;
		char result_list[SC_ZHUANSHENG_CHOU_RESULT_MAX];
	};

	class SCZhuanShengCombineResult  // 6979 ת���ϳɽ��
	{
	public:
		SCZhuanShengCombineResult();
		MessageHeader header;

		unsigned short new_equip_id;
		short is_auto_combine;
	};

}

#pragma pack(pop)

#endif //__MSG_ZHUANSHENGEQUIP_H__


