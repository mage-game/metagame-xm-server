#include "holidayguardconfig.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "checkresourcecenter.hpp"
#include "monster/monsterpool.h"
#include <set>
#include <string>
#include "config/logicconfigmanager.hpp"
#include <algorithm>

HolidayGuardConfig::HolidayGuardConfig() : m_npc_cfg_count(0)
{
	UNSTD_STATIC_CHECK(sizeof(m_warning_percent_list) / sizeof(m_warning_percent_list[0]) >= 2);

	m_warning_percent_list[0] = HOLIDAYGUARD_WARNING_PERCENT_50;
	m_warning_percent_list[1] = HOLIDAYGUARD_WARNING_PERCENT_10;

}

HolidayGuardConfig::~HolidayGuardConfig()
{

}

bool HolidayGuardConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = { 0 };

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "HolidayGuardConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// 其他
		PugiXmlNode OtherElement = RootElement.child("other");
		if (OtherElement.empty())
		{
			*err = path + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(OtherElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 关卡配置
		PugiXmlNode level_element = RootElement.child("level_scene_cfg");
		if (level_element.empty())
		{
			*err = path + ": no [level_scene_cfg].";
			return false;
		}

		iRet = this->InitLevelCfg(level_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitLevelCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 波次
		PugiXmlNode wave_element = RootElement.child("wave_list");
		if (wave_element.empty())
		{
			*err = path + ": no [wave_list].";
			return false;
		}

		iRet = this->InitWaveCfg(wave_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitWaveCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 路线
		PugiXmlNode path_element = RootElement.child("path_list");
		if (path_element.empty())
		{
			*err = path + ": no [path_list].";
			return false;
		}

		iRet = this->InitPathCfg(path_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitPathCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// buff掉落配置
		PugiXmlNode buff_drop_weight_element = RootElement.child("buff_drop_weight");
		if (buff_drop_weight_element.empty())
		{
			*err = path + ": no [buff_drop_weight].";
			return false;
		}

		iRet = this->InitBuffDropWeightCfg(buff_drop_weight_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitBuffDropWeightCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// buff配置
		PugiXmlNode buff_element = RootElement.child("buff_config");
		if (buff_element.empty())
		{
			*err = path + ": no [buff_config].";
			return false;
		}

		iRet = this->InitBuffCfg(buff_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitBuffCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	// 副本入口npc配置
	{
		PugiXmlNode enter_element = RootElement.child("enter_config");
		if (enter_element.empty())
		{
			*err = path + ": no [enter_config].";
			return false;
		}

		iRet = this->InitNpcCfg(enter_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitNpcCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{  // npc刷新限制
		PugiXmlNode npc_refresh_element = RootElement.child("npc_refresh_limit");
		if (npc_refresh_element.empty())
		{
			*err = path + ": no [npc_refresh_limit].";
			return false;
		}

		iRet = this->InitNpcLimitCfg(npc_refresh_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitNpcLimitCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int HolidayGuardConfig::InitWaveCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_wave_list[HolidayGuardLevelCfg::CONFIG_ITEM_MAX_COUNT];
	for (int i = 0; i < HolidayGuardLevelCfg::CONFIG_ITEM_MAX_COUNT; ++i)
	{
		last_wave_list[i] = -1;
	}

	while (!data_element.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0 || level >= HolidayGuardLevelCfg::CONFIG_ITEM_MAX_COUNT || level >= m_level_cfg.cfg_count)
		{
			return -1;
		}

		HolidayGuardLevelCfg::ConfigItem &cfg_item = m_level_cfg.cfg_list[level];
		int &last_wave = last_wave_list[level];

		int wave = 0;
		if (!PugiGetSubNodeValue(data_element, "wave", wave) || wave != last_wave + 1 || wave >= PERSONAL_HOLIDAYGUARD_MAX_WAVE_PER_LEVEL)
		{
			return -2;
		}
		last_wave = wave;

		HolidayGuardWaveCfg &wave_cfg = cfg_item.wave_list[wave];
		wave_cfg.wave = wave;

		if (!PugiGetSubNodeValue(data_element, "next_wave_s", wave_cfg.next_wave_s) || wave_cfg.next_wave_s <= 0)
		{
			return -3;
		}

		{
			char tmp_tag[32];
			for (int i = HOLIDAYGUARD_PATH_LEFT; i < HOLIDAYGUARD_PATH_MAX; ++i)
			{
				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "monster_id_%d", i);
				if (!PugiGetSubNodeValue(data_element, tmp_tag, wave_cfg.monster_id_list[i]) || wave_cfg.monster_id_list[i] < 0)
				{
					return -20;
				}

				if (wave_cfg.monster_id_list[i] > 0 && !MONSTERPOOL->IsMonsterExist(wave_cfg.monster_id_list[i]))
				{
					return -21;
				}

				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "monster_count_%d", i);
				if (!PugiGetSubNodeValue(data_element, tmp_tag, wave_cfg.monster_count_list[i]) || wave_cfg.monster_count_list[i] < 0)
				{
					return -25;
				}

				if (wave_cfg.monster_id_list[i] > 0 && 0 == wave_cfg.monster_count_list[i])
				{
					return -26;
				}

				wave_cfg.total_monster_count += wave_cfg.monster_count_list[i];
			}
		}

		++cfg_item.wave_count;

		data_element = data_element.next_sibling();
	}

	for (int i = 0; i < m_level_cfg.cfg_count; ++i)
	{
		if (!this->CheckLevelMonsterID(m_level_cfg.cfg_list[i]))
		{
			return -300000;
		}
	}

	return 0;
}

int HolidayGuardConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_level_cfg.cfg_count = 0;

	int last_level = -1;

	while (!data_element.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level != last_level + 1 || level >= HolidayGuardLevelCfg::CONFIG_ITEM_MAX_COUNT)
		{
			return -2;
		}
		last_level = level;

		HolidayGuardLevelCfg::ConfigItem &cfg_item = m_level_cfg.cfg_list[level];
		cfg_item.level = level;

		if (!PugiGetSubNodeValue(data_element, "scene_id", cfg_item.scene_id) || cfg_item.scene_id <= 0)
		{
			return -3;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg_item.scene_id);

		if (!PugiGetSubNodeValue(data_element, "birth_pos_x", cfg_item.birth_pos.x) || cfg_item.birth_pos.x <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "birth_pos_y", cfg_item.birth_pos.y) || cfg_item.birth_pos.y <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "need_level", cfg_item.role_level) || cfg_item.role_level <= 0)
		{
			return -7;
		}

		++m_level_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int HolidayGuardConfig::InitPathCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int path = 0;
		if (!PugiGetSubNodeValue(data_element, "path_idx", path) || path <= HOLIDAYGUARD_PATH_INVALID || path >= HOLIDAYGUARD_PATH_MAX)
		{
			return -1;
		}

		{
			char tmp_tag[32];
			for (int i = 0; i < HOLIDAYGUARD_PATH_POINT_COUNT; ++i)
			{
				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "pos_x_%d", i + 1);
				if (!PugiGetSubNodeValue(data_element, tmp_tag, wave_pathway_list[path][i].x) || wave_pathway_list[path][i].x < 0)
				{
					return -10;
				}

				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "pos_y_%d", i + 1);
				if (!PugiGetSubNodeValue(data_element, tmp_tag, wave_pathway_list[path][i].y) || wave_pathway_list[path][i].y < 0)
				{
					return -11;
				}
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int HolidayGuardConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "free_join_times", m_other_cfg.free_join_times) || m_other_cfg.free_join_times < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "first_wave_s", m_other_cfg.first_wave_refresh_delay) || m_other_cfg.first_wave_refresh_delay < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "refresh_interval", m_other_cfg.wave_refresh_interval) || m_other_cfg.wave_refresh_interval < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "life_tower_monster_id", m_other_cfg.life_tower_monster_id) || m_other_cfg.life_tower_monster_id < 0 ||
			!MONSTERPOOL->IsMonsterExist(m_other_cfg.life_tower_monster_id))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "warning_interval_s", m_other_cfg.warning_interval_s) || m_other_cfg.warning_interval_s < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "sync_interval_s", m_other_cfg.sync_interval_s) || m_other_cfg.sync_interval_s < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "buff_exit_time", m_other_cfg.buff_exit_time) || m_other_cfg.buff_exit_time <= 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "begin_time", m_other_cfg.fb_begin_time) || m_other_cfg.fb_begin_time < 0)
		{
			return -11;
		}

		if(!PugiGetSubNodeValue(data_element, "end_time", m_other_cfg.fb_end_time) || m_other_cfg.fb_end_time < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(data_element, "npc_refresh_interval", m_other_cfg.npc_refresh_interval) || m_other_cfg.npc_refresh_interval <= 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(data_element, "npc_refresh_count", m_other_cfg.npc_refresh_count) || m_other_cfg.npc_refresh_count <= 0)
		{
			return -16;
		}

		{
			PugiXmlNode leave_fb_reward_list = data_element.child("leave_fb_reward_list");
			if (leave_fb_reward_list.empty())
			{
				return -17;
			}

			PugiXmlNode leave_fb_reward = leave_fb_reward_list.child("leave_fb_reward");
			if (leave_fb_reward.empty())
			{
				return -18;
			}

			int i = 0;

			while (!leave_fb_reward.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -19;
				}

				if (!m_other_cfg.leave_fb_reward_list_cfg[i].ReadConfig(leave_fb_reward))
				{
					return -20;
				}

				++i;
				++m_other_cfg.leave_fb_reward_count;
				leave_fb_reward = leave_fb_reward.next_sibling();
			}
		}

		if (!PugiGetSubNodeValue(data_element, "everyday_can_fetch_reward_count", m_other_cfg.everyday_can_fetch_reward_count) || m_other_cfg.everyday_can_fetch_reward_count < 0)
		{
			return -21;
		}

		if (!PugiGetSubNodeValue(data_element, "time_within_tread_finish_refresh", m_other_cfg.time_within_tread_finish_refresh) || m_other_cfg.time_within_tread_finish_refresh <= 0)
		{
			return -22;
		}
	}

	return 0;
}

