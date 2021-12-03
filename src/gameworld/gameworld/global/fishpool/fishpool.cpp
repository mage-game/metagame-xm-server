#include "fishpool.hpp"
#include "fishpoolmanager.hpp"
#include "obj/character/role.h"

#include "servercommon/string/gameworldstr.h"
#include "servercommon/noticenum.h"
#include "servercommon/errornum.h"
#include "protocal/msgfishpool.h"
#include "world.h"
#include "gamelog.h"

#include "config/logicconfigmanager.hpp"
#include "global/fishpool/fishpoolconfig.hpp"

#include "item/itempool.h"
#include "item/knapsack.h"
#include "servercommon/struct/structcommon.h"
#include "other/runesystem/runesystem.hpp"
#include "other/sublock/sublock.hpp"
#include "task/newtaskmanager.h"
#include "other/magicalprecious/magicalprecious.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"

FishPool::FishPool(FishPoolManager *fish_pool_manager) 
	: m_fish_pool_manger(fish_pool_manager), m_role_id(0)
{

}

FishPool::~FishPool()
{

}

void FishPool::Init(const FishPoolListParam::FishPoolAttr &fish_pool_attr)
{
	m_role_id = fish_pool_attr.role_id;

	m_common_data = fish_pool_attr.common_data;
}

void FishPool::GetAttr(FishPoolListParam::FishPoolAttr *fish_pool_attr)
{
	if (NULL == fish_pool_attr) return;

 	fish_pool_attr->role_id = m_role_id;
 
 	fish_pool_attr->common_data = m_common_data;
}

void FishPool::Update(unsigned long interval, time_t now_second)
{
}

void FishPool::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
 	m_common_data.bullet_buy_times = 0;
 	m_common_data.bullet_buy_num = 0;
 	m_common_data.bullet_consume_num = 0;
	m_common_data.today_fang_fish_times = 0;
	m_common_data.today_buy_fang_fish_tims = 0;
 
 	m_fish_pool_manger->OnFishPoolChange(this, structcommon::CS_UPDATE);
	
 	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role_id));
 	if (NULL != role)
 	{
 		this->SendCommonInfo(role);
 	}
}

void FishPool::OnSyncUserInfo(Role *role)
{
 	if (NULL == role || role->GetUID() != m_role_id) 
 	{
 		return;
 	}
 
 	m_common_data.role_level = role->GetLevel(); 
 	role->GetName(m_common_data.role_name);
 
 	m_fish_pool_manger->OnFishPoolChange(this, structcommon::CS_UPDATE);
}

void FishPool::RaiseFish(Role *role)
{
 	if (NULL == role || role->GetUID() != m_role_id) 
 	{
 		return;
 	}

	if (0 != m_common_data.fang_fish_time)
	{
		return;
	}

	const FishPoolOtherConfig &other_cfg = LOGIC_CONFIG->GetFishPoolCfg().GetOtherCfg();

	if (m_common_data.today_fang_fish_times >= m_common_data.today_buy_fang_fish_tims + other_cfg.today_free_fang_fish_times)
	{
		role->NoticeNum(errornum::EN_FISH_POOL_FANG_FISH_TIMES_LIMIT);
		return;
	}
 
	FishQualityTypeCfg * fish_quality_cfg = LOGIC_CONFIG->GetFishPoolCfg().GetFishQualityCfg(m_common_data.fish_quality);
 	if (NULL == fish_quality_cfg)
 	{
 		return;
 	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}
 
	if (fish_quality_cfg->gold > 0 && !role->GetKnapsack()->GetMoney()->UseGold(fish_quality_cfg->gold, "RaiseFish"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}
 	
 	role->NoticeNum(noticenum::NT_FISHPOOL_RAISE_SUCC);
	role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_FISH_NUM, 1);

	m_common_data.fish_num = fish_quality_cfg->count;
	m_common_data.fang_fish_time = static_cast<int>(EngineAdapter::Instance().Time());
	m_common_data.today_fang_fish_times++;

 	this->SendAllRaiseInfo(role);
 	this->SendCommonInfo(role);
 
	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_FEEDING_FISH);

 	m_fish_pool_manger->OnFishPoolChange(this, structcommon::CS_UPDATE);
 
 	{
 		// 日志
 		gamelog::g_log_fish_pool.printf(LL_INFO, "RaiseFish::user[%d, %s], level[%d], fish_quality[%d], use_gold[%d]",
 			role->GetUID(), role->GetName(), role->GetLevel(), m_common_data.fish_quality, fish_quality_cfg->gold);
 	}
}

