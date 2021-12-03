#include "specialterritorywar.hpp"
#include "other/event/eventhandler.hpp"
#include "obj/character/monster.h"
#include "obj/character/role.h"
#include "obj/obj.h"

#include "scene/scenemanager.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "scene/worldshadow/worldshadow.hpp"

#include "monster/monsterpool.h"
#include "item/itempool.h"
#include "world.h"
#include "gamelog.h"

#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectinvisible.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "effect/skilleffect/effectreboundhurt.hpp"
#include "effect/skilleffect/effectwudiprotect.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectinterval.hpp"

#include "engineadapter.h"
#include "internalcomm.h"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/servercommon.h"

#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"

#include "gameworld/skill/skillpool.hpp"
#include "other/route/mailroute.hpp"
#include "protocal/msgscene.h"

#include "other/event/eventhandler.hpp"
#include "global/rank/rankmanager.hpp"
#include <algorithm>
#include "global/usercache/usercache.hpp"
#include "global/activity/activitymanager.hpp"
#include "ai/monsterai/monsteraibase.hpp"
#include "ai/monsterai/specialai/monsteraigongchengzhan.hpp"
#include "effect/sceneeffect/sceneeffect.hpp"
#include "globalconfig/globalconfig.h"
#include "ai/monsterai/specialai/monsteraibattlefield.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include <math.h>
#include "global/rank/topranker.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"

#include "ai/monsterai/specialai/monsteraigterritorywartower.hpp"

#include "obj/otherobj/trigger.h"
#include "obj/otherobj/effectobj.h"
#include "global/randactivity/randactivitymanager.hpp"
#include "config/activityconfig/territorywarconfig.hpp"

#include "item/knapsack.h"
#include "other/shop/roleshop.hpp"
#include "other/specialappearance/specialappearance.hpp"
#include "other/rolegoal/rolegoal.hpp"

SpecialTerritoryWar::SpecialTerritoryWar(Scene *scene):SpecialLogic(scene),m_is_finish(false),m_red_total_credit(0),m_blue_total_credit(0), m_read_next_can_buy_tower_wudi(0),
	m_blue_next_can_buy_tower_wudi(0), red_building_survive_flag(0), blue_building_survive_flag(0), m_center_relive_state(0),m_winner_side(-1),m_add_credit_interval(0),m_next_add_credit_time(0),
	center_relive_point_id(0), red_fortress_id(0), blue_fortress_id(0), m_building_be_attack_notice_interval(0), m_red_next_notice_building_be_attack_time(0), m_blue_next_notice_building_be_attack_time(0),
	m_center_point_next_notice_building_be_attack_time(0), m_last_first_guild_broadcast_time(0), m_last_best_killer_broadcast_time(0)
{
}

SpecialTerritoryWar::~SpecialTerritoryWar()
{
}

bool SpecialTerritoryWar::CanDestroy()
{
	if (ActivityManager::Instance().IsActivityClose(ACTIVITY_TYPE_TERRITORY_WAR))
	{
		return true;
	}
	return false;
}

void SpecialTerritoryWar::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_is_finish) return;

	if(now_second > (time_t)m_next_add_credit_time)
	{
		m_next_add_credit_time += m_add_credit_interval;
		this->AllAddCredit();
	}

	if (m_luck.next_lucky_timestamp > 0 && now_second >= m_luck.next_lucky_timestamp)
	{
		m_luck.next_lucky_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetTerritoryWarConfig().GetOtherCfg().lucky_interval;

		this->GaveLuckyItemToUser();
	}

	this->BroadcastZhanChang(now_second);
}

void SpecialTerritoryWar::OnActivityStatusChange(int status)
{
	switch (status)
	{
	case ACTIVITY_STATUS_STANDY:
		{
			this->OnActivityStandby();
		}
		break;
	case ACTIVITY_STATUS_OPEN:
		{
			this->OnActivityStart();
		}
		break;

	case ACTIVITY_STATUS_CLOSE:
		{
			this->OnActivityClose();
		}
		break;
	}
}

void SpecialTerritoryWar::OnRoleEnterScene(Role *role)
{
	if (INVALID_GUILD_ID == role->GetGuildID() || !ActivityManager::Instance().IsActivityOpen(ACTIVITY_TYPE_TERRITORY_WAR))
	{
		this->DelayKickOutRole(role);
		return;
	}

	BuildingInfo& red_fortress_info = m_building_map[red_fortress_id];
	BuildingInfo& blue_fortress_info = m_building_map[blue_fortress_id];

	if (m_is_finish || red_fortress_info.building_state == ENBUILDINGSTATUS_NONEXIST || blue_fortress_info.building_state == ENBUILDINGSTATUS_NONEXIST)
	{
		role->NoticeNum(errornum::EN_TW_WAR_IS_FINISH);
		this->DelayKickOutRole(role);
		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
	if (NULL == guild)
	{
		this->DelayKickOutRole(role);
		return;
	}

	JoinerInfo *joiner = this->GetJoinerInfo(role->GetUID());
	if (NULL == joiner)
	{
		joiner = new JoinerInfo();
		joiner->uid = role->GetUID();
		joiner->guild_id = role->GetGuildID();
		int temp_side = guild->GetGuildBattleRank();
		joiner->side = temp_side % 2;

		m_joiner_map[joiner->uid] = joiner;

		ROLE_LOG_QUICK6(LOG_TYPE_ACTIVITY_TERRITORYWAR, role, role->GetLevel(), 0, "Join", NULL);
	}

	this->SyncGlobalInfo(role);
	this->SyncRoleInfo(role);
	this->SendLuckyInfoToUser(role);

	EventHandler::Instance().OnEnterTerritoryWar(role);
}

void SpecialTerritoryWar::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL != role)
	{
		role->GetSpecialAppearance()->SetFbAppearance(0, 0, 0, true);

		JoinerInfo *joiner = this->GetJoinerInfo(role->GetUID());
		if (NULL != joiner)
		{
			joiner->special_image_id = INVAILD_APPEARANCE;
			joiner->ResetSkillList();
			this->ClearEdgeMedicineEffect(role);
		}
	}
}

void SpecialTerritoryWar::CreateAllBuilding()
{
	std::map<int, BuildingInfo>::iterator it = m_building_map.begin();
	for (; m_building_map.end() != it; it++)
	{
		Monster *monster = MONSTERPOOL->CreateMonster(it->second.building_id, m_scene, it->second.posi);
		if (NULL != monster)
		{
			if (MonsterInitParam::AITYPE_SPECIALAI_TERRITORYWAR_TOWER == monster->GetAIType())
			{
				MonsterAITerritoryWarTower *ai = (MonsterAITerritoryWarTower*)monster->GetMonsterAI();
				ai->SetSide(it->second.side);
			}

			it->second.building_state = ENBUILDINGSTATUS_EXIST;
			it->second.max_hp = static_cast<long long>(monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP));
			it->second.current_hp = static_cast<long long>(monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP));
		}
	}
}

bool SpecialTerritoryWar::CreateOneBuilding(BuildingInfo &building_info)
{
	Monster *monster = MONSTERPOOL->CreateMonster(building_info.building_id, m_scene, building_info.posi);
	if (NULL != monster)
	{
		if (MonsterInitParam::AITYPE_SPECIALAI_TERRITORYWAR_TOWER == monster->GetAIType())
		{
			MonsterAITerritoryWarTower *ai = (MonsterAITerritoryWarTower*)monster->GetMonsterAI();
			ai->SetSide(building_info.side);
		}
		building_info.building_state = ENBUILDINGSTATUS_EXIST;
		building_info.obj_id = monster->GetId();
		building_info.max_hp = static_cast<long long>(monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP));
		building_info.current_hp = static_cast<long long>(monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP));
		return true;
	}
	
	return false;
}


void SpecialTerritoryWar::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
	if (NULL == cha || delta_hp >= 0) return;

	Obj *attacker = m_scene->GetObj(enemy_id);
	if (NULL == attacker) return;

	if (Obj::OBJ_TYPE_ROLE != attacker->GetObjType()) return;

	Role * joiner_attacker = (Role*)attacker;
	if (NULL == joiner_attacker) return;

	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType())
	{
		Role *joiner_be_attack = (Role*)cha;
		if (NULL == joiner_be_attack) return;

		JoinerInfo *joiner = this->GetJoinerInfo(joiner_be_attack->GetUID());
		if (NULL == joiner) return;

		joiner->attack_player_map[joiner_attacker->GetUID()] = (unsigned int)(EngineAdapter::Instance().Time());

		return;
	}

	if (Obj::OBJ_TYPE_MONSTER == cha->GetObjType())
	{
		Monster * monster_be_attack = (Monster*)cha;
		if (NULL == monster_be_attack) return;

		unsigned short monster_id = monster_be_attack->GetMonsterId();
		
		if (!this->IsBuilding(monster_id))
			return;

		unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		
		BuildingInfo& building_info = m_building_map[monster_be_attack->GetMonsterId()];
		building_info.attack_player_map[joiner_attacker->GetUID()] = (unsigned int)(EngineAdapter::Instance().Time());
		building_info.current_hp = cha->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);

		if (0 == building_info.side && now_sec >= m_blue_next_notice_building_be_attack_time)
		{
			int length_one = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_territorywar_monster_build_be_attack, monster_id);
			if (length_one > 0)
			{
				this->NotifySideSystemMsg(gamestring::GAMESTRING_BUF, length_one, SYS_MSG_ACTIVE_NOTICE, 0);
			}

			m_blue_next_notice_building_be_attack_time = now_sec + m_building_be_attack_notice_interval; 
		}
		else if (1 == building_info.side && now_sec >= m_red_next_notice_building_be_attack_time)
		{
			int length_one = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_territorywar_monster_build_be_attack, monster_id);
			if (length_one > 0)
			{
				this->NotifySideSystemMsg(gamestring::GAMESTRING_BUF, length_one, SYS_MSG_ACTIVE_NOTICE, 1);
			}

			m_red_next_notice_building_be_attack_time = now_sec + m_building_be_attack_notice_interval; 
		}

		if (center_relive_point_id == monster_id || red_fortress_id == monster_id || blue_fortress_id == monster_id)
		{
			this->SyncGlobalInfoToAll();

			if(center_relive_point_id == monster_id && now_sec >= m_center_point_next_notice_building_be_attack_time)
			{
				int length_one = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_territorywar_monster_build_be_attack, monster_id);
				if (length_one > 0)
				{
					this->NotifySideSystemMsg(gamestring::GAMESTRING_BUF, length_one, SYS_MSG_ACTIVE_NOTICE, building_info.side);
				}

				m_center_point_next_notice_building_be_attack_time = now_sec + m_building_be_attack_notice_interval;
			}
		}

		return;
	}
}

