#ifndef _ACTIVITY_SWITCH_CONFIG_HPP_
#define _ACTIVITY_SWITCH_CONFIG_HPP_


#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <set>

// H-���������.xls

class ActivitySwitchConfig : public ConfigBase
{
public:
	ActivitySwitchConfig();
	~ActivitySwitchConfig();

	bool Init(const std::string &configname, std::string *err);

	bool IsBanActivity(int act_id);
	
private:
	int InitBanActivityCfg(PugiXmlNode RootElement);

	
	std::set<int> m_ban_activity_cfg;   // ��ֹǿ���Ļ�б�

};

#endif  // _ACTIVITY_SWITCH_CONFIG_HPP_

