#include "specialcrossyoumingboss.hpp"
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
#include "other/cross/rolecross.h"
#include "global/team/teammanager.hpp"
#include "obj/otherobj/gatherobj.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "friend/teaminfo.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "protocal/msgfb.h"
#include "servercommon/serverconfig/crossyoumingbossconfig.hpp"

SpecialCrossYouMingBoss::SpecialCrossYouMingBoss(Scene *scene)
	: SpecialLogic(scene), m_first(true), m_boss_count(0), m_monster_count(0), m_left_monster_count(0), m_ordinary_crystal_count(0), m_left_treasure_crystal_num(0), m_boss_next_flush_timestamp(0),
	m_monster_next_flush_timestamp(0), m_next_add_exp_timestamp(0), m_treasure_crystal_next_flush_timestamp(0), m_ordinary_crystal_next_flush_timestamp(0)
{
}

SpecialCrossYouMingBoss::~SpecialCrossYouMingBoss()
{
}

void SpecialCrossYouMingBoss::Update(unsigned long interval, time_t now_second)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	SpecialLogic::Update(interval, now_second);

	const CrossYoumingBossOtherCfg &other_cfg = CrossYoumingBossConfig::Instance().GetOtherCfg();

	if (m_first)
	{
		int layer = CrossYoumingBossConfig::Instance().GetLayerIndexBySceneId(this->GetSceneId());
		if (layer <= 0 || layer > CROSS_YOUMING_BOSS_SCENE_MAX)
		{
			return;
		}
		m_boss_count = CrossYoumingBossConfig::Instance().GetCrossBossBossCount(layer);
		m_monster_count = CrossYoumingBossConfig::Instance().GetCrossBossMonsterCount(layer);
		m_ordinary_crystal_count = CrossYoumingBossConfig::Instance().GetOrdinaryCrystalCount();
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
		this->RefreshOrdinaryCrystal();
		m_ordinary_crystal_next_flush_timestamp = static_cast<unsigned int>(now_second) + 5;
	}

	if (now_second >= m_monster_next_flush_timestamp)
	{
		this->RefreshMonster();
		m_monster_next_flush_timestamp = static_cast<unsigned int>(now_second) + 5;
	}

	if (now_second >= m_next_add_exp_timestamp)
	{
		this->AddExp();
		m_next_add_exp_timestamp = static_cast<unsigned int>(now_second) + other_cfg.reward_exp_cd_s;
	}

	if (now_second >= m_treasure_crystal_next_flush_timestamp)
	{
		this->RefreshTreasureCrystal();
		m_treasure_crystal_next_flush_timestamp = static_cast<unsigned int>(now_second) + other_cfg.treasure_crystal_flush_time_s;
		is_need_sync = true;
	}

	if (is_need_sync)
	{
		this->SyncSceneInfo();
	}
}

