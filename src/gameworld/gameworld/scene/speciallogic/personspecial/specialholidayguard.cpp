#include "specialholidayguard.hpp"
#include "engineadapter.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/otherobj/gatherobj.h"
#include "monster/monsterpool.h"
#include "protocal/msgfight.h"
#include "other/event/eventhandler.hpp"
#include "internalcomm.h"
#include "ai/monsterai/specialai/monsteraitowerdefendteam.hpp"
#include "config/logicconfigmanager.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/effectbase.hpp"
#include "other/fb/towerdefendteamfunc.hpp"
#include "obj/otherobj/fallingitem.h"
#include "gameworld/item/itempool.h"
#include "npc/transmit/fbmanager.hpp"
#include "effect/skilleffect/effecthpstore.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "global/randactivity/impl/randactivityholidayguard.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "other/capability/capability.hpp"
#include "item/knapsack.h"
#include "other/roleactivity/roleactivity.hpp"

SpecialHolidayGuard::SpecialHolidayGuard(Scene *scene)
	: SpecialLogic(scene), m_is_inited(false), m_is_finish(false), m_is_pass(false), m_is_last_wave(false), m_msg_need_sync(false),
	m_curr_wave_index(-1), m_next_wave_start_time(0), m_next_refresh_one_time(0), m_last_check_monster_status_time(0),
	m_last_warning_attack_time(0), m_last_sync_to_allclient_time(0), m_life_tower_left_hp(0), m_life_tower_left_maxhp(0), m_owner_objid(0),
	m_clear_wave_count(0), m_get_coin(0), m_get_item_count(0), m_send_drop_info_timestamp(0), m_kill_monster_count(0)
{
	memset(m_refresh_left_count, 0, sizeof(m_refresh_left_count));
	memset(m_kill_count, 0, sizeof(m_kill_count));
}

SpecialHolidayGuard::~SpecialHolidayGuard()
{
}

void SpecialHolidayGuard::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_is_finish)
	{
		if (0 != m_send_drop_info_timestamp && EngineAdapter::Instance().Time() >= m_send_drop_info_timestamp)
		{
			m_send_drop_info_timestamp = 0;

			Role *role = this->GetOwnerRole();
			if (nullptr != role)
			{
				Protocol::SCHolidayGuardFBDropInfo fbdi;
				fbdi.get_coin = m_get_coin;
				fbdi.get_item_count = 0;

				memset(fbdi.drop_items, 0, sizeof(Protocol::SCHolidayGuardFBDropInfo::DropItem) * Protocol::SCHolidayGuardFBDropInfo::FB_DROP_ITEM_MAX_NUM);
				for (int i = 0; i < m_get_item_count && i < Protocol::SCHolidayGuardFBDropInfo::FB_DROP_ITEM_MAX_NUM; ++i)
				{
					fbdi.drop_items[i].item_id = m_reward_item_list[i].item_id;
					fbdi.drop_items[i].num = static_cast<short>(m_reward_item_list[i].num);

					++fbdi.get_item_count;
				}

				int len = sizeof(fbdi) - sizeof(fbdi.drop_items[0]) * (Protocol::SCHolidayGuardFBDropInfo::FB_DROP_ITEM_MAX_NUM - fbdi.get_item_count);
				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&fbdi, len);

				Protocol::SCHolidayGuardResult ttdr;
				ttdr.is_passed = m_is_pass;
				ttdr.clear_wave_count = m_clear_wave_count;
				ttdr.resertotal_kill_monster_countve_sh = m_kill_monster_count;

				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&ttdr, sizeof(ttdr));
			}
		}
	}

	{
		// 刚启动
		if (!m_is_inited)
		{
			m_is_inited = true;

			m_next_wave_start_time = static_cast<unsigned int>(now_second) + LOGIC_CONFIG->GetHolidayGuardConfig().GetRefreshStartDelay();
			UInt32 monster_num = m_scene->MonsterNum();
			for (int i = 0; i < static_cast<int>(monster_num); ++i)
			{
				Monster *monster = m_scene->GetMonsterByIndex(i);
				if (nullptr!= monster && monster->IsAlive() &&
					monster->GetMonsterId() == LOGIC_CONFIG->GetHolidayGuardConfig().GetLifeHolidayMonsterID())
				{
					m_life_tower_left_maxhp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
					m_life_tower_left_hp = m_life_tower_left_maxhp;

					break;
				}
			}

			this->SyncToClient(Protocol::SCHolidayGuardInfo::NOTIFY_REASON_INIT);
		}
	}
 
	{
		// 新一波开始
		if (m_next_wave_start_time > 0 && now_second >= m_next_wave_start_time)
		{
			++m_curr_wave_index;
			const HolidayGuardWaveCfg *wave_cfg = LOGIC_CONFIG->GetHolidayGuardConfig().GetWaveCfg(m_scene->GetSceneID(), m_curr_wave_index);
			if (nullptr != wave_cfg)
			{
				m_next_wave_start_time = static_cast<unsigned int>(now_second) + wave_cfg->next_wave_s;

				for (int i = 0; i < HOLIDAYGUARD_PATH_MAX; ++i)
				{
					m_refresh_left_count[i] = wave_cfg->monster_count_list[i];
				}

				this->RefreshOne(now_second);
			}

			if (m_curr_wave_index + 1 >= LOGIC_CONFIG->GetHolidayGuardConfig().GetWaveCountBySceneID(m_scene->GetSceneID()))	// 最后一波了
			{
				m_is_last_wave = true;
				m_next_wave_start_time = 0;
			}

			this->SyncToClient(Protocol::SCHolidayGuardInfo::NOTIFY_REASON_NEW_WAVE_START);
		}
	}

	{
		// 按频率在各路线出怪
		if (m_next_refresh_one_time > 0 && now_second >= m_next_refresh_one_time)
		{
			this->RefreshOne(now_second);
		}
	}

	if (!m_is_finish)
	{
		// 检查是否杀光
		this->CheckAttackMonsterStatus(interval, now_second);
	}

	{
		if (m_curr_wave_index >= 0 && m_msg_need_sync)
		{
			this->SyncToClient();
		}
	}
}

