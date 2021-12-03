#include "scene/scenemanager.h"

#include <set>

#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "global/cross/crossusershadow.hpp"
#include "global/cross/crossuserregister.hpp"
#include "world.h"
#include "obj/character/role.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "servercommon/serverconfig/crossxiuluotowercommonconfig.hpp"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "scene/scene.h"
#include "scene/speciallogic/worldspecial/specialcrossboss.hpp"
#include "other/queryhandler/queryhandler.hpp"
#include "global/cross/crossmanager.h"
#include "other/rank/crosspersonrankshadow.hpp"
#include "servercommon/serverconfig/cross1v1commonconfig.hpp"
#include "servercommon/errornum.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "speciallogic/worldspecial/specialcross1v1.hpp"
#include "speciallogic/worldspecial/specialcrossmultiuserchallenge.hpp"
#include "speciallogic/worldspecial/specialcrossguildbattle.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "global/cross/crossactivity/crossrecordmanager.hpp"
#include "servercommon/serverconfig/cross3v3config.hpp"
#include "other/cross/crossroute.hpp"
#include "global/usercache/usercache.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/crossteamfbconfig.hpp"
#include "config/crossrankweekrewardconfig.hpp"
#include "item/itempool.h"
#include "servercommon/serverconfig/crosstuanzhanconfig.hpp"
#include "servercommon/serverconfig/crosspastureconfig.hpp"
#include "servercommon/serverconfig/crossbossconfig.hpp"
#include "servercommon/serverconfig/crosshotspringconfig.hpp"
#include "servercommon/string/crossstr.h"
#include "servercommon/serverconfig/crossshuijingconfig.hpp"
#include "servercommon/serverconfig/crossguildbattleconfig.hpp"
#include "servercommon/serverconfig/crossdarknightconfig.hpp"
#include "servercommon/serverconfig/crossfishingconfig.hpp"
#include "servercommon/userprotocal/systemmsgcode.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "gameworld/internalcomm.h"
#include "gameworld/doublesidefb/doublesidefbmanager.hpp"
#include "other/cross/rolecross.h"
#include "other/fb/nightfight/nightfightfbconfig.hpp"
#include "other/fb/messbattle/messbattleconfig.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "servercommon/serverconfig/crossliekunfbconfig.hpp"
#include "global/guild/guildmanager.hpp"
#include "gameworld/gamecommon.h"
#include "global/guild/guild.hpp"
#include "scene/worldshadow/worldshadow.hpp"
#include "protocal/msgfb.h"
#include "global/worldstatus3/worldstatus3.hpp"
#include "global/bossfamily/bossfamilymanager.hpp"
#include "global/droplog/droplogmanager.hpp"
#include "servercommon/serverconfig/crossmizangbossconfig.hpp"
#include "gameworld/gameworld/chat/chatmanager.h"
#include "global/crossrandactivity/crossramanagerlocal.hpp"
#include "global/globalrole/globalrolemanager.hpp"
#include "global/globalrole/commondata/commondataglobal.hpp"
#include "global/challengefield/challengefield.hpp"
#include "global/crosschallengefield/crosschallengefield.hpp"

#include "servercommon/serverconfig/crossyoumingbossconfig.hpp"
#include "servercommon/serverconfig/crossbianjingzhidiconfig.h"
#include "other/achieve/title/titlemanager.hpp"
#include "other/event/eventhandler.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "servercommon/serverconfig/husongshuijingconfig.hpp"
#include "gameworld/scene/speciallogic/worldspecial/specialcrosshusongshuijing.hpp"


