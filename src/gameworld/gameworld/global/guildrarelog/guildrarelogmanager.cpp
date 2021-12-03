#include "guildrarelogmanager.hpp"
#include "config/guildconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "gameworld/protocal/msgother.h"
#include "obj/character/role.h"
#include "gameworld/global/guild/guild.hpp"
#include "gameworld/global/guild/guildmanager.hpp"
#include "gameworld/world.h"
#include "gameworld/internalcomm.h"
#include "servercommon/serverconfig/localconfig.hpp"

GuildRareLogManager::GuildRareLogManager()
{
}

GuildRareLogManager::~GuildRareLogManager()
{
}

GuildRareLogManager & GuildRareLogManager::Instance()
{
	static GuildRareLogManager manager;
	return manager;
}

void GuildRareLogManager::AddLog(GuildID guild_id, int uid, const GameName role_name, ItemID item_id, int item_num, int scene_id, int monster_id)
{
	Guild *guild = GuildManager::Instance().GetGuild(guild_id);
	if (NULL != guild)
	{
		if (LOGIC_CONFIG->GetGuildConfig().CheckItemRare(item_id))
		{
			GuildRareLogItem temp_item;
			temp_item.item_id = item_id;
			temp_item.item_num = item_num;
			temp_item.monster_id = monster_id;
			temp_item.scene_id = scene_id;
			//temp_item.uid = uid;
			UserID target_userid = IntToUid(uid);
			int plat_type = LocalConfig::Instance().GetPlatType();
			UniqueUserID unique_userid(plat_type, target_userid);
			temp_item.uuid = UniqueUserIDToLongLong(unique_userid);;

			if (role_name != NULL)
			{
				strcpy(temp_item.role_name, role_name);
			}
			temp_item.timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			auto it_log_map = m_log_map.find(guild_id);
			if (m_log_map.end() != it_log_map)
			{
				if (it_log_map->second.size() >= GUILD_RARE_LOG_MAX_NUM)
				{
					it_log_map->second.pop_front();
					it_log_map->second.emplace_back(temp_item);

					this->SendOneLog(guild_id, temp_item);
				}
				else
				{
					it_log_map->second.emplace_back(temp_item);

					this->SendOneLog(guild_id, temp_item);
				}
			}
			else
			{
				m_log_map[guild_id].emplace_back(temp_item);

				this->SendOneLog(guild_id, temp_item);
			}
		}
	}
}

void GuildRareLogManager::AddLog(GuildID guild_id, GuildRareLogItem item)
{
	Guild *guild = GuildManager::Instance().GetGuild(guild_id);
	if (NULL != guild)
	{
		if (LOGIC_CONFIG->GetGuildConfig().CheckItemRare(item.item_id))
		{
			auto it_log_map = m_log_map.find(guild_id);
			if (m_log_map.end() != it_log_map)
			{
				if (it_log_map->second.size() >= GUILD_RARE_LOG_MAX_NUM)
				{
					it_log_map->second.pop_front();
					it_log_map->second.emplace_back(item);

					this->SendOneLog(guild_id, item);
				}
				else
				{
					it_log_map->second.emplace_back(item);

					this->SendOneLog(guild_id, item);
				}
			}
			else
			{
				m_log_map[guild_id].emplace_back(item);

				this->SendOneLog(guild_id, item);
			}
		}
	}
}

GuildRareLogManager::GuildRareLogItemDeque * GuildRareLogManager::GetLog(GuildID guild_id)
{
	auto it_idx = m_log_map.find(guild_id);
	if (m_log_map.end() != it_idx)
	{
		return &m_log_map[guild_id];
	}

	return nullptr;
}

