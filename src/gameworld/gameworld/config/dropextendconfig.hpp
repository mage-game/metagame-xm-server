#ifndef __DROPEXTEND_CONFIG_HPP__
#define __DROPEXTEND_CONFIG_HPP__

#include <map>
#include <string.h>
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

static const int EXTRA_DROP_MAX_NUM = 20;
static const int EXTRA_DROP_COLLECT_CUT_MAX_NUM = 20;

struct ExtraDropItemCfg
{
	ExtraDropItemCfg()
	{
		monster_min_level = 0;
		monster_max_level = 0;
		drop_count = 0;
		memset(dropid_list, 0, sizeof(dropid_list));
	}

	int monster_min_level;
	int monster_max_level;
	
	int drop_count;
	int dropid_list[EXTRA_DROP_MAX_NUM];
};

struct DropExtendOtherCfg
{
	DropExtendOtherCfg() : extradrop_time_gap(0), extradrop_distance(0) {}

	unsigned int extradrop_time_gap;
	unsigned int extradrop_distance;
};

struct DropExtendCollectCutDownCfg
{
	DropExtendCollectCutDownCfg() : drop_num(0), drop_per(0){}

	int drop_num;
	int drop_per;
};

class DropExtendConfig : public ConfigBase
{
public:
	DropExtendConfig();
	~DropExtendConfig();

	bool Init(const std::string &path, std::string *err);

	int GetRandomExtraDropId(int monster_level);
	int GetRandomCollectExtraDropId(int monster_level);
	const DropExtendOtherCfg *GetOtherCfg() { return &m_other_cfg; }
	int GetRandomCollectCutPer(int drop_num);
	const DropExtendCollectCutDownCfg *GetRandomCollectCutCfg(int drop_num);

private:
	int InitExtraDropConfig(PugiXmlNode RootElement);
	int InitCollectExtraDropConfig(PugiXmlNode RootElement);
	int InitOtherConfig(PugiXmlNode RootElement);
	int InitCollectCutConfig(PugiXmlNode RootElement);

	typedef std::vector<ExtraDropItemCfg> ExtraDropVec;
	typedef std::vector<ExtraDropItemCfg> CollectExtraDropVec;
	ExtraDropVec m_extra_drop_vec;
	CollectExtraDropVec m_collect_extra_drop_vec;
	DropExtendOtherCfg m_other_cfg;

	int m_collect_drop_cut_count;
	DropExtendCollectCutDownCfg m_collect_drop_cut_list[EXTRA_DROP_COLLECT_CUT_MAX_NUM];
};

#endif