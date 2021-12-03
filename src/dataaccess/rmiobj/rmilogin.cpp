#include "rmilogin.h"
#include "servercommon/darmi/dalogin.h"

#include "databaselayer.h"
#include "engineadapter.h"

#include "servercommon/servercommon.h"
#include "dataaccesslog.hpp"
#include "servercommon/crossdef.hpp"
#include "crossrolecache/crossrolecachemanager.hpp"

USE_DATAACCESS_VAR();
OLD_USE_INTERNAL_NETWORK();

RMILoginObject::RMILoginObject()
{
	m_method_list[dataaccessrmi::login::GET_PROF_NUM] = __GetProfNum;
	m_method_list[dataaccessrmi::login::USER_LOGIN] = __UserLogin;
	m_method_list[dataaccessrmi::login::ADD_ROLE] = __AddRole;
	m_method_list[dataaccessrmi::login::REMOVE_ROLE] = __RemoveRole;
	m_method_list[dataaccessrmi::login::FORBID] = __Forbid;
	m_method_list[dataaccessrmi::login::FORBID_ONE_ROLE] = __ForbidOneRole;
	m_method_list[dataaccessrmi::login::GET_ROLE] = __GetGold;
	m_method_list[dataaccessrmi::login::CHANGE_ROLE] = __ChangeGold;
	m_method_list[dataaccessrmi::login::ADD_NAME_INFO] = __AddNameInfo;
	m_method_list[dataaccessrmi::login::UPDATE_NAME_INFO] = __UpdateNameInfo;
	m_method_list[dataaccessrmi::login::DELETE_NAME_INFO] = __DeleteNameInfo;
	m_method_list[dataaccessrmi::login::DELETE_NAME_INFO_BY_NAME] = __DeleteNameInfoByName;
	m_method_list[dataaccessrmi::login::QUERY_NAME_EXIST] = __QueryNameExist;
	m_method_list[dataaccessrmi::login::RESET_ROLE_NAME] = __ResetRoleName;
	m_method_list[dataaccessrmi::login::INIT_WORLD_STATUS] = __InitWorldstatus;
	m_method_list[dataaccessrmi::login::SAVE_WORLD_STATUS] = __SaveWorldstatus;
}

RMILoginObject::~RMILoginObject()
{
}

void RMILoginObject::__free()
{
	delete this;
}

const char * RMILoginObject::__get_obj_id()
{
	return dataaccessrmi::LOGIN;
}

int RMILoginObject::__GetProfNum(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) * 5)
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	int prof1_num = 0, prof2_num = 0, prof3_num = 0, prof4_num = 0;
	int result = GetProfNum(&prof1_num, &prof2_num, &prof3_num, &prof4_num);

	if (!out_param->Push(result))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!out_param->Pushf("iiii", prof1_num, prof2_num, prof3_num, prof4_num))
		{
			return rmi::DispatchOutParamBuffTooShort;
		}
	}

	return rmi::DispatchOK;
}

int RMILoginObject::GetProfNum(int *prof1_num, int *prof2_num, int *prof3_num, int *prof4_num)
{
	if (NULL == prof1_num || NULL == prof2_num || NULL == prof3_num || NULL == prof4_num)
	{
		return -1;
	}

	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL);

	INIT_STMT(conn, stmt, -1);
	conn->begin();

	int ret = -1;
	DataAdapterList adapter_list;

	{
		QueryCondition condition;
		condition.count_type = FIND_COUNT_TYPE_COUNT;

		CmpNode node(tabledef::ROLE_ATTR_DETAIL_PROF);
		node.SetByte(PROF_TYPE_PROF_1);
		condition.cmp_list.push_back(node);

		ret = command->Find(stmt, condition, &adapter_list, false, false);
		if (ret < 0)
		{
			return ret;
		}

		*prof1_num = ret;
	}

	{
		QueryCondition condition;
		condition.count_type = FIND_COUNT_TYPE_COUNT;

		CmpNode node(tabledef::ROLE_ATTR_DETAIL_PROF);
		node.SetByte(PROF_TYPE_PROF_2);
		condition.cmp_list.push_back(node);

		ret = command->Find(stmt, condition, &adapter_list, false, false);
		if (ret < 0)
		{
			return ret;
		}

		*prof2_num = ret;
	}

	{
		QueryCondition condition;
		condition.count_type = FIND_COUNT_TYPE_COUNT;

		CmpNode node(tabledef::ROLE_ATTR_DETAIL_PROF);
		node.SetByte(PROF_TYPE_PROF_3);
		condition.cmp_list.push_back(node);

		ret = command->Find(stmt, condition, &adapter_list, false, false);
		if (ret < 0)
		{
			return ret;
		}

		*prof3_num = ret;
	}

	{
		QueryCondition condition;
		condition.count_type = FIND_COUNT_TYPE_COUNT;

		CmpNode node(tabledef::ROLE_ATTR_DETAIL_PROF);
		node.SetByte(PROF_TYPE_PROF_4);
		condition.cmp_list.push_back(node);

		ret = command->Find(stmt, condition, &adapter_list, false, false);
		if (ret < 0)
		{
			return ret;
		}

		*prof4_num = ret;
	}

	return 0;
}

