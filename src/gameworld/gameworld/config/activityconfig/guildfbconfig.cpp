#include "guildfbconfig.hpp"

#include "gamecommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "world.h"
#include "scene/scene.h"
#include "scene/map.h"
#include <string>

GuildFbConfig::GuildFbConfig()
	: m_sceneid(0), m_enter_pos(0, 0), m_guildexp_cost(0), m_guildlv_limit(0), m_guard_bossid(0), m_next_wave_wait_time(0), m_wave_count(0), m_delay_kick_time(0), m_activity_interval_s(0)
{
	memset(m_wave_list, 0, sizeof(m_wave_list));
}

GuildFbConfig::~GuildFbConfig()
{

}

bool GuildFbConfig::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "GuildFbConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// 其他配置
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = path + ": xml no other node->other";
			return false;
		}

		PugiXmlNode data_element = other_element.child("data");
		if (data_element.empty())
		{
			*err = path + ": xml no other node->other->data";
			return false;
		}

		if (!PugiGetSubNodeValue(data_element, "sceneid", m_sceneid) || m_sceneid <= 0)
		{
			*err = path + ": xml no other node->other->data->sceneid";
			return false;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(m_sceneid);

		if (!PugiGetSubNodeValue(data_element, "enterpos_x", m_enter_pos.x) || m_enter_pos.x <= 0)
		{
			*err = path + ": xml no other node->other->data->enterpos_x";
			return false;
		}
		if (!PugiGetSubNodeValue(data_element, "enterpos_y", m_enter_pos.y) || m_enter_pos.y <= 0)
		{
			*err = path + ": xml no other node->other->data->enterpos_y";
			return false;
		}

		if (!PugiGetSubNodeValue(data_element, "guildexp_cost", m_guildexp_cost) || m_guildexp_cost < 0)
		{
			*err = path + ": xml no other node->other->data->m_guildexp_cost";
			return false;
		}
		if (!PugiGetSubNodeValue(data_element, "guildlv_limit", m_guildlv_limit) || m_guildlv_limit <= 0)
		{
			*err = path + ": xml no other node->other->data->guildlv_limit";
			return false;
		}

		if (!PugiGetSubNodeValue(data_element, "guard_boss_id", m_guard_bossid) || m_guard_bossid <= 0)
		{
			*err = path + ": xml no other node->other->data->guard_boss_id";
			return false;
		}
		if (!MONSTERPOOL->IsMonsterExist(m_guard_bossid))
		{
			*err = path + ": xml no other node->other->data->m_guard_bossid not exist";
			return false;
		}

		if (!PugiGetSubNodeValue(data_element, "next_wave_wait_time", m_next_wave_wait_time) || m_next_wave_wait_time <= 0)
		{
			*err = path + ": xml no other node->other->data->m_next_wave_wait_time";
			return false;
		}

		if (!PugiGetSubNodeValue(data_element, "delay_kick_time", m_delay_kick_time))
		{
			*err = path + ": xml no other node->other->data->delay_kick_time";
			return false;
		}

		if (!PugiGetSubNodeValue(data_element, "activity_interval_s", m_activity_interval_s))
		{
			*err = path + ": xml no other node->other->data->m_activity_interval_s";
			return false;
		}
	}

	{
		// 波次 血量配置
		PugiXmlNode wave_element = RootElement.child("wave_cfg");
		if (wave_element.empty())
		{
			*err = path + ": xml no wave_cfg ";
			return false;
		}

		PugiXmlNode data_element = wave_element.child("data");
		if (data_element.empty())
		{
			*err = path + ": xml no other wave_cfg->data";
			return false;
		}

		int last_wave_idx = -1;
		while (!data_element.empty())
		{
			int wave_idx = 0;
			if (!PugiGetSubNodeValue(data_element, "wave", wave_idx) || wave_idx != last_wave_idx + 1 || wave_idx < 0 || wave_idx >= GUILD_FB_WAVE_MAX)
			{
				*err = path + ": xml no wave_cfg->data wave";
				return false;
			}
			last_wave_idx = wave_idx;

			++ m_wave_count;
			if (m_wave_count > GUILD_FB_WAVE_MAX)
			{
				*err = path + ": xml no wave_cfg->data wave too much GUILD_FB_WAVE_MAX";
				return false;
			}

			GuildFbWaveCfg &wave_cfg = m_wave_list[wave_idx];

			if (!PugiGetSubNodeValue(data_element, "sceneid", wave_cfg.sceneid) || wave_cfg.sceneid <= 0)
			{
				*err = path + ": xml no wave_cfg->data->sceneid";
				return false;
			}
			CheckResourceCenter::Instance().GetSceneCheck()->Add(wave_cfg.sceneid);

			if (!PugiGetSubNodeValue(data_element, "enterpos_x", wave_cfg.enter_pos.x) || wave_cfg.enter_pos.x <= 0)
			{
				*err = path + ": xml no other wave_cfg->data->enterpos_x";
				return false;
			}
			if (!PugiGetSubNodeValue(data_element, "enterpos_y", wave_cfg.enter_pos.y) || wave_cfg.enter_pos.y <= 0)
			{
				*err = path + ": xml no other wave_cfg->data->enterpos_y";
				return false;
			}

			if (!PugiGetSubNodeValue(data_element, "guard_maxhp", wave_cfg.guard_boss_maxhp) || wave_cfg.guard_boss_maxhp <= 0)
			{
				*err = path + ": xml no other wave_cfg->data guard_maxhp";
				return false;
			}

			if (!PugiGetSubNodeValue(data_element, "guard_boss_recover_per", wave_cfg.guard_boss_recover_per) || wave_cfg.guard_boss_recover_per < 0)
			{
				*err = path + ": xml no other wave_cfg->data guard_boss_recover_per";
				return false;
			}

			{
				wave_cfg.reward_count = ItemConfigData::ReadConfigList(data_element, std::string("reward_item"),
					wave_cfg.reward_item_list, GUILD_FB_WAVE_MAX_REWARD_COUNT);
				if (wave_cfg.reward_count < 0)
				{
					*err = path + ": xml no other wave_cfg->data reward_item";
					return false;
				}
			}

			if (!PugiGetSubNodeValue(data_element, "reward_gongxian", wave_cfg.reward_gongxian) || wave_cfg.reward_gongxian < 0)
			{
				*err = path + ": xml no other wave_cfg->reward_gongxian";
				return false;
			}
	
			data_element = data_element.next_sibling();
		}
	}

	{
		// 波次 怪物配置
		PugiXmlNode monster_element = RootElement.child("monster_cfg");
		if (monster_element.empty())
		{
			*err = path + ": xml no monster_cfg";
			return false;
		}

		PugiXmlNode data_element = monster_element.child("data");
		if (data_element.empty())
		{
			*err = path + ": xml no monster_cfg -> data";
			return false;
		}

		while (!data_element.empty())
		{
			int wave_idx = 0;
			if (!PugiGetSubNodeValue(data_element, "wave", wave_idx) || wave_idx >= m_wave_count || wave_idx < 0 || wave_idx >= GUILD_FB_WAVE_MAX)
			{
				*err = path + ": xml no other monster_cfg -> data -> wave";
				return false;
			}

			GuildFbWaveCfg &wave_cfg = m_wave_list[wave_idx];

			int sub_wave = 0;
			if (!PugiGetSubNodeValue(data_element, "sub_wave", sub_wave) || sub_wave < 0)
			{
				*err = path + ": xml no other monster_cfg -> data -> sub_wave";
				return false;
			}

			int monster_id = 0;
			if (!PugiGetSubNodeValue(data_element, "monster_id", monster_id) || monster_id <= 0)
			{
				*err = path + ": xml no other monster_cfg -> data -> monster_id";
				return false;
			}
			if (!MONSTERPOOL->IsMonsterExist(monster_id))
			{
				*err = path + ": xml no other monster_cfg -> data -> monster_id no exist";
				return false;
			}

			int pos_x = 0;
			if (!PugiGetSubNodeValue(data_element, "pos_x", pos_x) || pos_x <= 0)
			{
				*err = path + ": xml no other monster_cfg -> data -> pos_x";
				return false;
			}
			int pos_y = 0;
			if (!PugiGetSubNodeValue(data_element, "pos_y", pos_y) || pos_y <= 0)
			{
				*err = path + ": xml no other monster_cfg -> data -> pos_y";
				return false;
			}

			if (wave_cfg.enemy_count >= GUILD_FB_ENEMY_POINT_MAX)
			{
				*err = path + ": xml no other monster_cfg -> data -> wave_cfg->enemy_count >= GUILD_FB_ENEMY_POINT_MAX";
				return false;
			}
			
			wave_cfg.enemy_list[wave_cfg.enemy_count].sub_wave = sub_wave;
			wave_cfg.enemy_list[wave_cfg.enemy_count].enemy_monster_id = monster_id;
			wave_cfg.enemy_list[wave_cfg.enemy_count].birth_pos = Posi(pos_x, pos_y);

			++ wave_cfg.enemy_count;

			data_element = data_element.next_sibling();
		}
	}

	{
		// 波次 守卫点的坐标
		PugiXmlNode guard_pos_element = RootElement.child("guard_pos_cfg");
		if (guard_pos_element.empty())
		{
			*err = path + ": xml no other guard_pos_element";
			return false;
		}

		PugiXmlNode data_element = guard_pos_element.child("data");
		if (data_element.empty())
		{
			*err = path + ": xml no other guard_pos_element data";
			return false;
		}

		while (!data_element.empty())
		{
			int wave_idx = 0;
			if (!PugiGetSubNodeValue(data_element, "wave", wave_idx) || wave_idx >= m_wave_count || wave_idx < 0 || wave_idx >= GUILD_FB_WAVE_MAX)
			{
				*err = path + ": xml no other guard_pos_element wave";
				return false;
			}

			GuildFbWaveCfg &wave_cfg = m_wave_list[wave_idx];

			int pos_x = 0;
			if (!PugiGetSubNodeValue(data_element, "pos_x", pos_x) || pos_x <= 0)
			{
				*err = path + ": xml no other guard_pos_element pos_x";
				return false;
			}
			int pos_y = 0;
			if (!PugiGetSubNodeValue(data_element, "pos_y", pos_y) || pos_y <= 0)
			{
				*err = path + ": xml no other guard_pos_element pos_y";
				return false;
			}

			if (wave_cfg.guard_pt_count >= GUILD_FB_GUARD_PATH_POINT_MAX)
			{
				*err = path + ": xml no other guard_pos_element wave_cfg->guard_pt_count >= GUILD_FB_GUARD_PATH_POINT_MAX";
				return false;
			}

			wave_cfg.guard_path_ptlist[wave_cfg.guard_pt_count] = Posi(pos_x, pos_y);
			++ wave_cfg.guard_pt_count;

			data_element = data_element.next_sibling();
		}
	}

	return true;
}

