
#ifndef RMIBACKOBJDEF_H
#define RMIBACKOBJDEF_H

#include "rmiclient/rmidaloginclient.h"
#include "rmiclient/rmidaroleclient.h"
#include "servercommon/darmi/rmisystemclient.h"
#include "servercommon/logindef.h"
#include "inetworkmodule.h"

class LoginServer;

class RMIGetProfBackObjectImpl : public RMIGetProfNumBackObject
{
public:
	RMIGetProfBackObjectImpl()
	{

	}

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void GetProfNumRet(int ret, int prof1_num, int prof2_num, int prof3_num, int prof4_num);
};

class RMIUserLoginBackObjectImpl : public RMIUserLoginBackObject
{
public:
	RMIUserLoginBackObjectImpl() : netid(-1), merge_key(0)
	{
		memset(pname, 0, sizeof(pname));
		memset(original_panme, 0, sizeof(original_panme));
	}

	NetID netid;
	PlatName pname;
	PlatName original_panme;
	int merge_key;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void UserLoginRet(int ret, int db_index, int role_1, int role_2, int role_3);
};

class RMIUserLoginBackObjectImplCreate : public RMIUserLoginBackObject
{
public:
	RMIUserLoginBackObjectImplCreate() : login_server(NULL), netid(-1), avatar(0), sex(0), prof(0), default_db_index(0), plat_spid(0)
	{
		memset(pname, 0, sizeof(pname));
		memset(original_panme, 0, sizeof(original_panme));
		memset(role_name, 0, sizeof(role_name));
	}

	LoginServer *login_server;
	NetID netid;
	PlatName pname;
	PlatName original_panme;
	GameName role_name;
	char avatar;
	char sex;
	char prof;
	short default_db_index;
	int plat_spid;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void UserLoginRet(int ret, int db_index, int role_1, int role_2, int role_3);
};

class RMIAddRoleBackObjectImpl : public RMIAddRoleBackObject
{
public:
	RMIAddRoleBackObjectImpl() : netid(-1), avatar(0), sex(0), prof(0), level(0), plat_spid(0)
	{
		memset(pname, 0, sizeof(pname));
		memset(role_name, 0, sizeof(role_name));
	}

	NetID netid;
	UserID user_id;
	PlatName pname;
	GameName role_name;
	char avatar;
	char sex;
	char prof;
	unsigned short level;
	int plat_spid;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void AddRoleRet(int result);
	virtual void Error();
};

class RMIRemoveRoleBackObjectImpl : public RMIRemoveRoleBackObject
{
public:
	RMIRemoveRoleBackObjectImpl() : netid(-1)
	{
		memset(pname, 0, sizeof(pname));
	}

	UserID user_id;
	int netid;
	PlatName pname;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void RemoveRoleRet(int result);
};

class RMIPassAntiWallowBackObjectImpl : public RMIPassAntiWallowBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void PassAntiWallowRet(int ret);
};

class RMIForbidBackObjectImpl : public RMIForbidBackObject
{
public:
	RMIForbidBackObjectImpl() : login_server(NULL), cmd_id(0)
	{
		memset(pname, 0, sizeof(pname));
	}

	void *	operator new(size_t c);
	void	operator delete(void *m);

	LoginServer *login_server;
	PlatName pname;

	int cmd_id;
	std::string creator;
	std::string cmd;

protected:
	virtual void ForbidRet(int result, int db_index, int role_1, int role_2, int role_3);
};

class RMIForbidOneRoleBackObjectImpl : public RMIForbidOneRoleBackObject
{
public:
	RMIForbidOneRoleBackObjectImpl() : login_server(NULL), cmd_id(0), role_id(0)
	{
		memset(pname, 0, sizeof(pname));
	}

	void *	operator new(size_t c);
	void	operator delete(void *m);

	LoginServer *login_server;
	PlatName pname;

	int cmd_id;
	int role_id;
	std::string creator;
	std::string cmd;

protected:
	virtual void ForbidOneRoleRet(int result, int db_index);
};

class RMIAddNameInfoBackObjectImpl : public RMIAddNameInfoBackObject
{
public:
	RMIAddNameInfoBackObjectImpl() : netid(-1), db_index_to_create(0), avatar(0), sex(0), prof(0), plat_spid(0)
	{
		memset(pname, 0, sizeof(pname));
		memset(original_panme, 0, sizeof(original_panme));
		memset(role_name, 0, sizeof(role_name));
	}