void SpecialTerritoryWar::OnHurtOther(Character *cha, Character *target)
{
	if (NULL == cha && NULL == target)
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE == cha->GetObjType() && Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		Role *hurt_role = (Role *)cha;
		Role *target_role = (Role *)target;

		JoinerInfo *target_joiner = this->GetJoinerInfo(target_role->GetUID());
		if (NULL != target_joiner)
		{
			target_joiner->hurt_map[hurt_role->GetUserId()] = EngineAdapter::Instance().Time();
		}
	}
}

void SpecialTerritoryWar::OnCharacterDie(Character *dead, ObjID killer_id)
{
	if (NULL == dead) return;

	Obj *killer_obj = m_scene->GetObj(killer_id);
	if (NULL == killer_obj) return;

	// 玩家击杀玩家
	if (Obj::OBJ_TYPE_ROLE == dead->GetObjType() && Obj::OBJ_TYPE_ROLE == killer_obj->GetObjType())
	{
		Role *death = (Role*)dead;
		Role *killer = (Role *)killer_obj;
		if (NULL == dead || NULL == killer) return;

		JoinerInfo *joiner_death = this->GetJoinerInfo(death->GetUID());
		JoinerInfo *joiner_killer = this->GetJoinerInfo(killer->GetUID());
		if (NULL == joiner_death || NULL == joiner_killer) return;

		const TerritroyWarOtherCfg& other_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetOtherCfg();

		// 如果被击杀者有载具加击杀载具分，否则击杀角色分
		if (INVAILD_APPEARANCE < joiner_death->special_image_id && MAX_APPEARANCE > joiner_death->special_image_id)
		{
			std::map<int, unsigned int>::iterator killer_map = joiner_death->attack_player_map.find(killer->GetUID());
			if (killer_map == joiner_death->attack_player_map.end())
				return;

			this->PersonalAddCredit(killer, other_cfg.kill_car_credit);
			this->AddGuildCredit(killer, other_cfg.kill_car_guild_credit);
			joiner_death->attack_player_map.erase(killer_map);
			
			unsigned int now_sec = (unsigned int)(EngineAdapter::Instance().Time());
			std::map<int, unsigned int>::iterator itor = joiner_death->attack_player_map.begin();
			for (; itor != joiner_death->attack_player_map.end(); itor++)
			{
				if (now_sec - itor->second < other_cfg.assist_vaild_time)
				{
					this->PersonalAddCredit(itor->first, other_cfg.kill_car_assist_credit);
				}
			}

			joiner_death->attack_player_map.clear();
		}
		else if (joiner_death->special_image_id == INVAILD_APPEARANCE)
		{
			std::map<int, unsigned int>::iterator killer_map = joiner_death->attack_player_map.find(killer->GetUID());
			if (killer_map == joiner_death->attack_player_map.end())
				return;

			this->PersonalAddCredit(killer, other_cfg.kill_player_credit);
			this->AddGuildCredit(killer, other_cfg.kill_player_guild_credit);
			joiner_death->attack_player_map.erase(killer_map);

			unsigned int now_sec = (unsigned int)(EngineAdapter::Instance().Time());
			std::map<int, unsigned int>::iterator itor = joiner_death->attack_player_map.begin();
			for (; itor != joiner_death->attack_player_map.end(); itor++)
			{
				if (now_sec - itor->second < other_cfg.assist_vaild_time)
				{
					this->PersonalAddCredit(itor->first, other_cfg.kill_player_assist_credit);
				}
			}

			joiner_death->attack_player_map.clear();
		}

		for (std::map<UserID, time_t>::iterator it = joiner_death->hurt_map.begin(), 
			end = joiner_death->hurt_map.end(); it != end; ++ it)
		{
			if (it->second + ASSIST_LIMIT_TIME >= EngineAdapter::Instance().Time() && it->first != killer->GetUserId())
			{
				JoinerInfo *joiner_assist = this->GetJoinerInfo(UidToInt(it->first));
				if (NULL != joiner_assist)
				{
					joiner_assist->assist_count++;
					joiner_assist->lianzhan++;
					Role *assist_role = m_scene->GetRoleByUID(joiner_assist->uid);
					if (NULL != assist_role)
					{
						this->CheckLianZhanNotice(assist_role, death, true);
					}
				}
			}
		}

		++ joiner_killer->kill_count;
		++ joiner_killer->lianzhan;

		this->CheckLianZhanNotice(killer, death);

		joiner_death->lianzhan = 0;
		joiner_death->hurt_map.clear();

		death->GetSpecialAppearance()->SetFbAppearance(0, 0, 0, true);
		joiner_death->special_image_id = INVAILD_APPEARANCE;
		joiner_death->ResetSkillList();
		this->NotifyImage(death);	
	}

	// 玩家击杀怪物
	else if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType() && Obj::OBJ_TYPE_ROLE == killer_obj->GetObjType())
	{
		Monster *monster = (Monster*)dead;
		Role *killer = (Role *)killer_obj;
		if (NULL == monster || NULL == killer) return;

		JoinerInfo *joiner = this->GetJoinerInfo(killer->GetUID());
		if (NULL == joiner) return;

		if (this->IsBuilding(monster->GetMonsterId()))
		{
			BuildingInfo &building_info = m_building_map[monster->GetMonsterId()];
			building_info.building_state = ENBUILDINGSTATUS_NONEXIST;
			building_info.current_hp = 0;

			if (ENSIDESTATUS_RED == building_info.side)
			{
				red_building_survive_flag |= (1 << building_info.building_index);
				m_red_next_notice_building_be_attack_time = 0;
			}
			else if (ENSIDESTATUS_BLUE == building_info.side)
			{
				blue_building_survive_flag |= (1 <<building_info.building_index);
				m_blue_next_notice_building_be_attack_time = 0;
			}
			
			std::map<int, unsigned int>::iterator killer_map = building_info.attack_player_map.find(killer->GetUID());
			if (killer_map == building_info.attack_player_map.end())
				return;

			this->PersonalAddCredit(killer, building_info.kill_personal_credit);
			this->AddGuildCredit(killer, building_info.kill_guild_credit);
			building_info.attack_player_map.erase(killer_map);

			unsigned int now_sec = (unsigned int)(EngineAdapter::Instance().Time());
			std::map<int, unsigned int>::iterator itor = building_info.attack_player_map.begin();
			for (; itor != building_info.attack_player_map.end(); itor++)
			{
				if (now_sec - itor->second < building_info.assist_vaild_time)
				{
					this->PersonalAddCredit(itor->first, building_info.kill_assist_credit);
				}
			}

			building_info.attack_player_map.clear();

			if (center_relive_point_id != monster->GetMonsterId())
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_territorywar_monster_build_be_kill, monster->GetMonsterId());
				if (length > 0)
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
				}
			}

			if (center_relive_point_id == monster->GetMonsterId() && ENBUILDINGSTATUS_NONEXIST == building_info.building_state)
			{
				this->CreateOneBuilding(building_info);
				building_info.side = joiner->side;
				m_center_relive_state = killer->GetGuildID();
				this->SyncGlobalInfoToAll();
				m_center_point_next_notice_building_be_attack_time = 0;

				// 中央水晶占领方公告
				int length_one = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_territorywar_center_build_be_occupy_my_side, monster->GetMonsterId());
				if (length_one > 0)
				{
					this->NotifySideSystemMsg(gamestring::GAMESTRING_BUF, length_one, SYS_MSG_ACTIVE_NOTICE, building_info.side);
				}

				// 中心水晶被占领方公告
				if (1 == building_info.side)
				{
					int length_two = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_territorywar_center_build_be_occupy_other_side, monster->GetMonsterId());
					if (length_two > 0)
					{
						this->NotifySideSystemMsg(gamestring::GAMESTRING_BUF, length_two, SYS_MSG_ACTIVE_NOTICE, 0);
					}
				}
				else
				{
					int length_two = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_territorywar_center_build_be_occupy_other_side, monster->GetMonsterId());
					if (length_two > 0)
					{
						this->NotifySideSystemMsg(gamestring::GAMESTRING_BUF, length_two, SYS_MSG_ACTIVE_NOTICE, 1);
					}
				}
			}

			if (red_fortress_id == monster->GetMonsterId() && ENBUILDINGSTATUS_NONEXIST == building_info.building_state)
			{
				m_winner_side = 0;
				this->OnActivityClose();
			}

			if (blue_fortress_id == monster->GetMonsterId() && ENBUILDINGSTATUS_NONEXIST == building_info.building_state)
			{
				m_winner_side = 1;
				this->OnActivityClose();
			}

			if (center_relive_point_id != monster->GetMonsterId() && red_fortress_id != monster->GetMonsterId() && blue_fortress_id != monster->GetMonsterId())
			{
				EventHandler::Instance().OnPutTerritorWarKillTa(killer);
			}
		}
		else
		{
			int add_guaji_credit = this->GetGuajiMonsterCredit(monster->GetMonsterId());
			this->PersonalAddCredit(killer, add_guaji_credit);
		}
	}
	else
	{
		if (Obj::OBJ_TYPE_ROLE == dead->GetObjType())
		{
			Role *death = (Role*)dead;
			if (NULL == dead) return;

			death->GetSpecialAppearance()->SetFbAppearance(0, 0, 0, true);
			
			JoinerInfo *death_joiner = this->GetJoinerInfo(death->GetUID());
			if (NULL != death_joiner)
			{
				death_joiner->lianzhan = 0;
				death_joiner->hurt_map.clear();
				death_joiner->special_image_id = INVAILD_APPEARANCE;
				death_joiner->ResetSkillList();
			}
		}
		
	}
}

bool SpecialTerritoryWar::SpecialRelive(Role *role, int realive_type)
{
	if (m_is_finish || NULL == role)
	{
		return false;
	}

	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_HERE_GOLD == realive_type)
	{
		return false;
	}

	Posi realive_pos(0, 0);

	LOGIC_CONFIG->GetTerritoryWarConfig().GetTerritoryWarEnterInfo(role,&realive_pos);

	role->ReAlive(SPECIAL_ACTIVITY_FREE_REALIVE_HP_PER, 40, realive_pos);

	return true;
}

