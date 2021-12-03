
#include "configstruct.h"

//#define GetOrSetValue(NodeName,NodeValuePtr) (GetValue(NodeName,NodeValuePtr) || SetValue(NodeName,NodeValuePtr))
//#define GetOrSetListValue(ListName,ListValuePtr,ListCount,MaxListCount) (GetListValue(ListName,ListValuePtr,ListCount,MaxListCount) ||SetListValue(ListName,ListValuePtr,ListCount))

bool NetAddr::read()
{
	bool ret = true;
	if (is_show_ip)
	{
		ret = GetValue("Ip", &ip);
	}
	ret = ret && GetValue("Port",&port);

	return ret;
}
bool NetAddr::write()
{
	bool ret = true;
	if (is_show_ip)
	{
		ret = SetValue("Ip",&ip);
	}
	ret = ret && SetValue("Port",&port);
	
	return ret;
}

bool DBConfig::read()
{
	bool ret = GetValue("Fast_DB_Name",&fast_db_name)
		&& GetValue("Fast_DB_Init_Size_MB", &fast_db_init_size_MB)
		&& GetValue("Mysql_DB_Username",&mysql_db_username)
		&& GetValue("Mysql_DB_Password",&mysql_db_password)
		&& GetValue("Mysql_Addr",(ConfigElementBase*)&mysql_addr)
		&& GetValue("Mysql_DB_Charset",&mysql_db_charset)
		&& GetValue("Mysql_DB_Name",&mysql_db_name)
		&& GetValue("FlushInterval",&flushinterval);
	return ret;
}
bool DBConfig::write()
{
	bool ret = SetValue("Fast_DB_Name",&fast_db_name)
		&& SetValue("Fast_DB_Init_Size_MB", &fast_db_init_size_MB)
		&& SetValue("Mysql_DB_Username",&mysql_db_username)
		&& SetValue("Mysql_DB_Password",&mysql_db_password)
		&& SetValue("Mysql_Addr",(ConfigElementBase*)&mysql_addr)
		&& SetValue("Mysql_DB_Charset",&mysql_db_charset)
		&& SetValue("Mysql_DB_Name",&mysql_db_name)
		&& SetValue("FlushInterval",&flushinterval);
	return ret;
}

bool DBModuleAddr::read()
{
	bool ret = true;
	if (is_show_index)
	{
		ret = GetValue("Index",&index);
	}
	ret = ret && GetValue("DB_Config",(ConfigElementBase*)&db_config)
		&& GetValue("Module_Addr",(ConfigElementBase*)&module_addr);
	return ret;
}
bool DBModuleAddr::write()
{
	bool ret = true;
	if (is_show_index)
	{
		ret = SetValue("Index",&index);
	}
	ret = ret && SetValue("DB_Config",(ConfigElementBase*)&db_config)
		&& SetValue("Module_Addr",(ConfigElementBase*)&module_addr);
	return ret;
}

bool TableNameList::read()
{
	bool ret = GetListValue("Table",TableNames,TableCount,MaxTableNameCount);
	return ret;
}
bool TableNameList::write()
{
	bool ret = SetListValue("Table",TableNames,TableCount);
	return ret;
}

bool IndexList::read()
{
	bool ret = GetListValue("Index",Indexs,IndexCount,MAX_INDEX_COUNT);
	assert(IndexCount > 0);
	assert(IndexCount < MAX_INDEX_COUNT);
	return ret;
}
bool IndexList::write()
{
	bool ret = SetListValue("Index",Indexs,IndexCount);
	return ret;
}

