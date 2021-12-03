#include "chinesezodiacconfig.hpp"
#include "obj/character/role.h"
#include "item/itemextern.hpp"
#include "world.h"
#include "item/itempool.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>
#include <servercommon/configcommon.h>

ChineseZodiacConfig::ChineseZodiacConfig() : m_single_attr_count(0), m_suit_attr_count(0), m_xinghun_extra_count(0), m_total_pro(0), m_miji_count(0), m_miji_type_count(0)
{

}

ChineseZodiacConfig::~ChineseZodiacConfig()
{

}

bool ChineseZodiacConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ChineseZodiacConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 其他配置信息
		PugiXmlNode other_element = RootElement.child("other");
		if (other_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(other_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 单个生肖属性
		PugiXmlNode single_info_element = RootElement.child("single_info");
		if (single_info_element.empty())
		{
			*err = configname + ": no [single_info].";
			return false;
		}

		iRet = this->InitSingleAttrCfg(single_info_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitSingleAttrCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 套装生肖属性
		PugiXmlNode suit_info_element = RootElement.child("suit_info");
		if (suit_info_element.empty())
		{
			*err = configname + ": no [suit_info].";
			return false;
		}

		iRet = this->InitSuitAttrCfg(suit_info_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitSuitAttrCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 生肖装备属性
		PugiXmlNode equip_element = RootElement.child("equip_suit");
		if (equip_element.empty())
		{
			*err = configname + ": no [equip_suit].";
			return false;
		}

		//iRet = this->InitEquipSuitAttrCfg(equip_element);
		iRet = this->InitNewEquipSuitSuitCfg(equip_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitEquipSuitAttrCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 星魂
		PugiXmlNode xinghun_element = RootElement.child("xinghun");
		if (xinghun_element.empty())
		{
			*err = configname + ": no [xinghun].";
			return false;
		}

		iRet = this->InitChineseZodiacXingHunCfg(xinghun_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitChineseZodiacXingHunCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 星魂等级额外属性
		PugiXmlNode xinghun_extra_info_element = RootElement.child("xinghun_extra_info");
		if (xinghun_extra_info_element.empty())
		{
			*err = configname + ": no [xinghun_extra_info].";
			return false;
		}

		iRet = this->InitChineseZodiacXingHunExtraAttrCfg(xinghun_extra_info_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitChineseZodiacXingHunExtraAttrCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode xinghun_extra_info_element = RootElement.child("bead");
		if (xinghun_extra_info_element.empty())
		{
			*err = configname + ": no [bead].";
			return false;
		}

		iRet = this->InitBeadCfg(xinghun_extra_info_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBeadCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode xinghun_extra_info_element = RootElement.child("combine");
		if (xinghun_extra_info_element.empty())
		{
			*err = configname + ": no [combine].";
			return false;
		}

		iRet = this->InitCombineCfg(xinghun_extra_info_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitCombineCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode xinghun_extra_info_element = RootElement.child("combine_attr");
		if (xinghun_extra_info_element.empty())
		{
			*err = configname + ": no [combine_attr].";
			return false;
		}

		iRet = this->InitCombineAttrCfg(xinghun_extra_info_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitCombineAttrCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode xinghun_extra_info_element = RootElement.child("level_limit_cfg");
		if (xinghun_extra_info_element.empty())
		{
			*err = configname + ": no [level_limit_cfg].";
			return false;
		}

		iRet = this->InitLevelLimitCfg(xinghun_extra_info_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitLevelLimitCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode xinghun_extra_info_element = RootElement.child("funny_trun_combine");
		if (xinghun_extra_info_element.empty())
		{
			*err = configname + ": no [funny_trun_combine].";
			return false;
		}

		iRet = this->InitGglCombineTypeCfg(xinghun_extra_info_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitGglCombineTypeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

//	{
// 		PugiXmlNode xinghun_extra_info_element = RootElement.child("funny_trun_type_pro");
// 		if (xinghun_extra_info_element.empty())
// 		{
// 			*err = configname + ": no [funny_trun_type_pro].";
// 			return false;
// 		}
// 
// 		iRet = this->InitGglCombineItemCfg(xinghun_extra_info_element);
// 		if (iRet)
// 		{
// 			sprintf(errinfo,"%s: InitGglCombineItemCfg failed %d", configname.c_str(), iRet);
// 			*err = errinfo;
// 			return false;
// 		}
//	}

	{
		PugiXmlNode xinghun_extra_info_element = RootElement.child("bead_chapter_attr");
		if (xinghun_extra_info_element.empty())
		{
			*err = configname + ": no [bead_chapter_attr].";
			return false;
		}

		iRet = this->InitChapterAttrCfg(xinghun_extra_info_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitChapterAttrCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	//{
	//	PugiXmlNode xinghun_extra_info_element = RootElement.child("miji_cfg");
	//	if (xinghun_extra_info_element.empty())
	//	{
	//		*err = configname + ": no [miji_cfg].";
	//		return false;
	//	}

	//	iRet = this->InitMijiCfg(xinghun_extra_info_element);
	//	if (iRet)
	//	{
	//		sprintf(errinfo,"%s: InitMijiCfg failed %d", configname.c_str(), iRet);
	//		*err = errinfo;
	//		return false;
	//	}
	//}

	//{
	//	PugiXmlNode xinghun_extra_info_element = RootElement.child("miji_open_limit");
	//	if (xinghun_extra_info_element.empty())
	//	{
	//		*err = configname + ": no [miji_open_limit].";
	//		return false;
	//	}

	//	iRet = this->InitMijiOpenLimitCfg(xinghun_extra_info_element);
	//	if (iRet)
	//	{
	//		sprintf(errinfo,"%s: InitMijiOpenLimitCfg failed %d", configname.c_str(), iRet);
	//		*err = errinfo;
	//		return false;
	//	}
	//}

	{
		PugiXmlNode xinghun_extra_info_element = RootElement.child("miji_kong_open_limit");
		if (xinghun_extra_info_element.empty())
		{
			*err = configname + ": no [miji_kong_open_limit].";
			return false;
		}

		//iRet = this->InitMijiKongOpenLimitCfg(xinghun_extra_info_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMijiKongOpenLimitCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode xinghun_extra_info_element = RootElement.child("equip_broadcast");
		if (xinghun_extra_info_element.empty())
		{
			*err = configname + ": no [equip_broadcast].";
			return false;
		}

		iRet = this->InitBroadcastCfg(xinghun_extra_info_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBroadcastCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode xinghun_extra_info_element = RootElement.child("xingling");
		if (xinghun_extra_info_element.empty())
		{
			*err = configname + ": no [xingling].";
			return false;
		}

		iRet = this->InitXinglingCfg(xinghun_extra_info_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitXinglingCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode xinghun_extra_info_element = RootElement.child("shengxiao_color");
		if (xinghun_extra_info_element.empty())
		{
			*err = configname + ": no [shengxiao_color].";
			return false;
		}

		iRet = this->InitChineseZodiacColorCfg(xinghun_extra_info_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitChineseZodiacColorCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int ChineseZodiacConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "limit_level", m_other_cfg.limit_level) || m_other_cfg.limit_level < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "change_bead_type_need_gold", m_other_cfg.change_bead_type_need_gold) || m_other_cfg.change_bead_type_need_gold < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "ggl_day_bind_gold_time", m_other_cfg.ggl_day_bind_gold_time) || m_other_cfg.ggl_day_bind_gold_time < 0)
	{
		return -4;
	}

	PugiXmlNode Itemelement = data_element.child("comfort_reward");
	ItemID itemid = 0;
	if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid) )
	{
		if (!m_other_cfg.ggl_comfort_reward.ReadConfig(Itemelement))
		{
			return -5;
		}

	}else
	{	
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "ggl_consume_gold", m_other_cfg.ggl_consume_gold) || m_other_cfg.ggl_consume_gold < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "miji_compound_consume_gold1", m_other_cfg.miji_compound_consume_gold1) || m_other_cfg.miji_compound_consume_gold1 < 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "miji_compound_consume_gold2", m_other_cfg.miji_compound_consume_gold2) || m_other_cfg.miji_compound_consume_gold2 < 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(data_element, "miji_compound_consume_gold3", m_other_cfg.miji_compound_consume_gold3) || m_other_cfg.miji_compound_consume_gold3 < 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(data_element, "ggl_free_times", m_other_cfg.ggl_free_times) || m_other_cfg.ggl_free_times < 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "ggl_free_times_cd", m_other_cfg.ggl_free_times_cd) || m_other_cfg.ggl_free_times_cd < 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(data_element, "ggl_ten_consume_gold", m_other_cfg.ggl_ten_consume_gold) || m_other_cfg.ggl_ten_consume_gold < 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(data_element, "equiment_recycle", m_other_cfg.equiment_recycle) || m_other_cfg.equiment_recycle < 0 ||NULL == ITEMPOOL->GetItem(m_other_cfg.equiment_recycle))
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(data_element, "replacement_id", m_other_cfg.replacement_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.replacement_id))
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(data_element, "consume_replacement", m_other_cfg.consume_replacement) || m_other_cfg.consume_replacement < 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(data_element, "open_box_30_use_itemid", m_other_cfg.ten_chou_use_itemid) || NULL == ITEMPOOL->GetItem(m_other_cfg.ten_chou_use_itemid))
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(data_element, "open_box_30_use_item_num", m_other_cfg.ten_chou_use_item_num) || m_other_cfg.ten_chou_use_item_num < 0)
	{
		return -17;
	}

	return 0;
}

int ChineseZodiacConfig::InitSingleAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int soul_type = -1;
		int level = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", soul_type) || soul_type < 0 || soul_type >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0 || level > CHINESE_ZODIAC_LEVEL_MAX_LIMIT)
		{
			return -2;
		}

		ChineseZodiacSingleAttr::SingleType &single_cfg = m_single_attr_list[soul_type].single_type_vec[level];
		single_cfg.soul_type = soul_type;
		single_cfg.level = level;

		if (!PugiGetSubNodeValue(data_element, "color", single_cfg.color) || single_cfg.color < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "level_limit", single_cfg.level_limit) || single_cfg.level_limit < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", single_cfg.maxhp) || single_cfg.maxhp < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", single_cfg.gongji) || single_cfg.gongji < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", single_cfg.fangyu) || single_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", single_cfg.mingzhong) || single_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", single_cfg.shanbi) || single_cfg.shanbi < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", single_cfg.baoji) || single_cfg.baoji < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", single_cfg.jianren) || single_cfg.jianren < 0)
		{
			return -107;
		}

		if (!PugiGetSubNodeValue(data_element, "item_id", single_cfg.item_id) || single_cfg.item_id < 0)
		{
			return -108;
		}

		if (!PugiGetSubNodeValue(data_element, "expend", single_cfg.expend) || single_cfg.expend < 0)
		{
			return -109;
		}

		std::string name;
		if (!PugiGetSubNodeValue(data_element, "name", name))
		{
			return -110;
		}
		if (name.length() >= sizeof(single_cfg.name))
		{
			return -111;
		}
		memcpy(single_cfg.name, name.c_str(), (int)name.length());

		data_element = data_element.next_sibling();
	}
	
	return 0;
}

