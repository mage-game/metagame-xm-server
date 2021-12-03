#ifndef __GIFT_REPLACE_CONFIG_HPP__
#define __GIFT_REPLACE_CONFIG_HPP__

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <map>
#include <vector>

struct GiftReplaceCfg
{
	GiftReplaceCfg() : gift_id(0), role_min_level(0), role_max_level(0), re_gift_id(0) {}

	ItemID gift_id;
	int role_min_level;
	int role_max_level;
	ItemID re_gift_id;
};

class GiftReplaceConfig : public ConfigBase
{
public:
	GiftReplaceConfig();
	~GiftReplaceConfig();

	bool Init(std::string path, std::string *err);
	bool GetReplaceGiftId(ItemID &gift_id, int role_level);

private:
	int InitGiftReplaceCfg(PugiXmlNode RootElement);

	typedef std::vector<GiftReplaceCfg> GiftReplaceVec;
	typedef std::map<int, GiftReplaceVec> GiftReplaceMap;
	typedef std::map<int, GiftReplaceVec>::iterator GiftReplaceMapIt;
	GiftReplaceMap m_gift_replace_map;
};

#endif // __GIFT_REPLACE_CONFIG_HPP__

