#include "world.h"
#include "ilogmodule.h"
#include "gamedef.h"
#include "utility/configpath.h"
#include "internalcomm.h"
#include "gamelog.h"
#include "servercommon/internalprotocal/lgprotocal.h"
#include "servercommon/internalprotocal/wgprotocal.h"
#include "servercommon/serverconfig/serverconfigpool.h"
#include "common/memory/memmonitor.h"
#include "gamecommon.h"
#include "task/taskpool.h"
#include "scene/asynreqregister/asynreqregister.hpp"
#include "other/monitor/systemmonitor.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "other/worldboss/worldbossshadow.hpp"
#include "other/monsterdropsum/monsterdropsum.hpp"
#include "scene/marryshadow/marryshadow.h"

#include "global/camp/campmanager.hpp"
#include "global/mail/mailmanager.hpp"
#include "global/rank/rankmanager.hpp"
#include "global/tuodan/tuodanmanager.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/offlineregister/offlineregistermanager.hpp"
#include "global/trade/userpublicsalemanager.hpp"
#include "global/activity/activitymanager.hpp"
#include "global/worldboss/worldbossmanager.hpp"
#include "global/bossfamily/bossfamilymanager.hpp"
#include "global/system/system.hpp"
#include "global/specialactivity/specialactivitymanager.hpp"
#include "global/rank/topranker.hpp"
#include "global/challengefield/challengefield.hpp"
#include "global/crosschallengefield/crosschallengefield.hpp"
#include "servercommon/performancecatch.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "friend/friendblessmanager.h"
#include "global/fishpool/fishpoolmanager.hpp"
#include "global/worldevent/worldeventmanager.hpp"
#include "global/redpaper/redpapermanager.h"
#include "global/roleboss/rolebossmanager.h"
#include "global/combineserveractivity/combineserveractivitymanager.hpp"
#include "global/cross/crossmanager.h"
#include "global/cross/crossactivity/crossrecordmanager.hpp"

#include "scene/speciallogic/worldspecial/yaoshouguangchangmgr.hpp"
#include "scene/speciallogic/worldspecial/suoyaotamgr.hpp"

#include "global/offlineregister/spacemanager.hpp"

#include "scene/speciallogic/teamspecial/zhuaguimanager.hpp"
#include "scene/speciallogic/worldspecial/jinghuahusongmgr.hpp"
#include "scene/speciallogic/worldspecial/huangchenghuizhanmgr.hpp"
#include "scene/speciallogic/worldspecial/campdefendmanager.hpp"
#include "scene/guildshadow/guildshadow.hpp"
#include "config/logicconfigmanager.hpp"
#include "global/globalrole/globalrolemanager.hpp"
#include "other/shop/mysteriousshop.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "other/jinglingadvantage/jinglingadvantage.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "global/bossshanggu/bossshanggumanager.hpp"
#include "global/rank/couplerankmanager.hpp"
#include "scene/teamequipfbmanager/teamequipfbmanager.hpp"
#include "scene/roleequipfbmanager/roleequipfbmanager.hpp"
#include "global/worldstatus3/worldstatus3.hpp"
#include "scene/challengefbmanager/challengefbmanager.hpp"
#include "gameworld/gameworld/other/event/eventhandler.hpp"
#include "global/crossrandactivity/crossramanagerlocal.hpp"
#include "global/fightback/fightbackmanager.hpp"

static World *world_instance = NULL;

World::World(ILogModule *log, 
			 IConfigModule *config, 
			 IGameModule *game, 
			 INetworkModule *network, 
			 INetworkModule *internal_network, 
			 IClockModule *clock,
			 IRMIModule *rmi_module,
			 ITimerModule *timer,
			 IFileWriteModule *filewrite,
			 int total_server,
			 int server_index)
	: m_game_time(0), m_scene_manager(), m_total_server(total_server), m_server_index(server_index), m_listen_port(0), 
	m_global_server_netid(-1), m_login_server_netid(-1), m_is_on_server_start(false), m_last_update_dayid(0), m_last_reset_role_module_data_dayid(0),
	m_last_reset_role_module_timestamp(0), m_next_reset_role_module_timestamp(0)
{
	m_scene_manager.SetWorld(this);

	m_network = network;
	EngineAdapter::Instance().Init(log, config, game, network, clock, rmi_module, timer, filewrite);

	// 获取日志生成目录
	std::string log_dir = "log/";
	EngineAdapter::Instance().ConfigSyncValue(ROOT/"LogDir", &log_dir, log_dir);
	if (log_dir[log_dir.length() - 1] != '/')
	{
		log_dir += '/';
	}
	
	int open_console_log = 0;
	EngineAdapter::Instance().ConfigSyncValue(ROOT/"OpenConsoleLog", &open_console_log, open_console_log);

	gamelog::LogInit(log, log_dir, 0 != open_console_log, m_server_index);

	InternalComm::Instance().Init(internal_network);

	world_instance = this;
}

