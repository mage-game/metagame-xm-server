#include "config/logicconfigmanager.hpp"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "babyconfig.hpp"

#include <algorithm>

BabyConfig::BabyConfig() : m_all_baby_count(0), m_grade_cfg_count(0), m_qifu_tree_cfg_count(0)
{
	memset(m_spirit_max_level_list, 0, sizeof(m_spirit_max_level_list));
}

BabyConfig::~BabyConfig()
{

}


bool BabyConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "BabyConfig", *err);

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
		// 宝宝信息
		PugiXmlNode root_element = RootElement.child("baby_info");
		if (root_element.empty())
		{
			*err = configname + ": no [baby_info].";
			return false;
		}

		iRet = this->InitBabyInfoCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBabyInfoCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宝宝升级
		PugiXmlNode root_element = RootElement.child("baby_uplevel");
		if (root_element.empty())
		{
			*err = configname + ": no [baby_uplevel].";
			return false;
		}

		iRet = this->InitBabyLevelCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBabyLevelCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宝宝进阶
		PugiXmlNode root_element = RootElement.child("baby_upgrade");
		if (root_element.empty())
		{
			*err = configname + ": no [baby_upgrade].";
			return false;
		}

		iRet = this->InitBabyGradeCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBabyGradeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宝宝进阶成功率
		PugiXmlNode root_element = RootElement.child("baby_upgrade_rate");
		if (root_element.empty())
		{
			*err = configname + ": no [baby_upgrade_rate].";
			return false;
		}

		iRet = this->InitBabyUpgradeRateCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBabyUpgradeRateCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宝宝祈福树
		PugiXmlNode root_element = RootElement.child("qifu_tree");
		if (root_element.empty())
		{
			*err = configname + ": no [qifu_tree].";
			return false;
		}

		iRet = this->InitBabyQifuTreeCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBabyQifuTreeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 宝宝超生信息
		PugiXmlNode root_element = RootElement.child("baby_chaosheng");
		if (root_element.empty())
		{
			*err = configname + ": no [baby_chaosheng].";
			return false;
		}

		iRet = this->InitBabyChaoshengCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBabyChaoshengCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 守护精灵信息
		PugiXmlNode root_element = RootElement.child("baby_spirit");
		if (root_element.empty())
		{
			*err = configname + ": no [baby_spirit].";
			return false;
		}

		iRet = this->InitBabySpiritCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitBabySpiritCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 特殊宝宝
		PugiXmlNode root_element = RootElement.child("special_baby");
		if (root_element.empty())
		{
			*err = configname + ": no [special_baby].";
			return false;
		}

		iRet = this->InitSpecialBabyCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitSpecialBabyCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
	
	return true;	
}

int BabyConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "free_rename_times", m_other_cfg.free_rename_times) || m_other_cfg.free_rename_times < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "rename_card_id", m_other_cfg.rename_card_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.rename_card_id))
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "born_need_grade", m_other_cfg.born_need_grade) || m_other_cfg.born_need_grade < 0)
	{
		return -3;
	}

	return 0;
}

