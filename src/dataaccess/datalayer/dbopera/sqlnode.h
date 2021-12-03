
#ifndef SQLNODE_H
#define SQLNODE_H

#include <string>
#include <vector>

#include "dataadapter.h"


enum EQUAL_TYPE
{
	EQUAL_TYPE_EQUAL = 0x01,
	EQUAL_TYPE_LESS = 0x02,
	EQUAL_TYPE_GRANTER = 0x04,
	EQUAL_TYPE_LESS_EQUAL	= 0x08,
	EQUAL_TYPE_GRANTER_EQUAL= 0x10,
	EQUAL_TYPE_NOT_EQUAL	= 0x20,
};

enum ORDER_TYPE
{
	ORDER_TYPE_ASC,
	ORDER_TYPE_DESC
};

enum CMP_LIKE_TYPE
{
	CMP_LIKE_TYPE_L_LIKE,
	CMP_LIKE_TYPE_R_LIKE,
	CMP_LIKE_TYPE_LIKE,
	CMP_LIKE_TYPE_UNLIKE
};

enum FIND_COUNT_TYPE
{
	FIND_COUNT_TYPE_COUNT,
	FIND_COUNT_TYPE_DATA, 
};

class DataNode
{
public:
	DataNode();
	DataNode(int column);
	virtual ~DataNode();

	void					SetColumn(int column)			{ m_column = column; }
	int						GetColumn() const				{ return m_column;}
	const int &				GetDataType() const				{ return m_data_type; }

	void					SetByte(char v)					{ m_data_type = DATYPE_CHAR; m_char = v;}
	const char &			GetByte() const					{ return m_char; }

	void					SetInt32(int v)					{ m_data_type = DATYPE_INT; m_int = v;}
	const int &				GetInt32() const				{ return m_int; }

	void					SetLong64(long long v)			{ m_data_type = DATYPE_LONG64; m_long64 = v; }
	const long long&		GetLong64() const				{ return m_long64; }

	void					SetDouble(double v)				{ m_data_type = DATYPE_DOUBLE; m_double = v; }
	const double&			GetDouble() const				{ return m_double; }

	void					SetTime(long long v)			{ m_data_type = DATYPE_TIME; m_time = v; }
	const long long&		GetTime() const					{ return m_time; }

	void					SetString(const std::string& v)	{ m_data_type = DATYPE_STRING; m_string = v; }
	const std::string&		GetString() const				{ return m_string; }

	bool operator < (const DataNode& node) const;

protected:
	int						m_column;
	int						m_data_type;

	union
	{
		char				m_char;
		int					m_int;
		long long			m_long64;
		double				m_double;
		long long			m_time;
	};
	std::string				m_string;
};
typedef std::vector<DataNode> DataNodeList;

class CmpNode: public DataNode
{
public:
	CmpNode();
	CmpNode(int column);
	virtual ~CmpNode();

	void					SetEqualType(EQUAL_TYPE equal_type);
	EQUAL_TYPE				GetEqualType() const;

	void					SetLikeCmpType(CMP_LIKE_TYPE like_type);
	CMP_LIKE_TYPE			GetLikeCmpType() const;

	void					SetIntSet(const std::vector<int> &int_set);
	const std::vector<int>&	GetIntSet() const { return m_int_set; }

	void					SetLong64Set(const std::vector<long long> &long64_set);
	const std::vector<long long>& GetLong64Set() const { return m_long64_set; }

	bool					IsSet() const { return m_is_set; }

protected:
	// 检查优先级分别是m_is_set > m_like_cmp > m_equal_type
	bool					m_is_set;
	CMP_LIKE_TYPE			m_like_cmp;
	EQUAL_TYPE				m_equal_type;
	
	std::vector<int>		m_int_set;
	std::vector<long long>	m_long64_set;
};
typedef std::vector<CmpNode> CmpNodeList;

struct OrderType
{
	OrderType(int column , ORDER_TYPE t=ORDER_TYPE_DESC);
	OrderType(const OrderType& order_type);

	int						column;
	ORDER_TYPE				type;
};

typedef std::vector<OrderType> OrderTypeList; 

struct QueryCondition
{
public:
	QueryCondition();
	QueryCondition(int start_i, int end_i, FIND_COUNT_TYPE type=FIND_COUNT_TYPE_DATA);
	
	CmpNodeList				cmp_list;
	OrderTypeList			order_list;
	FIND_COUNT_TYPE			count_type;
	int						start_offset;
	int						query_num;
};

#endif



