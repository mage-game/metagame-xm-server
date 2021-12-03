#include "dailyfbconfig.hpp"

#include "servercommon/configcommon.h"
#include "monster/monsterpool.h"
#include "servercommon/servercommon.h"
#include "task/taskpool.h"
#include "checkresourcecenter.hpp"
#include "item/itempool.h"
#include <servercommon/fbdef.hpp>

DailyFBConfig::DailyFBConfig() : m_max_dailyfb_type(0), m_coin_max_wave(0)
{
}

DailyFBConfig::~DailyFBConfig()
{

}

bool DailyFBConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "DailyFBConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	// 日常副本
	{
		PugiXmlNode root_element = RootElement.child("dailyfb");
		if (root_element.empty())
		{
			*err = path + " xml not exp_wave node ";
			return false;
		}

		iRet = this->InitDailyFbCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitDailyFbCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	// 经验本波数配置
	{
		PugiXmlNode root_element = RootElement.child("expfb_wave");
		if (root_element.empty())
		{
			*err = path + " xml not expfb_wave node ";
			return false;
		}

		iRet = this->InitExpWaveCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitExpWaveCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	// vip对应可以购买的次数
	{
		PugiXmlNode root_element = RootElement.child("expfb_vip_pay_time");
		if (root_element.empty())
		{
			*err = path + " xml not expfb_reset node ";
			return false;
		}

		iRet = this->InitExpFbVipPayCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitExpFbVipPayCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	// 经验本重置配置
	{
		PugiXmlNode root_element = RootElement.child("expfb_reset");
		if (root_element.empty())
		{
			*err = path + " xml not expfb_reset node ";
			return false;
		}

		iRet = this->InitExpResetCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitExpResetCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	// 经验本小怪物坐标
	{
		PugiXmlNode root_element = RootElement.child("exp_monster_pos");
		if (root_element.empty())
		{
			*err = path + " xml not exp_monster_pos node ";
			return false;
		}

		iRet = this->InitExpMonsterPosCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitExpMonsterPosCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	// 经验本其他配置
	{
		PugiXmlNode root_element = RootElement.child("exp_other_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not exp_other_cfg node ";
			return false;
		}

		iRet = this->InitExpOtherCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitExpOtherCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int DailyFBConfig::InitDailyFbCfg(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -100000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -200000;
	}

	while (!data_element.empty())
	{
		int dailyfb_type = 0;

		if (!PugiGetSubNodeValue(data_element, "dailyfb_type", dailyfb_type) || dailyfb_type < 0 || dailyfb_type >= MAX_DAILY_FB_NUM)
		{
			return -1;
		}

		DailyFbItem &fb_item = m_dailyfb_map[dailyfb_type];
		fb_item.dailyfb_type = dailyfb_type;

		m_max_dailyfb_type = m_max_dailyfb_type < dailyfb_type ? dailyfb_type : m_max_dailyfb_type;

		if (!PugiGetSubNodeValue(data_element, "role_level", fb_item.role_level) || fb_item.role_level <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "complete_task_id", fb_item.complete_task_id) || NULL == TASKPOOL->GetCfgByTaskID(fb_item.complete_task_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "scene_id", fb_item.scene_id) || fb_item.scene_id <= 0)
		{
			return -6;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(fb_item.scene_id);

		if (!PugiGetSubNodeValue(data_element, "enter_pos_x", fb_item.birth_pos.x) || fb_item.birth_pos.x <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "enter_pos_y", fb_item.birth_pos.y) || fb_item.birth_pos.y <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "next_wave_refresh_interval", fb_item.next_wave_refresh_interval) || fb_item.next_wave_refresh_interval < 0)
		{
			return -9;
		}

		data_element = data_element.next_sibling();
	}
	return 0;
}

int DailyFBConfig::InitExpOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "time_limit", m_expfb_other_cfg.time_limit) || m_expfb_other_cfg.time_limit <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "interval_time", m_expfb_other_cfg.interval_time) || m_expfb_other_cfg.interval_time < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "day_times", m_expfb_other_cfg.day_times) || m_expfb_other_cfg.day_times <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "buff_add_gongji_per", m_expfb_other_cfg.buff_add_gongji_per) || m_expfb_other_cfg.buff_add_gongji_per <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "max_buff_time", m_expfb_other_cfg.max_buff_time) || m_expfb_other_cfg.max_buff_time <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(data_element, "buff_cost", m_expfb_other_cfg.buff_cost) || m_expfb_other_cfg.buff_cost <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(data_element, "start_refresh_monster_count", m_expfb_other_cfg.start_refresh_monster_count) || m_expfb_other_cfg.start_refresh_monster_count <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "monster_count", m_expfb_other_cfg.monster_count) || m_expfb_other_cfg.monster_count <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "extra_exp_per", m_expfb_other_cfg.extra_exp_per) || m_expfb_other_cfg.extra_exp_per <= 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(data_element, "one_bo_time", m_expfb_other_cfg.one_bo_time) || m_expfb_other_cfg.one_bo_time <= 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(data_element, "two_bo_time", m_expfb_other_cfg.two_bo_time) || m_expfb_other_cfg.two_bo_time <= 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "first_time_limit", m_expfb_other_cfg.first_time_limit) || m_expfb_other_cfg.first_time_limit <= 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(data_element, "baodi_exp1", m_expfb_other_cfg.baodi_exp1) || m_expfb_other_cfg.baodi_exp1 < 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(data_element, "baodi_exp2", m_expfb_other_cfg.baodi_exp2) || m_expfb_other_cfg.baodi_exp2 < 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(data_element, "baodi_exp3", m_expfb_other_cfg.baodi_exp3) || m_expfb_other_cfg.baodi_exp3 < 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(data_element, "refresh_cd1", m_expfb_other_cfg.refresh_cd1) || m_expfb_other_cfg.refresh_cd1 < 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(data_element, "refresh_cd2", m_expfb_other_cfg.refresh_cd2) || m_expfb_other_cfg.refresh_cd2 < 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(data_element, "refresh_cd3", m_expfb_other_cfg.refresh_cd3) || m_expfb_other_cfg.refresh_cd3 < 0)
	{
		return -18;
	}

	if (!PugiGetSubNodeValue(data_element, "total_wave", m_expfb_other_cfg.total_wave) || m_expfb_other_cfg.total_wave <= 0)
	{
		return -19;
	}

	if (!PugiGetSubNodeValue(data_element, "fir_wave_refresh_count", m_expfb_other_cfg.fir_wave_refresh_count) || m_expfb_other_cfg.fir_wave_refresh_count <= 0)
	{
		return -20;
	}

	if (!PugiGetSubNodeValue(data_element, "sec_wave_refresh_count", m_expfb_other_cfg.sec_wave_refresh_count) || m_expfb_other_cfg.sec_wave_refresh_count <= 0)
	{
		return -21;
	}

	if (!PugiGetSubNodeValue(data_element, "other_wave_refresh_count", m_expfb_other_cfg.other_wave_refresh_count) || m_expfb_other_cfg.other_wave_refresh_count <= 0)
	{
		return -22;
	}

	if (!PugiGetSubNodeValue(data_element, "exp_factor1", m_expfb_other_cfg.exp_factor1) || m_expfb_other_cfg.exp_factor1 <= 0)
	{
		return -23;
	}

	if (!PugiGetSubNodeValue(data_element, "exp_factor2", m_expfb_other_cfg.exp_factor2) || m_expfb_other_cfg.exp_factor2 <= 0)
	{
		return -24;
	}

	if (!PugiGetSubNodeValue(data_element, "exp_factor3", m_expfb_other_cfg.exp_factor3) || m_expfb_other_cfg.exp_factor3 <= 0)
	{
		return -25;
	}

	PugiXmlNode Itemelement = data_element.child("item_stuff");
	ItemID itemid = 0;
	if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid) )
	{
		if (!m_expfb_other_cfg.item_stuff.ReadConfig(Itemelement))
		{
			return -10;
		}

	}else
	{
		return -11;
	}

	return 0;
}

