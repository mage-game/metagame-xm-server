#include "migongxianfuconfig.hpp"
#include <string>
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "monster/monsterpool.h"
#include "protocal/msgfb.h"

MiGongXianFuTeamFbConfig::MiGongXianFuTeamFbConfig()
{

}

MiGongXianFuTeamFbConfig::~MiGongXianFuTeamFbConfig()
{

}

bool MiGongXianFuTeamFbConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "MiGongXianFuTeamFbConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("mode_list");
		if (root_element.empty())
		{
			*err = path + " xml not reward node ";
			return false;
		}

		iRet = this->InitModeList(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitModeList fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("layer_list");
		if (root_element.empty())
		{
			*err = path + " xml not reward node ";
			return false;
		}

		iRet = this->InitLayerList(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitLayerList fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int MiGongXianFuTeamFbConfig::InitModeList(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_mode = -1;
	while (!data_element.empty())
	{
		int mode = 0;
		if (!PugiGetSubNodeValue(data_element, "mode", mode) || mode - last_mode != 1 || mode >= TEAM_FB_MAX_MODE)
		{
			return -1;
		}
		last_mode = mode;

		MiGongXianFuModeItemCfg &mode_item = m_mode_map[mode];
		mode_item.mode = mode;

		if (!PugiGetSubNodeValue(data_element, "free_join_times", mode_item.free_join_times) || mode_item.free_join_times <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "time", mode_item.time) || mode_item.time <= 0)
		{
			return -3;
		}
		
		// 奖励物品
		PugiXmlNode Itemelement = data_element.child("reward_item1");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!mode_item.reward_item1.ReadConfig(Itemelement))
			{
				return -20;
			}
		}

		Itemelement = data_element.child("reward_item2");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!mode_item.reward_item2.ReadConfig(Itemelement))
			{
				return -21;
			}
		}

		Itemelement = data_element.child("reward_item3");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!mode_item.reward_item3.ReadConfig(Itemelement))
			{
				return -22;
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}


int MiGongXianFuTeamFbConfig::InitLayerList(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_mode = -1;
	int last_layer = -1;
	int normal_layer_num = 0;
	while (!data_element.empty())
	{
		int mode = 0;
		int layer = 0;

		if (!PugiGetSubNodeValue(data_element, "mode", mode) || mode - last_mode != 1 || mode >= TEAM_FB_MAX_MODE)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "layer", layer) || layer - last_layer != 1 || layer >= MIGONGXIANFU_LAYER_NUM)
		{
			return -2;
		}
		last_layer = layer;


		MiGongXianFuModeItemCfg &mode_item = m_mode_map[mode];
		mode_item.mode = mode;

		MiGongXianFuLayerItemCfg &layer_item = mode_item.layer_list[layer];
		layer_item.layer = layer;
		layer_item.mode = mode;

		if (!PugiGetSubNodeValue(data_element, "layer_type", layer_item.layer_type))
		{
			return -20;
		}

		{
			if (MGXF_LAYER_TYPE_NORMAL != layer_item.layer_type && MGXF_LAYER_TYPE_HIDE != layer_item.layer_type && MGXF_LAYER_TYPE_BOSS != layer_item.layer_type)
			{
				return -21;
			}
			
			if (MGXF_LAYER_TYPE_HIDE == layer_item.layer_type && MIGONGXIANFU_HIDE_LAYER_INDEX != layer_item.layer)
			{
				return -22;
			}

			if (MGXF_LAYER_TYPE_BOSS == layer_item.layer_type && MIGONGXIANFU_BOSS_LAYER_INDEX != layer_item.layer)
			{
				return -23;
			}

			if (MGXF_LAYER_TYPE_NORMAL == layer_item.layer_type)
			{
				++ normal_layer_num;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "scene_id", layer_item.scene_id) || layer_item.scene_id <= 0)
		{
			return -2;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(layer_item.scene_id);

 
		if (!PugiGetSubNodeValue(data_element, "enter_pos_x", layer_item.enter_pos.x) || layer_item.enter_pos.x <= 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(data_element, "enter_pos_y", layer_item.enter_pos.y) || layer_item.enter_pos.y <= 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_id", layer_item.boss_id) || (layer_item.boss_id != 0 && !MONSTERPOOL->IsMonsterExist(layer_item.boss_id)))
		{
			return -3;
		}
		
		if ((MGXF_LAYER_TYPE_HIDE == layer_item.layer_type || MGXF_LAYER_TYPE_BOSS == layer_item.layer_type) && 0 == layer_item.boss_id) //这两层不能没有boss
		{
			return -33;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_pos_x", layer_item.boss_pos.x) || layer_item.boss_pos.x < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_pos_y", layer_item.boss_pos.y) || layer_item.boss_pos.y < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "to_prve_weight", layer_item.to_prve_weight) || layer_item.to_prve_weight < 0)
		{
			return -6;
		}

		if (MIGONGXIANFU_LAYER_FIRST_INDEX == layer && layer_item.to_prve_weight != 0) //确保第一层没有去上一层的概率
		{
			return -66;
		}

		if (!PugiGetSubNodeValue(data_element, "to_hide_weight", layer_item.to_hide_weight) || layer_item.to_hide_weight < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "to_next_weight", layer_item.to_next_weight) || layer_item.to_next_weight < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "to_here_weight", layer_item.to_here_weight) || layer_item.to_here_weight < 0)
		{
			return -9;
		}

		//传送点位置
		{
			char tmp_tag[32] = {0};
			for (int i = 0; i < MIGONGXIANFU_DOOR_NUM; ++ i)
			{
				Posi &door = layer_item.door_list[i];
				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "door%d_x", i + 1);

				if (!PugiGetSubNodeValue(data_element, tmp_tag, door.x) || door.x < 0)
				{
					return -20;
				}

				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "door%d_y", i + 1);

				if (!PugiGetSubNodeValue(data_element, tmp_tag, door.y) || door.y < 0)
				{
					return -21;
				}
			}
		}

		if (MIGONGXIANFU_LAYER_NUM == layer + 1)
		{
			if (normal_layer_num != MIGONGXIANFU_NORMAL_LAYER_NUM)
			{
				return -999;
			}
			normal_layer_num = 0;
			last_mode = mode;
			last_layer = -1;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}


