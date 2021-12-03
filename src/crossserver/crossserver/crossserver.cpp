#include "crossserver.h"

#include "ilogmodule.h"
#include "engineadapter.h"
#include "internalcomm.h"

#include "servercommon/servercommon.h"

#include "crossservermodule.h"

#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"

#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/userprotocal/msgsystem.h"

#include "servercommon/serverconfig/configstruct.h"
#include "crossuser/crossuser.h"

#include "servercommon/userprotocal/crossmsgcode.h"
#include "protocal/msgcross.h"

static CrossServer *crossserver_inst = NULL;

CrossServer::CrossServer() 
	: m_cross_server_module(NULL), m_game_time(0), m_is_all_data_load_finish(false), m_last_update_dayid(0), m_last_send_hello_timestamp(0), 
	m_hidden_gs_netid(-1), m_hidden_gs_role_all_param_size(0)
{
	crossserver_inst = this;

	m_cross_status.SetCrossServer(this);
	m_cross_user_manager.SetCrossServer(this);
	m_cross_operate_register.SetCrossServer(this);
	m_cross_activity_manager.SetCrossServer(this);
	m_cross_person_rank.SetCrossServer(this);
	m_cross_couple_rank.SetCrossServer(this);
	m_cross_rand_activity_manager.SetCrossServer(this);

	memset(m_on_gs_handler_list, 0, sizeof(m_on_gs_handler_list));
	this->InitGSHandler();
}

CrossServer::~CrossServer()
{

}

CrossServer * CrossServer::GetInstCrossServer()
{
	return crossserver_inst;
}

bool CrossServer::Init(CrossServerModule *cross_server_module, ILogModule *log_module)
{
	if (NULL == cross_server_module || NULL == log_module) return false;

	m_cross_server_module = cross_server_module;

	m_log.SetLogModule(log_module);
	m_log.SetCatagory(CROSSSERVER_MODULE);
	
	m_game_time = EngineAdapter::Instance().GetGameTime();

	return true;
}

void CrossServer::Start()
{
	m_cross_status.OnServerStart();
	m_cross_user_manager.OnServerStart();
	m_cross_activity_manager.OnServerStart();
	m_cross_person_rank.OnServerStart();
	m_cross_couple_rank.OnServerStart();
	m_cross_rand_activity_manager.OnServerStart();
}

void CrossServer::Update()
{
	unsigned long now = EngineAdapter::Instance().GetGameTime();
	time_t now_second = EngineAdapter::Instance().Time();
	unsigned int now_dayid = EngineAdapter::Instance().DayID();

	unsigned long interval = now - m_game_time;

	m_cross_status.Update(interval, now_second);
	m_cross_user_manager.Update(interval, now_second);
	m_cross_operate_register.Update(interval, now_second);
	m_cross_activity_manager.Update(interval, now_second);
	m_cross_person_rank.Update(interval, now_second);
	m_cross_rand_activity_manager.Update(interval, now_second);

	m_game_time = now;

	if (!m_is_all_data_load_finish)
	{
		if (this->IsAllDataLoadFinish())
		{
			m_is_all_data_load_finish = true;
			m_last_update_dayid = m_cross_status.GetLastUpdateDayID();

			this->OnAllDataLoadFinish();
		}
	}
	else if (m_last_update_dayid != now_dayid)
	{
		this->OnDayChange(m_last_update_dayid, now_dayid);

		m_last_update_dayid = now_dayid;
		m_cross_status.SetLastUpdateDayID(m_last_update_dayid);
	}

	{
		if (now_second >= m_last_send_hello_timestamp + 2)
		{
			m_last_send_hello_timestamp = now_second;

			static crossgameprotocal::CrossGameHello cgh;
			this->SendToAllGameWorld((const char *)&cgh, sizeof(cgh));
		}
	}
}

bool CrossServer::IsAllDataLoadFinish()
{
	return m_cross_status.IsLoadFinish() && 
		m_cross_activity_manager.IsLoadFinish() &&
		m_cross_person_rank.IsLoadFinish()&&
		m_cross_rand_activity_manager.IsLoadFinish();
}

void CrossServer::OnAllDataLoadFinish()
{
	if (!m_cross_server_module->ListenForGlobal())
	{
		m_cross_server_module->StopServer();
	}
}

