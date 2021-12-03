//-------------------------------------------------------------------------------------------

// 一个协议，重复做了三遍，就想办法做成通用的

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

// 跨服召集信息
struct CrossCallInfo
{
	enum CALL_TYPE
	{
		CALL_TYPE_INVALID = 0,
		CALL_TYPE_GUILD = 1,						// 家族召集
		CALL_TYPE_CAMP = 2,							// 国家召集
		CALL_TYPE_XYJD_DEFFENDER = 10,				// 咸阳据点 - 召唤防御者
		CALL_TYPE_XYJD_ATTACKER = 11,				// 咸阳据点 - 召唤进攻者
		CALL_TYPE_XYJD_PROGRESS_HALF = 12,			// 咸阳据点 - 据点进度至一半，param是据点ID
		CALL_TYPE_XYCITY_MIDAO_DEFENDER = 20,		// 咸阳城 - 密道开启通知防御方
		CALL_TYPE_XYCITY_MIDAO_ATTACKER = 21,		// 咸阳城 - 密道开启通知进攻方
		CALL_TYPE_XYCITY_MIDAO_BOSS_HP = 22,		// 咸阳城 - 密道BOSS HP改变召集防守方

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
	// 通用常量
	static constexpr int MAX_ITEM_COUNT = 50;	// 协议里面最大的物品数量

	//-------------------------------------------------------------------------------------------

	// 通用结构

	// 单个物品信息
	struct ItemInfo
	{
		void Copy(const ItemConfigData &item);

		int		item_id = 0;		// 物品ID
		short	item_num = 0;		// 物品数量
		short	is_bind = 0;		// 是否绑定
	};

	// 物品列表
	struct ItemInfoList
	{
		void Copy1(const std::vector<ItemConfigData> &item_list);
		void Copy2(const ItemConfigData item_list[], int count);

		int item_count = 0;
		ItemInfo item_info_list[MAX_ITEM_COUNT];
	};

	//-------------------------------------------------------------------------------------------

	// 通用协议

	// 抽奖结果 3100
	class SCDrawResult
	{
	public:
		SCDrawResult();
		MessageHeader header;

		enum DRAW_REASON
		{
			DRAW_REASON_DEFAULT = 0,
			DRAW_REASON_BEAUTY = 1,			// 美人抽奖
			DRAW_REASON_GREATE_SOLDIER = 2,	// 名将抽奖
			DRAW_REASON_HAPPY_DRAW = 3,		// 随机活动 - 欢乐抽
			DRAW_REASON_HAPPY_DRAW2 = 4,	// 随机活动 - 欢乐抽2
		};

		int draw_reason;					// 抽奖原因
		int draw_count;						// 抽奖次数
		ItemInfoList reward_item_list;		// 奖励物品
	};
}

#pragma pack(pop)

#endif