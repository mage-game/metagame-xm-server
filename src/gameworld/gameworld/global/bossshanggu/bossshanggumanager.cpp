#include "bossshanggumanager.hpp"
#include "obj/character/role.h"
#include "protocal/msgother.h"
#include "engineadapter.h"
#include "internalcomm.h"

#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/string/globalstr.h"
#include "servercommon/commondata.hpp"
#include "world.h"
#include "config/logicconfigmanager.hpp"

#include "scene/changescenedelayer.h"
#include "scene/sceneregister/sceneregister.hpp"
#include "scene/generater/scenemonsterlogic.h"
#include "scene/generater/monstergenerater.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"

#include "global/usercache/usercache.hpp"
#include "global/team/team.hpp"
#include "item/itemextern.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "other/rolegoal/rolegoalconfig.hpp"
#include "other/vip/vipconfig.hpp"
#include "global/bossfamily/bossfamilyconfig.hpp"
#include "monster/monsterpool.h"
#include "item/knapsack.h"
#include "other/shop/shop.hpp"
#include "friend/teaminfo.hpp"
#include "other/vip/vip.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/activedegree/activedegree.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/rolegoal/rolegoal.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "scene/scene.h"
#include "scene/map.h"
#include "global/bossfamily/bossfamilymanager.hpp"

BossShangGuManager::BossShangGuManager()
{
}

BossShangGuManager::~BossShangGuManager()
{

}

BossShangGuManager & BossShangGuManager::Instance()
{
	static BossShangGuManager wbm;
	return wbm;
}

void BossShangGuManager::OnServerStart()
{
	// 系统启动创建所有boss和精英怪、采集物
	if (!LOGIC_CONFIG->GetShangGuBossConfig().IsShangGuBossOpen())
	{
		return;
	}

	this->CreateShangGuBossAllBoss();
	this->CreateShangGuBossGatherItemAndGoldMonster();
}

void BossShangGuManager::OnStopServer()
{

}

void BossShangGuManager::Update(unsigned long interval, time_t now_second)
{
	if (!LOGIC_CONFIG->GetShangGuBossConfig().IsShangGuBossOpen())
	{
		return;
	}

	//上古boss 先删除相关obj再创建
	this->UpdataShangGuBossGatherItemAndGoldMonster(now_second);

	for (std::vector<LevelBossInfo>::iterator level_boss_it = m_shanggu_boss_level_list.begin(); level_boss_it != m_shanggu_boss_level_list.end(); ++level_boss_it)
	{
		this->UpdateBossMap(now_second, level_boss_it->boss_map, level_boss_it->level);
	}
}

void BossShangGuManager::UpdateBossMap(time_t now_second, std::map<int, BossInfo> & boss_map, int level)
{
	for (std::map<int, BossInfo>::iterator boss_info_it = boss_map.begin(); boss_info_it != boss_map.end(); ++boss_info_it)
	{
		if (boss_info_it->second.next_refresh_time <= 0) continue;

		int flush_left_time_s = int(boss_info_it->second.next_refresh_time - now_second);
		if (!boss_info_it->second.has_notice && flush_left_time_s <= SECOND_PER_MIN)
		{
			this->SendBossInfoToAll(boss_info_it->second, Protocol::NOTIFY_REASON_BOSS_ADVANCE, NULL);
			boss_info_it->second.has_notice = true;
		}
		else if (flush_left_time_s <= 0) // 时间到，刷新boss
		{
			int boss_id = 0;
			Posi boss_pos(0, 0);

			const ShangGuBossInfoConfig::ItemConfig *boss_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossInfoCfg(level, boss_info_it->second.boss_id);
			if (NULL == boss_cfg)
			{
				continue;
			}

			boss_id = boss_cfg->boss_id;
			boss_pos = boss_cfg->pos;

			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(boss_info_it->second.scene_id, COMMON_SCENE_KEY);
			if (NULL == scene)
			{
				World::GetInstWorld()->GetSceneManager()->CreateFB(boss_info_it->second.scene_id, COMMON_SCENE_KEY, &scene);
			}

			Monster *monster = MONSTERPOOL->CreateMonster(boss_id, scene, boss_pos);
			if (NULL != monster)
			{
				// 数据初始化
				boss_info_it->second.obj_id = monster->GetId();
				boss_info_it->second.next_refresh_time = 0;
				boss_info_it->second.has_notice = false;

				this->SendBossInfoToAll(boss_info_it->second, Protocol::NOTIFY_REASON_BOSS_REFRESH, NULL);
				// modify 2019-03-28  策划需求去除单个BOSS刷新传闻
				//int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF),
				//	gamestring::g_refresh_one_boss, boss_info_it->second.boss_id, boss_info_it->second.scene_id);
				//if (length > 0)
				//{
				//	World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2, 0, 0);
				//}
			}
		}
	}
}

void BossShangGuManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{

}

void BossShangGuManager::OnAttack(Role *attacker, Monster *monster, long long &delta_hp)
{
	return;
}

void BossShangGuManager::CreateShangGuBossAllBoss()
{
	m_shanggu_boss_level_list.clear();
	const int shanggu_boss_layer_count = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossLayerCount();
	for (int layer = 0; layer < shanggu_boss_layer_count && layer < SHANGGU_BOSS_CFG_MAX_LAYER; ++layer)
	{
		const ShangGuBossLayerConfig *layer_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossLayerCfg(layer);
		if (NULL == layer_cfg) continue;

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(layer_cfg->scene_id, COMMON_SCENE_KEY);
		if (NULL == scene)
		{
			World::GetInstWorld()->GetSceneManager()->CreateFB(layer_cfg->scene_id, COMMON_SCENE_KEY, &scene);
		}

		if (NULL == scene)
		{
			gamelog::g_log_worldboss.printf(LL_ERROR, "[BossShangGuManager::CreateShangGuBossAllBoss] create scene Fail scene_id = %d", layer_cfg->scene_id);
			continue;
		}

		LevelBossInfo boss_map;
		boss_map.level = layer;
		boss_map.scene_id = layer_cfg->scene_id;

		const int boss_count = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossCount(layer);
		for (int i = 0; i < boss_count && i < ShangGuBossInfoConfig::MAX_BOSS_COUNT; ++i)
		{
			const ShangGuBossInfoConfig::ItemConfig *boss_item = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossInfo(layer, i);
			if (NULL == boss_item)
			{
				continue;
			}
			// boss在这里刷
			if (SHANGGU_BOSS_TYPE_BOSS != boss_item->type)
			{
				continue;
			}

			Monster *monster = MONSTERPOOL->CreateMonster(boss_item->boss_id, scene, boss_item->pos);
			if (NULL != monster)
			{
				// 数据初始化
				BossInfo &info = boss_map.boss_map[boss_item->boss_id];
				info.boss_id = boss_item->boss_id;
				info.scene_id = layer_cfg->scene_id;
				info.index = boss_item->index;
				info.obj_id = monster->GetId();
				info.next_refresh_time = 0;

				//////////////////////////////////////////////////////////////////////////////
				if (boss_item->boss_lv_seq > 0 && boss_item->boss_lv_seq <= SHANGGU_BOSS_HIDE_BOSS_MAX_LEVEL_NUM &&
					SHANGGU_BOSS_TYPE_BOSS == boss_item->type)
				{
					ShangGuHideBossInfo hide_info;
					hide_info.boss_lv_seq = boss_item->boss_lv_seq;
					hide_info.is_has_flush = false;

					int real_count = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossLvSeqCount(layer, boss_item->boss_lv_seq);
					if (real_count > 0)
					{
						hide_info.average_weight = ShangGuHideBossInfo::TOTAL_WEIGHT / real_count;
					}

					m_shanggu_hide_info_map[boss_item->boss_lv_seq] = hide_info;
				}
			}
		}

		m_shanggu_boss_level_list.push_back(boss_map);
	}
}

