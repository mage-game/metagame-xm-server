#ifndef __MSG_MAGIC_CARD_H__
#define __MSG_MAGIC_CARD_H__

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/magiccarddef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum MAGIC_CARD_REQ_TYPE
	{
		MAGIC_CARD_REQ_TYPE_ALL_INFO = 0,										// ����������Ϣ
		MAGIC_CARD_REQ_TYPE_CHOU_CARD,											// �齱��parm1 �鿨���� param2 �齱����
		MAGIC_CARD_REQ_TYPE_USE_CARD,											// ʹ��ħ����param1 ħ��id
		MAGIC_CARD_REQ_TYPE_UPGRADE_CARD,										// ����ħ����param1 ��ɫ�� param2 �����±꣬ param3 ����ħ��id�� param3 ����ħ������
		MAGIC_CARD_REQ_TYPE_EXCHANGE,											// ħ���һ���param1 ħ��id
		MAGIC_CARD_REQ_TYPE_SKILL_ACTIVE										// ����ħ������ param1 �����׿���ɫID
	};

	class CSMagicCardOperaReq													// ħ����������  5688
	{
	public:
		CSMagicCardOperaReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
		int param_1;
		int param_2;
		int param_3;
		int param_4;
	};

	class SCMagicCardAllInfo													// ħ��������Ϣ  6130
	{
	public:
		SCMagicCardAllInfo();
		MessageHeader header;

		int today_purple_free_chou_card_times;									// �Ͽ��齱��������ѳ鿨����
		int all_card_has_exchange_flag;											// ���п����Ƿ񱻶һ����ı��
		unsigned short all_card_num_list[MAGIC_CARD_MAX_LIMIT_COUNT + 1];		// ����ħ������������ħ�꣩
		MagicCardSlotItem card_slot_list[MAGIC_CARD_COLOR_TYPE_COLOR_COUNT][MAGIC_CARD_SLOT_TYPE_LIMIT_COUNT]; // ������ɫ�����б�
	};

	static const int SC_MAGIC_CARD_CHOU_CARD_MAX_TIMES = 50;
	class SCMagicCardChouCardResult												// ħ��������Ϣ  6131
	{
	public:
		SCMagicCardChouCardResult();
		MessageHeader header;

		short reward_list_count;												// �齱��ȡ�Ľ�������
		char all_reward_index_list[SC_MAGIC_CARD_CHOU_CARD_MAX_TIMES];			// �齱��ȡ�Ľ��������б�
	};
}

#pragma pack(pop)

#endif

