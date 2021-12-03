#ifndef __ZHIBAO_CONFIG_HPP__
#define __ZHIBAO_CONFIG_HPP__

#include <string>
#include "gamedef.h"
#include "servercommon/basedef.h"
#include "servercommon/struct/zhibaoparam.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "obj/character/attribute.hpp"

struct ZhiBaoLevelCfg
{
	ZhiBaoLevelCfg() : level(0), upleve_exp(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0),
		image_id(0), mount_attr_add(0), wing_attr_add(0) {}

	int level;
	int upleve_exp;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	short image_id;
	int mount_attr_add;
	int wing_attr_add;
};

struct ZhiBaoSkillCfg
{
	ZhiBaoSkillCfg() : skill_idx(0), skill_level(0), zhibao_level(0), param_a(0), param_b(0), param_c(0), param_d(0) {}

	int skill_idx;
	int skill_level;
	int zhibao_level;
	int param_a;
	int param_b;
	int param_c;
	int param_d;
};

struct ZhiBaoHuanhuaCfg
{
	struct ConfigItem
	{
		ConfigItem() : id(0), active_item_id(0)
		{
		}

		int id;
		ItemID active_item_id;
	};

	ZhiBaoHuanhuaCfg() : huanhua_count(0) {}
	int huanhua_count;
	ConfigItem cfg_list[MAX_ZHIBAO_HUANHUA_TPYE_COUNT];
};

struct ZhiBaoHuanhuaGradeCfg
{
	ZhiBaoHuanhuaGradeCfg()
	{

	}

	struct GradeCfg
	{
		GradeCfg() : huanhua_type(0), grade(0), stuff_id(0), stuff_count(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
		{

		}

		int huanhua_type;
		int grade;
		int stuff_id;
		int stuff_count;

		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
	};

	GradeCfg m_grade_list[MAX_ZHIBAO_HUANHUA_TPYE_COUNT][MAX_ZHIBAO_HUANHUA_GRADE_COUNT];
};

class ZhiBaoConfig : public ConfigBase
{
public:
	ZhiBaoConfig();
	~ZhiBaoConfig();

	bool Init(std::string path, std::string *err);

	ZhiBaoLevelCfg * GetZhiBaoLevelCfg(int level);
	ZhiBaoSkillCfg * GetZhiBaoSkillCfg(int skill_idx, int skill_level);
	int GetHuanHuaCount() { return m_huanhua_cfg.huanhua_count; }
	ZhiBaoHuanhuaCfg::ConfigItem * GetHuanHuaCfg(int huanhua_id);
	ZhiBaoHuanhuaGradeCfg::GradeCfg * GetHuanhuaGradeCfg(int huanhua_type, int grade);

private:
	int InitLevelCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);
	int InitAcitvityHuanHuaCfg(PugiXmlNode RootElement);
	int InitHuanhuaGradeCfg(PugiXmlNode RootElement);

	ZhiBaoLevelCfg m_level_cfg_list[MAX_ZHIBAO_LEVEL + 1];
	ZhiBaoSkillCfg m_skill_cfg_list[MAX_ZHIBAO_SKILL_COUNT][MAX_ZHIBAO_SKILL_LEVEL + 1];
	ZhiBaoHuanhuaCfg m_huanhua_cfg;
	ZhiBaoHuanhuaGradeCfg m_huanhua_grade_list;
};

#endif // __ZHIBAO_CONFIG_HPP__
