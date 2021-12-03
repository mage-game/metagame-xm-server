#ifndef __GGPROTOCAL_H__
#define __GGPROTOCAL_H__

#include "servercommon/basedef.h"
#include "servercommon/maildef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/guilddef.hpp"
#include "servercommon/tradedef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/struct/roleglobalmoduleinfo.hpp"

#pragma pack(push) 
#pragma pack(4)

// 发送给场景服务器通过UserID来进行的通讯协议

namespace ggprotocal
{
	// MAX_GGPROTOCAL_LENGTH 1024 定义在servercommon.h中
	enum _MsgType
	{
		MT_INVALID,

		MT_GG_BEGIN = 22000,
		
		/////////////////////////////////////////////  好友 /////////////////////////////////////////////
		MT_ADD_FRIEND_REQ,									// 加好友请求
		MT_ADD_FRIEND_ACK,									// 加好友响应
		MT_DELETE_FRIEND_REQ,								// 删除好友
		MT_FRIEND_RESET_NAME,								// 好友改名通知在线好友
		MT_FRIEND_SET_SEX,									// 好友改变性别通知在线好友
		
		/////////////////////////////////////////////  组队 /////////////////////////////////////////////
		MT_RESET_TEAM_INFO,									// 更新组队信息
		MT_LEAVE_TEAM,										// 离开队伍


		/////////////////////////////////////////////  其他 /////////////////////////////////////////////
		MT_ROLE_FORBID_TALK,								// 禁言	
		MT_ON_USER_REMOVE,									// global通知gameworld移除角色
		MT_MAIL_ATTACHMENT,									// 通知gameworld提取邮件附件 
		MT_SYSTEMCMD_TO_ROLE,								// 改变角色（威望等）数值的命令
		MT_SYSTEMCMD_TO_ROLE_COUNT,							// 设置角色功能次数（偷猪次数等）
		MT_SYSTEMCMD_TO_ROLE_FB_COUNT,						// 设置角色副本次数
		MT_GUILD_SHOP_BUY_ACK,								// 军团商店购买物品返回
		MT_SHOP_BUY_LIMIT_ITEM_ACK,							// 商城购买限购物品检查返回
		MT_GET_ONE_EQUIPMENT_INFO,							// 获取单个装备信息
		MT_GET_ONE_PET_INFO,								// 获取单个宠物信息
		MT_GET_ONE_MOUNT_INFO,								// 获取单个坐骑信息
		MT_CHONGZHI_NOTICE,									// 充值通知

		MT_ADD_MOUNT_WITH_DATA,								// 添加带数据的坐骑
		MT_ADD_PET_WITH_DATA,								// 添加带数据的宠物
		MT_KILL_OTHER_CAMP_GET_HONOUR,						// 击杀其他阵营玩家获得荣誉
		
		MT_GIVE_FLOWER,										// 送花
		MT_BE_GIVE_FLOWER,									// 被送花

		MT_GET_YUANLIFB_CHAPTER_LORD_GIFT,					// 获取元力副本霸主礼包

		MT_BUY_PUBLICSALE_ITEM_REQ,							// global->gameworld 购买物品请求
		MT_FETCH_OPEN_GAME_SUB_ACTIVITY_RANK_REWARD,		// 领取开服活动排行子活动奖励

		MT_XIANMENGZHAN_ENTER_REQ,							// 仙盟战，请求进入

		MT_RANDACTIIVITY_BUY_REQ_TO_GS,						// global->gameworld 随机活动限购购买请求
		MT_ACTIVITY_FIND_REQ_TO_GS,							// global->gameworld 活动找回请求

		MT_QUERY_POSITION,									// 查询位置
		MT_RSP_POSITION,									// 响应位置

		/////////////////////////////////////////////  交易 /////////////////////////////////////////////
		MT_TRADE_AFFIRM_ROUTE,								// 交易确认
		MT_TRADE_SYNC_STATE_ROUTE,							// 同步交易状态
		MT_TRADE_CONSUME_REQ,								// global请求进入交易成功状态 通知gameworld开始消耗物品


