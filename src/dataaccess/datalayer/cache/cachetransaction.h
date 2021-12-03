


#ifndef CACHETRANSACTION_H
#define CACHETRANSACTION_H

#include "db/transaction.h"
#include <set>

class CacheBase;

class CacheTransaction:public ITransaction
{
public:
	CacheTransaction();
	virtual ~CacheTransaction();

	virtual bool begin();
	virtual bool commit();
	virtual bool rollback();
	virtual bool isBegin();
	virtual ITransaction* clone();

	void InsertCacheComm(CacheBase* cachecomm);

	//static void Regist();

private:
	typedef std::set<CacheBase*> CommandSet;
	CommandSet m_cachedset;
	bool m_isbegin;
};

#endif

