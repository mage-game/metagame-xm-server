#ifndef RMIROLECLIENT_H
#define RMIROLECLIENT_H

#include "rmi/rmibase.h"
#include "servercommon/struct/roleinitparam.h"
#include "servercommon/struct/roleotherinitparam.hpp"
#include "servercommon/struct/skilllistparam.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/struct/rolequeryparam.hpp"
#include "servercommon/struct/hotkeylistparam.h"
#include "servercommon/struct/friendparam.h"
#include "servercommon/struct/enemyparam.h"
#include "servercommon/struct/businessparam.h"
#include "servercommon/struct/blacklistsparam.h"

#include "servercommon/struct/mountparam.hpp"
#include "servercommon/struct/dailyfindlistparam.hpp"

class RMIRoleSaveBackObject;

class RMIRoleClient : public rmi::RMIProxyObject
{
public:
	RMIRoleClient() {}
	virtual ~RMIRoleClient() {}

	bool RoleInitAsyn(int uid, rmi::RMIBackObject *backobj);

	bool RoleSaveAsyn(int uid, 
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
						bool is_logout = false,
						int allow_to_cross_flag = 0);

	bool RoleSaveStatusAsyn(int uid, bool is_logout, bool is_micro_pc, rmi::RMIBackObject *backobj);

	bool FriendSaveAsyn(int role_id, const FriendListParam &friends, const BlacklistsParam &blacks, rmi::RMIBackObject *backobj);

	bool RoleQueryAsyn(int uid, rmi::RMIBackObject *backobj);
};

class RMIRoleInitBackObject : public rmi::RMIBackObject
{
public:
	RMIRoleInitBackObject() {}
	virtual ~RMIRoleInitBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error) {}
	virtual void __timeout() {}
	virtual void __free();

protected:
	virtual void RoleInitRet(int ret, const RoleInitParam &p, 
									const RoleOtherInitParam &other_p,
									const ItemListParam &item_list_param,
									const HotkeyListParam &hotkey,
									const FriendListParam &friends,
									const BlacklistsParam &blacklists_param,
									const EnemyListParam &enemy,
									const BusinessListParam &business_param,
									const SkillListParam &skill_param,
									const DailyFindListParam &daily_find_param,
									long long last_save_time);
};

class RMIRoleQueryBackObject : public rmi::RMIBackObject
{
public:
	RMIRoleQueryBackObject() {}
	virtual ~RMIRoleQueryBackObject() {}

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error) {}
	virtual void __timeout();
	virtual void __free();

protected:
	virtual void RoleQueryRet(int ret, const RoleQueryParam &p, const ItemListParam &item_list_param);
};

class RMIRoleSaveBackObject : public rmi::RMIBackObject
{
public:
	RMIRoleSaveBackObject();
	virtual ~RMIRoleSaveBackObject();

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

	void SetData(const TLVSerializer &data);

	UserID log_user_id; 

protected:
	enum ERROR_REASON
	{
		ERROR_EXCEPT = -1,
		ERROR_REASONTIMEOUT = -2,
		ERROR_EXECUTE = -3,
	};

	virtual void RoleSaveRet(int ret);
	void Error(const UserID &user_id, int reason, int ret);
};

class RMIRoleSaveStatusBackObject : public rmi::RMIBackObject
{
public:
	RMIRoleSaveStatusBackObject();
	virtual ~RMIRoleSaveStatusBackObject();

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

	UserID log_user_id; 

protected:
	enum ERROR_REASON
	{
		ERROR_EXCEPT = -1,
		ERROR_REASONTIMEOUT = -2,
		ERROR_EXECUTE = -3,
	};

	virtual void RoleSaveStatusRet(int ret);
	void Error(const UserID &user_id, int reason, int ret);
};

class RMIFriendSaveBackObject : public rmi::RMIBackObject
{
public:
	RMIFriendSaveBackObject();
	virtual ~RMIFriendSaveBackObject();

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

	UserID log_user_id; 

protected:
	enum ERROR_REASON
	{
		ERROR_EXCEPT = -1,
		ERROR_REASONTIMEOUT = -2,
		ERROR_EXECUTE = -3,
	};

	void Error(const UserID &user_id, int reason, int ret);
};

#endif

