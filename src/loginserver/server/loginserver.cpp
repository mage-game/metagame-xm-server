#include "protocal/msglogin.h"
#include "loginserver.h"
#include "servercommon/userprotocal/loginmsgcode.h"
#include "servercommon/userprotocal/msgheader.h"
#include "ilogmodule.h"

#include "engineadapter.h"
#include "internalcomm.h"
#include "servercommon/servercommon.h"
#include "roleinitattr.h"
#include "iplimit.hpp"
#include "protocal/msglogin.h"
#include "protocal/msgregister.h"
#include "servercommon/internalprotocal/lgprotocal.h"
#include "servercommon/internalprotocal/lwprotocal.h"
#include "servercommon/internalprotocal/lglobalprotocal.h"
#include "servercommon/darmi/rmisystemclient.h"
#include "servercommon/serverconfig/serverconfigpool.h"

#include "rmibackobjdef.h"
#include "loginlog.h"

#include "platform/platform.h"
#include "utility/configpath.h"
#include "loginservermodule.h"

#include "loginusertrace.hpp"
#include "mergeloginmanager.h"
#include "config/loginconfig.h"

OLD_USE_INTERNAL_NETWORK();

LoginServer::LoginServer() 
	: m_min_login_interval_s(5), m_platform(NULL), last_check_prof_num_timestamp(0), m_prof1_num(0), 
	m_prof2_num(0), m_prof3_num(0), m_prof4_num(0), m_is_register_limit(false), m_is_auto_set_register_limit(false)
{
}

LoginServer::~LoginServer()
{

}

LoginServer & LoginServer::Instance()
{
	static LoginServer ls;
	return ls;
}

bool LoginServer::Init(const std::vector<int> &db_index_to_create, ILogModule *log, const std::string &configpath, 
					   int min_login_interval_s, int can_not_login_global_time)
{
	m_log.SetLogModule(log);
	m_log.SetCatagory("LoginServer");

	// 获取日志生成目录
	std::string log_dir = "log/";
	EngineAdapter::Instance().ConfigSyncValue(ROOT/"LogDir", &log_dir, log_dir);
	if (log_dir[log_dir.length() - 1] != '/')
	{
		log_dir += '/';
	}
	loginlog::LogInit(log, log_dir);

	// 加载注册配置
	std::string err;
	std::string configname = "registeconfig.xml";

	if (!RoleInitAttr::Instance().Init(configpath, configname, &err))
	{
		m_log.printf(LL_ERROR, err.c_str());
		m_log.printf(LL_ERROR, "Login Server On INIT, RoleInitAttr::Init return FALSE.");
		return false;
	}
	m_log.printf(LL_INFO, "Login Server On INIT, RoleInitAttr::Init Success.");
	
	configname = "iplimit.xml";

	if (!IPLimit::Instance().Init(configpath, configname, &err))
	{
		m_log.printf(LL_ERROR, err.c_str());
		m_log.printf(LL_ERROR, "Login Server On INIT, IPLimit::Init return FALSE.");
		return false;
	}

	configname = "loginusertraceconfig.xml";

	if (!LoginUserTrace::Instance().Init(configpath, configname, &err))
	{
		m_log.printf(LL_ERROR, err.c_str());
		m_log.printf(LL_ERROR, "Login Server On INIT, LoginUserTrace::Init return FALSE.");
		return false;
	}

	m_min_login_interval_s = min_login_interval_s;

	this->StartCmd();
	this->StartWorldStatus();

	m_platform = Platform::Create();

	return true;
}

void RMIGetProfBackObjectImpl::GetProfNumRet(int ret, int prof1_num, int prof2_num, int prof3_num, int prof4_num)
{
	if (0 == ret)
	{
		LoginServer::Instance().SetProfNum(prof1_num, prof2_num, prof3_num, prof4_num);
	}
}

void LoginServer::Update()
{
	time_t now = EngineAdapter::Instance().Time();

	if (now >= last_check_prof_num_timestamp + 10)
	{
		last_check_prof_num_timestamp = now;

		if (!m_is_auto_set_register_limit && LoginConfig::Instance()->IsForbidRegister())
		{
			m_is_auto_set_register_limit = true;
			this->SetIsRegisterLimit(true);
		}

		// 获得各个职业的人数
		RMIGetProfBackObjectImpl *ulboi = new RMIGetProfBackObjectImpl();

		RMILoginClient lc;
		lc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());
		lc.GetProfNum(ulboi);
	}
}

