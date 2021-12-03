#ifndef __GGLOBALPROTOCAL_H__
#define __GGLOBALPROTOCAL_H__

#include "servercommon/serverdef.h"
#include "servercommon/basedef.h"

#include "servercommon/maildef.hpp"
#include "servercommon/guilddef.hpp"
#include "servercommon/rankdef.hpp"
#include "servercommon/tradedef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/roleshopdef.hpp"
#include "servercommon/struct/global/worldstatusparam.hpp"
#include "servercommon/activitydef.hpp"

#include "servercommon/roleactivitydef.hpp"
#include "servercommon/fbdef.hpp"
#include "servercommon/marrydef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace gglobalprotocal
{
	enum _MsgType
	{
		MT_INVALID,

		///////////////////////////////////////////   ����Э��   /////////////////////////////////////////
		MT_GGLOBAL_BEGIN = 21000,
		MT_USER_MSG,										// �û��ڷ������ڲ���Ϣ ��ggprotocalЭ���е����ݣ�	

		MT_MSG_TO_CROSS_SERVER,								// ��cross����Ϣ (crossgameprotocalЭ��)
		MT_CROSS_GAME_MSG,									// cross����gameworld����Ϣ (crossgameprotocalЭ��)	
		MT_MSG_TO_USER,										// ���û�����Ϣ ��Protocal�û�Э�����ݣ�


		///////////////////////////////////////////   ����   /////////////////////////////////////////
		MT_ADD_PUBLICSALE_ITEM,								// ����������Ʒ
		MT_REMOVE_PUBLICSALE_ITEM,							// ɾ��������Ʒ
		MT_BUY_PUBLICSALE_ITEM,								// ������Ʒ
		MT_PUBLICSALE_GET_ITEM_LIST,						// ��ȡ������Ʒ�б�
		MT_PUBLICSALE_SEND_ITEM_INFO_TO_WORLD,				// ����������Ʒ��Ϣ����������Ƶ��


		///////////////////////////////////////////   ����   /////////////////////////////////////////
		MT_SYNC_PERSON_RANK_INFO,							// ͬ�����а���Ϣ
		MT_SYSTEMCMD_RELOAD,								// ֪ͨgameworld�ض����ñ������
		MT_SYSTEMCMD_TO_WORLD_COUNT,						// ����ȫ����ɫ���ܴ�����͵������ȣ�
		MT_SYSTEMCMD_TO_WORLD_FB_COUNT,						// ����ȫ����ɫ��������
		MT_GAMEWORLD_SEND_MAIL_TO_USER,						// gamewaorld���û����ʼ�
		MT_SYNC_ROLE_JOIN_CAMP,								// ���û�������Ӫ
		MT_ACTIVITY_STATUS,									// �״̬�㲥
		MT_CHECK_SHOPLIMIT_INFO,							// ����̳ǹ����޹���Ʒ
		MT_ACTIVITY_FORCE_TO_NEXT_STATE,					// �л������һ״̬
		MT_EVALUATE_ROLE,									// �������
		MT_SYNC_BOSS_FLUSH_TIME,							// ͬ��BOSSˢ��ʱ��
		MT_ROLE_RESET_NAME,									// ��ɫ����
		MT_SYNC_TEAM_RANK_INFO,								// ͬ��������а���Ϣ
		MT_ADD_CHEST_SHOP_NEWS_ITEM,						// ���ӱ�����Ϣ
		MT_FORBID_SUBLOCK,									// ȫ�ֽ�ֹ������
		MT_FORCE_SET_MICRO_PC_FLAG,							// ȫ�����õ�½���Ƿ������
		MT_FORCE_SET_AUTO_FORBID,							// �Ƿ��Զ����
		MT_LINGMAI_INFO_CHANGE,								// ������Ϣ�ı�
		MT_ADD_MYSTERIOUS_SHOP_NEWS_ITEM,					// ���������̵���Ϣ
		MT_KILL_OTHER_CAMP,									// ��ɱ������Ӫ���
		MT_MINGRENTANG_EVENT,								// ����������¼�
		MT_ADD_CARD_LUCKY_NEWS_ITEM,						// ���ӿ��Ƴ齱���˹���
		MT_ADD_FB_XHT_PASS_NEWS_ITEM,						// �����ɻ�������ͨ�ؼ�¼
		MT_SYNC_WORLD_LEVEL_INFO,							// ͬ���������ȼ���Ϣ
		MT_SYNC_LAST_BATTLEFILED_RANK_INFO,					// gameworld->global ͬ��ս������
		MT_SYNC_ROLE_SET_PROF,								// ͬ����ɫѡ��ְҵ
		MT_GM_WEEK_CHANGE,									// gameworld->global WeekChange
		MT_GM_EVALUATE_ROLE,								// GM�������
		MT_ADD_1V1_NEWSITEM,								// 1v1����ս��
		MT_COMMON_ACTIVITY_GET_CAPABILITY_RANK_REQ,			// gameworld->global ȡ��ɫս�����а�����
		MT_COMMON_ACTIVITY_GET_CAPABILITY_RANK_ACK,			// global->gameworld ȡ��ɫս�����а����ݷ���
		MT_ADD_RANDACTIVITY_LUCKY_ROLL_NEWS_ITEM,			// �����������˳齱���˹���
		MT_DATING_INVITE_REQ,								// Լ������
		MT_DATING_INVITE_CHECK_TARGET_RET,					// Լ�������鱻�����˷���
		MT_DATING_INVITE_ACK,								// Լ������ظ�
		MT_DATING_GM_FORCE_INVITE,							// GMԼ��ǿ������
		MT_DATING_GIVE_PRESENT_ACK,							// Լ���������ﷵ��
		MT_DATING_DAYCOUNT_CHECK_ACK,						// Լ���ÿ�մ����ɹ�����
		MT_ADD_COMBINE_QINGDIAN_ROLL_NEWS_ITEM,				// �Ϸ����ת�����˹���

		///////////////////////////////////////////   P2P����   /////////////////////////////////////////
		MT_TRADE_AGREE,										// ˫��ͬ�� ֪ͨglobal���׿�ʼ
		MT_TRADE_ITEM_LIST,									// ������Ʒ�ϴ� ˫��������Ĳ���
		MT_TRADE_AFFIRM,									// ֪ͨglobal������ʽ���� ˫��ȷ�Ϻ�Ĳ���
		MT_TRADE_CONSUME_RET,								// �۳���Ʒ����
		MT_TRADE_OVER,										// ������ֹ֪ͨglobal
		MT_TRADE_SUCC_RET,									// �������׳ɹ�

		///////////////////////////////////////////   ����   /////////////////////////////////////////
		MT_CREATE_GUILD,									// ��������
		MT_DISMISS_GUILD,									// ��ɢ����
		MT_APPLY_FOR_JOIN_GUILD,							// �������
		MT_APPLY_FOR_JOIN_GUILD_ACK,						// �������ظ�  ���� 
		MT_QUIT_GUILD,										// �˳�����
		MT_INVITE_GUILD,									// �������
		MT_INVITE_GUILD_ACK,								// �������ظ�
		MT_KICKOUT_GUILD,									// ����
		MT_APPOINT_GUILD,									// ����
		MT_LEAVE_POST,										// ж��
		MT_GUILD_MAIL_ALL,									// Ⱥ���ʼ�
		MT_GUILD_CHANGE_NOTICE,								// �޸Ĺ���
		MT_ALL_GUILD_BASE_INFO,								// ��ȡ���о����б�
		MT_GET_APPLY_FOR_LIST,								// ��ȡ�����б�
		MT_GET_MEMBER_LIST,									// ��ȡ��Ա�б�
		MT_GET_GUILD_BASE_INFO,								// ��ȡ������ϸ��Ϣ 
		MT_GET_GUILD_EVENT_LIST,							// ��ȡ��־�б�
		MT_GET_APPLY_FOR_JOIN_GUILD_LIST,					// ��ȡ���������ľ����б�
		MT_GUILD_SET_STORAGESHOP_USE,						// �����̵�ʹ��Ȩ��
		MT_GUILD_DELATE,									// ����
		MT_GUILD_ADD_EXP,									// ���Ӿ��ž���ֵ
		MT_GUILD_JIANZHU_ADD_EXP_RET,						// ���ӽ������鷵��
		MT_GUILD_MEMBER_SOS,								// ���ų�Ա���
		MT_GET_GUILD_INVITE_LIST,							// ��ȡ���������б�
		MT_GET_GUILD_JIANZHU_INFO,							// ��ȡ���Ž�����Ϣ
		MT_GUILD_RESET_NAME,								// ���Ÿ���
		MT_GUILD_APPLYFOR_SETUP,							// ������������
		MT_GUILD_BOSS_GM_ADD_EXP,							// GM���Ӿ������޾���
		MT_SYNC_GUILD_TASK_INFO_TO_GLOBAL,					// ͬ������������Ϣ gameworld->global
		MT_GUILD_SET_AUTO_KICKOUT_SETUP,					// ���������Ƿ��Զ�����
		MT_GUILD_YUNBIAO_START,								// ���ڻ��ʼ�� global->start
		MT_GUILD_YUNBIAO_STATUS,							// ��������״̬

		///////////////////////////////////////////   ����BOSS   /////////////////////////////////////////
		MT_WORLDBOSS_REPORT,								// gameworld�ϱ���global

		///////////////////////////////////////////   �   /////////////////////////////////////////
		MT_SYNC_GUILD_RANK_INFO,							// ����������Ϣͬ��
		MT_SYNC_USER_GIVE_FLOWER_NUM,						// gameworld-->global ͬ������ͻ�����
		MT_SYNC_USER_CHEST_SHOP_BUY_COUNT,					// gameworld-->global ͬ�����Ѱ������
		MT_SYNC_RAND_ACTIVITY_ALLLIMITBUY_BUY_SUCC,			// gameworld-->global ����ȫ���޹�����ɹ�
		MT_GM_ACTIVITY_FIND_REQ,							// gameworld-->global GM��һ�����
		MT_COMBINE_SERVER_ACTIVITY_SYNC_STATE,				// global֪ͨgameworld--�Ϸ���ӻ״̬
		MT_COMBINE_SERVER_ACTIVITY_SYNC_VALUE,				// gameworld�ϱ���global--�Ϸ��������Ϣ

		///////////////////////////////////////////   ���   /////////////////////////////////////////
		MT_MARRY_RESERVE_ACK,								// ԤԼ��鷵��
		MT_GM_MARRY_RESERVE_REQ,							// Gm����ԤԼ���
		MT_MARRY_INFO,										// ͬ����ǰ������Ϣ
		MT_MARRYOBJ_SYNC_STATUS_TO_GLOBAL,					// ͬ��������״̬��global
		MT_MARRY_HUNYAN_INVITE,								// ���緢�����
		MT_DIVORCE_REQ,										// �������
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct MessageHeader
	{
		MessageHeader() : msg_type(MT_INVALID) {}
		MessageHeader(int type) : msg_type(type) {}

		int	msg_type;
	};

	static const int MAX_REGISTER_SCENE_LIST_NUM = 1024;

	struct GGlobalUserMsg
	{
		GGlobalUserMsg() : header(MT_USER_MSG) {}

		MessageHeader header;
		int db_index;
		int role_id;
		// ���������ggprotocalЭ���е�����
	};

	struct GGlobalMsgToUser
	{
		GGlobalMsgToUser() : header(MT_MSG_TO_USER) {}

		MessageHeader header;
		int db_index;
		int role_id;
		// ���������Protocal�û�Э������
	};


	struct GGlobalMsgToCrossServer
	{
		GGlobalMsgToCrossServer() : header(MT_MSG_TO_CROSS_SERVER) {}
		MessageHeader header;

		int cross_activity_type;
		// ���������crossgameprotocalЭ������
	};

	struct GGlobalCrossGameMsg
	{
		GGlobalCrossGameMsg() : header(MT_CROSS_GAME_MSG) {}
		MessageHeader header;

		// ���������crossgameprotocalЭ������
	};

	//////////////////////////////////////////////    ����    //////////////////////////////////////////////////
	struct AddPublicSaleItem
	{
		AddPublicSaleItem() : header(MT_ADD_PUBLICSALE_ITEM) {}
		MessageHeader header;

		int db_index;
		int	role_id;
		GameName role_name;
		short is_to_world;
		short camp;
		SaleItemParam item_param;
	};

	struct RemovePublicSaleItem
	{
		RemovePublicSaleItem() : header(MT_REMOVE_PUBLICSALE_ITEM) {}
		MessageHeader header;

		int db_index;
		int	role_id;
		int	sale_index;
	};

	struct BuyPublicSaleItem
	{
		BuyPublicSaleItem() : header(MT_BUY_PUBLICSALE_ITEM) {}
		MessageHeader header;

		int db_index;
		int	role_id;
		GameName role_name;

		int	item_id;
		int	item_num;

		int	sale_value;
		int	price;

		short sale_item_type;
		short price_type;

		int	seller_db_index;
		int	seller_role_id;
		int	sale_index;
	};

	struct PublicSaleGetItemList
	{
		PublicSaleGetItemList() : header(MT_PUBLICSALE_GET_ITEM_LIST) {}
		MessageHeader header;

		int db_index;
		int	role_id;
	};

	struct PublicSaleSendItemInfoToWorld
	{
		PublicSaleSendItemInfoToWorld() : header(MT_PUBLICSALE_SEND_ITEM_INFO_TO_WORLD) {}
		MessageHeader header;

		int db_index;
		int	role_id;
		short camp;
		short sale_index;
		GameName role_name;
	};

	//////////////////////////////////////////////    ����    //////////////////////////////////////////////////

	struct GameworldSendMailToUser
	{
		GameworldSendMailToUser() : header(MT_GAMEWORLD_SEND_MAIL_TO_USER) {}

		MessageHeader		header;

		int reason;		
		MailContentParam contentparam;

		struct UserIDInfo
		{
			int db_index;
			int role_id; 
		};

		int user_num;
		UserIDInfo user_list[MAX_GAMEWORLD_SEND_MAIL_USER];
	};

	struct GSystemCmdReload
	{
		GSystemCmdReload() : header(MT_SYSTEMCMD_RELOAD) {}
		MessageHeader		header;

		int					config_type;
		int					param1;
		int					param2;
	};

	struct SystemCmdToWorldCount
	{
		SystemCmdToWorldCount() : header(MT_SYSTEMCMD_TO_WORLD_COUNT) {}
		MessageHeader header;

		int count_type;
		int	count;
	};

	struct SystemCmdToWorldFbCount
	{
		SystemCmdToWorldFbCount() : header(MT_SYSTEMCMD_TO_WORLD_FB_COUNT) {}
		MessageHeader header;

		int day_count_id;
		int	count;
	};

	struct SyncRoleJoinCamp
	{
		SyncRoleJoinCamp() : header(MT_SYNC_ROLE_JOIN_CAMP) {}
		MessageHeader header;

		int camp_type;
	};

	struct SyncRoleSetProf
	{
		SyncRoleSetProf() : header(MT_SYNC_ROLE_SET_PROF) {}
		MessageHeader header;

		int prof_type;
	};

	struct GmWeekChange
	{
		GmWeekChange() : header(MT_GM_WEEK_CHANGE) {}
		MessageHeader header;
	};

	struct ActivityForceToNextState
	{
		ActivityForceToNextState() : header(MT_ACTIVITY_FORCE_TO_NEXT_STATE) {}
		MessageHeader header;

		int activity_type;
	};

	struct EvaluateRole
	{
		EvaluateRole() : header(MT_EVALUATE_ROLE) {}
		MessageHeader header;

		int				db_index;
		int				role_id;

		int				target_uid;
	};

	struct GMEvaluateRole
	{
		GMEvaluateRole() : header(MT_GM_EVALUATE_ROLE) {}
		MessageHeader header;

		int				db_index;
		int				role_id;

		int				target_uid;
		short			rank_type;
		short			is_admire;
		int				times;
	};

	struct SyncBossFlushTime
	{
		SyncBossFlushTime() : header(MT_SYNC_BOSS_FLUSH_TIME) {}
		MessageHeader header;

		int				monster_id;
		unsigned int	flush_time;
		int				is_together_broadcast;
	};

	struct SyncTeamRankInfo
	{
		SyncTeamRankInfo() : header(MT_SYNC_TEAM_RANK_INFO) {}

		MessageHeader		header;

		TeamRankMemberInfo member_list[MAX_TEAM_MEMBER_NUM];
		int rank_type;
		long long rank_value;
		int flexible_int;
		long long flexible_ll;
	};

	struct AddChestShopNewsItem
	{
		AddChestShopNewsItem() : header(MT_ADD_CHEST_SHOP_NEWS_ITEM) {}
		MessageHeader		header;

		int					chest_shop_type;
		ChestShopNewsItem	news_item;
	};

	struct AddMysteriousShopNewsItem
	{
		AddMysteriousShopNewsItem() : header(MT_ADD_MYSTERIOUS_SHOP_NEWS_ITEM) {}
		MessageHeader		header;

		MysteriousShopNewsItem news_item;
	};

	struct AddRandActivityLuckyRollNewsItem
	{
		AddRandActivityLuckyRollNewsItem() : header(MT_ADD_RANDACTIVITY_LUCKY_ROLL_NEWS_ITEM) {}
		MessageHeader		header;

		RandActivityLuckyRollNewsItem news_item;
	};

	struct AddCombineQingdianRollNewsItem
	{
		AddCombineQingdianRollNewsItem() : header(MT_ADD_COMBINE_QINGDIAN_ROLL_NEWS_ITEM) {}
		MessageHeader		header;
	};

	struct MingrentangAchieve
	{
		MingrentangAchieve() : header(MT_MINGRENTANG_EVENT) {}
		MessageHeader header;

		int mingrentang_id;
		UserID user_id;
	};

	struct AddFBXianhuntaPassNewsItem
	{
		AddFBXianhuntaPassNewsItem() : header(MT_ADD_FB_XHT_PASS_NEWS_ITEM) {}
		MessageHeader		header;

		short fb_type;									// ��������
		short pass_level;								// �ؿ�
		XianhuntaFBPassNewsItem news_item;
	};

	struct SyncWorldLevelInfo
	{
		SyncWorldLevelInfo() : header(MT_SYNC_WORLD_LEVEL_INFO) {}
		MessageHeader		header;

		int					world_level;				// ����ȼ�
		int					top_user_level;				// ��߽�ɫ�ȼ�
		int					capability_rank_mincap;		// ս�������ս��
	};

	struct SyncLastBattleFieldRankItem
	{
		UserID user_id;
		char vip;
		char sex;
		char prof;
		char camp;
		int level;
		GameName user_name;
		GameName flexible_name;
		long long rank_value;
		long long flexible_ll;
		int flexible_int;
		long long reserved;
	};

	struct SyncLastBattleFieldRankInfo
	{
		SyncLastBattleFieldRankInfo() : header(MT_SYNC_LAST_BATTLEFILED_RANK_INFO) {}

		MessageHeader		header;

		int rank_type;
		int rank_count;
		SyncLastBattleFieldRankItem rank_list[PERSON_RANK_MAX_SIZE];
	};

	//////////////////////////////////////////////    P2P����    //////////////////////////////////////////////////
	struct TradeItemList
	{
		TradeItemList() : header(MT_TRADE_ITEM_LIST) {}
		MessageHeader		header;

		int					uid;
		int					empty_grid_num;

		int					item_count;
		ItemDataWrapper		item_list[P2P_TRADE_MAX_ITEM_COUNT];

		int					other_uid;			// ������
	};

	struct TradeAgree
	{
		TradeAgree() : header(MT_TRADE_AGREE) {}
		MessageHeader		header;

		int					req_uid;
		int					recv_uid;
		GameName			req_name;
		GameName			recv_name;
	};

	struct TradeAffirm
	{
		TradeAffirm() : header(MT_TRADE_AFFIRM) {}
		MessageHeader		header;

		int					from_uid;
		GameName			from_name;
		int					other_uid;			// ������
		GameName			other_name;
		long long			trade_id;
	};

	struct TradeConsumeRet
	{
		TradeConsumeRet() : header(MT_TRADE_CONSUME_RET) {}
		MessageHeader		header;

		int					ret;				// 0Ϊ�ɹ� ����ʧ��
		int					from_uid;
		GameName			from_name;
		int					other_uid;			// ������
		GameName			other_name;
		long long			trade_id;
	};

	struct TradeOver
	{
		TradeOver() : header(MT_TRADE_OVER) {}
		MessageHeader		header;

		int					uid;	
		GameName			user_name;
		int					other_uid;			// ������
		GameName			other_name;
		long long			trade_id;
	};

	struct TradeSuccRet
	{
		TradeSuccRet() : header(MT_TRADE_SUCC_RET) {}
		MessageHeader		header;

		int					uid;
		GameName			user_name;
		int					other_uid;
		GameName			other_user_name;
		long long			trade_id;
	};

	//////////////////////////////////////////////    ����    //////////////////////////////////////////////////
	struct CreateGuild
	{
		CreateGuild() : header(MT_CREATE_GUILD) {}
		MessageHeader		header;

		int					db_index;
		int					role_id;

		GameName			role_name;
		PlatName			plat_name;
		GuildName			guild_name;
		GuildNotice			guild_notice;

		int					level;
		char				sex;
		char				prof;
		char				camp;
		int					vip_level;
		int					capability;

		int					create_guild_type;

		ItemDataWrapper		item_wrapper;
	};

	struct DismissGuild
	{
		DismissGuild() : header(MT_DISMISS_GUILD) {}
		MessageHeader		header;

		GuildID				guild_id;
	};

	struct ApplyForJoinGuild
	{
		ApplyForJoinGuild() : header(MT_APPLY_FOR_JOIN_GUILD) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
		GameName			role_name;
		PlatName			plat_name;

		int					level;
		char				sex;
		char				prof;
		char				camp;
		char				is_auto_join;
		int					capability;
		int					vip_level;
	};

	struct ApplyForJoinGuildAck
	{
		ApplyForJoinGuildAck() : header(MT_APPLY_FOR_JOIN_GUILD_ACK) {}
		MessageHeader		header;

		struct ApplyForAck
		{
			int				apply_for_db_index;
			int				apply_for_role_id;
		};

		GuildID				guild_id;
		int					result;
		int					db_index;
		int					role_id;
		int					count;
		ApplyForAck			apply_for[MAX_APPLY_FOR_COUNT];
	};

	struct QuitGuild
	{
		QuitGuild() : header(MT_QUIT_GUILD) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
	};

	struct InviteGuild
	{
		InviteGuild() : header(MT_INVITE_GUILD) {}
		MessageHeader		header;

		GuildID				guild_id;

		int					db_index;
		int					role_id;

		int					beinvite_db_index;
		int					beinvite_role_id;
	};

	struct InviteGuildAck
	{
		InviteGuildAck() : header(MT_INVITE_GUILD_ACK) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
		GameName			role_name;
		PlatName			plat_name;

		int					level;
		char				prof;
		char				camp;
		char				sex;
		char				reserve;
		int					vip_level;
		int					capability;

		int					invite_db_index;
		int					invite_role_id;

		int					result;
	};

	struct KickoutGuild
	{
		KickoutGuild() : header(MT_KICKOUT_GUILD) {}
		MessageHeader		header;

		struct KickoutGuildRoleInfo
		{
			int					bekicker_db_index;
			int					bekicker_role_id;
		};

		GuildID				guild_id;

		int					kicker_db_index;
		int					kicker_role_id;

		int					 bekicker_count;
		KickoutGuildRoleInfo bekicker_role_info_list[MAX_MEMBER_COUNT];
	};

	struct AppointGuild
	{	
		AppointGuild() : header(MT_APPOINT_GUILD) {}
		MessageHeader		header;

		GuildID				guild_id;

		int					db_index;
		int					role_id;

		int					beappoint_db_index;
		int					beappoint_role_id;

		int					post;
	};

	struct LeavePostGuild
	{	
		LeavePostGuild() : header(MT_LEAVE_POST) {}
		MessageHeader		header;

		GuildID				guild_id;

		int					db_index;
		int					role_id;
	};

	struct GuildMailAll
	{
		GuildMailAll() : header(MT_GUILD_MAIL_ALL) {}
		MessageHeader		header;

		int					db_index;
		int					role_id;

		GuildID				guild_id;

		MailSubject			subject;

		int					contenttxt_len;
		MailContentTxt		contenttxt;
	};

	struct GuildChangeNotice
	{
		GuildChangeNotice() : header(MT_GUILD_CHANGE_NOTICE) {}
		MessageHeader		header;

		int					db_index;
		int					role_id;

		GuildID				guild_id;
		GuildNotice         notice;
	};

	struct AllGuildBaseInfo
	{
		AllGuildBaseInfo() : header(MT_ALL_GUILD_BASE_INFO) {}
		MessageHeader		header;

		int					db_index;
		int					role_id;
	};

	struct GetApplyForList
	{
		GetApplyForList() : header(MT_GET_APPLY_FOR_LIST) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
	};

	struct GetApplyForJoinGuildList
	{
		GetApplyForJoinGuildList() : header(MT_GET_APPLY_FOR_JOIN_GUILD_LIST) {}
		MessageHeader		header;

		int					db_index;
		int					role_id;
	};

	struct GetMemberList
	{
		GetMemberList() : header(MT_GET_MEMBER_LIST) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
	};

	struct GetGuildBaselInfo
	{
		GetGuildBaselInfo() : header(MT_GET_GUILD_BASE_INFO) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
	};

	struct GetGuildEventList
	{
		GetGuildEventList() : header(MT_GET_GUILD_EVENT_LIST) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
	};

	struct GetGuildInviteList
	{
		GetGuildInviteList() : header(MT_GET_GUILD_INVITE_LIST) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
	};

	struct GetGuildJianzhuInfo
	{
		GetGuildJianzhuInfo() : header(MT_GET_GUILD_JIANZHU_INFO) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
	};

	struct GuildSetStorageShopUse
	{
		GuildSetStorageShopUse() : header(MT_GUILD_SET_STORAGESHOP_USE) {}
		MessageHeader header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;

		int					storage_use_limit;
		int					shop_use_limit;
	};

	struct GuildDelate
	{
		GuildDelate() : header(MT_GUILD_DELATE) {}
		MessageHeader header;

		GuildID				guild_id;
		int					knapsack_index;
	};

	struct GuildSetAutoKickoutSetup
	{
		GuildSetAutoKickoutSetup() : header(MT_GUILD_SET_AUTO_KICKOUT_SETUP) {}
		MessageHeader		header;

		GuildID				guild_id;
		int					auto_kickout_setup;
	};

	struct GuildAddExp
	{
		GuildAddExp() : header(MT_GUILD_ADD_EXP) {}
		MessageHeader header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;

		int					use_type;
		int					value;

		int					add_exp;
	};

	struct GuildJianZhuAddExpRet
	{
		GuildJianZhuAddExpRet() : header(MT_GUILD_JIANZHU_ADD_EXP_RET) {}
		MessageHeader header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;

		int					jianzhu_type;
		int					use_gold;
		int					value;
		int					add_gongxian;
		int					add_caifu;
		int					add_exp;
	};

	enum SYNC_GUILD_INFO_OPERATE_TYPE
	{
		SYNC_GUILD_INFO_OPERATE_TYPE_INIT = 0,	// ��ʼ��
		SYNC_GUILD_INFO_OPERATE_TYPE_CREATE,	// ��������
		SYNC_GUILD_INFO_OPERATE_TYPE_REMOVE,	// ��ɢ����
		SYNC_GUILD_INFO_OPERATE_TYPE_UPDATE,	// ���¾���
	};

	struct GmAddGuildBossExp
	{
		GmAddGuildBossExp() : header(MT_GUILD_BOSS_GM_ADD_EXP) {}
		MessageHeader header;

		int guild_id;
		int uid;
		int add_exp;
	};

	struct SyncGuildTaskInfoToGlobal
	{
		SyncGuildTaskInfoToGlobal() : header(MT_SYNC_GUILD_TASK_INFO_TO_GLOBAL) {}
		MessageHeader header;

		int role_id;
		int accepted_task_count;
		int add_score;
		int refresh_color;
	};

	struct ForbidSublock
	{
		ForbidSublock() : header(MT_FORBID_SUBLOCK) {}
		MessageHeader header;
	};

	struct ForceSetMicroPcFlag
	{
		ForceSetMicroPcFlag() : header(MT_FORCE_SET_MICRO_PC_FLAG) {}
		MessageHeader header;

		int is_micro_pc_open;
	};

	struct ForceSetAutoForbid
	{
		ForceSetAutoForbid() : header(MT_FORCE_SET_AUTO_FORBID) {}
		MessageHeader header;

		int is_auto_forbid;
	};

	struct GGlobalRoleResetName
	{
		GGlobalRoleResetName():header(MT_ROLE_RESET_NAME){}
		MessageHeader	header;

		UserID			user_id;
		GameName		name;
	};

	struct GuildMemberSos
	{
		GuildMemberSos() : header(MT_GUILD_MEMBER_SOS) {}
		MessageHeader		header;

		GuildID				guild_id;

		int					db_index;
		int					role_id;

		int					sos_type;
		int					scene_id;
		short				pos_x;
		short				pos_y;
		int					enemy_uid;
	};

	struct GuildResetName
	{
		GuildResetName() : header(MT_GUILD_RESET_NAME) {}
		MessageHeader header;

		GuildID				guild_id;
		int					db_index;
		int					role_id;
		GuildName			new_name;
	};

	struct GuildApplyforSetup
	{
		GuildApplyforSetup() : header(MT_GUILD_APPLYFOR_SETUP) {}
		MessageHeader header;

		GuildID				guild_id;
		int					applyfor_setup;
		int					need_capability;
		int					need_level;
	};

	struct KillOtherCamp
	{
		KillOtherCamp() : header(MT_KILL_OTHER_CAMP) {}
		MessageHeader header;

		UserID killer_user_id;
		UserID bekiller_user_id;
		int delta_level;
	};

	struct GuildYunBiaoStart
	{
		GuildYunBiaoStart() : header(MT_GUILD_YUNBIAO_START) {}
		MessageHeader header;

		GuildID guild_id;
		int guild_level;
	};

	enum YUNBIAO_STATUS
	{
		YUNBIAO_STATUS_INVALID = 0,
		YUNBIAO_STATUS_START,						// ��ʼ����
		YUNBIAO_STATUS_COMPLETE,					// �ɹ���������
		YUNBIAO_STATUS_FAIL,						// ����ʧ��
		YUNBIAO_STATUS_UPDATE,						// �����ڳ���Ϣ
		YUNBIAO_STATUS_BE_HURT,						// �ڳ�������
		YUNBIAO_STATUS_ACTIVITY_END_FAIL,			// ���������ʧ��
		YUNBIAO_STATUS_MAX,
	};

	struct GuildYunBiaoStatus
	{
		GuildYunBiaoStatus() : header(MT_GUILD_YUNBIAO_STATUS) {}
		MessageHeader header;

		int status;
		GuildID guild_id;
		int guild_lv;

		GuildID rob_guild_id;
		int biaoche_pos_x;
		int biaoche_pos_y;
	};

	//////////////////////////////////////////////    ����BOSS    //////////////////////////////////////////////////
	struct ReportWorldBossToGlobal
	{
		ReportWorldBossToGlobal() : header(MT_WORLDBOSS_REPORT) {}
		MessageHeader header;

		enum ENEVENTTYPE
		{
			ENEVENTTYPE_BORN = 1,						// boss����
			ENEVENTTYPE_DIED,							// boss����
		};

		int event_type;
		int boss_id;

		int killer_uid;
		GameName killer_name;
	};

	//////////////////////////////////////////////    �    //////////////////////////////////////////////////
	struct SyncGuildRankInfo
	{
	public:
		SyncGuildRankInfo() : header(MT_SYNC_GUILD_RANK_INFO) {}

		MessageHeader header;

		static const int MAX_GUILD_RANK_NUM = 100;

		struct GuildRank
		{
			GuildID guild_id;
			long long rank_value;
		};

		int guild_rank_type;
		int guild_rank_count;
		GuildRank guild_rank_list[MAX_GUILD_RANK_NUM];
	};

	struct SyncUserGiveFlowerNum
	{
	public:
		SyncUserGiveFlowerNum() : header(MT_SYNC_USER_GIVE_FLOWER_NUM) {}
		MessageHeader header;

		int give_flower_num;
	};

	struct SyncUserChestShopBuyCount
	{
	public:
		SyncUserChestShopBuyCount() : header(MT_SYNC_USER_CHEST_SHOP_BUY_COUNT) {}
		MessageHeader header;

		int chest_shop_buy_count;
	};

	struct SyncRandActivityAllLimitBuyBuySucc
	{
	public:
		SyncRandActivityAllLimitBuyBuySucc() : header(MT_SYNC_RAND_ACTIVITY_ALLLIMITBUY_BUY_SUCC) {}
		MessageHeader header;

		short activity_type;
		short reserve;
	};

	struct MarryReserveAck
	{
		MarryReserveAck() : header(MT_MARRY_RESERVE_ACK) {}
		MessageHeader header;

		int uid_list[MARRY_COUPLE_USER_LIMIT_NUM];
		GameName user_name_list[MARRY_COUPLE_USER_LIMIT_NUM];
		char camp_list[MARRY_COUPLE_USER_LIMIT_NUM];
		short reserve_sh;
		short marry_level;
		short marry_reserve_index;
	};

	struct GmMarryReserveReq
	{
		GmMarryReserveReq() : header(MT_GM_MARRY_RESERVE_REQ) {}
		MessageHeader header;

		int uid;
		short reserve_index;
		short marry_level;
		short is_clear;
		short is_force_start;
		int is_force_next_status;
	};

	struct MarryInfo
	{
		MarryInfo() : header(MT_MARRY_INFO) {}
		MessageHeader header;

		SyncMarryInfo marry_info;
	};

	struct MarryobjSyncPosToGlobal
	{
		MarryobjSyncPosToGlobal() : header(MT_MARRYOBJ_SYNC_STATUS_TO_GLOBAL) {}
		MessageHeader		header;	

		short				pos_x;
		short				pos_y;
	};

	struct MarryHunyanInvite
	{
		MarryHunyanInvite() : header(MT_MARRY_HUNYAN_INVITE) {}
		MessageHeader		header;

		int					invite_uid;
		int					be_invite_uid;
		int					is_free_invite;
	};

	struct DivorceReq
	{
		DivorceReq() : header(MT_DIVORCE_REQ) {}
		MessageHeader		header;

		int					other_uid;
		GameName			other_name;
		int					other_sex;

		unsigned int		divorce_time;
		int					clear_intimacy;
	};

	struct DatingInviteReq
	{
		DatingInviteReq() : header(MT_DATING_INVITE_REQ) {}
		MessageHeader		header;

		int					req_uid;
		int					target_uid;
		int					is_specific_invite;
	};

	struct DatingInviteCheckTargetRet
	{
		DatingInviteCheckTargetRet() : header(MT_DATING_INVITE_CHECK_TARGET_RET) {}
		MessageHeader		header;

		int					req_uid;
		int					target_uid;
		int					is_specific_invite;
		int					check_result;
	};

	struct DatingInviteAck
	{
		DatingInviteAck() : header(MT_DATING_INVITE_ACK) {}
		MessageHeader		header;

		int					req_uid;
		int					target_uid;
		int					is_agree;
	};

	struct DatingGivePresentAck
	{
		DatingGivePresentAck() : header(MT_DATING_GIVE_PRESENT_ACK) {}
		MessageHeader		header;

		int					req_uid;
		int					present_type;
	};

	struct GMDatingForceInvite
	{
		GMDatingForceInvite() : header(MT_DATING_GM_FORCE_INVITE) {} 
		MessageHeader		header;

		int					self_uid;
		int					target_uid;
	};

	struct DatingDayconutCheckAck
	{
		DatingDayconutCheckAck() : header(MT_DATING_DAYCOUNT_CHECK_ACK) {}
		MessageHeader		header;

		int					uid;
		int					can_consume_daycount;
	};

	struct CommonActivityGetCapabilityRankReq
	{
		CommonActivityGetCapabilityRankReq() : header(MT_COMMON_ACTIVITY_GET_CAPABILITY_RANK_REQ) {}
		MessageHeader		header;

		int req_count;
	};

	struct CommonActivityCapabilityRankItem
	{
		CommonActivityCapabilityRankItem() : role_id(0), camp(0), reserved(0), reserved1(0), capability(0) 
		{
			memset(name, 0, sizeof(name));
		}

		int role_id;
		GameName name;
		char camp;
		char reserved;
		short reserved1;
		int capability;
	};

	struct CommonActivityGetCapabilityRankAck
	{
		CommonActivityGetCapabilityRankAck() : header(MT_COMMON_ACTIVITY_GET_CAPABILITY_RANK_ACK) {}
		MessageHeader		header;

		int count;
		CommonActivityCapabilityRankItem item_list[PERSON_RANK_MAX_SIZE];
	};

	struct GmActivityFindReq
	{
		GmActivityFindReq() : header(MT_GM_ACTIVITY_FIND_REQ) {}
		MessageHeader header;

		short is_find_reward;						// 0�������һ���Ϣ 1�������һػ����
		short activity_find_type;
		int self_uid;
	};

	struct CombineServerActivitySyncState
	{
	public:
		CombineServerActivitySyncState() : header(MT_COMBINE_SERVER_ACTIVITY_SYNC_STATE) {}
		MessageHeader header;

		char sub_activity_state_list[CSA_SUB_TYPE_MAX];
	};

	struct CombineServerActivitySyncValue
	{
	public:
		CombineServerActivitySyncValue() : header(MT_COMBINE_SERVER_ACTIVITY_SYNC_VALUE) {}
		MessageHeader header;

		int rank_type;
		int role_id;
		long long value;
	};
}


struct SyncPersonRankInfoItem
{
	int 			rank_type;
	long long		rank_value;
	int				record_index;

	GameName		flexible_name;
	int				flexible_int;
	long long		flexible_ll;
};

static const int SYNC_PERSON_RANK_INFO_SIZE = 1024;

struct SyncPersonRankInfo
{
	SyncPersonRankInfo() : header(gglobalprotocal::MT_SYNC_PERSON_RANK_INFO) {}

	gglobalprotocal::MessageHeader		header;

	int					db_index;
	int					role_id;
	long long           exp;
	int                 level;
	GameName			user_name;
	GuildName			guild_name;
	char				reserve_ch;
	char				sex;
	char				prof;
	char				camp;
	char				vip_level;
	char				reserved2;
	short				jingjie_level;

	void AddItem(int rank_type, long long rank_value, int record_index, GameName flexible_name, int flexible_int, long long flexible_ll)
	{
		if (item_count < 0 || item_count >= SYNC_PERSON_RANK_INFO_SIZE)
		{
			return;
		}

		SyncPersonRankInfoItem *item = &items[item_count];
		item->rank_type = rank_type;
		item->rank_value = rank_value;
		item->record_index = record_index;
		memcpy(item->flexible_name, flexible_name, sizeof(item->flexible_name));
		item->flexible_int = flexible_int;
		item->flexible_ll = flexible_ll;

		++ item_count;
	}

	void ResetItems()
	{
		item_count = 0;
		memset(items, 0, sizeof(items));
	}

	int item_count;
	SyncPersonRankInfoItem items[SYNC_PERSON_RANK_INFO_SIZE];
};

////////////////////////////////////////////////////////////////// �������� //////////////////////////////////////////////////////////
struct SyncCoupleRankItem
{
	int rank_type;
	int male_rank_value;
	int female_rank_value;
	int flexible_int;
	long long flexible_ll;
};

struct SyncCoupleRankInfo
{
	int male_uid;
	int female_uid;
	GameName male_name;
	GameName female_name;
	long long male_avatar;
	long long female_avatar;
	char male_prof;
	char female_prof;
	short reserve_sh;

	int item_count;
	SyncCoupleRankItem rank_item_list[COUPLE_RANK_TYPE_MAX];
};

#pragma pack(pop)

#endif  // __GGLOBALPROTOCAL_H__

