#ifndef __REALLYEQUIP_CONFIG_HPP__
#define __REALLYEQUIP_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/basedef.h"
#include <map>

#include "servercommon/struct/itemlistparam.h"

#include "item/itembase.h"

#include "obj/character/attribute.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

// 合成配置
struct ComposeListConfig
{
	ComposeListConfig() : itemid(ItemBase::INVALID_ITEM_ID), coin(0), bind(0)
	{
	}
	const static unsigned int COIN_BIND = 1;

	ItemID itemid;
	unsigned int coin;
	unsigned short bind;	// 1 绑定 0 非绑定
	unsigned short shuxing; // 真装备属性加成 n%
};
// 升级配置
struct LevelupListConfig
{
	LevelupListConfig() : itemid(ItemBase::INVALID_ITEM_ID), stuff_id_1(ItemBase::INVALID_ITEM_ID), stuff_count_1(0), stuff_id_2(ItemBase::INVALID_ITEM_ID), stuff_count_2(0)
	{
	}

	ItemID itemid;
	ItemID dirid;
	ItemID stuff_id_1;
	unsigned int stuff_count_1;
	ItemID stuff_id_2;
	unsigned int stuff_count_2;
};

// 其它配置
struct FeiXianOtherConfig
{
	unsigned short total_score_min;      // 最低总评分
	unsigned short total_score_max;      // 最高总评分
	unsigned short star_num;             // 对应星星数量(等级)
};

// 飞仙装备配置
class ReallyequipComposeConfig : public ConfigBase
{
public:
	ReallyequipComposeConfig();
	~ReallyequipComposeConfig();

	bool Init(const std::string &configname, std::string *err);

	const ComposeListConfig * GetCompostListCfg(unsigned int itemid);
	const LevelupListConfig * GetLevelupListCfg(unsigned int itemid);
	const FeiXianOtherConfig* GetOtherCfg(unsigned short score);

private:
	int InitComposeListConfig(PugiXmlNode RootElement);	// 合成列表配置
	int InitLevelUpConfig(PugiXmlNode RootElement);		// 升级列表配置
	int InitOtherConfig(PugiXmlNode RootElement);         // 其它列表配置

	std::map<int, ComposeListConfig> m_comlist_map;
	std::map<int, LevelupListConfig> m_levelup_map;
	std::vector<FeiXianOtherConfig> m_other_vec;                   // 存其它配置里的数据
};

#endif