void LoginServer::OnGSAccept(Port listen_port, NetID netid, IP ip, Port port)	// 接入gamesvr
{
	if (m_gs_list.size() <= netid)
	{
		int old_size = (int)m_gs_list.size();
		m_gs_list.resize(netid + 1);
		for (int i = old_size; i < (int)m_gs_list.size(); ++i)
		{
			m_gs_list[i] = 0;
		}
	}

	GameServer *gs = new GameServer();

	gs->netid = netid;
 	gs->port = 0;
	m_gs_list[netid] = gs;
}

void LoginServer::OnGateAccept(Port listen_port, NetID netid, IP ip, Port port)		// 接入gateway
{
	if (m_gateway_list.size() <= netid)
	{
		int old_size = (int)m_gateway_list.size();
		m_gateway_list.resize(netid + 1);
		for (int i = old_size; i < (int)m_gateway_list.size(); ++i)
		{
			m_gateway_list[i] = NULL;
		}
	}

	GateWayServer *gw = new GateWayServer();

	gw->netid = netid;
	gw->port = 0;
	m_gateway_list[netid] = gw;
}

void LoginServer::OnInnerRecv(NetID netid, const char *data, int length)
{
	if (m_gs_list.size() > netid && m_gs_list[netid] != 0)
	{
		OnGSRecv(m_gs_list[netid], data, length);
		return;
	}
	
	if (m_gateway_list.size() > netid && m_gateway_list[netid] != 0)
	{
		OnGateRecv(netid, data, length);
		return;
	}
}

void LoginServer::OnGSRecv(GameServer *gs, const char *data, int length)		// 处理gamesvr的消息
{
	lgprotocal::MessageHeader *header = (lgprotocal::MessageHeader*)data;

	switch (header->msg_type)
	{
	case lgprotocal::MT_REGISTER_GS:											// gamesvr注册后 相互获取之间的场景信息 用于切换场景
		OnRegisterGS(gs, data);
		break;

	case lgprotocal::MT_ROLE_FORBID:											// gamesvr请求角色封号
		{
			lgprotocal::RoleForbid *rf = (lgprotocal::RoleForbid *)data;
			OnRoleForbid(rf);
		}
		break;

	default:
		break;
	}
}

void LoginServer::OnGateRecv(NetID netid, const char *data, int length)			// 处理gateway的消息
{
	lwprotocal::MessageHeader *header = (lwprotocal::MessageHeader*)data;

	switch (header->msg_type)
	{
	case lwprotocal::MT_REGISTER_GW:
		OnRegisterGW(netid, data);
		break;

	default:
		break;
	}
}

void LoginServer::OnRegisterGW(NetID netid, const char *data)					// 有新的gateway注册进来 让gateway连接上所有的gamesvr
{
	lwprotocal::WLRegisterGW *rgw = (lwprotocal::WLRegisterGW*)data;

	GateWayServer *gw = m_gateway_list[netid];

	gw->hostname = rgw->hostname;
	gw->port = rgw->port;
	gw->netid = netid;

	AddGateWay(gw);

	int gs_size = (int)m_gs_list.size();
	for (int g = 0; g < gs_size; ++g)
	{
		GameServer *gs = m_gs_list[g];
		if (NULL != gs && 0 != gs->port)
		{
			lwprotocal::LWOnGSRegister ogsr;
			ogsr.index = gs->index;
			STRNCPY(ogsr.ip, gs->ip.c_str(), sizeof(ogsr.ip));
			ogsr.port = gs->port;
			InternalComm::Instance().NetSend(netid, (const char *)&ogsr, sizeof(lwprotocal::LWOnGSRegister));
		}
	}
}

void LoginServer::OnInnerDisconnect(NetID netid)
{
	if (m_gs_list.size() > netid && m_gs_list[netid] != 0)
	{
		OnGSDisconnect(m_gs_list[netid]);
		return;
	}

	if (m_gateway_list.size() > netid && m_gateway_list[netid] != 0)
	{
		OnGateDisconnect(netid);
		return;
	}
}

