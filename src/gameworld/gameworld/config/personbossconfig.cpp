#include "personbossconfig.hpp"

#include "config/logicconfigmanager.hpp"
#include "item/itemextern.hpp"
#include "item/itempool.h"
#include "monster/monsterpool.h"
#include "servercommon/commondata.hpp"

PersonBossConfig::PersonBossConfig() : m_max_boss_layer(0)
{

}

PersonBossConfig::~PersonBossConfig()
{

}

bool PersonBossConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = { 0 };
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "PersonBossConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode personboss_element = RootElement.child("other");
		if (personboss_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(personboss_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: PersonBossConfig::InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode personboss_element = RootElement.child("boss_scene_cfg");
		if (personboss_element.empty())
		{
			*err = configname + ": no [boss_scene_cfg].";
			return false;
		}

		iRet = this->InitBossSceneCfg(personboss_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: PersonBossConfig::InitBossSceneCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode personboss_element = RootElement.child("person_reset");
		if (personboss_element.empty())
		{
			*err = configname + ": no [person_reset].";
			return false;
		}

		iRet = this->InitBossBuyTimesCfg(personboss_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: PersonBossConfig::InitBossBuyTimesCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int PersonBossConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "limit_level", m_other_cfg.limit_level) || m_other_cfg.limit_level <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "buy_gold", m_other_cfg.buy_times_gold) || m_other_cfg.buy_times_gold <= 0)
	{
		return -2;
	}

	return 0;
}

int PersonBossConfig::InitBossSceneCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_max_boss_layer = 0;
	while (!dataElement.empty())
	{
		int layer = 0;
		if (!PugiGetSubNodeValue(dataElement, "layer", layer) || layer > MAX_PERSON_BOSS_LAYER || layer != m_max_boss_layer + 1)
		{
			return -1;
		}

		m_max_boss_layer = layer;

		PersonBossCfgItem &cfg = m_personal_boss_list[layer];
		cfg.layer = layer;

		if (!PugiGetSubNodeValue(dataElement, "need_level", cfg.need_level) || cfg.need_level <= 0 || cfg.need_level > MAX_ROLE_LEVEL)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_item_id", cfg.need_item_id) || NULL == ITEMPOOL->GetItem(cfg.need_item_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_item_num", cfg.need_item_num) || cfg.need_item_num <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "enter_pos_x", cfg.enter_pos.x) || cfg.enter_pos.x <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "enter_pos_y", cfg.enter_pos.y) || cfg.enter_pos.y <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_id", cfg.boss_id[1]) || cfg.boss_id[1] <= 0 || !MONSTERPOOL->IsMonsterExist(cfg.boss_id[1]))
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_id2", cfg.boss_id[2]) || cfg.boss_id[2] <= 0 || !MONSTERPOOL->IsMonsterExist(cfg.boss_id[2]))
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_id3", cfg.boss_id[3]) || cfg.boss_id[3] <= 0 || !MONSTERPOOL->IsMonsterExist(cfg.boss_id[3]))
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_id4", cfg.boss_id[4]) || cfg.boss_id[4] <= 0 || !MONSTERPOOL->IsMonsterExist(cfg.boss_id[4]))
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement, "flush_pos_x", cfg.flush_pos.x) || cfg.flush_pos.x <= 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "flush_pos_y", cfg.flush_pos.y) || cfg.flush_pos.y <= 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "day_enter_times", cfg.day_enter_times) || cfg.day_enter_times < 0)
		{
			return -12;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const PersonBossCfgItem * PersonBossConfig::GetBossSceneCfg(int layer)
{
	if (layer <= 0 || layer > m_max_boss_layer)
	{
		return NULL;
	}

	return &m_personal_boss_list[layer];
}

const PersonBossCfgItem * PersonBossConfig::GetBossSceneCfgBySceneId(int scene_id)
{
	for (int layer = 1; layer <= m_max_boss_layer; ++layer)
	{
		if (m_personal_boss_list[layer].scene_id == scene_id)
		{
			return &m_personal_boss_list[layer];
		}
	}

	return NULL;
}

bool PersonBossConfig::IsPersonBoss(int boss_id)
{
	bool is_person_boss = false;
	for (int layer = 1; layer <= m_max_boss_layer; ++layer)
	{
		for (int i = 0; i < PROF_TYPE_PROF_NO_LIMIT; i++)
		{
			if (m_personal_boss_list[layer].boss_id[i] == boss_id)
			{
				is_person_boss = true;
				break;
			}
		}
	}

	return is_person_boss;
}

int PersonBossConfig::InitBossBuyTimesCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_time = 0;
	while (!dataElement.empty())
	{
		PersonBossBuyTimesCfg cfg;
		if (!PugiGetSubNodeValue(dataElement, "reset_time", cfg.times) || cfg.times <= 0 || cfg.times != last_time + 1)
		{
			return -1;
		}
		last_time = cfg.times;

		if (!PugiGetSubNodeValue(dataElement, "need_gold", cfg.need_gold) || cfg.need_gold <= 0)
		{
			return -2;
		}

		m_personboss_buy_times_vec.push_back(cfg);
		dataElement = dataElement.next_sibling();
	}
	return 0;
}

int PersonBossConfig::GetBuyCostByTimes(int times)
{
	if ((int)m_personboss_buy_times_vec.size() <= 0)
	{
		return 0;
	}

	int i = (int)m_personboss_buy_times_vec.size() - 1;
	for (; i >= 0; --i)
	{
		if (times >= m_personboss_buy_times_vec[i].times)
		{
			return m_personboss_buy_times_vec[i].need_gold;
		}
	}

	return 0;
}