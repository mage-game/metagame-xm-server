#include "crossmanager.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "internalcomm.h"
#include "engineadapter.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "servercommon/serverconfig/crosscommonconfig.hpp"
#include "servercommon/internalprotocal/crossglobalprotocal.h"
#include "utility/configpath.h"
#include "crossusershadow.hpp"
#include "global/cross/crossuserregister.hpp"
#include "global/cross/crossactivity/crossrecordmanager.hpp"

#include "gamelog.h"
#include "config/sharedconfig/sharedconfig.h"
#include "servercommon/struct/roleallparam.hpp"
#include "world.h"
#include "servercommon/userprotocal/crossmsgcode.h"
#include "loginserver/server/gameserver.h"
#include "serverlogic.h"
#include <algorithm>
#include "global/bossfamily/bossfamilymanager.hpp"

CrossManager::CrossManager():m_last_print_msg_len_time(0)
{

}

CrossManager::~CrossManager()
{

}

CrossManager & CrossManager::Instance()
{
	static CrossManager cm;
	return cm;
}


void CrossManager::ConnectToCrossServerAsyn()
{
	std::map<int, NetAddress> &cross_cfg_map = CrossConfig::Instance().GetCrossAddrMap();

	for (std::map<int, NetAddress>::iterator net_it = cross_cfg_map.begin(); net_it != cross_cfg_map.end(); ++net_it)
	{
		int cur_cross_type = net_it->first;
		if (this->IsCrossServerConnected(cur_cross_type) || this->IsCrossServerConnecting(cur_cross_type)) continue;

		const NetAddress &net_addr = net_it->second;
		if (net_addr.Invalid()) continue;

		CrossInfoItem &cross_info = m_cross_info_map[cur_cross_type];
		cross_info.addr = net_addr;
		cross_info.cross_type = cur_cross_type;

		if (CrossConfig::Instance().IsHiddenServer() && cur_cross_type != CrossConfig::Instance().GetHiddenServerCrossType()) continue; // 隐藏服只连自己所在的跨服地址

		int handle = -1;
		bool ret = InternalComm::Instance().NetConnectAsyn(net_addr.ipstr.c_str(), net_addr.port, &handle, CROSS_CONNECT_ASYN_TIME_OUT_MS);
		if (ret)
		{
			cross_info.connet_handle = handle;

			if (!cross_info.has_start_connect) // 从开始到连接上跨服，只允许打印一次
			{
				cross_info.has_start_connect = true;
				gamelog::g_log_world.printf(LL_INFO, "[DEBUG] %u: ConnectToCrossServerAsyn [%s:%u] ...", (unsigned int)EngineAdapter::Instance().Time(),
					cross_info.addr.ipstr.c_str(), cross_info.addr.port);
			}
		}
	}
}

void CrossManager::DisconnectFromCrossServer(int cross_type)
{
	if (cross_type > 0)
	{
		CrossInfoMapIt cross_it = m_cross_info_map.find(cross_type);
		if (cross_it != m_cross_info_map.end())
		{
			if (!this->IsCrossServerConnected(cross_type)) return;

			NetID netid = cross_it->second.net_id;
			if ((NetID)-1 != netid)
			{
				InternalComm::Instance().NetDisconnect(netid);
			}
			CrossUserShadow::Instance().OnDisconnectFromCrossServer(cross_type);
			World::GetInstWorld()->GetSceneManager()->OnDisconnectFromCrossServer(cross_type);
			cross_it->second.net_id = -1;
			cross_it->second.has_connected = false;
			cross_it->second.has_start_connect = false;
			cross_it->second.connet_handle = -1;
		}
	}
	else
	{
		// 目前只有热更cross.xml配置才会执行这里。 并且是断开所有的crossserver
		for (CrossInfoMapIt cross_it = m_cross_info_map.begin(); cross_it != m_cross_info_map.end(); ++cross_it)
		{
			if (!this->IsCrossServerConnected(cross_it->first)) continue;

			NetID netid = cross_it->second.net_id;
			if ((NetID)-1 != netid)
			{
				InternalComm::Instance().NetDisconnect(netid);
			}
		}

		m_cross_info_map.clear(); // 要清掉，防止下次重新加载配置后，同样crosstype对应不一样的ip
		// 注意！！！
		// 由于m_cross_info_map在此处已经clear，那么当断开连接回调的时候World::OnInternalDisconnect根据netid获取不到跨服信息，导致不能正确的执行断开后续清理操作
		
		CrossUserShadow::Instance().OnDisconnectFromCrossServer(cross_type);
		World::GetInstWorld()->GetSceneManager()->OnDisconnectFromCrossServer(cross_type);
	}
}