bool SpecialTerritoryWar::SpecialIsEnemy(Role *role, Obj *obj)
{
	if (NULL == role || NULL == obj) return false;

	if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
	{
		Monster *building_monster = (Monster*)obj;
		if (this->IsBuilding(building_monster->GetMonsterId()))
		{
			const BuildingInfo &building_info = m_building_map[building_monster->GetMonsterId()];
			JoinerInfo *joiner = this->GetJoinerInfo(role->GetUID());
			if (NULL != joiner)
			{
				if (joiner->side == building_info.side)
					return false;
			}

			if (0 == building_info.preposition_building_1 && 0 == building_info.preposition_building_2)
				return true;

			if (this->IsBuilding(building_info.preposition_building_1))
			{
				if (ENBUILDINGSTATUS_EXIST == m_building_map[building_info.preposition_building_1].building_state)
					return false;
			}

			if (this->IsBuilding(building_info.preposition_building_2))
			{
				if (ENBUILDINGSTATUS_EXIST == m_building_map[building_info.preposition_building_2].building_state)
					return false;
			}
		}
	}

	if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *target_role = (Role*)obj;
		if (NULL == target_role) return false;

		if (role->GetGuildID() == target_role->GetGuildID())
			return false;
	}

	return SpecialLogic::SpecialIsEnemy(role, obj);
}

bool SpecialTerritoryWar::PerformSkill(Role *role, int skill_index, const Posi &pos, ObjID target_id)
{
	if (NULL == role ) return false;

	if (m_is_finish) return false;

	if (TW_SKILL_INVALID >= skill_index || TW_SKILL_MAX <= skill_index) return false;

	JoinerInfo *joiner_attcker = this->GetJoinerInfo(role->GetUID());
	if (NULL == joiner_attcker) return false;

	if (INVAILD_APPEARANCE == joiner_attcker->special_image_id) return false;

	if (!joiner_attcker->IsValidSkill(skill_index)) return false;

	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	TerritoryWarSkillItem * joiner_skill = joiner_attcker->GetSkillItem(skill_index);
	if (NULL == joiner_skill) return false;

	const TerritroyWarSkillItemCfg *skill_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetSkillItemCfg(skill_index);
	if (NULL == skill_cfg) return false;

	if (now_second < joiner_skill->last_perform_time + skill_cfg->cd_s)
	{
		role->NoticeNum(errornum::EN_ROLE_FIGHT_CD_NOT_ENOUGH);
		return false;
	}

	if (TW_SKILL_INDEX6 == skill_index)
	{
		if (target_id == role->GetId())
			return false;

		Obj *target_obj = m_scene->GetObj(target_id);
		if (NULL == target_obj) return false;

		if (Obj::OBJ_TYPE_ROLE != target_obj->GetObjType()) return false;

		Role *target_role = (Role*)target_obj;
		
		JoinerInfo *target_joiner = this->GetJoinerInfo(target_role->GetUID());
		if (NULL == target_joiner) return false;

		if (target_joiner->special_image_id == INVAILD_APPEARANCE)
			return false;
	}

	// 检查攻击距离够不够
	{
		Obj* target_obj = m_scene->GetObj(target_id);
		if (NULL == target_obj) return false;

		int atk_dis = skill_cfg->distance + 2; // 容错 比客户端距离大2

		Posi target_pos = target_obj->GetPos(); 

		if ((target_pos - role->GetPos()).IsLongerThan(atk_dis))
		{
			role->NoticeNum(errornum::EN_ROLE_FIGHT_DISTANCE_NOT_ENOUGH);

			Protocol::SCSkillTargetPos stp;
			stp.target_obj_id = target_obj->GetId();
			stp.pos_x = target_obj->GetPos().x;
			stp.pos_y = target_obj->GetPos().y;

			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&stp, sizeof(stp));

			return false;
		}
	}

	int num = 0;
	static ObjID target_list[32] = {INVALID_OBJ_ID};
	// 获取攻击目标列表
	{
		{
			Protocol::SCPerformAOESkill psaoe;
			psaoe.character = role->GetId();
			psaoe.skill = skill_index;
			psaoe.pos_x = pos.x;
			psaoe.pos_y = pos.y;
			psaoe.aoe_reason = 0;
			psaoe.target = target_id;
			psaoe.skill_data = 0;
			if (NULL != role->GetScene()) role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), &psaoe, sizeof(Protocol::SCPerformAOESkill));
		}

		const int max_num = skill_cfg->enemy_num;		// 受击最大数量
		const int range = skill_cfg->distance;				// 攻击距离

		static ObjID obj_list[256] = {0};
		Scene *scene = role->GetScene();
		int count = scene->GetZoneMatrix()->GetObjByArea(pos, range, range, obj_list, 256);

		// 第1个一定是主目标
		target_list[0] = target_id;
		num++;

		for (int i = 0; num < max_num && i < count; ++i)
		{
			Obj *obj = scene->GetObj(obj_list[i]);
			if (NULL != obj && Obj::IsCharactor(obj->GetObjType()) && role->IsEnemy(obj))
			{
				Character *target_cha = (Character *)obj;
				if (target_cha->GetEffectList()->IsInvisible()) continue;

				if (target_id == obj_list[i])
					continue;

				if (!scene->GetMap()->SaveArea(obj->GetPos().x, obj->GetPos().y))
				{
					target_list[num++] = obj_list[i];
				}
			}
		}
	}

	if (num <= 0)
		return false;

	joiner_skill->last_perform_time = static_cast<unsigned>(EngineAdapter::Instance().Time());
	this->SyncRoleInfo(role);

	for (int j = 0; j < num; j++)
	{
		this->DoPerformSkill(role, skill_cfg, target_list[j]);
	}

	return true;
}

long long SpecialTerritoryWar::SpecialRecalcInjure(Character *attacker, Character *injurer, long long injure)
{
	if (NULL == attacker || NULL == injurer) return injure;

	if (injurer->GetId() == attacker->GetId()) return injure;

	// 攻击者如果是人且处于变身状态直接return ,因为执行载具技能逻辑时已经计算过伤害
	if (Obj::OBJ_TYPE_ROLE == attacker->GetObjType())
	{
		Role *attacker_role = (Role*)attacker;
		JoinerInfo *attacker_joiner = this->GetJoinerInfo(attacker_role->GetUID());
		if (NULL == attacker_joiner) return injure;

		if (INVAILD_APPEARANCE != attacker_joiner->special_image_id)
			return injure;
	}

	switch(injurer->GetObjType())
	{
	case Obj::OBJ_TYPE_MONSTER:
		{
			Monster *injurer_monster = (Monster *)injurer;

			// 人没有变身状态打各种建筑
			if (Obj::OBJ_TYPE_ROLE == attacker->GetObjType() && this->IsBuilding(injurer_monster->GetMonsterId()))
			{
				const TerritoryWarFixHurtCfg& fix_hurt_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetFixHurtCfg();

				if (center_relive_point_id == injurer_monster->GetMonsterId())
				{
					injure = fix_hurt_cfg.fix_hurt_on_relive;
				}
				else if (red_fortress_id == injurer_monster->GetMonsterId() || blue_fortress_id == injurer_monster->GetMonsterId())
				{
					injure = fix_hurt_cfg.fix_hurt_on_fortress;
				}
				else
				{
					injure = fix_hurt_cfg.fix_hurt_on_tower;
				}
			}
		}
		break;

	case Obj::OBJ_TYPE_ROLE:
		{
			Role *injurer_role = (Role*)injurer;

			JoinerInfo *injure_join_role = this->GetJoinerInfo(injurer_role->GetUID());
			if (NULL == injure_join_role) return injure;

			// 塔打人，打各种车
			if (Obj::OBJ_TYPE_MONSTER == attacker->GetObjType())
			{
				Monster *attacke_monster = (Monster*)attacker;
				if (this->IsBuilding(attacke_monster->GetMonsterId()))
				{
					BuildingInfo &building_info = m_building_map[attacke_monster->GetMonsterId()];

					switch(injure_join_role->special_image_id)
					{
					case INVAILD_APPEARANCE:
						{
							injure = static_cast<long long>(injurer_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (building_info.tower_attack_man_per * ROLE_ATTR_PER));
						}
						break;

					case CAR_FIGHT_APEARANCE:
						{
							injure = static_cast<long long>(injurer_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (building_info.tower_attack_fight_car_per * ROLE_ATTR_PER));
						}
						break;

					case CAR_FANG_APEARANCE:
						{
							injure = static_cast<long long>(injurer_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (building_info.tower_attack_fang_car_per * ROLE_ATTR_PER));
						}	
						break;

					case CAR_CURE_APEARANCE:
						{
							injure = static_cast<long long>(injurer_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (building_info.tower_attack_cure_car_per * ROLE_ATTR_PER));
						}
						break;

					default:
						break;
					}
				}
			}
			// 人没有变身，对各种车伤害
			else if (Obj::OBJ_TYPE_ROLE == attacker->GetObjType())
			{
				const TerritoryWarFixHurtCfg& fix_hurt_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetFixHurtCfg();

				switch(injure_join_role->special_image_id)
				{
				case CAR_FIGHT_APEARANCE:
					{
						injure = static_cast<long long>(injurer_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (fix_hurt_cfg.man_attack_fight_car_per * ROLE_ATTR_PER));
					}
					break;

				case CAR_FANG_APEARANCE:
					{
						injure = static_cast<long long>(injurer_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (fix_hurt_cfg.man_attack_fang_car_per * ROLE_ATTR_PER));
					}
					break;

				case CAR_CURE_APEARANCE:
					{
						injure = static_cast<long long>(injurer_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (fix_hurt_cfg.man_attack_cure_car_per * ROLE_ATTR_PER));
					}
					break;

				default:
					break;
				}
			}
		}
		break;
	}

	return injure;
}

bool SpecialTerritoryWar::SpecialCanPassiveSkillEffect(Character *deliver, ObjID target, int skill_id)
{
	if(NULL == deliver) return false;

	if (Obj::OBJ_TYPE_ROLE == deliver->GetObjType())
	{
		Role * role = (Role*)deliver;
		if (NULL == role) return false;
		
		JoinerInfo *joiner = this->GetJoinerInfo(role->GetUID());
		if (NULL == joiner) return false;

		if (INVAILD_APPEARANCE != joiner->special_image_id)
			return false;
	}

	Obj *target_obj = m_scene->GetObj(target);
	if (NULL == target_obj) return false;

	if (Obj::OBJ_TYPE_MONSTER == target_obj->GetObjType())
	{
		Monster * target_monster = (Monster *)target_obj;

		if (this->IsBuilding(target_monster->GetMonsterId()))
		{
			return false;
		}
	}
	else if (Obj::OBJ_TYPE_ROLE == target_obj->GetObjType())
	{
		Role * target_role = (Role *)target_obj;

		JoinerInfo *joiner = this->GetJoinerInfo(target_role->GetUID());
		if (NULL == joiner) return false;

		if (INVAILD_APPEARANCE != joiner->special_image_id)
			return false;
	}

	return true;
}

