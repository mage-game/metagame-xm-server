#include "mysqlresult.h"
#include "db/statement.h"
#include "mysqldef.h"

#include "my_global.h"
#include "mysql.h"

#include "common/memory/clmempool.h"

//////////////////////////////////////////////////////////////////////////
// ÄÚ´æ³Ø
//////////////////////////////////////////////////////////////////////////

namespace mrmempool
{
	class MsgMemPool: public CLMemPool 
	{
	public:
		MsgMemPool():CLMemPool("DBResultMsgMem")
		{
			unsigned int length_list[] = {16, 32, 64, 128, 256, 512, 2048, 4096};
			this->Init(length_list, 8, 32);
		}
		virtual ~MsgMemPool()
		{
		}
	};

	MsgMemPool g_msg_mem_pool;
}

//////////////////////////////////////////////////////////////////////////

MysqlResult::MysqlResult()
{
}

MysqlResult::~MysqlResult()
{
	close();
}

IStatement* MysqlResult::getStmt()
{
	return m_stmt;
}
void MysqlResult::setStmt(IStatement* stmt)
{
	m_stmt = stmt;
}

bool MysqlResult::init()
{
	if(0 == m_stmt)
	{
		return false;
	}

	m_metadata = mysql_stmt_result_metadata((MYSQL_STMT*)(m_stmt->getStmt()));
	if (0 == m_metadata)
	{
		return false;
	}

	m_fieldCount = mysql_num_fields(m_metadata);

	if (m_fieldCount > 0)
	{
		m_result = new MYSQL_BIND[m_fieldCount];
		m_fieldIndexInfo = new TypeName[m_fieldCount];

		MYSQL_FIELD* fields = mysql_fetch_fields(m_metadata);
		for( unsigned int i = 0 ;i < m_fieldCount; ++i)
		{
			MYSQL_FIELD* field = &fields[i];

			MYSQL_BIND* bind = &m_result[i];
			memset(bind, 0, sizeof(MYSQL_BIND));
			bind->buffer_type = field->type;
			//bind->is_null = new char;
			//bind->length = new unsigned long;
			bind->is_null = (my_bool*)mrmempool::g_msg_mem_pool.Alloc(sizeof(my_bool));
			bind->length = (unsigned long*)mrmempool::g_msg_mem_pool.Alloc(sizeof(unsigned long));

			DBDATATYPE type = (DBDATATYPE)getDataType((int)field->type);

			m_fieldNameInfo[field->name].type = type;
			m_fieldNameInfo[field->name].index = i;
			m_fieldIndexInfo[i].type = (int)type;
			m_fieldIndexInfo[i].name = field->name;

			switch(type)
			{
			case TYPE_INT:
				//bind->buffer = malloc(sizeof(int));
				bind->buffer = mrmempool::g_msg_mem_pool.Alloc(sizeof(int));
				break;
			case TYPE_STRING:
				//bind->buffer = malloc(field->length);
				bind->buffer = mrmempool::g_msg_mem_pool.Alloc(field->length);
				*bind->length = field->length;
				bind->buffer_length = field->length;
				break;
			case TYPE_DATE:
				//bind->buffer = malloc(sizeof(MYSQL_TIME));
				bind->buffer = mrmempool::g_msg_mem_pool.Alloc(sizeof(MYSQL_TIME));
				break;
			case TYPE_TIME:
				//bind->buffer = malloc(sizeof(MYSQL_TIME));
				bind->buffer = mrmempool::g_msg_mem_pool.Alloc(sizeof(MYSQL_TIME));
				break;
			case TYPE_TIMESTAMP:
				//bind->buffer = malloc(sizeof(MYSQL_TIME));
				bind->buffer = mrmempool::g_msg_mem_pool.Alloc(sizeof(MYSQL_TIME));
				break;
			case TYPE_LONG64:
				//bind->buffer = malloc(sizeof(long long));
				bind->buffer = mrmempool::g_msg_mem_pool.Alloc(sizeof(long long));
				break;
			case TYPE_DOUBLE:
				//bind->buffer = malloc(sizeof(double));
				bind->buffer = mrmempool::g_msg_mem_pool.Alloc(sizeof(double));
				break;
			case TYPE_CHAR:
				//bind->buffer = malloc(sizeof(char));
				bind->buffer = mrmempool::g_msg_mem_pool.Alloc(sizeof(char));
				break;
			default:
				break;
			}
		}
		mysql_stmt_bind_result((MYSQL_STMT*)(m_stmt->getStmt()), m_result);
	}
	return true;
}

void MysqlResult::close()
{	
	clear();
	if(0 != m_stmt)
	{
		m_stmt->close();
		m_stmt = 0;
	}
}

