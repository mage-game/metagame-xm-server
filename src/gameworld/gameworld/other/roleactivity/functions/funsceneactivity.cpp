#include "world.h"
#include "engineadapter.h"
#include "gamecommon.h"
#include "obj/character/role.h"
#include "obj/otherobj/gatherobj.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "scene/guildshadow/guildshadow.hpp"
#include "scene/scene.h"
#include "scene/speciallogic/worldspecial/specialzhuxie.hpp"
#include "scene/speciallogic/worldspecial/specialqunxianluandou.hpp"
#include "scene/speciallogic/worldspecial/specialxianmengzhan.hpp"
#include "scene/speciallogic/worldspecial/specialnationalboss.hpp"
#include "scene/speciallogic/worldspecial/specialyizhandaodi.hpp"
#include "scene/speciallogic/worldspecial/specialwuxingguaji.hpp"
#include "scene/speciallogic/worldspecial/specialgongchengzhan.hpp"
#include "task/taskpool.h"
#include "other/route/mailroute.hpp"
#include "servercommon/errornum.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "globalconfig/globalconfig.h"
#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/activityconfig/guildbattleconfignew.hpp"
#include "config/activityconfig/xingzuoyijiconfig.hpp"
#include "config/activityconfig/gongchengzhanconfig.hpp"
#include "config/activityconfig/shuijingconfig.hpp"
#include "config/activityconfig/qunxianluandouconfig.hpp"
#include "protocal/msgactivity.hpp"
#include "gameworld/gamelog.h"
#include "global/guild/guildmanager.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "gameworld/protocal/msgfb.h"
#include "gameworld/gameworld/global/guild/guild.hpp"
#include "gameworld/gameworld/global/usercache/usercache.hpp"

#include "scene/speciallogic/worldspecial/specialterritorywar.hpp"
#include "config/activityconfig/yizhandaodiconfig.hpp"
#include "config/activityconfig/millionaireconfig.hpp"
#include "config/activityconfig/zhuxieconfig.hpp"
#include "config/activityconfig/tianjiangcaibaoconfig.h"
#include "config/activityconfig/wanglingexploreconfig.h"
#include "config/activityconfig/nationalbossconfig.hpp"
#include "config/activityconfig/xianmengzhanconfig.hpp"
#include "config/activityconfig/territorywarconfig.hpp"
#include "config/activityconfig/closedbetaconfig.hpp"
#include "config/activityconfig/wuxingguajiconfig.hpp"

#include "item/knapsack.h"
#include "other/daycounter/daycounter.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "scene/speciallogic/worldspecial/specialguildbattlenew.hpp"
#include "global/activity/activitymanager.hpp"

