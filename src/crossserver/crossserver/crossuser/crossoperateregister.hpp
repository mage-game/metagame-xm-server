/******************************************************************************************
FileName: CrossOperateRegister
Author: 
Description: �����������ʱ���ݴ�
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreanwang    2011/08/23/ 09:04:05     1.0           ����
*****************************************************************************************/

#ifndef __CROSS_OPERATE_REGISTER__
#define __CROSS_OPERATE_REGISTER__

#include <map>

#include "servercommon/crossdef.hpp"

static const int CROSS_OPERATE_CHECK_TIME_INTERVAL_S = 60;		// ���ڼ����

enum CROSS_OPERATE_TYPE
{
	CROSS_OPERATE_GET_CROSS_CHALLENGE_SESSION_INFO,				// ��ȡ���������Ϣ
	CROSS_OPERATE_GET_CROSS_CHALLENGE_SESSION_RECORD,			// ��ȡ������������¼
	
	CROSS_OPERATE_MAX
};

class CrossServer;

class CrossOperateRegister
{
public:
	CrossOperateRegister();
	~CrossOperateRegister();

	void SetCrossServer(CrossServer *cross_server) { m_cross_server = cross_server; }
	CrossServer * GetCrossServer() { return m_cross_server; }

	void Update(unsigned long interval, time_t now_second);

	bool CheckCanCrossOperate(const UniqueUserID &unique_user_id, int cross_operate_type, int operate_interval_s);

	bool CheckCanGetCrossChallengeSessionInfo(const UniqueUserID &unique_user_id);
	bool CheckCanGetCrossChallengeSessionRecord(const UniqueUserID &unique_user_id);

private:
	CrossServer *m_cross_server;

	struct UserNextTimeInfo
	{
		UserNextTimeInfo()
		{
			memset(next_time_list, 0, sizeof(next_time_list));
		}

		time_t next_time_list[CROSS_OPERATE_MAX];				// �´οɲ���ʱ���б�
	};
	
	typedef std::map<UniqueUserID, UserNextTimeInfo> UserNextTimeInfoMap;
	typedef std::map<UniqueUserID, UserNextTimeInfo>::iterator UserNextTimeInfoMapIt;
	UserNextTimeInfoMap m_user_time_info_map;					// �û���Ӧ���´οɲ���ʱ���б�

	time_t m_last_check_expire_time;							// �ϴμ�����ʱ��
};

#endif  // __CROSS_OPERATE_REGISTER__

