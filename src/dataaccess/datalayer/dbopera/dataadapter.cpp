#include "dataadapter.h"
#include "common/memory/clmempool.h"
#include <string.h>


MataTableDef::MataTableDef():m_table_id(-1), m_key_id_index(-1)
{
}
MataTableDef::~MataTableDef()
{
}

bool MataTableDef::Init(const TableInitArg &arg)
{
	m_table_id = arg.table_id;
	m_table_name = arg.table_name;
	m_key_id_index = arg.key_id_index;
	m_mata_data = arg.mata_data;

	for (int i = 0; i < (int)m_mata_data.size(); ++i)
	{
		if (m_mata_data[i].type == DATYPE_STRING)
		{
			m_malloc_area_list.push_back(i);
		}
	}
	
	m_data_prototype.Init(this);

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 内存池
//////////////////////////////////////////////////////////////////////////

namespace damempool
{
	class MsgMemPool: public CLMemPool 
	{
	public:
		MsgMemPool():CLMemPool("DBDAMsgMem")
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

DataAdapter::DataAdapter()
{
}

DataAdapter::DataAdapter(const DataAdapter &_r)
:m_data_area(_r.m_data_area), m_table(_r.m_table)
{
}

DataAdapter::~DataAdapter()
{
}

void DataAdapter::Malloc()
{
	for (int i = 0; i < (int)m_table->m_malloc_area_list.size(); ++i)
	{
		int index = m_table->m_malloc_area_list[i];
		int length = m_table->m_mata_data[index].length;

		int malloc_length = length + 1; // 预防手工插入(没有加0结尾)最长字符
		m_data_area[index].vcharp = (char*)damempool::g_msg_mem_pool.Alloc(malloc_length);
		memset(m_data_area[index].vcharp, 0, malloc_length);

		m_data_area[index].length = length;
	}
}

void DataAdapter::Free()
{
	for (int i = 0; i < (int)m_table->m_malloc_area_list.size(); ++i)
	{
		//free(m_data_area[m_table->m_malloc_area_list[i]].vcharp);
		damempool::g_msg_mem_pool.Free(m_data_area[m_table->m_malloc_area_list[i]].vcharp);
	}
}

void DataAdapter::Init(const MataTableDef *table)
{
	m_table = table;
	m_data_area.resize(m_table->m_mata_data.size());
	for (int i = 0; i < (int)m_table->m_mata_data.size(); ++i)
	{
		m_data_area[i].length = m_table->m_mata_data[i].length;
		m_data_area[i].vint64 = 0;
	}
}




