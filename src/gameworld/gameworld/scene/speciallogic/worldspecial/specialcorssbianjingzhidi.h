#ifndef __SPECIAL_CROSS_BIANJINGZHIDI_HPP__
#define __SPECIAL_CROSS_BIANJINGZHIDI_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include <vector>
#include <set>

// K-跨服-边境之地

class SpecialCrossBianJingZhiDi : public SpecialLogic
{
public:
	struct BossItem
	{
		BossItem() { this->Reset(); }

		void Reset()
		{
			boss_obj = INVALID_OBJ_ID;
			boss_id = 0;
			born_pos = Posi(0, 0);
			boss_live_flag = false;
			boss_next_flush_timestamp = 0;
			guild_id = 0;
			guild_uuid = INVALID_UNIQUE_USER_ID;
			memset(guild_name, 0, sizeof(GuildName));
			guild_hurt_map.clear();
			gather_obj = INVALID_OBJ_ID;
		}

		void BossReset()
		{
			guild_id = 0;
			guild_uuid = INVALID_UNIQUE_USER_ID;
			memset(guild_name, 0, sizeof(GuildName));
			guild_hurt_map.clear();
		}

		struct GuildHurt
		{
			GuildHurt() : guild_id(INVALID_UNIQUE_USER_ID), hurt(0), logout_timestamp(0), rank(0)
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
		int boss_id;										// bossid
		Posi born_pos;

		bool boss_live_flag;								// boss存活标记
		unsigned int boss_next_flush_timestamp;				// boss下次刷新时间戳

		GuildID guild_id;									// boss最终归属帮派id
		UniqueUserID guild_uuid;							// boss最终归属帮派uuid
		GuildName guild_name;								// boss最终归属帮派名
		std::map<UniqueUserID, GuildHurt> guild_hurt_map;	// 帮派dps表

		ObjID gather_obj;									// boss死亡后采集宝箱的objId
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
	SpecialCrossBianJingZhiDi(Scene *scene);
	virtual ~SpecialCrossBianJingZhiDi();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnAddObj(Obj *obj);
	void CheckBornBoss(time_t now_second);
	void CheckBornMonster(time_t now_second);

	virtual bool CanDestroy() { return m_is_finish; }

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);

	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);

	virtual void OnCharacterDie(Character *dead_cha, ObjID killer_objid);
	virtual void OnHurtOther(Character *cha, Character *target);

	void OnKillBoss(Role *role, Monster* boss, int boss_type);
	virtual void OnStartGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual bool SpecialCanGather(Role *role, GatherObj *gather);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual bool SpecialCanMountOn(Role *role){return false;}
	
	virtual int GetSpecialParam(Role *role);
	virtual bool SpecialStopGahterOnAttack(Role *role);

	void OnActivityStatusChange(int status);

	CrossBianJingZhiDiUser * GetCrossBianJingZhiDiUser(const UniqueUserID &user_id);

	void OnBuyGatherBuff(Role * role);
	SpecialCrossBianJingZhiDi::BossItem *GetBossItemByGatherObjId(ObjID gather_obj);
	int GetBossTypeByBossObjId(ObjID boss_obj, int boss_id);

	void SendGuildSos(Protocol::CSSendGuildSosReq *req, Role *role);
	void ReplyGuildSos(Protocol::CSReplyGuildSosReq *sos_reply, Role *role);

protected:
	void SendCrossBianJingZhiDiInfoToAllUser();
	void SendCrossBianJingZhiDiUserInfo(Role *role);
	void CheckUserTaskInfo(Role *role, short task_type, int param_id, const std::set<UniqueUserID> &uuid_set);  // uuid_set助攻玩家，比如对死亡的BOSS或者角色进行过伤害
	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);
	void SyncCrossBianJingZhiDiBossInfoToCross();

	void OnActivityStandby();
	void OnActivityStart();
	void OnActivityClose();

	void OnCrossBianJingZhiDiClose();

	void KickOutAllRole();
	void CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist = false);

	void SendBossInfo(Role *role = nullptr);
	void CheckSendBossInfo();

	void CheckGatherBuffInfo();
	void NotifyGatherBuffInfo(Role *role, short is_has_buff);

	UniqueUserID GetGuildUniqueUserId(Role *role);
	void CheckAllBossBound();								// 检查全部boss归属（帮派成员远离boss，将帮派成员从boss的dps表中去除）
	void CheckSingleBossBound(int boss_type);				// 检查单个boss归属（帮派成员远离boss，将帮派成员从boss的dps表中去除）

	bool m_is_finish;

	time_t m_next_reflush_boss_time;
	time_t m_next_reflush_monster_time;
	time_t m_reflush_notice_time;									// 边境BOSS刷新提示时间
	bool m_is_notice;
	
	std::map<long long, time_t> m_role_task_time_map;				// 任务时间

	unsigned m_send_boss_info_time;

	int m_monster_die_num;
	unsigned int m_next_check_buff_time;
	CrossBianJingZhiDiUserMap m_bianjingzhidi_usermap;				// 跨服边境之地用户列表

	BossItem m_boss_list[CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE];		// 副本内boss信息

	typedef std::map<UniqueUserID, SpecialCrossBianJingZhiDi::GuildItem> GuildMap;
	GuildMap m_guild_map;											// 帮派id和帮派名映射表
	
};

#endif // __SPECIAL_CROSS_BIANJINGZHIDI_HPP__
