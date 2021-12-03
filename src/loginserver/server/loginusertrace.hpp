#ifndef __LOGIN_USER_TRACE_HPP__
#define __LOGIN_USER_TRACE_HPP__

#include <string>
#include <set>

#include "servercommon/servercommon.h"

class LoginUserTrace
{
public:
	static LoginUserTrace & Instance();

	bool Init(const std::string &path, const std::string &configname, std::string *err);

	void SetOpenFlag(bool is_open);
	void AddPName(PlatName pname);
	void ClearPNameList();

	bool IsTrace(PlatName pname);
	
private:
	LoginUserTrace();
	~LoginUserTrace();

	bool m_is_open;
	std::set<std::string> m_pname_list;
};

#endif // __LOGIN_USER_TRACE_HPP__

