

#ifndef CACHEMAKER_H
#define CACHEMAKER_H

#include "fastdb.h"
#include "dbopera/sqlnode.h"
#include "cachecmpnode.h"

namespace cachemaker
{
	void MakeQuery(const MataTableDef *table, const CmpNodeList &cmpNodes, CacheCmpNodeList &cache_cmp_list, FASTDB_NS::dbQuery *query, FASTDB_NS::db_int8 long64value[64]);
	void MakeOrder(const MataTableDef *table, const OrderTypeList &order_list , FASTDB_NS::dbQuery *query, std::string &sql);
}

#endif
