#include "crosscommonconfig.hpp"
#include "servercommon/configcommon.h"

CrossCommonConfig::CrossCommonConfig()
{

}

CrossCommonConfig::~CrossCommonConfig()
{

}

CrossCommonConfig & CrossCommonConfig::Instance()
{
	static CrossCommonConfig ccc;
	return ccc;
}

bool CrossCommonConfig::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossCommonConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode WaitSceneListElement = RootElement.child("wait_scene_cfg_list");
		if (WaitSceneListElement.empty())
		{
			*err = path + ": xml node error in [config->wait_scene_cfg_list]";
			return false;
		}

		PugiXmlNode WaitSceneCfgElement = WaitSceneListElement.child("wait_scene_cfg");
		while (!WaitSceneCfgElement.empty())
		{
			int cross_activity_type = CROSS_ACTIVITY_TYPE_INVALID;
			if (!PugiGetSubNodeValue(WaitSceneCfgElement, "cross_activity_type", cross_activity_type) || cross_activity_type <= CROSS_ACTIVITY_TYPE_INVALID || cross_activity_type >= CROSS_ACTIVITY_TYPE_RESERVED)
			{
				*err = path + ": xml node error in [config->wait_scene_cfg_list->wait_scene_cfg->cross_activity_type]";
				return false;
			}

			int cross_activity_type_index = cross_activity_type - CROSS_ACTIVITY_TYPE_INVALID;
			if (cross_activity_type_index <= 0 || cross_activity_type_index >= CROSS_ACTIVITY_TYPE_MAX)
			{
				*err = path + ": xml node error in [config->wait_scene_cfg_list->wait_scene_cfg->cross_activity_type]";
				return false;
			}

			CrossActivityWaitSceneCfg &wait_scene_cfg = m_wait_scene_cfg_list[cross_activity_type_index];
			if (!wait_scene_cfg.Invalid())
			{
				*err = path + ": xml node error in [config->wait_scene_cfg_list->wait_scene_cfg->cross_activity_type], repeat";
				return false;
			}

			wait_scene_cfg.wait_scene_count = 0;

			PugiXmlNode WaitSceneElement = WaitSceneCfgElement.child("wait_scene");
			if (WaitSceneElement.empty())
			{
				*err = path + ": xml node error in [config->wait_scene_cfg_list->wait_scene_cfg->wait_scene]";
				return false;
			}

			while (!WaitSceneElement.empty())
			{
				if (wait_scene_cfg.wait_scene_count >= WAIT_SCENE_MAX)
				{
					*err = path + ": xml node error in [config->wait_scene_cfg_list->wait_scene_cfg->wait_scene], too many";
					return false;
				}

				CrossActivityWaitScene &wait_scene = wait_scene_cfg.wait_scene_list[wait_scene_cfg.wait_scene_count];

				if (!PugiGetSubNodeValue(WaitSceneElement, "scene_id", wait_scene.scene_id) || wait_scene.scene_id <= 0)
				{
					*err = path + ": xml node error in [config->wait_scene_cfg_list->wait_scene_cfg->wait_scene->scene_id]";
					return false;
				}

				m_wait_scene_id_set.insert(wait_scene.scene_id);

				PugiXmlNode PosListElement = WaitSceneElement.child("enter_pos_list");
				if (PosListElement.empty())
				{
					*err = path + ": xml node error in [config->wait_scene_cfg_list->wait_scene_cfg->wait_scene->enter_pos_list]";
					return false;
				}

				wait_scene.enter_pos_count = 0;

				PugiXmlNode PosElement = PosListElement.child("enter_pos");
				if (PosElement.empty())
				{
					*err = path + ": xml node error in [config->wait_scene_cfg_list->wait_scene_cfg->wait_scene->enter_pos_list->enter_pos]";
					return false;
				}

				while (!PosElement.empty())
				{
					if (wait_scene.enter_pos_count >= WAIT_SCENE_ENTER_POS_MAX)
					{
						*err = path + ": xml node error in [config->wait_scene_cfg_list->wait_scene_cfg->wait_scene->enter_pos_list->enter_pos], too many";
						return false;
					}

					if (!PugiGetSubNodeValue(PosElement, "x", wait_scene.enter_pos_list[wait_scene.enter_pos_count].x) || wait_scene.enter_pos_list[wait_scene.enter_pos_count].x <= 0)
					{
						*err = path + ": xml node error in [config->wait_scene_cfg_list->wait_scene_cfg->wait_scene->enter_pos_list->enter_pos->x]";
						return false;
					}
					if (!PugiGetSubNodeValue(PosElement, "y", wait_scene.enter_pos_list[wait_scene.enter_pos_count].y) || wait_scene.enter_pos_list[wait_scene.enter_pos_count].y <= 0)
					{
						*err = path + ": xml node error in [config->wait_scene_cfg_list->wait_scene_cfg->wait_scene->enter_pos_list->enter_pos->y]";
						return false;
					}
					if (!PugiGetSubNodeValue(PosElement, "rand_dis", wait_scene.rand_dis_list[wait_scene.enter_pos_count]) || wait_scene.rand_dis_list[wait_scene.enter_pos_count] <= 0)
					{
						*err = path + ": xml node error in [config->wait_scene_cfg_list->wait_scene_cfg->wait_scene->enter_pos_list->enter_pos->rand_dis]";
						return false;
					}

					++ wait_scene.enter_pos_count;
					PosElement = PosElement.next_sibling("enter_pos");
				}

				++ wait_scene_cfg.wait_scene_count;
				WaitSceneElement = WaitSceneElement.next_sibling("wait_scene");
			}

			WaitSceneCfgElement = WaitSceneCfgElement.next_sibling("wait_scene_cfg");
		}

		for (int i = 1; i < CROSS_ACTIVITY_TYPE_MAX; ++ i)
		{
			if (m_wait_scene_cfg_list[i].Invalid())
			{
				*err = path + ": xml node error in [config->wait_scene_cfg_list], [wait_scene_cfg] for some cross_activity_type missing";
				return false;
			}
		}
	}
	
	return true;
}