// --------------------------------------------------------------------------------------------------------
// 场景活动
// --------------------------------------------------------------------------------------------------------
void RoleActivity::OnActivityEnterReq(Protocol::CSActivityEnterReq *aer)
{
	if (aer->activity_type <= ACTIVITY_TYPE_INVALID || aer->activity_type >= ACTIVITY_TYPE_MAX || 
		aer->room_index < 0 || aer->room_index >= ACTIVITY_ROOM_MAX) 
	{
		return;
	}

	if (!m_role->GetRoleStatusManager()->CanEnterFB())
	{
		return;
	}

	// 一战到底 特殊处理
	if (ACTIVITY_TYPE_YIZHANDAODI == aer->activity_type)
	{
		this->CheckInitYiZhanDaoDiLeaveInfo();

		const int max_dead_count = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetDeadMaxCount();
		if (m_activity_info.yizhandaodi_dead_times >= max_dead_count)
		{
			m_role->NoticeNum(errornum::EN_YIZHANDAODI_DEAD_TIMES_LIMIT);
			return;
		}

		// 上面检查通过后，如果旧的房间信息还有效，进入旧房间
		if (0 != m_activity_info.yizhandaodi_leave_timestamp && -1 != m_activity_info.yizhandaodi_room_idx)
		{
			aer->room_index = m_activity_info.yizhandaodi_room_idx;
		}
	}

	// (战场特殊处理 3分钟内参与过战场 只能进入之前战场)
	if (ACTIVITY_TYPE_QUNXIANLUANDOU == aer->activity_type) 
	{
		this->CheckZhanChangParam();
		if (m_activity_info.zhanchang_room_info.Valid())
		{
			aer->room_index = m_activity_info.zhanchang_room_info.zhanchang_last_roomindex;
		}
	}
	else if (ACTIVITY_TYPE_NATIONAL_BOSS == aer->activity_type)
	{
		int camp = m_role->GetCamp();

		if (camp <= CAMP_TYPE_INVALID || camp >= CAMP_TYPE_MAX)
		{
			m_role->NoticeNum(errornum::EN_JOIN_CAMP_FIRST);
			return;
		}

		aer->room_index = 0; // 需求改为所有人进入同一个场景

		SpecialNationalBoss *nb_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialNationBoss(ActivityShadow::Instance().GetFBKeyByRoomIndex(aer->room_index), false);
		if (NULL == nb_logic || nb_logic->IsFinish())
		{
			m_role->NoticeNum(errornum::EN_NATIONAL_BOSS_CAMP_FINISH);
			return;
		}
	}
	else if (ACTIVITY_TYPE_XIANMENGZHAN == aer->activity_type)
	{
		if (INVALID_GUILD_ID == m_role->GetGuildID())
		{
			m_role->NoticeNum(errornum::EN_XIANMENGZHAN_INVALID_GUILD);
			return;
		}
	}

	Posi activity_pos(0, 0); int limit_level = 0, max_level = 0; 
	int activity_sceneid = this->GetActivityEnterInfoHelper(aer->activity_type, &activity_pos, &limit_level, &max_level);
	if (activity_sceneid > 0)
	{
		if (m_role->GetLevel() < limit_level)
		{
			m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_FB_LEVEL_LIMIT);
			return;
		}

		if (m_role->GetLevel() > max_level)
		{
			m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_FB_LEVEL_MAX_LIMIT);
			return;
		}

		if (!ActivityShadow::Instance().IsActivtyStandy(aer->activity_type) && !ActivityShadow::Instance().IsActivtyOpen(aer->activity_type))
		{
			m_role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
			return;
		}

		if (!m_role->GetRoleStatusManager()->CanEnterFB())
		{
			return;
		}

		if (ACTIVITY_TYPE_GONGCHENGZHAN == aer->activity_type)
		{
			if (0 != aer->room_index)
			{
				m_role->NoticeNum(errornum::EN_PARAMETER_ERROR);
				return;
			}

			if (INVALID_GUILD_ID == m_role->GetGuildID())
			{
				m_role->NoticeNum(errornum::EN_ENTER_NOT_GUILD);
				return;
			}
		}
		else if (ACTIVITY_TYPE_SHUIJING == aer->activity_type)
		{
			if (0 != aer->room_index)
			{
				m_role->NoticeNum(errornum::EN_PARAMETER_ERROR);
				return;
			}
			if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinActivity(ACTIVITY_TYPE_SHUIJING, m_role))
			{
				return;
			}
		}
		else if (ACTIVITY_TYPE_TIANJIANGCAIBAO == aer->activity_type)
		{
			if (0 != aer->room_index)
			{
				m_role->NoticeNum(errornum::EN_PARAMETER_ERROR);
				return;
			}

// 			int min_limit_level = LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetLimitLevel();
// 			if (m_role->GetLevel() < min_limit_level)
// 			{
// 				m_role->NoticeNum(errornum::EN_TIANJIANGCAIBAO_ENTER_LEVEL_LIMIT);
// 				return;
// 			}
		}
		else if (ACTIVITY_TYPE_WANGLINGEXPLORE == aer->activity_type)
		{
			if (0 != aer->room_index)
			{
				m_role->NoticeNum(errornum::EN_PARAMETER_ERROR);
				return;
			}

// 			int min_limit_level = LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreOtherCfg().m_min_level;
// 			if (m_role->GetLevel() < min_limit_level)
// 			{
// 				m_role->NoticeNum(errornum::EN_WANGLINGEXPLORE_ENTER_LEVEL_LIMIT);
// 				return;
// 			}
		}
		else if (ACTIVITY_TYPE_GUILDBATTLE == aer->activity_type)
		{
			if (0 != aer->room_index)
			{
				m_role->NoticeNum(errornum::EN_PARAMETER_ERROR);
				return;
			}

			if (INVALID_GUILD_ID == m_role->GetGuildID())
			{
				m_role->NoticeNum(errornum::EN_ENTER_NOT_GUILD);
				return;
			}
		}
		else if (ACTIVITY_TYPE_MILLIONAIRE == aer->activity_type)
		{
			if (0 != aer->room_index)
			{
				m_role->NoticeNum(errornum::EN_PARAMETER_ERROR);
				return;
			}
		}
		else if (ACTIVITY_TYPE_TERRITORY_WAR == aer->activity_type)
		{	
			if (!LOGIC_CONFIG->GetTerritoryWarConfig().CanEnterRoom(m_role, aer->room_index))
			{
				m_role->NoticeNum(errornum::EN_ENTER_GUILD_NOT_HAV_QUALIFICATION);
				return;
			}

			SpecialTerritoryWar *territorywar_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialTerritoryWar(ActivityShadow::Instance().GetFBKeyByRoomIndex(aer->room_index), false);
			if (NULL != territorywar_logic)
			{
				if (territorywar_logic->IsActivityFinish())
				{
					m_role->NoticeNum(errornum::EN_TW_WAR_IS_FINISH);
					return;
				}
			}
		}

		{
			// 封测活动开启期间
			if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_CLOSEDBETA))
			{
				const CloseBetaJoinActivityRewardCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetCloseBetaCfg().GetJoinActivityCfgByType(aer->activity_type);
				if (NULL != cfg_item)
				{
					int old_flag = m_activity_info.cba_join_activity_flag;

					m_activity_info.cba_join_activity_flag |= (1 << cfg_item->seq);

					if (old_flag != m_activity_info.cba_join_activity_flag)
					{
						this->SendCloseBetaActivityInfo();
					}
				}
			}

			{
				// 活动参与日志
				gamelog::g_log_roleactivity.printf(LL_INFO, "CommonActivity::OnActivityEnterReq user[%d, %s], level[%d], camp[%d], type[%d]", 
					m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), m_role->GetCamp(), aer->activity_type);

				ROLE_LOG_QUICK10(LOG_TYPE_FUNCTION_STATUS, m_role, LOG_TYPE_FUNCTION_STATUS_TYPE_ACTIVITY + aer->activity_type, 1, NULL, NULL, 
					m_role->GetLevel(), m_role->GetRoleActivity()->GetHistoryChongZhi(), 0, 0);

				gamelog::g_log_function_stats.printf(LL_INFO, "[JoinFB][user[%s %d] type:activity_type_%d times:1]", 
					m_role->GetName(), m_role->GetUID(), aer->activity_type);
			}
		}

		World::GetInstWorld()->GetSceneManager()->GoTo(m_role, activity_sceneid, ActivityShadow::Instance().GetFBKeyByRoomIndex(aer->room_index), activity_pos); 
	}
}