bool DBServerAddr::read()
{
	bool ret = GetValue("NoCacheTable",(ConfigElementBase*)&NoCacheTables)
		&& GetValue("IsDelayLoad", &IsDelayLoad)
		&& GetValue("NoDelayTables",(ConfigElementBase*)&NoDelayTables)
		&& GetValue("DBAccounter",(ConfigElementBase*)&DBAccounter)
		&& GetValue("DBName", (ConfigElementBase*)&DBName)
		&& GetValue("DBGlobal", (ConfigElementBase*)&DBGlobal)
		&& GetValue("DBSystem",(ConfigElementBase*)&DBSystem)
		&& GetValue("DBIndexCreateRole",(ConfigElementBase*)&DBIndexCreateRole)
		&& GetListValue("DBRole",(ConfigElementBase*)DBRoleList,DBRoleCount,MaxDBRoleCount)
		&& GetValue("DBCross", (ConfigElementBase*)&DBCross);
	check();
	return ret;
}
void DBServerAddr::check()
{
	assert(DBRoleCount > 0);
	assert(DBRoleCount < MaxDBRoleCount);
	assert(DBRoleCount <= DBIndexCreateRole.IndexCount);
	for (int i= 0;i<DBRoleCount;i++)
	{
		// 保证是连续并且递增的
		assert(i==DBRoleList[i].index);	
	}
	for (int i = 0; i < DBIndexCreateRole.IndexCount; ++i)
	{
		assert(DBIndexCreateRole.Indexs[i] < DBRoleCount);
	}
}
bool DBServerAddr::write()
{
	bool ret = SetValue("NoCacheTable",(ConfigElementBase*)&NoCacheTables)
		&& SetValue("IsDelayLoad", &IsDelayLoad)
		&& SetValue("NoDelayTables",(ConfigElementBase*)&NoDelayTables)
		&& SetValue("DBAccounter",(ConfigElementBase*)&DBAccounter)
		&& SetValue("DBName", (ConfigElementBase*)&DBName)
		&& SetValue("DBGlobal", (ConfigElementBase*)&DBGlobal)
		&& SetValue("DBSystem",(ConfigElementBase*)&DBSystem)
		&& SetValue("DBIndexCreateRole",(ConfigElementBase*)&DBIndexCreateRole)
		&& SetListValue("DBRole",(ConfigElementBase*)DBRoleList,DBRoleCount)
		&& SetValue("DBCross", (ConfigElementBase*)&DBCross);
	return ret;
}

bool RankServerAddr::read()
{
	bool ret = GetValue("Addr",(ConfigElementBase*)&addr);
	return ret;
}
bool RankServerAddr::write()
{
	bool ret = SetValue("Addr",(ConfigElementBase*)&addr);
	return ret;
}

bool CenterServerAddr::read()
{
	bool ret = GetValue("Addr",(ConfigElementBase*)&addr);
	return ret;
}
bool CenterServerAddr::write()
{
	bool ret = SetValue("Addr",(ConfigElementBase*)&addr);
	return ret;
}

bool LoginServerInnerAddr::read()
{
	bool ret = GetValue("Server_Ip", &server_ip)
		&& GetValue("For_Global",(ConfigElementBase*)&for_global)
		&& GetValue("For_Gameserver",(ConfigElementBase*)&for_gameserver)
		&& GetValue("For_Gateway",(ConfigElementBase*)&for_gateway);
	return ret;
}
bool LoginServerInnerAddr::write()
{
	bool ret = SetValue("Server_Ip", &server_ip)
		&& SetValue("For_Global",(ConfigElementBase*)&for_global)
		&& SetValue("For_Gameserver",(ConfigElementBase*)&for_gameserver)
		&& SetValue("For_Gateway",(ConfigElementBase*)&for_gateway);
	return ret;
}

bool LoginServerAddr::read()
{
	bool ret = GetValue("Inner_Connect_Addr",(ConfigElementBase*)&inner_connect_addr)
		&& GetValue("User_Connect_Addr",(ConfigElementBase*)&user_connect_addr);
	return ret;
}
bool LoginServerAddr::write()
{
	bool ret = SetValue("Inner_Connect_Addr",(ConfigElementBase*)&inner_connect_addr)
		&& SetValue("User_Connect_Addr",(ConfigElementBase*)&user_connect_addr);
	return ret;
}