		/////////////////////////////////////////////  军团 /////////////////////////////////////////////
		MT_GUILD_SYNC_ROLE_INFO,							// global同步军团信息到gameworld
		MT_CREATE_GUILD_FAIL_ROLLBACK,						// 创建军团失败 回滚 
		MT_GUILD_DELATE_FAIL_ROLLBACK,						// 弹劾失败 回滚
		MT_GUILD_BOSS_FEED_CONSUME_REQ,						// 军团BOSS喂养消耗
		MT_GUILD_STORAGE_PUT_ITEM_CONSUME_REQ,				// 军团仓库放入消耗
		MT_GUILD_STORAGE_TAKE_ITEM_GIVE_REQ,				// 军团仓库取出给予
		MT_GUILD_XIANGFANG_UPGRADE_LEVEL_CONSUME_REQ,		// 升级厢房消耗
		MT_GUILD_JIANZHU_ADDEXP_CONSUME_REQ,				// 升级建筑消耗

		/////////////////////////////////////////////  结婚 /////////////////////////////////////////////
		MT_MARRY_RESERVE_REQ,								// 结婚预约
		MT_MARRY_HUNYAN_ENTER,								// 进入婚宴
		MT_DATING_INVITE_CHECK_TARGET,						// 约会邀请检查被邀请人
		MT_DATING_CONSUME,									// 约会消耗
		MT_DATING_REWARD,									// 约会奖励
		MT_DATING_UPDATE_ADD_EXP,							// 约会定时加经验
		MT_DATING_DAYCOUNT_CHECK,							// 约会每日次数检查
		MT_LOVER_RESET_NAME,								// 爱人改名

		///////////////////////////////////////////  好友   //////////////////////////////////////////////
		MT_FRIEND_SET_LEVEL,								// 好友等级改变通知在线好友
	};

	struct MessageHeader
	{
		MessageHeader() : msg_type(MT_INVALID) {}
		MessageHeader(int type) : msg_type(type) {}

		int msg_type;
	};


	/////////////////////////////////////////////  好友 /////////////////////////////////////////////
	struct AddFriendReq 
	{
		AddFriendReq() : header(MT_ADD_FRIEND_REQ) {}
		MessageHeader header;

		int req_db_index;
		int req_role_id;
		GameName req_name;
		char req_avatar;
		char req_sex;
		char req_prof;
		char req_camp;
		int req_level;
		int is_yi_jian;
	};

	struct AddFriendAck 
	{
		AddFriendAck() : header(MT_ADD_FRIEND_ACK) {}
		MessageHeader header;

		int db_index_friend;
		int role_id_friend;
		GameName friend_name;
		char is_accept;						// 0不同意 1同意
		char reserved;
		char friend_sex;
		char friend_prof;
	};

	struct DeleteFriendReq
	{
		DeleteFriendReq() : header(MT_DELETE_FRIEND_REQ) {}
		MessageHeader header;

		int req_db_index;
		int req_role_id;
		GameName req_name;
		char req_avatar;
		char req_sex;
		char req_prof;
		char req_camp;
		int req_level;
	};

	struct FriendResetName
	{
		FriendResetName() : header(MT_FRIEND_RESET_NAME) {}
		MessageHeader header;

		int friend_role_id;
		GameName friend_new_name;
	};

	struct FriendSetSex
	{
		FriendSetSex() : header(MT_FRIEND_SET_SEX) {}
		MessageHeader header;

		int friend_role_id;
		int friend_plat_type;
		int sex;
	};

	struct FriendSetLevel
	{
		FriendSetLevel() : header(MT_FRIEND_SET_LEVEL) {}
		MessageHeader header;

		int friend_role_id;
		int level;
	};

	/////////////////////////////////////////////  好友送花 /////////////////////////////////////////////

	struct GiveFlower
	{
		GiveFlower() : header(MT_GIVE_FLOWER) {}
		MessageHeader		header;

		short grid_index;
		ItemID item_id;
		int target_uid;
		short is_anonymity;
		short is_marry;
	};

	struct BeGiveFlower
	{
		BeGiveFlower() : header(MT_BE_GIVE_FLOWER) {}
		MessageHeader		header;

		int from_uid;
		int flower_num;
		short from_camp;
		short is_anonymity;
		GameName from_name;
		ItemID item_id;
		short reserve;
	};