int RoleActivity::GetActivityEnterInfoHelper(int activity_type, Posi *enterpos, int *limit_leve, int *max_level)
{
	if (NULL == enterpos || NULL == limit_leve || NULL == max_level) return 0;

	UNSTD_STATIC_CHECK(36 == ACTIVITY_TYPE_MAX);

	JoinLimitUnit limit_unit;
	if (LOGIC_CONFIG->GetJoinLimitConfig().GetActivityJoinLimit(activity_type, &limit_unit))
	{
		*limit_leve = limit_unit.level;
		*max_level = limit_unit.max_level;
	}
	else
	{
		*limit_leve = 1;
		*max_level = 2000;
	}

	switch (activity_type)
	{
	case ACTIVITY_TYPE_ZHUXIE:
		{
			return LOGIC_CONFIG->GetZhuXieConfig().GetZhuXieFBEnterInfo(enterpos);
		}
		break;

	case ACTIVITY_TYPE_QUNXIANLUANDOU:
		{
			return LOGIC_CONFIG->GetQunXianLuanDouConfig().GetEnterInfo(QUNXIANLUANDOU_SIDE_1, enterpos);  // 默认用第一个阵营进去 进去后根据阵营调整玩家坐标
		}
		break;

	case ACTIVITY_TYPE_GONGCHENGZHAN:
		{
			int fb_key = ActivityShadow::Instance().GetFBKeyByRoomIndex(0);
			SpecialGongChengZhan *gongchengzhan_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialGongChengZhan(fb_key, true);
			if (NULL != gongchengzhan_logic)
			{
				bool is_shou_site = gongchengzhan_logic->GetRoleIsShouSite(m_role);
				return LOGIC_CONFIG->GetGongChengZhanConfig().GetEnterPos(is_shou_site, enterpos);
			}
		}
		break;

	case ACTIVITY_TYPE_GUILDBATTLE:
		{
			int fb_key = ActivityShadow::Instance().GetFBKeyByRoomIndex(0);
			SpecialGuildBattleNew *guildbattle_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialGuildBattle(fb_key, true);
			if (NULL != guildbattle_logic)
			{
				if (ActivityShadow::Instance().IsActivtyStandy(ACTIVITY_TYPE_GUILDBATTLE))
				{
					return LOGIC_CONFIG->GetGuildBattleConfigNew().GetGuildBattleEnterStandy(enterpos);
				}
				else if(ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILDBATTLE))
				{
					return LOGIC_CONFIG->GetGuildBattleConfigNew().GetGuildBattleEnterInfo(enterpos);
				}
			}
		}
		break;

	case ACTIVITY_TYPE_MILLIONAIRE:
		{
			int fb_key = ActivityShadow::Instance().GetFBKeyByRoomIndex(0);
			SpecialMillionaire *millionaire_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialMillionaire(fb_key, true);
			if (NULL != millionaire_logic)
			{
				return LOGIC_CONFIG->GetMillionaireConfig().GetMillionaireEnterInfo(enterpos);
			}
		}
		break;

	case ACTIVITY_TYPE_NATIONAL_BOSS:
		{
			return LOGIC_CONFIG->GetNationalBossConfig().GetSceneID();
		}
		break;

	case ACTIVITY_TYPE_XIANMENGZHAN:
		{
			return LOGIC_CONFIG->GetXianMengZhanConfig().GetEnterInfo(NULL);
		}
		break;

	case ACTIVITY_TYPE_YIZHANDAODI:
		{
			*limit_leve = LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetOpenLevel();
			return LOGIC_CONFIG->GetYiZhanDaoDiConfig().GetEnterPos(enterpos);
		}
		break;

	case ACTIVITY_TYPE_WUXING_GUAJI:
		{
			*limit_leve = LOGIC_CONFIG->GetWuXingGuaJiConfig().GetOpenLevel();
			return LOGIC_CONFIG->GetWuXingGuaJiConfig().GetEnterInfo(enterpos);
		}
		break;

	case ACTIVITY_TYPE_SHUIJING:
		{
			return LOGIC_CONFIG->GetShuiJingCfg().GetShuiJingEnterInfo(enterpos);
		}
		break;

	case ACTIVITY_TYPE_TIANJIANGCAIBAO:
		{
			int fb_key = ActivityShadow::Instance().GetFBKeyByRoomIndex(0);
			SpecialTianJiangCaiBao *tianjiangcaibao_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialTianJiangCaiBao(fb_key, true);

			if (NULL != tianjiangcaibao_logic)
			{
				return LOGIC_CONFIG->GetTianJiangCaiBaoConfig().GetEnterInfo(enterpos);
			}	
		}
		break;

	case ACTIVITY_TYPE_WANGLINGEXPLORE:
		{
			int fb_key = ActivityShadow::Instance().GetFBKeyByRoomIndex(0);
			SpecialWangLingExplore *wanglingexplore_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialWangLingExplore(fb_key, true);

			if (NULL != wanglingexplore_logic)
			{
				*enterpos = LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreOtherCfg().relive_pos;
				return LOGIC_CONFIG->GetWangLingExploreConfig().GetWangLingExploreOtherCfg().m_sceneid;
			}	
		}
		break;

	case ACTIVITY_TYPE_TERRITORY_WAR:
		{
			return LOGIC_CONFIG->GetTerritoryWarConfig().GetTerritoryWarEnterInfo(m_role,enterpos);
		}
		break;

	case ACTIVITY_TYPE_XINGZUOYIJI:
		{
			int fb_key = ActivityShadow::Instance().GetFBKeyByRoomIndex(0);
			SpecialXingzuoYiji *xingzuoyiji_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialXingzuoYiji(fb_key, true);

			if (NULL != xingzuoyiji_logic)
			{
				*enterpos = LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetOtherCfg()->relive_pos;
				return LOGIC_CONFIG->GetXingzuoYiJiConfig().XingzuoYijiGetOtherCfg()->scene_id;
			}	
		}
		break;

	}

	return 0;
}

