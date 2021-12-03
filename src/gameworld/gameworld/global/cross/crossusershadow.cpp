#include "crossusershadow.hpp"

#include "internalcomm.h"
#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/internalprotocal/crossglobalprotocal.h"
#include "obj/character/role.h"
#include "world.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "crossmanager.h"
#include"gameworld/gameworld/chat/chatmanager.h"

CrossUserShadow::CrossUserShadow()
{

}

CrossUserShadow::~CrossUserShadow()
{

}

CrossUserShadow & CrossUserShadow::Instance()
{
	static CrossUserShadow cus;
	return cus;
}

void CrossUserShadow::OnSyncCrossUser(crossgameprotocal::CrossGameSyncCrossUser *cgscu)
{
	switch (cgscu->sync_type)
	{
		case crossgameprotocal::CrossGameSyncCrossUser::SYNC_CROSS_USER_INIT:
		case crossgameprotocal::CrossGameSyncCrossUser::SYNC_CROSS_USER_ADD:
			{
				if (crossgameprotocal::CrossGameSyncCrossUser::SYNC_CROSS_USER_INIT == cgscu->sync_type)
				{
					//m_in_cross_user_map.clear();
					ClearCrossUserMap(cgscu->cross_type);
				}
				
				for (int i = 0; i < cgscu->count && i < crossgameprotocal::CrossGameSyncCrossUser::MAX_IN_CROSS_USER_NUM; ++ i)
				{
					UserID user_id = IntToUid(cgscu->in_cross_user_list[i].uid);
					m_in_cross_user_map[user_id] = cgscu->in_cross_user_list[i].cross_activity_type;
					
					World::GetInstWorld()->GetSceneManager()->OnCrossUserAdd(cgscu->in_cross_user_list[i].uid, cgscu->cross_type);

					Role *user = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
					if (NULL != user && NULL != user->GetScene())
					{
						EngineAdapter::Instance().NetDisconnect(user->GetNetId(), user->GetUserId(), user->GetName(), user->GetScene()->GetSceneID(), "cross");
					}
				}
			}
			break;

		case crossgameprotocal::CrossGameSyncCrossUser::SYNC_CROSS_USER_REMOVE:
			{
				for (int i = 0; i < cgscu->count && i < crossgameprotocal::CrossGameSyncCrossUser::MAX_IN_CROSS_USER_NUM; ++ i)
				{
					UserID user_id = IntToUid(cgscu->in_cross_user_list[i].uid);
					m_in_cross_user_map.erase(user_id);
				}

			}
			break;
	}
}

void CrossUserShadow::OnDisconnectFromCrossServer(const int cross_type)
{
	// 清除该战区的跨服状态角色表
	ClearCrossUserMap(cross_type);
}

void CrossUserShadow::OnUserLogin(Role *user)
{
	if (user->IsInCross()) return;

	InCrossUserMapIt it = m_in_cross_user_map.find(user->GetUserId());
	if (it != m_in_cross_user_map.end())
	{
		crossgameprotocal::GameCrossKickCrossUser gckcu;
		gckcu.bekick_original_uuid = UniqueUserIDToLongLong(user->GetUniqueUserID());
		//InternalComm::Instance().SendToCross((const char *)&gckcu, sizeof(gckcu), it->second);
		CrossManager::Instance().SendToAllCross((const char *)&gckcu, sizeof(gckcu));  // 通知所有连接的跨服尝试踢出这个角色
	}
}

void CrossUserShadow::ClearCrossUserMap(const int cross_type)
{
	if (cross_type > 0)
	{
		for (InCrossUserMapIt it = m_in_cross_user_map.begin(); it != m_in_cross_user_map.end(); )
		{
			int tmp_cross_type = CrossConfig::Instance().GetCrossTypeWithCrossFuncType(it->second);
			if (cross_type == tmp_cross_type)
			{
				m_in_cross_user_map.erase(it++);
			}
			else
			{
				it++;
			}
		}
	}
	else
	{
		m_in_cross_user_map.clear();
	}
}