void LoginServer::OnGSDisconnect(GameServer *gs)					// gamesvr断开 通知其他gamesvr场景缺失
{
	lgprotocal::LGUnRegisterGS urgs;

	urgs.scene_count = 0;

	int gs_scene_size = (int)gs->scene_list.size();

	for (int s = 0; s < gs_scene_size; ++s)
	{
		RemoveGSServer(gs->scene_list[s]);
		if (urgs.scene_count != MAX_SCENE_LIST_NUM)
		{
			urgs.scene_list[urgs.scene_count++] = gs->scene_list[s];
		}
	}

	NetID netid = gs->netid;
	delete gs;
	m_gs_list[netid] = 0;

	int gs_size = (int)m_gs_list.size();
	for (int g = 0; g < gs_size; ++g)
	{
		GameServer *gameserver = m_gs_list[g];
		if (NULL != gameserver)
		{
			InternalComm::Instance().NetSend(gameserver->netid, (const char *)&urgs, sizeof(lgprotocal::LGUnRegisterGS));
		}
	}
}

void LoginServer::OnGateDisconnect(NetID netid)				// gateway断开 清理m_gameway_use_list的多余数据 避免被随机到没意义的
{
	GateWayServer *gw = m_gateway_list[netid];

	RemoveGateWay(gw);

	delete gw;
	m_gateway_list[netid] = 0;
}

void LoginServer::OnRegisterGS(GameServer *gs, const char *data)
{
	lgprotocal::GLRegisterGS *rgs = (lgprotocal::GLRegisterGS *)data;

	gs->index = rgs->index;
	gs->ip = rgs->ip;
	gs->port = rgs->port;

	for (int i = 0; i < rgs->scene_count && i < MAX_SCENE_LIST_NUM; ++i)
	{
		gs->scene_list.push_back(rgs->scene_list[i]);
		AddGSServer(rgs->scene_list[i], gs);
	}

	lgprotocal::LGRegisterGS other_rgs;
	other_rgs.index = rgs->index;
	other_rgs.scene_count = rgs->scene_count;
	memcpy(other_rgs.scene_list, rgs->scene_list, sizeof(rgs->scene_list));

	int gs_size = (int)m_gs_list.size();
	for (int g = 0; g < gs_size; ++g)
	{
		GameServer *gameserver = m_gs_list[g];
		if (NULL != gameserver && 0 != gameserver->port)
		{
			// 通知其他场景服务器 新进来场景服务携带的所有场景信息 
			InternalComm::Instance().NetSend(gameserver->netid, (const char *)&other_rgs, sizeof(lgprotocal::LGRegisterGS));

			if (gameserver->index != gs->index)
			{
				lgprotocal::LGRegisterGS my_rgs;
				my_rgs.scene_count = 0;
				my_rgs.index = gameserver->index;

				for (std::vector<int>::iterator s = gameserver->scene_list.begin(), s_end = gameserver->scene_list.end();
					my_rgs.scene_count < MAX_SCENE_LIST_NUM && s != s_end; ++s)
				{
					my_rgs.scene_list[my_rgs.scene_count++] = *s;
				}

				// 通知新进来场景服务器 其他场景服务器场景信息 
				InternalComm::Instance().NetSend(gs->netid, (const char *)&my_rgs, sizeof(lgprotocal::LGRegisterGS));
			}
		}
	}

	// 通知所有的gateway有新gamesvr进来

	lwprotocal::LWOnGSRegister ogsr;
	ogsr.index = rgs->index;
	F_STRNCPY(ogsr.ip, rgs->ip, sizeof(ogsr.ip));
	ogsr.port = rgs->port;

	for (int w = 0; w < (int)m_gateway_list.size(); ++w)
	{
		GateWayServer *gw = m_gateway_list[w];
		if (NULL != gw && 0 != gw->port)
		{
			InternalComm::Instance().NetSend(gw->netid, (const char *)&ogsr, sizeof(lwprotocal::LWOnGSRegister));
		}
	}
}

void LoginServer::OnAccept(Port listen_port, NetID netid, IP ip, Port port)
{
	m_log.printf(LL_DEBUG, "LoginServer::OnAccept.");
}

