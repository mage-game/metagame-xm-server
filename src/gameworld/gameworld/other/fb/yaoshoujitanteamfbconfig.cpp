#include "yaoshoujitanteamfbconfig.hpp"
#include <string>
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "monster/monsterpool.h"
#include "protocal/msgfb.h"

YaoShouJiTanTeamFbConfig::YaoShouJiTanTeamFbConfig()
{

}

YaoShouJiTanTeamFbConfig::~YaoShouJiTanTeamFbConfig()
{

}

bool YaoShouJiTanTeamFbConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "YaoShouJiTanTeamFbConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// ÅäÖÃ
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

	return true;
}

int YaoShouJiTanTeamFbConfig::InitModeList(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_mode = -1;
	while (!data_element.empty())
	{
		YaoShouJiTanTeamFBModeConfig cfg_item;

		if (!PugiGetSubNodeValue(data_element, "mode", cfg_item.mode) || cfg_item.mode - last_mode != 1 || cfg_item.mode >= TEAM_FB_MAX_MODE)
		{
			return -1;
		}
		last_mode = cfg_item.mode;

		if (!PugiGetSubNodeValue(data_element, "scene_id", cfg_item.scene_id) || cfg_item.scene_id <= 0)
		{
			return -2;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg_item.scene_id);

		if (!PugiGetSubNodeValue(data_element, "birth_pos_x", cfg_item.enter_pos.x) || cfg_item.enter_pos.x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "birth_pos_y", cfg_item.enter_pos.y) || cfg_item.enter_pos.y <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "free_join_times", cfg_item.free_join_times) || cfg_item.free_join_times <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_id", cfg_item.boss_id) || !MONSTERPOOL->IsMonsterExist(cfg_item.boss_id))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_pos_x", cfg_item.boss_pos.x) || cfg_item.boss_pos.x <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_pos_y", cfg_item.boss_pos.y) || cfg_item.boss_pos.y <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "change_time_gap", cfg_item.change_time_gap) || cfg_item.change_time_gap <= 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_js_per", cfg_item.boss_js_per) || cfg_item.boss_js_per <= 0)
		{
			return -10;
		}
		
		PugiXmlNode Itemelement = data_element.child("reward_item1");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!cfg_item.reward_item1.ReadConfig(Itemelement))
			{
				return -20;
			}
		}

		Itemelement = data_element.child("reward_item2");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!cfg_item.reward_item2.ReadConfig(Itemelement))
			{
				return -21;
			}
		}

		Itemelement = data_element.child("reward_item3");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && itemid > 0)
		{
			if (!cfg_item.reward_item3.ReadConfig(Itemelement))
			{
				return -22;
			}
		}
		
		m_teamfb_mode_vec.push_back(cfg_item);
		data_element = data_element.next_sibling();
	}

	return 0;
}

const YaoShouJiTanTeamFBModeConfig *YaoShouJiTanTeamFbConfig::GetFbModeCfg(int mode)
{
	int size = static_cast<int>(m_teamfb_mode_vec.size());
	if (mode < 0 || mode >= size)
	{
		return NULL;
	}

	return &m_teamfb_mode_vec[mode];
}


int YaoShouJiTanTeamFbConfig::GetModeBySceneId(int scene_id)
{
	int size = static_cast<int>(m_teamfb_mode_vec.size());
	for (int i = 0; i < size; ++ i)
	{
		if (scene_id == m_teamfb_mode_vec[i].scene_id)
		{
			return m_teamfb_mode_vec[i].mode;
		}
	}

	return 0;
}

void YaoShouJiTanTeamFbConfig::GetDailyFindReward(int *item_count, ItemConfigData item_list[DailyFindRewardItem::ITEM_MAX_COUNT], int reward_factor)
{
	if (NULL == item_count) return;
	
	UNSTD_STATIC_CHECK(DailyFindRewardItem::ITEM_MAX_COUNT >= 3);

	const YaoShouJiTanTeamFBModeConfig *mode_cfg = this->GetFbModeCfg(TEAM_FB_MODE_EASY);
	int index = 0;
	if (NULL != mode_cfg)
	{
		item_list[index].item_id = mode_cfg->reward_item1.item_id;
		item_list[index].num = mode_cfg->reward_item1.num * reward_factor;
		item_list[index].is_bind = mode_cfg->reward_item1.is_bind;

		index += 1;
		item_list[index].item_id = mode_cfg->reward_item2.item_id;
		item_list[index].num = mode_cfg->reward_item2.num * reward_factor;
		item_list[index].is_bind = mode_cfg->reward_item2.is_bind;

		index += 1;
		item_list[index].item_id = mode_cfg->reward_item3.item_id;
		item_list[index].num = mode_cfg->reward_item3.num * reward_factor;
		item_list[index].is_bind = mode_cfg->reward_item3.is_bind;

		*item_count = index + 1;
	}
}

