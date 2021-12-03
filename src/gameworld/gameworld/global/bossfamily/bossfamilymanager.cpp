#include "bossfamilymanager.hpp"
#include "obj/character/role.h"
#include "protocal/msgother.h"
#include "engineadapter.h"
#include "internalcomm.h"

#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/string/globalstr.h"
#include "servercommon/commondata.hpp"
#include "world.h"
#include "config/logicconfigmanager.hpp"

#include "scene/changescenedelayer.h"
#include "scene/sceneregister/sceneregister.hpp"
#include "scene/generater/scenemonsterlogic.h"
#include "scene/generater/monstergenerater.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"

#include "global/usercache/usercache.hpp"
#include "global/team/team.hpp"
#include "item/itemextern.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "config/activityconfig/randactivityconfig.hpp"
#include "other/rolegoal/rolegoalconfig.hpp"
#include "other/vip/vipconfig.hpp"
#include "global/bossfamily/bossfamilyconfig.hpp"
#include "monster/monsterpool.h"
#include "item/knapsack.h"
#include "friend/teaminfo.hpp"
#include "other/vip/vip.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/activedegree/activedegree.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/rolegoal/rolegoal.hpp"
#include "other/followboss/followbossmanager.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "other/shop/roleshop.hpp"
#include "scene/worldshadow/worldshadow.hpp"
#include "other/fb/rolepreciousboss.hpp"
#include "servercommon/fbdef.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "other/multimount/multimount.hpp"
#include "config/activityconfig/babybossconfig.hpp"
#include "scene/speciallogic/worldspecial/specialbabyboss.hpp"
#include "global/bossshanggu/bossshanggumanager.hpp"
#include "other/route/mailroute.hpp"
#include <algorithm>
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "other/touzijihua/touzijihua.hpp"
#include "servercommon/serverconfig/crossbossconfig.hpp"
#include "servercommon/serverconfig/crossmizangbossconfig.hpp"
#include "config/activityconfig/godmagicbossconfig.hpp"
#include "scene/speciallogic/worldspecial/specialgodmagicboss.hpp"
#include "global/worldstatus/worldstatus.hpp"

BossFamilyManager::BossFamilyManager() :m_next_refresh_time(0), m_refresh_interval(0), m_kill_mikuboss_add_weary(0),
m_dabao_kick_delay_time(0), m_active_kick_delay_time(0), m_is_init_precious_boss(false), m_send_active_boss_info_time(0), m_next_sync_role_num_time(0)
{
}

BossFamilyManager::~BossFamilyManager()
{

}

BossFamilyManager & BossFamilyManager::Instance()
{
	static BossFamilyManager wbm;
	return wbm;
}


void BossFamilyManager::Init(const BossSpecialRefreshParam &param)
{
	m_special_refresh_param = param;

	// 2019-02-16 不在重算刷新间隔
	//this->UpdateMikuBossRefreshInterval();
}

void BossFamilyManager::GetInitParam(BossSpecialRefreshParam *param)
{
	*param = m_special_refresh_param;
}

void BossFamilyManager::OnRoleLogin(Role *role)
{
	if (role->GetScene()->IsDabaoBossScene(role->GetScene()->GetSceneID()))
	{
		this->SendDabaoBossInfo(role);

		this->SendDabaoBossNextFlushInfo(role, role->GetScene()->GetSceneID());
	}
	else if (role->GetScene()->IsActiveBossScene(role->GetScene()->GetSceneID()))
	{
		this->OnRoleLoginActiveBoss(role);

		this->SendActiveBossInfo(role);
		this->SendActiveBossNextFlushInfo(role, role->GetScene()->GetSceneID());
	}
	else if (role->GetScene()->IsBossFamilyScene(role->GetScene()->GetSceneID()))
	{
		this->SendFamilyBossInfo(role);
	}
	else if (role->GetScene()->IsBossDongKuScene(role->GetScene()->GetSceneID()))
	{
		this->SendMikuBossInfo(role);
	}
}

void BossFamilyManager::OnRoleLogout(Role *role)
{
	this->OnRoleLogoutActiveBoss(role);
}

void BossFamilyManager::OnCheckComeBackWithOnLogin(Role *role, long long last_save_time)
{
	if (CrossConfig::Instance().IsHiddenServer()) return;
	if (nullptr == role || nullptr == role->GetScene()) return;

	const BossFamilyOtherCfg &bossfamily_othercfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyOtherCfg();
	time_t time_out_interval = static_cast<time_t>(bossfamily_othercfg.comeback_timeout_min * SECOND_PER_MIN);

	// 如果已经离线超时,则回到主城
	if (EngineAdapter::Instance().Time() - static_cast<time_t>(last_save_time) >= time_out_interval)
	{
		int scene_id = role->GetScene()->GetSceneID();
		if (role->GetScene()->IsActiveBossScene(scene_id) ||	// 简单BOSS
		   (role->GetScene()->IsBossFamilyScene(scene_id)) ||	// VIPBOSS
		   (role->GetScene()->IsBossDongKuScene(scene_id)) ||	// 困难BOSS
		   (Scene::SCENE_TYPE_GODMAGIC_BOSS == role->GetScene()->GetSceneType())) // 神魔BOSS
		{
			World::GetInstWorld()->GetSceneManager()->GoTo(role, bossfamily_othercfg.leave_scene_id, 0, bossfamily_othercfg.leave_pos);
		}
	}
}

void BossFamilyManager::OnServerStart()
{
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	{
		// boss之家
		const std::map<int, BossFamilyBossInfoCfg> &m_familyboss_map = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyBossMap();
		std::map<int, BossFamilyBossInfoCfg>::const_iterator iter = m_familyboss_map.begin();
		for (; m_familyboss_map.end() != iter; iter++)
		{
			if (iter->second.is_cross)
			{
				if (CrossConfig::Instance().IsHiddenServer())
				{
					FamilyBossInfo &cross_family_boss_scene = m_cross_familyboss_info_map[iter->second.scene_id];

					BossInfo &cross_family_boss = cross_family_boss_scene.boss_info_map[iter->second.bossID];
					cross_family_boss.boss_id = iter->second.bossID;
					cross_family_boss.scene_id = iter->second.scene_id;

					cross_family_boss.born_pos = iter->second.born_pos;
					cross_family_boss.next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 5;
					cross_family_boss.boss_status = ENBOSSSTATUS_NONEXIST;
					cross_family_boss.refresh_interval = iter->second.refresh_interval;

					for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
					{
						cross_family_boss.killer_info_list[i].killer_uid = 0;
						cross_family_boss.killer_info_list[i].killier_time = 0;
						memset(cross_family_boss.killer_info_list[i].killer_name, 0, sizeof(cross_family_boss.killer_info_list[i].killer_name));
					}
				}
			}
			else
			{
				FamilyBossInfo &m_familyboss_info = m_familyboss_info_map[iter->second.scene_id];

				BossInfo &family_boss = m_familyboss_info.boss_info_map[iter->second.bossID];
				family_boss.boss_id = iter->second.bossID;
				family_boss.scene_id = iter->second.scene_id;

				family_boss.born_pos = iter->second.born_pos;
				family_boss.next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				family_boss.boss_status = ENBOSSSTATUS_NONEXIST;
				family_boss.refresh_interval = iter->second.refresh_interval;

				for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
				{
					family_boss.killer_info_list[i].killer_uid = 0;
					family_boss.killer_info_list[i].killier_time = 0;
					memset(family_boss.killer_info_list[i].killer_name, 0, sizeof(family_boss.killer_info_list[i].killer_name));
				}
			}
		}
	}

	{
		// 秘窟boss
		const std::map<int, BossFamilyMikuBossInfoCfg> &m_mikuboss_map = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyMikuBossMap();
		std::map<int, BossFamilyMikuBossInfoCfg>::const_iterator iter = m_mikuboss_map.begin();
		for (; m_mikuboss_map.end() != iter; iter++)
		{
			MikuBossInfo &m_mikuboss_info = m_mikuboss_info_map[iter->second.scene_id];

			BossInfo &miku_boss = m_mikuboss_info.boss_info_map[iter->second.bossID];
			miku_boss.boss_id = iter->second.bossID;
			miku_boss.scene_id = iter->second.scene_id;

			miku_boss.born_pos = iter->second.born_pos;
			miku_boss.next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			miku_boss.boss_status = ENBOSSSTATUS_NONEXIST;
			miku_boss.refresh_interval = iter->second.refresh_interval;

			if (1 == iter->second.is_peace)  // 和平层才会计算排行
			{
				ActiveBossHurtRankInfo hurt_rank_info;
				m_mikuboss_info.hurt_rank_info_map.emplace((std::make_pair(miku_boss.boss_id, hurt_rank_info)));
			}

			for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
			{
				miku_boss.killer_info_list[i].killer_uid = 0;
				miku_boss.killer_info_list[i].killier_time = 0;
				memset(miku_boss.killer_info_list[i].killer_name, 0, sizeof(miku_boss.killer_info_list[i].killer_name));
			}
		}
	}

	{
		// 打宝boss
		const std::map<int, DabaoBossInfoCfg> &m_dabaoboss_map = LOGIC_CONFIG->GetBossFaimlyConfig().GetDabaoBossMap();
		std::map<int, DabaoBossInfoCfg>::const_iterator iter = m_dabaoboss_map.begin();
		for (; m_dabaoboss_map.end() != iter; iter++)
		{
			DabaoBossInfo &dabaoboss_info = m_dabaoboss_info_map[iter->second.scene_id];

			BossInfo &dabao_boss = dabaoboss_info.boss_info_map[iter->second.bossID];
			dabao_boss.boss_id = iter->second.bossID;
			dabao_boss.scene_id = iter->second.scene_id;
			dabao_boss.born_pos = iter->second.born_pos;
			dabao_boss.refresh_interval = iter->second.refresh_interval;
			dabao_boss.begin_refresh_time = iter->second.start_refresh_time;
			dabao_boss.end_refresh_time = iter->second.end_refresh_time;

			dabao_boss.next_refresh_time = now_sec;

			dabao_boss.boss_status = ENBOSSSTATUS_NONEXIST;

			if (dabaoboss_info.next_notice_time == 0)
			{
				dabaoboss_info.next_notice_time = dabao_boss.next_refresh_time;
				dabaoboss_info.scene_id = dabao_boss.scene_id;
			}

			for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
			{
				dabao_boss.killer_info_list[i].killer_uid = 0;
				dabao_boss.killer_info_list[i].killier_time = 0;
				memset(dabao_boss.killer_info_list[i].killer_name, 0, sizeof(dabao_boss.killer_info_list[i].killer_name));
			}
		}
	}

	{
		// 活跃boss
		const std::map<int, ActiveBossInfoCfg> &m_activeboss_map = LOGIC_CONFIG->GetBossFaimlyConfig().GetActiveBossMap();
		std::map<int, ActiveBossInfoCfg>::const_iterator iter = m_activeboss_map.begin();
		for (; m_activeboss_map.end() != iter; iter++)
		{
			ActiveBossInfo &m_activeboss_info = m_activeboss_info_map[iter->second.scene_id];

			BossInfo &active_boss = m_activeboss_info.boss_info_map[iter->second.bossID];
			active_boss.boss_id = iter->second.bossID;
			active_boss.scene_id = iter->second.scene_id;

			active_boss.born_pos = iter->second.born_pos;
			active_boss.next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			active_boss.boss_status = ENBOSSSTATUS_NONEXIST;
			active_boss.refresh_interval = iter->second.refresh_interval;

			{
				ActiveBossHurtRankInfo hurt_rank_info;
				m_activeboss_info.hurt_rank_info_map.emplace((std::make_pair(active_boss.boss_id, hurt_rank_info)));
			}
			for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
			{
				active_boss.killer_info_list[i].killer_uid = 0;
				active_boss.killer_info_list[i].killier_time = 0;
				memset(active_boss.killer_info_list[i].killer_name, 0, sizeof(active_boss.killer_info_list[i].killer_name));
			}
		}

		const std::set<int> & active_boss_scene_id_set = LOGIC_CONFIG->GetBossFaimlyConfig().GetActiveBossSceneIDSet();
		std::set<int>::const_iterator it = active_boss_scene_id_set.begin();
		for (; it != active_boss_scene_id_set.end(); ++it)
		{
			RefreshActiveBossInfo(*it);
		}
	}

	{
		// 宝宝boss
		const std::map<int, BabyBossCfg> &m_babyboss_map = LOGIC_CONFIG->GetBabyBossConfig().GetBabyBossCfgMap();
		std::map<int, BabyBossCfg>::const_iterator iter = m_babyboss_map.begin();
		for (; m_babyboss_map.end() != iter; iter++)
		{
			BabyBossInfo &m_babyboss_info = m_babyboss_info_map[iter->second.scene_id];

			BossInfo &baby_boss = m_babyboss_info.boss_info_map[iter->second.boss_id];
			baby_boss.boss_id = iter->second.boss_id;
			baby_boss.scene_id = iter->second.scene_id;
			baby_boss.born_pos = iter->second.born_pos;
			baby_boss.next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			baby_boss.boss_status = ENBOSSSTATUS_NONEXIST;

			for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
			{
				baby_boss.killer_info_list[i].killer_uid = 0;
				baby_boss.killer_info_list[i].killier_time = 0;
				memset(baby_boss.killer_info_list[i].killer_name, 0, sizeof(baby_boss.killer_info_list[i].killer_name));
			}
		}
	}

	{
		const BossFamilyOtherCfg& other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyOtherCfg();
		m_refresh_interval = other_cfg.refresh_interval;
		m_next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_kill_mikuboss_add_weary = other_cfg.kill_add_weary;
		m_dabao_kick_delay_time = other_cfg.dabao_kick_delay_time;
		m_active_kick_delay_time = other_cfg.active_kick_delay_time;
	}
}

void BossFamilyManager::OnStopServer()
{

}

void BossFamilyManager::GMRefreshFamilyBossNow()
{
	this->RefreshAllFamilyBoss();
}

void BossFamilyManager::GMRefreshMikuBossNow()
{
	this->RefreshAllMikuBoss();
}

void BossFamilyManager::Update(unsigned long interval, time_t now_second)
{
	if (WorldShadow::Instance().IsLoadWordLevel() && !m_is_init_precious_boss)
	{
		//秘藏坐标
		m_precious_pos_pool = LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousBossPosCfg();

		// 秘藏boss
		const std::vector<PreciousMonsterCfg> &precious_monster_vec = LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousMonsterWorldLevelCfg();
		if (!precious_monster_vec.empty())
		{
			const PreciousBossOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousBossOtherCfg();

			for (std::vector<PreciousMonsterCfg>::const_iterator iter = precious_monster_vec.begin(); precious_monster_vec.end() != iter; iter++)
			{
				int monster_count = iter->refresh_max_count;
				//boss信息
				if (PreciousMonsterCfg::PRECIOUS_MONSTER_TYPE_BOSS == iter->monster_type)
				{
					BossInfo &boss_info = m_precious_boss_info_map[iter->monster_id];
					boss_info.boss_id = iter->monster_id;
					boss_info.scene_id = other_cfg.precious_boss_scene_id;

					boss_info.next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
					boss_info.refresh_interval = iter->refresh_interval;
					boss_info.boss_status = ENBOSSSTATUS_NONEXIST;
					boss_info.seq = iter->seq;

					for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
					{
						boss_info.killer_info_list[i].killer_uid = 0;
						boss_info.killer_info_list[i].killier_time = 0;
						memset(boss_info.killer_info_list[i].killer_name, 0, sizeof(boss_info.killer_info_list[i].killer_name));
					}
				}
				//普通小怪信息
				else
				{
					for (int index = 0; index < monster_count; ++index)
					{
						MonsterInfo monster_info;
						monster_info.monster_id = iter->monster_id;
						monster_info.scene_id = other_cfg.precious_boss_scene_id;

						monster_info.next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
						monster_info.refresh_interval = iter->refresh_interval;
						monster_info.monster_status = ENBOSSSTATUS_NONEXIST;
						monster_info.seq = iter->seq;
						if (PreciousMonsterCfg::PRECIOUS_MONSTER_TYPE_COMMON_MONSTER == iter->monster_type)
						{
							monster_info.is_great_monster = false;
						}
						else
						{
							//精英怪信息
							monster_info.is_great_monster = true;
						}

						m_refresh_precious_monster_vec.push_back(monster_info);
					}
				}
			}

			for (int i = 0; i < other_cfg.gather_num; ++i)
			{
				PreciousGatherInfo gather_info;
				gather_info.is_gather = 0;
				gather_info.next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				m_precious_gather_vec.push_back(gather_info);
			}
		}

		m_is_init_precious_boss = true;
	}

	if ((unsigned int)now_second >= m_next_refresh_time)
	{
		BossFamilyBossMapIt family_it = m_familyboss_info_map.begin();
		for (; m_familyboss_info_map.end() != family_it; family_it++)
		{
			std::map<int, BossInfo>::iterator family_boss = family_it->second.boss_info_map.begin();
			for (; family_it->second.boss_info_map.end() != family_boss; family_boss++)
			{
				if (ENBOSSSTATUS_NONEXIST == family_boss->second.boss_status)
				{
					int flush_left_time_s = int(family_boss->second.next_refresh_time - (unsigned int)now_second);

					if (!family_boss->second.has_notice && flush_left_time_s <= SECOND_PER_MIN)
					{
						this->SendBossInfoToAllGateway(family_boss->second, Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY, Protocol::NOTIFY_REASON_BOSS_ADVANCE);
						family_boss->second.has_notice = true;
					}
					else if (flush_left_time_s <= 0)
					{
						this->RefreshOneFamilyBoss(family_boss->second);
						family_boss->second.has_notice = false;
					}
				}
			}
		}

		// cross family boss
		if (CrossConfig::Instance().IsHiddenServer())
		{
			BossFamilyBossMapIt family_it = m_cross_familyboss_info_map.begin();
			for (; m_cross_familyboss_info_map.end() != family_it; family_it++)
			{
				std::map<int, BossInfo>::iterator family_boss = family_it->second.boss_info_map.begin();
				for (; family_it->second.boss_info_map.end() != family_boss; family_boss++)
				{
					if (ENBOSSSTATUS_NONEXIST == family_boss->second.boss_status)
					{
						int flush_left_time_s = int(family_boss->second.next_refresh_time - (unsigned int)now_second);

						if (!family_boss->second.has_notice && flush_left_time_s <= SECOND_PER_MIN)
						{
							this->SendBossInfoToAllGateway(family_boss->second, Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY, Protocol::NOTIFY_REASON_BOSS_ADVANCE);  // cross family boss
							family_boss->second.has_notice = true;
						}
						else if (flush_left_time_s <= 0)
						{
							this->RefreshOneFamilyBoss(family_boss->second);
							family_boss->second.has_notice = false;
						}
					}

					//if (ENBOSSSTATUS_NONEXIST == family_boss->second.boss_status && (unsigned int)now_second > family_boss->second.next_refresh_time)
					//{
					//	this->RefreshOneFamilyBoss(family_boss->second);
					//}
				}
			}
		}

		MikuBossMapIt miku_it = m_mikuboss_info_map.begin();
		for (; m_mikuboss_info_map.end() != miku_it; miku_it++)
		{
			std::map<int, BossInfo>::iterator miku_boss = miku_it->second.boss_info_map.begin();
			for (; miku_it->second.boss_info_map.end() != miku_boss; miku_boss++)
			{
				if (ENBOSSSTATUS_NONEXIST == miku_boss->second.boss_status)
				{
					int flush_left_time_s = int(miku_boss->second.next_refresh_time - (unsigned int)now_second);

					if (!miku_boss->second.has_notice && flush_left_time_s <= SECOND_PER_MIN)
					{
						this->SendBossInfoToAllGateway(miku_boss->second, Protocol::BOSS_ENTER_TYPE_BOSS_MIKU, Protocol::NOTIFY_REASON_BOSS_ADVANCE);
						miku_boss->second.has_notice = true;
					}
					else if (flush_left_time_s <= 0)
					{
						this->RefreshOneMikuBoss(miku_boss->second);
						miku_boss->second.has_notice = false;
						std::map<int, ActiveBossHurtRankInfo>::iterator hurt_info_it = miku_it->second.hurt_rank_info_map.find(miku_boss->first);
						if (miku_it->second.hurt_rank_info_map.end() != hurt_info_it)
						{
							hurt_info_it->second.hurt_info_list.clear();
						}
					}
				}
			}
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(miku_it->first, 0);
			if (scene != nullptr)
			{
				int monster_num = scene->MonsterNum();
				for (int j = 0; j < monster_num && j < 100; ++j)
				{
					Monster *monster = scene->GetMonsterByIndex(j);
					if (monster != nullptr)
					{
						auto hurt_rank_it = miku_it->second.hurt_rank_info_map.find(monster->GetMonsterId());
						if (monster->IsFullHp() && hurt_rank_it != miku_it->second.hurt_rank_info_map.end() && hurt_rank_it->second.hurt_info_list.size() > 0)
						{
							hurt_rank_it->second.hurt_info_list.clear();
							hurt_rank_it->second.rank_data_dirty_flag = 0;
							this->SendMikuBossHurtRankInfo(monster);
						}
						if (hurt_rank_it != miku_it->second.hurt_rank_info_map.end() && hurt_rank_it->second.rank_data_dirty_flag > 0)
						{
							std::sort(hurt_rank_it->second.hurt_info_list.begin(), hurt_rank_it->second.hurt_info_list.end());
							hurt_rank_it->second.rank_data_dirty_flag = 0;
							this->SendMikuBossHurtRankInfo(monster);
						}
					}
				}
			}
		}

		DabaoBossMapIt dabao_it = m_dabaoboss_info_map.begin();
		for (; m_dabaoboss_info_map.end() != dabao_it; dabao_it++)
		{
			if (!Scene::IsDabaoBossScene(dabao_it->second.scene_id)) continue;

			std::map<int, BossInfo>::iterator dabao_boss = dabao_it->second.boss_info_map.begin();
			for (; dabao_it->second.boss_info_map.end() != dabao_boss; dabao_boss++)
			{
				if (ENBOSSSTATUS_NONEXIST == dabao_boss->second.boss_status)
				{
					int flush_left_time_s = int(dabao_boss->second.next_refresh_time - (unsigned int)now_second);

					if (!dabao_boss->second.has_notice && flush_left_time_s <= SECOND_PER_MIN)
					{
						this->SendBossInfoToAllGateway(dabao_boss->second, Protocol::BOSS_ENTER_TYPE_BOSS_DABAO, Protocol::NOTIFY_REASON_BOSS_ADVANCE);
						dabao_boss->second.has_notice = true;
					}
					else if (flush_left_time_s <= 0)
					{
						this->RefreshOneDabaoBoss(dabao_boss->second);
						dabao_boss->second.has_notice = false;
					}
				}
			}

			/*	if (dabao_it->second.next_notice_time > 0 && (unsigned int)now_second > dabao_it->second.next_notice_time)
				{
					int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_dabao_boss_refresh_notice,
						dabao_it->second.scene_id);
					if (sendlen > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE);
					}

					dabao_it->second.next_notice_time = 0;

					std::map<int, BossInfo>::iterator dabao_boss = dabao_it->second.boss_info_map.begin();
					if (dabao_boss != dabao_it->second.boss_info_map.end())
					{
						unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
						unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(static_cast<time_t>(now_sec)));
						unsigned int today_sec = now_sec - today_zero_time;

						if (today_sec < static_cast<unsigned int>(dabao_boss->second.begin_refresh_time))
						{
							dabao_it->second.next_notice_time = today_zero_time + dabao_boss->second.begin_refresh_time;
						}
						else if (today_sec > static_cast<unsigned int>(dabao_boss->second.end_refresh_time))
						{
							int next_day_refresh_interval = EngineAdapter::Instance().NextDayInterval(0, 0, 0) + dabao_boss->second.begin_refresh_time;
							dabao_it->second.next_notice_time = now_sec + next_day_refresh_interval;
						}
						else
						{
							if (dabao_boss->second.refresh_interval > 0)
							{
								dabao_it->second.next_notice_time = now_sec + (dabao_boss->second.refresh_interval - (today_sec - dabao_boss->second.begin_refresh_time) % dabao_boss->second.refresh_interval);
							}
						}
					}
				}*/
		}

		for (int scene_id = 9020; scene_id <= 9029; ++scene_id)
		{
			this->CheckKickOutDabao(scene_id, now_second);
		}

		ActiveBossMapIt active_it = m_activeboss_info_map.begin();
		for (; m_activeboss_info_map.end() != active_it; active_it++)
		{
			std::map<int, BossInfo>::iterator active_boss = active_it->second.boss_info_map.begin();
			for (; active_it->second.boss_info_map.end() != active_boss; active_boss++)
			{
				if (ENBOSSSTATUS_NONEXIST == active_boss->second.boss_status)
				{
					int flush_left_time_s = int(active_boss->second.next_refresh_time - (unsigned int)now_second);

					if (!active_boss->second.has_notice && flush_left_time_s <= SECOND_PER_MIN)
					{
						this->SendBossInfoToAllGateway(active_boss->second, Protocol::BOSS_ENTER_TYPE_BOSS_ACTIVE, Protocol::NOTIFY_REASON_BOSS_ADVANCE);
						active_boss->second.has_notice = true;
					}
					else if (flush_left_time_s <= 0)
					{
						this->RefreshOneActiveBossInfo(active_boss->second);
						active_boss->second.has_notice = false;
						std::map<int, ActiveBossHurtRankInfo>::iterator hurt_info_it = active_it->second.hurt_rank_info_map.find(active_boss->first);
						if (active_it->second.hurt_rank_info_map.end() != hurt_info_it)
						{
							hurt_info_it->second.hurt_info_list.clear();
						}
					}
				}
			}
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(active_it->first, 0);
			if (scene != nullptr)
			{
				int monster_num = scene->MonsterNum();
				for (int j = 0; j < monster_num && j < 100; ++j)
				{
					Monster *monster = scene->GetMonsterByIndex(j);
					if (monster != nullptr)
					{
						auto hurt_rank_it = active_it->second.hurt_rank_info_map.find(monster->GetMonsterId());
						if (monster->IsFullHp()&& hurt_rank_it != active_it->second.hurt_rank_info_map.end()&& hurt_rank_it->second.hurt_info_list.size() > 0)
						{
							hurt_rank_it->second.hurt_info_list.clear();
							hurt_rank_it->second.rank_data_dirty_flag = 0;
							this->SendActiveBossHurtRankInfo(monster);
						}
						if (hurt_rank_it != active_it->second.hurt_rank_info_map.end() && hurt_rank_it->second.rank_data_dirty_flag > 0)
						{
							std::sort(hurt_rank_it->second.hurt_info_list.begin(), hurt_rank_it->second.hurt_info_list.end());
							hurt_rank_it->second.rank_data_dirty_flag = 0;
							this->SendActiveBossHurtRankInfo(monster);
						}
					}
				}
			}
		}

		{
			// 秘藏boss
			for (std::map<int, BossInfo>::iterator iter = m_precious_boss_info_map.begin();
			m_precious_boss_info_map.end() != iter; iter++)
			{
				if (ENBOSSSTATUS_NONEXIST == iter->second.boss_status && iter->second.next_refresh_time != 0)
				{
					int flush_left_time_s = int(iter->second.next_refresh_time - (unsigned int)now_second);

					if (!iter->second.has_notice && flush_left_time_s <= SECOND_PER_MIN)
					{
						this->SendBossInfoToAllGateway(iter->second, Protocol::BOSS_ENTER_TYPE_BOSS_PRECIOUS, Protocol::NOTIFY_REASON_BOSS_ADVANCE);
						iter->second.has_notice = true;
					}
					else if (flush_left_time_s <= 0)
					{
						this->RefreshOnePreciousBoss(iter->second);
						iter->second.has_notice = false;
					}
					break;
				}
			}

			// 秘藏小怪
			for (PreciousMonsterInfoVectorIt monster_info = m_refresh_precious_monster_vec.begin(); monster_info != m_refresh_precious_monster_vec.end();)
			{
				if (ENBOSSSTATUS_NONEXIST == monster_info->monster_status && (unsigned int)now_second > monster_info->next_refresh_time && monster_info->next_refresh_time != 0)
				{
					this->RefreshPreciousMonster(*monster_info);
					monster_info = m_refresh_precious_monster_vec.erase(monster_info);
				}
				else
				{
					++monster_info;
				}
			}

			//秘藏采集物
			for (std::vector<PreciousGatherInfo>::iterator iter = m_precious_gather_vec.begin();
					m_precious_gather_vec.end() != iter; ++iter)
			{
				if (0 == iter->is_gather && (unsigned int)now_second > iter->next_refresh_time && iter->next_refresh_time != 0)
				{
					iter->next_refresh_time = 0;
					this->PreciousBossRefreshGather(*iter);
				}
			}
		}

		BabyBossMapIt baby_it = m_babyboss_info_map.begin();
		for (; m_babyboss_info_map.end() != baby_it; baby_it++)
		{
			std::map<int, BossInfo>::iterator baby_boss = baby_it->second.boss_info_map.begin();
			for (; baby_it->second.boss_info_map.end() != baby_boss; baby_boss++)
			{
				if (!LOGIC_CONFIG->GetBabyBossConfig().IsBabyBoss(baby_boss->second.boss_id))
				{
					continue;
				}

				if (ENBOSSSTATUS_NONEXIST == baby_boss->second.boss_status)
				{
					int flush_left_time_s = int(baby_boss->second.next_refresh_time - (unsigned int)now_second);

					if (!baby_boss->second.has_notice && flush_left_time_s <= SECOND_PER_MIN)
					{
						this->SendBossInfoToAllGateway(baby_boss->second, Protocol::BOSS_ENTER_TYPE_BOSS_BABY, Protocol::NOTIFY_REASON_BOSS_ADVANCE);
						baby_boss->second.has_notice = true;
					}
					else if (flush_left_time_s <= 0)
					{
						this->RefreshOneBabyBossInfo(baby_boss->second);
						baby_boss->second.has_notice = false;
					}
				}
			}
		}

		m_next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time() + m_refresh_interval);
	}
	if (now_second >= m_send_active_boss_info_time + 1)
	{
		m_send_active_boss_info_time = static_cast<int> (now_second);
		this->SendActiveBossHurtRankInfo();
		this->SendMikuBossHurtRankInfo();
	}

	if (now_second >= m_next_sync_role_num_time)
	{
		static const int SYNC_ROLE_NUM_INTERVAL_S = 300;
		m_next_sync_role_num_time = now_second + SYNC_ROLE_NUM_INTERVAL_S;

		this->SyncRoleNumToHidden();
	}

	// 云游boss
	if ((unsigned int)now_second >= m_next_refresh_yunyou_boss_time)
	{
		if (m_next_refresh_yunyou_boss_time > 0)
		{
			this->RefreshAllYunyouBoss();
		}

		LOGIC_CONFIG->GetBossFaimlyConfig().GetNextRefreshTimeOfYunyouBoss(&m_next_refresh_yunyou_boss_time);
	}
}

void BossFamilyManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	{
		BossFamilyBossMapIt family_it = m_familyboss_info_map.begin();
		for (; m_familyboss_info_map.end() != family_it; family_it++)
		{
			std::map<int, BossInfo>::iterator family_boss = family_it->second.boss_info_map.begin();
			for (; family_it->second.boss_info_map.end() != family_boss; family_boss++)
			{
				BossInfo &boss_info = family_boss->second;
				for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
				{
					boss_info.killer_info_list[i].killer_uid = 0;
					boss_info.killer_info_list[i].killier_time = 0;
					memset(boss_info.killer_info_list[i].killer_name, 0, sizeof(boss_info.killer_info_list[i].killer_name));
				}
			}
		}

		if (CrossConfig::Instance().IsHiddenServer())
		{
			BossFamilyBossMapIt family_it = m_cross_familyboss_info_map.begin();
			for (; m_cross_familyboss_info_map.end() != family_it; family_it++)
			{
				std::map<int, BossInfo>::iterator family_boss = family_it->second.boss_info_map.begin();
				for (; family_it->second.boss_info_map.end() != family_boss; family_boss++)
				{
					BossInfo &boss_info = family_boss->second;

					for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
					{
						boss_info.killer_info_list[i].killer_uid = 0;
						boss_info.killer_info_list[i].killier_time = 0;
						memset(boss_info.killer_info_list[i].killer_name, 0, sizeof(boss_info.killer_info_list[i].killer_name));
					}
				}
			}
		}
	}

	{
		MikuBossMapIt miku_it = m_mikuboss_info_map.begin();
		for (; m_mikuboss_info_map.end() != miku_it; miku_it++)
		{
			std::map<int, BossInfo>::iterator miku_boss = miku_it->second.boss_info_map.begin();
			for (; miku_it->second.boss_info_map.end() != miku_boss; miku_boss++)
			{
				BossInfo &boss_info = miku_boss->second;
				for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
				{
					boss_info.killer_info_list[i].killer_uid = 0;
					boss_info.killer_info_list[i].killier_time = 0;
					memset(boss_info.killer_info_list[i].killer_name, 0, sizeof(boss_info.killer_info_list[i].killer_name));
				}
			}
		}
	}

	{
		DabaoBossMapIt dabao_it = m_dabaoboss_info_map.begin();
		for (; m_dabaoboss_info_map.end() != dabao_it; dabao_it++)
		{
			std::map<int, BossInfo>::iterator dabao_boss = dabao_it->second.boss_info_map.begin();
			for (; dabao_it->second.boss_info_map.end() != dabao_boss; dabao_boss++)
			{
				BossInfo &boss_info = dabao_boss->second;
				for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
				{
					boss_info.killer_info_list[i].killer_uid = 0;
					boss_info.killer_info_list[i].killier_time = 0;
					memset(boss_info.killer_info_list[i].killer_name, 0, sizeof(boss_info.killer_info_list[i].killer_name));
				}
			}
		}
	}

	{
		ActiveBossMapIt active_it = m_activeboss_info_map.begin();
		for (; m_activeboss_info_map.end() != active_it; active_it++)
		{
			std::map<int, BossInfo>::iterator active_boss = active_it->second.boss_info_map.begin();
			for (; active_it->second.boss_info_map.end() != active_boss; active_boss++)
			{
				BossInfo &boss_info = active_boss->second;
				for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
				{
					boss_info.killer_info_list[i].killer_uid = 0;
					boss_info.killer_info_list[i].killier_time = 0;
					memset(boss_info.killer_info_list[i].killer_name, 0, sizeof(boss_info.killer_info_list[i].killer_name));
				}
			}
		}
	}

	{
		for (std::map<int, BossInfo>::iterator iter = m_precious_boss_info_map.begin(); m_precious_boss_info_map.end() != iter; iter++)
		{
			BossInfo &boss_info = iter->second;
			for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
			{
				boss_info.killer_info_list[i].killer_uid = 0;
				boss_info.killer_info_list[i].killier_time = 0;
				memset(boss_info.killer_info_list[i].killer_name, 0, sizeof(boss_info.killer_info_list[i].killer_name));
			}
		}
	}
}

// 只执行死亡刷新等，不触发其他相关的逻辑
void BossFamilyManager::BossDie(Monster* monster)
{
	if (nullptr == monster) return;

	if (CrossConfig::Instance().IsHiddenServer())
	{
		this->HiddenServerBossDie(monster);

		return;
	}

	unsigned short monster_id = monster->GetMonsterId();
	ObjID monster_objid = monster->GetId();

	long long  scene_id = monster->GetScene()->GetSceneID();
	long long monster_set_id = (long long)((scene_id << 32) + monster_objid);

	// boss之家（vipboss）
	if (m_familyboss_set.end() != m_familyboss_set.find(monster_set_id))
	{
		if (m_familyboss_info_map.end() != m_familyboss_info_map.find(monster->GetScene()->GetSceneID()))
		{
			FamilyBossInfo& family_boss = m_familyboss_info_map[monster->GetScene()->GetSceneID()];

			if (family_boss.boss_info_map.end() != family_boss.boss_info_map.find(monster_id))
			{
				BossInfo & boss_family = family_boss.boss_info_map[monster_id];
				boss_family.boss_status = ENBOSSSTATUS_NONEXIST;
				// 根据平台导入玩家数调整刷新间隔
				int interval_s = WorldStatus::Instance().GetRefreshTimeInterval((int)boss_family.refresh_interval);
				boss_family.next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time() + interval_s);

				this->SendFamilyBossInfoToAll(boss_family.scene_id);
			}
		}
	}
}

void BossFamilyManager::HiddenServerBossDie(Monster *monster)
{
	if (nullptr == monster)
	{
		return;
	}

	int scene_id = monster->GetScene()->GetSceneID();
	int monster_id = monster->GetMonsterId();

	// 同步到common gameworld
	crossgameprotocal::CrossGameCrossCommonBossInfoChange ccbsi;

	{
		ccbsi.boss_type = Protocol::BOSS_ENTER_TYPE_IVNALID;
		ccbsi.reason_type = crossgameprotocal::CROSS_COMMON_BOSS_SYNC_REASON_TYPE_DIE;
		ccbsi.scene_id = scene_id;
		ccbsi.boss_id = monster_id;
		ccbsi.killer.uid = 0;
		ccbsi.killer.kill_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		memset(ccbsi.killer.name, 0, sizeof(GameName));
	}

	auto family_it = m_cross_familyboss_info_map.find(scene_id);
	if (family_it != m_cross_familyboss_info_map.end())
	{
		auto monster_it = family_it->second.boss_info_map.find(monster_id);
		if (monster_it != family_it->second.boss_info_map.end())
		{
			BossInfo &boss_info = monster_it->second;

			boss_info.boss_status = ENBOSSSTATUS_NONEXIST;

			// 跨服VIP BOSS修改为根据人数动态调整刷新间隔
			unsigned int interval_s = this->GetBossSpecialFlushInterval(monster_id);
			if (interval_s <= 0) interval_s = boss_info.refresh_interval;  // 如果动态刷新取不到配置，则使用VIPBoss配置的间隔

			boss_info.next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time() + interval_s);

			this->SendFamilyBossInfoToAll(boss_info.scene_id);

			ccbsi.boss_type = Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY;
			ccbsi.next_refresh_timestamp = boss_info.next_refresh_time;
		}
	}

	if (ccbsi.boss_type != Protocol::BOSS_ENTER_TYPE_IVNALID)
	{
		InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_COMMON, 0, (const char *)&ccbsi, sizeof(ccbsi));
	}
}

void BossFamilyManager::OnDabaoBossDie(Monster *monster)
{
	if (nullptr == monster) return;

	//const BossFamilyOtherCfg &cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyOtherCfg();
	int scene_id = monster->GetScene()->GetSceneID();

	std::map<int, DabaoBossInfo>::iterator iter = m_dabaoboss_info_map.find(scene_id);
	if (m_dabaoboss_info_map.end() != iter)
	{
		std::map<int, BossInfo>::iterator dabao_boss_it = iter->second.boss_info_map.find(monster->GetMonsterId());
		if (iter->second.boss_info_map.end() != dabao_boss_it)
		{
			BossInfo & dabao_boss = dabao_boss_it->second;
			dabao_boss.boss_status = ENBOSSSTATUS_NONEXIST;

			unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			// 根据平台导入玩家数调整刷新间隔
			int interval_s = WorldStatus::Instance().GetRefreshTimeInterval((int)dabao_boss.refresh_interval);
			dabao_boss.next_refresh_time = now_sec + interval_s;

			this->SendDabaoBossNextFlushInfoToAll(scene_id);
		}
	}
}

void BossFamilyManager::OnActiveBossDie(Monster *monster)
{
	if (nullptr == monster) return;

	int scene_id = monster->GetScene()->GetSceneID();

	std::map<int, ActiveBossInfo>::iterator iter = m_activeboss_info_map.find(scene_id);
	if (m_activeboss_info_map.end() != iter)
	{
		std::map<int, BossInfo>::iterator active_boss_it = iter->second.boss_info_map.find(monster->GetMonsterId());
		if (iter->second.boss_info_map.end() != active_boss_it)
		{
			BossInfo & active_boss = active_boss_it->second;
			active_boss.boss_status = ENBOSSSTATUS_NONEXIST;
			// 根据平台导入玩家数调整刷新间隔
			int interval_s = WorldStatus::Instance().GetRefreshTimeInterval((int)active_boss.refresh_interval);
			active_boss.next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + interval_s;

			this->SendActiveBossNextFlushInfoToAll(scene_id);
		}
	}
}

void BossFamilyManager::OnMikuBossDie(Monster *monster)
{
	if (nullptr == monster) return;

	//unsigned short monster_id = monster->GetMonsterId();
	ObjID monster_objid = monster->GetId();

	long long  scene_id = monster->GetScene()->GetSceneID();
	long long monster_set_id = (long long)((scene_id << 32) + monster_objid);

	// 精英boss
	if (m_mikuboss_set.end() != m_mikuboss_set.find(monster_set_id))
	{
		if (m_mikuboss_info_map.end() != m_mikuboss_info_map.find(monster->GetScene()->GetSceneID()))
		{
			MikuBossInfo& miku_boss = m_mikuboss_info_map[monster->GetScene()->GetSceneID()];

			if (miku_boss.boss_info_map.end() != miku_boss.boss_info_map.find(monster->GetMonsterId()))
			{
				BossInfo & boss_miku = miku_boss.boss_info_map[monster->GetMonsterId()];
				boss_miku.boss_status = ENBOSSSTATUS_NONEXIST;
				// 根据平台导入玩家数调整刷新间隔
				int interval_s = WorldStatus::Instance().GetRefreshTimeInterval((int)boss_miku.refresh_interval);
				boss_miku.next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time() + interval_s);

				this->SendMikuBossInfoToAll(boss_miku.scene_id);
			}
		}
	}
}

void BossFamilyManager::OnPreciousBossDie(Monster *monster)
{
	if (NULL == monster)
	{
		return;
	}

	//unsigned short monster_obj_id = monster->GetId();

	int monster_id = monster->GetMonsterId();
	if (m_precious_boss_info_map.find(monster_id) != m_precious_boss_info_map.end())
	{
		//下次刷新时间
		{
			m_precious_boss_info_map[monster_id].boss_status = ENBOSSSTATUS_NONEXIST;
			m_precious_boss_info_map[monster_id].next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time())
				+ m_precious_boss_info_map[monster_id].refresh_interval;
		}

		this->SendPreciousBossInfoToAll();
	}
}

void BossFamilyManager::OnYunyouBossDie(Monster *monster, int scene_id)
{
	if (NULL == monster) return;

	// 更新云游boss信息
	{
		YunyouBossInfo info;
		info.scene_id = scene_id;
		info.boss_id = monster->GetMonsterId();
		info.born_pos_x = monster->GetPos().x;
		info.born_pos_y = monster->GetPos().y;
		info.is_dead = true;

		this->UpdateYunyouBossInfo(info);
	}
}

void BossFamilyManager::OnRoleDie(int boss_type, Role *role)
{
	if (nullptr == role) return;

	switch (boss_type)
	{
	case Protocol::BOSS_ENTER_TYPE_BOSS_DABAO:
	{
		if (role->GetScene() == nullptr || !Scene::IsDabaoBossScene(role->GetScene()->GetSceneID())) return;

		const BossFamilyOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyOtherCfg();
		CommonDataParam *common_p = role->GetCommonDataParam();
		if (NULL == common_p) return;

		if (common_p->dabao_angry_value < other_cfg.max_value)
		{
			common_p->dabao_angry_value += other_cfg.dabao_angry_value_die;

			if (common_p->dabao_angry_value >= other_cfg.max_value)
			{
				common_p->dabao_kick_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.dabao_kick_delay_time;
			}
			this->SendDabaoBossInfo(role);
		}
	}
	break;

	default:
		break;
	}
}

void BossFamilyManager::MonsterBeKill(Role* killer, Monster *monster)
{
	if (NULL == killer || NULL == monster) return;

	if (CrossConfig::Instance().IsHiddenServer())
	{
		this->HiddenServerMonsterBeKill(killer, monster);

		return;
	}

	unsigned short monster_id = monster->GetMonsterId();
	ObjID monster_objid = monster->GetId();

	long long  scene_id = monster->GetScene()->GetSceneID();
	long long monster_set_id = (long long)((scene_id << 32) + monster_objid);

	// boss之家（vipboss）
	if (m_familyboss_set.end() != m_familyboss_set.find(monster_set_id))
	{
		if (m_familyboss_info_map.end() != m_familyboss_info_map.find(monster->GetScene()->GetSceneID()))
		{
			FamilyBossInfo& family_boss = m_familyboss_info_map[monster->GetScene()->GetSceneID()];

			if (family_boss.boss_info_map.end() != family_boss.boss_info_map.find(monster_id))
			{
				BossInfo & boss_family = family_boss.boss_info_map[monster_id];

				unsigned int min_kill_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				int killer_index = -1;
				for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
				{
					if (0 == boss_family.killer_info_list[i].killer_uid)
					{
						boss_family.killer_info_list[i].killer_uid = killer->GetUID();
						boss_family.killer_info_list[i].killier_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
						F_STRNCPY(boss_family.killer_info_list[i].killer_name, killer->GetName(), sizeof(GameName));
						killer_index = -1;
						break;
					}
					else
					{
						if (boss_family.killer_info_list[i].killier_time < min_kill_time)
						{
							min_kill_time = boss_family.killer_info_list[i].killier_time;
							killer_index = i;
						}
					}
				}

				if (0 <= killer_index && killer_index < KILLER_LIST_MAX_COUNT)
				{
					boss_family.killer_info_list[killer_index].killer_uid = killer->GetUID();
					boss_family.killer_info_list[killer_index].killier_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
					F_STRNCPY(boss_family.killer_info_list[killer_index].killer_name, killer->GetName(), sizeof(GameName));
				}

				this->SendBossInfoToAllGateway(boss_family, Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY, Protocol::NOTIFY_REASON_BOSS_DIE, killer->GetUID());
				this->SendFamilyBossInfoToAll(boss_family.scene_id);
				killer->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_KILL_BOSS_TYPE, 1, COND_FAMILY_BOSS);
				killer->GetRoleGoal()->CheckCond(COND_TYPE_KILL_BOSS_TYPE, 1, COND_FAMILY_BOSS);
				killer->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_VIP_BOSS);

				killer->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_KILL_VIP_BOSS, 1);

				killer->GetMagicalPreciousManager()->CheckAddKillBossTimes(CONDITION_TYPE_KILL_VIP_MIKU_BOSS1, 1, monster->GetLevel());
				killer->GetMagicalPreciousManager()->CheckAddKillBossTimes(CONDITION_TYPE_KILL_VIP_MIKU_BOSS2, 1, monster->GetLevel());
				killer->GetMagicalPreciousManager()->CheckAddKillBossTimes(CONDITION_TYPE_KILL_VIP_MIKU_BOSS3, 1, monster->GetLevel());
			}
		}

		m_familyboss_set.erase(monster_set_id);
	}

	// 精英boss
	if (m_mikuboss_set.end() != m_mikuboss_set.find(monster_set_id))
	{
		if (m_mikuboss_info_map.end() != m_mikuboss_info_map.find(monster->GetScene()->GetSceneID()))
		{
			MikuBossInfo& miku_boss = m_mikuboss_info_map[monster->GetScene()->GetSceneID()];

			if (miku_boss.boss_info_map.end() != miku_boss.boss_info_map.find(monster->GetMonsterId()))
			{
				BossInfo & boss_miku = miku_boss.boss_info_map[monster->GetMonsterId()];

				unsigned int min_kill_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				int killer_index = -1;
				for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
				{
					if (0 == boss_miku.killer_info_list[i].killer_uid)
					{
						boss_miku.killer_info_list[i].killer_uid = killer->GetUID();
						boss_miku.killer_info_list[i].killier_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
						F_STRNCPY(boss_miku.killer_info_list[i].killer_name, killer->GetName(), sizeof(GameName));
						killer_index = -1;
						break;
					}
					else
					{
						if (boss_miku.killer_info_list[i].killier_time < min_kill_time)
						{
							min_kill_time = boss_miku.killer_info_list[i].killier_time;
							killer_index = i;
						}
					}
				}

				if (0 <= killer_index && killer_index < KILLER_LIST_MAX_COUNT)
				{
					boss_miku.killer_info_list[killer_index].killer_uid = killer->GetUID();
					boss_miku.killer_info_list[killer_index].killier_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
					F_STRNCPY(boss_miku.killer_info_list[killer_index].killer_name, killer->GetName(), sizeof(GameName));
				}

				this->SendBossInfoToAllGateway(boss_miku, Protocol::BOSS_ENTER_TYPE_BOSS_MIKU, Protocol::NOTIFY_REASON_BOSS_DIE, killer->GetUID());
				this->SendMikuBossInfoToAll(boss_miku.scene_id);
				killer->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_KILL_BOSS_TYPE, 1, COND_MIKU_BOSS);
				killer->GetRoleGoal()->CheckCond(COND_TYPE_KILL_BOSS_TYPE, 1, COND_MIKU_BOSS);
				killer->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_KILL_MIKU_BOSS, 1);

				killer->GetMagicalPreciousManager()->CheckAddKillBossTimes(CONDITION_TYPE_KILL_VIP_MIKU_BOSS1, 1, monster->GetLevel());
				killer->GetMagicalPreciousManager()->CheckAddKillBossTimes(CONDITION_TYPE_KILL_VIP_MIKU_BOSS2, 1, monster->GetLevel());
				killer->GetMagicalPreciousManager()->CheckAddKillBossTimes(CONDITION_TYPE_KILL_VIP_MIKU_BOSS3, 1, monster->GetLevel());
				killer->GetRoleActivity()->AddOpenServerInvestFinishParam(RandActivityOpenServerInvestOtherCfg::INVEST_TYPE_BOSS, 0, false);
				//killer->AddKillBossNum(MonsterInitParam::BOSS_TYPE_TASK_MIKU, 1);

				killer->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_MIKU_BOSS);

				//const BossFamilyMikuBossInfoCfg * miku_boss_info_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetMikuBossInfo(monster->GetMonsterId());
				//if ((nullptr != miku_boss_info_cfg) && (1 != miku_boss_info_cfg->is_peace))  // 不是和平层才能增加投资计划
				//{
				//	killer->GetTouzijihua()->OnKillBoss();
				//}
			}
		}

		m_mikuboss_set.erase(monster_set_id);
	}
}

