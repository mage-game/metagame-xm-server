#ifndef _CAMPDEFEND_MGR_HPP_
#define _CAMPDEFEND_MGR_HPP_

#include "servercommon/servercommon.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include <map>
#include "obj/character/monster.h"
#include "obj/character/roleshadow.hpp"
#include "config/activityconfig/jinghuahusongconfig.hpp"
#include "other/queryhandler/queryhandler.hpp"

class Role;

class CampDefendMgr
{
public:
	static CampDefendMgr & Instance();

	struct Joiner
	{
		Joiner() : roleid(0), camp(CAMP_TYPE_INVALID), total_diaoxiang_hurt(0) { memset(name, 0, sizeof(GameName)); }
		
		int GetLastKillSelfTime(int killer_id)
		{
			std::map<int, unsigned int>::iterator iter = m_last_killself_time_dic.find(killer_id);
			if (m_last_killself_time_dic.end() == iter)
			{
				return 0;
			}

			return iter->second;
		}

		void AddKillSelfRecord(int killer_id, unsigned int kill_time)
		{
			m_last_killself_time_dic[killer_id] = kill_time;
		}

		int roleid;
		int camp;
		long long total_diaoxiang_hurt;
		GameName name;

		std::map<int, unsigned int> m_last_killself_time_dic;  // key 为roleid. value为时间
	};

	struct DiaoXiang
	{
		DiaoXiang() { this->Reset(); }

		int roleid;
		int objid;
		int sceneid;

		void Reset()
		{
			roleid = 0;
			objid = INVALID_OBJ_ID;
			sceneid = 0;
		}
	};

	void OnWorldStatusFirstStart();

	void Update(unsigned long interval, time_t now_second);

	void OnEnterScene(Role *role);
	void OnRoleChangeCamp(Role *role);
	void OnActivityStatusChange(int status, int activity_type);
	
	void UpdateDiaoXiang();
	void DelAllDiaoXiang();
	void CreateAllDiaoXiang();

	void UpdateOneDiaoXiang(int index);
	void DelOneDiaoXiang(int index);
	void CreateOneDiaoXiang(int index);

	void QueryCallback(OfflineQueryCacheNode *cache_node);
	
	void OnRoleDie(Role *role, Role *killer);
	void OnDiaoXiangDie(Role *role, RoleShadow *diaoxiang);
	void OnDiaoXiangAttack(Role *role, Attribute hurt);

	Scene *GetActingScene();
	int GetActingType() { return m_activity_type; }
	int GetActingShouFangCamp();
	RoleShadow *GetDiaoXiangByIndex(int index);

	int GetDiaoXiangIndex(RoleShadow *diaoxiang);
	int GetDiaoXiangStatus(RoleShadow *diaoxiang);

	void AddJoiner(Role *role);
	CampDefendMgr::Joiner *GetJoiner(int roleid);

	bool IsDiaoxiangCanBeAttacked(RoleShadow *diaoxiang, Role *role);
	bool IsInCampDefendingScene(Character *cha);
	bool IsCampDefending();
	bool IsCampDefendStandBy();
	bool IsCampDefendActType(int activity_type);

	void SyncCampDefendInfo();

private:
	CampDefendMgr();
	~CampDefendMgr();

	void CalcShoufangReward(RoleShadow *diaoxiang);
	void CalcGongfangReward(RoleShadow *diaoxiang);
	void CalcRewardOnActClose();

	int m_act_status;
	int m_activity_type;
	unsigned int m_next_syncinfo_time;
	DiaoXiang m_diaoxiang_list[MAX_CAMP_RECORD_RANK3_NUM];

	typedef std::map<int, CampDefendMgr::Joiner> JoinerDic;
	JoinerDic m_joiner_dic;
};

#endif




