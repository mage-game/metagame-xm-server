/******************************************************************************************
FileName: crossramanagerlocal.hpp
Author: face2wind
Description: 跨服随机活动管理器-gameworld进程
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
face2wind   2016/04/21/ 18:14:05     1.0           创建
*****************************************************************************************/

#ifndef __CROSS_RAND_ACTIVITY_MANAGER_HPP__
#define __CROSS_RAND_ACTIVITY_MANAGER_HPP__

#include "servercommon/activitydef.hpp"
#include "servercommon/crossrandactivitydef.hpp"

class Role;
class CrossRALocal;

namespace crossgameprotocal{
	struct CrossGameSyncCrossRandActivityData;
}

namespace Protocol {
	class CSCrossRandActivityRequest;
}

class CrossRAManagerLocal
{
public:
	static CrossRAManagerLocal & Instance();

	void Init(const CrossRandActivityParam &param, const CrossRandActivityBigParam &big_param);
	void Init2(const CrossRandActivityBigParam2 &big_param);

	void GetInitParam(CrossRandActivityParam *param, CrossRandActivityBigParam *big_param);

	void GetInitParam2(CrossRandActivityBigParam2 *big_param);

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnWeekChange();

	void OnUserLogin(Role *user);
	void OnUserLogout(Role *user);

	bool OnForceSetActivityTime(int rand_activity_type, unsigned int begin_time, unsigned int end_time);
	void OnForceToNextState(int rand_activity_type);

	CrossRALocal * GetActivity(int activity_type);

	void OnSyncActivityData(crossgameprotocal::CrossGameSyncCrossRandActivityData * srad, int length);

private:
	CrossRAManagerLocal();
	~CrossRAManagerLocal();

	CrossRALocal *m_rand_activity_list[CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT];

	CrossRandActivityBigParam m_act_big_param;
	CrossRandActivityBigParam2 m_act_big_param2;

	bool m_is_first;
};

#endif // __RAND_ACTIVITY_MANAGER_HPP__

