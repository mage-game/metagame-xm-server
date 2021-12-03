#ifndef __MITAMA_CONFIG_HPP__
#define __MITAMA_CONFIG_HPP__

#include "common/tinyxml/tinyxml.h"	

#include "servercommon/serverdef.h"	
#include "servercommon/mitamadef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include "item/itembase.h"

struct MitamaOtherConfig
{
	MitamaOtherConfig() {}
};

struct MitamaLevelConfig
{
	static const int MAX_CONSUME_ITEM_COUNT = 3;

	struct LevelConfig
	{
		LevelConfig() : level(0), consume_item_count(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0) {}

		int level;

		int consume_item_count;
		ItemConfigData consume_item_list[MAX_CONSUME_ITEM_COUNT];

		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
	};

	MitamaLevelConfig() : seq(0), max_level(-1) {}

	int seq;
	
	int max_level;
	LevelConfig level_cfg_list[MITAMA_MAX_LEVEL + 1];
};

struct MitamaTaskInfoConfig
{
	MitamaTaskInfoConfig() : seq(0), level(0), need_time(0), reward_item_count(0) {}

	int seq;
	int level;
	int need_time;		// 任务所需时间（分钟）
	int reward_item_count;
};

struct MitamaTaskRewardConfig
{
	static const int MAX_REWARD_ITEM_COUNT = 100;

	struct RewardInfo
	{
		RewardInfo() : index(0), weight(0) {}

		int index;

		ItemConfigData reward_item;
		int weight;
	};

	MitamaTaskRewardConfig() : seq(0), reward_count(0) {}

	int seq;
	
	int reward_count;
	RewardInfo reward_info_list[MAX_REWARD_ITEM_COUNT];
};

struct MitamaExchangeItemCfg
{
	MitamaExchangeItemCfg() : seq(0), need_score(0) {}

	int seq;

	ItemConfigData exchange_item;
	int need_score;
};

class MitamaConfig : public ConfigBase
{
public:
	MitamaConfig();
	~MitamaConfig();

	bool Init(const std::string &configname, std::string *err);

	const MitamaOtherConfig & GetOtherCfg() { return m_other_cfg; }
	const MitamaLevelConfig::LevelConfig * GetLevelCfg(int seq, int level);
	const MitamaTaskInfoConfig * GetTaskInfoCfg(int seq);
	const MitamaTaskRewardConfig::RewardInfo * GetRandomTaskRewardCfg(int seq);
	const MitamaExchangeItemCfg * GetExchangeItemCfg(int seq);

	int GetMaxMitamaCount() { return m_max_mitama_count; }
	int GetMaxTaskCount() { return m_max_task_count; }

protected:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitTaskInfoCfg(PugiXmlNode RootElement);
	int InitTaskRewardCfg(PugiXmlNode RootElement);
	int InitExchangeItemCfg(PugiXmlNode RootElement);

private:
	MitamaOtherConfig m_other_cfg;

	int m_max_mitama_count;
	MitamaLevelConfig m_level_cfg_list[MITAMA_MAX_MITAMA_COUNT];

	int m_max_task_count;
	MitamaTaskInfoConfig m_task_info_cfg_list[MITAMA_MAX_TASK_COUNT];
	MitamaTaskRewardConfig m_task_reward_cfg_list[MITAMA_MAX_TASK_COUNT];

	int m_max_exchange_item_count;
	MitamaExchangeItemCfg m_exchange_item_cfg_list[MITAMA_MAX_EXCHANGE_ITEM_COUNT];
};

#endif
