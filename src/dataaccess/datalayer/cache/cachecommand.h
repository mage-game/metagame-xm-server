

#ifndef CACHECOMMAND_H
#define CACHECOMMAND_H



#include "dbopera/dataadapter.h"
#include "dbopera/sqlnode.h"
#include "cachenode.h"
#include "cachebase.h"

class IConnection;
class IResultset;
class IStatement;
class ITransaction;
class DBCommand;


template<class T>
class CacheCommand: public CacheBase
{
public:
	CacheCommand();
	virtual ~CacheCommand();
	virtual bool Init(const MataTableDef *table, const DBCommand *dbcommand);
	virtual int Get(IStatement *stmt, long long id , DataAdapter *v, bool update = false ) const;
	virtual int Save(IStatement *stmt, DataAdapter *v);
	virtual int Update(IStatement *stmt, const DataAdapter &v);
	virtual int Remove(IStatement *stmt, long long id );

	// 注意：该函数返回DataAdapterList中的元素空间需要外部调用Free释放！！！
	virtual int Find(IStatement *stmt, const QueryCondition& query, DataAdapterList *vlist, bool update = false) const;
	virtual int Find(IStatement *stmt, const QueryCondition& query, DataAdapter *v, bool update = false) const;
	//virtual int Update(IStatement *stmt, const CmpNodeList &cmp, const DataAdapter &v);
	virtual int Update(IStatement *stmt, const CmpNodeList &cmp, const DataNodeList &data_node_list);
	virtual int Remove(IStatement *stmt, const CmpNodeList &cmp);

	virtual void Load(IStatement *stmt);
	virtual int Insert(const DataAdapter &v);

	virtual int RemoveCache(long long id);
	virtual int RemoveCache(const CmpNodeList &cmp);

protected:
	void RegistToTransaction(ITransaction* tx, const T &table, ECachedUpdateModel model);
};






//////////////////////////////////////////////////////////////////////////
// 模板实现
//////////////////////////////////////////////////////////////////////////


#include "cachemaker.h"
#include "fastdb.h"
#include "dbopera/dbcommand.h"
#include "db/statement.h"
#include "db/connection.h"
#include "db/transaction.h"
#include "db/connpool.h"
#include "cachemanager.h"
#include "cachetransaction.h"

template<class T>
CacheCommand<T>::CacheCommand():CacheBase()
{
	
}

template<class T>
CacheCommand<T>::~CacheCommand()
{
	assert(m_nagtive_list.size() == 0);	// 外部必须保证CacheCommand销毁之前调用Unnagtive
}

template<class T>
bool CacheCommand<T>::Init(const MataTableDef *table, const DBCommand *dbcommand)
{
	assert(table != 0);
	assert(dbcommand != 0);
	assert(dbcommand->GetTable() == table);

	m_table = table;
	m_dbcommand = dbcommand;
	
	return true;
}

template<class T>
int CacheCommand<T>::Get(IStatement *stmt, long long id , DataAdapter *v, bool update) const
{
	std::string strtmp = m_table->m_mata_data[m_table->m_key_id_index].name + "=";
	dbQuery q;
	db_int8 long64value = (db_int8)id;
	q.add(strtmp.c_str()).add(long64value);

	FASTDB_NS::dbDatabase *db = CacheManager::Instance()->GetCache();

	dbCursor<T> cursor(db);
	dbCursorType type = (update ? dbCursorForUpdate : dbCursorViewOnly);

	int n = cursor.select(q , type);
	if(0 == n)
	{
		return DBCommand::RESULT_NO_DATA;
	}

	cursor.get()->Save(v);
	return DBCommand::RESULT_SUC;
}

template<class T>
int CacheCommand<T>::Save(IStatement *stmt, DataAdapter *v)
{
	assert(v->m_table == m_table);

	long long nagive_id = GetNagtive();
	if (nagive_id == 0)
	{
		return DBCommand::RESULT_CONN_ERROR;
	}

	v->m_data_area[m_table->m_key_id_index].vint64 = nagive_id;

	T t;
	t.Update(*v);
	//if( !isNagtiveOnly() )
	{
		FASTDB_NS::dbDatabase *db = CacheManager::Instance()->GetCache();
		dbAnyReference ref;
		db->insertRecord(&T::dbDescriptor, &ref, &t);
	}

	RegistToTransaction(stmt->getConnection()->getTransaction(), t, ECachedUpdateModelAdd);

	return DBCommand::RESULT_SUC;
}

