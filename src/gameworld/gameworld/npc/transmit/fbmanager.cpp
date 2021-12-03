#include "fbmanager.hpp"
#include "other/route/mailroute.hpp"
#include "obj/character/role.h"

#include "scene/scene.h"
#include "scene/scenemanager.h"

#include "scene/speciallogic/speciallogic.hpp"
#include "scene/speciallogic/personspecial/specialzhanshendian.hpp"
#include "globalconfig/globalconfig.h"
#include "npc/npc.h"
#include "scene/speciallogic/guildspecial/specialguildmonster.hpp"
#include "gamelog.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/commondata.hpp"
#include "servercommon/userprotocal/msgsystem.h"
#include "world.h"
#include "gstr.h"
#include "other/fb/neqfbconfig.hpp"
#include "other/fb/phasefbconfig.hpp"
#include "global/team/teammanager.hpp"
#include "config/guildactivityconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/qingyuanshengdicconfig.h"
#include "other/vip/vipconfig.hpp"
#include "other/qingyuan/qingyuanconfig.hpp"
#include "other/fb/yaoshoujitanteamfbconfig.hpp"
#include "other/fb/storyfbconfig.hpp"
#include "other/fb/mojiefbconfig.hpp"
#include "other/fb/fbchallengeconfig.hpp"
#include "other/fb/migongxianfuconfig.hpp"
#include "other/fb/patafbconfig.hpp"
#include "other/fb/dailyfbconfig.hpp"
#include "other/fb/vipfbconfig.hpp"
#include "other/fb/dailytaskfbcofig.hpp"
#include "other/fb/fbguideconfig.hpp"
#include "other/teamequipfb/teamequipfbconfig.hpp"
#include "other/fb/towerdefendteamconfig.hpp"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "global/team/team.hpp"
#include "servercommon/qingyuandef.hpp"
#include "scene/speciallogic/worldspecial/specialhunyan.hpp"
#include "scene/speciallogic/personspecial/specialwushuangfb.hpp"
#include "scene/speciallogic/personspecial/specialpatafb.hpp"
#include "scene/speciallogic/personspecial/specialrunetower.hpp"
#include "scene/speciallogic/personspecial/specialarmordefend.hpp"
#include "gameworld/scene/speciallogic/personspecial/specialtuitufb.h"
#include "global/team/teammanager.hpp"
#include "gameworld/other/fb/migongxianfumgr.hpp"
#include "gameworld/other/camp/campconfig.hpp"
#include <vector>
#include "global/usercache/usercache.hpp"
#include "global/globalrole/globalrolemanager.hpp"
#include "global/globalrole/qingyuanglobal/qingyuanglobal.hpp"
#include "other/runesystem/runesystem.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "other/fb/rolefbtuitu.h"
#include "other/fb/fbtuituconfig.h"
#include "other/fb/funopenfbconfig.hpp"
#include "other/runesystem/runetowerconfig.hpp"
#include "task/newtaskmanager.h"
#include "friend/teaminfo.hpp"
#include "other/vip/vip.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/fb/rolestoryfb.hpp"
#include "other/fb/rolemojiefb.hpp"
#include "other/fb/rolefbchallenge.hpp"
#include "other/fb/rolefbphase.hpp"
#include "other/fb/rolepatafb.hpp"
#include "other/fb/rolevipfb.hpp"
#include "other/qingyuan/qingyuan.hpp"
#include "other/fb/roledailyfb.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "other/fb/rolefbequip.hpp"
#include "other/fb/armordefendconfig.hpp"
#include "other/fb/rolefbarmordefend.hpp"
#include "config/personbossconfig.hpp"
#include "other/fb/xinmofbconfig.hpp"
#include "config/zhuanzhiconfig.hpp"
#include "other/rolezhuanzhi/rolezhuanzhi.hpp"
#include "other/capability/capability.hpp"
#include "other/fb/fbequipconfig.hpp"
#include "other/fb/roleteamfb.hpp"
#include "global/randactivity/impl/randactivityholidayguard.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "gameworld/global/droplog/droplogmanager.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "other/fb/rolepersonbossfb.hpp"
#include "other/event/eventhandler.hpp"
#include "other/greatesoldier/greatesoldier.hpp"

using namespace std;

FBManager::FBManager() : m_hunyan_fb_key_inc(0), m_daily_fb_key_inc(0), m_team_equip_fb_key_inc(0)
{

}

FBManager::~FBManager()
{

}

FBManager & FBManager::Instance()
{
	static FBManager key; 
	return key; 
}

void FBManager::LeaveFB(Role *role)
{
	Scene *scene = role->GetScene();
	if (NULL != role && NULL != scene && (!scene->InStaticScene() || scene->GetSceneType() == Scene::SCENE_TYPE_VIP_FB))
	{
		scene->GetSpecialLogic()->OnLeaveFB(role);

		Posi target_pos;
		int target_scene_id = role->GetLastScene(&target_pos);

		gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::LeaveFB: %s[%d] [scene_id:%d]", role->GetName(), UidToInt(role->GetUserId()), scene->GetSceneID());

		if (CrossConfig::Instance().IsHiddenServer())
		{
			role->BackToOriginalServer();
		}

		World::GetInstWorld()->GetSceneManager()->GoTo(role, target_scene_id, -1, target_pos);	// 只能往普通场景传送
	}
}

void FBManager::EnterFB(Role *role, short fb_type, int param1, int param2, int param3)
{
	if (NULL == role)
	{
		return;
	}

	if (!role->GetRoleStatusManager()->CanEnterFB())
	{
		return;
	}

	if (FBCT_QINGYUAN == fb_type)
	{
		this->EnterQingyuanFB(role, (0 != param1));
	}
	else if (FBCT_HUNYAN == fb_type)
	{
		this->EnterHunyanFB(role, param1, param2);
	}
	else if (FBCT_STORY_FB == fb_type)
	{
		this->EnterStoryFb(role, param1);
	}
	else if (FBCT_DAILY_FB == fb_type)
	{
		this->EnterDailyFb(role, param1, param2);
	}
	else if (FBCT_CHALLENGE == fb_type)
	{
		this->EnterChallengeFB(role, param1);
	}
	else if (FBCT_PHASE == fb_type)
	{
		this->EnterPhaseFB(role, param1);
	}
	else if (FBCT_YAOSHOUJITANG_TEAM == fb_type || FBCT_MIGONGXIANFU_TEAM == fb_type)
	{
		this->EnterTeamFB(role, fb_type, param1);
	}
	else if (FBCT_PATAFB == fb_type)
	{
		this->EnterPataFB(role);
	}
	else if (FBCT_VIPFB == fb_type)
	{
		this->EnterVipFB(role, param1);
	}
	else if (FBCT_MOJIERENLEIFB == fb_type)
	{
		this->EnterMoJieHumanFb(role, param1);
	}
	else if (FBCT_MOJIEAIRENFB == fb_type)
	{
		//this->EnterMoJieHumanFb(role, param1);
	}
	else if (FBCT_FUN_OPEN_FB == fb_type)
	{
		//this->EnterFunOpenFB(role, param1);
	}
	else if (FBCT_FBGUIDE == fb_type)
	{
		this->EnterGuideFB(role, param1);
	}
	else if (FBCT_TOWERDEFEND_PERSONAL == fb_type)
	{
		if (!param2)
		{
			this->EnterPersonalDefendFB(role, param1);
		}
		else
		{
			this->EnterPersonalHolidayGuardFB(role, param1, param3);
		}
	}
	else if (FBCT_RUNE_TOWER == fb_type)
	{
		this->EnterRuneTowerFB(role);
	}
	else if (FBCT_TEAM_EQUIP_FB == fb_type)
	{
		this->EnterTeamEquipFb(role, param1);
	}
	else if (FBCT_DAILY_TASK_FB == fb_type)
	{
		this->EnterDailyTaskFb(role, param1);
	}
	else if (FBCT_TUITU_TUITU_FB == fb_type)
	{
		this->EnterTuituFb(role, param1, param2, param3);
	}
	else if (FBCT_QING_YUAN_SHENG_DI_FB == fb_type)
	{
		this->EnterQingYuanShengDiFb(role, param1);
	}
	else if(FBCT_NEQ_FB == fb_type)
	{
		this->EnterNewEquipFb(role, param1, param2);
	}
	else if (FBCT_ARMOR_FB == fb_type)
	{
		this->EnterArmorDefendFb(role, param1);
	}
	else if (FBCT_PERSON_BOSS == fb_type)
	{
		this->EnterPersonBossFB(role, param1);
	}
	else if (FBCT_XINMO_FB == fb_type)
	{
		this->EnterXinMoFB(role);
	}
	else if (FBCT_BUILD_TOWER == fb_type)
	{
		this->EnterBuildTowerFB(role);
	}
	else if(FBCT_TOWERDEFEND_TEAM== fb_type)
	{
		this->EnterTeamTowerDefendFb(role, param1);
	}
	else if (FBCT_EQUIP_TEAM_FB == fb_type)
	{
		this->EnterEquipTeamFb(role, param1);
	}
}

void FBManager::AutoFB(Role *role, short fb_type, int param1, int param2, int param3)
{
	if (NULL == role)
	{
		return;
	}

	Scene * scene = role->GetScene();
	if (NULL == scene)
	{
		return;
	}

	if (!Scene::IsStaticScene(scene->GetSceneType())) //活动副本中不允许扫荡
	{
		if (FBCT_DAILY_FB == fb_type)
		{
			role->NoticeNum(errornum::EN_SPECIAL_SCENE_OPERATE_LIMIT);
		}
		else
		{
			role->NoticeNum(errornum::EN_FB_ATUO_NOT_IN_STATIC_SCENE);
		}

		return;
	}
	

	if(FBCT_DAILY_FB == fb_type)
	{
		if (0 == param1)
		{
			role->GetRoleDailyFb()->OnPayFbTimes();
		}
	}
	else if (FBCT_STORY_FB == fb_type)
	{
		if(param1 >= 0)
		{
			role->GetFBStory()->AutoFBReqOne(param1);
		}
		else
		{
			role->GetFBStory()->AutoFBReqAll();
		}
		
	}
	else if (FBCT_PHASE == fb_type)
	{
		role->GetRoleFBPhase()->AutoFBReq(param1, param2);
	}
	else if (FBCT_PATAFB == fb_type)
	{
		role->GetRolePataFB()->AutoFBReq();
	}
	else if (FBCT_VIPFB == fb_type)
	{
		role->GetRoleVipFB()->AutoFBReq(param1);
	}
	else if (FBCT_TOWERDEFEND_PERSONAL == fb_type)
	{
		if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinFB(FBManager::FBCT_TOWERDEFEND_PERSONAL, role))
		{
			role->NoticeNum(errornum::EN_FBMANAGER_JOIN_FB_LIMIT);
			return;
		}

		role->GetFBTowerDefend()->PersonalFBAutoFB(param1);
	}
	else if(FBCT_NEQ_FB == fb_type)
	{
		if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinFB(FBManager::FBCT_NEQ_FB, role))
		{
			role->NoticeNum(errornum::EN_FBMANAGER_JOIN_FB_LIMIT);
			return;
		}
		role->GetFBEquip()->OnNeqAutoReq(param1, param2);
	}
	else if(FBCT_ARMOR_FB == fb_type)
	{
		int level = param1;

		if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinFB(FBManager::FBCT_ARMOR_FB, role))
		{
			role->NoticeNum(errornum::EN_FBMANAGER_JOIN_FB_LIMIT);
			return;
		}
		role->GetRoleFbArmorDefend()->PersonalFBAutoFB(level);
	}
	else if (FBCT_BUILD_TOWER == fb_type)
	{
		if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinFB(FBManager::FBCT_BUILD_TOWER, role))
		{
			role->NoticeNum(errornum::EN_FBMANAGER_JOIN_FB_LIMIT);
			return;
		}
		AutoBuildTowerFB(role, param1);
	}
}

