#include "shanggubossconfig.hpp"
#include "engineadapter.h"
#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"

#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"
#include "scene/sceneregister/sceneregister.hpp"
#include "scene/scene.h"
#include <set>
#include <string>
#include <time.h>
#include <memory.h>
#include <stdlib.h>


ShangGuBossConfig::ShangGuBossConfig()
{
}

ShangGuBossConfig::~ShangGuBossConfig()
{
}

bool ShangGuBossConfig::Init(const std::string &configname, std::string *err)
{
	char errinfo[1024] = {0};
	int iRet = 0;

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ShangGuBossConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	{
		PugiXmlNode shangguboss_element = RootElement.child("other");
		if (shangguboss_element.empty())
		{
			*err = configname + ": no [other].";
			return false;
		}

		iRet = this->InitShangGuBossOtherCfg(shangguboss_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitShangGuBossOtherCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode shangguboss_element = RootElement.child("shanggu_boss_layer");
		if (shangguboss_element.empty())
		{
			*err = configname + ": no [shanggu_boss_layer].";
			return false;
		}

		iRet = this->InitShangGuBossLayerCfg(shangguboss_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitShangGuBossLayerCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode bossflush_element = RootElement.child("shanggu_boss_flush");
		if (bossflush_element.empty())
		{
			*err = configname + ": no [shanggu_boss_flush].";
			return false;
		}

		iRet = this->InitShangGuBossFlushCfg(bossflush_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitShangGuBossFlushCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode boss_info_element = RootElement.child("shanggu_boss_info");
		if (boss_info_element.empty())
		{
			*err = configname + ": no [shanggu_boss_info].";
			return false;
		}

		iRet = this->InitShangGuBossInfoCfg(boss_info_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitShangGuBossInfoCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode boss_hide_element = RootElement.child("shanggu_boss_hide");
		if (boss_hide_element.empty())
		{
			*err = configname + ": no [shanggu_boss_hide].";
			return false;
		}

		iRet = this->InitShangGuBossHideCfg(boss_hide_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitShangGuBossHideCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode tiky_consume_element = RootElement.child("shanggu_boss_tiky_consume");
		if (tiky_consume_element.empty())
		{
			*err = configname + ": no [shanggu_boss_tiky_consume].";
			return false;
		}

		iRet = this->InitShangGuBossConsumeCfg(tiky_consume_element);
		if (iRet)
		{
			sprintf(errinfo,"%s: InitShangGuBossConsumeCfg failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

int ShangGuBossConfig::InitShangGuBossOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "shanggu_open_level", m_other_cfg.shanggu_open_level) || m_other_cfg.shanggu_open_level < 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "shanggu_day_free_times", m_other_cfg.shanggu_day_free_times) || m_other_cfg.shanggu_day_free_times < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "shanggu_day_max_tire", m_other_cfg.shanggu_day_max_tire) || m_other_cfg.shanggu_day_max_tire <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "shanggu_kill_boss_add_tire", m_other_cfg.shanggu_kill_boss_add_tire) || m_other_cfg.shanggu_kill_boss_add_tire < 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "shanggu_tiky_item_id", m_other_cfg.shanggu_tiky_item_id) || m_other_cfg.shanggu_tiky_item_id <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "shanggu_angry_add_interval_s", m_other_cfg.shanggu_angry_add_interval_s) || m_other_cfg.shanggu_angry_add_interval_s < 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "shanggu_angry_kill_monster_add", m_other_cfg.shanggu_angry_kill_monster_add) || m_other_cfg.shanggu_angry_kill_monster_add < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "shanggu_angry_min_box_add", m_other_cfg.shanggu_angry_min_box_add) || m_other_cfg.shanggu_angry_min_box_add < 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement, "shanggu_min_box_id", m_other_cfg.shanggu_min_box_id) || m_other_cfg.shanggu_min_box_id < 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElement, "shanggu_gather_min_box_time_s", m_other_cfg.shanggu_gather_min_box_time_s) || m_other_cfg.shanggu_gather_min_box_time_s <= 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElement, "shanggu_angry_max_box_add", m_other_cfg.shanggu_angry_max_box_add) || m_other_cfg.shanggu_angry_max_box_add < 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(dataElement, "shanggu_max_box_id", m_other_cfg.shanggu_max_box_id) || m_other_cfg.shanggu_max_box_id <= 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(dataElement, "shanggu_gather_max_box_time_s", m_other_cfg.shanggu_gather_max_box_time_s) || m_other_cfg.shanggu_gather_max_box_time_s <= 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(dataElement, "shanggu_angry_die_add", m_other_cfg.shanggu_angry_die_add) || m_other_cfg.shanggu_angry_die_add < 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(dataElement, "shanggu_angry_kick_need_tims_s", m_other_cfg.shanggu_angry_kick_need_tims_s) || m_other_cfg.shanggu_angry_kick_need_tims_s < 0)
	{
		return -15;
	}

	PugiXmlNode Itemelement = dataElement.child("reward_item_min");
	int item_id = 0;
	if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", item_id) && item_id > 0)
	{
		if (!m_other_cfg.reward_item_min.ReadConfig(Itemelement))
		{
			return -16;
		}
	}

	Itemelement = dataElement.child("reward_item_max");
	if (NULL != Itemelement && PugiGetSubNodeValue(Itemelement, "item_id", item_id) && item_id > 0)
	{
		if (!m_other_cfg.reward_item_max.ReadConfig(Itemelement))
		{
			return -17;
		}
	}

	if (!PugiGetSubNodeValue(dataElement, "can_use_bind_gold", m_other_cfg.can_use_bind_gold))
	{
		return -18;
	}

	int is_open = 0;
	if (!PugiGetSubNodeValue(dataElement, "is_open", is_open))
	{
		return -19;
	}

	m_other_cfg.is_open = is_open > 0;

	return 0;
}

