#ifndef __CROSS_CHALLENGE_FIELD_HPP__
#define __CROSS_CHALLENGE_FIELD_HPP__

/************************************************************************
* ���������
* ���ݴ����ط���ȡ
* ���ݱ��ͬ�������ط�
* 
* modify 2019-04-18
************************************************************************/

#include <map>
#include "servercommon/struct/global/challengeuserparam.hpp"
#include "protocal/msgactivity.hpp"

class GlobalUser;
class CrossChallengeUser;

class ChallengeUserListParam;

enum CROSS_CHALLENGE_FIELD_DATA_STATE
{	
	CROSS_CHALLENGE_FIELD_DATA_STATE_INVALID = 0,						// ��ʼ״̬
	CROSS_CHALLENGE_FIELD_DATA_STATE_LOADING,							// ���ݼ����� 
	CROSS_CHALLENGE_FIELD_DATA_STATE_LOAD_FINISH,						// ���ݼ������
	CROSS_CHALLENGE_FIELD_DATA_STATE_MAX,
};


class CrossChallengeField
{
public:
	static CrossChallengeField & Instance();

	bool OnServerStart();
	void OnServerStop();

	void Init();
	void GetInitParam(ChallengeFieldDataParam *data_param);

	bool IsLoadFinish() const { return (CROSS_CHALLENGE_FIELD_DATA_STATE_LOAD_FINISH == m_data_state); }

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void SetDirty(int rankpos);						// �����仯

	bool IsChallengeOpen();							// �������Ƿ���

	CrossChallengeUser * GetChallengeUser(const UniqueUserID &user_id, int *rankpos = NULL);					// ��ȡ�����������Ϣ
	CrossChallengeUser * GetChallengeUserByPos(int pos);														// ��ȡ�����������Ϣ
	void ConstructChallengeParam(Role *role,ChallengeParam * param, RoleAppearance &appearance);				// ��������������Ϣ

	void OnUserLevelChange(Role *role);
	void OnUserLogin(Role *role);
	void OnUserLogout(Role *role);

	void OnSyncChallengeParam(Role *role);															// ͬ�������������Ϣ
	void OnChallengeResult(UniqueUserID sponsor_userid, UniqueUserID opponent_userid, bool is_sponsor_win);		// ��ս���
	void OnGetChallengeUserInfo(Role *role);														// �·��Լ���������Ϣ

	void OnResetOpponentReq(Role *role);																	// ˢ�¶�������
	void OnFightReq(Role *role, int opponent_idx, bool ignore_rank_pos, int rank_pos, bool buy_join_times); // ս������
	void OnBuyJoinTimes(Role *role);																		// ������ս����
	void OnFetchJifenReq(Role *role, int seq);																// ��ȡ��������
	void OnGetRankInfo(Role *role);																			// ��ȡǰ100����Ϣ
	void OnGetLuckyInfo(Role *role);																		// ������
	void OnFetchGuangHui(Role *role);																		// ��ȡ����
	void OnGetOpponentDetailInfo(Role *role, bool is_get_top);												// ��ȡ������Ϣ
	void OnChallengeFieldBuyBuff(Role *role);																// ����buff����
	void RandOpponentHelper(UniqueUserID user_id);															// ���þ���������

	int GetChallengeUserRankPos(Role *role);																// ��ȡ�������
	void GMSetEmemySelf(Role *role, int percent);															// ���õ���

	void OnSyncInitCrossChallengefield(crossgameprotocal::SyncCrossChallengefieldDataInfo *sccbi);			// �ӿ��ͬ����ʼ������
	void OnSyncUpdateCrossChallengefield(crossgameprotocal::SyncCrossChallengefieldDataInfo *sccbi);		// �ӿ��ͬ����������

private: 
	CrossChallengeField();
	~CrossChallengeField();

	bool LoadChallengeUser();																		// ��ʼ���뾺�����������
	void LoadChallengeUserSucc();																	// ���ݼ������

	bool InitChallengeUser(const ChallengeUserListParam::ChallengeUserAttr &user_attr);				// ��ʼ���������������

	void CheckOnlineCreateAllCrossChallengeUser();													// �������о�������ɫ��Ϣ
	CrossChallengeUser * CheckCreateCrossChallengeUser(Role *role);									// ��鴴����������ɫ��Ϣ

	void ResizeUserList(int size);
	void GetChangeList(ChallengeUserListParam *p);													// ��ȡ�ı��б�����
	void ClearDirtyMark(int rank_begin, int rank_end);
	
	void CheckSave(time_t now_second);																// ��ʱ���������ݵ����ط�
	void AddDirtyDeque(long long unique_uid);														// ��������ݱ��
	bool CheckIsAddDirtyDeque(long long unique_uid);												// ��������ݱ��
	void DeleteDirtyDeque(long long unique_uid);													// ɾ�������ݱ��

	void OnInsertCrossChallengefieldUser(Role *role);												// ������������ݵ����ط�(������)
	void OnChallengeResultToHidden(const ChallengeUserListParam::ChallengeUserAttr &user_attr, const ChallengeUserListParam::ChallengeUserAttr &target_user_attr);

	int GetFreeRankPosSlot();																		// ��ȡ��ȱ����
	
	CrossChallengeUser * CreateChallengeUser(const UniqueUserID &user_id, const ChallengeParam &challenge_p, const RoleAppearance &challenge_appearance);
	void SetChallengeUser(int rankpos, CrossChallengeUser *challenge_user, bool is_init = false, bool need_reset_opponen = false);
	bool GenrateChallengeUserParamInfo(ChallengeParam &param, RoleAppearance &appearance, int rank_index, int robot_num);  // ���������������ɻ�������Ϣ

	void OnRankPosChange(UniqueUserID user_id, int rank_pos);										// �����仯
	
	void CheckGiveLuckyReward(time_t now_second);
	void CheckGiveRankReward(time_t now_second);
	void GenrateLuckyRankPos();

	int m_data_state;

	bool m_is_first;

	int m_had_init_count = 0;						// ��ʼ������
	
	int m_rankuser_num;
	CrossChallengeUser **m_rankuser_list;			// rankpos - CrossChallengeUser* �б�
	bool *m_dirty_mark;
	bool *m_old_state;
	bool m_is_change;

	time_t m_last_save_time;
	
	typedef std::map<UniqueUserID, int> RankPosMap;
	typedef std::map<UniqueUserID, int>::iterator RankPosMapIt;
	RankPosMap m_rankpos_map;						// uuid - rankpos�б�

	ChallengeFieldDataParam m_challenge_field_data;
	time_t m_last_check_give_lucky_reward_timestamp;
	time_t m_last_check_give_rank_reward_timestamp;

	Protocol::SCCrossChallengeFieldRankInfo m_msg_rank_info;

	// ��¼��ͬ���������������
	std::deque<long long> m_dirty_deque;
	std::set<long long> m_dirty_set;
};

#endif // __CHALLENGE_FIELD_HPP__

