#include <memory.h>
#include <stdlib.h>

#include "fightmountconfig.hpp"
#include "servercommon/servercommon.h"
#include "globalconfig/equipconfig.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "obj/character/attribute.hpp"
#include "gameworld/checkresourcecenter.hpp"

#include <string>


FightMountOpConfig::FightMountOpConfig() : m_use_jinjie_card_reward_itemid(0), m_chengzhangdan_percent(0), m_active_equip_grade(0), m_active_equip_skill_level(0), 
											m_is_open_special_cap_add(0), m_medal_skill_index(0)
{
	memset(m_equip_skill_level_cfg, 0, sizeof(m_equip_skill_level_cfg));
}

FightMountOpConfig::~FightMountOpConfig()
{

}

bool FightMountOpConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "FightMountOpConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 其他
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = InitOther(other_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOther failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 等级配置
		PugiXmlNode lvl_element = RootElement.child("level");
		if (lvl_element.empty())
		{
			*err = configname + ": no [level].";
			return false;
		}

		iRet = InitLevelCfg(lvl_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 进阶配置
		PugiXmlNode grade_element = RootElement.child("grade");
		if (grade_element.empty())
		{
			*err = configname + ": no [grade].";
			return false;
		}

		iRet = InitFightMountGradeCfg(grade_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitFightMountGradeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 进阶成功率配置
		PugiXmlNode upgrade_succ_element = RootElement.child("upgrade_prob");
		if (upgrade_succ_element.empty())
		{
			*err = configname + ": no [upgrade_prob].";
			return false;
		}

		iRet = InitUpGradeProbCfg(upgrade_succ_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitUpGradeProbCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 进阶形象
		PugiXmlNode image_element = RootElement.child("image_list");
		if (image_element.empty())
		{
			*err = configname + ": no [image_list].";
			return false;
		}

		iRet = InitImageCfg(image_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitImageCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 特殊形象配置
		PugiXmlNode mount_equip_element = RootElement.child("special_img");
		if (mount_equip_element.empty())
		{
			*err = configname + ": no [special_img].";
			return false;
		}

		iRet = InitSpecialImageCfg(mount_equip_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitSpecialImageCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 坐骑特殊形象进阶
		PugiXmlNode special_img_upgrade_element = RootElement.child("special_image_upgrade");
		if (special_img_upgrade_element.empty())
		{
			*err = configname + ": no [special_image_upgrade].";
			return false;
		}

		iRet = InitSpecialImageUpgradeCfg(special_img_upgrade_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitSpecialImageUpgradeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 装备配置
		PugiXmlNode mount_equip_element = RootElement.child("mount_equip_info");
		if (mount_equip_element.empty())
		{
			*err = configname + ": no [mount_equip_element].";
			return false;
		}

		iRet = InitEquipCfg(mount_equip_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitEquipCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 坐骑技能
		PugiXmlNode mount_skill_element = RootElement.child("mount_skill");
		if (mount_skill_element.empty())
		{
			*err = configname + ": no [mount_skill_element].";
			return false;
		}

		iRet = InitSkillCfg(mount_skill_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitSkillCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 升星物品配置
		PugiXmlNode up_star_stuff = RootElement.child("up_star_stuff");
		if (NULL == up_star_stuff)
		{
			*err = configname + ": no [up_star_stuff].";
			return false;
		}

		iRet = InitUpStarStuffCfg(up_star_stuff);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitUpStarStuffCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 升星经验值配置
		PugiXmlNode up_star_exp = RootElement.child("up_star_exp");
		if (NULL == up_star_exp)
		{
			*err = configname + ": no [up_star_exp].";
			return false;
		}

		iRet = InitUpStarExpCfg(up_star_exp);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitUpStarExpCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 装备技能等级配置
		PugiXmlNode equip_skill_level = RootElement.child("equip_skill_level");
		if (NULL == equip_skill_level)
		{
			*err = configname + ": no [equip_skill_level].";
			return false;
		}

		iRet = InitEquipSkillLevel(equip_skill_level);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitEquipSkillLevel failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 幻化特殊战力加成
		PugiXmlNode huanhua_special_cap_add = RootElement.child("huanhua_special_cap_add");
		if (NULL == huanhua_special_cap_add)
		{
			*err = configname + ": no [huanhua_special_cap_add].";
			return false;
		}

		iRet = InitHuanHuaSpecialCapAddCfg(huanhua_special_cap_add);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitHuanHuaSpecialCapAddCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int FightMountOpConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "mount_grade_reward_id", m_use_jinjie_card_reward_itemid) || NULL == ITEMPOOL->GetItem(m_use_jinjie_card_reward_itemid))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "chengzhangdan_percent", m_chengzhangdan_percent) || m_chengzhangdan_percent <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "active_equip_grade", m_active_equip_grade) || m_active_equip_grade < 0 || m_active_equip_grade > FightMountParam::MAX_GRADE)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "active_equip_skill_level", m_active_equip_skill_level) || m_active_equip_skill_level <= 0 || m_active_equip_skill_level > FightMountParam::EQUIP_MAX_LEVEL)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_open_special_cap_add", m_is_open_special_cap_add))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "medal_skill_index", m_medal_skill_index) || m_medal_skill_index < 0 || m_medal_skill_index >= FightMountParam::SKILL_COUNT)
		{
			return -6;
		}
	}

	return 0;
}

