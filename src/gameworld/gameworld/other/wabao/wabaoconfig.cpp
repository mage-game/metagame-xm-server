#include "wabaoconfig.hpp"
#include "servercommon/configcommon.h"
#include "globalconfig/globalconfig.h"
#include "monster/monsterpool.h"

#include "checkresourcecenter.hpp"

WabaoConfig::WabaoConfig()
{

}

WabaoConfig::~WabaoConfig()
{

}

bool WabaoConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "WabaoConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// ÔÓÏîÅäÖÃ
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOtherCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ±¦²Ø½±ÀøÅäÖÃ
		PugiXmlNode root_element = RootElement.child("baozang_reward");
		if (root_element.empty())
		{
			*err = path + " xml not baozang_reward node ";
			return false;
		}

		iRet = this->InitBaozangRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitBaozangRewardCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ±¦Í¼ÅäÖÃ
		PugiXmlNode root_element = RootElement.child("baotu");
		if (root_element.empty())
		{
			*err = path + " xml not baotu node ";
			return false;
		}

		iRet = this->InitBaotuCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitBaotuCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// Ë¢ÐÂµãÅäÖÃ
		PugiXmlNode root_element = RootElement.child("baozang_pos");
		if (root_element.empty())
		{
			*err = path + " xml not baozang_pos node ";
			return false;
		}

		iRet = this->InitRefreshPosCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitRefreshPosCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// ±¦Ïä³ØÅäÖÃ
		PugiXmlNode root_element = RootElement.child("baozang_pool");
		if (root_element.empty())
		{
			*err = path + " xml not baozang_pool node ";
			return false;
		}

		iRet = this->InitBaozangRewardPoolCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitBaozangPoolCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

bool WabaoConfig::GetRefreshPos(int *scene_id, int *pos_x, int *pos_y)
{
	if (NULL == scene_id || NULL == pos_x || NULL == pos_y)
	{
		return false;
	}

	int pos_index_count = 0;
	int pos_index_list[WabaoRefreshPosConfig::CONFIG_ITEM_MAX_COUNT]; memset(pos_index_list, 0, sizeof(pos_index_list));

	for (int i = 0; i < m_refresh_pos_cfg.cfg_count && pos_index_count < WabaoRefreshPosConfig::CONFIG_ITEM_MAX_COUNT; ++ i)
	{
		pos_index_list[pos_index_count] = i;
		++ pos_index_count;
	}

	if (pos_index_count > 0)
	{
		int rand_value = RandomNum(pos_index_count);
		int cfg_index = pos_index_list[rand_value];

		if (cfg_index >= 0 && cfg_index < WabaoRefreshPosConfig::CONFIG_ITEM_MAX_COUNT)
		{
			const WabaoRefreshPosConfig::ConfigItem &cfg_item = m_refresh_pos_cfg.cfg_list[cfg_index];

			*scene_id = cfg_item.scene_id;
			*pos_x = cfg_item.pos.x;
			*pos_y = cfg_item.pos.y;

			return true;
		}
	}

	return false;
}

int WabaoConfig::InitRefreshPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_refresh_pos_cfg.cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_refresh_pos_cfg.cfg_count >= WabaoRefreshPosConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}

		WabaoRefreshPosConfig::ConfigItem &cfg_item = m_refresh_pos_cfg.cfg_list[m_refresh_pos_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "scene_id", cfg_item.scene_id) || cfg_item.scene_id <= 0)
		{
			return -33;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg_item.scene_id);

		if (!PugiGetSubNodeValue(data_element, "pos_x", cfg_item.pos.x) || cfg_item.pos.x <= 0)
		{
			return -3;
		}
		if (!PugiGetSubNodeValue(data_element, "pos_y", cfg_item.pos.y) || cfg_item.pos.y <= 0)
		{
			return -4;
		}

		++ m_refresh_pos_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int WabaoConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "quick_complete_cost", m_other_cfg.quick_complete_cost) || m_other_cfg.quick_complete_cost <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "succ_prob", m_other_cfg.succ_prob) || m_other_cfg.succ_prob <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "max_reward_limit", m_other_cfg.max_reward_limit) || m_other_cfg.max_reward_limit <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "max_baotu_limit", m_other_cfg.max_baotu_limit) || m_other_cfg.max_baotu_limit <= 0)
		{
			return -4;
		}
		if (!PugiGetSubNodeValue(data_element, "shouhuzhe_time", m_other_cfg.shouhuzhe_time) || m_other_cfg.shouhuzhe_time <= 0)
		{
			return -5;
		}
		if (!PugiGetSubNodeValue(data_element, "open_level", m_other_cfg.open_level) || m_other_cfg.open_level <= 0 || m_other_cfg.open_level > MAX_ROLE_LEVEL)
		{
			return -6;
		}
		if (!PugiGetSubNodeValue(data_element, "init_baotu_times", m_other_cfg.init_baotu_times) || m_other_cfg.init_baotu_times < 0 || m_other_cfg.init_baotu_times > m_other_cfg.max_baotu_limit)
		{
			return -7;
		}
	}

	return 0;
}