void SceneManager::InitCrossHandler()
{
	memset(m_on_cross_handler, 0, sizeof(m_on_cross_handler));

	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_SYNC_CROSS_USER - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncCrossUser;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_SAVE_CROSS_ROLE_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSaveCrossRoleInfo;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_START_CROSS_ACK - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnStartCrossAck;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_USER_MSG - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossUserMsg;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_ALL_USER_MSG - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossAllUserMsg;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_SYNC_CROSS_ACTIVITY_STATUS - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossSyncActivityStatus;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_XIULUOTA_GOTO_SCENE - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossXiuluoTowerGotoScene;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_SYNC_XIULUOTOWER_USER_RESULT - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncXiuluotaUserResult;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_SYNC_XIULUOTOWER_RANK_TITLE - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncXiuluotaRankTitle;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_KILL_USER - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnKickCrossUser;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_GET_ROLE_INFO_REQUEST - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGetRoleInfoRequest;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_HELLO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossHello;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_SYNC_CROSS_PERSON_RANK_TITLE_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossSyncTitleInfo;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_FORBID_TALK - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossForbidRoleTalk;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_1V1_GOTO_SCENE - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGame1V1GotoScene;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_1V1_SYNC_MATCH_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncCross1v1MatchInfo;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_UPDATE_CROSS_RECORD - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGameUpdateCrossRecord;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_SYNC_ROLE_DATA - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGameSyncRoleData;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_CHANNEL_CHAT - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGameChannelChat;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_SINGLE_CHAT - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGameSingleChat;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_LOCAL_SPEAKER - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGameLocalSpeaker;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_RANK_INFO_REQ - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGameRankInfoReq;
	m_on_cross_handler[crossgameprotocal::MT_SYNC_CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncCrossMultiuserChallengeMatchingState;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_MULTIUSER_CHALLENGE_GOTO_SCENE - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGameMultiuserChallengeGotoScene;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_MULTIUSER_CHALLENGE_START_MATCH - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossMultiuserChallengeStartMatch;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_MULTIUSER_CHALLENGE_SCORE_RANK_REWARD - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGlobalMultiuserChallengeScoreRankReward;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_RANK_WEEK_REWARD_COMMAND - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OncrossGameRankWeekRewardCommand;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_TUANZHAN_GOTO_SCENE - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossTuanzhanGotoScene;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_CROSS_ACTIVITY_GOTO_SCENE - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossActivityGotoScene;
	m_on_cross_handler[crossgameprotocal::MT_GAME_GAME_BOSS_CREATE_BOSS_BROADCAST - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossBossCreateBossBroadcast;
	m_on_cross_handler[crossgameprotocal::MT_GAME_GAME_BOSS_INFO_REQ - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossBossBossInfoReq;
	m_on_cross_handler[crossgameprotocal::MT_SYNC_CROSS_TEAM_FB_USER_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncCrossTeamFBUserInfo;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_SEND_MAIL_TO_USER - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGameSendMailToUser;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_GUILD_BATTLE_INFO_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGuildBattleInfoSync;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_GET_GUILD_BATTLE_RANK_LOG_RESP - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGuildBattleGetRankInfoResp;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_GET_GUILD_BATTLE_BOSS_INFO_REQ - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGuildBattleGetBossInfoResp;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_GUILD_BATTLE_SPECIAL_TIME_NOTICE - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGuildBattleSpecialTimeNotice;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_GET_GUILD_BATTLE_MONSTER_INFO_REQ - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGuildBattleGetMonsterInfoResp;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_GUILD_BATTLE_USER_SCORE_RANK - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncCrossGuildBattleUserScoreRankTop;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_GUILD_BATTLE_REGISTER_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncCrossGuildBattleRegisterReq;
	m_on_cross_handler[crossgameprotocal::MT_GAME_GAME_GUILD_BATTLE_REGISTER_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncCrossGuildBattleRegister;

	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_FISHING_INFO_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossFishingInfoSync;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_CROSS_1V1_SCORE_RANK_REWARD - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCross1v1ScoreRankReward;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_WORLD_COMMON_DATA_CHANGE - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncWorldCommonDataChange;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_1V1_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGame1V1Info;

	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_CROSS_WORLD_LEVEL_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossWorldLevel;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_LIEKUN_FB_KILL_ZONE_BOSS_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGameLieKunFBKillZoneBossSync;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_LIEKUN_FB_TOP_REWARD_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGameLieKunFBTopRewardSync;

	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_MESS_BATTLE_INFO_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGameMessBattleRankTopSync;

	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_NIGHT_FIGHT_INFO_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGameNightFightRankTopSync;

	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_SYNC_RANK_FIRST - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncCrossRankFirst;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_SYNC_COUPLE_RANK_FIRST - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncCrossCoupleRankFirst;

	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_CROSS_COMMON_BOSS_INFO_CHANGE - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossCommonBossInfoChange;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_CROSS_COMMON_BOSS_SYNC_INFO_REQ - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncCrossCommonBossInfoReq;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_CROSS_COMMON_BOSS_SYNC_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncCrossCommonBossInfo;

	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_CROSS_COMMON_BOSS_DROP_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossCommonBossDropInfo;

	m_on_cross_handler[crossgameprotocal::MT_GAME_CROSS_SYNC_ROLE_NUM_TO_GAME - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncRoleNumToHidden;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_TIME_INFO_TO_HIDDEN - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncGameServerTimeInfo;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_SYNC_CROSS_RAND_ACTIVITY_DATA - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGameSyncCrossRandActivityData;

	m_on_cross_handler[crossgameprotocal::MT_GAME_GAME_CROSS_GUILD_BATTLE_SOS - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnGameCrossGuildBattleSos;
	m_on_cross_handler[crossgameprotocal::MT_GAME_GAME_KILL_CROSS_BOSS - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnGameCrossKillCrossBoss;

	m_on_cross_handler[crossgameprotocal::MT_PICK_GUILD_RARE_ITEM_IN_HIDDEN_GS - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnPickGuildRareItemInHiddenGs;
	m_on_cross_handler[crossgameprotocal::MT_BROADCAST_GUILD_RARE_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncRareItemFromOriginalGs;

	m_on_cross_handler[crossgameprotocal::MT_SYNC_CROSS_RANK_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnBroadcastCrossRankToGuildChat;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_SYSTEM_MSG_TO_ORIGIN - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnRecvAndTransferCrossSystemMsg;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_GUILD_CHAT - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossRequestGuildChat;
	m_on_cross_handler[crossgameprotocal::MT_GAME_GAME_CROSS_GUILD_MEMBER_BE_KILL - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnGameCrossGuildMemberBeKill;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_GAME_GET_GUILD_ENEMY_LIST - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnCrossGameGetGuildEnemyList;

	m_on_cross_handler[crossgameprotocal::MT_SYNC_CROSS_CHALLENGEFIELD_REQ - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncCrossChallengefiledInfoReq;
	m_on_cross_handler[crossgameprotocal::MT_SYNC_CROSS_CHALLENGEFIELD_DATA_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncCrossChallengefiledInfo;
	m_on_cross_handler[crossgameprotocal::MT_CROSS_HUSONG_SHUIJING_GATHER_INFO_REQ - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &SceneManager::OnSyncCrossHusongShuijingGatherInfo;
}

void SceneManager::OnSyncCrossUser(NetID net_id, const char *data, int length)
{
	crossgameprotocal::CrossGameSyncCrossUser *cgscu = (crossgameprotocal::CrossGameSyncCrossUser *)data;
	CrossUserShadow::Instance().OnSyncCrossUser(cgscu);
}

void SceneManager::OnSaveCrossRoleInfo(NetID net_id, const char *data, int length)
{
	static crossgameprotocal::CrossGameSaveCrossRoleInfo complete_cgscri;
	if (length <= (int)sizeof(complete_cgscri) - (int)sizeof(complete_cgscri.role_cross_param_data) || length > (int)sizeof(complete_cgscri)) return;

	memcpy(&complete_cgscri, data, length);

	crossgameprotocal::CrossGameSaveCrossRoleInfo *cgscri = &complete_cgscri;
	cgscri->original_username[sizeof(GameName) - 1] = 0;
	cgscri->pname[sizeof(PlatName) - 1] = 0;

	CrossUserRegister::Instance().OnSaveCrossRoleInfo(cgscri);
}

void SceneManager::OnStartCrossAck(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameStartCrossAck *cgsca = (crossgameprotocal::CrossGameStartCrossAck *)msg;
	cgsca->login_server_ip[sizeof(cgsca->login_server_ip) - 1] = 0;
	cgsca->pname[sizeof(cgsca->pname) - 1] = 0;

	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(cgsca->original_uid));
	if (NULL == role) return;

	role->GetRoleCross()->OnStartCrossAck(cgsca);
}

void SceneManager::OnCrossUserMsg(NetID net_id, const char *msg, int length)
{
	if (length <= (int)sizeof(crossgameprotocal::CrossGameUserMsg)) return;

	crossgameprotocal::CrossGameUserMsg *cross_user_msg = (crossgameprotocal::CrossGameUserMsg *)msg;

	Role *receiver_user = this->GetRole(IntToUid(cross_user_msg->uid));	
	if (NULL == receiver_user) return;

	EngineAdapter::Instance().NetSend(receiver_user->GetNetId(), msg + sizeof(crossgameprotocal::CrossGameUserMsg), 
		length - sizeof(crossgameprotocal::CrossGameUserMsg));
}

void SceneManager::OnCrossAllUserMsg(NetID net_id, const char *msg, int length)
{
	if (length <= (int)sizeof(crossgameprotocal::CrossGameAllUserMsg)) return;

	crossgameprotocal::CrossGameAllUserMsg *cross_all_user_msg = (crossgameprotocal::CrossGameAllUserMsg *)msg;

	const char * msgptr = msg + sizeof(crossgameprotocal::CrossGameAllUserMsg);
	int msglen = length - sizeof(crossgameprotocal::CrossGameAllUserMsg);

	if (CROSS_ACTIVITY_TYPE_INVALID != cross_all_user_msg->cross_activity_type)
	{
		CrossUserRegister::Instance().SendToAllCrossUser(msgptr, msglen, cross_all_user_msg->cross_activity_type);
	}
	else
	{
		World::GetInstWorld()->SendToAllGateway(msgptr, msglen);
		if (static_cast<size_t>(msglen) >= sizeof(Protocol::SCSpeaker))
		{
			Protocol::SCSpeaker * speaker = (Protocol::SCSpeaker *)(msgptr);
			if (speaker->header.msg_type == Protocol::MT_SYSTEM_SPEAKER_SC)
			{
				ChatManager::Instance().AddWorldChatBoard(msgptr, msglen, true);
			}
		}
	}
}

void SceneManager::OnCrossSyncActivityStatus(NetID net_id, const char *data, int length)
{
	crossgameprotocal::CrossGameSyncActivityStatus *cgsas = (crossgameprotocal::CrossGameSyncActivityStatus *)data;

	ActivityShadow::Instance().OnSyncCrossActivityStatus(CrossManager::Instance().GetCrossTypeWithNetID(net_id), cgsas);
}

void SceneManager::OnCrossXiuluoTowerGotoScene(NetID net_id, const char *data, int length)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	crossgameprotocal::CrossGameXiuluotaGotoScene *cgsas = (crossgameprotocal::CrossGameXiuluotaGotoScene *)data;

	UserID user_id = IntToUid(cgsas->cross_uid);
	if (!IsCrossUser(user_id)) 
	{
		return;
	}

	if (CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER != GetCrossUserCrossActivityType(user_id))
	{
		return;
	}

	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
	if (NULL == role) return;

	CrossXiuluoTowerUser *user = ActivityShadow::Instance().GetCrossXiuluoTowerUser(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
	if (NULL != user)
	{
		user->sex = role->GetSex();
		user->prof = role->GetProf();
		user->camp = role->GetCamp();
		user->level = role->GetLevel();
		role->GetName(user->user_name);

		if (user->cur_layer > 0)
		{
			-- user->cur_layer;
			user->cur_layer_kill_count = 0;
			user->not_drop_layer_count = 0;
			user->immediate_realive_count = 0;
		}

		Posi enter_pos;
		int scene_id = 0;
		if (ActivityShadow::Instance().IsCrossActivityStandby(CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER))
		{
			scene_id = CrossXiuluoTowerCommonConfig::Instance().GetSceneIdStandy(&enter_pos);
		}
		else if (ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER))
		{
			scene_id = CrossXiuluoTowerCommonConfig::Instance().GetLayerSceneId(user->cur_layer, &enter_pos);
		}
		
		World::GetInstWorld()->GetSceneManager()->GoTo(role, scene_id, CROSS_ACTIVITY_XIULUO_TOWER_SCENE_KEY, enter_pos);
	}
}

void SceneManager::OnSyncXiuluotaUserResult(NetID net_id, const char *data, int length)
{
	crossgameprotocal::CrossGameSyncXiuluoTowerUserResult *cgsxltur = (crossgameprotocal::CrossGameSyncXiuluoTowerUserResult *)data;

	UserID user_id = LongLongToUniqueUserID(cgsxltur->uuid).user_id;

	gamelog::g_log_cross.printf(LL_INFO, "CrossXiuluota ActivityResult uid %d, max_layer %d, rank_pos %d, kill_role_count %d, kill_boss_count %d, user_level %d, reward_cross_honor %d", 
		UidToInt(user_id), cgsxltur->max_layer, cgsxltur->rank_pos, cgsxltur->kill_role_count, cgsxltur->kill_boss_count, cgsxltur->user_level, cgsxltur->reward_cross_honor);

	if (cgsxltur->rank_pos <= 0)
	{
		return;
	}

// 	static MailContentParam mail_content_param; mail_content_param.Reset();
// 
// 	CrossXiuluoTowerRankRewardCfg::LevelRewardCfg *reward_cfg = CrossXiuluoTowerCommonConfig::Instance().GetRankRewardItem(cgsxltur->rank_pos, cgsxltur->user_level);
// 	if (NULL != reward_cfg)
// 	{
// 		mail_content_param.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_CROSS_HONOR, reward_cfg->reward_honor);
// 		
// 		mail_content_param.AddItem(reward_cfg->reward_item.item_id, reward_cfg->reward_item.num, reward_cfg->reward_item.is_bind);
// 	}
// 
// 	int length2 = SNPRINTF(mail_content_param.contenttxt, sizeof(mail_content_param.contenttxt), gamestring::g_cross_xiuluo_tower_reward_content,
// 		cgsxltur->max_layer + 1);
// 	if (length2 <= 0) return;
// 
// 	if (length2 < (int)sizeof(mail_content_param.contenttxt))
// 	{
// 		length2 += SNPRINTF(((char *)mail_content_param.contenttxt) + length2, sizeof(mail_content_param.contenttxt) - length2, 
// 			gamestring::g_cross_xiuluo_tower_rank_reward_content, cgsxltur->rank_pos);
// 	}
// 
// 	if (length2 > 0)
// 	{
// 		MailRoute::Instance().MailToUser(LongLongToUniqueUserID(cgsxltur->uuid).user_id, SYSTEM_MAIL_REASON_INVALID, mail_content_param);
// 
// 		UNSTD_STATIC_CHECK(MAX_ATTACHMENT_ITEM_NUM == 3);
// 		gamelog::g_log_mail.printf(LL_INFO, "CrossXiuluota ActivityReward uid %d"
// 			"item1[ %s ], item2[ %s ], virtual_items[%s] ",
// 			UidToInt(user_id),
// 			ItemDataWrapperLog(&mail_content_param.item_list[0], 0), 
// 			ItemDataWrapperLog(&mail_content_param.item_list[1], 1), 
// 			MailVirtualItemLog(&mail_content_param.virtual_item_list));
// 	}
}

void SceneManager::OnSyncXiuluotaRankTitle(NetID net_id, const char* data, int length)
{
	crossgameprotocal::CrossGameSyncXiuluoTowerRankTitle * cgrt = (crossgameprotocal::CrossGameSyncXiuluoTowerRankTitle*)data;
	WorldStatus2::Instance().SetCrossXiuluoTowerRankTitle(cgrt->rank, cgrt->uid);
}

void SceneManager::OnKickCrossUser(NetID net_id, const char *data, int length)
{
	crossgameprotocal::CrossGameKickUser *cgku = (crossgameprotocal::CrossGameKickUser *)data;

	if (CrossConfig::Instance().IsHiddenServer())
	{
		Role* user = this->GetRole(IntToUid(cgku->bekick_uid));
		if (NULL != user && NULL != user->GetScene())
		{
			EngineAdapter::Instance().NetDisconnect(user->GetNetId(), user->GetUserId(), user->GetName(), user->GetScene()->GetSceneID(), "OnKickCrossUser");
		}
		else
		{
			InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_UUID, cgku->bekick_uuid, data, sizeof(*cgku));
		}
	}
	else
	{
		CrossUserMap& cross_user_map = World::GetInstWorld()->GetSceneManager()->GetCrossUserMap();
		cross_user_map.erase(cgku->bekick_uuid);
	}
}

void SceneManager::OnCrossGetRoleInfoRequest(NetID net_id, const char *data, int length)
{
	crossgameprotocal::CrossGameGetRoleInfoRequest *cgku = (crossgameprotocal::CrossGameGetRoleInfoRequest *)data;

	switch (cgku->request_type)
	{
	case crossgameprotocal::GameCrossQueryRoleInfoRequest::QUERY_ROLE_INFO_TYPE_ALL:
		QueryHandler::Instance().OnQuery(cgku->target_uid, QUERY_FROM_CROSS, cgku->request_uid, cgku->request_plat_type);
		break;
	case crossgameprotocal::GameCrossQueryRoleInfoRequest::QUERY_ROLE_INFO_TYPE_UUID_NAME:
	{
		Protocol::SCFindRoleByNameRet drbnr;
		UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(cgku->target_uid);
		drbnr.msg_identify = cgku->param;
		if (NULL == uc)
		{
			drbnr.role_id = 0;
		}
		else
		{
			drbnr.uuserverid = LocalConfig::Instance().GetPlatType();
			drbnr.uuserverid = (drbnr.uuserverid << 32) + LocalConfig::Instance().GetMergeServerId();
			drbnr.uuid= uc->uuid;
			drbnr.role_id = uc->uid;
			drbnr.is_online = World::GetInstWorld()->GetSceneManager()->IsOnLine(uc->uuid);
			drbnr.at_cross = 0;
			if (drbnr.is_online != 0)
			{
				drbnr.at_cross = World::GetInstWorld()->GetSceneManager()->UserCrosserExist(uc->uuid);
			}
			STRNCPY(drbnr.role_name, uc->user_name, sizeof(GameName));
			drbnr.sex = uc->sex;
			drbnr.prof = uc->prof;
			drbnr.level = uc->level;
			drbnr.camp = uc->camp;
			drbnr.avatar_timestamp = uc->avatar_timestamp;
			drbnr.capability = uc->capability;
		}
		UniqueUserID uuid = UniqueUserID(cgku->request_plat_type, IntToUid(cgku->request_uid));
		CrossManager::Instance().UserProtocalTransferToGame(CROSS_FUNCTION_TYPE_CROSS_RANK, UniqueUserIDToLongLong(uuid), (const char *)&drbnr, sizeof(drbnr));
	}
		break;
	default:
		break;
	}
}

void SceneManager::OnCrossHello(NetID net_id, const char *data, int length)
{
	CrossManager::Instance().OnCrossHello(net_id);
}

void SceneManager::OnCrossForbidRoleTalk(NetID net_id, const char *data, int length)
{
	crossgameprotocal::CrossGameForbidTalk *cgft = (crossgameprotocal::CrossGameForbidTalk *)data;

	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(cgft->cross_role_id));
	if (nullptr != role)
	{
		role->ForbidTalk(cgft->forbid_time, "SceneManager::OnCrossForbidRoleTalk", "SceneManager::OnCrossForbidRoleTalk");
	}
}

void SceneManager::OnCrossSyncTitleInfo(NetID net_id, const char *data, int length)
{
	crossgameprotocal::CrossGameSyncCrossPersonRankTitleInfo *cgscprti = (crossgameprotocal::CrossGameSyncCrossPersonRankTitleInfo *)data;

	CrossPersonRankShadow::Instance().OnSyncCrossTitleInfo(cgscprti);
}

void SceneManager::OnCrossGame1V1GotoScene(NetID net_id, const char *data, int length)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	crossgameprotocal::CrossGame1V1GotoScene *cgs = (crossgameprotocal::CrossGame1V1GotoScene*)data;
	//UserID uid = LongLongToUniqueUserID(cgs->uuid).user_id;

	Role *role = this->GetRole(cgs->cross_uid);
	if (NULL == role)
	{
		return;
	}

	if (!IsCrossUser(cgs->cross_uid))
	{
		role->BackToOriginalServer();
		return;
	}

	if (!ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_CROSS_1V1))
	{
		role->NoticeNum(errornum::EN_ENTER_ACTIVITY_NOT_START);
		ROLE_LOG_QUICK6(LOG_TYPE_CROSS_1V1, role, 1, 1, "ActivityNotStart", NULL);

		role->BackToOriginalServer();
		return;
	}

	CrossActivity1v1FightUnit *match_unit = ActivityShadow::Instance().GetCross1v1FightMatchUnit(cgs->uuid);
	if (NULL == match_unit)
	{
		role->NoticeNum(errornum::EN_1V1_NOT_MATCH);
		ROLE_LOG_QUICK6(LOG_TYPE_CROSS_1V1, role, 1, 1, "CanNotGetMatchUnit", NULL);

		role->BackToOriginalServer();
		return;
	}

	int activity_sceneid = Cross1V1CommonConfig::Instance().GetSceneID();
	Posi activity_pos;
	if (match_unit->uuid1 == cgs->uuid)
	{
		activity_pos = Cross1V1CommonConfig::Instance().GetPos1();
	}
	else
	{
		activity_pos = Cross1V1CommonConfig::Instance().GetPos2();
	}

		role->Recover();

	World::GetInstWorld()->GetSceneManager()->GoTo(role, activity_sceneid, match_unit->fight_seq, activity_pos);
}

void SceneManager::OnCrossGame1V1Info(NetID net_id, const char *data, int length)
{
	crossgameprotocal::CrossGame1V1Info *sci = (crossgameprotocal::CrossGame1V1Info*)data;
	Role *role = this->GetRole(IntToUid(sci->cross_uid));
	if (NULL != role)
	{
		role->GetRoleCross()->OnSyncCross1V1Info(sci);
	}
}

void SceneManager::OnSyncCross1v1MatchInfo(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameSyncCross1v1MatchInfo *cgscmi = (crossgameprotocal::CrossGameSyncCross1v1MatchInfo *)msg;
	ActivityShadow::Instance().OnCross1v1MatchInfo(cgscmi);

	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_CROSS_1V1);
	if (scene_id > 0 && CrossConfig::Instance().IsHiddenServer())
	{
		FBConfigList::iterator i = m_fb_config_list.find(scene_id);
		if (i == m_fb_config_list.end()) return;

		for (int i = 0; i < cgscmi->unit_count && i < crossgameprotocal::CrossGameSyncCross1v1MatchInfo::MAX_FIGHT_UNIT_COUNT; ++i)
		{
			if (cgscmi->fight_unit_list[i].fight_seq > 0 && cgscmi->fight_unit_list[i].uuid1 > 0 && cgscmi->fight_unit_list[i].uuid2 > 0)
			{
				Scene *scene = NULL;

				// 同一个key的副本不能重复创建
				SpecialCross1v1 *special_cross_1v1_temp = this->GetSpecialCross1v1Challenge(cgscmi->fight_unit_list[i].fight_seq);
				if (NULL == special_cross_1v1_temp)
				{
					this->CreateFB(scene_id, cgscmi->fight_unit_list[i].fight_seq, &scene, true, 0);

					if (NULL != scene && Scene::SCENE_TYPE_CROSS_1V1 == scene->GetSceneType())
					{
						SpecialCross1v1 *special_cross_1v1 = dynamic_cast<SpecialCross1v1*>(scene->GetSpecialLogic());
						if (nullptr != special_cross_1v1)
							special_cross_1v1->OnSetFighterInfo(cgscmi->fight_unit_list[i].uuid1, cgscmi->fight_unit_list[i].uuid2);
					}
				}
				else
				{
					gamelog::g_log_cross.printf(LL_ERROR, "SceneManager::OnSyncCross1v1MatchInfo1 FB with the same key(fight_seq[%d]) cannot be created repeatedly.",
						cgscmi->fight_unit_list[i].fight_seq);
				}
			}
			else if (cgscmi->fight_unit_list[i].fight_seq > 0 && cgscmi->fight_unit_list[i].uuid1 > 0 && cgscmi->fight_unit_list[i].uuid2 >= 0)
			{
				Scene *scene = NULL;

				// 同一个key的副本不能重复创建
				//this->CreateFB(scene_id, cgscmi->fight_unit_list[i].fight_seq, &scene, true, CROSS_ACTIVITY_1V1_BATTLE_TIME_S + 5);
				SpecialCross1v1 *special_cross_1v1_temp = this->GetSpecialCross1v1Challenge(cgscmi->fight_unit_list[i].fight_seq);
				if (NULL == special_cross_1v1_temp)
				{
					this->CreateFB(scene_id, cgscmi->fight_unit_list[i].fight_seq, &scene, true, 0);

					if (NULL != scene && Scene::SCENE_TYPE_CROSS_1V1 == scene->GetSceneType())
					{
						SpecialCross1v1 *special_cross_1v1 = dynamic_cast<SpecialCross1v1*>(scene->GetSpecialLogic());
						if (nullptr != special_cross_1v1)
							special_cross_1v1->OnSetFighterInfo(cgscmi->fight_unit_list[i].uuid1, cgscmi->fight_unit_list[i].uuid2);
					}
				}
				else
				{
					gamelog::g_log_cross.printf(LL_ERROR, "SceneManager::OnSyncCross1v1MatchInfo2 FB with the same key(fight_seq[%d]) cannot be created repeatedly.",
						cgscmi->fight_unit_list[i].fight_seq);
				}
			}
		}
	}
}

void SceneManager::OnCrossMultiuserChallengeStartMatch(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameCrossMultiuserChallengeStartMatch *cgcmcsm = (crossgameprotocal::CrossGameCrossMultiuserChallengeStartMatch *)msg;
	if (0 == cgcmcsm->match_key) return;

	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; ++ i)
	{
		if (cgcmcsm->user_info_list[i].uuid <= 0) return;

		cgcmcsm->user_info_list[i].user_name[sizeof(GameName) - 1] = 0;
	}

	// 创建跨服3v3场景，获取不到副本key的场景就立即创建一个
	SpecialCrossMultiuserChallenge *special_cross_multiuser_challenge = this->GetSpecialCrossMultiuserChallenge(cgcmcsm->match_key, true);
	if (NULL != special_cross_multiuser_challenge)
	{
		special_cross_multiuser_challenge->OnMatchStart(cgcmcsm);
	}
}

void SceneManager::OnSyncCrossMultiuserChallengeMatchingState(NetID net_id, const char *data, int length)
{
	crossgameprotocal::CrossGameMultiuserChallengeMatchingStateNotify *scmcms = (crossgameprotocal::CrossGameMultiuserChallengeMatchingStateNotify *)data;

	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(scmcms->cross_uid));
	if (NULL == role) return;
	role->GetRoleCross()->OnSyncCrossMultiuserChallengeMatchingState(scmcms);
}

void SceneManager::OnCrossGameMultiuserChallengeGotoScene(NetID net_id, const char *data, int length)
{
	crossgameprotocal::CrossGameMultiuserChallengeGotoScene *gmcgs = (crossgameprotocal::CrossGameMultiuserChallengeGotoScene *)data;

	UserID user_id = IntToUid(gmcgs->cross_uid);
	if (!IsCrossUser(user_id)) 
	{
		return;
	}

	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
	if (NULL == role) return;

	role->ClearFightBuffEffect(EffectBase::CLEAR_REASON_LEAVE_SCENE);

	int CROSS_MULTIUSER_CHALLENGE_SCENE_ID = Cross3v3Config::Instance().GetMatchSceneID();

	Posi pos = Cross3v3Config::Instance().GetRelivePos(gmcgs->side);
	World::GetInstWorld()->GetSceneManager()->GoTo(role, CROSS_MULTIUSER_CHALLENGE_SCENE_ID, gmcgs->fb_key, pos);
}

void SceneManager::OnCrossGlobalMultiuserChallengeScoreRankReward(NetID net_id, const char *data, int length)
{
	if (!CrossConfig::Instance().CheckCrossServerHasCrossActivity(CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE, CrossManager::Instance().GetCrossTypeWithNetID(net_id)))
	{
		return;
	}

	// 开服天数不到
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	if (opengame_day < Cross3v3Config::Instance().GetOtherCfg().open_server_day)
	{
		return;
	}

	crossgameprotocal::CrossGameMultiuserChallengeScoreRankReward *scmcms = (crossgameprotocal::CrossGameMultiuserChallengeScoreRankReward *)data;
	CrossRoute::Instance().OnCrossMultiuserChallengeScoreRankReward(scmcms);
}


void SceneManager::OnCrossGameUpdateCrossRecord(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameUpdateCrossRecord *cgucr = (crossgameprotocal::CrossGameUpdateCrossRecord *)msg;
	
	//跨服牧场排行榜
// 	if (cgucr->record_param_list[2] <= CROSS_PASTURE_TOP_RANK_COUNT_MAX)
// 	{
// 		WorldStatus::Instance().SetCrossPastureTopRankUid(cgucr->record_param_list[2], cgucr->uid);
// 	}

	if (cgucr->uid < 0) return;
	CrossRecordManager::Instance().OnUpdateCrossRecord(IntToUid(cgucr->uid), cgucr);
}


void SceneManager::OncrossGameRankWeekRewardCommand(NetID net_id, const char *msg, int length)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	// 打一条日志展示协议信息
	crossgameprotocal::CrossGameRankWeekRewardCommand *cgrwrc = (crossgameprotocal::CrossGameRankWeekRewardCommand *)msg;

	for (int rank = 0; rank < cgrwrc->item_count; ++ rank)
	{
		crossgameprotocal::CrossGameRankWeekRewardCommand::Item &item = cgrwrc->item_list[rank];

		gamelog::g_log_cross.printf(LL_INFO, "OncrossGameRankWeekRewardCommand Protocal Detail: rank_type %d, rank %d, plat_type %d, uid %d, rank_value %lld",
			cgrwrc->rank_type, rank, item.plat_type, item.uid, item.rank_value);

		if (LocalConfig::Instance().GetPlatType() != item.plat_type)
		{
			gamelog::g_log_cross.printf(LL_INFO, "OncrossGameRankWeekRewardCommand not same plat: rank_type %d, rank %d, plat_type %d, uid %d, rank_value %lld",
				cgrwrc->rank_type, rank, item.plat_type, item.uid, item.rank_value);
		}
		else
		{
			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(item.uid);
			if (NULL == user_node)
			{
				gamelog::g_log_cross.printf(LL_INFO, "OncrossGameRankWeekRewardCommand not exist this user: rank_type %d, rank %d, plat_type %d, uid %d, rank_value %lld",
					cgrwrc->rank_type, rank, item.plat_type, item.uid, item.rank_value);
			}
			else
			{
				bool send_reward_succ = false;
				std::string reward_str;

				const CrossRankWeekReward *reward_cfg = LOGIC_CONFIG->GetCrossRankWeekRewardCfg().GetReward(cgrwrc->rank_type, rank);
				if (NULL != reward_cfg)
				{
					// 发奖励
					static MailContentParam contentparam; contentparam.Reset();
					const ItemBase *reward_item_base = ITEMPOOL->GetItem(reward_cfg->reward_item.item_id);
					if (NULL != reward_item_base)
					{
						contentparam.item_list[0].item_id = reward_cfg->reward_item.item_id;
						contentparam.item_list[0].num = reward_cfg->reward_item.num;
						contentparam.item_list[0].is_bind = (reward_cfg->reward_item.is_bind) ? 1 : 0;
						contentparam.item_list[0].invalid_time = reward_item_base->CalInvalidTime(); 
					}

					const char *tmp_str = ItemConfigDataLog(&reward_cfg->reward_item, NULL);
					reward_str = tmp_str;

					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_cross_rank_week_reward_content, 
						cgrwrc->rank_type, rank + 1, (int)reward_cfg->reward_item.item_id);
					if (length > 0)
					{
						MailRoute::MailToUser(IntToUid(item.uid), SYSTEM_MAIL_REASON_INVALID, contentparam);
						send_reward_succ = true;
					}
				}

				PlatName plat_name;
				LocalConfig::GetPlatName(item.plat_type, plat_name);

				gamelog::g_log_cross.printf(LL_INFO, "OncrossGameRankWeekRewardCommand Reward : ret %d, rank_type %d, rank %d, plat_type %d, plat_name %s, "
					"uid %d, user_name %s, rank_value %lld, reward_item %s",
					(send_reward_succ ? 1 : 0), cgrwrc->rank_type, rank, item.plat_type, plat_name, user_node->uid, user_node->user_name,
					item.rank_value, reward_str.c_str());
			}
		}
	}
}


void SceneManager::OnCrossTuanzhanGotoScene(NetID net_id, const char *data, int length)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	crossgameprotocal::CrossGameTuanzhanGotoScene *cgtzgs = (crossgameprotocal::CrossGameTuanzhanGotoScene *)data;

	UserID user_id = IntToUid(cgtzgs->cross_uid);
	if (!IsCrossUser(user_id))
	{
		return;
	}

	if (CROSS_ACTIVITY_TYPE_TUANZHAN != GetCrossUserCrossActivityType(user_id))
	{
		return;
	}

	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
	if (NULL == role) return;

	const Posi enter_pos = CrossTuanzhanConfig::Instance().GetRandomRelivePos();
	int scene_id = CrossTuanzhanConfig::Instance().GetOtherCfg().scene_id;

	World::GetInstWorld()->GetSceneManager()->GoTo(role, scene_id, CROSS_TUANZHAN_SCENE_KEY, enter_pos);
}

void SceneManager::OnCrossBossCreateBossBroadcast(NetID net_id, const char *data, int length)
{

	crossgameprotocal::GameGameBossCreateBossBroadcast *ggbcbb = (crossgameprotocal::GameGameBossCreateBossBroadcast *)data;

	static Protocol::SCBossInfoToAll bita;
	bita.notify_reason = Protocol::NOTIFY_REASON_BOSS_REFRESH;
	bita.scene_id = ggbcbb->scene_id;
	bita.boss_id = ggbcbb->boss_id;
	bita.next_refresh_time = ggbcbb->next_refresh_timestamp;
	bita.killer_uid = 0;
	if (ggbcbb->uuid > 0)
	{
		bita.killer_uid = UidToInt(LongLongToUniqueUserID(ggbcbb->uuid).user_id);
	}

	World::GetInstWorld()->SendToAllGateway((const char *)&bita, sizeof(bita));
}

void SceneManager::OnCrossBossBossInfoReq(NetID net_id, const char *data, int length)
{
	crossgameprotocal::GameGameBossInfoReq *ggbir = (crossgameprotocal::GameGameBossInfoReq *)data;
	if (ggbir->cross_boss_type == crossgameprotocal::CROSS_BOSS_TYPE_CROSS)
	{
		this->OnCrossBossInfoReq(ggbir);
	}
	else if (ggbir->cross_boss_type == crossgameprotocal::CROSS_BOSS_TYPE_MIZANG)
	{
		this->OnCrossMizangBossInfoReq(ggbir);
	}
	else if (ggbir->cross_boss_type == crossgameprotocal::CROSS_BOSS_TYPE_YOUMING)
	{
		this->OnCrossYoumingBossInfoReq(ggbir);
	}
}

void SceneManager::OnSyncCrossTeamFBUserInfo(NetID net_id, const char *data, int length)
{
	crossgameprotocal::SyncCrossTeamFBUserInfo *sctfbui = (crossgameprotocal::SyncCrossTeamFBUserInfo *)data;

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	ActivityShadow::Instance().SyncCrossTeamFBRoomInfo(sctfbui->user_info);
}

void SceneManager::OnCrossActivityGotoScene(NetID net_id, const char *data, int length)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	crossgameprotocal::CrossGameCrossActivityGotoScene *zgs = (crossgameprotocal::CrossGameCrossActivityGotoScene *)data;

	UserID user_id = IntToUid(zgs->cross_uid);
	if (!IsCrossUser(user_id)) 
	{
		return;
	}

	int act_type = GetCrossUserCrossActivityType(user_id);
	if (zgs->cross_activity_type != act_type)
	{
		return;
	}

	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
	if (NULL == role) return;

	Posi enter_pos;
	int scene_id = 0;
	int target_scene_key = 0;

	switch (act_type)
	{
	case CROSS_ACTIVITY_TYPE_PASTURE:
		{
			enter_pos = CrossPastureConfig::Instance().GetOtherCfg().role_pos;
			scene_id = CrossPastureConfig::Instance().GetOtherCfg().scene_id;	
			target_scene_key = CROSS_PUBLIC_SCENE_KEY;
		}
		break;

	case CROSS_ACTIVITY_TYPE_CROSS_BOSS:
		{
			int scene_level = role->GetRoleCross()->GetCrossBossSceneLevel();
			const CrossBossLayerCfg *layer_cfg = CrossBossConfig::Instance().GetCrossBossLayerCfg(scene_level);
			if (NULL == layer_cfg)
			{
				return;
			}

			if (role->GetRoleCross()->IsEnterCrossMoveToPos())
			{
				Posi move_pos = role->GetRoleCross()->GetEnterCrossMovePos();
				enter_pos = move_pos;
				role->GetRoleCross()->ClearEnterCrossMovePos();
			}
			else
			{
				enter_pos = layer_cfg->enter_pos;
			}

			scene_id = layer_cfg->scene_id;
			target_scene_key = CROSS_PUBLIC_SCENE_KEY;
		}
		break;

	case CROSS_ACTIVITY_TYPE_TEAM_FB:
		{
			const CrossTeamFBUserInfo *user_info = ActivityShadow::Instance().GetCrossTeamFBUserInfo(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()));
			if (NULL == user_info) return;

			const CrossTeamFBLayerCfg *cfg = LOGIC_CONFIG->GetCrossTeamFBConfig().GetCrossTeamFBLayerCfg(user_info->layer);
			if (NULL == cfg) return;

			enter_pos = cfg->enter_pos;
			scene_id = cfg->scene_id;
			target_scene_key = user_info->room + 1;
		}
		break;

	case CROSS_ACTIVITY_TYPE_HOT_SPRING:
		{
			enter_pos = CrossHotSpringConfig::Instance().GetEnterPos();
			scene_id = CrossHotSpringConfig::Instance().GetSceneID();
			target_scene_key = CROSS_HS_SCENE_KEY;
		}
		break;

	case CROSS_ACTIVITY_TYPE_SHUIJING:
		{
			enter_pos = CrossShuiJingConfig::Instance().GetOtherCfg().realive_pos;
			scene_id = CrossShuiJingConfig::Instance().GetOtherCfg().scene_id;	
			target_scene_key = CROSS_PUBLIC_SCENE_KEY;
		}
		break;

	case CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE:
	{
		scene_id = role->GetRoleCross()->GetCrossGuildBattleSceneId();
		if (ActivityShadow::Instance().IsCrossActivityStandby(CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE))
		{
			enter_pos = CrossGuildBattleConfig::Instance().GetStandyEnterPosBySceneID(scene_id);
		}
		else
		{
			enter_pos = CrossGuildBattleConfig::Instance().GetRandEnterPosBySceneID(scene_id);
		}
		
		if (enter_pos == Posi(0, 0))
		{
			return;
		}

		target_scene_key = CROSS_PUBLIC_SCENE_KEY;
	}
	break;

	case CROSS_ACTIVITY_TYPE_DARK_NIGHT:
	{
		//enter_pos = CrossDarkNightConfig::Instance().GetOtherCfg().born_posi;
		if (ActivityShadow::Instance().IsCrossActivityStandby(CROSS_ACTIVITY_TYPE_DARK_NIGHT))
		{
			enter_pos = gamecommon::GetDisRandPos(CrossDarkNightConfig::Instance().GetRealivePosiByKey(0),5);
		}
		else if (ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_DARK_NIGHT))
		{
			enter_pos = CrossDarkNightConfig::Instance().GetRandRealivePosi();
		}
		
		scene_id = CrossDarkNightConfig::Instance().GetOtherCfg().scene_id;
		target_scene_key = CROSS_PUBLIC_SCENE_KEY;
	}
	break;

	case CROSS_ACTIVITY_TYPE_FISHING:
	{
		const CrossFishingOtherConfig& other_cfg = CrossFishingConfig::Instance().GetOtherCfg();
		scene_id = other_cfg.sceneid;
		enter_pos = other_cfg.enter_pos;
		target_scene_key = CROSS_PUBLIC_SCENE_KEY;
	}
	break;

	case CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB:
	{
		const NightFightFbOtherConfig& other_cfg = LOGIC_CONFIG->GetNightFightFbConfig().GetOtherCfg();
		scene_id = other_cfg.scene_id;;

		DoubleSideFBManager::Instance().RequireEnterFb(role, CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB, true, target_scene_key);
		if (ActivityShadow::Instance().IsCrossActivityStandby(CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB))
		{	//准备进入固定区域
			enter_pos = gamecommon::GetDisRandPos(LOGIC_CONFIG->GetNightFightFbConfig().GetEnterPosStandy(), 5);
		}
		else if (ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB))
		{
			enter_pos = gamecommon::GetDisRandPos(LOGIC_CONFIG->GetNightFightFbConfig().GetRelievePos(), 5);
		}
	}
	break;

	case CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB:
	{
		const MessBattleFbOtherConfig& other_cfg = LOGIC_CONFIG->GetMessBattleFbConfig().GetOtherCfg();
		scene_id = other_cfg.scene_id;

		DoubleSideFBManager::Instance().RequireEnterFb(role, CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB, true, target_scene_key);
		if (ActivityShadow::Instance().IsCrossActivityStandby(CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB))
		{
			enter_pos = gamecommon::GetDisRandPos(LOGIC_CONFIG->GetMessBattleFbConfig().GetRelievePosByKey(0), 5);
		}
		else if (ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB))
		{
			enter_pos = gamecommon::GetDisRandPos(LOGIC_CONFIG->GetMessBattleFbConfig().GetRelievePos(), 5);
		}
	}
	break;

	case CROSS_ACTIVITY_TYPE_LIEKUN_FB:
	{
		int zone = role->GetRoleCross()->GetCrossLieKunSceneZone();
		if (zone < LIEKUN_ZONE_TYPE_MAIN || zone >= LIEKUN_ZONE_TYPE_COUNT)
		{
			return;
		}
		const CrossLieKunFbZoneCfg *zone_cfg = CrossLieKunFbConfig::Instance().GetCrossLieKunFbZoneCfg(zone);
		if (NULL == zone_cfg)
		{
			return;
		}

		enter_pos = zone_cfg->enter_pos;
		scene_id = zone_cfg->scene_id;
		target_scene_key = CROSS_PUBLIC_SCENE_KEY;
	}
	break;

	case CROSS_ACTIVITY_TYPE_VIP_BOSS:
	{
		static Protocol::CSEnterBossFamily ebf;

		if (!role->GetRoleCross()->GetCrossVipBossEnterInfo(&ebf))
		{
			role->BackToOriginalServer();
			return;
		}

		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(ebf.scene_id, 0);
		if (NULL == scene)
		{
			role->BackToOriginalServer();
			return;
		}

		scene_id = ebf.scene_id;
		target_scene_key = 0;

		switch (ebf.enter_type)
		{
		case Protocol::BOSS_ENTER_TYPE_BOSS_FAMILY:
		{
			const BossFamilyEnterConditionCfg * enter_condition_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetEnterConditionInfo(role, ebf.scene_id);
			if (NULL == enter_condition_cfg) return;

			const BossFamilyBossInfoCfg *boss_info_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyBossInfo(ebf.boss_id);
			if (nullptr == boss_info_cfg || 0 == boss_info_cfg->is_cross)
			{
				role->BackToOriginalServer();
				return;
			}

			// 传送到坐标
			if (role->GetRoleCross()->IsEnterCrossMoveToPos())
			{
				Posi move_pos = role->GetRoleCross()->GetEnterCrossMovePos();
				enter_pos = move_pos;

				role->GetRoleCross()->ClearEnterCrossMovePos();
			}
			else
			{
				enter_pos = enter_condition_cfg->enter_pos;
			}
		}
		break;
		}
	}
	break;

	case CROSS_ACTIVITY_TYPE_CROSS_MIZANG_BOSS:
	{
		int scene_level = role->GetRoleCross()->GetCrossMizangBossSceneLevel();
		const CrossMizangBossLayerCfg *layer_cfg = CrossMizangBossConfig::Instance().GetCrossBossLayerCfg(scene_level);
		if (NULL == layer_cfg)
		{
			return;
		}

		if (role->GetRoleCross()->IsEnterCrossMoveToPos())
		{
			Posi move_pos = role->GetRoleCross()->GetEnterCrossMovePos();
			enter_pos= move_pos;
			role->GetRoleCross()->ClearEnterCrossMovePos();
		}
		else
		{
			enter_pos = layer_cfg->enter_pos;
		}

		scene_id = layer_cfg->scene_id;
		target_scene_key = CROSS_PUBLIC_SCENE_KEY;
	}
	break;

	case CROSS_ACTIVITY_TYPE_CROSS_YOUMING_BOSS:
	{
		int scene_level = role->GetRoleCross()->GetCrossYoumingBossSceneLevel();
		const CrossYoumingBossLayerCfg *layer_cfg = CrossYoumingBossConfig::Instance().GetCrossBossLayerCfg(scene_level);
		if (NULL == layer_cfg)
		{
			return;
		}

		enter_pos = layer_cfg->enter_pos;
		scene_id = layer_cfg->scene_id;
		target_scene_key = CROSS_PUBLIC_SCENE_KEY;
	}
	break;

	case CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI:
	{
		const CrossBianJingZhiDiOtherCfg &other_cfg = CrossBianJingZhiDiConfig::Instance().GetCrossBianJingZhiDiOtherCfg();
		enter_pos = other_cfg.relive_pos;
		scene_id = other_cfg.m_sceneid;
		target_scene_key = CROSS_PUBLIC_SCENE_KEY;
	}
	break;

	case CROSS_ACTIVITY_TYPE_HUSONG_SHUIJING:
	{
		const HusongSJOtherCfg &other_cfg = HusongShuijingConfig::Instance().GetOtherCfg();
		//enter_pos = other_cfg.enter_pos;
		scene_id = other_cfg.scene_id;
		target_scene_key = CROSS_PUBLIC_SCENE_KEY;
		if (ActivityShadow::Instance().IsCrossActivityStandby(CROSS_ACTIVITY_TYPE_HUSONG_SHUIJING))
		{
			enter_pos = other_cfg.enter_pos;
		}
		else if(ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_HUSONG_SHUIJING))
		{
			enter_pos = HusongShuijingConfig::Instance().GetEnterPos(role->GetRoleCross()->GetCrossHusongShuijingEnterType());
		}
	}
	break;

	default:
		return;
	}

	World::GetInstWorld()->GetSceneManager()->GoTo(role, scene_id, target_scene_key, enter_pos);
}

