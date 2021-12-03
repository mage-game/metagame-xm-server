#ifndef __GUILD_STATION_HPP__
#define __GUILD_STATION_HPP__

#include <map>
#include <set>
#include <stdio.h>

#include "iclockmodule.h"
#include "scene/speciallogic/speciallogic.hpp"

#include "config/guildactivityconfig.hpp"
#include "servercommon/serverdef.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "protocal/msgguild.hpp"
#include "servercommon/string/globalstr.h"

class SpecialGuildStation : public SpecialLogic
{
	struct HurtBossInfo
	{
		HurtBossInfo() : role_id(0), hurt_val(0)
		{
			memset(user_name, 0, sizeof(user_name));
		}

		bool operator <(const HurtBossInfo &other) const
		{
			return hurt_val > other.hurt_val;
		}

		int role_id;
		GameName user_name;
		long long hurt_val;
	};

	struct RoleFbInfo
	{
		RoleFbInfo() : gather_num(0), reserve_sh(0)
		{

		}

		short gather_num;			// 已采集数量
		short reserve_sh;
	};

public:
	SpecialGuildStation(Scene *scene);
	virtual ~SpecialGuildStation();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool CanDestroy() { return false; }

	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual bool SpecialCanGather(Role *role, GatherObj *gather);
	virtual void OnAOIRoleEnter(Role *role, Role *target_role);

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);

	virtual bool SpecialRelive(Role *role, int realive_type);

	bool CallGuildBoss(Role *role, int boss_level, bool is_super_boss);
	int GetGuildBossId() { return m_boss_id; }
	void ResetGuildBoss();						// 重置boss相关数据

	void CheckUpdateStarLevel(unsigned int now_second);

	void CalcRoleHurtBossVal(Role *role, Attribute delta_hp);

	void CheckNoticeBossTimeLimit(time_t now_second);			// 检查提示活动剩余时间

	void SendGuildBossInfo(int notify_type = 0, Role *role = NULL);

	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);

	//天赐铜币----------------------------------------------------------------
	bool IsTianCiTongBiOpen() {  return m_tianci_tongbi_is_open; }
	void OnRoleGatherBigCoin(Role *role, GatherObj *bigcoin);			// 采集
	void BroadcastGatherInfo(Role *role, int gather_type);				// 广播采集
	void OnOpenTianCiTongBi();											// 开启活动
	void ForceTianCiTongBiNextState();									// 强制进入下一状态
	void OnTianCiTongBiUseGather(Role *role, bool is_auto = false);		// 提交采集物
	void SendTianCiTongBiRankInfo(Role *role = nullptr);				// 发送排名信息

	void AddTreeMaturity(int maturity);									// 增加成熟度

private:
	void GuildBossReward();

	void SortHurtRank();		// 伤害排序
	void GiveRankReward();		// 发放排行奖励

	void CreateBossGather();	// 创建boss采集物
	void TotemBless();

	//天赐铜币----------------------------------------------------------------
	Role * GetRole(GuildTianCiTongBiUser &dps_user);						  // 获取role
	GuildTianCiTongBiUser * GetGuildTianCiTongBiUser(const UserID user_id);	  // 获取铜币user

	void OnTianCiTongBiOpen();												  // 天赐铜币开启
	void OnTianCiTongBiClose();												  // 天赐铜币关闭
	void TianCiTongBiCheckGather();											  // 检查采集物刷新（暂时无用）
	void TianCiTongBiRefreshGather();										  // 刷新采集物
	void TianCiTongBiRefreshBigCoin();										  // 刷新铜币
	void TianCiTongBiRoleGatherChange(Role *role, int gather_type);			  // 采集状态改变
	void RefreshTianCiTongBiRank(Role *role = nullptr);						  // 刷新排名
	void ClearTianCiTongBiGahter();											  // 清除采集物
	void ClearTianCiTongBiBigCoin();										  // 清除铜币
	void SendGuildTianCiTongBiNpcInfo(Role *role = nullptr);
	void SendTianCiTongBiRoleGatherInfo(int flag = 0);

	int m_call_boss_uid;
	int m_boss_level;
	int m_boss_id;
	ObjID m_boss_objid;
	bool m_is_super_boss;
	bool m_is_notice_boss_timelimit;							// 是否已提示boss活动剩余时间
	unsigned int m_boss_dayid;

	time_t m_next_bless_time;
	unsigned int m_next_update_rank_time;						// 下次刷排行榜时间戳

	unsigned int m_next_change_star_time;						// 下次改变星级时间戳
	int m_cur_star_level;										// 当前星级

	std::map<int, unsigned int> m_boss_hurt_uid_list;
	std::set<ObjID> m_gathar_objid_list;						// boss采集宝箱

	std::vector<HurtBossInfo> m_hurt_boss_info_vec;		// 对boss伤害排名信息

	typedef std::map<int, RoleFbInfo> PlayerInfoMap;
	typedef std::map<int, RoleFbInfo>::iterator PlayerInfoMapIt;
	PlayerInfoMap m_role_info_map;								// 玩家副本信息

	//天赐铜币----------------------------------------------------------------
	//GuildBossDPSUserMap m_guildboss_dps_map;

	bool m_tianci_tongbi_is_open;																			// 天赐铜币是否开启
	unsigned int m_tianci_tongbi_open_time;																	// 天赐铜币开始时间
	unsigned int m_tianci_tongbi_close_time;																// 天赐铜币结束时间
	int m_tianci_tongbi_tree_maturity_degree;																// 天赐铜币树成熟度
	int m_tianci_tongbi_readiness_time;																		// 活动开始准备时间
	Posi m_tianci_tongbi_npc_pos;																			// 天赐铜币npc坐标

	GuildTianCiTongBiUserMap m_guild_tianci_tongbi_user_map;												// 天赐铜币用户列表

	//unsigned int m_next_refresh_tianci_tongbi_rank_time;
	GuildTianCiTongBiRankItemVec m_guild_tianci_tongbi_rank_vec;

	std::set<ObjID> m_tianci_tongbi_big_coin_objid_set;														// 天赐铜币大金币列表

	struct TianCiTongBiGatherInfo
	{
		TianCiTongBiGatherInfo() : obj_id(INVALID_OBJ_ID), next_refresh_time(0) {}

		ObjID obj_id;
		unsigned int next_refresh_time;
	};
	TianCiTongBiGatherInfo m_tianci_tongbi_gather_list[GUILD_TIANCI_TONGBI_GAHTER_NUM_MAX];
	int m_gather_num;																						// 已采集数量

	unsigned int m_next_tianci_tongbi_check_gather_time;
	std::map<int, int> m_guild_xianci_tongbi_gather_type_map;												// uid - gather_type 天赐铜币 采集物类型(不保存)
};

#endif // __GUILD_STATION_HPP__

