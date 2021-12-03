#ifndef MSGFRIEND_H
#define MSGFRIEND_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/serverdef.h"
#include "servercommon/frienddef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	//--------------------- ������� ---------------------//

	class CSFriendInfoReq									// 2150�ͻ��˻�ȡ�����б�����
	{
	public:
		CSFriendInfoReq();
		MessageHeader		header;
	};

	struct FriendInfo
	{
		int					user_id;
		GameName			gamename;
		int					intimacy;
		char				camp;
		char				sex;
		char				prof;
		char				is_online;
		GameName			group_name;
		short				level;
		short				reserve_sh;
		int					capability;
		long long			avatar_timestamp;
		unsigned int		last_logout_timestamp;
		int					gift_count;
		int					favorable_impression;
		int					day_chat_count;
		unsigned int		add_timestamp;			// ��Ӻ���ʱ��
	};

	class SCFriendInfoAck									//  2100���������ͺ����б�
	{
	public:
		SCFriendInfoAck();
		MessageHeader		header;

		int is_auto_dec_friend;
		int					count;
		FriendInfo			friend_list[MAX_FRIEND_NUM];
	};

	class CSAddFriendReq									// 2251�ͻ�����ӼӺ�������
	{
	public:
		CSAddFriendReq();
		MessageHeader		header;

		int 				friend_user_id;
		int					is_yi_jian;
	};

	class SCAddFriendRoute									// 2101������ѯ�ʱ���Ӻ��ѵĶ���								
	{
	public:
		SCAddFriendRoute();
		MessageHeader		header;

		GameName			req_gamename;
		int 				req_user_id;
		char				req_avatar;
		char				req_sex;
		char				req_prof;
		char				req_camp;
		int					req_level;
		long long			req_avatar_timestamp;
		int					req_total_capability;
	};

	class CSAddFriendRet									// 2152�ͻ��˴����Ƿ���ܼӺ�������
	{
	public:
		CSAddFriendRet();
		MessageHeader		header;

		int					req_user_id;
		GameName			req_gamename;
		char				is_accept;
		char				reserved;
		char				req_sex;
		char				req_prof;
	};

	enum CHANGE_FRIEND_STATE
	{
		FRIEND_UPDATE = 0,
		FRIEND_DELETE
	};

	class SCChangeFriend									// 2102��������ͻ��˷��ͺ�����Ϣ�ı�֪ͨ
	{
	public:
		SCChangeFriend();
		MessageHeader		header;

		int					user_id;
		GameName			gamename;
		int					intimacy;
		char				reserved;
		char				sex;
		char				changestate;
		char				prof;
		char				camp;
		char				is_online;
		short				level;
		int					capability;
		long long			avatar_timestamp;
		unsigned int		last_logout_timestamp;
		int					gift_count;
		int					favorable_impression;
		int					day_chat_count;
		unsigned int		add_timestamp;			// ��Ӻ���ʱ��
	};

	class CSDeleteFriend									// 2153�ͻ���ɾ����������							
	{
	public:
		CSDeleteFriend();
		MessageHeader		header;

		int					user_id;
		int					is_silence;
	};

	class SCAddFriendRet									// 2103��������ͻ��˷�����Ӻ���������
	{
	public:
		SCAddFriendRet();
		MessageHeader		header;

		GameName gamename;
		int is_accept;       // 0��ͬ�⣬1ͬ��
	};

	//--------------------- ��������� ---------------------//

	enum CHANGE_BLACK_STATE
	{
		BLACK_UPDATE = 0,
		BLACK_DELETE
	};

	struct BlacklistsItem
	{
		int					user_id;
		GameName			gamename;
		char				sex;
		char				prof;
		short				level;
		long long avatar_timestamp;
	};

	class SCChangeBlacklist									// 2161������֪ͨ�ͻ��˺������ı�
	{
	public:
		SCChangeBlacklist();
		MessageHeader		header;

		int					index;
		int					user_id;
		GameName			gamename;
		char				changstate;
		char				sex;
		char				prof;
		char				reserved;
		int					level;
		long long			avatar_timestamp;
	};

	class SCBlacklistsACK									// 2158��������ͻ��˷��غ�����
	{
	public:
		SCBlacklistsACK();
		MessageHeader		header;

		int					count;
		BlacklistsItem		blacklists[MAX_BLACK_NUM];
	};

	class CSAddBlackReq										// 2159�ͻ����������һ���������û�
	{
	public:
		CSAddBlackReq();
		MessageHeader		header;

		int					black_id;
		int					reserved;
	};

	class CSDeleteBlackReq									// 2160����ɾ��һ���������û�
	{
	public:
		CSDeleteBlackReq();
		MessageHeader		header;

		int					black_id;
	};


	//--------------------- ������� ---------------------//

	enum CHANGE_ENEMY_STATE
	{
		ENEMY_UPDATE = 0,
		ENEMY_DELETE
	};