void CrossManager::OnConnectCrossServerSucc(int handle, NetID netid)
{
	for (CrossInfoMapIt it = m_cross_info_map.begin(); it != m_cross_info_map.end(); ++it)
	{
		if (it->second.connet_handle >= 0 && it->second.connet_handle == handle)
		{
			it->second.net_id = netid;
			it->second.connet_handle = -1;
			it->second.hello_timestamp = EngineAdapter::Instance().Time();

			if (!it->second.has_connected) // 之前已连上跨服就不打日志
			{
				it->second.has_connected = 1;
				gamelog::g_log_world.printf(LL_INFO, "[DEBUG] %u: Connect CrossServer Succ! [%s:%u]", (unsigned int)EngineAdapter::Instance().Time(),
					it->second.addr.ipstr.c_str(), it->second.addr.port);
			}
			break;
		}
	}

	std::set<int> server_id_set = LocalConfig::Instance().GetAllowServerIdSet();

	if (!server_id_set.empty())
	{
		const int SERVER_MAX_COUNT = crossgameprotocal::GameCrossRegisterGameServer::MAX_SERVER_ID_NUM;
		UNSTD_STATIC_CHECK(LocalConfig::MAX_ALLOW_SERVER_ID_COUNT == SERVER_MAX_COUNT);

		crossgameprotocal::GameCrossRegisterGameServer gcrgs;
		gcrgs.cross_protocal_ver = CROSS_PROTOCAL_VERSION;
		gcrgs.plat_type = LocalConfig::Instance().GetPlatType();
		gcrgs.server_start_time = (unsigned int)SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime();
		gcrgs.server_combine_time = (unsigned int)SHAREDCONFIG->GetOpenServerConfig().GetServerRealCombineTime();
		gcrgs.role_all_param_size = sizeof(RoleAllParam);
		gcrgs.server_version = SHAREDCONFIG->GetOpenServerConfig().GetServerVersion();

		gcrgs.count = 0;
		for (std::set<int>::iterator it = server_id_set.begin(), end = server_id_set.end(); it != end && gcrgs.count < SERVER_MAX_COUNT; ++it)
		{
			gcrgs.server_id_list[gcrgs.count++] = *it;
		}

		int lenth = sizeof(gcrgs) - (sizeof(gcrgs.server_id_list[0]) * (SERVER_MAX_COUNT - gcrgs.count));
		InternalComm::Instance().NetSend(netid, (const char *)&gcrgs, lenth);
	}

	CrossUserRegister::Instance().OnConnectCrossServerSucc();
	CrossRecordManager::Instance().OnConnectCrossServerSucc();
	BossFamilyManager::Instance().OnConnectCrossServerSucc();
}

void CrossManager::OnDisconnectFromCross(CrossInfoItem *cross_info)
{
	int cross_type = -1;
	if (NULL != cross_info)
	{
		if (cross_info->has_connected) // 没连上跨服就不用重复打日志了
		{
			cross_info->has_connected = 0;
			cross_info->has_start_connect = 0;
			gamelog::g_log_world.printf(LL_INFO, "[DEBUG] %u: Disconnect From CrossServer!", (unsigned int)EngineAdapter::Instance().Time());
		}

		cross_info->net_id = -1;
		cross_info->connet_handle = -1;

		cross_type = cross_info->cross_type;
	}

	// 隐藏服一般只连一个跨服，有跨服断开肯定就没跨服连接了
	CrossUserShadow::Instance().OnDisconnectFromCrossServer(cross_type);
	World::GetInstWorld()->GetSceneManager()->OnDisconnectFromCrossServer(cross_type);
}

