#include "money.h"

#include "protocal/msgmoney.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "internalcomm.h"
#include "knapsack.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "rmibackobjdef.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "gamelog.h"
#include "other/event/eventhandler.hpp"
#include "world.h"

#include "other/route/mailroute.hpp"
#include "other/monitor/monitor.hpp"
#include "other/cross/rolecross.h"
#include "global/droplog/droplogmanager.hpp"

static const int FETCH_ACCOUNTDB_INTERVAL = 300;

Money::Money()
	: m_role(NULL),
	m_gold(0),
	m_gold_bind(0),
	m_coin(0),
	m_coin_bind(0),
	m_last_account_get_time(0),
	m_is_lock(false)
{

}

Money::~Money()
{

}

void Money::Init(Role *role, Int64 gold, Int64 gold_bind, Int64 coin, Int64 coin_bind)
{
	m_role = role;

	m_gold = gold;
	m_gold_bind = gold_bind;
	m_coin = coin;
	m_coin_bind = coin_bind;
}

void Money::Send()
{
	Protocol::SCMoney money;
	money.gold = m_gold;
	money.gold_bind = m_gold_bind;
	money.coin = m_coin;
	money.coin_bind = m_coin_bind;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&money, sizeof(money));
}

bool Money::GoldMoreThan(Int64 consume_gold)
{
	if (this->IsLock())
	{
		m_role->NoticeNum(errornum::EN_MONEY_IS_LOCK);
		return false;
	}

	return m_gold >= consume_gold;
}

bool Money::GoldBindMoreThan(Int64 consume_gold_bind)
{
	if (this->IsLock())
	{
		m_role->NoticeNum(errornum::EN_MONEY_IS_LOCK);
		return false;
	}

	return m_gold_bind >= consume_gold_bind;
}

bool Money::AllGoldMoreThan(Int64 consume_all_gold)
{
	if (this->IsLock())
	{
		m_role->NoticeNum(errornum::EN_MONEY_IS_LOCK);
		return false;
	}

	return (m_gold + m_gold_bind) >= consume_all_gold;
}

bool Money::CoinMoreThan(Int64 consume_coin)
{
	if (this->IsLock())
	{
		m_role->NoticeNum(errornum::EN_MONEY_IS_LOCK);
		return false;
	}

	return m_coin >= consume_coin;
}

bool Money::CoinBindMoreThan(Int64 consume_coin_bind)
{
	if (this->IsLock())
	{
		m_role->NoticeNum(errornum::EN_MONEY_IS_LOCK);
		return false;
	}

	return m_coin_bind >= consume_coin_bind;
}

bool Money::AllCoinMoreThan(Int64 consume_all_coin)
{
	if (this->IsLock())
	{
		m_role->NoticeNum(errornum::EN_MONEY_IS_LOCK);
		return false;
	}

	return (m_coin + m_coin_bind) >= consume_all_coin;
}

bool Money::UseGold(Int64 consume_gold, const char* type, bool is_consume_sys) // is_consume_sys指是否为消费系统消费的元宝 用于消费活动
{
	if (consume_gold <= 0 || m_gold < consume_gold || NULL == type) return false;

	if (this->IsLock())
	{
		m_role->NoticeNum(errornum::EN_MONEY_IS_LOCK);
		return false;
	}

	if (0 == consume_gold) return true;

	m_gold -= consume_gold;

	this->Send();

	m_role->GetMonitor()->AddMonitorNum(MONITOR_TYPE_DAY_GOLD_CONSUME, (int)consume_gold);
	if (is_consume_sys) 
	{
		const bool is_bopen = DropLogManager::Instance().IsBopenBegin();
		if (is_bopen)
		{
			DropLogManager::Instance().SetBopenBegin(false);
		}
		EventHandler::Instance().OnConsumeGold(m_role, consume_gold);
		if (is_bopen && !DropLogManager::Instance().IsBopenBegin())
		{
			DropLogManager::Instance().SetBopenBegin(true);
		}
	}

	gamelog::g_log_moneygold.printf(LL_INFO,"[Money::UseGold Succ] [user[%s %d] [level:%d] type:%s use_gold:%u remain_gold:%u]",
		m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, (unsigned int)consume_gold, (unsigned int)m_gold);

	ROLE_LOG_QUICK6(LOG_TYPE_GOLD_USE, m_role, consume_gold, m_gold, type, NULL);

	if (m_role->IsInCross())
	{
		this->OnUseGoldInCross(false, consume_gold, type, is_consume_sys);
	}

	return true;
}

