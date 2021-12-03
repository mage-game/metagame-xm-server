#include "specialcombineserverboss.hpp"
#include "global/combineserveractivity/combineserveractivitybossconfig.hpp"
#include "global/combineserveractivity/combineserveractivitymanager.hpp"
#include "protocal/msgactivity.hpp"

#include "world.h"
#include "engineadapter.h"
#include "config/logicconfigmanager.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "monster/monsterpool.h"
#include "obj/character/role.h"
#include "other/roleactivity/roleactivity.hpp"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"

#include "obj/character/monster.h"
#include "obj/otherobj/gatherobj.h"
#include "item/knapsack.h"
#include "scene/worldshadow/worldshadow.hpp"
#include "servercommon/string/gameworldstr.h"
#include "config/monsterreplaceconfig.hpp"

SpecialCombineServerBoss::SpecialCombineServerBoss(Scene *scene)
: SpecialLogic(scene), m_great_boss_refresh_status(STATUS_NOT_IN_TIME), m_great_boss_count(0), m_gather_count(0),
	m_common_boss_refresh_begin_time(0), m_common_boss_refresh_end_time(0), m_is_finish(false), m_common_boss_next_refresh_time(0), m_can_refresh_common_boss(false), is_activity_start(true)
{
	const std::map<int, CombineServerActivityBossCfg> &csa_boss_cfg_map = LOGIC_CONFIG->GetCSABossConfig().GetCSABossCfgMap();
	for (std::map<int, CombineServerActivityBossCfg>::const_iterator iter = csa_boss_cfg_map.begin(); iter != csa_boss_cfg_map.end(); ++iter)
	{
		CSABossInfo boss_info;
		switch (iter->second.boss_type)
		{
		case CombineServerActivityBossConfig::BOSS_TYPE_COMMON:
		{
			boss_info.next_flush_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			boss_info.boss_id = iter->second.boss_id;
			boss_info.born_pos = iter->second.pos;
			m_common_boss_map[boss_info.boss_id] = boss_info;
		}
		break;

		case CombineServerActivityBossConfig::BOSS_TYPE_GREAT:
		{
			boss_info.next_flush_time = 0;						//活动boss不是按时间刷新的
			boss_info.boss_id = iter->second.boss_id;
			boss_info.born_pos = iter->second.pos;
			m_great_boss_map[boss_info.boss_id] = boss_info;
		}
		break;

		default:
			break;
		}
	}

	time_t now_time = EngineAdapter::Instance().Time();
	time_t zero_time = GetZeroTime(now_time);
	m_common_boss_refresh_begin_time = static_cast<unsigned int>(zero_time) + LOGIC_CONFIG->GetCSABossConfig().GetCSABossOtherCfg().common_boss_begin_time;
	if (m_common_boss_refresh_begin_time > static_cast<unsigned int>(now_time))
	{
		m_common_boss_refresh_begin_time -= SECOND_PER_DAY;
	}

	m_common_boss_refresh_end_time = m_common_boss_refresh_begin_time + LOGIC_CONFIG->GetCSABossConfig().GetCSABossOtherCfg().common_boss_refresh_time;
}

SpecialCombineServerBoss::~SpecialCombineServerBoss()
{

}

