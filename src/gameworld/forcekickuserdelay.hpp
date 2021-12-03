#ifndef __FORCE_KICK_USER_DELAY_HPP__
#define __FORCE_KICK_USER_DELAY_HPP__

#include "servercommon/serverdef.h"
#include "itimermodule.h"

#include <stdlib.h>

class ForceKickUserDelay : public ITimerCallback
{
public:
	ForceKickUserDelay(const UserID &kick_user_id, int kick_user_reason);
	virtual ~ForceKickUserDelay() {}

	virtual void OnTimer();
	virtual void Free();

	void *	operator new(size_t c);
	void	operator delete(void *m);

protected:
	UserID			m_kick_user_id;
	int				m_kick_user_reason;
};

#endif

