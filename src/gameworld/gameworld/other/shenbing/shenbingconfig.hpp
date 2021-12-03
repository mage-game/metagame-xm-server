#ifndef __SHENBING_CONFIG_HPP__
#define __SHENBING_CONFIG_HPP__

#include <string>
#include "servercommon/struct/shenbingparam.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct ShenBingLevelCfg
{
	ShenBingLevelCfg() : level(0), uplevel_exp(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0),
		extra_zengshang(0), extra_mianshang(0), per_jingzhun(0), per_baoji(0), per_mianshang(0), per_pofang(0), 
		pvp_zengshang_per(0), pve_zengshang_per(0), pvp_jianshang_per(0), pve_jianshang_per(0),
		shuxingdan_limit(0), chengzhangdan_limit(0), is_broadcast(0) {}

	int level;
	int uplevel_exp;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	Attribute extra_zengshang;									//固定增伤
	Attribute extra_mianshang;									//固定减伤

	Attribute per_jingzhun;										//精准（破甲率）
	Attribute per_baoji;											//暴击（暴击伤害率）
	Attribute per_mianshang;										//减伤万分比
	Attribute per_pofang;											//增伤万分比

	Attribute pvp_zengshang_per;									//pvp增伤万分比
	Attribute pve_zengshang_per;									//pve增伤万分比
	Attribute pvp_jianshang_per;									//pvp减伤万分比
	Attribute pve_jianshang_per;									//pve减伤万分比
	int shuxingdan_limit;									// 属性丹限制
	int chengzhangdan_limit;								// 成长丹限制
	bool is_broadcast;
};

struct ShenBingSkillCfg
{
	ShenBingSkillCfg() : skill_idx(0), skill_level(0), shenbing_level(0), param_a(0), param_b(0), param_c(0),param_d(0) {}

	int skill_idx;
	int skill_level;
	int shenbing_level;
	int param_a;
	int param_b;
	int param_c;
	int param_d;
};

struct ShenBingUpLevelStuffCfg
{
	ShenBingUpLevelStuffCfg(): up_level_item_index(0), up_level_item_id(0), level_exp(0), baoji_per(0), baoji_mul(0){}

	short up_level_item_index;
	ItemID up_level_item_id;
	int level_exp;
	int baoji_per;
	int baoji_mul;
};

class ShenBingConfig : public ConfigBase
{
public:
	static const int MAX_SHENBING_LEVEL = 1000;
	static const int MAX_SHENBING_SKILL_COUNT = 4;
	static const int MAX_SHENBING_SKILL_LEVEL = 1;
	static const int MAX_UP_LEVEL_ITEM_COUNT = 3;

	ShenBingConfig();
	~ShenBingConfig();

	bool Init(std::string path, std::string *err);

	const ShenBingLevelCfg * GetShenBingLevelCfg(int level);
	const ShenBingSkillCfg * GetShenBingSkillCfg(int skill_idx, int skill_level);
	const ShenBingUpLevelStuffCfg * GetUpLevelStuffCfg(int stuff_index);

private:
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);
	int InitUpLevelStuffCfg(PugiXmlNode RootElement);

	ShenBingLevelCfg m_level_cfg_list[MAX_SHENBING_LEVEL + 1];
	ShenBingSkillCfg m_skill_cfg_list[MAX_SHENBING_SKILL_COUNT][MAX_SHENBING_SKILL_LEVEL + 1];

	ShenBingUpLevelStuffCfg m_uplevel_item_list[MAX_UP_LEVEL_ITEM_COUNT];
};

#endif // __SHENBING_CONFIG_HPP__
