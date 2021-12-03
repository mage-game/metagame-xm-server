#ifndef _BOSSSHANGGUMANAGER_HPP_
#define _BOSSSHANGGUMANAGER_HPP_

#include <vector>
#include <map>
#include <deque>

#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "shanggubossconfig.hpp"
#include "protocal/msgfb.h"
#include "obj/character/role.h"

class Role;
class Monster;

namespace Protocol
{
	struct SCWorldBossItemInfo;
}

class BossShangGuManager
{
public:

	enum ENBOSSSTATUS
	{
		ENBOSSSTATUS_NONEXIST = 0,										// ������
		ENBOSSSTATUS_EXIST,												// ����
	};

	enum SHANGGU_BOSS_KIND_TYPE
	{
		SHANGGU_BOSS_KIND_TYPE_MAX_BOX = 0,	// ����
		SHANGGU_BOSS_KIND_TYPE_MIN_BOX,		// С����
		SHANGGU_BOSS_KIND_TYPE_GOLD_MONSTER,// �ƽ��
		SHANGGU_BOSS_KIND_TYPE_HIDE_MONSTER,// ����boss
	};

	struct BossInfo
	{
		struct KillBossHistory
		{
			int uid;
			unsigned int kill_boss_time;
		};

		BossInfo() : index(0), boss_id(0), scene_id(0), obj_id(INVALID_OBJ_ID), has_notice(false), next_refresh_time(0)
		{
		}

		int index;
		int boss_id;
		int scene_id;
		ObjID obj_id;					// ��boss�����ڣ���ɱ���ˣ���Ϊ INVALID_OBJ_ID

		bool has_notice;
		unsigned int next_refresh_time;	// �´�ˢ��ʱ�䣬0���ʾ��û�б�����
		std::set<int> register_role_set;	// ע���˼�����BOSS�����ID�б�
		std::deque<KillBossHistory> kill_boss_history_list;
	};

	struct ShangGuHideBossInfo
	{
		static const int TOTAL_WEIGHT = 100;

		ShangGuHideBossInfo() : boss_lv_seq(0), average_weight(0), temp_weight(TOTAL_WEIGHT), is_has_flush(false) {}

		int boss_lv_seq;
		int average_weight;
		int temp_weight;
		bool is_has_flush;
	};

	struct ShangGuObjIdInfo
	{
		ShangGuObjIdInfo() : layer(-1), obj_id(INVALID_OBJ_ID), kind_type(-1) {}

		int layer;
		ObjID obj_id;
		short kind_type;
	};

	struct LevelBossInfo
	{
		LevelBossInfo() : level(0), scene_id(0) {}
		int level;
		int scene_id;
		std::map<int, BossInfo> boss_map;
	};


	static BossShangGuManager &Instance();

	void OnServerStart();
	void OnStopServer();

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnAttack(Role *attacker, Monster *monster, long long &delta_hp);

	void UpdateBossMap(time_t now_second, std::map<int, BossInfo> & boss_map, int level);    // boss����ˢ��
	void OnShangGuBossReq(Role *role, short opera_type, short param1, short param2 = 0, short param3 = 0);
	void OnShangGuBossDie(int scene_id, unsigned short boss_id, Role *killer_role);
	void OnShangGuBossEraseMap(int layer, ObjID obj_id);
	void OnBossDie(unsigned short boss_id, Role *killer_role, int flush_interval_s, std::map<int, BossInfo> & boss_map, int level);
	void RegisterConcernBoss(int uid, int param1, int param2);              // ��עboss
	void UnRegisterConcernBoss(int uid, int param1, int param2);            // ȡ����ע
	void SendShangGuKillerInfo(Role *role, int boss_id);
private:
	BossShangGuManager();
	~BossShangGuManager();

	void SendBossInfoToAll(BossInfo &boss_info, int notify_reason, Role *killer_role);         // boss����ˢ��֪ͨ�������
	void SendFollowBossList(Role *role);                                    // ��ע�б�
	void SendShangGuBossAllInfo(Role *role);
	void SendShangGuBossLayerInfo(Role *role, int layer);                   // ������Ϣ
	void SendShangGuBossSceneOtherInfo(Scene *scene, Role *role);           // ������������Ϣ

	void OnReqEnterShangGuBoss(Role *role, int level, int boss_id);
	void CreateShangGuBossAllBoss();
	void CreateShangGuBossGatherItemAndGoldMonster();
	void ClearShangGuBossAllItem();
	bool GetShangGuBossGatherItemRandomPosition(Scene *scene, Posi *pos);
	void UpdataShangGuBossGatherItemAndGoldMonster(time_t now_second);
	void OnFlushShangGuHideBoss(int boss_lv_seq, int level, Role *killer_role, int scene_id, int boss_id);   // �Ϲ�boss����boss
	void GetLayerBossMsg(short &boss_count, Protocol::SCWorldBossItemInfo *boss_info_list, int max_count, const std::map<int, BossInfo> & boss_map, Role *role, int level);

	std::vector<LevelBossInfo> m_shanggu_boss_level_list;
	std::map<long long, ShangGuObjIdInfo> m_shanggu_boss_obj_id_info_map;
	std::map<int, ShangGuHideBossInfo> m_shanggu_hide_info_map;
	unsigned int m_shanggu_boss_next_flush_timestamp;
};

#endif
