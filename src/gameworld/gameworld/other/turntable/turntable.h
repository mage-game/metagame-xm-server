#ifndef __TURN_TABLE_CLASS__
#define __TURN_TABLE_CLASS__

#include "gameworld/other/turntable/turntableconfig.h"

class Role;

class TurnTable
{
public:
	TurnTable();
	~TurnTable();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const short lucky_value_list[TURNTABLE_TYPE_MAX_COUNT]);
	void GetInitParam(short lucky_value_list[TURNTABLE_TYPE_MAX_COUNT]);

	void OnRoll(int req);

	void SendInfo();

	void NotifyImage(Role *role);

private:
	Role *m_role;
	short m_lucky_value_list[TURNTABLE_TYPE_MAX_COUNT];
};

#endif

