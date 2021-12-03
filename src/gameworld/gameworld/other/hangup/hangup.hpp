#ifndef __HANGUP_HPP__
#define __HANGUP_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/rolehangupdef.hpp"

class Role;

class Hangup
{
public:
	Hangup();
	~Hangup();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const RoleHangupParam &p);
	void GetInitParam(RoleHangupParam *p);

	void SaveHangupSetData(HangupSetData setdata);
	void SaveCustomSetData(CustomSetData customsetdata);

	void SendHangupInfo();

private:
	Role *m_role;

	HangupSetData m_hangup_set;
	CustomSetData m_custom_set;
};


#endif // __HANGUP_HPP__

