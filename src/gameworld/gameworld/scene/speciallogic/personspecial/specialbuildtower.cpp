#include "specialbuildtower.hpp"
#include "servercommon/servercommon.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "obj/character/monster.h"
#include "obj/obj.h"
#include "protocal/msgfb.h"
#include "monster/monsterpool.h"
#include "other/event/eventhandler.hpp"
#include "gamecommon.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "ai/monsterai/specialai/monsterairunningmonster.hpp"
#include "obj/otherobj/fallingitem.h"
#include <algorithm>
#include "monster/drop/droppool.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "gameworld/other/fb/buildtowerfbconfig.hpp"
#include "gameworld/item/knapsack.h"
#include "gameworld/global/droplog/droplogmanager.hpp"
#include "gameworld/effect/skilleffect/effectbuff.hpp"
#include "gameworld/obj/character/attribute.hpp"

SpecialBuildTower::SpecialBuildTower(Scene *scene)
: SpecialLogic(scene), m_owner_objid(INVALID_OBJ_ID), m_owner_user_id(INVALID_USER_ID), m_role_gongji(0),  m_is_init(false),
	m_is_finish(false), m_is_pass(false), m_total_douhun(0), m_cur_wave(-1), m_flush_next_wave_timestamp(0), m_flush_next_monster_timestamp(0), m_delay_kickout_timestamp(0),
	m_can_call_extra_monster(false), m_escape_monster_count(0), m_total_exp(0), m_xian_hun_stone_num(0), m_hun_suipian_num(0),m_notify_next_wave_timestamp(0), m_special_monster_num(0),
	m_can_buy_extra_monster(true), m_remain_buyable_monster_num(0)
{
	memset(m_wave_flush_count_list, 0, sizeof(m_wave_flush_count_list));
	memset(m_boss_flush_timestamp_list, 0, sizeof(m_boss_flush_timestamp_list));
}

SpecialBuildTower::~SpecialBuildTower()
{

}

void SpecialBuildTower::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_is_finish)
	{
		if (m_delay_kickout_timestamp > 0 && now_second >= m_delay_kickout_timestamp)
		{
			this->DelayKickOutAllRole(CHANGE_SCENE_REASON_FB_FINISH);
		}
		return;
	}

	if (m_flush_next_wave_timestamp > 0 && now_second > m_flush_next_wave_timestamp)
	{
		this->FlushMonsterWave();
		m_flush_next_wave_timestamp = now_second + LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg().two_wave_second;
	}

	if (now_second > m_flush_next_monster_timestamp)
	{
		this->FlushOneMonster();
		m_flush_next_monster_timestamp = now_second + 1;

		if (m_can_call_extra_monster)	//刷新额外怪物
		{
			this->OnCreateSpMonster();
		}
	}
	
	this->CheckMonsterEscape();
}

void SpecialBuildTower::OnRoleEnterScene(Role *role)
{
	m_owner_user_id = role->GetUserId();
	m_owner_objid = role->GetId();

	// 进入自动满血
	role->Recover(true, true, true);
	this->AddSceneSpeed(role);

	if (!m_is_init)
	{
		this->InitBackstage();

		time_t now_time = EngineAdapter::Instance().Time();
		m_flush_next_wave_timestamp = now_time + LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg().prepare_time_second;

		m_role_gongji = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI);

		m_is_init = true;

		//EventHandler::Instance().OnEnterBuildTowerFb(role);
		m_total_douhun = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg().init_douhun;
	}
	
	if (m_cur_wave == -1 && m_special_monster_num == 0)
	{
		m_remain_buyable_monster_num = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg().max_extra_monster_num;

		this->SyncFBSceneLogicInfo(Protocol::SCBuildTowerFBSceneLogicInfo::NOTIFY_PREPARE_TIME);
	}
	else
	{
		this->SyncFBSceneLogicInfo();
	}
}

