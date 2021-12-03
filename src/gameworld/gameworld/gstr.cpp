#include <stdarg.h>
#include <string>
#include <unordered_map>

#include "world.h"
#include "gstr.h"
#include "servercommon/configcommon.h"
#include "servercommon/userprotocal/msgsystem.h"

#include "gameworld/gameworld/engineadapter.h"

#include "obj/character/role.h"

namespace gstr
{
	char GSTR_BUF[2048] = { 0 };

	typedef std::unordered_map<std::string, std::string> StrContainer;

	StrContainer container_list[ContainerType_Max];

	std::string nofind_str = "";

	static const int BUFFER_LEN = 2048;
	char str_buffer[BUFFER_LEN] = {0};

	bool Init(const std::string &path, std::string *error)
	{
		char errinfo[1024] = { 0 }; 
		pugi::xml_document document; 
		pugi::xml_parse_result result = document.load_file(path.c_str());
		if (path == "" || pugi::status_ok != result.status)
		{
			sprintf(errinfo, "Load config [%s] Error.%s", path.c_str(), result.description());
			*error = errinfo;
			return false; 
		}
		const pugi::xml_node &root_element = document.child("gstr"); 
		if (root_element.empty())
		{
			sprintf(errinfo, "Load config [%s] Error.cannot find root node.", path.c_str());
			*error = errinfo;
			return false; 
		}

		const char *type_name_list[ContainerType_Max] = {"errors", "notices", "mailcontents", "sysmsg"};
		
		for (int i = 0; i < ContainerType_Max; ++i)
		{
			const char * type_name = type_name_list[i];
			StrContainer &container = container_list[i];
			const pugi::xml_node type_node = root_element.child(type_name);
			if (type_node.empty())
			{
				sprintf(errinfo, "Load config [%s] Error.cannot find type node : %s", path.c_str(), type_name);
				*error = errinfo;
				return false;
			}

			pugi::xml_node str_node = type_node.first_child();
			while (str_node)
			{
				std::string str;
				if (!PugiGetNodeValue(str_node, str))
				{
					sprintf(errinfo, "Load config [%s] Error.cannot get value at str node : %s", path.c_str(), str_node.name());
					*error = errinfo;
					return false;
				}

				container[str_node.name()] = str;
				str_node = str_node.next_sibling();
			}
		}

		return true;
	}

	bool Reload(const std::string &path, std::string *err)
	{
		for (int i = 0; i < ContainerType_Max; ++i)
		{
			StrContainer &container = container_list[i];
			container.clear();
		}

		return Init(path, err);
	}

	const std::string &GetStr(ContainerType type, const char *key)
	{
		if (type < 0 || type >= ContainerType_Max) return nofind_str;

		StrContainer &container = container_list[type];
		if (container.find(key) != container.end())
		{
			return container[key];
		}
		return nofind_str;
	}

	void SendHelper(GSNetID netid, const char *fmt, va_list va)
	{
		int str_len = vsnprintf(str_buffer, BUFFER_LEN, fmt, va);
		if (str_len >= BUFFER_LEN) str_len = -1;
		str_buffer[sizeof(str_buffer) - 1] = '\0';

		if (str_len > 0 && str_len <= PROTOCAL_MAX_SYSTEM_MSG_LENGTH)
		{
			static Protocol::SCSystemMsg sm;
			sm.send_time = (unsigned int)time(0);
			sm.msg_type = SYS_MSG_CENTER_PERSONAL_NOTICE;
			sm.msg_length = (short)str_len;
			memcpy(sm.msg, str_buffer, str_len);
			sm.color = 0;
			sm.display_pos = 0;

			int sendlen = sizeof(Protocol::SCSystemMsg) - PROTOCAL_MAX_SYSTEM_MSG_LENGTH + str_len;
			EngineAdapter::Instance().NetSend(netid, (const char *)&sm, sendlen);
		}
	}

	void SendError(Role * role, const char *key, ...)
	{
		const char *fmt = GetStr(ContainerType_Error, key).c_str();
		va_list va;
		va_start(va, key);
		SendHelper(role->GetNetId(), fmt, va);
		va_end(va);
	}


	void SendError(const UserID &user_id, const char *key, ...)
	{
		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
		if (nullptr == role)
		{
			return;
		}

		const char *fmt = GetStr(ContainerType_Error, key).c_str();
		va_list va;
		va_start(va, key);
		SendHelper(role->GetNetId(), fmt, va);
		va_end(va);
	}

	void SendNotice(Role *role, const char *key, ...)
	{
		const char *fmt = GetStr(ContainerType_Notice, key).c_str();
		va_list va;
		va_start(va, key);
		SendHelper(role->GetNetId(), fmt, va);
		va_end(va);
	}

	void SendNotice(const UserID &user_id, const char *key, ...)
	{
		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
		if (nullptr == role)
		{
			return;
		}

		const char *fmt = GetStr(ContainerType_Notice, key).c_str();
		va_list va;
		va_start(va, key);
		SendHelper(role->GetNetId(), fmt, va);
		va_end(va);
	}

	int MailContent(char *buffer, int len, const char *key, ...)
	{
		int str_len = 0;
		{
			const char *fmt = GetStr(ContainerType_MailContent, key).c_str();
			va_list va;
			va_start(va, key);
			str_len = vsnprintf(buffer, len, fmt, va);
			if (str_len >= len) str_len = -1;
			va_end(va);
			buffer[len - 1] = '\0';
		}

		return str_len;
	}

	int SysMsgContent(char *buffer, int len, const char *key, ...)
	{
		int str_len = 0;
		{
			const char *fmt = GetStr(ContainerType_SysMsg, key).c_str();
			va_list va;
			va_start(va, key);
			str_len = vsnprintf(buffer, len, fmt, va);
			if (str_len >= len) str_len = -1;
			va_end(va);
			buffer[len - 1] = '\0';
		}

		return str_len;
	}
}