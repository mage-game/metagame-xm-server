#include <time.h>

#include "utility/configpath.h"
#include "task/taskpool.h"
#include "engineadapter.h"

#include "obj/character/role.h"
#include "npc.h"

#include "servercommon/errornum.h"

#include "config/sceneconfig.h"
#include "globalconfig/globalconfig.h"


NPC::NPC() : m_npc_id(0), m_pos(), m_default_dialog_id(0)
{
}

NPC::~NPC()
{
	
}

void NPC::Init(const ConfigSceneNpc& npcconfig)
{
	m_npc_id = npcconfig.id;
	m_pos = Posi(npcconfig.x, npcconfig.y);
	m_default_dialog_id = npcconfig.default_dialog_id;
}

bool NPC::CanTouch(Role *role, bool notify)
{
	if (NULL == role) return false;

	unsigned int MAX_TOUCH_DISTANCE = GLOBALCONFIG->GetNpcMaxTouchDistance();

	if ((m_pos - role->GetPos()).IsLongerThan(MAX_TOUCH_DISTANCE))
	{
		if (notify) role->NoticeNum(errornum::EN_TOO_FAR_FROM_NPC);
		return false;
	}

	return true;
}
