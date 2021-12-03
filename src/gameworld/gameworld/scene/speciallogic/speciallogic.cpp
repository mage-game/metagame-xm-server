#include "speciallogic.hpp"

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/character/roleshadow.hpp"
#include "obj/otherobj/fallingitem.h"

#include "effect/sceneeffect/sceneeffectbase.hpp"
#include "effect/sceneeffect/sceneskillpool.hpp"

#include "protocal/msgscene.h"
#include "protocal/msgfb.h"

#include "engineadapter.h"

#include "world.h"
#include "monster/monsterpool.h"
#include "config/logicconfigmanager.hpp"

#include "scene/speciallogic/guildspecial/specialguildstation.hpp"
#include "scene/speciallogic/guildspecial/specialguildboss.hpp"

#include "scene/speciallogic/worldspecial/specialzhuxie.hpp"
#include "scene/speciallogic/worldspecial/specialshuijing.hpp"
#include "scene/speciallogic/worldspecial/specialqunxianluandou.hpp"
#include "scene/speciallogic/worldspecial/specialgongchengzhan.hpp"
#include "scene/speciallogic/worldspecial/specialxianmengzhan.hpp"
#include "scene/speciallogic/worldspecial/specialhunyan.hpp"
#include "scene/speciallogic/personspecial/specialshenmozhixi.hpp"
#include "scene/speciallogic/personspecial/specialexpfb.hpp"
#include "scene/speciallogic/personspecial/specialcoinfb.hpp"
#include "scene/speciallogic/guildspecial/specialguildmonster.hpp"
#include "scene/speciallogic/personspecial/specialstory.hpp"
#include "scene/speciallogic/personspecial/specialchallenge.hpp"
#include "scene/speciallogic/personspecial/specialphasefb.hpp"
#include "scene/speciallogic/other/specialchallengefield.hpp"
#include "scene/speciallogic/other/specialcrosslocalchallengefield.hpp"
#include "scene/speciallogic/teamspecial/specialyaoshoujitanteamfb.hpp"
#include "scene/speciallogic/personspecial/specialloversfb.hpp"
#include "scene/speciallogic/personspecial/specialzhanshendian.hpp"
#include "scene/speciallogic/worldspecial/specialnationalboss.hpp"
#include "scene/speciallogic/worldspecial/specialyizhandaodi.hpp"
#include "scene/speciallogic/teamspecial/specialmigongxianfuteamfb.hpp"
#include "scene/speciallogic/personspecial/specialvipfb.hpp"
#include "scene/speciallogic/worldspecial/specialguildbattlenew.hpp"
#include "scene/speciallogic/worldspecial/specialterritorywar.hpp"
#include "scene/speciallogic/personspecial/specialmojiefb.hpp"
#include "scene/speciallogic/worldspecial/specialmillionaire.hpp"
#include "scene/guildshadow/guildshadow.hpp"

#include "globalconfig/globalconfig.h"
#include "gameworld/scene/speciallogic/personspecial/specialtowerdefend.hpp"
#include "gameworld/scene/speciallogic/guildspecial/specialguildfb.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialwuxingguaji.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialpersonalzhuanzhi.hpp"
#include "gameworld/scene/speciallogic/personspecial/specialwushuangfb.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialcrossxiuluotower.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialcross1v1.hpp"
#include "gameworld/scene/speciallogic/personspecial/specialpatafb.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialcrossmultiuserchallenge.hpp"

#include "gameworld/scene/speciallogic/worldspecial/specialyaoshouguangchang.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialsuoyaota.hpp"
#include "gameworld/scene/speciallogic/teamspecial/specialteamzhuagui.hpp"
#include "gameworld/scene/speciallogic/personspecial/specialcampgaojiduobao.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialcrosstuanzhan.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialcrosspasture.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialcrossboss.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialcrosshotspring.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialtianjiangcaibao.h"
#include "gameworld/scene/speciallogic/worldspecial/specialwanglingexplore.h"
#include "gameworld/scene/speciallogic/worldspecial/specialcrossteamfb.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialcrossshuijing.hpp"

