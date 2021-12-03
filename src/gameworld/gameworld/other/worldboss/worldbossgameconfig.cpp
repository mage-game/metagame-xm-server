#include <memory.h>
#include <stdlib.h>

#include "worldbossgameconfig.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "scene/sceneregister/sceneregister.hpp"
#include "monster/monsterpool.h"

#include <set>
#include <string>


WorldBossGameConfig::WorldBossGameConfig()
{

}

WorldBossGameConfig::~WorldBossGameConfig()
{

}

bool WorldBossGameConfig::Init(const std::string &configname, std::string *err)
{
	//TiXmlDocument m_document;
	//char errinfo[1024] = {0};

	//if (configname == "" || !m_document.LoadFile(configname))
	//{
	//	sprintf(errinfo,"Load WorldBossGameConfig config [%s] Error.%s", configname.c_str(),m_document.ErrorDesc());
	//	*err = errinfo;
	//	return false;
	//}

	//TiXmlElement *RootElement = m_document.RootElement();
	//if (NULL == RootElement)
	//{
	//	sprintf(errinfo,"Load WorldBossGameConfig config [%s] Error.cannot find root node.", configname.c_str());
	//	*err = errinfo;
	//	return false;
	//}

	//{
	//	TiXmlElement *worldboss_list_element = RootElement->FirstChildElement("worldboss_list");
	//	if (NULL == worldboss_list_element)
	//	{
	//		*err = configname + ": no [worldboss_list].";
	//		return false;
	//	}

	//	int iRet = this->InitWorldBossCfg(worldboss_list_element);
	//	if (iRet)
	//	{
	//		sprintf(errinfo,"%s: InitWorldBossCfg failed %d", configname.c_str(), iRet);
	//		*err = errinfo;
	//		return false;
	//	}
	//}

	//{
	//	TiXmlElement *jingyingboss_list_element = RootElement->FirstChildElement("jingyingboss_list");
	//	if (NULL == jingyingboss_list_element)
	//	{
	//		*err = configname + ": no [jingyingboss_list].";
	//		return false;
	//	}

	//	int iRet = this->InitJingyingBossCfg(jingyingboss_list_element);
	//	if (iRet)
	//	{
	//		sprintf(errinfo,"%s: InitJingyingBossCfg failed %d", configname.c_str(), iRet);
	//		*err = errinfo;
	//		return false;
	//	}
	//}

	return true;
}

int WorldBossGameConfig::InitWorldBossCfg(TiXmlElement *RootElement)
{
	//TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	//while (NULL != dataElement)
	//{
	//	int boss_id = 0;
	//	if (!GetSubNodeValue(dataElement, "bossID", boss_id) || boss_id <= 0)
	//	{
	//		return -1;
	//	}

	//	if (!MONSTERPOOL->IsMonsterExist(boss_id))
	//	{
	//		return -2;
	//	}

	//	WorldBossRefreshPos cfg;
	//
	//	if (!GetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
	//	{
	//		return -3;
	//	}
	//	CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);

	//	SceneRegister::Instance().AddWorldBossScene(cfg.scene_id);			// 精英boss的场景不算世界boss场景，不用加入

	//	int entry_scene_id = 0;
	//	if (!GetSubNodeValue(dataElement, "entry_scene_id", entry_scene_id) || entry_scene_id <= 0)
	//	{
	//		return -4;
	//	}
	//	CheckResourceCenter::Instance().GetSceneCheck()->Add(entry_scene_id);

	//	SceneRegister::Instance().AddWorldBossScene(entry_scene_id);		// 入口场景也作为世界boss场景

	//	if (!GetSubNodeValue(dataElement, "born_x", cfg.pos_x) || cfg.pos_x < 0)
	//	{
	//		return -7;
	//	}

	//	if (!GetSubNodeValue(dataElement, "born_y", cfg.pos_y) || cfg.pos_y < 0)
	//	{
	//		return -8;
	//	}

	//	m_worldboss_pos_map[boss_id] = cfg;

	//	dataElement = dataElement->NextSiblingElement();
	//}

	return 0;
}

int WorldBossGameConfig::InitJingyingBossCfg(TiXmlElement *RootElement)
{
	//TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	//while (NULL != dataElement)
	//{
	//	int boss_id = 0;
	//	if (!GetSubNodeValue(dataElement, "bossID", boss_id) || boss_id <= 0)
	//	{
	//		return -1;
	//	}

	//	if (!MONSTERPOOL->IsMonsterExist(boss_id))
	//	{
	//		return -2;
	//	}

	//	JIngyingBossRefreshPos cfg;

	//	if (!GetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
	//	{
	//		return -3;
	//	}
	//	CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);

	//	// SceneRegister::Instance().AddJingyingBossScene(cfg.scene_id);		

	//	int entry_scene_id = 0;
	//	if (!GetSubNodeValue(dataElement, "entry_scene_id", entry_scene_id) || entry_scene_id <= 0)
	//	{
	//		return -4;
	//	}
	//	CheckResourceCenter::Instance().GetSceneCheck()->Add(entry_scene_id);

	//	char tmp_tag[32];
	//	for (int i = 0; i < JIngyingBossRefreshPos::MAX_JINGYING_REFRESH_POS; ++i)
	//	{
	//		memset(tmp_tag, 0, sizeof(tmp_tag));
	//		sprintf(tmp_tag, "born_x%d", i + 1);
	//		if (!GetSubNodeValue(dataElement, tmp_tag, cfg.born_x_list[i]) || cfg.born_x_list[i] < 0)
	//		{
	//			return -10;
	//		}

	//		memset(tmp_tag, 0, sizeof(tmp_tag));
	//		sprintf(tmp_tag, "born_y%d", i + 1);
	//		if (!GetSubNodeValue(dataElement, tmp_tag, cfg.born_y_list[i]) || cfg.born_y_list[i] < 0)
	//		{
	//			return -11;
	//		}
	//	}

	//	if (m_jingying_pos_map.end() != m_jingying_pos_map.find(boss_id))
	//	{
	//		return -20;
	//	}

	//	m_jingying_pos_map[boss_id] = cfg;

	//	dataElement = dataElement->NextSiblingElement();
	//}

	return 0;
}

const WorldBossRefreshPos * WorldBossGameConfig::GetWorldBossRefreshPos(int boss_id)
{
	std::map<int, WorldBossRefreshPos>::const_iterator iter = m_worldboss_pos_map.find(boss_id);
	if (m_worldboss_pos_map.end() == iter) return NULL;

	return &iter->second;
}

const JIngyingBossRefreshPos * WorldBossGameConfig::GetJingyingBossRefreshPos(int boss_id)
{
	std::map<int, JIngyingBossRefreshPos>::const_iterator iter = m_jingying_pos_map.find(boss_id);
	if (m_jingying_pos_map.end() == iter) return NULL;

	return &iter->second;
}