int RMILoginObject::__UserLogin(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) * 5)
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	const char *pname = NULL;
	char can_insert = 0;
	if (!in_param.Pop(&pname) || !in_param.Pop(&can_insert))
	{
		return rmi::DispatchParamError;
	}

	int db_index = 0, role_1 = 0, role_2 = 0, role_3 = 0;
	int result = UserLogin(pname, &db_index, &role_1, &role_2, &role_3, can_insert);

	if (!out_param->Push(result))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!out_param->Pushf("iiii", db_index, role_1, role_2, role_3))
		{
			return rmi::DispatchOutParamBuffTooShort;
		}
	}

	return rmi::DispatchOK;
}

int RMILoginObject::UserLogin(const char *pname, int *db_index, int *role_id_1, int *role_id_2, int *role_id_3, char can_insert)
{
	// printf("UserLogin:%d %s\n", pid, pname);

	{
		// 先判断是否跨服
		bool is_cross = false;
		{
			int db_start_pos = -1;
 			for (int i = strlen(pname) - 1; i >= 0; i--)
			{
				if ('_' == pname[i])
				{
					db_start_pos = i;
					break;
				}
			}

			if (db_start_pos >= 0 && db_start_pos < (int)strlen(pname))
			{
				int server_num = atoi(pname + db_start_pos + 1);
				is_cross = (CROSS_USER_DB_INDEX == server_num);
			}	
		}

		if (is_cross) // 如果是跨服 
		{
			PlatName temp_pname = {0}; STRNCPY(temp_pname, pname, sizeof(PlatName));
			*role_id_1 = CrossRoleCacheManager::Instance().GetCrossRoleID(temp_pname);
			*db_index = CROSS_USER_DB_INDEX;

			dataaccesslog::g_log_role.printf(LL_DEBUG, "UserLogin:[pname1:%s] [role_id_1:%d] [role_id_2:%d] [role_id_3:%d]", 
				temp_pname, *role_id_1, *role_id_2, *role_id_3);

			return 0;
		}
	}

	const MataTableDef *logintable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_LOGIN);
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_LOGIN);

	INIT_STMT(conn, stmt, -1);
	conn->begin();

	QueryCondition condition;

	CmpNode node(tabledef::LOGIN_PLAT_USER_NAME);
	node.SetString(pname);
	condition.cmp_list.push_back(node);

	int ret = -1;

	DataAdapter logindata = logintable->GetPrototype();
	logindata.Malloc();
	if (DBCommand::RESULT_SUC == command->Find(stmt, condition, &logindata))
	{
		ret = 0;
		*db_index = logindata.m_data_area[tabledef::LOGIN_DB_INDEX].vint;
		*role_id_1 = logindata.m_data_area[tabledef::LOGIN_ROLE_ID_1].vint;
		*role_id_2 = logindata.m_data_area[tabledef::LOGIN_ROLE_ID_2].vint;
		*role_id_3 = logindata.m_data_area[tabledef::LOGIN_ROLE_ID_3].vint;
	}

	time_t now = EngineAdapter::Instance().Time();

	if (ret == 0)
	{
		time_t forbidtime = logindata.m_data_area[tabledef::LOGIN_FORBIDTIME].vint64;
		if (forbidtime != 0 && forbidtime > now)
		{
			// 帐号被封
// 			*role_id_1 = 0;
// 			*role_id_2 = 0;
// 			*role_id_3 = 0;
			conn->rollback();
			ret = -2;
		}
		else
		{
			logindata.m_data_area[tabledef::LOGIN_LASTLOGINTIME].vint64 = now;

			if (DBCommand::RESULT_SUC != command->Update(stmt, logindata))
			{
				*role_id_1 = 0;
				*role_id_2 = 0;
				*role_id_3 = 0;
				conn->rollback();
				ret = -1;
			}
			else
			{
				// 查询单个角色是否被封号
				{
					time_t forbid_time_1 = logindata.m_data_area[tabledef::LOGIN_FORBID_TIME_1].vint;
					if (forbid_time_1 != 0 && forbid_time_1 > now)
					{
						*role_id_1 = 0;
					}

					time_t forbid_time_2 = logindata.m_data_area[tabledef::LOGIN_FORBID_TIME_2].vint;
					if (forbid_time_2 != 0 && forbid_time_2 > now)
					{
						*role_id_2 = 0;
					}

					time_t forbid_time_3 = logindata.m_data_area[tabledef::LOGIN_FORBID_TIME_3].vint;
					if (forbid_time_3 != 0 && forbid_time_3 > now)
					{
						*role_id_3 = 0;
					}

					// 整理一下角色顺序
					if (0 == *role_id_1)
					{
						*role_id_1 = *role_id_2;
						*role_id_2 = 0;
					}

					if (0 == *role_id_2)
					{
						*role_id_2 = *role_id_3;
						*role_id_3 = 0;
					}
				}

				conn->commit();
				ret = 0;
			}
		}

		logindata.Free();

		return ret < 0 ? ret : 0;
	}

	*role_id_1 = 0;
	*role_id_2 = 0;
	*role_id_3 = 0;

	if (0 == can_insert)
	{
		ret = 0;
	}
	else
	{
		STRNCPY(logindata.m_data_area[tabledef::LOGIN_PLAT_USER_NAME].vcharp, pname, 
			logindata.m_data_area[tabledef::LOGIN_PLAT_USER_NAME].length);
		logindata.m_data_area[tabledef::LOGIN_DB_INDEX].vint = 0;
		logindata.m_data_area[tabledef::LOGIN_ROLE_ID_1].vint = 0;
		logindata.m_data_area[tabledef::LOGIN_ROLE_ID_2].vint = 0;
		logindata.m_data_area[tabledef::LOGIN_ROLE_ID_3].vint = 0;
		logindata.m_data_area[tabledef::LOGIN_LASTLOGINTIME].vint64 = now;
		logindata.m_data_area[tabledef::LOGIN_CREATETIME].vint64 = now;
		logindata.m_data_area[tabledef::LOGIN_FORBIDTIME].vint64 = 0;
		logindata.m_data_area[tabledef::LOGIN_FORBID_TIME_1].vint64 = 0;
		logindata.m_data_area[tabledef::LOGIN_FORBID_TIME_2].vint64 = 0;
		logindata.m_data_area[tabledef::LOGIN_FORBID_TIME_3].vint64 = 0;

		ret = -1;
		if (DBCommand::RESULT_SUC == command->Save(stmt, &logindata))
		{
			ret = 0;
		}
	}

	logindata.Free();

	conn->commit();

	return ret < 0 ? ret : 0;
}