void SpecialBuildTower::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role || m_owner_user_id != role->GetUserId())
	{
		return;
	}
}

void SpecialBuildTower::InitBackstage()
{
	for (int wave = 0; wave <= BUILD_TOWER_MAX_MONSTER_WAVE; wave++)
	{
		const BuildTowerFbMonsterFlushCfg::ConfigItem *cfg = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbMonsterFlushCfg(wave);
		if (NULL != cfg)
		{
			m_wave_flush_count_list[wave] = cfg->flush_num;
		}
	}
}

bool SpecialBuildTower::FlushMonsterWave()
{
	if (m_is_finish) return false;

	if (m_cur_wave >= BUILD_TOWER_MAX_MONSTER_WAVE)
	{
		m_flush_next_wave_timestamp = 0;
		return false;
	}
	
	m_cur_wave ++;
	m_can_call_extra_monster = true;
	m_notify_next_wave_timestamp = 0;

	int remain_buyable = BUILD_TOWER_MAX_MONSTER_WAVE - m_cur_wave - m_special_monster_num + 1;
	if (m_remain_buyable_monster_num > remain_buyable )
	{
		m_remain_buyable_monster_num = remain_buyable > 0 ? remain_buyable : 0 ;
	}

	this->SyncFBSceneLogicInfo(Protocol::SCBuildTowerFBSceneLogicInfo::NOTIFY_MONSTER_WAVE);

	return true;
}

bool SpecialBuildTower::FlushOneMonster()
{
	if (m_is_finish) return false;

	if (m_cur_wave < 0 || m_cur_wave > BUILD_TOWER_MAX_MONSTER_WAVE)
	{
		return false;
	}

	const Posi *born_pos = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbMonsterPathPos(0);
	if (NULL == born_pos)
	{
		return false;
	}

	if (m_role_gongji <= 0) return false;

	time_t now_time = EngineAdapter::Instance().Time();

	bool all_flushed = true;
	for (int wave = 0; wave <= m_cur_wave && wave <= BUILD_TOWER_MAX_MONSTER_WAVE; wave++)
	{
		const BuildTowerFbMonsterFlushCfg::ConfigItem *cfg = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbMonsterFlushCfg(wave);
		if (NULL == cfg) continue;

		if (m_wave_flush_count_list[wave] > 0)
		{
			Role *role = this->GetOwnerRole();
			if (NULL == role) return false;

			Monster *monster = MONSTERPOOL->CreateMonster(cfg->monster_id, m_scene, *born_pos, 0, 0, 0, role->GetLevel());
			if (NULL != monster)
			{
				//Attribute monster_maxhp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				//Attribute new_monster_maxhp = static_cast<Attribute>(m_role_gongji * cfg->monster_maxhp_gongji_times);
				//monster->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, new_monster_maxhp - monster_maxhp);
				//monster->Recover();

				m_wave_flush_count_list[wave]--;

				if (m_wave_flush_count_list[wave] <= 0 && cfg->boss_id > 0)
				{
					m_boss_flush_timestamp_list[wave] = now_time + 10;
				}
			}
		}

		if (m_boss_flush_timestamp_list[wave] > 0 && now_time > m_boss_flush_timestamp_list[wave])
		{
			Role *role = this->GetOwnerRole();
			if (NULL == role) return false;

			Monster *monster = MONSTERPOOL->CreateMonster(cfg->boss_id, m_scene, *born_pos, 0, 0, 0, role->GetLevel());
			if (NULL != monster)
			{
				//Attribute monster_maxhp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				//Attribute new_monster_maxhp = static_cast<Attribute>(m_role_gongji * cfg->boss_maxhp_gongji_times);
				//monster->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, new_monster_maxhp - monster_maxhp);
				//monster->Recover();

				this->SyncFBSceneLogicInfo(Protocol::SCBuildTowerFBSceneLogicInfo::NOTIFY_BOSS_FLUSH);
			}

			m_boss_flush_timestamp_list[wave] = 0;
		}
		if (m_wave_flush_count_list[wave] > 0 || m_boss_flush_timestamp_list[wave] > 0)
		{
			all_flushed = false;
		}
	}

	if (all_flushed)
	{
		m_notify_next_wave_timestamp = 1;
		this->SyncFBSceneLogicInfo(Protocol::SCBuildTowerFBSceneLogicInfo::NOTIFY_WAVE_FLUSH_END);
	}

	return true;
}

