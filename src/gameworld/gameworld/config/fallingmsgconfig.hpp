#ifndef __FALLING_MSG_CONFIG_HPP__
#define __FALLING_MSG_CONFIG_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/maildef.hpp"
#include <vector>


enum FALLING_ITEM_BROADCAST_TYPE
{
	FALLING_ITEM_BROADCAST_TYPE_INVALID = -1,

	FALLING_ITEM_BROADCAST_TYPE_NO_BROADCAST = 0,		//不广播
	FALLING_ITEM_BROADCAST_TYPE_NORMAL = 1,				//自定义广播,大于1也是自定义广播，由策划配

	FALLING_ITEM_BROADCAST_TYPE_MAX
};

enum FALLING_ITEM_GAME_STRING_TYPE
{
	FALLING_ITEM_GAME_STRING_TYPE_INVALID = -1,
	
	FALLING_ITEM_GAME_STRING_TYPE_NORMAL = 0,			//普通
	FALLING_ITEM_GAME_STRING_TYPE_EQUIP = 1,			//装备
	FALLING_ITEM_GAME_STRING_TYPE_NORMAL_FROM_BODY = 2, //普通，来自残骸
	FALLING_ITEM_GAME_STRING_TYPE_EQUIP_FROM_BODY = 3,	//装备，来自残骸

	FALLING_ITEM_GAME_STRING_TYPE_MAX					//在这个枚举插入类型时，记得搜所有引用，修改格式检查的char数组
};

struct FallingMSG
{
	FallingMSG() : sys_msg_type(0), color(0) {}

	short sys_msg_type;
	short color;
	std::string game_string[FALLING_ITEM_GAME_STRING_TYPE_MAX];
};

class FallingMSGConfig : public ConfigBase
{
public:
	FallingMSGConfig();
	~FallingMSGConfig();

	bool Init(const std::string &path, std::string *err);
	const FallingMSG * GetFallingMSG(short broadcast_type);
	
private:
	int InitFallingMSGConfig(PugiXmlNode RootElement);
	bool CheckFmt(const char * str, const char * fmt);
	std::map<int, FallingMSG> falling_msg;
};

#endif // __FALLING_MSG_CONFIG_HPP__

