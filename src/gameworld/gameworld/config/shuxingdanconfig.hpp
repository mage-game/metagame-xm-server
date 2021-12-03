#ifndef __SHU_XING_DAN_CONFIG_H__
#define __SHU_XING_DAN_CONFIG_H__

#include <map>
#include "servercommon/configcommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

static const int SHUXINGDAN_MAX_TYPE = 2;				// 属性丹类型最大数

enum SHUXINGDAN_TYPE
{
	SHUXINGDAN_TYPE_INVALID = 0,
	SHUXINGDAN_TYPE_JINGLING,
	SHUXINGDAN_TYPE_XIULIAN,
	SHUXINGDAN_TYPE_CHENGJIU,
	SHUXINGDAN_TYPE_SHENGWANG,
	SHUXINGDAN_TYPE_MOUNT,
	SHUXINGDAN_TYPE_WING,
	SHUXINGDAN_TYPE_HALO,
	SHUXINGDAN_TYPE_SHENGONG,
	SHUXINGDAN_TYPE_SHENYI,
	SHUXINGDAN_TYPE_FIGHTMOUNT,
	SHUXINGDAN_TYPE_SHENBING,
	SHUXINGDAN_TYPE_FOOTPRINT,
	SHUXINGDAN_TYPE_CLOAK,
	SHUXINGDAN_TYPE_FABAO,
	SHUXINGDAN_TYPE_SHIZHUANG,
	SHUXINGDAN_TYPE_SHIZHUANG_SHENBING,
	SHUXINGDAN_TYPE_QILINBI,
	SHUXINGDAN_TYPE_MASK,
	SHUXINGDAN_TYPE_TOUSHI,
	SHUXINGDAN_TYPE_YAOSHI,


	SHUXINGDAN_TYPE_UPGRADE_BEGIN = 100,		// 进阶系统属性丹类型 100 + upgrade_type
	SHUXINGDAN_TYPE_UPGRADE_END = 130,

	SHUXINGDAN_TYPE_MAX,
};

enum SHUXINGDAN_SLOT_TYPE
{
	SHUXINGDAN_SLOT_TYPE_ZIZHI = 0,		//资质丹
	SHUXINGDAN_SLOT_TYPE_CHENGZHANG,	//成长丹

	SHUXINGDAN_SLOT_TYPE_MAX,
};

struct ShuxingdanItemConfig
{
	ShuxingdanItemConfig() : type(0), slot_idx(0), use_limit_num(0), order_limit(0),
	gongji(0), fangyu(0), maxhp(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), attr_per(0){}

	int type;
	int slot_idx;
	int use_limit_num;
	//使用阶数限制
	int order_limit;

	Attribute gongji;
	Attribute fangyu;
	Attribute maxhp;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	int attr_per;
};

class ShuxingdanConfig : public ConfigBase
{
public:
	ShuxingdanConfig();
	~ShuxingdanConfig();

	bool Init(std::string path, std::string *err);

	const ShuxingdanItemConfig * GetShuxingdanItem(int type, int slot_idx);

private:
	int InitShuxingdanCfg(PugiXmlNode RootElement);

	typedef std::map<long long, ShuxingdanItemConfig> ShuxingdanItemMap;
	typedef std::map<long long, ShuxingdanItemConfig>::iterator ShuxingdanItemMapIt;

	ShuxingdanItemMap m_shuxingdan_cfglist;
};


#endif