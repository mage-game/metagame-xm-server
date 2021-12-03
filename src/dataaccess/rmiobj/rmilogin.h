
#ifndef RMILOGIN_H
#define RMILOGIN_H

#include "rmi/rmibase.h"
#include "servercommon/basedef.h"
#include "servercommon/logindef.h"

class RMILoginObject : public rmi::RMIObject
{
public:
	RMILoginObject();
	virtual ~RMILoginObject();

	virtual void __free();
	virtual const char * __get_obj_id();

	static int __GetProfNum(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int GetProfNum(int *prof1_num, int *prof2_num, int *prof3_num, int *prof4_num);

	static int __UserLogin(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int UserLogin(const char *pname, int *db_index, int *role_id_1, int *role_id_2, int *role_id_3, char can_insert);

	static int __AddRole(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int AddRole(const char *pname, int db_index, int role_id);

	static int __RemoveRole(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int RemoveRole(const char *pname, int db_index, int role_id);

	// ·âºÅ
	static int __Forbid(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int Forbid(const char *pname, unsigned int forbid_time, int *db_index, int *role_id_1, int *role_id_2, int *role_id_3);
	static int ChangeRoleLastSaveTime(int role_id, long long save_time);

	// ·âµ¥½ÇÉ«
	static int __ForbidOneRole(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int ForbidOneRole(const char *pname, unsigned int forbid_time, int forbid_role_id, int *db_index);

	static int __GetGold(TLVUnserializer &in_param, TLVSerializer *out_param);
	static Int64 GetGold(const char *pname, int role_id, int *authority_type);

	static int __ChangeGold(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int ChangeGold(const char *pname, int role_id, Int64 gold_delta, Int64 *new_gold);

	static int __AddNameInfo(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int AddName(const char *role_name, const char *pname);

	static int __UpdateNameInfo(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int UpdateNameInfo(const char *role_name, int db_index, int role_id);

	static int __DeleteNameInfo(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int DeleteNameInfo(int db_index, int role_id, const char *pname);

	static int __DeleteNameInfoByName(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int DeleteNameInfoByName(const char *role_name);

	static int __AddIdentity(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int AddIdentity(const char *pname, const char *name, const char *id, bool check_repeat);

	static int __QueryNameExist(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int QueryNameExist(const char *role_name);

	static int __ResetRoleName(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int ResetRoleName(int db_index, int role_id, const char *role_name);

	static int __InitWorldstatus(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int InitWorldstatus(int *is_register_limit);

	static int __SaveWorldstatus(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SaveWorldstatus(int is_register_limit);
};

#endif