void FBManager::LogFunctionStats(Role *role, int fb_type, int param1, int param2, int param3)
{
	if (NULL == role)
	{
		return;
	}

	gamelog::g_log_function_stats.printf(LL_INFO, "[JoinFB][user[%s %d] type:fb_type_%d times:1 param1:%d param2:%d param3:%d]", 
		role->GetName(), role->GetUID(), fb_type, param1, param2, param3);

	ROLE_LOG_QUICK10(LOG_TYPE_FUNCTION_STATUS, role, LOG_TYPE_FUNCTION_STATUS_TYPE_FB + fb_type, 1, NULL, NULL, 
		role->GetLevel(), role->GetRoleActivity()->GetHistoryChongZhi(), 0, 0);
}

void FBManager::BuildTowerFbBuyTimes(Role *role)
{
	const BuildTowerFbOtherCfg &other_cfg = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg();

	int buy_times = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_BUILD_TOWER_FB_BUY_TIMES);
	int day_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_BUILD_TOWER_FB_BUY_TIMES);
	if (buy_times >= day_buy_times)
	{
		role->NoticeNum(errornum::EN_TOWERDEFEND_FB_BUY_LIMIT_TIMES);
		return ;
	}

	if (!role->GetKnapsack()->GetMoney()->AllGoldMoreThan(other_cfg.buy_times_gold))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return ;
	}

	if (!role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.buy_times_gold, "EnterBuildTowerFB", true))
	{
		return ;
	}

	role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_BUILD_TOWER_FB_BUY_TIMES);

	return ;
}

bool FBManager::AutoBuildTowerFB(Role *role, int flush_extra_num)
{
	if (NULL == role || flush_extra_num < 0 || flush_extra_num > BUILD_TOWER_MAX_MONSTER_WAVE + 1)
	{
		return false;
	}

	if (0 == role->GetCommonDataParam()->is_pass_build_tower_fb)
	{
		role->NoticeNum(errornum::EN_STORY_FB_NO_PASS);
		return false;
	}

	const BuildTowerFbOtherCfg &other_cfg = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg();

	int buy_times = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_BUILD_TOWER_FB_BUY_TIMES);
	if (!role->GetDayCounter()->DayCountCheck(DayCounter::DAYCOUNT_ID_BUILD_TOWER_FB_ENTER_TIMES, buy_times + other_cfg.enter_free_times))
	{
		return false;
	}

	long long total_exp = 0;
	int index_count = 0;
	static ItemConfigData reward_item_list[MonsterInitParam::MAX_DROP_ITEM_COUNT];
	memset(reward_item_list, 0, sizeof(reward_item_list));
	ItemConfigData item_list_cmp[MonsterInitParam::MAX_DROP_ITEM_COUNT];
	memset(item_list_cmp, 0, sizeof(item_list_cmp));
	//LOGIC_CONFIG->GetBuildTowerFbConfig().GetSweeepReward(flush_extra_num, &index_count, reward_item_list, &total_exp, role->GetLevel());
	
	for (int wave = 0; wave <= BUILD_TOWER_MAX_MONSTER_WAVE; wave++)
	{
		const BuildTowerFbMonsterFlushCfg::ConfigItem *cfg = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbMonsterFlushCfg(wave);
		if (NULL == cfg) continue;

		for (int i = 0; i < cfg->flush_num; i++)
		{
			long long reward_exp = static_cast<long long>(LOGIC_CONFIG->GetBuildTowerFbConfig().GetThisMonsterExp(cfg->monster_id, role->GetLevel()));
			MONSTERPOOL->GetMonsterDrop(cfg->monster_id, reward_item_list, &index_count, &reward_exp, NULL, NULL);
			total_exp += reward_exp;
		}

		for (int j = 0; j < index_count; j++)
		{
			if (reward_item_list[j].item_id == item_list_cmp[j].item_id && reward_item_list[j].num != item_list_cmp[j].num)
			{
				this->AutoFbAddLog(role, cfg->monster_id, reward_item_list[j].item_id, reward_item_list[j].num - item_list_cmp[j].num, other_cfg.scene_id);
				item_list_cmp[j].num = reward_item_list[j].num;
			}
			else
			{
				this->AutoFbAddLog(role, cfg->monster_id, reward_item_list[j].item_id, reward_item_list[j].num, other_cfg.scene_id);
				item_list_cmp[j].item_id = reward_item_list[j].item_id;
				item_list_cmp[j].num = reward_item_list[j].num;
			}
		}

		if (cfg->boss_id > 0)
		{
			long long reward_exp = static_cast<long long>(LOGIC_CONFIG->GetBuildTowerFbConfig().GetThisMonsterExp(cfg->boss_id, role->GetLevel()));
			MONSTERPOOL->GetMonsterDrop(cfg->boss_id, reward_item_list, &index_count, &reward_exp, NULL, NULL);
			total_exp += reward_exp;

			for (int j = 0; j < index_count; j++)
			{
				if (reward_item_list[j].item_id == item_list_cmp[j].item_id && reward_item_list[j].num != item_list_cmp[j].num)
				{
					this->AutoFbAddLog(role, cfg->boss_id, reward_item_list[j].item_id, reward_item_list[j].num - item_list_cmp[j].num, other_cfg.scene_id);
					item_list_cmp[j].num = reward_item_list[j].num;
				}
				else
				{
					this->AutoFbAddLog(role, cfg->boss_id, reward_item_list[j].item_id, reward_item_list[j].num, other_cfg.scene_id);
					item_list_cmp[j].item_id = reward_item_list[j].item_id;
					item_list_cmp[j].num = reward_item_list[j].num;
				}
			}
		}
	}

	if (flush_extra_num > 0)
	{
		for (int wave = 0; wave < flush_extra_num; wave++)
		{
			long long reward_exp = 0;
			MONSTERPOOL->GetMonsterDrop(other_cfg.extra_monster_id, reward_item_list, &index_count, &reward_exp, NULL, NULL);
			total_exp += reward_exp;
		}

		for (int j = 0; j < index_count; j++)
		{
			if (reward_item_list[j].item_id == item_list_cmp[j].item_id && reward_item_list[j].num != item_list_cmp[j].num)
			{
				this->AutoFbAddLog(role, other_cfg.extra_monster_id, reward_item_list[j].item_id, reward_item_list[j].num -item_list_cmp[j].num, other_cfg.scene_id);
			}
			else
			{
				this->AutoFbAddLog(role, other_cfg.extra_monster_id, reward_item_list[j].item_id, reward_item_list[j].num, other_cfg.scene_id);
				//item_list_cmp[j].item_id = reward_item_list[j].item_id;
				//item_list_cmp[j].num = reward_item_list[j].num;
			}
		}
	}


	int cost_gold = other_cfg.extra_call_gold * flush_extra_num;
	if (!role->GetKnapsack()->GetMoney()->AllGoldMoreThan(cost_gold))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (cost_gold > 0 && !role->GetKnapsack()->GetMoney()->UseAllGold(cost_gold, "EnterBuildTowerFB", true))
	{
		return false;
	}

	// 给予
	if (index_count > 0)
	{
		role->GetKnapsack()->PutList(index_count, reward_item_list, PUT_REASON_BUILD_TOWER_AUTO);
	}

	std::string item_log;
	ItemConfigDataListLog(item_log, index_count, reward_item_list);
	gamelog::g_log_fb.printf(LL_INFO,"FBManager::AutoBuildTowerFB,user:[%d][%s],extra:[%d],cost:[%d],reward:[%s]", 
		role->GetUID(), role->GetName(), flush_extra_num,cost_gold, item_log.c_str());

// 	Protocol::SCAutoFBRewardDetail afrd;
// 	afrd.fb_type = FBManager::FBCT_BUILD_TOWER;
// 	afrd.reward_coin = 0;
// 	afrd.reward_exp = total_exp;
// 	afrd.reward_yuanli = 0;
// 	afrd.reward_xianhun = 0;
// 	afrd.reserve_sh = 0;
// 
// 	afrd.item_count = 0;
// 
// 	for (int i = 0;i < index_count && afrd.item_count <Protocol::SCAutoFBRewardDetail::REWARD_ITEM_MAX_COUNT; i++)
// 	{
// 		if (reward_item_list[i].item_id <= 0) continue;
// 
// 		afrd.item_list[afrd.item_count].item_id = reward_item_list[i].item_id;
// 		afrd.item_list[afrd.item_count].num = reward_item_list[i].num;
// 		afrd.item_count++;
// 	}
// 	int len = sizeof(afrd) - sizeof(afrd.item_list[0])*(Protocol::SCAutoFBRewardDetail::REWARD_ITEM_MAX_COUNT - afrd.item_count);
// 	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&afrd, len);
	role->AddExp(total_exp, "SweepBuildTowerFB", Role::EXP_ADD_REASON_DEFAULT);

	role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_BUILD_TOWER_FB_ENTER_TIMES);

	EventHandler::Instance().OnEnterBuildTowerFB(role);
	return true;
}
void FBManager::EnterQingyuanFB(Role *leader_role, bool is_skip)
{
	if (NULL == leader_role)
	{
		return;
	}

	if (!leader_role->GetTeamInfo()->InTeam())
	{
		leader_role->NoticeNum(errornum::EN_MARRY_NO_IN_TEAM);
		return;
	}
	
	Team *team = leader_role->GetTeamInfo()->GetMyTeam();
	if (NULL != team)
	{
		if(QINGYUAN_FB_TEAM_MEMBER_COUNT ==  team->GetMemberNum())
		{
			this->EnterQingyuanFBWithLover(leader_role, is_skip);
		}
		else
		{
			leader_role->NoticeNum(errornum::EN_QINGYUAN_TEAM_MEMBER_ERROR);
		}
	}
}

void FBManager::EnterQingyuanFBPersonal(Role *leader_role, bool is_skip)
{
	if (NULL == leader_role)
	{
		return;
	}

// 	if (!leader_role->IsMarry())
// 	{
// 		leader_role->NoticeNum(errornum::EN_MARRY_NOT_MARRIED);
// 		return;
// 	}

	if (!leader_role->GetTeamInfo()->InTeam())
	{
		return;
	}

	Team *team = leader_role->GetTeamInfo()->GetMyTeam();
	if (NULL == team || 1 !=  team->GetMemberNum())
	{
		return;
	}

// 	if (is_skip)
// 	{
// 		int highest_qingyuan = leader_role->GetQingyuan()->GetQingyuanValue();
// 		if (LOGIC_CONFIG->GetQingyuanCfg().GetMaxWaveCount() <= QINGYUAN_SKIP_TO_LAST_WAVE_COUNT)
// 		{
// 			leader_role->NoticeNum(errornum::EN_QINGYUAN_FB_CAN_NOT_SKIP);
// 			return;
// 		}
// 	}

	if (!leader_role->GetQingyuan()->CanJoinFB(true))
	{
		leader_role->NoticeNum(errornum::EN_FB_YOU_CANOT_ENTER_FB);
		return;
	}

	leader_role->GetQingyuan()->AddEnterTimes();
	leader_role->GetQingyuan()->SetSkipFBWaves(is_skip);

	const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
	
	int scene_key = leader_role->GetUID();
	int scene_id = other_cfg.scene_id;
	Posi enter_pos = Posi(other_cfg.birth_pos_x, other_cfg.birth_pos_y);

	{
		this->LogFunctionStats(leader_role, FBCT_QINGYUAN, 0, 0, 0);

		bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(leader_role, scene_id, scene_key, enter_pos);

		gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFB: %s[%d] ret:%d fb_param:%d,%d,%d ", 
			leader_role->GetName(), leader_role->GetUID(), (ret ? 1 : 0), FBManager::FBCT_QINGYUAN, 0, 0);

		if (!ret)
		{
			leader_role->GetQingyuan()->SetSkipFBWaves(false);
			leader_role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
			return;
		}

		leader_role = NULL;
	}
}