	/////////////////////////////////////////////  组队 /////////////////////////////////////////////
	struct TeamMemberItem 
	{
		int				db_index;
		int				role_id;
		int				scene_id;
		int				scene_key;
		SceneIndex		scene_index;
		ObjID			obj_id;
		char			is_online;
		char			towerdefend_attrtype;
		short			reserve;
	};

	static const int MAX_TEAM_MEMBERITEM_COUNT = 10;

	struct UpdateTeamInfo
	{
		UpdateTeamInfo() : header(MT_RESET_TEAM_INFO) {}
		MessageHeader		header;
	
		int					team_index;
		int					team_seq;
		char				team_leader_index;
		char				assign_mode;
		short				limit_level;
		int					limit_capability;
		int					member_count;
		TeamMemberItem		team_member[MAX_TEAM_MEMBERITEM_COUNT];
	};

	struct RoleLeaveTeam
	{
		RoleLeaveTeam() : header(MT_LEAVE_TEAM) {} 
		MessageHeader		header;
	};


	/////////////////////////////////////////////  其他 /////////////////////////////////////////////
	struct RoleForbidTalk
	{
		RoleForbidTalk() : header(MT_ROLE_FORBID_TALK) {}
		MessageHeader header;

		unsigned int last_time;
	};

	struct OnGlobalUserRemove
	{
		OnGlobalUserRemove() : header(MT_ON_USER_REMOVE) {}
		MessageHeader header;
	};

	struct MailAttachment
	{
		MailAttachment() : header(MT_MAIL_ATTACHMENT) {}

		MessageHeader header;

		int sender_uid;
		int mail_index;
		int item_index;
		int is_last;

		int coin;
		int coin_bind;
		int gold;
		int gold_bind;

		int item_count;
		ItemDataWrapper item_list[MAX_ATTACHMENT_ITEM_NUM];

		MailVirtualItemList virtual_item_list;
		MailSubject subject;
		MailContentTxt contenttxt;
	};

	struct SystemCmdToRole
	{
		SystemCmdToRole() : header(MT_SYSTEMCMD_TO_ROLE) {}
		MessageHeader header;

		int effect_type;
		int	effect_param;
		int	effect_param1;
	};

	struct SystemCmdToRoleCount
	{
		SystemCmdToRoleCount() : header(MT_SYSTEMCMD_TO_ROLE_COUNT) {}
		MessageHeader header;

		int count_type;
		int	count;
	};

	struct SystemCmdToRoleFbCount
	{
		SystemCmdToRoleFbCount() : header(MT_SYSTEMCMD_TO_ROLE_FB_COUNT) {}
		MessageHeader header;

		int day_count_id;
		int	count;
	};

	struct ChongzhiNotice
	{
		ChongzhiNotice() : header(MT_CHONGZHI_NOTICE) {}
		MessageHeader header;
	};

	struct GuildShopBuyAck
	{
		GuildShopBuyAck() : header(MT_GUILD_SHOP_BUY_ACK) {}
		MessageHeader header;

		int is_allow;
		UInt16 item_id;
		short item_num;
	};

	struct GetOneEquipmentInfo
	{
		GetOneEquipmentInfo() : header(MT_GET_ONE_EQUIPMENT_INFO) {}
		MessageHeader header;

		int	viewer_db_index;
		int	viewer_role_id;
		int equipment_index;
	};

	struct GetOnePetInfo
	{
		GetOnePetInfo() : header(MT_GET_ONE_PET_INFO) {}
		MessageHeader header;

		int	viewer_db_index;
		int	viewer_role_id;
		int pet_index;
	};

	struct GetOneMountInfo
	{
		GetOneMountInfo() : header(MT_GET_ONE_MOUNT_INFO) {}
		MessageHeader header;

		int	viewer_db_index;
		int	viewer_role_id;
		int mount_index;
	};

	struct AddMountWithData
	{
		AddMountWithData() : header(MT_ADD_MOUNT_WITH_DATA) {}
		MessageHeader		header;

		UInt16				mount_id;
		short				grade;
		short				strengthen_level;	
		short				reserve;
	};

