#include "protocal/msglogin.h"
#include "loginserver.h"
#include "protocal/msgregister.h"
#include "servercommon/servercommon.h"
#include "engineadapter.h"
#include "internalcomm.h"
#include "rmibackobjdef.h"
#include "loginlog.h"
#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "platform/platform.h"

#include "loginusertrace.hpp"

#include "mergeloginmanager.h"

OLD_USE_INTERNAL_NETWORK();

void RMIGetRoleInfoBackObjectImpl::GetRoleInfoRet(int ret, const RoleInfoList &role_info)
{
	if (0 != ret) result = Protocol::LOGIN_RESULT_NO_ROLE;

	MergeLoginManager::Instance().AddRoleInfoList(netid, merge_key, result, &role_info, original_panme, db_index);

	if (LoginUserTrace::Instance().IsTrace(pname))
	{
		loginlog::g_log_login_trace.printf(LL_INFO, "[GetRoleInfoRet] [plat_name:%s] ret:%d role_count::%d", pname, ret, role_info.count);
	}
}

void RMIUserLoginBackObjectImpl::UserLoginRet(int ret, int db_index, int role_1, int role_2, int role_3)
{
	int result = Protocol::LOGIN_RESULT_SUC;

	int role_id_list[16] = {0};
	int role_count = 0;
	if (role_1 != 0) role_id_list[role_count++] = role_1;
	if (role_2 != 0) role_id_list[role_count++] = role_2;
	if (role_3 != 0) role_id_list[role_count++] = role_3;

	if (0 == ret)
	{
		if (0 == role_count)
		{
			result = Protocol::LOGIN_RESULT_NO_ROLE;					// 无角色

			if (LoginUserTrace::Instance().IsTrace(pname))
			{
				loginlog::g_log_login_trace.printf(LL_INFO, "[RMIUserLoginBackObjectImpl no_role] [plat_name:%s] ret:%d db_index:%d role_id:%d", pname, ret, db_index, role_1);
			}
		}
	}
	else
	{
		if (ret == -2)
		{
			result = Protocol::LOGIN_LOGIN_FORBID;						// 禁止登录

			if (LoginUserTrace::Instance().IsTrace(pname))
			{
				loginlog::g_log_login_trace.printf(LL_INFO, "[RMIUserLoginBackObjectImpl forbid] [plat_name:%s] ret:%d db_index:%d role_id:%d ", pname, ret, db_index, role_1);
			}
		}
		else
		{
			result = Protocol::LOGIN_SERVER_ERROR;						// db加载出错

			if (LoginUserTrace::Instance().IsTrace(pname))
			{
				loginlog::g_log_login_trace.printf(LL_INFO, "[RMIUserLoginBackObjectImpl error] [plat_name:%s] ret:%d db_index:%d role_id:%d ", pname, ret, db_index, role_1);
			}
		}
	}

	if (0 == role_count)
	{
		MergeLoginManager::Instance().AddRoleInfoList(netid, merge_key, result, NULL, original_panme, db_index);
		return;
	}

	RMIGetRoleInfoBackObjectImpl *gribo = new RMIGetRoleInfoBackObjectImpl();
	gribo->db_index = db_index;
	gribo->netid = netid;
	gribo->result = result;
	gribo->merge_key = merge_key;
	F_STRNCPY(gribo->pname, pname, sizeof(pname));
	F_STRNCPY(gribo->original_panme, original_panme, sizeof(gribo->original_panme));

	RMILRoleClient lc;
	lc.__bind_session(InternalComm::Instance().GetDBHandler().GetRoleDB());
	lc.GetRoleInfoAsyn(role_id_list, role_count, gribo);

	if (LoginUserTrace::Instance().IsTrace(pname))
	{
		loginlog::g_log_login_trace.printf(LL_INFO, "[RMIUserLoginBackObjectImpl] [plat_name:%s] ret:%d db_index:%d role_id:%d", pname, ret, db_index, role_1);
	}
}

void LoginServer::OnLoginReq(NetID netid, const char *data)							// 帐号登录 
{
	Protocol::CSLogin *scl = (Protocol::CSLogin *)data;
	scl->pname[sizeof(PlatName) - 1] = 0;

	static PlatName original_panme = {0};
	F_STRNCPY(original_panme, scl->pname, sizeof(original_panme));

	if (!m_platform->CheckAccount(netid, scl->pname, scl->login_time, scl->login_str, scl->server)) return;

	MergeLoginManager::Instance().OnLoginReq(netid, original_panme, scl->server);	

	{
		// 发送职业人数信息
		Protocol::SCProfNumInfo pni;
		this->GetProfNum(&pni.prof1_num, &pni.prof2_num, &pni.prof3_num, &pni.prof4_num);
		EngineAdapter::Instance().NetSend(netid, (const char*)&pni, sizeof(pni));
	}
}

