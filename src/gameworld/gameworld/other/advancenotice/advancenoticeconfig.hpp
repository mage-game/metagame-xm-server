#ifndef __ADVANCENOTICE_CONFIG_HPP__
#define __ADVANCENOTICE_CONFIG_HPP__

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/maildef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <map>

struct AdvanceNoticeGuideCfg
{
	AdvanceNoticeGuideCfg() : id(0), start_level(0), end_level(0) {}

	int id;
	short start_level;
	short end_level;
	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
};

struct AdvanceNoticeDayCfg
{
	AdvanceNoticeDayCfg() : id(0), open_day(0), level_limit(0) {}

	int id;
	short open_day;
	short level_limit;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

class AdvanceNoticeConfig : public ConfigBase
{
public:
	AdvanceNoticeConfig();
	~AdvanceNoticeConfig();

	bool Init(std::string path, std::string *err);

	const AdvanceNoticeGuideCfg * GetAdvanceNoticeCfg(int id);
	const AdvanceNoticeDayCfg * GetAdvanceNoticeDayCfg(int id);

private:
	int InitAdvanceNoticeGuideCfg(PugiXmlNode RootElement);
	int InitAdvanceDayCfg(PugiXmlNode RootElement);

	std::map<int, AdvanceNoticeGuideCfg> m_advance_notice_map;
	std::map<int, AdvanceNoticeDayCfg> m_advance_notice_day_map;
};

#endif // __ADVANCENOTICE_CONFIG_HPP__