void SceneManager::OnCrossGameSendMailToUser(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameSendMailToUser *cgsmtu = (crossgameprotocal::CrossGameSendMailToUser *)msg;

	static MailContentParam contentparam; contentparam.Reset();
	contentparam = cgsmtu->contentparam;

	MailRoute::MailToUser(IntToUid(cgsmtu->user_id), SYSTEM_MAIL_REASON_INVALID, contentparam);
}

void SceneManager::OnCrossGuildBattleGetBossInfoResp(NetID net_id, const char *data, int length)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	crossgameprotocal::CrossGameGetGuildBattleBossInfoReq *resp_msg = (crossgameprotocal::CrossGameGetGuildBattleBossInfoReq *)data;
	Scene *scene = this->GetSceneById(resp_msg->scene_id, CROSS_PUBLIC_SCENE_KEY);
	if (NULL != scene)
	{
		SpecialCrossGuildBattle *scene_logical = dynamic_cast<SpecialCrossGuildBattle *>(scene->GetSpecialLogic());
		if (scene_logical)
		{
			scene_logical->SynBossInfoToOrigin(resp_msg->uuid);
		}
	}
}

void SceneManager::OnCrossGuildBattleGetMonsterInfoResp(NetID net_id, const char *data, int length)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	crossgameprotocal::CrossGameGetGuildBattleMonsterInfoReq *resq_msg = (crossgameprotocal::CrossGameGetGuildBattleMonsterInfoReq *)data;
	int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM] = { 0 };
	CrossGuildBattleConfig::Instance().GetGuildBattleSceneIdList(scene_id_list);

	static Protocol::SCCrossGuildBattleGetMonsterInfoResp pro;
	memset(pro.scene_list, 0, sizeof(pro.scene_list));

	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
	{
		Scene *scene = this->GetSceneById(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY);
		if (nullptr != scene && Scene::SCENE_TYPE_CROSS_GUILDBATTLE == scene->GetSceneType())
		{
			SpecialCrossGuildBattle *special = dynamic_cast<SpecialCrossGuildBattle*>(scene->GetSpecialLogic());
			if (nullptr != special)
			{
				pro.scene_list[i] = special->GetSpecialMonsterCount();
			}
		}
	}

	InternalComm::Instance().UserProtocalTransferToGame(resq_msg->uuid, (const char *)&pro, sizeof(pro));
}