void RMIGetRoleInfoBackObjectImplRoleReq::GetRoleInfoRet(int ret, const RoleInfoList &role_info_list)
{
	Protocol::SCLoginAck la;
	la.is_merged_server = LocalConfig::Instance().IsMergedServer() ? 1 : 0;

	do 
	{
		if (0 != ret)
		{
			la.result = Protocol::LOGIN_RESULT_NO_ROLE;
			break;
		}

		int role_id = UidToInt(user_id);

		int role_index = -1;
		for (int i = 0; i < role_info_list.count && i < RoleInfoList::MAX_ROLE_NUM; i++)
		{
			if (role_info_list.role_info_list[i].role_id == role_id)
			{
				role_index = i;
				break;
			}
		}

		if (role_index < 0)
		{
			la.result = Protocol::LOGIN_RESULT_NO_ROLE;
			break;
		}

		const RoleInfoList::__roleinfo &role_info = role_info_list.role_info_list[role_index];
		GameServer *gs = LoginServer::Instance().GetGSServer(role_info.scene_id);
		if (NULL == gs)
		{
			la.result = Protocol::LOGIN_SCENE_NOT_EXIST;
			break;
		}

		LoginServer::GateWayServer *gw = LoginServer::Instance().RandomGatewayServer();
		if (NULL == gw)
		{
			la.result = Protocol::LOGIN_RESULT_NO_GATEWAY;
			break;
		}

		unsigned int time = (unsigned int)(EngineAdapter::Instance().Time());

		GameName role_name;
		F_STRNCPY(role_name, role_info.role_name, sizeof(GameName));

		GetSessionKey(InternalComm::Instance().m_session_key_base, role_info.scene_id, 0, 
			role_info.last_scene_id, role_id, role_name, time, 1, pname, la.key);

		la.scene_id = role_info.scene_id;
		la.last_scene_id = role_info.last_scene_id;
		la.uid = role_id;
		la.time = time;
		la.result = Protocol::LOGIN_RESULT_SUC;
		STRNCPY(la.gs_hostname, gw->hostname.c_str(), sizeof(la.gs_hostname));
		la.gs_port = gw->port;
		la.gs_index = gs->index;
		la.server_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

		EngineAdapter::Instance().NetSend(netid, (const char*)&la, sizeof(Protocol::SCLoginAck));

		loginlog::g_log_login.printf(LL_INFO, "[RMIGetRoleInfoBackObjectImplRoleReq Success] pname:%s uid[%d] sid:%d last_sid:%d gw:%s:%d gs_index:%d.",
			pname, UidToInt(user_id), la.scene_id, la.last_scene_id, la.gs_hostname, (int)la.gs_port, la.gs_index);

		if (LoginUserTrace::Instance().IsTrace(pname))
		{
			loginlog::g_log_login_trace.printf(LL_INFO, "[GetRoleInfoRet] [plat_name:%s] result:%d", pname, la.result);
		}

		return;
	} while (0);

	la.scene_id = 0;
	la.last_scene_id = 0;
	la.uid = 0;
	la.time = 0;
	memset(la.key, 0, sizeof(SessionKey));
	memset(la.gs_hostname, 0, sizeof(la.gs_hostname));
	la.gs_port = 0;
	la.gs_index = 0;
	EngineAdapter::Instance().NetSend(netid, (const char*)&la, sizeof(Protocol::SCLoginAck));

	loginlog::g_log_login.printf(LL_WARNING, "[RMIGetRoleInfoBackObjectImplRoleReq return result:%d] pname:%s uid[%d, %d].",
							la.result, pname, user_id.db_index, user_id.role_id);

	if (LoginUserTrace::Instance().IsTrace(pname))
	{
		loginlog::g_log_login_trace.printf(LL_INFO, "[GetRoleInfoRet] [plat_name:%s] result:%d", pname, la.result);
	}
}

