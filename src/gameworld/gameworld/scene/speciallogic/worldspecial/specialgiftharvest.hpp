#ifndef _SPECIAL_GIFT_HARVEST_HPP_
#define _SPECIAL_GIFT_HARVEST_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include <set>
#include "servercommon/crossdef.hpp"
#include "servercommon/serverdef.h"

class Role;

// �����ո� - ������Ϣ
struct GiftHarvestSkillItem
{
	GiftHarvestSkillItem() : skill_id(0), next_can_perform_time(0) {}

	bool Invalid() const { return 0 == skill_id; }
	void Reset() { skill_id = 0; next_can_perform_time = 0; }

	UInt16 skill_id;
	unsigned int next_can_perform_time;
};

// �����ո� - �����Ϣ
struct GiftHarvestPlayerInfo
{
	GiftHarvestPlayerInfo() : uid(INVALID_USER_ID), cur_get_score(0), cur_get_score_times(0), sex(0), prof(0), avatar_timestamp(0)
	{
		memset(name, 0, sizeof(name));
	}

	UserID uid;
	GameName name;															// ��ɫ��
	int cur_get_score;														// ���� ��õĻ���
	short cur_get_score_times;												// ���� ��ȡ���ֵĴ���
	char sex;																// �Ա�
	char prof;																// ְҵ
	long long avatar_timestamp;												// ͷ��

	GiftHarvestSkillItem skill_list[GIFT_HARVEST_SKILL_CFG_MAX_COUNT];		// ��ɫ�ڵ�ǰ������Ķ��⼼����Ϣ
};

// �����ո� - ������Ϣ
struct GiftHarvestMonster
{
	GiftHarvestMonster() : monster_id(0), total_get_score(0), max_num(0), cur_num(0) {}

	int monster_id;															// ����id
	int total_get_score;													// ���������û���
	int max_num;															// �������
	int cur_num;															// ��ǰ����
};

class SpecialGiftHarvest : public SpecialLogic
{
	static const int PLAYER_FIRST_ENTER_ADD_SCORE = 500;					// ��ҵ�һ�ν������ͻ���

public:
	SpecialGiftHarvest(Scene *scene);
	virtual ~SpecialGiftHarvest();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnAddObj(Obj *obj);
	virtual void OnRemoveObj(Obj *obj);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialCanPerformSkill(Role *role) { return m_activity_open; }
	virtual bool SpecialCanSupplyEffect(Role *role, int supply_type) { return m_activity_open; }
	virtual int SpecialRecalcInjure(Character *attacker, Character *injurer, int injure);
	virtual bool PerformSkill(Role *role, int skill_index, const Posi &pos, ObjID target_id);
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool CanDestroy() { return false; }

	void OnActivityStatusChange(int activity_status, int round_staus);

	void OnActivityOpen();	// �����
	void OnActivityClose();	// �����

	void DrawJoinReward();	// �ɷ����뽱��
	void ReadyRound();		// ׼������
	void StartRound();		// ��ʼ����
	void StopRound();		// ��������

	bool CanEnter();		// �ܷ����

	void SendPlayerInfoToPlayer(Role *role);	// ������Ϣ
	void SendPlayerInfoToPlayer(ObjID obj_id);	// ������Ϣ

	GiftHarvestPlayerInfo * GetPlayerInfo(ObjID obj_id);
	GiftHarvestPlayerInfo * GetPlayerInfo(UserID uid);

protected:

	Role * _GetRole(ObjID obj_id);

	void _FixMonsterNum(int monster_id);					// �ȶ���������
	void _ResetAllData();									// �����������ݣ������ʱ��
	void _ResetRoundData();									// ���ñ������ݣ���ʼ��һ��ʱ��
	void _SyncSkillInfo(Role *role, GiftHarvestSkillItem *skill_info);	// ���ͼ�����Ϣ

	bool m_activity_open;													// �������־
	bool m_cur_round_open;													// �ִο�����־
	unsigned int m_last_check_rank_time;									// ��һ�θ������а��ʱ���
	unsigned int m_last_check_monster_time;									// ��һ���������ʱ���

	typedef std::map<UserID, GiftHarvestPlayerInfo> UserInfoMap;
	typedef std::map<UserID, GiftHarvestPlayerInfo>::iterator UserInfoMapIt;
	UserInfoMap m_user_info_cache;											// �����Ϣ���棬�����������

	typedef std::map<int, GiftHarvestMonster> MonsterInfoMap;
	typedef std::map<int, GiftHarvestMonster>::iterator MonsterInfoMapIt;
	MonsterInfoMap m_monster_info_map;										// ������Ϣ
};


#endif // _SPECIAL_GIFT_HARVEST_HPP_

