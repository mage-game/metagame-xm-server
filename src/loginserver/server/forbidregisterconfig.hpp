#ifndef __FORBID_REGISTER_COFIG_HPP__
#define __FORBID_REGISTER_COFIG_HPP__

#include "common/tinyxml/tinyxml.h"
#include <map>
#include <string>
#include <set>

union PlatUnion
{
	int id;
	char spid[4];
};

class ForbidRegisterConfig
{
public:
	ForbidRegisterConfig();
	~ForbidRegisterConfig();

	bool Init(const std::string &path, std::string *err);

	int GetForbidRegisterHour() { return m_forbid_register_time_h;  }

private:
	int InitCloseRegRoleCfg(TiXmlElement *RootElement);

	int m_forbid_register_time_h;
	std::map<int, int> m_agentadapt_map;
};

#endif // __FORBID_REGISTER_COFIG_HPP__

