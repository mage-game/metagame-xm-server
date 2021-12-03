#ifndef __GUILD_XIANSHU_CONFIG_HPP__
#define __GUILD_XIANSHU_CONFIG_HPP__

#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <string>
#include "obj/character/attribute.hpp"

class GuildXianshuConfig : public ConfigBase
{
public:
	const static int XIANSHU_MAX_LEVEL = 120;
	const static int XIANSHU_ATTR_MAX_NUM = 7;

	struct UplevelCfg 
	{
		UplevelCfg() : xianshu_level(0), need_guild_level(0), need_gongxian(0), longhui_change(0)
		{
		}

		int xianshu_level;
		int need_guild_level;
		int need_gongxian;
		int longhui_change;

		CharIntAttrs::AttrPair add_attr_list[XIANSHU_ATTR_MAX_NUM];
	};

	struct UplevelCfgList
	{
		UplevelCfgList() : cfg_count(0) 
		{
		}

		int cfg_count;
		UplevelCfg cfg_list[XIANSHU_MAX_LEVEL];
	};

	GuildXianshuConfig();
	~GuildXianshuConfig();

	bool Init(const std::string &path, std::string *err);

	UplevelCfg * GetXianshuUplevelCfg(int level);
	void GetXianshuAttr(int level, CharIntAttrs &base_add);

private:
	int InitUplevelCfg(PugiXmlNode root_element);

	UplevelCfgList m_xianshu_uplevel_cfglist;
};

#endif // __GUILD_XIANSHU_CONFIG_HPP__


