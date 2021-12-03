#include "crossserver.h"

#include "ilogmodule.h"
#include "engineadapter.h"
#include "internalcomm.h"
#include "servercommon/servercommon.h"
#include "crossservermodule.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/serverconfig/configstruct.h"
#include "crossuser/crossuser.h"
#include "servercommon/struct/roleallparam.hpp"
#include "servercommon/errornum.h"
#include "crossactivity/impl/crossactivityxiuluotower.hpp"
#include "protocal/msgcross.h"
#include <map>
#include "servercommon/serverconfig/localconfig.hpp"
#include "crossactivity/impl/crossactivityteamfb.hpp"
#include "config/sharedconfig.h"
#include "crossactivity/impl/crossactivityxiuluotower.hpp"
#include "crossactivity/impl/crossactivityfishing.hpp"
#include "crossactivity/impl/crossactivitymessbattle.hpp"
#include "crossactivity/impl/crossactivitynightfight.hpp"
#include "crossrandactivity/impl/crossrandactivitychongzhirank.hpp"
#include "crossrandactivity/impl/crossrandactivityconsumerank.hpp"
#include "crossactivity/impl/crossactivitycrossbianjingzhidi.hpp"

void CrossServer::InitGSHandler()
{
	memset(m_on_gs_handler_list, 0, sizeof(m_on_gs_handler_list));

	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_START_CROSS_REQ - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnStartCrossReq;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_REGISTER_GAME_SERVER - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnRegisterGameServer;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_SAVE_CROSS_ROLE_RET - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnSaveCrossRoleRet;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_USER_MSG - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnUserMsg;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_USER_LOGIN - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossUserLogin;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_USER_LOGOUT - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossUserLogout;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_KICK_CROSS_USER - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnKickCrossUser;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_TRANSFER_TO_GAME - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnGameCrossRransferToGame;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_ACTIVITY_FORCE_NEXTSTATE - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnActivityForceNextState;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_SYNC_XIULUOTOWER_RESULT - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnSyncXiuluotaResult;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_SYNC_XIULUOTOWER_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnSyncXiuluotaInfo;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_SYNC_DROP_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossXiuluoTowerSyncDropLog;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_SYNC_XIULUOTOWER_RANK_TITLE - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossXiuluoTowerSyncRankTitle;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_GUILD_BATTLE_SYNC_DROP_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossGuildBattleSyncDropLog;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_SYNC_CROSS_PERSON_RANK_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnSyncCrossPersonRankInfo;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_QUERY_ROLE_INFO_REQUEST - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnQueryRoleInfoRequest;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_USER_PROTOCAL_TRANSFER_TO_GAME - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnUserProtocalTransferToGame;
	m_on_gs_handler_list[crossgameprotocal::MT_CROSS_GAME_SYNC_ROLE_DATA - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnSyncRoleData;
	m_on_gs_handler_list[crossgameprotocal::MT_CROSS_GAME_CHANNEL_CHAT - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossChannelChat;
	m_on_gs_handler_list[crossgameprotocal::MT_CROSS_GAME_RANK_INFO_REQ - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossRankInfoReq;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_SEND_MAIL_TO_USER - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnGameCrossSendMailToUser;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_GM_ADD_WEEK - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnGameCrossGmAddWeek;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_WORLD_COMMON_DATA_CHANGE - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnSyncWorldCommonDataChange;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_FORBID_TALK - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnForbidCrossUserTalk;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_1V1_MATCH_REQ - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossActivity1V1MatchReq;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_1V1_FIGHT_RESULT - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCross1V1FightResult;
	m_on_gs_handler_list[crossgameprotocal::MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_REQ - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossMultiuserChallengeMatchingReq;
	m_on_gs_handler_list[crossgameprotocal::MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_CANCEL - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossMultiuserChallengeMatchingCancel;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_MULTIUSER_CHALLENGE_RANK_LIST - crossglobalprotocal::MT_CROSSGLOBAL_BEGIN] = &CrossServer::OnGlobalSyncCrossMultiuserChallengeRankList;
	m_on_gs_handler_list[crossgameprotocal::MT_GAMECROSS_CROSS_MULTIUSER_CHALLENGE_MATCH_RESULT - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossMultiuserChallengeMatchResult;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_TUANZHAN_SYNC_FIGHT_RESULT - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossTuanzhanSyncFightResult;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_PASTURE_INFO_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnPastureInfoSync;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_BOSS_SYNC_PLAYER_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossBossSyncPlayerInfo;
	m_on_gs_handler_list[crossgameprotocal::MT_CROSS_TEAM_FB_USER_OPTION_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossTeamFBUserOptionReq;
	m_on_gs_handler_list[crossgameprotocal::MT_CROSS_TEAM_FB_ROOM_OPTION_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossTeamFBRoomOptionReq;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_GUILD_BATTLE_INFO_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnGuildBattleInfoSync;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_GUILD_BATTLE_RANK_LOG_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnGuildBattleRankLogInfoSync;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_GET_GUILD_BATTLE_RANK_LOG_REQ - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnGuildBattleGetRankLogInfoReq;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_COMMON_INFO_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCommonInfoSync;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_COMMON_OPEAR_REQ_CS - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossCommonOperaReq;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_COMMON_ATTEND_INFO_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCommonAttendInfoSync;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_FISHING_INFO_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossFishingInfoSync;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_1V1_MATCH_RESULT_REQ - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossGameCross1V1MatchResult;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_1V1_ROLE_RANK_BASE_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnSyncCross1V1RoleRankBaseInfo;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_1V1_RANK_LIST - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnSyncCross1V1RoleRankList;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossMultiuserChallengeGetMatchingState;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_MESS_BATTLE_INFO_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossMessBattleInfoSync;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_NIGHT_FIGHT_INFO_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossNightFightInfoSync;

	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_LIEKUN_FB_INFO_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnCrossLieKunFBInfoSync;
	m_msg_handler_list[Protocol::MT_CROSS_LIEKUN_FB_REQ_CS] = HandlerItem(&CrossServer::OnCrossLieKunFBReq, sizeof(Protocol::CSCrossLieKunFBReq));
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_LIEKUN_FB_KILL_ZONE_BOSS_SYNC - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnGameCrossLieKunFBKillZoneBossSync;


	m_msg_handler_list[Protocol::MT_CROSS_1V1_MATCH_QUERY_CS] = HandlerItem(&CrossServer::OnCross1V1MatchQuery, sizeof(Protocol::CSCross1v1MatchQuery));
	m_msg_handler_list[Protocol::MT_CROSS_1V1_WEEK_RECORD_QUERY_CS] = HandlerItem(&CrossServer::OnCross1V1RecordQuery, sizeof(Protocol::CSCross1v1WeekRecordQuery));
	m_msg_handler_list[Protocol::MT_CROSS_1V1_GET_RANK_LIST_CS] = HandlerItem(&CrossServer::OnCross1V1GetRankList, sizeof(Protocol::CSGetCross1V1RankList));
	m_msg_handler_list[Protocol::MT_GET_MULTIUSER_CHALLENGE_RANK_LIST_CS] = HandlerItem(&CrossServer::OnGetMultiuserChallengeRankList, sizeof(Protocol::CSGetMultiuserChallengeRankList));
	m_msg_handler_list[Protocol::MT_MULTIUSER_CHALLENGE_CHECK_USER_HAS_MATCH_CS] = HandlerItem(&CrossServer::OnCheckMultiuserChallengeUserHasMatch, sizeof(Protocol::CSCheckMultiuserChallengeHasMatch));
	m_msg_handler_list[Protocol::MT_CROSS_GET_PERSON_RANK_LIST_CS] = HandlerItem(&CrossServer::OnGetCrossPersonRankList, sizeof(Protocol::CSCrossGetPersonRankList));
	m_msg_handler_list[Protocol::MT_CROSS_XIULUO_TOWER_GET_DROP_LOG_CS] = HandlerItem(&CrossServer::OnCrossXiuluoTowerGetDropLog, sizeof(Protocol::CSCrossXiuluoTowerDropLog));
	m_msg_handler_list[Protocol::MT_CROSS_GUILD_BATTLE_GET_DROP_LOG_CS] = HandlerItem(&CrossServer::OnCrossGuildBattleGetDropLog, sizeof(Protocol::CSCrossGuildBattleDropLog));
	m_msg_handler_list[Protocol::MT_CROSS_GUILD_BATTLE_GET_MONSTER_INFO_CS] = HandlerItem(&CrossServer::OnCrossGuildBattleGetMonsterInfo, sizeof(Protocol::CSCrossGuildBattleGetMonsterInfoReq));

	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_GM_OPERA - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnGmOpera;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_SYNC_CROSS_COUPLE_RANK_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnSyncCrossCoupleRankInfo;
	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_SYNC_COUPLE_RANK_DIVORCE - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnDivorceSync;

	m_on_gs_handler_list[crossgameprotocal::MT_GAME_CROSS_SYNC_CROSS_RAND_ACTIVITY_DATA - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnSyncCrossRandActivityData;

	m_msg_handler_list[Protocol::MT_CROSS_RA_CHONGZHI_RANK_GET_RANK_CS] = HandlerItem(&CrossServer::OnCrossRAChongzhiRankGetRank, sizeof(Protocol::CSCrossRAChongzhiRankGetRank));
	m_msg_handler_list[Protocol::MT_CROSS_RA_CONSUME_RANK_GET_RANK_CS] = HandlerItem(&CrossServer::OnCrossRAConsumeRankGetRank, sizeof(Protocol::CSCrossRAConsumeRankGetRank));

	// 跨服边境之地
	m_msg_handler_list[Protocol::MT_CROSS_SERVER_BIANJINGZHIDI_BOSS_INFO_CS] = HandlerItem(&CrossServer::OnCrossBianJingZhiDiBossInfoReq, sizeof(Protocol::CSCrossServerBianJingZhiDiBossInfoReq));
	m_on_gs_handler_list[crossgameprotocal::MT_HIDDEN_CROSS_BIANJINGZHIDI_BOSS_INFO - crossgameprotocal::MT_CROSS_GAME_BEGIN] = &CrossServer::OnSyncCrossBianJingZhiDiBossInfo;
}

void CrossServer::OnStartCrossReq(NetID global_netid, const char *data, int length)
{
	static crossgameprotocal::GameCrossStartCrossReq complete_gcscr;
	if (length <= (int)sizeof(complete_gcscr) - (int)sizeof(complete_gcscr.role_cross_param_data) || length > (int)sizeof(complete_gcscr)) return;

	memcpy(&complete_gcscr, data, length);

	crossgameprotocal::GameCrossStartCrossReq *gcscr = &complete_gcscr;
	gcscr->pname[sizeof(gcscr->pname) - 1] = 0;

	if (gcscr->cross_activity_type <= CROSS_ACTIVITY_TYPE_INVALID || gcscr->cross_activity_type >= CROSS_ACTIVITY_TYPE_RESERVED ||
		gcscr->plat_type < 0 || 0 == gcscr->pname[0] || gcscr->uid <= 0 || 
		gcscr->role_cross_param_data_len <= 0 || gcscr->role_cross_param_data_len > (int)sizeof(gcscr->role_cross_param_data))
	{
		return;
	}

	if (IsCrossUser(IntToUid(gcscr->uid))) return;

	static RoleAllParam all_param;
	all_param.Reset();

	TLVUnserializer instream;
	instream.Reset(gcscr->role_cross_param_data, gcscr->role_cross_param_data_len);
	if (!all_param.Unserialize(instream, true))
	{
		m_log.printf(LL_INFO, "[CrossServer::OnSaveRoleInfoReq RoleAllParam Unserialize fail] plat:%d role_id:%d", gcscr->plat_type, gcscr->uid);
		return;
	}

	UserID original_user_id = IntToUid(gcscr->uid);
	static GameName original_user_name = {0};
	F_STRNCPY(original_user_name, all_param.role_param.role_name, sizeof(original_user_name));

	UniqueUserID original_unique_user_id(gcscr->plat_type, original_user_id);

	if (0 == gcscr->is_force && !m_cross_activity_manager.CheckCanStartCross(gcscr->cross_activity_type, original_unique_user_id, 0))
	{
		this->NoticeNum(global_netid, original_user_id, errornum::EN_CAN_NOT_START_CROSS);
		return;
	}

	//============================
	static PlatTypeUnion plat_union;
	plat_union.id = gcscr->plat_type;

	static char plat_type_str[4];
	for (int i = 0; i < 3; ++i)
	{
		plat_type_str[i] = plat_union.spid[i + 1];	// 根据localconfig.cpp写入的方式读取出来
	}
	plat_type_str[3] = '\0';

	static PlatName new_pname = {0};
	static GameName new_user_name = {0};
	SNPRINTF(new_pname, sizeof(new_pname), "%s|%s", plat_type_str, gcscr->pname);
	//============================

	F_STRNCPY(new_user_name, all_param.role_param.role_name, sizeof(new_user_name));

	F_STRNCPY(all_param.role_param.role_name, new_user_name, sizeof(all_param.role_param.role_name));

	{
		all_param.role_param.last_scene_id = all_param.role_param.scene_id;
		all_param.role_param.last_scene_x = all_param.role_param.scene_x;
		all_param.role_param.last_scene_y = all_param.role_param.scene_y;

		Posi pos(0, 0);
		all_param.role_param.scene_id = m_cross_activity_manager.GetWaitSceneID(original_unique_user_id, gcscr->cross_activity_type, &pos);
		all_param.role_param.scene_x = pos.x;
		all_param.role_param.scene_y = pos.y;
	}

	{
		static crossgameprotocal::CrossGameSaveCrossRoleInfo cgscri;
		cgscri.cross_activity_type = gcscr->cross_activity_type;
		cgscri.original_plat_type = gcscr->plat_type;
		cgscri.original_uid = UidToInt(original_user_id);
		F_STRNCPY(cgscri.original_username, original_user_name, sizeof(GameName));
		F_STRNCPY(cgscri.pname, new_pname, sizeof(PlatName));

		TLVSerializer outstream;
		outstream.Reset(cgscri.role_cross_param_data, sizeof(cgscri.role_cross_param_data));

		if (!all_param.Serialize(outstream, true)) return;
		cgscri.role_cross_param_data_len = outstream.Size();

		int send_len = sizeof(cgscri) - sizeof(cgscri.role_cross_param_data) + cgscri.role_cross_param_data_len;
		InternalComm::Instance().NetSend(m_hidden_gs_netid, (const char *)&cgscri, send_len);
	}

	m_log.printf(LL_INFO, "[CrossServer::OnSaveRoleInfoReq NetworkRecv][plat:%d role_id:%d package size:%u]", gcscr->plat_type, gcscr->uid, gcscr->role_cross_param_data_len);
}

void CrossServer::OnRegisterGameServer(NetID gs_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossRegisterGameServer *cgrgs = (crossgameprotocal::GameCrossRegisterGameServer *)data;
	if (cgrgs->cross_protocal_ver > CROSS_PROTOCAL_VERSION) 
	{
		InternalComm::Instance().NetDisconnect(gs_netid);
		return;
	}

	for (int i = 0; i < cgrgs->count && i < crossgameprotocal::GameCrossRegisterGameServer::MAX_SERVER_ID_NUM; ++i)
	{
		if (CROSS_USER_DB_INDEX == cgrgs->server_id_list[i])
		{
			UniqueServerID gs_unique_id(cgrgs->plat_type, cgrgs->server_id_list[i]);

			m_hidden_gs_role_all_param_size = cgrgs->role_all_param_size;
			m_hidden_gs_unique_id = gs_unique_id;
			m_hidden_gs_netid = gs_netid;

			m_log.printf(LL_INFO, "CrossServer::OnRegisterGameServer, Hidden GameServer registering ...");
		}
	}

	// hidden gs连过来后，其他gs才能连
	if (m_hidden_gs_netid == (NetID)-1)
	{
		m_log.printf(LL_ERROR, "CrossServer::OnRegisterGameServer, Hidden GameServer is not connected, can't connect!");

		InternalComm::Instance().NetDisconnect(gs_netid);
		return;
	}

	if (SHAREDCONFIG->GetOpenServerConfig().GetServerVersion() != cgrgs->server_version)
	{
		m_log.printf(LL_WARNING, "CrossServer::OnRegisterGameServer, server_version not equal, hidden[%u] != origin[%u], can't connect!",
			SHAREDCONFIG->GetOpenServerConfig().GetServerVersion(), cgrgs->server_version);

		InternalComm::Instance().NetDisconnect(gs_netid);
		return;
	}

	if (m_hidden_gs_role_all_param_size != cgrgs->role_all_param_size)
	{
		m_log.printf(LL_ERROR, "CrossServer::OnRegisterGameServer, role_all_param_size not equal, hidden[%u] != origin[%u], can't connect!",
			m_hidden_gs_role_all_param_size, cgrgs->role_all_param_size);

		InternalComm::Instance().NetDisconnect(gs_netid);
		return;
	}

	crossgameprotocal::CrossGameTimeInfoToHidden cgtith; // 发送原服时间给隐藏服
	cgtith.count = 0;

	for (int i = 0; i < cgrgs->count && i < crossgameprotocal::GameCrossRegisterGameServer::MAX_SERVER_ID_NUM; ++ i)
	{
		UniqueServerID gs_unique_id(cgrgs->plat_type, cgrgs->server_id_list[i]);

		GameServerInfo &info = m_game_server_info_map[gs_unique_id];
		info.netid = gs_netid;
		info.gs_unique_id = gs_unique_id;
		info.server_start_time = cgrgs->server_start_time;
		info.server_combine_time = cgrgs->server_combine_time;
		info.server_start_day_index = GetDayIndex(cgrgs->server_start_time, EngineAdapter::Instance().Time());

		{
			cgtith.server_info_list[i].gs_unique_id = info.gs_unique_id;
			cgtith.server_info_list[i].server_start_time = info.server_start_time;
			cgtith.server_info_list[i].server_combine_time = info.server_combine_time;
			cgtith.server_info_list[i].server_start_day_index = info.server_start_day_index;
			cgtith.count++;
		}

		m_log.printf(LL_INFO, "CrossServer::OnRegisterGameServer, plat_type[%d], server_id[%d], global_netid[%d], role_all_param_size[%u], server_version[%u]",
			cgrgs->plat_type, cgrgs->server_id_list[i], gs_netid, cgrgs->role_all_param_size, cgrgs->server_version);
	}


	m_cross_user_manager.OnRegisterGameServer(gs_netid, cgrgs);
	m_cross_activity_manager.OnRegisterGameServer(gs_netid);
	m_cross_person_rank.OnRegisterGameServer(gs_netid);
	m_cross_couple_rank.OnRegisterGameServer(gs_netid);
	m_cross_rand_activity_manager.OnRegisterGameServer(gs_netid);

	// send to hidden, time info
	int send_info_len = sizeof(cgtith) - sizeof(cgtith.server_info_list[0]) * (crossgameprotocal::GameCrossRegisterGameServer::MAX_SERVER_ID_NUM - cgtith.count);
	this->SendToHiddenGameWorld((const char *)&cgtith, send_info_len);
}

void CrossServer::OnSaveCrossRoleRet(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossSaveCrossRoleRet *gcscrr = (crossgameprotocal::GameCrossSaveCrossRoleRet *)data;
	gcscrr->pname[sizeof(gcscrr->pname) - 1] = 0;
	gcscrr->original_username[sizeof(gcscrr->original_username) - 1] = 0;
	gcscrr->login_server_ip[sizeof(gcscrr->login_server_ip) - 1] = 0;

	if (0 != gcscrr->result) return;

	static crossgameprotocal::CrossGameStartCrossAck cgsca;
	cgsca.result = gcscrr->result;
	cgsca.original_uid = gcscrr->original_uid;
	cgsca.cross_activity_type = gcscrr->cross_activity_type;
	STRNCPY(cgsca.login_server_ip, gcscrr->login_server_ip, sizeof(cgsca.login_server_ip));
	cgsca.login_server_port = gcscrr->login_server_port;
	F_STRNCPY(cgsca.pname, gcscrr->pname, sizeof(cgsca.pname));
	cgsca.login_time = gcscrr->login_time;
	memcpy(cgsca.login_str, gcscrr->login_str, sizeof(cgsca.login_str));
	cgsca.anti_wallow = gcscrr->anti_wallow;
	cgsca.reserved1 = 0;
	cgsca.server = IntToUid(gcscrr->uid).db_index;

	this->SendToGameWorld(UniqueUserID(gcscrr->original_plat_type, IntToUid(gcscrr->original_uid)), (const char *)&cgsca, sizeof(cgsca));
}

void CrossServer::OnUserMsg(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossUserMsg *user_msg = (crossgameprotocal::GameCrossUserMsg *)data;

	int msg_length = length - (int)sizeof(crossgameprotocal::GameCrossUserMsg);
	if (msg_length < (int)sizeof(Protocol::MessageHeader)) 
	{
		m_log.printf(LL_DEBUG, "[CrossServer::OnUserMsg msg lengh error]");
		return;
	}

	Protocol::MessageHeader *header = (Protocol::MessageHeader *)(data + sizeof(crossgameprotocal::GameCrossUserMsg));

	unsigned short msg_type = header->msg_type;
	if (0 == m_msg_handler_list[msg_type].func)
	{
		m_log.printf(LL_DEBUG, "[CrossServer::OnUserMsg Unknown msg[%u]]", msg_type);
		return;
	}

	if (msg_length != m_msg_handler_list[msg_type].data_size)
	{
		m_log.printf(LL_DEBUG, "[CrossServer::OnUserMsg msg[%u], length[%d] != %d]", msg_type, msg_length, m_msg_handler_list[msg_type].data_size);
		return;
	}

	(this->*m_msg_handler_list[msg_type].func)(global_netid, LongLongToUniqueUserID(user_msg->uuid), user_msg->merge_server_id, (const char *)header, msg_length);
}


void CrossServer::OnCrossUserLogin(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossUserLogin*gcul = (crossgameprotocal::GameCrossUserLogin *)data;
	gcul->original_user_name[sizeof(gcul->original_user_name) - 1] = 0;

	m_cross_user_manager.OnCrossUserLogin(gcul);
}

void CrossServer::OnCrossUserLogout(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossUserLogout *gcul = (crossgameprotocal::GameCrossUserLogout *)data;

	m_cross_user_manager.OnCrossUserLogout(IntToUid(gcul->uid));
}

void CrossServer::OnKickCrossUser(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossKickCrossUser *gckcu = (crossgameprotocal::GameCrossKickCrossUser *)data;

	m_cross_user_manager.OnKickCrossUser(LongLongToUniqueUserID(gckcu->bekick_original_uuid));
}

void CrossServer::OnGameCrossRransferToGame(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossRransferToGame *gcrtg = (crossgameprotocal::GameCrossRransferToGame *)data;
	if (gcrtg->length + (int)sizeof(crossgameprotocal::GameCrossRransferToGame) != length)
	{
		return;
	}

	const char *transfer_msg = (const char *)gcrtg + sizeof(crossgameprotocal::GameCrossRransferToGame);

	if (crossgameprotocal::CROSS_TRANSFER_TYPE_HIDDEN == gcrtg->transfer_type)
	{
		this->SendToHiddenGameWorld(transfer_msg, gcrtg->length);
	}
	else if (crossgameprotocal::CROSS_TRANSFER_TYPE_COMMON == gcrtg->transfer_type)
	{
		this->SendToAllGameWorld(transfer_msg, gcrtg->length, m_hidden_gs_netid);
	}
	else if (crossgameprotocal::CROSS_TRANSFER_TYPE_ALL == gcrtg->transfer_type)
	{
		this->SendToAllGameWorld(transfer_msg, gcrtg->length);
	}
	else if (crossgameprotocal::CROSS_TRANSFER_TYPE_OTHER == gcrtg->transfer_type)
	{
		this->SendToAllGameWorld(transfer_msg, gcrtg->length, global_netid);
	}
	else if (crossgameprotocal::CROSS_TRANSFER_TYPE_BY_UUID == gcrtg->transfer_type)
	{
		this->SendToGameWorld(LongLongToUniqueUserID(gcrtg->param), transfer_msg, gcrtg->length);
	}
	else if (crossgameprotocal::CROSS_TRANSFER_TYPE_BY_GLOBAL_NETID == gcrtg->transfer_type)
	{
		this->SendToGameWorld((unsigned int)gcrtg->param, transfer_msg, gcrtg->length);
	}
}

void CrossServer::OnActivityForceNextState(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossActivityForceNextState *gcafns = (crossgameprotocal::GameCrossActivityForceNextState *)data;

	m_cross_activity_manager.OnForceToNextState(gcafns->activity_type);
	m_cross_rand_activity_manager.OnForceToNextState(gcafns->activity_type);
}

void CrossServer::OnSyncXiuluotaResult(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossSyncXiuluoTowerResult *gcafns = (crossgameprotocal::GameCrossSyncXiuluoTowerResult *)data;

	CrossActivityXiuluoTower *xiuluota_activity = m_cross_activity_manager.GetCrossActivityXiuluoTower();
	if (NULL != xiuluota_activity)
	{
		xiuluota_activity->OnSyncUserResult(gcafns);
	}
}

void CrossServer::OnSyncXiuluotaInfo(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossSyncXiuluoTowerInfo *gcsxti = (crossgameprotocal::GameCrossSyncXiuluoTowerInfo *)data;

	static crossgameprotocal::CrossGameCrossSyncXiuluoTowerInfo cgcsxti;
	cgcsxti.uuid = gcsxti->uuid;
	cgcsxti.type = gcsxti->type;
	cgcsxti.param = gcsxti->param;
	CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(gcsxti->uuid), (const char *)&cgcsxti, sizeof(cgcsxti));
}

void CrossServer::OnSyncCrossPersonRankInfo(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossSyncCrossPersonRankInfo *gcscpri = (crossgameprotocal::GameCrossSyncCrossPersonRankInfo *)data;

	m_cross_person_rank.OnRankInfoChange(*gcscpri);
}

void CrossServer::OnQueryRoleInfoRequest(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossQueryRoleInfoRequest *gcscpri = (crossgameprotocal::GameCrossQueryRoleInfoRequest *)data;

	UniqueUserID target_uuid = UniqueUserID(gcscpri->target_plat_type, IntToUid(gcscpri->target_uid));

	static crossgameprotocal::CrossGameGetRoleInfoRequest cggrir;
	cggrir.target_uid = gcscpri->target_uid;
	cggrir.request_plat_type = gcscpri->request_plat_type;
	cggrir.request_uid = gcscpri->request_uid;
	cggrir.request_type = gcscpri->request_type;
	cggrir.param = gcscpri->param;
	CrossServer::GetInstCrossServer()->SendToGameWorld(target_uuid, (const char *)&cggrir, sizeof(cggrir));
}

void CrossServer::OnUserProtocalTransferToGame(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossUserProtocalTransferToGame *gcupt = (crossgameprotocal::GameCrossUserProtocalTransferToGame *)data;

	if (length < (int)sizeof(crossgameprotocal::GameCrossUserProtocalTransferToGame) + gcupt->protocal_len)
	{
		return;
	}

	if (0 == gcupt->uuid)
	{
		CrossServer::GetInstCrossServer()->SendToAllUser(true, 0, 
			(const char *)gcupt + sizeof(crossgameprotocal::GameCrossUserProtocalTransferToGame), gcupt->protocal_len);
	}
	else
	{
		if (NULL != this->GetCrossUserManager()->GetCrossUser(LongLongToUniqueUserID(gcupt->uuid)))
		{
			this->SendToCrossUser(LongLongToUniqueUserID(gcupt->uuid), data + sizeof(crossgameprotocal::GameCrossUserProtocalTransferToGame),
				length - sizeof(crossgameprotocal::GameCrossUserProtocalTransferToGame));
		}
		else
		{
			CrossServer::GetInstCrossServer()->SendToUser(LongLongToUniqueUserID(gcupt->uuid),
				(const char *)gcupt + sizeof(crossgameprotocal::GameCrossUserProtocalTransferToGame), gcupt->protocal_len);
		}
	}
}

void CrossServer::OnSyncRoleData(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::CrossGameSyncRoleData *cgsrd = (crossgameprotocal::CrossGameSyncRoleData *)data;

	CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(cgsrd->uuid), data, length);
}

void CrossServer::OnCrossChannelChat(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::CrossGameChannelChat *cgcc = (crossgameprotocal::CrossGameChannelChat *)data;

	CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(cgcc->uuid), data, length);
}

void CrossServer::OnCrossRankInfoReq(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::CrossGameRankInfoReq *cgrir = (crossgameprotocal::CrossGameRankInfoReq *)data;

	CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(cgrir->uuid), data, length);
}

void CrossServer::OnGameCrossSendMailToUser(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossSendMailToUser *gcffhn = (crossgameprotocal::GameCrossSendMailToUser*)data;

	static crossgameprotocal::CrossGameSendMailToUser cgffhn;
	cgffhn.user_id = UidToInt(LongLongToUniqueUserID(gcffhn->unique_user_id).user_id);
	cgffhn.contentparam = gcffhn->contentparam;

	CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(gcffhn->unique_user_id), (const char *)&cgffhn, sizeof(cgffhn));
}

void CrossServer::OnGameCrossGmAddWeek(NetID global_netid, const char *data, int length)
{
	this->OnWeekChange();
}

void CrossServer::OnSyncWorldCommonDataChange(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossWorldCommonDataChange *gcwcdc = (crossgameprotocal::GameCrossWorldCommonDataChange *)data;

	// 纯粹转发到本服
	crossgameprotocal::CrossGameWorldCommonDataChange cgwcdc;

	cgwcdc.uuid = gcwcdc->uuid;
	cgwcdc.change_type = gcwcdc->change_type;
	cgwcdc.sub_type = gcwcdc->sub_type;
	cgwcdc.param_2 = gcwcdc->param_2;
	cgwcdc.param_3 = gcwcdc->param_3;
	cgwcdc.param_4 = gcwcdc->param_4;

	if (cgwcdc.uuid > 0)
	{
		CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(cgwcdc.uuid), (const char *)&cgwcdc, sizeof(cgwcdc));
	}
	else
	{
		CrossServer::GetInstCrossServer()->SendToAllGameWorld((const char *)&cgwcdc, sizeof(cgwcdc));
	}
}

void CrossServer::OnForbidCrossUserTalk(NetID global_net_id, const char *data, int length)
{
	crossgameprotocal::GameCrossForbidTalk *gcft = (crossgameprotocal::GameCrossForbidTalk *)data;

	UniqueUserID unique_userid = LongLongToUniqueUserID(gcft->uuid);

	CrossUser *cross_user = CrossServer::GetInstCrossServer()->GetCrossUserManager()->GetCrossUser(unique_userid);
	if (nullptr != cross_user)
	{
		crossgameprotocal::CrossGameForbidTalk cgft;

		cgft.cross_role_id = cross_user->GetCrossUid();
		cgft.forbid_time = gcft->forbid_time;

		CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char *)&cgft, sizeof(cgft));
	}
}

