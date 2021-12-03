#ifndef __CROSS_RA_LOCAL_CONSUME_RANK_HPP__
#define __CROSS_RA_LOCAL_CONSUME_RANK_HPP__

#include "iclockmodule.h"

#include "servercommon/activitydef.hpp"
#include "servercommon/serverdef.h"
#include "global/crossrandactivity/crossralocal.hpp"

class CrossRALocalConsumeRank : public CrossRALocal
{
	struct LocalConsumeInfo
	{		
		long long server_total_consume;										// ������ڼ�������

		int mvp_uid;														// �÷�������ߵ����
		GameName mvp_name;													// �������
		int mvp_server_id;													// ���ԭ������id
		long long mvp_total_consume;										// ������Ѷ��
	};

public:
	CrossRALocalConsumeRank();
	virtual ~CrossRALocalConsumeRank();

	virtual void Update(unsigned long interval, unsigned int now_second);
	virtual void OnDayChange() {}
	virtual void OnWeekChange() {}

	virtual void OnMessageFromCross(int msg_type, char *data);

	void OnSomeOneConsume(Role *role, long long add_consume, long long total_consume);

protected:
	void CheckSendRewards(int rank);
	void ResetAllData();
	virtual void OnActivityStatusChange(int from_status, int to_status);

	LocalConsumeInfo *m_consume_info;
	unsigned int m_next_sync_to_cross_time;
};

#endif // __CROSS_RA_LOCAL_CONSUME_RANK_HPP__

