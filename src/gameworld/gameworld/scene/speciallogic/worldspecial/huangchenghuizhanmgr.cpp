#include "huangchenghuizhanmgr.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/monsterreplaceconfig.hpp"
#include "config/activityconfig/huangchenghuizhanconfig.hpp"
#include "monster/monsterpool.h"

#include "obj/character/role.h"
#include "obj/character/monster.h"

#include "servercommon/errornum.h"

#include "world.h"
#include "scene/scene.h"
#include "global/worldstatus//worldstatus.hpp"

#include "protocal/msgrole.h"

#include "other/event/eventhandler.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "other/achieve/title/titleconfig.hpp"

HuangchenghuizhanMgr::HuangchenghuizhanMgr() : m_next_refresh_time(0), m_monster_num(0)
{
	
}

HuangchenghuizhanMgr::~HuangchenghuizhanMgr()
{

}

HuangchenghuizhanMgr & HuangchenghuizhanMgr::Instance()
{
	static HuangchenghuizhanMgr sshz;
	return sshz;
}

void HuangchenghuizhanMgr::OnRoleLogin(Role *role)
{
	this->SyncHuangChengInfoToRole(role);
}

void HuangchenghuizhanMgr::Update(unsigned long interval, time_t now_second)
{
	if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_HUANGCHENGHUIZHAN))
	{
		return;
	}

	if (m_next_refresh_time != 0 && now_second > m_next_refresh_time)
	{
		m_next_refresh_time = 0;

		const HchzOtherCfg &other_cfg = LOGIC_CONFIG->GetHuangChengHuiZhanCfg().GetOtherCfg();
		Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.sceneid, 0);
		if (NULL == scene)
		{
			return;
		}

		const std::vector<HchzRefreshCfg> &refresh_vec = LOGIC_CONFIG->GetHuangChengHuiZhanCfg().GetRefreshCfg();
		std::vector<HchzRefreshCfg>::const_iterator iter;
		
		for (iter = refresh_vec.begin(); refresh_vec.end() != iter; ++ iter)
		{
			int monsterid = LOGIC_CONFIG->GetReMonsterCfg().GetReMonsterIdByWorldLevel(iter->monsterid);
			
			for (int i = 0; i < iter->num; ++ i)
			{
				Posi refresh_pos;
				LOGIC_CONFIG->GetHuangChengHuiZhanCfg().GetRandomRefresPos(&refresh_pos);

				if (MONSTERPOOL->CreateMonster(monsterid, scene, refresh_pos))
				{
					++ m_monster_num;
				}
			}
		}

		this->SyncHuangChengInfoToAll();
	}
}

void HuangchenghuizhanMgr::OnMonsterDie(Monster *monster, Role *killer_role)
{
	if (!ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_HUANGCHENGHUIZHAN))
	{
		return;
	}

	if (NULL == monster)
	{
		return;
	}

	const HchzOtherCfg &other_cfg = LOGIC_CONFIG->GetHuangChengHuiZhanCfg().GetOtherCfg();

	if (NULL == monster->GetScene() || monster->GetScene()->GetSceneID() != other_cfg.sceneid)
	{
		return;
	}

	int monsterid = LOGIC_CONFIG->GetReMonsterCfg().GetOrginalWorldLevMonsterId(monster->GetMonsterId());
	if (!LOGIC_CONFIG->GetHuangChengHuiZhanCfg().IsHuangchenghuizhanMonsterId(monsterid))
	{
		return;
	}
	
	if (m_monster_num > 0)
	{
		-- m_monster_num;

		if (m_monster_num <= 0)
		{
			m_next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.time;
		}
		
		if (NULL != killer_role)
		{
			killer_role->AddHCHZKillCount(1);
			killer_role->GetTitleManager()->CheckAllTitleCondition(TITLE_TYPE_HCHZ_KILLER);
		}

		this->SyncHuangChengInfoToAll();
	}
}

void HuangchenghuizhanMgr::OnActivityStatusChange(int status)
{
	switch (status)
	{
	case ACTIVITY_STATUS_OPEN:
		{
			m_next_refresh_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			this->SyncHuangChengInfoToAll();
		}
		break;

	case ACTIVITY_STATUS_CLOSE:
		{
			m_next_refresh_time = 0;
			m_monster_num = 0;
			this->KillAllMonster();

			this->SyncHuangChengInfoToAll();
		}
		break;
	}
}

void HuangchenghuizhanMgr::KillAllMonster()
{
	const HchzOtherCfg &other_cfg = LOGIC_CONFIG->GetHuangChengHuiZhanCfg().GetOtherCfg();
	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(other_cfg.sceneid, 0);
	if (NULL == scene)
	{
		return;
	}
	UInt32 monster_num = scene->MonsterNum();
	for (UInt32 i = 0; i < monster_num; ++i)
	{
		Monster *monster = scene->GetMonsterByIndex(i);
		if (NULL != monster)
		{
			int monsterid = LOGIC_CONFIG->GetReMonsterCfg().GetOrginalWorldLevMonsterId(monster->GetMonsterId());
			if (LOGIC_CONFIG->GetHuangChengHuiZhanCfg().IsHuangchenghuizhanMonsterId(monsterid))
			{
				monster->ForceSetDead();
			}
		}
	}
}

bool HuangchenghuizhanMgr::IsInHuangChengHuizhan(Role *role)
{
	if (NULL == role) return false;

	const HchzOtherCfg &other_cfg = LOGIC_CONFIG->GetHuangChengHuiZhanCfg().GetOtherCfg();

	if (ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_HUANGCHENGHUIZHAN) && 
		role->GetScene()->GetSceneID() == other_cfg.sceneid)
	{
		return true;
	}

	return false;
}

void HuangchenghuizhanMgr::SyncHuangChengInfoToAll()
{
	static Protocol::SCHuangChengHuiZhanInfo cmd;
	cmd.monster_num = m_monster_num;
	cmd.next_refrestime = m_next_refresh_time;

	World::GetInstWorld()->SendToAllGateway((const char*)&cmd, sizeof(cmd));
}

void HuangchenghuizhanMgr::SyncHuangChengInfoToRole(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	static Protocol::SCHuangChengHuiZhanInfo cmd;
	cmd.monster_num = m_monster_num;
	cmd.next_refrestime = m_next_refresh_time;
	role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
}