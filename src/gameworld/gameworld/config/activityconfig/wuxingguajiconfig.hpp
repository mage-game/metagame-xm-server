#ifndef _WUXINGGUAJI_CONFIG_HPP_
#define _WUXINGGUAJI_CONFIG_HPP_

#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

#include <vector>

struct WuXingGuaJiStuffCfg
{
	WuXingGuaJiStuffCfg() : stuff_itemid(0), need_count(0) {}

	int stuff_itemid;
	int need_count;
};

struct WuXingGuaJiTargetCfg
{
	WuXingGuaJiTargetCfg() {}

	int GetStuffIdx(int item_id)
	{
		for (int i = 0; i < WUXINGGUAJI_STUFF_MAX; i++)
		{
			if (need_stuff_list[i].stuff_itemid == item_id)
			{
				return i;
			}
		}

		return -1;
	}
		
	WuXingGuaJiStuffCfg need_stuff_list[WUXINGGUAJI_STUFF_MAX];
	ItemConfigData reward_cfg;
};

struct WuXingGuaJiRelaceBossCfg
{
	WuXingGuaJiRelaceBossCfg() : replace_monster_id(0), replace_need_world_level(0), flush_pos(0, 0) {}

	int replace_monster_id;
	int replace_need_world_level;
	Posi flush_pos;
};

struct WuXingGuaJiBossCfg
{
	WuXingGuaJiBossCfg() : replace_boss_count(0) {}

	int replace_boss_count;
	WuXingGuaJiRelaceBossCfg replace_boss_list[WUXINGGUAJI_MAX_RELACE_BOSS_NUM];
};

class WuXingGuaJiConfig : public ConfigBase
{
public:
	WuXingGuaJiConfig();
	~WuXingGuaJiConfig();

	bool Init(const std::string &configname, std::string *err);
	
	int GetOpenLevel() { return m_open_level; }
	int GetEnterInfo(Posi *enter_pos);

	bool GetBossCfg(int boss_index, int *monster_id, Posi *flush_pos);
	WuXingGuaJiTargetCfg * GetWuXingGuaJiTargetCfg(int target_idx);
	bool CheckTargetIsFinish(int target_idx, WuXingGuaJiTargetInfo *target_info);
	int GetLuckyInterval() { return m_lucky_interval; }
	const ItemConfigData & GetLuckyItem() { return m_lucky_item; }

private:
	int m_open_level;

	int m_sceneid;
	Posi m_enter_pos;

	int m_lucky_interval;
	ItemConfigData m_lucky_item;

	WuXingGuaJiTargetCfg m_target_cfglist[WUXINGGUAJI_TARGET_MAX];				// 目标配置
	WuXingGuaJiBossCfg m_boss_cfg_list[WUXINGGUAJI_BOSS_NUM];					// BOSS列表
};

#endif // WuXingGuaJiConfig



