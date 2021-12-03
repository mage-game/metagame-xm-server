#ifndef __RMISYSTEM_H__
#define __RMISYSTEM_H__

#include "rmi/rmibase.h"

class PublicNoticeList;
class CmdList;
class IStatement;
class DataAdapter;

class RMISystemObject : public rmi::RMIObject
{
public:
	RMISystemObject();
	virtual ~RMISystemObject();

	virtual void __free();
	virtual const char * __get_obj_id();

	static int __GetPublicNotice(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int GetPublicNotice(PublicNoticeList *notice_list);

	static int __GetCmd(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int GetCmd(char type, CmdList *cmd_list);

	static int __SwapRole(TLVUnserializer &in_param, TLVSerializer *out_param);
	static int SwapRole(int role_id1, int role_id2);
private:
	static int GetRolePlatName(IStatement* stmt, int role_id, std::string& out_platname);
	static int GetPlatNameLoginInfo(IStatement* stmt, const std::string& in_platname, DataAdapter* out_da);
	static int SwapLoginRole(IStatement* stmt,const std::string& platname1, int role_id1, const std::string& platname2, int role_id2);
	static int SwapRoleNameMap(IStatement* stmt,const std::string& platname1, int role_id1, const std::string& platname2, int role_id2);
	static int UpdateRoleNameMap(IStatement* stmt,const std::string& platname, int role_id);
	static int SwapRoleAccountGold(IStatement* stmt,const std::string& platname1, int role_id1, const std::string& platname2, int role_id2);
	static int UpdateAccountGold(IStatement* stmt,const std::string& platname, int role_id);
};

#endif
