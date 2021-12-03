
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

	// ע�⣺�ú�������DataAdapterList�е�Ԫ�ؿռ���Ҫ�ⲿ����Free�ͷţ�����
	virtual int Find(IStatement *stmt, const QueryCondition& query, DataAdapterList *vlist, bool update = false) const=0;
	virtual int Find(IStatement *stmt, const QueryCondition& query, DataAdapter *v, bool update = false) const=0;
	//virtual int Update(IStatement *stmt, const CmpNodeList &cmp, const DataAdapter &v)=0;

	// ע�⣬ʹ��updateʱdata_node_list���ܺ���id�ֶΣ���������cacheʧЧ���������ݳ���
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


	// �����ύ�����ݿ�����ݲ�����flushʱ�ύ�����ݿ�
	MEM_NODE_MAP m_flush_map;

	// �ڴ���־��������й����������ݲ�����¼�ڴˣ�����commitʱ���ó���Ӧ��MEN_NODE�ŵ�m_flush_map��
	TRANSACTION_MAP m_transaction_nodes;

	// ��m_flush_map��m_transaction_nodes�Ĳ������������
	Mutex m_lock;

	// ��ǰ����n�����ݣ�֮��ֻ��Ҫupdate����
	typedef std::list<long long> NagtiveList;
	NagtiveList m_nagtive_list;
	Mutex m_nagtive_lock;

	long long GetNagtive();
private:
	int  GetNagtiveCount();
	bool NagtiveHelper(IStatement *stmt);
};

#endif






