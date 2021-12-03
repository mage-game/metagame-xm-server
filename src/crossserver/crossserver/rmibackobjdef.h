#ifndef __CROSS_RMIBACKOBJDEF_H__
#define __CROSS_RMIBACKOBJDEF_H__

#include "rmiclient/rmicross.hpp"

class CrossPersonRank;
class CrossStatus;

class RMIInitCrossStatusBackObjectImpl : public RMIInitCrossStatusBackObject
{
public:
	RMIInitCrossStatusBackObjectImpl() : crossstatus(NULL) {}

	void *	operator new(size_t c);
	void	operator delete(void *m);

	CrossStatus *crossstatus;

protected:
	virtual void InitCrossStatusRet(int ret, const CrossStatusParam &crossstatus_param);
};

class RMISaveCrossStatusBackObjectImpl : public RMISaveCrossStatusBackObject
{
public:
	RMISaveCrossStatusBackObjectImpl() {}

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveCrossStatusRet(int ret)
	{
		if (0 != ret)
		{
			printf("RMISaveCrossStatusBackObjectImpl SaveCrossStateRet ret:%d \n", ret);
		}
	}
};

class RMIInitCrossPersonRankBackObjectImpl : public RMIInitCrossPersonRankBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

	CrossPersonRank *cross_person_rank;

protected:
	virtual void InitCrossPersonRankRet(int ret, const CrossPersonRankParam &cross_person_rank_param);
};

class RMISaveCrossPersonRankBackObjectImpl : public RMISaveCrossPersonRankBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveCrossPersonRankRet(int ret)
	{
		if (0 != ret)
		{
			printf("RMISavePersonRankBackObjectImpl SaveCrossPersonRankRet Failed ret:%d\n", ret);
		}
	}
};

#endif


