
#include "cachemaker.h"
USE_FASTDB_NAMESPACE

namespace cachemaker
{
	void MakeQuery(const MataTableDef *table, const CmpNodeList &cmpNodes, CacheCmpNodeList &cache_cmp_list, FASTDB_NS::dbQuery *dbquery, db_int8 long64value[64])
	{
		int long64index = 0;
		for(CmpNodeList::const_iterator iter = cmpNodes.begin(); iter != cmpNodes.end() ; ++iter)
		{
			cache_cmp_list.push_back(CacheCmpNode(*iter));
		}
		for(CacheCmpNodeList::iterator iter = cache_cmp_list.begin() ; iter != cache_cmp_list.end() ; ++iter)
		{
			dbquery->And(table->m_mata_data[iter->GetColumn()].name.c_str());
			if( iter->IsSet())
			{
				dbquery->append(dbQueryElement::qExpression, " in ");
				switch(iter->GetDataType())
				{
				case DATYPE_INT:
					{
						dbquery->add(iter->GetIntSet());
					}
					break;
				case DATYPE_LONG64:
					{
						dbquery->add(iter->GetLong64Set());
					}
					break;
				}
			}
			else
			{
				if(iter->GetLikeCmpType() != CMP_LIKE_TYPE_UNLIKE && iter->GetDataType() == DATYPE_STRING)
				{
					dbquery->append(dbQueryElement::qExpression, " like ");
					switch (iter->GetLikeCmpType())
					{
					case CMP_LIKE_TYPE_L_LIKE:
						iter->SetString(iter->GetString() + "%");
						dbquery->add(iter->GetString());
						break;
					case CMP_LIKE_TYPE_R_LIKE:
						iter->SetString("%" + iter->GetString());
						dbquery->add(iter->GetString());
						break;
					case CMP_LIKE_TYPE_LIKE:
						iter->SetString("%" + iter->GetString() + "%");
						dbquery->add(iter->GetString());
						break;
					default:
						break;
					}
				}
				else
				{
					switch(iter->GetEqualType())
					{
					case EQUAL_TYPE_EQUAL:
						dbquery->append(dbQueryElement::qExpression, "=");
						break;
					case EQUAL_TYPE_LESS:
						dbquery->append(dbQueryElement::qExpression, "<");
						break;
					case EQUAL_TYPE_GRANTER:
						dbquery->append(dbQueryElement::qExpression, ">");
						break;
					case EQUAL_TYPE_LESS_EQUAL:
						dbquery->append(dbQueryElement::qExpression, "<=");
						break;
					case EQUAL_TYPE_GRANTER_EQUAL:
						dbquery->append(dbQueryElement::qExpression, ">=");
						break;
					case EQUAL_TYPE_NOT_EQUAL:
						dbquery->append(dbQueryElement::qExpression, "<>");
						break;
					}

					switch(iter->GetDataType())
					{
					case DATYPE_CHAR:
						dbquery->add(iter->GetByte());
						break;
					case DATYPE_INT:
						dbquery->add(iter->GetInt32());
						break;
					case DATYPE_LONG64:
						{
							long64value[long64index] = (db_int8)iter->GetLong64();
							dbquery->add(long64value[long64index]);
							++long64index;
						}
						break;
					case DATYPE_DOUBLE:
						dbquery->add(iter->GetDouble());
						break;
					case DATYPE_STRING:
						dbquery->add(iter->GetString());
						break;
					case DATYPE_TIME:
						{
							long64value[long64index] = (db_int8)iter->GetLong64();
							dbquery->add(long64value[long64index]);
							++long64index;
						}
						break;
					}
				}
			}
		}
	}

	void MakeOrder(const MataTableDef *table, const OrderTypeList &order_list , FASTDB_NS::dbQuery *query, std::string &sql)
	{
		if(order_list.empty())
		{
			return;
		}
		sql = " order by ";
		for(OrderTypeList::const_iterator iter = order_list.begin(); iter != order_list.end(); ++iter)
		{
			sql += table->m_mata_data[iter->column].name;
			switch(iter->type)
			{
			case ORDER_TYPE_ASC:
				sql += " asc,";
				break;
			case ORDER_TYPE_DESC:
				sql += " desc,";
				break;
			}
		}
		sql.erase(sql.size() - 1);
		query->append(dbQueryElement::qExpression, sql.c_str());
	}

}