void SpecialHolidayGuard::OnRoleEnterScene(Role * role)
{
	if(nullptr == role)
	{
		return;
	}

	if (m_is_finish)
	{
		this->DelayKickOutRole(role, CHANGE_SCENE_REASON_FB_FINISH);
	}
	else
	{
		bool is_first_enter = (INVALID_USER_ID == m_owner_user_id);

		m_owner_user_id = role->GetUserId();
		m_owner_objid = role->GetId();

		EventHandler::Instance().OnEnterFBHolidayGuardPerson(role, is_first_enter);

		m_msg_need_sync = true;

		// 下坐骑
		role->GetMountManager()->GoonMount(false);
	}
}

void SpecialHolidayGuard::OnRoleLeaveScene(Role * role, bool is_logout)
{
	if (nullptr == role)
	{
		return;
	}

	this->AutoPickFallingItem(role);

	Protocol::SCHolidayGuardResult ttdr;
	ttdr.is_passed = m_is_pass;
	ttdr.clear_wave_count = m_clear_wave_count;
	ttdr.resertotal_kill_monster_countve_sh = m_kill_monster_count;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&ttdr, sizeof(ttdr));
}

void SpecialHolidayGuard::OnLeaveFB(Role * role)
{
	if (nullptr == role)
	{
		return;
	}

	if (!m_is_finish)
	{
		this->OnFinish(false);
	}

	// role->GetSpecialAppearance()->SetFbAppearance(SPECIAL_APPEARANCE_TYPE_NORMAL, 0, false);
	role->GetMountManager()->GoonMount(true, false, true);
}

void SpecialHolidayGuard::OnAttack(Character * cha, ObjID enemy_id, Attribute delta_hp)
{
	if (nullptr == cha) return;

	if (Obj::OBJ_TYPE_MONSTER == cha->GetObjType())
	{
		Monster *monster = (Monster*)cha;

		if (monster->GetMonsterId() == LOGIC_CONFIG->GetHolidayGuardConfig().GetLifeHolidayMonsterID() && monster->IsAlive())
		{
			unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			{
				if (now_second >= m_last_warning_attack_time + LOGIC_CONFIG->GetHolidayGuardConfig().GetWarningInterval())
				{
					m_last_warning_attack_time = now_second;

					this->NotifyWarning(Protocol::SCHolidayGuardWarning::WARNING_TYPE_NORMAL, 0);
				}
			}

			{
				long long now_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
				long long max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				long long old_hp = now_hp + (-delta_hp);

				int old_percent = static_cast<int>(max_hp > 0 ? old_hp * 100 / max_hp : 0);
				int now_percent = static_cast<int>(max_hp > 0 ? now_hp * 100 / max_hp : 0);

				if (old_percent > now_percent)
				{
					int warning_percent = 0;
					if (LOGIC_CONFIG->GetHolidayGuardConfig().CheckWarning(old_percent, now_percent, &warning_percent))
					{
						this->NotifyWarning(Protocol::SCHolidayGuardWarning::WARNING_TYPE_LOW_PERCENT, warning_percent);
					}
				}

				m_life_tower_left_hp = now_hp;
				m_msg_need_sync = true;
			}
		}
	}
}

