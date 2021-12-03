
#include "cachecmpnode.h"


CacheCmpNode::CacheCmpNode()
:m_is_set(false), m_like_cmp(CMP_LIKE_TYPE_UNLIKE), m_equal_type(EQUAL_TYPE_EQUAL) 
{
}

CacheCmpNode::CacheCmpNode(int column)
:DataNode(column), m_is_set(false), m_like_cmp(CMP_LIKE_TYPE_UNLIKE), m_equal_type(EQUAL_TYPE_EQUAL)
{

}

CacheCmpNode::CacheCmpNode(const CmpNode &cmp)
:DataNode(cmp),
m_is_set(cmp.IsSet()),
m_like_cmp(cmp.GetLikeCmpType()),
m_equal_type(cmp.GetEqualType())
{
	if (m_is_set)
	{
		switch(m_data_type)
		{
		case DATYPE_INT:
			if (cmp.GetIntSet().size() != 0)
			{
				FASTDB_NS::dbArray<FASTDB_NS::db_int4> int_set(&cmp.GetIntSet()[0], cmp.GetIntSet().size());
				m_int_set = int_set;
			}
			break;
		case DATYPE_LONG64:
			if (cmp.GetLong64Set().size() != 0)
			{
				FASTDB_NS::dbArray<FASTDB_NS::db_int8> long64_set((const FASTDB_NS::db_int8*)&cmp.GetLong64Set()[0], cmp.GetLong64Set().size());
				m_long64_set = long64_set;
			}
			break;
		}
	}
}

CacheCmpNode::~CacheCmpNode()
{
}

EQUAL_TYPE CacheCmpNode::GetEqualType() const
{
	return m_equal_type;
}

CMP_LIKE_TYPE CacheCmpNode::GetLikeCmpType() const
{
	return m_like_cmp;
}
