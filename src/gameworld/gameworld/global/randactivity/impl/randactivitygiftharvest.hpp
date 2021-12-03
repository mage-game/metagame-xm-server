#ifndef RAND_ACTIVITY_GIFT_HARVEST_ACTIVITY_HPP__
#define RAND_ACTIVITY_GIFT_HARVEST_ACTIVITY_HPP__

#include "global/randactivity/randactivity.hpp"

enum GIFT_HARVEST_ROUND_STATUS
{
	GIFT_HARVEST_ROUND_STATUS_CLOSE = 0,	// ���ֹر�״̬
	GIFT_HARVEST_ROUND_STATUS_STANDY,		// ����׼��״̬
	GIFT_HARVEST_ROUND_STATUS_OPEN,			// ���ֽ���״̬
	GIFT_HARVEST_ROUND_STATUS_END,			// ���ֽ���״̬��������
	GIFT_HARVEST_ROUND_STATUS_MAX
};

struct RandActivityGiftHarvestParam
{
	struct RankItem
	{
		int role_id;
		int score;
	};

	int rank_num;	// �ϰ�����
	RankItem m_rank_list[GIFT_HARVEST_PERSON_RANK_MAX];
};

class RandActivityGiftHarvest : public RandActivity
{
public:
	RandActivityGiftHarvest(RandActivityManager *activity_manager);
	~RandActivityGiftHarvest();

	virtual void Init(const RandActivityData &data);
	virtual void GetInitParam(RandActivityData *data);
	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);

	int GetCurRound() {return m_cur_round;}								// ��ȡ��ǰ�ִ�
	void SendRankInfo(Role* role);										// �������а���Ϣ
	void SendRoundInfo(Role* role = nullptr);							// �����ִ�״̬��Ϣ
	void UpdateRankInfo(Role* role, int score_add);						// ��ӡ��滻�����½�ɫ����

private:

	void _Init(time_t now_second);			// ��ʼ��������ʱ��
	void _ResetRoundData(int round);		// ���ñ�����Ϣ���رջʱ���ã�
	void _DrawRankReward();					// �����н���
	void _ReadyCurRound();					// ׼������
	void _StartCurRound();					// ��ʼ����
	void _StopCurRound();					// ��������

	void _SyncActivityStatusToGameworld(int round_status);

	RandActivityGiftHarvestParam::RankItem * _GetRankItem(int role_id);	// ��ȡ����item
	RandActivityGiftHarvestParam::RankItem * _GetRankEndItem();			// ��ȡ�������һλitem

	bool m_is_init;

	int m_cur_round;					// ����	�ִΣ�> 0��
	int m_cur_round_status;				// ����	״̬ (enum GIFT_HARVEST_ROUND_STATUS)
	time_t m_cur_round_ready_timestamp;	// ���� ׼��ʱ���
	time_t m_cur_round_open_timestamp;	// ����	����ʱ���
	time_t m_cur_round_close_timestamp;	// ����	����ʱ���
	
	//int m_cur_rank_num;													// �ϰ�����
	//GiftHarvestRankItem m_cur_round_rank[GIFT_HARVEST_PERSON_RANK_MAX];	// �������а�
	RandActivityGiftHarvestParam m_param;									// ���а�����

	bool rank_update_flag;	// ���а�ĸ��±�־�����Ը���Э�飩
};


#endif // RAND_ACTIVITY_GIFT_HARVEST_ACTIVITY_HPP__