#include "gameworld/scene/speciallogic/personspecial/specialfunopenwing.hpp"
#include "gameworld/scene/speciallogic/personspecial/specialfunopenmountfb.hpp"
#include "gameworld/scene/speciallogic/personspecial/specialfunopenxiannv.hpp"
#include "gameworld/scene/speciallogic/personspecial/specialfbguide.hpp"
#include "gameworld/scene/speciallogic/personspecial/specialrunetower.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialteamequipfb.hpp"
#include "gameworld/scene/speciallogic/personspecial/specialdailytaskfb.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialxingzuoyiji.hpp"
#include "gameworld/scene/speciallogic/personspecial/specialtuitufb.h"
#include "gameworld/scene/speciallogic/worldspecial/specialqingyuanshengdi.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialcrossguildbattle.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialcombineserverboss.hpp"
#include "gameworld/scene/speciallogic/personspecial/specialnewequip.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialcrossdarknight.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialbabyboss.hpp"
#include "gameworld/scene/speciallogic/personspecial/specialarmordefend.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialcrossfishing.hpp"
#include "gameworld/scene/speciallogic/doublesidefbspecial/specialnightfight.hpp"
#include "gameworld/scene/speciallogic/personspecial/specialbuildtower.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialguildquestion.hpp"
#include "gameworld/scene/speciallogic/doublesidefbspecial/specialmessbattle.hpp"
#include "gameworld/scene/speciallogic/teamspecial/specialtowerdefendteam.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialcrossliekunfb.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialgodmagicboss.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialgiftharvest.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialcorssbianjingzhidi.h"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "other/route/mailroute.hpp"
#include "personspecial/specialfighting.h"
#include "item/knapsack.h"
#include "global/bossfamily/bossfamilymanager.hpp"
#include "worldspecial/specialshangguboss.hpp"
#include "personspecial/specialpersonboss.hpp"
#include "personspecial/specialxinmofb.hpp"
#include "teamspecial/specialequipteam.hpp"
#include "personspecial/specialholidayguard.hpp"
#include "worldspecial/specialcrossmizangboss.hpp"
#include "worldspecial/specialcrossyoumingboss.hpp"
#include "worldspecial/specialcrosshusongshuijing.hpp"
#include "personspecial/specialtowerdefendtest.hpp"

