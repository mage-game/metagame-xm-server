#ifndef __RANDACTIVITY_LUCKY_CLOUD_BUY__
#define __RANDACTIVITY_LUCKY_CLOUD_BUY__

#include "global/randactivity/randactivity.hpp"

class RandActivityLuckyCloudBuy : public RandActivity
{
public:
	RandActivityLuckyCloudBuy(RandActivityManager *rand_activity_manager);
	virtual ~RandActivityLuckyCloudBuy();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	virtual void OnUserLogin(Role *user);
	virtual void OnUserLogout(Role *user);
	virtual void OnSpecialActivityStatusChange(int from_status, int to_status);
	void InitActivityData();
	void SaveActivityData();

	bool IsInCurrentRound(int role_id); // �Ƿ��ڲ��뱾���ƹ�
	bool IsAllRoundFinish();			// �Ƿ�����ɵ�����������
	bool CanJoin();						// ��ǰ�Ƿ��ܲμ�
	bool IsThisRoundAllSell();			// �����ǿ���Ƿ�����
	short GetThisRoundBuyNum();			// ���ֵĵ�ǰ������
	short GetThisRoundIndex();			// �����ѽ��е��ڼ���

	void SendBuyRecordInfo(Role *role, unsigned int last_open_timestamp);	// ���͹����¼
	void RecordOpenState(Role* role);						// ��¼����״̬
	void RecordCloseState(Role* role);						// ��¼���ر�״̬
	void RecordBuy(Role* role);								// ��¼����

private:

	void InitToday();
	void NoticeAllCanBuy(int open_flag);

	int GetLotteryDrawing();						// ��ȡ���˶�
	void RewardLucky(int role_id,unsigned int join_timestamp = 0);					// ����
	void UnRewardLucky();							// ������
	void StopCurrentRound();						// ��������
	void StartNextRound(time_t now_second);			// ��ʼ��һ��

	int m_rounds_num;						// ��������
	bool m_is_init;					// 

	int m_round_index;						// ��������
	int m_this_round_sale_num;				// ���ַ�����
	int m_this_round_buy_num;				// ����������
	std::map<int, int> m_role_id_buy_map;	// ���ֲ��빺������
	time_t m_this_round_end_tick;			// ���ֽ�����ʱ���
	time_t m_next_round_begin_tick;			// ��һ�ֿ�ʼ��ʱ���
	bool m_can_buy;							// �ܷ���ı�־
	bool m_is_this_round_end;				// ���ֵý�����־

	std::set<int> m_open_panel_role;		// ��¼������е���ң�ʵʱ�������ݵ�����
	unsigned int m_last_buy_timestamp;		// ��һ������ʱ��ʱ���
	std::deque<RandActivityLuckyCloudBuyParam::BuyLogItem> m_buy_record;	// ������ʷ��¼
};

#endif


