#include "worldbossmanager.hpp"
#include <algorithm>
#include "obj/character/role.h"
#include "protocal/msgother.h"
#include "engineadapter.h"
#include "internalcomm.h"

#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/string/globalstr.h"
#include "world.h"
#include "scene/scene.h"
#include "other/worldboss/worldbossshadow.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "global/worldboss/worldbossbaseconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/userprotocal/msgsystem.h"

#include "scene/sceneregister/sceneregister.hpp"
#include "servercommon/errornum.h"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"
#include "protocal/msgother.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "gameworld/gameworld/other/route/mailroute.hpp"

#include "global/usercache/usercache.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effecthspecialshield.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "other/rolegoal/rolegoalconfig.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/rolegoal/rolegoal.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "global/bossfamily//bossfamilyconfig.hpp"
#include "servercommon/activedegreedef.hpp"
#include "other/activedegree/activedegree.hpp"

WorldBossManager::WorldBossManager() : m_next_refresh_worldboss_time(0), m_next_refresh_hurtinfo_time(0), m_refresh_hurtinfo_interval(0)
{
}

WorldBossManager::~WorldBossManager()
{

}

WorldBossManager &WorldBossManager::Instance()
{
	static WorldBossManager wbm;
	return wbm;
}

void WorldBossManager::OnServerStart()
{
	// 启动时更具配置，给boss列表赋值
	std::map<int, WorldBossInfoCfg> &worldboss_info_map = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossInfoCfgMap();
	std::map<int, WorldBossInfoCfg>::iterator iter = worldboss_info_map.begin();
	for (;worldboss_info_map.end() != iter; iter++)
	{
		WorldBossInfoCfg &worldboss_info = iter->second;
		WorldBossInfo &boss_info = m_worldboss_info_map[worldboss_info.bossID];
		boss_info.boss_id = worldboss_info.bossID;
		boss_info.scence_id = worldboss_info.scene_id;
		boss_info.boss_status = ENBOSSSTATUS_NONEXIST;
		boss_info.shield_continue_time = worldboss_info.shield_continue_time;
		boss_info.shield_effective_times = worldboss_info.shield_effective_times;
		boss_info.shield_fix_hurt = worldboss_info.shield_fix_hurt;
		boss_info.is_activity_first_hudun = 0;
		boss_info.is_activity_second_hudun = 0;
		boss_info.first_trigger_hudun_per = worldboss_info.first_trigger_hudun;
		boss_info.second_trigger_hudun_per = worldboss_info.second_trigger_hudun;
		boss_info.trigger_hudun_reward = boss_info.shield_effective_times;
		boss_info.roll_max_num = worldboss_info.roll_max_num;
	}

	LOGIC_CONFIG->GetWorldBossConfig().GetNextRefreshTimeOfBoss(&m_next_refresh_worldboss_time);
	const WorldBossOtherCfg& other_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossOtherCfg();
	m_refresh_hurtinfo_interval = other_cfg.refresh_hurt_rank;
	m_roll_effective_time_interval = other_cfg.roll_effective_time;
	m_next_refresh_hurtinfo_time = static_cast<unsigned int>(EngineAdapter::Instance().Time() + m_refresh_hurtinfo_interval);
}

void WorldBossManager::OnStopServer()
{

}

void WorldBossManager::GMRefreshNow()
{
	this->RefreshAllWorldBoss();

	Protocol::SCWorldBossBorn wbb;
	World::GetInstWorld()->SendToAllGateway((const char*)&wbb, sizeof(wbb));
}

