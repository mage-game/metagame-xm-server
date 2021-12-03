#include "itembase.h"
#include "itempool.h"
#include "item/itemfunction.h"
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/string/gameworldstr.h"
#include "equipment/suitbase.hpp"
#include "equipment/equipment.h"
#include "item/other/otheritem.hpp"
#include "item/gift/defgift.hpp"
#include "item/gift/randgift.hpp"
#include "item/gift/selectgift.hpp"
#include "item/expense/norexitem.hpp"
#include "checkresourcecenter.hpp"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "gamelog.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define LOADEQUIPMENT(NODENAME, CLASSNAME, EQUIPMENTTYPE) \
	currentElement = RootElement.child(NODENAME);\
	if (!currentElement || !currentElement.first_child())\
	{\
		*err = configname + ": xml node error in getting node [" NODENAME "]. ";\
		return false;\
	}\
	else\
	{\
		PugiXmlNode subElement = currentElement.child("path");\
		while(subElement)\
		{\
			ItemBase* item = new CLASSNAME(EQUIPMENTTYPE);\
			std::string path;\
			\
			if (!PugiGetNodeValue(subElement,path))\
			{\
				*err = configname + ": xml node error in node [itemmanager->" NODENAME "->path] ";\
				delete item;\
				Release();\
				return false;\
			}\
			\
			if (!item->Init((dir+path).c_str(), err))\
			{\
				delete item;\
				Release();\
				return false;\
			}\
			\
			if (m_item_pool[item->GetItemId()] != 0)\
			{\
				sprintf(errinfo,"load path: %s error: item id[%d] repeat", path.c_str(),item->GetItemId());\
				*err = errinfo;\
				delete item;\
				Release();\
				return false;\
			}\
			m_item_pool[item->GetItemId()] = item;\
			subElement = subElement.next_sibling();\
		}\
	}

//-------------------------------------------------------------------------------------------
#define LOADITEM(NODENAME, CLASSNAME) \
	currentElement = RootElement.child(NODENAME);\
	if (!currentElement)\
	{\
		*err = configname + ": xml node error in getting node [" NODENAME "]. ";\
		return false;\
	}\
	else\
	{\
		PugiXmlNode subElement = currentElement.child("path");\
		while(subElement)\
		{\
			ItemBase* item = new CLASSNAME();\
			std::string path;\
			\
			if (!PugiGetNodeValue(subElement,path))\
			{\
				*err = configname + ": xml node error in node [itemmanager->" NODENAME "->path] ";\
				delete item;\
				Release();\
				return false;\
			}\
			\
			if (!item->Init((dir+path).c_str(), err))\
			{\
				delete item;\
				Release();\
				return false;\
			}\
			\
			if (m_item_pool[item->GetItemId()] != 0)\
			{\
				sprintf(errinfo,"load path: %s error: item id[%d] repeat", path.c_str(),item->GetItemId());\
				*err = errinfo;\
				delete item;\
				Release();\
				return false;\
			}\
			m_item_pool[item->GetItemId()] = item;\
			subElement = subElement.next_sibling();\
		}\
	}

//-------------------------------------------------------------------------------------------
#define LOADCONFIG \
	PugiXmlDocument document;\
	char errinfo[1024];\
	PUGI_XML_LOAD_AND_CHECK(document, std::string(path), "Item Config", *err)\
	\
	PugiXmlNode RootElement = document.document_element();\
	\
	if (!RootElement )\
	{\
		sprintf(errinfo,"%s: xml node error in root.", path);\
		*err = errinfo;\
		return false;\
	}

//-------------------------------------------------------------------------------------------
#define GETNADEVALUE(NODENAME, VALUENAME) \
	if (!PugiGetSubNodeValue(RootElement, NODENAME, VALUENAME) )\
	{\
		sprintf(errinfo,"%s: xml node error in node [%s].", path, NODENAME);\
		*err = errinfo;\
		return false;\
	}

