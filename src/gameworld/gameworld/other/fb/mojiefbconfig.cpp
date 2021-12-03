#include "mojiefbconfig.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"
#include "item/itempool.h"

MoJieFbConfig::MoJieFbConfig()
{	
	m_human_pian_cfg_count = 0;
	m_ai_ren_pian_cfg_count = 0;
	m_jing_ling_pian_cfg_count = 0;			
	m_jing_ling_pian_chapter_cfg_count = 0;	
	m_skill_cfg_count = 0;
}

MoJieFbConfig::~MoJieFbConfig()
{
}

bool MoJieFbConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = { 0 };
	
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "MoJieFbConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if(NULL == RootElement)
	{
		*err = path + "xml root node error";
		return false;
	}
	
	{
		// 人类篇
		PugiXmlNode root_element = RootElement.child("renlei_pian");
		if(root_element.empty())
		{
			*err = path + ":no [renlei_pian]";
			return false;
		}

		iRet = this->InitHumanPian(root_element);
		if(iRet)
		{
			sprintf(err_info, "%s: InitRenLeiPian failed %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 矮人篇
		PugiXmlNode root_element = RootElement.child("airen_pian");
		if(root_element.empty())
		{
			*err = path + ":no [airen_pian]";
			return false;
		}

		iRet = this->InitAiRenPian(root_element);
		if(iRet)
		{
			sprintf(err_info, "%s: InitAiRenPian failed %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 精灵篇
		PugiXmlNode root_element = RootElement.child("jingling_pian");
		if(root_element.empty())
		{
			*err = path + ":no [jingling_pian]";
			return false;
		}

		iRet = this->InitJingLingPian(root_element);
		if(iRet)
		{
			sprintf(err_info, "%s: InitJingLingPian failed %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 魔戒技能
		PugiXmlNode root_element = RootElement.child("ji_neng");
		if(root_element.empty())
		{
			*err = path + ":no [ji_neng]";
			return false;
		}

		iRet = this->InitSkillPian(root_element);
		if(iRet)
		{
			sprintf(err_info, "%s: InitSkillPian failed %d", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int MoJieFbConfig::InitHumanPian(PugiXmlNode RootElement)
{
	PugiXmlNode data_elemnet = RootElement.child("data");
	if(data_elemnet.empty())
	{
		return -1000;
	}

	int last_fb_id = -1;
	while(!data_elemnet.empty())
	{
		int fb_id = -1;
		if(!PugiGetSubNodeValue(data_elemnet, "fb_id", fb_id) || fb_id != last_fb_id + 1)
		{
			return -1001;
		}

		if(fb_id < 0 || fb_id >= MOJIE_FB_HUMAN_PIAN_MAX_COUNT)
		{
			return -1002;
		}

		last_fb_id = fb_id;

		HumanPianCfg & cfg = m_human_pian_cfg[fb_id];
		cfg.fb_id = fb_id;
		m_human_pian_cfg_count = fb_id;

		if(!PugiGetSubNodeValue(data_elemnet, "enter_pos_x", cfg.enter_pos.x) || cfg.enter_pos.x <= 0)
		{
			return -1003;
		}

		if(!PugiGetSubNodeValue(data_elemnet, "enter_pos_y", cfg.enter_pos.y) || cfg.enter_pos.y <= 0)
		{
			return -1004;
		}
		
		if(!PugiGetSubNodeValue(data_elemnet, "scene_id", cfg.scene_id) || cfg.scene_id < 0)
		{
			return -1005;
		}

		if(!PugiGetSubNodeValue(data_elemnet, "boss_id", cfg.boss_id) || !MONSTERPOOL->IsMonsterExist(cfg.boss_id))
		{
			return -1006;
		}

		if(!PugiGetSubNodeValue(data_elemnet, "born_pos_x", cfg.born_pos.x) || cfg.born_pos.x <= 0)
		{
			return -1007;
		}

		if(!PugiGetSubNodeValue(data_elemnet, "born_pos_y", cfg.born_pos.y) || cfg.born_pos.y <= 0)
		{
			return -1008;
		}

		if(!PugiGetSubNodeValue(data_elemnet, "mojie_id", cfg.mojie_id) || NULL != ITEMPOOL->GetItem(cfg.mojie_id))
		{
			return -1009;
		}

		data_elemnet = data_elemnet.next_sibling();

	}

	return 0;
}

int MoJieFbConfig::InitAiRenPian(PugiXmlNode RootElement)
{
	PugiXmlNode data_elemnet = RootElement.child("data");
	if(data_elemnet.empty())
	{
		return -1000;
	}

	int last_fb_id = -1;
	while(!data_elemnet.empty())
	{
		int fb_id = -1;
		if(!PugiGetSubNodeValue(data_elemnet, "fb_id", fb_id) || fb_id != last_fb_id + 1)
		{
			return -1001;
		}

		if(fb_id < 0 || fb_id >= MOJIE_FB_HUMAN_PIAN_MAX_COUNT)
		{
			return -1002;
		}

		last_fb_id = fb_id;

		AiRenPianCfg & cfg = m_ai_ren_pian_cfg[fb_id];
		cfg.fb_id = fb_id;
		m_ai_ren_pian_cfg_count = fb_id;

		if(!PugiGetSubNodeValue(data_elemnet, "enter_pos_x", cfg.enter_pos.x) || cfg.enter_pos.x <= 0)
		{
			return -1003;
		}

		if(!PugiGetSubNodeValue(data_elemnet, "enter_pos_y", cfg.enter_pos.y) || cfg.enter_pos.y <= 0)
		{
			return -1004;
		}

		if(!PugiGetSubNodeValue(data_elemnet, "scene_id", cfg.scene_id) || cfg.scene_id < 0)
		{
			return -1005;
		}

		if(!PugiGetSubNodeValue(data_elemnet, "boss_id", cfg.boss_id) || !MONSTERPOOL->IsMonsterExist(cfg.boss_id))
		{
			return -1006;
		}

		if(!PugiGetSubNodeValue(data_elemnet, "born_pos_x", cfg.born_pos.x) || cfg.born_pos.x <= 0)
		{
			return -1007;
		}

		if(!PugiGetSubNodeValue(data_elemnet, "born_pos_y", cfg.born_pos.y) || cfg.born_pos.y <= 0)
		{
			return -1008;
		}

		if(!PugiGetSubNodeValue(data_elemnet, "mojie_id", cfg.mojie_id) || NULL != ITEMPOOL->GetItem(cfg.mojie_id))
		{
			return -1009;
		}

		data_elemnet = data_elemnet.next_sibling();

	}
	
	return 0;
}

int MoJieFbConfig::InitJingLingPian(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if(data_element.empty())
	{
		return -1000;
	}
	
	int chapter = -1;
	int last_chapter = 0;
	int task = -1;
	int last_task = -1;
	
	while(!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "chapter", chapter) || chapter < 0 || chapter >= MOJIE_FB_SKILL_PIAN_ZHANGJIE_MAX_COUNT)
		{
			return -1001;
		}
		
		if (chapter != last_chapter)
		{
			last_chapter = chapter;
			last_task = -1;
		}
		
		if (!PugiGetSubNodeValue(data_element, "task", task) || task < 0 || task != last_task + 1 || task >= MOJIE_FB_JINGLING_PIAN_ZHANGJIE_TASK_MAX_COUNT)
		{
			return -1002;
		}

		last_task = task;

		m_jing_ling_pian_cfg[chapter].chapter = chapter;
		JingLingPianCfg::TaskConfig &cfg = m_jing_ling_pian_cfg[chapter].task_list[task];
		cfg.task = task;
		m_jing_ling_pian_chapter_cfg_count = chapter;

		if(!PugiGetSubNodeValue(data_element, "id", cfg.id) || cfg.id < 0)
		{
			return -1010;
		}

		if(!PugiGetSubNodeValue(data_element, "param1", cfg.param1) || cfg.param1 < 0)
		{
			return -1004;
		}

		if(!PugiGetSubNodeValue(data_element, "param2", cfg.param2) || cfg.param2 < 0)
		{
			return -1005;
		}

		if(!PugiGetSubNodeValue(data_element, "param3", cfg.param3) || cfg.param3 < 0)
		{
			return -1006;
		}
		
		PugiXmlNode itemellment = data_element.child("reward_item1");
		ItemID itemid = 0;
		if(NULL != itemellment && PugiGetSubNodeValue(itemellment, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
		{
			if(!cfg.reward1.ReadConfig(itemellment))
			{
				return -1007;
			}
		}
		else
		{
			return -1008;
		}

		itemellment = data_element.child("reward_item2");
		itemid = 0;
		if(NULL != itemellment && PugiGetSubNodeValue(itemellment, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
		{
			if(!cfg.reward2.ReadConfig(itemellment))
			{
				return -1009;
			}
		}
		else
		{
			return -1010;
		}

		if(!PugiGetSubNodeValue(data_element, "mojie_id", cfg.mojie_id) || cfg.mojie_id < 0 || cfg.mojie_id > MOJIE_FB_SKILL_MAX_COUNT)
		{
			return -1011;
		}

		if(!PugiGetSubNodeValue(data_element, "desc", cfg.task_desc))
		{
			return -1012;
		}
		
		m_jing_ling_pian_cfg[chapter].task_list_count = task;
		m_jing_ling_pian_cfg_count = cfg.id;

		data_element = data_element.next_sibling();
	}
	
	return 0;
}

int MoJieFbConfig::InitSkillPian(PugiXmlNode RootElement)
{
	PugiXmlNode data_elemnet = RootElement.child("data");
	if(data_elemnet.empty())
	{
		return -1000;
	}

	int last_mojie_id = -1;
	while(!data_elemnet.empty())
	{
		int mojie_id = -1;
		if(!PugiGetSubNodeValue(data_elemnet, "mojie_id", mojie_id) || mojie_id != last_mojie_id + 1)
		{
			return -1001;
		}

		if(mojie_id < 0 || mojie_id >= MOJIE_FB_SKILL_MAX_COUNT)
		{
			return -1002;
		}

		last_mojie_id = mojie_id;
		m_skill_cfg_count = mojie_id;

		MoJieSkillCfg & cfg = m_skill_cfg[mojie_id];

		if(!PugiGetSubNodeValue(data_elemnet, "mojie_jineng_type", cfg.mojie_jineng_type) || cfg.mojie_jineng_type < 0)
		{
			return -1003;
		}

		if(!PugiGetSubNodeValue(data_elemnet, "param_a", cfg.param_a) || cfg.param_a < 0)
		{
			return -1004;
		}

		if(!PugiGetSubNodeValue(data_elemnet, "param_b", cfg.param_b) || cfg.param_b < 0)
		{
			return -1005;
		}

		if(!PugiGetSubNodeValue(data_elemnet, "param_c", cfg.param_c) || cfg.param_c < 0)
		{
			return -1006;
		}

		if(!PugiGetSubNodeValue(data_elemnet, "param_d", cfg.param_d) || cfg.param_d < 0)
		{
			return -1007;
		}

		data_elemnet = data_elemnet.next_sibling();

	}
	return 0;
}

const JingLingPianCfg::TaskConfig * MoJieFbConfig::GetJingLingCfgToParam(int chapter, int param1, int param2, int param3)
{
	if(chapter < 0 || chapter >= MOJIE_FB_SKILL_PIAN_ZHANGJIE_MAX_COUNT || chapter >= m_jing_ling_pian_chapter_cfg_count)
	{
		return NULL;
	}

	int size = m_jing_ling_pian_cfg[chapter].task_list_count;

	if(MOJIEFB_FANGYU_TASK == chapter)
	{
		for (int i = size - 1; i >= 0; i --)
		{
			if (param1 >= m_jing_ling_pian_cfg[chapter].task_list[i].param1)
			{
				return &m_jing_ling_pian_cfg[chapter].task_list[i];
			}
		}
	}

	else if(MOJIEFB_ZHUANGBEI_TASK == chapter)
	{
		for(int i = 0; i < size; i ++ )
		{
			JingLingPianCfg::TaskConfig &task_cfg = m_jing_ling_pian_cfg[chapter].task_list[i];
			
			if(param1 == task_cfg.param1 && param2 == task_cfg.param2 && param3 == task_cfg.param3)
			{
				return &m_jing_ling_pian_cfg[chapter].task_list[i];
			}
		}
	}

	else if(MOJIEFB_BOSS_TASK == chapter)
	{
		for(int i = 0; i < size; i ++ )
		{
			if(param1 == m_jing_ling_pian_cfg[chapter].task_list[i].param1)
			{
				return &m_jing_ling_pian_cfg[chapter].task_list[i];
			}
		}
	}

	else if(MOJIEFB_SHENZHU_TASK  == chapter)
	{
		for(int i = 0; i < size; i ++ )
		{
			if(param1 == m_jing_ling_pian_cfg[chapter].task_list[i].param1)
			{
				return &m_jing_ling_pian_cfg[chapter].task_list[i];
			}
		}
	}
	
	return NULL;
}

const HumanPianCfg * MoJieFbConfig::GetHumanPianCfg(int fb_id)
{
	if(fb_id < 0 || fb_id >= m_human_pian_cfg_count || fb_id >= MOJIE_FB_HUMAN_PIAN_MAX_COUNT)
	{
		return NULL;
	}
	return &m_human_pian_cfg[fb_id];
}

const AiRenPianCfg * MoJieFbConfig::GetAiRenCfg(int fb_id)
{
	if(fb_id < 0 || fb_id >= m_ai_ren_pian_cfg_count || fb_id >= MOJIE_FB_AIREN_PIAN_MAX_COUNT)
	{
		return NULL;
	}
	return &m_ai_ren_pian_cfg[fb_id];
}

const JingLingPianCfg * MoJieFbConfig::GetJingLingCfg(int fb_id)// 返回章节
{
	if(fb_id < 0 || fb_id >= m_jing_ling_pian_chapter_cfg_count || fb_id >= MOJIE_FB_SKILL_PIAN_ZHANGJIE_MAX_COUNT)
	{
		return NULL;
	}
	return &m_jing_ling_pian_cfg[fb_id];
}

const MoJieSkillCfg * MoJieFbConfig::GetMoJieCfg(int id)
{
	if(id < 0 || id >= m_skill_cfg_count)
	{
		return NULL;
	}
	return &m_skill_cfg[id];
}