
#ifndef RMIBACKOBJDEF_H
#define RMIBACKOBJDEF_H

#include "servercommon/logindef.h"

#include "rmiclient/rmiroleclient.h"
#include "rmiclient/rmiloginclient.h"
#include "inetworkmodule.h"

class SceneManager;

class RMIRoleInitBackObjectImpl : public RMIRoleInitBackObject
{
public:
	SceneManager *scene_manager;
	SceneIndex	scene_index;
	int scene_id;
	GSNetID netid;
	IP userip;
	PlatName pname;
	UserID user_id;
	bool is_login;
	int inc_key;
	int plat_spid;

	bool is_micro_pc;

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

	void *	operator new(size_t c);
	void	operator delete(void *m);
};

class RMIRoleSaveBackObjImplLocal : public RMIRoleSaveBackObject
{
public:
	virtual void RoleSaveRet(int ret);

	void *	operator new(size_t c);
	void	operator delete(void *m);
};

class RMIRoleSaveBackObjImplRemote : public RMIRoleSaveBackObject
{
public:
	UserID uid;
	GameName role_name;
	GSNetID netid;
	int target_scene_id;
	int target_scene_key;
	int scene_id;
	unsigned short gs_index;
	PlatName pname;

	virtual void RoleSaveRet(int ret);

	void *	operator new(size_t c);
	void	operator delete(void *m);
};

class RMIRoleSaveBackObjectImplRole : public RMIRoleSaveBackObject
{
public:
	virtual void RoleSaveRet(int ret);

	void *	operator new(size_t c);
	void	operator delete(void *m);
};

class RMIGetGoldBackObjectImpl : public RMIGetGoldBackObject
{
public:
	SceneManager *scene_manager;

	UserID user_id;

	virtual void GetGoldRet(Int64 gold, int authority_type);

	void *	operator new(size_t c);
	void	operator delete(void *m);
};

class RMIGetGoldBackObjectImplChange : public RMIGetGoldBackObject
{
public:
	SceneManager *scene_manager;
	SceneIndex scene_index;
	ObjID obj_id;

	UserID user_id;
	Int64 gold_get;
	GSNetID netid;

	void *	operator new(size_t c);
	void	operator delete(void *m);
protected:
	virtual void GetGoldRet(Int64 gold);
};

class RMIChangeGoldBackObjectImpl : public RMIChangeGoldBackObject
{
public:
	SceneManager *scene_manager;

	SceneIndex scene_index; 
	ObjID obj_id; 
	UserID user_id;

	Int64 gold_get;
	int authority_type;

	PlatName pname;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void ChangeGoldRet(int result, Int64 new_gold);
	virtual void Error(const char *err_str);
};

class RMIChangeGoldBackObjectImplErrorAdd : public RMIChangeGoldBackObject
{
public:
	UserID user_id;
	Int64 gold_get;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void ChangeGoldRet(int result, Int64 new_gold);
	virtual void Error(const char *err_str);
};

class RMIRoleSaveStatusBackObjectImpl : public RMIRoleSaveStatusBackObject
{
public:
	virtual void RoleSaveStatusRet(int ret);

	void *	operator new(size_t c);
	void	operator delete(void *m);
};

class RMIQueryNameExistBackObjectImpl : public RMIQueryNameExistBackObject
{
public:
	UserID user_id;
	GameName role_name;
	bool is_item_reset;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void QueryNameExistRet(int result);
};

class RMIResetNameBackObjectImpl : public RMIResetNameBackObject
{
public:
	UserID user_id;
	GameName role_name;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void ResetNameRet(int result);
};


class RMIRoleQueryBackObjectImpl : public RMIRoleQueryBackObject
{
public:
	int query_from;
	int plat_type;
	UserID viewer_user_id;
	int target_user_id;

	virtual void RoleQueryRet(int ret, const RoleQueryParam &p, const ItemListParam &item_list_param);

	virtual void __timeout();

	void *	operator new(size_t c);
	void	operator delete(void *m);
};


#endif
