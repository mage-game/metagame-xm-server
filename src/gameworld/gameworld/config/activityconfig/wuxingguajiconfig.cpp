#include "wuxingguajiconfig.hpp"

#include "gamecommon.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "world.h"
#include "scene/scene.h"
#include "scene/map.h"
#include <string>

#include "scene/worldshadow/worldshadow.hpp"

#include "item/itempool.h"

WuXingGuaJiConfig::WuXingGuaJiConfig()
	: m_open_level(0), m_sceneid(0), m_enter_pos(0, 0), m_lucky_interval(0)
{

}

WuXingGuaJiConfig::~WuXingGuaJiConfig()
{

}

bool WuXingGuaJiConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "WuXingGuaJiConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{																					// ¶ÁÈ¡other
		PugiXmlNode otherElem = RootElement.child("other_config");
		if (otherElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->other]", path.c_str());
			*err =  errinfo;
			return false;
		}

		PugiXmlNode dataElem = otherElem.child("data");
		if (dataElem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data]", path.c_str());
			*err =  errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(dataElem, "open_level", m_open_level) || m_open_level <= 0)
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data->m_open_level: %d ]", path.c_str(), m_open_level);
			*err = errinfo;
			return false;
		}

		if (!PugiGetSubNodeValue(dataElem, "sceneid", m_sceneid) || m_sceneid <= 0)
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data->sceneid: %d ]", path.c_str(), m_sceneid);
			*err = errinfo;
			return false;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(m_sceneid);	

		{
			int pos_x = 0;
			if (!PugiGetSubNodeValue(dataElem, "enter_pos_x", pos_x) || pos_x <= 0)
			{
				sprintf(errinfo,"%s:xml node error in node [config->other->data->pos_x: %d ]", path.c_str(), pos_x);
				*err = errinfo;
				return false;
			}

			int pos_y = 0;
			if (!PugiGetSubNodeValue(dataElem, "enter_pos_y", pos_y) || pos_y <= 0)
			{
				sprintf(errinfo,"%s:xml node error in node [config->other->data->pos_y: %d ]", path.c_str(), pos_y);
				*err = errinfo;
				return false;
			}

			m_enter_pos = Posi(pos_x, pos_y);
		}

		if (!PugiGetSubNodeValue(dataElem, "lucky_interval", m_lucky_interval) || m_lucky_interval <= 0)
		{
			sprintf(errinfo,"%s:xml node error in node [config->other->data->m_lucky_interval: %d ]", path.c_str(), m_lucky_interval);
			*err = errinfo;
			return false;
		}

		PugiXmlNode LuckyItemElement = dataElem.child("lucky_item");
		if (LuckyItemElement.empty())
		{
			*err = path + ": xml node error in [config->lucky_item]";
			return false;
		}

		if (!m_lucky_item.ReadConfig(LuckyItemElement))
		{
			*err = path + ": xml node error in [config->lucky_item]";
			return false;
		}
	}

	{
		PugiXmlNode target_listElement = RootElement.child("target_config");
		if (target_listElement.empty())
		{
			*err = path + ": xml node error in [config->target_config]";
			return false;
		}

		PugiXmlNode DataElement = target_listElement.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml node error in [config->target_config->data]";
			return false;
		}

		int last_target_index = -1;
		while (!DataElement.empty())
		{
			int target_index = 0;
			if (!PugiGetSubNodeValue(DataElement, "target_index", target_index) || target_index < 0 || target_index >= WUXINGGUAJI_TARGET_MAX || target_index != last_target_index + 1)
			{
				sprintf(errinfo,"%s:xml node error in node [config->other->data->target_index: %d ]", path.c_str(), target_index);
				*err = errinfo;
				return false;
			}
			last_target_index = target_index;

			WuXingGuaJiTargetCfg &target_cfg = m_target_cfglist[target_index];

			char tmp_tag[32] = {0};
			for (int i = 0; i < WUXINGGUAJI_STUFF_MAX; i++)
			{
				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "stuff_itemid_%d", i + 1);

				int item_id = 0;
				if (!PugiGetSubNodeValue(DataElement, tmp_tag, item_id) || item_id < 0)
				{
					sprintf(errinfo,"%s:xml node error in node [config->other->data->item_id: %d ]", path.c_str(), item_id);
					*err = errinfo;
					return false;
				}
				if (item_id > 0 && NULL == ITEMPOOL->GetItem(item_id))
				{
					sprintf(errinfo,"%s:xml node error in node [config->other->data->item_id: %d not exist]", path.c_str(), item_id);
					*err = errinfo;
					return false;
				}

				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "stuff_count_%d", i + 1);

				int item_count = 0;
				if (!PugiGetSubNodeValue(DataElement, tmp_tag, item_count) || item_count < 0)
				{
					sprintf(errinfo,"%s:xml node error in node [config->other->data->item_count: %d ]", path.c_str(), item_count);
					*err = errinfo;
					return false;
				}

				target_cfg.need_stuff_list[i].stuff_itemid = item_id;
				target_cfg.need_stuff_list[i].need_count = item_count;
			}

			PugiXmlNode RewardItemElement = DataElement.child("reward_item");
			if (RewardItemElement.empty())
			{
				*err = path + ": xml node error in [config->reward_item]";
				return false;
			}

			if (!target_cfg.reward_cfg.ReadConfig(RewardItemElement))
			{
				*err = path + ": xml node error in [config->reward_item]";
				return false;
			}

			DataElement = DataElement.next_sibling();
		}

		{
			PugiXmlNode BossCfgListElem = RootElement.child("boss_cfg");
			if (BossCfgListElem.empty())
			{
				sprintf(errinfo,"%s:xml node error in node [config->boss_cfg]", path.c_str());
				*err =  errinfo;
				return false;
			}

			PugiXmlNode dataElem = BossCfgListElem.child("data");
			if (dataElem.empty())
			{
				sprintf(errinfo,"%s:xml node error in node [config->boss_cfg->data]", path.c_str());
				*err =  errinfo;
				return false;
			}

			while (!dataElem.empty())
			{
				int boss_index = 0;
				if (!PugiGetSubNodeValue(dataElem, "boss_index", boss_index) || boss_index < 0 || boss_index >= WUXINGGUAJI_BOSS_NUM)
				{
					sprintf(errinfo,"%s:xml node error in node [config->boss_reward->data->boss_index]", path.c_str());
					*err = errinfo;
					return false;
				}

				WuXingGuaJiBossCfg &boss_cfg = m_boss_cfg_list[boss_index];

				if (boss_cfg.replace_boss_count >= WUXINGGUAJI_MAX_RELACE_BOSS_NUM)
				{
					sprintf(errinfo,"%s:xml node error in node [config->boss_reward->data->boss_index] more than limit", path.c_str());
					*err = errinfo;
					return false;
				}

				WuXingGuaJiRelaceBossCfg &replace_boss_cfg = boss_cfg.replace_boss_list[boss_cfg.replace_boss_count];

				if (!PugiGetSubNodeValue(dataElem, "monster_id", replace_boss_cfg.replace_monster_id) || !MONSTERPOOL->IsMonsterExist(replace_boss_cfg.replace_monster_id))
				{
					sprintf(errinfo,"%s:xml node error in node [config->boss_reward->data->monster_id]", path.c_str());
					*err = errinfo;
					return false;
				}

				if (!PugiGetSubNodeValue(dataElem, "limitLev", replace_boss_cfg.replace_need_world_level) || replace_boss_cfg.replace_need_world_level < 0)
				{
					sprintf(errinfo,"%s:xml node error in node [config->boss_reward->data->limitLev]", path.c_str());
					*err = errinfo;
					return false;
				}

				if (!PugiGetSubNodeValue(dataElem, "pos_x", replace_boss_cfg.flush_pos.x) || replace_boss_cfg.flush_pos.x <= 0)
				{
					sprintf(errinfo,"%s:xml node error in node [config->boss_reward->data->pos_x]", path.c_str());
					*err = errinfo;
					return false;
				}

				if (!PugiGetSubNodeValue(dataElem, "pos_y", replace_boss_cfg.flush_pos.y) || replace_boss_cfg.flush_pos.y <= 0)
				{
					sprintf(errinfo,"%s:xml node error in node [config->boss_reward->data->pos_y]", path.c_str());
					*err = errinfo;
					return false;
				}

				++ boss_cfg.replace_boss_count;
				dataElem = dataElem.next_sibling();
			}
		}
	}

	return true;
}