void GuildRareLogManager::SendAllLog(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	GuildRareLogItemDeque *log_deque = this->GetLog(role->GetGuildID());

	Protocol::SCGuildRareLogRet msg;
	msg.count = 0;

	if (nullptr != log_deque)
	{
		for (auto &log_item : *log_deque)
		{
			if (msg.count >= GUILD_RARE_LOG_MAX_NUM)
			{
				break;
			}
			msg.log_item[msg.count] = log_item;
			msg.count++;
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&msg, sizeof(Protocol::SCGuildRareLogRet));

}

void GuildRareLogManager::SendOneLog(GuildID guild_id, GuildRareLogItem temp_item)
{
	Guild *guild = GuildManager::Instance().GetGuild(guild_id);
	if (NULL != guild)
	{
		int member_uid_list[MAX_MEMBER_COUNT];
		memset(member_uid_list, 0, sizeof(member_uid_list));
		int member_total_count = guild->GetGuildMemberManager().GetMemberList(member_uid_list);

		for (int i = 0; i < member_total_count; i++)
		{
			Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(member_uid_list[i]));
			if (role != NULL)
			{
				Protocol::SCGuildRareLogRet msg;
				msg.count = 0;
				msg.log_item[msg.count++] = temp_item;

				EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&msg, sizeof(Protocol::SCGuildRareLogRet));
			}
			//else
			//{
				//同步给隐藏服的仙盟玩家
				//UserID target_userid = IntToUid(member_uid_list[i]);
				//int plat_type = LocalConfig::Instance().GetPlatType();
				//UniqueUserID to_unique_userid(plat_type, target_userid);
				//long long target_uuid = UniqueUserIDToLongLong(to_unique_userid);

				//std::map<long long, CrossUserInfo> &cross_user_map = World::GetInstWorld()->GetSceneManager()->GetCrossUserMap();
				//if (cross_user_map.find(target_uuid) != cross_user_map.end())
				//{
				//	crossgameprotocal::BroadcastGuildRareInfo info;

				//	info.guild_id = guild_id;
				//	info.target_uuid = target_uuid;
				//	info.item.uid = temp_item.uid;
				//	if (temp_item.role_name != NULL)
				//	{
				//		strcpy(info.item.role_name, temp_item.role_name);
				//	}
				//	info.item.item_id = temp_item.item_id;
				//	info.item.item_num = temp_item.item_num;
				//	info.item.timestamp = temp_item.timestamp;
				//	info.item.scene_id = temp_item.scene_id;
				//	info.item.monster_id = temp_item.monster_id;
				//	memcpy(info.item.xianpin_type_list, temp_item.xianpin_type_list, sizeof(info.item.xianpin_type_list));

				//	InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_HIDDEN, 0, (const char *)&info, sizeof(info));
				//	//InternalComm::Instance().UserProtocalTransferToGame(target_uuid, (const char *)&info, sizeof(info));
				//}
			//}
		}

		{
			crossgameprotocal::BroadcastGuildRareInfo info;

			info.plat_type = LocalConfig::Instance().GetPlatType();
			info.guild_id = guild_id;
			info.target_uuid = 0;

			//info.item.uid = temp_item.uid;
			info.item.uuid = temp_item.uuid;
			if (temp_item.role_name != NULL)
			{
				strcpy(info.item.role_name, temp_item.role_name);
			}
			info.item.item_id = temp_item.item_id;
			info.item.item_num = temp_item.item_num;
			info.item.is_from_gift = temp_item.is_from_gift;
			info.item.gift_item_id = temp_item.gift_item_id;
			info.item.timestamp = temp_item.timestamp;
			info.item.scene_id = temp_item.scene_id;
			info.item.monster_id = temp_item.monster_id;
			memcpy(info.item.xianpin_type_list, temp_item.xianpin_type_list, sizeof(info.item.xianpin_type_list));
			InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_HIDDEN, 0, (const char *)&info, sizeof(info));
		}
	}
}

void GuildRareLogManager::SendOneLogToRole(Role *role, GuildRareLogItem item)
{
	if (role != NULL)
	{
		Protocol::SCGuildRareLogRet msg;
		msg.count = 0;
		msg.log_item[msg.count++] = item;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&msg, sizeof(Protocol::SCGuildRareLogRet));
	}
}