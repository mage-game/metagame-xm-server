#ifndef __CROSS_MANAGER__
#define __CROSS_MANAGER__

#include "inetworkmodule.h"
#include "servercommon/servercommon.h"

#include <map>

struct CrossInfoItem
{
	CrossInfoItem() : cross_type(-1), connet_handle(-1), net_id(-1), hello_timestamp(0), has_start_connect(0), has_connected(0) {}

	int cross_type;
	NetAddress addr;

	int connet_handle;	// 异步连接的回调标志，连接成功后失效
	NetID net_id;		// 连接成功后net_id才有效

	time_t hello_timestamp;
	char has_start_connect;
	char has_connected;
};

// 原服的时间信息
struct UniqueGameServerTimeInfo
{
	UniqueGameServerTimeInfo() : server_start_time(0), server_combine_time(0), server_start_day_index(0) {}

	unsigned int server_start_time;
	unsigned int server_combine_time;
	int server_start_day_index;
};

class CrossManager
{
public:
	static CrossManager & Instance();

	void ConnectToCrossServerAsyn();
	void DisconnectFromCrossServer(int cross_type = 0);		// 主动断开连接跨服

	void OnConnectCrossServerSucc(int handle, NetID netid);
	
	void OnDisconnectFromCross(CrossInfoItem *cross_info);
	void OnDisconnectFromCrossServer(NetID net_id);			// 断开连接时触发
	void OnDisconnectFromCrossServerHandle(int handle);		// 连接失败时触发
	
	void CheckCrossConnection();

	bool ReloadCrossConfig();

	bool IsCrossServerConnected(int cross_type = 0);
	bool IsAllCrossServerConnected();

	void OnCrossHello(NetID net_id);
	void Update(unsigned long interval, time_t now_second);

	bool SendToAllCross(const char *data, int length);
	bool SendToCrossWithFuncType(int cross_function_type, const char *data, int length);
	 bool SendToCrossWithCrossActivityType(int cross_activity_type, const char *data, int length);
	 bool SendToCrossWithCrossRandActivityType(int cross_rand_activity_type, const char *data, int length);

	bool UserMsgTransmitToCross(const UserID &user_id, int msg_type, const char *data, int length);
	bool UserProtocalTransferToGame(int cross_func_type, long long uuid, const char *data, int length);		// uuid为0的时候，把协议发给所有服务器的玩家

	bool IsCrossHandle(int handle);
	bool IsCrossNetID(NetID net_id);
	int GetCrossTypeWithNetID(NetID net_id);

	void OnGameServerTimeInfo(crossgameprotocal::CrossGameTimeInfoToHidden *cgtith);
	int GetGameServerDayIndex(const UniqueServerID &unique_server_id);
	unsigned int GetGameServerStartTime(const UniqueServerID &unique_server_id);
	unsigned int GetGameServerCombineTime(const UniqueServerID &unique_server_id);

private:
	CrossManager();
	~CrossManager();

	bool IsCrossServerConnecting(int cross_type);

	void OnReloadCrossServerConfig(bool is_cross_addr_change, bool old_is_open);

	// 获取跨服信息
	CrossInfoItem * GetCrossInfoWithHandle(int handle);
	CrossInfoItem * GetCrossInfoWithNetID(NetID net_id);
	CrossInfoItem * GetCrossInfoWithCrossType(int cross_type);

	int GetNetIdWithFuncType(int cross_function_type);								// 根据跨服功能类型取得网络号
	int GetFuncTypeWithCrossProtocolMsgType(int protocol_msg_type);					// 根据跨服协议号取得对应的跨服功能类型

	typedef std::map<int, CrossInfoItem> CrossInfoMap;
	typedef std::map<int, CrossInfoItem>::iterator CrossInfoMapIt;
	CrossInfoMap m_cross_info_map;


	////////////// 隐藏服存储 ////////////////////////////////////////////////////////////

	std::map<UniqueServerID, UniqueGameServerTimeInfo> m_game_server_time_info_map; // 原服开服时间信息

private:
	void OnSendMsgToCross(int msg_typ, int len);
	void LogSendMsgToCrossLen();
private:
	struct MsgtypLength
	{
		MsgtypLength(int typ, int len) :msg_typ(typ), msg_len(len) {}
		int msg_typ = 0;
		int msg_len = 0;
	};
	//msg_type, len
	std::map<int, int> m_msg_send_to_cross_len;
	time_t m_last_print_msg_len_time;

};

#endif

