#ifndef __FAMOUS_MAN_HPP__
#define __FAMOUS_MAN_HPP__

#include "protocal/msgfamousman.h"

class Role;

class FamousMan
{
public: 
	FamousMan();
	~FamousMan();

	void SetRole(Role *role) { m_role = role; }	

	void FamousManOpera(Role *role, int opera_type);
	void OnCheckFamousMan(int famous_type, int param);

	void SendFamousManInfo(Role *role = NULL);	

private:
	Role			*m_role;
};

#endif // __FAMOUS_MAN_HPP__