int HolidayGuardConfig::InitBuffDropWeightCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int monster_id = 0;
		if (!PugiGetSubNodeValue(data_element, "monster_id", monster_id) || monster_id < 0 || nullptr == MONSTERPOOL->GetMonsterParam(monster_id))
		{
			return -1;
		}

		if (m_buff_drop_weight_map.find(monster_id) != m_buff_drop_weight_map.end())
		{
			return -2;
		}

		HolidayGuardBuffDropWeightCfg &cfg = m_buff_drop_weight_map[monster_id];
		cfg.monster_id = monster_id;

		char temp_str[32] = { 0 };
		for (int i = 0; i < HOLIDAYGUARD_BUFF_DROP_WEIGHT_COUNT; i++)
		{
			sprintf(temp_str, "weight_%d", i + 1);
			if (!PugiGetSubNodeValue(data_element, temp_str, cfg.weight[i]) || cfg.weight[i] < 0)
			{
				return -3;
			}

			cfg.total_weight += cfg.weight[i];
		}

		if (0 >= cfg.total_weight)
		{
			return -4;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int HolidayGuardConfig::InitBuffCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int buff_type = 0;
		if (!PugiGetSubNodeValue(data_element, "buff_type", buff_type) || buff_type <= HOLIDAY_GUARD_BUFF_TYPE_INVALID || buff_type >= HOLIDAY_GUARD_BUFF_TYPE_MAX)
		{
			return -1;
		}

		if (m_buff_cfg_map.find(buff_type) != m_buff_cfg_map.end())
		{
			return -2;
		}

		HolidayGuardBuffCfg &cfg = m_buff_cfg_map[buff_type];
		cfg.buff_type = buff_type;

		char temp_str[32] = { 0 };
		for (int i = 0; i < 4; i++)
		{
			sprintf(temp_str, "param_%d", i);
			if (!PugiGetSubNodeValue(data_element, temp_str, cfg.param[i]) || cfg.param[i] < 0)
			{
				return -3;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int HolidayGuardConfig::InitNpcCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		if (m_npc_cfg_count >= HOLIDAYGUARD_NPC_CFG_MAX_COUNT)
		{
			return -1;
		}

		HolidayGuardNpcRefreshCfg &tmp_cfg = m_npc_cfg_list[m_npc_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "index", tmp_cfg.index) || tmp_cfg.index < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "scene_id", tmp_cfg.scene_id) || tmp_cfg.scene_id < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "npc_id", tmp_cfg.npc_id) || tmp_cfg.npc_id < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "npc_x", tmp_cfg.coordinate_x) || tmp_cfg.coordinate_x < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "npc_y", tmp_cfg.coordinate_y) || tmp_cfg.coordinate_y < 0)
		{
			return -6;
		}

		++m_npc_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int HolidayGuardConfig::InitNpcLimitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int scene_id = 0;
	int npc_count = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "scene_id", scene_id) || scene_id <= 0 || m_scene_npc_count_map.find(scene_id) != m_scene_npc_count_map.end())
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "limit_count", npc_count) || npc_count <= 0)
		{
			return -3;
		}

		m_scene_npc_count_map.emplace(std::make_pair(scene_id, npc_count));

		data_element = data_element.next_sibling();
	}

	return 0;
}