int FightMountOpConfig::InitLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");

	int last_level = 0;
	while (!dataElement.empty())
	{
		int mount_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "mount_level", mount_level) || mount_level <= 0 || mount_level > FightMountParam::MAX_LEVEL)
		{
			return -1;
		}

		FightMountLevelConfig cfg;
		cfg.mount_level = mount_level;

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0) return -107;

		m_level_list[mount_level] = cfg;
		last_level = mount_level;

		dataElement = dataElement.next_sibling();
	}

	if (last_level != FightMountParam::MAX_LEVEL)
	{
		return -2;
	}

	return 0;
}

int FightMountOpConfig::InitFightMountGradeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		short grade = 0;
		if (!PugiGetSubNodeValue(dataElement, "grade", grade) || grade < 0 || grade >= FightMountParam::MAX_GRADE)
		{
			return -1;
		}

		FightMountGradeConfig &cfg = m_grade_cfg_list[grade];
		cfg.grade = grade;

		if (!PugiGetSubNodeValue(dataElement, "client_grade", cfg.client_grade) || cfg.client_grade < 0 || cfg.client_grade >= FightMountParam::MAX_GRADE)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "upgrade_stuff_id", cfg.upgrade_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.upgrade_stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "upgrade_stuff2_id", cfg.upgrade_stuff2_id) || nullptr == ITEMPOOL->GetItem(cfg.upgrade_stuff2_id))
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "upgrade_stuff_count", cfg.upgrade_stuff_count) || cfg.upgrade_stuff_count <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "coin", cfg.coin) || cfg.coin < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "image_id", cfg.active_image_id) || cfg.active_image_id <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "bless_val_limit", cfg.bless_val_limit) || cfg.bless_val_limit <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "broadcast", cfg.is_broadcast))
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "shuxingdan_limit", cfg.shuxingdan_limit) || cfg.shuxingdan_limit < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "chengzhangdan_limit", cfg.chengzhangdan_limit) || cfg.chengzhangdan_limit < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_clear_bless", cfg.is_clear_bless) || cfg.is_clear_bless < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "bless_addition", cfg.bless_addition) || cfg.bless_addition < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip_level_limit", cfg.equip_level_limit) || cfg.equip_level_limit < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0) return -107;

		if (!PugiGetSubNodeValue(dataElement, "movespeed", cfg.movespeed) || cfg.movespeed < 0) return -108;

		if (!PugiGetSubNodeValue(dataElement, "pack_num", cfg.pack_num) || cfg.pack_num < 0) return -109;

		if (!PugiGetSubNodeValue(dataElement, "equip_level_toplimit", cfg.equip_level_toplimit) || cfg.equip_level_toplimit < 0) return -110;

		if (!PugiGetSubNodeValue(dataElement, "extra_zengshang", cfg.extra_zengshang) || cfg.extra_zengshang < 0) return -111;

		if (!PugiGetSubNodeValue(dataElement, "extra_mianshang", cfg.extra_mianshang) || cfg.extra_mianshang < 0) return -112;

		if (!PugiGetSubNodeValue(dataElement, "per_jingzhun", cfg.per_jingzhun) || cfg.per_jingzhun < 0) return -113;

		if (!PugiGetSubNodeValue(dataElement, "per_baoji", cfg.per_baoji) || cfg.per_baoji < 0) return -114;

		if (!PugiGetSubNodeValue(dataElement, "jianshang_per", cfg.per_mianshang) || cfg.per_mianshang < 0) return -115;

		if (!PugiGetSubNodeValue(dataElement, "zengshang_per", cfg.per_pofang) || cfg.per_pofang < 0) return -116;

		if (!PugiGetSubNodeValue(dataElement, "pvp_zengshang_per", cfg.pvp_zengshang_per) || cfg.pve_zengshang_per < 0) return -117;

		if (!PugiGetSubNodeValue(dataElement, "pve_zengshang_per", cfg.pve_zengshang_per) || cfg.pve_zengshang_per < 0) return -118;

		if (!PugiGetSubNodeValue(dataElement, "pvp_jianshang_per", cfg.pvp_jianshang_per) || cfg.pvp_jianshang_per < 0) return -119;

		if (!PugiGetSubNodeValue(dataElement, "pve_jianshang_per", cfg.pve_jianshang_per) || cfg.pve_jianshang_per < 0) return -120;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FightMountOpConfig::InitUpGradeProbCfg(PugiXmlNode RootElement)
{
	short last_high = -1;
	short last_grade = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int grade = 0;
		if (!PugiGetSubNodeValue(dataElement, "grade", grade) || grade < 0)
		{
			return -1;
		}

		if (grade != last_grade) last_high = -1;

		FightMountUpGradeProbConfig cfg;

		if (!PugiGetSubNodeValue(dataElement, "low_bless", cfg.low_bless_val) || cfg.low_bless_val < 0)
		{
			return -3;
		}

		if (cfg.low_bless_val != last_high + 1) 
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(dataElement, "high_bless", cfg.high_bless_val) || cfg.high_bless_val < cfg.low_bless_val)
		{
			return -4;
		}

		last_high = cfg.high_bless_val;

		if (!PugiGetSubNodeValue(dataElement, "succ_rate", cfg.succ_rate) || cfg.succ_rate < 0 || cfg.succ_rate > 10000)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "fail_add_1_rate", cfg.fail_add_1_bless_rate) || cfg.fail_add_1_bless_rate < 0 || cfg.fail_add_1_bless_rate > 100)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "fail_add_2_rate", cfg.fail_add_2_bless_rate) || cfg.fail_add_2_bless_rate < 0 || cfg.fail_add_2_bless_rate > 100)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "fail_add_3_rate", cfg.fail_add_3_bless_rate) || cfg.fail_add_3_bless_rate < 0 || cfg.fail_add_3_bless_rate > 100)
		{
			return -8;
		}

		if (cfg.fail_add_1_bless_rate + cfg.fail_add_2_bless_rate + cfg.fail_add_3_bless_rate != 100)
		{
			return -9;
		}

		cfg.fail_add_2_bless_rate += cfg.fail_add_1_bless_rate;
		cfg.fail_add_3_bless_rate += cfg.fail_add_2_bless_rate;

		std::map<int, FightMountUpGradeProbList>::iterator iter = m_upgrade_prob_map.find(grade);
		if (m_upgrade_prob_map.end() != iter)
		{
			FightMountUpGradeProbList &cfg_list = iter->second;
			if (!cfg_list.Add(cfg))
			{
				return -50;
			}
		}
		else
		{
			FightMountUpGradeProbList cfg_list;
			cfg_list.grade = grade;
			cfg_list.Add(cfg);
			m_upgrade_prob_map[grade] = cfg_list;
		}

		last_grade = grade;

		dataElement = dataElement.next_sibling();
	}

	for (std::map<int, FightMountUpGradeProbList>::const_iterator iter = m_upgrade_prob_map.begin(); m_upgrade_prob_map.end() != iter; ++iter)
	{
		const FightMountUpGradeProbList &cfg_list = iter->second;

		if (cfg_list.prob_count <= 0 || cfg_list.prob_count > FightMountUpGradeProbList::MAX_PROB_COUNT_PER_GRADE)
		{
			return -60;
		}
		const FightMountGradeConfig *grade_cfg = this->GetGradeConfig(cfg_list.grade);
		if (NULL == grade_cfg)
		{
			return -100;
		}
		if (grade_cfg->bless_val_limit != cfg_list.prob_list[cfg_list.prob_count - 1].high_bless_val)
		{
			return -101;
		}
	}

	return 0;
}

