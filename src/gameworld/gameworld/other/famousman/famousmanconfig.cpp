#include "famousmanconfig.hpp"
#include "servercommon/configcommon.h"

FamousManConfig::FamousManConfig():m_during_times(0)
{
	
}

FamousManConfig::~FamousManConfig()
{

}

bool FamousManConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "FamousManConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// 名人配置
		PugiXmlNode root_element = RootElement.child("famous_man");
		if (root_element.empty())
		{
			*err = path + " xml not goldvip_active node ";
			return false;
		}

		iRet = this->InitFamousManCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitFamousManCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 名人其他配置
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitOthernCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOthernCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int FamousManConfig::InitFamousManCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_famous_type = 0;
	while (!data_element.empty())
	{
		int famous_man_type = 0;
		if (!PugiGetSubNodeValue(data_element, "famous_type", famous_man_type) || famous_man_type <= FAMOUSN_MAN_INVAILD || famous_man_type >= FAMOUS_MAN_MAX_TYPE || famous_man_type <= last_famous_type)
		{
			return -1;
		}

		FamousManCfg &cfg = m_famous_man_cfg[famous_man_type];
		cfg.famous_type = famous_man_type;
		last_famous_type = famous_man_type;

		if (!PugiGetSubNodeValue(data_element, "param_1", cfg.param_1) || cfg.param_1 <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "param_2", cfg.param_2) || cfg.param_2 < 0)
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const FamousManCfg * FamousManConfig::GetFamousManCfg(int famous_type)
{
	if (famous_type <= FAMOUSN_MAN_INVAILD || famous_type >= FAMOUS_MAN_MAX_TYPE)
	{
		return NULL;
	}

	if (m_famous_man_cfg[famous_type].famous_type != famous_type)
	{
		return NULL;
	}

	return &m_famous_man_cfg[famous_type];
}

int FamousManConfig::InitOthernCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		//int famous_man_type = 0;
		if (!PugiGetSubNodeValue(data_element, "during_time", m_during_times) || m_during_times < 0)
		{
			return -1;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}
