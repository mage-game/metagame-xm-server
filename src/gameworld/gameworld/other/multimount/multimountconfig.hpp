#ifndef __MULTIMOUNT_CONFIG_H__
#define __MULTIMOUNT_CONFIG_H__

#include "obj/character/attribute.hpp"
#include "servercommon/multimountdef.hpp"
#include "servercommon/newmultimountdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "common/tinyxml/tinyxml.h"
#include <set>
#include <vector>

// 双人坐骑

struct MultiMountOtherConfig
{
	MultiMountOtherConfig() {}
};

struct MultiMountInfoConfig
{
	struct GradeCfg
	{
		GradeCfg() : star_level(0), upgrade_stuff_id(0), upgrade_stuff_num(0), 
			max_bless(0), min_need_bless(0), success_rate(0), fail_add_bless_min(0), fail_add_bless_max(0),
			active_mount_id(0), is_active_image(false), is_broadcast(false), move_speed(0),
			maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) {}

		int GetRandFailBless() const;

		int star_level;															// 阶数id

		ItemID upgrade_stuff_id;											// 进阶消耗物品
		int upgrade_stuff_num;

		int max_bless;
		int min_need_bless;
		int success_rate;
		int fail_add_bless_min;
		int fail_add_bless_max;

		int active_mount_id;
		bool is_active_image;
		bool is_broadcast;

		Attribute move_speed;

		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
		Attribute baoji;
		Attribute jianren;
	};

	MultiMountInfoConfig() : mount_id(0), res_id(0), first_active_img_grade(-1), max_grade(-1) {}

	int mount_id;															// 坐骑ID
	int res_id;																// 资源ID

	int first_active_img_grade;												// 第一个激活形象的阶数（用于使用物品激活坐骑）

	int max_grade;
	GradeCfg upgrade_cfglist[COUPLEMOUNT_MAX_GRADE + 1];						// 进阶配置列表
};

struct MultiMountSpecialConfig
{
	MultiMountSpecialConfig() : image_id(0), res_id(0), move_speed(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) {}

	int image_id;
	int res_id;
	Attribute move_speed;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

struct MultimountEquipConfig
{
	MultimountEquipConfig() : equip_type(0), max_level(0), upgrade_need_stuff(0), upgrade_stuff_count(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0) {}

	int equip_type;
	int max_level;
	int upgrade_need_stuff;
	int upgrade_stuff_count;

	int maxhp;
	int gongji;
	int fangyu;
	int mingzhong;
	int shanbi;
};

struct MultimountSpecialImgUplevelCfg
{
	MultimountSpecialImgUplevelCfg() : special_img_id(0), grade(0), stuff_id(0), stuff_num(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0) {}

	int special_img_id;
	int grade;
	int stuff_id;
	int stuff_num;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

//struct MultimountHuanHuaSpecialCapAddCfgItem
//{
//	MultimountHuanHuaSpecialCapAddCfgItem() : huanhua_id(-1), huanhua_level(0), maxhp(0), gongji(0), fangyu(0) {}
//
//	int huanhua_id;
//	int huanhua_level;
//	int maxhp;
//	int gongji;
//	int fangyu;
//};

class MultiMountConfig : public ConfigBase
{
public:
	MultiMountConfig();
	~MultiMountConfig();

	bool Init(const std::string &configname, std::string *err);

	const MultiMountInfoConfig * GetMultiMountCfg(int mount_id);
	const MultiMountInfoConfig::GradeCfg * GetMultiMountGradeCfg(int mount_id, int grade) const;
	const MultiMountInfoConfig::GradeCfg * GetMultiMountActiveImgCfg(int mount_id) const; // 获取刚好激活坐骑形象的那个阶的配置
	const MultiMountSpecialConfig * GetMultiMountSpecialImgCfg(int image_id) const;
	const MultimountEquipConfig * GetEquipCfg(int equip_type) const;
	const MultimountSpecialImgUplevelCfg *GetSpecialImgUplevelCfg(int special_img_id, int grade);
	//const MultimountHuanHuaSpecialCapAddCfgItem* GetHuanHuaSpecialCapAddCfg(int huanhua_id)const;
	//bool IsAddSpecialCap();

protected:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitMultiMountInfoCfg(PugiXmlNode RootElement);
	int InitMultiMountUpgradeCfg(PugiXmlNode RootElement);
	int InitMultiMountSpecialImgCfg(PugiXmlNode RootElement);
	int InitMultiMountEquipCfg(PugiXmlNode RootElement);
	int InitMultiMountSpecialImgUplevelCfg(PugiXmlNode RootElement);
	//int InitHuanHuaSpecialCapAddCfg(PugiXmlNode RootElement);

private:
	int m_mount_max_id;
	MultiMountInfoConfig m_mount_cfg_list[COUPLEMOUNT_MAX_ID + 1];

	int m_max_special_img_id;
	MultiMountSpecialConfig m_special_img_cfg_list[MULTIMOUNT_SPECIAL_IMG_MAX_ID_LIMIT + 1];

	MultimountEquipConfig m_equip_cfg_list[MULTIMOUNT_EQUIP_TYPE_NUM];

	std::vector<MultimountSpecialImgUplevelCfg> m_multi_mount_special_img_uplevel_cfg;

	//typedef std::map<int, MultimountHuanHuaSpecialCapAddCfgItem> MultimountHuanHuaSpecialCapAddCfg;
	//MultimountHuanHuaSpecialCapAddCfg m_huanhua_special_cap_cfg;
	//bool m_is_open_special_cap_add;
};

#endif

