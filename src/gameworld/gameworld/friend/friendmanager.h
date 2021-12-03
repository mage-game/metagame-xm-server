#ifndef FRIENDMANAGER_H
#define FRIENDMANAGER_H

#include <map>

#include "gamedef.h"
#include "servercommon/struct/friendparam.h"
#include "servercommon/struct/enemyparam.h"
#include "servercommon/struct/blacklistsparam.h"

#include "servercommon/maildef.hpp"
#include "servercommon/internalprotocal/ggprotocal.h"

class Role;
class SceneManager;

class FriendManager
{
public:
	FriendManager();
	~FriendManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role * role, const FriendListParam &friends, const BlacklistsParam & blacks, const EnemyListParam &enemys, 
		int all_give_flower_num, int day_give_flower_num, int all_charm, int week_add_charm, int day_add_charm);
	void GetInitParam(int *all_give_flower_num, int *day_give_flower_num, int *all_charm);

	void OnWeekChange();
	void Update(UInt32 now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	bool AddFriend(UserID user_id, const char *reason);
	bool RemoveFriend(UserID user_id, const char *reason);
	
	bool AddEnemy(UniqueUserID unique_user_id, bool is_be_kill, const char *reason);
	bool RemoveEnemy(UserID user_id, int plat_type, const char *reason);
	bool AddBlack(const UserID &user_id, const GameName gamename, char sex, char prof, const char *reason);
	bool RemoveBlack(const UserID &user_id, const char *reason);

	bool IsEnemy(UniqueUserID unique_userid);
	bool IsFriend(UserID user_id, int *index = NULL);
	bool IsFriendFull();
	bool IsBlack(const UserID &user_id);

	void SendFriendListToRole();
	void SendEnemyListToRole();
	void SendBlackListToRole() const;

	void DecOfflinkLongTimeFriend();
	void GetFriendList(FriendListParam *friend_list_param) const;
	void GetEnemyList(EnemyListParam *enemy_list_param) const;
	void GetBlackList(BlacklistsParam * black_list_param) const;

	void GetChangeFriendList(FriendListParam *friend_list_param) const;
	void GetChangeBlackList(BlacklistsParam *black_list_param) const;
	void GetChangeEnemyList(EnemyListParam *enemy_list_param) const;
	void ClearDirtyMark();
	void ClearFriendDirtyMark();
	void ClearEnemyDirtyMark();
	void ClearBlackDirtyMark();

	int GetWeekAddCharm() { return m_week_add_charm; }
	int GetDayAddCharm() { return m_day_add_charm; }
	int GetFriendNumber() { return m_friend_num; }

	int GetMaxFriendNum();
	int GetFriendIntimacy(const UserID &friend_user_id);
	int GetFriendGiftCount(const UserID &friend_user_id);
	void SetFriendGiftCount(const UserID &friend_user_id, int gift_count_value);
	void ClearIntimacy(const UserID &user_id, const char *reason);

	static void IntimacyIncrease(Role *this_role, Role *other_role, int value, const char *reason, bool from_team = false);

	bool IntimacyIncreaseHelper(const UserID &user_id, int value, bool is_set, const char *reason, bool from_team, int *p_curr_intimacy = NULL);

	void OnRoleResetName();
	void OnResetFriendName(int friend_uid, GameName new_friend_name);

	void OnRoleSetSex();
	void OnSetFriendSex(int friend_uid, int friend_plat_type, char sex);
	
	void OnRoleUpgradeLevel();	
	void OnSetFriendLevel(int friend_uid);

	void SystemMailToAllFriend(MailSubject subject, MailContentTxt contenttxt);

	void SendSongHuaInfo();
	void OnGiveFlower(ggprotocal::GiveFlower *gf_msg);
	void OnBeGiveFlower(ggprotocal::BeGiveFlower *bfg_msg);
	int GetAllGiveFlowerNum() { return m_all_give_flower_num; }
	int GetDayGiveFlowerNum() { return m_day_give_flower_num; }
	void AddAllCharm(int value);
	int GetAllCharm() { return m_all_charm; }
	bool SetAllCharm(int charm);

	void SendAllCharm();

	int GetFriendUidList(int *friend_list_uid, int length);

	int GetFavorableImpression(const UserID &friend_user_id);
	bool AddFavorableImpression(const UserID &friend_user_id);
	bool SetFavorableImpression(const UserID &friend_user_id, int number);
	int GetDayChatCount(const UserID &friend_user_id);

private:

	void SendFriendChangeToRole(int index, const UserID &user_id) const;
	void SendEnemyChangeToRole(int index, const UserID &user_id, int plat_type) const;
	void SendBlackChangeToRole(int index, const UserID &user_id) const;

	void ResizeFriendList(int size);
	void ResizeEnemyList(int size);
	void ResizeBlackList(int size);

	void FriendSave();
	void CheckClearAddCharm();

	struct FriendItem
	{
		UserID		user_id;
		int			intimacy;				// 亲密度
		int			team_today_intimacy;	// 每天组队亲密度
		int			gift_count;				// 每天送礼次数
		int			favorable_impression;	// 好感度
		int			day_chat_count;			// 每天聊天条数
		long long	add_timestamp;			// 添加好友时间
	};

	struct EnemyItem
	{
		UserID		user_id;
		int			plat_type;
		long long	last_kill_time;
		int			kill_count;
		int         be_kill_count;
	};

	struct BlackItem
	{
		UserID		user_id;
		GameName	gamename;
		char		sex;
		char		prof;
	};

	Role			*m_role;

	int				m_friend_num;								// 好友数量
	int				m_max_friend_num;
	FriendItem		*m_friend_list;
	bool			*m_friend_dirty_mark;
	bool			*m_friend_old_state;
	bool			m_friend_list_is_change;

	int				m_black_num;								// 黑名单数量
	int				m_max_black_num;
	BlackItem		*m_black_list;
	bool			*m_black_dirty_mark;
	bool			*m_black_old_state;
	bool			m_black_list_is_change;


	int				m_enemy_num;								// 敌人数量
	int				m_max_enemy_num;
	EnemyItem		*m_enemy_list;
	bool			*m_enemy_dirty_mark;
	bool			*m_enemy_old_state;
	bool			m_enemy_list_change;

	static const int GIVE_FLOWER_BROADCAST_NUM = 99;			// 送花广播数量

	int				m_all_give_flower_num;
	int				m_day_give_flower_num;
	int				m_all_charm;
	int				m_week_add_charm;
	int				m_day_add_charm;

	bool			m_sent_to_client;
	bool			m_friend_chked;
};

#endif