void SpecialTerritoryWar::OnTrigger(Role *role, Trigger *trigger, int trigger_action_param_list[TRIGGER_ACTION_PARAM_MAX], bool *remove)
{
	if (NULL == role || NULL == trigger || NULL == remove || NULL == trigger_action_param_list) return;

	if (TRIGGER_ACTION_TYPE_ICE_LANDMINE != trigger->GetActionType() && TRIGGER_ACTION_TYPE_FIRE_LANDMINE != trigger->GetActionType())
		return;
	
	const TerritroyWarFightShopCfg *fight_shop_cfg = NULL;

	if (TRIGGER_ACTION_TYPE_ICE_LANDMINE == trigger->GetActionType())
	{
		fight_shop_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetFightShopCfg(LANDMINE, ICE_LANDMINE);
	}
	else
	{
		fight_shop_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetFightShopCfg(LANDMINE, FIER_LANDMINE);
	}
	
	if (NULL == fight_shop_cfg) return;

	bool has_role = false;
	static ObjID obj_list[64] = {0};
	int count = m_scene->GetZoneMatrix()->GetObjByArea(trigger->GetPos(), fight_shop_cfg->param5, fight_shop_cfg->param5, obj_list, sizeof(obj_list) / sizeof(obj_list[0]));
	for (int i = 0; i < count && i < (int)sizeof(obj_list) / (int)sizeof(obj_list[0]); ++ i)
	{
		Obj *obj = m_scene->GetObj(obj_list[i]);
		if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
		{
			Role *role = (Role*)obj;
			if (role->IsAlive()) 
			{
				JoinerInfo *join_role = this->GetJoinerInfo(role->GetUID());
				if (NULL == join_role)
				 continue;

				if (join_role->side == trigger->GetAffiliation())
					continue;

				if (INVAILD_APPEARANCE == join_role->special_image_id)
				{
					Attribute hurt = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (fight_shop_cfg->param1 * ROLE_ATTR_PER));
					EffectOnce *effect = new EffectOnce(trigger->GetId(), 1, -hurt, EffectBase::FIGHT_TYPE_NORMAL);
					effect->SetSave(false);
					role->AddEffect(effect);
				}
				else
				{
					Attribute hurt = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (fight_shop_cfg->param2 * ROLE_ATTR_PER));
					EffectOnce *effect = new EffectOnce(trigger->GetId(), 1, -hurt, EffectBase::FIGHT_TYPE_NORMAL);
					effect->SetSave(false);
					role->AddEffect(effect);
				}

				if (TRIGGER_ACTION_TYPE_ICE_LANDMINE == trigger->GetActionType())
				{
					EffectSpecialState *effect_state = new EffectSpecialState(trigger->GetId(), 1, fight_shop_cfg->param3 * 1000, EBT_XUANYUN, 1);
					effect_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
					effect_state->SetClientEffectType(EffectBase::ECT_TW_LANDMINE_FROZEN);
					role->AddEffect(effect_state);
				}

				has_role = true;
				*remove = true;
			}
		}
	}

	if (has_role)
	{
		trigger->ChangeTriggerCount(1);
		switch (trigger->GetActionType())
		{
		case TRIGGER_ACTION_TYPE_ICE_LANDMINE:
			{	
				EffectObj::CreateEffectObject(m_scene, trigger->GetPos(), Trigger::PRODUCT_ID_TRIGGER_SPECIAL_ICE_LANDMINE, EffectBase::PRODUCT_METHOD_TRIGGER,  (EngineAdapter::Instance().Time() + fight_shop_cfg->param6));
			}
			break;

		case TRIGGER_ACTION_TYPE_FIRE_LANDMINE:
			{
				EffectObj::CreateEffectObject(m_scene, trigger->GetPos(), Trigger::PRODUCT_ID_TRIGGER_SPECIAL_FIRE_LANDMINE, EffectBase::PRODUCT_METHOD_TRIGGER, (EngineAdapter::Instance().Time() + fight_shop_cfg->param6));
			}
			break;
		}
	}
}

void SpecialTerritoryWar::OnReliveShopBuyReq(Role *role, short goods_id)
{
	if(NULL == role || TOWER_WUDI > goods_id || MAX_RLIVE_SHOP_INDEX <= goods_id)
	{
		return;
	}

	if (TOWER_WUDI == goods_id)
	{
		this->BuyTowerWudi(role);
	}
	else if (TRANSPORT == goods_id)
	{
		this->BuyTransport(role);
	}
	else if (TRANSPORT_TO_CENTER == goods_id)
	{
		this->BuyTransportToCenter(role);
	}
}

void SpecialTerritoryWar::OnFightShopBuyReq(Role *role, short type, short goods_id)
{
	if(NULL == role || BIAN_SHEN > type || FIGHT_SHOP_MAX_TYPE <= type)
	{
		return;
	}

	if(BIAN_SHEN == type)
	{
		this->BuyCar(role, goods_id);
	}
	else if(MEDICAMENT == type)
	{
		this->BuyMedicine(role, goods_id);
	}

	else if(LANDMINE == type)
	{
		this->BuyLandMine(role, goods_id);
	}
	else
	{
		return;
	}
}

SpecialTerritoryWar::JoinerInfo* SpecialTerritoryWar::GetJoinerInfo(int uid)
{
	std::map<int, JoinerInfo *>::iterator iter = m_joiner_map.find(uid);
	if (m_joiner_map.end() == iter)
	{
		return NULL;
	}

	return iter->second;
}

bool SpecialTerritoryWar::IsBuilding(int building_id)
{
	std::map<int, BuildingInfo>::iterator iter = m_building_map.find(building_id);
	if (m_building_map.end() == iter)
	{
		return false;
	}

	return true;
}

short SpecialTerritoryWar::GetGuajiMonsterCredit(int monster_id)
{
	std::map<int, GuajiMonsterInfo>::iterator iter = m_guaji_monster_map.find(monster_id);
	if (m_guaji_monster_map.end() != iter)
	{
		return iter->second.kill_credit_reward;
	}

	return 0;
}

int SpecialTerritoryWar::GetSpecialParam(Role *role)
{
	JoinerInfo *joiner = this->GetJoinerInfo(role->GetUID());
	if (NULL != joiner)
	{
		return joiner->side;
	}

	return -1;
}

void SpecialTerritoryWar::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
{
	if (NULL != str_buff && str_len > 0) 
	{
		int sendlen = 0;
		static Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, str_buff, str_len, notice_type))
		{
			m_scene->SendToRole((const char*)&sm, sendlen);
		}
	}
}

void SpecialTerritoryWar::NotifySideSystemMsg(char *str_buff, int str_len, int notice_type, short side)
{
	if (NULL != str_buff && str_len > 0) 
	{
		int sendlen = 0;
		static Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, str_buff, str_len, notice_type))
		{
			for(unsigned int i = 0; i < m_scene->RoleNum(); i++)
			{
				Role *temp_role = m_scene->GetRoleByIndex(i);
				if (NULL != temp_role)
				{
					JoinerInfo * temp_joiner = this->GetJoinerInfo(temp_role->GetUID());
					if (NULL != temp_joiner && temp_joiner->side == side)
					{
						EngineAdapter::Instance().NetSend(temp_role->GetNetId(),(const char*)&sm, sendlen);
					}
				}
			}
		}
	}
};

void SpecialTerritoryWar::NotifyImage(Role *role)
{
	// 广播形象
	static Protocol::SCTerritoryWarApperance cmd;
	JoinerInfo *joiner = this->GetJoinerInfo(role->GetUID());
	if (NULL != joiner)
	{
		cmd.obj_id = role->GetId();
		cmd.special_image = joiner->special_image_id;

		role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
	}
}

void SpecialTerritoryWar::OnUseLandMine(Role *role, int type, Posi place_posi)
{
	if (NULL == role) return;

	JoinerInfo *joiner_role = this->GetJoinerInfo(role->GetUID());
	if (NULL == joiner_role) return;

	if (FIER_LANDMINE > type || TYPE_2_MAX_INDEX <= type)
		return;

	if (FIER_LANDMINE == type)
	{
		if (joiner_role->fire_landmine_count <= 0)
		{
			joiner_role->fire_landmine_count = 0;
			return;
		}
	}
	else if (ICE_LANDMINE == type)
	{
		if (joiner_role->ice_landmine_count <= 0)
		{
			joiner_role->ice_landmine_count = 0;
			return;
		}
	}

	const TerritroyWarFightShopCfg *fight_shop_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetFightShopCfg(LANDMINE, type);
	if (NULL == fight_shop_cfg) return;

	Map *map = m_scene->GetMap();
	if (NULL == map) return;

	if ( map->SaveArea(place_posi.x, place_posi.y))
	{
		role->NoticeNum(errornum::EN_SAVEAREA_SET_LANDMINE);
		return;
	}

	int param_list1[2] = {0};
	int param_list2[4] = {0};

	Trigger *trigger = new Trigger();
	if (NULL == trigger) return;

	trigger->SetPos(Posi(place_posi.x, place_posi.y));
	trigger->InitAffiliation(joiner_role->side);
	trigger->InitVisible(false, true);
	GameName trigger_name;
	role->GetName(trigger_name);
	trigger->SetName(trigger_name);
	
	if(FIER_LANDMINE == type)
	{
		trigger->Init(1, fight_shop_cfg->param4, TRIGGER_COND_TYPE_ENTER_AREA, param_list1, TRIGGER_ACTION_TYPE_FIRE_LANDMINE, param_list2, 1);
		joiner_role->fire_landmine_count--;
	}
	else
	{
		trigger->Init(1, fight_shop_cfg->param4, TRIGGER_COND_TYPE_ENTER_AREA, param_list1, TRIGGER_ACTION_TYPE_ICE_LANDMINE, param_list2, 1);
		joiner_role->ice_landmine_count--;
	}
	
	m_scene->AddObj(trigger);
	this->SyncRoleInfo(role);

}

void SpecialTerritoryWar::OnActivityStandby()
{
}

void SpecialTerritoryWar::OnActivityStart()
{
	m_joiner_map.clear();
	m_building_map.clear();

	m_is_finish = false;
	m_red_total_credit = 0;
	m_blue_total_credit = 0;
	m_read_next_can_buy_tower_wudi = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_blue_next_can_buy_tower_wudi = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_center_relive_state = 0;
	m_winner_side = -1;

	const TerritroyWarOtherCfg &other_cfg =  LOGIC_CONFIG->GetTerritoryWarConfig().GetOtherCfg();
	m_add_credit_interval = other_cfg.add_credit_interval_s;
	m_next_add_credit_time = static_cast<unsigned int>(EngineAdapter::Instance().Time() + m_add_credit_interval); 
	center_relive_point_id = other_cfg.center_relive_point_id;
	red_fortress_id = other_cfg.red_fortress_id;
	blue_fortress_id = other_cfg.blue_fortress_id;
	m_building_be_attack_notice_interval = other_cfg.attack_notice_interval;
	m_luck.next_lucky_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetTerritoryWarConfig().GetOtherCfg().lucky_interval;
	m_luck.luck_user_count = 0;
	this->InitBudingMap();
	this->InitGuajiMonsterMap();
}

