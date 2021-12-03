#include "xianmengzhanconfig.hpp"

#include "monster/monsterpool.h"
#include "monster/monsterinitparam.h"
#include "item/itempool.h"

#include "servercommon/configcommon.h"

#include "checkresourcecenter.hpp"

XianMengZhanConfig::XianMengZhanConfig()
: m_scene_id(0), 
m_guild_call_count_limit(0), m_guild_call_gold_cost(0), m_guild_call_cd_time(0), 
m_center_area_pos(0, 0), m_center_area_range(0), m_has_center_reward_data(0), m_has_center_reward_interval(0), 
m_center_area_gather_id(0), m_defend_area_reward_interval(0), m_reward_has_defend_area(0), 
m_reward_center_area_kill(0), m_reward_center_area_dead(0), m_reward_not_center_area_kill(0), m_reward_not_center_area_dead(0),
m_reward_rank_gift_count(0), m_reward_guild_max_rank(0), m_free_jifen_interval_s(0), m_free_jifen(0)
{
	memset(m_realive_area_list, 0, sizeof(m_realive_area_list));
}

XianMengZhanConfig::~XianMengZhanConfig()
{
}

bool XianMengZhanConfig::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "XianMengZhanConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{ // 读取 其他配置

		PugiXmlNode OtherElement = RootElement.child("other");
		if (OtherElement.empty())
		{
			*err = path + ": xml node error in [other].";
			return false;
		}

		PugiXmlNode DataElement = OtherElement.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml node error in [config->other->data].";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "scene_id", m_scene_id))
		{
			*err = path + ": xml node error in [config->other->data->scene_id].";
			return false;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(m_scene_id);

		if (!PugiGetSubNodeValue(DataElement, "guild_call_count_limit", m_guild_call_count_limit) || m_guild_call_count_limit <= 0)
		{
			*err = path + ": xml node error in [config->other->data->guild_call_count_limit]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "guild_call_gold_cost", m_guild_call_gold_cost) || m_guild_call_gold_cost <= 0)
		{
			*err = path + ": xml node error in [config->other->data->guild_call_gold_cost]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "guild_call_cd_time", m_guild_call_cd_time) || m_guild_call_cd_time < 0)
		{
			*err = path + ": xml node error in [config->other->data->guild_call_cd_time]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "center_area_pos_x", m_center_area_pos.x) || m_center_area_pos.x <= 0)
		{
			*err = path + ": xml node error in [config->other->data->center_area_pos_x]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "center_area_pos_y", m_center_area_pos.y) || m_center_area_pos.y <= 0)
		{
			*err = path + ": xml node error in [config->other->data->center_area_pos_y]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "center_area_range", m_center_area_range) || m_center_area_range <= 0)
		{
			*err = path + ": xml node error in [config->other->data->center_area_range]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "center_area_reward_interval", m_has_center_reward_interval) || m_has_center_reward_interval <= 0)
		{
			*err = path + ": xml node error in [config->other->data->center_area_reward_interval]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "reward_has_center_area", m_has_center_reward_data) || m_has_center_reward_data <= 0)
		{
			*err = path + ": xml node error in [config->other->data->reward_has_center_area]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "center_gather_id", m_center_area_gather_id) || m_center_area_gather_id <= 0)
		{
			*err = path + ": xml node error in [config->other->data->center_gather_id]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "defend_area_reward_interval", m_defend_area_reward_interval) || m_defend_area_reward_interval <= 0)
		{
			*err = path + ": xml node error in [config->other->data->defend_area_reward_interval]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "reward_has_defend_area", m_reward_has_defend_area) || m_reward_has_defend_area <= 0)
		{
			*err = path + ": xml node error in [config->other->data->reward_has_defend_area]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "reward_center_area_kill", m_reward_center_area_kill) || m_reward_center_area_kill < 0)
		{
			*err = path + ": xml node error in [config->other->data->reward_center_area_kill]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "reward_center_area_dead", m_reward_center_area_dead) || m_reward_center_area_dead < 0)
		{
			*err = path + ": xml node error in [config->other->data->reward_center_area_dead]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "reward_not_center_area_kill", m_reward_not_center_area_kill) || m_reward_not_center_area_kill < 0)
		{
			*err = path + ": xml node error in [config->other->data->reward_not_center_area_kill]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "reward_not_center_area_dead", m_reward_not_center_area_dead) || m_reward_not_center_area_dead < 0)
		{
			*err = path + ": xml node error in [config->other->data->reward_not_center_area_dead]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "free_jifen_interval_s", m_free_jifen_interval_s) || m_free_jifen_interval_s <= 0)
		{
			*err = path + ": xml node error in [config->other->data->free_jifen_interval_s]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "free_jifen", m_free_jifen) || m_free_jifen <= 0)
		{
			*err = path + ": xml node error in [config->other->data->free_jifen]";
			return false;
		}
	}

	{   // 读取 复活点列表

		PugiXmlNode RealiveAreaElement = RootElement.child("realive_area");
		if (RealiveAreaElement.empty())
		{
			*err = path + ": xml node error in [config->realive_area]";
			return false;
		}

		PugiXmlNode DataElement = RealiveAreaElement.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml node error in [config->realive_area->data]";
			return false;
		}

		int realive_area_size = 0;

		while (!DataElement.empty())
		{
			if (realive_area_size >= XIANMENGZHAN_REALIVE_AREA_NUM) {
				*err = path + ": xml node error in [config->realive_area->realive_area_size]";
				return false;
			}

			Posi &realive_pos = m_realive_area_list[realive_area_size++];

			if (!PugiGetSubNodeValue(DataElement, "pos_x", realive_pos.x) || realive_pos.x <= 0)
			{
				*err = path + ": xml node error in [config->realive_area->realive_pos->pos_x]";
				return false;
			}
			if (!PugiGetSubNodeValue(DataElement, "pos_y", realive_pos.y) || realive_pos.y <= 0)
			{
				*err = path + ": xml node error in [config->realive_area->realive_pos->pos_y]";
				return false;
			}

			DataElement = DataElement.next_sibling();
		}

		if (XIANMENGZHAN_REALIVE_AREA_NUM != realive_area_size) {
			*err = path + ": xml node error in [config->realive_area->realive_area_size]";
			return false;
		}
	}

	{   // 读取 守卫据点列表

		PugiXmlNode DefendAreaElement = RootElement.child("defend_area");
		if (DefendAreaElement.empty())
		{
			*err = path + ": xml node error in [config->defend_area]";
			return false;
		}

		PugiXmlNode DataElement = DefendAreaElement.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml node error in [config->defend_area->data]";
			return false;
		}

		int defend_area_size = 0;

		while (!DataElement.empty())
		{
			if (defend_area_size >= XIANMENGZHAN_DEFEND_AREA_NUM) {
				*err = path + ": xml node error in [config->defend_area->defend_area_size]";
				return false;
			}

			DefendAreaCfg &defend_area = m_defend_area_list[defend_area_size++];

			if (!PugiGetSubNodeValue(DataElement, "user_realive_pos_x", defend_area.user_realive_pos.x) || defend_area.user_realive_pos.x <= 0)
			{
				*err = path + ": xml node error in [config->defend_area->data->user_realive_pos_x]";
				return false;
			}
			if (!PugiGetSubNodeValue(DataElement, "user_realive_pos_y", defend_area.user_realive_pos.y) || defend_area.user_realive_pos.y <= 0)
			{
				*err = path + ": xml node error in [config->defend_area->data->user_realive_pos_y]";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "area_gather_pos_x", defend_area.gather_pos.x) || defend_area.gather_pos.x <= 0)
			{
				*err = path + ": xml node error in [config->defend_area->data->area_gather_pos_x]";
				return false;
			}
			if (!PugiGetSubNodeValue(DataElement, "area_gather_pos_y", defend_area.gather_pos.y) || defend_area.gather_pos.y <= 0)
			{
				*err = path + ": xml node error in [config->defend_area->data->area_gather_pos_y]";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "area_gather_id", defend_area.gather_id) || defend_area.gather_id <= 0)
			{
				*err = path + ": xml node error in [config->defend_area->data->area_gather_id]";
				return false;
			}

			DataElement = DataElement.next_sibling();
		}

		if (XIANMENGZHAN_DEFEND_AREA_NUM != defend_area_size) 
		{
			*err = path + ": xml node error in [config->defend_area->defend_area_size]";
			return false;
		}
	}

	{   // 读取 奖励信息

		PugiXmlNode RewardElement = RootElement.child("reward");
		if (RewardElement.empty())
		{
			*err = path + ": xml node error in [config->reward]";
			return false;
		}

		PugiXmlNode DataElement = RewardElement.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml node error in [config->reward->data]";
			return false;
		}

		int last_need_rank = 0;
		int last_role_level[XIANMENGZHAN_REWARD_GUILD_RANK_MAX];
		memset(last_role_level, 0, sizeof(last_role_level));

		while (!DataElement.empty())
		{
			int need_rank = 0;
			if (!PugiGetSubNodeValue(DataElement, "need_rank", need_rank) || need_rank <= 0 || need_rank > XIANMENGZHAN_REWARD_GUILD_RANK_MAX || need_rank < last_need_rank)
			{
				*err = path + ": xml node error in [config->reward->need_rank]";
				return false;
			}
			last_need_rank = need_rank;

			int find_cfg_index = -1;
			for (int i = 0; i < m_reward_rank_gift_count; ++ i)
			{
				if (m_reward_rank_gift_list[i].need_rank == need_rank)
				{
					find_cfg_index = i;
					break;
				}
			}
			if (find_cfg_index < 0)
			{
				if (m_reward_rank_gift_count >= XIANMENGZHAN_REWARD_GUILD_RANK_MAX)
				{
					*err = path + ": xml node error in [config->reward->need_rank] too many config 1";
					return false;
				}

				find_cfg_index = m_reward_rank_gift_count;
				++ m_reward_rank_gift_count;
			}

			if (find_cfg_index < 0 || find_cfg_index >= XIANMENGZHAN_REWARD_GUILD_RANK_MAX)
			{
				*err = path + ": xml node error in [config->reward->need_rank] too many config 2";
				return false;
			}

			RankRewardCfg &rank_reward_cfg = m_reward_rank_gift_list[find_cfg_index];
			rank_reward_cfg.need_rank = need_rank;

			if (rank_reward_cfg.reward_detail_count >= RankRewardCfg::REWARD_DETAIL_MAX_COUNT)
			{
				*err = path + ": xml node error in [config->reward->need_rank] too many config 3";
				return false;
			}

			RankRewardCfg::RewardDetail &reward_detail = rank_reward_cfg.reward_detail_list[rank_reward_cfg.reward_detail_count];

			if (!PugiGetSubNodeValue(DataElement, "need_level", reward_detail.need_level) || reward_detail.need_level <= last_role_level[find_cfg_index])
			{
				*err = path + ": xml node error in [config->reward->need_level]";
				return false;
			}
			last_role_level[find_cfg_index] = reward_detail.need_level;

			if (!PugiGetSubNodeValue(DataElement, "exp", reward_detail.exp) || reward_detail.exp <= 0)
			{
				*err = path + ": xml node error in [config->reward->exp]";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "xianhun", reward_detail.xianhun) || reward_detail.xianhun <= 0)
			{
				*err = path + ": xml node error in [config->reward->xianhun]";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "gongxian", reward_detail.gongxian) || reward_detail.gongxian <= 0)
			{
				*err = path + ": xml node error in [config->reward->gongxian]";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "honor", reward_detail.honor) || reward_detail.honor <= 0)
			{
				*err = path + ": xml node error in [config->reward->honor]";
				return false;
			}

			++ rank_reward_cfg.reward_detail_count;
			DataElement = DataElement.next_sibling();
		}

		if (1 != m_reward_rank_gift_list[0].need_rank)
		{
			*err = path + ": xml node error in [config->reward->need_rank] first need_rank_must be 1";
			return false;
		}
	}

	{
		// 读取仙盟排名礼包奖励

		PugiXmlNode GuildRankGiftElement = RootElement.child("guild_rank_gift");
		if (GuildRankGiftElement.empty())
		{
			*err = path + ": xml node error in [config->guild_rank_gift]";
			return false;
		}

		PugiXmlNode DataElement = GuildRankGiftElement.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml node error in [config->guild_rank_gift->data]";
			return false;
		}

		m_reward_guild_max_rank = 0;

		while (!DataElement.empty())
		{
			int need_rank = 0;
			if (!PugiGetSubNodeValue(DataElement, "need_rank", need_rank) || need_rank < 0)
			{
				*err = path + ": xml node error in [config->guild_rank_gift->need_rank]";
				return false;
			}

			if (need_rank <= 0 || need_rank > XIANMENGZHAN_REWARD_GUILD_RANK_GIFT_MAX || need_rank != m_reward_guild_max_rank + 1)
			{
				*err = path + ": xml node error in [config->guild_rank_gift->not in order or too many]";
				return false;
			}
			m_reward_guild_max_rank = need_rank;

			ItemConfigData &reward_item = m_reward_guild_rank_list[need_rank];
			
			PugiXmlNode RewardItemElement = DataElement.child("reward_item");
			if (RewardItemElement.empty())
			{
				*err = path + ": xml node error in [config->guild_rank_gift->reward_item error]";
				return false;
			}

			if (!reward_item.ReadConfig(RewardItemElement))
			{
				*err = path + ": xml node error in [config->guild_rank_gift->reward_item read error]";
				return false;
			}			

			DataElement = DataElement.next_sibling();
		}
	}

	return true;
}

int XianMengZhanConfig::GetEnterInfo(Posi *pos, int enter_area)
{
	if (NULL != pos)  
	{
		int real_enter_area = enter_area;
		int enter_area_count = static_cast<int>(sizeof(m_realive_area_list) / sizeof(m_realive_area_list[0]));
		
		if (enter_area_count <= 0)
		{
			return 0;
		}

		if (enter_area < 0 || enter_area >= enter_area_count)
		{
			real_enter_area = RandomNum(enter_area_count);
		}

		*pos = m_realive_area_list[real_enter_area];
	}

	return m_scene_id;
}

bool XianMengZhanConfig::GetRealivePos(int defend_area, int enter_area, Posi *pos)
{
	if (NULL == pos) return false;

	if (defend_area >= 0 && defend_area < XIANMENGZHAN_DEFEND_AREA_NUM) 
	{
		*pos = m_defend_area_list[defend_area].user_realive_pos;
	} 
	else 
	{
		this->GetEnterInfo(pos, enter_area);
	}

	return true;
}

bool XianMengZhanConfig::GetRewardKill(const Posi &dead_pos, int *reward_killer_score, int *reward_dead_score)
{
	if (NULL == reward_killer_score || NULL == reward_dead_score) return false;

	Posi delta_pos = dead_pos - m_center_area_pos;

	if ((delta_pos.x * delta_pos.x + delta_pos.y * delta_pos.y) <= (m_center_area_range * m_center_area_range))
	{
		*reward_dead_score = m_reward_center_area_dead;
		*reward_killer_score = m_reward_center_area_kill;
	}
	else
	{
		*reward_dead_score = m_reward_not_center_area_dead;
		*reward_killer_score = m_reward_not_center_area_kill;
	}

	return true;
}

const XianMengZhanConfig::RankRewardCfg::RewardDetail * XianMengZhanConfig::GetRankReward(int guild_rank, int role_level)
{
	if (guild_rank <= 0 || guild_rank > XIANMENGZHAN_REWARD_GUILD_RANK_MAX || role_level <= 0 || role_level >= MAX_ROLE_LEVEL)
	{
		return NULL;
	}

	for (int i = m_reward_rank_gift_count - 1; i >= 0; -- i)
	{
		RankRewardCfg *reward_cfg = &m_reward_rank_gift_list[i];

		if (guild_rank >= reward_cfg->need_rank)
		{
			for (int detail_index = reward_cfg->reward_detail_count - 1; detail_index >= 0; -- detail_index)
			{
				if (role_level >= reward_cfg->reward_detail_list[detail_index].need_level)
				{
					return &reward_cfg->reward_detail_list[detail_index];
				}
			}
		}
	}

	return NULL;
}

bool XianMengZhanConfig::GetDefendAreaGatherInfo(int defend_area, int *gather_id, Posi *pos)
{
	if (NULL == gather_id || NULL == pos || defend_area < 0 || defend_area >= XIANMENGZHAN_DEFEND_AREA_NUM)
	{
		return false;
	}

	*gather_id = m_defend_area_list[defend_area].gather_id;
	*pos = m_defend_area_list[defend_area].gather_pos;

	return true;
}

const ItemConfigData * XianMengZhanConfig::GetRewardGuildRankGift(int guild_rank, bool *is_in_rank)
{
	if (guild_rank <= 0) 
	{
		return NULL;
	}

	if (NULL != is_in_rank) *is_in_rank = true;

	if (guild_rank >= m_reward_guild_max_rank) 	// 如果是超过最后一个奖励，都领最后一个
	{
		if (NULL != is_in_rank) *is_in_rank = false;

		guild_rank = m_reward_guild_max_rank; 
	}

	return &m_reward_guild_rank_list[guild_rank];
}

int XianMengZhanConfig::GetEnterAreaCount()
{
	int enter_area_count = static_cast<int>(sizeof(m_realive_area_list) / sizeof(m_realive_area_list[0]));
	return enter_area_count;
}