void WorldBossManager::Update(unsigned long interval, time_t now_second)
{
	if ((unsigned int)now_second >= m_next_refresh_hurtinfo_time)
	{
		std::map<int, WorldBossInfo>::iterator it = m_worldboss_info_map.begin();
		for(; m_worldboss_info_map.end() != it; it++)
		{
			if ( 1 == it->second.is_change)
			{
				it->second.is_change = 0;
				this->CalcHurtRank(it->second.boss_id);
				this->SendHurtInfoToAll(it->second.boss_id);
			}

			if (0 == it->second.is_give_hudun_reward_1 && ((unsigned int)now_second > it->second.roll_effective_time_1) && 0 != it->second.roll_effective_time_1)
			{
				if (it->second.get_hudun_reward_role_uid_1 > 0 && it->second.boss_id > 0 && it->second.current_roll_max_point_1 > 0)
				{
					this->GiveHundunReward(it->second.get_hudun_reward_role_uid_1, it->second.boss_id, it->second.current_roll_max_point_1, 1);
					it->second.get_hudun_reward_role_uid_1 = 0;
					it->second.is_give_hudun_reward_1 = 1;
					it->second.current_roll_max_point_1 = 0;
				}
			}
			else if (0 == it->second.is_give_hudun_reward_2 && ((unsigned int)now_second > it->second.roll_effective_time_2) && 0 != it->second.roll_effective_time_2)
			{
				if (it->second.get_hudun_reward_role_uid_2 > 0 && it->second.boss_id > 0 && it->second.current_roll_max_point_2 > 0)
				{
					this->GiveHundunReward(it->second.get_hudun_reward_role_uid_2, it->second.boss_id, it->second.current_roll_max_point_2, 2);
					it->second.get_hudun_reward_role_uid_2 = 0;
					it->second.is_give_hudun_reward_2 = 1;
					it->second.current_roll_max_point_2 = 0;
				}
			}
		}
		m_next_refresh_hurtinfo_time = static_cast<unsigned int>(EngineAdapter::Instance().Time() + m_refresh_hurtinfo_interval);
	}

	if ((unsigned int)now_second >= m_next_refresh_worldboss_time)
	{
		LOGIC_CONFIG->GetWorldBossConfig().GetNextRefreshTimeOfBoss(&m_next_refresh_worldboss_time);
		Protocol::SCWorldBossInfoList scweri;
		scweri.next_refresh_time = m_next_refresh_worldboss_time;
		scweri.count = 0;

		std::map<int, WorldBossInfo>::iterator iter = m_worldboss_info_map.begin();
		for(;m_worldboss_info_map.end() != iter && scweri.count < Protocol::SCWorldBossInfoList::MAX_BOSS_COUNT; iter++)
		{
			WorldBossInfo &boss_info = iter->second;
			if (ENBOSSSTATUS_NONEXIST == boss_info.boss_status)
			{
				this->RefreshOneWorldBoss(boss_info);

				Protocol::SCWorldBossBorn wbb;
				World::GetInstWorld()->SendToAllGateway((const char*)&wbb, sizeof(wbb));
			}

			{
				boss_info.hurt_map.clear();
				boss_info.person_rank_vec.clear();
				boss_info.guild_hurt_map.clear();
				boss_info.guild_rank_vec.clear();
				boss_info.is_change = 1;
				boss_info.is_activity_first_hudun = 0;
				boss_info.is_activity_second_hudun = 0;
				boss_info.trigger_hudun_reward = boss_info.shield_effective_times;
				this->CalcHurtRank(boss_info.boss_id);
				this->SendHurtInfoToAll(boss_info.boss_id);
			}

			scweri.boss_list[scweri.count].boss_id = boss_info.boss_id;
			scweri.boss_list[scweri.count].status = boss_info.boss_status;
			for (int i = 0; i < WORLD_KILLER_LIST_MAX_COUNT; i++)
			{
				scweri.boss_list[scweri.count].killer_info_list[i].killer_uid = boss_info.killer_info_list[i].killer_uid;
				scweri.boss_list[scweri.count].killer_info_list[i].killier_time = boss_info.killer_info_list[i].killier_time;
				F_STRNCPY(scweri.boss_list[scweri.count].killer_info_list[i].killer_name, boss_info.killer_info_list[i].killer_name, sizeof(GameName));
			}

			++  scweri.count;
		}

		World::GetInstWorld()->SendToAllGateway((const char*)&scweri, sizeof(scweri));
		
		int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_worldboss_refresh);
		if (len > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL);
		}

	}
}

void WorldBossManager::OnWeekChange()
{
 	this->CalcWeekRank();
	// 策划需求屏蔽公会奖励
// 	this->GiveWeekRankReward();
}

void WorldBossManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	std::map<int, WorldBossInfo>::iterator iter = m_worldboss_info_map.begin();
	for (;m_worldboss_info_map.end() != iter; iter++)
	{
		WorldBossInfo &boss_info = iter->second;
		for(int i = 0; i < WORLD_KILLER_LIST_MAX_COUNT;i++)
		{
			boss_info.killer_info_list[i].killer_uid = 0;
			boss_info.killer_info_list[i].killier_time = 0;
			memset(boss_info.killer_info_list[i].killer_name, 0 , sizeof(boss_info.killer_info_list[i].killer_name));
		}
	}
}

void WorldBossManager::OnAttack(Role *attacker, Monster *monster, long long &delta_hp)
{

}

void WorldBossManager::OnBeenAttack(Role *attacker, Monster *monster, UInt16 skill_id, long long delta_hp, bool from_skill)
{
	/* 2019-03-21 修改为达到临界值后再触发护盾。以免EffectBase::EffectHP里血量修正执行顺序问题，造成该次新增的护盾影响了本次的伤害 */

	if (NULL == attacker || NULL == monster || delta_hp >= 0) return;

	WorldBossInfo &boss_info = m_worldboss_info_map[monster->GetMonsterId()];
	boss_info.is_change = 1;
	
	if (0 == boss_info.is_activity_first_hudun && 0 == boss_info.is_activity_second_hudun)
	{
		Attribute boss_current_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		//if((boss_current_hp + delta_hp) <  boss_info.first_activity_hudun_hp)
		if (boss_current_hp <  boss_info.first_activity_hudun_hp)
		{
			//delta_hp = boss_info.first_activity_hudun_hp - boss_current_hp;
			EffectSpecialShield *effect_special_shield = new EffectSpecialShield(monster->GetId(), 1, boss_info.shield_continue_time, boss_info.shield_effective_times, 1);
			effect_special_shield->SetFixHurt(boss_info.shield_fix_hurt);
			monster->AddEffect(effect_special_shield);
			boss_info.is_activity_first_hudun = 1;
			boss_info.is_protect_by_hudun_1 = 1;
			boss_info.is_protect_by_hudun_2 = 0;
		}
	}
	else if (1 == boss_info.is_activity_first_hudun && 0 == boss_info.is_activity_second_hudun)
	{
		Attribute boss_current_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		//if((boss_current_hp + delta_hp) <  boss_info.second_activity_hudun_hp)
		if (boss_current_hp <  boss_info.second_activity_hudun_hp)
		{
			//delta_hp = boss_info.second_activity_hudun_hp - boss_current_hp;
			EffectSpecialShield *effect_special_shield = new EffectSpecialShield(monster->GetId(), 1, boss_info.shield_continue_time, boss_info.shield_effective_times, 1);
			effect_special_shield->SetFixHurt(boss_info.shield_fix_hurt);
			monster->AddEffect(effect_special_shield);
			boss_info.is_activity_second_hudun = 1;
			boss_info.is_protect_by_hudun_2 = 1;
			boss_info.is_protect_by_hudun_1 = 0;
		}
	}

	if (1 == boss_info.is_protect_by_hudun_1)
	{
		EffectSpecialShield *effect_special_shield = monster->GetEffectList()->GetEffectSpecialShield();
		if (NULL != effect_special_shield)
		{
			boss_info.trigger_hudun_reward = effect_special_shield->GetLeftEffectiveTimes();
			if (1 >= boss_info.trigger_hudun_reward && 1 == boss_info.is_give_hudun_reward_1)
			{
				this->SendHudunDestroyInfo(boss_info, 1);
				boss_info.roll_effective_time_1 = static_cast<unsigned int>(EngineAdapter::Instance().Time() + m_roll_effective_time_interval);
				boss_info.is_give_hudun_reward_1 = 0;
				boss_info.current_roll_max_point_1 = 0;
				boss_info.is_protect_by_hudun_1 = 0;
			}
		}
	}

	if (1 == boss_info.is_protect_by_hudun_2)
	{
		EffectSpecialShield *effect_special_shield = monster->GetEffectList()->GetEffectSpecialShield();
		if (NULL != effect_special_shield)
		{
			boss_info.trigger_hudun_reward = effect_special_shield->GetLeftEffectiveTimes();
			if (1 >= boss_info.trigger_hudun_reward && 1 ==boss_info.is_give_hudun_reward_2)
			{
				this->SendHudunDestroyInfo(boss_info, 2);
				boss_info.roll_effective_time_2 = static_cast<unsigned int>(EngineAdapter::Instance().Time() + m_roll_effective_time_interval);
				boss_info.is_give_hudun_reward_2 = 0;
				boss_info.current_roll_max_point_2 = 0;
				boss_info.is_protect_by_hudun_2 = 0;
			}
		}
	}

	PersonHurtInfo &hurt_info = boss_info.hurt_map[attacker->GetUID()];
	hurt_info.uid = attacker->GetUID();

	hurt_info.hurt += -delta_hp;
	
	hurt_info.guild_id = attacker->GetGuildID();

	if (INVALID_GUILD_ID != hurt_info.guild_id)
	{
		GuildHurtInfo &guild_hurt_info = boss_info.guild_hurt_map[attacker->GetGuildID()];

		guild_hurt_info.guild_id = hurt_info.guild_id;

		guild_hurt_info.guild_hurt += -delta_hp;
	}

	if (delta_hp < 0)
	{
		attacker->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_CAUSE_HARM_ON_BOSS, -delta_hp, CON_WORLD_BOSS);
		attacker->GetRoleGoal()->CheckCond(COND_TYPE_CAUSE_HARM_ON_BOSS, -delta_hp, CON_WORLD_BOSS);
	}
}

