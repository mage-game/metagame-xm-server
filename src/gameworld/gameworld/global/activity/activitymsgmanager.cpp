#include "activitymsgmanager.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "gameworld/obj/character/role.h"
#include "gameworld/other/cross/rolecross.h"
#include "gameworld/scene/scene.h"
#include "gameworld/config/logicconfigmanager.hpp"
#include "gameworld/config/activitymsgconfig.hpp"
#include "gameworld/gstr.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "gameworld/scene/scenemanager.h"
#include "gameworld/world.h"
#include "gameworld/config/guildconfig.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialcrossguildbattle.hpp"
#include "gameworld/scene/activityshadow/activityshadow.hpp"
#include "gameworld/global/guild/guildmanager.hpp"
#include "gameworld/global/guild/guild.hpp"
#include "servercommon/serverconfig/crossguildbattleconfig.hpp"

ActivityMsgManager& ActivityMsgManager::Instance()
{
	static ActivityMsgManager msg_manager;
	return msg_manager;
}

void ActivityMsgManager::Init()
{
	m_msg_role_map.clear();
	m_monster_attack_msg_time.clear();
}

void ActivityMsgManager::OnRoleLogout(Role * role)
{
	m_msg_role_map.erase(GetRoleKey(role));
}

void ActivityMsgManager::OnRoleDie(Scene * scene, Role* role, ObjID killer)
{

	//普通场景只有仙盟运镖和双倍护送需要
	if (Scene::SCENE_TYPE_COMMON_SCENE == scene->GetSceneType())
	{
		if (103 != scene->GetSceneID() || (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_HUSONG) && !ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILD_YUNBIAO)))
		{
			return;
		}
	}

	ActivityMsg *msg_cfg = LOGIC_CONFIG->GetActivityMsgConfig().GetActivityMsgType(scene->GetSceneType());
	if (nullptr == msg_cfg)
	{
		return;
	}

	Role * kill_role = scene->GetPrincipal(killer);
	if (msg_cfg->guilder_die >= MSG_TYPE_BROADCAST_INVILIDE && nullptr != kill_role && 
		(role->GetGuildPost()==GUILD_POST_FU_TUANGZHANG || role->GetGuildPost() == GUILD_POST_TUANGZHANG))
	{
		auto itr = m_guilder_role_die.find(scene->GetSceneType());
		unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		if (itr == m_guilder_role_die.end() || now_sec > itr->second)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "guilder_die_notice",
				role->GetUID(), role->GetName(), role->GetCamp(), kill_role->GetUID(), kill_role->GetName(), kill_role->GetCamp());
			if (lenth > 0) {
				this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, msg_cfg->guilder_die);
			}
			m_guilder_role_die[scene->GetSceneType()] = now_sec + LOGIC_CONFIG->GetActivityMsgConfig().GetGuilderDieTime();
		}
	}

	if ( msg_cfg->role_die > MSG_TYPE_BROADCAST_INVILIDE)
	{
		std::map<long long, RoleActivityMsg>::iterator itr = m_msg_role_map.find(this->GetRoleKey(role));
		if (itr != m_msg_role_map.end() && itr->second.scene_type == scene->GetSceneType())
		{
			itr->second.dead_count++;
			if (LOGIC_CONFIG->GetActivityMsgConfig().CheckRoleDeadCount(scene->GetSceneType(),itr->second.dead_count))
			{
				int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "role_die_count_notie", role->GetUID(), 
					role->GetName(), role->GetCamp(),itr->second.dead_count);
				if (lenth > 0){
					this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, msg_cfg->role_die);
				}
			}	
		}
		else
		{
			RoleActivityMsg role_data;
			role_data.user_key = this->GetRoleKey(role);
			role_data.scene_type = scene->GetSceneType();
			role_data.dead_count = 1;
			m_msg_role_map[role_data.user_key] = role_data;
		}
	}
	
	if(nullptr != kill_role)
	{
		std::map<long long, RoleActivityMsg>::iterator itr = m_msg_role_map.find(this->GetRoleKey(kill_role));
		if (itr != m_msg_role_map.end() && itr->second.scene_type == scene->GetSceneType())
		{
			itr->second.lian_zhan_count++;
			if (LOGIC_CONFIG->GetActivityMsgConfig().CheckRoleLianZhanCount(scene->GetSceneType(), itr->second.lian_zhan_count))
			{
				int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "lian_zhan_notice", kill_role->GetUID(), kill_role->GetName(),
					kill_role->GetCamp(), itr->second.lian_zhan_count);
				if (lenth > 0)
				{
				this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, msg_cfg->lian_zhan);
				}
			}
		}
		else
		{
			RoleActivityMsg role_data;
			role_data.user_key = this->GetRoleKey(kill_role);
			role_data.scene_type = scene->GetSceneType();
			role_data.lian_zhan_count = 1;
			m_msg_role_map[role_data.user_key] = role_data;
		}

		std::map<long long, RoleActivityMsg>::iterator it = m_msg_role_map.find(this->GetRoleKey(role));
		if (it != m_msg_role_map.end() && it->second.scene_type == scene->GetSceneType())
		{
			if (LOGIC_CONFIG->GetActivityMsgConfig().CheckRoleLianZhanEnd(scene->GetSceneType(), it->second.lian_zhan_count))
			{
				int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "lian_zhan_end_notice", role->GetUID(), role->GetName(), role->GetCamp(),
					kill_role->GetUID(), kill_role->GetName(), kill_role->GetCamp(), it->second.lian_zhan_count);
				if (lenth > 0) {
					this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, msg_cfg->lian_zhan);
				}
			}
			it->second.lian_zhan_count = 0;
		}
	}
}

