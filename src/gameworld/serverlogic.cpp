#include "serverlogic.h"
#include "common/platform/socket/pisocket.h"

#include "iclockmodule.h"
#include "inetworkmodule.h"
#include "ilogmodule.h"
#include "igamemodule.h"
#include "iconfigmodule.h"
#include "irmimodule.h"
#include "itimermodule.h"

#include "servercommon/servercommon.h"
#include "gameworld/world.h"
#include "common/platform/system.h"
#include "common/syspath.h"
#include "utility/configpath.h"
#include "gameserver/game.h"
#include "internalcomm.h"
#include "hightimermgr.h"
#include "global/cross/crossmanager.h"
#include "servercommon/platfromhelper.hpp"

class ServerNetworkCallback : public IEngineNetCallback
{
public:
	ServerNetworkCallback(ServerLogic *server) : m_server(server), m_world(0) {}
	virtual ~ServerNetworkCallback() {}

	void SetWorld(World *world) { m_world = world; }
	void SetAllowIpPrefix(const std::string &allow_ip_prefix) { m_allow_ip_prefix = allow_ip_prefix; }

	virtual void OnAccept(Port listen_port, NetID netid, IP ip, Port port)
	{
		sockaddr_in host_adr;
		memset(&host_adr, 0, sizeof(sockaddr_in));
		host_adr.sin_family = AF_INET;
		host_adr.sin_addr.s_addr = htonl(ip);
		std::string ip_str = inet_ntoa(host_adr.sin_addr);
		std::string ip_record = ip_str;
		ip_str.resize(m_allow_ip_prefix.size());
		if (ip_str != m_allow_ip_prefix)
		{
			m_server->m_log.printf(LL_WARNING, "Network Accept NOT allow ip[%s].", ip_record.c_str());
			m_server->m_network->Disconnect(netid);
			return;
		}

		m_world->OnAccept(listen_port, netid, ip, port);
	}

	virtual void OnRecv(NetID netid, const char *data, int length)
	{
		m_world->OnRecv(netid, data, length);
	}

	virtual void OnDisconnect(NetID netid)
	{
		m_world->OnDisconnect(netid);
	}

	virtual void OnConnect(bool result, int handle, NetID netid, IP ip, Port port)
	{
	}

private:
	ServerLogic *m_server;
	World *m_world;
	std::string m_allow_ip_prefix;
};

class ServerInternalNetCallback : public IEngineNetCallback
{
public:
	ServerInternalNetCallback(ServerLogic *server) : m_server(server), m_world(0) {}
	virtual ~ServerInternalNetCallback() {}

	void SetWorld(World *world) { m_world = world; }
	void SetAllowIpPrefix(const std::string &allow_ip_prefix) { m_allow_ip_prefix = allow_ip_prefix; }

	virtual void OnAccept(Port listen_port, NetID netid, IP ip, Port port)
	{
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

		m_world->OnInternalAccept(listen_port, netid, ip, port);
	}

	virtual void OnRecv(NetID netid, const char *data, int length)
	{
		m_world->OnInternalRecv(netid, data, length);
	}

	virtual void OnDisconnect(NetID netid)
	{
		m_world->OnInternalDisconnect(netid);
	}

	virtual void OnConnect(bool result, int handle, NetID netid, IP ip, Port port)
	{
		if (handle == InternalComm::Instance().m_reconnet_login_handle)
		{
			InternalComm::Instance().m_reconnet_login_handle = -1;

			if (!result)
			{
				m_world->ReConnectToLogin();
			}
			else
			{
				InternalComm::Instance().m_login_server_id = netid;
				m_world->RegisterToLogin();
			}
		}
		else if (CrossManager::Instance().IsCrossHandle(handle))
		{
			if (!result)
			{
				CrossManager::Instance().OnDisconnectFromCrossServerHandle(handle); // 连接不成功只能利用回调标志handle
			}
			else
			{
				CrossManager::Instance().OnConnectCrossServerSucc(handle, netid); // 连接成功，netid才有效
			}

			CrossManager::Instance().CheckCrossConnection();
		}
	}

private:
	ServerLogic *m_server;
	World *m_world;
	std::string m_allow_ip_prefix;
};

static ServerLogic *server_logic = NULL;

ServerLogic::ServerLogic(bool check_res_mode, int* check_ret, int total_server, int server_index)
	: m_check_res_mode(check_res_mode), m_check_ret(check_ret), m_total_server(total_server), m_server_index(server_index),
	m_clock(NULL), m_game(NULL), m_network(NULL), m_internal_network(NULL), m_rmi(NULL), m_timer(NULL),
	m_current_frame(-1), m_frame_start_time(0), m_frame_elapse_time(0), m_pi_time(0), m_system_time_ms(0),
	m_network_callback(NULL), m_internal_network_callback(NULL), m_world(NULL)
{
	m_network_callback = new ServerNetworkCallback(this);
	m_internal_network_callback = new ServerInternalNetCallback(this);

	server_logic = this;
}

