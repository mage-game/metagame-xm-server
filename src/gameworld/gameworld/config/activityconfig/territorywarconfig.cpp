#include "territorywarconfig.hpp"

#include "monster/monsterpool.h"
#include "monster/monsterinitparam.h"
#include "item/itempool.h"

#include "servercommon/configcommon.h"
#include "global/rank/rankmanager.hpp"
#include "servercommon/errornum.h"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"

#include "checkresourcecenter.hpp"
#include <string>

TerritoryWarConfig::TerritoryWarConfig()
{
}

TerritoryWarConfig::~TerritoryWarConfig()
{
}

bool TerritoryWarConfig::Init(const std::string &configname, std::string *err)
{
	char err_info[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "TerritoryWarConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + " xml root node error";
		return false;
	}

	{	// ÆäËûÅäÖÃ
		PugiXmlNode root_elment = RootElement.child("other");
		if (NULL == root_elment)
		{
			*err = configname + "xml not other node";
			return false;
		}
		iRet = this->InitOtherCfg(root_elment);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOtherCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// ½¨ÖþÅäÖÃ
		PugiXmlNode root_elment = RootElement.child("building");
		if (NULL == root_elment)
		{
			*err = configname + "xml not building node";
			return false;
		}
		iRet = this->InitbBuildingCfg(root_elment);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitbBuildingCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// Õ½¶·ÉÌµêÅäÖÃ
		PugiXmlNode root_elment = RootElement.child("fight_shop");
		if (NULL == root_elment)
		{
			*err = configname + "xml not fight_shop node";
			return false;
		}
		iRet = this->InitFightShopCfg(root_elment);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitFightShopCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// ¸´»îÉÌµêÅäÖÃ
		PugiXmlNode root_elment = RootElement.child("relive_shop");
		if (NULL == root_elment)
		{
			*err = configname + "xml not relive_shop node";
			return false;
		}
		iRet = this->InitReliveShopCfg(root_elment);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitReliveShopCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// ¼¼ÄÜÅäÖÃ
		PugiXmlNode root_elment = RootElement.child("skill_list");
		if (NULL == root_elment)
		{
			*err = configname + "xml not skill_list node";
			return false;
		}
		iRet = this->InitSkill(root_elment);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitSkill fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// ¸öÈË»ý·Ö½±ÀøÅäÖÃ
		PugiXmlNode root_elment = RootElement.child("personal_credit_reward");
		if (NULL == root_elment)
		{
			*err = configname + "xml not personal_credit_reward node";
			return false;
		}
		iRet = this->InitPersonaleRewardCfg(root_elment);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitPersonaleRewardCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// ¹Ò»ú¹ÖÎïÅäÖÃ
		PugiXmlNode root_elment = RootElement.child("guaji_monster");
		if (NULL == root_elment)
		{
			*err = configname + "xml not guaji_monster node";
			return false;
		}
		iRet = this->InitGuajiMonster(root_elment);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitGuajiMonster fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// »î¶¯½áÊø½±ÀøÅäÖÃ
		PugiXmlNode root_elment = RootElement.child("activity_close_reward");
		if (NULL == root_elment)
		{
			*err = configname + "xml not activity_close_reward node";
			return false;
		}
		iRet = this->InitActivityEndCfg(root_elment);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitActivityEndCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{	// ¹Ì¶¨ÉËº¦ÅäÖÃ
		PugiXmlNode root_elment = RootElement.child("player_fix_hurt");
		if (NULL == root_elment)
		{
			*err = configname + "xml not player_fix_hurt node";
			return false;
		}
		iRet = this->InitFixHurtCfg(root_elment);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitFixHurtCfg fail %d ", configname.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int TerritoryWarConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if(!PugiGetSubNodeValue(dataElement,"scene_id",m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -1;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.scene_id);

	if (!PugiGetSubNodeValue(dataElement,"blue_relive_pos_x",m_other_cfg.blue_relive_pos.x) || m_other_cfg.blue_relive_pos.x <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement,"blue_relive_pos_y",m_other_cfg.blue_relive_pos.y) || m_other_cfg.blue_relive_pos.y <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement,"red_relive_pos_x",m_other_cfg.red_relive_pos.x) || m_other_cfg.red_relive_pos.x <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement,"red_relive_pos_y",m_other_cfg.red_relive_pos.y) || m_other_cfg.red_relive_pos.y <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement,"kill_player_credit",m_other_cfg.kill_player_credit) || m_other_cfg.kill_player_credit < 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement,"kill_car_credit",m_other_cfg.kill_car_credit) || m_other_cfg.kill_car_credit < 0)
	{
		return -7;
	}

	if(!PugiGetSubNodeValue(dataElement,"add_credit_interval_s",m_other_cfg.add_credit_interval_s) || m_other_cfg.add_credit_interval_s <= 0)
	{
		return -8;
	}

	if(!PugiGetSubNodeValue(dataElement,"add_credit",m_other_cfg.add_credit) || m_other_cfg.add_credit < 0)
	{
		return -9;
	}

	if(!PugiGetSubNodeValue(dataElement,"kill_player_guild_credit",m_other_cfg.kill_player_guild_credit) || m_other_cfg.kill_player_guild_credit < 0)
	{
		return -8;
	}

	if(!PugiGetSubNodeValue(dataElement,"kill_car_guild_credit",m_other_cfg.kill_car_guild_credit) || m_other_cfg.kill_car_guild_credit < 0)
	{
		return -9;
	}

	if(!PugiGetSubNodeValue(dataElement,"kill_player_assist_credit",m_other_cfg.kill_player_assist_credit) || m_other_cfg.kill_player_assist_credit < 0)
	{
		return -10;
	}

	if(!PugiGetSubNodeValue(dataElement,"kill_car_assist_credit",m_other_cfg.kill_car_assist_credit) || m_other_cfg.kill_car_assist_credit < 0)
	{
		return -11;
	}

	if(!PugiGetSubNodeValue(dataElement,"assist_vaild_time",m_other_cfg.assist_vaild_time) || m_other_cfg.assist_vaild_time < 0)
	{
		return -12;
	}

	if(!PugiGetSubNodeValue(dataElement,"ice_landmine_num_limit",m_other_cfg.ice_landmine_num_limit) || m_other_cfg.ice_landmine_num_limit < 0)
	{
		return -13;
	}

	if(!PugiGetSubNodeValue(dataElement,"fire_landmine_num_limit",m_other_cfg.fire_landmine_num_limit) || m_other_cfg.fire_landmine_num_limit < 0)
	{
		return -14;
	}

	if(!PugiGetSubNodeValue(dataElement,"center_relive_point_id",m_other_cfg.center_relive_point_id) || m_other_cfg.center_relive_point_id < 0 || !MONSTERPOOL->IsMonsterExist(m_other_cfg.center_relive_point_id))
	{
		return -15;
	}

	if(!PugiGetSubNodeValue(dataElement,"red_fortress_id",m_other_cfg.red_fortress_id) || m_other_cfg.red_fortress_id < 0 || !MONSTERPOOL->IsMonsterExist(m_other_cfg.red_fortress_id))
	{
		return -16;
	}

	if(!PugiGetSubNodeValue(dataElement,"blue_fortress_id",m_other_cfg.blue_fortress_id) || m_other_cfg.blue_fortress_id < 0 || !MONSTERPOOL->IsMonsterExist(m_other_cfg.blue_fortress_id))
	{
		return -17;
	}

	if(!PugiGetSubNodeValue(dataElement,"attack_notice_interval",m_other_cfg.attack_notice_interval) || m_other_cfg.attack_notice_interval < 0 )
	{
		return -18;
	}

	{
		PugiXmlNode element = dataElement.child("huizhang_extra_reward_list");
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

	if (!PugiGetSubNodeValue(dataElement, "lucky_interval", m_other_cfg.lucky_interval) || m_other_cfg.lucky_interval <= 0)
	{
		return -19;
	}

	{
		PugiXmlNode Itemelement = dataElement.child("lucky_item");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
		{
			if (!m_other_cfg.lucky_item.ReadConfig(Itemelement))
			{
				return -20;
			}

		}else
		{
			return -21;
		}
	}

	if (!PugiGetSubNodeValue(dataElement, "luck_people", m_other_cfg.luck_people_num) || m_other_cfg.luck_people_num <= 0)
	{
		return -22;
	}

	return 0; 
}

