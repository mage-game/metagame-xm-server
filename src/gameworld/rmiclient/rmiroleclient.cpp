#include "rmiroleclient.h"
#include "servercommon/darmi/darole.h"
#include "servercommon/serverclmempool.h"
#include "gamelog.h"

#include "servercommon/servercommon.h"
#include "engineadapter.h"

USE_DATAACCESS_VAR();
USE_DATAACCESS_ROLE_VAR();

static char TLV_BUFF[4 * 1024 * 1024];

enum RMI_INIT_ERROR_TYPE
{
	RMI_INIT_ERROR_REQUEST,
	RMI_INIT_ERROR_RESPONSE,
	RMI_INIT_ERROR_EXCEPTION,
	RMI_INIT_ERROR_TIMEOUT,
};

/*static void RMIInitError(int err, const char *msg, bool stop = true)
{
	printf("RMI Init Error[type: %d, msg: %s]\n", err, NULL == msg ? "" : msg);

	if (stop)
	{
		EngineAdapter::Instance().StopGame();
	}
}*/

// 获取Role初始化信息

bool RMIRoleClient::RoleInitAsyn(int uid, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(uid);
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::ROLE, dataaccessrmi::role::ROLE_INIT, s, backobj, true, 5000);
	}

	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMIRoleClient::RoleQueryAsyn(int uid, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(uid);
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::ROLE, dataaccessrmi::role::ROLE_QUERY, s, backobj, false, 5000);
	}

	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMIRoleInitBackObject::__response(TLVUnserializer &in_param)
{
	int ret;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	static RoleInitParam p;
	static RoleOtherInitParam other_p;
	static ItemListParam item_list_param;
	static HotkeyListParam hotkey;
	static FriendListParam friends;
	static BlacklistsParam blacklists_param;
	static EnemyListParam enemy;
	static BusinessListParam business_param;
	static SkillListParam skill_param;
	static DailyFindListParam daily_find_param;

	long long last_save_time = 0;

	if (ret == 0)
	{
		if (!p.Unserialize(in_param)
			|| !other_p.Unserialize(in_param)
			|| !item_list_param.Unserialize(in_param)
			|| !hotkey.Unserialize(in_param)
			|| !friends.Unserialize(in_param)
			|| !blacklists_param.Unserialize(in_param)
			|| !enemy.Unserialize(in_param)
			|| !business_param.Unserialize(in_param)
			|| !skill_param.Unserialize(in_param)
			|| !daily_find_param.Unserialize(in_param)
			|| !in_param.Pop(&last_save_time))
		{
			return false;
		}
	}

	RoleInitRet(ret, p, other_p, item_list_param, hotkey, friends, blacklists_param, enemy, business_param, 
		skill_param, daily_find_param, last_save_time);

	return true;
}

void RMIRoleInitBackObject::RoleInitRet(int ret, const RoleInitParam &p, 
										const RoleOtherInitParam &other_p,
										const ItemListParam &item_list_param,
										const HotkeyListParam &hotkey,
										const FriendListParam &friends,
										const BlacklistsParam &blacklists_param,
										const EnemyListParam &enemy,
										const BusinessListParam &business_param,
										const SkillListParam &skill_param,
										const DailyFindListParam &daily_find_param,
										long long last_save_time)
{
	printf("RoleInitRet:%d\n", ret);
}

void RMIRoleInitBackObject::__free()
{
	delete this;
}

// 保存Role信息
bool RMIRoleClient::RoleSaveAsyn(int uid,
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
								 RMIRoleSaveBackObject *backobj,
								 bool is_logout,
								 int allow_to_cross_flag)
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
		sizeof(DailyFindListParam) ) * 2 < sizeof(TLV_BUFF));

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
	ret = s.Push(allow_to_cross_flag);
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

RMIRoleSaveBackObject::RMIRoleSaveBackObject()
{
}

RMIRoleSaveBackObject::~RMIRoleSaveBackObject()
{
}

void RMIRoleSaveBackObject::SetData(const TLVSerializer &data)
{
}

bool RMIRoleSaveBackObject::__response(TLVUnserializer &int_param)
{
	int ret;
	if (!int_param.Pop(&ret))
	{
		return false;
	}

	RoleSaveRet(ret);

	return true;
}

void RMIRoleSaveBackObject::__free()
{
	delete this;
}

void RMIRoleSaveBackObject::RoleSaveRet(int ret)
{
	printf("RoleSaveRet:%d\n", ret);
}

void RMIRoleSaveBackObject::__exception(int error)
{
	Error(log_user_id, ERROR_EXCEPT, 0);
}

void RMIRoleSaveBackObject::__timeout()
{
	Error(log_user_id, ERROR_REASONTIMEOUT, 0);
}

