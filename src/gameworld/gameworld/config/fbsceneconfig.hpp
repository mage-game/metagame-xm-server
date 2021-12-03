#ifndef __FB_SCENE_HPP__
#define __FB_SCENE_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "scene/scene.h"

struct FbSceneConfig
{
	FbSceneConfig():scene_type(0), pb_mount(0), pb_fightmount(0), view_double(0){}

	int scene_type;
	short pb_mount;
	short pb_fightmount;
	int view_double;
};

class FbSceneCfg : public ConfigBase
{
public:

	FbSceneCfg();
	~FbSceneCfg();

	bool Init(const std::string &configname, std::string *err);

	const FbSceneConfig * GetFbSceneCfg(int scene_type);

private:

	FbSceneConfig scene_cfg[Scene::SCENE_TYPE_COUNT];
};

#endif // __FB_SCENE_HPP__

