#ifndef __JING_LING_HOME_HPP__
#define __JING_LING_HOME_HPP__

#include "global/globalrole/globalrole.hpp"
#include "servercommon/serverdef.h"
#include "servercommon/jinglingdef.hpp"

class Role;

class JingLingHome : public IGlobalRoleItem
{
public:
	JingLingHome(GlobalRole *g_role);
	~JingLingHome();

	virtual void Init(const JingLingHomeParam &param);
	virtual void GetAttr(JingLingHomeParam &param);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void OnInfoReq(Role *role);
	void OnPutJingLing(Role *role, short jingling_index, short home_index);
	void OnTakeOutJingLing(Role *role, short home_index);
	void OnQuick(Role *role, short home_index);
	void OnGetReward(Role *role, short home_index);
	void OnBeRob(Role *role, short jingling_index, short home_index);
	void OnRefreshList(Role *role);
	void OnReadRobRecord(Role *role);

	void RecordRob(int role_id);
	int GetRobTimes(int role_id);

	bool CanRob();
	void OnTakeOffJingLing(Role *role, ItemID item_id);
	void OnJingLingCapabilityChange(Role *role);

private:
	void CalcReward();
	void SendInfo(Role *role, int reason = 0);
	void SendListInfo(Role *role);
	void SendRobRecord(Role *role);

	JingLingHomeParam m_param;
};

#endif // __JING_LING_HOME_HPP__