int HolidayGuardConfig::GetWaveCountBySceneID(int scene_id) const
{
	const HolidayGuardLevelCfg::ConfigItem * cfg_item = this->GetLevelCfgBySceneID(scene_id);
	if (nullptr == cfg_item)
	{
		return 0;
	}

	return cfg_item->wave_count;
}

const HolidayGuardWaveCfg * HolidayGuardConfig::GetWaveCfg(int scene_id, int wave) const
{
	if (scene_id <= 0 || wave < 0 || wave >= PERSONAL_HOLIDAYGUARD_MAX_WAVE_PER_LEVEL)
	{
		return nullptr;
	}

	const HolidayGuardLevelCfg::ConfigItem * cfg_item = this->GetLevelCfgBySceneID(scene_id);
	if (nullptr != cfg_item)
	{
		if (wave < cfg_item->wave_count)
		{
			return &cfg_item->wave_list[wave];
		}
	}

	return nullptr;
}

const HolidayGuardWaveCfg * HolidayGuardConfig::GetWaveCfgByMonsterID(int scene_id, int monster_id) const
{
	if (monster_id <= 0 || scene_id <= 0)
	{
		return nullptr;
	}

	const HolidayGuardLevelCfg::ConfigItem *cfg_item = this->GetLevelCfgBySceneID(scene_id);
	if (nullptr == cfg_item)
	{
		return nullptr;
	}

	for (int i = 0; i < cfg_item->wave_count; ++i)
	{
		const HolidayGuardWaveCfg *wave_cfg = &cfg_item->wave_list[i];

		for (int j = 0; j < HOLIDAYGUARD_PATH_MAX; ++j)
		{
			if (wave_cfg->monster_id_list[j] == monster_id)
			{
				return wave_cfg;
			}
		}
	}

	return nullptr;
}