void SpecialBuildTower::OnAddObj(Obj *obj)
{
	if (NULL == obj) return;

	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster *)obj;

		if (this->IsTowerMonster(monster->GetMonsterId()))
		{
			return;
		}

		if (monster->GetAIType() == MonsterInitParam::AITYPE_BUILD_TOWER_RUNNING)
		{
			MonsterAIRunningMonster *ai = (MonsterAIRunningMonster *)monster->GetMonsterAI();

			Posi walk_pos_list[MAX_RUNNING_MONSTER_AI_RUNNING_STEP];
			memset(walk_pos_list, 0, sizeof(walk_pos_list));

			int pos_count = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbMonsterPathPosCount();
			for (int i = 0; i < pos_count; i++)
			{
				const Posi *born_pos = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbMonsterPathPos(i);
				walk_pos_list[i] = Posi(born_pos->x, born_pos->y);
			}

			ai->SetWalkPosList(pos_count, walk_pos_list);
		}

		this->AddToMoveList(monster->GetId());
	}
}

bool SpecialBuildTower::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster *)obj;
		if (NULL != monster)
		{
			return monster->IsAlive() && MonsterInitParam::AITYPE_BUILD_TOWER_TOWER != monster->GetAIType();
		}
	}

	return false;
}

bool SpecialBuildTower::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
{
	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster *)obj;
		if (NULL != monster)
		{
			return check_alive ? (monster->IsAlive() && this->IsTowerMonster(monster->GetMonsterId())) : this->IsTowerMonster(monster->GetMonsterId());
		}
	}

	return false;
}

void SpecialBuildTower::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{
	if (NULL == role || NULL == falling_item)
	{
		return;
	}
	const ItemBase *item = ITEMPOOL->GetItem(falling_item->GetItemId());
	if (NULL != item)
	{
		m_drop_item_map[falling_item->GetItemId()] +=falling_item->GetItemNum();

		if (item->GetColor() >= ItemBase::I_COLOR_PURPLE)
		{
			DropLogManager::Instance().AddFbLog(role->GetUID(),role->GetName(), falling_item->GetMonsterId(),falling_item->GetItemId(), 
				falling_item->GetItemNum(), falling_item->GetSceneId(), item->GetColor());
		}

		this->SyncFBSceneLogicInfo();
	}
}

void SpecialBuildTower::AddToMoveList(ObjID obj_id)
{
	MovingMonsterInfoMapIt it = m_moving_monster_step_map.find(obj_id);
	if (it != m_moving_monster_step_map.end()) 
	{
		return; 
	}

	MovingMonsterInfo info;
	info.obj_id = obj_id;
	info.cur_pos_index = 0;
	
	m_moving_monster_step_map[obj_id] = info;
}

void SpecialBuildTower::RemoveFromMoveList(ObjID obj_id)
{
	MovingMonsterInfoMapIt it = m_moving_monster_step_map.find(obj_id);
	if (it != m_moving_monster_step_map.end()) 
	{
		m_moving_monster_step_map.erase(it); 
	}

	const BuildTowerFbMonsterFlushCfg::ConfigItem *cfg = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbMonsterFlushCfg(m_cur_wave);
	if (NULL != cfg)
	{
		if (m_boss_flush_timestamp_list[m_cur_wave] <= 0 && m_wave_flush_count_list[m_cur_wave] <= 0 && m_moving_monster_step_map.size() <= 0)
		{
			m_flush_next_wave_timestamp = EngineAdapter::Instance().Time();

			if (m_cur_wave >= BUILD_TOWER_MAX_MONSTER_WAVE)
			{
				m_flush_next_wave_timestamp = 0;
				this->OnFBFinish(true);
			}
		}
	}
}