int DailyFBConfig::InitExpWaveCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -200000;
	}

	while (!data_element.empty())
	{
		ExpFbWaveItem cfg;

		if (!PugiGetSubNodeValue(data_element, "level", cfg.level) || cfg.level < 0 || cfg.level >= MAX_ROLE_LEVEL_VERSION_LIMIT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_id", cfg.monster_id) || cfg.monster_id < 0 || !MONSTERPOOL->IsMonsterExist(cfg.monster_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_id2", cfg.monster_id2) || cfg.monster_id2 < 0 || !MONSTERPOOL->IsMonsterExist(cfg.monster_id2))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_id3", cfg.monster_id3) || cfg.monster_id3 < 0 || !MONSTERPOOL->IsMonsterExist(cfg.monster_id3))
		{
			return -4;
		}

		m_exp_wave_vector.push_back(cfg);

		data_element = data_element.next_sibling();
	}
	return 0;
}

int DailyFBConfig::InitExpMonsterPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -200000;
	}
	while (!data_element.empty())
	{
		ExpFbWaveMonsterPos item_cfg;

		if (!PugiGetSubNodeValue(data_element, "pos_x", item_cfg.pos.x) || item_cfg.pos.x <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "pos_y", item_cfg.pos.y) || item_cfg.pos.y <= 0)
		{
			return -2;
		}

		m_exp_monster_pos.push_back(item_cfg);

		data_element = data_element.next_sibling();
	}
	return 0;
}