void FishPool::HarvestFish(Role *role, bool is_skip)
{
 	if (NULL == role || role->GetUID() != m_role_id)
 	{
 		return;
 	}

	FishQualityTypeCfg * fish_quality_cfg = LOGIC_CONFIG->GetFishPoolCfg().GetFishQualityCfg(m_common_data.fish_quality);
	if (NULL == fish_quality_cfg)
	{
		return;
	}

	if (!is_skip)
	{
		if (0 == m_common_data.fang_fish_time || static_cast<int>(EngineAdapter::Instance().Time()) < m_common_data.fang_fish_time + fish_quality_cfg->need_time)
		{
			return;
		}
	}

	int reward_num = m_common_data.fish_num;
	if (is_skip)
	{
		reward_num = fish_quality_cfg->count;
	}

	if (FISH_QUALITY_COUNT - 1 == m_common_data.fish_quality)
	{
		reward_num = fish_quality_cfg->count;
	}

	long long add_exp = (role->GetLevel() + 50) * fish_quality_cfg->exp * reward_num;
	int add_rune_score = fish_quality_cfg->rune_score * reward_num;

	ItemConfigData reward_item;
	reward_item.item_id = fish_quality_cfg->reward_item.item_id;
	reward_item.num = fish_quality_cfg->reward_item.num * reward_num;
	reward_item.is_bind = fish_quality_cfg->reward_item.is_bind;

	role->GetKnapsack()->PutOrMail(reward_item, PUT_REASON_FISH_POOL_HARVEST);
	role->AddExp(add_exp, "fish_pool_exp", Role::EXP_ADD_REASON_EXP_FISH_POOL);
	role->GetRuneSystem()->AddRuneJingHua(add_rune_score);

	static Protocol::SCFishPoolShouFishRewardInfo fpari;
	fpari.item_id = fish_quality_cfg->reward_item.item_id;
	fpari.item_num = fish_quality_cfg->reward_item.num * reward_num;
	fpari.exp = add_exp;
	fpari.rune_score = add_rune_score;
	fpari.quality = m_common_data.fish_quality;
	if (is_skip)
	{
		fpari.is_skip = 1;
	}
	else
	{
		fpari.is_skip = 0;
	}
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&fpari, sizeof(fpari));

	m_common_data.fang_fish_time = 0;
	m_common_data.fish_quality = 0;
	m_common_data.fish_num = 0;

	this->SendAllRaiseInfo(role);
	this->SendCommonInfo(role);

 	m_fish_pool_manger->OnFishPoolChange(this, structcommon::CS_UPDATE);
 
 	{
 		// 日志
 		gamelog::g_log_fish_pool.printf(LL_INFO, "HarvestFish::user[%d, %s], level[%d], fish_quality[%d], addexp[%lld]",
 			role->GetUID(), role->GetName(), role->GetLevel(), m_common_data.fish_quality, add_exp);
 	}
}