void SpecialCombineServerBoss::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (is_activity_start)
	{
		unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(now_time));
		auto common_boss_refresh_cfg = LOGIC_CONFIG->GetCSABossConfig().GetCSACommonBossRefreshCfg();

		unsigned int next_refresh_time = 0;
		for (const auto &it : common_boss_refresh_cfg)
		{
			if (now_second < today_zero_time + it / 100 * 3600 + it % 100 * 60)
			{
				next_refresh_time = today_zero_time + it / 100 * 3600 + it % 100 * 60;
				break;
			}
		}

		if (next_refresh_time > 0)
		{
			this->OnActivityStatusChange(CSA_SUB_ACTIVITY_STATE_OPEN);
			m_common_boss_next_refresh_time = next_refresh_time;
			m_can_refresh_common_boss = true;
		}
		else
		{
			unsigned int tomorrow_zero_time = today_zero_time + 86400;

			for (const auto &it : common_boss_refresh_cfg)
			{
				if (now_second < tomorrow_zero_time + it / 100 * 3600 + it % 100 * 60)
				{
					next_refresh_time = tomorrow_zero_time + it / 100 * 3600 + it % 100 * 60;
					break;
				}
			}

			this->OnActivityStatusChange(CSA_SUB_ACTIVITY_STATE_OPEN);
			m_common_boss_next_refresh_time = next_refresh_time;
			m_can_refresh_common_boss = true;
		}

		is_activity_start = false;
	}

	if (static_cast<unsigned int>(now_second) >= m_common_boss_refresh_begin_time + SECOND_PER_DAY)
	{
		m_common_boss_refresh_begin_time += SECOND_PER_DAY;
		m_common_boss_refresh_end_time += SECOND_PER_DAY;
	}

	if (this->IsInGreatBossRefreshTime() && STATUS_NOT_IN_TIME == m_great_boss_refresh_status)
	{
		this->KillAllMonster();
		
		{
			for (auto &it : m_real_great_boss_map)
			{
				it.second.next_flush_time = 1;	//仅发给客户端，活动boss不随时间刷新
			}

			{
				unsigned int next_refresh_time = 0;

				if (m_common_boss_next_refresh_time > now_second)
				{
					next_refresh_time = m_common_boss_next_refresh_time;
				}
				else
				{
					unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(now_second));
					unsigned int tomorrow_zero_time = today_zero_time + 86400;

					auto common_boss_refresh_cfg = LOGIC_CONFIG->GetCSABossConfig().GetCSACommonBossRefreshCfg();
					for (const auto &it : common_boss_refresh_cfg)
					{
						if (now_second < tomorrow_zero_time + it / 100 * 3600 + it % 100 * 60)
						{
							next_refresh_time = tomorrow_zero_time + it / 100 * 3600 + it % 100 * 60;
							break;
						}
					}
				}

				for (auto &it : m_real_common_boss_map)
				{
					it.second.next_flush_time = next_refresh_time;
				}
			}
		}

		m_great_boss_refresh_status = STATUS_IN_TIME;
		this->RefreshGreatBoss();
	}

	if (!this->IsInGreatBossRefreshTime() && STATUS_IN_TIME == m_great_boss_refresh_status)
	{
		this->KillAllMonster();			//这里按语义应该是只销毁GreatBoss，后面会强制刷新CommonBoss

		{
			for (auto &it : m_real_great_boss_map)
			{
				it.second.next_flush_time = 1;	//仅发给客户端，活动boss不随时间刷新
			}

			{
				unsigned int next_refresh_time = 0;

				if (m_common_boss_next_refresh_time > now_second)
				{
					next_refresh_time = m_common_boss_next_refresh_time;
				}
				else
				{
					unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(now_second));
					unsigned int tomorrow_zero_time = today_zero_time + 86400;

					auto common_boss_refresh_cfg = LOGIC_CONFIG->GetCSABossConfig().GetCSACommonBossRefreshCfg();
					for (const auto &it : common_boss_refresh_cfg)
					{
						if (now_second < tomorrow_zero_time + it / 100 * 3600 + it % 100 * 60)
						{
							next_refresh_time = tomorrow_zero_time + it / 100 * 3600 + it % 100 * 60;
							break;
						}
					}
				}

				for (auto &it : m_real_common_boss_map)
				{
					it.second.next_flush_time = next_refresh_time;
				}
			}
		}

		m_great_boss_refresh_status = STATUS_NOT_IN_TIME;
		m_great_boss_count = 0;
		//this->CheckRefreshCommonBoss(static_cast<unsigned int>(now_second), true);
		this->SendBossInfo(NULL);
	}

	//if (this->IsInCommonBossRefreshTime() && STATUS_NOT_IN_TIME == m_great_boss_refresh_status)
	//{
	//	this->CheckRefreshCommonBoss(static_cast<unsigned int>(now_second));
	//}

	if (this->IsInCommonBossRefreshTime())
	{
		unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(now_second));

		if (m_common_boss_next_refresh_time == 0)
		{
			auto common_boss_refresh_cfg = LOGIC_CONFIG->GetCSABossConfig().GetCSACommonBossRefreshCfg();
			for (const auto &it:common_boss_refresh_cfg)
			{
				if (now_second <= today_zero_time + it / 100 * 3600 + it % 100 * 60)
				{
					m_common_boss_next_refresh_time = today_zero_time + it / 100 * 3600 + it % 100 * 60;		//设置下次刷新时间
					m_can_refresh_common_boss = true;

					break;
				}
			}
		}

		if (now_second >= m_common_boss_next_refresh_time)
		{
			if (m_can_refresh_common_boss)
			{
				this->RefreshCommonBoss();				//刷新普通boss
			}
			
			auto common_boss_refresh_cfg = LOGIC_CONFIG->GetCSABossConfig().GetCSACommonBossRefreshCfg();
			for (const auto &it : common_boss_refresh_cfg)
			{
				if (now_second < today_zero_time + it / 100 * 3600 + it % 100 * 60)
				{
					m_common_boss_next_refresh_time = today_zero_time + it / 100 * 3600 + it % 100 * 60;
					m_can_refresh_common_boss = true;

					break;
				}
			}
		}
	}
}

