#include "eventhandler.hpp"

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "internalcomm.h"

#include "scene/speciallogic/speciallogic.hpp"

#include "global/guild/guild.hpp"
#include "global/tuodan/tuodanmanager.hpp"
#include "global/guild/guildmanager.hpp"
#include "other/monsterdropsum/monsterdropsum.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "global/bossfamily/bossfamilyconfig.hpp"
#include "other/rolegoal/rolegoalconfig.hpp"
#include "other/famousman/famousmanconfig.hpp"
#include "scene/guildshadow/guildshadow.hpp"
#include "globalconfig/globalconfig.h"
#include "gamelog.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "other/route/mailroute.hpp"
#include "servercommon/string/gameworldstr.h"
#include "global/worldstatus/worldstatus.hpp"
#include "global/challengefield/challengefield.hpp"
#include "global/crosschallengefield/crosschallengefield.hpp"
#include "obj/character/roleshadow.hpp"
#include "global/fishpool/fishpoolmanager.hpp"
#include "global/worldevent/worldeventmanager.hpp"
#include "global/bossfamily/bossfamilymanager.hpp"
#include "other/rolegoal/rolegoal.hpp"
#include "global/rank/topranker.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "global/roleboss/rolebossmanager.h"
#include "global/combineserveractivity/combineserveractivitymanager.hpp"
#include "other/chengjiu/chengjiu.h"
#include "servercommon/chengjiudef.hpp"
#include "item/itempool.h"
#include "gameworld/other/fb/migongxianfumgr.hpp"
#include "scene/speciallogic/worldspecial/jinghuahusongmgr.hpp"
#include "scene/speciallogic/worldspecial/campdefendmanager.hpp"
#include "gameworld/gameworld/other/worldboss/worldbossshadow.hpp"
#include "global/globalrole/lovecontract/lovecontract.hpp"
#include "equipment/equipment.h"
#include "gameworld/npc/transmit/fbmanager.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "other/shenzhouweapon/shenzhouweapon.hpp"
#include "global/worldboss/worldbossbaseconfig.hpp"
#include "task/newtaskmanager.h"
#include "friend/friendmanager.h"
#include "other/vip/vip.hpp"
#include "other/shop/roleshop.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/activedegree/activedegree.hpp"
#include "other/welfare/welfare.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/halo/halomanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/footprint/footprint.hpp"
#include "other/cloak/cloakmanager.hpp"
#include "other/fb/rolemojiefb.hpp"
#include "other/discountbuy/discountbuy.hpp"
#include "other/zerogift/zerogift.hpp"
#include "other/monthcard/monthcard.h"
#include "other/shizhuang/shizhuang.h"
#include "other/cross/rolecross.h"
#include "other/xiannvshouhu/xiannvshouhumanager.hpp"
#include "other/jinglingguanghuan/jinglingguanghuanmanager.hpp"
#include "other/jinglingfazhen/jinglingfazhenmanager.hpp"
#include "other/goldvip/goldvip.hpp"
#include "other/famousman/famousman.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "global/globalrole/fighting/rolefighting.hpp"
#include "equipment/equipmentmanager.h"
#include "other/jinglingadvantage/jinglingadvantage.hpp"
#include "other/shengxingzhuli/shengxingzhulimanager.hpp"
#include "other/fabao/fabaomanager.hpp"
#include "other/jinjietalent/jinjietalent.hpp"
#include "other/talent/talent.hpp"
#include "other/shengqi/shengqi.hpp"
#include "global/rank/couplerankmanager.hpp"
#include "global/bossshanggu/bossshanggumanager.hpp"
#include "other/zhuanzhiequip/zhuanzhiequip.h"
#include "other/seal/seal.hpp"
#include "other/elementheart/elementheart.hpp"
#include "other/baby/baby.hpp"
#include "other/qingyuan/qingyuan.hpp"
#include "world.h"
#include "scene/speciallogic/worldspecial/specialcrossboss.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "other/rolezhuanzhi/rolezhuanzhi.hpp"
#include "other/yaoshi/yaoshimanager.hpp"
#include "other/toushi/toushimanager.hpp"
#include "other/qilinbi/qilinbimanager.hpp"
#include "other/mask/maskmanager.hpp"
#include "other/touzijihua/touzijihua.hpp"
#include "other/upgrade/upgrademanager.hpp"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "other/rolebigsmallgoal/rolebigsmallgoal.hpp"
#include "servercommon//struct/upgradeparam.hpp"
#include "other/xiannv/xiannvmanager.hpp"
#include "other/jingling/jinglingmanager.h"
#include "other/runesystem/runesystem.hpp"
#include "other/shengesystem/shengesystem.hpp"
#include "other/shenshou/shenshou.hpp"
#include "other/shenyin/shenyin.hpp"
#include "global/globalrole/globalrolemanager.hpp"
#include "global/globalrole/commondata/commondataglobal.hpp"
#include "gameworld/equipment/stonemananger.hpp"
#include "other/molong/molong.h"
#include "other/orangeequipcollect/orangeequipcollect.hpp"
#include "task/other/tumotask.hpp"
#include "other/equipbaptize/equipbaptize.hpp"
#include "scene/speciallogic/worldspecial/specialcrossmizangboss.hpp"
#include "scene/speciallogic/worldspecial/specialcrossyoumingboss.hpp"
#include "other/greatesoldier/greatesoldier.hpp"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include "gameworld/gameworld/chat/chatmanager.h"
#include "servercommon/guilddef.hpp"
#include "global/guild/roleguild.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "other/rolecrossrandactivity/rolecrossrandactivity.hpp"
#include "scene/speciallogic/worldspecial/specialgodmagicboss.hpp"
#include "other/crossgoal/crossgoal.hpp"
#include "other/crossgoal/crossgoalconfig.hpp"
#include "other/xianzunka/xianzunka.hpp"
#include "servercommon/rankdef.hpp"
#include "servercommon/serverconfig/localconfig.hpp"
#include "global/usercache/usercache.hpp"
#include "other/tianshuxunzhu/tianshuxunzhu.hpp"
#include "other/tianshuxunzhu/tianshuxunzhuconfig.hpp"

EventHandler::EventHandler()
{

}

EventHandler::~EventHandler()
{

}

EventHandler & EventHandler::Instance()
{
	static EventHandler event_handler;
	return event_handler;
}

void EventHandler::OnRoleHurtMonster(Role *role, Monster *monster, Attribute &hurt)   // hurt < 0 是负数来的
{
	if (NULL == role || NULL == monster || 0 == hurt) 
	{
		return;
	}

	// 世界boss
	if (monster->IsWorldBossMonster())
	{
		role->GetRoleActivity()->OnAttackWorldBoss();
		WorldBossShadow::Instance().OnAttack(role, monster, hurt);
	}

	// 福利boss
	//const WorldBossInfoCfg * world_boss_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossInfo(monster->GetMonsterId());
	//if (NULL != world_boss_cfg)
	//{
	//	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_FULI_BOSS);
	//}
	
	//const BossFamilyMikuBossInfoCfg * miku_boss_info_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetMikuBossInfo(monster->GetMonsterId());
	//if (NULL != miku_boss_info_cfg)
	//{
	//	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_WORLD_BOSS);
	//}

	if (Scene::IsBossDongKuScene(monster->GetScene()->GetSceneID()) || Scene::IsDabaoBossScene(monster->GetScene()->GetSceneID()) || Scene::IsActiveBossScene(monster->GetScene()->GetSceneID()) || Scene::IsBossFamilyScene(monster->GetScene()->GetSceneID()))
	{
		BossFamilyManager::Instance().OnAttack(role, monster, hurt);
	}

	if (monster->GetScene()->GetSceneType() == Scene::SCENE_TYPE_SHANGGU_BOSS)
	{
		BossShangGuManager::Instance().OnAttack(role, monster, hurt);
	}

	if (Scene::SCENE_TYPE_CROSS_BOSS == monster->GetScene()->GetSceneType())
	{
		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(monster->GetScene()->GetSceneID(), CROSS_PUBLIC_SCENE_KEY);
		if (nullptr != scene)
		{
			SpecialCrossBoss *cross_boss = dynamic_cast<SpecialCrossBoss*>(scene->GetSpecialLogic());
			if (nullptr != cross_boss)
			{
				cross_boss->OnAttackCalHurt(role, monster, hurt);
			}
		}
	}

	if (Scene::SCENE_TYPE_CROSS_MIZANG_BOSS == monster->GetScene()->GetSceneType())
	{
		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(monster->GetScene()->GetSceneID(), CROSS_PUBLIC_SCENE_KEY);
		if (nullptr != scene)
		{
			SpecialCrossMiZangBoss *cross_boss = dynamic_cast<SpecialCrossMiZangBoss*>(scene->GetSpecialLogic());
			if (nullptr != cross_boss)
			{
				cross_boss->OnAttackCalHurt(role, monster, hurt);
			}
		}
	}

	if (Scene::SCENE_TYPE_CROSS_YOUMING_BOSS == monster->GetScene()->GetSceneType())
	{
		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(monster->GetScene()->GetSceneID(), CROSS_PUBLIC_SCENE_KEY);
		if (nullptr != scene)
		{
			SpecialCrossYouMingBoss *cross_boss = dynamic_cast<SpecialCrossYouMingBoss*>(scene->GetSpecialLogic());
			if (nullptr != cross_boss)
			{
				cross_boss->OnAttackCalHurt(role, monster, hurt);
			}
		}
	}

	if (Scene::SCENE_TYPE_GODMAGIC_BOSS == monster->GetScene()->GetSceneType())
	{
		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(monster->GetScene()->GetSceneID(), 0);
		if (nullptr != scene)
		{
			SpecialGodmagicBoss *godmagic_boss = dynamic_cast<SpecialGodmagicBoss*>(scene->GetSpecialLogic());
			if (nullptr != godmagic_boss)
			{
				godmagic_boss->OnAttackCalHurt(role, monster, hurt);
			}
		}
	}

	if (ActivityShadow::Instance().GetMoShenJiangLinBossID() == monster->GetMonsterId())
	{
		ActivityShadow::Instance().OnRoleHurtMonster(role, monster, hurt);
	}
}

void EventHandler::OnMonsterDisapper(Monster *monster)
{
	if (NULL == monster) 
	{
		return;
	}

	WorldEventManager::Instance().OnMonsterDisapper(monster);
}

void EventHandler::OnKillerRoleKillMonster(Role *role, Monster *monster)
{
	if (NULL == role || NULL == monster) 
	{
		return;
	}

	WorldEventManager::Instance().OnKillMonster(role, monster);
	ActivityShadow::Instance().OnKillMonster(role, monster);

	if (monster->IsBossMonster())
	{
		role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_KILLBOSS);
		role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_KILL_BOSS);
		role->GetRoleActivity()->OnRAExtremeChallengeAddPlan(RA_EXTREME_CHALLENGE_KILL_BOSS, 1);
		role->GetRoleActivity()->OnCombineServerKillBossActivity(monster->GetMonsterId(), monster->GetLevel());

		if (monster->GetScene()->GetSceneType() == Scene::FB_TYPE::SCENE_TYPE_BABY_BOSS)
		{
			role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_BB_BOSS, 1);
		}

		int scene_id = monster->GetScene()->GetSceneID();
		if (Scene::IsBossFamilyScene(scene_id) || Scene::IsBossDongKuScene(scene_id) || Scene::IsWorldBossScene(scene_id) || Scene::IsActiveBossScene(scene_id))
		{
			role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_KILL_BOSS, 1);
		}
	}
	else
	{
		role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_KILL_MONSTER);
		role->GetRoleActivity()->OnRAExtremeChallengeAddPlan(RA_EXTREME_CHALLENGE_KILL_MONSTER, 1);
	}
}

void EventHandler::OnKillerTeamKillMonster(Role *role, Monster *monster)
{
	if (NULL == role || NULL == monster) 
	{
		return;
	}

	if (monster->IsBossMonster())
	{
		role->GetRoleActivity()->OnKillBoss(monster->GetMonsterId());
	}
}

void EventHandler::OnKillMonster(Role *role, int scene_id, const Posi &pos, int monster_id, int monster_level)
{
	if (NULL == role) return;
	
	role->GetTaskManager()->OnKillMonster(scene_id, pos, monster_id, monster_level);
	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_KILL_BOSS_TYPE);
	role->GetFBMojie()->OnKillMonster(monster_id);
	role->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_KILL_BOSS, 1, monster_id);
	role->GetRoleGoal()->CheckCond(COND_TYPE_KILL_BOSS, 1, monster_id);

	Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
	if (NULL != guild)
	{
		guild->OnMemberKillMonter(role, monster_id);
	}
}

void EventHandler::OnStartGather(Role *role, int gather_id, ObjID obj_id, const Posi &gather_pos)
{
	if (NULL == role) return;

	role->GetShenzhouWeapon()->OnStartGather(gather_id);

	if (NULL != role->GetScene()) role->GetScene()->GetSpecialLogic()->OnStartGather(role, gather_id, obj_id, gather_pos);
}

void EventHandler::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &gather_pos)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnGather(gather_id);
	role->GetShenzhouWeapon()->OnGather(gather_id);
	role->GetQingyuan()->OnGatherXunyouHongbao(gather_id);
	JingHuaHuSongMgr::Instance().OnGather(role, gather_id, obj_id, gather_pos);
	JinglingAdvantage::Instance().OnGather(role, gather_id, gather_pos);
	BossFamilyManager::Instance().OnPreciousBossGather(role, gather_id, gather_pos);
	ActivityShadow::Instance().OnGather(role, gather_id, obj_id);
	role->GetRoleActivity()->OnRAPlantingTreeWatering(gather_id, obj_id);

	if (NULL != role->GetScene()) role->GetScene()->GetSpecialLogic()->OnGather(role, gather_id, obj_id, gather_pos);
}