int FightMountOpConfig::InitImageCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int image_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "image_id", image_id) || image_id < 0)
		{
			return -1;
		}

		FightMountImageConfig cfg;

		cfg.image_id = image_id;

		m_image_list[cfg.image_id] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FightMountOpConfig::InitSpecialImageCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int image_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "image_id", image_id) || image_id <= 0 || image_id >= FightMountParam::MAX_SPECIAL_IMAGE_COUNT)
		{
			return -1;
		}

		FightMountSpecialImgCfg &cfg_item = m_special_img_cfglist[image_id];
		if (cfg_item.IsValid())
		{
			return -2;
		}

		/*if (!PugiGetSubNodeValue(dataElement, "zhuanzhi_prof", cfg_item.zhuanzhi_prof) || cfg_item.zhuanzhi_prof < 0)
		{
			return -3;
		}*/

		cfg_item.image_id = image_id;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FightMountOpConfig::InitSpecialImageUpgradeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_image_id = 0;
	int last_grade = -1;

	while (!dataElement.empty())
	{
		int image_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "special_img_id", image_id) || image_id <= 0 || image_id >= FightMountParam::MAX_SPECIAL_IMAGE_COUNT)
		{
			return -1;
		}

		if (image_id != last_image_id)
		{
			last_image_id = image_id;
			last_grade = -1;
		}

		FightMountSpecialImgUpgradeCfgList &cfg_list = m_special_img_upgrade_map[image_id];

		cfg_list.max_grade = 0;
		if (!PugiGetSubNodeValue(dataElement, "grade", cfg_list.max_grade) || cfg_list.max_grade < 0 || cfg_list.max_grade > FightMountParam::MAX_UPGRADE_LIMIT 
			|| cfg_list.max_grade != last_grade + 1)
		{
			return -2;
		}

		last_grade = cfg_list.max_grade;

		FightMountSpecialImgUpgradeCfg &cfg_item = cfg_list.upgrade_list[cfg_list.max_grade];
		cfg_item.grade = cfg_list.max_grade;

		if (!PugiGetSubNodeValue(dataElement, "stuff_id", cfg_item.stuff_id) || NULL == ITEMPOOL->GetItem(cfg_item.stuff_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_num", cfg_item.stuff_num) || cfg_item.stuff_num <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "shuxingdan_count", cfg_item.shuxingdan_count) || cfg_item.shuxingdan_count < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "chengzhangdan_count", cfg_item.chengzhangdan_count) || cfg_item.chengzhangdan_count < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip_level", cfg_item.equip_level) || cfg_item.equip_level < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg_item.maxhp) || cfg_item.maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg_item.gongji) || cfg_item.gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg_item.fangyu) || cfg_item.fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg_item.mingzhong) || cfg_item.mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg_item.shanbi) || cfg_item.shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg_item.baoji) || cfg_item.baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg_item.jianren) || cfg_item.jianren < 0) return -107;

		if (!PugiGetSubNodeValue(dataElement, "extra_zengshang", cfg_item.extra_zengshang) || cfg_item.extra_zengshang < 0) return -108;

		if (!PugiGetSubNodeValue(dataElement, "extra_mianshang", cfg_item.extra_mianshang) || cfg_item.extra_mianshang < 0) return -109;

		if (!PugiGetSubNodeValue(dataElement, "per_jingzhun", cfg_item.per_jingzhun) || cfg_item.per_jingzhun < 0) return -110;

		if (!PugiGetSubNodeValue(dataElement, "per_baoji", cfg_item.per_baoji) || cfg_item.per_baoji < 0) return -111;

		if (!PugiGetSubNodeValue(dataElement, "jianshang_per", cfg_item.per_mianshang) || cfg_item.per_mianshang < 0) return -112;

		if (!PugiGetSubNodeValue(dataElement, "zengshang_per", cfg_item.per_pofang) || cfg_item.per_pofang < 0) return -113;

		if (!PugiGetSubNodeValue(dataElement, "pvp_zengshang_per", cfg_item.pvp_zengshang_per) || cfg_item.pve_zengshang_per < 0) return -114;

		if (!PugiGetSubNodeValue(dataElement, "pve_zengshang_per", cfg_item.pve_zengshang_per) || cfg_item.pve_zengshang_per < 0) return -115;

		if (!PugiGetSubNodeValue(dataElement, "pvp_jianshang_per", cfg_item.pvp_jianshang_per) || cfg_item.pvp_jianshang_per < 0) return -116;

		if (!PugiGetSubNodeValue(dataElement, "pve_jianshang_per", cfg_item.pve_jianshang_per) || cfg_item.pve_jianshang_per < 0) return -117;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FightMountOpConfig::InitEquipCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int equip_idx = 0;
	int equip_level = 0;
	int old_equip_idx = 0;
	int old_equip_level = 0;
	while (!dataElement.empty())
	{

		if (!PugiGetSubNodeValue(dataElement, "equip_idx", equip_idx) || equip_idx < 0 || equip_idx >= FightMountParam::EQUIP_COUNT || equip_idx < old_equip_idx)
		{
			return -1;
		}

		FightMountEquipConifg &equip_cfg = m_equip_cfg_list[equip_idx];
		equip_cfg.equip_idx = equip_idx;

		if (old_equip_idx != equip_idx)
		{
			old_equip_level = equip_idx;
			old_equip_level = 0;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip_level", equip_level) || equip_level < 0 || equip_level > FightMountParam::EQUIP_MAX_LEVEL || equip_level < old_equip_level) return -2;

		old_equip_level = equip_level;

		FightMountEquipConifg::FightMountEquipUpLevelConfig &equip_level_cfg = equip_cfg.equip_level_cfg_list[equip_level];
		equip_level_cfg.equip_level = equip_level;

		PugiXmlNode Itemelement = dataElement.child("item");
		if (!equip_level_cfg.item.ReadConfig(Itemelement)) 
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", equip_level_cfg.maxhp) || equip_level_cfg.maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", equip_level_cfg.gongji) || equip_level_cfg.gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", equip_level_cfg.fangyu) || equip_level_cfg.fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", equip_level_cfg.mingzhong) || equip_level_cfg.mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", equip_level_cfg.shanbi) || equip_level_cfg.shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", equip_level_cfg.baoji) || equip_level_cfg.baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", equip_level_cfg.jianren) || equip_level_cfg.jianren < 0) return -107;

		if (!PugiGetSubNodeValue(dataElement, "add_percent", equip_level_cfg.add_percent) || equip_level_cfg.add_percent < 0) return -108;

		equip_cfg.max_level++;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FightMountOpConfig::InitSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int skill_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_idx", skill_idx) || skill_idx < 0 || skill_idx >= FightMountParam::EQUIP_COUNT)
		{
			return -1;
		}

		int skill_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_level", skill_level) || skill_level < 0)
		{
			return -2;
		}

		FightMountSkillConifg &cfg = m_skill_cfg_list[skill_idx][skill_level];
		cfg.index = skill_idx;
		cfg.skill_level = skill_level;

		if (!PugiGetSubNodeValue(dataElement, "skill_type", cfg.skill_type) || cfg.skill_type < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "grade", cfg.mount_grade) || cfg.mount_grade < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "uplevel_stuff_id", cfg.uplevel_stuff_id) || (cfg.skill_type > 0 && NULL == ITEMPOOL->GetItem(cfg.uplevel_stuff_id)))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "uplevel_stuff_num", cfg.uplevel_stuff_num) || cfg.uplevel_stuff_num < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_a", cfg.param_a) || cfg.param_a < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_b", cfg.param_b) || cfg.param_b < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_c", cfg.param_c) || cfg.param_c < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_d", cfg.param_d) || cfg.param_d < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0) return -107;

		if (!PugiGetSubNodeValue(dataElement, "capability", cfg.capability) || cfg.capability < 0) return -108;

		if (!PugiGetSubNodeValue(dataElement, "skill_cap_per", cfg.skill_cap_per) || cfg.skill_cap_per < 0) return -109;

		if (!PugiGetSubNodeValue(dataElement, "base_attr_add_per", cfg.base_attr_add_per) || cfg.base_attr_add_per < 0) return -110;

		if (!PugiGetSubNodeValue(dataElement, "end_day_of_up_skill3", cfg.end_day_of_up_skill) || cfg.end_day_of_up_skill < 0) return -111;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

