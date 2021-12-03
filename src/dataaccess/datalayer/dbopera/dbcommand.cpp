
#include "db/connection.h"
#include "db/statement.h"
#include "db/resultset.h"
#include "db/connpool.h"
#include "cache/cachecommand.h"

#include "sqlmaker.h"
#include "dbcommand.h"

DBCommand::DBCommand():m_table(0), m_cache(0)
{

}

DBCommand::~DBCommand()
{

}

bool DBCommand::Init(const MataTableDef *table, CacheBase *cache)
{
	m_table = table;
	m_cache = cache;

	m_select_base = "select ";
	for (int i = 0; i < (int)table->m_mata_data.size(); ++i)
	{
		m_select_base += table->m_mata_data[i].name + ",";
	}
	m_select_base.resize(m_select_base.size() - 1);

	m_select_base += " from " + table->m_table_name + " ";

	m_update_base = "update " + table->m_table_name + " set ";
	for (int i = 0; i < (int)table->m_mata_data.size(); ++i)
	{
		if (i != m_table->m_key_id_index)
		{
			m_update_base += table->m_mata_data[i].name + "=?,";
		}
	}
	m_update_base.resize(m_update_base.size() - 1);
	m_update_base += " ";

	m_insert_base = "insert into " + table->m_table_name + " (";
	for (int i = 0; i < (int)table->m_mata_data.size(); ++i)
	{
		if (i != m_table->m_key_id_index)
		{
			m_insert_base += table->m_mata_data[i].name + ",";
		}
	}
	m_insert_base.resize(m_insert_base.size() - 1);
	m_insert_base += ") value (";
	for (int i = 0; i < (int)table->m_mata_data.size(); ++i)
	{
		if (i != m_table->m_key_id_index)
		{
			m_insert_base += "?,";
		}
	}
	m_insert_base.resize(m_insert_base.size() - 1);
	m_insert_base += ") ";

	m_delete_base = "delete from " + table->m_table_name + " ";

	//printf("%s\n%s\n%s\n%s\n", m_select_base.c_str(), m_update_base.c_str(), m_insert_base.c_str(), m_delete_base.c_str());

	return true;
}

int DBCommand::Get(IStatement *stmt, long long id , DataAdapter *v, bool update, bool usedCached) const
{
	assert(v->m_table == m_table);

	if (m_table->m_key_id_index == -1)
	{
		return RESULT_NO_PRIMA_KEY;
	}

	if(usedCached && m_cache != 0)
	{
		assert(m_cache->GetTable() == m_table);
		return m_cache->Get(stmt, id, v, update);
	}

	std::string sql = m_select_base + "where " + m_table->m_mata_data[m_table->m_key_id_index].name + " = ?";
	if(update)
	{
		sql += " for update";
	}

	stmt->prepare(sql.c_str());
	stmt->setLong64(0, id);

	IResultset *rs = stmt->executeQuery();
	if (rs == 0)
	{
		return RESULT_CONN_ERROR;
	}

	int ret = RESULT_NO_DATA;
	bool isErr = true;
	if(rs->next(&isErr))
	{
		ReadTable(v, rs);
		ret = RESULT_SUC;
	}
	else if (isErr)
	{
		ret = RESULT_AREA_ERROR;
	}
	delete rs;

	return ret;
}



int DBCommand::Save(IStatement *stmt, DataAdapter *v, bool usedCached) const
{
	assert(v->m_table == m_table);

	if (m_table->m_key_id_index == -1)
	{
		return RESULT_NO_PRIMA_KEY;
	}

	if(usedCached && m_cache != 0)
	{
		assert(m_cache->GetTable() == m_table);
		return m_cache->Save(stmt, v);
	}

	stmt->prepare(m_insert_base.c_str());
	WriteTable(*v, stmt);
	int returnret = RESULT_NO_DATA;
	int ret = stmt->execute();
	if (ret > 0)
	{
		v->m_data_area[m_table->m_key_id_index].vint64 = stmt->getInsertID();
		returnret = RESULT_SUC;
	}
	else if (ret == -1)
	{
		return RESULT_CONN_ERROR;
	}

	return returnret;
}