void SpecialCombineServerBoss::OnCharacterDie(Character *dead_cha, ObjID killer_objid)
{
	if (Obj::OBJ_TYPE_MONSTER == dead_cha->GetObjType())
	{
		Monster *dead_monster = (Monster *)dead_cha;
		//std::map<int, CSABossInfo>::iterator iter = m_great_boss_map.find(dead_monster->GetMonsterId());
		//if (iter != m_great_boss_map.end())
		std::map<int, CSABossInfo>::iterator iter = m_real_great_boss_map.find(dead_monster->GetMonsterId());
		if (iter != m_real_great_boss_map.end())
		{
			iter->second.next_flush_time = 1;							//仅发给客户端，活动boss不随时间刷新
			--m_great_boss_count;
			if (m_great_boss_count <= 0 && this->IsInGreatBossRefreshTime())
			{
				this->RefreshGather();
				//传闻
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_csa_boss_killall_greatboss);
					if (length > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
					}
				}
			}
		}
		else
		{
			//iter = m_common_boss_map.find(dead_monster->GetMonsterId());
			//if (iter != m_common_boss_map.end())
			iter = m_real_common_boss_map.find(dead_monster->GetMonsterId());
			if (iter != m_real_common_boss_map.end())
			{
				unsigned int next_flush_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) +
					LOGIC_CONFIG->GetCSABossConfig().GetCSABossOtherCfg().common_boss_refresh_interval;

				if (next_flush_time > m_common_boss_refresh_begin_time + LOGIC_CONFIG->GetCSABossConfig().GetCSABossOtherCfg().common_boss_refresh_time)
				{
					next_flush_time = m_common_boss_refresh_begin_time + SECOND_PER_DAY;
				}

				//iter->second.next_flush_time = next_flush_time;
				unsigned int now_second = (unsigned int)(EngineAdapter::Instance().Time());

				if (m_common_boss_next_refresh_time > now_second)
				{
					iter->second.next_flush_time = m_common_boss_next_refresh_time;
				}
				else
				{
					unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(now_second));
					unsigned int tomorrow_zero_time = today_zero_time + 86400;
					unsigned int next_refresh_time = 0;

					auto common_boss_refresh_cfg = LOGIC_CONFIG->GetCSABossConfig().GetCSACommonBossRefreshCfg();
					for (const auto &it : common_boss_refresh_cfg)
					{
						if (now_second < tomorrow_zero_time + it / 100 * 3600 + it % 100 * 60)
						{
							next_refresh_time = tomorrow_zero_time + it / 100 * 3600 + it % 100 * 60;
							break;
						}
					}

					if (next_refresh_time > now_second)
					{
						iter->second.next_flush_time = next_refresh_time;
					}
				}
			}
		}

		Role *killer = m_scene->GetPrincipal(killer_objid);
		if (NULL != killer)
		{
			++killer->GetRoleActivity()->GetParam()->csa_boss_daily_kill_count;
			CombineServerActivityManager::Instance().OnCSABossCheckRank(CombineServerActivityManager::BOSS_RANK_TYPE_PERSONAL,
				killer->GetUID(), killer->GetRoleActivity()->GetParam()->csa_boss_daily_kill_count);

			int guild_id = killer->GetGuildID();
			Guild *guild = GuildManager::Instance().GetGuild(guild_id);
			if (NULL != guild)
			{
				++guild->GetGuildCommonDataParam()->kill_csa_boss_num;
				CombineServerActivityManager::Instance().OnCSABossCheckRank(CombineServerActivityManager::BOSS_RANK_TYPE_GUILD,
					guild_id, guild->GetGuildCommonDataParam()->kill_csa_boss_num);
			}

			killer->GetRoleActivity()->SendCSABossRoleInfo();
		}

		this->SendBossInfo(NULL);
	}
}

