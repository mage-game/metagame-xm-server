#ifndef __FB_GUIDE_CONFIG__
#define __FB_GUIDE_CONFIG__

#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/maildef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "monster/monsterpool.h"
#include <map>
#include "monster/drop/droppool.hpp"

struct FbGuideCfg
{
	FbGuideCfg(): type(0), task_id(0), scene_id(0), is_target_type(0), target_type(0), enter_pos(0, 0){}
	
	int type;
	int task_id;
	int scene_id;
	int is_target_type;
	int target_type;
	std::vector<UInt16> drop_id_list1;
	std::vector<UInt16> drop_id_list2;
	std::vector<UInt16> drop_id_list3;
	std::vector<UInt16> gather_id_list;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	Posi enter_pos;
};

class FbGuideConfig : public ConfigBase
{
public:
	FbGuideConfig();
	~FbGuideConfig();

	bool Init(const std::string &configname, std::string *err);

	const FbGuideCfg * GetFbGuideCfgByTaskId(int type);
	const FbGuideCfg * GetFbGuideCfgBySceneId(int scene_id);
	bool IsLawFulGatherId(int scene_id, int gather_id);

private:

	int InitFbGuideCfg(PugiXmlNode RootElement);
	std::map<int, FbGuideCfg> m_fb_guide_cfg_map;
};

#endif
