#include "worldeventconfig.hpp"
#include "obj/otherobj/worldeventobj.h"
#include "engineadapter.h"
#include "scene/scene.h"
#include "servercommon/configcommon.h"
#include "monster/monsterpool.h"
#include <set>
#include "checkresourcecenter.hpp"
#include "item/itempool.h"
#include "item/itembase.h"
#include "servercommon/serverconfig/configstruct.h"
#include "servercommon/serverconfig/localconfig.hpp"

WorldEventSceneConfigItem * WorldEventNpcPosConfig::FindSceneCfg(int scene_id)
{
	for (int i = 0; i < WORLD_EVENT_NPC_LOCATE_SCENE_MAX_COUNT; ++ i)
	{
		if (scene_id == scene_list[i].scene_id)
		{
			return &scene_list[i];
		}
	}

	return NULL;
}

void WorldEventSceneConfigItem::RandPos()
{
	for (int i = 0; i < pos_count; ++ i)
	{
		int rand_pos = RandomNum(pos_count);

		Posi tmp_pos = pos_list[i];
		pos_list[i] = pos_list[rand_pos];
		pos_list[rand_pos] = tmp_pos;
	}
}

bool WorldEventSceneConfigItem::TopPos(Posi *pos)
{
	if (NULL != pos && pos_count > 0)
	{
		*pos = pos_list[pos_count - 1];
		return true;
	}

	return false;
}

bool WorldEventSceneConfigItem::PushPos(const Posi &pos)
{
	if (pos_count >= WORLD_EVENT_POS_CONFIG_MAX_COUNT_PER_SCENE)
	{
		return false;
	}

	pos_list[pos_count] = pos;
	++ pos_count;

	return true;
}

void WorldEventSceneConfigItem::PopPos()
{
	-- pos_count;
	if (pos_count < 0)
	{
		pos_count = 0;
	}
}

void WorldEventPosCfg::RandPos()
{
	for (int i = 0; i < pos_item_count; ++ i)
	{
		int rand_pos = RandomNum(pos_item_count);

		WorldEventPosItem tmp_pos = pos_item_list[i];
		pos_item_list[i] = pos_item_list[rand_pos];
		pos_item_list[rand_pos] = tmp_pos;
	}
}
bool WorldEventPosCfg::Top(WorldEventPosItem *pos_item)
{
	if (NULL != pos_item && pos_item_count > 0)
	{
		*pos_item = pos_item_list[pos_item_count - 1];
		return true;
	}

	return false;
}

bool WorldEventPosCfg::Push(const WorldEventPosItem &pos_item)
{
	if (pos_item_count >= WORLD_EVENT_POS_CONFIG_MAX_COUNT)
	{
		return false;
	}

	pos_item_list[pos_item_count] = pos_item;
	++ pos_item_count;
	return true;
}

void WorldEventPosCfg::Pop()
{
	-- pos_item_count;
	if (pos_item_count < 0)
	{
		pos_item_count = 0;
	}
}

/*
static long long ConvertParamToLongLong(int param_0, short param_1, short param_2)
{
	union 
	{
		long long ll_value; 
		struct  
		{
			int param_0;
			short param_1;
			short param_2;
		};
	} union_param;

	union_param.param_0 = param_0;
	union_param.param_1 = param_1;
	union_param.param_2 = param_2;

	return union_param.ll_value;
}
*/

WorldEventConfig::WorldEventConfig()
{
	memset(m_npc_idlist, 0, sizeof(m_npc_idlist));
}

WorldEventConfig::~WorldEventConfig()
{

}