// 初始化升星物品数据
int FightMountOpConfig::InitUpStarStuffCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_up_star_item_index = -1;
	while (!dataElement.empty())
	{
		int up_star_item_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "up_star_item_index", up_star_item_index) || up_star_item_index < 0 || up_star_item_index > FightMountParam::MAX_UP_STAR_ITEM_COUNT || up_star_item_index <= last_up_star_item_index)
		{
			return -1;
		}

		FightMountUpStarStuffCfg &cfg = m_upstar_item_list[up_star_item_index];
		cfg.up_star_item_index = up_star_item_index;

		last_up_star_item_index = up_star_item_index;

		if (!PugiGetSubNodeValue(dataElement, "up_star_item_id", cfg.up_star_item_id) || cfg.up_star_item_id < 0 || NULL == ITEMPOOL->GetItem(cfg.up_star_item_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "star_exp", cfg.star_exp) || cfg.star_exp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji_per", cfg.baoji_per) || cfg.baoji_per < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji_mul", cfg.baoji_mul) || cfg.baoji_mul < 0)
		{
			return -5;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

// 初始化升星经验值数据
int FightMountOpConfig::InitUpStarExpCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_star_level = -1;
	while (!dataElement.empty())
	{
		int star_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "star_level", star_level) || star_level < 0 || star_level > FightMountParam::MAX_STAR_LEVEL || star_level <= last_star_level)
		{
			return -1;
		}

		FightMountUpStarExpCfg &cfg = m_upstar_exp_list[star_level];
		last_star_level = star_level;

		if (!PugiGetSubNodeValue(dataElement, "up_star_level_exp", cfg.up_star_level_exp) || cfg.up_star_level_exp <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "up_star_total_exp", cfg.up_star_total_exp) || cfg.up_star_total_exp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren", cfg.jianren) || cfg.jianren < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "speed", cfg.movespeed) || cfg.movespeed < 0)
		{
			return -11;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FightMountOpConfig::InitEquipSkillLevel(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int equip_min_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_min_level", equip_min_level) || equip_min_level < 0 || equip_min_level >= FightMountParam::EQUIP_MAX_LEVEL)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "equip_skill_level", m_equip_skill_level_cfg[equip_min_level]) || m_equip_skill_level_cfg[equip_min_level] < 0)
		{
			return -2;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int FightMountOpConfig::InitHuanHuaSpecialCapAddCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		FightMountHuanHuaSpecialCapAddCfgItem cfg_item;
		if (!PugiGetSubNodeValue(dataElement, "huanhua_id", cfg_item.huanhua_id) || cfg_item.huanhua_id <= 0 || cfg_item.huanhua_id >= FightMountParam::MAX_SPECIAL_IMAGE_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "huanhua_level", cfg_item.huanhua_level) || cfg_item.huanhua_level <= 0 || cfg_item.huanhua_level > FightMountParam::MAX_UPGRADE_LIMIT)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg_item.maxhp) || cfg_item.maxhp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg_item.gongji) || cfg_item.gongji < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg_item.fangyu) || cfg_item.fangyu < 0)
		{
			return -5;
		}

		// 判断重复
		if (m_huanhua_special_cap_cfg.find(cfg_item.huanhua_id) != m_huanhua_special_cap_cfg.end())
		{
			return -6;
		}
		m_huanhua_special_cap_cfg[cfg_item.huanhua_id] = cfg_item;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const FightMountLevelConfig * FightMountOpConfig::GetLevelConfig(int mount_level) const
{
	if (mount_level < 0 || mount_level > FightMountParam::MAX_LEVEL)
	{
		return NULL;
	}

	if (m_level_list[mount_level].mount_level != mount_level)
	{
		return NULL;
	}

	return &m_level_list[mount_level];
}

