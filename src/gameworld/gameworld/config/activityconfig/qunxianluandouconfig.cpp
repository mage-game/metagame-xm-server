#include "qunxianluandouconfig.hpp"

#include "servercommon/configcommon.h"

#include "checkresourcecenter.hpp"

#include "monster/monsterpool.h"
#include "monster/monsterinitparam.h"
#include "item/itempool.h"

QunXianLuanDouConfig::QunXianLuanDouConfig()
	: m_score_reward_cfg_count(0)
{
	memset(m_realive_poslist, 0, sizeof(m_realive_poslist));
	memset(m_defender_poslist, 0, sizeof(m_defender_poslist));
	memset(m_defender_monster_id_list, 0, sizeof(m_defender_monster_id_list));
	memset(m_rank_user_reward_title_id_list, 0, sizeof(m_rank_user_reward_title_id_list));
}

QunXianLuanDouConfig::~QunXianLuanDouConfig()
{

}

bool QunXianLuanDouConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = { 0 };
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "QunXianLuanDouConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// 其他配置信息
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = path + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 复活点配置信息
		PugiXmlNode realive_element = RootElement.child("relive_pos");
		if (realive_element.empty())
		{
			*err = path + ": no [relive_pos].";
			return false;
		}

		iRet = this->InitPosCfg(realive_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitPosCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 阵营奖励
		PugiXmlNode side_element = RootElement.child("side_reward_cfg");
		if (side_element.empty())
		{
			*err = path + ": no [side_reward_cfg].";
			return false;
		}

		iRet = this->InitSideRewardCfg(side_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitSideRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 积分奖励
		PugiXmlNode reward_element = RootElement.child("reward");
		if (reward_element.empty())
		{
			*err = path + ": no [reward].";
			return false;
		}

		iRet = this->InitScoreRewardCfg(reward_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitScoreRewardCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 排名奖励
		PugiXmlNode reward_element = RootElement.child("rank_title");
		if (reward_element.empty())
		{
			*err = path + ": no [rank_title].";
			return false;
		}

		iRet = this->InitRankTitleCfg(reward_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitRankTitleCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int QunXianLuanDouConfig::GetEnterInfo(int side, Posi *enterpos)
{
	if (QUNXIANLUANDOU_SIDE_1 <= side && side < QUNXIANLUANDOU_SIDE_MAX && NULL != enterpos) *enterpos = m_realive_poslist[side];
	return m_other_cfg.scene_id;
}

bool QunXianLuanDouConfig::GetRealivePos(int side, Posi *realive_pos)
{
	if (side < QUNXIANLUANDOU_SIDE_1 || side >= QUNXIANLUANDOU_SIDE_MAX || NULL == realive_pos) return false;

	*realive_pos = m_realive_poslist[side];
	return true;
}

bool QunXianLuanDouConfig::GetDefenderPos(int side, Posi *defender_pos)
{
	if (side < QUNXIANLUANDOU_SIDE_1 || side >= QUNXIANLUANDOU_SIDE_MAX || NULL == defender_pos) return false;

	*defender_pos = m_defender_poslist[side];
	return true;
}

int QunXianLuanDouConfig::GetDefenderMonsterId(int side)
{
	if (side < QUNXIANLUANDOU_SIDE_1 || side >= QUNXIANLUANDOU_SIDE_MAX) return 0;
	return m_defender_monster_id_list[side];
}

bool QunXianLuanDouConfig::GetShenShiBirthInfo(int *hp, Posi *birth_pos)
{
	if (NULL == hp || NULL == birth_pos) return false;

	*hp = m_other_cfg.shenshi_hp;
	*birth_pos = m_other_cfg.shenshi_birthpos;
	return true;
}

const QunXianLuanDouConfig::ScoreRewardItem * QunXianLuanDouConfig::GetScoreRewardHonor(int score)
{
	for (int i = m_score_reward_cfg_count - 1; i >= 0; -- i)
	{
		if (score >= m_score_reward_cfg_list[i].need_score)
		{
			return &m_score_reward_cfg_list[i];
		}
	}

	return NULL;
}

const QXLDSideRewardConfig::ConfigItem * QunXianLuanDouConfig::GetSideRewardCfg(int role_level)
{
	for (int i = 0; i < m_side_reward_cfg.cfg_count; ++ i)
	{
		if (role_level >= m_side_reward_cfg.cfg_list[i].role_min_level && role_level <= m_side_reward_cfg.cfg_list[i].role_max_level)
		{
			return &m_side_reward_cfg.cfg_list[i];
		}
	}

	return NULL;
}

int QunXianLuanDouConfig::GetOpenServerReward(ItemConfigData reward[MAX_ATTACHMENT_ITEM_NUM])
{
	if (NULL == reward) return 0;

	int i = 0;
	for(;i < m_other_cfg.first_reward_item_count  && i < MAX_ATTACHMENT_ITEM_NUM; i++)
	{
		reward[i].item_id = m_other_cfg.first_reward[i].item_id;
		reward[i].num = m_other_cfg.first_reward[i].num;
		reward[i].is_bind = m_other_cfg.first_reward[i].is_bind;
	}

	return i;
}

int QunXianLuanDouConfig::GetTitleID(int side)
{
	if (side < 0 || side >= QUNXIANLUANDOU_SIDE_MAX)
	{
		return 0;
	}

	return m_rank_user_reward_title_id_list[side];
}

int QunXianLuanDouConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -1;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.scene_id);

	if (!PugiGetSubNodeValue(data_element, "room_max_role_num", m_other_cfg.room_max_role_num) || m_other_cfg.room_max_role_num <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "lucky_interval", m_other_cfg.lucky_interval) || m_other_cfg.lucky_interval <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "luck_people", m_other_cfg.luck_people_num) || m_other_cfg.luck_people_num < 0)
	{
		return -4;
	}
	
	PugiXmlNode Itemelement = data_element.child("lucky_item");
	if (Itemelement.empty())
	{
		return -5;
	}

	if (!m_other_cfg.lucky_item.ReadConfig(Itemelement))
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "shenshi_hp", m_other_cfg.shenshi_hp) || m_other_cfg.shenshi_hp <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "shenshi_birthpos_x", m_other_cfg.shenshi_birthpos.x) || m_other_cfg.shenshi_birthpos.x <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "shenshi_birthpos_y", m_other_cfg.shenshi_birthpos.y) || m_other_cfg.shenshi_birthpos.y <= 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(data_element, "rob_shenshi_honor", m_other_cfg.robshenshi_honor) || m_other_cfg.robshenshi_honor < 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(data_element, "rob_shenshi_sidehonor", m_other_cfg.robshenshi_sidehonor) || m_other_cfg.robshenshi_sidehonor < 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "kill_honor", m_other_cfg.kill_honor) || m_other_cfg.kill_honor < 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(data_element, "assist_honor", m_other_cfg.assist_honor) || m_other_cfg.assist_honor < 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(data_element, "free_reward_interval_s", m_other_cfg.free_reward_interval_s) || m_other_cfg.free_reward_interval_s < 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(data_element, "free_reward_honor", m_other_cfg.free_reward_honor) || m_other_cfg.free_reward_honor < 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(data_element, "robot_create_start_interval", m_other_cfg.robot_create_start_interval) || m_other_cfg.robot_create_start_interval < 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(data_element, "robot_create_interval", m_other_cfg.robot_create_interval) || m_other_cfg.robot_create_interval < 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(data_element, "robot_max_count", m_other_cfg.robot_max_count) || m_other_cfg.robot_max_count < 0)
	{
		return -18;
	}

	if (!PugiGetSubNodeValue(data_element, "robot_birth_pos_x", m_other_cfg.robot_birth_pos.x) || m_other_cfg.robot_birth_pos.x <= 0)
	{
		return -19;
	}

	if (!PugiGetSubNodeValue(data_element, "robot_birth_pos_y", m_other_cfg.robot_birth_pos.y) || m_other_cfg.robot_birth_pos.y <= 0)
	{
		return -20;
	}

	{
		// 开服争霸第一名奖励
		PugiXmlNode element = data_element.child("first_reward_list");
		if (element.empty())
		{
			return -21;
		}

		m_other_cfg.first_reward_item_count = 0;
		PugiXmlNode item_element = element.child("first_reward");
		while (!item_element.empty())
		{
			if (m_other_cfg.first_reward_item_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -200;
			}

			if (!m_other_cfg.first_reward[m_other_cfg.first_reward_item_count].ReadConfig(item_element))
			{
				return -200 - m_other_cfg.first_reward_item_count;
			}

			++m_other_cfg.first_reward_item_count;
			item_element = item_element.next_sibling();
		}
	}

	return 0;
}

