#include "rmicross.hpp"

#include "servercommon/darmi/dacross.hpp"
#include "servercommon/darmi/damodule.h"
#include "servercommon/struct/structcommon.h"

#include "databaselayer.h"
#include "engineadapter.h"

#include "servercommon/serverclmempool.h"
#include "servercommon/translateint.h"

#include "dataaccesslog.hpp"

#include "common/bintohex.h"

USE_DATAACCESS_VAR();
OLD_USE_INTERNAL_NETWORK();

RMICrossObject::RMICrossObject()
{
	m_method_list[dataaccessrmi::cross::INIT_CROSSSTATUS] = __InitCrossStatus;
	m_method_list[dataaccessrmi::cross::SAVE_CROSSSTATUS] = __SaveCrossStatus;
	m_method_list[dataaccessrmi::cross::INIT_CROSS_PERSON_RANK] = __InitCrossPersonRank;
	m_method_list[dataaccessrmi::cross::SAVE_CROSS_PERSON_RANK] = __SaveCrossPersonRank;
}

RMICrossObject::~RMICrossObject()
{
}

void RMICrossObject::__free()
{
	delete this;
}

const char * RMICrossObject::__get_obj_id()
{
	return dataaccessrmi::CROSS;
}

int RMICrossObject::__InitCrossStatus(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(CrossStatusParam) * 2)
	{
		//dataaccesslog::g_log_cross.printf(LL_CRITICAL, "RMICrossObject::__InitCrossStatus_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	long long id_from = 0;

	if (!in_param.Pop(&id_from))
	{
		//dataaccesslog::g_log_cross.printf(LL_CRITICAL, "RMICrossObject::__InitCrossStatus_2");
		return rmi::DispatchParamError;
	}

	static CrossStatusParam crossstatus_param;
	crossstatus_param.Reset();

	int result = InitCrossStatus(id_from, &crossstatus_param);

	if (!out_param->Push(result))
	{
		//dataaccesslog::g_log_cross.printf(LL_CRITICAL, "RMICrossObject::__InitCrossStatus_3 result %d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!crossstatus_param.Serialize(*out_param))
		{
			//dataaccesslog::g_log_cross.printf(LL_CRITICAL, "RMICrossObject::__InitCrossStatus_4 result %d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else 
	{
		//dataaccesslog::g_log_cross.printf(LL_CRITICAL, "RMICrossObject::__InitCrossStatus_5 result %d", result);
	}

	return rmi::DispatchOK;	
}

int RMICrossObject::InitCrossStatus(long long id_from, CrossStatusParam *crossstatus_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *crossstatus_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_CROSSSTATUS);

	QueryCondition crossstatus_cond;
	CmpNode crossstatus_id_node(tabledef::CROSSSTATUS_IDCROSSSTATUS);
	crossstatus_id_node.SetLong64(id_from);
	crossstatus_id_node.SetEqualType(EQUAL_TYPE_GRANTER);
	crossstatus_cond.cmp_list.push_back(crossstatus_id_node);
	crossstatus_cond.query_num = 1;

	DataAdapterList crossstate_list;
	int ret = crossstatus_command->Find(stmt, crossstatus_cond, &crossstate_list);
	if (ret < 0)
	{
		return ret;
	}

	ret = 0;

	DataAdapterList::iterator i = crossstate_list.begin();
	for (; i != crossstate_list.end(); ++i)
	{
		{
			crossstatus_param->last_update_dayid = i->m_data_area[tabledef::CROSSSTATUS_LAST_UPDATE_DAYID].vint;
		}

		{
			memset(&crossstatus_param->cross_person_rank_title_param, 0, sizeof(crossstatus_param->cross_person_rank_title_param));

			int length = i->m_data_area[tabledef::CROSSSTATUS_CROSS_PERSON_RANK_TITLE_DATA].length;
			if (length <= (int)sizeof(crossstatus_param->cross_person_rank_title_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::CROSSSTATUS_CROSS_PERSON_RANK_TITLE_DATA].vcharp, length, (char*)&crossstatus_param->cross_person_rank_title_param);
			}
		}

		{
			memset(&crossstatus_param->cross_activity_param, 0, sizeof(crossstatus_param->cross_activity_param));

			int length = i->m_data_area[tabledef::CROSSSTATUS_CROSS_ACTIVITY_DATA].length;
			if (length <= (int)sizeof(crossstatus_param->cross_activity_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::CROSSSTATUS_CROSS_ACTIVITY_DATA].vcharp, length, (char*)&crossstatus_param->cross_activity_param);
			}
		}

		break;
	}

	for (; i != crossstate_list.end(); ++i)
	{
		i->Free();
	}

	return ret < 0 ? ret : 0;
}