void SpecialBuildTower::CheckMonsterEscape()
{
	std::vector<ObjID> remove_obj_list;

	for (MovingMonsterInfoMapIt it = m_moving_monster_step_map.begin(); m_moving_monster_step_map.end() != it; it ++)
	{
		Obj *obj = m_scene->GetObj(it->second.obj_id);
		if (NULL == obj) continue;

		if (obj->GetObjType() != Obj::OBJ_TYPE_MONSTER) continue;

		Monster *monster = (Monster *)obj;
		if (monster->GetAIType() == MonsterInitParam::AITYPE_BUILD_TOWER_RUNNING)
		{
			MonsterAIRunningMonster *ai = (MonsterAIRunningMonster *)monster->GetMonsterAI();
			if (ai->IsAtWalkEndPos())
			{
				remove_obj_list.push_back(monster->GetId());
			}
		}
	}

	for (std::vector<ObjID>::iterator iter = remove_obj_list.begin(); remove_obj_list.end() != iter; ++ iter)
	{
		this->RemoveFromMoveList(*iter);
		m_scene->DeleteObj(*iter);

		m_escape_monster_count ++;

		if (!m_is_finish && m_escape_monster_count >= LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg().escape_num_to_failure)
		{
			this->OnFBFinish(false);
			this->SyncFBSceneLogicInfo(Protocol::SCBuildTowerFBSceneLogicInfo::NOTIFY_FB_END, true);
		}
		else
		{
			this->SyncFBSceneLogicInfo();
		}
	}
}

void SpecialBuildTower::OnLeaveFB(Role *role)
{
	if (!m_is_finish)
	{
		if (role->GetUserId() == m_owner_user_id)
		{
			this->OnFBFinish(false);
			this->SyncFBSceneLogicInfo(Protocol::SCBuildTowerFBSceneLogicInfo::NOTIFY_FB_END, true);
		}
	}
}

void SpecialBuildTower::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster *monster = (Monster *)dead;
		if (NULL == monster) return;

		if (!this->IsTowerMonster(monster->GetMonsterId()))
		{
			const int reward_douhun = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerRewardDouHun(monster->GetMonsterId());
			m_total_douhun += reward_douhun;

			Role *killer_role = this->GetOwnerRole();
			if (NULL != killer_role)
			{
				long long real_add_exp = 0;
				long long add_exp = LOGIC_CONFIG->GetBuildTowerFbConfig().GetThisMonsterExp(monster->GetMonsterId(), killer_role->GetLevel());
				killer_role->AddExp(add_exp, "BuildTowerFbKillMonster", Role::EXP_ADD_REASON_KILL_MONSTER, &real_add_exp);
				m_total_exp += real_add_exp;
			}

			this->RemoveFromMoveList(dead->GetId());

			this->SyncFBSceneLogicInfo();
		}
	}

	Obj *obj = m_scene->GetObj(killer_id);
	Monster *monster = (Monster *)dead;
	Role *role = this->GetOwnerRole();
	const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(monster->GetMonsterId());

	if (NULL == obj || NULL == monster || NULL == role || NULL == monster_param)
	{
		return;
	}

	if(obj->GetObjType() != Obj::OBJ_TYPE_ROLE)
	{
		for (std::vector<UInt16>::const_iterator  it = monster_param->dropid_list.begin(); it != monster_param->dropid_list.end(); ++it)
		{
			DROPPOOL->DropHelper(*it, m_scene, monster->GetPos(), INVALID_USER_ID, -1, monster->GetMonsterId(), false, false, nullptr);
		}
	}
}