int RMILoginObject::__AddRole(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	int db_index, role_id;
	const char *pname = NULL;
	if (!in_param.Popf("sii", &pname, &db_index, &role_id))
	{
		return rmi::DispatchParamError;
	}

	int result = AddRole(pname, db_index, role_id);

	if (!out_param->Push(result))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}
	
	return rmi::DispatchOK;
}

int RMILoginObject::AddRole(const char *pname, int db_index, int role_id)
{
	const MataTableDef *logintable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_LOGIN);
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_LOGIN);

	INIT_STMT(conn, stmt, -1);
	conn->begin();

	QueryCondition condition;

	CmpNode node(tabledef::LOGIN_PLAT_USER_NAME);
	node.SetString(pname);
	condition.cmp_list.push_back(node);

	DataAdapter logindata = logintable->GetPrototype();
	logindata.Malloc();
	if (DBCommand::RESULT_SUC != command->Find(stmt, condition, &logindata))
	{
		logindata.Free();
		conn->rollback();
		return -1;
	}

	time_t forbidtime = logindata.m_data_area[tabledef::LOGIN_FORBIDTIME].vint64;
	if (forbidtime != 0 && forbidtime > EngineAdapter::Instance().Time())
	{
		// 帐号被封
		logindata.Free();
		conn->rollback();
		return -1;
	}

	bool find_space = false;
	if (logindata.m_data_area[tabledef::LOGIN_ROLE_ID_1].vint == 0)
	{
		find_space = true;
		logindata.m_data_area[tabledef::LOGIN_DB_INDEX].vint = db_index;
		logindata.m_data_area[tabledef::LOGIN_ROLE_ID_1].vint = role_id;
	}
	else if (logindata.m_data_area[tabledef::LOGIN_ROLE_ID_2].vint == 0)
	{
		find_space = true;
		logindata.m_data_area[tabledef::LOGIN_ROLE_ID_2].vint = role_id;
	}
	else if (logindata.m_data_area[tabledef::LOGIN_ROLE_ID_3].vint == 0)
	{
		find_space = true;
		logindata.m_data_area[tabledef::LOGIN_ROLE_ID_3].vint = role_id;
	}
	else
	{
		dataaccesslog::g_log_role.printf(LL_INFO, "[RMILoginObject::AddRole error role full] [pname:%s]", pname);
	}

	if (!find_space)
	{
		logindata.Free();
		conn->rollback();
		return -1;
	}

	if (DBCommand::RESULT_SUC != command->Update(stmt, logindata))
	{
		logindata.Free();
		conn->rollback();
		return -1;
	}

	logindata.Free();

	conn->commit();

	return 0;
}

