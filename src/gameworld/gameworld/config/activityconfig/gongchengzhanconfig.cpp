#include "gongchengzhanconfig.hpp"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "monster/monsterpool.h"
#include "item/itempool.h"
#include "skill/skillpool.hpp"
#include "scene/scene.h"
#include "item/itemgriddata.h"
#include "item/itembase.h"
#include "item/expense/norexitem.hpp"
#include "scene/sceneregister/sceneregister.hpp"

GongChengZhanConfig::GongChengZhanConfig()
{

}

GongChengZhanConfig::~GongChengZhanConfig()
{

}

bool GongChengZhanConfig::Init(const std::string &path, std::string *err)
{
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "GongChengZhanConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	int iRet = 0;

	{
		PugiXmlNode elem = RootElement.child("zhangong_reward");
		if (elem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->zhangong_reward]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitZhangongReward(elem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitZhangongReward fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode elem = RootElement.child("other");
		if (elem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->other]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitOther(elem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitOther fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode elem = RootElement.child("day_reward");
		if (elem.empty())
		{
			sprintf(errinfo,"%s:xml node error in node [config->day_reward]", path.c_str());
			*err =  errinfo;
			return false;
		}

		iRet = this->InitDailyReward(elem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitDailyReward fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode elem = RootElement.child("worship_exp");
		if (elem.empty())
		{
			sprintf(errinfo, "%s:xml node error in node [config->worship_exp]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitWorshipCfg(elem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitWorshipCfg fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode elem = RootElement.child("sos_cfg");
		if (elem.empty())
		{
			sprintf(errinfo, "%s:xml node error in node [config->sos_cfg]", path.c_str());
			*err = errinfo;
			return false;
		}

		iRet = this->InitSosCfg(elem);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitSosCfg fail %d ", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int GongChengZhanConfig::InitZhangongReward(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_zhangong = -1;
	while (!data_element.empty())
	{
		int zhangong = 0;
		if (!PugiGetSubNodeValue(data_element, "zhangong", zhangong) || (zhangong < last_zhangong))
		{
			return -1;
		}
		last_zhangong = zhangong;

		GczZhangongRewardItemCfg item_cfg;

		item_cfg.zhangong = zhangong;

		if (!PugiGetSubNodeValue(data_element, "sheng_wang", item_cfg.shengwang_reward) || item_cfg.shengwang_reward < 0)
		{
			return -2;
		}

		{
			PugiXmlNode  item_list_element = data_element.child("reward_item_list");
			if (item_list_element.empty()) return -3;

			PugiXmlNode  item_element = item_list_element.child("reward_item");
			if (item_element.empty()) return -4;

			int count = 0;
			while (!item_element.empty())
			{
				if (count >= MAX_ATTACHMENT_ITEM_NUM) return -5;

				if (!item_cfg.reward_item_list[count].ReadConfig(item_element)) return -6;

				count++;

				item_element = item_element.next_sibling();
			}
		}

		m_zhangong_reward_vec.push_back(item_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int GongChengZhanConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id < 0)
	{
		return -1;
	}

	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.scene_id);

	if (!PugiGetSubNodeValue(data_element, "zhangong_rank_refresh_time", m_other_cfg.zhangong_rank_refresh_time) || m_other_cfg.zhangong_rank_refresh_time < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "boss1_id", m_other_cfg.boss1_id) || !MONSTERPOOL->IsMonsterExist(m_other_cfg.boss1_id))
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "boos1_x", m_other_cfg.boos1_pos.x) || m_other_cfg.boos1_pos.x <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "boos1_y", m_other_cfg.boos1_pos.y) || m_other_cfg.boos1_pos.y <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "boss2_id", m_other_cfg.boss2_id) || m_other_cfg.boss2_id <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "boos2_x", m_other_cfg.boos2_pos.x) || m_other_cfg.boos2_pos.x <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "boos2_y", m_other_cfg.boos2_pos.y) || m_other_cfg.boos2_pos.y <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "relive1_x", m_other_cfg.relive1_pos.x) || m_other_cfg.relive1_pos.x <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "relive1_y", m_other_cfg.relive1_pos.y) || m_other_cfg.relive1_pos.y <= 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(data_element, "relive2_x", m_other_cfg.relive2_pos.x) || m_other_cfg.relive2_pos.x <= 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(data_element, "relive2_y", m_other_cfg.relive2_pos.y) || m_other_cfg.relive2_pos.y <= 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "monster1_id", m_other_cfg.monster1_id) || !MONSTERPOOL->IsMonsterExist(m_other_cfg.monster1_id))
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(data_element, "monster2_id", m_other_cfg.monster2_id) || !MONSTERPOOL->IsMonsterExist(m_other_cfg.monster2_id))
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(data_element,"kill_monster1_zhangong", m_other_cfg.kill_monster1_zhangong) || m_other_cfg.kill_monster1_zhangong <= 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(data_element, "kill_monster2_zhangong", m_other_cfg.kill_monster2_zhangong) || m_other_cfg.kill_monster2_zhangong <= 0)
	{
		return -15;
	}

 //	if (!PugiGetSubNodeValue(data_element, "hurt_role_01_zhangong", m_other_cfg.hurt_role_01_zhangong) || m_other_cfg.hurt_role_01_zhangong <= 0)