void CrossServer::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;

	crossgameprotocal::CrossGameTimeInfoToHidden cgtith; // 发送原服时间给隐藏服
	cgtith.count = 0;

	for (GameServerInfoMapIt it = m_game_server_info_map.begin(); it != m_game_server_info_map.end(); it++)
	{
		GameServerInfo &info = it->second;
		info.server_start_day_index = GetDayIndex(info.server_start_time, EngineAdapter::Instance().Time());

		if (cgtith.count < crossgameprotocal::GameCrossRegisterGameServer::MAX_SERVER_ID_NUM)
		{
			cgtith.server_info_list[cgtith.count].gs_unique_id = info.gs_unique_id;
			cgtith.server_info_list[cgtith.count].server_start_time = info.server_start_time;
			cgtith.server_info_list[cgtith.count].server_combine_time = info.server_combine_time;
			cgtith.server_info_list[cgtith.count].server_start_day_index = info.server_start_day_index;
			cgtith.count++;
		}
	}

	// send to hidden, time info
	int send_info_len = sizeof(cgtith) - sizeof(cgtith.server_info_list[0]) * (crossgameprotocal::GameCrossRegisterGameServer::MAX_SERVER_ID_NUM - cgtith.count);
	this->SendToHiddenGameWorld((const char *)&cgtith, send_info_len);

	m_cross_activity_manager.OnDayChange();
	m_cross_person_rank.OnDayChange();
	m_cross_couple_rank.OnDayChange();
	m_cross_rand_activity_manager.OnDayChange();

	const tm *tm_now = EngineAdapter::Instance().LocalTime();
	if (NULL != tm_now && 1 == tm_now->tm_wday)
	{
		this->OnWeekChange();
	}
}

void CrossServer::OnWeekChange()
{
	m_cross_activity_manager.OnWeekChange();
	m_cross_person_rank.OnWeekChange();
	m_cross_rand_activity_manager.OnWeekChange();
}

void CrossServer::OnInternalGameServerAccept(Port listen_port, NetID netid, IP ip, Port port)
{
	printf("\n[DEBUG] OnInternalGameServerAccept, port = %d, netid = %d\n", listen_port, netid);

	if (m_game_server_netid_list.size() <= netid)
	{
		int old_size = (int)m_game_server_netid_list.size();

		m_game_server_netid_list.resize(netid + 1);
		for (int i = old_size; i < (int)m_game_server_netid_list.size(); ++i)
		{
			m_game_server_netid_list[i] = -1;
		}
	}

	m_game_server_netid_list[netid] = netid;
}

void CrossServer::OnInternalRecv(NetID netid, const char *data, int length)
{
	// printf("[DEBUG] OnInternalRecv, netid = %d\n", netid);

	if (netid >= m_game_server_netid_list.size() || (NetID)-1 == m_game_server_netid_list[netid])
	{
		return;
	}

	crossgameprotocal::MessageHeader *header = (crossgameprotocal::MessageHeader *)data;

	if (netid != m_hidden_gs_netid)
	{
		// 拦截
		/*
		if (crossglobalprotocal::MT_SAVE_CROSS_ROLE_RET == header->msg_type || 
			crossglobalprotocal::MT_CROSS_USER_LOGIN == header->msg_type || 
			crossglobalprotocal::MT_CROSS_USER_LOGOUT == header->msg_type || 
			crossglobalprotocal::MT_CROSS_USER_FIRST_ENTER_SCENE == header->msg_type)
		{
			return;
		}
		*/
	}

	int msg_type_index = header->msg_type - crossgameprotocal::MT_CROSS_GAME_BEGIN;
	if (msg_type_index < 0 || msg_type_index >= (int)sizeof(m_on_gs_handler_list) / (int)sizeof(m_on_gs_handler_list[0]) ||
		NULL == m_on_gs_handler_list[msg_type_index])
	{
		m_log.printf(LL_DEBUG, "OnInternalRecv unknow msg_type %d.", (int)header->msg_type);
		return;
	}

	(this->*(m_on_gs_handler_list[msg_type_index]))(netid, data, length);
}

