#ifndef __SPECIAL_CROSS_FISHING_HPP__
#define __SPECIAL_CROSS_FISHING_HPP__
#include <map>
#include "scene/speciallogic/speciallogic.hpp"
#include "servercommon/serverconfig/crossfishingconfig.hpp"

struct CrossFishingUserInfo
{
	enum FISHING_STATUS
	{
		FISHING_STATUS_IDLE,				// δ���㣬�����ڵ������
		FISHING_STATUS_WAITING,				// �ڵ������ȴ��׸�
		FISHING_STATUS_CAST,				// �Ѿ��׸ͣ��ȴ������¼�
		FISHING_STATUS_HOOKED,				// �Ѿ������¼����ȴ�����
		FISHING_STATUS_PULLED,				// �Ѿ����ˣ��ȴ������ѡ��
	};

	enum SPECIAL_STATUS
	{
		SPECIAL_STATUS_OIL,					// ʹ��������
		SPECIAL_STATUS_AUTO_FISHING,		// �Զ�����
		SPECIAL_STATUS_QUICK_FISHING,		// ���ٵ���
	};

	CrossFishingUserInfo()
	{
		uuid = 0;
		prof = 0;
		special_status_flag = 0;
		least_count_cfg_index = 0;
		is_consumed_auto_fishing = 0;
		is_init = 0;
		cur_score_stage = 0;
		score_stage_fetch_flag = 0;
		total_score = 0;
		special_status_oil_end_timestamp = 0;
		steal_fish_count = 0;
		be_stealed_fish_count = 0;
		buy_steal_count = 0;
		news_count = 0;

		memset(user_name, 0, sizeof(user_name));
		memset(fish_num_list, 0, sizeof(fish_num_list));
		memset(gear_num_list, 0, sizeof(gear_num_list));

		this->Reset();
	}

	// ���õ��տ�ʼ����ʱ��״̬
	void Reset()
	{
		fishing_status = FISHING_STATUS_WAITING;
		use_bait_type = -1;
		get_fish_type = -1;
		event_type = -1;
		event_trigger_timestamp = 0;
		auto_pull_timestamp = 0;

		memset(helper_uuid_list, 0, sizeof(helper_uuid_list));
	}

	bool IsSpecialStatusSet(int status) const
	{
		return (special_status_flag & (1 << status)) != 0;
	}

	void SetSpecialStatus(int status)
	{
		special_status_flag |= (1 << status);
	}

	void ClearSpecialStatus(int status)
	{
		special_status_flag &= ~(1 << status);
	}

	long long 		uuid;											// ���uid
	GameName		user_name;										// �������
	char			prof;											// ְҵ
	char			fishing_status;									// ��ǰ����״̬
	unsigned char	special_status_flag;							// ����״̬���
	char			use_bait_type;									// ��ǰʹ�õ��������

	char			get_fish_type;									// ��ǰ��õ�������
	char			event_type;										// ��ǰ�������¼�����
	char			least_count_cfg_index;							// ��͵���������������
	char			is_consumed_auto_fishing;						// �Ƿ����ѹ��Զ�����

	char			is_init;										// �Ƿ��ѳ�ʼ��
	char			cur_score_stage;								// ��ǰ���ֽ׶�
	short			score_stage_fetch_flag;							// ���ֽ׶ν�����ȡ���
	int				total_score;									// �ܻ���

	time_t			event_trigger_timestamp;						// �¼�����ʱ���
	time_t			auto_pull_timestamp;							// �Զ�����ʱ�����û�д����¼���Ϊ0
	time_t			special_status_oil_end_timestamp;				// ����״̬���ͽ���ʱ���

	long long		helper_uuid_list[MAX_TEAM_MEMBER_NUM];			// ��æ������ͬ���uid
	int				fish_num_list[FISHING_FISH_TYPE_MAX_COUNT];		// ��ǰ���ϵĸ����������
	int				gear_num_list[FISHING_GEAR_MAX_COUNT];			// ��ǰӵ�еķ�������

	int				steal_fish_count;								// ͵�����
	int				be_stealed_fish_count;							// ��͵�����
	int				buy_steal_count;								// ����͵�����

	int				news_count;										// ��־����
	FishingNews		news_list[FISHING_NEWS_MAX];					// ��־�б�

	std::vector<ItemConfigData> get_reward_item_vec;				// �����Ʒ����
};

class Role;

class SpecialCrossFishing : public SpecialLogic
{
public:

	struct ScoreRankItem
	{
		ScoreRankItem() : uuid(0), total_score(0) {}

		inline bool operator < (ScoreRankItem& rhs) const
		{
			return total_score > rhs.total_score;
		}

		long long  uuid;
		int total_score;
	};

	SpecialCrossFishing(Scene *scene);
	virtual ~SpecialCrossFishing();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool CanDestroy();
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual bool SpecialCanGather(Role *role, GatherObj *gather);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj) { return false; }
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual int GetSpecialParam(Role *role);

	void OnActivityStatusChange(int status);

	void GmAddGear(Role *role);