const FightMountGradeConfig * FightMountOpConfig::GetGradeConfig(int grade) const
{
	if (grade < 0 || grade >= FightMountParam::MAX_GRADE || m_grade_cfg_list[grade].grade <= 0)
	{
		return NULL;
	}

	return &m_grade_cfg_list[grade];	
}

const FightMountUpGradeProbConfig * FightMountOpConfig::GetUpGradeProbConfig(int grade, int bless_val) const
{
	std::map<int, FightMountUpGradeProbList>::const_iterator iter = m_upgrade_prob_map.find(grade);
	if (m_upgrade_prob_map.end() != iter)
	{
		const FightMountUpGradeProbList &cfg_list = iter->second;

		for (int i = 0; i < cfg_list.prob_count && i < FightMountUpGradeProbList::MAX_PROB_COUNT_PER_GRADE; ++i)
		{
			if (cfg_list.prob_list[i].low_bless_val <= bless_val && bless_val <= cfg_list.prob_list[i].high_bless_val)
			{
				return &cfg_list.prob_list[i];
			}
		}
	}	

	return NULL;
}

const FightMountImageConfig * FightMountOpConfig::GetFightMountImage(int mount_attr_id) const
{
	if (mount_attr_id < 0 || mount_attr_id >= FightMountParam::MAX_IMAGE_COUNT)
	{
		return NULL;
	}

	if (m_image_list[mount_attr_id].image_id != mount_attr_id)
	{
		return NULL;
	}

	return &m_image_list[mount_attr_id];
}