bool Money::UseGoldBind(Int64 consume_gold_bind, const char* type)
{
	if (consume_gold_bind <= 0 || m_gold_bind < consume_gold_bind || NULL == type) return false;

	if (this->IsLock())
	{
		m_role->NoticeNum(errornum::EN_MONEY_IS_LOCK);
		return false;
	}

	if (0 == consume_gold_bind) return true;

	m_gold_bind -= consume_gold_bind;

	this->Send();

	gamelog::g_log_moneygold.printf(LL_INFO, "[Money::UseGoldBind Succ] [user[%s %d] [level:%d] type:%s use_gold_bind:%u remain_gold_bind:%u]",
		m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, (unsigned int)consume_gold_bind, (unsigned int)m_gold_bind);

	ROLE_LOG_QUICK6(LOG_TYPE_GOLD_BIND_USE, m_role, consume_gold_bind, m_gold_bind, type, NULL);

	if (m_role->IsInCross())
	{
		this->OnUseGoldInCross(false, consume_gold_bind, type, false);
	}

	return true;
}

bool Money::UseCoin(Int64 consume_coin, const char* type)
{
	if (consume_coin <= 0 || m_coin < consume_coin || NULL == type) return false;

	if (this->IsLock())
	{
		m_role->NoticeNum(errornum::EN_MONEY_IS_LOCK);
		return false;
	}

	if (0 == consume_coin) return true;

	m_coin -= consume_coin;

	this->Send();

	gamelog::g_log_moneycoin.printf(LL_INFO, "[Money::UseCoin Succ] [user[%s %d] [level:%d] type:%s use_coin:%u remain_coin:%u]",
		m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, (unsigned int)consume_coin, (unsigned int)m_coin);

	ROLE_LOG_QUICK6(LOG_TYPE_COIN_USE, m_role, consume_coin, m_coin, type, NULL);

	return true;
}

bool Money::UseCoinBind(Int64 consume_coin_bind, const char* type)
{
	if (consume_coin_bind <= 0 || m_coin_bind < consume_coin_bind || NULL == type) return false;

	if (this->IsLock())
	{
		m_role->NoticeNum(errornum::EN_MONEY_IS_LOCK);
		return false;
	}

	if (0 == consume_coin_bind) return true;

	m_coin_bind -= consume_coin_bind;

	this->Send();

	gamelog::g_log_moneycoin.printf(LL_INFO, "[Money::UseCoinBind Succ] [user[%s %d] [level:%d] type:%s use_coin_bind:%u remain_coin_bind:%u]",
		m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, (unsigned int)consume_coin_bind, (unsigned int)m_coin_bind);

	ROLE_LOG_QUICK6(LOG_TYPE_COIN_BIND_USE, m_role, consume_coin_bind, m_coin_bind, type, NULL);

	return true;
}

