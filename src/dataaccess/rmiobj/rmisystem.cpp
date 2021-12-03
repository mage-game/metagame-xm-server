#include "rmisystem.h"
#include "servercommon/darmi/dasystem.h"

#include "databaselayer.h"
#include "engineadapter.h"
#include "dataaccesslog.hpp"

#include "servercommon/struct/system/publicnoticelist.h"
#include "servercommon/struct/system/cmdlist.h"

#include "servercommon/servercommon.h"

USE_DATAACCESS_VAR();
OLD_USE_INTERNAL_NETWORK();

RMISystemObject::RMISystemObject()
{
	m_method_list[dataaccessrmi::system::GET_PUBLIC_NOTICE] = __GetPublicNotice;
	m_method_list[dataaccessrmi::system::GET_CMD] = __GetCmd;
	m_method_list[dataaccessrmi::system::SWAP_ROLE] = __SwapRole;
}

RMISystemObject::~RMISystemObject()
{
}

void RMISystemObject::__free()
{
	delete this;
}

const char * RMISystemObject::__get_obj_id()
{
	return dataaccessrmi::SYSTEM;
}

int RMISystemObject::__GetPublicNotice(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < 8192)
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	PublicNoticeList param;
	param.count = 0;

	int result = GetPublicNotice(&param);
	out_param->Push(result);
	if (result == 0)
	{
		param.Serialize(*out_param);
	}

	return rmi::DispatchOK;
}

int RMISystemObject::GetPublicNotice(PublicNoticeList *notice_list)
{
	INIT_STMT(conn, stmt, -1)

	conn->begin(false);

	DBCommand *publicnoticecommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_PUBLICNOTICE);

	long long now = (long long)EngineAdapter::Instance().Time();

	QueryCondition notice_condition;
	notice_condition.query_num = PublicNoticeList::MAX_PUBLICNOTICE_NUM;

	CmpNode notice_node1(tabledef::PUBLICNOTICE_IS_FORBID);
	notice_node1.SetInt32(0);
	notice_node1.SetEqualType(EQUAL_TYPE_EQUAL);
	notice_condition.cmp_list.push_back(notice_node1);

	CmpNode notice_node2(tabledef::PUBLICNOTICE_END_SEND_TIME);
	notice_node2.SetLong64(now);
	notice_node2.SetEqualType(EQUAL_TYPE_GRANTER);
	notice_condition.cmp_list.push_back(notice_node2);

	DataAdapterList publicnotice_list;
	int ret = publicnoticecommand->Find(stmt, notice_condition, &publicnotice_list, true, false);
	if (ret < 0)
	{
		conn->rollback();
		return ret;
	}

	int count = 0;
	DataAdapterList::iterator i = publicnotice_list.begin(); 

	for (; i != publicnotice_list.end() && count < PublicNoticeList::MAX_PUBLICNOTICE_NUM; ++i)
	{
		notice_list->notice_list[count].notice_id = i->m_data_area[tabledef::PUBLICNOTICE_IDPUBLICNOTICE].vint;
		notice_list->notice_list[count].creator = i->m_data_area[tabledef::PUBLICNOTICE_CREATOR].vcharp;
		notice_list->notice_list[count].notice = i->m_data_area[tabledef::PUBLICNOTICE_CONTENT].vcharp;
		notice_list->notice_list[count].type = i->m_data_area[tabledef::PUBLICNOTICE_TYPE].vint;
		notice_list->notice_list[count].color = i->m_data_area[tabledef::PUBLICNOTICE_COLOR].vint;
		notice_list->notice_list[count].urgent = i->m_data_area[tabledef::PUBLICNOTICE_IS_URGENT].vint;
		notice_list->notice_list[count].begin_time = i->m_data_area[tabledef::PUBLICNOTICE_BEGIN_SEND_TIME].vint64;
		notice_list->notice_list[count].end_time = i->m_data_area[tabledef::PUBLICNOTICE_END_SEND_TIME].vint64;
		notice_list->notice_list[count].send_interval = i->m_data_area[tabledef::PUBLICNOTICE_SEND_INTERVAL].vint64;

		if (notice_list->notice_list[count].urgent)
		{
			if (now > i->m_data_area[tabledef::PUBLICNOTICE_NEXT_SEND_TIME].vint64)
			{
				++ count;
				
				i->m_data_area[tabledef::PUBLICNOTICE_NEXT_SEND_TIME].vint64 = now + i->m_data_area[tabledef::PUBLICNOTICE_SEND_INTERVAL].vint64;
				publicnoticecommand->Update(stmt, *i, false);
			}
		}
		else 
		{
			++ count;
		}
		
		i->Free();
	}

	for (; i != publicnotice_list.end(); ++i)
	{
		i->Free();
	}

	notice_list->count = count;

	conn->commit();

	return 0;
}