void EventHandler::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{
	if (NULL == role || NULL == falling_item)
	{
		return;
	}

	if (NULL != role->GetScene() && NULL != role->GetScene()->GetSpecialLogic())
	{
		role->GetScene()->GetSpecialLogic()->OnPickFallingItem(role, falling_item, item_wrapper);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EventHandler::OnCompleteTask(Role *role, TaskID task_id)
{
	if (NULL == role) return;

	role->GetMountManager()->OnCompleteTask(task_id);
	role->GetFightMountManager()->OnCompleteTask(task_id);
	role->GetWing()->OnCompleteTask(task_id);
	role->GetHalo()->OnCompleteTask(task_id);
	role->GetShengong()->OnCompleteTask(task_id);
	role->GetShenyi()->OnCompleteTask(task_id);
	role->GetFootPrint()->OnCompleteTask(task_id);
	role->GetCloakManager()->OnCompleteTask(task_id);
	role->GetXiannvShouhu()->OnCompleteTask(task_id);
	role->GetJinglingGuanghuan()->OnCompleteTask(task_id);
	role->GetJinglingFazhen()->OnCompleteTask(task_id);
	role->GetYaoShiManager()->OnCompleteTask(task_id);
	role->GetTouShiManager()->OnCompleteTask(task_id);
	role->GetQiLinBiManager()->OnCompleteTask(task_id);
	role->GetMaskManager()->OnCompleteTask(task_id);
	role->GetFabaoManager()->OnCompleteTask(task_id);
	role->GetShizhuang()->OnCompleteTask(task_id);
	role->GetUpgradeManager()->OnCompleteTask(task_id);
	role->GetXiannvManager()->OnCompleteTask(task_id);
	role->GetJingLingMgr()->OnCompleteTask(task_id);
	role->GetRuneSystem()->OnCompleteTask(task_id);
	role->GetShenzhouWeapon()->OnCompleteTask(task_id);
	role->GetShengeSystem()->OnCompleteTask(task_id);
	role->GetShenshou()->OnCompleteTask(task_id);
	role->GetShenYin()->OnCompleteTask(task_id);
	role->GetGreateSoldier()->OnCompleteTask(task_id);
	role->GetShengqi()->OnCompleteTask(task_id);
	role->GetChineseZodiac()->OnCompleteTask(task_id);
}

void EventHandler::OnAddFriend(Role *role)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_OWN_FRIEND);
	
	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_ADD_FRIEND);
}

void EventHandler::OnResetName(Role *role, GameName name)
{
	if (NULL == role || nullptr == name) return;

	UserID lover_user_id = IntToUid(role->GetLoverUid());
	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(lover_user_id);
	if (nullptr != lover_role)
	{
		lover_role->GetBaby()->SendAllBabyInfo();
	}
}

void EventHandler::OnJoinActivity(Role *role, int activity_type)
{
	if (NULL == role) return;

	int activity_find_type = -1;
	switch (activity_type)
	{
		case ACTIVITY_TYPE_GUILD_QUESTION:
		{
			activity_find_type = ACTIVITY_FIND_TYPE_BANGPAIDATI;
		}
		break;

		case ACTIVITY_TYPE_NIGHT_FIGHT_FB:
		{
			activity_find_type = ACTIVITY_FIND_TYPE_NIGHT_FIGHT_FB ;		// 夜战王城	23
		}
		break;

		case ACTIVITY_TYPE_MESS_BATTLE_FB:
		{
			activity_find_type = ACTIVITY_FIND_TYPE_MESS_BATTLE_FB ;			// 乱斗战场	31
		}
		break;

		case CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB:
		{
			activity_find_type = ACTIVITY_FIND_TYPE_CROSS_NIGHT_FIGHT_FB;		// 跨服夜战王城 3085
		}
		break;

		case CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB:
		{
			activity_find_type = ACTIVITY_FIND_TYPE_CROSS_MESS_BATTLE_FB;// 跨服乱斗战场	3086
		}
		break;

		default:
		break;
	}

	if (-1 != activity_find_type)
	{
		role->GetWelfare()->OnJoinActivity(activity_find_type);
	}
}

void EventHandler::OnChangeCapability(Role *role, int cap_type)
{
	if (NULL == role) return;
	
	role->OnChangeCapability(cap_type);
	role->GetRoleActivity()->OnChangeCapability(cap_type);
}

void EventHandler::OnRoleUpgradeLevel(Role *role, int old_level, int level)
{
	if (NULL == role) return;

	role->GetFriendManager()->OnRoleUpgradeLevel();
	role->GetRoleActivity()->OnRoleUpgradeLevel(old_level, level);
	role->GetRoleShop()->OnRoleLevelUp(old_level, level);
	role->GetTaskManager()->OnUpLevel(old_level, level);
	role->GetDiscountBuy()->OnRoleLevelChange(level);
//	role->GetWing()->OnRoleLevelChange();
	ChallengeField::Instance().OnUserLevelChange(role);
	CrossChallengeField::Instance().OnUserLevelChange(role);
	FishPoolManager::Instance().OnUserLevelChange(role);

	role->GetMountManager()->OnRoleUpLevel(level);
	role->GetWing()->OnRoleUpLevel(level);
	role->GetHalo()->OnRoleUpLevel(level);
	role->GetShengong()->OnRoleUpLevel(level);
	role->GetShenyi()->OnRoleUpLevel(level);
	role->GetFootPrint()->OnRoleUpLevel(level);
	role->GetCloakManager()->OnRoleUpLevel(level);
	role->GetXiannvShouhu()->OnRoleUpLevel(level);
	role->GetJinglingGuanghuan()->OnRoleUpLevel(level);
	role->GetJinglingFazhen()->OnRoleUpLevel(level);
	role->GetFightMountManager()->OnRoleUpLevel(level);
	role->GetZeroGift()->OnRoleUpLevel(old_level, level);
	role->GetFabaoManager()->OnRoleUpLevel(level);
	role->GetShizhuang()->OnRoleUpLevel(level);
	role->GetYaoShiManager()->OnRoleUpLevel(level);
	role->GetTouShiManager()->OnRoleUpLevel(level);
	role->GetQiLinBiManager()->OnRoleUpLevel(level);
	role->GetMaskManager()->OnRoleUpLevel(level);

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_ROLE_LEVEL, level);
	role->GetRoleGoal()->CheckCond(COND_TYPE_ROLE_LEVEL, level, 0);
	role->GetFamousMan()->OnCheckFamousMan(FAMOUS_MAN_ROLE_LEVEL, level);
	role->GetTalent()->OnUpLevel(old_level, level);
	role->GetShengqi()->CheckActivateShengqiSystem();
	role->GetSeal()->CheckActivateSealSystem();
	role->GetUpgradeManager()->OnRoleUpLevel(level);
	role->GetOrangeEquipCollect()->OnCheckActive(level);

	role->GetEquipBaptize()->CheckOpenPartAndSlot();

	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_CON_TO_PROF6);

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_ROLE_LEVEL);//嘉年华

	GuildID guild_id = role->GetGuildID();
	if (INVALID_GUILD_ID != guild_id)
	{
		//if (level >= 150)					//角色等级150级后每20级江湖传闻一次
		//{
		//	if (level == 150)
		//	{
		//		this->OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_UPLEVEL, level);

		//		role->GetRoleGuild()->GetRoleGuildParam()->next_notice_level = level + 20;
		//	}
		//	else if (level == role->GetRoleGuild()->GetRoleGuildParam()->next_notice_level)
		//	{
		//		this->OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_UPLEVEL, level);

		//		role->GetRoleGuild()->GetRoleGuildParam()->next_notice_level = level + 20;
		//	}
		//}

		auto guild_rumor_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildRumorCfg(GUILD_EVENT_TYPE_RUMOR_UPLEVEL);
		if (guild_rumor_cfg != NULL)
		{
			//if (level == 160)			//策划需求特殊处理。。
			//{
			//	this->OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_UPLEVEL, level);
			//}
			if (level >= guild_rumor_cfg->param_1)			//可配置
			{
				if (guild_rumor_cfg->param_2 > 0)
				{
					if (level % guild_rumor_cfg->param_2 == guild_rumor_cfg->param_3)
					{
						this->OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_UPLEVEL, level);
					}
				}
			}
		}
	}

	RoleFighting *fighting = GLOBAL_ROLE_MGR.GetRoleFighting(role->GetUID());
	if (NULL != fighting)
	{
		fighting->OnRoleUpLevel(role, level);
	}
}

void EventHandler::OnRoleRealiveSucc(Role *role, int realive_type)
{
	if (nullptr == role)
	{
		return;
	}

	if (Scene::SCENE_TYPE_CROSS_DARK_NIGHT == role->GetScene()->GetSceneType())
	{
		// 复活三秒无敌
		EffectSpecialState *wudi_effect = new EffectSpecialState(role->GetId(), 1, 3000, EBT_WUDI, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		wudi_effect->SetClientEffectType(EffectBase::ECT_DARK_NIGHT_WUDI);
		role->AddEffect(wudi_effect);
	}
}

bool EventHandler::OnRMBBuy(Role* role, int chongzhi_num)
{
	if (role->GetTouzijihua()->OnBuyFoundation(chongzhi_num))
	{
		return true;
	}

	if (role->GetRoleActivity()->OnCSATouzijihuaBuy(chongzhi_num))
	{
		return true;
	}

	if (role->GetRoleActivity()->OnCSAFoundationBuy(chongzhi_num))
	{
		return true;
	}

	if (role->GetRoleCrossRandActivity()->OnCloudPurchaseRMBBuyTicket(chongzhi_num))
	{
		return true;
	}

	return false;
}

void EventHandler::OnAddChongzhi(Role *role, long long chongzhi_gold, bool is_notice_mail)
{
	if (NULL == role) return;

	{
		// 邮件通知 
		if (is_notice_mail)
		{
			static MailContentParam contentparam; contentparam.Reset();
			int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_money_chongzhi_subject);
			int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_money_chongzhi_content, static_cast<int>(chongzhi_gold));
			if (length1 > 0 && length2 > 0)
			{
				MailRoute::MailToUser(role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam, MAIL_TYPE_CHONGZHI);
			}
		}
	}

	if (SHAREDCONFIG->GetOpenServerConfig().IsIosExamine())
	{
		gamelog::g_log_roleactivity.printf(LL_INFO, "[EventHandler::OnAddChongzhi fail][user[%d %s] gold_num:%lld",
			role->GetUID(), role->GetName(), chongzhi_gold);
		return;
	}

	role->GetVip()->AddVipExp(static_cast<int>(chongzhi_gold));
	role->GetWelfare()->OnAddChongZhi(chongzhi_gold);
	role->GetRoleActivity()->OnAddChongZhi(chongzhi_gold);
	role->GetGoldVip()->CheckAddScore(static_cast<int>(chongzhi_gold));
	role->GetEquipmentManager()->NotifyStrengLevel(role);
	role->GetShengxingzhuliManager()->OnRoleChongzhi(static_cast<int>(chongzhi_gold));
	role->GetRoleCrossRandActivity()->OnAddChongZhi(chongzhi_gold);
	role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_RECHARGE, static_cast<int>(chongzhi_gold));
	role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_TARGET_RECHARGE, static_cast<int>(chongzhi_gold));
	role->GetXianZunKa()->OnAddChongzhi(chongzhi_gold);
	gamelog::g_log_roleactivity.printf(LL_INFO, "[EventHandler::OnAddChongzhi Succ][user[%d %s] gold_num:%lld",
		role->GetUID(), role->GetName(), chongzhi_gold);
}

void EventHandler::OnAddGoldBind(Role *role, long long gold_bind)
{
	if (NULL == role) return;
	if (gold_bind > 0)
	{
		role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_MONEY_GOLD_BIND, (unsigned int)gold_bind);
	}
}

void EventHandler::OnConsumeGold(Role *role, long long consume_gold)
{
	if (NULL == role) return;

	role->GetRoleActivity()->OnConsumeGold(consume_gold);
	role->GetMolong()->OnConsumeGold(static_cast<int>(consume_gold));
	
	role->GetRoleCrossRandActivity()->OnConsumeGold(consume_gold);

	if (role->IsInCross())
	{
		role->GetRoleCross()->OnCrossConsumeGold(int(consume_gold));
	}
}

void EventHandler::OnCheckCrossConsumeGold(Role *role, long long total_consume_gold, long long daily_consume_gold)
{
	if (nullptr == role) return;

	if (total_consume_gold > 0)
	{
		role->GetRoleActivity()->OnCheckCrossConsumeGold(total_consume_gold);
	}

	if (daily_consume_gold > 0)
	{
		role->GetRoleActivity()->OnCheckCrossDailyConsumeGold(daily_consume_gold);
	}

	// 跨服活动
	{
		role->GetRoleCrossRandActivity()->OnConsumeGold(total_consume_gold);
	}
}

void EventHandler::OnShopBuyItem(Role *role, int buy_count, const ItemExtern::BuyItemInfo buy_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM], long long consume_gold, long long consume_bind_gold)
{
	if (NULL == role || buy_count <= 0) return;

	for (int i = 0; i < buy_count && i < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM; ++ i)
	{
		long long buy_item_consume_gold = 0;
		long long buy_item_consume_bind_gold = 0;

		if (consume_bind_gold >= buy_list[i].consume_gold)
		{
			buy_item_consume_bind_gold = buy_list[i].consume_gold;
		}
		else
		{
			buy_item_consume_bind_gold = consume_bind_gold;
			buy_item_consume_gold = buy_list[i].consume_gold - buy_item_consume_bind_gold;
		}

		consume_bind_gold -= buy_item_consume_bind_gold;
		consume_gold -= buy_item_consume_gold;

		if (buy_item_consume_gold > 0) 
		{
			ROLE_LOG_QUICK10(LOG_TYPE_GET_ITEM, role, buy_list[i].item_id, LOG_GET_ITEM_WAY_SHOP, NULL, NULL, LOG_MONEY_TYPE_GOLD, buy_item_consume_gold, role->GetLevel(), role->GetRoleActivity()->GetHistoryChongZhi());
		}

		if (buy_item_consume_bind_gold > 0)
		{
			ROLE_LOG_QUICK10(LOG_TYPE_GET_ITEM, role, buy_list[i].item_id, LOG_GET_ITEM_WAY_SHOP, NULL, NULL, LOG_MONEY_TYPE_GOLD_BIND, buy_item_consume_bind_gold, role->GetLevel(), role->GetRoleActivity()->GetHistoryChongZhi());
		}

		gamelog::g_log_shop.printf(LL_INFO,"[Shop::Buy Succ][user[%d %s] item_id:%d num:%d consume_type:%d total_price:%d]", 
			role->GetUID(), role->GetName(), buy_list[i].item_id, buy_list[i].item_count, 2, buy_list[i].consume_gold); // 2主要是后台需要，以前预留，代表是元宝购买的意思，之前的项目有区分是用绑定或者非绑定元宝
	}
}

