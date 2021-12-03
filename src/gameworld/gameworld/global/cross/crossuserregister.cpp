#include "crossuserregister.hpp"

#include "internalcomm.h"

#include "utility/configpath.h"

#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/internalprotocal/crossglobalprotocal.h"
#include "rmiclient/rmigroleclient.h"
#include "obj/character/role.h"
#include "world.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"

void RMIGCreateCrossRoleBackObjectImpl::CreateCrossRoleRet(int ret, const UserID &user_id)
{
	if (0 == ret)
	{
		RMIGRoleSaveBackObjectImplCrossRole *rsbo = new RMIGRoleSaveBackObjectImplCrossRole();
		rsbo->user_id = user_id;
		rsbo->cross_activity_type = cross_activity_type;
		F_STRNCPY(rsbo->pname, pname, sizeof(PlatName));
		rsbo->original_plat_type = original_plat_type;
		rsbo->original_user_id = original_user_id;
		F_STRNCPY(rsbo->original_username, original_username, sizeof(GameName));

		RMIGRoleClient rc;
		rc.__bind_session(InternalComm::Instance().GetDBHandler().GetRoleDB());

		rc.RoleSaveAsyn(UidToInt(user_id), all_param.role_param, all_param.other_param, all_param.item_param, all_param.hotkey_param,
			all_param.friend_param, all_param.black_param, all_param.enemy_param, all_param.business_param, all_param.skill_param, all_param.daily_find_param, rsbo, false);
	}
}

void RMIGRoleSaveBackObjectImplCrossRole::RoleSaveRet(int ret)
{
	if (0 == ret)
	{
		CrossUserRegister::Instance().OnSaveCrossRoleInfoSucc(pname, user_id, original_plat_type, original_user_id, original_username);

		// static Platform *platform = Platform::Create();

		static crossgameprotocal::GameCrossSaveCrossRoleRet gcscrr;
		gcscrr.result = 0;
		gcscrr.cross_activity_type = cross_activity_type;
		gcscrr.original_plat_type = original_plat_type;
		gcscrr.original_uid = UidToInt(original_user_id);
		F_STRNCPY(gcscrr.original_username, original_username, sizeof(GameName));

		F_STRNCPY(gcscrr.pname, pname, sizeof(PlatName)); PlatNameReCatPostfix(gcscrr.pname);
		gcscrr.uid = UidToInt(user_id);

		static char login_server_ip[64] = {0};
		if (0 == login_server_ip[0])
		{
			std::string hostname = "127.0.0.1";
			EngineAdapter::Instance().ConfigSyncValue(ROOT/"GatewayModule"/"GameUser"/"LocalIP", &hostname, hostname);
			STRNCPY(login_server_ip, hostname.c_str(), sizeof(login_server_ip));
		}

		UNSTD_STATIC_CHECK(sizeof(login_server_ip) == sizeof(gcscrr.login_server_ip));

		F_STRNCPY(gcscrr.login_server_ip, login_server_ip, sizeof(gcscrr.login_server_ip));
		gcscrr.login_server_port = ServerConfigPool::Instance().common_config.ipconfigs.login_server_addr.user_connect_addr.port;
		gcscrr.anti_wallow = 1;
		gcscrr.reserved1 = 0;
		gcscrr.server = user_id.db_index;
		gcscrr.login_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

		// platform->CreateLoginStr(gcscrr.login_str, gcscrr.pname, gcscrr.login_time, gcscrr.anti_wallow, gcscrr.server);

		InternalComm::Instance().SendToCross((const char *)&gcscrr, sizeof(gcscrr));
	}
	else
	{
		Error(user_id, ERROR_EXECUTE, ret);
	}
}

CrossUserRegister::CrossUserRegister()
{

}

CrossUserRegister::~CrossUserRegister()
{

}

CrossUserRegister & CrossUserRegister::Instance()
{
	static CrossUserRegister cum;
	return cum;
}

void CrossUserRegister::OnSaveCrossRoleInfo(crossgameprotocal::CrossGameSaveCrossRoleInfo *cgscri)
{
	bool IS_HIDDEN_SERVER = CrossConfig::Instance().IsHiddenServer();
	if (!IS_HIDDEN_SERVER) return;

	if (cgscri->cross_activity_type <= CROSS_ACTIVITY_TYPE_INVALID || cgscri->cross_activity_type >= CROSS_ACTIVITY_TYPE_RESERVED ||
		cgscri->original_plat_type < 0 || cgscri->original_uid <= 0 || 0 == cgscri->original_username[0] || 0 == cgscri->pname[0] ||
		cgscri->role_cross_param_data_len <= 0 || cgscri->role_cross_param_data_len > (int)sizeof(cgscri->role_cross_param_data))
	{
		return;
	}

	if (IsCrossUser(IntToUid(cgscri->original_uid))) return;

	static RoleAllParam all_param;
	all_param.Reset();

	TLVUnserializer instream;
	instream.Reset(cgscri->role_cross_param_data, cgscri->role_cross_param_data_len);
	if (!all_param.Unserialize(instream, true)) return;

	PlatNameCatPostfix(cgscri->pname, CROSS_USER_DB_INDEX);

	if (!CrossUserRegister::Instance().CheckCanSaveCrossRoleInfo(cgscri->pname))
	{
		return;
	}

	UniqueUserID unique_user_id(cgscri->original_plat_type, IntToUid(cgscri->original_uid));

	RMIGCreateCrossRoleBackObjectImpl *ccrbo = new RMIGCreateCrossRoleBackObjectImpl();
	ccrbo->cross_activity_type = cgscri->cross_activity_type;
	F_STRNCPY(ccrbo->pname, cgscri->pname, sizeof(PlatName));
	ccrbo->original_plat_type = cgscri->original_plat_type;
	ccrbo->original_user_id = IntToUid(cgscri->original_uid);
	F_STRNCPY(ccrbo->original_username, cgscri->original_username, sizeof(GameName));
	ccrbo->all_param = all_param;

	RMIGRoleClient rc;
	rc.__bind_session(InternalComm::Instance().GetDBHandler().GetRoleDB());
	rc.CreateCrossRoleAsyn(cgscri->pname, cgscri->cross_activity_type, UniqueUserIDToLongLong(unique_user_id), ccrbo);
}

