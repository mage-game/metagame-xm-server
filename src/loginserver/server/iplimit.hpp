#ifndef __IP_LIMIT_HPP__
#define __IP_LIMIT_HPP__

#include <string>
#include <set>

class IPLimit
{
public:
	static IPLimit & Instance();

	bool Init(const std::string& path, const std::string& configname, std::string* err);
	
	bool GetLimitSwitch() { return m_limit_switch; }
	bool CanPassIPLimit(const std::string & ip);

private:
	IPLimit();
	~IPLimit();

	bool m_limit_switch; 
	
	typedef std::set<std::string> AuthIPList;
	typedef std::set<std::string>::iterator AuthIPListIt;

	AuthIPList m_auth_ip_list;
};

#endif  // __IP_LIMIT_HPP__