int WuXingGuaJiConfig::GetEnterInfo(Posi *enter_pos)
{
	if (NULL != enter_pos) *enter_pos = m_enter_pos;
	return m_sceneid;
}

bool WuXingGuaJiConfig::GetBossCfg(int boss_index, int *monster_id, Posi *flush_pos)
{
	if (boss_index < 0 || boss_index >= WUXINGGUAJI_BOSS_NUM || NULL == monster_id || NULL == flush_pos)
	{
		return false;
	}

	for (int i = m_boss_cfg_list[boss_index].replace_boss_count - 1; i >= 0 && i < WUXINGGUAJI_MAX_RELACE_BOSS_NUM; i--)
	{
		WuXingGuaJiRelaceBossCfg &replace_boss_cfg = m_boss_cfg_list[boss_index].replace_boss_list[i];
		if (WorldShadow::Instance().GetWorldLevel() >= replace_boss_cfg.replace_need_world_level)
		{
			*flush_pos = replace_boss_cfg.flush_pos;
			*monster_id = replace_boss_cfg.replace_monster_id;
			return true;
		}
	}

	return false;
}

WuXingGuaJiTargetCfg * WuXingGuaJiConfig::GetWuXingGuaJiTargetCfg(int target_idx)
{
	if (target_idx < 0 || target_idx >= WUXINGGUAJI_TARGET_MAX)
	{
		return NULL;
	}

	return &m_target_cfglist[target_idx];
}

bool WuXingGuaJiConfig::CheckTargetIsFinish(int target_idx, WuXingGuaJiTargetInfo *target_info)
{
	if (target_idx < 0 || target_idx >= WUXINGGUAJI_TARGET_MAX || NULL == target_info)
	{
		return false;
	}

	WuXingGuaJiTargetCfg *target_cfg = &m_target_cfglist[target_idx];

	for (int i = 0; i < WUXINGGUAJI_STUFF_MAX; i++)
	{
		if (target_cfg->need_stuff_list[i].stuff_itemid > 0)
		{
			if (target_info->stuff_num_list[i] < target_cfg->need_stuff_list[i].need_count)
			{
				return false;
			}
		}
	}

	return true;
}




