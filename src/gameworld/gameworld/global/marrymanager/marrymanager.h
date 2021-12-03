#ifndef __MARRY_MANAGER_H__
#define __MARRY_MANAGER_H__

#include "servercommon/marrydef.hpp"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "protocal/msgother.h"
#include "engineadapter.h"

#include "marry.h"

class Role;

class MarryManager
{
public:
	MarryManager();
	~MarryManager();


	void Init(const MarryReserveParam &marry_reserve_param);
	void GetInitParam(MarryReserveParam *marry_reserve_param);

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void OnMarryReserveReq(Role *user, int reserve_index, int marry_level, int rune_index);
	void OnMarryReserveAck(gglobalprotocal::MarryReserveAck *mra);
	void OnMarryHunyanInvite(gglobalprotocal::MarryHunyanInvite *mhi);
	void OnMarryHunyanEnterReq(Role *user);
	void OnMarryGetReserveInfo(Role *user);
	void OnMarryobjSyncPos(gglobalprotocal::MarryobjSyncPosToGlobal *msptg);
	
	void OnUserLogin(Role *user);

	void OnGmForceMarryStart();
	void OnGmForceMaryyToNextStatus();

private:
	void OnMarryStart(int marry_index);
	void OnMarryStop();

	void CalcNextMarryStartTime();

	unsigned int m_last_oper_time_list[MARRY_MAX_DAILY_MERRY_NUM];				// 操作时间戳 两次操作大于2S时间
	MarryReserveParam m_marry_param;

	Marry m_marry;																// 当前婚礼

	int m_next_marry_index;														// 下一场婚礼的索引
	unsigned int m_next_marry_start_time;
};

#endif // __MARRY_MANAGER_H__