SpecialLogic * SpecialLogic::CreateSpecialLogic(Scene *scene, int speciallogic_type)
{
	UNSTD_STATIC_CHECK(88 == Scene::SCENE_TYPE_COUNT);

	switch (speciallogic_type)
	{
	case Scene::SCENE_TYPE_COMMON_SCENE:
		{
			// 去掉阵营驻地代码 -- 导致部分普通场景仙盟篝火不会被打断
			//int camp_type = GLOBALCONFIG->GetCampTypeOfScene(scene->GetSceneID());
			//if (CAMP_TYPE_INVALID < camp_type && camp_type < CAMP_TYPE_MAX)
			//{
			//	return new SpecialCampStation(scene, camp_type);
			//}

			return new SpecialLogic(scene);
		}
		break;

	case Scene::SCENE_TYPE_GUILD_STATION:
		{
			return new SpecialGuildStation(scene);
		}
		break;

	case Scene::SCENE_TYPE_ZHUXIE:
		{
			return new SpecialZhuXie(scene);
		}
		break;

	case Scene::SCENE_TYPE_SHUIJING:
		{
			return new SpecialShuiJing(scene);
		}
		break;

	case Scene::SCENE_TYPE_COINFB:
		{
			return new SpecialCoinFB(scene);
		}
		break;

	case Scene::SCENE_TYPE_EXPFB:
		{
			return new SpecialExpFB(scene);
		}
		break;

	case Scene::SCENE_TYPE_PHASE_FB:
		{
			return new SpecialPhaseFB(scene);
		}
		break;

	case Scene::SCENE_TYPE_CHALLENGE_FB:
		{
			return new SpecialChallenge(scene);
		}
		break;

	case Scene::SCENE_TYPE_QUNXIANLUANDOU:
		{
			return new SpecialQunXianLuanDou(scene);
		}
		break;

	case Scene::SCENE_TYPE_PERSONAL_TOWERDEFEND:
		{
			return new SpecialTowerDefend(scene);
		}
		break;

	case Scene::SCENE_TYPE_XIANMENGZHAN:
		{
			return new SpecialXianMengZhan(scene);
		}
		break;

	case Scene::SCENE_TYPE_GONGCHENGZHAN:
		{
			return new SpecialGongChengZhan(scene);
		}
		break;

	case Scene::SCENE_TYPE_HUN_YAN_FB:
		{
			return new SpecialHunyan(scene);
		}
		break;

	case Scene::SCENE_TYPE_NATIONAL_BOSS:
		{
			return new SpecialNationalBoss(scene);
		}
		break;

	case Scene::SCENE_TYPE_GUILD_MONSTER:
		{
			return new SpecialGuildMonster(scene);
		}
		break;

	case Scene::SCENE_TYPE_CHALLENGE_FIELD:
		{
			return new SpecialChallengeField(scene);
		}
		break;

	case Scene::SCENE_TYPE_STORY_FB:
		{
			return new SpecialStoryFB(scene);
		}
		break;

	case Scene::SCENE_TYPE_TAOSHOUJITAN_TEAM_FB:
		{
			return new SpecialYaoShouJiTanTeamFb(scene);
		}
		break;

	case Scene::SCENE_TYPE_LOVERS_FB:
		{
			return new SpecialLoversFB(scene);
		}
		break;

	case Scene::SCENE_TYPE_ZHANSHENDIAN_FB:
		{
			return new SpecialZhanShenDian(scene);
		}
		break;

	case Scene::SCENE_TYPE_SHENMOZHIXI_FB:
		{
			return new SpecialShenmozhixi(scene);
		}
		break;

	case Scene::SCENE_TYPE_YIZHANDAODI:
		{
			return new SpecialYiZhanDaoDi(scene);
		}
		break;

	case Scene::SCENE_TYPE_CROSS_GUILDBATTLE:
	{
		return new SpecialCrossGuildBattle(scene);
	}
	break;

	case Scene::SCENE_TYPE_GUILD_FB:
		{
			return new SpecialGuildFb(scene);
		}
		break;

	case Scene::SCENE_TYPE_WUXINGGUAJI:
		{
			return new SpecialWuXingGuaJi(scene);
		}
		break;

	case Scene::SCENE_TYPE_ZHUANZHI_PERSONAL:
		{
			return new SpecialPersonalZhuanzhi(scene);
		}
		break;

	case Scene::SCENE_TYPE_MIGONGXIANFU_TEAM:
		{
			return new SpecialMiGongXianFuTeamFb(scene);
		}
		break;

	case Scene::SCENE_TYPE_WUSHUANG_FB:
		{
			return new SpecialWushuangFB(scene);
		}
		break;

	case Scene::SCENE_TYPE_CROSS_XIULUO_TOWER:
		{
			return new SpecialCrossXiuluoTower(scene);
		}
		break;

	case Scene::SCENE_TYPE_CROSS_1V1:
		{
			return new SpecialCross1v1(scene);
		}
		break;

	case Scene::SCENE_TYPE_PATA_FB:
		{
			return new SpecialPataFb(scene);
		}
		break;

	case Scene::SCENE_TYPE_CROSS_MULTIUSER_CHALLENGE:
		{
			return new SpecialCrossMultiuserChallenge(scene);
		}
		break;
	
	case Scene::SCENE_TYPE_GUILD_BOSS:
		{
			return new SpecialGuildBoss(scene);
		}
		break;

	case Scene::SCENE_TYPE_YAOSHOU_GUANGCHANG:
		{
			return new SpecialYaoShouGuangChang(scene);
		}
		break;

	case Scene::SCENE_TYPE_SUOYAOTA:
		{
			return new SpecialSuoYaoTa(scene);
		}
		break;

	case Scene::SCENE_TYPE_TEAM_ZHUAGUI:
		{
			return new SpecialTeamZhuaGui(scene);
		}
		break;

	case Scene::SCENE_TYPE_CAMPGAOJI_DUOBAO:
		{
			return new SpecialCampGaojiDuoBao(scene);
		}
		break;

	case Scene::SCENE_TYPE_CROSS_TUANZHAN:
		{
			return new SpecialCrossTuanzhan(scene);
		}
		break;

	case Scene::SCENE_TYPE_CROSS_PASTURE:
		{
			return new SpecialCrossPasture(scene);
		}
		break;

	case Scene::SCENE_TYPE_VIP_FB:
		{
			return new SpecialVipFb(scene);
		}
		break;

	case Scene::SCENE_TYPE_GUILDBATTLE:
		{
			return new SpecialGuildBattleNew(scene);
		}
		break;

	case Scene::SCENE_TYPE_TIANJIANGCAIBAO:
		{
			return new SpecialTianJiangCaiBao(scene);
		}
		break;

	case Scene::SCENE_TYPE_CROSS_BOSS:
		{
			return new SpecialCrossBoss(scene);
		}
		break;

	case Scene::SCENE_TYPE_HOTSPRING:
		{
			return new SpecialCrossHotspring(scene);
		}
		break;

	case Scene::SCENE_TYPE_WANGLINGEXPLORE:
		{
			return new SpecialWangLingExplore(scene);
		}
		break;

	case Scene::SCENE_TYPE_CROSS_TEAM_FB:
		{
			return new SpecialCrossTeamFB(scene);
		}
		break;

	case Scene::SCENE_TYPE_TERRITORY_WAR:
		{
			return new SpecialTerritoryWar(scene);
		}
		break;
	case Scene::SCENE_TYPE_MO_JIE:
		{
			return new SpecialMoJieFB(scene);
		}
		break;

	case Scene::SCENE_TYPE_FUN_TYPE_MOUNT:
		{
			return new SpecialFunOpenMountFb(scene);
		}
		break;

	case Scene::SCENE_TYPE_FUN_TYPE_WING:
		{
			return new SpecialFunOpenWingFb(scene);
		}
		break;

	case Scene::SCENE_TYPE_FUN_TYPE_XIANNV:
		{
			return new SpecialFunOpenXiannvFb(scene);
		}
		break;

	case Scene::SCENE_TYPE_CROSS_SHUIJING:
		{
			return new SpecialCrossShuiJing(scene);
		}
		break;

	case Scene::SCENE_TYPE_FB_GUIDE:
		{
			return new SpecialFbGuide(scene);
		}
		break;

	case Scene::SCENE_TYPE_RUNE_TOWER:
		{
			return new SpecialRuneTower(scene);
		}
		break;
	
	case Scene::SCENE_TYPE_TEAM_EQUIP_FB:
		{
			return new SpecialTeamEquipFB(scene);
		}
		break;

	case Scene::SCENE_TYPE_MILLIONAIRE:
		{
			return new SpecialMillionaire(scene);
		}
		break;

	case Scene::SCENE_TYPE_DAILY_TASK_FB:
		{
			return new SpecialDailyTaskFb(scene);
		}
		break;

	//case Scene::SCENE_TYPE_XINGZUOYIJI:
	//	{
	//		return new SpecialXingzuoYiji(scene);
	//	}
	//	break;

	case Scene::SCENE_TYPE_TUITU_FB:
		{
			return new SpecialTuitu(scene);
		}
		break;
		
	case Scene::SCENE_TYPE_FIGHTING:
		{
			return new SpecialFighting(scene);
		}
		break;

	case Scene::SCENE_TYPE_QINYUAN_SHENGDI:
		{
			return new SpecialQingYuanShengDi(scene);
		}
		break;

	case Scene::SCENE_TYPE_CSA_BOSS:
		{
			return new SpecialCombineServerBoss(scene);
		}
		break;

	case Scene::SCENE_TYPE_PERSONAL_EQUIPFB:
		{
			return new SpecialNewEquip(scene);
		}
		break;

	case Scene::SCENE_TYPE_CROSS_DARK_NIGHT:
		{
			return new SpecialCrossDarkNight(scene);
		}
		break;

	case Scene::SCENE_TYPE_BABY_BOSS:
		{
			return new SpecialBabyBoss(scene);
		}
		break;

	case Scene::SCENE_TYPE_SHANGGU_BOSS:
		{
			return new SpecialShangGuBoss(scene);
		}
		break;


	case Scene::SCENE_TYPE_ARMOR_DEFEND:
		{
			return new SpecialArmorDefend(scene);
		}
		break;

	case Scene::SCENE_TYPE_FISHING:
		{
			return new SpecialCrossFishing(scene);
		}
		break;

	case Scene::SCENE_TYPE_MESS_BATTLE_FB:
		{
			return new SpecialMessBattle(scene);
		}
		break;

	case Scene::SCENE_TYPE_BUILD_TOWER:
		{
			return new SpecialBuildTower(scene);
		}
		break;

	case Scene::SCENE_TYPE_NIGHT_FIGHT_FB:
		{
			return new SpecialNightFight(scene);
		}
		break;

	case Scene::SCENE_TYPE_PERSON_BOSS:
		{
			return new SpecialPersonBoss(scene);
		}
		break;

	case Scene::SCENE_TYPE_GUILD_QUESTION:
		{
			return new SpecialGuildQuestion(scene);
		}
		break;

	case Scene::SCENE_TYPE_XINMO_FB:
		{
			return new SpecialXinMoFb(scene);
		}
		break;

	case Scene::SCENE_TYPE_TEAM_TOWERDEFEND:
		{
			return new SpecialTeamTowerDefend(scene);
		}
		break;

	case Scene::SCENE_TYPE_CROSS_LIEKUN_FB:
		{
			return new SpecialCrossLieKunFb(scene);
		}
		break;

	case Scene::SCENE_TYPE_TEAM_EQUIPFB:
		{
			return new SpecialEquipTeam(scene);
		}
		break;

	case Scene::SCENE_TYPE_HOLIDAY_GUARD_FB:
		{
			return new SpecialHolidayGuard(scene);
		}
		break;

	case Scene::SCENE_TYPE_CROSS_MIZANG_BOSS: 
	{
		return new SpecialCrossMiZangBoss(scene);
	}
	break;

	case Scene::SCENE_TYPE_CROSS_YOUMING_BOSS:
	{
		return new SpecialCrossYouMingBoss(scene);
	}
	break;

	case Scene::SCENE_TYPE_GODMAGIC_BOSS:
	{
		return new SpecialGodmagicBoss(scene);
	}
	break;

	case Scene::SCENE_TYPE_GIFT_HARVEST:
	{
		return new SpecialGiftHarvest(scene);
	}
	break;

	case Scene::SCENE_TYPE_CROSS_BIANJINGZHIDI:
	{
		return new SpecialCrossBianJingZhiDi(scene);
	}
	break;

	case Scene::SCENE_TYPE_CROSS_CHALLENGE_FIELD:
	{
		return new SpecialCrossLocalChallengeField(scene);
	}
	break;

	case Scene::SCENE_TYPE_CROSS_HUSONG_SHUIJING:
	{
		return new SpecialCrossHusongShuijing(scene);
	}
	break;

	case Scene::SCENE_TYPE_TOWERDEFEND_TEST:
	{
		return new SpecialTowerDefendTest(scene);
	}
	break;

	default:

		return new SpecialLogic(scene);
	}

	return new SpecialLogic(scene);
}