bool WorldEventConfig::Init(std::string path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "WorldEventConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	//{
	//	// 世界对象配置
	//	PugiXmlNode root_element = RootElement.child("world_event_obj");
	//	if (root_element.empty())
	//	{
	//		*err = path + " xml not world_event_obj node ";
	//		return false;
	//	}

	//	iRet = this->InitWorldEventObjCfg(root_element);
	//	if (0 != iRet)
	//	{
	//		sprintf(err_info, "%s InitWorldEventObj fail %d ", path.c_str(), iRet);
	//		*err = err_info;
	//		return false;
	//	}
	//}

	//{
	//	// 事件活动开启配置
	//	PugiXmlNode root_element = RootElement.child("open_cfg");
	//	if (root_element.empty())
	//	{
	//		*err = path + " xml not open_cfg node ";
	//		return false;
	//	}

	//	iRet = this->InitOpenCfg(root_element);
	//	if (0 != iRet)
	//	{
	//		sprintf(err_info, "%s InitOpenCfg fail %d ", path.c_str(), iRet);
	//		*err = err_info;
	//		return false;
	//	}
	//}

	//{
	//	// npc配置
	//	PugiXmlNode root_element = RootElement.child("npc_pos");
	//	if (root_element.empty())
	//	{
	//		*err = path + " xml not npc_pos node ";
	//		return false;
	//	}

	//	iRet = this->InitNpcPosCfg(root_element);
	//	if (0 != iRet)
	//	{
	//		sprintf(err_info, "%s InitNpcPosCfg fail %d ", path.c_str(), iRet);
	//		*err = err_info;
	//		return false;
	//	}
	//}

	//{
	//	// npc id配置
	//	PugiXmlNode root_element = RootElement.child("npc_obj");
	//	if (root_element.empty())
	//	{
	//		*err = path + " xml not npc_pos node ";
	//		return false;
	//	}

	//	iRet = this->InitNpcObjCfg(root_element);
	//	if (0 != iRet)
	//	{
	//		sprintf(err_info, "%s InitNpcObjCfg fail %d ", path.c_str(), iRet);
	//		*err = err_info;
	//		return false;
	//	}
	//}

	//{
	//	// 其他配置
	//	PugiXmlNode root_element = RootElement.child("other");
	//	if (root_element.empty())
	//	{
	//		*err = path + " xml not other node ";
	//		return false;
	//	}

	//	iRet = this->InitOtherCfg(root_element);
	//	if (0 != iRet)
	//	{
	//		sprintf(err_info, "%s InitOtherCfg fail %d ", path.c_str(), iRet);
	//		*err = err_info;
	//		return false;
	//	}
	//}

	//{
	//	// 事件类型配置
	//	PugiXmlNode root_element = RootElement.child("event_type");
	//	if (root_element.empty())
	//	{
	//		*err = path + " xml not event_type node ";
	//		return false;
	//	}

	//	iRet = this->InitWorldEventTypeCfg(root_element);
	//	if (0 != iRet)
	//	{
	//		sprintf(err_info, "%s InitWorldEventTypeCfg fail %d ", path.c_str(), iRet);
	//		*err = err_info;
	//		return false;
	//	}
	//}

	//{
	//	// 神魔之隙配置
	//	PugiXmlNode root_element = RootElement.child("shenmozhixi_pos_cfg");
	//	if (root_element.empty())
	//	{
	//		*err = path + " xml not shenmozhixi_pos_cfg node ";
	//		return false;
	//	}

	//	iRet = this->InitShenmozhijiPosCfg(root_element);
	//	if (0 != iRet)
	//	{
	//		sprintf(err_info, "%s InitShenmozhijiPosCfg fail %d ", path.c_str(), iRet);
	//		*err = err_info;
	//		return false;
	//	}
	//}

	//{
	//	// 天女散花掉落配置
	//	PugiXmlNode root_element = RootElement.child("tiannvsanhua_falling_item_cfg");
	//	if (root_element.empty())
	//	{
	//		*err = path + " xml not tiannvsanhua_falling_item_cfg node ";
	//		return false;
	//	}

	//	iRet = this->InitTiannvsanhuaFallingItemCfg(root_element);
	//	if (0 != iRet)
	//	{
	//		sprintf(err_info, "%s InitTiannvsanhuaFallingItemCfg fail %d ", path.c_str(), iRet);
	//		*err = err_info;
	//		return false;
	//	}
	//}

	//{
	//	// 天女散花移动配置
	//	PugiXmlNode root_element = RootElement.child("tiannvsanhua_move_cfg");
	//	if (root_element.empty())
	//	{
	//		*err = path + " xml not tiannvsanhua_move_cfg node ";
	//		return false;
	//	}

	//	iRet = this->InitTiannvsanhuaMoveCfg(root_element);
	//	if (0 != iRet)
	//	{
	//		sprintf(err_info, "%s InitTiannvsanhuaMoveCfg fail %d ", path.c_str(), iRet);
	//		*err = err_info;
	//		return false;
	//	}
	//}

	//{
	//	// 点亮字配置
	//	PugiXmlNode root_element = RootElement.child("light_word_cfg");
	//	if (root_element.empty())
	//	{
	//		*err = path + " xml not light_word_cfg node ";
	//		return false;
	//	}

	//	iRet = this->InitLightWordCfg(root_element);
	//	if (0 != iRet)
	//	{
	//		sprintf(err_info, "%s InitLightWordCfg fail %d ", path.c_str(), iRet);
	//		*err = err_info;
	//		return false;
	//	}
	//}

	{
		// 外观配置
		PugiXmlNode root_element = RootElement.child("appearance_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not appearance_cfg node ";
			return false;
		}

		iRet = this->InitAppearanceCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitAppearanceCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	//{
	//	// 随机boss配置
	//	iRet = this->InitKillRandomBossConfig(RootElement);
	//	if (0 != iRet)
	//	{
	//		sprintf(err_info, "%s InitKillRandomBossConfig fail %d ", path.c_str(), iRet);
	//		*err = err_info;
	//		return false;
	//	}
	//}

	//{
	//	// 激情PK配置
	//	iRet = this->InitJiqingPKConfig(RootElement);
	//	if (0 != iRet)
	//	{
	//		sprintf(err_info, "%s InitJiqingPKConfig fail %d ", path.c_str(), iRet);
	//		*err = err_info;
	//		return false;
	//	}
	//}

	//{
	//	// 虚空之门
	//	iRet = this->InitXukongzhimengConfig(RootElement);
	//	if (0 != iRet)
	//	{
	//		sprintf(err_info, "%s InitXukongzhimengConfig fail %d ", path.c_str(), iRet);
	//		*err = err_info;
	//		return false;
	//	}
	//}

	//{
	//	// 全民塔防
	//	iRet = this->InitQuanmintafangConfig(RootElement);
	//	if (0 != iRet)
	//	{
	//		sprintf(err_info, "%s InitQuanmintafangConfig fail %d ", path.c_str(), iRet);
	//		*err = err_info;
	//		return false;
	//	}
	//}

	//{
	//	// 奖励配置
	//	PugiXmlNode root_element = RootElement.child("event_reward");
	//	if (root_element.empty())
	//	{
	//		*err = path + " xml not event_reward node ";
	//		return false;
	//	}

	//	iRet = this->InitRewardConfig(root_element);
	//	if (0 != iRet)
	//	{
	//		sprintf(err_info, "%s InitRewardConfig fail %d ", path.c_str(), iRet);
	//		*err = err_info;
	//		return false;
	//	}
	//}

	return true;
}

bool WorldEventConfig::GetNPCPos(int scene_cfg_index, int pos_cfg_index, int *scene_id, Posi *pos)
{
	if (scene_cfg_index >= 0 && scene_cfg_index < WORLD_EVENT_NPC_LOCATE_SCENE_MAX_COUNT)
	{
		const WorldEventSceneConfigItem &scene_cfg_item = m_npc_pos_cfg.scene_list[scene_cfg_index];
		
		if (pos_cfg_index >= 0 && pos_cfg_index < scene_cfg_item.pos_count)
		{
			if (NULL != scene_id) *scene_id = scene_cfg_item.scene_id;
			if (NULL != pos) *pos = scene_cfg_item.pos_list[pos_cfg_index];

			return true;
		}
	}

	return false;
}

const WorldEventLogicOpenConfig * WorldEventConfig::GetLogicOpenCfg(int event_type)
{
	if (event_type <= WORLD_EVENT_TYPE_INVALID || event_type >= WORLD_EVENT_TYPE_MAX)
	{
		return NULL;
	}

	return &m_logic_open_cfglist[event_type];
}

void WorldEventConfig::GetNpcIDList(int npc_idlist[WORLD_EVENT_NPC_TYPE_MAX_COUNT])
{
	memcpy(npc_idlist, m_npc_idlist, sizeof(m_npc_idlist));
}

bool WorldEventConfig::GetNpcIDIndex(int npc_id, int *locate_index)
{
	if (NULL == locate_index)
	{
		return false;
	}

	for (int i = 0; i < WORLD_EVENT_NPC_TYPE_MAX_COUNT; ++ i)
	{
		if (npc_id == m_npc_idlist[i])
		{
			*locate_index = i;
			return true;
		}
	}

	return false;
}

const WorldEventObjConfig::ConfigItem * WorldEventConfig::GetWorldEventObjCfg(int world_event_obj_id)
{
	for (int i = 0; i < m_world_event_obj_cfg.cfg_count; ++ i)
	{
		if (m_world_event_obj_cfg.cfg_list[i].world_event_id == world_event_obj_id)
		{
			return &m_world_event_obj_cfg.cfg_list[i];
		}
	}

	return NULL;
}

WorldEventObj * WorldEventConfig::CreateWorldEventObj(int world_event_id, Scene *scene, const Posi &posi)
{
	if (NULL == scene)
	{
		return NULL;
	}

	const WorldEventObjConfig::ConfigItem *world_event_obj_cfg = this->GetWorldEventObjCfg(world_event_id);
	if (NULL == world_event_obj_cfg)
	{
		return NULL;
	}

	WorldEventObj *world_event_obj = new WorldEventObj();

	time_t disappear_time = 0;
	if (world_event_obj_cfg->last_time_s > 0)
	{
		disappear_time = EngineAdapter::Instance().Time() + world_event_obj_cfg->last_time_s;
	}

	world_event_obj->Init(world_event_obj_cfg->world_event_id, world_event_obj_cfg->obj_type, world_event_obj_cfg->param, 
		world_event_obj_cfg->max_hp, world_event_obj_cfg->touch_distance, disappear_time);
	world_event_obj->SetPos(posi);
	scene->AddObj(world_event_obj);

	return world_event_obj;
}

const WorldEventLightWordConfig::ConfigItem * WorldEventConfig::GetLightWordCfg(int word_seq)
{
	if (word_seq >= 0 && word_seq < m_light_word_cfg.cfg_count)
	{
		return &m_light_word_cfg.cfg_list[word_seq];
	}

	return NULL;
}

Posi WorldEventConfig::GetTiannvshanhuaMonsterMovePos(int monster_idx, int target_move_pos_idx)
{
	if (monster_idx >= 0 && monster_idx < WORLD_EVENT_TIANNVSHANHUA_MONSTER_COUNT && 
		target_move_pos_idx >= 0 && target_move_pos_idx < WORLD_EVENT_TIANNVSHANHUA_MOVE_POINT_MAX_COUNT)
	{
		return m_tiannvshanhua_move_cfg.cfg_list[monster_idx].pos_list[target_move_pos_idx];
	}

	return Posi(0, 0);
}

const WorldEventTypeConfig::ConfigItem * WorldEventConfig::GetEventTypeCfg(int event_type)
{
	if (event_type > WORLD_EVENT_TYPE_INVALID && event_type < WORLD_EVENT_TYPE_MAX)
	{
		return &m_event_type_cfg.cfg_list[event_type];
	}

	return NULL;
}

const WorldEventRewardConfig::ConfigItem * WorldEventConfig::GetRewardCfg(int event_type)
{
	if (event_type > WORLD_EVENT_TYPE_INVALID && event_type < WORLD_EVENT_TYPE_MAX)
	{
		return &m_reward_config.cfg_list[event_type];
	}

	return NULL;
}

bool WorldEventConfig::RandFindNpcId(short find_npc_flag, char *find_npc_id)
{
// 	UNSTD_STATIC_CHECK(WORLD_EVENT_NPC_TYPE_MAX_COUNT <= (int)sizeof(find_npc_flag) * 8);
// 
// 	if (NULL == find_npc_id)
// 	{
// 		return false;
// 	}
// 
// 	*find_npc_id = 0;
// 
// 	int can_find_npc_count = 0;
// 	int can_find_npc_idlist[WORLD_EVENT_NPC_TYPE_MAX_COUNT]; memset(can_find_npc_idlist, 0, sizeof(can_find_npc_idlist));
// 
// 	for (int i = 0; i < WORLD_EVENT_NPC_TYPE_MAX_COUNT && can_find_npc_count < WORLD_EVENT_NPC_TYPE_MAX_COUNT; ++ i)
// 	{
// 		if (0 == (find_npc_flag & (1 << i)))
// 		{
// 			can_find_npc_idlist[can_find_npc_count] = m_npc_idlist[i];
// 			++ can_find_npc_count;
// 		}
// 	}
// 
// 	if (can_find_npc_count > 0)
// 	{
// 		int rand_idx = RandomNum(can_find_npc_count);
// 
// 		* find_npc_id = can_find_npc_idlist[rand_idx];
// 		return true;
// 	}

	return false;
}

bool WorldEventConfig::IsFindNpcMonster(int monster_id)
{
	if (m_find_npc_monsterid_set.end() != m_find_npc_monsterid_set.find(monster_id))
	{
		return true;
	}

	return false;
}

bool WorldEventConfig::IsKillRandBossEventBoss(int monster_id)
{
	if (m_kill_randomboss_bossid_set.end() != m_kill_randomboss_bossid_set.find(monster_id))
	{
		return true;
	}

	return false;
}

bool WorldEventConfig::IsQuanmintafangEventMonster(int monster_id)
{
	if (m_quanmintafang_monster_id_set.end() != m_quanmintafang_monster_id_set.find(monster_id))
	{
		return true;
	}

	return false;
}

int WorldEventConfig::RandSpecialMonsterAppearance()
{
	if (!m_appearance_monster_set.empty())
	{
		int rand_value = RandomNum((int)m_appearance_monster_set.size());
		std::set<int>::iterator it = m_appearance_monster_set.begin();
		advance(it, rand_value);

		return *it;
	}

	return 0;
}

int WorldEventConfig::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "shenmozhixi_world_event_id", m_other_cfg.shenmozhixi_world_event_id))
	{
		return -1;
	}

	const WorldEventObjConfig::ConfigItem *shenmozhixi_obj_cfg = this->GetWorldEventObjCfg(m_other_cfg.shenmozhixi_world_event_id);
	if (NULL == shenmozhixi_obj_cfg)
	{
		return -2;
	}
	if (WORLD_EVENT_OBJ_SHENMOZHIXI != shenmozhixi_obj_cfg->obj_type)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "tiannnvshanhua_monster_id", m_other_cfg.tiannnvshanhua_monster_id) || m_other_cfg.tiannnvshanhua_monster_id <= 0)
	{
		return -4;
	}

	const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(m_other_cfg.tiannnvshanhua_monster_id);
	if (NULL == monster_param)
	{
		return -5;
	}

	if (MonsterInitParam::AITYPE_FOOL != monster_param->ai_type)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "tiannvshanhua_scene_id", m_other_cfg.tiannvshanhua_scene_id) || m_other_cfg.tiannvshanhua_scene_id <= 0)
	{
		return -7;
	}

	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.tiannvshanhua_scene_id);
	
	if (!PugiGetSubNodeValue(dataElement, "jiqingpk__monster_id", m_jiqing_pk_cfg.monster_id) || m_jiqing_pk_cfg.monster_id <= 0 || 
		!MONSTERPOOL->IsMonsterExist(m_jiqing_pk_cfg.monster_id))
	{
		return -8;
	}
	
	if (!PugiGetSubNodeValue(dataElement, "xukongzhimeng_door_event_id", m_xukongzhimeng_cfg.door_event_id ) || m_xukongzhimeng_cfg.door_event_id  <= 0)
	{
		return -9;
	}

	m_quanmingtafang_cfg.door_event_id = m_xukongzhimeng_cfg.door_event_id;

	const WorldEventObjConfig::ConfigItem *dorr_obj_cfg = this->GetWorldEventObjCfg(m_xukongzhimeng_cfg.door_event_id);
	if (NULL == dorr_obj_cfg)
	{
		return -10;
	}
	if (WORLD_EVENT_OBJ_XUKONGZHIMENG != dorr_obj_cfg->obj_type)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(dataElement, "xukongzhimeng_flush_monster_interval", m_xukongzhimeng_cfg.flush_monster_interval) || m_xukongzhimeng_cfg.flush_monster_interval <= 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(dataElement, "quanmintafang_tower_event_id", m_quanmingtafang_cfg.tower_event_id) || m_quanmingtafang_cfg.tower_event_id <= 0)
	{
		return -13;
	}

	const WorldEventObjConfig::ConfigItem *tower_obj_cfg = this->GetWorldEventObjCfg(m_quanmingtafang_cfg.tower_event_id);
	if (NULL == tower_obj_cfg)
	{
		return -14;
	}
	if (WORLD_EVENT_OBJ_TOWER != tower_obj_cfg->obj_type)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(dataElement, "quanmintafang_flush_monster_interval", m_quanmingtafang_cfg.flush_monster_interval) || m_quanmingtafang_cfg.flush_monster_interval <= 0)
	{
		return -16;
	}

	if (!PugiGetSubNodeValue(dataElement, "quanmintafang_kill_monster_score", m_quanmingtafang_cfg.kill_monster_score) || m_quanmingtafang_cfg.kill_monster_score <= 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(dataElement, "quanmintafang_tower_addscore", m_quanmingtafang_cfg.tower_addscore) || m_quanmingtafang_cfg.tower_addscore <= 0)
	{
		return -18;
	}

	if (!PugiGetSubNodeValue(dataElement, "quanmintafang_tower_addscore_interval", m_quanmingtafang_cfg.tower_addscore_interval) || m_quanmingtafang_cfg.tower_addscore_interval <= 0)
	{
		return -19;
	}


	if (!PugiGetSubNodeValue(dataElement, "quanmintafang_tower_addscore_range", m_quanmingtafang_cfg.tower_addscore_range) ||  m_quanmingtafang_cfg.tower_addscore_range <= 0)
	{
		return -20;
	}

	if (!PugiGetSubNodeValue(dataElement, "quanmintafang_monster_disapper_hp", m_quanmingtafang_cfg.monster_disapper_hp) ||  m_quanmingtafang_cfg.monster_disapper_hp <= 0)
	{
		return -201;
	}

	if (!PugiGetSubNodeValue(dataElement, "randomboss_flush_num", m_kill_randomboss_cfg.flush_num) ||  m_kill_randomboss_cfg.flush_num <= 0)
	{
		return -21;
	}

	if (!PugiGetSubNodeValue(dataElement, "shenmozhixi_fb_scene_id", m_other_cfg.shenmozhixi_fb_scene_id) || m_other_cfg.shenmozhixi_fb_scene_id <= 0)
	{
		return -222;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.shenmozhixi_fb_scene_id);

	if (!PugiGetSubNodeValue(dataElement, "shenmozhixi_fb_birth_pos_x", m_other_cfg.shenmozhixi_fb_birth_pos.x) ||  m_other_cfg.shenmozhixi_fb_birth_pos.x <= 0)
	{
		return -22;
	}
	
	if (!PugiGetSubNodeValue(dataElement, "shenmozhixi_fb_birth_pos_y", m_other_cfg.shenmozhixi_fb_birth_pos.y) ||  m_other_cfg.shenmozhixi_fb_birth_pos.y <= 0)
	{
		return -23;
	}
	
	if (!PugiGetSubNodeValue(dataElement, "tiannvshanhua_drop_range", m_other_cfg.tiannvshanhua_drop_range) ||  m_other_cfg.tiannvshanhua_drop_range <= 0)
	{
		return -24;
	}

	return 0;
}