bool SpecialHolidayGuard::OnTimeout()
{
	if (!m_is_finish)
	{
		m_is_finish = true;

		this->OnFinish(false);
	}

	return m_is_pass;
}

bool SpecialHolidayGuard::CanDestroy()
{
	return m_is_finish;
}

void SpecialHolidayGuard::OnRemoveObj(Obj * obj)
{
	if (nullptr == obj) return;

	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *monster = (Monster*)obj;

		if (monster->GetMonsterId() == LOGIC_CONFIG->GetHolidayGuardConfig().GetLifeHolidayMonsterID())
		{
			if (!m_is_finish)
			{
				m_life_tower_left_hp = 0;
				this->SyncToClient();

				m_is_finish = true;

				this->OnFinish(false);
			}
		}
		else
		{
			if (MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType())
			{
				const HolidayGuardWaveCfg * wave_cfg = LOGIC_CONFIG->GetHolidayGuardConfig().GetWaveCfgByMonsterID(m_scene->GetSceneID(), monster->GetMonsterId());
				if (nullptr != wave_cfg)
				{
					if (wave_cfg->wave >= 0 && wave_cfg->wave < PERSONAL_HOLIDAYGUARD_MAX_WAVE_PER_LEVEL)
					{
						++m_kill_count[wave_cfg->wave];
						if (m_kill_count[wave_cfg->wave] >= wave_cfg->total_monster_count)
						{
							++m_clear_wave_count;

							m_msg_need_sync = true;
						}
					}
				}
			}
		}
	}
}

void SpecialHolidayGuard::OnCharacterDie(Character * dead, ObjID killer_id)
{
	if (nullptr == dead) return;

	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
	{
		Monster * monster = (Monster *)dead;

		++m_kill_monster_count;
		this->SyncToClient(Protocol::SCHolidayGuardInfo::NOTIFY_REASON_KILL_MONSTER);

		HolidayGuardBuffDropWeightCfg *buff_weight_cfg = LOGIC_CONFIG->GetHolidayGuardConfig().GetBuffDropCfg(monster->GetMonsterId());
		if (nullptr != buff_weight_cfg)
		{
			if (buff_weight_cfg->total_weight > 0)
			{
				int buff_type = DEFEND_BUff_TYPE_INVALID;
				int rand_value = RandomNum(buff_weight_cfg->total_weight);

				for (int i = 0; i < (HOLIDAYGUARD_BUFF_DROP_WEIGHT_COUNT - 1); i++)
				{
					if (rand_value < buff_weight_cfg->weight[i])
					{
						buff_type = i + 1;
						break;
					}
					else
					{
						rand_value -= buff_weight_cfg->weight[i];
					}
				}

				if (buff_type > HOLIDAY_GUARD_BUFF_TYPE_INVALID && buff_type < HOLIDAY_GUARD_BUFF_TYPE_MAX)
				{
					FallingItem *fallingitem = new FallingItem();
					fallingitem->Init(0, 0, false, 0, INVALID_USER_ID, m_scene->GetSceneID(), LOGIC_CONFIG->GetHolidayGuardConfig().GetBuffDropExitTime(), 0, FALLING_ITEM_BROADCAST_TYPE_NO_BROADCAST, false);
					fallingitem->SetPos(monster->GetPos());
					fallingitem->SetObjPosi(monster->GetPos());
					fallingitem->SetBuffFalling(true);
					fallingitem->SetBuffAppearan(buff_type);
					m_scene->AddObj(fallingitem);
				}
			}
		}
	}
}

bool SpecialHolidayGuard::SpecialRelive(Role * role, int realive_type)
{
	if (nullptr == role) return false;

	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		Posi realive_pos;
		int target_scene_id = m_scene->GetTownPoint(role, &realive_pos);
		if (target_scene_id == m_scene->GetSceneID())
		{
			if (role->ReAlive(10, 100, realive_pos)) return true;
		}
	}

	return false;
}