void SpecialLogic::DestorySpecialLogic(SpecialLogic *speciallogic)
{
	delete speciallogic;
}

SpecialLogic::SpecialLogic(Scene *scene) 
	: m_scene(scene),  m_last_check_monster_status_time(0)
{

}

SpecialLogic::~SpecialLogic()
{
	for (SceneEffectVectorIt it = m_scene_effect_vec.begin(), end = m_scene_effect_vec.end(); it != end; ++ it)
	{
		delete *it;
	}

	m_scene_effect_vec.clear();
}

void SpecialLogic::OnAttackCalcHurt(Character *cha, ObjID enemy_id, Attribute &delta_hp)
{

}

void SpecialLogic::OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp)
{
}

void SpecialLogic::OnHurtOther(Character *cha, Character *target)
{

}

void SpecialLogic::OnCharacterDie(Character *dead, ObjID killer_id)
{
}

void SpecialLogic::OnRoleEnterScene(Role *role)
{
}

bool SpecialLogic::SpecialIsEnemy(Role *role, Obj *obj)
{
	switch (obj->GetObjType())	
	{
	case Obj::OBJ_TYPE_MONSTER:
		{
			Monster *monster = (Monster*)obj;

			// 仙盟镖车
			if (!GuildShadow::Instance().IsCanAttackBiaoChe(role, monster))
			{
				return false;
			}

			return monster->IsAlive();
		}
		break;

	case Obj::OBJ_TYPE_ROLE:
		{
			Role *target_role = (Role*)obj;

			if (target_role != role && Scene::IsPerciousBossScene(m_scene->GetSceneID()) 
				&& (BossFamilyManager::Instance().IsInForbidPkArea(role) || BossFamilyManager::Instance().IsInForbidPkArea(target_role)))
			{
				return false;
			}

			return (target_role->IsAlive() && role->RoleIsEnemy(target_role));
		}
		break;

	case Obj::OBJ_TYPE_ROLE_SHADOW:
		{
			RoleShadow *target_role_shadow = (RoleShadow*)obj;
			return target_role_shadow->IsAlive() && role->RoleShadowIsEnemy(target_role_shadow);
		}
		break;

	case Obj::OBJ_TYPE_MARRY:
		{
			return false;
		}
		break;
	}

	return false;

}

