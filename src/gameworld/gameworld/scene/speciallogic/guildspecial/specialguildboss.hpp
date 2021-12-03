#ifndef __GUILD_BOSS_HPP__
#define __GUILD_BOSS_HPP__

#include <map>
#include <set>
#include <stdio.h>

#include "iclockmodule.h"
#include "scene/speciallogic/speciallogic.hpp"

#include "config/guildactivityconfig.hpp"
#include "servercommon/serverdef.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "protocal/msgguild.hpp"
#include "servercommon/string/globalstr.h"

class SpecialGuildBoss : public SpecialLogic
{
public:
	SpecialGuildBoss(Scene *scene);
	virtual ~SpecialGuildBoss();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual bool SpecialCanGuWu() { return true; } 
	virtual bool CanDestroy() { return false; }
	virtual bool SpecialCanMountOn(Role *role){return false;}
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	void ResetGuildBoss(bool is_kickout_all_role);

private:
	bool m_is_inited;
};

#endif // __GUILD_STATION_HPP__