int ChineseZodiacConfig::InitSuitAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_suit_attr_count = 0;

	while (!data_element.empty())
	{
		int level = -1;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0)
		{
			return -1;
		}

		ChineseZodiacSuitAttr &suit_cfg = m_suit_attr_list[m_suit_attr_count];
		suit_cfg.level = level;

		if (!PugiGetSubNodeValue(data_element, "maxhp", suit_cfg.maxhp) || suit_cfg.maxhp < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", suit_cfg.gongji) || suit_cfg.gongji < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", suit_cfg.fangyu) || suit_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", suit_cfg.mingzhong) || suit_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", suit_cfg.shanbi) || suit_cfg.shanbi < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", suit_cfg.baoji) || suit_cfg.baoji < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", suit_cfg.jianren) || suit_cfg.jianren < 0)
		{
			return -107;
		}

		++ m_suit_attr_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChineseZodiacConfig::InitEquipAttrCfg(PugiXmlNode RootElement)
{
	return 0;
// 	PugiXmlNode data_element = RootElement.child("data");
// 	if (data_element.empty())
// 	{
// 		return -10000;
// 	}
// 
// 	int quality = 0;
// 
// 	while (!data_element.empty())
// 	{
// 		ItemID item_id = 0;
// 		if (!PugiGetSubNodeValue(data_element, "consume_stuff_id", item_id) || item_id <= 0)
// 		{
// 			return -1;
// 		}
// 		const ItemBase *item = ITEMPOOL->GetItem(item_id);
// 		if (NULL == item)
// 		{
// 			return -2;
// 		}
// 		ChineseZodiacEquipAttr equip_cfg;
// 		if (!PugiGetSubNodeValue(data_element, "quality", equip_cfg.quality) || equip_cfg.quality != item->GetColor())
// 		{
// 			return -3;
// 		}
// 
// 		if (!PugiGetSubNodeValue(data_element, "equip_type", equip_cfg.equip_type) || equip_cfg.equip_type < 0 || equip_cfg.equip_type >= CHINESE_ZODIAC_CUR_EQUIP_SLOT_MAX_LIMIT)
// 		{
// 			return -1;
// 		}
// 	
// 		if (!PugiGetSubNodeValue(data_element, "zodiac_level", equip_cfg.zodiac_level) || equip_cfg.zodiac_level < 0 || equip_cfg.zodiac_level > CHINESE_ZODIAC_LEVEL_MAX_LIMIT)
// 		{
// 			return -3;
// 		}
// 	
// 		if (!PugiGetSubNodeValue(data_element, "maxhp", equip_cfg.maxhp) || equip_cfg.maxhp < 0)
// 		{
// 			return -101;
// 		}
// 
// 		if (!PugiGetSubNodeValue(data_element, "gongji", equip_cfg.gongji) || equip_cfg.gongji < 0)
// 		{
// 			return -102;
// 		}
// 
// 		if (!PugiGetSubNodeValue(data_element, "fangyu", equip_cfg.fangyu) || equip_cfg.fangyu < 0)
// 		{
// 			return -103;
// 		}
// 
// 		if (!PugiGetSubNodeValue(data_element, "mingzhong", equip_cfg.mingzhong) || equip_cfg.mingzhong < 0)
// 		{
// 			return -104;
// 		}
// 
// 		if (!PugiGetSubNodeValue(data_element, "shanbi", equip_cfg.shanbi) || equip_cfg.shanbi < 0)
// 		{
// 			return -105;
// 		}
// 
// 		if (!PugiGetSubNodeValue(data_element, "baoji", equip_cfg.baoji) || equip_cfg.baoji < 0)
// 		{
// 			return -106;
// 		}
// 
// 		if (!PugiGetSubNodeValue(data_element, "jianren", equip_cfg.jianren) || equip_cfg.jianren < 0)
// 		{
// 			return -107;
// 		}
// 
// 		data_element = data_element.next_sibling();
// 	}
// 
// 	return 0;
}