void RMIRoleSaveBackObject::Error(const UserID &user_id, int reason, int ret)
{
	gamelog::g_log_save_error.printf(LL_ERROR, "[RMIRoleSaveBackObject::Error Unserialize_Error][role_id %d][reason:%d][ret:%d]", 
		UidToInt(user_id), reason, ret);
}

// 保存在线状态
bool RMIRoleClient::RoleSaveStatusAsyn(int uid, bool is_logout, bool is_micro_pc, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(uid) && s.Push(is_logout) && s.Push(is_micro_pc);
	if (!ret) 
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::ROLE, dataaccessrmi::role::ROLE_SAVE_STATUS, s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}


RMIRoleSaveStatusBackObject::RMIRoleSaveStatusBackObject()
{
}

RMIRoleSaveStatusBackObject::~RMIRoleSaveStatusBackObject()
{
}

bool RMIRoleSaveStatusBackObject::__response(TLVUnserializer &int_param)
{
	int ret;
	if (!int_param.Pop(&ret))
	{
		return false;
	}

	RoleSaveStatusRet(ret);

	return true;
}

void RMIRoleSaveStatusBackObject::__free()
{
	delete this;
}

void RMIRoleSaveStatusBackObject::RoleSaveStatusRet(int ret)
{
	printf("RoleSaveStatusRet:%d\n", ret);
}

void RMIRoleSaveStatusBackObject::__exception(int error)
{
	Error(log_user_id, ERROR_EXCEPT, 0);
}

void RMIRoleSaveStatusBackObject::__timeout()
{
	Error(log_user_id, ERROR_REASONTIMEOUT, 0);
}

void RMIRoleSaveStatusBackObject::Error(const UserID &user_id, int reason, int ret)
{
	gamelog::g_log_save_error.printf(LL_ERROR, "[RMIRoleSaveStatusBackObject::Error Unserialize_Error][role_id %d][reason:%d][ret:%d]", 
		UidToInt(user_id), reason, ret);
}

// 保存Friend信息
bool RMIRoleClient::FriendSaveAsyn(int role_id, const FriendListParam &friends, const BlacklistsParam &blacks, rmi::RMIBackObject *backobj)
{
	UNSTD_STATIC_CHECK((sizeof(FriendListParam) + sizeof(BlacklistsParam)) * 2 < sizeof(TLV_BUFF));

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(role_id);
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

	ret = blacks.Serialize(s);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	ret = m_session.rmi_module->Call(m_session, dataaccessrmi::ROLE, dataaccessrmi::role::FRIEND_SAVE, s, backobj, true, 10000);
	if (!ret)
	{
		delete backobj;
		return false;
	}

	return ret;
}

bool RMIRoleQueryBackObject::__response(TLVUnserializer &in_param)
{
	int ret;
	if (!in_param.Pop(&ret))
	{
		return false;
	}

	static RoleQueryParam p;
	static ItemListParam item_list_param;

	if (ret == 0)
	{
		if (!p.Unserialize(in_param)
			|| !item_list_param.Unserialize(in_param))
		{
			return false;
		}
	}

	RoleQueryRet(ret, p, item_list_param);

	return true;
}

void RMIRoleQueryBackObject::__free()
{
	delete this;
}

void RMIRoleQueryBackObject::RoleQueryRet(int ret, const RoleQueryParam &p, const ItemListParam &item_list_param)
{
	printf("RoleQueryRet:%d, uid:%d\n", ret, p.role_id);
}

void RMIRoleQueryBackObject::__timeout()
{
	printf("RMIRoleQueryBackObject::__timeout\n");
}

RMIFriendSaveBackObject::RMIFriendSaveBackObject()
{
}

RMIFriendSaveBackObject::~RMIFriendSaveBackObject()
{
}

bool RMIFriendSaveBackObject::__response(TLVUnserializer &int_param)
{
	int ret;
	if (!int_param.Pop(&ret))
	{
		return false;
	}

	if (0 != ret)
	{
		Error(log_user_id, ERROR_EXECUTE, ret);
	}

	return true;
}

void RMIFriendSaveBackObject::__free()
{
	delete this;
}

void RMIFriendSaveBackObject::__exception(int error)
{
	Error(log_user_id, ERROR_EXCEPT, 0);
}

void RMIFriendSaveBackObject::__timeout()
{
	Error(log_user_id, ERROR_REASONTIMEOUT, 0);
}

void RMIFriendSaveBackObject::Error(const UserID &user_id, int reason, int ret)
{
	gamelog::g_log_save_error.printf(LL_ERROR, "[RMIFriendBackObject::Error Unserialize_Error][role_id %d][reason:%d][ret:%d]", 
		UidToInt(user_id), reason, ret);
}