void BossShangGuManager::CreateShangGuBossGatherItemAndGoldMonster()
{
	if (0 == m_shanggu_boss_next_flush_timestamp)
	{
		unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_shanggu_boss_next_flush_timestamp = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossGatherItemNextFlushTime(now_second);
	}

	const int layer_count = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossLayerCount();
	const ShangGuBossOtherCfg &other_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossOtherCfg();
	for (int layer = 0; layer < layer_count && layer < SHANGGU_BOSS_CFG_MAX_LAYER; ++layer)
	{
		const ShangGuBossLayerConfig *layer_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossLayerCfg(layer);
		if (NULL == layer_cfg)
		{
			continue;
		}

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(layer_cfg->scene_id, COMMON_SCENE_KEY);
		if (NULL == scene)
		{
			continue;
		}

		{	// 大宝箱
			Posi pos(-1, -1);
			ObjID gather_id = other_cfg.shanggu_max_box_id;
			const static int gather_time = other_cfg.shanggu_gather_max_box_time_s * 1000;

			int creat_count = 0;
			int loop_times = 0;
			while (creat_count < layer_cfg->max_box_num && loop_times < 1000)
			{
				++loop_times;
				if (GetShangGuBossGatherItemRandomPosition(scene, &pos))
				{
					GatherObj *gather_obj = new GatherObj();
					gather_obj->Init(NULL, gather_id, gather_time, 0, true);
					gather_obj->SetPos(pos);
					scene->AddObj(gather_obj);

					++creat_count;

					{
						ShangGuObjIdInfo info;
						info.layer = layer;
						info.obj_id = gather_obj->GetId();
						info.kind_type = SHANGGU_BOSS_KIND_TYPE_MAX_BOX;

						long long key = ConvertParamToLongLong(info.layer, info.obj_id);
						m_shanggu_boss_obj_id_info_map[key] = info;
					}
				}
			}
		}

		{	// 小宝箱
			Posi pos(-1, -1);
			ObjID gather_id = other_cfg.shanggu_min_box_id;
			const static int gather_time = other_cfg.shanggu_gather_min_box_time_s * 1000;

			int creat_count = 0;
			int loop_times = 0;
			while (creat_count < layer_cfg->min_box_num && loop_times < 1000)
			{
				++loop_times;
				if (GetShangGuBossGatherItemRandomPosition(scene, &pos))
				{
					GatherObj *gather_obj = new GatherObj();
					gather_obj->Init(NULL, gather_id, gather_time, 0, true);
					gather_obj->SetPos(pos);
					scene->AddObj(gather_obj);

					++creat_count;

					{
						ShangGuObjIdInfo info;
						info.layer = layer;
						info.obj_id = gather_obj->GetId();
						info.kind_type = SHANGGU_BOSS_KIND_TYPE_MIN_BOX;

						long long key = ConvertParamToLongLong(info.layer, info.obj_id);
						m_shanggu_boss_obj_id_info_map[key] = info;
					}
				}
			}
		}

		// 黄金怪
		for (int index = 0; index < ShangGuBossInfoConfig::MAX_BOSS_COUNT; ++index)
		{
			const ShangGuBossInfoConfig::ItemConfig *boss_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossInfo(layer, index);
			if (NULL == boss_cfg)
			{
				continue;
			}

			// 只刷黄金怪
			if (SHANGGU_BOSS_TYPE_GOLD_MONSTER == boss_cfg->type)
			{
				Monster *monster = MONSTERPOOL->CreateMonster(boss_cfg->boss_id, scene, boss_cfg->pos);
				if (NULL != monster)
				{
					ShangGuObjIdInfo info;
					info.layer = layer;
					info.obj_id = monster->GetId();
					info.kind_type = SHANGGU_BOSS_KIND_TYPE_GOLD_MONSTER;

					long long key = ConvertParamToLongLong(info.layer, info.obj_id);
					m_shanggu_boss_obj_id_info_map[key] = info;
				}
			}
		}
	}
}

void BossShangGuManager::OnShangGuBossDie(int scene_id, unsigned short boss_id, Role *killer_role)
{
	for (std::vector<LevelBossInfo>::iterator level_cfg_it = m_shanggu_boss_level_list.begin(); level_cfg_it != m_shanggu_boss_level_list.end(); ++level_cfg_it)
	{
		if (level_cfg_it->scene_id == scene_id)
		{
			const ShangGuBossInfoConfig::ItemConfig *boss_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossInfoCfg(level_cfg_it->level, boss_id);
			if (NULL != boss_cfg)
			{
				int flush_interval_s = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossNextFlushTimeInterval(static_cast<unsigned int>(EngineAdapter::Instance().Time()));
				this->OnBossDie(boss_id, killer_role, flush_interval_s, level_cfg_it->boss_map, level_cfg_it->level);

				// 随机出怪，需是boss
				if (SHANGGU_BOSS_TYPE_BOSS == boss_cfg->type && boss_cfg->boss_lv_seq > 0 && boss_cfg->boss_lv_seq <= SHANGGU_BOSS_HIDE_BOSS_MAX_LEVEL_NUM)
				{
					std::map<int, ShangGuHideBossInfo>::iterator it = m_shanggu_hide_info_map.find(boss_cfg->boss_lv_seq);
					if (it != m_shanggu_hide_info_map.end() && it->second.temp_weight > 0 && !it->second.is_has_flush)
					{
						int rand_val = RandomNum(it->second.temp_weight);
						if (rand_val < it->second.average_weight)
						{
							it->second.is_has_flush = true;
							this->OnFlushShangGuHideBoss(boss_cfg->boss_lv_seq, level_cfg_it->level, killer_role, scene_id, boss_id);
						}

						it->second.temp_weight -= it->second.average_weight;
					}
				}
			}
			return;
		}
	}
}