// 偷假玩家的鱼
void FishPool::StealFakeFish(Role *steal_role, int be_steal_uid, char type)
{
	if (NULL == steal_role)
	{
		return;
	}

	if (steal_role->GetUID() != m_role_id)
	{
		return;
	}

	Protocol::FishPoolGeneralInfoItem * fake_pool = this->GetFakePool(be_steal_uid);
	if (NULL == fake_pool) return;

	FishQualityTypeCfg * fish_quality_cfg = LOGIC_CONFIG->GetFishPoolCfg().GetFishQualityCfg(fake_pool->fish_quality);
	if (NULL == fish_quality_cfg)
	{
		return;
	}

	FishPool *stealer_fish_pool = m_fish_pool_manger->GetFishPool(steal_role->GetUID());
	if (NULL == stealer_fish_pool)
	{
		return;
	}

	if (fake_pool->fish_num <= fish_quality_cfg->steal_limit)
	{
		stealer_fish_pool->SendCommonInfo(steal_role);
		this->FishPoolChange(steal_role, be_steal_uid, fake_pool->fish_num, fake_pool->fish_quality, 2);
		return;
	}

	if (!stealer_fish_pool->HasBullet())
	{
		stealer_fish_pool->SendCommonInfo(steal_role);
		this->FishPoolChange(steal_role, be_steal_uid, fake_pool->fish_num, fake_pool->fish_quality, 2);
		steal_role->NoticeNum(errornum::EN_FISH_POOL_LACK_BULLET);
		return;
	}

	stealer_fish_pool->ConsumeBullet();

	steal_role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_CATCH_FISH);

	int is_steal_succ = 0;
	if (2 != type)
	{
		int rand_value = RandomNum(100);
		if (rand_value >= fish_quality_cfg->be_steal_pro || 1 == type)						// 偷鱼概率失败了
		{
			if (0 == type)
			{
				steal_role->NoticeNum(errornum::EN_FISH_POOL_STEAL_FAIL);
			}
		}
		else
		{
			int multiple = 1;
		
			fish_quality_cfg->reward_item.num = fish_quality_cfg->reward_item.num * multiple;
			steal_role->GetKnapsack()->PutOrMail(fish_quality_cfg->reward_item, PUT_REASON_FISH_POOL_HARVEST);
			steal_role->AddExp((steal_role->GetLevel() + 50) * fish_quality_cfg->exp * multiple, "teal_fish_pool_exp", Role::EXP_ADD_REASON_EXP_FISH_POOL);
			steal_role->GetRuneSystem()->AddRuneJingHua(fish_quality_cfg->rune_score * multiple);

			is_steal_succ = 1;

			if (fake_pool->fish_num > 0)
			{
				fake_pool->fish_num--;
			}

			steal_role->NoticeNum(noticenum::NT_FISHPOOL_HARVEST_SUCC);
		}
		this->FishPoolChange(steal_role, be_steal_uid, fake_pool->fish_num, fake_pool->fish_quality, is_steal_succ);
	}

	stealer_fish_pool->SendCommonInfo(steal_role);

	m_fish_pool_manger->OnFishPoolChange(stealer_fish_pool, structcommon::CS_UPDATE);
}

