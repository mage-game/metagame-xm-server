#ifndef RAND_ACTIVITY_GIFT_HARVEST_ACTIVITY_HPP__
#define RAND_ACTIVITY_GIFT_HARVEST_ACTIVITY_HPP__

#include "global/randactivity/randactivity.hpp"

enum GIFT_HARVEST_ROUND_STATUS
{
	GIFT_HARVEST_ROUND_STATUS_CLOSE = 0,	// 本轮关闭状态
	GIFT_HARVEST_ROUND_STATUS_STANDY,		// 本轮准备状态
	GIFT_HARVEST_ROUND_STATUS_OPEN,			// 本轮进行状态
	GIFT_HARVEST_ROUND_STATUS_END,			// 本轮结束状态（发奖）
	GIFT_HARVEST_ROUND_STATUS_MAX
};

struct RandActivityGiftHarvestParam
{
	struct RankItem
	{
		int role_id;
		int score;
	};

	int rank_num;	// 上榜人数
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

	int GetCurRound() {return m_cur_round;}								// 获取当前轮次
	void SendRankInfo(Role* role);										// 发送排行榜信息
	void SendRoundInfo(Role* role = nullptr);							// 发送轮次状态信息
	void UpdateRankInfo(Role* role, int score_add);						// 添加、替换、更新角色排行

private:

	void _Init(time_t now_second);			// 初始化（开服时）
	void _ResetRoundData(int round);		// 重置本轮信息（关闭活动时重置）
	void _DrawRankReward();					// 发排行奖励
	void _ReadyCurRound();					// 准备本轮
	void _StartCurRound();					// 开始本轮
	void _StopCurRound();					// 结束本轮

	void _SyncActivityStatusToGameworld(int round_status);

	RandActivityGiftHarvestParam::RankItem * _GetRankItem(int role_id);	// 获取排行item
	RandActivityGiftHarvestParam::RankItem * _GetRankEndItem();			// 获取排行最后一位item

	bool m_is_init;

	int m_cur_round;					// 当轮	轮次（> 0）
	int m_cur_round_status;				// 当轮	状态 (enum GIFT_HARVEST_ROUND_STATUS)
	time_t m_cur_round_ready_timestamp;	// 当轮 准备时间戳
	time_t m_cur_round_open_timestamp;	// 当轮	开启时间戳
	time_t m_cur_round_close_timestamp;	// 当轮	结束时间戳
	
	//int m_cur_rank_num;													// 上榜人数
	//GiftHarvestRankItem m_cur_round_rank[GIFT_HARVEST_PERSON_RANK_MAX];	// 当轮排行榜
	RandActivityGiftHarvestParam m_param;									// 排行榜数据

	bool rank_update_flag;	// 排行榜的更新标志（用以更新协议）
};


#endif // RAND_ACTIVITY_GIFT_HARVEST_ACTIVITY_HPP__

