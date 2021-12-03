#ifndef __JINGJIE_CONFIG_HPP__
#define __JINGJIE_CONFIG_HPP__

#include "servercommon/servercommon.h"

#include "servercommon/struct/itemlistparam.h"
#include "gameworld/gameworld/item/itembase.h"
#include "servercommon/configcommon.h"
#include "servercommon/configbase.h"

const static int MAX_JINGJIE_LEVEL = 100;

struct JingJieLevelCfg
{
	JingJieLevelCfg() : jingjielevel(0), cap_limit(0), stuff_id(0), stuff_num(0), maxhp(0), gongji(0), fangyu(0)
	{

	}

	int jingjielevel;
	int cap_limit;
	ItemID stuff_id;
	int stuff_num;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
};

struct JingjieOtherCfg
{
	JingjieOtherCfg() : yazhi_add_hurt_per(0), yazhi_xuanyun_trigger_rate(0), yazhi_xuanyun_durations(0) {}

	int yazhi_add_hurt_per;					// 压制增伤万分比
	int yazhi_xuanyun_cds;					// 压制眩晕cd（秒）
	int yazhi_xuanyun_trigger_rate;			// 压制眩晕触发概率
	int yazhi_xuanyun_durations;			// 压制眩晕时长
};

class JingJieConfig : public ConfigBase
{
public:

	JingJieConfig();
	~JingJieConfig();

	bool Init(const std::string &path, std::string *err);

	const JingJieLevelCfg * GetJingJieCfgByLevel(int jingjie_level);
	const JingjieOtherCfg & GetOtherCfg() { return m_other_cfg; }
	
private:
	int InitJingjieCfg(TiXmlElement *RootElement);
	int InitOtherCfg(TiXmlElement *RootElement);

	JingJieLevelCfg m_jingjie_cfg_list[MAX_JINGJIE_LEVEL + 1];

	JingjieOtherCfg m_other_cfg;
};

#endif // __JIEZHEN_CONFIG_HPP__

