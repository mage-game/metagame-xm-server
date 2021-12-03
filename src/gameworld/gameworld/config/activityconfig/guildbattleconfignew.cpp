#include "guildbattleconfignew.hpp"
#include "monster/monsterpool.h"
#include "monster/monsterinitparam.h"
#include "item/itempool.h"

#include "servercommon/configcommon.h"

#include "checkresourcecenter.hpp"
#include <string>

GuildBattleConfigNew::GuildBattleConfigNew() : m_warning_interval_s(0)
{
	m_warning_percent_list[0] = GUILD_BATTLE_FLAG_HP_WARNING_20;
	m_warning_percent_list[1] = GUILD_BATTLE_FLAG_HP_WARNING_50;
}

GuildBattleConfigNew::~GuildBattleConfigNew()
{
}

bool GuildBattleConfigNew::Init(const std::string &configname, std::string *err)
{
	char err_info[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "GuildBattleConfigNew", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + " xml root node error";
		return false;
	}

	{	// 其他配置
		PugiXmlNode root_elment = RootElement.child("other");
		if (NULL == root_elment)
		{
			*err = configname + "xml not other node";
			return false;
		}
		iRet = this->InitGuildBattleOtherCfg(root_elment);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitGuildBattleOtherCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// 个人积分奖励配置
		PugiXmlNode root_element = RootElement.child("personal_credit_reward");
		if (root_element.empty())
		{
			*err = configname + "xml not personal_credit_reward node";
			return false;
		}

		iRet = this->InitGuildBattlePersonalRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info,"%s InitGuildBattlePersonalRewardCfg fail %d",configname.c_str(),iRet);
			*err = err_info;
			return false;
		}
	}

	{	// 积分排名奖励配置
		PugiXmlNode root_element = RootElement.child("rank_reward");
		if (root_element.empty())
		{
			*err = configname + "xml not rank_reward node";
			return false;
		}

		iRet =  this->InitGuildBattleRankRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info,"%s InitGuildBattleRankRewardCfg fail %d",configname.c_str(),iRet);
			*err = err_info;
			return false;
		}
	}

	{	// 据点配置
		PugiXmlNode root_element = RootElement.child("point");
		if (root_element.empty())
		{
			*err = configname + "xml not point node";
			return false;
		}

		iRet = this->InitGuildBattlePointCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitGuildBattlePointCfg fail %d", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// 每日奖励
		PugiXmlNode root_element = RootElement.child("daily_reward");
		if (root_element.empty())
		{
			*err = configname + "xml not daily_reward node";
			return false;
		}

		iRet = this->InitDailyRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitDailyRewardCfg fail %d", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// 求救配置
		PugiXmlNode root_element = RootElement.child("sos_cfg");
		if (root_element.empty())
		{
			*err = configname + "xml not sos_cfg node";
			return false;
		}

		iRet = this->InitSosCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitSosCfg fail %d", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// 复活点配置
		PugiXmlNode root_element = RootElement.child("realive_pos_cfg");
		if (root_element.empty())
		{
			*err = configname + "xml not realive_pos_cfg node";
			return false;
		}

		iRet = this->InitRealivePosCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitRealivePosCfg fail %d", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int GuildBattleConfigNew::GetGuildBattleEnterInfo(Posi *pos)
{
	if (NULL == pos) return 0;

	int size = static_cast<int>(m_realive_pos_cfg_vec.size());
	if (0 == size)
	{
		return 0;
	}

	*pos = m_realive_pos_cfg_vec[RandomNum(0, size)];

	return m_other_cfg.scene_id;
}

int GuildBattleConfigNew::GetGuildBattleEnterStandy(Posi *pos) const
{
	if (NULL == pos) return 0;

	if (m_realive_pos_cfg_vec.size() <= 0)
	{
		return 0;
	}

	*pos = m_realive_pos_cfg_vec[0];

	return m_other_cfg.scene_id;
}

const GuildBattlePersonalRewardCfg * GuildBattleConfigNew::GetPersonalRewardCfg(int require_credit) const
{
	if (require_credit <= 0) return NULL;

	std::map<int,GuildBattlePersonalRewardCfg>::const_iterator iter = m_personal_reward_list.begin();

	for (;iter != m_personal_reward_list.end();iter++)
	{
		if (iter->second.require_credit_min <= require_credit && require_credit <= iter->second.require_credit_max)
		{
			return &iter->second;
		}
	}
 
 	return NULL;
}

const GuildBattlePersonalRewardCfg * GuildBattleConfigNew::GetPersonalRewardCfgByIndex(int index) const
{
	if (index <= 0) return NULL;

	std::map<int,GuildBattlePersonalRewardCfg>::const_iterator iter = m_personal_reward_list.begin();

	for (;iter != m_personal_reward_list.end();iter++)
	{
		if (iter->second.reward_index == index)
		{
			return &iter->second;
		}
	}

	return NULL;
}

const GuildBattleRankRewardCfg * GuildBattleConfigNew::GetRankRewardCfg(int rank)const
{
	if (rank < 0) return NULL;

	const std::map<int, GuildBattleRankRewardCfg>::const_iterator iter = m_rank_reward_list.find(rank);
	if (m_rank_reward_list.end() == iter)
		return NULL;
	
	return &iter->second;
}

const GuildBattlePointCfg * GuildBattleConfigNew::GetGuildBattlePointCfg(int index) const
{
	if (index < 0 || index >= static_cast<int>(m_point_vec.size())) return NULL;

	return &m_point_vec[index];
}

const ItemConfigData * GuildBattleConfigNew::GetDailyReward(int rank)
{
	if (rank <= 0 || rank > GUILD_BATTLE_NEW_REWARD_CITY_COUNT)
	{
		return nullptr;
	}

	return m_daily_reward_item_list[rank - 1];
}

bool GuildBattleConfigNew::IsFlag(int monster_id)
{
	for (auto it = m_point_vec.begin(); it != m_point_vec.end(); ++it)
	{
		if (it->boss_id == monster_id)
		{
			return true;
		}
	}

	return false;
}

int GuildBattleConfigNew::GetSosCost(int times)
{
	if (times < 0 || times >= static_cast<int>(m_sos_cost_cfg_vec.size()))
	{
		return -1;
	}

	return m_sos_cost_cfg_vec[times];
}

Posi GuildBattleConfigNew::GetClosestRealivePos(Posi pos)
{
	Posi result;
	float distance = 999999.0f;	//随便一个很大的数
	for (Posi realive_pos : m_realive_pos_cfg_vec)
	{
		float dist = realive_pos.Dist(pos);
		if (dist < distance)
		{
			distance = dist;
			result = realive_pos;
		}
	}

	return result;
}

bool GuildBattleConfigNew::CheckWarning(int old_percent, int now_percent, int *warning_percent)
{
	for (int i = 0; i < WARNING_HP_LIST_LEN; ++i)
	{
		if (old_percent >= m_warning_percent_list[i] && now_percent < m_warning_percent_list[i])
		{
			if (NULL != warning_percent) *warning_percent = m_warning_percent_list[i];
			return true;
		}
	}

	return false;
}

int GuildBattleConfigNew::InitGuildBattleOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if(!PugiGetSubNodeValue(dataElement,"scene_id",m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -1;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.scene_id);

	if (!PugiGetSubNodeValue(dataElement,"realive_pos_x",m_other_cfg.realive_pos.x) || m_other_cfg.realive_pos.x <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement,"realive_pos_y",m_other_cfg.realive_pos.y) || m_other_cfg.realive_pos.y <= 0)
	{
		return -3;
	}

	{
		PugiXmlNode element = dataElement.child("huizhang_extra_reward_list");
		if (element.empty())
		{
			return -100;
		}

		int i = 0;
		PugiXmlNode item_element = element.child("huizhang_extra_reward");
		while (!item_element.empty())
		{
			if (i >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -101 - i;
			}

			if (!m_other_cfg.huizhang_extra_reward[i].ReadConfig(item_element))
			{
				return - 151 - i;
			}

			i++;
			m_other_cfg.huizhang_extra_reward_item_count++;
			item_element = item_element.next_sibling();
		}
	}

	if (!PugiGetSubNodeValue(dataElement, "lucky_interval", m_other_cfg.lucky_interval) || m_other_cfg.lucky_interval <= 0)
	{
		return -4;
	}

	{
		PugiXmlNode Itemelement = dataElement.child("lucky_item");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid) )
		{
			if (!m_other_cfg.lucky_item.ReadConfig(Itemelement))
			{
				return -5;
			}

		}else
		{
			return -6;
		}
	}

	if (!PugiGetSubNodeValue(dataElement, "luck_people", m_other_cfg.luck_people_num) || m_other_cfg.luck_people_num <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "get_score_interva", m_other_cfg.get_score_interva) || m_other_cfg.get_score_interva < 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement, "point_guild_score", m_other_cfg.point_guild_score) || m_other_cfg.point_guild_score < 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElement, "point_person_score", m_other_cfg.point_person_score) || m_other_cfg.point_person_score < 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElement, "big_point_guild_score", m_other_cfg.big_point_guild_score) || m_other_cfg.big_point_guild_score < 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(dataElement, "big_point_personscore", m_other_cfg.big_point_personscore) || m_other_cfg.big_point_personscore < 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(dataElement, "hurt_point_score", m_other_cfg.hurt_point_score) || m_other_cfg.hurt_point_score < 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(dataElement, "kill_score", m_other_cfg.kill_score) || m_other_cfg.kill_score < 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(dataElement, "assist_score", m_other_cfg.assist_score) || m_other_cfg.assist_score < 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(dataElement, "recalc_time", m_other_cfg.recalc_time) || m_other_cfg.recalc_time < 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(dataElement, "shield_times", m_other_cfg.shield_times) || m_other_cfg.shield_times < 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(dataElement, "mount_special_image_id", m_other_cfg.mount_special_image_id) || m_other_cfg.mount_special_image_id < 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(dataElement, "warning_interval_s", m_warning_interval_s) || m_warning_interval_s < 0)
	{
		return -18;
	}

	if (!PugiGetSubNodeValue(dataElement, "opengame_day", m_other_cfg.opengame_day) || m_other_cfg.opengame_day < 0)
	{
		return -19;
	}

	if (!PugiGetSubNodeValue(dataElement, "get_score_interva_person", m_other_cfg.get_score_interva_person) || m_other_cfg.get_score_interva_person < 0)
	{
		return -20;
	}

	if (!PugiGetSubNodeValue(dataElement, "effective_distance_person", m_other_cfg.effective_distance_person) || m_other_cfg.effective_distance_person < 0)
	{
		return -21;
	}

	return 0;
}