	struct AddPetWithData
	{
		AddPetWithData() : header(MT_ADD_PET_WITH_DATA) {}
		MessageHeader		header;

		UInt16				pet_id;
		short				level;
		short				wuxing;
		short				grow;
		short				zizhi;
		short				reserve;
	};

	struct GetYuanLiFBChapterLordGift
	{
		GetYuanLiFBChapterLordGift() : header(MT_GET_YUANLIFB_CHAPTER_LORD_GIFT) {}
		MessageHeader			header;

		int						chapter_index;
	};

	struct BuyPublicSaleItemReq
	{
		BuyPublicSaleItemReq() : header(MT_BUY_PUBLICSALE_ITEM_REQ) {}
		MessageHeader header;

		int seller_uid;
		int sale_index;

		int item_id;
		int item_num;
		int buy_money;
		int coin_price;
		int gold_price;
	};
	
	struct FetchOpenGameSubActivityRankReward
	{
		FetchOpenGameSubActivityRankReward() : header(MT_FETCH_OPEN_GAME_SUB_ACTIVITY_RANK_REWARD) {} 
		MessageHeader header;

		short sub_activity_type;
		short rank_pos;
	};

	struct XianMengZhanEnterReq
	{
	public:
		XianMengZhanEnterReq() : header(MT_XIANMENGZHAN_ENTER_REQ) {}
		MessageHeader header;
	};

	/////////////////////////////////////////////  交易 /////////////////////////////////////////////
	struct TradeAffirmRoute
	{
		TradeAffirmRoute() : header(MT_TRADE_AFFIRM_ROUTE) {}
		MessageHeader header;
		
		int		item_count;
		ItemDataWrapper item_list[P2P_TRADE_MAX_ITEM_COUNT]; 
	};

	struct TradeSyncStateRoute
	{
		TradeSyncStateRoute() : header(MT_TRADE_SYNC_STATE_ROUTE) {}
		MessageHeader header;

		int trade_state;

		int other_trade_state;
		int other_uid;
		int other_level;
		int other_capability;
		GameName other_user_name;
		PlatName other_plat_name;

		long long trade_id;
	};

	struct TradeConsumeReq
	{
		TradeConsumeReq() : header(MT_TRADE_CONSUME_REQ) {}
		MessageHeader header;
	};


	/////////////////////////////////////////////  军团 /////////////////////////////////////////////
	struct GuildSyncRoleInfo
	{
	public:
		GuildSyncRoleInfo() : header(MT_GUILD_SYNC_ROLE_INFO) {}

		MessageHeader		header;

		GuildID				guild_id;
		GuildName			guild_name;
		int					guild_post;
		GuildID				union_guild_id;
		int					guild_task_luck;
	};

	struct CreateGuildFailRollback
	{
		CreateGuildFailRollback() : header(MT_CREATE_GUILD_FAIL_ROLLBACK) {}
		MessageHeader		header;

		int					create_guild_type;
		ItemDataWrapper		item_wrapper;
	};

	struct GuildDelateFailRollback
	{
		GuildDelateFailRollback() : header(MT_GUILD_DELATE_FAIL_ROLLBACK) {}
		MessageHeader		header;

		ItemDataWrapper		item_wrapper;
	};

	struct GuildStoragePutItemConsumeReq
	{
		GuildStoragePutItemConsumeReq() : header(MT_GUILD_STORAGE_PUT_ITEM_CONSUME_REQ) {}
		MessageHeader		header;
		
		int					verify_key;

		int					put_index;
		int					knapsack_index;
		ItemDataWrapper		item_wrapper;
	};

	struct GuildStorageTakeItemGiveReq
	{
		GuildStorageTakeItemGiveReq() : header(MT_GUILD_STORAGE_TAKE_ITEM_GIVE_REQ) {}
		MessageHeader		header;

		int					verify_key;

		int					take_index;

		ItemDataWrapper		item_wrapper;
	};

	struct GuildBossFeedConsumeReq
	{
	public:
		GuildBossFeedConsumeReq() : header(MT_GUILD_BOSS_FEED_CONSUME_REQ) {}
		MessageHeader		header;

		int					feed_type;
		int					verify_key;
	};

