#ifndef __DA_LOGIN_H__
#define __DA_LOGIN_H__

#include "damodule.h"

namespace dataaccessrmi
{
	namespace login
	{
		static const char *GET_PROF_NUM = "GetProfNum";
		static const char *USER_LOGIN = "UserLogin";
		static const char *ADD_ROLE = "AddRole";
		static const char *REMOVE_ROLE = "RemoveRole";
		static const char *FORBID = "Forbid";
		static const char *FORBID_ONE_ROLE = "ForbidOneRole";
		static const char *GET_ROLE = "GetRole";
		static const char *CHANGE_ROLE = "ChangeRole";
		static const char *ADD_NAME_INFO = "AddNameInfo";
		static const char *UPDATE_NAME_INFO = "UpdateNameInfo";
		static const char *DELETE_NAME_INFO = "DeleteNameInfo";
		static const char *DELETE_NAME_INFO_BY_NAME = "ByNameDeleteNameInfo";
		static const char *QUERY_NAME_EXIST = "QueryNameExist";
		static const char *RESET_ROLE_NAME = "ResetRoleName";
		static const char *INIT_WORLD_STATUS = "InitWorldstatus";
		static const char *SAVE_WORLD_STATUS = "SaveWorldstatus";
	}
}

#endif