void CrossManager::OnDisconnectFromCrossServer(NetID net_id)
{
	this->OnDisconnectFromCross(this->GetCrossInfoWithNetID(net_id));
}

void CrossManager::OnDisconnectFromCrossServerHandle(int handle)
{
	this->OnDisconnectFromCross(this->GetCrossInfoWithHandle(handle));
}

void CrossManager::CheckCrossConnection()
{
	if (CrossConfig::Instance().IsOpenCross())
	{
		this->ConnectToCrossServerAsyn();
	}
	else
	{
		this->OnDisconnectFromCrossServer(0);
	}
}

bool CrossManager::ReloadCrossConfig()
{
	std::string config_dir = "../config";
	EngineAdapter::Instance().ConfigSyncValue(ROOT/"ConfigDir", &config_dir, config_dir);
	if (config_dir[config_dir.length() - 1] != '/')
	{
		config_dir += '/';
	}

	bool old_is_open = CrossConfig::Instance().IsOpenCross();
	std::string err;
	std::string configpath = config_dir + "cross.xml";
	bool is_cross_addr_change = false, is_open_cross_change = false;

	if (!CrossConfig::Instance().Reload(configpath, &err, &is_cross_addr_change, &is_open_cross_change))
	{
		return false;
	}

	this->OnReloadCrossServerConfig(is_cross_addr_change, old_is_open);

	return true;
}

bool CrossManager::IsCrossServerConnected(int cross_type)
{
	if (cross_type > 0)
	{
		CrossInfoMapIt cross_it = m_cross_info_map.find(cross_type);
		if (cross_it != m_cross_info_map.end())
		{
			return ((NetID)-1 != cross_it->second.net_id);
		}

		return false;
	}
	else
	{
		auto it = std::find_if(m_cross_info_map.begin(), m_cross_info_map.end(), [](const decltype(m_cross_info_map)::value_type& value)->bool {
			return value.second.net_id == NetID(-1);
		});
		return it == m_cross_info_map.end();
		/*bool some_cross_connected = false;
		for (CrossInfoMapIt cross_it = m_cross_info_map.begin(); cross_it != m_cross_info_map.end(); ++cross_it)
		{
			if ((NetID)-1 != cross_it->second.net_id)
			{
				some_cross_connected = true;
				break;
			}
		}

		return some_cross_connected;*/
	}
}

bool CrossManager::IsAllCrossServerConnected()
{
	if (m_cross_info_map.empty())		// 空的肯定没有连接跨服
	{
		return false;
	}

	bool all_connected = true;
	for (CrossInfoMapIt cross_it = m_cross_info_map.begin(); cross_it != m_cross_info_map.end(); ++cross_it)
	{
		if ((NetID)-1 == cross_it->second.net_id)
		{
			all_connected = false;
			break;
		}
	}

	return all_connected;
}

bool CrossManager::IsCrossServerConnecting(int cross_type)
{
	if (cross_type > 0)
	{
		CrossInfoMapIt cross_it = m_cross_info_map.find(cross_type);
		if (cross_it != m_cross_info_map.end())
		{
			return (-1 != cross_it->second.connet_handle);
		}

		return false;
	}
	else
	{
		bool some_cross_connecting = false;
		for (CrossInfoMapIt cross_it = m_cross_info_map.begin(); cross_it != m_cross_info_map.end(); ++cross_it)
		{
			if (-1 != cross_it->second.connet_handle)
			{
				some_cross_connecting = true;
				break;
			}
		}

		return some_cross_connecting;
	}
}

void CrossManager::OnReloadCrossServerConfig(bool is_cross_addr_change, bool old_is_open)
{
	bool now_is_open = CrossConfig::Instance().IsOpenCross();

	if (old_is_open)
	{
		if (!now_is_open || is_cross_addr_change) 
		{
			this->DisconnectFromCrossServer();
			gamelog::g_log_world.printf(LL_MAINTANCE, "Reload cross.xml. Call DisconnectFromCrossServer()");
		}
	}
	else
	{
		if (now_is_open || is_cross_addr_change)
		{
			this->ConnectToCrossServerAsyn();
		}
	}
}

