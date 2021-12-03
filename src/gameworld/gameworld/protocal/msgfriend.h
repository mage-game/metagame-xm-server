#ifndef MSGFRIEND_H
#define MSGFRIEND_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/serverdef.h"
#include "servercommon/frienddef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	//--------------------- 好友相关 ---------------------//

	class CSFriendInfoReq									// 2150客户端获取好友列表请求
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
		unsigned int		add_timestamp;			// 添加好友时间
	};

	class SCFriendInfoAck									//  2100服务器发送好友列表
	{
	public:
		SCFriendInfoAck();
		MessageHeader		header;

		int is_auto_dec_friend;
		int					count;
		FriendInfo			friend_list[MAX_FRIEND_NUM];
	};

	class CSAddFriendReq									// 2251客户端添加加好友请求
	{
	public:
		CSAddFriendReq();
		MessageHeader		header;

		int 				friend_user_id;
		int					is_yi_jian;
	};

	class SCAddFriendRoute									// 2101服务器询问被添加好友的对象								
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

	class CSAddFriendRet									// 2152客户端处理是否接受加好友请求
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

	class SCChangeFriend									// 2102服务器向客户端发送好友信息改变通知
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
		unsigned int		add_timestamp;			// 添加好友时间
	};

	class CSDeleteFriend									// 2153客户端删除好友请求							
	{
	public:
		CSDeleteFriend();
		MessageHeader		header;

		int					user_id;
		int					is_silence;
	};

	class SCAddFriendRet									// 2103服务器向客户端发送添加好友请求结果
	{
	public:
		SCAddFriendRet();
		MessageHeader		header;

		GameName gamename;
		int is_accept;       // 0不同意，1同意
	};

	//--------------------- 黑名单相关 ---------------------//

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

	class SCChangeBlacklist									// 2161服务器通知客户端黑名单改变
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

	class SCBlacklistsACK									// 2158服务器向客户端返回黑名单
	{
	public:
		SCBlacklistsACK();
		MessageHeader		header;

		int					count;
		BlacklistsItem		blacklists[MAX_BLACK_NUM];
	};

	class CSAddBlackReq										// 2159客户端请求添加一个黑名单用户
	{
	public:
		CSAddBlackReq();
		MessageHeader		header;

		int					black_id;
		int					reserved;
	};

	class CSDeleteBlackReq									// 2160请求删除一个黑名单用户
	{
	public:
		CSDeleteBlackReq();
		MessageHeader		header;

		int					black_id;
	};


	//--------------------- 仇人相关 ---------------------//

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
// 	class SCChangeEnemy										// 2105服务器通知客户端仇人改变
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

	class SCChangeEnemy										// 2105服务器通知客户端仇人改变
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

	class SCEnemyListACK									// 2104服务器发送仇人列表
	{
	public:
		SCEnemyListACK();
		MessageHeader		header;

		int					count;
		EnemyItem			enemy_list[MAX_ENEMY_NUM];
	};

	class CSEnemyDelete										// 2154请求删除仇人
	{
	public:
		CSEnemyDelete();
		MessageHeader		header;

		long long			unique_user_id;
	};

	//--------------------- 送花功能相关 ---------------------//

	class CSGiveFlower										// 5155送花
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

	class CSGiveFlowerKissReq							// 2156送花回吻请求
	{
	public:
		CSGiveFlowerKissReq();
		MessageHeader	header;

		int				target_uid;
	};

	class SCGiveFlowerKissAck							// 2106送花回吻返回
	{
	public:
		SCGiveFlowerKissAck();
		MessageHeader	header;

		int				from_uid;
		int				from_camp;
		GameName		from_name;
	};

	class SCGiveFlower									// 2107送花通知
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

	class SCAllCharmChange								// 2108魅力值改变
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
		FRIEND_BLESS_OPERA_TYPE_QUERY = 1,					// 好友祝福信息请求
		FRIEND_BLESS_OPERA_TYPE_BLESS = 2,					// 好友祝福祝福请求
		FRIEND_BLESS_OPERA_TYPE_FETCH_REWARD = 3,			// 好友祝福领取奖励请求
		FRIEND_BLESS_OPERA_TYPE_INVITE_BLESS = 4,			// 邀请好友祝福我

		FRIEND_BLESS_OPERA_TYPE_MAX,		
	};

	class CSFriendBlessOperaReq								// 2157好友祝福操作请求
	{
	public:
		CSFriendBlessOperaReq();
		MessageHeader		header;

		int opera_type;
		int param;
	};

	struct FriendBlessStatusItem
	{
		int uid;										// 好友uid
		char has_bless;									// 我是否祝福过他
		char bless_me;									// 他是否祝福过我
		char has_fetch_reward;							// 我是否领取了祝福奖励
		char reserve_ch;
	};

	class SCFriendBlessAllInfo								// 2109好友祝福信息
	{
	public:
		SCFriendBlessAllInfo();
		MessageHeader		header;

		int fetch_reward_times;								// 领取奖励次数
		int bless_times;									// 祝福次数
		int bless_item_count;								// 祝福信息个数
		FriendBlessStatusItem bless_item_list[MAX_FRIEND_NUM]; // 祝福信息
	};

	class SCFriendBlessInviteBless							// 2111邀请好友祝福我
	{
	public:
		SCFriendBlessInviteBless();
		MessageHeader		header;

		int invite_uid;										// 邀请者uid
		GameName invite_name;								// 邀请者名字
	};

	enum FRIEND_BLESS_NOTIFY_REASON
	{
		FRIEND_BLESS_NOTIFY_REASON_DEFAULT = 0,
		FRIEND_BLESS_NOTIFY_REASON_REMOVE_STATUS = 1,		// 删除祝福状态
		FRIEND_BLESS_NOTIFY_REASON_UPDATE_STATUS = 2,		// 更新祝福状态
		FRIEND_BLESS_NOTIFY_REASON_UPDATE_REWARD_TIMES = 3,	// 更新领奖数量
	
		FRIEND_BLESS_NOTIFY_REASON_MAX,
	};

	class SCFriendBlessChangeInfo							// 2110好友祝福变更信息						
	{
	public:
		SCFriendBlessChangeInfo();
		MessageHeader		header;

		int notify_reason;									// 通知原因
		int fetch_reward_times;								// 领取奖励次数
		int bless_times;									// 祝福次数
		FriendBlessStatusItem bless_item;					// 祝福状态信息
	};

	class CSOfflineFriendAutoDecFlag						// 2114 是否自动删除离线x天好友的			
	{
	public:
		CSOfflineFriendAutoDecFlag();
		MessageHeader		header;

		int is_auto_dec;
	};
}

#pragma pack(pop)

#endif