int ChineseZodiacConfig::InitEquipSuitAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int last_suit = 0;
	while (!data_element.empty())
	{
		int zidoac = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", zidoac) || zidoac < 0 || zidoac >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
		{
			return -1;
		}
		int suit_id = -1;
		if (!PugiGetSubNodeValue(data_element, "suit_id", suit_id) || suit_id < 0 ||suit_id >= CHINESE_ZODIAC_MAX_EQUIP_SUIT)
		{
			return -2;
		}
		if (suit_id != last_suit + 1 && suit_id != 0)
		{
			return -3;
		}
		last_suit = suit_id;
		ChineseZodiacEquipSuitAttr & suit_cfg = m_equip_suit_attr_list[zidoac][suit_id];
		suit_cfg.suit_id = suit_id;

		if (!PugiGetSubNodeValue(data_element, "suit_color", suit_cfg.suit_color) || suit_cfg.suit_color <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", suit_cfg.maxhp) || suit_cfg.maxhp < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", suit_cfg.gongji) || suit_cfg.gongji < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", suit_cfg.fangyu) || suit_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", suit_cfg.mingzhong) || suit_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", suit_cfg.shanbi) || suit_cfg.shanbi < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", suit_cfg.baoji) || suit_cfg.baoji < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", suit_cfg.jianren) || suit_cfg.jianren < 0)
		{
			return -107;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChineseZodiacConfig::InitChineseZodiacXingHunExtraAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_xinghun_extra_count = 0;

	while (!data_element.empty())
	{
		int level = -1;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level <= 0)
		{
			return -1;
		}

		if (m_xinghun_extra_count < 0 || m_xinghun_extra_count >= CHINESE_ZODIAC_XINGHUN_TITLE_COUNT_MAX)
		{
			return -2;
		}

		ChineseZodiacXingHunExtraAttrCfg &xinghun_extra_cfg = m_xinghun_attr_list[m_xinghun_extra_count];
		xinghun_extra_cfg.level = level;

		if (!PugiGetSubNodeValue(data_element, "maxhp", xinghun_extra_cfg.maxhp) || xinghun_extra_cfg.maxhp < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", xinghun_extra_cfg.gongji) || xinghun_extra_cfg.gongji < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", xinghun_extra_cfg.fangyu) || xinghun_extra_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", xinghun_extra_cfg.mingzhong) || xinghun_extra_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", xinghun_extra_cfg.shanbi) || xinghun_extra_cfg.shanbi < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", xinghun_extra_cfg.baoji) || xinghun_extra_cfg.baoji < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", xinghun_extra_cfg.jianren) || xinghun_extra_cfg.jianren < 0)
		{
			return -107;
		}

		++ m_xinghun_extra_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChineseZodiacConfig::InitChineseZodiacXingHunCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_seq = -1;
	int last_xinghun_level = CHINESE_ZODIAC_XINGHUN_LEVEL_MAX;
	int protect_level_temp = 0;

	while (!data_element.empty())
	{
		ChineseZodiacXingHunCfg xinghun_cfg;

		if (!PugiGetSubNodeValue(data_element, "seq", xinghun_cfg.seq) || xinghun_cfg.seq < 0 || xinghun_cfg.seq >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "level", xinghun_cfg.level) || xinghun_cfg.level < 0 || xinghun_cfg.level > CHINESE_ZODIAC_XINGHUN_LEVEL_MAX)
		{
			return -2;
		}

		if (xinghun_cfg.seq == last_seq && xinghun_cfg.level != last_xinghun_level + 1)
		{
			return -1001;
		}

		if (xinghun_cfg.seq != last_seq)
		{
			if (xinghun_cfg.level != 0 || xinghun_cfg.seq != last_seq + 1)
			{
				return -1003;
			}
		}

		last_seq = xinghun_cfg.seq;
		last_xinghun_level = xinghun_cfg.level;

		if (!PugiGetSubNodeValue(data_element, "consume_stuff_id", xinghun_cfg.consume_stuff_id) || NULL == ITEMPOOL->GetItem(xinghun_cfg.consume_stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "consume_stuff_num", xinghun_cfg.consume_stuff_num) || xinghun_cfg.consume_stuff_num <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "succ_percent", xinghun_cfg.succ_percent) || xinghun_cfg.succ_percent < 0 || xinghun_cfg.succ_percent > 100)
		{
			return -5;
		}

		protect_level_temp = 0;
		if (!PugiGetSubNodeValue(data_element, "is_protect_level", protect_level_temp) || protect_level_temp < 0 || protect_level_temp > 1)
		{
			return -6;
		}
		xinghun_cfg.is_protect_level = (0 != protect_level_temp);

		if (!PugiGetSubNodeValue(data_element, "backwards_highest_level", xinghun_cfg.backwards_highest_level) || xinghun_cfg.backwards_highest_level < 0 || xinghun_cfg.backwards_highest_level > CHINESE_ZODIAC_XINGHUN_LEVEL_MAX)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "max_baoji_value", xinghun_cfg.max_baoji_value) || xinghun_cfg.max_baoji_value < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "faild_add_baoji_value", xinghun_cfg.faild_add_baoji_value) || xinghun_cfg.faild_add_baoji_value < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji_value_change_value", xinghun_cfg.baoji_value_change_value) || xinghun_cfg.baoji_value_change_value < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "mount_add", xinghun_cfg.mount_add) || xinghun_cfg.mount_add < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "wing_add", xinghun_cfg.wing_add) || xinghun_cfg.wing_add < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "halo_add", xinghun_cfg.halo_add) || xinghun_cfg.halo_add < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(data_element, "shengong_add", xinghun_cfg.shengong_add) || xinghun_cfg.shengong_add < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(data_element, "shenyi_add", xinghun_cfg.shenyi_add) || xinghun_cfg.shenyi_add < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(data_element, "footprint_add", xinghun_cfg.footprint_add) || xinghun_cfg.footprint_add < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(data_element, "fightmount_add", xinghun_cfg.fightmount_add) || xinghun_cfg.fightmount_add < 0)
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(data_element, "protect_item_id", xinghun_cfg.protect_item_id) || NULL == ITEMPOOL->GetItem(xinghun_cfg.protect_item_id))
		{
			return -18;
		}

		if (!PugiGetSubNodeValue(data_element, "protect_item_num", xinghun_cfg.protect_item_num) || xinghun_cfg.protect_item_num <= 0)
		{
			return -19;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", xinghun_cfg.maxhp) || xinghun_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", xinghun_cfg.gongji) || xinghun_cfg.gongji < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", xinghun_cfg.fangyu) || xinghun_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", xinghun_cfg.mingzhong) || xinghun_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", xinghun_cfg.shanbi) || xinghun_cfg.shanbi < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", xinghun_cfg.baoji) || xinghun_cfg.baoji < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", xinghun_cfg.jianren) || xinghun_cfg.jianren < 0)
		{
			return -106;
		}

		long long key = ConvertParamToLongLong(xinghun_cfg.seq, xinghun_cfg.level);
		if (m_xinghun_level_map.find(key) != m_xinghun_level_map.end())
		{
			return -1000;
		}

		m_xinghun_level_map[key] = xinghun_cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChineseZodiacConfig::InitBroadcastCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int xingzuo_level = 0;
		if (!PugiGetSubNodeValue(data_element, "xingzuo_level", xingzuo_level) || xingzuo_level <= 0)
		{
			return -10001;
		}

		bool is_broadcast;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast))
		{
			return -10002;
		}

		m_is_broadcast_map[xingzuo_level] = is_broadcast;

		data_element = data_element.next_sibling();
	}

	return 0;
}