bool IPConfig::read()
{
	bool ret = GetValue("DB_Server_Addr",(ConfigElementBase*)&db_server_addr)
		&& GetValue("Rank_Server_Addr",(ConfigElementBase*)&rank_server_addr)
		&& GetValue("Center_Server_Addr",(ConfigElementBase*)&center_server_addr)
		&& GetValue("Login_Server_Addr",(ConfigElementBase*)&login_server_addr);
	return ret;
}

bool IPConfig::write()
{
	bool ret = SetValue("DB_Server_Addr",(ConfigElementBase*)&db_server_addr)
		&& SetValue("Rank_Server_Addr",(ConfigElementBase*)&rank_server_addr)
		&& SetValue("Center_Server_Addr",(ConfigElementBase*)&center_server_addr)
		&& SetValue("Login_Server_Addr",(ConfigElementBase*)&login_server_addr);
	return ret;
}

bool ServerTimeOut::read()
{
	bool ret = GetValue("IS_Check_TimeOut",&is_check_timeout)
		&& GetValue("Check_TimeOut_Interval_MS",&check_timeout_interval_ms)
		&& GetValue("LoginServer_Heartbeat_DeadTime_MS",&login_server_heartbeat_dead_time_ms)
		&& GetValue("Gateway_Heartbeat_DeadTime_MS",&gateway_heartbeat_dead_time_ms)
		&& GetValue("Gameworld_Heartbeat_DeadTime_MS",&gameworld_heartbeat_dead_time_ms)
		&& GetValue("LoginServer_Max_Server_Time_MS",&login_server_max_server_time_ms);
	return ret;
}

bool ServerTimeOut::write()
{
	bool ret = SetValue("IS_Check_TimeOut",&is_check_timeout)
		&& SetValue("Check_TimeOut_Interval_MS",&check_timeout_interval_ms)
		&& SetValue("LoginServer_Heartbeat_DeadTime_MS",&login_server_heartbeat_dead_time_ms)
		&& SetValue("Gateway_Heartbeat_DeadTime_MS",&gateway_heartbeat_dead_time_ms)
		&& SetValue("Gameworld_Heartbeat_DeadTime_MS",&gameworld_heartbeat_dead_time_ms)
		&& SetValue("LoginServer_Max_Server_Time_MS",&login_server_max_server_time_ms);
	return ret;
}

bool CommonConfig::read()
{
	bool ret = GetValue("IPConfig",(ConfigElementBase*)&ipconfigs)
		&& GetValue("SessionKey",&sessionkey)
		&& GetValue("GlobalServer_MaxLoginInterval_S",&global_server_maxlogininterval_s)
		&& GetValue("LoginServer_MinLoginInterval_S",&login_server_minlogininterval_s)
		&& GetValue("LoginServer_CannotLoginGlobalTime_S",&login_server_cannotloginglobaltime_s)
		&& GetValue("LoginServer_RoleNumRecordInterval",&login_server_rolenumrecordinterval_s)
		&& GetValue("LoginServer_AccountCheckType",&login_server_account_check_type)
		&& GetValue("GameWorld_Scene_Status_Log_Interval",&game_world_scene_status_log_interval_s)
		&& GetValue("YouKe_AntiWallow",&youke_anti_wallow)
		&& GetValue("SystemCmdCheckInterval",&system_cmd_check_interval_s)
		&& GetValue("TimeOut",(ConfigElementBase*)&timeout)
		&& GetValue("ChatChannelCD", (ConfigElementBase*)&chat_channel_cd)
		&& GetValue("Wallow", (ConfigElementBase*)&wallow_config)
		&& GetValue("dynamic_cfg_path", &dynamic_cfg_path)
		&& GetValue("namefilter_open_flag", &namefilter_open_flag)
		&& GetValue("is_micro_pc_open", &is_micro_pc_open);

	return ret;
}

