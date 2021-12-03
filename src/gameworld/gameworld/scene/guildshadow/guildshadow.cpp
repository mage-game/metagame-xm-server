#include "guildshadow.hpp"

#include "engineadapter.h"

#include "world.h"
#include "gstr.h"

#include "gamecommon.h"

#include "obj/obj.h"

#include "scene/scene.h"
#include "scene/worldshadow/worldshadow.hpp"
#include "scene/guildkeyregister/guildkeyregister.hpp"

#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"

#include "config/logicconfigmanager.hpp"
#include "monster/monsterpool.h"
#include "other/daycounter/daycounter.hpp"

#include "skill/skillbase.hpp"
#include "other/itemlevelreward/itemlevelreward.hpp"

GuildShadow & GuildShadow::Instance()
{
	static GuildShadow gs;
	return gs;
}


GuildShadow::GuildShadow() 
{

}

GuildShadow::~GuildShadow()
{

}

void GuildShadow::Update(unsigned long interval)
{
	this->UpdateBiaoChe(interval);
}

void GuildShadow::GetGuildName(GuildID guild_id, GuildName guild_name)
{
	if (INVALID_GUILD_ID == guild_id) return;

	Guild *guild = GuildManager::Instance().GetGuild(guild_id);
	if (NULL != guild)
	{
		guild->GetGuildName(guild_name);
	}
}

int GuildShadow::GetGuildLevel(GuildID guild_id)
{
	if (INVALID_GUILD_ID == guild_id) return 0;

	Guild *guild = GuildManager::Instance().GetGuild(guild_id);
	if (NULL != guild)
	{
		return guild->GetGuildLevel();
	}

	return 0;
}

int GuildShadow::GetGuildMemberCount(GuildID guild_id)
{
	if (INVALID_GUILD_ID == guild_id) return 0;

	Guild *guild = GuildManager::Instance().GetGuild(guild_id);
	if (NULL != guild)
	{
		return guild->GetGuildMemberManager().GetMemberCount();
	}

	return 0;
}

int GuildShadow::GetGuildCamp(GuildID guild_id)
{
	if (INVALID_GUILD_ID == guild_id) return CAMP_TYPE_INVALID;

	Guild *guild = GuildManager::Instance().GetGuild(guild_id);
	if (NULL != guild)
	{
		return guild->GetCamp();
	}

	return CAMP_TYPE_INVALID;
}


void GuildShadow::OnGuildYunBiaoStart(gglobalprotocal::GuildYunBiaoStart *ybs)
{
	const int biaoche_sceneid = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheSceneID();

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(biaoche_sceneid, 0);
	if (NULL != scene)
	{
		GuildBiaoCheInfoMapIt it = m_biaoche_map.find(ybs->guild_id);		// 已经有镖车 不太可能
		if (it != m_biaoche_map.end()) return;

		const int biaoche_monsterid = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheMonsterID();

		Posi birth_pos(0, 0);
		if (LOGIC_CONFIG->GetGuildConfig().GetBiaoChePos(0, &birth_pos))
		{
			int world_lv = WorldShadow::Instance().GetWorldLevel();
			int add_maxhp = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheAddMaxHp(world_lv);

			Monster *biaoche_monster = MonsterPool::Instance()->CreateMonster(biaoche_monsterid, scene, birth_pos, add_maxhp, 0, 0, ybs->guild_level, ybs->guild_id);
			if (NULL != biaoche_monster)
			{
				int exp_interval = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheExpInterval();
				int report_pos_interval = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheReportPosInterval();

				GuildBiaoCheInfo biaoche_info;
				biaoche_info.guild_id = ybs->guild_id;
				biaoche_info.guild_lv = ybs->guild_level;
				biaoche_info.objid = biaoche_monster->GetId();
				biaoche_info.pos_seq = 0;
				biaoche_info.next_give_husong_exp_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + exp_interval;
				biaoche_info.next_report_pos_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + report_pos_interval;
				biaoche_info.next_sync_biaoche_pos_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

				m_biaoche_map[ybs->guild_id] = biaoche_info;

				this->SyncBiaoCheStatusToGlobal(gglobalprotocal::YUNBIAO_STATUS_START, ybs->guild_id, ybs->guild_level, 0, 0, 0);
			}
		}
	}
}

