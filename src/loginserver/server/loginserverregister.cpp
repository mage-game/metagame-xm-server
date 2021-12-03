#include "protocal/msglogin.h"
#include "loginserver.h"
#include "protocal/msgregister.h"
#include "rmiclient/rmidaroleclient.h"
#include "engineadapter.h"
#include "internalcomm.h"
#include "servercommon/servercommon.h"
#include "roleinitattr.h"
#include "servercommon/darmi/darole.h"
#include "rmibackobjdef.h"
#include "loginlog.h"
#include "platform/platform.h"
#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/namefilter.hpp"
#include "mergeloginmanager.h"
#include "servercommon/logdef.h"

USE_DATAACCESS_VAR();
OLD_USE_INTERNAL_NETWORK();
USE_DATAACCESS_ROLE_VAR();

void RMIUpdateNameInfoBackObjectImpl::UpdateNameInfoRet(int result)
{
	if (result == 0)
	{
		//printf("UpdateNameInfoRet:%d\n", result);
	}
}

void RMIAddRoleBackObjectImpl::AddRoleRet(int ret)
{
	if (ret != 0)
	{
		Error();
		return ;
	}

	Protocol::SCCreateRoleAck cra;
	cra.result = Protocol::SCCreateRoleAck::RESULT_TYPE_SUCC;
	cra.role_id = UidToInt(user_id);
	F_STRNCPY(cra.role_name, role_name, sizeof(cra.role_name));
	cra.avatar = avatar;
	cra.sex = sex;
	cra.prof = prof;
	cra.level = level;
	cra.create_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	EngineAdapter::Instance().NetSend(netid, (const char *)&cra, sizeof(Protocol::SCCreateRoleAck));

	LOG_QUICK12(loginlog::g_log_quick, LOG_TYPE_REGISTER, cra.role_id, cra.role_name, pname, cra.sex, cra.prof, NULL, NULL, plat_spid, 0, 0, 0);

	loginlog::g_log_create.printf(LL_INFO, "[RMIAddRoleBackObjectImpl Success] pname:%s user_id[%d] role_name:%s avatar:%d sex:%d.", pname, UidToInt(user_id), role_name, (int)avatar, (int)sex);

	RMIUpdateNameInfoBackObjectImpl *unibo = new RMIUpdateNameInfoBackObjectImpl();

	RMILoginClient lc;
	lc.__bind_session(InternalComm::Instance().GetDBHandler().GetNameDB());
	bool result = lc.UpdateNameInfoAsyn(role_name, user_id.db_index, cra.role_id, unibo);
	if (!result)
	{
		loginlog::g_log_create.printf(LL_ERROR, "[RMIAddRoleBackObjectImpl RMIError] pname:%s user_id[%d, %d] role_name:%s avatar:%d sex:%d.", pname, user_id.db_index, user_id.role_id, role_name, (int)avatar, (int)sex);
	}
}

void RMIAddRoleBackObjectImpl::Error()
{
	RMIDestroyRoleBackObject *crboi = new RMIDestroyRoleBackObject();
	RMILRoleClient rc;
	rc.__bind_session(InternalComm::Instance().GetDBHandler().GetRoleDB());
	rc.DestroyRoleAsyn(UidToInt(user_id), crboi);

	RMIDeleteNameInfoByNameBackObject *dnibo = new RMIDeleteNameInfoByNameBackObject();
	RMILoginClient lc;
	lc.__bind_session(InternalComm::Instance().GetDBHandler().GetNameDB());
	lc.DeleteNameInfoByNameAsyn(role_name, dnibo);

	Protocol::SCCreateRoleAck cra;
	cra.result = Protocol::SCCreateRoleAck::RESULT_TYPE_NO_SPACE;
	cra.role_id = 0;
	memset(cra.role_name, 0, sizeof(GameName));
	cra.avatar = 0;
	cra.sex = FEMALE;
	cra.prof = 0;
	cra.level = 0;
	cra.create_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	EngineAdapter::Instance().NetSend(netid, (const char *)&cra, sizeof(Protocol::SCCreateRoleAck));

	loginlog::g_log_create.printf(LL_ERROR, "[RMIAddRoleBackObjectImpl::Error] pname:%s user_id[%d, %d] role_name:%s avatar:%d sex:%d.", pname, user_id.db_index, user_id.role_id, role_name, (int)avatar, (int)sex);
}

