#pragma once
#ifndef __SHENQI_CONFIG_HPP__
#define __SHENQI_CONFIG_HPP__
#include <map>
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/shenqidef.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/maildef.hpp"
#include "gameworld/gameworld/config/gameconfigcommon/gameconfigcommon.h"
#include "servercommon/configbase.h"
#include "config/gameconfigcommon/gameconfigcommon.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

static const int SHENQI_MAX_ID = 64;						// 神器id最大值
static const int SHENQI_UPLEVEL_MAX_LEVEL = 100;			// 神器最大等级
static const int SHENQI_UPLEVEL_ADDEXP_CFG_NUM_MAX = 10;	// 神器升级加经验配置最大数

enum SHEN_QI_QUALITY  //品质
{
	SHEN_QI_QUALITY_INVALID = 0,
	SHEN_QI_QUALITY_BLUE = 1,
	SHNE_QI_QUALITY_PURPLE = 2,
	SHEN_QI_QUALITY_ORANGE = 3,
	SHEN_QI_QUALITY_RED = 4,
};
struct ShenqiOtherCfg
{
	ShenqiOtherCfg() {}

	int shenbing_uplevel_stuff_id = 0;			//神兵升级消耗材料id
	int baojia_uplevel_stuff_id = 0;			//宝甲升级消耗材料id
	//int shenbing_active_texiao_stuff_id = 0;	//神兵触发特效消耗材料
	//int shenbing_active_texiao_stuff_count = 0;//神兵触发特效材料数量
	int shenbing_suit_trigger_quality = 0;		//神兵触发特效套装品质
	int shenbing_suit_trigger_per = 0;			//神兵触发特效概率(百分比)
	int shenbing_suit_duration_time = 0;		//神兵触发特效持续时间(s)
	//int baojia_active_texiao_stuff_id = 0;	//宝甲触发特效套装材料
	//int baojia_active_texiao_stuff_count = 0;	//宝甲触发特效材料数量
	int baojia_suit_trigger_quality = 0;		//宝甲触发特效套装品质
	
};

struct ShenqiDecomposeCfg
{
	ShenqiDecomposeCfg() : item_id(0), get_item_count(0) {}

	int item_id;
	int get_item_count;
	ItemConfigData get_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct ShenqiInlayCfg
{
	ShenqiInlayCfg() : id(0), part_type(0)
	{
		memset(inlay_stuff_list, 0, sizeof(inlay_stuff_list));
	}

	short id;
	short part_type;
	unsigned short inlay_stuff_list[SHENQI_QUALITY_TYPE_MAX];
	AttributesConfig attibute_config_list[SHENQI_QUALITY_TYPE_MAX];
};

struct ShenqiImageCfg
{
	ShenqiImageCfg() : id(0), need_quality(0), active_image_id(0) {}

	int id;
	int need_quality;
	int active_image_id;
};

struct ShenqiUplevelCfg
{
	ShenqiUplevelCfg() : level(0), need_exp(0)/*active_texiao_id(0)*/{}

	int level;
	int need_exp;
	AttributesConfig attibute_config;
	//int active_texiao_id;
};

struct ShenqiUplevelAddexpCfg
{
	ShenqiUplevelAddexpCfg() :addexp(0), weight(0) {}

	int addexp;
	int weight;
};

struct ShenqiTeXiaoExtraAttributes
{
	ShenqiTeXiaoExtraAttributes() 
	{
		this->Reset();
	}
	void AddAttrs(CharIntAttrs &attrs, double factor = 1.0) const;
	int ReadConfig(PugiXmlNode & data_element);
	void Reset()
	{
		pojia_per = 0;
		zhufu_per = 0;
		jianshang_per = 0;
		shanbi_per = 0;
		per_gongji = 0;
		per_fangyu = 0;
		per_maxhp = 0;
		per_pofang = 0;
		per_mianshang = 0;
	}
	Attribute pojia_per;
	Attribute zhufu_per;
	Attribute jianshang_per;
	Attribute shanbi_per;
	Attribute per_gongji;
	Attribute per_fangyu;
	Attribute per_maxhp;
	Attribute per_pofang;
	Attribute per_mianshang;
};
struct ShenqiActiveTeXiaoCfg
{
	ShenqiActiveTeXiaoCfg() : seq(0),shenqi_id(0), trigger_quality(0),active_texiao_stuff_id(0), active_texiao_stuff_count(0){}

