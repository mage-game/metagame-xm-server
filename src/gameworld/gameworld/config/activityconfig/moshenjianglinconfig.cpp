#include "moshenjianglinconfig.hpp"
#include "servercommon/configcommon.h"
#include "gameworld/gameworld/checkresourcecenter.hpp"
#include "gameworld/gameworld/monster/monsterpool.h"

MoShenJiangLinConfig::MoShenJiangLinConfig()
{

}

MoShenJiangLinConfig::~MoShenJiangLinConfig()
{

}

bool MoShenJiangLinConfig::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "MoShenJiangLinConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

		// Ä§Éñ½µÁÙ»ù±¾ÅäÖÃ
	{
		PugiXmlNode MoshenjiangLinlistElement = RootElement.child("moshen_jianglin");
		if (MoshenjiangLinlistElement.empty())
		{
			*err = path + ": xml node error in [config->moshen_jianglin]";
			return false;
		}

		PugiXmlNode DataElement = MoshenjiangLinlistElement.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml node error in [config->moshen_jianglin->data]";
			return false;
		}

		int last_tm_wday = -1;
		while (!DataElement.empty())
		{
			int tm_wday = 0;
			if (!PugiGetSubNodeValue(DataElement, "tm_wday", tm_wday) || tm_wday < 0 || tm_wday >= MAX_DAY_IN_WEEK)
			{
				*err = path + ": xml node error in [config->moshen_jianglin->data->tm_wday tm_wday < 0 || tm_wday >= MAX_DAY_IN_WEEK]";
				return false;
			}
			
			if (tm_wday != last_tm_wday + 1)
			{
				*err = path + ": xml node error in [config->moshen_jianglin->data->tm_wday tm_wday != last_tm_wday + 1]";
				return false;
			}
			last_tm_wday = tm_wday;
			
			m_boss_list[tm_wday].tm_wday = tm_wday;

			MoShenJiangLinBossCfg &cfg = m_boss_list[tm_wday];

			if (!PugiGetSubNodeValue(DataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
			{
				*err = path + ": xml node error in [config->moshen_jianglin->data->scene_id]";
				return false;
			}

			CheckResourceCenter::Instance().GetSceneCheck()->Add(cfg.scene_id);

			int pos_x = 0;
			if (!PugiGetSubNodeValue(DataElement, "pos_x", pos_x)|| pos_x <= 0)
			{
				*err = path + ": xml node error in [config->moshen_jianglin->data->pos_x]";
				return false;
			}
			cfg.pos.x = pos_x;

			int pos_y = 0 ;
			if (!PugiGetSubNodeValue(DataElement, "pos_y", pos_y)|| pos_y <= 0)
			{
				*err = path + ": xml node error in [config->moshen_jianglin->data->pos_y]";
				return false;
			}
			cfg.pos.y = pos_y;
			
			int valid_reward_count = 0;
			if (!PugiGetSubNodeValue(DataElement, "base_xianhun", cfg.base_xianhun))
			{
				*err = path + ": xml node error in [config->moshen_jianglin->data->base_xianhun]";
				return false;
			}
			if (cfg.base_xianhun > 0)
			{
				++ valid_reward_count;
			}

			if (!PugiGetSubNodeValue(DataElement, "base_yuanli", cfg.base_yuanli))
			{
				*err = path + ": xml node error in [config->moshen_jianglin->data->base_yuanli]";
				return false;
			}
			if (cfg.base_yuanli > 0)
			{
				++ valid_reward_count;
			}

			if (!PugiGetSubNodeValue(DataElement, "base_tongbi", cfg.base_tongbi))
			{
				*err = path + ": xml node error in [config->moshen_jianglin->data->base_tongbi]";
				return false;
			}
			if (cfg.base_tongbi > 0)
			{
				++ valid_reward_count;
			}
			
			if (1 != valid_reward_count)
			{
				*err = path + ": xml node error in [config->moshen_jianglin->data->tm_wday 1 != valid_reward_count]";
				return false;
			}

			DataElement = DataElement.next_sibling();
		}
	}
	
	// Ä§Éñ½µÁÙbossÅäÖÃ
	{
		PugiXmlNode MoshenjiangLinbosslistElement = RootElement.child("moshen_jianglin_boss");
		if (MoshenjiangLinbosslistElement.empty())
		{
			*err = path + ": xml node error in [config->moshen_jianglin_boss]";
			return false;
		}

		PugiXmlNode DataElement = MoshenjiangLinbosslistElement.child("data");
		if (DataElement.empty())
		{
			*err = path + ": xml node error in [config->moshen_jianglin_boss->data]";
			return false;
		}

		int last_tm_wday = -1;
		while (!DataElement.empty())
		{
			int tm_wday = 0;
			if (!PugiGetSubNodeValue(DataElement, "tm_wday", tm_wday)|| tm_wday < 0 || tm_wday >= MAX_DAY_IN_WEEK)
			{
				*err = path + ": xml node error in [config->moshen_jianglin_boss->data->tm_wday < 0 || tm_wday >= MAX_DAY_IN_WEEK]";
				return false;
			}
			
			if (tm_wday != last_tm_wday + 1)
			{
				*err = path + ": xml node error in [config->moshen_jianglin_boss->data->tm_wday != last_tm_wday + 1]";
				return false;
			}
			last_tm_wday = tm_wday;

			MoShenJiangLinBossCfg &cfg = m_boss_list[tm_wday];

			char tmp_tag[32] = {0};

			for (int i = 0; i < WORLD_LEVEL_BOSS_MAX; i++)
			{
				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "boss_%d_id", i);

				if (!PugiGetSubNodeValue(DataElement, tmp_tag, cfg.boss_idlist[i].boss_id)|| cfg.boss_idlist[i].boss_id <= 0)
				{
					*err = path + ": xml node error in [config->moshen_jianglin_boss->data->]" + tmp_tag;
					return false;
				}

				if (!MONSTERPOOL->IsMonsterExist(cfg.boss_idlist[i].boss_id))
				{
					*err = path + ": xml node error in [config->moshen_jianglin_boss->data->monster not exist]" + tmp_tag;
					return false;
				}

				memset(tmp_tag, 0, sizeof(tmp_tag));
				sprintf(tmp_tag, "world_%d_level", i);

				if (!PugiGetSubNodeValue(DataElement, tmp_tag, cfg.boss_idlist[i].world_lv)|| cfg.boss_idlist[i].world_lv <= 0)
				{
					*err = path + ": xml node error in [config->moshen_jianglin_boss->data->world_lv]" + tmp_tag;
					return false;
				}
			}

			DataElement = DataElement.next_sibling();
		}
	}

	return true;
}

MoShenJiangLinBossCfg * MoShenJiangLinConfig::GetMoShenJiangLinBossCfg(int tm_wday)
{
	if (tm_wday < 0 || tm_wday >= MAX_DAY_IN_WEEK) return NULL;

	return &m_boss_list[tm_wday];
}






