void BossShangGuManager::OnShangGuBossEraseMap(int layer, ObjID obj_id)
{
	long long key = ConvertParamToLongLong(layer, obj_id);
	std::map<long long, ShangGuObjIdInfo>::iterator it = m_shanggu_boss_obj_id_info_map.find(key);
	if (it != m_shanggu_boss_obj_id_info_map.end())
	{
		m_shanggu_boss_obj_id_info_map.erase(it);
	}

	const ShangGuBossLayerConfig *layer_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossLayerCfg(layer);
	if (NULL != layer_cfg)
	{
		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(layer_cfg->scene_id, COMMON_SCENE_KEY);
		if (NULL != scene)
		{
			this->SendShangGuBossSceneOtherInfo(scene, NULL);
		}
	}
}

void BossShangGuManager::OnBossDie(unsigned short boss_id, Role *killer_role, int flush_interval_s, std::map<int, BossInfo> & boss_map, int level)
{
	unsigned int now_second = (unsigned int)EngineAdapter::Instance().Time();

	std::map<int, BossInfo>::iterator boss_info_it = boss_map.find(boss_id);
	if (boss_info_it != boss_map.end())
	{
		boss_info_it->second.next_refresh_time = (unsigned int)(now_second + flush_interval_s);
		boss_info_it->second.obj_id = INVALID_OBJ_ID;

		if (NULL == killer_role) return;  // 把判空放这里是为防止role为空时，next_refresh_time没赋值到导致不会刷新BOSS的BUG

		this->SendBossInfoToAll(boss_info_it->second, Protocol::NOTIFY_REASON_BOSS_DIE, killer_role);

		BossInfo::KillBossHistory history_item;
		history_item.uid = killer_role->GetUID();
		history_item.kill_boss_time = (unsigned int)EngineAdapter::Instance().Time();
		boss_info_it->second.kill_boss_history_list.push_back(history_item);

		if ((int)boss_info_it->second.kill_boss_history_list.size() > Protocol::SCWorldBossItemInfo::HISTORY_KILLER_MAX_COUNT) //不用存那么多
		{
			boss_info_it->second.kill_boss_history_list.pop_front();
		}
	}
}

void BossShangGuManager::RegisterConcernBoss(int uid, int param1, int param2)
{
	if (!LOGIC_CONFIG->GetShangGuBossConfig().IsShangGuBossOpen())
	{
		return;
	}

	for (std::vector<LevelBossInfo>::iterator it = m_shanggu_boss_level_list.begin(); it != m_shanggu_boss_level_list.end(); ++it)
	{
		if (it->level != param1) continue;

		for (std::map<int, BossInfo>::iterator boss_info_it = it->boss_map.begin(); boss_info_it != it->boss_map.end(); ++boss_info_it)
		{
			if (boss_info_it->second.index == param2)
			{
				boss_info_it->second.register_role_set.insert(uid);
				break;
			}
		}
		break;
	}
}

void BossShangGuManager::UnRegisterConcernBoss(int uid, int param1, int param2)
{
	if (!LOGIC_CONFIG->GetShangGuBossConfig().IsShangGuBossOpen())
	{
		return;
	}

	for (std::vector<LevelBossInfo>::iterator it = m_shanggu_boss_level_list.begin(); it != m_shanggu_boss_level_list.end(); ++it)
	{
		if (it->level != param1) continue;

		for (std::map<int, BossInfo>::iterator boss_info_it = it->boss_map.begin(); boss_info_it != it->boss_map.end(); ++boss_info_it)
		{
			if (boss_info_it->second.index == param2)
			{
				boss_info_it->second.register_role_set.erase(uid);
				break;
			}
		}

		break;
	}
}

void BossShangGuManager::OnShangGuBossReq(Role *role, short opera_type, short param1, short param2, short param3)
{
	if (!LOGIC_CONFIG->GetShangGuBossConfig().IsShangGuBossOpen())
	{
		return;
	}

	switch (opera_type)
	{
	case Protocol::SHANGGU_BOSS_OPERA_TYPE_ENTER:
	{
		this->OnReqEnterShangGuBoss(role, param1, param2);
	}
	break;

	case Protocol::SHANGGU_BOSS_OPERA_TYPE_ALL_BOSS_INFO:
	{
		this->SendShangGuBossAllInfo(role);
		this->SendShangGuBossSceneOtherInfo(NULL, role);
	}
	break;

	case Protocol::SHANGGU_BOSS_OPERA_TYPE_LAYER_BOSS_INFO:
	{

		this->SendShangGuBossLayerInfo(role, param1);
	}
	break;

	case Protocol::SHANGGU_BOSS_OPERA_TYPE_SET_CONCERN:
	{
		const ShangGuBossInfoConfig::ItemConfig * boss_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossInfoCfgByBossId(param1);
		if (NULL != boss_cfg)
		{
			if (param2 != Protocol::CSFollowBossReq::GET_FOLLOW_LIST)
			{
				const ShangGuBossInfoConfig * layer_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossCfgByBossId(param1);
				if (layer_cfg != nullptr)
				{
					role->SetShangGuBossConcern(layer_cfg->layer, boss_cfg->index, param2 == Protocol::CSFollowBossReq::FOLLOW_BOSS);
					role->NoticeNum(noticenum::NT_WORLD_BOSS_CONCERN_SUCC);
				}
			}

			this->SendFollowBossList(role);
		}
	}
	break;

	default:
		break;
	}
}