void FishPool::StealFish(Role *steal_role, int be_steal_uid, char type, char quality)
{
 	if (NULL == steal_role)
 	{
 		return;
 	}

	if (steal_role->GetUID() == m_role_id)
	{
		steal_role->NoticeNum(errornum::EN_FISH_POOL_CANNOT_STEAL_SELF);
		return;
	}

	if (m_role_id != be_steal_uid)
	{
		steal_role->NoticeNum(errornum::EN_FISH_POOL_STEAL_ERROR);
		return;
	}

	FishQualityTypeCfg * fish_quality_cfg = LOGIC_CONFIG->GetFishPoolCfg().GetFishQualityCfg(m_common_data.fish_quality);
	if (NULL == fish_quality_cfg)
	{
		return;
	}

	if (m_common_data.fish_num <= fish_quality_cfg->steal_limit || m_common_data.fish_quality != quality)
	{
		this->FishPoolChange(steal_role, be_steal_uid, m_common_data.fish_num, m_common_data.fish_quality, 2);
		steal_role->NoticeNum(errornum::EN_FISH_POOL_IS_CHANGE);
		return;
	}
 
 	FishPool *stealer_fish_pool = m_fish_pool_manger->GetFishPool(steal_role->GetUID());
 	if (NULL == stealer_fish_pool)
 	{
 		return;
 	}
 
 	if (!stealer_fish_pool->HasBullet())
 	{
 		steal_role->NoticeNum(errornum::EN_FISH_POOL_LACK_BULLET);
 		return;
 	}

	int is_steal_succ = 0;
	stealer_fish_pool->ConsumeBullet();

	steal_role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_CATCH_FISH);

	if (2 != type)
	{
		int rand_value = RandomNum(100);
		if (rand_value >= fish_quality_cfg->be_steal_pro || 1 == type)						// 偷鱼概率失败了
		{
			if (0 == type)
			{
				steal_role->NoticeNum(errornum::EN_FISH_POOL_STEAL_FAIL);
			}
		}
		else
		{
			int multiple = 1;
			if (stealer_fish_pool->IsStealUid(m_role_id))
			{
				multiple = 2;
			}
	
			ItemConfigData reward_item;
			reward_item.item_id = fish_quality_cfg->reward_item.item_id;
			reward_item.num = fish_quality_cfg->reward_item.num * multiple;
			reward_item.is_bind = fish_quality_cfg->reward_item.is_bind;

			steal_role->GetKnapsack()->PutOrMail(reward_item, PUT_REASON_FISH_POOL_HARVEST);
			steal_role->AddExp((steal_role->GetLevel() + 50) * fish_quality_cfg->exp * multiple, "teal_fish_pool_exp", Role::EXP_ADD_REASON_EXP_FISH_POOL);
			steal_role->GetRuneSystem()->AddRuneJingHua(fish_quality_cfg->rune_score * multiple);

			is_steal_succ = 1;

			if (m_common_data.fish_num > 0)
			{
				m_common_data.fish_num--;

				// 偷鱼者是过来复仇的，不能将对方变成仇人
				if (1 == multiple)
				{
					this->AddTealUid(steal_role->GetUID());
					Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role_id));
					if (NULL != role)
					{
						// 刷新仇人列表
						m_fish_pool_manger->SendStealFishPoolGeneralInfo(role);
					}
				}
				else
				{
					//记录复仇成功
					stealer_fish_pool->ChangeTealUid(m_role_id);
					m_fish_pool_manger->SendStealFishPoolGeneralInfo(steal_role);
				}
			}

			steal_role->NoticeNum(noticenum::NT_FISHPOOL_HARVEST_SUCC);
		}

		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role_id));
		if (NULL != role)
		{
			this->SendAllRaiseInfo(role);
		}
		m_fish_pool_manger->OnFishPoolChange(this, structcommon::CS_UPDATE);
		this->FishPoolChange(steal_role, be_steal_uid, m_common_data.fish_num, m_common_data.fish_quality, is_steal_succ);
	}

	stealer_fish_pool->SendCommonInfo(steal_role);
	m_fish_pool_manger->OnFishPoolChange(stealer_fish_pool, structcommon::CS_UPDATE);
}

void FishPool::AddTealUid(int uid)
{
	int index = -1;
	for (int i = 0; i < FISH_POOL_BE_TEAL_FISH_UID_MAX; i++)
	{
		if (m_common_data.teal_fish_list[i].uid == uid)
		{
			index = i;
		}
	}

	if (-1 == index)
	{
		for (int i = 0; i < FISH_POOL_BE_TEAL_FISH_UID_MAX; i++)
		{
			for (int j = 0; j < FISH_POOL_BE_TEAL_FISH_UID_MAX; j++)
			{
				if (m_common_data.teal_fish_list[i].steal_fish_time > m_common_data.teal_fish_list[j].steal_fish_time)
				{
					break;
				}

				if (j == FISH_POOL_BE_TEAL_FISH_UID_MAX - 1)
				{
					index = i;
					break;
				}
			}

			if (-1 != index)
			{
				break;
			}
		}
	}

	if (index >= 0 && index < FISH_POOL_BE_TEAL_FISH_UID_MAX)
	{
		m_common_data.teal_fish_list[index].steal_fish_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		m_common_data.teal_fish_list[index].uid = uid;
		m_common_data.teal_fish_list[index].is_fuchou = 0;
		m_common_data.teal_fish_list[index].be_steal_quality = m_common_data.fish_quality;
	}
}

bool FishPool::IsStealUid(int uid)
{
	if (uid <= 0) return false;

	for (int i = 0; i < FISH_POOL_BE_TEAL_FISH_UID_MAX; i++)
	{
		if (uid == m_common_data.teal_fish_list[i].uid && 0 == m_common_data.teal_fish_list[i].is_fuchou)
		{
			return true;
		}
	}
	return false;
}