bool SpecialHolidayGuard::SpecialIsEnemy(Role * role, Obj * obj)
{
	if (nullptr == role || nullptr == obj) return false;

	switch (obj->GetObjType())
	{
	case Obj::OBJ_TYPE_ROLE:
	{
		return false;
	}
	break;

	case Obj::OBJ_TYPE_MONSTER:
	{
		Monster *monster = (Monster*)obj;
		return monster->IsAlive() && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType();
	}
	break;
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

bool SpecialHolidayGuard::SpecialIsFriend(Role * role, Obj * obj, bool check_alive)
{
	if (nullptr == role || nullptr == obj) return false;

	switch (obj->GetObjType())
	{
	case Obj::OBJ_TYPE_ROLE:
	{
		Role *friend_role = (Role*)obj;
		return (!check_alive || friend_role->IsAlive());
	}
	break;

	case Obj::OBJ_TYPE_MONSTER:
	{
		Monster *monster = (Monster*)obj;
		return (!check_alive || monster->IsAlive()) && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType();
	}
	break;
	}

	return SpecialLogic::SpecialIsFriend(role, obj, check_alive);
}

bool SpecialHolidayGuard::SpecialCanReAlive(Role * role, int realive_type)
{
	if (nullptr == role) return false;

	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	{
		return role->GetDeadTime() >= HOLIDAYGUARD_HOME_REALIVE_LIMIT_INTERVAL;
	}

	return role->GetDeadTime() >= HOLIDAYGUARD_GOLD_REALIVE_LIMIT_INTERVAL;
}

void SpecialHolidayGuard::OnPickFallingItem(Role * role, FallingItem * falling_item, ItemDataWrapper * item_wrapper)
{
	if (nullptr != role && nullptr != falling_item)
	{
		const ItemBase *item = ITEMPOOL->GetItem(falling_item->GetItemId());
		if (nullptr != item)
		{
			bool is_find = false;

			for (int i = 0; i < m_get_item_count && i < REWARD_ITEM_MAX_TYPE; ++i)
			{
				if (m_reward_item_list[i].item_id == falling_item->GetItemId())
				{
					m_reward_item_list[i].num += falling_item->GetItemNum();
					is_find = true;

					break;
				}
			}

			if (!is_find && m_get_item_count < REWARD_ITEM_MAX_TYPE)
			{
				m_reward_item_list[m_get_item_count].item_id = falling_item->GetItemId();
				m_reward_item_list[m_get_item_count].num = falling_item->GetItemNum();

				++m_get_item_count;
			}
		}
		else
		{
			m_get_coin += falling_item->GetCoinOrGold();
		}

		if (falling_item->IsBuffFalling())
		{
			HolidayGuardBuffCfg *buff_cfg = LOGIC_CONFIG->GetHolidayGuardConfig().GetBufffCfg(falling_item->GetBuffAppearan());
			if (nullptr != buff_cfg)
			{
				switch (buff_cfg->buff_type)
				{
				case HOLIDAY_GUARD_BUFF_TYPE_RECOVER:
				{
					UInt32 monster_num = m_scene->MonsterNum();
					for (int i = 0; i < (int)monster_num; ++i)
					{
						Monster *monster = m_scene->GetMonsterByIndex(i);
						if (nullptr != monster && monster->IsAlive() &&
							monster->GetMonsterId() == LOGIC_CONFIG->GetHolidayGuardConfig().GetLifeHolidayMonsterID())
						{
							Attribute add_hp = static_cast<Attribute>(monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (buff_cfg->param[0] * ROLE_ATTR_PER));
							monster->ChangeHp(&add_hp, true);
							m_life_tower_left_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
							this->SyncToClient();
							break;
						}
					}
				}
				break;

				case HOLIDAY_GUARD_BUFF_TYPE_ADD_GONGJI:
				{
					EffectBuff *add_gongji = new EffectBuff(role->GetId(), 1, buff_cfg->param[1], 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
					add_gongji->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
					Attribute value = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (buff_cfg->param[0] * ROLE_ATTR_PER));
					add_gongji->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, value, buff_cfg->param[0]);
					add_gongji->SetClientEffectType(EffectBase::ECT_NSTF_ADD_GONGJI);
					role->AddEffect(add_gongji);
				}
				break;

				case HOLIDAY_GUARD_BUFF_TYPE_KILL_MONSTER:
				{
					UInt32 monster_num = m_scene->MonsterNum();
					for (int i = 0; i < (int)monster_num; ++i)
					{
						Monster *monster = m_scene->GetMonsterByIndex(i);
						if (nullptr != monster && monster->IsAlive() &&
							monster->GetMonsterId() != LOGIC_CONFIG->GetHolidayGuardConfig().GetLifeHolidayMonsterID())
						{
							monster->ForceSetDead(role);
						}
					}
				}
				break;

				case HOLIDAY_GUARD_BUFF_TYPE_ADD_HUDUN:
				{
					UInt32 monster_num = m_scene->MonsterNum();
					for (int i = 0; i < (int)monster_num; ++i)
					{
						Monster *monster = m_scene->GetMonsterByIndex(i);
						if (nullptr != monster && monster->IsAlive() &&
							monster->GetMonsterId() == LOGIC_CONFIG->GetHolidayGuardConfig().GetLifeHolidayMonsterID())
						{
							EffectHpStore *effect_hp_store = new EffectHpStore(role->GetId(), 1, buff_cfg->param[1], buff_cfg->param[0], 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
							effect_hp_store->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
							monster->AddEffect(effect_hp_store);
							break;
						}
					}
				}
				break;

				default:
					break;
				}
			}
		}
	}
}

void SpecialHolidayGuard::OnNextWaveReq(Role * role)
{
	if (nullptr == role) return;

	const HolidayGuardWaveCfg * wave_cfg = LOGIC_CONFIG->GetHolidayGuardConfig().GetWaveCfg(m_scene->GetSceneID(), m_curr_wave_index + 1);
	if (nullptr == wave_cfg) return;

	for (int i = 0; i < HOLIDAYGUARD_PATH_MAX; ++i)
	{
		if (m_refresh_left_count[i] > 0)
		{
			role->NoticeNum(errornum::EN_TOWER_DEFEND_CURR_WAVE_NOT_DONE);
			return;
		}
	}

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (m_next_wave_start_time > 0 && now_second < m_next_wave_start_time)
	{
		m_next_wave_start_time = now_second;

		this->SyncToClient();

		role->NoticeNum(noticenum::NT_TOWERDEFEND_NEXT_WAVE_SUCC);
	}
}

Role * SpecialHolidayGuard::GetOwnerRole()
{
	Obj *obj = m_scene->GetObj(m_owner_objid);
	if (nullptr != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role*)obj;
		if (m_owner_user_id == role->GetUserId())
		{
			return role;
		}
	}

	return nullptr;
}

void SpecialHolidayGuard::RefreshOne(time_t now_second)
{
	const HolidayGuardWaveCfg * wave_cfg = LOGIC_CONFIG->GetHolidayGuardConfig().GetWaveCfg(m_scene->GetSceneID(), m_curr_wave_index);
	if (nullptr == wave_cfg) return;

	bool is_left = false;

	for (int path = HOLIDAYGUARD_PATH_LEFT; path < HOLIDAYGUARD_PATH_MAX; ++path)
	{
		if (m_refresh_left_count[path] > 0)
		{
			Posi birth_pos = LOGIC_CONFIG->GetHolidayGuardConfig().GetBirthPos(path);

			Monster *monster = MONSTERPOOL->CreateMonster(wave_cfg->monster_id_list[path], m_scene, gamecommon::GetDisRandPos(birth_pos, 4));
			if (nullptr != monster)
			{
				MonsterAIBase *aibase = monster->GetMonsterAI();
				if (nullptr != aibase && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == aibase->GetAIType())
				{
					MonsterAITowerDefendTeam *aiattack = (MonsterAITowerDefendTeam *)aibase;
					aiattack->SetBirthPath(path);
				}

				--m_refresh_left_count[path];
			}

			if (m_refresh_left_count[path] > 0) is_left = true;
		}
	}

	if (is_left)
	{
		m_next_refresh_one_time = static_cast<unsigned int>(now_second) + LOGIC_CONFIG->GetHolidayGuardConfig().GetRefreshInterval();
	}
	else
	{
		m_next_refresh_one_time = 0;
	}
}

void SpecialHolidayGuard::SyncToClient(int reason)
{
	Role *role = this->GetOwnerRole();
	if (nullptr != role)
	{
		static Protocol::SCHolidayGuardInfo ttdi;
		ttdi.reason = static_cast<short>(reason);
		ttdi.time_out_stamp = 0;
		ttdi.is_finish = m_is_finish ? 1 : 0;
		ttdi.is_pass = m_is_pass ? 1 : 0;
		ttdi.pass_time_s = 0;
		ttdi.life_tower_left_hp = m_life_tower_left_hp;
		ttdi.life_tower_left_maxhp = m_life_tower_left_maxhp;
		ttdi.curr_wave = m_curr_wave_index;
		ttdi.reserve_1 = 0;
		ttdi.next_wave_refresh_time = m_next_wave_start_time;
		ttdi.clear_wave_count = m_clear_wave_count;
		ttdi.get_coin = 0;
		ttdi.get_item_count = 0;
		ttdi.total_kill_monster_count = m_kill_monster_count;


		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&ttdi, sizeof(ttdi));

		m_last_sync_to_allclient_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	}

	m_msg_need_sync = false;
}

