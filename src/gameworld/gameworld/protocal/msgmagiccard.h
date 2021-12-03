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
		MAGIC_CARD_REQ_TYPE_ALL_INFO = 0,										// 请求所有信息
		MAGIC_CARD_REQ_TYPE_CHOU_CARD,											// 抽奖，parm1 抽卡类型 param2 抽奖次数
		MAGIC_CARD_REQ_TYPE_USE_CARD,											// 使用魔卡，param1 魔卡id
		MAGIC_CARD_REQ_TYPE_UPGRADE_CARD,										// 升级魔卡，param1 颜色， param2 卡槽下标， param3 消耗魔卡id， param3 消耗魔卡数量
		MAGIC_CARD_REQ_TYPE_EXCHANGE,											// 魔卡兑换，param1 魔卡id
		MAGIC_CARD_REQ_TYPE_SKILL_ACTIVE										// 激活魔卡技能 param1 技能套卡颜色ID
	};

	class CSMagicCardOperaReq													// 魔卡操作请求  5688
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

	class SCMagicCardAllInfo													// 魔卡所有信息  6130
	{
	public:
		SCMagicCardAllInfo();
		MessageHeader header;

		int today_purple_free_chou_card_times;									// 紫卡抽奖今日已免费抽卡次数
		int all_card_has_exchange_flag;											// 所有卡牌是否被兑换过的标记
		unsigned short all_card_num_list[MAGIC_CARD_MAX_LIMIT_COUNT + 1];		// 所有魔卡数量（包括魔魂）
		MagicCardSlotItem card_slot_list[MAGIC_CARD_COLOR_TYPE_COLOR_COUNT][MAGIC_CARD_SLOT_TYPE_LIMIT_COUNT]; // 所有颜色卡槽列表
	};

	static const int SC_MAGIC_CARD_CHOU_CARD_MAX_TIMES = 50;
	class SCMagicCardChouCardResult												// 魔卡所有信息  6131
	{
	public:
		SCMagicCardChouCardResult();
		MessageHeader header;

		short reward_list_count;												// 抽奖获取的奖励数量
		char all_reward_index_list[SC_MAGIC_CARD_CHOU_CARD_MAX_TIMES];			// 抽奖获取的奖励索引列表
	};
}

#pragma pack(pop)

#endif

