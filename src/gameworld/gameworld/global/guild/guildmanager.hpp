#ifndef __GUILD_MANAGER_HPP__
#define __GUILD_MANAGER_HPP__

#include <map>
#include <set>

#include "servercommon/serverdef.h"
#include "servercommon/struct/global/guildparam.hpp"

#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "protocal/msgscguild.hpp"
#include "protocal/msgguild.hpp"

class Guild;
class Role;

class GuildManager
{
	friend class RMIInitGuildBackObjectImpl;
	friend class RMICreateGuildBackObjectImpl;
	friend class RMIDismissGuildBackObjectImpl;

public:
	typedef std::map<GuildID, Guild*> GuildMap;
	typedef std::map<GuildID, Guild*>::iterator GuildMapIt;
	typedef std::map<GuildID, Guild*>::const_iterator GuildMapConstIt;

	static GuildManager & Instance();

	bool OnServerStart();
	void OnServerStop();

	void Update(unsigned long interval, time_t now_second);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	
	const GuildMap & GetGuildMap() { return m_guild_map; }
	Guild * GetGuild(GuildID guild_id);
	Guild * GetGuild(const UserID & user_id);

	void OnUserLogin(Role *role);
	void OnUserLogout(Role *role);
		
	// 仙盟操作相关----------------------------------------------------------------------------------------
	void CreateGuild(Role *role, gglobalprotocal::CreateGuild *create_req);
	void DismissGuild(Role *role, gglobalprotocal::DismissGuild *dismiss_req);
	void ApplyForJoin(Role *role, gglobalprotocal::ApplyForJoinGuild *join_req);
	void ApplyForJoinAck(Role *role, gglobalprotocal::ApplyForJoinGuildAck *join_ack);
	void QuitGuild(Role *role, gglobalprotocal::QuitGuild *quit_req);
	void Invite(Role *role, gglobalprotocal::InviteGuild *invite_req);
	void InviteAck(Role *role, gglobalprotocal::InviteGuildAck *invite_ack, bool is_autojoin = false);
	bool AutoJoinInviteAck(Role *role, gglobalprotocal::InviteGuildAck *invite_ack, bool is_autojoin = false);
	void KickoutGuild(Role *role, gglobalprotocal::KickoutGuild *kick_req);
	void Appoint(Role *role, gglobalprotocal::AppointGuild *appoint_req);				// 任命
	void LeavePost(Role *role, gglobalprotocal::LeavePostGuild *leavepost_req);
	void MailAll(Role *role, gglobalprotocal::GuildMailAll *mail_all_req);
	void ChangeNotice(Role *role, gglobalprotocal::GuildChangeNotice *chg_notice_req);
	void SetAutoKickoutSetup(Role *role, gglobalprotocal::GuildSetAutoKickoutSetup *gsssu);
	void Delate(Role *role, gglobalprotocal::GuildDelate *gd);						   // 弹劾
	bool DismissGuild(Guild *guild, const UserID &user_id, int dismiss_type);
	bool ExtendGuildMember(Role *role ,bool can_use_gold, short num);
	void SendGuildMemberCountInfo(Role *role);

	void OnCheckCanDelate(Role *role, Protocol::CSGuildCheckCanDelate *gccd);
	
	void GetAllGuildBaseInfo(Role *role, gglobalprotocal::AllGuildBaseInfo *all_guild_req);
	void GetApplyForList(Role *role, gglobalprotocal::GetApplyForList *apply_info_req);
	void GetApplyForJoinGuildList(Role *role);
	void GetMemberList(Role *role, gglobalprotocal::GetMemberList *memberlist_info_req, bool force_get = false);
	void GetGuildBaseInfo(Role *role, gglobalprotocal::GetGuildBaselInfo *base_info_req);
	void GetGuildEventList(Role *role, gglobalprotocal::GetGuildEventList *eventlist_info_req);
	void GetGuildInviteList(Role *role, gglobalprotocal::GetGuildInviteList *invitelist_info_req);

	void OnUserAddApplyFor(const UserID &user_id, GuildID guild_id);
	void OnUserRemoveApplyFor(const UserID &user_id, GuildID guild_id);
	void OnUserJoinGuildRemoveApplyFor(const UserID &user_id, GuildID guild_id);

	void OnGuildAddGuildExp(Role *role, gglobalprotocal::GuildAddExp *gae);
	void OnGuildMemberSos(Role *role, gglobalprotocal::GuildMemberSos *gms);

	bool OnGuildResetName(Role *role, gglobalprotocal::GuildResetName *grn);
	void OnGuildApplyforSetup(Role *role, gglobalprotocal::GuildApplyforSetup *afs);
	bool CanGuildResetName(Role *role);

	int GetGuildCamp(GuildID guild_id);

	void UserResetName(const UserID &user_id, GameName name);

