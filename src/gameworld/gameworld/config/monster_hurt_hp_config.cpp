#include "monster_hurt_hp_config.h"
#include "monster/monsterpool.h"

#include <algorithm>

bool MonsterHurtHpConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = { 0 };

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "MonsterHurtHpConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	// 怪物受击最大伤害
	{
		PugiXmlNode root_element = RootElement.child("monster_max_hurt_hp");
		if (root_element.empty())
		{
			*err = path + " xml not monster_max_hurt_hp node ";
			return false;
		}

		iRet = this->InitHurtCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitHurtCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	//怪物等级差碾压
	{
		PugiXmlNode root_element = RootElement.child("level_delta_hurt_per");
		if (root_element.empty())
		{
			*err = path + " xml not level_delta_hurt_per node ";
			return false;
		}

		iRet = this->InitLevelDeltaCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitLevelDeltaCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	// 不受怪物压制场景类型
	{
		PugiXmlNode root_element = RootElement.child("ignore_lv_delta_scene");
		if (root_element.empty())
		{
			*err = path + " xml not ignore_lv_delta_scene node ";
			return false;
		}

		iRet = this->InitIgnoreLvDeltaCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitIgnoreLvDeltaCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int MonsterHurtHpConfig::InitHurtCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100000;
	}

	while (!data_element.empty())
	{
		int monster_id = 0;
		if (!PugiGetSubNodeValue(data_element, "monster_id", monster_id) || !MONSTERPOOL->IsMonsterExist(monster_id))
		{
			return -1;
		}

		HurtConfig& hurt_cfg = m_hurt_cfg_map[monster_id];

		hurt_cfg.monster_id = monster_id;

		if (!PugiGetSubNodeValue(data_element, "percent", hurt_cfg.percent) || hurt_cfg.percent < 0)
		{
			return -2;
		}

		data_element = data_element.next_sibling();

	}

	return 0;
}

int MonsterHurtHpConfig::InitLevelDeltaCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100000;
	}

	while (!data_element.empty())
	{
		int level_delta = 0;
		if (!PugiGetSubNodeValue(data_element, "level_delta", level_delta) || level_delta <= 0)
		{
			return -1;
		}

		LevelDeltaCfg& level_delta_cfg = m_level_delta_map[level_delta];

		level_delta_cfg.level_delta = level_delta;

		if (!PugiGetSubNodeValue(data_element, "add_hurt_per", level_delta_cfg.add_hurt_per) || level_delta_cfg.add_hurt_per < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "dec_attack_per", level_delta_cfg.dec_attack_per) || level_delta_cfg.dec_attack_per < 0)
		{
			return -3;
		}

		data_element = data_element.next_sibling();

	}

	return 0;
}

int MonsterHurtHpConfig::InitIgnoreLvDeltaCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -100000;
	}

	while (!data_element.empty())
	{
		int scene_type = 0;
		if (!PugiGetSubNodeValue(data_element, "scene_type", scene_type) || scene_type <= 0)
		{
			return -1;
		}

		m_ignore_lv_delta_vec.emplace_back(scene_type);

		data_element = data_element.next_sibling();

	}

	return 0;
}

HurtConfig* MonsterHurtHpConfig::GetConfig(UInt16 monster_id)
{
	hurt_cfg_iter iter = m_hurt_cfg_map.find(monster_id);
	if (iter != m_hurt_cfg_map.end())
	{
		return &(iter->second);
	}
	return NULL;
}

LevelDeltaCfg * MonsterHurtHpConfig::GetLevelDeltaCfg(int level_delta)
{
	auto it = m_level_delta_map.find(level_delta);
	if (it != m_level_delta_map.end())
	{
		return &(it->second);
	}

	return nullptr;
}

bool MonsterHurtHpConfig::IsIgnoreLevelDelta(int scene_type)
{
	auto it = std::find_if(m_ignore_lv_delta_vec.begin(), m_ignore_lv_delta_vec.end(), [scene_type](const int &cmp_scene_type) {
		return cmp_scene_type == scene_type; });

	return it != m_ignore_lv_delta_vec.end();
}