	struct GuildXiangfangUpgradeLevelConsumeReq
	{
		GuildXiangfangUpgradeLevelConsumeReq() : header(MT_GUILD_XIANGFANG_UPGRADE_LEVEL_CONSUME_REQ) {}
		MessageHeader		header;

		int consume_gold;
		int verify_key;
	};

	struct GuildJianZhuAddExpConsumeReq
	{
		GuildJianZhuAddExpConsumeReq() : header(MT_GUILD_JIANZHU_ADDEXP_CONSUME_REQ) {}
		MessageHeader		header;

		int jianzhu_type;
		int use_gold;
		int value;
		int add_exp;
	};


	///////////////////////////////////////////////////////////////////////////////////

	struct KillOtherCampGetHonour
	{
		KillOtherCampGetHonour() : header(MT_KILL_OTHER_CAMP_GET_HONOUR) {}
		MessageHeader header;

		UserID bekiller_user_id;
		int delta_level;
	};

	struct MarryReserveReq
	{
		MarryReserveReq() : header(MT_MARRY_RESERVE_REQ) {}
		MessageHeader		header;

		short reserve_index;
		short marry_level;
		int rune_index;
	};

	struct MarryHunyanEnter
	{
		MarryHunyanEnter() : header(MT_MARRY_HUNYAN_ENTER) {}
		MessageHeader		header;
	};

	struct RandActivityBuyReqToGS
	{
	public:
		RandActivityBuyReqToGS() : header(MT_RANDACTIIVITY_BUY_REQ_TO_GS) {}
		MessageHeader header;

		short buy_type;
		short index;
		short num;
		short activity_type;
	};

	struct ActivityFindReqToGS
	{
	public:
		ActivityFindReqToGS() : header(MT_ACTIVITY_FIND_REQ_TO_GS) {}
		MessageHeader header;

		short is_find_reward;
		short activity_find_type;
		int activity_open_flag;
	};

	/////////////////////////////////////////////  约会 /////////////////////////////////////////////

	struct DatingInviteCheckTarget
	{
		DatingInviteCheckTarget() : header(MT_DATING_INVITE_CHECK_TARGET) {}
		MessageHeader		header;

		int					req_uid;
		int					is_specific_invite;
	};

	struct DatingConsume
	{
		DatingConsume() : header(MT_DATING_CONSUME) {}
		MessageHeader		header;

		enum
		{
			CONSUME_TYPE_INVITE_COST,		// 邀请消耗
			CONSUME_TYPE_DATING_INVITE,		// 消耗每日邀请次数
			CONSUME_TYPE_DATING_BE_INVITED, // 消耗每日被邀请次数
			CONSUME_TYPE_DATING_GIVE_PRESENT,	// 赠送礼物
		};

		int					consume_type;
		int					param;
	};

	struct DatingReward
	{
		DatingReward() : header(MT_DATING_REWARD) {}
		MessageHeader		header;

		int					question_score;
		int					other_uid;
		int					present_type;
	};

	struct DatingUpdateAddExp
	{
		DatingUpdateAddExp() : header(MT_DATING_UPDATE_ADD_EXP) {}
		MessageHeader		header;

		int					present_type;
	};

	struct DatingDaycountCheck
	{
		DatingDaycountCheck() : header(MT_DATING_DAYCOUNT_CHECK) {}
		MessageHeader		header;

		int					dating_side;
	};

	struct LoverResetName
	{
		LoverResetName() : header(MT_LOVER_RESET_NAME) {}
		MessageHeader header;

		int lover_role_id;
		GameName lover_new_name;
	};

	struct QueryPosition
	{
		QueryPosition() : header(MT_QUERY_POSITION) {}
		MessageHeader header;

		enum QP_REASON
		{
			QP_REASON_LOVER_SKILL = 0,
		};

		int from_uid;
		int target_uid;
		short reason;
		short reserve;
	};

	struct RspPosition
	{
		RspPosition() : header(MT_RSP_POSITION) {}
		MessageHeader header;

		int my_uid;
		short reason;
		char is_static_scene;
		char reserve;
		int my_scene_id;
		int my_scene_key;
		int pos_x;
		int pos_y;
	};
}

#pragma pack(pop)

#endif // __GGPROTOCAL_H__