void CrossServer::OnCrossActivity1V1MatchReq(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossActivity1V1MatchReq *gcamr = (crossgameprotocal::GameCrossActivity1V1MatchReq *)data;

	m_cross_activity_manager.GetCrossActivity1V1()->OnMatchReq(global_netid, gcamr);
}

void CrossServer::OnCross1V1FightResult(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossCross1V1FightResult *gccfr = (crossgameprotocal::GameCrossCross1V1FightResult*)data;
	m_cross_activity_manager.GetCrossActivity1V1()->OnFightResult(global_netid, gccfr);
}

void CrossServer::OnCrossMultiuserChallengeMatchingReq(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossCrossMuliuserChallengeMatchingReq *gccmcmr = (crossgameprotocal::GameCrossCrossMuliuserChallengeMatchingReq *)data;
	if (gccmcmr->count <= 0 || gccmcmr->count > CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT) return;

	for (int i = 0; i < gccmcmr->count; ++ i)
	{
		gccmcmr->user_info_list[i].user_name[sizeof(GameName) - 1] = 0;
	}

	m_cross_activity_manager.GetCrossActivityCrossMultiuserChallenge()->OnMatchingReq(global_netid, gccmcmr);
}

void CrossServer::OnCrossMultiuserChallengeMatchingCancel(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossCrossMuliuserChallengeMatchingCancel *gccmcmc = (crossgameprotocal::GameCrossCrossMuliuserChallengeMatchingCancel *)data;
	if (gccmcmc->uuid <= 0) return;

	m_cross_activity_manager.GetCrossActivityCrossMultiuserChallenge()->OnMatchingCancel(global_netid, LongLongToUniqueUserID(gccmcmc->uuid));
}

