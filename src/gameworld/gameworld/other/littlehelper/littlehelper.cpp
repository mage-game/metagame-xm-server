#include "littlehelper.h"
#include "config/logicconfigmanager.hpp"
#include "config/lotterycountconfig.hpp"
#include "global/worldstatus3/worldstatus3.hpp"
#include "item/itempool.h"
#include "monster/drop/dropconfig.hpp"
#include "monster/drop/droppool.hpp"
#include "item/knapsack.h"
#include "other/jinglingadvantage/jinglingadvantage.hpp"
#include "task/newtaskmanager.h"
#include "task/other/husongtask.hpp"
#include "other/fb/roledailyfb.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "other/activedegree/activedegree.hpp"
#include "global/bossfamily/bossfamilymanager.hpp"
#include "other/daycounter/daycounter.hpp"
#include "npc/transmit/fbmanager.hpp"
#include "task/other/paohuantask.hpp"
#include "task/other/tumotask.hpp"
#include "task/other/guildtask.hpp"
#include "other/touzijihua/touzijihua.hpp"
#include "other/vip/vip.hpp"
#include "other/vip/vipconfig.hpp"
#include "config/activityconfig/babybossconfig.hpp"
#include "other/shop/shop.hpp"
#include "other/chengjiu/chengjiu.h"
#include "other/roleactivity/roleactivity.hpp"
#include "obj/otherobj/fallingitem.h"
#include "global/droplog/droplogmanager.hpp"

LittleHelper::LittleHelper() : m_role(nullptr)
{
}

LittleHelper::~LittleHelper()
{
}

void LittleHelper::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid)
	{
		return;
	}
	this->SendRefreshInfo(Protocol::LITTLE_HELPER_COMPLETE_TYPE_ALL);
}

bool LittleHelper::GiveDropItemList(lhns::complete_type_t type)
{
	if (type < 0 || type >= Protocol::LITTLE_HELPER_COMPLETE_TYPE_MAX)
	{
		return false;
	}
	const LittleHelperCfg * cfg = LOGIC_CONFIG->GetLittleHelperConfig().GetHelperCfgByLevel(type, m_role->GetLevel());
	if (nullptr == cfg)
	{
		return false;
	}
	ItemConfigData item_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM];
	int list_num = 0;
	for (size_t i = 0; i < cfg->drop_id_vec.size(); ++i)
	{
		static ItemConfigData drop_item_list[DropConfig::DROP_ITEM_PROB_MAX];
		const int drop_item_count = DROPPOOL->GetDropItemList(cfg->drop_id_vec[i], drop_item_list);
		for (int i = 0; i < drop_item_count && i < DropConfig::DROP_ITEM_PROB_MAX; ++i)
		{
			if (!ItemExtern::PutInItemConfigList(ItemNamespace::MAX_KNAPSACK_GRID_NUM, item_list, &list_num,
				drop_item_list[i].item_id, drop_item_list[i].num, drop_item_list[i].is_bind))
			{
				break;
			}
		}
	}

	for (int i = 0; i < list_num && i < ItemNamespace::MAX_KNAPSACK_GRID_NUM; ++i)
	{
		EquipmentParam equip_param;
		ItemDataWrapper item_wrapper;
		if (!FallingItem::GetPickItem(cfg->monster_id, item_list[i].item_id, item_list[i].num, item_list[i].is_bind, item_wrapper, &equip_param, 0))
		{
			continue;
		}
		m_role->GetKnapsack()->Put(item_wrapper, PUT_REASON_LITTLE_HELPER_BOSS, NULL, cfg->monster_id);
	}

	return true;
}

