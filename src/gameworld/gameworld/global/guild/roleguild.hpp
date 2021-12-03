#ifndef __GUILD_ROLE_GUILD_HPP__
#define __GUILD_ROLE_GUILD_HPP__

struct RoleCrossGlobalData;
class Role;

#include "servercommon/guilddef.hpp"
#include "obj/character/attribute.hpp"

class RoleGuild
{
public:
	RoleGuild();
	~RoleGuild();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const RoleGuildParam &role_guild_param);
	void GetInitParam(RoleGuildParam *role_guild_param);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	// 跨服中获取不到该战力，需要同步到RoleCrossGlobalData中
	void GetRoleCrossData(RoleCrossGlobalData &data);

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnWeekChange();

	void GuildExchange(ItemID item_id, short exchange_num);
	void SendRoleGuildInfo();

	void OnSkillUplevel(int skill_index);
	//void OnSkillUplevelALL();
	void OnSkillUplevelALLByLowestLevel();
	void OnRoleLogin();
	void OnLeaveGuild();
	void OnJoinGuild();
	unsigned int GetLastLeaveGuildTime();
	unsigned int GetLastJoinGuildTime();

	int GetDailyGongXian() { return m_guild_param.daily_gongxian; }
	short GetTerritoryWelfFlag() {return m_guild_param.territorywar_reward_flag;}
	long long GetTotemExp() { return m_guild_param.guild_boss_activity_totem_total_exp; }
	void SetDailyGongXian(int gongxian) { m_guild_param.daily_gongxian = gongxian; }
	void ResetTerritoryRewardDailyFlag(){ m_guild_param.territorywar_reward_flag = 0;}
	void ResetTotemExp(){ m_guild_param.guild_boss_activity_totem_total_exp = 0;}
	void AddTotemExp(long long totem_exp){ m_guild_param.guild_boss_activity_totem_total_exp += totem_exp;}

	RoleGuildParam * GetRoleGuildParam(){ return &m_guild_param;}

	int GetGuildStorageScore() { return m_guild_param.storage_score; }
	void SetGuildStorageScore(int storage_score) { m_guild_param.storage_score = storage_score; }

	void OnLoginNoticeGuildRedPaperInfo();
	void NoticeGuildRedPaperInfo(int notice_reason);
	void CreateGuildRedPaper(int red_paper_seq, int can_fetch_time, int red_paper_index);
	bool AddGuildRedPaperAuthority(int paper_type, int cond_param);
	void OpenServerSyncRedPapaerInfo();
	void SyncRedPaperInfoToGuild();

	int GetMazeRawardLayer() { return m_guild_param.maze_reward_layer; }
	void SetMazeRawardLayer(int maze_reward_layer) { m_guild_param.maze_reward_layer = maze_reward_layer; }

	// 天赐铜币-------------------------------------------
	int GetGuilaTianCiTongBiRewardLimit() { return m_guild_param.guild_tianci_tongbi_reward_limit; }
	void AddGuilaTianCiTongBiRewardLimit(int param) { m_guild_param.guild_tianci_tongbi_reward_limit += param; }

	bool IsCanJoinTianCiTongBi();
	void SetGuilaTianCiTongBiJoinGuild(GuildID guild_id) { m_guild_param.join_tiancitongbi_guild_id = guild_id; }

	// 捐献-----------------------------------------------
	int GetTodayJuanxianGold() { return m_guild_param.today_juanxian_gold; }
	void AddTodayJuanxianGold(int gold);

	// 掷骰子--------------------------------------------
	void ResetTodayGuildPaoSaiziTimes();
	int GetTodayGuildPaoSaiziTimes() { return m_guild_param.today_paosaizi_times; }
	void AddTodayGuildPaoSaiziTimes() { m_guild_param.today_paosaizi_times++; }
	
private:
	Role *m_role;
	RoleGuildParam m_guild_param;
	CharIntAttrs m_attrs_add;
};

#endif // __GUILD_XIANSHU_HPP__