// 	{
// 		return -16;
// 	}

	if (!PugiGetSubNodeValue(data_element, "kill_role_zhangong", m_other_cfg.kill_role_zhangong) || m_other_cfg.kill_role_zhangong <= 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(data_element, "hurt_boos_01_zhangong", m_other_cfg.hurt_boos_01_zhangong) || m_other_cfg.hurt_boos_01_zhangong <= 0)
	{
		return -18;
	}

	if (!PugiGetSubNodeValue(data_element, "kill_boos_zhangong", m_other_cfg.kill_boos_zhangong) || m_other_cfg.kill_boos_zhangong <= 0)
	{
		return -19;
	}

	if (!PugiGetSubNodeValue(data_element, "kill_boos_win_zhangong", m_other_cfg.kill_boos_win_zhangong) || m_other_cfg.kill_boos_win_zhangong <= 0)
	{
		return -20;
	}

	if (!PugiGetSubNodeValue(data_element, "kill_boos_attack_zhangong", m_other_cfg.kill_boos_attack_zhangong) || m_other_cfg.kill_boos_attack_zhangong <= 0)
	{
		return -21;
	}

	if (!PugiGetSubNodeValue(data_element, "online_will_get_zhangong", m_other_cfg.online_will_get_zhangong) || m_other_cfg.online_will_get_zhangong <= 0)
	{
		return -22;
	}

	if (!PugiGetSubNodeValue(data_element, "how_much_time_will_get_zhangong", m_other_cfg.how_much_time_will_get_zhangong) || m_other_cfg.how_much_time_will_get_zhangong <= 0)
	{
		return -23;
	}	
	if (!PugiGetSubNodeValue(data_element, "resource_x", m_other_cfg.resource_pos.x) || m_other_cfg.resource_pos.x <= 0)
	{
		return -24;
	}
	if (!PugiGetSubNodeValue(data_element, "resource_y", m_other_cfg.resource_pos.y) || m_other_cfg.resource_pos.y <= 0)
	{
		return -25;
	}
	if (!PugiGetSubNodeValue(data_element, "gong_qiang_x", m_other_cfg.gong_qiang_pos.x) || m_other_cfg.gong_qiang_pos.x <= 0)
	{
		return -26;
	}
	if (!PugiGetSubNodeValue(data_element, "gong_qiang_y", m_other_cfg.gong_qiang_pos.y) || m_other_cfg.gong_qiang_pos.y <= 0)
	{
		return -27;
	}
	if (!PugiGetSubNodeValue(data_element, "resource_zuo_xia_x", m_other_cfg.resource_zuo_xia_pos.x) || m_other_cfg.resource_zuo_xia_pos.x <= 0)
	{
		return -28;
	}
	if (!PugiGetSubNodeValue(data_element, "resource_zuo_xia_y", m_other_cfg.resource_zuo_xia_pos.y) || m_other_cfg.resource_zuo_xia_pos.y <= 0)
	{
		return -29;
	}
	if (!PugiGetSubNodeValue(data_element, "resource_you_shang_x", m_other_cfg.resource_you_shang_pos.x) || m_other_cfg.resource_you_shang_pos.x <= 0)
	{
		return -30;
	}
	if (!PugiGetSubNodeValue(data_element, "resource_you_shang_y", m_other_cfg.resource_you_shang_pos.y) || m_other_cfg.resource_you_shang_pos.y <= 0)
	{
		return -31;
	}

	if (!PugiGetSubNodeValue(data_element, "cz_fashion_yifu_id", m_other_cfg.cz_fashion_yifu_id) || m_other_cfg.cz_fashion_yifu_id < 0 || NULL == ITEMPOOL->GetItem(m_other_cfg.cz_fashion_yifu_id))
	{
		return -32;
	}

	if (!PugiGetSubNodeValue(data_element, "chengzhu_skill_id", m_other_cfg.chengzhu_skill_id) || m_other_cfg.chengzhu_skill_id <= 0 || NULL == SKILLPOOL->GetSkill(m_other_cfg.chengzhu_skill_id))
	{
		return -33;
	}

	if (!PugiGetSubNodeValue(data_element, "cz_fashion_equi_id", m_other_cfg.cz_fashion_equi_id) || m_other_cfg.cz_fashion_equi_id < 0 || NULL == ITEMPOOL->GetItem(m_other_cfg.cz_fashion_equi_id))
	{
		return -34;
	}

	if (!PugiGetSubNodeValue(data_element, "cz_lover_fashion_yifu_id", m_other_cfg.cz_lover_fashion_yifu_id) || m_other_cfg.cz_lover_fashion_yifu_id < 0 || NULL == ITEMPOOL->GetItem(m_other_cfg.cz_lover_fashion_yifu_id))
	{
		return -133;
	}

	if (!PugiGetSubNodeValue(data_element, "cz_lover_fashion_equi_id", m_other_cfg.cz_lover_fashion_equi_id) || m_other_cfg.cz_lover_fashion_equi_id < 0 || NULL == ITEMPOOL->GetItem(m_other_cfg.cz_lover_fashion_equi_id))
	{
		return -134;
	}

	if (!PugiGetSubNodeValue(data_element, "cz_gold_reward", m_other_cfg.cz_gold_reward) || m_other_cfg.cz_gold_reward <= 0)
	{
		return -34;
	}

	if (!PugiGetSubNodeValue(data_element, "cz_shengwang_reward", m_other_cfg.cz_shengwang_reward) || m_other_cfg.cz_shengwang_reward <= 0)
	{
		return -35;
	}
	
	//PugiXmlNode Itemelement = data_element.child("cz_reward_item");
	//ItemID itemid = 0;
	//if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid) )
	//{
	//	if (!m_other_cfg.cz_reward_item.ReadConfig(Itemelement))
	//	{
	//		return -51;
	//	}

	//}else
	//{
	//	return -52;
	//}
	{
		PugiXmlNode element = data_element.child("cz_reward_item_list");
		if (element.empty())
		{
			return -51;
		}

		int i = 0;
		PugiXmlNode item_element = element.child("cz_reward_item");
		while (!item_element.empty())
		{
			if (i >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -52 - i;
			}

			if (!m_other_cfg.cz_reward_item[i].ReadConfig(item_element))
			{
				return -53 - i;
			}

			i++;
			item_element = item_element.next_sibling();
		}
	}

	{
		m_other_cfg.team_reward_item_count = ItemConfigData::ReadConfigList(data_element, "team_reward_item", m_other_cfg.team_reward_item, MAX_ATTACHMENT_ITEM_NUM);
		if (m_other_cfg.team_reward_item_count < 0)
		{
			return -63 - m_other_cfg.team_reward_item_count;
		}
	}

	if (!PugiGetSubNodeValue(data_element, "win_shengwang_reward", m_other_cfg.win_shengwang_reward) || m_other_cfg.win_shengwang_reward <= 0)
	{
		return -35;
	}

	if (!PugiGetSubNodeValue(data_element, "win_reward_gongzi", m_other_cfg.win_reward_gongzi) || m_other_cfg.win_reward_gongzi < 0)
	{
		return -36;
	}

	int win_read_ret = ItemConfigData::ReadConfigList(data_element, "win_reward_item", m_other_cfg.win_reward_item, MAX_ATTACHMENT_ITEM_NUM);
	if (win_read_ret < 0)
	{
		return -200 + win_read_ret;
	}

	if (!PugiGetSubNodeValue(data_element, "fail_shengwang_reward", m_other_cfg.fail_shengwang_reward) || m_other_cfg.fail_shengwang_reward <= 0)
	{
		return -37;
	}

	int fail_read_ret = ItemConfigData::ReadConfigList(data_element, "fail_reward_item", m_other_cfg.fail_reward_item, MAX_ATTACHMENT_ITEM_NUM);
	if (fail_read_ret < 0)
	{
		return -300 + fail_read_ret;
	}

	{
		PugiXmlNode element = data_element.child("huizhang_extra_reward_list");
		if (element.empty())
		{
			return -100;
		}

		int i = 0;
		PugiXmlNode item_element = element.child("huizhang_extra_reward");
		while (!item_element.empty())
		{
			if (i >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -101 - i;
			}

			if (!m_other_cfg.huizhang_extra_reward[i].ReadConfig(item_element))
			{
				return - 151 - i;
			}

			i++;
			m_other_cfg.huizhang_extra_reward_item_count++;
			item_element = item_element.next_sibling();
		}
	}

	if (!PugiGetSubNodeValue(data_element, "lucky_interval", m_other_cfg.lucky_interval) || m_other_cfg.lucky_interval <= 0)
	{
		return -53;
	}
	
	{
		PugiXmlNode Itemelement = data_element.child("lucky_item");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid) )
		{
			if (!m_other_cfg.lucky_item.ReadConfig(Itemelement))
			{
				return -54;
			}

		}else
		{
			return -55;
		}
	}

	if (!PugiGetSubNodeValue(data_element, "luck_people", m_other_cfg.luck_people_num) || m_other_cfg.luck_people_num <= 0)
	{
		return -56;
	}

	if (!PugiGetSubNodeValue(data_element, "opengame_day", m_other_cfg.opengame_day) || m_other_cfg.opengame_day <= 0)
	{
		return -57;
	}

	if (!PugiGetSubNodeValue(data_element, "worship_scene_id", m_other_cfg.worship_scene_id) || !SceneRegister::Instance().CheckSceneIsExist(m_other_cfg.worship_scene_id))
	{
		return -58;
	}

	if (!PugiGetSubNodeValue(data_element, "worship_pos_x", m_other_cfg.worship_pos.x) || m_other_cfg.worship_pos.x <= 0)
	{
		return -59;
	}

	if (!PugiGetSubNodeValue(data_element, "worship_pos_y", m_other_cfg.worship_pos.y) || m_other_cfg.worship_pos.y <= 0)
	{
		return -60;
	}

	//if (!GetSubNodeValue(data_element, "worship_duration", m_other_cfg.worship_duration) || m_other_cfg.worship_duration <= 0)
	//{
	//	return -61;
	//}

	//if (!GetSubNodeValue(data_element, "worship_start_time_s", m_other_cfg.worship_start_time_s) || m_other_cfg.worship_start_time_s <= 0)
	//{
	//	return -62;
	//}

	if (!PugiGetSubNodeValue(data_element, "worship_range", m_other_cfg.worship_range) || m_other_cfg.worship_range <= 0)
	{
		return -63;
	}

	if (!PugiGetSubNodeValue(data_element, "worship_addexp_interval", m_other_cfg.worship_addexp_interval) || m_other_cfg.worship_addexp_interval <= 0)
	{
		return -64;
	}

	if (!PugiGetSubNodeValue(data_element, "worship_click_time", m_other_cfg.worship_click_time_limit) || m_other_cfg.worship_click_time_limit <= 0)
	{
		return -65;
	}

	if (!PugiGetSubNodeValue(data_element, "worship_click_cd", m_other_cfg.worship_click_cd) || m_other_cfg.worship_click_cd <= 0)
	{
		return -66;
	}

	if (!PugiGetSubNodeValue(data_element, "lucky_interval", m_other_cfg.lucky_interval) || m_other_cfg.lucky_interval <= 0)
	{
		return -67;
	}

	if (!PugiGetSubNodeValue(data_element, "cz_wife_title_id", m_other_cfg.cz_wife_title_id) || m_other_cfg.cz_wife_title_id <= 0)
	{
		return -68;
	}

	if (!PugiGetSubNodeValue(data_element, "cz_husband_title_id", m_other_cfg.cz_husband_title_id) || m_other_cfg.cz_husband_title_id <= 0)
	{
		return -69;
	}

	if (!PugiGetSubNodeValue(data_element, "cz_guild_member_title_id", m_other_cfg.cz_guild_member_title_id) || m_other_cfg.cz_guild_member_title_id <= 0)
	{
		return -70;
	}

	if (!PugiGetSubNodeValue(data_element, "exp_factor", m_other_cfg.exp_factor) || m_other_cfg.exp_factor < 0)
	{
		return -71;
	}

	if (!PugiGetSubNodeValue(data_element, "exp_factor_time", m_other_cfg.exp_factor_time) || m_other_cfg.exp_factor_time < 0)
	{
		return -72;
	}

	if (!PugiGetSubNodeValue(data_element, "exp_factor_click", m_other_cfg.exp_factor_click) || m_other_cfg.exp_factor_click < 0)
	{
		return -73;
	}

	return 0;
}

