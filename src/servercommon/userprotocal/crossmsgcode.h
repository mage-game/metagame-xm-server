#ifndef __CROSS_MSG_CODE_H__
#define __CROSS_MSG_CODE_H__

namespace Protocol
{
	// 注意！！ 每种跨服玩法协议号单独一个段 global根据协议号转发给相应cross

	enum
	{
		CROSS_SERVER_MSG_BEGIN = 14000,

		CROSS_SERVER_COMMON_BEGIN = CROSS_SERVER_MSG_BEGIN,		// 通用
		CROSS_SERVER_COMMON_END = 14090,

		CROSS_ACTIVITY_1V1_BEGIN = 14100,						// 跨服1v1

		CROSS_ACTIVITY_3V3_BEGIN = 14130,						// 跨服3v3

		CROSS_LIEKUN_FB_BEGIN = 14160,							// 跨服猎鲲地带

		CROSS_XIULUO_TOWER_BEGIN = 14190,						// 跨服修罗塔

		CROSS_GUILD_BATTLE_BEGIN = 14220,						// 跨服帮派战

		CROSS_TEAM_FB_BEGIN = 14250,							// 跨服组队本

		CROSS_BIANJINGZHIDI_BEGIN = 14280,						// 跨服边境之地

		// 新增的加在这里...

		CROSS_OTHER_BEGIN = 14800,								// 其他

		CROSS_RAND_ACTIVITY_MSG_BEGIN=14900,

		CROSS_SERVER_MSG_END = 15000,
	};


	// 每个协议段的开始协议号对应上面的定义。
	// 请求协议分发的时候根据CrossManager::GetFuncTypeWithCrossProtocolMsgType获取活动号(func_type)发送给对应的crossserver
	enum
	{
		// =============================================================================================================
		MT_RETURN_ORIGINAL_SERVER_SC = CROSS_SERVER_COMMON_BEGIN,			// 通知玩家回到原服 14000
		MT_CROSS_GET_PERSON_RANK_LIST_ACK_SC = 14001,						// 下发跨服排行榜列表
		MT_CROSS_GET_COUPLE_RANK_LIST_ACK_SC = 14002,						// 下发跨服情侣榜列表
		MT_NOTICE_CAN_ENTER_CROSS_SC = 14003,								// 通知玩家进入跨服

		MT_CROSS_GET_PERSON_RANK_LIST_CS = 14010,							// 获取跨服排行榜列表

		// =============================================================================================================
		MT_CROSS_1V1_MATCH_ACK_SC = CROSS_ACTIVITY_1V1_BEGIN,				// 跨服1V1匹配状态 14100
		MT_CROSS_1V1_WEEK_RECORD_SC = 14101,								// 跨服1V1个人记录
		MT_CROSS_1V1_RANK_LIST_SC = 14102,									// 跨服1V1排行榜
		MT_CROSS_1V1_MATCH_RESULT_SC = 14103,								// 跨服1V1匹配结果
		MT_CROSS_1V1_FIGHT_RESULT_SC = 14104,								// 跨服1V1战斗结果

		MT_CROSS_1V1_MATCH_QUERY_CS = 14115,								// 跨服1V1匹配状态查询
		MT_CROSS_1V1_WEEK_RECORD_QUERY_CS = 14116,							// 跨服1V1个人记录查询
		MT_CROSS_1V1_GET_RANK_LIST_CS = 14117,								// 跨服1V1获取排行榜
		// =============================================================================================================

		MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_SC = CROSS_ACTIVITY_3V3_BEGIN, // 跨服3V3匹配状态通知 14130
		MT_CROSS_MULTIUSER_CHALLENGE_RANKLIST_SC = 14131,					// 跨服3V3排行榜列表
		MT_CROSS_MULTIUSER_CHALLENGE_HAS_MATCH_NOTICE_SC = 14132,			// 跨服3V3是否有比赛通知

		MT_GET_MULTIUSER_CHALLENGE_RANK_LIST_CS = 14145,					// 获取跨服3V3排行榜列表
		MT_MULTIUSER_CHALLENGE_CHECK_USER_HAS_MATCH_CS = 14146,				// 跨服3V3检查玩家是否有比赛
		// =============================================================================================================

		MT_CROSS_LIEKUN_FB_REQ_CS = CROSS_LIEKUN_FB_BEGIN,					// 跨服猎鲲地带，请求信息 14160
		MT_CROSS_LIEKUN_FB_PLAYER_INFO_SC = 14175,							// 跨服猎鲲地带，玩家信息
		// =============================================================================================================

		MT_CROSS_XIULUO_TOWER_GET_DROP_LOG_CS = CROSS_XIULUO_TOWER_BEGIN,	// 跨服修罗塔掉落日志信息请求 14190
		MT_CROSS_XIULUO_TOWER_DROP_LOG_SC = 14205,							// 跨服修罗塔掉落日志信息下发
		// =============================================================================================================

		MT_CROSS_GUILD_BATTLE_GET_DROP_LOG_CS = CROSS_GUILD_BATTLE_BEGIN,	// 跨服帮派战掉落日志信息请求 14220
		MT_CROSS_GUILD_BATTLE_GET_MONSTER_INFO_CS = 14221,					// 跨服帮派战掉落怪物信息请求

		MT_CROSS_GUILD_BATTLE_DROP_LOG_SC = 14235,							// 跨服帮派战掉落日志信息下发
		// =============================================================================================================

		MT_NOTICE_CROSS_TEAM_FB_ROOM_INFO_CHANGE_SC = CROSS_TEAM_FB_BEGIN,	// 跨服组队本，房间信息改变 14250
		MT_NOTICE_CROSS_TEAM_FB_ROOM_LIST_INFO_SC = 14251,					// 跨服组队本，房间列表信息
		MT_NOTICE_CROSS_TEAM_FB_ROOM_INFO_SC = 14252,						// 跨服组队本，房间信息

		// =============================================================================================================

		MT_CROSS_SERVER_BIANJINGZHIDI_BOSS_INFO_SC = CROSS_BIANJINGZHIDI_BEGIN,	// 跨服边境之地, 下发BOSS信息 14280

		MT_CROSS_SERVER_BIANJINGZHIDI_BOSS_INFO_CS = 14285,						// 跨服边境之地， 获取BOSS信息

		// =============================================================================================================
		
		MT_CROSS_RA_CHONGZHI_RANK_GET_RANK_CS = 14901,						// 跨服充值排行活动，请求战区排行榜
	
		MT_CROSS_RA_CHONGZHI_RANK_GET_RANK_ACK_SC = 14907,					// 跨服充值排行活动，请求战区排行榜返回

		MT_CROSS_RA_CONSUME_RANK_GET_RANK_CS = 14908,						// 跨服消费排行活动，请求战区排行榜
		MT_CROSS_RA_CONSUME_RANK_GET_RANK_ACK_SC = 14909,					// 跨服消费排行活动，请求战区排行榜返回
		
		MT_CROSS_SERVER_MSG_MAX,
	};
}

#endif	// __CROSS_MSG_CODE_H__