void SpecialHolidayGuard::OnFinish(bool is_pass)
{
	m_is_pass = is_pass;
	m_next_wave_start_time = 0;
	m_is_finish = true;

	Role *role = this->GetOwnerRole();
	if(nullptr == role)
	{
		return;
	}

	if (is_pass)
	{
		this->SyncToClient();
		role->Recover();
	}

	m_send_drop_info_timestamp = EngineAdapter::Instance().Time();

	role->NotifyFBFinish();

	UInt32 monster_num = m_scene->MonsterNum();
	for (int i = 0; i < static_cast<int>(monster_num); i++)
	{
		Monster *monster = m_scene->GetMonsterByIndex(i);
		if (nullptr != monster && monster->GetMonsterId() != LOGIC_CONFIG->GetHolidayGuardConfig().GetLifeHolidayMonsterID())
		{
			monster->ForceSetDead();
		}
	}

	RandActivityHolidayGuard *holiday_guard_activity = (RandActivityHolidayGuard*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_HOLIDAY_GUARD);
	if (nullptr == holiday_guard_activity)
	{
		return;
	}

	RoleFBTowerDefend *tower_defend = role->GetFBTowerDefend();
	if(nullptr == tower_defend)
	{
		return;
	}

	tower_defend->SetPersonalHolidayGuardFBKillMonsterCount(m_kill_monster_count);

	role->GetRoleActivity()->OnHolidayGuardAddKillMonsterCount(m_kill_monster_count);

	holiday_guard_activity->AddOrReplaceRole(role);

	int have_reward_count = tower_defend->GetPersonalHolidayGuardFBEnterCount();
	int everyday_can_fetch_reward_count = LOGIC_CONFIG->GetHolidayGuardConfig().GetEverydayLeaveFbRewardCount();
	if (have_reward_count <= everyday_can_fetch_reward_count )
	{
		ItemConfigData reward_list_cfg[MAX_ATTACHMENT_ITEM_NUM];
		int ret = LOGIC_CONFIG->GetHolidayGuardConfig().GetLeaveFbReward(reward_list_cfg, MAX_ATTACHMENT_ITEM_NUM);
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < ret; ++i)
		{
			role->GetKnapsack()->PutOrMail(reward_list_cfg[i], PUT_REASON_HOLIDAY_GUARD_REWARD);
		}
	}

	this->DelayKickOutAllRole();
}