void BossFamilyManager::OnDabaoMonsterBeKill(Monster *monster, Role *role)
{
	if (NULL == role || NULL == monster) return;

	bool is_boss = monster->IsBossMonster();
	const BossFamilyOtherCfg &cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyOtherCfg();
	int scene_id = role->GetScene()->GetSceneID();
	if (!role->GetScene()->IsDabaoBossScene(scene_id))
	{
		return;
	}

	if (is_boss)
	{
		std::map<int, DabaoBossInfo>::iterator iter = m_dabaoboss_info_map.find(scene_id);
		if (m_dabaoboss_info_map.end() != iter)
		{
			std::map<int, BossInfo>::iterator dabao_boss_it = iter->second.boss_info_map.find(monster->GetMonsterId());
			if (iter->second.boss_info_map.end() != dabao_boss_it)
			{
				BossInfo & dabao_boss = dabao_boss_it->second;

				unsigned int min_kill_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				int killer_index = -1;
				for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
				{
					if (0 == dabao_boss.killer_info_list[i].killer_uid)
					{
						dabao_boss.killer_info_list[i].killer_uid = role->GetUID();
						dabao_boss.killer_info_list[i].killier_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
						F_STRNCPY(dabao_boss.killer_info_list[i].killer_name, role->GetName(), sizeof(GameName));
						killer_index = -1;
						break;
					}
					else
					{
						if (dabao_boss.killer_info_list[i].killier_time < min_kill_time)
						{
							min_kill_time = dabao_boss.killer_info_list[i].killier_time;
							killer_index = i;
						}
					}
				}

				if (0 <= killer_index && killer_index < KILLER_LIST_MAX_COUNT)
				{
					dabao_boss.killer_info_list[killer_index].killer_uid = role->GetUID();
					dabao_boss.killer_info_list[killer_index].killier_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
					F_STRNCPY(dabao_boss.killer_info_list[killer_index].killer_name, role->GetName(), sizeof(GameName));
				}

				this->SendBossInfoToAllGateway(dabao_boss, Protocol::BOSS_ENTER_TYPE_BOSS_DABAO, Protocol::NOTIFY_REASON_BOSS_DIE, role->GetUID());
				role->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_KILL_BOSS_TYPE, 1, COND_DABAO_BOSS);
				role->GetRoleGoal()->CheckCond(COND_TYPE_KILL_BOSS_TYPE, 1, COND_DABAO_BOSS);
				role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_KILL_DABAO_BOSS, 1);
			}
		}
	}

	if (role->GetTeamInfo()->InTeam())
	{
		Team *team = role->GetTeamInfo()->GetMyTeam();
		if (NULL == team) return;

		int team_user_list[MAX_TEAM_MEMBER_NUM]; memset(team_user_list, 0, sizeof(team_user_list));
		int team_user_count = team->GetMemberUidList(team_user_list, MAX_TEAM_MEMBER_NUM);

		if (team_user_count <= 0)
		{
			return;
		}

		for (int i = 0; i < team_user_count; ++i)
		{
			Role *join_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(team_user_list[i]));
			if (NULL == join_role || join_role->GetScene()->GetSceneID() != role->GetScene()->GetSceneID())
			{
				continue;
			}

			CommonDataParam *common_p = join_role->GetCommonDataParam();
			if (NULL == common_p) return;

			if (common_p->dabao_angry_value >= 0 && common_p->dabao_angry_value < cfg.max_value)
			{
				if (is_boss)
				{
					this->SendDabaoBossNextFlushInfo(join_role, scene_id);
				}
				else
				{
					common_p->dabao_angry_value += cfg.kill_monster_value;
					if (common_p->dabao_angry_value >= cfg.max_value)
					{
						common_p->dabao_kick_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + cfg.dabao_kick_delay_time;
					}
				}
			}

			this->SendDabaoBossInfo(join_role);
		}

	}
	else
	{
		CommonDataParam *common_p = role->GetCommonDataParam();
		if (NULL == common_p) return;

		if (common_p->dabao_angry_value >= 0 && common_p->dabao_angry_value < cfg.max_value)
		{
			if (is_boss)
			{
				this->SendDabaoBossNextFlushInfo(role, scene_id);
			}
			else
			{
				common_p->dabao_angry_value += cfg.kill_monster_value;
				if (common_p->dabao_angry_value >= cfg.max_value)
				{
					common_p->dabao_kick_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + cfg.dabao_kick_delay_time;
				}
			}

			this->SendDabaoBossInfo(role);
		}
	}
}

void BossFamilyManager::OnActiveMonsterBeKill(Monster *monster, Role *role)
{
	if (NULL == role || NULL == monster) return;

	bool is_boss = monster->IsBossMonster();

	int scene_id = role->GetScene()->GetSceneID();
	if (!role->GetScene()->IsActiveBossScene(scene_id))
	{
		return;
	}

	if (is_boss)
	{
		std::map<int, ActiveBossInfo>::iterator iter = m_activeboss_info_map.find(scene_id);
		if (m_activeboss_info_map.end() != iter)
		{
			std::map<int, BossInfo>::iterator active_boss_it = iter->second.boss_info_map.find(monster->GetMonsterId());
			if (iter->second.boss_info_map.end() != active_boss_it)
			{
				BossInfo & active_boss = active_boss_it->second;

				unsigned int min_kill_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				int killer_index = -1;
				for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
				{
					if (0 == active_boss.killer_info_list[i].killer_uid)
					{
						active_boss.killer_info_list[i].killer_uid = role->GetUID();
						active_boss.killer_info_list[i].killier_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
						F_STRNCPY(active_boss.killer_info_list[i].killer_name, role->GetName(), sizeof(GameName));
						killer_index = -1;
						break;
					}
					else
					{
						if (active_boss.killer_info_list[i].killier_time < min_kill_time)
						{
							min_kill_time = active_boss.killer_info_list[i].killier_time;
							killer_index = i;
						}
					}
				}

				if (0 <= killer_index && killer_index < KILLER_LIST_MAX_COUNT)
				{
					active_boss.killer_info_list[killer_index].killer_uid = role->GetUID();
					active_boss.killer_info_list[killer_index].killier_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
					F_STRNCPY(active_boss.killer_info_list[killer_index].killer_name, role->GetName(), sizeof(GameName));
				}

				this->SendBossInfoToAllGateway(active_boss, Protocol::BOSS_ENTER_TYPE_BOSS_ACTIVE, Protocol::NOTIFY_REASON_BOSS_DIE, role->GetUID());
				role->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_KILL_BOSS_TYPE, 1, COND_ACTIVE_BOSS);
				role->GetRoleGoal()->CheckCond(COND_TYPE_KILL_BOSS_TYPE, 1, COND_ACTIVE_BOSS);
				role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_KILL_ACTIVE_BOSS, 1);
				role->GetRoleActivity()->AddOpenServerInvestFinishParam(RandActivityOpenServerInvestOtherCfg::INVEST_TYPE_BOSS, 0);
				//role->AddKillBossNum(MonsterInitParam::BOSS_TYPE_TASK_ACTIVE, 1);
			}
		}
	}

	if (role->GetTeamInfo()->InTeam())
	{
		Team *team = role->GetTeamInfo()->GetMyTeam();

		int team_user_list[MAX_TEAM_MEMBER_NUM]; memset(team_user_list, 0, sizeof(team_user_list));
		int team_user_count = team->GetMemberUidList(team_user_list, MAX_TEAM_MEMBER_NUM);

		if (team_user_count <= 0)
		{
			return;
		}

		for (int i = 0; i < team_user_count; ++i)
		{
			Role *join_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(team_user_list[i]));
			if (NULL == join_role || join_role->GetScene()->GetSceneID() != role->GetScene()->GetSceneID())
			{
				continue;
			}

			if (is_boss)
			{
				this->SendActiveBossNextFlushInfo(join_role, scene_id);
				join_role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_ACTIVE_BOSS);
			}

			this->SendActiveBossInfo(join_role);
		}

	}
	else
	{
		if (is_boss)
		{
			this->SendActiveBossNextFlushInfo(role, scene_id);
			role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_ACTIVE_BOSS);
		}
			this->SendActiveBossInfo(role);
	}
	// 活跃boss排名奖励屏蔽
	/*const WearyCfg *weary_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetWearyCfg(Protocol::CSBossFamilyOperate::BOSS_FAMILY_BUY_ACTIVE_BOSS_WEARY);
	if (weary_cfg != nullptr)
	{
		std::map<int, ActiveBossInfo>::iterator iter = m_activeboss_info_map.find(scene_id);
		if (m_activeboss_info_map.end() != iter)
		{
			std::map<int, ActiveBossHurtRankInfo>::iterator hurt_rank_info_it = iter->second.hurt_rank_info_map.find(monster->GetMonsterId());

			if (hurt_rank_info_it != iter->second.hurt_rank_info_map.end())
			{
				ActiveBossHurtRankInfo &hurt_rank_info = hurt_rank_info_it->second;
				int rank = 0;
				for (std::vector<BossHurtInfo>::iterator hurt_info_it = hurt_rank_info.hurt_info_list.begin(); hurt_info_it != hurt_rank_info.hurt_info_list.end(); ++hurt_info_it)
				{
					++rank;
					const BossFamilyBossHurtRankCfg::RankItem *rank_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossHurtRankCfg(monster->GetMonsterId(), rank);
					if (rank_cfg != nullptr)
					{
						Role *temp_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(hurt_info_it->uid));
						if (temp_role != nullptr)
						{
							CommonDataParam *common_data = temp_role->GetCommonDataParam();
							if (common_data->active_boss_weary + weary_cfg->kill_add_weary <= weary_cfg->weary_limit)
							{
								common_data->active_boss_weary += weary_cfg->kill_add_weary;
								BossFamilyManager::Instance().SendActiveBossInfo(role);

								int prof = temp_role->GetProf();
								if (prof > 0 && prof < PROF_TYPE_PROF_NO_LIMIT)
								{
									static MailContentParam mail_param; mail_param.Reset();

									SNPRINTF(mail_param.contenttxt, sizeof(mail_param.contenttxt), gamestring::g_active_boss_rank_reward_mail_context, monster->GetMonsterId(),rank);

									for (int j = 0; j < rank_cfg->item_count_list[prof] && j < MAX_ATTACHMENT_ITEM_NUM; ++j)
									{
										mail_param.AddItem(rank_cfg->item_list[prof][j]);
									}

									MailRoute::Instance().MailToRole(temp_role, SYSTEM_MAIL_REASON_INVALID, mail_param);
								}
							}
						}
					}
				}
			}
		}
	}*/

}

void BossFamilyManager::OnMikuMonsterBeKill(Monster *monster, Role *role)
{
	//if (NULL == role || NULL == monster) return;

	//int scene_id = role->GetScene()->GetSceneID();
	//if (!role->GetScene()->IsBossDongKuScene(scene_id))
	//{
	//	return;
	//}

	//std::map<int, MikuMonsterInfo>::iterator iter = m_miku_monster_info_map.find(scene_id);
	//if (m_miku_monster_info_map.end() != iter)
	//{
	//	std::map<int, MonsterInfo>::iterator miku_monster_it = iter->second.monster_info_map.find(monster->GetMonsterId());
	//	if (iter->second.monster_info_map.end() != miku_monster_it)
	//	{
	//		MonsterInfo & miku_monster = miku_monster_it->second;
	//		miku_monster.monster_status = ENBOSSSTATUS_NONEXIST;
	//		--m_miku_monster_num[miku_monster.scene_id];
	//		this->SendMikuMonsterInfoToAll(scene_id);

	//		unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	//		unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(static_cast<time_t>(now_sec)));
	//		unsigned today_sec = now_sec - today_zero_time;

	//		if (today_sec < static_cast<unsigned int>(miku_monster.begin_refresh_time))
	//		{
	//			miku_monster.next_refresh_time = today_zero_time + miku_monster.begin_refresh_time;
	//		}
	//		else if (today_sec > static_cast<unsigned int>(miku_monster.end_refresh_time))
	//		{
	//			int next_day_refresh_interval = EngineAdapter::Instance().NextDayInterval(0, 0, 0) + miku_monster.begin_refresh_time;
	//			miku_monster.next_refresh_time = now_sec + next_day_refresh_interval;
	//		}
	//		else
	//		{
	//			if (miku_monster.refresh_interval > 0)
	//			{
	//				miku_monster.next_refresh_time = now_sec + (miku_monster.refresh_interval - (today_sec - miku_monster.begin_refresh_time) % miku_monster.refresh_interval);
	//			}
	//		}
	//	}
	//}
}

void BossFamilyManager::OnBabyMonsterDie(Monster *monster, Role *role)
{
	if (monster == nullptr || role == nullptr) return;

	unsigned int now_time = (unsigned int)EngineAdapter::Instance().Time();
	std::map<int, BabyBossInfo>::iterator iter = m_babyboss_info_map.find(monster->GetScene()->GetSceneID());
	if (m_babyboss_info_map.end() != iter)
	{
		std::map<int, BossInfo>::iterator baby_boss_it = iter->second.boss_info_map.find(monster->GetMonsterId());
		if (iter->second.boss_info_map.end() != baby_boss_it)
		{
			BossInfo & baby_boss = baby_boss_it->second;
			baby_boss.boss_status = ENBOSSSTATUS_NONEXIST;
			//baby_boss.next_refresh_time = now_time + LOGIC_CONFIG->GetBabyBossConfig().GetBabyBossNextFlushTimeInterval(now_time);
			// 修改为击杀后倒计时刷新
			const BabyBossCfg *boss_cfg = LOGIC_CONFIG->GetBabyBossConfig().GetBabyBossCfgByBossId(baby_boss.boss_id);
			if (nullptr != boss_cfg && 1 == boss_cfg->is_boss)
			{
				// 根据平台导入玩家数调整刷新间隔
				int interval_s = WorldStatus::Instance().GetRefreshTimeInterval(boss_cfg->refresh_interval);
				baby_boss.next_refresh_time = now_time + interval_s;
			}
			else
			{
				baby_boss.next_refresh_time = now_time + LOGIC_CONFIG->GetBabyBossConfig().GetBabyBossNextFlushTimeInterval(now_time);
			}

			this->SendBossInfoToAllGateway(baby_boss, Protocol::BOSS_ENTER_TYPE_BOSS_BABY, Protocol::NOTIFY_REASON_BOSS_DIE, role->GetUID());
		}
	}
}

void BossFamilyManager::OnPreciousMonsterBeKill(Monster *monster, Role *role)
{
	if (NULL == monster)
	{
		return;
	}

	unsigned short monster_obj_id = monster->GetId();

	//给奖励
	this->PreciousBossRewardHelper(role, monster->GetMonsterId());

	int pos_type = -1;
	int task_type = -1;
	std::map<ObjID, MonsterInfo>::iterator monster_iter = m_precious_monster_map.find(monster_obj_id);
	if (monster_iter != m_precious_monster_map.end())
	{
		if (MonsterInitParam::MONSTER_TYPE_NORMAL == monster->GetMonsterType())
		{
			if (monster_iter->second.is_great_monster)
			{
				task_type = PreciousBossParam::PRECIOUS_BOSS_TASK_TYPE_GREAT_MONSTER;
				pos_type = PreciousBossRefreshPosCfg::PRECIOUS_BOSS_POS_TYPE_GREAT_MONSTER;
			}
			else
			{
				task_type = PreciousBossParam::PRECIOUS_BOSS_TASK_TYPE_COMMON_MONSTER;
				pos_type = PreciousBossRefreshPosCfg::PRECIOUS_BOSS_POS_TYPE_COMMON_MONSTER;
			}
			this->PreciousBossTaskHelper(role, task_type);
		}
	}
	else
	{
		pos_type = PreciousBossRefreshPosCfg::PRECIOUS_BOSS_POS_TYPE_BOSS;
		task_type = PreciousBossParam::PRECIOUS_BOSS_TASK_TYPE_BOSS;
		this->PreciousBossTaskHelper(role, task_type);
	}

	int monster_id = monster->GetMonsterId();
	if (m_precious_boss_info_map.find(monster_id) != m_precious_boss_info_map.end())
	{
		//boss击杀信息
		std::map<int, BossInfo>::iterator iter = m_precious_boss_info_map.find(monster_id);
		if (m_precious_boss_info_map.end() != iter && ENBOSSSTATUS_NONEXIST == iter->second.boss_status)
		{
			BossInfo & boss_info = iter->second;
			unsigned int min_kill_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			int killer_index = -1;
			for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
			{
				if (0 == boss_info.killer_info_list[i].killer_uid)
				{
					boss_info.killer_info_list[i].killer_uid = role->GetUID();
					boss_info.killer_info_list[i].killier_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
					F_STRNCPY(boss_info.killer_info_list[i].killer_name, role->GetName(), sizeof(GameName));
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

			if (0 <= killer_index && killer_index < KILLER_LIST_MAX_COUNT)
			{
				boss_info.killer_info_list[killer_index].killer_uid = role->GetUID();
				boss_info.killer_info_list[killer_index].killier_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				F_STRNCPY(boss_info.killer_info_list[killer_index].killer_name, role->GetName(), sizeof(GameName));
			}

			this->AddPreciousBossPos(pos_type, boss_info.born_pos);
		}
		this->SendPreciousBossInfoToAll();
	}
	else if (m_precious_monster_map.find(monster_obj_id) != m_precious_monster_map.end())
	{
		//下次刷新时间
		MonsterInfo &monster_info = m_precious_monster_map[monster_obj_id];
		if (ENBOSSSTATUS_EXIST == monster_info.monster_status && 0 == monster_info.next_refresh_time)
		{
			monster_info.monster_status = ENBOSSSTATUS_NONEXIST;
			monster_info.next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time())
				+ monster_info.refresh_interval;

			this->AddPreciousBossPos(pos_type, monster_info.born_pos);
			m_refresh_precious_monster_vec.push_back(monster_info);
		}
		m_precious_monster_map.erase(monster_obj_id);
	}
}

void BossFamilyManager::OnAttack(Role *attacker, Monster *monster, long long &delta_hp)
{
	if (NULL == attacker || NULL == monster) return;

	int scene_id = monster->GetScene()->GetSceneID();
	CommonDataParam *common_p = attacker->GetCommonDataParam();
	if (NULL == common_p)
	{
		return;
	}
	const WearyCfg *weary_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetWearyCfg(Protocol::CSBossFamilyOperate::BOSS_FAMILY_BUY_MIKU_WEARY);
	if (weary_cfg == NULL)
	{
		return;
	}

	if (attacker->GetScene()->IsBossDongKuScene(scene_id))    //精英（洞窟）boss疲劳满攻击Boss无伤害
	{
		long long temp_scene_id = scene_id;
		ObjID monster_obj_id = monster->GetId();
		long long monster_set_id = (long long)((temp_scene_id << 32) + monster_obj_id);
		if (m_mikuboss_set.end() != m_mikuboss_set.find(monster_set_id))
		{
			// 和平层不检测疲劳
			bool is_not_peace = true;
			const BossFamilyMikuBossInfoCfg * miku_boss_info_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetMikuBossInfo(monster->GetMonsterId());
			if (nullptr != miku_boss_info_cfg)
			{
				is_not_peace = (1 != miku_boss_info_cfg->is_peace);
			}

			if (is_not_peace && common_p->miku_boss_weary >= weary_cfg->weary_limit)
			{
				attacker->NoticeNum(errornum::EN_BOSS_DONGKU_WEARY_LIMIT);
				delta_hp = 0;
			}

			if (delta_hp < 0)
			{
				std::map<int, MikuBossInfo>::iterator iter = m_mikuboss_info_map.find(scene_id);
				if (m_mikuboss_info_map.end() != iter)
				{
					{
						int attacker_uid = attacker->GetUID();
						std::map<int, ActiveBossHurtRankInfo>::iterator hurt_rank_info_it = iter->second.hurt_rank_info_map.find(monster->GetMonsterId());
						if (hurt_rank_info_it != iter->second.hurt_rank_info_map.end())
						{
							bool is_find = false;
							ActiveBossHurtRankInfo &rank_info = hurt_rank_info_it->second;
							rank_info.rank_data_dirty_flag = 1;
							for (std::vector<BossHurtInfo>::iterator hurt_info_it = rank_info.hurt_info_list.begin(); hurt_info_it != rank_info.hurt_info_list.end(); ++hurt_info_it)
							{
								if (hurt_info_it->uid == attacker_uid)
								{
									hurt_info_it->hurt += -delta_hp;
									if (hurt_info_it->hurt < 0)
									{
										hurt_info_it->hurt = INT_MAX;
									}

									is_find = true;
									break;
								}
							}

							if (!is_find)
							{
								BossHurtInfo hurt_info;
								hurt_info.uid = attacker_uid;
								attacker->GetName(hurt_info.name);
								hurt_info.hurt = -delta_hp;

								rank_info.hurt_info_list.emplace_back(hurt_info);
							}
						}
					}
				}

				attacker->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_CAUSE_HARM_ON_BOSS, -delta_hp, COND_MIKU_BOSS);
				attacker->GetRoleGoal()->CheckCond(COND_TYPE_CAUSE_HARM_ON_BOSS, -delta_hp, COND_MIKU_BOSS);
			}
		}
	}
	else if (attacker->GetScene()->IsDabaoBossScene(scene_id))
	{
		if (delta_hp < 0)
		{
			attacker->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_CAUSE_HARM_ON_BOSS, -delta_hp, COND_DABAO_BOSS);
			attacker->GetRoleGoal()->CheckCond(COND_TYPE_CAUSE_HARM_ON_BOSS, -delta_hp, COND_DABAO_BOSS);
		}
	}
	else if (attacker->GetScene()->IsActiveBossScene(scene_id))   //活跃boss疲劳满攻击Boss无伤害
	{
		this->OnAttackActiveBoss(attacker, monster, delta_hp);
// 		const WearyCfg *active_boss_weary_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetWearyCfg(Protocol::CSBossFamilyOperate::BOSS_FAMILY_BUY_ACTIVE_BOSS_WEARY);
// 		if (active_boss_weary_cfg == NULL)
// 		{
// 			return;
// 		}
// 
// 		if (common_p->active_boss_weary >= active_boss_weary_cfg->weary_limit)
// 		{
// 			attacker->NoticeNum(errornum::EN_BOSS_DONGKU_WEARY_LIMIT);
// 			delta_hp = 0;
// 		}
// 
// 		if (delta_hp < 0)
// 		{
// 			attacker->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_CAUSE_HARM_ON_BOSS, -delta_hp, COND_ACTIVE_BOSS);
// 			attacker->GetRoleGoal()->CheckCond(COND_TYPE_CAUSE_HARM_ON_BOSS, -delta_hp, COND_ACTIVE_BOSS);
// 		}
	}
	else if (attacker->GetScene()->IsBossFamilyScene(scene_id))
	{
		if (delta_hp < 0)
		{
			attacker->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_CAUSE_HARM_ON_BOSS, -delta_hp, COND_FAMILY_BOSS);
			attacker->GetRoleGoal()->CheckCond(COND_TYPE_CAUSE_HARM_ON_BOSS, -delta_hp, COND_FAMILY_BOSS);
		}
	}
}

bool BossFamilyManager::CanFlyByShoe(Role *role, int target_scene_id)
{
	if (Scene::IsBossFamilyScene(target_scene_id))
	{
		role->NoticeNum(errornum::EN_NOT_FLY_BY_SHOE_BOSS_FAMILY_LIMIT);
		return false;
	}

	if (Scene::IsDabaoBossScene(target_scene_id))
	{
		role->NoticeNum(errornum::EN_NOT_FLY_BY_SHOE_DABAO_LIMIT);
		return false;
	}

	if (Scene::IsActiveBossScene(target_scene_id))
	{
		role->NoticeNum(errornum::EN_NOT_FLY_BY_SHOE_ACTIVE_LIMIT);
		return false;
	}

	if (Scene::IsBossDongKuScene(target_scene_id))
	{
		role->NoticeNum(errornum::EN_NOT_FLY_BY_SHOE_BOSS_DONGKU_LIMIT);
		return false;
	}

	return true;
}

void BossFamilyManager::OnEnterBossFamily(Role *role, Protocol::CSEnterBossFamily *msg)
{
	if (NULL == role || NULL == msg) return;

	if (Protocol::BOSS_ENTER_TYPE_IVNALID >= msg->enter_type || Protocol::BOSS_ENTER_TYPE_MAX <= msg->enter_type) return;

	if (Protocol::BOSS_ENTER_LEAVE_BOSS_SCENE == msg->enter_type)   //离开场景
	{
		if (!Scene::IsBossFamilyScene(role->GetScene()->GetSceneID()) && !Scene::IsDabaoBossScene(role->GetScene()->GetSceneID()) 
			&& !Scene::IsBossDongKuScene(role->GetScene()->GetSceneID()) && !Scene::IsActiveBossScene(role->GetScene()->GetSceneID())
			&& !Scene::IsPerciousBossScene(role->GetScene()->GetSceneID()))
			return;

		const BossFamilyOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyOtherCfg();
		gamelog::g_log_debug.printf(LL_DEBUG, "BOSS_ENTER_LEAVE_BOSS_SCENE: %s[%d] [scene_id:%d]", role->GetName(), UidToInt(role->GetUserId()), other_cfg.leave_scene_id);
		World::GetInstWorld()->GetSceneManager()->GoTo(role, other_cfg.leave_scene_id, 0, other_cfg.leave_pos);
	}

	if (role->GetRoleStatusManager()->IsInFB())
	{
		role->NoticeNum(errornum::EN_CAN_NOT_ENTER_BOSS);
		return;
	}

	const BossFamilyEnterConditionCfg * enter_condition_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetEnterConditionInfo(role, msg->scene_id);
	if (NULL == enter_condition_cfg) return;

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(enter_condition_cfg->scene_id, 0);
	if (NULL == scene) return;

	if (msg->scene_id == role->GetScene()->GetSceneID()) return;   // 已经在此场景

	{// 检查双人坐骑的另一半是否足够等级进入目标场景
		MultiMountBindItem *bind_info = role->GetMultiMount()->GetMountOnInfo();
		if (NULL != bind_info && bind_info->follower_uid != role->GetUID())
		{
			Role *follower_role = role->GetScene()->GetRoleByUID(bind_info->follower_uid);
			if (NULL != follower_role && !SceneRegister::Instance().CheckSceneLevelLimit(follower_role, msg->scene_id))
			{
				follower_role->NoticeNum(errornum::EN_SCENE_TRANSPORT_LEVEL_LIMIT);
				role->NoticeNum(errornum::EN_FOLLOWER_LEVEL_NOT_ENOUGH);
				return;
			}
		}
	}

	if (!SceneRegister::Instance().CheckSceneLevelLimit(role, msg->scene_id))
	{
		role->NoticeNum(errornum::EN_SCENE_TRANSPORT_LEVEL_LIMIT);
		return;
	}

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack == nullptr) return;

	int use_gold = 0;
	int consum_item_id = 0;
	int consum_item_num = 0;
	if (enter_condition_cfg->free_vip_level == 0 || role->GetVip()->GetVipLevel() < enter_condition_cfg->free_vip_level)
	{
		use_gold = enter_condition_cfg->cost_gold;
		consum_item_id = enter_condition_cfg->need_item_id;
		consum_item_num = enter_condition_cfg->need_item_num;
	}

	bool consum_succ = false;
	switch (msg->enter_type)
	{
	case Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY:
	{
		if (!Scene::IsBossFamilyScene(msg->scene_id)) return;

		if (use_gold > 0 && !knapsack->GetMoney()->GoldMoreThan(use_gold))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
		if (use_gold > 0 && knapsack->GetMoney()->UseGold(use_gold, "EnterBossFamily")) 
		{
			consum_succ = true;
		}

		if (!consum_succ && consum_item_id > 0 && !role->GetKnapsack()->ConsumeItemOrUseGold(consum_item_id, consum_item_num, "EnterBossFamily", true))
			return;

		this->SendFamilyBossInfo(role, msg->scene_id);
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_MIKU:
	{
		if (!Scene::IsBossDongKuScene(msg->scene_id)) return;
		if (use_gold > 0 && !knapsack->GetMoney()->GoldMoreThan(use_gold))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
		if (use_gold > 0 && knapsack->GetMoney()->UseAllGold(use_gold, "EnterBossMiKu"))
		{
			consum_succ = true;
		}

		if (!consum_succ && consum_item_id > 0 && !role->GetKnapsack()->ConsumeItemOrUseGold(consum_item_id, consum_item_num, "EnterBossMiKu"))
			return;

		this->SendMikuBossInfo(role, msg->scene_id);
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_DABAO:
	{
		if (!Scene::IsDabaoBossScene(msg->scene_id)) return;

		const int enter_count = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_DABAO_ENTER_COUNT);
		const BossFamilyOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyOtherCfg();
		const DabaoBuyCostCfg *cost_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetDabaoBuyCostCfg(enter_count);
		if (nullptr == cost_cfg)
		{
			role->NoticeNum(errornum::EN_CONFIG_ERROR);
			return;
		}

		int can_enter_count = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_DABAO_BOSS_ENTER_TIMES);
		if (enter_count >= can_enter_count)
		{
			role->NoticeNum(errornum::EN_ENTER_DABAO_MAP_COUNT_LIMIT);
			return;
		}

		bool is_free = false;
		if (enter_count < other_cfg.dabao_free_times)
		{
			is_free = true;
		}

		if (!is_free)
		{
			if (cost_cfg->cost_gold > 0 && !knapsack->GetMoney()->GoldMoreThan(cost_cfg->cost_gold))
			{
				role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}
			if (cost_cfg->cost_gold > 0 && knapsack->GetMoney()->UseAllGold(cost_cfg->cost_gold, "EnterDabaoBoss"))
			{
				consum_succ = true;
			}
			if (!consum_succ && cost_cfg->consume_item_id > 0 && !role->GetKnapsack()->ConsumeItemOrUseGold(cost_cfg->consume_item_id, cost_cfg->consume_item_num, "EnterDabaoBoss", true, true))
				return;
		}

		CommonDataParam *common_p = role->GetCommonDataParam();
		if (common_p == NULL) return;

		common_p->dabao_angry_value = 0;
		common_p->dabao_kick_time = 0;
		common_p->dabao_next_add_angry_timestamp = 0;

		role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_DABAO_ENTER_COUNT);
		this->SendDabaoBossInfo(role);
		this->SendDabaoBossNextFlushInfo(role, msg->scene_id);
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_ACTIVE:
	{
		if (!Scene::IsActiveBossScene(msg->scene_id)) return;

		if (use_gold > 0 && !knapsack->GetMoney()->GoldMoreThan(use_gold))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
		if (use_gold > 0 && knapsack->GetMoney()->UseAllGold(use_gold, "EnterBossActive"))
		{
			consum_succ = true;
		}

		if (!consum_succ && consum_item_id > 0 && !role->GetKnapsack()->ConsumeItemOrUseGold(consum_item_id, consum_item_num, "EnterBossActive"))
			return;

		role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_ACTIVE_ENTER_COUNT);
		this->SendActiveBossInfo(role);
		this->RefreshActiveBossInfo(msg->scene_id);
		this->SendActiveBossNextFlushInfo(role, msg->scene_id);
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_PRECIOUS:
	{
		const PreciousBossOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousBossOtherCfg();
		if (other_cfg.precious_boss_limit_level > role->GetLevel())
		{
			role->NoticeNum(errornum::EN_SCENE_TRANSPORT_LEVEL_LIMIT);
			return;
		}
	}
	break;

	default:
		return;
	}

	World::GetInstWorld()->GetSceneManager()->GoTo(role, enter_condition_cfg->scene_id, 0, enter_condition_cfg->enter_pos);
}

