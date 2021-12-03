#include "jingjieconfig.hpp"
#include "servercommon/configcommon.h"

JingJieConfig::JingJieConfig()
{
}

JingJieConfig::~JingJieConfig()
{
}

bool JingJieConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	TiXmlDocument document;
	if ("" == path || !document.LoadFile(path))
	{
		sprintf(err_info, "%s load JingJieConfig fail !\n %s", path.c_str(), document.ErrorDesc());
		*err = err_info;
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();
	if (NULL == RootElement)
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// 境界配置
		TiXmlElement *root_element = RootElement->FirstChildElement("jingjie");
		if (NULL == root_element)
		{
			*err = path + " xml not jingjie node ";
			return false;
		}

		iRet = this->InitJingjieCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitJingjieCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 其他配置
		TiXmlElement *root_element = RootElement->FirstChildElement("other");
		if (NULL == root_element)
		{
			*err = path + " xml not [ other ] node ";
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


	return true;
}

int JingJieConfig::InitJingjieCfg(TiXmlElement *RootElement)
{
	if (NULL == RootElement)
	{
		return -10000;
	}
	
	TiXmlElement *data_element = RootElement->FirstChildElement("data");
	if (NULL == data_element)
	{
		return -20000;
	}

	int jingjie_level = 0;
	int last_jingjie_level = -1;
	
	while (NULL != data_element)
	{
		if (!GetSubNodeValue(data_element, "jingjie_level", jingjie_level) || jingjie_level < 0 || jingjie_level > MAX_JINGJIE_LEVEL || jingjie_level != last_jingjie_level + 1)
		{
			return -1;
		}

		last_jingjie_level = jingjie_level;
		JingJieLevelCfg &level_cfg = m_jingjie_cfg_list[jingjie_level];
		level_cfg.jingjielevel = jingjie_level;
		if (!GetSubNodeValue(data_element, "cap_limit", level_cfg.cap_limit) || level_cfg.cap_limit < 0)
		{
			return -2;
		}

		if (!GetSubNodeValue(data_element, "stuff_id", level_cfg.stuff_id) || level_cfg.stuff_id < 0)
		{
			return -3;
		}

		if (!GetSubNodeValue(data_element, "stuff_num", level_cfg.stuff_num) || level_cfg.stuff_num < 0)
		{
			return -4;
		}

		int tmp_val = 0;
		if (!GetSubNodeValue(data_element, "maxhp", tmp_val) || tmp_val < 0) return -10;
		level_cfg.maxhp = tmp_val;

		if (!GetSubNodeValue(data_element, "gongji", tmp_val) || tmp_val < 0) return -20;
		level_cfg.gongji = tmp_val;

		if (!GetSubNodeValue(data_element, "fangyu", tmp_val) || tmp_val < 0) return -30;
		level_cfg.fangyu = tmp_val;

		data_element = data_element->NextSiblingElement();
	}

	return 0;
}

int JingJieConfig::InitOtherCfg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	if (NULL == dataElement)
	{
		return -100000;
	}

	if (!GetSubNodeValue(dataElement, "yazhi_add_hurt_per", m_other_cfg.yazhi_add_hurt_per) || m_other_cfg.yazhi_add_hurt_per < 0)
	{
		return -1;
	}

	if (!GetSubNodeValue(dataElement, "yazhi_xuanyun_cds", m_other_cfg.yazhi_xuanyun_cds) || m_other_cfg.yazhi_xuanyun_cds <= 0)
	{
		return -2;
	}

	if (!GetSubNodeValue(dataElement, "yazhi_xuanyun_trigger_rate", m_other_cfg.yazhi_xuanyun_trigger_rate) || m_other_cfg.yazhi_xuanyun_trigger_rate < 0)
	{
		return -3;
	}

	if (!GetSubNodeValue(dataElement, "yazhi_xuanyun_durations", m_other_cfg.yazhi_xuanyun_durations) || m_other_cfg.yazhi_xuanyun_durations <= 0)
	{
		return -4;
	}

	return 0;
}

const JingJieLevelCfg * JingJieConfig::GetJingJieCfgByLevel(int jingjie_level)
{
	if (jingjie_level <= 0 || jingjie_level > MAX_JINGJIE_LEVEL)
	{
		return NULL;
	}

	if (m_jingjie_cfg_list[jingjie_level].jingjielevel != jingjie_level)
	{
		return NULL;
	}

	return &m_jingjie_cfg_list[jingjie_level];
}