int TerritoryWarConfig::InitbBuildingCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while(!dataElement.empty())
	{
		int building_id = 0;
		if (!PugiGetSubNodeValue(dataElement,"building_id",building_id) || building_id <= 0)
		{
			return -1;
		}

		std::map<int, TerritroyWarBuildingCfg>::iterator it = m_building_cfg_list.find(building_id);
		if (m_building_cfg_list.end() != it)
		{
			return -2;
		}

		TerritroyWarBuildingCfg cfg;
		cfg.building_id = building_id;

		if (!PugiGetSubNodeValue(dataElement,"building_pos_x",cfg.building_pos.x) || cfg.building_pos.x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement,"building_pos_y",cfg.building_pos.y) || cfg.building_pos.y <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement,"guild_credit_reward",cfg.guild_credit_reward) || cfg.guild_credit_reward <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement,"personal_credit_reward",cfg.personal_credit_reward) || cfg.personal_credit_reward <= 0)
		{
			return -6;
		}


		if (!PugiGetSubNodeValue(dataElement,"side",cfg.side) || cfg.side < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement,"preposition_monster_1",cfg.preposition_monster_1) || cfg.preposition_monster_1 < 0)
		{
			return -8;
		}

		if (0 != cfg.preposition_monster_1)
		{
			std::map<int, TerritroyWarBuildingCfg>::iterator monster_1 = m_building_cfg_list.find(cfg.preposition_monster_1);
			if (m_building_cfg_list.end() == monster_1)
			{
				return -9;
			}
		}

		if (!PugiGetSubNodeValue(dataElement,"preposition_monster_2",cfg.preposition_monster_2) || cfg.preposition_monster_2 < 0)
		{
			return -10;
		}

		if (0 != cfg.preposition_monster_2)
		{
			std::map<int, TerritroyWarBuildingCfg>::iterator monster_2 = m_building_cfg_list.find(cfg.preposition_monster_2);
			if (m_building_cfg_list.end() == monster_2)
			{
				return -11;
			}
		}

		if(!PugiGetSubNodeValue(dataElement,"building_index",cfg.building_index) || cfg.building_index < 0)
		{
			return -12;
		}

		if(!PugiGetSubNodeValue(dataElement,"assist_credit_reward", cfg.kill_assist_credit) || cfg.kill_assist_credit < 0)
		{
			return -13;
		}

		if(!PugiGetSubNodeValue(dataElement,"assist_vaild_time", cfg.assist_vaild_time) || cfg.assist_vaild_time < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement,"tower_attack_man_per", cfg.tower_attack_man_per) || cfg.tower_attack_man_per < 0 || cfg.tower_attack_man_per > 10000)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement,"tower_attack_fight_car_per", cfg.tower_attack_fight_car_per) || cfg.tower_attack_fight_car_per < 0 || cfg.tower_attack_fight_car_per > 10000)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(dataElement,"tower_attack_fang_car_per", cfg.tower_attack_fang_car_per) || cfg.tower_attack_fang_car_per < 0 || cfg.tower_attack_fang_car_per > 10000)
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(dataElement,"tower_attack_cure_car_per", cfg.tower_attack_cure_car_per) || cfg.tower_attack_cure_car_per < 0 || cfg.tower_attack_cure_car_per > 10000)
		{
			return -18;
		}

		m_building_cfg_list[building_id] = cfg;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int TerritoryWarConfig::InitFightShopCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while(!dataElement.empty())
	{
		int type = 0;
		if (!PugiGetSubNodeValue(dataElement, "type", type) || type < 0)
		{ 
			return -1;
		}
		
		int goods_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "goods_id", goods_id) || goods_id < 0 || m_fight_shop_cfg_list[type].end() != m_fight_shop_cfg_list[type].find(goods_id))
		{ 
			return -2;
		}
		TerritroyWarFightShopCfg &cfg = m_fight_shop_cfg_list[type][goods_id];
		cfg.type = type;
		cfg.goods_id = goods_id;
	

		if (!PugiGetSubNodeValue(dataElement, "cost_credit", cfg.cost_credit) || cfg.cost_credit <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement,"guild_credit_reward", cfg.guild_credit_reward) || cfg.guild_credit_reward < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement,"personal_credit_reward", cfg.personal_credit_reward) || cfg.personal_credit_reward < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement,"param1", cfg.param1) || cfg.param1 < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement,"param2", cfg.param2) || cfg.param2 < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement,"param3", cfg.param3) || cfg.param3 < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement,"param4", cfg.param4) || cfg.param4 < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement,"param5", cfg.param5) || cfg.param5 < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement,"param6", cfg.param6) || cfg.param6 < 0)
		{
			return -10;
		}

		dataElement = dataElement.next_sibling();
	}
	
	return 0;
}