void RMICreateRoleBackObjectImpl::CreateRoleRet(int ret, int role_id)
{
	if (0 != ret)
	{
		Error();
		return;
	}

	UserID user_id = IntToUid(role_id);
	if (user_id.role_id >= MAX_ROLE_ID)
	{
		Error();
		return;
	}

	MergeLoginManager::Instance().AddCachePlatNameDbIndex(original_panme, db_index);

	RMIAddRoleBackObjectImpl *crboi = new RMIAddRoleBackObjectImpl();
	crboi->netid = netid;
	crboi->user_id = IntToUid(role_id);
	F_STRNCPY(crboi->pname, pname, sizeof(crboi->pname));
	F_STRNCPY(crboi->role_name, role_name, sizeof(crboi->role_name));
	crboi->avatar = avatar;
	crboi->sex = sex;
	crboi->prof = prof;
	crboi->level = level;
	crboi->plat_spid = plat_spid;

	RMILoginClient lc;
	lc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());
	pname[sizeof(PlatName) - 1] = 0;
	bool result = lc.AddRoleAsyn(pname, db_index, role_id, crboi);
	if (!result)
	{
		loginlog::g_log_create.printf(LL_ERROR, "[RMICreateRoleBackObjectImpl RMIError] pname:%s user_id[%d, %d] role_name:%s avatar:%d sex:%d.", pname, db_index, role_id, role_name, (int)avatar, (int)sex);
	}
}
void RMICreateRoleBackObjectImpl::Error()
{
	Protocol::SCCreateRoleAck cra;
	cra.result = Protocol::SCCreateRoleAck::RESULT_TYPE_NO_SPACE;
	cra.role_id = 0;
	memset(cra.role_name, 0, sizeof(GameName));
	cra.avatar = 0;
	cra.sex = FEMALE;
	cra.prof = 0;
	cra.level = 0;
	cra.create_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	EngineAdapter::Instance().NetSend(netid, (const char *)&cra, sizeof(Protocol::SCCreateRoleAck));

	RMIDeleteNameInfoByNameBackObject *dnibo = new RMIDeleteNameInfoByNameBackObject();
	RMILoginClient lc;
	lc.__bind_session(InternalComm::Instance().GetDBHandler().GetNameDB());
	lc.DeleteNameInfoByNameAsyn(role_name, dnibo);
	
	loginlog::g_log_create.printf(LL_ERROR, "[RMICreateRoleBackObjectImpl::Error] pname:%s db_index:%d role_name:%s avatar:%d sex:%d.", pname, db_index, role_name, (int)avatar, (int)sex);
}

