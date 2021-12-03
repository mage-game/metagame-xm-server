#include "zhuaguiconfig.hpp"

#include "gamecommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "world.h"
#include "scene/scene.h"
#include "scene/map.h"
#include "monster/drop/droppool.hpp"
#include <string>

#include "item/itempool.h"

ZhuaGuiConfig::ZhuaGuiConfig() 	
	: m_limit_lv(0), m_scene_id(0), m_enter_pos(0, 0), m_married_hunli_add_per(0),m_married_add_boss_per(0), m_drop_time_limit(0), m_couple_drop_time_limit(0),m_kick_delay_time(0),
	m_mojing_reward_time(0), m_mojing_reward(0)
{
}

ZhuaGuiConfig::~ZhuaGuiConfig()
{

}

bool ZhuaGuiConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ZhuaGuiConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode other_element = RootElement.child("other_cfg");
		if (other_element.empty())
		{
			*err = configname + ": no [other_cfg].";
			return false;
		}

		iRet = this->InitOtherCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode nf_element = RootElement.child("npc_flush_cfg");
		if (nf_element.empty())
		{
			*err = configname + ": no [npc_flush_cfg].";
			return false;
		}

		iRet = this->InitNpcFlushCfg(nf_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitNpcFlushCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode np_element = RootElement.child("npc_pos_cfg");
		if (np_element.empty())
		{
			*err = configname + ": no [npc_pos_cfg].";
			return false;
		}

		iRet = this->InitNpcPosCfg(np_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitNpcPosCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode ms_element = RootElement.child("monster_cfg");
		if (ms_element.empty())
		{
			*err = configname + ": no [monster_cfg].";
			return false;
		}

		iRet = this->InitMonsterCfg(ms_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMonsterCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode rd_element = RootElement.child("reward_dec");
		if (rd_element.empty())
		{
			*err = configname + ": no [reward_dec].";
			return false;
		}

		iRet = this->InitRewardDecCfg(rd_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitRewardDecCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode rd_element = RootElement.child("team_add_per");
		if (rd_element.empty())
		{
			*err = configname + ": no [team_add_per].";
			return false;
		}

		iRet = this->InitTeamAddCfg(rd_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTeamAddCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int ZhuaGuiConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "scene_id", m_scene_id) || m_scene_id <= 0)
	{
		return -11;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_scene_id);

	if (!PugiGetSubNodeValue(dataElement, "enter_pos_x", m_enter_pos.x) || m_enter_pos.x <= 0)
	{
		return -12;
	}
	if (!PugiGetSubNodeValue(dataElement, "enter_pos_y", m_enter_pos.y) || m_enter_pos.y <= 0)
	{
		return -13;
	}
	if (!PugiGetSubNodeValue(dataElement, "m_limit_lv", m_limit_lv) || m_limit_lv <= 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(dataElement, "married_hunli_add_per", m_married_hunli_add_per) || m_married_hunli_add_per < 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(dataElement, "married_add_boss_per", m_married_add_boss_per) || m_married_add_boss_per < 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(dataElement, "drop_time_limit", m_drop_time_limit) || m_drop_time_limit < 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(dataElement, "kick_delay_time", m_kick_delay_time))
	{
		return -18;
	}

	if (!PugiGetSubNodeValue(dataElement, "couple_drop_time_limit", m_couple_drop_time_limit) || m_couple_drop_time_limit < 0)
	{
		return -19;
	}

	if (!PugiGetSubNodeValue(dataElement, "mojing_reward_time", m_mojing_reward_time))
	{
		return -20;
	}

	if (!PugiGetSubNodeValue(dataElement, "mojing_reward", m_mojing_reward))
	{
		return -21;
	}

	return 0;
}

int ZhuaGuiConfig::InitNpcFlushCfg(PugiXmlNode RootElement)
{
	int idx = 0;
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		if (idx >= ZHUAGUI_SCENE_COUNT)
		{
			return -1;
		}

		ZhuaGuiNpcSceneCfg *npc_cfg = &m_scene_npc_list[idx];

		if (!PugiGetSubNodeValue(dataElement, "scene_id", npc_cfg->scene_id) || npc_cfg->scene_id <= 0)
		{
			return - 2;
		}

		if (!PugiGetSubNodeValue(dataElement, "npc_count", npc_cfg->npc_count) || npc_cfg->npc_count > ZHUAGUI_SCENE_POS_COUNT ||npc_cfg->npc_count <= 0)
		{
			return - 3;
		}

		if (!PugiGetSubNodeValue(dataElement, "flush_min_count", npc_cfg->flush_min_count) || npc_cfg->flush_min_count <= 0 || npc_cfg->flush_min_count > npc_cfg->npc_count)
		{
			return - 4;
		}

		++ idx;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}	

int ZhuaGuiConfig::InitNpcPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int scene_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "scene_id", scene_id) || scene_id <= 0)
		{
			return - 1;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(scene_id);

		ZhuaGuiNpcSceneCfg *npc_cfg = this->GetZhuaGuiNpcSceneCfgBySceneId(scene_id);
		if (NULL != npc_cfg)
		{
			Posi &npc_pos = npc_cfg->pos_list[npc_cfg->pos_count];

			if (!PugiGetSubNodeValue(dataElement, "pos_x", npc_pos.x) || npc_pos.x <= 0)
			{
				return - 2;
			}
			if (!PugiGetSubNodeValue(dataElement, "pos_y", npc_pos.y) || npc_pos.y <= 0)
			{
				return - 3;
			}

			npc_cfg->pos_count ++;
		}
		
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuaGuiConfig::InitMonsterCfg(PugiXmlNode RootElement)
{
	int idx = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		if (idx >= ZHUAGUI_MONSTER_FLUSH_COUNT)
		{
			return -1;
		}

		ZhuaGuiMonsterFlushCfg *monster_cfg = &m_monster_flush_list[idx];

		if (!PugiGetSubNodeValue(dataElement, "monster_id", monster_cfg->monster_id) || monster_cfg->monster_id <= 0)
		{
			return - 2;
		}
		if (!MONSTERPOOL->IsMonsterExist(monster_cfg->monster_id))
		{
			return -22;
		}

		if (!PugiGetSubNodeValue(dataElement, "flush_pos_x", monster_cfg->flush_pos.x) || monster_cfg->flush_pos.x <= 0)
		{
			return - 3;
		}

		if (!PugiGetSubNodeValue(dataElement, "flush_pos_y", monster_cfg->flush_pos.y) || monster_cfg->flush_pos.y <= 0)
		{
			return - 4;
		}
		if (!PugiGetSubNodeValue(dataElement, "flush_num", monster_cfg->flush_num) || monster_cfg->flush_num <= 0)
		{
			return - 5;
		}

		if (!PugiGetSubNodeValue(dataElement, "flush_range", monster_cfg->flush_range) || monster_cfg->flush_range <= 0)
		{
			return - 6;
		}

		if (!PugiGetSubNodeValue(dataElement, "flush_prob", monster_cfg->flush_prob) || monster_cfg->flush_prob <= 0 || monster_cfg->flush_prob > 100)
		{
			return - 7;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_boss", monster_cfg->is_boss) || monster_cfg->is_boss > 1 || monster_cfg->is_boss < 0)
		{
			return - 8;
		}

		if (!PugiGetSubNodeValue(dataElement, "give_hunli", monster_cfg->give_hunli) || monster_cfg->give_hunli <= 0)
		{
			return - 9;
		}

		if (!PugiGetSubNodeValue(dataElement, "team_level_min", monster_cfg->team_level_min) || monster_cfg->team_level_min < 0)
		{
			return - 12;
		}

		if (!PugiGetSubNodeValue(dataElement, "team_level_max", monster_cfg->team_level_max) || monster_cfg->team_level_max < 0 || monster_cfg->team_level_max < monster_cfg->team_level_min)
		{
			return - 13;
		}

		{
			monster_cfg->drop_id_list.clear();

			PugiXmlNode dropid_list_element = dataElement.child("drop_id_list");
			if (dropid_list_element.empty())
			{
				return -104;
			}

			PugiXmlNode dropid_element = dropid_list_element.child("dropid");
			while (!dropid_element.empty())
			{
				UInt16 dropid = 0;
				if (!PugiGetNodeValue(dropid_element, dropid)) return false;

				if (0 != dropid && NULL != DROPPOOL->GetDropConfig(dropid))
				{
					monster_cfg->drop_id_list.push_back(dropid);
				}

				dropid_element = dropid_element.next_sibling();
			}
		}

		{
			monster_cfg->couple_drop_id_list.clear();

			PugiXmlNode dropid_list_element = dataElement.child("couple_drop_id_list");
			if (dropid_list_element.empty())
			{
				return -104;
			}

			PugiXmlNode dropid_element = dropid_list_element.child("dropid");
			while (!dropid_element.empty())
			{
				UInt16 dropid = 0;
				if (!PugiGetNodeValue(dropid_element, dropid)) return false;

				if (0 != dropid && NULL != DROPPOOL->GetDropConfig(dropid))
				{
					monster_cfg->couple_drop_id_list.push_back(dropid);
				}

				dropid_element = dropid_element.next_sibling();
			}
		}

		++ idx;

		dataElement = dataElement.next_sibling();

	}

	return 0;
}

int ZhuaGuiConfig::InitRewardDecCfg(PugiXmlNode RootElement)
{
	int idx = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		if (idx >= ZHUAGUI_REWARD_DEC_COUNT)
		{
			return -1;
		}

		ZhuaGuiRewardDecCfg *reward_dec_cfg = &m_reward_dec_list[idx];

		if (!PugiGetSubNodeValue(dataElement, "kill_monster", reward_dec_cfg->kill_monster) || reward_dec_cfg->kill_monster <= 0)
		{
			return - 2;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_per", reward_dec_cfg->reward_per) || reward_dec_cfg->reward_per <= 0)
		{
			return - 3;
		}

		++ idx;

		dataElement = dataElement.next_sibling();

	}

	return 0;
}

int ZhuaGuiConfig::InitTeamAddCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int team_player_num = 0;

		if (!PugiGetSubNodeValue(dataElement, "team_player_num", team_player_num) || team_player_num <= 0 || team_player_num > MAX_TEAM_MEMBER_NUM)
		{
			return - 1;
		}

		ZhuaGuiTeamAdd &cfg = m_team_add_list[team_player_num - 1];
		cfg.team_player_num = team_player_num;

		if (!PugiGetSubNodeValue(dataElement, "hunli_add_per", cfg.hunli_add_per) || cfg.hunli_add_per < 0)
		{
			return - 2;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_boss_per", cfg.add_boss_per) || cfg.add_boss_per < 0)
		{
			return - 3;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

ZhuaGuiNpcSceneCfg * ZhuaGuiConfig::GetZhuaGuiNpcSceneCfgByIdx(int idx)
{
	if (idx < 0 || idx >= ZHUAGUI_SCENE_COUNT)
	{
		return NULL;
	}

	if (m_scene_npc_list[idx].scene_id <=0)
	{
		return NULL;
	}

	return &m_scene_npc_list[idx];
}

ZhuaGuiNpcSceneCfg * ZhuaGuiConfig::GetZhuaGuiNpcSceneCfgBySceneId(int scene_id)
{
	for (int i = 0; i < ZHUAGUI_SCENE_COUNT; i++)
	{
		if (m_scene_npc_list[i].scene_id == scene_id)
		{
			return &m_scene_npc_list[i];
		}
	}

	return NULL;
}

bool ZhuaGuiConfig::IsNeedFlush(int scene_id, int curr_count)
{
	ZhuaGuiNpcSceneCfg *npc_cfg = this->GetZhuaGuiNpcSceneCfgBySceneId(scene_id);
	if (NULL != npc_cfg && curr_count <= npc_cfg->flush_min_count)
	{
		return true;
	}
	
	return false;
}

bool ZhuaGuiConfig::GetRandomFlushInfo(int scene_id, int *npc_count, Posi randpos_list[ZHUAGUI_SCENE_POS_COUNT])
{
	ZhuaGuiNpcSceneCfg *npc_cfg = this->GetZhuaGuiNpcSceneCfgBySceneId(scene_id);
	if (NULL != npc_cfg && npc_cfg->npc_count < npc_cfg->pos_count) // npc的个数肯定要少于点的个数
	{
		if (NULL != npc_count) *npc_count = npc_cfg->npc_count;

		std::vector<Posi> pos_vec;
		for (int i = 0; i < npc_cfg->pos_count; i++)
		{
			pos_vec.push_back(npc_cfg->pos_list[i]);
		}

		for (int rand_i = 0; rand_i < npc_cfg->npc_count; rand_i++)
		{
			if (!pos_vec.empty())
			{
				int idx = RandomNum((int)pos_vec.size());

				std::vector<Posi>::iterator it = pos_vec.begin();
				std::advance(it, idx);

				randpos_list[rand_i] = *it;

				pos_vec.erase(it);
			}
		}

		return true;
	}

	return false;
}

bool ZhuaGuiConfig::GetEnterFbInfo(int *scene_id, Posi *enter_pos)
{
	if (NULL != scene_id && NULL != enter_pos)
	{
		*scene_id = m_scene_id;
		*enter_pos = m_enter_pos;

		return true;
	}

	return false;
}

bool ZhuaGuiConfig::IsBoss(int monster_id)
{
	if (monster_id <= 0)
	{
		return false;
	}

	for (int i = 0; i < ZHUAGUI_MONSTER_FLUSH_COUNT; i++)
	{
		if (m_monster_flush_list[i].IsBoss() && m_monster_flush_list[i].monster_id == monster_id)
		{
			return true;
		}
	}

	return false;
}

const ZhuaGuiTeamAdd * ZhuaGuiConfig::GetTeamAddPer(int team_num)
{
	if (team_num <= 0 || team_num > MAX_TEAM_MEMBER_NUM)
	{
		return NULL;
	}

	return &m_team_add_list[team_num - 1];
}

ZhuaGuiMonsterFlushCfg * ZhuaGuiConfig::GetFlushMonsterInfoByIdx(int idx)
{
	if (idx < 0 || idx >= ZHUAGUI_MONSTER_FLUSH_COUNT)
	{
		return NULL;
	}

	if (m_monster_flush_list[idx].monster_id <= 0)
	{
		return NULL;
	}

	return &m_monster_flush_list[idx];
}

ZhuaGuiMonsterFlushCfg * ZhuaGuiConfig::GetFlushMonsterInfoByMonsterId(int monster_id)
{
	for (int i = 0; i < ZHUAGUI_MONSTER_FLUSH_COUNT; i++)
	{
		if (m_monster_flush_list[i].monster_id == monster_id)
		{
			return &m_monster_flush_list[i];
		}
	}

	return NULL;
}

int ZhuaGuiConfig::GetHunliByMonsterID(int monster_id)
{
	for (int i = 0; i < ZHUAGUI_MONSTER_FLUSH_COUNT; i++)
	{
		if (m_monster_flush_list[i].monster_id == monster_id)
		{
			return m_monster_flush_list[i].give_hunli;
		}
	}

	return 0;
}

int ZhuaGuiConfig::GetHunliFix(int fetch_guicount, int give_huoli ,int team_player_num, bool is_has_couple)
{
	float reward_huoli = (float)give_huoli;
	float reward_add_per = 100;

	if (team_player_num > 0 && team_player_num <= MAX_TEAM_MEMBER_NUM)
	{
		reward_add_per += m_team_add_list[team_player_num - 1].hunli_add_per;
	}

	if (is_has_couple)
	{
		reward_add_per += m_married_hunli_add_per;
	}
	
	for (int i = 0; i < ZHUAGUI_REWARD_DEC_COUNT; i++)
	{
		if (fetch_guicount <= m_reward_dec_list[i].kill_monster)
		{
			return static_cast<int>(reward_huoli * (reward_add_per / 100.f) * (m_reward_dec_list[i].reward_per / 100.0f));
		}
	}

	return 0;
}


