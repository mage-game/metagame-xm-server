#ifndef __RANKSHADOW_H_
#define __RANKSHADOW_H_

#include "iclockmodule.h"
#include "servercommon/rankdef.hpp"
#include "engineadapter.h"

class Role;

class RankShadow
{
public:
	union FLEXIBLE_LL_DATA
	{
		FLEXIBLE_LL_DATA() : ll(0) {}

		long long ll;

		struct
		{
			short a;
			short b;
			short c;
			short d;
		};
	};

	RankShadow();
	~RankShadow();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role);
	void Update(unsigned long interval, time_t now_second);
	void UpdateToGlobal();
	void UpdateToCross();
	void UpdateFunc();
	
	void OnRoleLeaveScene();
	void UpdataOneToGlobal(int rank_type, long long rank_value, int record_index, GameName flexible_name, int flexible_int, long long flexible_ll);

	void ForceSyncData() { m_next_update_time = EngineAdapter::Instance().Time(); }

private:
	Role *m_role;

	time_t m_next_update_time;
	time_t m_next_update_to_cross_time;
};

#endif //__RANKSHADOW_H_