void SpecialBuildTower::OnTowerOperate(int operate_type, int pos_index, int tower_type)
{
	switch (operate_type)
	{
	case Protocol::BUILD_TOWER_OPERA_TYPE_BUILD:
	{
		this->OnBuildTowerReq(pos_index, tower_type);
	}
	break;

	case Protocol::BUILD_TOWER_OPERA_TYPE_UPGRADE:
	{
		this->OnUpgradeTowerReq(pos_index);
	}
	break;

	case Protocol::BUILD_TOWER_OPERA_TYPE_REMOVE:
	{
		this->OnRemoveTowerReq(pos_index);
	}
	break;

	case Protocol::BUILD_TOWER_OPERA_TYPE_FLUSH:
	{
		this->OnFlushFirstWaveReq();
	}
	break;

	case Protocol::BUILD_TOWER_OPERA_TYPE_CALL:
	{
		this->OnBuyExtraMonsterReq(pos_index);
	}
	break;

	default: break;
	}
}

void SpecialBuildTower::OnBuildTowerReq(int pos_index, int tower_type)
{
	if (m_is_finish) return;

	if (pos_index < 0 || pos_index > BUILD_TOWER_MAX_TOWER_POS_INDEX || tower_type < 0 || tower_type > BUILD_TOWER_MAX_TYPE)
	{
		return;
	}

	if (m_tower_info_list[pos_index].level > 0 || INVALID_OBJ_ID != m_tower_info_list[pos_index].obj_id)
	{
		return;
	}

	const Posi *tower_pos = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbTowerPos(pos_index);
	if (NULL == tower_pos)
	{
		return;
	}

	Role *role = this->GetOwnerRole();
	if (NULL == role)
	{
		return;
	}

	int range = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg().tower_plant_range;
	if ((*tower_pos - role->GetPos()).IsLongerThan(range))
	{
// 		role->NoticeNum(errornum::EN_BUILD_TOWER_TOO_FAR);
// 		return;
	}

	const BuildTowerFbTowerCfg::ConfigItem *cfg = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbTowerCfg(tower_type, 1);
	if (m_total_douhun < cfg->need_douhun)
	{
		role->NoticeNum(errornum::EN_BUILD_TOWER_LOW_DOUHUN);
		return;
	}

	m_total_douhun -= cfg->need_douhun;

	Monster *monster = MONSTERPOOL->CreateMonster(cfg->monster_id, m_scene, *tower_pos, 0, 0, 0, role->GetLevel());
	if (NULL == monster)
	{
		return;
	}

	Attribute mosnter_old_gongji = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI);
	Attribute role_new_gongji = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI);
	monster->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, role_new_gongji - mosnter_old_gongji);

	m_tower_info_list[pos_index].tower_type = tower_type;
	m_tower_info_list[pos_index].obj_id = monster->GetId();
	m_tower_info_list[pos_index].level = 1;

	role->NoticeNum(noticenum::NT_BUILD_TOWER_BUILD_SUCC);

	this->SyncFBSceneLogicInfo();
}

