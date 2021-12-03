#include "rmiloginclient.h"
#include "servercommon/darmi/dalogin.h"

#include "servercommon/servercommon.h"

USE_DATAACCESS_VAR();
USE_DATAACCESS_LOGIN_VAR();

static char TLV_BUFF[1024];

bool RMILoginClient::GetGoldAsyn(const char *pname, int role_id, rmi::RMIBackObject *backobj)
{
	if (NULL == pname) return false;

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Pushf("si", pname, role_id);
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::LOGIN, dataaccessrmi::login::GET_ROLE, s, backobj, false, 5000);
	}
	if (!ret)
	{
		delete backobj;
	}

	return ret;
}

bool RMIGetGoldBackObject::__response(TLVUnserializer &out_param)
{
	int authority_type = 0;
	Int64 gold = 0;
	if (!out_param.Pop(&gold) || !out_param.Pop(&authority_type))
	{
		return false;
	}
	GetGoldRet(gold, authority_type);
	return true;
}
void RMIGetGoldBackObject::GetGoldRet(Int64 gold, int authority_type)
{
	printf("GetGoldRet ret:%d\n", (int)gold);
}
void RMIGetGoldBackObject::__free()
{
	delete this;
}

bool RMILoginClient::ChangeGoldAsyn(const char *pname, int role_id, Int64 gold_delta, rmi::RMIBackObject *backobj)
{
	if (NULL == pname) return false;

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Pushf("ski", pname, gold_delta, role_id);
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::LOGIN, dataaccessrmi::login::CHANGE_ROLE, s, backobj, false, 30000);
	}
	if (!ret)
	{
		delete backobj;
	}
	return ret;
}

bool RMIChangeGoldBackObject::__response(TLVUnserializer &out_param)
{
	int result;
	Int64 new_gold;
	if (!out_param.Pop(&result) || !out_param.Pop(&new_gold))
	{
		Error("unseriase param error");
		return false;
	}
	ChangeGoldRet(result, new_gold);
	return true;
}
void RMIChangeGoldBackObject::ChangeGoldRet(int result, Int64 new_gold)
{
	if (result != 0)
	{
		Error("result not 0");
		return;
	}
	printf("ChangeGoldRet ret:%d new_gold:%d\n", result, (int)new_gold);
}
void RMIChangeGoldBackObject::__free()
{
	delete this;
}
void RMIChangeGoldBackObject::__exception(int error)
{
	Error("exception");
}
void RMIChangeGoldBackObject::__timeout()
{
	Error("timeout");
}
void RMIChangeGoldBackObject::Error(const char *err_str)
{
	printf("RMIChangeGoldBackObject::Error:%s\n", err_str);
}

bool RMILoginClient::QueryNameExistAsyn(const char *reset_name, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;

	char buff[64];
	s.Reset(buff, sizeof(buff));

	bool ret = s.Push(reset_name);
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::LOGIN, dataaccessrmi::login::QUERY_NAME_EXIST, s, backobj, true, 5000);
	}

	if (!ret)
	{
		delete backobj;
	}

	return ret;
}

bool RMIQueryNameExistBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1;
	if (!out_param.Pop(&ret))
	{
		return false;
	}

	QueryNameExistRet(ret);

	return true;
}

void RMIQueryNameExistBackObject::__exception(int error)
{
	Error("exception");
}
void RMIQueryNameExistBackObject::__timeout()
{
	Error("timeout");
}
void RMIQueryNameExistBackObject::__free()
{
	delete this;
}
void RMIQueryNameExistBackObject::QueryNameExistRet(int result)
{
	if (result != 0)
	{
		Error("result not 0");
		return;
	}

	printf("RMIQueryNameExistBackObject ret:%d\n", result);
}
void RMIQueryNameExistBackObject::Error(const char *err_str)
{
	printf("RMIQueryNameExistBackObject::Error:%s\n", err_str);
}

bool RMILoginClient::ResetNameAsyn(int db_index, int role_id, const char *reset_name, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;

	char buff[64];
	s.Reset(buff, sizeof(buff));

	bool ret = s.Push(db_index) && s.Push(role_id) && s.Push(reset_name);
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::LOGIN, dataaccessrmi::login::RESET_ROLE_NAME, s, backobj, true, 5000);
	}

	if (!ret)
	{
		delete backobj;
	}

	return ret;
}

bool RMIResetNameBackObject::__response(TLVUnserializer &out_param)
{
	int ret;
	if (!out_param.Pop(&ret))
	{
		return false;
	}

	ResetNameRet(ret);

	return true;
}

void RMIResetNameBackObject::__exception(int error)
{
	Error("exception");
}
void RMIResetNameBackObject::__timeout()
{
	Error("timeout");
}
void RMIResetNameBackObject::__free()
{
	delete this;
}
void RMIResetNameBackObject::ResetNameRet(int result)
{
	if (result != 0)
	{
		Error("result not 0");
		return;
	}

	printf("RMIResetNameBackObject ret:%d\n", result);
}
void RMIResetNameBackObject::Error(const char *err_str)
{
	printf("RMIResetNameBackObject::Error:%s\n", err_str);
}