void RMIAddNameInfoBackObjectImpl::AddNameInfoRet(int result)
{
	if (result == 0)
	{
		RoleInitParam p = RoleInitAttr::Instance().GetInitParam(prof);

		RMICreateRoleBackObjectImpl *crboi = new RMICreateRoleBackObjectImpl();
		crboi->db_index = db_index_to_create;
		crboi->netid = netid;
		F_STRNCPY(crboi->pname, pname, sizeof(crboi->pname));
		F_STRNCPY(crboi->original_panme, original_panme, sizeof(crboi->original_panme));
		F_STRNCPY(crboi->role_name, role_name, sizeof(crboi->role_name));
		crboi->avatar = avatar;
		crboi->sex = sex;
		crboi->prof = prof;
		crboi->level = 1;
		crboi->plat_spid = plat_spid;

		F_STRNCPY(p.role_name, role_name, sizeof(p.role_name));
		p.avatar = avatar;
		p.sex = sex;

		RMILRoleClient lc;
		lc.__bind_session(InternalComm::Instance().GetDBHandler().GetRoleDB());
		bool result = lc.CreateRoleAsyn(db_index_to_create, p, crboi);
		if (!result)
		{
			loginlog::g_log_create.printf(LL_ERROR, "[RMIAddNameInfoBackObjectImpl RMIError] pname:%s db_index:%d role_name:%s avatar:%d sex:%d.", pname, db_index_to_create, role_name, (int)avatar, (int)sex);
		}
	}
	else
	{
		// 重名
		Protocol::SCCreateRoleAck cra;
		cra.result = Protocol::SCCreateRoleAck::RESULT_TYPE_EXIST_NAME;	// 重名
		cra.role_id = 0;
		memset(cra.role_name, 0, sizeof(GameName));
		cra.avatar = 0;
		cra.sex = FEMALE;
		cra.prof = 0;
		cra.level = 0;
		cra.create_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		EngineAdapter::Instance().NetSend(netid, (const char *)&cra, sizeof(cra));
		
		loginlog::g_log_create.printf(LL_WARNING, "[RMIAddNameInfoBackObjectImpl NameExist] pname:%s role_name:%s avatar:%d sex:%d.", pname, role_name, (int)avatar, (int)sex);
	}
}

void RMIUserLoginBackObjectImplCreate::UserLoginRet(int ret, int db_index, int role_1, int role_2, int role_3)
{
	if (ret == 0) 
	{
		if (role_1 == 0 || role_2 == 0 || role_3 == 0)
		{
			int db_index_to_create = db_index;
			if (role_1 == 0 && role_2 == 0 && role_3 == 0)
			{
				db_index_to_create = default_db_index; // 还没有创建过角色 使用所在服的默认db
			}

			RMIAddNameInfoBackObjectImpl *anibo = new RMIAddNameInfoBackObjectImpl();
			anibo->db_index_to_create = db_index_to_create;
			anibo->netid = netid;
			F_STRNCPY(anibo->pname, pname, sizeof(anibo->pname));
			F_STRNCPY(anibo->original_panme, original_panme, sizeof(anibo->original_panme));
			F_STRNCPY(anibo->role_name, role_name, sizeof(anibo->role_name));
			anibo->avatar = avatar;
			anibo->sex = sex;
			anibo->prof = prof;
			anibo->plat_spid = plat_spid;

			RMILoginClient lc;
			lc.__bind_session(InternalComm::Instance().GetDBHandler().GetNameDB());
			bool result = lc.AddNameInfoAsyn(role_name, pname, anibo);

			if (!result)
			{
				loginlog::g_log_create.printf(LL_ERROR, "[RMIUserLoginBackObjectImplCreate RMIError] pname:%s role_name:%s avatar:%d sex:%d.", pname, role_name, (int)avatar, (int)sex);
			}

			return;
		}
  
		// 没有空位了
	}

	Protocol::SCCreateRoleAck cra;
	cra.result = Protocol::SCCreateRoleAck::RESULT_TYPE_NO_SPACE;
	cra.role_id = 0;
	memset(cra.role_name, 0, sizeof(GameName));
	cra.avatar = avatar;
	cra.sex = sex;
	cra.prof = 0;
	cra.level = 0;
	cra.create_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	EngineAdapter::Instance().NetSend(netid, (const char *)&cra, sizeof(cra));

	loginlog::g_log_create.printf(LL_WARNING, "[RMIUserLoginBackObjectImplCreate %s] pname:%s role_name:%s avatar:%d sex:%d.", (ret == 0 ? "NoRoleSpace" : "ret!=0"), pname, role_name, (int)avatar, (int)sex);
}