void FishPool::ChangeTealUid(int uid)
{
	if (uid <= 0) return;

	for (int i = 0; i < FISH_POOL_BE_TEAL_FISH_UID_MAX; i++)
	{
		if (uid == m_common_data.teal_fish_list[i].uid)
		{
			m_common_data.teal_fish_list[i].is_fuchou = 1;
		}
	}
}

void FishPool::FishPoolChange(Role * role, int uid, int fish_num, int fish_quality, int is_steal_succ)
{
	if (NULL == role)
	{
		return;
	}

	static Protocol::SCFishPoolChange pro;
	pro.uid = uid;
	pro.fish_num = fish_num;
	pro.fish_quality = fish_quality;
	pro.is_steal_succ = is_steal_succ;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, sizeof(pro));
}

void FishPool::BuyBulletReq(Role *role)
{
 	if (NULL == role || role->GetUID() != m_role_id)
 	{
 		return;
 	}
 
 	const FishPoolOtherConfig &other_cfg = LOGIC_CONFIG->GetFishPoolCfg().GetOtherCfg();
 	if (m_common_data.bullet_buy_times >= other_cfg.day_buy_bullet_limit_times)
 	{
 		role->NoticeNum(errornum::EN_FISH_POOL_BUY_BULLET_LIMIT_TIMES);
 		return;
 	}
 
 	if (role->GetSublock()->CheckHasLock())
 	{
 		return;
 	}
 
 	if (role->GetKnapsack()->IsLock())
 	{
 		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
 		return;
 	}

	int gold = LOGIC_CONFIG->GetFishPoolCfg().GetBuyBulletGold(m_common_data.bullet_buy_times + 1);
	if (gold <= 0) return;
 
 	if (!role->GetKnapsack()->GetMoney()->UseGold(gold, "BuyBullet"))
 	{
 		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
 		return;
 	}
 
	++m_common_data.bullet_buy_times;
 	
 	this->IncreaseBullet(other_cfg.give_bullet_per_buy);
 	this->SendCommonInfo(role);
 
 	{
 		// 日志
 		gamelog::g_log_fish_pool.printf(LL_INFO, "IncreaseBullet::user[%d, %s], level[%d], use_gold[%d], buy_num[%d], bullet_buy_num[%d]",
 			role->GetUID(), role->GetName(), role->GetLevel(), gold, other_cfg.give_bullet_per_buy, m_common_data.bullet_buy_num);
 	}
}

bool FishPool::HasBullet()
{
 	int own_bullet = LOGIC_CONFIG->GetFishPoolCfg().GetOtherCfg().base_bullet_num + m_common_data.bullet_buy_num;
 
 	return own_bullet > m_common_data.bullet_consume_num;
}

void FishPool::IncreaseBullet(int add_value)
{
 	if (add_value <= 0)
 	{
 		return;
 	}
 
 	m_common_data.bullet_buy_num += add_value;
 	m_fish_pool_manger->OnFishPoolChange(this, structcommon::CS_UPDATE);
}

void FishPool::ConsumeBullet()
{
 	++ m_common_data.bullet_consume_num;
 	m_fish_pool_manger->OnFishPoolChange(this, structcommon::CS_UPDATE);
 
 	{
 		// 日志
 		gamelog::g_log_fish_pool.printf(LL_INFO, "ConsumeBullet::user[%d, %s], bullet_consume_num[%d]", m_role_id, m_common_data.role_name, m_common_data.bullet_consume_num);
 	}
}

void FishPool::SendAllRaiseInfo(Role *role)
{
 	if (NULL == role) return;
 
 	static Protocol::SCFishPoolAllRaiseInfo fpari;
 	fpari.owner_uid = m_role_id;
	fpari.fish_quality = m_common_data.fish_quality;
	fpari.fish_num = m_common_data.fish_num;
	fpari.fang_fish_time = m_common_data.fang_fish_time;
 
 	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&fpari, sizeof(fpari));
}