World::~World()
{
}

World * World::GetInstWorld()
{
	return world_instance;
}

bool World::Init()
{
	gamelog::g_log_world.print(LL_MAINTANCE, "Game Global Init.");

	gamecommon::GlobalInit();

	gamelog::g_log_world.print(LL_MAINTANCE, "World Init.");

	return true;
}

class MsgMoniterTimer : public ITimerCallback
{
	int interval_s;

public:
	MsgMoniterTimer(int i):interval_s(i) {}

	virtual void OnTimer()
	{
		static char buff[1024 * 10];

		memmonitor::PrintDyMemInfo(buff);

		gamelog::g_log_msgmoniter.print(LL_INFO, buff);

		EngineAdapter::Instance().CreateTimerSecond(interval_s, new MsgMoniterTimer(interval_s));
	}

	virtual void Free() { delete this; }
};

bool World::Start(bool check_res_mode, int total_server, int server_index)
{
	m_local_ip = "127.0.0.1";
	m_listen_port = 7777;
	EngineAdapter::Instance().ConfigSyncValue(ROOT/"World"/"GameServer"/"Index", &m_server_index, m_server_index);
	EngineAdapter::Instance().ConfigSyncValue(ROOT/"World"/"GameServer"/"LocalIP", &m_local_ip, m_local_ip);
	EngineAdapter::Instance().ConfigSyncValue(ROOT/"World"/"GameServer"/"ListenPort", &m_listen_port, m_listen_port);
	m_total_server = total_server; m_server_index = server_index;
	m_listen_port += server_index; // 监听端口递增
	EngineAdapter::Instance().m_server_index = m_server_index; // 使用命令行参数的

	// 获取配置目录
	std::string config_dir = "../config";
	EngineAdapter::Instance().ConfigSyncValue(ROOT/"ConfigDir", &config_dir, config_dir);
	if (config_dir[config_dir.length() - 1] != '/')
	{
		config_dir += '/';
	}

	// 加载所有服务器配置
	char err_info[1024]={0};
	if(!ServerConfigPool::Instance().Init(config_dir, err_info))
	{
		gamelog::g_log_world.printf(LL_ERROR, err_info);
		gamelog::g_log_world.printf(LL_ERROR, "World On INIT, ServerConfigPool::Init return FALSE.");
		return false;
	}
	gamelog::g_log_world.printf(LL_INFO, "World On INIT, ServerConfigPool::Init Success.");

	InternalComm::Instance().SetSesstionKeyBase(ServerConfigPool::Instance().common_config.sessionkey);
	
	if (!check_res_mode)
	{
		if (!InitChannel())
		{
			return false;
		}
	}

	// 读配置 初始化场景
	clock_t init_beg_time = clock();
	if (!InitGameWorld(config_dir))				
	{
		return false;
	}

	clock_t init_end_time = clock();
	gamelog::g_log_world.printf(LL_INFO, "========== InitGameWorld Succ, total cost time: %f s ==========", (init_end_time - init_beg_time) * 1.0 / CLOCKS_PER_SEC);
	
	// 资源检查模式  
	if (check_res_mode)							
	{
		gamelog::g_log_world.printf(LL_MAINTANCE, "[Resource Check Success.] dayid:%d", EngineAdapter::Instance().DayID());
		return true;
	}

	// 角色缓存和好友关系缓存
	if (!LoadFromDBDirectly())
	{
		return false;
	}

	// 连接数据存取服务器
	if (!ConnectToDB())							
	{
		return false;
	}

	// 连接登陆服务器
	if (!ConnectToLoginServer())
	{
		return false;
	}

	// 为gateway监听
	if (!ListenForGateway())
	{
		return false;
	}
	
	gamelog::g_log_world.printf(LL_MAINTANCE, "World[total_server:%d server_index:%d] Start.", m_total_server, m_server_index);
	m_game_time = EngineAdapter::Instance().GetGameTime();

	static const int MSG_MONITER_INTERVAL_S = 60;
	EngineAdapter::Instance().CreateTimerSecond(MSG_MONITER_INTERVAL_S, new MsgMoniterTimer(MSG_MONITER_INTERVAL_S));

	return true;
}