const FightMountSpecialImgCfg * FightMountOpConfig::GetSpecialImgCfg(int special_img_id)
{
	if (special_img_id > 0 && special_img_id < FightMountParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		if (m_special_img_cfglist[special_img_id].IsValid())
		{
			return &m_special_img_cfglist[special_img_id];
		}
	}

	return NULL;
}


const FightMountSpecialImgUpgradeCfg * FightMountOpConfig::GetSpecialImgUpgradeCfg(int special_img_id, int grade)
{
	FightMountSpecialImgUpgradeCfgMap::iterator iter = m_special_img_upgrade_map.find(special_img_id);
	if (iter != m_special_img_upgrade_map.end())
	{
		FightMountSpecialImgUpgradeCfgList &cfg_list = iter->second;

		if (grade < 0 || grade > FightMountParam::MAX_UPGRADE_LIMIT || grade > iter->second.max_grade)
		{
			return NULL;
		}

		if (cfg_list.upgrade_list[grade].grade == grade)
		{
			return &cfg_list.upgrade_list[grade];
		}
	}

	return NULL;
}

const FightMountEquipConifg * FightMountOpConfig::GetEquipCfg(int equip_idx)
{
	if (equip_idx >= 0 && equip_idx < FightMountParam::EQUIP_COUNT)
	{
		return &m_equip_cfg_list[equip_idx];
	}

	return NULL;
}