void GuildShadow::OnGuildYunBiaoComplete(GuildID guild_id, int guild_lv)
{
	this->SyncBiaoCheStatusToGlobal(gglobalprotocal::YUNBIAO_STATUS_COMPLETE, guild_id, guild_lv, 0, 0, 0);
}

void GuildShadow::OnGuildYunBiaoFail(GuildID guild_id, int guild_lv, GuildID rob_guild_id, const UserID &rob_userid)
{
	GuildBiaoCheInfoMapIt it = m_biaoche_map.find(guild_id);
	if (it != m_biaoche_map.end())
	{
		std::map<GuildID, std::set<UserID> >::iterator hurt_guild_it = it->second.hurt_user_map.find(rob_guild_id);
		if (hurt_guild_it != it->second.hurt_user_map.end())
		{
//			const int get_rob_gongxian = LOGIC_CONFIG->GetGuildConfig().GetRobBiaoCheGongXian(it->second.guild_lv);
			const int get_member_rob_count_max = LOGIC_CONFIG->GetGuildConfig().GetMemberRobBiaoCheCountMax();

			for (std::set<UserID>::iterator hurt_user_it = hurt_guild_it->second.begin(); hurt_user_it != hurt_guild_it->second.end(); ++hurt_user_it)
			{
				Role *rob_role = World::GetInstWorld()->GetSceneManager()->GetRole(*hurt_user_it);  // 给这个仙盟所有对镖车造成伤害过的玩家发奖励
				if (NULL != rob_role)
				{
					if (rob_role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_ROB_GUILD_BIAOCHE_COUNT,
						get_member_rob_count_max)) // 每天最多3次
					{
//						rob_role->AddGuildGongxian(get_rob_gongxian, "RobBiaoCheGongXian");
						rob_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_ROB_GUILD_BIAOCHE_COUNT);
					}
				}
			}
		}

		this->SyncBiaoCheStatusToGlobal(gglobalprotocal::YUNBIAO_STATUS_FAIL, guild_id, guild_lv, rob_guild_id, 0, 0);

		m_biaoche_map.erase(it);  // 清理镖车
	}
}

void GuildShadow::SyncBiaoCheStatusToGlobal(int status, GuildID guild_id, int guild_lv, GuildID rob_guild_id, int biaoche_pos_x, int biaoche_pos_y)
{
	static gglobalprotocal::GuildYunBiaoStatus gybs;
	gybs.status = status;
	gybs.guild_id = guild_id;
	gybs.guild_lv = guild_lv;
	gybs.rob_guild_id = rob_guild_id;
	gybs.biaoche_pos_x = biaoche_pos_x;
	gybs.biaoche_pos_y = biaoche_pos_y;

	GuildManager::Instance().OnYunBiaoStatus(&gybs);
}

void GuildShadow::OnGuildYunBiaoDead(GuildID guild_id, GuildID rob_guild_id, const UserID &rob_userid)
{
	GuildBiaoCheInfoMapIt it = m_biaoche_map.find(guild_id);
	if (it != m_biaoche_map.end())
	{
		this->OnGuildYunBiaoFail(guild_id, it->second.guild_lv, rob_guild_id, rob_userid);
	}
}

void GuildShadow::OnForceGuildYunBiaoEnd()
{
	if (m_biaoche_map.empty())
	{
		return;
	}

	for (GuildBiaoCheInfoMapIt it = m_biaoche_map.begin(); it != m_biaoche_map.end(); it ++)
	{
		const int biaoche_sceneid = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheSceneID();
		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(biaoche_sceneid, 0);
		if (NULL != scene)
		{
			Obj *biaoche_obj = scene->GetObj(it->second.objid);
			if (NULL != biaoche_obj && Obj::OBJ_TYPE_MONSTER == biaoche_obj->GetObjType())
			{
				Monster *biaoche_monster = (Monster*)biaoche_obj;

				biaoche_monster->ForceSetDead();
			}
		}

		this->SyncBiaoCheStatusToGlobal(gglobalprotocal::YUNBIAO_STATUS_ACTIVITY_END_FAIL, it->first, it->second.guild_lv, 0, 0, 0);
	}

	m_biaoche_map.clear();
}