int WorldEventConfig::InitWorldEventObjCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_world_event_obj_cfg.cfg_count = 0;

	int last_world_event_id = 0;

	while (!dataElement.empty())
	{
		if (m_world_event_obj_cfg.cfg_count >= WORLD_EVENT_OBJ_TYPE_MAX_COUNT)
		{
			return -1;
		}

		WorldEventObjConfig::ConfigItem &cfg_item = m_world_event_obj_cfg.cfg_list[m_world_event_obj_cfg.cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "world_event_id", cfg_item.world_event_id) || cfg_item.world_event_id != last_world_event_id + 1)
		{
			return -1;
		}
		last_world_event_id = cfg_item.world_event_id;

		if (!PugiGetSubNodeValue(dataElement, "obj_type", cfg_item.obj_type) || cfg_item.obj_type <= WORLD_EVENT_OBJ_INVALID || cfg_item.obj_type >= WORLD_EVENT_OBJ_MAX)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "event_param", cfg_item.param) || cfg_item.param < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "last_time_s", cfg_item.last_time_s) || cfg_item.last_time_s < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "max_hp", cfg_item.max_hp) || cfg_item.max_hp <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "touch_distance", cfg_item.touch_distance) || cfg_item.touch_distance < 0)
		{
			return -6;
		}

		switch (cfg_item.obj_type)
		{
		case WORLD_EVENT_OBJ_NPC:
			{
				if (!MONSTERPOOL->IsMonsterExist(cfg_item.param))
				{
					return -1001;
				}
			}
			break;
		}

		++ m_world_event_obj_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int WorldEventConfig::InitOpenCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_timestamp_list[WORLD_EVENT_TYPE_MAX];

	for (int i = 0; i < WORLD_EVENT_TYPE_MAX; ++ i)
	{
		last_timestamp_list[i] = ROLE_RESET_MODULE_DATA_TM_HOUR * 100;
	}

	while (!dataElement.empty())
	{
		int event_type = 0;

		if (!PugiGetSubNodeValue(dataElement, "event_type", event_type) || event_type <= WORLD_EVENT_TYPE_INVALID || event_type >= WORLD_EVENT_TYPE_MAX)
		{
			return -1;
		}

		WorldEventLogicOpenConfig &logic_open_cfg = m_logic_open_cfglist[event_type];

		if (logic_open_cfg.cfg_count >= WORLD_EVENT_LOGIC_OPEN_MAX_TIMES_PER_DAY)
		{
			return -2;
		}

		int &last_timestamp = last_timestamp_list[event_type];
		WorldEventLogicOpenConfig::ConfigItem &cfg_item = logic_open_cfg.cfg_list[logic_open_cfg.cfg_count];

		int tmp_timestamp = 0;
		if (!PugiGetSubNodeValue(dataElement, "invalid_timestamp", tmp_timestamp) || tmp_timestamp < last_timestamp)
		{
			return -3;
		}
		last_timestamp = tmp_timestamp;

		if (!this->TransformLogicStateTime(tmp_timestamp, &cfg_item.logic_state_time[WORLD_EVENT_LOGIC_TYPE_INVALID]))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "standby_timestamp", tmp_timestamp) || tmp_timestamp < last_timestamp)
		{
			return -5;
		}
		last_timestamp = tmp_timestamp;

		if (!this->TransformLogicStateTime(tmp_timestamp, &cfg_item.logic_state_time[WORLD_EVENT_LOGIC_TYPE_STAND]))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "start_timestamp", tmp_timestamp) || tmp_timestamp < last_timestamp)
		{
			return -7;
		}
		last_timestamp = tmp_timestamp;

		if (!this->TransformLogicStateTime(tmp_timestamp, &cfg_item.logic_state_time[WORLD_EVENT_LOGIC_TYPE_START]))
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(dataElement, "stop_timestamp", tmp_timestamp) || tmp_timestamp < last_timestamp)
		{
			return -9;
		}
		last_timestamp = tmp_timestamp;

		if (!this->TransformLogicStateTime(tmp_timestamp, &cfg_item.logic_state_time[WORLD_EVENT_LOGIC_TYPE_STOP]))
		{
			return -10;
		}

		++ logic_open_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	for (int i = WORLD_EVENT_TYPE_INVALID + 1; i < WORLD_EVENT_TYPE_MAX; ++ i)
	{
		if (m_logic_open_cfglist[i].cfg_count <= 0)
		{
			return -20000;
		}
	}

	return 0;
}

