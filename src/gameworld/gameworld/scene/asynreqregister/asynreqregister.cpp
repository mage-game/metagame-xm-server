#include "asynreqregister.hpp"

#include "gamelog.h"

AsynReqRegister::AsynReqRegister()
	: m_inc_key(0)
{

}

AsynReqRegister::~AsynReqRegister()
{

}

AsynReqRegister & AsynReqRegister::Instance()
{
	static AsynReqRegister asyn_register;
	return asyn_register;
}

int AsynReqRegister::OnAsynReq(GSNetID netid)
{
	gamelog::g_log_login.printf(LL_DEBUG, "[AsynReqRegister::OnAsynReq netid[%d,%d] ", netid.gateway_netid, netid.netid);

	m_gsnetid_set[netid] = ++m_inc_key;

	return m_inc_key;
}

void AsynReqRegister::OnNetidDisconnect(GSNetID netid)
{
	gamelog::g_log_login.printf(LL_DEBUG, "[AsynReqRegister::OnNetidDisconnect netid[%d,%d] ", netid.gateway_netid, netid.netid);

	m_gsnetid_set.erase(netid);
}

bool AsynReqRegister::OnAsynRspCheckNetidExist(GSNetID netid, int key)
{
	std::map<GSNetID, int>::iterator it = m_gsnetid_set.find(netid);
	if (it != m_gsnetid_set.end())
	{
		if (key == it->second)
		{
			m_gsnetid_set.erase(it);
			return true;
		}
		else 
		{
			gamelog::g_log_login.printf(LL_ERROR, "[ERROR AsynReqRegister::OnAsynRspCheckNetidExist netid exist bug key not match [%d,%d]]", netid.gateway_netid, netid.netid);
		}
	}
	else 
	{
		gamelog::g_log_login.printf(LL_ERROR, "[ERROR AsynReqRegister::OnAsynRspCheckNetidExist netid not exist[%d,%d]", netid.gateway_netid, netid.netid);
	}

	return false;
}

