#include "fbtuituconfig.h"
#include "servercommon/configcommon.h"
#include "gameworld/gameworld/item/itempool.h"
#include "gameworld/monster/monsterpool.h"

TuituFbConfig::TuituFbConfig()
{
	
}

TuituFbConfig::~TuituFbConfig()
{
	
}

bool TuituFbConfig::Init(const std::string &path, std::string *err)
{
	char err_info[512] = { 0 };
	int iRet = 0;

	PugiXmlDocument doc;
	PUGI_XML_LOAD_AND_CHECK(doc, path, "TuituFbConfig", *err);

	PugiXmlNode RootElement = doc.document_element();
	if (RootElement.empty())
	{
		*err = path + "xml no root node.\n";
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + "xml no other node.\n";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(err_info, "%s InitOtherCfg failed [%d]", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("fb_info");
		if (root_element.empty())
		{
			*err = path + "xml no fb_info node.\n";
			return false;
		}

		iRet = this->InitFbInfoCfg(root_element);
		if (iRet)
		{
			sprintf(err_info, "%s InitFbInfoCfg failed [%d]", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("star_reward");
		if (root_element.empty())
		{
			*err = path + "xml no star_reward node.\n";
			return false;
		}

		iRet = this->InitStarRewardCfg(root_element);
		if (iRet)
		{
			sprintf(err_info, "%s InitStarRewardCfg failed [%d]", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

const TuituFbInfoConfig * TuituFbConfig::GetFbinfoCfg(int fb_type)
{
	if (fb_type < 0 || fb_type >= TUITU_FB_TYPE_MAX)
	{
		return NULL;
	}

	return &m_fb_info_cfg_list[fb_type];
}

const TuituFbInfoConfig::Layer * TuituFbConfig::GetLayerCfg(int fb_type, int chapter, int level)
{
	if (fb_type < 0 || fb_type >= TUITU_FB_TYPE_MAX || chapter < 0 || chapter >= TUITU_FB_CHAPER_NUM_MAX || level < 0 || level >= TUITU_FB_LEVLE_NUM_MAX)
	{
		return NULL;
	}

	if (chapter > m_fb_info_cfg_list[fb_type].max_chapter || chapter >= TUITU_FB_CHAPER_NUM_MAX)
	{
		return NULL;
	}

	if (level > m_fb_info_cfg_list[fb_type].chapter_cfg_list[chapter].max_level || level >= TUITU_FB_LEVLE_NUM_MAX)
	{
		return NULL;
	}

	 return &m_fb_info_cfg_list[fb_type].chapter_cfg_list[chapter].layer_cfg_list[level];
}

const TuituFbStarRewardConfig::ItemCfg * TuituFbConfig::GetStarRewardCfg(int fb_type, int chapter, int seq)
{
	if (fb_type < 0 || fb_type >= TUITU_FB_TYPE_MAX || chapter < 0 || chapter >= TUITU_FB_CHAPER_NUM_MAX
		|| seq < 0 || seq > TUITU_FB_CHAPTER_STAR_REWARD_NUM_MAX)
	{
		return NULL;
	}

	TuituFbStarRewardConfig &star_reward_cfg = m_star_reward_cfg_list[fb_type][chapter];
	if (star_reward_cfg.chapter != chapter || seq >= star_reward_cfg.item_cfg_count)
	{
		return NULL;
	}

	return &star_reward_cfg.item_cfg_list[seq];
}

int TuituFbConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return - 1000;
	}
	
	if (!PugiGetSubNodeValue(data_element, "normal_free_join_times", m_other_cfg.normal_free_join_times) || m_other_cfg.normal_free_join_times <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "hard_free_join_times", m_other_cfg.hard_free_join_times) || m_other_cfg.hard_free_join_times <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "normal_buy_times_need_gold", m_other_cfg.normal_buy_times_need_gold) || m_other_cfg.normal_buy_times_need_gold <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "hard_buy_times_need_gold", m_other_cfg.hard_buy_times_need_gold) || m_other_cfg.hard_buy_times_need_gold <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "saodang_quan_itemid", m_other_cfg.saodang_quan_itemid) || NULL == ITEMPOOL->GetItem(m_other_cfg.saodang_quan_itemid))
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "inchurt_per", m_other_cfg.inchurt_per) || m_other_cfg.inchurt_per <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "mianshang_per", m_other_cfg.mianshang_per) || m_other_cfg.mianshang_per <= 0)
	{
		return -7;
	}
	
	return 0;
}

