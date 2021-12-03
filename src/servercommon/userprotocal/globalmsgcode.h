#ifndef __GLOBALMSGCODE_H__
#define __GLOBALMSGCODE_H__

namespace Protocol
{
	enum _MsgTypeGlobal
	{
		//////////////////////////////////////////////// 全局服务器相关 /////////////////////////////////////////////////////////
		MT_ENTER_GLOBAL_SERVER_ACK_SC = 9000,									// 请求登陆全局服务器回复
		MT_TIME_ACK_SC = 9001,													// 服务器时间返回
		MT_WALLOW_NOTICE_SC = 9002,												// 用户防沉迷返回
		MT_DISCONNECT_NOTICE_SC = 9003,											// 通知客户端断开服务器

		MT_ENTER_GLOBAL_SERVER_REQ_CS = 9050,									// 请求登陆全局服务器
		MT_TIME_REQ_CS = 9051,													// 请求服务器时间

		//////////////////////////////////////////////// 组队相关 /////////////////////////////////////////////////////////
		MT_TEAM_INFO_SC = 9100,													// 队伍信息
		MT_OUT_OF_TEAM_SC = 9101,												// 队伍已经解散
		MT_INVITE_USER_TRANSMIT_SC = 9102,										// 通知被邀请
		MT_REQ_JOIN_TEAM_SC = 9103,												// 通知队长有人申请加入队伍
		MT_TEAM_LIST_ACK_SC = 9104,												// 请求所在场景队伍列表回复
		MT_JOIN_TEAM_SC = 9105,													// 通知玩家加入了队伍
		MT_ROLE_TEAM_INFO_SC = 9106,											// 角色相关的队伍信息
		MT_TEAM_ROLL_DROP_RET_SC = 9107,										// 检取物品时摇出点数
		MT_TEAM_LEADER_CHANGE_SC = 9108,										// 通知队长发生变更
		
		MT_CREATE_TEAM_CS = 9150,												// 创建队伍
		MT_INVITE_USER_CS = 9151,												// 邀请加入队伍
		MT_INVITE_RET_CS = 9152,												// 回复邀请
		MT_REQ_JOIN_TEAM_RET_CS = 9153,											// 队长回复允许申请者加入队伍
		MT_REQ_JOIN_TEAM_CS = 9154,												// 玩家申请加入某队伍
		MT_DISMISS_TEAM_CS = 9155,												// 队长解散队伍
		MT_KICK_OUT_OF_TEAM_CS = 9156,											// 队长将队员踢出队伍
		MT_CHANGE_TEAM_LEADER_CS = 9157,										// 换队长
		MT_EXIT_TEAM_CS = 9158,													// 玩家退出队伍
		MT_TEAM_LIST_REQ_CS = 9159,												// 请求所在场景的队伍列表
		MT_CHANGE_MUST_CHECK_CS = 9160,											// 改变队伍加入是否是要队长通过
		MT_CHANGE_ASSIGN_MODE_CS = 9161,										// 改变队伍分配模式
		MT_CHANGE_MEMBER_CAN_INVITE_CS = 9162,									// 改变队伍普通队员是否可邀请
		MT_CHANGE_TEAM_LIMIT_CS = 9163,											// 修改队伍加入限制
		MT_AUTO_HAVE_TEAM_CS = 9164,											// 自动组队
		MT_AUTO_APPLY_JOIN_TEAM_CS = 9165,										// 自动答应队伍邀请
		MT_INVITE_UNIQUE_USER_CS = 9166,										// 邀请加入队伍（使用uuid）

		//////////////////////////////////////////////// 其他相关 /////////////////////////////////////////////////////////
		MT_GET_ROLE_SIMPLE_INFO_SC = 9300,										// 查看玩家简单资料返回
		MT_ROLE_IS_OFFLINE_OR_NOT_EXISTED,										// 玩家下线或角色不存在
		MT_RANDOM_ROLE_LIST_SC,													// 随机玩家列表返回							
		MT_ACTIVITY_STATUS_SC,													// 活动状态返回
		MT_ACTIVITY_ROOM_INFO_ACK_SC,											// 请求温泉信息返回
		MT_QUESTION_RANK_INFO_ACK_SC,											// 请求答题榜返回
		MT_QUESTION_TOTAL_RESULT_SC,											// 玩家答题活动最终结果
		MT_QUESTION_REBEGIN_SC,													// 答题活动重新开始
		MT_QUESTION_ANWER_RESULT_SC,											// 本题回答结果
		MT_ROLE_EVALUATE_INFO_SC,												// 返回评价结果
		MT_GET_ROLE_LIST_CAP_SC,												// 查看角色列表战斗力
		MT_XIANNV_ALL_LIMIT_BUY_COUNT_SC,										// 随机活动：仙女全服限购剩余数量
		MT_PET_ALL_LIMIT_BUY_COUNT_SC,											// 随机活动：宠物全服限购剩余数量
		MT_RANDACTIVITY_LUCKY_NEWS_SC,											// 随机活动：幸运转盘玩家列表
		MT_DATING_TARGET_LIST_SC,												// 约会目标列表信息
		MT_DATING_NOTIFY_SC,													// 约会相关提醒
		MT_COMBINE_QINGDIAN_ROLL_NEWS_SC,										// 合服庆典累冲转盘玩家列表
		
