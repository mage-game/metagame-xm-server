#ifndef __SPECIAL_QUN_XIAN_LUAN_DOU_HPP__
#define __SPECIAL_QUN_XIAN_LUAN_DOU_HPP__

#include <set>

#include "scene/speciallogic/speciallogic.hpp"

#include "protocal/msgactivity.hpp"

class Role;
class BattleFieldShenShi;

class SpecialQunXianLuanDou : public SpecialLogic
{
	friend struct QunXianLuanDouUser;

public:

	// Ĥ����Ϣ
	struct WorshipItem
	{
		WorshipItem() :manual_worship_times(0), next_can_manual_worship_timestamp(0), manual_worship_reward_timestamp(0) {}

		Int32 manual_worship_times;					// Ĥ�ݴ���
		UInt32 next_can_manual_worship_timestamp;   // �´��ܹ�Ĥ�ݵ�ʱ���
		UInt32 manual_worship_reward_timestamp;     // Ĥ�ݸ��轱����ʱ���
	};

	SpecialQunXianLuanDou(Scene *scene);
	virtual ~SpecialQunXianLuanDou();

	virtual bool CanDestroy() { return false; }
	
	virtual void OnRoleEnterScene(Role *role);
	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnAddObj(Obj *obj);
	virtual void OnRemoveObj(Obj *obj);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual void OnCharacterDie(Character *dead, ObjID killer_objid);
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual bool SpecialRelive(Role *role, int realive_type);

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);

	virtual bool SpecialCanMountOn(Role *role){return false;}
	virtual bool SpecialCanPerformSkill(Role *role);

	virtual int GetSpecialParam(Role *role);
	virtual int GetRoleShadowSpecialParam(RoleShadow *roleshadow);

	virtual int GetObjSide(ObjID id);

	void OnActivityStatusChange(int status);

	void OnPickUpBattleFiledShenShi(Role *role, ObjID shenshi_objid);
	QunXianLuanDouUser * GetQunXianLuanDouUser(const UserID &user_id);

	bool IsWorshipDuring();
	void OnWorship(Role* role);
	void SendWorshipInfo(Role *role);
	void SyncWorshipStart(Role *role);
	void CheckWorshipReward(time_t now_second);

	void CheckSendFirstRank();  // ���ȼ�鷢���������е�һ����

protected:
	
	void CheckLeaveQunXianLuanDouUer(time_t now_second);

	void AddSideScore(int side, int score);
	int GetRoleNumLeastSide();
	void CheckUserRank();
	void CheckFirstRank(QunXianLuanDouUser *qxlduser);

	bool IsRoleHasShenshi(Role *role, ObjID *out_shenshi_objid = NULL);
	bool CheckShenShiOwonerStatus(Role *role, bool is_set, ObjID shenshi_objid = INVALID_OBJ_ID);

	void OnBattleFiledShenShiDead(BattleFieldShenShi *dead, ObjID killer_objid);
	void OnBattleFiledShenShiDisappear(BattleFieldShenShi *shenshi);
	void OnRoleDead(Role *dead_role, ObjID killer_objid);

	void OnActivityStandy();
	void OnActivityStart();
	void OnActivityClose();

	void SendRankToUser(Role *role = NULL);
	void GetSelfRankInfo(Role *role, int &self_score, int &self_rank_pos);
	void SendSideInfoToUser(Role *role = NULL);
	void SendQunXianLuanDouUserInfo(QunXianLuanDouUser *qunxianluandou_user, int notify_reason = Protocol::QUNXIANLUANDOU_NOTIFY_REASON_DEFAULT);
	void BroadcastLianzhanInfo(QunXianLuanDouUser *qunxianluandou_user);
	void CheckLianZhanNotice(Role *kill_user, Role *dead_user);
	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);

	int GetResultNotifyReason(int side);
	void Clear();

	bool IsRewardDouble(int side);

	void RealiveHereHelper(Role *role, int realive_type);

	void CheckPersonalScoreReward(Role *role);

	void SendLuckyInfoToUser(Role *role = NULL);
	void GaveLuckyItemToUser();

	void CreateRobot();

	bool m_isopen;
	bool m_has_calc_weak_side_protect;													// �Ƿ��Ѿ�����������Ӫ����

	QunXianLuanDouUserMap m_qunxianluandou_usermap;										// Ⱥ���Ҷ��û��б�

	bool m_need_notify_sideinfo;														// ���㲥��Ӫ��Ϣʱ��
	QunXianLuanDouSideInfo m_sideinfo_list[QUNXIANLUANDOU_SIDE_MAX];					// ��Ӫ��Ϣ�б�

	int m_rankinfo_count;																// ʵʱ���а���Ϣ����
	QunXianLuanDouUserRankItem m_rankinfo_list[QUNXIANLUANDOU_RANK_NUM];				// ʵʱ���а���Ϣ

	time_t m_shenshi_next_flush_timestamp;												// ��ʯ�´�ˢ��ʱ���
	time_t m_last_check_leaveuser_timestamp;											// ����� �뿪�û�ʱ���

	time_t m_last_check_rank_timestamp;													// ���һ�μ�����а�ʱ���
	Protocol::SCQunXianLuanDouRankInfo m_qxldri;										// ���а�Э��

	std::set<ObjID> m_shenshi_objidlist;												// ��ʯobj id�б�

	time_t m_next_free_reward_time;														// �´���ѽ���ʱ��

	unsigned int m_last_first_guild_broadcast_time;
	unsigned int m_last_best_killer_broadcast_time;

	Protocol::SCQunxianluandouLuckyRewardInfo m_luck;

	int m_robot_max_count;
	unsigned int m_robot_create_next_interval;
	unsigned int m_robot_create_interval;
	Posi m_robot_birth_pos;

	QunXianLuanDouUserRankItem m_rank_first_one_side_list[QUNXIANLUANDOU_SIDE_MAX];		// ��Ӫ��һ�б�

	time_t m_kick_out_all_user_time;

	std::map<UniqueUserID, WorshipItem> m_worship_user_map;  // Ĥ�ݽ�ɫ�б�<unique_uid, info>
	time_t m_worship_start_time;							// Ĥ�ݿ�ʼʱ��
};

#endif // __SPECIAL_QUN_XIAN_LUAN_DOU_HPP__


