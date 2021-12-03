#ifndef _SPECIAL_CROSS_LIEKUNFB_HPP_
#define _SPECIAL_CROSS_LIEKUNFB_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include <map>

static const int CORSS_LIEKUN_RETAIN_OWNER_TIME_S = 60;  // 猎鲲副本掉线归属保留时间 秒

class SpecialCrossLieKunFb : public SpecialLogic
{
public:
	struct BossItem
	{
		BossItem() { this->Reset(); }

		void Reset()
		{
			boss_obj = 0;
			boss_id = 0;
			replace_boss_id = 0;
			born_pos = Posi(0, 0);
			boss_live_flag = false;
			boss_next_flush_timestamp = 0;
			guild_id = 0;
			guild_uuid = INVALID_UNIQUE_USER_ID;
			guild_hurt_map.clear();
		}

		void BossReset()
		{
			guild_id = 0;
			guild_uuid = INVALID_UNIQUE_USER_ID;
			guild_hurt_map.clear();
		}

		struct GuildHurt
		{
			GuildHurt() : guild_id(INVALID_UNIQUE_USER_ID), hurt(0), logout_timestamp(0),rank(0)
			{
				uuid_map.clear();
			}

			UniqueUserID guild_id;							// 帮派id
			long long hurt;									// dps
			time_t logout_timestamp;						// 帮派成员全部掉线的时间更新标记
			int rank;

			std::map<long long, long long> uuid_map;		// 帮派成员dps表 param1成员uuid param2dps
		};

		ObjID boss_obj;										// boss的objid
		int boss_id;										// 原bossid
		int replace_boss_id;								// 替换后bossid
		Posi born_pos;

		bool boss_live_flag;								// boss存活标记
		unsigned int boss_next_flush_timestamp;				// boss下次刷新时间戳

		GuildID guild_id;									// boss最终归属帮派id
		UniqueUserID guild_uuid;							// boss最终归属帮派uuid
		
		std::map<UniqueUserID, GuildHurt> guild_hurt_map;	// 帮派dps表
	};

	struct GuildItem
	{
		GuildItem() : guild_id(INVALID_UNIQUE_USER_ID)
		{
			memset(guild_name, 0, sizeof(guild_name));
		}

		UniqueUserID guild_id;	// 帮派uuid
		GuildName guild_name;	// 帮派名
	};

public:
	SpecialCrossLieKunFb(Scene *scene);
	virtual ~SpecialCrossLieKunFb();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual void OnAddObj(Obj *obj);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialCanReAlive(Role *role, int realive_type);
	virtual bool CanDestroy() { return !m_activity_open; }

	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);		// 采集
	virtual bool SpecialCanGather(Role *role, GatherObj *gather);
	virtual bool SpecialCanPick(Role *role, FallingItem *falling_item);

	void FlushGather(bool boss_is_live, int boss_id);										// 刷新采集物
	void FlushPartal(bool boss_is_live);													// 刷新传送门

	void SyncFBSceneInfo();																	// 同步副本信息
	void SyncFBLiveFlagInfo();																// 同步主boss存活标记信息
	void SyncKillZoneBossGuildId(UniqueUserID highest_dps_guild_id);						// 同步击杀主boss帮派信息
	int GetSceneInfo();

	void OnActivityOpen();
	void OnActivityClose();

private:
	bool IsMainZone();
	void CreateMainZoneBoss(time_t now_second);						// 创建主区域的区域boss
	void CreateMinorZoneBoss(time_t now_second);					// 创建副区域的区域boss
	UniqueUserID GetGuildUniqueUserId(Role *role);
	SpecialCrossLieKunFb::BossItem *GetBossItemById(int boss_id);

	void CheckAllBossBound();															// 检查全部boss归属（帮派成员远离boss，将帮派成员从boss的dps表中去除）
	void CheckSingleBossBound(int boss_type/*, long long *high_hurt_role_uuid = NULL*/);	// 检查单个boss归属（帮派成员远离boss，将帮派成员从boss的dps表中去除）

	void KickAllToOrginalToServer();

private:
	bool m_first;
	bool m_activity_open;
	short m_cur_zone;								// 所在的区域

	bool m_init_jy_boss_id;							// 初始化精英boss等级
	bool m_is_enter_main_zone;						// 主区域是否有人进入过

	bool m_enter_close;								// 入口关闭标志
	unsigned int m_enter_close_timestamp;			// 入口关闭标志时间戳

	BossItem m_boss_list[LIEKUN_BOSS_TYPE_COUNT];	// 副本内boss信息

	typedef std::map<UniqueUserID, SpecialCrossLieKunFb::GuildItem> GuildMap;
	GuildMap m_guild_map;							// 帮派id和帮派名映射表

	unsigned int m_check_user_time;					// 下次检查更新玩家信息的时间，直接默认是1秒检查一次，不然每次TICK都检查会很频繁
};

#endif // _SPECIAL_CROSS_LIEKUNFB_HPP_