#include <memory.h>
#include <stdlib.h>

#include "cloakconfig.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "obj/character/attribute.hpp"
#include "gameworld/checkresourcecenter.hpp"
#include "skill/skillpool.hpp"

#include <string>

CloakConfig::CloakConfig()
{
	memset(m_equip_skill_level_cfg, 0, sizeof(m_equip_skill_level_cfg));
}

CloakConfig::~CloakConfig()
{

}

bool CloakConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = { 0 };
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "CloakConfig", *err);

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
			sprintf(errinfo, "%s: InitOther failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 特殊形象配置
		PugiXmlNode spec_halo_element = RootElement.child("special_img");
		if (spec_halo_element.empty())
		{
			*err = configname + ": no [special_img].";
			return false;
		}

		iRet = InitSpecialImageCfg(spec_halo_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitSpecialImageCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 光环特殊形象进阶
		PugiXmlNode special_img_upgrade_element = RootElement.child("special_image_upgrade");
		if (special_img_upgrade_element.empty())
		{
			*err = configname + ": no [special_image_upgrade].";
			return false;
		}

		iRet = InitSpecialImageUpgradeCfg(special_img_upgrade_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitSpecialImageUpgradeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 装备配置
		PugiXmlNode halo_equip_element = RootElement.child("cloak_equip_info");
		if (halo_equip_element.empty())
		{
			*err = configname + ": no [halo_equip_element].";
			return false;
		}

		iRet = InitEquipCfg(halo_equip_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitEquipCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 光环技能
		PugiXmlNode halo_skill_element = RootElement.child("cloak_skill");
		if (halo_skill_element.empty())
		{
			*err = configname + ": no [halo_skill_element].";
			return false;
		}

		iRet = InitSkillCfg(halo_skill_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitSkillCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 升级物品配置
		PugiXmlNode up_star_stuff = RootElement.child("up_level_stuff");
		if (NULL == up_star_stuff)
		{
			*err = configname + ": no [up_level_stuff].";
			return false;
		}

		iRet = InitUpLevelStuffCfg(up_star_stuff);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitUpLevelStuffCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 等级经验值配置
		PugiXmlNode up_star_exp = RootElement.child("up_level_cfg");
		if (NULL == up_star_exp)
		{
			*err = configname + ": no [up_level_cfg].";
			return false;
		}

		iRet = InitUpLevelCfg(up_star_exp);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitUpLevelCfg failed %d", configname.c_str(), iRet);
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
			sprintf(errinfo, "%s: InitImageCfg failed %d", configname.c_str(), iRet);
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
			sprintf(errinfo, "%s: InitEquipSkillLevel failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int CloakConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "chengzhangdan_percent", m_other_cfg.chengzhangdan_percent) || m_other_cfg.chengzhangdan_percent < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "active_equip_level", m_other_cfg.active_equip_level) || m_other_cfg.active_equip_level < 0 || m_other_cfg.active_equip_level > CloakParam::MAX_LEVEL)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "active_equip_skill_level", m_other_cfg.active_equip_skill_level) || m_other_cfg.active_equip_skill_level <= 0 || m_other_cfg.active_equip_skill_level > CloakParam::EQUIP_MAX_LEVEL)
		{
			return -4;
		}
	}

	return 0;
}

int CloakConfig::InitImageCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	while (!dataElement.empty())
	{
		int image_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "image_id", image_id) || image_id < 0 || image_id >= CloakParam::MAX_IMAGE_COUNT)
		{
			return -1;
		}

		CloakImageConfig cfg;

		cfg.image_id = image_id;

		m_image_list[cfg.image_id] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CloakConfig::InitSpecialImageCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int image_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "image_id", image_id) || image_id <= 0 || image_id > CloakParam::MAX_SPECIAL_IMAGE_ID)
		{
			return -1;
		}

		CloakSpecialImgCfg &cfg_item = m_special_img_cfglist[image_id];
		if (cfg_item.IsValid())
		{
			return -2;
		}

		cfg_item.image_id = image_id;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CloakConfig::InitSpecialImageUpgradeCfg(PugiXmlNode RootElement)
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
		if (!PugiGetSubNodeValue(dataElement, "special_img_id", image_id) || image_id <= 0 || image_id > CloakParam::MAX_SPECIAL_IMAGE_ID)
		{
			return -1;
		}

		if (image_id != last_image_id)
		{
			last_image_id = image_id;
			last_grade = -1;
		}

		CloakSpecialImgUpgradeCfgList &cfg_list = m_special_img_upgrade_map[image_id];

		cfg_list.max_grade = 0;
		if (!PugiGetSubNodeValue(dataElement, "grade", cfg_list.max_grade) || cfg_list.max_grade < 0 || cfg_list.max_grade > CloakParam::MAX_UPGRADE_LIMIT
			|| cfg_list.max_grade != last_grade + 1)
		{
			return -2;
		}

		last_grade = cfg_list.max_grade;

		CloakSpecialImgUpgradeCfg &cfg_item = cfg_list.upgrade_list[cfg_list.max_grade];
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

		/*if (!PugiGetSubNodeValue(dataElement, "extra_zengshang", cfg_item.extra_zengshang) || cfg_item.extra_zengshang < 0) return -108;

		if (!PugiGetSubNodeValue(dataElement, "extra_mianshang", cfg_item.extra_mianshang) || cfg_item.extra_mianshang < 0) return -109;

		if (!PugiGetSubNodeValue(dataElement, "per_jingzhun", cfg_item.per_jingzhun) || cfg_item.per_jingzhun < 0) return -110;

		if (!PugiGetSubNodeValue(dataElement, "per_baoji", cfg_item.per_baoji) || cfg_item.per_baoji < 0) return -111;

		if (!PugiGetSubNodeValue(dataElement, "jianshang_per", cfg_item.per_mianshang) || cfg_item.per_mianshang < 0) return -112;

		if (!PugiGetSubNodeValue(dataElement, "zengshang_per", cfg_item.per_pofang) || cfg_item.per_pofang < 0) return -113;

		if (!PugiGetSubNodeValue(dataElement, "pvp_zengshang_per", cfg_item.pvp_zengshang_per) || cfg_item.pve_zengshang_per < 0) return -114;

		if (!PugiGetSubNodeValue(dataElement, "pve_zengshang_per", cfg_item.pve_zengshang_per) || cfg_item.pve_zengshang_per < 0) return -115;

		if (!PugiGetSubNodeValue(dataElement, "pvp_jianshang_per", cfg_item.pvp_jianshang_per) || cfg_item.pvp_jianshang_per < 0) return -116;

		if (!PugiGetSubNodeValue(dataElement, "pve_jianshang_per", cfg_item.pve_jianshang_per) || cfg_item.pve_jianshang_per < 0) return -117;*/

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CloakConfig::InitEquipCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int equip_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_idx", equip_idx) || equip_idx < 0 || equip_idx >= CloakParam::EQUIP_COUNT)
		{
			return -1;
		}

		CloakEquipConifg &equip_cfg = m_equip_cfg_list[equip_idx];

		int level = 0;

		if (!PugiGetSubNodeValue(dataElement, "equip_level", level) || level < 0 || level >= CloakParam::EQUIP_MAX_LEVEL) return -2;

		PugiXmlNode item_element = dataElement.child("item");
		if (NULL != item_element)
		{
			if (!equip_cfg.level_cfg[level].item.ReadConfig(item_element))
			{
				return -3;
			}
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", equip_cfg.level_cfg[level].maxhp) || equip_cfg.level_cfg[level].maxhp < 0) return -101;

		if (!PugiGetSubNodeValue(dataElement, "gongji", equip_cfg.level_cfg[level].gongji) || equip_cfg.level_cfg[level].gongji < 0) return -102;

		if (!PugiGetSubNodeValue(dataElement, "fangyu", equip_cfg.level_cfg[level].fangyu) || equip_cfg.level_cfg[level].fangyu < 0) return -103;

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", equip_cfg.level_cfg[level].mingzhong) || equip_cfg.level_cfg[level].mingzhong < 0) return -104;

		if (!PugiGetSubNodeValue(dataElement, "shanbi", equip_cfg.level_cfg[level].shanbi) || equip_cfg.level_cfg[level].shanbi < 0) return -105;

		if (!PugiGetSubNodeValue(dataElement, "baoji", equip_cfg.level_cfg[level].baoji) || equip_cfg.level_cfg[level].baoji < 0) return -106;

		if (!PugiGetSubNodeValue(dataElement, "jianren", equip_cfg.level_cfg[level].jianren) || equip_cfg.level_cfg[level].jianren < 0) return -107;

		if (!PugiGetSubNodeValue(dataElement, "add_percent", equip_cfg.level_cfg[level].add_percent) || equip_cfg.level_cfg[level].add_percent < 0) return -108;

		equip_cfg.max_level++;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CloakConfig::InitSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int skill_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_idx", skill_idx) || skill_idx < 0 || skill_idx >= CloakParam::EQUIP_COUNT)
		{
			return -1;
		}

		int skill_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_level", skill_level) || skill_level <= 0)
		{
			return -2;
		}

		CloakSkillConifg &cfg = m_skill_cfg_list[skill_idx][skill_level];
		cfg.index = skill_idx;
		cfg.skill_level = skill_level;

		if (!PugiGetSubNodeValue(dataElement, "skill_type", cfg.skill_type) || cfg.skill_type < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "level", cfg.cloak_level) || cfg.cloak_level < 0)
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

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CloakConfig::InitUpLevelStuffCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_up_level_item_index = -1;
	while (!dataElement.empty())
	{
		int up_level_item_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "up_level_item_index", up_level_item_index) || up_level_item_index < 0 || up_level_item_index >= CloakParam::MAX_UP_LEVEL_ITEM_COUNT || up_level_item_index != last_up_level_item_index + 1)
		{
			return -1;
		}

		CloakUpLevelStuffCfg &cfg = m_upstar_item_list[up_level_item_index];
		cfg.up_level_item_index = up_level_item_index;

		last_up_level_item_index = up_level_item_index;

		if (!PugiGetSubNodeValue(dataElement, "up_level_item_id", cfg.up_level_item_id) || cfg.up_level_item_id < 0 || NULL == ITEMPOOL->GetItem(cfg.up_level_item_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_exp", cfg.add_exp) || cfg.add_exp < 0)
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

int CloakConfig::InitUpLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_level = -1;
	int last_image = 0;
	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level < 0 || level > CloakParam::MAX_LEVEL || level != last_level + 1)
		{
			return -1;
		}

		CloakUpLevelCfg &cfg = m_up_level_cfg_list[level];
		cfg.level = level;
		last_level = level;

		if (!PugiGetSubNodeValue(dataElement, "up_level_exp", cfg.up_level_exp) || cfg.up_level_exp <= 0)
		{
			return -2;
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

		if (!PugiGetSubNodeValue(dataElement, "pack_num", cfg.pack_num) || cfg.pack_num <= 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "active_image", cfg.active_image) || cfg.active_image < 0 || cfg.active_image >= CloakParam::MAX_IMAGE_COUNT || cfg.active_image < last_image)
		{
			return -12;
		}

		last_image = cfg.active_image;

		if (!PugiGetSubNodeValue(dataElement, "is_broadcast", cfg.is_broadcast) || cfg.is_broadcast < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_broadcast_image", cfg.is_broadcast_image) || cfg.is_broadcast_image < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "shuxingdan_limit", cfg.shuxingdan_limit) || cfg.shuxingdan_limit < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement, "chengzhangdan_limit", cfg.chengzhangdan_limit) || cfg.chengzhangdan_limit < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(dataElement, "extra_zengshang", cfg.extra_zengshang) || cfg.extra_zengshang < 0) return -108;

		if (!PugiGetSubNodeValue(dataElement, "extra_mianshang", cfg.extra_mianshang) || cfg.extra_mianshang < 0) return -109;

		if (!PugiGetSubNodeValue(dataElement, "per_jingzhun", cfg.per_jingzhun) || cfg.per_jingzhun < 0) return -110;

		if (!PugiGetSubNodeValue(dataElement, "per_baoji", cfg.per_baoji) || cfg.per_baoji < 0) return -111;

		if (!PugiGetSubNodeValue(dataElement, "jianshang_per", cfg.per_mianshang) || cfg.per_mianshang < 0) return -112;

		if (!PugiGetSubNodeValue(dataElement, "zengshang_per", cfg.per_pofang) || cfg.per_pofang < 0) return -113;

		if (!PugiGetSubNodeValue(dataElement, "pvp_zengshang_per", cfg.pvp_zengshang_per) || cfg.pve_zengshang_per < 0) return -114;

		if (!PugiGetSubNodeValue(dataElement, "pve_zengshang_per", cfg.pve_zengshang_per) || cfg.pve_zengshang_per < 0) return -115;

		if (!PugiGetSubNodeValue(dataElement, "pvp_jianshang_per", cfg.pvp_jianshang_per) || cfg.pvp_jianshang_per < 0) return -116;

		if (!PugiGetSubNodeValue(dataElement, "pve_jianshang_per", cfg.pve_jianshang_per) || cfg.pve_jianshang_per < 0) return -117;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int CloakConfig::InitEquipSkillLevel(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int equip_min_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "equip_min_level", equip_min_level) || equip_min_level < 0 || equip_min_level >= CloakParam::EQUIP_MAX_LEVEL)
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

const CloakImageConfig * CloakConfig::GetCommonImage(int attr_id) const
{
	if (attr_id < 0 || attr_id >= CloakParam::MAX_IMAGE_COUNT)
	{
		return NULL;
	}

	if (m_image_list[attr_id].image_id != attr_id)
	{
		return NULL;
	}

	return &m_image_list[attr_id];
}

const CloakSpecialImgCfg * CloakConfig::GetSpecialImgCfg(int special_img_id)
{
	if (special_img_id > 0 && special_img_id <= CloakParam::MAX_SPECIAL_IMAGE_ID)
	{
		if (m_special_img_cfglist[special_img_id].IsValid())
		{
			return &m_special_img_cfglist[special_img_id];
		}
	}

	return NULL;
}


const CloakSpecialImgUpgradeCfg * CloakConfig::GetSpecialImgUpgradeCfg(int special_img_id, int grade)
{
	CloakSpecialImgUpgradeCfgMap::iterator iter = m_special_img_upgrade_map.find(special_img_id);
	if (iter != m_special_img_upgrade_map.end())
	{
		CloakSpecialImgUpgradeCfgList &cfg_list = iter->second;

		if (grade < 0 || grade > CloakParam::MAX_UPGRADE_LIMIT || grade > iter->second.max_grade)
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

const CloakEquipConifg * CloakConfig::GetEquipCfg(int equip_idx)
{
	if (equip_idx >= 0 && equip_idx < CloakParam::EQUIP_COUNT)
	{
		return &m_equip_cfg_list[equip_idx];
	}

	return NULL;
}

const CloakSkillConifg * CloakConfig::GetSkillCfg(int skill_idx, int level)
{
	if (skill_idx >= 0 && skill_idx < CloakParam::SKILL_COUNT
		&& level > 0 && level <= CloakParam::SKILL_MAX_LEVEL
		&& m_skill_cfg_list[skill_idx][level].skill_level > 0)
	{
		return &m_skill_cfg_list[skill_idx][level];
	}

	return NULL;
}

const CloakUpLevelCfg * CloakConfig::GetUpLevelCfg(int star_level)
{
	if (star_level < 0 || star_level > CloakParam::MAX_LEVEL || m_up_level_cfg_list[star_level].level != star_level)
		return NULL;

	return &m_up_level_cfg_list[star_level];
}

const CloakUpLevelStuffCfg * CloakConfig::GetUpLevelStuffCfg(int stuff_index)
{
	if (stuff_index < 0 || stuff_index >= CloakParam::MAX_UP_LEVEL_ITEM_COUNT)
		return NULL;

	if (m_upstar_item_list[stuff_index].up_level_item_id <= 0)
	{
		return NULL;
	}

	return &m_upstar_item_list[stuff_index];
}

int CloakConfig::GetEquipSkillLevel(int min_equip_level)
{
	if (min_equip_level >= m_other_cfg.active_equip_skill_level && min_equip_level < CloakParam::EQUIP_MAX_LEVEL)
	{
		return m_equip_skill_level_cfg[min_equip_level];
	}

	return -1;
}




