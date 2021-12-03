#include "petconfig.hpp"
#include "obj/character/role.h"
#include "item/itemextern.hpp"
#include "world.h"
#include "item/itempool.h"
#include "skill/skillpool.hpp"
#include "protocal/msgpet.h"
#include <servercommon/configcommon.h>


PetConfig::PetConfig() : m_all_pet_cfg_count(0), m_egg_cfg_count(0), m_reward_cfg_count(0), m_grade_cfg_count(0), m_extra_attr_cfg_count(0), m_skill_cfg_list_count(0), 
	m_max_special_img_id(0), m_max_qinmi_level(0), m_food_market_cfg_count(0)
{
	memset(m_food_item_id_list, 0, sizeof(m_food_item_id_list));
}

PetConfig::~PetConfig()
{

}

bool PetConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "PetConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 其他配置信息
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宠物信息
		PugiXmlNode root_element = RootElement.child("pet_info");
		if (root_element.empty())
		{
			*err = configname + ": no [pet_info].";
			return false;
		}

		iRet = this->InitPetInfoCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPetInfoCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宠物升级
		PugiXmlNode root_element = RootElement.child("pet_uplevel");
		if (root_element.empty())
		{
			*err = configname + ": no [pet_uplevel].";
			return false;
		}

		iRet = this->InitPetUplevelCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPetUplevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宠物进阶
		PugiXmlNode root_element = RootElement.child("pet_upgrade");
		if (root_element.empty())
		{
			*err = configname + ": no [pet_upgrade].";
			return false;
		}

		iRet = this->InitPetUpgradeCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPetUpgradeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宠物进阶成功率
		PugiXmlNode root_element = RootElement.child("pet_upgrade_rate");
		if (root_element.empty())
		{
			*err = configname + ": no [pet_upgrade_rate].";
			return false;
		}

		iRet = this->InitPetUpgradeRateCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPetUpgradeRateCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宠物抽奖
		PugiXmlNode root_element = RootElement.child("pet_choujiang");
		if (root_element.empty())
		{
			*err = configname + ": no [pet_choujiang].";
			return false;
		}

		iRet = this->InitPetChoujiangRewardCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPetChoujiangRewardCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宠物蛋
		PugiXmlNode root_element = RootElement.child("pet_egg");
		if (root_element.empty())
		{
			*err = configname + ": no [pet_egg].";
			return false;
		}

		iRet = this->InitPetEggCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPetEggCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宠物羁绊额外属性
		PugiXmlNode root_element = RootElement.child("extra_attr");
		if (root_element.empty())
		{
			*err = configname + ": no [extra_attr].";
			return false;
		}

		iRet = this->InitPetExtraAttrCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPetExtraAttrCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宠物技能
		PugiXmlNode root_element = RootElement.child("skill");
		if (root_element.empty())
		{
			*err = configname + ": no [skill].";
			return false;
		}

		iRet = this->InitPetSkillCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPetSkillCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宠物技能等级
		PugiXmlNode root_element = RootElement.child("skill_level");
		if (root_element.empty())
		{
			*err = configname + ": no [skill_level].";
			return false;
		}

		iRet = this->InitPetSkillLevelCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPetSkillLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宠物幻化
		PugiXmlNode root_element = RootElement.child("special_img_cfg");
		if (root_element.empty())
		{
			*err = configname + ": no [special_img_cfg].";
			return false;
		}

		iRet = this->InitPetSpecialCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPetSpecialCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宠物喂养
		PugiXmlNode root_element = RootElement.child("qinmi_level");
		if (root_element.empty())
		{
			*err = configname + " no [qinmi_level]";
			return false;
		}

		iRet = this->InitPetQinmiLevelCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPetQinmiLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 吃货市场
		PugiXmlNode root_element = RootElement.child("food_market");
		if (root_element.empty())
		{
			*err = configname + " no [food_market]";
			return false;
		}

		iRet = this->InitPetFoodMarketCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPetQinmiLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 幻化升级
		PugiXmlNode root_element = RootElement.child("special_img_uplevel_cfg");
		if (root_element.empty())
		{
			*err = configname + " no [special_img_uplevel_cfg]";
			return false;
		}

		iRet = this->InitPetSpecialUplevelCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPetSpecialUplevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int PetConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}
	
	if (!PugiGetSubNodeValue(data_element, "low_chou_consume_gold", m_other_cfg.low_chou_consume_gold) || m_other_cfg.low_chou_consume_gold <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "low_chou_consume_item_id", m_other_cfg.low_chou_consume_item_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.low_chou_consume_item_id))
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "high_chou_consume_gold", m_other_cfg.high_chou_consume_gold) || m_other_cfg.high_chou_consume_gold <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "high_chou_consume_item_id", m_other_cfg.high_chou_consume_item_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.high_chou_consume_item_id))
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "cold_time", m_other_cfg.free_chou_cold_time_hour) || m_other_cfg.free_chou_cold_time_hour < 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "rename_card_id", m_other_cfg.rename_card_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.rename_card_id))
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "food_market_free_times_per_day", m_other_cfg.food_market_free_times_per_day) || m_other_cfg.food_market_free_times_per_day < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "food_market_once_need_gold", m_other_cfg.food_market_once_need_gold) || m_other_cfg.food_market_once_need_gold <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "food_market_ten_times_need_gold", m_other_cfg.food_market_ten_times_need_gold) || m_other_cfg.food_market_ten_times_need_gold <= 0)
	{
		return -9;
	}

	return 0;
}

