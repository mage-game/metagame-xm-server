
#ifndef DBCOMMAND_H
#define DBCOMMAND_H

#include "dataadapter.h"
#include "sqlnode.h"

class IConnection;
class IResultset;
class IStatement;
class CacheBase;

/*
	ʹ��ע�⣺
	
	1��ʹ��
			bool Get(long long id , DataAdapter *v);
			void Save(DataAdapter *v);
			bool Update(const DataAdapter &v);
			bool Remove(long long id );
		��4������ʱ���ñ������һ�� long long ���͵�ΨһID����
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

	// ʹ������4��������ǰ���Ǹñ�ӵ��ID
	int Get(IStatement *stmt, long long id , DataAdapter *v, bool update = false, bool usedCached = true) const;
	int Save(IStatement *stmt, DataAdapter *v, bool usedCached = true) const;
	int Update(IStatement *stmt, const DataAdapter &v, bool usedCached = true) const;
	int Remove(IStatement *stmt, long long id, bool usedCached = true) const;

	// ע�⣺�ú�������DataAdapterList�е�Ԫ�ؿռ���Ҫ�ⲿ����Free�ͷţ�����
	int Find(IStatement *stmt, const QueryCondition& query, DataAdapterList *vlist, bool update = false, bool usedCached = true) const;
	int Find(IStatement *stmt, const QueryCondition& query, DataAdapter *v, bool update = false, bool usedCached = true) const;

	// �ú�����ʹ�û���ʱ����û�����⣬��ʹ�û���Ļ�CacheCommon��idҲ���ˣ��ڴ���־���������ò��Ҳ����Ҫ�ú���������ע�͵�
	//int Update(IStatement *stmt, const CmpNodeList &cmp, const DataAdapter &v, bool usedCached = true) const;

	// ע�⣬ʹ��cacheʱupdateʱdata_node_list���ܺ���id�ֶΣ���������cacheʧЧ���������ݳ���
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


