#ifndef _HUANGCHENGHUIZHAN_CONFIG_HPP_
#define _HUANGCHENGHUIZHAN_CONFIG_HPP_

#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamedef.h"

#include <vector>

struct HchzOtherCfg
{
	HchzOtherCfg() : sceneid(0), time(0){}

	int sceneid;
	int time;
};

struct HchzRefreshCfg
{
	HchzRefreshCfg() : monsterid(0), num(0) {}

	int monsterid;
	int num;
};

class HuangchenghuizhanConfig : public ConfigBase
{
public:
	HuangchenghuizhanConfig();
	~HuangchenghuizhanConfig();

	bool Init(const std::string &configname, std::string *err);

	const HchzOtherCfg & GetOtherCfg() { return m_other_cfg; }
	const std::vector<HchzRefreshCfg> & GetRefreshCfg() { return m_refresh_vec; }
	void GetRandomRefresPos(Posi *posi);
	bool IsHuangchenghuizhanMonsterId(int monsterid);

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitRefreshCfg(PugiXmlNode RootElement);
	int InitPosCfg(PugiXmlNode RootElement);
	
	HchzOtherCfg m_other_cfg;

	typedef std::vector<HchzRefreshCfg> HchzRefreshVec;
	HchzRefreshVec m_refresh_vec;

	typedef std::vector<Posi> HchzPosVec;
	HchzPosVec m_pos_vec;
};

#endif



