#include "specialgodmagicboss.hpp"
#include <algorithm>
#include "obj/character/role.h"
#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "engineadapter.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "internalcomm.h"
#include "gamelog.h"
#include "servercommon/userprotocal/msgcrosscommon.h"
#include "obj/character/monster.h"
#include "monster/monsterpool.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "obj/character/attribute.hpp"
#include "effect/effectbase.hpp"
#include <limits>
#include "servercommon/string/crossstr.h"
#include "servercommon/servercommon.h"
#include "world.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "globalconfig/globalconfig.h"
#include "gameworld/gameworld/monster/drop/droppool.hpp"
#include "other/event/eventhandler.hpp"
#include "global/team/teammanager.hpp"
#include "obj/otherobj/gatherobj.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "friend/teaminfo.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "protocal/msgfb.h"
#include "global/bossfamily/bossfamilymanager.hpp"
#include "other/event/eventhandler.hpp"
#include "gameworld/config/activityconfig/godmagicbossconfig.hpp"
#include "servercommon/commondata.hpp"
#include "global/worldstatus/worldstatus.hpp"

SpecialGodmagicBoss::SpecialGodmagicBoss(Scene *scene)
	: SpecialLogic(scene), m_first(true), m_boss_count(0), m_monster_count(0), m_left_monster_count(0), m_ordinary_crystal_count(0), m_left_treasure_crystal_num(0), m_boss_next_flush_timestamp(0),
	m_monster_next_flush_timestamp(0), m_next_add_exp_timestamp(0), m_treasure_crystal_next_flush_timestamp(0), m_ordinary_crystal_next_flush_timestamp(0)
{
}

SpecialGodmagicBoss::~SpecialGodmagicBoss()
{
}

void SpecialGodmagicBoss::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	const GodmagicBossOtherCfg &other_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetOtherCfg();

	if (m_first)
	{
		int layer = LOGIC_CONFIG->GetGodmagicBossConfig().GetLayerIndexBySceneId(this->GetSceneId());
		if (layer <= 0 || layer > GODMAGIC_BOSS_SCENE_MAX)
		{
			return;
		}
		m_boss_count = LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossBossCount(layer);

		const GodmagicBossLayerCfg *layer_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossLayerCfg(layer);
		if (NULL != layer_cfg && layer_cfg->is_flush_monster > 0)
		{
			m_monster_count = LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossMonsterCount(layer);
		}
		
		m_ordinary_crystal_count = LOGIC_CONFIG->GetGodmagicBossConfig().GetOrdinaryCrystalCount();
		m_first = false;
	}

	this->ReduceGatherRoleHp();

	bool is_need_sync = false;

	if (now_second >= m_boss_next_flush_timestamp)
	{
		this->RefreshBoss();
		m_boss_next_flush_timestamp = static_cast<unsigned int>(now_second) + 1;
	}

	if (now_second >= m_ordinary_crystal_next_flush_timestamp)
	{
		this->RefreshOrdinaryCrystal();   // 普通水晶刷新  屏蔽
		m_ordinary_crystal_next_flush_timestamp = static_cast<unsigned int>(now_second) + 5;
	}

	if (now_second >= m_monster_next_flush_timestamp)
	{
		//this->RefreshMonster();
		m_monster_next_flush_timestamp = static_cast<unsigned int>(now_second) + 5;
	}

	if (now_second >= m_next_add_exp_timestamp)
	{
		this->AddExp();
		m_next_add_exp_timestamp = static_cast<unsigned int>(now_second) + other_cfg.reward_exp_cd_s;
	}

	if (now_second >= m_treasure_crystal_next_flush_timestamp)
	{
		this->RefreshTreasureCrystal();   // 珍惜水晶刷新  屏蔽
		m_treasure_crystal_next_flush_timestamp = static_cast<unsigned int>(now_second) + other_cfg.treasure_crystal_flush_time_s;
		//is_need_sync = true;
	}

	if (is_need_sync)
	{
		this->SyncSceneInfo();
	}
}

void SpecialGodmagicBoss::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	const int role_num = (int)m_scene->RoleNum();
	for (int i = 0; i < role_num; ++i)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);
		if (NULL != temp_role)
		{
			std::map<int, GodmagicRoleInfo>::iterator user_it = m_user_map.find(temp_role->GetUID());

			if (user_it != m_user_map.end())
			{
				user_it->second.tire_value = 0;
				user_it->second.today_gather_ordinary_crystal_times = 0;
				user_it->second.today_gather_treasure_crystal_times = 0;
			}
			this->SendInfoToRole(temp_role);
		}
	}
}

void SpecialGodmagicBoss::OnActivityOpen()
{
}

void SpecialGodmagicBoss::OnActivityClose()
{
}

