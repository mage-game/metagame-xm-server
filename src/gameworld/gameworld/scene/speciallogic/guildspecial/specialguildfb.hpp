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

	void AddCurIndexRewardRecord(Role *role, const Protocol::SCGuildFBRewardRecordInfo::ItemInfo &reward_item); // ��ӵ�ǰ�ؿ�������¼

	void GiveFbIndexRewardToSceneRole();																		// ����ÿ�ؿ�����

	void OnCreateNextFbIndex();																				    // ������һ���ؿ�
	void OnNextFbIndexStart();																				    // ��һ���ؿ���ʼ											
	Guild *GetOwnerGuild();																					    // ��ȡ��������

private:
	void OnGuardBossDead();										// ����boss����
	void OnEnemyDead();											// ��������
	void UpdateGuardBossAI();
	void FlushWaveEnemyMonster();								// ˢ�¹���

	void SyncInfoToRole(Role *role, int reason);				// ���͸�����Ϣ
	void SyncRewardRecordToSceneRole();							// ����������ҷ�����Ϣ
	void SendHistoryRewardRecordInfo(Role *role);				// ������ʷ������¼��Ϣ
	void SendCurWaveRewardInfo(Role *role);						// ���͵�ǰ�ؿ�������¼��Ϣ

	void OnFbFinish(bool is_pass);

	bool m_is_open;
	bool m_is_pass;
	bool m_is_finish;

	ObjID m_guard_boss_objid;				// ����bossOBJID

	int m_fb_index;							// ��ǰ�ؿ�����
	int m_fb_index_enemy_count;				// ��ǰ�ؿ���ʣ������
	int m_fb_index_enemy_max;				// ��ǰ�ؿ�����������
	time_t m_next_fb_index_time;			// ��һ�ؿ���ʼʱ��

	int m_wave_index;						// ��ǰ��������
	int m_wave_enemy_count;					// ��ǰ������ʣ������

	int m_guard_boss_pos_idx;				// ����boss��ǰ��������

	time_t m_last_sync_time;				// ��һ��ͬ����Ϣʱ��s
	
	unsigned int m_kick_role_time;			// �����ʱ��

	std::map<int, std::vector<Protocol::SCGuildFBRewardRecordInfo::ItemInfo> > m_cur_wave_reward_record_map;   // ��ǰ�ؿ�������¼�� uid - reward_vec
};

#endif // __GUILD_FB_HPP__