void WorldBossManager::MonsterDie(gglobalprotocal::ReportWorldBossToGlobal *report_info)
{
	if (NULL == report_info) return;

	if (gglobalprotocal::ReportWorldBossToGlobal::ENEVENTTYPE_DIED != report_info->event_type)
		return;

	WorldBossInfo &boss_info = m_worldboss_info_map[report_info->boss_id];
	PersonHurtInfo &hurt_info = boss_info.hurt_map[report_info->killer_uid];

	unsigned int min_kill_time = boss_info.killer_info_list[0].killier_time;
	int killer_index = -1;
	for (int i = 0; i < WORLD_KILLER_LIST_MAX_COUNT; i++)
	{
		if (0 == boss_info.killer_info_list[i].killer_uid)
		{
			boss_info.killer_info_list[i].killer_uid = report_info->killer_uid;
			boss_info.killer_info_list[i].killier_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			F_STRNCPY(boss_info.killer_info_list[i].killer_name, report_info->killer_name,sizeof(GameName));
			killer_index = -1;
			break;
		}
		else
		{
			if (boss_info.killer_info_list[i].killier_time < min_kill_time)
			{
				min_kill_time = boss_info.killer_info_list[i].killier_time;
				killer_index = i;
			}
		}
	}

	if (0 <= killer_index && killer_index < WORLD_KILLER_LIST_MAX_COUNT)
	{
		boss_info.killer_info_list[killer_index].killer_uid = report_info->killer_uid;
		boss_info.killer_info_list[killer_index].killier_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		F_STRNCPY(boss_info.killer_info_list[killer_index].killer_name, report_info->killer_name,sizeof(GameName));
	}


	this->CalcHurtRank(report_info->boss_id);
	this->SendHurtInfoToAll(report_info->boss_id);
	// 策划需求屏蔽个人和公会排行奖励
//	this->GivePersonHurtReward(report_info->boss_id);
// 	this->GiveGuildHurtReward(report_info->boss_id);

	Role *killer_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(report_info->killer_uid));
	if (NULL != killer_role)
	{
		//this->GiveRandomRedPaper(killer_role, boss_info.boss_id);
		this->GiveKillerGuildReward(killer_role, boss_info.boss_id);
	}

	Guild *guild = GuildManager::Instance().GetGuild(hurt_info.guild_id);
	if (NULL != guild)
	{
		guild->AddGuildWorldBossCurrentKillNum();
		guild->ChangeWorldBossCurrentKillBossTime();
	}
	
	boss_info.boss_status = ENBOSSSTATUS_NONEXIST;
	this->SendBossInfoToAllGateway(boss_info, report_info->killer_uid);

	{
		Role *killer_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(report_info->killer_uid));
		if (NULL != killer_role)
		{
			killer_role->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_KILL_BOSS_TYPE, 1, CON_WORLD_BOSS);
			killer_role->GetRoleGoal()->CheckCond(COND_TYPE_KILL_BOSS_TYPE, 1, CON_WORLD_BOSS);
			killer_role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_KILL_WORLD_BOSS, 1);
			killer_role->GetRoleActivity()->AddOpenServerInvestFinishParam(RandActivityOpenServerInvestOtherCfg::INVEST_TYPE_BOSS, 0);
			this->CheckAddActiveDegree(report_info->boss_id);
		}
	}
}

