#include "servercommon/configcommon.h"
#include "fbsceneconfig.hpp"

FbSceneCfg::FbSceneCfg()
{
	
}

FbSceneCfg::~FbSceneCfg()
{

}

bool FbSceneCfg::Init(const std::string &configname, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "FbSceneCfg", *err);

	PugiXmlNode root_element = document.document_element();
	if (root_element.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode fb_element = root_element.child("fb_scene_cfg_list");
		if (fb_element.empty())
		{
			*err = configname + ": xml node error in [fb_scene_cfg_list].";
			return false;
		}

		PugiXmlNode data_element = fb_element.child("data");
		if (data_element.empty())
		{
			*err = configname + ": xml node error in [fb_scene_cfg_list->data] empty.";
			return false;
		}

		while (!data_element.empty())
		{
			int scene_type = -1;
			if (!PugiGetSubNodeValue(data_element, "scene_type", scene_type) || scene_type < 0 || scene_type >= Scene::SCENE_TYPE_COUNT)
			{
				*err = configname + ": xml node error in [fb_scene_cfg_list->data->scene_type].";
				return false;
			}
			
			FbSceneConfig &cfg = scene_cfg[scene_type];
			cfg.scene_type = scene_type;
	
			if (!PugiGetSubNodeValue(data_element, "pb_mount", cfg.pb_mount) || (cfg.pb_mount != 0 && cfg.pb_mount != 1))
			{
				*err = configname + ": xml node error in [fb_scene_cfg_list->data->pb_mount].";
				return false;
			}

			if (!PugiGetSubNodeValue(data_element, "pb_fightmount", cfg.pb_fightmount) || (cfg.pb_fightmount != 0 && cfg.pb_fightmount != 1))
			{
				*err = configname + ": xml node error in [fb_scene_cfg_list->data->pb_fightmount].";
				return false;
			}

			if (!PugiGetSubNodeValue(data_element, "view_double", cfg.view_double) || (cfg.view_double < 0))
			{
				*err = configname + ": xml node error in [fb_scene_cfg_list->data->view_double]. < 1";
				return false;
			}

			data_element = data_element.next_sibling();
		}
	}

	return true;
}

const FbSceneConfig * FbSceneCfg::GetFbSceneCfg(int scene_type)
{
	if (scene_type < 0 || scene_type >= Scene::SCENE_TYPE_COUNT)
		return NULL;

	return &scene_cfg[scene_type];
}