void CrossManager::OnCrossHello(NetID net_id)
{
	CrossInfoItem *cross_info = this->GetCrossInfoWithNetID(net_id);
	if (NULL != cross_info)
	{
		cross_info->hello_timestamp = EngineAdapter::Instance().Time();
	}
}

void CrossManager::Update(unsigned long interval, time_t now_second)
{
	const static int CROSS_HELLO_TIME_OUT = 100;

	for (CrossInfoMapIt cross_it = m_cross_info_map.begin(); cross_it != m_cross_info_map.end(); ++cross_it)
	{
		if ((NetID)-1 != cross_it->second.net_id && 0 != cross_it->second.hello_timestamp && now_second >= cross_it->second.hello_timestamp + CROSS_HELLO_TIME_OUT)
		{
			cross_it->second.hello_timestamp = 0;

#ifndef _DEBUG
			this->DisconnectFromCrossServer(cross_it->first);
			gamelog::g_log_world.printf(LL_INFO, "CrossManager::not receive cross hello more than %d s, disconnect cross_netid\n", CROSS_HELLO_TIME_OUT);
#endif
		}
	}

	if (this->IsCrossServerConnected()) // 只要有一个跨服连上了，就执行依赖跨服的相关功能的Update
	{
		if (m_last_print_msg_len_time + 60 < now_second)
		{
			this->LogSendMsgToCrossLen();
			m_last_print_msg_len_time = now_second;
		}
	}
}

bool CrossManager::SendToAllCross(const char *data, int length)
{
	for (CrossInfoMapIt cross_it = m_cross_info_map.begin(); cross_it != m_cross_info_map.end(); ++cross_it)
	{
		if ((NetID)-1 != cross_it->second.net_id)
		{
			InternalComm::Instance().NetSend(cross_it->second.net_id, (const char *)data, length);
		}
	}

	return true;
}

bool CrossManager::SendToCrossWithFuncType(int cross_function_type, const char *data, int length)
{
	if (NULL == data || length <= 0 || length > crossgameprotocal::MAX_CROSSGAMEPROTOCAL_LENGTH) return false;

	this->OnSendMsgToCross(*(int*)data, length);
	return InternalComm::Instance().NetSend(this->GetNetIdWithFuncType(cross_function_type), (const char *)data, length);
}

bool CrossManager::SendToCrossWithCrossActivityType(int cross_activity_type, const char *data, int length)
{
	if (NULL == data || length <= 0 || length > crossgameprotocal::MAX_CROSSGAMEPROTOCAL_LENGTH) return false;

	return this->SendToCrossWithFuncType(cross_activity_type, (const char *)data, length);
}

bool CrossManager::SendToCrossWithCrossRandActivityType(int cross_rand_activity_type, const char *data, int length)
{
	if (NULL == data || length <= 0 || length > crossgameprotocal::MAX_CROSSGAMEPROTOCAL_LENGTH) return false;

	return this->SendToCrossWithFuncType(cross_rand_activity_type, (const char *)data, length);
}

bool CrossManager::UserMsgTransmitToCross(const UserID &user_id, int msg_type, const char *data, int length)
{
	if (length <= 0 || length + sizeof(crossgameprotocal::GameCrossUserMsg) > MAX_MSG_SIZE) return false;

	static const int PLAT_TYPE = LocalConfig::Instance().GetPlatType();
	static char tmp_buffer[MAX_MSG_SIZE];

	int send_len = 0;

	crossgameprotocal::GameCrossUserMsg *global_cross_user_msg = (crossgameprotocal::GameCrossUserMsg *)(tmp_buffer + send_len);
	*global_cross_user_msg = crossgameprotocal::GameCrossUserMsg();
	global_cross_user_msg->uuid = UniqueUserIDToLongLong(UniqueUserID(PLAT_TYPE, user_id));
	global_cross_user_msg->merge_server_id = LocalConfig::Instance().GetMergeServerId();

	send_len += sizeof(crossgameprotocal::GameCrossUserMsg);

	memcpy(tmp_buffer + send_len, data, length);
	send_len += length;

	int func_type = this->GetFuncTypeWithCrossProtocolMsgType(msg_type);
	return this->SendToCrossWithFuncType(func_type, (const char *)tmp_buffer, send_len);
}

