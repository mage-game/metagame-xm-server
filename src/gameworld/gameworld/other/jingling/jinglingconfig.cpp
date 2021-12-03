#include "item/itempool.h"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"
#include "obj/character/attribute.hpp"
#include "servercommon/servercommon.h"
#include "jinglingconfig.hpp"
#include "gamedef.h"
#include "skill/skillpool.hpp"
#include "equipment/equipment.h"

JingLingConfig::JingLingConfig() : m_halo_max_img_id(0), m_halo_max_level(0), m_is_open_special_cap_add(false)
{
	memset(m_skill_slot_open_consume_list, 0, sizeof(m_skill_slot_open_consume_list));
}

JingLingConfig::~JingLingConfig()
{

}

bool JingLingConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "JingLingConfig", *err);

	PugiXmlNode RootElement = document.document_element();

	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		// 其他
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitOther(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOther failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 升级
		PugiXmlNode root_element = RootElement.child("uplevel");
		if (root_element.empty())
		{
			*err = configname + ": no [uplevel].";
			return false;
		}

		iRet = this->InitUpLevelConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitUpLevelConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 天赋
		PugiXmlNode root_element = RootElement.child("talent_attr");
		if (root_element.empty())
		{
			*err = configname + ": no [talent_attr].";
			return false;
		}

		iRet = this->InitTalentConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTalentConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 组合
		PugiXmlNode root_element = RootElement.child("group");
		if (root_element.empty())
		{
			*err = configname + ": no [group].";
			return false;
		}

		iRet = this->InitGroupConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitGroupConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 来源
		PugiXmlNode root_element = RootElement.child("get_jingling");
		if (root_element.empty())
		{
			*err = configname + ": no [get_jingling].";
			return false;
		}

		iRet = this->InitGetConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitGetConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 精灵卡牌
		PugiXmlNode root_element = RootElement.child("jingling_card");
		if (root_element.empty())
		{
			*err = configname + ": no [jingling_card].";
			return false;
		}

		iRet = this->InitCardConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitCardConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 精灵升阶
		PugiXmlNode root_element = RootElement.child("upgrade");
		if (root_element.empty())
		{
			*err = configname + ": no [upgrade].";
			return false;
		}

		iRet = this->InitUpGradeConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitUpGradeConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 精灵进阶成功率
		PugiXmlNode root_element = RootElement.child("upgradeprob");
		if (root_element.empty())
		{
			*err = configname + ": no [upgradeprob].";
			return false;
		}

		iRet = this->InitUpGradeProbConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitUpGradeProbConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 精灵装备
		PugiXmlNode root_element = RootElement.child("equip");
		if (root_element.empty())
		{
			*err = configname + ": no [equip].";
			return false;
		}

		iRet = this->InitEquipConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitEquipConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//幻化形象
		PugiXmlNode root_element = RootElement.child("jingling_phantom");
		if (root_element.empty())
		{
			*err = configname + ": no [jingling_phantom].";
			return false;
		}

		iRet = this->InitPhantomConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitPhantomConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//灵魄聚灵
		PugiXmlNode root_element = RootElement.child("jingling_soul");
		if (root_element.empty())
		{
			*err = configname + ": no [jingling_soul].";
			return false;
		}

		iRet = this->InitSoulConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitSoulConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		//灵魄套装
		PugiXmlNode root_element = RootElement.child("soul_taozhuang");
		if (root_element.empty())
		{
			*err = configname + ": no [soul_taozhuang].";
			return false;
		}

		iRet = this->InitSoulTaoZhuangConfig(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitSoulTaoZhuangConfig failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 光环进阶（升级）配置
		PugiXmlNode root_element = RootElement.child("halo_uplevel");
		if (root_element.empty())
		{
			*err = configname + " xml not halo_uplevel node ";
			return false;
		}

		iRet = this->InitHaloUplevelCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitHaloUplevelCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 光环激活配置
		PugiXmlNode root_element = RootElement.child("halo_active");
		if (root_element.empty())
		{
			*err = configname + " xml not halo_active node ";
			return false;
		}

		iRet = this->InitHaloActiveCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitHaloActiveCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 悟性配置
		PugiXmlNode root_element = RootElement.child("wuxing");
		if (root_element.empty())
		{
			*err = configname + " xml not wuxing node ";
			return false;
		}

		iRet = this->InitWuXingCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitWuXingCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 仙阵配置
		PugiXmlNode root_element = RootElement.child("xianzhen");
		if (root_element.empty())
		{
			*err = configname + " xml not xianzhen node ";
			return false;
		}

		iRet = this->InitXianZhenCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitXianZhenCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 仙阵魂玉配置
		PugiXmlNode root_element = RootElement.child("xianzhen_hunyu");
		if (root_element.empty())
		{
			*err = configname + " xml not xianzhen_hunyu node ";
			return false;
		}

		iRet = this->InitXianZhenHunYuCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitXianZhenHunYuCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 技能配置
		PugiXmlNode root_element = RootElement.child("skill");
		if (root_element.empty())
		{
			*err = configname + " xml not skill node ";
			return false;
		}

		iRet = this->InitSkillCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitSkillCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 技能刷新配置
		PugiXmlNode root_element = RootElement.child("skill_refresh");
		if (root_element.empty())
		{
			*err = configname + " xml not skill_refresh node ";
			return false;
		}

		iRet = this->InitSkillRefreshCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitSkillRefreshCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 技能刷新概率配置
		PugiXmlNode root_element = RootElement.child("skill_refresh_rate");
		if (root_element.empty())
		{
			*err = configname + " xml not skill_refresh_rate node ";
			return false;
		}

		iRet = this->InitSkillRefreshRateCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitSkillRefreshRateCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 家园额外奖励
		PugiXmlNode root_element = RootElement.child("home_other_reward");
		if (root_element.empty())
		{
			*err = configname + " xml not home_other_reward node ";
			return false;
		}

		iRet = this->InitHomeOtherRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitHomeOtherRewardCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 精灵探险
		PugiXmlNode root_element = RootElement.child("explore");
		if (root_element.empty())
		{
			*err = configname + " xml not explore node ";
			return false;
		}

		iRet = this->InitExploreCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitExploreCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 精灵探险奖励
		PugiXmlNode root_element = RootElement.child("explore_reward");
		if (root_element.empty())
		{
			*err = configname + " xml not explore_reward node ";
			return false;
		}

		iRet = this->InitExploreRewardCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitExploreRewardCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 精灵探险名字
		PugiXmlNode root_element = RootElement.child("explore_name");
		if (root_element.empty())
		{
			*err = configname + " xml not explore_name node ";
			return false;
		}

		iRet = this->InitExploreNameCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitExploreNameCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 超强战力加成
		PugiXmlNode root_element = RootElement.child("huanhua_special_cap_add");
		if (root_element.empty())
		{
			*err = configname + " xml not huanhua_special_cap_add node ";
			return false;
		}

		iRet = this->InitHuanHuaSpecialCapAddCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitHuanHuaSpecialCapAddCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// 仙宠技能槽开启消耗
		PugiXmlNode root_element = RootElement.child("skill_slot_open_cousume");
		if (root_element.empty())
		{
			*err = configname + " xml not skill_slot_open_cousume node ";
			return false;
		}

		iRet = this->InitSkillSlotOpenConsumeCfg(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s InitSkillSlotOpenConsumeCfg fail %d ", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int JingLingConfig::InitOther(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "halo_upgrade_item_id", m_other_cfg.halo_upgrade_item_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.halo_upgrade_item_id))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "halo_normal_exp", m_other_cfg.halo_normal_exp) || m_other_cfg.halo_normal_exp < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "halo_baoji_rate", m_other_cfg.halo_baoji_rate) || m_other_cfg.halo_baoji_rate < 0 || m_other_cfg.halo_baoji_rate > 100)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "halo_baoji_exp", m_other_cfg.halo_baoji_exp) || m_other_cfg.halo_baoji_exp < m_other_cfg.halo_normal_exp)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "rename_cost", m_other_cfg.rename_cost) || m_other_cfg.rename_cost < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "xianzhen_stuff_id", m_other_cfg.xianzhen_stuff_id) || NULL == ITEMPOOL->GetItem(m_other_cfg.xianzhen_stuff_id))
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "xianzhen_add_exp", m_other_cfg.xianzhen_add_exp) || m_other_cfg.xianzhen_add_exp < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(data_element, "xianzhen_small_rate", m_other_cfg.xianzhen_small_rate) || m_other_cfg.xianzhen_small_rate < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(data_element, "xianzhen_small_exp", m_other_cfg.xianzhen_small_exp) || m_other_cfg.xianzhen_small_exp < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(data_element, "xianzhen_big_rate", m_other_cfg.xianzhen_big_rate) || m_other_cfg.xianzhen_big_rate < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(data_element, "xianzhen_big_exp", m_other_cfg.xianzhen_big_exp) || m_other_cfg.xianzhen_big_exp < 0)
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(data_element, "xianzhen_force_big_num", m_other_cfg.xianzhen_force_big_num) || m_other_cfg.xianzhen_force_big_num < 0)
		{
			return -18;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_free_refresh_count", m_other_cfg.skill_free_refresh_count) || m_other_cfg.skill_free_refresh_count < 0)
		{
			return -19;
		}

		if (!PugiGetSubNodeValue(data_element, "get_skill_item", m_other_cfg.get_skill_item) || NULL == ITEMPOOL->GetItem(m_other_cfg.get_skill_item))
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(data_element, "home_get_reward_interval", m_other_cfg.home_get_reward_interval) || m_other_cfg.home_get_reward_interval < 0)
		{
			return -21;
		}

		if (!PugiGetSubNodeValue(data_element, "home_reward_interval", m_other_cfg.home_reward_interval) || m_other_cfg.home_reward_interval < 0)
		{
			return -22;
		}

		if (!PugiGetSubNodeValue(data_element, "home_reward_times_limit", m_other_cfg.home_reward_times_limit) || m_other_cfg.home_reward_times_limit < 0)
		{
			return -23;
		}

		if (!PugiGetSubNodeValue(data_element, "home_reward_hunli_level", m_other_cfg.home_reward_hunli_level) || m_other_cfg.home_reward_hunli_level < 0)
		{
			return -24;
		}

		if (!PugiGetSubNodeValue(data_element, "home_rob_times_limit", m_other_cfg.home_rob_times_limit) || m_other_cfg.home_rob_times_limit < 0)
		{
			return -25;
		}

		if (!PugiGetSubNodeValue(data_element, "home_rob_repeat_limit", m_other_cfg.home_rob_repeat_limit) || m_other_cfg.home_rob_repeat_limit < 0)
		{
			return -26;
		}

		if (!PugiGetSubNodeValue(data_element, "home_rob_lingjing_per", m_other_cfg.home_rob_lingjing_per) || m_other_cfg.home_rob_lingjing_per < 0)
		{
			return -27;
		}

		if (!PugiGetSubNodeValue(data_element, "home_rob_lingjing_max", m_other_cfg.home_rob_lingjing_max) || m_other_cfg.home_rob_lingjing_max < 0)
		{
			return -28;
		}

		if (!PugiGetSubNodeValue(data_element, "home_rob_hunli_per", m_other_cfg.home_rob_hunli_per) || m_other_cfg.home_rob_hunli_per < 0)
		{
			return -29;
		}

		if (!PugiGetSubNodeValue(data_element, "home_rob_hunli_max", m_other_cfg.home_rob_hunli_max) || m_other_cfg.home_rob_hunli_max < 0)
		{
			return -30;
		}

		if (!PugiGetSubNodeValue(data_element, "home_rob_item_min", m_other_cfg.home_rob_item_min) || m_other_cfg.home_rob_item_min < 0)
		{
			return -31;
		}

		if (!PugiGetSubNodeValue(data_element, "home_rob_item_max", m_other_cfg.home_rob_item_max) || m_other_cfg.home_rob_item_max < 0)
		{
			return -32;
		}

		if (!PugiGetSubNodeValue(data_element, "home_quick_gold", m_other_cfg.home_quick_gold) || m_other_cfg.home_quick_gold < 0)
		{
			return -33;
		}

		if (!PugiGetSubNodeValue(data_element, "home_quick_time", m_other_cfg.home_quick_time) || m_other_cfg.home_quick_time < 0)
		{
			return -34;
		}

		if (!PugiGetSubNodeValue(data_element, "explore_times", m_other_cfg.explore_times) || m_other_cfg.explore_times < 0)
		{
			return -35;
		}

		if (!PugiGetSubNodeValue(data_element, "explore_hp_percent", m_other_cfg.explore_hp_percent) || m_other_cfg.explore_hp_percent < 0)
		{
			return -36;
		}

		if (!PugiGetSubNodeValue(data_element, "explore_other_buy", m_other_cfg.explore_other_buy) || m_other_cfg.explore_other_buy < 0)
		{
			return -37;
		}

		if (!PugiGetSubNodeValue(data_element, "explore_other_buy_gold", m_other_cfg.explore_other_buy_gold) || m_other_cfg.explore_other_buy_gold < 0)
		{
			return -38;
		}

		if (!PugiGetSubNodeValue(data_element, "explore_buff_add_per", m_other_cfg.explore_buff_add_per) || m_other_cfg.explore_buff_add_per < 0)
		{
			return -39;
		}

		if (!PugiGetSubNodeValue(data_element, "explore_buff_buy_gold", m_other_cfg.explore_buff_buy_gold) || m_other_cfg.explore_buff_buy_gold < 0)
		{
			return -40;
		}

		if (!PugiGetSubNodeValue(data_element, "explore_buff_max_count", m_other_cfg.explore_buff_max_count) || m_other_cfg.explore_buff_max_count < 0)
		{
			return -41;
		}

		if (!PugiGetSubNodeValue(data_element, "is_open_special_cap_add", m_is_open_special_cap_add))
		{
			return -42;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_solt_free_count", m_other_cfg.skill_solt_free_count) || m_other_cfg.skill_solt_free_count < 0)
		{
			return -43;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_refresh_consume_id", m_other_cfg.skill_refresh_consume_id) || m_other_cfg.skill_refresh_consume_id < 0 || NULL == ITEMPOOL->GetItem(m_other_cfg.skill_refresh_consume_id))
		{
			return -44;
		}

		if (!PugiGetSubNodeValue(data_element, "refresh_one_consume_count", m_other_cfg.refresh_one_consume_count) || m_other_cfg.refresh_one_consume_count <= 0)
		{
			return -45;
		}

		if (!PugiGetSubNodeValue(data_element, "refresh_ten_consume_count", m_other_cfg.refresh_ten_consume_count) || m_other_cfg.refresh_ten_consume_count <= 0)
		{
			return -46;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitUpLevelConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int index = 0;
	ItemID last_itemid = 0;

	while (!data_element.empty())
	{
		ItemID item_id;
		if (!PugiGetSubNodeValue(data_element, "item_id", item_id) || NULL == ITEMPOOL->GetItem(item_id))
		{
			return -20;
		}

		CHECK_ITEM_IS_JINGLING(item_id);

		if (last_itemid != item_id)
		{
			last_itemid = item_id;
			index = 0;
		}

		if (m_jingling_map.end() == m_jingling_map.find(item_id))
		{
			JingLingLevelItemList item_list_cfg;
			item_list_cfg.item_id = item_id;
			m_jingling_map[item_id] = item_list_cfg;
		}

		std::map<ItemID, JingLingLevelItemList>::iterator iter = m_jingling_map.find(item_id);
		if (index > JINGLING_MAX_LEVEL)
		{
			return -22;
		}

		JingLingItemCfg &item_cfg = iter->second.item_list[index + 1];
		item_cfg.item_id = item_id;

		if (!PugiGetSubNodeValue(data_element, "level", item_cfg.level) || item_cfg.level != index + 1 || item_cfg.level > JINGLING_MAX_LEVEL)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "cost_lingjing", item_cfg.cost_lingjing) || item_cfg.cost_lingjing <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", item_cfg.gongji) || item_cfg.gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", item_cfg.fangyu) || item_cfg.fangyu < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", item_cfg.maxhp) || item_cfg.maxhp < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", item_cfg.mingzhong) || item_cfg.mingzhong < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", item_cfg.shanbi) || item_cfg.shanbi < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", item_cfg.baoji) || item_cfg.baoji < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", item_cfg.jianren) || item_cfg.jianren < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "jingzhun_per", item_cfg.jingzhun_per ) || item_cfg.jingzhun_per < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji_per", item_cfg.baoji_per) || item_cfg.baoji_per < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(data_element, "pofang_per", item_cfg.pofang_per) || item_cfg.pofang_per < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(data_element, "mianshang_per", item_cfg.mianshang_per) || item_cfg.mianshang_per < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", item_cfg.is_broadcast) || item_cfg.is_broadcast < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_num", item_cfg.skill_num) || item_cfg.skill_num < 0)
		{
			return -17;
		}

		++ index;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitTalentConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		ItemID item_id;
		if (!PugiGetSubNodeValue(data_element, "item_id", item_id) || NULL == ITEMPOOL->GetItem(item_id))
		{
			return -20;
		}

		CHECK_ITEM_IS_JINGLING(item_id);

		if (NULL == this->GetJingLingItemCfg(item_id, 1))
		{
			return -200;
		}

		JingLingTalentAttrItemCfg talent_cfg;
		talent_cfg.item_id = item_id;

		if (!PugiGetSubNodeValue(data_element, "maxhp_zizhi", talent_cfg.maxhp_zizhi) || talent_cfg.maxhp_zizhi < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji_zizhi", talent_cfg.gongji_zizhi) || talent_cfg.gongji_zizhi < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu_zizhi", talent_cfg.fangyu_zizhi) || talent_cfg.fangyu_zizhi < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_num", talent_cfg.skill_num) || talent_cfg.skill_num < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "max_num", talent_cfg.max_num) || talent_cfg.max_num < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "home_reward_lingjing", talent_cfg.home_reward_lingjing) || talent_cfg.home_reward_lingjing < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "home_reward_hunli", talent_cfg.home_reward_hunli) || talent_cfg.home_reward_hunli < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "home_other_rate", talent_cfg.home_other_rate) || talent_cfg.home_other_rate < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "home_other_type", talent_cfg.home_other_type)
			|| talent_cfg.home_other_type < 0 || talent_cfg.home_other_type >= JING_LING_HOME_REWARD_TYPE_MAX)
		{
			return -9;
		}

		{
			JINGLINGCONFIG_GETTALENT(JL_TALENT_GONGJI, "type1", "rate1");
		}
		{
			JINGLINGCONFIG_GETTALENT(JL_TALENT_FANGYU, "type2", "rate2");
		}
		{
			JINGLINGCONFIG_GETTALENT(JL_TALENT_MAXHP, "type3", "rate3");
		}
		{
			JINGLINGCONFIG_GETTALENT(JL_TALENT_MINGZHONG, "type4", "rate4");
		}
		{
			JINGLINGCONFIG_GETTALENT(JL_TALENT_SHANGBI, "type5", "rate5");
		}
		{
			JINGLINGCONFIG_GETTALENT(JL_TALENT_BAOJI, "type6", "rate6");
		}
		{
			JINGLINGCONFIG_GETTALENT(JL_TALENT_JIANREN, "type7", "rate7");
		}
		{
			JINGLINGCONFIG_GETTALENT(JL_TALENT_JINGZHUN_PER, "type8", "rate8");
		}
		{
			JINGLINGCONFIG_GETTALENT(JL_TALENT_BAOJI_PER, "type9", "rate9");
		}
		{
			JINGLINGCONFIG_GETTALENT(JL_TALENT_POFANG_PER, "type10", "rate10");
		}
		{
			JINGLINGCONFIG_GETTALENT(JL_TALENT_MIANSHANG_PER, "type11", "rate11");
		}

		std::map<ItemID, JingLingTalentAttrItemCfg>::iterator iter = m_talent_attr_map.find(talent_cfg.item_id);
		if (m_talent_attr_map.end() != iter)
		{
			return -900;
		}
		m_talent_attr_map[talent_cfg.item_id] = talent_cfg;
		
		data_element = data_element.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitGroupConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		JingLingGroupCfg group_cfg;
		char item_num = 0;

		if (!PugiGetSubNodeValue(data_element, "id", group_cfg.id) || group_cfg.id <= 0)
		{
			return -9999;
		}

		{
			if (!PugiGetSubNodeValue(data_element, "itemid1", group_cfg.itemid1))
			{
				return -1;
			}

			if (group_cfg.itemid1 != 0)
			{
				++ item_num;
				CHECK_ITEM_IS_JINGLING(group_cfg.itemid1);

				if (NULL == this->GetJingLingItemCfg(group_cfg.itemid1, 1))
				{
					return -201;
				}
			}
		}
		
		{
			if (!PugiGetSubNodeValue(data_element, "itemid2", group_cfg.itemid2))
			{
				return -2;
			}

			if (group_cfg.itemid2 != 0)
			{
				++ item_num;
				CHECK_ITEM_IS_JINGLING(group_cfg.itemid2);

				if (NULL == this->GetJingLingItemCfg(group_cfg.itemid2, 1))
				{
					return -202;
				}
			}
		}

		{
			if (!PugiGetSubNodeValue(data_element, "itemid3", group_cfg.itemid3))
			{
				return -3;
			}

			if (group_cfg.itemid3 != 0)
			{
				++ item_num;
				CHECK_ITEM_IS_JINGLING(group_cfg.itemid3);

				if (NULL == this->GetJingLingItemCfg(group_cfg.itemid3, 1))
				{
					return -203;
				}
			}
		}
		
		{
			if (!PugiGetSubNodeValue(data_element, "itemid4", group_cfg.itemid4))
			{
				return -4;
			}

			if (group_cfg.itemid4 != 0)
			{
				++ item_num;
				CHECK_ITEM_IS_JINGLING(group_cfg.itemid4);

				if (NULL == this->GetJingLingItemCfg(group_cfg.itemid4, 1))
				{
					return -204;
				}
			}
		}

		{
			if (!PugiGetSubNodeValue(data_element, "itemid5", group_cfg.itemid5))
			{
				return -5;
			}

			if (group_cfg.itemid5 != 0)
			{
				++ item_num;
				CHECK_ITEM_IS_JINGLING(group_cfg.itemid5);

				if (NULL == this->GetJingLingItemCfg(group_cfg.itemid5, 1))
				{
					return -205;
				}
			}
		}

		if (item_num <= 1)
		{
			return -50;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", group_cfg.gongji) || group_cfg.gongji < 0)
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", group_cfg.fangyu) || group_cfg.fangyu < 0)
		{
			return -30;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", group_cfg.maxhp) || group_cfg.maxhp < 0)
		{
			return -21;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji_per", group_cfg.gongji_per) || group_cfg.gongji_per < 0)
		{
			return -22;
		}

		if (!PugiGetSubNodeValue(data_element, "mianshang_per", group_cfg.mianshang_per) || group_cfg.mianshang_per < 0)
		{
			return -23;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp_per", group_cfg.maxhp_per) || group_cfg.maxhp_per < 0)
		{
			return -24;
		}

		if (!PugiGetSubNodeValue(data_element, "jingzhun_per", group_cfg.jingzhun_per) || group_cfg.jingzhun_per < 0)
		{
			return -25;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji_per", group_cfg.baoji_per) || group_cfg.baoji_per < 0)
		{
			return -26;
		}

		if (!PugiGetSubNodeValue(data_element, "skill_id", group_cfg.skill_id))
		{
			return -30;
		}

		if (group_cfg.skill_id != 0 && NULL == SKILLPOOL->GetSkill(group_cfg.skill_id))
		{
			return -31;
		}

		if (group_cfg.skill_id != 0 && item_num < 3)
		{
			return -32;
		}

		std::map<int, JingLingGroupCfg>::iterator iter = m_jingling_group_map.find(group_cfg.id);
		if (m_jingling_group_map.end() != iter)
		{
			return -900;
		}
		m_jingling_group_map[group_cfg.id] = group_cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitGetConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		JingLingGetCfg item_cfg;
		if (!PugiGetSubNodeValue(data_element, "itemid", item_cfg.itemid) || NULL == ITEMPOOL->GetItem(item_cfg.itemid))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "getway", item_cfg.getway) || item_cfg.getway <= JL_GETWAY_MIN || item_cfg.getway >= JL_GETWAY_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "param1", item_cfg.param1) || item_cfg.param1 < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "param2", item_cfg.param2) || item_cfg.param2 < 0)
		{
			return -4;
		}
		
		int count = 0;
		int is_has = 0;

		UNSTD_STATIC_CHECK(ItemNamespace::MAX_XIANPIN_NUM >= 6)

		JINGLINGCONFIG_GET_FIX_TALENT("type1", 1);
		JINGLINGCONFIG_GET_FIX_TALENT("type2", 2);
		JINGLINGCONFIG_GET_FIX_TALENT("type3", 3);
		JINGLINGCONFIG_GET_FIX_TALENT("type4", 4);
		JINGLINGCONFIG_GET_FIX_TALENT("type5", 5);
		JINGLINGCONFIG_GET_FIX_TALENT("type6", 6);
	
		m_jingling_get.push_back(item_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitCardConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_type = 0;
	int last_level = 0;

	while (!data_element.empty())
	{
		JingLingCardCfg item_cfg;
		if (!PugiGetSubNodeValue(data_element, "type", item_cfg.type) || item_cfg.type < 0 || item_cfg.type >= JINGLING_CARD_MAX_TYPE)
		{
			return -1;
		}
		
		if (last_type != item_cfg.type)
		{
			if (1 != item_cfg.type - last_type)
			{
				return -100;
			}

			last_level = 0;
			last_type = item_cfg.type;
		}

		if (!PugiGetSubNodeValue(data_element, "level", item_cfg.level) || item_cfg.level < 0 || item_cfg.level > JINGLING_CARD_MAX_LEVEL || 1 != item_cfg.level - last_level)
		{
			return -2;
		}
		last_level = item_cfg.level;

		if (!PugiGetSubNodeValue(data_element, "stuff_id", item_cfg.stuff_id) || NULL == ITEMPOOL->GetItem(item_cfg.stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "exp", item_cfg.exp) || item_cfg.exp <= 0)
		{
			return -4;
		}

		std::string temp_attr_type_str;
		char str[256] = {0};

		for (int i = 0; i < JINGLING_CARD_ATTR_COUNT; ++ i)
		{
			memset(str, 0, sizeof(str));
			sprintf(str, "attr_type%d", i + 1);

			if (!PugiGetSubNodeValue(data_element, str, temp_attr_type_str))
			{
				return -10;
			}

			if (!CharIntAttrs::GetAttrTypeFromAttrString(temp_attr_type_str, &item_cfg.attr_list[i].attr_type) || !CharIntAttrs::IsRoleBaseAttr(item_cfg.attr_list[i].attr_type))
			{
				return -11;
			}

			memset(str, 0, sizeof(str));
			sprintf(str, "attr_value%d", i + 1);

			if (!PugiGetSubNodeValue(data_element, str, item_cfg.attr_list[i].value) || item_cfg.attr_list[i].value < 0)
			{
				return -12;
			}
		}

		JingLingCardLevelVec &vec = m_jingling_card_map[item_cfg.type];
		vec.push_back(item_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitUpGradeConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_grade = -1;
	int last_bless_val_limit = 0;

	while (!data_element.empty())
	{
		JingLingUpGradeCfg item_cfg;

		if (!PugiGetSubNodeValue(data_element, "grade", item_cfg.grade) || item_cfg.grade < 0 || item_cfg.grade > JINGLING_MAX_GRADE || 1 != item_cfg.grade - last_grade)
		{
			return -1;
		}
		last_grade = item_cfg.grade;

		if (!PugiGetSubNodeValue(data_element, "bless_val_limit", item_cfg.bless_val_limit) || item_cfg.bless_val_limit < 0 || item_cfg.bless_val_limit <= last_bless_val_limit)
		{
			return -2;
		}
		last_bless_val_limit = item_cfg.bless_val_limit;


		if (!PugiGetSubNodeValue(data_element, "stuff_id", item_cfg.stuff_id) || NULL == ITEMPOOL->GetItem(item_cfg.stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_count", item_cfg.stuff_count) || item_cfg.stuff_count <= 0)
		{
			return -33;
		}

		if (!PugiGetSubNodeValue(data_element, "broadcast", item_cfg.broadcast) || item_cfg.broadcast < 0 || item_cfg.broadcast > 1)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", item_cfg.maxhp) || item_cfg.maxhp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", item_cfg.gongji) || item_cfg.gongji < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", item_cfg.fangyu) || item_cfg.fangyu < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", item_cfg.mingzhong) || item_cfg.mingzhong < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", item_cfg.shanbi) || item_cfg.shanbi < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "active_image_id", item_cfg.active_image_id) || item_cfg.active_image_id < 0 || item_cfg.active_image_id >= JINGLING_MAX_IMAGE_ID)
		{
			return -10;
		}

		m_jingling_upgrade_vec.push_back(item_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitUpGradeProbConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_grade = 0;
	int last_high_bless = -1;

	while (!data_element.empty())
	{
		JingLingUpGradeProbCfg item_cfg;

		if (!PugiGetSubNodeValue(data_element, "grade", item_cfg.grade) || item_cfg.grade < 0 || item_cfg.grade >= JINGLING_MAX_GRADE)
		{
			return -1;
		}

		if (item_cfg.grade != last_grade)
		{
			last_grade = item_cfg.grade;
			last_high_bless = -1;
		}

		if (!PugiGetSubNodeValue(data_element, "low_bless", item_cfg.low_bless) || item_cfg.low_bless < 0)
		{
			return -3;
		}

		if (item_cfg.low_bless != last_high_bless + 1) 
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(data_element, "high_bless", item_cfg.high_bless) || item_cfg.high_bless < item_cfg.low_bless)
		{
			return -4;
		}

		last_high_bless = item_cfg.high_bless;

		if (!PugiGetSubNodeValue(data_element, "succ_rate", item_cfg.succ_rate) || item_cfg.succ_rate < 0 || item_cfg.succ_rate > 10000)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "fail_add_1_rate", item_cfg.fail_add_1_rate) || item_cfg.fail_add_1_rate < 0 || item_cfg.fail_add_1_rate > 100)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "fail_add_2_rate", item_cfg.fail_add_2_rate) || item_cfg.fail_add_2_rate < 0 || item_cfg.fail_add_2_rate > 100)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "fail_add_3_rate", item_cfg.fail_add_3_rate) || item_cfg.fail_add_3_rate < 0 || item_cfg.fail_add_3_rate > 100)
		{
			return -8;
		}

		if (item_cfg.fail_add_1_rate + item_cfg.fail_add_2_rate + item_cfg.fail_add_3_rate != 100)
		{
			return -9;
		}

		item_cfg.fail_add_2_rate += item_cfg.fail_add_1_rate;
		item_cfg.fail_add_3_rate += item_cfg.fail_add_2_rate;

		m_jingling_upgradeprob_vec.push_back(item_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitEquipConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_level = 0;
	int last_index = 0;

	while (!data_element.empty())
	{
		JingLingEquipStrengthCfg item_cfg;

		if (!PugiGetSubNodeValue(data_element, "index", item_cfg.index) || item_cfg.index < 0 || item_cfg.index >= JINGLING_EQUIP_MAX_PART)
		{
			return -1;
		}

		if (last_index != item_cfg.index)
		{
			last_index = item_cfg.index;
			last_level = 0;
		}
		
		if (!PugiGetSubNodeValue(data_element, "strength_level", item_cfg.strength_level) || 1 != item_cfg.strength_level - last_level)
		{
			return -2;
		}
		last_level = item_cfg.strength_level;

		if (!PugiGetSubNodeValue(data_element, "stuff_id", item_cfg.stuff_id) || NULL == ITEMPOOL->GetItem(item_cfg.stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_count", item_cfg.stuff_count) || item_cfg.stuff_count <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", item_cfg.maxhp) || item_cfg.maxhp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", item_cfg.gongji) || item_cfg.gongji < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", item_cfg.fangyu) || item_cfg.fangyu < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", item_cfg.mingzhong) || item_cfg.mingzhong < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", item_cfg.shanbi) || item_cfg.shanbi < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "broadcast", item_cfg.broadcast) || item_cfg.broadcast < 0 || item_cfg.broadcast > 1)
		{
			return -10;
		}

		JingLingEquipStrengthVec &vec = m_jingling_equip_strength_map[item_cfg.index];
		vec.push_back(item_cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitPhantomConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int is_broadcast = 0;
	int last_type = -1;
	int last_level = 0;

	while (!data_element.empty())
	{
		JingLingPhantomCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "type", cfg.type) || cfg.type < 0 || cfg.type >= JINGLING_PTHANTOM_MAX_TYPE)
		{
			return -1;
		}

		if (last_type != cfg.type)
		{
			if (1 != cfg.type - last_type)
			{
				return -100;	
			}

			last_level = 0;
		}
		last_type = cfg.type;

		if (!PugiGetSubNodeValue(data_element, "stuff_id", cfg.stuff_id) || NULL == ITEMPOOL->GetItem(cfg.stuff_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_num", cfg.stuff_num) || cfg.stuff_num <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "level", cfg.level) || cfg.level < 0 || cfg.level > JINGLING_PTHANTOM_MAX_LEVEL)
		{
			return -4;
		}

		if (last_level != cfg.level && 1 != cfg.level -last_level)
		{
			return -200;
		}
		last_level = cfg.level;

		if (!PugiGetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast) || is_broadcast < 0 || is_broadcast > 1)
		{
			return -10;
		}
		
		cfg.is_broadcast = (1 == is_broadcast);
	
		JingLingPhantomVec &vec = m_jingling_phantom_map[cfg.type];
		vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitSoulConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int is_broadcast = 0;
	int down_level = 0;
	int last_level = 0;
	int last_type = 0;

	while (!data_element.empty())
	{
		JingLingSoulCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "type", cfg.type) || cfg.type < 0 || cfg.type >= JINGLING_CARD_MAX_TYPE)
		{
			return -1;
		}

		if (last_type != cfg.type)
		{
			if (last_type != cfg.type - 1)
			{
				return -200;
			}

			last_type = cfg.type;
			last_level = 0;
		}

		if (!PugiGetSubNodeValue(data_element, "level", cfg.level) || cfg.level <= 0 || cfg.level > JINGLING_SOUL_MAX_LEVEL || cfg.level != last_level + 1)
		{
			return -2;
		}
		last_level = cfg.level;
		
		if (!PugiGetSubNodeValue(data_element, "stuff1_id", cfg.stuff1_id) || NULL == ITEMPOOL->GetItem(cfg.stuff1_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff1_num", cfg.stuff1_num) || cfg.stuff1_num <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff2_id", cfg.stuff2_id) || NULL == ITEMPOOL->GetItem(cfg.stuff2_id))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff2_num", cfg.stuff2_num) || cfg.stuff2_num < 0)
		{
			return -6;
		}
		
		if (!PugiGetSubNodeValue(data_element, "down_level", down_level) || down_level < 0 || down_level >1)
		{
			return -7;
		}

		down_level == 1 ? cfg.fail_down_level= true : cfg.fail_down_level = false;

		if (!PugiGetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "succ_rate", cfg.succ_rate) || cfg.succ_rate <= 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast) || is_broadcast < 0 || is_broadcast > 1)
		{
			return -14;
		}

		 cfg.is_broadcast = (1 == is_broadcast);

		JingLingSoulVec &vec = m_jingling_soul_map[cfg.type];
		vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}
		
	return 0;
}

