#ifndef __ROLEEXPCONFIG_H__
#define __ROLEEXPCONFIG_H__

#include <string>

#include "gamedef.h"
#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"

/*
��ɫ���� roleexp.xml
��ɫת�� rolezhuansheng.xml
*/

struct RoleLevelCfg
{
	RoleLevelCfg() : max_exp(0), maxhp(0), maxmp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
	{}

	long long max_exp;						// �õȼ������ֵ

	Attribute maxhp;						// 
	Attribute maxmp;						// �õȼ������������ ����ֵ
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

class RoleLevelConfig
{
public:
	RoleLevelConfig();
	~RoleLevelConfig();

	bool Init(const std::string &path, std::string *err);

	long long GetRoleMaxExp(int level);

	RoleLevelCfg * GetLevelCfg(int level);

public:
	RoleLevelCfg m_rolelevelcfg_list[MAX_ROLE_LEVEL + 1];
};

struct RoleZhuanShengCfg
{
	RoleZhuanShengCfg() : zhuansheng_level(0), is_broadcast(0){}
	int zhuansheng_level;
	bool is_broadcast;

	ItemConfigData consume_item;
};

class RoleZhuanShengConfig
{
public:
	RoleZhuanShengConfig();
	~RoleZhuanShengConfig();

	bool Init(const std::string &path, std::string *err);
	RoleZhuanShengCfg * GetZhuanShengCfg(int role_level);

protected:
	RoleZhuanShengCfg m_zhuansheng_cfg_list[MAX_ZHUAN_SHENG_LEVEL + 1];
};

#endif // __ROLEEXPCONFIG_H__