void SpecialBuildTower::OnUpgradeTowerReq(int pos_index)
{
	if (m_is_finish) return;

	if (pos_index < 0 || pos_index > BUILD_TOWER_MAX_TOWER_POS_INDEX)
	{
		return;
	}

	if (m_tower_info_list[pos_index].level <= 0 || INVALID_OBJ_ID == m_tower_info_list[pos_index].obj_id)
	{
		return;
	}

	const Posi *tower_pos = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbTowerPos(pos_index);
	if (NULL == tower_pos)
	{
		return;
	}

	Role *role = this->GetOwnerRole();
	if (NULL == role)
	{
		return;
	}

	int range = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg().tower_plant_range;
	if ((*tower_pos - role->GetPos()).IsLongerThan(range))
	{
// 		role->NoticeNum(errornum::EN_BUILD_TOWER_TOO_FAR);
// 		return;
	}
	
	if (m_tower_info_list[pos_index].level >= BUILD_TOWER_MAX_LEVEL)
	{
		return;
	}

	const BuildTowerFbTowerCfg::ConfigItem *next_level_cfg = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbTowerCfg(m_tower_info_list[pos_index].tower_type, m_tower_info_list[pos_index].level + 1);
	if (NULL == next_level_cfg)
	{
		return;
	}

	if (m_total_douhun < next_level_cfg->need_douhun)
	{
		role->NoticeNum(errornum::EN_BUILD_TOWER_LOW_DOUHUN);
		return;
	}

	m_total_douhun -= next_level_cfg->need_douhun;

	m_scene->DeleteObj(m_tower_info_list[pos_index].obj_id);

	Monster *monster = MONSTERPOOL->CreateMonster(next_level_cfg->monster_id, m_scene, *tower_pos, 0, 0, 0, role->GetLevel());
	if (NULL == monster)
	{
		return;
	}

	Attribute monster_old_gongji = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI);
	Attribute role_new_gongji = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI);

	monster->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, role_new_gongji - monster_old_gongji);

	m_tower_info_list[pos_index].obj_id = monster->GetId();
	m_tower_info_list[pos_index].level ++;

	role->NoticeNum(noticenum::NT_BUILD_TOWER_UPLEVEL_SUCC);

	this->SyncFBSceneLogicInfo();
}

void SpecialBuildTower::OnRemoveTowerReq(int pos_index)
{
	if (m_is_finish) return;

	if (pos_index < 0 || pos_index > BUILD_TOWER_MAX_TOWER_POS_INDEX)
	{
		return;
	}

	const Posi *tower_pos = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbTowerPos(pos_index);
	if (NULL == tower_pos)
	{
		return;
	}

	Role *role = this->GetOwnerRole();
	if (NULL == role)
	{
		return;
	}

	int range = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg().tower_plant_range;
	if ((*tower_pos - role->GetPos()).IsLongerThan(range))
	{
// 		role->NoticeNum(errornum::EN_BUILD_TOWER_TOO_FAR);
// 		return;
	}

	if (m_tower_info_list[pos_index].level <= 0 || INVALID_OBJ_ID == m_tower_info_list[pos_index].obj_id)
	{
		return;
	}

	const BuildTowerFbTowerCfg::ConfigItem *cfg = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbTowerCfg(m_tower_info_list[pos_index].tower_type, m_tower_info_list[pos_index].level);
	if (NULL == cfg)
	{
		return;
	}
	
	m_total_douhun += cfg->return_douhun;

	m_scene->DeleteObj(m_tower_info_list[pos_index].obj_id);

	m_tower_info_list[pos_index].Reset();

	role->NoticeNum(noticenum::NT_BUILD_TOWER_REMOVE_SUCC);

	this->SyncFBSceneLogicInfo();
}

void SpecialBuildTower::OnFlushFirstWaveReq()
{
	if (m_is_finish) return;

	if (m_cur_wave >= 0 || m_flush_next_wave_timestamp <= 0)
	{
		return;
	}

	m_flush_next_wave_timestamp = EngineAdapter::Instance().Time();;
}