void CrossServer::OnGlobalSyncCrossMultiuserChallengeRankList(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GlobalCrossMultiuserChallengeRankList *gcmcrl = (crossgameprotocal::GlobalCrossMultiuserChallengeRankList *)data;
	if (gcmcrl->count <= 0 || gcmcrl->count > crossgameprotocal::MAX_MULTIUSER_CHALLENGE_RANK_COUNT) return;

	for (int i = 0; i < gcmcrl->count; ++ i)
	{
		gcmcrl->rank_list[i].name[sizeof(GameName) - 1] = 0;
	}

	m_cross_activity_manager.GetCrossActivityCrossMultiuserChallenge()->OnGlobalSyncRankList(gcmcrl);
}

void CrossServer::OnCrossMultiuserChallengeMatchResult(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossCrossMultiuserChallengeMatchResult *gccmcmr = (crossgameprotocal::GameCrossCrossMultiuserChallengeMatchResult *)data;

	for (int i = 0; i < CROSS_MULTIUSER_CHALLENGE_SIDE_MEMBER_COUNT * CROSS_MULTIUSER_CHALLENGE_SIDE_MAX; ++ i)
	{
		gccmcmr->sync_user_info_list[i].user_name[sizeof(GameName) - 1] = 0;
	}

	m_cross_activity_manager.GetCrossActivityCrossMultiuserChallenge()->OnMatchResult(gccmcmr);
}