int GuildBattleConfigNew::InitGuildBattlePersonalRewardCfg(PugiXmlNode RootElemen)
{
	PugiXmlNode dataElement = RootElemen.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_require_credit_min = 0;
	int last_index = 0;
	
	while(!dataElement.empty())
	{
		int require_credit_min = 0;
		if (!PugiGetSubNodeValue(dataElement, "reward_credit_min" ,require_credit_min) || require_credit_min < 0 || require_credit_min <= last_require_credit_min)
		{
			return -1;
		}

		if (m_personal_reward_list.end() != m_personal_reward_list.find(require_credit_min))
		{
			return -100;
		}

		GuildBattlePersonalRewardCfg &cfg = m_personal_reward_list[require_credit_min];
		cfg.require_credit_min = require_credit_min;
		last_require_credit_min = require_credit_min;

		if (!PugiGetSubNodeValue(dataElement,"require_credit_max",cfg.require_credit_max) || cfg.require_credit_max <= 0 || cfg.require_credit_min > cfg.require_credit_max)
		{
			return -2;
		}

		{
			PugiXmlNode element = dataElement.child("reward_item_list");
			if (element.empty())
			{
				return -100;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("reward_item");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -101 - i;
				}

				if (!cfg.reward_item[i].ReadConfig(item_element))
				{
					return - 151 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}

		if (!PugiGetSubNodeValue(dataElement,"banggong",cfg.banggong) || cfg.banggong < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement,"reward_index",cfg.reward_index) || cfg.reward_index <= 0 || cfg.reward_index > 31 || cfg.reward_index < last_index)
		{
			return -4;
		}
		last_index = cfg.reward_index;
		
		if (!PugiGetSubNodeValue(dataElement,"shengwang",cfg.shengwang) || cfg.shengwang < 0)
		{
			return -5;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildBattleConfigNew::InitGuildBattleRankRewardCfg(PugiXmlNode RootElemen)
{
	PugiXmlNode dataElement = RootElemen.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_rank = 0;
	while(!dataElement.empty())
	{
		int rank;
		if (!PugiGetSubNodeValue(dataElement,"rank",rank) || rank < 0 || rank < last_rank)
		{
			return -1;
		}

		if (m_rank_reward_list.end() != m_rank_reward_list.find(rank))
		{
			return -100;
		}

		GuildBattleRankRewardCfg &cfg = m_rank_reward_list[rank];
		cfg.rank = rank;
		last_rank = rank;

		if (!PugiGetSubNodeValue(dataElement,"base_role_exp",cfg.base_role_exp) || cfg.base_role_exp < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement,"banggong",cfg.banggong) || cfg.banggong < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement,"shengwang",cfg.shengwang) || cfg.shengwang < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongzi", cfg.gongzi) || cfg.gongzi < 0)
		{
			return -7;
		}

		{
			PugiXmlNode element = dataElement.child("reward_item_list");
			if (element.empty())
			{
				return -100;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("reward_item");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -101 - i;
				}

				if (!cfg.reward_item[i].ReadConfig(item_element))
				{
					return - 151 - i;
				}

				i++;
				cfg.reward_item_count++;
				item_element = item_element.next_sibling();
			}
		}

		{
			cfg.collective_reward_item_count = ItemConfigData::ReadConfigList(dataElement, "collective_reward_item", cfg.collective_reward_item, MAX_ATTACHMENT_ITEM_NUM);
			if (cfg.collective_reward_item_count < 0)
			{
				return -200 - cfg.collective_reward_item_count;
			}
		}

		dataElement = dataElement.next_sibling();
	}
	
	return 0;
}

int GuildBattleConfigNew::InitGuildBattlePointCfg(PugiXmlNode RootElemen)
{
	PugiXmlNode dataElement = RootElemen.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}
	
	while (!dataElement.empty())
	{

		GuildBattlePointCfg point_cfg;

		if (!PugiGetSubNodeValue(dataElement, "boss_id", point_cfg.boss_id) || !MONSTERPOOL->IsMonsterExist(point_cfg.boss_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x", point_cfg.realive_pos.x) || point_cfg.realive_pos.x < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", point_cfg.realive_pos.y) || point_cfg.realive_pos.y < 0)
		{
			return -3;
		}

		m_point_vec.push_back(point_cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildBattleConfigNew::InitDailyRewardCfg(PugiXmlNode RootElemen)
{
	PugiXmlNode dataElement = RootElemen.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int rank = 0;
		if (!PugiGetSubNodeValue(dataElement, "rank", rank) || rank <= 0 || rank > GUILD_BATTLE_NEW_REWARD_CITY_COUNT)
		{
			return -1;
		}

		int item_count = ItemConfigData::ReadConfigList(dataElement, "reward_item", m_daily_reward_item_list[rank - 1], MAX_ATTACHMENT_ITEM_NUM);
		if (item_count < 0)
		{
			return -100 - item_count;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildBattleConfigNew::InitSosCfg(PugiXmlNode RootElemen)
{
	PugiXmlNode dataElement = RootElemen.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int times = -1;
		if (!PugiGetSubNodeValue(dataElement, "times", times) || times < 0 || times != static_cast<int>(m_sos_cost_cfg_vec.size()))
		{
			return -1;
		}

		int cost = -1;
		if (!PugiGetSubNodeValue(dataElement, "cost", cost) || cost < 0)
		{
			return -2;
		}

		m_sos_cost_cfg_vec.push_back(cost);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int GuildBattleConfigNew::InitRealivePosCfg(PugiXmlNode RootElemen)
{
	PugiXmlNode dataElement = RootElemen.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		Posi pos;
		if (!PugiGetSubNodeValue(dataElement, "realive_pos_x", pos.x) || pos.x < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "realive_pos_y", pos.y) || pos.y < 0)
		{
			return -2;
		}

		m_realive_pos_cfg_vec.push_back(pos);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}