void GuildShadow::OnGuildYunBiaoBeHurt(GuildID guild_id, GuildID hurt_guild_id, const UserID &hurt_userid)
{
	GuildBiaoCheInfoMapIt it = m_biaoche_map.find(guild_id);
	if (it != m_biaoche_map.end())
	{
		std::map<GuildID, std::set<UserID> >::iterator hurt_guild_it = it->second.hurt_user_map.find(hurt_guild_id);
		if (hurt_guild_it != it->second.hurt_user_map.end())
		{
			hurt_guild_it->second.insert(hurt_userid);
		}
		else
		{
			std::set<UserID> hurt_user_set;
			hurt_user_set.insert(hurt_userid);

			it->second.hurt_user_map[hurt_guild_id] = hurt_user_set;
		}

		// 播报镖车被攻击
		unsigned int now_s = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		if (now_s >= it->second.next_report_biaoche_behurt_time)
		{
			int report_behurt_interval = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheBehurtReportInterval();
			it->second.next_report_biaoche_behurt_time += now_s + report_behurt_interval;

			const int biaoche_sceneid = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheSceneID();
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(biaoche_sceneid, 0);
			if (NULL != scene)
			{
				Obj *biaoche_obj = scene->GetObj(it->second.objid);
				if (NULL != biaoche_obj && Obj::OBJ_TYPE_MONSTER == biaoche_obj->GetObjType())
				{
					Monster *biaoche_monster = (Monster*)biaoche_obj;

					this->SyncBiaoCheStatusToGlobal(gglobalprotocal::YUNBIAO_STATUS_BE_HURT, it->second.guild_id, it->second.guild_lv, 0, biaoche_monster->GetPos().x, biaoche_monster->GetPos().y);
				}
			}
		}
	}
}

bool GuildShadow::IsCanAttackBiaoChe(Role *role, Monster *monster)
{
	if (nullptr == role || nullptr == monster)
	{
		return true;
	}

	int biaoche_id = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheMonsterID();
	if (monster->GetMonsterId() == biaoche_id)
	{
		if (role->GetGuildID() == INVALID_GUILD_ID)
		{
			return false;
		}

		GuildBiaoCheInfoMapIt it = m_biaoche_map.find(role->GetGuildID());
		if (it != m_biaoche_map.end())
		{
			if (it->second.objid == monster->GetId())
			{
				return false;
			}
		}
	}

	return true;
}

int GuildShadow::GetGuildBiaocheObjId(GuildID guild_id)
{
	GuildBiaoCheInfoMapIt it = m_biaoche_map.find(guild_id);
	if (it != m_biaoche_map.end())
	{
		return it->second.objid;
	}

	return INVALID_OBJ_ID;
}

void GuildShadow::BiaoCheGaveHusongExp(const GuildBiaoCheInfo &biaoche_info)
{
	const int biaoche_sceneid = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheSceneID();

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(biaoche_sceneid, 0);
	if (NULL != scene)
	{
		Obj *biaoche_obj = scene->GetObj(biaoche_info.objid);
		if (NULL != biaoche_obj && Obj::OBJ_TYPE_MONSTER == biaoche_obj->GetObjType())
		{
			Monster *biaoche_monster = (Monster*)biaoche_obj;

			const int get_exp_range = LOGIC_CONFIG->GetGuildConfig().GetGaveHusongExpRange();

			static ObjID target_list[256] = { 0 };
			int target_count = SkillBase::GetTargetByArea(biaoche_monster, biaoche_monster->GetPos(), get_exp_range, target_list, sizeof(target_list) / sizeof(target_list[0]));

			for (int i = 0; i < target_count; i++)
			{
				Obj *obj = scene->GetObj(target_list[i]);
				if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
				{
					Role *role = (Role*)obj;
					if (role->GetGuildID() == biaoche_info.guild_id)  // 护送自己的镖车的角色 给予奖励
					{
						//const int gave_exp = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheAddExp(role->GetLevel());

						//策划需求，修改增加经验规则（2018-1-3）
						long long gave_exp = static_cast<long long>(LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig().exp_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));

						role->AddExp(gave_exp, "BiaoCheGaveHusongExp", Role::EXP_ADD_REASON_HUSONG);
					}
				}
			}
		}
	}
}

