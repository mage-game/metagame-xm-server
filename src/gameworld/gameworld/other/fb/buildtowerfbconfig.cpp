#include "buildtowerfbconfig.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"
#include "item/itempool.h"
#include "checkresourcecenter.hpp"
#include "monster/monsterpool.h"

BuildTowerFbConfig::BuildTowerFbConfig() 
{

}

BuildTowerFbConfig::~BuildTowerFbConfig()
{

}

bool BuildTowerFbConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};
	PugiXmlDocument document;

	PUGI_XML_LOAD_AND_CHECK(document, path, "BuildTowerFbConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + ": xml root node error.";
		return false;
	}

	{
		// ÆäËûÅäÖÃ
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + ": xml no other node";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitOtherCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ËþÅäÖÃ
		PugiXmlNode root_element = RootElement.child("tower_config");
		if (root_element.empty())
		{
			*err = path + ": xml no tower_config node";
			return false;
		}

		iRet = this->InitTowerCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTowerCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}
	{
		// ËþÎ»ÖÃÅäÖÃ
		PugiXmlNode root_element = RootElement.child("tower_pos");
		if (root_element.empty())
		{
			*err = path + ": xml no tower_pos node";
			return false;
		}

		iRet = this->InitTowerPosCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitTowerPosCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ¹ÖÎïÂ·ÏßÅäÖÃ
		PugiXmlNode root_element = RootElement.child("monster_path");
		if (root_element.empty())
		{
			*err = path + ": xml no monster_path node";
			return false;
		}

		iRet = this->InitMonsterPathCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMonsterPathCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ¹ÖÎïË¢ÐÂÅäÖÃ
		PugiXmlNode root_element = RootElement.child("monster_flush");
		if (root_element.empty())
		{
			*err = path + ": xml no monster_flush node";
			return false;
		}

		iRet = this->InitMonsterFlushCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitMonsterFlushCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// ¹ÖÎïµôÂä¼ÇÂ¼
		PugiXmlNode root_element = RootElement.child("drop_record");
		if (root_element.empty())
		{
			*err = path + ": xml no drop_record node";
			return false;
		}

		iRet = this->InitDropRecordCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitDropRecordCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		// Ëþ·À±¾¹ÖÎï¾­Ñé
		PugiXmlNode root_element = RootElement.child("monster_exp");
		if (root_element.empty())
		{
			*err = path + ": xml no monster_exp node";
			return false;
		}

		iRet = this->InitBuildTowerFbMonsterExpCfg(root_element);
		if (iRet)
		{
			sprintf(errinfo, "%s: InitBuildTowerFbMonsterExpCfg failed %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int BuildTowerFbConfig::InitOtherCfg( PugiXmlNode RootElement )
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(data_element, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(data_element, "pos_x", m_other_cfg.born_pos.x) || m_other_cfg.born_pos.x < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(data_element, "pos_y", m_other_cfg.born_pos.y) || m_other_cfg.born_pos.y < 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(data_element, "enter_level", m_other_cfg.enter_level) || m_other_cfg.enter_level <= 0 || m_other_cfg.enter_level > MAX_ROLE_LEVEL)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(data_element, "enter_free_times", m_other_cfg.enter_free_times) || m_other_cfg.enter_free_times <= 0)
	{
		return -5;
	}

// 	if (!PugiGetSubNodeValue(data_element, "day_buy_times", m_other_cfg.day_buy_times) || m_other_cfg.day_buy_times <= 0)
// 	{
// 		return -6;
// 	}

	if (!PugiGetSubNodeValue(data_element, "buy_times_gold", m_other_cfg.buy_times_gold) || m_other_cfg.buy_times_gold <= 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(data_element, "sweep_item_id", m_other_cfg.sweep_item_id) || m_other_cfg.sweep_item_id <= 0 || NULL == ITEMPOOL->GetItem(m_other_cfg.sweep_item_id))
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(data_element, "sweep_item_num", m_other_cfg.sweep_item_num) || m_other_cfg.sweep_item_num <= 0)
	{
		return -9;

	}
	if (!PugiGetSubNodeValue(data_element, "prepare_time_second", m_other_cfg.prepare_time_second) || m_other_cfg.prepare_time_second <= 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(data_element, "two_wave_second", m_other_cfg.two_wave_second) || m_other_cfg.two_wave_second <= 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(data_element, "extra_monster_id", m_other_cfg.extra_monster_id) || !MONSTERPOOL->IsMonsterExist(m_other_cfg.extra_monster_id))
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(data_element, "extra_monster_maxhp_gongji_times", m_other_cfg.extra_monster_maxhp_gongji_times) || m_other_cfg.extra_monster_maxhp_gongji_times <= 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(data_element, "extra_call_gold", m_other_cfg.extra_call_gold) || m_other_cfg.extra_call_gold <= 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(data_element, "tower_plant_range", m_other_cfg.tower_plant_range) || m_other_cfg.tower_plant_range <= 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(data_element, "init_douhun", m_other_cfg.init_douhun) || m_other_cfg.init_douhun <= 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(data_element, "escape_num_to_failure", m_other_cfg.escape_num_to_failure) || m_other_cfg.escape_num_to_failure <= 0)
	{
		return -18;
	}

	if (!PugiGetSubNodeValue(data_element, "max_extra_monster_num", m_other_cfg.max_extra_monster_num) || m_other_cfg.max_extra_monster_num < 0)
	{
		return -19;
	}

	if (!PugiGetSubNodeValue(data_element, "add_speed_per", m_other_cfg.add_speed_per) || m_other_cfg.add_speed_per < 0)
	{
		return -20;
	}

	return 0;

}