int PetConfig::InitPetInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_all_pet_cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_all_pet_cfg_count >= PET_MAX_COUNT_LIMIT)
		{
			return -2;
		}

		PetInfoConfig &pet_cfg = m_all_pet_cfg_list[m_all_pet_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "id", pet_cfg.pet_id) || pet_cfg.pet_id != m_all_pet_cfg_count)
		{
			return -3;
		}

		std::string name;
		if (!PugiGetSubNodeValue(data_element, "name", name))
		{
			return -5;
		}
		if (name.length() >= sizeof(pet_cfg.pet_name))
		{
			return -6;
		}

		memcpy(pet_cfg.pet_name, name.c_str(), (int)name.length());
		pet_cfg.pet_name[name.length()] = '\0';

		if (!PugiGetSubNodeValue(data_element, "active_skill_num", pet_cfg.active_skill_num) || pet_cfg.active_skill_num < 0 || pet_cfg.active_skill_num > 1)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "passive_skill_num", pet_cfg.passive_skill_num) || pet_cfg.passive_skill_num < 0 || pet_cfg.passive_skill_num > 2)
		{
			return -8;
		}
		
		++ m_all_pet_cfg_count;
		data_element = data_element.next_sibling();
	}
	return 0;
}

int PetConfig::InitPetUplevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int id = -1;
		if (!PugiGetSubNodeValue(data_element, "id", id) || id < 0 || id >= PET_MAX_COUNT_LIMIT)
		{
			return -1;
		}

		if (m_all_pet_cfg_list[id].max_level > PET_MAX_LEVEL_LIMIT)
		{
			return -20;
		}
	
		int level = -1;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level != m_all_pet_cfg_list[id].max_level + 1)
		{
			return -2;
		}
		m_all_pet_cfg_list[id].max_level = level;

		PetLevelConfig &level_cfg = m_all_pet_cfg_list[id].level_cfg_list[level];
		level_cfg.level = level;
		

		if (!PugiGetSubNodeValue(data_element, "uplevel_consume_coin", level_cfg.uplevel_consume_coin) || level_cfg.uplevel_consume_coin < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", level_cfg.maxhp) || level_cfg.maxhp < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", level_cfg.gongji) || level_cfg.gongji < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", level_cfg.fangyu) || level_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", level_cfg.mingzhong) || level_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", level_cfg.shanbi) || level_cfg.shanbi < 0)
		{
			return -105;
		}

		data_element = data_element.next_sibling();
	}
	
	return 0;
}