void FBManager::EnterQingyuanFBWithLover(Role *leader_role, bool is_skip)
{
	if (NULL == leader_role)
	{
		return;
	}

// 	if (!leader_role->IsMarry())
// 	{
// 		leader_role->NoticeNum(errornum::EN_MARRY_NOT_MARRIED);
// 		return;
// 	}

	TeamInfo *team_info = leader_role->GetTeamInfo();
	if (NULL == team_info)
	{
		leader_role->NoticeNum(errornum::EN_FB_NOT_IN_TEAM);
		return;
	}

	if (!team_info->InTeam()) 
	{
		leader_role->NoticeNum(errornum::EN_FB_NOT_IN_TEAM);
		return;
	}

	Team *team = team_info->GetMyTeam();
	if (NULL == team)
	{
		return;
	}

	if (!team->IsLeader(leader_role->GetUserId()))
	{
		leader_role->NoticeNum(errornum::EN_MARRY_RESERVE_MUST_BE_TEAM_LEADER);
		return;
	}

	if (QINGYUAN_FB_TEAM_MEMBER_COUNT != team->GetMemberNum())
	{
		leader_role->NoticeNum(errornum::EN_MARRY_TEAM_MEMBER_NUM_INVALID);
		return;
	}

	Role *target_role = NULL;
	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++ i)
	{
		Team::TeamMember *team_member = team->GetTeamMember(i);
		if (NULL != team_member)
		{
			if (team_member->user_id != leader_role->GetUserId())
			{
				target_role = World::GetInstWorld()->GetSceneManager()->GetRole(team_member->user_id);
				break;
			}
		}
	}

	if (NULL == target_role) 
	{
//		leader_role->NoticeNum(errornum::EN_QINGYUAN_TARGET_IN_TEAM);
		leader_role->NoticeNum(errornum::EN_SOME_TEAMMATE_NOT_ONLINE);
		return;
	}

// 	if (leader_role->GetLoverUid() != target_role->GetUID())
// 	{
// 		return;
// 	}

	if (target_role->GetSex() == leader_role->GetSex())
	{
		gstr::SendError(leader_role, "en_qingyuan_target_same_sex");
		return;
	}

	if (!leader_role->GetQingyuan()->CanJoinFB(true))
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_marry_fb_can_not_enter,
			UidToInt(leader_role->GetUserId()), leader_role->GetName(), (int)leader_role->GetCamp());

		if (length > 0)
		{
			static Protocol::SCSystemMsg sm;
			int sendlen = 0;

			if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3))
			{
				team->BroadcastToAllMember((const char*)&sm, sendlen);
			}
		}

		return;
	}

	if (!target_role->GetQingyuan()->CanJoinFB(true))
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_marry_fb_can_not_enter,
			UidToInt(target_role->GetUserId()), target_role->GetName(), (int)target_role->GetCamp());

		if (length > 0)
		{
			static Protocol::SCSystemMsg sm;
			int sendlen = 0;

			if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3))
			{
				team->BroadcastToAllMember((const char*)&sm, sendlen);
			}
		}

		return;
	}

	leader_role->GetQingyuan()->AddEnterTimes();
	target_role->GetQingyuan()->AddEnterTimes();

	const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
	int scene_key = team->GetTeamSeq();
	int scene_id = other_cfg.scene_id;
	Posi enter_pos = Posi(other_cfg.birth_pos_x, other_cfg.birth_pos_y);

	{
		this->LogFunctionStats(leader_role, FBCT_QINGYUAN, 0, 0, 0);

		bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(leader_role, scene_id, scene_key, enter_pos);

		gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFB: %s[%d] ret:%d fb_param:%d,%d,%d ", 
			leader_role->GetName(), leader_role->GetUID(), (ret ? 1 : 0), FBManager::FBCT_QINGYUAN, 0, 0);

		if (!ret)
		{
			leader_role->GetQingyuan()->SetSkipFBWaves(false);
			target_role->GetQingyuan()->SetSkipFBWaves(false);
			leader_role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
			return;
		}

		leader_role = NULL;
	}

	{
		this->LogFunctionStats(target_role, FBCT_QINGYUAN, 0, 0, 0);

		bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(target_role, scene_id, scene_key, enter_pos);

		gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFB: %s[%d] ret:%d fb_param:%d,%d,%d ", 
			target_role->GetName(), target_role->GetUID(), (ret ? 1 : 0), FBManager::FBCT_QINGYUAN, 0, 0);

		if (!ret)
		{
			target_role->GetQingyuan()->SetSkipFBWaves(false);
			target_role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
			return;
		}

		target_role = NULL;
	}
}

void FBManager::EnterHunyanFB(Role *role, int yanhui_type, int yanhui_fb_key)
{
// 	if (NULL == role)
// 	{
// 		return;
// 	}
// 
// 	const QingyuanHunyanActiviryConfig * hunyan_act_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanHunyanActivityCfg();
// 	if (NULL == hunyan_act_cfg) return;
// 	if (role->GetQingyuan()->GetTodayHunyanJoinTimes() >= hunyan_act_cfg->join_yanhui_max_count)
// 	{
// 		role->NoticeNum(errornum::EN_MARRY_HUNYAN_JOIN_LIMIT);
// 		return;
// 	}
// 
// 	const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
// 	if (role->GetQingyuan()->GetTodayHunyanTimes() >= other_cfg.open_times)
// 	{
// 		role->NoticeNum(errornum::EN_MARRY_HUNYAN_OPEN_LIMIT);
// 		return;
// 	}
// 
// 	if (yanhui_type <= YANHUI_TYPE_INVALID || yanhui_type >= YANHUI_TYPE_MAX)
// 	{
// 		return;
// 	}
// 
// 	if (!role->IsMarry())
// 	{
// 		role->NoticeNum(errornum::EN_MARRY_NOT_MARRIED);
// 		return;
// 	}
// 
// 	if (this->IsHunyanRunning(role))
// 	{
// 		role->NoticeNum(errornum::EN_HUNYAN_IS_RUNNING);
// 		return;
// 	}
// 
// 	if(yanhui_type == YANHUI_TYPE_NOBIND_DIAMONDS && role->GetQingyuan()->IsFirstDiamondHunyan())
// 	{
// 
// 	}
// 	else
// 	{
// 		Team *team = role->GetTeamInfo()->GetMyTeam();
// 		if(NULL != team)
// 		{
// 			if(team->GetMemberNum() != QINGYUAN_FB_TEAM_MEMBER_COUNT)
// 			{
// 				role->NoticeNum(errornum::EN_YANHUI_TEAM_MEMBER_ERROR);
// 				return;
// 			}
// 
// 			if (NULL == team->GetTeamMember(IntToUid(role->GetLoverUid())))
// 			{
// 				role->NoticeNum(errornum::EN_YANHUI_TEAM_MEMBER_ERROR);
// 				return;
// 			}
// 		}
// 
// 		const QingyuanHunyanConfig::ConfigItem * marry_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanCfg(yanhui_type);
// 		if (NULL == marry_cfg)
// 		{
// 			return;
// 		}
// 		
// 		if (role->GetCommonDataParam()->today_putong_hunyan_times < 1 && YANHUI_TYPE_BIND_DIAMONDS == yanhui_type)
// 		{
// 			role->GetCommonDataParam()->today_putong_hunyan_times++;
// 			//lover_role->GetCommonDataParam()->today_putong_hunyan_times++;
// 
// 			//lover_role->GetQingyuan()->SendMarryInfo();
// 
// 			role->GetQingyuan()->SetYanhuiFBType(YANHUI_TYPE_BIND_DIAMONDS);
// 		}
// 		else
// 		{
// 			if (YANHUI_TYPE_BIND_DIAMONDS == yanhui_type)
// 			{
// 				if (!role->GetKnapsack()->GetMoney()->AllGoldMoreThan(marry_cfg->need_coin))
// 				{
// 					role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
// 					return;
// 				}
// 
// 				if (!role->GetKnapsack()->GetMoney()->UseAllGold(marry_cfg->need_coin, "yanhui"))
// 				{
// 					return;
// 				}
// 
// 				role->GetQingyuan()->SetYanhuiFBType(YANHUI_TYPE_BIND_DIAMONDS);
// 			}
// 
// 			if (YANHUI_TYPE_NOBIND_DIAMONDS == yanhui_type)
// 			{
// 				if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(marry_cfg->need_gold))
// 				{
// 					role->NoticeNum(errornum::EN_CHONGZHI_WANT_MONEY_HAS_NOT_ENOUGH);
// 					return;
// 				}
// 
// 				if (!role->GetKnapsack()->GetMoney()->UseGold(marry_cfg->need_gold, "yanhui"))
// 				{
// 					return;
// 				}
// 
// 				role->GetQingyuan()->SetYanhuiFBType(YANHUI_TYPE_NOBIND_DIAMONDS);
// 			}
// 		}
// 
// 		role->GetQingyuan()->AddTodayHunyanTimes();
// 		role->NoticeNum(noticenum::NT_YANHUI_START_SUCC);
// 
// 		const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();
// 		int scene_key = this->GetHunyanFBKey();
// 		int scene_id = hunyan_cfg.hunyan_scene_id;
// 		Posi enter_pos = Posi(hunyan_cfg.hunyan_birth_pos_x, hunyan_cfg.hunyan_birth_pos_y);
// 
// 		const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
// 
// 		static MailContentParam contentParam;
// 		contentParam.Reset();
// 		if (yanhui_type == YANHUI_TYPE_BIND_DIAMONDS)
// 		{
// 			contentParam.item_list[0].item_id = other_cfg.bind_gold_hy_reward_item.item_id;
// 			contentParam.item_list[0].is_bind = (other_cfg.bind_gold_hy_reward_item.is_bind) ? 1 : 0;
// 			contentParam.item_list[0].num = other_cfg.bind_gold_hy_reward_item.num;
// 		}
// 		else
// 		{
// 			contentParam.item_list[0].item_id = other_cfg.gold_hy_reward_item.item_id;
// 			contentParam.item_list[0].is_bind = (other_cfg.gold_hy_reward_item.is_bind) ? 1 : 0;
// 			contentParam.item_list[0].num = other_cfg.gold_hy_reward_item.num;
// 		}
// 
// 		int length = SNPRINTF(contentParam.contenttxt, sizeof(contentParam.contenttxt), gamestring::g_hunyan_reward_notice);
// 		if (length > 0)
// 		{
// 			MailRoute::MailToUser(role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentParam);
// 			//MailRoute::MailToUser(IntToUid(role->GetLoverUid()), SYSTEM_MAIL_REASON_INVALID, contentParam);
// 		}
// 
// 		this->RegisterHunyanKey(role, scene_key);
// 
// 		role->GetQingyuan()->SetCanHunyanOpen();
// 		role->GetQingyuan()->SendMarryInfo();
// 
// 		{
// 			// 传闻
// 			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_hunyan_open_person_yanhui_notice, role->GetUID(), role->GetName(),
// 				(int)role->GetCamp());
// 			if (length > 0) 
// 			{
// 				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
// 			}
// 		}
// 
// 		{
// 
// 			bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(role, scene_id, scene_key, enter_pos);
// 
// 			if (!ret)
// 			{
// 				role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
// 				return;
// 			}
// 			
// 		}
// 
// 		role = NULL;
// 	}
}