void RoleActivity::CheckZhanChangParam()
{
	ZhanchangRoomInfo &zhanchang_info = m_activity_info.zhanchang_room_info;

	// 1：上次离开战场的时间已经超过3分钟
	unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (0 != zhanchang_info.zhanchang_last_leavetime && now_second >= zhanchang_info.zhanchang_last_leavetime + ZHANCHANG_RESERVE_INFO_TIME_S)
	{
		zhanchang_info.Reset();
	}
	else if (0 != zhanchang_info.zhanchang_last_leavetime)
	{
		unsigned int activity_opentime = 0;
		{
			if (ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_QUNXIANLUANDOU))
			{
				activity_opentime = ActivityShadow::Instance().GetActivityBeginTime(ACTIVITY_TYPE_QUNXIANLUANDOU);
			}
		}
		if (0 != activity_opentime && zhanchang_info.zhanchang_last_leavetime < activity_opentime) 
		{
			zhanchang_info.Reset();		// 离开战场未超过3分钟 且 离开时间戳小于战场开启时间戳，则重置房间信息 （一般只有使用gm命令才会出现这种情况）
		}
	}
}

void RoleActivity::OnGuildBattleSosReq(Protocol::CSSendGuildSosReq *sgsr)
{
	if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILDBATTLE))
	{
		return;
	}

	int scene_id = LOGIC_CONFIG->GetGuildBattleConfigNew().GuildBattleGetOtherCfg().scene_id;
	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, COMMON_SCENE_KEY);
	if (nullptr == scene || scene->GetSceneType() != Scene::SCENE_TYPE_GUILDBATTLE)
	{
		return;
	}

	SpecialGuildBattleNew *guild_battle = dynamic_cast<SpecialGuildBattleNew*>(scene->GetSpecialLogic());
	if (nullptr == guild_battle)
	{
		return;
	}

	if (!guild_battle->OnSosCost(sgsr, m_role))
	{
		return;
	}
}

