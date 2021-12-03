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

		short gather_num;			// �Ѳɼ�����
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
	void ResetGuildBoss();						// ����boss�������

	void CheckUpdateStarLevel(unsigned int now_second);

	void CalcRoleHurtBossVal(Role *role, Attribute delta_hp);

	void CheckNoticeBossTimeLimit(time_t now_second);			// �����ʾ�ʣ��ʱ��

	void SendGuildBossInfo(int notify_type = 0, Role *role = NULL);

	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);

	//���ͭ��----------------------------------------------------------------
	bool IsTianCiTongBiOpen() {  return m_tianci_tongbi_is_open; }
	void OnRoleGatherBigCoin(Role *role, GatherObj *bigcoin);			// �ɼ�
	void BroadcastGatherInfo(Role *role, int gather_type);				// �㲥�ɼ�
	void OnOpenTianCiTongBi();											// �����
	void ForceTianCiTongBiNextState();									// ǿ�ƽ�����һ״̬
	void OnTianCiTongBiUseGather(Role *role, bool is_auto = false);		// �ύ�ɼ���
	void SendTianCiTongBiRankInfo(Role *role = nullptr);				// ����������Ϣ

	void AddTreeMaturity(int maturity);									// ���ӳ����

private:
	void GuildBossReward();

	void SortHurtRank();		// �˺�����
	void GiveRankReward();		// �������н���

	void CreateBossGather();	// ����boss�ɼ���
	void TotemBless();

	//���ͭ��----------------------------------------------------------------
	Role * GetRole(GuildTianCiTongBiUser &dps_user);						  // ��ȡrole
	GuildTianCiTongBiUser * GetGuildTianCiTongBiUser(const UserID user_id);	  // ��ȡͭ��user

	void OnTianCiTongBiOpen();												  // ���ͭ�ҿ���
	void OnTianCiTongBiClose();												  // ���ͭ�ҹر�
	void TianCiTongBiCheckGather();											  // ���ɼ���ˢ�£���ʱ���ã�
	void TianCiTongBiRefreshGather();										  // ˢ�²ɼ���
	void TianCiTongBiRefreshBigCoin();										  // ˢ��ͭ��
	void TianCiTongBiRoleGatherChange(Role *role, int gather_type);			  // �ɼ�״̬�ı�
	void RefreshTianCiTongBiRank(Role *role = nullptr);						  // ˢ������
	void ClearTianCiTongBiGahter();											  // ����ɼ���
	void ClearTianCiTongBiBigCoin();										  // ���ͭ��
	void SendGuildTianCiTongBiNpcInfo(Role *role = nullptr);
	void SendTianCiTongBiRoleGatherInfo(int flag = 0);

	int m_call_boss_uid;
	int m_boss_level;
	int m_boss_id;
	ObjID m_boss_objid;
	bool m_is_super_boss;
	bool m_is_notice_boss_timelimit;							// �Ƿ�����ʾboss�ʣ��ʱ��
	unsigned int m_boss_dayid;

	time_t m_next_bless_time;
	unsigned int m_next_update_rank_time;						// �´�ˢ���а�ʱ���

	unsigned int m_next_change_star_time;						// �´θı��Ǽ�ʱ���
	int m_cur_star_level;										// ��ǰ�Ǽ�

	std::map<int, unsigned int> m_boss_hurt_uid_list;
	std::set<ObjID> m_gathar_objid_list;						// boss�ɼ�����

	std::vector<HurtBossInfo> m_hurt_boss_info_vec;		// ��boss�˺�������Ϣ

	typedef std::map<int, RoleFbInfo> PlayerInfoMap;
	typedef std::map<int, RoleFbInfo>::iterator PlayerInfoMapIt;
	PlayerInfoMap m_role_info_map;								// ��Ҹ�����Ϣ

	//���ͭ��----------------------------------------------------------------
	//GuildBossDPSUserMap m_guildboss_dps_map;

	bool m_tianci_tongbi_is_open;																			// ���ͭ���Ƿ���
	unsigned int m_tianci_tongbi_open_time;																	// ���ͭ�ҿ�ʼʱ��
	unsigned int m_tianci_tongbi_close_time;																// ���ͭ�ҽ���ʱ��
	int m_tianci_tongbi_tree_maturity_degree;																// ���ͭ���������
	int m_tianci_tongbi_readiness_time;																		// ���ʼ׼��ʱ��
	Posi m_tianci_tongbi_npc_pos;																			// ���ͭ��npc����

	GuildTianCiTongBiUserMap m_guild_tianci_tongbi_user_map;												// ���ͭ���û��б�

	//unsigned int m_next_refresh_tianci_tongbi_rank_time;
	GuildTianCiTongBiRankItemVec m_guild_tianci_tongbi_rank_vec;

	std::set<ObjID> m_tianci_tongbi_big_coin_objid_set;														// ���ͭ�Ҵ����б�

	struct TianCiTongBiGatherInfo
	{
		TianCiTongBiGatherInfo() : obj_id(INVALID_OBJ_ID), next_refresh_time(0) {}

		ObjID obj_id;
		unsigned int next_refresh_time;
	};
	TianCiTongBiGatherInfo m_tianci_tongbi_gather_list[GUILD_TIANCI_TONGBI_GAHTER_NUM_MAX];
	int m_gather_num;																						// �Ѳɼ�����

	unsigned int m_next_tianci_tongbi_check_gather_time;
	std::map<int, int> m_guild_xianci_tongbi_gather_type_map;												// uid - gather_type ���ͭ�� �ɼ�������(������)
};

#endif // __GUILD_STATION_HPP__