void SceneManager::OnCrossGuildBattleSpecialTimeNotice(NetID net_id, const char *data, int length)
{
	ActivityShadow::Instance().OnCrossGuildBattleSpecialTimeNotice();
}

void SceneManager::OnSyncCrossGuildBattleUserScoreRankTop(NetID net_id, const char *data, int length)
{
	//crossgameprotocal::CrossGameGuildBattleUserScoreRank *cggbusr = (crossgameprotocal::CrossGameGuildBattleUserScoreRank *)cggbusr;

}

void SceneManager::OnSyncCrossGuildBattleRegisterReq(NetID net_id, const char *data, int length)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	crossgameprotocal::CrossGameGuildBattleRegisterSync * cggmsg = (crossgameprotocal::CrossGameGuildBattleRegisterSync*)data;

	crossgameprotocal::GameGameGuildBattleRegisterSync  ggrs;

	memset(ggrs.owner_list, 0, sizeof(ggrs.owner_list));

	CrossGuildBattleGuildItem cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	CrossGuildBattleTuanzhangInfo cross_guildbattle_tuanzhang_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	CrossGuildBattleApperanceInfo cross_guildbattle_apperance_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];

	WorldStatus::Instance().GetCrossGuildBattleInfo(cross_guildbattle_list);
	WorldStatus::Instance().GetCrossGuildBattleTuanzhangInfo(cross_guildbattle_tuanzhang_info_list);
	WorldStatus::Instance().GetCrossGuildBattleApperanceInfo(cross_guildbattle_apperance_info_list);

	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
	{
		ggrs.owner_list[i].guild_id.plat_type = cross_guildbattle_list[i].plat_type;
		ggrs.owner_list[i].guild_id.user_id  = IntToUid(cross_guildbattle_list[i].guild_id);
		F_STRNCPY(ggrs.owner_list[i].guild_tuanzhang_name,cross_guildbattle_list[i].guild_tuanzhang_name,sizeof(GameName));
		F_STRNCPY(ggrs.owner_list[i].guild_name,cross_guildbattle_list[i].guild_name,sizeof(GameName));
		ggrs.owner_list[i].prof = cross_guildbattle_tuanzhang_info_list[i].tuanzhang_prof;
		ggrs.owner_list[i].sex = cross_guildbattle_tuanzhang_info_list[i].tuanzhang_sex;
		ggrs.owner_list[i].wuqi_id = cross_guildbattle_apperance_info_list[i].wuqi_id;
		ggrs.owner_list[i].shizhuang_wuqi = cross_guildbattle_apperance_info_list[i].shizhuang_wuqi;
		ggrs.owner_list[i].shizhuang_wuqi_is_special = cross_guildbattle_apperance_info_list[i].shizhuang_wuqi_is_special;
		ggrs.owner_list[i].shizhuang_body = cross_guildbattle_apperance_info_list[i].shizhuang_body;
		ggrs.owner_list[i].shizhuang_body_is_special = cross_guildbattle_apperance_info_list[i].shizhuang_body_is_special;
		ggrs.owner_list[i].halo_used_imageid = cross_guildbattle_apperance_info_list[i].halo_used_imageid;
		ggrs.owner_list[i].qilinbi_used_imageid = cross_guildbattle_apperance_info_list[i].qilinbi_used_imageid;
		ggrs.owner_list[i].toushi_used_imageid = cross_guildbattle_apperance_info_list[i].toushi_used_imageid;
		ggrs.owner_list[i].mask_used_imageid = cross_guildbattle_apperance_info_list[i].mask_used_imageid;
	}

	InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_GLOBAL_NETID,cggmsg->gs_netid, (const char*)&ggrs, sizeof(ggrs));
}

