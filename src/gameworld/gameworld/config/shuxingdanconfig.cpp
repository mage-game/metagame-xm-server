#include "shuxingdanconfig.hpp"
#include "servercommon/servercommon.h"

ShuxingdanConfig::ShuxingdanConfig()
{

}

ShuxingdanConfig::~ShuxingdanConfig() 
{

}

bool ShuxingdanConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "ShuxingdanConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// ÅäÖÃ
		PugiXmlNode root_element = RootElement.child("reward");
		if (root_element.empty())
		{
			*err = path + " xml not reward node ";
			return false;
		}

		iRet = this->InitShuxingdanCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitShuxingdanCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int ShuxingdanConfig::InitShuxingdanCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	while (!dataElement.empty())
	{
		ShuxingdanItemConfig cfg;

		if (!PugiGetSubNodeValue(dataElement, "type", cfg.type) || cfg.type <= SHUXINGDAN_TYPE_INVALID || cfg.type >= SHUXINGDAN_TYPE_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "slot_idx", cfg.slot_idx) || cfg.slot_idx < 0) 
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) 
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) 
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) 
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) 
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) 
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0) 
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "attr_per", cfg.attr_per) || cfg.attr_per < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "use_limit_num", cfg.use_limit_num) || cfg.use_limit_num <= 0) 
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "order_limit", cfg.order_limit) || cfg.order_limit <= 0)
		{
			return -12;
		}

		long long key = ConvertParamToLongLong(cfg.type, cfg.slot_idx);

		if (m_shuxingdan_cfglist.end() != m_shuxingdan_cfglist.find(key))
		{
			return -1000;
		}

		m_shuxingdan_cfglist[key] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const ShuxingdanItemConfig * ShuxingdanConfig::GetShuxingdanItem(int type, int slot_idx)
{
	long long key = ConvertParamToLongLong(type, slot_idx);

	ShuxingdanItemMapIt it = m_shuxingdan_cfglist.find(key);
	if (it != m_shuxingdan_cfglist.end())
	{
		return &it->second;
	}

	return NULL;
}