Posi HolidayGuardConfig::GetBirthPos(int path) const
{
	if (path <= HOLIDAYGUARD_PATH_INVALID || path >= HOLIDAYGUARD_PATH_MAX)
	{
		return wave_pathway_list[HOLIDAYGUARD_PATH_MID][0];
	}

	return wave_pathway_list[path][0];
}

bool HolidayGuardConfig::GetNextPosi(int birth_path, const Posi &curr_pos, Posi *pos)
{
	UNSTD_STATIC_CHECK(HOLIDAYGUARD_PATH_MAX > 0);

	birth_path = birth_path % HOLIDAYGUARD_PATH_MAX;

	Posi last_posi = wave_pathway_list[birth_path][HOLIDAYGUARD_PATH_POINT_COUNT - 1];

	for (int count_i = 0; count_i < HOLIDAYGUARD_PATH_MAX; count_i++)
	{
		int i = (count_i + birth_path) % HOLIDAYGUARD_PATH_MAX;

		bool is_find = false;
		for (int j = 0; j < HOLIDAYGUARD_PATH_POINT_COUNT - 1; j++)
		{
			Posi pos1 = wave_pathway_list[i][j], pos2 = wave_pathway_list[i][j + 1];
			Posi deltaPos = pos2 - pos1;

			float dir = atan2((float)deltaPos.y, (float)deltaPos.x);
			dir = -dir;
			float distance = sqrt((float)(deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y));

			Posi offofset_pos = curr_pos - pos1;

			float x = offofset_pos.x * cos(dir) - offofset_pos.y * sin(dir);
			float y = offofset_pos.x * sin(dir) + offofset_pos.y * cos(dir);
			if (x >= -2.0f && x <= distance + 2.0f && y >= -20.0f && y <= 20.0f)
			{
				is_find = true;
				if (nullptr != pos) *pos = pos2;
			}
		}
		if (is_find) return true;
	}

	if (nullptr != pos) *pos = last_posi;

	return false;
}

