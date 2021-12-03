#ifndef __GUILD_MEMBER_HPP__
#define __GUILD_MEMBER_HPP__

class GuildMemberManager;

#include "servercommon/servercommon.h"
#include "servercommon/struct/global/guildparam.hpp"
#include "obj/character/role.h"

class GuildMember
{
public:
	GuildMember(GuildMemberManager *member_manager);
	~GuildMember();

	GuildMemberManager * GetGuildMemberManager() { return m_member_manager; }

	void Init(const GuildMemberParam::Member &member_param);
	void GetAttr(GuildMemberParam::Member *member_attr) const;

	void OnDayChange();
	void OnMonthChange();

	const UserID & GetUserID() { return m_user_id; }

	char GetPost() { return m_post; }

	int GetGongxian() { return m_gongxian; }
	int GetTotalGongxian() { return m_total_gongxian; }

	long long GetJoinTime() { return m_join_time; }
	long long GetLastLoginTime() { return m_last_login_time; }

	unsigned int GetLastChangeNoticeTime() { return m_last_change_notice_time; }
	void SetLastChangeNoticeTime(unsigned int last_time) { m_last_change_notice_time = last_time; }
	unsigned int GetLastMailAllTime() { return m_last_mail_all_time; }
	void SetLastMailAllTime(unsigned int last_time) { m_last_mail_all_time = last_time; }

	void SetLuckyColor(int color);
	int GetLuckyColor() { return m_lucky_color; }

	void SetLastLoginTime(time_t now_second);

	void SetOnline(bool is_online) { m_is_online = is_online; }
	bool IsOnline() { return m_is_online; }

	void BeAppoint(int post);

	bool CheckNotifyPrincipal(int notify_type);

	void SetIsChongZhi(bool is_chongzhi) { m_is_chongzhi = is_chongzhi; }
	bool IsChongZhi() { return m_is_chongzhi; }

	void OnSyncInfo(int gongxian, int total_gongxian);

	void SetTodayLastGuildPaoSaiziTime(time_t value);
	long long GetTodayLastGuildPaoSaiziTime(){ return m_today_last_guild_pao_saizi_time; }

	void AddTodayGuildPaoSaiziTimes();
	int GetTodayGuildPaoSaiziTimes(){ return m_today_guild_pao_saizi_times; }

	void AddTodayGuildSaiziScore(int value);
	int GetTodayGuildSaiziScore(){ return m_today_guild_saizi_score; }

	void SetMazeLayer(int maze_layer);
	int GetMazeLayer() { return m_maze_layer; }

	void SetMazeCompleteTime(long long maze_complete_time);
	long long GetMazeCompleteTime() { return m_maze_complete_time; }

	void SetMazeCdEndTime(long long maze_cd_end_time) { m_maze_cd_end_time = maze_cd_end_time; }
	long long GetMazeCdEndTime() { return m_maze_cd_end_time; }

	void SetHasJoinGuildBattle(bool has_join_guild_battle);
	bool GetHasJoinGuildBattle() const { return m_common_param.has_join_guild_battle > 0; }

	void OnFetchDailyReward(Role *role);
	void SendDailyRewardTimes(Role *role, int rank = -1);

	// ���˹���----------------------------------------------------------------------------------
	void ResetGongzi() { m_common_param.gongzi = 0; }
	void AddGongzi(int gongzi) { m_common_param.gongzi += gongzi; }
	int GetGongzi() { return m_common_param.gongzi; }

	GuildMemberParam::MemberCommonParam &GetCommonData() { return m_common_param; }
	bool IsSigninSucc(Role *role);

	void * operator new(size_t c);
	void operator delete(void *m);

private:
	GuildMemberManager *m_member_manager;
	int m_member_index;							// ��Ա�±�

	UserID m_user_id;							// ��ɫID
	
	char m_post;								// ְλ

	int m_gongxian;								// ����
	int m_total_gongxian;						// �ܹ���
	
	bool m_is_chongzhi;							// �Ƿ��ֵ

	bool m_is_online;							// �Ƿ��ڵ�¼״̬
	long long m_join_time;						// ����ʱ��
	long long m_last_login_time;				// �����¼ʱ��

	unsigned int m_last_change_notice_time;     // ����޸����˹���ʱ��
	unsigned int m_last_mail_all_time;		    // ��������ʼ�ʱ��

	int m_lucky_color;							// ��������

	int m_today_guild_pao_saizi_times;			// ÿ�칫�������Ӵ���
	long long m_today_last_guild_pao_saizi_time;// ���һ��������ʱ��
	int m_today_guild_saizi_score;				// ÿ�����ӻ���

	int m_maze_layer;							// �Թ���ǰ��
	long long m_maze_complete_time;				// �Թ�ͨ��ʱ��
	long long m_maze_cd_end_time;				// CD����ʱ��

	GuildMemberParam::MemberCommonParam m_common_param;				// ����ͨ������
};

#endif // __GUILD_MEMBER_HPP__

