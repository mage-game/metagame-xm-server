#ifndef __GUILD_SHADOW_HPP__
#define __GUILD_SHADOW_HPP__

#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/guilddef.hpp"
#include "obj/character/monster.h"

#include <map>

class GuildShadow
{
public:
	static GuildShadow & Instance();

	void Update(unsigned long interval);

	void GetGuildName(GuildID guild_id, GuildName guild_name);
	int GetGuildLevel(GuildID guild_id);

	int GetGuildMemberCount(GuildID guild_id);
	int GetGuildCamp(GuildID guild_id);

	void OnGuildYunBiaoStart(gglobalprotocal::GuildYunBiaoStart *ybs);
	void OnGuildYunBiaoDead(GuildID guild_id, GuildID rob_guild_id, const UserID &rob_userid);
	void OnForceGuildYunBiaoEnd();																	// 活动结束时强制失败
	void OnGuildYunBiaoBeHurt(GuildID guild_id, GuildID hurt_guild_id, const UserID &hurt_userid);
	bool IsCanAttackBiaoChe(Role *role, Monster *monster);
	int GetGuildBiaocheObjId(GuildID guild_id);

private:
	GuildShadow();
	~GuildShadow();


	struct GuildBiaoCheInfo
	{
		GuildBiaoCheInfo() : guild_id(0), guild_lv(0), objid(INVALID_OBJ_ID), pos_seq(0), next_give_husong_exp_time(0), next_report_pos_time(0), next_sync_biaoche_pos_time(0), next_report_biaoche_behurt_time(0) {}

		GuildID guild_id;
		int guild_lv;

		ObjID objid;											// 镖车对象ID 
		int pos_seq;											// 当前镖车的点 出生点就是第一个点
		unsigned int next_give_husong_exp_time;					// 下次发护送经验的时间
		unsigned int next_report_pos_time;						// 下次广播镖车坐标的时间 
		unsigned int next_sync_biaoche_pos_time;				// 下次同步镖车坐标的时间
		unsigned int next_report_biaoche_behurt_time;			// 下次播报镖车被攻击的时间

		std::map<GuildID, std::set<UserID> > hurt_user_map;		// 所有对镖车有伤害的玩家
	};

	void BiaoCheGaveHusongExp(const GuildBiaoCheInfo &biaoche_info);
	void BiaoCheReportPos(const GuildBiaoCheInfo &biaoche_info);
	void UpdateBiaoChe(unsigned long interval);

	void OnGuildYunBiaoComplete(GuildID guild_id, int guild_lv);
	void OnGuildYunBiaoFail(GuildID guild_id, int guild_lv, GuildID rob_guild_id, const UserID &rob_userid);
	void SyncBiaoCheStatusToGlobal(int status, GuildID guild_id, int guild_lv, GuildID rob_guild_id, int biaoche_pos_x, int biaoche_pos_y);

	typedef std::map<GuildID, GuildBiaoCheInfo> GuildBiaoCheInfoMap;
	typedef std::map<GuildID, GuildBiaoCheInfo>::iterator GuildBiaoCheInfoMapIt;
	GuildBiaoCheInfoMap m_biaoche_map;

	time_t m_last_update_biaoche_time = 0;
};

#endif // __GUILD_SHADOW_HPP__