void LoginServer::OnCreateRole(NetID netid, const char *data)
{
	if (m_is_register_limit)
	{
		Protocol::SCCreateRoleAck cra;
		cra.result = Protocol::SCCreateRoleAck::RESULT_TYPE_SERVER_LIMIT;
		EngineAdapter::Instance().NetSend(netid, (const char *)&cra, sizeof(cra));
		return;
	}

	Protocol::CSCreateRole *cr = (Protocol::CSCreateRole*)data;
	cr->pname[sizeof(PlatName) - 1] = 0;

	static PlatName original_panme = {0};
	F_STRNCPY(original_panme, cr->pname, sizeof(original_panme));

	if (cr->server >= MAX_DB_IDNEX || cr->server < 0) return;		// 超过一个平台允许的服数

	if (!m_platform->CheckAccount(netid, cr->pname, cr->login_time, cr->login_str, cr->server)) return;

	if (0 == cr->role_name[0]) return; 
	InvalidCharFilte(cr->role_name);
	RoleNameCatPostfix(cr->role_name, cr->server); 

	static const bool NAMEFILTER_OPEN_FLAG = ServerConfigPool::Instance().common_config.namefilter_open_flag;
	if (NAMEFILTER_OPEN_FLAG && NameFilter::Instance().HasSensitiveWord(sizeof(cr->role_name), cr->role_name)) 
	{
		// 名字不合法
		Protocol::SCCreateRoleAck cra;
		cra.result = Protocol::SCCreateRoleAck::RESULT_TYPE_NAME_INVALID;
		cra.role_id = 0;
		memset(cra.role_name, 0, sizeof(GameName));
		cra.avatar = 0;
		cra.sex = FEMALE;
		cra.prof = 0;
		cra.level = 0;
		cra.create_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		EngineAdapter::Instance().NetSend(netid, (const char *)&cra, sizeof(cra));

		return;
	}

	if (cr->prof < PROF_TYPE_PROF_1 || cr->prof > PROF_BASE_TYPE_MAX)
	{
		return;
	}

	RMIUserLoginBackObjectImplCreate *ulboic = new RMIUserLoginBackObjectImplCreate();
	ulboic->netid = netid;
	F_STRNCPY(ulboic->pname, cr->pname, sizeof(ulboic->pname));
	F_STRNCPY(ulboic->original_panme, original_panme, sizeof(ulboic->original_panme));
	F_STRNCPY(ulboic->role_name, cr->role_name, sizeof(ulboic->role_name));
	ulboic->avatar = cr->avatar;
	ulboic->sex = cr->sex;
	ulboic->prof = cr->prof;
	ulboic->default_db_index = cr->server;
	ulboic->plat_spid = cr->plat_spid;

	ulboic->login_server = this;

	RMILoginClient lc;
	lc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());
	bool result = lc.UserLoginAsyn(ulboic->pname, 1, ulboic);

	if (!result)
	{
		loginlog::g_log_create.printf(LL_ERROR, "[LoginServer::OnCreateRole RMIError] pname:%s role_name:%s avatar:%d sex:%d.", ulboic->pname, ulboic->role_name, (int)ulboic->avatar, (int)ulboic->sex);
	}
}

void RMIDeleteNameInfoBackObjectImpl::DeleteNameInfoRet(int result)
{
	if (result == 0)
	{
		//printf("DeleteNameInfoRet:%d\n", result);
	}
}

void RMIDestroyRoleBackObjectImpl::DestroyRoleRet(int ret)
{
	if (ret != 0)
	{
		RMIAddRoleBackObject *crbo = new RMIAddRoleBackObject();
		RMILoginClient lc;
		lc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());
		lc.AddRoleAsyn(pname, user_id.db_index, user_id.role_id, crbo);

		Protocol::SCDestroyRoleAck cra;
		cra.role_id = UidToInt(user_id);
		cra.result = -1;
		EngineAdapter::Instance().NetSend(netid, (const char *)&cra, sizeof(Protocol::SCDestroyRoleAck));

		loginlog::g_log_destory.printf(LL_ERROR, "[RMIDestroyRoleBackObjectImpl ret:%d] pname:%s user_id[%d, %d].", ret, pname, user_id.db_index, user_id.role_id);

		return ;
	}

	Protocol::SCDestroyRoleAck cra;
	cra.role_id = UidToInt(user_id);
	cra.result =  0;
	EngineAdapter::Instance().NetSend(netid, (const char *)&cra, sizeof(Protocol::SCDestroyRoleAck));

	loginlog::g_log_destory.printf(LL_INFO, "[RMIRemoveRoleBackObjectImpl Success] pname:%s user_id[%d].", pname, UidToInt(user_id));
}