void SceneManager::OnSyncCrossGuildBattleRegister(NetID net_id, const char *data, int length)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	int cross_type = CrossManager::Instance().GetCrossTypeWithNetID(net_id);
	if (!CrossConfig::Instance().CheckCrossServerHasCrossActivity(CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE, cross_type))
	{
		return;//战区不匹配
	}
	
	crossgameprotocal::GameGameGuildBattleRegisterSync * ggrs = (crossgameprotocal::GameGameGuildBattleRegisterSync*) data;

	CrossGuildBattleGuildItem cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	CrossGuildBattleTuanzhangInfo cross_guildbattle_tuanzhang_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	CrossGuildBattleApperanceInfo cross_guildbattle_apperance_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
	{
		cross_guildbattle_list[i].plat_type = ggrs->owner_list[i].guild_id.plat_type;
		cross_guildbattle_list[i].guild_id = UidToInt(ggrs->owner_list[i].guild_id.user_id);
		F_STRNCPY(cross_guildbattle_list[i].guild_tuanzhang_name, ggrs->owner_list[i].guild_tuanzhang_name, sizeof(GameName));
		F_STRNCPY(cross_guildbattle_list[i].guild_name, ggrs->owner_list[i].guild_name, sizeof(GameName));
		cross_guildbattle_tuanzhang_info_list[i].tuanzhang_prof = ggrs->owner_list[i].prof;
		cross_guildbattle_tuanzhang_info_list[i].tuanzhang_sex = ggrs->owner_list[i].sex;
		cross_guildbattle_apperance_info_list[i].wuqi_id = ggrs->owner_list[i].wuqi_id;
		cross_guildbattle_apperance_info_list[i].shizhuang_wuqi = ggrs->owner_list[i].shizhuang_wuqi;
		cross_guildbattle_apperance_info_list[i].shizhuang_wuqi_is_special = ggrs->owner_list[i].shizhuang_wuqi_is_special;
		cross_guildbattle_apperance_info_list[i].shizhuang_body = ggrs->owner_list[i].shizhuang_body;
		cross_guildbattle_apperance_info_list[i].shizhuang_body_is_special = ggrs->owner_list[i].shizhuang_body_is_special;
		cross_guildbattle_apperance_info_list[i].halo_used_imageid = ggrs->owner_list[i].halo_used_imageid;
		cross_guildbattle_apperance_info_list[i].qilinbi_used_imageid = ggrs->owner_list[i].qilinbi_used_imageid;
		cross_guildbattle_apperance_info_list[i].toushi_used_imageid = ggrs->owner_list[i].toushi_used_imageid;
		cross_guildbattle_apperance_info_list[i].mask_used_imageid = ggrs->owner_list[i].mask_used_imageid;
	}

	WorldStatus::Instance().SetCrossGuildBattleInfo(cross_guildbattle_list);
	WorldStatus::Instance().SetCrossGuildBattleTuanzhangInfo(cross_guildbattle_tuanzhang_info_list);
	WorldStatus::Instance().SetCrossGuildBattleApperanceInfo(cross_guildbattle_apperance_info_list);
}