void FishPool::SendCommonInfo(Role *role)
{
 	if (NULL == role) return;
 
 	static Protocol::SCFishPoolCommonInfo foci;
 	foci.common_data.owner_uid = m_role_id;

 	F_STRNCPY(foci.common_data.owner_name, m_common_data.role_name, sizeof(foci.common_data.owner_name));
 	foci.common_data.role_level = m_common_data.role_level;

 	foci.common_data.bullet_buy_times = m_common_data.bullet_buy_times;
 	foci.common_data.bullet_buy_num = m_common_data.bullet_buy_num;
 	foci.common_data.bullet_consume_num = m_common_data.bullet_consume_num;

	foci.common_data.today_fang_fish_times = m_common_data.today_fang_fish_times;
	foci.common_data.today_buy_fang_fish_tims = m_common_data.today_buy_fang_fish_tims;

 	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&foci, sizeof(foci));
}

void FishPool::UpFishQuality(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (m_common_data.fish_quality >= FISH_QUALITY_COUNT - 1)
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (0 != m_common_data.fang_fish_time)
	{
		role->NoticeNum(errornum::EN_FISH_POOL_UP_FISH_LIMIT);
		return;
	}

	const FishPoolOtherConfig &other_cfg = LOGIC_CONFIG->GetFishPoolCfg().GetOtherCfg();
	if (m_common_data.today_fang_fish_times >= m_common_data.today_buy_fang_fish_tims + other_cfg.today_free_fang_fish_times)
	{
		role->NoticeNum(errornum::EN_FISH_POOL_FANG_FISH_TIMES_LIMIT);
		return;
	}

	FishQualityTypeCfg * fish_quality_cfg = LOGIC_CONFIG->GetFishPoolCfg().GetFishQualityCfg(m_common_data.fish_quality);
	if (NULL == fish_quality_cfg)
	{
		return;
	}

	if (fish_quality_cfg->up_quality_gold > 0 && !role->GetKnapsack()->GetMoney()->UseGold(fish_quality_cfg->up_quality_gold, "RaiseFish_up_quality"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int rand_val = RandomNum(100);
	if (rand_val <= fish_quality_cfg->up_quality_pro)
	{
		m_common_data.fish_quality++;
		m_fish_pool_manger->OnFishPoolChange(this, structcommon::CS_UPDATE);
		role->NoticeNum(noticenum::NT_FISH_POOL_UP_FISH_QUALITY_SUCC);
	}
	else
	{
		role->NoticeNum(errornum::EN_FISH_POOL_UP_FISH_FAIL);
	}

	static Protocol::SCUpFishQualityRet fpari;
	fpari.quality = m_common_data.fish_quality;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&fpari, sizeof(fpari));
}

void FishPool::BuyFangFishTimes(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	const FishPoolOtherConfig &other_cfg = LOGIC_CONFIG->GetFishPoolCfg().GetOtherCfg();
	if (m_common_data.today_buy_fang_fish_tims >= other_cfg.today_buy_fang_fish_times_limit)
	{
		role->NoticeNum(errornum::EN_FISH_POOL_BUY_FANG_FISH_TIMES_LIMIT);
	}

	int gold = LOGIC_CONFIG->GetFishPoolCfg().GetBuyFangFishGold(m_common_data.today_buy_fang_fish_tims + 1);

	if (gold > 0 && !role->GetKnapsack()->GetMoney()->UseGold(gold, "RaiseFish_buy_fang_fish_times"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}
	
	m_common_data.today_buy_fang_fish_tims++;
	this->SendCommonInfo(role);
	m_fish_pool_manger->OnFishPoolChange(this, structcommon::CS_UPDATE);
	role->NoticeNum(noticenum::NT_CROSS_BOSS_BUY_RELIVE_TIME_SUCC);
}

bool FishPool::IsCanSteal()
{
	FishQualityTypeCfg * fish_quality_cfg = LOGIC_CONFIG->GetFishPoolCfg().GetFishQualityCfg(m_common_data.fish_quality);
	if (NULL == fish_quality_cfg)
	{
		return false;
	}

	// 已经成熟，就不显示在可偷列表
	if (static_cast<int>(EngineAdapter::Instance().Time()) > m_common_data.fang_fish_time + fish_quality_cfg->need_time)
	{
		return false;
	}

	if (m_common_data.fish_num > fish_quality_cfg->steal_limit)
	{
		return true;
	}
	
	return false;
}

bool FishPool::IsInOtherPoolList(FishPool * fish_pool)
{
	if (NULL == fish_pool)
	{
		return false;
	}

	for (int i = 0; i < FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT; i++)
	{
		if (m_other_pool_list[i].owner_uid == fish_pool->GetOwnerUid())
		{
			return true;
		}
	}
	return false;
}

int FishPool::GetOtherPoolList(Protocol::FishPoolGeneralInfoItem other_pool_list[])
{
	int count = 0;
	// 获取真玩家的鱼池
	for (int i = 0; i < FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT && count < FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT; i++)
	{
		FishPool *other_fish_pool = m_fish_pool_manger->GetFishPool(m_other_pool_list[i].owner_uid);
		if (NULL != other_fish_pool && other_fish_pool->GetOwnerUid() == m_other_pool_list[i].owner_uid && other_fish_pool->IsCanSteal())
		{
			other_pool_list[count].owner_uid = m_other_pool_list[i].owner_uid;
			F_STRNCPY(other_pool_list[count].owner_name, m_other_pool_list[i].owner_name, sizeof(GameName));
			other_pool_list[count].fish_quality = m_other_pool_list[i].fish_quality;
			other_pool_list[count].fish_num = m_other_pool_list[i].fish_num;
			other_pool_list[count].fang_fish_time = m_other_pool_list[i].fang_fish_time;
			other_pool_list[count].is_fake_pool = m_other_pool_list[i].is_fake_pool;
			other_pool_list[count].is_fuchou = m_other_pool_list[i].is_fuchou;
			other_pool_list[count].be_steal_quality = m_other_pool_list[i].be_steal_quality;
			other_pool_list[count].steal_fish_time = m_other_pool_list[i].steal_fish_time;

			count++;
		}
	}

	// 获取假鱼池
	for (int i = 0; i < FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT && count < FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT; i++)
	{
		FishPool *other_fish_pool = m_fish_pool_manger->GetFishPool(m_other_pool_list[i].owner_uid);
		FishQualityTypeCfg * fish_quality_cfg = LOGIC_CONFIG->GetFishPoolCfg().GetFishQualityCfg(m_other_pool_list[i].fish_quality);
		if (NULL != fish_quality_cfg)
		{
			if (NULL == other_fish_pool && m_other_pool_list[i].fish_num > fish_quality_cfg->steal_limit)
			{
				if (static_cast<int>(EngineAdapter::Instance().Time()) > m_other_pool_list[count].fang_fish_time + fish_quality_cfg->need_time)
				{
					continue;
				}

				other_pool_list[count].owner_uid = m_other_pool_list[i].owner_uid;
				F_STRNCPY(other_pool_list[count].owner_name, m_other_pool_list[i].owner_name, sizeof(GameName));
				other_pool_list[count].fish_quality = m_other_pool_list[i].fish_quality;
				other_pool_list[count].fish_num = m_other_pool_list[i].fish_num;
				other_pool_list[count].fang_fish_time = m_other_pool_list[i].fang_fish_time;
				other_pool_list[count].is_fake_pool = m_other_pool_list[i].is_fake_pool;
				other_pool_list[count].is_fuchou = m_other_pool_list[i].is_fuchou;
				other_pool_list[count].be_steal_quality = m_other_pool_list[i].be_steal_quality;
				other_pool_list[count].steal_fish_time = m_other_pool_list[i].steal_fish_time;

				count++;
			}
		}
	}

	return count;
}

void FishPool::SetOtherPoolList(Protocol::FishPoolGeneralInfoItem * other_pool_list, int array_size)
{
	if (NULL == other_pool_list) return;

	for (int i = 0; i < FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT; i++)
	{
		m_other_pool_list[i].Reset();
	}

	for (int i = 0; i < array_size; i++)
	{
		if (i >= FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT) return;

		m_other_pool_list[i].owner_uid = other_pool_list[i].owner_uid;
		F_STRNCPY(m_other_pool_list[i].owner_name, other_pool_list[i].owner_name, sizeof(GameName));
		m_other_pool_list[i].fish_quality = other_pool_list[i].fish_quality;
		m_other_pool_list[i].fish_num = other_pool_list[i].fish_num;
		m_other_pool_list[i].fang_fish_time = other_pool_list[i].fang_fish_time;
		m_other_pool_list[i].is_fake_pool = other_pool_list[i].is_fake_pool;
		m_other_pool_list[i].is_fuchou = other_pool_list[i].is_fuchou;
		m_other_pool_list[i].be_steal_quality = other_pool_list[i].be_steal_quality;
		m_other_pool_list[i].steal_fish_time = other_pool_list[i].steal_fish_time;
	}
}

void FishPool::OnSkipFishPool(Role *role, int quality)
{
	if (role == nullptr || quality < 0 || quality >= FISH_QUALITY_COUNT)
	{
		return;
	}
	
	const FishPoolSkipCfg &skip_cfg = LOGIC_CONFIG->GetFishPoolCfg().GetSkipCfg();
	const FishPoolOtherConfig &other_cfg = LOGIC_CONFIG->GetFishPoolCfg().GetOtherCfg();
	if (NULL != role)
	{
		if (role->GetLevel() < skip_cfg.skip_steal_level_limit || role->GetLevel() < skip_cfg.fish_skip_cfg[quality].limit_level)
		{
			return;
		}

		int steal_times = LOGIC_CONFIG->GetFishPoolCfg().GetOtherCfg().base_bullet_num + m_common_data.bullet_buy_num - m_common_data.bullet_consume_num;
		int fish_count = m_common_data.today_buy_fang_fish_tims + other_cfg.today_free_fang_fish_times - m_common_data.today_fang_fish_times;

		int consume = steal_times * skip_cfg.skip_steal_consume + fish_count * skip_cfg.fish_skip_cfg[quality].consume;

		if (consume <= 0)
		{
			return;
		}

		if (!role->GetKnapsack()->GetMoney()->UseAllGold(consume, "Skip fish pool"))
		{
			role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		for (int i = 0; i < fish_count; ++i)
		{
			m_common_data.fish_quality = quality;
			this->HarvestFish(role, true);
		}

		m_common_data.today_fang_fish_times += fish_count;

		for (int i = 0; i < steal_times; ++i)
		{
			int steal_quality = LOGIC_CONFIG->GetFishPoolCfg().GetFakeFishQuality();
			FishQualityTypeCfg * fish_quality_cfg = LOGIC_CONFIG->GetFishPoolCfg().GetFishQualityCfg(steal_quality);
			FishPool *stealer_fish_pool = m_fish_pool_manger->GetFishPool(role->GetUID());
			if (NULL == stealer_fish_pool || NULL == fish_quality_cfg)
			{
				continue;
			}

			stealer_fish_pool->ConsumeBullet();

			if (RandomNum(100) >= fish_quality_cfg->be_steal_pro)
			{
				continue;
			}

			role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_CATCH_FISH);

			int multiple = 1;
			fish_quality_cfg->reward_item.num = fish_quality_cfg->reward_item.num * multiple;
			role->GetKnapsack()->PutOrMail(fish_quality_cfg->reward_item, PUT_REASON_FISH_POOL_HARVEST);
			role->AddExp((role->GetLevel() + 50) * fish_quality_cfg->exp * multiple, "teal_fish_pool_exp", Role::EXP_ADD_REASON_EXP_FISH_POOL);
			role->GetRuneSystem()->AddRuneJingHua(fish_quality_cfg->rune_score * multiple);
			role->NoticeNum(noticenum::NT_FISHPOOL_HARVEST_SUCC);

			m_fish_pool_manger->OnFishPoolChange(stealer_fish_pool, structcommon::CS_UPDATE);
		}

		this->SendCommonInfo(role);
	}
}

Protocol::FishPoolGeneralInfoItem * FishPool::GetFakePool(int fake_uid)
{
	for (int i = 0; i < FISH_POOL_RANDOM_GET_ROLE_MAX_COUNT; i++)
	{
		if (m_other_pool_list[i].owner_uid == fake_uid)
		{
			return &m_other_pool_list[i];
		}
	}
	return NULL;
}