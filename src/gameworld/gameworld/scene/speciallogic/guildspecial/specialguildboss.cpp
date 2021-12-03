#include "specialguildstation.hpp"

#include "globalconfig/globalconfig.h"
#include "obj/character/monster.h"
#include "internalcomm.h"
#include "obj/character/role.h"
#include "gamecommon.h"
#include "servercommon/noticenum.h"

#include "config/logicconfigmanager.hpp"
#include "servercommon/string/gameworldstr.h"
#include "item/itempool.h"
#include "other/route/mailroute.hpp"

#include "servercommon/errornum.h"
#include "config/guildactivityconfig.hpp"
#include "specialguildboss.hpp"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"

SpecialGuildBoss::SpecialGuildBoss(Scene *scene)
	: SpecialLogic(scene), m_is_inited(false)
{

}

SpecialGuildBoss::~SpecialGuildBoss()
{

}

void SpecialGuildBoss::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);
	
	if (!m_is_inited)
	{
		m_is_inited = true;

		this->ResetGuildBoss(false);
	}
}

void SpecialGuildBoss::OnCharacterDie(Character *dead, ObjID killer_id)
{
// 	if (NULL == dead) return;
// 
// 	if (Obj::OBJ_TYPE_MONSTER == dead->GetObjType())
// 	{
// 		Monster *monster = (Monster *)dead; 
// 
// 		const GuildBossConfig *boss_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetBossCfgBySceneId(m_scene->GetSceneID());
// 		
// 		if (NULL == boss_cfg || boss_cfg->boss_id != monster->GetMonsterId())
// 		{
// 			return;
// 		}
// 
// 		GuildID guild_id = m_scene->GetSceneKey();
// 
// 		Role *killer = m_scene->GetPrincipal(killer_id);
// 
// 		{
// 			if (NULL != killer && killer->GetGuildID() == guild_id)
// 			{
// 				killer->AddGuildGongxian(boss_cfg->killer_gongxian, "AddByLastKillBoss");
// 			}
// 		}
// 
// 		{
// 			Guild *guild = GuildManager::Instance().GetGuild(guild_id);
// 			if (NULL != guild)
// 			{
// 				// 考虑到玩家对boss放了个持续伤害然后下线
// 				// 注意：不管击杀者在不在，或者是不是该仙盟的人，都认为boss被击杀。只是显示上不会有击杀者的信息
// 				if (NULL == killer || killer->GetGuildID() != guild_id)
// 				{
// 					guild->OnGuildBossBeKilled(NULL, boss_cfg->index);
// 				}
// 				else
// 				{
// 					guild->OnGuildBossBeKilled(killer, boss_cfg->index);
// 				}
// 			}
// 		}
// 	}
}

void SpecialGuildBoss::ResetGuildBoss(bool is_kickout_all_role)
{
// 	if (is_kickout_all_role)
// 	{
// 		this->DelayKickOutAllRole();
// 	}
// 
// 	for (UInt32 i = 0; i < m_scene->MonsterNum(); i++)
// 	{
// 		Monster *monster = m_scene->GetMonsterByIndex(i);
// 		if (NULL != monster && Obj::OBJ_TYPE_MONSTER == monster->GetObjType())
// 		{
// 			monster->ForceSetDead();
// 		}
// 	}
// 
// 	GuildID guild_id = m_scene->GetSceneKey();
// 	Guild * guild = GuildManager::Instance().GetGuild(guild_id);
// 	if (NULL == guild)
// 	{
// 		return;
// 	}
// 	
// 	const GuildBossConfig *boss_cfg = LOGIC_CONFIG->GetGuildActivityCfg().GetBossCfgBySceneId(m_scene->GetSceneID());
// 	if (NULL == boss_cfg)
// 	{
// 		return;
// 	}
// 
// 	if (guild->IsBossBeKilled(boss_cfg->index))
// 	{
// 		return;
// 	}
// 
// 	const Posi boss_pos = Posi(boss_cfg->bosspos_x, boss_cfg->bosspos_y);
// 	MONSTERPOOL->CreateMonster(boss_cfg->boss_id, m_scene, boss_pos);
}