int DailyFBConfig::InitCoinWaveCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -200000;
	}

	short last_wave = 0;

	while (!data_element.empty())
	{
		CoinFbWaveItem wave_item;

		if (!PugiGetSubNodeValue(data_element, "wave", wave_item.wave) || wave_item.wave < 0 || wave_item.wave - last_wave != 1)
		{
			return -1;
		}
		last_wave = wave_item.wave;
		m_coin_max_wave = wave_item.wave;

		if (!PugiGetSubNodeValue(data_element, "monster_id", wave_item.monster_id) || wave_item.monster_id < 0 || !MONSTERPOOL->IsMonsterExist(wave_item.monster_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_count", wave_item.monster_count) || wave_item.monster_count < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "next_wave_time_gap", wave_item.next_wave_time_gap) || wave_item.next_wave_time_gap < 0)
		{
			return -4;
		}

		m_coin_wave_vector.push_back(wave_item);

		data_element = data_element.next_sibling();
	}
	return 0;
}

int DailyFBConfig::InitCoinPathCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -200000;
	}
	while (!data_element.empty())
	{
		Posi pos;

		if (!PugiGetSubNodeValue(data_element, "pos_x", pos.x) || pos.x <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "pos_y", pos.y) || pos.y <= 0)
		{
			return -2;
		}

		m_coin_path.push_back(pos);

		data_element = data_element.next_sibling();
	}
	return 0;
}

int DailyFBConfig::InitExpResetCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -200000;
	}

	int total_count = 0;
	while (!data_element.empty())
	{
		if (total_count < 0 || total_count >= MAX_RESET_COUNT_CFG)
		{
			return -1;
		}

		int reset_time = 0;
		if (!PugiGetSubNodeValue(data_element, "reset_time", reset_time) || reset_time <= 0)
		{
			return -2;
		}

		ExpFbResetCfg &cfg = m_expfb_reset_cfg[total_count];
		cfg.reset_time = reset_time;

		if (!PugiGetSubNodeValue(data_element, "need_gold", cfg.need_gold) || cfg.need_gold <= 0)
		{
			return -3;
		}
		total_count++;

		data_element = data_element.next_sibling();
	}
	return 0;
}

int DailyFBConfig::InitExpFbVipPayCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -200000;
	}

	int total_count = 0;
	while (!data_element.empty())
	{
		if (total_count < 0 || total_count > MAX_VIP_LEVEL)
		{
			return -1;
		}

		int vip_level = 0;
		if (!PugiGetSubNodeValue(data_element, "vip_level", vip_level) || vip_level < 0)
		{
			return -2;
		}

		ExpFbVipPayCfg &cfg = m_exp_vip_pay_cfg[total_count];
		cfg.vip_level = vip_level;

		if (!PugiGetSubNodeValue(data_element, "pay_time", cfg.pay_time) || cfg.pay_time < 0)
		{
			return -3;
		}
		total_count++;

		data_element = data_element.next_sibling();
	}
	return 0;
}

