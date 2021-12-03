#include "common/platform/system.h"
#include "common/platform/socket/pisocket.h"
#include "common/syspath.h"

#include "servercommon/servercommon.h"
#include "iclockmodule.h"
#include "inetworkmodule.h"
#include "ilogmodule.h"
#include "igamemodule.h"
#include "iconfigmodule.h"
#include "irmimodule.h"

#include "loginservermodule.h"
#include "server/protocal/msglogin.h"
#include "server/loginserver.h"
#include "engineadapter.h"
#include "internalcomm.h"
#include "utility/configpath.h"
#include "common/typetransform.h"

#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "servercommon/namefilter.hpp"

#include "gameserver/game.h"

#include "server/iplimit.hpp"
#include "config/loginconfig.h"

class ServerNetworkCallback : public IEngineNetCallback
{
public:
	ServerNetworkCallback(LoginServerModule *server) : m_server(server) {}
	~ServerNetworkCallback() {}

	virtual void OnAccept(Port listen_port, NetID netid, IP ip, Port port)
	{
		if (netid > 100000)	// 不可能有100000个用户以上连接上来的
		{
			m_server->m_network->Disconnect(netid);
			return;
		}

		if (netid >= m_server->m_connect_list.size())
		{
			while (netid >= m_server->m_connect_list.size())
			{
				m_server->m_connect_list.resize((m_server->m_connect_list.size() + 1) * 2);
			}
		}

		LoginServer::Instance().OnAccept(listen_port, netid, ip, netid);

		m_server->m_connect_list[netid].accept_time = m_server->m_current_time;
		m_server->m_connect_list[netid].last_active_time = m_server->m_current_time;
		m_server->m_connect_list[netid].netid = netid;
		m_server->m_connect_list[netid].ip = ip;
	}

	virtual void OnRecv(NetID netid, const char *data, int length)
	{
		if (netid >= m_server->m_connect_list.size()) return;

		IP ip = m_server->m_connect_list[netid].ip;

		static bool ip_limit = IPLimit::Instance().GetLimitSwitch();
		if (ip_limit)
		{
			sockaddr_in host_adr;
			memset(&host_adr, 0, sizeof(sockaddr_in));

			host_adr.sin_family = AF_INET;
			host_adr.sin_addr.s_addr = htonl(ip);
			std::string ip_str = inet_ntoa(host_adr.sin_addr);

			if (!IPLimit::Instance().CanPassIPLimit(ip_str))
			{
				return; 
			}
		}

		LoginServer::Instance().OnRecv(ip, netid, data, length);
		m_server->m_connect_list[netid].last_active_time = m_server->m_current_time;
	}

	virtual void OnDisconnect(NetID netid)
	{
		LoginServer::Instance().OnDisconnect(netid);
		m_server->m_connect_list[netid].Reset();
	}

	virtual void OnConnect(bool result, int handle, NetID netid, IP ip, Port port)
	{
	}

private:
	LoginServerModule *m_server;
};

class ServerInternalNetCallback : public IEngineNetCallback
{
public:
	ServerInternalNetCallback(LoginServerModule *server):m_server(server){}
	~ServerInternalNetCallback(){}

	void SetAllowIpPrefix(const std::string &allow_ip_prefix) { m_allow_ip_prefix = allow_ip_prefix; }