void World::Update()
{
	//unsigned long beg_time = PITime();

	if (!m_is_on_server_start)
	{
		m_is_on_server_start = true;
		this->OnServerStart();
		return;
	}

	if (!this->IsAllDataLoadFinish()) // 所有数据加载完了才准update
	{
		return;
	}

	if (0 == m_last_reset_role_module_data_dayid)
	{
		m_last_reset_role_module_data_dayid = WorldStatus::Instance().GetLastResetRoleModuleDataDayId();
		m_last_reset_role_module_timestamp = WorldStatus::Instance().GetLastResetRoleModuleDataTimestamp();
		this->RecalNextResetDataTimeStamp();
	}

	if (0 == m_last_update_dayid)
	{
		m_last_update_dayid = WorldStatus::Instance().GetLastUpdateDayId();

		this->OnAllDataLoadFinish();
	}

	unsigned long now = EngineAdapter::Instance().GetGameTime();
	
	unsigned long interval = now - m_game_time;

	this->UpdateGlobal(interval);
	m_scene_manager.Update(interval);
	ActivityShadow::Instance().Update(interval);
	SystemMonitor::Instance().Update(interval);
	WorldBossShadow::Instance().Update(interval);
	MonsterDropSum::Instance().Update(interval);
	MarryShadow::Instance().Update(interval);
	PerformanceCatch::Instance().Update(interval);
	GuildShadow::Instance().Update(interval);
	

	// 如果有跨服断开，尝试重连跨服
	if (CrossConfig::Instance().IsOpenCross() && !CrossManager::Instance().IsAllCrossServerConnected())
	{
		static time_t next_check_cross_connection_time = 0;
		if (next_check_cross_connection_time == 0 || EngineAdapter::Instance().Time() >= next_check_cross_connection_time)
		{
			next_check_cross_connection_time = EngineAdapter::Instance().Time() + 5;
			CrossManager::Instance().CheckCrossConnection();
		}
	}

	m_game_time = now;

	/*if (m_game_time -  m_begin_time> 1000)
	{
		gamelog::g_log_world.printf(LL_DEBUG, "total:%d pc:%d(%d) ec:%d(%d) lc:%d(%d)", gamestat::g_game_stat.user_packet_count, 
			gamestat::g_game_stat.sync_pos_count, gamestat::g_game_stat.sync_pos_packege_count,
			gamestat::g_game_stat.enter_count, gamestat::g_game_stat.enter_packege_count,
			gamestat::g_game_stat.leave_count, gamestat::g_game_stat.leave_packege_count);
		m_begin_time = m_game_time;
	}*/

	//gamelog::g_log_world.printf(LL_DEBUG, "World Update cast %d ms.", PITime() - beg_time);
}