bool SpecialLogic::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
{
	switch (obj->GetObjType())
	{
	case Obj::OBJ_TYPE_MONSTER:
		{
			return false;
		}
		break;

	case Obj::OBJ_TYPE_ROLE:
		{
			Role *friend_role = (Role *)obj;
			return (!check_alive || friend_role->IsAlive()) && role->RoleIsFriend(friend_role);
		}
		break;

	}

	return false;
}

int SpecialLogic::GetObjSide(ObjID id)
{
	return -1;
}

void SpecialLogic::AddSceneEffect(SceneEffectBase *scene_effect)
{
	if (NULL != scene_effect)
	{
		m_scene_effect_vec.push_back(scene_effect);
	}
}

void SpecialLogic::DelSceneEffect(ObjID deliver_objid, int product_id, int product_method)
{
	for (SceneEffectVectorIt it = m_scene_effect_vec.begin(); it != m_scene_effect_vec.end();)
	{
		SceneEffectBase *scene_effect = *it;

		if (scene_effect->GetDeliverObjid() == deliver_objid && scene_effect->GetProductid() == product_id &&
			scene_effect->GetProductMethod() == product_method)
		{
			scene_effect->DelEffectObj();
			delete scene_effect;
			it = m_scene_effect_vec.erase(it);
		}
		else
		{
			++ it;
		}
	}
}

