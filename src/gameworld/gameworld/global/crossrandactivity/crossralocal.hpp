/******************************************************************************************
FileName: crossralocal.hpp
Author: face2wind
Description: �������-gameworld���̣���CrossRandActivity�������ӵײ�Э��ֱ�ӽ���
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
face2wind   2016/04/21/ 18:14:05     1.0           ����
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
	void SendToCrossActivity(char *data, int length);	// ���͵�crossserver��Ӧ�Ļ������
	void NotifyActStatusToClient(Role *user = NULL);	// ֪ͨ�ͻ��˵�ǰ�״̬
	virtual void OnActivityStatusChange(int from_status, int to_status);

	int m_activity_type;								// ��������
	int m_activity_status;								// ��ǰ�״̬

	unsigned int m_begin_time;
	unsigned int m_end_time;

	CrossRandActivityData m_custom_data;				// �Զ������ݣ���Ż������������
	CrossRandActivityBigParam *m_big_param_data;		// ����ͨ������
	CrossRandActivityBigParam2 *m_big_param_data2;		// ����ͨ������
};

#endif // __ACTIVITY_HPP__

