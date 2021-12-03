#include "mentalityconfig.hpp"

#include "item/itempool.h"
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "globalconfig/globalconfig.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char * MENTALITY_TYPE_STR_MAPPER[] = 
{
	"maxhp",
	"gongji",
	"fangyu",
	"mingzhong",
	"shanbi",
	"fujia_shanghai",
	"dikang_shanghai" 
};

UNSTD_STATIC_CHECK(sizeof(MENTALITY_TYPE_STR_MAPPER) / sizeof(MENTALITY_TYPE_STR_MAPPER[0]) == MENTALITY_TYPE_MAX);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MentalityConfig::MentalityConfig()
	: m_mentality_title_count(0), m_gengu_title_count(0)
{

}

MentalityConfig::~MentalityConfig()
{

}

bool MentalityConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "MentalityConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}
	
	{	
		// 元神修炼相关
		PugiXmlNode MentalityListElement = RootElement.child("mentality_list");
		if (MentalityListElement.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->mentality_list]", path.c_str());
			*err = errinfo;
			return false;
		}
	

		int last_level = -1;

		PugiXmlNode ItemElement = MentalityListElement.child("data");
		while (!ItemElement.empty())
		{
			int level = 0;
			if (!PugiGetSubNodeValue(ItemElement, "mentality_level", level) || level != last_level + 1 || level < 0 || level > MAX_MENTALITY_BASE_LEVEL)		
			{
				sprintf(errinfo, "%s: xml node error in [config->mentality_list->data->mentality_level]", path.c_str());
				*err = errinfo;
				return false;
			}
			last_level = level;

			MentalityBaseItemConfig &mbic = m_mentality_base_config_list[level];
			if (!PugiGetSubNodeValue(ItemElement, "role_level_limit", mbic.rolelevel_limit) || mbic.rolelevel_limit <= 0 || mbic.rolelevel_limit > MAX_ROLE_LEVEL)
			{
				sprintf(errinfo, "%s: xml node error in [config->mentality_list->data->role_level_limit]", path.c_str());
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(ItemElement, "need_yuanli", mbic.need_yuanli) || mbic.need_yuanli < 0)
			{
				sprintf(errinfo, "%s: xml node error in [config->mentality_list->data->need_yuanli]", path.c_str());
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(ItemElement, "cd_time", mbic.cd_time) || mbic.cd_time < 0)
			{
				sprintf(errinfo, "%s: xml node error in [config->mentality_list->data->cd_time]", path.c_str());
				*err = errinfo;
				return false;
			}
			
			for (int i = 0; i < MENTALITY_TYPE_MAX; ++ i)
			{
				std::string temp_string(MENTALITY_TYPE_STR_MAPPER[i]);

				short attr_type = 0;
				if (!CharIntAttrs::GetAttrTypeFromAttrString(temp_string, &attr_type))
				{
					sprintf(errinfo, "mentality_list init fail: mentality type defination error");
					*err = errinfo;
					return false;
				}
				mbic.all_attrlist[i].attr_type = attr_type;

				if (!PugiGetSubNodeValue(ItemElement, MENTALITY_TYPE_STR_MAPPER[i], mbic.all_attrlist[i].attr_value) || mbic.all_attrlist[i].attr_value < 0)
				{
					sprintf(errinfo, "%s: xml node error in [config->mentality_list->data->%s]", path.c_str(), MENTALITY_TYPE_STR_MAPPER[i]);
					*err = errinfo;
					return false;
				}
			}

			ItemElement = ItemElement.next_sibling();
		}

		if (MAX_MENTALITY_BASE_LEVEL != last_level)
		{
			sprintf(errinfo, "%s: xml node error in [config->mentality_list], total mentality level error", path.c_str());
			*err = errinfo;
			return false;
		}
	}

	{	
		// 元神称号
		PugiXmlNode MentalityTitleElement = RootElement.child("mentality_title");
		if (MentalityTitleElement.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->mentality_title]", path.c_str());
			*err = errinfo;
			return false;
		}
		

		m_mentality_title_count = 0;

		PugiXmlNode ItemElement = MentalityTitleElement.child("data");
		while (!ItemElement.empty())
		{
			if (m_mentality_title_count >= MAX_MENTATITY_TITLE_COUNT_MAX)
			{
				sprintf(errinfo, "%s: xml node error in [config->mentality_title->data], too many title", path.c_str());
				*err = errinfo;
				return false;
			}
			
			MentalityTitleConfig &mtc = m_mentality_title[m_mentality_title_count];

			int upgrade_level = 0;
			if (!PugiGetSubNodeValue(ItemElement, "upgrade_level", upgrade_level) || upgrade_level <= 0)
			{
				sprintf(errinfo, "%s: xml node error in [config->mentality_title->data->mentality_level], must be positive", path.c_str());
				*err = errinfo;
				return false;
			}
			
			mtc.least_mentality_level = upgrade_level;
			if (mtc.least_mentality_level > MAX_MENTALITY_BASE_LEVEL)
			{	
				sprintf(errinfo, "%s: xml node error in [config->mentality_title->data->mentality_level], over max mentality level", path.c_str());
				*err = errinfo;
				return false;
			}
			
			for (int i = 0; i < MENTALITY_TYPE_MAX; ++ i)
			{
				std::string temp_string(MENTALITY_TYPE_STR_MAPPER[i]);

				short attr_type = 0;
				if (!CharIntAttrs::GetAttrTypeFromAttrString(temp_string, &attr_type))
				{
					sprintf(errinfo, "mentality_title init fail: mentality type defination error");
					*err = errinfo;
					return false;
				}
				mtc.mentality_attrlist[i].attr_type = attr_type;

				if (!PugiGetSubNodeValue(ItemElement, MENTALITY_TYPE_STR_MAPPER[i], mtc.mentality_attrlist[i].attr_value) || mtc.mentality_attrlist[i].attr_value < 0)
				{
					sprintf(errinfo, "%s: xml node error in [config->mentality_title->data->%s]", path.c_str(), MENTALITY_TYPE_STR_MAPPER[i]);
					*err = errinfo;
					return false;
				}
			}
			
			++ m_mentality_title_count;
			ItemElement = ItemElement.next_sibling();
		}

		if (m_mentality_title_count <= 0 || m_mentality_title_count > MAX_MENTATITY_TITLE_COUNT_MAX)
		{
			sprintf(errinfo, "%s: xml node error in [config->mentality_title->data], too few", path.c_str());
			*err = errinfo;
			return false;
		}
	}

	{	
		// 根骨修炼相关
		PugiXmlNode GenguListElement = RootElement.child("gengu_list");
		if (GenguListElement.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->gengu_list]", path.c_str());
			*err = errinfo;
			return false;
		}

		int last_level = -1;

		PugiXmlNode ItemElement = GenguListElement.child("data");
		while (!ItemElement.empty())
		{
			int level = 0;
			if (!PugiGetSubNodeValue(ItemElement, "gengu_level", level) || level != last_level + 1 || level < 0 || level > MAX_MENTALITY_GENGU_LEVEL)
			{
				sprintf(errinfo, "%s: xml node error in [config->gengu_list->data->gengu_level]", path.c_str());
				*err = errinfo;
				return false;
			}
			last_level = level;
			
			MentalityGenguItemConfig &mgic = m_mentality_gengu_config_list[level];
			if (!PugiGetSubNodeValue(ItemElement, "protect_level", mgic.protect_level) || mgic.protect_level < 0 || mgic.protect_level > level)
			{
				sprintf(errinfo, "%s: xml node error in [config->gengu_list->data->protect_level]", path.c_str());
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(ItemElement, "succ_rate", mgic.succ_rate) ||  mgic.succ_rate < 0 ||  mgic.succ_rate > 100)
			{
				sprintf(errinfo, "%s: xml node error in [config->gengu_list->data->succ_rate]", path.c_str());
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(ItemElement, "need_coin", mgic.need_coin) || mgic.need_coin < 0)
			{
				sprintf(errinfo, "%s: xml node error in [config->gengu_list->data->need_coin]", path.c_str());
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(ItemElement, "item_id", mgic.upgrade_itemid) || mgic.upgrade_itemid <= 0 || NULL == ITEMPOOL->GetItem(mgic.upgrade_itemid))
			{
				sprintf(errinfo, "%s: xml node error in [config->gengu_list->data->item_id]", path.c_str());
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(ItemElement, "protect_item_id", mgic.protect_item_id) || mgic.protect_item_id <= 0 || NULL == ITEMPOOL->GetItem(mgic.protect_item_id))
			{
				sprintf(errinfo, "%s: xml node error in [config->gengu_list->data->protect_item_id]", path.c_str());
				*err = errinfo;
				return false;
			}

			if (!PugiGetSubNodeValue(ItemElement, "protect_item_num", mgic.protect_item_num) || mgic.protect_item_num < 0)
			{
				sprintf(errinfo, "%s: xml node error in [config->gengu_list->data->protect_item_num]", path.c_str());
				*err = errinfo;
				return false;
			}

			for (int i = 0; i < MENTALITY_TYPE_MAX; ++ i)
			{
				std::string temp_string(MENTALITY_TYPE_STR_MAPPER[i]);

				short attr_type = 0;
				if (!CharIntAttrs::GetAttrTypeFromAttrString(temp_string, &attr_type))
				{
					sprintf(errinfo, "gengu_list init fail: gengu type defination error");
					*err = errinfo;
					return false;
				}
				mgic.all_attrlist[i].attr_type = attr_type;

				if (!PugiGetSubNodeValue(ItemElement, MENTALITY_TYPE_STR_MAPPER[i], mgic.all_attrlist[i].attr_value) || mgic.all_attrlist[i].attr_value < 0)
				{
					sprintf(errinfo, "%s: xml node error in [config->gengu_list->data->%s]", path.c_str(), MENTALITY_TYPE_STR_MAPPER[i]);
					*err = errinfo;
					return false;
				}
			}

			ItemElement = ItemElement.next_sibling();
		}

		if (MAX_MENTALITY_GENGU_LEVEL != last_level)
		{
			sprintf(errinfo, "%s: xml node error in [config->gengu_list] last_level[%d], not all level defined", path.c_str(), last_level);
			*err = errinfo;
			return false;
		}
	}

	{	
		// 根骨称号
		PugiXmlNode GenguTitleElement = RootElement.child("gengu_title");
		if (GenguTitleElement.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->gengu_title]", path.c_str());
			*err = errinfo;
			return false;
		}

		m_gengu_title_count = 0;

		PugiXmlNode ItemElement = GenguTitleElement.child("data");
		while (!ItemElement.empty())
		{
			if (m_gengu_title_count >= MAX_GENGU_TITLE_COUNT_MAX)
			{
				sprintf(errinfo, "%s: xml node error in [config->gengu_title->data], too many gengu title", path.c_str());
				*err = errinfo;
				return false;
			}

			GenguTitleConfig &gtc = m_gengu_title[m_gengu_title_count];
			if (!PugiGetSubNodeValue(ItemElement, "gengu_level", gtc.least_gengu_level) || gtc.least_gengu_level <= 0 || gtc.least_gengu_level > MAX_MENTALITY_GENGU_LEVEL)
			{
				sprintf(errinfo, "%s: xml node error in [config->gengu_title->data->gengu_level]", path.c_str());
				*err = errinfo;
				return false;
			}

			if (m_gengu_title_count > 0 && gtc.least_gengu_level <= m_gengu_title[m_gengu_title_count - 1].least_gengu_level)
			{
				sprintf(errinfo, "%s: xml node error in [config->gengu_title->data->gengu_level], must increase by degrees", path.c_str());
				*err = errinfo;
				return false;
			}

			for (int i = 0; i < MENTALITY_TYPE_MAX; ++ i)
			{
				std::string temp_string(MENTALITY_TYPE_STR_MAPPER[i]);

				short attr_type = 0;
				if (!CharIntAttrs::GetAttrTypeFromAttrString(temp_string, &attr_type))
				{
					sprintf(errinfo, "mentality_title init fail: mentality type defination error");
					*err = errinfo;
					return false;
				}
				gtc.gengu_attrlist[i].attr_type = attr_type;

				if (!PugiGetSubNodeValue(ItemElement, MENTALITY_TYPE_STR_MAPPER[i], gtc.gengu_attrlist[i].attr_value) || gtc.gengu_attrlist[i].attr_value < 0)
				{
					sprintf(errinfo, "%s: xml node error in [config->gengu_title->data->%s]", path.c_str(), MENTALITY_TYPE_STR_MAPPER[i]);
					*err = errinfo;
					return false;
				}
			}

			++ m_gengu_title_count;
			ItemElement = ItemElement.next_sibling();
		}

		if (m_gengu_title_count <= 0 || m_gengu_title_count > MAX_GENGU_TITLE_COUNT_MAX)
		{
			printf(errinfo, "%s: xml node error in [config->gengu_title->data], too few gengu title", path.c_str());
			*err = errinfo;
			return false;
		}
	}

	{
		// 其他
		PugiXmlNode otherElement = RootElement.child("other");
		if (otherElement.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->other]", path.c_str());
			*err = errinfo;
			return false;
		}

		PugiXmlNode dataElement = otherElement.child("data");
		if (dataElement.empty())
		{
			sprintf(errinfo, "%s: xml node error in [config->other->data]", path.c_str());
			*err = errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(dataElement, "clear_cd_cost", m_other_cfg.clear_cd_cost) || m_other_cfg.clear_cd_cost <= 0)
		{
			sprintf(errinfo, "%s: xml node error in [config->other->data->clear_cd_cost]", path.c_str());
			*err = errinfo;
			return false;
		}
	}

	//{	
	//	// 五行配置
	//	int last_level_list[MENTALITY_WUXING_MAX_COUNT];
	//	int last_attr_val_list[MENTALITY_WUXING_MAX_COUNT];
	//	int last_need_total_mentality_level_list[MENTALITY_WUXING_MAX_COUNT];

	//	for (int i = 0; i < MENTALITY_WUXING_MAX_COUNT; ++ i)
	//	{
	//		last_level_list[i] = -1;
	//		last_attr_val_list[i] = -1;
	//		last_need_total_mentality_level_list[i] = 0;
	//	}

	//	PugiXmlNode XiSuiElement = RootElement.child("xisui");
	//	if (XiSuiElement.empty())
	//	{
	//		sprintf(errinfo, "%s: xml node error in [config->xisui]", path.c_str());
	//		*err = errinfo;
	//		return false;
	//	}

	//	PugiXmlNode dataElement = XiSuiElement.child("data");
	//	while (!dataElement.empty())
	//	{
	//		int wx_index = 0;
	//		if (!PugiGetSubNodeValue(dataElement, "wuxing_type", wx_index) || wx_index < 0 || wx_index >= MENTALITY_WUXING_MAX_COUNT)
	//		{
	//			sprintf(errinfo, "%s: xml node error in [config->xisui->data->wuxing_type]", path.c_str());
	//			*err = errinfo;
	//			return false;
	//		}

	//		int wx_level = 0;
	//		if (!PugiGetSubNodeValue(dataElement, "wuxing_level", wx_level) || wx_level < 0 || wx_level > MENTALITY_WUXING_MAX_LEVEL ||
	//			wx_level != last_level_list[wx_index] + 1)
	//		{
	//			sprintf(errinfo, "%s: xml node error in [config->xisui->data->wuxing_level]", path.c_str());
	//			*err = errinfo;
	//			return false;
	//		}
	//		last_level_list[wx_index] = wx_level;

	//		MentalityWuxingConfig &wx_cfg = m_wuxing_cfglist[wx_index][wx_level];
	//		wx_cfg.wuxing_index = wx_index;
	//		wx_cfg.wuxing_level = wx_level;

	//		if (!PugiGetSubNodeValue(dataElement, "attr_val", wx_cfg.attr_val) || wx_cfg.attr_val <= last_attr_val_list[wx_index])
	//		{
	//			sprintf(errinfo, "%s: xml node error in [config->xisui->data->attr_val]", path.c_str());
	//			*err = errinfo;
	//			return false;
	//		}
	//		last_attr_val_list[wx_index] = wx_cfg.attr_val;

	//		if (!PugiGetSubNodeValue(dataElement, "need_jimai_total_level", wx_cfg.need_total_mentality_level) || wx_cfg.need_total_mentality_level < last_need_total_mentality_level_list[wx_index])
	//		{
	//			sprintf(errinfo, "%s: xml node error in [config->xisui->data->need_jimai_total_level]", path.c_str());
	//			*err = errinfo;
	//			return false;
	//		}
	//		last_need_total_mentality_level_list[wx_index] = wx_cfg.need_total_mentality_level;

	//		if (!PugiGetSubNodeValue(dataElement, "consume_stuff_id", wx_cfg.uplevel_stuff_id) || NULL == ITEMPOOL->GetItem(wx_cfg.uplevel_stuff_id))
	//		{
	//			sprintf(errinfo, "%s: xml node error in [config->xisui->data->consume_stuff_id]", path.c_str());
	//			*err = errinfo;
	//			return false;
	//		}

	//		if (!PugiGetSubNodeValue(dataElement, "consume_stuff_num", wx_cfg.uplevel_stuff_num) || wx_cfg.uplevel_stuff_num < 0)
	//		{
	//			sprintf(errinfo, "%s: xml node error in [config->xisui->data->consume_stuff_num]", path.c_str());
	//			*err = errinfo;
	//			return false;
	//		}

	//		if (!PugiGetSubNodeValue(dataElement, "succ_rate", wx_cfg.succ_rate) || wx_cfg.succ_rate < 0 || wx_cfg.succ_rate > 100)
	//		{
	//			sprintf(errinfo, "%s: xml node error in [config->xisui->data->succ_rate]", path.c_str());
	//			*err = errinfo;
	//			return false;
	//		}

	//		if (!PugiGetSubNodeValue(dataElement, "reduce_rate", wx_cfg.reduce_rate) || wx_cfg.reduce_rate < 0 || wx_cfg.reduce_rate > 100)
	//		{
	//			sprintf(errinfo, "%s: xml node error in [config->xisui->data->reduce_rate]", path.c_str());
	//			*err = errinfo;
	//			return false;
	//		}

	//		dataElement = dataElement.next_sibling();
	//	}

	//	for (int i = 0; i < MENTALITY_WUXING_MAX_COUNT; ++ i)
	//	{
	//		if (MENTALITY_WUXING_MAX_LEVEL != last_level_list[i])
	//		{
	//			sprintf(errinfo, "%s: xml node error in [config->xisui->data->wuxing_level] not enough", path.c_str());
	//			*err = errinfo;
	//			return false;
	//		}

	//		if (0 != m_wuxing_cfglist[i][0].attr_val)
	//		{
	//			sprintf(errinfo, "%s: xml node error in m_wuxing_cfglist[i][0].attr_val must be 0", path.c_str());
	//			*err = errinfo;
	//			return false;
	//		}
	//	}
	//}

	return true;	
}

