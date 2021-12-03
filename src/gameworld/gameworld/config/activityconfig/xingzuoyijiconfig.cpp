#include "xingzuoyijiconfig.hpp"

#include "monster/monsterpool.h"
#include "monster/monsterinitparam.h"
#include "item/itempool.h"

#include "servercommon/configcommon.h"
#include <string>
#include "checkresourcecenter.hpp"

XingzuoYiJiConfig::XingzuoYiJiConfig()
{
}

XingzuoYiJiConfig::~XingzuoYiJiConfig()
{
}

bool XingzuoYiJiConfig::Init(const std::string &configname, std::string *err)
{
	char err_info[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "XingzuoYiJiConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + " xml root node error";
		return false;
	}

	{	// ÆäËûÅäÖÃ
		PugiXmlNode root_elment = RootElement.child("other");
		if (NULL == root_elment)
		{
			*err = configname + "xml not other node";
			return false;
		}
		iRet = this->InitXingzuoYijiOtherCfg(root_elment);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitXingzuoYijiOtherCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// BOSSÅäÖÃ
		PugiXmlNode root_elment = RootElement.child("boss");
		if (NULL == root_elment)
		{
			*err = configname + "xml not boss node";
			return false;
		}
		iRet = this->InitXingzuoYijiBossCfg(root_elment);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitXingzuoYijiBossCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// ±¦ÏäÀàÐÍÅäÖÃ
		PugiXmlNode root_elment = RootElement.child("box_type");
		if (NULL == root_elment)
		{
			*err = configname + "xml not box_type node";
			return false;
		}
		iRet = this->InitXingzuoYijiBoxTypeCfg(root_elment);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitXingzuoYijiBoxTypeCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// ±¦ÏäÎïÆ·ÅäÖÃ
		PugiXmlNode root_elment = RootElement.child("box_item");
		if (NULL == root_elment)
		{
			*err = configname + "xml not box_item node";
			return false;
		}
		iRet = this->InitXingzuoYijiBoxItemCfg(root_elment);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitXingzuoYijiBoxItemCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// ±¦ÏäËæ»ú×ø±êÅäÖÃ
		PugiXmlNode root_elment = RootElement.child("box_random_pos");
		if (NULL == root_elment)
		{
			*err = configname + "xml not box_random_pos node";
			return false;
		}
		iRet = this->InitXingzuoYijiBoxRandomPosCfg(root_elment);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitXingzuoYijiBoxRandomPosCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// É¨µ´ÅäÖÃ
		PugiXmlNode root_elment = RootElement.child("skip_cfg");
		if (NULL == root_elment)
		{
			*err = configname + "xml not skip_cfg node";
			return false;
		}
		iRet = this->InitXingzuoYijiSkipCfg(root_elment);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitXingzuoYijiSkipCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int XingzuoYiJiConfig::InitXingzuoYijiOtherCfg(PugiXmlNode RootElement)
{

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if(!PugiGetSubNodeValue(dataElement,"scene_id",m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -1;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.scene_id);

	if (!PugiGetSubNodeValue(dataElement,"relive_pos_x",m_other_cfg.relive_pos.x) || m_other_cfg.relive_pos.x <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement,"relive_pos_y",m_other_cfg.relive_pos.y) || m_other_cfg.relive_pos.y <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement,"refresh_time",m_other_cfg.refresh_time) || m_other_cfg.refresh_time <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement,"common_box_gather_limit",m_other_cfg.common_box_gather_limit) || m_other_cfg.common_box_gather_limit <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement,"great_box_gather_limit",m_other_cfg.great_box_gather_limit) || m_other_cfg.great_box_gather_limit <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement,"common_box_refresh_time",m_other_cfg.common_box_refresh_time) || m_other_cfg.common_box_refresh_time <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement,"boss_disappear_time",m_other_cfg.boss_disappear_time) || m_other_cfg.boss_disappear_time <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement,"boss_max_refresh_times",m_other_cfg.boss_max_refresh_times) || m_other_cfg.boss_max_refresh_times <= 0)
	{
		return -8;
	}

	return 0;
}

int XingzuoYiJiConfig::InitXingzuoYijiBossCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while(!dataElement.empty())
	{
		XingzuoYijiBossCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "world_level_min",cfg.world_level_min) || cfg.world_level_min < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "world_level_max",cfg.world_level_max) || cfg.world_level_max < 0 || cfg.world_level_max < cfg.world_level_min)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_id_0", cfg.monster_id) || cfg.monster_id <= 0 || !MONSTERPOOL->IsMonsterExist(cfg.monster_id))
		{ 
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_id_1", cfg.monster_id_second) || cfg.monster_id_second <= 0 || !MONSTERPOOL->IsMonsterExist(cfg.monster_id_second))
		{ 
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_id_2", cfg.monster_id_third) || cfg.monster_id_third <= 0 || !MONSTERPOOL->IsMonsterExist(cfg.monster_id_third))
		{ 
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement,"boss_pos_x_0",cfg.boss_pos.x) || cfg.boss_pos.x < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement,"boss_pos_y_0",cfg.boss_pos.y) || cfg.boss_pos.y < 0)
		{
			return -7;
		}

		m_boss_cfg_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const XingzuoYijiBoxTypeCfg* XingzuoYiJiConfig::XingzuoYijiGetBoxTypeCfg(int index)
{
	if (index < SENIOR_BOX || index >= MAX_GRADE_BOX)
	{
		return NULL;
	}

	return &m_box_type_cfg[index];
}