int PetConfig::InitPetUpgradeCfg(PugiXmlNode RootElement)
{
	for (int id = 0; id < m_all_pet_cfg_count && id < PET_MAX_COUNT_LIMIT; ++ id)
	{
		PugiXmlNode data_element = RootElement.child("data");
		if (data_element.empty())
		{
			return -10000;
		}

		m_grade_cfg_count = 0;
		while (!data_element.empty())
		{
			if (m_all_pet_cfg_list[id].max_grade > PET_MAX_GRADE_LIMIT)
			{
				return -20;
			}

			int grade = -1;
			if (!PugiGetSubNodeValue(data_element, "grade", grade) || grade != m_all_pet_cfg_list[id].max_grade + 1)
			{
				return -2;
			}
			m_all_pet_cfg_list[id].max_grade = grade;

			PetGradeConfig &grade_cfg = m_all_pet_cfg_list[id].grade_cfg_list[grade];
			grade_cfg.grade = grade;

			if (!PugiGetSubNodeValue(data_element, "consume_stuff_id", grade_cfg.consume_stuff_id) || NULL == ITEMPOOL->GetItem(grade_cfg.consume_stuff_id))
			{
				return -5;
			}

			if (!PugiGetSubNodeValue(data_element, "consume_stuff_num", grade_cfg.consume_stuff_num) || grade_cfg.consume_stuff_num <= 0)
			{
				return -6;
			}

			if (!PugiGetSubNodeValue(data_element, "max_bless_value", grade_cfg.max_bless_value) || grade_cfg.max_bless_value <= 0)
			{
				return -7;
			}

			if (!PugiGetSubNodeValue(data_element, "broadcast", grade_cfg.is_broadcast) || grade_cfg.is_broadcast < 0 || grade_cfg.is_broadcast > 1)
			{
				return -8;
			}

			if (!PugiGetSubNodeValue(data_element, "maxhp", grade_cfg.maxhp) || grade_cfg.maxhp < 0)
			{
				return -101;
			}

			if (!PugiGetSubNodeValue(data_element, "gongji", grade_cfg.gongji) || grade_cfg.gongji < 0)
			{
				return -102;
			}

			if (!PugiGetSubNodeValue(data_element, "fangyu", grade_cfg.fangyu) || grade_cfg.fangyu < 0)
			{
				return -103;
			}

			if (!PugiGetSubNodeValue(data_element, "mingzhong", grade_cfg.mingzhong) || grade_cfg.mingzhong < 0)
			{
				return -104;
			}

			if (!PugiGetSubNodeValue(data_element, "shanbi", grade_cfg.shanbi) || grade_cfg.shanbi < 0)
			{
				return -105;
			}

			++ m_grade_cfg_count;
			data_element = data_element.next_sibling();
		}
	}

	// 验证阶数相同
	/*
	for (int id = 1; id < m_all_pet_cfg_count && id < PET_MAX_COUNT_LIMIT; ++ id)
	{
		if (pet_grade_cfg_max_count[0] != pet_grade_cfg_max_count[id])
		{
			return -200;
		}
	}
*/
	
	return 0;
}

int PetConfig::InitPetUpgradeRateCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int next_bless = 0;
	while (!data_element.empty())
	{
		int grade = -1;
		if (!PugiGetSubNodeValue(data_element, "grade", grade) || grade < 0 || grade >= m_grade_cfg_count || grade > PET_MAX_GRADE_LIMIT)
		{
			return -2;
		}

		PetUpGradeRateConfig rate_temp;

		rate_temp.grade = grade;

		if (!PugiGetSubNodeValue(data_element, "low_bless", rate_temp.low_bless) || rate_temp.low_bless != next_bless)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "high_bless", rate_temp.high_bless) || rate_temp.high_bless <= rate_temp.low_bless)
		{
			return -6;
		}

		next_bless = rate_temp.high_bless + 1;

		if (!PugiGetSubNodeValue(data_element, "succ_rate", rate_temp.succ_rate) || rate_temp.succ_rate < 0 || rate_temp.succ_rate > 10000)
		{
			return -7;
		}

		if (rate_temp.succ_rate >= 10000)
		{
			next_bless = 0;
		}

		if (!PugiGetSubNodeValue(data_element, "fail_add_1_rate", rate_temp.fail_add_1_rate) || rate_temp.fail_add_1_rate < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "fail_add_2_rate", rate_temp.fail_add_2_rate) || rate_temp.fail_add_2_rate < 0)
		{
			return -9;
		}
		
		if (!PugiGetSubNodeValue(data_element, "fail_add_3_rate", rate_temp.fail_add_3_rate) || rate_temp.fail_add_3_rate < 0)
		{
			return -10;
		}

		if (rate_temp.fail_add_1_rate + rate_temp.fail_add_2_rate + rate_temp.fail_add_3_rate != 100)
		{
			return -11;
		}

		m_grade_cfg_list[grade].vect.push_back(rate_temp);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int PetConfig::InitPetChoujiangRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_reward_cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_reward_cfg_count >= PET_REWARD_CFG_COUNT_LIMIT)
		{
			return -20;
		}

		PetRewardConfig &reward_cfg = m_pet_reward_list[m_reward_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "seq", reward_cfg.seq) || reward_cfg.seq != m_reward_cfg_count)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "is_get_reward", reward_cfg.is_get_reward) || reward_cfg.is_get_reward < 0 || reward_cfg.is_get_reward > 1)
		{
			return -2;
		}

		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -3;
		}

		if (!reward_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "low_weight", reward_cfg.low_weight) || reward_cfg.low_weight < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "hight_weight", reward_cfg.height_weight) || reward_cfg.height_weight < 0)
		{
			return -6;
		}

		int is_broadcast = 0;
		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast))
		{
			return -7;
		}
		reward_cfg.is_broadcast = (is_broadcast != 0);
		
		++ m_reward_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int PetConfig::InitPetEggCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_egg_cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_egg_cfg_count >= PET_EGG_MAX_COUNT_LIMIT)
		{
			return -20;
		}

		PetEggConfig &egg_cfg = m_egg_cfg_list[m_egg_cfg_count];
		if (!PugiGetSubNodeValue(data_element, "item_id", egg_cfg.item_id) || NULL == ITEMPOOL->GetItem(egg_cfg.item_id))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "transform_item_id", egg_cfg.transform_item_id) || NULL == ITEMPOOL->GetItem(egg_cfg.transform_item_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "transform_item_num", egg_cfg.transform_item_num) || egg_cfg.transform_item_num <= 0)
		{
			return -3;
		}

		if (PugiGetSubNodeValue(data_element, "pet_id", egg_cfg.pet_id))
		{
			if (egg_cfg.pet_id < 0 || egg_cfg.pet_id >= PET_MAX_COUNT_LIMIT || egg_cfg.pet_id >= m_all_pet_cfg_count)
			{
				return -4;
			}
		}

		++ m_egg_cfg_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int PetConfig::InitPetExtraAttrCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_extra_attr_cfg_count = 0;

	while (!data_element.empty())
	{
		
		if (m_extra_attr_cfg_count >= PET_MAX_EXTRA_ATTR_CFG_COUNT_LIMIT)
		{
			return -20;
		}

		PetExtraAttrConfig &extra_attr_cfg = m_extra_attr_cfg_list[m_extra_attr_cfg_count];

		if (!PugiGetSubNodeValue(data_element, "pet_id_1", extra_attr_cfg.pet_id_list[0]) || extra_attr_cfg.pet_id_list[0] < 0 || extra_attr_cfg.pet_id_list[0] >= PET_MAX_COUNT_LIMIT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "pet_id_2", extra_attr_cfg.pet_id_list[1]) || extra_attr_cfg.pet_id_list[1] < 0 || extra_attr_cfg.pet_id_list[1] >= PET_MAX_COUNT_LIMIT)
		{
			return -2;
		}

		if (PugiGetSubNodeValue(data_element, "pet_id_3", extra_attr_cfg.pet_id_list[2]))
		{
			if (extra_attr_cfg.pet_id_list[2] < 0 || extra_attr_cfg.pet_id_list[2] >= PET_MAX_COUNT_LIMIT)
			{
				return -3;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "seq", extra_attr_cfg.seq) || extra_attr_cfg.seq != m_extra_attr_cfg_count)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", extra_attr_cfg.maxhp) || extra_attr_cfg.maxhp < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", extra_attr_cfg.gongji) || extra_attr_cfg.gongji < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", extra_attr_cfg.fangyu) || extra_attr_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", extra_attr_cfg.mingzhong) || extra_attr_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", extra_attr_cfg.shanbi) || extra_attr_cfg.shanbi < 0)
		{
			return -105;
		}

		++ m_extra_attr_cfg_count;
		data_element = data_element.next_sibling();
	}
	return 0;
}

