#ifndef __CROSSSERVER_H__
#define __CROSSSERVER_H__

#include <vector>
#include <map>

#include "inetworkmodule.h"
#include "servercommon/serverdef.h"
#include "utility/logagent.h"

#include "servercommon/crossdef.hpp"

#include "crossstatus/crossstatus.hpp"
#include "crossuser/crossusermanager.hpp"
#include "crossuser/crossoperateregister.hpp"
#include "crossactivity/crossactivitymanager.hpp"
#include "crossrank/crosspersonrank.hpp"
#include "crossrank/crosscouplerank.hpp"
#include "crossrandactivity/crossrandactivitymanager.hpp"

class CrossServerModule;

class CrossServer
{
public:
	CrossServer();
	~CrossServer();

	static CrossServer * GetInstCrossServer();

	bool Init(CrossServerModule *cross_server_module, ILogModule *log_module);

	void Start();
	void Update();

	bool IsAllDataLoadFinish();
	void OnAllDataLoadFinish();

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnWeekChange();

	void OnInternalGameServerAccept(Port listen_port, NetID netid, IP ip, Port port);
	void OnInternalRecv(NetID netid, const char *data, int length);
	void OnInternalDisconnect(NetID netid);
	void OnHiddenGlobalDisconnect();

	void StopServer();

	CrossStatus * GetCrossStatus() { return &m_cross_status; }
	CrossUserManager * GetCrossUserManager() { return &m_cross_user_manager; }
	CrossActivityManager * GetCrossActivityManager() { return &m_cross_activity_manager; }
	CrossPersonRank * GetCrossPersonRank() { return &m_cross_person_rank; }
	CrossRandActivityManager * GetCrossRandActivityManager() { return &m_cross_rand_activity_manager; }

	bool SendToUser(NetID global_netid, const UserID &user_id, const char *data, int length);				// Protocol
	bool SendToUser(const UniqueUserID &unique_user_id, const char *data, int length);						// Protocol
	bool SendToCrossUser(const UserID &cross_user_id, const char *data, int length);						// Protocol
	bool SendToCrossUser(const UniqueUserID &original_unique_user_id, const char *data, int length);		// Protocol
	bool SendToAllUser(bool is_to_all_global, NetID global_netid, const char *data, int length, int cross_activity_type = CROSS_ACTIVITY_TYPE_INVALID, bool is_shield_new_server = false, UniqueServerID new_server_id = INVALID_UNIQUE_SERVER_ID);	// Protocol
	bool SendToAllCrossUser(const char *data, int length, int cross_activity_type = CROSS_ACTIVITY_TYPE_RESERVED);// Protocol

	bool SendToGameWorld(NetID gs_netid, const char *data, int length);										// crossgameprotocal
	bool SendToGameWorld(const UniqueUserID &unique_user_id, const char *data, int length);					// crossgameprotocal
	bool SendToGameWorld(const UniqueServerID &unique_server_id, const char *data, int length);				// crossgameprotocal
	bool SendToHiddenGameWorld(const char *data, int length);												// crossgameprotocal
	bool SendToAllGameWorld(const char *data, int length, NetID except_global_netid = -1);					// crossgameprotocal
	bool SendToAllGameWorldShieldNewServer(const char *data, int length, NetID except_global_netid = -1, UniqueServerID new_server_id = INVALID_UNIQUE_SERVER_ID);	// crossgameprotocal

	void NoticeNum(NetID global_netid, const UserID &user_id, int notice_num);
	void NoticeNumToCrossUser(const UserID &user_id, int notice_num);

	bool SystemMsgToAllUser(bool is_to_all_global, NetID global_netid, const char *msg, int length, int msg_type = SYS_MSG_CENTER_NOTICE, int pos = 0, int color = 0, bool is_shield_new_server = false, UniqueServerID new_server_id = INVALID_UNIQUE_SERVER_ID);
	bool SystemMsgToAllHiddenGlobalUser(const char *msg, int length, int msg_type = SYS_MSG_CENTER_NOTICE, int pos = 0, int color = 0);
	bool SystemMsgToAllGlobalUser(const char *msg, int length, int msg_type = SYS_MSG_CENTER_NOTICE, int pos = 0, int color = 0, bool is_shield_new_server = false, UniqueServerID new_server_id = INVALID_UNIQUE_SERVER_ID);

	void KickAllCrossUser(int cross_activity_type = CROSS_ACTIVITY_TYPE_RESERVED);