void SpecialTerritoryWar::OnActivityClose()
{
	if (m_is_finish) return;

	m_is_finish = true;
	this->CalcRewardOnActClose();

	//复活所有玩家，防止活动结束时玩家不能复活的问题，这里全补满血
	this->AllRoleReLive();

	this->KillAllMonster();
	this->DelayKickOutAllRole();

	// 清理数据
	std::map<int, JoinerInfo *>::iterator it = m_joiner_map.begin();
	for(; it != m_joiner_map.end(); it++)
	{
		delete it->second;
	}
	m_joiner_map.clear();

	m_building_map.clear();
	m_guaji_monster_map.clear();
}

void SpecialTerritoryWar::InitBudingMap()
{
	const std::map<int, TerritroyWarBuildingCfg>&building_info_map = LOGIC_CONFIG->GetTerritoryWarConfig().GetBuildingCfgMap();
	std::map<int, TerritroyWarBuildingCfg>::const_iterator iter= building_info_map.begin();
	for (;building_info_map.end() != iter; iter++)
	{
		BuildingInfo& building_info = m_building_map[iter->second.building_id];
		building_info.building_id = iter->second.building_id;
		building_info.posi = iter->second.building_pos;
		building_info.kill_guild_credit = iter->second.guild_credit_reward;
		building_info.kill_personal_credit = iter->second.personal_credit_reward;
		building_info.kill_assist_credit = iter->second.kill_assist_credit;
		building_info.assist_vaild_time = iter->second.assist_vaild_time;
		building_info.building_index = iter->second.building_index;
		building_info.side= iter->second.side;
		building_info.preposition_building_1 = iter->second.preposition_monster_1;
		building_info.preposition_building_2 = iter->second.preposition_monster_2;
		building_info.tower_attack_man_per = iter->second.tower_attack_man_per;
		building_info.tower_attack_fight_car_per = iter->second.tower_attack_fight_car_per;
		building_info.tower_attack_fang_car_per = iter->second.tower_attack_fang_car_per;
		building_info.tower_attack_cure_car_per = iter->second.tower_attack_cure_car_per;
		
		this->CreateOneBuilding(building_info);
	}
}

void SpecialTerritoryWar::InitGuajiMonsterMap()
{
	const std::map<int, TerritroyWarGuajiMonsterCfg>&guaji_monster_map = LOGIC_CONFIG->GetTerritoryWarConfig().GetGuaJiMonsterMap();
	std::map<int, TerritroyWarGuajiMonsterCfg>::const_iterator iter = guaji_monster_map.begin();
	for(;guaji_monster_map.end() != iter; iter++)
	{
		GuajiMonsterInfo &guaji_monster = m_guaji_monster_map[iter->second.monster_id];
		guaji_monster.monster_id = iter->second.monster_id;
		guaji_monster.kill_credit_reward = iter->second.kill_credit_reward;
	}
}

void SpecialTerritoryWar::DoPerformSkill(Role *role, const TerritroyWarSkillItemCfg *skill_cfg, ObjID target_id)
{
	if (NULL == role || NULL == skill_cfg) return;

	Obj *target_obj = m_scene->GetObj(target_id);
	if (NULL != target_obj/* && Obj::OBJ_TYPE_ROLE == target_obj->GetObjType()*/)
	{
		role->OnStartFight();
	}

	switch(skill_cfg->skill_index)
	{
	case TW_SKILL_INDEX1: 
	case TW_SKILL_INDEX2:
	case TW_SKILL_INDEX3:
	case TW_SKILL_INDEX4:
		{
			Obj *obj = m_scene->GetObj(target_id);
			if (NULL != obj)
			{
				if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
				{
					Monster * monster = (Monster *)m_scene->GetObj(target_id);
					if (NULL == monster) return;

					if (this->IsBuilding(monster->GetMonsterId()))
					{
						EffectOnce *effect = new EffectOnce(role->GetId(), skill_cfg->skill_index, -skill_cfg->fix_hurt, EffectBase::FIGHT_TYPE_NORMAL);
						effect->SetSave(false);
						monster->AddEffect(effect);
					}
					else
					{
						Attribute hurt = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI));
						EffectOnce *effect = new EffectOnce(role->GetId(), skill_cfg->skill_index, -hurt, EffectBase::FIGHT_TYPE_NORMAL);
						effect->SetSave(false);
						monster->AddEffect(effect);
					}
				}
				else if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
				{
					Role *target_role = (Role *)m_scene->GetObj(target_id);
					if (NULL == target_role) return;

					JoinerInfo *joiner = this->GetJoinerInfo(target_role->GetUID());
					if (NULL == joiner) return;

					long long hurt = 0;
					switch (joiner->special_image_id)
					{
					case INVAILD_APPEARANCE:
					{
						hurt = static_cast<long long>(target_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (skill_cfg->hurt_percent * ROLE_ATTR_PER));
					}
					break;

					case CAR_FIGHT_APEARANCE:
					{
						hurt = static_cast<long long>(target_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (skill_cfg->fix_hurt_on_fight_car * ROLE_ATTR_PER));
					}
					break;

					case CAR_FANG_APEARANCE:
					{
						hurt = static_cast<long long>(target_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (skill_cfg->fix_hurt_on_fang_car * ROLE_ATTR_PER));
					}
					break;

					case CAR_CURE_APEARANCE:
					{
						hurt = static_cast<long long>(target_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (skill_cfg->fix_hurt_on_cure_car * ROLE_ATTR_PER));
					}
					break;

					default:
						break;
					}

					EffectOnce *effect = new EffectOnce(role->GetId(), skill_cfg->skill_index, -hurt, EffectBase::FIGHT_TYPE_NORMAL);
					effect->SetSave(false);
					target_role->AddEffect(effect);
				}
			}
		}
		break;

	case TW_SKILL_INDEX5:
		{
			EffectSpecialState *effect_state = new EffectSpecialState(role->GetId(), skill_cfg->skill_index, skill_cfg->param_a, EBT_WUDI, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
			effect_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
			role->AddEffect(effect_state);	
		}
		break;

	case TW_SKILL_INDEX6:	
		{
			if (role->GetId() == target_id) return;

			Obj *obj = m_scene->GetObj(target_id);
			if (NULL == obj) return;

			if (Obj::OBJ_TYPE_ROLE != obj->GetObjType()) return;

			Role *target_role = (Role*)obj;
			JoinerInfo *target_info = this->GetJoinerInfo(target_role->GetUID());
			if (NULL == target_info) return;

			if (INVAILD_APPEARANCE == target_info->special_image_id) return;

			int count = skill_cfg->param_a / 1000;
			if (count <= 0) count =1;

			long long cure_value = static_cast<long long>(target_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (skill_cfg->param_b * 0.01));

			cure_value = static_cast<long long>(cure_value / count);
			if (cure_value <=0) cure_value = 1;

			EffectInterval *effect = new EffectInterval(role->GetId(), TW_SKILL_INDEX6, EBT_ADD_BLOOD, 1);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
			effect->SetParam(1000, count, cure_value, target_role->GetUID());
			effect->SetSave(false);
			target_role->AddEffect(effect);
		}
		break;

	default:
		break;
	}
}

