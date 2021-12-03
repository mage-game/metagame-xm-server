#ifndef __MSG_FIGHTING_H__
#define __MSG_FIGHTING_H__

#pragma pack(push) 
#pragma pack(4)

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/fightingdef.hpp"

// ������ϵ��Э��

namespace Protocol
{
	// 5955  ����������
	class CSFightingReq
	{
	public:
		enum ReqType
		{
			REQ_TYPE_M_MINING_INFO = 0,							// �ڿ�-��ȡ�����Ŀ����Ϣ�������Ĭ���·�N�����������еģ����þ��У�
			REQ_TYPE_M_BEEN_ROB_INFO,							// �ڿ�-��ȡ�����ٵ���Ϣ
			REQ_TYPE_M_REFLUSH_MINING,							// �ڿ�-ˢ�¿����ɫ
			REQ_TYPE_M_START_MINING,							// �ڿ�-��ʼ�ڿ�
			REQ_TYPE_M_BUY_TIMES,								// �ڿ�-�����ڿ����
			REQ_TYPE_M_ROB,										// �ڿ�-��������param1 NULL  param2 ���UID����֤��������
			REQ_TYPE_M_ROB_ROBOT,								// �ڿ�-������������ˣ�param1 �������±꣨0-7��
			REQ_TYPE_M_REVENGE,									// �ڿ�-����param1 �������¼index
			REQ_TYPE_M_FETCH_REWARD,							// �ڿ�-��ȡ�ڿ���

			REQ_TYPE_S_SAILING_INFO = 9,						// ����-��ȡ�����ĺ�����Ϣ�������Ĭ���·�N�����������еģ����þ��У�
			REQ_TYPE_S_BEEN_ROB_INFO,							// ����-��ȡ�����ٵ���Ϣ
			REQ_TYPE_S_REFLUSH_SAILING,							// ����-ˢ�º��е���ɫ
			REQ_TYPE_S_START_SAILING,							// ����-��ʼ����
			REQ_TYPE_S_BUY_TIMES,								// ����-�����д���
			REQ_TYPE_S_ROB,										// ����-�������ᣬparam1 NULL  param2 ���UID����֤��������
			REQ_TYPE_S_ROB_ROBOT,								// ����-������������ˣ�param1 �������±꣨0-7��
			REQ_TYPE_S_REVENGE,									// ����-����param1 �������¼index
			REQ_TYPE_S_FETCH_REWARD,							// ����-��ȡ���н���

			REQ_TYPE_C_INFO = 18,								// ����-�����б���Ϣ
			REQ_TYPE_C_REFLUSH,									// ����-ˢ���б�
			REQ_TYPE_C_BUY_FIGHTING_TIMES,						// ����-VIP������ս����
			REQ_TYPE_C_FIGHTING,								// ����-��ս����
		};

		CSFightingReq();
		MessageHeader header;

		short req_type;
		short param1;
		int param2;
	};

	// �ڿ����Э�� ================================================================================================================================

	// 5956  �ڿ������Ϣ
	class SCFightingMiningBaseInfo
	{
	public:
		SCFightingMiningBaseInfo();
		MessageHeader header;

		char today_mining_times;											// �������ڿ����
		char today_buy_times;												// �����ѹ������
		char today_rob_mine_times;											// ���������ٿ����
		char mining_quality;												// ��ǰ������
		char mining_been_rob_times;											// �����ٴ���������ȡ����ʱ������ͻ��˼��㽱����
		char reserve_ch;
		short reserve_sh;
		unsigned int mining_end_time;										// ��ǰ���ջ��ʱ���
	};

	// 5957  ��������ʷ�б�
	class SCFightingMiningBeenRobList
	{
	public:
		SCFightingMiningBeenRobList();
		MessageHeader header;

		struct HistoryItem
		{
			GameName rober_name;												// ����������
			unsigned int rob_time;												// ����ʱ��
			char has_revenge;													// �Ƿ��ѱ���
			char real_index;													// �����ݿ�洢������±꣨���ڸ���
			char cur_quality;													// ��Ʒ��
			char rober_sex;
			char rober_prof;
			char reserve_ch;
			short reserve_sh;
			int rober_capability;
		};

		HistoryItem rob_list[FIGHTING_MAX_ROB_HISTORY_COUNT];					// ��������ʷ�б�
	};

	// 5958  ���б�
	class SCFightingMiningList
	{
	public:
		SCFightingMiningList();
		MessageHeader header;

		const static int MAX_NOTIFY_COUNT = 50;					// ����·�����

		struct ItemInfo
		{
			int owner_uid;										// ����0˵���˱�����Ч�������ʾ�����ˣ���random_index
			int random_index;									// ���ڵ���0˵���˱�����Ч������ȷ�����������֡�ְҵ���Ա�ȵ�����

			GameName owner_name;
			unsigned int mining_end_time;						// ��ǰ���ջ�ʱ��
			char cur_quality;									// ��ǰ��Ʒ��
			char robot_index;									// �����������±꣨0-7�����ͻ���������սʱʹ��
			char owner_sex;										// �����Ա�
			char owner_prof;									// ����ְҵ
			int owner_capability;								// ����ս��
			short owner_level;									// �����ȼ�
			short reserve_sh;
		};

		int mine_count;											// ʵ����Ч����
		ItemInfo mine_list[MAX_NOTIFY_COUNT];
	};

	// �������Э�� ================================================================================================================================

