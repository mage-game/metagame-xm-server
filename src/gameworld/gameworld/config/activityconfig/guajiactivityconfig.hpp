#ifndef __GUAJIACTIVITY_CONFIG_HPP__
#define __GUAJIACTIVITY_CONFIG_HPP__

#include <set>

#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

static const int GUAJI_SCENE_ID_MAX = 8;

class GuaJiActivityConfig : public ConfigBase
{
public:
	GuaJiActivityConfig();
	~GuaJiActivityConfig();

	bool Init(const std::string &path, std::string *err);

	bool IsGuaJiSceneID(int scene_id);
	
	int GetRewardInterval() {return m_reward_interval; }
	int GetGuaJiSceneIdCount() {return m_scene_id_count; }
	int GetGuaJiSceneIdByIndex(int index);
	int GetExtraTimes() { return m_reward_times;}

private:

	int InitOtherCfg(PugiXmlNode RootElement);
	int InitGuaJiSceneIdCfg(PugiXmlNode RootElement);
	
	int m_reward_interval;
	int m_reward_times;
	int m_scene_id_count;
	int m_scene_id_list[GUAJI_SCENE_ID_MAX];

	std::set<int> m_scene_id_set;
};

#endif // __GUAJIACTIVITY_CONFIG_HPP__

