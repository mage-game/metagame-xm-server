#ifndef RMILOGINCLIENT_H
#define RMILOGINCLIENT_H

#include "rmi/rmibase.h"
#include "servercommon/basedef.h"

class RMILoginClient : public rmi::RMIProxyObject
{
public:
	RMILoginClient(){};
	virtual ~RMILoginClient(){};

	bool GetGoldAsyn(const char *pname, int role_id, rmi::RMIBackObject *backobj);
	bool ChangeGoldAsyn(const char *pname, int role_id, Int64 gold_delta, rmi::RMIBackObject *backobj);
	bool QueryNameExistAsyn(const char *reset_name, rmi::RMIBackObject *backobj);
	bool ResetNameAsyn(int db_index, int role_id, const char *reset_name, rmi::RMIBackObject *backobj);

protected:
};

class RMIGetGoldBackObject : public rmi::RMIBackObject
{
public:
	RMIGetGoldBackObject(){};
	virtual ~RMIGetGoldBackObject(){};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();
protected:
	virtual void GetGoldRet(Int64 gold, int authority_type);
};

class RMIChangeGoldBackObject : public rmi::RMIBackObject
{
public:
	RMIChangeGoldBackObject(){};
	virtual ~RMIChangeGoldBackObject(){};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();
protected:
	virtual void ChangeGoldRet(int result, Int64 new_gold);
	virtual void Error(const char *err_str);
};

class RMIQueryNameExistBackObject : public rmi::RMIBackObject
{
public:
	RMIQueryNameExistBackObject() {}
	virtual ~RMIQueryNameExistBackObject() {}

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void QueryNameExistRet(int result);
	virtual void Error(const char *err_str);
};

class RMIResetNameBackObject : public rmi::RMIBackObject
{
public:
	RMIResetNameBackObject() {}
	virtual ~RMIResetNameBackObject() {}

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void ResetNameRet(int result);
	virtual void Error(const char *err_str);
};


#endif