void World::UpdateGlobal(unsigned long intervals)
{
	time_t now_second = EngineAdapter::Instance().Time();

	RedPaperManager::Instance().Update(intervals, now_second);
	ActivityManager::Instance().Update(intervals, now_second);
	SpecialActivityManager::Instance().Update(intervals, now_second);
	CampManager::Instance().Update(intervals, now_second);
	UserMailManager::Instance().Update(intervals, now_second);
	RankManager::Instance().Update(intervals, now_second);
	WorldStatus::Instance().Update(intervals, now_second);
	GuildManager::Instance().Update(intervals, now_second);
	OfflineRegisterManager::Instance().Update(intervals, now_second);
	UserPublicSaleManager::Instance().Update(intervals, now_second);
	System::Instance().Update(intervals, now_second);
	TopRanker::Instance().Update(intervals, now_second);
	ChallengeField::Instance().Update(intervals, now_second);
	CrossChallengeField::Instance().Update(intervals, now_second);
	WorldBossManager::Instance().Update(intervals, now_second);
	BossFamilyManager::Instance().Update(intervals, now_second);
	BossShangGuManager::Instance().Update(intervals, now_second);
	RandActivityManager::Instance().Update(intervals, now_second);
	FishPoolManager::Instance().Update(intervals, now_second);
	WorldEventManager::Instance().Update(intervals, now_second);
	RoleBossManager::Instance().Update(intervals, now_second);
	CombineServerActivityManager::Instance().Update(intervals, now_second);
	CrossRecordManager::Instance().Update(intervals, now_second);
	CrossManager::Instance().Update(intervals, now_second);
	YaoShouGuangChangMgr::Instance().Update(intervals, now_second);
	SuoYaoTaMgr::Instance().Update(intervals, now_second);
	SpaceManager::Instance().Update(intervals, now_second);
	ZhuaGuiMgr::Instance().Update(intervals, now_second);
	JingHuaHuSongMgr::Instance().Update(intervals, now_second);
	HuangchenghuizhanMgr::Instance().Update(intervals, now_second);
	LOGIC_CONFIG->GetMysteriousShop().Update(intervals, now_second);
	CampDefendMgr::Instance().Update(intervals, now_second);
	GlobalRoleManager::Instance().Update(intervals, now_second);
	TuodanManager::Instance().Update(intervals, now_second);
	WorldStatus2::Instance().Update(intervals, now_second);
	WorldStatus3::Instance().Update(intervals, now_second);
	CoupleRankManager::Instance().Update(intervals, now_second);
	TeamEquipFBManager::Instance().Update(intervals);
	RoleEquipFBManager::Instance().Update(intervals);
	ChallengeFBManager::Instance().Update(intervals);
	CrossRAManagerLocal::Instance().Update(intervals, now_second);
	FightBackManager::Instance().Update(intervals, now_second);

	unsigned int today_id = EngineAdapter::Instance().DayID();

	if (today_id != m_last_update_dayid)
	{	
		this->OnDayChange(m_last_update_dayid, today_id);

		m_last_update_dayid = today_id;
		WorldStatus::Instance().SetLastUpdateDayId(m_last_update_dayid);
	}

	if (m_next_reset_role_module_timestamp > 0 && now_second >= m_next_reset_role_module_timestamp)
	{
		this->OnResetRoleModuleData(m_last_reset_role_module_data_dayid, today_id); 

		m_last_reset_role_module_data_dayid = today_id;
		m_last_reset_role_module_timestamp = now_second;

		WorldStatus::Instance().SetLastResetRoleModuleDataDayId(m_last_reset_role_module_data_dayid);
		WorldStatus::Instance().SetLastResetRoleModuleDataTimestamp(static_cast<unsigned int>(m_last_reset_role_module_timestamp));

		this->RecalNextResetDataTimeStamp();
	}
}

void World::Stop(bool check_res_mode)
{
	if (check_res_mode)							// 资源检查模式  
	{
		return;
	}

	m_scene_manager.LogoutAllRole();

	//for (int i = 0; i < (int)m_db_rmi_session_list.size(); ++i)
	//{
	//	EngineAdapter::Instance().RMICloseSession(m_db_rmi_session_list[i]);
	//}

	this->OnServerStop();

	gamelog::g_log_world.print(LL_MAINTANCE, "World Stop.");
}

void World::Release()
{
	m_scene_manager.Release();
	gamelog::g_log_world.print(LL_MAINTANCE, "World Release.");
}

void World::OnAccept(Port listen_port, NetID netid, IP ip, Port port)
{
	m_gateway_set.insert(netid);
}

void World::OnRecv(NetID netid, const char *data, int length)
{
	wgprotocal::MessageHeader *header = (wgprotocal::MessageHeader*)data;

	switch (header->msg_type)
	{
	case wgprotocal::MT_NET_RECV_MSG:
		OnRecvMsg(netid, data, length);
		break;
	case wgprotocal::MT_NET_DISCONNECT_NOTICE:
		OnNetDisconnect(netid, data);
		break;
	default:
		break;
	}
}

void World::OnRecvMsg(NetID netid, const char *data, int length)
{
	wgprotocal::WGNetRecvMsg *nrm = (wgprotocal::WGNetRecvMsg*)data;
	GSNetID idtmp((unsigned short)(netid), (unsigned short)(nrm->header.netid));
	m_scene_manager.OnRecv(idtmp, nrm->header.user_ip, data + sizeof(wgprotocal::WGNetRecvMsg), length - sizeof(wgprotocal::WGNetRecvMsg));
}