int TerritoryWarConfig::InitReliveShopCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_goods_id = -1;

	while(!dataElement.empty())
	{
		int goods_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "goods_id", goods_id) || goods_id < 0 || goods_id <= last_goods_id)
		{ 
			return -1;
		}

		TerritroyWarReliveShopCfg &cfg = m_relive_shop_cfg_list[goods_id];
		cfg.goods_id = goods_id;
		last_goods_id = goods_id;

		if (!PugiGetSubNodeValue(dataElement,"cost_credit", cfg.cost_credit) || cfg.cost_credit <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement,"param1", cfg.param1) || cfg.param1 < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement,"param2", cfg.param2) || cfg.param2 < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement,"param3", cfg.param3) || cfg.param3 < 0)
		{
			return -5;
		}

		dataElement = dataElement.next_sibling();
	}
	return 0;
}

int TerritoryWarConfig::InitSkill(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_skill_index = 0;
	while (!data_element.empty())
	{
		int skill_index = 0;
		if (!PugiGetSubNodeValue(data_element, "skill_index", skill_index) || (1 != skill_index - last_skill_index) || TW_SKILL_INVALID >= skill_index || TW_SKILL_MAX <= skill_index)
		{
			return -1;
		}
		last_skill_index = skill_index;

		TerritroyWarSkillItemCfg &item_cfg = m_skill_map[skill_index];
		item_cfg.skill_index = skill_index;

		if (!PugiGetSubNodeValue(data_element, "skill_level", item_cfg.skill_level) || item_cfg.skill_level <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "distance", item_cfg.distance) || item_cfg.distance <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "attack_range", item_cfg.attack_range) || item_cfg.attack_range <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "enemy_num", item_cfg.enemy_num) || item_cfg.enemy_num <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "cd_s", item_cfg.cd_s) || item_cfg.cd_s <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "hurt_percent", item_cfg.hurt_percent) || item_cfg.hurt_percent < 0 || item_cfg.hurt_percent > 10000)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "fix_hurt", item_cfg.fix_hurt) || item_cfg.fix_hurt < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "param_a", item_cfg.param_a) || item_cfg.param_a < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "param_b", item_cfg.param_b) || item_cfg.param_b < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "param_c", item_cfg.param_c) || item_cfg.param_c < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "param_d", item_cfg.param_d) || item_cfg.param_d < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "fix_hurt_on_fight_car", item_cfg.fix_hurt_on_fight_car) || item_cfg.fix_hurt_on_fight_car < 0 || item_cfg.fix_hurt_on_fight_car > 10000)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(data_element, "fix_hurt_on_fang_car", item_cfg.fix_hurt_on_fang_car) || item_cfg.fix_hurt_on_fang_car < 0 || item_cfg.fix_hurt_on_fang_car > 10000)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(data_element, "fix_hurt_on_cure_car", item_cfg.fix_hurt_on_cure_car) || item_cfg.fix_hurt_on_cure_car < 0 || item_cfg.fix_hurt_on_cure_car > 10000)
		{
			return -15;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int TerritoryWarConfig::InitPersonaleRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_person_credit_min = 0;
	int last_index = 0;

	while(!dataElement.empty())
	{
		int person_credit_min = 0;
		if (!PugiGetSubNodeValue(dataElement, "person_credit_min", person_credit_min) || person_credit_min < 0 || person_credit_min <= last_person_credit_min)
		{ 
			return -1;
		}

		TerritroyWarPersonalRewardCfg &cfg = m_person_credit_reward_cfg_list[person_credit_min];
		cfg.person_credit_min = person_credit_min;
		last_person_credit_min = person_credit_min;

		if (!PugiGetSubNodeValue(dataElement,"person_credit_max", cfg.person_credit_max) || cfg.person_credit_max <= 0 || cfg.person_credit_min > cfg.person_credit_max)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement,"reward_index", cfg.reward_index) || cfg.reward_index <= 0 || cfg.reward_index > 31 || cfg.reward_index < last_index)
		{
			return -3;
		}

		last_index = cfg.reward_index;

		PugiXmlNode Itemelement = dataElement.child("item1");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
		{
			if (!cfg.item_1.ReadConfig(Itemelement))
			{
				return -4;
			}
		}

		Itemelement = dataElement.child("item2");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
		{
			if (!cfg.item_2.ReadConfig(Itemelement))
			{
				return -5;
			}
		}

		Itemelement = dataElement.child("item3");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
		{
			if (!cfg.item_3.ReadConfig(Itemelement))
			{
				return -6;
			}
		}

		if (!PugiGetSubNodeValue(dataElement,"banggong", cfg.banggong) || cfg.banggong < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement,"shengwang", cfg.shengwang) || cfg.shengwang < 0)
		{
			return -8;
		}

		dataElement = dataElement.next_sibling();
	}
	return 0;
}