int RMILoginObject::__RemoveRole(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	int db_index, role_id;
	const char *pname = NULL;
	if (!in_param.Popf("sii", &pname, &db_index , &role_id))
	{
		return rmi::DispatchParamError;
	}

	int result = RemoveRole(pname, db_index, role_id);

	if (!out_param->Push(result))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMILoginObject::RemoveRole(const char *pname, int db_index, int role_id)
{
	const MataTableDef *logintable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_LOGIN);
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_LOGIN);

	INIT_STMT(conn, stmt, -1);
	conn->begin();

	QueryCondition condition;

	CmpNode node(tabledef::LOGIN_PLAT_USER_NAME);
	node.SetString(pname);
	condition.cmp_list.push_back(node);

	DataAdapter logindata = logintable->GetPrototype();
	logindata.Malloc();

	if (DBCommand::RESULT_SUC != command->Find(stmt, condition, &logindata))
	{
		logindata.Free();
		conn->rollback();
		return -1;
	}

	time_t forbidtime = logindata.m_data_area[tabledef::LOGIN_FORBIDTIME].vint64;
	if (forbidtime != 0 && forbidtime > EngineAdapter::Instance().Time())
	{
		// 帐号被封
		logindata.Free();
		conn->rollback();
		return -1;
	}

	if (logindata.m_data_area[tabledef::LOGIN_DB_INDEX].vint != db_index)
	{
		logindata.Free();
		conn->rollback();
		return -1;
	}

	bool find_role = false;
	if (logindata.m_data_area[tabledef::LOGIN_ROLE_ID_1].vint == role_id)
	{
		find_role = true;
		logindata.m_data_area[tabledef::LOGIN_ROLE_ID_1].vint = logindata.m_data_area[tabledef::LOGIN_ROLE_ID_2].vint;
		logindata.m_data_area[tabledef::LOGIN_ROLE_ID_2].vint = logindata.m_data_area[tabledef::LOGIN_ROLE_ID_3].vint;
		logindata.m_data_area[tabledef::LOGIN_ROLE_ID_3].vint = 0;
	}
	else if (logindata.m_data_area[tabledef::LOGIN_ROLE_ID_2].vint == role_id)
	{
		find_role = true;
		logindata.m_data_area[tabledef::LOGIN_ROLE_ID_2].vint = logindata.m_data_area[tabledef::LOGIN_ROLE_ID_3].vint;
		logindata.m_data_area[tabledef::LOGIN_ROLE_ID_3].vint = 0;
	}
	else if (logindata.m_data_area[tabledef::LOGIN_ROLE_ID_3].vint == role_id)
	{
		find_role = true;
		logindata.m_data_area[tabledef::LOGIN_ROLE_ID_3].vint = 0;
	}

	if (!find_role)
	{
		logindata.Free();
		conn->rollback();
		return -1;
	}

	if (logindata.m_data_area[tabledef::LOGIN_ROLE_ID_1].vint == 0
		&& logindata.m_data_area[tabledef::LOGIN_ROLE_ID_2].vint == 0
		&& logindata.m_data_area[tabledef::LOGIN_ROLE_ID_3].vint == 0)
	{
		logindata.m_data_area[tabledef::LOGIN_DB_INDEX].vint = 0;
	}

	if (DBCommand::RESULT_SUC != command->Update(stmt, logindata))
	{
		logindata.Free();
		conn->rollback();
		return -1;
	}

	logindata.Free();

	conn->commit();

	return 0;
}

int RMILoginObject::__Forbid(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) * 5)
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	const char *pname = NULL;
	unsigned int forbid_time;
	if (!in_param.Pop(&pname) || !in_param.Pop(&forbid_time))
	{
		return rmi::DispatchParamError;
	}
	
	int db_index, role_1, role_2, role_3;
	int result = Forbid(pname, forbid_time, &db_index, &role_1, &role_2, &role_3);

	if (!out_param->Push(result) ||
		!out_param->Push(db_index) || !out_param->Push(role_1) || !out_param->Push(role_2) || !out_param->Push(role_3))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMILoginObject::Forbid(const char *pname, unsigned int forbid_time, int *db_index, int *role_id_1, int *role_id_2, int *role_id_3)
{
	const MataTableDef *logintable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_LOGIN);
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_LOGIN);

	INIT_STMT(conn, stmt, -1);
	conn->begin();

	QueryCondition condition;
	CmpNode node(tabledef::LOGIN_PLAT_USER_NAME);
	node.SetString(pname);
	condition.cmp_list.push_back(node);

	DataAdapter logindata = logintable->GetPrototype();
	logindata.Malloc();
	if (DBCommand::RESULT_SUC != command->Find(stmt, condition, &logindata, true, true))
	{
		logindata.Free();
		conn->rollback();
		return -1;
	}

	*db_index = logindata.m_data_area[tabledef::LOGIN_DB_INDEX].vint;
	*role_id_1 = logindata.m_data_area[tabledef::LOGIN_ROLE_ID_1].vint;
	*role_id_2 = logindata.m_data_area[tabledef::LOGIN_ROLE_ID_2].vint;
	*role_id_3 = logindata.m_data_area[tabledef::LOGIN_ROLE_ID_3].vint;

	long long now = EngineAdapter::Instance().Time();
	logindata.m_data_area[tabledef::LOGIN_FORBIDTIME].vint64 = now + forbid_time;

	if (DBCommand::RESULT_SUC != command->Update(stmt, logindata))
	{
		logindata.Free();
		conn->rollback();
		return -1;
	}

	logindata.Free();

	if (*role_id_1 != 0)
	{
		RMILoginObject::ChangeRoleLastSaveTime(*role_id_1, now);
	}
	if (*role_id_2 != 0)
	{
		RMILoginObject::ChangeRoleLastSaveTime(*role_id_2, now);
	}
	if (*role_id_3 != 0)
	{
		RMILoginObject::ChangeRoleLastSaveTime(*role_id_3, now);
	}

	conn->commit();

	return 0;
}