// 	struct EnemyItem
// 	{
// 		GameName			gamename;
// 		long long			last_kill_time;
// 		long long			unique_user_id;
// 		int					kill_count;
// 		int					be_kill_count;
// 		int					index;
// 		char				camp;
// 		char				sex;
// 		char				prof;
// 		char				is_online;
// 		short				level;
// 		short				reserve_sh;
// 		int					capability;
// 		long long			avatar_timestamp;
// 	};
//
// 	class SCChangeEnemy										// 2105������֪ͨ�ͻ��˳��˸ı�
// 	{
// 	public:
// 		SCChangeEnemy();
// 		MessageHeader		header;
// 
// 		int					index;
// 		long long			unique_user_id;
// 		GameName			gamename;
// 		long long			last_kill_time;
// 		int					kill_count;
// 		int					be_kill_count;
// 		char				camp;
// 		char				sex;
// 		char				changstate;
// 		char				prof;
// 		short				level;
// 		char				is_online;
// 		char				reserve_ch;
// 		int					capability;
// 		long long			avatar_timestamp;
// 	};
	struct EnemyItem
	{
		long long			last_kill_time;
		long long			unique_user_id;
		int					kill_count;
		int					be_kill_count;
		int					index;
	};

	class SCChangeEnemy										// 2105������֪ͨ�ͻ��˳��˸ı�
	{
	public:
		SCChangeEnemy();
		MessageHeader		header;

		int					index;
		long long			unique_user_id;
		long long			last_kill_time;
		int					kill_count;
		int					be_kill_count;
		short				changstate;
		short				reserve_sh;
	};

	class SCEnemyListACK									// 2104���������ͳ����б�
	{
	public:
		SCEnemyListACK();
		MessageHeader		header;

		int					count;
		EnemyItem			enemy_list[MAX_ENEMY_NUM];
	};

	class CSEnemyDelete										// 2154����ɾ������
	{
	public:
		CSEnemyDelete();
		MessageHeader		header;

		long long			unique_user_id;
	};

	//--------------------- �ͻ�������� ---------------------//

	class CSGiveFlower										// 5155�ͻ�
	{
	public:
		CSGiveFlower();
		MessageHeader	header;

		short			grid_index;
		ItemID			item_id;
		int				target_uid;
		short			is_anonymity;
		short			is_marry;	
	};

	class CSGiveFlowerKissReq							// 2156�ͻ���������
	{
	public:
		CSGiveFlowerKissReq();
		MessageHeader	header;

		int				target_uid;
	};

	class SCGiveFlowerKissAck							// 2106�ͻ����Ƿ���
	{
	public:
		SCGiveFlowerKissAck();
		MessageHeader	header;

		int				from_uid;
		int				from_camp;
		GameName		from_name;
	};

	class SCGiveFlower									// 2107�ͻ�֪ͨ
	{
	public:
		SCGiveFlower();
		MessageHeader		header;

		int					target_uid;
		int					from_uid;
		int					flower_num;
		int					is_anonymity;
		GameName			target_name;
		GameName			from_name;
		ItemID				item_id;
		short				reserve;
	};

	class SCSoneHuaInfo		// 2112
	{
	public:
		SCSoneHuaInfo();
		MessageHeader	header;

		int				daily_use_free_song_hua_times;
	};
	
	class CSSoneHuaInfoReq	// 2113
	{
	public:
		CSSoneHuaInfoReq();
		MessageHeader	 header;
	};

	class SCAllCharmChange								// 2108����ֵ�ı�
	{
	public:
		SCAllCharmChange();
		MessageHeader		header;

		int uid;
		int all_charm;
		int day_charm;
	};

	enum FRIEND_BLESS_OPERA_TYPE
	{
		FRIEND_BLESS_OPERA_TYPE_INVALID = 0,
		FRIEND_BLESS_OPERA_TYPE_QUERY = 1,					// ����ף����Ϣ����
		FRIEND_BLESS_OPERA_TYPE_BLESS = 2,					// ����ף��ף������
		FRIEND_BLESS_OPERA_TYPE_FETCH_REWARD = 3,			// ����ף����ȡ��������
		FRIEND_BLESS_OPERA_TYPE_INVITE_BLESS = 4,			// �������ף����

		FRIEND_BLESS_OPERA_TYPE_MAX,		
	};

	class CSFriendBlessOperaReq								// 2157����ף����������
	{
	public:
		CSFriendBlessOperaReq();
		MessageHeader		header;

		int opera_type;
		int param;
	};

	struct FriendBlessStatusItem
	{
		int uid;										// ����uid
		char has_bless;									// ���Ƿ�ף������
		char bless_me;									// ���Ƿ�ף������
		char has_fetch_reward;							// ���Ƿ���ȡ��ף������
		char reserve_ch;
	};

	class SCFriendBlessAllInfo								// 2109����ף����Ϣ
	{
	public:
		SCFriendBlessAllInfo();
		MessageHeader		header;

		int fetch_reward_times;								// ��ȡ��������
		int bless_times;									// ף������
		int bless_item_count;								// ף����Ϣ����
		FriendBlessStatusItem bless_item_list[MAX_FRIEND_NUM]; // ף����Ϣ
	};

	class SCFriendBlessInviteBless							// 2111�������ף����
	{
	public:
		SCFriendBlessInviteBless();
		MessageHeader		header;

		int invite_uid;										// ������uid
		GameName invite_name;								// ����������
	};

	enum FRIEND_BLESS_NOTIFY_REASON
	{
		FRIEND_BLESS_NOTIFY_REASON_DEFAULT = 0,
		FRIEND_BLESS_NOTIFY_REASON_REMOVE_STATUS = 1,		// ɾ��ף��״̬
		FRIEND_BLESS_NOTIFY_REASON_UPDATE_STATUS = 2,		// ����ף��״̬
		FRIEND_BLESS_NOTIFY_REASON_UPDATE_REWARD_TIMES = 3,	// �����콱����
	
		FRIEND_BLESS_NOTIFY_REASON_MAX,
	};

	class SCFriendBlessChangeInfo							// 2110����ף�������Ϣ						
	{
	public:
		SCFriendBlessChangeInfo();
		MessageHeader		header;

		int notify_reason;									// ֪ͨԭ��
		int fetch_reward_times;								// ��ȡ��������
		int bless_times;									// ף������
		FriendBlessStatusItem bless_item;					// ף��״̬��Ϣ
	};

	class CSOfflineFriendAutoDecFlag						// 2114 �Ƿ��Զ�ɾ������x����ѵ�			
	{
	public:
		CSOfflineFriendAutoDecFlag();
		MessageHeader		header;

		int is_auto_dec;
	};
}

#pragma pack(pop)

#endif