bool LittleHelper::Consume(lhns::complete_type_t type)
{
	if (type < 0 || type >= Protocol::LITTLE_HELPER_COMPLETE_TYPE_MAX)
	{
		return false;
	}
	const LittleHelperCfg * cfg = LOGIC_CONFIG->GetLittleHelperConfig().GetHelperCfgByLevel(type, m_role->GetLevel());
	if (nullptr == cfg)
	{
		return false;
	}
	if (cfg->gold > 0)
	{
		Money *money = m_role->GetKnapsack()->GetMoney();
		if (nullptr == money)
		{
			return false;
		}
		switch (cfg->money_type)
		{
		case lhns::CONSUME_TYPE_BIND_GOLD:
		{
			if (!money->AllGoldMoreThan(cfg->gold))
			{
				return false;
			}
			int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "LittleHelper::Consume type:%d", (int)type);
			if (len <= 0)
			{
				return false;
			}
			if (!money->UseAllGold(cfg->gold, gamestring::GAMESTRING_BUF))
			{
				return false;
			}
		}
		break;
		case lhns::CONSUME_TYPE_GOLD:
		{
			if (!money->GoldMoreThan(cfg->gold))
			{
				return false;
			}
			int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "LittleHelper::Consume type:%d", (int)type);
			if (len <= 0)
			{
				return false;
			}
			if (!money->UseGold(cfg->gold, gamestring::GAMESTRING_BUF))
			{
				return false;
			}
		}
		break;
		default: return false;
		}
	}
	return true;
}

bool LittleHelper::ConsumeAndReward(lhns::complete_type_t type)
{
	if (type < 0 || type >= Protocol::LITTLE_HELPER_COMPLETE_TYPE_MAX)
	{
		return false;
	}
	if (!this->Consume(type))
	{
		return false;
	}
	if (!this->GiveDropItemList(type))
	{
		return false;
	}
	return true;
}

void LittleHelper::Opera(const Protocol::CSLittleHelperOpera::Req req)
{
	const int max_repeat_times = 20;	//防止客户端传错,导致循环太多次
	this->CheckBuyEscortTimes(req.type, req.param_0);
	for (int i = 0; i < req.param_0 && i < max_repeat_times; ++i)
	{
		switch (req.type)
		{
		case Protocol::LITTLE_HELPER_COMPLETE_TYPE_EASY_BOSS:
		{
			this->EasyBoss(req.type);
		}
		break;
		case Protocol::LITTLE_HELPER_COMPLETE_TYPE_DIFFICULT_BOSS:
		{
			this->DifficultBoss(req.type);
		}
		break;
		case Protocol::LITTLE_HELPER_COMPLETE_TYPE_BABY_BOSS:
		{
			this->BabyBoss(req.type);
		}
		break;
		case Protocol::LITTLE_HELPER_COMPLETE_TYPE_SUIT_BOSS:
		{
			this->SuitBoss(req.type);
		}
		break;
		case Protocol::LITTLE_HELPER_COMPLETE_TYPE_DEMON_BOSS:
		{
			this->DemonBoss(req.type);
		}
		break;
		case Protocol::LITTLE_HELPER_COMPLETE_TYPE_PET_ADVENTURE:
		{
			JinglingAdvantage::Instance().OnSkipGather(m_role, req.param_1);
		}
		break;
		case Protocol::LITTLE_HELPER_COMPLETE_TYPE_ESCORT_FAIRY:
		{
			this->EscortFairy(req.type);
		}
		break;
		case Protocol::LITTLE_HELPER_COMPLETE_TYPE_EXP_FB:
		{
			m_role->GetRoleDailyFb()->AutoFBReq(req.param_1);
		}
		break;
		case Protocol::LITTLE_HELPER_COMPLETE_TYPE_TOWER_DEFENSE_FB:
		{
			FBManager::Instance().AutoBuildTowerFB(m_role, req.param_1);
		}
		break;
		case Protocol::LITTLE_HELPER_COMPLETE_TYPE_CYCLE_TASK:
		{
			m_role->GetPaoHuanTask()->OnOneKeySkipTask();
		}
		break;
		case Protocol::LITTLE_HELPER_COMPLETE_TYPE_EXP_TASK:
		{
			m_role->GetDailyTask()->OnOneKeyFinishAllTumoTask();
		}
		break;
		case Protocol::LITTLE_HELPER_COMPLETE_TYPE_GUILD_TASK:
		{
			m_role->GetGuildTask()->OneKeyCompleteTask();
		}
		break;
		default: break;
		}
	}
}

