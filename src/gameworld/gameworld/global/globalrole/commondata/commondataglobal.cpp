#include "commondataglobal.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/serverconfig/localconfig.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "gameworld/gameworld/internalcomm.h"
#include "gameworld/gameworld/protocal/msgfriendgift.hpp"
#include "gameworld/gameworld/config/logicconfigmanager.hpp"
#include "obj/character/role.h"
#include "other/event/eventhandler.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "other/mount/mountmanager.hpp"
#include "global/challengefield/challengefieldconfig.hpp"
#include "gamelog.h"

CommonDataGlobal::CommonDataGlobal(GlobalRole *g_role) : IGlobalRoleItem(g_role)
{
}

CommonDataGlobal::~CommonDataGlobal()
{
}

void CommonDataGlobal::Init(const CommonDataGlobalParam & attr_data)
{
	m_param = attr_data;

	this->InitGiveItemRecord();
}

void CommonDataGlobal::GetAttr(CommonDataGlobalParam & attr_data)
{
	this->SaveGiveItemRecord();
	attr_data = m_param;
}

void CommonDataGlobal::OnRoleLogin(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	role->SyncChengZhuSkill(m_param.is_chengzhu);

	this->CheckAddCombineKillBossCount();

	// 触发跨服消耗的元宝
	if (m_param.sys_consume_gold_in_cross > 0)
	{
		EventHandler::Instance().OnConsumeGold(role, m_param.sys_consume_gold_in_cross);

		m_param.sys_consume_gold_in_cross = 0;
	}

	//跨服竞技场
	{
		CheckSyncCrossChallengefieldRankMountId(true);
		CheckSyncCrossChallengefieldRankTitleId(true);
	}
}

void CommonDataGlobal::OnRoleLogout(Role *role)
{

}

void CommonDataGlobal::AddUseGoldOnCross(int comsume_gold)
{
	m_param.sys_consume_gold_in_cross += comsume_gold;
}

void CommonDataGlobal::AddCombineKillBoss(int count)
{
	if (count <= 0)
	{
		return;
	}

	m_param.combine_kill_boss_count += count;
}

void CommonDataGlobal::CheckAddCombineKillBossCount()
{
	Role *role = GetRole();
	if (nullptr != role)
	{
		role->GetRoleActivity()->OnSyncCombineKillBossCount(m_param.combine_kill_boss_count);
	}

	m_param.combine_kill_boss_count = 0;
}

void CommonDataGlobal::SetChengzhu()
{
	m_param.is_chengzhu = 1;

	this->SyncChengzhu();
}

void CommonDataGlobal::ClearChengzhu()
{
	m_param.is_chengzhu = 0;

	this->SyncChengzhu();
}

void CommonDataGlobal::SyncChengzhu()
{
	Role * role = this->GetRole();
	if (nullptr != role)
	{
		role->SyncChengZhuSkill(m_param.is_chengzhu);
	}
	else
	{
		this->SyncChengzhuToCloss();
	}
}

void CommonDataGlobal::SyncChengzhuToCloss()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

// 	int PLAT_TYPE = LocalConfig::Instance().GetPlatType();
// 	long long uuid = UniqueUserIDToLongLong(UniqueUserID(PLAT_TYPE, IntToUid(m_role_id)));

	crossgameprotocal::GameCrossWorldCommonDataChange cross_msg;

	cross_msg.uuid = 0;
	cross_msg.change_type = crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_CHENGZHU_CHANGE;
	cross_msg.param_2 = m_param.is_chengzhu;

	InternalComm::Instance().SendToCross((const char*)&cross_msg, sizeof(cross_msg));
}

void CommonDataGlobal::InitGiveItemRecord()
{
	m_give_item_list.clear();
	m_recieve_item_list.clear();

	for (unsigned int i = 0; i < FRIEND_ITEM_GIFT_RECORD_MAX_COUNT && i < static_array_size(m_param.gift_record_list); i++)
	{
		// 我送的
		if (m_param.gift_record_list[i].give_gift.uid > 0)
		{
			m_give_item_list.push_back(m_param.gift_record_list[i].give_gift);
		}
			
		// 送我的
		if (m_param.gift_record_list[i].recieve_gift.uid > 0)
		{
			m_recieve_item_list.push_back(m_param.gift_record_list[i].recieve_gift);
		}
	}
}

void CommonDataGlobal::SaveGiveItemRecord()
{
	memset(m_param.gift_record_list, 0, sizeof(m_param.gift_record_list));

	// 更新我赠送的
	int i = 0;
	for (auto& item : m_give_item_list)
	{
		if (i >= FRIEND_ITEM_GIFT_RECORD_MAX_COUNT)
		{
			break;
		}
		m_param.gift_record_list[i].give_gift = item;
		i++;
	}

	// 更新赠送我的
	i = 0;
	for (auto& item : m_recieve_item_list)
	{
		if (i >= FRIEND_ITEM_GIFT_RECORD_MAX_COUNT)
		{
			break;
		}
		m_param.gift_record_list[i].recieve_gift = item;
		i++;
	}
}