void RoleActivity::OnGongChengZhanSosReq(Protocol::CSSendGuildSosReq *sgsr)
{
	if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GONGCHENGZHAN))
	{
		return;
	}

	int scene_id = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg().scene_id;
	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id, COMMON_SCENE_KEY);
	if (nullptr == scene || scene->GetSceneType() != Scene::SCENE_TYPE_GONGCHENGZHAN)
	{
		return;
	}

	SpecialGongChengZhan *gongcheng_zhan = dynamic_cast<SpecialGongChengZhan*>(scene->GetSpecialLogic());
	if (nullptr == gongcheng_zhan)
	{
		return;
	}

	if (!gongcheng_zhan->OnSosCost(sgsr, m_role))
	{
		return;
	}
}

void RoleActivity::OnSendGuildSosReq(int sos_type)
{
	if (NULL == m_role->GetScene()) return;

	if (!m_role->GetRoleStatusManager()->IsInFB() && INVALID_GUILD_ID != m_role->GetGuildID())
	{
		static gglobalprotocal::GuildMemberSos gms;
		gms.guild_id = m_role->GetGuildID();
		gms.db_index = m_role->GetUserId().db_index;
		gms.role_id = m_role->GetUserId().role_id;
		gms.sos_type = sos_type;
		gms.scene_id = m_role->GetScene()->GetSceneID();
		gms.pos_x = static_cast<short>(m_role->GetPos().x);
		gms.pos_y = static_cast<short>(m_role->GetPos().y);
		gms.enemy_uid = 0;
		GuildManager::Instance().OnGuildMemberSos(m_role, &gms);
	}
}

