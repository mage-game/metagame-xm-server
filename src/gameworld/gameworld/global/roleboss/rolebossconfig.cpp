#include "rolebossconfig.h"
#include "checkresourcecenter.hpp"
#include "monster/drop/droppool.hpp"
#include "engineadapter.h"

RoleBossConfig::RoleBossConfig() : m_refresh_time_count(0), m_exp_factor_count(0)
{
	memset(m_refresh_time_list, 0, sizeof(m_refresh_time_list));
	memset(m_refresh_cfg_list, 0, sizeof(m_refresh_cfg_list));

	{
		m_exp_factor_list[m_exp_factor_count ++] = ExpFactor(0, 1.0);
		m_exp_factor_list[m_exp_factor_count ++] = ExpFactor(1, 0.9);
		m_exp_factor_list[m_exp_factor_count ++] = ExpFactor(2, 0.8);
		m_exp_factor_list[m_exp_factor_count ++] = ExpFactor(3, 0.7);
		m_exp_factor_list[m_exp_factor_count ++] = ExpFactor(10, 0.65);
		m_exp_factor_list[m_exp_factor_count ++] = ExpFactor(20, 0.6);
	}
}

RoleBossConfig::~RoleBossConfig()
{
}

bool RoleBossConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "RoleBossConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// 刷新时间
		PugiXmlNode root_element = RootElement.child("refresh_time");
		if (root_element.empty())
		{
			*err = path + " xml not refresh_time node ";
			return false;
		}

		iRet = this->InitRefreshTimeCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitRefreshTimeCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 刷新配置
		PugiXmlNode root_element = RootElement.child("refresh_info");
		if (root_element.empty())
		{
			*err = path + " xml not refresh_info node ";
			return false;
		}

		iRet = this->InitRefreshCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitRefreshTimeCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

const RoleBossRefreshCfg * RoleBossConfig::GetRefreshCfg(int index) 
{
	if (index >= 0 && index < ROLE_BOSS_MAX_COUNT)
	{
		return &m_refresh_cfg_list[index];
	}

	return NULL;
}

time_t RoleBossConfig::CalNextRefreshTimestamp() 
{
	int index = -1;
	int min_interval = 60 * 60 * 24;

	for (int i = 0; i < m_refresh_time_count; ++i)
	{
		int temp = EngineAdapter::Instance().NextDayInterval(m_refresh_time_list[i] / 100, m_refresh_time_list[i] % 100, 0);
		if (temp < min_interval)		// 找下一个最近的时间点
		{
			min_interval = temp;
			index = i;
		}
	}

	if (index >= 0 && index < m_refresh_time_count)
	{
		time_t next_refresh_time = EngineAdapter::Instance().Time() + min_interval;
		return next_refresh_time;
	}

	return 0;
}

double RoleBossConfig::GetExpFactor(int rank)
{
	for (int i = 0; i < EXP_FACTOR_MAX_COUNT && i < m_exp_factor_count; ++ i)
	{
		if (rank <= m_exp_factor_list[i].rank)
		{
			return m_exp_factor_list[i].factor;
		}
	}

	return m_exp_factor_list[m_exp_factor_count - 1].factor;
}

int RoleBossConfig::InitRefreshTimeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_refresh_time = -1;

	while (!dataElement.empty())
	{
		if (m_refresh_time_count >= ROLE_BOSS_REFRESH_TIME_CFG_MAX_COUNT)
		{
			return -1;
		}
 
		int refresh_timestamp = 0;
		if (!PugiGetSubNodeValue(dataElement, "refresh_timestamp", refresh_timestamp) || 
			refresh_timestamp <= last_refresh_time || refresh_timestamp >= 2400)
		{
			return -2;
		}
		last_refresh_time = refresh_timestamp;

		m_refresh_time_list[m_refresh_time_count] = refresh_timestamp;

		++ m_refresh_time_count;
		dataElement = dataElement.next_sibling();
	}
	
	return 0;
}

int RoleBossConfig::InitRefreshCfg(PugiXmlNode RootElement)
{

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int cfg_count = 0;

	while (!dataElement.empty())
	{
		if (cfg_count >= ROLE_BOSS_MAX_COUNT)
		{
			return -1;
		}

		RoleBossRefreshCfg &cfg_item = m_refresh_cfg_list[cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg_item.scene_id) || cfg_item.scene_id <= 0)
		{
			return -2;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg_item.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "pos_x", cfg_item.pos.x) || cfg_item.pos.x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", cfg_item.pos.y) || cfg_item.pos.y <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "attr_level", cfg_item.attr_level) || cfg_item.attr_level < 60)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "exp_level", cfg_item.exp_level) || cfg_item.exp_level < 60)
		{
			return -6;
		}

		{
			// 最高伤害
			std::string dropid_string;
			if (!PugiGetSubNodeValue(dataElement, "top_hurt_dropid", dropid_string))
			{
				return -15;
			}

			char tmp_buff[256]; memset(tmp_buff, 0, sizeof(tmp_buff));
			strncpy(tmp_buff, dropid_string.c_str(), sizeof(tmp_buff));

			char *p = strtok(tmp_buff, "|");
			while (p)
			{
				int dorp_id = atoi(p);
				if (dorp_id != 0 && NULL == DROPPOOL->GetDropConfig(dorp_id))
				{
					return -16;
				}

				cfg_item.top_hurt_dropid_set.push_back(static_cast<UInt16>(dorp_id));

				p = strtok(NULL, "|");
			}

			if (cfg_item.top_hurt_dropid_set.empty())
			{
				return -17;
			}
		}

		{
			// 参与
			std::string dropid_string;
			if (!PugiGetSubNodeValue(dataElement, "join_dropid", dropid_string))
			{
				return -25;
			}

			char tmp_buff[256]; memset(tmp_buff, 0, sizeof(tmp_buff));
			strncpy(tmp_buff, dropid_string.c_str(), sizeof(tmp_buff));

			char *p = strtok(tmp_buff, "|");
			while (p)
			{
				int dorp_id = atoi(p);
				if (NULL == DROPPOOL->GetDropConfig(dorp_id))
				{
					return -26;
				}

				cfg_item.join_dropid_set.push_back(static_cast<UInt16>(dorp_id));

				p = strtok(NULL, "|");
			}

			if (cfg_item.join_dropid_set.empty())
			{
				return -27;
			}
		}

		{
			// 击杀
			std::string dropid_string;
			if (!PugiGetSubNodeValue(dataElement, "kill_drop_id", dropid_string))
			{
				return -35;
			}

			char tmp_buff[256]; memset(tmp_buff, 0, sizeof(tmp_buff));
			strncpy(tmp_buff, dropid_string.c_str(), sizeof(tmp_buff));

			char *p = strtok(tmp_buff, "|");
			while (p)
			{
				int dorp_id = atoi(p);
				if (NULL == DROPPOOL->GetDropConfig(dorp_id))
				{
					return -36;
				}

				cfg_item.kill_dropid_set.push_back(static_cast<UInt16>(dorp_id));

				p = strtok(NULL, "|");
			}

			if (cfg_item.kill_dropid_set.empty())
			{
				return -37;
			}
		}

		++ cfg_count;
		dataElement = dataElement.next_sibling();
	}

	if (ROLE_BOSS_MAX_COUNT != cfg_count)
	{
		return -20000;
	}

	return 0;
}
