#include "zhuanzhiconfig.hpp"
#include "item/itempool.h"
#include <set>
#include "item/itembase.h"
#include "monster/monsterpool.h"
#include "skill/skillpool.hpp"
#include "task/taskpool.h"

ZhuanzhiConfig::ZhuanzhiConfig() : m_change_item_cfgcount(0), m_max_vir_skill_count(0)
{

}

ZhuanzhiConfig::~ZhuanzhiConfig()
{

}

bool ZhuanzhiConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = { 0 };

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "ZhuanzhiConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// ÔÓÏîÅäÖÃ
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s ZhuanzhiConfig::InitOtherCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	/*{
		PugiXmlNode root_element = RootElement.child("item_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not item_cfg node ";
			return false;
		}

		iRet = this->InitItemChangeCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s ZhuanzhiConfig::InitItemChangeCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}*/

	{
		PugiXmlNode root_element = RootElement.child("zhuanzhi_attr");
		if (root_element.empty())
		{
			*err = path + " xml not zhuanzhi_attr node ";
			return false;
		}

		iRet = this->InitZhuanzhiAttrCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitZhuanzhiAttrCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("zhuanzhi_vir_skill");
		if (root_element.empty())
		{
			*err = path + " xml not zhuanzhi_vir_skill node ";
			return false;
		}

		iRet = this->InitZhuanzhiVirSkillCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitZhuanzhiVirSkillCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("zhuanzhi_skill");
		if (root_element.empty())
		{
			*err = path + " xml not zhuanzhi_skill node ";
			return false;
		}

		iRet = this->InitZhuanzhiSkillCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitZhuanzhiSkillCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("zhuanzhi_fb");
		if (root_element.empty())
		{
			*err = path + " xml not zhuanzhi_fb node ";
			return false;
		}

		iRet = this->InitZhuanzhiFbCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitZhuanzhiFbCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("zhuanzhi_tianming");
		if (root_element.empty())
		{
			*err = path + " xml not zhuanzhi_tianming node ";
			return false;
		}

		iRet = this->InitZhuanzhiTianMingCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitZhuanzhiTianMingCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("zhuanzhi_canglong");
		if (root_element.empty())
		{
			*err = path + " xml not zhuanzhi_canglong node ";
			return false;
		}

		iRet = this->InitZhuanzhiCangLongCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitZhuanzhiCangLongCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("zhuanzhi_shengxiao");
		if (root_element.empty())
		{
			*err = path + " xml not zhuanzhi_shengxiao node ";
			return false;
		}

		iRet = this->InitZhuanzhiShengXiaoCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitZhuanzhiShengXiaoCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("zhuanzhi_yuanqi");
		if (root_element.empty())
		{
			*err = path + " xml not zhuanzhi_yuanqi node ";
			return false;
		}

		iRet = this->InitZhuanzhiYuanQiCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitZhuanzhiYuanQiCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("zhuanzhi_shenqi");
		if (root_element.empty())
		{
			*err = path + " xml not zhuanzhi_shenqi node ";
			return false;
		}

		iRet = this->InitZhuanzhiShenQiCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitZhuanzhiShenQiCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("zhuanzhi_xingxiu");
		if (root_element.empty())
		{
			*err = path + " xml not zhuanzhi_xingxiu node ";
			return false;
		}

		iRet = this->InitZhuanzhiXingXiuCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitZhuanzhiXingXiuCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int ZhuanzhiConfig::InitItemChangeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	std::set<ItemID> repeat_checker;

	while (!dataElement.empty())
	{
		if (m_change_item_cfgcount >= CHANGE_ITEM_CFG_MAX_COUNT)
		{
			return -1;
		}

		ItemID item_id = 0; const ItemBase *item_base = NULL;
		if (!PugiGetSubNodeValue(dataElement, "prof_1", item_id))
		{
			return -2;
		}
		m_change_item_cfglist[m_change_item_cfgcount].item_list[PROF_TYPE_PROF_1] = item_id;

		item_base = ITEMPOOL->GetItem(item_id);
		if (NULL == item_base || PROF_TYPE_PROF_1 != item_base->GetLimitProf() % PROF_TYPE_PROF_ZHUANZHI_BASE)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "prof_2", item_id))
		{
			return -4;
		}
		m_change_item_cfglist[m_change_item_cfgcount].item_list[PROF_TYPE_PROF_2] = item_id;

		item_base = ITEMPOOL->GetItem(item_id);
		if (NULL == item_base || PROF_TYPE_PROF_2 != item_base->GetLimitProf() % PROF_TYPE_PROF_ZHUANZHI_BASE)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "prof_3", item_id))
		{
			return -6;
		}
		m_change_item_cfglist[m_change_item_cfgcount].item_list[PROF_TYPE_PROF_3] = item_id;

		item_base = ITEMPOOL->GetItem(item_id);
		if (NULL == item_base || PROF_TYPE_PROF_3 != item_base->GetLimitProf() % PROF_TYPE_PROF_ZHUANZHI_BASE)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "prof_4", item_id))
		{
			return -8;
		}
		m_change_item_cfglist[m_change_item_cfgcount].item_list[PROF_TYPE_PROF_4] = item_id;

		item_base = ITEMPOOL->GetItem(item_id);
		if (NULL == item_base || PROF_TYPE_PROF_4 != item_base->GetLimitProf() % PROF_TYPE_PROF_ZHUANZHI_BASE)
		{
			return -9;
		}

		for (int i = PROF_TYPE_PROF_1; i <= PROF_TYPE_PROF_4; ++i)
		{
			int prof_itemid = m_change_item_cfglist[m_change_item_cfgcount].item_list[i];
			m_itemid_cfgindex_map[prof_itemid] = m_change_item_cfgcount;

			if (repeat_checker.end() != repeat_checker.find(prof_itemid))
			{
				return -10;
			}
			repeat_checker.insert(prof_itemid);
		}

		++m_change_item_cfgcount;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	if (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "change_prof_stuff_id", m_other_cfg.change_prof_stuff_id))// || NULL == ITEMPOOL->GetItem(m_other_cfg.change_prof_stuff_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "normal_skill_fix_hurt_per", m_other_cfg.normal_skill_fix_hurt_per) || m_other_cfg.normal_skill_fix_hurt_per < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "team_injure_increase_per", m_other_cfg.team_injure_increase_per) || m_other_cfg.team_injure_increase_per < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "hurt_boss_injure_add_per", m_other_cfg.hurt_boss_injure_add_per) || m_other_cfg.hurt_boss_injure_add_per < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "one_key_level3_task_cost_gold", m_other_cfg.one_key_level3_task_cost_gold) || m_other_cfg.one_key_level3_task_cost_gold < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "role_level_to_prof_level5", m_other_cfg.role_level_to_prof_level5) || m_other_cfg.role_level_to_prof_level5 <= 0 || m_other_cfg.role_level_to_prof_level5 > MAX_ROLE_LEVEL)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "one_key_item_price_1", m_other_cfg.one_key_item_price_1) || m_other_cfg.one_key_item_price_1 < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "one_key_item_price_2", m_other_cfg.one_key_item_price_2) || m_other_cfg.one_key_item_price_2 < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "one_key_item_id_1", m_other_cfg.one_key_item_id_1) || m_other_cfg.one_key_item_id_1 < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "one_key_item_id_2", m_other_cfg.one_key_item_id_2) || m_other_cfg.one_key_item_id_2 < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "fb_kick_out_time_s", m_other_cfg.fb_kick_time_s) || m_other_cfg.fb_kick_time_s < 0)
		{
			return -11;
		}
	}

	return 0;
}

