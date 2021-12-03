#include "crossrandactivityopenconfig.hpp"

#include "servercommon/configcommon.h"

#include "servercommon/serverconfig/configstruct.h"

CrossRandActivityOpenConfig::CrossRandActivityOpenConfig()
	: m_activity_open_time_cfg_count(0)
{

}

CrossRandActivityOpenConfig::~CrossRandActivityOpenConfig()
{

}

CrossRandActivityOpenConfig & CrossRandActivityOpenConfig::Instance()
{
	static CrossRandActivityOpenConfig cvcc;
	return cvcc;
}

bool CrossRandActivityOpenConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};

	PugiXmlDocument document;

	PUGI_XML_LOAD_AND_CHECK(document, path, "CrossRandActivityOpenConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		/*
		// 其他配置
		TiXmlElement *act_open_element = RootElement->FirstChildElement("other");
		if (NULL == act_open_element)
		{
			*err = path + ": xml node error in [other]";
			return false;
		}

		iRet = this->InitOtherCfg(act_open_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
		*/
	}

	{
		// 读取开启时间配置
		PugiXmlNode act_open_element = RootElement.child("act_open_cfg");
		if (NULL == act_open_element)
		{
			*err = path + ": xml node error in [act_open_cfg]";
			return false;
		}

		iRet = this->InitOpenTimeCfg(act_open_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOpenTimeCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const CrossRandActivityTimeCfg * CrossRandActivityOpenConfig::GetOpenCfg(int activity_type)
{
	int act_index = activity_type - CROSS_RAND_ACTIVITY_TYPE_BEGIN;
	if (act_index < 0 || act_index >= CROSS_RAND_ACTIVITY_TYPE_MAX_COUNT)
	{
		return NULL;
	}

	return &m_activity_open_time_cfg_list[act_index];
}

int CrossRandActivityOpenConfig::InitOtherCfg(PugiXmlNode RootElement)
{

	return 0;
}

int CrossRandActivityOpenConfig::InitOpenTimeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return 0; // 允许空配置
	}

	while (!dataElement.empty())
	{
		int activity_type = 0;
		if (!PugiGetSubNodeValue(dataElement, "activity_type", activity_type) || activity_type < CROSS_RAND_ACTIVITY_TYPE_BEGIN || activity_type >= CROSS_RAND_ACTIVITY_TYPE_END)
		{
			return -1;
		}

		CrossRandActivityTimeCfg &cfg_item = m_activity_open_time_cfg_list[activity_type - CROSS_RAND_ACTIVITY_TYPE_BEGIN];

		cfg_item.activity_type = activity_type;

		if (cfg_item.time_cfg_count >= ROSS_RAND_ACTIVITY_TIME_CFG_COUNT_MAX)
		{
			return -2;
		}

		CrossRandActivityTimeCfg::TimeItem &time_item = cfg_item.time_cfg_list[cfg_item.time_cfg_count];

		PugiXmlNode BeginTimeElement = dataElement.child("begin_time");
		if (!BeginTimeElement)
		{
			return -5;
		}

		time_t begin_time = 0;
		if (!ConvertTimeStringToUnixTime2(BeginTimeElement.first_child().value(), &begin_time))
		{
			return -6;
		}

		PugiXmlNode EndTimeElement = dataElement.child("end_time");
		if (!EndTimeElement)
		{
			return -7;
		}

		time_t end_time = 0;
		if (!ConvertTimeStringToUnixTime2(EndTimeElement.first_child().value(), &end_time))
		{
			return -8;
		}

		time_item.begin_timestamp = static_cast<unsigned int>(begin_time);
		time_item.end_timestamp = static_cast<unsigned int>(end_time);

		if (time_item.begin_timestamp >= time_item.end_timestamp)
		{
			return -9;
		}

		// 两个以上的开启时间，限制时间先后(队列存储先进先出)
		if (cfg_item.time_cfg_count > 0 && time_item.begin_timestamp <= cfg_item.time_cfg_list[cfg_item.time_cfg_count - 1].end_timestamp)
		{
			return -10;
		}
		
		++ cfg_item.time_cfg_count;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}