const MentalityBaseItemConfig * MentalityConfig::GetTrainMentalityBaseItem(int mentality_level)
{
	if (mentality_level >= 0 && mentality_level < MAX_MENTALITY_BASE_LEVEL)
	{
		return &m_mentality_base_config_list[mentality_level];
	}

	return NULL;
}

const MentalityGenguItemConfig * MentalityConfig::GetUpgradeMentalityGenguItem(int gengu_level)
{
	if (gengu_level >= 0 && gengu_level < MAX_MENTALITY_GENGU_LEVEL)
	{
		return &m_mentality_gengu_config_list[gengu_level];
	}

	return NULL;
}

void MentalityConfig::GetMentalityBaseAttr(int mentality_type, int mentality_level, CharIntAttrs &base_add, int prof) const
{
	if (mentality_type < 0 || mentality_type >= MENTALITY_TYPE_MAX ||
		mentality_level <= 0 || mentality_level > MAX_MENTALITY_BASE_LEVEL) return;

	const CharIntAttrs::AttrPair &mentality_attr = m_mentality_base_config_list[mentality_level].all_attrlist[mentality_type];

	if (CharIntAttrs::IsRoleBaseAttr(mentality_attr.attr_type))
	{
//		base_add.m_attrs[mentality_attr.attr_type] += GETPROFVALUE(prof, mentality_attr.attr_type, mentality_attr.attr_value);
		Attribute attr_val = GETPROFVALUE(prof, mentality_attr.attr_type, mentality_attr.attr_value);
		base_add.AddValue(mentality_attr.attr_type, attr_val);
	}
}

