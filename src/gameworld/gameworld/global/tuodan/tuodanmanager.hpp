#ifndef __TUODAN_MAN_HPP__
#define __TUODAN_MAN_HPP__

#include <map>
#include <set>
#include "servercommon/struct/global/tuodanparam.hpp"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "protocal/msgtuodan.h"
#include "servercommon/activitydef.hpp"
#include "engineadapter.h"
#include "protocal/msgactivity.hpp"

class TuodanManager;
class Role;


struct TuodanItem
{
	TuodanItem() : uid(0), create_time(0)
	{
		memset(notice, 0, sizeof(notice));;
	}

	bool Invalid() { return 0 == uid || 0 == create_time; }
	int uid;
	unsigned int create_time;
	TuodanNotice notice;
};

class TuodanManager
{
	friend class RMIInitTuodanBackObjectImpl;

public:
	static TuodanManager & Instance();
	
	bool IsLoadFinish() { return m_is_loaded; }

	void OnServerStart();
	void OnServerStop();
	void Init(int ret, const TuodanParam &tuodan_param);
	void Update(unsigned long interval, time_t now_second);

	void OnMarry(Role *role);
	void OnSetSex(Role *role);
	void TuodanOpera(Role *role, int req_type, TuodanNotice notice);
	void SendAllTuodan(Role *role);
	void SendSingleTuodanToWorld(Role *role, int change_type);

	bool OnToudanChange(TuodanItem *toudan_item, int change_state);
	void PackTuodanInfo(TuodanItem* tuodan_item, Protocol::UserTuodanInfo *tuodan_info);

private:
	TuodanManager();
	~TuodanManager();

	TuodanItem *GetTuodanItem(int uid);
	void OnUpdateTuodan(Role *role, TuodanNotice notice);
	void OnRemoveTuodan(Role *role);
	bool CheckSaveToudan(time_t now_second);
	
	bool m_is_loaded;
	TuodanParam m_change_param;
	time_t m_last_save_time;

	std::map<int, TuodanItem*> m_tuodan_map;
};

#endif  // __TUODAN_MAN_HPP__