	int seq;
	int shenqi_id;
	int trigger_quality;
	int active_texiao_stuff_id;
	int active_texiao_stuff_count;
	AttributesConfig attibute_config;
	ShenqiTeXiaoExtraAttributes extra_attibute;
};

class ShenqiConfig : public ConfigBase
{
public:
	ShenqiConfig();
	
	bool Init(const std::string& config_path, std::string* err);

	const ShenqiOtherCfg& GetOtherCfg() { return m_other_cfg; }
	const ShenqiDecomposeCfg* GetDecomposeCfg(const int item_id) const;
	const ShenqiInlayCfg* GetShenBingInlayCfg(const short id, const short part_type) const;
	const ShenqiImageCfg* GetShenbingImageCfg(const short id) const;
	const ShenqiUplevelCfg* GetShenBingUplevelCfg(const int level) const;
	const ShenqiInlayCfg* GetBaojiaInlayCfg(const short id, const short part_type) const;
	const ShenqiImageCfg* GetBaojiaImageCfg(const short id) const;
	const ShenqiUplevelCfg* GetBaojieUplevelCfg(const int level) const;
	const int GetUplevelRandAddexp() const;
	const ShenqiActiveTeXiaoCfg *GetShenbingActiveTeXiaoCfg(const short texiao_id, int quality) const;
	const ShenqiActiveTeXiaoCfg *GetBaojiaActiveTeXiaoCfg(const short texiao_id, int quality) const;

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitDecomposeCfg(PugiXmlNode RootElement);
	int InitShenbingInlayCfg(PugiXmlNode RootElement);
	int InitShenbingImageCfg(PugiXmlNode RootElement);
	int InitShenbingUplevelCfg(PugiXmlNode RootElement);
	int InitBaojiaInlayCfg(PugiXmlNode RootElement);
	int InitBaojiaImageCfg(PugiXmlNode RootElement);
	int InitBaojiaUplevelCfg(PugiXmlNode RootElement);
	int InitUplevelAddexpCfg(PugiXmlNode RootElement);
	int InitShenbingActiveTeXiaoCfg(PugiXmlNode RootElement);
	int InitBaojiagActiveTeXiaoCfg(PugiXmlNode RootElement);

	ShenqiOtherCfg m_other_cfg;									//其他配置
	std::map<int, ShenqiDecomposeCfg> m_decompose_cfg_map;		//分解配置

	std::map<int, ShenqiInlayCfg> m_shenbing_inlay_cfg_map;						//神兵镶嵌配置
	ShenqiImageCfg m_shenbing_image_cfg_list[SHENQI_MAX_ID];					//神兵形象
	int m_shenbing_uplevel_cfg_count;
	ShenqiUplevelCfg m_shenbing_uplevel_cfg_list[SHENQI_UPLEVEL_MAX_LEVEL];		//神兵升级配置

	std::map<int, ShenqiInlayCfg> m_baojia_inlay_cfg_map;						//宝甲镶嵌配置
	ShenqiImageCfg m_baojia_image_cfg_list[SHENQI_MAX_ID];						//宝甲形象
	int m_baojia_uplevel_cfg_count;
	ShenqiUplevelCfg m_baojia_uplevel_cfg_list[SHENQI_UPLEVEL_MAX_LEVEL];		//宝甲升级配置

	int m_shenbing_active_texiao_cfg_count;
	ShenqiActiveTeXiaoCfg m_shenbing_active_texiao_list[SHENQI_MAX_ID];				//神兵激活特效配置
	int m_baojia_active_texiao_cfg_count;
	ShenqiActiveTeXiaoCfg m_baojia_active_texiao_list[SHENQI_MAX_ID];				//宝甲激活特效配置

	int m_uplevel_addexp_cfg_count;
	int m_uplevel_addexp_total_weight;
	ShenqiUplevelAddexpCfg m_uplevel_addexp_cfg_list[SHENQI_UPLEVEL_ADDEXP_CFG_NUM_MAX]; //升级加经验配置
};

#endif