int RMICrossObject::__SaveCrossStatus(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		//dataaccesslog::g_log_cross.printf(LL_CRITICAL, "RMICrossObject::__SaveCrossStatus_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static CrossStatusParam crossstatus_param;

	if (!crossstatus_param.Unserialize(in_param))
	{
		//dataaccesslog::g_log_cross.printf(LL_CRITICAL, "RMICrossObject::__SaveCrossStatus_2");
		return rmi::DispatchParamError;
	}

	int result = SaveCrossStatus(crossstatus_param);
	if (0 != result)
	{
		//dataaccesslog::g_log_cross.printf(LL_CRITICAL, "RMICrossObject::__SaveCrossStatus_3 result:%d", result);
	}

	if (!out_param->Push(result))
	{
		//dataaccesslog::g_log_cross.printf(LL_CRITICAL, "v::__SaveCrossStatus_4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMICrossObject::SaveCrossStatus(const CrossStatusParam &crossstatus_param)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	const MataTableDef *crossstatus_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_CROSSSTATUS);
	DBCommand *crossstatus_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_CROSSSTATUS);

	DataNodeList crossstatus_item_list;
	crossstatus_item_list.reserve(tabledef::TABLE_CROSSSTATUS_MAXINDEX);

	DataNode last_update_dayid_node(tabledef::CROSSSTATUS_LAST_UPDATE_DAYID);
	last_update_dayid_node.SetInt32(crossstatus_param.last_update_dayid);
	crossstatus_item_list.push_back(last_update_dayid_node);

	static CrossPersonRankTitleParamHex cross_person_rank_title_hex;
	BinToHex((char*)&crossstatus_param.cross_person_rank_title_param, sizeof(crossstatus_param.cross_person_rank_title_param), cross_person_rank_title_hex);
	cross_person_rank_title_hex[sizeof(cross_person_rank_title_hex) - 1] = 0;

	DataNode cross_person_rank_title_node(tabledef::CROSSSTATUS_CROSS_PERSON_RANK_TITLE_DATA);
	cross_person_rank_title_node.SetString(cross_person_rank_title_hex);
	crossstatus_item_list.push_back(cross_person_rank_title_node);

	{
		static CrossActivityParamHex cross_activity_status_hex;
		BinToHex((char*)&crossstatus_param.cross_activity_param, sizeof(crossstatus_param.cross_activity_param), cross_activity_status_hex);
		cross_activity_status_hex[sizeof(cross_activity_status_hex) - 1] = 0;

		DataNode cross_activity_status_node(tabledef::CROSSSTATUS_CROSS_ACTIVITY_DATA);
		cross_activity_status_node.SetString(cross_activity_status_hex);
		crossstatus_item_list.push_back(cross_activity_status_node);
	}

	CmpNodeList condition;

	CmpNode for_index_node(tabledef::CROSSSTATUS_FOR_INDEX);
	for_index_node.SetLong64(1);  // 仅有一条记录 
	condition.push_back(for_index_node);

	int count = crossstatus_command->Update(stmt, condition, crossstatus_item_list);
	if (count < 0)
	{
		ret = -1;
	}
	else if (0 == count)
	{
		DataAdapter crossstatus_item = crossstatus_table->GetPrototype();
		crossstatus_item.Malloc();
		
		crossstatus_item.m_data_area[tabledef::CROSSSTATUS_FOR_INDEX].vint64 = 1;
		crossstatus_item.m_data_area[tabledef::CROSSSTATUS_LAST_UPDATE_DAYID].vint = crossstatus_param.last_update_dayid;
	
		if (DBCommand::RESULT_SUC != crossstatus_command->Save(stmt, &crossstatus_item))
		{
			ret = -2;
		}

		crossstatus_item.Free();
	}

	return ret < 0 ? ret : 0;
}

int RMICrossObject::__InitCrossPersonRank(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(CrossPersonRankParam) * 2)
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	static CrossPersonRankParam cross_person_rank_param;
	cross_person_rank_param.count = 0;

	int result = InitCrossPersonRank(&cross_person_rank_param);

	if (!out_param->Push(result))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!cross_person_rank_param.Serialize(*out_param))
		{
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else 
	{

	}

	return rmi::DispatchOK;
}	

