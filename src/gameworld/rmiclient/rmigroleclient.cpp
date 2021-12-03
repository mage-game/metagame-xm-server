#include "rmigroleclient.h"
#include "servercommon/darmi/darole.h"

#include "servercommon/servercommon.h"

USE_DATAACCESS_VAR();
USE_DATAACCESS_ROLE_VAR();

static char TLV_BUFF[10 * 1024 * 1024];

bool RMIGRoleClient::GetRoleInfoAsyn(int *role_id, int num, rmi::RMIBackObject *backobj)
{
	if (NULL == role_id) return false;

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(num);
	if (!ret) return false;

	for (int i = 0; i < num; ++i)
	{
		ret = s.Push(role_id[i]);
		if (! ret)
		{
			delete backobj;
			return false;
		}
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::ROLE, dataaccessrmi::role::GET_ROLE_INFO, s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
	}

	return ret;
}

bool RMIGGetRoleInfoBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1;

	if (out_param.Pop(&ret))
	{
		RoleInfoList info;

		if (0 == ret)
		{
			if (!info.Unserialize(&out_param))
			{
				return false;
			}
		}
		
		this->GetRoleInfoRet(ret, info);

		return true;
	}

	return false;
}

void RMIGGetRoleInfoBackObject::GetRoleInfoRet(int ret, const RoleInfoList &role_info)
{
	printf("GetRoleInfoRet:%d\n", ret);
}

void RMIGGetRoleInfoBackObject::__free()
{
	delete this;
}

bool RMIGRoleClient::CreateCrossRoleAsyn(PlatName pname, int cross_activity_type, long long original_uuid, RMIGCreateCrossRoleBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Pushf("sik", pname, cross_activity_type, original_uuid);
	if (!ret) 
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::ROLE, dataaccessrmi::role::CREATE_CROSS_ROLE, s, backobj, true, 10000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}


bool RMIGRoleClient::RoleSaveAsyn(int uid, 
								  const RoleInitParam &p, 
								  const RoleOtherInitParam &other_p,
								  const ItemListParam &item_list_param,
								  const HotkeyListParam &hotkey,
								  const FriendListParam &friends,
								  const BlacklistsParam &blacklists_param,
								  const EnemyListParam &enemy,
								  const BusinessListParam &business,
								  const SkillListParam &skill_param,
								  const DailyFindListParam &daily_find_param,
								  RMIGRoleSaveBackObject *backobj,
								  bool is_logout)
{
	// 添加参数时一定要加上相应的静态断言
	UNSTD_STATIC_CHECK((
		sizeof(RoleInitParam) +
		sizeof(RoleOtherInitParam) + 
		sizeof(ItemListParam) +
		sizeof(HotkeyListParam) +
		sizeof(FriendListParam) +
		sizeof(BlacklistsParam) +
		sizeof(EnemyListParam) +
		sizeof(BusinessListParam) +
		sizeof(SkillListParam) +
		sizeof(DailyFindListParam)) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(uid);
	if (!ret) 
	{
		delete backobj;
		return false;
	}

	ret = p.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}
	ret = other_p.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}
	ret = item_list_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}
	ret = hotkey.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}
	ret = friends.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}
	ret = blacklists_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}
	ret = enemy.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}
	ret = business.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}
	ret = skill_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}
	ret = daily_find_param.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}
	ret = s.Push(is_logout);
	if (!ret) 
	{
		delete backobj;
		return false;
	}
	ret = s.Push(0);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::ROLE, dataaccessrmi::role::ROLE_SAVE, s, backobj, true, 10000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMIGCreateCrossRoleBackObject::__response(TLVUnserializer &in_param)
{
	int ret = 0;
	int uid = 0;
	if (!in_param.Pop(&ret) || !in_param.Pop(&uid))
	{
		return false;
	}

	this->CreateCrossRoleRet(ret, IntToUid(uid));

	return true;
}

void RMIGCreateCrossRoleBackObject::__exception(int error)
{
	Error("exception");
}

void RMIGCreateCrossRoleBackObject::__timeout()
{
	Error("timeout");
}

void RMIGCreateCrossRoleBackObject::__free()
{
	delete this;
}

void RMIGCreateCrossRoleBackObject::Error(const char *err_str)
{
	printf("CreateCrossRole error: %s", err_str);
}

void RMIGCreateCrossRoleBackObject::CreateCrossRoleRet(int ret, const UserID &user_id)
{
	printf("CreateCrossRoleRet:%d, user_id=%d\n", ret, UidToInt(user_id));
}

void RMIGRoleSaveBackObject::SetData(const TLVSerializer &data)
{
}

bool RMIGRoleSaveBackObject::__response(TLVUnserializer &int_param)
{
	int ret;
	if (!int_param.Pop(&ret))
	{
		return false;
	}

	RoleSaveRet(ret);

	return true;
}

void RMIGRoleSaveBackObject::__free()
{
	delete this;
}

void RMIGRoleSaveBackObject::__exception(int error)
{
	Error(user_id, ERROR_EXCEPT, 0);
}

void RMIGRoleSaveBackObject::__timeout()
{
	Error(user_id, ERROR_REASON_TIMEOUT, 0);
}

void RMIGRoleSaveBackObject::Error(const UserID &user_id, int reason, int ret)
{
	//gamelog::g_log_save_error.printf(LL_ERROR, "[RMIGRoleSaveBackObject::Error Unserialize_Error][role_id %d][reason:%d][ret:%d]", 
	//	UidToInt(user_id), reason, ret);
}

void RMIGRoleSaveBackObject::RoleSaveRet(int ret)
{
	printf("RoleSaveRet:%d\n", ret);
}