int ZhuanzhiConfig::InitZhuanzhiAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int zhuanzhi_task_complete_count = 0;
	int last_complete_count = 0;
	while (!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "task_complete_count", zhuanzhi_task_complete_count) || zhuanzhi_task_complete_count <= 0 || zhuanzhi_task_complete_count > ROLE_ZHUANZHI_TASK_TOTAL_COUNT_MAX)
		{
			return -1;
		}

		if (zhuanzhi_task_complete_count != last_complete_count + 1)
		{
			return -1001;
		}

		last_complete_count = zhuanzhi_task_complete_count;

		ZhuanzhiAttrCfg &cfg = m_zhuanzhi_attr_cfg[zhuanzhi_task_complete_count];

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "pojia", cfg.per_pojia) || cfg.per_pojia < 0)
		{
			return -4;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiConfig::InitZhuanzhiSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_prof_level = 0;
	int last_prof = 0;

	while (!dataElement.empty())
	{
		int prof_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "prof_level", prof_level) || prof_level <= 0 || prof_level > MAX_PROF_LEVEL)
		{
			return -1;
		}

		int prof = 0;
		if (!PugiGetSubNodeValue(dataElement, "prof", prof) || prof <= PROF_TYPE_INVALID || prof >= PROF_TYPE_PROF_NO_LIMIT)
		{
			return -2;
		}

		if (last_prof_level == prof_level && prof != last_prof + 1)
		{
			return -100;
		}
		if (last_prof_level != prof_level && prof != 1)
		{
			return -200;
		}

		last_prof_level = prof_level;
		last_prof = prof;

		ZhuanZhiLevelSkillConfig::ConfigItem &cfg = m_zhuanzhi_level_skill_cfg[prof_level].config_list[prof];

		if (!PugiGetSubNodeValue(dataElement, "pvp_zengshang_per", cfg.pvp_zengshang_per) || cfg.pvp_zengshang_per < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "pvp_jianshang_per", cfg.pvp_jianshang_per) || cfg.pvp_jianshang_per < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "pve_zengshang_per", cfg.pve_zengshang_per) || cfg.pve_zengshang_per < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "pve_jianshang_per", cfg.pve_jianshang_per) || cfg.pve_jianshang_per < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_target_num", cfg.add_target_num) || cfg.add_target_num < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "role_level_attr_per", cfg.role_level_attr_per) || cfg.role_level_attr_per < 0)
		{
			return -8;
		}

		for (unsigned int i = 0; i < static_array_size(cfg.add_skill_list); i++)
		{
			std::string add_skill_name = "add_skill_" + std::to_string(i + 1);
			if (!PugiGetSubNodeValue(dataElement, add_skill_name, cfg.add_skill_list[i]) || cfg.add_skill_list[i] < 0)
			{
				return -100 - i;
			}

			bool is_vir_skill = false;
			if ((cfg.add_skill_list[i] > 0) && cfg.add_skill_list[i] < (int)static_array_size(m_zhuanzhi_vir_skill_cfg_list))
			{
				int vir_skill_seq = cfg.add_skill_list[i];
				if (!m_zhuanzhi_vir_skill_cfg_list[vir_skill_seq].IsValid())
				{
					return -200 - i;
				}

				is_vir_skill = true;
			}

			if (cfg.add_skill_list[i] > 0 && !is_vir_skill && NULL == SKILLPOOL->GetSkill(cfg.add_skill_list[i]))
			{
				return -300 - i;
			}
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiConfig::InitZhuanzhiVirSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_vir_skill_seq = 0;
	while (!dataElement.empty())
	{
		int vir_skill_seq = 0;
		if (!PugiGetSubNodeValue(dataElement, "vir_skill_seq", vir_skill_seq) || vir_skill_seq <= 0 || vir_skill_seq >= ZHUANZHI_VIR_SKILL_TYPE_MAX_COUNT)
		{
			return -1;
		}

		if (vir_skill_seq != last_vir_skill_seq + 1)
		{
			return -100;
		}
		last_vir_skill_seq = vir_skill_seq;

		ZhuanzhiVirSkillCfg &cfg = m_zhuanzhi_vir_skill_cfg_list[vir_skill_seq];
		cfg.vir_skill_seq = vir_skill_seq;

		if (!PugiGetSubNodeValue(dataElement, "vir_skill_type", cfg.vir_skill_type) || cfg.vir_skill_type <= ZHUANZHI_VIR_SKILL_TYPE_INVALID || cfg.vir_skill_type >= ZHUANZHI_VIR_SKILL_TYPE_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "follow_perform_skill_id", cfg.follow_perform_skill_id) || cfg.follow_perform_skill_id < 0 || (cfg.follow_perform_skill_id > 0 && NULL == SKILLPOOL->GetSkill(cfg.follow_perform_skill_id)))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "cd_time", cfg.cd_time) || cfg.cd_time < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "rate", cfg.rate) || cfg.rate <= 0 || cfg.rate > 10000)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_1", cfg.param_1) || cfg.param_1 < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_2", cfg.param_2) || cfg.param_2 < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_3", cfg.param_3) || cfg.param_3 < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_4", cfg.param_4) || cfg.param_4 < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_5", cfg.param_5) || cfg.param_5 < 0)
		{
			return -10;
		}

		m_max_vir_skill_count++;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiConfig::InitZhuanzhiFbCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_prof_level = 0;
	int last_fb_index = 0;
	while (!dataElement.empty())
	{
		int prof_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "prof_level", prof_level) || prof_level <= 0 || prof_level > MAX_PROF_LEVEL)
		{
			return -1;
		}

		int fb_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "fb_index", fb_index) || fb_index < 0 || fb_index > PER_PROF_MAX_FB_INDEX)
		{
			return -100;
		}

		if (prof_level == last_prof_level && fb_index != last_fb_index + 1)
		{
			return -101;
		}

		if (prof_level != last_prof_level && prof_level != last_prof_level + 1)
		{
			return -102;
		}

		last_fb_index = fb_index;
		last_prof_level = prof_level;

		ZhuanzhiFbCfg &cfg = m_zhuanzhi_fb_cfg[prof_level][fb_index];
		cfg.prof_level = prof_level;

		if (!PugiGetSubNodeValue(dataElement, "level_limit", cfg.level_limit) || cfg.level_limit <= 0 || cfg.level_limit > MAX_ROLE_LEVEL)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "scene_id", cfg.scene_id) || cfg.scene_id <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_id", cfg.boss_id) || cfg.boss_id <= 0 || !MONSTERPOOL->IsMonsterExist(cfg.boss_id))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_pos_x", cfg.boss_pos.x) || cfg.boss_pos.x < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_pos_y", cfg.boss_pos.y) || cfg.boss_pos.y < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "enter_pos_x", cfg.enter_pos.x) || cfg.enter_pos.x < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "enter_pos_y", cfg.enter_pos.y) || cfg.enter_pos.y < 0)
		{
			return -8;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiConfig::InitZhuanzhiTianMingCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_level = 0;
	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level <= 0 || level > MAX_ZHUANZHI_TIANMING_LEVEL)
		{
			return -1;
		}

		if (level != last_level + 1)
		{
			return -10;
		}
		last_level = level;

		ZhuanzhiStruct &cfg = m_zhuanzhi_tianming_cfg[level - 1];
		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_id", cfg.need_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.need_stuff_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_num", cfg.need_stuff_num) || cfg.need_stuff_num <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_exp", cfg.need_exp) || cfg.need_exp <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "pojia", cfg.per_pojia) || cfg.per_pojia < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "exp_factor", cfg.exp_factor) || cfg.exp_factor < 0)
		{
			return -9;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiConfig::InitZhuanzhiCangLongCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_level = 0;
	//int last_type = -1;
	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level <= 0 || level > MAX_ZHUANZHI_CANGLONG_LEVEL)
		{
			return -2;
		}

		if (level != last_level + 1)
		{
			return -3;
		}

		last_level = level;

		ZhuanzhiStruct &cfg = m_zhuanzhi_canglong_cfg[level - 1];
		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "pretaskid", cfg.pretaskid) || NULL == TASKPOOL->GetCfgByTaskID(cfg.pretaskid))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_id", cfg.need_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.need_stuff_id))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_num", cfg.need_stuff_num) || cfg.need_stuff_num <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_exp", cfg.need_exp) || cfg.need_exp <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "pojia", cfg.per_pojia) || cfg.per_pojia < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "exp_factor", cfg.exp_factor) || cfg.exp_factor < 0)
		{
			return -12;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiConfig::InitZhuanzhiShengXiaoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_level = 0;
	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level <= 0 || level > MAX_ZHUANZHI_SHENGXIAO_LEVEL)
		{
			return -1;
		}

		if (level != last_level + 1)
		{
			return -10;
		}
		last_level = level;

		ZhuanzhiStruct &cfg = m_zhuanzhi_shengxiao_cfg[level - 1];
		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_id", cfg.need_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.need_stuff_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_num", cfg.need_stuff_num) || cfg.need_stuff_num <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_exp", cfg.need_exp) || cfg.need_exp <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "pojia", cfg.per_pojia) || cfg.per_pojia < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "exp_factor", cfg.exp_factor) || cfg.exp_factor < 0)
		{
			return -9;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiConfig::InitZhuanzhiYuanQiCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_level = 0;
	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level <= 0 || level > MAX_ZHUANZHI_YUANQI_LEVEL)
		{
			return -1;
		}

		if (level != last_level + 1)
		{
			return -10;
		}
		last_level = level;

		ZhuanzhiStruct &cfg = m_zhuanzhi_yuanqi_cfg[level - 1];
		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_id", cfg.need_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.need_stuff_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_num", cfg.need_stuff_num) || cfg.need_stuff_num <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_exp", cfg.need_exp) || cfg.need_exp <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "pojia", cfg.per_pojia) || cfg.per_pojia < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "exp_factor", cfg.exp_factor) || cfg.exp_factor < 0)
		{
			return -9;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiConfig::InitZhuanzhiShenQiCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_level = 0;
	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level <= 0 || level > MAX_ZHUANZHI_SHENQI_LEVEL)
		{
			return -1;
		}

		if (level != last_level + 1)
		{
			return -10;
		}
		last_level = level;

		ZhuanzhiStruct &cfg = m_zhuanzhi_shenqi_cfg[level - 1];
		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_id", cfg.need_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.need_stuff_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_num", cfg.need_stuff_num) || cfg.need_stuff_num <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_exp", cfg.need_exp) || cfg.need_exp <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "pojia", cfg.per_pojia) || cfg.per_pojia < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "exp_factor", cfg.exp_factor) || cfg.exp_factor < 0)
		{
			return -9;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ZhuanzhiConfig::InitZhuanzhiXingXiuCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	int last_level = 0;
	while (!dataElement.empty())
	{
		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level <= 0 || level > MAX_ZHUANZHI_XINGXIU_LEVEL)
		{
			return -2;
		}

		if (level != last_level + 1)
		{
			return -3;
		}

		last_level = level;

		ZhuanzhiStruct &cfg = m_zhuanzhi_xingxiu_cfg[level - 1];
		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "pretaskid", cfg.pretaskid) || NULL == TASKPOOL->GetCfgByTaskID(cfg.pretaskid))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_id", cfg.need_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.need_stuff_id))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_stuff_num", cfg.need_stuff_num) || cfg.need_stuff_num <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_exp", cfg.need_exp) || cfg.need_exp <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "pojia", cfg.per_pojia) || cfg.per_pojia < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "exp_factor", cfg.exp_factor) || cfg.exp_factor < 0)
		{
			return -14;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

bool ZhuanzhiConfig::GetChangeItemID(ItemID old_itemid, int to_prof, ItemID *new_itemid)
{
	if (NULL == new_itemid || to_prof <= PROF_TYPE_INVALID || to_prof >= PROF_TYPE_PROF_NO_LIMIT)
	{
		return false;
	}

	std::map<ItemID, int>::iterator it = m_itemid_cfgindex_map.find(old_itemid);
	if (m_itemid_cfgindex_map.end() == it)
	{
		return false;
	}

	int cfg_idx = it->second;
	if (cfg_idx < 0 || cfg_idx >= m_change_item_cfgcount)
	{
		return false;
	}

	*new_itemid = m_change_item_cfglist[cfg_idx].item_list[to_prof];
	return true;
}

const ZhuanzhiAttrCfg * ZhuanzhiConfig::GetZhuanzhiAttrCfg(int complete_count) const
{
	if (complete_count <= 0 || complete_count > ROLE_ZHUANZHI_TASK_TOTAL_COUNT_MAX)
	{
		return NULL;
	}

	return &m_zhuanzhi_attr_cfg[complete_count];
}

const ZhuanZhiLevelSkillConfig::ConfigItem * ZhuanzhiConfig::GetZhuanzhiSkillCfg(int prof_level, int base_prof) const
{
	if (prof_level <= 0 || prof_level > MAX_PROF_LEVEL || base_prof <= PROF_TYPE_INVALID || base_prof >= PROF_TYPE_PROF_NO_LIMIT)
	{
		return NULL;
	}

	return &m_zhuanzhi_level_skill_cfg[prof_level].config_list[base_prof];
}

const ZhuanzhiVirSkillCfg * ZhuanzhiConfig::GetZhuanzhiVirSkillCfg(int vir_skill_seq) const
{
	if (vir_skill_seq <= 0 || vir_skill_seq >= ZHUANZHI_VIR_SKILL_TYPE_MAX_COUNT)
	{
		return NULL;
	}

	return &m_zhuanzhi_vir_skill_cfg_list[vir_skill_seq];
}

const ZhuanzhiFbCfg * ZhuanzhiConfig::GetZhuanzhiFbCfg(int prof_level, int fb_index) const
{
	if (prof_level <= 0 || prof_level > MAX_PROF_LEVEL || fb_index < 0 || fb_index > PER_PROF_MAX_FB_INDEX)
	{
		return NULL;
	}

	return &m_zhuanzhi_fb_cfg[prof_level][fb_index];
}

const ZhuanzhiStruct * ZhuanzhiConfig::GetZhuanzhiTianMingCfg(short level) const
{
	if (level <= 0 || level > MAX_ZHUANZHI_TIANMING_LEVEL)
	{
		return NULL;
	}

	return &m_zhuanzhi_tianming_cfg[level - 1];
}

const ZhuanzhiStruct *ZhuanzhiConfig::GetZhuanzhiCangLongCfg(short level) const
{
	if (level <= 0 || level > MAX_ZHUANZHI_CANGLONG_LEVEL)
	{
		return NULL;
	}

	return &m_zhuanzhi_canglong_cfg[level - 1];
}

const ZhuanzhiStruct *ZhuanzhiConfig::GetZhuanzhiShengXiaoCfg(short level) const
{
	if (level <= 0 || level > MAX_ZHUANZHI_SHENGXIAO_LEVEL)
	{
		return NULL;
	}

	return &m_zhuanzhi_shengxiao_cfg[level - 1];
}

const ZhuanzhiStruct *ZhuanzhiConfig::GetZhuanzhiYuanQiCfg(short level) const
{
	if (level <= 0 || level > MAX_ZHUANZHI_YUANQI_LEVEL)
	{
		return NULL;
	}

	return &m_zhuanzhi_yuanqi_cfg[level - 1];
}

const ZhuanzhiStruct *ZhuanzhiConfig::GetZhuanzhiShenQiCfg(short level) const
{
	if (level <= 0 || level > MAX_ZHUANZHI_SHENQI_LEVEL)
	{
		return NULL;
	}

	return &m_zhuanzhi_shenqi_cfg[level - 1];
}

const ZhuanzhiStruct *ZhuanzhiConfig::GetZhuanzhiXingXiuCfg(short level) const
{
	if (level <= 0 || level > MAX_ZHUANZHI_XINGXIU_LEVEL)
	{
		return NULL;
	}

	return &m_zhuanzhi_xingxiu_cfg[level - 1];
}