int RMILoginObject::ChangeRoleLastSaveTime(int role_id, long long save_time)
{
	DBCommand* rolecommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE);

	INIT_STMT(conn, stmt, -1);

	DataNodeList role_update;
	DataNode last_save_time(tabledef::ROLE_LAST_SAVE_TIME);
	last_save_time.SetLong64(save_time);
	role_update.push_back(last_save_time);

	CmpNodeList condition;
	CmpNode id_node(tabledef::ROLE_ROLE_ID);
	id_node.SetInt32(role_id);
	condition.push_back(id_node);

	int count = rolecommand->Update(stmt, condition, role_update);
	return count;
}

int RMILoginObject::__ForbidOneRole(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) * 2)
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	const char *pname = NULL;
	int forbid_role_id = 0;
	unsigned int forbid_time;
	if (!in_param.Pop(&pname) || !in_param.Pop(&forbid_role_id) || !in_param.Pop(&forbid_time))
	{
		return rmi::DispatchParamError;
	}

	int db_index = 0;
	int result = ForbidOneRole(pname, forbid_time, forbid_role_id, &db_index);

	if (!out_param->Push(result) || !out_param->Push(db_index))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMILoginObject::ForbidOneRole(const char *pname, unsigned int forbid_time, int forbid_role_id, int *db_index)
{
	const MataTableDef *logintable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_LOGIN);
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_LOGIN);

	INIT_STMT(conn, stmt, -1);
	conn->begin();

	QueryCondition condition;
	CmpNode node(tabledef::LOGIN_PLAT_USER_NAME);
	node.SetString(pname);
	condition.cmp_list.push_back(node);

	DataAdapter logindata = logintable->GetPrototype();
	logindata.Malloc();
	if (DBCommand::RESULT_SUC != command->Find(stmt, condition, &logindata, true, true))
	{
		logindata.Free();
		conn->rollback();
		return -1;
	}

	long long now = EngineAdapter::Instance().Time();
	if (logindata.m_data_area[tabledef::LOGIN_ROLE_ID_1].vint == forbid_role_id)
	{
		logindata.m_data_area[tabledef::LOGIN_FORBID_TIME_1].vint64 = now + forbid_time;
	}
	else if (logindata.m_data_area[tabledef::LOGIN_ROLE_ID_2].vint64 == forbid_role_id)
	{
		logindata.m_data_area[tabledef::LOGIN_FORBID_TIME_2].vint64 = now + forbid_time;
	}
	else if (logindata.m_data_area[tabledef::LOGIN_ROLE_ID_3].vint64 == forbid_role_id)
	{
		logindata.m_data_area[tabledef::LOGIN_FORBID_TIME_3].vint64 = now + forbid_time;
	}
	else
	{
		// 没找到
		logindata.Free();
		conn->rollback();
		return -1;
	}

	*db_index = logindata.m_data_area[tabledef::LOGIN_DB_INDEX].vint;
	if (DBCommand::RESULT_SUC != command->Update(stmt, logindata))
	{
		logindata.Free();
		conn->rollback();
		return -1;
	}

	logindata.Free();

	conn->commit();

	return 0;
}

int RMILoginObject::__GetGold(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(Int64))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	const char *pname = NULL;
	int role_id = 0;
	if (!in_param.Pop(&pname) || !in_param.Pop(&role_id))
	{
		return rmi::DispatchParamError;
	}

	int authority_type = 0;
	Int64 gold = GetGold(pname, role_id, &authority_type);
	if (!out_param->Push(gold) || !out_param->Push(authority_type))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

Int64 RMILoginObject::GetGold(const char *pname, int role_id, int *authority_type)
{
	const MataTableDef *goldtable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ACCOUNTGOLD);
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ACCOUNTGOLD);

	INIT_STMT(conn, stmt, -1);

	QueryCondition condition;

	CmpNode node(tabledef::ACCOUNTGOLD_PLAT_USER_NAME);
	node.SetString(pname);
	condition.cmp_list.push_back(node);

	CmpNode role_node(tabledef::ACCOUNTGOLD_ROLE_ID);
	role_node.SetInt32(role_id);
	condition.cmp_list.push_back(role_node);

	Int64 gold = 0;

	DataAdapter golddata = goldtable->GetPrototype();
	golddata.Malloc();
	if (DBCommand::RESULT_SUC == command->Find(stmt, condition, &golddata))
	{
		gold = golddata.m_data_area[tabledef::ACCOUNTGOLD_GOLD].vint64;
		if (NULL != authority_type) *authority_type = golddata.m_data_area[tabledef::ACCOUNTGOLD_AUTHORITY_TYPE].vint;
	}

	//time_t now = EngineAdapter::Instance().Time();
	//if (now - golddata.m_data_area[tabledef::ACCOUNTGOLD_LAST_GET_GOLD_TIME].vint64 < 5)
	//{
	//	gold = 0;
	//}

	golddata.Free();

	return gold;
}

