#ifndef __CROSS_VIP_BOSS_CONFIG_HPP__
#define __CROSS_VIP_BOSS_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"

struct CrossVipBossOtherCfg
{
	CrossVipBossOtherCfg() : is_open(false) {}

	bool is_open;															// 是否开放本功能-总开关
};

class CrossVipBossConfig
{
public:
	CrossVipBossConfig();
	~CrossVipBossConfig();

	static CrossVipBossConfig & Instance();
	bool Init(const std::string &path, std::string *err);

	const CrossVipBossOtherCfg & GetOtherCfg() { return m_other_cfg; }

private:
	int InitOtherCfg(TiXmlElement *RootElement);

	CrossVipBossOtherCfg m_other_cfg;
};

#endif // __CROSS_VIP_BOSS_CONFIG_HPP__