//-------------------------------------------------------------------------------------------
#define LOADITEMBASEATTR \
	GETNADEVALUE("id", m_item_id);\
	GETNADEVALUE("search_type", m_search_type);\
	GETNADEVALUE("color", m_color);\
	if (m_color <= 0 || m_color >= ItemBase::I_COLOR_MAX)\
	{\
		sprintf(errinfo,"%s: xml node error in node [color] value.", path);\
		*err = errinfo;\
		return false;\
	}\
	GETNADEVALUE("isbind", m_is_bind);\
	GETNADEVALUE("candiscard", m_can_discard);\
	GETNADEVALUE("cansell", m_can_sell);\
	GETNADEVALUE("isbroadcast", m_is_broadcast);\
	GETNADEVALUE("sellprice", m_sell_price);\
	GETNADEVALUE("recycltype", m_recycl_type);\
	GETNADEVALUE("recyclget", m_recycl_get);\
	if (m_sell_price < 0)\
	{\
		sprintf(errinfo,"%s: xml node error in node [sellprice] must be >= 0.", path);\
		*err = errinfo;\
		return false;\
	}\
	GETNADEVALUE("pile_limit", m_pile_limit);\
	if (m_pile_limit <= 0)\
	{\
	sprintf(errinfo,"%s: xml node error in node [pile_limit] must be > 0.", path);\
	*err = errinfo;\
	return false;\
	}\
	GETNADEVALUE("isdroprecord", m_is_record);\
	GETNADEVALUE("time_length", m_time_length);\
	GETNADEVALUE("invalid_time", m_invalid_time);\
	PugiGetSubNodeValue(RootElement, "is_curday_valid", m_is_curday_valid);\
	GETNADEVALUE("limit_prof", m_limit_prof);\
	if (m_limit_prof <= PROF_TYPE_INVALID || m_limit_prof > PROF_TYPE_MAX)\
	{\
		sprintf(errinfo,"%s: xml node error in node [limit_prof] value.", path);\
		*err = errinfo;\
		return false;\
	}\
	GETNADEVALUE("limit_level", m_limit_level);\
	if (m_limit_level < 0 || m_limit_level > MAX_ROLE_LEVEL)\
	{\
		sprintf(errinfo,"%s: xml node error in node [limit_level] value.", path);\
		*err = errinfo;\
		return false;\
	}\
	GETNADEVALUE("limit_sex", m_limit_sex);\
	if (m_limit_sex < FEMALE || m_limit_sex > SEX_MAX)\
	{\
		sprintf(errinfo,"%s: xml node error in node [limit_sex] value.", path);\
		*err = errinfo;\
		return false;\
	}\
	GETNADEVALUE("other_sex_itemid", m_other_sex_itemid);\
	if (m_other_sex_itemid != 0)\
	{\
		if (m_limit_sex == SEX_MAX)\
		{\
			sprintf(errinfo,"%s: xml node error in node [other_sex_itemid] value.", path);\
			*err = errinfo;\
			return false;\
		}\
		CheckResourceCenter::Instance().GetOtherSexItemCheck()->Add(m_item_id, m_other_sex_itemid);\
	}\
	std::string item_name;\
	GETNADEVALUE("name", item_name);\
	if (item_name.length() >= sizeof(m_item_name))\
	{\
		sprintf(errinfo,"%s: xml node error in node [name] too long.", path);\
		*err = errinfo;\
		return false;\
	}\
	memcpy(m_item_name, item_name.c_str(), (int)item_name.length());\
	m_item_name[item_name.length()] = '\0';

//-------------------------------------------------------------------------------------------
#define LOADITEMUSECD() \
	GETNADEVALUE("colddown_id", m_colddown_id);\
	GETNADEVALUE("server_colddown", m_server_colddown);\
	if (m_server_colddown < 0)\
	{\
		sprintf(errinfo,"%s: xml colddown error.", path);\
		*err = errinfo;\
		return false;\
	}

