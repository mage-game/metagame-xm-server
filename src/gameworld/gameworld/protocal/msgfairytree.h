#ifndef __MSG_FAIRY_TREE_HPP__
#define __MSG_FAIRY_TREE_HPP__

#include "servercommon/fairytreedef.hpp"
#include "servercommon/userprotocal/msgheader.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum FAIRY_TREE_REQ_TYPE
	{
		FAIRY_TREE_REQ_TYPE_ALL_INFO = 0,
		FAIRY_TREE_REQ_TYPE_FETCH_MONEY_REWARD,			// ��ȡ���߽�Ǯ����
		FAIRY_TREE_REQ_TYPE_FETCH_GIFT_REWARD,			// ��ȡ�����������
		FAIRY_TREE_REQ_TYPE_UPLEVEL,					// ����
		FAIRY_TREE_REQ_TYPE_UPGRADE,					// ����
		FAIRY_TREE_REQ_TYPE_DRAW_ONCE,					// �齱1��
		FAIRY_TREE_REQ_TYPE_DRAW_TEN_TIMES,				// �齱10��
		FAIRY_TREE_REQ_TYPE_DRAW_WITH_GOLD,				// Ԫ���齱

		FAIRY_TREE_REQ_TYPE_MAX,
	};

	class SCFairyTreeAllInfo				// 6139
	{
	public:
		SCFairyTreeAllInfo();
		MessageHeader header;

		int cur_reward_bind_gold;
		int cur_reward_bind_coin;

		unsigned int online_time_s_for_gift;

		int level;

		int grade;
		int grade_val;

		int funny_farm_score;
	};

	class SCFairyTreeDrawRewardInfo			// 6140
	{
	public:
		SCFairyTreeDrawRewardInfo();
		MessageHeader header;

		short reward_count;
		short reward_seq_list[FAIRY_TREE_DRAW_MAX_REWARD_COUNT];
	};

	class CSFairyTreeOperaReq				// 5689
	{
	public:
		CSFairyTreeOperaReq();
		MessageHeader header;

		unsigned short req_type;
		unsigned short param_1;
	};
}

#pragma pack(pop)

#endif