int PetConfig::InitPetSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_skill_id_2_skill_index_map.clear();
	m_skill_cfg_list_count = 0;

	while (!data_element.empty())
	{
		int skill_index = 0;
		if (!PugiGetSubNodeValue(data_element, "skill_index", skill_index) || skill_index != m_skill_cfg_list_count)
		{
			return -1;
		}
	
		PetSkillConfig &skill_cfg = m_skill_cfg_list[skill_index];
		skill_cfg.skill_index = skill_index;

		if (!PugiGetSubNodeValue(data_element, "skill_id", skill_cfg.skill_id) || NULL == SKILLPOOL->GetSkill(skill_cfg.skill_id))
		{
			return -2;
		}

		if (m_skill_id_2_skill_index_map.find(skill_cfg.skill_id) != m_skill_id_2_skill_index_map.end())
		{
			return -3;
		}

		m_skill_id_2_skill_index_map[skill_cfg.skill_id] = skill_index;

		if (PugiGetSubNodeValue(data_element, "own_by_pet_id", skill_cfg.own_by_pet_id))
		{
			if (NULL == this->GetPetInfo(skill_cfg.own_by_pet_id))
			{
				return -4;
			}
		}
		else
		{
			skill_cfg.own_by_pet_id = INVALID_PET_ID;
		}

		++ m_skill_cfg_list_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int PetConfig::InitPetSkillLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int skill_id = 0;
		if (!PugiGetSubNodeValue(data_element, "skill_id", skill_id) || NULL == SKILLPOOL->GetSkill(skill_id))
		{
			return -1;
		}

		if (m_skill_id_2_skill_index_map.find(skill_id) == m_skill_id_2_skill_index_map.end())
		{
			return -2;
		}

		int skill_index = m_skill_id_2_skill_index_map[skill_id];
		PetSkillConfig &skill_cfg = m_skill_cfg_list[skill_index];

		if (skill_cfg.max_skill_level >= PetSkillConfig::MAX_SKILL_LEVEL_LIMIT)
		{
			return -3;
		}

		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level != skill_cfg.max_skill_level + 1)
		{
			return -4;
		}

		skill_cfg.max_skill_level = level;

		PetSkillConfig::LevelConfig &level_cfg = skill_cfg.skill_level_cfglist[level];
		level_cfg.level = level;

		if (!PugiGetSubNodeValue(data_element, "consume_item_id", level_cfg.consume_item_id) || NULL == ITEMPOOL->GetItem(level_cfg.consume_item_id))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "consume_item_num", level_cfg.consume_item_num) || level_cfg.consume_item_num <= 0)
		{
			return -6;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int PetConfig::InitPetSpecialCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_max_special_img_id = 0;

	while (!data_element.empty())
	{
		if (m_max_special_img_id >= PET_SPECIAL_IMG_MAX_ID_LIMIT)
		{
			return -1;
		}

		int image_id = 0;
		if (!PugiGetSubNodeValue(data_element, "image_id", image_id) || image_id != m_max_special_img_id + 1)
		{
			return -2;
		}

		PetSpecialConfig &special_img_cfg = m_special_img_cfg_list[image_id];
		special_img_cfg.image_id = image_id;
		m_max_special_img_id = image_id;

		if (!PugiGetSubNodeValue(data_element, "maxhp", special_img_cfg.maxhp) || special_img_cfg.maxhp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", special_img_cfg.gongji) || special_img_cfg.gongji < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", special_img_cfg.fangyu) || special_img_cfg.fangyu < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", special_img_cfg.mingzhong) || special_img_cfg.mingzhong < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", special_img_cfg.shanbi) || special_img_cfg.shanbi < 0)
		{
			return -9;
		}

		data_element = data_element.next_sibling();
	}
	return 0;
}