	virtual void OnAccept(Port listen_port, NetID netid, IP ip, Port port)
	{
		m_server->m_log.printf(LL_DEBUG, "Internal Network OnAccept netid:%d.", netid);

		// 检查IP前缀是否合法
		sockaddr_in host_adr;
		memset(&host_adr, 0, sizeof(sockaddr_in));
		host_adr.sin_family = AF_INET;
		host_adr.sin_addr.s_addr = htonl(ip);
		std::string ip_str = inet_ntoa(host_adr.sin_addr);
		std::string ip_record = ip_str;
		ip_str.resize(m_allow_ip_prefix.size());
		if (ip_str != m_allow_ip_prefix)
		{
			m_server->m_log.printf(LL_WARNING, "Internal Network Accept NOT allow ip[%s].", ip_record.c_str());
			m_server->m_internal_network->Disconnect(netid);
			return;
		}
		m_server->m_log.printf(LL_DEBUG, "Internal Network Accept ip[%s].", ip_record.c_str());

		if (listen_port == m_server->m_gs_listen_port)
		{
			m_server->m_log.printf(LL_INFO, "GameServer Accept.");
			LoginServer::Instance().OnGSAccept(listen_port, netid, ip, port);
			return;
		}

		if (listen_port == m_server->m_gateway_listen_port)
		{
			m_server->m_log.printf(LL_INFO, "GateWay Accept.");
			LoginServer::Instance().OnGateAccept(listen_port, netid, ip, port);
			return;
		}
	}

	virtual void OnRecv(NetID netid, const char *data, int length)
	{
		LoginServer::Instance().OnInnerRecv(netid, data, length);
	}

	virtual void OnDisconnect(NetID netid)
	{
		LoginServer::Instance().OnInnerDisconnect(netid);
	}

	virtual void OnConnect(bool result, int handle, NetID netid, IP ip, Port port)
	{
	}

private:
	LoginServerModule *m_server;
	std::string m_allow_ip_prefix;
};

LoginServerModule::LoginServerModule()
	: m_clock(0), m_game(0), m_network(0), m_internal_network(0), 
	m_current_time(0), m_network_callback(0), m_internal_network_callback(0),
	m_check_timeout(true),
	m_last_check_timeout(0),
	m_check_timeout_interval(3000),
	m_heart_time(15000),
	m_max_server_time(60000)
{
	m_network_callback = new ServerNetworkCallback(this);
	m_internal_network_callback = new ServerInternalNetCallback(this);
}

LoginServerModule::~LoginServerModule()
{
	delete m_network_callback;
	delete m_internal_network_callback;
}

void LoginServerModule::Free()
{
	delete this;
}

void LoginServerModule::StopServer()
{
	Game *game = (Game *)this->Interface();
	game->Stop();
}

int LoginServerModule::Init()
{
	srand(PITime());

	EXPECT_ON_INIT(LOG_MODULE)

	ILogModule *log = dynamic_cast<ILogModule *>(Interface()->QueryModule(LOG_MODULE));

	m_log.SetLogModule(log);
	m_log.SetCatagory(LOGIN_MODULE);

	return IModule::Succeed;
}