bool SpecialLogic::IsAllMonsterDead()
{
	if (m_scene->HasHiddenMonster())
	{
		return false;
	}
	UInt32 monster_num = m_scene->MonsterNum();
	if (monster_num <= 0) return true;

	for (int i = 0; i < (int)monster_num; i++)
	{
		Monster *monster = m_scene->GetMonsterByIndex(i);
		if (NULL != monster && Obj::OBJ_TYPE_MONSTER == monster->GetObjType())
		{
			if (monster->IsAlive())
			{
				return false;
			}
		}
	}

	return true;
}

void SpecialLogic::CheckSceneMonsterStatus(unsigned long interval, time_t now_second)
{
	if (now_second >= m_last_check_monster_status_time + 1)
	{
		m_last_check_monster_status_time = static_cast<unsigned int>(now_second);

		if (this->IsAllMonsterDead()) this->OnAllMonsterDead();
	}
}

void SpecialLogic::UpdateSceneEffectList(unsigned long interval, time_t now_second)
{
	for (SceneEffectVectorIt it = m_scene_effect_vec.begin(); it != m_scene_effect_vec.end(); )
	{
		SceneEffectBase *scene_effect = *it;

		if (!scene_effect->Effect(interval, now_second))
		{
			delete scene_effect;
			it = m_scene_effect_vec.erase(it);
		}
		else 
		{
			++ it;
		}
	}
}