int BabyConfig::InitBabyInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}
	
	m_all_baby_count = 0;

	while (!data_element.empty())
	{
		if (m_all_baby_count >= BABY_MAX_COUNT)
		{
			return -20;
		}

		int id = -1;
		if (!PugiGetSubNodeValue(data_element, "id", id) || id != m_all_baby_count)
		{
			return -1;
		}

		BabyInfoCfg &info_cfg = m_all_baby_list[m_all_baby_count];
		info_cfg.baby_id = id;

		std::string name;
		if (!PugiGetSubNodeValue(data_element, "name", name))
		{
			return -5;
		}
		if (name.length() >= sizeof(info_cfg.baby_name))
		{
			return -6;
		}

		memcpy(info_cfg.baby_name, name.c_str(), (int)name.length());
		info_cfg.baby_name[name.length()] = '\0';

		if (!PugiGetSubNodeValue(data_element, "maxhp", info_cfg.maxhp) || info_cfg.maxhp < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", info_cfg.gongji) || info_cfg.gongji < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", info_cfg.fangyu) || info_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", info_cfg.mingzhong) || info_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", info_cfg.shanbi) || info_cfg.shanbi < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", info_cfg.baoji) || info_cfg.baoji < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", info_cfg.jianren) || info_cfg.jianren < 0)
		{
			return -107;
		}

		++ m_all_baby_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int BabyConfig::InitBabyLevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int id = -1;
		if (!PugiGetSubNodeValue(data_element, "id", id) || id < 0 || id >= m_all_baby_count)
		{
			return -1;
		}

		if (m_all_baby_list[id].max_level >= BABY_MAX_LEVEL_LIMIT)
		{
			return -20;
		}

		int level = -1;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level != m_all_baby_list[id].max_level + 1)
		{
			return -2;
		}

		m_all_baby_list[id].max_level = level;

		BabyLevelCfg &level_cfg = m_all_baby_list[id].level_cfg_list[level];
		level_cfg.level = level;
		
		if (!PugiGetSubNodeValue(data_element, "uplevel_consume_item_1", level_cfg.stuff_id_1) || NULL == ITEMPOOL->GetItem(level_cfg.stuff_id_1))
		{
			return -33;
		}

		if (!PugiGetSubNodeValue(data_element, "uplevel_consume_num_1", level_cfg.stuff_num_1) || level_cfg.stuff_num_1 < 0)
		{
			return -34;
		}

		if (!PugiGetSubNodeValue(data_element, "uplevel_consume_item_2", level_cfg.stuff_id_2) || NULL == ITEMPOOL->GetItem(level_cfg.stuff_id_2))
		{
			return -35;
		}

		if (!PugiGetSubNodeValue(data_element, "uplevel_consume_num_2", level_cfg.stuff_num_2) || level_cfg.stuff_num_2 < 0)
		{
			return -36;
		}

		if (!PugiGetSubNodeValue(data_element, "uplevel_consume_item_3", level_cfg.stuff_id_3) || NULL == ITEMPOOL->GetItem(level_cfg.stuff_id_3))
		{
			return -37;
		}

		if (!PugiGetSubNodeValue(data_element, "uplevel_consume_num_3", level_cfg.stuff_num_3) || level_cfg.stuff_num_3 < 0)
		{
			return -38;
		}

		if (!PugiGetSubNodeValue(data_element, "uplevel_consume_item_4", level_cfg.stuff_id_4) || NULL == ITEMPOOL->GetItem(level_cfg.stuff_id_4))
		{
			return -39;
		}

		if (!PugiGetSubNodeValue(data_element, "uplevel_consume_num_4", level_cfg.stuff_num_4) || level_cfg.stuff_num_4 < 0)
		{
			return -40;
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

		if (!PugiGetSubNodeValue(data_element, "baoji", level_cfg.baoji) || level_cfg.baoji < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", level_cfg.jianren) || level_cfg.jianren < 0)
		{
			return -107;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int BabyConfig::InitBabyGradeCfg(PugiXmlNode RootElement)
{
	for (int id = 0; id < m_all_baby_count && id < BABY_MAX_COUNT; ++ id)
	{
		PugiXmlNode data_element = RootElement.child("data");
		if (data_element.empty())
		{
			return -10000;
		}

		m_grade_cfg_count = 0;
		while (!data_element.empty())
		{
			if (m_all_baby_list[id].max_grade > BABY_MAX_GRADE_LIMIT)
			{
				return -20;
			}

			int grade = -1;
			if (!PugiGetSubNodeValue(data_element, "grade", grade) || grade != m_all_baby_list[id].max_grade + 1)
			{
				return -2;
			}
			m_all_baby_list[id].max_grade = grade;

			BabyGradeConfig &grade_cfg = m_all_baby_list[id].grade_cfg_list[grade];
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

			if (!PugiGetSubNodeValue(data_element, "baoji", grade_cfg.baoji) || grade_cfg.baoji < 0)
			{
				return -106;
			}

			if (!PugiGetSubNodeValue(data_element, "jianren", grade_cfg.jianren) || grade_cfg.jianren < 0)
			{
				return -107;
			}

			++ m_grade_cfg_count;
			data_element = data_element.next_sibling();
		}
	}

	return 0;
}

int BabyConfig::InitBabyUpgradeRateCfg(PugiXmlNode RootElement)
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
		if (!PugiGetSubNodeValue(data_element, "grade", grade) || grade < 0 || grade >= m_grade_cfg_count || grade > BABY_MAX_GRADE_LIMIT)
		{
			return -2;
		}

		BabyUpGradeRateConfig rate_temp;

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

int BabyConfig::InitBabyQifuTreeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_qifu_tree_cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_qifu_tree_cfg_count >= BABY_MAX_QIFU_TREE_CFG_COUNT)
		{
			return -20;
		}

		int qifu_type = -1;
		if (!PugiGetSubNodeValue(data_element, "qifu_type", qifu_type) || qifu_type != m_qifu_tree_cfg_count + 1)
		{
			return -1;
		}

		if (qifu_type <= QIFU_TREE_MIN || qifu_type >= QIFU_TREE_MAX)
		{
			return -21;
		}

		BabyQifuTreeConfig &qifu_cfg =  m_qifu_tree_cfg_list[qifu_type];
		qifu_cfg.qifu_type = qifu_type;

		if (!PugiGetSubNodeValue(data_element, "qifu_consume_bind_gold", qifu_cfg.qifu_consume_bind_gold) || qifu_cfg.qifu_consume_bind_gold < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "qifu_consume_gold", qifu_cfg.qifu_consume_gold) || qifu_cfg.qifu_consume_gold < 0)
		{
			return -3;
		}

		if (qifu_cfg.qifu_consume_bind_gold <= 0 && qifu_cfg.qifu_consume_gold <= 0)
		{
			return -4;
		}

		if (qifu_cfg.qifu_consume_bind_gold > 0 && qifu_cfg.qifu_consume_gold > 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "activity_qifu_consume_bind_gold", qifu_cfg.activity_qifu_consume_bind_gold) || qifu_cfg.activity_qifu_consume_bind_gold < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "activity_qifu_consume_gold", qifu_cfg.activity_qifu_consume_gold) || qifu_cfg.activity_qifu_consume_gold < 0)
		{
			return -7;
		}

		if (qifu_cfg.activity_qifu_consume_bind_gold <= 0 && qifu_cfg.activity_qifu_consume_gold <= 0)
		{
			return -8;
		}

		if (qifu_cfg.activity_qifu_consume_bind_gold > 0 && qifu_cfg.activity_qifu_consume_gold > 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "qifu_reward_baby_id", qifu_cfg.qifu_reward_baby_id) || qifu_cfg.qifu_reward_baby_id < 0 || qifu_cfg.qifu_reward_baby_id >= m_all_baby_count)
		{
			return -10;
		}

		++ m_qifu_tree_cfg_count;
		data_element = data_element.next_sibling();
	}

	if (m_qifu_tree_cfg_count != BABY_MAX_QIFU_TREE_CFG_COUNT)
	{
		return -30;
	}

	return 0;
}