void SceneManager::OnCrossFishingInfoSync(NetID net_id, const char *data, int length)
{
	crossgameprotocal::CrossGameFishingInfoSync *cgfis = (crossgameprotocal::CrossGameFishingInfoSync *)data;

	if (cgfis->rank <= CROSS_FISHING_TOP_RANK_COUNT_MAX)
	{
		//策划需要玩家的称号带上去且提示战力提升，5s玩家从跨服回到本服
		//CrossFishingCalcTopTimerCallback *timer = new CrossFishingCalcTopTimerCallback(cgfis->rank, cgfis->uid);
		//EngineAdapter::Instance().CreateTimer(5000, timer);

		WorldStatus::Instance().SetCrossFishingTopRankUid(cgfis->rank, cgfis->uid);
	}
}

void SceneManager::OnCross1v1ScoreRankReward(NetID net_id, const char * msg, int length)
{
	if (!CrossConfig::Instance().CheckCrossServerHasCrossActivity(CROSS_ACTIVITY_TYPE_CROSS_1V1, CrossManager::Instance().GetCrossTypeWithNetID(net_id)))
	{
		return;
	}

	// 开服天数不到
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	if (opengame_day < Cross1V1CommonConfig::Instance().GetOtherCfg().open_server_day)
	{
		return;
	}

	crossgameprotocal::CrossGameCross1V1ScoreRankReward *cgcsrr = (crossgameprotocal::CrossGameCross1V1ScoreRankReward *)msg;
	CrossRoute::Instance().OnCross1V1ScoreRankReward(cgcsrr);
}

void SceneManager::OnSyncWorldCommonDataChange(NetID net_id, const char *data, int length)
{
	// 消息发送方crossserver的战区类型
	int cross_type = CrossManager::Instance().GetCrossTypeWithNetID(net_id);
	
	crossgameprotocal::CrossGameWorldCommonDataChange *wcdc = (crossgameprotocal::CrossGameWorldCommonDataChange *)data;
	WorldStatus2CommonDataParam &common_data_2 = WorldStatus2::Instance().GetCommonData2();

	switch (wcdc->change_type)
	{
	case crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_1V1:
	{
		if (CrossConfig::Instance().IsHiddenServer())
		{
			return;
		}

		if (!CrossConfig::Instance().CheckCrossServerHasCrossActivity(CROSS_ACTIVITY_TYPE_CROSS_1V1, cross_type))
		{
			return;
		}

		if (crossgameprotocal::WORLD_COMMONDATA_SUB_TYPE_INIT == wcdc->sub_type)  // 只要满足服务器匹配，初始化就要执行
		{
			if (common_data_2.cross1v1_season_count <= 0)
			{
				common_data_2.cross1v1_season_count = 1;
			}
			common_data_2.cross_1v1_cur_join_season_dayid = wcdc->param_2;
			gamelog::g_log_cross.printf(LL_INFO, "SceneManager::OnSyncWorldCommonDataChange: 1v1Init s_dayid:%d", common_data_2.cross_1v1_cur_join_season_dayid);
		}

		// 开服天数不到，不同步
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
		if (opengame_day < Cross1V1CommonConfig::Instance().GetOtherCfg().open_server_day)
		{
			return;
		}

		switch (wcdc->sub_type)
		{
		case crossgameprotocal::WORLD_COMMONDATA_SUB_TYPE_SEANSON:
		{
			//int old_cross1v1_season_count = common_data_2.cross1v1_season_count;
			//common_data_2.cross1v1_season_count = wcdc->param_2;
			if (common_data_2.cross1v1_season_count <= 0)
			{
				common_data_2.cross1v1_season_count = 1;
			}
			common_data_2.cross1v1_season_count++;
			common_data_2.cross_1v1_cur_join_season_dayid = wcdc->param_2;

			// 赛季变更,第一次赛季同步不算做变更
			//if (old_cross1v1_season_count != common_data_2.cross1v1_season_count && common_data_2.cross1v1_season_count > 1)
			{
				this->OnSyncCrossSeasonChange(CROSS_ACTIVITY_TYPE_CROSS_1V1);
			}
		}
		break;

		case crossgameprotocal::WORLD_COMMONDATA_SUB_TYPE_RANK_MIN_SCORE:
		{
			// int season_count = wcdc->param_3; season_count %= CROSS_1V1_SEASON_MAX;
			int season_count = common_data_2.cross1v1_season_count; season_count %= CROSS_1V1_SEASON_MAX;
			if (season_count >= 0 && season_count < CROSS_1V1_SEASON_MAX)
			{
				common_data_2.cross1v1_rank_min_score[season_count] = wcdc->param_2;
			}
		}
		break;
		}
	}
	break;

	case crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_3V3:
	{
		if (CrossConfig::Instance().IsHiddenServer())
		{
			return;
		}

		if (!CrossConfig::Instance().CheckCrossServerHasCrossActivity(CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE, cross_type))
		{
			// 战区不匹配
			return;
		}

		if (crossgameprotocal::WORLD_COMMONDATA_SUB_TYPE_INIT == wcdc->sub_type)  // 只要满足服务器匹配，初始化就要执行
		{
			if (common_data_2.cross3v3_season_count <= 0)
			{
				common_data_2.cross3v3_season_count = 1;
			}
			common_data_2.cross_3v3_cur_join_season_dayid = wcdc->param_2;
			gamelog::g_log_cross.printf(LL_INFO, "SceneManager::OnSyncWorldCommonDataChange: 3v3Init s_dayid:%d", common_data_2.cross_3v3_cur_join_season_dayid);
		}

		// 开服天数不到，不同步
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
		if (opengame_day < Cross3v3Config::Instance().GetOtherCfg().open_server_day)
		{
			return;
		}

		switch (wcdc->sub_type)
		{
		case crossgameprotocal::WORLD_COMMONDATA_SUB_TYPE_SEANSON:
		{
			//int old_cross3v3_season_count = common_data_2.cross3v3_season_count;
			//common_data_2.cross3v3_season_count = wcdc->param_2;
			//common_data_2.cross3v3_day_index = wcdc->param_3;
			if (common_data_2.cross3v3_season_count <= 0)
			{
				common_data_2.cross3v3_season_count = 1;
			}
			common_data_2.cross3v3_season_count++;
			common_data_2.cross_3v3_cur_join_season_dayid = wcdc->param_2;
			
			// 赛季变更,第一次赛季同步不算做变更
			//if (old_cross3v3_season_count != common_data_2.cross3v3_season_count && common_data_2.cross3v3_season_count > 1)
			{
				this->OnSyncCrossSeasonChange(CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE);
			}
		}
		break;

		case crossgameprotocal::WORLD_COMMONDATA_SUB_TYPE_RANK_MIN_SCORE:
		{
			// int season_count = wcdc->param_3; season_count %= CROSS_3V3_SEASON_MAX;
			int season_count = common_data_2.cross3v3_season_count; season_count %= CROSS_3V3_SEASON_MAX;
			if (season_count >= 0 && season_count < CROSS_3V3_SEASON_MAX)
			{
				common_data_2.cross3v3_rank_min_score[season_count] = wcdc->param_2;
			}
		}
		break;
		}
	}
	break;

	case crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_CHENGZHU_CHANGE:
	{
		std::set<Role*> role_list;
		long long user_uuid = wcdc->uuid;
		this->GetAllRole(role_list, [user_uuid](Role *cmp_role) -> bool {
			return UniqueUserIDToLongLong(cmp_role->GetRoleCross()->GetCrossOriginalUniqueUserID()) == user_uuid;
		});

		for (auto role : role_list)
		{
			role->SyncChengZhuSkill(wcdc->param_2);
		}
	}
	break;

	case crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_COMBINE_KILL_BOSS:
		{
			UniqueUserID unique_user_id = LongLongToUniqueUserID(wcdc->uuid);
			int user_id = UidToInt(unique_user_id.user_id);

			CommonDataGlobal *common_global = GlobalRoleManager::Instance().GetCommonDataGlobal(user_id);
			if (nullptr != common_global)
			{
				common_global->AddCombineKillBoss(wcdc->param_2);
			}
		}
		break;

	case crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_GUILD_GONGZI:
	{
		UniqueUserID unique_user_id = LongLongToUniqueUserID(wcdc->uuid);
		int user_id = UidToInt(unique_user_id.user_id);

		std::string reason = "WorldStatus::AddGuildGongziByMonster: monster_id:" + std::to_string(wcdc->param_4);
		GuildManager::Instance().OnAddGongzi(wcdc->param_2, user_id, wcdc->sub_type, wcdc->param_3, reason.c_str());

		//仙盟频道消息
		{
			Guild *guild = GuildManager::Instance().GetGuild(wcdc->param_2);
			if (NULL != guild)
			{
				if (wcdc->sub_type == GUILD_GONGZI_GIVE_TYPE_ONLY_GUILD)
				{
					int length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSChannelChat), sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSChannelChat), "guild_gongzi_guild_killboss_chat",
						wcdc->param_4, wcdc->param_3);
					if (length > 0)
					{
						Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)(gamestring::GAMESTRING_BUF);
						cc->channel_type = chatdef::CHANNEL_TYPE_GUILD_SYSTEM;
						cc->msg_length = length;

						guild->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat), chatdef::ORIGIN_TYPE_GUILD_SYSTEM_MSG);
					}
				}
				else
				{
					UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(user_id);
					if (NULL != user_node)
					{
						GameName role_name; memset(role_name, 0, sizeof(GameName));
						user_node->GetName(role_name);
						int length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSChannelChat), sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSChannelChat), "guild_gongzi_killboss_chat",
							role_name, wcdc->param_4, wcdc->param_3);
						if (length > 0)
						{
							Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)(gamestring::GAMESTRING_BUF);
							cc->channel_type = chatdef::CHANNEL_TYPE_GUILD_SYSTEM;
							cc->msg_length = length;

							guild->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat), chatdef::ORIGIN_TYPE_GUILD_SYSTEM_MSG);
						}
					}
				}
			}
		}
	}
	break;

	case crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_SINGLE_CHAT_LISTEN:
	{
		UniqueUserID target_uuid(wcdc->param_2, IntToUid(wcdc->param_3));
		ChatManager::Instance().OnCrossAddListenRoleInfo(wcdc->uuid, UniqueUserIDToLongLong(target_uuid));
	}
	break;

	case crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_GUILD_KILL_BOSS:
	{
		GuildManager::Instance().OnAddGuildKillBossNum(wcdc->param_2, wcdc->param_3);
	}
	break;

	case crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_CROSS_CHALLENGEFIELD:
	{
		UniqueUserID unique_user_id = LongLongToUniqueUserID(wcdc->uuid);
		int user_id = UidToInt(unique_user_id.user_id);

		CommonDataGlobal *common_global = GlobalRoleManager::Instance().GetCommonDataGlobal(user_id);
		if (nullptr != common_global)
		{
			common_global->AddCrossChallengefieldRankImageId(wcdc->param_4, wcdc->param_2, wcdc->param_3);
		}
	}
	break;

	}
}