int JingLingConfig::InitSoulTaoZhuangConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int is_broadcast = 0;

	while (!data_element.empty())
	{
		JingLingSoulTaoZhuangCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "suit_id", cfg.suit_id) || cfg.suit_id <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "need_min_level", cfg.need_min_level) || cfg.need_min_level <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "is_broadcast", is_broadcast) || is_broadcast < 0 || is_broadcast > 1)
		{
			return -8;
		}

		cfg.is_broadcast = (1 == is_broadcast);

		m_soul_taozhuang_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}
int JingLingConfig::InitHaloActiveCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100;
	}

	m_halo_max_img_id = 0;

	int last_need_level = 0;

	while (!dataElement.empty())
	{
		if (m_halo_max_img_id >= HALO_MAX_IMAGEID)
		{
			return -1;
		}

		int img_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "halo_id", img_id) || img_id != m_halo_max_img_id + 1)
		{
			return -2;
		}
		m_halo_max_img_id = img_id;

		JingLingHaloActiveCfg &cfg = m_halo_active_cfg_list[img_id];
		cfg.halo_id = img_id;

		if (!PugiGetSubNodeValue(dataElement, "active_need_level", cfg.active_need_level) || cfg.active_need_level <= last_need_level || cfg.active_need_level > m_halo_max_level)
		{
			return -3;
		}
		last_need_level = cfg.active_need_level;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitHaloUplevelCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_halo_max_level = 0;

	while (!dataElement.empty())
	{
		if (m_halo_max_level >= JINGLING_HALO_MAX_LEVEL)
		{
			return -1;
		}

		int level(0);
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level != m_halo_max_level + 1)
		{
			return -2;
		}
		m_halo_max_level = level;

		JingLingHaloUplevelCfg &cfg = m_halo_uplevel_cfg_list[level];
		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "uplevel_need_stuff_count", cfg.uplevel_need_stuff_count) || cfg.uplevel_need_stuff_count <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "big_grade", cfg.big_grade) || cfg.big_grade <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "bless_val_limit", cfg.bless_val_limit) || cfg.bless_val_limit <= 0)
		{
			return -7;
		}

		int is_broadcast(0);
		if (!PugiGetSubNodeValue(dataElement, "broadcast", is_broadcast) || is_broadcast < 0)
		{
			return -8;
		}
		cfg.is_broadcast = (0 != is_broadcast);

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -20;
		}
		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -21;
		}
		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -22;
		}
		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -23;
		}
		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -24;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitWuXingCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{

		int quality = 0;
		if (!PugiGetSubNodeValue(dataElement, "quality", quality) || quality < 0 || quality >= JING_LING_COLOR_COUNT)
		{
			return -100;
		}

		int wuxing_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "wuxing_level", wuxing_level) || wuxing_level < 0 || wuxing_level > JING_LING_WU_XING_LEVEL_MAX)
		{
			return -1;
		}

		JingLingWuXingCfg &cfg = m_diff_wuxing_attr_cfg_list[quality][wuxing_level];
		cfg.wuxing_level = wuxing_level;

		if (!PugiGetSubNodeValue(dataElement, "stuff_id", cfg.stuff_id) || (0 != cfg.stuff_id && NULL == ITEMPOOL->GetItem(cfg.stuff_id)))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_num", cfg.stuff_num) || (0 != cfg.stuff_id && cfg.stuff_num <= 0))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "safe_id", cfg.safe_id) || (0 != cfg.safe_id && NULL == ITEMPOOL->GetItem(cfg.safe_id)))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "safe_num", cfg.safe_num) || (0 != cfg.safe_id && cfg.safe_num <= 0))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "succ_rate", cfg.succ_rate) || cfg.succ_rate < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "is_down", cfg.is_down))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "broadcast_type", cfg.broadcast_type))
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp_zizhi", cfg.maxhp_zizhi) || cfg.maxhp_zizhi < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji_zizhi", cfg.gongji_zizhi) || cfg.gongji_zizhi < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu_zizhi", cfg.fangyu_zizhi) || cfg.fangyu_zizhi < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "skill_num", cfg.skill_num) || cfg.skill_num < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement, "recycle_stuff_num", cfg.recycle_stuff_num) || cfg.recycle_stuff_num < 0)
		{
			return -16;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitXianZhenCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int xianzhen_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "xianzhen_level", xianzhen_level) || xianzhen_level < 0 || xianzhen_level > JING_LING_XIAN_ZHEN_LEVEL_MAX)
		{
			return -1;
		}

		JingLingXianZhenCfg &cfg = m_xianzhen_cfg_list[xianzhen_level];
		cfg.xianzhen_level = xianzhen_level;

		if (!PugiGetSubNodeValue(dataElement, "need_exp", cfg.need_exp) || cfg.need_exp < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_num", cfg.stuff_num) || cfg.stuff_num <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "convert_rate", cfg.convert_rate) || cfg.convert_rate < 0)
		{
			return -6;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitXianZhenHunYuCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int hunyu_type = 0;
		if (!PugiGetSubNodeValue(dataElement, "hunyu_type", hunyu_type) || hunyu_type < 0 || hunyu_type > XIAN_ZHEN_HUN_YU_TYPE_MAX)
		{
			return -1;
		}

		int hunyu_level = 0;
		if (!PugiGetSubNodeValue(dataElement, "hunyu_level", hunyu_level) || hunyu_level < 0 || hunyu_level > JING_LING_XIAN_ZHEN_HUN_YU_LEVEL_MAX)
		{
			return -1;
		}

		JingLingXianZhenHunYuCfg &cfg = m_xianzhen_hunyu_cfg_list[hunyu_type][hunyu_level];
		cfg.hunyu_type = hunyu_type;
		cfg.hunyu_level = hunyu_level;

		if (!PugiGetSubNodeValue(dataElement, "stuff_id", cfg.stuff_id) || NULL == ITEMPOOL->GetItem(cfg.stuff_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_num", cfg.stuff_num) || cfg.stuff_num < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "convert_rate", cfg.convert_rate) || cfg.convert_rate < 0)
		{
			return -4;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitSkillCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int skill_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_id", skill_id) || skill_id <= 0 || skill_id > JING_LING_SKILL_ID_MAX)
		{
			return -1;
		}

		JingLingSkillCfg &cfg = m_skill_cfg_list[skill_id];
		cfg.skill_id = skill_id;

		if (!PugiGetSubNodeValue(dataElement, "pre_skill", cfg.pre_skill) || cfg.pre_skill < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "book_id", cfg.book_id) || NULL == ITEMPOOL->GetItem(cfg.book_id))
		{
			return -100;
		}

		if (!PugiGetSubNodeValue(dataElement, "skill_level", cfg.skill_level) || cfg.skill_level < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "skill_type", cfg.skill_type) || cfg.skill_type < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg.maxhp) || cfg.maxhp < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg.gongji) || cfg.gongji < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg.fangyu) || cfg.fangyu < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", cfg.mingzhong) || cfg.mingzhong < 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", cfg.shanbi) || cfg.shanbi < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", cfg.baoji) || cfg.baoji < 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "kangbao", cfg.kangbao) || cfg.kangbao < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp_per", cfg.maxhp_per) || cfg.maxhp_per < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji_per", cfg.gongji_per) || cfg.gongji_per < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu_per", cfg.fangyu_per) || cfg.fangyu_per < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "constant_zengshang", cfg.constant_zengshang) || cfg.constant_zengshang < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement, "constant_mianshang", cfg.constant_mianshang) || cfg.constant_mianshang < 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(dataElement, "capability", cfg.capability) || cfg.capability < 0)
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(dataElement, "remove_stuff_id", cfg.remove_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.remove_stuff_id))
		{
			return -18;
		}

		if (!PugiGetSubNodeValue(dataElement, "move_stuff_id", cfg.move_stuff_id) || NULL == ITEMPOOL->GetItem(cfg.move_stuff_id))
		{
			return -19;
		}

		if (!PugiGetSubNodeValue(dataElement, "cooldown", cfg.cooldown) || cfg.cooldown < 0)
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_1", cfg.param_1) || cfg.param_1 < 0)
		{
			return -21;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_2", cfg.param_2) || cfg.param_2 < 0)
		{
			return -22;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_3", cfg.param_3) || cfg.param_3 < 0)
		{
			return -23;
		}

		if (!PugiGetSubNodeValue(dataElement, "param_4", cfg.param_4) || cfg.param_4 < 0)
		{
			return -24;
		}

		if (!PugiGetSubNodeValue(dataElement, "rand_weight", cfg.rand_weight) || cfg.rand_weight < 0)
		{
			return -25;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitSkillRefreshCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int stage = 0;
		if (!PugiGetSubNodeValue(dataElement, "stage", stage) || stage <= 0 || stage >= JING_LING_SKILL_REFRESH_STATE_MAX)
		{
			return -1;
		}

		JingLingSkillRefreshCfg &cfg = m_skill_refresh_list[stage];
		cfg.stage = stage;

		if (!PugiGetSubNodeValue(dataElement, "min_count", cfg.min_count) || cfg.min_count < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_count", cfg.max_count) || cfg.max_count < cfg.min_count)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "once_gold", cfg.once_gold) || cfg.once_gold < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "ten_gold", cfg.ten_gold) || cfg.ten_gold < 0)
		{
			return -5;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitSkillRefreshRateCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int stage = 0;
		if (!PugiGetSubNodeValue(dataElement, "stage", stage) || stage <= 0 || stage >= JING_LING_SKILL_REFRESH_STATE_MAX)
		{
			return -1;
		}

		JingLingSkillRefreshCfg &cfg = m_skill_refresh_list[stage];

		if (cfg.cfg_count >= JING_LING_SKILL_REFRESH_RATE_MAX)
		{
			return -2;
		}

		JingLingSkillRefreshRateCfg &rate_cfg = cfg.cfg_list[cfg.cfg_count++];

		if (!PugiGetSubNodeValue(dataElement, "skill_id", rate_cfg.skill_id) || rate_cfg.skill_id < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "weight", rate_cfg.weight) || rate_cfg.weight < 0)
		{
			return -4;
		}

		cfg.total_weight += rate_cfg.weight;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitHomeOtherRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int other_type = 0;
		if (!PugiGetSubNodeValue(dataElement, "other_type", other_type) || other_type < 0 || other_type >= JING_LING_HOME_REWARD_TYPE_MAX)
		{
			return -1;
		}

		JingLingHomeOtherRewardCfg &cfg = m_home_reward_cfg_list[other_type];

		if (cfg.reward_count < 0 && cfg.reward_count >= JING_LING_HOME_REWARD_ITEM_CFG_MAX)
		{
			return -2;
		}

		int weight = 0;
		if (!PugiGetSubNodeValue(dataElement, "weight", weight) || weight <= 0)
		{
			return -3;
		}
		cfg.weight_list[cfg.reward_count] = weight;

		PugiXmlNode element = dataElement.child("reward_item");
		if (element.empty() || !cfg.reward_item_list[cfg.reward_count].ReadConfig(element))
		{
			return -4;
		}

		cfg.total_weight += weight;
		cfg.reward_count++;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitExploreCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int mode = 0;
		if (!PugiGetSubNodeValue(dataElement, "mode", mode) || mode < 0 || mode >= JING_LING_EXPLORE_MODE_COUNT)
		{
			return -1;
		}

		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level < 0 || level >= JING_LING_EXPLORE_LEVEL_COUNT)
		{
			return -2;
		}

		JingLingExploreCfg &cfg = m_explore_cfg_list[mode][level];
		if (0 != cfg.max_capability)
		{
			return -3;
		}

		cfg.mode = mode;
		cfg.level = level;

		if (!PugiGetSubNodeValue(dataElement, "min_capability", cfg.min_capability) || cfg.min_capability <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_capability", cfg.max_capability) || cfg.max_capability < cfg.min_capability)
		{
			return -5;
		}

		{
			if (!PugiGetSubNodeValue(dataElement, "free_times", cfg.free_times) || cfg.free_times < 0)
			{
				return -6;
			}

			char fetch_gold_name[32] = { 0 };
			for (int i = 0; i < JING_LING_EXPLORE_GOLE_FETCH_COUNT; i++)
			{
				SNPRINTF(fetch_gold_name, sizeof(fetch_gold_name), "fetch_gold_%d", i + 1);

				if (!PugiGetSubNodeValue(dataElement, fetch_gold_name, cfg.fetch_gold_list[i]) || cfg.fetch_gold_list[i] <= 0)
				{
					return -200 - i;
				}
			}
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitExploreRewardCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int mode = 0;
		if (!PugiGetSubNodeValue(dataElement, "mode", mode) || mode < 0 || mode >= JING_LING_EXPLORE_MODE_COUNT)
		{
			return -1;
		}

		int level = 0;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level < 0 || level >= JING_LING_EXPLORE_LEVEL_COUNT)
		{
			return -2;
		}

		JingLingExploreCfg &cfg = m_explore_cfg_list[mode][level];
		if (cfg.reward_count >= JING_LING_EXPLORE_REWARD_COUNT_MAX)
		{
			return -3;
		}

		JingLingExploreCfg::RewardCfg &reward_cfg = cfg.reward_cfg_list[cfg.reward_count];

		if (!PugiGetSubNodeValue(dataElement, "role_level", reward_cfg.role_level) || reward_cfg.role_level <= 0)
		{
			return -4;
		}

		reward_cfg.item_count = ItemConfigData::ReadConfigList(dataElement, "reward_item", reward_cfg.reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
		if (reward_cfg.item_count < 0)
		{
			return -100 + reward_cfg.item_count;
		}

		cfg.reward_count++;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitExploreNameCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_other_cfg.explore_name_count = 0;
	while (!dataElement.empty())
	{
		m_other_cfg.explore_name_count++;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int JingLingConfig::InitHuanHuaSpecialCapAddCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		JingLingHuanHuaSpecialCapAddCfgItem cfg_item;
		if (!PugiGetSubNodeValue(dataElement, "huanhua_id", cfg_item.huanhua_id) || cfg_item.huanhua_id < 0 || cfg_item.huanhua_id >= JINGLING_PTHANTOM_MAX_TYPE)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "huanhua_level", cfg_item.huanhua_level) || cfg_item.huanhua_level <= 0 || cfg_item.huanhua_level > JINGLING_MAX_GRADE)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", cfg_item.maxhp) || cfg_item.maxhp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", cfg_item.gongji) || cfg_item.gongji < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", cfg_item.fangyu) || cfg_item.fangyu < 0)
		{
			return -5;
		}

		// 判断重复
		if (m_huanhua_special_cap_cfg.find(cfg_item.huanhua_id) != m_huanhua_special_cap_cfg.end())
		{
			return -6;
		}
		m_huanhua_special_cap_cfg[cfg_item.huanhua_id] = cfg_item;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}


const JingLingTalentAttrItemCfg * JingLingConfig::GetTalentAttrCfg(ItemID itemid)
{
	std::map<ItemID, JingLingTalentAttrItemCfg>::const_iterator iter = m_talent_attr_map.find(itemid);
	
	if (m_talent_attr_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

const JingLingItemCfg * JingLingConfig::GetJingLingItemCfg(ItemID itemid, int level)
{
	std::map<ItemID, JingLingLevelItemList>::iterator iter = m_jingling_map.find(itemid);

	if (m_jingling_map.end() == iter)
	{
		return NULL;
	}
	
	if (level <= 0 || level > JINGLING_MAX_LEVEL)
	{
		return NULL;
	}
	
	JingLingItemCfg &level_cfg = iter->second.item_list[level];

	if (level_cfg.level != level)
	{
		return NULL;
	}

	return &iter->second.item_list[level];
}

const JingLingGroupCfg *JingLingConfig::GetJingLingGroupCfg(int group_id)
{
	std::map<int, JingLingGroupCfg>::const_iterator iter = m_jingling_group_map.find(group_id);
	if (m_jingling_group_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

void JingLingConfig::GetJingLingGroupCfg(ItemID id1, ItemID id2, ItemID id3, ItemID id4, ItemID id5, int groupid_list[JINGLING_MAX_GROUP_IN_ONECE], int *group_num)
{
	int num = 0;
	std::map<int, JingLingGroupCfg>::iterator iter;
	for (iter = m_jingling_group_map.begin(); iter != m_jingling_group_map.end(); ++ iter)
	{
		JingLingGroupCfg *group_cfg = &iter->second;
		if (group_cfg->IsAllMatch(id1, id2, id3, id4, id5) && num < JINGLING_MAX_GROUP_IN_ONECE)
		{
			groupid_list[num] = group_cfg->id;
			++ num;
		}
	}

	if (NULL != group_num)
	{
		*group_num = num;
	}
}

const JingLingCardCfg *JingLingConfig::GetJingLingCardCfg(int card_type, int level)
{
	JingLingCardMap::const_iterator iter = m_jingling_card_map.find(card_type);
	if (m_jingling_card_map.end() == iter)
	{
		return NULL;
	}

	int index = level - 1;

	const JingLingCardLevelVec &vec = iter->second;
	if (index < 0 || index >= static_cast<int>(vec.size()))
	{
		return NULL;
	}

	return &vec.at(index);
}

const JingLingUpGradeCfg *JingLingConfig::GetJingLingUpGradeCfg(int level)
{
	if (level < 0 || level >= static_cast<int>(m_jingling_upgrade_vec.size()))
	{
		return NULL;
	}

	return &m_jingling_upgrade_vec.at(level);
}

const JingLingUpGradeProbCfg *JingLingConfig::GetJingLingUpGradeProbCfg(int old_grade, int old_bless_val)
{
	for (JingLingUpGradeProbVec::const_iterator iter = m_jingling_upgradeprob_vec.begin(); m_jingling_upgradeprob_vec.end() != iter; ++ iter)
	{
		if (iter->grade == old_grade && old_bless_val >= iter->low_bless && old_bless_val <= iter->high_bless)
		{
			return &(*iter);
		}
	}

	return NULL;
}

const JingLingEquipStrengthCfg *JingLingConfig::GetJingLingEquipStrengthCfg(int part_index, int level)
{
	JingLingEquipStrengthMap::const_iterator map_iter = m_jingling_equip_strength_map.find(part_index);
	if (m_jingling_equip_strength_map.end() == map_iter)
	{
		return NULL;
	}

	const JingLingEquipStrengthVec &vec = map_iter->second;
	if (level <= 0 || level > static_cast<int>(vec.size()))
	{
		return NULL;
	}

	return &vec.at(level - 1);
}

const JingLingPhantomCfg *JingLingConfig::GetJingLingPhantomCfg(int type, int level)
{
	if (type < 0 || type >= (JINGLING_PTHANTOM_MAX_TYPE) || level <= 0 || level > JINGLING_PTHANTOM_MAX_LEVEL)
	{
		return NULL;
	}
	
	JingLingPhantomMap::const_iterator map_iter = m_jingling_phantom_map.find(type);
	if (m_jingling_phantom_map.end() == map_iter)
	{
		return NULL;
	}

	const JingLingPhantomVec &vec = map_iter->second;
	if (level <= 0 || level >= static_cast<int>(vec.size()))
	{
		return NULL;
	}

	//配置表有第一条无用数据(客户端需要),所以不需要 -1
	return &vec.at(level);
}

bool JingLingConfig::HasJingLingPhantomCfg(int phantom_img_id)
{
	if (phantom_img_id < 0 || phantom_img_id >= JINGLING_PTHANTOM_MAX_TYPE)
	{
		return false;
	}

	JingLingPhantomMap::const_iterator map_iter = m_jingling_phantom_map.find(phantom_img_id);
	if (m_jingling_phantom_map.end() == map_iter)
	{
		return false;
	}

	return map_iter->second.size() > 0;	
}

const JingLingSoulCfg* JingLingConfig::GetJingLingSoulcfg(int type, int level)
{
	if (level <= 0 || level > JINGLING_SOUL_MAX_LEVEL)
	{
		return NULL;
	}

	JingLingSoulMap::const_iterator iter = m_jingling_soul_map.find(type);
	if (m_jingling_soul_map.end() ==  iter)
	{
		return NULL;
	}

	const JingLingSoulVec &vec = iter->second;
	if (level <= 0 || level > static_cast<int>(vec.size()))
	{
		return NULL;
	}

	return &vec.at(level - 1);
}

const JingLingSoulTaoZhuangCfg* JingLingConfig::GetSoulTaoZhuangCfg(int level)
{
	if (level <= 0 || level > JINGLING_SOUL_MAX_LEVEL || level > static_cast<int>(m_soul_taozhuang_vec.size()))
	{
		return NULL;
	}

	return &m_soul_taozhuang_vec.at(level - 1);
}

const bool JingLingConfig::GetRandomTalent(ItemGridData *item_grid_data)
{
	if (NULL == item_grid_data) return false;
	
	const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(item_grid_data->item_id);
	if (NULL == equip_item || ItemBase::I_TYPE_EQUIPMENT != equip_item->GetItemType())
	{
		return false;
	}

	if (Equipment::E_TYPE_SPECIAL_JINGLING != equip_item->GetEquipType())
	{
		return false;
	}

	const JingLingTalentAttrItemCfg *item_cfg = this->GetTalentAttrCfg(item_grid_data->item_id);

	if (NULL == item_cfg)
	{
		return false;
	}

	EquipmentParam *equip_param = (EquipmentParam *)item_grid_data->param;
	memset(equip_param->xianpin_type_list, 0, sizeof(equip_param->xianpin_type_list));

	short num = 0;
	std::map<int, JingLingTalentAttrItemCfg::TalentAttr>::const_iterator iter;

	for (iter = item_cfg->random_talent_map.begin(); iter != item_cfg->random_talent_map.end(); ++ iter)
	{
		const JingLingTalentAttrItemCfg::TalentAttr *attr_item = &iter->second;

		if (attr_item->talent_type > JL_TALENT_INVALID_TYPE && attr_item->talent_type < JL_TALENT_MAX_TYPE &&
			attr_item->param1 > 0 && RandomNum(10000) <= attr_item->get_rate && num < item_cfg->max_num 
			&& num < ItemNamespace::MAX_XIANPIN_NUM)
		{
			equip_param->xianpin_type_list[num] = attr_item->talent_type;
			++ num;
		}
	}

	return true;
}

const bool JingLingConfig::GetFixTalent(ItemGridData *item_grid_data, int getway, int getway_param1, int getway_param2)
{
	if (NULL == item_grid_data) return false;

	const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(item_grid_data->item_id);
	if (NULL == equip_item || ItemBase::I_TYPE_EQUIPMENT != equip_item->GetItemType())
	{
		return false;
	}

	if (Equipment::E_TYPE_SPECIAL_JINGLING != equip_item->GetEquipType())
	{
		return false;
	}

	EquipmentParam *equip_param = (EquipmentParam *)item_grid_data->param;
	memset(equip_param->xianpin_type_list, 0, sizeof(equip_param->xianpin_type_list));

	int size = static_cast<int>(m_jingling_get.size());
	for (int i = 0; i < size; ++ i)
	{
		const JingLingGetCfg &get_cfg = m_jingling_get[i];

		if (get_cfg.itemid == item_grid_data->item_id && get_cfg.getway == getway && 
			get_cfg.param1 == getway_param1 && get_cfg.param2 == getway_param2 && 
			sizeof(equip_param->xianpin_type_list) == sizeof(get_cfg.talent_list))
		{
			memcpy(equip_param->xianpin_type_list, get_cfg.talent_list, sizeof(equip_param->xianpin_type_list));

			break;
		}
	}

	return true;
}

const bool JingLingConfig::SetJingLingInitLevel(ItemGridData *item_grid_data)
{
	if (NULL == item_grid_data) return false;

	const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(item_grid_data->item_id);
	if (NULL == equip_item || ItemBase::I_TYPE_EQUIPMENT != equip_item->GetItemType())
	{
		return false;
	}

	if (Equipment::E_TYPE_SPECIAL_JINGLING != equip_item->GetEquipType())
	{
		return false;
	}
	EquipmentParam *equip_param = (EquipmentParam *)item_grid_data->param;
	equip_param->strengthen_level = 1;

	return true;
}

int JingLingConfig::GetTalentValue(ItemID item_id, int talent_type)
{
	if (item_id <= 0 || talent_type <= 0)
	{
		return 0;
	}

	const JingLingTalentAttrItemCfg *item_cfg = this->GetTalentAttrCfg(item_id);
	if (NULL == item_cfg)
	{
		return 0;
	}

	std::map<int, JingLingTalentAttrItemCfg::TalentAttr>::const_iterator iter = item_cfg->random_talent_map.find(talent_type);
	if(item_cfg->random_talent_map.end() == iter)
	{
		return 0;
	}

	return iter->second.param1;
}

int JingLingConfig::GetRecycleLingjing(ItemID item_id, short level)
{
	int recycle_lingjing = 0;
	for (int i = 1; i < level; ++ i)
	{
		const JingLingItemCfg *cfg = this->GetJingLingItemCfg(item_id, i);
		if (cfg != NULL)
		{
			recycle_lingjing += cfg->cost_lingjing;
		}
	}

	return recycle_lingjing;
}



int JingLingConfig::GetHaloActiveIdWithLevel(int halo_level)
{
	if (halo_level <= 0 || halo_level > m_halo_max_level)
	{
		return -1;
	}

	for (int index = 0; index <= m_halo_max_img_id; ++ index)
	{
		if (halo_level == m_halo_active_cfg_list[index].active_need_level)
		{
			return m_halo_active_cfg_list[index].halo_id;
		}
	}

	return -1;
}

const JingLingHaloUplevelCfg * JingLingConfig::GetHaloUplevelCfg(int halo_level)
{
	if (halo_level <= 0 || halo_level > m_halo_max_level)
	{
		return NULL;
	}

	return &m_halo_uplevel_cfg_list[halo_level];
}

const JingLingWuXingCfg *JingLingConfig::GetJingLingWuXingCfg(int color, int wuxing_level)
{
	if (wuxing_level < 0 || wuxing_level > JING_LING_WU_XING_LEVEL_MAX || color < 0 || color >= JING_LING_COLOR_COUNT
		|| wuxing_level != m_diff_wuxing_attr_cfg_list[color][wuxing_level].wuxing_level)
	{
		return NULL;
	}

	return &m_diff_wuxing_attr_cfg_list[color][wuxing_level];
}

const JingLingXianZhenCfg *JingLingConfig::GetJingLingXianZhenCfg(int xianzhen_level)
{
	if (xianzhen_level < 0 || xianzhen_level > JING_LING_XIAN_ZHEN_LEVEL_MAX
		|| xianzhen_level != m_xianzhen_cfg_list[xianzhen_level].xianzhen_level)
	{
		return NULL;
	}

	return &m_xianzhen_cfg_list[xianzhen_level];
}

const JingLingXianZhenHunYuCfg *JingLingConfig::GetJingLingXianZhenHunYuCfg(int hunyu_type, int hunyu_level)
{
	if (hunyu_type < 0 || hunyu_type >= XIAN_ZHEN_HUN_YU_TYPE_MAX
		|| hunyu_level < 0 || hunyu_level > JING_LING_XIAN_ZHEN_HUN_YU_LEVEL_MAX
		|| hunyu_level != m_xianzhen_hunyu_cfg_list[hunyu_type][hunyu_level].hunyu_level)
	{
		return NULL;
	}

	return &m_xianzhen_hunyu_cfg_list[hunyu_type][hunyu_level];
}

const JingLingSkillCfg * JingLingConfig::GetSkillCfg(int skill_id)
{
	if (skill_id <= 0 || skill_id > JING_LING_SKILL_ID_MAX)
	{
		return NULL;
	}

	return &m_skill_cfg_list[skill_id];
}

const JingLingSkillCfg * JingLingConfig::GetSkillCfgByItem(ItemID item_id)
{
	for (int i = 0; i <= JING_LING_SKILL_ID_MAX; i++)
	{
		if (i != m_skill_cfg_list[i].skill_id) break;

		if (m_skill_cfg_list[i].book_id == item_id)
		{
			return &m_skill_cfg_list[i];
		}
	}

	return NULL;
}

const JingLingSkillRefreshCfg * JingLingConfig::GetJingLingSkillRefreshCfg(int refresh_count)
{
	for (int i = 1; i < JING_LING_SKILL_REFRESH_STATE_MAX; i++)
	{
		if (m_skill_refresh_list[i].min_count <= refresh_count && refresh_count <= m_skill_refresh_list[i].max_count)
		{
			return &m_skill_refresh_list[i];
		}
	}

	return NULL;
}

const ItemConfigData *JingLingConfig::RandHomeOtherRewardItem(int other_type)
{
	if (other_type < 0 || other_type >= JING_LING_HOME_REWARD_TYPE_MAX
		|| m_home_reward_cfg_list[other_type].total_weight <= 0)
	{
		return NULL;
	}

	JingLingHomeOtherRewardCfg &cfg = m_home_reward_cfg_list[other_type];
	int rand_num = RandomNum(cfg.total_weight);
	for (int i = 0; i < cfg.reward_count && i < JING_LING_HOME_REWARD_ITEM_CFG_MAX; i++)
	{
		if (rand_num < cfg.weight_list[i])
		{
			return &cfg.reward_item_list[i];
		}
		rand_num -= cfg.weight_list[i];
	}

	return NULL;
}

const JingLingExploreCfg *JingLingConfig::GetJingLingExploreCfg(int mode, int level)
{
	if (mode < 0 || mode >= JING_LING_EXPLORE_MODE_COUNT
		|| level < 0 || level >= JING_LING_EXPLORE_LEVEL_COUNT)
	{
		return NULL;
	}

	return &m_explore_cfg_list[mode][level];
}

const JingLingHuanHuaSpecialCapAddCfgItem* JingLingConfig::GetHuanHuaSpecialCapAddCfg(int huanhua_id)const
{
	//if (huanhua_id <= 0 || huanhua_id > JINGLING_MAX_IMAGE_ID)
	//{
	//	return nullptr;
	//}

	auto iter = m_huanhua_special_cap_cfg.find(huanhua_id);
	if (iter != m_huanhua_special_cap_cfg.end())
	{
		return &(iter->second);
	}

	return nullptr;
}

bool JingLingConfig::IsAddSpecialCap()
{
	return m_is_open_special_cap_add;
}

ItemID JingLingConfig::RandJingLingId()
{
	if (m_talent_attr_map.empty())
	{
		return 0;
	}

	std::map<ItemID, JingLingTalentAttrItemCfg>::iterator it = m_talent_attr_map.begin();
	std::advance(it, RandomNum(m_talent_attr_map.size()));

	return it->first;
}

int JingLingConfig::InitSkillSlotOpenConsumeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		int slot_index = -1;
		if (!PugiGetSubNodeValue(data_element, "slot_index", slot_index) || slot_index < 0 || slot_index >= JING_LING_SKILL_COUNT_MAX)
		{
			return -1;
		}
		SkillSlotOpenConsume &cfg = m_skill_slot_open_consume_list[slot_index];
		cfg.slot_index = slot_index;

		if (!PugiGetSubNodeValue(data_element, "gold", cfg.gold) || cfg.gold < 0)
		{
			return -2;
		}

		if (slot_index >= m_other_cfg.skill_solt_free_count && cfg.gold <= 0) // 不是默认开启的槽位不能配0
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const int JingLingConfig::GetSkillSlotOpenConsume(int slot_index)
{
	if (slot_index < 0 || slot_index >= JING_LING_SKILL_COUNT_MAX)
	{
		return 0;
	}

	return m_skill_slot_open_consume_list[slot_index].gold;
}