void World::OnNetDisconnect(NetID netid, const char *data)
{
	wgprotocal::WGNetDisconnect *nd = (wgprotocal::WGNetDisconnect*)data;

	GSNetID idtmp((unsigned short)(netid), (unsigned short)(nd->header.netid));
	m_scene_manager.OnDisconnect(idtmp);

	AsynReqRegister::Instance().OnNetidDisconnect(idtmp);
}

void World::OnDisconnect(NetID netid)
{
	m_scene_manager.OnGateWayDisconnect(netid);
	m_gateway_set.erase(netid);

	if (m_gateway_set.empty())	// 所有的gateway断开时候保存所有世界相关的信息 
	{
	}
}

void World::OnInternalAccept(Port listen_port, NetID netid, IP ip, Port port)
{
}

void World::OnInternalRecv(NetID netid, const char *data, int length)
{
	if (netid == m_login_server_netid)
	{
		// 登陆服务器来消息
		lgprotocal::MessageHeader *header = (lgprotocal::MessageHeader*)data;
		switch (header->msg_type)
		{
		case lgprotocal::MT_LG_REGISTER_GS:
			OnLGRegisterGS(data);
			break;
		case lgprotocal::MT_LG_UNREGISTER_GS:
			OnLGUnRegisterGS(data);
			break;
		case lgprotocal::MT_KICK_ROLE:
			OnLGKickRole(data);
			break;
		default:
			break;
		}
		return;
	}
	else if (CrossManager::Instance().IsCrossNetID(netid))
	{
		m_scene_manager.OnCrossServerRecv(netid, data, length);
	}
}

void World::OnInternalDisconnect(NetID netid)
{
	if (netid == InternalComm::Instance().m_login_server_id)
	{
		InternalComm::Instance().m_login_server_id = -1;

		// 启动重连机制
		ReConnectToLogin();

		gamelog::g_log_world.printf(LL_MAINTANCE, "OnInternalDisconnect loginserver: netid[%d]", (int)netid);
	}
	else if (CrossManager::Instance().IsCrossNetID(netid))  // 注意！！如果是热更cross.xml导致的断开，由于m_cross_info_map被清除这里将永远返回flase
	{
		CrossManager::Instance().OnDisconnectFromCrossServer(netid);
		
		gamelog::g_log_world.printf(LL_MAINTANCE, "OnInternalDisconnect crossserver netid[%d]", (int)netid);
	}
}

void World::ReConnectToLogin()
{
	std::string login_server_ip = ServerConfigPool::Instance().common_config.ipconfigs.login_server_addr.inner_connect_addr.server_ip;
	Port login_server_port = ServerConfigPool::Instance().common_config.ipconfigs.login_server_addr.inner_connect_addr.for_gameserver.port;

	int handle = -1;
	bool ret = InternalComm::Instance().NetConnectAsyn(login_server_ip.c_str(), login_server_port, &handle, 1000);
	if (ret)
	{
		InternalComm::Instance().m_reconnet_login_handle = handle;
	}
	else
	{
		// log
	}
}

bool World::RegisterToLogin()
{
	lgprotocal::GLRegisterGS rgs;
	STRNCPY(rgs.ip, m_local_ip.c_str(), sizeof(rgs.ip));
	rgs.index = m_server_index;
	rgs.port = m_listen_port;
	rgs.scene_count = m_scene_manager.GetSceneList(rgs.scene_list, MAX_SCENE_LIST_NUM);
	return InternalComm::Instance().NetSend(InternalComm::Instance().m_login_server_id, (const char *)&rgs, sizeof(lgprotocal::GLRegisterGS));
}

void World::OnLGRegisterGS(const char *msg)
{
	lgprotocal::LGRegisterGS *rgs = (lgprotocal::LGRegisterGS*)msg;
	
	if (rgs->index == m_server_index) return; // 把本地的去掉 只记录别的gs
	
	for (int i = 0; i < rgs->scene_count && i < MAX_SCENE_LIST_NUM; ++i)
	{
		m_scene_manager.AddGS(rgs->scene_list[i], rgs->index);
	}
}

void World::OnLGUnRegisterGS(const char *msg)
{
	lgprotocal::LGUnRegisterGS *urgs = (lgprotocal::LGUnRegisterGS*)msg;

	for (int i = 0; i < urgs->scene_count && i < MAX_SCENE_LIST_NUM; ++i)
	{
		m_scene_manager.DeleteGS(urgs->scene_list[i]);
	}
}

