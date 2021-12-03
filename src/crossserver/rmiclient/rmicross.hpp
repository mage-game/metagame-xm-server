/******************************************************************************************
FileName: rmicross.hpp
Author: dreamwang
Description: cross数据库回调相关定义 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/08/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __RMI_CROSS_HPP__
#define __RMI_CROSS_HPP__

#include "rmi/rmibase.h"

#include "servercommon/struct/cross/crossstatusparam.hpp"
#include "servercommon/struct/global/rankparam.hpp"

class RMICrossClient : public rmi::RMIProxyObject
{
public:
	RMICrossClient() {}
	virtual ~RMICrossClient() {}

	bool InitCrossStatusAsyn(long long id_from, rmi::RMIBackObject *backobj);
	bool SaveCrossStatusAsyn(const CrossStatusParam &CrossStatus_param, rmi::RMIBackObject *backobj);

	bool InitCrossPersonRankAsyn(rmi::RMIBackObject *backobj);
	bool SaveCrossPersonRankAsyn(const CrossPersonRankParam &CrossPersonRank_param, rmi::RMIBackObject *backobj);
};

class RMIInitCrossStatusBackObject : public rmi::RMIBackObject
{
public:
	RMIInitCrossStatusBackObject() {}
	virtual ~RMIInitCrossStatusBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitCrossStatusRet(int ret, const CrossStatusParam &status_param);
	void InitError();
};

class RMISaveCrossStatusBackObject : public rmi::RMIBackObject
{
public:
	RMISaveCrossStatusBackObject() {}
	virtual ~RMISaveCrossStatusBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void SaveCrossStatusRet(int ret);
};


class RMIInitCrossPersonRankBackObject : public rmi::RMIBackObject
{
public:
	RMIInitCrossPersonRankBackObject (){};
	virtual ~RMIInitCrossPersonRankBackObject (){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void InitCrossPersonRankRet(int ret, const CrossPersonRankParam &cross_person_rank_param);
};

class RMISaveCrossPersonRankBackObject : public rmi::RMIBackObject
{
public:
	RMISaveCrossPersonRankBackObject (){};
	virtual ~RMISaveCrossPersonRankBackObject (){};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void SaveCrossPersonRankRet(int ret);
};


#endif  // __RMI_CROSS_HPP__