const DailyFbItem *DailyFBConfig::GetDailyFbItemCfg(int dailyfb_type)
{
	std::map<int, DailyFbItem>::const_iterator iter = m_dailyfb_map.find(dailyfb_type);
	if (m_dailyfb_map.end() == iter)
	{
		return NULL;
	}
	return &iter->second;
}

int DailyFBConfig::GetDailyFbTypeBySceneId(int scene_id)
{
	std::map<int, DailyFbItem>::const_iterator iter;
	for (iter = m_dailyfb_map.begin(); m_dailyfb_map.end() != iter; ++ iter)
	{
		if (scene_id == iter->second.scene_id)
		{
			return iter->second.dailyfb_type;
		}
	}
	return -1;
}

const ExpFbWaveItem *DailyFBConfig::GetExpFbWaveItemCfg(int level)
{
	for(int i = static_cast<int>(m_exp_wave_vector.size()) - 1; i >= 0; i--)
	{
		if (level >= m_exp_wave_vector[i].level || 0 == i)
		{
			return  &m_exp_wave_vector[i];
		}
	}

	return  NULL;
}

const CoinFbWaveItem *DailyFBConfig::GetCoinFbWaveItemCfg(short wave)
{
	for (int i = 0; i < static_cast<int>(m_coin_wave_vector.size()); ++ i)
	{
		const CoinFbWaveItem *item_cfg = &m_coin_wave_vector[i];
		if (item_cfg->wave == wave)
		{
			return item_cfg;
		}
	}

	return NULL;
}

void DailyFBConfig::GetDailyFindReward(int dailyfb_type, int role_level, int *get_exp, int *get_coin)
{
	if (NULL == get_exp || NULL == get_coin) return;

	if (DAILYFB_TYPE_EXP == dailyfb_type)
	{

	}
}

const int DailyFBConfig::GetExpFbResetGold(int reset_time)
{
	for(int i = MAX_RESET_COUNT_CFG - 1; i >= 0; i--)
	{
		if (m_expfb_reset_cfg[i].reset_time != 0 && m_expfb_reset_cfg[i].reset_time <= reset_time)
		{
			return m_expfb_reset_cfg[i].need_gold;
		}
	}

	return 0;
}

const int DailyFBConfig::GetVipPayTime(int vip_level)
{
	for(int i = MAX_VIP_LEVEL; i >= 0; i--)
	{
		if (m_exp_vip_pay_cfg[i].vip_level == vip_level)
		{
			return m_exp_vip_pay_cfg[i].pay_time;
		}
	}

	return 0;
}

int DailyFBConfig::GetExpRefreshCdByMemberNum(int member_num)
{
	if (1 == member_num)
	{
		return m_expfb_other_cfg.refresh_cd1;
	}
	else if (2 == member_num)
	{
		return m_expfb_other_cfg.refresh_cd2;
	}

	return m_expfb_other_cfg.refresh_cd3;
}

long long DailyFBConfig::GetExpBaodiByEnterTimes(int enter_times)
{
	if (enter_times <= 0 || enter_times > 3)
	{
		return 0;
	}

	if (1 == enter_times)
	{
		return m_expfb_other_cfg.baodi_exp1;
	}
	else if (2 == enter_times)
	{
		return m_expfb_other_cfg.baodi_exp2;
	}

	return m_expfb_other_cfg.baodi_exp3;
}

double DailyFBConfig::GetDailyFbFindFactor()
{
	double factor = m_expfb_other_cfg.exp_factor1 * m_expfb_other_cfg.fir_wave_refresh_count + m_expfb_other_cfg.exp_factor2 * m_expfb_other_cfg.sec_wave_refresh_count +
		m_expfb_other_cfg.exp_factor3 * m_expfb_other_cfg.other_wave_refresh_count;
	return factor*m_expfb_other_cfg.monster_count;
}