void BossFamilyManager::OnGetBossInfoReq(Role *role, int enter_type)
{
	if (NULL == role)
	{
		return;
	}

	switch (enter_type)
	{
	case Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY:
	{
		this->OnFamilyBossInfoReq(role);
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_MIKU:
	{
		this->OnMikuBossInfoReq(role);
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_DABAO:
	{
		this->OnDabaoBossInfoReq(role);
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_ACTIVE:
	{
		this->OnActiveBossInfoReq(role);
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_PRECIOUS:
	{
		this->SendPreciousBossInfo(role);
	}
	break;

	default:
		return;
	}
}

void BossFamilyManager::OnFamilyBossInfoReq(Role *role)
{
	if (NULL == role) return;

	const std::set<int> & family_boss_scene_id_set = LOGIC_CONFIG->GetBossFaimlyConfig().GetFamilyBossSceneIDSet();
	std::set<int>::const_iterator it = family_boss_scene_id_set.begin();
	for (; it != family_boss_scene_id_set.end(); ++it)
	{
		if (role->GetScene()->IsBossFamilyScene(*it))
		{
			this->SendFamilyBossInfo(role, *it);
		}
	}
}

void BossFamilyManager::OnMikuBossInfoReq(Role *role)
{
	if (NULL == role) return;

	const std::set<int> & miku_boss_scene_id_set = LOGIC_CONFIG->GetBossFaimlyConfig().GetMikuBossSceneIDSet();
	std::set<int>::const_iterator it = miku_boss_scene_id_set.begin();
	for (; it != miku_boss_scene_id_set.end(); ++it)
	{
		if (role->GetScene()->IsBossDongKuScene(*it))
		{
			this->SendMikuBossInfo(role, *it);
		}
	}
}

void BossFamilyManager::OnDabaoBossInfoReq(Role *role)
{
	if (NULL == role) return;

	this->SendDabaoBossInfo(role);
	const std::set<int> & dabao_boss_scene_id_set = LOGIC_CONFIG->GetBossFaimlyConfig().GetDaobaoBossSceneIDSet();
	std::set<int>::const_iterator it = dabao_boss_scene_id_set.begin();
	for (; it != dabao_boss_scene_id_set.end(); ++it)
	{
		if (role->GetScene()->IsDabaoBossScene(*it))
		{
			this->SendDabaoBossNextFlushInfo(role, *it);
		}
	}
}

void BossFamilyManager::OnActiveBossInfoReq(Role *role)
{
	if (NULL == role) return;

	this->SendActiveBossInfo(role);
	const std::set<int> & active_boss_scene_id_set = LOGIC_CONFIG->GetBossFaimlyConfig().GetActiveBossSceneIDSet();
	std::set<int>::const_iterator it = active_boss_scene_id_set.begin();
	for (; it != active_boss_scene_id_set.end(); ++it)
	{
		if (role->GetScene()->IsActiveBossScene(*it))
		{
			this->RefreshActiveBossInfo(*it);
			this->SendActiveBossNextFlushInfo(role, *it);
		}
	}
}

void BossFamilyManager::OnBossKillerInfoReq(Role *role, int boss_type, int boss_id, int scene_id)
{
	if (NULL == role) return;

	if (Protocol::BOSS_ENTER_TYPE_IVNALID >= boss_type || Protocol::BOSS_ENTER_TYPE_ALL_MAX <= boss_type)
	{
		return;
	}

	switch (boss_type)
	{
	case Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY:
	{
		if (!Scene::IsBossFamilyScene(scene_id))
			return;

		std::map<int, FamilyBossInfo>::iterator iter = m_familyboss_info_map.find(scene_id);
		if (m_familyboss_info_map.end() != iter)
		{
			std::map<int, BossInfo>::iterator family_boss = iter->second.boss_info_map.find(boss_id);
			if (iter->second.boss_info_map.end() != family_boss)
			{
				this->SendBossKillerList(role, family_boss->second);
			}
		}
		else if (m_familyboss_info_map.end() == iter)
		{
			iter = m_cross_familyboss_info_map.find(scene_id);
			if (iter != m_cross_familyboss_info_map.end())
			{
				std::map<int, BossInfo>::iterator family_boss = iter->second.boss_info_map.find(boss_id);
				if (iter->second.boss_info_map.end() != family_boss)
				{
					this->SendBossKillerList(role, family_boss->second);
				}
			}
		}
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_MIKU:
	{
		if (!Scene::IsBossDongKuScene(scene_id))
			return;

		std::map<int, MikuBossInfo>::iterator iter = m_mikuboss_info_map.find(scene_id);
		if (m_mikuboss_info_map.end() != iter)
		{
			std::map<int, BossInfo>::iterator miku_boss = iter->second.boss_info_map.find(boss_id);
			if (iter->second.boss_info_map.end() != miku_boss)
			{
				this->SendBossKillerList(role, miku_boss->second);
			}
		}
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_DABAO:
	{
		if (!Scene::IsDabaoBossScene(scene_id)) return;

		std::map<int, DabaoBossInfo>::iterator iter = m_dabaoboss_info_map.find(scene_id);
		if (m_dabaoboss_info_map.end() != iter)
		{
			std::map<int, BossInfo>::iterator dabao_boss = iter->second.boss_info_map.find(boss_id);
			if (iter->second.boss_info_map.end() != dabao_boss)
			{
				this->SendBossKillerList(role, dabao_boss->second);
			}
		}
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_ACTIVE:
	{
		if (!Scene::IsActiveBossScene(scene_id))
			return;

		std::map<int, ActiveBossInfo>::iterator iter = m_activeboss_info_map.find(scene_id);
		if (m_activeboss_info_map.end() != iter)
		{
			std::map<int, BossInfo>::iterator active_boss = iter->second.boss_info_map.find(boss_id);
			if (iter->second.boss_info_map.end() != active_boss)
			{
				this->SendBossKillerList(role, active_boss->second);
			}
		}
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_PRECIOUS:
	{
		if (!Scene::IsPerciousBossScene(scene_id))
			return;

		std::map<int, BossInfo>::iterator iter = m_precious_boss_info_map.find(boss_id);
		if (iter != m_precious_boss_info_map.end())
		{
			this->SendBossKillerList(role, iter->second);
		}
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_BABY:
	{
		this->SendBabyBossKillerInfo(role, boss_id);
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_SHANGGU:
	{
		BossShangGuManager::Instance().SendShangGuKillerInfo(role, boss_id);
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_GODMAGIC_BOSS:
	{
		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
		if (scene != NULL)
		{
			SpecialGodmagicBoss * godmagic_special = dynamic_cast<SpecialGodmagicBoss*>(scene->GetSpecialLogic());
			if (NULL != godmagic_special)
			{
				godmagic_special->SendBossKillerRecordInfo(role, boss_id);
			}
		}
	}
	break;

	default:
		return;
	}
}

void BossFamilyManager::OnFollowBossReq(Role *role, int boss_type, int boss_id, int scene_id)
{
	if (NULL == role) return;

	if (Protocol::BOSS_ENTER_TYPE_IVNALID >= boss_type || Protocol::BOSS_ENTER_TYPE_ALL_MAX <= boss_type)
	{
		return;
	}

	bool is_boss = false;
	switch (boss_type)
	{
	case Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY:
	{
		if (!Scene::IsBossFamilyScene(scene_id))
			return;

		std::map<int, FamilyBossInfo>::iterator iter = m_familyboss_info_map.find(scene_id);
		if (m_familyboss_info_map.end() != iter)
		{
			std::map<int, BossInfo>::iterator family_boss = iter->second.boss_info_map.find(boss_id);
			if (iter->second.boss_info_map.end() != family_boss)
			{
				is_boss = true;
			}
		}
		else if (m_familyboss_info_map.end() == iter)
		{
			iter = m_cross_familyboss_info_map.find(scene_id);
			if (iter != m_cross_familyboss_info_map.end())
			{
				std::map<int, BossInfo>::iterator family_boss = iter->second.boss_info_map.find(boss_id);
				if (iter->second.boss_info_map.end() != family_boss)
				{
					is_boss = true;
				}
			}
		}
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_MIKU:
	{
		if (!Scene::IsBossDongKuScene(scene_id))
			return;

		std::map<int, MikuBossInfo>::iterator iter = m_mikuboss_info_map.find(scene_id);
		if (m_mikuboss_info_map.end() != iter)
		{
			std::map<int, BossInfo>::iterator miku_boss = iter->second.boss_info_map.find(boss_id);
			if (iter->second.boss_info_map.end() != miku_boss)
			{
				is_boss = true;
			}
		}
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_DABAO:
	{
		if (!Scene::IsDabaoBossScene(scene_id)) return;

		DabaoBossMapIt itr = m_dabaoboss_info_map.find(scene_id);
		if (m_dabaoboss_info_map.end() != itr)
		{
			std::map<int, BossInfo>::iterator dabao_boss = itr->second.boss_info_map.find(boss_id);
			if (itr->second.boss_info_map.end() != dabao_boss)
			{
				is_boss = true;
			}
		}
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_ACTIVE:
	{
		if (!Scene::IsActiveBossScene(scene_id))
			return;
		ActiveBossMapIt itr = m_activeboss_info_map.find(scene_id);
		if (m_activeboss_info_map.end() != itr)
		{
			std::map<int, BossInfo>::iterator act_boss = itr->second.boss_info_map.find(boss_id);
			if (itr->second.boss_info_map.end() != act_boss)
			{
				is_boss = true;
			}
		}
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_BOSS_BABY:
	{
		if (LOGIC_CONFIG->GetBabyBossConfig().IsBabyBoss(boss_id))
		{
			is_boss = true;
		}
	}
	break;

	case Protocol::BOSS_TYPE_CROSS_BOSS:
	{
		auto boss_cfg = CrossBossConfig::Instance().GetCrossBossBossCfgByMonsterId(scene_id, boss_id);
		if (boss_cfg)
		{
			is_boss = true;
		}
	}
	break;

	case Protocol::BOSS_TYPE_CROSS_MIZANG_BOSS:
	{
		auto boss_cfg = CrossMizangBossConfig::Instance().GetCrossBossBossCfgByMonsterId(scene_id, boss_id);
		if (boss_cfg)
		{
			is_boss = true;
		}
	}
	break;

	case Protocol::BOSS_ENTER_TYPE_GODMAGIC_BOSS:
	{
		auto boss_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossBossCfgByMonsterId(scene_id, boss_id);
		if (boss_cfg)
		{
			is_boss = true;
		}
	}
	break;

	default:
		return;
	}

	if (is_boss)
	{
		role->GetFollowBossManager()->FollowBoss(boss_type, boss_id, scene_id);
	}
}

void BossFamilyManager::OnUnFollowBossReq(Role *role, int boss_type, int boss_id, int scene_id)
{
	if (NULL == role || 0 >= boss_id) return;

	if (Protocol::BOSS_ENTER_TYPE_IVNALID >= boss_type || Protocol::BOSS_ENTER_TYPE_ALL_MAX <= boss_type)
	{
		return;
	}

	if (!Scene::IsBossFamilyScene(scene_id) && 
		!Scene::IsBossDongKuScene(scene_id) && 
		!Scene::IsDabaoBossScene(scene_id) && 
		!Scene::IsActiveBossScene(scene_id) && 
		!LOGIC_CONFIG->GetBabyBossConfig().IsBabyBoss(boss_id) && 
		!CrossBossConfig::Instance().GetCrossBossBossCfgByMonsterId(scene_id, boss_id) && 
		!CrossMizangBossConfig::Instance().GetCrossBossBossCfgByMonsterId(scene_id, boss_id)&&
		!LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossBossCfgByMonsterId(scene_id, boss_id))
	{
		return;
	}

	role->GetFollowBossManager()->UnFollowBoss(boss_type, boss_id, scene_id);
}

void BossFamilyManager::SendDabaoBossInfo(Role *role)
{
	if (NULL == role) return;

	static Protocol::SCDabaoBossInfo gfi;
	CommonDataParam *common_p = role->GetCommonDataParam();
	if (NULL == common_p) return;

	gfi.dabao_angry_value = common_p->dabao_angry_value;
	gfi.enter_count = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_DABAO_ENTER_COUNT);

	// 登录时下发信息会不断重置踢出时间
// 	const BossFamilyOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyOtherCfg();
// 	if (common_p->dabao_angry_value >= other_cfg.max_value)
// 	{
// 		common_p->dabao_kick_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + m_dabao_kick_delay_time;
// 	}

	gfi.kick_time = common_p->dabao_kick_time;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gfi, sizeof(gfi));
}

void BossFamilyManager::SendActiveBossInfo(Role *role)
{
	if (NULL == role) return;

	static Protocol::SCActiveBossInfo gfi;
	CommonDataParam *common_p = role->GetCommonDataParam();
	if (NULL == common_p) return;

	gfi.enter_count = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_ACTIVE_ENTER_COUNT);

	//const BossFamilyOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyOtherCfg();

	gfi.active_boss_weary = common_p->active_boss_weary;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gfi, sizeof(gfi));
}

void BossFamilyManager::SendFamilyBossInfo(Role *role, int scene_id)
{
	if (NULL == role) return;

	static Protocol::SCFamilyBossInfo gfi;
	gfi.count = 0;

	if (0 == scene_id)
	{
		scene_id = role->GetScene()->GetSceneID();
	}

	gfi.scene_id = scene_id;

	std::map<int, FamilyBossInfo>::iterator iter = m_familyboss_info_map.find(scene_id);

	if (m_familyboss_info_map.end() != iter)
	{
		std::map<int, BossInfo>::iterator family_boss = iter->second.boss_info_map.begin();
		for (; gfi.count < Protocol::SCFamilyBossInfo::MAX_BOSS_COUNT && iter->second.boss_info_map.end() != family_boss; family_boss++)
		{
			gfi.boss_list[gfi.count].boss_id = family_boss->second.boss_id;
			gfi.boss_list[gfi.count].status = family_boss->second.boss_status;
			gfi.boss_list[gfi.count].next_refresh_time = family_boss->second.next_refresh_time;

			++gfi.count;
		}
	}
	else if (m_familyboss_info_map.end() == iter)
	{
		auto cross_iter = m_cross_familyboss_info_map.find(scene_id);
		if (m_cross_familyboss_info_map.end() != cross_iter)
		{
			std::map<int, BossInfo>::iterator family_boss = cross_iter->second.boss_info_map.begin();
			for (; gfi.count < Protocol::SCFamilyBossInfo::MAX_BOSS_COUNT && cross_iter->second.boss_info_map.end() != family_boss; family_boss++)
			{
				gfi.boss_list[gfi.count].boss_id = family_boss->second.boss_id;
				gfi.boss_list[gfi.count].status = family_boss->second.boss_status;
				gfi.boss_list[gfi.count].next_refresh_time = family_boss->second.next_refresh_time;

				++gfi.count;
			}
		}
	}

	int sendlen = sizeof(gfi) - sizeof(gfi.boss_list[0]) * (Protocol::SCFamilyBossInfo::MAX_BOSS_COUNT - gfi.count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gfi, sendlen);
}

void BossFamilyManager::SendMikuBossInfo(Role *role, int scene_id)
{
	if (NULL == role) return;

	static Protocol::SCMikuBossInfo gfi;
	CommonDataParam *common_p = role->GetCommonDataParam();
	if (NULL == common_p) return;

	gfi.miku_boss_weary = common_p->miku_boss_weary;
	gfi.reserve = 0;
	gfi.count = 0;
	gfi.scene_id = role->GetScene()->GetSceneID();
	gfi.miku_monster_num = 0;

	std::map<int, MikuBossInfo>::iterator iter = m_mikuboss_info_map.find(role->GetScene()->GetSceneID());
	if (scene_id != 0)
	{
		iter = m_mikuboss_info_map.find(scene_id);
		gfi.scene_id = scene_id;
	}

	if (m_miku_monster_num.find(gfi.scene_id) != m_miku_monster_num.end())
	{
		gfi.miku_monster_num = m_miku_monster_num[gfi.scene_id];
	}

	if (m_mikuboss_info_map.end() != iter)
	{
		std::map<int, BossInfo>::iterator miku_boss = iter->second.boss_info_map.begin();
		for (; gfi.count < Protocol::SCMikuBossInfo::MAX_BOSS_COUNT && iter->second.boss_info_map.end() != miku_boss; miku_boss++)
		{
			gfi.boss_list[gfi.count].boss_id = miku_boss->second.boss_id;
			gfi.boss_list[gfi.count].status = miku_boss->second.boss_status;
			gfi.boss_list[gfi.count].next_refresh_time = miku_boss->second.next_refresh_time;
			++gfi.count;
		}
	}

	int sendlen = sizeof(gfi) - sizeof(gfi.boss_list[0]) * (Protocol::SCMikuBossInfo::MAX_BOSS_COUNT - gfi.count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gfi, sendlen);
}

void BossFamilyManager::SendMikuMonsterInfo(Role *role, int scene_id)
{
	//if (NULL == role) return;

	//static Protocol::SCMikuMonsterInfo gfi;
	//gfi.miku_monster_num = 0;

	//if (m_miku_monster_num.find(scene_id) != m_miku_monster_num.end())
	//{
	//	gfi.miku_monster_num = m_miku_monster_num[scene_id];
	//}

	//EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gfi, sizeof(Protocol::SCMikuMonsterInfo));
}

void BossFamilyManager::SendFamilyBossInfoToAll(int scene_id)
{
	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
	if (NULL == scene) return;

	for (int i = 0; i < (int)scene->RoleNum(); ++i)
	{
		Role *temp_role = scene->GetRoleByIndex(i);
		if (NULL == temp_role) continue;
		this->SendFamilyBossInfo(temp_role);
	}
}

void BossFamilyManager::SendMikuBossInfoToAll(int scene_id)
{
	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
	if (NULL == scene) return;

	for (int i = 0; i < (int)scene->RoleNum(); ++i)
	{
		Role *temp_role = scene->GetRoleByIndex(i);
		if (NULL == temp_role) continue;
		this->SendMikuBossInfo(temp_role);
	}
}

void BossFamilyManager::SendMikuMonsterInfoToAll(int scene_id)
{
	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
	if (NULL == scene) return;

	for (int i = 0; i < (int)scene->RoleNum(); ++i)
	{
		Role *temp_role = scene->GetRoleByIndex(i);
		if (NULL == temp_role) continue;
		this->SendMikuMonsterInfo(temp_role, scene_id);
	}
}

void BossFamilyManager::SendBossRoleInfo(Role *role)
{
	if (NULL == role) return;

	static Protocol::SCBossRoleInfo gfi;
	CommonDataParam *common_p = role->GetCommonDataParam();
	if (NULL == common_p) return;

	gfi.miku_boss_weary = common_p->miku_boss_weary;
	gfi.reserve = 0;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gfi, sizeof(gfi));
}

void BossFamilyManager::SendDabaoBossNextFlushInfo(Role *role, int scene_id)
{
	if (NULL == role)
	{
		return;
	}

	if (!role->GetScene()->IsDabaoBossScene(scene_id))
	{
		return;
	}

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
	if (NULL == scene)
	{
		return;
	}

	Protocol::SCDabaoBossNextFlushInfo gfi;
	gfi.scene_id = scene_id;
	gfi.count = 0;

	std::map<int, DabaoBossInfo>::iterator iter = m_dabaoboss_info_map.find(role->GetScene()->GetSceneID());
	if (scene_id != 0)
	{
		iter = m_dabaoboss_info_map.find(scene_id);
	}

	if (m_dabaoboss_info_map.end() != iter)
	{
		std::map<int, BossInfo>::iterator dabao_boss = iter->second.boss_info_map.begin();
		for (; gfi.count < Protocol::SCDabaoBossNextFlushInfo::MAX_BOSS_COUNT && iter->second.boss_info_map.end() != dabao_boss; dabao_boss++)
		{
			if (ENBOSSSTATUS_NONEXIST == dabao_boss->second.boss_status)
			{
				gfi.boss_list[gfi.count].boss_id = dabao_boss->second.boss_id;
				gfi.boss_list[gfi.count].next_refresh_time = dabao_boss->second.next_refresh_time;
				++gfi.count;
			}
		}
	}

	if (0 < gfi.count)
	{
		int sendlen = sizeof(gfi) - sizeof(gfi.boss_list[0]) * (Protocol::SCDabaoBossNextFlushInfo::MAX_BOSS_COUNT - gfi.count);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gfi, sendlen);
	}
}

void BossFamilyManager::SendDabaoBossNextFlushInfoToAll(int scene_id)
{
	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
	if (NULL == scene) return;

	for (int i = 0; i < (int)scene->RoleNum(); ++i)
	{
		Role *temp_role = scene->GetRoleByIndex(i);
		if (NULL == temp_role) continue;
		this->SendDabaoBossNextFlushInfo(temp_role, scene_id);
	}
}

void BossFamilyManager::SendActiveBossNextFlushInfo(Role *role, int scene_id)
{
	if (NULL == role)
	{
		return;
	}

	if (!role->GetScene()->IsActiveBossScene(scene_id))
	{
		return;
	}

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
	if (NULL == scene)
	{
		return;
	}

	Protocol::SCActiveBossNextFlushInfo gfi;
	gfi.scene_id = scene_id;
	gfi.count = 0;

	std::map<int, ActiveBossInfo>::iterator iter = m_activeboss_info_map.find(role->GetScene()->GetSceneID());
	if (scene_id != 0)
	{
		iter = m_activeboss_info_map.find(scene_id);
	}

	if (m_activeboss_info_map.end() != iter)
	{
		std::map<int, BossInfo>::iterator active_boss = iter->second.boss_info_map.begin();
		for (; gfi.count < Protocol::SCActiveBossNextFlushInfo::MAX_BOSS_COUNT && iter->second.boss_info_map.end() != active_boss; active_boss++)
		{
			if (ENBOSSSTATUS_NONEXIST == active_boss->second.boss_status)
			{
				gfi.boss_list[gfi.count].boss_id = active_boss->second.boss_id;
				gfi.boss_list[gfi.count].next_refresh_time = active_boss->second.next_refresh_time;
				++gfi.count;
			}
		}
	}

	if (0 < gfi.count)
	{
		int sendlen = sizeof(gfi) - sizeof(gfi.boss_list[0]) * (Protocol::SCActiveBossNextFlushInfo::MAX_BOSS_COUNT - gfi.count);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gfi, sendlen);
	}
}

void BossFamilyManager::SendActiveBossNextFlushInfoToAll(int scene_id)
{
	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
	if (NULL == scene) return;

	for (int i = 0; i < (int)scene->RoleNum(); ++i)
	{
		Role *temp_role = scene->GetRoleByIndex(i);
		if (NULL == temp_role) continue;
		this->SendActiveBossNextFlushInfo(temp_role, scene_id);
	}
}

void BossFamilyManager::SendBossInfoToAllGateway(BossInfo boss_info, int boss_type, int notify_reason, int killer_uid /* = 0 */)
{
	Protocol::SCBossInfoToAll bita;
	if (0 >= boss_info.boss_id)
	{
		return;
	}

	if (Protocol::BOSS_ENTER_TYPE_IVNALID >= boss_type || Protocol::BOSS_ENTER_TYPE_MAX <= boss_type)
	{
		return;
	}

	bita.notify_reason = notify_reason;
	bita.boss_type = boss_type;
	bita.boss_id = boss_info.boss_id;
	bita.scene_id = boss_info.scene_id;
	bita.status = boss_info.boss_status;
	bita.next_refresh_time = 0;
	if (ENBOSSSTATUS_NONEXIST == boss_info.boss_status)
	{
		bita.next_refresh_time = boss_info.next_refresh_time;
	}
	bita.killer_uid = killer_uid;

	if (Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY == boss_type && CrossConfig::Instance().IsHiddenServer())  // 跨服VIPboss需要进行所有服务器广播
	{
		InternalComm::Instance().UserProtocalTransferToGame(0, (const char *)&bita, sizeof(bita));
	}
	else
	{
		World::GetInstWorld()->SendToAllGatewayAndCrossUser((const char *)&bita, sizeof(bita));
	}
}


void BossFamilyManager::SendBossKillerList(Role *role, BossInfo boss_info)
{
	if (NULL == role || 0 >= boss_info.boss_id)
	{
		return;
	}

	if (!Scene::IsBossFamilyScene(boss_info.scene_id) && !Scene::IsBossDongKuScene(boss_info.scene_id) && !Scene::IsDabaoBossScene(boss_info.scene_id) && !Scene::IsActiveBossScene(boss_info.scene_id) && !Scene::IsPerciousBossScene(boss_info.scene_id))
	{
		return;
	}

	Protocol::SCBossKillerList bkl;
	for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
	{
		bkl.killer_info_list[i].killer_uid = boss_info.killer_info_list[i].killer_uid;
		bkl.killer_info_list[i].killier_time = boss_info.killer_info_list[i].killier_time;
		F_STRNCPY(bkl.killer_info_list[i].killer_name, boss_info.killer_info_list[i].killer_name, sizeof(GameName));
	}
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&bkl, sizeof(bkl));
}

void BossFamilyManager::SendPreciousBossInfoToAll()
{
	const PreciousBossOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousBossOtherCfg();
	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.precious_boss_scene_id, 0);
	if (NULL == scene) return;

	for (int i = 0; i < (int)scene->RoleNum(); ++i)
	{
		Role *temp_role = scene->GetRoleByIndex(i);
		if (NULL == temp_role) continue;
		this->SendPreciousBossInfo(temp_role);
	}
}

void BossFamilyManager::SendPreciousBossInfo(Role *role)
{
	static Protocol::SCPreciousBossInfo gfi;
	gfi.count = 0;
	std::map<int, BossInfo>::iterator iter = m_precious_boss_info_map.begin();
	for (; gfi.count < Protocol::SCPreciousBossInfo::MAX_BOSS_COUNT && m_precious_boss_info_map.end() != iter; ++iter)
	{
		gfi.boss_list[gfi.count].boss_id = iter->second.boss_id;
		gfi.boss_list[gfi.count].next_refresh_time = iter->second.next_refresh_time;
		++gfi.count;
	}

	if (0 < gfi.count)
	{
		int sendlen = sizeof(gfi) - sizeof(gfi.boss_list[0]) * (Protocol::SCPreciousBossInfo::MAX_BOSS_COUNT - gfi.count);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gfi, sendlen);
	}
}

void BossFamilyManager::SendPreciousBossPosInfo(Role *role, int param, int param_2)
{
	if (role == nullptr) return;

	static Protocol::SCPreciousPosInfo pro;
	Posi pos;
	if (param_2 > 0)
	{
		std::map<int, BossInfo>::iterator it = m_precious_boss_info_map.find(param_2);
		if (it != m_precious_boss_info_map.end())
		{
			pro.param = it->second.boss_id;
			pos = it->second.born_pos;
			pro.pos_x = pos.x;
			pro.pos_y = pos.y;
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pro, sizeof(pro));
		}
	}
	else
	{
		switch (param)
		{
		case PreciousBossParam::PRECIOUS_BOSS_TASK_TYPE_BOSS:
		{
			for (std::map<int, BossInfo>::iterator it = m_precious_boss_info_map.begin(); it != m_precious_boss_info_map.end(); it++)
			{
				if (ENBOSSSTATUS_EXIST == it->second.boss_status)
				{
					pro.param = it->second.boss_id;
					pos = it->second.born_pos;
					pro.pos_x = pos.x;
					pro.pos_y = pos.y;
					EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pro, sizeof(pro));
					break;
				}
			}
		}
		break;

		case PreciousBossParam::PRECIOUS_BOSS_TASK_TYPE_COMMON_MONSTER:
		{
			bool find = false;
			for (std::map<ObjID, MonsterInfo>::iterator it = m_precious_monster_map.begin(); it != m_precious_monster_map.end(); it++)
			{
				if (!find)
				{
					MonsterInfo &common_monster = it->second;
					if (ENBOSSSTATUS_EXIST == common_monster.monster_status && !common_monster.is_great_monster)
					{
						pro.param = common_monster.monster_id;
						pos = common_monster.born_pos;
						pro.pos_x = pos.x;
						pro.pos_y = pos.y;
						EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pro, sizeof(pro));
						find = true;
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
		break;

		case PreciousBossParam::PRECIOUS_BOSS_TASK_TYPE_GATHER:
		{
			for (std::vector<PreciousGatherInfo>::iterator iter = m_precious_gather_vec.begin(); iter != m_precious_gather_vec.end(); ++iter)
			{
				if (iter->is_gather == 1)
				{
					const PreciousBossOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousBossOtherCfg();
					pos = iter->pos;
					pro.param = other_cfg.gather_id;
					pro.pos_x = pos.x;
					pro.pos_y = pos.y;

					EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pro, sizeof(pro));
					break;
				}
			}
		}
		break;

		case PreciousBossParam::PRECIOUS_BOSS_TASK_TYPE_GREAT_MONSTER:
		{
			bool find = false;
			for (std::map<ObjID, MonsterInfo>::iterator it = m_precious_monster_map.begin(); it != m_precious_monster_map.end(); it++)
			{
				if (!find)
				{
					MonsterInfo &common_monster = it->second;
					if (ENBOSSSTATUS_EXIST == common_monster.monster_status && common_monster.is_great_monster)
					{
						pro.param = common_monster.monster_id;
						pos = common_monster.born_pos;
						pro.pos_x = pos.x;
						pro.pos_y = pos.y;
						EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pro, sizeof(pro));
						find = true;
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
		break;

		default:
			return;
		}
	}
}

void BossFamilyManager::OnBossFamilyOperate(Role *role, int operate_type)
{
	if (NULL == role) return;

	switch (operate_type)
	{
	case Protocol::CSBossFamilyOperate::BOSS_FAMILY_BUY_MIKU_WEARY:
	{
		CommonDataParam *common_data = role->GetCommonDataParam();
		if (NULL == common_data) return;

		const WearyCfg *weary_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetWearyCfg(Protocol::CSBossFamilyOperate::BOSS_FAMILY_BUY_MIKU_WEARY);
		if (weary_cfg == NULL)
		{
			return;
		}

		int buy_times = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_BUY_MIKU_WERARY);
		int vip_can_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_FB_MIKU_BOSS_BUY_WEARY);
		if (buy_times >= vip_can_buy_times)
		{
			role->NoticeNum(errornum::EN_NOT_FAMILY_BOSS_BUY_WEARY_TIME_LIMIT);
			return;
		}
		else
		{
			const WearyCostCfg *weary_cost_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBuyWearyCostCfg(buy_times);
			if (weary_cost_cfg == NULL)
			{
				role->NoticeNum(errornum::EN_NOT_FAMILY_BOSS_BUY_WEARY_TIME_LIMIT);
				return;
			}

			if (role->GetKnapsack()->IsLock())
			{
				role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
				return;
			}

			if (weary_cost_cfg->cost_tpye == 0)
			{
				if (!role->GetKnapsack()->GetMoney()->UseGold(weary_cost_cfg->cost, "BuyMikuWeary"))
				{
					role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}
			}
			else if (weary_cost_cfg->cost_tpye == 1)
			{
				if (!role->GetKnapsack()->GetMoney()->UseGoldBind(weary_cost_cfg->cost, "BuyMikuWeary"))
				{
					role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}
			}
			else if (weary_cost_cfg->cost_tpye == 2)
			{
				if (!role->GetKnapsack()->GetMoney()->UseAllGold(weary_cost_cfg->cost, "BuyMikuWeary"))
				{
					role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}
			}

			common_data->miku_boss_weary -= weary_cfg->buy_give_weary;
			role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_BUY_MIKU_WERARY);
			role->NoticeNum(noticenum::NT_ELEMENT_SHOP_BUY_SUCC);
			role->GetRoleActivity()->AddTurntableScore(weary_cost_cfg->cost);
			this->SendBossRoleInfo(role);
		}
	}
	break;

	case Protocol::CSBossFamilyOperate::BOSS_FAMILY_BUY_ACTIVE_BOSS_WEARY:
	{
		CommonDataParam *common_data = role->GetCommonDataParam();
		if (NULL == common_data) return;

		const WearyCfg *weary_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetWearyCfg(Protocol::CSBossFamilyOperate::BOSS_FAMILY_BUY_ACTIVE_BOSS_WEARY);
		if (weary_cfg == NULL)
		{
			return;
		}

		int buy_times = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_BUY_ACTIVE_BOSS_WERARY);
		int vip_can_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_FB_MIKU_BOSS_BUY_WEARY);
		if (buy_times >= vip_can_buy_times)
		{
			role->NoticeNum(errornum::EN_NOT_FAMILY_BOSS_BUY_WEARY_TIME_LIMIT);
			return;
		}
		else
		{
			const WearyCostCfg *weary_cost_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBuyWearyCostCfg(buy_times);
			if (weary_cost_cfg == NULL)
			{
				role->NoticeNum(errornum::EN_NOT_FAMILY_BOSS_BUY_WEARY_TIME_LIMIT);
				return;
			}

			if (role->GetKnapsack()->IsLock())
			{
				role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
				return;
			}

			if (weary_cost_cfg->cost_tpye == 0)
			{
				if (!role->GetKnapsack()->GetMoney()->UseGold(weary_cost_cfg->cost, "BuyActiveBossWeary"))
				{
					role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}
			}
			else if (weary_cost_cfg->cost_tpye == 1)
			{
				if (!role->GetKnapsack()->GetMoney()->UseGoldBind(weary_cost_cfg->cost, "BuyActiveBossWeary"))
				{
					role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}
			}
			else if (weary_cost_cfg->cost_tpye == 2)
			{
				if (!role->GetKnapsack()->GetMoney()->UseAllGold(weary_cost_cfg->cost, "BuyActiveBossWeary"))
				{
					role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}
			}

			role->DecActiveBossWeary(weary_cfg->buy_give_weary);
			role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_BUY_ACTIVE_BOSS_WERARY);
			role->NoticeNum(noticenum::NT_ELEMENT_SHOP_BUY_SUCC);
			role->GetRoleActivity()->AddTurntableScore(weary_cost_cfg->cost);
		}
	}
	break;

	case Protocol::CSBossFamilyOperate::BOSS_DABAO_BUY_ENTER_COUNT:
	{
		/*int can_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_DABAO_BOSS_ENTER_TIMES);
		int today_buy_count = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_DABAO_BOSS_BUY_COUNT);
		if (today_buy_count >= can_buy_times)
		{
			role->NoticeNum(errornum::EN_HUSONG_TASK_VIP_BUY_LIMIT);
			return;
		}

		int cost_gold = LOGIC_CONFIG->GetBossFaimlyConfig().GetDabaoBossBuyCountCost(today_buy_count);
		if (cost_gold <= 0 || !role->GetKnapsack()->GetMoney()->GoldMoreThan(cost_gold))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		if (cost_gold > 0)
		{
			if (!role->GetKnapsack()->GetMoney()->UseGold(cost_gold, "OnBossFamilyOperate-dabaoboss_buyentercount")) return;
			role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_DABAO_BOSS_BUY_COUNT);
			role->NoticeNum(noticenum::NT_ELEMENT_SHOP_BUY_SUCC);
			role->GetRoleActivity()->AddTurntableScore(cost_gold);
		}*/
	}
	break;

	default:
		break;
	}
}

void BossFamilyManager::SendPreciousBossTaskInfo(Role *role)
{
	static Protocol::SCPreciousBossTaskInfo task_info;
	 
	int count = 0;
	int task_count = 0;
	PreciousBossParam::TaskParam *task = role->GetRolePreciousBoss()->GetPreciousBossParamTaskList(task_count);
	for (int i = 0; i < task_count && count < Protocol::SCPreciousBossTaskInfo::MAX_TASK_COUNT; i++)
	{
		task_info.task_list[count].task_type = task[i].task_type;
		task_info.task_list[count].is_finish = task[i].is_finish;
		task_info.task_list[count].task_condition = task[i].task_condition;
		++count;
	}

	if (count > 0)
	{
		task_info.count = count;
		int sendlen = sizeof(task_info) - sizeof(task_info.task_list[0]) * (Protocol::SCPreciousBossTaskInfo::MAX_TASK_COUNT - count);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&task_info, sendlen);
	}
}

void BossFamilyManager::OnPreciousBossGather(Role *role, int gather_id, const Posi &gather_pos)
{
	if (NULL == role)
	{
		return;
	}

	const PreciousBossOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousBossOtherCfg();
	if (other_cfg.gather_id != gather_id)
	{
		return;
	}

	for (std::vector<PreciousGatherInfo>::iterator iter = m_precious_gather_vec.begin(); iter != m_precious_gather_vec.end(); ++iter)
	{
		if (iter->pos == gather_pos)
		{
			iter->is_gather = 0;
			iter->next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.gather_refresh_interval;
			break;
		}
	}
	this->PreciousBossTaskHelper(role, PreciousBossParam::PRECIOUS_BOSS_TASK_TYPE_GATHER);
	this->PreciousBossRewardHelper(role, gather_id);
	this->AddPreciousBossPos(PreciousBossRefreshPosCfg::PRECIOUS_BOSS_POS_TYPE_GATHER, gather_pos);
}

bool BossFamilyManager::IsInForbidPkArea(Role *role)
{
	const PreciousBossOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousBossOtherCfg();
	if (!(role->GetPos() - other_cfg.forbid_pk_pos).IsLongerThan(other_cfg.forbid_pk_radius))
	{
		role->NoticeNum(errornum::EN_PRECIOUS_BOSS_IN_SAVE_AREA);
		return true;
	}

	return false;
}

void BossFamilyManager::RefreshAllFamilyBoss()
{
	std::map<int, FamilyBossInfo>::iterator iter = m_familyboss_info_map.begin();
	for (; m_familyboss_info_map.end() != iter; ++iter)
	{
		std::map<int, BossInfo>::iterator family_boss = iter->second.boss_info_map.begin();
		for (; iter->second.boss_info_map.end() != family_boss; family_boss++)
		{
			if (ENBOSSSTATUS_NONEXIST == family_boss->second.boss_status)
			{
				this->RefreshOneFamilyBoss(family_boss->second);
			}
		}
	}

	if (CrossConfig::Instance().IsHiddenServer())
	{
		std::map<int, FamilyBossInfo>::iterator iter = m_cross_familyboss_info_map.begin();
		for (; m_cross_familyboss_info_map.end() != iter; ++iter)
		{
			std::map<int, BossInfo>::iterator family_boss = iter->second.boss_info_map.begin();
			for (; iter->second.boss_info_map.end() != family_boss; family_boss++)
			{
				if (ENBOSSSTATUS_NONEXIST == family_boss->second.boss_status)
				{
					this->RefreshOneFamilyBoss(family_boss->second);
				}
			}
		}
	}
}

void BossFamilyManager::RefreshAllMikuBoss()
{
	std::map<int, MikuBossInfo>::iterator iter = m_mikuboss_info_map.begin();
	for (; m_mikuboss_info_map.end() != iter; ++iter)
	{
		std::map<int, BossInfo>::iterator miku_boss = iter->second.boss_info_map.begin();
		for (; iter->second.boss_info_map.end() != miku_boss; miku_boss++)
		{
			if (ENBOSSSTATUS_NONEXIST == miku_boss->second.boss_status)
			{
				this->RefreshOneMikuBoss(miku_boss->second);
			}
		}
	}
}

void BossFamilyManager::RefreshOneFamilyBoss(BossInfo & family_boss)
{
	if (family_boss.boss_id < 0) return;

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(family_boss.scene_id, 0);
	if (NULL != scene)
	{
		Monster *new_monster = MONSTERPOOL->CreateMonster(family_boss.boss_id, scene, family_boss.born_pos);
		if (NULL != new_monster)
		{
			family_boss.boss_status = ENBOSSSTATUS_EXIST;
			family_boss.next_refresh_time = 0;
			long long scene_id = family_boss.scene_id;
			ObjID monster_obj_id = new_monster->GetId();
			long long monster_set_id = ((scene_id << 32) + monster_obj_id);
			m_familyboss_set.insert(monster_set_id);

			this->SendBossInfoToAllGateway(family_boss, Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY, Protocol::NOTIFY_REASON_BOSS_REFRESH);
			this->SendFamilyBossInfoToAll(family_boss.scene_id);


			// 跨服中的boss出生,需要同步到非跨服
			if (CrossConfig::Instance().IsHiddenServer())
			{
				BossFamilyBossMapIt it = m_cross_familyboss_info_map.find(family_boss.scene_id);
				if (it != m_cross_familyboss_info_map.end())
				{
					crossgameprotocal::CrossGameCrossCommonBossInfoChange ccbsi;

					ccbsi.boss_type = Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY;
					ccbsi.reason_type = crossgameprotocal::CROSS_COMMON_BOSS_SYNC_REASON_TYPE_BORN;
					ccbsi.scene_id = static_cast<unsigned short>(scene_id);
					ccbsi.boss_id = family_boss.boss_id;

					InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_COMMON, 0, (const char *)&ccbsi, sizeof(ccbsi));
				}
			}
		}
	}
}