const ChineseZodiacSingleAttr::SingleType * ChineseZodiacConfig::GetSingleAttr(int soul_type, int level)
{
	if (soul_type < 0 || soul_type >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT || level < 0 || level > CHINESE_ZODIAC_LEVEL_MAX_LIMIT)
	{
		return  NULL;
	}

	return &m_single_attr_list[soul_type].single_type_vec[level];
}

const ChineseZodiacSuitAttr * ChineseZodiacConfig::GetSuitAttr(int level)
{
	if (level < 0)
	{
		return NULL;
	}

	for(int i = m_suit_attr_count - 1; i >= 0; -- i)
	{
		if (level >= m_suit_attr_list[i].level)
		{
			return &m_suit_attr_list[i];
		}
	}

	return NULL;
}

// const ChineseZodiacEquipAttr * ChineseZodiacConfig::GetEquigpAttrByColor(int color, int equip_type, int level)
// {
// 	if (color < 0 || color >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT) return NULL;
// 
// 	if (equip_type < 0 || equip_type >= CHINESE_ZODIAC_CUR_EQUIP_SLOT_MAX_LIMIT) return  NULL;
// 
// 	if (level <= 0 || level > CHINESE_ZODIAC_MAX_EQUIP_LEVEL) return  NULL;
// 
// 	if (m_equip_attr_list[color][equip_type][level].level != level) return NULL;
// 
// 	return &m_equip_attr_list[color][equip_type][level];
// }