int RMISystemObject::__GetCmd(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < 8192)
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	char type = 0;
	bool ret = in_param.Pop(&type);
	if (!ret) return rmi::DispatchParamError;

	CmdList param;
	param.count = 0;

	int result = GetCmd(type, &param);
	out_param->Push(result);
	if (result == 0)
	{
		param.Serialize(*out_param);
	}

	return rmi::DispatchOK;
}
int RMISystemObject::GetCmd(char type, CmdList *cmd_list)
{
	INIT_STMT(conn, stmt, -1)

	conn->begin(false);

	DBCommand *cmdcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_COMMAND);

	QueryCondition cmd_condition;
	cmd_condition.query_num = CmdList::MAX_CMD_NUM;

	CmpNode cmd_node1(tabledef::COMMAND_CONFIRMTIME);
	cmd_node1.SetInt32(0);
	cmd_condition.cmp_list.push_back(cmd_node1);

	CmpNode cmd_node2(tabledef::COMMAND_TYPE);
	cmd_node2.SetByte(type);
	cmd_condition.cmp_list.push_back(cmd_node2);

	DataAdapterList command_list;
	int ret = cmdcommand->Find(stmt, cmd_condition, &command_list, true, false);
	if (ret < 0)
	{
		conn->rollback();
		return ret;
	}

	long long now = (long long)EngineAdapter::Instance().Time();

	int count = 0;
	DataAdapterList::iterator i = command_list.begin(); 

	for (; i != command_list.end() && count < CmdList::MAX_CMD_NUM; ++i)
	{
		cmd_list->cmd_list[count].cmd_id = i->m_data_area[tabledef::COMMAND_IDCOMMAND].vint;
		cmd_list->cmd_list[count].creator = i->m_data_area[tabledef::COMMAND_CREATOR].vcharp;
		cmd_list->cmd_list[count].cmd = i->m_data_area[tabledef::COMMAND_CMD].vcharp;
		++count;

		i->m_data_area[tabledef::COMMAND_CONFIRMTIME].vint64 = now;
		cmdcommand->Update(stmt, *i, false);

		i->Free();
	}

	for (; i != command_list.end(); ++i)
	{
		i->Free();
	}

	cmd_list->count = count;

	conn->commit();

	return 0;
}

int RMISystemObject::__SwapRole(TLVUnserializer& in_param, TLVSerializer* out_param)
{
	int role_id1 = 0;
	int role_id2 = 0;
	int ret = 0;

	do 
	{
		if (!in_param.Pop(&role_id1))
		{
			ret = -1;
			break;
		}
		if (!in_param.Pop(&role_id2))
		{
			ret = -2;
			break;
		}
		ret = RMISystemObject::SwapRole(role_id1, role_id2);
	} while (false);

	out_param->Push(ret);
	return ret;
}

int RMISystemObject::SwapRole(int role_id1, int role_id2)
{
	INIT_STMT(conn, stmt, -1)
	conn->begin(false);

	int ret;
	std::string platname_1;
	std::string platname_2;
	do
	{
		ret = RMISystemObject::GetRolePlatName(stmt, role_id1, platname_1);
		if (ret < 0)
		{
			ret -= 10000;
			break;
		}
		ret = RMISystemObject::GetRolePlatName(stmt, role_id2, platname_2);
		if (ret < 0)
		{
			ret -= 20000;
			break;
		}
		if (platname_1 == platname_2)
		{
			ret -= 30000;
			break;
		}
		ret = RMISystemObject::SwapLoginRole(stmt, platname_1, role_id1, platname_2, role_id2);
		if (ret < 0)
		{
			ret -= 40000;
			break;
		}
		ret = RMISystemObject::SwapRoleNameMap(stmt, platname_1, role_id1, platname_2, role_id2);
		if (ret < 0)
		{
			ret -= 50000;
			break;
		}
		ret = RMISystemObject::SwapRoleAccountGold(stmt, platname_1, role_id1, platname_2, role_id2);
		if (ret < 0)
		{
			ret -= 60000;
			break;
		}

	} while (false);

	if (ret < 0)
	{
		if (!conn->rollback())
		{
			ret -= 700000;
		}
	}
	else
	{
		if (!conn->commit())
		{
			ret = -800000;
		}
	}

	dataaccesslog::g_log_system.printf(LL_INFO, "RMISystemObject::SwapRole( %d , %d ) ret: %d", role_id1, role_id2, ret);
	return ret;
}

