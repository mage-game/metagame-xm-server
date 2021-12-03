#include "crossusermanager.hpp"
#include "crossuser.h"
#include "crossserver.h"
#include "internalcomm.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/serverconfig/crossconfig.hpp"

CrossUserManager::CrossUserManager()
	: m_cross_server(NULL)
{
	
}

CrossUserManager::~CrossUserManager()
{
	
}

void CrossUserManager::Release()
{
	for (OriginalUUIDCrossUserMapIt it = m_uniqe_user_id_user_map.begin(); it != m_uniqe_user_id_user_map.end(); ++ it)
	{
		delete it->second;
	}

	m_uniqe_user_id_user_map.clear();
	m_cross_user_id_user_map.clear();
}

void CrossUserManager::OnInternalGSDisconnnect(const UniqueServerID &unique_server_id)
{
	// GS断开连接了，把属于原服GS的玩家清理掉
	for (OriginalUUIDCrossUserMapIt it = m_uniqe_user_id_user_map.begin(); it != m_uniqe_user_id_user_map.end(); )
	{
		UniqueUserID unique_user_id(it->first);
		UniqueServerID user_unique_server_id(it->first.plat_type, it->first.user_id.db_index);

		if (user_unique_server_id == unique_server_id)
		{
			this->OnKickCrossUser(unique_user_id);

			m_uniqe_user_id_user_map.erase(it++);
		}
		else
		{
			++it;
		}
	}
}

void CrossUserManager::Update(unsigned long interval, time_t now_second)
{
	
}

CrossUser * CrossUserManager::GetCrossUser(const UniqueUserID &original_unique_user_id)
{
	OriginalUUIDCrossUserMapIt it = m_uniqe_user_id_user_map.find(original_unique_user_id);
	if (it != m_uniqe_user_id_user_map.end())
	{
		return it->second;
	}

	return NULL;
}

CrossUser * CrossUserManager::GetCrossUser(const UserID &cross_user_id)
{
	if (!IsCrossUser(cross_user_id)) return NULL;

	CrossUserIDCrossUserMapIt it = m_cross_user_id_user_map.find(cross_user_id);
	if (it != m_cross_user_id_user_map.end())
	{
		return it->second;
	}

	return NULL;
}

void CrossUserManager::OnCrossUserLogin(crossgameprotocal::GameCrossUserLogin *gcul)
{
	// printf("[DEBUG] OnCrossUserLogin %d original(plat:%d uid:%d)\n", gcul->uid, gcul->original_plat_type, gcul->original_uid);

	CrossUser *cross_user = new CrossUser(UniqueUserID(gcul->original_plat_type, IntToUid(gcul->original_uid)), gcul->original_user_name, IntToUid(gcul->uid));
	m_uniqe_user_id_user_map[cross_user->GetOriginalUniqueUserID()] = cross_user;
	m_cross_user_id_user_map[cross_user->GetCrossUserID()] = cross_user;

	m_cross_server->GetCrossActivityManager()->OnCrossUserLogin(cross_user);

	{
		static crossgameprotocal::CrossGameSyncCrossUser cgscu;
		cgscu.sync_type = crossgameprotocal::CrossGameSyncCrossUser::SYNC_CROSS_USER_ADD;
		cgscu.cross_type = CrossConfig::Instance().GetHiddenServerCrossType();
		cgscu.count = 1;
		cgscu.in_cross_user_list[0].uid = UidToInt(cross_user->GetOriginalUserID());
		cgscu.in_cross_user_list[0].cross_activity_type = cross_user->GetCrossActivityType();

		int send_len = sizeof(crossgameprotocal::CrossGameSyncCrossUser) - (crossgameprotocal::CrossGameSyncCrossUser::MAX_IN_CROSS_USER_NUM - cgscu.count) * sizeof(cgscu.in_cross_user_list[0]);
		CrossServer::GetInstCrossServer()->SendToGameWorld(cross_user->GetOriginalUniqueServerID(), (const char *)&cgscu, send_len);
	}
}