const ChineseZodiacEquipSuitAttr * ChineseZodiacConfig::GetEquipSuitAttr(int zodiac_type, int color) const
{
	if (color <= 0 || zodiac_type < 0 || zodiac_type >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
	{
		return nullptr;
	}
	for (int i = 0; i < CHINESE_ZODIAC_MAX_EQUIP_SUIT; i++)
	{
		if (color == m_equip_suit_attr_list[zodiac_type][i].suit_color)
		{
			return &m_equip_suit_attr_list[zodiac_type][i];
		}
	}

	return nullptr;
}

const ChineseZodiacXingHunCfg * ChineseZodiacConfig::GetChineseZodiacXingHunCfg(int zodiac_type, int xinghun_level)
{
	if (zodiac_type < 0 || zodiac_type >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
	{
		return NULL;
	}

	if (xinghun_level < 0 || xinghun_level > CHINESE_ZODIAC_XINGHUN_LEVEL_MAX)
	{
		return NULL;
	}

	long long key = ConvertParamToLongLong(zodiac_type, xinghun_level);

	std::map<long long, ChineseZodiacXingHunCfg>::iterator it = m_xinghun_level_map.find(key);
	if (it != m_xinghun_level_map.end() && it->second.level == xinghun_level)
	{
		return &it->second;
	}

	return NULL;
}

const ChineseZodiacXingHunExtraAttrCfg * ChineseZodiacConfig::GetChineseZodiacXingHunAttrCfg(int xinghun_level)
{
	if (xinghun_level < 0 || xinghun_level > CHINESE_ZODIAC_XINGHUN_LEVEL_MAX)
	{
		return NULL;
	}

	if (m_xinghun_extra_count <= 0)
	{
		return NULL;
	}

	for(int i = m_xinghun_extra_count - 1; i >= 0; -- i)
	{
		if (xinghun_level >= m_xinghun_attr_list[i].level)
		{
			return &m_xinghun_attr_list[i];
		}
	}

	return NULL;
}

const TianXiangBeadCfg * ChineseZodiacConfig::GetBeadCfg(short type) const
{
	if (type <= TIAN_XIANG_BEAD_TYPE_INVALID  || type >= TIAN_XIANG_BEAD_TYPE_MAX)
	{
		return NULL;
	}

	if (m_bead_cfg[type].type == type)
	{
		return &m_bead_cfg[type];
	}

	return NULL;
}

const TianXiangCombineSeqCfg * ChineseZodiacConfig::GetCombineSeqCfg(int seq) const
{
	if (seq < 0 || seq >= TIAN_XIANG_ALL_CHAPTER_COMBINE * TIAN_XIANG_CURR_CHAPTER_NUM)
	{
		return NULL;
	}

	const TianXiangCombineSeqCfg &cfg = m_bead_combine_cfg_list[seq];
	if (cfg.bead_list[0].type <= TIAN_XIANG_BEAD_TYPE_INVALID)
	{
		return NULL;
	}

	if (cfg.seq == seq)
	{
		return &cfg;
	}

	return NULL;
}

const TianXiangCombineAttrCfg * ChineseZodiacConfig::GetCombineAttrCfg(int seq) const
{
	if (seq < 0 || seq >= m_bead_combine_attr_cfg_count)
	{
		return NULL;
	}

	if (m_bead_combine_attr_cfg_list[seq].seq == seq)
	{
		return &m_bead_combine_attr_cfg_list[seq];
	}

	return NULL;
}

const TianXiangLevelLimitCfg * ChineseZodiacConfig::GetLevelLimitCfg(int level) const
{
	for (int i = m_level_limit_cfg_count - 1; i >= 0 ; -- i )
	{
		if (level >= m_level_limit_cfg_list[i].limit_level )
		{
			return &m_level_limit_cfg_list[i];
		}
	}

	return NULL;
}

int ChineseZodiacConfig::InitBeadCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_bead_cfg_count = 1;
	while (!data_element.empty())
	{
		if (m_bead_cfg_count >= TIAN_XIANG_BEAD_TYPE_MAX)
		{
			return -1001;
		}

		int type = 0;
		if (!PugiGetSubNodeValue(data_element, "type", type) || type != m_bead_cfg_count)
		{
			return -1002;
		}

		TianXiangBeadCfg &cfg = m_bead_cfg[type];
		cfg.type = type;
		int iRet = cfg.attr.ReadConfig(data_element);
		if (iRet < 0)
		{
			return iRet;
		}

		if (!PugiGetSubNodeValue(data_element, "item_id", cfg.item_id) || NULL == ITEMPOOL->GetItem(cfg.item_id))
		{
			return -1003;
		}

		++m_bead_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChineseZodiacConfig::InitCombineAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_bead_combine_attr_cfg_count = 0;
	while (!data_element.empty())
	{
		if (m_bead_combine_attr_cfg_count >= TIAN_XIANG_ALL_CHAPTER_COMBINE * TIAN_XIANG_CURR_CHAPTER_NUM)
		{
			return -1001;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_bead_combine_attr_cfg_count)
		{
			return -1002;
		}

		TianXiangCombineAttrCfg &cfg = m_bead_combine_attr_cfg_list[seq];
		cfg.seq = seq;
		int iRet = cfg.attr.ReadConfig(data_element);
		if (iRet < 0)
		{
			return iRet;
		}

		std::string name;
		if (!PugiGetSubNodeValue(data_element, "name", name))
		{
			return -1003;
		}
		if (name.length() >= sizeof(cfg.name))
		{
			return -1004;
		}
		memcpy(cfg.name, name.c_str(), (int)name.length());

		++m_bead_combine_attr_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChineseZodiacConfig::InitChapterAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int count = 0;
	while (!data_element.empty())
	{
		if (count >= TIAN_XIANG_CURR_CHAPTER_NUM)
		{
			return -1001;
		}

		int chapter = 0;
		if (!PugiGetSubNodeValue(data_element, "chapter", chapter) || chapter != count)
		{
			return -1002;
		}

		ChapterAttributesConfig &cfg = m_bead_chapter_attr_list[chapter];
		
		cfg.chapter = chapter;
		
		int iRet = cfg.attr.ReadConfig(data_element);
		if (iRet < 0)
		{
			return iRet;
		}

		if (!PugiGetSubNodeValue(data_element, "per_attr", cfg.per_attr) || cfg.per_attr < 0)
		{
			return -1003;
		}

		if (!PugiGetSubNodeValue(data_element, "per_jingzhun", cfg.per_jingzhun) || cfg.per_jingzhun < 0)
		{
			return -1006;
		}

		std::string name;
		if (!PugiGetSubNodeValue(data_element, "name", name))
		{
			return -1004;
		}
		if (name.length() >= sizeof(cfg.name))
		{
			return -1005;
		}
		memcpy(cfg.name, name.c_str(), (int)name.length());

		++count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChineseZodiacConfig::InitLevelLimitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_limit_level = 0;
	int last_bead_num = 0;
	m_level_limit_cfg_count = 0;
	while(!data_element.empty())
	{
		if (m_level_limit_cfg_count >= TIAN_XIANG_LEVEL_LIMIT_MAX_CFG)
		{
			return -1001;
		}

		TianXiangLevelLimitCfg &cfg = m_level_limit_cfg_list[m_level_limit_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "limit_level", cfg.limit_level) || cfg.limit_level < last_limit_level)
		{
			return -1002;
		}
		last_limit_level = cfg.limit_level;

		if (!PugiGetSubNodeValue(data_element, "bead_num", cfg.bead_num) || cfg.bead_num < last_bead_num)
		{
			return -1002;
		}
		last_bead_num = cfg.bead_num;

		++m_level_limit_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int ChineseZodiacConfig::InitCombineCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_bead_combine_cfg_count = 0;
	while (!data_element.empty())
	{
		if (m_bead_combine_cfg_count >= TIAN_XIANG_ALL_CHAPTER_COMBINE * TIAN_XIANG_CURR_CHAPTER_NUM)
		{
			return -1001;
		}

		int seq = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_bead_combine_cfg_count)
		{
			return -1205;
		}

		TianXiangCombineSeqCfg &cfg = m_bead_combine_cfg_list[seq];
		cfg.seq = seq;

		for (int index = 0; index < TIAN_XIANG_COMBINE_MEX_BEAD_NUM; ++ index)
		{
			TianXiangCombineItem &bead_cfg = cfg.bead_list[index];
			char name[30] = {};
			SNPRINTF(name, sizeof(name), "type_%d", index);
			if (!PugiGetSubNodeValue(data_element, name, bead_cfg.type) || bead_cfg.type < TIAN_XIANG_BEAD_TYPE_INVALID || bead_cfg.type > TIAN_XIANG_BEAD_TYPE_SAME)
			{
				return -1200;
			}

			memset(name, 0, sizeof(name));
			SNPRINTF(name, sizeof(name), "x_%d", index);
			if (!PugiGetSubNodeValue(data_element, name, bead_cfg.x) || bead_cfg.x < SCHAR_MIN || bead_cfg.x > SCHAR_MAX)
			{
				return -1201;
			}

			memset(name, 0, sizeof(name));
			SNPRINTF(name, sizeof(name), "y_%d", index);
			if (!PugiGetSubNodeValue(data_element, name, bead_cfg.y) || bead_cfg.y < SCHAR_MIN || bead_cfg.y > SCHAR_MAX)
			{
				return -1202;
			}
		}

		++m_bead_combine_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

bool ChineseZodiacConfig::CheckTypeIndex(int type_index)
{
	if (type_index <= GUNGUNLE_TYPE_INVALID || type_index >= GUNGUNLE_TYPE_MAX)
	{
		return false;
	}
	return true;
}

int ChineseZodiacConfig::InitGglCombineTypeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_total_pro = 0;
	while (!data_element.empty())
	{
		GglCombineType type_cfg;

		if (!PugiGetSubNodeValue(data_element, "type_index", type_cfg.type_index) || !CheckTypeIndex(type_cfg.type_index))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "combine_same_num", type_cfg.combine_same_num) || type_cfg.combine_same_num < 0)
		{
			return -2;
		}
		
		if (!PugiGetSubNodeValue(data_element, "type_pro", type_cfg.type_pro) || type_cfg.type_pro <= 0 || type_cfg.type_pro > 10000)
		{
			return -3;
		}

		m_total_pro += type_cfg.type_pro;

		int count = ItemConfigData::ReadConfigList(data_element, "reward_item", type_cfg.reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
		if(count < 0)
		{
			return -100 + count;
		}

		m_ggl_combine_type[type_cfg.type_index] = type_cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

// int ChineseZodiacConfig::InitGglCombineItemCfg(PugiXmlNode RootElement)
// {
// 	PugiXmlNode data_element = RootElement.child("data");
// 	if (data_element.empty())
// 	{
// 		return -10000;
// 	}
// 
// 	int last_type = -1;
// 	while (!data_element.empty())
// 	{
// 		int type = -1;
// 		int type_pro = -1;
// 		int ten_type_pro = -1;
// 		if (!PugiGetSubNodeValue(data_element, "type", type) || type < 0 || last_type + 1 != type)
// 		{
// 			return -1;
// 		}
// 
// 		if (!PugiGetSubNodeValue(data_element, "type_pro", type_pro) || type_pro < 0)
// 		{
// 			return -2;
// 		}
// 
// 		if (!PugiGetSubNodeValue(data_element, "ten_type_pro", ten_type_pro) || ten_type_pro < 0)
// 		{
// 			return -3;
// 		}
// 
// 		last_type = type;
// 
// 		m_total_pro += type_pro;
// 
// 		m_ggl_item_pro.push_back(type_pro);
// 
// 		m_total_ten_pro += ten_type_pro;
// 
// 		m_ggl_item_ten_pro.push_back(ten_type_pro);
// 
// 		data_element = data_element.next_sibling();
// 	}
// 
// 	return 0;
// }

int ChineseZodiacConfig::InitXinglingCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_id = 0;
	//int last_level = -1;

	while (!dataElement.empty())
	{
		int id = -1;
		if (!PugiGetSubNodeValue(dataElement, "id", id) || id < 0 || id >= TIAN_XIANG_CURR_CHAPTER_NUM)
		{
			return -1;
		}

		if (id == last_id + 1)
		{
			last_id = id;
			//last_level = -1;
		}

		int level = -1;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level < 0 || level >= XING_LING_MAX_LEVEL)
		{
			return -2;
		}
		//last_level = level;

		XingLingCfg::ConfigItem &cfg = m_xingling_cfg.cfg_list[id][level];
		if (0 != cfg.level)
		{
			return -100;
		}

		cfg.id = id;
		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "uplevel_stuff_id", cfg.uplevel_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.uplevel_stuff_id)) 
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(dataElement, "uplevel_stuff_num", cfg.uplevel_stuff_num) || cfg.uplevel_stuff_num < 0) 
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(dataElement, "bless_val_limit", cfg.bless_val_limit) || cfg.bless_val_limit < 0) 
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_prob", cfg.add_prob) || cfg.add_prob < 0) 
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "init_prob", cfg.init_prob) || cfg.init_prob < 0) 
		{
			return -5;
		}


		if (!PugiGetSubNodeValue(dataElement, "add_bless", cfg.add_bless) || cfg.add_bless < 0) 
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0) 
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0) 
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0) 
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0) 
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "kangbao", cfg.kangbao) || cfg.kangbao < 0) 
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0) 
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "xingtu_add_prob", cfg.xingtu_add_prob) || cfg.xingtu_add_prob < 0) 
		{
			return -14;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ChineseZodiacConfig::InitChineseZodiacColorCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int i = 0;

	while (!dataElement.empty())
	{
		if (i < 0 || i >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
		{
			return -100;
		}

		ChineseZodiacColorCfg &cfg = m_chinesezodiac_color_list[i];

		if (!PugiGetSubNodeValue(dataElement, "shengxiao_color", cfg.chinesezodiac_color) || cfg.chinesezodiac_color < 0)
		{
			return -1;
		}

		dataElement = dataElement.next_sibling();

		i++;
	}

	return 0;
}

int ChineseZodiacConfig::InitNewEquipSuitSuitCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_suit = 0;
	while (!data_element.empty())
	{
		int zidoac = -1;
		int suit_color = 0;
		if (!PugiGetSubNodeValue(data_element, "seq", zidoac) || zidoac < 0 || zidoac >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "suit_color", suit_color) || suit_color <= ItemBase::I_COLOR_INVALID || suit_color >= ItemBase::I_COLOR_MAX)
		{
			return -2;
		}

		int cfg_count = m_new_equip_suit_cfg[zidoac][suit_color].cfg_count;

		if (cfg_count < 0 || cfg_count >= CHINESE_ZODIAC_MAX_EQUIP_SUIT)
		{
			return -3;
		}

		int suit_id = -1;
		if (!PugiGetSubNodeValue(data_element, "suit_id", suit_id) || suit_id < 0 || suit_id >= CHINESE_ZODIAC_MAX_EQUIP_SUIT)
		{
			return -4;
		}
		if (suit_id != last_suit + 1 && suit_id != 0)
		{
			return -5;
		}
		last_suit = suit_id;

		ChineseZodiacEquipSuitAttr &suit_cfg = m_new_equip_suit_cfg[zidoac][suit_color].attr_cfg[cfg_count];

		suit_cfg.suit_id = suit_id;
		suit_cfg.suit_color = suit_color;

		if (!PugiGetSubNodeValue(data_element, "maxhp", suit_cfg.maxhp) || suit_cfg.maxhp < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", suit_cfg.gongji) || suit_cfg.gongji < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", suit_cfg.fangyu) || suit_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", suit_cfg.mingzhong) || suit_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", suit_cfg.shanbi) || suit_cfg.shanbi < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", suit_cfg.baoji) || suit_cfg.baoji < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", suit_cfg.jianren) || suit_cfg.jianren < 0)
		{
			return -107;
		}

		if (!PugiGetSubNodeValue(data_element, "need_count", suit_cfg.need_count) || suit_cfg.need_count < 0)
		{
			return -108;
		}

		if (!PugiGetSubNodeValue(data_element, "per_kangbao", suit_cfg.per_kangbao) || suit_cfg.per_kangbao < 0)
		{
			return -108;
		}

		if (!PugiGetSubNodeValue(data_element, "per_baoji_hurt", suit_cfg.per_baoji_hurt) || suit_cfg.per_baoji_hurt < 0)
		{
			return -109;
		}

		if (!PugiGetSubNodeValue(data_element, "per_baoji", suit_cfg.per_baoji) || suit_cfg.per_baoji < 0)
		{
			return -110;
		}

		++m_new_equip_suit_cfg[zidoac][suit_color].cfg_count;

		data_element = data_element.next_sibling();
	}

	return 0;
}