bool CrossManager::UserProtocalTransferToGame(int cross_func_type, long long uuid, const char *data, int length)
{
	if (length <= 0 || length + sizeof(crossgameprotocal::GameCrossUserProtocalTransferToGame) > MAX_MSG_SIZE) return false;

	static char tmp_buffer[MAX_MSG_SIZE];

	int send_len = 0;

	crossgameprotocal::GameCrossUserProtocalTransferToGame *gcupttg = (crossgameprotocal::GameCrossUserProtocalTransferToGame *)(tmp_buffer + send_len);
	*gcupttg = crossgameprotocal::GameCrossUserProtocalTransferToGame();
	gcupttg->uuid = uuid;
	gcupttg->protocal_len = length;

	send_len += sizeof(crossgameprotocal::GameCrossUserProtocalTransferToGame);

	memcpy(tmp_buffer + send_len, data, length);
	send_len += length;

	return this->SendToCrossWithFuncType(cross_func_type, (const char *)tmp_buffer, send_len);
}

bool CrossManager::IsCrossHandle(int handle)
{
	for (CrossInfoMapIt it = m_cross_info_map.begin(); it != m_cross_info_map.end(); ++it)
	{
		if (it->second.connet_handle >= 0 && it->second.connet_handle == handle)
		{
			return true;
		}
	}

	return false;
}

bool CrossManager::IsCrossNetID(NetID net_id)
{
	for (CrossInfoMapIt it = m_cross_info_map.begin(); it != m_cross_info_map.end(); ++it)
	{
		if (it->second.net_id >= 0 && it->second.net_id == net_id)
		{
			return true;
		}
	}

	return false;
}

int CrossManager::GetCrossTypeWithNetID(NetID net_id)
{
	CrossInfoItem *info = this->GetCrossInfoWithNetID(net_id);
	if (NULL != info)
	{
		return info->cross_type;
	}

	return 0;
}

CrossInfoItem * CrossManager::GetCrossInfoWithHandle(int handle)
{
	for (CrossInfoMapIt it = m_cross_info_map.begin(); it != m_cross_info_map.end(); ++it)
	{
		if (it->second.connet_handle >= 0 && it->second.connet_handle == handle)
		{
			return &it->second;
		}
	}

	return NULL;
}

CrossInfoItem * CrossManager::GetCrossInfoWithNetID(NetID net_id)
{
	for (CrossInfoMapIt it = m_cross_info_map.begin(); it != m_cross_info_map.end(); ++it)
	{
		if (it->second.net_id >= 0 && it->second.net_id == net_id)
		{
			return &it->second;
		}
	}

	return NULL;
}

CrossInfoItem * CrossManager::GetCrossInfoWithCrossType(int cross_type)
{
	CrossInfoMapIt cross_it = m_cross_info_map.find(cross_type);
	if (cross_it != m_cross_info_map.end())
	{
		return &cross_it->second;
	}

	return NULL;
}

int CrossManager::GetNetIdWithFuncType(int cross_function_type)
{
	int cross_type = CrossConfig::Instance().GetCrossTypeWithCrossFuncType(cross_function_type);

	if (CrossConfig::Instance().IsHiddenServer())
	{
		// 隐藏服，所有跨服功能类型都只传回到自己对应的（也是唯一的）cross进程
		cross_type = CrossConfig::Instance().GetHiddenServerCrossType();
	}

	CrossInfoItem *cross_info = this->GetCrossInfoWithCrossType(cross_type);
	if (NULL != cross_info && NetID(-1) != cross_info->net_id)
	{
		return cross_info->net_id;
	}

	return -1;
}