void LoginServer::OnRecv(IP ip, NetID netid, const char *data, int length)
{
	if (length < (int)sizeof(Protocol::MessageHeader))
	{
		EngineAdapter::Instance().NetDisconnect(netid);
		return;
	}

	Protocol::MessageHeader *header = (Protocol::MessageHeader *)data;

	switch (header->msg_type)
	{
	case Protocol::MT_NEW_LOGIN_REQ_CS:
		if (length == sizeof(Protocol::CSLogin))
		{
			OnLoginReq(netid, data);
		}
		break;

	case Protocol::MT_NEW_ROLE_REQ_CS:
		if (length == sizeof(Protocol::CSRoleReq))
		{
			OnRoleReq(ip, netid, data);
		}
		break;

	case Protocol::MT_CREATE_ROLE_REQ_CS:
		if (length == sizeof(Protocol::CSCreateRole))
		{
			OnCreateRole(netid, data);
		}
		break;

	case Protocol::MT_L_HEARTBEAT_CS:
		{
			Protocol::SCLHeartBeat hb;
			EngineAdapter::Instance().NetSend(netid, (const char *)&hb, sizeof(hb));
		}
		break;

	default:
		m_log.printf(LL_DEBUG, "OnRecv Unknow msg type:%d.", header->msg_type);
		//EngineAdapter::Instance().NetDisconnect(netid);
		break;
	}
}

void LoginServer::OnDisconnect(NetID netid)
{
	m_log.printf(LL_DEBUG, "LoginServer::OnDisconnect.");
}

GameServer * LoginServer::GetGSServer(int scene_id)
{
	GSSceneListIt it = m_scene_list.find(scene_id);

	if (it != m_scene_list.end())
	{
		return it->second;
	}
	
	return NULL;
}

void LoginServer::BroadcastToAllGSServer(const char *data, int sendlen)
{
	int gs_size = (int)m_gs_list.size();
	for (int g = 0; g < gs_size; ++g)
	{
		GameServer *gameserver = m_gs_list[g];
		if (NULL != gameserver && 0 != gameserver->port)
		{
			InternalComm::Instance().NetSend(gameserver->netid, data, sendlen);
		}
	}
}

void LoginServer::AddGSServer(int scene_id, GameServer *gs)
{
	GSSceneListIt it = m_scene_list.find(scene_id);
	if (it != m_scene_list.end()) return;
	m_scene_list[scene_id] = gs;
}

void LoginServer::RemoveGSServer(int scene_id)
{
	GSSceneListIt it = m_scene_list.find(scene_id);
	if (it == m_scene_list.end()) return;
	m_scene_list.erase(it);
}

LoginServer::GateWayServer * LoginServer::RandomGatewayServer()
{
	int gw_size = (int)m_gameway_use_list.size();
	if (0 == gw_size) return NULL;

	int r = RandomNum(gw_size);
	return m_gameway_use_list[r];
}

void LoginServer::AddGateWay(GateWayServer *gw)
{
	m_gameway_use_list.push_back(gw);
}

void LoginServer::RemoveGateWay(GateWayServer *gw)
{
	for (GateWayListIt it = m_gameway_use_list.begin(); it != m_gameway_use_list.end(); ++it)
	{
		if ((*it)->netid == gw->netid)
		{
			m_gameway_use_list.erase(it);
			break;
		}
	}
}

class LoginCmdTimer : public ITimerCallback
{
public:
	LoginServer *login_server;

	virtual void OnTimer()
	{
		login_server->StartCmd();
	}

	virtual void Free() { delete this; }
};

void RMILoginGetCmdBackObjectImpl::GetCmdRet(int ret, const CmdList &cmd_list)
{
	if (ret != 0) return;

	for (int i = 0; i < cmd_list.count && i < CmdList::MAX_CMD_NUM; ++i)
	{
		login_server->OnCmd(cmd_list.cmd_list[i].cmd_id, cmd_list.cmd_list[i].creator, cmd_list.cmd_list[i].cmd);
	}
}

void LoginServer::StartCmd()
{
	RMILoginGetCmdBackObjectImpl *gpnbo = new RMILoginGetCmdBackObjectImpl();
	gpnbo->login_server = this;

	RMISystemClient sc;
	sc.__bind_session(InternalComm::Instance().GetDBHandler().GetSystemDB());
	sc.GetCmd(RMISystemClient::CMD_TYPE_LOGIN, gpnbo);

	static const unsigned int CMD_CHECK_INTERVAL = ServerConfigPool::Instance().common_config.system_cmd_check_interval_s;

	LoginCmdTimer *timer = new LoginCmdTimer();
	timer->login_server = this;
	EngineAdapter::Instance().CreateTimerSecond(CMD_CHECK_INTERVAL, timer);
}