void LittleHelper::OperaReq(const Protocol::CSLittleHelperOpera::Req req)
{
	//m_drop_item_vec_map.clear();
	DropLogManager::Instance().BeginLogGetItem();
	this->Opera(req);
	DropLogManager::Instance().EndLogGetItem(m_role);
	m_role->GetKnapsack()->UpdateToMail();
	this->SendRefreshInfo(req.type);
	//this->CheckSendDropItemListInfo();
}

void LittleHelper::OperaRepeatReq(const Protocol::CSLittleHelperOpera::Req req_list[Protocol::LITTLE_HELPER_COMPLETE_TYPE_MAX], const int count)
{
	if (count <= 0 || count > Protocol::LITTLE_HELPER_COMPLETE_TYPE_MAX || NULL == req_list)
	{
		return;
	}
	
	//m_drop_item_vec_map.clear();
	DropLogManager::Instance().BeginLogGetItem();
	for (int i = 0; i < count; ++i)
	{
		this->Opera(req_list[i]);
	}
	DropLogManager::Instance().EndLogGetItem(m_role);
	m_role->GetKnapsack()->UpdateToMail();
	this->SendRefreshInfo(Protocol::LITTLE_HELPER_COMPLETE_TYPE_ALL);
	//this->CheckSendDropItemListInfo();
}

void LittleHelper::EasyBoss(lhns::complete_type_t type)
{
	const WearyCfg *active_boss_weary_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetWearyCfg(Protocol::CSBossFamilyOperate::BOSS_FAMILY_BUY_ACTIVE_BOSS_WEARY);//疲劳值类型weary_tpye，0是精英boss、1是活跃boss
	if (active_boss_weary_cfg == NULL)
	{
		return;
	}
	const WearyCfg *weary_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetWearyCfg(Protocol::CSBossFamilyOperate::BOSS_FAMILY_BUY_ACTIVE_BOSS_WEARY);
	if (weary_cfg == NULL)
	{
		return;
	}
	if (m_role->GetActiveBossWeary() >= active_boss_weary_cfg->weary_limit)
	{
		return;
	}
	if (!this->ConsumeAndReward(type))
	{
		return;
	}
	m_role->AddActiveBossWeary(weary_cfg->kill_add_weary);
	m_role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_ACTIVE_BOSS);
	this->OnKillMonster(type);
}

void LittleHelper::DifficultBoss(lhns::complete_type_t type)
{
	const WearyCfg *weary_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetWearyCfg(Protocol::CSBossFamilyOperate::BOSS_FAMILY_BUY_MIKU_WEARY);
	if (weary_cfg == NULL)
	{
		return;
	}
	if (m_role->GetMikuBossWeary() >= weary_cfg->weary_limit)
	{
		return;
	}
	if (!this->ConsumeAndReward(type))
	{
		return;
	}
	m_role->AddKillBossNum(MonsterInitParam::BOSS_TYPE_TASK_MIKU, 1);

	m_role->AddMikuBossWeary(weary_cfg->kill_add_weary);
	m_role->GetTouzijihua()->OnKillBoss();
	m_role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_MIKU_BOSS);
	this->OnKillMonster(type);
}

