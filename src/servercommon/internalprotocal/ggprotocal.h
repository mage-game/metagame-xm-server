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

// ���͸�����������ͨ��UserID�����е�ͨѶЭ��

namespace ggprotocal
{
	// MAX_GGPROTOCAL_LENGTH 1024 ������servercommon.h��
	enum _MsgType
	{
		MT_INVALID,

		MT_GG_BEGIN = 22000,
		
		/////////////////////////////////////////////  ���� /////////////////////////////////////////////
		MT_ADD_FRIEND_REQ,									// �Ӻ�������
		MT_ADD_FRIEND_ACK,									// �Ӻ�����Ӧ
		MT_DELETE_FRIEND_REQ,								// ɾ������
		MT_FRIEND_RESET_NAME,								// ���Ѹ���֪ͨ���ߺ���
		MT_FRIEND_SET_SEX,									// ���Ѹı��Ա�֪ͨ���ߺ���
		
		/////////////////////////////////////////////  ��� /////////////////////////////////////////////
		MT_RESET_TEAM_INFO,									// ���������Ϣ
		MT_LEAVE_TEAM,										// �뿪����


		/////////////////////////////////////////////  ���� /////////////////////////////////////////////
		MT_ROLE_FORBID_TALK,								// ����	
		MT_ON_USER_REMOVE,									// global֪ͨgameworld�Ƴ���ɫ
		MT_MAIL_ATTACHMENT,									// ֪ͨgameworld��ȡ�ʼ����� 
		MT_SYSTEMCMD_TO_ROLE,								// �ı��ɫ�������ȣ���ֵ������
		MT_SYSTEMCMD_TO_ROLE_COUNT,							// ���ý�ɫ���ܴ�����͵������ȣ�
		MT_SYSTEMCMD_TO_ROLE_FB_COUNT,						// ���ý�ɫ��������
		MT_GUILD_SHOP_BUY_ACK,								// �����̵깺����Ʒ����
		MT_SHOP_BUY_LIMIT_ITEM_ACK,							// �̳ǹ����޹���Ʒ��鷵��
		MT_GET_ONE_EQUIPMENT_INFO,							// ��ȡ����װ����Ϣ
		MT_GET_ONE_PET_INFO,								// ��ȡ����������Ϣ
		MT_GET_ONE_MOUNT_INFO,								// ��ȡ����������Ϣ
		MT_CHONGZHI_NOTICE,									// ��ֵ֪ͨ

		MT_ADD_MOUNT_WITH_DATA,								// ��Ӵ����ݵ�����
		MT_ADD_PET_WITH_DATA,								// ��Ӵ����ݵĳ���
		MT_KILL_OTHER_CAMP_GET_HONOUR,						// ��ɱ������Ӫ��һ������
		
		MT_GIVE_FLOWER,										// �ͻ�
		MT_BE_GIVE_FLOWER,									// ���ͻ�

		MT_GET_YUANLIFB_CHAPTER_LORD_GIFT,					// ��ȡԪ�������������

		MT_BUY_PUBLICSALE_ITEM_REQ,							// global->gameworld ������Ʒ����
		MT_FETCH_OPEN_GAME_SUB_ACTIVITY_RANK_REWARD,		// ��ȡ����������ӻ����

		MT_XIANMENGZHAN_ENTER_REQ,							// ����ս���������

		MT_RANDACTIIVITY_BUY_REQ_TO_GS,						// global->gameworld �����޹���������
		MT_ACTIVITY_FIND_REQ_TO_GS,							// global->gameworld ��һ�����

		MT_QUERY_POSITION,									// ��ѯλ��
		MT_RSP_POSITION,									// ��Ӧλ��

		/////////////////////////////////////////////  ���� /////////////////////////////////////////////
		MT_TRADE_AFFIRM_ROUTE,								// ����ȷ��
		MT_TRADE_SYNC_STATE_ROUTE,							// ͬ������״̬
		MT_TRADE_CONSUME_REQ,								// global������뽻�׳ɹ�״̬ ֪ͨgameworld��ʼ������Ʒ


		/////////////////////////////////////////////  ���� /////////////////////////////////////////////
		MT_GUILD_SYNC_ROLE_INFO,							// globalͬ��������Ϣ��gameworld
		MT_CREATE_GUILD_FAIL_ROLLBACK,						// ��������ʧ�� �ع� 
		MT_GUILD_DELATE_FAIL_ROLLBACK,						// ����ʧ�� �ع�
		MT_GUILD_BOSS_FEED_CONSUME_REQ,						// ����BOSSι������
		MT_GUILD_STORAGE_PUT_ITEM_CONSUME_REQ,				// ���Ųֿ��������
		MT_GUILD_STORAGE_TAKE_ITEM_GIVE_REQ,				// ���Ųֿ�ȡ������
		MT_GUILD_XIANGFANG_UPGRADE_LEVEL_CONSUME_REQ,		// �����᷿����
		MT_GUILD_JIANZHU_ADDEXP_CONSUME_REQ,				// ������������

		/////////////////////////////////////////////  ��� /////////////////////////////////////////////
		MT_MARRY_RESERVE_REQ,								// ���ԤԼ
		MT_MARRY_HUNYAN_ENTER,								// �������
		MT_DATING_INVITE_CHECK_TARGET,						// Լ�������鱻������
		MT_DATING_CONSUME,									// Լ������
		MT_DATING_REWARD,									// Լ�ά��
		MT_DATING_UPDATE_ADD_EXP,							// Լ�ᶨʱ�Ӿ���
		MT_DATING_DAYCOUNT_CHECK,							// Լ��ÿ�մ������
		MT_LOVER_RESET_NAME,								// ���˸���

		///////////////////////////////////////////  ����   //////////////////////////////////////////////
		MT_FRIEND_SET_LEVEL,								// ���ѵȼ��ı�֪ͨ���ߺ���
	};

	struct MessageHeader
	{
		MessageHeader() : msg_type(MT_INVALID) {}
		MessageHeader(int type) : msg_type(type) {}

		int msg_type;
	};


	/////////////////////////////////////////////  ���� /////////////////////////////////////////////
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
		char is_accept;						// 0��ͬ�� 1ͬ��
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

	/////////////////////////////////////////////  �����ͻ� /////////////////////////////////////////////

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

	/////////////////////////////////////////////  ��� /////////////////////////////////////////////
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


	/////////////////////////////////////////////  ���� /////////////////////////////////////////////
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

	/////////////////////////////////////////////  ���� /////////////////////////////////////////////
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


	/////////////////////////////////////////////  ���� /////////////////////////////////////////////
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

	/////////////////////////////////////////////  Լ�� /////////////////////////////////////////////

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
			CONSUME_TYPE_INVITE_COST,		// ��������
			CONSUME_TYPE_DATING_INVITE,		// ����ÿ���������
			CONSUME_TYPE_DATING_BE_INVITED, // ����ÿ�ձ��������
			CONSUME_TYPE_DATING_GIVE_PRESENT,	// ��������
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