void EventHandler::OnGiveFlower(Role *role, int flower_num)
{
	if (NULL == role) return;
	{
		// 随机活动
		role->GetRoleActivity()->OnGiveFlower(flower_num);
	}
}

void EventHandler::OnBeGiveFlower(Role *role)
{
	if (NULL == role) return;
}

void EventHandler::OnVipLevelChange(Role *role)
{
	if (NULL == role) return;

	role->OnRoleBaseInfoChange();

	WorldStatus::Instance().OnUpgradeVipLevel(role->GetVip()->GetVipLevel());
	role->GetRoleActivity()->SendOpenGameActivityInfo();

	role->GetZhuanzhiEquip()->CheckAllPartChangeSlot(ZHUANZHI_STONE_SLOT_OPEN_TYPE_VIP);

//	role->GetKnapsack()->VipLevelExtendValidNum(role);					// 屏蔽vip等级拓展背包格子

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_VIPBUFF);
}

void EventHandler::OnMountUpgrade(Role *role, int old_grade, int to_grade)
{
	if (NULL == role) return;

	role->GetRoleActivity()->OnMountUpgrade(old_grade, to_grade);
	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_MOUNT_UPGRADE);

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_MOUNT_JIE, to_grade);

	if (to_grade > old_grade && to_grade > 0)
	{
		role->GetRoleActivity()->SendOpenGameActivityInfo();
		role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_MOUNT_GRADE);
		role->GetRoleGoal()->CheckCond(COND_TYPE_UPGRADE_MOUNT, 0, 0);
		role->GetFamousMan()->OnCheckFamousMan(FAMOUS_MAN_MOUNT_GRADE, to_grade);
		//role->GetUpgradeRush()->OnUpgradeLevelChange(UPGRADE_RUSH_TYPE_MOUNT, to_grade);
		role->GetJinJieTalent()->OnCheckOpenTalentGrid(JINJIE_TALENT_MOUNT, to_grade);
		JinjiesysFuncHelper::Instance().OnUpgrade(role, JINJIE_TYPE_MOUNT, old_grade, to_grade);
		
		role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_MOUNT_GRADE);//嘉年华

		role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_MOUNT, to_grade,old_grade);   // 进阶丹返还活动
		WorldStatus::Instance().IncTotalUpgradeMountRecordList(to_grade);
	}
}

void EventHandler::OnFightMountUpgrade(Role *role, int old_grade, int to_grade)
{
	if (NULL == role || old_grade >= to_grade) return;

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_MO_QI_LEVEL, to_grade);
	role->GetRoleGoal()->CheckCond(COND_TYPE_UPGRADE_FIGHT_MOUNT, 0, 0);
	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_FIGHT_MOUNT_GRADE);
	//role->GetUpgradeRush()->OnUpgradeLevelChange(UPGRADE_RUSH_TYPE_FIGHTMOUNT, to_grade);
	role->GetJinJieTalent()->OnCheckOpenTalentGrid(JINJIE_TALENT_FIGHTMOUNT, to_grade);
	WorldStatus2::Instance().IncTotalUpgradeFightMountRecordList(to_grade);
	JinjiesysFuncHelper::Instance().OnUpgrade(role, JINJIE_TYPE_FIGHT_MOUNT, old_grade, to_grade);

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_FIGHT_MOUNT_GRADE);//嘉年华
	role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_FIGHT_MOUNT, to_grade,old_grade);   // 进阶丹返还活动
}

void EventHandler::OnQibingUpgrade(Role *role, int old_grade, int to_grade)
{
	if (NULL == role) return;
}

void EventHandler::OnCloakUpLevel(Role * role, int old_grade, int to_grade)
{
	if (NULL == role || old_grade >= to_grade) return;
	JinjiesysFuncHelper::Instance().OnUpgrade(role, JINJIE_TYPE_CLOAK, old_grade, to_grade);
}

void EventHandler::OnMountUpStar(Role *role, int old_star, int to_star)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_MOUNT_UP_STAR);

	if (to_star > old_star)
	{
		role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_MOUNT_STAR);
	}
}

void EventHandler::OnWingUpStar(Role *role, int old_star, int to_star)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_WING_UP_STAR);

	if (to_star > old_star)
	{
		role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_WING_STAR);
	}
}

void EventHandler::OnHaloUpStar(Role *role, int old_star, int to_star)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_HALO_UP_STAR);

	if (to_star > old_star)
	{
		role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_HALO_STAR);
	}
}

void EventHandler::OnShengongUpStar(Role *role, int old_star, int to_star)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_SHENGONG_UP_STAR);

	if (to_star > old_star)
	{
		role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_SHENGONG_STAR);
	}
}

void EventHandler::OnShenyiUpStar(Role *role, int old_star, int to_star)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_SHENYI_UP_STAR);

	if(to_star > old_star)
	{
		role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_SHENYI_STAR);
	}
}

void EventHandler::OnFightMountUpStar(Role *role, int old_star, int to_star)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_FIGHT_MOUNT_UP_STAR);

	if(to_star > old_star)
	{
		role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_FIGHT_MOUNT_STAR);
	}
}

void EventHandler::OnFootprintUpStar(Role *role, int old_star, int to_star)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_FOOTPRINT_UP_STAR);

	if (to_star > old_star)
	{
		role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_FOOTPRINT_STAR);
	}
}

void EventHandler::OnPutonEquip(Role *role, short equip_type, short color, short quality, short level, short xian_pin_count)
{
	if (NULL == role) return;

	role->GetShizhuang()->CheckActiveByCondition(SHIZHUANG_CHECK_ACTIVE_TYPE_EQUIP);
	role->GetRoleActivity()->OnPutonEquip();

	if (ItemBase::I_COLOR_ORANGE == color || ItemBase::I_COLOR_RED == color)
	{
		role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_EQU_1_STAR_AND_COLOR, xian_pin_count);
		role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_EQU_2_STAR_AND_COLOR, xian_pin_count);
	}
	if (ItemBase::I_COLOR_RED == color)
	{
		role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_EQU_3_STAR_AND_COLOR, xian_pin_count);
		//role->GetFamousMan()->OnCheckFamousMan(FAMOUS_MAN_EQUIP_GRADE, color);
	}
	if (Equipment::E_TYPE_JIEZHI == equip_type)
	{
		role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_EQU_JIEZHI);
	}
	
	role->GetFBMojie()->OnPutonEquip(equip_type, quality, color);

	role->GetRoleGoal()->CheckCond(PERSONAL_GOAL_COND_INVALID_TYPE, 0, 0);

	// 宝石开启槽刷新
	role->GetStoneManager()->SendToClient(Protocol::SCStoneInfo::STONE_REASON_DEFAULT);
}

void EventHandler::OnTakeOffEquip(Role *role)
{
	if (NULL == role) return;

	role->GetShizhuang()->CheckActiveByCondition(SHIZHUANG_CHECK_ACTIVE_TYPE_EQUIP);
}

void EventHandler::OnPutOnZhuanZhiEquip(Role *role, short equip_type, short color, short level)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_PUT_ON_ZHUANZHI_EQUIP);
	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_PUT_ON_ZHUANZHI_EQUIP);

	role->GetElementHeart()->ActiveElementTexture(equip_type);

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_WEAR_ZHUANZHI_EQUIPMENT); // 嘉年华

	if (ItemBase::I_COLOR_RED == color)
	{
		role->GetFamousMan()->OnCheckFamousMan(FAMOUS_MAN_EQUIP_GRADE, color);
	}
}

void EventHandler::OnForgeZhuanZhiSuit(Role *role)
{
	if (NULL == role) return;

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_ACTIVE_ZHUANZHI_SUIT_FANG); //嘉年华
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_ACTIVE_ZHUANZHI_SUIT);		 //嘉年华
}

void EventHandler::OnPutOnBaizhanEquip(Role *role, short equip_type, short color, short level)
{
	if (NULL == role) return;
}

void EventHandler::OnStrengthenConsumeCoin(Role *role, int consume_coin)
{
	if (NULL == role) return;

}

void EventHandler::OnKillWorldBoss(Role *role)
{
	if (NULL == role) return;

	role->OnKillWorldBoss();
	role->GetRoleActivity()->OnKillWorldBoss(role);

	{
		Guild *guild = GuildManager::Instance().GetGuild(role->GetUserId());
		if (NULL != guild)
		{
			guild->OnMemberKillWorldBoss();
		}
	}
}

void EventHandler::OnKillMiZangBoss(Role *role)
{
	if (NULL == role) return;
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_MIZANG_BOSS);
}

void EventHandler::OnKillCrossBoss(Role * role)
{
	if (NULL == role) return;
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_CROSS_BOSS);
	role->GetCrossGoal()->ChangeCrossGoalParam(CROSS_GOAL_COND_KILL_CROSS_BOSS, 1);
}

void EventHandler::OnKillRoleOnCrossBossScene(Role *role)
{
	if (NULL == role) return;
	role->GetCrossGoal()->ChangeCrossGoalParam(CROSS_GOAL_COND_CROSS_BOSS_ROLE_KILLER, 1);
}

void EventHandler::OnAddShengwang(Role *role, int add_shengwang)
{
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_TOTAL_SHENGWANG);		 //嘉年华
}

void EventHandler::OnChangeCamp(Role *role, int camp)
{
}

void EventHandler::OnItemCompose(Role *role, int product_id, int num)
{
	if (NULL == role) return;

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_COMPOSE);
}

void EventHandler::OnPetOpenEgg(Role *role)
{
}

void EventHandler::OnEquipShenzhu(Role *role)
{
	if (NULL == role) return;
}

void EventHandler::OnReCalcAttr(Role *role, bool is_init)
{
	if (NULL == role) return;

	role->GetFBMojie()->OnChangeFangyu();
	
	if (!is_init)
	{
		role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_XIANNV_SHENWU_CAP); // 嘉年华
	}
}

void EventHandler::OnStrengthenEquipOperate(Role *role, int equip_index, int old_level, int to_level)
{
	if (NULL == role) return;

	role->GetRoleActivity()->OnStrengthenEquip(equip_index, old_level, to_level);

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_STRENGTH);

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_EQUIP_STRENGTH);
	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_PUTON_EQUIP_STRENGTH, to_level);

	role->GetRoleGoal()->CheckCond(COND_TYPE_TOTAL_STRENGTHEN, 0, 0);
	role->GetRoleGoal()->CheckCond(COND_TYPE_STRENGTHEN, 0, 0);

	//装备强化大小目标检查
	role->GetEquipmentManager()->CheckFuncOpen();
	role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_EQUIP_STRENGTHEN, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_2, role->GetEquipmentManager()->GetTotalStrengthen());
}

//void EventHandler::OnUpLevelEquipOperate(Role* role, int equip_index, int old_level, int to_level)
//{
//	if (NULL == role) return;
//
//}

void EventHandler::OnUpLevelEquipOperate(Role *role, bool is_puton, short type, short color, short quality, short level)
{
	if (NULL == role) return;

	// 宝石开启槽刷新
	role->GetStoneManager()->SendToClient(Protocol::SCStoneInfo::STONE_REASON_DEFAULT);

	role->GetRoleActivity()->OnEquipUpLevel();
	//role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_EQUIP_LEVEL);
}

void EventHandler::OnUpStarEquipOperate(Role *role, int equip_index, int old_level, int to_level)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_UPSTAR);

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_EQUIP_UPSTAR);
	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_PUTON_EQUIP_STAR, to_level);

	role->GetRoleGoal()->CheckCond(COND_TYPE_TOTAL_UPSTAR, 0, 0);
	role->GetRoleGoal()->CheckCond(COND_TYPE_UPSTAR, 0, 0);
}

void EventHandler::OnEquipUpQuality(Role *role, int equip_index, int old_level, int to_level)
{
	if (NULL == role || old_level >= to_level) return;
}

void EventHandler::OnInheritEquipOperate(Role *role, int equip_index1, int equip_index2)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_INHERIT);
}

void EventHandler::OnFuLingEquipOperate(Role *role, int equip_index1)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_FULIN);
}

void EventHandler::OnEquipBaptize(Role *role)
{
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_EQUIP_BAPTIZE); // 嘉年华
}

void EventHandler::OnQunXianLuanDouKill(Role *role)
{
	if (NULL == role) return;
	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_YSZC_KILL_ROLE);
}

void EventHandler::OnQunXianLuanDouAssist(Role *role)
{
	if (NULL == role) return;
}

void EventHandler::OnChangeProf(Role *role, int prof)
{
}

void EventHandler::OnZhuanzhi(Role *role)
{
	if (NULL == role) return;

	role->GetRoleZhuanZhi()->GetRoleZhuanZhiSkill().RecalcActivedSkill();
	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_ONE_ZHUAN);
	role->GetTalent()->OnZhuanzhi();

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_PROF_LEVEL); //嘉年华

	GuildID guild_id = role->GetGuildID();
	if (INVALID_GUILD_ID != guild_id)
	{
		auto guild_rumor_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildRumorCfg(GUILD_EVENT_TYPE_RUMOR_ZHUANZHI);
		if (guild_rumor_cfg != NULL)
		{
			if (role->GetProfLevel() >= guild_rumor_cfg->param_1)				
			{
				if (guild_rumor_cfg->param_2 > 0)
				{
					if (role->GetProfLevel() % guild_rumor_cfg->param_2 == guild_rumor_cfg->param_3)
					{
						this->OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_ZHUANZHI, role->GetProfLevel());
					}
				}
			}
		}
	}
}

