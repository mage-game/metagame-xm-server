#ifndef MSG_MSG_CARDZU_H
#define MSG_MSG_CARDZU_H

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/cardzudef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	static const int CARDZU_BATCH_CHOUCARD_TIMES = 10;						// 元宝连续抽卡次数

	enum CARDZU_REQ_TYPE
	{
		CARDZU_REQ_TYPE_CHOU_CARD = 0,										// 抽卡请求
		CARDZU_REQ_TYPE_HUALING,											// 化灵请求
		CARDZU_REQ_TYPE_LINGZHU,											// 灵铸请求
		CARDZU_REQ_TYPE_ACTIVE_ZUHE,										// 激活卡牌组合
		CARDZU_REQ_TYPE_UPGRADE_ZUHE,										// 升级卡牌组合
	};

	class CSCardzuOperaReq													// 卡牌组合操作请求  5650
	{
	public:
		CSCardzuOperaReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
		int param_1;
		int param_2;
		int param_3;
	};

	class SCCardzuAllInfo													// 卡组所有信息 5600
	{
	public:
		SCCardzuAllInfo();
		MessageHeader header;

		int lingli;															// 灵力
		char today_coin_buy_card_times[CARDZU_TYPE_MAX_COUNT];				// 卡组今日铜币单抽次数
		char today_gold_bind_buy_card_times[CARDZU_TYPE_MAX_COUNT];			// 卡组今日绑钻单抽次数
		short all_card_count_list[CARDZU_MAX_CARD_ID + 1];					// 所有卡牌的数量列表
		short all_zuhe_level_list[CARDZU_MAX_ZUHE_ID + 1];					// 所有组合的等级列表
	};

	class SCCardzuChangeNotify												// 卡组信息增量更新 5601
	{
	public:
		SCCardzuChangeNotify();
		MessageHeader header;

		struct ChangeCardItem
		{
			short card_id;
			short count;
		};

		int lingli;
		char today_coin_buy_card_times[CARDZU_TYPE_MAX_COUNT];				// 卡组今日铜币单抽次数
		char today_gold_bind_buy_card_times[CARDZU_TYPE_MAX_COUNT];			// 卡组今日绑钻单抽次数
		
		short change_zuhe_id;
		short change_zuhe_level;

		int change_card_count;
		ChangeCardItem change_card_list[CARDZU_BATCH_CHOUCARD_TIMES];
	};

	class SCCardzuChouCardResult											// 抽卡结果统一通知  5602
	{
	public:
		SCCardzuChouCardResult();
		MessageHeader header;

		short all_card_count_list[CARDZU_BATCH_CHOUCARD_TIMES];				// 抽到的卡牌ID列表
	};
}

#pragma pack(pop)

#endif

