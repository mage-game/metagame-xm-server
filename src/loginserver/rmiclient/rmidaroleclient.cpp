#include "rmidaroleclient.h"
#include "servercommon/darmi/darole.h"

#include "servercommon/servercommon.h"

USE_DATAACCESS_VAR();
OLD_USE_INTERNAL_NETWORK();
USE_DATAACCESS_ROLE_VAR();

static char TLV_BUFF[160 * 1024];

bool RMILRoleClient::GetRoleInfoAsyn(int *role_id, int num, rmi::RMIBackObject *backobj)
{
	if (NULL == role_id) return false;

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(num);
	if (! ret)
	{
		delete backobj;
		return false;
	}
	for (int i = 0; i < num; ++i)
	{
		ret = s.Push(role_id[i]);
		if (! ret)
		{
			delete backobj;
			return false;
		}
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::ROLE, dataaccessrmi::role::GET_ROLE_INFO, s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
	}
	return ret;
}


bool RMIGetRoleInfoBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1;

	if (out_param.Pop(&ret))
	{
		RoleInfoList info;
		if (ret == 0)
		{
			if (!info.Unserialize(&out_param))
			{
				return false;
			}
		}

		GetRoleInfoRet(ret, info);
		return true;
	}
	return false;
}
void RMIGetRoleInfoBackObject::GetRoleInfoRet(int ret, const RoleInfoList &role_info)
{
	printf("GetRoleInfoRet:%d\n", ret);
}
void RMIGetRoleInfoBackObject::__free()
{
	delete this;
}

bool RMILRoleClient::CreateRoleAsyn(int db_index, const RoleInitParam &p, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK((sizeof(RoleInitParam)) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));
	
	bool ret = s.Push(db_index);
	if (!ret)
	{
		delete backobj;
		return false;
	}
	ret = p.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::ROLE, dataaccessrmi::role::CREATE_ROLE, s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}
bool RMICreateRoleBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1, role_id = 0;
	if (out_param.Pop(&ret) && out_param.Pop(&role_id))
	{
		CreateRoleRet(ret, role_id);
		return true;
	}
	Error();
	return false;
}
void RMICreateRoleBackObject::CreateRoleRet(int ret, int role_id)
{
	printf("CreateRoleRet:%d role_id:%d\n", ret, role_id);
}
void RMICreateRoleBackObject::__free()
{
	delete this;
}

bool RMILRoleClient::DestroyRoleAsyn(int role_id, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK(sizeof(int) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(role_id);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::ROLE, dataaccessrmi::role::DESTROY_ROLE, s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMIDestroyRoleBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1;
	if (out_param.Pop(&ret))
	{
		DestroyRoleRet(ret);
		return true;
	}
	return false;
}
void RMIDestroyRoleBackObject::DestroyRoleRet(int ret)
{
	printf("DestroyRoleRet: %d\n", ret);
}
void RMIDestroyRoleBackObject::__free()
{
	delete this;
}

