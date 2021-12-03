#include "yizhandaodiconfig.hpp"

#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"
#include "scene/worldshadow/worldshadow.hpp"

YiZhanDaoDiConfig::YiZhanDaoDiConfig()
	: m_open_level(0), m_sceneid(0), m_realive_pos_count(0), m_dead_max_count(0), m_lucky_interval(0), 
	m_reward_count(0), m_jisha_title_count(0), m_jisha_score_reward_count(0), m_guwu_gongji_gold(0), m_guwu_gongji_max_per(0), m_guwu_gongji_percent(0), 
	m_guwu_maxhp_gold(0), m_guwu_maxhp_max_per(0), m_guwu_maxhp_percent(0), m_kill_honor(0), m_gather_id(0), m_gather_num(0), m_first_refresh_interval(0),
	m_refresh_interval(0), m_gather_time(0), m_gather_exit_time(0), m_gather_buff_total_pro(0), m_broact_role_num_interval(0), m_broact_title_id(0),
	m_reward_kill_list_count(0)
{
	memset(m_realive_poslist, 0, sizeof(m_realive_poslist));
	memset(m_rank_user_reward_title_id_list, 0, sizeof(m_rank_user_reward_title_id_list));
}

YiZhanDaoDiConfig::~YiZhanDaoDiConfig()
{

}

