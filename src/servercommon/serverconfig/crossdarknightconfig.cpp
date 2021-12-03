#include "crossdarknightconfig.hpp"
#include "servercommon/configcommon.h"

#include <algorithm>

CrossDarkNightConfig::CrossDarkNightConfig() : m_activity_open_time_count(0), m_rand_pos_count(0)
{

}

CrossDarkNightConfig::~CrossDarkNightConfig()
{

}

CrossDarkNightConfig & CrossDarkNightConfig::Instance()
{
	static CrossDarkNightConfig _instance;
	return _instance;
}


bool CrossDarkNightConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = { 0 };

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossDarkNightConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// 其他配置
		PugiXmlNode OtherElement = RootElement.child("other_cfg");
		if (OtherElement.empty())
		{
			*err = path + ": xml node error in [config->other]";
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
		// 活动开启配置
		PugiXmlNode ActivityOpenElement = RootElement.child("activity_open_cfg");
		if (ActivityOpenElement.empty())
		{
			*err = path + ": xml node error in [config->activity_open_cfg]";
			return false;
		}

		iRet = this->InitActivityOpenCfg(ActivityOpenElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitActivityOpenCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 怪物刷新点配置
		PugiXmlNode RandPosiElement = RootElement.child("rand_posi_cfg");
		if (RandPosiElement.empty())
		{
			*err = path + ": xml node error in [config->rand_posi_cfg]";
			return false;
		}

		iRet = this->InitRandPosiCfg(RandPosiElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitRandPosiCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 怪物配置
		PugiXmlNode MonsterElement = RootElement.child("monster_cfg");
		if (MonsterElement.empty())
		{
			*err = path + ": xml node error in [config->monster_cfg]";
			return false;
		}

		iRet = this->InitMonsterCfg(MonsterElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitMonsterCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// Boss配置
		PugiXmlNode BossElement = RootElement.child("boss_cfg");
		if (BossElement.empty())
		{
			*err = path + ": xml node error in [config->boss_cfg]";
			return false;
		}

		iRet = this->InitBossCfg(BossElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitBossCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宝箱配置
		PugiXmlNode BoxElement = RootElement.child("box_cfg");
		if (BoxElement.empty())
		{
			*err = path + ": xml node error in [config->box_cfg]";
			return false;
		}

		iRet = this->InitBoxCfg(BoxElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitBoxCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 积分配置
		PugiXmlNode ScoreElement = RootElement.child("score_cfg");
		if (ScoreElement.empty())
		{
			*err = path + ": xml node error in [config->score_cfg]";
			return false;
		}

		iRet = this->InitScoreCfg(ScoreElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitScoreCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 复活点
		PugiXmlNode RealivePosElement = RootElement.child("create_pos");
		if (RealivePosElement.empty())
		{
			*err = path + ": xml node error in [config->create_pos]";
			return false;
		}

		iRet = this->InitRealivePosiCfg(RealivePosElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitRealivePosiCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宝箱刷新位置
		PugiXmlNode RealivePosElement = RootElement.child("box_rand_pos");
		if (RealivePosElement.empty())
		{
			*err = path + ": xml node error in [config->box_rand_pos]";
			return false;
		}

		iRet = this->InitBoxFreshPosiCfg(RealivePosElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitBoxFreshPosiCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int CrossDarkNightConfig::InitOtherCfg(PugiXmlNode Rootelement)
{
	PugiXmlNode dataElement = Rootelement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int is_open = 0;
	if (!PugiGetSubNodeValue(dataElement, "is_open", is_open) || (is_open != 0 && 1 != is_open))
	{
		return -1;
	}
	m_other_cfg.is_open = (0 != is_open);

	if (!PugiGetSubNodeValue(dataElement, "open_level", m_other_cfg.open_level) || m_other_cfg.open_level <= 0 || m_other_cfg.open_level > MAX_ROLE_LEVEL)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "born_posi_x", m_other_cfg.born_posi.x) || m_other_cfg.born_posi.x <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "born_posi_y", m_other_cfg.born_posi.y) || m_other_cfg.born_posi.y <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "initiative_leave_reward_percent", m_other_cfg.leave_reward_percent) || m_other_cfg.leave_reward_percent <= 0 ||
		m_other_cfg.leave_reward_percent > 100)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "flush_monster_interval_s", m_other_cfg.flush_monster_interval_s) || m_other_cfg.flush_monster_interval_s <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "score_reward_interval_s", m_other_cfg.score_reward_interval_s) || m_other_cfg.score_reward_interval_s <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement, "reward_score", m_other_cfg.reward_score) || m_other_cfg.reward_score <= 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElement, "kill_role_score", m_other_cfg.kill_role_score) || m_other_cfg.kill_role_score <= 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElement, "role_assis_score", m_other_cfg.role_assis_score) || m_other_cfg.role_assis_score <= 0)
	{
		return -11;
	}

	return 0;
}