template<class T>
int CacheCommand<T>::Update(IStatement *stmt, const DataAdapter &v)
{
	assert(v.m_table == m_table);

	std::string strtmp = m_table->m_mata_data[m_table->m_key_id_index].name + "=";
	dbQuery q;
	db_int8 long64value = (db_int8)v.m_data_area[m_table->m_key_id_index].vint64;
	q.add(strtmp.c_str()).add(long64value);

	FASTDB_NS::dbDatabase *db = CacheManager::Instance()->GetCache();
	dbCursor<T> cursor(db);
	int n = cursor.select(q , dbCursorForUpdate);
	if(0 == n)
	{
		return DBCommand::RESULT_NO_DATA;
	}

	cursor.get()->Update(v);
	cursor.update();

	RegistToTransaction(stmt->getConnection()->getTransaction(), *cursor.get(), ECachedUpdateModelUpdate);

	return DBCommand::RESULT_SUC;
}

template<class T>
int CacheCommand<T>::Remove(IStatement *stmt, long long id)
{
	std::string strtmp = m_table->m_mata_data[m_table->m_key_id_index].name + "=";
	dbQuery q;
	db_int8 long64value = (db_int8)id;
	q.add(strtmp.c_str()).add(long64value);

	FASTDB_NS::dbDatabase *db = CacheManager::Instance()->GetCache();
	dbCursor<T> cursor(db);
	int n = cursor.select(q , dbCursorForUpdate);
	if( 0 == n )
	{
		return DBCommand::RESULT_NO_DATA;
	}

	do
	{
		RegistToTransaction(stmt->getConnection()->getTransaction(), *cursor.get(), ECachedUpdateModelDelete);
	}
	while(cursor.next());
	cursor.removeAllSelected();

	return DBCommand::RESULT_SUC;
}

template<class T>
int CacheCommand<T>::Find(IStatement *stmt, const QueryCondition& query, DataAdapterList *vlist, bool update) const
{
	vlist->clear();
	FASTDB_NS::dbDatabase *db = CacheManager::Instance()->GetCache();
	dbCursor<T> cursor(db);

	dbQuery dbquery;
	CacheCmpNodeList cache_cmp_list;
	db_int8 long64value[64];
	cachemaker::MakeQuery(m_table, query.cmp_list, cache_cmp_list, &dbquery, long64value);
	std::string sql;
	cachemaker::MakeOrder(m_table, query.order_list, &dbquery, sql);

	dbCursorType type = (update ? dbCursorForUpdate : dbCursorViewOnly);

	int n = cursor.select(dbquery, type);
	if(query.count_type == FIND_COUNT_TYPE_COUNT)
	{
		return n;
	}
	if(0 == n)
	{
		return 0;
	}
	else
	{
		int i = 0;
		n = 0;
		DataAdapter node = m_table->GetPrototype();
		do
		{
			if(query.query_num == 0)
			{
				node.Malloc();
				cursor.get()->Save(&node);
				vlist->push_back(node);
				++n;
			}
			else
			{
				++i;
				if(i > query.start_offset)
				{
					node.Malloc();
					cursor.get()->Save(&node);
					vlist->push_back(node);
					++n;
					if (i - query.start_offset >= query.query_num)
					{
						break;
					}
				}
			}
		}
		while(cursor.next());
	}
	return n;
}

template<class T>
int CacheCommand<T>::Find(IStatement *stmt, const QueryCondition& query, DataAdapter *v, bool update) const
{
	assert(v->m_table == m_table);

	FASTDB_NS::dbDatabase *db = CacheManager::Instance()->GetCache();
	dbCursor<T> cursor(db);
	dbQuery dbquery;
	CacheCmpNodeList cmp_list_tmp;
	db_int8 long64value[64];
	cachemaker::MakeQuery(m_table, query.cmp_list, cmp_list_tmp, &dbquery, long64value);

	std::string sql;
	cachemaker::MakeOrder(m_table, query.order_list, &dbquery, sql);
	dbCursorType type = update ? dbCursorForUpdate : dbCursorViewOnly;

	int n = cursor.select(dbquery, type);
	if( 0 == n )
	{
		return DBCommand::RESULT_NO_DATA;
	}

	cursor.get()->Save(v);

	return DBCommand::RESULT_SUC;
}