int PetConfig::InitPetQinmiLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_max_qinmi_level = 0;

	while (!data_element.empty())
	{
		int qinmi_level = 0;
		if (!PugiGetSubNodeValue(data_element, "qinmi_level", qinmi_level) || (qinmi_level != m_max_qinmi_level && qinmi_level != m_max_qinmi_level + 1))
		{
			return -1;
		}

		if (qinmi_level > PET_QINMI_LEVEL_MAX_LEVEL)
		{
			return -1000;
		}

		m_qinmi_level_cfg_list[qinmi_level].qinmi_level = qinmi_level;
		m_max_qinmi_level = qinmi_level;

		if (m_qinmi_level_cfg_list[qinmi_level].max_star >= PET_QINMI_LEVEL_MAX_STAR)
		{
			return -2000;
		}

		int star = 0;
		if (!PugiGetSubNodeValue(data_element, "star", star) || star != m_qinmi_level_cfg_list[qinmi_level].max_star + 1)
		{
			return -2;
		}

		PetQinmiLevelConfig::StarConfig &star_cfg = m_qinmi_level_cfg_list[qinmi_level].star_cfg_list[star];
		star_cfg.star = star;
		m_qinmi_level_cfg_list[qinmi_level].max_star = star;

		if (!PugiGetSubNodeValue(data_element, "need_qinmi_val", star_cfg.need_qinmi_val) || star_cfg.need_qinmi_val <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "ignore_fangyu_percent", star_cfg.ignore_fangyu_percent) || star_cfg.ignore_fangyu_percent < 0 || star_cfg.ignore_fangyu_percent > 10000)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", star_cfg.maxhp) || star_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", star_cfg.gongji) || star_cfg.gongji < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", star_cfg.fangyu) || star_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", star_cfg.mingzhong) || star_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", star_cfg.shanbi) || star_cfg.shanbi < 0)
		{
			return -104;
		}

		data_element = data_element.next_sibling();
	}

	for (int index = 0; index <= m_max_qinmi_level && index <= PET_QINMI_LEVEL_MAX_LEVEL; ++ index)
	{
		if (m_qinmi_level_cfg_list[index].max_star != PET_QINMI_LEVEL_MAX_STAR)
		{
			return -3000;
		}
	}

	return 0;
}

