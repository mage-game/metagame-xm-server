#ifndef _ROLEZHANSHENDIAN_HPP_
#define _ROLEZHANSHENDIAN_HPP_

#include "obj/character/attribute.hpp"

class Role;

class RoleZhanshenDian
{
public:
	RoleZhanshenDian();
	~RoleZhanshenDian();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, short zhanshendian_pass_level, short yesterday_pass_level, unsigned int fetch_day_reward_timestamp);
	void GetInitParam(short *zhanshendian_pass_level, short *yesterday_pass_level, unsigned int *fetch_day_reward_timestamp);

	void SetPassLevel(int set_level);
	int GetPassLevel() { return m_zhanshendian_pass_level; }

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void FetchDayReward();

	void SendInfo();

private:
	Role *m_role;

	short m_zhanshendian_pass_level;
	short m_yesterday_pass_level;
	unsigned int m_fetch_day_reward_timestamp;
};

#endif