int MysqlResult::getColumnCount()
{
	return m_fieldCount;
}
std::string MysqlResult::getColumnName(int index)
{
	if (!validIndex(index))
	{
		return 0;
	}
	return m_fieldIndexInfo[index].name;
}
bool MysqlResult::getColumnDataType(int index, int* type)
{
	if (!validIndex(index))
	{
		return false;
	}
	*type = m_fieldIndexInfo[index].type;
	return true;
}

int MysqlResult::findColumn(const char* columnName)
{
	std::map<std::string,IndexType>::iterator iter = m_fieldNameInfo.find(columnName);
	if(iter == m_fieldNameInfo.end())
	{
		return -1;
	}
	return (int)iter->second.index;
}

bool MysqlResult::next(bool* isErr)
{
	*isErr=false;
	int ret = mysql_stmt_fetch((MYSQL_STMT*)(m_stmt->getStmt()));
	if(0 == ret)
	{
		return true;
	}
	if(MYSQL_NO_DATA !=  ret)
	{
		*isErr=true;
	}
	return false;
}

bool MysqlResult::isNull(int index, bool* isnull)
{
	if (!validIndex(index))
	{
		return false;
	}
	MYSQL_BIND* bind = &m_result[index];
	*isnull =(0 != *bind->is_null);
	return true;
}

bool MysqlResult::isNull(const char* columnName, bool* isnull)
{
	int index = findColumn(columnName);
	return isNull(index, isnull);
}

bool MysqlResult::getByte(int index, char* ret)
{
	if (!validIndex(index))
	{
		return false;
	}
	MYSQL_BIND* bind = &m_result[index];
	if (bind->buffer_type != MYSQL_TYPE_TINY)
	{
		return false;
	}
	*ret = *(char*)(bind->buffer);
	return true;
}

bool MysqlResult::getByte(const char* columnName, char* ret)
{
	int index = findColumn(columnName);
	return getByte(index, ret);
}

//bool MysqlResult::getBytes(int index, char* ret, int& length)
//{
//	if (!validIndex(index))
//	{
//		return false;
//	}
//
//	MYSQL_BIND* bind = &m_result[index];
//	if (bind->buffer_type != MYSQL_TYPE_BLOB)
//	{
//		return false;
//	}
//
//	unsigned long size = length;
//	if (*bind->is_null)
//	{
//		return false;
//	}
//	else
//	{
//		if (size > *bind->length)
//		{
//			size = *bind->length;
//		}
//	}
//	memcpy(ret, bind->buffer, size);
//	length = (int)size;
//	return true;
//}
//bool MysqlResult::getBytes(const char* columnName, char* ret, int& length)
//{
//	int index = findColumn(columnName);
//	return getBytes(index, ret, length);
//}

bool MysqlResult::getBoolean(int index, bool* ret)
{
	if (!validIndex(index))
	{
		return false;
	}

	MYSQL_BIND* bind = &m_result[index];
	if(bind->buffer_type != MYSQL_TYPE_TINY)
	{
		return false;
	}

	*ret = *(bool*)bind->buffer;
	return true;
}
bool MysqlResult::getBoolean(const char* columnName, bool* ret)
{
	int index = findColumn(columnName);
	return getBoolean(index, ret);
}

bool MysqlResult::getString(int index, char* ret, unsigned long* length)
{
	if (!validIndex(index))
	{
		return false;
	}
	MYSQL_BIND* bind = &m_result[index];
	if ( bind->buffer_type != MYSQL_TYPE_STRING && bind->buffer_type != MYSQL_TYPE_VAR_STRING && bind->buffer_type != MYSQL_TYPE_BLOB)
	{
		return false;
	}

	unsigned long size = *length;
	if (size > *bind->length)
	{
		size = *bind->length;
	}
	memcpy(ret, bind->buffer, size);
	*length = size;
	return true;
}
bool MysqlResult::getString(const char* columnName, char* ret, unsigned long* length)
{
	int index = findColumn(columnName);
	return getString(index, ret, length);
}

bool MysqlResult::getInt32(int index, int* ret)
{
	if (!validIndex(index))
	{
		return false;
	}
	MYSQL_BIND* bind = &m_result[index];
	if(bind->buffer_type != MYSQL_TYPE_LONG)
	{
		return false;
	}
	*ret = *(int*)(bind->buffer);
	return true;
}
bool MysqlResult::getInt32(const char* columnName, int* ret)
{
	int index = findColumn(columnName);
	return getInt32(index, ret);
}

bool MysqlResult::getDouble(int index, double* ret)
{
	if (!validIndex(index))
	{
		return false;
	}

	MYSQL_BIND* bind = &m_result[index];
	if(bind->buffer_type != MYSQL_TYPE_DOUBLE)
	{
		return false;
	}

	*ret = *(double*)(bind->buffer);
	return true;
}
bool MysqlResult::getDouble(const char* columnName, double* ret)
{
	int index = findColumn(columnName);
	return getDouble(index, ret);
}