int CrossCommonConfig::GetWaitSceneCount(int cross_activity_type)
{
	int cross_activity_type_index = cross_activity_type - CROSS_ACTIVITY_TYPE_INVALID;
	if (cross_activity_type_index <= 0 || cross_activity_type_index >= CROSS_ACTIVITY_TYPE_MAX) return 0;

	return m_wait_scene_cfg_list[cross_activity_type_index].wait_scene_count;
}

int CrossCommonConfig::GetWaitSceneID(int cross_activity_type, Posi *pos, int wait_scene_index)
{
	int cross_activity_type_index = cross_activity_type - CROSS_ACTIVITY_TYPE_INVALID;
	if (cross_activity_type_index <= 0 || cross_activity_type_index >= CROSS_ACTIVITY_TYPE_MAX) return 0;

	CrossActivityWaitSceneCfg &wait_scene_cfg = m_wait_scene_cfg_list[cross_activity_type_index];

	int scene_id = 0;
	if (wait_scene_cfg.wait_scene_count > 0 && wait_scene_cfg.wait_scene_count <= WAIT_SCENE_MAX)
	{
		int scene_index = (wait_scene_index >= 0 && wait_scene_index < wait_scene_cfg.wait_scene_count ? wait_scene_index : RandomNum(wait_scene_cfg.wait_scene_count));

		scene_id = wait_scene_cfg.wait_scene_list[scene_index].scene_id;
		
		if (wait_scene_cfg.wait_scene_list[scene_index].enter_pos_count > 0 && wait_scene_cfg.wait_scene_list[scene_index].enter_pos_count <= WAIT_SCENE_ENTER_POS_MAX)
		{
			int pos_index = RandomNum(wait_scene_cfg.wait_scene_list[scene_index].enter_pos_count);

			if (NULL != pos) *pos = gamecommon::GetDisRandPos(wait_scene_cfg.wait_scene_list[scene_index].enter_pos_list[pos_index], wait_scene_cfg.wait_scene_list[scene_index].rand_dis_list[pos_index]);
		}
	}

	return scene_id;
}

bool CrossCommonConfig::IsCrossWaitScene(int scene_id)
{
	return (m_wait_scene_id_set.find(scene_id) != m_wait_scene_id_set.end());	
}