void RMIRemoveRoleBackObjectImpl::RemoveRoleRet(int ret)
{
	if (ret != 0)
	{
		Protocol::SCDestroyRoleAck cra;
		cra.role_id = UidToInt(user_id);
		cra.result = -1;
		EngineAdapter::Instance().NetSend(netid, (const char *)&cra, sizeof(Protocol::SCDestroyRoleAck));

		loginlog::g_log_destory.printf(LL_ERROR, "[RMIRemoveRoleBackObjectImpl ret:%d] pname:%s user_id[%d, %d].", ret, pname, user_id.db_index, user_id.role_id);
		return;
	}
	
	RMIDestroyRoleBackObjectImpl *drboi = new RMIDestroyRoleBackObjectImpl();
	drboi->user_id = user_id;
	drboi->netid = netid;
	F_STRNCPY(drboi->pname, pname, sizeof(drboi->pname));

	RMILRoleClient lc;
	lc.__bind_session(InternalComm::Instance().GetDBHandler().GetRoleDB());
	bool result = lc.DestroyRoleAsyn(UidToInt(user_id), drboi);
	if (!result)
	{
		loginlog::g_log_destory.printf(LL_ERROR, "[RMIRemoveRoleBackObjectImpl RMIError] pname:%s user_id[%d, %d].", pname, user_id.db_index, user_id.role_id);
	}

	if (result)
	{
		RMIDeleteNameInfoBackObjectImpl *dnibo = new RMIDeleteNameInfoBackObjectImpl();
		RMILoginClient loginc;
		loginc.__bind_session(InternalComm::Instance().GetDBHandler().GetNameDB());
		loginc.DeleteNameInfoAsyn(user_id.db_index, UidToInt(user_id), pname, dnibo);
	}
}

void LoginServer::OnDestroyRole(NetID netid, const char *data)
{
	//Protocol::CSDestroyRole *dr = (Protocol::CSDestroyRole*)data;
	//dr->pname[sizeof(PlatName) - 1] = 0;

	//char anti_wallow;
	//if (!m_platform->CheckAccount(netid, dr->pname, dr->login_time, dr->anti_wallow, dr->login_str, &anti_wallow, dr->server)) return ;

	//UserID user_id = IntToUid(dr->role_id);
	//
	//PlatName pname;
	//F_STRNCPY(pname, dr->pname, sizeof(PlatName));

	//UserItem *user = m_user_manager.GetUser(user_id);
	//if (user != 0)
	//{
	//	// 踢一次
	//	KickUser(user_id);

	//	Protocol::SCDestroyRoleAck cra;
	//	cra.role_id = UidToInt(user_id);
	//	cra.result = -2;
	//	EngineAdapter::Instance().NetSend(netid, (const char *)&cra, sizeof(Protocol::SCDestroyRoleAck));

	//	loginlog::g_log_destory.printf(LL_WARNING, "[LoginServer::OnDestroyRole UserHasLogin] pname:%s user_id[%d, %d] login_time:%u.", pname, 
	//		user_id.db_index, user_id.role_id, (unsigned int)user->GetLoginTime());
	//	return;
	//}

	//RMIRemoveRoleBackObjectImpl *rrboi = new RMIRemoveRoleBackObjectImpl();
	//rrboi->user_id = user_id;
	//rrboi->netid = netid;
	//F_STRNCPY(rrboi->pname, pname, sizeof(rrboi->pname));

	//RMILoginClient lc;
	//lc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());
	//bool result = lc.RemoveRoleAsyn(pname, user_id.db_index, UidToInt(user_id), rrboi);
	//if (!result)
	//{
	//	loginlog::g_log_destory.printf(LL_ERROR, "[LoginServer::OnDestroyRole RMIError] pname:%s user_id[%d, %d].", pname, user_id.db_index, user_id.role_id);
	//}
}

