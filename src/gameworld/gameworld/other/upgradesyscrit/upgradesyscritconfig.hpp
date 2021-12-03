#ifndef _UPGRADESYSCONFIG_HPP_
#define _UPGRADESYSCONFIG_HPP_

#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/configbase.h"
#include "item/itempool.h"
#include <map>

enum UPGRADE_SYS_CRIT_TYPE
{
	UPGRADE_SYS_CRIT_TYPE_INVALID = 0,
	UPGRADE_SYS_CRIT_TYPE_MOUNT = 1,					//坐骑
	UPGRADE_SYS_CRIT_TYPE_WING = 2,						//羽翼
	UPGRADE_SYS_CRIT_TYPE_SHIZHUANG = 3,				//时装
	UPGRADE_SYS_CRIT_TYPE_SHENBING = 4,					//神兵
	UPGRADE_SYS_CRIT_TYPE_FABAO = 5,					//法宝
	UPGRADE_SYS_CRIT_TYPE_FOOTPRINT = 6,				//足迹
	UPGRADE_SYS_CRIT_TYPE_HALO = 7,						//光环
	UPGRADE_SYS_CRIT_TYPE_FIGHTMOUNT = 8,				//战骑
	UPGRADE_SYS_CRIT_TYPE_TOUSHI = 9,					//头饰
	UPGRADE_SYS_CRIT_TYPE_MASK = 10,					//面饰
	UPGRADE_SYS_CRIT_TYPE_YAOSHI = 11,					//腰饰
	UPGRADE_SYS_CRIT_TYPE_QILINBI = 12,					//麒麟臂
	UPGRADE_SYS_CRIT_TYPE_LINGTONG = 13,				//灵童
	UPGRADE_SYS_CRIT_TYPE_LINGGONG = 14,				//灵弓
	UPGRADE_SYS_CRIT_TYPE_LINGQI = 15,					//灵骑
	UPGRADE_SYS_CRIT_TYPE_SHENGONG = 16,				//仙女光环
	UPGRADE_SYS_CRIT_TYPE_SHENYI = 17,					//仙女法阵
	UPGRADE_SYS_CRIT_TYPE_FLYPET = 18,					//飞宠
	UPGRADE_SYS_CRIT_TYPE_WEIYAN = 19,					//尾焰

	UPGRADE_SYS_CRIT_TYPE_END
};

const int MAX_UPGRADE_CRIT_TYPE_COUNT = 20;
UNSTD_STATIC_CHECK(MAX_UPGRADE_CRIT_TYPE_COUNT == UPGRADE_SYS_CRIT_TYPE_END)

struct UpgradeSysCritStruct
{
	UpgradeSysCritStruct() : upgrade_exp(0), crit_rate(0), crit_value(0) {}

	int upgrade_exp;											//进阶获得的经验
	int crit_rate;												//进阶暴击率,万分比
	int crit_value;												//暴击值
};

class UpgradeSysCritConfig : public ConfigBase
{
public:
	UpgradeSysCritConfig();
	~UpgradeSysCritConfig();

	bool Init(const std::string &configname, std::string *err);

	const UpgradeSysCritStruct* GetUpgradeSysCritConfigByType(int type);
	const int GetUpgradeSysCritRate(int type);
	const int GetUpgradeSysCritValue(int type);

private:
	
	int InitUpgradeSysCritConfig(PugiXmlNode RootElement);

	UpgradeSysCritStruct upgrade_sys_crit[MAX_UPGRADE_CRIT_TYPE_COUNT];
};

#endif /*_UPGRADESYSCONFIG_HPP_*/
