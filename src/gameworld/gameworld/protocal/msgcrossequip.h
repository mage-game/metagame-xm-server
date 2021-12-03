#ifndef _MSG_CROSS_EQUIP_H_
#define _MSG_CROSS_EQUIP_H_

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/crossequipdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum CROSS_EQUIP_REQ_TYPE
	{
		CROSS_EQUIP_REQ_TYPE_INFO = 0,					// ����������Ϣ
		CROSS_EQUIP_REQ_TYPE_DOUQI_GRADE_UP = 1,		// ������������
		CROSS_EQUIP_REQ_TYPE_DOUQI_XIULIAN = 2,			// ������������

		CROSS_EQUIP_REQ_TYPE_ROLL = 3,					// �齱���� param1--roll_type  param_2--roll_times_type

		CROSS_EQUIP_REQ_TAKEOFF = 4,					// ����װ�� param1--equipment_index(�ο���ͨװ��)
		CROSS_EQUIP_REQ_LIANZHI = 5,					// ����װ�� param1--equipment_index param2--douqi_grade
		CROSS_EQUIP_REQ_ONE_EQUIP_INFO = 6,				// ����װ����Ϣ param1--equipment_index
		CROSS_EQUIP_REQ_ALL_EQUIP_INFO = 7,				// ����װ����Ϣ 

		CROSS_EQUIP_REQ_TYPE_MAX,
	};

	//enum CROSS_EQUIP_ROLL_TYPE
	//{
	//	CROSS_EQUIP_ROLL_TYPE_LOW_LEVEL = 1,			// ����
	//	CROSS_EQUIP_ROLL_TYPE_MID_LEVEL = 2,			// �м�
	//	CROSS_EQUIP_ROLL_TYPE_HIGHT_LEVEL = 3,			// �߼�
	//};

	//enum CROSS_EQUIP_ROLL_TIMES_TYPE
	//{
	//	CROSS_EQUIP_ROLL_TIMES_TYEP_BEGIN,

	//	CROSS_EQUIP_ROLL_TYPE_ONE_TIMES = 1,			// һ��
	//	CROSS_EQUIP_ROLL_TYPE_TEN_TIMES = 2,			// ʮ��
	//	CROSS_EQUIP_ROLL_TYPE_THIRTY_TIMES = 3,			// ��ʮ��

	//	CROSS_EQUIP_ROLL_TIMES_TYEP_MAX,
	//};


	// ��������8961
	class CSCrossEquipOpera
	{
	public:
		CSCrossEquipOpera();
		MessageHeader header;

		unsigned short req_type;
		unsigned short param_1;
		unsigned short param_2;
		unsigned short param_3;
	};

	// ��Ϣ�·� 8962
	class SCCrossEquipAllInfo
	{
	public:
		SCCrossEquipAllInfo();
		MessageHeader header;

		unsigned short douqi_grade;			// ��ǰ�����׼�
		unsigned short xiulian_times;		// ������������
		long long douqi_exp;				// ��ǰ��������

		unsigned int chuanshi_fragment;		// ������Ƭ

		unsigned short douqidan_used_count[CROSS_EQUIP_DOUQIDAN_TYPE_MAX];	// ������ʹ�ô���
		unsigned short reserve_sh;
	};

	// �齱���� 8963
	const static int CROSS_EQUIP_ROLL_TIMES_MAX = 30;
	class SCCrossEquipRollResult
	{
	public:
		SCCrossEquipRollResult();
		MessageHeader header;

		unsigned int __unused;

		struct RewardResult
		{
			ItemID item_id;
			short reward_num;
		};

		int reward_count;										// �齱����
		RewardResult reward_list[CROSS_EQUIP_ROLL_TIMES_MAX];	// �齱����
	};

	class SCCrossEquipOneEquip				// 8964 ����װ����Ϣ -- ����
	{
	public:
		SCCrossEquipOneEquip();
		MessageHeader	header;

		int equip_index;
		ItemDataWrapper	item_wrapper;
	};

	class SCCrossEquipAllEquip				// 8965 ����װ����Ϣ 
	{
	public:
		SCCrossEquipAllEquip();
		MessageHeader header;

		ItemDataWrapper equipment_list[CROSS_EQUIP_INDEX_MAX];
	};

	// 8966 ������Ƭ�ı� ���½���ʾ
	class SCCrossEquipChuanshiFragmentChange
	{
	public:
		SCCrossEquipChuanshiFragmentChange();
		MessageHeader header;

		enum CHUANSHI_FRAG_CHANGE_TYPE
		{
			CHUANSHI_FRAG_CHANGE_TYPE_ADD,
			CHUANSHI_FRAG_CHANGE_TYPE_DEC,
		};

		short change_type;					// 0 ���� 1 ����
		short reserve_sh;					// ����
		unsigned int change_fragment;		// ������Ƭ�ı�����
	};

	// 8967 ��������ı� ���½���ʾ
	class SCCrossEquipDouqiExpChange
	{
	public:
		SCCrossEquipDouqiExpChange();
		MessageHeader header;

		int add_exp;						// ���ӵĶ�������
	};

}

#pragma pack(pop)

#endif

