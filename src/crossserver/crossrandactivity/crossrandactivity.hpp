/******************************************************************************************
FileName: crossrandactivity.hpp
Author: face2wind
Description: 跨服随机活动-cross进程，跟CrossRALocal可以无视底层协议直接交互
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
face2wind   2016/04/21/ 18:14:05     1.0           创建
*****************************************************************************************/

#ifndef __CROSS_RAND_ACTIVITY_HPP__
#define __CROSS_RAND_ACTIVITY_HPP__

#include "engineadapter.h"

#include "servercommon/activitydef.hpp"
#include "servercommon/serverdef.h"
#include "servercommon/crossdef.hpp"

#include <queue>
#include "servercommon/serverconfig/crossrandactivityopenconfig.hpp"

class CrossUser;

class CrossRandActivity
{	
public:
	CrossRandActivity(int cross_activity_type);
	virtual ~CrossRandActivity();

	virtual void Init();
	virtual void Update(unsigned int now_second);
	virtual void OnDayChange() {}
	virtual void OnWeekChange() {}

	bool IsInitFinish() { return m_is_init_finish; }

	virtual void OnRegisterGameServer(NetID global_netid);
	
	virtual int GetCrossRandActivityType() { return m_activity_type; }
	virtual int GetActivityStatus() { return m_activity_status; }

	void ForceToNextState();
	void ForceSetActivityTime(unsigned int begin_time, unsigned int end_time);

	virtual void OnCommonMessageFromGameworld(NetID global_netid, int msg_type, char *data);
	virtual void OnMessageFromGameworld(NetID global_netid, int msg_type, char *data) {}

protected:
	void SyncActivityStatusToGameworld(bool is_status_change = false, NetID global_netid = UINT_MAX);
	virtual void OnActivityStatusChange(int from_status, int to_status) {}

	void SendToGameworldActivity(char *data, int length, NetID global_netid = UINT_MAX); // 发送到gameworld本地对应的活动管理器

	bool ReadNextActivityTime();

	bool m_is_init_finish;							// 初始化完成标志

	int m_activity_type;							// 跨服活动类型
	int m_activity_status;							// 当前活动状态

	unsigned int m_begin_time;
	unsigned int m_end_time;

	std::queue<CrossRandActivityTimeCfg::TimeItem> m_open_time_list;
};

#endif // __CROSS_ACTIVITY_HPP__