void GuildShadow::BiaoCheReportPos(const GuildBiaoCheInfo &biaoche_info)
{
	const int biaoche_sceneid = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheSceneID();

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(biaoche_sceneid, 0);
	if (NULL != scene)
	{
		Obj *biaoche_obj = scene->GetObj(biaoche_info.objid);
		if (NULL != biaoche_obj && Obj::OBJ_TYPE_MONSTER == biaoche_obj->GetObjType())
		{
			Monster *biaoche_monster = (Monster*)biaoche_obj;

			GuildName guild_name; memset(guild_name, 0, sizeof(guild_name));
			this->GetGuildName(biaoche_info.guild_id, guild_name);
//			int guild_level = this->GetGuildLevel(biaoche_info.guild_id);
			int goal_percent = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheGoalPercent(biaoche_info.pos_seq);

			int len = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "guild_biaoche_pos_report", guild_name, goal_percent, scene->GetSceneID(),
				biaoche_monster->GetPos().x, biaoche_monster->GetPos().y);
			if (len > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE);
			}
		}
	}
}

void GuildShadow::UpdateBiaoChe(unsigned long interval)
{
	time_t now = EngineAdapter::Instance().Time();
	if (now >= m_last_update_biaoche_time + 2)
	{
		m_last_update_biaoche_time = now;

		std::vector<GuildID> complete_biaoche_vec;

		for (GuildBiaoCheInfoMapIt it = m_biaoche_map.begin(); it != m_biaoche_map.end(); ++it)
		{
			GuildBiaoCheInfo &biaoche_info = it->second;

			// 定时发经验
			if (now >= biaoche_info.next_give_husong_exp_time)
			{
				int gongxian_interval = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheExpInterval();
				biaoche_info.next_give_husong_exp_time = static_cast<unsigned int>(now) + gongxian_interval;

				this->BiaoCheGaveHusongExp(biaoche_info);
			}

			// 广播镖车坐标
			if (now >= biaoche_info.next_report_pos_time)
			{
				int report_pos_interval = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheReportPosInterval();
				biaoche_info.next_report_pos_time = static_cast<unsigned int>(now) + report_pos_interval;

				this->BiaoCheReportPos(biaoche_info);
			}

			int biaoche_sceneid = LOGIC_CONFIG->GetGuildConfig().GetBiaoCheSceneID();

			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(biaoche_sceneid, 0);
			if (NULL != scene)
			{
				Obj *biaoche_obj = scene->GetObj(biaoche_info.objid);
				if (NULL != biaoche_obj && Obj::OBJ_TYPE_MONSTER == biaoche_obj->GetObjType())
				{
					Monster *biaoche_monster = (Monster*)biaoche_obj;
					if (0 == biaoche_monster->GetDirDistance())		// 车停着 设置下一段路径
					{
						Posi next_pos(0, 0);
						if (LOGIC_CONFIG->GetGuildConfig().GetBiaoChePos(biaoche_info.pos_seq + 1, &next_pos))
						{
							Posi deltaP = next_pos - biaoche_monster->GetPos();
							Scalar distance = 0;
							Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
							biaoche_monster->SetDir(dir, distance);

							++biaoche_info.pos_seq;
						}
						else // 运镖完成 
						{
							this->OnGuildYunBiaoComplete(biaoche_info.guild_id, biaoche_info.guild_lv);

							scene->DeleteObj(biaoche_monster->GetId());
							complete_biaoche_vec.push_back(biaoche_info.guild_id);
						}
					}

					// 同步坐标
					if (now >= biaoche_info.next_sync_biaoche_pos_time)
					{
						this->SyncBiaoCheStatusToGlobal(gglobalprotocal::YUNBIAO_STATUS_UPDATE, biaoche_info.guild_id, biaoche_info.guild_lv, 0, biaoche_monster->GetPos().x, biaoche_monster->GetPos().y);
						biaoche_info.next_sync_biaoche_pos_time = static_cast<unsigned int>(now) + 5;
					}
				}
			}
		}

		for (std::vector<GuildID>::iterator it = complete_biaoche_vec.begin(); it != complete_biaoche_vec.end(); ++it)
		{
			m_biaoche_map.erase(*it);
		}
	}
}