int WorldEventConfig::InitNpcPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	std::set<long long> repeat_checker;

	while (!dataElement.empty())
	{
		int scene_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "scene_id", scene_id) || scene_id <= 0)
		{
			return -1;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(scene_id);

		int pos_x = 0;
		if (!PugiGetSubNodeValue(dataElement, "pos_x", pos_x) || pos_x <= 0)
		{
			return -2;
		}

		int pos_y = 0;
		if (!PugiGetSubNodeValue(dataElement, "pos_y", pos_y) || pos_y <= 0)
		{
			return -3;
		}

		long long key_value = ConvertParamToLongLong(scene_id, static_cast<short>(pos_x), static_cast<short>(pos_y));

		if (repeat_checker.find(key_value) != repeat_checker.end())
		{
			return -4;
		}
		repeat_checker.insert(key_value);

		int scene_cfg_index = -1; 
		{
			int free_slot = -1;

			for (int i = 0; i < WORLD_EVENT_NPC_LOCATE_SCENE_MAX_COUNT; ++ i)
			{
				if (0 == m_npc_pos_cfg.scene_list[i].scene_id && free_slot < 0)
				{
					free_slot = i;
				}

				if (scene_id == m_npc_pos_cfg.scene_list[i].scene_id)
				{
					scene_cfg_index = i;
					break;
				}
			}
			if (scene_cfg_index < 0 || scene_cfg_index >= WORLD_EVENT_NPC_LOCATE_SCENE_MAX_COUNT)
			{
				if (free_slot < 0 || free_slot >= WORLD_EVENT_NPC_LOCATE_SCENE_MAX_COUNT)
				{
					return -5;
				}

				scene_cfg_index = free_slot;
			}
		}

		if (m_npc_pos_cfg.scene_list[scene_cfg_index].pos_count >= WORLD_EVENT_POS_CONFIG_MAX_COUNT_PER_SCENE)
		{
			return -6;
		}

		Posi &pos = m_npc_pos_cfg.scene_list[scene_cfg_index].pos_list[m_npc_pos_cfg.scene_list[scene_cfg_index].pos_count];
		pos.x = pos_x;
		pos.y = pos_y;

		m_npc_pos_cfg.scene_list[scene_cfg_index].scene_id = scene_id;
		
		++ m_npc_pos_cfg.scene_list[scene_cfg_index].pos_count;
		dataElement = dataElement.next_sibling();
	}

	for (int i = 0; i < WORLD_EVENT_NPC_LOCATE_SCENE_MAX_COUNT; ++ i)
	{
		if (0 == m_npc_pos_cfg.scene_list[i].scene_id)
		{
			return -20000;
		}

		if (m_npc_pos_cfg.scene_list[i].pos_count <= WORLD_EVENT_NPC_COUNT_PER_SCENE)
		{
			 return -20001;
		}
	}

	return 0;
}