const FightMountSkillConifg * FightMountOpConfig::GetSkillCfg(int skill_idx, int level)
{
	if (skill_idx >= 0 && skill_idx < FightMountParam::SKILL_COUNT 
		&& level > 0 && level <= FightMountParam::SKILL_MAX_LEVEL 
		&& m_skill_cfg_list[skill_idx][level].skill_level > 0)
	{
		return &m_skill_cfg_list[skill_idx][level];
	}

	return NULL;
}

const FightMountUpStarExpCfg * FightMountOpConfig::GetUpStarExpCfg(int star_level)
{
	if (star_level < 0 || star_level >= FightMountParam::MAX_STAR_LEVEL || m_upstar_exp_list[star_level].up_star_level_exp <= 0)
		return NULL;

	return &m_upstar_exp_list[star_level];
}

const FightMountUpStarStuffCfg * FightMountOpConfig::GetUpStarStuffCfg(int stuff_index)
{
	if (stuff_index < 0 || stuff_index >= FightMountParam::MAX_UP_STAR_ITEM_COUNT)
		return NULL;

	return &m_upstar_item_list[stuff_index];
}

int FightMountOpConfig::GetEquipSkillLevel(int min_equip_level)
{
	if (min_equip_level >= m_active_equip_skill_level && min_equip_level < FightMountParam::EQUIP_MAX_LEVEL)
	{
		return m_equip_skill_level_cfg[min_equip_level];
	}

	return -1;
}

const FightMountHuanHuaSpecialCapAddCfgItem* FightMountOpConfig::GetHuanHuaSpecialCapAddCfg(int huanhua_id)const
{
	if (huanhua_id <= 0 || huanhua_id >= FightMountParam::MAX_SPECIAL_IMAGE_COUNT)
	{
		return nullptr;
	}

	auto iter = m_huanhua_special_cap_cfg.find(huanhua_id);
	if (iter != m_huanhua_special_cap_cfg.end())
	{
		return &(iter->second);
	}

	return nullptr;
}

bool FightMountOpConfig::IsAddSpecialCap()
{
	return m_is_open_special_cap_add;
}
