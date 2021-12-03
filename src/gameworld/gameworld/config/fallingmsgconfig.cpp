#include "fallingmsgconfig.hpp"

FallingMSGConfig::FallingMSGConfig()
{
	
}

FallingMSGConfig::~FallingMSGConfig()
{

}

bool FallingMSGConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "FallingMSGConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + "xml root node error.";
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("falling_msg");
		if (root_element.empty())
		{
			*err = path + "xml no falling_msg node.";
			return false;
		}

		iRet = this->InitFallingMSGConfig(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitFallingMSGConfig fail %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const FallingMSG * FallingMSGConfig::GetFallingMSG(short broadcast_type)
{
	if (falling_msg.find(broadcast_type) == falling_msg.end())
	{
		return nullptr;
	}
	return &falling_msg[broadcast_type];
}

int FallingMSGConfig::InitFallingMSGConfig(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	const char format[FALLING_ITEM_GAME_STRING_TYPE_MAX][10] = {"dsddd", "dsddds", "dsdddddd", "dsdddsddd"};
	while (!data_element.empty())
	{
		int broadcast_type;
		if (!PugiGetSubNodeValue(data_element, "broadcast_type", broadcast_type))
		{
			return -1;
		}
		FallingMSG cfg;
		if (!PugiGetSubNodeValue(data_element, "sys_msg_type", cfg.sys_msg_type) || cfg.sys_msg_type < 0 || cfg.sys_msg_type >= SYS_MSG_TYPE_MAX)
		{
			return -2;
		}
		if (!PugiGetSubNodeValue(data_element, "color", cfg.color) || cfg.color <= ItemBase::I_COLOR_INVALID || cfg.color >= ItemBase::I_COLOR_MAX)
		{
			return -3;
		}
		for (int i = 0; i < FALLING_ITEM_GAME_STRING_TYPE_MAX; ++i)
		{
			const std::string column_name = "game_string_" + std::to_string(i);
			if (!PugiGetSubNodeValue(data_element, column_name, cfg.game_string[i]) || cfg.game_string[i].size() == 0 || !this->CheckFmt(cfg.game_string[i].c_str(), format[i]))
			{
				return -100 - i;
			}
		}
		falling_msg[broadcast_type] = cfg;
		data_element = data_element.next_sibling();
	}

	return 0;
}

bool FallingMSGConfig::CheckFmt(const char *str, const char *fmt)
{
	const char *fmt_p = fmt;
	const char *str_p = str;
	while (*fmt_p)
	{
		while (*str_p && *str_p++ != '%');
		if (*str_p != *fmt_p++)
		{
			return false;
		}
	}
	return true;
}