int WorldEventConfig::InitNpcObjCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int npc_id_count = 0;

	while (!dataElement.empty())
	{
		if (npc_id_count >= WORLD_EVENT_NPC_TYPE_MAX_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "world_event_npc_id", m_npc_idlist[npc_id_count]) || m_npc_idlist[npc_id_count] < 0)
		{
			return -2;
		}

		const WorldEventObjConfig::ConfigItem *npc_obj_cfg = this->GetWorldEventObjCfg(m_npc_idlist[npc_id_count]);
		if (NULL == npc_obj_cfg)
		{
			return -3;
		}

		if (WORLD_EVENT_OBJ_NPC != npc_obj_cfg->obj_type)
		{
			return -4;
		}

		m_find_npc_monsterid_set.insert(npc_obj_cfg->param);

		++ npc_id_count;
		dataElement = dataElement.next_sibling();
	}

	if (WORLD_EVENT_NPC_TYPE_MAX_COUNT != npc_id_count)
	{
		return -20000;
	}

	return 0;
}

int WorldEventConfig::InitWorldEventTypeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_event_type = WORLD_EVENT_TYPE_INVALID;

	while (!dataElement.empty())
	{
		int event_type = 0;
		if (!PugiGetSubNodeValue(dataElement, "event_type", event_type) || event_type >= WORLD_EVENT_TYPE_MAX || event_type != last_event_type + 1)
		{
			return -1;
		}
		last_event_type = event_type;

		WorldEventTypeConfig::ConfigItem &cfg_item = m_event_type_cfg.cfg_list[event_type];

		if (!PugiGetSubNodeValue(dataElement, "complete_event_score", cfg_item.complete_event_score) || cfg_item.complete_event_score < 0)
		{
			return -21;
		}

		bool valid_reward_type_count = 0;

		if (!PugiGetSubNodeValue(dataElement, "base_coin", cfg_item.base_coin) || cfg_item.base_coin < 0)
		{
			return -4;
		}
		if (cfg_item.base_coin > 0)
		{
			valid_reward_type_count = true;
		}

		if (!PugiGetSubNodeValue(dataElement, "base_xianhun", cfg_item.base_xianhun) || cfg_item.base_xianhun < 0)
		{
			return -5;
		}
		if (cfg_item.base_xianhun > 0)
		{
			valid_reward_type_count = true;
		}

		if (!PugiGetSubNodeValue(dataElement, "base_zhenqi", cfg_item.base_zhenqi) || cfg_item.base_zhenqi < 0)
		{
			return -6;
		}
		if (cfg_item.base_zhenqi > 0)
		{
			valid_reward_type_count = true;
		}

		PugiXmlNode baseItemElement = dataElement.child("reward_item");
		if (baseItemElement.empty())
		{
			return -7;
		}

		if (cfg_item.base_item.ReadConfig(baseItemElement))
		{
			valid_reward_type_count = true;

			const ItemBase *item_base = ITEMPOOL->GetItem(cfg_item.base_item.item_id);
			if (NULL == item_base)
			{
				return -8;
			}
		}
		else
		{
			cfg_item.base_item.item_id = 0;
		}

		if (true != valid_reward_type_count)
		{
			return -10;
		}

		dataElement = dataElement.next_sibling();
	}

	if (WORLD_EVENT_TYPE_MAX - 1 != last_event_type)
	{
		return -20000;
	}

	return 0;
}

int WorldEventConfig::InitShenmozhijiPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_shenmozhiji_pos_cfg.pos_item_count = 0;

	std::set<long long> repeat_checker;

	while (!dataElement.empty())
	{
		if (m_shenmozhiji_pos_cfg.pos_item_count >= WORLD_EVENT_POS_CONFIG_MAX_COUNT)
		{
			return -1;
		}

		WorldEventPosItem &pos_item = m_shenmozhiji_pos_cfg.pos_item_list[m_shenmozhiji_pos_cfg.pos_item_count];

		if (!PugiGetSubNodeValue(dataElement, "scene_id", pos_item.scene_id) || pos_item.scene_id <= 0)
		{
			return -2;
		}
		CheckResourceCenter::Instance().GetSceneCheck()->Add(pos_item.scene_id);

		if (!PugiGetSubNodeValue(dataElement, "pos_x", pos_item.pos.x) || pos_item.pos.x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", pos_item.pos.y) || pos_item.pos.y <= 0)
		{
			return -4;
		}

		long long key_value = ConvertParamToLongLong(pos_item.scene_id, static_cast<short>(pos_item.pos.x), static_cast<short>(pos_item.pos.y));

		if (repeat_checker.find(key_value) != repeat_checker.end())
		{
			return -4;
		}
		repeat_checker.insert(key_value);

		++ m_shenmozhiji_pos_cfg.pos_item_count;
		dataElement = dataElement.next_sibling();
	}

	if (m_shenmozhiji_pos_cfg.pos_item_count <= WORLD_EVENT_SHENMOZHIXI_PORTAL_MAX_COUNT)
	{
		return -20000;
	}

	return 0;
}

