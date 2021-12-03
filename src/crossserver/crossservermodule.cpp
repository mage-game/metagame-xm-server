#include "crossservermodule.h"

#include "common/platform/system.h"
#include "common/platform/socket/pisocket.h"

#include "iclockmodule.h"
#include "inetworkmodule.h"
#include "ilogmodule.h"
#include "igamemodule.h"
#include "iconfigmodule.h"

#include "gameserver/game.h"

#include "engineadapter.h"
#include "internalcomm.h"

#include "utility/configpath.h"
#include "common/typetransform.h"
#include "common/syspath.h"

#include "servercommon/servercommon.h"
#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/string/crossstr.h"

#include "crossserver/crosslog.hpp"

#include "config/crossserverconfig.h"
#include "config/sharedconfig.h"
#include "servercommon/serverconfig/localconfig.hpp"

class ServerInternalNetCallback : public IEngineNetCallback
{
public:
	ServerInternalNetCallback(CrossServerModule *server_module): m_server_module(server_module)
	{
		m_cross_server = &m_server_module->m_cross_server;
	}

	virtual ~ServerInternalNetCallback() {}

	virtual void OnAccept(Port listen_port, NetID netid, IP ip, Port port)
	{
		m_server_module->m_log.printf(LL_DEBUG, "ServerInternalNetworkCallback::ON_ACCEPT");

		m_cross_server->OnInternalGameServerAccept(listen_port, netid, ip, port);
	}

	virtual void OnRecv(NetID netid, const char *data, int length)
	{
		m_cross_server->OnInternalRecv(netid, data, length);
	}

	virtual void OnDisconnect(NetID netid)
	{
		m_server_module->m_log.printf(LL_DEBUG, "ServerInternalNetCallback::ON_DISCONNECT");

		m_cross_server->OnInternalDisconnect(netid);
	}

	virtual void OnConnect(bool result, int handle, NetID netid, IP ip, Port port)
	{
		m_server_module->m_log.printf(LL_DEBUG, "Internal Network OnConnect netid:%d.", netid);
	}

private:
	CrossServerModule *m_server_module;

	CrossServer *m_cross_server;
};

CrossServerModule::CrossServerModule()
	: m_clock(NULL), m_game(NULL), m_timer(NULL), m_internal_network_callback(NULL), m_current_frame(0)
{	
	m_internal_network_callback = new ServerInternalNetCallback(this);
}

CrossServerModule::~CrossServerModule()
{	
	delete m_internal_network_callback;
}

void CrossServerModule::Free()
{
	delete this;
}

void CrossServerModule::StopServer()
{
	Game *game = (Game *)this->Interface();
	game->Stop();
}

int CrossServerModule::Init()
{
	srand(PITime());

	DEPEND_ON_INIT(LOG_MODULE)

	ILogModule *log = dynamic_cast<ILogModule *>(Interface()->QueryModule(LOG_MODULE));

	m_log.SetLogModule(log);
	m_log.SetCatagory(CROSSSERVER_MODULE);

	return IModule::Succeed;
}

