#ifndef SHENGXINGZHULI_CONFIG_HPP
#define	SHENGXINGZHULI_CONFIG_HPP

#include "item/itembase.h"
#include "servercommon/configcommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
/*
就是玩家每天冲到一定数量的钱后，
*/

static const int CHONGZHI_MAX_COUNT = 100;						// 充值档位配置个数

enum SHENGXINGZHULI_SYSTEM_TYPE
{
	SHENGXINGZHULI_SYSTEM_TYPE_BEGIN = 0,
	SHENGXINGZHULI_SYSTEM_TYPE_MOUNT = SHENGXINGZHULI_SYSTEM_TYPE_BEGIN,			// 坐骑系统
	SHENGXINGZHULI_SYSTEM_TYPE_WING = 1,			// 翅膀
	SHENGXINGZHULI_SYSTEM_TYPE_SHENGONG = 2,		// 神弓
	SHENGXINGZHULI_SYSTEM_TYPE_HALO = 3,			// 光环
	SHENGXINGZHULI_SYSTEM_TYPE_SHENYI = 4,			// 神翼
	SHENGXINGZHULI_SYSTEM_TYPE_FIGHT_MOUNT = 5,		// 战骑
	SHENGXINGZHULI_SYSTEM_TYPE_FOOT_PRINT = 6,		// 足迹

	SHENGXINGZHULI_SYSTEM_TYPE_COUNT,
};

static const int NEED_CHONGZHI_FIRST_COUNT = 3;
struct ShengxingzhuliOtherCfg
{
	ShengxingzhuliOtherCfg() : open_day(0), add_value(0)
	{
		memset(need_chongzhi, 0, sizeof(need_chongzhi));
	}

	int need_chongzhi[NEED_CHONGZHI_FIRST_COUNT];				// 当日需要冲值的钱
	int open_day;					// 开服第几天开启
	int add_value;					// 无限增值量
	ItemConfigData reward_item_list[SHENGXINGZHULI_SYSTEM_TYPE_COUNT];		// 对应功能满阶之后送的道具
};


struct ShengxingzhuliWeekdaySystem
{
	ShengxingzhuliWeekdaySystem() : monday_sys(0), tuesday_sys(0), wednesday_sys(0), thursday_sys(0), friday_sys(0), saturday_sys(0), sunday_sys(0)
	{
	}
	int monday_sys;					// 礼拜一对应系统
	int tuesday_sys;				// 礼拜二对应系统
	int wednesday_sys;				// 礼拜三对应系统
	int thursday_sys;				// 礼拜四对应系统
	int friday_sys;					// 礼拜五对应系统
	int saturday_sys;				// 礼拜六对应系统
	int sunday_sys;					// 礼拜日对应系统
};

class ShengxingzhuliConfig : public ConfigBase
{
public:
	ShengxingzhuliConfig();
	bool Init(const std::string &configname, std::string *err);

	const ShengxingzhuliOtherCfg& GetShengxingzhuliOtherCfg() { return m_other_cfg; }
	const ShengxingzhuliWeekdaySystem * GetShengxingzhuliWeekdaySystemCfg() { return &m_weekday_system_cfg; }

	const int GetStageByChongzhi(int chongzhi);
	const int GetNeedChongZhiByStage(int stage);

private:
	ShengxingzhuliWeekdaySystem m_weekday_system_cfg;
	ShengxingzhuliOtherCfg m_other_cfg;
	int InitShengxingzhuliOtherConfig(PugiXmlNode RootElement);
	int InitShengxingzhuliWeekdaySystemConfig(PugiXmlNode RootElement);
};


#endif