bool CommonConfig::write()
{
	bool ret = SetValue("IPConfig",(ConfigElementBase*)&ipconfigs)
		&& SetValue("SessionKey",&sessionkey)
		&& SetValue("GlobalServer_MaxLoginInterval_S",&global_server_maxlogininterval_s)
		&& SetValue("LoginServer_MinLoginInterval_S",&login_server_minlogininterval_s)
		&& SetValue("LoginServer_CannotLoginGlobalTime_S",&login_server_cannotloginglobaltime_s)
		&& SetValue("LoginServer_RoleNumRecordInterval",&login_server_rolenumrecordinterval_s)
		&& SetValue("LoginServer_AccountCheckType",&login_server_account_check_type)
		&& SetValue("GameWorld_Scene_Status_Log_Interval",&game_world_scene_status_log_interval_s)
		&& SetValue("YouKe_AntiWallow",&youke_anti_wallow)
		&& SetValue("SystemCmdCheckInterval",&system_cmd_check_interval_s)
		&& SetValue("TimeOut",(ConfigElementBase*)&timeout)
		&& SetValue("ChatChannelCD", (ConfigElementBase*)&chat_channel_cd)
		&& SetValue("Wallow", (ConfigElementBase*)&wallow_config)
		&& SetValue("unique_dynamic_cfg_path", &dynamic_cfg_path)
		&& SetValue("namefilter_open_flag", &namefilter_open_flag)
		&& SetValue("is_micro_pc_open", &is_micro_pc_open);

	return ret;
}

bool CommonConfig::LoadConfig(const char* filename, char *err_info)
{
	// 加载配置
	if (!m_document.LoadFile(filename) || 0 == m_document.FirstChildElement("config")|| 0 == m_document.FirstChildElement("config")->FirstChildElement())
	{
		sprintf(err_info,"Load xml config %s fail.%s", filename, m_document.ErrorDesc());
		return false;
		//TiXmlElement tmp_root("config");
		//m_document.InsertEndChild(tmp_root);
		//m_document.SaveFile();
		//if (!m_document.LoadFile(filename))
		//{
		//	sprintf(err_info,"%s",m_document.ErrorDesc());
		//	return false;
		//}
		//SetElement(m_document.RootElement());
		//if (!write()) return false;
		//m_document.SaveFile();
	}

	TiXmlElement *root = m_document.FirstChildElement("config");
	SetElement(root);

	// 读取配置
	if (!read())
	{
		sprintf(err_info,"Read xml config fail.");
		return false;
	}
	//m_document.SaveFile();

	return true;
}

bool ChatChannelCD::read()
{
	bool ret = GetValue("World", &world_cd)
		&& GetValue("Country",&country_cd);
	return ret;
}
bool ChatChannelCD::write()
{
	bool ret = SetValue("World", &world_cd)
		&& SetValue("Country",&country_cd);
	return ret;
}

bool WallowConfig::read()
{
	bool ret = GetValue("ServerCheck", &server_check)
		&& GetValue("CheckIDRepeate",&check_repeate)
		&& GetValue("TimerInterval",&timer_interval)
		&& GetValue("KickTime",&kick_time)
		&& GetValue("NoticeAheadOfTime",&notice_ahead_of_time)
		&& GetValue("OneHourNoticeTime",&one_hour_notice_time)
		&& GetValue("TwoHourNoticeTime",&two_hour_notice_time)
		&& GetValue("AntiWallowRestTime",&anti_wallow_rest_time)
		&& GetValue("IsOpenAntiWallow",&is_open_anti_wallow);
	return ret;
}
bool WallowConfig::write()
{
	bool ret = SetValue("ServerCheck", &server_check)
		&& SetValue("CheckIDRepeate",&check_repeate)
		&& SetValue("TimerInterval",&timer_interval)
		&& SetValue("KickTime",&kick_time)
		&& SetValue("NoticeAheadOfTime",&notice_ahead_of_time)
		&& SetValue("OneHourNoticeTime",&one_hour_notice_time)
		&& SetValue("TwoHourNoticeTime",&two_hour_notice_time)
		&& SetValue("AntiWallowRestTime",&anti_wallow_rest_time)
		&& SetValue("IsOpenAntiWallow",&is_open_anti_wallow);
	return ret;
}

