#include "rmisystemclient.h"
#include "servercommon/darmi/dasystem.h"

#include "servercommon/servercommon.h"

USE_DATAACCESS_VAR();
OLD_USE_INTERNAL_NETWORK();

static char TLV_BUFF[1024];

bool RMISystemClient::GetPublicNoticeAsyn(rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = m_session.rmi_module->Call(m_session, dataaccessrmi::SYSTEM, dataaccessrmi::system::GET_PUBLIC_NOTICE, s, backobj, false, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMISystemClient::GetCmd(char type, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK(sizeof(char) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	if (!s.Push(type))
	{
		delete backobj;
		return false;
	}

	bool ret = m_session.rmi_module->Call(m_session, dataaccessrmi::SYSTEM, dataaccessrmi::system::GET_CMD, s, backobj, false, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMISystemClient::SwapRole(int role_id1, int role_id2, rmi::RMIBackObject* backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	if (!s.Push(role_id1))
	{
		delete backobj;
		return false;
	}
	if (!s.Push(role_id2))
	{
		delete backobj;
		return false;
	}

	bool ret = m_session.rmi_module->Call(m_session, dataaccessrmi::SYSTEM, dataaccessrmi::system::SWAP_ROLE, s, backobj, false, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;

}

bool RMIGetPublicNoticeBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;

	if (!in_param.Pop(&ret))
	{
		return false;
	}

	static PublicNoticeList notice_list;
	notice_list.count = 0; 

	if (0 == ret)
	{
		if (!notice_list.Unserialize(in_param))
		{
			return false;
		}
	}

	this->GetPublicNoticeRet(ret, notice_list);

	return true;
}

void RMIGetPublicNoticeBackObject::GetPublicNoticeRet(int ret, const PublicNoticeList &notice_list)
{
	printf("GetPublicNoticeRet:%d\n", ret);
}

void RMIGetPublicNoticeBackObject::__free()
{
	delete this;
}

bool RMIGetCmdBackObject::__response(TLVUnserializer &in_param)
{
	int ret = -1;

	if (!in_param.Pop(&ret))
	{
		return false;
	}

	static CmdList cmd_list;
	cmd_list.count = 0;

	if (0 == ret)
	{
		if (!cmd_list.Unserialize(in_param))
		{
			return false;
		}
	}

	this->GetCmdRet(ret, cmd_list);

	return true;
}

void RMIGetCmdBackObject::GetCmdRet(int ret, const CmdList &cmd_list)
{
	printf("GetCmdRet:%d\n", ret);
}

void RMIGetCmdBackObject::__free()
{
	delete this;
}



bool RMISwapRoleBackObject::__response(TLVUnserializer& in_param)
{
	int ret = 0;
	if (!in_param.Pop(&ret))
	{
		return false;
	}
	SwapRoleRet(ret);
	return true;
}

void RMISwapRoleBackObject::__free()
{
	delete this;
}

void RMISwapRoleBackObject::SwapRoleRet(int ret)
{

}