void BossFamilyManager::RefreshOneMikuBoss(BossInfo &miku_boss)
{
	if (miku_boss.boss_id < 0) return;

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(miku_boss.scene_id, 0);
	if (NULL != scene)
	{
		Monster *new_monster = MONSTERPOOL->CreateMonster(miku_boss.boss_id, scene, miku_boss.born_pos);
		if (NULL != new_monster)
		{
			miku_boss.boss_status = ENBOSSSTATUS_EXIST;
			miku_boss.next_refresh_time = 0;
			long long scene_id = miku_boss.scene_id;
			ObjID monster_obj_id = new_monster->GetId();
			long long monster_set_id = ((scene_id << 32) + monster_obj_id);
			m_mikuboss_set.insert(monster_set_id);

			this->SendBossInfoToAllGateway(miku_boss, Protocol::BOSS_ENTER_TYPE_BOSS_MIKU, Protocol::NOTIFY_REASON_BOSS_REFRESH);
			this->SendMikuBossInfoToAll(miku_boss.scene_id);
		}
	}
}

void BossFamilyManager::RefreshOneDabaoBoss(BossInfo &dabao_boss)
{
	if (dabao_boss.boss_id < 0) return;

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(dabao_boss.scene_id, 0);
	if (NULL != scene)
	{
		Monster *new_monster = MONSTERPOOL->CreateMonster(dabao_boss.boss_id, scene, dabao_boss.born_pos);
		if (NULL != new_monster)
		{
			dabao_boss.boss_status = ENBOSSSTATUS_EXIST;
			dabao_boss.next_refresh_time = 0;
			this->SendBossInfoToAllGateway(dabao_boss, Protocol::BOSS_ENTER_TYPE_BOSS_DABAO, Protocol::NOTIFY_REASON_BOSS_REFRESH);

			// modify 2019-03-28  策划需求去除单个BOSS刷新传闻
			//int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF),
			//	gamestring::g_refresh_one_boss, dabao_boss.boss_id, dabao_boss.scene_id);
			//if (length > 0)
			//{
			//	World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2, 0, 0);
			//}
		}
	}
}

void BossFamilyManager::RefreshMikuMonster(MonsterInfo &miku_monster)
{
	//if (miku_monster.monster_id < 0) return;

	//Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(miku_monster.scene_id, 0);
	//if (NULL != scene)
	//{
	//	Monster *new_monster = MONSTERPOOL->CreateMonster(miku_monster.monster_id, scene, miku_monster.born_pos);
	//	if (NULL != new_monster)
	//	{
	//		miku_monster.monster_status = ENBOSSSTATUS_EXIST;
	//		++m_miku_monster_num[miku_monster.scene_id];
	//	}
	//}
}

void BossFamilyManager::RefreshOneActiveBossInfo(BossInfo &active_boss)
{
	if (active_boss.boss_id < 0) return;

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(active_boss.scene_id, 0);
	if (NULL != scene && active_boss.refresh_interval > 0)
	{
		MONSTERPOOL->CreateMonster(active_boss.boss_id, scene, active_boss.born_pos);
		active_boss.boss_status = ENBOSSSTATUS_EXIST;
		active_boss.next_refresh_time = 0;
		this->SendBossInfoToAllGateway(active_boss, Protocol::BOSS_ENTER_TYPE_BOSS_ACTIVE, Protocol::NOTIFY_REASON_BOSS_REFRESH);

		// modify 2019-03-28  策划需求去除单个BOSS刷新传闻
		//int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF),
		//	gamestring::g_refresh_one_boss, active_boss.boss_id, active_boss.scene_id);
		//if (length > 0)
		//{
		//	World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2, 0, 0);
		//}
	}
}

void BossFamilyManager::RefreshActiveBossInfo(int scene_id)
{	
	//服务端根据配置刷新boss
	//if (!Scene::IsActiveBossScene(scene_id))
	{
		return;
	}

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
	ActiveBossMapIt active_it = m_activeboss_info_map.find(scene_id);

	if (NULL != scene && m_activeboss_info_map.end() != active_it)
	{
		ActiveBossInfo &activeboss_info = active_it->second;
		SceneMonsterLogic::MonsterGeneraterList moster_gen_list = scene->GetMonsterLogic()->GetMonsterGenList();
		if (!moster_gen_list.empty())
		{
			SceneMonsterLogic::MonsterGeneraterList::iterator iter = moster_gen_list.begin();
			for (; iter != moster_gen_list.end(); ++iter)
			{
				int next_flush_time = (*iter)->GetMonsterNextFlushTime();
				if (0 > next_flush_time)
				{
					continue;
				}

				int bossID = (*iter)->GetMonsterID();
				std::map<int, BossInfo>::iterator boss_it = activeboss_info.boss_info_map.find(bossID);
				if (activeboss_info.boss_info_map.end() != boss_it)
				{
					BossInfo &boss_info = boss_it->second;
					boss_info.boss_id = bossID;
					boss_info.scene_id = scene_id;
					if (0 == next_flush_time)
					{
						boss_info.boss_status = ENBOSSSTATUS_EXIST;
						boss_info.next_refresh_time = 0;
					}
					else
					{
						boss_info.boss_status = ENBOSSSTATUS_NONEXIST;
						boss_info.next_refresh_time = next_flush_time;
					}
				}
			}
		}
	}
}

void BossFamilyManager::RefreshOneBabyBossInfo(BossInfo &baby_boss)
{
	if (baby_boss.boss_id < 0) return;
	const BabyBossCfg *boss_cfg = LOGIC_CONFIG->GetBabyBossConfig().GetBabyBossCfgByBossId(baby_boss.boss_id);
	if (nullptr == boss_cfg || boss_cfg->is_boss == 0)
	{
		return;
	}

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(baby_boss.scene_id, 0);
	if (NULL != scene)
	{
		Monster *monster = MONSTERPOOL->CreateMonster(baby_boss.boss_id, scene, boss_cfg->born_pos);
		if (NULL != monster)
		{
			baby_boss.born_pos = boss_cfg->born_pos;
			baby_boss.boss_status = ENBOSSSTATUS_EXIST;
			baby_boss.next_refresh_time = 0;

			this->SendBossInfoToAllGateway(baby_boss, Protocol::BOSS_ENTER_TYPE_BOSS_BABY, Protocol::NOTIFY_REASON_BOSS_REFRESH);
		}
	}
}

void BossFamilyManager::CheckKickOutDabao(int scene_id, time_t now_second)
{
	if (!Scene::IsDabaoBossScene(scene_id)) return;

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
	if (NULL == scene) return;

	const BossFamilyOtherCfg &bossfamily_othercfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyOtherCfg();

	for (int i = 0; i < static_cast<int>(scene->RoleNum()); ++i)
	{
		Role *role = scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			CommonDataParam *common_p = role->GetCommonDataParam();
			if (NULL == common_p) return;

			if (common_p->dabao_next_add_angry_timestamp == 0)
			{
				common_p->dabao_next_add_angry_timestamp = (unsigned int)(now_second + bossfamily_othercfg.dabao_add_angry_interval_s);
			}

			if (0 < common_p->dabao_kick_time && (unsigned int)now_second >= common_p->dabao_kick_time)
			{
				common_p->dabao_kick_time = 0;

				Posi target_pos(0, 0);
				int target_scene_id = role->GetLastScene(&target_pos);
				ChangeSceneDelayer *timer = new ChangeSceneDelayer(World::GetInstWorld()->GetSceneManager(), scene->GetIndex(), role->GetId(),
					role->GetUserId(), target_scene_id, 0, target_pos, CHANGE_SCENE_REASON_INVALID);

				EngineAdapter::Instance().CreateTimer(1, timer);
				role->NoticeNum(noticenum::NT_KICK_OUT_FROM_DABAO);
			}

			if (common_p->dabao_next_add_angry_timestamp > 0 && common_p->dabao_angry_value < bossfamily_othercfg.max_value && (unsigned int)now_second >= common_p->dabao_next_add_angry_timestamp)
			{
				int add_angry_value_times = 1;
				if (bossfamily_othercfg.dabao_add_angry_interval_s > 0)
				{
					add_angry_value_times += ((unsigned int)now_second - common_p->dabao_next_add_angry_timestamp) / bossfamily_othercfg.dabao_add_angry_interval_s;
				}
				
				common_p->dabao_next_add_angry_timestamp = (unsigned int)(now_second + bossfamily_othercfg.dabao_add_angry_interval_s);
				int add_angry_value = bossfamily_othercfg.dabao_add_angry_value * add_angry_value_times;
				int total_angry_value = common_p->dabao_angry_value + add_angry_value;

				common_p->dabao_angry_value = (total_angry_value >= bossfamily_othercfg.max_value) ? bossfamily_othercfg.max_value : total_angry_value;

				if (common_p->dabao_angry_value >= bossfamily_othercfg.max_value)
				{
					common_p->dabao_kick_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + bossfamily_othercfg.dabao_kick_delay_time;
				}

				this->SendDabaoBossInfo(role);
			}
		}
	}
}