int WorldEventConfig::InitTiannvsanhuaFallingItemCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_tiannvshanhua_falling_item_cfg.cfg_count = 0;

	while (!dataElement.empty())
	{
		if (m_tiannvshanhua_falling_item_cfg.cfg_count >= WORLD_EVENT_TIANNVSHANHUA_FALLING_ITEM_MAX_COUNT)
		{
			return -1;
		}

		WorldEventFallingItemConfig::ConfigItem &cfg_item = m_tiannvshanhua_falling_item_cfg.cfg_list[m_tiannvshanhua_falling_item_cfg.cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "bind_gold", cfg_item.bind_gold) || cfg_item.bind_gold <= 0)
		{
			return -2;
		}

		++ m_tiannvshanhua_falling_item_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int WorldEventConfig::InitTiannvsanhuaMoveCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int pos_count_list[WORLD_EVENT_TIANNVSHANHUA_MONSTER_COUNT]; memset(pos_count_list, 0, sizeof(pos_count_list));

	while (!dataElement.empty())
	{
		int monster_idx = 0;
		if (!PugiGetSubNodeValue(dataElement, "monster_idx", monster_idx) || monster_idx < 0 || monster_idx >= WORLD_EVENT_TIANNVSHANHUA_MONSTER_COUNT)
		{
			return -1;
		}

		int &pos_count = pos_count_list[monster_idx];
		if (pos_count >= WORLD_EVENT_TIANNVSHANHUA_MOVE_POINT_MAX_COUNT)
		{
			return -2;
		}

		Posi &cfg_pos = m_tiannvshanhua_move_cfg.cfg_list[monster_idx].pos_list[pos_count];

		if (!PugiGetSubNodeValue(dataElement, "pos_x", cfg_pos.x) || cfg_pos.x <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", cfg_pos.y) || cfg_pos.y <= 0)
		{
			return -4;
		}

		++ pos_count;
		dataElement = dataElement.next_sibling();
	}

	for (int i = 0; i < WORLD_EVENT_TIANNVSHANHUA_MONSTER_COUNT; ++ i)
	{
		if (WORLD_EVENT_TIANNVSHANHUA_MOVE_POINT_MAX_COUNT != pos_count_list[i])
		{
			return -30000;
		}
	}

	return 0;
}

int WorldEventConfig::InitLightWordCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_light_word_cfg.cfg_count = 0;

	while (!dataElement.empty())
	{
		int word_seq = 0;
		if (!PugiGetSubNodeValue(dataElement, "word_seq", word_seq) || word_seq >= WORLD_EVENT_TITLE_WORD_MAX_NUM || word_seq != m_light_word_cfg.cfg_count)
		{
			return -1;
		}

		WorldEventLightWordConfig::ConfigItem &cfg_item = m_light_word_cfg.cfg_list[word_seq];
		cfg_item.word_seq = word_seq;

		if (!PugiGetSubNodeValue(dataElement, "stuff_id", cfg_item.stuff_id) || cfg_item.stuff_id <= 0 || NULL == ITEMPOOL->GetItem(cfg_item.stuff_id))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_num", cfg_item.stuff_count) || cfg_item.stuff_count <= 0)
		{
			return -5;
		}

		++ m_light_word_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int WorldEventConfig::InitAppearanceCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int mosnter_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "monster_id", mosnter_id) || !MONSTERPOOL->IsMonsterExist(mosnter_id))
		{
			return -1;
		}

		m_appearance_monster_set.insert(mosnter_id);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

bool WorldEventConfig::TransformLogicStateTime(int time_value, WorldEventLogicOpenConfig::LogicStateTime  *logic_state_time)
{
	if (NULL == logic_state_time)
	{
		return false;
	}

	int hour = time_value / 100;
	int minute = time_value % 100;

	if (hour < 0 || hour >= 24)
	{
		return false;
	}

	if (minute < 0 || minute >= 60)
	{
		return false;
	}

	logic_state_time->hour = hour;
	logic_state_time->minute = minute;

	return true;
}

int WorldEventConfig::InitKillRandomBossConfig(PugiXmlNode RootElement)
{
	{
		// 读取随机boss的刷新位置 
		PugiXmlNode rootElement = RootElement.child("randomboss_pos_cfg");
		if (rootElement.empty())
		{
			return -9000;
		}

		PugiXmlNode dataElement = rootElement.child("data");
		if (dataElement.empty())
		{
			return -10000;
		}

		std::set<long long> repeat_checker;

		while (!dataElement.empty())
		{
			int scene_id = 0;
			if (!PugiGetSubNodeValue(dataElement, "scene_id", scene_id) || scene_id <= 0)
			{
				return -1;
			}
			CheckResourceCenter::Instance().GetSceneCheck()->Add(scene_id);

			int pos_x = 0;
			if (!PugiGetSubNodeValue(dataElement, "pos_x", pos_x) || pos_x <= 0)
			{
				return -2;
			}

			int pos_y = 0;
			if (!PugiGetSubNodeValue(dataElement, "pos_y", pos_y) || pos_y <= 0)
			{
				return -3;
			}

			{
				union 
				{
					long long ll_value;
					struct  
					{
						int scene_id;
						short pos_x;
						short pos_y;
					};
				} pos_key;

				pos_key.scene_id = scene_id;
				pos_key.pos_x = static_cast<short>(pos_x);
				pos_key.pos_y = static_cast<short>(pos_y);

				if (repeat_checker.find(pos_key.ll_value) != repeat_checker.end())
				{
					return -4;
				}
				repeat_checker.insert(pos_key.ll_value);
			}

			if (m_kill_randomboss_cfg.pos_count >= WORLD_EVENT_RANDOM_BOSS_POS_COUNT)
			{
				return -5;
			}

			m_kill_randomboss_cfg.pos_cfg_list[m_kill_randomboss_cfg.pos_count].scene_id = scene_id;
			m_kill_randomboss_cfg.pos_cfg_list[m_kill_randomboss_cfg.pos_count].pos.x = pos_x;
			m_kill_randomboss_cfg.pos_cfg_list[m_kill_randomboss_cfg.pos_count].pos.y = pos_y;

			++ m_kill_randomboss_cfg.pos_count;
			
			dataElement = dataElement.next_sibling();
		}
	}

	{
		// 读取随机boss的ID
		PugiXmlNode rootElement = RootElement.child("randomboss_id_cfg");
		if (rootElement.empty())
		{
			return -90001;
		}

		PugiXmlNode dataElement = rootElement.child("data");
		if (dataElement.empty())
		{
			return -100001;
		}

		while (!dataElement.empty())
		{
			int boss_id = 0;
			if (!PugiGetSubNodeValue(dataElement, "boss_id", boss_id) || boss_id <= 0)
			{
				return -11;
			}
			const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(boss_id);
			if (NULL == monster_param)
			{
				return -21;
			}

			if (monster_param->disappear_time <= 0)
			{
				return -22;
			}

			if (m_kill_randomboss_cfg.boss_id_count >= WORLD_EVENT_RANDOM_BOSS_TYPE_MAX)
			{
				return -51;
			}

			m_kill_randomboss_cfg.boss_id_list[m_kill_randomboss_cfg.boss_id_count] = boss_id;
			++ m_kill_randomboss_cfg.boss_id_count;

			m_kill_randomboss_bossid_set.insert(boss_id);

			dataElement = dataElement.next_sibling();
		}
	}

	return 0;
}