int BuildTowerFbConfig::InitTowerCfg( PugiXmlNode RootElement )
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_tower_type = -1;
	int last_level = 0;

	while (!data_element.empty())
	{
		int tower_type = 0;
		if (!PugiGetSubNodeValue(data_element, "tower_type", tower_type) || tower_type < 0 || tower_type >= BUILD_TOWER_MAX_TYPE)
		{
			return -1;
		}

		int level = 0;
		if (!PugiGetSubNodeValue(data_element, "tower_level", level) || level <= 0 || level > BUILD_TOWER_MAX_LEVEL)
		{
			return -2;
		}

		if (tower_type == last_tower_type && level != last_level + 1)
		{
			return -10;
		}
		if (tower_type != last_tower_type && level != 1)
		{
			return -20;
		}

		last_tower_type = tower_type;
		last_level = level;

		m_build_tower_cfg[tower_type].tower_type = tower_type;
		BuildTowerFbTowerCfg::ConfigItem &cfg = m_build_tower_cfg[tower_type].cfg_list[level];
		cfg.tower_level = level;

		if (!PugiGetSubNodeValue(data_element, "monster_id", cfg.monster_id) || !MONSTERPOOL->IsMonsterExist(cfg.monster_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "need_douhun", cfg.need_douhun) || cfg.need_douhun < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "return_douhun", cfg.return_douhun) || cfg.return_douhun < 0)
		{
			return -5;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int BuildTowerFbConfig::InitTowerPosCfg( PugiXmlNode RootElement )
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_pos_index = -1;
	while (!data_element.empty())
	{
		int pos_index = 0;
		if (!PugiGetSubNodeValue(data_element, "pos_index", pos_index) || pos_index < 0 || pos_index > BUILD_TOWER_MAX_TOWER_POS_INDEX)
		{
			return -1;
		}

		if (pos_index != last_pos_index + 1)
		{
			return -10;
		}

		last_pos_index = pos_index;

		if (!PugiGetSubNodeValue(data_element, "pos_x", m_tower_pos_cfg.pos_list[pos_index].x) || m_tower_pos_cfg.pos_list[pos_index].x < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "pos_y", m_tower_pos_cfg.pos_list[pos_index].y) || m_tower_pos_cfg.pos_list[pos_index].y < 0)
		{
			return -3;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int BuildTowerFbConfig::InitMonsterPathCfg( PugiXmlNode RootElement )
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_pos_index = -1;
	while (!data_element.empty())
	{
		int pos_index = 0;
		if (!PugiGetSubNodeValue(data_element, "pos_index", pos_index) || pos_index < 0 || pos_index > BUILD_TOWER_MAX_PATH_POS_INDEX)
		{
			return -1;
		}

		if (pos_index != last_pos_index + 1)
		{
			return -10;
		}

		last_pos_index = pos_index;

		if (!PugiGetSubNodeValue(data_element, "pos_x", m_monster_path_cfg.pos_list[pos_index].x) || m_monster_path_cfg.pos_list[pos_index].x < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "pos_y", m_monster_path_cfg.pos_list[pos_index].y) || m_monster_path_cfg.pos_list[pos_index].y < 0)
		{
			return -3;
		}

		m_monster_path_cfg.cfg_count++;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int BuildTowerFbConfig::InitMonsterFlushCfg( PugiXmlNode RootElement )
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int last_wave = -1;
	while (!data_element.empty())
	{
		int wave = 0;
		if (!PugiGetSubNodeValue(data_element, "wave", wave) || wave < 0 || wave > BUILD_TOWER_MAX_MONSTER_WAVE)
		{
			return -1;
		}

		if (wave != last_wave + 1)
		{
			return -10;
		}

		last_wave = wave;

		BuildTowerFbMonsterFlushCfg::ConfigItem &cfg = m_monster_flush_cfg.cfg_list[wave];
		cfg.wave = wave;

		if (!PugiGetSubNodeValue(data_element, "monster_id", cfg.monster_id) || !MONSTERPOOL->IsMonsterExist(cfg.monster_id))
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_maxhp_gongji_times", cfg.monster_maxhp_gongji_times) || cfg.monster_maxhp_gongji_times <= 0)
		{
			return -3;
		}
		
		if (!PugiGetSubNodeValue(data_element, "flush_num", cfg.flush_num) || cfg.flush_num <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_id", cfg.boss_id) || cfg.boss_id < 0 || (cfg.boss_id > 0 && !MONSTERPOOL->IsMonsterExist(cfg.boss_id)))
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "boss_maxhp_gongji_times", cfg.boss_maxhp_gongji_times) || (cfg.boss_id > 0 && cfg.boss_maxhp_gongji_times <= 0))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_douhun", cfg.monster_douhun) || cfg.monster_douhun <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_douhun", cfg.boss_douhun) || (cfg.boss_id > 0 && cfg.boss_douhun <= 0))
		{
			return -8;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int BuildTowerFbConfig::InitDropRecordCfg(PugiXmlNode  RootElement)
{
	if (NULL == RootElement)
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	while (!data_element.empty())
	{
		ItemID item_id;
		if (!PugiGetSubNodeValue(data_element, "item_id", item_id) || NULL == ITEMPOOL->GetItem(item_id))
		{
			return -1;
		}

		int record_type;
		if (!PugiGetSubNodeValue(data_element, "record_type", record_type) || record_type <= 0 )
		{
			return -2;
		}

		BuildTowerDropRecordCfgMapIt it = m_drop_record_cfg_map.find(item_id);

		if (m_drop_record_cfg_map.end() != it)
		{
			return -30000;
		}

		m_drop_record_cfg_map[item_id] = record_type;

		data_element = data_element.next_sibling();
	}

	return 0;
}

int BuildTowerFbConfig::InitBuildTowerFbMonsterExpCfg(PugiXmlNode  RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	int last_level = 0;
	if(!PugiGetSubNodeValue(data_element, "role_level", last_level) || last_level < 0)
	{
		return -1;
	}
	last_level--;

	while (!data_element.empty())
	{
		int role_level = 0;
		if (!PugiGetSubNodeValue(data_element, "role_level", role_level) || role_level != last_level + 1 || role_level > MAX_ROLE_LEVEL)
		{
			return -2;
		}

		last_level = role_level;

		BuildTowerFbMonsterExpCfg &cfg_item = m_monster_exp_cfg_list[role_level];
		cfg_item.role_level = role_level;

		if (!PugiGetSubNodeValue(data_element, "boss_exp", cfg_item.boss_exp) || cfg_item.boss_exp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "monster_exp", cfg_item.monster_exp) || cfg_item.monster_exp < 0)
		{
			return -4;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const BuildTowerFbTowerCfg::ConfigItem * BuildTowerFbConfig::GetBuildTowerFbTowerCfg( int tower_type, int level )
{
	if (tower_type < 0 || tower_type >= BUILD_TOWER_MAX_TYPE || level <= 0 || level > BUILD_TOWER_MAX_LEVEL)
	{
		return NULL;
	}

	return &m_build_tower_cfg[tower_type].cfg_list[level];
}

const Posi * BuildTowerFbConfig::GetBuildTowerFbTowerPos( int pos_index )
{
	if (pos_index < 0 || pos_index > BUILD_TOWER_MAX_TOWER_POS_INDEX)
	{
		return NULL;
	}

	return &m_tower_pos_cfg.pos_list[pos_index];
}

const int BuildTowerFbConfig::GetBuildTowerFbMonsterPathPosCount()
{
	return m_monster_path_cfg.cfg_count;
}

const Posi * BuildTowerFbConfig::GetBuildTowerFbMonsterPathPos( int pos_index )
{
	if (pos_index < 0 || pos_index > BUILD_TOWER_MAX_PATH_POS_INDEX)
	{
		return NULL;
	}

	return &m_monster_path_cfg.pos_list[pos_index];
}

const BuildTowerFbMonsterFlushCfg::ConfigItem * BuildTowerFbConfig::GetBuildTowerFbMonsterFlushCfg( int wave )
{
	if (wave < 0 || wave > BUILD_TOWER_MAX_MONSTER_WAVE)
	{
		return NULL;
	}

	return &m_monster_flush_cfg.cfg_list[wave];
}

const int BuildTowerFbConfig::GetBuildTowerRewardDouHun( int monster_id )
{
	if (monster_id <= 0 || !MONSTERPOOL->IsMonsterExist(monster_id))
	{
		return 0;
	}

	for (int wave = 0; wave <= BUILD_TOWER_MAX_MONSTER_WAVE; wave++)
	{
		if (m_monster_flush_cfg.cfg_list[wave].boss_id == monster_id)
		{
			return m_monster_flush_cfg.cfg_list[wave].boss_douhun;
		}
		else if (m_monster_flush_cfg.cfg_list[wave].monster_id == monster_id)
		{
			return m_monster_flush_cfg.cfg_list[wave].monster_douhun;
		}
	}

	return 0;
}

const int BuildTowerFbConfig::GetDropRecordType(ItemID item_id)
{
	BuildTowerDropRecordCfgMapIt it = m_drop_record_cfg_map.find(item_id);
	if (m_drop_record_cfg_map.end() == it)
	{
		return 0;
	}

	return it->second;
}

void BuildTowerFbConfig::GetSweeepReward(int flush_extra_num, int *index_count, ItemConfigData reward_item_list[MonsterInitParam::MAX_DROP_ITEM_COUNT], long long *total_exp, int role_level)
{
	for (int wave = 0; wave <= BUILD_TOWER_MAX_MONSTER_WAVE; wave++)
	{
		const BuildTowerFbMonsterFlushCfg::ConfigItem *cfg = this->GetBuildTowerFbMonsterFlushCfg(wave);
		if (NULL == cfg) continue;

		for (int i = 0; i < cfg->flush_num; i++)
		{
			long long reward_exp = static_cast<long long>(this->GetThisMonsterExp(cfg->monster_id, role_level));
			MONSTERPOOL->GetMonsterDrop(cfg->monster_id, reward_item_list, index_count, &reward_exp, NULL, NULL);
			*total_exp += reward_exp;
		}

		if (cfg->boss_id > 0)
		{
			long long reward_exp =static_cast<long long>(this->GetThisMonsterExp(cfg->boss_id, role_level));
			MONSTERPOOL->GetMonsterDrop(cfg->boss_id, reward_item_list, index_count, &reward_exp, NULL, NULL);
			*total_exp += reward_exp;
		}
	}

	if (flush_extra_num > 0)
	{
		for (int wave = 0; wave < flush_extra_num; wave++)
		{
			long long reward_exp = 0;
			MONSTERPOOL->GetMonsterDrop(m_other_cfg.extra_monster_id, reward_item_list, index_count, &reward_exp, NULL, NULL);
			*total_exp += reward_exp;
		}
	}
}

long long BuildTowerFbConfig::GetThisMonsterExp(int monster_id, int role_level)
{
	if (monster_id <= 0 || !MONSTERPOOL->IsMonsterExist(monster_id))
	{
		return 0;
	}

	for (int wave = 0; wave <= BUILD_TOWER_MAX_MONSTER_WAVE; wave++)
	{
		if (m_monster_flush_cfg.cfg_list[wave].boss_id == monster_id)
		{
			return m_monster_exp_cfg_list[role_level].boss_exp;
		}

		if (m_monster_flush_cfg.cfg_list[wave].monster_id == monster_id)
		{
			return m_monster_exp_cfg_list[role_level].monster_exp;
		}
	}

	return 0;
}

const BuildTowerFbMonsterExpCfg * BuildTowerFbConfig::GetBuildTowerFbMonsterExpCfg(int role_level)
{
	if (role_level <= 0 || role_level > MAX_ROLE_LEVEL)
	{
		return NULL;
	}

	return &m_monster_exp_cfg_list[role_level];
}