void RoleActivity::OnReplyGuildBattleSosReq(Protocol::CSReplyGuildSosReq *sos_reply)
{
	if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GUILDBATTLE))
	{
		return;
	}
	Scene *scene = m_role->GetScene();

	if (NULL == scene || scene->GetSceneType() != Scene::SCENE_TYPE_GUILDBATTLE)
	{
		return;
	}

	SpecialGuildBattleNew *guild_battle = dynamic_cast<SpecialGuildBattleNew*>(scene->GetSpecialLogic());
	if (nullptr == guild_battle)
	{
		return;
	}

	m_role->ResetPos(sos_reply->pos_x, sos_reply->pos_y);
}

void RoleActivity::OnReplyGongChengZhanSosReq(Protocol::CSReplyGuildSosReq *sos_reply)
{
	if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_GONGCHENGZHAN))
	{
		return;
	}

	Scene *scene = m_role->GetScene();

	if (NULL == scene || scene->GetSceneType() != Scene::SCENE_TYPE_GONGCHENGZHAN)
	{
		return;
	}

	SpecialGongChengZhan *gongcheng_zhan = dynamic_cast<SpecialGongChengZhan*>(scene->GetSpecialLogic());
	if (nullptr == gongcheng_zhan)
	{
		return;
	}

	m_role->ResetPos(sos_reply->pos_x, sos_reply->pos_y);
}

void RoleActivity::OnZhuXieFetchReward(int task_id)
{
	if (NULL == m_role->GetScene()) return;

	SpecialLogic *special_logic = m_role->GetScene()->GetSpecialLogic();
	if (NULL != special_logic && Scene::SCENE_TYPE_ZHUXIE == special_logic->GetSceneType())
	{
		SpecialZhuXie *zhuxie_special = dynamic_cast<SpecialZhuXie*>(special_logic);
		if (nullptr != zhuxie_special)
			zhuxie_special->OnFetchTaskReward(m_role, task_id);
	}
}

void RoleActivity::OnXianMengZhanEnterReq()
{
	Posi enterpos(0, 0);
	int scene_id = LOGIC_CONFIG->GetXianMengZhanConfig().GetEnterInfo(&enterpos);
	if (scene_id > 0)
	{
		if (!m_role->GetRoleStatusManager()->CanEnterFB())
		{
			return;
		}

		World::GetInstWorld()->GetSceneManager()->GoTo(m_role, scene_id, 1, enterpos);
	}
}

void RoleActivity::XianMengZhanGuildCall(bool use_gold)
{
	if (NULL == m_role->GetScene()) return;

	SpecialLogic *special_logic = m_role->GetScene()->GetSpecialLogic();
	if (NULL != special_logic && Scene::SCENE_TYPE_XIANMENGZHAN == special_logic->GetSceneType())
	{
		SpecialXianMengZhan *xianmengzhan_special = dynamic_cast<SpecialXianMengZhan*>(special_logic);
		if (nullptr != xianmengzhan_special)
			xianmengzhan_special->OnRoleCallGuildMembers(m_role, use_gold);
	}
}

void RoleActivity::XianMengZhanGuildFollow()
{
	if (NULL == m_role->GetScene()) return;

	SpecialLogic *special_logic = m_role->GetScene()->GetSpecialLogic();
	if (NULL != special_logic && Scene::SCENE_TYPE_XIANMENGZHAN == special_logic->GetSceneType())
	{
		SpecialXianMengZhan *xianmengzhan_special = dynamic_cast<SpecialXianMengZhan*>(special_logic);
		if (nullptr != xianmengzhan_special)
			xianmengzhan_special->OnRoleFollowGuildLeader(m_role);
	}
}