void SceneManager::OnCrossWorldLevel(NetID net_id, const char *data, int length)
{
	crossgameprotocal::CrossGameCrossWorldLevelInfo *cgqis = (crossgameprotocal::CrossGameCrossWorldLevelInfo *)data;

	WorldShadow::Instance().SetHiddenWorldLevel(cgqis->world_level);
}

void SceneManager::OnCrossGameLieKunFBKillZoneBossSync(NetID net_id, const char *data, int length)
{
	crossgameprotocal::CrossGameLieKunFBKillZoneBossSync *cglkfbmis = (crossgameprotocal::CrossGameLieKunFBKillZoneBossSync *)data;

	WorldStatus2::Instance().SetCrossLieKunFBGuildId(cglkfbmis->zone, cglkfbmis->kill_zone_boss_guild_id);
}

void SceneManager::OnCrossGameLieKunFBTopRewardSync(NetID net_id, const char *data, int length)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	// 消息发送方crossserver的战区类型
	int cross_type = CrossManager::Instance().GetCrossTypeWithNetID(net_id);
	if (!CrossConfig::Instance().CheckCrossServerHasCrossActivity(CROSS_ACTIVITY_TYPE_LIEKUN_FB, cross_type))
	{
		// 战区不匹配
		return;
	}

	GuildID liekun_guild_id = WorldStatus2::Instance().GetCrossLieKunFBTopOneGuildId();

	WorldStatus2::Instance().SetLieKunTopRankGuildId(liekun_guild_id);

	Guild *guild = GuildManager::Instance().GetGuild(liekun_guild_id);
	if (NULL == guild)
	{
		return;
	}

	// 发放奖励
//	const CrossLieKunFbOtherCfg &other_cfg = CrossLieKunFbConfig::Instance().GetOtherCfg();

	// 获取帮派所有成员
	static int member_uid_list[MAX_MEMBER_COUNT];
	memset(member_uid_list, 0, sizeof(member_uid_list));

	// 发送奖励
	int member_count = guild->GetGuildMemberManager().GetMemberList(member_uid_list);
	for (int i = 0; i < member_count && i < MAX_MEMBER_COUNT; ++i)
	{
		//gamecommon::SendMultiMailToRole(IntToUid(member_uid_list[i]), &other_cfg.title_item, 1, NULL, false, "cross_liekunfb_top_reward");

		Role *member_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
		if (member_role != NULL)
		{
			WorldStatus2::Instance().SyncCrossActivityTitleInfo(member_role, SYNC_REASON_ACTIVITY);
		}
	}
}

void SceneManager::OnCrossGameMessBattleRankTopSync(NetID net_id, const char *data, int length)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	crossgameprotocal::CrossGameMessBattleInfoSync * cgmbi = (crossgameprotocal::CrossGameMessBattleInfoSync *)data;

	WorldStatus2::Instance().SetMessBattleTopRankUid(cgmbi->rank, cgmbi->uid);
}
void SceneManager::OnCrossGameNightFightRankTopSync(NetID net_id, const char *data, int length)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	crossgameprotocal::CrossGameNightFightInfoSync * cgnfi = (crossgameprotocal::CrossGameNightFightInfoSync *)data;
	
	WorldStatus2::Instance().SetNightFightTopRankUid(cgnfi->rank, cgnfi->uid);
}

void SceneManager::OnSyncCrossRankFirst(NetID net_id, const char *data, int length)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	// 消息发送方crossserver的战区类型
	int cross_type = CrossManager::Instance().GetCrossTypeWithNetID(net_id);
	if (!CrossConfig::Instance().CheckCrossServerHasCrossActivity(CROSS_FUNCTION_TYPE_CROSS_RANK, cross_type))
	{
		// 战区不匹配
		return;
	}

	crossgameprotocal::CrossGameSyncRankFirst *msg = (crossgameprotocal::CrossGameSyncRankFirst *)data;
	switch (msg->rank_type)
	{
	case CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY:
	{
		WorldStatus3::Instance().SetCrossAddCapRankFirst(msg->uid_list);
	}
	break;

	case CROSS_PERSON_RANK_TYPE_ADD_CHARM:
	{
		WorldStatus3::Instance().SetCrossAddCharmRankFirst(msg->uid_list);
	}
	break;

	case CROSS_PERSON_RANK_TYPE_GUILD_KILL_BOSS:
	{
		WorldStatus3::Instance().SetCrossGuildKillRankFirst(msg->uid_list);

		for (unsigned int i = 0; i < static_array_size(msg->uid_list); i ++)
		{
			Guild *guild = GuildManager::Instance().GetGuild(msg->uid_list[i]);
			if (nullptr != guild)
			{
				guild->ResetGuildKillBossNum();
			}
		}
	}
	break;

	default:
		break;
	}
}

void SceneManager::OnSyncCrossCoupleRankFirst(NetID net_id, const char *data, int length)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	// 消息发送方crossserver的战区类型
	int cross_type = CrossManager::Instance().GetCrossTypeWithNetID(net_id);
	if (!CrossConfig::Instance().CheckCrossServerHasCrossActivity(CROSS_FUNCTION_TYPE_CROSS_RANK, cross_type))
	{
		// 战区不匹配
		return;
	}

	crossgameprotocal::CrossGameSyncCoupleRankFirst *msg = (crossgameprotocal::CrossGameSyncCoupleRankFirst *)data;

	switch (msg->rank_type)
	{
	case CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP:
	{
		WorldStatus3::Instance().SetCrossQingyuanCapRankFirstMale(msg->male_uuid_list);
		WorldStatus3::Instance().SetCrossQingyuanCapRankFirstFemale(msg->female_uuid_list);
	}
	break;

	default:
		break;
	}
}

void SceneManager::OnCrossCommonBossInfoChange(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameCrossCommonBossInfoChange *sccbi = (crossgameprotocal::CrossGameCrossCommonBossInfoChange *)msg;

	BossFamilyManager::Instance().OnCrossCommonBossInfoChange(sccbi);
}

void SceneManager::OnSyncCrossCommonBossInfoReq(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameSyncCrossCommonBossInfoReq *sccbir = (crossgameprotocal::CrossGameSyncCrossCommonBossInfoReq *)msg;
	BossFamilyManager::Instance().OnCrossCommonBossInfoSyncReq(sccbir->global_netid);
}

void SceneManager::OnSyncCrossCommonBossInfo(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameSyncCrossCommonBossInfo *sccbi = (crossgameprotocal::CrossGameSyncCrossCommonBossInfo *)msg;
	BossFamilyManager::Instance().OnSyncCrossCommonBossInfo(sccbi);
}

void SceneManager::OnCrossCommonBossDropInfo(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameSyncCrossCommonBossDropInfo *sccbdi = (crossgameprotocal::CrossGameSyncCrossCommonBossDropInfo *)msg;
	if (sccbdi->uuid > 0 && sccbdi->item_id > 0)
	{
		int uid = UidToInt(LongLongToUniqueUserID(sccbdi->uuid).user_id);
		DropLogManager::Instance().AddLog(uid, sccbdi->role_name, (int)sccbdi->monster_id, sccbdi->item_id, sccbdi->item_num, sccbdi->scene_id,sccbdi->xianpin_type_list, sizeof(sccbdi->xianpin_type_list)/sizeof(sccbdi->xianpin_type_list[0]));
	}
}

void SceneManager::OnSyncRoleNumToHidden(NetID net_id, const char *msg, int length)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	//printf("\n \\ OnSyncRoleNumToHidden... \n");

	crossgameprotocal::GameCrossSyncRoleNumToGame *gcsrntg = (crossgameprotocal::GameCrossSyncRoleNumToGame *)msg;

	m_unique_server_role_num.erase(gcsrntg->unique_serverId);

	ServerRoleNum role_num;
	for (int i = 1; i <= MAX_ROLE_LEVEL; i++)
	{
		role_num.role_num_list[i] = gcsrntg->role_num_list[i];
	}

	m_unique_server_role_num[gcsrntg->unique_serverId] = role_num;
}

void SceneManager::OnSyncGameServerTimeInfo(NetID net_id, const char *msg, int length)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	crossgameprotocal::CrossGameTimeInfoToHidden *cgtith = (crossgameprotocal::CrossGameTimeInfoToHidden *)msg;
	CrossManager::Instance().OnGameServerTimeInfo(cgtith);
}

void SceneManager::OnCrossGameSyncCrossRandActivityData(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameSyncCrossRandActivityData *srad = (crossgameprotocal::CrossGameSyncCrossRandActivityData *)msg;
	CrossRAManagerLocal::Instance().OnSyncActivityData(srad, length);
}

