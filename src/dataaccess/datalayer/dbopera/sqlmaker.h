
#ifndef SQLMAKER_H
#define SQLMAKER_H

#include "sqlnode.h"

class IStatement;


/*
	使用注意：
		此处只做sql拼接，关于' ` " 等符号，由外部保证过滤
*/

namespace sqlmaker
{
	std::string MakeUpdate(const MataTableDef *table, const DataNodeList &data_node_list);

	std::string MakeQuery(const MataTableDef *table, const CmpNodeList &cmp_node_list);

	std::string MakeOrder(const MataTableDef *table, const QueryCondition &query);

	void WriteStmt(int index, const DataNode &dateNode, IStatement *stmt);

	void WriteStmt(int startIndex, const CmpNodeList &cmp_node_list, IStatement *stmt);

	void WriteStmt(int startIndex, const DataNodeList &data_node_list, IStatement *stmt);
}


#endif



