#ifndef __IMG_FULING_CONFIG_HPP__
#define __IMG_FULING_CONFIG_HPP__

#include "servercommon/imgfulingdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/serverdef.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <vector>
#include <string>
#include <map>

struct ImgFulingLevelCfg
{
	ImgFulingLevelCfg() : level(0), exp(0), maxhp(0), gongji(0), fangyu(0),
				mingzhong(0), shanbi(0), jianren(0), per_add(0), broadcast(false)
	{}

	int level;
	unsigned int exp;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	int per_add;
	bool broadcast;
};

struct ImgFulingSkillCfg
{
	ImgFulingSkillCfg() : system_type(0), level(0), img_count_limit(0), param_a(0), param_b(0), param_c(0), param_d(0), param_e(0), broadcast(false)
	{}

	int system_type;
	int level;
	int img_count_limit;
	int param_a;
	int param_b;
	int param_c;
	int param_d;
	int param_e;
	bool broadcast;
};

struct ImgFulingStuffCfg
{
	ImgFulingStuffCfg() : stuff_id(0), add_exp(0)
	{}

	ItemID stuff_id;
	unsigned int add_exp;
};

struct ImgFulingJingJieEquipPerAdd
{
	ImgFulingJingJieEquipPerAdd() : min_level(0), jingjie_equip_per_add(0)
	{}

	int min_level;
	int jingjie_equip_per_add;
};

class ImgFulingConfig : public ConfigBase
{
public:
	ImgFulingConfig();
	~ImgFulingConfig();

	bool Init(const std::string &configname, std::string *err);

	const ImgFulingLevelCfg * GetLevelCfg(int jingjie_type, int level);
	const ImgFulingLevelCfg * GetAttrCfg(int jingjie_type, int level);

	int GetJingJieTypeBySkillIndex(int index);
	int GetSkillIndexByJingJieType(int jingjie_type);
	const ImgFulingSkillCfg * GetSkillCfg(int index, int skill_level);
	int GetSkillLevelByCount(int index, int img_count);

	const ImgFulingStuffCfg * GetStuffCfg(int jingjie_type, int stuff_index);
	int GetJingJieEquipPerAdd(int min_level);

private:
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);
	int InitUpLevelStuffCfg(PugiXmlNode RootElement);
	int InitJineJieEquipPerAddCfg(PugiXmlNode RootElement);

	std::vector<ImgFulingLevelCfg> m_img_fuling_level_cfg_list[JINGJIE_TYPE_COUNT];
	std::vector<ImgFulingSkillCfg> m_img_fuling_skill_cfg_list[IMG_FULING_SKILL_MAX];
	std::vector<ImgFulingStuffCfg> m_img_fuling_stuff_cfg_list[JINGJIE_TYPE_COUNT];
	std::vector<ImgFulingJingJieEquipPerAdd> m_img_fuling_jingjie_equip_per_add_cfg_vec;
};

#endif // __IMG_FULING_CONFIG_HPP__
