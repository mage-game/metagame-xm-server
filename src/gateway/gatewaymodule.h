#pragma once

#ifndef SERVERLOGICMODULE_H
#define SERVERLOGICMODULE_H

#include <limits.h>
#include <vector>

#include "imodule.h"
#include "utility/logagent.h"
#include "irmimodule.h"
#include "inetworkmodule.h"

static const char GATEWAY_MODULE[] = "GatewayModule";

class ServerNetworkCallback;
class ServerInternalNetCallback;
class IClockModule;
class IGameModule;
class ITimerModule;

#pragma pack(push) 
#pragma pack(4)
class GatewayModule : public IModule
{
	friend class ServerNetworkCallback;
	friend class ServerInternalNetCallback;
public:
	GatewayModule(int gateway_index);
	~GatewayModule();

	virtual int Init();
	virtual int Start();
	virtual int Update();
	virtual int Stop();
	virtual int Release();

	virtual void Free();

	void StopServer();

	void OnRecvUserMsg(NetID netid, IP userip, const char *data, int length);
	void OnUserDisconnect(NetID netid);
private:
	bool ConnectToChatServer();
	bool ConnectToGlobalServer();
	bool ConnectToLoginServer();
	bool ListenForUser();

	bool RegisterToLogin();

	LogAgent		m_log;

	IClockModule	*m_clock;
	IGameModule		*m_game;
	INetworkModule	*m_network;
	INetworkModule	*m_internal_network;
	ITimerModule	*m_timer;

	unsigned long	m_current_time;

	ServerNetworkCallback *m_network_callback;
	ServerInternalNetCallback *m_internal_network_callback;

	NetID			m_login_server_id;
	NetID			m_chat_server_id;
	NetID			m_global_server_id;

	void OnRecvLoginServerMsg(const char *data, int length);
	void OnRegisterGS(const char *data);

	struct GS 
	{
		GS():index(-1), port(-1), netid(-1)
		{
		}
		int				index;
		std::string		ip;
		unsigned short	port;
		NetID			netid;
		void Reset()
		{
			index = -1;
			ip = "";
			port = -1;
			netid = -1;
		}
	};
	typedef std::vector<GS>	GSList;
	GSList		m_gs_list;

	void OnRecvGSMsg(const char *data, int length);
	void OnRecvSend(const char *data, int length);
	void OnRecvMulSend(const char *data, int length);
	void OnRecvSendAll(const char *data, int length);
	void OnRecvDisconnect(const char *data, int length);
	void OnHasCheck(const char *data, int length);

	struct User 
	{
		User() :netid(-1), userip(0), gs_index(-1), last_active_time(0), has_checked(false),
			xor_data_len(0), haskey(false) { }
		NetID	netid;
		IP		userip;
		int		gs_index;
		unsigned long last_active_time;
		bool	has_checked;
		char	xor_data[256];
		int		xor_data_len;
		bool    haskey;

		void XorData(char* data, int len)
		{
			if (!haskey)
			{
				return;
			}
			for (int i = 0; i < len; i++)
			{
				data[i] ^= xor_data[i % xor_data_len];
			}
		}

		void SetKey(const char* data, size_t len)
		{
			xor_data_len = (std::min)(static_array_size(xor_data), len);
			if (xor_data_len <= 0)
			{
				return;
			}
			memcpy(xor_data, data, xor_data_len);
			haskey = true;
		}
		void Reset()
		{
			netid = -1;
			gs_index = -1;
			last_active_time = 0;
			has_checked = false;
			xor_data_len = 0;
			haskey = false;
		}
	};
	typedef std::vector<User> UserList;
	UserList	m_user_list;

protected:
	// 超时相关
	void EraseTimeOut();
	bool			m_check_timeout;
	unsigned long	m_last_check_timeout;
	unsigned long	m_check_timeout_interval;
	unsigned long	m_heartbeat_dead_time;

protected:
	char	*m_buffer;
	char	*m_point_to_msg_beg;

public:
	void FlushRecord();
protected:
	// 消息统计相关
	struct MsgRecordItem
	{
		MsgRecordItem():cur_num(0), total_num(0){}
		long long cur_num;
		long long total_num;
	};
	static const int MAX_MSG_TYPE = (int)USHRT_MAX + 1;
	MsgRecordItem m_msg_recorder[MAX_MSG_TYPE];

	LogAgent m_msg_record_log;
	LogAgent m_disconnect_log;
	
	void RecordMsg(unsigned short type, int num);
	
private:
	int m_gateway_index;

	char tmp_buff[MAX_MSG_SIZE];
};

#pragma pack(pop)
#endif