void CrossUserManager::OnCrossUserLogout(const UserID &cross_user_id, bool is_hidden_global_disconnect)
{
	// printf("[DEBUG] OnCrossUserLogout %d\n", UidToInt(cross_user_id));

	CrossUser *cross_user = this->GetCrossUser(cross_user_id);
	if (NULL != cross_user)
	{
		m_cross_server->GetCrossActivityManager()->OnCrossUserLogout(cross_user);

		{
			static crossgameprotocal::CrossGameSyncCrossUser cgscu;
			cgscu.sync_type = crossgameprotocal::CrossGameSyncCrossUser::SYNC_CROSS_USER_REMOVE;
			cgscu.cross_type = CrossConfig::Instance().GetHiddenServerCrossType();
			cgscu.count = 1;
			cgscu.in_cross_user_list[0].uid = UidToInt(cross_user->GetOriginalUserID());
			cgscu.in_cross_user_list[0].cross_activity_type = cross_user->GetCrossActivityType();

			int send_len = sizeof(crossgameprotocal::CrossGameSyncCrossUser) - (crossgameprotocal::CrossGameSyncCrossUser::MAX_IN_CROSS_USER_NUM - cgscu.count) * sizeof(cgscu.in_cross_user_list[0]);
			CrossServer::GetInstCrossServer()->SendToGameWorld(cross_user->GetOriginalUniqueServerID(), (const char *)&cgscu, send_len);
		}

		if (!is_hidden_global_disconnect)
		{
			m_uniqe_user_id_user_map.erase(cross_user->GetOriginalUniqueUserID());
			m_cross_user_id_user_map.erase(cross_user->GetCrossUserID());
			delete cross_user;
		}
	}
}

void CrossUserManager::OnHiddenGlobalDisconnect()
{
	for (CrossUserIDCrossUserMapIt it = m_cross_user_id_user_map.begin(); it != m_cross_user_id_user_map.end(); ++ it)
	{
		this->OnCrossUserLogout(it->first, true);
	}

	this->Release();
}

void CrossUserManager::OnRegisterGameServer(NetID global_netid, crossgameprotocal::GameCrossRegisterGameServer *gcrgs)
{
	static crossgameprotocal::CrossGameSyncCrossUser cgscu;
	cgscu.sync_type = crossgameprotocal::CrossGameSyncCrossUser::SYNC_CROSS_USER_INIT;
	cgscu.cross_type = CrossConfig::Instance().GetHiddenServerCrossType();
	cgscu.count = 0;

	for (OriginalUUIDCrossUserMapIt it = m_uniqe_user_id_user_map.begin(), end = m_uniqe_user_id_user_map.end(); it != end; ++ it)
	{
		CrossUser *cross_user = it->second;
		if (NULL != cross_user)
		{
			if (cross_user->GetOriginalPlatType() == gcrgs->plat_type)
			{
				bool is_this_server_user = false;

				for (int i = 0; i < gcrgs->count && i < crossgameprotocal::GameCrossRegisterGameServer::MAX_SERVER_ID_NUM; ++ i)
				{
					if (cross_user->GetOriginalServerID() == gcrgs->server_id_list[i])
					{
						is_this_server_user = true;
						break;
					}
				}

				if (is_this_server_user)
				{
					cgscu.in_cross_user_list[cgscu.count].uid = UidToInt(cross_user->GetOriginalUserID());
					cgscu.in_cross_user_list[cgscu.count].cross_activity_type = cross_user->GetCrossActivityType();

					++ cgscu.count;
					if (cgscu.count >= crossgameprotocal::CrossGameSyncCrossUser::MAX_IN_CROSS_USER_NUM)
					{
						break;
					}
				}
			}
		}
	}

	int send_len = sizeof(cgscu) - (crossgameprotocal::CrossGameSyncCrossUser::MAX_IN_CROSS_USER_NUM - cgscu.count) * sizeof(cgscu.in_cross_user_list[0]);
	InternalComm::Instance().NetSend(global_netid, (const char *)&cgscu, send_len);
}

void CrossUserManager::OnKickCrossUser(const UniqueUserID &original_unique_user_id)
{
	CrossUser* cross_user = this->GetCrossUser(original_unique_user_id);
	crossgameprotocal::CrossGameKickUser cgku;
	cgku.bekick_uid = 0;
	cgku.bekick_uuid = 0;
	if (NULL != cross_user)
	{
		cgku.bekick_uid = UidToInt(cross_user->GetCrossUserID());
		CrossServer::GetInstCrossServer()->SendToHiddenGameWorld((const char*)& cgku, sizeof(cgku));
	}
	else
	{
		cgku.bekick_uuid = UniqueUserIDToLongLong(original_unique_user_id);
		CrossServer::GetInstCrossServer()->SendToGameWorld(original_unique_user_id, (const char*)& cgku, sizeof(cgku));
	}
}

