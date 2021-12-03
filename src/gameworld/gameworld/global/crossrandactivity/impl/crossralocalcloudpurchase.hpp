#ifndef __CROSS_RA_LOCAL_CLOUDPURCHASE_HPP__
#define __CROSS_RA_LOCAL_CLOUDPURCHASE_HPP__

#include "iclockmodule.h"

#include "servercommon/activitydef.hpp"
#include "servercommon/serverdef.h"
#include "global/crossrandactivity/crossralocal.hpp"
#include "servercommon/crossdef.hpp"

class CrossRALocalCloudPurchase : public CrossRALocal
{
public:
	CrossRALocalCloudPurchase();
	virtual ~CrossRALocalCloudPurchase();

	virtual void Update(unsigned long interval, unsigned int now_second);

	virtual void OnMessageFromCross(int msg_type, char *data);

	void OnUserBuy(Role *role, int seq, int times);
	void SendAllInfoToUser(Role *role = NULL);
	void SendServerRecordToUser(Role *role = NULL);

protected:

	void CheckGiveRewards(int type, int uid, int server_id, GameName name);
	void GiveScoreToAllJoinUser(int type, int except_uid);
	void AddRecordReward(int type, int uid, int server_id, GameName name, ItemID item_id);

	void OnInfoChange(int type, int times, unsigned int timestamp);
	void SyncAllInfoToCross();

	virtual void OnActivityStatusChange(int from_status, int to_status);
};

#endif // __CROSS_RA_LOCAL_ZHONGZHI_RANK_HPP__