		MT_FIND_ROLE_BY_NAME_CS = 9350,											// 按玩家名称查找玩家
		MT_GET_ROLE_SIMPLE_INFO_CS,												// 查看玩家简单资料（global里面信息 返回9301协议）
		MT_GET_ROLE_BASE_INFO_CS,												// 查看玩家基本资料（转到gameworld获取信息）
		MT_GET_RANDOM_ROLE_LIST_CS,												// 获取随机的玩家列表（作为推荐好友用）
		MT_HOT_SPRING_INFO_REQ_CS,												// 请求温泉信息
		MT_QUESTION_RANK_INFO_REQ_CS,											// 请求答题榜
		MT_GET_ONE_EQUIPMENT_INFO_CS,											// 请求单个装备信息
		MT_GET_ONE_PET_INFO_CS,													// 请求单个宠物信息
		MT_GET_ONE_MOUNT_INFO_CS,												// 请求单个坐骑信息
		MT_GET_ONE_SOUL_INFO_CS,												// 请求单个将魂信息
		MT_GET_TRUCK_POS_INFO_CS,												// 请求镖车位置信息
		MT_GET_ROLE_SIGNATURE_CS,												// 请求角色签名
		MT_GET_ROLE_DAY_COUNT_CS,												// 请求角色计数信息
		MT_GET_ROLE_LIST_CAP_CS,												// 查看角色列表战斗力
		MT_LAND_WAR_JOIN_REQ_CS,												// 领地战报名
		MT_LAND_WAR_ENTER_MATCH_REQ_CS,											// 领地战请求入场
		MT_LAND_WAR_GIVEUP_LAND_CS,												// 领地战放弃领地
		MT_LAND_WAR_GET_LAND_LIST_CS,											// 领地战拉取各领地信息
		MT_LAND_WAR_GET_JOIN_GUILD_LIST_CS,										// 领地战拉取报名军团列表
		MT_LAND_WAR_GET_REPORT_LIST_CS,											// 领地战拉取战报列表
		MT_MARRY_RESERVE_REQ_CS,												// 结婚预约
		MT_MARRY_HUNYAN_ENTER_REQ_CS,											// 请求进入婚宴
		MT_MARRY_GET_MARRY_RESERVE_INFO_CS,										// 请求婚礼预约信息
		MT_GET_ACTIVITY_ROME_STATUS_CS,											// 请求活动房间信息
		MT_RAND_ACTIVITY_XIANNV_BUY_REQ_CS,										// 随机活动：仙女限购购买请求
		MT_GET_XIANNV_ALL_LIMIT_BUY_COUNT_CS,									// 随机活动：获取仙女全服限购剩余数量
		MT_RAND_ACTIVITY_PET_BUY_REQ_CS,										// 随机活动：宠物限购购买请求
		MT_GET_PET_ALL_LIMIT_BUY_COUNT_CS,										// 随机活动：获取宠物全服限购剩余数量
		MT_DATING_GET_TARGET_LIST_CS,											// 获取约会对象列表
		MT_DATING_CANCEL_INVITE_CS,												// 取消约会邀请
		MT_DATING_EVALUTE_CS,													// 约会评价
		MT_DATING_QUIT_CS,														// 退出约会
		MT_DATING_GIVE_PRESENT_CS,												// 约会赠送礼物
		MT_DATING_ANSWER_QUESTION_CS,											// 约会答题
		MT_ACTIVITY_FIND_REQ_CS,												// 活动找回请求
		MT_COMBINE_QINGDIAN_ROLL_NEWS_REQ_CS,									// 合服庆典累冲转盘捷报请求

