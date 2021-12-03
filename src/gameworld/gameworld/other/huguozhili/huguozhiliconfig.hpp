#ifndef __HUGUOZHILI_CONFIG_H__
#define __HUGUOZHILI_CONFIG_H__

#include <map>
#include "servercommon/configcommon.h"

static const int ACTIVE_CFG_COUNT = 32;

// 其他
struct HuguozhiliOtherConfig
{
	HuguozhiliOtherConfig(): injure_maxhp_per(0),buff_interval_s(0), buff_cd_s(0)
	{};

	int injure_maxhp_per;			// 受伤上限（最大血量的百分比）
	int buff_interval_s;			// buff持续时间（秒）
	int buff_cd_s;					// buff冷却时间（秒，从激活时计算）
};


// 天神护体 - 激活
struct HuguozhiliActiveConfig
{
	HuguozhiliActiveConfig() : can_active_times(0), need_die_times(0)
	{};

	int can_active_times;			// 可兑换（激活）次数
	int need_die_times;				// 需要死亡次数
};

class HuguozhiliConfig : public ConfigBase
{
public:
	HuguozhiliConfig() {};
	~HuguozhiliConfig() {};

	bool Init(const std::string &configname, std::string *err);

	const HuguozhiliOtherConfig * GetOtherConfig() { return &m_other_cfg; }
	const HuguozhiliActiveConfig * GetActiveConfig(short exchange_times);
	int GetCanActiveTimes(int die_times);	// 获取可以激活的次数

private:
	int InitOtherCfg(TiXmlElement* root_element);
	int InitActiveCfg(TiXmlElement* root_element);

	HuguozhiliOtherConfig m_other_cfg;
	std::map<int, HuguozhiliActiveConfig> m_active_cfglist;
};


#endif // __HUGUOZHILI_CONFIG_H__