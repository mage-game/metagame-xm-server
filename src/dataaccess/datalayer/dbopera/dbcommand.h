
#ifndef DBCOMMAND_H
#define DBCOMMAND_H

#include "dataadapter.h"
#include "sqlnode.h"

class IConnection;
class IResultset;
class IStatement;
class CacheBase;

/*
	使用注意：
	
	1、使用
			bool Get(long long id , DataAdapter *v);
			void Save(DataAdapter *v);
			bool Update(const DataAdapter &v);
			bool Remove(long long id );
		等4个函数时，该表必须有一个 long long 类型的唯一ID索引
*/

class DBCommand
{
public:
	enum
	{
		RESULT_SUC = 0,
		RESULT_NO_DATA=-1,
		RESULT_CONN_ERROR=-2,
		RESULT_NO_PRIMA_KEY=-3,
		RESULT_AREA_ERROR=-4,
	};
	DBCommand();
	~DBCommand();

	bool Init(const MataTableDef *table, CacheBase *cache=0);

	// 使用以下4个函数的前提是该表拥有ID
	int Get(IStatement *stmt, long long id , DataAdapter *v, bool update = false, bool usedCached = true) const;
	int Save(IStatement *stmt, DataAdapter *v, bool usedCached = true) const;
	int Update(IStatement *stmt, const DataAdapter &v, bool usedCached = true) const;
	int Remove(IStatement *stmt, long long id, bool usedCached = true) const;

	// 注意：该函数返回DataAdapterList中的元素空间需要外部调用Free释放！！！
	int Find(IStatement *stmt, const QueryCondition& query, DataAdapterList *vlist, bool update = false, bool usedCached = true) const;
	int Find(IStatement *stmt, const QueryCondition& query, DataAdapter *v, bool update = false, bool usedCached = true) const;

	// 该函数不使用缓存时本身没有问题，但使用缓存的话CacheCommon连id也改了，内存日志会出错，现在貌似也不需要该函数，所以注释掉
	//int Update(IStatement *stmt, const CmpNodeList &cmp, const DataAdapter &v, bool usedCached = true) const;

	// 注意，使用cache时update时data_node_list不能含有id字段，否则轻则cache失效，重则数据出错
	int Update(IStatement *stmt, const CmpNodeList &cmp, const DataNodeList &data_node_list, bool usedCached = true) const;
	int Remove(IStatement *stmt, const CmpNodeList &cmp, bool usedCached = true) const;

	const MataTableDef *GetTable()const { return m_table; }
protected:
	void ReadTable(DataAdapter *data, IResultset *rs) const;
	void WriteTable(const DataAdapter &data, IStatement *stmt) const;
	std::string	m_select_base;
	std::string	m_update_base;
	std::string	m_insert_base;
	std::string	m_delete_base;

	const MataTableDef	*m_table;
	CacheBase			*m_cache;
};


#endif