void World::OnLGKickRole(const char *msg)
{
	lgprotocal::LGKickRole *lkr = (lgprotocal::LGKickRole*)msg;

	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(lkr->role_id));
	if (NULL != role)
	{
		EngineAdapter::Instance().NetDisconnect(role->GetNetId(), role->GetUserId(), role->GetName(), 0, "Be Frobit form Login KickOut");
	}
	EventHandler::Instance().OnForbidRole(lkr->role_id);
}

bool World::SendToAllGateway(const char *msg, int length)
 {
	bool ret = true;

	for (GateWaySet::iterator i = m_gateway_set.begin(); i != m_gateway_set.end(); ++i)
	{
		ret &= EngineAdapter::Instance().NetSendAll(*i, msg, length);
	}

	return ret;
}

bool World::SendToAllGatewayAndCrossUser(const char *msg, int length)
{
	bool ret = true;

	// 跨服中
	CrossUserMap cross_user_map = m_scene_manager.GetCrossUserMap();
	for (auto it : cross_user_map)
	{
		InternalComm::Instance().UserProtocalTransferToGame(it.first, msg, length);
	}

	// 本服
	for (GateWaySet::iterator i = m_gateway_set.begin(); i != m_gateway_set.end(); ++i)
	{
		ret &= EngineAdapter::Instance().NetSendAll(*i, msg, length);
	}

	return ret;
}

void World::OnServerStart()
{
	UserMailManager::Instance().OnServerStart();
	CampManager::Instance().OnServerStart();
	RankManager::Instance().OnServerStart();
	WorldStatus::Instance().OnServerStart();
	GuildManager::Instance().OnServerStart();
	OfflineRegisterManager::Instance().OnServerStart();
	UserPublicSaleManager::Instance().OnServerStart();
	WorldBossManager::Instance().OnServerStart();
	BossFamilyManager::Instance().OnServerStart();
	BossShangGuManager::Instance().OnServerStart();
	System::Instance().Init();
	ChallengeField::Instance().OnServerStart();
	CrossChallengeField::Instance().OnServerStart();
	YaoShouGuangChangMgr::Instance().OnServerStart();
	SuoYaoTaMgr::Instance().OnServerStart();
	SpaceManager::Instance().OnServerStart();
	JingHuaHuSongMgr::Instance().OnServerStart();
	FishPoolManager::Instance().OnServerStart();
	GlobalRoleManager::Instance().OnServerStart();
	TuodanManager::Instance().OnServerStart();
	JinglingAdvantage::Instance().OnServerStart();
	//CrossRecordManager::Instance().OnServerStart();
	WorldStatus2::Instance().OnServerStart();
	WorldStatus3::Instance().OnServerStart();
}

void World::OnServerStop()
{
	if (!this->IsAllDataLoadFinish()) // 所有数据加载完了才准保存下面数据
	{
		return;
	}

	WorldBossManager::Instance().OnStopServer();
	BossFamilyManager::Instance().OnStopServer();
	BossShangGuManager::Instance().OnStopServer();
	RedPaperManager::Instance().OnStopServer();	// 必须放在UserMailManager和OfflineRegisterManager之前，
												// 因为1.需要通过邮件把元宝退给玩家； 2.需要把离线玩家发红包消耗的元宝数记下
	UserMailManager::Instance().OnServerStop();
	CampManager::Instance().OnServerStop();
	RankManager::Instance().OnServerStop();
	WorldStatus::Instance().OnServerStop();
	GuildManager::Instance().OnServerStop();
	OfflineRegisterManager::Instance().OnServerStop();
	UserPublicSaleManager::Instance().OnServerStop();
	ChallengeField::Instance().OnServerStop();
	CrossChallengeField::Instance().OnServerStop();
	FishPoolManager::Instance().OnServerStop();
	CrossRecordManager::Instance().OnServerStop();
	SpaceManager::Instance().OnServerStop();
	GlobalRoleManager::Instance().OnServerStop();
	TuodanManager::Instance().OnServerStop();
	WorldStatus2::Instance().OnServerStop();
	WorldStatus3::Instance().OnServerStop();
}