void EventHandler::OnAcceptHusongTask(Role *role)
{
	if (NULL == role) return;

	role->GetRoleCross()->OnCrossTeamFBOption(CROSS_TEAM_FB_STATE_OPTION_QUIT, 0, 0, 0);
	role->GetRoleActivity()->OnAcceptHusongTask();
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_HUSONG);

	gamelog::g_log_function_stats.printf(LL_INFO, "[Join][user[%s %d] type:HuSongTask times:1]", role->GetName(), UidToInt(role->GetUserId()));

	ROLE_LOG_QUICK10(LOG_TYPE_FUNCTION_STATUS, role, LOG_TYPE_FUNCTION_STATUS_TYPE_ACCEPT_HUSONG, 1, NULL, NULL, 
		role->GetLevel(), role->GetRoleActivity()->GetHistoryChongZhi(), 0, 0);
}

void EventHandler::OnAcceptDailyTask(Role *role)
{
	if (NULL == role) return;

	gamelog::g_log_function_stats.printf(LL_INFO, "[Join][user[%s %d] type:DailyTask times:1]", role->GetName(), UidToInt(role->GetUserId()));

	ROLE_LOG_QUICK10(LOG_TYPE_FUNCTION_STATUS, role, LOG_TYPE_FUNCTION_STATUS_TYPE_ACCEPT_DAILY, 1, NULL, NULL, 
		role->GetLevel(), role->GetRoleActivity()->GetHistoryChongZhi(), 0, 0);
}

void EventHandler::OnAcceptGuildTask(Role *role)
{
	if (NULL == role) return;

	gamelog::g_log_function_stats.printf(LL_INFO, "[Join][user[%s %d] type:GuildTask times:1]", role->GetName(), UidToInt(role->GetUserId()));

	ROLE_LOG_QUICK10(LOG_TYPE_FUNCTION_STATUS, role, LOG_TYPE_FUNCTION_STATUS_TYPE_ACCEPT_GUILDTASK, 1, NULL, NULL, 
		role->GetLevel(), role->GetRoleActivity()->GetHistoryChongZhi(), 0, 0);
}

void EventHandler::OnAcceptPaohuanTask(Role *role)
{
	if (NULL == role) return;
}

void EventHandler::OnEnterFBStory(Role *role)
{
	if (NULL == role) return;

}

void EventHandler::OnEnterFBExp(Role *role, int times)
{
	if (NULL == role) return;
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_EXP_FB);
	role->GetRoleGoal()->CheckCond(COND_TYPE_EXP_FB, 0, 0);
	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_EXP_FB_PASS_LEVEL, times);
	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_EXP_FB_WAVE);
	role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_FB_EXP);
	role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_ENTER_EXP_FB, 1);
	role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_EXP_FB, 1);
}

void EventHandler::OnEnterFBCoin(Role *role)
{
	if (NULL == role) return;
}

void EventHandler::OnEnterFBEquip(Role *role, bool is_first, bool is_trial)
{
	if (NULL == role) return;
	if (is_first)
	{
		role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_EQUIP_FB);
	}
}

void EventHandler::OnEnterFBTowerDefendPerson(Role *role, bool is_first)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnEnterSceneType(Scene::SCENE_TYPE_PERSONAL_TOWERDEFEND);
	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_TOWER_DEFEND_FB_ENTER_COUNT);

	if (is_first)
	{
		role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_TOWERDEFEND_FB);
	}
}

void EventHandler::OnEnterFBTowerDefendTeam(Role *role, bool is_first)
{
	if (NULL == role) return;

	if (is_first)
	{
		role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_TOWERDEFEND_TEAM);
	}
}

void EventHandler::OnEnterYaoShouGuangChang(Role *role)
{
	if (NULL == role) return;
}

void EventHandler::OnEnterSuoYaoTa(Role *role)
{
	if (NULL == role) return;
}

void EventHandler::OnEnterGuildFb(Role *role)
{
	if (NULL == role) return;
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_GUILD_FB);

	if (role->GetGuildID() != INVALID_GUILD_ID)
	{
		//ActivityShadow::Instance().SetGuildMap(role->GetGuildID());
	}
}

void EventHandler::OnEnterQingYuanFb(Role *role)
{
	if (NULL == role) return;
}

void EventHandler::OnPassYsjtTeamFb(Role *role)
{
	if (NULL == role) return;
}

void EventHandler::OnPassExpFb(Role * role)
{
	if (nullptr == role)
	{
		return;
	}
	role->GetTaskManager()->OnPassFB(FBManager::FBCT_DAILY_FB);
}

void EventHandler::OnPassTowerDfendTeamFb(Role *role)
{
	if (NULL == role) return;
}

void EventHandler::OnPassMgxfTeamFb(Role *role)
{
	if (NULL == role) return;
}

void EventHandler::OnShuiJingGather(Role *role)
{
	if (NULL == role) return;

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_SJHJ_SHUI_JIN_COUNT);
	role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_SHUIJING_GATHER, 1);
}

void EventHandler::OnAutoFbStory(Role *role)
{
	if (NULL == role) return;
}

void EventHandler::OnEnterQuanXianLuanDou(Role *role)
{
	if (NULL == role) return;

	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_QUNXIANLUANDOU);
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_YSZC);
}

void EventHandler::OnEnterGongchengzhan(Role *role)
{
	if (NULL == role) return;

	role->GetRoleActivity()->OnEnterGongchengzhan();
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_GONGCHENGZHAN);
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_GCZ);

	role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_GONGCHENG_ZHAN, 1);
	role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_GONG_CHENG_ZHAN, 1);
}

void EventHandler::OnEnterQuestion_2(Role *role)
{
	if (NULL == role) return;

	role->GetRoleActivity()->OnEnterQuestion_2();
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_QUESTION_2);
	//role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_ANSWER);
}

void EventHandler::OnEnterHotSpring(Role *role)
{
	if (NULL == role) return;
	
	//role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_HOTSPRING);
}

void EventHandler::OnEnterArmorDefendFB(Role *role, bool is_first)
{
	if (NULL == role) return;

	if (is_first)
	{
		role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_ARMORDEFEND_FB);
	}
}

void EventHandler::OnEnterBuildTowerFB(Role *role)
{
	if (NULL == role) return;

	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_BUILD_TOWER_FB);
}

void EventHandler::OnEnterEquipTeamFb(Role * role)
{
	if (NULL == role) return;

	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_EQUIP_TEAM_FB);
}
void EventHandler::OnEnterTerritoryWar(Role *role)
{
	if (NULL == role) return;

	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_TERRITORYWAR);
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_LINGTUZHAN);
}

void EventHandler::OnEnterGuildBattle(Role *role)
{
	if (NULL == role) return;

	role->GetRoleActivity()->OnEnterGuildBattle();
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_GUILDBATTLE);
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_GUILDBATTLE);
	role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_XIAN_MENG_ZHENG_BA, 1);
}

void EventHandler::OnEnterXianmengzhan(Role *role)
{
	if (NULL == role) return;

	role->GetRoleActivity()->OnEnterXianmengzhan();
}

void EventHandler::OnEnterZhuXieZhanChang(Role *role)
{
	if (NULL == role) return;

	role->GetRoleActivity()->OnEnterZhuXieZhanChang();
}

void EventHandler::OnEnterTianJiangCaiBao(Role *role)
{
	if (NULL == role) return;

	role->GetRoleActivity()->OnEnterTianJiangCaiBao();
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_TIANJIANGCAIBAO);
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_TJCB);
	role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_TIAN_JIANG_CAI_BAO, 1);
}

void EventHandler::OnEnterWangLingExplore(Role *role)
{
	if (NULL == role) return;

	role->GetRoleActivity()->OnEnterWangLingExplore();
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_WANGLINGEXPLORE);
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_WLTX);
	role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_WL_TANGXIAN, 1);
}

void EventHandler::OnEnterGuildYunBiao(Role *role)
{
	if (NULL == role) return;
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_GUILD_YUNBIAO);
}

void EventHandler::OnEnterShuijing(Role *role)
{
	if (NULL == role) return;
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_SHUIJING);
	role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_SHUI_JING_HUAN_JING, 1);
}

void EventHandler::OnEnterWorship(Role * role)
{
	if (NULL == role) return;
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_WORSHIP);
}

void EventHandler::OnEnterCross3v3(Role * role)
{
	if (NULL == role) return;
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_CROSS_MULTIUSER_CHALLENGE);
}

void EventHandler::OnEnterCrossShuijing(Role * role)
{
	if (NULL == role) return;
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_CROSS_SHUIJING);
}

void EventHandler::OnEnterCrossGuildBattle(Role * role)
{
	if (NULL == role) return;
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_CROSS_GUILDBATTLE);
}

void EventHandler::OnEnterCrossDarkNight(Role * role)
{
	if (NULL == role) return;
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_DARK_NIGHT);
}

void EventHandler::OnEnterCrossLieKun(Role * role)
{
	if (NULL == role) return;
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_LIEKUN_FB);
}

void EventHandler::OnEnterCRossXiuluoTower(Role * role)
{
	if (NULL == role) return;
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_XIULUOTOWER);
}

void EventHandler::OnBanBenChouJiang(Role* role, int num)
{
	if (nullptr == role) return;

	role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_BANG_BEN_CHOU_JIANG, num);
}

void EventHandler::OnBuyFbCount(Role *role, int scene_type)
{
	if (NULL == role) return;

}


void EventHandler::OnChestShopXunBao(Role *role, int shop_type, int count, bool is_free)
{
	if (NULL == role) return;

	if (!is_free)
	{
		WorldStatus::Instance().OnXunbao(count);
		role->GetRoleActivity()->SendOpenGameActivityInfo();
	}

	// 随机活动
	role->GetRoleActivity()->OnChestShopXunBao(count, is_free);

	if (!is_free)
	{
		role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_XUN_BAO, count);
	}

	if (CHEST_SHOP_TYPE_JINGLING == shop_type)
	{
		for (int i = 0; i < count; i++)
		{
			role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_JINGLINT_CHOUHUN);
		}
	}
}

void EventHandler::OnRobHusong(Role *role)
{
}

void EventHandler::OnGuildGongXian(Role *role, int today_gongxian)
{
}

void EventHandler::OnCompleteHusongTask(Role *role, int task_color)
{
	if (NULL == role) return;
	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_HUSONG_TIMES);
	role->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_HUSONG_TASK, 1, 0);
	role->GetRoleGoal()->CheckCond(COND_TYPE_HUSONG_TASK, 1, 0);
	role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_HUSONG, 1);
	role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_HU_SONG, 1);
}

void EventHandler::OnGuildDonate(Role *role, int donate_type, int donate_value)
{
	if (NULL == role)
	{
		return;
	}

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_GUILD_DONATE);
	if (Protocol::ADD_GUILD_EXP_TYPE_ITEM == donate_type)
	{
		role->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_GUILD_DONATE, donate_value, 0);
		role->GetRoleGoal()->CheckCond(COND_TYPE_GUILD_DONATE, donate_value, 0);
	}
}

void EventHandler::OnEnterGuildMonsterFB(Role *role, bool is_first)
{
	if (NULL == role)
	{
		return;
	}
}

void EventHandler::OnEnterChallengeFB(Role *role, int level, int layer)
{
	if (NULL == role)
	{
		return;
	}

	role->GetTaskManager()->OnEnterSceneType(Scene::SCENE_TYPE_CHALLENGE_FB);
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_CHALLENGE_FB);
}

void EventHandler::OnPassChallengeFB(Role *role, int level)
{
	if (NULL == role)
	{
		return;
	}

	role->GetTaskManager()->OnPassFB(FBManager::FBCT_CHALLENGE);
	role->GetTaskManager()->OnPassFBLayer(FBManager::FBCT_CHALLENGE);
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_CHALLENGE_FB);
	role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_CHALLENGE_FB, 1);
}

void EventHandler::OnPassStoryFB(Role *role, int fb_index)
{
	if (NULL == role) return;
}

void EventHandler::OnPassTowerDefendFB(Role *role, int level)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnPassFB(FBManager::FBCT_TOWERDEFEND_PERSONAL);
	role->GetTaskManager()->OnPassFBLayer(FBManager::FBCT_TOWERDEFEND_PERSONAL);

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_TOWERDEFEND_FB_LEVEL);//嘉年华

	role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_PROTECT_FB, 1);
}

void EventHandler::OnPassFBExp(Role *role)
{
	if (NULL == role) return;
}

void EventHandler::OnEnterPhaseFB(Role *role)
{
	if (NULL == role) return;

	role->GetRoleActivity()->OnEnterPhaseFB();
}

void EventHandler::OnPassPhaseFB(Role *role, int fb_type)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnPassFBLayer(FBManager::FBCT_PHASE, fb_type); // 通关进阶副本
	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_PHASE_FB_LEVEL);
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_PHASE_FB);
	role->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_PHASE_FB, 1, 0);
	role->GetRoleGoal()->CheckCond(COND_TYPE_PHASE_FB, 1, 0);
	role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_PASS_PHASE_FB, 1);
	role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_PHASE_FB, 1);
}

void EventHandler::OnPassPataFB(Role *role)
{
	if (NULL == role) return;

	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_PATA_FB);
	role->GetRoleGoal()->CheckCond(COND_TYPE_PATA_FB, 0, 0);
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_PATA_FB_LEVEL);//嘉年华
}

void EventHandler::OnPassRuneTowerFB(Role *role, int layer, bool is_auto)
{
	if (NULL == role) return;

	role->GetRoleGoal()->CheckCond(COND_TYPE_RUNE_TOWER, 0, 0);
	role->GetTaskManager()->OnPassFB(FBManager::FBCT_RUNE_TOWER);
	role->GetTaskManager()->OnPassFBLayer(FBManager::FBCT_RUNE_TOWER);
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_RUNE_TOWER);
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_RUNETOWER_LEVEL);//嘉年华

	if (!is_auto)
	{
		//江湖传闻
		GuildID guild_id = role->GetGuildID();
		if (INVALID_GUILD_ID != guild_id)
		{
			auto guild_rumor_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildRumorCfg(GUILD_EVENT_TYPE_RUMOR_RUNE_TOWER_FB);
			if (guild_rumor_cfg != NULL)
			{
				if (layer >= guild_rumor_cfg->param_1)
				{
					if (guild_rumor_cfg->param_2 > 0)
					{
						if (layer % guild_rumor_cfg->param_2 == guild_rumor_cfg->param_3)
						{
							this->OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_RUNE_TOWER_FB, layer);
						}
					}
				}
			}
		}
	}
}