int RMICrossObject::InitCrossPersonRank(CrossPersonRankParam *cross_person_rank_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *rank_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_CROSS_PERSONRANK);

	QueryCondition rank_condition;
	CmpNode rank_cmp(tabledef::CROSS_PERSONRANK_RANK_INDEX);
	rank_cmp.SetInt32(0);
	rank_cmp.SetEqualType(EQUAL_TYPE_GRANTER_EQUAL);
	rank_condition.cmp_list.push_back(rank_cmp);

	CmpNode uid_cmp(tabledef::CROSS_PERSONRANK_UUID);
	uid_cmp.SetLong64(0);
	uid_cmp.SetEqualType(EQUAL_TYPE_GRANTER);
	rank_condition.cmp_list.push_back(uid_cmp);
	rank_condition.query_num = CROSS_PERSON_RANK_TYPE_MAX * CROSS_PERSON_RANK_MAX_SIZE;

	DataAdapterList rank_list;
	int ret = rank_command->Find(stmt, rank_condition, &rank_list);
	if (ret < 0)
	{
		cross_person_rank_param->count = 0;
		return -1;
	}

	int count = 0;
	DataAdapterList::iterator i = rank_list.begin(); 

	for (; i != rank_list.end() && count < (CROSS_PERSON_RANK_TYPE_MAX * CROSS_PERSON_RANK_MAX_SIZE); ++i)
	{
		cross_person_rank_param->cross_person_rank_list[count].change_state = structcommon::CS_NONE;
		cross_person_rank_param->cross_person_rank_list[count].index = i->m_data_area[tabledef::CROSS_PERSONRANK_RANK_INDEX].vint;
		cross_person_rank_param->cross_person_rank_list[count].uuid = i->m_data_area[tabledef::CROSS_PERSONRANK_UUID].vint64;
		STRNCPY(cross_person_rank_param->cross_person_rank_list[count].user_name, i->m_data_area[tabledef::CROSS_PERSONRANK_USER_NAME].vcharp,
			sizeof(cross_person_rank_param->cross_person_rank_list[0].user_name));
		cross_person_rank_param->cross_person_rank_list[count].sex = i->m_data_area[tabledef::CROSS_PERSONRANK_ROLE_SEX].vint;
		cross_person_rank_param->cross_person_rank_list[count].prof = i->m_data_area[tabledef::CROSS_PERSONRANK_ROLE_PROF].vint;
		cross_person_rank_param->cross_person_rank_list[count].camp = i->m_data_area[tabledef::CROSS_PERSONRANK_ROLE_CAMP].vint;

		cross_person_rank_param->cross_person_rank_list[count].exp = i->m_data_area[tabledef::CROSS_PERSONRANK_ROLE_EXP].vint64;
		cross_person_rank_param->cross_person_rank_list[count].level = i->m_data_area[tabledef::CROSS_PERSONRANK_ROLE_LEVEL].vint;
		cross_person_rank_param->cross_person_rank_list[count].rank_type = i->m_data_area[tabledef::CROSS_PERSONRANK_RANK_TYPE].vint;
		cross_person_rank_param->cross_person_rank_list[count].rank_value = i->m_data_area[tabledef::CROSS_PERSONRANK_RANK_VALUE].vint64;
		cross_person_rank_param->cross_person_rank_list[count].record_index = i->m_data_area[tabledef::CROSS_PERSONRANK_RECORD_INDEX].vint;
		cross_person_rank_param->cross_person_rank_list[count].flexible_ll = i->m_data_area[tabledef::CROSS_PERSONRANK_FLEXIBLE_LL].vint64;
		cross_person_rank_param->cross_person_rank_list[count].flexible_int = i->m_data_area[tabledef::CROSS_PERSONRANK_FLEXIBLE_INT].vint;
		cross_person_rank_param->cross_person_rank_list[count].vip_level = i->m_data_area[tabledef::CROSS_PERSONRANK_VIP_LEVEL].vint;
		STRNCPY(cross_person_rank_param->cross_person_rank_list[count].flexible_name, i->m_data_area[tabledef::CROSS_PERSONRANK_FLEXIBLE_NAME].vcharp,
			sizeof(cross_person_rank_param->cross_person_rank_list[0].flexible_name));

		++count;
		i->Free();
	}

	for (; i != rank_list.end(); ++i)
	{
		i->Free();
	}

	cross_person_rank_param->count = count;

	return ret < 0 ? ret : 0;
}