	// 5959 ���� - ������Ϣ
	class SCFightingChallengeBaseInfo
	{
	public:
		SCFightingChallengeBaseInfo();
		MessageHeader header;

		int challenge_score;												// ���� - �ܻ���
		char challenge_limit_times;											// ���� - ��ǰʣ����ս����
		char vip_buy_times;													// �ѹ���Ĵ�������ͬVIP��������һ����
		short reserve_sh;
		unsigned int next_add_challenge_timestamp;							// ��һ��������ս������ʱ���
		unsigned int next_auto_reflush_time;								// ��һ���Զ�ˢ���б�ʱ��
	};

	// 5960 ս�����֪ͨ
	class SCFightingResultNotify
	{
	public:
		SCFightingResultNotify();
		MessageHeader header;

		struct ItemInfo
		{
			ItemID item_id;													// ������ƷID
			short num;														// ������Ʒ����
		};

		char is_win;														// �Ƿ�ʤ��
		char fighting_type;													// ս�����ͣ���� ChallengeType

		short is_skip;														// �Ƿ�Ϊɨ��,0����1��
		long long reward_exp;												// ��������ֵ
		ItemInfo reward_item_list[MAX_ATTACHMENT_ITEM_NUM];					// ������Ʒ�б�
	};
	
	// 5961 ���� - ս���б�
	class SCFightingChallengeList
	{
	public:
		SCFightingChallengeList();
		MessageHeader header;

		struct OpponentInfo
		{
			char camp;
			char prof;
			char sex;
			char has_win;
			short reserve_sh;
			short random_name_index; // ���ɵĶ���������������ͻ��˸������ȥ��������б��Ӧ�����֣������ֵΪ-1��˵���ñ�������������������name��
			GameName name;
			int capability;

			RoleAppearance appearance;
		};

		OpponentInfo opponent_list[FIGHTING_CHALLENGE_OPPONENT_COUNT];		// ������Ϣ
	};

	// �������Э�� ================================================================================================================================

	// 5962  ����������Ϣ
	class SCFightingSailingBaseInfo
	{
	public:
		SCFightingSailingBaseInfo();
		MessageHeader header;

		char today_sailing_times;											// �����Ѵ���
		char today_buy_times;												// �����ѹ������
		char today_rob_times;												// ���������ٴ���
		char sailing_quality;												// ��ǰƷ��
		char sailing_been_rob_times;										// �����ٴ���������ȡ����ʱ������ͻ��˼��㽱����
		char reserve_ch;
		short reserve_sh;
		unsigned int sailing_end_time;										// ��ǰ���ջ��ʱ���
	};

	// 5963  ��������ʷ�б�
	class SCFightingSailingBeenRobList
	{
	public:
		SCFightingSailingBeenRobList();
		MessageHeader header;

		struct HistoryItem
		{
			GameName rober_name;												// ����������
			unsigned int rob_time;												// ����ʱ��
			char has_revenge;													// �Ƿ��ѱ���
			char real_index;													// �����ݿ�洢������±꣨���ڸ���
			char cur_quality;													// ��Ʒ��
			char rober_sex;
			char rober_prof;
			char reserve_ch;
			short reserve_sh;
			int rober_capability;
		};

		HistoryItem rob_list[FIGHTING_MAX_ROB_HISTORY_COUNT];			// ��������ʷ�б�
	};

	// 5964  �����б�
	class SCFightingSailingList
	{
	public:
		SCFightingSailingList();
		MessageHeader header;

		const static int MAX_NOTIFY_COUNT = 50;					// ����·�����

		struct ItemInfo
		{
			int owner_uid;										// ����0˵���˱�����Ч�������ʾ�����ˣ���random_index
			int random_index;									// ���ڵ���0˵���˱�����Ч������ȷ�����������֡�ְҵ���Ա�ȵ�����

			GameName owner_name;
			unsigned int sailing_end_time;						// ��ǰ���ջ�ʱ��
			char cur_quality;									// ��ǰ��Ʒ��
			char robot_index;									// �����������±꣨0-7�����ͻ���������սʱʹ��
			char owner_sex;										// �����Ա�
			char owner_prof;									// ����ְҵ
			int owner_capability;								// ����ս��
			short owner_level;									// �����ȼ�
			short reserve_sh;
		};

		int sailing_count;										// ʵ����Ч����
		ItemInfo sailing_list[MAX_NOTIFY_COUNT];
	};


	// 5965 ս����ʼ����ʱ֪ͨ
	class SCFightingCountDownNotify
	{
	public:
		SCFightingCountDownNotify();
		MessageHeader header;

		unsigned int start_fighting_time;						// ��ʼս��ʱ���
	};

	// 5966  �ڿ󣬺���-���µ������¼�����͸����Ӷ���ұ��ˣ�û�鿴֮ǰһֱ����ʾ�����µ�¼Ҳһ����
	class SCFightingBeenRobNotify
	{
	public:
		SCFightingBeenRobNotify();
		MessageHeader header;

		char type;												// 0 �ڿ�1 ����
		char reserve_ch;
		short reserve_sh;
	};

	// 5967  �ڿ󣬺���-���µ������¼���㲥�������ˣ�ֻ������ʱ��һ�Σ�
	class SCFightingRobingNotify
	{
	public:
		SCFightingRobingNotify();
		MessageHeader header;

		GameName rober_name;
		GameName been_rob_name;
		char type;												// 0 �ڿ�1 ����
		char quality;											// Ʒ��
		short reserve_sh;
	};
}

#pragma pack(pop)

#endif