int TerritoryWarConfig::InitGuajiMonster(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_monster_id = 0;

	while(!dataElement.empty())
	{
		int monster_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "monster_id", monster_id) || monster_id <= 0 || monster_id <= last_monster_id)
		{ 
			return -1;
		}

		TerritroyWarGuajiMonsterCfg &cfg = m_guaji_monster_cfg_list[monster_id];
		cfg.monster_id = monster_id;
		last_monster_id = monster_id;

		if (!PugiGetSubNodeValue(dataElement,"kill_credit_reward", cfg.kill_credit_reward) || cfg.kill_credit_reward < 0)
		{
			return -2;
		}

		dataElement = dataElement.next_sibling();
	}
	return 0;
}

int TerritoryWarConfig::InitActivityEndCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while(!dataElement.empty())
	{
		int room_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "room_index", room_index) || room_index < 0 || room_index >= MAX_TRRITORYWAR_ROOM)
		{ 
			return -1;
		}

		int reward_index = 0;
		if (!PugiGetSubNodeValue(dataElement, "reward_index", reward_index) || reward_index < 0 || m_activity_reward_cfg_list[room_index].end() != m_activity_reward_cfg_list[room_index].find(reward_index))
		{ 
			return -2;
		}
		TerritroyWarActivityEndCfg &cfg = m_activity_reward_cfg_list[room_index][reward_index];
		cfg.room_index = room_index;
		cfg.reward_index = reward_index;

		PugiXmlNode Itemelement = dataElement.child("item1");
		ItemID itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
		{
			if (!cfg.reward_item_1.ReadConfig(Itemelement))
			{
				return -3;
			}
		}

		Itemelement = dataElement.child("item2");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
		{
			if (!cfg.reward_item_2.ReadConfig(Itemelement))
			{
				return -4;
			}
		}

		Itemelement = dataElement.child("item3");
		itemid = 0;
		if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
		{
			if (!cfg.reward_item_3.ReadConfig(Itemelement))
			{
				return -5;
			}
		}

		if (!PugiGetSubNodeValue(dataElement, "banggong", cfg.banggong) || cfg.banggong < 0)
		{ 
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "shengwang", cfg.shengwang) || cfg.shengwang < 0)
		{ 
			return -7;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int TerritoryWarConfig::InitFixHurtCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if(!PugiGetSubNodeValue(dataElement,"man_attack_fight_car_per", m_fixhurt_cfg.man_attack_fight_car_per) || m_fixhurt_cfg.man_attack_fight_car_per < 0 || m_fixhurt_cfg.man_attack_fight_car_per > 10000)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement,"fix_hurt_on_tower", m_fixhurt_cfg.fix_hurt_on_tower) || m_fixhurt_cfg.fix_hurt_on_tower < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement,"fix_hurt_on_relive", m_fixhurt_cfg.fix_hurt_on_relive) || m_fixhurt_cfg.fix_hurt_on_relive < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement,"fix_hurt_on_fortress", m_fixhurt_cfg.fix_hurt_on_fortress) || m_fixhurt_cfg.fix_hurt_on_fortress < 0)
	{
		return -4;
	}

	if(!PugiGetSubNodeValue(dataElement,"man_attack_fang_car_per", m_fixhurt_cfg.man_attack_fang_car_per) || m_fixhurt_cfg.man_attack_fang_car_per < 0 || m_fixhurt_cfg.man_attack_fang_car_per > 10000)
	{
		return -9;
	}

	if(!PugiGetSubNodeValue(dataElement,"man_attack_cure_car_per", m_fixhurt_cfg.man_attack_cure_car_per) || m_fixhurt_cfg.man_attack_cure_car_per < 0 || m_fixhurt_cfg.man_attack_cure_car_per > 10000)
	{
		return -10;
	}

	return 0; 
}