void ChineseZodiacConfig::GetGglResult(int &index,int & res_num)
{
	if (m_total_pro <= 0)
	{
		return;
	}
	int rand_pro = RandomNum(m_total_pro);
	
	for (auto  itr = m_ggl_combine_type.begin(); itr != m_ggl_combine_type.end(); ++itr)
	{
		if (rand_pro < itr->second.type_pro)
		{
			index = itr->second.type_index;
			break;
		}
		rand_pro -= itr->second.type_pro;
	}
	return;
}

// void ChineseZodiacConfig::GetGglTenResult(int &index, int &res_num)
// {
// 	int combine_item[GGL_COMBINE_ITEM_NUM];
// 
// 	for (int i = 0; i < GGL_COMBINE_ITEM_NUM; i++)
// 	{
// 		if (m_total_ten_pro <= 0)
// 		{
// 			return;
// 		}
// 
// 		int rand_item_pro = RandomNum(m_total_ten_pro);
// 
// 		for (int j = 0; j < static_cast<int>(m_ggl_item_ten_pro.size()); j++)
// 		{
// 			if (rand_item_pro < m_ggl_item_ten_pro[j])
// 			{
// 				combine_item[i] = j;
// 				res_num += (int)(j * pow(10.0, i));
// 				break;
// 			}
// 			rand_item_pro -= m_ggl_item_ten_pro[j];
// 		}
// 	}
// 
// 	int same_num = 0;
// 	int combine_type = -1;
// 
// 	if (combine_item[0] == combine_item[1] && combine_item[1] == combine_item[2])
// 	{
// 		same_num = 3;
// 		combine_type = combine_item[0];
// 	}
// 
// 	int result = this->GetGglResultCombineType(same_num, combine_type);
// 	if (-1 != combine_type && -1 != result)
// 	{
// 		index = result;
// 		return;
// 	}
// 
// 	if (combine_item[0] == combine_item[1])
// 	{
// 		same_num = 2;
// 		combine_type = combine_item[0];
// 	}
// 	else if (combine_item[0] == combine_item[2])
// 	{
// 		same_num = 2;
// 		combine_type = combine_item[0];
// 	}
// 	else if (combine_item[1] == combine_item[2])
// 	{
// 		same_num = 2;
// 		combine_type = combine_item[1];
// 	}
// 
// 	result = this->GetGglResultCombineType(same_num, combine_type);
// 	if (-1 != combine_type && -1 != result)
// 	{
// 		index = result;
// 		return;
// 	}
// 
// 	for (int i = 0; i < GGL_COMBINE_ITEM_NUM; i++)
// 	{
// 		result = this->GetGglResultCombineType(0, combine_item[i]);
// 		if (-1 != result)
// 		{
// 			index = result;
// 			return;
// 		}
// 	}
// 
// 	index = -1;
// }