void SpecialGodmagicBoss::OnCharacterDie(Character *dead_cha, ObjID killer_objid)
{
	if (NULL == dead_cha) return;

	Obj *killer_obj = m_scene->GetObj(killer_objid);
	if (NULL == killer_obj)
	{
		return;
	}

	if (dead_cha->GetObjType() == Obj::OBJ_TYPE_ROLE)
	{
		Role *dead_role = (Role *)dead_cha;
		dead_role->DieOnReliveTireScene(true);
		dead_role->SendReliveTireInfo();
	}

	if (Obj::OBJ_TYPE_ROLE == killer_obj->GetObjType())
	{
		Role *killer_role = (Role *)killer_obj;

		if (dead_cha->GetObjType() == Obj::OBJ_TYPE_ROLE)    // 玩家杀玩家
		{
			Role *dead_role = (Role *)dead_cha;

			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_be_killed_in_cross_boss, killer_role->GetUID(),
				killer_role->GetName(), (int)killer_role->GetCamp(), m_scene->GetSceneID(), dead_role->GetUID(), dead_role->GetName(), dead_role->GetCamp());
			if (length > 0)
			{
				int sendlen = 0;
				Protocol::SCSystemMsg sm;

				if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3))
				{
					InternalComm::Instance().UserProtocalTransferToGame(0, (const char*)&sm, sendlen);
				}
			}
		}
		else if (dead_cha->GetObjType() == Obj::OBJ_TYPE_MONSTER)    // 玩家杀怪物
		{
			Monster *monster = (Monster *)dead_cha;

			if (monster == nullptr) return;
			bool is_need_sync = false;
			unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			for (int i = 0; i < m_boss_count && i < MAX_GODMAGIC_BOSS_PER_SCENE; i++)
			{
				if ((m_boss_list[i].boss_id == monster->GetMonsterId()) && (1 == m_boss_list[i].is_exist))
				{
					const GodmagicBossBossCfg::BossCfg *boss_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossBossCfgByMonsterId(this->GetSceneId(), m_boss_list[i].boss_id);
					if (NULL == boss_cfg) continue;

					m_boss_list[i].is_exist = 0;
					//m_boss_list[i].next_flush_time = now_time + LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossNextFlushTimeInterval(now_time);
					 // 根据平台导入玩家数调整刷新间隔
					int interval_s = WorldStatus::Instance().GetRefreshTimeInterval(boss_cfg->flush_interval);
					m_boss_list[i].next_flush_time = now_time + interval_s;


					int record_index = m_boss_list[i].record_cur_index % GODMAGIC_BOSS_MAX_HISTROY_RECROD;
					m_boss_list[i].killer_recrod_list[record_index].dead_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
					killer_role->GetName(m_boss_list[i].killer_recrod_list[record_index].killer_name);
					m_boss_list[i].killer_recrod_list[record_index].uid = killer_role->GetUID();

					m_boss_list[i].record_cur_index = record_index + 1;

					if (m_boss_list[i].record_count < GODMAGIC_BOSS_MAX_HISTROY_RECROD)
					{
						++m_boss_list[i].record_count;
					}

					this->CreateBossBroadcast(Protocol::NOTIFY_REASON_BOSS_DIE, m_scene->GetSceneID(), m_boss_list[i].boss_id, m_boss_list[i].next_flush_time, nullptr);

					is_need_sync = true;
					break;
				}
			}

			const GodmagicBossOtherCfg &other_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetOtherCfg();
			for (int i = 0; i < m_monster_count && i < GODMAGIC_BOSS_MAX_MONSTER_NUM; i++)
			{
				if ((m_monster_list[i].monster_id == monster->GetMonsterId()) && (1 == m_monster_list[i].is_exist))
				{
					int layer = LOGIC_CONFIG->GetGodmagicBossConfig().GetLayerIndexBySceneId(this->GetSceneId());
					const GodmagicBossMonsterCfg::MonsterCfg *monster_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossMonsterCfgByLayer(layer, i);
					if (NULL != monster_cfg)
					{
						m_monster_list[i].is_exist = 0;
						m_monster_list[i].next_flush_time = now_time + other_cfg.monster_flush_time_s;
						if (m_left_monster_count > 0)
						{
							m_left_monster_count--;
						}

						is_need_sync = true;
						break;
					}
				}
			}

			if (is_need_sync)
			{
				this->SyncSceneInfo();
			}
		}
	}
	
}

