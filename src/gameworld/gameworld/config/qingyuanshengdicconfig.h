#ifndef __QINGYUANSHENGDICONFIG_H__
#define __QINGYUANSHENGDICONFIG_H__

#include "gamedef.h"
#include "servercommon/configcommon.h"
#include "servercommon/configbase.h"
#include "servercommon/maildef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <vector>
#include <map>

struct QYSDOtherCfg
{
	QYSDOtherCfg() : male_item_count(0), female_item_count(0), skip_task_consume(0), skip_task_limit_level(0){}

	int male_item_count;
	ItemConfigData male_reward_item_list[MAX_ATTACHMENT_ITEM_NUM];

	int female_item_count;
	ItemConfigData female_reward_item_list[MAX_ATTACHMENT_ITEM_NUM];

	int skip_task_consume;
	int skip_task_limit_level;
};

struct QYSDLayerCfg
{
	QYSDLayerCfg() : layer(0), scene_id(0), birth_pos(0, 0), enter_level(0), begin_time(0), end_time(0) {}

	struct MonsterItem
	{
		MonsterItem() : monster_id(0), monster_count(0), refresh_time(0), pos_type(0) {}
		int monster_id;
		int monster_count;
		int refresh_time;
		int pos_type;
	};

	struct GatherItem
	{
		GatherItem() : gather_id(0), gather_count(0), gather_time(0), refresh_time(0), pos_type(0) {}
		int gather_id;
		int gather_count;
		int gather_time;
		int refresh_time;
		int pos_type;
	};

	int layer;
	int scene_id;
	Posi birth_pos;
	int enter_level;
	int begin_time;
	int end_time;

	std::vector<MonsterItem> monster_vec;
	std::vector<GatherItem> gather_vec;
};

enum QYSD_TASK_TYPE
{
	QYSD_TASK_TYPE_NONE = 0,
	QYSD_TASK_TYPE_MONSTER,
	QYSD_TASK_TYPE_JINGYING_MONSTER,
	QYSD_TASK_TYPE_BOSS,
	QYSD_TASK_TYPE_MONSTER_ID,
	QYSD_TASK_TYPE_GATHER,
	QYSD_TASK_TYPE_GATHER_ID,
};

struct QYSDTaskCfg
{
	QYSDTaskCfg() : task_id(0), level_min(0), level_max(0), task_type(0), param1(0), param2(0), reward_exp(0), male_item_count(0), female_item_count(0) {}

	int task_id;
	int	level_min;
	int	level_max;
	int task_type;
	int param1;
	int param2;
	long long reward_exp;

	int male_item_count;
	ItemConfigData male_reward_item_list[MAX_ATTACHMENT_ITEM_NUM];

	int female_item_count;
	ItemConfigData female_reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

class QingYuanShengDiConfig : public ConfigBase
{
public:
	QingYuanShengDiConfig();
	~QingYuanShengDiConfig();

	bool Init(const std::string &configname, std::string *err);

	const QYSDOtherCfg * GetQYSDOtherCfg() { return &m_other_cfg; }
	const QYSDLayerCfg * GetQYSDLayerCfg(int layer);
	int GetCanEnterMaxLayer(int marry_level);
	const std::vector<Posi> * GetQYSDPosCfg(int pos_type);
	const QYSDTaskCfg * GetQYSDTaskCfg(int task_id);
	int GetQYSDTaskList(int marry_level, int *task_id_list, int task_id_count);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitLayerCfg(PugiXmlNode RootElement);
	int InitMonsterCfg(PugiXmlNode RootElement);
	int InitGatherCfg(PugiXmlNode RootElement);
	int InitPosCfg(PugiXmlNode RootElement);
	int InitTaskCfg(PugiXmlNode RootElement);

	QYSDOtherCfg m_other_cfg;
	std::map<int, QYSDLayerCfg> m_layer_cfg_map;
	std::map<int, std::vector<Posi> > m_pos_map;
	std::map<int, QYSDTaskCfg> m_task_cfg_map;
};
#endif