int GuildFbConfig::GetEnterInfo(Posi *enter_pos)
{
	if (NULL != enter_pos) *enter_pos = m_enter_pos;
	return m_sceneid;
}

int GuildFbConfig::GetGuardBossMaxHp(int wave_idx)
{
	if (wave_idx < 0 || wave_idx >= m_wave_count || wave_idx >= GUILD_FB_WAVE_MAX) 
	{
		return 0;
	}

	GuildFbWaveCfg *wave_cfg = &m_wave_list[wave_idx];

	return wave_cfg->guard_boss_maxhp;
}

bool GuildFbConfig::GetGuardPathPos(int wave_idx, int pos_idx, Posi *pos)
{
	if (wave_idx < 0 || wave_idx >= m_wave_count || wave_idx >= GUILD_FB_WAVE_MAX) 
	{
		return false;
	}
	
	GuildFbWaveCfg *wave_cfg = &m_wave_list[wave_idx];

	if (pos_idx < 0 || pos_idx >= wave_cfg->guard_pt_count || pos_idx >= GUILD_FB_GUARD_PATH_POINT_MAX)
	{
		return false;
	}

	if (NULL != pos)
	{
		*pos = wave_cfg->guard_path_ptlist[pos_idx];
		return true;
	}

	return false;
}

GuildFbWaveCfg * GuildFbConfig::GetGuildFbWaveCfg(int wave_idx)
{
	if (wave_idx < 0 || wave_idx >= m_wave_count || wave_idx >= GUILD_FB_WAVE_MAX) 
	{
		return NULL;
	}

	return &m_wave_list[wave_idx];
}









