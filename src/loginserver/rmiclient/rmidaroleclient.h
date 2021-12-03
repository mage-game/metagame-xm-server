#ifndef RMIDAROLECLIENT_H
#define RMIDAROLECLIENT_H

#include "rmi/rmibase.h"
#include "servercommon/struct/roleinfolist.h"
#include "servercommon/struct/roleinitparam.h"

class RMILRoleClient : public rmi::RMIProxyObject
{
public:
	RMILRoleClient(){};
	virtual ~RMILRoleClient(){};

	bool GetRoleInfoAsyn(int *role_id, int num, rmi::RMIBackObject *backobj);
	bool CreateRoleAsyn(int db_index, const RoleInitParam &p, rmi::RMIBackObject *backobj);
	bool DestroyRoleAsyn(int role_id, rmi::RMIBackObject *backobj);

protected:
};

class RMIGetRoleInfoBackObject : public rmi::RMIBackObject
{
public:
	RMIGetRoleInfoBackObject(){};
	virtual ~RMIGetRoleInfoBackObject(){};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();
protected:
	virtual void GetRoleInfoRet(int ret, const RoleInfoList &role_info);
};

class RMICreateRoleBackObject : public rmi::RMIBackObject
{
public:
	RMICreateRoleBackObject(){};
	virtual ~RMICreateRoleBackObject(){};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error){ Error(); }
	virtual void __timeout(){ Error(); }
	virtual void __free();
protected:
	virtual void CreateRoleRet(int ret, int role_id);
	virtual void Error(){}
};

class RMIDestroyRoleBackObject : public rmi::RMIBackObject
{
public:
	RMIDestroyRoleBackObject(){};
	virtual ~RMIDestroyRoleBackObject(){};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();
protected:
	virtual void DestroyRoleRet(int ret);
};

#endif