void BossFamilyManager::RecalcMikuFlushTime()
{
	// 开服之前不要重新计算（有可能服务器在开服之前已经搭好了，只是没开放入口）
	int opengame_day_index = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex();
	if (opengame_day_index <= 0)
	{
		return;
	}

	int role_level_list[MAX_ROLE_LEVEL + 1] = { 0 };

	unsigned int yesterday_zero_time = (unsigned int)GetZeroTime(EngineAdapter::Instance().Time()) - 24 * 3600;

	const std::map<int, UserCacheNode> & user_map = UserCacheManager::Instance().GetUserCacheMap();
	for (std::map<int, UserCacheNode>::const_iterator it_user = user_map.begin(); it_user != user_map.end(); it_user++)
	{
		int level = it_user->second.level;
		if (level > 0 && level <= MAX_ROLE_LEVEL)
		{
			if (it_user->second.last_logout_timestamp >= yesterday_zero_time || UserCacheManager::Instance().IsUserOnline(it_user->first))
			{
				role_level_list[level]++;
			}
		}
	}

	int boss_count = 0;
	memset(&m_special_refresh_param, 0, sizeof(m_special_refresh_param));

	const std::map<int, BossSpecialRefreshCfg> &refresh_map = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossSpecialRefreshCfg();
	for (std::map<int, BossSpecialRefreshCfg>::const_iterator it = refresh_map.begin(); it != refresh_map.end(); it++)
	{
		const BossSpecialRefreshCfg &refresh_cfg = it->second;

		int min_level = refresh_cfg.min_role_level, max_level = refresh_cfg.max_role_level;
		if (min_level <= 0) min_level = 1;
		if (max_level > MAX_ROLE_LEVEL) max_level = MAX_ROLE_LEVEL;
		
		int role_count = 0;
		for (int lv = min_level; lv <= max_level; lv++)
		{
			role_count += role_level_list[lv];
		}

		for (int i = 0; i < refresh_cfg.item_count && i < BossSpecialRefreshParam::REFRESH_ITEM_MAX; i++)
		{
			if (role_count <= refresh_cfg.item_list[i].role_num)
			{
				m_special_refresh_param.refresh_item_list[boss_count].boss_id = refresh_cfg.boss_id;
				m_special_refresh_param.refresh_item_list[boss_count].refresh_interval = refresh_cfg.item_list[i].flush_time_s;
				boss_count++;
				break;
			}
		}

		if (boss_count >= BossSpecialRefreshParam::REFRESH_ITEM_MAX) break;
	}

	this->UpdateMikuBossRefreshInterval();
}

// 原来是每天定时刷新所有boss刷新间隔信息，现在改成每次boss死亡就重新计算单个boss的刷新时间
// 原有的机制尽量不动，所以写得比较多
void BossFamilyManager::RecalcMikuOnBossFlushTime(int boss_id)
{
	// 开服第一天之前不要重新计算
	int opengame_day_index = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex();
	if (opengame_day_index <= 0)
	{
		return;
	}

	const std::map<int, BossFamilyMikuBossInfoCfg> &m_mikuboss_map = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyMikuBossMap();
	std::map<int, BossFamilyMikuBossInfoCfg>::const_iterator miku_cfg_it = m_mikuboss_map.begin();
	if (miku_cfg_it == m_mikuboss_map.end())
	{
		return;
	}

	BossSpecialRefreshParam::RefreshItem *cur_boss_refresh_item = NULL;
	for (int i = 0; i < BossSpecialRefreshParam::REFRESH_ITEM_MAX; ++i)
	{
		// 找到一个已存在的节点或者新节点存放当前boss刷新信息
		if (m_special_refresh_param.refresh_item_list[i].boss_id <= 0 || m_special_refresh_param.refresh_item_list[i].boss_id == boss_id)
		{
			cur_boss_refresh_item = &m_special_refresh_param.refresh_item_list[i];
			cur_boss_refresh_item->boss_id = boss_id;
			cur_boss_refresh_item->refresh_interval = miku_cfg_it->second.refresh_interval; // 初始化用默认刷新间隔
			break;
		}
	}
	if (NULL == cur_boss_refresh_item)
	{
		return;
	}

	int role_level_list[MAX_ROLE_LEVEL + 1] = { 0 };
	unsigned int yesterday_zero_time = (unsigned int)GetZeroTime(EngineAdapter::Instance().Time()) - SECOND_PER_DAY;
	const std::map<int, UserCacheNode> & user_cache_map = UserCacheManager::Instance().GetUserCacheMap();
	for (std::map<int, UserCacheNode>::const_iterator it_user = user_cache_map.begin(); it_user != user_cache_map.end(); it_user++)
	{
		int level = it_user->second.level;
		if (level > 0 && level <= MAX_ROLE_LEVEL)
		{
			if (it_user->second.last_logout_timestamp >= yesterday_zero_time ||	UserCacheManager::Instance().IsUserOnline(it_user->first))
			{
				role_level_list[level]++;
			}
		}
	}

	const std::map<int, BossSpecialRefreshCfg> &refresh_map = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossSpecialRefreshCfg();
	std::map<int, BossSpecialRefreshCfg>::const_iterator it = refresh_map.find(boss_id);
	if (it != refresh_map.end())
	{
		const BossSpecialRefreshCfg &refresh_cfg = it->second;

		int min_level = refresh_cfg.min_role_level, max_level = refresh_cfg.max_role_level;
		if (min_level <= 0) min_level = 1;
		if (max_level > MAX_ROLE_LEVEL) max_level = MAX_ROLE_LEVEL;

		int role_count = 0;
		for (int lv = min_level; lv <= max_level; lv++)
		{
			role_count += role_level_list[lv];
		}

		for (int i = 0; i < refresh_cfg.item_count && i < BossSpecialRefreshCfg::ITEM_COUNT_MAX; i++)
		{
			if (role_count <= refresh_cfg.item_list[i].role_num)
			{
				cur_boss_refresh_item->boss_id = refresh_cfg.boss_id;
				cur_boss_refresh_item->refresh_interval = refresh_cfg.item_list[i].flush_time_s;
				break;
			}
		}
	}

	this->UpdateMikuBossRefreshInterval();
}

void BossFamilyManager::UpdateMikuBossRefreshInterval()
{
	for (int i = 0; i < BossSpecialRefreshParam::REFRESH_ITEM_MAX; i++)
	{
		BossSpecialRefreshParam::RefreshItem &info = m_special_refresh_param.refresh_item_list[i];
		if (0 == info.boss_id) break;

		for (MikuBossMapIt it = m_mikuboss_info_map.begin(); it != m_mikuboss_info_map.end(); it++)
		{
			MikuBossInfo &miku_info = it->second;
			std::map<int, BossInfo>::iterator it_miku = miku_info.boss_info_map.find(info.boss_id);
			if (it_miku != miku_info.boss_info_map.end())
			{
				it_miku->second.refresh_interval = info.refresh_interval;
			}
		}
	}
}

void BossFamilyManager::RefreshOnePreciousBoss(BossInfo &precious_boss)
{
	if (precious_boss.boss_id < 0) return;
	if (m_precious_boss_info_map.find(precious_boss.boss_id) == m_precious_boss_info_map.end())
	{
		return;
	}

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(precious_boss.scene_id, 0);
	if (NULL != scene)
	{
		const std::vector<PreciousMonsterCfg> &precious_monster_vec = LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousMonsterWorldLevelCfg();
		if (precious_boss.seq < 0 || precious_boss.seq >= static_cast<int>(precious_monster_vec.size()))
		{
			return;
		}

		if (!precious_monster_vec.empty() && static_cast<int>(precious_monster_vec.size()) > precious_boss.seq)
		{
			if (precious_monster_vec[precious_boss.seq].seq == precious_boss.seq)
			{
				precious_boss.boss_id = precious_monster_vec[precious_boss.seq].monster_id;
			}
		}

		Posi pos = this->PreciousBossPosHelper(PreciousBossRefreshPosCfg::PRECIOUS_BOSS_POS_TYPE_BOSS);
		if (pos != Posi(0, 0))
		{
			Monster *monster = MONSTERPOOL->CreateMonster(precious_boss.boss_id, scene, pos);
			if (NULL != monster)
			{
				precious_boss.born_pos = pos;

				precious_boss.boss_status = ENBOSSSTATUS_EXIST;
				precious_boss.next_refresh_time = 0;

				this->SendBossInfoToAllGateway(precious_boss, Protocol::BOSS_ENTER_TYPE_BOSS_PRECIOUS, Protocol::NOTIFY_REASON_BOSS_REFRESH);
			}
		}
	}
}

void BossFamilyManager::RefreshPreciousMonster(MonsterInfo &precious_monster)
{
	if (precious_monster.monster_id < 0) return;

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(precious_monster.scene_id, 0);
	int type = precious_monster.is_great_monster ? PreciousBossRefreshPosCfg::PRECIOUS_BOSS_POS_TYPE_GREAT_MONSTER : PreciousBossRefreshPosCfg::PRECIOUS_BOSS_POS_TYPE_COMMON_MONSTER;
	if (NULL != scene)
	{
		const std::vector<PreciousMonsterCfg> &precious_monster_vec = LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousMonsterWorldLevelCfg();
		if (precious_monster.seq < 0 || precious_monster.seq >= static_cast<int>(precious_monster_vec.size()))
		{
			return;
		}

		if (!precious_monster_vec.empty())
		{
			if (precious_monster_vec[precious_monster.seq].seq == precious_monster.seq)
			{
				precious_monster.monster_id = precious_monster_vec[precious_monster.seq].monster_id;
			}
		}

		Posi pos = this->PreciousBossPosHelper(type);
		if (pos != Posi(0, 0))
		{
			Monster *monster = MONSTERPOOL->CreateMonster(precious_monster.monster_id, scene, pos);
			if (NULL != monster)
			{
				precious_monster.born_pos = pos;
				precious_monster.monster_status = ENBOSSSTATUS_EXIST;
				precious_monster.next_refresh_time = 0;
				if (m_precious_monster_map.find(monster->GetId()) == m_precious_monster_map.end())
				{
					m_precious_monster_map[monster->GetId()] = precious_monster;
				}
			}
		}
	}
}

void BossFamilyManager::PreciousBossRefreshGather(PreciousGatherInfo &gather_info)
{
	const PreciousBossOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousBossOtherCfg();
	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.precious_boss_scene_id, 0);
	if (NULL != scene)
	{
		Posi pos = this->PreciousBossPosHelper(PreciousBossRefreshPosCfg::PRECIOUS_BOSS_POS_TYPE_GATHER);
		if (pos != Posi(0, 0))
		{
			GatherObj *gather_obj = new GatherObj();
			gather_obj->SetPos(pos);
			gather_obj->Init(NULL, other_cfg.gather_id, other_cfg.gather_cost_time, 0, true);
			scene->AddObj(gather_obj);

			gather_info.pos = pos;
			gather_info.is_gather = 1;
			gather_info.next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.gather_refresh_interval;
		}
	}
}

void BossFamilyManager::PreciousBossRewardHelper(Role *role, int reward_param, bool is_skip)
{
	const std::map<int, PreciousBossRewardCfg> &precious_reward_map =
		LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousBossRewardCfg(role->GetLevel());
	if (!precious_reward_map.empty())
	{
		std::map<int, PreciousBossRewardCfg>::const_iterator iter = precious_reward_map.find(reward_param);
		if (iter != precious_reward_map.end())
		{
			role->GetRoleShop()->AddChestShopPreciousBossScore(iter->second.reward_score, "PreciousBoss");

			if (!is_skip)
			{
				Role *member_list[MAX_TEAM_MEMBER_NUM] = { NULL };
				int team_rolenum = role->GetTeamInfo()->GetOtherMemberInTheSameScene(member_list, sizeof(member_list) / sizeof(member_list[0]));

				for (int i = 0; i < team_rolenum; ++i)
				{
					if (NULL == member_list[i])
					{
						continue;
					}

					this->PreciousBossTeamRewardHelper(member_list[i], reward_param);
				}
			}
		}
	}
}

void BossFamilyManager::OnEnterBabyBoss(Role *role, int scene_id, unsigned short boss_id)
{
	const BabyBossCfg *boss_cfg = LOGIC_CONFIG->GetBabyBossConfig().GetBabyBossCfgByBossId(boss_id);
	if (nullptr == boss_cfg)
	{
		return;
	}

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
	if (nullptr == role || nullptr == scene || scene->GetSceneType() != Scene::SCENE_TYPE_BABY_BOSS)
	{
		return;
	}

	if (role->GetRoleStatusManager()->IsInFB())
	{
		role->NoticeNum(errornum::EN_CAN_NOT_ENTER_BOSS);
		return;
	}

	Posi result_pos;
	for (int i = 0; i < 100; ++i)
	{
		result_pos = gamecommon::GetDisRandPos(boss_cfg->enter_pos, 15);
		if (nullptr != scene->GetMap())
		{
			// 寻找一个可达点
			if (scene->GetMap()->IsValidatePos(Obj::OBJ_TYPE_ROLE, result_pos.x, result_pos.y))
			{
				break;
			}
		}
	}

	if (Posi(0, 0) == result_pos)
	{
		return;
	}

	short &enter_times = role->GetCommonDataParam()->baby_boss_enter_times;
	int enter_limit_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_BABY_BOSS_BUY_ENTER_TIMES);
	if (enter_times >= enter_limit_times)
	{
		return;
	}

	const BabyBossOtherCfg &baby_boss_other_cfg = LOGIC_CONFIG->GetBabyBossConfig().GetOtherCfg();
	const BabyBossEnterCostCfg *cfg = LOGIC_CONFIG->GetBabyBossConfig().GetEnterCostCfg(enter_times);

	if (nullptr == cfg) return;

	if (!role->GetKnapsack()->ConsumeItemOrUseGold(baby_boss_other_cfg.need_item_id, cfg->need_item_num, "BossFamilyManager::OnEnterBabyBoss", true, true))
	{
		return;
	}

	++enter_times;

	this->SendBabyBossRoleInfo(role);

	World::GetInstWorld()->GetSceneManager()->GoTo(role, scene_id, 0, result_pos);
}

void BossFamilyManager::OnLeaveBabyBossReq(Role *role)
{
	if (nullptr != role)
	{
		Scene *scene = role->GetScene();
		if (nullptr != scene)
		{
			const BabyBossCfg *scene_cfg = LOGIC_CONFIG->GetBabyBossConfig().GetBabyBossCfgBySceneId(scene->GetSceneID());
			if (nullptr == scene_cfg)
			{
				return;
			}

			if (scene->GetSceneType() != Scene::SCENE_TYPE_BABY_BOSS)
			{
				return;
			}

			Posi pos;
			int scene_id = role->GetLastScene(&pos);
			World::GetInstWorld()->GetSceneManager()->GoTo(role, scene_id, 0, pos);
		}
	}
}

bool BossFamilyManager::IsBabyBossEnterTimesEnough(Role * role)
{
	if (nullptr == role)
	{
		return false;
	}
	short enter_times = role->GetCommonDataParam()->baby_boss_enter_times;
	int enter_limit_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_BABY_BOSS_BUY_ENTER_TIMES);
	if (enter_times >= enter_limit_times)
	{
		return false;
	}
	return true;
}

bool BossFamilyManager::AddBabyBossEnterTimes(Role * role)
{
	if (nullptr == role)
	{
		return false;
	}
	short &enter_times = role->GetCommonDataParam()->baby_boss_enter_times;
	int enter_limit_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_BABY_BOSS_BUY_ENTER_TIMES);
	if (enter_times >= enter_limit_times)
	{
		return false;
	}
	++enter_times;
	this->SendBabyBossRoleInfo(role);
	return true;
}

void BossFamilyManager::SendBabyBossRoleInfo(Role *role)
{
	if (nullptr != role)
	{
		Protocol::SCBabyBossRoleInfo pro;
		pro.angry_value = role->GetCommonDataParam()->baby_boss_angry_value;
		pro.enter_times = role->GetCommonDataParam()->baby_boss_enter_times;
		pro.kick_time = role->GetCommonDataParam()->baby_boss_kick_time;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, sizeof(pro));
	}
}

void BossFamilyManager::SendAllBabyBossInfo(Role *role)
{
	if (nullptr != role)
	{
		Scene *scene = nullptr;
		SpecialBabyBoss *special = nullptr;
		static Protocol::SCAllBabyBossInfo pro;
		pro.boss_count = 0;

		BabyBossMapIt baby_it = m_babyboss_info_map.begin();
		for (; m_babyboss_info_map.end() != baby_it; baby_it++)
		{
			std::map<int, BossInfo>::iterator baby_boss = baby_it->second.boss_info_map.begin();
			for (; baby_it->second.boss_info_map.end() != baby_boss; baby_boss++)
			{
				const BabyBossCfg *boss_cfg = LOGIC_CONFIG->GetBabyBossConfig().GetBabyBossCfgByBossId(baby_boss->second.boss_id);
				if (nullptr == boss_cfg || boss_cfg->is_boss == 0)
				{
					continue;;
				}

				scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(baby_boss->second.scene_id, 0);
				if (nullptr == scene || scene->GetSceneType() != Scene::SCENE_TYPE_BABY_BOSS)
				{
					continue;
				}

				special = dynamic_cast<SpecialBabyBoss*>(scene->GetSpecialLogic());
				if (nullptr == special)
				{
					continue;
				}

				pro.boss_info_list[pro.boss_count].boss_id = baby_boss->second.boss_id;
				pro.boss_info_list[pro.boss_count].scene_id = baby_boss->second.scene_id;
				pro.boss_info_list[pro.boss_count].next_refresh_time = baby_boss->second.next_refresh_time;
				special->GetKillerInfo(pro.boss_info_list[pro.boss_count].boss_id, pro.boss_info_list[pro.boss_count].killer_info_list, KILLER_LIST_MAX_COUNT);
				++pro.boss_count;
			}
		}

		if (pro.boss_count > 0)
		{
			int send_len = sizeof(pro) - sizeof(pro.boss_info_list[0]) * (Protocol::SCAllBabyBossInfo::MAX_BABY_BOSS_COUNT - pro.boss_count);

			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, send_len);
		}
	}
}

void BossFamilyManager::OnGodmagicBossReq(Role * role, Protocol::CSGodmagicBossInfoReq* req)
{
	if (role == NULL) return;

	if (Protocol::GODMAGIC_BOSS_OPERA_TYPE_ENTER == req->req_type)
	{
		this->OnEnterGodmagicBoss(role, req->param_1, req->param_2);
		return;
	}
	else if (Protocol::GODMAGIC_BOSS_OPERA_TYPE_PLAYER_INFO == req->req_type)
	{
		this->SendGodmagicBossRoleInfo(role);
		return;
	}
	else if (Protocol::GODMAGIC_BOSS_OPERA_TYPE_GET_FLUSH_INFO == req->req_type)
	{
		if (0 == req->param_1)
		{
			for (int i = 1; i <= GODMAGIC_BOSS_SCENE_MAX; ++i)
			{
				const GodmagicBossLayerCfg *layer_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossLayerCfg(i);
				if (NULL == layer_cfg)
				{
					continue;
				}

				SpecialGodmagicBoss *godmagic_boss = World::GetInstWorld()->GetSceneManager()->GetSpeciaGodmagicBoss(layer_cfg->scene_id, 0);
				if (NULL != godmagic_boss)
				{
					godmagic_boss->SyncSceneInfo(role);
				}
			}
		}
		else
		{
			const GodmagicBossLayerCfg *layer_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossLayerCfg(req->param_1);
			if (NULL == layer_cfg)
			{
				return;
			}

			SpecialGodmagicBoss *godmagic_boss = World::GetInstWorld()->GetSceneManager()->GetSpeciaGodmagicBoss(layer_cfg->scene_id, 0);
			if (NULL != godmagic_boss)
			{
				godmagic_boss->SyncSceneInfo(role);
			}
		}
		return;
	}
}

void BossFamilyManager::OnEnterGodmagicBoss(Role *role, int scene_id, int boss_id)
{
	const GodmagicBossBossCfg::BossCfg *boss_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetGodmagicBossBossCfgByMonsterId(scene_id,boss_id);
	if (nullptr == boss_cfg)
	{
		return;
	}

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, 0);
	if (nullptr == role || nullptr == scene || scene->GetSceneType() != Scene::SCENE_TYPE_GODMAGIC_BOSS)
	{
		return;
	}

	if (role->GetRoleStatusManager()->IsInFB())
	{
		role->NoticeNum(errornum::EN_CAN_NOT_ENTER_BOSS);
		return;
	}

	Posi result_pos;
	for (int i = 0; i < 100; ++i)
	{
		result_pos = gamecommon::GetDisRandPos(Posi(boss_cfg->flush_pos_x,boss_cfg->flush_pos_y), 15);
		if (nullptr != scene->GetMap())
		{
			// 寻找一个可达点
			if (scene->GetMap()->IsValidatePos(Obj::OBJ_TYPE_ROLE, result_pos.x, result_pos.y))
			{
				break;
			}
		}
	}

	if (Posi(0, 0) == result_pos)
	{
		return;
	}

	World::GetInstWorld()->GetSceneManager()->GoTo(role, scene_id, 0, result_pos);
}

void BossFamilyManager::SendGodmagicBossRoleInfo(Role * role)
{
	if (NULL == role) return;
	CommonDataParam * comm_p = role->GetCommonDataParam();
	const GodmagicBossOtherCfg &other_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetOtherCfg();
	static Protocol::SCGodmagicBossPlayerInfo  cbpi;

	int left_can_kill_boss_num = other_cfg.daily_boss_num - comm_p->godmagic_today_kill_boss_num;
	cbpi.left_can_kill_boss_num = left_can_kill_boss_num > 0 ? left_can_kill_boss_num : 0;
	int left_ordinary_crystal_gather_times = other_cfg.ordinary_crystal_gather_times - comm_p->today_gather_ordinary_crystal_num;
	cbpi.left_ordinary_crystal_gather_times = left_ordinary_crystal_gather_times > 0 ? left_ordinary_crystal_gather_times : 0;
	int left_treasure_crystal_gather_times = other_cfg.treasure_crystal_gather_times -comm_p->today_gather_treasure_crystal_num;
	cbpi.left_treasure_crystal_gather_times = left_treasure_crystal_gather_times > 0 ? left_treasure_crystal_gather_times : 0;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&cbpi, sizeof(cbpi));
}

int BossFamilyManager::GetGodmagicBossTire(Role * role)
{
	if (NULL == role) return 0;
	CommonDataParam * comm_p = role->GetCommonDataParam();
	const GodmagicBossOtherCfg &other_cfg = LOGIC_CONFIG->GetGodmagicBossConfig().GetOtherCfg();

	int left_can_kill_boss_num = other_cfg.daily_boss_num - comm_p->godmagic_today_kill_boss_num;
	return left_can_kill_boss_num > 0 ? left_can_kill_boss_num : 0;
}

bool BossFamilyManager::AddGodmagicBossKillNum(Role * role)
{
	if (NULL == role) return false;
	CommonDataParam * comm_p = role->GetCommonDataParam();

	++comm_p->godmagic_today_kill_boss_num;
	this->SendGodmagicBossRoleInfo(role);
	return true;
}

void BossFamilyManager::PreciousBossTeamRewardHelper(Role *role, int reward_param)
{
	if (reward_param >= PreciousBossParam::PRECIOUS_BOSS_TASK_TYPE_BOSS && reward_param < PreciousBossParam::PRECIOUS_BOSS_TASK_TYPE_MAX)
	{
		return;
	}

	const std::map<int, PreciousBossRewardCfg> &precious_reward_map =
		LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousBossRewardCfg(role->GetLevel());
	if (!precious_reward_map.empty())
	{
		std::map<int, PreciousBossRewardCfg>::const_iterator iter = precious_reward_map.find(reward_param);
		if (iter != precious_reward_map.end())
		{
			const PreciousBossOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousBossOtherCfg();
			role->GetRoleShop()->AddChestShopPreciousBossScore(iter->second.reward_score * other_cfg.team_member_reward_precent / 100, "PreciousBossTeam");
		}
	}
}

void BossFamilyManager::PreciousBossTaskHelper(Role *role, int task_type, bool is_team_task)
{
	const PreciousBossTaskCfg *task_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetPreciousBossTaskCfg(task_type);
	if (NULL != task_cfg)
	{
		bool is_finish = role->GetRolePreciousBoss()->AddPreciousBossTaskCondition(task_type, task_cfg->task_condition);

		if (is_finish)
		{
			this->PreciousBossRewardHelper(role, task_type);
		}

		this->SendPreciousBossTaskInfo(role);
	}

	if (!is_team_task)
	{
		Role *member_list[MAX_TEAM_MEMBER_NUM] = { NULL };
		int team_rolenum = role->GetTeamInfo()->GetOtherMemberInTheSameScene(member_list, sizeof(member_list) / sizeof(member_list[0]));

		for (int i = 0; i < team_rolenum && i < MAX_TEAM_MEMBER_NUM; ++i)
		{
			if (NULL == member_list[i])
			{
				continue;
			}

			this->PreciousBossTaskHelper(member_list[i], task_type, true);
		}
	}
}