bool CrossUserRegister::CheckCanSaveCrossRoleInfo(PlatName pname)
{
	PnameUserIDmapIt it_id = m_pname_uid_map.find(pname);
	if (it_id != m_pname_uid_map.end())
	{
		Role *user = World::GetInstWorld()->GetSceneManager()->GetRole(it_id->second);
		if (NULL != user) return false;

		CrossUserInfoMapIt it = m_cross_user_info_map.find(it_id->second);
		if (it != m_cross_user_info_map.end())
		{
			if (EngineAdapter::Instance().Time() < it->second.create_time + 5)
			{
				return false;
			}
		}
	}

	return true;
}

void CrossUserRegister::OnSaveCrossRoleInfoSucc(PlatName pname, const UserID &user_id, int original_plat_type, const UserID &original_user_id, GameName original_user_name)
{
	m_pname_uid_map[pname] = user_id;

	CrossUserInfo cui;
	cui.original_plat_type = original_plat_type;
	cui.original_user_id = original_user_id;
	F_STRNCPY(cui.original_user_name, original_user_name, sizeof(cui.original_user_name));
	cui.create_time = EngineAdapter::Instance().Time();

	m_cross_user_info_map[user_id] = cui;
}

void CrossUserRegister::OnUserLogin(Role *user)
{
	if (user->IsInCross())
	{
		this->SendCrossUserLoginToCross(user->GetUserId());
	}
}

void CrossUserRegister::OnUserLogout(Role *user)
{
	if (user->IsInCross())
	{
		for (PnameUserIDmapIt it = m_pname_uid_map.begin(), end = m_pname_uid_map.end(); it != end; ++ it)
		{
			if (it->second == user->GetUserId())
			{
				m_pname_uid_map.erase(it);
				break;
			}
		}

		m_cross_user_info_map.erase(user->GetUserId());

		static crossgameprotocal::GameCrossUserLogout gcul;
		gcul.uid = UidToInt(user->GetUserId());
		InternalComm::Instance().SendToCross((const char *)&gcul, sizeof(gcul));
	}
}

void CrossUserRegister::OnConnectCrossServerSucc()
{
	bool IS_HIDDEN_SERVER = CrossConfig::Instance().IsHiddenServer();
	if (!IS_HIDDEN_SERVER) return;
	
	for (CrossUserInfoMapIt it = m_cross_user_info_map.begin(), end = m_cross_user_info_map.end(); it != end; ++ it)
	{
		if (NULL != World::GetInstWorld()->GetSceneManager()->GetRole(it->first))
		{
			this->SendCrossUserLoginToCross(it->first);
		}
	}
}

void CrossUserRegister::SendCrossUserLoginToCross(const UserID &user_id)
{
	CrossUserInfoMapIt it = m_cross_user_info_map.find(user_id);
	if (it != m_cross_user_info_map.end())
	{
		static crossgameprotocal::GameCrossUserLogin gcul;
		gcul.uid = UidToInt(user_id);
		gcul.original_plat_type = it->second.original_plat_type;
		gcul.original_uid = UidToInt(it->second.original_user_id);
		F_STRNCPY(gcul.original_user_name, it->second.original_user_name, sizeof(gcul.original_user_name));
		InternalComm::Instance().SendToCross((const char *)&gcul, sizeof(gcul));
	}
}

void CrossUserRegister::SendToAllCrossUser(const char *msg, int len, int cross_activity_type)
{
	if (NULL == msg || len <= 0) return;

	for (CrossUserInfoMapIt it = m_cross_user_info_map.begin(), end = m_cross_user_info_map.end(); it != end; ++ it)
	{
		if (CROSS_ACTIVITY_TYPE_RESERVED == cross_activity_type || GetCrossUserCrossActivityType(it->first) == cross_activity_type)
		{
			Role *user = World::GetInstWorld()->GetSceneManager()->GetRole(it->first);
			if (NULL != user)
			{
				EngineAdapter::Instance().NetSend(user->GetNetId(), msg, len);
			}
		}
	}
}

void CrossUserRegister::KickAllCrossUser(int cross_activity_type)
{
	for (CrossUserInfoMapIt it = m_cross_user_info_map.begin(), end = m_cross_user_info_map.end(); it != end; ++ it)
	{
		if (CROSS_ACTIVITY_TYPE_RESERVED == cross_activity_type || GetCrossUserCrossActivityType(it->first) == cross_activity_type)
		{
			Role *user = World::GetInstWorld()->GetSceneManager()->GetRole(it->first);
			if (NULL != user && NULL != user->GetScene())
			{
				EngineAdapter::Instance().NetDisconnect(user->GetNetId(), user->GetUserId(), user->GetName(), user->GetScene()->GetSceneID(), "cross");
			}
		}
	}
}