int WorldEventConfig::InitJiqingPKConfig(PugiXmlNode RootElement)
{
	{
		// 读取激情PK刷新位置 
		PugiXmlNode rootElement = RootElement.child("jiqingpk_pos_cfg");
		if (rootElement.empty())
		{
			return -9000;
		}

		PugiXmlNode dataElement = rootElement.child("data");
		if (dataElement.empty())
		{
			return -10000;
		}

		std::set<long long> repeat_checker;

		while (!dataElement.empty())
		{
			int scene_id = 0;
			if (!PugiGetSubNodeValue(dataElement, "scene_id", scene_id) || scene_id <= 0)
			{
				return -1;
			}
			CheckResourceCenter::Instance().GetSceneCheck()->Add(scene_id);

			int pos_x = 0;
			if (!PugiGetSubNodeValue(dataElement, "pos_x", pos_x) || pos_x <= 0)
			{
				return -2;
			}

			int pos_y = 0;
			if (!PugiGetSubNodeValue(dataElement, "pos_y", pos_y) || pos_y <= 0)
			{
				return -3;
			}

			{
				union 
				{
					long long ll_value;
					struct  
					{
						int scene_id;
						short pos_x;
						short pos_y;
					};
				} pos_key;

				pos_key.scene_id = scene_id;
				pos_key.pos_x = static_cast<short>(pos_x);
				pos_key.pos_y = static_cast<short>(pos_y);

				if (repeat_checker.find(pos_key.ll_value) != repeat_checker.end())
				{
					return -4;
				}
				repeat_checker.insert(pos_key.ll_value);
			}

			if (m_jiqing_pk_cfg.pos_count >= WORLD_EVENT_PKROLE_POS_COUNT)
			{
				return -5;
			}

			m_jiqing_pk_cfg.pos_cfg_list[m_jiqing_pk_cfg.pos_count].scene_id = scene_id;
			m_jiqing_pk_cfg.pos_cfg_list[m_jiqing_pk_cfg.pos_count].pos.x = pos_x;
			m_jiqing_pk_cfg.pos_cfg_list[m_jiqing_pk_cfg.pos_count].pos.y = pos_y;

			++ m_jiqing_pk_cfg.pos_count;

			dataElement = dataElement.next_sibling();
		}
	}

	return 0;
}

int WorldEventConfig::InitXukongzhimengConfig(PugiXmlNode RootElement)
{
	{
		// 读取虚空之门怪物的ID
		PugiXmlNode rootElement = RootElement.child("xukongzhimeng_monsterid_cfg");
		if (rootElement.empty())
		{
			return -90001;
		}

		PugiXmlNode dataElement = rootElement.child("data");
		if (dataElement.empty())
		{
			return -100001;
		}

		while (!dataElement.empty())
		{
			int monster_id = 0;
			if (!PugiGetSubNodeValue(dataElement, "monster_id", monster_id) || monster_id <= 0)
			{
				return -11;
			}

			const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(monster_id);
			if (NULL == monster_param)
			{
				return -12;
			}

			if (monster_param->disappear_time <= 0)
			{
				return -13;
			}

			if (m_xukongzhimeng_cfg.monster_idcount >= WORLD_EVENT_XUKONGZHIMENG_MONSTER_MAX)
			{
				return -51;
			}

			m_xukongzhimeng_cfg.monster_idlist[m_xukongzhimeng_cfg.monster_idcount] = monster_id;
			++ m_xukongzhimeng_cfg.monster_idcount;

			dataElement = dataElement.next_sibling();
		}
	}

	{
		// 读取虚空之门配置
		PugiXmlNode rootElement = RootElement.child("xukongzhimeng_cfg");
		if (rootElement.empty())
		{
			return -90001;
		}

		PugiXmlNode dataElement = rootElement.child("data");
		if (dataElement.empty())
		{
			return -100001;
		}

		std::set<long long> repeat_checker_startpos;

		int last_door_idx = -1;
		Posi last_start_pos(-1, -1);

		while (!dataElement.empty())
		{
			int door_idx = 0;
			if (!PugiGetSubNodeValue(dataElement, "door_idx", door_idx) || door_idx < 0 || door_idx >= WORLD_EVENT_XUKONGZHIMENG_DOOR_MAX)
			{
				return -21;
			}

			int scene_id = 0;
			if (!PugiGetSubNodeValue(dataElement, "scene_id", scene_id) || scene_id <= 0)
			{
				return -31;
			}
			CheckResourceCenter::Instance().GetSceneCheck()->Add(scene_id);

			int start_pos_x = 0;
			if (!PugiGetSubNodeValue(dataElement, "start_pos_x", start_pos_x) || start_pos_x <= 0)
			{
				return -41;
			}

			int start_pos_y = 0;
			if (!PugiGetSubNodeValue(dataElement, "start_pos_y", start_pos_y) || start_pos_y <= 0)
			{
				return -51;
			}

			{
				if (-1 == last_door_idx || last_door_idx != door_idx) //  不同door_idx的startpos不能一样
				{
					union 
					{
						long long ll_value;
						struct  
						{
							int scene_id;
							short pos_x;
							short pos_y;
						};
					} pos_key;

					pos_key.scene_id = scene_id;
					pos_key.pos_x = static_cast<short>(start_pos_x);
					pos_key.pos_y = static_cast<short>(start_pos_y);

					if (repeat_checker_startpos.find(pos_key.ll_value) != repeat_checker_startpos.end())
					{
						return -61;
					}
					repeat_checker_startpos.insert(pos_key.ll_value);

					last_door_idx = door_idx;
					last_start_pos = Posi(start_pos_x, start_pos_y);

					++ m_xukongzhimeng_cfg.doorcfg_count;
				}
				else if (last_start_pos != Posi(start_pos_x, start_pos_y)) // 同一个door start点要一样 
				{
					return -71;
				}
			}

			int disapper_pos_x = 0;
			if (!PugiGetSubNodeValue(dataElement, "disapper_pos_x", disapper_pos_x) || disapper_pos_x <= 0)
			{
				return -81;
			}

			int disapper_pos_y = 0;
			if (!PugiGetSubNodeValue(dataElement, "disapper_pos_y", disapper_pos_y) || disapper_pos_y <= 0)
			{
				return -91;
			}

			XukongzhimengConfig::DoorCfgItem &door_cfg = m_xukongzhimeng_cfg.doorcfg_list[door_idx];

			door_cfg.scene_id = scene_id;
			door_cfg.start_pos = Posi(start_pos_x, start_pos_y);
			
			if (door_cfg.disapper_count >= WORLD_EVENT_XUKONGZHIMENG_DISAPPER_POS_MAX)
			{
				return -101;
			}

			door_cfg.disapper_pos_list[door_cfg.disapper_count] = Posi(disapper_pos_x, disapper_pos_y);

			++ door_cfg.disapper_count;
			dataElement = dataElement.next_sibling();
		}
	}

	return 0;
}

