#ifndef _SPECIAL_CROSS_TUANZHAN_HPP_
#define _SPECIAL_CROSS_TUANZHAN_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include <set>
#include "servercommon/crossdef.hpp"
#include "servercommon/serverdef.h"

class Role;

struct CrossTuanzhanPillaInfo
{
	CrossTuanzhanPillaInfo() { this->Reset(); }

	void Reset()
	{
		index = 0;
		monster_id = 0;
		obj_id = INVALID_OBJ_ID;
		owner_side = CROSS_TUANZHAN_SIDE_MAX;
		memset(owner_name, 0, sizeof(owner_name));
		owner_unique_user_id = INVALID_UNIQUE_USER_ID;
	}

	short index;
	unsigned short monster_id;												// ���ӹ���id
	ObjID obj_id;															// ���ӵĶ���id
	Posi position;															// ��������
	short owner_side;														// ռ�����ӵ���Ӫ
	GameName owner_name;													// ռ�����������
	UniqueUserID owner_unique_user_id;										// ռ�����ӵ����Ψһid
};

struct CrossTuanzhanSideInfo
{
	CrossTuanzhanSideInfo() { this->Reset(); }

	void Reset()
	{
		winner_side = CROSS_TUANZHAN_SIDE_MAX;
		memset(side_player_num_list, 0, sizeof(side_player_num_list));
		memset(side_score_list, 0, sizeof(side_score_list));
	}

	void SidePlayerNumChange(int side, int change_value);
	void SideScoreChange(int side, int change_value);
	void RecalWinnerSide();

	int GetMinPlayerNumSide() const;

	int winner_side;
	int side_player_num_list[CROSS_TUANZHAN_SIDE_MAX];
	unsigned int side_score_list[CROSS_TUANZHAN_SIDE_MAX];
};

struct CrossTuanzhanPlayerInfo
{
	CrossTuanzhanPlayerInfo() : uuid(INVALID_UNIQUE_USER_ID), camp(CAMP_TYPE_INVALID), is_fetch_rank_reward(0), score_reward_fetch_seq(0), role_level(0),
		hp_on_leave_scene(0), obj_id(INVALID_OBJ_ID), side(CROSS_TUANZHAN_SIDE_MAX), score(0), kill_num(0), assist_kill_num(0),
		dur_kill_num(0), been_kill_num(0), dur_been_kill_num(0)
	{
		memset(name, 0, sizeof(name));
	}

	friend bool operator<(const CrossTuanzhanPlayerInfo &player_info1, const CrossTuanzhanPlayerInfo &player_info2);
	friend bool operator>(const CrossTuanzhanPlayerInfo &player_info1, const CrossTuanzhanPlayerInfo &player_info2);

	UniqueUserID uuid;
	GameName name;															// ��ɫ��
	char camp;																// ��ɫ��Ӫ
	char is_fetch_rank_reward;
	short score_reward_fetch_seq;
	int role_level;
	long long hp_on_leave_scene;											// �ϴ��˳�����ʣ��Ѫ��
	ObjID obj_id;															// ����ID
	short side;																// ��ս��������Ӫ
	unsigned int score;														// ����
	unsigned int kill_num;													// ��ɱ����
	unsigned int assist_kill_num;											// ��������
	unsigned int dur_kill_num;												// ��ɱ����
	unsigned int been_kill_num;												// ������
	unsigned int dur_been_kill_num;											// ������ɱ����

	typedef std::map<UniqueUserID, time_t> AttackerTimeMap;
	typedef std::map<UniqueUserID, time_t>::iterator AttackerTimeMapIt;
	AttackerTimeMap attack_user_map;										// ������ɫ������б�
};

struct CrossTuanzhanRankPlayerInfo
{
	CrossTuanzhanRankPlayerInfo() { this->Reset(); }

	void Reset()
	{
		uuid = INVALID_UNIQUE_USER_ID;
		side = CROSS_TUANZHAN_SIDE_MAX;
		role_level = 0;
		score = 0;
		memset(name, 0, sizeof(name));
	}