void FBManager::EnterTeamFB(Role *leader_role, int fb_type, int mode)
{
	if (NULL == leader_role)
	{
		return;
	}

	if (mode < TEAM_FB_MODE_EASY || mode >= TEAM_FB_MAX_MODE)
	{
		return;
	}

	TeamInfo *team_info = leader_role->GetTeamInfo();
	if (NULL == team_info)
	{
		leader_role->NoticeNum(errornum::EN_FB_NOT_IN_TEAM);
		return;
	}

	if (!team_info->InTeam())
	{
		leader_role->NoticeNum(errornum::EN_FB_NOT_IN_TEAM);
		return;
	}

	Team *team = team_info->GetMyTeam();
	if (NULL == team)
	{
		return;
	}

	if (!team->IsLeader(leader_role->GetUserId()))
	{
		leader_role->NoticeNum(errornum::EN_MARRY_RESERVE_MUST_BE_TEAM_LEADER);
		return;
	}
	
	//根据不同的组队副本类型
	int scene_id = 0;
	Posi enter_pos;
	int day_count_id = 0;
	int free_join_times = 0;
	int scene_key = team->GetTeamSeq();

	{
		if (FBCT_YAOSHOUJITANG_TEAM == fb_type)  // 妖兽祭坛组队本
		{
			day_count_id = DayCounter::DAYCOUNT_ID_YAOSHOUJITAN_JOIN_TIMES;

			const YaoShouJiTanTeamFBModeConfig *mode_cfg = LOGIC_CONFIG->GetYaoShouJiTanTeamFbCfg().GetFbModeCfg(mode);
			if (NULL != mode_cfg)
			{
				scene_id = mode_cfg->scene_id;
				enter_pos = mode_cfg->enter_pos;
				free_join_times = mode_cfg->free_join_times;
			}

			if (TeamManager::TEAM_TYPE_YAOSHOUJITANG != team->GetTeamType())
			{
				leader_role->NoticeNum(errornum::EN_FB_TEAM_TYPE_YAOSHOUJITAN_LIMIT);
				return;
			}
		}
		else if (FBCT_MIGONGXIANFU_TEAM == fb_type)		// 迷宫仙府
		{
			day_count_id = DayCounter::DAYCOUNT_ID_MIGOGNXIANFU_JOIN_TIMES;
			scene_key =  MiGongXianFuMgr::Instance().GetChangeSceneKey(team->GetTeamSeq(), MIGONGXIANFU_LAYER_FIRST_INDEX);  //不同的层有不同的scene_key, 做个层的偏移

			const MiGongXianFuModeItemCfg *mode_cfg = LOGIC_CONFIG->GetMiGongXianFuTeamFbCfg().GetModeItemCfg(mode);
			const MiGongXianFuLayerItemCfg *layer_cfg = LOGIC_CONFIG->GetMiGongXianFuTeamFbCfg().GetLayerItemCfg(mode, 0);
			if (NULL != mode_cfg && NULL != layer_cfg)
			{
				scene_id = layer_cfg->scene_id;
				enter_pos = layer_cfg->enter_pos;
				free_join_times = mode_cfg->free_join_times;
			}

			if (TeamManager::TEAM_TYPE_TEAM_MIGONGXIANFU != team->GetTeamType())
			{
				leader_role->NoticeNum(errornum::EN_FB_TEAM_TYPE_MIGONGXIANFU_LIMIT);
				return;
			}
		}

		else
		{
			return;
		}
	}

	int join_user_list[MAX_TEAM_MEMBER_NUM]; memset(join_user_list, 0, sizeof(join_user_list));
	int join_user_count = team->GetMemberUidList(join_user_list, MAX_TEAM_MEMBER_NUM);

	if (join_user_count <= 0)
	{
		return;
	}

	// 检查角色进入条件是否满足
	for (int i = 0; i < join_user_count; ++ i)
	{
		Role *join_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(join_user_list[i]));
		if (NULL == join_role)
		{
			leader_role->NoticeNum(errornum::EN_TEAM_TOWERDEFEND_PARTNER_NOT_ONLINE);
			return;
		}
		
		//次数检查
		if (join_role->GetDayCounter()->GetDayCount(day_count_id) >= free_join_times)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_team_towerdefend_teammate_join_times_limit,
				UidToInt(join_role->GetUserId()), join_role->GetName(), (int)join_role->GetCamp());

			if (length > 0)
			{
				static Protocol::SCSystemMsg sm;
				int sendlen = 0;

				if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3))
				{
					team->BroadcastToAllMember((const char*)&sm, sendlen);
				}
			}

			return;
		}

		// 等级检查
		if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinFB(fb_type, join_role))
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_team_towerdefend_teammate_level_limit,
				UidToInt(join_role->GetUserId()), join_role->GetName(), (int)join_role->GetCamp());

			if (length > 0)
			{
				static Protocol::SCSystemMsg sm;
				int sendlen = 0;

				if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3))
				{
					team->BroadcastToAllMember((const char*)&sm, sendlen);
				}
			}

			return;
		}
	
		// 状态检查
		if (!join_role->GetRoleStatusManager()->CanEnterFB(true, false))
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_team_towerdefend_teammate_can_not_enter,
				UidToInt(join_role->GetUserId()), join_role->GetName(), (int)join_role->GetCamp());

			if (length > 0)
			{
				static Protocol::SCSystemMsg sm;
				int sendlen = 0;

				if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3))
				{
					team->BroadcastToAllMember((const char*)&sm, sendlen);
				}
			}

			return;
		}
	}

	if (scene_id <= 0)
	{
		return;
	}
		
	// 队员进入
	for (int i = 0; i < join_user_count; ++ i)
	{
		Role *join_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(join_user_list[i]));
		if (NULL != join_role)
		{
			int uid = join_role->GetUID();
			GameName user_name;
			join_role->GetName(user_name);

			{
				gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFB: %s[%d] fb_param:%d,%d,%d Team_Towerdefend", 
					user_name, uid, fb_type, 0, 0);

				this->LogFunctionStats(join_role, fb_type, mode, 0, 0);
			}

			bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(join_role, scene_id, scene_key, enter_pos);
			if (!ret)
			{
				join_role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
			}

			join_role = NULL;
		}
	}
}

void FBManager::EnterStoryFb(Role *role, int fb_index)
{
	if (!role->GetFBStory()->CanEnter(fb_index))
	{
		return;
	}

	const StoryFBLevelCfg::ConfigItem *lvl_cfg = LOGIC_CONFIG->GetStoryFBConfig().GetLevelCfg(fb_index);
	if (NULL == lvl_cfg)
	{
		return;
	}

	role->GetFBStory()->OnEnterFB(fb_index);

	this->LogFunctionStats(role, FBCT_STORY_FB, fb_index, 0, 0);

	bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(role, lvl_cfg->scene_id, role->GetUID(), lvl_cfg->enter_pos);
	if (!ret)
	{
		role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
	}

	gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFb: %s[%d] ret:%d fb_param:%d,%d",
		role->GetName(), role->GetUID(), (ret ? 1 : 0), FBCT_STORY_FB, fb_index);
}

void FBManager::EnterMoJieHumanFb(Role *role, int fb_id)
{
	if (!role->GetFBMojie()->CanEnter(fb_id))
	{
		return;
	}

	const HumanPianCfg *cfg = LOGIC_CONFIG->GetMoJieFBConfig().GetHumanPianCfg(fb_id);
	if (NULL == cfg)
	{
		return;
	}

	role->GetFBMojie()->OnEnterFB(fb_id);

	this->LogFunctionStats(role, FBCT_MOJIEAIRENFB, fb_id, 0, 0);

	bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(role, cfg->scene_id, role->GetUID(), cfg->enter_pos);
	if (!ret)
	{
		role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
	}

	gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFb: %s[%d] ret:%d fb_param:%d,%d",
		role->GetName(), role->GetUID(), (ret ? 1 : 0), FBCT_MOJIEAIRENFB, fb_id);
}

void FBManager::NotifySystemMsg(char *str_buff, int str_len, int notice_type, Role * role)
{	
	if (NULL == role) return;

	if (NULL != str_buff && str_len > 0) 
	{
		int sendlen = 0;
		static Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, str_buff, str_len, notice_type))
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&sm, sizeof(sm));
		}
	}
}

void FBManager::EnterDailyFbReal(Role *role, int dailyfb_type, int scene_key)
{
	if (NULL == role) return;

	const DailyFbItem *fb_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetDailyFbItemCfg(dailyfb_type);
	if (NULL == fb_cfg)
	{
		return;
	}

	Knapsack *knapsack = role->GetKnapsack();
	if (NULL == knapsack || knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}
	
	if (role->GetRoleDailyFb()->GetExpFbHistoryEnterTimes() != 0)
	{
		// 材料1
		const ExpFbOtherCfg &exp_other_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetExpOtherCfg();
		static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = exp_other_cfg.item_stuff.item_id;
		stuff_list[0].num = exp_other_cfg.item_stuff.num;
		stuff_list[0].buyable = false;

		if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, true))
		{
			role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}

		if (!ItemExtern::ConsumeItem(role, consume_list, "ExpFb"))
		{
			return;
		}
	}

	this->LogFunctionStats(role, FBCT_DAILY_FB, dailyfb_type, 0, 0);
	
	role->GetRoleDailyFb()->OnEnterExpFb();

	bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(role, fb_cfg->scene_id, scene_key, fb_cfg->birth_pos);
	if (!ret)
	{
		role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
	}

	gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFb: %s[%d] ret:%d fb_param:%d,%d", 
		role->GetName(), role->GetUID(), (ret ? 1 : 0), FBCT_DAILY_FB, dailyfb_type);
}

void FBManager::EnterDailyFb(Role *role, int dailyfb_type, int is_team)
{
	if (NULL == role) return;

	const DailyFbItem *fb_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetDailyFbItemCfg(dailyfb_type);
	if (NULL == fb_cfg)
	{
		return;
	}

	if(0 == is_team) // 个人
	{
		if (!role->GetRoleDailyFb()->CheckEenterCD())
		{
			return;
		}

		if (!role->GetRoleDailyFb()->CanEnter(dailyfb_type))
		{
			return;
		}

		m_daily_fb_key_inc++;
		this->EnterDailyFbReal(role, dailyfb_type, m_daily_fb_key_inc);
	}
	else // 组队
	{
		if (this->CheckTeamEnterDailyFB(role, dailyfb_type))
		{
			Team *team = role->GetTeamInfo()->GetMyTeam();
			if(NULL != team)
			{
				m_daily_fb_key_inc++;
				for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++ i)
				{
					Team::TeamMember *member = team->GetTeamMember(i);
					if (NULL != member)
					{
						Role *team_role = World::GetInstWorld()->GetSceneManager()->GetRole(member->user_id);
						if (NULL != team_role)
						{
							this->EnterDailyFbReal(team_role, dailyfb_type, m_daily_fb_key_inc);
						}
					}
				}
			}
		}
	}
}

void FBManager::EnterTeamEquipFb(Role *role, int layer)
{
	if (NULL == role) return;

	const TeamEquipFBLayerCfg *fb_cfg = LOGIC_CONFIG->GetTeamEquipFBConfig().GetTeamEquipFBLayerCfg(layer);
	if (NULL == fb_cfg)
	{
		return;
	}

	if (this->CheckTeamEnterEquipFb(role, layer))
	{
		Team *team = role->GetTeamInfo()->GetMyTeam();
		if(NULL != team) 
		{
			m_team_equip_fb_key_inc++;

			for (int i = 0; i < team->GetMemberNum() && i < MAX_TEAM_MEMBER_NUM; ++ i)
			{
				Team::TeamMember *member = team->GetTeamMember(i);
				if(NULL != member)
				{
					Role *team_role = World::GetInstWorld()->GetSceneManager()->GetRole(member->user_id);
					if (NULL != team_role)
					{
						this->LogFunctionStats(team_role, FBCT_TEAM_EQUIP_FB, layer, 0, 0);
						bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(team_role, fb_cfg->scene_id, m_team_equip_fb_key_inc, fb_cfg->enter_pos);
						if (!ret)
						{
							team_role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
						}
						gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFb: %s[%d] ret:%d fb_param:%d,%d", team_role->GetName(), team_role->GetUID(), (ret ? 1 : 0), FBCT_TEAM_EQUIP_FB, layer);
					}
				}
			}
		}
	}
}

void FBManager::EnterChallengeFB(Role *role, int fb_index)
{
	if (!role->GetFBChallenge()->CanEnter(fb_index))
	{
		return;
	}

	const ChallengeFBLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetChallengeFBConfig().GetLevelCfg(fb_index);
	if (NULL == level_cfg)
	{
		return;
	}

	this->LogFunctionStats(role, FBCT_CHALLENGE, fb_index, 0, 0);

	int fight_layer = role->GetFBChallenge()->GetFightLayer(level_cfg->level);
	if (fight_layer >= 0 && fight_layer < ChallengeFBLevelCfg::LAYER_PER_LEVEL)
	{
		role->GetFBChallenge()->OnEnterFB(fb_index);

		bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(role, level_cfg->start_scene_id + fight_layer, role->GetUID(), level_cfg->birth_pos);
		if (!ret)
		{
			role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
		}

		gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFb: %s[%d] ret:%d fb_param:%d,%d",
			role->GetName(), role->GetUID(), (ret ? 1 : 0), FBCT_CHALLENGE, fb_index);
	}
}