int QunXianLuanDouConfig::InitPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_realivepos_side = -1;
	while (!data_element.empty())
	{
		int realivepos_side = 0;
		if (!PugiGetSubNodeValue(data_element, "side", realivepos_side) || realivepos_side < QUNXIANLUANDOU_SIDE_1 || realivepos_side >= QUNXIANLUANDOU_SIDE_MAX || realivepos_side != last_realivepos_side + 1)
		{
			return -1;
		}
		last_realivepos_side = realivepos_side;

		Posi &realive_pos = m_realive_poslist[realivepos_side];

		if (!PugiGetSubNodeValue(data_element, "pos_x", realive_pos.x) || realive_pos.x <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "pos_y", realive_pos.y) || realive_pos.y <= 0)
		{
			return -3;
		}

		Posi &defender_pos = m_defender_poslist[realivepos_side];

		if (!PugiGetSubNodeValue(data_element, "defender_x", defender_pos.x) || defender_pos.x <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "defender_y", defender_pos.y) || defender_pos.y <= 0)
		{
			return -5;
		}

		int monster_id = 0;
		if (!PugiGetSubNodeValue(data_element, "defender_monster_id", monster_id) || NULL == MONSTERPOOL->GetMonsterParam(monster_id))
		{
			return -6;
		}
		if (MonsterInitParam::AITYPE_SPECIALAI_BATTLEFIELD != MONSTERPOOL->GetMonsterParam(monster_id)->ai_type)
		{
			return -7;
		}
		m_defender_monster_id_list[realivepos_side] = monster_id;

		data_element = data_element.next_sibling();
	}

	if (QUNXIANLUANDOU_SIDE_MAX - 1 != last_realivepos_side)
	{
		return -4;
	}

	return 0;
}

