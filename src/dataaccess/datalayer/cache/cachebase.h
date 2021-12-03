
#ifndef CACHEBASE_H
#define CACHEBASE_H

#include <list>
#include <map>

#include "common/platform/thread/mutex.h"

#include "dbopera/dataadapter.h"
#include "dbopera/sqlnode.h"
#include "dbopera/dbcommand.h"
#include "cachenode.h"

class ITransaction;

class CacheBase
{
public:
	typedef CachedNode MEM_NODE;
	typedef std::list<MEM_NODE*> MEM_NODE_LIST;
	typedef std::map<long long,MEM_NODE*> MEM_NODE_MAP;
	typedef std::map<ITransaction* , MEM_NODE_MAP> TRANSACTION_MAP;

	virtual ~CacheBase(){}

	const MataTableDef *GetTable()const { return m_table; }

	virtual bool Init(const MataTableDef *table, const DBCommand *dbcommand)=0;

	virtual int Get(IStatement *stmt, long long id , DataAdapter *v, bool update = false ) const=0;
	virtual int Save(IStatement *stmt, DataAdapter *v)=0;
	virtual int Update(IStatement *stmt, const DataAdapter &v)=0;
	virtual int Remove(IStatement *stmt, long long id )=0;

	// 注意：该函数返回DataAdapterList中的元素空间需要外部调用Free释放！！！
	virtual int Find(IStatement *stmt, const QueryCondition& query, DataAdapterList *vlist, bool update = false) const=0;
	virtual int Find(IStatement *stmt, const QueryCondition& query, DataAdapter *v, bool update = false) const=0;
	//virtual int Update(IStatement *stmt, const CmpNodeList &cmp, const DataAdapter &v)=0;

	// 注意，使用update时data_node_list不能含有id字段，否则轻则cache失效，重则数据出错
	virtual int Update(IStatement *stmt, const CmpNodeList &cmp, const DataNodeList &data_node_list)=0;
	virtual int Remove(IStatement *stmt, const CmpNodeList &cmp)=0;
	
	virtual int RemoveCache(long long id) = 0;
	virtual int RemoveCache(const CmpNodeList &cmp) = 0;

	virtual void Load(IStatement *stmt)=0;
	virtual int Insert(const DataAdapter &v)=0;

	void Flush(IStatement *stmt);
	void Commit(ITransaction *transation);
	void Rollback(ITransaction *transation);
	void Nagtive(IStatement *stmt);
	void Unnagtive(IStatement *stmt);
protected:

	enum ECachedUpdateModel
	{
		ECachedUpdateModelAdd = 0x01,               //to do add
		ECachedUpdateModelDelete = 0x02,            //to do delete
		ECachedUpdateModelUpdate = 0x04,            //to do update
	};

	const DBCommand *m_dbcommand;
	const MataTableDef *m_table;


	// 即将提交到数据库的数据操作，flush时提交到数据库
	MEM_NODE_MAP m_flush_map;

	// 内存日志，事务进行过程中则数据操作记录于此，事务commit时则拿出相应的MEN_NODE放到m_flush_map中
	TRANSACTION_MAP m_transaction_nodes;

	// 对m_flush_map和m_transaction_nodes的操作均用这个锁
	Mutex m_lock;

	// 提前插入n条数据，之后只需要update即可
	typedef std::list<long long> NagtiveList;
	NagtiveList m_nagtive_list;
	Mutex m_nagtive_lock;

	long long GetNagtive();
private:
	int  GetNagtiveCount();
	bool NagtiveHelper(IStatement *stmt);
};

#endif