bool Money::UseAllGold(Int64 consume_all_gold, const char *type, bool bind_first, Int64 *use_gold, Int64 *use_gold_bind, bool is_consume_sys)
{
	if (consume_all_gold <= 0 || NULL == type)
	{
		return false;
	}

	if (this->IsLock())
	{
		m_role->NoticeNum(errornum::EN_MONEY_IS_LOCK);
		return false;
	}

	if ((m_gold + m_gold_bind) < consume_all_gold)
	{
		return false;
	}

	Int64 need_gold = 0, need_gold_bind = 0;
	{
		if (bind_first)
		{
			need_gold_bind = consume_all_gold;
			if (need_gold_bind > m_gold_bind)
			{
				need_gold_bind = m_gold_bind;
				need_gold = consume_all_gold - m_gold_bind;
			}
		}
		else
		{
			need_gold = consume_all_gold;
			if (need_gold > m_gold)
			{
				need_gold = m_gold;
				need_gold_bind = consume_all_gold - m_gold;
			}
		}
	}
	if (need_gold > m_gold || need_gold_bind > m_gold_bind) // 加强判断
	{
		return false;
	}

	m_gold -= need_gold;
	m_gold_bind -= need_gold_bind;

	this->Send();

	if (NULL != use_gold) *use_gold = need_gold;
	if (NULL != use_gold_bind) *use_gold_bind = need_gold_bind;

	gamelog::g_log_moneygold.printf(LL_INFO, "[Money::UseAllGold Succ][user[%s %d] [level:%d] type:%s use_gold:%u remain_gold:%u use_gold_bind:%u remain_gold_bind:%u]",
		m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, 
		(unsigned int)need_gold, (unsigned int)m_gold, (unsigned int)need_gold_bind, (unsigned int )m_gold_bind);

	if (need_gold_bind > 0)
	{
		ROLE_LOG_QUICK6(LOG_TYPE_GOLD_BIND_USE, m_role, need_gold_bind, m_gold_bind, type, NULL);

		gamelog::g_log_moneygold.printf(LL_INFO, "[Money::UseGoldBind Succ][user[%s %d] [level:%d] type:%s use_gold_bind:%u remain_gold_bind:%u]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, (unsigned int)need_gold_bind, (unsigned int)m_gold_bind);

		if (m_role->IsInCross())
		{
			this->OnUseGoldInCross(false, need_gold_bind, type, false);
		}
	}

	if (need_gold > 0)
	{
		if (is_consume_sys) 
		{
			const bool is_bopen = DropLogManager::Instance().IsBopenBegin();
			if (is_bopen)
			{
				DropLogManager::Instance().SetBopenBegin(false);
			}
			EventHandler::Instance().OnConsumeGold(m_role, need_gold);
			if (is_bopen && !DropLogManager::Instance().IsBopenBegin())
			{
				DropLogManager::Instance().SetBopenBegin(true);
			}
		}

		ROLE_LOG_QUICK6(LOG_TYPE_GOLD_USE, m_role, need_gold, m_gold, type, NULL);

		gamelog::g_log_moneygold.printf(LL_INFO,"[Money::UseGold Succ][user[%s %d] [level:%d] type:%s use_gold:%u remain_gold:%u]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, (unsigned int)need_gold, (unsigned int)m_gold);

		if (m_role->IsInCross())
		{
			this->OnUseGoldInCross(false, need_gold, type, is_consume_sys);
		}
	}

	return true;
}

bool Money::UseAllCoin(Int64 consume_all_coin, const char *type, bool bind_first, Int64 *use_coin, Int64 *use_coin_bind )
{
	if (consume_all_coin <= 0 || NULL == type)
	{
		return false;
	}

	if (this->IsLock())
	{
		m_role->NoticeNum(errornum::EN_MONEY_IS_LOCK);
		return false;
	}

	if ((m_coin + m_coin_bind) < consume_all_coin)
	{
		return false;
	}

	Int64 need_coin = 0, need_coin_bind = 0;
	{
		if (bind_first)
		{
			need_coin_bind = consume_all_coin;
			if (need_coin_bind > m_coin_bind)
			{
				need_coin_bind = m_coin_bind;
				need_coin = consume_all_coin - m_coin_bind;
			}
		}
		else
		{
			need_coin = consume_all_coin;
			if (need_coin > m_coin)
			{
				need_coin = m_coin;
				need_coin_bind = consume_all_coin - m_coin;
			}
		}
	}
	if (need_coin > m_coin || need_coin_bind > m_coin_bind) // 加强判断
	{
		return false;
	}

	m_coin -= need_coin;
	m_coin_bind -= need_coin_bind;

	this->Send();

	if (NULL != use_coin) *use_coin = need_coin;
	if (NULL != use_coin_bind) *use_coin_bind = need_coin_bind;

	gamelog::g_log_moneycoin.printf(LL_INFO, "[Money::UseAllCoin Succ][user[%s %d] [level:%d] type:%s use_coin:%u remain_coin:%u use_coin_bind:%u remain_coin_bind:%u]",
		m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, 
		(unsigned int)need_coin, (unsigned int)m_coin, (unsigned int)need_coin_bind, (unsigned int )m_coin_bind);

	if (need_coin_bind > 0)
	{
		ROLE_LOG_QUICK6(LOG_TYPE_COIN_BIND_USE, m_role, need_coin_bind, m_coin_bind, type, NULL);

		gamelog::g_log_moneycoin.printf(LL_INFO, "[Money::UseBindCoin Succ][user[%s %d] [level:%d] type:%s use_coin_bind:%u remain_coin_bind:%u]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, (unsigned int)need_coin_bind, (unsigned int)m_coin_bind);
	}

	if (need_coin > 0)
	{
		ROLE_LOG_QUICK6(LOG_TYPE_COIN_USE, m_role, need_coin, m_coin, type, NULL);

		gamelog::g_log_moneycoin.printf(LL_INFO,"[Money::UseCoin Succ][user[%s %d] [level:%d] type:%s use_coin:%u remain_coin:%u]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, (unsigned int)need_coin, (unsigned int)m_coin);
	}

	return true;
}

bool Money::Add(Int64 gold, Int64 gold_bind, Int64 coin, Int64 coin_bind, const char* type)
{
	if (gold < 0 || gold_bind < 0 || coin < 0 || coin_bind < 0 || NULL == type) return false;
	
	Int64 old_coin = m_coin;
	Int64 old_coin_bind = m_coin_bind;

	m_gold += gold;
	m_gold_bind += gold_bind;
	m_coin += coin;
	m_coin_bind += coin_bind;

	this->Send();

	m_role->GetMonitor()->AddMonitorNum(MONITOR_TYPE_DAY_GOLD_GET, (int)gold);
	EventHandler::Instance().OnAddAllCoin(m_role, old_coin + old_coin_bind, m_coin + m_coin_bind);

	if (gold > 0 || gold_bind > 0)
	{
		gamelog::g_log_moneygold.printf(LL_INFO, "[Money::AddAllGold Succ][user[%s %d] [level:%d] type:%s add_gold:%u add_gold_bind:%u remain_gold:%u remain_gold_bind:%u]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, (unsigned int)gold, (unsigned int)gold_bind, (unsigned int)m_gold, (unsigned int)m_gold_bind);
	}

	if (coin > 0 || coin_bind > 0)
	{
		gamelog::g_log_moneycoin.printf(LL_INFO, "[Money::AddAllCoin Succ][user[%s %d] [level:%d] type:%s add_coin:%u  remain_coin:%u add_coin_bind:%u  remain_coin_bind:%u]",
			m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, (unsigned int)coin, (unsigned int)m_coin, (unsigned int)coin_bind, (unsigned int)m_coin_bind);
	}

	if (gold > 0) 
	{
		ROLE_LOG_QUICK6(LOG_TYPE_GOLD_ADD, m_role, gold, m_gold, type, NULL);
	}
	if (gold_bind > 0) 
	{
		ROLE_LOG_QUICK6(LOG_TYPE_GOLD_BIND_ADD, m_role, gold_bind, m_gold_bind, type, NULL);
	}
	
	if (coin_bind > 0) 
	{
		ROLE_LOG_QUICK6(LOG_TYPE_COIN_BIND_ADD, m_role, coin_bind, m_coin_bind, type, NULL);
	}

	if (coin > 0) 
	{
		ROLE_LOG_QUICK6(LOG_TYPE_COIN_ADD, m_role, coin, m_coin, type, NULL);
	}

	return true;
}

bool Money::AddGold(Int64 gold, const char* type, bool is_chongzhi)
{
	if (gold < 0 || NULL == type) return false;
	if (0 == gold) return true;

	m_gold += gold;

	this->Send();

	if (!is_chongzhi) m_role->GetMonitor()->AddMonitorNum(MONITOR_TYPE_DAY_GOLD_GET, (int)gold);

	gamelog::g_log_moneygold.printf(LL_INFO, "[Money::AddGold Succ][user[%s %d] [level:%d] type:%s add_gold:%u remain_gold:%u]",
		m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, (unsigned int)gold, (unsigned int)m_gold);

	ROLE_LOG_QUICK6(LOG_TYPE_GOLD_ADD, m_role, gold, m_gold, type, NULL);

	return true;
}

bool Money::AddGoldBind(Int64 gold_bind, const char* type)
{
	if (gold_bind < 0 || NULL == type) return false;
	if (0 == gold_bind) return true;

	m_gold_bind += gold_bind;

	this->Send();

	gamelog::g_log_moneygold.printf(LL_INFO, "[Money::AddGoldBind Succ][user[%s %d] [level:%d] type:%s add_gold_bind:%u remain_gold_bind:%u]",
		m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, (unsigned int)gold_bind, (unsigned int)m_gold_bind);

	ROLE_LOG_QUICK6(LOG_TYPE_GOLD_BIND_ADD, m_role, gold_bind, m_gold_bind, type, NULL);

	EventHandler::Instance().OnAddGoldBind(m_role, m_gold_bind);

	return true;
}

bool Money::AddCoin(Int64 coin, const char* type)
{
	if (coin < 0 || NULL == type) return false;
	if (0 == coin) return true;

	Int64 old_coin = m_coin;

	m_coin += coin;

	this->Send();

	gamelog::g_log_moneycoin.printf(LL_INFO, "[Money::AddCoin Succ][user[%s %d] [level:%d] type:%s add_coin:%u remain_coin:%u]",
		m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, (unsigned int)coin, (unsigned int)m_coin);

	ROLE_LOG_QUICK6(LOG_TYPE_COIN_ADD, m_role, coin, m_coin, type, NULL);
	
	EventHandler::Instance().OnAddAllCoin(m_role, old_coin + m_coin_bind, m_coin + m_coin_bind);
	return true;
}

bool Money::AddCoinBind(Int64 coin_bind, const char* type)
{
	if (coin_bind < 0 || NULL == type) return false;
	if (0 == coin_bind) return true;

	Int64 old_coin_bind = m_coin_bind;

	m_coin_bind += coin_bind;

	this->Send();

	gamelog::g_log_moneycoin.printf(LL_INFO, "[Money::AddCoinBind Succ][user[%s %d] [level:%d] type:%s add_coin_bind:%u remain_coin_bind:%u]",
		m_role->GetName(), UidToInt(m_role->GetUserId()), m_role->GetLevel(), type, (unsigned int)coin_bind, (unsigned int)m_coin_bind);

	ROLE_LOG_QUICK6(LOG_TYPE_COIN_BIND_ADD, m_role, coin_bind, m_coin_bind, type, NULL);

	EventHandler::Instance().OnAddAllCoin(m_role, old_coin_bind + m_coin, m_coin_bind + m_coin);

	return true;
}

void RMIGetGoldBackObjectImpl::GetGoldRet(Int64 gold, int authority_type)
{
	if (gold > 0) 
	{
		Role *role = scene_manager->GetRole(user_id);
		if (NULL != role)
		{
			role->GetKnapsack()->GetMoney()->GetAccountGoldToRole(gold, authority_type);
		}
	}
}

void Money::GetAccountGoldFromDB(time_t now_second)
{
	if (now_second > FETCH_ACCOUNTDB_INTERVAL + m_last_account_get_time)
	{
		RMIGetGoldBackObjectImpl *ggboi = new RMIGetGoldBackObjectImpl();

		ggboi->scene_manager = World::GetInstWorld()->GetSceneManager();
		ggboi->user_id = m_role->GetUserId();

		PlatName pname;
		m_role->GetPlatName(pname);

		RMILoginClient lc;
		lc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());
		lc.GetGoldAsyn(pname, m_role->GetUID(), ggboi);

		m_last_account_get_time = now_second;
	}
}

void RMIChangeGoldBackObjectImplErrorAdd::ChangeGoldRet(int result, Int64 new_gold)
{
	if (result != 0)
	{
		Error("result not 0");
	}
}

void RMIChangeGoldBackObjectImplErrorAdd::Error(const char *err_str)
{
	// 记录到Log
	printf("RMIChangeGoldBackObject::Error:%s, user_id[%d, %d] glod_get:%d\n", NULL == err_str ? "" : err_str, user_id.db_index, user_id.role_id, (int)gold_get);
}

void RMIChangeGoldBackObjectImpl::ChangeGoldRet(int result, Int64 new_gold)
{
	gamelog::g_log_moneygold.printf(LL_INFO, "[RMIChangeGoldBackObjectImpl::ChangeGoldRet][result[%d] user[%d] [gold_get:%d] [new_gold:%d]",
		result, UidToInt(user_id), (int)gold_get, (int)new_gold);

	if (0 == result)
	{
		Role *role = scene_manager->GetRole(user_id);
		if (NULL != role && !role->GetRoleCross()->IsTransferToCrossIng())
		{
			// 只有充值获得元宝才触发rmb购买
			if (EventHandler::Instance().OnRMBBuy(role, (int)gold_get))
			{
				// RMB直接购买也触发充值事件
				EventHandler::Instance().OnAddChongzhi(role, gold_get, false);
				return;
			}

			Money *money = role->GetKnapsack()->GetMoney();
			if (money->AddGold(gold_get,"GetAccountGold", true))
			{
				if (1 == authority_type && Role::AUTHORITY_TYPE_INVALID == role->GetAuthority())
				{
					role->SetAuthority(Role::AUTHORITY_TYPE_TEST, false);
				}

				role->Save();	// 迅速保存
				EventHandler::Instance().OnAddChongzhi(role, gold_get, true);
				return;
			}
		}
		
		RMIChangeGoldBackObjectImplErrorAdd *cgboi = new RMIChangeGoldBackObjectImplErrorAdd();  // 把钱加回来
		
		cgboi->gold_get = gold_get;
		cgboi->user_id = user_id;

		RMILoginClient lc;
		lc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());
		bool ret = lc.ChangeGoldAsyn(pname, UidToInt(user_id), -gold_get, cgboi);

		gamelog::g_log_moneygold.printf(LL_INFO, "[RMIChangeGoldBackObjectImplErrorAdd %s] [user[%d] [gold_get:%d] ]",
			ret ? "succ" : "fail", UidToInt(user_id), (int)gold_get);
	}
}

void RMIChangeGoldBackObjectImpl::Error(const char *err_str)
{
	printf("RMIChangeGoldBackObject::Error:%s\n", NULL == err_str ? "" : err_str);  // 记录Log
}

void Money::GetAccountGoldToRole(Int64 gold, int authority_type)
{
	RMIChangeGoldBackObjectImpl *cgboi = new RMIChangeGoldBackObjectImpl();

	cgboi->scene_manager = World::GetInstWorld()->GetSceneManager();
	cgboi->user_id = m_role->GetUserId();
	cgboi->gold_get = gold;
	cgboi->authority_type = authority_type;

	m_role->GetPlatName(cgboi->pname);

	RMILoginClient lc;
	lc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());
	lc.ChangeGoldAsyn(cgboi->pname, m_role->GetUID(), gold, cgboi);
}

void Money::ForceGetGold()
{
	m_last_account_get_time = EngineAdapter::Instance().Time() - FETCH_ACCOUNTDB_INTERVAL;
}

void Money::QuickLogByCross(int log_type, QuickLogParam &param)
{
	if (log_type != LOG_TYPE_GOLD_USE && log_type != LOG_TYPE_GOLD_BIND_USE) return;

	LOG_QUICK8(gamelog::g_log_quick, log_type, param.uid, param.user_name, param.plat_name, param.n1, param.n2, param.type, "ByCross");
}

void Money::OnUseGoldInCross(bool is_bind, long long use_gold, const char *type, bool is_consume_sys)
{
	static crossgameprotocal::UseGoldInHiddenGs ugihs;
	ugihs.is_use_bind = is_bind;
	ugihs.is_consume_sys = is_consume_sys;
	ugihs.uid = UidToInt(m_role->GetRoleCross()->GetCrossOriginalUniqueUserID().user_id);
	m_role->GetName(ugihs.user_name);
	RoleCross::CrossNameToOriginName(ugihs.user_name);
	m_role->GetPlatName(ugihs.plat_name);
	RoleCross::CrossPlatNameToOrigin(ugihs.plat_name);
	ugihs.use_gold = use_gold;
	ugihs.cur_gold = is_bind ? m_gold_bind : m_gold;
	STRNCPY(ugihs.type, type, sizeof(ugihs.type));

	InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_UUID,
		UniqueUserIDToLongLong(m_role->GetRoleCross()->GetCrossOriginalUniqueUserID()),
		(const char*)&ugihs, sizeof(ugihs));
}