int QunXianLuanDouConfig::InitSideRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_side_reward_cfg.cfg_count = 0;

	int last_role_level = 0;

	while (!dataElement.empty())
	{
		if (m_side_reward_cfg.cfg_count >= QXLDSideRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}

		QXLDSideRewardConfig::ConfigItem &cfg_item = m_side_reward_cfg.cfg_list[m_side_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "role_level_min", cfg_item.role_min_level) || cfg_item.role_min_level != last_role_level + 1)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "role_level_max", cfg_item.role_max_level) || cfg_item.role_max_level < cfg_item.role_min_level)
		{
			return -3;
		}
		last_role_level = cfg_item.role_max_level;

		if (!PugiGetSubNodeValue(dataElement, "win_honor", cfg_item.win_honor) || cfg_item.win_honor < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "win_exp", cfg_item.win_exp) || cfg_item.win_exp < 0)
		{
			return -5;
		}

		PugiXmlNode win_item = dataElement.child("win_item");
		if (win_item.empty() || !cfg_item.win_item.ReadConfig(win_item))
		{
			return -6;
		}

// 		if (!PugiGetSubNodeValue(dataElement, "win_item_id", cfg_item.win_item_id) || NULL == ITEMPOOL->GetItem(cfg_item.win_item_id))
// 		{
// 			return -6;
// 		}

		if (!PugiGetSubNodeValue(dataElement, "lose_honor", cfg_item.lose_honor) || cfg_item.lose_honor < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "lose_exp", cfg_item.lose_exp) || cfg_item.lose_exp < 0)
		{
			return -8;
		}

		PugiXmlNode lose_item = dataElement.child("lose_item");
		if (lose_item.empty() || !cfg_item.lose_item.ReadConfig(lose_item))
		{
			return -9;
		}
// 		if (!PugiGetSubNodeValue(dataElement, "lose_item_id", cfg_item.lose_item_id) || NULL == ITEMPOOL->GetItem(cfg_item.lose_item_id))
// 		{
// 			return -9;
// 		}

		++m_side_reward_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}


int QunXianLuanDouConfig::InitScoreRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_score_reward_cfg_count = 0;

	int last_need_score = -1; int last_reward_honor = 0; int last_index = 0;

	while (!dataElement.empty())
	{
		if (m_score_reward_cfg_count >= SCORE_REWARD_ITEM_MAX_COUNT)
		{
			return -1;
		}

		ScoreRewardItem &cfg_item = m_score_reward_cfg_list[m_score_reward_cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "reward_index", cfg_item.reward_index) || cfg_item.reward_index <= 0 || cfg_item.reward_index >= SCORE_REWARD_ITEM_MAX_COUNT || cfg_item.reward_index < last_index)
		{
			return -2;
		}
		last_index = cfg_item.reward_index;

		if (!PugiGetSubNodeValue(dataElement, "need_score_min", cfg_item.need_score) || cfg_item.need_score <= last_need_score)
		{
			return -3;
		}
		last_need_score = cfg_item.need_score;

		if (!PugiGetSubNodeValue(dataElement, "reward_honor", cfg_item.reward_honor) || cfg_item.reward_honor < last_reward_honor)
		{
			return -4;
		}
		last_reward_honor = cfg_item.reward_honor;

		{
			PugiXmlNode element = dataElement.child("reward_item_s_list");
			if (element.empty())
			{	
				return -100;
			}

			cfg_item.reward_item_count = 0;
			PugiXmlNode item_element = element.child("reward_item_s");
			while (!item_element.empty())
			{
				if (cfg_item.reward_item_count >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -200;
				}

				if (!cfg_item.reward_item[cfg_item.reward_item_count].ReadConfig(item_element))
				{
					return -200 - cfg_item.reward_item_count;
				}

				++cfg_item.reward_item_count;
				item_element = item_element.next_sibling();
			}
		}

		++m_score_reward_cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int QunXianLuanDouConfig::InitRankTitleCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int side = -1;
		if (!PugiGetSubNodeValue(dataElement, "side", side) || side < 0 || side >= QUNXIANLUANDOU_SIDE_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "title_id", m_rank_user_reward_title_id_list[side]) || m_rank_user_reward_title_id_list[side] <= 0)
		{
			return -2;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}