#include "guildxianshuconfig.hpp"
#include "servercommon/guilddef.hpp"
#include "servercommon/configcommon.h"

GuildXianshuConfig::GuildXianshuConfig()
{

}

GuildXianshuConfig::~GuildXianshuConfig()
{

}

bool GuildXianshuConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "GuildXianshuConfig", *err);

	PugiXmlNode root_element = document.document_element();
	if (root_element.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// ÖÕ½áÁ¬Ê¤½±Àø
		PugiXmlNode uplevel_config_element = root_element.child("uplevel_config");
		if (uplevel_config_element.empty())
		{
			*err = path + ": xml node error in [config->uplevel_config]";
			return false;
		}

		iRet = this->InitUplevelCfg(uplevel_config_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitUplevelCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

GuildXianshuConfig::UplevelCfg * GuildXianshuConfig::GetXianshuUplevelCfg(int level)
{
	if (level < 0 || level >= m_xianshu_uplevel_cfglist.cfg_count || level >= GuildXianshuConfig::XIANSHU_MAX_LEVEL)
	{
		return NULL;
	}

	return &m_xianshu_uplevel_cfglist.cfg_list[level];
}

void GuildXianshuConfig::GetXianshuAttr(int level, CharIntAttrs &base_add)
{
	if (level < 0 || level >= m_xianshu_uplevel_cfglist.cfg_count || level >= GuildXianshuConfig::XIANSHU_MAX_LEVEL)
	{
		return;
	}

	GuildXianshuConfig::UplevelCfg &uplevel_cfg = m_xianshu_uplevel_cfglist.cfg_list[level];

	for (int i = 0; i < XIANSHU_ATTR_MAX_NUM; i++)
	{
		if (CharIntAttrs::IsRoleBaseAttr(uplevel_cfg.add_attr_list[i].attr_type))
		{
			base_add.m_attrs[uplevel_cfg.add_attr_list[i].attr_type] += uplevel_cfg.add_attr_list[i].attr_value;
		}
	}
}

int GuildXianshuConfig::InitUplevelCfg(PugiXmlNode root_element)
{
	m_xianshu_uplevel_cfglist.cfg_count = 0;

	int last_xianshu_level = -1;

	PugiXmlNode data_element = root_element.child("data");
	while (!data_element.empty())
	{
		if (m_xianshu_uplevel_cfglist.cfg_count >= XIANSHU_MAX_LEVEL)
		{
			return -100;
		}

		int xianshu_level = 0;
		if (!PugiGetSubNodeValue(data_element, "xianshu_level", xianshu_level) || xianshu_level < 0 || xianshu_level > XIANSHU_MAX_LEVEL || xianshu_level != last_xianshu_level + 1)
		{
			return -1;
		}
		last_xianshu_level = xianshu_level;

		UplevelCfg &uplevel_cfg = m_xianshu_uplevel_cfglist.cfg_list[m_xianshu_uplevel_cfglist.cfg_count];

		if (!PugiGetSubNodeValue(data_element, "need_guild_level", uplevel_cfg.need_guild_level) || uplevel_cfg.need_guild_level < 0 || uplevel_cfg.need_guild_level > GUILD_LEVEL_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "upgrade_need_gongxian", uplevel_cfg.need_gongxian) || uplevel_cfg.need_gongxian <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "longhui_change", uplevel_cfg.longhui_change) || uplevel_cfg.longhui_change < 0)
		{
			return -6;
		}

		const char *LONGHI_ATTR_STR[] = {"maxhp", "gongji", "fangyu", "mingzhong", "shanbi", "baoji", "jianren"};
		UNSTD_STATIC_CHECK(sizeof(LONGHI_ATTR_STR) / sizeof(LONGHI_ATTR_STR[0]) == XIANSHU_ATTR_MAX_NUM);

		for (int i = 0; i < XIANSHU_ATTR_MAX_NUM; i++)
		{
			std::string temp_str(LONGHI_ATTR_STR[i]);

			short attr_type = 0;
			if (!CharIntAttrs::GetAttrTypeFromAttrString(temp_str, &attr_type))
			{
				return -4;
			}
			uplevel_cfg.add_attr_list[i].attr_type = attr_type;

			if (!PugiGetSubNodeValue(data_element, temp_str, uplevel_cfg.add_attr_list[i].attr_value) || uplevel_cfg.add_attr_list[i].attr_value < 0)
			{
				return -5;
			}
		}

		++ m_xianshu_uplevel_cfglist.cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