Posi BossFamilyManager::PreciousBossPosHelper(int type)
{
	Posi pos(0, 0);
	if (m_precious_pos_pool.find(type) == m_precious_pos_pool.end())
	{
		return pos;
	}

	std::vector<Posi> &pos_vec = m_precious_pos_pool[type];
	int pos_count = (int)pos_vec.size();
	if (pos_count <= 0)
	{
		return pos;
	}

	int index = RandomNum(pos_count);

	pos = pos_vec[index];
	pos_vec[index] = pos_vec[pos_count - 1];
	pos_vec.pop_back();

	return pos;
}

void BossFamilyManager::AddPreciousBossPos(int pos_type, const Posi &pos)
{
	std::map<int, std::vector<Posi> >::iterator it = m_precious_pos_pool.find(pos_type);
	if (it == m_precious_pos_pool.end())
	{
		return;
	}

	it->second.push_back(pos);
}

void BossFamilyManager::SendBabyBossKillerInfo(Role *role, int boss_id)
{
	if (nullptr == role)
	{
		return;
	}

	const BabyBossCfg *boss_cfg = LOGIC_CONFIG->GetBabyBossConfig().GetBabyBossCfgByBossId(boss_id);
	if (nullptr == boss_cfg)
	{
		return;
	}

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(boss_cfg->scene_id, 0);

	if (nullptr == scene)
	{
		return;
	}

	SpecialBabyBoss *special = (SpecialBabyBoss *)scene->GetSpecialLogic();

	if (nullptr == special)
	{
		return;
	}

	static Protocol::SCBossKillerList bkl;

	Protocol::BabyBossInfo::KillerInfo killer_info_list[KILLER_LIST_MAX_COUNT];
	special->GetKillerInfo(boss_id, killer_info_list, KILLER_LIST_MAX_COUNT);

	for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
	{
		bkl.killer_info_list[i].killer_uid = killer_info_list[i].killer_uid;
		bkl.killer_info_list[i].killier_time = killer_info_list[i].killier_time;
		F_STRNCPY(bkl.killer_info_list[i].killer_name, killer_info_list[i].killer_name, sizeof(GameName));
	}
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&bkl, sizeof(bkl));
}

void BossFamilyManager::OnRoleLoginActiveBoss(Role *role)
{
	if (nullptr == role || nullptr == role->GetScene())
	{
		return;
	}

	int scene_id = role->GetScene()->GetSceneID();

	//if (!Scene::IsActiveBossScene(scene_id))
	//{
	//	return;
	//}

	// 只记录当前场景的
	ActiveBossMapIt active_it = m_activeboss_info_map.find(scene_id);
	if (m_activeboss_info_map.end() == active_it)
	{
		return;
	}

	auto hurt_rank_it = active_it->second.hurt_rank_info_map.begin();
	for (; hurt_rank_it != active_it->second.hurt_rank_info_map.end(); ++hurt_rank_it)
	{
		auto user_it = hurt_rank_it->second.hurt_info_list.begin();
		for (; user_it != hurt_rank_it->second.hurt_info_list.end(); ++user_it)
		{
			if (user_it->uid == role->GetUID())
			{
				user_it->logouttime = 0;  // 重置掉线时间
			}
		}
	}
}

void BossFamilyManager::OnRoleLogoutActiveBoss(Role *role)
{
	if (nullptr == role || nullptr == role->GetScene())
	{
		return;
	}

	int scene_id = role->GetScene()->GetSceneID();

	if (!Scene::IsActiveBossScene(scene_id))
	{
		return;
	}

	// 只记录当前场景的
	ActiveBossMapIt active_it = m_activeboss_info_map.find(scene_id);
	if (m_activeboss_info_map.end() == active_it)
	{
		return;
	}

	auto hurt_rank_it = active_it->second.hurt_rank_info_map.begin();
	for (;hurt_rank_it != active_it->second.hurt_rank_info_map.end(); ++ hurt_rank_it)
	{
		auto user_it = hurt_rank_it->second.hurt_info_list.begin();
		for (; user_it != hurt_rank_it->second.hurt_info_list.end(); ++ user_it)
		{
			if (user_it->uid == role->GetUID())
			{
				user_it->logouttime = static_cast<unsigned int>(EngineAdapter::Instance().Time());  // 记录掉线的时间
			}
		}
	}
}

void BossFamilyManager::OnAttackActiveBoss(Role *attacker, Monster *monster, long long &delta_hp)
{
	if (nullptr == attacker || nullptr == monster) return;

	CommonDataParam *common_p = attacker->GetCommonDataParam();
	int scene_id = monster->GetScene()->GetSceneID();

	const WearyCfg *active_boss_weary_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetWearyCfg(Protocol::CSBossFamilyOperate::BOSS_FAMILY_BUY_ACTIVE_BOSS_WEARY);
	if (NULL == common_p || active_boss_weary_cfg == NULL)
	{
		return;
	}

	if (common_p->active_boss_weary >= active_boss_weary_cfg->weary_limit)
	{
		attacker->NoticeNum(errornum::EN_BOSS_DONGKU_WEARY_LIMIT);
		delta_hp = 0;
	}
	else if (delta_hp < 0)
	{
		std::map<int, ActiveBossInfo>::iterator iter = m_activeboss_info_map.find(scene_id);
		if (m_activeboss_info_map.end() != iter)
		{
			{
				int attacker_uid = attacker->GetUID();
				std::map<int, ActiveBossHurtRankInfo>::iterator hurt_rank_info_it = iter->second.hurt_rank_info_map.find(monster->GetMonsterId());
				if (hurt_rank_info_it != iter->second.hurt_rank_info_map.end())
				{
					bool is_find = false;
					ActiveBossHurtRankInfo &rank_info = hurt_rank_info_it->second;
					rank_info.rank_data_dirty_flag = 1;
					for (std::vector<BossHurtInfo>::iterator hurt_info_it = rank_info.hurt_info_list.begin(); hurt_info_it != rank_info.hurt_info_list.end(); ++hurt_info_it)
					{
						if (hurt_info_it->uid == attacker_uid)
						{
							hurt_info_it->hurt += -delta_hp;
							if (hurt_info_it->hurt < 0)
							{
								hurt_info_it->hurt = INT_MAX;
							}

							is_find = true;
							break;
						}
					}

					if (!is_find)
					{
						BossHurtInfo hurt_info;
						hurt_info.uid = attacker_uid;
						attacker->GetName(hurt_info.name);
						hurt_info.hurt = -delta_hp;

						rank_info.hurt_info_list.emplace_back(hurt_info);
					}
				}
			}

// 			std::map<int, BossInfo>::iterator active_boss_it = iter->second.boss_info_map.find(monster->GetMonsterId());
// 			if (iter->second.boss_info_map.end() != active_boss_it)
// 			{
// 				BossInfo &boss_info = active_boss_it->second;
// 
// 				long long cur_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
// 				cur_hp += delta_hp;
// 				long long max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
// 				int left_hp_per = static_cast<int>(cur_hp * 10000 / max_hp);
// 
// 				if (left_hp_per < other_cfg.first_trigger_hudun_hp_per && 0 == (boss_info.boss_shield_status & 1))
// 				{
// 					boss_info.boss_shield_status |= 1;
// 
// 					EffectSpecialShield *effect_special_shield = new EffectSpecialShield(monster->GetId(), 1, other_cfg.shield_continue_time * 1000, other_cfg.shield_effective_times, 1);
// 					effect_special_shield->SetFixHurt(other_cfg.shield_fix_hurt);
// 					monster->AddEffect(effect_special_shield);
// 				}
// 				else if (left_hp_per < other_cfg.second_trigger_hudun_hp_per && (boss_info.boss_shield_status & 1) != 0 && 0 == (boss_info.boss_shield_status & (1 << 1)))
// 				{
// 					boss_info.boss_shield_status |= 1 << 1;
// 
// 					EffectSpecialShield *effect_special_shield = new EffectSpecialShield(monster->GetId(), 1, other_cfg.shield_continue_time * 1000, other_cfg.shield_effective_times, 1);
// 					effect_special_shield->SetFixHurt(other_cfg.shield_fix_hurt);
// 					monster->AddEffect(effect_special_shield);
// 				}
// 			}
		}

		{
			attacker->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_CAUSE_HARM_ON_BOSS, -delta_hp, COND_ACTIVE_BOSS);
			attacker->GetRoleGoal()->CheckCond(COND_TYPE_CAUSE_HARM_ON_BOSS, -delta_hp, COND_ACTIVE_BOSS);
		}
	}
}

void BossFamilyManager::SendActiveBossHurtRankInfo(Monster *monster )
{

	struct UserInfo
	{
		UserInfo() : hurt(0), rank(0) {}

		long long hurt;
		int rank;
	};

	static const int HURT_RESERVE_LOGOUT_TIME_S = 60;  // 角色掉线后伤害排行数据保留时间
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	static Protocol::SCActiveBossHurtRankInfo abhri;
	if (nullptr == monster)
	{
		for (ActiveBossMapIt iter = m_activeboss_info_map.begin(); m_activeboss_info_map.end() != iter; ++iter)
		{
			Scene * scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(iter->first,0);
			if(nullptr == scene) continue;
			std::map<int, ActiveBossHurtRankInfo>::iterator hurt_rank_info_it = iter->second.hurt_rank_info_map.begin();
			for (; iter->second.hurt_rank_info_map.end()!= hurt_rank_info_it;++hurt_rank_info_it)
			{
				std::map<int, BossInfo>::iterator boss_info_it = iter->second.boss_info_map.find(hurt_rank_info_it->first);
				if(iter->second.boss_info_map.end() == boss_info_it) continue;

				abhri.monster_id = hurt_rank_info_it->first;
				abhri.rank_count = 0;
				abhri.my_hurt = 0;
				abhri.my_rank = SHRT_MAX;

				static std::map<int, UserInfo> temp_hurt_map; temp_hurt_map.clear();

				for (auto hurt_info_it = hurt_rank_info_it->second.hurt_info_list.begin(); hurt_info_it != hurt_rank_info_it->second.hurt_info_list.end(); ++hurt_info_it)
				{
					UserInfo &user_info = temp_hurt_map[hurt_info_it->uid];
					user_info.hurt = hurt_info_it->hurt;
					user_info.rank = SHRT_MAX;

					if (abhri.rank_count >= Protocol::SCActiveBossHurtRankInfo::MAX_RANK_COUNT) continue;

					F_STRNCPY(abhri.rank_info_list[abhri.rank_count].name, hurt_info_it->name, sizeof(hurt_info_it->name));
					abhri.rank_info_list[abhri.rank_count].hurt = hurt_info_it->hurt;
					++abhri.rank_count;
					user_info.rank = abhri.rank_count;
				}

				for (auto user_it = hurt_rank_info_it->second.send_info_user.begin(); user_it != hurt_rank_info_it->second.send_info_user.end(); ++user_it)
				{
					user_it->second = 0;
				}
				const int MAX_GET_ROLE = 256;
				ObjID obj_list[MAX_GET_ROLE] = { 0 };
				int count = scene->GetZoneMatrix()->GetObjByCircleArea(boss_info_it->second.born_pos, ACTIVE_BOSS_RANG_INFO, obj_list, sizeof(obj_list) / sizeof(obj_list[0]));
				for (int i = 0; i < count && i < MAX_GET_ROLE; ++i)
				{
					Role * role = scene->GetPrincipal(obj_list[i]);
					if (nullptr != role)
					{
						if ( hurt_rank_info_it->second.send_info_user.end() != hurt_rank_info_it->second.send_info_user.find(role->GetUID()))
						{
							hurt_rank_info_it->second.send_info_user[role->GetUID()] = 1;
							continue;
						}

						auto tmp_itr = temp_hurt_map.find(role->GetUID());
						if (temp_hurt_map.end() != tmp_itr)
						{
							abhri.my_hurt = tmp_itr->second.hurt;
							abhri.my_rank = tmp_itr->second.rank;
						}
						else
						{
							abhri.my_hurt = 0;
							abhri.my_rank = SHRT_MAX;
						}
						EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&abhri, sizeof(abhri));
						hurt_rank_info_it->second.send_info_user[role->GetUID()] = 1;
					}
				}

				for (auto user_it = hurt_rank_info_it->second.send_info_user.begin(); user_it != hurt_rank_info_it->second.send_info_user.end();)
				{
					if (0 == user_it->second) // 这个里面的都是已经离开区域的，只有一个不能删除的理由就是掉线保留
					{
						Protocol::SCActiveBossLeaveInfo acbli;
						Role *role = scene->GetRoleByUID(user_it->first);
						if (nullptr != role)
						{
							EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&acbli, sizeof(acbli));
						}
						bool is_offline_reserve = false;
						for (auto hurt_it = hurt_rank_info_it->second.hurt_info_list.begin(); hurt_it != hurt_rank_info_it->second.hurt_info_list.end(); ++hurt_it)
						{
							if (hurt_it->uid == user_it->first)
							{
								if (nullptr == role && hurt_it->logouttime > 0 && now < HURT_RESERVE_LOGOUT_TIME_S + hurt_it->logouttime)
								{
									// 角色不在场景，并且处于保留时间
									is_offline_reserve = true;
									break;
								}

								hurt_rank_info_it->second.hurt_info_list.erase(hurt_it);
								break;
							}
						}
						if (!is_offline_reserve)
						{
							hurt_rank_info_it->second.send_info_user.erase(user_it++);
						}
						else
						{
							++user_it;
						}
					}
					else {
						++user_it;
					}
				}
			}
		}
	}
	else
	{
		Scene *scene = monster->GetScene();
		ActiveBossMapIt iter = m_activeboss_info_map.find(scene->GetSceneID());
		if (m_activeboss_info_map.end() == iter)
		{
			return;
		}
		std::map<int, ActiveBossHurtRankInfo>::iterator hurt_rank_info_it = iter->second.hurt_rank_info_map.find(monster->GetMonsterId());
		std::map<int, BossInfo>::iterator boss_info_it = iter->second.boss_info_map.find(monster->GetMonsterId());
		if (iter->second.hurt_rank_info_map.end() == hurt_rank_info_it || iter->second.boss_info_map.end() == boss_info_it)
		{
			return;
		}

		abhri.monster_id = hurt_rank_info_it->first;
		abhri.rank_count = 0;
		abhri.my_hurt = 0;
		abhri.my_rank = SHRT_MAX;

		static std::map<int, UserInfo> temp_hurt_map; temp_hurt_map.clear();

		for (auto hurt_info_it = hurt_rank_info_it->second.hurt_info_list.begin(); hurt_info_it != hurt_rank_info_it->second.hurt_info_list.end(); ++hurt_info_it)
		{
			UserInfo &user_info = temp_hurt_map[hurt_info_it->uid];

			user_info.hurt = hurt_info_it->hurt;
			user_info.rank = SHRT_MAX;

			if (abhri.rank_count >= Protocol::SCActiveBossHurtRankInfo::MAX_RANK_COUNT) continue;

			F_STRNCPY(abhri.rank_info_list[abhri.rank_count].name, hurt_info_it->name, sizeof(hurt_info_it->name));
			abhri.rank_info_list[abhri.rank_count].hurt = hurt_info_it->hurt;
			++abhri.rank_count;

			user_info.rank = abhri.rank_count;
		}

		for (auto user_it = hurt_rank_info_it->second.send_info_user.begin(); user_it != hurt_rank_info_it->second.send_info_user.end(); ++user_it)
		{
			user_it->second = 0;
		}
		const int MAX_GET_ROLE = 256;
		ObjID obj_list[MAX_GET_ROLE] = { 0 };
		int count = scene->GetZoneMatrix()->GetObjByCircleArea(boss_info_it->second.born_pos, ACTIVE_BOSS_RANG_INFO, obj_list, sizeof(obj_list) / sizeof(obj_list[0]));
		for (int i = 0; i < count && i < MAX_GET_ROLE; ++i)
		{
			Role * role = scene->GetPrincipal(obj_list[i]);
			if (nullptr != role )
			{
				auto tmp_itr = temp_hurt_map.find(role->GetUID());
				if (temp_hurt_map.end() != tmp_itr)
				{
					abhri.my_hurt = tmp_itr->second.hurt;
					abhri.my_rank = tmp_itr->second.rank;
				}
				else
				{
					abhri.my_hurt = 0;
					abhri.my_rank = SHRT_MAX;
				}
				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&abhri, sizeof(abhri));
				hurt_rank_info_it->second.send_info_user[role->GetUID()] = 1;
			}
		}

		for (auto user_it = hurt_rank_info_it->second.send_info_user.begin(); user_it != hurt_rank_info_it->second.send_info_user.end();)
		{
			if (0 == user_it->second)  // 这个里面的都是已经离开区域的，只有一个不能删除的理由就是掉线保留
			{
				Protocol::SCActiveBossLeaveInfo acbli;
				Role *role = scene->GetRoleByUID(user_it->first);
				if (nullptr != role)
				{
					EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&acbli, sizeof(acbli));
				}
				bool is_offline_reserve = false;
				for (auto hurt_it = hurt_rank_info_it->second.hurt_info_list.begin(); hurt_it != hurt_rank_info_it->second.hurt_info_list.end(); ++hurt_it)
				{
					if (hurt_it->uid == user_it->first)
					{
						if (nullptr == role && hurt_it->logouttime > 0 && now < HURT_RESERVE_LOGOUT_TIME_S + hurt_it->logouttime)
						{
							// 角色不在场景，并且处于保留时间
							is_offline_reserve = true;
							break;
						}

						hurt_rank_info_it->second.hurt_info_list.erase(hurt_it);
						break;
					}
				}
				if (!is_offline_reserve)
				{
					hurt_rank_info_it->second.send_info_user.erase(user_it++);
				}
				else
				{
					++user_it;
				}
			}
			else {
				++user_it;
			}
		}
	}
}

void BossFamilyManager::SendMikuBossHurtRankInfo(Monster *monster)
{

	struct UserInfo
	{
		UserInfo() : hurt(0), rank(0) {}

		long long hurt;
		int rank;
	};

	static Protocol::SCMikuBossHurtRankInfo abhri;
	if (nullptr == monster)
	{
		for (MikuBossMapIt iter = m_mikuboss_info_map.begin(); m_mikuboss_info_map.end() != iter; ++iter)
		{
			Scene * scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(iter->first, 0);
			if (nullptr == scene) continue;
			std::map<int, ActiveBossHurtRankInfo>::iterator hurt_rank_info_it = iter->second.hurt_rank_info_map.begin();
			for (; iter->second.hurt_rank_info_map.end() != hurt_rank_info_it; ++hurt_rank_info_it)
			{
				std::map<int, BossInfo>::iterator boss_info_it = iter->second.boss_info_map.find(hurt_rank_info_it->first);
				if (iter->second.boss_info_map.end() == boss_info_it) continue;

				abhri.monster_id = hurt_rank_info_it->first;
				abhri.rank_count = 0;
				abhri.my_hurt = 0;
				abhri.my_rank = SHRT_MAX;

				static std::map<int, UserInfo> temp_hurt_map; temp_hurt_map.clear();

				for (auto hurt_info_it = hurt_rank_info_it->second.hurt_info_list.begin(); hurt_info_it != hurt_rank_info_it->second.hurt_info_list.end(); ++hurt_info_it)
				{
					UserInfo &user_info = temp_hurt_map[hurt_info_it->uid];
					user_info.hurt = hurt_info_it->hurt;
					user_info.rank = SHRT_MAX;

					if (abhri.rank_count >= Protocol::SCMikuBossHurtRankInfo::MAX_RANK_COUNT) continue;

					F_STRNCPY(abhri.rank_info_list[abhri.rank_count].name, hurt_info_it->name, sizeof(hurt_info_it->name));
					abhri.rank_info_list[abhri.rank_count].hurt = hurt_info_it->hurt;
					++abhri.rank_count;

					user_info.rank = abhri.rank_count;
				}

				for (auto user_it = hurt_rank_info_it->second.send_info_user.begin(); user_it != hurt_rank_info_it->second.send_info_user.end(); ++user_it)
				{
					user_it->second = 0;
				}
				const int MAX_GET_ROLE = 256;
				ObjID obj_list[MAX_GET_ROLE] = { 0 };
				int count = scene->GetZoneMatrix()->GetObjByCircleArea(boss_info_it->second.born_pos, MIKU_BOSS_RANG_INFO, obj_list, sizeof(obj_list) / sizeof(obj_list[0]));
				for (int i = 0; i < count && i < MAX_GET_ROLE; ++i)
				{
					Role * role = scene->GetPrincipal(obj_list[i]);
					if (nullptr != role)
					{
						if (hurt_rank_info_it->second.send_info_user.end() != hurt_rank_info_it->second.send_info_user.find(role->GetUID()))
						{
							hurt_rank_info_it->second.send_info_user[role->GetUID()] = 1;
							continue;
						}

						auto tmp_itr = temp_hurt_map.find(role->GetUID());
						if (temp_hurt_map.end() != tmp_itr)
						{
							abhri.my_hurt = tmp_itr->second.hurt;
							abhri.my_rank = tmp_itr->second.rank;
						}
						else
						{
							abhri.my_hurt = 0;
							abhri.my_rank = SHRT_MAX;
						}
						EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&abhri, sizeof(abhri));
						hurt_rank_info_it->second.send_info_user[role->GetUID()] = 1;
					}
				}

				for (auto user_it = hurt_rank_info_it->second.send_info_user.begin(); user_it != hurt_rank_info_it->second.send_info_user.end();)
				{
					if (0 == user_it->second)
					{
						Protocol::SCMikuBossLeaveInfo acbli;
						Role *role = scene->GetRoleByUID(user_it->first);
						if (nullptr != role)
						{
							EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&acbli, sizeof(acbli));
						}
						for (auto hurt_it = hurt_rank_info_it->second.hurt_info_list.begin(); hurt_it != hurt_rank_info_it->second.hurt_info_list.end(); ++hurt_it)
						{
							if (hurt_it->uid == user_it->first)
							{
								hurt_rank_info_it->second.hurt_info_list.erase(hurt_it);
								break;
							}
						}
						hurt_rank_info_it->second.send_info_user.erase(user_it++);
					}
					else {
						++user_it;
					}
				}
			}
		}
	}
	else
	{
		Scene *scene = monster->GetScene();
		MikuBossMapIt iter = m_mikuboss_info_map.find(scene->GetSceneID());
		if (m_mikuboss_info_map.end() == iter)
		{
			return;
		}
		std::map<int, ActiveBossHurtRankInfo>::iterator hurt_rank_info_it = iter->second.hurt_rank_info_map.find(monster->GetMonsterId());
		std::map<int, BossInfo>::iterator boss_info_it = iter->second.boss_info_map.find(monster->GetMonsterId());
		if (iter->second.hurt_rank_info_map.end() == hurt_rank_info_it || iter->second.boss_info_map.end() == boss_info_it)
		{
			return;
		}

		abhri.monster_id = hurt_rank_info_it->first;
		abhri.rank_count = 0;
		abhri.my_hurt = 0;
		abhri.my_rank = SHRT_MAX;

		static std::map<int, UserInfo> temp_hurt_map; temp_hurt_map.clear();

		for (auto hurt_info_it = hurt_rank_info_it->second.hurt_info_list.begin(); hurt_info_it != hurt_rank_info_it->second.hurt_info_list.end(); ++hurt_info_it)
		{
			UserInfo &user_info = temp_hurt_map[hurt_info_it->uid];
			user_info.hurt = hurt_info_it->hurt;
			user_info.rank = SHRT_MAX;

			if (abhri.rank_count >= Protocol::SCMikuBossHurtRankInfo::MAX_RANK_COUNT) continue;

			F_STRNCPY(abhri.rank_info_list[abhri.rank_count].name, hurt_info_it->name, sizeof(hurt_info_it->name));
			abhri.rank_info_list[abhri.rank_count].hurt = hurt_info_it->hurt;
			++abhri.rank_count;

			user_info.rank = abhri.rank_count;
		}

		for (auto user_it = hurt_rank_info_it->second.send_info_user.begin(); user_it != hurt_rank_info_it->second.send_info_user.end(); ++user_it)
		{
			user_it->second = 0;
		}
		const int MAX_GET_ROLE = 256;
		ObjID obj_list[MAX_GET_ROLE] = { 0 };
		int count = scene->GetZoneMatrix()->GetObjByCircleArea(boss_info_it->second.born_pos, MIKU_BOSS_RANG_INFO, obj_list, sizeof(obj_list) / sizeof(obj_list[0]));
		for (int i = 0; i < count && i < MAX_GET_ROLE; ++i)
		{
			Role * role = scene->GetPrincipal(obj_list[i]);
			if (nullptr != role)
			{
				auto tmp_itr = temp_hurt_map.find(role->GetUID());
				if (temp_hurt_map.end() != tmp_itr)
				{
					abhri.my_hurt = tmp_itr->second.hurt;
					abhri.my_rank = tmp_itr->second.rank;
				}
				else
				{
					abhri.my_hurt = 0;
					abhri.my_rank = SHRT_MAX;
				}
				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&abhri, sizeof(abhri));
				hurt_rank_info_it->second.send_info_user[role->GetUID()] = 1;
			}
		}

		for (auto user_it = hurt_rank_info_it->second.send_info_user.begin(); user_it != hurt_rank_info_it->second.send_info_user.end();)
		{
			if (0 == user_it->second)
			{
				Protocol::SCMikuBossLeaveInfo acbli;
				Role *role = scene->GetRoleByUID(user_it->first);
				if (nullptr != role)
				{
					EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&acbli, sizeof(acbli));
				}
				for (auto hurt_it = hurt_rank_info_it->second.hurt_info_list.begin(); hurt_it != hurt_rank_info_it->second.hurt_info_list.end(); ++hurt_it)
				{
					if (hurt_it->uid == user_it->first)
					{
						hurt_rank_info_it->second.hurt_info_list.erase(hurt_it);
						break;
					}
				}
				hurt_rank_info_it->second.send_info_user.erase(user_it++);
			}
			else {
				++user_it;
			}
		}
	}
}