void EventHandler::OnPassArmordefendFB(Role *role)
{
	role->GetTaskManager()->OnPassFBLayer(FBManager::FBCT_ARMOR_FB);
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_ARMORDEFEND_FB_LEVEL);//嘉年华
}

void EventHandler::OnEnterNationalBoss(Role *role)
{
	if (NULL == role)
	{
		return;
	}
}

void EventHandler::OnCompleteDailyTask(Role *role, int complete_times)
{
	if (NULL == role)
	{
		return;
	}

	if (complete_times > 0)
	{
		int cur_complete_times = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_COMMIT_DAILY_TASK_COUNT);

		role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_COMMIT_DAILY_TASK_COUNT, cur_complete_times + complete_times);

		role->GetDailyTask()->CheckSyncRewardFlag(complete_times > 1);
		
		role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_DAILY_TASK, complete_times);

		role->GetRoleActivity()->OnCompleteDailyTask(complete_times);

		role->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_DAYLY_TASK, complete_times, 0);
		role->GetRoleGoal()->CheckCond(COND_TYPE_DAYLY_TASK, complete_times, 0);
		role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_DAILY_TASK, complete_times);
		role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_EXP_TASK, complete_times);
	}
}

void EventHandler::OnCompleteGuildTask(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_GUILD_TASK_COMPLETE_COUNT);
	role->GetRoleActivity()->OnCompleteGuildTask();
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_GUILD_TASK);
	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_GUILD_FINISH_TASK);
	role->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_GUILD_TASK, 1, 0);
	role->GetRoleGoal()->CheckCond(COND_TYPE_GUILD_TASK, 1, 0);
	role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_GUILD_TASK, 1);
	role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_GUILD, 1);
}

void EventHandler::OnCompletePaohuanTask(Role *role, int complete_times)
{
	if (NULL == role)
	{
		return;
	}

	if (complete_times > 0)
	{
		role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_PAOHUAN_TASK, complete_times);
		role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_GUA_JI, complete_times);

		int cur_complete_times = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_COMMIT_PAOHUAN_TASK_COUNT);
		role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_COMMIT_PAOHUAN_TASK_COUNT, cur_complete_times + complete_times);
	}
}

void EventHandler::OnAnswerQuestion(Role *role, int answer_times)
{
	if (NULL == role)
	{
		return;
	}

	if (answer_times > 0)
	{
		int cur_answer_times = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_ANSWER_QUESTION_COUNT);

		role->GetDayCounter()->DayCountSet(DayCounter::DAYCOUNT_ID_ANSWER_QUESTION_COUNT, cur_answer_times + answer_times);

		{
			gamelog::g_log_roleactivity.printf(LL_INFO, "Question::Answer user[%d, %s], level[%d], answer_times[%d]", 
				role->GetUID(), role->GetName(), role->GetLevel(), role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_ANSWER_QUESTION_COUNT));
		}
	}
}

void EventHandler::OnBuyMonthCard(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	role->GetMonthCard()->OnBuyCard();
	WorldStatus::Instance().OnBuyMonthCard();
	role->GetRoleActivity()->SendOpenGameActivityInfo();
}

void EventHandler::OnLeaveGuild(Role *role, GuildID guild_id)
{
	if (NULL == role)
	{
		return;
	}
	
	role->GetTaskManager()->OnGuildTaskStateChange();
}

void EventHandler::OnJoinGuild(Role *role, GuildID guild_id)
{
	if (NULL == role)
	{
		return;
	}

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_GUILD);

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_JOIN_GUILD);
	role->GetTaskManager()->OnGuildTaskStateChange();
	role->GetRoleActivity()->OnJoinGuild(guild_id);

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_JOIN_GUILD);
}

void EventHandler::OnGatheXianJiu(Role *role)
{
	if (NULL == role)
	{
		return;
	}
}

void EventHandler::OnCompleteWaBao(Role *role)
{
	if (NULL == role) return;
	
	role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_WABAO, 1);
}

void EventHandler::OnEnterTeamFB(Role *role)
{
	if (NULL == role)
	{
		return;
	}
}

void EventHandler::OnEnterChallengeField(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_1V1_FIELD);
	role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_ARENA, 1);
}

void EventHandler::OnEnterZhanShenDian(Role *role)
{
	if (NULL == role)
	{
		return;
	}
}

void EventHandler::OnPassZhanshendian(Role *role, int level)
{
	if (NULL == role)
	{
		return;
	}
}

void EventHandler::OnCompleteJiLian(Role *role)
{
	if (NULL == role)
	{
		return;
	}
}

void EventHandler::OnCompleteCampTask(Role *role)
{
	if (NULL == role)
	{
		return;
	}
}

void EventHandler::OnCustomAvatarChange(Role *role, long long avatar_timestamp)
{
	if (NULL == role)
	{
		return;
	}
}

void EventHandler::OnBuyTouzijihua(Role *role, int plan_type)
{
	if (NULL == role)
	{
		return;
	}
}

void EventHandler::OnFriendBless(Role *role)
{
	if (NULL == role)
	{
		return;
	}
}

void EventHandler::OnXianjianSoulAddExp(Role *role)
{
	if(NULL == role)
	{
		return;
	}
}

void EventHandler::OnXianjianSoulActive(Role *role)
{
	if(NULL == role)
	{
		return;
	}
}

void EventHandler::OnXianjianActive(Role *role)
{
	if(NULL == role)
	{
		return;
	}
}

void EventHandler::OnQingyuanPromoteLevel(Role *role)
{
	if(NULL == role)
	{
		return;
	}
}

void EventHandler::OnQingyuanUpQuality(Role *role)
{
	if(NULL == role)
	{
		return;
	}
}

void EventHandler::OnQingyuanEquip2UpLevel(Role *role)
{
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_ACTIVE_QINGYUAN_EQUIP2);		 //嘉年华
}

void EventHandler::OnVipQifu(Role *role, int qifu_type, int qifu_times)
{
	if(NULL == role)
	{
		return;
	}

	role->GetRoleActivity()->OnVipQifu(qifu_type, qifu_times);
}

void EventHandler::OnSetSpecialAppearance(Role *role, int special_appearance_type)
{
	if (NULL == role)
	{
		return;
	}
}

void EventHandler::OnSetOtherSpecialAppearance(Role *role, int special_appearance_type)
{
	if (NULL == role)
	{
		return;
	}

	WorldEventManager::Instance().OnSetSpecialOtherAppearance(role, special_appearance_type);
}

void EventHandler::OnActiveDegreeComplete(Role *role,int active_degree_type)
{
	if (NULL == role)
	{
		return;
	}

	role->GetRoleActivity()->OnActiveDegreeComplete(active_degree_type);
	role->GetRoleActivity()->OnRAExtremeChallengeAddPlan(RA_EXTREME_CHALLENGE_ACTIVE, active_degree_type);
}

void EventHandler::OnDivorce(Role *role)
{
	LoveContract * love_contract = GLOBAL_ROLE_MGR.GetLoveContract(role->GetUID());
	if (NULL == love_contract) return;
	love_contract->OnDivorce();

	CoupleRankManager::Instance().OnDivorce(role);

	static crossgameprotocal::GameCrossCoupleRankDivorce pro;
	pro.plat_type = LocalConfig::Instance().GetPlatType();
	pro.uid = role->GetUID();

	InternalComm::Instance().SendToCross((const char *)&pro, sizeof(pro));
}

void EventHandler::OnMarry(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	MarriageSeekingManager::Instance().OnMarry(role);
	TuodanManager::Instance().OnMarry(role);
	role->GetRoleGoal()->CheckCond(COND_TYPE_TUO_DAN, 0, 0);
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_MARRY_SUCC);//嘉年华
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_MARRY_SUCC_TYPE);//嘉年华
	TianShuXunZhu::CheckShiXueTianShu(role, CHENGZHANG_TIANSHU_TYPE_ONMARRY);

	LoveContract * love_contract = GLOBAL_ROLE_MGR.GetLoveContract(role->GetUID());
	if (NULL == love_contract) return;
	love_contract->OnMarry();

	//江湖传闻
	if (role->GetSex() == FEMALE)
	{
		GuildID guild_id = role->GetGuildID();
		if (INVALID_GUILD_ID != guild_id)
		{
			Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role->GetLoverUid()));
			if (target_role != NULL)
			{
				if (guild_id == target_role->GetGuildID())
				{
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, target_role->GetName(), role->GetName(), GUILD_EVENT_TYPE_RUMOR_MARRY);
				}
				else
				{
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), target_role->GetName(), GUILD_EVENT_TYPE_RUMOR_MARRY);

					if (target_role->GetGuildID() != INVALID_GUILD_ID)
					{
						EventHandler::Instance().OnAddGuildRumorEvent(target_role->GetGuildID(), target_role->GetName(), role->GetName(), GUILD_EVENT_TYPE_RUMOR_MARRY);
					}
				}
			}
		}
		else
		{
			Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role->GetLoverUid()));
			if (target_role != NULL)
			{
				if (target_role->GetGuildID() != INVALID_GUILD_ID)
				{
					EventHandler::Instance().OnAddGuildRumorEvent(target_role->GetGuildID(), target_role->GetName(), role->GetName(), GUILD_EVENT_TYPE_RUMOR_MARRY);
				}
			}
		}
	}
}

void EventHandler::OnMarryRepeat(Role* role)
{
	if (nullptr == role)
	{
		return;
	}
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_MARRY_SUCC);//嘉年华
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_MARRY_SUCC_TYPE);//嘉年华

	//江湖传闻
	if (role->GetSex() == FEMALE)
	{
		GuildID guild_id = role->GetGuildID();
		if (INVALID_GUILD_ID != guild_id)
		{
			Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role->GetLoverUid()));
			if (target_role != NULL)
			{
				if (guild_id == target_role->GetGuildID())
				{
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, target_role->GetName(), role->GetName(), GUILD_EVENT_TYPE_RUMOR_MARRY);
				}
				else
				{
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), target_role->GetName(), GUILD_EVENT_TYPE_RUMOR_MARRY);

					if (target_role->GetGuildID() != INVALID_GUILD_ID)
					{
						EventHandler::Instance().OnAddGuildRumorEvent(target_role->GetGuildID(), target_role->GetName(), role->GetName(), GUILD_EVENT_TYPE_RUMOR_MARRY);
					}
				}
			}
		}
		else
		{
			Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role->GetLoverUid()));
			if (target_role != NULL)
			{
				if (target_role->GetGuildID() != INVALID_GUILD_ID)
				{
					EventHandler::Instance().OnAddGuildRumorEvent(target_role->GetGuildID(), target_role->GetName(), role->GetName(), GUILD_EVENT_TYPE_RUMOR_MARRY);
				}
			}
		}
	}
}

void EventHandler::OnRaProposeSucc(Role *role, Role *target_role)
{
	if (NULL == role || NULL == target_role)
	{
		return;
	}

	role->GetRoleActivity()->OnMarry(target_role);		//target 指的是被求婚者
}

void EventHandler::OnEnterYiZhanDaoDi(Role *role)
{
	if (NULL == role) return;

	role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_YI_ZHAN_FENG_SHEN, 1);

	gamelog::g_log_function_stats.printf(LL_INFO, "[Join][user[%s %d] type:YiZhanDaoDi times:1]", role->GetName(), UidToInt(role->GetUserId()));
}

void EventHandler::OnJoinTeam(Role *role)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_ADD_TEAM);

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_JOIN_TEAM);

	role->GetFBTowerDefend()->OnJoinTeam();
}

void EventHandler::OnLeaveTeam(Role *role)
{
	if (NULL == role) return;
	role->GetFBTowerDefend()->OnLeaveTeam();
}

void EventHandler::OnChangeSex(Role *role, int from_sex)
{
	if (NULL == role || from_sex == role->GetSex()) return;

	MarriageSeekingManager::Instance().OnChangeSex(role);
	TopRanker::Instance().OnRoleChangeSex(role, from_sex, role->GetSex());
}

void EventHandler::OnPersonRankSnapshot(int old_top_uid_list[PERSON_RANK_TYPE_MAX], int new_top_uid_list[PERSON_RANK_TYPE_MAX])
{
	RandActivityManager::Instance().OnPersonRankSnapshot(old_top_uid_list, new_top_uid_list);
}

void EventHandler::OnPersonRankChange(UserID uid, int rank_index, int rank_type, long long rank_value)
{
	if (rank_index >= 3)		//排行榜前三才播江湖传闻
	{
		return;
	}

	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(uid);
	if (role == NULL)
	{
		return;
	}

	//江湖传闻
	GuildID guild_id = role->GetGuildID();
	if (INVALID_GUILD_ID != guild_id)
	{
		switch (rank_type)
		{
			case PERSON_RANK_TYPE_WORLD_RIGHT_ANSWER:
			{
				if (role->GetRoleGuild()->GetRoleGuildParam()->world_ques_rank == -1 && rank_index >= 0)		//第一次上榜前3
				{
					role->GetRoleGuild()->GetRoleGuildParam()->world_ques_rank = rank_index;
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_WORLD_QUESTION_RANK, rank_index + 1);
				}
				else if (0 <= rank_index && rank_index < role->GetRoleGuild()->GetRoleGuildParam()->world_ques_rank)
				{
					role->GetRoleGuild()->GetRoleGuildParam()->world_ques_rank = rank_index;
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_WORLD_QUESTION_RANK, rank_index + 1);
				}
			}
			break;

			case PERSON_RANK_TYPE_CHARM:
			{
				if (role->GetRoleGuild()->GetRoleGuildParam()->charm_rank == -1 && rank_index >= 0)
				{
					role->GetRoleGuild()->GetRoleGuildParam()->charm_rank = rank_index;
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_CHARM_RANK, rank_index + 1);
				}
				else if (0 <= rank_index && rank_index < role->GetRoleGuild()->GetRoleGuildParam()->charm_rank)
				{
					role->GetRoleGuild()->GetRoleGuildParam()->charm_rank = rank_index;
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_CHARM_RANK, rank_index + 1);
				}
			}
			break;

			case PERSON_RANK_TYPE_RA_PROFESS_MALE:														//表白榜男榜
			{
				if (role->GetRoleGuild()->GetRoleGuildParam()->profess_rank == -1 && rank_index >= 0)
				{
					role->GetRoleGuild()->GetRoleGuildParam()->profess_rank = rank_index;
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_EXPRESS_RANK, rank_index + 1);
				}
				else if (0 <= rank_index && rank_index < role->GetRoleGuild()->GetRoleGuildParam()->profess_rank)
				{
					role->GetRoleGuild()->GetRoleGuildParam()->profess_rank = rank_index;
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_EXPRESS_RANK, rank_index + 1);
				}
			}
			break;

			case PERSON_RANK_TYPE_RA_PROFESS_FEMALE:													//表白榜女榜
			{
				if (role->GetRoleGuild()->GetRoleGuildParam()->profess_rank == -1 && rank_index >= 0)
				{
					role->GetRoleGuild()->GetRoleGuildParam()->profess_rank = rank_index;
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_EXPRESS_RANK, rank_index + 1);
				}
				else if (0 <= rank_index && rank_index < role->GetRoleGuild()->GetRoleGuildParam()->profess_rank)
				{
					role->GetRoleGuild()->GetRoleGuildParam()->profess_rank = rank_index;
					EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_EXPRESS_RANK, rank_index + 1);
				}
			}
			break;
		}
	}
}