int XingzuoYiJiConfig::InitXingzuoYijiBoxTypeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while(!dataElement.empty())
	{
		int gather_index = -1;
		if (!PugiGetSubNodeValue(dataElement, "gather_index", gather_index) || gather_index < 0 || gather_index >= MAX_GRADE_BOX)
		{ 
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_id", m_box_type_cfg[gather_index].gather_id) || m_box_type_cfg[gather_index].gather_id < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_time_s", m_box_type_cfg[gather_index].gather_time_s) || m_box_type_cfg[gather_index].gather_time_s <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_num", m_box_type_cfg[gather_index].gather_num) || m_box_type_cfg[gather_index].gather_num <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_limit_time", m_box_type_cfg[gather_index].gather_limit_time) || m_box_type_cfg[gather_index].gather_limit_time <= 0)
		{
			return -5;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int XingzuoYiJiConfig::InitXingzuoYijiBoxItemCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while(!dataElement.empty())
	{

		XingzuoYijiBoxItemCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "world_level_min",cfg.world_level_min) || cfg.world_level_min < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "world_level_max",cfg.world_level_max) || cfg.world_level_max < 0 || cfg.world_level_max < cfg.world_level_min)
		{
			return -2;
		}

		if (cfg.MAX_BOX_TYPE != MAX_GRADE_BOX)
		{
			return -100;
		}

		PugiXmlNode item_element = dataElement.child("senior_item");
		if (NULL != item_element)
		{
			if (!cfg.item[SENIOR_BOX].ReadConfig(item_element))
			{
				return -3;
			}
		}

		item_element = dataElement.child("medium_item");
		if (NULL != item_element)
		{
			if (!cfg.item[MEDIUM_BOX].ReadConfig(item_element))
			{
				return -4;
			}
		}

		item_element = dataElement.child("junior_item");
		if (NULL != item_element)
		{
			if (!cfg.item[JUNIOR_BOX].ReadConfig(item_element))
			{
				return -5;
			}
		}

		item_element = dataElement.child("molong_item");
		if (NULL != item_element)
		{
			if (!cfg.item[MOLONG_BOX].ReadConfig(item_element))
			{
				return -6;
			}
		}

		m_box_item_cfg_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int XingzuoYiJiConfig::InitXingzuoYijiBoxRandomPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while(!dataElement.empty())
	{
		Posi pos;

		if (!PugiGetSubNodeValue(dataElement, "gather_pox_x", pos.x) || pos.x < 0)
		{ 
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "gather_pox_y", pos.y) || pos.y < 0)
		{
			return -2;
		}

		m_box_pos.push_back(pos);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int XingzuoYiJiConfig::InitXingzuoYijiSkipCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int quality = -1;
		if (!PugiGetSubNodeValue(dataElement, "quality", quality) || quality < SENIOR_BOX || quality > MEDIUM_BOX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "limit_level", m_skip_cfg[quality].limit_level) || m_skip_cfg[quality].limit_level < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "consume", m_skip_cfg[quality].consume) || m_skip_cfg[quality].consume <= 0)
		{
			return -3;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const ItemConfigData* XingzuoYiJiConfig::XingzuoYijiGetBoxItem(int world_level, int box_type)
{
	if (world_level < 0 || world_level > MAX_ROLE_LEVEL)
	{
		return NULL;
	}

	if (box_type < 0 || box_type >= MAX_GRADE_BOX)
	{
		return NULL;
	}

	std::vector<XingzuoYijiBoxItemCfg>::iterator it = m_box_item_cfg_vec.begin();
	for (; it != m_box_item_cfg_vec.end(); ++it)
	{
		if (world_level < it->world_level_min || world_level > it->world_level_max)
		{
			continue;
		}

		return &(it->item[box_type]);
	}

	return NULL;
}

const XingzuoYijiBossCfg* XingzuoYiJiConfig::XingzuoYijiGetBossCfg(int world_level)
{
	if (world_level < 0 || world_level > MAX_ROLE_LEVEL)
	{
		return NULL;
	}

	std::vector<XingzuoYijiBossCfg>::iterator it = m_boss_cfg_vec.begin();
	for (; it != m_boss_cfg_vec.end(); ++it)
	{
		XingzuoYijiBossCfg &cfg = *it;
		if (world_level < cfg.world_level_min || world_level > cfg.world_level_max)
		{
			continue;
		}

		return &cfg;
	}

	return NULL;
}

const XingzuoYijiSkipCfg* XingzuoYiJiConfig::XingzuoYijiGetSkipCfg(int quality)
{
	if (quality < SENIOR_BOX || quality > MEDIUM_BOX)
	{
		return NULL;
	}

	return &m_skip_cfg[quality];
}

const int XingzuoYiJiConfig::XingzuoYijiGetBoxType(int gather_id)
{
	for (int box_type = 0; box_type < MAX_GRADE_BOX; ++box_type)
	{
		if (m_box_type_cfg[box_type].gather_id == gather_id)
		{
			return box_type;
		}
	}

	return -1;
}

std::vector<Posi> &XingzuoYiJiConfig::XingzuoYijiGetRandomPos()
{
	int size = static_cast<int>(m_box_pos.size());

	for (int i = 0; i < size; ++i)
	{
		int rand_num = RandomNum(size);

		Posi temp = m_box_pos[i];
		m_box_pos[i] = m_box_pos[rand_num];
		m_box_pos[rand_num] = temp;
	}

	return m_box_pos;
}