void CrossServer::OnInternalDisconnect(NetID netid)
{
	printf("[DEBUG] OnInternalDisconnect, netid = %d\n", netid);

	if (netid < m_game_server_netid_list.size())
	{
		m_game_server_netid_list[netid] = -1;

		for (GameServerInfoMapIt it = m_game_server_info_map.begin(); it != m_game_server_info_map.end();)
		{
			if (it->second.netid == netid)
			{
				m_cross_user_manager.OnInternalGSDisconnnect(it->first);
				m_cross_activity_manager.OnInternalGSDisconnnect(netid, it->first);
				m_game_server_info_map.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}

	if (netid == m_hidden_gs_netid)	
	{
		this->OnHiddenGlobalDisconnect();

		m_hidden_gs_unique_id = INVALID_UNIQUE_SERVER_ID;
		m_hidden_gs_netid = -1;

		printf("[DEBUG] Hidden Global disconnect!\n");
	}
}

void CrossServer::OnHiddenGlobalDisconnect()
{
	m_cross_user_manager.OnHiddenGlobalDisconnect();

	m_cross_status.OnServerStop();				// 隐藏服断线时保存数据
	m_cross_user_manager.OnServerStop();
	m_cross_activity_manager.OnServerStop();
	m_cross_person_rank.OnServerStop();
	m_cross_rand_activity_manager.OnServerStop();
}

void CrossServer::StopServer()
{
	m_cross_server_module->StopServer();
}

NetID CrossServer::GetGameServerNetID(const UniqueServerID &unique_server_id)
{
	GameServerInfoMapIt it = m_game_server_info_map.find(unique_server_id);
	if (it == m_game_server_info_map.end()) return (NetID)-1;

	return it->second.netid;
}

NetID CrossServer::GetGameServerNetID(const UniqueUserID &uniqe_user_id)
{
	return this->GetGameServerNetID(UniqueServerID(uniqe_user_id.plat_type, uniqe_user_id.user_id.db_index));
}

bool CrossServer::SendToUser(NetID global_netid, const UserID &user_id, const char *data, int length)
{
	if (NULL == data || length <= 0 || length > MAX_MSG_SIZE) return false;

	static char msg_buff[sizeof(crossgameprotocal::CrossGameUserMsg) + MAX_MSG_SIZE];

	int send_len = 0;

	crossgameprotocal::CrossGameUserMsg *cross_user_msg = (crossgameprotocal::CrossGameUserMsg *)(msg_buff + send_len);

	*cross_user_msg = crossgameprotocal::CrossGameUserMsg();
	cross_user_msg->uid = UidToInt(user_id);
	send_len += sizeof(crossgameprotocal::CrossGameUserMsg);

	memcpy(msg_buff + send_len, data, length);
	send_len += length;

	return InternalComm::Instance().NetSend(global_netid, msg_buff, send_len);
}

bool CrossServer::SendToUser(const UniqueUserID &unique_user_id, const char *data, int length)
{
	NetID global_netid = this->GetGameServerNetID(unique_user_id);
	if ((NetID)-1 == global_netid) return false;

	return this->SendToUser(global_netid, unique_user_id.user_id, data, length);
}

bool CrossServer::SendToCrossUser(const UserID &cross_user_id, const char *data, int length)
{
	if (!IsCrossUser(cross_user_id)) return false;

	return this->SendToUser(m_hidden_gs_netid, cross_user_id, data, length);
}

bool CrossServer::SendToCrossUser(const UniqueUserID &original_unique_user_id, const char *data, int length)
{
	CrossUser *cross_user = m_cross_user_manager.GetCrossUser(original_unique_user_id);
	if (NULL != cross_user)
	{
		return this->SendToCrossUser(cross_user->GetCrossUserID(), data, length);
	}

	return false;
}

bool CrossServer::SendToAllUser(bool is_to_all_global, NetID global_netid, const char *data, int length, int cross_activity_type, bool is_shield_new_server, UniqueServerID new_server_id)
{
	if (NULL == data || length <= 0 || length > MAX_MSG_SIZE) return false;

	static char msg_buff[sizeof(crossgameprotocal::CrossGameAllUserMsg) + MAX_MSG_SIZE];

	int send_len = 0;

	crossgameprotocal::CrossGameAllUserMsg *cross_global_user_msg = (crossgameprotocal::CrossGameAllUserMsg *)(msg_buff + send_len);
	*cross_global_user_msg = crossgameprotocal::CrossGameAllUserMsg();
	cross_global_user_msg->cross_activity_type = cross_activity_type;;
	send_len += sizeof(crossgameprotocal::CrossGameAllUserMsg);

	memcpy(msg_buff + send_len, data, length);
	send_len += length;

	if (is_to_all_global)
	{
		if (is_shield_new_server)
		{
			return this->SendToAllGameWorldShieldNewServer(msg_buff, send_len, -1, new_server_id);
		}
		else
		{
			return this->SendToAllGameWorld(msg_buff, send_len);
		}
	}
	else
	{
		return InternalComm::Instance().NetSend(global_netid, msg_buff, send_len);
	}
}

bool CrossServer::SendToAllCrossUser(const char *data, int length, int cross_activity_type)
{
	return this->SendToAllUser(false, m_hidden_gs_netid, data, length, cross_activity_type);
}

bool CrossServer::SendToGameWorld(NetID global_netid, const char *data, int length)
{
	if (NULL == data || length <= 0 || length > crossgameprotocal::MAX_CROSSGAMEPROTOCAL_LENGTH) return false;

	return InternalComm::Instance().NetSend(global_netid, data, length);
}

bool CrossServer::SendToGameWorld(const UniqueUserID &unique_user_id, const char *data, int length)
{
	NetID gs_netid = this->GetGameServerNetID(unique_user_id);
	if ((NetID)-1 == gs_netid) return false;

	return this->SendToGameWorld(gs_netid, data, length);
}

bool CrossServer::SendToGameWorld(const UniqueServerID &unique_server_id, const char *data, int length)
{
	NetID gs_netid = this->GetGameServerNetID(unique_server_id);
	if ((NetID)-1 == gs_netid) return false;

	return this->SendToGameWorld(gs_netid, data, length);
}

bool CrossServer::SendToHiddenGameWorld(const char *data, int length)
{
	return this->SendToGameWorld(m_hidden_gs_netid, data, length);
}

bool CrossServer::SendToAllGameWorld(const char *data, int length, NetID except_global_netid)
{
	bool ret = true;

	for (GameServerNetIDListIt it = m_game_server_netid_list.begin(), end = m_game_server_netid_list.end(); it != end; ++ it)
	{
		if (*it != (NetID)-1)
		{
			if (*it != except_global_netid)
			{
				ret = ret && InternalComm::Instance().NetSend(*it, data, length);
			}
		}
	}

	return ret;
}

bool CrossServer::SendToAllGameWorldShieldNewServer(const char *data, int length, NetID except_global_netid, UniqueServerID new_server_id)
{
	if (new_server_id == INVALID_UNIQUE_SERVER_ID)
	{
		return false;
	}

	GameServerInfoMapIt cur_server = m_game_server_info_map.find(new_server_id);
	if (cur_server == m_game_server_info_map.end())
	{
		return false;
	}

	bool is_new_server = false;
	if (cur_server->second.server_start_day_index < 7)
	{
		is_new_server = true;
	}

	bool ret = false;
	GameServerInfoMapIt it = m_game_server_info_map.begin();
	for(; it != m_game_server_info_map.end(); it++)
	{
		GameServerInfo &server_info = it->second;
		if (is_new_server)
		{
			if (new_server_id != server_info.gs_unique_id)
			{
				continue;
			}
		}
		else
		{
			if (server_info.server_start_day_index < 7)
			{
				continue;
			}
		}

		if (server_info.netid == except_global_netid)
		{
			continue;
		}

		ret = InternalComm::Instance().NetSend(server_info.netid, data, length);
	}
	
	return ret;
}

void CrossServer::NoticeNum(NetID global_netid, const UserID &user_id, int notice_num)
{
	Protocol::SCNoticeNum nn;
	nn.notice_num = notice_num;
	this->SendToUser(global_netid, user_id, (const char *)&nn, sizeof(nn));
}

void CrossServer::NoticeNumToCrossUser(const UserID &user_id, int notice_num)
{
	this->NoticeNum(m_hidden_gs_netid, user_id, notice_num);
}

bool CrossServer::SystemMsgToAllUser(bool is_to_all_global, NetID global_netid, const char *msg, int length, int msg_type, int pos, int color, bool is_shield_new_server, UniqueServerID new_server_id)
{
	if (NULL == msg || length < 0 || length > PROTOCAL_MAX_SYSTEM_MSG_LENGTH) return false;

	static Protocol::SCSystemMsg sm;
	sm.send_time = (unsigned int)EngineAdapter::Instance().Time();
	sm.msg_type = (short)msg_type;
	sm.msg_length = (short)length;
	sm.display_pos = pos;
	sm.color = color;
	memcpy(sm.msg, msg, length);

	int sendlen = sizeof(Protocol::SCSystemMsg) - PROTOCAL_MAX_SYSTEM_MSG_LENGTH + length;
	return this->SendToAllUser(is_to_all_global, global_netid, (const char *)&sm, sendlen, CROSS_ACTIVITY_TYPE_INVALID, is_shield_new_server, new_server_id);
}

bool CrossServer::SystemMsgToAllHiddenGlobalUser(const char *msg, int length, int msg_type, int pos, int color)
{
	return this->SystemMsgToAllUser(false, m_hidden_gs_netid, msg, length, msg_type, pos, color);
}

bool CrossServer::SystemMsgToAllGlobalUser(const char *msg, int length, int msg_type, int pos, int color, bool is_shield_new_server, UniqueServerID new_server_id)
{
	return this->SystemMsgToAllUser(true, -1, msg, length, msg_type, pos, color, is_shield_new_server, new_server_id);
}

void CrossServer::KickAllCrossUser(int cross_activity_type)
{
	crossglobalprotocal::CrossGlobalKickAllCrossUser cgkacu;
	cgkacu.cross_activity_type = cross_activity_type;
	this->SendToHiddenGameWorld((const char *)&cgkacu, sizeof(cgkacu));
}

bool CrossServer::IsUserLocateServerConnect(const UniqueUserID &uniqe_user_id)
{
	NetID netid = this->GetGameServerNetID(uniqe_user_id);
	return ((NetID)-1 != netid);
}

int CrossServer::GetGameServerDayIndex(const UniqueServerID &unique_server_id)
{
	GameServerInfoMapIt it = m_game_server_info_map.find(unique_server_id);
	if (it == m_game_server_info_map.end()) return 0;

	return it->second.server_start_day_index;
}

unsigned int CrossServer::GetGameServerStartTime(const UniqueServerID &unique_server_id)
{
	GameServerInfoMapIt it = m_game_server_info_map.find(unique_server_id);
	if (it == m_game_server_info_map.end()) return 0;

	return it->second.server_start_time;
}

unsigned int CrossServer::GetGameServerCombineTime(const UniqueServerID &unique_server_id)
{
	GameServerInfoMapIt it = m_game_server_info_map.find(unique_server_id);
	if (it == m_game_server_info_map.end()) return 0;

	return it->second.server_combine_time;
}

void CrossServer::OnTransferToGlobal(NetID global_netid, const char *data, int length)
{
	crossglobalprotocal::GlobalCrossTransferToGlobal *trans_to_global = (crossglobalprotocal::GlobalCrossTransferToGlobal *)data;

	this->SendToGameWorld(UniqueServerID(trans_to_global->target_plat_type, trans_to_global->target_server_id), data + sizeof(crossglobalprotocal::GlobalCrossTransferToGlobal), length - sizeof(crossglobalprotocal::GlobalCrossTransferToGlobal));
}

void CrossServer::OnTransferToAllGlobal(NetID global_netid, const char *data, int length)
{
	crossglobalprotocal::GlobalCrossTransferToAllGlobal *trans_to_all_global = (crossglobalprotocal::GlobalCrossTransferToAllGlobal *)data;

	this->SendToAllGameWorld(data + sizeof(crossglobalprotocal::GlobalCrossTransferToAllGlobal), length - sizeof(crossglobalprotocal::GlobalCrossTransferToAllGlobal), (0 == trans_to_all_global->is_send_back ? global_netid: (NetID)-1));
}

void CrossServer::OnGlobalCrossFixPlatTypeCheckUserAck(NetID global_netid, const char *data, int length)
{
	crossglobalprotocal::GlobalCrossFixPlatTypeCheckUserAck *gcfptcua = (crossglobalprotocal::GlobalCrossFixPlatTypeCheckUserAck *)data;
	if (gcfptcua->tofix_uuid <= 0 || gcfptcua->real_uuid <= 0 ) return;

	UniqueUserID tofix_unique_user_id = LongLongToUniqueUserID(gcfptcua->tofix_uuid);
	UniqueUserID real_unique_user_id = LongLongToUniqueUserID(gcfptcua->real_uuid);

	if (tofix_unique_user_id.user_id != real_unique_user_id.user_id) return;

	//m_cross_activity_manager.GetCrossActivityCrossChallenge()->OnFixPlatTypeCheckUserAck(gcfptcua->fix_type, gcfptcua->fix_param, tofix_unique_user_id, real_unique_user_id);
}

void CrossServer::OnGetCrossPersonRankList(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len)
{
	Protocol::CSCrossGetPersonRankList *gcprl = (Protocol::CSCrossGetPersonRankList *)msg;

	if (gcprl->rank_type < CROSS_COUPLE_RANK_TYPE_BEGIN)
	{
		m_cross_person_rank.OnGetRankList(global_netid, unique_user_id, gcprl->rank_type);
	}
	else
	{
		m_cross_couple_rank.OnGetCoupleRankList(global_netid, unique_user_id, gcprl->rank_type - CROSS_COUPLE_RANK_TYPE_BEGIN);
	}
}


void CrossServer::MailToGameWorldUser(const UniqueUserID &uniqe_user_id, const MailContentParam &content_param)
{
	static crossgameprotocal::CrossGameSendMailToUser cgffhn;
	cgffhn.user_id = UidToInt(uniqe_user_id.user_id);
	cgffhn.contentparam = content_param;

	this->SendToGameWorld(uniqe_user_id, (const char *)&cgffhn, sizeof(cgffhn));
}