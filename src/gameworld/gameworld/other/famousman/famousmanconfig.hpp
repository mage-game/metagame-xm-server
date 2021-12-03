#ifndef __FAMOUS_MAN_CONFIG_HPP__
#define __FAMOUS_MAN_CONFIG_HPP__

#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "other/achieve/title/titleconfig.hpp"

enum FAMOUNS_MAN_TYPE
{
	FAMOUSN_MAN_INVAILD = 0,

	FAMOUS_MAN_ROLE_LEVEL,		// 等级最先达到X级
	FAMOUS_MAN_MOUNT_GRADE,		// 坐骑最先达到X阶
	FAMOUS_MAN_WING_GRADE,		// 羽翼最先达到X阶
	FAMOUS_MAN_EQUIP_GRADE,		// 最先集齐一套X阶红色装备
	FAMOUS_MAN_TOWER_LEVEL,		// 勇者之塔最先通关X层
	FAMOUS_MAN_ACTIVE_XIANNV,	// 最先激活女神XXX

	FAMOUS_MAN_MAX_TYPE,
};

struct FamousManCfg
{
	FamousManCfg():famous_type(FAMOUSN_MAN_INVAILD), param_1(0), param_2(0){}

	int famous_type;
	int param_1;
	int param_2;
};

class FamousManConfig : public ConfigBase
{
public:

	FamousManConfig();
	~FamousManConfig();

	bool Init(const std::string &path, std::string *err);

	const FamousManCfg * GetFamousManCfg(int famous_type);

	int GetOpenTime() { return m_during_times; }

private:
	int InitFamousManCfg(PugiXmlNode RootElement);
	int InitOthernCfg(PugiXmlNode RootElement);

	FamousManCfg m_famous_man_cfg[FAMOUS_MAN_MAX_TYPE];
	int m_during_times;
};

#endif // __FAMOUS_MAN_CONFIG_HPP__

