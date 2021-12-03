#ifndef __RMIGROLECLIENT_H__
#define __RMIGROLECLIENT_H__

#include "rmi/rmibase.h"
#include "servercommon/struct/roleinfolist.h"
#include "servercommon/struct/roleinitparam.h"
#include "servercommon/crossdef.hpp"
#include "servercommon/struct/roleallparam.hpp"

class RMIGRoleSaveBackObject;

class RMIGCreateCrossRoleBackObject : public rmi::RMIBackObject
{
public:
	RMIGCreateCrossRoleBackObject() : cross_activity_type(CROSS_ACTIVITY_TYPE_INVALID)
	{
		memset(pname, 0, sizeof(pname));
	};
	virtual ~RMIGCreateCrossRoleBackObject() {};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

	int				cross_activity_type;
	PlatName		pname;

protected:
	virtual void CreateCrossRoleRet(int ret, const UserID &user_id);
	void Error(const char *err_str);
};


class RMIGRoleClient : public rmi::RMIProxyObject
{
public:
	RMIGRoleClient(){};
	virtual ~RMIGRoleClient(){};

	bool GetRoleInfoAsyn(int *role_id, int num, rmi::RMIBackObject *backobj);
	
	bool CreateCrossRoleAsyn(PlatName pname, int cross_activity_type, long long original_uuid, RMIGCreateCrossRoleBackObject *backobj);

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
		RMIGRoleSaveBackObject *backobj,
		bool is_logout = false);

protected:
};

class RMIGGetRoleInfoBackObject : public rmi::RMIBackObject
{
public:
	RMIGGetRoleInfoBackObject(){};
	virtual ~RMIGGetRoleInfoBackObject(){};

	virtual bool __response(TLVUnserializer &out_param);
	virtual void __exception(int error){};
	virtual void __timeout(){};
	virtual void __free();

protected:
	virtual void GetRoleInfoRet(int ret, const RoleInfoList &role_info);
};

class RMIGCreateCrossRoleBackObjectImpl : public RMIGCreateCrossRoleBackObject
{
public:
	RMIGCreateCrossRoleBackObjectImpl() : original_plat_type(-1)
	{
		memset(original_username, 0, sizeof(original_username));
	}

	virtual void CreateCrossRoleRet(int ret, const UserID &user_id);

	void *	operator new(size_t c);
	void	operator delete(void *m);

	int				original_plat_type;
	UserID			original_user_id;
	GameName		original_username;
	RoleAllParam	all_param;
};

class RMIGRoleSaveBackObject : public rmi::RMIBackObject
{
public:
	RMIGRoleSaveBackObject() {};
	virtual ~RMIGRoleSaveBackObject() {};

	virtual bool __response(TLVUnserializer &in_param);
	virtual void __exception(int error);
	virtual void __timeout();
	virtual void __free();

	void SetData(const TLVSerializer &data);

	UserID			user_id;

protected:
	enum ERROR_REASON
	{
		ERROR_EXCEPT = -1,
		ERROR_REASON_TIMEOUT = -2,
		ERROR_EXECUTE = -3,
	};

	virtual void RoleSaveRet(int ret);
	void Error(const UserID &user_id, int reason, int ret);
};

class RMIGRoleSaveBackObjectImplCrossRole : public RMIGRoleSaveBackObject
{
public:
	RMIGRoleSaveBackObjectImplCrossRole() : cross_activity_type(CROSS_ACTIVITY_TYPE_INVALID), original_plat_type(-1)
	{
		memset(pname, 0, sizeof(pname));
		memset(original_username, 0, sizeof(original_username));
	}

	virtual void RoleSaveRet(int ret);

	void *	operator new(size_t c);
	void	operator delete(void *m);

	int				cross_activity_type;
	PlatName		pname;
	int				original_plat_type;
	UserID			original_user_id;
	GameName		original_username;
};

class RMIGGetRoleInfoBackObjectImplFixPlatType : public RMIGGetRoleInfoBackObject
{
public:
	RMIGGetRoleInfoBackObjectImplFixPlatType() : fix_type(0), fix_param(0)
	{
		memset(tofix_role_name, 0, sizeof(tofix_role_name));
	}

	virtual void GetRoleInfoRet(int ret, const RoleInfoList &role_info);

	int fix_type;
	int fix_param;
	UniqueUserID tofix_unique_user_id;
	GameName tofix_role_name;
};

#endif // __RMIGROLECLIENT_H__