public:
	void ResetUserInfoOnFail(Role *role, const char *reason);			// �������̳���ʱ���������״̬���տ�ʼ����ʱ��״̬

																		// ����Ŵ�������������Ľӿ�
	bool StartFishing(Role *role);										// ��ʼ���㣨���������棩
	bool CastingRods(Role *role, int bait_type);						// �׸ͣ�����1��������� ����2:�Ƿ���������
	bool PullRods(Role *role, bool is_perfect);							// ���ˣ��������¼�
	bool ConfirmEvent(Role *role, int param1);							// ȷ�ϱ��ε����¼�
	void UseFishingGear(Role *role, int type);							// ʹ�÷���
	void HelpBigFish(Role *helper_role, int be_helped_uid);				// ����������
	void StopFishing(Role *role);										// ֹͣ���㣨�뿪������棩
	void AutoFishingReq(Role *role, int is_set, int special_status);	// �����Զ����㣬is_setΪ0����ȡ��
	void OnRandUserReq(Role *role);										// ������������Ϣ
	bool IsCanStealFish(Role *role, long long target_uuid);				// �ܷ�͵��(�ͻ�����ʾ)
	void OnStealFishReq(Role *role, long long target_uuid);				// ����͵��
	void OnBuyStealCountReq(Role *role);								// ������͵�����
	void OnExChange(Role* role, int combination_index);					// ����һ�
	void OnBuyBait(Role* role, int bait_type, int num);					// �������
	void OnFetchScoreReward(Role* Role);								// ��ȡ���ֽ���

	void SendFishingRankListToRole(Role * role = nullptr);
	void SetEndTime(unsigned int end_time) { m_activity_end_time = end_time; }

private:
	inline void OnActivityStart() { m_isopen = true; }
	void OnActivityClose();

	void OnEventTrigger(Role *role);														// �����¼�

	void OnCheckEventGetFish(Role *role, CrossFishingUserInfo *user_info);					// ����¼������Ϲ�
	void OnCheckEventTreasure(Role *role, CrossFishingUserInfo *user_info);					// ����¼��ƾɱ���
	void OnCheckEventGotYuwang(Role *role, CrossFishingUserInfo *user_info);				// ����¼�����
	void OnCheckEventGotYucha(Role *role, CrossFishingUserInfo *user_info);					// ����¼����
	void OnCheckEventGotOil(Role *role, CrossFishingUserInfo *user_info);					// ����¼�����
	void OnCheckEventRubber(Role *role, CrossFishingUserInfo *user_info);					// ����¼�����
	void OnCheckEventBigFish(Role *role, CrossFishingUserInfo *user_info);					// ����¼���˵�еĴ���

	void OnConfirmEventGetFish(Role *role, CrossFishingUserInfo *user_info, int give_uid);	// ȷ���¼������Ϲ�������3��Ҫ������ң����Լ���0
	void OnConfirmEventTreasure(Role *role, CrossFishingUserInfo *user_info, bool is_open);	// ȷ���¼��ƾɱ���
	void OnConfirmEventFisher(Role *role, CrossFishingUserInfo *user_info);					// ȷ���¼�����������
	void OnConfirmEventRubber(Role *role, CrossFishingUserInfo *user_info);					// ȷ���¼�����
	void OnConfirmEventBigFish(Role *role, CrossFishingUserInfo *user_info, int give_uid);	// ȷ���¼���˵�еĴ��㣬����3��Ҫ������ң����Լ���0

	bool AddUserInfo(Role *role);														// ��������Ϣ
	CrossFishingUserInfo* GetUserInfo(long long uid);									// ��ȡ�����Ϣ
	void SendUserInfo(Role *role);														// ���������Ϣ
	void SendTeamMemberInfo(Role *role);												// ������Ҷ��ѵ�fish_num_list���Լ�
	void SendFishInfoToTeam(Role *role);												// ������ҵ�fish_num_list�����ѣ������Լ���
	void SendCheckEventResult(Role *role, int param1, int param2, int param3);			// ���ͼ���¼����

	void UpdateScoreRank(Role *role);
	void SendStageScoreInfo(Role* role);												// ����׶λ�����Ϣ
	void NotifyStatuChange(Role* role);													// ����״̬�ı�

	void ChangeFishNum(Role *role, int fish_type, int num, bool is_chg_score = true,bool is_steal = false);	// ��������ı�ʱ
	void NoticeActStatus();																// ����״̬
	void CheckRewardOnClose();
	void AddRewardItem(long long uuid, const ItemConfigData *reward_item_list, int put_count);// ��ӻ������Ʒ��չʾ�ã�

private:
	bool m_isopen;
	bool m_has_notice_act_close;														// �Ƿ񹫸�����Ҫ����

	typedef std::map<long long, CrossFishingUserInfo> FishingUserList;
	typedef std::map<long long, CrossFishingUserInfo>::iterator FishingUserIt;
	FishingUserList m_user_list;

	std::vector<ScoreRankItem> m_score_rank_vec;										// ������������б�

	typedef std::map<long long, unsigned short> GatherIdMap;							// ��ҵ���Ĳɼ���
	GatherIdMap m_gather_id_map;

	unsigned int m_activity_end_time;
	int m_broadcast_times;
	unsigned int m_next_broadcast_time;													
};

#endif