void WorldBossManager::RollPoint(Role *role, int boss_id, int hudun_index)
{
	if (NULL == role || boss_id < 0) return;
	
	WorldBossMapIt boss_info = m_worldboss_info_map.find(boss_id);
	if (m_worldboss_info_map.end() == boss_info) return;

	if (boss_info->second.roll_max_num <= 0)
		return;

	int roll_point = RandomNum(boss_info->second.roll_max_num);

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (1 == hudun_index)
	{
		if (1 == boss_info->second.is_give_hudun_reward_1 || now_sec > boss_info->second.roll_effective_time_1)
		{
			role->NoticeNum(errornum::EN_WORLD_BOSS_REWARD_IS_GIVE);
			return;
		}

		if (roll_point == (boss_info->second.roll_max_num - 1))
		{
			this->SendTopRollInfo(boss_id, hudun_index, roll_point, role->GetName());
			this->GiveHundunReward(role->GetUID(), boss_id, roll_point, hudun_index);
			boss_info->second.get_hudun_reward_role_uid_1 = 0;
			boss_info->second.is_give_hudun_reward_1 = 1;
		}
		else
		{
			if (roll_point == 0)
				roll_point++;

			if (roll_point > boss_info->second.current_roll_max_point_1)
			{
				this->SendTopRollInfo(boss_id, hudun_index, roll_point, role->GetName());
				boss_info->second.current_roll_max_point_1 = roll_point;
				boss_info->second.get_hudun_reward_role_uid_1 = role->GetUID();
			}
		}
	}

	else if (2 == hudun_index)
	{
		if (1 == boss_info->second.is_give_hudun_reward_2 || now_sec > boss_info->second.roll_effective_time_2)
		{
			role->NoticeNum(errornum::EN_WORLD_BOSS_REWARD_IS_GIVE);
			return;
		}

		if (roll_point == (boss_info->second.roll_max_num - 1))
		{
			this->SendTopRollInfo(boss_id, hudun_index, roll_point, role->GetName());
			this->GiveHundunReward(role->GetUID(), boss_id, roll_point, hudun_index);
			boss_info->second.get_hudun_reward_role_uid_2 = 0;
			boss_info->second.is_give_hudun_reward_2 = 1;
		}
		else
		{
			if (roll_point == 0)
				roll_point++;

			if (roll_point > boss_info->second.current_roll_max_point_2)
			{
				this->SendTopRollInfo(boss_id, hudun_index, roll_point, role->GetName());
				boss_info->second.current_roll_max_point_2 = roll_point;
				boss_info->second.get_hudun_reward_role_uid_2 = role->GetUID();
			}
		}
	}
	else
	{
		return;
	}

	Protocol::SCWorldBossRollInfo cmd;
	cmd.roll_point = roll_point;
	cmd.hudun_index = hudun_index;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

void WorldBossManager::GivePersonHurtReward(int boss_id)
{
	if (boss_id <= 0)
	{
		return;
	}

	WorldBossInfo &boss_info = m_worldboss_info_map[boss_id];
	std::vector<PersonHurtRankItem> &hur_vec = boss_info.person_rank_vec;
	if (hur_vec.empty())
		return;

	int person_hurt_size = static_cast<int>(hur_vec.size());
	const WorldBossOtherCfg & other_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossOtherCfg();

	for(int i = 0; i < person_hurt_size && i < other_cfg.person_reward_rank_max; i++)
	{
		const WorldBossPersonHurtRankRewardCfg* person_hurt_reward_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossPersonHurtReward(boss_id, i + 1);
		if (NULL == person_hurt_reward_cfg)
			continue;

		static MailContentParam contentparam; contentparam.Reset();
		contentparam.AddItem(person_hurt_reward_cfg->gift_item.item_id, person_hurt_reward_cfg->gift_item.num, person_hurt_reward_cfg->gift_item.is_bind);
		contentparam.AddItem(person_hurt_reward_cfg->stuff_item.item_id, person_hurt_reward_cfg->stuff_item.num, person_hurt_reward_cfg->stuff_item.is_bind);

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_worldboss_person_hurt_reward);
		if (length > 0)
		{
			MailRoute::MailToUser(IntToUid(hur_vec[i].uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}
}

void WorldBossManager::CheckAddActiveDegree(int boss_id)
{
	if (m_worldboss_info_map.find(boss_id) == m_worldboss_info_map.end())
	{
		return;
	}

	const WorldBossInfoCfg * boss_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossByBossId(boss_id);
	if (NULL == boss_cfg || boss_cfg->scene_id <= 0) return;

	Scene * boss_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(boss_cfg->scene_id, 0);
	if (NULL == boss_scene) return;

	for (int i = 0; i < (int)boss_scene->RoleNum(); ++i)
	{
		Role *role = boss_scene->GetRoleByIndex(i);
		if (role != nullptr)
		{
			role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_WORLD_BOSS);
		}
	}
}

void WorldBossManager::GiveGuildHurtReward(int boss_id)
{
	if (boss_id <= 0)
	{
		return;
	}

	WorldBossInfo &boss_info = m_worldboss_info_map[boss_id];
	std::vector<GuildHurtRankItem> &hur_vec = boss_info.guild_rank_vec;

	if (hur_vec.empty())
		return;

	int guild_hurt_size = static_cast<int>(hur_vec.size());
	const WorldBossOtherCfg & other_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossOtherCfg();

	for(int i = 0; i < guild_hurt_size && i < other_cfg.guild_reward_rank_max; i++)
	{
		const WorldBossGuildHurtRankRewardCfg* guild_hurt_reward_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossGuildHurtReward(boss_id, i + 1);
		if (NULL == guild_hurt_reward_cfg)
			continue;

		static MailContentParam contentparam; contentparam.Reset();
		contentparam.AddItem(guild_hurt_reward_cfg->gift_item.item_id, guild_hurt_reward_cfg->gift_item.num, guild_hurt_reward_cfg->gift_item.is_bind);

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_worldboss_guild_hurt_reward);
		if (length > 0)
		{
			Guild *guild = GuildManager::Instance().GetGuild(hur_vec[i].guild_id);
			if (NULL == guild) continue;

			static int member_uid_list[MAX_MEMBER_COUNT]; 
			memset(member_uid_list, 0, sizeof(member_uid_list));

			int member_count = guild->GetGuildMemberManager().GetMemberList(member_uid_list);
			for (int j = 0; j < member_count && j < MAX_MEMBER_COUNT; ++ j)
			{
				MailRoute::MailToUser(IntToUid(member_uid_list[j]), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
	}
}

void WorldBossManager::GiveKillerGuildReward(Role *role, int boss_id)
{
	if (nullptr == role || role->GetScene() == nullptr)
	{
		return;
	}

	if (boss_id <= 0)
	{
		return;
	}

	const WorldBossInfoCfg* world_boss_info_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossInfo(boss_id);
	if (NULL == world_boss_info_cfg) return;

	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (guild == nullptr) return;

	static int member_uid_list[MAX_MEMBER_COUNT]; memset(member_uid_list, 0, sizeof(member_uid_list));
	int member_count = guild->GetGuildMemberManager().GetMemberList(member_uid_list);

	for (int i = 0; i < member_count; ++i)
	{
		Role * guild_member = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
		if (guild_member == nullptr || guild_member->GetScene() == nullptr || 
			guild_member->GetScene()->GetSceneID() != role->GetScene()->GetSceneID())
		{
			continue;
		}

		static MailContentParam contentparam; contentparam.Reset();
		contentparam.AddItem(world_boss_info_cfg->stuff_item.item_id, world_boss_info_cfg->stuff_item.num, world_boss_info_cfg->stuff_item.is_bind);

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_worldboss_killer_guild_reward);
		if (length > 0)
		{
			MailRoute::MailToUser(IntToUid(member_uid_list[i]), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}
}

void WorldBossManager::GiveWeekRankReward()
{
	if (!m_guild_week_rank_vec.empty())
	{
		int week_rank_size = static_cast<int>(m_guild_week_rank_vec.size());
		const WorldBossOtherCfg & other_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossOtherCfg();

		for(int i = 0; i < week_rank_size && i < other_cfg.week_reward_rank_max; i++)
		{
			const WorldBossWeekRankRewardCfg* week_rank_reward_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossWeekRankReward(i + 1);
			if (NULL == week_rank_reward_cfg)
				continue;

			static MailContentParam contentparam; contentparam.Reset();
			contentparam.AddItem(week_rank_reward_cfg->stuff_item.item_id, week_rank_reward_cfg->stuff_item.num, week_rank_reward_cfg->stuff_item.is_bind);
			contentparam.AddItem(week_rank_reward_cfg->gift_item.item_id, week_rank_reward_cfg->gift_item.num, week_rank_reward_cfg->gift_item.is_bind);

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_worldboss_week_rank_reward);
			if (length > 0)
			{
				Guild *guild = GuildManager::Instance().GetGuild(m_guild_week_rank_vec[i].guild_id);
				if (NULL == guild) continue;

				static int member_uid_list[MAX_MEMBER_COUNT]; 
				memset(member_uid_list, 0, sizeof(member_uid_list));

				int member_count = guild->GetGuildMemberManager().GetMemberList(member_uid_list);
				for (int j = 0; j < member_count && j < MAX_MEMBER_COUNT; ++ j)
				{
					MailRoute::MailToUser(IntToUid(member_uid_list[j]), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}
	}
}

void WorldBossManager::GiveRandomRedPaper(Role *role,int boss_id)
{
	if (NULL == role) return;

	const WorldBossInfoCfg * boss_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossByBossId(boss_id);
	if (NULL == boss_cfg || boss_cfg->scene_id <= 0)
		return;

	Scene * boss_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(boss_cfg->scene_id, 0);
	if (NULL == boss_scene)
		return;

	int joiner_num = 0;
	if (INVALID_GUILD_ID != role->GetGuildID())
	{
		for (unsigned int i =0; i < boss_scene->RoleNum(); i++ )
		{
			Role *temp_role = boss_scene->GetRoleByIndex(i);
			if (NULL == temp_role)
				continue;

			if (role->GetGuildID() == temp_role->GetGuildID())
			{
				joiner_num++;
			}
		}
	}
	else
	{
		joiner_num = 1;
	}
	
	int repaper_price = joiner_num * boss_cfg->per_bind_gold;

	if (repaper_price > 0)
	{
		RedPaperManager::Instance().OnCreateSceneRedPaper(role, RED_PAPER_TYPE_RAND, repaper_price, joiner_num, true, 1, boss_id);
	}
}

void WorldBossManager::GiveHundunReward(int uid, int boss_id, int roll_num, int hudun_index)
{
	if (0 >= uid || boss_id <= 0) return;

	const WorldBossInfoCfg * boss_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossInfo(boss_id);
	if (NULL == boss_cfg)
		return;

	static MailContentParam contentparam; contentparam.Reset();
	contentparam.AddItem(boss_cfg->gift_ierm.item_id, boss_cfg->gift_ierm.num, boss_cfg->gift_ierm.is_bind);

	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_worldboss_hudun_reward);
	if (length > 0)
	{
		MailRoute::MailToUser(IntToUid(uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
	}

	Scene * boss_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(boss_cfg->scene_id, 0);
	if (NULL == boss_scene)
		return;

	UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(uid);
	if (NULL == user_node)
		return;

	int length_broadcast = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_worldboss_hudun_reward_broadcast, 
		uid, user_node->user_name, (int)user_node->camp, roll_num, boss_id, hudun_index);
	if (length_broadcast > 0)
	{
		this->NotifySystemMsg(boss_scene, gamestring::GAMESTRING_BUF, length_broadcast, SYS_MSG_CENTER_NOTICE_3);
	}
}

void WorldBossManager::NotifySystemMsg(Scene* scene, char *str_buff, int str_len, int notice_type)
{
	if (NULL == scene) return;

	if (NULL != str_buff && str_len > 0) 
	{
		int sendlen = 0;
		static Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, str_buff, str_len, notice_type))
		{
			scene->SendToRole((const char*)&sm, sendlen);
		}
	}
}

void WorldBossManager::SendWorldBossListInfo(Role *role)
{
	if (NULL != role)
	{
		Protocol::SCWorldBossInfoList scweri;
		scweri.next_refresh_time = m_next_refresh_worldboss_time;
		scweri.count = 0;

		std::map<int, WorldBossInfo>::iterator iter = m_worldboss_info_map.begin();
		for (; scweri.count < LOGIC_CONFIG->GetWorldBossConfig().WorldBossGetMaxBossCount() && scweri.count < Protocol::SCWorldBossInfoList::MAX_BOSS_COUNT && m_worldboss_info_map.end() != iter; iter++)
		{
			WorldBossInfo &boss_info = iter->second;
			scweri.boss_list[scweri.count].boss_id = boss_info.boss_id;
			scweri.boss_list[scweri.count].status = boss_info.boss_status;

			for(int i = 0; i < WORLD_KILLER_LIST_MAX_COUNT;i++)
			{
				scweri.boss_list[scweri.count].killer_info_list[i].killer_uid = boss_info.killer_info_list[i].killer_uid;
				scweri.boss_list[scweri.count].killer_info_list[i].killier_time = boss_info.killer_info_list[i].killier_time;
				F_STRNCPY(scweri.boss_list[scweri.count].killer_info_list[i].killer_name, boss_info.killer_info_list[i].killer_name, sizeof(GameName));
			}

			++  scweri.count;
		}

		int sendlen = sizeof(scweri) - sizeof(scweri.boss_list[0]) * (Protocol::SCWorldBossInfoList::MAX_BOSS_COUNT - scweri.count);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&scweri, sendlen);
	}
}

void WorldBossManager::SendWorldBossPersonalHurtInfo(Role *role,int boss_id)
{
	if (NULL == role || boss_id <= 0) return;

	WorldBossInfo &boss_info = m_worldboss_info_map[boss_id];
	std::map<int, PersonHurtInfo> &hurt_map = boss_info.hurt_map;
	std::vector<PersonHurtRankItem> &person_rank_vec = boss_info.person_rank_vec;
	
	static Protocol::SCWorldBossSendPersonalHurtInfo cmd;
	cmd.my_hurt = 0;
	cmd.rank_count = 0;
	cmd.self_rank =0;
	
	std::map<int, PersonHurtInfo>::iterator hurt_map_it = hurt_map.find(role->GetUID());
	if (hurt_map.end() != hurt_map_it)
	{
		cmd.my_hurt = hurt_map_it->second.hurt;
	}

	int person_hurt_size = static_cast<int>(person_rank_vec.size());
	for (int i = 0;i < Protocol::SCWorldBossSendPersonalHurtInfo::RANK_NUM && i < person_hurt_size; ++ i)
	{
		Protocol::SCWorldBossSendPersonalHurtInfo::RankItem &rank_item = cmd.rank_list[i];

		PersonHurtRankItem person_hurt_item = person_rank_vec[i];

		rank_item.uid = person_hurt_item.uid;
		rank_item.hurt = person_hurt_item.hurt;

		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(rank_item.uid);
		if (NULL != user_node)
		{
			F_STRNCPY(rank_item.name, user_node->user_name, sizeof(GameName));
		}
		
		++ cmd.rank_count;

		if (rank_item.uid == role->GetUID())
		{
			cmd.self_rank = cmd.rank_count;
		}
	}
	
	unsigned int sendlen = 0;
	sendlen = sizeof(cmd) - sizeof(cmd.rank_list[0]) * (Protocol::SCWorldBossSendPersonalHurtInfo::RANK_NUM - cmd.rank_count);

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sendlen);
}

void WorldBossManager::SendWorldBossGuildHurtInfo(Role *role, int boss_id)
{
	if (NULL == role || boss_id <= 0) return;

	WorldBossInfo &boss_info = m_worldboss_info_map[boss_id];
	std::map<int, GuildHurtInfo> &hurt_map = boss_info.guild_hurt_map;
	std::vector<GuildHurtRankItem> &guild_rank_vec = boss_info.guild_rank_vec;

	static Protocol::SCWorldBossSendGuildHurtInfo cmd;
	cmd.my_guild_hurt = 0;
	cmd.my_guild_rank = 0;
	cmd.rank_count = 0;

	std::map<int, GuildHurtInfo>::iterator hurt_map_it = hurt_map.find(role->GetGuildID());
	if (hurt_map.end() != hurt_map_it)
	{
		cmd.my_guild_hurt = hurt_map_it->second.guild_hurt;
	}

	int guild_hurt_size = static_cast<int>(guild_rank_vec.size());
	for (int i = 0;i < Protocol::SCWorldBossSendGuildHurtInfo::RANK_NUM && i < guild_hurt_size; ++ i)
	{
		Protocol::SCWorldBossSendGuildHurtInfo::RankItem &rank_item = cmd.rank_list[i];

		GuildHurtRankItem guild_hurt_item = guild_rank_vec[i];

		rank_item.guild_id = guild_hurt_item.guild_id;
		rank_item.guild_hurt = guild_hurt_item.hurt;

		Guild *guild = GuildManager::Instance().GetGuild(rank_item.guild_id);
		if (NULL != guild)
		{
			guild->GetGuildName(rank_item.guild_name);
		}

		++ cmd.rank_count;

		if (rank_item.guild_id == role->GetGuildID())
		{
			cmd.my_guild_rank = cmd.rank_count;
		}
	}

	unsigned int sendlen = 0;
	if (cmd.rank_count < 5)
	{
		sendlen = sizeof(cmd) - sizeof(cmd.rank_list[0]) * (Protocol::SCGBGlobalInfo::RANK_NUM - cmd.rank_count);
	}
	else
	{
		sendlen = sizeof(cmd) - sizeof(cmd.rank_list[0]) * (Protocol::SCGBGlobalInfo::RANK_NUM - 5);
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sendlen);
}

void WorldBossManager::SendWorldBossWeekRankInfo(Role *role)
{
	if (NULL == role) return;
	
	static Protocol::SCWorldBossWeekRankInfo cmd;
	cmd.my_guild_kill_count = 0;
	cmd.my_guild_rank = 0;

	Guild *role_guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL != role_guild)
	{
		cmd.my_guild_kill_count = role_guild->GetLastWeekWorldBossKillNum();
	}

	int week_rank_size = static_cast<int>(m_guild_week_rank_vec.size());
	for (int i = 0;i < Protocol::SCWorldBossWeekRankInfo::RANK_NUM && i < week_rank_size; ++ i)
	{
		Protocol::SCWorldBossWeekRankInfo::RankItem &rank_item = cmd.rank_list[i];

		GuildWeekRankItem week_rank_item = m_guild_week_rank_vec[i];

		rank_item.guild_id = week_rank_item.guild_id;
		rank_item.guild_kill_count = week_rank_item.kill_worldboss_num;

		Guild *guild = GuildManager::Instance().GetGuild(rank_item.guild_id);
		if (NULL != guild)
		{
			guild->GetGuildName(rank_item.guild_name);
		}

		++ cmd.rank_count;
	}

	unsigned int sendlen = 0;
	if (cmd.rank_count < 20)
	{
		sendlen = sizeof(cmd) - sizeof(cmd.rank_list[0]) * (Protocol::SCGBGlobalInfo::RANK_NUM - cmd.rank_count);
	}
	else
	{
		sendlen = sizeof(cmd) - sizeof(cmd.rank_list[0]) * (Protocol::SCGBGlobalInfo::RANK_NUM - 20);
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sendlen);
}

void WorldBossManager::SendHurtInfoToAll(int boss_id)
{
	const WorldBossInfoCfg * boss_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossByBossId(boss_id);
	if (NULL == boss_cfg || boss_cfg->scene_id <= 0)
		return;

	Scene * boss_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(boss_cfg->scene_id, 0);
	if (NULL == boss_scene)
	 return;

	for (unsigned int i = 0; i < boss_scene->RoleNum(); ++ i)
	{
		Role *temp_role = boss_scene->GetRoleByIndex(i);
		if (NULL == temp_role)
			continue;
		this->SendWorldBossPersonalHurtInfo(temp_role, boss_id);
		this->SendWorldBossGuildHurtInfo(temp_role, boss_id);
	}
}

void WorldBossManager::SendHudunDestroyInfo(WorldBossInfo &boss_info, int hudun_index)
{
	if (boss_info.scence_id < 0) return;

	Scene * boss_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(boss_info.scence_id, 0);
	if (NULL == boss_scene)
		return;

	for (unsigned int i = 0; i < boss_scene->RoleNum(); ++ i)
	{
		Role *temp_role = boss_scene->GetRoleByIndex(i);
		if (NULL == temp_role)
			continue;
		Protocol::SCWorldBossCanRoll cmd;
		cmd.boss_id = boss_info.boss_id;
		cmd.hudun_index = hudun_index;
		EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (const char*)&cmd, sizeof(cmd));
	}
}

void WorldBossManager::SendTopRollInfo(int boss_id, int hudun_index, int top_roll_point, const char* top_name)
{
	const WorldBossInfoCfg * boss_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossByBossId(boss_id);
	if (NULL == boss_cfg || boss_cfg->scene_id <= 0)
		return;

	Scene * boss_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(boss_cfg->scene_id, 0);
	if (NULL == boss_scene)
		return;

	for (unsigned int i = 0; i < boss_scene->RoleNum(); ++ i)
	{
		Role *temp_role = boss_scene->GetRoleByIndex(i);
		if (NULL == temp_role)
			continue;

		Protocol::SCWorldBossRollTopPointInfo cmd;
		cmd.boss_id = boss_id;
		cmd.hudun_index = hudun_index;
		cmd.top_roll_point = top_roll_point;
		F_STRNCPY(cmd.top_roll_name , top_name, sizeof(GameName));
		EngineAdapter::Instance().NetSend(temp_role->GetNetId(), (const char*)&cmd, sizeof(cmd));
	}
}

void WorldBossManager::SendBossInfoToAllGateway(WorldBossInfo boss_info, int killer_uid)
{
	Protocol::SCWorldBossInfoToAll bita;
	if (0 == boss_info.boss_id)
	{
		return;
	}

	if (!Scene::IsWorldBossScene(boss_info.scence_id))
	{
		return;
	}

	bita.boss_id = boss_info.boss_id;
	bita.scene_id = boss_info.scence_id;
	bita.status = boss_info.boss_status;
	bita.next_refresh_time = 0;
	if (ENBOSSSTATUS_NONEXIST == boss_info.boss_status)
	{
		bita.next_refresh_time = m_next_refresh_worldboss_time;
	}
	bita.killer_uid = killer_uid;

	World::GetInstWorld()->SendToAllGateway((const char *)&bita, sizeof(bita));
}

void WorldBossManager::SendBossKillerList(Role *role, int boss_id)
{
	if (NULL == role || 0 >= boss_id)
	{
		return;
	}

	WorldBossMapIt boss_it = m_worldboss_info_map.find(boss_id);
	if (m_worldboss_info_map.end() != boss_it)
	{
		WorldBossInfo &boss_info = boss_it->second;
		Protocol::SCWorldBossKillerList wbkl;
		for(int i = 0; i < WORLD_KILLER_LIST_MAX_COUNT;i++)
		{
			wbkl.killer_info_list[i].killer_uid = boss_info.killer_info_list[i].killer_uid;
			wbkl.killer_info_list[i].killier_time = boss_info.killer_info_list[i].killier_time;
			F_STRNCPY(wbkl.killer_info_list[i].killer_name, boss_info.killer_info_list[i].killer_name, sizeof(GameName));
		}
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&wbkl, sizeof(wbkl));
	}
}

