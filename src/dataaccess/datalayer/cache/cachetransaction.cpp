
#include "cachetransaction.h"
#include "cachemanager.h"
#include "cachecommand.h"

CacheTransaction::CacheTransaction():m_isbegin(false)
{

}

CacheTransaction::~CacheTransaction()
{

}


bool CacheTransaction::begin()
{
	if (CacheManager::Instance()->GetCache()->isOpen())
	{
		CacheManager::Instance()->GetCache()->commit();
	}
	m_isbegin = true;
	return true;
}

bool CacheTransaction::commit()
{
	if (CacheManager::Instance()->GetCache()->isOpen())
	{
		CacheManager::Instance()->GetCache()->commit();
	}
	m_isbegin = false;

	CommandSet::iterator iter;
	for(iter = m_cachedset.begin() ; iter != m_cachedset.end() ; ++iter)
	{
		(*iter)->Commit(this);
	}
	m_cachedset.clear();

	return true;
}

bool CacheTransaction::rollback()
{
	if( CacheManager::Instance()->GetCache()->isOpen())
	{
		CacheManager::Instance()->GetCache()->rollback();
	}
	m_isbegin = false;

	CommandSet::iterator iter;
	for(iter = m_cachedset.begin() ; iter != m_cachedset.end() ; ++iter)
	{
		(*iter)->Rollback(this);
	}
	m_cachedset.clear();

	return true;
}

bool CacheTransaction::isBegin()
{
	return m_isbegin;
}

ITransaction* CacheTransaction::clone()
{
	return new CacheTransaction();
}

void CacheTransaction::InsertCacheComm(CacheBase* cachecomm)
{
	m_cachedset.insert(cachecomm);
}

//void CacheTransaction::Regist()
//{
//	
//}