void SpecialBuildTower::OnCallExtraMonsterReq()
{
	if (m_is_finish) return;

	Role *role = this->GetOwnerRole();
	if (NULL == role)
	{
		return;
	}

	if (!m_can_call_extra_monster)
	{
		role->NoticeNum(errornum::EN_CUR_WAVE_HAS_CALL_EXTRA);
		return;
	}

	const BuildTowerFbOtherCfg &other_cfg = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg();
	if (!role->GetKnapsack()->GetMoney()->AllGoldMoreThan(other_cfg.extra_call_gold))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.extra_call_gold, "OnCallExtraMonsterReq", true))
	{
		return;
	}

	const Posi *born_pos = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbMonsterPathPos(0);
	if (NULL == born_pos)
	{
		return;
	}

	int extra_monster_id = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg().extra_monster_id;
	Monster *monster = MONSTERPOOL->CreateMonster(extra_monster_id, m_scene, *born_pos, 0, 0, 0, role->GetLevel());
	if (NULL == monster)
	{
		return;
	}

	//Attribute monster_maxhp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	//Attribute new_monster_maxhp = static_cast<Attribute>(m_role_gongji * other_cfg.extra_monster_maxhp_gongji_times);
	//
	//monster->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, new_monster_maxhp - monster_maxhp);
	//monster->Recover();

	m_can_call_extra_monster = false;

	role->NoticeNum(noticenum::NT_BUILD_TOWER_CALL_SUCC);

	this->SyncFBSceneLogicInfo(Protocol::SCBuildTowerFBSceneLogicInfo::NOTIFY_EXTRA_BOSS);
}

bool SpecialBuildTower::IsTowerMonster(int monster_id)
{
	for (int tower_type = 0; tower_type <= BUILD_TOWER_MAX_TYPE; tower_type++)
	{
		for (int tower_level = 1; tower_level <= BUILD_TOWER_MAX_LEVEL; tower_level++)
		{
			const BuildTowerFbTowerCfg::ConfigItem *cfg = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbTowerCfg(tower_type, tower_level);
			if (NULL != cfg && monster_id == cfg->monster_id)
			{
				return true;
			}
		}
	}
	
	return false;
}

void SpecialBuildTower::OnFBFinish(bool is_passed)
{
	if (m_is_finish) return;

	m_is_finish = true;
	m_is_pass = is_passed;

	this->KillAllMonster();

	Role *role = this->GetOwnerRole();
	if (NULL != role)
	{
		this->AutoPickFallingItem(role);
		this->SyncFBSceneLogicInfo(Protocol::SCBuildTowerFBSceneLogicInfo::NOTIFY_FB_END);

		if (is_passed && 0 == role->GetCommonDataParam()->is_pass_build_tower_fb)
		{
			role->GetCommonDataParam()->is_pass_build_tower_fb = 1;
		}

		if(is_passed)
		{
			m_delay_kickout_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 9;
		}
		else
		{
			this->DelayKickOutRole(role, CHANGE_SCENE_REASON_FB_FINISH);
		}
	}
}

Role * SpecialBuildTower::GetOwnerRole()
{
	Obj *obj = m_scene->GetObj(m_owner_objid);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		if (m_owner_user_id == role->GetUserId()) 
		{
			return role;
		}
	}

	return NULL;
}