void CrossServer::OnCross1V1MatchQuery(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len)
{
	//Protocol::CSCross1v1MatchQuery *ccmq = (Protocol::CSCross1v1MatchQuery *)msg;

	//m_cross_activity_manager.GetCrossActivity1V1()->OnMatchQuery(global_netid, unique_user_id, ccmq->req_type);
}

void CrossServer::OnCross1V1RecordQuery(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len)
{
	m_cross_activity_manager.GetCrossActivity1V1()->OnQueryWeekRecord(global_netid, unique_user_id);
}

void CrossServer::OnCross1V1GetRankList(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len)
{
	m_cross_person_rank.OnGetRankList(global_netid, unique_user_id, CROSS_PERSON_RANK_TYPE_1V1_SCORE);
}

void CrossServer::OnGetMultiuserChallengeRankList(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len)
{
	m_cross_person_rank.OnGetRankList(global_netid, unique_user_id, CROSS_PERSON_RANK_TYPE_3V3_SCORE);
}

void CrossServer::OnCheckMultiuserChallengeUserHasMatch(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len)
{
	m_cross_activity_manager.GetCrossActivityCrossMultiuserChallenge()->OnCheckUserHasMatch(global_netid, unique_user_id);
}

void CrossServer::OnCrossTuanzhanSyncFightResult(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossTuanzhanSyncFightResult *tzsfr = (crossgameprotocal::GameCrossTuanzhanSyncFightResult *)data;

	m_cross_activity_manager.GetCrossActivityTuanzhan()->OnFightResult(tzsfr);
}