void EventHandler::OnCoupleRankChange(UserID uid, int rank_index, int rank_type, long long rank_value)
{
	if (rank_index >= 3)				//排行榜前三才播江湖传闻
	{
		return;
	}

	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(uid);
	if (role == NULL)
	{
		return;
	}

	//江湖传闻
	GuildID guild_id = role->GetGuildID();
	if (INVALID_GUILD_ID != guild_id)
	{
		switch (rank_type)
		{
		case COUPLE_RANK_TYPE_QINGYUAN_CAP:
		{
			if (role->GetRoleGuild()->GetRoleGuildParam()->qingyuan_rank == -1 && rank_index >= 0)					//第一次上榜前3
			{
				role->GetRoleGuild()->GetRoleGuildParam()->qingyuan_rank = rank_index;

				GameName lover_name;
				role->GetLoverName(lover_name);
				EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), lover_name, GUILD_EVENT_TYPE_RUMOR_BE_LOVER_RANK, rank_index + 1);
			}
			else if (0 <= rank_index && rank_index < role->GetRoleGuild()->GetRoleGuildParam()->qingyuan_rank)
			{
				role->GetRoleGuild()->GetRoleGuildParam()->qingyuan_rank = rank_index;

				GameName lover_name;
				role->GetLoverName(lover_name);
				EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), lover_name, GUILD_EVENT_TYPE_RUMOR_BE_LOVER_RANK, rank_index + 1);
			}
		}
		break;

		case COUPLE_RANK_TYPE_BABY_CAP:
		{
			if (role->GetRoleGuild()->GetRoleGuildParam()->baby_rank == -1 && rank_index >= 0)
			{
				role->GetRoleGuild()->GetRoleGuildParam()->baby_rank = rank_index;

				GameName lover_name;
				role->GetLoverName(lover_name);
				EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), lover_name, GUILD_EVENT_TYPE_RUMOR_BE_BABY_RANK, rank_index + 1);
			}
			else if (0 <= rank_index && rank_index < role->GetRoleGuild()->GetRoleGuildParam()->baby_rank)
			{
				role->GetRoleGuild()->GetRoleGuildParam()->baby_rank = rank_index;

				GameName lover_name;
				role->GetLoverName(lover_name);
				EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), lover_name, GUILD_EVENT_TYPE_RUMOR_BE_BABY_RANK, rank_index + 1);
			}
		}
		break;

		case COUPLE_RANL_TYPE_LITTLE_PET:
		{
			if (role->GetRoleGuild()->GetRoleGuildParam()->li_pet_rank == -1 && rank_index >= 0)
			{
				role->GetRoleGuild()->GetRoleGuildParam()->li_pet_rank = rank_index;

				GameName lover_name;
				role->GetLoverName(lover_name);
				EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), lover_name, GUILD_EVENT_TYPE_RUMOR_LITTLE_PET_RANK, rank_index + 1);
			}
			else if (0 <= rank_index && rank_index < role->GetRoleGuild()->GetRoleGuildParam()->li_pet_rank)
			{
				role->GetRoleGuild()->GetRoleGuildParam()->li_pet_rank = rank_index;

				GameName lover_name;
				role->GetLoverName(lover_name);
				EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), lover_name, GUILD_EVENT_TYPE_RUMOR_LITTLE_PET_RANK, rank_index + 1);
			}
		}
		break;
		}
	}
}

void EventHandler::OnKillRoleShadow(Role *role, RoleShadow *role_shadow)
{
	if (NULL == role || NULL == role_shadow)
	{
		return;
	}

	if (ROLE_SHADOW_TYPE_ROLE_BOSS == role_shadow->GetRoleShadowType())
	{
		RoleBossManager::Instance().OnKillRoleShadow(role, role_shadow);
	}

	if (ROLE_SHADOW_TYPE_CAMPDEFEND == role_shadow->GetRoleShadowType())
	{
		CampDefendMgr::Instance().OnDiaoXiangDie(role, role_shadow);
	}
}

void EventHandler::OnChangeXianmengzhanWiner(int old_winer, int new_winer)
{
	//CombineServerActivityManager::Instance().OnChangeXianmengzhanWiner(old_winer, new_winer);
}

void EventHandler::OnChangeGongchengzhanWiner(int old_winer, int new_winer)
{
	CombineServerActivityManager::Instance().OnChangeGongchengzhanWiner(old_winer, new_winer);

	CommonDataGlobal *old_common_global = GlobalRoleManager::Instance().GetCommonDataGlobal(old_winer);
	if (nullptr != old_common_global)
	{
		old_common_global->ClearChengzhu();
	}

	CommonDataGlobal *new_common_global = GlobalRoleManager::Instance().GetCommonDataGlobal(new_winer);
	if (nullptr != new_common_global)
	{
		new_common_global->SetChengzhu();
	}

	std::set<Role*> role_list;
	World::GetInstWorld()->GetSceneManager()->GetAllRole(role_list);
	for (auto role : role_list)
	{
		role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_GONGCHENG_ZHAN_WINER);//嘉年华
	}
}

void EventHandler::OnEffectHpStoreBroken(Character *cha, ObjID deliverer, UInt16 product_id, char product_method)
{
	if (NULL == cha) return;
}

void EventHandler::OnAddChengJiu(Role *role, int old_chengjiu, int new_chengjiu)
{
	if (NULL == role) return;

	if (new_chengjiu > old_chengjiu)
	{
		role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_CHENGJIU_TITLE);
	}
}

void EventHandler::OnAddAllCoin(Role *role, Int64 old_all_coin, Int64 new_allcoin)
{
	if (NULL == role) return;
}

void EventHandler::OnUpgradeSysUpgrade(Role *role, int upgrade_type, int old_grade, int new_grade)
{
	if (nullptr == role) return;
	if (old_grade >= new_grade) return;
		
	WorldStatus2::Instance().IncTotalUpgradeRecordList(upgrade_type, new_grade);

	int jinjie_type = JinjiesysFuncHelper::Instance().UpgradeManagerTypeToJinJieSysType(upgrade_type);
	if (JINJIE_TYPE_INVALID != jinjie_type)
	{
		JinjiesysFuncHelper::Instance().OnUpgrade(role, jinjie_type, old_grade, new_grade);
	}

	int theme = RoleActivity::JinJieActThemeGetByUpgrade(upgrade_type);
	role->GetRoleActivity()->OnRaJinJieOpera(theme, new_grade,old_grade);   // 进阶丹返还活动
	
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_UPGRADE_SYS_GRADE);//嘉年华
}

void EventHandler::OnUpgradeSysActive(Role *role, int upgrade_type, bool is_recal)
{
	if (nullptr == role) return;
	
	int jinjie_type = JinjiesysFuncHelper::Instance().UpgradeManagerTypeToJinJieSysType(upgrade_type);
	if (JINJIE_TYPE_INVALID != jinjie_type)
	{
		JinjiesysFuncHelper::Instance().OnActiveJinjieSys(role, jinjie_type);
	}
}

void EventHandler::OnUpgradeSysActiveSpecialImage(Role *role, int upgrade_type, short special_img_id, bool is_notify)
{
	if (nullptr == role) return;
		

	int jinjie_type = JinjiesysFuncHelper::Instance().UpgradeManagerTypeToJinJieSysType(upgrade_type);
	if (JINJIE_TYPE_INVALID != jinjie_type)
	{
		JinjiesysFuncHelper::Instance().OnActiveSpecialImg(role, jinjie_type, special_img_id);
	}
}

void EventHandler::OnUpSkillLevel(Role *role, int skill_id, int old_level, int new_level, int total_skill_level)
{
	if (NULL == role) return;

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_SKILL_TOTAL_LEVEL, total_skill_level);
}

void EventHandler::OnUpXianJieLevel(Role *role, int old_level, int new_level)
{
	if (NULL == role) return;
}

void EventHandler::OnUpChengJiuTitleLevel(Role *role, int old_level, int new_level)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_CHENGJIU_TITLE);
	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_CHENGJIU_TITLE, new_level);
	role->GetRoleGoal()->CheckCond(COND_TYPE_CHENG_JIU_LEVEL, 0, 0);
}

void EventHandler::OnAccountTotalLoginDayCountChange(Role *role, int day_count)
{
	if (NULL == role) return;

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_LOGIN_LAST_DAY, day_count);
}

void EventHandler::OnWingUpgrade(Role *role, int old_jie, int new_jie)
{
	if (NULL == role || old_jie >= new_jie) return;

	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_WING_GRADE);
	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_WING_JIE_CHANGE, new_jie);
	//role->GetUpgradeRush()->OnUpgradeLevelChange(UPGRADE_RUSH_TYPE_WING, new_jie);
	role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_WING, new_jie,old_jie);   // 进阶丹返还活动
	WorldStatus::Instance().IncTotalUpgradeWingRecordList(new_jie);

	if (new_jie > old_jie)
	{
		role->GetRoleGoal()->CheckCond(COND_TYPE_UPGRADE_WING, 0, 0);
		role->GetFamousMan()->OnCheckFamousMan(FAMOUS_MAN_WING_GRADE, new_jie);
		role->GetJinJieTalent()->OnCheckOpenTalentGrid(JINJIE_TALENT_WING, new_jie);
		JinjiesysFuncHelper::Instance().OnUpgrade(role, JINJIE_TYPE_WING, old_jie, new_jie);
		role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_WING_GRADE);//嘉年华
	}
}

void EventHandler::OnShiZhuangUpgrade(Role *role, int old_jie, int new_jie)
// 时装阶数变化
{
	if (nullptr == role || old_jie >= new_jie)
	{
		return;
	}
	role->GetJinJieTalent()->OnCheckOpenTalentGrid(JINJIE_TALENT_SHIZHUANG, new_jie);

	if (new_jie > old_jie && new_jie > 0)
	{
		role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_FASHION, new_jie,old_jie);   // 进阶丹返还活动
	}

	WorldStatus2::Instance().IncTotalUpgradeShizhuangRecordList(new_jie);
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_SHIZHUANG_GRADE);//嘉年华
	JinjiesysFuncHelper::Instance().OnUpgrade(role, JINJIE_TYPE_SHIZHUANG, old_jie, new_jie);
}
void EventHandler::OnShenBingUpgrade(Role *role, int old_jie, int new_jie)
// 神兵阶数变化
{
	if (nullptr == role || old_jie >= new_jie)
	{
		return;
	}
	role->GetJinJieTalent()->OnCheckOpenTalentGrid(JINJIE_TALENT_SHENBING, new_jie);

	if (new_jie > old_jie && new_jie > 0)
	{
		role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_SHENBING, new_jie,old_jie);   // 进阶丹返还活动
	}
	
	WorldStatus2::Instance().IncTotalUpgradeShenbingRecordList(new_jie);
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_SHENBING_GRADE);//嘉年华
	JinjiesysFuncHelper::Instance().OnUpgrade(role, JINJIE_TYPE_SHENBING, old_jie, new_jie);
}

void EventHandler::OnHaloUpgrade(Role *role, int old_jie, int new_jie)
{
	if (NULL == role || old_jie >= new_jie) return;

	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_HALO_GRADE);

	WorldStatus::Instance().IncTotalUpgradeHaloRecordList(new_jie);
	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_GUANG_HUAN_LEVEL, new_jie);
	role->GetRoleGoal()->CheckCond(COND_TYPE_UPGRADE_HALO, 0, 0);
	//role->GetUpgradeRush()->OnUpgradeLevelChange(UPGRADE_RUSH_TYPE_HALO, new_jie);
	role->GetJinJieTalent()->OnCheckOpenTalentGrid(JINJIE_TALENT_HALO, new_jie);
	JinjiesysFuncHelper::Instance().OnUpgrade(role, JINJIE_TYPE_HALO, old_jie, new_jie);

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_HALO_GRADE);//嘉年华
	role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_GUANGHUAN, new_jie,old_jie);   // 进阶丹返还活动
}

void EventHandler::OnShengongUpgrade(Role *role, int old_jie, int new_jie)
{
	if (NULL == role || old_jie >= new_jie) return;

	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_SHENGONG_GRADE);

	WorldStatus::Instance().IncTotalUpgradeShengongRecordList(new_jie);
	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_SHEN_GONG_LEVEL, new_jie);
	role->GetRoleGoal()->CheckCond(COND_TYPE_UPGRADE_SHENGONG, 0, 0);
	JinjiesysFuncHelper::Instance().OnUpgrade(role, JINJIE_TYPE_SHENGONG, old_jie, new_jie);
	role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_SHENGONG, new_jie,old_jie);   // 进阶丹返还活动
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_SHENGONG_GRADE);//嘉年华
	//role->GetJinJieTalent()->OnCheckOpenTalentGrid(JINJIE_TALENT_SHENGGONG, new_jie);
}

