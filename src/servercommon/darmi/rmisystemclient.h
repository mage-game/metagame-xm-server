#ifndef __RMISYSTEMCLIENT_H__
#define __RMISYSTEMCLIENT_H__

#include "rmi/rmibase.h"
#include "servercommon/struct/system/publicnoticelist.h"
#include "servercommon/struct/system/cmdlist.h"

class RMISystemClient : public rmi::RMIProxyObject
{
public:
	RMISystemClient(){};
	virtual ~RMISystemClient(){};

	bool GetPublicNoticeAsyn(rmi::RMIBackObject *backobj);
	
	// �Ӻ�̨���ݿ�ȡ�õ���������� ��ֵ��Ӧcommand���type�ֶ�
	enum CMD_TYPE
	{
		CMD_NONE = 0,
		CMD_TYPE_LOGIN,		// ��½���������������
		CMD_TYPE_GLOBAL,	// ȫ�ַ��������������
	};

	bool GetCmd(char type, rmi::RMIBackObject *backobj);
	bool SwapRole(int role_id1, int role_id2, rmi::RMIBackObject* backobj);
};

class RMIGetPublicNoticeBackObject : public rmi::RMIBackObject
{
public:
	RMIGetPublicNoticeBackObject() {}
	virtual ~RMIGetPublicNoticeBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();

protected:
	virtual void GetPublicNoticeRet(int ret, const PublicNoticeList &notice_list);
};

class RMIGetCmdBackObject : public rmi::RMIBackObject
{
public:
	RMIGetCmdBackObject(){};
	virtual ~RMIGetCmdBackObject(){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();
protected:
	virtual void GetCmdRet(int ret, const CmdList &cmd_list);
};

class RMISwapRoleBackObject : public rmi::RMIBackObject
{
public:
	RMISwapRoleBackObject(){};
	virtual ~RMISwapRoleBackObject(){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();
protected:
	virtual void SwapRoleRet(int ret);
};
#endif // __RMISYSTEMCLIENT_H__