	bool IsUserLocateServerConnect(const UniqueUserID &uniqe_user_id);
	int GetGameServerDayIndex(const UniqueServerID &unique_server_id);
	unsigned int GetGameServerStartTime(const UniqueServerID &unique_server_id);
	unsigned int GetGameServerCombineTime(const UniqueServerID &unique_server_id);

	NetID GetHiddenServerNetid() { return m_hidden_gs_netid; }

	void MailToGameWorldUser(const UniqueUserID &uniqe_user_id, const MailContentParam &content_param);
protected:
	CrossServerModule		*m_cross_server_module;

	LogAgent				m_log;									// ��־
	unsigned long			m_game_time;							// ��ǰ��Ϸʱ��
	bool					m_is_all_data_load_finish;				// ȫ�����ݼ�����ɱ��
	unsigned int			m_last_update_dayid;					// ���update����
	time_t					m_last_send_hello_timestamp;			// ���һ�η���������ʱ���

	CrossStatus				m_cross_status;							// ���״̬
	CrossUserManager		m_cross_user_manager;					// �����ҹ���
	CrossOperateRegister	m_cross_operate_register;				// ��������ݴ�����
	CrossActivityManager	m_cross_activity_manager;				// ����������
	CrossPersonRank			m_cross_person_rank;					// ����������а�
	CrossCoupleRank			m_cross_couple_rank;					// ����������а�
	CrossRandActivityManager m_cross_rand_activity_manager;			// �������������

protected:
	NetID GetGameServerNetID(const UniqueServerID &unique_server_id);
	NetID GetGameServerNetID(const UniqueUserID &uniqe_user_id);

	typedef std::vector<NetID> GameServerNetIDList;
	typedef std::vector<NetID>::iterator GameServerNetIDListIt;
	GameServerNetIDList		m_game_server_netid_list;				// gameserver netid �б�

	struct GameServerInfo
	{
		GameServerInfo() : netid(0), server_start_time(0), server_combine_time(0), server_start_day_index(0) {}

		NetID netid;
		UniqueServerID gs_unique_id;
		unsigned int server_start_time;
		unsigned int server_combine_time;
		int server_start_day_index;
	};
	typedef std::map<UniqueServerID, GameServerInfo> GameServerInfoMap;
	typedef GameServerInfoMap::iterator GameServerInfoMapIt;
	GameServerInfoMap		m_game_server_info_map;

	UniqueServerID			m_hidden_gs_unique_id;					// ���CrossServerID
	NetID					m_hidden_gs_netid;						// ���global netid
	UInt32					m_hidden_gs_role_all_param_size;		// ���ط�RoleAllParam�ĳ���

	void OnTransferToGlobal(NetID global_netid, const char *data, int length);
	void OnTransferToAllGlobal(NetID global_netid, const char *data, int length);

	void OnGlobalCrossFixPlatTypeCheckUserAck(NetID global_netid, const char *data, int length);

protected:
	typedef void (CrossServer::*OnGSHandler)(NetID gs_netid, const char *data, int length);

	OnGSHandler m_on_gs_handler_list[4096];	

	void InitGSHandler();
	void OnStartCrossReq(NetID gs_netid, const char *data, int length);
	void OnRegisterGameServer(NetID gs_netid, const char *data, int length);
	void OnSaveCrossRoleRet(NetID global_netid, const char *data, int length);
	void OnUserMsg(NetID global_netid, const char *data, int length);
	void OnCrossUserLogin(NetID global_netid, const char *data, int length);
	void OnCrossUserLogout(NetID global_netid, const char *data, int length);
	void OnKickCrossUser(NetID global_netid, const char *data, int length);
	void OnGameCrossRransferToGame(NetID global_netid, const char *data, int length);
	void OnActivityForceNextState(NetID global_netid, const char *data, int length);
	void OnSyncXiuluotaResult(NetID global_netid, const char *data, int length);
	void OnSyncXiuluotaInfo(NetID global_netid, const char *data, int length);
	void OnSyncCrossPersonRankInfo(NetID global_netid, const char *data, int length);
	void OnQueryRoleInfoRequest(NetID global_netid, const char *data, int length);
	void OnUserProtocalTransferToGame(NetID global_netid, const char *data, int length);
	void OnSyncRoleData(NetID global_netid, const char *data, int length);
	void OnCrossChannelChat(NetID global_netid, const char *data, int length);
	void OnCrossRankInfoReq(NetID global_netid, const char *data, int length);

	void OnGameCrossSendMailToUser(NetID global_netid, const char *data, int length);
	void OnGameCrossGmAddWeek(NetID global_netid, const char *data, int length);
	void OnSyncWorldCommonDataChange(NetID global_netid, const char *data, int length);