bool PersonRankFunc(const WorldBossManager::PersonHurtRankItem value1, const WorldBossManager::PersonHurtRankItem value2)
{
	return value1.hurt > value2.hurt;
}

bool GuildRankFunc(const WorldBossManager::GuildHurtRankItem value1, const WorldBossManager::GuildHurtRankItem value2)
{
	return value1.hurt > value2.hurt;
}

bool WeekRankFunc(const WorldBossManager::GuildWeekRankItem value1, const WorldBossManager::GuildWeekRankItem value2)
{
	if (value1.kill_worldboss_num != value2.kill_worldboss_num)
	{
		return value1.kill_worldboss_num > value2.kill_worldboss_num;
	}
	
	return value1.kill_num_last_change_time < value2.kill_num_last_change_time;
}

void WorldBossManager::CalcHurtRank(int boss_id)
{
	if (boss_id < 0) return;

	WorldBossInfo &boss_info = m_worldboss_info_map[boss_id];

	boss_info.person_rank_vec.clear();
	boss_info.guild_rank_vec.clear();

	{
		std::map<int, PersonHurtInfo>::iterator hurt_map_it = boss_info.hurt_map.begin();
		for (; boss_info.hurt_map.end() != hurt_map_it; hurt_map_it++)
		{
			boss_info.person_rank_vec.push_back(PersonHurtRankItem(hurt_map_it->second.uid, hurt_map_it->second.hurt));
		}

		if (!boss_info.person_rank_vec.empty())
		{
			std::sort(boss_info.person_rank_vec.begin(), boss_info.person_rank_vec.end(), &PersonRankFunc);
		}
	}

	{
		std::map<GuildID, GuildHurtInfo>::iterator guild_hurt_it = boss_info.guild_hurt_map.begin();
		for (; boss_info.guild_hurt_map.end() != guild_hurt_it; guild_hurt_it++)
		{
			boss_info.guild_rank_vec.push_back(GuildHurtRankItem(guild_hurt_it->second.guild_id, guild_hurt_it->second.guild_hurt));
		}

		if (!boss_info.guild_rank_vec.empty())
		{
			std::sort(boss_info.guild_rank_vec.begin(), boss_info.guild_rank_vec.end(), &GuildRankFunc);
		}
	}
}

