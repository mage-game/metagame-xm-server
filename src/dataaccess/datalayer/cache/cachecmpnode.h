
#ifndef CACHECMPNODE_H
#define CACHECMPNODE_H

#include "fastdb.h"
#include "dbopera/sqlnode.h"

class CacheCmpNode: public DataNode
{
public:
	CacheCmpNode();
	CacheCmpNode(const CmpNode &cmp);
	CacheCmpNode(int column);
	virtual ~CacheCmpNode();

	void					SetEqualType(EQUAL_TYPE equal_type);
	EQUAL_TYPE				GetEqualType() const;

	void					SetLikeCmpType(CMP_LIKE_TYPE like_type);
	CMP_LIKE_TYPE			GetLikeCmpType() const;

	void					SetIntSet(const std::vector<int> &int_set);
	FASTDB_NS::dbArray<FASTDB_NS::db_int4>&	GetIntSet() { return m_int_set; }

	void					SetLong64Set(const std::vector<long long> &long64_set);
	FASTDB_NS::dbArray<FASTDB_NS::db_int8>& GetLong64Set() { return m_long64_set; }

	bool					IsSet() const { return m_is_set; }

protected:
	// 检查优先级分别是m_is_set > m_like_cmp > m_equal_type
	bool					m_is_set;
	CMP_LIKE_TYPE			m_like_cmp;
	EQUAL_TYPE				m_equal_type;

	FASTDB_NS::dbArray<FASTDB_NS::db_int4> m_int_set;
	FASTDB_NS::dbArray<FASTDB_NS::db_int8> m_long64_set;
};
typedef std::vector<CacheCmpNode> CacheCmpNodeList;


#endif