void RMIUserLoginBackObjectImplRole::UserLoginRet(int ret, int db_index, int role_1, int role_2, int role_3)
{
	if (ret == 0)
	{
		if (db_index != user_id.db_index || (role_1 != UidToInt(user_id) && role_2 != UidToInt(user_id) && role_3 != UidToInt(user_id)))
		{
			loginlog::g_log_login.printf(LL_WARNING, "[RMIUserLoginBackObjectImplRole UserIDError] pname:%s uid[%d, %d] game_thread:%d.", 
									pname, user_id.db_index, user_id.role_id, game_thread);

			if (LoginUserTrace::Instance().IsTrace(pname))
			{
				loginlog::g_log_login_trace.printf(LL_INFO, "[RMIUserLoginBackObjectImplRole error1] [plat_name:%s] ret:%d db_index:%d role_id:%d", pname, ret, db_index, role_1);
			}

			return;
		}

		RMIGetRoleInfoBackObjectImplRoleReq *gribo = new RMIGetRoleInfoBackObjectImplRoleReq();
		gribo->user_id = user_id;
		gribo->ip = ip;
		gribo->netid = netid;
		gribo->login_server = login_server;
		F_STRNCPY(gribo->pname, pname, sizeof(gribo->pname));
		gribo->min_login_interval_time = min_login_interval_time;
	
		int role_id_list = UidToInt(user_id);

		RMILRoleClient lc;
		lc.__bind_session(InternalComm::Instance().GetDBHandler().GetRoleDB());
		bool result = lc.GetRoleInfoAsyn(&role_id_list, 1, gribo);

		if (!result)
		{
			loginlog::g_log_login.printf(LL_ERROR, "[RMIUserLoginBackObjectImplRole RMIError] pname:%s uid[%d, %d] game_thread:%d.", 
					pname, user_id.db_index, user_id.role_id, game_thread);
		}
	}
	else 
	{	
		Protocol::SCLoginAck la;		
		la.is_merged_server = 0;
		la.result = Protocol::LOGIN_SERVER_ERROR;			// 服务器发生错误
		la.scene_id = 0;
		la.last_scene_id = 0;
		la.uid = 0;
		la.time = 0;
		memset(la.key, 0, sizeof(SessionKey));
		memset(la.gs_hostname, 0, sizeof(la.gs_hostname));
		la.gs_port = 0;
		la.gs_index = 0;
		la.server_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		EngineAdapter::Instance().NetSend(netid, (const char*)&la, sizeof(Protocol::SCLoginAck));

		if (LoginUserTrace::Instance().IsTrace(pname))
		{
			loginlog::g_log_login_trace.printf(LL_INFO, "[RMIUserLoginBackObjectImplRole error2] [plat_name:%s] ret:%d db_index:%d role_id:%d", pname, ret, db_index, role_1);
		}

		return;
	}

	if (LoginUserTrace::Instance().IsTrace(pname))
	{
		loginlog::g_log_login_trace.printf(LL_INFO, "[RMIUserLoginBackObjectImplRole] [plat_name:%s] ret:%d db_index:%d role_id:%d", pname, ret, db_index, role_1);
	}
}

void LoginServer::OnRoleReq(IP ip, NetID netid, const char *data)
{
	Protocol::CSRoleReq *rr = (Protocol::CSRoleReq*)data;
	rr->pname[sizeof(PlatName) - 1] = 0;

	static PlatName original_panme = {0};
	F_STRNCPY(original_panme, rr->pname, sizeof(original_panme));

	if (!m_platform->CheckAccount(netid, rr->pname, rr->login_time, rr->login_str, rr->server)) return ;

	UserID user_id = IntToUid(rr->role_id);

	if (!LocalConfig::Instance().IsServerIdAllowed(user_id.db_index)) return;

	static PlatName real_panme = {0};
	F_STRNCPY(real_panme, original_panme, sizeof(real_panme));
	PlatNameCatPostfix(real_panme, user_id.db_index);

	if (LoginUserTrace::Instance().IsTrace(real_panme))
	{
		loginlog::g_log_login_trace.printf(LL_INFO, "[OnRoleReq] [plat_name:%s]", real_panme);
	}

	RMIUserLoginBackObjectImplRole *gribo = new RMIUserLoginBackObjectImplRole();
	gribo->user_id = user_id;
	gribo->netid = netid;
	gribo->ip = ip;
	gribo->login_server = this;
	F_STRNCPY(gribo->pname, real_panme, sizeof(gribo->pname));
	gribo->min_login_interval_time = m_min_login_interval_s;

	RMILoginClient lc;
	lc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());
	bool result = lc.UserLoginAsyn(gribo->pname, 1, gribo);

	if (!result)
	{
		loginlog::g_log_login.printf(LL_ERROR, "[LoginServer::OnRoleReq RMIError] pname:%s uid[%d, %d].", gribo->pname, user_id.db_index, user_id.role_id);
	}
}