int PetConfig::InitPetFoodMarketCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_food_market_cfg_count = 0;
	int real_food_count = 0;

	while (!data_element.empty())
	{
		int seq = -1;
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq != m_food_market_cfg_count)
		{
			return -1;
		}

		if (seq >= PET_FOOD_MARKET_MAX_REWARD_COUNT)
		{
			return -1000;
		}

		PetFoodMarketConfig &food_market_cfg = m_food_market_cfg_list[seq];
		food_market_cfg.seq = seq;

		{
			PugiXmlNode item_element = data_element.child("reward_item");
			if (item_element.empty())
			{
				return -100;
			}

			if (!food_market_cfg.reward_item.ReadConfig(item_element))
			{
				return -101;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "low_weight", food_market_cfg.low_weight) || food_market_cfg.low_weight < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "hight_weight", food_market_cfg.hight_weight) || food_market_cfg.hight_weight < 0)
		{
			return -3;
		}

		int can_feed = 0;
		if (!PugiGetSubNodeValue(data_element, "can_feed", can_feed) || can_feed > 1 || can_feed < 0)
		{
			return -4;
		}
		food_market_cfg.can_feed = (0 != can_feed);

		if (food_market_cfg.can_feed)
		{
			if (real_food_count >= PET_FOOD_MARKET_MAX_FOOD_COUNT)
			{
				return -200;
			}

			m_food_item_id_list[real_food_count ++] = seq;
		}

		if (!PugiGetSubNodeValue(data_element, "add_qinmi_val", food_market_cfg.add_qinmi_val) || food_market_cfg.add_qinmi_val < 0)
		{
			return -5;
		}

		if (food_market_cfg.can_feed && food_market_cfg.add_qinmi_val <= 0)
		{
			return -6;
		}

		++ m_food_market_cfg_count;
		data_element = data_element.next_sibling();
	}

	if (real_food_count != PET_FOOD_MARKET_MAX_FOOD_COUNT)
	{
		return -2000;
	}

	return 0;
}

int PetConfig::InitPetSpecialUplevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int image_id = 0;
	int last_image_id = 0;
	int level = 0;
	int last_level = -1;		// 从0开始方便客户端

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "image_id", image_id) || image_id <= 0 || image_id > m_max_special_img_id)
		{
			return -1;
		}

		if (last_image_id != image_id)
		{
			last_image_id = image_id;
			last_level = -1;
		}

		if (!PugiGetSubNodeValue(data_element, "level", level) || level !=  last_level + 1)
		{
			return -3;
		}
		last_level = level;

		m_special_img_uplevel_map[image_id].max_level = level;

		PetSpecialImageUplevelConfig::UplevelItemConfig &special_img_uplevel_cfg = m_special_img_uplevel_map[image_id].uplevel_cfg_list[level];

		if (!PugiGetSubNodeValue(data_element, "item_id", special_img_uplevel_cfg.item_id) || NULL == ITEMPOOL->GetItem(special_img_uplevel_cfg.item_id))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "item_num", special_img_uplevel_cfg.item_num) || special_img_uplevel_cfg.item_num <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", special_img_uplevel_cfg.maxhp) || special_img_uplevel_cfg.maxhp < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", special_img_uplevel_cfg.gongji) || special_img_uplevel_cfg.gongji < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", special_img_uplevel_cfg.fangyu) || special_img_uplevel_cfg.fangyu < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", special_img_uplevel_cfg.mingzhong) || special_img_uplevel_cfg.mingzhong < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", special_img_uplevel_cfg.shanbi) || special_img_uplevel_cfg.shanbi < 0)
		{
			return -10;
		}

		data_element = data_element.next_sibling();
	}
	return 0;
}

const PetLevelConfig * PetConfig::GetPetLevelCfg(int id, int level)
{
	if (id < 0 || id >= m_all_pet_cfg_count || id >= PET_MAX_COUNT_LIMIT)
	{
		return NULL;
	}

	if (level <= 0 || level > m_all_pet_cfg_list[id].max_level || level > PET_MAX_LEVEL_LIMIT)
	{
		return NULL;
	}

	return &m_all_pet_cfg_list[id].level_cfg_list[level];
}


const PetGradeConfig * PetConfig::GetPetGradeCfg(int id, int grade)
{
	if (id < 0 || id >= m_all_pet_cfg_count || id >= PET_MAX_COUNT_LIMIT)
	{
		return NULL;
	}

	if (grade < 0 || grade > m_all_pet_cfg_list[id].max_grade || grade > PET_MAX_GRADE_LIMIT)
	{
		return NULL;
	}

	return &m_all_pet_cfg_list[id].grade_cfg_list[grade];
}