int CrossDarkNightConfig::InitActivityOpenCfg(PugiXmlNode Rootelement)
{
	PugiXmlNode dataElement = Rootelement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int pre_activity_week_day = 0;
	int pre_activity_end_time = 0;

	while (!dataElement.empty())
	{
		if (m_activity_open_time_count >= CROSS_DARK_NIGHT_OPEN_TIME_MAX_COUNT)
		{
			return -1;
		}

		CrossDarkNightOpenTimeInfo &open_cfg = m_activity_open_cfg_list[m_activity_open_time_count];

		if (!PugiGetSubNodeValue(dataElement, "activity_week_day", open_cfg.activity_week_day) || open_cfg.activity_week_day < SUNDAY || open_cfg.activity_week_day > SATURDAY)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "activity_ready_time", open_cfg.activity_ready_time) || open_cfg.activity_ready_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_ready_time))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "activity_start_time", open_cfg.activity_start_time) || open_cfg.activity_start_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_start_time))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "activity_end_time", open_cfg.activity_end_time) || open_cfg.activity_end_time < 0 || this->DayTimeFormatInvalid(open_cfg.activity_end_time))
		{
			return -5;
		}

		if (open_cfg.activity_week_day < pre_activity_week_day)
		{
			return -6;
		}
		else if (open_cfg.activity_week_day == pre_activity_week_day)
		{
			if (open_cfg.activity_ready_time <= pre_activity_end_time)
			{
				return -7;
			}
		}

		pre_activity_end_time = open_cfg.activity_end_time;
		pre_activity_week_day = open_cfg.activity_week_day;

		int ready_time_sec = int(open_cfg.activity_ready_time / 100) * 3600 + (open_cfg.activity_ready_time % 100) * 60;
		int start_time_sec = int(open_cfg.activity_start_time / 100) * 3600 + (open_cfg.activity_start_time % 100) * 60;
		int end_time_sec = int(open_cfg.activity_end_time / 100) * 3600 + (open_cfg.activity_end_time % 100) * 60;

		open_cfg.ready_dur_s = start_time_sec - ready_time_sec;
		open_cfg.open_dur_s = end_time_sec - start_time_sec;

		if (open_cfg.ready_dur_s < 0 || open_cfg.open_dur_s <= 0)
		{
			return -9;
		}

		++ m_activity_open_time_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossDarkNightConfig::InitBoxCfg(PugiXmlNode Rootelement)
{
	PugiXmlNode dataElement = Rootelement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_id", m_box_cfg.gather_id) || m_box_cfg.gather_id < 0)
	{
		return -2;
	}
	
	if (!PugiGetSubNodeValue(dataElement, "another_reward_times", m_box_cfg.another_reward_times) || m_box_cfg.another_reward_times < 0)
	{
		return -3;
	}

	{
		PugiXmlNode item_element = dataElement.child("reward_item");
		if (NULL == item_element)
		{
			return -4;
		}

		if (!m_box_cfg.reward_item.ReadConfigNoCheck(item_element))
		{
			return -5;
		}
	}

	if (!PugiGetSubNodeValue(dataElement, "gathe_time_s", m_box_cfg.gather_time_s) || m_box_cfg.gather_time_s <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "disappear_time_s", m_box_cfg.disappear_time_s) || m_box_cfg.disappear_time_s < 0)
	{
		return -7;
	}

	{
		PugiXmlNode item2_element = dataElement.child("reward_item2");
		if (NULL == item2_element)
		{
			return -8;
		}

		if (!m_box_cfg.reward_item2.ReadConfigNoCheck(item2_element))
		{
			return -9;
		}
	}

	return 0;
}