void SpecialTerritoryWar::CalcRewardOnActClose()
{
	if (!m_is_finish) return;

	if (m_winner_side == -1)
	{
		m_winner_side = m_red_total_credit >= m_blue_total_credit? 1 : 0;
	}

	this->SyncGlobalInfoToAll();

	int rank_1 = (m_scene->GetSceneKey() - 1) * 2 + 1;
	int rank_2 = (m_scene->GetSceneKey() - 1) * 2 + 2;
	GuildRank *guild_rank = RankManager::Instance().GetGuildRank();
	if (NULL != guild_rank)
	{
		int rank_1_guildid = INVALID_GUILD_ID;
		int rank_2_guildid = INVALID_GUILD_ID;

		if (m_winner_side == 1)
		{
			rank_1_guildid = LOGIC_CONFIG->GetTerritoryWarConfig().GetGuildIDByRank(rank_1);
			rank_2_guildid = LOGIC_CONFIG->GetTerritoryWarConfig().GetGuildIDByRank(rank_2);
		}
		else
		{
			rank_1_guildid = LOGIC_CONFIG->GetTerritoryWarConfig().GetGuildIDByRank(rank_2);
			rank_2_guildid = LOGIC_CONFIG->GetTerritoryWarConfig().GetGuildIDByRank(rank_1);
		}

		if (rank_1_guildid != INVALID_GUILD_ID)
		{
			Guild *guild = GuildManager::Instance().GetGuild(rank_1_guildid);
			if (NULL != guild)
			{
				guild->SetTerritoryWarRank(rank_1);
			}
		}

		if (rank_2_guildid != INVALID_GUILD_ID)
		{
			Guild *guild = GuildManager::Instance().GetGuild(rank_2_guildid);
			if (NULL != guild)
			{
				guild->SetTerritoryWarRank(rank_2);
			}
		}
	}

	const TerritroyWarActivityEndCfg * activity_reward_cfg_1 = LOGIC_CONFIG->GetTerritoryWarConfig().GetActivityEndCfg(m_scene->GetSceneKey(),0);
	const TerritroyWarActivityEndCfg * activity_reward_cfg_2 = LOGIC_CONFIG->GetTerritoryWarConfig().GetActivityEndCfg(m_scene->GetSceneKey(),1); 
	if (NULL == activity_reward_cfg_1 || NULL == activity_reward_cfg_2) return;
	
	std::map<int, JoinerInfo *>::iterator iter;
	for (iter = m_joiner_map.begin(); m_joiner_map.end() != iter; ++ iter)
	{
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(iter->second->uid);
		if (NULL == user_node)
			continue;
		
		if (iter->second->side == m_winner_side)
		{
			//发送活动结束邮件奖励
			static MailContentParam contentparam; contentparam.Reset();
			if (NULL != ITEMPOOL->GetItem(activity_reward_cfg_1->reward_item_1.item_id) && activity_reward_cfg_1->reward_item_1.num > 0)
			{
				contentparam.AddItem(activity_reward_cfg_1->reward_item_1.item_id,activity_reward_cfg_1->reward_item_1.num,activity_reward_cfg_1->reward_item_1.is_bind);
			}

			if (NULL != ITEMPOOL->GetItem(activity_reward_cfg_1->reward_item_2.item_id) && activity_reward_cfg_1->reward_item_2.num > 0)
			{
				contentparam.AddItem(activity_reward_cfg_1->reward_item_2.item_id,activity_reward_cfg_1->reward_item_2.num,activity_reward_cfg_1->reward_item_2.is_bind);
			}

			if (NULL != ITEMPOOL->GetItem(activity_reward_cfg_1->reward_item_3.item_id) && activity_reward_cfg_1->reward_item_3.num > 0)
			{
				contentparam.AddItem(activity_reward_cfg_1->reward_item_3.item_id,activity_reward_cfg_1->reward_item_3.num,activity_reward_cfg_1->reward_item_3.is_bind);
			}

			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GUILDGONGXIAN, activity_reward_cfg_1->banggong);
			contentparam.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_CONVERTSHOP_SHENGWANG, activity_reward_cfg_1->shengwang);

			int length_1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_territorywar_result, rank_1);

			if (length_1 > 0)
			{
				MailRoute::MailToUser(IntToUid(iter->second->uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}

			LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_TERRITORYWAR, iter->second->uid, user_node->user_name, NULL, user_node->level, activity_reward_cfg_1->reward_item_1.item_id, "Reward", NULL, activity_reward_cfg_1->reward_item_1.num, activity_reward_cfg_1->reward_item_1.is_bind, 0, 0);
			LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_TERRITORYWAR, iter->second->uid, user_node->user_name, NULL, user_node->level, activity_reward_cfg_1->reward_item_2.item_id, "Reward", NULL, activity_reward_cfg_1->reward_item_2.num, activity_reward_cfg_1->reward_item_2.is_bind, 0, 0);
			LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_TERRITORYWAR, iter->second->uid, user_node->user_name, NULL, user_node->level, activity_reward_cfg_1->reward_item_3.item_id, "Reward", NULL, activity_reward_cfg_1->reward_item_3.num, activity_reward_cfg_1->reward_item_3.is_bind, 0, 0);

		}
		else if (iter->second->side != m_winner_side)
		{
			//发送活动结束邮件奖励
			static MailContentParam contentparam; contentparam.Reset();
			if (NULL != ITEMPOOL->GetItem(activity_reward_cfg_2->reward_item_1.item_id) && activity_reward_cfg_2->reward_item_1.num > 0)
			{
				contentparam.AddItem(activity_reward_cfg_2->reward_item_1.item_id,activity_reward_cfg_2->reward_item_1.num,activity_reward_cfg_2->reward_item_1.is_bind);
			}

			if (NULL != ITEMPOOL->GetItem(activity_reward_cfg_2->reward_item_2.item_id) && activity_reward_cfg_2->reward_item_2.num > 0)
			{
				contentparam.AddItem(activity_reward_cfg_2->reward_item_2.item_id,activity_reward_cfg_2->reward_item_2.num,activity_reward_cfg_2->reward_item_2.is_bind);
			}

			if (NULL != ITEMPOOL->GetItem(activity_reward_cfg_2->reward_item_3.item_id) && activity_reward_cfg_2->reward_item_3.num > 0)
			{
				contentparam.AddItem(activity_reward_cfg_2->reward_item_3.item_id,activity_reward_cfg_2->reward_item_3.num,activity_reward_cfg_2->reward_item_3.is_bind);
			}

			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GUILDGONGXIAN, activity_reward_cfg_2->banggong);
			contentparam.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_CONVERTSHOP_SHENGWANG, activity_reward_cfg_2->shengwang);

			int length_1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_territorywar_result, rank_2);

			if (length_1 > 0)
			{
				MailRoute::MailToUser(IntToUid(iter->second->uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}

			LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_TERRITORYWAR, iter->second->uid, user_node->user_name, NULL, user_node->level, activity_reward_cfg_1->reward_item_1.item_id, "Reward", NULL, activity_reward_cfg_1->reward_item_1.num, activity_reward_cfg_1->reward_item_1.is_bind, 0, 0);
			LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_TERRITORYWAR, iter->second->uid, user_node->user_name, NULL, user_node->level, activity_reward_cfg_1->reward_item_2.item_id, "Reward", NULL, activity_reward_cfg_1->reward_item_2.num, activity_reward_cfg_1->reward_item_2.is_bind, 0, 0);
			LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ACTIVITY_TERRITORYWAR, iter->second->uid, user_node->user_name, NULL, user_node->level, activity_reward_cfg_1->reward_item_3.item_id, "Reward", NULL, activity_reward_cfg_1->reward_item_3.num, activity_reward_cfg_1->reward_item_3.is_bind, 0, 0);
		}
	}
}

void SpecialTerritoryWar::CheckCreditReward(Role *role)
{
	if (NULL == role) return;

	JoinerInfo *joiner = this->GetJoinerInfo(role->GetUID());
	if (NULL == joiner) return;

	role->GetRoleGoal()->CheckBattleFieldCond(ACTIVITY_TYPE_TERRITORY_WAR, joiner->history_credit);

	const TerritroyWarPersonalRewardCfg * person_credit_reward = LOGIC_CONFIG->GetTerritoryWarConfig().GetPersonalCreditCfg(joiner->history_credit);
	if (NULL != person_credit_reward)
	{
		for(int i = person_credit_reward->reward_index; i > joiner->fetch_last_credit_reward_index && i > 0; i--)
		{
			if (this->IsGetCreditReward(joiner,person_credit_reward->reward_index))
			{
				continue;
			}

			const TerritroyWarPersonalRewardCfg * reward = LOGIC_CONFIG->GetTerritoryWarConfig().GetPersonalCreditCfgByIndex(i);
			if (NULL == reward)
			{
				continue;
			}

			// 打包奖励物品
			ItemConfigData reward_item_1[3] = {reward->item_1, reward->item_2, reward->item_3};
			// 奖励优先放背包，背包满了发邮件
			role->GetKnapsack()->PutListOrMail(reward_item_1,PUT_REASON_TERRITORY_WAR);

			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_fetch_target_reward,
					role->GetUID(), role->GetName(), (int)role->GetCamp(), person_credit_reward->person_credit_min);
				if (length > 0) 
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
				}
			}

			role->AddGuildGongxian(reward->banggong, "TerritoryWar");
			role->GetRoleShop()->AddChestShopShengwang(reward->shengwang, "TerritoryWar");
			this->SetCreditRewardFlag(joiner, reward->reward_index);
		}

		joiner->fetch_last_credit_reward_index = person_credit_reward->reward_index;
		this->SyncRoleInfo(role);
	}
}

void SpecialTerritoryWar::SyncGlobalInfoToAll()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); ++ i)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);
		this->SyncGlobalInfo(temp_role);
	}
}

