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

	bool IsInCurrentRound(int role_id); // 是否在参与本轮云购
	bool IsAllRoundFinish();			// 是否已完成当天所有轮数
	bool CanJoin();						// 当前是否能参加
	bool IsThisRoundAllSell();			// 本轮是库存是否卖完
	short GetThisRoundBuyNum();			// 本轮的当前购买数
	short GetThisRoundIndex();			// 今日已进行到第几轮

	void SendBuyRecordInfo(Role *role, unsigned int last_open_timestamp);	// 发送购买记录
	void RecordOpenState(Role* role);						// 记录面板打开状态
	void RecordCloseState(Role* role);						// 记录面板关闭状态
	void RecordBuy(Role* role);								// 记录购买

private:

	void InitToday();
	void NoticeAllCanBuy(int open_flag);

	int GetLotteryDrawing();						// 抽取幸运儿
	void RewardLucky(int role_id,unsigned int join_timestamp = 0);					// 发奖
	void UnRewardLucky();							// 不发奖
	void StopCurrentRound();						// 结束本轮
	void StartNextRound(time_t now_second);			// 开始下一轮

	int m_rounds_num;						// 今日轮数
	bool m_is_init;					// 

	int m_round_index;						// 本轮索引
	int m_this_round_sale_num;				// 本轮发售量
	int m_this_round_buy_num;				// 本轮已售量
	std::map<int, int> m_role_id_buy_map;	// 本轮参与购买的玩家
	time_t m_this_round_end_tick;			// 本轮结束的时间戳
	time_t m_next_round_begin_tick;			// 下一轮开始的时间戳
	bool m_can_buy;							// 能否购买的标志
	bool m_is_this_round_end;				// 本轮得结束标志

	std::set<int> m_open_panel_role;		// 记录打开面板中的玩家（实时更新数据的需求）
	unsigned int m_last_buy_timestamp;		// 上一个购买时的时间戳
	std::deque<RandActivityLuckyCloudBuyParam::BuyLogItem> m_buy_record;	// 购买历史记录
};

#endif