void CrossServer::OnPastureInfoSync(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossPastureInfoSync *pis = (crossgameprotocal::GameCrossPastureInfoSync*)data;
	m_cross_activity_manager.GetCrossActivityPasture()->OnReceiveInfoSync(pis);
}

void CrossServer::OnCrossBossSyncPlayerInfo(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossBossSyncPlayerInfo *tzsfr = (crossgameprotocal::GameCrossBossSyncPlayerInfo *)data;
	if (tzsfr->cross_boss_type == crossgameprotocal::CROSS_BOSS_TYPE_CROSS)
	{
		CrossActivityCrossBoss *cross_boss = m_cross_activity_manager.GetCrossActivityCrossBoss();
		if (NULL != cross_boss)
		{
			cross_boss->OnPlayerInfoResult(tzsfr);
		}
	}
	else if (tzsfr->cross_boss_type == crossgameprotocal::CROSS_BOSS_TYPE_MIZANG)
	{
		CrossActivityCrossMizangBoss *cross_boss = m_cross_activity_manager.GetCrossActivityCrossMizangBoss();
		if (NULL != cross_boss)
		{
			cross_boss->OnPlayerInfoResult(tzsfr);
		}
	}
	else if (tzsfr->cross_boss_type == crossgameprotocal::CROSS_BOSS_TYPE_YOUMING)
	{
		CrossActivityCrossYouMingBoss *cross_boss = m_cross_activity_manager.GetCrossActivityCrossYouMingBoss();
		if (NULL != cross_boss)
		{
			cross_boss->OnPlayerInfoResult(tzsfr);
		}
	}
}