void ActivityMsgManager::OnMonsterDie(Scene *scene, Monster* monster, ObjID killer)
{
	if (nullptr == scene || Scene::SCENE_TYPE_CROSS_GUILDBATTLE == scene->GetSceneType())
	{
		return;
	}

	ActivityMsg *msg_cfg = LOGIC_CONFIG->GetActivityMsgConfig().GetActivityMsgType(scene->GetSceneType());
	if (nullptr == msg_cfg || msg_cfg->monster_die <= MSG_TYPE_BROADCAST_INVILIDE)
	{
		return;
	}

	Role * kill_role = scene->GetPrincipal(killer);
	if ( nullptr == kill_role)
	{
		return;
	}

	if (LOGIC_CONFIG->GetActivityMsgConfig().NeedMonsterDeadMsg(scene->GetSceneType(), monster->GetMonsterId()))
	{
		GuildName gname;
		kill_role->GetGuildName(gname);

		if (monster->GetMonsterId() == LOGIC_CONFIG->GetGuildConfig().GetBiaoCheMonsterID())
		{
			if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILD_YUNBIAO))
			{
				return;
			}
			UserID user = monster->GetOwnerUserId();
			Role * own_role = World::GetInstWorld()->GetSceneManager()->GetRole(user);
			if (nullptr != own_role)
			{
				GuildName owng_name;
				own_role->GetGuildName(owng_name);
				int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "rob_biao_che_notice", gname, owng_name);
				if (lenth > 0)
				{
					this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, msg_cfg->monster_die);
				}
			}
		}
		else
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "monster_die_notice", gname, monster->GetMonsterId());
			if (lenth > 0)
			{
				this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, msg_cfg->monster_die);
			}
		}
	}
}