void SpecialCombineServerBoss::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	const ItemConfigData *item = LOGIC_CONFIG->GetCSABossConfig().GetGatherRandRewardItem(gather_id);
	if (NULL != item)
	{
		role->GetKnapsack()->PutOrMail(*item, PUT_REASON_CSA_BOSS_GATHER);
		--m_gather_count;
		if (m_gather_count <= 0 && this->IsInGreatBossRefreshTime())
		{
			this->RefreshGreatBoss();
		}
	}

	this->SendBossInfo(NULL);
}

void SpecialCombineServerBoss::SendBossInfo(Role *role)
{
	static Protocol::SCCSABossInfo csa_boss_info;
	int boss_count = 0;
	//for (std::map<int, CSABossInfo>::iterator iter = m_common_boss_map.begin();
	//	iter != m_common_boss_map.end() && boss_count < COMBINE_SERVER_BOSS_MAX_COUNT; ++iter)
	for (std::map<int, CSABossInfo>::iterator iter = m_real_common_boss_map.begin();
	iter != m_real_common_boss_map.end() && boss_count < COMBINE_SERVER_BOSS_MAX_COUNT; ++iter)
	{
		//csa_boss_info.boss_info[boss_count].boss_id = static_cast<unsigned short>(iter->second.boss_id);
		csa_boss_info.boss_info[boss_count].boss_id = static_cast<unsigned short>(iter->second.original_monster_id);
		csa_boss_info.boss_info[boss_count].boss_type = CombineServerActivityBossConfig::BOSS_TYPE_COMMON;
		csa_boss_info.boss_info[boss_count].next_refresh_time = iter->second.next_flush_time;
		++boss_count;
	}

	//for (std::map<int, CSABossInfo>::iterator iter = m_great_boss_map.begin();
	//	iter != m_great_boss_map.end() && boss_count < COMBINE_SERVER_BOSS_MAX_COUNT; ++iter)
	for (std::map<int, CSABossInfo>::iterator iter = m_real_great_boss_map.begin();
	iter != m_real_great_boss_map.end() && boss_count < COMBINE_SERVER_BOSS_MAX_COUNT; ++iter)
	{
		//csa_boss_info.boss_info[boss_count].boss_id = static_cast<unsigned short>(iter->second.boss_id);
		csa_boss_info.boss_info[boss_count].boss_id = static_cast<unsigned short>(iter->second.original_monster_id);
		csa_boss_info.boss_info[boss_count].boss_type = CombineServerActivityBossConfig::BOSS_TYPE_GREAT;
		csa_boss_info.boss_info[boss_count].next_refresh_time = iter->second.next_flush_time;
		++boss_count;
	}

	csa_boss_info.refresh_status = m_great_boss_refresh_status;
	csa_boss_info.gather_count = m_gather_count;
	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&csa_boss_info, sizeof(csa_boss_info));
	}
	else
	{
		m_scene->SendToRole((const char*)&csa_boss_info, sizeof(csa_boss_info));
	}
}

void SpecialCombineServerBoss::OnActivityStatusChange(int status)
{
	switch (status)
	{

	case CSA_SUB_ACTIVITY_STATE_OPEN:
	{
		this->RefreshCommonBoss(true);
	}
	break;

	case CSA_SUB_ACTIVITY_STATE_FINISH:
		{
			m_is_finish = true;
			this->KillAllMonster();
			this->DelayKickOutAllRole();
		}
		break;
	default:
		break;
	}
}