void SceneManager::OnGameCrossGuildBattleSos(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::GameGameCrossGuildBattleSos * gdsos = (crossgameprotocal::GameGameCrossGuildBattleSos *)msg;

	static Protocol::SCGuildMemberSos gms;
	gms.sos_type = gdsos->sos_type;
	gms.member_uid = gdsos->member_uid;
	F_STRNCPY(gms.member_name, gdsos->member_name, sizeof(GameName));
	gms.member_scene_id = gdsos->member_scene_id;
	gms.member_pos_x = gdsos->member_pos_x;
	gms.member_pos_y = gdsos->member_pos_y;
	gms.enemy_uid = 0;
	memset(gms.enemy_name, 0, sizeof(gms.enemy_name));
	gms.enemy_camp = 0;
	gms.enemy_guild_id = INVALID_GUILD_ID;
	memset(gms.enemy_guild_name, 0, sizeof(gms.enemy_guild_name));

	Guild * guild = GuildManager::Instance().GetGuild(gdsos->guild_id);
	if (nullptr != guild)
	{
		guild->SendMessageToGuildAll(Protocol::GUILD_NOTIFY_TYPE_MEMBER_SOS, (const char*)&gms, sizeof(gms));

		{
			// 需要发送消息
			if (1 == gdsos->sos_sysmsg)
			{
				// 仙盟系统消息
				//static Protocol::SCSystemMsg sm;
				//int sendlen = 0;
				//
				//int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "cross_guild_sos_sysmsg",   // dsdd
				//	gdsos->member_uid, gdsos->member_name, gdsos->member_camp, gdsos->member_scene_id);
				//
				//if (length > 0)
				//{
				//	if (World::GetInstWorld()->GetSceneManager()->CreateSystemMsg(&sm, &sendlen, gstr::GSTR_BUF, length, SYS_MSG_ONLY_CHAT_GUILD))
				//	{
				//		guild->SendMessageToGuildAll(Protocol::GUILD_NOTIFY_TYPE_MEMBER_SOS, (const char*)&sm, sendlen);
				//	}
				//}
				// 系统仙盟聊天
				static char buff[2048];
				int length = gstr::SysMsgContent(buff + sizeof(Protocol::CSChannelChat), sizeof(buff), "cross_guild_sos_sysmsg",   // dsd sddddd
					gdsos->member_uid, gdsos->member_name, gdsos->member_camp, 
					gdsos->member_scene_name, gdsos->member_pos_x, gdsos->member_pos_y, gdsos->member_scene_id, gdsos->member_scene_key, 1); // 跨服过来的位置标记为1

				if (length > 0)
				{
					Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)buff;
					cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
					cc->msg_length = length;

					guild->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat));
				}
			}
		}
	}

}

void SceneManager::OnGameCrossKillCrossBoss(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::GameGameKillCrossBoss * klboss = (crossgameprotocal::GameGameKillCrossBoss *)msg;

	Guild * guild = GuildManager::Instance().GetGuild(klboss->guild_id);
	if(nullptr != guild)
	{
		if (klboss->scene_type == Scene::SCENE_TYPE_CROSS_BOSS || klboss->scene_type == Scene::SCENE_TYPE_CROSS_MIZANG_BOSS)
		{
			guild->AddMemberKillCrossBoss(klboss->uid, 1);
		}
		
		if (klboss->scene_type == Scene::SCENE_TYPE_CROSS_BOSS)
		{
			guild->AddCrossGoalkillCrossBoss(klboss->uid, 1);
		}

		if (klboss->scene_type == Scene::SCENE_TYPE_CROSS_BIANJINGZHIDI)
		{
			guild->AddCrossGoalkillBaiZhanBoss(klboss->uid, 1);
		}
	}
}

void SceneManager::OnPickGuildRareItemInHiddenGs(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::PickGuildRareItemInHiddenGs *info = (crossgameprotocal::PickGuildRareItemInHiddenGs *)msg;

	GuildRareLogItem temp_item;

	temp_item.uuid = info->item.uuid;
	temp_item.item_id = info->item.item_id;
	temp_item.item_num = info->item.item_num;
	temp_item.is_from_gift = info->item.is_from_gift;
	temp_item.gift_item_id = info->item.gift_item_id;
	temp_item.monster_id = info->item.monster_id;
	temp_item.scene_id = info->item.scene_id;
	if (info->item.role_name != NULL)
	{
		strcpy(temp_item.role_name, info->item.role_name);
	}
	temp_item.timestamp = info->item.timestamp;
	memcpy(temp_item.xianpin_type_list, info->item.xianpin_type_list, sizeof(temp_item.xianpin_type_list));

	GuildRareLogManager::Instance().AddLog(info->guild_id, temp_item);
}

void SceneManager::OnSyncRareItemFromOriginalGs(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::BroadcastGuildRareInfo *info = (crossgameprotocal::BroadcastGuildRareInfo *)msg;

	std::set<Role*> role_list;

	World::GetInstWorld()->GetSceneManager()->GetAllRole(role_list, [=](Role *cmp_role) -> bool {
		return cmp_role->GetGuildID() == info->guild_id && cmp_role->GetRoleCross()->GetOriginPlatType() == info->plat_type;
	});

	for (auto guild_member_role : role_list)
	{
		if (NULL == guild_member_role)
		{
			continue;
		}

		GuildRareLogItem temp_item;
		temp_item.uuid = info->item.uuid;
		temp_item.item_id = info->item.item_id;
		temp_item.item_num = info->item.item_num;
		temp_item.is_from_gift = info->item.is_from_gift;
		temp_item.gift_item_id = info->item.gift_item_id;
		temp_item.monster_id = info->item.monster_id;
		temp_item.scene_id = info->item.scene_id;
		if (info->item.role_name != NULL)
		{
			strcpy(temp_item.role_name, info->item.role_name);
		}
		temp_item.timestamp = info->item.timestamp;
		memcpy(temp_item.xianpin_type_list, info->item.xianpin_type_list, sizeof(temp_item.xianpin_type_list));

		GuildRareLogManager::Instance().SendOneLogToRole(guild_member_role, temp_item);
	}
}

void SceneManager::OnBroadcastCrossRankToGuildChat(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::SyncCrossRankInfoToOriginGs *info = (crossgameprotocal::SyncCrossRankInfoToOriginGs *)msg;

	EventHandler::Instance().OnBroadCastCrossRankTop3ToGuild(info->cross_rank_type, info->param1, info->param2, info->rank_value, info->rank_pos);
}

void SceneManager::OnRecvAndTransferCrossSystemMsg(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::SyncCrossSystemMsgToOriginServer *info = (crossgameprotocal::SyncCrossSystemMsgToOriginServer *)msg;
	int base_protocal_length = sizeof(crossgameprotocal::SyncCrossSystemMsgToOriginServer);
	if (length < 0 || length != info->msg_length + base_protocal_length)
	{
		return;
	}

	if (info->uuid != 0)  // 转发给对应的人或公会
	{
		if (SYS_MSG_ONLY_CHAT_GUILD == info->msg_type) // 转发给公会
		{
			UniqueUserID guild_id = LongLongToUniqueUserID(info->uuid);
			int msg_ptr_offset = sizeof(Protocol::SCSystemMsg) - PROTOCAL_MAX_SYSTEM_MSG_LENGTH;
			if (info->msg_length > msg_ptr_offset)
			{
				this->SystemMsgToGuild(UidToInt(guild_id.user_id), (const char*)msg + base_protocal_length + msg_ptr_offset, info->msg_length - msg_ptr_offset, 
					SYS_MSG_ONLY_CHAT_GUILD);
			}
		}
	}
	else
	{
		this->SendToAllUser((const char*)msg + sizeof(crossgameprotocal::SyncCrossSystemMsgToOriginServer), info->msg_length);
	}
}

void SceneManager::OnCrossRequestGuildChat(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameRequestGuildChat *cgrgc = (crossgameprotocal::CrossGameRequestGuildChat *)msg;
	if (length < 0 || cgrgc->msg_length <= 0 || length < cgrgc->msg_length || (size_t)cgrgc->msg_length >= sizeof(cgrgc->msg_buff))
	{
		return;
	}

	static const int PLAT_TYPE = LocalConfig::Instance().GetPlatType();
	UniqueUserID guild_id = LongLongToUniqueUserID(cgrgc->guild_id);
	
	if (PLAT_TYPE != guild_id.plat_type)
	{
		return;
	}

	Guild * guild = GuildManager::Instance().GetGuild(UidToInt(guild_id.user_id));
	if (nullptr == guild)
	{
		return;
	}

	static char buff[2048];
	if (cgrgc->msg_length >= (int)sizeof(buff) - (int)sizeof(Protocol::CSChannelChat))
	{
		return;
	}

	Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)buff;
	cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
	cc->msg_length = cgrgc->msg_length;
	F_STRNCPY(buff + sizeof(Protocol::CSChannelChat), cgrgc->msg_buff, cgrgc->msg_length);

	if (chatdef::ORIGIN_TYPE_GUILD_SYSTEM_CHAT == cgrgc->msg_origin_type)
	{
		guild->GetGuildMemberManager().GuildSystemChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat));
	}
	else
	{
		guild->GetGuildMemberManager().GuildleaderGuildChat(cc, cc->msg_length + sizeof(Protocol::CSChannelChat));
	}
}

void SceneManager::OnGameCrossGuildMemberBeKill(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::GameGameCrossGuildMemberBeKill * ggcgmbk = (crossgameprotocal::GameGameCrossGuildMemberBeKill *)msg;

	Guild * guild = GuildManager::Instance().GetGuild(ggcgmbk->be_killer_guild_id);
	if (nullptr != guild)
	{
		GuildEnemyInfo info;

		info.enemy_uuid = ggcgmbk->killer_uuid;
		F_STRNCPY(info.enemy_name, ggcgmbk->killer_name, sizeof(GameName));
		info.kill_score = 1;  // 外部这里作为新玩家，设为1
		info.last_kill_timestamp = (unsigned int)EngineAdapter::Instance().Time();

		guild->AddGuildEnemy(info);
	}
}

void SceneManager::OnCrossGameGetGuildEnemyList(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameGetGuildEnemyList * cgggel = (crossgameprotocal::CrossGameGetGuildEnemyList *)msg;

	Guild * guild = GuildManager::Instance().GetGuild(cgggel->guild_id);
	if (nullptr != guild)
	{
		guild->SendGuildEnemy(nullptr, cgggel->request_uuid);
	}
}

void SceneManager::OnSyncCrossChallengefiledInfoReq(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameSyncCrossChallengefieldReq *sccbir = (crossgameprotocal::CrossGameSyncCrossChallengefieldReq *)msg;
	ChallengeField::Instance().OnSyncChallengeUserInitDataToGame(sccbir->global_netid);
}

void SceneManager::OnSyncCrossChallengefiledInfo(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::SyncCrossChallengefieldDataInfo *sccbir = (crossgameprotocal::SyncCrossChallengefieldDataInfo *)msg;

	switch (sccbir->sync_type)
	{
	case crossgameprotocal::SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_TO_INIT_GAME:
	{
		CrossChallengeField::Instance().OnSyncInitCrossChallengefield(sccbir);
	}
	break;

	case crossgameprotocal::SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_UPDATE_TO_HIDDEN:
	{
		ChallengeField::Instance().OnSyncOrignalChallengefieldInfo(sccbir);
	}
	break;

	case crossgameprotocal::SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_RANK_CHANGE_TO_HIDDEN:
	{
		ChallengeField::Instance().OnSyncChallengefieldRankInfo(sccbir);
	}
	break;

	case crossgameprotocal::SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_UPDATE_TO_GAME:
	{
		CrossChallengeField::Instance().OnSyncUpdateCrossChallengefield(sccbir);
	}
	break;

	}
}

void SceneManager::OnSyncCrossHusongShuijingGatherInfo(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossHusongShuijingGatherInfoReq * hs_req = (crossgameprotocal::CrossHusongShuijingGatherInfoReq *)msg;
	const HusongSJOtherCfg other_cfg = HusongShuijingConfig::Instance().GetOtherCfg();
	SpecialCrossHusongShuijing * special_logic = World::GetInstWorld()->GetSceneManager()->GetSpcialCrossHusongShuijing(other_cfg.scene_id, COMMON_SCENE_KEY, false);
	if (NULL != special_logic)
	{
		special_logic->SyncShuijingInfo(hs_req->uuid);
	}
}
