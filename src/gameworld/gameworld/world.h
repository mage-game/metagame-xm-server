#ifndef __WORLD_H__
#define __WORLD_H__

#include <set>
#include "engineadapter.h"
#include "scene/scenemanager.h"

class ILogModule;
class IConfigModule;
class IGameModule;
class INetworkModule;
class IClockModule;
class IRMIModule;
class ITimerModule;
class IFileWriteModule;

class World
{
public:
	World(ILogModule *log, 
		IConfigModule *config, 
		IGameModule *game, 
		INetworkModule *network, 
		INetworkModule *internal_network, 
		IClockModule *clock,
		IRMIModule *rmi_module,
		ITimerModule *timer,
		IFileWriteModule *filewrite,
		int total_server,
		int server_index);
	~World();

	static World * GetInstWorld();

	bool Init();
	bool Start(bool check_res_mode, int total_server, int server_index);
	void Update();
	void Stop(bool check_res_mode);
	void Release();

	void OnAccept(Port listen_port, NetID netid, IP ip, Port port);
	void OnRecv(NetID netid, const char *data, int length);
	void OnDisconnect(NetID netid);

	void OnInternalAccept(Port listen_port, NetID netid, IP ip, Port port);
	void OnInternalRecv(NetID netid, const char *data, int length);
	void OnInternalDisconnect(NetID netid);

	void ReConnectToLogin();
	bool RegisterToLogin();

	bool SendToAllGateway(const char *msg, int length);				// 发送给本服所有在线的玩家(不包括当前处于跨服的)
	bool SendToAllGatewayAndCrossUser(const char *msg, int length); // 发送给本服所有在线包括当前处于跨服状态的玩家

	SceneManager * GetSceneManager() { return &m_scene_manager; }

	bool ReloadScene(const std::string &config_dir, std::string *err);

	bool IsAllDataLoadFinish(); 
	void OnAllDataLoadFinish();

	void OnResetRoleModuleData(unsigned int old_dayid, unsigned int now_dayid);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnWeekChange();

private:
	INetworkModule *m_network;
	unsigned long m_game_time;
	
	SceneManager m_scene_manager;
	std::vector<rmi::Session> m_db_rmi_session_list;

	typedef std::set<NetID> GateWaySet;
	GateWaySet		m_gateway_set;

	std::string		m_local_ip;

	int				m_total_server;
	int				m_server_index;

	Port			m_listen_port;

	NetID			m_global_server_netid;
	NetID			m_login_server_netid;

	bool			m_is_on_server_start;
	unsigned int	m_last_update_dayid;

	unsigned int	m_last_reset_role_module_data_dayid;
	time_t			m_last_reset_role_module_timestamp;
	time_t			m_next_reset_role_module_timestamp;

	// from gateway
	void OnRecvMsg(NetID netid, const char *data, int length);
	void OnNetDisconnect(NetID netid, const char *data);

	// from loginserver
	void OnLGRegisterGS(const char *msg);
	void OnLGUnRegisterGS(const char *msg);
	void OnLGKickRole(const char *msg);

protected:
	void RecalNextResetDataTimeStamp();
	bool ListenForGateway();

	bool ConnectToDB();
	bool ConnectToLoginServer();

	bool LoadFromDBDirectly();

	void OnServerStart();
	void OnServerStop();

	void UpdateGlobal(unsigned long intervals);

	bool InitChannel();

	bool InitGameWorld(std::string config_dir);

	bool InitGlobalConfig(const std::string &config_dir);								// 全局配置 存放一些不太多的模块配置
	bool InitSkill(const std::string &config_dir);										// 技能
	bool InitItem(const std::string &config_dir);										// 物品
	bool InitScene(const std::string &config_dir);										// 场景
	bool InitMonster(const std::string &config_dir);									// 怪物
	bool InitTask(const std::string &config_dir);										// 任务
	bool InitDrop(const std::string &config_dir);										// 掉落
	bool InitLogicConfig(const std::string &config_dir);								// 逻辑配置 存放一些较大的配置
};

#endif
