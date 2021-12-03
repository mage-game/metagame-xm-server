#ifndef __FRIEND_CONFIG_HPP__
#define __FRIEND_CONFIG_HPP__

#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct FriendOtherCfg
{
	FriendOtherCfg() : max_favorable_impression(0), max_day_favorable_impression(0), add_favorable_impression(0) {}

	int max_favorable_impression;
	int max_day_favorable_impression;
	int add_favorable_impression;
};

class FriendConfig : public ConfigBase
{
public:
	FriendConfig();
	~FriendConfig();
	
	bool Init(const std::string &configname, std::string *err);
	const FriendOtherCfg &GetOtherCfg() { return m_other_cfg;}

private:

	int InitOther(PugiXmlNode RootElement);

	FriendOtherCfg m_other_cfg;
};

#endif