void FBManager::EnterPhaseFB(Role *role, int fb_index)
{
	if (!role->GetRoleFBPhase()->CanEnter(fb_index))
	{
		return;
	}

	int next_fb_level = role->GetRoleFBPhase()->GetPassLevel(fb_index) + 1;
	const PhaseLevelCfg::ConfigItem *level_cfg = LOGIC_CONFIG->GetPhaseFBConfig().GetLevelCfg(fb_index, next_fb_level);
	if (NULL == level_cfg)
	{
		return;
	}

	role->GetRoleFBPhase()->OnEnterFB(fb_index);

	this->LogFunctionStats(role, FBCT_PHASE, fb_index, next_fb_level, 0);

	bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(role, level_cfg->scene_id, role->GetUID(), level_cfg->enter_pos);
	if (!ret)
	{
		role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
	}

	gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFb: %s[%d] ret:%d fb_param:%d,%d,%d", 
		role->GetName(), role->GetUID(), (ret ? 1 : 0), FBCT_PHASE, fb_index, next_fb_level);
}

void FBManager::EnterPataFB(Role *role)
{
	int level = role->GetRolePataFB()->GetTodayLevel() + 1;
	if (!role->GetRolePataFB()->CanEnter(level))
	{
		return;
	}

	if (role->GetGreateSoldier()->IsBianshenStatus())
	{
		role->NoticeNum(errornum::EN_BIANSHEN_LIMIT);
		return;
	}

	const PataLevelConfig *level_cfg = LOGIC_CONFIG->GetPataFbCfg().GetLevelCfgByLevel(level);
	if (NULL == level_cfg)
	{
		return;
	}

	this->LogFunctionStats(role, FBCT_PATAFB, level, 0, 0);

	bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(role, level_cfg->scene_id, role->GetUID(), level_cfg->birth_pos);
	if (!ret)
	{
		role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
	}

	gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFb: %s[%d] ret:%d fb_param:%d,%d",
		role->GetName(), role->GetUID(), (ret ? 1 : 0), FBCT_PATAFB, level);
}

void FBManager::EnterVipFB(Role *role, int fb_level)
{
	if (!role->GetRoleVipFB()->CanEnter(fb_level))
	{
		return;
	}

	const VipLevelConfig *level_cfg = LOGIC_CONFIG->GetVipFbCfg().GetLevelCfgByLevel(fb_level);
	if (NULL == level_cfg)
	{
		return;
	}
	
	role->GetRoleVipFB()->OnEnterFB(fb_level);
	this->LogFunctionStats(role, FBCT_VIPFB, fb_level, 0, 0);

	bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(role, level_cfg->scene_id, role->GetUID(), level_cfg->birth_pos);
	if (!ret)
	{
		role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
	}

	gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFb: %s[%d] ret:%d fb_param:%d,%d",
		role->GetName(), role->GetUID(), (ret ? 1 : 0), FBCT_VIPFB, fb_level);
}

void FBManager::EnterFunOpenFB(Role *role, int fb_type)
{
 	const FunOpenSceneCfg * fb_cfg = LOGIC_CONFIG->GetFunOpenFBCfg().GetFunOpenSceneCfg(fb_type);
	if (NULL == fb_cfg)
	{
		return;
	}

	bool is_complete = true;
	if (!role->GetTaskManager()->HasTask(fb_cfg->task_id, &is_complete))
	{
		return;
	}

	if (is_complete)
	{
		return;
	}
 
	bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(role, fb_cfg->scene_id, role->GetUID(), fb_cfg->enter_pos);
	if (!ret)
	{
		role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
	}

	gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFb: %s[%d] ret:%d fb_type:%d, fun_open_type:%d",
		role->GetName(), role->GetUID(), (ret ? 1 : 0), FBCT_FUN_OPEN_FB, fb_type);
}

void FBManager::EnterGuideFB(Role *role, int type)
{
	const FbGuideCfg *fb_cfg = LOGIC_CONFIG->GetFbGuideConfig().GetFbGuideCfgByTaskId(type);
	if (NULL == fb_cfg)
	{
		return;
	}
	
	if (0 == fb_cfg->is_target_type)
	{
		bool is_complete = false;
		if (!role->GetTaskManager()->HasTask(fb_cfg->task_id, &is_complete) || is_complete)
		{
			role->NoticeNum(errornum::EN_TACK_FINISH_YET_ERROR);
			return;
		}
	}

	gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFb: %s[%d] fb_type:%d, task_id:%d",
		role->GetName(), role->GetUID(), FBCT_FBGUIDE, type);

	World::GetInstWorld()->GetSceneManager()->GoTo(role, fb_cfg->scene_id, role->GetUID(), fb_cfg->enter_pos);
}

void FBManager::EnterRuneTowerFB(Role *role)
{
	int enter_layer = role->GetRuneSystem()->GetRuneTowerTodayLayer() + 1;
	const RuneTowerLayerCfg *cfg = LOGIC_CONFIG->GetRuneTowerCfg().GetRuneTowerLayerCfg(enter_layer);
	if (NULL == cfg)
	{
		role->NoticeNum(errornum::EN_RUNE_TOWER_LAYER_LIMIT);
		return;
	}

	if (role->GetLevel() < cfg->level_limit)
	{
		role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
		return;
	}

	gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFb: %s[%d] fb_type:%d",
		role->GetName(), role->GetUID(), FBCT_RUNE_TOWER);

	World::GetInstWorld()->GetSceneManager()->GoTo(role, cfg->scene_id, role->GetUID(), cfg->enter_pos);
}

void FBManager::EnterDailyTaskFb(Role *role, int scene_id)
{
	const DailyFBCfg *branch_cfg = LOGIC_CONFIG->GetDailyTaskFBCfg().GetDailyTaskFBConfig(scene_id, role->GetLevel());
	if (NULL == branch_cfg) return;

	gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFb: %s[%d] fb_type:%d",
		role->GetName(), role->GetUID(), FBCT_DAILY_TASK_FB);

	World::GetInstWorld()->GetSceneManager()->GoTo(role, branch_cfg->scene_id, role->GetUID(), branch_cfg->enter_pos);
}

void FBManager::EnterTuituFb(Role* role, int fb_type, int chapter, int level)
{
	if (NULL == role)
	{
		return;
	}

	if (role->GetGreateSoldier()->IsBianshenStatus())
	{
		role->NoticeNum(errornum::EN_BIANSHEN_LIMIT);
		return;
	}

	const TuituFbInfoConfig::Layer* fb_cfg = LOGIC_CONFIG->GetTuituFbConfig().GetLayerCfg(fb_type, chapter, level);
	if (NULL == fb_cfg)
	{
		return;
	}

	if (!role->GetTuituFb()->CanEnterFb(fb_type, chapter, level))
	{
		return;
	}

	World::GetInstWorld()->GetSceneManager()->GoTo(role, fb_cfg->scene_id, role->GetUID(), fb_cfg->enter_pos);
}

void FBManager::EnterPersonalDefendFB(Role *role, int level)
{
	if (NULL == role) return;

	if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinFB(FBManager::FBCT_TOWERDEFEND_PERSONAL, role))
	{
		role->NoticeNum(errornum::EN_FBMANAGER_JOIN_FB_LIMIT);
		return;
	}

	const TowerDefendLevelCfg::ConfigItem * lvl_cfg = LOGIC_CONFIG->GetTowerDefendConfig().GetLevelCfg(level);
	if (NULL == lvl_cfg)
	{
		return;
	}

	bool can_enter = role->GetFBTowerDefend()->PersonalFBCanEnter(level);
	if (!can_enter)
	{
		return;
	}

	role->GetFBTowerDefend()->OnPersonalFBEnter();

	gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFB: %s[%d] fb_type:%d fb_level:%d",
		role->GetName(), role->GetUID(), FBCT_TOWERDEFEND_PERSONAL, level);

	World::GetInstWorld()->GetSceneManager()->GoTo(role, lvl_cfg->scene_id, role->GetUID(), lvl_cfg->birth_pos);
}

void FBManager::EnterPersonalHolidayGuardFB(Role *role, int level, int npc_index)
{
	if (nullptr == role) return;

	if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinFB(FBManager::FBCT_TOWERDEFEND_PERSONAL, role))
	{
		role->NoticeNum(errornum::EN_FBMANAGER_JOIN_FB_LIMIT);
		return;
	}

	const HolidayGuardLevelCfg::ConfigItem * lvl_cfg = LOGIC_CONFIG->GetHolidayGuardConfig().GetLevelCfg(level);
	if (nullptr == lvl_cfg)
	{
		return;
	}

	bool can_enter = role->GetFBTowerDefend()->PersonalHolidayGuardFBCanEnter();
	if (!can_enter)
	{
		return;
	}

	role->GetFBTowerDefend()->OnPersonalHolidayGuardFBEnter();

	gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFB: %s[%d] fb_type:%d fb_level:%d",
		role->GetName(), role->GetUID(), FBCT_TOWERDEFEND_PERSONAL, level);

	World::GetInstWorld()->GetSceneManager()->GoTo(role, lvl_cfg->scene_id, role->GetUID(), lvl_cfg->birth_pos);

	RandActivityHolidayGuard *holiday_guard_activity = (RandActivityHolidayGuard*)RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_HOLIDAY_GUARD);
	if (nullptr != holiday_guard_activity)
	{
		holiday_guard_activity->DelNpc(npc_index);
	}
}

void FBManager::EnterQingYuanShengDiFb(Role* role, int layer)
{
	const QYSDLayerCfg *cfg = LOGIC_CONFIG->GetQinYuanShengDiCfg().GetQYSDLayerCfg(layer);
	if (NULL == cfg)
	{
		return;
	}

	const tm *local_tm = EngineAdapter::Instance().LocalTime();
	int day_time = local_tm->tm_hour * 100 + local_tm->tm_min;

	if (day_time < cfg->begin_time || day_time > cfg->end_time)
	{
		role->NoticeNum(errornum::EN_QING_YUAN_SHENG_DI_TIME_LIMIT);
		return;
	}

	QingyuanGlobal *qy_blobal = GLOBAL_ROLE_MGR.GetQingyuanGlobal(role->GetUID());
	if (NULL == qy_blobal || qy_blobal->GetMarryLevel() < cfg->enter_level)
	{
		role->NoticeNum(errornum::EN_QING_YUAN_SHENG_DI_LEVEL_LIMIT);
		return;
	}

	int max_layer = LOGIC_CONFIG->GetQinYuanShengDiCfg().GetCanEnterMaxLayer(qy_blobal->GetMarryLevel());
	if (layer < max_layer - 2)
	{
		role->NoticeNum(errornum::EN_QING_YUAN_SHENG_DI_LAYER_LIMIT);
		return;
	}

	World::GetInstWorld()->GetSceneManager()->GoTo(role, cfg->scene_id, layer, cfg->birth_pos);
}

void FBManager::EnterNewEquipFb(Role* role, int chapter, int level)
{
	int target_scene_id = 0;
	Posi target_scene_pos;
	int scene_key = 0;

	if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinFB(FBManager::FBCT_NEQ_FB, role))
	{
		role->NoticeNum(errornum::EN_FBMANAGER_JOIN_FB_LIMIT);
		return;
	}

	const NeqFBLevelCfg *lvl_cfg = LOGIC_CONFIG->GetNeqFBConfig().GetLevelCfg(chapter, level);
	if (NULL == lvl_cfg)
	{
		return;
	}

	bool can_enter = role->GetFBEquip()->CanEnterNeq(chapter, level);
	if (!can_enter)
	{
		return;
	}

	target_scene_pos.x = lvl_cfg->pos_x;
	target_scene_pos.y = lvl_cfg->pos_y;
	target_scene_id = lvl_cfg->scene_id;
	scene_key = role->GetUID();

	role->GetFBEquip()->OnNeqEnter(chapter, level);

	//this->LogFunctionStats(role, FBManager::FBCT_NEQ_FB, chapter, level);

	//int uid = role->GetUID();
	GameName user_name;
	role->GetName(user_name);

	World::GetInstWorld()->GetSceneManager()->GoTo(role, target_scene_id, scene_key, target_scene_pos);
}