void SpecialBuildTower::SyncFBSceneLogicInfo(int notify_reason, bool is_active_leave_fb)
{
	Role *role = this->GetOwnerRole();
	if (NULL == role)
	{
		return;
	}

	static Protocol::SCBuildTowerFBSceneLogicInfo cmd;
	cmd.reset();

	cmd.notify_reason = notify_reason;
	cmd.is_finish = m_is_finish ? 1 : 0;
	cmd.is_pass = m_is_pass ? 1 : 0;
	//cmd.can_call_extra_monster = m_can_call_extra_monster ? 1 : 0;
	cmd.can_call_extra_monster = m_can_buy_extra_monster ? 1 : 0;

	cmd.time_out_timestamp = m_scene->GetSceneTimeOutStamp();
	cmd.exp = m_total_exp;
	cmd.douhun = m_total_douhun;
	cmd.cur_wave = m_cur_wave;
	cmd.escape_monster_count = m_escape_monster_count;
	cmd.next_wave_timestamp = static_cast<unsigned int>(m_flush_next_wave_timestamp);
	cmd.notify_next_wave_timestamp = m_notify_next_wave_timestamp;
	cmd.special_monster_num = m_special_monster_num;
	cmd.remain_buyable_monster_num = m_remain_buyable_monster_num;

	for (int i = 0; i <= BUILD_TOWER_MAX_TOWER_POS_INDEX; i++)
	{
		cmd.tower_info_list[i].tower_type = m_tower_info_list[i].tower_type;
		cmd.tower_info_list[i].tower_level = m_tower_info_list[i].level;
	}

	for (BuildTowerDropItemMapIt iter = m_drop_item_map.begin(); m_drop_item_map.end() != iter; iter++)
	{
		cmd.item_list[cmd.item_count].item_id = iter->first;
		cmd.item_list[cmd.item_count].num = iter->second;
		cmd.item_count++;
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}


void SpecialBuildTower::OnBuyExtraMonsterReq(int monster_num)
{
	if (m_is_finish) return;

	Role *role = this->GetOwnerRole();
	if (NULL == role)
	{
		return;
	}

	if (!m_can_buy_extra_monster)						
	{
		role->NoticeNum(errornum::EN_CUR_WAVE_HAS_CALL_EXTRA);
		return;
	}

	if(monster_num <= 0)
	{
		return;
	}

	if (m_remain_buyable_monster_num >= monster_num && monster_num <= (BUILD_TOWER_MAX_MONSTER_WAVE - m_cur_wave + 1))	//额外怪物购买数限制（少于等于剩余数）
	{
		const BuildTowerFbOtherCfg &other_cfg = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg();
		if (!role->GetKnapsack()->GetMoney()->AllGoldMoreThan(other_cfg.extra_call_gold * monster_num))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		if (!role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.extra_call_gold * monster_num, "OnCallExtraMonsterReq", true))
		{
			return;
		}

		m_special_monster_num += monster_num;			//记录购买的额外怪物数

		m_remain_buyable_monster_num -= monster_num;

		if (m_remain_buyable_monster_num <= 0)
		{
			m_can_buy_extra_monster = false;			
		}

		this->SyncFBSceneLogicInfo();
	}
}

void SpecialBuildTower::OnCreateSpMonster()
{
	if (m_is_finish) return;

	Role *role = this->GetOwnerRole();
	if (NULL == role)
	{
		return;
	}

	if (m_can_call_extra_monster && m_special_monster_num > 0)						//刷新一波
	{
		//const BuildTowerFbOtherCfg &other_cfg = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg();

		const Posi *born_pos = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbMonsterPathPos(0);
		if (NULL == born_pos)
		{
			return;
		}

		int extra_monster_id = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg().extra_monster_id;
		Monster *monster = MONSTERPOOL->CreateMonster(extra_monster_id, m_scene, *born_pos, 0, 0, 0, role->GetLevel());
		if (NULL == monster)
		{
			return;
		}

		//Attribute monster_maxhp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		//Attribute new_monster_maxhp = static_cast<Attribute>(m_role_gongji * other_cfg.extra_monster_maxhp_gongji_times);
		//
		//monster->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, new_monster_maxhp - monster_maxhp);
		//monster->Recover();

		m_can_call_extra_monster = false;			//等待下一波刷新

		m_special_monster_num--;					//额外怪物剩余数量变更

		this->SyncFBSceneLogicInfo(Protocol::SCBuildTowerFBSceneLogicInfo::NOTIFY_EXTRA_BOSS);
	}
}
void SpecialBuildTower::AddSceneSpeed(Role *role)
{
	if (NULL == role) return;
	const BuildTowerFbOtherCfg & other_cfg = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg();
	Attribute add_speed = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED)*other_cfg.add_speed_per / 100;
	EffectBuff *effect_speed = new EffectBuff(INVALID_OBJ_ID, 0, 24 * 3600 * 1000, static_cast<char>(100), EffectBase::PRODUCT_METHOD_SCENE_SKILL);
	effect_speed->AddMoveSpeedEffect(add_speed, other_cfg.add_speed_per);
	effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	effect_speed->SetSave(false);
	effect_speed->SetClearOnDie(false);

	role->AddEffect(effect_speed);
}