int RMILoginObject::__ChangeGold(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(Int64))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	const char *pname = NULL;
	Int64 gold_delta = 0;
	int role_id = 0;
	if (!in_param.Popf("ski", &pname, &gold_delta, &role_id))
	{
		return rmi::DispatchParamError;
	}

	Int64 new_gold = 0;
	int result = ChangeGold(pname, role_id, gold_delta, &new_gold);

	if (NULL != pname)
	{
		dataaccesslog::g_log_accountgold.printf(LL_INFO, "RMILoginObject::__ChangeGold plat_name:%s result:%d gold_delta:%d new_gold:%d",
			pname, result, (int)gold_delta, (int)new_gold);
	}

	if (!out_param->Push(result) || !out_param->Push(new_gold))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMILoginObject::ChangeGold(const char *pname, int role_id, Int64 gold_delta, Int64 *new_gold)
{
	if (gold_delta == 0) return true;

	// 加钱回来也走这条路经
	//if (gold_delta < 0)
	//{
	//	return -1;
	//}

	*new_gold = 0;

	const MataTableDef *goldtable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ACCOUNTGOLD);
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ACCOUNTGOLD);

	INIT_STMT(conn, stmt, -1);
	conn->begin(false);

	QueryCondition condition;

	CmpNode node(tabledef::ACCOUNTGOLD_PLAT_USER_NAME);
	node.SetString(pname);
	condition.cmp_list.push_back(node);

	CmpNode role_node(tabledef::ACCOUNTGOLD_ROLE_ID);
	role_node.SetInt32(role_id);
	condition.cmp_list.push_back(role_node);

	Int64 gold_old = 0;

	DataAdapter golddata = goldtable->GetPrototype();
	golddata.Malloc();
	if (DBCommand::RESULT_SUC == command->Find(stmt, condition, &golddata, true))
	{
		gold_old = golddata.m_data_area[tabledef::ACCOUNTGOLD_GOLD].vint64;
	}

	time_t now = EngineAdapter::Instance().Time();
	if (gold_delta > 0)	// <= 是gameworld加钱不成功把钱加回数据库的流程 不检查提金间隔 
	{
		if (gold_old < gold_delta)
		{
			*new_gold = gold_old;

			golddata.Free();
			conn->rollback();
			return -2;
		}

		if (now < golddata.m_data_area[tabledef::ACCOUNTGOLD_LAST_GET_GOLD_TIME].vint64 + 5)
		{
			golddata.Free();
			conn->rollback();
			return -3;
		}
	}

	Int64 gold_new = gold_old - gold_delta;

	golddata.m_data_area[tabledef::ACCOUNTGOLD_GOLD].vint64 = gold_new;
	golddata.m_data_area[tabledef::ACCOUNTGOLD_LAST_GET_GOLD_TIME].vint64 = (long long)now;

	if (DBCommand::RESULT_SUC != command->Update(stmt, golddata))
	{
		golddata.Free();
		conn->rollback();
		return -1;
	}
	*new_gold = gold_new;

	golddata.Free();

	conn->commit();

	return 0;
}

int RMILoginObject::__AddNameInfo(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	const char *role_name = NULL, *pname = NULL;

	if (!in_param.Pop(&role_name) || !in_param.Pop(&pname))
	{
		return rmi::DispatchParamError;
	}

	int result = AddName(role_name, pname);

	if (!out_param->Push(result))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMILoginObject::AddName(const char *role_name, const char *pname)
{
	const MataTableDef *nametable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_NAME_MAP);
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_NAME_MAP);

	INIT_STMT(conn, stmt, -1);
	conn->begin();

	QueryCondition condition;

	CmpNode node(tabledef::ROLE_NAME_MAP_ROLE_NAME);
	node.SetString(role_name);
	condition.cmp_list.push_back(node);

	int ret = 0;

	DataAdapter namedata = nametable->GetPrototype();
	namedata.Malloc();
	if (DBCommand::RESULT_SUC == command->Find(stmt, condition, &namedata))
	{
		namedata.Free();
		conn->rollback();
		ret = -1;
		return ret;
	}
	
	ret = -1;

	STRNCPY(namedata.m_data_area[tabledef::ROLE_NAME_MAP_ROLE_NAME].vcharp, role_name, 
		namedata.m_data_area[tabledef::ROLE_NAME_MAP_ROLE_NAME].length);
	STRNCPY(namedata.m_data_area[tabledef::ROLE_NAME_MAP_PLAT_USER_NAME].vcharp, pname, 
		namedata.m_data_area[tabledef::ROLE_NAME_MAP_PLAT_USER_NAME].length);

	namedata.m_data_area[tabledef::ROLE_NAME_MAP_DB_INDEX].vint = 0;
	namedata.m_data_area[tabledef::ROLE_NAME_MAP_ROLE_ID].vint = 0;

	if (DBCommand::RESULT_SUC == command->Save(stmt, &namedata))
	{
		ret = 0;
	}

	namedata.Free();

	conn->commit();

	return ret < 0 ? ret : 0;
}

