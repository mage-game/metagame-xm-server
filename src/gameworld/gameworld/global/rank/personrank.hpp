/******************************************************************************************
FileName: personrank.hpp
Author: 
Description: 个人排行榜  
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreanwang    2011/08/26/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __PERSON_RANK_DEF_HPP__
#define __PERSON_RANK_DEF_HPP__

#include <map>
#include <set>

#include "servercommon/struct/global/rankparam.hpp"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "protocal/msgcsrank.h"
#include "servercommon/activitydef.hpp"
#include "engineadapter.h"
#include "protocal/msgactivity.hpp"

class RankManager;
class Role;

struct PersonRankItem
{
	PersonRankItem()
		: user_id(INVALID_USER_ID), reserve(0), sex(0), prof(0), camp(0), vip_level(0), 
		exp(0), level(0), rank_type(-1), rank_value(0), record_index(0), flexible_int(0), flexible_ll(0), reserved(0),
		jingjie_level(0), reserve1(0)
	{
		memset(user_name, 0, sizeof(user_name));
		memset(flexible_name, 0, sizeof(flexible_name));
	}

	bool Invalid() { return -1 == rank_type; }
	void Reset() { rank_type = -1; }

	int GetRoleId() { return UidToInt(user_id); }

	UserID user_id;
	GameName user_name;
	GameName flexible_name;
	char reserve;
	char sex;
	char prof;
	char camp;
	int vip_level;
	long long exp;
	int level;
	int rank_type;
	long long rank_value;
	int record_index;								// 每种排行类型 允许多条记录 如果仅仅有一条 填0 

	int flexible_int;
	long long flexible_ll;
	long long reserved;

	short jingjie_level;
	short reserve1;
};

class PersonRank
{
public:
	PersonRank();
	~PersonRank();

	void SetRankManager(RankManager *rank_manager) { m_rank_manager = rank_manager; }

	void Init(const PersonRankParam &personrank_param);
	void Update(time_t now_second);

	void OnUserLogin(Role *user);
	void Snapshot();
	void ForceSnapshot();

	void OnGameworldAccept(NetID netid);

	void GetChangeRankItemList(PersonRankParam *personrank_param);
	void ClearDirtyMark();

	void OnGetRankList(Role *role, Protocol::CSGetPersonRankList &rank_list_req);
	void OnGetRankTopUser(Role *role, Protocol::CSGetPersonRankTopUser &top_user_req);
	void OnGetRealTimeRankList(Role *role, int rank_type);

	void OnBaseInfoChange(Role *role);
	void OnRankInfoChange(const UserID &user_id, SyncPersonRankInfo &rank_info);

	void OnLastBattleFieldRankChange(gglobalprotocal::SyncLastBattleFieldRankInfo &rank_info);

	void OnCrossRankInfoReq(crossgameprotocal::CrossGameRankInfoReq &cross_rank_info_req);

	void ClearRank(int rank_type);

	int GetUserRank(UserID user_id, int rank_type);
	bool IsUserOnRank(UserID user_id, int rank_type = -1);
	void DeleteUserFromRank(UserID user_id, int rank_type = -1);

	bool GetRankUserInfo(int rank_type, const UserID &user_id, GameName user_name, char *user_sex, char *camp = NULL, int *level = NULL);
	const PersonRankItem * GetPersonRankItem(int rank_type, const UserID &user_id, int index = 0);
	int GetRankTopUser(int rank_type, int max_user, UserID *user_list, int camp_limit = CAMP_TYPE_INVALID);	// 注意：GetRankTopUser是在最新数据里边找到当前排名靠前的角色id
	int GetRankTopItem(int rank_type, int item_count, PersonRankItem *item_list);		// 注意：GetRankTopUser是在最新数据里边找到当前排名靠前的角色id
	int GetSnapShotTopUser(int rank_type, int max_user, UserID *user_list);				// 注意：GetSnapShotTopUser是在排名镜像里边找到当前排名靠前的角色id，会有延迟
	int GetSnapShotTopItem(int rank_type, int item_count, PersonRankItem *item_list);	// 注意：GetSnapShotTopUser是在排名镜像里边找到当前排名靠前，会有延迟
	void GetBipinTopUserInfo(int rand_activity_type, int history_cap_type, Protocol::TopUserInfo topuser_list[CAMP_TYPE_MAX]);
		
	bool IsRankLoadFinish() { return m_load_flag; }  

	void UserResetName(const UserID &user_id, GameName name);

	void SetRankList(int rank_count, PersonRankItem *rank_item);

	int GetRankList(int rank_type, int count, PersonRankItem *rank_list);
	bool AllowSnapshot(int rank_type);

	PersonRankItem * GetTopOneItemByRankType(int rank_type);
private:
	bool IsValidIndex(int index) { return index >= 0 && index < PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE; }

	void UpdateRankList(PersonRankItem &rank_item);
	void UpdateBaseInfo(PersonRankItem &rank_item, Role *role);
	bool GetSwapRankItemSlot(int rank_type, const PersonRankItem &rank_item, int *swap_post);
	bool GetFreeRankItemSlot(int rank_type, int *free_post);

	bool AddUserIndex(int rank_type, const UserID &user_id, int rank_index);
	bool RemoveUserIndex(int rank_type, const UserID &user_id, int rank_index);
	bool GetUserRecordIndex(int rank_type, const UserID &user_id, int record_index, int *rank_index);
	int GetRankTypeSize(int rank_type);

	void OnRankLoadFinish();
	void OnRankPosChange(int rank_type, const UserID &user_id, int rank_index);
	
	void RecalcWorldLevelInfo();
	void SendWorldLevelToUser(bool is_broadcast = false, Role *user = NULL);
	void SyncWorldLevelInfoToGameWorld();

	bool CanUpdateUserRankValue(PersonRankItem &rank_item, int find_index);
	bool SensitiveToSex(int rank_type);
	bool SensitiveToCamp(int rank_type);
	bool RankTypeNeedLog(int rank_type);
	
	RankManager *m_rank_manager;
	PersonRankItem m_rank_list[PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE];

	bool m_dirty_mark[PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE];
	bool m_old_state[PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE];
	bool m_change_flag;
	bool m_load_flag;

	typedef std::map<UserID, std::set<int> > UserIndexMap;
	typedef std::map<UserID, std::set<int> >::iterator UserIndexMapIt;

	UserIndexMap m_user_index_map[PERSON_RANK_TYPE_MAX];

	time_t m_force_snapshot_time;
	time_t m_last_snapshot_time;
	PersonRankItem m_rank_snapshot[PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE];

	int m_world_level;																// 世界等级
	int m_top_user_level;															// 全服角色最高等级
	int m_capability_rank_mincap;													// 战力榜最低角色战力 
};

#endif  // __PERSON_RANK_DEF_HPP__