const MiGongXianFuModeItemCfg *MiGongXianFuTeamFbConfig::GetModeItemCfg(int mode)
{
	std::map<int, MiGongXianFuModeItemCfg>::const_iterator iter = m_mode_map.find(mode);
	if (m_mode_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

const MiGongXianFuLayerItemCfg *MiGongXianFuTeamFbConfig::GetLayerItemCfgBySceneId(int scene_id)
{
	std::map<int, MiGongXianFuModeItemCfg>::const_iterator iter;
	for (iter = m_mode_map.begin(); m_mode_map.end() != iter; ++ iter)
	{
		for (int i = 0; i < MIGONGXIANFU_LAYER_NUM; ++ i)
		{
			if (iter->second.layer_list[i].scene_id == scene_id)
			{
				return &iter->second.layer_list[i];
			}
		}
	}

	return NULL;
}

const MiGongXianFuLayerItemCfg *MiGongXianFuTeamFbConfig::GetLayerItemCfg(int mode, int layer)
{
	const MiGongXianFuModeItemCfg *mode_cfg = this->GetModeItemCfg(mode);
	if (NULL == mode_cfg)
	{
		return NULL;
	}

	if (layer < 0 || layer >= MIGONGXIANFU_LAYER_NUM)
	{
		return NULL;
	}

	return &mode_cfg->layer_list[layer];
}

void MiGongXianFuTeamFbConfig::GetDailyFindReward(int *item_count, ItemConfigData item_list[DailyFindRewardItem::ITEM_MAX_COUNT], int reward_factor)
{
	if (NULL == item_count) return;

	UNSTD_STATIC_CHECK(DailyFindRewardItem::ITEM_MAX_COUNT >= 3);

	const MiGongXianFuModeItemCfg *mode_cfg = this->GetModeItemCfg(TEAM_FB_MODE_EASY);
	int index = 0;
	if (NULL != mode_cfg)
	{
		item_list[index].item_id = mode_cfg->reward_item1.item_id;
		item_list[index].num = mode_cfg->reward_item1.num;
		item_list[index].is_bind = mode_cfg->reward_item1.is_bind;

		index += 1;
		item_list[index].item_id = mode_cfg->reward_item2.item_id;
		item_list[index].num = mode_cfg->reward_item2.num;
		item_list[index].is_bind = mode_cfg->reward_item2.is_bind;

		index += 1;
		item_list[index].item_id = mode_cfg->reward_item3.item_id;
		item_list[index].num = mode_cfg->reward_item3.num;
		item_list[index].is_bind = mode_cfg->reward_item3.is_bind;

		*item_count = index + 1;
	}
}