int CrossServerModule::Start()
{
	DEPEND_ON_START(CLOCK_MODULE)
	DEPEND_ON_START(NETWORK_MODULE)
	DEPEND_ON_START(INTERNAL_NETWORK)
	DEPEND_ON_START(GAME_MODULE)
	DEPEND_ON_START(CONFIG_MODULE)
	DEPEND_ON_START(LOG_MODULE)
	DEPEND_ON_START(RMI_MODULE)
	DEPEND_ON_START(TIMER_MODULE)

	m_clock = dynamic_cast<IClockModule*>(Interface()->QueryModule(CLOCK_MODULE));
	m_internal_network = dynamic_cast<INetworkModule*>(Interface()->QueryModule(INTERNAL_NETWORK));
	m_game = dynamic_cast<IGameModule*>(Interface()->QueryModule(GAME_MODULE));
	m_timer = dynamic_cast<ITimerModule*>(Interface()->QueryModule(TIMER_MODULE));
	IRMIModule *rmi_module = dynamic_cast<IRMIModule*>(Interface()->QueryModule(RMI_MODULE));
	ILogModule *log = dynamic_cast<ILogModule*>(Interface()->QueryModule(LOG_MODULE));
	IConfigModule *config = dynamic_cast<IConfigModule *>(Interface()->QueryModule(CONFIG_MODULE));

	EngineAdapter::Instance().Init(log, config, m_game, m_clock, rmi_module, m_timer);

	// 获取配置目录
	std::string config_dir = "../config";
	config->SyncValue(ROOT/"ConfigDir", &config_dir, config_dir);
	if (config_dir[config_dir.length() - 1] != '/')
	{
		config_dir += '/';
	}

	// 初始化全局配置
	char err_info[1024]={0};
	if(!ServerConfigPool::Instance().Init(config_dir, err_info))
	{
		m_log.printf(LL_ERROR, err_info);
		m_log.printf(LL_ERROR, "Cross Server On Start, ServerConfigPool::Init return FALSE.");

		this->StopServer();

		return IModule::Fail;
	}
	m_log.printf(LL_INFO, "Cross Server On Start, ServerConfigPool::Init Success.");

	// 加载SharedConfig
	{
		std::string error;
		if (!SHAREDCONFIG->Init(config_dir, &error))
		{
			m_log.printf(LL_MAINTANCE, "SharedConfig::Init Return Fail! Error At:%s", error.c_str());

			this->StopServer();

			return IModule::Fail;
		}
		m_log.printf(LL_INFO, "Cross Server On Start, SharedConfig::Init Success.");
	}


	{
		// 加载local config
		std::string error;
		if (!LocalConfig::Instance().Init(config_dir + "local.xml", &error))
		{
			m_log.printf(LL_ERROR, error.c_str());
			m_log.printf(LL_ERROR, "Cross Server On Start, LocalConfig::Init return FALSE.");

			this->StopServer();

			return IModule::Fail;
		}
		m_log.printf(LL_INFO, "Cross Server On Start, LocalConfig::Init Success.");
	}

	// 初始化字符串资源
	std::string error;
	if (!gamestring::StringInit((config_dir + "/serverconfig/string.xml").c_str(), &error))
	{
		m_log.printf(LL_MAINTANCE, "gamestring::StringInit Return Fail! Error At:%s", error.c_str());

		config->write();

		this->StopServer();

		return IModule::Fail;
	}

	m_internal_network->RegisterCallback(m_internal_network_callback);
	InternalComm::Instance().Init(m_internal_network);

	// 加载crossserver配置
	config_dir += "crossserver/";
	if (!this->InitCrossServerConfig(config_dir))
	{
		config->write();

		this->StopServer();

		return IModule::Fail;
	}

	// 连接到数据库
	if (!this->ConnectToDB())
	{
		config->write();

		this->StopServer();

		return IModule::Fail;
	}
	
	m_cross_server.Init(this, log);

	std::string log_dir = "log/";
	EngineAdapter::Instance().ConfigSyncValue(ROOT/"LogDir", &log_dir, log_dir);
	if (log_dir[log_dir.length() - 1] != '/')
	{
		log_dir += '/';
	}
	crosslog::LogInit(log, log_dir);

	m_cross_server.Start();

	config->write();

	m_log.printf(LL_MAINTANCE, "CrossServerModule started");

	SysPath::CreateFileByPath("status/cross.start");
	
	return IModule::Succeed;
}

int CrossServerModule::Update()
{
	if (m_clock->GetCurrentFrame() != m_current_frame)
	{	
		m_current_frame = m_clock->GetCurrentFrame();
		m_cross_server.Update();
	}

	return IModule::Pending;
}

int CrossServerModule::Stop()
{
	SysPath::CreateFileByPath("status/cross.end");

	return IModule::Succeed;
}

int CrossServerModule::Release()
{
	return IModule::Succeed;
}

bool CrossServerModule::ConnectToDB()
{
	std::string cross_db_server_ip = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBCross.module_addr.ip;
	Port cross_db_server_port = ServerConfigPool::Instance().common_config.ipconfigs.db_server_addr.DBCross.module_addr.port;

	rmi::Session cross_session = EngineAdapter::Instance().RMICreateSession(cross_db_server_ip.c_str(), cross_db_server_port);
	if (cross_session.handle == 0)
	{
		m_log.printf(LL_MAINTANCE, "Connect Cross DBLayer[%s:%d] Fail!", cross_db_server_ip.c_str(), (int)cross_db_server_port);
		return false;
	}
	m_log.printf(LL_MAINTANCE, "Connect Cross DBLayer[%s:%d] Suc!", cross_db_server_ip.c_str(), (int)cross_db_server_port);
	
	InternalComm::Instance().SetDBSession(rmi::Session(), NULL, 0, rmi::Session(), rmi::Session(), rmi::Session(), cross_session);

	m_log.printf(LL_MAINTANCE, "Connect to DataAccessServer suc.");
	
	return true;
}

bool CrossServerModule::ListenForGlobal()
{
	unsigned short listen_for_global_port = 10777;
	int backlog = 64;
	EngineAdapter::Instance().ConfigSyncValue(ROOT/CROSSSERVER_MODULE/"ForGlobal"/"ListenPort", &listen_for_global_port, listen_for_global_port);
	EngineAdapter::Instance().ConfigSyncValue(ROOT/CROSSSERVER_MODULE/"ForGlobal"/"Backlog", &backlog, backlog);

	bool ret = m_internal_network->Listen(listen_for_global_port, backlog);
	if (!ret)
	{
		m_log.print(LL_MAINTANCE, "NetListen ForGlobal return FAIL!");
		return false;
	}

	m_log.printf(LL_MAINTANCE, "Server ForGlobal Listen[%d] suc.", (int)listen_for_global_port);

	return true;
}

bool CrossServerModule::InitCrossServerConfig(const std::string &config_dir)
{
	std::string err;
	if (!CrossServerConfig::Instance().Init(config_dir, &err))
	{
		m_log.printf(LL_ERROR, err.c_str());
		m_log.printf(LL_ERROR, "CrossServer On INIT, CrossServerConfig::Init return FALSE.");
		return false;
	}

	m_log.printf(LL_INFO, "CrossServer On INIT, CrossServerConfig::Init Success.");

	return true;
}