void RoleActivity::OnBattleFieldPickUpShenShiReq(int shenshi_objid)
{
	if (NULL == m_role->GetScene()) return;

	SpecialQunXianLuanDou *special_logic = dynamic_cast<SpecialQunXianLuanDou*>(m_role->GetScene()->GetSpecialLogic());
	if (NULL != special_logic && Scene::SCENE_TYPE_QUNXIANLUANDOU == special_logic->GetSceneType())
	{
		special_logic->OnPickUpBattleFiledShenShi(m_role, shenshi_objid);
	}	
}

void RoleActivity::AddBattleFieldHonor(int honor_val, const char *type, bool notify_client)
{
	if (0 == honor_val || NULL == type) return;

	m_activity_info.battlefield_honor += honor_val;
	if (m_activity_info.battlefield_honor < 0) m_activity_info.battlefield_honor = 0;

	if (notify_client) this->OnGetBattleFieldHonorReq(honor_val);

	gamelog::g_log_roleactivity.printf(LL_INFO, "[RoleActivity::AddBattleFieldHonor][user[%s %d] type:%s battlefield_honor:%d, remain_honor:%d]",
		m_role->GetName(), UidToInt(m_role->GetUserId()), type, honor_val, m_activity_info.battlefield_honor);

	ROLE_LOG_QUICK6(LOG_TYPE_HONOUR, m_role, honor_val, m_activity_info.battlefield_honor, type, NULL);
}

void RoleActivity::OnGetBattleFieldHonorReq(int delta_honor)
{
	Protocol::SCBattleFieldHonorChange bfhc;
	bfhc.honor = m_activity_info.battlefield_honor;
	bfhc.delta_honor = delta_honor;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&bfhc, sizeof(bfhc));
}

void RoleActivity::OnEnterGongchengzhan()
{

}

void RoleActivity::OnEnterGuildBattle()
{
	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
	if (NULL != guild)
	{
		guild->SetJoinNewGuildBattleFlag(1);
	}
}

void RoleActivity::OnEnterQuestion_2()
{

}

void RoleActivity::SendGuildFbInfo()
{

}

void RoleActivity::SyncGuildFbStatus()
{
	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetUserId());
	if (NULL != guild)
	{
		guild->SyncGuildFbStatus(m_role->GetNetId());
	}
}

void RoleActivity::SyncGuildBeastInfo()
{
	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetUserId());
	if (NULL != guild)
	{
		guild->SyncGuildBeastStatus(m_role->GetNetId());
	}
}

void RoleActivity::SyncGuildBonfireStatus()
{
	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetUserId());
	if (NULL != guild)
	{
		guild->SyncGuildBonfireStatus(m_role->GetNetId());
	}
}

void RoleActivity::SendGuildMonsterInfo()
{
	Protocol::SCGuildMonsterInfo scinfo;

	Guild *guild = GuildManager::Instance().GetGuild(m_role->GetUserId());
	if (NULL != guild)
	{
		scinfo.guild_finish_seq = guild->GetMonsterFinishSeq();
		guild->GetMonsterFinishUsername(scinfo.guild_finish_username);
		scinfo.guild_finish_username[sizeof(scinfo.guild_finish_username) - 1] = '\0';

		scinfo.prof = -1;

		if (strlen(scinfo.guild_finish_username) > 0)
		{
			bool is_online = false;
			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNodeByName(scinfo.guild_finish_username, &is_online);

			if (NULL != user_node)
			{
				scinfo.prof = user_node->prof;
			}
		}
	}

	scinfo.my_finish_seq = m_activity_info.guild_monster_finish_seq;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&scinfo, sizeof(scinfo));
}

void RoleActivity::OnNationalBossBuyBuffReq()
{
	if (NULL != m_role->GetScene())
	{
		SpecialNationalBoss *special_logic = dynamic_cast<SpecialNationalBoss*>(m_role->GetScene()->GetSpecialLogic());
		if (NULL != special_logic && Scene::SCENE_TYPE_NATIONAL_BOSS == special_logic->GetSceneType())
		{
			special_logic->OnBuyBuff(m_role);
		}
	}
}