int WorldEventConfig::InitQuanmintafangConfig(PugiXmlNode RootElement)
{
	{
		// 全民塔防怪物的ID
		PugiXmlNode rootElement = RootElement.child("quanmintafang_monsterid_cfg");
		if (rootElement.empty())
		{
			return -90001;
		}

		PugiXmlNode dataElement = rootElement.child("data");
		if (dataElement.empty())
		{
			return -100001;
		}

		while (!dataElement.empty())
		{
			int monster_id = 0;
			if (!PugiGetSubNodeValue(dataElement, "monster_id", monster_id) || monster_id <= 0)
			{
				return -11;
			}

			const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(monster_id);
			if (NULL == monster_param)
			{
				return -12;
			}

			if (monster_param->disappear_time <= 0)
			{
				return -13;
			}

			if (m_quanmingtafang_cfg.monster_idcount >= WORLD_EVENT_QUANMINGTAFANG_MONSTER_MAX)
			{
				return -51;
			}

			m_quanmintafang_monster_id_set.insert(monster_id);
			m_quanmingtafang_cfg.monster_idlist[m_quanmingtafang_cfg.monster_idcount] = monster_id;
			++ m_quanmingtafang_cfg.monster_idcount;

			dataElement = dataElement.next_sibling();
		}
	}

	{
		// 读取全民塔防配置
		PugiXmlNode rootElement = RootElement.child("quanmintafang_cfg");
		if (rootElement.empty())
		{
			return -90001;
		}

		PugiXmlNode dataElement = rootElement.child("data");
		if (dataElement.empty())
		{
			return -100001;
		}

		std::set<long long> repeat_checker_disapperpos;

		int last_tower_idx = -1;
		Posi last_disapper_pos(-1, -1);

		while (!dataElement.empty())
		{
			int tower_idx = 0;
			if (!PugiGetSubNodeValue(dataElement, "tower_idx", tower_idx) || tower_idx < 0 || tower_idx >= WORLD_EVENT_QUANMINGTAFANG_TOWER_MAX)
			{
				return -21;
			}

			int scene_id = 0;
			if (!PugiGetSubNodeValue(dataElement, "scene_id", scene_id) || scene_id <= 0)
			{
				return -31;
			}
			CheckResourceCenter::Instance().GetSceneCheck()->Add(scene_id);

			int disapper_pos_x = 0;
			if (!PugiGetSubNodeValue(dataElement, "disapper_pos_x", disapper_pos_x) || disapper_pos_x <= 0)
			{
				return -41;
			}

			int disapper_pos_y = 0;
			if (!PugiGetSubNodeValue(dataElement, "disapper_pos_y", disapper_pos_y) || disapper_pos_y <= 0)
			{
				return -51;
			}

			{
				if (-1 == last_tower_idx || last_tower_idx != tower_idx) //  不同tower_idx的disapper_pos不能一样
				{
					union 
					{
						long long ll_value;
						struct  
						{
							int scene_id;
							short pos_x;
							short pos_y;
						};
					} pos_key;

					pos_key.scene_id = scene_id;
					pos_key.pos_x = static_cast<short>(disapper_pos_x);
					pos_key.pos_y = static_cast<short>(disapper_pos_y);

					if (repeat_checker_disapperpos.find(pos_key.ll_value) != repeat_checker_disapperpos.end())
					{
						return -61;
					}
					repeat_checker_disapperpos.insert(pos_key.ll_value);

					last_tower_idx = tower_idx;
					last_disapper_pos = Posi(disapper_pos_x, disapper_pos_y);

					++ m_quanmingtafang_cfg.towercfg_count;
				}
				else if (last_disapper_pos != Posi(disapper_pos_x, disapper_pos_y)) // 同一个tower disapper_pos点要一样 
				{
					return -71;
				}
			}

			int start_pos_x = 0;
			if (!PugiGetSubNodeValue(dataElement, "start_pos_x", start_pos_x) || start_pos_x <= 0)
			{
				return -81;
			}

			int start_pos_y = 0;
			if (!PugiGetSubNodeValue(dataElement, "start_pos_y", start_pos_y) || start_pos_y <= 0)
			{
				return -91;
			}

			QuanmintafangConfig::TowerCfgItem &tower_cfg = m_quanmingtafang_cfg.towercfg_list[tower_idx];

			tower_cfg.scene_id = scene_id;
			tower_cfg.disapper_pos = Posi(disapper_pos_x, disapper_pos_y);

			if (tower_cfg.start_count >= WORLD_EVENT_QUANMINGTAFANG_START_POS_MAX)
			{
				return -101;
			}

			tower_cfg.start_pos_list[tower_cfg.start_count] = Posi(start_pos_x, start_pos_y);

			++ tower_cfg.start_count;
			dataElement = dataElement.next_sibling();
		}
	}

	return 0;
}

int WorldEventConfig::InitRewardConfig(PugiXmlNode RootElement)
{

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_need_score_list[WORLD_EVENT_TYPE_MAX]; memset(last_need_score_list, 0, sizeof(last_need_score_list));
	int reward_grade_count[WORLD_EVENT_TYPE_MAX]; memset(reward_grade_count, 0, sizeof(reward_grade_count));

	while (!dataElement.empty())
	{
		int event_type = 0;
		if (!PugiGetSubNodeValue(dataElement, "event_type", event_type) || event_type <= WORLD_EVENT_TYPE_INVALID || event_type >= WORLD_EVENT_TYPE_MAX)
		{
			return -1;
		}

		WorldEventRewardConfig::ConfigItem &cfg_item = m_reward_config.cfg_list[event_type];

		if (reward_grade_count[event_type] >= WORLD_EVENT_REWARD_GRADE_COUNT)
		{
			return -2;
		}

		int curr_reward_grade = reward_grade_count[event_type];

		if (!PugiGetSubNodeValue(dataElement, "need_score", cfg_item.need_score_list[curr_reward_grade]) || cfg_item.need_score_list[curr_reward_grade] <= last_need_score_list[event_type])
		{
			return -3;
		}
		last_need_score_list[event_type] = cfg_item.need_score_list[curr_reward_grade];

		if (!PugiGetSubNodeValue(dataElement, "reward_factor", cfg_item.reward_factor_list[curr_reward_grade]) || cfg_item.reward_factor_list[curr_reward_grade] <= 0)
		{
			return -4;
		}

		++ reward_grade_count[event_type];
		dataElement = dataElement.next_sibling();
	}

	for (int i = WORLD_EVENT_TYPE_INVALID + 1; i < WORLD_EVENT_TYPE_MAX; ++ i)
	{
		if (reward_grade_count[i] <= 0)
		{
			return -20000;
		}
	}

	return 0;
}