	void OnForbidCrossUserTalk(NetID global_net_id, const char *data, int length);

	void OnCrossActivity1V1MatchReq(NetID global_netid, const char *data, int length);
	void OnCross1V1FightResult(NetID global_netid, const char *data, int length);

	void OnCrossMultiuserChallengeMatchingReq(NetID global_netid, const char *data, int length);
	void OnCrossMultiuserChallengeMatchingCancel(NetID global_netid, const char *data, int length);
	void OnGlobalSyncCrossMultiuserChallengeRankList(NetID global_netid, const char *data, int length);
	void OnCrossMultiuserChallengeMatchResult(NetID global_netid, const char *data, int length);
	void OnCrossMultiuserChallengeGetMatchingState(NetID global_netid, const char *data, int length);

	void OnCross1V1MatchQuery(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len);
	void OnCross1V1RecordQuery(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len);
	void OnCross1V1GetRankList(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len);

	void OnGetMultiuserChallengeRankList(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len);
	void OnCheckMultiuserChallengeUserHasMatch(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len);

	void OnCrossTuanzhanSyncFightResult(NetID global_netid, const char *data, int length);

	void OnPastureInfoSync(NetID global_netid, const char *data, int length);

	void OnCrossBossSyncPlayerInfo(NetID global_netid, const char *data, int length);

	void OnCrossTeamFBUserOptionReq(NetID global_netid, const char *data, int length);
	void OnCrossTeamFBRoomOptionReq(NetID global_netid, const char *data, int length);

	void OnCommonInfoSync(NetID global_netid, const char *data, int length);
	void OnCommonAttendInfoSync(NetID global_netid, const char *data, int length);

	void OnCrossXiuluoTowerSyncDropLog(NetID global_netid, const char *data, int length);
	void OnCrossXiuluoTowerGetDropLog(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len);
	void OnCrossXiuluoTowerSyncRankTitle(NetID global_netid, const char *data, int length);

	void OnCrossGuildBattleSyncDropLog(NetID global_netid, const char *data, int length);
	void OnCrossGuildBattleGetDropLog(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len);
	void OnCrossGuildBattleGetMonsterInfo(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len);

	void OnGuildBattleInfoSync(NetID global_netid, const char *data, int length);
	void OnGuildBattleRankLogInfoSync(NetID global_netid, const char *data, int length);
	void OnGuildBattleGetRankLogInfoReq(NetID global_netid, const char *data, int length);

	void OnCrossFishingInfoSync(NetID global_net_id, const char *data, int length);

	void OnCrossGameCross1V1MatchResult(NetID global_netid, const char *data, int length);

	void OnSyncCross1V1RoleRankBaseInfo(NetID global_netid, const char *data, int length);

	void OnSyncCross1V1RoleRankList(NetID global_netid, const char *data, int length);

	void OnCrossCommonOperaReq(NetID global_netid, const char *data, int length);

	void OnCrossLieKunFBInfoSync(NetID global_netid, const char *data, int length);
	void OnCrossLieKunFBReq(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len);
	void OnGameCrossLieKunFBKillZoneBossSync(NetID global_netid, const char *data, int length);

	void OnCrossMessBattleInfoSync(NetID global_net_id, const char *data, int lenth);
	void OnCrossNightFightInfoSync(NetID global_net_id, const char *data, int lenth);
	void OnGmOpera(NetID global_netid, const char *data, int length);

	void OnSyncCrossCoupleRankInfo(NetID global_net_id, const char *data, int length);
	void OnDivorceSync(NetID global_netid, const char * data, int length);
	void OnSyncCrossRandActivityData(NetID global_netid, const char *data, int length);

	void OnCrossRAChongzhiRankGetRank(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len);
	void OnCrossRAConsumeRankGetRank(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len);

	void OnCrossBianJingZhiDiBossInfoReq(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len);
	void OnSyncCrossBianJingZhiDiBossInfo(NetID global_netid, const char *data, int length);

protected:
	typedef void (CrossServer::*HandleFunc)(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len);
	static const int MAX_MSG_TYPE = (int)USHRT_MAX + 1;

	struct HandlerItem 
	{
		HandlerItem() : func(NULL), data_size(0){}
		HandlerItem(HandleFunc f, int s) : func(f), data_size(s){}

		HandleFunc	func;
		int			data_size;
	};

	HandlerItem m_msg_handler_list[MAX_MSG_TYPE];

	void OnGetCrossPersonRankList(NetID global_netid, const UniqueUserID &unique_user_id, int merge_server_id, const char *msg, int len);
};

#endif	// __CROSSSERVER_H__