void SpecialCombineServerBoss::CheckRefreshCommonBoss(unsigned int now_second, bool is_force)
{
	if (m_is_finish)
	{
		return;
	}

	bool is_refresh = false;
	for (std::map<int, CSABossInfo>::iterator iter = m_common_boss_map.begin(); iter != m_common_boss_map.end(); ++iter)
	{
		if ((iter->second.next_flush_time != 0 && now_second > iter->second.next_flush_time) || is_force)
		{
			const MonsterInitParam *param = MONSTERPOOL->GetMonsterParam(iter->second.boss_id);
			if (NULL != param)
			{
				long long extra_hp = param->hp * this->CalcBossExtraHpHelper();
				Monster *monster = MONSTERPOOL->CreateMonster(iter->second.boss_id, m_scene, iter->second.born_pos, extra_hp);
				if (NULL != monster)
				{
					iter->second.next_flush_time = 0;
					is_refresh = true;
				}
			}
		}
	}

	if (is_refresh)
	{
		this->SendBossInfo(NULL);
	}
}

void SpecialCombineServerBoss::RefreshGreatBoss()
{
	if (m_is_finish)
	{
		return;
	}

	for (std::map<int, CSABossInfo>::iterator iter = m_great_boss_map.begin(); iter != m_great_boss_map.end(); ++iter)
	{
		int replace_boss_id = LOGIC_CONFIG->GetReMonsterCfg().GetReMonsterIdByWorldLevel(iter->second.boss_id);

		bool is_refresh_now = true;
		for (const auto &temp_it : m_real_great_boss_map)
		{
			if (temp_it.second.original_monster_id == iter->second.boss_id)
			{
				if (temp_it.second.next_flush_time == 0)
				{
					is_refresh_now = false;
				}

				break;
			}
		}

		if (is_refresh_now)
		{
			for (auto temp_it = m_real_great_boss_map.begin(); temp_it != m_real_great_boss_map.end(); )
			{
				if (temp_it->second.original_monster_id == iter->second.boss_id)
				{
					m_real_great_boss_map.erase(temp_it++);

					break;
				}
				else
				{
					temp_it++;
				}
			}

			const MonsterInitParam *param = MONSTERPOOL->GetMonsterParam(replace_boss_id);
			if (NULL != param)
			{
				long long extra_hp = param->hp * this->CalcBossExtraHpHelper();
				Monster *monster = MONSTERPOOL->CreateMonster(replace_boss_id, m_scene, iter->second.born_pos, extra_hp);
				if (NULL != monster)
				{
					iter->second.next_flush_time = 0;
					++m_great_boss_count;

					CSABossInfo boss_info;
					boss_info.next_flush_time = 0;
					boss_info.boss_id = replace_boss_id;
					boss_info.born_pos = iter->second.born_pos;
					boss_info.original_monster_id = iter->second.boss_id;
					m_real_great_boss_map[boss_info.boss_id] = boss_info;
				}
			}
		}
	}

	//传闻
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_csa_boss_refresh_greatboss);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
		}
	}

	this->SendBossInfo(NULL);
}

void SpecialCombineServerBoss::RefreshGather()
{
	if (m_is_finish)
	{
		return;
	}

	const std::vector<Posi> &pos_vec = LOGIC_CONFIG->GetCSABossConfig().GetCSARandomPosiCfg();
	std::vector<Posi>::const_iterator iter = pos_vec.begin();
	for (int gather_type = CombineServerActivityBossConfig::GATHER_TYPE_GOLD; gather_type < CombineServerActivityBossConfig::GATHER_TYPE_MAX; ++gather_type)
	{
		const CombineServerActivityGatherCfg *gather_cfg = LOGIC_CONFIG->GetCSABossConfig().GetCSAGatherCfgByGatherType(gather_type);
		if (NULL == gather_cfg)
		{
			continue;
		}

		for (int i = 0; i < gather_cfg->count && iter != pos_vec.end(); ++i)
		{
			GatherObj *gather_obj = new GatherObj();
			gather_obj->SetPos(*iter);
			gather_obj->Init(NULL, gather_cfg->gather_id, gather_cfg->gather_time_ms, 0, true);
			m_scene->AddObj(gather_obj);
			++iter;
			++m_gather_count;
		}
	}

	this->SendBossInfo(NULL);
}

