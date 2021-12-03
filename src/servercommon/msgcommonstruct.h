//-------------------------------------------------------------------------------------------

// һ��Э�飬�ظ��������飬����취����ͨ�õ�

//-------------------------------------------------------------------------------------------

#ifndef __MSG_COMMON_STRUCT_H__
#define __MSG_COMMON_STRUCT_H__

#include <vector>
#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"
#include "servercommon/userprotocal/msgheader.h"

struct ItemConfigData;

#pragma pack(push) 
#pragma pack(4)

// ����ټ���Ϣ
struct CrossCallInfo
{
	enum CALL_TYPE
	{
		CALL_TYPE_INVALID = 0,
		CALL_TYPE_GUILD = 1,						// �����ټ�
		CALL_TYPE_CAMP = 2,							// �����ټ�
		CALL_TYPE_XYJD_DEFFENDER = 10,				// �����ݵ� - �ٻ�������
		CALL_TYPE_XYJD_ATTACKER = 11,				// �����ݵ� - �ٻ�������
		CALL_TYPE_XYJD_PROGRESS_HALF = 12,			// �����ݵ� - �ݵ������һ�룬param�Ǿݵ�ID
		CALL_TYPE_XYCITY_MIDAO_DEFENDER = 20,		// ������ - �ܵ�����֪ͨ������
		CALL_TYPE_XYCITY_MIDAO_ATTACKER = 21,		// ������ - �ܵ�����֪ͨ������
		CALL_TYPE_XYCITY_MIDAO_BOSS_HP = 22,		// ������ - �ܵ�BOSS HP�ı��ټ����ط�

		CALL_TYPE_MAX,
	};

	int call_type = 0;
	int origin_server_role_id = 0;
	GameName name = {0};
	short post = 0;
	short camp = 0;
	GuildID guild_id = 0;
	int activity_type = 0;
	int scene_id = 0;
	int scene_key = 0;
	int x = 0;
	int y = 0;
	int param = 0;
};

namespace Protocol
{
	// ͨ�ó���
	static constexpr int MAX_ITEM_COUNT = 50;	// Э������������Ʒ����

	//-------------------------------------------------------------------------------------------

	// ͨ�ýṹ

	// ������Ʒ��Ϣ
	struct ItemInfo
	{
		void Copy(const ItemConfigData &item);

		int		item_id = 0;		// ��ƷID
		short	item_num = 0;		// ��Ʒ����
		short	is_bind = 0;		// �Ƿ��
	};

	// ��Ʒ�б�
	struct ItemInfoList
	{
		void Copy1(const std::vector<ItemConfigData> &item_list);
		void Copy2(const ItemConfigData item_list[], int count);

		int item_count = 0;
		ItemInfo item_info_list[MAX_ITEM_COUNT];
	};

	//-------------------------------------------------------------------------------------------

	// ͨ��Э��

	// �齱��� 3100
	class SCDrawResult
	{
	public:
		SCDrawResult();
		MessageHeader header;

		enum DRAW_REASON
		{
			DRAW_REASON_DEFAULT = 0,
			DRAW_REASON_BEAUTY = 1,			// ���˳齱
			DRAW_REASON_GREATE_SOLDIER = 2,	// �����齱
			DRAW_REASON_HAPPY_DRAW = 3,		// ���� - ���ֳ�
			DRAW_REASON_HAPPY_DRAW2 = 4,	// ���� - ���ֳ�2
		};

		int draw_reason;					// �齱ԭ��
		int draw_count;						// �齱����
		ItemInfoList reward_item_list;		// ������Ʒ
	};
}

#pragma pack(pop)

#endif