void SpecialLogic::DelayKickOutRole(Role *role, int reason)
{
	if (NULL == role) return;
	
	Posi target_pos(0, 0);
	int target_scene_id = role->GetLastScene(&target_pos);
	ChangeSceneDelayer *timer = new ChangeSceneDelayer(World::GetInstWorld()->GetSceneManager(), m_scene->GetIndex(), role->GetId(), 
		role->GetUserId(), target_scene_id, 0, target_pos, reason);

	EngineAdapter::Instance().CreateTimer(1, timer);
}

void SpecialLogic::DelayKickOutAllRole(int reason)
{
	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			this->DelayKickOutRole(role, reason);
		}
	}
}

void SpecialLogic::AllRoleReLive()
{
	for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
	{
		Role *role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			role->Recover(true, true, true);
		}
	}
}

void SpecialLogic::TriggerFBPass()
{
	if (!m_scene->InStaticScene()) 
	{
		int role_num = static_cast<int>(m_scene->RoleNum());
		for (int i = 0; i < role_num; ++ i)
		{
			Role *role = m_scene->GetRoleByIndex(i);
			if (NULL != role && Obj::OBJ_TYPE_ROLE == role->GetObjType())
			{
				m_scene->OnTrigger(TRIGGER_COND_TYPE_FB_PASS, role);
			}
		}
	}
}

void SpecialLogic::NotifyBossHPPercent(unsigned short monster_id)
{
	if (!MONSTERPOOL->IsMonsterExist(monster_id))
	{
		return;
	}

	long long monster_left_hp = 0, monster_max_hp = 0;
	UInt32 monster_num = m_scene->MonsterNum();
	for (UInt32 i = 0;i < monster_num; ++ i)
	{
		Monster *monster = m_scene->GetMonsterByIndex(i);
		if (NULL != monster && monster->GetMonsterId() == monster_id)
		{
			monster_left_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			monster_max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			break;
		}
	}

	Protocol::SCFBBossHPPercentInfo pofn;
	pofn.monster_id = monster_id;

	if (monster_left_hp > 0 && monster_max_hp >0)
	{
		pofn.hp_percent = static_cast<unsigned short>(monster_left_hp * 1.0 / monster_max_hp * 10000);		// 计算血量剩余的万分比
	}
	else
	{
		pofn.hp_percent = 0;
	}

	if (pofn.hp_percent > 10000) 
	{
		pofn.hp_percent = 10000;
	}

	m_scene->SendToRole((const char *)&pofn, sizeof(pofn));
}

bool SpecialLogic::SpecialRelive(Role *role, int realive_type)
{
	// 新项目木有阵营复活，世界boss复活就走外面正常流程

	// 这是在其他阵营回城复活，元宝复活由上层调用的地方处理。将来如果增加阵营主城的speciallogic，这个逻辑搬走
	//if (GLOBALCONFIG->IsOtherCampScene(role->GetCamp(), m_scene->GetSceneID()) &&
	//	Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	//{
	//	Posi realive_pos;
	//	int target_scene_id = GLOBALCONFIG->GetCenterCityRealivePos(realive_pos);
	//	
	//	if (role->ReAliveTargetScene(target_scene_id, realive_pos)) 
	//	{
	//		return true;
	//	}
	//}
	//else if (SceneRegister::Instance().IsWorldBossScene(m_scene->GetSceneID()))
	//{
	//	if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == realive_type)
	//	{
	//		int target_scene_id; Posi realive_pos;
	//		if (LOGIC_CONFIG->GetWorldBossConfig().GetRealivePos(m_scene->GetSceneID(), &target_scene_id, &realive_pos))
	//		{
	//			if (role->ReAliveTargetScene(target_scene_id, realive_pos)) 
	//			{
	//				return true;
	//			}
	//		}
	//	}
	//}

	return false;
}