void SpecialGodmagicBoss::RefreshTreasureCrystal()
{
	this->CleanAllTreasureCrystal();

	int layer = LOGIC_CONFIG->GetGodmagicBossConfig().GetLayerIndexBySceneId(this->GetSceneId());
	const GodmagicBossTreasureCrystalPosCfg *point_list = LOGIC_CONFIG->GetGodmagicBossConfig().GetTreasureCrystalPointListByLayer(layer);
	if (NULL == point_list)
	{
		return;
	}

	int cfg_point_num = point_list->pos_num;
	const GodmagicBossOtherCfg &other_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetOtherCfg();
	int treasure_crystal_num = other_cfg.treasure_crystal_flush_num;
	int gather_id = LOGIC_CONFIG->GetGodmagicBossConfig().GetTreasureGatherIdBySceneId(this->GetSceneId());

	int index = 0;
	std::set<int> index_set; index_set.clear();
	if (cfg_point_num > 0)
	{
		for (int i = 0; i < cfg_point_num && i < GODMAGIC_BOSS_MAX_TREASURE_CRYSTAL_POS_NUM; i++)
		{
			int rand_index = RandomNum(cfg_point_num);

			std::set<int>::iterator it = index_set.find(rand_index);
			if (it == index_set.end())
			{
				const Posi &pos = point_list->pos_list[rand_index];
				{
					if (m_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, pos.x, pos.y))
					{
						GatherObj *gather_obj = new GatherObj();
						gather_obj->SetPos(pos);
						gather_obj->Init(NULL, gather_id, other_cfg.gather_time_s * 1000, 0, true, 0);

						m_scene->AddObj(gather_obj);
						m_treasure_crystal_obj_id_set.insert(gather_obj->GetId());

						index_set.insert(rand_index);
						index++;
					}
				}
			}

			if (int(index_set.size()) >= treasure_crystal_num)
			{
				break;
			}
		}
	}

	m_left_treasure_crystal_num = static_cast<int>(m_treasure_crystal_obj_id_set.size());
}

void SpecialGodmagicBoss::CleanAllTreasureCrystal()
{
	for (std::set<ObjID>::iterator it = m_treasure_crystal_obj_id_set.begin(); it != m_treasure_crystal_obj_id_set.end(); ++it)
	{
		Obj *obj = m_scene->GetObj(*it);
		if (NULL != obj && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
		{
			m_scene->DeleteObj(*it);
		}
	}

	m_treasure_crystal_obj_id_set.clear();
	m_left_treasure_crystal_num = 0;
}

void SpecialGodmagicBoss::RefreshOrdinaryCrystal()
{
	bool is_need_sync = false;
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	const GodmagicBossOtherCfg &other_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetOtherCfg();
	for (int i = 0; i < m_ordinary_crystal_count && i < GODMAGIC_BOSS_MAX_ORDINARY_CRYSTAL; i++)
	{
		if (0 == m_ordinary_crystal_list[i].is_exist && now >= m_ordinary_crystal_list[i].next_flush_time)
		{
			const GodmagicBossOrdinaryCrystalPosCfg *ordinary_crystal_pos = LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossOrdinaryCrystalPosCfg(i + 1);
			if (NULL != ordinary_crystal_pos)
			{
				if (m_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, ordinary_crystal_pos->pos.x, ordinary_crystal_pos->pos.y))
				{
					GatherObj *gather_obj = new GatherObj();
					gather_obj->SetPos(ordinary_crystal_pos->pos);
					gather_obj->Init(NULL, other_cfg.ordinary_crystal_gather_id, other_cfg.gather_time_s1 * 1000, 0, true, 0);

					m_scene->AddObj(gather_obj);

					if (NULL != gather_obj)
					{
						m_ordinary_crystal_list[i].index = ordinary_crystal_pos->index;
						m_ordinary_crystal_list[i].is_exist = 1;
						m_ordinary_crystal_list[i].next_flush_time = 0;

						is_need_sync = true;
					}
				}
			}
		}
	}

	if (is_need_sync)
	{
		this->SyncSceneInfo();
	}
}

void SpecialGodmagicBoss::RefreshMonster()
{
	bool is_need_sync = false;
	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	for (int i = 0; i < m_monster_count && i < GODMAGIC_BOSS_MAX_MONSTER_NUM; i++)
	{
		if (0 == m_monster_list[i].is_exist && now_time >= m_monster_list[i].next_flush_time)
		{
			int layer = LOGIC_CONFIG->GetGodmagicBossConfig().GetLayerIndexBySceneId(this->GetSceneId());
			const GodmagicBossMonsterCfg::MonsterCfg *monster_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossMonsterCfgByLayer(layer, i);
			if (NULL != monster_cfg)
			{
				Posi pos;
				pos.x = monster_cfg->pos_x;
				pos.y = monster_cfg->pos_y;
				Monster *monster = MONSTERPOOL->CreateMonster(monster_cfg->monster_id, m_scene, pos);
				if (NULL != monster)
				{
					m_monster_list[i].monster_id = monster_cfg->monster_id;
					m_monster_list[i].is_exist = 1;
					m_monster_list[i].next_flush_time = 0;

					++m_left_monster_count;
					is_need_sync = true;
				}
			}
		}
	}

	if (is_need_sync)
	{
		this->SyncSceneInfo();
	}
}

