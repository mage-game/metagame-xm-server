#include "crossstatus.hpp"

#include "rmibackobjdef.h"

#include "internalcomm.h"
#include "engineadapter.h"

#include "crossserver.h"

CrossStatus::CrossStatus()
	: m_cross_server(NULL), m_data_state(CROSSSTATE_DATA_STATUS_INVALID), m_last_save_time(0), m_last_update_dayid(0)
{

}

CrossStatus::~CrossStatus()
{

}

void CrossStatus::Init(const CrossStatusParam &crossstatus_param)
{
	CrossServer::GetInstCrossServer()->GetCrossActivityManager()->InitCrossActivity(crossstatus_param.cross_activity_param);
	CrossServer::GetInstCrossServer()->GetCrossPersonRank()->InitRankTitleParam(crossstatus_param.cross_person_rank_title_param);
	CrossServer::GetInstCrossServer()->GetCrossRandActivityManager()->InitActivity();

	m_last_update_dayid = crossstatus_param.last_update_dayid;

	if (0 == m_last_update_dayid) m_last_update_dayid = EngineAdapter::Instance().DayID();

	this->LoadCrossStatusSucc();
}

void CrossStatus::OnServerStart()
{
	this->LoadCrossStatus(0);
}

void CrossStatus::OnServerStop()
{
	this->SaveCrossStatus();
}

void CrossStatus::Update(unsigned long interval, time_t now_second)
{
	if (!this->IsLoadFinish()) return;
	
	if (now_second >= m_last_save_time + CROSSSTATUS_SAVE_INTERVAL)
	{
		this->SaveCrossStatus(); 

		m_last_save_time = now_second;
	}
}

bool CrossStatus::LoadCrossStatus(long long id_from)
{
	if (this->IsLoadFinish()) return false;

	m_data_state = CROSSSTATE_DATA_STATUS_LOADING;

	RMIInitCrossStatusBackObjectImpl *impl = new RMIInitCrossStatusBackObjectImpl();
	impl->crossstatus = this;

	RMICrossClient cc;
	cc.__bind_session(InternalComm::Instance().GetDBHandler().GetCrossDB());
	return cc.InitCrossStatusAsyn(id_from, impl);

	return false;
}

void RMIInitCrossStatusBackObjectImpl::InitCrossStatusRet(int ret, const CrossStatusParam &crossstatus_param)
{
	if (ret < 0)
	{
		printf(" RMIInitCrossStatusBackObjectImpl::InitCrossStatusRet Failed \n");
		EngineAdapter::Instance().StopGame();
		return;
	}

	crossstatus->Init(crossstatus_param);
}

void CrossStatus::LoadCrossStatusSucc()
{
	m_data_state = CROSSSTATE_DATA_STATUS_LOAD_FINISH;

	m_last_save_time = EngineAdapter::Instance().Time();
}

bool CrossStatus::SaveCrossStatus()
{
	if (!this->IsLoadFinish()) return false;

	static CrossStatusParam crossstate_param; 

	crossstate_param.last_update_dayid = m_last_update_dayid;
	CrossServer::GetInstCrossServer()->GetCrossActivityManager()->GetCrossActivityParam(&crossstate_param.cross_activity_param);
	CrossServer::GetInstCrossServer()->GetCrossPersonRank()->GetRankTitleParam(&crossstate_param.cross_person_rank_title_param);

	RMISaveCrossStatusBackObjectImpl *impl = new RMISaveCrossStatusBackObjectImpl();

	RMICrossClient cc;
	cc.__bind_session(InternalComm::Instance().GetDBHandler().GetCrossDB());
	return cc.SaveCrossStatusAsyn(crossstate_param, impl);

	return false;
}