void SpecialLogic::OnSpecialGuWu(Role *role, bool is_gold)
{
	if (!this->SpecialCanGuWu() || NULL == role) return;

	int cost_coin = GLOBALCONFIG->GetGuWuCostCoin();
	int cost_gold = GLOBALCONFIG->GetGuWuCostGold();

	if (cost_coin <= 0 || cost_gold <= 0) return;
	
	if (is_gold && !role->GetKnapsack()->GetMoney()->GoldMoreThan(cost_gold))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!is_gold && !role->GetKnapsack()->GetMoney()->AllCoinMoreThan(cost_coin))
	{
		role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
		return;
	}

	if (is_gold)
	{
		role->GetKnapsack()->GetMoney()->UseGold(cost_gold, "FbGuWuCost");
	}
	else
	{
		role->GetKnapsack()->GetMoney()->UseAllCoin(cost_coin, "FbGuWuCost");
	}

	Attribute add_gongji = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * GLOBALCONFIG->GetGuWuGongPer() / 100;

	EffectBuff *effect = new EffectBuff(role->GetId(), 0, GLOBALCONFIG->GetGuWuTime() * 1000, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
	effect->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, add_gongji, GLOBALCONFIG->GetGuWuGongPer());
	effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	effect->SetClientEffectType(EffectBase::ECT_OTHER_FBGF);
	effect->SetSave(false);
	role->AddEffect(effect);

	role->NoticeNum(noticenum::NT_FBGUWU_SUCC);
}

void SpecialLogic::KillAllMonster()
{
	if (NULL == m_scene) return;
	UInt32 monster_num = m_scene->MonsterNum();
	for (UInt32 i = 0; i < monster_num; ++i)
	{
		Monster *monster = m_scene->GetMonsterByIndex(i);
		if (NULL != monster)
		{
			monster->ForceSetDead();
		}
	}
}

int SpecialLogic::GetPassTime()
{
	int pass_time_s = static_cast<unsigned int>(EngineAdapter::Instance().Time() - m_scene->GetCreateTime());
	if (pass_time_s < 0)
	{
		pass_time_s = 0;
	}

	if (m_scene->GetTimeOut() > 0 && pass_time_s > (int)m_scene->GetTimeOut())
	{
		pass_time_s = m_scene->GetTimeOut();
	}
	return pass_time_s;
}

short SpecialLogic::GetKillAllMonster()
{
	return m_scene->GetSceneFBDynInfo()->kill_monster + this->GetKillBoss();
}

short SpecialLogic::GetKillBoss()
{
	return m_scene->GetSceneFBDynInfo()->kill_boss;
}

void SpecialLogic::AutoPickFallingItem(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	static MailContentParam contentparam; contentparam.Reset();

	unsigned int falling_item_num = m_scene->FallingItemNum();
	for (unsigned int i = 0; i < falling_item_num; i++)
	{
		FallingItem *falling_item = m_scene->GetFallingItemByIndex(i);
		if (NULL != falling_item && !falling_item->IsBuffFalling() && FallingItem::PR_SUC == falling_item->Pick(role, true, &contentparam))
		{
			if (contentparam.item_count >= MAX_ATTACHMENT_ITEM_NUM || (contentparam.item_count > 0 && i == falling_item_num - 1))
			{
				int contenttxt_length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_knapsack_put_content);
				if (contenttxt_length > 0)
				{
					MailRoute::MailToRole(role, SYSTEM_MAIL_REASON_INVALID, contentparam);
					contentparam.Reset();
				}
			}
		}
	}
}