void SpecialGodmagicBoss::RefreshBoss()
{
	bool is_need_sync = false;
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	for (int i = 0; i < m_boss_count && i < MAX_GODMAGIC_BOSS_PER_SCENE; i++)
	{
		if (0 == m_boss_list[i].is_exist && (!m_boss_list[i].has_notice_role) && (int(m_boss_list[i].next_flush_time - now)) <= SECOND_PER_MIN)
		{
			this->CreateBossBroadcast(Protocol::NOTIFY_REASON_BOSS_ADVANCE, m_scene->GetSceneID(), m_boss_list[i].boss_id, m_boss_list[i].next_flush_time, nullptr);
			m_boss_list[i].has_notice_role = true;
		}
		else if (0 == m_boss_list[i].is_exist && now >= m_boss_list[i].next_flush_time)
		{
			int layer = LOGIC_CONFIG->GetGodmagicBossConfig().GetLayerIndexBySceneId(this->GetSceneId());
			const GodmagicBossBossCfg::BossCfg *boss_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossBossCfgByLayer(layer, i);
			if (NULL != boss_cfg)
			{
				Posi pos;
				pos.x = boss_cfg->flush_pos_x;
				pos.y = boss_cfg->flush_pos_y;
				Monster *monster = MONSTERPOOL->CreateMonster(boss_cfg->boss_id, m_scene, pos);
				if (NULL != monster)
				{
					m_boss_list[i].boss_id = boss_cfg->boss_id;
					m_boss_list[i].is_exist = 1;
					m_boss_list[i].next_flush_time = 0;
					m_boss_list[i].has_notice_role = false;

					is_need_sync = true;

					this->CreateBossBroadcast(Protocol::NOTIFY_REASON_BOSS_REFRESH, m_scene->GetSceneID(), boss_cfg->boss_id, m_boss_list[i].next_flush_time, nullptr);
				}
			}
		}
	}

	if (is_need_sync)
	{
		this->SyncSceneInfo();
	}
}

void SpecialGodmagicBoss::AddExp()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); i++)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			long long add_exp = LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossRewardExp(role->GetLevel());
			if (add_exp <= 0)
			{
				continue;
			}

			role->AddExp(add_exp, "SpecialGodmagicBoss::AddExp", Role::EXP_ADD_REASON_DEFAULT);
		}
	}
}

void SpecialGodmagicBoss::ReduceGatherRoleHp()
{
	if (int(m_user_map.size()) <= 0) return;

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	const GodmagicBossOtherCfg &other_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetOtherCfg();
	for (GodmagicRoleInfoMapIt it = m_user_map.begin(); it != m_user_map.end(); it++)
	{
		if (it->second.is_gather_stauts && it->second.next_reduce_hp_timestamp != 0 && now >= it->second.next_reduce_hp_timestamp)
		{
			UniqueUserID uuid = LongLongToUniqueUserID(it->first);
			Role *role = m_scene->GetRoleByUUID(uuid);
			if (NULL != role)
			{
				Attribute cur_hp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				Attribute reduce_hp = -(long long)(cur_hp / 100 * other_cfg.gather_reduce_hp_precent);
				if (reduce_hp < 0)
				{
					role->ChangeHp(&reduce_hp, true);
					role->NoticeNum(errornum::EN_CROSSBOSS_GATHER_REDUCE_HP_NOTICE);
				}

				it->second.next_reduce_hp_timestamp = now + other_cfg.gather_reduce_hp_cd_s;
			}
		}
	}
}

void SpecialGodmagicBoss::SetGatherStautsFalse(Role * role)
{
	std::map<int, GodmagicRoleInfo>::iterator user_it = m_user_map.find(role->GetUID());

	if (user_it != m_user_map.end() && user_it->second.is_gather_stauts)
	{
		user_it->second.is_gather_stauts = false;
		user_it->second.next_reduce_hp_timestamp = 0;
	}
}