void LittleHelper::BabyBoss(lhns::complete_type_t type)
{
	short &enter_times = m_role->GetCommonDataParam()->baby_boss_enter_times;
	int enter_limit_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_BABY_BOSS_BUY_ENTER_TIMES);
	if (enter_times >= enter_limit_times)
	{
		return;
	}
	if (!this->CalcBabyBossConsume(type))
	{
		return;
	}

	const BabyBossOtherCfg &baby_boss_other_cfg = LOGIC_CONFIG->GetBabyBossConfig().GetOtherCfg();
	const BabyBossEnterCostCfg *cfg = LOGIC_CONFIG->GetBabyBossConfig().GetEnterCostCfg(enter_times);
	if (nullptr == cfg)
	{
		return;
	}
	if (!m_role->GetKnapsack()->ConsumeItemOrUseGold(baby_boss_other_cfg.need_item_id, cfg->need_item_num, "LittleHelper::BabyBoss", true, true))
	{
		return;
	}
	if (!this->ConsumeAndReward(type))
	{
		return;
	}
	++enter_times;
	BossFamilyManager::Instance().SendBabyBossRoleInfo(m_role);
	this->OnKillMonster(type);
}

bool LittleHelper::CalcBabyBossConsume(lhns::complete_type_t type)
{
	if (type < 0 || type >= Protocol::LITTLE_HELPER_COMPLETE_TYPE_MAX)
	{
		return false;
	}
	Knapsack *knapsack = m_role->GetKnapsack();
	Money *money = knapsack->GetMoney();
	if (knapsack == NULL || money == NULL)
	{
		return false;
	}
	const LittleHelperCfg * cfg = LOGIC_CONFIG->GetLittleHelperConfig().GetHelperCfgByLevel(type, m_role->GetLevel());
	if (nullptr == cfg)
	{
		return false;
	}
	Int64 need_bind_gold = 0;
	Int64 need_gold = 0;
	if (cfg->money_type == lhns::CONSUME_TYPE_BIND_GOLD)
	{
		need_bind_gold += cfg->gold;
	}
	else
	{
		need_gold += cfg->gold;
	}

	short enter_times = m_role->GetCommonDataParam()->baby_boss_enter_times;
	const BabyBossOtherCfg &baby_boss_other_cfg = LOGIC_CONFIG->GetBabyBossConfig().GetOtherCfg();
	const BabyBossEnterCostCfg *cost_cfg = LOGIC_CONFIG->GetBabyBossConfig().GetEnterCostCfg(enter_times);
	if (nullptr == cost_cfg)
	{
		return false;
	}
	int has_num = knapsack->GetItemNum(baby_boss_other_cfg.need_item_id);
	int need_buy_num = cost_cfg->need_item_num - has_num;
	if (need_buy_num > 0)
	{
		int gold_price = 0, bind_gold_price = 0;
		if (!LOGIC_CONFIG->GetShop().GetShopItemGoldPrice(baby_boss_other_cfg.need_item_id, &gold_price, &bind_gold_price))
		{
			return false;
		}
		need_bind_gold += bind_gold_price * need_buy_num;
	}

	if (!money->GoldMoreThan(need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}
	if (!money->GoldBindMoreThan(need_bind_gold))
	{
		if (!money->AllGoldMoreThan(need_bind_gold + need_gold))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return false;
		}
	}

	return true;
}