int ShangGuBossConfig::InitShangGuBossLayerCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	m_shanggu_boss_layer_count = 0;

	while (!dataElement.empty())
	{
		int layer = -1;
		if (!PugiGetSubNodeValue(dataElement, "layer", layer) || layer < 0 || layer >= SHANGGU_BOSS_CFG_MAX_LAYER || layer != m_shanggu_boss_layer_count)
		{
			return -1;
		}

		ShangGuBossLayerConfig &layer_cfg = m_shanggu_boss_layer_cfg_list[layer];
		layer_cfg.layer = layer;

		if (!PugiGetSubNodeValue(dataElement, "scene_id", layer_cfg.scene_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x", layer_cfg.pos.x) || layer_cfg.pos.x <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", layer_cfg.pos.y) || layer_cfg.pos.y <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_role_level", layer_cfg.need_role_level) || layer_cfg.need_role_level <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "min_box_num", layer_cfg.min_box_num) || layer_cfg.min_box_num <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_box_num", layer_cfg.max_box_num) || layer_cfg.max_box_num <= 0)
		{
			return -8;
		}

		++m_shanggu_boss_layer_count;
		dataElement = dataElement.next_sibling();
	}

	if (m_shanggu_boss_layer_count > SHANGGU_BOSS_CFG_MAX_LAYER)
	{
		return -100;
	}

	return 0;
}