void BossShangGuManager::OnReqEnterShangGuBoss(Role *role, int level, int boss_id)
{
	if (NULL == role)
	{
		return;
	}

	if (level < 0 || level >= SHANGGU_BOSS_CFG_MAX_LAYER || level >= LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossLayerCount())
	{
		return;
	}

	if (!role->GetRoleStatusManager()->CanEnterFB(true, false))
	{
		return;
	}

	const ShangGuBossLayerConfig *layer_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossLayerCfg(level);
	if (NULL == layer_cfg)
	{
		return;
	}

	if (role->GetScene()->GetSceneID() == layer_cfg->scene_id) // 已在对应场景里，不给传送
	{
		return;
	}

	if (role->GetLevel() < layer_cfg->need_role_level)
	{
		role->NoticeNum(errornum::EN_CHALLENGE_FB_LEVEL_LIMIT);
		return;
	}

	const ShangGuBossOtherCfg &other_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossOtherCfg();
	int max_enter_times = other_cfg.shanggu_day_free_times + LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_SHANGGU_BOSS_BUY_ENTER_TIMES);

	if (role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_SHANGGUBOSS_ENTER_TIMES) >= max_enter_times)
	{
		role->NoticeNum(errornum::EN_SHANGGU_BOSS_ENTER_TIMES_FULL);
		return;
	}

	// 消耗
	int consume_num = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossEnterConsumeStuffNum(role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_SHANGGUBOSS_ENTER_TIMES) + 1);
	if (!role->GetKnapsack()->ConsumeItemOrUseGold(other_cfg.shanggu_tiky_item_id, consume_num, "BossShangGuManager::OnReqEnterShangGuBoss", true, other_cfg.can_use_bind_gold))
	{
		return;
	}

	// 给予
	role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_SHANGGUBOSS_ENTER_TIMES);

	gamelog::g_log_debug.printf(LL_DEBUG, "BOSS_ENTER_SHANGGU_BOSS_SCENE: %s[%d] [scene_id:%d] enter_times:[%d]", role->GetName(), UidToInt(role->GetUserId()), layer_cfg->scene_id, role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_SHANGGUBOSS_ENTER_TIMES));
	World::GetInstWorld()->GetSceneManager()->GoTo(role, layer_cfg->scene_id, COMMON_SCENE_KEY, layer_cfg->pos);
}

void BossShangGuManager::ClearShangGuBossAllItem()
{
	const int layer_count = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossLayerCount();
	for (int layer = 0; layer < layer_count && layer < SHANGGU_BOSS_CFG_MAX_LAYER; ++layer)
	{
		const ShangGuBossLayerConfig *layer_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossLayerCfg(layer);
		if (NULL == layer_cfg)
		{
			continue;
		}

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(layer_cfg->scene_id, COMMON_SCENE_KEY);
		if (NULL == scene)
		{
			continue;
		}

		for (std::map<long long, ShangGuObjIdInfo>::const_iterator it = m_shanggu_boss_obj_id_info_map.begin(); it != m_shanggu_boss_obj_id_info_map.end(); ++it)
		{
			bool is_skip = false;
			Obj *obj = scene->GetObj(it->second.obj_id);
			if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
			{
				Monster *monster = (Monster *)obj;
				if (LOGIC_CONFIG->GetShangGuBossConfig().IsShangguBossMonster(monster->GetMonsterId()))
				{
					is_skip = true;
				}
			}
			if (!is_skip)
			{
				scene->DeleteObj(it->second.obj_id);
			}
		}
	}

	m_shanggu_boss_obj_id_info_map.clear();
}