int RMILoginObject::__UpdateNameInfo(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	const char *role_name = NULL;
	int db_index, role_id;
	if (!in_param.Popf("sii", &role_name, &db_index, &role_id))
	{
		return rmi::DispatchParamError;
	}

	int result = UpdateNameInfo(role_name, db_index, role_id);

	if (!out_param->Push(result))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMILoginObject::UpdateNameInfo(const char *role_name, int db_index, int role_id)
{
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_NAME_MAP);

	INIT_STMT(conn, stmt, -1);
	conn->begin();

	DataNodeList name_update;
	name_update.reserve(tabledef::TABLE_ROLE_NAME_MAP_MAXINDEX - 2);

	DataNode db_index_node(tabledef::ROLE_NAME_MAP_DB_INDEX);
	db_index_node.SetInt32(db_index);
	name_update.push_back(db_index_node);

	DataNode role_id_node(tabledef::ROLE_NAME_MAP_ROLE_ID);
	role_id_node.SetInt32(role_id);
	name_update.push_back(role_id_node);

	CmpNodeList condition;
	CmpNode name_node(tabledef::ROLE_NAME_MAP_ROLE_NAME);
	name_node.SetString(role_name);
	condition.push_back(name_node);

	int count = command->Update(stmt, condition, name_update);
	if (count < 1)
	{
		conn->rollback();
		return -1;
	}

	conn->commit();

	return 0;
}

int RMILoginObject::__DeleteNameInfo(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	int db_index, role_id;
	const char *pname = NULL;
	if (!in_param.Popf("iis", &db_index, &role_id, &pname))
	{
		return rmi::DispatchParamError;
	}

	int result = DeleteNameInfo(db_index, role_id, pname);

	if (!out_param->Push(result))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMILoginObject::DeleteNameInfo(int db_index, int role_id, const char *pname)
{
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_NAME_MAP);

	INIT_STMT(conn, stmt, -1);

	conn->begin();

	CmpNodeList cmp_node;

	CmpNode role_id_node(tabledef::ROLE_NAME_MAP_ROLE_ID);
	role_id_node.SetInt32(role_id);
	cmp_node.push_back(role_id_node);

	CmpNode db_index_node(tabledef::ROLE_NAME_MAP_DB_INDEX);
	db_index_node.SetInt32(db_index);
	cmp_node.push_back(db_index_node);

	CmpNode pname_node(tabledef::ROLE_NAME_MAP_PLAT_USER_NAME);
	pname_node.SetString(pname);
	cmp_node.push_back(pname_node);

	int count = command->Remove(stmt, cmp_node);
	if (count <= 0)
	{
		conn->rollback();
		return -1;
	}

	conn->commit();

	return 0;
}

int RMILoginObject::__DeleteNameInfoByName(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	const char *name = NULL;
	if (!in_param.Pop(&name))
	{
		return rmi::DispatchParamError;
	}

	int result = DeleteNameInfoByName(name);

	if (!out_param->Push(result))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMILoginObject::DeleteNameInfoByName(const char *role_name)
{
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_NAME_MAP);

	INIT_STMT(conn, stmt, -1);

	conn->begin();

	CmpNodeList cmp_node;

	CmpNode role_name_node(tabledef::ROLE_NAME_MAP_ROLE_NAME);
	role_name_node.SetString(role_name);
	cmp_node.push_back(role_name_node);

	int count = command->Remove(stmt, cmp_node);
	if (count <= 0)
	{
		conn->rollback();
		return -1;
	}

	conn->commit();

	return 0;
}

int RMILoginObject::__AddIdentity(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	const char *pname = NULL, *name = NULL, *id = NULL;
	bool check_repeat;
	if (!in_param.Popf("sssb", &pname, &name, &id, &check_repeat))
	{
		return rmi::DispatchParamError;
	}

	int result = AddIdentity(pname, name, id, check_repeat);

	if (!out_param->Push(result))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMILoginObject::AddIdentity(const char *pname, const char *name, const char *id, bool check_repeat)
{
	const MataTableDef *idtable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ACCOUNTIDENTITY);
	DBCommand *idcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ACCOUNTIDENTITY);

	INIT_STMT(conn, stmt, -1);
	conn->begin(false);

	DataAdapter iddata = idtable->GetPrototype();
	iddata.Malloc();

	if (check_repeat)
	{
		QueryCondition condition;

		CmpNode id_node(tabledef::ACCOUNTIDENTITY_IDENTITY_NUM);
		id_node.SetString(id);
		condition.cmp_list.push_back(id_node);

		if (DBCommand::RESULT_SUC == idcommand->Find(stmt, condition, &iddata, false, false))
		{
			iddata.Free();
			conn->rollback();
			return -2;  // id 重复
		}
	}

	STRNCPY(iddata.m_data_area[tabledef::ACCOUNTIDENTITY_PLAT_USER_NAME].vcharp, pname, 
		iddata.m_data_area[tabledef::ACCOUNTIDENTITY_PLAT_USER_NAME].length);

	STRNCPY(iddata.m_data_area[tabledef::ACCOUNTIDENTITY_NAME].vcharp, name, 
		iddata.m_data_area[tabledef::ACCOUNTIDENTITY_NAME].length);

	STRNCPY(iddata.m_data_area[tabledef::ACCOUNTIDENTITY_IDENTITY_NUM].vcharp, id, 
		iddata.m_data_area[tabledef::ACCOUNTIDENTITY_IDENTITY_NUM].length);

	if (DBCommand::RESULT_SUC != idcommand->Save(stmt, &iddata, false))
	{
		iddata.Free();
		conn->rollback();
		return -1;
	}

	iddata.Free();

	conn->commit();

	return 0;
}