const PetRewardConfig * PetConfig::GetRandomRewardCfg(int type)
{
	if (type < PET_CHOU_TYPE_LOW || type >= PET_CHOU_TYPE_MAX)
	{
		return NULL;
	}

	int all_weight = 0;
	int weight_list[PET_REWARD_CFG_COUNT_LIMIT];
	memset(weight_list, 0 , sizeof(weight_list));

	switch (type)
	{
	case PET_CHOU_TYPE_LOW:
		{
			for (int i = 0; i < m_reward_cfg_count && i < PET_REWARD_CFG_COUNT_LIMIT; ++ i)
			{
				all_weight += m_pet_reward_list[i].low_weight;
				weight_list[i] = m_pet_reward_list[i].low_weight;
			}
		}
		break;

	case PET_CHOU_TYPE_HIGH:
		{
			for (int i = 0; i < m_reward_cfg_count && i < PET_REWARD_CFG_COUNT_LIMIT; ++ i)
			{
				all_weight += m_pet_reward_list[i].height_weight;
				weight_list[i] = m_pet_reward_list[i].height_weight;
			}
		}
		break;
	}

	if (all_weight <= 0)
	{
		return NULL;
	}

	int rand_weight = RandomNum(all_weight);

	for (int index = 0; index < m_reward_cfg_count && index < PET_REWARD_CFG_COUNT_LIMIT; ++ index)
	{
		if (rand_weight < weight_list[index])
		{
			return &m_pet_reward_list[index];
		}
		rand_weight -= weight_list[index];
	}

	return NULL;
}

const PetEggConfig * PetConfig::GetPetEggCfg(int pet_id)
{
	if (pet_id < 0 || pet_id >= m_all_pet_cfg_count || pet_id >= PET_MAX_COUNT_LIMIT)
	{
		return NULL;
	}

	for (int i = 0; i < m_egg_cfg_count && i < PET_EGG_MAX_COUNT_LIMIT; ++ i)
	{
		if (m_egg_cfg_list[i].pet_id == pet_id)
		{
			return &m_egg_cfg_list[i];
		}
	}

	return NULL;
}

const PetEggConfig * PetConfig::GetPetEggCfgByItemid(ItemID egg_id)
{
	if (NULL == ITEMPOOL->GetItem(egg_id))
	{
		return NULL;
	}

	for (int i = 0; i < m_egg_cfg_count && i < PET_EGG_MAX_COUNT_LIMIT; ++ i)
	{
		if (m_egg_cfg_list[i].item_id == egg_id)
		{
			return &m_egg_cfg_list[i];
		}
	}

	return NULL;
}

const PetInfoConfig * PetConfig::GetPetInfo(int id)
{
	if (id < 0 || id >= m_all_pet_cfg_count || id >= PET_MAX_COUNT_LIMIT)
	{
		return NULL;
	}

	return &m_all_pet_cfg_list[id];
}

const PetUpGradeRateConfig * PetConfig::GetSuccRate(int grade, short bless)
{
	if (bless < 0 || bless >= SHRT_MAX)
	{
		return NULL;
	}

	if (grade < 0 || grade > PET_MAX_GRADE_LIMIT || grade >= m_grade_cfg_count)
	{
		return NULL;
	}

	for (std::vector<PetUpGradeRateConfig>::const_iterator it = m_grade_cfg_list[grade].vect.begin(); it != m_grade_cfg_list[grade].vect.end(); ++ it)
	{
		if (bless >= it->low_bless && bless <= it->high_bless)
		{
			return &(*it);
		}
	}

	return NULL;
}


int PetUpGradeRateConfig::GetRandomZhufuOnFail() const
{
	int taotal_rate = fail_add_1_rate + fail_add_2_rate + fail_add_3_rate;
	if (taotal_rate <= 0)
	{
		return 0;
	}

	int rand_rate = RandomNum(taotal_rate);
	if (rand_rate < fail_add_1_rate)
	{
		return 8;
	}

	rand_rate -= fail_add_1_rate;
	if (rand_rate < fail_add_2_rate)
	{
		return 10;
	}

	rand_rate -= fail_add_2_rate;
	if (rand_rate < fail_add_3_rate)
	{
		return 12;
	}

	return 0;
}

const PetExtraAttrConfig * PetConfig::GetExtraAttrPetList(int index)
{
	if (index < 0 || index >= PET_MAX_EXTRA_ATTR_CFG_COUNT_LIMIT || index >= m_extra_attr_cfg_count)
	{
		return NULL;
	}

	return &m_extra_attr_cfg_list[index];
}

const PetSkillConfig * PetConfig::GetSkillWithId(int skill_id)
{
	std::map<int, int>::iterator it = m_skill_id_2_skill_index_map.find(skill_id);
	if (it == m_skill_id_2_skill_index_map.end())
	{
		return NULL;
	}

	return this->GetSkillWithIndex(it->second);
}

const PetSkillConfig * PetConfig::GetSkillWithIndex(int skill_index)
{
	if (skill_index < 0 || skill_index >= m_skill_cfg_list_count || skill_index >= PET_SKILL_CFG_MAX_COUNT_LIMIT)
	{
		return NULL;
	}

	return &m_skill_cfg_list[skill_index];
}

const PetSpecialConfig * PetConfig::GetSpecialImgCfg(int image_id)
{
	if (image_id <= 0 || image_id > m_max_special_img_id || image_id > PET_SPECIAL_IMG_MAX_ID_LIMIT)
	{
		return NULL;
	}

	return &m_special_img_cfg_list[image_id];
}