int BabyConfig::InitBabyChaoshengCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_chaosheng_cfg_count = 0;

	while (!data_element.empty())
	{
		if (m_chaosheng_cfg_count >= BABY_MAX_CHAOSHENG_COUNT)
		{
			return -1000;
		}

		int chaosheng_num = 0;
		if (!PugiGetSubNodeValue(data_element, "chaosheng_num", chaosheng_num) || chaosheng_num != m_chaosheng_cfg_count + 1)
		{
			return -1;
		}

		BabyChaoshengConfig &chaosheng_cfg = m_chaosheng_cfg_list[chaosheng_num];
		m_chaosheng_cfg_count = chaosheng_num;

		if (!PugiGetSubNodeValue(data_element, "need_gold", chaosheng_cfg.need_gold) || chaosheng_cfg.need_gold <= 0)
		{
			return -2;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int BabyConfig::InitBabySpiritCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int id = -1;
	memset(m_spirit_max_level_list, 0, sizeof(m_spirit_max_level_list));

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "id", id) || id < 0 || id >= BABY_SPIRIT_COUNT)
		{
			return -1;
		}

		if (m_spirit_max_level_list[id] >= BABY_MAX_SPIRIT_LEVEL_LIMIT)
		{
			return -1000;
		}

		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level != m_spirit_max_level_list[id] + 1)
		{
			return -2;
		}

		BabySpiritConfig &spirit_cfg = m_spirit_cfg_list[id][level];
		m_spirit_max_level_list[id] = level;

		if (!PugiGetSubNodeValue(data_element, "consume_item", spirit_cfg.consume_item) || NULL == ITEMPOOL->GetItem(spirit_cfg.consume_item))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "train_val", spirit_cfg.train_val) || spirit_cfg.train_val <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", spirit_cfg.maxhp) || spirit_cfg.maxhp < 0)
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", spirit_cfg.gongji) || spirit_cfg.gongji < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", spirit_cfg.fangyu) || spirit_cfg.fangyu < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", spirit_cfg.mingzhong) || spirit_cfg.mingzhong < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", spirit_cfg.shanbi) || spirit_cfg.shanbi < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", spirit_cfg.baoji) || spirit_cfg.baoji < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", spirit_cfg.jianren) || spirit_cfg.jianren < 0)
		{
			return -106;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int BabyConfig::InitSpecialBabyCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}
	while (!data_element.empty())
	{
		int special_baby_type = -1;
		if (!PugiGetSubNodeValue(data_element, "special_baby_type", special_baby_type) || special_baby_type < 0 || special_baby_type >= SPECIAL_BABY_TYPE_MAX)
		{
			return -1;
		}

		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "level", level) || level <= 0)
		{
			return -2;
		}

		auto &cfg = m_special_baby_list[special_baby_type];

		SpecialBabyLvCfg lv_cfg;
		lv_cfg.level = level;

		if (!PugiGetSubNodeValue(data_element, "speical_baby_quality", lv_cfg.speical_baby_quality) || lv_cfg.speical_baby_quality < 0 || lv_cfg.speical_baby_quality >= SPECIAL_BABY_QUALITY_MAX)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "active_item_id", lv_cfg.active_item_id) || NULL == ITEMPOOL->GetItem(lv_cfg.active_item_id))
		{
			return -4;
		}

		int iRet = lv_cfg.attr_cfg.ReadConfig(data_element);
		if (iRet < 0)
		{
			return iRet;
		}

		cfg.push_back(lv_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int BabyUpGradeRateConfig::GetRandomZhufuOnFail() const
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

const BabyUpGradeRateConfig * BabyConfig::GetSuccRate(int grade, short bless)
{
	if (bless < 0 || bless >= SHRT_MAX)
	{
		return NULL;
	}

	if (grade < 0 || grade > BABY_MAX_GRADE_LIMIT || grade >= m_grade_cfg_count)
	{
		return NULL;
	}

	for (std::vector<BabyUpGradeRateConfig>::const_iterator it = m_grade_cfg_list[grade].vect.begin(); it != m_grade_cfg_list[grade].vect.end(); ++ it)
	{
		if (bless >= it->low_bless && bless <= it->high_bless)
		{
			return &(*it);
		}
	}

	return NULL;
}

const BabyQifuTreeConfig * BabyConfig::GetQifuCfg(int type)
{
	if (type <= QIFU_TREE_MIN || type >= QIFU_TREE_MAX)
	{
		return NULL;
	}

	return &m_qifu_tree_cfg_list[type];
}

const BabyInfoCfg * BabyConfig::GetBabyInfo(int baby_id)
{
	if (baby_id < 0 || baby_id >= m_all_baby_count)
	{
		return NULL;
	}

	return &m_all_baby_list[baby_id];
}

const BabyGradeConfig * BabyConfig::GetBabyGradeCfg(int id, int grade)
{
	if (id < 0 || id >= m_all_baby_count || id >= BABY_MAX_COUNT)
	{
		return NULL;
	}

	if (grade < 0 || grade > m_all_baby_list[id].max_grade || grade > BABY_MAX_GRADE_LIMIT)
	{
		return NULL;
	}

	return &m_all_baby_list[id].grade_cfg_list[grade];
}

const BabyLevelCfg * BabyConfig::GetBabyLevelCfg(int id, int level)
{
	if (id < 0 || id >= m_all_baby_count || id >= BABY_MAX_COUNT)
	{
		return NULL;
	}

	if (level < 0 || level > m_all_baby_list[id].max_level || level > BABY_MAX_LEVEL_LIMIT)
	{
		return NULL;
	}

	return &m_all_baby_list[id].level_cfg_list[level];
}

int BabyConfig::GetBabyChaoshengGold(int num)
{
	if (num <= 0 || num > m_chaosheng_cfg_count || num > BABY_MAX_CHAOSHENG_COUNT)
	{
		return -1;
	}

	return m_chaosheng_cfg_list[num].need_gold;
}

const BabySpiritConfig * BabyConfig::GetBabySpiritCfg(int spirit_id, int level)
{
	if (spirit_id < 0 || spirit_id >= BABY_SPIRIT_COUNT || level <= 0 || level > m_spirit_max_level_list[spirit_id] || level > BABY_MAX_SPIRIT_LEVEL_LIMIT)
	{
		return NULL;
	}

	return &m_spirit_cfg_list[spirit_id][level];
}

const SpecialBabyLvCfg * BabyConfig::GetSpecialBabyLvCfg(int speical_baby_type, int quality, int lv)
{
	if (speical_baby_type < 0 || speical_baby_type >= SPECIAL_BABY_TYPE_MAX || quality < 0 || quality >= SPECIAL_BABY_QUALITY_MAX)
	{
		return nullptr;
	}

	auto it = std::find_if(m_special_baby_list[speical_baby_type].begin(), m_special_baby_list[speical_baby_type].end(), [quality, lv](const SpecialBabyLvCfg &tmp_cfg) {
		return quality == tmp_cfg.speical_baby_quality && lv == tmp_cfg.level;
	});

	if (it != m_special_baby_list[speical_baby_type].end())
	{
		return &(*it);
	}

	return nullptr;
}