	NetID netid;
	int db_index_to_create;
	PlatName pname;
	PlatName original_panme;
	GameName role_name;
	char avatar;
	char sex;
	char prof;
	int plat_spid;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void AddNameInfoRet(int result);
};

class RMIUpdateNameInfoBackObjectImpl : public RMIUpdateNameInfoBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void UpdateNameInfoRet(int result);
};

class RMIDeleteNameInfoBackObjectImpl : public RMIDeleteNameInfoBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void DeleteNameInfoRet(int result);
};



class RMIGetRoleInfoBackObjectImpl : public RMIGetRoleInfoBackObject
{
public:
	RMIGetRoleInfoBackObjectImpl() : db_index(0), netid(-1), result(0), merge_key(0)
	{
		memset(pname, 0, sizeof(pname));
		memset(original_panme, 0, sizeof(original_panme));
	}

	int db_index;
	NetID netid;
	PlatName pname;
	PlatName original_panme;
	int result;
	int merge_key;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void GetRoleInfoRet(int ret, const RoleInfoList &role_info);
};

class LoginServer;

class RMIUserLoginBackObjectImplRole : public RMIUserLoginBackObject
{
public:
	RMIUserLoginBackObjectImplRole() : game_thread(0), ip(0), netid(-1), login_server(NULL), min_login_interval_time(0)
	{
		memset(pname, 0, sizeof(pname));
	}

	int game_thread;
	UserID user_id;
	IP ip;
	NetID netid;
	LoginServer *login_server;
	PlatName pname;
	int min_login_interval_time;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void UserLoginRet(int ret, int db_index, int role_1, int role_2, int role_3);
};

class RMIGetRoleInfoBackObjectImplRoleReq : public RMIGetRoleInfoBackObject
{
public:
	RMIGetRoleInfoBackObjectImplRoleReq() : ip(0), netid(-1), login_server(NULL), min_login_interval_time(0)
	{
		memset(pname, 0, sizeof(pname));
	}

	UserID user_id;
	IP ip;
	NetID netid;
	LoginServer *login_server;
	PlatName pname;

	int min_login_interval_time;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void GetRoleInfoRet(int ret, const RoleInfoList &role_info_list);
};


class RMICreateRoleBackObjectImpl : public RMICreateRoleBackObject
{
public:
	RMICreateRoleBackObjectImpl() : db_index(0), netid(-1), avatar(0), sex(0), prof(0), level(0), plat_spid(0)
	{
		memset(pname, 0, sizeof(pname));
		memset(original_panme, 0, sizeof(original_panme));
		memset(role_name, 0, sizeof(role_name));
	}

	int db_index;
	NetID netid;
	PlatName pname;
	PlatName original_panme;

	GameName role_name;
	char avatar;
	char sex;
	char prof;
	unsigned short level;
	int plat_spid;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void CreateRoleRet(int ret, int role_id);
	virtual void Error();
};


class RMIDestroyRoleBackObjectImpl : public RMIDestroyRoleBackObject
{
public:
	RMIDestroyRoleBackObjectImpl() : netid(-1)
	{
		memset(pname, 0, sizeof(pname));
	}

	UserID user_id;
	int netid;
	PlatName pname;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void DestroyRoleRet(int ret);
};

class RMILoginGetCmdBackObjectImpl : public RMIGetCmdBackObject
{
public:
	RMILoginGetCmdBackObjectImpl() : login_server(NULL) {}

	LoginServer *login_server;

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void GetCmdRet(int ret, const CmdList &cmd_list);
};

class RMILoginInitWorldStatusBackObjectImpl : public RMILoginInitWorldStatusBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void InitWorldStatusRet(int result, bool is_register_limit);
};

class RMILoginSaveWorldStatusBackObjectImpl : public RMILoginSaveWorldStatusBackObject
{
public:
	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	virtual void SaveWorldStatusRet(int result)
	{
		if (0 != result)
		{
			printf("RMILoginSaveWorldStatusBackObjectImpl SaveWorldStateRet ret:%d \n", result);
		}
	}
};

#endif

