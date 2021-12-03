#ifndef __CROSS_STATUS_HPP__
#define __CROSS_STATUS_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/struct/cross/crossstatusparam.hpp"

class CrossServer;

enum CROSSSTATE_DATA_STATUS
{			
	CROSSSTATE_DATA_STATUS_INVALID = 0,						// 初始状态
	CROSSSTATE_DATA_STATUS_LOADING,							// 数据加载中 
	CROSSSTATE_DATA_STATUS_LOAD_FINISH,						// 数据加载完成 
	CROSSSTATE_DATA_STATUS_MAX,
};

static const int CROSSSTATUS_SAVE_INTERVAL = 30;			// 30秒保存一次

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

	int m_data_state;									// 数据加载状态
	time_t m_last_save_time;							// 上次保存时间
	unsigned int m_last_update_dayid;					// 上一次更新的DayId
};

#endif // __CROSS_STATUS_HPP__

