#ifndef __SPACE_CONFIG_HPP__
#define __SPACE_CONFIG_HPP__

#include <map>
#include <string.h>
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class SpaceConfig : public ConfigBase
{
public:
	SpaceConfig();
	~SpaceConfig();

	bool Init(const std::string &path, std::string *err);

	int GetCaiAddRenqi() { return m_cai_addrenqi; }
	int GetCaiAddHuoli() { return m_cai_addhuoli; }
	int GetCaiDayHuoliLimit() { return m_cai_dayhuoli_limit; }

	int GetGiftAddRenqi() { return m_gift_addrenqi; }
	int GetGiftAddHuoLiZhi() { return m_gift_addhuoli; }
	int GetGiftCost() { return m_gift_cost; }

private:
	int InitOtherCfg(PugiXmlNode RootElement);

	int m_cai_addrenqi;													// 踩一踩 增加人气
	int m_cai_addhuoli;													// 踩一踩 增加活力
	int m_cai_dayhuoli_limit;											// 踩一踩 每日增加活力限制

	int m_gift_addrenqi;												// 赠礼 增加人气
	int m_gift_addhuoli;												// 赠礼 增加活力值
	int m_gift_cost;													// 赠礼 花费
};

#endif // __SPACE_CONFIG_HPP__