int RMICrossObject::__SaveCrossPersonRank(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	static CrossPersonRankParam cross_person_rank_param;
	cross_person_rank_param.count = 0;

	if (!cross_person_rank_param.Unserialize(in_param))
	{
		return rmi::DispatchParamError;
	}

	int result = SaveCrossPersonRank(cross_person_rank_param);

	if (0 != result)
	{
	}

	if (!out_param->Push(result))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMICrossObject::SaveCrossPersonRank(const CrossPersonRankParam &cross_person_rank_param)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	const MataTableDef *rank_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_CROSS_PERSONRANK);
	DBCommand *rank_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_CROSS_PERSONRANK);

	conn->begin();

	for (int i = 0; i < cross_person_rank_param.count && i < CROSS_PERSON_RANK_TYPE_MAX * CROSS_PERSON_RANK_MAX_SIZE; ++i)
	{
		switch (cross_person_rank_param.cross_person_rank_list[i].change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_nodes;

				CmpNode index_node(tabledef::CROSS_PERSONRANK_RANK_INDEX);
				index_node.SetInt32(cross_person_rank_param.cross_person_rank_list[i].index);
				cmp_nodes.push_back(index_node);

				int count = rank_command->Remove(stmt, cmp_nodes);
				if (count < 1)
				{
					ret = -1;
				}
			}
			break; // case structcommon::CS_DELETE

		case structcommon::CS_INSERT:
			{
				DataAdapter rank_item = rank_table->GetPrototype();
				rank_item.Malloc();

				rank_item.m_data_area[tabledef::CROSS_PERSONRANK_RANK_INDEX].vint = cross_person_rank_param.cross_person_rank_list[i].index;
				rank_item.m_data_area[tabledef::CROSS_PERSONRANK_UUID].vint64 = cross_person_rank_param.cross_person_rank_list[i].uuid;
				STRNCPY(rank_item.m_data_area[tabledef::CROSS_PERSONRANK_USER_NAME].vcharp, cross_person_rank_param.cross_person_rank_list[i].user_name,
					rank_item.m_data_area[tabledef::CROSS_PERSONRANK_USER_NAME].length);

				rank_item.m_data_area[tabledef::CROSS_PERSONRANK_ROLE_SEX].vint = cross_person_rank_param.cross_person_rank_list[i].sex;
				rank_item.m_data_area[tabledef::CROSS_PERSONRANK_ROLE_PROF].vint = cross_person_rank_param.cross_person_rank_list[i].prof;
				rank_item.m_data_area[tabledef::CROSS_PERSONRANK_ROLE_CAMP].vint = cross_person_rank_param.cross_person_rank_list[i].camp;

				rank_item.m_data_area[tabledef::CROSS_PERSONRANK_ROLE_EXP].vint64 = cross_person_rank_param.cross_person_rank_list[i].exp;
				rank_item.m_data_area[tabledef::CROSS_PERSONRANK_ROLE_LEVEL].vint = cross_person_rank_param.cross_person_rank_list[i].level;
				rank_item.m_data_area[tabledef::CROSS_PERSONRANK_RANK_TYPE].vint = cross_person_rank_param.cross_person_rank_list[i].rank_type;
				rank_item.m_data_area[tabledef::CROSS_PERSONRANK_RANK_VALUE].vint64 = cross_person_rank_param.cross_person_rank_list[i].rank_value;
				rank_item.m_data_area[tabledef::CROSS_PERSONRANK_RECORD_INDEX].vint = cross_person_rank_param.cross_person_rank_list[i].record_index;
				rank_item.m_data_area[tabledef::CROSS_PERSONRANK_FLEXIBLE_LL].vint64 = cross_person_rank_param.cross_person_rank_list[i].flexible_ll;
				rank_item.m_data_area[tabledef::CROSS_PERSONRANK_FLEXIBLE_INT].vint = cross_person_rank_param.cross_person_rank_list[i].flexible_int;
				rank_item.m_data_area[tabledef::CROSS_PERSONRANK_VIP_LEVEL].vint = cross_person_rank_param.cross_person_rank_list[i].vip_level;
				STRNCPY(rank_item.m_data_area[tabledef::CROSS_PERSONRANK_FLEXIBLE_NAME].vcharp, cross_person_rank_param.cross_person_rank_list[i].flexible_name,
					rank_item.m_data_area[tabledef::CROSS_PERSONRANK_FLEXIBLE_NAME].length);
			
				if (DBCommand::RESULT_SUC != rank_command->Save(stmt, &rank_item))
				{
					ret = -2;
				}

				rank_item.Free();
			}
			break; // case structcommon::CS_INSERT

		case structcommon::CS_UPDATE:
			{
				DataNodeList set_nodes;

				DataNode uid_node(tabledef::CROSS_PERSONRANK_UUID);
				uid_node.SetLong64(cross_person_rank_param.cross_person_rank_list[i].uuid);
				set_nodes.push_back(uid_node);

				DataNode user_name_node(tabledef::CROSS_PERSONRANK_USER_NAME);
				user_name_node.SetString((const char *)cross_person_rank_param.cross_person_rank_list[i].user_name);
				set_nodes.push_back(user_name_node);

				DataNode role_gender_node(tabledef::CROSS_PERSONRANK_ROLE_SEX);
				role_gender_node.SetInt32(cross_person_rank_param.cross_person_rank_list[i].sex);
				set_nodes.push_back(role_gender_node);

				DataNode role_prof_node(tabledef::CROSS_PERSONRANK_ROLE_PROF);
				role_prof_node.SetInt32(cross_person_rank_param.cross_person_rank_list[i].prof);
				set_nodes.push_back(role_prof_node);

				DataNode role_camp_node(tabledef::CROSS_PERSONRANK_ROLE_CAMP);
				role_camp_node.SetInt32(cross_person_rank_param.cross_person_rank_list[i].camp);
				set_nodes.push_back(role_camp_node);

				DataNode role_exp_node(tabledef::CROSS_PERSONRANK_ROLE_EXP);
				role_exp_node.SetLong64((cross_person_rank_param.cross_person_rank_list[i].exp));
				set_nodes.push_back(role_exp_node);

				DataNode role_level_node(tabledef::CROSS_PERSONRANK_ROLE_LEVEL);
				role_level_node.SetInt32((int)(cross_person_rank_param.cross_person_rank_list[i].level));
				set_nodes.push_back(role_level_node);

				DataNode rank_type_node(tabledef::CROSS_PERSONRANK_RANK_TYPE);
				rank_type_node.SetInt32(cross_person_rank_param.cross_person_rank_list[i].rank_type);
				set_nodes.push_back(rank_type_node);

				DataNode rank_value_node(tabledef::CROSS_PERSONRANK_RANK_VALUE);
				rank_value_node.SetLong64(cross_person_rank_param.cross_person_rank_list[i].rank_value);
				set_nodes.push_back(rank_value_node);

				DataNode record_index_node(tabledef::CROSS_PERSONRANK_RECORD_INDEX);
				record_index_node.SetInt32(cross_person_rank_param.cross_person_rank_list[i].record_index);
				set_nodes.push_back(record_index_node);

				DataNode flexible_ll_node(tabledef::CROSS_PERSONRANK_FLEXIBLE_LL);
				flexible_ll_node.SetLong64(cross_person_rank_param.cross_person_rank_list[i].flexible_ll);
				set_nodes.push_back(flexible_ll_node);

				DataNode flexible_int_node(tabledef::CROSS_PERSONRANK_FLEXIBLE_INT);
				flexible_int_node.SetInt32(cross_person_rank_param.cross_person_rank_list[i].flexible_int);
				set_nodes.push_back(flexible_int_node);

				DataNode vip_level_node(tabledef::CROSS_PERSONRANK_VIP_LEVEL);
				vip_level_node.SetInt32(cross_person_rank_param.cross_person_rank_list[i].vip_level);
				set_nodes.push_back(vip_level_node);

				DataNode flexible_name_node(tabledef::CROSS_PERSONRANK_FLEXIBLE_NAME);
				flexible_name_node.SetString((const char *)cross_person_rank_param.cross_person_rank_list[i].flexible_name);
				set_nodes.push_back(flexible_name_node);

				CmpNodeList cmp_nodes;
				CmpNode index_node(tabledef::CROSS_PERSONRANK_RANK_INDEX);
				index_node.SetInt32(cross_person_rank_param.cross_person_rank_list[i].index);
				cmp_nodes.push_back(index_node);

				CmpNode cmp_uuid_node(tabledef::CROSS_PERSONRANK_UUID);
				cmp_uuid_node.SetLong64(0);
				cmp_uuid_node.SetEqualType(EQUAL_TYPE_GRANTER);
				cmp_nodes.push_back(cmp_uuid_node);

				int count = rank_command->Update(stmt, cmp_nodes, set_nodes);
				if (count < 0)
				{
					ret = -3; 
				}
			}
			break; // case structcommon::CS_UPDATE

		} // switch end

	} // for end

	conn->commit();

	return ret < 0 ? ret : 0;
}
