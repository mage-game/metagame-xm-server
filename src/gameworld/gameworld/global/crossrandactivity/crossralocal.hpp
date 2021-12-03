/******************************************************************************************
FileName: crossralocal.hpp
Author: face2wind
Description: 跨服随机活动-gameworld进程，跟CrossRandActivity可以无视底层协议直接交互
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
face2wind   2016/04/21/ 18:14:05     1.0           创建
*****************************************************************************************/

#ifndef __CROSS_RA_LOCAL_HPP__
#define __CROSS_RA_LOCAL_HPP__

#include "iclockmodule.h"

#include "servercommon/activitydef.hpp"
#include "servercommon/serverdef.h"
#include "servercommon/crossrandactivitydef.hpp"

class CrossRALocal
{
public:
	CrossRALocal(int m_activity_type);
	virtual ~CrossRALocal();

	virtual void Init(const CrossRandActivityData &data);
	virtual void GetInitParam(CrossRandActivityData *data);

	virtual void SetBigData(CrossRandActivityBigParam *big_data);
	virtual void SetBigData2(CrossRandActivityBigParam2 *big_data);

	virtual void Update(unsigned long interval, unsigned int now_second) {}
	virtual void OnDayChange() {}
	virtual void OnWeekChange() {}

	virtual void OnUserLogin(Role *user);
	virtual void OnUserLogout(Role *user) {}

	virtual void OnCommonMessageFromCross(int msg_type, char *data);
	virtual void OnMessageFromCross(int msg_type, char *data) {}

	void OnForceToNextState();
	bool OnForceSetActivityTime(unsigned int begin_time, unsigned int end_time);

protected:
	void SendToCrossActivity(char *data, int length);	// 发送到crossserver对应的活动管理器
	void NotifyActStatusToClient(Role *user = NULL);	// 通知客户端当前活动状态
	virtual void OnActivityStatusChange(int from_status, int to_status);

	int m_activity_type;								// 跨服活动类型
	int m_activity_status;								// 当前活动状态

	unsigned int m_begin_time;
	unsigned int m_end_time;

	CrossRandActivityData m_custom_data;				// 自定义数据，存放活动自身所需数据
	CrossRandActivityBigParam *m_big_param_data;		// 随机活动通用数据
	CrossRandActivityBigParam2 *m_big_param_data2;		// 随机活动通用数据
};

#endif // __ACTIVITY_HPP__

