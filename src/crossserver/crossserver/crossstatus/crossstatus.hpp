#ifndef __CROSS_STATUS_HPP__
#define __CROSS_STATUS_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/struct/cross/crossstatusparam.hpp"

class CrossServer;

enum CROSSSTATE_DATA_STATUS
{			
	CROSSSTATE_DATA_STATUS_INVALID = 0,						// ��ʼ״̬
	CROSSSTATE_DATA_STATUS_LOADING,							// ���ݼ����� 
	CROSSSTATE_DATA_STATUS_LOAD_FINISH,						// ���ݼ������ 
	CROSSSTATE_DATA_STATUS_MAX,
};

static const int CROSSSTATUS_SAVE_INTERVAL = 30;			// 30�뱣��һ��

class CrossStatus
{
	friend class RMIInitCrossStatusBackObjectImpl;

public:
	CrossStatus();
	~CrossStatus();

	void Init(const CrossStatusParam &crossstatus_param);

	void SetCrossServer(CrossServer *cross_server) { m_cross_server = cross_server; }
	CrossServer * GetCrossServer() { return m_cross_server; }

	void OnServerStart();
	void OnServerStop();

	void Update(unsigned long interval, time_t now_second);

	bool IsLoadFinish() const { return (CROSSSTATE_DATA_STATUS_LOAD_FINISH == m_data_state); }

	unsigned int GetLastUpdateDayID() { return m_last_update_dayid; }
	void SetLastUpdateDayID(unsigned int dayid) { m_last_update_dayid = dayid; }
	
private:
	bool LoadCrossStatus(long long id_from);
	void LoadCrossStatusSucc();
	bool SaveCrossStatus();

	CrossServer *m_cross_server;

	int m_data_state;									// ���ݼ���״̬
	time_t m_last_save_time;							// �ϴα���ʱ��
	unsigned int m_last_update_dayid;					// ��һ�θ��µ�DayId
};

#endif // __CROSS_STATUS_HPP__

