#ifndef __RMI_DEF_HPP__
#define __RMI_DEF_HPP__

#define INSERT_BINARY_DATA_TO_ADAPTER(DATA_ADEPTER, TABLE_FIELD, HEX_TYPE, PARAM)	\
{																		\
	static HEX_TYPE hex_buff##__LINE__;									\
	BinToHex((const char*)&PARAM, sizeof(PARAM), hex_buff##__LINE__);	\
	hex_buff##__LINE__[sizeof(hex_buff##__LINE__) - 1] = '\0';			\
	STRNCPY(DATA_ADEPTER.m_data_area[TABLE_FIELD].vcharp, (const char*)&hex_buff##__LINE__, DATA_ADEPTER.m_data_area[TABLE_FIELD].length);	\
}

#define UPDATE_INT_DATA_NODE(NODENAME, TABLE_FIELD, DATA)				\
{																		\
	DataNode NODENAME(TABLE_FIELD);										\
	NODENAME.SetInt32(DATA);											\
	node_list.push_back(NODENAME);										\
}

#define UPDATE_STRING_DATA_NODE(NODENAME, TABLE_FIELD, DATA)				\
{																		\
	DataNode NODENAME(TABLE_FIELD);										\
	NODENAME.SetString(DATA);											\
	node_list.push_back(NODENAME);										\
}

#define UPDATE_INT64_DATA_NODE(NODENAME, TABLE_FIELD, DATA)				\
{																		\
	DataNode NODENAME(TABLE_FIELD);										\
	NODENAME.SetLong64(DATA);											\
	node_list.push_back(NODENAME);										\
}

#define UPDATE_BINARY_DATA_NODE(NODENAME, TABLE_FIELD, DATA, TYPE)		\
{																		\
	DataNode NODENAME(TABLE_FIELD);										\
	static TYPE hex_buff##__LINE__;										\
	BinToHex((const char *)(&DATA), sizeof(DATA), hex_buff##__LINE__);	\
	hex_buff##__LINE__[sizeof(hex_buff##__LINE__) - 1] = 0;				\
	NODENAME.SetString(hex_buff##__LINE__);								\
	node_list.push_back(NODENAME);										\
}

#define UPDATE_BINARY_DATA_TO_MYSQL(NODENAME, TABLE_FIELD, DATA, TYPE)				\
{																					\
	DataNode NODENAME(TABLE_FIELD);													\
	int out_len = 0;																\
	int buffer_len = sizeof(g_struct_code_buffer);									\
	DATA.SerializeToMysql(buffer_len, g_struct_code_buffer, &out_len);				\
	static TYPE hex_buff##__LINE__;													\
	BinToHex((const char *)g_struct_code_buffer, out_len, hex_buff##__LINE__);		\
	hex_buff##__LINE__[sizeof(hex_buff##__LINE__) - 1] = 0;							\
	NODENAME.SetString(hex_buff##__LINE__);											\
	node_list.push_back(NODENAME);													\
}

#define UPDATE_BINARY_DATA_TO_LIST(NODENAME, TABLE_FIELD, DATA, TYPE, NODELIST)			\
{																						\
	DataNode NODENAME(TABLE_FIELD);														\
	static TYPE hex_buff##__LINE__;														\
	BinToHex((const char *)(&DATA), sizeof(DATA), hex_buff##__LINE__);					\
	hex_buff##__LINE__[sizeof(hex_buff##__LINE__) - 1] = 0;								\
	NODENAME.SetString(hex_buff##__LINE__);												\
	NODELIST.push_back(NODENAME);														\
}

#define GET_BINARY_DATA(DATAADAPTER, TABLE_FIELD, PARAM)								\
{																						\
	PARAM.Reset();																		\
	if (DATAADAPTER.m_data_area[TABLE_FIELD].length > sizeof(PARAM) * 2)				\
	{																					\
		DATAADAPTER.Free();																\
		return -1;																		\
	}																					\
	HexToBin(DATAADAPTER.m_data_area[TABLE_FIELD].vcharp, DATAADAPTER.m_data_area[TABLE_FIELD].length, (char*)&PARAM);	\
}


#include "servercommon/servercommon.h"
#include "dbopera/dataadapter.h"
#include "common/bintohex.h"
#include "dbopera/sqlnode.h"

inline int HexToBinAutoDataPege(DataAdapter &data_attr, DataPageParam &data_page, int tabledef_data, int tabledef_len)\
{
	int ret = 0;
	data_page.len = data_attr.m_data_area[tabledef_len].vint;
	int length = (int)data_attr.m_data_area[tabledef_data].length;
	if (length != data_page.len * 2)
	{
		ret = -1;
	}
	else
	{
		HexToBin(data_attr.m_data_area[tabledef_data].vcharp, length, data_page.data);
	}

	return ret;
}

inline void BinToHexAutoDataPegex(DataNodeList &attr_list, DataPageParam &data_page, int tabledef_data, int tabledef_len)
{
	DataNode len_node(tabledef_len);
	len_node.SetInt32(data_page.len);
	attr_list.push_back(len_node);

	DataNode page_node(tabledef_data);
	static char hex_buff[DATA_PAGE_LENGTH_HEX]; memset(hex_buff, 0, sizeof(hex_buff));
	BinToHex(data_page.data, data_page.len, hex_buff);
	hex_buff[sizeof(hex_buff) - 1] = 0;
	page_node.SetString(hex_buff);
	attr_list.push_back(page_node);
}

#endif // __RMI_DEF_HPP__
