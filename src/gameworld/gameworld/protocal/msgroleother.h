#ifndef MSGROLEOTHER_H
#define MSGROLEOTHER_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/serverdef.h"
#include "servercommon/mazedef.hpp"

#pragma pack(push) 
#pragma pack(4)


namespace Protocol
{

	/*
		��ɫ��������Э��
	*/

	class CSRoleChooseCamp
	{
	public:
		CSRoleChooseCamp();
		MessageHeader	header;

		char camp_type;
		char is_random;
		short reserved_other;
	};

	class CSRoleChooseProf
	{
	public:
		CSRoleChooseProf();
		MessageHeader	header;

		char prof_type;
		char reserved;
		short reserved_other;
	};

	class CSSaveRoleHangupSet
	{
	public:
		CSSaveRoleHangupSet();
		MessageHeader	header;

		HangupSetData	hangup_set;
	};

	class CSSaveRoleCustomSet
	{
	public:
		CSSaveRoleCustomSet();
		MessageHeader	header;
		
		CustomSetData	custom_set;
	};

	class SCHangupSetInfo
	{
	public:
		SCHangupSetInfo();
		MessageHeader	header;
		
		HangupSetData	hangup_set;
		CustomSetData	custom_set;
	};

	class CSDailyWelfareTipsReq
	{
	public:
		CSDailyWelfareTipsReq();
		MessageHeader	header;
	};

	class SCDailyWelfareTips
	{
	public:
		SCDailyWelfareTips();
		MessageHeader	header;

		static const int DAILY_WELFARE_TIPS_MAX = 32;
		char welfare_tips_curr[DAILY_WELFARE_TIPS_MAX];
		char welfare_tips_max[DAILY_WELFARE_TIPS_MAX];
	};

	class CSLightenWeddingRing			// 2827 �������
	{
	public:
		CSLightenWeddingRing();
		MessageHeader header;
		short use_gold;
		short reserve;
	};

	class CSUplevelWeddingRing			// 2828 �������
	{
	public:
		CSUplevelWeddingRing();
		MessageHeader header;
	};

	class CSPerformWeddingRingSkill		// 2829 �ͷŻ�似��
	{
	public:
		CSPerformWeddingRingSkill();
		MessageHeader header;

		short ring_level;				// ���ȼ�
		short target_obj_id;			// Ŀ�꼴���µĶ���ID
	};

	class SCWeddingRingSkillInfo		// 6007 ��似����Ϣ
	{
	public:
		SCWeddingRingSkillInfo();
		MessageHeader header;

		unsigned int	last_wr_skill_4_perform_time;			// �ϴλ��4������ʹ��ʱ��
		unsigned int	last_wr_skill_5_perform_time;			// �ϴλ��5������ʹ��ʱ��
		unsigned int	last_wr_skill_6_perform_time;			// �ϴλ��6������ʹ��ʱ��
	};

	////////////////////////////////////// �Թ� /////////////////////////////////
	class CSQueryMaze
	{
	public:
		CSQueryMaze();
		MessageHeader header;
	};

	class CSMoveMaze
	{
	public:
		CSMoveMaze();
		MessageHeader header;

		short target_x;
		short target_y;
	};

	class CSBuyMaze
	{
	public:
		CSBuyMaze();
		MessageHeader header;
	};

	class CSFetchMazeJifenReward
	{
	public:
		CSFetchMazeJifenReward();
		MessageHeader header;

		int seq;
	};

	class SCMazeMoveResult
	{
	public:
		SCMazeMoveResult();
		MessageHeader header;

		enum RET
		{
			RET_DEFAULT = 0,		// Ĭ��
			RET_BOMB = 1,			// ը��
			RET_ITEM = 2,			// ����֮����
			RET_COIN = 3,			// ����֮ͭǮ
		};

		short result;
		short reserve_sh;
		int param;					// RET_BOMB��0�� RET_ITEM������������ RET_COIN��ͭǮ����
	};

	class SCMazeInfo
	{
	public:
		SCMazeInfo();
		MessageHeader header;

		enum REASON
		{
			REASON_DEFAULT = 0,
			REASON_DAYCHANGE = 1,			// ��������
			REASON_BUY = 2,					// �������
			REASON_MOVE = 3,				// �ƶ�
		};

		short reason;
		short jifen_reward_flag;								// ���ֽ������

		char shown_box_x;										// ��ʾ�ı���λ��
		char shown_box_y;
		short today_item_count;									// �����ȡ��������

		int today_coin;											// �����ȡͭǮ����

		char buy_times;											// �ѹ������
		char curr_x;											// ��ɫ��ǰ����
		char curr_y;
		char jifen;												// ����

		char move_times;										// �ƶ�����
		char reserve_ch;										// ����1
		short reserve_sh;										// ����2

		// ������Ϣ 5*5, maze_grid[i][j] ��λ������1:���䣬2:ը����4:�ȹ�
		char maze_grid[MazeParam::ONE_SIDE_COUNT][MazeParam::ONE_SIDE_COUNT];	
		char reserve_list[3];
	};
}

#pragma pack(pop)

#endif