void SpecialHolidayGuard::CheckAttackMonsterStatus(unsigned long interval, time_t now_second)
{
	if (now_second >= m_last_check_monster_status_time + 1)
	{
		m_last_check_monster_status_time = static_cast<unsigned int>(now_second);

		for (int i = 0; i < TOWERDEFEND_PATH_MAX; ++i)
		{
			if (m_refresh_left_count[i] > 0) return; // 怪都没出完呢
		}
		UInt32 monster_num = m_scene->MonsterNum();
		for (int i = 0; i < (int)monster_num; ++i)
		{
			Monster *monster = m_scene->GetMonsterByIndex(i);
			if (nullptr != monster && monster->IsAlive() && MonsterInitParam::AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK == monster->GetAIType())
			{
				return;
			}
		}

		this->OnAllAttackMonsterDead();
	}
}

void SpecialHolidayGuard::OnAllAttackMonsterDead()
{
	if (!m_is_finish && m_is_last_wave)
	{
		m_is_finish = true;

		this->OnFinish(true);
	}
}

void SpecialHolidayGuard::NotifyWarning(short warning_type, short percent)
{
	Protocol::SCHolidayGuardWarning ttdw;
	ttdw.warning_type = warning_type;
	ttdw.percent = percent;

	m_scene->SendToRole((const char *)&ttdw, sizeof(ttdw));
}
