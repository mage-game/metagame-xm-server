//-------------------------------------------------------------------------------------------

// 直接从配置读取字符串资源，存入无序容器

//-------------------------------------------------------------------------------------------

#ifndef __GAMEWORLD_GSTR_H__
#define __GAMEWORLD_GSTR_H__
 
#include <string>

#include "servercommon/serverdef.h"

class Role;

enum ContainerType
{
	ContainerType_Error = 0,
	ContainerType_Notice,
	ContainerType_MailContent,
	ContainerType_SysMsg,

	ContainerType_Max
};

namespace gstr
{
	extern char GSTR_BUF[2048];

	bool Init(const std::string &path, std::string *error);
	bool Reload(const std::string &path, std::string *err);

	const std::string &GetStr(ContainerType type, const char *key);

	void SendError(const UserID &user_id, const char *key, ...);
	void SendError(Role *role, const char *key, ...);

	void SendNotice(const UserID &user_id, const char *key, ...);
	void SendNotice(Role *role, const char *key, ...);

	int MailContent(char *buffer, int len, const char *key, ...);
	int SysMsgContent(char *buffer, int len, const char *key, ...);
}

#endif