void MentalityConfig::GetGenguBaseAttr(int gengu_type, int gengu_level, CharIntAttrs &base_add, int prof) const
{
	if(gengu_type < 0 || gengu_type >= MENTALITY_TYPE_MAX || 
		gengu_level <= 0 || gengu_level > MAX_MENTALITY_GENGU_LEVEL) return;	

	const CharIntAttrs::AttrPair &gengu_attr = m_mentality_gengu_config_list[gengu_level].all_attrlist[gengu_type];
	
	if (CharIntAttrs::IsRoleBaseAttr(gengu_attr.attr_type))
	{
//		base_add.m_attrs[gengu_attr.attr_type] += GETPROFVALUE(prof, gengu_attr.attr_type, gengu_attr.attr_value);
		Attribute attr_val = GETPROFVALUE(prof, gengu_attr.attr_type, gengu_attr.attr_value);
		base_add.AddValue(gengu_attr.attr_type, attr_val);
	}
}

bool MentalityConfig::IsGenguLevelBoardcast(int gengu_level)
{
	if (gengu_level < 0 || gengu_level > MAX_MENTALITY_GENGU_LEVEL) return false;

	for (int i = 0; i < m_gengu_title_count; ++i)
	{
		if (gengu_level == m_gengu_title[i].least_gengu_level) return true;
	}

	return false;
}

