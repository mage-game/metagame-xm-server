#include "flycorrectionconfig.hpp"

#include "servercommon/configcommon.h"
#include "servercommon/campdef.hpp"

#include "scene/sceneregister/sceneregister.hpp"

FlyCorrectionConfig::FlyCorrectionConfig()
{
}

FlyCorrectionConfig::~FlyCorrectionConfig()
{
}

bool FlyCorrectionConfig::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "FlyCorrectionConfig", *err);

	PugiXmlNode root_element = document.document_element();
	if (root_element.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	PugiXmlNode flycorrection_element = root_element.child("flycorrection");
	if (flycorrection_element.empty())
	{
		*err = path + ": xml node error in [config->flycorrection]";
		return false;
	}

	PugiXmlNode data_element = flycorrection_element.child("data");
	while (!data_element.empty())
	{
		int scene_id = 0;
		if (!PugiGetSubNodeValue(data_element, "scene_id", scene_id) || !SceneRegister::Instance().CheckSceneIsExist(scene_id))
		{
			*err = path + ": xml node error in [config->flycorrection->data->scene_id]";
			return false;
		}

		CorrectionSceneCfg &config = m_correction_scene_cfg_map[scene_id];
		if (0 != config.scene_id)
		{
			*err = path + ": xml node error in [config->flycorrection->data->scene_id] repeat";
			return false;
		}
		config.scene_id = scene_id;

		if (!PugiGetSubNodeValue(data_element, "correction_scene_id", config.correction_scene_id) || !SceneRegister::Instance().CheckSceneIsExist(config.correction_scene_id))
		{
			*err = path + ": xml node error in [config->flycorrection->data->correction_scene_id]";
			return false;
		}

		if (!PugiGetSubNodeValue(data_element, "correction_x", config.correction_x) || config.correction_x <= 0)
		{
			*err = path + ": xml node error in [config->flycorrection->data->correction_x]";
			return false;
		}

		if (!PugiGetSubNodeValue(data_element, "correction_y", config.correction_y) || config.correction_y <= 0)
		{
			*err = path + ": xml node error in [config->flycorrection->data->correction_y]";
			return false;
		}

		if (!PugiGetSubNodeValue(data_element, "ignore_camp", config.ignore_camp) || config.ignore_camp < 0 || config.ignore_camp >= CAMP_TYPE_MAX)
		{
			*err = path + ": xml node error in [config->flycorrection->data->ignore_camp]";
			return false;
		}

		data_element = data_element.next_sibling();
	}

	return true;
}

CorrectionSceneCfg * FlyCorrectionConfig::GetCorrectionSceneCfg(int scene_id)
{
	std::map<int, CorrectionSceneCfg>::iterator it = m_correction_scene_cfg_map.find(scene_id);
	if (it != m_correction_scene_cfg_map.end())
	{
		return &it->second;
	}

	return NULL;
}