void SpecialGodmagicBoss::OnAddObj(Obj *obj)
{
	if (NULL == obj) return;

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role *)obj;
		if (NULL != role)
		{
			std::map<int, GodmagicRoleInfo>::iterator user_it = m_user_map.find(role->GetUID());

			if (user_it == m_user_map.end()) // 新登录玩家，初始化数据
			{
				GodmagicRoleInfo &info = m_user_map[role->GetUID()];

				info.obj_id = role->GetId();
				info.today_gather_ordinary_crystal_times = role->GetCommonDataParam()->today_gather_ordinary_crystal_num;
				info.today_gather_treasure_crystal_times = role->GetCommonDataParam()->today_gather_treasure_crystal_num;
				info.tire_value = role->GetCommonDataParam()->godmagic_today_kill_boss_num;
				info.is_gather_stauts = false;
				info.next_reduce_hp_timestamp = 0;
			}
			else
			{
				user_it->second.obj_id = role->GetId();
				user_it->second.today_gather_ordinary_crystal_times = role->GetCommonDataParam()->today_gather_ordinary_crystal_num;
				user_it->second.today_gather_treasure_crystal_times = role->GetCommonDataParam()->today_gather_treasure_crystal_num;
				user_it->second.tire_value = role->GetCommonDataParam()->godmagic_today_kill_boss_num;
				user_it->second.is_gather_stauts = false;
				user_it->second.next_reduce_hp_timestamp = 0;
			}

			this->SyncSceneInfo(role);
		}
	}
}

void SpecialGodmagicBoss::OnGather(Role * role, int gather_id, ObjID obj_id, const Posi & pos)
{
	if (NULL == role)
	{
		return;
	}

	if (!role->GetKnapsack()->HaveSpace())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}
	CommonDataParam *comm_p = role->GetCommonDataParam();
	const GodmagicBossOtherCfg &other_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetOtherCfg();
	int treasure_gather_id = LOGIC_CONFIG->GetGodmagicBossConfig().GetTreasureGatherIdBySceneId(this->GetSceneId());

	std::map<int, GodmagicRoleInfo>::iterator user_it = m_user_map.find(role->GetUID());

	int index = 0;
	if (gather_id == other_cfg.ordinary_crystal_gather_id)
	{
		if (user_it != m_user_map.end() && (other_cfg.ordinary_crystal_gather_times - user_it->second.today_gather_ordinary_crystal_times > 0))
		{
			user_it->second.today_gather_ordinary_crystal_times++;
			comm_p->today_gather_ordinary_crystal_num++;
			if (!role->GetKnapsack()->Put(other_cfg.ordinary_crystal_reward_item, PUT_REASON_GODMAGIC_BOSS_GATHER))
			{
				role->NoticeNum(errornum::EN_GUILD_STORAGE_PUT_ERROR);
				return;
			}

			int times_left = other_cfg.ordinary_crystal_gather_times - user_it->second.today_gather_ordinary_crystal_times;
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_boss_gather_times_left, times_left);
			if (length > 0)
			{
				static Protocol::SCSystemMsg sm;
				int sendlen = 0;

				if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_PERSONAL_NOTICE))
				{
					EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&sm, sizeof(sm));
				}
			}

			user_it->second.is_gather_stauts = false;
			user_it->second.next_reduce_hp_timestamp = 0;

			for (int i = 0; i < m_ordinary_crystal_count && i < GODMAGIC_BOSS_MAX_ORDINARY_CRYSTAL; ++i)
			{
				const GodmagicBossOrdinaryCrystalPosCfg *ordinary_crystal_pos = LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossOrdinaryCrystalPosCfg(i + 1);
				if (NULL != ordinary_crystal_pos)
				{
					if (ordinary_crystal_pos->pos.x == pos.x && ordinary_crystal_pos->pos.y == pos.y)
					{
						index = ordinary_crystal_pos->index;
						break;
					}
				}
			}

			if (index > 0 && index <= m_ordinary_crystal_count && index <= GODMAGIC_BOSS_MAX_ORDINARY_CRYSTAL)
			{
				if (index == m_ordinary_crystal_list[index - 1].index)
				{
					m_ordinary_crystal_list[index - 1].is_exist = 0;
					m_ordinary_crystal_list[index - 1].next_flush_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.ordinary_crystal_flush_time_s;
				}
			}
		}
	}

	if (gather_id == treasure_gather_id)
	{
		int layer = LOGIC_CONFIG->GetGodmagicBossConfig().GetLayerIndexBySceneId(this->GetSceneId());
		const GodmagicBossLayerCfg *layer_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossLayerCfg(layer);
		if (NULL == layer_cfg)
		{
			return;
		}

		if (user_it != m_user_map.end() && (other_cfg.treasure_crystal_gather_times - user_it->second.today_gather_treasure_crystal_times > 0))
		{
			user_it->second.today_gather_treasure_crystal_times++;
			comm_p->today_gather_treasure_crystal_num++;

			if (!role->GetKnapsack()->Put(layer_cfg->treasure_crystal_reward_item, PUT_REASON_GODMAGIC_BOSS_GATHER))
			{
				role->NoticeNum(errornum::EN_GUILD_STORAGE_PUT_ERROR);
				return;
			}

			int times_left = other_cfg.treasure_crystal_gather_times - user_it->second.today_gather_treasure_crystal_times;
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_boss_treasure_gather_times_left, times_left);
			if (length > 0)
			{
				static Protocol::SCSystemMsg sm;
				int sendlen = 0;

				if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_PERSONAL_NOTICE))
				{
					EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&sm, sizeof(sm));
				}
			}

			user_it->second.is_gather_stauts = false;
			user_it->second.next_reduce_hp_timestamp = 0;
			m_left_treasure_crystal_num--;

			this->SyncSceneInfo(NULL);
		}
	}

	this->SendInfoToRole(role);
}