void EventHandler::OnShenyiUpgrade(Role *role, int old_jie, int new_jie)
{
	if (NULL == role || old_jie >= new_jie) return;

	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_SHENYI_GRADE);

	WorldStatus::Instance().IncTotalUpgradeShenyiRecordList(new_jie);
	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_SHEN_WING_LEVEL, new_jie);
	role->GetRoleGoal()->CheckCond(COND_TYPE_UPGRADE_SHENYI, 0, 0);
	JinjiesysFuncHelper::Instance().OnUpgrade(role, JINJIE_TYPE_SHENYI, old_jie, new_jie);
	role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_SHENYI, new_jie,old_jie);
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_SHENYI_GRADE);//嘉年华
	//role->GetJinJieTalent()->OnCheckOpenTalentGrid(JINJIE_TALENT_SHENYI, new_jie);
}

void EventHandler::OnFaBaoUpgrade(Role *role, int old_jie, int new_jie)
{
	if (NULL == role || old_jie >= new_jie) return;
	role->GetJinJieTalent()->OnCheckOpenTalentGrid(JINJIE_TALENT_FABAO, new_jie);
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_FABAO_GRADE);//嘉年华

	role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_FABAO, new_jie,old_jie);   // 进阶丹返还活动
	WorldStatus2::Instance().IncTotalUpgradeFabaoRecordList(new_jie);
	JinjiesysFuncHelper::Instance().OnUpgrade(role, JINJIE_TYPE_FABAO, old_jie, new_jie);
}

void EventHandler::OnFootprintUpgrade(Role *role, int old_jie, int new_jie)
{
	if (NULL == role || old_jie >= new_jie) return;

	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_FOOTPRINT_GRADE);

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_FOOTPRINT_LEVEL, new_jie);
	role->GetRoleGoal()->CheckCond(COND_TYPE_UPGRADE_FOOTPRINT, 0, 0);
	//role->GetUpgradeRush()->OnUpgradeLevelChange(UPGRADE_RUSH_TYPE_FOOTPRINT, new_jie);
	role->GetJinJieTalent()->OnCheckOpenTalentGrid(JINJIE_TALENT_FOOTPRINT, new_jie);
	role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_FOOTSPRINT, new_jie,old_jie);   // 进阶丹返还活动
	JinjiesysFuncHelper::Instance().OnUpgrade(role, JINJIE_TYPE_FOOTPRINT, old_jie, new_jie);		// 大小目标
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_FOOTPRINT_GRADE); //嘉年华
	WorldStatus2::Instance().IncTotalUpgradeFootprintRecordList(new_jie);
}

void EventHandler::OnPutOnJinglingCount(Role *role, int old_count, int new_count)
{
	if (NULL == role || old_count >= new_count) return;

	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_PUT_JINLING_COUNT);

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_COLLECT_JINGLING); //嘉年华
}

void EventHandler::OnPataFBPassLevel(Role *role, int old_level, int new_level)
{
	if (NULL == role || old_level >= new_level) return;

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_PATA_PASS_LEVEL, new_level);
	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_PATA_FB_LEVEL);
	role->GetFamousMan()->OnCheckFamousMan(FAMOUS_MAN_TOWER_LEVEL, new_level);

	//江湖传闻
	GuildID guild_id = role->GetGuildID();
	if (INVALID_GUILD_ID != guild_id)
	{
		auto guild_rumor_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildRumorCfg(GUILD_EVENT_TYPE_RUMOR_PATA_FB);
		if (guild_rumor_cfg != NULL)
		{
			if (new_level >= guild_rumor_cfg->param_1)
			{
				if (guild_rumor_cfg->param_2 > 0)
				{
					if (new_level % guild_rumor_cfg->param_2 == guild_rumor_cfg->param_3)
					{
						this->OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_PATA_FB, new_level);
					}
				}
			}
		}
	}

}

void EventHandler::OnExpFBPassWave(Role *role, int old_wave, int new_wave)
{
	if (NULL == role || old_wave >= new_wave) return;
}


void EventHandler::OnTowerdefendteamFBPassWave(Role *role, int old_wave, int new_wave)
{
	if (NULL == role || old_wave >= new_wave) return;

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_TOWERDEFEND_TEAM_FB_WAVE); //嘉年华
}

void EventHandler::OnEquipTeamFBPass(Role *role, int new_layer)
{
	if (NULL == role)
	{
		return;
	}
}

void EventHandler::OnRecycleItem(Role *role, ItemID item_id)
{
	if (NULL == role || item_id <= 0) return;

	//装备回收
	{
		const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(item_id);
		if (NULL == equip_item || ItemBase::I_TYPE_EQUIPMENT != equip_item->GetItemType())
		{
			return;
		}
		if (equip_item->GetEquipType() < Equipment::E_TYPE_MIN || equip_item->GetEquipType() >= Equipment::E_TYPE_MAX)
		{
			return;
		}

		role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_EQUIP_RECYCLE);
		role->GetRoleGoal()->CheckCond(COND_TYPE_RECYCL_EQUIP, 1, 0);
	}
}

void EventHandler::OnLieMingChouHun(Role *role)
{
	if (NULL == role) return;
	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_LIEMING_CHOUHUN);
}

void EventHandler::OnLieMingHunAddExp(Role *role)
{
	if (NULL == role) return;
	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_LIEMING_HUN_ADD_EXP);
}

void EventHandler::OnInlayStone(Role *role, ItemID item_id)
{
	if (NULL == role) return;
	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_INLAY_STONE);

	role->GetRoleGoal()->CheckCond(COND_TYPE_TOTAL_STONE, 0, 0);
	role->GetRoleGoal()->CheckCond(COND_TYPE_STONE, 0, 0);
	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_INLAY_STONE);

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_EQUIP_STONE); // 嘉年华

	role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_STONE, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_2, role->GetStoneManager()->GetTotalStoneLevel());
}

void EventHandler::OnUplevelstone(Role *role)
{
	if (NULL == role) return;

	role->GetRoleGoal()->CheckCond(COND_TYPE_TOTAL_STONE, 0, 0);
	role->GetRoleGoal()->CheckCond(COND_TYPE_STONE, 0, 0);

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_EQUIP_STONE); // 嘉年华

	//检查宝石大小目标
	role->GetStoneManager()->CheckFuncOpen();
	role->GetRoleBigSmallGoal()->OnSystemOperaCheck(ROLE_BIG_SMALL_GOAL_SYSTEM_STONE, ROLE_BIG_SMALL_GOAL_CONDITION_TYPE_2, role->GetStoneManager()->GetTotalStoneLevel());
}

void EventHandler::OnJingLingUpLevel(Role *role, ItemID item_id, int to_level)
{
	if (NULL == role) return;
	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_JINGLING_MAX_LEVEL);
	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_ZHUANGBEI_JINGLING_LEVEL, to_level);
	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_JINGLING_UP_LEVEL);
}

void EventHandler::OnJingLingWuxingUpLevel(Role *role, int to_level)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_JINGLING_WUXING_UP_LEVEL);
}

void EventHandler::OnJingLingGather(Role *role, int count)
{
	if (NULL == role) return;
	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_JINGLING_GATHER, count);
}

void EventHandler::OnDestroyTeam(int team_index)
{
	MiGongXianFuMgr::Instance().OnDestroyTeam(team_index);
}

void EventHandler::OnGatherBonfire(Role *role)
{
	if (NULL == role) return;
}

void EventHandler::OnShenZhuangJinJie(Role *role)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_SZ_JINJIE);
}

void EventHandler::OnShenZhuangShenZhu(Role *role)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_SZ_SHENZHU);
}

void EventHandler::OnJingLingFeiSheng(Role *role)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_JL_FEISHENG);
}

void EventHandler::OnJingLingEquipmentStrength(Role *role)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_JL_ZHUANGBEI);
}

void EventHandler::OnJingLingPuton(Role *role, int level)
{
	if (NULL == role) return;

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_ZHUANGBEI_JINGLING_LEVEL, level);
}

void EventHandler::OnJingLingSkillChange(Role *role)
{
	if (NULL == role) return;

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_COLLECT_JINGLING_SKILL); // 嘉年华
}

void EventHandler::OnActiveHeshenluoshu(Role *role)
{
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_ACTIVE_LUOSHU); // 嘉年华
}

void EventHandler::OnJingjieUplevel(Role *role)
{
	if (NULL == role) return;

	//嘉年华
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_JINEJIE_LEVEL);

	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_JINGJIE_LEVEL);
}

void EventHandler::OnEnterCrossTeamFb(Role *role)
{
	if (NULL == role) return;

	role->GetRoleActivity()->OnEnterCrossTeamFb();
	role->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_CROSS_TEAM_FB, 1, 0);
	role->GetRoleGoal()->CheckCond(COND_TYPE_CROSS_TEAM_FB, 1, 0);
}

void EventHandler::OnEnterTeamFb(Role *role)
{
	if (NULL == role) return;
	role->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_CROSS_TEAM_FB, 1, 0);
	role->GetRoleGoal()->CheckCond(COND_TYPE_CROSS_TEAM_FB, 1, 0);
}

void EventHandler::OnPassTeamFb(Role *role)
{
	if (NULL == role) return;
}

void EventHandler::OnEnterCross1v1(Role *role)
{
	if (NULL == role) return;

	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_CROSS_1V1);
	role->GetRoleActivity()->OnEnterCross1v1();
	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_CROSS_1V1);
}

void EventHandler::OnCompleteCrossFB(Role *role)
{
	if (NULL == role) return;
}

void EventHandler::OnOpenGuildBox(Role *role)
{
	if (NULL == role) return;

	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_GUILD_BOX);
	role->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_GUILD_BOX_DIG, 1, 0);
	role->GetRoleGoal()->CheckCond(COND_TYPE_GUILD_BOX_DIG, 1, 0);
	role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_OPEN_GUILD_BOX, 1);
}

void EventHandler::OnFetchGuildBox(Role *role)
{
	if (NULL == role) return;

	role->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_GUILD_BOX_OPEN, 1, 0);
	role->GetRoleGoal()->CheckCond(COND_TYPE_GUILD_BOX_OPEN, 1, 0);
}

void EventHandler::OnUplevelZhiBao(Role *role, int level)
{
	if (NULL == role) return;

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_FABAO_LEVEL, level);
	role->GetRoleGoal()->CheckCond(COND_TYPE_ZHI_BAO_LEVEL, 0, 0);
}

void EventHandler::OnXiuLuoTaKill(Role *role)
{
	if (NULL == role) return;

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_XLT_KILL_ROLE);
}

void EventHandler::OnGongChengZhanWin(Role *role)
{
	if (NULL == role) return;

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_GCZ_WIN_TIMES);
}

void EventHandler::OnCoss1v1FightResult(Role *role, bool is_win)
{
	if (NULL == role) return;
	
	if (is_win)
	{
		role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_1V1_WIN_TIMES);
	}
}

void EventHandler::OnPutEquipInGuild(Role *role)
{
	if (NULL == role) return;

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_GUILD_DONATION_EQU);
	role->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_GUILD_STORAGE, 1, 0);
	role->GetRoleGoal()->CheckCond(COND_TYPE_GUILD_STORAGE, 1, 0);
}

void EventHandler::OnPutTerritorWarKillTa(Role *role)
{
	if (NULL == role) return;

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_LTZ_SKILL_TA);
}

void EventHandler::OnKillRole(Role *dead, Role *killer)
{
	if (NULL == dead || NULL == killer) return;

	killer->OnKillOtherRole(dead);
	
	killer->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_SKILL_ROLE);

	if (GLOBALCONFIG->IsRedNameColor(dead->GetEvil()))
	{
		killer->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_SKILL_RED_NAME_ROLE);
	}
}

void EventHandler::OnActiveMoJie(Role *role, int mojie_itemid)
{
	if (NULL == role) return;
	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_ACTIVITE_MOJIE, mojie_itemid);
}

void EventHandler::OnActiveXiannv(Role *role, int xiannv_itemid)
{
	if (NULL == role) return;
	
	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_ACTIVITE_NVSHEN, xiannv_itemid);
	role->GetRoleGoal()->CheckCond(COND_TYPE_ACTIVE_XIANNV, 0, 0);
	role->GetFamousMan()->OnCheckFamousMan(FAMOUS_MAN_ACTIVE_XIANNV, xiannv_itemid);
	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_ACTIVE_XIANNV);

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_ACTIVE_XIANNV); // 嘉年华
}

void EventHandler::OnXunzhangUpLevel(Role *role, int total_level)
{
	if (NULL == role) return;

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_XUN_ZHANG_TOTLE_LEVEL, total_level);
	role->GetRoleGoal()->CheckCond(COND_TYPE_XUN_ZHANG_LEVEL, total_level, 0);
}

void EventHandler::OnKillGuildBoss(Role *role)
{
	if (NULL == role) return;

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_SKILL_GUILD_BOSS);
}

void EventHandler::OnChestShop(Role *role, int shop_type, int item_id)
{	
	if (NULL == role) return;

	if (CHEST_SHOP_TYPE_EQUIP == shop_type)
	{
		
	}
	else if(CHEST_SHOP_TYPE_JINGLING == shop_type)
	{
		role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_LIEQU_JINGLING_TOTAL);
		role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_LIEQU_JINGLING, item_id);
	}
}

void EventHandler::OnUpdateTuodan(Role *role)
{
	if (NULL == role) return;

	role->GetRoleGoal()->RoleGoalAddRecordTimes(COND_TYPE_TUO_DAN, 1, 0);
	role->GetRoleGoal()->CheckCond(COND_TYPE_TUO_DAN, 1, 0);
}

void EventHandler::OnXiannvUplevelZizhi(Role *role, int level)
{
	if (NULL == role) return;

	role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_NVSHEN_LEVEL, level);
	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_XIANNV_UP_ZIZHI);
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_ACTIVE_XIANNV); // 嘉年华
}

void EventHandler::OnRuneUplevel(Role *role, int level)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_RUNE_UP_LEVEL);
}

void EventHandler::OnUplevelShenBing(Role *role, int level)
{

}

void EventHandler::OnHotspringJoin(Role *role)
{
	if (NULL == role) return;
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_HOT_SPRING);
	role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_HOTSPRING_JOIN, 1);

}

void EventHandler::OnHotspringAnswerRight(Role *role)
{
	if (NULL == role) return;

	role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_HOTSPRING_ANSWER_RIGHT, 1);
}