void CrossServer::OnCrossTeamFBUserOptionReq(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::CrossTeamFBOptionReq *ctfbuor = (crossgameprotocal::CrossTeamFBOptionReq *)data;
	CrossActivityTeamFB *cross_team_fb = (CrossActivityTeamFB*)CrossServer::GetInstCrossServer()->GetCrossActivityManager()->GetCrossActivity(CROSS_ACTIVITY_TYPE_TEAM_FB);
	if (NULL != cross_team_fb)
	{
		cross_team_fb->OnCrossTeamFBOption(ctfbuor->option_type, ctfbuor->user_info, ctfbuor->param, ctfbuor->param2, ctfbuor->param3);
	}
}

void CrossServer::OnCrossTeamFBRoomOptionReq(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::CrossTeamFBOption2Req *ctfbror = (crossgameprotocal::CrossTeamFBOption2Req *)data;
	CrossActivityTeamFB *cross_team_fb = (CrossActivityTeamFB*)CrossServer::GetInstCrossServer()->GetCrossActivityManager()->GetCrossActivity(CROSS_ACTIVITY_TYPE_TEAM_FB);
	if (NULL != cross_team_fb)
	{
		cross_team_fb->OnCrossTeamFBRoomOption(ctfbror->option2_type, ctfbror->layer, ctfbror->room);
	}
}


void CrossServer::OnCrossCommonOperaReq(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::CSCrossCommonOperaReq *cor = (crossgameprotocal::CSCrossCommonOperaReq*)data;
	switch (cor->req_type)
	{
	case crossgameprotocal::CROSS_COMMON_OPERA_REQ_CROSS_GUILDBATTLE_BOSS_INFO:
	{
		crossgameprotocal::CrossGameGetGuildBattleBossInfoReq msg;
		msg.scene_id = cor->param_1;
		msg.uuid = cor->param_3;
		InternalComm::Instance().NetSend(m_hidden_gs_netid, (const char *)&msg, sizeof(msg));
	}
	break;

	}
}

