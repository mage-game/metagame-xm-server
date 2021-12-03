#ifndef __GUILD_FB_HPP__
#define __GUILD_FB_HPP__

#include <map>
#include <set>
#include <stdio.h>

#include "iclockmodule.h"
#include "scene/speciallogic/speciallogic.hpp"

#include "config/guildactivityconfig.hpp"
#include "servercommon/serverdef.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "protocal/msgguild.hpp"
#include "protocal/msgfb.h"
#include "servercommon/string/globalstr.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "global/guild/guild.hpp"

#include <map>
#include <vector>

class SpecialGuildFb : public SpecialLogic
{
	struct HurtMonsterInfo
	{
		HurtMonsterInfo() : role_id(0), hurt_val(0)
		{
			memset(user_name, 0, sizeof(user_name));
		}

		bool operator <(const HurtMonsterInfo &other) const
		{
			return hurt_val > other.hurt_val;
		}

		int role_id;
		GameName user_name;
		long long hurt_val;
	};

public:
	SpecialGuildFb(Scene *scene);
	virtual ~SpecialGuildFb();

	virtual bool CanDestroy();

	virtual void OnRoleEnterScene(Role *role);
	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual bool OnTimeout();

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);

	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialCanMountOn(Role *role) {return false;}

	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);

	bool IsFbOpen() { return m_is_open; } 
	void SetFbIndex(int fb_index) { m_fb_index = fb_index; }

	void OnGuildFbOpen();
	void OnGuildFbClose();

	bool GetCurFbIndexFirstPos(Posi *get_pos);
	bool GetGuardBossPos(Posi *get_pos);

	void AddCurIndexRewardRecord(Role *role, const Protocol::SCGuildFBRewardRecordInfo::ItemInfo &reward_item); // 添加当前关卡奖励记录

	void GiveFbIndexRewardToSceneRole();																		// 发放每关卡奖励

	void OnCreateNextFbIndex();																				    // 创建下一个关卡
	void OnNextFbIndexStart();																				    // 下一个关卡开始											
	Guild *GetOwnerGuild();																					    // 获取所属仙盟

private:
	void OnGuardBossDead();										// 守卫boss死亡
	void OnEnemyDead();											// 怪物死亡
	void UpdateGuardBossAI();
	void FlushWaveEnemyMonster();								// 刷新怪物

	void SyncInfoToRole(Role *role, int reason);				// 发送副本信息
	void SyncRewardRecordToSceneRole();							// 给场景内玩家发送信息
	void SendHistoryRewardRecordInfo(Role *role);				// 发送历史奖励记录信息
	void SendCurWaveRewardInfo(Role *role);						// 发送当前关卡奖励记录信息

	void OnFbFinish(bool is_pass);

	bool m_is_open;
	bool m_is_pass;
	bool m_is_finish;

	ObjID m_guard_boss_objid;				// 守卫bossOBJID

	int m_fb_index;							// 当前关卡索引
	int m_fb_index_enemy_count;				// 当前关卡怪剩余数量
	int m_fb_index_enemy_max;				// 当前关卡怪配置数量
	time_t m_next_fb_index_time;			// 下一关卡开始时间

	int m_wave_index;						// 当前波数索引
	int m_wave_enemy_count;					// 当前波数怪剩余数量

	int m_guard_boss_pos_idx;				// 守卫boss当前坐标索引

	time_t m_last_sync_time;				// 下一次同步信息时间s
	
	unsigned int m_kick_role_time;			// 踢玩家时间

	std::map<int, std::vector<Protocol::SCGuildFBRewardRecordInfo::ItemInfo> > m_cur_wave_reward_record_map;   // 当前关卡奖励记录表 uid - reward_vec
};

#endif // __GUILD_FB_HPP__