//-------------------------------------------------------------------------------------------
#define LOADCOMPOSEBASE \
	GETNADEVALUE("cancompose", m_can_compose);\
	GETNADEVALUE("need_coin", m_need_coin);\
	GETNADEVALUE("need_num", m_need_num);\
	GETNADEVALUE("product_item_id", m_product_item_id);\
	if (m_can_compose)\
	{\
	if (m_need_coin < 0 || m_need_num <= 0 || m_product_item_id <= 0)\
		{\
		sprintf(errinfo,"%s: xml error ComposeItem need_coin:%d, need_num:%d, product_item_id:%d.", \
		path, (int)m_need_coin, (int)m_need_num, (int)m_product_item_id);\
		*err = errinfo;\
		return false;\
		}\
		CheckResourceCenter::Instance().GetItemValidCheck()->Add(m_item_id, m_product_item_id, 1);\
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Equipment::Init(const char* path, std::string *err)
{
	LOADCONFIG;
	LOADITEMBASEATTR;

	if (1 != m_pile_limit)
	{
		sprintf(errinfo,"%s: xml node error in node pile_limit[%d] != 1.", path, m_pile_limit);
		*err = errinfo;
		return false;
	}
	
	//GETNADEVALUE("quality", m_quality);
// 	if (m_quality < ItemBase::I_QUALITY_MIN || m_quality >= ItemBase::I_QUALITY_MAX)
// 	{
// 		sprintf(errinfo,"%s: xml node error in node [%s] value.", path, "quality");
// 		*err = errinfo;
// 		return false;
// 	}

	GETNADEVALUE("appe_type", m_act_shizhung_type);

	UNSTD_STATIC_CHECK(12 == BASE_ATTR_NUM);
	m_base_attr_type_list[0] = CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI;
	GETNADEVALUE("attack", m_base_attr_value_list[0]);

	m_base_attr_type_list[1] = CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU;
	GETNADEVALUE("fangyu", m_base_attr_value_list[1]);

	m_base_attr_type_list[2] = CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP;
	GETNADEVALUE("hp", m_base_attr_value_list[2]);

	m_base_attr_type_list[3] = CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXMP;
	GETNADEVALUE("mp", m_base_attr_value_list[3]);

	m_base_attr_type_list[4] = CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG;
	GETNADEVALUE("mingzhong", m_base_attr_value_list[4]);

	m_base_attr_type_list[5] = CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI;
	GETNADEVALUE("shanbi", m_base_attr_value_list[5]);

	m_base_attr_type_list[6] = CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI;
	GETNADEVALUE("baoji", m_base_attr_value_list[6]);

	m_base_attr_type_list[7] = CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN;
	GETNADEVALUE("jianren", m_base_attr_value_list[7]);

	m_base_attr_type_list[8] = CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN;
	GETNADEVALUE("per_jingzhun", m_base_attr_value_list[8]);

	m_base_attr_type_list[9] = CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT;
	GETNADEVALUE("per_baoji", m_base_attr_value_list[9]);

	m_base_attr_type_list[10] = CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG;
	GETNADEVALUE("per_pofang", m_base_attr_value_list[10]);

	m_base_attr_type_list[11] = CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG;
	GETNADEVALUE("per_mianshang", m_base_attr_value_list[11]);

	GETNADEVALUE("can_strengthen", m_can_strengthen);
	int tmp_shen = 0;
	GETNADEVALUE("shen", tmp_shen);
	m_is_shen = (1 == tmp_shen) ? 1 : 0;

	GETNADEVALUE("order", m_order);
	if (m_order >= ItemPool::MAX_EQUIPMENT_ORDER)
	{
		sprintf(errinfo,"%s: order:%d order must less 20.", path, m_order);
		return false;
	}

	GETNADEVALUE("sub_type", m_sub_type);
	GETNADEVALUE("guild_storage_score", m_guild_storage_score);
	GETNADEVALUE("god_attr", god_attr);
	
	GETNADEVALUE("suit_id", m_suit_id);
	if (0 != m_suit_id)
	{
		int tmp = m_suit_id / 1000;

		int suit_prof = tmp % 10;
		int suit_type = tmp / 10;

		{
			if (this->GetLimitProf() != suit_prof)
			{
				sprintf(errinfo,"%s: myprof %d != suit_prof %d.", path, this->GetLimitProf(), suit_prof);
				*err = errinfo;
				return false;
			}
		}

		if (E_TYPE_XIANGLIAN == m_equipment_type || E_TYPE_JIEZHI == m_equipment_type || E_TYPE_WUQI == m_equipment_type)
		{
			if (EquipSuitCfg::WUQI_SUIT_TYPE_FACTOR != suit_type * 10000)
			{
				sprintf(errinfo,"%s: wuqi suit id must start with 2.", path);
				*err = errinfo;
				return false;
			}
		}
		else
		{
			if (EquipSuitCfg::FANGJU_SUIT_TYPE_FACTOR != suit_type * 10000)
			{
				sprintf(errinfo,"%s: fangju suit id must start with 1.", path);
				*err = errinfo;
				return false;
			}
		}

		if (0 != suit_prof)		// 职业为0的套装其实是不存在的
		{
			CheckResourceCenter::Instance().GetSuitCheck()->Add(m_item_id, m_suit_id);
		}
	}	

	return true;
}

bool NorexItem::Init(const char* path, std::string *err)
{
	LOADCONFIG;
	LOADITEMBASEATTR;

	GETNADEVALUE("use_type", m_use_type);
	if (m_use_type <= I_NOREX_ITEM_INVALID || m_use_type >= I_NOREX_ITEM_MAX)
	{
		sprintf(errinfo,"%s: xml node error in node [%s] value.", path, "use_type");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("colddown_id", m_colddown_id);
	if (m_colddown_id < 0)
	{
		sprintf(errinfo,"%s: xml node error in node [%s] value.", path, "colddown_id");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("server_colddown", m_server_colddown);
	if (m_colddown_id < 0)
	{
		sprintf(errinfo,"%s: xml node error in node [%s] value.", path, "server_colddown");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("colddown_id", m_colddown_id);
	if (m_colddown_id < 0)
	{
		sprintf(errinfo,"%s: xml node error in node [%s] value.", path, "colddown_id");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("use_daytimes", m_use_daytimes);
	if (m_param1 < 0)
	{
		sprintf(errinfo,"%s: xml node error in node [%s] value.", path, "use_daytimes");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("param1", m_param1);
	if (m_param1 < 0)
	{
		sprintf(errinfo,"%s: xml node error in node [%s] value.", path, "param1");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("param2", m_param2);
	if (m_param2 < 0)
	{
		sprintf(errinfo,"%s: xml node error in node [%s] value.", path, "param2");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("param3", m_param3);
	if (m_param3 < 0)
	{
		sprintf(errinfo,"%s: xml node error in node [%s] value.", path, "param3");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("param4", m_param4);
	if (m_param4 < 0)
	{
		sprintf(errinfo,"%s: xml node error in node [%s] value.", path, "m_param4");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("param5", m_param5);
	if (m_param5 < 0)
	{
		sprintf(errinfo, "%s: xml node error in node [%s] value.", path, "m_param5");
		*err = errinfo;
		return false;
	}

	switch (m_use_type)
	{
	case I_NOREX_ITEM_TITLE_CARD:
		{
			CheckResourceCenter::Instance().GetNorexCheck()->Add(m_item_id);
		}
		break;

	default:
		break;
	}

	return true;
}

bool OtherItem::Init(const char* path, std::string *err)
{
	LOADCONFIG;
	LOADITEMBASEATTR;

	GETNADEVALUE("sub_type", m_sub_type);
	if (m_sub_type < I_SUB_TYPE_INVALID || m_sub_type >= OTHER_ITEM_SUB_TYPE_NUM)
	{
		sprintf(errinfo,"%s: xml node error in node [%s] value.", path, "sub_type");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("value", m_value);
	if (m_value < 0)
	{
		sprintf(errinfo,"%s: xml node error in node [%s] value.", path, "value");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("guild_storage_score", m_guild_storage_score);
	if (m_guild_storage_score < 0)
	{
		sprintf(errinfo,"%s: xml node error in node [%s] guild_storage_score.", path, "guild_storage_score");
		*err = errinfo;
		return false;
	}

	return true;
}

bool DefGift::Init(const char* path, std::string *err)
{
	LOADCONFIG;
	LOADITEMBASEATTR;

	//if (!m_is_bind)
	//{
	//	sprintf(errinfo,"%s: xml error in node [%s] value. gift must be bind", path, "is_bind");
	//	*err = errinfo;
	//	return false;
	//}

	GETNADEVALUE("coin_bind", m_coin_bind);
	if (m_coin_bind < 0)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value.", path, "coin_bind");
		*err = errinfo;
		return false;
	}
	
	GETNADEVALUE("gold_bind", m_gold_bind);
	if (m_gold_bind < 0)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value.", path, "gold_bind");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("gold", m_gold);
	if (m_gold < 0)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value.", path, "gold");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("is_check_prof", m_is_check_prof);
	GETNADEVALUE("item_num", m_item_num);
	if (m_item_num < 0 || m_item_num > GIFT_ITEM_MAX_NUM)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value.", path, "item_num");
		*err = errinfo;
		return false;
	}

	char strNode[32] = "";
	for (int i = 0; i < m_item_num; ++i)
	{
		sprintf(strNode,"item_%d_id", i + 1);
		GETNADEVALUE(strNode, m_gift_item_list[i].item_id);
		if (m_gift_item_list[i].item_id <= 0)
		{
			sprintf(errinfo,"%s: xml error in node [%s] value.", path, strNode);
			*err = errinfo;
			return false;
		}

		if (m_gift_item_list[i].item_id == m_item_id)
		{
			sprintf(errinfo,"%s: def gift cannot cantain self %d.", path, m_item_id);
			*err = errinfo;
			return false;
		}

		sprintf(strNode,"item_%d_num", i + 1);
		GETNADEVALUE(strNode, m_gift_item_list[i].num);
		if (m_gift_item_list[i].num <= 0)
		{
			sprintf(errinfo,"%s: xml error in node [%s] value.", path, strNode);
			*err = errinfo;
			return false;
		}

		CheckResourceCenter::Instance().GetItemValidCheck()->Add(m_item_id, m_gift_item_list[i].item_id, m_gift_item_list[i].num);

		sprintf(strNode,"is_bind_%d", i + 1);
		GETNADEVALUE(strNode, m_gift_item_list[i].is_bind);
	}

	for (int i = m_item_num; i < GIFT_ITEM_MAX_NUM; ++i)
	{
		sprintf(strNode,"item_%d_id", i + 1);
		GETNADEVALUE(strNode, m_gift_item_list[i].item_id);
		if (0 != m_gift_item_list[i].item_id)
		{
			sprintf(errinfo,"%s: xml error in node [%s] not zero.", path, strNode);
			*err = errinfo;
			return false;
		}
	}

	GETNADEVALUE("need_gold", m_need_gold);
	if (m_need_gold < 0)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value.", path, "need_gold");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("is_box", is_box);
	if (is_box < 0)
	{
		sprintf(errinfo, "%s: xml error in node [%s] value.", path, "is_box");
		*err = errinfo;
		return false;
	}

	return true;
}

bool RandGift::Init(const char* path, std::string *err)
{
	LOADCONFIG;
	LOADITEMBASEATTR;

	if (!m_is_bind)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value. gift must be bind", path, "is_bind");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("coin_bind", m_coin_bind);
	if (m_coin_bind < 0)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value.", path, "coin_bind");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("gold_bind", m_gold_bind);
	if (m_gold_bind < 0)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value.", path, "gold_bind");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("gold", m_gold);
	if (m_gold < 0)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value.", path, "gold");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("is_check_prof", m_is_check_prof);
	GETNADEVALUE("item_num", m_item_num);
	if (m_item_num < 0 || m_item_num > GIFT_ITEM_MAX_NUM)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value.", path, "item_num");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("rand_num", m_rand_num);
	if (m_rand_num < 0 || m_rand_num >= m_item_num)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value.", path, "rand_num");
		*err = errinfo;
		return false;
	}

	m_sum_rate = 0;

	char strNode[32] = "";
	for (int i = 0; i < m_item_num; ++i)
	{
		sprintf(strNode,"item_%d_id", i + 1);
		GETNADEVALUE(strNode, m_gift_item_list[i].item_id);
		if (m_gift_item_list[i].item_id <= 0)
		{
			sprintf(errinfo,"%s: xml error in node [%s] value.", path, strNode);
			*err = errinfo;
			return false;
		}

		if (m_gift_item_list[i].item_id == m_item_id)
		{
			sprintf(errinfo,"%s: rand gift cannot cantain self %d.", path, m_item_id);
			*err = errinfo;
			return false;
		}

		sprintf(strNode,"item_%d_num", i + 1);
		GETNADEVALUE(strNode, m_gift_item_list[i].num);
		if (m_gift_item_list[i].num <= 0)
		{
			sprintf(errinfo,"%s: xml error in node [%s] value.", path, strNode);
			*err = errinfo;
			return false;
		}

		CheckResourceCenter::Instance().GetItemValidCheck()->Add(m_item_id, m_gift_item_list[i].item_id, m_gift_item_list[i].num);

		sprintf(strNode,"is_bind_%d", i + 1);
		GETNADEVALUE(strNode, m_gift_item_list[i].is_bind);

		sprintf(strNode,"item_%d_rate", i + 1);
		GETNADEVALUE(strNode, m_item_rate_list[i]);
		if (m_item_rate_list[i] <= 0)
		{
			sprintf(errinfo,"%s: xml error in node [%s] value.", path, strNode);
			*err = errinfo;
			return false;
		}

		m_sum_rate += m_item_rate_list[i];
	}

	if (m_sum_rate <= 0)
	{
		sprintf(errinfo,"%s: xml error m_sum_rate <= 0.", path);
		*err = errinfo;
		return false;
	}

	for (int i = m_item_num; i < GIFT_ITEM_MAX_NUM; ++i)
	{
		sprintf(strNode,"item_%d_id", i + 1);
		GETNADEVALUE(strNode, m_gift_item_list[i].item_id);
		if (0 != m_gift_item_list[i].item_id)
		{
			sprintf(errinfo,"%s: xml error in node [%s] not zero.", path, strNode);
			*err = errinfo;
			return false;
		}
	}

	UNSTD_STATIC_CHECK(5 == RAND_GIFT_BROADCAST_ITEM_NUM);
	GETNADEVALUE("boardcast_item_1", m_boardcast_item_list[0]);
	GETNADEVALUE("boardcast_item_2", m_boardcast_item_list[1]);
	GETNADEVALUE("boardcast_item_3", m_boardcast_item_list[2]);
	GETNADEVALUE("boardcast_item_4", m_boardcast_item_list[3]);
	GETNADEVALUE("boardcast_item_5", m_boardcast_item_list[4]);

	for (int i = 0; i < RAND_GIFT_BROADCAST_ITEM_NUM; i++)
	{
		if (0 == m_boardcast_item_list[i]) continue;

		bool has_found = false;
		for (int k = 0; k < GIFT_ITEM_MAX_NUM; k++)
		{
			if (0 == m_gift_item_list[k].item_id) continue;

			if (m_boardcast_item_list[i] == m_gift_item_list[k].item_id)
			{
				has_found = true;
				break;
			}
		}

		if (!has_found)
		{
			sprintf(errinfo,"%s: xml error broadcast_item not found item_id = %d.", path, (int)m_boardcast_item_list[i]);
			*err = errinfo;
			return false;
		}
	}

	GETNADEVALUE("boardcast_string_id", m_boardcast_string_id);
	if (m_boardcast_item_list[0] > 0 || m_boardcast_item_list[1] > 0 || m_boardcast_item_list[2] > 0 || m_boardcast_item_list[3] > 0 || m_boardcast_item_list[4] > 0)
	{
		if (m_boardcast_string_id <= 0 || m_boardcast_string_id >= OPEN_RANDGIFT_BROADCAST_ID_MAX_NUM)
		{
			sprintf(errinfo,"%s: xml error boardcast_string_id error", path);
			*err = errinfo;
			return false;
		}
	}
	else
	{
		if (m_boardcast_string_id > 0)
		{
			sprintf(errinfo,"%s: xml error boardcast_string_id no need", path);
			*err = errinfo;
			return false;
		}
	}

	GETNADEVALUE("need_gold", m_need_gold);
	if (m_need_gold < 0)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value.", path, "need_gold");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("day_usetimes", m_day_usetimes);
	if (m_day_usetimes < 0)
	{
		sprintf(errinfo, "%s: xml error in node [%s] value.", path, "day_usetimes");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("is_box", is_box);
	if (is_box < 0)
	{
		sprintf(errinfo, "%s: xml error in node [%s] value.", path, "is_box");
		*err = errinfo;
		return false;
	}

	return true;
}

bool SelectGift::Init(const char* path, std::string *err)
{
	LOADCONFIG;
	LOADITEMBASEATTR;

	if (!m_is_bind)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value. gift must be bind", path, "is_bind");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("coin_bind", m_coin_bind);
	if (m_coin_bind < 0)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value.", path, "coin_bind");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("gold_bind", m_gold_bind);
	if (m_gold_bind < 0)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value.", path, "gold_bind");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("gold", m_gold);
	if (m_gold < 0)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value.", path, "gold");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("is_check_prof", m_is_check_prof);
	GETNADEVALUE("item_num", m_item_num);
	if (m_item_num < 0 || m_item_num > GIFT_ITEM_MAX_NUM)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value.", path, "item_num");
		*err = errinfo;
		return false;
	}

	char strNode[32] = "";
	for (int i = 0; i < m_item_num; ++i)
	{
		sprintf(strNode,"item_%d_id", i + 1);
		GETNADEVALUE(strNode, m_gift_item_list[i].item_id);
		if (m_gift_item_list[i].item_id <= 0)
		{
			sprintf(errinfo,"%s: xml error in node [%s] value.", path, strNode);
			*err = errinfo;
			return false;
		}

		if (m_gift_item_list[i].item_id == m_item_id)
		{
			sprintf(errinfo,"%s: def gift cannot cantain self %d.", path, m_item_id);
			*err = errinfo;
			return false;
		}

		sprintf(strNode,"item_%d_num", i + 1);
		GETNADEVALUE(strNode, m_gift_item_list[i].num);
		if (m_gift_item_list[i].num <= 0)
		{
			sprintf(errinfo,"%s: xml error in node [%s] value.", path, strNode);
			*err = errinfo;
			return false;
		}

		CheckResourceCenter::Instance().GetItemValidCheck()->Add(m_item_id, m_gift_item_list[i].item_id, m_gift_item_list[i].num);

		sprintf(strNode,"is_bind_%d", i + 1);
		GETNADEVALUE(strNode, m_gift_item_list[i].is_bind);
	}

	for (int i = m_item_num; i < GIFT_ITEM_MAX_NUM; ++i)
	{
		sprintf(strNode,"item_%d_id", i + 1);
		GETNADEVALUE(strNode, m_gift_item_list[i].item_id);
		if (0 != m_gift_item_list[i].item_id)
		{
			sprintf(errinfo,"%s: xml error in node [%s] not zero.", path, strNode);
			*err = errinfo;
			return false;
		}
	}

	GETNADEVALUE("need_gold", m_need_gold);
	if (m_need_gold < 0)
	{
		sprintf(errinfo,"%s: xml error in node [%s] value.", path, "need_gold");
		*err = errinfo;
		return false;
	}

	GETNADEVALUE("is_box", is_box);
	if (is_box < 0)
	{
		sprintf(errinfo, "%s: xml error in node [%s] value.", path, "is_box");
		*err = errinfo;
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ItemPool::Init(const std::string &dir, const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, dir + configname, "ItemManager", *err)

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	PugiXmlNode currentElement;

	LOADEQUIPMENT("TouKui", Equipment, Equipment::E_TYPE_TOUKUI);
	LOADEQUIPMENT("YiFu", Equipment, Equipment::E_TYPE_YIFU);
	LOADEQUIPMENT("HuTui", Equipment, Equipment::E_TYPE_KUZI);
	LOADEQUIPMENT("XieZi", Equipment, Equipment::E_TYPE_XIEZI);
	LOADEQUIPMENT("HuShou", Equipment, Equipment::E_TYPE_HUSHOU);
	LOADEQUIPMENT("XiangLian", Equipment, Equipment::E_TYPE_XIANGLIAN);
	LOADEQUIPMENT("WuQi", Equipment, Equipment::E_TYPE_WUQI);
	LOADEQUIPMENT("JieZhi", Equipment, Equipment::E_TYPE_JIEZHI);
	LOADEQUIPMENT("YaoDai", Equipment, Equipment::E_TYPE_YAODAI);
	LOADEQUIPMENT("YuPei", Equipment, Equipment::E_TYPE_YUPEI);
	LOADEQUIPMENT("ShouZhuo", Equipment, Equipment::E_TYPE_SHOUZHUO);

	LOADEQUIPMENT("JingLing", Equipment, Equipment::E_TYPE_SPECIAL_JINGLING);
	LOADEQUIPMENT("HunJie", Equipment, Equipment::E_TYPE_SPECIAL_HUNJIE);
	LOADEQUIPMENT("littlepet", Equipment, Equipment::E_TYPE_SPECIAL_LITTLEPET);

	LOADEQUIPMENT("MountEquip", Equipment, Equipment::E_TYPE_MOUNT);
	LOADEQUIPMENT("WingEquip", Equipment, Equipment::E_TYPE_WING);
	LOADEQUIPMENT("HaloEquip", Equipment, Equipment::E_TYPE_HALO);
	LOADEQUIPMENT("ShenGongEquip", Equipment, Equipment::E_TYPE_SHEN_GONG);
	LOADEQUIPMENT("ShenYiEquip", Equipment, Equipment::E_TYPE_SHEN_YI);

	LOADEQUIPMENT("zhuansheng1", Equipment, Equipment::E_TYPE_ZHUANSHENG_1);
	LOADEQUIPMENT("zhuansheng2", Equipment, Equipment::E_TYPE_ZHUANSHENG_2);
	LOADEQUIPMENT("zhuansheng3", Equipment, Equipment::E_TYPE_ZHUANSHENG_3);
	LOADEQUIPMENT("zhuansheng4", Equipment, Equipment::E_TYPE_ZHUANSHENG_4);
	LOADEQUIPMENT("zhuansheng5", Equipment, Equipment::E_TYPE_ZHUANSHENG_5);
	LOADEQUIPMENT("zhuansheng6", Equipment, Equipment::E_TYPE_ZHUANSHENG_6);
	LOADEQUIPMENT("zhuansheng7", Equipment, Equipment::E_TYPE_ZHUANSHENG_7);
	LOADEQUIPMENT("zhuansheng8", Equipment, Equipment::E_TYPE_ZHUANSHENG_8);

	LOADEQUIPMENT("qingyuan1", Equipment, Equipment::E_TYPE_QINGYUAN_1);
	LOADEQUIPMENT("qingyuan2", Equipment, Equipment::E_TYPE_QINGYUAN_2);
	LOADEQUIPMENT("qingyuan3", Equipment, Equipment::E_TYPE_QINGYUAN_3);
	LOADEQUIPMENT("qingyuan4", Equipment, Equipment::E_TYPE_QINGYUAN_4);

	// todo
	LOADEQUIPMENT("flyWuQi",		Equipment, Equipment::E_TYPE_ZHUANZHI_WUQI);
	LOADEQUIPMENT("flyYiFu",		Equipment, Equipment::E_TYPE_ZHUANZHI_YIFU);
	LOADEQUIPMENT("flyHuShou",		Equipment, Equipment::E_TYPE_ZHUANZHI_HUSHOU);
	LOADEQUIPMENT("flyYaoDai",		Equipment, Equipment::E_TYPE_ZHUANZHI_YAODAI);
//	LOADEQUIPMENT("flyHuTui",		Equipment, Equipment::E_TYPE_ZHUANZHI_HUTUI);
	LOADEQUIPMENT("flytoukui",		Equipment, Equipment::E_TYPE_ZHUANZHI_TOUKUI);
	LOADEQUIPMENT("flyXiangLian",	Equipment, Equipment::E_TYPE_ZHUANZHI_XIANGLIAN);
	LOADEQUIPMENT("flyShouZhuo",	Equipment, Equipment::E_TYPE_ZHUANZHI_SHOUZHUO);
	LOADEQUIPMENT("flyJieZhi",		Equipment, Equipment::E_TYPE_ZHUANZHI_JIEZHI);
	LOADEQUIPMENT("flyXieZi",		Equipment, Equipment::E_TYPE_ZHUANZHI_XIEZI);
	LOADEQUIPMENT("flyYuPei",		Equipment, Equipment::E_TYPE_ZHUANZHI_YUPEI);

	LOADEQUIPMENT("littlepet_equip1", Equipment, Equipment::E_TYPE_LITTLE_PET_1);
	LOADEQUIPMENT("littlepet_equip2", Equipment, Equipment::E_TYPE_LITTLE_PET_2);
	LOADEQUIPMENT("littlepet_equip3", Equipment, Equipment::E_TYPE_LITTLE_PET_3);
	LOADEQUIPMENT("littlepet_equip4", Equipment, Equipment::E_TYPE_LITTLE_PET_4);

	LOADEQUIPMENT("chineseZidoac_equip1", Equipment, Equipment::E_TYPE_CHINESE_ZODIAC_1);
	LOADEQUIPMENT("chineseZidoac_equip2", Equipment, Equipment::E_TYPE_CHINESE_ZODIAC_2);
	LOADEQUIPMENT("chineseZidoac_equip3", Equipment, Equipment::E_TYPE_CHINESE_ZODIAC_3);
	LOADEQUIPMENT("chineseZidoac_equip4", Equipment, Equipment::E_TYPE_CHINESE_ZODIAC_4);
	LOADEQUIPMENT("chineseZidoac_equip5", Equipment, Equipment::E_TYPE_CHINESE_ZODIAC_5);

	LOADEQUIPMENT("shenshou1", Equipment, Equipment::E_TYPE_SHENSHOU_1);
	LOADEQUIPMENT("shenshou2", Equipment, Equipment::E_TYPE_SHENSHOU_2);
	LOADEQUIPMENT("shenshou3", Equipment, Equipment::E_TYPE_SHENSHOU_3);
	LOADEQUIPMENT("shenshou4", Equipment, Equipment::E_TYPE_SHENSHOU_4);
	LOADEQUIPMENT("shenshou5", Equipment, Equipment::E_TYPE_SHENSHOU_5);

	LOADEQUIPMENT("impguard", Equipment, Equipment::E_TYPE_SPECIAL_IMP_GUARD);

	LOADEQUIPMENT("shenmo1", Equipment, Equipment::E_TYPE_GREATESOLDIER_1);
	LOADEQUIPMENT("shenmo2", Equipment, Equipment::E_TYPE_GREATESOLDIER_2);
	LOADEQUIPMENT("shenmo3", Equipment, Equipment::E_TYPE_GREATESOLDIER_3);
	LOADEQUIPMENT("shenmo4", Equipment, Equipment::E_TYPE_GREATESOLDIER_4);

	LOADEQUIPMENT("baizhan_0", Equipment, Equipment::E_TYPE_BAIZHAN_WUQI);
	LOADEQUIPMENT("baizhan_1", Equipment, Equipment::E_TYPE_BAIZHAN_YIFU);
	LOADEQUIPMENT("baizhan_2", Equipment, Equipment::E_TYPE_BAIZHAN_HUSHOU);
	LOADEQUIPMENT("baizhan_3", Equipment, Equipment::E_TYPE_BAIZHAN_YAODAI);
	LOADEQUIPMENT("baizhan_4", Equipment, Equipment::E_TYPE_BAIZHAN_TOUKUI);
	LOADEQUIPMENT("baizhan_5", Equipment, Equipment::E_TYPE_BAIZHAN_XIANGLIAN);
	LOADEQUIPMENT("baizhan_6", Equipment, Equipment::E_TYPE_BAIZHAN_SHOUZHUO);
	LOADEQUIPMENT("baizhan_7", Equipment, Equipment::E_TYPE_BAIZHAN_JIEZHI);
	LOADEQUIPMENT("baizhan_8", Equipment, Equipment::E_TYPE_BAIZHAN_XIEZI);
	LOADEQUIPMENT("baizhan_9", Equipment, Equipment::E_TYPE_BAIZHAN_YUPEI);

	LOADEQUIPMENT("douqi_wuqi", Equipment, Equipment::E_TYPE_CHUANSHI_WUQI);
	LOADEQUIPMENT("douqi_toukui", Equipment, Equipment::E_TYPE_CHUANSHI_TOUKUI);
	LOADEQUIPMENT("douqi_yifu", Equipment, Equipment::E_TYPE_CHUANSHI_YIFU);
	LOADEQUIPMENT("douqi_hushou", Equipment, Equipment::E_TYPE_CHUANSHI_HUSHOU);
	LOADEQUIPMENT("douqi_fuwu", Equipment, Equipment::E_TYPE_CHUANSHI_YAODAI);
	LOADEQUIPMENT("douqi_hutui", Equipment, Equipment::E_TYPE_CHUANSHI_HUTUI);
	LOADEQUIPMENT("douqi_xiezi", Equipment, Equipment::E_TYPE_CHUANSHI_XIEZI);
	LOADEQUIPMENT("douqi_shouzhuo", Equipment, Equipment::E_TYPE_CHUANSHI_ZHUOZI);
	LOADEQUIPMENT("douqi_xianglian", Equipment, Equipment::E_TYPE_CHUANSHI_XIANGLIAN);
	LOADEQUIPMENT("douqi_jiezhi", Equipment, Equipment::E_TYPE_CHUANSHI_JIEZHI);

	LOADITEM("ExpenseNorex", NorexItem);

	LOADITEM("Other", OtherItem);

	LOADITEM("DefGift", DefGift);
	LOADITEM("RandGift", RandGift);
	LOADITEM("SelectGift", SelectGift);
	LOADITEM("SelectGift1", SelectGift);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ItemPool *gs_item_pool_config = NULL;

bool ItemPool::Reload(const std::string &dir, const std::string &filename, std::string *err)
{
	ItemPool *temp = new ItemPool();

	if (!temp->Init(dir, filename, err))
	{
		delete temp; temp = NULL;
		return false;
	}

	if (NULL != gs_item_pool_config) delete gs_item_pool_config;

	gs_item_pool_config = temp;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RELOAD_ITEM(RELOAD_TYPE, CLASS_XML_DIR, CLASSNAME) \
	if (RELOAD_TYPE == item_type)\
	{\
		SNPRINTF(tmp_str, sizeof(tmp_str), "%s/item/%s/%d.xml", dir.c_str(), CLASS_XML_DIR, item_id);\
		PugiXmlDocument m_document;\
		PUGI_XML_LOAD_AND_CHECK(m_document, std::string(tmp_str), "Load Item", *err);\
		ItemBase *tmp_item = new CLASSNAME();\
		if (!tmp_item->Init(tmp_str, err))\
		{\
			SNPRINTF(tmp_str, sizeof(tmp_str), "ReloadItem error ItemInit item_id:%d!!!", item_id);\
			*err = tmp_str;\
			delete tmp_item;\
			return false;\
		}\
		if (NULL != m_item_pool[item_id])\
		{\
			delete m_item_pool[item_id];\
		}\
		m_item_pool[item_id] = tmp_item;\
		return true;\
	}

bool ItemPool::ReloadOneItem(const std::string &dir, int item_type, ItemID item_id, std::string *err)
{
	static char tmp_str[1024] = {0};

	RELOAD_ITEM(RELOAD_I_TYPE_RANDGIFT, "gift", RandGift);
	RELOAD_ITEM(RELOAD_I_TYPE_EXPENSE, "expense", NorexItem);
	RELOAD_ITEM(RELOAD_I_TYPE_OTHER, "other", OtherItem);

	SNPRINTF(tmp_str, sizeof(tmp_str), "ReloadItem error cannot find item:%d!", item_id);
	*err = tmp_str;

	return false;
}

ItemPool * ItemPool::Instance()
{
	if (NULL == gs_item_pool_config)
	{
		gs_item_pool_config = new ItemPool();
	}

	return gs_item_pool_config;
}

ItemPool::ItemPool()
{
	memset(m_item_pool, 0, sizeof(m_item_pool));
}

ItemPool::~ItemPool()
{
	this->Release();
}

void ItemPool::Release()
{
	for (int i = 0; i < MAX_ITEM_ID; ++i)
	{
		if (NULL != m_item_pool[i])
		{
			delete m_item_pool[i];
		}
	}
	memset(m_item_pool, 0, sizeof(m_item_pool));
}

const ItemBase * ItemPool::GetItem(ItemID item_id)
{
	if (item_id < 0 || item_id >= MAX_ITEM_ID) return NULL;

	return m_item_pool[item_id];
}

long long ItemPool::ConvertToLongLong(short param1, short param2, short param3, char param4, char param5)
{
	union ParamToLongLong
	{
		struct
		{
			short m_param1;
			short m_param2;										
			short m_param3;									
			char m_param4;					
			char m_param5;
		};

		long long val;
	};

	ParamToLongLong ll;

	ll.m_param1 = param1;
	ll.m_param2 = param2;
	ll.m_param3 = param3;
	ll.m_param4 = param4;
	ll.m_param5 = param5;

	return ll.val;
}