int CrossManager::GetFuncTypeWithCrossProtocolMsgType(int protocol_msg_type)
{
	using namespace Protocol;

	int func_type = CROSS_FUNCTION_TYPE_COMMON;
	if (protocol_msg_type >= CROSS_SERVER_COMMON_BEGIN && protocol_msg_type <= CROSS_SERVER_COMMON_END)
	{
		func_type = CROSS_FUNCTION_TYPE_COMMON;
	}

	else if (protocol_msg_type >= CROSS_ACTIVITY_1V1_BEGIN && protocol_msg_type < CROSS_ACTIVITY_3V3_BEGIN)
	{
		func_type = CROSS_ACTIVITY_TYPE_CROSS_1V1;
	}

	else if (protocol_msg_type < CROSS_LIEKUN_FB_BEGIN)
	{
		func_type = CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE;
	}

	else if (protocol_msg_type < CROSS_XIULUO_TOWER_BEGIN)
	{
		func_type = CROSS_ACTIVITY_TYPE_LIEKUN_FB;
	}

	else if (protocol_msg_type < CROSS_GUILD_BATTLE_BEGIN)
	{
		func_type = CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER;
	}

	else if (protocol_msg_type < CROSS_TEAM_FB_BEGIN)
	{
		func_type = CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE;
	}

	else if (protocol_msg_type < CROSS_BIANJINGZHIDI_BEGIN)
	{
		func_type = CROSS_ACTIVITY_TYPE_TEAM_FB;
	}

	else if (protocol_msg_type < MT_CROSS_SERVER_MSG_MAX)
	{
		func_type = CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI;
	}

	UNSTD_STATIC_CHECK(MT_CROSS_SERVER_MSG_MAX == 14910); // 跨服协议类型有增加，记得这里加判断
	UNSTD_STATIC_CHECK((int)MT_CROSS_SERVER_MSG_MAX < (int)CROSS_SERVER_MSG_END); // 注意！千万不能超过

	return func_type;
}

void CrossManager::OnSendMsgToCross(int msg_typ, int len)
{
	m_msg_send_to_cross_len[msg_typ] += len;
}

void CrossManager::LogSendMsgToCrossLen()
{
	if (m_msg_send_to_cross_len.empty())
	{
		return;
	}

	std::vector<MsgtypLength> send_info;
	send_info.reserve(m_msg_send_to_cross_len.size());
	for (const auto& value : m_msg_send_to_cross_len)
	{
		send_info.emplace_back(value.first, value.second);
	}

	std::sort(send_info.begin(), send_info.end(), [](const MsgtypLength& a, const MsgtypLength& b) {
		return a.msg_len > b.msg_len;
	});
	for (size_t i = 0; i < 5 && i < send_info.size(); i++)
	{
		gamelog::g_log_serious_error.buff_printf("msg_typ: %d msg_len: %d ", send_info[i].msg_typ, send_info[i].msg_len);
	}
	gamelog::g_log_serious_error.commit_buff(LL_INFO);
	m_msg_send_to_cross_len.clear();
}

void CrossManager::OnGameServerTimeInfo(crossgameprotocal::CrossGameTimeInfoToHidden *cgtith)
{
	if (NULL == cgtith)  return;

	for(int i = 0; i < cgtith->count && i < crossgameprotocal::GameCrossRegisterGameServer::MAX_SERVER_ID_NUM; ++i)
	{
		crossgameprotocal::CrossGameTimeInfoToHidden::UniqueGameSerInfo &msg_info = cgtith->server_info_list[i];

		UniqueGameServerTimeInfo &time_info = m_game_server_time_info_map[msg_info.gs_unique_id];
		time_info.server_start_time = msg_info.server_start_time;
		time_info.server_combine_time = msg_info.server_combine_time;
		time_info.server_start_day_index = msg_info.server_start_day_index;
	}
}

int CrossManager::GetGameServerDayIndex(const UniqueServerID &unique_server_id)
{
	auto it = m_game_server_time_info_map.find(unique_server_id);
	if (it == m_game_server_time_info_map.end()) return 0;

	return it->second.server_start_day_index;
}

unsigned int CrossManager::GetGameServerStartTime(const UniqueServerID &unique_server_id)
{
	auto it = m_game_server_time_info_map.find(unique_server_id);
	if (it == m_game_server_time_info_map.end()) return 0;

	return it->second.server_start_time;
}

unsigned int CrossManager::GetGameServerCombineTime(const UniqueServerID &unique_server_id)
{
	auto it = m_game_server_time_info_map.find(unique_server_id);
	if (it == m_game_server_time_info_map.end()) return 0;

	return it->second.server_combine_time;
}


