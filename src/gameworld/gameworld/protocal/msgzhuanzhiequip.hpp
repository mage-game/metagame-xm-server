#ifndef __MSG_ZHUANZHI_EQUIP_H__
#define __MSG_ZHUANZHI_EQUIP_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/zhuanzhiequipdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{

	// תְװ����������
	enum ZHUANZHI_EQUIP_OPERATE_TYPE
	{
		ZHUANZHI_EQUIP_OPERATE_TYPE_EQUIP_INFO = 0,		// ����װ����Ϣ
		ZHUANZHI_EQUIP_OPERATE_TYPE_STONE_INFO,			// ������ʯ��Ϣ
		ZHUANZHI_EQUIP_OPERATE_TYPE_SUIT_INFO,			// ������װ��Ϣ

		ZHUANZHI_EQUIP_OPERATE_TYPE_TAKE_OFF,			// ���� p1: part_index
		ZHUANZHI_EQUIP_OPERATE_TYPE_UP_STAR,			// ���� p1: part_index
		ZHUANZHI_EQUIP_OPERATE_TYPE_FULING,				// ���� p1: part_index

		ZHUANZHI_EQUIP_OPERATE_TYPE_INLAY_STONE,		// ��Ƕ��ʯ p1: part_index p2: slot_index p3: bag_index
		ZHUANZHI_EQUIP_OPERATE_TYPE_UNINLAY_STONE,		// ж����ʯ p1: part_index p2: slot_index 
		ZHUANZHI_EQUIP_OPERATE_TYPE_UP_LEVEL,			// ������ʯ	p1: part_index p2: slot_index 
		ZHUANZHI_EQUIP_OPERATE_TYPE_REFINE_STONE,		// ������ʯ p1: part_index p2: seq p3: is_autobuy
		ZHUANZHI_EQUIP_OPERATE_TYPE_FORGE_SUIT,			// ������װ	p1: suit_index p2: part_index 
		ZHUANZHI_EQUIP_OPERATE_TYPE_ZHIZUN_COMPOSE,		// ����װ���ϳ�	p1: compose_id p2: best_attr_num p3: equip_index (����װ������,-1Ϊ������ p4:bag_index1 p5:bag_index2

		ZHUANZHI_EQUIP_OPERATE_TYPE_STONE_RESOLVE,		// ��ʯ�ֽ�	p1: bag_index 
		ZHUANZHI_EQUIP_OPERATE_TYPE_STONE_CONVERT,		// ��ʯ�һ�	p1: seq 

		ZHUANZHI_EQUIP_OPERATE_TYPE_AWAKENING_INFO,		// ���������Ϣ
		ZHUANZHI_EQUIP_OPERATE_TYPE_AWAKENING_WAKE,		// ���� p1: part_index p2: �Զ�����
		ZHUANZHI_EQUIP_OPERATE_TYPE_AWAKENING_REPLACE,	// �滻 p1: part_index
		ZHUANZHI_EQUIP_OPERATE_TYPE_AWAKENING_LOCK,		// ���� p1: part_index, p2: lock_index
		ZHUANZHI_EQUIP_OPERATE_TYPE_AWAKENING_UNLOCK,	// ���� p1: part_index, p2: lock_index
	};

	class CSZhuanzhiEquipOpe		// 8770
	{
	public:
		CSZhuanzhiEquipOpe();
		MessageHeader header;						

		short operate;                                  // �������ͣ�0 = ������Ϣ��1 = ����...��
		short param1;
		short param2;
		short param3;
		short param4;
		short param5;
	};

	class SCZhuanzhiEquipInfo			// 8771 תְװ����Ϣ
	{
	public:
		SCZhuanzhiEquipInfo();
		MessageHeader header;

		ZhuanzhiEquipItem zhuanzhi_equip_list[E_INDEX_MAX];
	};

	class SCZhuanzhiStoneInfo			// 8772 תְ��ʯ��Ϣ
	{
	public:
		SCZhuanzhiStoneInfo();
		MessageHeader header;

		UInt32 stone_score;									// ��ʯ����
		ZhuanzhiStonesPartData stone_list[E_INDEX_MAX];
	};

	class SCZhuanzhiSuitInfo			// 8773 תְ��װ��Ϣ
	{
	public:
		SCZhuanzhiSuitInfo();
		MessageHeader header;

		char part_suit_type_list[E_INDEX_MAX];		// ��װ�����б�
		char part_order_list[E_INDEX_MAX];			// �����б�
	};

	class SCZhuanzhiEquipAwakeningAllInfo			// 8928 ����תְװ��������Ϣ
	{
	public:
		SCZhuanzhiEquipAwakeningAllInfo();
		MessageHeader header;

		unsigned char zhuanzhi_equip_awakening_lock_flag[MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_BYTE];
		ZhuanzhiEquipAwakening zhuanzhi_equip_awakening_list[E_INDEX_MAX];		// ������Ϣ
	};

	class SCZhuanzhiEquipAwakeningInfo				// 8929 һ��תְװ��������Ϣ
	{
	public:
		SCZhuanzhiEquipAwakeningInfo();
		MessageHeader header;

		int index;												// ����
		unsigned char zhuanzhi_equip_awakening_lock_flag[MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_BYTE];
		ZhuanzhiEquipAwakening zhuanzhi_equip_awakening;		// ������Ϣ
	};

	class CSZhuanzhiEquipCompose		// 8952
	{
	public:
		CSZhuanzhiEquipCompose();
		MessageHeader header;

		ItemID item_id;												//�ϳɵ�װ��id
		short xianpin_num;											//��Ʒ����
		short bag_index_count;										//������������
		short bag_index_list[MAX_ZHUANZHI_EQUIP_COMPOSE_COUNT];		//���������б�
	};

	class SCZhuanzhiEquipComposeSucceed				// 8957 תְװ���ϳɳɹ�
	{
	public:
		SCZhuanzhiEquipComposeSucceed();
		MessageHeader header;

		short is_succeed;												// �Ƿ�ɹ�
		short reverse_sh;
	};
}

#pragma pack(pop)

#endif