void ActivityMsgManager::MonsterOnAttack(Scene * scene, Monster* monster, ObjID attacker, Attribute delta_hp)
{
	if (nullptr == scene || nullptr == monster)
	{
		return;
	}

	if (Scene::SCENE_TYPE_CROSS_GUILDBATTLE == scene->GetSceneType() || Scene::SCENE_TYPE_GUILDBATTLE == scene->GetSceneType())
	{
		return;
	}

	ActivityMsg *msg_cfg = LOGIC_CONFIG->GetActivityMsgConfig().GetActivityMsgType(scene->GetSceneType());
	if (nullptr == msg_cfg || msg_cfg->monster_attack <= MSG_TYPE_BROADCAST_INVILIDE)
	{
		return;
	}
	Role * attk_role = scene->GetPrincipal(attacker);
	if (NULL == attk_role)
	{
		return;
	}

	if (monster->GetMonsterId() == LOGIC_CONFIG->GetGuildConfig().GetBiaoCheMonsterID())
	{
		if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILD_YUNBIAO))
		{
			return;
		}

		long long now_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		long long max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		long long old_hp = now_hp + (-delta_hp);

		int old_percent = static_cast<int>(max_hp > 0 ? old_hp * 100 / max_hp : 0);
		int now_percent = static_cast<int>(max_hp > 0 ? now_hp * 100 / max_hp : 0);

		if (old_percent > now_percent)
		{
			int warning_percent = LOGIC_CONFIG->GetActivityMsgConfig().CheckMonsterHpPercent(scene->GetSceneType(), old_percent, now_percent);
			GuildID guild_id = monster->GetSpecialParam();
			Guild* guild = GuildManager::Instance().GetGuild(guild_id);
			if (nullptr != guild && warning_percent >= 0)
			{
				int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "biao_che_hp_notice", guild->GetGuildName(), warning_percent);
				if (lenth > 0) {
					this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, msg_cfg->monster_attack,guild_id);
				}
			}
		}
		return;
	}

	UserID own_user = monster->GetOwnerUserId();
	if (INVALID_USER_ID != own_user)
	{
		int notify_cd = LOGIC_CONFIG->GetActivityMsgConfig().GetMonsterAttackNotifyCD(scene->GetSceneType(), monster->GetMonsterId());
		if (notify_cd > 0)
		{
			long long key = ParamToLongLong(monster->GetMonsterId(), UidToInt(own_user));
			unsigned int now_sec = static_cast<int>(EngineAdapter::Instance().Time());
			std::map<long long, unsigned int>::iterator itr = m_monster_attack_msg_time.find(key);
			if ((m_monster_attack_msg_time.end() != itr && itr->second + notify_cd < now_sec) ||
				m_monster_attack_msg_time.end() == itr)
			{
				m_monster_attack_msg_time[key] = now_sec;
				int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "monster_on_attack_warning", monster->GetMonsterId());
				if (lenth > 0)
				{
					this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, msg_cfg->monster_attack);
				}
			}
		}
	}
	else
	{
		if (LOGIC_CONFIG->GetActivityMsgConfig().NeedMonsterHpNotify(scene->GetSceneType(), monster->GetMonsterId()))
		{
			long long now_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			long long max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			long long old_hp = now_hp + (-delta_hp);

			int old_percent = static_cast<int>(max_hp > 0 ? old_hp * 100 / max_hp : 0);
			int now_percent = static_cast<int>(max_hp > 0 ? now_hp * 100 / max_hp : 0);

			if (old_percent > now_percent)
			{
				int warning_percent = LOGIC_CONFIG->GetActivityMsgConfig().CheckMonsterHpPercent(scene->GetSceneType(), old_percent, now_percent);
				if (warning_percent >= 0)
				{
					int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "monster_hp_warning", monster->GetMonsterId(), warning_percent);
					if (lenth > 0) {
						this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, msg_cfg->monster_attack);
					}
				}
			}
		}
	}
}
bool ActivityMsgManager::IsNeedRankTopMsg(Scene *scene)
{
	if (nullptr == scene)
	{
		return false;
	}
	long long key = ParamToLongLong(scene->GetSceneKey(), scene->GetSceneID());
	std::map<long long, unsigned int>::iterator itr = m_rank_top_time.find(key);
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	int interval = LOGIC_CONFIG->GetActivityMsgConfig().GetRankTopInterval();
	if (m_rank_top_time.end() == itr || itr->second + interval <= now_sec)
	{
		m_rank_top_time[key] = now_sec;
		return true;
	}

	return false;
}
void ActivityMsgManager::RankTopMsg(Scene *scene, RankTopItem * top_list, int count)
{
	if (nullptr == scene ||  nullptr == top_list ||count <= 0)
	{
		return;
	}
	if (Scene::SCENE_TYPE_CROSS_GUILDBATTLE == scene->GetSceneType())
	{
		int city_index = CrossGuildBattleConfig::Instance().GetCityIndex(scene->GetSceneID());
		if (count == 1)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_guild_battle_top1", city_index, top_list[0].user_name);
			if (lenth > 0) {
				this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, MSG_TYPE_BROADCAST_SCENE);
			}
		}
		else if (count == 2)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_guild_battle_top2", city_index,
				top_list[0].user_name, top_list[1].user_name);
			if (lenth > 0) {
				this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, MSG_TYPE_BROADCAST_SCENE);
			}
		}
		else if (count >= 3)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_guild_battle_top3", city_index,
				top_list[0].user_name, top_list[1].user_name, top_list[2].user_name);
			if (lenth > 0) {
				this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, MSG_TYPE_BROADCAST_SCENE);
			}
		}
	}
	else
	{
		if (count == 1)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "rank_top_notice1", top_list[0].user_name);
			if (lenth > 0) {
				this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, MSG_TYPE_BROADCAST_SCENE);
			}
		}
		else if (count == 2)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "rank_top_notice2",
				top_list[0].user_name, top_list[1].user_name);
			if (lenth > 0) {
				this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, MSG_TYPE_BROADCAST_SCENE);
			}
		}
		else if (count >= 3)
		{
			int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "rank_top_notice3",
				top_list[0].user_name, top_list[1].user_name, top_list[2].user_name);
			if (lenth > 0) {
				this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, MSG_TYPE_BROADCAST_SCENE);
			}
		}
	}
}

