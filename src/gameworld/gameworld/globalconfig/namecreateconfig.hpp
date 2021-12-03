#ifndef __ACTIVE_NAME_CREATE_HPP__
#define __ACTIVE_NAME_CREATE_HPP__

#include "servercommon/configbase.h"
#include "servercommon/serverdef.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>
#include <string>

class NameCreateConfig : public ConfigBase
{
public:
	NameCreateConfig();
	~NameCreateConfig();

	bool Init(const std::string &configname, std::string *err);
	bool GetRandName(GameName name);
	bool GetRandName(GameName name, int rand_index, bool is_female);

private:
	int InitNameCreateConfig(PugiXmlNode RootElement);

	std::vector<std::string> m_female_first_vec;
	std::vector<std::string> m_female_last_vec;
	std::vector<std::string> m_male_first_vec;
	std::vector<std::string> m_male_last_vec;
};

#endif	// __ACTIVE_NAME_CREATE_HPP__