// int ChineseZodiacConfig::GetGglResultCombineType(int same_num, int combine_type)
// {
// 	for (int i = 0; i < static_cast<int>(m_ggl_combine_type.size()); i++)
// 	{
// 		if (m_ggl_combine_type[i].combine_same_num == same_num)
// 		{
// 			for (int j = 0; j < static_cast<int>(m_ggl_combine_type[i].combine_type_vec.size()); j++)
// 			{
// 				if (atoi(m_ggl_combine_type[i].combine_type_vec[j].c_str()) == combine_type)
// 				{
// 					return i;
// 				}
// 			}
// 		}
// 	}
// 
// 	return -1;
// }

const GglCombineType *ChineseZodiacConfig::GetGglRewardCfg(int combine_type) 
{
	std::map<int, GglCombineType>::iterator itr = m_ggl_combine_type.find(combine_type);
	if (m_ggl_combine_type.end() != itr)
	{
		return &itr->second;
	}

	return NULL;
}

const ChapterAttributesConfig * ChineseZodiacConfig::GetChapterAttr(int chapter)
{
	if (chapter < 0 || chapter >= TIAN_XIANG_CURR_CHAPTER_NUM)
	{
		return  NULL;
	}

	return &m_bead_chapter_attr_list[chapter];
}

//int ChineseZodiacConfig::InitMijiCfg(PugiXmlNode RootElement)
//{
//	PugiXmlNode data_element = RootElement.child("data");
//	if (data_element.empty())
//	{
//		return -10000;
//	}
//	
//	int curr_type = -1;
//
//	while (!data_element.empty())
//	{
//		if (m_miji_count >= MIJI_KONG_KILL_MAX_COUNT)
//		{
//			return -1001;
//		}
//
//		ChineseZodiacMiji & cfg = m_miji_list[m_miji_count];
//
//		cfg.index = m_miji_count;
//
//		if (!PugiGetSubNodeValue(data_element, "type", cfg.type) || cfg.type < 0)
//		{
//			return -1;
//		}
//	
//		if (cfg.type != curr_type)
//		{
//			m_miji_type_count++;
//		}
//
//		curr_type = cfg.type;
//
//		if (!PugiGetSubNodeValue(data_element, "level", cfg.level) || cfg.level < 0)
//		{
//			return -2;
//		}
//
//		if (!PugiGetSubNodeValue(data_element, "value", cfg.value) || cfg.value < 0)
//		{
//			return -3;
//		}
//
//		if (!PugiGetSubNodeValue(data_element, "capacity", cfg.capacity) || cfg.capacity < 0)
//		{
//			return -4;
//		}
//
//		if (!PugiGetSubNodeValue(data_element, "item_id", cfg.item_id) || NULL == ITEMPOOL->GetItem(cfg.item_id))
//		{
//			return -5;
//		}
//
//		m_miji_count++;
//		data_element = data_element.next_sibling();
//	}
//
//	return 0;
//}

