#include "xinmofbconfig.hpp"
#include "monster/monsterpool.h"

XinMoFbConfig::XinMoFbConfig()
{
}

XinMoFbConfig::~XinMoFbConfig()
{

}

bool XinMoFbConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = { 0 };
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "XinMoFbConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode xinmo_element = RootElement.child("other");
		if (xinmo_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(xinmo_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: XinMoFbConfig::InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode xinmo_element = RootElement.child("layer");
		if (xinmo_element.empty())
		{
			*err = configname + ": no [layer].";
			return false;
		}

		iRet = this->InitLayerCfg(xinmo_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: XinMoFbConfig::InitLayerCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode xinmo_element = RootElement.child("monster");
		if (xinmo_element.empty())
		{
			*err = configname + ": no [monster].";
			return false;
		}

		iRet = this->InitMonsterCfg(xinmo_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: XinMoFbConfig::InitMonsterCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode xinmo_element = RootElement.child("pass_skill");
		if (xinmo_element.empty())
		{
			*err = configname + ": no [pass_skill].";
			return false;
		}

		iRet = this->InitPassSkillCfg(xinmo_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: XinMoFbConfig::InitPassSkillCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int XinMoFbConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "kick_out_time", m_other_cfg.kick_out_time) || m_other_cfg.kick_out_time <= 0)
	{
		return -1001;
	}

	return 0;
}

int XinMoFbConfig::InitLayerCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int next_layer = 0;
	while (!data_element.empty())
	{
		int layer = -1;
		if (!PugiGetSubNodeValue(data_element, "layer", layer) || layer < 0 || layer >= XINMOFB_LAYER_TYPE_MAX || layer != next_layer)
		{
			return -1;
		}

		XinMoFbLayerConfig &cfg = m_layer_cfg_list[layer];
		cfg.layer = layer;

		if (!PugiGetSubNodeValue(data_element, "role_level", cfg.role_level) || cfg.role_level <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "enter_pos_x", cfg.enter_posi.x) || cfg.enter_posi.x <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "enter_pos_y", cfg.enter_posi.y) || cfg.enter_posi.y <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "fb_time", cfg.fb_time) || cfg.fb_time <= 0)
		{
			return -6;
		}

		++next_layer;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int XinMoFbConfig::InitMonsterCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int layer = -1;
		if (!PugiGetSubNodeValue(data_element, "layer", layer) || layer < 0 || layer >= XINMOFB_LAYER_TYPE_MAX)
		{
			return -1;
		}

		XinMoFbMonsterConfig &monster_cfg = m_monster_cfg_list[layer];

		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= XINMOFB_MONSTER_MAX_NUM)
		{
			return -2;
		}

		++monster_cfg.monster_count;

		XinMoFbMonsterConfig::MonsterConfig &cfg = monster_cfg.monster_cfg_list[seq];
		cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "monster_id", cfg.monster_id) || cfg.monster_id < 0 )//|| !MONSTERPOOL->IsMonsterExist(cfg.monster_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "pos_x", cfg.bron_pos.x) || cfg.bron_pos.x <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "pos_y", cfg.bron_pos.y) || cfg.bron_pos.y <= 0)
		{
			return -5;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int XinMoFbConfig::InitPassSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int next_skill_seq = 0;
	while (!data_element.empty())
	{
		int skill_seq = -1;
		if (!PugiGetSubNodeValue(data_element, "skill_seq", skill_seq) || skill_seq < 0 || skill_seq >= XINMOFB_LAYER_TYPE_MAX ||skill_seq != next_skill_seq)
		{
			return -1;
		}

		XinMoFbPassSkillConfig &skill_cfg = m_pass_skill_cfg_list[skill_seq];
		skill_cfg.skill_seq = skill_seq;

		if (!PugiGetSubNodeValue(data_element, "cd_time_s", skill_cfg.cd_time_s) || skill_cfg.cd_time_s < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "percent_val", skill_cfg.percent_val) || skill_cfg.percent_val < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "effect_val", skill_cfg.effect_val) || skill_cfg.effect_val < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_index", skill_cfg.skill_index) || skill_cfg.skill_index < 0)
		{
			return -7;
		}

		++next_skill_seq;
		data_element = data_element.next_sibling();
	}

	return 0;
}

const XinMoFbLayerConfig * XinMoFbConfig::GetLayerCfg(int layer)
{
	if (layer < 0 || layer >= XINMOFB_LAYER_TYPE_MAX)
	{
		return NULL;
	}

	return &m_layer_cfg_list[layer];
}

const XinMoFbLayerConfig * XinMoFbConfig::GetLayerCfgBySceneId(int scene_id)
{
	if (scene_id <= 0)
	{
		return NULL;
	}

	for (int layer = 0; layer < XINMOFB_LAYER_TYPE_MAX; ++layer)
	{
		if (scene_id == m_layer_cfg_list[layer].scene_id)
		{
			return &m_layer_cfg_list[layer];
		}
	}

	return NULL;
}

const XinMoFbMonsterConfig::MonsterConfig * XinMoFbConfig::GetMonsterCfg(int layer, int seq)
{
	if (layer < 0 || layer >= XINMOFB_LAYER_TYPE_MAX || seq < 0 || seq >= XINMOFB_MONSTER_MAX_NUM)
	{
		return NULL;
	}

	return &m_monster_cfg_list[layer].monster_cfg_list[seq];
}

const XinMoFbPassSkillConfig * XinMoFbConfig::GetPassSkillCfg(int skill_seq)
{
	if (skill_seq < 0 || skill_seq >= XINMOFB_LAYER_TYPE_MAX)
	{
		return NULL;
	}

	return &m_pass_skill_cfg_list[skill_seq];
}

int XinMoFbConfig::GetMonsterCount(int layer)
{
	if (layer < 0 || layer >= XINMOFB_LAYER_TYPE_MAX)
	{
		return 0;
	}

	return m_monster_cfg_list[layer].monster_count;
}