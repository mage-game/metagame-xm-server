#ifndef __FBCONFIG_HPP__
#define __FBCONFIG_HPP__

#include <map>
#include <set>

#include "servercommon/serverdef.h"
#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

static const int MAX_FB_NUM = 60;										// 副本数量
static const int FB_DAYCOUNT_RESET_MAX = 10;							// 副本计数最大可重置次数

class FBConfig : public ConfigBase
{
public:
	FBConfig();
	~FBConfig();

	bool Init(const std::string &configname, std::string *err);

	int GetFBDayCount(int daycount_id, int role_level, int *max_reset_count = NULL) const;
	bool GetFBDayCountResetCost(int daycount_id, int role_level, int reset_num, int *need_gold, int *need_coin) const;
	int GetFBDayCountIDBySceneId(int scene_id);

	bool IsDaycountIDExist(int daycount_id);
	
private:
	struct FBConfigItem
	{
		FBConfigItem() : daycount_id(-1), max_free_count(0), max_reset_count(0)
		{
			memset(reset_gold_list, 0, sizeof(reset_gold_list));
			memset(reset_coin_list, 0, sizeof(reset_coin_list));
		}

		bool Invalid() const { return daycount_id < 0; }
		
		int daycount_id;												// 计数ID
		short max_free_count;											// 最大免费次数
		short max_reset_count;											// 最大重置次数
		short reset_gold_list[FB_DAYCOUNT_RESET_MAX];					// 重置所需元宝
		short reset_coin_list[FB_DAYCOUNT_RESET_MAX];					// 重置所需铜币
	};
	
	FBConfigItem m_fb_list[MAX_FB_NUM];									// 所有副本计数器

	typedef std::map<int, int> SceneDayCounterIdMap;
	typedef std::map<int, int>::iterator SceneDayCounterIdMapIt;
	SceneDayCounterIdMap m_scene_daycount_map;							// 场景ID计数器ID对应表
};

#endif // __FBCONFIG_HPP__

