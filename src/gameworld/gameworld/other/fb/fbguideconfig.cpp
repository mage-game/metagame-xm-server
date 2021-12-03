#include "fbguideconfig.hpp"
#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"
#include "item/itempool.h"
#include "task/taskpool.h"

FbGuideConfig::FbGuideConfig()
{
}

FbGuideConfig::~FbGuideConfig()
{
}

bool FbGuideConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "FbGuideConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 副本其他配置
		PugiXmlNode level_element = RootElement.child("guide_list");
		if (level_element.empty())
		{
			*err = configname + ": no [fbguide_cfg].";
			return false;
		}

		iRet = this->InitFbGuideCfg(level_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitFbGuideCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int FbGuideConfig::InitFbGuideCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while(!dataElement.empty())
	{
		FbGuideCfg cfg;

		if (!PugiGetSubNodeValue(dataElement, "task_id", cfg.task_id) || cfg.task_id < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_target_type", cfg.is_target_type))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "target_type", cfg.target_type))
		{
			return -4;
		}

		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);

		{
			cfg.drop_id_list1.clear();

			PugiXmlNode dropid_list_element = dataElement.child("drop_id_list1");
			if (dropid_list_element.empty())
			{
				return -5;
			}

			PugiXmlNode dropid_element = dropid_list_element.child("dropid");
			while (!dropid_element.empty())
			{
				UInt16 dropid = 0;
				if (!PugiGetNodeValue(dropid_element, dropid)) break;	 // 策划可能会配空

				if (0 != dropid && NULL != DROPPOOL->GetDropConfig(dropid))
				{
					cfg.drop_id_list1.push_back(dropid);
				}
				else
				{
					return -6;
				}

				dropid_element = dropid_element.next_sibling();
			}
		}

		{
			cfg.drop_id_list2.clear();

			PugiXmlNode dropid_list_element = dataElement.child("drop_id_list2");
			if (dropid_list_element.empty())
			{
				return -7;
			}

			PugiXmlNode dropid_element = dropid_list_element.child("dropid");
			while (!dropid_element.empty())
			{
				UInt16 dropid = 0;
				if (!PugiGetNodeValue(dropid_element, dropid)) break;	// 策划可能会配空

				if (0 != dropid && NULL != DROPPOOL->GetDropConfig(dropid))
				{
					cfg.drop_id_list2.push_back(dropid);
				}
				else
				{
					return -8;
				}

				dropid_element = dropid_element.next_sibling();
			}
		}

		{
			cfg.drop_id_list3.clear();

			PugiXmlNode dropid_list_element = dataElement.child("drop_id_list3");
			if (dropid_list_element.empty())
			{
				return -9;
			}

			PugiXmlNode dropid_element = dropid_list_element.child("dropid");
			while (!dropid_element.empty())
			{
				UInt16 dropid = 0;
				if (!PugiGetNodeValue(dropid_element, dropid)) break;	// 策划可能会配空

				if (0 != dropid && NULL != DROPPOOL->GetDropConfig(dropid))
				{
					cfg.drop_id_list3.push_back(dropid);
				}
				else
				{
					return -10;
				}

				dropid_element = dropid_element.next_sibling();
			}
		}

		{
			PugiXmlNode  item_list_element = dataElement.child("reward_item_list");
			if (item_list_element.empty()) return -12;

			PugiXmlNode  item_element = item_list_element.child("reward_item");
			if (NULL != item_element)
			{
				int count = 0;
				while (!item_element.empty())
				{
					if (count >= MAX_ATTACHMENT_ITEM_NUM) return -13;

					if (!cfg.reward_item_list[count].ReadConfig(item_element)) break;	// 策划可能会配0

					count++;

					item_element = item_element.next_sibling();
				}
			}
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x", cfg.enter_pos.x) || cfg.enter_pos.x <= 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", cfg.enter_pos.y) || cfg.enter_pos.y <= 0)
		{
			return -16;
		}

		{
			cfg.gather_id_list.clear();

			PugiXmlNode dropid_list_element = dataElement.child("gather_id_list");
			if (dropid_list_element.empty())
			{
				return -17;
			}

			PugiXmlNode dropid_element = dropid_list_element.child("dropid");// 采集物
			while (!dropid_element.empty())
			{
				UInt16 dropid = 0;
				if (!PugiGetNodeValue(dropid_element, dropid)) return -18;

				if (0 != dropid && dropid > 0)
				{
					cfg.gather_id_list.push_back(dropid);
				}
				else
				{
					return -19;
				}

				dropid_element = dropid_element.next_sibling();
			}
		}


		if (!PugiGetSubNodeValue(dataElement, "type", cfg.type) || cfg.type < 0)
		{
			return -20;
		}

		m_fb_guide_cfg_map[cfg.type] = cfg;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const FbGuideCfg * FbGuideConfig::GetFbGuideCfgByTaskId(int type)
{
	if (type <= 0) return NULL;

	std::map<int, FbGuideCfg>::iterator iter = m_fb_guide_cfg_map.find(type);
	if (iter != m_fb_guide_cfg_map.end())
	{
		return &iter->second;
	}

	return NULL;
}

const FbGuideCfg * FbGuideConfig::GetFbGuideCfgBySceneId(int scene_id)
{
	if (scene_id <= 0) return NULL;

	std::map<int, FbGuideCfg>::iterator iter = m_fb_guide_cfg_map.begin();
	for (; iter != m_fb_guide_cfg_map.end(); iter++)
	{
		if (iter->second.scene_id == scene_id)
		{
			return &iter->second;
		}
	}

	return NULL;
}

bool FbGuideConfig::IsLawFulGatherId(int scene_id, int gather_id)
{
	const FbGuideCfg * cfg = this->GetFbGuideCfgBySceneId(scene_id);
	if (NULL == cfg) return false;

	int count = static_cast<int>(cfg->gather_id_list.size());
	for (int i = 0; i < count; i++)
	{
		if (gather_id == cfg->gather_id_list[i])
		{
			return true;
		}
	}

	return false;
}