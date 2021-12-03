#ifndef __UPDATE_NOTICE_CONFIG_HPP__
#define __UPDATE_NOTICE_CONFIG_HPP__

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct UpdateNoticeOtherConfig
{
	UpdateNoticeOtherConfig() : version(0), is_reward_item(false) {}

	int version;
	bool is_reward_item;
	ItemConfigData reward_item;
};

class UpdateNoticeConfig : public ConfigBase
{
public:
	UpdateNoticeConfig();
	~UpdateNoticeConfig();

	bool Init(const std::string &path, std::string *err);
	
	const UpdateNoticeOtherConfig & GetOtherCfg() { return m_other_cfg; }

private:
	int InitOtherCfg(PugiXmlNode root_element);

	UpdateNoticeOtherConfig m_other_cfg;
};

#endif // __UPDATE_NOTICE_CONFIG_HPP__

