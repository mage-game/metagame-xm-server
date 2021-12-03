#ifndef __CHALLENGE_FIELD_HPP__
#define __CHALLENGE_FIELD_HPP__

#include <map>
#include "servercommon/struct/global/challengeuserparam.hpp"
#include "protocal/msgactivity.hpp"

class GlobalUser;
class ChallengeUser;

class ChallengeUserListParam;

enum CHALLENGE_FIELD_DATA_STATE
{	
	CHALLENGE_FIELD_DATA_STATE_INVALID = 0,						// ��ʼ״̬
	CHALLENGE_FIELD_DATA_STATE_LOADING,							// ���ݼ����� 
	CHALLENGE_FIELD_DATA_STATE_LOAD_FINISH,						// ���ݼ������
	CHALLENGE_FIELD_DATA_STATE_MAX,
};

enum RANDOM_CAPABILITY											// ƥ��ս�����Ʊ���
{
	CAPABILITY_MULTIPE_1 = 2,								
	CAPABILITY_MULTIPE_2 = 3,
	CAPABILITY_MULTIPE_3 = 4,
	CAPABILITY_MULTIPE_4 = 5,

	CAPABILITY_NUM = CAPABILITY_MULTIPE_4 - CAPABILITY_MULTIPE_1 + 1,
};


class ChallengeField
{
	friend class RMIInitChallengeUserObjectImpl;

public:
	static ChallengeField & Instance();

	bool OnServerStart();
	void OnServerStop();

	void Init(const ChallengeFieldDataParam &data_param);
	void GetInitParam(ChallengeFieldDataParam *data_param);

	bool IsLoadFinish() const { return (CHALLENGE_FIELD_DATA_STATE_LOAD_FINISH == m_data_state); }

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void SetDirty(int rankpos);						// �����仯

	bool IsChallengeOpen();							// �������Ƿ���

	int GetEndRankRewardOpenday();					// ��ȡ����������ʱ�Ŀ�������

	ChallengeUser * GetChallengeUser(const UniqueUserID &user_id, int *rankpos = NULL);					// ��ȡ�����������Ϣ
	ChallengeUser * GetChallengeUserByPos(int pos);													// ��ȡ�����������Ϣ
	void ConstructChallengeParam(Role *role,ChallengeParam * param, RoleAppearance &appearance);    // ��ȡ�����������Ϣ

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

	int GetRoleRewardRankPos(int uid);																		// ��ȡ�������
	int GetChallengeUserRankPos(Role *role);																// ��ȡ�������
	void GMSetEmemySelf(Role *role, int percent);															// ���õ���

	///////////////////////////////////// ��������� ////////////////////////////////////////////
	bool IsCanSyncCrossChallengeUserInfo();																	// �ܷ��������ͬ��
	void OnSyncChallengeUserInitDataToGame(NetID net_id);													// �·���ʼ���ݸ���Ϸ��
	void OnSyncOrignalChallengefieldInfo(crossgameprotocal::SyncCrossChallengefieldDataInfo *sccbi);		// ����Ϸ����������
	void OnSyncChallengefieldRankInfo(crossgameprotocal::SyncCrossChallengefieldDataInfo *sccbi);			// ��Ϸ�����������������
	void OnUpdateChallengefieldInfoToGame();																// �Ѹ�������ͬ������Ϸ��
	void CheckGiveCrossRankReward();																		// �������н���

	void CrossRandOpponentHelper(UniqueUserID user_id);


private: 
	ChallengeField();
	~ChallengeField();

	bool LoadChallengeUser(int rank_from);
	void LoadChallengeUserSucc();
	bool InitChallengeUser(const ChallengeUserListParam::ChallengeUserAttr &user_attr);

	void ResizeUserList(int size);
	void GetChangeList(ChallengeUserListParam *p, int rank_begin, int *rank_end) const;
	void ClearDirtyMark(int rank_begin, int rank_end);
	
	void CheckSave(time_t now_second);

	int GetFreeRankPosSlot();					// ��ȡ��ȱ����
	
	ChallengeUser * CreateChallengeUser(const UniqueUserID &user_id, const ChallengeParam &challenge_p, const RoleAppearance &challenge_appearance);
	void SetChallengeUser(int rankpos, ChallengeUser *challenge_user, bool is_init = false, bool need_reset_opponen = false);
	bool GenrateChallengeUserParamInfo(ChallengeParam &param, RoleAppearance &appearance, int rank_index, int robot_num);  // ���������������ɻ�������Ϣ

	void OnRankPosChange(UniqueUserID user_id, int rank_pos);
	void CheckGiveLuckyReward(time_t now_second);
	void CheckGiveRankReward(time_t now_second);
	void GenrateLuckyRankPos();
	bool randLimitCap(int rand_interval, int begin_pos, int challenge_cap, int& get_pos);

	///////////////////////////////////// ��������� ////////////////////////////////////////////
	bool GenrateCrossChallengeUserParamInfo(ChallengeParam &param, RoleAppearance &appearance, int rank_index, int robot_num);  // ���������������ɻ�������Ϣ
	void ResetCrossChallengeUserRank();																							// �������������
	void SwapCrossChallengeUserRank();																							// ������������
	void SwapChallengeUserRank(ChallengeUser *challenge_user_1, ChallengeUser *challenge_user_2);								// ��������

	int m_data_state;

	bool m_is_first;
	
	int m_rankuser_num;
	ChallengeUser **m_rankuser_list;
	bool *m_dirty_mark;
	bool *m_old_state;
	bool m_is_change;

	time_t m_last_save_time;
	
	typedef std::map<UniqueUserID, int> RankPosMap;
	typedef std::map<UniqueUserID, int>::iterator RankPosMapIt;
	RankPosMap m_rankpos_map;

	ChallengeFieldDataParam m_challenge_field_data;
	time_t m_last_check_give_lucky_reward_timestamp;
	time_t m_last_check_give_rank_reward_timestamp;

	Protocol::SCChallengeFieldRankInfo m_msg_rank_info;
};

#endif // __CHALLENGE_FIELD_HPP__

