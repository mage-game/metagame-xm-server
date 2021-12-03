
#include "sqlnode.h"
#include "dataadapter.h"


DataNode::DataNode()
:m_column(-1), m_data_type(DATYPE_INT), m_long64(0)
{
}

DataNode::DataNode(int column)
:m_column(column), m_data_type(DATYPE_INT), m_long64(0)
{

}

DataNode::~DataNode()
{
}

bool DataNode::operator < (const DataNode& node) const
{
	switch(m_data_type)
	{
	case DATYPE_INT:
		return m_int < node.m_int;
		break;
	case DATYPE_STRING:
		return m_string < node.m_string;
		break;
	case DATYPE_LONG64:
		return m_long64 < node.m_long64;
		break;
	case DATYPE_DOUBLE:
		return m_double < node.m_double;
		break;
	}
	return false;
}

CmpNode::CmpNode()
:m_is_set(false), m_like_cmp(CMP_LIKE_TYPE_UNLIKE), m_equal_type(EQUAL_TYPE_EQUAL)
{
}

CmpNode::CmpNode(int column)
:DataNode(column), m_is_set(false), m_like_cmp(CMP_LIKE_TYPE_UNLIKE), m_equal_type(EQUAL_TYPE_EQUAL)
{

}

CmpNode::~CmpNode()
{
}


void CmpNode::SetEqualType( EQUAL_TYPE equal_type )
{
	m_equal_type = equal_type;
	// 去掉高优先级的
	m_is_set = false;
	m_like_cmp = CMP_LIKE_TYPE_UNLIKE;
}

EQUAL_TYPE CmpNode::GetEqualType() const
{
	return m_equal_type;
}

void CmpNode::SetLikeCmpType( CMP_LIKE_TYPE like_type )
{
	m_like_cmp = like_type;
	// 去掉高优先级的
	m_is_set = false;
}

CMP_LIKE_TYPE CmpNode::GetLikeCmpType() const
{
	return m_like_cmp;
}

void CmpNode::SetIntSet(const std::vector<int> &int_set)
{
	m_data_type = DATYPE_INT;
	m_is_set = true;
	m_int_set = int_set;
}

void CmpNode::SetLong64Set(const std::vector<long long> &long64_set)
{
	m_data_type = DATYPE_LONG64;
	m_is_set = true;
	m_long64_set = long64_set;
}

OrderType::OrderType(int column, ORDER_TYPE t)
:column(column) , type(t)
{
}

OrderType::OrderType(const OrderType& order_type)
:column(order_type.column) , type(order_type.type)
{
}


QueryCondition::QueryCondition():count_type(FIND_COUNT_TYPE_DATA), start_offset(0),query_num(0)
{
}

QueryCondition::QueryCondition(int start_i, int end_i, FIND_COUNT_TYPE type)
:count_type(type), start_offset(start_i), query_num(end_i)
{
}