		//////////////////////////////////////////////// 邮件相关 /////////////////////////////////////////////////////////
		MT_GLOBAL_MAIL_SEND_RESULT_SC = 9500,									// 提取附件返回
		MT_GLOBAL_MAIL_DELETE_SC = 9501,										// 删除邮件返回
		MT_GLOBAL_MAIL_LOCK_SC = 9502,											// 锁邮件返回
		MT_GLOBAL_MAIL_UNLOCK_SC = 9503,										// 解锁邮件返回
		MT_GLOBAL_MAIL_LIST_SC = 9504,											// 邮件列表返回
		MT_GLOBAL_MAIL_DETAIL_SC = 9505,										// 邮件详细信息
		MT_GLOBAL_MAIL_ATTACHMENT_SC = 9506,									// 提取邮件附件返回
		MT_GLOBAL_MAIL_NEW_MAIL_NOTICE_SC = 9507,								// 新邮件通知
		MT_GLOBAL_MAIL_HAS_UNREAD_MAIL_SC = 9508,								// 未读取邮件通知
		
		//////////////////////////////////////////////// 交易市场 /////////////////////////////////////////////////////////
		MT_PUBLICSALE_SEARCH_CS = 9650,											// 搜索

		MT_PUBLICSALE_ADD_ACK_SC = 9600,										// 拍卖物品上架返回
		MT_PUBLICSALE_DEL_ACK_SC = 9601,										// 拍卖物品下架返回
		MT_PUBLICSALE_BUY_ACK_SC = 9602,										// 购买拍卖物品返回
		MT_PUBLICSALE_GET_ITEM_LIST_SC = 9603,									// 获取自己拍卖物品列表
		MT_PUBLICSALE_SEARCH_SC = 9604,											// 搜索返回 

		//////////////////////////////////////////////// 排行榜相关 /////////////////////////////////////////////////////////
		MT_RANK_GET_PERSONRANK_LIST_ACK_SC = 10000,
		MT_RANK_GET_GUILDRANK_LIST_ACK_SC = 10001,
		MT_RANK_GET_TEAMRANK_LIST_ACK_SC = 10002,
		MT_RANK_GET_PERSONRANK_TOP_USER_SC = 10003,
		MT_RANK_GET_WORLD_LEVEL_ACK_SC = 10004,
		MT_RANK_GET_COUPLE_RANK_SC = 10005,
		MT_RANK_GET_SPECIAL_RANK_SC = 10006, 

		MT_RANK_GET_PERSONRANK_LIST_CS = 10050,
		MT_RANK_GET_GUILDRANK_LIST_CS = 10051,
		MT_RANK_GET_TEAMRANK_LIST_CS = 10052,
		MT_RANK_GET_PERSONRANK_TOP_USER_CS = 10053,
		MT_RANK_GET_COUPLE_RANK_LIST_CS = 10054,
		MT_RANK_GET_SPECIAL_RANK_LIST_CS = 10055, 
	