void SpecialTerritoryWar::SyncGlobalInfo(Role *role)
{
	if(NULL == role) return;

	Protocol::SCTerritoryWarGlobeInfo cmd;
	cmd.red_guild_credit = m_red_total_credit;
	cmd.blue_guild_credit = m_blue_total_credit;
	BuildingInfo & center_relive_building = m_building_map[center_relive_point_id];
	cmd.center_relive_side = center_relive_building.side;
	cmd.reserve = 0;

	cmd.center_relive_max_hp = center_relive_building.max_hp;
	cmd.center_relive_curr_hp = center_relive_building.current_hp;

	BuildingInfo & red_fortress = m_building_map[red_fortress_id];
	cmd.red_fortress_max_hp = red_fortress.max_hp;
	cmd.red_fortress_curr_hp = red_fortress.current_hp;

	BuildingInfo & blue_fortress = m_building_map[blue_fortress_id];
	cmd.blue_fortress_max_hp = blue_fortress.max_hp;
	cmd.blue_fortress_curr_hp = blue_fortress.current_hp;

	cmd.red_building_survive_flag = red_building_survive_flag;
	cmd.blue_building_survive_flag = blue_building_survive_flag;

	cmd.read_next_can_buy_tower_wudi = m_read_next_can_buy_tower_wudi;
	cmd.blue_next_can_buy_tower_wudi = m_blue_next_can_buy_tower_wudi;

	cmd.winner_side = m_winner_side;
	cmd.reserve_1 = 0;
	cmd.reserve_2 = 0;
	
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

void SpecialTerritoryWar::SyncRoleInfo(Role *role)
{
	if (NULL == role) return;

	JoinerInfo *joiner = this->GetJoinerInfo(role->GetUID());
	if (NULL == joiner)
		return;

	static Protocol::SCTerritoryWarRoleInfo cmd;

	cmd.current_credit = joiner->current_credit;
	cmd.history_credit = joiner->history_credit;
	cmd.credit_reward_flag = joiner->credit_reward_flag;
	cmd.kill_count = joiner->kill_count;
	cmd.assist_count = joiner->assist_count;
	cmd.death_count = joiner->death_count;
	cmd.special_image_id = joiner->special_image_id;
	cmd.side = joiner->side;
	cmd.reserve_1 = 0;
	cmd.reserve_2 = 0;
	cmd.ice_landmine_count = joiner->ice_landmine_count;
	cmd.fire_landmine_count = joiner->fire_landmine_count;

	for (int i = 0; i < Protocol::TERRITORYWAR_SKILL_COUNT_MAX;i++)
	{
		cmd.skill_list[i].skill_index = joiner->skill_list[i].skill_index;
		cmd.skill_list[i].last_perform_time = joiner->skill_list[i].last_perform_time;
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

void SpecialTerritoryWar::AllAddCredit()
{
	int person_add_credit = LOGIC_CONFIG->GetTerritoryWarConfig().GetOtherCfg().add_credit;
	for (unsigned int i = 0; i < m_scene->RoleNum(); ++ i)
	{
		Role *temp_role = m_scene->GetRoleByIndex(i);
		if (NULL == temp_role)
			continue;

		this->PersonalAddCredit(temp_role, person_add_credit);
	}
}

void SpecialTerritoryWar::PersonalAddCredit(Role *role, int add_person_credti)
{
	if (NULL == role) return;

	JoinerInfo *joiner = this->GetJoinerInfo(role->GetUID());
	if (NULL == joiner) return;

	joiner->current_credit += add_person_credti;
	joiner->history_credit += add_person_credti;
	this->CheckCreditReward(role);
	this->SyncRoleInfo(role);
}

void SpecialTerritoryWar::PersonalAddCredit(int uid, int add_person_credti)
{
	JoinerInfo *joiner = this->GetJoinerInfo(uid);
	if (NULL == joiner) return;

	Role *role = m_scene->GetRoleByUID(uid);
	if (NULL == role) return;

	joiner->current_credit += add_person_credti;
	joiner->history_credit += add_person_credti;
	this->CheckCreditReward(role);
	this->SyncRoleInfo(role);
}


void SpecialTerritoryWar::AddGuildCredit(Role *role, int add_guild_credit)
{
	if (NULL == role) return;

	JoinerInfo *joiner = this->GetJoinerInfo(role->GetUID());
	if (NULL != joiner)
	{
		if (ENSIDESTATUS_RED == joiner->side)
		{
			m_red_total_credit += add_guild_credit;
		}
		else if (ENSIDESTATUS_BLUE == joiner->side)
		{
			m_blue_total_credit += add_guild_credit;
		}

		this->SyncGlobalInfoToAll();
	}
}

void SpecialTerritoryWar::BuyTowerWudi(Role *role)
{
	if (NULL == role) return;

	JoinerInfo *joiner = this->GetJoinerInfo(role->GetUID());
	if (NULL == joiner) return;

	const TerritroyWarReliveShopCfg *relive_shop_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetReliveShopCfg(TOWER_WUDI);
	if (NULL == relive_shop_cfg) return;

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if ( 0 == joiner->side)
	{
		if (now_sec < m_blue_next_can_buy_tower_wudi)
		{
			role->NoticeNum(errornum::EN_TW_WAR_TOWER_WUDI_CLOD_LIMIT);
			return;
		}
	}
	else if (1 == joiner->side)
	{
		if (now_sec < m_read_next_can_buy_tower_wudi)
		{
			role->NoticeNum(errornum::EN_TW_WAR_TOWER_WUDI_CLOD_LIMIT);
			return;
		}
	}
	else
	{
		return;
	}

	if (joiner->current_credit < relive_shop_cfg->cost_credit)
	{
		role->NoticeNum(errornum::EN_TW_WAR_CREDIT_NO_ENOUGHT);
		return;
	}
	else
	{
		joiner->current_credit -= relive_shop_cfg->cost_credit;
		this->SyncRoleInfo(role);

		if(0 == joiner->side)
		{
			m_blue_next_can_buy_tower_wudi = static_cast<unsigned int>(EngineAdapter::Instance().Time() + relive_shop_cfg->param2);
		}
		else if (1 == joiner->side)
		{
			m_read_next_can_buy_tower_wudi = static_cast<unsigned int>(EngineAdapter::Instance().Time() + relive_shop_cfg->param2);
		}

		this->SyncGlobalInfoToAll();

		std::map<int, BuildingInfo>::iterator iter = m_building_map.begin();
		for(;iter != m_building_map.end();iter++)
		{
			if (ENBUILDINGSTATUS_EXIST == iter->second.building_state && joiner->side == iter->second.side)
			{
				Obj *obj = m_scene->GetObj(iter->second.obj_id);
				if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
				{
					Monster *building = (Monster*)(obj);
					if (center_relive_point_id != building->GetMonsterId() && red_fortress_id != building->GetMonsterId() && blue_fortress_id != building->GetMonsterId())
					{
						EffectSpecialState *effect_state = new EffectSpecialState(role->GetId(), 1, relive_shop_cfg->param1, EBT_TERRITORYWAR_TOWER_WUDI, 1);
						effect_state->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
						building->AddEffect(effect_state);
					}
				}
			}
		}
	}
}

void SpecialTerritoryWar::BuyTransport(Role *role)
{
	if (NULL == role) return;

	JoinerInfo *joiner = this->GetJoinerInfo(role->GetUID());
	if (NULL == joiner) return;

	const TerritroyWarReliveShopCfg *relive_shop_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetReliveShopCfg(TRANSPORT);
	if (NULL == relive_shop_cfg) return;
	
	if (joiner->current_credit < relive_shop_cfg->cost_credit)
	{
		role->NoticeNum(errornum::EN_TW_WAR_CREDIT_NO_ENOUGHT);
		return;
	}
	else
	{
		joiner->current_credit -= relive_shop_cfg->cost_credit;
		if (joiner->current_credit < 0)
			joiner->current_credit = 0;
		
		this->SyncRoleInfo(role);
		std::map<int, BuildingInfo>::iterator iter = m_building_map.begin();
		for(;iter != m_building_map.end();iter++)
		{
			BuildingInfo building = iter->second;
			if (building.side != joiner->side)
				continue;

			if (ENBUILDINGSTATUS_NONEXIST == building.building_state)
				continue;

			if (center_relive_point_id == building.building_id || red_fortress_id == building.building_id || blue_fortress_id == building.building_id)
				continue;

			World::GetInstWorld()->GetSceneManager()->GoTo(role, m_scene->GetSceneID(), m_scene->GetSceneKey(), building.posi);
				return;
		}

		if (iter == m_building_map.end())
		{
			role->NoticeNum(errornum::EN_TW_WAR_TRANSPORT_NOT_TARGET);
			return;
		}
	}
}

void SpecialTerritoryWar::BuyTransportToCenter(Role *role)
{
	if (NULL == role) return;

	JoinerInfo *joiner = this->GetJoinerInfo(role->GetUID());
	if (NULL == joiner) return;

	if (joiner->guild_id != m_center_relive_state)
	{
		role->NoticeNum(errornum::EN_TW_WAR_TRANSPORT_CENTER_LIMIT);
		return;
	}

	const TerritroyWarBuildingCfg * building_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetBuildingCfg(center_relive_point_id);
	if (NULL != building_cfg)
	{
		World::GetInstWorld()->GetSceneManager()->GoTo(role, m_scene->GetSceneID(), m_scene->GetSceneKey(), building_cfg->building_pos);
		return;
	}
}

void SpecialTerritoryWar::BuyCar(Role *role,short goods_id)
{
	if (FIGHT_CAR_GONG > goods_id || TYPE_0_MAX_INDEX <= goods_id) return;

	if (NULL == role) return;

	JoinerInfo *joiner = this->GetJoinerInfo(role->GetUID());
	if (NULL == joiner) return;

	if (INVAILD_APPEARANCE != joiner->special_image_id)
	{
		role->NoticeNum(errornum::EN_TERRITORYWAR_BUY_CAR_LIMIT);
		return;
	}

	if (FIGHT_CAR_GONG == goods_id)
	{
		const TerritroyWarFightShopCfg *fight_shop_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetFightShopCfg(BIAN_SHEN, FIGHT_CAR_GONG);
		if (NULL != fight_shop_cfg)
		{
			if (joiner->current_credit < fight_shop_cfg->cost_credit)
			{
				role->NoticeNum(errornum::EN_TW_WAR_CREDIT_NO_ENOUGHT);
				return;
			}
				
			joiner->current_credit -= fight_shop_cfg->cost_credit;
			if (joiner->current_credit < 0)
				joiner->current_credit = 0;

			role->GetSpecialAppearance()->SetFbAppearance(SPECIAL_APPERANCE_TYPE_TERRITORYWAR, CAR_FIGHT_APEARANCE, 0, true);
			joiner->special_image_id = CAR_FIGHT_APEARANCE;
			joiner->SetSkillList(TW_SKILL_INDEX1, TW_SKILL_INDEX4);
			this->NotifyImage(role);
			this->SyncRoleInfo(role);
		}
		
		return;
	}
	else if (FIGHT_CAR_FANG == goods_id)
	{
		const TerritroyWarFightShopCfg *fight_shop_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetFightShopCfg(BIAN_SHEN, FIGHT_CAR_FANG);
		if (NULL != fight_shop_cfg)
		{
			if (joiner->current_credit < fight_shop_cfg->cost_credit)
			{
				role->NoticeNum(errornum::EN_TW_WAR_CREDIT_NO_ENOUGHT);
				return;
			}

			joiner->current_credit -= fight_shop_cfg->cost_credit;
			if (joiner->current_credit < 0)
				joiner->current_credit = 0;

			role->GetSpecialAppearance()->SetFbAppearance(SPECIAL_APPERANCE_TYPE_TERRITORYWAR, CAR_FANG_APEARANCE, 0, true);
			joiner->special_image_id = CAR_FANG_APEARANCE;
			joiner->SetSkillList(TW_SKILL_INDEX2, TW_SKILL_INDEX5);
			this->NotifyImage(role);
			this->SyncRoleInfo(role);
		}

		return;
	}
	else if (FIGHT_CAR_CURE == goods_id)
	{
		const TerritroyWarFightShopCfg *fight_shop_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetFightShopCfg(BIAN_SHEN, FIGHT_CAR_CURE);
		if (NULL != fight_shop_cfg)
		{
			if (joiner->current_credit < fight_shop_cfg->cost_credit)
			{
				role->NoticeNum(errornum::EN_TW_WAR_CREDIT_NO_ENOUGHT);
				return;
			}

			joiner->current_credit -= fight_shop_cfg->cost_credit;
			if (fight_shop_cfg < 0)
				joiner->current_credit = 0;
			
			role->GetSpecialAppearance()->SetFbAppearance(SPECIAL_APPERANCE_TYPE_TERRITORYWAR, CAR_CURE_APEARANCE, 0, true);
			joiner->special_image_id = CAR_CURE_APEARANCE;
			joiner->SetSkillList(TW_SKILL_INDEX3, TW_SKILL_INDEX6);
			this->NotifyImage(role);
			this->SyncRoleInfo(role);
		}

		return;
	}
}

void SpecialTerritoryWar::BuyMedicine(Role *role,short goods_id)
{
	if (EDGE_MEDICINE > goods_id || TYPE_1_MAX_INDEX  <= goods_id) return;

	if (NULL == role) return;

	JoinerInfo *joiner = this->GetJoinerInfo(role->GetUID());
	if (NULL == joiner || NULL == role) return;
	
	if (EDGE_MEDICINE == goods_id)
	{
		const TerritroyWarFightShopCfg *fight_shop_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetFightShopCfg(MEDICAMENT, EDGE_MEDICINE);
		if (NULL == fight_shop_cfg) return;
		
		if (joiner->current_credit < fight_shop_cfg->cost_credit)
		{
			role->NoticeNum(errornum::EN_TW_WAR_CREDIT_NO_ENOUGHT);
			return;
		}
		else
		{
			joiner->current_credit -= fight_shop_cfg->cost_credit;
			if (joiner->current_credit < 0)
				joiner->current_credit = 0;
			
			this->SyncRoleInfo(role);
			this->AddEdgeMedicineEffect(role);
			return;
		}
	}
	else if(RELIVE_MEDICINE == goods_id)
	{	
		if (role->IsAlive()) return;
		
		const TerritroyWarFightShopCfg *fight_shop_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetFightShopCfg(MEDICAMENT, RELIVE_MEDICINE);
		if (NULL == fight_shop_cfg) return;
			
		if (joiner->current_credit < fight_shop_cfg->cost_credit)
		{
			role->NoticeNum(errornum::EN_TW_WAR_CREDIT_NO_ENOUGHT);
			return;
		}
		else
		{
			joiner->current_credit -= fight_shop_cfg->cost_credit;
			if (joiner->current_credit < 0)
				joiner->current_credit = 0;
			
			this->SyncRoleInfo(role);
			role->ReAlive(fight_shop_cfg->param1, fight_shop_cfg->param2, role->GetPos(), true);
			return;
		}
	}
}

void SpecialTerritoryWar::BuyLandMine(Role *role,short goods_id)
{
	if (FIER_LANDMINE > goods_id || TYPE_2_MAX_INDEX  <= goods_id) return;

	if (NULL == role) return;

	JoinerInfo *joiner = this->GetJoinerInfo(role->GetUID());
	if (NULL == joiner || NULL == role) return;

	if (FIER_LANDMINE == goods_id)
	{
		const TerritroyWarOtherCfg &other_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetOtherCfg();
		if (joiner->fire_landmine_count >= other_cfg.fire_landmine_num_limit)
		{
			role->NoticeNum(errornum::EN_TERRITORYWAR_BUY_FIRE_LANDMINE_LIMIT);
			return;
		}

		const TerritroyWarFightShopCfg *fight_shop_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetFightShopCfg(LANDMINE, FIER_LANDMINE);
		if (NULL == fight_shop_cfg) return;

		if (joiner->current_credit < fight_shop_cfg->cost_credit)
		{
			role->NoticeNum(errornum::EN_TW_WAR_CREDIT_NO_ENOUGHT);
			return;
		}
		else
		{
			joiner->current_credit -= fight_shop_cfg->cost_credit;
			if (joiner->current_credit < 0)
				joiner->current_credit = 0;
			
			joiner->fire_landmine_count++;
			this->SyncRoleInfo(role);
			return;
		}
	}
	else if (ICE_LANDMINE == goods_id)
	{
		const TerritroyWarOtherCfg &other_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetOtherCfg();
		if (joiner->ice_landmine_count >= other_cfg.ice_landmine_num_limit)
		{
			role->NoticeNum(errornum::EN_TERRITORYWAR_BUY_ICE_LANDMINE_LIMIT);
			return;
		}
		
		const TerritroyWarFightShopCfg *fight_shop_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetFightShopCfg(LANDMINE, ICE_LANDMINE);
		if (NULL == fight_shop_cfg) return;

		if (joiner->current_credit < fight_shop_cfg->cost_credit)
		{
			role->NoticeNum(errornum::EN_TW_WAR_CREDIT_NO_ENOUGHT);
			return;
		}
		else
		{
			joiner->current_credit -= fight_shop_cfg->cost_credit;
			if (joiner->current_credit < 0)
				joiner->current_credit = 0;
			
			joiner->ice_landmine_count++;
			this->SyncRoleInfo(role);
			return;
		}
	}
}

void SpecialTerritoryWar::AddEdgeMedicineEffect(Role *role)
{
	if (NULL == role) return;
	
	const TerritroyWarFightShopCfg *fight_shop_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetFightShopCfg(MEDICAMENT, EDGE_MEDICINE);
	if (NULL == fight_shop_cfg) return;

	EffectBuff *effect_buff = new EffectBuff(role->GetId(), 1, fight_shop_cfg->param1, 1);
	effect_buff->SetMergeRule(EffectBase::MERGE_RULE_MERGE);
	int value = (int)(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * (fight_shop_cfg->param2 * ROLE_ATTR_PER));
	effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, value, fight_shop_cfg->param2);
	effect_buff->SetClientEffectType(EffectBase::ECT_TW_EDGE_MEDICINE);
	role->AddEffect(effect_buff);
}
void SpecialTerritoryWar::ClearEdgeMedicineEffect(Role *role)
{
	if (NULL == role) return;
	
	role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTRBUFF, EBT_INVALID, role->GetId(),EffectBase::PRODUCT_METHOD_SCENE_SKILL);
}

void SpecialTerritoryWar::SendLuckyInfoToUser(Role *role)
{
	if (NULL == role)
	{
		int sendlen = sizeof(m_luck) - (ZHANCHANG_LUACY_ROLE_MAX - m_luck.luck_user_count) * sizeof(m_luck.luck_user_namelist[0]);
		m_scene->SendToRole((const char*)&m_luck, sendlen);
	}
	else
	{
		int sendlen = sizeof(m_luck) - (ZHANCHANG_LUACY_ROLE_MAX - m_luck.luck_user_count) * sizeof(m_luck.luck_user_namelist[0]);
		if (sendlen > 0 && sendlen <= static_cast<int>(sizeof(m_luck)))
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&m_luck, sendlen);
		}
	}
}