void CommonDataGlobal::AddGiveItemRecord(const ItemGiftInfo &info, bool is_give)
{
	if (is_give)
	{
		if (m_give_item_list.size() >= FRIEND_ITEM_GIFT_RECORD_MAX_COUNT)
		{
			m_give_item_list.pop_front();
		}

		m_give_item_list.push_back(info);
	}
	else
	{
		if (m_recieve_item_list.size() >= FRIEND_ITEM_GIFT_RECORD_MAX_COUNT)
		{
			m_recieve_item_list.pop_front();
		}

		m_recieve_item_list.push_back(info);
	}
}

void CommonDataGlobal::SendGiveItemRecord(Role *role, bool is_give)
{
	if (NULL == role) return;

	Protocol::SCGiveItemRecord gir;
	gir.is_give = is_give ? 1 : 0;
	gir.record_count = 0;

	if (is_give)
	{
		for (auto& record_item : m_give_item_list)
		{
			ItemGiftInfo& msg_item = gir.record_list[gir.record_count];
			msg_item.uid = record_item.uid;
			F_STRNCPY(msg_item.role_name, record_item.role_name, sizeof(msg_item.role_name));
			msg_item.item_id = record_item.item_id;
			msg_item.item_num = record_item.item_num;
			msg_item.timestamp = record_item.timestamp;

			gir.record_count++;
			if (gir.record_count >= FRIEND_ITEM_GIFT_RECORD_MAX_COUNT)
			{
				break;
			}
		}
	}
	else
	{
		for (auto& record_item : m_recieve_item_list)
		{
			ItemGiftInfo& msg_item = gir.record_list[gir.record_count];
			msg_item.uid = record_item.uid;
			F_STRNCPY(msg_item.role_name, record_item.role_name, sizeof(msg_item.role_name));
			msg_item.item_id = record_item.item_id;
			msg_item.item_num = record_item.item_num;
			msg_item.timestamp = record_item.timestamp;

			gir.record_count++;
			if (gir.record_count >= FRIEND_ITEM_GIFT_RECORD_MAX_COUNT)
			{
				break;
			}
		}
	}

	int send_len = sizeof(gir) - (FRIEND_ITEM_GIFT_RECORD_MAX_COUNT - gir.record_count) * sizeof(gir.record_list[0]);

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&gir, send_len);
}

void CommonDataGlobal::CheckReMoveInvalidId()
{
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	
	if(now >= m_param.nex_clear_id_time && m_param.nex_clear_id_time > 0)
	{
		CheckSyncCrossChallengefieldRankMountId(false);
		CheckSyncCrossChallengefieldRankTitleId(false);
	}
}

void CommonDataGlobal::CalcNextClearTimes()
{
//	int reward_week_day = LOGIC_CONFIG->GetChallengeFieldCfg().GetCrossRewardDay();
	int interval = SECOND_PER_DAY * 7;

	m_param.nex_clear_id_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + interval;
}

void CommonDataGlobal::AddCrossChallengefieldRankImageId(short rank, short mount_id, short title_id)
{
	if (rank <= 0 && mount_id <= 0 && title_id <= 0)
	{
		return;
	}

	m_param.cross_challengefield_rank = rank;


	//坐骑
	{
		if (m_param.cross_challengefield_rank_mount_id > 0)
		{
			m_param.cross_challengefield_rank_last_mount_id = m_param.cross_challengefield_rank_mount_id;
		}

		m_param.cross_challengefield_rank_mount_id = mount_id;

		CheckSyncCrossChallengefieldRankMountId(true);
	}

	

	// 称号
	{
		if (m_param.cross_challengefield_rank_title_id > 0)
		{
			m_param.cross_challengefield_rank_last_title_id = m_param.cross_challengefield_rank_title_id;
		}

		m_param.cross_challengefield_rank_title_id = title_id;

		CheckSyncCrossChallengefieldRankTitleId(true);
	}

	this->CalcNextClearTimes();

	gamelog::g_log_global_role.printf(LL_INFO, "CommonDataGlobal::AddCrossChallengefieldRankImageId role(%d), rank[%d], mount_id[%d], title_id[%d], invalid_time[%u]",
		m_role_id, rank, mount_id, title_id, m_param.nex_clear_id_time);
}

short CommonDataGlobal::GetCrossChallengefieldRankMountId()
{
	return m_param.cross_challengefield_rank_mount_id;
}