void LittleHelper::SuitBoss(lhns::complete_type_t type)
{
	if (BossFamilyManager::Instance().IsDabaoBossTireFull(m_role))
	{
		return;
	}
	if (!this->CalcSuitBossConsume(type))	//这里确保了材料和元宝足够,下面可以分开消耗
	{
		return;
	}
	const int enter_count = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_DABAO_ENTER_COUNT);
	const BossFamilyOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyOtherCfg();
	if (enter_count >= other_cfg.dabao_free_times)
	{
		const DabaoBuyCostCfg *cost_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetDabaoBuyCostCfg(enter_count);
		if (nullptr == cost_cfg)
		{
			m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
			return;
		}

		int can_enter_count = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_DABAO_BOSS_ENTER_TIMES);
		if (enter_count >= can_enter_count)
		{
			m_role->NoticeNum(errornum::EN_ENTER_DABAO_MAP_COUNT_LIMIT);
			return;
		}
		if (cost_cfg->cost_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(cost_cfg->cost_gold))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
		bool consum_succ = false;
		if (cost_cfg->cost_gold > 0 && m_role->GetKnapsack()->GetMoney()->UseAllGold(cost_cfg->cost_gold, "LittleHelper::SuitBoss"))
		{
			consum_succ = true;
		}
		if (!consum_succ && cost_cfg->consume_item_id > 0 && !m_role->GetKnapsack()->ConsumeItemOrUseGold(cost_cfg->consume_item_id, cost_cfg->consume_item_num, "LittleHelper::SuitBoss", true, true))
		{
			return;
		}
	}
	if (!this->Consume(type))
	{
		return;
	}
	if (!this->GiveDropItemList(type))
	{
		return;
	}
	m_role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_DABAO_ENTER_COUNT);
	BossFamilyManager::Instance().SendDabaoBossInfo(m_role);
	this->OnKillMonster(type);
}

bool LittleHelper::CalcSuitBossConsume(lhns::complete_type_t type)
{
	if (type < 0 || type >= Protocol::LITTLE_HELPER_COMPLETE_TYPE_MAX)
	{
		return false;
	}
	Knapsack *knapsack = m_role->GetKnapsack();
	Money *money = knapsack->GetMoney();
	if (knapsack == NULL || money == NULL)
	{
		return false;
	}
	const LittleHelperCfg * cfg = LOGIC_CONFIG->GetLittleHelperConfig().GetHelperCfgByLevel(type, m_role->GetLevel());
	if (nullptr == cfg)
	{
		return false;
	}
	Int64 need_bind_gold = 0;
	Int64 need_gold = 0;
	if (cfg->money_type == lhns::CONSUME_TYPE_BIND_GOLD)
	{
		need_bind_gold += cfg->gold;
	}
	else
	{
		need_gold += cfg->gold;
	}
	const int enter_count = m_role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_DABAO_ENTER_COUNT);
	const BossFamilyOtherCfg &other_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyOtherCfg();
	if (enter_count >= other_cfg.dabao_free_times)
	{
		const DabaoBuyCostCfg *cost_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetDabaoBuyCostCfg(enter_count);
		if (nullptr == cost_cfg)
		{
			m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
			return false;
		}

		int can_enter_count = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role->GetVip()->GetVipLevel(), VAT_DABAO_BOSS_ENTER_TIMES);
		if (enter_count >= can_enter_count)
		{
			m_role->NoticeNum(errornum::EN_ENTER_DABAO_MAP_COUNT_LIMIT);
			return false;
		}
		bool consum_succ = false;
		if (cost_cfg->cost_gold > 0)
		{
			need_bind_gold += cost_cfg->cost_gold;
			consum_succ = true;
		}

		int has_num = knapsack->GetItemNum(cost_cfg->consume_item_id);
		int need_buy_num = cost_cfg->consume_item_num - has_num;
		if (!consum_succ && need_buy_num > 0)
		{
			int gold_price = 0, bind_gold_price = 0;
			if (!LOGIC_CONFIG->GetShop().GetShopItemGoldPrice(cost_cfg->consume_item_id, &gold_price, &bind_gold_price))
			{
				return false;
			}
			need_bind_gold += bind_gold_price * need_buy_num;
		}
	}
	if (!money->GoldMoreThan(need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return false;
	}
	if (!money->GoldBindMoreThan(need_bind_gold))
	{
		if (!money->AllGoldMoreThan(need_bind_gold + need_gold))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return false;
		}
	}

	return true;
}

void LittleHelper::DemonBoss(lhns::complete_type_t type)
{
	int tire = BossFamilyManager::Instance().GetGodmagicBossTire(m_role);
	if (tire <= 0)
	{
		return;
	}
	if (!this->ConsumeAndReward(type))
	{
		return;
	}
	BossFamilyManager::Instance().AddGodmagicBossKillNum(m_role);
	this->OnKillMonster(type);
}

