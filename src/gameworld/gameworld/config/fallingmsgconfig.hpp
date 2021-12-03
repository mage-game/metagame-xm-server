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

	FALLING_ITEM_BROADCAST_TYPE_NO_BROADCAST = 0,		//���㲥
	FALLING_ITEM_BROADCAST_TYPE_NORMAL = 1,				//�Զ���㲥,����1Ҳ���Զ���㲥���ɲ߻���

	FALLING_ITEM_BROADCAST_TYPE_MAX
};

enum FALLING_ITEM_GAME_STRING_TYPE
{
	FALLING_ITEM_GAME_STRING_TYPE_INVALID = -1,
	
	FALLING_ITEM_GAME_STRING_TYPE_NORMAL = 0,			//��ͨ
	FALLING_ITEM_GAME_STRING_TYPE_EQUIP = 1,			//װ��
	FALLING_ITEM_GAME_STRING_TYPE_NORMAL_FROM_BODY = 2, //��ͨ�����Բк�
	FALLING_ITEM_GAME_STRING_TYPE_EQUIP_FROM_BODY = 3,	//װ�������Բк�

	FALLING_ITEM_GAME_STRING_TYPE_MAX					//�����ö�ٲ�������ʱ���ǵ����������ã��޸ĸ�ʽ����char����
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