bool BossShangGuManager::GetShangGuBossGatherItemRandomPosition(Scene *scene, Posi *pos)
{
	if (NULL == scene || NULL == pos)
	{
		return false;
	}

	// 获取随机坐标
	Axis rang_x = 0;
	Axis rang_y = 0;
	scene->GetMap()->GetArea(&rang_x, &rang_y);
	if (rang_x <= 0 || rang_y <= 0)
	{
		return false;
	}

	int pos_x = -1;
	int pos_y = -1;

	int loop_times = 0;
	bool is_find = false;

	while (loop_times < 100)		// 100次应该可以随机到一个可用位置了
	{
		pos_x = RandomNum(rang_x);
		pos_y = RandomNum(rang_y);

		if ((scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, pos_x, pos_y))/* || (dir != Dir_S && fabs(dir) > Dir_Max*/) /* _dir >= Dir_Count*/
		{
			is_find = true;

			// 一定范围内必须没有其它采集物
			static ObjID obj_list[256] = { 0 };
			const static int AOI_RANGE = 3;
			int count = scene->GetZoneMatrix()->GetObjByArea(Posi(pos_x, pos_y), AOI_RANGE, AOI_RANGE, obj_list, sizeof(obj_list) / sizeof(obj_list[0]));
			for (int i = 0; i < count; i++)
			{
				Obj *obj = scene->GetObj(obj_list[i]);
				if (NULL != obj && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
				{
					is_find = false;
					break;
				}
			}
		}

		if (is_find)
		{
			break;
		}

		++loop_times;
	}

	if (!is_find)
	{
		return false;
	}

	*pos = Posi(pos_x, pos_y);

	return true;
}

void BossShangGuManager::SendShangGuBossAllInfo(Role *role)
{
	if (NULL == role || NULL == role->GetScene())
	{
		return;
	}

	static Protocol::SCShangGuBossAllInfo dkai;
	dkai.tire_value = 0;
	dkai.enter_times = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_SHANGGUBOSS_ENTER_TIMES);
	dkai.layer_count = 0;

	for (std::vector<LevelBossInfo>::iterator level_data_it = m_shanggu_boss_level_list.begin(); level_data_it != m_shanggu_boss_level_list.end(); ++level_data_it)
	{
		Protocol::SCShangGuBossAllInfo::LayerBossInfo &boss_level_info = dkai.layer_list[dkai.layer_count];
		this->GetLayerBossMsg(boss_level_info.boss_count, boss_level_info.boss_info_list, Protocol::SC_SHANGGU_BOSS_LAYER_MAX_COUNT, level_data_it->boss_map, role, level_data_it->level);
		++dkai.layer_count;
	}

	int send_len = sizeof(dkai) - (Protocol::SC_SHANGGU_BOSS_MAX_LAYER - dkai.layer_count) * sizeof(dkai.layer_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&dkai, send_len);
}

void BossShangGuManager::SendShangGuBossLayerInfo(Role *role, int layer)
{
	if (NULL == role)
	{
		return;
	}

	static Protocol::SCShangGuBossLayerInfo dkli;
	dkli.boss_count = 0;
	dkli.cur_layer = layer;

	for (std::vector<LevelBossInfo>::iterator level_data_it = m_shanggu_boss_level_list.begin(); level_data_it != m_shanggu_boss_level_list.end(); ++level_data_it)
	{
		if (level_data_it->level == dkli.cur_layer)
		{
			this->GetLayerBossMsg(dkli.boss_count, dkli.boss_info_list, Protocol::SC_SHANGGU_BOSS_LAYER_MAX_COUNT, level_data_it->boss_map, role, dkli.cur_layer);
			break;
		}
	}

	int send_len = sizeof(dkli) - (Protocol::SC_SHANGGU_BOSS_LAYER_MAX_COUNT - dkli.boss_count) * sizeof(dkli.boss_info_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&dkli, send_len);
}

void BossShangGuManager::SendShangGuBossSceneOtherInfo(Scene *scene, Role *role)
{
	const int layer_count = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossLayerCount();

	static Protocol::SCShangGuBossSceneOtherInfo oio; memset(oio.layer_info_list, 0, sizeof(oio.layer_info_list));
	oio.layer_count = layer_count;

	for (int layer = 0; layer < layer_count && layer < Protocol::SC_SHANGGU_BOSS_MAX_LAYER; ++layer)
	{
		for (std::map<long long, ShangGuObjIdInfo>::const_iterator it = m_shanggu_boss_obj_id_info_map.begin(); it != m_shanggu_boss_obj_id_info_map.end(); ++it)
		{
			if (it->second.layer == layer)
			{
				if (SHANGGU_BOSS_KIND_TYPE_MAX_BOX == it->second.kind_type)
				{
					++oio.layer_info_list[layer].max_boss_num;
				}
				else if (SHANGGU_BOSS_KIND_TYPE_MIN_BOX == it->second.kind_type)
				{
					++oio.layer_info_list[layer].min_boss_num;
				}
				else if (SHANGGU_BOSS_KIND_TYPE_GOLD_MONSTER == it->second.kind_type)
				{
					++oio.layer_info_list[layer].gold_monster_num;
				}
				else if (SHANGGU_BOSS_KIND_TYPE_HIDE_MONSTER == it->second.kind_type)
				{
					++oio.layer_info_list[layer].hide_boss_num;
				}
			}
		}
	}

	int length = sizeof(oio) - (Protocol::SC_SHANGGU_BOSS_MAX_LAYER - oio.layer_count) * (sizeof(oio.layer_info_list[0]));

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&oio, length);
		return;
	}

	if (NULL != scene)
	{
		scene->SendToRole((const char*)&oio, length);
	}
	else
	{
		World::GetInstWorld()->SendToAllGateway((const char*)&oio, length);
	}
}