void BossFamilyManager::HiddenServerMonsterBeKill(Role *killer, Monster *monster)
{
	if (nullptr == killer || nullptr == monster)
	{
		return;
	}

	int scene_id = monster->GetScene()->GetSceneID();
	int monster_id = monster->GetMonsterId();

	// 同步到common gameworld
	crossgameprotocal::CrossGameCrossCommonBossInfoChange ccbsi;

	{
		ccbsi.boss_type = Protocol::BOSS_ENTER_TYPE_IVNALID;
		ccbsi.reason_type = crossgameprotocal::CROSS_COMMON_BOSS_SYNC_REASON_TYPE_DIE;
		ccbsi.scene_id = scene_id;
		ccbsi.boss_id = monster_id;
		ccbsi.killer.uid = killer->GetUID();
		ccbsi.killer.kill_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		killer->GetName(ccbsi.killer.name);
	}

	auto family_it = m_cross_familyboss_info_map.find(scene_id);
	if (family_it != m_cross_familyboss_info_map.end())
	{
		auto monster_it = family_it->second.boss_info_map.find(monster_id);
		if (monster_it != family_it->second.boss_info_map.end())
		{
			BossInfo &boss_info = monster_it->second;
			
			this->UpdateBossKillerInfo(&boss_info, killer);

			this->SendBossInfoToAllGateway(boss_info, Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY, Protocol::NOTIFY_REASON_BOSS_DIE, killer->GetUID());
			this->SendFamilyBossInfoToAll(boss_info.scene_id);

			ccbsi.boss_type = Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY;
			ccbsi.next_refresh_timestamp = boss_info.next_refresh_time;
		}
	}

	if (ccbsi.boss_type != Protocol::BOSS_ENTER_TYPE_IVNALID)
	{
		InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_COMMON, 0, (const char *)&ccbsi, sizeof(ccbsi));
	}
}

void BossFamilyManager::OnYunyouMonsterBeKill(Monster *monster, Role *role, int scene_id)
{
// 	if (NULL == role || NULL == monster) return;
// 
// 	// 更新云游boss信息
// 	{
// 		YunyouBossInfo info;
// 		info.scene_id = scene_id;
// 		info.boss_id = monster->GetMonsterId();
// 		info.born_pos_x = monster->GetPos().x;
// 		info.born_pos_y = monster->GetPos().y;
// 		info.is_dead = true;
// 
// 		this->UpdateYunyouBossInfo(info);
// 	}
}

void BossFamilyManager::OnEventDropOwnerWithMonsterDie(Role *drop_owner, Monster *monster)
{
	if (nullptr == drop_owner || nullptr == monster) return;
	Scene *scene = monster->GetScene();
	if (nullptr == scene) return;

	unsigned short monster_id = monster->GetMonsterId();
	ObjID monster_objid = monster->GetId();
	int scene_id = scene->GetSceneID();
	long long temp_scene_id = scene_id;
	long long monster_set_id = (long long)((temp_scene_id << 32) + monster_objid);

	if (Scene::IsBossDongKuScene(scene->GetSceneID()))
	{
		if (m_mikuboss_set.end() != m_mikuboss_set.find(monster_set_id))
		{
			if (m_mikuboss_info_map.end() != m_mikuboss_info_map.find(scene_id))
			{
				MikuBossInfo& miku_boss = m_mikuboss_info_map[scene_id];

				if (miku_boss.boss_info_map.end() != miku_boss.boss_info_map.find(monster_id))
				{
					drop_owner->AddKillBossNum(MonsterInitParam::BOSS_TYPE_TASK_MIKU, 1);

					const BossFamilyMikuBossInfoCfg * miku_boss_info_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetMikuBossInfo(monster->GetMonsterId());
					if ((nullptr != miku_boss_info_cfg) && (1 != miku_boss_info_cfg->is_peace))  // 不是和平层才能增加投资计划
					{
						drop_owner->GetTouzijihua()->OnKillBoss();
					}
				}
			}
		}
	}

	if (Scene::IsActiveBossScene(scene->GetSceneID()))
	{
		std::map<int, ActiveBossInfo>::iterator iter = m_activeboss_info_map.find(scene_id);
		if (m_activeboss_info_map.end() != iter)
		{
			std::map<int, BossInfo>::iterator active_boss_it = iter->second.boss_info_map.find(monster->GetMonsterId());
			if (iter->second.boss_info_map.end() != active_boss_it && ENBOSSSTATUS_EXIST == active_boss_it->second.boss_status)
			{
				drop_owner->AddKillBossNum(MonsterInitParam::BOSS_TYPE_TASK_ACTIVE, 1);
			}
		}
	}
}

void BossFamilyManager::OnCrossCommonBossInfoChange(crossgameprotocal::CrossGameCrossCommonBossInfoChange *sccbi)
{
	if (CrossConfig::Instance().IsHiddenServer()) return;

	if (Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY == sccbi->boss_type)
	{
		if (crossgameprotocal::CROSS_COMMON_BOSS_SYNC_REASON_TYPE_BORN == sccbi->reason_type)
		{
			FamilyBossInfo &family_boss_scene_info = m_cross_familyboss_info_map[sccbi->scene_id];
			BossInfo &family_boss = family_boss_scene_info.boss_info_map[sccbi->boss_id];

			family_boss.scene_id = sccbi->scene_id;
			family_boss.boss_id = sccbi->boss_id;
			family_boss.boss_status = ENBOSSSTATUS_EXIST;
			family_boss.next_refresh_time = 0;

			this->SendBossInfoToAllGateway(family_boss, Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY, Protocol::NOTIFY_REASON_BOSS_REFRESH);
		}
		else if (crossgameprotocal::CROSS_COMMON_BOSS_SYNC_REASON_TYPE_DIE == sccbi->reason_type)
		{
			BossFamilyBossMapIt iter = m_cross_familyboss_info_map.find(sccbi->scene_id);
			if (iter != m_cross_familyboss_info_map.end())
			{
				std::map<int, BossInfo>::iterator boss_it = iter->second.boss_info_map.find(sccbi->boss_id);
				if (boss_it != iter->second.boss_info_map.end())
				{
					BossInfo &boss_info = boss_it->second;
					boss_info.boss_status = ENBOSSSTATUS_NONEXIST;
					boss_info.next_refresh_time = sccbi->next_refresh_timestamp;

					static KillerInfo killer_info;

					killer_info.killer_uid = sccbi->killer.uid;
					killer_info.killier_time = sccbi->killer.kill_timestamp;
					F_STRNCPY(killer_info.killer_name, sccbi->killer.name, sizeof(GameName));

					this->UpdateBossKillerInfo(&boss_info, &killer_info);

					this->SendBossInfoToAllGateway(boss_info, Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY, Protocol::NOTIFY_REASON_BOSS_DIE, killer_info.killer_uid);
				}
			}
		}

		this->SendFamilyBossInfoToAll(sccbi->scene_id);
	}
}

void BossFamilyManager::UpdateBossKillerInfo(BossInfo *boss_info, KillerInfo *killer_info)
{
	if (nullptr == boss_info || nullptr == killer_info) return;

	unsigned int min_kill_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	int killer_index = -1;
	for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
	{
		if (0 == boss_info->killer_info_list[i].killer_uid)
		{
			boss_info->killer_info_list[i].killer_uid = killer_info->killer_uid;
			boss_info->killer_info_list[i].killier_time = killer_info->killier_time;
			F_STRNCPY(boss_info->killer_info_list[i].killer_name, killer_info->killer_name, sizeof(GameName));
			killer_index = -1;
			break;
		}
		else
		{
			if (boss_info->killer_info_list[i].killier_time < min_kill_time)
			{
				min_kill_time = boss_info->killer_info_list[i].killier_time;
				killer_index = i;
			}
		}
	}

	if (0 <= killer_index && killer_index < KILLER_LIST_MAX_COUNT)
	{
		boss_info->killer_info_list[killer_index].killer_uid = killer_info->killer_uid;
		boss_info->killer_info_list[killer_index].killier_time = killer_info->killier_time;
		F_STRNCPY(boss_info->killer_info_list[killer_index].killer_name, killer_info->killer_name, sizeof(GameName));
	}
}

void BossFamilyManager::UpdateBossKillerInfo(BossInfo *boss_info, Role *killer)
{
	if (nullptr == boss_info || nullptr == killer) return;

	unsigned int min_kill_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	int killer_index = -1;
	for (int i = 0; i < KILLER_LIST_MAX_COUNT; i++)
	{
		if (0 == boss_info->killer_info_list[i].killer_uid)
		{
			boss_info->killer_info_list[i].killer_uid = killer->GetUID();
			boss_info->killer_info_list[i].killier_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			killer->GetName(boss_info->killer_info_list[i].killer_name);

			killer_index = -1;
			break;
		}
		else
		{
			if (boss_info->killer_info_list[i].killier_time < min_kill_time)
			{
				min_kill_time = boss_info->killer_info_list[i].killier_time;
				killer_index = i;
			}
		}
	}

	if (0 <= killer_index && killer_index < KILLER_LIST_MAX_COUNT)
	{
		boss_info->killer_info_list[killer_index].killer_uid = killer->GetUID();
		boss_info->killer_info_list[killer_index].killier_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		killer->GetName(boss_info->killer_info_list[killer_index].killer_name);
	}
}

void BossFamilyManager::OnCrossCommonBossInfoSyncReq(NetID net_id)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	// 每种boss分开同步，防止数量多了同步不全
	static crossgameprotocal::CrossGameSyncCrossCommonBossInfo sccbi;
	sccbi.boss_count = 0;

	bool is_full = false;

	{
		sccbi.boss_type = Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY;

		for (BossFamilyBossMapIt it = m_cross_familyboss_info_map.begin(); it != m_cross_familyboss_info_map.end(); ++it)
		{
			if (is_full)
			{
				break;
			}

			for (std::map<int, BossInfo>::iterator boss_it = it->second.boss_info_map.begin(); boss_it != it->second.boss_info_map.end(); ++boss_it)
			{
				if (sccbi.boss_count >= crossgameprotocal::CrossGameSyncCrossCommonBossInfo::BOSS_MAX_COUNT)
				{
					is_full = true;
					break;
				}

				crossgameprotocal::CrossGameSyncCrossCommonBossInfo::BossInfo &boss_info = sccbi.boss_info_list[sccbi.boss_count];

				boss_info.scene_id = boss_it->second.scene_id;
				boss_info.boss_id = boss_it->second.boss_id;
				boss_info.boss_status = static_cast<char>(boss_it->second.boss_status);
				boss_info.next_refresh_timestamp = boss_it->second.next_refresh_time;

				for (int i = 0; i < KILLER_LIST_MAX_COUNT && i < crossgameprotocal::CrossGameSyncCrossCommonBossInfo::KILLER_INFO_MAX_COUNT; ++i)
				{
					boss_info.killer_info_list[i].killer_uid = boss_it->second.killer_info_list[i].killer_uid;
					boss_info.killer_info_list[i].kill_timestamp = boss_it->second.killer_info_list[i].killier_time;
					F_STRNCPY(boss_info.killer_info_list[i].killer_name, boss_it->second.killer_info_list[i].killer_name, sizeof(GameName));
				}

				++sccbi.boss_count;
			}

			if (sccbi.boss_count > 0)
			{
				int send_len = UNSTD_OFFSETOFF(crossgameprotocal::CrossGameSyncCrossCommonBossInfo, boss_info_list) + sizeof(sccbi.boss_info_list[0]) * sccbi.boss_count;
				InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_GLOBAL_NETID, net_id, (const char *)&sccbi, send_len);
			}
		}
	}
}

void BossFamilyManager::OnSyncCrossCommonBossInfo(crossgameprotocal::CrossGameSyncCrossCommonBossInfo *sccbi)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	for (int i = 0; i < sccbi->boss_count && i < crossgameprotocal::CrossGameSyncCrossCommonBossInfo::BOSS_MAX_COUNT; ++i)
	{
		int scene_id = sccbi->boss_info_list[i].scene_id;
		int boss_id = sccbi->boss_info_list[i].boss_id;

		if (Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY == sccbi->boss_type)
		{
			FamilyBossInfo &family_boss_scene_info = m_cross_familyboss_info_map[scene_id];
			BossInfo &boss_info = family_boss_scene_info.boss_info_map[boss_id];

			boss_info.scene_id = scene_id;
			boss_info.boss_id = boss_id;
			boss_info.boss_status = sccbi->boss_info_list[i].boss_status;
			boss_info.next_refresh_time = sccbi->boss_info_list[i].next_refresh_timestamp;

			for (int j = 0; j < KILLER_LIST_MAX_COUNT && j < crossgameprotocal::CrossGameSyncCrossCommonBossInfo::KILLER_INFO_MAX_COUNT; ++j)
			{
				boss_info.killer_info_list[j].killer_uid = sccbi->boss_info_list[i].killer_info_list[j].killer_uid;
				boss_info.killer_info_list[j].killier_time = sccbi->boss_info_list[i].killer_info_list[j].kill_timestamp;
				F_STRNCPY(boss_info.killer_info_list[j].killer_name, sccbi->boss_info_list[i].killer_info_list[j].killer_name, sizeof(GameName));
			}
		}
	}

	// 同步完通知玩家
	if (Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY == sccbi->boss_type)
	{
		for (BossFamilyBossMapIt scene_it = m_cross_familyboss_info_map.begin(); scene_it != m_cross_familyboss_info_map.end(); ++scene_it)
		{
			this->SendFamilyBossInfoToAll(scene_it->first);
		}
	}
}

void BossFamilyManager::OnYunyouBossOperate(Role *role, int operate_type, int param_0)
{
	if (NULL == role) return;

	switch (operate_type)
	{

	case Protocol::CSYunyouBossOperate::TYPE_BOSS_INFO_REQ:
		{
			this->SendYunyouBossInfo(role, param_0);
		}
		break;

	case Protocol::CSYunyouBossOperate::TYPE_BOSS_COUNT_ALL_SCENE:
		{
			this->SendYunyouAllSceneBossInfo(role);
		}
		break;
	}
}

void BossFamilyManager::RefreshAllYunyouBoss()
{
	const std::vector<YunyouBossBornCfg> &yunyou_boss_vec = LOGIC_CONFIG->GetBossFaimlyConfig().GetYunyouBossAllBornCfg();
	if (yunyou_boss_vec.empty())
	{
		return;
	}

	YunyouBossInfo info;
	for (auto &it : yunyou_boss_vec)
	{
		info.scene_id = it.scene_id;
		info.boss_id = it.boss_id;
		info.born_pos_x = it.born_pos.GetPos().x;
		info.born_pos_y = it.born_pos.GetPos().y;
		info.is_dead = false;

		if (this->IsYunyouBossDead(it.scene_id, it.boss_id))
		{
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(it.scene_id, 0);
			if (NULL != scene)
			{
				MONSTERPOOL->CreateMonster(it.boss_id, scene, it.born_pos.GetPos());
			}
		}

		this->UpdateYunyouBossInfo(info);
	}

	//传闻
// 	int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "yunyou_boss_refresh_notice");
// 	if (length > 0)
// 	{
// 		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL);
// 	}
}

void BossFamilyManager::UpdateYunyouBossInfo(const YunyouBossInfo &info)
{
	auto it = m_yunyou_boss_info_map.find(info.scene_id);
	if (it != m_yunyou_boss_info_map.end())
	{
		auto vec_it = std::find_if(it->second.begin(), it->second.end(), [info](const YunyouBossInfo &tmp_info) {
			return (tmp_info.scene_id == info.scene_id && tmp_info.boss_id == info.boss_id);
		
		});
		if (vec_it != it->second.end())
		{
			vec_it->is_dead = info.is_dead;
			vec_it->born_pos_x = info.born_pos_x;
			vec_it->born_pos_y = info.born_pos_y;
		}
		else
		{
			it->second.push_back(info);
		}
	}
	else
	{
		auto &boss_vec =  m_yunyou_boss_info_map[info.scene_id];
		boss_vec.push_back(info);
	}
}

void BossFamilyManager::SendYunyouBossInfo(Role *role, int scene_id)
{
	if (nullptr == role)
	{
		return;
	}

	static Protocol::SCYouyouBossInfo msg;
	msg.nex_refresh_time = m_next_refresh_yunyou_boss_time;
	msg.scene_id = scene_id;
	msg.boss_count = 0;
	memset(msg.boss_info_list, 0, sizeof(msg.boss_info_list));

	auto it = m_yunyou_boss_info_map.find(scene_id);
	if (it != m_yunyou_boss_info_map.end())
	{
		for (auto &vec_it : it->second)
		{
			if (msg.boss_count >= YUNYOU_BOSS_MAX_NUM_PER_SCENE)
			{
				break;
			}

			if (vec_it.is_dead)
			{
				continue;
			}

			Protocol::SCYouyouBossInfo::BossInfo &info = msg.boss_info_list[msg.boss_count];
			info.scene_id = vec_it.scene_id;
			info.boss_id = vec_it.boss_id;
			info.born_pos_x = vec_it.born_pos_x;
			info.born_pos_y = vec_it.born_pos_y;

			msg.boss_count++;
		}
	}

	int send_len = sizeof(msg) - (YUNYOU_BOSS_MAX_NUM_PER_SCENE - msg.boss_count) * sizeof(msg.boss_info_list[0]);

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&msg, send_len);
}

void BossFamilyManager::SendYunyouAllSceneBossInfo(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	static Protocol::SCYouyouSceneInfo scene_msg;
	scene_msg.scene_count = 0;
	memset(scene_msg.scene_info_list, 0, sizeof(scene_msg.scene_info_list));

	int boss_count = 0;
	for (auto &it : m_yunyou_boss_info_map)
	{
		if ((unsigned int)scene_msg.scene_count >= static_array_size(scene_msg.scene_info_list))
		{
			break;
		}

		boss_count = 0;
		for (auto &vec_it : it.second)
		{	
			if (!vec_it.is_dead)
			{
				boss_count ++;
			}
		}

		Protocol::SCYouyouSceneInfo::SceneInfo &scene_info = scene_msg.scene_info_list[scene_msg.scene_count];
		scene_info.scene_id = it.first;
		scene_info.boss_count = boss_count;

		scene_msg.scene_count++;
	}

	int send_len = sizeof(scene_msg) - (YUNYOU_BOSS_MAX_SCENE_NUM - scene_msg.scene_count) * sizeof(scene_msg.scene_info_list[0]);

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&scene_msg, send_len);
}

bool BossFamilyManager::IsYunyouBossDead(int scene_id, int boss_id)
{
	auto it = m_yunyou_boss_info_map.find(scene_id);
	if (it != m_yunyou_boss_info_map.end())
	{
		auto vec_it = std::find_if(it->second.begin(), it->second.end(), [scene_id, boss_id](const YunyouBossInfo &tmp_info) {
			return (tmp_info.scene_id == scene_id && tmp_info.boss_id == boss_id);

		});
		if (vec_it != it->second.end())
		{
			return vec_it->is_dead;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return true;
	}
}

int BossFamilyManager::GetBossSpecialFlushInterval(int boss_id)
{
	const BossSpecialRefreshCfg *refresh_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossSpecialRefreshCfgByBossId(boss_id);
	if (NULL == refresh_cfg)
	{
		return 0;
	}

	if (refresh_cfg->boss_id != boss_id)
	{
		return 0;
	}

	int min_level = refresh_cfg->min_role_level, max_level = refresh_cfg->max_role_level;
	if (min_level <= 0) min_level = 1;
	if (max_level > MAX_ROLE_LEVEL) max_level = MAX_ROLE_LEVEL;

	int interval = 0;
	int role_count = World::GetInstWorld()->GetSceneManager()->GetAllServerRoleLevelNum(min_level, max_level);
	for (int i = 0; i < refresh_cfg->item_count && i < BossSpecialRefreshCfg::ITEM_COUNT_MAX; i++)
	{
		if (role_count <= refresh_cfg->item_list[i].role_num)
		{
			interval = refresh_cfg->item_list[i].flush_time_s;
			break;
		}
	}

	return interval;
}

bool BossFamilyManager::IsDabaoBossTireFull(Role * role)
{
	if (nullptr == role)
	{
		return true;
	}
	//const BossFamilyOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyOtherCfg();

	int can_enter_count = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_DABAO_BOSS_ENTER_TIMES);
	if (role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_DABAO_ENTER_COUNT) >= can_enter_count)
	{
		role->NoticeNum(errornum::EN_ENTER_DABAO_MAP_COUNT_LIMIT);
		return true;
	}
	return false;
}

bool BossFamilyManager::OnUseBossRefreshCardActiveBoss(Role *role, const int boss_id /* = 0 */)
{
	if (nullptr == role || nullptr == role->GetScene())
	{
		return false;
	}
	
	int scene_id = role->GetScene()->GetSceneID();
//	Posi self_pos = role->GetPos();

	ActiveBossMapIt active_it = m_activeboss_info_map.find(scene_id);
	if (m_activeboss_info_map.end() == active_it)
	{
		return false;
	}

// 	if (boss_id < 0)  // 客户端搜索
// 	{
// 		// 搜索离自己最近的一只未刷新BOSS进行刷新
// 		int tmp_boss_id = 0;
// 		float min_distance = 9999999.0f;
// 
// 		std::map<int, BossInfo>::iterator active_boss = active_it->second.boss_info_map.begin();
// 		for (; active_it->second.boss_info_map.end() != active_boss; active_boss++)
// 		{
// 			if (ENBOSSSTATUS_NONEXIST == active_boss->second.boss_status)
// 			{
// 				float dist = active_boss->second.born_pos.Dist(self_pos);
// 				if (dist < min_distance)
// 				{
// 					min_distance = dist;
// 					tmp_boss_id = active_boss->first;
// 				}
// 			}
// 		}
// 
// 		static const int DISTANCE_LIMIT = 25;
// 		if ((int)min_distance > DISTANCE_LIMIT)
// 		{
// 			return false;
// 		}
// 	}

	if (boss_id <= 0)
	{
		return false;
	}

	std::map<int, BossInfo>::iterator active_boss = active_it->second.boss_info_map.find(boss_id);
	if (active_it->second.boss_info_map.end() != active_boss)
	{
		if (ENBOSSSTATUS_NONEXIST != active_boss->second.boss_status)
		{
			return false;
		}

		this->RefreshOneActiveBossInfo(active_boss->second);
		active_boss->second.has_notice = false;
		std::map<int, ActiveBossHurtRankInfo>::iterator hurt_info_it = active_it->second.hurt_rank_info_map.find(active_boss->first);
		if (active_it->second.hurt_rank_info_map.end() != hurt_info_it)
		{
			hurt_info_it->second.hurt_info_list.clear();
		}

		return true;
	}

	return false;
}

bool BossFamilyManager::OnUseBossRefreshCardMikuBoss(Role *role, const int boss_id /* = 0 */)
{
	if (nullptr == role || nullptr == role->GetScene())
	{
		return false;
	}

	int scene_id = role->GetScene()->GetSceneID();
//	Posi self_pos = role->GetPos();

	MikuBossMapIt miku_it = m_mikuboss_info_map.find(scene_id);
	if (m_mikuboss_info_map.end() == miku_it)
	{
		return false;
	}

// 	if (boss_id < 0)  // 客户端搜索
// 	{
// 		// 搜索离自己最近的一只未刷新BOSS进行刷新
// 		int tmp_boss_id = 0;
// 		float min_distance = 9999999.0f;
// 
// 		std::map<int, BossInfo>::iterator miku_boss = miku_it->second.boss_info_map.begin();
// 		for (; miku_it->second.boss_info_map.end() != miku_boss; miku_boss++)
// 		{
// 			if (ENBOSSSTATUS_NONEXIST == miku_boss->second.boss_status)
// 			{
// 				float dist = miku_boss->second.born_pos.Dist(self_pos);
// 				if (dist < min_distance)
// 				{
// 					min_distance = dist;
// 					tmp_boss_id = miku_boss->first;
// 				}
// 			}
// 		}
// 
// 		static const int DISTANCE_LIMIT = 25;
// 		if ((int)min_distance > DISTANCE_LIMIT)
// 		{
// 			return false;
// 		}
// 	}

	if (boss_id <= 0)
	{
		return false;
	}

	std::map<int, BossInfo>::iterator miku_boss = miku_it->second.boss_info_map.find(boss_id);
	if (miku_it->second.boss_info_map.end() != miku_boss)
	{
		if (ENBOSSSTATUS_NONEXIST != miku_boss->second.boss_status)
		{
			return false;
		}

		this->RefreshOneMikuBoss(miku_boss->second);
		miku_boss->second.has_notice = false;
		std::map<int, ActiveBossHurtRankInfo>::iterator hurt_info_it = miku_it->second.hurt_rank_info_map.find(miku_boss->first);
		if (miku_it->second.hurt_rank_info_map.end() != hurt_info_it)
		{
			hurt_info_it->second.hurt_info_list.clear();
		}

		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
// 同步等级区间人数的写在这里。SceneManager的Update是遍历场景的逻辑，暂时不放这个进去
void BossFamilyManager::SyncRoleNumToHidden()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	int plat_type = LocalConfig::Instance().GetPlatType();
	int server_id = LocalConfig::Instance().GetMergeServerId();

	crossgameprotocal::GameCrossSyncRoleNumToGame gcsrntg;
	memset(gcsrntg.role_num_list, 0, sizeof(gcsrntg.role_num_list));
	gcsrntg.unique_serverId = TranslateInt64(plat_type, server_id);
	gcsrntg.reserve_sh = 0;

	unsigned int yesterday_zero_time = (unsigned int)GetZeroTime(EngineAdapter::Instance().Time()) - SECOND_PER_DAY;
	const std::map<int, UserCacheNode> & user_cache_map = UserCacheManager::Instance().GetUserCacheMap();
	for (std::map<int, UserCacheNode>::const_iterator it_user = user_cache_map.begin(); it_user != user_cache_map.end(); it_user++)
	{
		int level = it_user->second.level;
		if (level > 0 && level <= MAX_ROLE_LEVEL)
		{
			if (it_user->second.last_logout_timestamp >= yesterday_zero_time || UserCacheManager::Instance().IsUserOnline(it_user->first))
			{
				if (gcsrntg.role_num_list[level] >= (short)20000) continue;

				gcsrntg.role_num_list[level]++;
			}
		}
	}

	InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_HIDDEN, 0, (const char *)&gcsrntg, sizeof(gcsrntg));
}

void BossFamilyManager::OnConnectCrossServerSucc()
{
	// 跨服连接成功，延迟当前时间20秒后同步服务器人数
	time_t now = EngineAdapter::Instance().Time();
	m_next_sync_role_num_time = now + 20;

}
