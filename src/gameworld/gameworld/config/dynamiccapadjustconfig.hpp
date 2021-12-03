#ifndef __DYNAMIC_CAP_ADJUST_CONFIG_HPP__
#define __DYNAMIC_CAP_ADJUST_CONFIG_HPP__

#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/servercommon.h"
#include <string>
#include <vector>
#include <map>

struct CapAdjustKeyObj 
{
	CapAdjustKeyObj() : cap_type(-1), param_0(0), param_1(0)
	{}

	CapAdjustKeyObj(int _cap_type, int _param_0, int _param_1) : cap_type(_cap_type), param_0(_param_0), param_1(_param_1)
	{}

	bool operator <(const CapAdjustKeyObj &rhs) const
	{
		if (cap_type != rhs.cap_type)
		{
			return cap_type < rhs.cap_type;
		}
		else if (param_0 != rhs.param_0)
		{
			return param_0 < rhs.param_0;
		}

		return param_1 < rhs.param_1;
	}

	int cap_type;
	int param_0;
	int param_1;
};

/*
 *该配置用于解决的问题:利用存在全局表里的数据去计算角色的战力，则在隐藏服内会因没有全局数据而导致战力下降
 */

class DynamicCapAdjustConfig : public ConfigBase
{
public:
	DynamicCapAdjustConfig();
	~DynamicCapAdjustConfig();

	bool Init(const std::string &path, std::string *err);

	int GetSubIndex(const CapAdjustKeyObj &key);
	const std::map<CapAdjustKeyObj, int> & GetAdjustMap() { return m_cap_adjust_map; }

private:
	int InitCapAdjustCfg(PugiXmlNode RootElement);

	std::map<CapAdjustKeyObj, int> m_cap_adjust_map;
};

#endif // __DYNAMIC_CAP_ADJUST_CONFIG_HPP__

