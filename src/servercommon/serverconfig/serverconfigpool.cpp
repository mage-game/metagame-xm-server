
#include "serverconfigpool.h"

bool ServerConfigPool::Init(const std::string &config_path, char *err_info)
{
	if (!common_config.LoadConfig((config_path+"serverconfig/commonconfig.xml").c_str(),err_info))
	{
		return false;
	}

	return true;
}

bool ServerConfigPool::GetLocalDBLConfig(int type, int index, std::string &fastdbname, unsigned int &db_init_size_MB, unsigned long &flush_interval,
										 std::string &dbusername,std::string &dbpassword,std::string &dbhost,unsigned short &dbport,
										 std::string &dbcharset,std::string &databasename, std::string &module_ip, unsigned short &module_port)
{
	DBModuleAddr *localdbconfig=0;
	switch (type)
	{
	case DBServerAddr::LDBLT_ACCOUNTER:
		{
			localdbconfig = &(common_config.ipconfigs.db_server_addr.DBAccounter);
			break;
		}
	case DBServerAddr::LDBLT_GLOBAL:
		{
			localdbconfig = &(common_config.ipconfigs.db_server_addr.DBGlobal);
			break;
		}
	case DBServerAddr::LDBLT_ROLE:
		{
			if (index < 0|| index >= common_config.ipconfigs.db_server_addr.DBRoleCount)
			{
				return false;
			}
			localdbconfig = &(common_config.ipconfigs.db_server_addr.DBRoleList[index]);
			break;
		}
	case DBServerAddr::LDBLT_SYSTEM:
		{
			localdbconfig = &(common_config.ipconfigs.db_server_addr.DBSystem);
			break;
		}
	case DBServerAddr::LDBLT_NAME:
		{
			localdbconfig = &(common_config.ipconfigs.db_server_addr.DBName);
			break;
		}
	case DBServerAddr::LDBLT_CROSS:
		{
			localdbconfig = &(common_config.ipconfigs.db_server_addr.DBCross);
			break;
		}
	default:
		return false;
	}

	fastdbname = localdbconfig->db_config.fast_db_name;
	db_init_size_MB = localdbconfig->db_config.fast_db_init_size_MB;
	flush_interval = localdbconfig->db_config.flushinterval;
	dbusername = localdbconfig->db_config.mysql_db_username;
	dbpassword = localdbconfig->db_config.mysql_db_password;
	dbhost = localdbconfig->db_config.mysql_addr.ip;
	dbport = localdbconfig->db_config.mysql_addr.port;
	dbcharset = localdbconfig->db_config.mysql_db_charset;
	databasename = localdbconfig->db_config.mysql_db_name;
	module_ip = localdbconfig->module_addr.ip;
	module_port = localdbconfig->module_addr.port;

	return true;
}

bool ServerConfigPool::GetLocalDBNoCacheTables(std::vector<std::string> &table_list)
{
	for (int i=0;i<common_config.ipconfigs.db_server_addr.NoCacheTables.TableCount && i<common_config.ipconfigs.db_server_addr.NoCacheTables.MaxTableNameCount;++i)
	{
		table_list.push_back(common_config.ipconfigs.db_server_addr.NoCacheTables.TableNames[i]);
	}
	return true;
}
bool ServerConfigPool::GetLocalDBNoDelayLoadTables(std::vector<std::string> &table_list)
{
	for (int i=0;i<common_config.ipconfigs.db_server_addr.NoDelayTables.TableCount && i<common_config.ipconfigs.db_server_addr.NoDelayTables.MaxTableNameCount;++i)
	{
		table_list.push_back(common_config.ipconfigs.db_server_addr.NoDelayTables.TableNames[i]);
	}
	return true;
}