void FBManager::EnterArmorDefendFb(Role* role, int level)
{
	if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinFB(FBManager::FBCT_ARMOR_FB, role))
	{
		role->NoticeNum(errornum::EN_FBMANAGER_JOIN_FB_LIMIT);
		return;
	}

	const ArmorDefendLevelCfg::ConfigItem * lvl_cfg = LOGIC_CONFIG->GetArmorDefendConfig().GetLevelCfg(level);
	if (NULL == lvl_cfg)
	{
		return;
	}

	bool can_enter = role->GetRoleFbArmorDefend()->PersonalFBCanEnter(level);
	if (!can_enter)
	{
		return;
	}

	Posi target_scene_pos = lvl_cfg->birth_pos;
	int target_scene_id = lvl_cfg->scene_id;
	int scene_key = role->GetUID();

	role->GetRoleFbArmorDefend()->OnPersonalFBEnter();


	//this->LogFunctionStats(role, fb_type, param1, param2);

	//int uid = role->GetUID();
	GameName user_name;
	role->GetName(user_name);

	bool is_new_player = role->GetRoleFbArmorDefend()->IsNewPlayer();

	SpecialArmorDefend* special_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialArmorDefend(target_scene_id, scene_key, true);
	if (NULL != special_logic)
	{
		special_logic->SetIsPlayerGuider(is_new_player);
	}

	World::GetInstWorld()->GetSceneManager()->GoTo(role, target_scene_id, scene_key, target_scene_pos);

}

void FBManager::OnPersonalBossBuyEnterTimes(Role *role)
{
	if (role == nullptr) return;

	int had_buy_times = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_PERSON_BOSS_BUY_TIMES);
	int can_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_PERSON_BOSS_BUY_TIMES);
	if (had_buy_times >= can_buy_times)
	{
		role->NoticeNum(errornum::EN_PERSONAL_BOSS_BUY_LIMIT_VIP);
		return;
	}

	int need_gold = LOGIC_CONFIG->GetPersonBossConfig().GetBuyCostByTimes(role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_PERSON_BOSS_BUY_TIMES) + 1);
	if (need_gold <= 0)
	{
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(need_gold))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->UseGold(need_gold, "PersonalBossBuyEnterTimes"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_PERSON_BOSS_BUY_TIMES);
}

bool FBManager::EnterPersonBossFB(Role *role, int layer)
{
	if (NULL == role)
	{
		return false;
	}

	if (layer <= 0 || layer > MAX_PERSON_BOSS_LAYER)
	{
		return false;
	}

	const PersonBossCfgItem *cfg = LOGIC_CONFIG->GetPersonBossConfig().GetBossSceneCfg(layer);
	if (NULL == cfg)
	{
		return false;
	}
	int enter_times = role->GetRolePersonBossFb()->GetTodayEnterByLayer(layer);
	if (enter_times >= cfg->day_enter_times)
	{
		role->NoticeNum(errornum::EN_FB_DAY_COUNT_LIMIT);
		return false;
	}

	if (role->GetLevel() < cfg->need_level)
	{
		role->NoticeNum(errornum::EN_FBMANAGER_JOIN_FB_LIMIT);
		return false;
	}

	if (!role->GetKnapsack()->ConsumeItemOrUseGold(cfg->need_item_id, cfg->need_item_num, "FBManager::EnterPersonBossFB"))
	{
		return false;
	}

	this->LogFunctionStats(role, FBCT_PERSON_BOSS, layer, 0, 0);


	bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(role, cfg->scene_id, role->GetUID(), cfg->enter_pos);
	if (!ret)
	{
		role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
	}

	return true;
}

bool FBManager::EnterXinMoFB(Role * role)
{
	int index = 0;

	// 四转有三个副本需要通关  屏蔽
	/*if (role->GetProfLevel() == CANGLONG_LIGHT_UP_PROF_LEVEL_LIMIT)
	{
		int canglong_lv = role->GetRoleZhuanZhi()->GetCangLongLevel();
		if (canglong_lv <= 0 || 0 != canglong_lv % MAX_CANGLONG_LEVEL_PER_GRADE)
		{
			return false;
		}

		if (MAX_CANGLONG_LEVEL_PER_GRADE > 0)
		{
			index = static_cast<int>((canglong_lv - 1) / MAX_CANGLONG_LEVEL_PER_GRADE);
		}
	}*/

	const ZhuanzhiFbCfg *cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiFbCfg(role->GetProfLevel() + 1, index);
	if (NULL == cfg)
	{
		role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
		return false;
	}

	if (role->GetLevel() < cfg->level_limit)
	{
		role->NoticeNum(errornum::EN_FB_ROLE_LEVEL_LIMIT);
		return false;
	}

	this->LogFunctionStats(role, FBCT_ZHUANZHI_PERSONAL, cfg->prof_level, 0, 0);

	World::GetInstWorld()->GetSceneManager()->GoTo(role, cfg->scene_id, role->GetUID(), cfg->enter_pos);

	return true;
}

void FBManager::EnterBuildTowerFB(Role * role)
{
	if (NULL == role)
	{
		return ;
	}

	const BuildTowerFbOtherCfg &other_cfg = LOGIC_CONFIG->GetBuildTowerFbConfig().GetBuildTowerFbOtherCfg();

	if (role->GetLevel() < other_cfg.enter_level)
	{
		role->NoticeNum(errornum::EN_FB_ROLE_LEVEL_LIMIT);
		return ;
	}

	int buy_times = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_BUILD_TOWER_FB_BUY_TIMES);
	int enter_times = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_BUILD_TOWER_FB_ENTER_TIMES);

	if (enter_times >= buy_times + other_cfg.enter_free_times)
	{
		role->NoticeNum(errornum::EN_FB_DAY_COUNT_LIMIT);
		return ;
	}

	role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_BUILD_TOWER_FB_ENTER_TIMES);
	EventHandler::Instance().OnEnterBuildTowerFB(role);

	World::GetInstWorld()->GetSceneManager()->GoTo(role, other_cfg.scene_id, role->GetUID(), other_cfg.born_pos);
}

void FBManager::EnterTeamTowerDefendFb(Role *leader_role, int mode)
{
	if (NULL == leader_role)
	{
		return;
	}

	TeamInfo *team_info = leader_role->GetTeamInfo();
	if (NULL == team_info)
	{
		leader_role->NoticeNum(errornum::EN_FB_NOT_IN_TEAM);
		return;
	}

	if (!team_info->InTeam())
	{
		leader_role->NoticeNum(errornum::EN_FB_NOT_IN_TEAM);
		return;
	}

	Team *team = team_info->GetMyTeam();
	if (NULL == team)
	{
		return;
	}

	if (!team->IsLeader(leader_role->GetUserId()))
	{
		leader_role->NoticeNum(errornum::EN_MARRY_RESERVE_MUST_BE_TEAM_LEADER);
		return;
	}

	// 队伍类型判断
	if (TeamManager::TEAM_TYPE_TEAM_TOWERDEFEND != team->GetTeamType())
	{
		leader_role->NoticeNum(errornum::EN_FB_TEAM_TYPE_TOWERDEFEND_LIMIT);
		return;
	}

	// 队长检查进入次数,队友有就扣除
	const TeamTowerDefendOtherCfg &other_cfg = LOGIC_CONFIG->GetTeamTowerDefendConfig().GetOtherCfg();
	int join_times = leader_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_TEAM_TOWERDEFEND_JOIN_TIMES);

	if (join_times >= other_cfg.free_join_times)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_team_towerdefend_teammate_join_times_limit,
			UidToInt(leader_role->GetUserId()), leader_role->GetName(), (int)leader_role->GetCamp());

		if (length > 0)
		{
			static Protocol::SCSystemMsg sm;
			int sendlen = 0;

			if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_PERSONAL_NOTICE))
			{
				team->BroadcastToAllMember((const char*)&sm, sendlen);
			}
		}

		return;
	}

	int join_user_list[MAX_TEAM_MEMBER_NUM]; memset(join_user_list, 0, sizeof(join_user_list));
	int join_user_count = team->GetMemberUidList(join_user_list, MAX_TEAM_MEMBER_NUM);

	if (join_user_count <= 0)
	{
		return;
	}

	short member_min_wave = TEAM_TOWERDEFEND_WAVE_COUNT;

	// 检查角色进入条件是否满足
	int attr_type_role_count[TEAM_TOWERDEFEND_ATTRTYPE_MAX];
	memset(attr_type_role_count, 0, sizeof(attr_type_role_count));
	std::map<int, UserID, std::greater<int> > capablity_user_map;   // 按战力排序map

	for (int i = 0; i < join_user_count && i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		Role *join_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(join_user_list[i]));
		if (NULL == join_role)
		{
			leader_role->NoticeNum(errornum::EN_TEAM_TOWERDEFEND_PARTNER_NOT_ONLINE);
			return;
		}

		// 等级判断
		if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinFB(FBManager::FBCT_TOWERDEFEND_TEAM, join_role))
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_team_towerdefend_teammate_level_limit,
				UidToInt(join_role->GetUserId()), join_role->GetName(), (int)join_role->GetCamp());

			if (length > 0)
			{
				static Protocol::SCSystemMsg sm;
				int sendlen = 0;

				if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_PERSONAL_NOTICE))
				{
					team->BroadcastToAllMember((const char*)&sm, sendlen);
				}
			}

			return;
		}

		// 状态判断
		if (!join_role->GetRoleStatusManager()->CanEnterFB(true, false))
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_team_towerdefend_teammate_can_not_enter,
				UidToInt(join_role->GetUserId()), join_role->GetName(), (int)join_role->GetCamp());

			if (length > 0)
			{
				static Protocol::SCSystemMsg sm;
				int sendlen = 0;

				if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_PERSONAL_NOTICE))
				{
					team->BroadcastToAllMember((const char*)&sm, sendlen);
				}
			}

			return;
		}

		// 属性设置
		int attr_type = join_role->GetFBTowerDefend()->GetTeamAttrType();

		if (attr_type > TEAM_TOWERDEFEND_ATTRTYPE_INVALID && attr_type < TEAM_TOWERDEFEND_ATTRTYPE_MAX)  // 如果该角色已经选择，那么记录
		{
			attr_type_role_count[attr_type] += 1;
		}
		else																							 // 如果该角色没选择，那么按战力排序，待会分配
		{
			capablity_user_map[join_role->GetCapability()->GetTotalCapability()] = join_role->GetUserId();
		}

		short min_wave = join_role->GetFBTowerDefend()->GetTeamPassdeWave();
		min_wave = min_wave/ 10 * 10;
		if (min_wave < member_min_wave && min_wave >=0)
		{
			member_min_wave = min_wave;
		}
	}

	// 如果没设置模式，服务器自动帮他设置
	for (std::map<int, UserID, std::greater<int> >::iterator it = capablity_user_map.begin(); it != capablity_user_map.end(); ++it)
	{
		UserID &join_user_id = it->second;

		Role *join_role = World::GetInstWorld()->GetSceneManager()->GetRole(join_user_id);
		if (NULL == join_role)
		{
			leader_role->NoticeNum(errornum::EN_TEAM_TOWERDEFEND_PARTNER_NOT_ONLINE);
			return;
		}

		// 默认的枚举顺序就是按战力来排序的 
		for (int j = TEAM_TOWERDEFEND_ATTRTYPE_INVALID + 1; j < TEAM_TOWERDEFEND_ATTRTYPE_MAX; ++j)
		{
			if (0 == attr_type_role_count[j])
			{
				join_role->GetFBTowerDefend()->SetTeamAttrType(static_cast<char>(j));
				attr_type_role_count[j] += 1;

				break;
			}
		}
	}

	// 检查
	for (int i = TEAM_TOWERDEFEND_ATTRTYPE_INVALID + 1; i < TEAM_TOWERDEFEND_ATTRTYPE_MAX; ++i)
	{
		if (attr_type_role_count[i] > 1)
		{
			leader_role->NoticeNum(errornum::EN_TEAM_TOWERDEFEND_REPEAT_ATTR_TYPE);
			return;
		}
	}

	int scene_key = team->GetTeamSeq();

	// 队员进入
	for (int i = 0; i < join_user_count; ++i)
	{
		Role *join_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(join_user_list[i]));
		if (NULL != join_role)
		{
			// 第一进入该副本不扣除次数
// 			if (0 == join_role->GetCommonDataParam()->has_enter_team_fb_tower_defend)
// 			{
// 				join_role->GetCommonDataParam()->has_enter_team_fb_tower_defend = 1;
// 				join_role->GetRoleTeamFB()->SendInfo();
// 			}
// 			else
// 			{

			// 扣除次数，没有也要加，加至最大值
			join_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_TEAM_TOWERDEFEND_JOIN_TIMES);
			
			join_role->GetFBTowerDefend()->SetTeamMemberMinWave(member_min_wave - 1);
			this->LogFunctionStats(join_role, FBCT_TOWERDEFEND_TEAM, 0, 0, 0);

			bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(join_role, other_cfg.team_enter_sceneid, scene_key, other_cfg.team_enter_pos);
			if (!ret)
			{
				join_role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
			}

			join_role = NULL;
		}
	}
}