int ShangGuBossConfig::InitShangGuBossFlushCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		ShangGuBossFlushConfig cfg;

		if (!PugiGetSubNodeValue(dataElement, "seq", cfg.seq) || cfg.seq < 0)
		{
			return -1;
		}

		int temp = 0;
		if (!PugiGetSubNodeValue(dataElement, "time", temp) || temp / 100 < 0 || temp / 100 > 23 || temp % 100 < 0 || temp % 100 > 59)
		{
			return -3;
		}

		cfg.time = (temp / 100) * SECOND_PER_HOUR + (temp % 100) * SECOND_PER_MIN;

		m_shanggu_boss_flush_cfg_vec.push_back(cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ShangGuBossConfig::InitShangGuBossInfoCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		int layer = -1;
		if (!PugiGetSubNodeValue(dataElement, "layer", layer) || layer < 0 || layer >= SHANGGU_BOSS_CFG_MAX_LAYER || layer >= m_shanggu_boss_layer_count)
		{
			return -1;
		}
		ShangGuBossInfoConfig &boss_info_cfg = m_shanggu_boss_info_cfg_list[layer];
		boss_info_cfg.layer = layer;
		boss_info_cfg.scene_id = m_shanggu_boss_layer_cfg_list[layer].scene_id;

		if (boss_info_cfg.boss_item_count >= ShangGuBossInfoConfig::MAX_BOSS_COUNT)
		{
			return -2;
		}

		ShangGuBossInfoConfig::ItemConfig &item_cfg = boss_info_cfg.boss_item_list[boss_info_cfg.boss_item_count];
		item_cfg.index = boss_info_cfg.boss_item_count;


		if (!PugiGetSubNodeValue(dataElement, "type", item_cfg.type) || item_cfg.type < SHANGGU_BOSS_TYPE_BOSS || item_cfg.type >= SHANGGU_BOSS_TYPE_MAX)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_id", item_cfg.boss_id) )//|| !MONSTERPOOL->IsMonsterExist(item_cfg.boss_id))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "x_pos", item_cfg.pos.x) || item_cfg.pos.x <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "y_pos", item_cfg.pos.y) || item_cfg.pos.y <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "kill_add_angry", item_cfg.kill_add_angry) || item_cfg.kill_add_angry <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_delta_level", item_cfg.max_delta_level) || item_cfg.max_delta_level <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_lv_seq", item_cfg.boss_lv_seq) || item_cfg.boss_lv_seq < 0 || item_cfg.boss_lv_seq > SHANGGU_BOSS_HIDE_BOSS_MAX_LEVEL_NUM)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "min_delta_level", item_cfg.min_delta_level) || item_cfg.min_delta_level <= 0)
		{
			return -10;
		}

		++boss_info_cfg.boss_item_count;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ShangGuBossConfig::InitShangGuBossHideCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	while (!dataElement.empty())
	{
		int boss_lv_seq = 0;
		if (!PugiGetSubNodeValue(dataElement, "boss_lv_seq", boss_lv_seq) || boss_lv_seq <= 0 || boss_lv_seq > SHANGGU_BOSS_HIDE_BOSS_MAX_LEVEL_NUM)
		{
			return -1;
		}
		ShangGuBossHideBossConfig &boss_info_cfg = m_shanggu_boss_hide_boss_cfg_list[boss_lv_seq];
		boss_info_cfg.boss_lv_seq = boss_lv_seq;

		int seq = -1;
		if (!PugiGetSubNodeValue(dataElement, "seq", seq) || seq < 0 || seq >= ShangGuBossHideBossConfig::LEVEL_MAX_MONSTER_COUNT)
		{
			return -2;
		}

		ShangGuBossHideBossConfig::LevelConfig &lv_cfg = boss_info_cfg.monster_list[seq];
		lv_cfg.seq = seq;

		if (!PugiGetSubNodeValue(dataElement, "monster_id", lv_cfg.monster_id))// || !MONSTERPOOL->IsMonsterExist(lv_cfg.monster_id))
		{
			return -3;
		}
		++boss_info_cfg.monster_count;

		if (!PugiGetSubNodeValue(dataElement, "kill_add_angry", lv_cfg.kill_add_angry) || lv_cfg.kill_add_angry <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x0", lv_cfg.pos_list[0].x) || lv_cfg.pos_list[0].x <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y0", lv_cfg.pos_list[0].y) || lv_cfg.pos_list[0].y <= 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x1", lv_cfg.pos_list[1].x) || lv_cfg.pos_list[1].x <= 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y1", lv_cfg.pos_list[1].y) || lv_cfg.pos_list[1].y <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x2", lv_cfg.pos_list[2].x) || lv_cfg.pos_list[2].x <= 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y2", lv_cfg.pos_list[2].y) || lv_cfg.pos_list[2].y <= 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x3", lv_cfg.pos_list[3].x) || lv_cfg.pos_list[3].x <= 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y3", lv_cfg.pos_list[3].y) || lv_cfg.pos_list[3].y <= 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x4", lv_cfg.pos_list[4].x) || lv_cfg.pos_list[4].x <= 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y4", lv_cfg.pos_list[4].y) || lv_cfg.pos_list[4].y <= 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x5", lv_cfg.pos_list[5].x) || lv_cfg.pos_list[5].x <= 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y5", lv_cfg.pos_list[5].y) || lv_cfg.pos_list[5].y <= 0)
		{
			return -16;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x6", lv_cfg.pos_list[6].x) || lv_cfg.pos_list[6].x <= 0)
		{
			return -17;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y6", lv_cfg.pos_list[6].y) || lv_cfg.pos_list[6].y <= 0)
		{
			return -18;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_x7", lv_cfg.pos_list[7].x) || lv_cfg.pos_list[7].x <= 0)
		{
			return -19;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y7", lv_cfg.pos_list[7].y) || lv_cfg.pos_list[7].y <= 0)
		{
			return -20;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int ShangGuBossConfig::InitShangGuBossConsumeCfg(PugiXmlNode RootElement)
{
	m_shanggu_boss_max_enter_times = 0;

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		if (m_shanggu_boss_max_enter_times >= SHANGGU_BOSS_MAX_ENTER_TIMES)
		{
			return -1000;
		}

		int enter_times = 0;
		if (!PugiGetSubNodeValue(dataElement, "enter_times", enter_times) || enter_times != m_shanggu_boss_max_enter_times + 1)
		{
			return -1;
		}
		m_shanggu_boss_max_enter_times = enter_times;

		int need_consume_item_num = 0;
		if (!PugiGetSubNodeValue(dataElement, "need_consume_item_num", need_consume_item_num) || need_consume_item_num <= 0)
		{
			return -2;
		}

		m_shanggu_boss_enter_times_need_consume_item_num_list[enter_times] = need_consume_item_num;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const ShangGuBossLayerConfig * ShangGuBossConfig::GetShangGuBossLayerCfgBySceneId(int scene_id)
{
	for (int layer = 0; layer < SHANGGU_BOSS_CFG_MAX_LAYER && layer < m_shanggu_boss_layer_count; ++layer)
	{
		if (scene_id == m_shanggu_boss_layer_cfg_list[layer].scene_id)
		{
			return &m_shanggu_boss_layer_cfg_list[layer];
		}
	}

	return NULL;
}

const ShangGuBossLayerConfig * ShangGuBossConfig::GetShangGuBossLayerCfg(int layer)
{
	if (layer < 0 || layer >= SHANGGU_BOSS_CFG_MAX_LAYER || layer >= m_shanggu_boss_layer_count)
	{
		return NULL;
	}

	return &m_shanggu_boss_layer_cfg_list[layer];
}

const ShangGuBossInfoConfig::ItemConfig * ShangGuBossConfig::GetShangGuBossInfo(int layer, int index)
{
	if (layer < 0 || layer >= SHANGGU_BOSS_CFG_MAX_LAYER || layer >= m_shanggu_boss_layer_count ||
		index < 0 || index >= m_shanggu_boss_info_cfg_list[layer].boss_item_count || index >= ShangGuBossInfoConfig::MAX_BOSS_COUNT)
	{
		return NULL;
	}

	return &m_shanggu_boss_info_cfg_list[layer].boss_item_list[index];
}

const ShangGuBossInfoConfig::ItemConfig * ShangGuBossConfig::GetShangGuBossInfoCfg(int layer, int boss_id)
{
	if (layer < 0 || layer >= SHANGGU_BOSS_CFG_MAX_LAYER || layer >= m_shanggu_boss_layer_count)
	{
		return NULL;
	}

	for (int i = 0; i < m_shanggu_boss_info_cfg_list[layer].boss_item_count; ++i)
	{
		const ShangGuBossInfoConfig::ItemConfig &item = m_shanggu_boss_info_cfg_list[layer].boss_item_list[i];
		if (item.boss_id == boss_id)
		{
			return &item;
		}
	}

	return NULL;
}

const ShangGuBossInfoConfig::ItemConfig * ShangGuBossConfig::GetShangGuBossInfoCfgByBossId(int boss_id)
{
	for (int layer = 0; layer < m_shanggu_boss_layer_count && layer < SHANGGU_BOSS_CFG_MAX_LAYER; ++layer)
	{
		for (int i = 0; i < m_shanggu_boss_info_cfg_list[layer].boss_item_count; ++i)
		{
			const ShangGuBossInfoConfig::ItemConfig &item = m_shanggu_boss_info_cfg_list[layer].boss_item_list[i];
			if (item.boss_id == boss_id)
			{
				return &item;
			}
		}
	}
	return NULL;
}

const ShangGuBossInfoConfig * ShangGuBossConfig::GetShangGuBossCfgByBossId(int boss_id)
{
	for (int layer = 0; layer < m_shanggu_boss_layer_count && layer < SHANGGU_BOSS_CFG_MAX_LAYER; ++layer)
	{
		for (int i = 0; i < m_shanggu_boss_info_cfg_list[layer].boss_item_count; ++i)
		{
			const ShangGuBossInfoConfig::ItemConfig &item = m_shanggu_boss_info_cfg_list[layer].boss_item_list[i];
			if (item.boss_id == boss_id)
			{
				return &m_shanggu_boss_info_cfg_list[layer];
			}
		}
	}
	return NULL;
}

const ShangGuBossHideBossConfig::LevelConfig * ShangGuBossConfig::GetShangGuBossHideBossCfg(int boss_lv_seq, int seq)
{
	if (boss_lv_seq <= 0 || boss_lv_seq > SHANGGU_BOSS_HIDE_BOSS_MAX_LEVEL_NUM || seq < 0 ||
		seq >= m_shanggu_boss_hide_boss_cfg_list[boss_lv_seq].monster_count || seq >= ShangGuBossHideBossConfig::LEVEL_MAX_MONSTER_COUNT)
	{
		return NULL;
	}

	return &m_shanggu_boss_hide_boss_cfg_list[boss_lv_seq].monster_list[seq];
}

const ShangGuBossHideBossConfig::LevelConfig * ShangGuBossConfig::GetShangGuBossHideBossCfgByMonsterId(int monster_id)
{
	for (int i = 1; i <= SHANGGU_BOSS_HIDE_BOSS_MAX_LEVEL_NUM; ++i)
	{
		for (int j = 0; j < m_shanggu_boss_hide_boss_cfg_list[i].monster_count && j < ShangGuBossHideBossConfig::LEVEL_MAX_MONSTER_COUNT; ++j)
		{
			if (monster_id == m_shanggu_boss_hide_boss_cfg_list[i].monster_list[j].monster_id)
			{
				return &m_shanggu_boss_hide_boss_cfg_list[i].monster_list[j];
			}
		}
	}

	return NULL;
}

const ShangGuBossInfoConfig::ItemConfig * ShangGuBossConfig::GetShangGuBossInfoWithSceneID(int scene_id, int boss_id) const
{
	for (int layer = 0; layer < SHANGGU_BOSS_CFG_MAX_LAYER && layer < m_shanggu_boss_layer_count; ++layer)
	{
		const ShangGuBossInfoConfig &info_cfg = m_shanggu_boss_info_cfg_list[layer];
		if (info_cfg.scene_id != scene_id) continue;

		for (int i = 0; i < info_cfg.boss_item_count && i < ShangGuBossInfoConfig::MAX_BOSS_COUNT; ++i)
		{
			if (info_cfg.boss_item_list[i].boss_id == boss_id)
			{
				return &info_cfg.boss_item_list[i];
			}
		}
	}

	return NULL;
}

int ShangGuBossConfig::GetShangGuBossCount(int layer)
{
	if (layer < 0 || layer >= SHANGGU_BOSS_CFG_MAX_LAYER || layer >= m_shanggu_boss_layer_count)
	{
		return 0;
	}

	return m_shanggu_boss_info_cfg_list[layer].boss_item_count;
}

int ShangGuBossConfig::GetShangGuBossHideBossCount(int boss_lv_seq)
{
	if (boss_lv_seq <= 0 || boss_lv_seq > SHANGGU_BOSS_HIDE_BOSS_MAX_LEVEL_NUM)
	{
		return 0;
	}

	return m_shanggu_boss_hide_boss_cfg_list[boss_lv_seq].monster_count;
}

int ShangGuBossConfig::GetShangGuBossNextFlushTimeInterval(unsigned int now_second)
{
	int day_second = now_second - static_cast<unsigned int>(GetZeroTime(now_second));

	for (int i = 0; i < (int)m_shanggu_boss_flush_cfg_vec.size(); ++i)
	{
		if (m_shanggu_boss_flush_cfg_vec[i].time > day_second)
		{
			return m_shanggu_boss_flush_cfg_vec[i].time - day_second;
		}
	}

	return static_cast<int>(GetZeroTime(now_second + SECOND_PER_DAY) - now_second + m_shanggu_boss_flush_cfg_vec[0].time);
}

unsigned int ShangGuBossConfig::GetShangGuBossGatherItemNextFlushTime(unsigned int now_second)
{
	int day_second = now_second - static_cast<unsigned int>(GetZeroTime(now_second));

	for (int i = 0; i < (int)m_shanggu_boss_flush_cfg_vec.size(); ++i)
	{
		if (m_shanggu_boss_flush_cfg_vec[i].time > day_second)
		{
			return static_cast<unsigned int>(GetZeroTime(now_second) + m_shanggu_boss_flush_cfg_vec[i].time);
		}
	}

	return static_cast<unsigned int>(GetZeroTime(now_second + SECOND_PER_DAY) + m_shanggu_boss_flush_cfg_vec[0].time);
}

int ShangGuBossConfig::GetShangGuBossLvSeqCount(int layer, int boss_lv_seq)
{
	if (layer < 0 || layer >= SHANGGU_BOSS_CFG_MAX_LAYER || layer >= m_shanggu_boss_layer_count)
	{
		return 0;
	}

	int count = 0;
	for (int i = 0; i < m_shanggu_boss_info_cfg_list[layer].boss_item_count && i < ShangGuBossInfoConfig::MAX_BOSS_COUNT; ++i)
	{
		if (boss_lv_seq == m_shanggu_boss_info_cfg_list[layer].boss_item_list[i].boss_lv_seq)
		{
			++count;
		}
	}

	return count;
}

int ShangGuBossConfig::GetShangGuBossEnterConsumeStuffNum(short enter_times)
{
	if (enter_times < 0 || enter_times > m_shanggu_boss_max_enter_times)
	{
		return INT_MAX;
	}

	return m_shanggu_boss_enter_times_need_consume_item_num_list[enter_times];
}

bool ShangGuBossConfig::IsShangguBossMonster(int boss_id)
{
	for (int layer = 0; layer < m_shanggu_boss_layer_count && layer < SHANGGU_BOSS_CFG_MAX_LAYER; ++layer)
	{
		for (int i = 0; i < m_shanggu_boss_info_cfg_list[layer].boss_item_count; ++i)
		{
			const ShangGuBossInfoConfig::ItemConfig &item = m_shanggu_boss_info_cfg_list[layer].boss_item_list[i];
			if (item.boss_id == boss_id)
			{
				return item.type == SHANGGU_BOSS_TYPE_BOSS;
			}
		}
	}

	return false;
}