void CommonDataGlobal::CheckSyncCrossChallengefieldRankMountId(bool is_add)
{
	Role *role = GetRole();
	if (nullptr == role)
	{
		return;
	}

	if (is_add)
	{
		if (m_param.cross_challengefield_rank_last_mount_id == 0 && m_param.cross_challengefield_rank_mount_id == 0)
		{
			return;
		}

		if (m_param.cross_challengefield_rank_last_mount_id == m_param.cross_challengefield_rank_mount_id)
		{
			EventHandler::Instance().OnGetSceneActivityReward(role, CROSS_ACTIVITY_TYPE_CROSS_CHALLENGEFIELD, m_param.cross_challengefield_rank_mount_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_MOUNT, m_param.cross_challengefield_rank);
			m_param.cross_challengefield_rank_last_mount_id = 0;
			return;
		}

		if (m_param.cross_challengefield_rank_last_mount_id > 0)
		{
			role->GetMountManager()->UnActiveSpecialImage(m_param.cross_challengefield_rank_last_mount_id);

			gamelog::g_log_global_role.printf(LL_INFO, "CommonDataGlobal::CheckSyncCrossChallengefieldRankMountId:remove role(%d %s), mount_id[%d]",
				m_role_id, role->GetName(), m_param.cross_challengefield_rank_last_mount_id);
		}

		if (m_param.cross_challengefield_rank_mount_id > 0)
		{
			bool is_active = role->GetMountManager()->IsActiveSpecialImage(m_param.cross_challengefield_rank_mount_id);
			if (!is_active)
			{
				role->GetMountManager()->ActiveSpecialImage(m_param.cross_challengefield_rank_mount_id, false);

				EventHandler::Instance().OnGetSceneActivityReward(role, CROSS_ACTIVITY_TYPE_CROSS_CHALLENGEFIELD, m_param.cross_challengefield_rank_mount_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_MOUNT, m_param.cross_challengefield_rank);

				gamelog::g_log_global_role.printf(LL_INFO, "CommonDataGlobal::CheckSyncCrossChallengefieldRankMountId:add role(%d %s), mount_id[%d]",
					m_role_id, role->GetName(), m_param.cross_challengefield_rank_mount_id);
			}
		}
	}
	else
	{
		if (m_param.cross_challengefield_rank_mount_id > 0)
		{
			role->GetMountManager()->UnActiveSpecialImage(m_param.cross_challengefield_rank_mount_id);

			gamelog::g_log_global_role.printf(LL_INFO, "CommonDataGlobal::CheckSyncCrossChallengefieldRankMountId:remove role(%d %s), mount_id[%d]",
				m_role_id, role->GetName(), m_param.cross_challengefield_rank_mount_id);
		}

		m_param.cross_challengefield_rank_mount_id = 0;
		m_param.cross_challengefield_rank_last_mount_id = 0;
	}
}

short CommonDataGlobal::GetCrossChallengefieldRankTitleId()
{
	return m_param.cross_challengefield_rank_title_id;
}

void CommonDataGlobal::CheckSyncCrossChallengefieldRankTitleId(bool is_add)
{
	Role *role = GetRole();
	if (nullptr == role)
	{
		return;
	}

	if (is_add)
	{
		if (m_param.cross_challengefield_rank_last_title_id == 0 && m_param.cross_challengefield_rank_title_id == 0)
		{
			return;
		}

		if (m_param.cross_challengefield_rank_last_title_id == m_param.cross_challengefield_rank_title_id)
		{
			EventHandler::Instance().OnGetSceneActivityReward(role, CROSS_ACTIVITY_TYPE_CROSS_CHALLENGEFIELD, m_param.cross_challengefield_rank_title_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE, m_param.cross_challengefield_rank);
			m_param.cross_challengefield_rank_last_title_id = 0;
			return;
		}

		if (m_param.cross_challengefield_rank_last_title_id > 0)
		{
			role->GetTitleManager()->RemoveTitle(m_param.cross_challengefield_rank_last_title_id, true, true, true);

			gamelog::g_log_global_role.printf(LL_INFO, "CommonDataGlobal::CheckSyncCrossChallengefieldRankTitleId:remove role(%d %s), title_id[%d]",
				m_role_id, role->GetName(), m_param.cross_challengefield_rank_last_title_id);
		}

		if (m_param.cross_challengefield_rank_title_id > 0)
		{
			if (!role->GetTitleManager()->HasOtherTitle(m_param.cross_challengefield_rank_title_id))
			{
				role->GetTitleManager()->AddTitle(m_param.cross_challengefield_rank_title_id, true, true, true, true);
				EventHandler::Instance().OnGetSceneActivityReward(role, CROSS_ACTIVITY_TYPE_CROSS_CHALLENGEFIELD, m_param.cross_challengefield_rank_title_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE, m_param.cross_challengefield_rank);

				gamelog::g_log_global_role.printf(LL_INFO, "CommonDataGlobal::CheckSyncCrossChallengefieldRankTitleId:add role(%d %s), title_id[%d]",
					m_role_id, role->GetName(), m_param.cross_challengefield_rank_title_id);
			}
		}
	}
	else
	{
		if (m_param.cross_challengefield_rank_title_id > 0)
		{
			role->GetTitleManager()->RemoveTitle(m_param.cross_challengefield_rank_title_id, true, true, true);

			gamelog::g_log_global_role.printf(LL_INFO, "CommonDataGlobal::CheckSyncCrossChallengefieldRankTitleId:remove role(%d %s), title_id[%d]",
				m_role_id, role->GetName(), m_param.cross_challengefield_rank_title_id);
		}
		
		m_param.cross_challengefield_rank_title_id = 0;
		m_param.cross_challengefield_rank_last_title_id = 0;
	}
}