void MentalityConfig::GetMentalityTitleAttr(CharIntAttrs &base_add, int least_mentality_level, int prof)
{
	for (int i = m_mentality_title_count - 1; i >= 0 && i < MAX_MENTATITY_TITLE_COUNT_MAX; --i)					// 从高级往低级遍历 遇到第一个满足时结束循环
	{
		if (least_mentality_level >= m_mentality_title[i].least_mentality_level)
		{
			for (int j = 0; j < MENTALITY_TYPE_MAX; j++)
			{
				int attr_type = m_mentality_title[i].mentality_attrlist[j].attr_type;
				if (CharIntAttrs::IsRoleBaseAttr(attr_type))
				{
					long long add_value = m_mentality_title[i].mentality_attrlist[j].attr_value;
					add_value = GETPROFVALUE(prof, attr_type, add_value);

//					base_add.m_attrs[attr_type] += add_value;
					base_add.AddValue(attr_type, add_value);
				}
			}

			return;
		}
	}
}

void MentalityConfig::GetGenguTitleAttr(CharIntAttrs &base_add, int least_gengu_level, int prof)
{
	for (int i = m_gengu_title_count - 1; i >= 0; i--)	// 从高级往低级遍历 遇到第一个满足时结束循环
	{
		if (least_gengu_level >= m_gengu_title[i].least_gengu_level)
		{
			for (int j = 0; j < MENTALITY_TYPE_MAX; j++)
			{
				int attr_type = m_gengu_title[i].gengu_attrlist[j].attr_type;
				if (CharIntAttrs::IsRoleBaseAttr((char)attr_type))
				{
					long long add_value = m_gengu_title[i].gengu_attrlist[j].attr_value;
					add_value = GETPROFVALUE(prof, attr_type, add_value);

//					base_add.m_attrs[attr_type] += add_value;
					base_add.AddValue(attr_type, add_value);
				}
			}

			return;
		}
	}
}

const MentalityWuxingConfig * MentalityConfig::GetWuxingCfg(int wx_index, int wx_level)
{
	if (wx_index >= 0 && wx_index < MENTALITY_WUXING_MAX_COUNT && wx_level >= 0 && wx_level <= MENTALITY_WUXING_MAX_LEVEL)
	{
		return &m_wuxing_cfglist[wx_index][wx_level];
	}

	return NULL;
}