int LoginServerModule::Start()
{
	DEPEND_ON_START(CLOCK_MODULE)
	DEPEND_ON_START(NETWORK_MODULE)
	DEPEND_ON_START(GAME_MODULE)
	DEPEND_ON_START(CONFIG_MODULE)
	DEPEND_ON_START(INTERNAL_NETWORK)
	DEPEND_ON_START(RMI_MODULE)
	DEPEND_ON_START(TIMER_MODULE)
	
	ILogModule *log = dynamic_cast<ILogModule *>(Interface()->QueryModule(LOG_MODULE));
	IConfigModule *config = dynamic_cast<IConfigModule *>(Interface()->QueryModule(CONFIG_MODULE));
	m_clock = dynamic_cast<IClockModule*>(Interface()->QueryModule(CLOCK_MODULE));
	m_network = dynamic_cast<INetworkModule*>(Interface()->QueryModule(NETWORK_MODULE));
	m_internal_network = dynamic_cast<INetworkModule*>(Interface()->QueryModule(INTERNAL_NETWORK));
	m_game = dynamic_cast<IGameModule*>(Interface()->QueryModule(GAME_MODULE));
	m_rmi = dynamic_cast<IRMIModule*>(Interface()->QueryModule(RMI_MODULE));
	ITimerModule *timer = dynamic_cast<ITimerModule*>(Interface()->QueryModule(TIMER_MODULE));

	EngineAdapter::Instance().Init(log, config, m_game, m_network, m_clock, m_rmi, timer);

	m_current_time = m_clock->GetFrameTime();

	// 配置目录
	std::string config_dir = "../config";
	config->SyncValue(ROOT/"ConfigDir", &config_dir, config_dir);
	if (config_dir[config_dir.length() - 1] != '/')
	{
		config_dir += '/';
	}

	// 初始化全局配置
	char err_info[1024]={0};
	if(!ServerConfigPool::Instance().Init(config_dir,err_info))
	{
		m_log.printf(LL_ERROR, err_info);
		m_log.printf(LL_ERROR, "Login Server On Start, ServerConfigPool::Init return FALSE.");

		this->StopServer();

		return Fail;
	}
	m_log.printf(LL_INFO, "Login Server On Start, ServerConfigPool::Init Success.");

	std::string err;
	if (!LocalConfig::Instance().Init(config_dir + "local.xml", &err))
	{
		m_log.printf(LL_ERROR, err.c_str());
		m_log.printf(LL_ERROR, "Login Server On Start, LocalConfig::Init return FALSE.");
		
		this->StopServer();
		
		return Fail;
	}
	m_log.printf(LL_INFO, "Login Server On Start, LocalConfig::Init Success.");

	if (!LOGIN_CONFIG->Init(config_dir, &err))
	{
		m_log.printf(LL_ERROR, err.c_str());
		m_log.printf(LL_ERROR, "Login Server On Start, LoginConfig::Init return FALSE.");

		this->StopServer();

		return Fail;
	}

	std::string allow_ip_prefix = "127.0.0.1";
	config->SyncValue(ROOT/LOGIN_MODULE/"AllowIPPrefix", &allow_ip_prefix, allow_ip_prefix);
	m_internal_network_callback->SetAllowIpPrefix(allow_ip_prefix);

	m_network->RegisterCallback(m_network_callback);
	m_internal_network->RegisterCallback(m_internal_network_callback);

	InternalComm::Instance().Init(m_internal_network, ServerConfigPool::Instance().common_config.sessionkey);

	// 连接db
	if (!ConnectToDB())
	{
		config->write();

		this->StopServer();

		return Fail;
	}
	
	int min_login_interval_s = ServerConfigPool::Instance().common_config.login_server_minlogininterval_s;
	int can_not_login_global_time = ServerConfigPool::Instance().common_config.login_server_cannotloginglobaltime_s;

	// 从ServerConfigPool中读
	std::vector<int> index_list;
	for (int i = 0; i < ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBIndexCreateRole.IndexCount; ++i)
	{
		index_list.push_back(ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBIndexCreateRole.Indexs[i]);
	}

	// 敏感词配置路径
	std::string name_filter_config_dir = config_dir + "/serverconfig/namefilter.txt";

	// 初始化LoginServer
	config_dir += "loginserver/";
	if (!LoginServer::Instance().Init(index_list, log, config_dir, min_login_interval_s, can_not_login_global_time))
	{
		config->write();

		this->StopServer();

		return IModule::Fail;
	}

	// 为gs开listen端口
	if (!ListenForGameServer())
	{
		config->write();

		this->StopServer();

		return Fail;
	}

	// 为gateway开listen端口
	if (!ListenForGateWay())
	{
		config->write();

		this->StopServer();

		return Fail;
	}

	// 为玩家开listen端口
	if (!ListenForUser())
	{
		config->write();

		this->StopServer();

		return Fail;
	}

	// 初始化敏感词
	std::string error;
	if (!NameFilter::Instance().Init(name_filter_config_dir.c_str(), &error))
	{
		m_log.printf(LL_ERROR, "NameFilter Return Fail! Error At:%s", error.c_str());

		this->StopServer();

		return Fail;
	}

	// 登陆服务器还是一直检查的好
	m_check_timeout = true;	// ServerConfigPool::Instance().common_config.timeout.is_check_timeout;
	m_check_timeout_interval = ServerConfigPool::Instance().common_config.timeout.check_timeout_interval_ms;
	m_heart_time = ServerConfigPool::Instance().common_config.timeout.login_server_heartbeat_dead_time_ms;
	m_max_server_time = ServerConfigPool::Instance().common_config.timeout.login_server_max_server_time_ms;

	m_log.printf(LL_MAINTANCE, "Login Server started");

	config->write();

	SysPath::CreateFileByPath("status/login.start");

	return IModule::Succeed;
}

