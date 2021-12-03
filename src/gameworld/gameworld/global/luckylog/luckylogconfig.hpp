#ifndef __LUCKY_LOG_CONFIG_H__
#define __LUCKY_LOG_CONFIG_H__

#include "servercommon/configcommon.h"
#include "servercommon/configbase.h"
#include <vector>
#include <map>
#include "servercommon/serverdef.h"

#define LUCKY_LOG_CFG() LOGIC_CONFIG->GetLuckyLogConfig()

class LuckyLogConfig : public ConfigBase
{
	using NeedLogItemVector = std::vector<ItemID>;
	using NeedLogItemMap = std::map<int, NeedLogItemVector>;

public:
	LuckyLogConfig() {};
	~LuckyLogConfig() {};

	bool Init(const std::string &configname, std::string *err);

	bool CheckNeedLog(int activity_type, ItemID item_id);

private:

	int InitLogInfo(TiXmlElement *root_element);

	NeedLogItemMap m_need_log_item_map;
};

#endif // __LUCKY_LOG_CONFIG_H__