void BossShangGuManager::UpdataShangGuBossGatherItemAndGoldMonster(time_t now_second)
{
	if (0 != m_shanggu_boss_next_flush_timestamp && now_second > m_shanggu_boss_next_flush_timestamp)
	{
		m_shanggu_boss_next_flush_timestamp = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossGatherItemNextFlushTime(static_cast<unsigned int>(now_second));

		// 清掉场景里残留大宝箱、小宝箱、黄金怪、隐藏BOSS
		this->ClearShangGuBossAllItem();
		this->CreateShangGuBossGatherItemAndGoldMonster();

		for (int layer = 0; layer < SHANGGU_BOSS_CFG_MAX_LAYER; ++layer)
		{
			const ShangGuBossLayerConfig *layer_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossLayerCfg(layer);
			if (NULL != layer_cfg)
			{
				Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(layer_cfg->scene_id, COMMON_SCENE_KEY);
				if (NULL != scene)
				{
					this->SendShangGuBossSceneOtherInfo(scene, NULL);
				}
			}
		}
	}
}

void BossShangGuManager::OnFlushShangGuHideBoss(int boss_lv_seq, int level, Role *killer_role, int scene_id, int boss_id)
{
	if (NULL == killer_role || boss_lv_seq <= 0 || boss_lv_seq > SHANGGU_BOSS_HIDE_BOSS_MAX_LEVEL_NUM)
	{
		return;
	}

	const int boss_count = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossHideBossCount(boss_lv_seq);
	if (boss_count <= 0)
	{
		return;
	}

	const ShangGuBossLayerConfig *layer_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossLayerCfg(level);
	if (NULL == layer_cfg)
	{
		return;
	}

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(layer_cfg->scene_id, COMMON_SCENE_KEY);
	if (NULL == scene)
	{
		return;
	}

	int count = 0;
	int loop_times = 0;
	std::set<int> index_list_set;	// 坐标索引集合
	while (count < boss_count && loop_times < 1000)
	{
		++loop_times;
		int rand_index = RandomNum(ShangGuBossHideBossConfig::POSI_MAX_COUNT);
		if (index_list_set.find(rand_index) == index_list_set.end())
		{
			index_list_set.insert(rand_index);
			++count;
		}
	}

	std::set<int>::const_iterator it = index_list_set.begin();
	for (int i = 0; i < boss_count && i < ShangGuBossHideBossConfig::LEVEL_MAX_MONSTER_COUNT; ++i)
	{
		const ShangGuBossHideBossConfig::LevelConfig *hide_boss_cfg = LOGIC_CONFIG->GetShangGuBossConfig().GetShangGuBossHideBossCfg(boss_lv_seq, i);
		if (NULL == hide_boss_cfg)
		{
			continue;
		}

		if (it == index_list_set.end())
		{
			continue;
		}

		if (*it >= 0 && *it < ShangGuBossHideBossConfig::POSI_MAX_COUNT)
		{
			Monster *monster = MONSTERPOOL->CreateMonster(hide_boss_cfg->monster_id, scene, hide_boss_cfg->pos_list[*it]);
			if (NULL != monster)
			{
				ShangGuObjIdInfo info;
				info.layer = level;
				info.obj_id = monster->GetId();
				info.kind_type = SHANGGU_BOSS_KIND_TYPE_HIDE_MONSTER;

				long long key = ConvertParamToLongLong(info.layer, info.obj_id);
				m_shanggu_boss_obj_id_info_map[key] = info;
			}
		}

		++it;
	}

	{
		// 传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shanggu_boss_hide_flush_notice,
			killer_role->GetUID(), killer_role->GetName(), (int)killer_role->GetCamp(), scene_id, boss_id);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3, 0, 0);
		}
	}

	this->SendShangGuBossSceneOtherInfo(scene, NULL);
}