void RMIForbidBackObjectImpl::ForbidRet(int result, int db_index, int role_1, int role_2, int role_3)
{
	loginlog::g_log_cmd.printf(LL_INFO, "ForbidRet %s [%d][%s] %s", (result == 0 ? "Suc" : "Fail"), cmd_id, creator.c_str(), cmd.c_str());
	
	if (0 == result)
	{
		lgprotocal::LGKickRole lkr;
		lkr.role_id = role_1;
		LoginServer::Instance().BroadcastToAllGSServer((const char*)&lkr, sizeof(lkr));
	}
}

void RMIForbidOneRoleBackObjectImpl::ForbidOneRoleRet(int result, int db_index)
{
	loginlog::g_log_cmd.printf(LL_INFO, "ForbidOneRoleRet %s [%d][%s] %s", (result == 0 ? "Suc" : "Fail"), cmd_id, creator.c_str(), cmd.c_str());

	if (0 == result)
	{
		lgprotocal::LGKickRole lkr;
		lkr.role_id = role_id;
		LoginServer::Instance().BroadcastToAllGSServer((const char*)&lkr, sizeof(lkr));
	}
}

void LoginServer::OnCmd(int cmd_id, const std::string &creator, const std::string &cmd)
{
	std::istringstream iss;
	iss.str(cmd);

	std::string c;
	if ((iss >> c).fail()) return;

	// 命令非常简单，先简单这么写，以后命令多了再改

	if (c == "Forbid")	// 封号
	{
		std::string pname_tmp;
		unsigned int forbid_time;
		if (!(iss >> pname_tmp >> forbid_time).fail() && pname_tmp.size() < sizeof(PlatName))
		{
			PlatName pname;
			STRNCPY(pname, pname_tmp.c_str(), sizeof(pname));

			RMIForbidBackObjectImpl *fboi = new RMIForbidBackObjectImpl();
			fboi->login_server = this;
			F_STRNCPY(fboi->pname, pname, sizeof(fboi->pname));
			fboi->cmd_id = cmd_id;
			fboi->creator = creator;
			fboi->cmd = cmd;

			RMILoginClient lc;
			lc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());
			bool ret = lc.Frobid(pname, forbid_time, fboi);
			if (!ret)
			{
				loginlog::g_log_cmd.printf(LL_WARNING, "RMICallFail [%d][%s] %s", cmd_id, creator.c_str(), cmd.c_str());
			}

			return ;
		}
	}
	else if (c == "ForbidOneRole")
	{
		std::string pname_tmp;
		unsigned int forbid_time = 0;
		int role_id = 0;
		if (!(iss >> pname_tmp >> role_id >> forbid_time).fail() && pname_tmp.size() < sizeof(PlatName))
		{
			PlatName pname;
			STRNCPY(pname, pname_tmp.c_str(), sizeof(pname));

			RMIForbidOneRoleBackObjectImpl *fboi = new RMIForbidOneRoleBackObjectImpl();
			fboi->login_server = this;
			F_STRNCPY(fboi->pname, pname, sizeof(fboi->pname));
			fboi->role_id = role_id;
			fboi->cmd_id = cmd_id;
			fboi->creator = creator;
			fboi->cmd = cmd;

			RMILoginClient lc;
			lc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());
			bool ret = lc.FrobidOneRole(pname, role_id, forbid_time, fboi);
			if (!ret)
			{
				loginlog::g_log_cmd.printf(LL_WARNING, "RMICallFail [%d][%s] %s", cmd_id, creator.c_str(), cmd.c_str());
			}

			return;
		}
	}
	else if ("AddLoginUserTrace" == c)
	{
		std::string pname_tmp;
		if (!(iss >> pname_tmp).fail() && pname_tmp.size() < sizeof(PlatName))
		{
			PlatName pname;
			STRNCPY(pname, pname_tmp.c_str(), sizeof(pname));

			LoginUserTrace::Instance().AddPName(pname);
			loginlog::g_log_cmd.printf(LL_INFO, "Suc [%d][%s] %s", cmd_id, creator.c_str(), cmd.c_str());
			return;
		}
	}
	else if ("ClearLoginUserTrace" == c)
	{
		LoginUserTrace::Instance().ClearPNameList();
		loginlog::g_log_cmd.printf(LL_INFO, "Suc [%d][%s] %s", cmd_id, creator.c_str(), cmd.c_str());
		return;
	}
	else if ("SetLoginUserTraceFlag" == c)
	{
		int open_flag = 0;
		if (!(iss >> open_flag).fail())
		{
			LoginUserTrace::Instance().SetOpenFlag(0 != open_flag);
			loginlog::g_log_cmd.printf(LL_INFO, "Suc [%d][%s] %s", cmd_id, creator.c_str(), cmd.c_str());
			return;
		}
	}
	else if ("ClearLoginDbIndexCache" == c)
	{
		std::string pname_tmp;
		if (!(iss >> pname_tmp).fail() && pname_tmp.size() < sizeof(PlatName))
		{
			PlatName pname;
			STRNCPY(pname, pname_tmp.c_str(), sizeof(pname));

			MergeLoginManager::Instance().ClearDbIndex(pname);
			return;
		}
	}
	else if ("ClearAllLoginDbIndexCache" == c)
	{
		MergeLoginManager::Instance().ClearAllDbIndex();
		return;
	}
	else if ("RegisterLimit" == c)
	{
		int is_register_limit = 0;
		if (!(iss >> is_register_limit).fail())
		{
			if (m_is_register_limit != (0 != is_register_limit))
			{
				m_is_register_limit = (0 != is_register_limit);
				this->SaveWorldStatus();
			}
			return;
		}
	}
	else if ("ReloadLoginConfig" == c) // login进程 - 热更配置
	{
		std::string tmp_error;
		if (!LOGIN_CONFIG->Reload(&tmp_error))
		{
			m_log.printf(LL_ERROR, "LoginConfig::OnCmd, Reload Login Config Error : %s", tmp_error.c_str());

			loginlog::g_log_cmd.printf(LL_INFO, "Fail [%d][%s] %s : %s", cmd_id, creator.c_str(), cmd.c_str(), tmp_error.c_str());
		}
		else
		{
			loginlog::g_log_cmd.printf(LL_INFO, "Suc [%d][%s] %s", cmd_id, creator.c_str(), cmd.c_str());
		}
		return;
	}

	// 命令分析不成功
	loginlog::g_log_cmd.printf(LL_WARNING, "Parse Error : [%d][%s] %s", cmd_id, creator.c_str(), cmd.c_str());
}

