
#ifndef RMILOGINCLIENT_H
#define RMILOGINCLIENT_H

#include "rmi/rmibase.h"
#include "servercommon/logindef.h"

class RMILoginClient : public rmi::RMIProxyObject
{
public:
	RMILoginClient(){};
	virtual ~RMILoginClient(){};

	bool GetProfNum(rmi::RMIBackObject *backobj);
	bool UserLoginAsyn(const char *pname, char can_insert, rmi::RMIBackObject *backobj);
	bool AddRoleAsyn(const char *pname, int db_index, int role_id, rmi::RMIBackObject *backobj);
	bool RemoveRoleAsyn(const char *pname, int db_index, int role_id, rmi::RMIBackObject *backobj);
	bool Frobid(const char *pname, unsigned int forbid_time, rmi::RMIBackObject *backobj);
	bool FrobidOneRole(const char *pname, int role_id, unsigned int forbid_time, rmi::RMIBackObject *backobj);
	bool AddNameInfoAsyn(const char *role_name, const char *pname, rmi::RMIBackObject *backobj);
	bool UpdateNameInfoAsyn(const char *role_name, int db_index, int role_id, rmi::RMIBackObject *backobj);
	bool DeleteNameInfoAsyn(int db_index, int role_id, const char *pname, rmi::RMIBackObject *backobj);
	bool DeleteNameInfoByNameAsyn(const char *role_name, rmi::RMIBackObject *backobj);
	bool AddIdentityAsyn(const char *pname, const char *name, const char *id, bool check_repeat, rmi::RMIBackObject *backobj);
	bool InitWorldStatus(rmi::RMIBackObject *backobj);
	bool SaveWorldStatus(bool is_register_limit, rmi::RMIBackObject *backobj);
};

class RMIGetProfNumBackObject : public rmi::RMIBackObject
{
public:
	RMIGetProfNumBackObject(){};
	virtual ~RMIGetProfNumBackObject(){};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();
protected:
	virtual void GetProfNumRet(int ret, int prof1_num, int prof2_num, int prof3_num, int prof4_num);
};

class RMIUserLoginBackObject : public rmi::RMIBackObject
{
public:
	RMIUserLoginBackObject(){};
	virtual ~RMIUserLoginBackObject(){};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();
protected:
	virtual void UserLoginRet(int ret, int db_index, int role_1, int role_2, int role_3);
};

class RMIAddRoleBackObject : public rmi::RMIBackObject
{
public:
	RMIAddRoleBackObject(){};
	virtual ~RMIAddRoleBackObject(){};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error){ Error(); }
	virtual void __timeout(){ Error(); }
	virtual void __free();
protected:
	virtual void AddRoleRet(int result);
	virtual void Error(){}
};

class RMIRemoveRoleBackObject : public rmi::RMIBackObject
{
public:
	RMIRemoveRoleBackObject(){};
	virtual ~RMIRemoveRoleBackObject(){};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();
protected:
	virtual void RemoveRoleRet(int result);
};

class RMIPassAntiWallowBackObject : public rmi::RMIBackObject
{
public:
	RMIPassAntiWallowBackObject(){};
	virtual ~RMIPassAntiWallowBackObject(){};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();
protected:
	virtual void PassAntiWallowRet(int ret);
};

class RMIForbidBackObject : public rmi::RMIBackObject
{
public:
	RMIForbidBackObject(){};
	virtual ~RMIForbidBackObject(){};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();
protected:
	virtual void ForbidRet(int result, int db_index, int role_1, int role_2, int role_3);
};

class RMIForbidOneRoleBackObject : public rmi::RMIBackObject
{
public:
	RMIForbidOneRoleBackObject() {};
	virtual ~RMIForbidOneRoleBackObject() {};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error) {}
	virtual void __timeout() {}
	virtual void __free();
protected:
	virtual void ForbidOneRoleRet(int result, int db_index);
};

class RMIAddNameInfoBackObject : public rmi::RMIBackObject
{
public:
	RMIAddNameInfoBackObject(){};
	virtual ~RMIAddNameInfoBackObject(){};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();
protected:
	virtual void AddNameInfoRet(int result);
};

class RMIUpdateNameInfoBackObject : public rmi::RMIBackObject
{
public:
	RMIUpdateNameInfoBackObject(){};
	virtual ~RMIUpdateNameInfoBackObject(){};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();
protected:
	virtual void UpdateNameInfoRet(int result);
};

class RMIDeleteNameInfoBackObject : public rmi::RMIBackObject
{
public:
	RMIDeleteNameInfoBackObject(){};
	virtual ~RMIDeleteNameInfoBackObject(){};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();
protected:
	virtual void DeleteNameInfoRet(int result);
};

class RMIDeleteNameInfoByNameBackObject : public rmi::RMIBackObject
{
public:
	RMIDeleteNameInfoByNameBackObject(){};
	virtual ~RMIDeleteNameInfoByNameBackObject(){};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();
protected:
	virtual void DeleteNameInfoByNameRet(int result);
};

class RMILoginInitWorldStatusBackObject : public rmi::RMIBackObject
{
public:
	RMILoginInitWorldStatusBackObject() {};
	virtual ~RMILoginInitWorldStatusBackObject() {};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error) {};
	virtual void __timeout() {};
	virtual void __free();
protected:
	virtual void InitWorldStatusRet(int result, bool is_register_limit);
};

class RMILoginSaveWorldStatusBackObject : public rmi::RMIBackObject
{
public:
	RMILoginSaveWorldStatusBackObject() {};
	virtual ~RMILoginSaveWorldStatusBackObject() {};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error) {};
	virtual void __timeout() {};
	virtual void __free();
protected:
	virtual void SaveWorldStatusRet(int ret);
};

#endif