int RMISystemObject::GetRolePlatName(IStatement* stmt, int role_id, std::string& out_platname)
{
	if (!stmt)
	{
		return -101;
	}
	const MataTableDef *nametable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_NAME_MAP);
	if (!nametable)
	{
		return -102;
	}
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_NAME_MAP);
	if (!command)
	{
		return -103;
	}

	QueryCondition condition;
	CmpNode node(tabledef::ROLE_NAME_MAP_ROLE_ID);
	node.SetInt32(role_id);
	condition.cmp_list.push_back(node);

	DataAdapter namedata = nametable->GetPrototype();
	namedata.Malloc();
	int ret = 0;
	do 
	{
		ret = command->Find(stmt, condition, &namedata);
		if (DBCommand::RESULT_SUC != ret)
		{
			break;
		}
		out_platname = namedata.m_data_area[tabledef::ROLE_NAME_MAP_PLAT_USER_NAME].vcharp;
	} while (false);
	namedata.Free();
	return ret;
}

int RMISystemObject::GetPlatNameLoginInfo(IStatement* stmt, const std::string& in_platname, DataAdapter* out_da)
{
	if (!stmt)
	{
		return -101;
	}
	if (!out_da)
	{
		return -102;
	}
	//const MataTableDef *logintable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_LOGIN);
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_LOGIN);
	if (!command)
	{
		return -103;
	}

	QueryCondition condition;

	CmpNode node(tabledef::LOGIN_PLAT_USER_NAME);
	node.SetString(in_platname);
	condition.cmp_list.push_back(node);

	return command->Find(stmt, condition, out_da);
}

static int GetRoleField(int role_id, const DataAdapter* in_da, tabledef::TABLE_LOGIN_FIELDS& out_field_role, tabledef::TABLE_LOGIN_FIELDS& out_field_forbid)
{
	if (!in_da)
	{
		return -1;
	}
	if (in_da->m_data_area[tabledef::LOGIN_ROLE_ID_1].vint == role_id)
	{
		out_field_forbid = tabledef::LOGIN_FORBID_TIME_1;
		out_field_role = tabledef::LOGIN_ROLE_ID_1;
	}else if (in_da->m_data_area[tabledef::LOGIN_ROLE_ID_2].vint == role_id)
	{
		out_field_forbid = tabledef::LOGIN_FORBID_TIME_2;
		out_field_role = tabledef::LOGIN_ROLE_ID_2;
	}
	else if (in_da->m_data_area[tabledef::LOGIN_ROLE_ID_3].vint == role_id)
	{
		out_field_forbid = tabledef::LOGIN_FORBID_TIME_3;
		out_field_role = tabledef::LOGIN_ROLE_ID_3;
	}
	else
	{
		return -2;
	}
	return 0;
}

