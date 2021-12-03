#ifndef __CROSS_SERVER_CONFIG_H__
#define __CROSS_SERVER_CONFIG_H__

#include <string>

class CrossServerConfig
{
public:
	static CrossServerConfig & Instance();
	
	bool Init(const std::string &path, std::string *err);

private:
	CrossServerConfig();
	~CrossServerConfig();
};

#endif