bool HolidayGuardConfig::CheckWarning(int old_percent, int now_percent, int *warning_percent)
{
	for (int i = 0; i < HOLIDAYGUARD_WARNING_PERCENT_COUNT; ++i)
	{
		if (old_percent >= m_warning_percent_list[i] && now_percent < m_warning_percent_list[i])
		{
			if (nullptr != warning_percent) *warning_percent = m_warning_percent_list[i];
			return true;
		}
	}

	return false;
}

const HolidayGuardLevelCfg::ConfigItem * HolidayGuardConfig::GetLevelCfgBySceneID(int scene_id) const
{
	for (int i = 0; i < m_level_cfg.cfg_count && i < HolidayGuardLevelCfg::CONFIG_ITEM_MAX_COUNT; ++i)
	{
		if (m_level_cfg.cfg_list[i].scene_id == scene_id)
		{
			return &m_level_cfg.cfg_list[i];
		}
	}

	return nullptr;
}

const HolidayGuardLevelCfg::ConfigItem * HolidayGuardConfig::GetLevelCfg(int level) const
{
	if (level < 0 || level >= HolidayGuardLevelCfg::CONFIG_ITEM_MAX_COUNT || level >= m_level_cfg.cfg_count)
	{
		return nullptr;
	}

	return &m_level_cfg.cfg_list[level];
}

HolidayGuardBuffDropWeightCfg * HolidayGuardConfig::GetBuffDropCfg(int monster_id)
{
	std::map<int, HolidayGuardBuffDropWeightCfg>::iterator iter = m_buff_drop_weight_map.find(monster_id);
	if (iter != m_buff_drop_weight_map.end() && iter->second.monster_id == monster_id)
	{
		return &iter->second;
	}

	return nullptr;
}

HolidayGuardBuffCfg * HolidayGuardConfig::GetBufffCfg(int buff_type)
{
	std::map<int, HolidayGuardBuffCfg>::iterator iter = m_buff_cfg_map.find(buff_type);
	if (iter != m_buff_cfg_map.end() && iter->second.buff_type == buff_type)
	{
		return &iter->second;
	}

	return nullptr;
}

bool HolidayGuardConfig::CheckLevelMonsterID(const HolidayGuardLevelCfg::ConfigItem &cfg_item)
{
	std::set<int> monster_id_list;

	for (int j = 0; j < cfg_item.wave_count; ++j)
	{
		const HolidayGuardWaveCfg &wave_cfg = cfg_item.wave_list[j];
		std::set<int> cur_wave_monster_id_list;

		for (int k = 0; k < HOLIDAYGUARD_PATH_MAX; ++k)
		{
			if (wave_cfg.monster_id_list[k] > 0)
			{
				cur_wave_monster_id_list.insert(wave_cfg.monster_id_list[k]);
			}
		}

		if (cur_wave_monster_id_list.size() <= 0)
		{
			return false;
		}

		for (std::set<int>::iterator it = cur_wave_monster_id_list.begin(); cur_wave_monster_id_list.end() != it; ++it)
		{
			if (monster_id_list.end() != monster_id_list.find(*it))
			{
				return false;
			}

			monster_id_list.insert(*it);
		}
	}

	return true;
}

bool HolidayGuardConfig::GetFBBeginTimeAndEndTime(unsigned int *begin_time, unsigned int *end_time)
{
	if (nullptr == begin_time || nullptr == end_time)
	{
		return false;
	}

	*begin_time = m_other_cfg.fb_begin_time;
	*end_time = m_other_cfg.fb_end_time;

	return true;
}

int HolidayGuardConfig::GetNpcCfg(HolidayGuardNpcRefreshCfg *get_npc_cfg_list, int max_get_count)
{
	if (nullptr == get_npc_cfg_list)
	{
		return 0;
	}

	max_get_count = (std::min)(max_get_count, m_npc_cfg_count);
	memcpy(get_npc_cfg_list, m_npc_cfg_list, sizeof(HolidayGuardNpcRefreshCfg) * max_get_count);
	return max_get_count;
}

int HolidayGuardConfig::GetLeaveFbReward(ItemConfigData *leave_fb_reward, int length)
{
	if (nullptr == leave_fb_reward)
	{
		return 0;
	}

	int i = 0;
	for (; i < m_other_cfg.leave_fb_reward_count && i < length; ++i)
	{
		leave_fb_reward[i] = m_other_cfg.leave_fb_reward_list_cfg[i];
	}

	return i;
}