int DBCommand::Update(IStatement *stmt, const DataAdapter &v, bool usedCached) const
{
	assert(v.m_table == m_table);

	if (m_table->m_key_id_index == -1)
	{
		return RESULT_NO_PRIMA_KEY;
	}

	if(usedCached && m_cache != 0)
	{
		assert(m_cache->GetTable() == m_table);
		return m_cache->Update(stmt, v);
	}

	std::string sql = m_update_base + "where " + m_table->m_mata_data[m_table->m_key_id_index].name + " = ?";
	stmt->prepare(sql.c_str());
	WriteTable(v, stmt);
	stmt->setLong64((int)m_table->m_mata_data.size() - 1, v.m_data_area[m_table->m_key_id_index].vint64);
	int returnret = RESULT_NO_DATA;
	int ret = stmt->execute();
	if (ret > 0)
	{
		returnret = RESULT_SUC;
	}
	else if (ret == -1)
	{
		return RESULT_CONN_ERROR;
	}

	return returnret;
}

int DBCommand::Remove(IStatement *stmt, long long id, bool usedCached) const
{
	if (m_table->m_key_id_index == -1)
	{
		return RESULT_NO_PRIMA_KEY;
	}

	if(usedCached && m_cache != 0)
	{
		assert(m_cache->GetTable() == m_table);
		return m_cache->Remove(stmt, id);
	}

	std::string sql = m_delete_base + "where " + m_table->m_mata_data[m_table->m_key_id_index].name + " = ? ";
	stmt->prepare(sql.c_str());
	stmt->setLong64(0 , id);

	int returnret = RESULT_NO_DATA;
	int ret = stmt->execute();
	if (ret > 0)
	{
		returnret = RESULT_SUC;
	}
	else if (ret == -1)
	{
		return RESULT_CONN_ERROR;
	}

	return returnret;
}

int DBCommand::Find(IStatement *stmt, const QueryCondition& query, DataAdapterList *vlist, bool update, bool usedCached) const
{
	if(usedCached && m_cache != 0)
	{
		assert(m_cache->GetTable() == m_table);
		return m_cache->Find(stmt, query, vlist, update);
	}

	vlist->clear();
	std::string sqlWhere = sqlmaker::MakeQuery(m_table, query.cmp_list);
	if(query.count_type == FIND_COUNT_TYPE_COUNT )
	{
		long long count = 0;
		std::string count_sql = "select count(*) as count1 from " + m_table->m_table_name + sqlWhere;
		stmt->prepare(count_sql.c_str());
		sqlmaker::WriteStmt(0 , query.cmp_list , stmt );
		IResultset *rs = stmt->executeQuery();
		if (rs == 0)
		{
			return RESULT_CONN_ERROR;
		}
		bool isErr = true;
		if(rs->next(&isErr))
		{
			 rs->getLong64(0, &count);
		}
		delete rs;

		return (int)count;
	}

	std::string sql = m_select_base + sqlWhere + sqlmaker::MakeOrder(m_table, query);
	if(update)
	{
		sql += " for update";
	}

	stmt->prepare(sql.c_str());
	sqlmaker::WriteStmt(0, query.cmp_list, stmt);
	IResultset *rs = stmt->executeQuery();
	if (rs == 0)
	{
		return RESULT_CONN_ERROR;
	}
	
	bool isErr = true;
	int count = 0;
	while (rs->next(&isErr))
	{
		DataAdapter node = m_table->GetPrototype();
		node.Malloc();
		ReadTable(&node, rs);
		vlist->push_back(node);
		++count;
	}
	delete rs;

	return count;
}

int DBCommand::Find(IStatement *stmt, const QueryCondition& query, DataAdapter *v, bool update, bool usedCached) const
{
	assert(v->m_table == m_table);

	if(usedCached && m_cache != 0)
	{
		assert(m_cache->GetTable() == m_table);
		return m_cache->Find(stmt, query, v, update);
	}

	std::string sql = m_select_base + sqlmaker::MakeQuery(m_table, query.cmp_list) + sqlmaker::MakeOrder(m_table, query);
	if(update)
	{
		sql += " for update";
	}

	stmt->prepare(sql.c_str());
	sqlmaker::WriteStmt(0 , query.cmp_list , stmt);
	IResultset *rs = stmt->executeQuery();
	if (rs == 0)
	{
		return RESULT_CONN_ERROR;
	}
	int returnret = RESULT_NO_DATA;
	bool isErr = true;
	if(rs->next(&isErr))
	{
		ReadTable(v , rs);
		returnret = RESULT_SUC;
	}
	delete rs;

	return returnret;
}