void SpecialTerritoryWar::GaveLuckyItemToUser()
{
	int luck_people_num = LOGIC_CONFIG->GetTerritoryWarConfig().GetOtherCfg().luck_people_num;

	int lucky_role_count = 0;
	int lucky_role_idx_list[ZHANCHANG_LUACY_ROLE_MAX] = {0};

	{
		std::map<int, int> idx_map;

		int max_rolenum = m_scene->RoleNum();
		for (int i = 0; i < max_rolenum; i++)  // 通过map来打乱idx，用于随机
		{
			idx_map.insert(std::make_pair(RandomNum(1024), i));
		}

		int loop_count = 0;
		while (!idx_map.empty() && lucky_role_count < luck_people_num && ++ loop_count < 128)
		{
			int rand_index = RandomNum(static_cast<int>(idx_map.size()));
			std::map<int, int>::iterator it = idx_map.begin();
			std::advance(it, rand_index);

			lucky_role_idx_list[lucky_role_count ++] = it->second;
			idx_map.erase(it);
		}
	}

	m_luck.luck_user_count = 0;

	char str_format[256] = {0};
	int max_length = sizeof(str_format) / sizeof(str_format[0]);
	int str_length = 0;

	const ItemConfigData &lucky_item = LOGIC_CONFIG->GetTerritoryWarConfig().GetOtherCfg().lucky_item;

	for (int i = 0; i < lucky_role_count && i < ZHANCHANG_LUACY_ROLE_MAX; i++)
	{
		int role_idx = lucky_role_idx_list[i];
		if (role_idx < (int)m_scene->RoleNum())
		{
			Role *lucky_role = m_scene->GetRoleByIndex(role_idx);
			if (NULL != lucky_role)
			{
				const ItemBase *itembase = ITEMPOOL->GetItem(lucky_item.item_id);
				if (NULL != itembase)
				{
					lucky_role->GetKnapsack()->PutOrMail(lucky_item, PUT_REASON_LTZ_LUCK_REWARD);
				}

				if (m_luck.luck_user_count < ZHANCHANG_LUACY_ROLE_MAX)
				{
					lucky_role->GetName(m_luck.luck_user_namelist[m_luck.luck_user_count]);
					++ m_luck.luck_user_count;
				}

				if (i < 5)	// 最多打包5个名字 不用ZHANCHANG_LUACY_ROLE_MAX 防止改动会导致字符串不够
				{
					int temp_length = SNPRINTF(str_format + str_length, max_length - str_length, "{r;%d;%s;%d} ", 
						UidToInt(lucky_role->GetUserId()), lucky_role->GetName(), lucky_role->GetCamp());
					str_length += temp_length;
				}
			}
		}
	}

	this->SendLuckyInfoToUser();

	{
		int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wuxingguaji_lucky_notice, 
			str_format);
		if (str_len > 0) 
		{
			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
		}
	}
}

void SpecialTerritoryWar::BroadcastZhanChang(time_t now_second)
{
	unsigned int now_sec = static_cast<unsigned int>(now_second);

	if (now_sec > m_last_first_guild_broadcast_time + FIRST_GUILD_INTERVAL)
	{
		m_last_first_guild_broadcast_time = now_sec;

		char first_side = m_winner_side;
		if (first_side == -1)
		{
			first_side = m_red_total_credit >= m_blue_total_credit? 1 : 0;
		}

		int rank_1 = (m_scene->GetSceneKey() - 1) * 2 + 1;
		int rank_2 = (m_scene->GetSceneKey() - 1) * 2 + 2;
		GuildRank *guild_rank = RankManager::Instance().GetGuildRank();
		if (NULL != guild_rank)
		{
			int rank_1_guildid = INVALID_GUILD_ID;

			if (first_side == 1)
			{
				rank_1_guildid = LOGIC_CONFIG->GetTerritoryWarConfig().GetGuildIDByRank(rank_1);
			}
			else
			{
				rank_1_guildid = LOGIC_CONFIG->GetTerritoryWarConfig().GetGuildIDByRank(rank_2);
			}

			Guild *guild = GuildManager::Instance().GetGuild(rank_1_guildid);
			if (NULL != guild)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_first_guild, guild->GetGuildName());
				if (length > 0) 
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
				}
			}
		}
	}

	if (now_sec > m_last_best_killer_broadcast_time + BEST_KILLER_INTERVAL)
	{
		m_last_best_killer_broadcast_time = now_sec;

		int killer_id = this->GetBestKiller();
		if (killer_id > 0)
		{
			JoinerInfo * joiner = m_joiner_map[killer_id];
			if (NULL != joiner && joiner->kill_count > 1)
			{
				const UserCacheNode *cache_user = UserCacheManager::Instance().GetUserNode(killer_id);
				if (NULL != cache_user)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guildbattle_best_killer,
						cache_user->uid, cache_user->user_name, (int)cache_user->camp, joiner->kill_count);
					if (length > 0) 
					{
						this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
					}
				}
			}
		}
	}
}

int SpecialTerritoryWar::GetBestKiller()
{
	int best_killer_id = -1;
	int best_killer_num = -1;
	for (std::map<int, JoinerInfo *>::iterator it = m_joiner_map.begin(); it != m_joiner_map.end(); it++)
	{
		if (NULL != it->second)
		{
			if (it->second->kill_count > best_killer_num)
			{
				best_killer_id = it->second->uid;
				best_killer_num = it->second->kill_count;
			}
		}
	}

	return best_killer_id;
}

void SpecialTerritoryWar::CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist)
{
	if (NULL != kill_role && NULL != dead_role)
	{
		JoinerInfo *killer = this->GetJoinerInfo(kill_role->GetUID());
		JoinerInfo *dead = this->GetJoinerInfo(dead_role->GetUID());
		if (NULL != killer && NULL != dead)
		{
			if (!is_assist && dead->lianzhan >= 3)
			{
				int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_stoplianzhan_notice,
					UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), 
					UidToInt(dead_role->GetUserId()), dead_role->GetName(), (int)dead_role->GetCamp(), dead->lianzhan);
				if (str_len > 0) 
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
				}
			}

			if (killer->lianzhan > 0)
			{
				int str_len = 0;
				int notice_type = SYS_MSG_ACTIVE_NOTICE;

				if (0 == killer->lianzhan % 3)
				{
					notice_type = SYS_MSG_ACTIVE_NOTICE;
					str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pvpzhanchang_lianzhan_notice,
						UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), 
						killer->lianzhan, killer->lianzhan);
				}

				if (str_len > 0) this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, notice_type);
			}
		}
	}
}