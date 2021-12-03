#ifndef _SPECIAL_CROSS_PASTURE_HPP_
#define _SPECIAL_CROSS_PASTURE_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include <set>
#include "servercommon/crossdef.hpp"
#include "servercommon/serverdef.h"

class Role;

struct CrossPasturePlayerInfo
{
	struct PastureSkillItem 
	{
		PastureSkillItem () : skill_id(0), last_perform_time(0) {}

		bool Invalid() const { return 0 == skill_id; }
		void Reset() { skill_id = 0; last_perform_time = 0; }

		UInt16 skill_id;
		unsigned int last_perform_time;
	};

	CrossPasturePlayerInfo() : uuid(INVALID_UNIQUE_USER_ID), score(0), cur_cross_get_score(0), get_score_times(0), cur_cross_get_score_times(0), last_relive_time(0)
	{
		memset(name, 0, sizeof(name));
	}

	UniqueUserID uuid;
	GameName name;															// ��ɫ��
	int score;																// ��ǰ�ܻ���
	int cur_cross_get_score;												// ��ǰ�ο����û���
	int get_score_times;													// ��ȡ���ֵ��ܴ���
	int cur_cross_get_score_times;											// ��ǰ�ο����ȡ���ֵĴ���
	unsigned int last_relive_time;											// ��һ�θ����ʱ���

	PastureSkillItem skill_list[CROSS_PASTURE_SKILL_CFG_MAX_COUNT];			// ��ɫ�ڵ�ǰ������Ķ��⼼����Ϣ
};

struct CrossPastureMonster
{
	CrossPastureMonster() : monster_id(0), score(0), max_num(0), cur_num(0), next_refresh_time(0) {}

	int monster_id;															// ����id
	int score;																// ����ũ����û���
	int max_num;															// �������
	int cur_num;															// ��ǰ����
	unsigned int next_refresh_time;											// ��ID�Ĺ����´�ˢ��ʱ��
};
struct CrossPastureRankItem													// Ⱥ���Ҷ� ���а���Ϣ
{
	CrossPastureRankItem() : uuid(INVALID_UNIQUE_USER_ID), score(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UniqueUserID uuid;
	GameName user_name;																// ��ɫ��
	int score;																		// �ܻ���
};

class SpecialCrossPasture : public SpecialLogic
{
	static const int PLAYER_FIRST_ENTER_ADD_SCORE = 0;					// ��ҵ�һ�ν������ͻ���

public:
	SpecialCrossPasture(Scene *scene);
	virtual ~SpecialCrossPasture();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnAddObj(Obj *obj);
	virtual void OnRemoveObj(Obj *obj);

	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialCanPerformSkill(Role *role) { return m_activity_open; }
	virtual bool SpecialCanSupplyEffect(Role *role, int supply_type) { return m_activity_open; }
	virtual long long SpecialRecalcInjure(Character *attacker, Character *injurer, long long injure);
	virtual bool PerformSkill(Role *role, int skill_index, const Posi &pos, ObjID target_id);

	virtual int GetSpecialParam(Role *role);

	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool CanDestroy() { return false; }
	virtual bool SpecialIsCostMp(Role *role) { return false; }

	void OnActivityOpen();
	void OnActivityClose();

	void ClearRankTitle();

	void RankItemReward();

protected:
	void SendPlayerInfoToPlayer(Role *role);
	void SendPlayerInfoToPlayer(ObjID obj_id);

	void NotifySystemMsgToScene(char *str_buff, int str_len, int notice_type);
	void SyncPlayerInfoToCross(Role *role);

	Role * GetRole(ObjID obj_id);
	CrossPasturePlayerInfo * GetPlayerInfo(UniqueUserID uuid);
	CrossPasturePlayerInfo * GetPlayerInfo(ObjID obj_id);
	void FixMonsterNum(int monster_id);
	void FixSPMonsterNum(int monster_id, time_t now_second);

	void CheckPastureUserRank();
	void SendPastureRankToUser(Role*role=NULL);
	bool m_activity_open;

	time_t m_activity_end_time;												// �����ʱ�䣨��ֹcrossû֪ͨ������

	typedef std::map<UniqueUserID, CrossPasturePlayerInfo> UserInfoMap;
	typedef std::map<UniqueUserID, CrossPasturePlayerInfo>::iterator UserInfoMapIt;
	UserInfoMap m_user_info_cache;											// �����Ϣ���棬�����������

	std::map<int, CrossPastureMonster> m_monster_info_map;

	unsigned int m_last_check_animal_status_time;

	std::map<UniqueUserID, unsigned int> m_role_last_die_time_map;			// ��¼����ϴ�����ʱ�䣬N��������Ѫ����

	time_t m_pasture_last_rank_timestamp;													// ���һ�μ�����а�ʱ���
	std::vector<CrossPastureRankItem> m_pasture_rank_vec;
	time_t m_pasture_next_refresh_specialmonster_timestamp;									// ��һ��ˢ��speaial monster��ʱ���

	int special_monster = 0;
	Posi special_monster_pos;
};


#endif // _SPECIAL_CROSS_1V1_HPP_

