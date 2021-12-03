#ifndef __ACTIVITY_QUN_XIAN_LUAN_DOU_HPP__
#define __ACTIVITY_QUN_XIAN_LUAN_DOU_HPP__

#include "global/activity/activity.hpp"

class ActivityQunXianLuanDou : public Activity
{
public:
	ActivityQunXianLuanDou(ActivityManager *activity_manager);
	virtual ~ActivityQunXianLuanDou();

	virtual void OnActivityStatusChange(int from_status, int to_status);
	virtual int GetRoomMaxUserCount();

	void CheckFirstThreeRankUser(QunXianLuanDouUserRankItem rank_list[], int list_len);
	void ClearRankTitle();
protected:
	virtual bool IsRecordRoomStatus(int *activity_sceneid = NULL);

	int m_room_count;
	QunXianLuanDouUserRankItem m_total_rank_list[QUNXIANLUANDOU_SIDE_MAX];
};

#endif // __ACTIVITY_QUN_XIAN_LUAN_DOU_HPP__

