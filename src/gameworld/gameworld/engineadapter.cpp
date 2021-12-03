
#include "engineadapter.h"
#include "inetworkmodule.h"
#include "servercommon/internalprotocal/wgprotocal.h"
#include "servercommon/servercommon.h"

#include "gamecommon.h"
#include "gamedef.h"

EngineAdapter::EngineAdapter():m_server_index(-1), m_game_thread(-1)
{
	m_buffer = new char[MAX_MSG_SIZE];	// 4M, 不可能有比4M还大的一个网络包，一定够用了
	memset(m_buffer, 0, MAX_MSG_SIZE);
}

EngineAdapter::~EngineAdapter()
{
	delete []m_buffer;
}

void EngineAdapter::Init(ILogModule *log, 
						 IConfigModule *config, 
						 IGameModule *game, 
						 INetworkModule *network, 
						 IClockModule *clock, 
						 IRMIModule *rmi_module,
						 ITimerModule *timer,
						 IFileWriteModule *filewrite)
{
	m_log = log;
	m_config = config;
	m_game = game;
	m_network = network;
	m_clock = clock;
	m_rmi = rmi_module;
	m_time = timer;
	m_filewrite = filewrite;
}

bool EngineAdapter::NetSend(const GSNetID &gsnetid, const char *data, unsigned int length)
{
	if (NULL == data || length > MAX_MSG_SIZE - sizeof(wgprotocal::GWNetSendMsg)) return false;

	++gamecommon::g_game_stat.user_packet_count;

	wgprotocal::GWNetSendMsg *msg = (wgprotocal::GWNetSendMsg*)m_buffer;
	*msg = wgprotocal::GWNetSendMsg();

	msg->header.netid = gsnetid.netid;

	memcpy(m_buffer + sizeof(wgprotocal::GWNetSendMsg), data, length);

	return m_network->Send(gsnetid.gateway_netid, m_buffer, length + sizeof(wgprotocal::GWNetSendMsg));
}

bool EngineAdapter::NetMulSend(GSNetID *netidlist, int num, const char *data, unsigned int length)
{
	if (NULL == data) return false;

	gamecommon::g_game_stat.user_packet_count += num;

	const int MAX_GATEWAY_ID = 256;
	bool gsmark[MAX_GATEWAY_ID];
	unsigned short gslist[MAX_GATEWAY_ID];
	int gsnum = 0;
	memset(gsmark, 0, sizeof(gsmark));

	GSNetID *netid_p = netidlist;
	GSNetID *netid_end = netidlist + num;
	while (netid_p != netid_end)
	{
		unsigned short gwn = netid_p->gateway_netid;
		if (gwn < MAX_GATEWAY_ID && !gsmark[gwn])
		{
			gsmark[gwn] = true;
			gslist[gsnum++] = gwn;
		}
		++netid_p;
	}

	bool ret = true;
	for (int cur = 0; cur < gsnum; ++cur)
	{
		wgprotocal::GWNetMulSendMsg *msg = (wgprotocal::GWNetMulSendMsg*)m_buffer;
		*msg = wgprotocal::GWNetMulSendMsg();
		msg->count = 0;

		unsigned short *beg_netids = (unsigned short*)(m_buffer + sizeof(wgprotocal::GWNetMulSendMsg));

		int gwid = gslist[cur];

		unsigned short *end_netids = beg_netids;
		GSNetID *netid_p = netidlist;
		while (netid_p != netid_end)
		{
			if (netid_p->gateway_netid == gwid)
			{
				*end_netids++ = netid_p->netid;
			}
			++netid_p;
		}
		msg->count = int(end_netids - beg_netids);

		char *data_pos = (char *)end_netids;
		int total_len = int(data_pos - m_buffer) + length;
		if (total_len > MAX_MSG_SIZE)
		{
			ret = false;
			continue;
		}
		memcpy(data_pos, data, length);

		ret &= m_network->Send(gwid, m_buffer, total_len);
	}

	return ret;
}

bool EngineAdapter::NetSendAll(NetID gateway_netid, const char *data, unsigned int length)
{
	if (NULL == data || length > MAX_MSG_SIZE - sizeof(wgprotocal::GWNetSendAll)) return false;

	wgprotocal::GWNetSendAll *msg = (wgprotocal::GWNetSendAll*)m_buffer;
	*msg = wgprotocal::GWNetSendAll();

	memcpy(m_buffer + sizeof(wgprotocal::GWNetSendAll), data, length);

	return m_network->Send(gateway_netid, m_buffer, length + sizeof(wgprotocal::GWNetSendAll));
}

void EngineAdapter::NetDisconnect(const GSNetID &gsnetid, UserID user_id, const GameName role_name, int scene_id, const char *reason)
{
	wgprotocal::GWNetDisconnect ndis;
	ndis.header.netid = gsnetid.netid;
	ndis.uid = UidToInt(user_id);
	ndis.scene_id = scene_id;
	STRNCPY(ndis.role_name, role_name, sizeof(ndis.role_name));
	STRNCPY(ndis.server_name, "GameWorld", sizeof(ndis.server_name));
	STRNCPY(ndis.reason, reason, sizeof(ndis.reason));
	m_network->Send(gsnetid.gateway_netid, (const char *)&ndis, sizeof(wgprotocal::GWNetDisconnect));
}

void EngineAdapter::NetHasCheck(const GSNetID &gsnetid)
{
	wgprotocal::GWHasCheck hc;
	hc.header.netid = gsnetid.netid;

	m_network->Send(gsnetid.gateway_netid, (const char *)&hc, sizeof(wgprotocal::GWHasCheck));
}