void BossShangGuManager::GetLayerBossMsg(short &boss_count, Protocol::SCWorldBossItemInfo *boss_info_list, int max_count, const std::map<int, BossInfo> & boss_map, Role *role, int level)
{
	boss_count = 0;
	for (std::map<int, BossInfo>::const_iterator boss_info_it = boss_map.begin(); boss_info_it != boss_map.end() && boss_count < Protocol::SC_SHANGGU_BOSS_LAYER_MAX_COUNT * Protocol::SC_SHANGGU_BOSS_MAX_LAYER; ++boss_info_it)
	{
		Protocol::SCWorldBossItemInfo &killer_item = boss_info_list[boss_count];

		killer_item.boss_id = boss_info_it->second.boss_id;
		killer_item.next_refresh_time = boss_info_it->second.next_refresh_time;
		killer_item.is_concern = (role->GetShangGuBossConcern(level, boss_info_it->second.index) ? 1 : 0);
		++boss_count;
	}
}

void BossShangGuManager::SendFollowBossList(Role *role)
{
	if (role == nullptr) return;

	Protocol::SCFollowBossInfo fbi;
	fbi.count = 0;

	for (std::vector<LevelBossInfo>::iterator it = m_shanggu_boss_level_list.begin(); it != m_shanggu_boss_level_list.end(); ++it)
	{
		for (std::map<int, BossInfo>::iterator boss_info_it = it->boss_map.begin(); boss_info_it != it->boss_map.end() && fbi.count < Protocol::SCFollowBossInfo::MAX_BOSS_COUNT; ++boss_info_it)
		{
			if (role->GetShangGuBossConcern(it->level, boss_info_it->second.index) == 1)
			{
				fbi.follow_boss_list[fbi.count].boss_id = boss_info_it->second.boss_id;
				fbi.follow_boss_list[fbi.count].boss_type = Protocol::BOSS_ENTER_TYPE_BOSS_SHANGGU;
				fbi.follow_boss_list[fbi.count].reserve_ch = 0;
				fbi.follow_boss_list[fbi.count].scene_id = boss_info_it->second.scene_id;
				++fbi.count;
			}
		}
	}
	
	int sendlen = sizeof(fbi) - sizeof(fbi.follow_boss_list[0]) * (Protocol::SCFollowBossInfo::MAX_BOSS_COUNT - fbi.count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fbi, sendlen);
}

void BossShangGuManager::SendBossInfoToAll(BossInfo &boss_info, int notify_resaon, Role *killer_role)
{
	static Protocol::SCBossInfoToAll bita;
	bita.notify_reason = notify_resaon;
	bita.boss_type = Protocol::BOSS_ENTER_TYPE_BOSS_SHANGGU;
	bita.boss_id = boss_info.boss_id;
	bita.scene_id = boss_info.scene_id;
	bita.status = boss_info.next_refresh_time == 0 ? ENBOSSSTATUS_EXIST : ENBOSSSTATUS_NONEXIST;
	bita.next_refresh_time = boss_info.next_refresh_time;
	bita.killer_uid = 0;
	if (NULL != killer_role)
	{
		bita.killer_uid = killer_role->GetUID();
	}
	World::GetInstWorld()->SendToAllGatewayAndCrossUser((const char *)&bita, sizeof(bita));
}

void BossShangGuManager::SendShangGuKillerInfo(Role *role, int boss_id)
{
	if (!LOGIC_CONFIG->GetShangGuBossConfig().IsShangGuBossOpen())
	{
		return;
	}

	if (nullptr == role)
	{
		return;
	}

	static Protocol::SCBossKillerList bkl;
	int count = 0;
	for (std::vector<LevelBossInfo>::iterator it = m_shanggu_boss_level_list.begin(); it != m_shanggu_boss_level_list.end(); ++it)
	{
		std::map<int, BossInfo>::iterator boss_info_it = it->boss_map.find(boss_id);
		if (boss_info_it != it->boss_map.end())
		{
			std::deque<BossInfo::KillBossHistory>::const_iterator history_it = boss_info_it->second.kill_boss_history_list.begin();
			count = 0;
			for (;history_it != boss_info_it->second.kill_boss_history_list.end() && count < KILLER_LIST_MAX_COUNT; ++history_it)
			{
				UserCacheNode * cache_user = UserCacheManager::Instance().GetUserNode(history_it->uid);
				if (nullptr == cache_user) continue;
				bkl.killer_info_list[count].killier_time = history_it->kill_boss_time;
				bkl.killer_info_list[count].killer_uid = history_it->uid;
				cache_user->GetName(bkl.killer_info_list[count].killer_name);

				++count;
			}
		}
	}
	
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&bkl, sizeof(bkl));
}