void WorldBossManager::CalcWeekRank()
{
	m_guild_week_rank_vec.clear();

	const GuildManager::GuildMap &guild_map = GuildManager::Instance().GetGuildMap();
	std::map<GuildID, Guild*>::const_iterator it = guild_map.begin();
	for (; guild_map.end() != it; it++)
	{
		Guild *guild = it->second;
		if (NULL != guild)
		{
			guild->SetWorldBossLastWeekKillNum();

			if (guild->GetLastWeekWorldBossKillNum() > 0)
			{
				GuildWeekRankItem item;
				item.guild_id = guild->GetGuildID();
				item.kill_worldboss_num = guild->GetLastWeekWorldBossKillNum();
				item.kill_num_last_change_time = guild->GetWorldBossLastKillBossTime();
				guild->GetGuildName(item.guild_name);
				m_guild_week_rank_vec.push_back(item);
			}

			guild->ClearWorldBossCurrentKillNum();
		}
	}

	if (!m_guild_week_rank_vec.empty())
	{
		std::sort(m_guild_week_rank_vec.begin(), m_guild_week_rank_vec.end(), &WeekRankFunc);
	}
}

void WorldBossManager::RefreshAllWorldBoss()
{
	std::map<int, WorldBossInfo>::iterator iter = m_worldboss_info_map.begin();
	for(;m_worldboss_info_map.end() != iter; ++iter)
	{
		if (ENBOSSSTATUS_NONEXIST == iter->second.boss_status)
		{
			iter->second.boss_status = ENBOSSSTATUS_EXIST;
			WorldBossShadow::Instance().OnBossRefresh(iter->second);
			iter->second.person_rank_vec.clear();
			iter->second.guild_rank_vec.clear();
			iter->second.hurt_map.clear();
			iter->second.guild_hurt_map.clear();

			iter->second.is_change = 1;
			iter->second.is_activity_first_hudun = 0;
			iter->second.is_activity_second_hudun = 0;
			iter->second.trigger_hudun_reward = iter->second.shield_effective_times;
			this->CalcHurtRank(iter->second.boss_id);
			this->SendHurtInfoToAll(iter->second.boss_id);
		}
	}

	int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_worldboss_refresh);
	if (len > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL);
	}
}

void WorldBossManager::RefreshOneWorldBoss(WorldBossInfo &world_boss)
{
	if (world_boss.boss_id <= 0)
	{
		return;
	}

	WorldBossShadow::Instance().OnBossRefresh(world_boss);
}



