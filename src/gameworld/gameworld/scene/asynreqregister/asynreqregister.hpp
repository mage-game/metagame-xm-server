#ifndef __ASYN_REQ_REGISTER_HPP__
#define __ASYN_REQ_REGISTER_HPP__

#include "servercommon/serverdef.h"

#include <map>

class AsynReqRegister
{
public:
	static AsynReqRegister & Instance();

	int OnAsynReq(GSNetID netid);
	void OnNetidDisconnect(GSNetID netid);
	bool OnAsynRspCheckNetidExist(GSNetID netid, int key);

private:
	AsynReqRegister();
	~AsynReqRegister();

	int m_inc_key;

	std::map<GSNetID, int> m_gsnetid_set;
};

#endif // __ASYN_REQ_REGISTER_HPP__