bool YiZhanDaoDiConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = { 0 };
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "YiZhanDaoDiConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode OtherConfigElement = RootElement.child("other");
		if (OtherConfigElement.empty())
		{
			*err = path + ": xml node error in [config->other]";
			return false;
		}

		PugiXmlNode DataElement = OtherConfigElement.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml node error in [config->other->data]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "open_level", m_open_level) || m_open_level <= 0)
		{
			*err = path + ": xml node error in [config->open_level]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "scene_id", m_sceneid) || m_sceneid <= 0)
		{
			*err = path + ": xml node error in [config->scene_id]";
			return false;
		}

		CheckResourceCenter::Instance().GetSceneCheck()->Add(m_sceneid);

		if (!PugiGetSubNodeValue(DataElement, "dead_max_count", m_dead_max_count) || m_dead_max_count <= 0)
		{
			*err = path + ": xml node error in [config->dead_max_count]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "lucky_interval", m_lucky_interval) || m_lucky_interval <= 0)
		{
			*err = path + ": xml node error in [config->lucky_interval]";
			return false;
		}

		PugiXmlNode LuckyItemElement = DataElement.child("lucky_item");
		if (LuckyItemElement.empty())
		{
			*err = path + ": xml node error in [config->lucky_item]";
			return false;
		}

		if (!m_lucky_item.ReadConfig(LuckyItemElement))
		{
			*err = path + ": xml node error in [config->lucky_item]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "gongji_guwu_gold", m_guwu_gongji_gold) || m_guwu_gongji_gold <= 0)
		{
			*err = path + ": xml node error in [config->gongji_guwu_gold]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "gongji_guwu_max_per", m_guwu_gongji_max_per) || m_guwu_gongji_max_per <= 0)
		{
			*err = path + ": xml node error in [config->gongji_guwu_max_per]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "gongji_guwu_add_per", m_guwu_gongji_percent) || m_guwu_gongji_percent <= 0)
		{
			*err = path + ": xml node error in [config->gongji_guwu_add_per]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "hp_guwu_gold", m_guwu_maxhp_gold) || m_guwu_maxhp_gold <= 0)
		{
			*err = path + ": xml node error in [config->hp_guwu_gold]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "hp_guwu_max_per", m_guwu_maxhp_max_per) || m_guwu_maxhp_max_per <= 0)
		{
			*err = path + ": xml node error in [config->hp_guwu_max_per]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "hp_guwu_add_per", m_guwu_maxhp_percent) || m_guwu_maxhp_percent <= 0)
		{
			*err = path + ": xml node error in [config->hp_guwu_add_per]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "kill_honor", m_kill_honor) || m_kill_honor <= 0)
		{
			*err = path + ": xml node error in [config->kill_honor]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "gather_id", m_gather_id) || m_gather_id <= 0)
		{
			*err = path + ": xml node error in [config->gather_id]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "gather_num", m_gather_num) || m_gather_num < 0)
		{
			*err = path + ": xml node error in [config->gather_num]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "first_refresh_interval", m_first_refresh_interval) || m_first_refresh_interval <= 0)
		{
			*err = path + ": xml node error in [config->first_refresh_interval]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "refresh_interval", m_refresh_interval) || m_refresh_interval <= 0)
		{
			*err = path + ": xml node error in [config->refresh_interval]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "gather_time", m_gather_time) || m_gather_time <= 0)
		{
			*err = path + ": xml node error in [config->gather_time]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "gather_exit_time", m_gather_exit_time) || m_gather_exit_time <= 0)
		{
			*err = path + ": xml node error in [config->gather_exit_time]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "broast_role_num_interval", m_broact_role_num_interval) || m_broact_role_num_interval <= 0)
		{
			*err = path + ": xml node error in [config->broast_role_num_interval]";
			return false;
		}

		if (!PugiGetSubNodeValue(DataElement, "broact_title_id", m_broact_title_id) || m_broact_title_id <= 0)
		{
			*err = path + ": xml node error in [config->broact_title_id]";
			return false;
		}
	}

	{
		PugiXmlNode realive_pos_listElement = RootElement.child("realive_pos_list");
		if (realive_pos_listElement.empty())
		{
			*err = path + ": xml node error in [config->realive_pos_list]";
			return false;
		}

		PugiXmlNode DataElement = realive_pos_listElement.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml node error in [config->realive_pos_list->data]";
			return false;
		}

		int count = 0;
		while (!DataElement.empty())
		{
			if (count >= YIZHANDAODI_REALIVE_POS_MAX)
			{
				*err = path + ": xml node error in [config->realive_pos_list->data] more than limit";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "realive_pos_x", m_realive_poslist[count].x) || m_realive_poslist[count].x <= 0)
			{
				*err = path + ": xml node error in [config->realive_pos_list->data->realive_pos_x]";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "realive_pos_y", m_realive_poslist[count].y) || m_realive_poslist[count].y <= 0)
			{
				*err = path + ": xml node error in [config->realive_pos_list->data->realive_pos_y]";
				return false;
			}

			++ count;
			DataElement = DataElement.next_sibling();
		}

		m_realive_pos_count = count;
	}

	{
		PugiXmlNode kill_rank_rewardtElement = RootElement.child("kill_rank_reward");
		if (kill_rank_rewardtElement.empty())
		{
			*err = path + ": xml node error in [config->kill_rank_reward]";
			return false;
		}

		PugiXmlNode DataElement = kill_rank_rewardtElement.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml node error in [config->kill_rank_reward->data]";
			return false;
		}

		while (!DataElement.empty())
		{
			int rank = 0;

			if (!PugiGetSubNodeValue(DataElement, "rank", rank) || rank < 0 || rank > YIZHANDAODI_RANK_NUM)
			{
				*err = path + ": xml node error in [config->kill_rank_reward->data->rank]";
				return false;
			}
			
			/*PugiXmlNode RankItemElement = DataElement.child("reward_item");
			if (RankItemElement.empty())
			{
				*err = path + ": xml node error in [config->reward_item]";
				return false;
			}*/

			int item_count = ItemConfigData::ReadConfigList(DataElement, "reward_item", m_jisha_reward_list[rank].reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
			//if (!m_jisha_reward_list[rank].reward_item.ReadConfig(RankItemElement))
			if (item_count < 0)
			{
				*err = path + ": xml node error in [config->reward_item] " + std::to_string(item_count);
				return false;
			}
			m_jisha_reward_list[rank].reward_item_count = item_count;
			DataElement = DataElement.next_sibling();
		}
	}

	{
		PugiXmlNode kill_title_listElement = RootElement.child("kill_title");
		if (kill_title_listElement.empty())
		{
			*err = path + ": xml node error in [config->kill_title]";
			return false;
		}

		PugiXmlNode DataElement = kill_title_listElement.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml node error in [config->kill_title->data]";
			return false;
		}

		int count = 0;
		while (!DataElement.empty())
		{
			if (count >= YIZHANDAODI_JISHA_TITLE_MAX)
			{
				*err = path + ": xml node error in [config->kill_title->data] more than limit";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "kill_count", m_jisha_title_list[count].jisha_count) || m_jisha_title_list[count].jisha_count <= 0)
			{
				*err = path + ": xml node error in [config->kill_title->data->kill_count]";
				return false;
			}

			if (count > 0 && m_jisha_title_list[count].jisha_count <= m_jisha_title_list[count - 1].jisha_count)
			{
				*err = path + ": xml node error in [config->kill_title->data->kill_count]";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "title_id", m_jisha_title_list[count].title_id) || m_jisha_title_list[count].title_id <= 0)
			{
				*err = path + ": xml node error in [config->kill_title->data->title_id]";
				return false;
			}

			++ count;
			DataElement = DataElement.next_sibling();
		}

		m_jisha_title_count = count;
	}
	
	// 击杀积分兑换荣誉配置
	{
		PugiXmlNode kill_score_reward_listElement = RootElement.child("kill_score_reward");
		if (kill_score_reward_listElement.empty())
		{
			*err = path + ": xml node error in [config->kill_score_reward]";
			return false;
		}

		PugiXmlNode DataElement = kill_score_reward_listElement.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml node error in [config->kill_score_reward->data]";
			return false;
		}

		int count = 0;
		while (!DataElement.empty())
		{
			if (count >= YIZHANDAODI_JISHA_SCORE_REWARD_MAX)
			{
				*err = path + ": xml node error in [config->kill_score_reward->data] more than limit";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "kill_score_min", m_jisha_score_reward_list[count].jisha_score_min) || m_jisha_score_reward_list[count].jisha_score_min <= 0)
			{
				*err = path + ": xml node error in [config->kill_score_reward->data->kill_score_min]";
				return false;
			}

			if (count > 0 && m_jisha_score_reward_list[count].jisha_score_min <= m_jisha_score_reward_list[count - 1].jisha_score_min)
			{
				*err = path + ": xml node error in [config->kill_score_reward->data->kill_score_min]";
				return false;
			}
			
			if (!PugiGetSubNodeValue(DataElement, "kill_score_max", m_jisha_score_reward_list[count].jisha_score_max) || m_jisha_score_reward_list[count].jisha_score_max <= 0)
			{
				*err = path + ": xml node error in [config->kill_score_reward->data->kill_score_max]";
				return false;
			}

			if (count > 0 && m_jisha_score_reward_list[count].jisha_score_max <= m_jisha_score_reward_list[count - 1].jisha_score_max)
			{
				*err = path + ": xml node error in [config->kill_score_reward->data->kill_score_max]";
				return false;
			}
			
			if (m_jisha_score_reward_list[count].jisha_score_min >= m_jisha_score_reward_list[count].jisha_score_max)
			{
				*err = path + ": xml node error in [config->kill_score_reward->data->kill_score_max]";
				return false;
			}
			
			if (!PugiGetSubNodeValue(DataElement, "reward_shengwang", m_jisha_score_reward_list[count].jisha_shengwang) || m_jisha_score_reward_list[count].jisha_shengwang <= 0)
			{
				*err = path + ": xml node error in [config->kill_score_reward->data->reward_honour]";
				return false;
			}

			if (count > 0 && m_jisha_score_reward_list[count].jisha_shengwang <= m_jisha_score_reward_list[count - 1].jisha_shengwang)
			{
				*err = path + ": xml node error in [config->kill_score_reward->data->reward_honour]";
				return false;
			}

			++ count;
			DataElement = DataElement.next_sibling();
		}

		m_jisha_score_reward_count = count;
	}

	{
		// 被杀配置
		PugiXmlNode be_kill_reward_listElement = RootElement.child("be_kill_reward");
		if (be_kill_reward_listElement.empty())
		{
			*err = path + ": xml node error in [config->be_kill_reward]";
			return false;
		}

		PugiXmlNode DataElement = be_kill_reward_listElement.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml node error in [config->be_kill_reward->data]";
			return false;
		}

		int last_add_maxhp_percent = 0;

		while (!DataElement.empty())
		{
			int dead_times = 0;
			if (!PugiGetSubNodeValue(DataElement, "be_kill_times", dead_times) || dead_times != m_dead_cfg.max_dead_times + 1)
			{
				*err = path + ": xml node error in [config->be_kill_reward->data->be_kill_times]";
				return false;
			}
			m_dead_cfg.max_dead_times = dead_times;

			if (dead_times >= YiZhanDaoDiDeadConfig::MAX_DEAD_TIMES)
			{
				*err = path + ": xml node error in [config->be_kill_reward->data too many]";
				return false;
			}

			YiZhanDaoDiDeadConfig::ConfigItem &cfg_item = m_dead_cfg.cfg_list[dead_times];
			cfg_item.dead_times = dead_times;

			if (!PugiGetSubNodeValue(DataElement, "add_maxhp_percent", cfg_item.add_maxhp_percent) || cfg_item.add_maxhp_percent < last_add_maxhp_percent)
			{
				*err = path + ": xml node error in [config->be_kill_reward->data->add_maxhp_percent]";
				return false;
			}
			last_add_maxhp_percent = cfg_item.add_maxhp_percent;

			if (!PugiGetSubNodeValue(DataElement, "has_item_reward", cfg_item.has_item_reward) || cfg_item.has_item_reward < 0)
			{
				*err = path + ": xml node error in [config->be_kill_reward->data->has_item_reward]";
				return false;
			}

			if (0 != cfg_item.has_item_reward)
			{
				PugiXmlNode reward_item_element = DataElement.child("reward_item");
				if (reward_item_element.empty())
				{
					*err = path + ": xml node error in [config->be_kill_reward->data->reward_item not find]";
					return false;
				}

				if (!cfg_item.reward_item.ReadConfig(reward_item_element))
				{
					*err = path + ": xml node error in [config->be_kill_reward->data->reward_item read error]";
					return false;
				}
			}
			
			DataElement = DataElement.next_sibling();
		}

		if (m_dead_cfg.max_dead_times < m_dead_max_count)
		{
			*err = path + ": xml node error in [config->be_kill_reward m_dead_cfg.max_dead_times must not less than m_dead_max_count]";
			return false;
		}
	}

	{
		PugiXmlNode gather_list_listElement = RootElement.child("gather_pos");
		if (gather_list_listElement.empty())
		{
			*err = path + ": xml node error in [config->gather_pos]";
			return false;
		}

		PugiXmlNode DataElement = gather_list_listElement.child("data");
		while (!DataElement.empty())
		{
			Posi positem;

			if (!PugiGetSubNodeValue(DataElement, "pos_x", positem.x))
			{
				*err = path + ": xml node error in [config->gather_pos->pos_x]";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "pos_y", positem.y))
			{
				*err = path + ": xml node error in [config->gather_pos->pos_y]";
				return false;
			}

			m_gather_pos_list.push_back(positem);

			DataElement = DataElement.next_sibling();
		}

		if (m_gather_num > static_cast<int>(m_gather_pos_list.size()))
		{
			*err = path + ": m_gather_pos_list.size() less than m_gather_num";
			return false;
		}
	}

	{
		PugiXmlNode gather_buff_listElement = RootElement.child("gather_buff");
		if (gather_buff_listElement.empty())
		{
			*err = path + ": xml node error in [config->gather_buff]";
			return false;
		}

		PugiXmlNode DataElement = gather_buff_listElement.child("data");
		while (!DataElement.empty())
		{
			YiZhanDaoDiGatherBuffCfg buff_cfg;

			if (!PugiGetSubNodeValue(DataElement, "buff_seq", buff_cfg.buff_seq) || buff_cfg.buff_seq <= 0 || m_gather_buff_map.find(buff_cfg.buff_seq) != m_gather_buff_map.end())
			{
				*err = path + ": xml node error in [config->gather_buff->buff_seq]";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "buff_pro", buff_cfg.buff_pro) || buff_cfg.buff_pro < 0)
			{
				*err = path + ": xml node error in [config->gather_buff->buff_pro]";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "buff_type", buff_cfg.buff_type) || buff_cfg.buff_seq < YIZHAN_BUFF_ADD_SPEED || buff_cfg.buff_type >= YIZHAN_BUFF_MAX_TYPE)
			{
				*err = path + ": xml node error in [config->gather_buff->buff_type]";
				return false;
			}

			for(int i = 0; i < 4; i++)
			{
				char temp_str[32] = {0};
				sprintf(temp_str, "param_%d", i + 1);

				if (!PugiGetSubNodeValue(DataElement, temp_str, buff_cfg.param[i]) || buff_cfg.param[i] < 0)
				{
					*err = path + ": xml node error in [config->gather_buff->temp_str]";
					return false;
				}
			}

			if (!PugiGetSubNodeValue(DataElement, "is_notice", buff_cfg.is_notice) || buff_cfg.is_notice < 0)
			{
				*err = path + ": xml node error in [config->gather_buff->is_notice]";
				return false;
			}

			m_gather_buff_total_pro += buff_cfg.buff_pro;
			m_gather_buff_map[buff_cfg.buff_seq] = buff_cfg;
			
			DataElement = DataElement.next_sibling();
		}

		if (m_gather_buff_total_pro <= 0)
		{
			*err = path + ": m_gather_buff_total_pro must more than 0";
			return false;
		}
	}

	{
		PugiXmlNode Element = RootElement.child("rank_title");
		if (Element.empty())
		{
			*err = path + ": xml node error in [config->rank_title]";
			return false;
		}

		PugiXmlNode DataElement = Element.child("data");
		while (!DataElement.empty())
		{
			YiZhanDaoDiGatherBuffCfg buff_cfg;

			int rank = -1;
			if (!PugiGetSubNodeValue(DataElement, "rank", rank) || rank < 0 || rank >= YIZHANDAODI_REWARD_TITLE_RANK_COUNT)
			{
				*err = path + ": xml node error in [config->rank_title->rank]";
				return false;
			}

			if (!PugiGetSubNodeValue(DataElement, "title_id", m_rank_user_reward_title_id_list[rank]) || m_rank_user_reward_title_id_list[rank] <= 0)
			{
				*err = path + ": xml node error in [config->rank_title->title_id]";
				return false;
			}

			DataElement = DataElement.next_sibling();
		}
	}

	{
		PugiXmlNode NumRewardElement = RootElement.child("kill_num_reward");
		if (NumRewardElement.empty())
		{
			*err = path + ": xml node error in [config->kill_num_reward]";
			return false;
		}

		iRet = this->InitKillNumReward(NumRewardElement);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitKillNumReward failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int YiZhanDaoDiConfig::InitKillNumReward(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}
	m_reward_kill_list_count = 0;
	int last_kill_num = 0;
	while (!dataElement.empty())
	{
		int kill_count = 0;
		if (!PugiGetSubNodeValue(dataElement, "kill_count", kill_count) || kill_count <= 0)
		{
			return -1;
		}
		if (kill_count <= last_kill_num)
		{
			return -2;
		}
		last_kill_num = kill_count;
		m_reward_kill_num[m_reward_kill_list_count].kill_count = kill_count;
		m_reward_kill_num[m_reward_kill_list_count].reward_count = ItemConfigData::ReadConfigList(dataElement, "reward_item", m_reward_kill_num[m_reward_kill_list_count].reward_item, MAX_ATTACHMENT_ITEM_NUM);
		if (m_reward_kill_num[m_reward_kill_list_count].reward_count < 0)
		{ 
			return -4 + m_reward_kill_num[m_reward_kill_list_count].reward_count;
		}

		m_reward_kill_list_count++;
		if (m_reward_kill_list_count >YIZHANDAODI_JISHA_REWARD_NUM)
		{
			return -5;
		}
		dataElement = dataElement.next_sibling();
	}
	return 0;
}
int YiZhanDaoDiConfig::GetEnterPos(Posi *enter_pos)
{
	if (this->RandomRealivePos(enter_pos))
	{
		return m_sceneid;
	}

	return 0;
}

bool YiZhanDaoDiConfig::RandomRealivePos(Posi *get_pos)
{
	if (NULL == get_pos || m_realive_pos_count <= 0 || m_realive_pos_count > YIZHANDAODI_REALIVE_POS_MAX) return false;

	int rand_idx = RandomNum(m_realive_pos_count);
	*get_pos = m_realive_poslist[rand_idx];

	return true;
}

int YiZhanDaoDiConfig::GetJiShaTitle(int jisha_count)
{
	for (int i = m_jisha_title_count - 1; i >= 0; i--)
	{
		if (jisha_count >= m_jisha_title_list[i].jisha_count)
		{
			return m_jisha_title_list[i].title_id;
		}
	}

	return 0;
}

const ItemConfigData * YiZhanDaoDiConfig::GetJishaRankRewardItem(int rank_idx)
{
	if (rank_idx < 0 || rank_idx > YIZHANDAODI_RANK_NUM) return NULL;

	return m_jisha_reward_list[rank_idx].reward_item_list;
}

const int YiZhanDaoDiConfig::GetJishaRankRewardItemCount(int rank_idx)
{
	if (rank_idx < 0 || rank_idx > YIZHANDAODI_RANK_NUM) return 0;

	return m_jisha_reward_list[rank_idx].reward_item_count;
}

void YiZhanDaoDiConfig::GetGongjiGuWuInfo(int *need_gold, int *max_gpngji_per, int *add_gongji_per)
{
	if (NULL != need_gold) *need_gold = m_guwu_gongji_gold;
	if (NULL != max_gpngji_per) *max_gpngji_per = m_guwu_gongji_max_per;
	if (NULL != add_gongji_per) *add_gongji_per = m_guwu_gongji_percent;
}	

void YiZhanDaoDiConfig::GetMaxHPGuWuInfo(int *need_gold, int *max_hp_per, int *add_maxhp_per)
{
	if (NULL != need_gold) *need_gold = m_guwu_maxhp_gold;
	if (NULL != max_hp_per) *max_hp_per = m_guwu_maxhp_max_per;
	if (NULL != add_maxhp_per) *add_maxhp_per = m_guwu_maxhp_percent;
}

int YiZhanDaoDiConfig::GetJishaShengwangReward(int jiasha_score)
{
	for (int i = 0; i < m_jisha_score_reward_count; ++i)
	{
		if (jiasha_score >= m_jisha_score_reward_list[i].jisha_score_min && jiasha_score <= m_jisha_score_reward_list[i].jisha_score_max)
		{
			return m_jisha_score_reward_list[i].jisha_shengwang;
		}
	}
	
	return 0;
}

const YiZhanDaoDiDeadConfig::ConfigItem * YiZhanDaoDiConfig::GetDeadCfg(int dead_times)
{
	if (dead_times <= 0 || dead_times > m_dead_cfg.max_dead_times || dead_times >= YiZhanDaoDiDeadConfig::MAX_DEAD_TIMES)
	{
		return NULL;
	}

	return &m_dead_cfg.cfg_list[dead_times];
}

int YiZhanDaoDiConfig::GetRankTitle(int rank)
{
	if (rank < 0 || rank >= YIZHANDAODI_REWARD_TITLE_RANK_COUNT)
	{
		return 0;
	}

	return m_rank_user_reward_title_id_list[rank];
}

const YiZhanDaoDiConfig::KillNumReward* YiZhanDaoDiConfig::GetKillNumReward(int kill_count, int & index)
{
	if(kill_count<=0)
	{
		index = -1;
		return NULL;
	}
	//杀人数够领最高奖
	if (kill_count >= m_reward_kill_num[m_reward_kill_list_count - 1].kill_count)
	{
		index = m_reward_kill_list_count - 1;
		return &m_reward_kill_num[m_reward_kill_list_count - 1];
	}
	for (int i = 0; i < m_reward_kill_list_count; ++i)
	{
		if (kill_count < m_reward_kill_num[i].kill_count)
		{
			if (i > 0)
			{
				index = i - 1;
				return &m_reward_kill_num[i - 1];
			}
			else
			{
				index = -1;
				return NULL;  //杀人数不够领奖
			}
		}
	}
	return NULL;
}