void CrossServer::OnCommonInfoSync(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossCommonInfoSync *cis = (crossgameprotocal::GameCrossCommonInfoSync *)data;

	static crossgameprotocal::CrossGameUpdateCrossRecord cgucr; 
	cgucr.update_cross_record_type = crossgameprotocal::CrossGameUpdateCrossRecord::UPDATE_CROSS_RECORD_CROSS_COMMON;
	cgucr.uid = UidToInt(LongLongToUniqueUserID(cis->uuid).user_id);
	cgucr.record_param_list[0] = cis->has_use_gold;
	CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(cis->uuid), (const char *)&cgucr, sizeof(cgucr));
}

void CrossServer::OnCommonAttendInfoSync(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossCommonAttendInfoSync *cis = (crossgameprotocal::GameCrossCommonAttendInfoSync *)data;

	static crossgameprotocal::CrossGameUpdateCrossRecord cgucr;
	cgucr.update_cross_record_type = crossgameprotocal::CrossGameUpdateCrossRecord::UNDATE_CROSS_RECORD_CROSS_ATTEND;
	cgucr.uid = UidToInt(LongLongToUniqueUserID(cis->uuid).user_id);
	cgucr.record_param_list[0] = cis->attend_activity_num;
	CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(cis->uuid), (const char *)&cgucr, sizeof(cgucr));
}

void CrossServer::OnCrossXiuluoTowerSyncDropLog(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossSyncXiuluoTowerDropInfo *csxltdi = (crossgameprotocal::GameCrossSyncXiuluoTowerDropInfo *)data;
	m_cross_activity_manager.GetCrossActivityXiuluoTower()->OnSyncDropLog(LongLongToUniqueUserID(csxltdi->uuid), csxltdi);
}

void CrossServer::OnCrossXiuluoTowerGetDropLog(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len)
{
	m_cross_activity_manager.GetCrossActivityXiuluoTower()->OnGetDropLog(unique_user_id);
}

void CrossServer::OnCrossXiuluoTowerSyncRankTitle(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossSyncXiuluoTowerRankTitle *cgxltr = (crossgameprotocal::GameCrossSyncXiuluoTowerRankTitle *)data;
	m_cross_activity_manager.GetCrossActivityXiuluoTower()->OnRecvSyncRankTile(cgxltr);
}

void CrossServer::OnCrossGuildBattleSyncDropLog(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossSyncGuildBattleDropInfo *pro = (crossgameprotocal::GameCrossSyncGuildBattleDropInfo *)data;
	m_cross_activity_manager.GetCrossActivityGuildBattle()->OnSyncDropLog(LongLongToUniqueUserID(pro->uuid), pro);
}

void CrossServer::OnCrossGuildBattleGetDropLog(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len)
{
	m_cross_activity_manager.GetCrossActivityGuildBattle()->OnGetDropLog(unique_user_id);
}

void CrossServer::OnCrossGuildBattleGetMonsterInfo(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len)
{
	crossgameprotocal::CrossGameGetGuildBattleMonsterInfoReq req;
	req.uuid = UniqueUserIDToLongLong(unique_user_id);
	InternalComm::Instance().NetSend(m_hidden_gs_netid, (const char *)&req, sizeof(req));
}

void CrossServer::OnGuildBattleInfoSync(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossGuildBattleInfoSync *cmd = (crossgameprotocal::GameCrossGuildBattleInfoSync *)data;
	m_cross_activity_manager.GetCrossActivityGuildBattle()->OnRecvGuildBattleInfo(cmd);
}

void CrossServer::OnGuildBattleRankLogInfoSync(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossGuildBattleRankLogSync *cmd = (crossgameprotocal::GameCrossGuildBattleRankLogSync *)data;
	m_cross_activity_manager.GetCrossActivityGuildBattle()->OnRecvGuildBattleRankLogInfo(cmd);
}

void CrossServer::OnGuildBattleGetRankLogInfoReq(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossGetGuildBattleRankLogReq *cmd = (crossgameprotocal::GameCrossGetGuildBattleRankLogReq *)data;
	m_cross_activity_manager.GetCrossActivityGuildBattle()->OnRecvGuildBattleGetRankLogInfoReq(global_netid, cmd);
}

void CrossServer::OnCrossFishingInfoSync(NetID global_net_id, const char *data, int length)
{
	crossgameprotocal::GameCrossFishingInfoSync *cmd = (crossgameprotocal::GameCrossFishingInfoSync *)data;

	CrossActivity *cross_activity = m_cross_activity_manager.GetCrossActivity(CROSS_ACTIVITY_TYPE_FISHING);
	if (cross_activity != nullptr)
	{
		CrossActivityFishing *cross_fish_activity = reinterpret_cast<CrossActivityFishing *>(cross_activity);
		cross_fish_activity->OnReceiveFishingInfo(cmd);
	}
}

void CrossServer::OnCrossGameCross1V1MatchResult(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::CrossGameCross1V1MatchResult *cgcmr = (crossgameprotocal::CrossGameCross1V1MatchResult *)data;
	if (cgcmr->uuid <= 0) return;

	m_cross_activity_manager.GetCrossActivity1V1()->OnMatchQuery(global_netid, LongLongToUniqueUserID(cgcmr->uuid), cgcmr->req_type);
}

void CrossServer::OnSyncCross1V1RoleRankBaseInfo(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossActivity1V1RoleRankBaseInfo *gcarrbi = (crossgameprotocal::GameCrossActivity1V1RoleRankBaseInfo*)data;
	m_cross_activity_manager.GetCrossActivity1V1()->OnSyncRoleRankBaseInfo(global_netid, gcarrbi);
}

void CrossServer::OnSyncCross1V1RoleRankList(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossCross1V1RankList *gccrl = (crossgameprotocal::GameCrossCross1V1RankList *)data;
	if (gccrl->count <= 0 || gccrl->count > crossgameprotocal::MAX_CROSS_1V1_RANK_COUNT) return;

	for (int i = 0; i < gccrl->count; ++i)
	{
		gccrl->rank_list[i].name[sizeof(GameName) - 1] = 0;
	}

	m_cross_activity_manager.GetCrossActivity1V1()->OnGlobalSyncRankList(global_netid, gccrl);
}
void CrossServer::OnCrossLieKunFBInfoSync(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossLieKunFBInfoSync *gclkfbis = (crossgameprotocal::GameCrossLieKunFBInfoSync *)data;
	m_cross_activity_manager.GetCrossActivityCrossLieKunFb()->OnRecvCrossLieKunFBInfo(gclkfbis);
}