int WabaoConfig::InitBaozangRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_baozang_reward_cfg.cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_baozang_reward_cfg.cfg_count >= WabaoBaozangRewardConfig::BAOZANG_TYPE_MAX)
		{
			return -1;
		}

		int reward_type = 0;
		if (!PugiGetSubNodeValue(data_element, "reward_type", reward_type) || reward_type < WabaoBaozangRewardConfig::BAOZANG_TYPE_GOLD || reward_type >= WabaoBaozangRewardConfig::BAOZANG_TYPE_OTHER)
		{
			return -2;
		}

		WabaoBaozangRewardConfig::ConfigItem &cfg_item = m_baozang_reward_cfg.cfg_list[reward_type];
		cfg_item.reward_type = reward_type;

		if (!PugiGetSubNodeValue(data_element, "reward_limit", cfg_item.reward_limit) || cfg_item.reward_limit < 0)
		{
			return -3;
		}

		++ m_baozang_reward_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int WabaoConfig::InitBaotuCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_baotu_cfg.cfg_count = 0;
	int last_total_degree = 0;

	while (!data_element.empty())
	{
		if (m_baotu_cfg.cfg_count >= WabaoBaotuConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}

		WabaoBaotuConfig::ConfigItem &cfg_item = m_baotu_cfg.cfg_list[m_baotu_cfg.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "total_degree", cfg_item.total_degree) || cfg_item.total_degree <= last_total_degree)
		{
			return -2;
		}

		last_total_degree = cfg_item.total_degree;

		if (!PugiGetSubNodeValue(data_element, "baotu_count", cfg_item.baotu_count))
		{
			return -3;
		}

		++ m_baotu_cfg.cfg_count;
		data_element = data_element.next_sibling();
	}
	return 0;
}

int WabaoConfig::InitBaozangRewardPoolCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	
	while (!data_element.empty())
	{
		int index = 0;
		int reward_type = 0;
		if (!PugiGetSubNodeValue(data_element, "index", index) || index < 0)
		{
			return -1;
		}
		
		if (!PugiGetSubNodeValue(data_element, "reward_type", reward_type) || reward_type < WabaoBaozangRewardConfig::BAOZANG_TYPE_GOLD || reward_type >= WabaoBaozangRewardConfig::BAOZANG_TYPE_OTHER)
		{
			return -2;
		}

		WabaoBaozangRewardConfig::ConfigItem &item_cfg = m_baozang_reward_cfg.cfg_list[reward_type];
		WabaoBaozangRewardItemConfig item;
		item.reward_type = reward_type;
		item.index = index;
		{
			PugiXmlNode element = data_element.child("reward_item_list");
			if (element.empty())
			{
				return -3;
			}

			int i = 0;
			PugiXmlNode item_element = element.child("reward_item");
			while (!item_element.empty())
			{
				if (i >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return -4;
				}

				if (!item.reward_item[i].ReadConfig(item_element))
				{
					return 100 - i;
				}

				i++;
				item_element = item_element.next_sibling();
			}
		}

		if (!PugiGetSubNodeValue(data_element, "weight", item.weight) || item.weight < 0)
		{
			return -5;
		}
		item_cfg.total_weight += item.weight;
		item_cfg.cfg_vec.push_back(item);
		data_element = data_element.next_sibling();
	}

	return 0;
}