int TerritoryWarConfig::GetTerritoryWarEnterInfo(Role *role,Posi *pos)
{
	if (NULL == pos || NULL == role) return 0;

	if (INVALID_GUILD_ID == role->GetGuildID())
	{
		role->NoticeNum(errornum::EN_ENTER_NOT_GUILD);
		return 0;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
	if (NULL == guild)
	{
		return 0;
	}
	
	int rank = guild->GetGuildBattleRank();

	if (rank <= 0 || rank > Protocol::CAN_JOIN_MAX_RANK)
	{
		role->NoticeNum(errornum::EN_ENTER_GUILD_NOT_HAV_QUALIFICATION);
		return 0;
	}
	else
	{
		long long side = rank % 2;
		if(side)
		{
			*pos = m_other_cfg.red_relive_pos;
		}
		else
		{
			*pos = m_other_cfg.blue_relive_pos;
		}
	}

	return m_other_cfg.scene_id;
}

GuildID TerritoryWarConfig::GetGuildIDByRank(int rank)
{
	if (rank <= 0 || rank > 10)
	{
		return INVALID_GUILD_ID;
	}
	else
	{
		GuildID guildbattel_rank_list[Protocol::CAN_JOIN_MAX_RANK];
		memset(guildbattel_rank_list, 0, sizeof(guildbattel_rank_list));
		GuildRank *guild_rank = RankManager::Instance().GetGuildRank();
		if (NULL != guild_rank)
		{
			guild_rank->GetRankTopGuild(GUILD_RANK_TYPE_GUILDBATTLE, Protocol::CAN_JOIN_MAX_RANK, guildbattel_rank_list);
			return guildbattel_rank_list[rank - 1];
		}
	}

	return INVALID_GUILD_ID;
}

bool TerritoryWarConfig::CanEnterRoom(Role *role, int room_index)
{
	if (NULL == role)
	{
		return false;
	}

	if (INVALID_GUILD_ID == role->GetGuildID())
	{
		role->NoticeNum(errornum::EN_ENTER_NOT_GUILD);
		return false;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_ENTER_NOT_GUILD);
		return false;
	}

	int rank = guild->GetGuildBattleRank();

	if (rank < 1 || rank > Protocol::CAN_JOIN_MAX_RANK)
	{
		role->NoticeNum(errornum::EN_ENTER_GUILD_NOT_HAV_QUALIFICATION);
		return false;
	}
	
	if (0 == room_index)
	{
		if (rank == 1 || rank == 2)
			return true;
	}
	else if (1 == room_index)
	{
		if (rank == 3 || rank == 4)
			return true;
	}
	else if (2 == room_index)
	{
		if (rank == 5 || rank == 6)
			return true;
	}
	else if (3 == room_index)
	{
		if (rank == 7 || rank == 8)
			return true;
	}
	else if (4 == room_index)
	{
		if (rank == 9 || rank == 10)
			return true;
	}

	return false;
}