bool World::IsAllDataLoadFinish()
{
	if(!CampManager::Instance().IsLoadFinish())
	{
		return false;
	}
	if(!WorldStatus::Instance().IsLoadFinish())
	{
		return false;
	}
	if(!OfflineRegisterManager::Instance().IsLoadFinish())
	{
		return false;
	}
	if(!RankManager::Instance().IsLoadFinish())
	{
		return false;
	}
	if(!TopRanker::Instance().IsLoadFinish())
	{
		return false;
	}
	if(!GuildManager::Instance().IsGuildLoadFinish())
	{
		return false;
	}
	if(!UserPublicSaleManager::Instance().IsLoadFinish())
	{
		return false;
	}
	if(!ChallengeField::Instance().IsLoadFinish())
	{
		return false;
	}
// 	if (!CrossChallengeField::Instance().IsLoadFinish())
// 	{
// 		return false;
// 	}

	if (!CrossConfig::Instance().IsHiddenServer() && !GlobalRoleManager::Instance().IsLoadFinish())
	{
		return false;
	}
	if (!TuodanManager::Instance().IsLoadFinish())
	{
		return false;
	}
// 	if(!FishPoolManager::Instance().IsLoadFinish())
// 	{
// 		return false;
// 	}
// 	if(!CrossRecordManager::Instance().IsLoadFinish())
// 	{
// 		return false;
// 	}
	if (!WorldStatus2::Instance().IsLoadFinish())
	{
		return false;
	}
	if (!WorldStatus3::Instance().IsLoadFinish())
	{
		return false;
	}

	return true;
}

void World::OnAllDataLoadFinish()
{
	CrossManager::Instance().CheckCrossConnection();
}

void World::OnResetRoleModuleData(unsigned int old_dayid, unsigned int now_dayid)
{
}

void World::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;

	ChallengeField::Instance().OnDayChange(old_dayid, now_dayid);
	CrossChallengeField::Instance().OnDayChange(old_dayid, now_dayid);
	FriendBlessManager::Instance().OnDayChange(old_dayid, now_dayid);
	FishPoolManager::Instance().OnDayChange(old_dayid, now_dayid);
	WorldEventManager::Instance().OnDayChange(old_dayid, now_dayid);
	GuildManager::Instance().OnDayChange(old_dayid, now_dayid);
	RankManager::Instance().OnDayChange(old_dayid, now_dayid);
	WorldStatus::Instance().OnDayChange(old_dayid, now_dayid);
	CampManager::Instance().OnDayChange(old_dayid, now_dayid);
	SpecialActivityManager::Instance().OnDayChange(old_dayid, now_dayid);
	TopRanker::Instance().OnDayChange(old_dayid, now_dayid);
	RandActivityManager::Instance().OnDayChange(old_dayid, now_dayid);
	CrossRecordManager::Instance().OnDayChange(old_dayid, now_dayid);
	GlobalRoleManager::Instance().OnDayChange(old_dayid, now_dayid);
	WorldBossManager::Instance().OnDayChange(old_dayid, now_dayid);
	BossFamilyManager::Instance().OnDayChange(old_dayid, now_dayid);
	BossShangGuManager::Instance().OnDayChange(old_dayid, now_dayid);
	m_scene_manager.OnDayChange(old_dayid, now_dayid);
	WorldStatus2::Instance().OnDayChange(old_dayid, now_dayid);
	ActivityShadow::Instance().OnDayChange(old_dayid, now_dayid);
	CoupleRankManager::Instance().OnDayChange(old_dayid, now_dayid);
	WorldStatus3::Instance().OnDayChange(old_dayid, now_dayid);
	CrossRAManagerLocal::Instance().OnDayChange(old_dayid, now_dayid);

	const tm *tm_now = EngineAdapter::Instance().LocalTime();
	if (NULL != tm_now && 1 == tm_now->tm_wday)
	{
		this->OnWeekChange();
	}

	// 必须放在最后，否则之后的模块使用仙盟每日增加战力排行榜会有问题
	GuildManager::Instance().RecordYesterdayCapability();
}

void World::OnWeekChange()
{
	RankManager::Instance().OnWeekChange();
	TopRanker::Instance().OnWeekChange();
	CrossRecordManager::Instance().OnWeekChange();
	WorldBossManager::Instance().OnWeekChange();
	CrossRAManagerLocal::Instance().OnWeekChange();
}