int LoginServerModule::Update()
{
	m_current_time = m_clock->GetFrameTime();

	if (m_check_timeout && (m_current_time - m_last_check_timeout > m_check_timeout_interval))
	{
		EraseTimeOut();
		m_last_check_timeout = m_current_time;
	}

	LoginServer::Instance().Update();

	return IModule::Pending;
}

int LoginServerModule::Stop()
{
	for (int i = 0; i < (int)m_db_rmi_session_list.size(); ++i)
	{
		m_rmi->CloseSession(m_db_rmi_session_list[i]);
	}

	SysPath::CreateFileByPath("status/login.end");

	return IModule::Succeed;
}

int LoginServerModule::Release()
{
	return IModule::Succeed;
}

bool LoginServerModule::ConnectToDB()
{
	std::string db_server_ip = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBAccounter.module_addr.ip;
	Port db_server_port = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBAccounter.module_addr.port;

	rmi::Session accounter_session = EngineAdapter::Instance().RMICreateSession(db_server_ip.c_str(), db_server_port);
	if (accounter_session.handle == 0)
	{
		m_log.printf(LL_MAINTANCE, "Connect Accounter DBLayer[%s:%d] Fail!", db_server_ip.c_str(), (int)db_server_port);
		return false;
	}
	m_db_rmi_session_list.push_back(accounter_session);
	m_log.printf(LL_MAINTANCE, "Connect Accounter DBLayer[%s:%d] Suc!", db_server_ip.c_str(), (int)db_server_port);

	unsigned int role_db_num = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBRoleCount;
	if (role_db_num == 0 || role_db_num > (unsigned int)DBHandler::MAX_ROLE_DB_INDEX)
	{
		m_log.printf(LL_MAINTANCE, "Role DBLayer NUM:%d is NOT valid!", role_db_num);
		return false;
	}

	rmi::Session role_db_session_list[DBHandler::MAX_ROLE_DB_INDEX];

	for (unsigned int i = 0; i < role_db_num; ++i)
	{
		std::string role_db_server_ip = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBRoleList[i].module_addr.ip;
		Port role_db_server_port = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBRoleList[i].module_addr.port;
		
		rmi::Session role_db = EngineAdapter::Instance().RMICreateSession(role_db_server_ip.c_str(), role_db_server_port);
		if (role_db.handle == 0)
		{
			m_log.printf(LL_MAINTANCE, "Connect Role DBLayer[%s:%d] Fail!", role_db_server_ip.c_str(), (int)role_db_server_port);
			return false;
		}

		m_db_rmi_session_list.push_back(role_db);
		role_db_session_list[i] = role_db;
	}

	std::string system_db_server_ip = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBSystem.module_addr.ip;
	Port system_db_server_port = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBSystem.module_addr.port;

	rmi::Session system_session = EngineAdapter::Instance().RMICreateSession(system_db_server_ip.c_str(), system_db_server_port);
	if (system_session.handle == 0)
	{
		m_log.printf(LL_MAINTANCE, "Connect System DBLayer[%s:%d] Fail!", system_db_server_ip.c_str(), (int)system_db_server_port);
		return false;
	}
	m_db_rmi_session_list.push_back(system_session);
	m_log.printf(LL_MAINTANCE, "Connect System DBLayer[%s:%d] Suc!", system_db_server_ip.c_str(), (int)system_db_server_port);

	std::string name_db_server_ip = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBName.module_addr.ip;
	Port name_db_server_port = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBName.module_addr.port;

	rmi::Session name_session = EngineAdapter::Instance().RMICreateSession(name_db_server_ip.c_str(), name_db_server_port);
	if (name_session.handle == 0)
	{
		m_log.printf(LL_MAINTANCE, "Connect Name DBLayer[%s:%d] Fail!", name_db_server_ip.c_str(), (int)name_db_server_port);
		return false;
	}
	m_db_rmi_session_list.push_back(name_session);
	m_log.printf(LL_MAINTANCE, "Connect Name DBLayer[%s:%d] Suc!", name_db_server_ip.c_str(), (int)name_db_server_port);

	InternalComm::Instance().SetDBSession(accounter_session, role_db_session_list, role_db_num, rmi::Session(), system_session, name_session);

	m_log.printf(LL_MAINTANCE, "Connect to DataAccessServer suc. Accounter[%s:%d], Role_db_num:%d", 
		db_server_ip.c_str(), (int)db_server_port, role_db_num);

	return true;
}