void RoleActivity::SetYiZhanDaoDiLeaveInfo(unsigned int leave_time, int room_index)
{
	m_activity_info.yizhandaodi_leave_timestamp = leave_time;
	m_activity_info.yizhandaodi_room_idx = room_index;
}

void RoleActivity::CheckInitYiZhanDaoDiLeaveInfo()
{
	if (0 != m_activity_info.yizhandaodi_leave_timestamp)
	{
		unsigned int activity_opentime = ActivityShadow::Instance().GetActivityBeginTime(ACTIVITY_TYPE_YIZHANDAODI);
		if (0 != activity_opentime && m_activity_info.yizhandaodi_leave_timestamp < activity_opentime) 
		{
			m_activity_info.yizhandaodi_leave_timestamp = 0;
			m_activity_info.yizhandaodi_room_idx = -1;
			m_activity_info.yizhandaodi_dead_times = 0;
		}
	}
}

void RoleActivity::YiZhanDaoDiGuWu(Protocol::CSYiZhanDaoDiGuwu *yzddgw)
{
	if (NULL == m_role->GetScene()) return;

	SpecialYiZhanDaoDi *special_logic = dynamic_cast<SpecialYiZhanDaoDi*>(m_role->GetScene()->GetSpecialLogic());
	if (NULL != special_logic && Scene::SCENE_TYPE_YIZHANDAODI == special_logic->GetSceneType())
	{
		special_logic->YiZhanDaoDiGuWu(m_role, yzddgw);
	}
}

void RoleActivity::WuXingGuaJiGetReward(Protocol::CSWuXingGuaJiGetReward *wxgjgr)
{
	if (NULL == m_role->GetScene()) return;

	SpecialWuXingGuaJi *special_logic = dynamic_cast<SpecialWuXingGuaJi*>(m_role->GetScene()->GetSpecialLogic());
	if (NULL != special_logic && Scene::SCENE_TYPE_WUXINGGUAJI == special_logic->GetSceneType())
	{
		special_logic->WuXingGuaJiGetReward(m_role, wxgjgr);
	}
}

void RoleActivity::SetFirstEnterGuildFb(GuildID guild_id)
{
	m_activity_info.guildfb_first_enter_guild_id = guild_id;
}

bool RoleActivity::CanEnterGuildFb()
{
	if (m_activity_info.guildfb_first_enter_guild_id > 0 && m_role->GetGuildID() != m_activity_info.guildfb_first_enter_guild_id)
	{
		m_role->NoticeNum(errornum::EN_GUILD_ALREADY_JOIN_GUILD_FB);

		return false;
	}

	if (ActivityManager::Instance().IsActivityClose(ACTIVITY_TYPE_GUILD_FB))
	{
		m_role->NoticeNum(errornum::EN_GUILD_FB_OUT_OF_TIME);
		return false;
	}

	return true;
}

void RoleActivity::OnCSGCZFetchDailyRewardReq()
{
	if (m_role->GetGuildID() != WorldStatus::Instance().GetGCZWinGuild())
	{
		return;
	}

	int fetch_times = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_GCZ_DAILY_REWARD_TIMES);
	if (fetch_times > 0)
	{
		m_role->NoticeNum(errornum::EN_GONGCHENGZHAN_REWARD_COUNT_LIMIT);
		return;
	}
	m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_GCZ_DAILY_REWARD_TIMES);

	int reward_type = GczDailyRewardItemCfg::REWARD_TYPE_NORMAL;
	if (m_role->GetUID() == WorldStatus::Instance().GetGCZChengzhuID())
	{
		reward_type = GczDailyRewardItemCfg::REWARD_TYPE_CHENZHU;
	}

	const GczDailyRewardItemCfg *reward_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetDailyRewardItemCfg(reward_type);
	if (NULL != reward_cfg)
	{
		m_role->GetKnapsack()->PutListOrMail(reward_cfg->reward_item_list, PUT_REASON_GCZ_DAILY_REWARD);
	}
}

void RoleActivity::OnEnterZhuXieZhanChang()
{

}

void RoleActivity::OnEnterTianJiangCaiBao()
{

}

void RoleActivity::OnEnterWangLingExplore()
{

}

void RoleActivity::OnEnterXianmengzhan()
{

}