inline bool SpecialCombineServerBoss::IsInCommonBossRefreshTime()
{
	time_t now_time = EngineAdapter::Instance().Time();
	unsigned int today_zero_time = static_cast<unsigned int>(now_time - GetZeroTime(now_time));
	const CombineServerActivityBossOtherCfg &cfg = LOGIC_CONFIG->GetCSABossConfig().GetCSABossOtherCfg();
	if (cfg.common_boss_begin_time > cfg.common_boss_end_time)
	{
		return today_zero_time >= cfg.common_boss_begin_time || today_zero_time <= cfg.common_boss_end_time;
	}
	else
	{
		return today_zero_time >= cfg.common_boss_begin_time && today_zero_time <= cfg.common_boss_end_time;
	}
}

inline bool SpecialCombineServerBoss::IsInGreatBossRefreshTime()
{
	time_t now_time = EngineAdapter::Instance().Time();
	unsigned int today_zero_time = static_cast<unsigned int>(now_time - GetZeroTime(now_time));
	const CombineServerActivityBossOtherCfg &cfg = LOGIC_CONFIG->GetCSABossConfig().GetCSABossOtherCfg();
	if (cfg.great_boss_begin_time > cfg.great_boss_end_time)
	{
		return today_zero_time >= cfg.great_boss_begin_time || today_zero_time <= cfg.great_boss_end_time;
	}
	else
	{
		return today_zero_time >= cfg.great_boss_begin_time && today_zero_time <= cfg.great_boss_end_time;
	}
}

inline long long SpecialCombineServerBoss::CalcBossExtraHpHelper()
{
	int world_level = WorldShadow::Instance().GetWorldLevel();
	return static_cast<long long>(0.00005f * world_level * world_level - 0.02f * world_level + 1.875f);
}

void SpecialCombineServerBoss::KillAllCommonBoss()
{
	UInt32 monster_num = m_scene->MonsterNum();
	for (UInt32 i = 0; i < monster_num; ++i)
	{
		Monster *monster = m_scene->GetMonsterByIndex(i);
		if (NULL != monster)
		{
			if (m_common_boss_map.find(monster->GetMonsterId()) != m_common_boss_map.end())
			{
				monster->ForceSetDead();
			}
		}
	}
}

void SpecialCombineServerBoss::RefreshCommonBoss(bool is_force)			//林智扬需求，普通boss按配置的时间段刷新
{
	if (m_is_finish)
	{
		return;
	}

	for (std::map<int, CSABossInfo>::iterator iter = m_common_boss_map.begin(); iter != m_common_boss_map.end(); ++iter)
	{
		if (m_can_refresh_common_boss || is_force)
		{
			int replace_boss_id = LOGIC_CONFIG->GetReMonsterCfg().GetReMonsterIdByWorldLevel(iter->second.boss_id);

			bool is_refresh_now = true;
			for (const auto &temp_it : m_real_common_boss_map)
			{
				if (temp_it.second.original_monster_id == iter->second.boss_id)
				{
					if (temp_it.second.next_flush_time == 0)
					{
						is_refresh_now = false;
					}

					break;
				}
			}

			if (is_refresh_now)
			{
				for (auto temp_it = m_real_common_boss_map.begin(); temp_it != m_real_common_boss_map.end(); )
				{
					if (temp_it->second.original_monster_id == iter->second.boss_id)
					{
						m_real_common_boss_map.erase(temp_it++);

						break;
					}
					else
					{
						temp_it++;
					}
				}

				const MonsterInitParam *param = MONSTERPOOL->GetMonsterParam(replace_boss_id);
				if (NULL != param)
				{
					long long extra_hp = param->hp * this->CalcBossExtraHpHelper();
					Monster *monster = MONSTERPOOL->CreateMonster(replace_boss_id, m_scene, iter->second.born_pos, extra_hp);
					if (NULL != monster)
					{
						iter->second.next_flush_time = 0;

						CSABossInfo boss_info;
						boss_info.next_flush_time = 0;
						boss_info.boss_id = replace_boss_id;
						boss_info.born_pos = iter->second.born_pos;
						boss_info.original_monster_id = iter->second.boss_id;
						m_real_common_boss_map[boss_info.boss_id] = boss_info;
					}
				}
			}
		}
	}

	m_can_refresh_common_boss = false;

	this->SendBossInfo(NULL);
}