void FBManager::EnterEquipTeamFb(Role *role, int param1)
{
	TeamInfo *team_info = role->GetTeamInfo();
	if (!team_info->InTeam())
	{
		role->NoticeNum(errornum::EN_FB_NOT_IN_TEAM);
		return;
	}

	if (!team_info->IsLeader()) // 组队状态下 只能由队长申请进入副本
	{
		role->NoticeNum(errornum::EN_FB_NOT_TEAM_LEADER);
		return;
	}

	Team *team = TeamManager::Instance().GetTeam(team_info->GetTeamIndex());
	if (nullptr == team || TeamManager::TEAM_TYPE_EQUIP_TEAM_FB != team->GetTeamType())
	{
		return;
	}

	const FBEquipLayerCfg *layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgByLayer(1);
	if (nullptr == layer_cfg)
	{
		return;
	}

	int target_scene_id = layer_cfg->scene_id;
	Posi enter_pos;
	enter_pos.x = 0;
	enter_pos.y = 0;
	int scene_key = team->GetTeamSeq();

	int member_uid_list[MAX_TEAM_MEMBER_NUM] = { 0 };
	int member_num = team->GetMemberUidList(member_uid_list, MAX_TEAM_MEMBER_NUM);
	int member_min_layer = -1;  // 组队成员中最小的历史通过层数

	{
		// 进入次数队长必须有，队员有则扣除
		const FBEquipOtherCfg &other_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg();
		int join_times = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_TEAM_EQUIP_FB_JOIN_TIMES);

		if (join_times >= other_cfg.join_times)
		{
			role->NoticeNum(errornum::EN_FB_TEAM_DAY_COUNT_LIMIT);
			return;
		}

		// 检查等级、状态条件
		for (int i = 0; i < member_num && i < MAX_TEAM_MEMBER_NUM; ++i)
		{
			Role *temp_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
			if (nullptr == temp_role) continue;

			if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinFB(FBCT_EQUIP_TEAM_FB, temp_role))
			{
				role->NoticeNum(errornum::EN_FB_TEAM_LEVEL_REQUIRE);
				return;
			}

			if (!temp_role->GetRoleStatusManager()->CanEnterFB())
			{
				role->NoticeNum(errornum::EN_FB_TEAMMATE_CANOT_ENTER_FB);
				return;
			}

			int member_layer = temp_role->GetFBEquip()->GetTeamFBPassMaxLevel();
			if (-1 == member_min_layer || member_layer < member_min_layer)
			{
				member_min_layer = member_layer;
			}
		}
	}

	{
		// 修正要进入的层数
		if (member_min_layer > 0)
		{
			member_min_layer = (member_min_layer / 10) * 10;
			const FBEquipLayerCfg *temp_layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgByLayer(member_min_layer);
			if (nullptr != temp_layer_cfg)
			{
				target_scene_id = temp_layer_cfg->scene_id;
			}
		}
	}

	// 队员进入
	for (int i = 0; i < member_num && i < MAX_TEAM_MEMBER_NUM; ++i)
	{
		Role *temp_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
		if (nullptr == temp_role) continue;

		temp_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_TEAM_EQUIP_FB_JOIN_TIMES);

		int temp_role_join_times = temp_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_TEAM_EQUIP_FB_JOIN_TIMES);

		if (temp_role_join_times > LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg().join_times)		//进入次数大于每天限制的参与次数，当天最大通关层数不再累加
		{
			temp_role->GetFBEquip()->SetSpecialLimitFlag(RoleFBEquip::NO_REWARD);
		}

		// 发放之前层次的掉落奖励
		//temp_role->GetFBEquip()->GiveEquipTeamFbLayerReward(member_min_layer);
		temp_role->GetFBEquip()->SetMemberMinLayer(member_min_layer);								//记录奖励层数

		if (temp_role_join_times <= LOGIC_CONFIG->GetEquipFBConfig().GetOtherCfg().join_times)		//进入次数小于每天限制的参与次数，有奖励
		{
			temp_role->GetFBEquip()->SetPreLayerRewardFlag(RoleFBEquip::GIVE_REWARD);
		}

		this->LogFunctionStats(temp_role, FBCT_EQUIP_TEAM_FB, 0, 0, 0);

		EventHandler::Instance().OnEnterEquipTeamFb(temp_role);

		World::GetInstWorld()->GetSceneManager()->GoTo(temp_role, target_scene_id, scene_key, enter_pos);
		}
	}

void FBManager::OnUserLogin(Role *role)
{
	if (0 != role->GetLoverUid())
	{
		std::map<int, int>::iterator it = m_opening_hunyan_register.find(role->GetUID());
		if (it != m_opening_hunyan_register.end())
		{
			const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();

			Scene *huyan_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(hunyan_cfg.hunyan_scene_id, it->second);
			if (NULL != huyan_scene && Scene::SCENE_TYPE_HUN_YAN_FB == huyan_scene->GetSceneType())
			{
				SpecialHunyan *hunyan_logic = dynamic_cast<SpecialHunyan*>(huyan_scene->GetSpecialLogic());
				if (nullptr != hunyan_logic && !hunyan_logic->HunyanIsStop())
				{
					hunyan_logic->SendHunyanInfo(role, Protocol::HUNYAN_NOTIFY_REASON_LOGIN);
				}
			}
			else 
			{
				m_opening_hunyan_register.erase(it);
			}
		}
	}

	int size = static_cast<int>(m_all_hunyan_info_list.size());
	for(int i = 0; i < size; i ++)
	{
		Scene *hunyan_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig().hunyan_scene_id, m_all_hunyan_info_list[i]);
		if(NULL != hunyan_scene)
		{
			SpecialHunyan *special_hunyan = dynamic_cast<SpecialHunyan*>(hunyan_scene->GetSpecialLogic());
			if(NULL != special_hunyan)
			{ 
				special_hunyan->SendHunyanInfo(role, Protocol::HUNYAN_NOTIFY_REASON_LOGIN);
			}
		}
	}
}

bool FBManager::IsHunyanRunning(Role *role)
{
	std::map<int, int>::iterator it = m_opening_hunyan_register.find(role->GetUID());
	if (it != m_opening_hunyan_register.end())
	{
		return true;
	}

	return false;
}

void FBManager::RegisterHunyanKey(Role *role, int fb_key)
{
	if (NULL != role && fb_key > 0)
	{
		m_opening_hunyan_register[role->GetUID()] = fb_key;
	}
}

void FBManager::UnRegisterHunyanKey(int uid)
{
	if (uid > 0)
	{
		m_opening_hunyan_register.erase(uid);
	}
}

int FBManager::GetMyHunyanFBKey(int uid)
{
	if (uid > 0)
	{
		std::map<int, int>::iterator it = m_opening_hunyan_register.find(uid);
		if (it != m_opening_hunyan_register.end())
		{
			return it->second;
		}
	}

	return -1;
}

void FBManager::ReqNextLevel(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	Scene *scene = role->GetScene();
	if (NULL == scene)
	{
		return;
	}

	SpecialLogic *special_logic = scene->GetSpecialLogic();
	if (NULL == special_logic)
	{
		return;
	}

	switch(special_logic->GetSceneType())
	{
	case Scene::SCENE_TYPE_GUILD_MONSTER:
		{
			SpecialGuildMonster *special_guildmonster = dynamic_cast<SpecialGuildMonster*>(special_logic);
			if (nullptr != special_guildmonster)
				special_guildmonster->OnReqNextLevel();
		}
		break;

	case Scene::SCENE_TYPE_ZHANSHENDIAN_FB:
		{
			SpecialZhanShenDian *special_zhanshendian = dynamic_cast<SpecialZhanShenDian*>(special_logic);
			if (nullptr != special_zhanshendian)
				special_zhanshendian->OnReqNextLevel();
		}
		break;

	case Scene::SCENE_TYPE_WUSHUANG_FB:
		{
			SpecialWushuangFB *special_wushuang = dynamic_cast<SpecialWushuangFB*>(special_logic);
			if (nullptr != special_wushuang)
				special_wushuang->OnReqNextLevel();
		}
		break;

	case Scene::SCENE_TYPE_PATA_FB:
		{
			SpecialPataFb *special_pata = dynamic_cast<SpecialPataFb*>(special_logic);
			if (nullptr != special_pata)
				special_pata->OnReqNextLevel();
		}
		break;

	case Scene::SCENE_TYPE_RUNE_TOWER:
		{
			SpecialRuneTower *special_runetower = dynamic_cast<SpecialRuneTower*>(special_logic);
			if (nullptr != special_runetower)
				special_runetower->OnReqNextLevel();
		}
		break;

	case Scene::SCENE_TYPE_TUITU_FB:
		{
			SpecialTuitu* special_tuitu = dynamic_cast<SpecialTuitu*>(special_logic);
			if (nullptr != special_tuitu)
				special_tuitu->OnReqNextLevel();
		}
		break;
	}
}

void FBManager::SaveHunYanInviteInfo(Role *role, int scene_key)
{
	if(role == NULL && scene_key < 0)
	{
		return;
	}

	int size = static_cast<int>(m_all_hunyan_info_list.size());
	for(int i = 0; i < size; i ++)
	{
		if (scene_key == m_all_hunyan_info_list[i])
		{
			return;
		}
	}

	m_all_hunyan_info_list.push_back(scene_key);

// 	std::map<int, HunYanInviteInfo *>::iterator it1 = m_hunyan_info_map.find(role->GetUID());
// 	if(it1 != m_hunyan_info_map.end())
// 	{
// 		HunYanInviteInfo *invite_info = it1->second;
// 
// 		bool is_find = false;
// 		int size = static_cast<int>(invite_info->invite_list.size());
// 		for(int i = 0; i < size; i ++)
// 		{
// 			if(scene_key == invite_info->invite_list[i])
// 			{
// 				is_find = true;
// 				break;
// 			}
// 		}
// 		if(!is_find)
// 		{
// 			invite_info->invite_list.push_back(scene_key);
// 		}
// 	}
// 	else
// 	{
// 		HunYanInviteInfo *invite_info = new HunYanInviteInfo();
// 		invite_info->invite_list.push_back(scene_key);
// 		m_hunyan_info_map[role->GetUID()] = invite_info;
// 	}
}

