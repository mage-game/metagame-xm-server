#ifndef __LOGINSERVER_H__
#define __LOGINSERVER_H__

#include <map>
#include <vector>
#include <set>

#include "servercommon/logindef.h"

#include "inetworkmodule.h"

#include "gameserver.h"
#include "utility/logagent.h"

class Platform;
namespace lgprotocal
{
	struct RoleForbid;
}

class LoginServer
{
public:
	~LoginServer();

	static LoginServer & Instance();

	bool Init(const std::vector<int> &db_index_to_create, ILogModule *log, const std::string &configpath, 
		int min_login_interval_s, int can_not_login_global_time);

	void Update();
	
	void OnAccept(Port listen_port, NetID netid, IP ip, Port port);			// 接入用户 
	void OnRecv(IP ip, NetID netid, const char *data, int length);
	void OnDisconnect(NetID netid);
	
	void OnGSAccept(Port listen_port, NetID netid, IP ip, Port port);		// 接入Game
	void OnGateAccept(Port listen_port, NetID netid, IP ip, Port port);		// 接入Gateway
	
	void OnGSDisconnect(GameServer *gs);									// Game断开
	void OnGateDisconnect(NetID netid);										// Geteway断开 

	void OnInnerRecv(NetID netid, const char *data, int length);
	void OnInnerDisconnect(NetID netid);

	GameServer * GetGSServer(int scene_id);
	void BroadcastToAllGSServer(const char *data, int sendlen);

	void StartCmd();
	void OnCmd(int cmd_id, const std::string &creator, const std::string &cmd);

	void SetProfNum(int prof1_num, int prof2_num, int prof3_num, int prof4_num);
	void GetProfNum(int *prof1_num, int *prof2_num, int *prof3_num, int *prof4_num);

	void StartWorldStatus();
	void SaveWorldStatus();
	void SetIsRegisterLimit(bool is_register_limit) { m_is_register_limit = is_register_limit; }

protected:
	LoginServer();

	LogAgent		m_log;

	int				m_min_login_interval_s;

	Platform		*m_platform;

	void OnLoginReq(NetID netid, const char *data);
	void OnRoleReq(IP ip, NetID netid, const char *data);
	void OnCreateRole(NetID netid, const char *data);
	void OnDestroyRole(NetID netid, const char *data);

	typedef std::vector<GameServer*> GSList;
	typedef std::vector<GameServer*>::iterator GSListIt;
	GSList m_gs_list;														// 接入的gamesvr列表
	
	typedef std::map<int, GameServer*> GSSceneList;
	typedef std::map<int, GameServer*>::iterator GSSceneListIt;
	GSSceneList m_scene_list;	

	void OnRegisterGS(GameServer *gs, const char *data);
	void AddGSServer(int scene_id, GameServer *gs);
	void RemoveGSServer(int scene_id);

	void OnGSRecv(GameServer *gs, const char *data, int length);

	void OnRoleForbid(lgprotocal::RoleForbid *rf);

public:
	struct GateWayServer
	{
		GateWayServer() : netid(-1), port(0) {}

		NetID			netid;
		std::string		hostname;
		unsigned short	port;
	};

	GateWayServer * RandomGatewayServer();

protected:
	typedef std::vector<GateWayServer*>	GateWayList;
	typedef std::vector<GateWayServer*>::iterator GateWayListIt;
	GateWayList		m_gateway_list;										// 快速索引使用 

	GateWayList		m_gameway_use_list;									// 当前有用的队列 仅仅用于随机选择gateway

	void AddGateWay(GateWayServer *gw);
	void RemoveGateWay(GateWayServer *gw);

	void OnRegisterGW(NetID netid, const char *data);
	void OnGateRecv(NetID netid, const char *data, int length);

	time_t last_check_prof_num_timestamp;
	int m_prof1_num;
	int m_prof2_num;
	int m_prof3_num;
	int m_prof4_num;

	bool m_is_register_limit;
	bool m_is_auto_set_register_limit;
};

#endif


