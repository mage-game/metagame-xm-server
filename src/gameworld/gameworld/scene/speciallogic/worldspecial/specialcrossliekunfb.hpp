#ifndef _SPECIAL_CROSS_LIEKUNFB_HPP_
#define _SPECIAL_CROSS_LIEKUNFB_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include <map>

static const int CORSS_LIEKUN_RETAIN_OWNER_TIME_S = 60;  // ���︱�����߹�������ʱ�� ��

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

			UniqueUserID guild_id;							// ����id
			long long hurt;									// dps
			time_t logout_timestamp;						// ���ɳ�Աȫ�����ߵ�ʱ����±��
			int rank;

			std::map<long long, long long> uuid_map;		// ���ɳ�Աdps�� param1��Աuuid param2dps
		};

		ObjID boss_obj;										// boss��objid
		int boss_id;										// ԭbossid
		int replace_boss_id;								// �滻��bossid
		Posi born_pos;

		bool boss_live_flag;								// boss�����
		unsigned int boss_next_flush_timestamp;				// boss�´�ˢ��ʱ���

		GuildID guild_id;									// boss���չ�������id
		UniqueUserID guild_uuid;							// boss���չ�������uuid
		
		std::map<UniqueUserID, GuildHurt> guild_hurt_map;	// ����dps��
	};

	struct GuildItem
	{
		GuildItem() : guild_id(INVALID_UNIQUE_USER_ID)
		{
			memset(guild_name, 0, sizeof(guild_name));
		}

		UniqueUserID guild_id;	// ����uuid
		GuildName guild_name;	// ������
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

	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);		// �ɼ�
	virtual bool SpecialCanGather(Role *role, GatherObj *gather);
	virtual bool SpecialCanPick(Role *role, FallingItem *falling_item);

	void FlushGather(bool boss_is_live, int boss_id);										// ˢ�²ɼ���
	void FlushPartal(bool boss_is_live);													// ˢ�´�����

	void SyncFBSceneInfo();																	// ͬ��������Ϣ
	void SyncFBLiveFlagInfo();																// ͬ����boss�������Ϣ
	void SyncKillZoneBossGuildId(UniqueUserID highest_dps_guild_id);						// ͬ����ɱ��boss������Ϣ
	int GetSceneInfo();

	void OnActivityOpen();
	void OnActivityClose();

private:
	bool IsMainZone();
	void CreateMainZoneBoss(time_t now_second);						// ���������������boss
	void CreateMinorZoneBoss(time_t now_second);					// ���������������boss
	UniqueUserID GetGuildUniqueUserId(Role *role);
	SpecialCrossLieKunFb::BossItem *GetBossItemById(int boss_id);

	void CheckAllBossBound();															// ���ȫ��boss���������ɳ�ԱԶ��boss�������ɳ�Ա��boss��dps����ȥ����
	void CheckSingleBossBound(int boss_type/*, long long *high_hurt_role_uuid = NULL*/);	// ��鵥��boss���������ɳ�ԱԶ��boss�������ɳ�Ա��boss��dps����ȥ����

	void KickAllToOrginalToServer();

private:
	bool m_first;
	bool m_activity_open;
	short m_cur_zone;								// ���ڵ�����

	bool m_init_jy_boss_id;							// ��ʼ����Ӣboss�ȼ�
	bool m_is_enter_main_zone;						// �������Ƿ����˽����

	bool m_enter_close;								// ��ڹرձ�־
	unsigned int m_enter_close_timestamp;			// ��ڹرձ�־ʱ���

	BossItem m_boss_list[LIEKUN_BOSS_TYPE_COUNT];	// ������boss��Ϣ

	typedef std::map<UniqueUserID, SpecialCrossLieKunFb::GuildItem> GuildMap;
	GuildMap m_guild_map;							// ����id�Ͱ�����ӳ���

	unsigned int m_check_user_time;					// �´μ����������Ϣ��ʱ�䣬ֱ��Ĭ����1����һ�Σ���Ȼÿ��TICK�������Ƶ��
};

#endif // _SPECIAL_CROSS_LIEKUNFB_HPP_