//template<class T>
//int CacheCommand<T>::Update(IStatement *stmt, const CmpNodeList &cmp, const DataAdapter &v)
//{
//	assert(v.m_table == m_table);
//	FASTDB_NS::dbDatabase *db = CacheManager::Instance()->GetCache();
//	dbCursor<T> cursor(db);
//	dbQuery dbquery;
//	CacheCmpNodeList cmp_list_tmp;
//	fastdb::db_int8 long64value[64];
//	cachemaker::MakeQuery(m_table, cmp, cmp_list_tmp, &dbquery, long64value);
//	int n = cursor.select(dbquery, dbCursorForUpdate);
//	if(0 == n)
//	{
//		return 0;
//	}
//	else
//	{
//		do
//		{
//			cursor.get()->Update(v);
//			RegistToTransaction(stmt->getConnection()->getTransaction(), *cursor.get(), ECachedUpdateModelUpdate );
//			cursor.update();
//		}
//		while(cursor.next());
//	}
//	return n;
//}

template<class T>
int CacheCommand<T>::Update(IStatement *stmt, const CmpNodeList &cmp, const DataNodeList &data_node_list)
{
	FASTDB_NS::dbDatabase *db = CacheManager::Instance()->GetCache();
	dbCursor<T> cursor(db);

	dbQuery dbquery;
	CacheCmpNodeList cmp_list_tmp;
	db_int8 long64value[64];
	cachemaker::MakeQuery(m_table, cmp, cmp_list_tmp, &dbquery, long64value);
	int n = cursor.select(dbquery, dbCursorForUpdate);
	if( 0 == n )
	{
		return 0;
	}
	else
	{
		DataAdapter datacp = m_table->GetPrototype();
		datacp.Malloc();
		do
		{
			cursor.get()->Save(&datacp);
			for (DataNodeList::const_iterator i = data_node_list.begin(); i != data_node_list.end(); ++i)
			{
				int culumn = i->GetColumn();
				switch(m_table->m_mata_data[culumn].type)
				{
				case DATYPE_CHAR:
					datacp.m_data_area[culumn].vchar = i->GetByte();
					break;
				case DATYPE_INT:
					datacp.m_data_area[culumn].vint = i->GetInt32();
					break;
				case DATYPE_LONG64:
					datacp.m_data_area[culumn].vint64 = i->GetLong64();
					break;
				case DATYPE_DOUBLE:
					datacp.m_data_area[culumn].vdouble = i->GetDouble();
					break;
				case DATYPE_STRING:
					{
						unsigned int char_size = (unsigned int)i->GetString().size() + 1;
						(char_size > m_table->m_mata_data[culumn].length) ? (char_size = m_table->m_mata_data[culumn].length) : 0;
						memcpy(datacp.m_data_area[culumn].vcharp, i->GetString().c_str(), char_size);
					}
					break;
				case DATYPE_TIME:
					datacp.m_data_area[culumn].vtime = i->GetTime();
					break;
				}
			}

			cursor.get()->Update(datacp);
			cursor.update();
			RegistToTransaction(stmt->getConnection()->getTransaction(), *cursor.get(), ECachedUpdateModelUpdate);
		}
		while(cursor.next());

		datacp.Free();
	}
	return n;
}

template<class T>
int CacheCommand<T>::Remove(IStatement *stmt, const CmpNodeList &cmp)
{
	FASTDB_NS::dbDatabase *db = CacheManager::Instance()->GetCache();
	dbCursor<T> cursor(db);

	dbQuery dbquery;
	int n = 0;
	CacheCmpNodeList cache_cmp_list;
	db_int8 long64value[64];
	cachemaker::MakeQuery(m_table, cmp, cache_cmp_list, &dbquery, long64value);
	n = cursor.select(dbquery, dbCursorForUpdate);
	if( n == 0 )
	{
		return 0;
	}
	do
	{
		RegistToTransaction(stmt->getConnection()->getTransaction(), *cursor.get(), ECachedUpdateModelDelete);
	}
	while(cursor.next());
	cursor.removeAllSelected();
	return n;
}