void SpecialCrossYouMingBoss::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	const int role_num = (int)m_scene->RoleNum();
	for (int i = 0; i < role_num; ++i)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);
		if (NULL != temp_role)
		{
			long long uuid = UniqueUserIDToLongLong(temp_role->GetRoleCross()->GetCrossOriginalUniqueUserID());
			std::map<long long, RoleCrossInfo>::iterator user_it = m_user_map.find(uuid);

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

void SpecialCrossYouMingBoss::OnActivityOpen()
{
}

void SpecialCrossYouMingBoss::OnActivityClose()
{
}

void SpecialCrossYouMingBoss::OnCharacterDie(Character *dead_cha, ObjID killer_objid)
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
		dead_role->DieOnCrossYoumingBossScene(true);
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
			long long uuid = UniqueUserIDToLongLong(killer_role->GetRoleCross()->GetCrossOriginalUniqueUserID());
			bool is_need_sync = false;
			unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			for (int i = 0; i < m_boss_count && i < MAX_CROSS_YOUMING_BOSS_PER_SCENE; i++)
			{
				if ((m_boss_list[i].boss_id == monster->GetMonsterId()) && (1 == m_boss_list[i].is_exist))
				{
					m_boss_list[i].is_exist = 0;
					m_boss_list[i].next_flush_time = now_time + CrossYoumingBossConfig::Instance().GetCrossBossNextFlushTimeInterval(now_time);

					int record_index = m_boss_list[i].record_cur_index % CROSS_BOSS_MAX_HISTROY_RECROD;
					m_boss_list[i].killer_recrod_list[record_index].dead_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
					killer_role->GetName(m_boss_list[i].killer_recrod_list[record_index].killer_name);
					m_boss_list[i].killer_recrod_list[record_index].uuid = uuid;

					m_boss_list[i].record_cur_index = record_index + 1;

					if (m_boss_list[i].record_count < CROSS_BOSS_MAX_HISTROY_RECROD)
					{
						++m_boss_list[i].record_count;
					}

					is_need_sync = true;
					break;
				}
			}

			const CrossYoumingBossOtherCfg &other_cfg = CrossYoumingBossConfig::Instance().GetOtherCfg();
			for (int i = 0; i < m_monster_count && i < CROSS_YOUMING_BOSS_MAX_MONSTER_NUM; i++)
			{
				if ((m_monster_list[i].monster_id == monster->GetMonsterId()) && (1 == m_monster_list[i].is_exist))
				{
					int layer = CrossYoumingBossConfig::Instance().GetLayerIndexBySceneId(this->GetSceneId());
					const CrossYoumingBossMonsterCfg::MonsterCfg *monster_cfg = CrossYoumingBossConfig::Instance().GetCrossBossMonsterCfgByLayer(layer, i);
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

void SpecialCrossYouMingBoss::RefreshTreasureCrystal()
{
	this->CleanAllTreasureCrystal();

	int layer = CrossYoumingBossConfig::Instance().GetLayerIndexBySceneId(this->GetSceneId());
	const CrossYoumingBossTreasureCrystalPosCfg *point_list = CrossYoumingBossConfig::Instance().GetTreasureCrystalPointListByLayer(layer);
	if (NULL == point_list)
	{
		return;
	}

	int cfg_point_num = point_list->pos_num;
	const CrossYoumingBossOtherCfg &other_cfg = CrossYoumingBossConfig::Instance().GetOtherCfg();
	int treasure_crystal_num = other_cfg.treasure_crystal_flush_num;
	int gather_id = CrossYoumingBossConfig::Instance().GetTreasureGatherIdBySceneId(this->GetSceneId());

	int index = 0;
	std::set<int> index_set; index_set.clear();
	if (cfg_point_num > 0)
	{
		for (int i = 0; i < cfg_point_num && i < CROSS_YOUMING_BOSS_MAX_TREASURE_CRYSTAL_POS_NUM; i++)
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

void SpecialCrossYouMingBoss::CleanAllTreasureCrystal()
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

void SpecialCrossYouMingBoss::RefreshOrdinaryCrystal()
{
	bool is_need_sync = false;
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	const CrossYoumingBossOtherCfg &other_cfg = CrossYoumingBossConfig::Instance().GetOtherCfg();
	for (int i = 0; i < m_ordinary_crystal_count && i < CROSS_YOUMING_BOSS_MAX_ORDINARY_CRYSTAL; i++)
	{
		if (0 == m_ordinary_crystal_list[i].is_exist && now >= m_ordinary_crystal_list[i].next_flush_time)
		{
			const CrossYoumingBossOrdinaryCrystalPosCfg *ordinary_crystal_pos = CrossYoumingBossConfig::Instance().GetCrossBossOrdinaryCrystalPosCfg(i + 1);
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

void SpecialCrossYouMingBoss::RefreshMonster()
{
	bool is_need_sync = false;
	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	for (int i = 0; i < m_monster_count && i < CROSS_YOUMING_BOSS_MAX_MONSTER_NUM; i++)
	{
		if (0 == m_monster_list[i].is_exist && now_time >= m_monster_list[i].next_flush_time)
		{
			int layer = CrossYoumingBossConfig::Instance().GetLayerIndexBySceneId(this->GetSceneId());
			const CrossYoumingBossMonsterCfg::MonsterCfg *monster_cfg = CrossYoumingBossConfig::Instance().GetCrossBossMonsterCfgByLayer(layer, i);
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

void SpecialCrossYouMingBoss::RefreshBoss()
{
	bool is_need_sync = false;
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; i < m_boss_count && i < MAX_CROSS_YOUMING_BOSS_PER_SCENE; i++)
	{
		if (0 == m_boss_list[i].is_exist && (!m_boss_list[i].has_notice_role) && (int(m_boss_list[i].next_flush_time - now)) <= SECOND_PER_MIN)
		{
			m_boss_list[i].has_notice_role = true;
		}
		else if (0 == m_boss_list[i].is_exist && now >= m_boss_list[i].next_flush_time)
		{
			int layer = CrossYoumingBossConfig::Instance().GetLayerIndexBySceneId(this->GetSceneId());
			const CrossYoumingBossBossCfg::BossCfg *boss_cfg = CrossYoumingBossConfig::Instance().GetCrossBossBossCfgByLayer(layer, i);
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
				}
			}
		}
	}

	if (is_need_sync)
	{
		this->SyncSceneInfo();
	}
}

void SpecialCrossYouMingBoss::AddExp()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); i++)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			long long add_exp = CrossYoumingBossConfig::Instance().GetCrossBossRewardExp(role->GetLevel());
			if (add_exp <= 0)
			{
				continue;
			}

			role->AddExp(add_exp, "SpecialCrossYouMingBoss::AddExp", Role::EXP_ADD_REASON_DEFAULT);
		}
	}
}

void SpecialCrossYouMingBoss::ReduceGatherRoleHp()
{
	if (int(m_user_map.size()) <= 0) return;

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	const CrossYoumingBossOtherCfg &other_cfg = CrossYoumingBossConfig::Instance().GetOtherCfg();
	for (RoleCrossInfoMapIt it = m_user_map.begin(); it != m_user_map.end(); it++)
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

void SpecialCrossYouMingBoss::SetGatherStautsFalse(Role * role)
{
	long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	std::map<long long, RoleCrossInfo>::iterator user_it = m_user_map.find(uuid);

	if (user_it != m_user_map.end() && user_it->second.is_gather_stauts)
	{
		user_it->second.is_gather_stauts = false;
		user_it->second.next_reduce_hp_timestamp = 0;
	}
}

void SpecialCrossYouMingBoss::OnAddObj(Obj *obj)
{
	if (NULL == obj) return;

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role *)obj;
		if (NULL != role)
		{
			long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
			std::map<long long, RoleCrossInfo>::iterator user_it = m_user_map.find(uuid);

			if (user_it == m_user_map.end()) // 新登录玩家，初始化数据
			{
				RoleCrossInfo &info = m_user_map[uuid];

				info.obj_id = role->GetId();
				info.today_gather_ordinary_crystal_times = role->GetRoleCross()->GetCrossYoumingBossTodayGatherOrdinaryCrystalNum();
				info.today_gather_treasure_crystal_times = role->GetRoleCross()->GetCrossYoumingBossTodayGatherTreasureCrystalNum();
				info.tire_value = role->GetRoleCross()->GetCrossYoumingBossTodayKillBossNum();
				info.is_gather_stauts = false;
				info.next_reduce_hp_timestamp = 0;
			}
			else
			{
				user_it->second.obj_id = role->GetId();
				user_it->second.today_gather_ordinary_crystal_times = role->GetRoleCross()->GetCrossYoumingBossTodayGatherOrdinaryCrystalNum();
				user_it->second.today_gather_treasure_crystal_times = role->GetRoleCross()->GetCrossYoumingBossTodayGatherTreasureCrystalNum();
				user_it->second.tire_value = role->GetRoleCross()->GetCrossYoumingBossTodayKillBossNum();
				user_it->second.is_gather_stauts = false;
				user_it->second.next_reduce_hp_timestamp = 0;
			}

			this->SyncSceneInfo(role);
		}
	}
}

void SpecialCrossYouMingBoss::OnGather(Role * role, int gather_id, ObjID obj_id, const Posi & pos)
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

	const CrossYoumingBossOtherCfg &other_cfg = CrossYoumingBossConfig::Instance().GetOtherCfg();
	int treasure_gather_id = CrossYoumingBossConfig::Instance().GetTreasureGatherIdBySceneId(this->GetSceneId());

	long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	std::map<long long, RoleCrossInfo>::iterator user_it = m_user_map.find(uuid);

	int index = 0;
	if (gather_id == other_cfg.ordinary_crystal_gather_id)
	{
		if (user_it != m_user_map.end() && (other_cfg.ordinary_crystal_gather_times - user_it->second.today_gather_ordinary_crystal_times > 0))
		{
			user_it->second.today_gather_ordinary_crystal_times++;
			if (!role->GetKnapsack()->Put(other_cfg.ordinary_crystal_reward_item, PUT_REASON_CROSS_YOUMING_BOSS_GATHER))
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

			for (int i = 0; i < m_ordinary_crystal_count && i < CROSS_YOUMING_BOSS_MAX_ORDINARY_CRYSTAL; ++i)
			{
				const CrossYoumingBossOrdinaryCrystalPosCfg *ordinary_crystal_pos = CrossYoumingBossConfig::Instance().GetCrossBossOrdinaryCrystalPosCfg(i + 1);
				if (NULL != ordinary_crystal_pos)
				{
					if (ordinary_crystal_pos->pos.x == pos.x && ordinary_crystal_pos->pos.y == pos.y)
					{
						index = ordinary_crystal_pos->index;
						break;
					}
				}
			}

			if (index > 0 && index <= m_ordinary_crystal_count && index <= CROSS_YOUMING_BOSS_MAX_ORDINARY_CRYSTAL)
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
		int layer = CrossYoumingBossConfig::Instance().GetLayerIndexBySceneId(this->GetSceneId());
		const CrossYoumingBossLayerCfg *layer_cfg = CrossYoumingBossConfig::Instance().GetCrossBossLayerCfg(layer);
		if (NULL == layer_cfg)
		{
			return;
		}

		if (user_it != m_user_map.end() && (other_cfg.treasure_crystal_gather_times - user_it->second.today_gather_treasure_crystal_times > 0))
		{
			user_it->second.today_gather_treasure_crystal_times++;
			if (!role->GetKnapsack()->Put(layer_cfg->treasure_crystal_reward_item, PUT_REASON_CROSS_YOUMING_BOSS_GATHER))
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

void SpecialCrossYouMingBoss::OnAttackCalHurt(Role *attacker, Monster *monster, Attribute &delta_hp)
{
	if (attacker == nullptr || monster == nullptr)
	{
		return;
	}

	//const CrossYoumingBossOtherCfg &other_cfg = CrossYoumingBossConfig::Instance().GetOtherCfg();
	long long uuid = UniqueUserIDToLongLong(attacker->GetRoleCross()->GetCrossOriginalUniqueUserID());
	std::map<long long, RoleCrossInfo>::iterator user_it = m_user_map.find(uuid);

	if (user_it == m_user_map.end())
	{
		return;
	}

	if (this->IsCrossBossTireFull(attacker))
	{
		if (monster->IsBossMonster())
		{
			attacker->NoticeNum(errornum::EN_BOSS_DONGKU_WEARY_LIMIT);
			delta_hp = 0;
		}
	}
}

void SpecialCrossYouMingBoss::OnStopGather(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	std::map<long long, RoleCrossInfo>::iterator user_it = m_user_map.find(uuid);

	if (user_it != m_user_map.end())
	{
		user_it->second.is_gather_stauts = false;
		user_it->second.next_reduce_hp_timestamp = 0;
	}
}

void SpecialCrossYouMingBoss::SendInfoToRole(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	const CrossYoumingBossOtherCfg &other_cfg = CrossYoumingBossConfig::Instance().GetOtherCfg();
	long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	std::map<long long, RoleCrossInfo>::iterator user_it = m_user_map.find(uuid);

	if (user_it != m_user_map.end())
	{
		static Protocol::SCCrossYouMingBossPlayerInfo  cbpi;

		int left_can_kill_boss_num = user_it->second.tire_value;
		cbpi.left_can_kill_boss_num = left_can_kill_boss_num > 0 ? left_can_kill_boss_num : 0;
		int left_ordinary_crystal_gather_times = other_cfg.ordinary_crystal_gather_times - user_it->second.today_gather_ordinary_crystal_times;
		cbpi.left_ordinary_crystal_gather_times = left_ordinary_crystal_gather_times > 0 ? left_ordinary_crystal_gather_times : 0;
		int left_treasure_crystal_gather_times = other_cfg.treasure_crystal_gather_times - user_it->second.today_gather_treasure_crystal_times;
		cbpi.left_treasure_crystal_gather_times = left_treasure_crystal_gather_times > 0 ? left_treasure_crystal_gather_times : 0;
		role->GetRoleCross()->GetCrossYoumingBossConcernFlag(cbpi.concern_flag);

		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&cbpi, sizeof(cbpi));
	}
}

void SpecialCrossYouMingBoss::SendDropRecordInfo(Role * role)
{
	if (NULL == role)
	{
		return;
	}
	std::deque<CrossBossDropRecordItem> &m_drop_record_list = WorldStatus2::Instance().GetCrossBossDropRecord();

	static Protocol::SCCrossBossDropRecord cbdr;
	cbdr.record_count = 0;
	for (std::deque<CrossBossDropRecordItem>::const_iterator it = m_drop_record_list.begin(); it != m_drop_record_list.end(); ++it)
	{
		if (cbdr.record_count >= CROSS_BOSS_MAX_DROP_RECROD)
		{
			break;
		}

		cbdr.dorp_record_list[cbdr.record_count].uuid = it->uuid;
		F_STRNCPY(cbdr.dorp_record_list[cbdr.record_count].role_name, it->name, sizeof(GameName));
		cbdr.dorp_record_list[cbdr.record_count].pickup_timestamp = it->pickup_timestamp;
		cbdr.dorp_record_list[cbdr.record_count].scene_id = it->scene_id;
		cbdr.dorp_record_list[cbdr.record_count].monster_id = it->monster_id;
		cbdr.dorp_record_list[cbdr.record_count].item_id = it->item_id;
		cbdr.dorp_record_list[cbdr.record_count].item_num = it->item_wrapper.num;
		ARRAYCPY(cbdr.dorp_record_list[cbdr.record_count].xianpin_type_list, it->item_wrapper.param_data.xianpin_type_list);
		cbdr.record_count++;
	}

	int length = sizeof(cbdr) - (CROSS_BOSS_MAX_DROP_RECROD - cbdr.record_count) * sizeof(cbdr.dorp_record_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&cbdr, length);
}

void SpecialCrossYouMingBoss::SendBossKillerRecordInfo(Role * role, int boss_id)
{
	if (NULL == role)
	{
		return;
	}

	for (int i = 0; i < MAX_CROSS_YOUMING_BOSS_PER_SCENE; i++)
	{
		if (boss_id != m_boss_list[i].boss_id)
		{
			continue;
		}

		static Protocol::SCCrossBossBossKillRecord cbbkr;

		cbbkr.record_count = 0;

		for (int j = 0; j < CROSS_BOSS_MAX_HISTROY_RECROD && cbbkr.record_count < CROSS_BOSS_MAX_HISTROY_RECROD; j++)
		{
			if (m_boss_list[i].killer_recrod_list[j].uuid != 0)
			{
				cbbkr.killer_record_list[cbbkr.record_count].uuid = m_boss_list[i].killer_recrod_list[j].uuid;
				cbbkr.killer_record_list[cbbkr.record_count].dead_timestamp = m_boss_list[i].killer_recrod_list[j].dead_timestamp;
				F_STRNCPY(cbbkr.killer_record_list[cbbkr.record_count].killer_name, m_boss_list[i].killer_recrod_list[j].killer_name, sizeof(GameName));

				cbbkr.record_count++;
			}
		}

		int length = sizeof(cbbkr) - (CROSS_BOSS_MAX_HISTROY_RECROD - cbbkr.record_count) * sizeof(cbbkr.killer_record_list[0]);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&cbbkr, length);

		break;
	}
}

void SpecialCrossYouMingBoss::OnSyncDataToCrossSever(crossgameprotocal::GameGameBossInfoReq *ggbir)
{
	if (NULL == ggbir)
	{
		return;
	}

	switch (ggbir->req_type)
	{
	case Protocol::CROSS_YOUMING_BOSS_OPERA_TYPE_GET_FLUSH_INFO:
	{
		int layer = CrossYoumingBossConfig::Instance().GetLayerIndexBySceneId(this->GetSceneId());
		const CrossYoumingBossLayerCfg *layer_cfg = CrossYoumingBossConfig::Instance().GetCrossBossLayerCfg(layer);
		if (NULL == layer_cfg)
		{
			return;
		}

		static Protocol::SCCrossYouMingBossSceneInfo cbpi;

		cbpi.left_monster_count = (short)m_left_monster_count;
		cbpi.left_treasure_crystal_num = (short)m_left_treasure_crystal_num;
		cbpi.layer = (short)layer;
		cbpi.treasure_crystal_gather_id = (short)layer_cfg->treasure_crystal_gather_id;
		cbpi.monster_next_flush_timestamp = m_monster_next_flush_timestamp;
		cbpi.treasure_crystal_next_flush_timestamp = m_treasure_crystal_next_flush_timestamp;

		for (int i = 0; i < MAX_CROSS_YOUMING_BOSS_PER_SCENE; i++)
		{
			cbpi.boss_list[i].boss_id = m_boss_list[i].boss_id;
			cbpi.boss_list[i].is_exist = m_boss_list[i].is_exist;
			cbpi.boss_list[i].next_flush_time = m_boss_list[i].next_flush_time;
		}

		InternalComm::Instance().UserProtocalTransferToGame(ggbir->uuid, (const char *)&cbpi, sizeof(cbpi));
	}
	break;

	case Protocol::CROSS_YOUMING_BOSS_OPERA_TYPE_BOSS_KILL_RECORD:
	{
		for (int i = 0; i < MAX_CROSS_YOUMING_BOSS_PER_SCENE; i++)
		{
			if (ggbir->param_2 != m_boss_list[i].boss_id)
			{
				continue;
			}

			static Protocol::SCCrossBossBossKillRecord cbbkr;

			cbbkr.record_count = 0;

			for (int j = 0; j < CROSS_BOSS_MAX_HISTROY_RECROD && cbbkr.record_count < CROSS_BOSS_MAX_HISTROY_RECROD; j++)
			{
				if (m_boss_list[i].killer_recrod_list[j].uuid != 0)
				{
					cbbkr.killer_record_list[cbbkr.record_count].uuid = m_boss_list[i].killer_recrod_list[j].uuid;
					cbbkr.killer_record_list[cbbkr.record_count].dead_timestamp = m_boss_list[i].killer_recrod_list[j].dead_timestamp;
					F_STRNCPY(cbbkr.killer_record_list[cbbkr.record_count].killer_name, m_boss_list[i].killer_recrod_list[j].killer_name, sizeof(GameName));

					cbbkr.record_count++;
				}
			}

			InternalComm::Instance().UserProtocalTransferToGame(ggbir->uuid, (const char *)&cbbkr, sizeof(cbbkr));
			break;
		}
	}
	break;

	default:
		break;
	}
}

void SpecialCrossYouMingBoss::OnPickFallingItem(Role *role, int scene_id, unsigned short monster_id, ItemID item_id, const ItemDataWrapper &item_wrapper)
{
	if (NULL == role)
	{
		return;
	}

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	const MonsterInitParam *monster_cfg = MONSTERPOOL->GetMonsterParam(monster_id);
	if (NULL == monster_cfg)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	std::map<long long, RoleCrossInfo>::iterator user_it = m_user_map.find(uuid);

	if (user_it == m_user_map.end())
	{
		return;
	}

	std::deque<CrossBossDropRecordItem> &m_drop_record_list = WorldStatus2::Instance().GetCrossBossDropRecord();

	if (MonsterInitParam::MONSTER_TYPE_BOSS == monster_cfg->monster_type && MonsterInitParam::BOSS_TYPE_CROSS_BOSS == monster_cfg->boss_type)
	{
		CrossBossDropRecordItem drop_item;
		drop_item.uuid = uuid;
		role->GetName(drop_item.name);
		drop_item.scene_id = scene_id;
		drop_item.monster_id = monster_id;
		drop_item.item_id = item_id;
		drop_item.item_wrapper = item_wrapper;
		drop_item.pickup_timestamp = (unsigned int)EngineAdapter::Instance().Time();
		m_drop_record_list.push_back(drop_item);

		if (m_drop_record_list.size() > CROSS_BOSS_MAX_DROP_RECROD * 2)
		{
			m_drop_record_list.pop_front();
		}
	}
}

void SpecialCrossYouMingBoss::OnRoleLeaveScene(Role *role, bool is_logout)
{
	this->SetGatherStautsFalse(role);
	this->AutoPickFallingItem(role);
	this->SyncResultToCross(role);
}

void SpecialCrossYouMingBoss::OnRoleEnterScene(Role * role)
{

}

void SpecialCrossYouMingBoss::OnStartGather(Role * role, int gather_id, ObjID obj_id, const Posi & pos)
{
	if (NULL == role)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	const CrossYoumingBossOtherCfg &other_cfg = CrossYoumingBossConfig::Instance().GetOtherCfg();

	long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	std::map<long long, RoleCrossInfo>::iterator user_it = m_user_map.find(uuid);
	if (user_it != m_user_map.end())
	{
		if (user_it->second.is_gather_stauts && 0 == user_it->second.next_reduce_hp_timestamp)
		{
			user_it->second.next_reduce_hp_timestamp = now + other_cfg.gather_reduce_hp_cd_s;
		}
	}
}

bool SpecialCrossYouMingBoss::SpecialRelive(Role * role, int realive_type)
{
	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)   // 回城复活
	{
		role->DieOnReliveTireScene();
	}

	return false;
}

bool SpecialCrossYouMingBoss::SpecialCanGather(Role * role, GatherObj * gather)
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

	long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	std::map<long long, RoleCrossInfo>::iterator user_it = m_user_map.find(uuid);

	if (gather->GetGatherIngObjId() != INVALID_OBJ_ID && gather->GetGatherIngObjId() != role->GetId())
	{
		if (user_it != m_user_map.end())
		{
			unsigned int now_time = (unsigned int)(EngineAdapter::Instance().Time());
			if (now_time > user_it->second.next_notice_gather_timestamp || user_it->second.last_notice_gather_obj_id != gather->GetId())
			{
				role->NoticeNum(errornum::EN_SOMEONE_GATHER_ING);
				user_it->second.last_notice_gather_obj_id = gather->GetId();
				user_it->second.next_notice_gather_timestamp = now_time + 1;
			}
		}
		return false;
	}

	const CrossYoumingBossOtherCfg &other_cfg = CrossYoumingBossConfig::Instance().GetOtherCfg();
	int treasure_gather_id = CrossYoumingBossConfig::Instance().GetTreasureGatherIdBySceneId(this->GetSceneId());

	bool can_gather = true;
	if (gather->GatherId() == other_cfg.ordinary_crystal_gather_id)
	{
		if (user_it != m_user_map.end() && (other_cfg.ordinary_crystal_gather_times - user_it->second.today_gather_ordinary_crystal_times <= 0))
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
		if (user_it != m_user_map.end() && (other_cfg.treasure_crystal_gather_times - user_it->second.today_gather_treasure_crystal_times <= 0))
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

bool SpecialCrossYouMingBoss::SpecialCanReAlive(Role * role, int realive_type)
{
	if (NULL == role)
	{
		return false;
	}

	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		return role->CanCrossYoumingBossRealiveWithCurTire(); // 免费复活受复活疲劳限制
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

bool SpecialCrossYouMingBoss::AddCrossBossTireToDropOwn(Role *role)
{
	if (role == nullptr)
	{
		return false;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	std::map<long long, RoleCrossInfo>::iterator user_it = m_user_map.find(uuid);

	if (user_it == m_user_map.end())
	{
		return false;
	}

	user_it->second.tire_value++;  // 归属者加疲劳
	this->SendInfoToRole(role);

	return true;
}

bool SpecialCrossYouMingBoss::IsCrossBossTireFull(Role * role)
{
	if (NULL == role)
	{
		return false;
	}

	const CrossYoumingBossOtherCfg &other_cfg = CrossYoumingBossConfig::Instance().GetOtherCfg();
	long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	std::map<long long, RoleCrossInfo>::iterator user_it = m_user_map.find(uuid);

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

bool SpecialCrossYouMingBoss::DecCrossBossTire(Role * role, int tire_value)
{
	if (NULL == role)
	{
		return false;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	std::map<long long, RoleCrossInfo>::iterator user_it = m_user_map.find(uuid);

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

void SpecialCrossYouMingBoss::GetCrossBossInfo(int index, Protocol::SCCrossYouMingBossBossInfoAck::BossInfo &boss_info, short &left_treasure_crystal_count, short &left_monster)
{
	if (index < 0 || index >= MAX_CROSS_YOUMING_BOSS_PER_SCENE)
	{
		return;
	}

	boss_info.boss_id = m_boss_list[index].boss_id;
	boss_info.next_flush_time = m_boss_list[index].next_flush_time;
	left_treasure_crystal_count = (short)m_left_treasure_crystal_num;
	left_monster = (short)m_left_monster_count;
}

bool SpecialCrossYouMingBoss::ConcernBoss(long long uuid, int boss_id)
{
	std::map<int, std::set<long long> >::iterator it = m_boss_register_role.find(boss_id);
	if (it != m_boss_register_role.end())
	{
		std::set<long long>::iterator uuid_it = it->second.find(uuid);
		if (uuid_it == it->second.end())
		{
			it->second.insert(uuid);

			return true;
		}
	}
	else
	{
		std::set<long long> tmp_set;
		tmp_set.insert(uuid);
		m_boss_register_role[boss_id] = tmp_set;

		return true;
	}

	return false;
}

bool SpecialCrossYouMingBoss::UnConcernBoss(long long uuid, int boss_id)
{
	std::map<int, std::set<long long> >::iterator it = m_boss_register_role.find(boss_id);
	if (it != m_boss_register_role.end())
	{
		std::set<long long>::iterator uuid_it = it->second.find(uuid);
		if (uuid_it != it->second.end())
		{
			it->second.erase(uuid);

			return true;
		}
	}

	return false;
}

void SpecialCrossYouMingBoss::SyncSceneInfo(Role *role)
{
	int layer = CrossYoumingBossConfig::Instance().GetLayerIndexBySceneId(this->GetSceneId());
	const CrossYoumingBossLayerCfg *layer_cfg = CrossYoumingBossConfig::Instance().GetCrossBossLayerCfg(layer);
	if (NULL == layer_cfg)
	{
		return;
	}

	static Protocol::SCCrossYouMingBossSceneInfo cbpi;

	cbpi.left_monster_count = (short)m_left_monster_count;
	cbpi.left_treasure_crystal_num = (short)m_left_treasure_crystal_num;
	cbpi.layer = (short)layer;
	cbpi.treasure_crystal_gather_id = (short)layer_cfg->treasure_crystal_gather_id;
	cbpi.monster_next_flush_timestamp = m_monster_next_flush_timestamp;
	cbpi.treasure_crystal_next_flush_timestamp = m_treasure_crystal_next_flush_timestamp;

	for (int i = 0; i < MAX_CROSS_YOUMING_BOSS_PER_SCENE; i++)
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

void SpecialCrossYouMingBoss::SyncResultToCross(Role *role)
{
	long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	std::map<long long, RoleCrossInfo>::iterator user_it = m_user_map.find(uuid);

	if (user_it != m_user_map.end()) // 把需要保存的信息传回原服
	{
		user_it->second.obj_id = 0;

		{
			crossgameprotocal::GameCrossBossSyncPlayerInfo player_info;

			player_info.cross_boss_type = crossgameprotocal::CROSS_BOSS_TYPE_YOUMING;
			player_info.uuid = uuid;
			player_info.today_gather_ordinary_crystal_times = user_it->second.today_gather_ordinary_crystal_times;
			player_info.today_gather_treasure_crystal_times = user_it->second.today_gather_treasure_crystal_times;
			player_info.tire_value = user_it->second.tire_value;
			role->GetRoleCross()->GetCrossYoumingBossConcernFlag(player_info.concern_flag);
			InternalComm::Instance().SendToCross((const char *)&player_info, sizeof(player_info));
		}

		role->BackToOriginalServer();
	}
}

void SpecialCrossYouMingBoss::CreateBossBroadcast(int notify_reason, int scene_id, int boss_id, unsigned int next_refresh_time, Role *drop_owner_role)
{
	Protocol::SCBossInfoToAll bita;
	bita.notify_reason = notify_reason;
	bita.boss_type = Protocol::BOSS_TYPE_CROSS_YOUMING_BOSS;
	bita.boss_id = boss_id;
	bita.next_refresh_time = next_refresh_time;
	bita.scene_id = m_scene->GetSceneID();
	bita.status = ENBOSSSTATUS_NONEXIST;
	if (bita.next_refresh_time != 0)
	{
		bita.status = ENBOSSSTATUS_EXIST;
	}
	bita.killer_uid = 0;
	if (drop_owner_role != nullptr)
	{
		bita.killer_uid = drop_owner_role->GetUID();
	}

	InternalComm::Instance().UserProtocalTransferToGame(0, (const char *)&bita, sizeof(bita));

	// 刷新传闻
	//int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cross_boss_refresh_boss3_content, m_scene->GetSceneID());
	//if (length > 0)
	//{
	//	World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
	//}
}

