#ifndef __SPECIAL_CROSS_BIANJINGZHIDI_HPP__
#define __SPECIAL_CROSS_BIANJINGZHIDI_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include <vector>
#include <set>

// K-���-�߾�֮��

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

			UniqueUserID guild_id;							// ����id
			long long hurt;									// dps
			time_t logout_timestamp;						// ���ɳ�Աȫ�����ߵ�ʱ����±��
			int rank;

			std::map<long long, long long> uuid_map;		// ���ɳ�Աdps�� param1��Աuuid param2dps
		};

		ObjID boss_obj;										// boss��objid
		int boss_id;										// bossid
		Posi born_pos;

		bool boss_live_flag;								// boss�����
		unsigned int boss_next_flush_timestamp;				// boss�´�ˢ��ʱ���

		GuildID guild_id;									// boss���չ�������id
		UniqueUserID guild_uuid;							// boss���չ�������uuid
		GuildName guild_name;								// boss���չ���������
		std::map<UniqueUserID, GuildHurt> guild_hurt_map;	// ����dps��

		ObjID gather_obj;									// boss������ɼ������objId
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
	void CheckUserTaskInfo(Role *role, short task_type, int param_id, const std::set<UniqueUserID> &uuid_set);  // uuid_set������ң������������BOSS���߽�ɫ���й��˺�
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
	void CheckAllBossBound();								// ���ȫ��boss���������ɳ�ԱԶ��boss�������ɳ�Ա��boss��dps����ȥ����
	void CheckSingleBossBound(int boss_type);				// ��鵥��boss���������ɳ�ԱԶ��boss�������ɳ�Ա��boss��dps����ȥ����

	bool m_is_finish;

	time_t m_next_reflush_boss_time;
	time_t m_next_reflush_monster_time;
	time_t m_reflush_notice_time;									// �߾�BOSSˢ����ʾʱ��
	bool m_is_notice;
	
	std::map<long long, time_t> m_role_task_time_map;				// ����ʱ��

	unsigned m_send_boss_info_time;

	int m_monster_die_num;
	unsigned int m_next_check_buff_time;
	CrossBianJingZhiDiUserMap m_bianjingzhidi_usermap;				// ����߾�֮���û��б�

	BossItem m_boss_list[CROSS_BIANJINGZHIDI_MAX_BOSS_TYPE];		// ������boss��Ϣ

	typedef std::map<UniqueUserID, SpecialCrossBianJingZhiDi::GuildItem> GuildMap;
	GuildMap m_guild_map;											// ����id�Ͱ�����ӳ���
	
};

#endif // __SPECIAL_CROSS_BIANJINGZHIDI_HPP__