template<class T>
void CacheCommand<T>::RegistToTransaction(ITransaction* tx, const T &table, ECachedUpdateModel model)
{
	m_lock.Lock();

	if(tx->isBegin())
	{
		CacheTransaction *cachetx = (CacheTransaction*)tx;
		cachetx->InsertCacheComm(this);
		TRANSACTION_MAP::iterator iter = m_transaction_nodes.find(tx);
		if(m_transaction_nodes.end() == iter)
		{
			DataAdapter v = m_table->GetPrototype();
			v.Malloc();
			table.Save(&v);
			MEM_NODE *nodePtr = new MEM_NODE(v);
			nodePtr->SetUpdateMode(model);
			m_transaction_nodes[tx][table.GetId()] = nodePtr;
		}
		else
		{
			MEM_NODE_MAP::iterator iter2 = iter->second.find(table.GetId());
			if(iter2 == iter->second.end())
			{
				DataAdapter v = m_table->GetPrototype();
				v.Malloc();
				table.Save(&v);
				MEM_NODE *nodePtr = new MEM_NODE(v);
				nodePtr->SetUpdateMode(model);
				iter->second[table.GetId()] = nodePtr;
			}
			else
			{
				iter2->second->SetUpdateMode(iter2->second->GetUpdateMode() | model);
				table.Save(iter2->second->GetNode());
			}
		}
	}
	else
	{
		MEM_NODE_MAP::iterator iter = m_flush_map.find(table.GetId());
		if(iter == m_flush_map.end())
		{
			DataAdapter v = m_table->GetPrototype();
			v.Malloc();
			table.Save(&v);

			MEM_NODE *nodePtr = new MEM_NODE(v);
			model = ((model == ECachedUpdateModelAdd) ? ECachedUpdateModelUpdate : model);
			nodePtr->SetUpdateMode(model);

			m_flush_map[table.GetId()] = nodePtr;
		}
		else
		{
			model = ((model == ECachedUpdateModelAdd) ? ECachedUpdateModelUpdate : model);

			int mode_new = (iter->second->GetUpdateMode() | model);
			if(mode_new & ECachedUpdateModelDelete)
			{
				mode_new = ECachedUpdateModelDelete;
			}
			else if(mode_new & ECachedUpdateModelAdd)
			{
				mode_new =  ECachedUpdateModelUpdate;
			}
			else if(mode_new & ECachedUpdateModelUpdate)
			{
				mode_new =  ECachedUpdateModelUpdate;
			}
			iter->second->SetUpdateMode(mode_new);
			table.Save(iter->second->GetNode());
		}
	}
	m_lock.Unlock();
}

template<class T>
void CacheCommand<T>::Load(IStatement *stmt)
{
	QueryCondition query;
	DataAdapterList dal;
	T table;
	m_dbcommand->Find(stmt, query, &dal, false, false);

	FASTDB_NS::dbDatabase *db = CacheManager::Instance()->GetCache();

	for (DataAdapterList::iterator i = dal.begin(); i != dal.end(); ++i)
	{
		dbAnyReference ref;
		table.Update(*i);
		db->insertRecord(&T::dbDescriptor, &ref, &table);
		i->Free();
	}
}

template<class T>
int CacheCommand<T>::Insert(const DataAdapter &v)
{
	assert(v.m_table == m_table);

	T t;
	t.Update(v);
	
	FASTDB_NS::dbDatabase *db = CacheManager::Instance()->GetCache();
	dbAnyReference ref;
	db->insertRecord(&T::dbDescriptor, &ref, &t);

	return DBCommand::RESULT_SUC;
}

template<class T>
int CacheCommand<T>::RemoveCache(long long id)
{
	std::string strtmp = m_table->m_mata_data[m_table->m_key_id_index].name + "=";
	dbQuery q;
	db_int8 long64value = (db_int8)id;
	q.add(strtmp.c_str()).add(long64value);

	FASTDB_NS::dbDatabase *db = CacheManager::Instance()->GetCache();
	dbCursor<T> cursor(db);
	int n = cursor.select(q , dbCursorForUpdate);
	if( 0 == n )
	{
		return DBCommand::RESULT_NO_DATA;
	}

	cursor.removeAllSelected();

	return DBCommand::RESULT_SUC;
}

template<class T>
int CacheCommand<T>::RemoveCache(const CmpNodeList &cmp)
{
	FASTDB_NS::dbDatabase *db = CacheManager::Instance()->GetCache();
	dbCursor<T> cursor(db);

	dbQuery dbquery;
	int n = 0;
	CacheCmpNodeList cache_cmp_list;
	db_int8 long64value[64];
	cachemaker::MakeQuery(m_table, cmp, cache_cmp_list, &dbquery, long64value);
	n = cursor.select(dbquery, dbCursorForUpdate);
	if( n == 0 )
	{
		return 0;
	}
	
	cursor.removeAllSelected();

	return n;
}

#endif