//int ChineseZodiacConfig::InitMijiOpenLimitCfg(PugiXmlNode RootElement)
//{
//	PugiXmlNode data_element = RootElement.child("data");
//	if (data_element.empty())
//	{
//		return -10000;
//	}
//
//	int index = 0;
//	while (!data_element.empty())
//	{
//		if (index >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
//		{
//			return 1001;
//		}
//
//		ChineseZodiacMijiOpenLimit &cfg = m_miji_open_limit_list[index];
//
//		if (!PugiGetSubNodeValue(data_element, "last_miji_num_limit", cfg.last_zodiac_miji_num_limit) || cfg.last_zodiac_miji_num_limit < 0)
//		{
//			return -1;
//		}
//
//		data_element = data_element.next_sibling();
//
//		index++;
//	}
//
//	return 0;
//}
//
//int ChineseZodiacConfig::InitMijiKongOpenLimitCfg(PugiXmlNode RootElement)
//{
//	PugiXmlNode data_element = RootElement.child("data");
//	if (data_element.empty())
//	{
//		return -10000;
//	}
//
//	int level_limit = 0;
//	int index = 0;
//	while (!data_element.empty())
//	{
//		if (index >= MIJI_KONG_NUM)
//		{
//			return 1001;
//		}
//
//		ChineseZodiacMijiKongOpenLimit &cfg = m_miji_kong_open_limit_list[index];
//
//		if (!PugiGetSubNodeValue(data_element, "zodiac_level_limit", cfg.zodiac_level_limit) || cfg.zodiac_level_limit < 0 || cfg.zodiac_level_limit < level_limit)
//		{
//			return -1;
//		}
//
//		level_limit = cfg.zodiac_level_limit;
//		index++;
//
//		data_element = data_element.next_sibling();
//	}
//
//	return 0;
//}
//
//const ChineseZodiacMiji * ChineseZodiacConfig::GetMijiCfgByIndex(int index)
//{
//	if (index < 0 || index >= m_miji_count) return NULL;
//
//	return &m_miji_list[index];
//}
//
//const ChineseZodiacMijiOpenLimit * ChineseZodiacConfig::GetMijiOpenLimit(int type)
//{
//	if (type < 0 || type >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
//	{
//		return NULL;
//	}
//	return &m_miji_open_limit_list[type];
//}
//
//int ChineseZodiacConfig::GetMijiKongOpenNumByZodiacLevel(int zodiac_level)
//{
//	if (zodiac_level < 0 || zodiac_level > CHINESE_ZODIAC_LEVEL_MAX_LIMIT)
//	{
//		return 0;
//	}
//	
//	for (int i = MIJI_KONG_NUM - 1; i >= 0; i--)
//	{
//		if (zodiac_level >= m_miji_kong_open_limit_list[i].zodiac_level_limit)
//		{
//			return i + 1;
//		}
//	}
//
//	return 0;
//}
//
//const ChineseZodiacMiji * ChineseZodiacConfig::GetMijiCfgByTypeAndLevel(int type, int level)
//{
//	for (int i = 0; i < m_miji_count; i++)
//	{
//		if (m_miji_list[i].type == type && m_miji_list[i].level == level)
//		{
//			return &m_miji_list[i];
//		}
//	}
//
//	return NULL;
//}
//
//const ChineseZodiacMiji * ChineseZodiacConfig::GetMijiCfgByItemid(int item_id)
//{
//	for (int i = 0; i < m_miji_count; i++)
//	{
//		if (m_miji_list[i].item_id == item_id)
//		{
//			return &m_miji_list[i];
//		}
//	}
//
//	return NULL;
//}

const XingLingCfg::ConfigItem * ChineseZodiacConfig::GetXinglingCfg(int id, int level)
{
	if (id < 0 || id >= TIAN_XIANG_CURR_CHAPTER_NUM || level < 0 || level >= XING_LING_MAX_LEVEL)
	{
		return NULL;
	}

	return &m_xingling_cfg.cfg_list[id][level];
}

int ChineseZodiacConfig::GetColorCfg(int zodiac_type)
{
	if (zodiac_type < 0 || zodiac_type >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT)
	{
		return -1;
	}
	
	return m_chinesezodiac_color_list[zodiac_type].chinesezodiac_color;
}

const ChineseZodiacEquipSuitAttr * ChineseZodiacConfig::GetNewSuitCfg(int zodiac_type, int color, int count)
{
	if (zodiac_type < 0 || zodiac_type >= CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT) return NULL;
	if (color <= ItemBase::I_COLOR_INVALID || color >= ItemBase::I_COLOR_MAX) return NULL;

	if (m_new_equip_suit_cfg[zodiac_type][color].cfg_count > 0)
	{
		for (int i = m_new_equip_suit_cfg[zodiac_type][color].cfg_count - 1; i >= 0 && i < CHINESE_ZODIAC_MAX_EQUIP_SUIT; i--)
		{
			if (count >= m_new_equip_suit_cfg[zodiac_type][color].attr_cfg[i].need_count)
			{
				return &m_new_equip_suit_cfg[zodiac_type][color].attr_cfg[i];
			}
		}
	}

	return NULL;
}