void CrossServer::OnCrossLieKunFBReq(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len)
{
	Protocol::CSCrossLieKunFBReq *csclkfbr = (Protocol::CSCrossLieKunFBReq*)msg;
	switch (csclkfbr->type)
	{
	case Protocol::LIEKUNFB_TYPE_GET_PLAYER_INFO:
	{
		m_cross_activity_manager.GetCrossActivityCrossLieKunFb()->SendCrossLieKunFBPlayerInfo(global_netid, unique_user_id, csclkfbr->param2);
	}
	break;

	}
}

void CrossServer::OnGameCrossLieKunFBKillZoneBossSync(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossLieKunFBKillZoneBossSync *gclkfbkzbs = (crossgameprotocal::GameCrossLieKunFBKillZoneBossSync *)data;

	crossgameprotocal::CrossGameLieKunFBKillZoneBossSync cglkfbkzbs;

	cglkfbkzbs.uuid = gclkfbkzbs->uuid;
	cglkfbkzbs.zone = gclkfbkzbs->zone;
	cglkfbkzbs.kill_zone_boss_guild_id = gclkfbkzbs->kill_zone_boss_guild_id;

	m_cross_activity_manager.GetCrossActivityCrossLieKunFb()->OnRecvCrossLieKunKillZoneBossGuildInfo(gclkfbkzbs->zone, gclkfbkzbs->kill_zone_boss_guild_id);
	CrossServer::GetInstCrossServer()->SendToGameWorld(LongLongToUniqueUserID(gclkfbkzbs->uuid), (const char *)&cglkfbkzbs, sizeof(cglkfbkzbs));
}

void CrossServer::OnCrossMultiuserChallengeGetMatchingState(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossCrossMuliuserChallengeGetMatchingState *gccmcgms = (crossgameprotocal::GameCrossCrossMuliuserChallengeGetMatchingState *)data;
	if (gccmcgms->uuid <= 0) return;

	m_cross_activity_manager.GetCrossActivityCrossMultiuserChallenge()->OnGetMatchingState(global_netid, LongLongToUniqueUserID(gccmcgms->uuid));
}

void CrossServer::OnCrossMessBattleInfoSync(NetID global_net_id, const char *data, int lenth)
{
	crossgameprotocal::GameCrossMessBattleInfoSync * gcmbi = (crossgameprotocal::GameCrossMessBattleInfoSync *)data;
	CrossActivityMessBattle * activity = dynamic_cast<CrossActivityMessBattle*>(m_cross_activity_manager.GetCrossActivity(CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB));
	if (nullptr != activity)
	{
		activity->OnRecvMessBattleRankTop(gcmbi);
	}
}
void CrossServer::OnCrossNightFightInfoSync(NetID global_net_id, const char *data, int lenth)
{
	crossgameprotocal::GameCrossnNightFightInfoSync * gcnfi = (crossgameprotocal::GameCrossnNightFightInfoSync *)data;
	CrossActivityNightFight * activity = dynamic_cast <CrossActivityNightFight *>(m_cross_activity_manager.GetCrossActivity(CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB));
	if (nullptr != activity)
	{
		activity->OnRecvNightFightRankTop(gcnfi);
	}
}

void CrossServer::OnGmOpera(NetID global_netid, const char *data, int length)
{
	enum
	{
		CALC_PERSON_RANK = 0,
		CALC_COUPLE_RANK,
		FAKE_COUPLE_RANK,
	};

	crossgameprotocal::GameCrossGmOpera *pro = (crossgameprotocal::GameCrossGmOpera *)data;
	switch (pro->opera_type)
	{
	case CALC_PERSON_RANK:
	{
		m_cross_person_rank.GmRankReward(pro->param_1, 0 != pro->param_2);
	}
	break;

	case CALC_COUPLE_RANK:
	{
		m_cross_couple_rank.GmGiveRankReward(pro->param_1, 0 != pro->param_2);
	}
	break;

	case FAKE_COUPLE_RANK:
	{
		m_cross_couple_rank.GmFakeUser(pro->param_1);
	}
	break;

	default:
		break;
	}
}

void CrossServer::OnSyncCrossCoupleRankInfo(NetID global_net_id, const char *data, int length)
{
	crossgameprotocal::GameCrossSyncCrossCoupleRankInfo *pro = (crossgameprotocal::GameCrossSyncCrossCoupleRankInfo *)data;
	m_cross_couple_rank.OnSyncGameWorldRankList(pro);
}

void CrossServer::OnDivorceSync(NetID global_netid, const char * data, int length)
{
	crossgameprotocal::GameCrossCoupleRankDivorce *pro = (crossgameprotocal::GameCrossCoupleRankDivorce *)data;
	m_cross_couple_rank.OnDivorce(pro->plat_type, pro->uid);
}

void CrossServer::OnSyncCrossRandActivityData(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossSyncCrossRandActivityData *scrad = (crossgameprotocal::GameCrossSyncCrossRandActivityData*)data;
	m_cross_rand_activity_manager.OnSyncActivityData(global_netid, scrad, length);
}

void CrossServer::OnCrossRAChongzhiRankGetRank(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len)
{
	CrossRandActivityChongzhiRank *act = (CrossRandActivityChongzhiRank *)m_cross_rand_activity_manager.GetActivity(CROSS_RAND_ACTIVITY_TYPE_CHONGZHI_RANK);
	if (NULL != act)
	{
		act->OnGetRankList(global_netid, unique_user_id);
	}
}

void CrossServer::OnCrossRAConsumeRankGetRank(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len)
{
	Protocol::CSCrossRAConsumeRankGetRank * get_rank = (Protocol::CSCrossRAConsumeRankGetRank*)msg;
	CrossRandActivityConsumeRank *act = (CrossRandActivityConsumeRank *)m_cross_rand_activity_manager.GetActivity(CROSS_RAND_ACTIVITY_TYPE_CONSUME_RANK);
	if (NULL != act)
	{
		act->OnGetRankList(global_netid, unique_user_id,get_rank->modify_id);
	}
}

void CrossServer::OnCrossBianJingZhiDiBossInfoReq(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len)
{
	//Protocol::CSCrossServerBianJingZhiDiBossInfoReq *csclkfbr = (Protocol::CSCrossServerBianJingZhiDiBossInfoReq*)msg;
	m_cross_activity_manager.GetCrossActivityCrossBianJingZhiDi()->SendCrossBianJingZhiDiBossInfo(global_netid, unique_user_id);
}

void CrossServer::OnSyncCrossBianJingZhiDiBossInfo(NetID global_netid, const char *data, int length)
{
	crossgameprotocal::GameCrossSyncCrossBianJingZhiBossInfo *scrad = (crossgameprotocal::GameCrossSyncCrossBianJingZhiBossInfo*)data;
	m_cross_activity_manager.GetCrossActivityCrossBianJingZhiDi()->OnSyncCrossBianJingZhiDiBossInfo(scrad);
}
