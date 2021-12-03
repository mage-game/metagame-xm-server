#include "p2ptrademanager.h"
#include "p2ptrade.h"
#include "servercommon/errornum.h"
#include "obj/character/role.h"
#include "world.h"
#include "protocal/msgtrade.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "gamelog.h"
#include "other/p2ptrade/traderegister.h"

P2PTradeManager & P2PTradeManager::Instance()
{
	static P2PTradeManager _instance;
	return _instance;
}

P2PTradeManager::P2PTradeManager() : m_p2ptrade_seq(0)
{
	
}

P2PTradeManager::~P2PTradeManager()
{
	
}

void P2PTradeManager::OnReqTrade(Role *role, int target_uid)
{
	if (NULL == role)
	{
		return;
	}

	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(target_uid));
	if (NULL == target_role)
	{
		role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	if (TS_NONE != target_role->GetTradeRegister()->GetTradeState())
	{
		role->NoticeNum(noticenum::NT_TRADE_OTHER_IS_TRAING);
		return;
	}

	if (role->GetLevel() < TRADE_LEVEL_LIMIT || target_role->GetLevel() < TRADE_LEVEL_LIMIT)
	{
		role->NoticeNum(errornum::EN_TRADE_LEVEL_LIMIT);
		return;
	}

	if (!role->IsInCanTradingTime())
	{
		role->NoticeNum(errornum::EN_TRADE_LOGIN_TIME_LIMIT);
		return;
	}

	if (!target_role->IsInCanTradingTime())
	{
		role->NoticeNum(errornum::EN_BETRADE_LOGIN_TIME_LIMIT);
		return;
	}

	Protocol::SCReqTradeRoute rtr;
	rtr.req_uid = UidToInt(role->GetUserId());
	role->GetName(rtr.req_name);
	rtr.level = role->GetLevel();
	rtr.sex = role->GetSex();
	rtr.prof = role->GetProf();
	rtr.camp = role->GetCamp();
	rtr.reserved = 0;
	rtr.avatar_timestamp = role->GetAvatarTimeStamp();
	EngineAdapter::Instance().NetSend(target_role->GetNetId(), (const char *)&rtr, sizeof(rtr));

	role->NoticeNum(noticenum::NT_READE_REQ_SUCC);
}

void P2PTradeManager::OnReqTradeAgree(Role *role, int target_uid)
{
	if (NULL == role || TS_NONE != role->GetTradeRegister()->GetTradeState())
	{
		role->NoticeNum(noticenum::NT_TRADE_OTHER_IS_TRAING);
		return;
	}

	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(target_uid));
	if (NULL == target_role)
	{
		role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	if (TS_NONE != target_role->GetTradeRegister()->GetTradeState())
	{
		role->NoticeNum(noticenum::NT_TRADE_OTHER_IS_TRAING);
		return;
	}

	if (role->GetLevel() < TRADE_LEVEL_LIMIT || target_role->GetLevel() < TRADE_LEVEL_LIMIT)
	{
		role->NoticeNum(errornum::EN_TRADE_LEVEL_LIMIT);
		return;
	}

	if (!role->IsInCanTradingTime())
	{
		role->NoticeNum(errornum::EN_TRADE_LOGIN_TIME_LIMIT);
		return;
	}

	if (!target_role->IsInCanTradingTime())
	{
		role->NoticeNum(errornum::EN_BETRADE_LOGIN_TIME_LIMIT);
		return;
	}

	if (m_p2p_user_trade_map.end() == m_p2p_user_trade_map.find(role->GetUserId()) && m_p2p_user_trade_map.end() == m_p2p_user_trade_map.find(target_role->GetUserId()))
	{
		long long p2ptrade_inc_id = TranslateInt64(static_cast<int>(EngineAdapter::Instance().Time()), ++ m_p2ptrade_seq);
		P2PTrade p2ptrade(p2ptrade_inc_id, role->GetUserId(), target_role->GetUserId());

		m_p2ptrade_map[p2ptrade_inc_id] = p2ptrade;
		m_p2p_user_trade_map[role->GetUserId()] = p2ptrade_inc_id;
		m_p2p_user_trade_map[target_role->GetUserId()] = p2ptrade_inc_id;

		target_role->GetTradeRegister()->BuildTradeRelation(p2ptrade_inc_id, role->GetUserId()); 
		role->GetTradeRegister()->BuildTradeRelation(p2ptrade_inc_id, target_role->GetUserId());

		target_role->GetTradeRegister()->SyncOtherUserInfo();
		role->GetTradeRegister()->SyncOtherUserInfo();

		gamelog::g_log_p2ptrade.printf(LL_INFO, "p2ptrade->trade start[trade_id:%lld] role[uid:%d, role_name:%s], "
			"other_role[uid:%d, role_name:%s]",
			p2ptrade_inc_id, role->GetUID(), role->GetName(), target_role->GetUID(), target_role->GetName());
	}
}

bool P2PTradeManager::PutItem(Role *role, int register_index, int knapcack_index, int item_num)
{
	if (NULL == role)
	{
		return false;
	}

	P2PUserTradeMapIt it = m_p2p_user_trade_map.find(role->GetUserId());
	if (it == m_p2p_user_trade_map.end())
	{
		return false;
	}

	return role->GetTradeRegister()->PutItem(register_index, knapcack_index, item_num);
}

