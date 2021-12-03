#ifndef __CROSS_RECORD_MANAGER_HPP__
#define __CROSS_RECORD_MANAGER_HPP__

#include <map>

#include "servercommon/struct/global/crossrecordlistparam.hpp"

#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/internalprotocal/crossglobalprotocal.h"

#include "crossrecord.hpp"

#include "engineadapter.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"

class Role;

enum CROSS_RECORD_MANAGER_DATA_STATE
{	
	CROSS_RECORD_MANAGER_DATA_STATE_INVALID = 0,						// 初始状态
	CROSS_RECORD_MANAGER_DATA_STATE_LOADING,							// 数据加载中 
	CROSS_RECORD_MANAGER_DATA_STATE_LOAD_FINISH,						// 数据加载完成
	CROSS_RECORD_MANAGER_DATA_STATE_MAX,
};

class CrossRecordManager
{
	friend class RMIInitCrossRecordObjectImpl;

public:
	static CrossRecordManager & Instance();

	bool OnServerStart();
	void OnServerStop();

	bool IsLoadFinish() const { return (CROSS_RECORD_MANAGER_DATA_STATE_LOAD_FINISH == m_data_state); }

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnWeekChange();

	CrossRecord * CreateCrossRecord(Role *user);
	CrossRecord * GetCrossRecord(const UserID &user_id);
	CrossRecord * GetCrossRecord(Role *user, bool is_auto_create = false);

	void OnSyncRoleBaseInfo(Role *user);
	void OnUserResetName(const UserID &user_id, GameName name);
	
	void OnConnectCrossServerSucc();
	void OnUserLogin(Role *user);

	void OnUpdateCrossRecord(const UserID &user_id, crossgameprotocal::CrossGameUpdateCrossRecord *cgucr);
	void OnGetCrossRecord(NetID cross_netid, const UserID &user_id, const UniqueUserID &req_unique_user_id);

	void OnCrossActivityStatusChange(int cross_activity_type, int from_status, int to_status);

	void OnGMAddCrossMultiuserChallengeScore(Role *user, int add_score);
	bool SaveCrossRecord(CrossRecord *cross_record, char change_state);

private:
	CrossRecordManager();
	~CrossRecordManager();

	bool LoadCrossRecord(long long id_from);
	void LoadCrossRecordSucc();
	bool InitCrossRecord(const CrossRecordListParam::CrossRecordAttr &attr);
	bool CheckSave(time_t now_second);

	void ClearAllCrossChallengeOrderViewInfo(bool is_sync_to_role = true, int order_view_match_index = -1);

	int m_data_state;															// 数据加载状态
	CrossRecordListParam m_change_list_param;									// 待存档数据
	time_t m_last_save_time;													// 上次保存时间
	
	typedef std::map<UserID, CrossRecord*> UserCrossRecordMap;
	typedef std::map<UserID, CrossRecord*>::iterator UserCrossRecordMapIt;
	UserCrossRecordMap m_user_cross_record_map;									// 角色和跨服记录对应表
};

#endif // __CROSS_RECORD_MANAGER_HPP__