void ActivityMsgManager::RankFirstMsg(Scene *scene, RankTopItem & first)
{
	if (scene == nullptr)
	{
		return;
	}
	if (Scene::SCENE_TYPE_CROSS_GUILDBATTLE == scene->GetSceneType())
	{
		int city_index = CrossGuildBattleConfig::Instance().GetCityIndex(scene->GetSceneID());
		int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_guild_battle_first",city_index,first.user_name);
		if (lenth > 0) {
			this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, MSG_TYPE_BROADCAST_SCENE);
		}
	}
	else
	{
		int lenth = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "rank_top_first_notice", first.user_name);
		if (lenth > 0) {
			this->SendActivityMsg(scene, gstr::GSTR_BUF, lenth, MSG_TYPE_BROADCAST_SCENE);
		}
	}
}

long long ActivityMsgManager::GetRoleKey(Role *role)
{
	if (NULL == role)
	{
		return -1;
	}

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return static_cast<long long>(role->GetUID());
	}
	else
	{
		return UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	}
}

UniqueUserID ActivityMsgManager::GetGuildUniqueUserId(Role *role)
{
	if (NULL == role) return INVALID_UNIQUE_USER_ID;

	int plat_type = role->GetRoleCross()->GetCrossOriginalUniqueUserID().plat_type;
	UniqueUserID guild_id(plat_type, IntToUid(role->GetGuildID()));

	return guild_id;
}

RoleActivityMsg * ActivityMsgManager::GetRoleActivityMsg(long long user_key)
{
	std::map<long long, RoleActivityMsg>::iterator itr = m_msg_role_map.find(user_key);
	if (itr != m_msg_role_map.end())
	{
		return &itr->second;
	}
	return nullptr;
}

void ActivityMsgManager::SendActivityMsg(Scene * scene, char * str_buff, int len, int notice_type, GuildID guild_id)
{
	if (NULL != str_buff && len > 0)
	{
		if (notice_type == MSG_TYPE_BROADCAST_SCENE)
		{
			int send_len = 0;
			Protocol::SCSystemMsg sm;
			if (SceneManager::CreateSystemMsg(&sm, &send_len, str_buff, len, SYS_MSG_ACTIVITY_SPECIAL))
			{
				scene->SendToRole((const char*)&sm, send_len);
			}
		}
		else if (notice_type == MSG_TYPE_BROADCAST_GUILD)
		{
			int send_len = 0;
			Protocol::SCSystemMsg sm;
			if (SceneManager::CreateSystemMsg(&sm, &send_len, str_buff, len, SYS_MSG_ACTIVITY_SPECIAL))
			{
				scene->SendToRole((const char *)&sm, send_len, [guild_id](Role * send_role)->bool {
					if (send_role->GetGuildID() == guild_id)
					{
						return true;
					}
					return false; });
			}
		}
		else if (notice_type == MSG_TYPE_BROADCAST_GEMEWORLD)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(str_buff, len, SYS_MSG_ACTIVITY_SPECIAL);
		}
		else if(notice_type == MSG_TYPE_BROADCAST_ALL_GAMEWORLD)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(str_buff, len, SYS_MSG_ACTIVITY_SPECIAL);
		}
	}
}