ServerLogic::~ServerLogic()
{
	delete m_network_callback;
	delete m_internal_network_callback;
}

ServerLogic * ServerLogic::GetInstServerLogic()
{
	return server_logic;
}

void ServerLogic::Free()
{
	delete this;
}

void ServerLogic::StopServer()
{
	Game *game = (Game*)this->Interface();
	game->Stop();
}

int ServerLogic::Init()
{
	srand(PITime());

	EXPECT_ON_INIT(LOG_MODULE)

	m_log.SetLogModule(dynamic_cast<ILogModule*>(Interface()->QueryModule(LOG_MODULE)));
	m_log.SetCatagory(SERVER_MODULE);

	return IModule::Succeed;
}

int ServerLogic::Start()
{
	DEPEND_ON_START(CLOCK_MODULE)
	DEPEND_ON_START(NETWORK_MODULE)
	DEPEND_ON_START(GAME_MODULE)
	DEPEND_ON_START(CONFIG_MODULE)
	DEPEND_ON_START(INTERNAL_NETWORK)
	DEPEND_ON_START(RMI_MODULE)
	DEPEND_ON_START(TIMER_MODULE)
	DEPEND_ON_START(FILE_WRITE_MODULE)
	
	ILogModule *log = dynamic_cast<ILogModule *>(Interface()->QueryModule(LOG_MODULE));
	IConfigModule *config = dynamic_cast<IConfigModule *>(Interface()->QueryModule(CONFIG_MODULE));
	m_clock = dynamic_cast<IClockModule*>(Interface()->QueryModule(CLOCK_MODULE));
	m_network = dynamic_cast<INetworkModule*>(Interface()->QueryModule(NETWORK_MODULE));
	m_internal_network = dynamic_cast<INetworkModule*>(Interface()->QueryModule(INTERNAL_NETWORK));
	m_game = dynamic_cast<IGameModule*>(Interface()->QueryModule(GAME_MODULE));
	m_rmi = dynamic_cast<IRMIModule*>(Interface()->QueryModule(RMI_MODULE));
	m_timer = dynamic_cast<ITimerModule*>(Interface()->QueryModule(TIMER_MODULE));
	IFileWriteModule *filewrite = dynamic_cast<IFileWriteModule *>(Interface()->QueryModule(FILE_WRITE_MODULE));

	m_world = new World(log, config, m_game, m_network, m_internal_network, m_clock, m_rmi, m_timer, filewrite, m_total_server, m_server_index);

	m_network_callback->SetWorld(m_world);
	m_internal_network_callback->SetWorld(m_world);

	std::string allow_ip_prefix = "127.0.0.1";
	config->SyncValue(ROOT/"World"/"AllowIPPrefix", &allow_ip_prefix, allow_ip_prefix);
	m_network_callback->SetAllowIpPrefix(allow_ip_prefix);
	m_internal_network_callback->SetAllowIpPrefix(allow_ip_prefix);

	m_network->RegisterCallback(m_network_callback);
	m_internal_network->RegisterCallback(m_internal_network_callback);
	
	if (!m_world->Init())
	{
		config->write();

		this->StopServer();
		
		return IModule::Fail;
	}

	m_log.printf(LL_MAINTANCE, "Server Init.");

	bool succ = m_world->Start(m_check_res_mode, m_total_server, m_server_index);
	
	if (m_check_res_mode) 
	{
		this->StopServer();

		if (m_check_ret)
		{
			*m_check_ret = succ ? 0 : 1;
		}

		return IModule::Succeed;
	}
	
	if (!succ)
	{
		config->write();
		
		this->StopServer();

		return IModule::Fail;
	}

	m_log.printf(LL_MAINTANCE, "Server started");

	config->write();

	SysPath::CreateFileByPath("status/game.start");

	return IModule::Succeed;
}

int ServerLogic::Update()
{
	m_pi_time = PITime();

	struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned long long tv_sec = static_cast<unsigned long long>(tv.tv_sec);
	unsigned long long tv_usec = static_cast<unsigned long long>(tv.tv_usec);
	m_system_time_ms = tv_sec * 1000 + tv_usec / 1000;

	if (m_clock->GetCurrentFrame() != m_current_frame)
	{
		m_frame_start_time = m_clock->GetFrameTime();
		m_current_frame = m_clock->GetCurrentFrame();
		m_frame_elapse_time = 0;

		m_world->Update();
	}
	else
	{
		m_frame_elapse_time = static_cast<int>(m_pi_time - m_frame_start_time);
	}

	HighTimerMgr::Instance().Update(m_pi_time);
 
	return IModule::Pending;
}

int ServerLogic::Stop()
{
	m_world->Stop(m_check_res_mode);
	m_log.printf(LL_MAINTANCE, "Server stopped");

	m_world->Release();
	m_log.printf(LL_MAINTANCE, "Server released");

	delete m_world;
	m_world = 0;

	SysPath::CreateFileByPath("status/game.end");

	return IModule::Succeed;
}

int ServerLogic::Release()
{
	return IModule::Succeed;
}

