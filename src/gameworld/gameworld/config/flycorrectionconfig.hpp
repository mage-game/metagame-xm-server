#ifndef __FLY_CORRECTION_CONFIG_HPP__
#define __FLY_CORRECTION_CONFIG_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <map>

struct CorrectionSceneCfg
{
	CorrectionSceneCfg() : scene_id(0), correction_scene_id(0), correction_x(0), correction_y(0), ignore_camp(0) {}

	int scene_id;
	int correction_scene_id;
	int correction_x;
	int correction_y;
	int ignore_camp;
};

class FlyCorrectionConfig : public ConfigBase
{
public:
	FlyCorrectionConfig();
	~FlyCorrectionConfig();

	bool Init(const std::string &path, std::string *err);

	CorrectionSceneCfg * GetCorrectionSceneCfg(int scene_id);

private:
	std::map<int, CorrectionSceneCfg> m_correction_scene_cfg_map;
};

#endif // __FLY_CORRECTION_CONFIG_HPP__