int CrossDarkNightConfig::InitMonsterCfg(PugiXmlNode Rootelement)
{
	PugiXmlNode dataElement = Rootelement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		if (m_monster_cfg.monster_count >= CROSS_DARK_NIGHT_MONSTER_MAX_COUNT)
		{
			return -1;
		}

		CrossDarkNightMonsterCfg &cfg = m_monster_cfg.monste_list[m_monster_cfg.monster_count];
		if (!PugiGetSubNodeValue(dataElement, "monster_id", cfg.monster_id) || cfg.monster_id <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_num", cfg.reward_num) || cfg.reward_num <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "box_rate", cfg.box_rate) || cfg.box_rate < 0 || cfg.box_rate > 100)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "score", cfg.score) || cfg.score < 0)
		{
			return -9;
		}

		m_monster_cfg.monster_count++;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossDarkNightConfig::InitBossCfg(PugiXmlNode Rootelement)
{
	PugiXmlNode dataElement = Rootelement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int pos_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "pos_index", pos_index) || pos_index < 0 || pos_index >= CROSS_DARK_NIGHT_BOSS_POS_INDEX_MAX)
		{
			return -100;
		}
		CrossDarkNightBossCfgInfo &info_cfg = m_boss_cfg[pos_index];

		int seq_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "seq_index", seq_index) || pos_index < 0 || pos_index >= CROSS_DARK_NIGHT_BOSS_MAX_COUNT)
		{
			return 101;
		}

		if (seq_index != info_cfg.boss_count)
		{
			return 102;
		}
		info_cfg.boss_list[info_cfg.boss_count].seq_index = seq_index;

		CrossDarkNightBossCfg &cfg = info_cfg.boss_list[info_cfg.boss_count];

		if (!PugiGetSubNodeValue(dataElement, "monster_id", cfg.boss_id) || cfg.boss_id < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "rand_posi_index_begin", cfg.rand_posi_index_begin) || cfg.rand_posi_index_begin < 0 ||
			cfg.rand_posi_index_begin >= m_rand_pos_count)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "rand_posi_index_end", cfg.rand_posi_index_end) || cfg.rand_posi_index_end < 0 ||
			cfg.rand_posi_index_end < cfg.rand_posi_index_begin || cfg.rand_posi_index_end >= m_rand_pos_count)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "score", cfg.score) || cfg.score < 0)
		{
			return -6;
		}

		if (!cfg.scene_role_num.ReadConfig(dataElement, "scene_role_num"))
		{
			return -7;
		}

		if (!cfg.scene_role_num.Check(NumListConfig::CHECK_POLICY_GRANTER_EQUAL_0))
		{
			return -8;
		}

		if (!cfg.scene_role_num.Check(NumListConfig::CHECK_POLICY_UP_ORDER))
		{
			return -9;
		}

		if (!cfg.reward_num.ReadConfig(dataElement, "reward_num"))
		{
			return -10;
		}

		if (!cfg.reward_num.Check(NumListConfig::CHECK_POLICY_GRANTER_EQUAL_0))
		{
			return -11;
		}

		if (!cfg.reward_num.Check(NumListConfig::CHECK_POLICY_UP_ORDER))
		{
			return -12;
		}

		if (!cfg.drop_num.ReadConfig(dataElement, "drop_num"))
		{
			return -13;
		}

		if (!cfg.drop_num.Check(NumListConfig::CHECK_POLICY_GRANTER_EQUAL_0))
		{
			return -14;
		}

		if (!cfg.drop_num.Check(NumListConfig::CHECK_POLICY_UP_ORDER))
		{
			return -13;
		}

		//drop_num和reward_num都要根据scene_role_num(场景中人数)来刷，所以元素个数不少于scene_role_num
		if (cfg.drop_num.Count() < cfg.scene_role_num.Count() || cfg.reward_num.Count() < cfg.scene_role_num.Count())
		{
			return -16;
		}

		if (!cfg.drop_pos_list.ReadConfig(dataElement, "drop_pos_list"))
		{
			return -17;
		}

		info_cfg.boss_count++;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossDarkNightConfig::InitRandPosiCfg(PugiXmlNode Rootelement)
{
	PugiXmlNode dataElement = Rootelement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		if (m_rand_pos_count >= CROSS_PASTURE_RELIVE_POS_MAX_COUNT)
		{
			return -1;
		}

		Posi &player_pos = m_monster_create_pos_list[m_rand_pos_count];

		if (!PugiGetSubNodeValue(dataElement, "x_pos", player_pos.x) || player_pos.x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "y_pos", player_pos.y) || player_pos.y <= 0)
		{
			return -4;
		}

		++m_rand_pos_count;
		dataElement = dataElement.next_sibling();
	}

	if (m_rand_pos_count <= 0)
	{
		return -5;
	}

	return 0;
}