void LittleHelper::OnKillMonster(lhns::complete_type_t type)
{
	m_role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_KILLBOSS);
	m_role->GetTaskManager()->OnCheckSatisfyStatus(TASK_SATISFY_STATUS_TYPE_KILL_BOSS_TYPE);
	m_role->GetChengJiuMgr()->OnChengJiuCondition(CJ_CONDITION_KILL_BOSS);
	m_role->GetRoleActivity()->OnRAExtremeChallengeAddPlan(RA_EXTREME_CHALLENGE_KILL_BOSS, 1);
	//m_role->GetTaskManager()->OnKillMonster(scene_id, pos, monster_id, monster_level);		//没有bossid先注释

	if (type == Protocol::LITTLE_HELPER_COMPLETE_TYPE_BABY_BOSS)
	{
		m_role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_BB_BOSS, 1);	//宝宝
	}
	if (type == Protocol::LITTLE_HELPER_COMPLETE_TYPE_EASY_BOSS)
	{
		m_role->GetRoleActivity()->OnKuangHaiQingDianRecore(RA_KUANG_HAI_TASK_TYPE_KILL_BOSS, 1); //活跃
	}
}

void LittleHelper::EscortFairy(lhns::complete_type_t type)
{
	const LittleHelperCfg * cfg = LOGIC_CONFIG->GetLittleHelperConfig().GetHelperCfgByLevel(type, m_role->GetLevel());
	if (nullptr == cfg)
	{
		return;
	}
	if (!m_role->GetHusongTask()->LittleHelperCanAcceptHusongTask(true))
	{
		return;
	}
	if (!this->Consume(type))
	{
		return;
	}
	m_role->GetHusongTask()->SetTaskColor(TASK_COLOR_ORANGE);
	m_role->GetHusongTask()->OnLittleHelperAddTask(cfg->task_id, false);
	m_role->GetHusongTask()->OnLittleHelperRemoveTask(cfg->task_id, true);
}

void LittleHelper::CheckBuyEscortTimes(int type, int repeat_times)
{
	const int can_husong_times = m_role->GetHusongTask()->GetTimes();
	if (type == Protocol::LITTLE_HELPER_COMPLETE_TYPE_ESCORT_FAIRY && repeat_times > can_husong_times)
	{
		m_role->GetHusongTask()->OnBuyTimes(repeat_times - can_husong_times);
	}
}

void LittleHelper::AutoBuildTowerFB(short repeat_times, int flush_extra_num)
{
	if (flush_extra_num > BUILD_TOWER_MAX_MONSTER_WAVE + 1)
	{

	}
	FBManager::Instance().AutoBuildTowerFB(m_role, flush_extra_num);
}

void LittleHelper::SendRefreshInfo(short type)
{
	Protocol::SCLittleHelperInfo info;
	info.type = type;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, sizeof(info));
}

void LittleHelper::CheckSendDropItemListInfo()
{
	if (m_drop_item_vec_map.size() == 0)
	{
		return;
	}
	bool is_break = false;
	Protocol::SCLittleHelperItemInfo info;
	info.count = 0;
	for (auto &map_it : m_drop_item_vec_map)
	{
		for (size_t i = 0; i < map_it.second.size(); ++i)
		{
			if (info.count >= lhns::MAX_DROP_ITEM_COUNT)
			{
				is_break = true;
				break;
			}
			UNSTD_STATIC_CHECK(sizeof(info.item_list[info.count]) == sizeof(map_it.second[i]));
			memcpy(&info.item_list[info.count], &map_it.second[i], sizeof(info.item_list[info.count]));
			++info.count;
		}
		if (is_break)
		{
			break;
		}
	}
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&info, sizeof(info) - (lhns::MAX_DROP_ITEM_COUNT - info.count) * sizeof(Protocol::SCLittleHelperItemInfo::RewardItem));
}