const TerritroyWarBuildingCfg * TerritoryWarConfig::GetBuildingCfg(int building_id) const
{
	if (building_id <= 0)
	{
		return NULL;
	}

	const std::map<int,TerritroyWarBuildingCfg>::const_iterator iter = m_building_cfg_list.find(building_id);

	if (m_building_cfg_list.end() != iter) 
	{
		return &iter->second;
	}

	return NULL;
}

const TerritroyWarFightShopCfg * TerritoryWarConfig::GetFightShopCfg(short type,short goods_id) const
{
	if (type < 0 || type >= FIGHT_SHOP_MAX_TYPE || goods_id < 0)
	{
		return NULL;
	}

	const std::map<short, TerritroyWarFightShopCfg>::const_iterator iter = m_fight_shop_cfg_list[type].find(goods_id);

	if (m_fight_shop_cfg_list[type].end() != iter) 
	{
		return &iter->second;
	}

	return NULL;
}

const TerritroyWarReliveShopCfg * TerritoryWarConfig::GetReliveShopCfg(short goods_id) const
{
	if (goods_id < 0)
	{
		return NULL;
	}

	const std::map<short, TerritroyWarReliveShopCfg>::const_iterator iter = m_relive_shop_cfg_list.find(goods_id);

	if (m_relive_shop_cfg_list.end() != iter) 
	{
		return &iter->second;
	}

	return NULL;
}