void SpecialGodmagicBoss::OnAttackCalHurt(Role *attacker, Monster *monster, Attribute &delta_hp)
{
	if (attacker == nullptr || monster == nullptr)
	{
		return;
	}

	std::map<int, GodmagicRoleInfo>::iterator user_it = m_user_map.find(attacker->GetUID());

	if (user_it == m_user_map.end())
	{
		return;
	}

	if (this->IsGodmagicBossTireFull(attacker))
	{
		if (monster->IsBossMonster())
		{
			attacker->NoticeNum(errornum::EN_BOSS_DONGKU_WEARY_LIMIT);
			delta_hp = 0;
		}
	}
}

void SpecialGodmagicBoss::OnStopGather(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	std::map<int, GodmagicRoleInfo>::iterator user_it = m_user_map.find(role->GetUID());

	if (user_it != m_user_map.end())
	{
		user_it->second.is_gather_stauts = false;
		user_it->second.next_reduce_hp_timestamp = 0;
	}
}

void SpecialGodmagicBoss::SendInfoToRole(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	const GodmagicBossOtherCfg &other_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetOtherCfg();
	std::map<int, GodmagicRoleInfo>::iterator user_it = m_user_map.find(role->GetUID());

	if (user_it != m_user_map.end())
	{
		static Protocol::SCGodmagicBossPlayerInfo  cbpi;

		int left_can_kill_boss_num = other_cfg.daily_boss_num - user_it->second.tire_value;
		cbpi.left_can_kill_boss_num = left_can_kill_boss_num > 0 ? left_can_kill_boss_num : 0;
		int left_ordinary_crystal_gather_times = other_cfg.ordinary_crystal_gather_times - user_it->second.today_gather_ordinary_crystal_times;
		cbpi.left_ordinary_crystal_gather_times = left_ordinary_crystal_gather_times > 0 ? left_ordinary_crystal_gather_times : 0;
		int left_treasure_crystal_gather_times = other_cfg.treasure_crystal_gather_times - user_it->second.today_gather_treasure_crystal_times;
		cbpi.left_treasure_crystal_gather_times = left_treasure_crystal_gather_times > 0 ? left_treasure_crystal_gather_times : 0;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&cbpi, sizeof(cbpi));
	}
}

void SpecialGodmagicBoss::SendBossKillerRecordInfo(Role * role, int boss_id)
{
	if (NULL == role)
	{
		return;
	}

	for (int i = 0; i < MAX_GODMAGIC_BOSS_PER_SCENE; i++)
	{
		if (boss_id != m_boss_list[i].boss_id)
		{
			continue;
		}

		Protocol::SCBossKillerList bkl;

		for (int j = 0; j < GODMAGIC_BOSS_MAX_HISTROY_RECROD && j < KILLER_LIST_MAX_COUNT; j++)
		{
			if (m_boss_list[i].killer_recrod_list[j].uid != 0)
			{
				bkl.killer_info_list[j].killer_uid = m_boss_list[i].killer_recrod_list[j].uid;
				bkl.killer_info_list[j].killier_time = m_boss_list[i].killer_recrod_list[j].dead_timestamp;
				F_STRNCPY(bkl.killer_info_list[j].killer_name, m_boss_list[i].killer_recrod_list[j].killer_name, sizeof(GameName));
			}
		}
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&bkl, sizeof(bkl));

		break;
	}
}

void SpecialGodmagicBoss::OnPickFallingItem(Role *role, int scene_id, unsigned short monster_id, ItemID item_id, const ItemDataWrapper &item_wrapper)
{

}

void SpecialGodmagicBoss::OnRoleLeaveScene(Role *role, bool is_logout)
{
	this->SetGatherStautsFalse(role);
}

void SpecialGodmagicBoss::OnRoleEnterScene(Role * role)
{

}

void SpecialGodmagicBoss::OnStartGather(Role * role, int gather_id, ObjID obj_id, const Posi & pos)
{
	if (NULL == role)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	const GodmagicBossOtherCfg &other_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetOtherCfg();

	std::map<int, GodmagicRoleInfo>::iterator user_it = m_user_map.find(role->GetUID());
	if (user_it != m_user_map.end())
	{
		if (user_it->second.is_gather_stauts && 0 == user_it->second.next_reduce_hp_timestamp)
		{
			user_it->second.next_reduce_hp_timestamp = now + other_cfg.gather_reduce_hp_cd_s;
		}
	}
}

