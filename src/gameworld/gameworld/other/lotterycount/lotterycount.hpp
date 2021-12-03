#ifndef __LOTTERY_COUNT_HPP__
#define __LOTTERY_COUNT_HPP__

#include <map>
#include <vector>
#include <string>

#include "servercommon/rolelotterycountdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class Role;

class LotteryCount
{
public:
	LotteryCount();
	~LotteryCount();
	void SetRole(Role *role);
	void Init(Role * role, const RoleLotteryCountParam & param);
	void GetInitParam(RoleLotteryCountParam *param);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnRoleLogin();
	int ReplaceLotterySequence(int lottery_type, LotterySequence & lottery_sequence, int* is_replace_garbage_sequence = nullptr);
	int ReplaceLotterySequenceBySeqType(int lottery_type, int seq_type, LotterySequence & lottery_sequence);
	int GetRoleLotteryCount(int lottery_type, LotterySequence sequence);
	bool AddRoleLotteryCount(int lottery_type, LotterySequence sequence, UserLotteryCount count);
	bool ResetRoleLotteryCount(int lottery_type, LotterySequence sequence);
private:

	Role * m_role;
	RoleLotteryCountParam m_param;
};

#endif // __LOTTERY_COUNT_HPP__