//int DBCommand::Update(IStatement *stmt, const CmpNodeList &cmp, const DataAdapter &v, bool usedCached) const
//{
//	assert(v.m_table == m_table);
//  if(usedCached && m_cache != 0)
//	{
//		assert(m_cache->GetTable() == m_table);
//		return m_cache->Update(stmt, cmp, v);
//	}
//
//	std::string sql = m_update_base + sqlmaker::MakeQuery(m_table, cmp);
//	stmt->prepare(sql.c_str());
//	WriteTable(v, stmt);
//	int startIndex = (m_table->m_key_id_index == -1) ? (int)m_table->m_mata_data.size() : ((int)m_table->m_mata_data.size() - 1);
//	sqlmaker::WriteStmt(startIndex, cmp, stmt);
//	
//	int count = stmt->execute();
//	if (count == -1)
//	{
//		return RESULT_CONN_ERROR;
//	}
//
//	return count;
//}

int DBCommand::Update(IStatement *stmt, const CmpNodeList &cmp, const DataNodeList &data_node_list, bool usedCached) const
{
	if(usedCached && m_cache != 0)
	{
		assert(m_cache->GetTable() == m_table);
		return m_cache->Update(stmt, cmp, data_node_list);
	}

	std::string sql = "update " + m_table->m_table_name + " ";
	sql += sqlmaker::MakeUpdate(m_table, data_node_list) + sqlmaker::MakeQuery(m_table, cmp);
	stmt->prepare(sql.c_str());
	sqlmaker::WriteStmt(0, data_node_list , stmt);
	sqlmaker::WriteStmt((int)data_node_list.size(), cmp , stmt);
	int count = stmt->execute();
	if (count == -1)
	{
		return RESULT_CONN_ERROR;
	}

	return count;
}

int DBCommand::Remove(IStatement *stmt, const CmpNodeList &cmp, bool usedCached) const
{
	if(usedCached && m_cache != 0)
	{
		assert(m_cache->GetTable() == m_table);
		return m_cache->Remove(stmt, cmp);
	}

	std::string sql = m_delete_base + sqlmaker::MakeQuery(m_table, cmp);
	stmt->prepare(sql.c_str());
	sqlmaker::WriteStmt(0, cmp, stmt);

	int count = stmt->execute();
	if (count == -1)
	{

		return RESULT_CONN_ERROR;
	}

	return count;
}

void DBCommand::ReadTable(DataAdapter *data, IResultset *rs) const
{

	for (int i = 0; i < (int)m_table->m_mata_data.size(); ++i)
	{
		switch (m_table->m_mata_data[i].type)
		{
		case DATYPE_CHAR:
			rs->getByte(i, &data->m_data_area[i].vchar);
			break;
		case DATYPE_INT:
			rs->getInt32(i, &data->m_data_area[i].vint);
			break;
		case DATYPE_LONG64:
			rs->getLong64(i, &data->m_data_area[i].vint64);
			break;
		case DATYPE_DOUBLE:
			rs->getDouble(i, &data->m_data_area[i].vdouble);
			break;
		case DATYPE_STRING:
			{
				unsigned long len = data->m_data_area[i].length;
				rs->getString(i, data->m_data_area[i].vcharp, &len);
				data->m_data_area[i].length = len;
			}
			break;
		case DATYPE_TIME:
			rs->getLong64(i, &data->m_data_area[i].vtime);
			break;
		}
	}
}

void DBCommand::WriteTable(const DataAdapter &data, IStatement *stmt) const
{
	int index = 0;
	for (int i = 0; i < (int)m_table->m_mata_data.size(); ++i)
	{
		if (i != m_table->m_key_id_index)
		{
			switch (m_table->m_mata_data[i].type)
			{
			case DATYPE_CHAR:
				stmt->setByte(index++, data.m_data_area[i].vchar);
				break;
			case DATYPE_INT:
				stmt->setInt32(index++, data.m_data_area[i].vint);
				break;
			case DATYPE_LONG64:
				stmt->setLong64(index++, data.m_data_area[i].vint64);
				break;
			case DATYPE_DOUBLE:
				stmt->setDouble(index++, data.m_data_area[i].vdouble);
				break;
			case DATYPE_STRING:
				stmt->setString(index++, data.m_data_area[i].vcharp, data.m_data_area[i].length);
				break;
			case DATYPE_TIME:
				stmt->setLong64(index++, data.m_data_area[i].vtime);
				break;
			}
		}
	}
}