bool SpecialGodmagicBoss::SpecialRelive(Role * role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)   // 回城复活
	{
		role->DieOnReliveTireScene();
	}

	return false;
}

bool SpecialGodmagicBoss::SpecialCanGather(Role * role, GatherObj * gather)
{
	if (NULL == role || NULL == gather)
	{
		return false;
	}

	if (!role->GetKnapsack()->HaveSpace())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	std::map<int, GodmagicRoleInfo>::iterator user_it = m_user_map.find(role->GetUID());
	if (user_it == m_user_map.end()) return false;

	if (gather->GetGatherIngObjId() != INVALID_OBJ_ID && gather->GetGatherIngObjId() != role->GetId())
	{
		unsigned int now_time = (unsigned int)(EngineAdapter::Instance().Time());
		if (now_time > user_it->second.next_notice_gather_timestamp || user_it->second.last_notice_gather_obj_id != gather->GetId())
		{
			role->NoticeNum(errornum::EN_SOMEONE_GATHER_ING);
			user_it->second.last_notice_gather_obj_id = gather->GetId();
			user_it->second.next_notice_gather_timestamp = now_time + 1;
		}
		
		return false;
	}

	const GodmagicBossOtherCfg &other_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetOtherCfg();
	int treasure_gather_id = LOGIC_CONFIG->GetGodmagicBossConfig().GetTreasureGatherIdBySceneId(this->GetSceneId());

	bool can_gather = true;
	if (gather->GatherId() == other_cfg.ordinary_crystal_gather_id)
	{
		if (other_cfg.ordinary_crystal_gather_times - user_it->second.today_gather_ordinary_crystal_times <= 0)
		{
			can_gather = false;
			if (!can_gather)
			{
				role->NoticeNum(errornum::EN_CROSS_BOSS_GATHER_TIME_LIMIT);
			}
		}

		user_it->second.is_gather_stauts = can_gather;

		return can_gather;
	}

	if (gather->GatherId() == treasure_gather_id)
	{
		if (other_cfg.treasure_crystal_gather_times - user_it->second.today_gather_treasure_crystal_times <= 0)
		{
			can_gather = false;
			if (!can_gather)
			{
				role->NoticeNum(errornum::EN_CROSS_BOSS_GATHER_TIME_LIMIT);
			}
		}

		user_it->second.is_gather_stauts = can_gather;

		return can_gather;
	}

	return false;
}

bool SpecialGodmagicBoss::SpecialCanReAlive(Role * role, int realive_type)
{
	if (NULL == role)
	{
		return false;
	}

	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		return role->CanRealiveWithCurTire(); // 免费复活受复活疲劳限制
	}
	else if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_HERE_GOLD == realive_type)
	{
		return true; // 任何时候都允许元宝复活
	}
	else
	{
		return false;
	}
}

bool SpecialGodmagicBoss::AddGodmagicBossTireToDropOwn(Role *role)
{
	if (role == nullptr)
	{
		return false;
	}

	std::map<int, GodmagicRoleInfo>::iterator user_it = m_user_map.find(role->GetUID());

	if (user_it == m_user_map.end())
	{
		return false;
	}

	user_it->second.tire_value++;  // 归属者加疲劳
	role->GetCommonDataParam()->godmagic_today_kill_boss_num++;

	this->SendInfoToRole(role);
	return true;
}

bool SpecialGodmagicBoss::IsGodmagicBossTireFull(Role * role)
{
	if (NULL == role)
	{
		return false;
	}

	const GodmagicBossOtherCfg &other_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetOtherCfg();
	std::map<int, GodmagicRoleInfo>::iterator user_it = m_user_map.find(role->GetUID());

	if (user_it == m_user_map.end())
	{
		return false;
	}

	if (user_it->second.tire_value >= other_cfg.daily_boss_num)
	{
		return true;
	}

	return false;
}

bool SpecialGodmagicBoss::DecGodmagicBossTire(Role * role, int tire_value)
{
	if (NULL == role)
	{
		return false;
	}

	std::map<int, GodmagicRoleInfo>::iterator user_it = m_user_map.find(role->GetUID());

	if (user_it == m_user_map.end())
	{
		return false;
	}

	if (user_it->second.tire_value < tire_value)
	{
		return false;
	}

	user_it->second.tire_value -= tire_value;

	this->SendInfoToRole(role);
	return true;
}

void SpecialGodmagicBoss::GetGodmagicBossInfo(int index, Protocol::SCGodmagicBossInfoAck::BossInfo &boss_info, short &left_treasure_crystal_count, short &left_monster)
{
	if (index < 0 || index >= MAX_GODMAGIC_BOSS_PER_SCENE)
	{
		return;
	}

	boss_info.boss_id = m_boss_list[index].boss_id;
	boss_info.next_flush_time = m_boss_list[index].next_flush_time;
	left_treasure_crystal_count = (short)m_left_treasure_crystal_num;
	left_monster = (short)m_left_monster_count;
}

