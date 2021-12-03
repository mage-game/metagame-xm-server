#include "dropextendconfig.hpp"
#include "item/itempool.h"
#include "item/itembase.h"
#include "monster/drop/droppool.hpp"

DropExtendConfig::DropExtendConfig(): m_collect_drop_cut_count(0)
{

}

DropExtendConfig::~DropExtendConfig()
{

}

bool DropExtendConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "DropExtendConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("extra_drop_list");
		if (root_element.empty())
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitExtraDropConfig(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitExtraDropConfig fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("collect_extra_drop_list");
		if (root_element.empty())
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitCollectExtraDropConfig(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitCollectExtraDropConfig fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitOtherConfig(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOtherConfig fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("collect_extra_drop_cut");
		if (root_element.empty())
		{
			*err = path + " xml not collect_extra_drop_cut node ";
			return false;
		}

		iRet = this->InitCollectCutConfig(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitCollectCutConfig fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int DropExtendConfig::InitExtraDropConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_max_level = 0;

	while (!dataElement.empty())
	{
 		ExtraDropItemCfg item_cfg;

 		if (!PugiGetSubNodeValue(dataElement, "monster_min_level", item_cfg.monster_min_level) || item_cfg.monster_min_level < 0 || (last_max_level != 0 && item_cfg.monster_min_level <= last_max_level))
 		{
 			return -1;
 		}
 		
 		if (!PugiGetSubNodeValue(dataElement, "monster_max_level", item_cfg.monster_max_level) || item_cfg.monster_max_level < item_cfg.monster_min_level)
 		{
 			return -2;
 		}
		last_max_level = item_cfg.monster_max_level;
 
		char str[256] = {0};
		
		item_cfg.drop_count = 0;
		bool has_empty_dropid = false;

		for (int i = 0; i < EXTRA_DROP_MAX_NUM; ++ i)
		{
			memset(str, 0, sizeof(str));
			sprintf(str, "dropid%d", i + 1);
			
			int drop_id = 0;
			if (!PugiGetSubNodeValue(dataElement, str, drop_id) || drop_id < 0 || 
				(drop_id > 0 && NULL == DROPPOOL->GetDropConfig(drop_id)) ||
				(drop_id > 0 && has_empty_dropid))
			{
				return -12;
			}

			if (0 == drop_id)
			{
				has_empty_dropid = true;
			}

			else if (drop_id > 0)
			{
				item_cfg.dropid_list[item_cfg.drop_count] = drop_id;
				++ item_cfg.drop_count;
			}

		}

 		m_extra_drop_vec.push_back(item_cfg);
 
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int DropExtendConfig::InitCollectExtraDropConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_max_level = 0;

	while (!dataElement.empty())
	{
		ExtraDropItemCfg item_cfg;

		if (!PugiGetSubNodeValue(dataElement, "monster_min_level", item_cfg.monster_min_level) || item_cfg.monster_min_level < 0 || (last_max_level != 0 && item_cfg.monster_min_level <= last_max_level))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "monster_max_level", item_cfg.monster_max_level) || item_cfg.monster_max_level < item_cfg.monster_min_level)
		{
			return -2;
		}
		last_max_level = item_cfg.monster_max_level;

		char str[256] = {0};

		item_cfg.drop_count = 0;
		bool has_empty_dropid = false;

		for (int i = 0; i < EXTRA_DROP_MAX_NUM; ++ i)
		{
			memset(str, 0, sizeof(str));
			sprintf(str, "dropid%d", i + 1);

			int drop_id = 0;
			if (!PugiGetSubNodeValue(dataElement, str, drop_id) || drop_id < 0 || 
				(drop_id > 0 && NULL == DROPPOOL->GetDropConfig(drop_id)) ||
				(drop_id > 0 && has_empty_dropid))
			{
				return -12;
			}

			if (0 == drop_id)
			{
				has_empty_dropid = true;
			}

			else if (drop_id > 0)
			{
				item_cfg.dropid_list[item_cfg.drop_count] = drop_id;
				++ item_cfg.drop_count;
			}

		}

		m_collect_extra_drop_vec.push_back(item_cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int DropExtendConfig::InitOtherConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	if (!PugiGetSubNodeValue(dataElement, "extradrop_time_gap", m_other_cfg.extradrop_time_gap) || m_other_cfg.extradrop_time_gap <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "extradrop_distance", m_other_cfg.extradrop_distance) || m_other_cfg.extradrop_distance <= 0)
	{
		return -2;
	}

	return 0;
}

int DropExtendConfig::InitCollectCutConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	m_collect_drop_cut_count = 0;
	int last_drop_num = 0;
	int last_drop_per = 0;

	while (!dataElement.empty())
	{
		if (m_collect_drop_cut_count < 0 || m_collect_drop_cut_count >= EXTRA_DROP_COLLECT_CUT_MAX_NUM)
		{
			return -1;
		}

		DropExtendCollectCutDownCfg &cfg = m_collect_drop_cut_list[m_collect_drop_cut_count];

		if (!PugiGetSubNodeValue(dataElement, "drop_num", cfg.drop_num) || cfg.drop_num <= 0 || (last_drop_num != 0 && cfg.drop_num >= last_drop_num))
		{
			return -2;
		}

		last_drop_num = cfg.drop_num;

		if (!PugiGetSubNodeValue(dataElement, "drop_per", cfg.drop_per) || cfg.drop_per < 0 || (last_drop_per != 0 && cfg.drop_per <= last_drop_per))
		{
			return -3;
		}

		last_drop_per = cfg.drop_per;

		m_collect_drop_cut_count++;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}


int DropExtendConfig::GetRandomExtraDropId(int monster_level)
{
	for(ExtraDropVec::const_iterator iter = m_extra_drop_vec.begin(); m_extra_drop_vec.end() != iter; ++ iter)
	{
		if (monster_level >= iter->monster_min_level && monster_level <= iter->monster_max_level && iter->drop_count)
		{
			int index = RandomNum(iter->drop_count);
			if (index >= 0 && index < EXTRA_DROP_MAX_NUM)
			{
				return iter->dropid_list[index];
			}
		}
	}

	return 0;
}

int DropExtendConfig::GetRandomCollectExtraDropId(int monster_level)
{
	for(CollectExtraDropVec::const_iterator iter = m_collect_extra_drop_vec.begin(); m_collect_extra_drop_vec.end() != iter; ++ iter)
	{
		if (monster_level >= iter->monster_min_level && monster_level <= iter->monster_max_level && iter->drop_count)
		{
			int index = RandomNum(iter->drop_count);
			if (index >= 0 && index < EXTRA_DROP_MAX_NUM)
			{
				return iter->dropid_list[index];
			}
		}
	}

	return 0;
}

int DropExtendConfig::GetRandomCollectCutPer(int drop_num)
{
	for (int i = 0; i < m_collect_drop_cut_count && i < EXTRA_DROP_COLLECT_CUT_MAX_NUM; ++i)
	{
		DropExtendCollectCutDownCfg&cfg = m_collect_drop_cut_list[i];
		if (drop_num >= cfg.drop_num)
		{
			return cfg.drop_per;
		}
	}

	return 10000;
}

const DropExtendCollectCutDownCfg *DropExtendConfig::GetRandomCollectCutCfg(int drop_num)
{
	for (int i = 0; i < m_collect_drop_cut_count && i < EXTRA_DROP_COLLECT_CUT_MAX_NUM; ++i)
	{
		if (drop_num >= m_collect_drop_cut_list[i].drop_num)
		{
			return &m_collect_drop_cut_list[i];
		}
	}

	return nullptr;
}