bool P2PTradeManager::PutMoney(Role *role, int gold, int coin)
{
	if (gold > 0)  // 屏掉元宝功能
	{
		return false;
	}

	if (NULL == role)
	{
		return false;
	}

	P2PUserTradeMapIt it = m_p2p_user_trade_map.find(role->GetUserId());
	if (it == m_p2p_user_trade_map.end())
	{
		return false;
	}

	return role->GetTradeRegister()->PutMoney(gold, coin); 
}

void P2PTradeManager::OnReqTradeLock(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	Role *other_role = World::GetInstWorld()->GetSceneManager()->GetRole(role->GetTradeRegister()->GetOtherUserID());
	if (NULL == other_role)
	{
		role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	P2PUserTradeMapIt it = m_p2p_user_trade_map.find(role->GetUserId());
	if (it != m_p2p_user_trade_map.end())
	{
		if (role->GetTradeRegister()->Lock())
		{	
			other_role->NoticeNum(noticenum::NT_TRADE_ANOTHER_HAS_LOCK);
			other_role->GetTradeRegister()->SyncOtherUserInfo();

			if (TS_LOCK == role->GetTradeRegister()->GetTradeState())
			{
				UNSTD_STATIC_CHECK(P2P_TRADE_MAX_ITEM_COUNT == LOG_MAX_ITEM_NUM);

				TradeListItem trade_item_list[P2P_TRADE_MAX_ITEM_COUNT];
				int item_num = role->GetTradeRegister()->GetTradeItemList(trade_item_list);

				gamelog::g_log_p2ptrade.printf(LL_INFO, "p2ptrade->lock[trade_id:%lld] role[uid:%d, role_name:%s], "
					"other_role[uid:%d, role_name:%s], gold: %d, "
					"trade_item_list[item_num:%d, item1: [%s], item2: [%s], item3: [%s], item4: [%s], item5: [%s], "
					"item6: [%s], item7: [%s], item8: [%s]]",
					it->second, role->GetUID(), role->GetName(),
					other_role->GetUID(), other_role->GetName(), role->GetTradeRegister()->GetTradeGold(),
					item_num, ItemDataWrapperLog(&trade_item_list[0].item_wrapper, 0), ItemDataWrapperLog(&trade_item_list[1].item_wrapper, 1), 
					ItemDataWrapperLog(&trade_item_list[2].item_wrapper, 2), ItemDataWrapperLog(&trade_item_list[3].item_wrapper, 3), 
					ItemDataWrapperLog(&trade_item_list[4].item_wrapper, 4), ItemDataWrapperLog(&trade_item_list[5].item_wrapper, 5),
					ItemDataWrapperLog(&trade_item_list[6].item_wrapper, 6), ItemDataWrapperLog(&trade_item_list[7].item_wrapper, 7));
			}
		}
	}
}

void P2PTradeManager::OnReqTradeAffirm(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	Role *other_role = World::GetInstWorld()->GetSceneManager()->GetRole(role->GetTradeRegister()->GetOtherUserID());
	if (NULL == other_role)
	{
		role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	P2PUserTradeMapIt it = m_p2p_user_trade_map.find(role->GetUserId());
	if (it == m_p2p_user_trade_map.end())
	{
		return;
	}

	P2PUserTradeMapIt other_it = m_p2p_user_trade_map.find(other_role->GetUserId());
	if (other_it == m_p2p_user_trade_map.end())
	{
		return;
	}

	if (other_it->second != it->second)
	{
		return;
	}

	if (TS_LOCK != role->GetTradeRegister()->GetTradeState())
	{
		role->NoticeNum(errornum::EN_TRADE_LOCK_FIRST);
		return;
	}

	if (TS_LOCK != other_role->GetTradeRegister()->GetTradeState() && TS_AFFIRM != other_role->GetTradeRegister()->GetTradeState())
	{
		role->NoticeNum(errornum::EN_TRADE_WAIT_FOR_ANOTHER_LOCK);
		return;
	}

	if (role->GetTradeRegister()->Affirm())
	{
		if (!other_role->GetTradeRegister()->HasAffirm())
		{
			other_role->NoticeNum(noticenum::NT_TRADE_ANOTHER_HAS_AFFIRM);
		}

		other_role->GetTradeRegister()->SyncOtherUserInfo();
	}

	bool is_trade_succ = false;

	if (TS_AFFIRM == role->GetTradeRegister()->GetTradeState() && TS_AFFIRM == other_role->GetTradeRegister()->GetTradeState())
	{
		// 消耗给予

		bool can_trade = role->GetTradeRegister()->CheckCanTrade(other_role);
		if (can_trade)
		{
			can_trade = other_role->GetTradeRegister()->CheckCanTrade(role);
		}

		if (can_trade)
		{
			bool trade_ret = role->GetTradeRegister()->DoTrade(other_role);
			bool other_trade_ret = other_role->GetTradeRegister()->DoTrade(role);

			if (trade_ret && other_trade_ret)
			{
				role->GetTradeRegister()->SyncOtherUserInfo();
				other_role->GetTradeRegister()->SyncOtherUserInfo();

				role->NoticeNum(noticenum::NT_TRADE_SUCC);
				other_role->NoticeNum(noticenum::NT_TRADE_SUCC);

				is_trade_succ = true;
			}
			else
			{
				// 交易失败，理论上不应该能走到这里
				role->NoticeNum(errornum::EN_TRADE_CHECK_TRADE_FAIL);
				other_role->NoticeNum(errornum::EN_TRADE_CHECK_TRADE_FAIL);

				gamelog::g_log_p2ptrade.printf(LL_ERROR, "p2ptrade->affirm::do trade fail [trade_id:%lld] role[uid:%d, role_name:%s], "
					"other_role[uid:%d, role_name:%s]",
					it->second, role->GetUID(), role->GetName(),
					other_role->GetUID(), other_role->GetName());

				gamelog::g_log_serious_error.printf(LL_CRITICAL, "p2ptrade->affirm::do trade fail [trade_id:%lld] role[uid:%d, role_name:%s], "
					"other_role[uid:%d, role_name:%s]",
					it->second, role->GetUID(), role->GetName(),
					other_role->GetUID(), other_role->GetName());
			}
		}
		else
		{
			// 检查交易失败
			gamelog::g_log_p2ptrade.printf(LL_INFO, "p2ptrade->affirm::check trade fail [trade_id:%lld] role[uid:%d, role_name:%s], "
				"other_role[uid:%d, role_name:%s]",
				it->second, role->GetUID(), role->GetName(),
				other_role->GetUID(), other_role->GetName());
		}

		// 打日志
		gamelog::g_log_p2ptrade.printf(LL_INFO, "p2ptrade->affirm::trade result [trade_id:%lld] [result:%d], "
			"role[uid:%d, role_name:%s], "
			"other_role[uid:%d, role_name:%s]",
			it->second, (is_trade_succ ? 1 : 0), 
			role->GetUID(), role->GetName(),
			other_role->GetUID(), other_role->GetName());

		ROLE_LOG_QUICK6(LOG_TYPE_P2P_TRADE_AFFIRM, role, other_role->GetUID(), (is_trade_succ ? 1 : 0), NULL, NULL);
		ROLE_LOG_QUICK6(LOG_TYPE_P2P_TRADE_AFFIRM, other_role, role->GetUID(), (is_trade_succ ? 1 : 0), NULL, NULL);

		// 无论交易成功与否，交易结束，清理数据
		m_p2ptrade_map.erase(it->second); 
		m_p2p_user_trade_map.erase(role->GetUserId());
		m_p2p_user_trade_map.erase(other_role->GetUserId());
		role->GetTradeRegister()->Clear();
		other_role->GetTradeRegister()->Clear();
	}
}

void P2PTradeManager::OnReqCancel(Role *role, const char *reason)
{
	if (NULL == role)
	{
		return;
	}

	P2PUserTradeMapIt user_trade_it = m_p2p_user_trade_map.find(role->GetUserId());
	if (user_trade_it == m_p2p_user_trade_map.end()) 
	{
		return;	
	}

	long long trade_id = user_trade_it->second;

	P2PTradeMapIt trade_map_it = m_p2ptrade_map.find(trade_id);
	if (trade_map_it != m_p2ptrade_map.end())
	{
		P2PTrade &p2p_trade = trade_map_it->second;
		P2PTradeUser *p2p_user = p2p_trade.GetOtherP2PTradeUser(role->GetUserId());

		if (NULL != p2p_user)
		{
			P2PUserTradeMapIt other_user_trade_it = m_p2p_user_trade_map.find(p2p_user->user_id);
			if (other_user_trade_it != m_p2p_user_trade_map.end()) 
			{
				Role *other_role = World::GetInstWorld()->GetSceneManager()->GetRole(p2p_user->user_id);
				if (NULL != other_role)
				{
					other_role->GetTradeRegister()->OnCancel();
					other_role->GetTradeRegister()->Clear();
					other_role->NoticeNum(errornum::EN_TRADE_FAILED_CANCLE_ANOTHER);
				}
			}

			m_p2p_user_trade_map.erase(p2p_user->user_id);
		}
	}

	m_p2ptrade_map.erase(trade_id);
	m_p2p_user_trade_map.erase(role->GetUserId());
	role->GetTradeRegister()->OnCancel();
	role->GetTradeRegister()->Clear();
	role->NoticeNum(errornum::EN_TRADE_FAILED_CANCLE_SELF);

	if (NULL != reason)
	{
		gamelog::g_log_p2ptrade.printf(LL_INFO, "p2ptrade->trade Cancel[trade_id:%lld], [reason:%s]", trade_id, reason);
	}
	else
	{
		gamelog::g_log_p2ptrade.printf(LL_INFO, "p2ptrade->trade Cancel[trade_id:%lld]", trade_id);
	}
}

void P2PTradeManager::OnUserLeaveScene(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	this->OnReqCancel(role, "UserLeaveScene");
}