bool MysqlResult::getDate(int index, tm* ret)
{
	if (!validIndex(index))
	{
		return false;
	}
	MYSQL_BIND* bind = &m_result[index];
	if(bind->buffer_type != MYSQL_TYPE_DATETIME)
	{
		return false;
	}
	ret->tm_year = (*(MYSQL_TIME*)bind->buffer).year;
	ret->tm_mon = (*(MYSQL_TIME*)bind->buffer).month;
	ret->tm_mday = (*(MYSQL_TIME*)bind->buffer).day;
	ret->tm_hour = (*(MYSQL_TIME*)bind->buffer).hour;
	ret->tm_min = (*(MYSQL_TIME*)bind->buffer).minute;
	ret->tm_sec = (*(MYSQL_TIME*)bind->buffer).second;
	return true;
}
bool MysqlResult::getDate(const char* columnName, tm* ret)
{
	int index = findColumn(columnName);
	return getDate(index, ret);
}

bool MysqlResult::getTime(int index, tm* ret)
{
	if (!validIndex(index))
	{
		return false;
	}
	MYSQL_BIND* bind = &m_result[index];
	if(bind->buffer_type != MYSQL_TYPE_TIME)
	{
		return false;
	}
	ret->tm_year = (*(MYSQL_TIME*)bind->buffer).year;
	ret->tm_mon = (*(MYSQL_TIME*)bind->buffer).month;
	ret->tm_mday = (*(MYSQL_TIME*)bind->buffer).day;
	ret->tm_hour = (*(MYSQL_TIME*)bind->buffer).hour;
	ret->tm_min = (*(MYSQL_TIME*)bind->buffer).minute;
	ret->tm_sec = (*(MYSQL_TIME*)bind->buffer).second;
	return true;
}
bool MysqlResult::getTime(const char* columnName, tm* ret)
{
	int index = findColumn(columnName);
	return getTime(index, ret);
}

bool MysqlResult::getTimestamp(int index, tm* ret)
{
	if (!validIndex(index))
	{
		return false;
	}
	MYSQL_BIND* bind = &m_result[index];
	if(bind->buffer_type != MYSQL_TYPE_TIMESTAMP)
	{
		return false;
	}
	ret->tm_year = (*(MYSQL_TIME*)bind->buffer).year;
	ret->tm_mon = (*(MYSQL_TIME*)bind->buffer).month;
	ret->tm_mday = (*(MYSQL_TIME*)bind->buffer).day;
	ret->tm_hour = (*(MYSQL_TIME*)bind->buffer).hour;
	ret->tm_min = (*(MYSQL_TIME*)bind->buffer).minute;
	ret->tm_sec = (*(MYSQL_TIME*)bind->buffer).second;
	return true;
}
bool MysqlResult::getTimestamp(const char* columnName, tm* ret)
{
	int index = findColumn(columnName);
	return getTimestamp(index, ret);
}

bool MysqlResult::getLong64(int index, long long* ret)
{
	if (!validIndex(index))
	{
		return false;
	}

	MYSQL_BIND* bind = &m_result[index];
	if(bind->buffer_type != MYSQL_TYPE_LONGLONG)
	{
		return false;
	}

	*ret = *(long long*)(bind->buffer);
	return true;
}
bool MysqlResult::getLong64(const char* columnName, long long* ret)
{
	int index = findColumn(columnName);
	return getLong64(index, ret);
}

bool MysqlResult::validIndex(int index)
{
	if (index < 0 || (unsigned int)index >= m_fieldCount)
	{
		return false;
	}
	return true;
}

void MysqlResult::cleanParam(MYSQL_BIND* bind)
{
	if(0 != bind->is_null)
	{
		//delete bind->is_null;
		mrmempool::g_msg_mem_pool.Free(bind->is_null);
		bind->is_null = 0;
	}
	if(0 != bind->length)
	{
		//delete bind->length;
		mrmempool::g_msg_mem_pool.Free(bind->length);
		bind->length = 0;
	}
	if(0 != bind->buffer)
	{
		//free(bind->buffer);
		mrmempool::g_msg_mem_pool.Free(bind->buffer);
		bind->buffer = 0;
	}
}

void MysqlResult::clear()
{
	if (0 != m_metadata)
	{
		mysql_free_result(m_metadata);
		m_metadata = 0;
	}
	if (0 != m_fieldIndexInfo)
	{
		delete[] m_fieldIndexInfo;
		m_fieldIndexInfo = 0;
	}
	m_fieldNameInfo.clear();
	if (0 != m_result)
	{
		for (unsigned int i = 0; i < m_fieldCount; ++i)
		{
			cleanParam(&m_result[i]);
		}
		delete[] m_result;
		m_result = 0;
	}
	//if(0 != m_stmt)
	//{
	//	m_stmt->clearResultSet( this );
	//	m_stmt = 0;
	//}
}



