#ifndef __COMMON_ACTIVITY_CONFIG_HPP__
#define __COMMON_ACTIVITY_CONFIG_HPP__


#include <string>
#include <set>
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class CommonActivityConfig : public ConfigBase
{
public:
	CommonActivityConfig();
	~CommonActivityConfig();

	bool Init(std::string path, std::string *err);

	bool IsUnitOpen(int unit_type);

private:
	int InitOpenUnitCfg(PugiXmlNode RootElement);

	std::set<int> m_open_unit;							// 开启的单元
};

#endif

