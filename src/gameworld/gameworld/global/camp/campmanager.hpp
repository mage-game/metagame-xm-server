#ifndef __CAMP_MANAGER_HPP__
#define __CAMP_MANAGER_HPP__

#include "iclockmodule.h"

#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/struct/global/campparam.hpp"
#include "servercommon/campdef.hpp"

#include "camp.hpp"

#include "engineadapter.h"

class Role;

class CampManager
{
	friend class RMIInitCampBackObjectImpl;

public:
	static CampManager & Instance();

	bool OnServerStart();
	void OnServerStop();
	void OnWorldStatusFirstStart();

	bool IsLoadFinish() { return m_load_flag; }

	void Update(unsigned long interval, time_t now_second);	

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void OnUserLogin(Role *user);

	void OnRoleChangeCamp(int from_camp, int to_camp);
	void OnRoleJoinCamp(int camp_type);
	void OnGetCampInfo(Role *role);

	void GetCampTotalCapability(long long capability_list[CAMP_TYPE_MAX]);

	void CalCampCabality();
	void CalcCampRank3(int camp);
	void CheckRoleRank3Reward(Role *role);

private:

	CampManager();
	~CampManager();

	Camp * GetCamp(int camp);

	bool Load();
	void LoadRet(int ret, const CampParam &camp_param);
	void Save();

	bool m_load_flag;

	time_t m_next_save_time;


	Camp m_camp_list[CAMP_TYPE_MAX];
};

#endif // __CAMP_MANAGER_HPP__

