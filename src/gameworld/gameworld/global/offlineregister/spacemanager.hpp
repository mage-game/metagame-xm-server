#ifndef __SPACE_MANAGER_HPP__
#define __SPACE_MANAGER_HPP__

#include <stdlib.h>

#include <map>
#include <set>

class Role;

#include "servercommon/serverdef.h"

#include "servercommon/struct/global/spaceparam.hpp"

#include "protocal/msgrole.h"

static const int MAX_SAVE_SPACE_INTERVAL = 20;					// 回档时间
static const int MAX_REMOVE_SPACE_INTERVAL = 30;				// Remove检查间隔

enum SPACE_DATA_STATE
{				
	SPACE_STATE_INVALID = 0,									// 初始状态
	SPACE_STATE_LOADING,										// 数据加载中 
	SPACE_STATE_LOAD_FINISH,									// 数据加载完成 
	SPACE_DATA_STATE_MAX
};

class SpaceManager
{
	friend class RMIInitSpaceRecordBackObjectImpl;

public:
	static SpaceManager & Instance();

	void SyncSpaceMsgInfoToRole(Role *role, int targe_uid, int msg_type, bool is_new_notice);

	void OnGetRoleSpaceMsgInfo(Role *role, Protocol::CSGetRoleSpaceMsgInfo *grsmi);
	void OnKeyCaiAllFriend(Role *role);
	void OnOperSpace(Role *role, Protocol::CSSpaceOper *so, bool is_notify = true);	
	void OnRemoveRecord(Role *role, Protocol::CSSpaceRemoveRecord *srr);	

	void OnServerStart();
	void OnServerStop();

	bool IsLoadFinish();

	void Update(unsigned long interval, time_t now_second);

private:
	SpaceManager();
	~SpaceManager();

	bool CheckSaveSpaceRecord(time_t now_second);
	bool OnSpaceRecordChange(const SpaceRecord &space_record, int change_state);
	bool AddSpaceRecord(int uid, const SpaceRecord &space_record, bool is_init = false);

	bool LoadSpaceRecord(long long record_id_from);
	void LoadSpaceRecordSucc();

	bool CheckRecoreFull(const SpaceRecord &space_record);
	long long GetRecordUnqiueID();
	std::map<long long, SpaceRecord> * GetUserTypeRecordMap(int uid, int msg_type);
	bool CheckCanCai(Role *role, int target_uid);

	SpaceParam m_change_param; 

	int m_data_state;
	time_t m_last_save_time; 
	time_t m_last_remove_time; 

	struct UserSpaceRecord
	{
		UserSpaceRecord() {}

		std::map<long long, SpaceRecord> * GetMsgMap(int msg_type, int *max_size)
		{
			if (Protocol::SPACE_MSG_TYPE_LIUYAN == msg_type)
			{
				if (NULL != max_size) *max_size = SPACE_LIUYAN_MAX;
				return &liuyan_map;
			}
			else if (Protocol::SPACE_MSG_TYPE_CAI == msg_type)
			{
				if (NULL != max_size) *max_size = SPACE_CAI_MAX;
				return &cai_map;
			}
			else if (Protocol::SPACE_MSG_TYPE_GIFT == msg_type)
			{
				if (NULL != max_size) *max_size = SPACE_GIFT_MAX;
				return &gift_map;
			}

			return NULL;
		}

		std::map<long long, SpaceRecord> liuyan_map;					// 留言板
		std::map<long long, SpaceRecord> cai_map;						// 踩/被踩记录
		std::map<long long, SpaceRecord> gift_map;						// 礼物记录
	};

	typedef std::map<int, UserSpaceRecord> UserSpaceRecordMap;
	typedef std::map<int, UserSpaceRecord>::iterator UserSpaceRecordMapIt;
	UserSpaceRecordMap m_user_record_map;

	int inc_seq;
};

#endif // __SPACE_MANAGER_HPP__