int TuituFbConfig::InitFbInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int fb_type = 0;
	int chapter = 0;
	int level = 0;

	char tmp_monster_id[64] = { 0 };
	char tmp_monster_pos_x[32] = { 0 };
	char tmp_monster_pos_y[32] = { 0 };

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "fb_type", fb_type) || fb_type < 0 || fb_type >= TUTUI_FB_TYPE_NUM_MAX)
		{
			return -1;
		}

		TuituFbInfoConfig &fb_info_cfg = m_fb_info_cfg_list[fb_type];

		if (!PugiGetSubNodeValue(data_element, "chapter", chapter) || chapter < 0 || chapter >= TUITU_FB_CHAPER_NUM_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0 || level >= TUITU_FB_LEVLE_NUM_MAX)
		{
			return -3;
		}

		if (fb_info_cfg.max_chapter != chapter && chapter != fb_info_cfg.max_chapter + 1)
		{
			return -4;
		}

		if (level != fb_info_cfg.chapter_cfg_list[chapter].max_level + 1)
		{
			return -5;
		}

		fb_info_cfg.max_chapter = chapter;
		fb_info_cfg.chapter_cfg_list[chapter].max_level = level;

		TuituFbInfoConfig::Layer &layer_cfg = fb_info_cfg.chapter_cfg_list[chapter].layer_cfg_list[level];

		if (!PugiGetSubNodeValue(data_element, "enter_level_limit", layer_cfg.enter_level_limit) || layer_cfg.enter_level_limit <= 0)
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(data_element, "need_pass_fb_type", layer_cfg.need_pass_fb_type) || layer_cfg.need_pass_fb_type < 0
			|| layer_cfg.need_pass_fb_type >= TUTUI_FB_TYPE_NUM_MAX)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "need_pass_chapter", layer_cfg.need_pass_chapter) || layer_cfg.need_pass_chapter < 0 
			|| layer_cfg.need_pass_chapter >= TUITU_FB_CHAPER_NUM_MAX)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "need_pass_level", layer_cfg.need_pass_level) || layer_cfg.need_pass_level < 0
			|| layer_cfg.need_pass_level >= TUITU_FB_LEVLE_NUM_MAX)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "saodang_star_num", layer_cfg.saodang_star_num) || layer_cfg.saodang_star_num < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "time_limit_1_star", layer_cfg.time_limit_1_star) || layer_cfg.time_limit_1_star <= 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "time_limit_2_star", layer_cfg.time_limit_2_star) || layer_cfg.time_limit_2_star > layer_cfg.time_limit_1_star)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "time_limit_3_star", layer_cfg.time_limit_3_star) || layer_cfg.time_limit_3_star > layer_cfg.time_limit_2_star)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "capability", layer_cfg.capability) || layer_cfg.capability <= 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(data_element, "scene_id", layer_cfg.scene_id) || layer_cfg.scene_id <= 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(data_element, "enter_x", layer_cfg.enter_pos.x) || layer_cfg.enter_pos.x <= 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(data_element, "enter_y", layer_cfg.enter_pos.y) || layer_cfg.enter_pos.y <= 0)
		{
			return -16;
		}

		layer_cfg.monster_count = 0;
		for (int i = 0; i < TUITU_FB_GUANQIA_MONSTER_NUM_MAX; ++i)
		{
			sprintf(tmp_monster_id, "monster_%d", i);
			sprintf(tmp_monster_pos_x, "monster_%d_pos_x", i);
			sprintf(tmp_monster_pos_y, "monster_%d_pos_y", i);

			if (PugiGetSubNodeValue(data_element, tmp_monster_id, layer_cfg.monster_list[i].monster_id))
			{
				if (!MONSTERPOOL->IsMonsterExist(layer_cfg.monster_list[i].monster_id))
				{
					return - 17;
				}

				if (!PugiGetSubNodeValue(data_element, tmp_monster_pos_x, layer_cfg.monster_list[i].pos.x) || layer_cfg.monster_list[i].pos.x <= 0)
				{
					return -18;
				}

				if (!PugiGetSubNodeValue(data_element, tmp_monster_pos_y, layer_cfg.monster_list[i].pos.y) || layer_cfg.monster_list[i].pos.y <= 0)
				{
					return -19;
				}

				++layer_cfg.monster_count;
			}
			else
			{
				break;
			}

			{
				layer_cfg.item_count = 0;
				PugiXmlNode element = data_element.child("first_pass_reward_list");
				if (element.empty())
				{
					return -100;
				}

				PugiXmlNode item_element = element.child("first_pass_reward");
				while (!item_element.empty())
				{
					if (layer_cfg.item_count >= MAX_ATTACHMENT_ITEM_NUM)
					{
						return -101 - layer_cfg.item_count;
					}

					if (!layer_cfg.reward_item_list[layer_cfg.item_count].ReadConfig(item_element))
					{
						if (0 == layer_cfg.reward_item_list[layer_cfg.item_count].item_id)
						{
							break;
						}

						return -151 - layer_cfg.item_count;
					}

					layer_cfg.item_count++;
					item_element = item_element.next_sibling();
				}
			}
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int TuituFbConfig::InitStarRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	int chapter = 0;
	int seq = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "chapter", chapter) || chapter < 0 || chapter >= TUITU_FB_CHAPER_NUM_MAX)
		{
			return -1;
		}

		TuituFbStarRewardConfig &star_reward_cfg = m_star_reward_cfg_list[TUITU_FB_TYPE_NORMAL][chapter];		//目前只有普通模式有星级奖励
		star_reward_cfg.chapter = chapter;

		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= TUITU_FB_CHAPTER_STAR_REWARD_NUM_MAX
			|| seq != star_reward_cfg.item_cfg_count)
		{
			return -2;
		}

		TuituFbStarRewardConfig::ItemCfg &item_cfg = star_reward_cfg.item_cfg_list[star_reward_cfg.item_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "star_num", item_cfg.need_star) || item_cfg.need_star <= 0)
		{
			return -3;
		}

		if (!ItemConfigData::ReadConfigList(data_element, "reward", item_cfg.reward_list, MAX_ATTACHMENT_ITEM_NUM))
		{
			return -4;
		}

		++star_reward_cfg.item_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}