const PetQinmiLevelConfig::StarConfig * PetConfig::GetQinmiLevelStarCfg(int level, int star)
{
	if (level < 0 || level > m_max_qinmi_level || level > PET_QINMI_LEVEL_MAX_LEVEL)
	{
		return NULL;
	}

	if (star <= 0 || star > m_qinmi_level_cfg_list[level].max_star || star > PET_QINMI_LEVEL_MAX_STAR)
	{
		return NULL;
	}

	return &m_qinmi_level_cfg_list[level].star_cfg_list[star];
}

const PetFoodMarketConfig * PetConfig::GetFoodMarketCfg(int seq)
{
	if (seq < 0 || seq >= m_food_market_cfg_count || seq >= PET_FOOD_MARKET_MAX_REWARD_COUNT)
	{
		return NULL;
	}

	return &m_food_market_cfg_list[seq];
}

int PetConfig::GetPetQinmiLevelMaxStar(int level)
{
	if (level < 0 || level > m_max_qinmi_level || level > PET_QINMI_LEVEL_MAX_LEVEL)
	{
		return 0;
	}

	return m_qinmi_level_cfg_list[level].max_star;
}

int PetConfig::GetPetFoodSeq(int index)
{
	if (index < 0 || index >= PET_FOOD_MARKET_MAX_FOOD_COUNT)
	{
		return -1;
	}

	return m_food_item_id_list[index];
}

const PetFoodMarketConfig * PetConfig::GetFoodMarketOnceRandomRewardCfg()
{
	// 总权重
	int total_weight = 0;
	for (int index = 0; index < m_food_market_cfg_count && index < PET_FOOD_MARKET_MAX_REWARD_COUNT; ++ index)
	{
		total_weight += m_food_market_cfg_list[index].low_weight;
	}

	if (total_weight <= 0)
	{
		return NULL;
	}

	// 计算
	int rand_val = RandomNum(total_weight);
	for (int index = 0; index < m_food_market_cfg_count && index < PET_FOOD_MARKET_MAX_REWARD_COUNT; ++ index)
	{
		if (rand_val < m_food_market_cfg_list[index].low_weight)
		{
			return &m_food_market_cfg_list[index];
		}

		rand_val -= m_food_market_cfg_list[index].low_weight;
	}

	return NULL;
}

bool PetConfig::GetFoodMarketRandomRewards(int result_list[], int list_len)
{
	if (NULL == result_list || list_len <= 0 || list_len > Protocol::SC_CHOU_PET_FOOD_MARKET_MAX_TIMES)
	{
		return false;
	}

	// 总权重
	int total_weight = 0;
	for (int index = 0; index < m_food_market_cfg_count && index < PET_FOOD_MARKET_MAX_REWARD_COUNT; ++ index)
	{
		total_weight += m_food_market_cfg_list[index].hight_weight;
	}

	if (total_weight <= 0)
	{
		return false;
	}

	// 计算
	int rand_val_list[Protocol::SC_CHOU_PET_FOOD_MARKET_MAX_TIMES]; memset(rand_val_list, 0, sizeof(rand_val_list));
	for (int index = 0; index < list_len && index < Protocol::SC_CHOU_PET_FOOD_MARKET_MAX_TIMES; ++ index)
	{
		int rand_val = RandomNum(total_weight);
		rand_val_list[index] = rand_val;
	}

	for (int i = 0; i < list_len && i < Protocol::SC_CHOU_PET_FOOD_MARKET_MAX_TIMES; ++ i)
	{
		for (int j = 0; j < m_food_market_cfg_count && j < PET_FOOD_MARKET_MAX_REWARD_COUNT; ++ j)
		{
			if (rand_val_list[i] < m_food_market_cfg_list[j].hight_weight)
			{
				result_list[i] = m_food_market_cfg_list[j].seq;
				break;
			}

			rand_val_list[i] -= m_food_market_cfg_list[j].hight_weight;
		}
	}

	return true;
}

const PetSpecialImageUplevelConfig::UplevelItemConfig * PetConfig::GetSpecialImgUplevelCfg(const int image_id, const int to_level)
{
	if (image_id <= 0 || image_id > m_max_special_img_id || image_id > PET_SPECIAL_IMG_MAX_ID_LIMIT)
	{
		return NULL;
	}

	SpecialImgUplevelCfgMap::const_iterator iter = m_special_img_uplevel_map.find(image_id);
	if (iter == m_special_img_uplevel_map.end())
	{
		return NULL;
	}

	if (to_level < 0 || to_level > iter->second.max_level)
	{
		return NULL;
	}

	return &iter->second.uplevel_cfg_list[to_level];
}