const TerritroyWarSkillItemCfg * TerritoryWarConfig::GetSkillItemCfg(int skill_index) const
{
	std::map<int,  TerritroyWarSkillItemCfg>::const_iterator iter = m_skill_map.find(skill_index);
	if (m_skill_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

const TerritroyWarPersonalRewardCfg * TerritoryWarConfig::GetPersonalCreditCfg(int credit)const
{
	if (credit <= 0) return NULL;

	std::map<int,TerritroyWarPersonalRewardCfg>::const_iterator iter = m_person_credit_reward_cfg_list.begin();

	for (;iter != m_person_credit_reward_cfg_list.end();iter++)
	{
		if (iter->second.person_credit_min <= credit && credit <= iter->second.person_credit_max)
		{
			return &iter->second;
		}
	}

	return NULL;
}

const TerritroyWarPersonalRewardCfg * TerritoryWarConfig::GetPersonalCreditCfgByIndex(int index)const
{
	if (index <= 0) return NULL;

	std::map<int,TerritroyWarPersonalRewardCfg>::const_iterator iter = m_person_credit_reward_cfg_list.begin();

	for (;iter != m_person_credit_reward_cfg_list.end();iter++)
	{
		if (iter->second.reward_index == index)
		{
			return &iter->second;
		}
	}

	return NULL;
}

const TerritroyWarGuajiMonsterCfg* TerritoryWarConfig::GetGuajiMonsterCfg(int monster_id) const
{
	if (monster_id < 0)
	{
		return NULL;
	}

	const std::map<int, TerritroyWarGuajiMonsterCfg>::const_iterator iter = m_guaji_monster_cfg_list.find(monster_id);

	if (m_guaji_monster_cfg_list.end() != iter) 
	{
		return &iter->second;
	}

	return NULL;
}

const TerritroyWarActivityEndCfg * TerritoryWarConfig::GetActivityEndCfg(int room_index,short reward_index) const
{
	if (room_index <= 0 || room_index > 5)
		return NULL;

	if (reward_index < 0)
		return NULL;
	
	const std::map<int, TerritroyWarActivityEndCfg>::const_iterator iter = m_activity_reward_cfg_list[room_index - 1].find(reward_index);

	if (m_activity_reward_cfg_list[room_index - 1].end() != iter) 
	{
		return &iter->second;
	}

	return NULL;
}