void FBManager::DeleteHunYanInviteInfo(int uid, int scene_key)
{
	if(scene_key < 0)
	{
		return;
	}

	for (std::vector<int>::iterator it_vec = m_all_hunyan_info_list.begin(); it_vec != m_all_hunyan_info_list.end(); it_vec++)
	{
		if (scene_key == *it_vec)
		{
			m_all_hunyan_info_list.erase(it_vec);
			break;
		}
	}
	
	this->SendHunYanFbInviteInfoToWorld();

// 	std::map<int, HunYanInviteInfo *>::iterator it = m_hunyan_info_map.find(uid);
// 	if(it != m_hunyan_info_map.end())
// 	{
//  		HunYanInviteInfo *invite_info = it->second;
// 
// 		for (std::vector<int>::iterator it_vec = invite_info->invite_list.begin(); it_vec != invite_info->invite_list.end(); it_vec++)
// 		{
// 			if (scene_key == *it_vec)
// 			{
// 				invite_info->invite_list.erase(it_vec);
// 				break;
// 			}
// 		}
// 		
// 		if(0 == static_cast<int>(invite_info->invite_list.size()))
// 		{
// 			delete invite_info;
// 			m_hunyan_info_map.erase(it);
// 		}
// 	}
}

void FBManager::GetAllHunYanFbInviteInfo(Protocol::SCQingyuanHunyanInviteInfo &qhii, Role *role)
{
	int size = static_cast<int>(m_all_hunyan_info_list.size());
	for(int i = 0; i < size; i ++)
	{
		Scene *hunyan_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig().hunyan_scene_id, m_all_hunyan_info_list[i]);
		if(NULL != hunyan_scene)
		{
			SpecialHunyan *special_hunyan = (SpecialHunyan *)hunyan_scene->GetSpecialLogic();
			if(NULL != special_hunyan)
			{ 
				qhii.scq_invite_list[i].yanhui_fb_key = special_hunyan->GetSceneKey();
				qhii.scq_invite_list[i].yanhui_type = special_hunyan->GetYanHuiType();
				special_hunyan->GetMenName(qhii.scq_invite_list[i].mem_name);
				special_hunyan->GetWomenNmae(qhii.scq_invite_list[i].women_name);
				if (NULL != role)
				{
					qhii.scq_invite_list[i].gather_num = special_hunyan->GetGatherByUid(role->GetUID());
				}
				else
				{
					qhii.scq_invite_list[i].gather_num = 0;
				}
				
				++ qhii.invite_count;
			}
		}
	}
}

void FBManager::SendHunYanFbInviteInfoToRole(Role *role)
{
	static Protocol::SCQingyuanHunyanInviteInfo qhii;
	qhii.invite_count = 0;
	this->GetAllHunYanFbInviteInfo(qhii, role);

	unsigned int sendlen = sizeof(qhii) - sizeof(qhii.scq_invite_list[0]) * (Protocol::SCQingyuanHunyanInviteInfo::INVITE_NUM - qhii.invite_count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&qhii, sendlen);
}

void FBManager::SendHunYanFbInviteInfoToWorld()
{
	static Protocol::SCQingyuanHunyanInviteInfo qhii;
	qhii.invite_count = 0;
	this->GetAllHunYanFbInviteInfo(qhii, NULL);

	unsigned int sendlen = sizeof(qhii) - sizeof(qhii.scq_invite_list[0]) * (Protocol::SCQingyuanHunyanInviteInfo::INVITE_NUM - qhii.invite_count);
	World::GetInstWorld()->SendToAllGateway((const char*)&qhii, sendlen);
}	

void FBManager::SendTeamEquipFBCountInfo(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	CommonDataParam *common_data = role->GetCommonDataParam();
	if (NULL == common_data)
	{
		return;
	}

	Protocol::SCTeamEquipFbDropCountInfo cmd;
	cmd.team_equip_fb_day_count = common_data->team_equip_fb_day_count;
	cmd.team_equip_fb_day_buy_count = common_data->team_equip_fb_day_buy_count;
	cmd.team_equip_fb_pass_flag = common_data->team_equip_fb_pass_flag;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

void FBManager::TeamEquipFbBuyDropTimes(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	CommonDataParam *common_data = role->GetCommonDataParam();
	if (NULL == common_data)
	{
		return;
	}

	if (common_data->team_equip_fb_day_buy_count >= LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_FB_TEAM_EQUIP_COUNT))
	{
		role->NoticeNum(errornum::EN_TEAM_EQUIP_FB_BUY_TIME_VIP_LIMIT);
		return;
	}

	int need_gold = LOGIC_CONFIG->GetTeamEquipFBConfig().GetTeamEquipFbBuyTimesCost(common_data->team_equip_fb_day_buy_count);
	if (need_gold <= 0)
	{
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->UseAllGold(need_gold, "BuyTeamEquipFbDropTimes"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	common_data->team_equip_fb_day_buy_count++;

	this->SendTeamEquipFBCountInfo(role);
}

bool FBManager::CheckTeamEnterDailyFB(Role *role, int dailyfb_type)
{
	if (NULL == role) return false;

	Team *team = role->GetTeamInfo()->GetMyTeam();
	if(NULL == team)
	{
		role->NoticeNum(errornum::EN_FB_ENTER_FB_PLEAR_TEAM);
		return false;
	}

	if (!team->IsLeader(role->GetUserId()))
	{
		role->NoticeNum(errornum::EN_FB_ENTER_FB_NOT_LEADER);
		return false;
	}

	if (1 == team->GetMemberNum())
	{
		role->NoticeNum(errornum::EN_FB_ENTER_FB_TEAM_NUM_INSU);
		return false;
	}

	// 检查队员能否进入
	for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++ i)
	{
		Team::TeamMember *member = team->GetTeamMember(i);
		if (NULL == member)
		{
			continue;
		}

		Role *team_role = World::GetInstWorld()->GetSceneManager()->GetRole(member->user_id);
		if (NULL == team_role)
		{	
			continue;
		}

		bool is_can_enter = true;

		if (!team_role->GetRoleStatusManager()->CanEnterFB(true, false))
		{
			is_can_enter = false;
		}

		if (is_can_enter && team_role->GetKnapsack()->IsLock())
		{
			team_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
			is_can_enter = false;
		}

		if (is_can_enter && !team_role->GetRoleDailyFb()->CheckEenterCD())
		{
			is_can_enter = false;
		}
	
		if (is_can_enter && !team_role->GetRoleDailyFb()->CanEnter(dailyfb_type))
		{
			is_can_enter = false;
		}

		// 在经验本外且不是第一次则检查材料
		if (team_role->GetScene()->GetSceneType() != Scene::SCENE_TYPE_EXPFB &&
			team_role->GetRoleDailyFb()->GetExpFbHistoryEnterTimes() != 0)
		{
			// 材料1
			const ExpFbOtherCfg &exp_other_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetExpOtherCfg();
			static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
			ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
			stuff_list[0].item_id = exp_other_cfg.item_stuff.item_id;
			stuff_list[0].num = exp_other_cfg.item_stuff.num;
			stuff_list[0].buyable = false;

			if (is_can_enter && !ItemExtern::GetConsumeListByOrder(team_role, false, 1, stuff_list, &consume_list, true))
			{
				team_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
				gstr::SendError(team_role, "fb_enter_stuff_not_enough");
				is_can_enter = false;
			}
		}

		// 有队友不满足条件提示
		if (!is_can_enter)
		{
			int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_dailyfb_cant_enter_fb, team_role->GetUID(),
				team_role->GetName(), (int)team_role->GetCamp());

			if (len > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3, role);
			}

			return false;
		}
	}

	return true;
}

bool FBManager::CheckTeamEnterEquipFb(Role *role, int layer)
{
	if (NULL == role) return false;

	Team *team = role->GetTeamInfo()->GetMyTeam();
	if(NULL == team) 
	{
		role->NoticeNum(errornum::EN_ENTER_TEAM_EQUIP_FB_TEAM_LIMIT);
		return false;
	}

	if (!team->IsLeader(role->GetUserId()))
	{
		role->NoticeNum(errornum::EN_ENTER_TEAM_EQUIP_FB_LEADER_LIMIT);
		return false;
	}

	const TeamEquipFBLayerCfg *fb_cfg = LOGIC_CONFIG->GetTeamEquipFBConfig().GetTeamEquipFBLayerCfg(layer);
	if (NULL == fb_cfg)
	{
		return false;
	}

	// 检查队员能否进入
	for (int i = 0; i < team->GetMemberNum() && i < MAX_TEAM_MEMBER_NUM; ++ i)
	{
		Team::TeamMember *member = team->GetTeamMember(i);
		if (NULL == member)
		{
			continue;
		}

		Role *team_role = World::GetInstWorld()->GetSceneManager()->GetRole(member->user_id);
		if (NULL == team_role)
		{	
			continue;
		}

		bool can_create_fb = true;

		if (!team_role->GetRoleStatusManager()->CanEnterFB(true, false))
		{
			can_create_fb = false;
		}

		if (team_role->GetLevel() < fb_cfg->level_limit)
		{
			can_create_fb = false;
		}

		if (!can_create_fb)
		{
			int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_team_equip_fb_notice, team_role->GetUID(),
				team_role->GetName(), (int)team_role->GetCamp());

			if (len > 0)
			{
				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3, role);
			}

			return false;
		}
	}

	return true;
}

void FBManager::AutoFbAddLog(Role * role, int monster_id, ItemID item_id, int num, int scene_id)
{
	if (nullptr == role || item_id <= 0 || num <= 0)
	{
		return;
	}
	const ItemBase *item = ITEMPOOL->GetItem(item_id);
	if (nullptr != item)
	{
		if (item->GetColor() >= ItemBase::I_COLOR_PURPLE)
		{
			DropLogManager::Instance().AddFbLog(role->GetUID(), role->GetName(), monster_id, item_id,num,scene_id,item->GetColor());
		}
	}
}

// void FBManager::HunYanFbResendInvite(Role *role)
// {
// 	std::map<int, HunYanInviteInfo *>::iterator it = m_hunyan_info_map.find(role->GetUID());
// 	if(it != m_hunyan_info_map.end())
// 	{
// 		HunYanInviteInfo *info = it->second;
// 		if(0 != static_cast<int>(info->invite_list.size()))
// 		{
// 			Scene *hunyan_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig().hunyan_scene_id, info->invite_list[0]);
// 			if(hunyan_scene != NULL)
// 			{
// 				SpecialHunyan *special_hunyan = (SpecialHunyan *)hunyan_scene->GetSpecialLogic();
// 				if(special_hunyan != NULL)
// 				{
// 					special_hunyan->SendYanhuiInviteInfo(role);
// 				}
// 				
// 			}
// 		}
// 	}
// }
// 
// void FBManager::GetHunYanFbInviteInfo(Role *role, Protocol::SCQingyuanHunyanInviteInfo &qhii)
// {
// 	if(role != NULL)
// 	{
// 		qhii.invite_count = 0;
// 
// 		std::map<int, HunYanInviteInfo *>::iterator it = m_hunyan_info_map.find(role->GetUID());
// 		if(it != m_hunyan_info_map.end())
// 		{
// 			HunYanInviteInfo *info = it->second;
// 				
// 			int size = static_cast<int>(info->invite_list.size());
// 
// 			for(int i = 0; i < size; i ++)
// 			{
// 				Scene *hunyan_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig().hunyan_scene_id, info->invite_list[i]);
// 				
// 				if(hunyan_scene != NULL)
// 				{
// 					SpecialHunyan *special_hunyan = (SpecialHunyan *)hunyan_scene->GetSpecialLogic();
// 					
// 					if(special_hunyan != NULL)
// 					{
// 						qhii.scq_invite_list[i].yanhui_fb_key = special_hunyan->GetSceneKey();
// 						qhii.scq_invite_list[i].yanhui_type = special_hunyan->GetYanHuiType();
// 						special_hunyan->GetMenName(qhii.scq_invite_list[i].mem_name);
// 						special_hunyan->GetWomenNmae(qhii.scq_invite_list[i].women_name);
// 
// 						++ qhii.invite_count;
// 					}
// 				}
// 			}
// 		}
// 	}
// }