int RMISystemObject::SwapLoginRole(IStatement* stmt, const std::string& platname1, int role_id1, const std::string& platname2, int role_id2)
{
	if (!stmt)
	{
		return -1001;
	}
	const MataTableDef* logintable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_LOGIN);
	if (!logintable)
	{
		return -1002;
	}
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_LOGIN);
	if (!command)
	{
		return -1003;
	}
	DataAdapter logindata1 = logintable->GetPrototype();
	logindata1.Malloc();
	int ret = 0;
	ret = GetPlatNameLoginInfo(stmt, platname1, &logindata1);
	if (ret == 0)
	{//ret == 0
		DataAdapter logindata2 = logintable->GetPrototype();
		logindata2.Malloc();
		ret = GetPlatNameLoginInfo(stmt, platname2, &logindata2);
		if (ret < 0)
		{//goto free
			ret -= 2000;
		}
		else
		{//ret == 0 
			do
			{
				tabledef::TABLE_LOGIN_FIELDS role_id1_field_role;
				tabledef::TABLE_LOGIN_FIELDS role_id1_field_forbid;
				ret = GetRoleField(role_id1, &logindata1, role_id1_field_role, role_id1_field_forbid);
				if (ret < 0)
				{
					ret -= 3000;
					break;
				}
				tabledef::TABLE_LOGIN_FIELDS role_id2_field_role;
				tabledef::TABLE_LOGIN_FIELDS role_id2_field_forbid;
				ret = GetRoleField(role_id2, &logindata2, role_id2_field_role, role_id2_field_forbid);
				if (ret < 0)
				{
					ret -= 4000;
					break;
				}

				std::swap(logindata1.m_data_area[role_id1_field_role], logindata2.m_data_area[role_id2_field_role]);
				std::swap(logindata1.m_data_area[role_id1_field_forbid], logindata2.m_data_area[role_id2_field_forbid]);
				if (command->Update(stmt, logindata1) != DBCommand::RESULT_SUC)
				{
					ret -= 5000;
					break;
				}
				if (command->Update(stmt, logindata2) != DBCommand::RESULT_SUC)
				{
					ret -= 6000;
					break;
				}
			} while (false);
		}
		logindata2.Free();
	}
	logindata1.Free();

	return ret;
}

int RMISystemObject::SwapRoleNameMap(IStatement* stmt, const std::string& platname1, int role_id1, const std::string& platname2, int role_id2)
{
	if (!stmt)
	{
		return -1001;
	}
	int ret = 0;
	ret = RMISystemObject::UpdateRoleNameMap(stmt, platname1, role_id2);
	if ( ret < 0)
	{
		ret -= 2000;
		return ret;
	}
	ret = RMISystemObject::UpdateRoleNameMap(stmt, platname2, role_id1);
	if ( ret < 0)
	{
		ret -= 3000;
		return ret;
	}
	return 0;
}

int RMISystemObject::UpdateRoleNameMap(IStatement* stmt, const std::string& platname, int role_id)
{
	if (!stmt)
	{
		return -101;
	}
	//const MataTableDef* namemaptable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_NAME_MAP);
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_NAME_MAP);
	if (!command)
	{
		return -102;
	}

	DataNodeList name_update;

	DataNode node_platname(tabledef::ROLE_NAME_MAP_PLAT_USER_NAME);
	node_platname.SetString(platname);
	name_update.push_back(node_platname);

	CmpNodeList condition;
	CmpNode node_role_id(tabledef::ROLE_NAME_MAP_ROLE_ID);
	node_role_id.SetInt32(role_id);
	condition.push_back(node_role_id);

	int count = command->Update(stmt, condition, name_update);
	if (count < 1)
	{
		return -2;
	}
	return 0;
}

int RMISystemObject::SwapRoleAccountGold(IStatement* stmt, const std::string& platname1, int role_id1, const std::string& platname2, int role_id2)
{
	if (!stmt)
	{
		return -1001;
	}
	int ret = 0;
	ret = RMISystemObject::UpdateAccountGold(stmt, platname1, role_id2);
	if (ret < 0)
	{
		ret -= 2000;
		return ret;
	}
	ret = RMISystemObject::UpdateAccountGold(stmt, platname2, role_id1);
	if (ret < 0)
	{
		ret -= 3000;
		return ret;
	}
	return 0;
}

int RMISystemObject::UpdateAccountGold(IStatement* stmt, const std::string& platname, int role_id)
{
	if (!stmt)
	{
		return -101;
	}
	//const MataTableDef* namemaptable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ACCOUNTGOLD);
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ACCOUNTGOLD);
	if (!command)
	{
		return -102;
	}

	DataNodeList name_update;

	DataNode node_platname(tabledef::ACCOUNTGOLD_PLAT_USER_NAME);
	node_platname.SetString(platname);
	name_update.push_back(node_platname);

	CmpNodeList condition;
	CmpNode node_role_id(tabledef::ACCOUNTGOLD_ROLE_ID);
	node_role_id.SetInt32(role_id);
	condition.push_back(node_role_id);

	int ret = command->Update(stmt, condition, name_update);
	if (ret < 0)
	{
		return ret;
	}
	return 0;
}