void LoginServer::SetProfNum(int prof1_num, int prof2_num, int prof3_num, int prof4_num)
{
	m_prof1_num = prof1_num;
	m_prof2_num = prof2_num;
	m_prof3_num = prof3_num;
	m_prof4_num = prof4_num;
}

void LoginServer::GetProfNum(int *prof1_num, int *prof2_num, int *prof3_num, int *prof4_num)
{
	if (NULL != prof1_num) *prof1_num = m_prof1_num;
	if (NULL != prof2_num) *prof2_num = m_prof2_num;
	if (NULL != prof3_num) *prof3_num = m_prof3_num;
	if (NULL != prof4_num) *prof4_num = m_prof4_num;
}

void LoginServer::OnRoleForbid(lgprotocal::RoleForbid *rf)
{
	if (rf->forbit_time_s <= 0) return;

	rf->plat_name[sizeof(rf->plat_name) - 1] = 0;
	rf->reason[sizeof(rf->reason) - 1] = 0;

	RMIForbidBackObjectImpl *fboi = new RMIForbidBackObjectImpl();
	fboi->login_server = this;
	F_STRNCPY(fboi->pname, rf->plat_name, sizeof(fboi->pname));
	fboi->cmd_id = 0;
	fboi->creator = rf->reason;
	fboi->cmd = "";

	RMILoginClient lc;
	lc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());
	bool ret = lc.Frobid(rf->plat_name, rf->forbit_time_s, fboi);
	
	loginlog::g_log_role_forbid.printf(LL_INFO, "Role Forbid %s! plat_name:%s reason:%s", ret ? "Succ" : "Fail", rf->plat_name, rf->reason);
}

void RMILoginInitWorldStatusBackObjectImpl::InitWorldStatusRet(int result, bool is_register_limit)
{
	if (0 == result)
	{
		LoginServer::Instance().SetIsRegisterLimit(is_register_limit);
	}
}

void LoginServer::StartWorldStatus()
{
	RMILoginInitWorldStatusBackObjectImpl *impl = new RMILoginInitWorldStatusBackObjectImpl();

	RMILoginClient lc;
	lc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());
	lc.InitWorldStatus(impl);
}

void LoginServer::SaveWorldStatus()
{
	RMILoginSaveWorldStatusBackObjectImpl *impl = new RMILoginSaveWorldStatusBackObjectImpl();

	RMILoginClient lc;
	lc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());
	lc.SaveWorldStatus(m_is_register_limit, impl);
}