void EventHandler::OnMillionaireGatherBox(Role *role)
{
	if (NULL == role) return;

	role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_MILLIONAIRE_BOX, 1);
}

void EventHandler::OnWanglingFinishTask(Role *role)
{
	if (NULL == role) return;

}

void EventHandler::OnTianjiangcaibaoFinishTask(Role *role)
{
	if (NULL == role) return;

	role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_TIANJIANGCAIBAO_TASK, 1);
}

void EventHandler::OnShuijingFinishTask(Role *role)
{
	if (NULL == role) return;

	role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_SHUIJING_TASK, 1);
}

void EventHandler::OnChatOnWorldChannel(Role *role)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_CHAT_ON_WORLD_CHANNEL);
}

void EventHandler::OnPassFbTuitu(Role *role)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_STATIFY_STATUS_TYPE_TUIFU_NORMAL_FB);
	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_STATIFY_STATUS_TYPE_TUIFU_HARD_FB);
}

void EventHandler::OnPassEquipFb(Role *role, int chapter, int level)
{
	if (NULL == role) return;

	role->GetTaskManager()->OnPassFBLayer(FBManager::FBCT_NEQ_FB);

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_NEWEQUIP_FB_LEVEL); //嘉年华

	role->GetTouzijihua()->OnPassFb();
//	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_EQUIP_FB);
// 	role->GetRoleActivity()->OnPassEquipFB(chapter, level);
	//role->GetRoleGoal()->OnPassEquipFB(chapter, level);
}

void EventHandler::OnGetItem(Role * role, ItemID item_id, int num)
{
	if (nullptr == role) return;

	role->GetTaskManager()->OnGetItem(item_id, num);
	if (num > 0)
	{
		// 人物大小目标
		role->GetRoleBigSmallGoal()->OnGetItem(item_id, num);
	}
}

void EventHandler::OnLightUpZhuanZhi(Role * role, int zhuanzhi_type, short level)
{
	if (nullptr == role) return;
	role->GetTaskManager()->OnCheckSatisfyStatus(zhuanzhi_type);
	role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_CON_TO_PROF6);
}

void EventHandler::OnHunyinInlay(Role *role, int hunqi_id, int hunyin_slot)
{
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_SHENZHOU_WEAPON_INLAY_COUNT);//嘉年华
}

void EventHandler::OnPutonShenshouEquip(Role *role, int shenshou_id, int item_id)
{
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_SHENSHOU_EQUIP_COUNT);//嘉年华
}

void EventHandler::OnJoinFishingActivity(Role *role)
{
	if (nullptr == role) return;

	role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_JOIN_FISHING);
	role->GetWelfare()->OnJoinActivity(ACTIVITY_FIND_TYPE_FISHING);
}

void EventHandler::OnForbidRole(int uid)
{
	ChatManager::Instance().OnChatForbidRole(uid);
}

void EventHandler::OnCross1v1DurWin(Role *role, int num)
{

}

void EventHandler::OnYaoShiUpgrade(Role *role, int old_grade, int to_grade)
{
	if (NULL == role) return;

	// 	role->GetRoleActivity()->OnYaoShiUpgrade(old_grade, to_grade);
	 
	 	if (to_grade > old_grade && to_grade > 0)
	 	{
	 		/*role->GetRoleActivity()->SendOpenGameActivityInfo();*/
			
			WorldStatus2::Instance().IncTotalUpgradeYaoshiRecordList(to_grade);
			JinjiesysFuncHelper::Instance().OnUpgrade(role, JINJIE_TYPE_YAOSHI, old_grade, to_grade);
			role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_YAOSHI, to_grade,old_grade);   // 进阶丹返还活动
			role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_YAOSHI_GRADE);//嘉年华
	 	}
}

void EventHandler::OnTouShiUpgrade(Role *role, int old_grade, int to_grade)
{
	if (NULL == role) return;

	// 	role->GetRoleActivity()->OnTouShiUpgrade(old_grade, to_grade);
	// 
	if (to_grade > old_grade && to_grade > 0)
	{
		/*role->GetRoleActivity()->SendOpenGameActivityInfo();*/

		WorldStatus2::Instance().IncTotalUpgradeToushiRecordList(to_grade);
		JinjiesysFuncHelper::Instance().OnUpgrade(role, JINJIE_TYPE_TOUSHI, old_grade, to_grade);
		role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_TOUSHI, to_grade,old_grade);   // 进阶丹返还活动
		role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_TOUSHI_GRADE);//嘉年华
	}
}

void EventHandler::OnQiLinBiUpgrade(Role *role, int old_grade, int to_grade)
{
	if (NULL == role) return;

	// 	role->GetRoleActivity()->OnQiLinBiUpgrade(old_grade, to_grade);
	// 
	 	if (to_grade > old_grade && to_grade > 0)
	 	{
	 		/*role->GetRoleActivity()->SendOpenGameActivityInfo();*/
	 
			WorldStatus2::Instance().IncTotalUpgradeQilinbiRecordList(to_grade);
			JinjiesysFuncHelper::Instance().OnUpgrade(role, JINJIE_TYPE_QILINBI, old_grade, to_grade);
			role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_QILINBI, to_grade,old_grade);   // 进阶丹返还活动
			role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_QILINGBI_GRADE);//嘉年华
	 	}
}

void EventHandler::OnMaskUpgrade(Role *role, int old_grade, int to_grade)
{
	if (NULL == role) return;

	// 	role->GetRoleActivity()->OnMaskUpgrade(old_grade, to_grade);
	// 
	 	if (to_grade > old_grade && to_grade > 0)
	 	{
	 		/*role->GetRoleActivity()->SendOpenGameActivityInfo();
	 
	 		if (to_grade < MAX_UPGRADE_RECORD_COUNT)
	 		{
	 			WorldStatus::Instance().GetCommonData().total_upgrade_mask_record_list[to_grade]++;
	 		}*/
			JinjiesysFuncHelper::Instance().OnUpgrade(role, JINJIE_TYPE_MASK, old_grade, to_grade);
			role->GetRoleActivity()->OnRaJinJieOpera(JINJIE_ACT_THEME_TYPE_MASK, to_grade,old_grade);   // 进阶丹返还活动
			role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_MASK_GRADE);//嘉年华
	 	}
}

void EventHandler::OnShengqiStrengthLevel(Role *role)
{
	if (nullptr == role) return;

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_SHENGQI_STRENGTH_LEVEL);
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_SHENGQI_LEVEL_COUNT);
}

void EventHandler::OnChineseZodiacUpLevel(Role *role)
{
	if (nullptr == role) return;

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_CHINESE_ZODIAC_LEVEL);
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_CHINESE_ZODIAC_LEVEL_COUNT);
	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_CHINESE_ZODIAC_ACTIVE_COUNT);
}

void EventHandler::OnSealInlay(Role *role)
{
	if (nullptr == role) return;

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_SEAL_INLAY_COUNT);
}

void EventHandler::OnPassXinMoFb(Role *role, int layer)
{
	if (nullptr == role)
	{
		return;
	}
	role->GetTaskManager()->OnPassFBLayer(FBManager::FBCT_XINMO_FB, layer);
	role->GetTaskManager()->OnCheckSatisfyStatus(FBManager::FBCT_XINMO_FB);
}

void EventHandler::OnEnterFBHolidayGuardPerson(Role *role, bool is_first)
{
	if (nullptr == role) return;
}

void EventHandler::OnDecShengwuEssence(Role *role, int consume_number)
{
	if (nullptr == role || 0 >= consume_number) return;

}

void EventHandler::OnAddShengwuEssence(Role *role, int add_number)
{
	if (nullptr == role || 0 >= add_number) return;

}

void EventHandler::OnActiveGreateSoldier(Role *role, int seq)
{
	if (nullptr == role) return;

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_GREATESOLDIER_ACTIVE);//嘉年华

	//role->GetFamousMan()->OnCheckFamousMan(FAMOUS_MAN_ACTIVE_GREATE_SOLDIER, seq);
}

void EventHandler::OnUpPotentialGreateSoldier(Role *role, int seq)
{
	if (nullptr == role) return;

	role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_GREATESOLDIER_UPLEVEL);//嘉年华
}

void EventHandler::OnGreateSoldierTenDraw(Role *role)
{
	if (nullptr == role) return;

	//role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEFREE_TYPE_GREATE_SOLDIER_TEN_DRAM);
}

void EventHandler::OnAddGuildRumorEvent(GuildID guild_id, const char *name_1, const char *name_2, int event_type, int param0, int param1, int param2, int param3)
{
	Guild *guild = GuildManager::Instance().GetGuild(guild_id);

	if (NULL != guild)
	{
		guild->GetGuildEventManager().OnRumorEvent(name_1, name_2, event_type, param0, param1, param2, param3);
	}
}

void EventHandler::OnGetSceneActivityReward(Role *role, int activity_id, int reward_id, int reward_type, int param)
{
	Protocol::SCSceneActivityRewardInfo info;

	info.activity_id = activity_id;
	info.reward_id = reward_id;
	info.reward_type = reward_type;
	info.param = param;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&info, sizeof(info));
}

void EventHandler::OnBroadCastCrossRankTop3ToGuild(int cross_rank_type, int param1, int param2, long long rank_value, int rank_pos)
{
	switch (cross_rank_type)
	{
	case CROSS_PERSON_RANK_TYPE::CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY:
	{
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(param1);
		if (NULL != user_node)
		{
			if (user_node->guild_id != INVALID_GUILD_ID)
			{
				GameName name;
				user_node->GetName(name);

				if (name != NULL)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSChannelChat), sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSChannelChat), 
						gamestring::g_add_capability_rank_top3, user_node->uid, name, user_node->camp, static_cast<int>(rank_value), rank_pos);
					if (length > 0)
					{
						Guild *guild = GuildManager::Instance().GetGuild(user_node->guild_id);
						if (NULL != guild)
						{
							//guild->GuildSystemMsg(length, gamestring::GAMESTRING_BUF, 0, NULL, SYS_MSG_ONLY_CHAT_GUILD);
							Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)(gamestring::GAMESTRING_BUF);
							cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
							cc->msg_length = length;

							guild->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat));
						}
					}
				}
			}
		}
	}
	break;

	case CROSS_PERSON_RANK_TYPE::CROSS_PERSON_RANK_TYPE_ADD_CHARM:
	{
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(param1);
		if (NULL != user_node)
		{
			if (user_node->guild_id != INVALID_GUILD_ID)
			{
				GameName name;
				user_node->GetName(name);

				if (name != NULL)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSChannelChat), sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSChannelChat),
						gamestring::g_add_charm_rank_top3, user_node->uid, name, user_node->camp, static_cast<int>(rank_value), rank_pos);
					if (length > 0)
					{
						Guild *guild = GuildManager::Instance().GetGuild(user_node->guild_id);
						if (NULL != guild)
						{
							//guild->GuildSystemMsg(length, gamestring::GAMESTRING_BUF, 0, NULL, SYS_MSG_ONLY_CHAT_GUILD);
							Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)(gamestring::GAMESTRING_BUF);
							cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
							cc->msg_length = length;

							guild->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat));
						}
					}
				}
			}
		}
	}
	break;

	case CROSS_COUPLE_RANK_TYPE_BEGIN + CROSS_COUPLE_RANK_TYPE::CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP:
	{
		UserCacheNode *user_node_1 = UserCacheManager::Instance().GetUserNode(param1);
		UserCacheNode *user_node_2 = UserCacheManager::Instance().GetUserNode(param2);

		if (user_node_1 == NULL || user_node_2 == NULL)
		{
			return;
		}

		GameName name_1;
		user_node_1->GetName(name_1);

		GameName name_2;
		user_node_2->GetName(name_2);

		if (name_1 != NULL && name_2 != NULL)
		{
			if (user_node_1->guild_id != user_node_2->guild_id)
			{
				if (user_node_1->guild_id != INVALID_GUILD_ID)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSChannelChat), sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSChannelChat), 
						gamestring::g_qingyuan_rank_top3, user_node_1->uid, name_1, user_node_1->camp,
						user_node_2->uid, name_2, user_node_2->camp, static_cast<int>(rank_value), rank_pos);
					if (length > 0)
					{
						Guild *guild = GuildManager::Instance().GetGuild(user_node_1->guild_id);
						if (NULL != guild)
						{
							//guild->GuildSystemMsg(length, gamestring::GAMESTRING_BUF, 0, NULL, SYS_MSG_ONLY_CHAT_GUILD);
							Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)(gamestring::GAMESTRING_BUF);
							cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
							cc->msg_length = length;

							guild->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat));
						}
					}
				}

				if (user_node_2->guild_id != INVALID_GUILD_ID)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSChannelChat), sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSChannelChat), 
						gamestring::g_qingyuan_rank_top3, user_node_1->uid, name_1, user_node_1->camp,
						user_node_2->uid, name_2, user_node_2->camp, static_cast<int>(rank_value), rank_pos);
					if (length > 0)
					{
						Guild *guild = GuildManager::Instance().GetGuild(user_node_2->guild_id);
						if (NULL != guild)
						{
							//guild->GuildSystemMsg(length, gamestring::GAMESTRING_BUF, 0, NULL, SYS_MSG_ONLY_CHAT_GUILD);
							Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)(gamestring::GAMESTRING_BUF);
							cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
							cc->msg_length = length;

							guild->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat));
						}
					}
				}
			}
			else if (user_node_1->guild_id != INVALID_GUILD_ID)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSChannelChat), sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSChannelChat), 
					gamestring::g_qingyuan_rank_top3, user_node_1->uid, name_1, user_node_1->camp,
					user_node_2->uid, name_2, user_node_2->camp, static_cast<int>(rank_value), rank_pos);
				if (length > 0)
				{
					Guild *guild = GuildManager::Instance().GetGuild(user_node_1->guild_id);
					if (NULL != guild)
					{
						//guild->GuildSystemMsg(length, gamestring::GAMESTRING_BUF, 0, NULL, SYS_MSG_ONLY_CHAT_GUILD);
						Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)(gamestring::GAMESTRING_BUF);
						cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
						cc->msg_length = length;

						guild->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat));
					}
				}
			}
		}
	}
	break;

	default:
		break;
	}
}