int GongChengZhanConfig::InitDailyReward(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		GczDailyRewardItemCfg item_cfg;

		if (!PugiGetSubNodeValue(data_element, "reward_type", item_cfg.reward_type) || item_cfg.reward_type < 0 || item_cfg.reward_type >= GczDailyRewardItemCfg::REWARD_TYPE_MAX)
		{
			return -1;
		}

		PugiXmlNode item_element_list = data_element.child("reward_item_list");
		if (NULL == item_element_list)
		{
			return -2;
		}

		PugiXmlNode reward_item_element = item_element_list.child("reward_item");
		if (reward_item_element.empty())
		{
			return -3;
		}

		int reward_count = 0;

		while (!reward_item_element.empty())
		{
			if (reward_count >= MAX_ATTACHMENT_ITEM_NUM)
			{
				return -4;
			}
			int item_id = 0;
			if (NULL != reward_item_element && PugiGetSubNodeValue(reward_item_element, "item_id", item_id) && NULL != ITEMPOOL->GetItem(item_id))
			{
				if (!item_cfg.reward_item_list[reward_count].ReadConfig(reward_item_element))
				{
					return -5;
				}
			}else
			{
				return -6;
			}

			++ reward_count;
			reward_item_element = reward_item_element.next_sibling();
		}

		m_daily_reward_vec.push_back(item_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}
int GongChengZhanConfig::InitWorshipCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_level = 0;

	while (!data_element.empty())
	{
		GongChengZhanWorshipExpConfig cfg;
		if (!PugiGetSubNodeValue(data_element, "role_level", cfg.level) || cfg.level <= 0 || last_level >= cfg.level)
		{
			return -1;
		}
		cfg.min_level = last_level;
		last_level = cfg.level+1;

		if (!PugiGetSubNodeValue(data_element, "interval_addexp", cfg.interval_addexp) || cfg.interval_addexp <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "click_addexp", cfg.click_addexp) || cfg.click_addexp <= 0)
		{
			return -3;
		}

		m_worship_exp_cfg_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int GongChengZhanConfig::InitSosCfg(PugiXmlNode RootElemen)
{
	PugiXmlNode dataElement = RootElemen.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while ( !dataElement.empty())
	{
		int times = -1;
		if (!PugiGetSubNodeValue(dataElement, "times", times) || times < 0 || times != static_cast<int>(m_sos_cost_cfg_vec.size()))
		{
			return -1;
		}

		int cost = -1;
		if (!PugiGetSubNodeValue(dataElement, "cost", cost) || cost < 0)
		{
			return -2;
		}

		m_sos_cost_cfg_vec.push_back(cost);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

// 玩家进入场景时对应的pos
int GongChengZhanConfig::GetEnterPos(bool is_shou_site, Posi *enter_pos)
{
	if (NULL == enter_pos) return 0;

	if (is_shou_site)
	{
		*enter_pos = m_other_cfg.relive2_pos;
	}
	else
	{
		*enter_pos = m_other_cfg.relive1_pos;
	}

	return m_other_cfg.scene_id;
}

// 根据战功获取对应的奖励
GczZhangongRewardItemCfg *GongChengZhanConfig::GetGczZhangongRewardItemCfgForZhangong(int zhangong)
{
	if (!m_zhangong_reward_vec.empty())
	{
		// 没有奖励
		if(zhangong < m_zhangong_reward_vec[0].zhangong)
		{
			return NULL;
		}
		
		// 最大的奖励
		if(zhangong >= m_zhangong_reward_vec[m_zhangong_reward_vec.size() - 1].zhangong)
		{
			return &m_zhangong_reward_vec[m_zhangong_reward_vec.size() - 1];
		}

		int size = static_cast<int>(m_zhangong_reward_vec.size());
		for (int i = 0; i < size; ++ i)
		{
			if(i + 1 >= size)
			{
				return NULL;
			}

			if(zhangong >= m_zhangong_reward_vec[i].zhangong && zhangong < m_zhangong_reward_vec[i + 1].zhangong)
			{
				return &m_zhangong_reward_vec[i];
			}
		}
	}
	return NULL;
}

bool GongChengZhanConfig::GetChengZhuShizhuang(int *yf_part_type, int *yf_part_index, int *wq_part_type, int *wq_part_index, bool is_lover)
{
	if (NULL == yf_part_type || NULL == yf_part_index || NULL == wq_part_type || NULL == wq_part_index)
	{
		return false;
	}

	const ItemBase *yf_card = nullptr;
	const ItemBase *wq_card = nullptr;
	if (is_lover)
	{
		yf_card = ITEMPOOL->GetItem(m_other_cfg.cz_lover_fashion_yifu_id);
		wq_card = ITEMPOOL->GetItem(m_other_cfg.cz_lover_fashion_equi_id);
	}
	else
	{
		yf_card = ITEMPOOL->GetItem(m_other_cfg.cz_fashion_yifu_id);
		wq_card = ITEMPOOL->GetItem(m_other_cfg.cz_fashion_equi_id);
	}

	if (NULL == yf_card || NULL == wq_card || ItemBase::I_TYPE_EXPENSE != yf_card->GetItemType() || ItemBase::I_TYPE_EXPENSE != wq_card->GetItemType())
	{
		return false;
	}

	NorexItem *yf_norexitem = (NorexItem *)yf_card;
	NorexItem *wq_norexitem = (NorexItem *)wq_card;

	*yf_part_type = yf_norexitem->GetParam1();
	*yf_part_index = yf_norexitem->GetParam2();
	*wq_part_type = wq_norexitem->GetParam1();
	*wq_part_index = wq_norexitem->GetParam2();

	return true;
}

const GongChengZhanWorshipExpConfig * GongChengZhanConfig::GetWorshipExpCfg(const int role_level)
{
	for (auto it = m_worship_exp_cfg_vec.begin(); it != m_worship_exp_cfg_vec.end(); ++it)
	{
		if (role_level <= it->level && role_level >= it->min_level)
		{
			return &(*it);
		}
	}

	return nullptr;
}

const GczDailyRewardItemCfg *GongChengZhanConfig::GetDailyRewardItemCfg(int reward_type)
{
	if (reward_type < 0 || reward_type >= static_cast<int>(m_daily_reward_vec.size()))
	{
		return NULL;
	}

	return &m_daily_reward_vec[reward_type];
}

int GongChengZhanConfig::GetSosCost(int times)
{
	if (times < 0 || times >= static_cast<int>(m_sos_cost_cfg_vec.size()))
	{
		return -1;
	}

	return m_sos_cost_cfg_vec[times];
}