bool SpecialGodmagicBoss::ConcernBoss(int uid, int boss_id)
{
	std::map<int, std::set<int> >::iterator it = m_boss_register_role.find(boss_id);
	if (it != m_boss_register_role.end())
	{
		std::set<int>::iterator uuid_it = it->second.find(uid);
		if (uuid_it == it->second.end())
		{
			it->second.insert(uid);

			return true;
		}
	}
	else
	{
		std::set<int> tmp_set;
		tmp_set.insert(uid);
		m_boss_register_role[boss_id] = tmp_set;

		return true;
	}

	return false;
}

bool SpecialGodmagicBoss::UnConcernBoss(int uid, int boss_id)
{
	std::map<int, std::set<int> >::iterator it = m_boss_register_role.find(boss_id);
	if (it != m_boss_register_role.end())
	{
		std::set<int>::iterator uuid_it = it->second.find(uid);
		if (uuid_it != it->second.end())
		{
			it->second.erase(uid);

			return true;
		}
	}

	return false;
}

void SpecialGodmagicBoss::SyncSceneInfo(Role *role)
{
	int layer = LOGIC_CONFIG->GetGodmagicBossConfig().GetLayerIndexBySceneId(this->GetSceneId());
	const GodmagicBossLayerCfg *layer_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossLayerCfg(layer);
	if (NULL == layer_cfg)
	{
		return;
	}

	static Protocol::SCGodmagicBossSceneInfo cbpi;

	cbpi.left_monster_count = (short)m_left_monster_count;
	cbpi.left_treasure_crystal_num = (short)m_left_treasure_crystal_num;
	cbpi.layer = (short)layer;
	cbpi.treasure_crystal_gather_id = (short)layer_cfg->treasure_crystal_gather_id;
	cbpi.monster_next_flush_timestamp = m_monster_next_flush_timestamp;
	cbpi.treasure_crystal_next_flush_timestamp = m_treasure_crystal_next_flush_timestamp;

	for (int i = 0; i < MAX_GODMAGIC_BOSS_PER_SCENE; i++)
	{
		cbpi.boss_list[i].boss_id = m_boss_list[i].boss_id;
		cbpi.boss_list[i].is_exist = m_boss_list[i].is_exist;
		cbpi.boss_list[i].next_flush_time = m_boss_list[i].next_flush_time;
	}

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&cbpi, sizeof(cbpi));
	}
	else
	{
		const int role_num = (int)m_scene->RoleNum();
		for (int i = 0; i < role_num; ++i)
		{
			Role *temp_role = m_scene->GetRoleByIndex(i);
			if (NULL != temp_role)
			{
				EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (char*)&cbpi, sizeof(cbpi));
			}
		}
	}
}

void SpecialGodmagicBoss::CreateBossBroadcast(int notify_reason, int scene_id, int boss_id, unsigned int next_refresh_time, Role *drop_owner_role)
{
	Protocol::SCBossInfoToAll bita;
	bita.notify_reason = notify_reason;
	bita.boss_type = Protocol::BOSS_ENTER_TYPE_GODMAGIC_BOSS;
	bita.boss_id = boss_id;
	bita.next_refresh_time = next_refresh_time;
	bita.scene_id = m_scene->GetSceneID();
	bita.status = ENBOSSSTATUS_NONEXIST;
	if (bita.next_refresh_time == 0)
	{
		bita.status = ENBOSSSTATUS_EXIST;
	}
	bita.killer_uid = 0;
	if (drop_owner_role != nullptr)
	{
		bita.killer_uid = drop_owner_role->GetUID();
	}
	//std::map<int, std::set<long long> >::iterator it = m_boss_register_role.find(boss_id);
	//if (it != m_boss_register_role.end())
	//{
	//	for (std::set<long long>::iterator uuid_it = it->second.begin(); uuid_it != it->second.end(); uuid_it++)
	//	{
	//		Role* role = m_scene->GetRoleByUUID(LongLongToUniqueUserID(*uuid_it));
	//		if (role != nullptr)
	//		{
	//			EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&bita, sizeof(bita));
	//		}
	//		else
	//		{
	//			InternalComm::Instance().UserProtocalTransferToGame(*uuid_it, (const char *)&bita, sizeof(bita));
	//		}
	//	}
	//}

	World::GetInstWorld()->GetSceneManager()->SendToAllUser((const char *)&bita, sizeof(bita));

	// 刷新传闻
	//int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_boss_refresh_boss_content, m_scene->GetSceneID());
	//if (length > 0)
	//{
	//	World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
	//}
}