	UniqueUserID uuid;
	short side;
	short role_level;
	unsigned int score;
	GameName name;
	short kill;
	short assist;
};

class SpecialCrossTuanzhan : public SpecialLogic
{
public:
	SpecialCrossTuanzhan(Scene *scene);
	virtual ~SpecialCrossTuanzhan();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual void OnCharacterDie(Character *dead_cha, ObjID killer_objid);

	virtual void OnAddObj(Obj *obj);
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialCanPerformSkill(Role *role);
	virtual bool SpecialCanSupplyEffect(Role *role, int supply_type);

	virtual int GetSpecialParam(Role *role);

	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool CanDestroy() { return false; }
	virtual bool SpecialIsCostMp(Role *role) { return false; }

	void OnFetchScoreReward(Role *role);

	void OnActivityOpen();
	void OnActivityClose();

protected:
	void OnRecalcUserSide();
	void AddStaticScore();
	void AddPillaScore();
	void RecalDeadRewardBuff(Role *role, int hp_percent = 0);
	void UpdateUserRank(const CrossTuanzhanPlayerInfo *player_info);

	void SendActivityStateToPlayer(Role *role = NULL);
	void SendPlayerInfoToPlayer(Role *role, bool is_broacast = false);

	void SendAllPillaInfoToPlayer(Role *role);
	void SendPillaInfoToToAllPlayer(const CrossTuanzhanPillaInfo *info);

	void SendSideInfoToPlayer(Role *role = NULL);
	void SendRankListToPlayer(Role *role = NULL);
	
	void BroadcastDurKillInfo(ObjID obj_id, unsigned int dur_kill_num);

	void NotifySystemMsgToScene(char *str_buff, int str_len, int notice_type);
	void SendResultToAllPlayer();
	void SyncResultToCross();
	void NotifySystemMsg(char * str_buff, int str_len);

	Role * GetRole(ObjID obj_id);
	CrossTuanzhanPlayerInfo * GetPlayerInfo(UniqueUserID uuid);
	CrossTuanzhanPlayerInfo * GetPlayerInfo(ObjID obj_id);
	CrossTuanzhanPillaInfo * GetPillaInfo(ObjID obj_id);

	bool m_activity_open;
	bool m_fighting;

	time_t m_next_add_static_score_time;									// ��һ�μӹ̶�������ֵ�ʱ��
	time_t m_next_add_pilla_score_time;										// ��һ�μ�ͨ��������ʱ��
	time_t m_next_send_rank_data_time;										// ��һ�ι㲥���а���Ϣʱ��
	time_t m_next_rand_user_side_time;										// ��һ�������Ӫ��ʱ��
	time_t m_last_broacast_time;											// �ϴμ�¼�㲥��ʱ��

	time_t m_fight_start_time;												// ս����ʼʱ��
	time_t m_activity_end_time;												// �����ʱ�䣨��ֹcrossû֪ͨ������

	typedef std::map<UniqueUserID, CrossTuanzhanPlayerInfo> UserInfoMap;
	typedef std::map<UniqueUserID, CrossTuanzhanPlayerInfo>::iterator UserInfoMapIt;
	UserInfoMap m_user_info_cache;											// �����Ϣ���棬�����������

	CrossTuanzhanSideInfo m_side_info;										// ��Ӫ��Ϣ����m_user_info_cache��ã��������ݣ�

	CrossTuanzhanPillaInfo m_pilla_list[CROSS_TUANZHAN_PILLA_MAX_COUNT];	// �����б�

	int m_player_rank_list_count;
	CrossTuanzhanRankPlayerInfo m_player_rank_list[CROSS_TUANZHAN_RANK_LIST_SIZE];	// ά��һ������������а�������Ҷ�ʱˢ����ʾ
};


#endif // _SPECIAL_CROSS_1V1_HPP_