int RMILoginObject::__QueryNameExist(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	const char *name_tmp = 0;

	if ( !in_param.Pop(&name_tmp) )
	{
		return rmi::DispatchParamError;
	}

	int result = QueryNameExist(name_tmp);

	out_param->Push(result);

	return rmi::DispatchOK;
}

int RMILoginObject::QueryNameExist(const char *role_name)
{
	const MataTableDef *nametable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_NAME_MAP);
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_NAME_MAP);

	INIT_STMT(conn, stmt, -1);

	QueryCondition condition;

	CmpNode node(tabledef::ROLE_NAME_MAP_ROLE_NAME);
	node.SetString(role_name);
	condition.cmp_list.push_back(node);

	DataAdapter namedata = nametable->GetPrototype();
	namedata.Malloc();

	if (DBCommand::RESULT_SUC == command->Find(stmt, condition, &namedata))
	{
		namedata.Free();
		return -1;
	}

	namedata.Free();

	return 0;
}

int RMILoginObject::__ResetRoleName(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	int db_index = -1, role_id = 0;
	const char *name_tmp = 0;

	if (!in_param.Pop(&db_index) || !in_param.Pop(&role_id) || !in_param.Pop(&name_tmp))
	{
		return rmi::DispatchParamError;
	}

	int result = ResetRoleName(db_index, role_id, name_tmp);

	out_param->Push(result);

	return rmi::DispatchOK;
}

int RMILoginObject::ResetRoleName(int db_index, int role_id, const char *role_name)
{
	const MataTableDef *nametable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_NAME_MAP);
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_NAME_MAP);

	INIT_STMT(conn, stmt, -1);

	QueryCondition condition1;

	CmpNode node1(tabledef::ROLE_NAME_MAP_ROLE_NAME);
	node1.SetString(role_name);
	condition1.cmp_list.push_back(node1);

	DataAdapter namedata = nametable->GetPrototype();
	namedata.Malloc();

	if (DBCommand::RESULT_SUC == command->Find(stmt, condition1, &namedata))
	{
		namedata.Free();
		return -2;
	}

	namedata.Free();

	DataNodeList name_update;

	DataNode name_node(tabledef::ROLE_NAME_MAP_ROLE_NAME);
	name_node.SetString(role_name);
	name_update.push_back(name_node);

	CmpNodeList condition2;

	CmpNode node2(tabledef::ROLE_NAME_MAP_DB_INDEX);
	node2.SetInt32(db_index);
	condition2.push_back(node2);

	CmpNode node3(tabledef::ROLE_NAME_MAP_ROLE_ID);
	node3.SetInt32(role_id);
	condition2.push_back(node3);

	conn->begin();

	if (0 >= command->Update(stmt, condition2, name_update))
	{
		conn->rollback();
		return -1;
	}

	conn->commit();

	return 0;
}

int RMILoginObject::__InitWorldstatus(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	int is_register_limit = 0;
	int result = InitWorldstatus(&is_register_limit);

	out_param->Push(result);
	out_param->Push(is_register_limit);

	return rmi::DispatchOK;
}
int RMILoginObject::InitWorldstatus(int *is_register_limit)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *worldstatus_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_WORLDSTATUS);

	CmpNode node1(tabledef::WORLDSTATUS_IDWORLDSTATUS);
	node1.SetLong64(0);
	node1.SetEqualType(EQUAL_TYPE_GRANTER);

	QueryCondition cond;
	cond.cmp_list.push_back(node1);
	cond.query_num = 1;

	DataAdapterList worldstate_list;
	int ret = worldstatus_command->Find(stmt, cond, &worldstate_list);
	if (ret < 0)
	{
		return ret;
	}

	ret = 0;

	DataAdapterList::iterator i = worldstate_list.begin();
	for (; i != worldstate_list.end(); ++i)
	{
		*is_register_limit = static_cast<unsigned int>(i->m_data_area[tabledef::WORLDSTATUS_REGISTER_LIMIT].vint);
		break;  // 只取一条记录 
	}

	for (; i != worldstate_list.end(); ++i)
	{
		i->Free();
	}

	return ret < 0 ? ret : 0;
}

int RMILoginObject::__SaveWorldstatus(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	int is_register_limit = 0;
	if (!in_param.Pop(&is_register_limit))
	{
		return rmi::DispatchParamError;
	}

	int result = SaveWorldstatus(is_register_limit);

	out_param->Push(result);

	return rmi::DispatchOK;
}
int RMILoginObject::SaveWorldstatus(int is_register_limit)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	DataNodeList node_list;

	DataNode node_reg(tabledef::WORLDSTATUS_REGISTER_LIMIT);
	node_reg.SetInt32(is_register_limit);
	node_list.push_back(node_reg);

	CmpNodeList condition;

	CmpNode for_index_node(tabledef::WORLDSTATUS_FOR_INDEX);
	for_index_node.SetLong64(1);
	condition.push_back(for_index_node);

	DBCommand *worldstatus_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_WORLDSTATUS);
	int count = worldstatus_command->Update(stmt, condition, node_list);
	if (count < 0)
	{
		ret = -1;
	}
	return ret < 0 ? ret : 0;
}


