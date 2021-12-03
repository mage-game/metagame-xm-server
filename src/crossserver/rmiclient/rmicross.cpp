#include "rmicross.hpp"
#include "servercommon/darmi/dacross.hpp"

#include "servercommon/servercommon.h"
#include "engineadapter.h"

USE_DATAACCESS_VAR();

static char TLV_BUFF[8 * 1024 * 1024];

enum RMI_INIT_ERROR_TYPE
{
	RMI_INIT_ERROR_REQUEST,
	RMI_INIT_ERROR_RESPONSE,
	RMI_INIT_ERROR_EXCEPTION,
	RMI_INIT_ERROR_TIMEOUT,
};

static void RMIInitError(int err, const char *msg, bool stop = true) 
{
	printf("RMI Init Error[type: %d, msg: %s]\n", err, NULL == msg ? "" : msg);

	if (stop)
	{
		EngineAdapter::Instance().StopGame();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool RMICrossClient::InitCrossStatusAsyn(long long id_from, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(id_from);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitCrossStatusAsyn");
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::CROSS, dataaccessrmi::cross::INIT_CROSSSTATUS, s, backobj, true, 10000); 
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitCrossStatusAsyn");
		return false;
	}

	return ret;
}

bool RMIInitCrossStatusBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitCrossStatusAsyn");
		return false;
	}

	static CrossStatusParam crossstatus_param;

	if (ret == 0)
	{
		if (!crossstatus_param.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitCrossStatusAsyn");
			return false;
		}
	}

	this->InitCrossStatusRet(ret, crossstatus_param);

	return true;
}

void RMIInitCrossStatusBackObject::InitCrossStatusRet(int ret, const CrossStatusParam &CrossStatus_param)
{
	printf("RMIInitCrossStatusBackObject::InitCrossStatusRet:%d\n", ret);
}

void RMIInitCrossStatusBackObject::__exception(int error)
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "InitCrossStatusAsyn");
}

void RMIInitCrossStatusBackObject::__timeout()
{
	this->InitError();
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "InitCrossStatusAsyn");
}

void RMIInitCrossStatusBackObject::__free()
{
	delete this;
}

void RMIInitCrossStatusBackObject::InitError()
{
	printf("RMIInitCrossStatusBackObject InitError\n");
}

bool RMICrossClient::SaveCrossStatusAsyn(const CrossStatusParam &crossstatus_param, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK(sizeof(CrossStatusParam) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = crossstatus_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::CROSS, dataaccessrmi::cross::SAVE_CROSSSTATUS, s, backobj, true, 10000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMISaveCrossStatusBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SaveCrossStatusRet(ret);

	return true;
}

void RMISaveCrossStatusBackObject::SaveCrossStatusRet(int ret)
{
	printf("SaveCrossStatusRet:%d\n", ret);
}

void RMISaveCrossStatusBackObject::__free()
{
	delete this;
}

void RMISaveCrossStatusBackObject::__exception(int error)
{
	printf("RMISaveCrossStatusBackObject __exception: %d !!!\n", error);
}

void RMISaveCrossStatusBackObject::__timeout()
{
	printf("RMISaveCrossStatusBackObject __timeout !!!\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////

bool RMICrossClient::InitCrossPersonRankAsyn(rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = m_session.rmi_module->Call(m_session, dataaccessrmi::CROSS, dataaccessrmi::cross::INIT_CROSS_PERSON_RANK, 
		s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		RMIInitError(RMI_INIT_ERROR_REQUEST, "InitCrossPersonRankAsyn");
		return false;
	}

	return ret;
}

bool RMICrossClient::SaveCrossPersonRankAsyn(const CrossPersonRankParam &CrossPersonRank_param, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK(sizeof(CrossPersonRankParam) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = CrossPersonRank_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::CROSS, dataaccessrmi::cross::SAVE_CROSS_PERSON_RANK,
		s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}


bool RMIInitCrossPersonRankBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitCrossPersonRankAsyn");
		return false;
	}

	static CrossPersonRankParam cross_person_rank_param;
	cross_person_rank_param.count = 0;

	if (0 == ret)
	{
		if (!cross_person_rank_param.Unserialize(in_param))
		{
			RMIInitError(RMI_INIT_ERROR_RESPONSE, "InitCrossPersonRankAsyn");
			return false;
		}
	}

	this->InitCrossPersonRankRet(ret, cross_person_rank_param);

	return true;
}

void RMIInitCrossPersonRankBackObject::InitCrossPersonRankRet(int ret, const CrossPersonRankParam &cross_person_rank_param)
{
	printf("InitPersonRankRet:%d\n", ret);
}

void RMIInitCrossPersonRankBackObject::__exception(int error)
{
	RMIInitError(RMI_INIT_ERROR_EXCEPTION, "InitCrossPersonRankAsyn");
}

void RMIInitCrossPersonRankBackObject::__timeout()
{
	RMIInitError(RMI_INIT_ERROR_TIMEOUT, "InitCrossPersonRankAsyn");
}

void RMIInitCrossPersonRankBackObject::__free()
{
	delete this;
}

bool RMISaveCrossPersonRankBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	this->SaveCrossPersonRankRet(ret);

	return true;
}

void RMISaveCrossPersonRankBackObject::SaveCrossPersonRankRet(int ret)
{
	printf("SaveCrossPersonRankRet:%d\n", ret);
}

void RMISaveCrossPersonRankBackObject::__exception(int error)
{
	printf("RMISaveCrossPersonRankBackObject SaveError\n");
}

void RMISaveCrossPersonRankBackObject::__timeout()
{
}

void RMISaveCrossPersonRankBackObject::__free()
{
	delete this;
}
////////////////////////////////////////////////////////////////////////////////////////////////

