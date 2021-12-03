
#include "cachebase.h"

#include "db/connpool.h"
#include "db/statement.h"
#include "db/connection.h"


int CacheBase::GetNagtiveCount()
{
	return 100;
}

void CacheBase::Nagtive(IStatement *stmt)
{
	m_nagtive_lock.Lock();
	if( (int)m_nagtive_list.size() <= (GetNagtiveCount() / 2) )
	{
		NagtiveHelper(stmt);
	}
	m_nagtive_lock.Unlock();
}

void CacheBase::Unnagtive(IStatement *stmt)
{
	m_nagtive_lock.Lock();

	for(NagtiveList::const_iterator iter = m_nagtive_list.begin() ; iter != m_nagtive_list.end() ; ++iter)
	{
		m_dbcommand->Remove(stmt, *iter, false);
	}
	m_nagtive_list.clear();
	m_nagtive_lock.Unlock();
}

long long CacheBase::GetNagtive()
{
	m_nagtive_lock.Lock();
	bool ret = true;
	if(m_nagtive_list.empty())
	{
		IConnection *conn = ConnPool::Instance()->GetConn();
		if (conn == 0)
		{
			return 0;
		}
		IStatement *stmt_tmp = conn->createStatement();
		conn->begin(false);
		ret = NagtiveHelper(stmt_tmp);
		conn->commit();
		delete stmt_tmp;
		ConnPool::Instance()->PutConn(conn);
	}

	long long nagtive_id = 0;
	if (ret)
	{
		nagtive_id = *m_nagtive_list.begin();
		m_nagtive_list.pop_front();
	}
	
	m_nagtive_lock.Unlock();
	return nagtive_id;
}

bool CacheBase::NagtiveHelper(IStatement *stmt)
{
	DataAdapter t = m_table->GetPrototype();
	t.Malloc();
	for (int i = 0; i < (int)t.m_data_area.size(); ++i)
	{
		if (m_table->m_mata_data[i].type == DATYPE_STRING)
		{
			t.m_data_area[i].length = t.m_data_area[i].length > 1 ? 1 : t.m_data_area[i].length;
		}
	}
	bool ret = true;
	while((int)m_nagtive_list.size() < GetNagtiveCount())
	{
		if (m_dbcommand->Save(stmt, &t, false) != DBCommand::RESULT_SUC)
		{
			ret = false;
			break;
		}
		m_nagtive_list.push_back(t.m_data_area[m_table->m_key_id_index].vint64);
	}
	t.Free();
	return ret;
}

void CacheBase::Flush(IStatement *stmt)
{
	MEM_NODE_MAP flushMap;
	{
		m_lock.Lock();
		if(m_flush_map.size() == 0)
		{
			m_lock.Unlock();
			return;
		}
		m_flush_map.swap(flushMap);
		m_lock.Unlock();
	}
	for(MEM_NODE_MAP::iterator iter = flushMap.begin(); iter != flushMap.end(); ++iter)
	{
		switch(iter->second->GetUpdateMode())
		{
		case ECachedUpdateModelUpdate:
			{
				m_dbcommand->Update(stmt, *iter->second->GetNode(), false);
			}
			break;
		case ECachedUpdateModelDelete:
			{
				DataAdapter *node = iter->second->GetNode();
				m_dbcommand->Remove(stmt, node->m_data_area[m_table->m_key_id_index].vint64, false);
			}
			break;
		}
		iter->second->GetNode()->Free();
		delete iter->second;
	}
}

void CacheBase::Commit(ITransaction* transation)
{
	m_lock.Lock();
	TRANSACTION_MAP::iterator iter = m_transaction_nodes.find(transation);
	if(m_transaction_nodes.end() == iter)
	{
		m_lock.Unlock();
		return;
	}
	for(MEM_NODE_MAP::const_iterator iter1 = iter->second.begin(); iter1 != iter->second.end(); ++iter1)
	{
		MEM_NODE_MAP::iterator iter2 = m_flush_map.find(iter1->first);
		if(iter1->second->GetUpdateMode() & ECachedUpdateModelDelete)
		{
			iter1->second->SetUpdateMode(ECachedUpdateModelDelete);
		}
		else if(iter1->second->GetUpdateMode() & ECachedUpdateModelAdd)
		{
			iter1->second->SetUpdateMode(ECachedUpdateModelUpdate);
		}
		else if(iter1->second->GetUpdateMode() & ECachedUpdateModelUpdate)
		{
			iter1->second->SetUpdateMode(ECachedUpdateModelUpdate);
		}
		if(iter2 == m_flush_map.end())
		{
			m_flush_map[iter1->first] = iter1->second;
		}
		else
		{
			iter2->second->GetNode()->Free();
			delete iter2->second;
			iter2->second = iter1->second;
		}
	}
	m_transaction_nodes.erase(iter);
	m_lock.Unlock();
}

void CacheBase::Rollback(ITransaction* transation)
{
	m_lock.Lock();
	TRANSACTION_MAP::iterator iter = m_transaction_nodes.find(transation);
	if(m_transaction_nodes.end() == iter)
	{
		m_lock.Unlock();
		return;
	}
	for(MEM_NODE_MAP::const_iterator iter1 = iter->second.begin(); iter1 != iter->second.end(); ++iter1)
	{
		if(iter1->second->GetUpdateMode() & ECachedUpdateModelAdd)
		{
			iter1->second->SetUpdateMode(ECachedUpdateModelDelete);
			MEM_NODE_MAP::iterator iter2 = m_flush_map.find(iter1->first);
			if(iter2 == m_flush_map.end())
			{
				m_flush_map[iter1->first] = iter1->second;
			}
			else
			{
				iter2->second->GetNode()->Free();
				delete iter2->second;
				iter2->second = iter1->second;
			}
		}
		else
		{
			iter1->second->GetNode()->Free();
			delete iter1->second;
		}
	}
	m_transaction_nodes.erase(iter);
	m_lock.Unlock();
}

