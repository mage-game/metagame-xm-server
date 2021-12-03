#ifndef __GUILD_MEMBER_MANAGER_HPP__
#define __GUILD_MEMBER_MANAGER_HPP__

#include <map>

#include "servercommon/struct/global/guildparam.hpp"
#include "servercommon/internalprotocal/gglobalprotocal.h"

#include "protocal/msgscguild.hpp"
#include "protocal/msgguild.hpp"

class Guild;
class GuildMember;
class Role;

class GuildMemberManager
{
public:
	GuildMemberManager();
	~GuildMemberManager();

	void SetGuild(Guild *guild) { m_guild = guild;}
	Guild * GetGuild() { return m_guild; }

	void Init(const GuildMemberParam &member_param);
	void GetChangeMemberList(GuildMemberParam *member_param);
	void ClearDirtyMark();

	void OnDayChange();
	void OnMonthChange();

	GuildMember * AddMember(const UserID &user_id, int post); 
	void SendWelcomChat(const UserID &user_id);
	bool RemoveMember(const UserID &user_id);
	GuildMember * GetMember(const UserID &user_id, int *member_index = NULL);
	GuildMember * GetReplaceTuanZhangMember();
	GuildMember * GetTuanZhangMember();
	GuildMember * GetFuTuanZhangMember();
	int GetTuanZhangUid();
	int GetFuTuanZhangUid();

	int GetMemberCount();
	void GetMemberList(Protocol::SCGuildMemberList *member_list);
	int GetMemberList(int member_uid_list[MAX_MEMBER_COUNT]);
	int GetOnlineMemberList(int member_uid_list[MAX_MEMBER_COUNT]);
	void GetMemberLuckyInfo(Protocol::SCGuildLuckyInfo *gli);
	bool HasOnlineMember();

	int GetCapability(int level_limit = 0, int login_time_limit = 0);
	int GetInviteDayCount() { return m_day_invite_count; }

	int Appoint(GuildMember *appoint_member, GuildMember *beappoint_member, int post);
	void LeavePost(GuildMember *leavepost_member);
	void Dismiss();
	void CheckAutoKickout();

	void SystemMailToAll(int mail_reason);
	void SystemMailToAll(MailSubject subject, MailContentTxt contenttxt);
	void SystemMailToAll(const MailContentParam &mail_content_param, bool is_send_to_tuanzhang = true);
	void SystemMailToPrincipal(MailSubject subject, MailContentTxt contenttxt, int notify_type);
	void SystemMailToPrincipal(const MailContentParam &mail_content_param, int notify_type);
	void SendMailToAll(Role *role, gglobalprotocal::GuildMailAll *mail_all_req);

	void NotifyMsgToPrincipal(int notify_type, GuildMember *member = NULL, int notify_param = 0, int nofity_param1 = 0);
	void SendMessageToAll(int notify_type, const char *data, int len, GuildMember *member = NULL);
	void SendMessageToPrincipal(int notify_type, const char *data, int len, GuildMember *member = NULL);
	void OnChat(const char* buff, int total_len);
	bool OnSystemMsg(const char* buff, int total_len, int filter_member_count = 0, const UserID *filter_member_list = NULL);

	void SetDirty(int member_index);

	void OnSyncGuildInfoToAllMember();
	void OnSyncGuildSorgeChangeToAllMember(Protocol::SCGuildStorgeChange &cmd);
	void OnRefreshGuildAppearanceInfo();
	
	void SyncSCGulidSaiziInfo(Role *role, int pao_saizi_num = -1);
	void CalcSaiziRank();
	void ReqGuildPaoSaizi(Role *role);
	int GetSaiziRankByUid(int uid);
	void GuildleaderGuildChat(void* msg, int length);  // 以盟主身份发送仙盟聊天
	void SendSaiziRankMsg();
	void SendSaiziReward();

	void SendGuildBattleDailyRewardInfo(int rank);

	void AutoClearMember(unsigned int now_sec);

	int GetMemberSigninTotalCountToday() const { return m_total_member_signin_count_today; }
	bool IsSigninSucc(Role *role);

	int GetMemberGongziRate(int uid);						// 获取成员工资比例
	int GetRealGongziTotalRate();							// 获取工资实际总权重

	int GetPostCount(int post);								// 获取该职位人数

	//系统聊天
	void GuildSystemChat(void* msg, int length, int origin_type = chatdef::ORIGIN_TYPE_GUILD_SYSTEM_CHAT);

private:
	bool IsPostFull(int post);

	int GetPrincipalList(int notify_type, UserID user_list[MAX_MEMBER_COUNT], bool check_online = false);

	Guild *m_guild;
	GuildMember *m_member_list[MAX_MEMBER_COUNT];

	bool m_dirty_mark[MAX_MEMBER_COUNT];
	bool m_old_state[MAX_MEMBER_COUNT];
	bool m_change_flag;

	typedef std::map<UserID, int> UserIDIndexMap;
	typedef std::map<UserID, int>::iterator UserIDIndexMapIt;
	UserIDIndexMap m_userid_index_map;

	int m_day_invite_count;			// 每日仙盟邀请次数
	std::vector<GuildMember *> m_guild_saizi_rank_vec;		// 骰子积分排行榜

	int m_total_member_signin_count_today;
};

#endif // __GUILD_MEMBER_MANAGER_HPP__

