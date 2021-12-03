#ifndef SHENGXINGZHULI_MANAGER_HPP
#define	SHENGXINGZHULI_MANAGER_HPP

#include "servercommon/shengxingzhulidef.hpp"
#include "other/footprint/footprint.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/shengong/shengongfunction.hpp"
#include "other/shengong/shengongconfig.hpp"
#include "other/shenyi/shenyiconfig.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"

class Role;

const static unsigned int sec_per_day = 24 * 60 * 60;

class ShengxingzhuliManager
{
public:
	ShengxingzhuliManager();
	~ShengxingzhuliManager();
	void SetRole(Role *role) { m_role = role; }

	void InitParam(const ShengxingzhuliParam &shengxingzhuli_param);
	void GetInitParam(ShengxingzhuliParam * shengxingzhuli_param);

	void OnRoleChongzhi(int chongzhi);
	//屏蔽奖励功能
	void OnGetReward(bool is_manual);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void SendAllInfo();
private:
	Role * m_role;
	ShengxingzhuliParam m_param;

	void SetIsGetReward();

	int GetDaySystemIndex(int day = 0);		// 根据day获得那天系统的序号
	bool IsSystemMaxGrade(int sys_type);	// 对应的系统是否最高级
	bool IsSystemActive(int sys_type);		// 对应的系统是否激活
};

#endif