#ifndef __CROSS_RA_LOCAL_ZHONGZHI_RANK_HPP__
#define __CROSS_RA_LOCAL_ZHONGZHI_RANK_HPP__

#include "iclockmodule.h"

#include "servercommon/activitydef.hpp"
#include "servercommon/serverdef.h"
#include "global/crossrandactivity/crossralocal.hpp"

class CrossRALocalChongzhiRank : public CrossRALocal
{
	struct LocalChongzhiInfo
	{		
		long long server_total_chongzhi;									// 本服活动期间总充值

		int mvp_uid;														// 该服充值最高的玩家
		GameName mvp_name;													// 玩家名字
		int mvp_server_id;													// 玩家原服务器id
		long long mvp_total_chongzhi;										// 玩家充值额度
	};

public:
	CrossRALocalChongzhiRank();
	virtual ~CrossRALocalChongzhiRank();

	virtual void Update(unsigned long interval, unsigned int now_second);
	virtual void OnDayChange() {}
	virtual void OnWeekChange() {}

	virtual void OnMessageFromCross(int msg_type, char *data);

	void OnSomeOneChongzhi(Role *role, long long add_chongzhi, long long total_chongzhi);

protected:
	void CheckSendRewards(int rank);
	void ResetAllData();
	virtual void OnActivityStatusChange(int from_status, int to_status);

	LocalChongzhiInfo *m_chongzhi_info;
	unsigned int m_next_sync_to_cross_time;
};

#endif // __CROSS_RA_LOCAL_ZHONGZHI_RANK_HPP__