		//////////////////////////////////////////////// 军团相关 /////////////////////////////////////////////////////////
		MT_CREATE_GUILD_SC = 9800,												// 返回“创建军团”结果
		MT_DISMISS_GUILD_SC = 9801,												// 返回“解散军团”结果
		MT_APPLY_FOR_JOIN_GUILD_SC = 9802,										// 返回“申请加入军团” 结果
		MT_NOTIFY_SUPER_SC = 9803,												// 通知军团管理者 
		MT_QUIT_GUILD_SC = 9804,												// 返回“退出军团” 结果
		MT_INVITE_GUILD_SC = 9805,												// 邀请加入军团 返回
		MT_INVITE_GUILD_NOTIFY_SC = 9806,										// 邀请加入军团 通知
		MT_KICK_OUT_GUILD_SC = 9807,											// 踢出军团返回
		MT_KICK_OUT_GUILD_NOTIFY_SC = 9808,										// 提出军团通知
		MT_APPOINT_GUILD_SC = 9809,												// 任命返回
		MT_LEAVEPOST_GUILD_SC = 9810,											// 卸任返回
		MT_CHANGE_NOTICE_SC = 9811,												// 修改公告返回
		MT_MAILALL_SC = 9812,													// 群发邮件返回
		MT_GUILD_ALL_BASE_INFO_SC = 9813,										// 军团列表
		MT_GUILD_APPLY_FOR_LIST_SC = 9814,										// 军团申请列表
		MT_GUILD_EVENT_LIST_SC = 9815,											// 军团普通事件和大事件列表
		MT_GUILD_MEMBER_LIST_SC = 9816,											// 军团成员列表
		MT_GUILD_BASE_INFO_SC = 9817,											// 军团基本信息
		MT_SYNC_GUILD_INFO_SC_RESERVED = 9818,									// 同步军团信息 保留
		MT_APPLY_FOR_JOIN_GUILD_ACK_SC = 9819,									// 回复加入申请通知
		MT_APPLY_FOR_JOIN_GUILD_LIST_SC = 9820,									// 已发申请的军团列表返回
		MT_GUILD_MEMBER_SOS_SC = 9821,											// 军团成员求救通知
		MT_GUILD_INVITE_LIST_SC = 9822,											// 军团邀请列表
		MT_GUILD_RESET_NAME_SC = 9823,											// 军团改名
		MT_GUILD_CHECK_CAN_DELATE_ACK_SC = 9824,								// 军团能否弹劾回复
		MT_GUILD_OPERA_SUCC_SC = 9825,											// 军团操作成功返回
		MT_GUILD_ROLE_GUILD_INFO_SC = 9826,										// 角色的军团信息
		MT_GUILD_CUR_MAX_MEMBER_COUNT_SC = 9827,								// 军团当前最大成员数量
		MT_GUILD_ROLE_GUILD_PAPER_INFO_SC = 9828,								// 角色的公会红包信息
		MT_GUILD_ROLE_GUILD_PAPER_CREATE_CS = 9829,								// 角色请求创建红包
		MT_GUILD_ROLE_GUILD_PAPER_SINGAL_CHAT_CS = 9830,						// 私聊有红包未派发的玩家
		MT_GUILD_MAZE_OPERATE_CS = 9831,										// 公会迷宫操作请求
		MT_GUILD_MEMBER_MAZE_INFO_SC = 9832,									// 公会成员迷宫信息
		MT_GUILD_MAZE_RANK_INFO_SC = 9833,										// 公会迷宫排行信息
		MT_GUILD_SIGNIN_REQ_CS = 9834,											// 仙盟签到请求
		MT_GUILD_SIGNIN_ALL_INFO_SC = 9835,										// 仙盟签到所以信息
		MT_GUILD_CHANGE_AVATAR_CS = 9836,										// 仙盟头像更换
		MT_GUILD_APPEARANCE_INFO_SC = 9837,										// 仙盟外观信息
		MT_GUILD_BIAOCHE_OPERA_CS = 9838,										// 帮派镖车请求

		//////////////////////////////////////////////// 世界boss /////////////////////////////////////////////////////////
		MT_WORLD_BOSS_INFO_TO_ALL_SC = 10100,									// 发送世界boss广播信息
		MT_WORLDBOSS_INFO_SC = 10101,											// 发送世界boss信息
		MT_WORLDBOSS_BORN_SC = 10102,											// 发送世界boss出生
		MT_SEND_WORLDBOSS_PERSONAL_HURT_INFO_SC = 10103,						// 发送世界boss个人伤害排名信息
		MT_SEND_WORLDBOSS_GUILD_HURT_INFO_SC = 10104,							// 发送世界boss公会伤害排名信息
		MT_WORLDBOSS_WEEK_RANK_SC = 10105,										// 发送世界周榜信息
		MT_WORLDBOSS_CAN_ROLL_SC = 10106,										// 发送boss护盾被破，可摇点
		MT_WORLDBOSS_ROLL_INFO_SC = 10107,										// 发送玩家摇点结果
		MT_WORLDBOSS_TOP_ROLL_INFO_SC = 10108,									// 发送玩家最高摇点结果
		MT_WORLDBOSS_KILLER_LIST_SC = 10109,									// 发送boss击杀列表
		MT_WORLDBOSS_HP_INFO_SC = 10111,										// boss血量信息

		MT_WORLDBOSS_KILLER_INFO_CS = 10150,									// 请求世界boss击杀列表
		MT_GET_WORLDBOSS_INFO_CS = 10151,										// 请求世界boss信息
		MT_GET_WORLDBOSS_PRESONAL_HURT_INFO_CS = 10152,							// 请求世界boss个人伤害排名信息
		MT_GET_WORLDBOSS_GUILD_HURT_INFO_CS = 10153,							// 请求世界boss公会伤害排名信息
		MT_GET_WORLDBOSS_WEEK_RANK_INFO_CS = 10154,								// 请求世界boss击杀数量周榜信息
		MT_GET_WORLDBOSS_ROLL_REQ_CS = 10155,									// 请求摇点

		//////////////////////////////////////////////// 阵营 /////////////////////////////////////////////////////////
		MT_CAMP_INFO_SC = 10200,												// 阵营信息返回

		MT_CAMP_INFO_CS = 10250,												// 拉取阵营信息
	};
}

#endif // __GLOBALMSGCODE_H__