bool LoginServerModule::ListenForGameServer()
{
	m_gs_listen_port = ServerConfigPool::Instance().common_config.ipconfigs.login_server_addr.inner_connect_addr.for_gameserver.port;

	int inner_backlog = 64;
	EngineAdapter::Instance().ConfigSyncValue(ROOT/LOGIN_MODULE/"InnerComm"/"ForGameServer"/"Backlog", &inner_backlog, inner_backlog);
	bool ret = m_internal_network->Listen(m_gs_listen_port, inner_backlog);
	if (!ret)
	{
		m_log.print(LL_MAINTANCE, "LoginServer GS NetListen return FAIL!");
		return false;
	}
	m_log.printf(LL_MAINTANCE, "LoginServer GS Listen[%d].", m_gs_listen_port);

	return true;
}
bool LoginServerModule::ListenForGateWay()
{
	m_gateway_listen_port = ServerConfigPool::Instance().common_config.ipconfigs.login_server_addr.inner_connect_addr.for_gateway.port;

	int gw_backlog = 64;
	EngineAdapter::Instance().ConfigSyncValue(ROOT/LOGIN_MODULE/"InnerComm"/"ForGateWay"/"Backlog", &gw_backlog, gw_backlog);
	bool ret = m_internal_network->Listen(m_gateway_listen_port, gw_backlog);
	if (!ret)
	{
		m_log.print(LL_MAINTANCE, "LoginServer GateWay NetListen return FAIL!");
		return false;
	}
	m_log.printf(LL_MAINTANCE, "LoginServer GateWay Listen[%d].", m_gateway_listen_port);

	return true;
}

bool LoginServerModule::ListenForUser()
{
	Port listen_port = ServerConfigPool::Instance().common_config.ipconfigs.login_server_addr.user_connect_addr.port;

	int backlog = 64;
	EngineAdapter::Instance().ConfigSyncValue(ROOT/LOGIN_MODULE/"ForUser"/"Backlog", &backlog, backlog);
	bool ret = m_network->Listen(listen_port, backlog);
	if (!ret)
	{
		m_log.print(LL_MAINTANCE, "NetListen return FAIL!");
		return false;
	}
	m_log.printf(LL_MAINTANCE, "LoginServer Listen[%d].", listen_port);

	return true;
}

void LoginServerModule::EraseTimeOut()
{
	for (ConnectUserList::iterator i = m_connect_list.begin(); i != m_connect_list.end(); ++i)
	{
		if (i->netid != (NetID)(-1))
		{
			if (m_current_time - i->last_active_time > m_heart_time)
			{
				m_log.printf(LL_DEBUG, "EreasTimeOut Not Active.");
				m_network->Disconnect(i->netid);
			}

			if (m_current_time - i->accept_time > m_max_server_time)
			{
				m_log.printf(LL_DEBUG, "EreasTimeOut Max Server Time.");
				m_network->Disconnect(i->netid);
			}
		}
	}
}