	bool IsGuildLoadFinish() { return m_load_flag; }

	long long GetCampCapability(int camp, int level_limit, int login_time_limit);

	void SyncToRole(const UserID &user_id, GuildID guild_id, const GuildName guild_name, int post);
	void SyncToRole(Role *role, GuildID guild_id, const GuildName guild_name, int post);
	
	void SyncGuildBaseInfo(Role *role);

	void ClearXianmengzhanRankValue();
	void ClearGuildBattleRank();
	void ClearTerritoryWarRank();
	void ClearXianmengBipinKillBossCount();
	void RecordYesterdayCapability();

	void OnStartGuildFbReq(Role *role);
	void OnStartGuildBonfireReq(Role *role);
	void OnZhuFuMemberLuckyReq(Role *role, int be_zhufu_uid);
	void OnGetAllMemberLuckyInfoReq(Role *role);
	void OnInviteZhuFuLuckyReq(Role *role, int invite_uid);
	void OnCallBeastReq(Role *role);
	void OnActiveDegreeInfoReq(Role *role);
	
	void OnFetchGuildBossRedbag(Role *role, int index);
	void GuildBoxOperate(Role *role, int operate_type, int param1, int param2);
	void GuildBoxSendInfo(Role *role, GuildBoxInfo *guild_box_info);
	void GuildBoxUplevel(Role *role, int box_index);
	void GuildBoxOpen(Role *role, int box_index);
	void GuildBoxFetch(Role *role, int box_index, bool is_notice = true);
	void GuildBoxMialReward(Role *role, int box_index);
	void GuildBoxSendNeedAssistInfo(Role *role);
	void GuildBoxAssist(Role *role, int box_index, int target_uid);
	void GuildBoxCleanCD(Role *role);
	void GuildInviteAssist(Role *role, int target_uid);
	void GuildthankAssist(Role *role, int box_index, int target_uid);
	void GuildBoxUpdate(Role *role);
	bool GuildBoxIsStart(Role *role, bool is_notice);
	void GuildBoxGmOpenNow(Role *role, int box_index);
	void GuildBoxGmCleanCD(Role *role);
	void GuildBoxGmStart(bool is_start);

	void OnFetchDailyReward(Role *role);

	void OnFetchTerritoryReward(Role *role, int param1);
	void OnFetchTerritoryExtraReward(Role *role);

	int OnGmCommand(GuildID guild_id, int oper_type);

	void SendGuildRedPaperlistInfo(Role *role);
	void SingalChatRedPaperRole(Role *role, int red_paper_index);

	void SetAutoClear(Role *role, short is_auto_clear);

	void OnCrossGuildBattleBegin();
	void SendGuildBattleJoinReward(const std::set<GuildID> &ignore_guild_list);
	void ChangeAvatar(Role *role, unsigned long long avatar_timestamp);

	// 镖车------------------------------------------
	void OnBiaoCheStart(Role *role, GuildID guild_id);
	void OnTransToGuildBiaoCheReq(Role *role, GuildID guild_id);
	void OnYunBiaoStatus(gglobalprotocal::GuildYunBiaoStatus *gybs);
	void OnGetBiaoChePos(Role *role, GuildID guild_id);

	void OnGuildKillCrossBossRankReward();

	// 仙盟工资------------------------------------------
	void OnGiveGongzi(Role *role);			// 发工资
	bool OnAddGongzi(GuildID guild_id, int uid, int add_gongzi_type, int add_val, const char* reason); // 增加仙盟工资

	// 仙盟BOSS击杀榜------------------------------------------
	void OnAddGuildKillBossNum(GuildID guild_id, int num);
	void OnSyncCrossGuildKillRank(GuildID guild_id);

private:
	GuildManager();
	~GuildManager();

	void Init(int ret, const AllGuildParam &guild_list);
	void CreateGuildFailRollback(Role *role, int create_guild_type, const ItemDataWrapper &item_wrapper);

	void OnAddGuild(Guild *guild, bool is_create);
	void OnRemoveGuild(Guild *guild);

	Guild * AddGuild(const AllGuildParam::GuildParam &guild_param, bool is_create);
	void RemoveGuild(GuildID guild_id);

	void SyncLuckyInfo(Guild *guild, Role *role, int change_uid = 0, int flush_color = 0);

	int GmDismissGuild(Guild *guild);
	int GmClearNotice(Guild *guild);
	
	bool m_load_flag;
	GuildMap m_guild_map;

	typedef std::map<UserID, std::set<GuildID> > UserApplyForMap; 
	typedef std::map<UserID, std::set<GuildID> >::iterator UserApplyForMapIt;
	UserApplyForMap m_user_applyfor_mgr;

	bool m_guild_box_is_gm_start;
};

#endif // __GUILD_MANAGER_HPP__

