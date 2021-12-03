#ifndef __SERVERCONFIGPOOL_H__
#define __SERVERCONFIGPOOL_H__

#include <string>
#include <vector>

#include "configstruct.h"

class ServerConfigPool
{
public:
	~ServerConfigPool() {}

	static ServerConfigPool& Instance()
	{
		static ServerConfigPool config_pool;
		return config_pool;
	}

	bool Init(const std::string &config_path, char *err_info);

	bool GetLocalDBLConfig(int type, int index, std::string &fastdbname, unsigned int &db_init_size_MB, unsigned long &flush_interval,
		std::string &dbusername,std::string &dbpassword,std::string &dbhost,unsigned short &dbport,
		std::string &dbcharset,std::string &databasename, std::string &module_ip, unsigned short &module_port);

	bool GetLocalDBNoCacheTables(std::vector<std::string> &table_list);
	bool GetLocalDBNoDelayLoadTables(std::vector<std::string> &table_list);

	// IPConfig ipconfigs;
	CommonConfig common_config;

private:
	ServerConfigPool() {}
};

#endif // __SERVERCONFIGPOOL_H__