int CrossDarkNightConfig::InitRealivePosiCfg(PugiXmlNode Rootelement)
{
	PugiXmlNode dataElement = Rootelement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		Posi temp;
		if (!PugiGetSubNodeValue(dataElement, "x_pos", temp.x) || temp.x <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "y_pos", temp.y) || temp.y <= 0)
		{
			return -2;
		}

		m_realive_posi_vec.push_back(temp);
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossDarkNightConfig::InitBoxFreshPosiCfg(PugiXmlNode Rootelement)
{
	PugiXmlNode dataElement = Rootelement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		Posi temp;
		if (!PugiGetSubNodeValue(dataElement, "pos_x", temp.x) || temp.x <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", temp.y) || temp.y <= 0)
		{
			return -2;
		}

		m_box_fresh_posi_vec.push_back(temp);
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CrossDarkNightConfig::InitScoreCfg(PugiXmlNode Rootelement)
{
	PugiXmlNode dataElement = Rootelement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		CrossDarkNightScoreCfg temp_cfg;

		if (!PugiGetSubNodeValue(dataElement, "score", temp_cfg.score) || temp_cfg.score <= 0)
		{
			return -1;
		}

		{
			int item_count = ItemConfigData::ReadConfigListNoCheck(dataElement, std::string("reward_item"), temp_cfg.reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
			if (item_count < 0)
			{
				return -2100 + item_count;
			}

			temp_cfg.item_count = item_count;
		}

		if (!PugiGetSubNodeValue(dataElement, "cross_honor", temp_cfg.cross_honor) || temp_cfg.cross_honor < 0)
		{
			return -2;
		}
		
		m_score_cfg_vec.push_back(temp_cfg);
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

bool CrossDarkNightConfig::DayTimeFormatInvalid(int day_time)
{
	int hour = day_time / 100;
	int minute = day_time % 100;

	return (hour < 0 || hour >= 24 || minute < 0 || minute >= 60);
}

Posi CrossDarkNightConfig::GetRandomMonsterCreatePosByMonsterId(int monster_id)
{
	const CrossDarkNightBossCfg *monster_cfg = this->GetBossInfoById(monster_id);
	if (NULL != monster_cfg)
	{
		int rand_index = RandomNum(monster_cfg->rand_posi_index_begin, monster_cfg->rand_posi_index_end + 1);

		if (rand_index >= 0 && rand_index < m_rand_pos_count)
		{
			return m_monster_create_pos_list[rand_index];
		}
	}

	return Posi(0, 0);
}

const CrossDarkNightMonsterCfg *CrossDarkNightConfig::GetMonsterInfoCfg(int monster_id)
{
	for (int i = 0; i < m_monster_cfg.monster_count; i ++)
	{
		if (monster_id == m_monster_cfg.monste_list[i].monster_id)
		{
			return &m_monster_cfg.monste_list[i];
		}
	}

	return NULL;
}

const CrossDarkNightOpenTimeInfo *CrossDarkNightConfig::GetActivityOpenTimeInfo(int index)
{
	if (index >= 0 && index < m_activity_open_time_count && index < CROSS_DARK_NIGHT_OPEN_TIME_MAX_COUNT)
	{
		return &m_activity_open_cfg_list[index];
	}

	return NULL;
}

int CrossDarkNightConfig::GetOpenActivityConfig(CrossDarkNightOpenTimeInfo cfg_list[CROSS_DARK_NIGHT_OPEN_TIME_MAX_COUNT])
{
	memcpy(cfg_list, m_activity_open_cfg_list, sizeof(m_activity_open_cfg_list));
	return m_activity_open_time_count;
}

const CrossDarkNightBossCfg *CrossDarkNightConfig::GetBossInfoById(int boss_id)
{
	if (boss_id <= 0)
	{
		return NULL;
	}

	for (int i = 0; i < CROSS_DARK_NIGHT_BOSS_POS_INDEX_MAX; i++)
	{
		for (int j = 0; j < m_boss_cfg[i].boss_count; ++j)
		{
			if (boss_id == m_boss_cfg[i].boss_list[j].boss_id)
			{
				return &m_boss_cfg[i].boss_list[j];
			}
		}
	}

	return NULL;
}

const CrossDarkNightBossCfg *CrossDarkNightConfig::GetBossInfoByIndex(int pos_index, int index)
{
	if (pos_index < 0 || pos_index >= CROSS_DARK_NIGHT_BOSS_POS_INDEX_MAX)
	{
		return NULL;
	}

	if (index < 0 || index >= m_boss_cfg[pos_index].boss_count)
	{
		return NULL;
	}

	return &m_boss_cfg[pos_index].boss_list[index];
}

void CrossDarkNightConfig::GetDropPosiListById(int boss_id, std::vector<Posi> &rand_pos_list)
{
	for (int i = 0; i < CROSS_DARK_NIGHT_BOSS_POS_INDEX_MAX; i++)
	{
		for (int j = 0; j < m_boss_cfg[i].boss_count; ++j)
		{
			if (boss_id == m_boss_cfg[i].boss_list[j].boss_id)
			{
				for (int index = 0; index < m_boss_cfg[i].boss_list[j].drop_pos_list.Count(); index ++)
				{
					Posi tmp_posi = *m_boss_cfg[i].boss_list[j].drop_pos_list.Value(index);
					rand_pos_list.push_back(tmp_posi);
				}
				break;
			}
		}
	}

	if (!rand_pos_list.empty())
	{
		std::random_shuffle(rand_pos_list.begin(), rand_pos_list.end());
	}
}

Posi CrossDarkNightConfig::GetRandRealivePosi() 
{
	int rand_index = RandomNum(m_realive_posi_vec.size());

	if (rand_index >=0 && rand_index < (int)m_realive_posi_vec.size())
	{
		return m_realive_posi_vec.at(rand_index);
	}

	return Posi(0, 0);
}

Posi CrossDarkNightConfig::GetRealivePosiByKey(unsigned int key) const
{
	if (m_realive_posi_vec.size() <= 0)
	{
		return Posi(0, 0);
	}

	if (key >= m_realive_posi_vec.size())
	{
		int rand_index = RandomNum(m_realive_posi_vec.size());

		return m_realive_posi_vec[rand_index];
	}

	return m_realive_posi_vec[key];
}

Posi CrossDarkNightConfig::GetRandFreshBoxPosi()
{
	int rand_index = RandomNum(m_box_fresh_posi_vec.size());

	if (rand_index >= 0 && rand_index < (int)m_box_fresh_posi_vec.size())
	{
		return m_box_fresh_posi_vec.at(rand_index);
	}

	return Posi(0, 0);
}

void CrossDarkNightConfig::GetBossDropAndRewardNumByRoleNum(int *drop_num, int *reward_num, const CrossDarkNightBossCfg * boss_cfg, int role_num)
{
	if (drop_num == nullptr || reward_num == nullptr || boss_cfg == nullptr || 
		boss_cfg->scene_role_num.Count() <= 0 || boss_cfg->drop_num.Count() <= 0 || boss_cfg->reward_num.Count() <= 0)
	{
		return;
	}

	int stage = - 1;

	for (int i = boss_cfg->scene_role_num.Count() - 1; i >= 0; --i)
	{
		int num = *boss_cfg->scene_role_num.Value(i);
		if (role_num >= num)
		{
			stage = i;
			break;
		}
	}

	if (stage < 0) return;

	if (stage >= boss_cfg->drop_num.Count())
	{
		*drop_num = *boss_cfg->drop_num.Value(boss_cfg->drop_num.Count() - 1);
	}
	else
	{
		*drop_num = *boss_cfg->drop_num.Value(stage);
	}

	if (stage >= boss_cfg->reward_num.Count())
	{
		*reward_num = *boss_cfg->reward_num.Value(boss_cfg->reward_num.Count() - 1);
	}
	else
	{
		*reward_num = *boss_cfg->reward_num.Value(stage);
	}
}