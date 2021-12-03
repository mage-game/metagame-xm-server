#include "cardzu.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/cardzu/cardzuconfig.hpp"
#include "item/itemextern.hpp"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "other/event/eventhandler.hpp"
#include "protocal/msgcardzu.h"
#include "servercommon/noticenum.h"
#include "obj/character/attribute.hpp"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"
#include "other/sublock/sublock.hpp"

Cardzu::Cardzu() : m_role(NULL), m_active_max_cardzu_id(0)
{
}

Cardzu::~Cardzu()
{

}

void Cardzu::InitParam(Role *role, const CardzuParam &param)
{
	m_role = role;
	m_param = param;

	this->CheckMaxActiveCardzu(true);
}

void Cardzu::GetInitParam(CardzuParam *param)
{
	*param = m_param;
}

void Cardzu::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
 	if (is_recalc)
 	{
 		m_attrs_add.Reset();
 
 		for (int zuhe_idx = 0; zuhe_idx <= CARDZU_MAX_ZUHE_ID; ++ zuhe_idx)
 		{
 			int zuhe_level = m_param.zuhe_level_list[zuhe_idx];
 			const CardzuZuheConfig::AttributeConfig *attr_cfg = LOGIC_CONFIG->GetCardzuCfg().GetZuheAttributeCfg(zuhe_idx, zuhe_level);
 			if (NULL != attr_cfg)
 			{
 				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += attr_cfg->maxhp;
 				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += attr_cfg->gongji;
 				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += attr_cfg->fangyu;
 				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += attr_cfg->mingzhong;
 				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += attr_cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += attr_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += attr_cfg->jianren;
 			}
 		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_CARDZU, m_attrs_add);
 	}

 	base_add.Add(m_attrs_add.m_attrs, true);
}

void Cardzu::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
 	if (old_dayid == now_dayid) return;
 
 	memset(m_param.today_coin_buy_card_times, 0, sizeof(m_param.today_coin_buy_card_times));
	memset(m_param.today_gold_bind_buy_card_times, 0, sizeof(m_param.today_gold_bind_buy_card_times));
 	this->OnNotifyChange(-1);
}

void Cardzu::SendCardzuAllInfo()
{
 	static Protocol::SCCardzuAllInfo base_info;
 	base_info.lingli = m_param.lingli;
 	memcpy(base_info.today_coin_buy_card_times, m_param.today_coin_buy_card_times, sizeof(m_param.today_coin_buy_card_times));
	memcpy(base_info.today_gold_bind_buy_card_times, m_param.today_gold_bind_buy_card_times, sizeof(m_param.today_gold_bind_buy_card_times));
 	memcpy(base_info.all_card_count_list, m_param.card_count_list, sizeof(m_param.card_count_list));
 	memcpy(base_info.all_zuhe_level_list, m_param.zuhe_level_list, sizeof(m_param.zuhe_level_list));
 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&base_info, sizeof(base_info));
}

void Cardzu::OnChouCardReq(int cardzu_id, int type)
{
 	switch(type)
 	{
 	case BUY_LINGLI_TYPE_COIN:
 		{
 			this->OnCoinChouCard(cardzu_id);
 		}
 		break;
 
 	case BUY_LINGLI_TYPE_GOLD:
 		{
 			this->OnGoldChouCard(cardzu_id);
 		}
 		break;
 
 	case BUY_LINGLI_TYPE_GOLD10:
 		{
 			this->OnGoldBatchChouCard(cardzu_id);
 		}
 		break;
		
	case BUY_LINGLI_TYPE_GOLD_BIND:
		{
			this->OnGoldBindChouCard(cardzu_id);
		}
		break;
 	}
}

void Cardzu::OnHualingReq(int card_id, int count)
{
 	if (card_id < 0 || card_id >= CARDZU_MAX_CARD_ID || count <= 0)
 	{
 		return;
 	}
 
 	const CardzuCardConfig *card_cfg = LOGIC_CONFIG->GetCardzuCfg().GetCardCfg(card_id);
 	if (NULL == card_cfg)
 	{
 		return;
 	}
 
 	if (m_param.card_count_list[card_id] < count)
 	{
 		m_role->NoticeNum(errornum::EN_CARDZU_HUALING_CARD_NUM_LIMIT);
 		return;
 	}
 
 	// 消耗
 	int old_count = m_param.card_count_list[card_id];
 	m_param.card_count_list[card_id] -= count;
 
 	// 给予
 	int old_lingli = m_param.lingli;
 	m_param.lingli += (card_cfg->sell_lingli * count);
 
 	ChangeCardSet list;
 	list.insert(card_id);
 	this->OnNotifyChange(-1, &list);
 
 	m_role->NoticeNum(noticenum::NT_CARDZU_HUALING_SUCC);
 
 	gamelog::g_log_cardzu.printf(LL_INFO, "OnHualingReq user %d %s, card_idx %d, old_count %d, new_count %d, old_lingli %d, new_lingli %d",
 		m_role->GetUID(), m_role->GetName(), card_id, old_count, m_param.card_count_list[card_id], old_lingli, m_param.lingli);
}

void Cardzu::OnLingzhuReq(int card_id)
{
 	if (card_id < 0 || card_id >= CARDZU_MAX_CARD_ID)
 	{
 		return;
 	}
 
 	const CardzuCardConfig *card_cfg = LOGIC_CONFIG->GetCardzuCfg().GetCardCfg(card_id);
 	if (NULL == card_cfg)
 	{
 		return;
 	}
 
 	if (m_param.lingli < card_cfg->need_lingli)
 	{
 		m_role->NoticeNum(errornum::EN_CARDZU_ZHULING_LINGLI_LIMIT);
 		return;
 	}
 
 	// 消耗
 	int old_lingli = m_param.lingli;
 	m_param.lingli -= card_cfg->need_lingli;
 
 	// 给予
 	int old_count = m_param.card_count_list[card_id];
 	++ m_param.card_count_list[card_id];
 
 	ChangeCardSet list;
 	list.insert(card_id);
 	this->OnNotifyChange(-1, &list);
 
 	m_role->NoticeNum(noticenum::NT_CARDZU_LINGZHU_SUCC);
 
 	gamelog::g_log_cardzu.printf(LL_INFO, "OnLingzhuReq user %d %s, card_idx %d, old_count %d, new_count %d, old_lingli %d, new_lingli %d",
 		m_role->GetUID(), m_role->GetName(), card_id, old_count, m_param.card_count_list[card_id], old_lingli, m_param.lingli);
}

void Cardzu::OnActiveZuheReq(int zuhe_id)
{
 	if (zuhe_id < 0 || zuhe_id >= CARDZU_MAX_ZUHE_ID)
 	{
 		return;
 	}
 
 	if (m_param.zuhe_level_list[zuhe_id] > 0)
 	{
 		m_role->NoticeNum(errornum::EN_CARDZU_HAS_ACTIVED);
 		return;
 	}
 
 	const CardzuZuheConfig *card_cfg = LOGIC_CONFIG->GetCardzuCfg().GetZuheCfg(zuhe_id);
 	if (NULL == card_cfg)
 	{
 		return;
 	}
 
 	if (card_cfg->cardzu_id > m_active_max_cardzu_id)
 	{
 		m_role->NoticeNum(errornum::EN_CARDZU_HAS_NOT_ACTIVED);
 		return;
 	}
 
 	// 检查合成原料是否足够
 	if (card_cfg->need_card1_num > 0 && m_param.card_count_list[card_cfg->need_card1_id] < card_cfg->need_card1_num)
 	{
 		m_role->NoticeNum(errornum::EN_CARDZU_ACTIVED_ZUHE_CARD_NUM_LIMIT);
 		return;
 	}
 
 	if (card_cfg->need_card2_num > 0 && m_param.card_count_list[card_cfg->need_card2_id] < card_cfg->need_card2_num)
 	{
 		m_role->NoticeNum(errornum::EN_CARDZU_ACTIVED_ZUHE_CARD_NUM_LIMIT);
 		return;
 	}
 
 	if (card_cfg->need_card3_num > 0 && m_param.card_count_list[card_cfg->need_card3_id] < card_cfg->need_card3_num)
 	{
 		m_role->NoticeNum(errornum::EN_CARDZU_ACTIVED_ZUHE_CARD_NUM_LIMIT);
 		return;
 	}
 
 	ChangeCardSet consume_card_id_set;
 	{
 		// 消耗
 		if (card_cfg->need_card1_num > 0)
 		{
 			m_param.card_count_list[card_cfg->need_card1_id] -= card_cfg->need_card1_num;
 			consume_card_id_set.insert(card_cfg->need_card1_id);
 		}
 
 		if (card_cfg->need_card2_num > 0)
 		{
 			m_param.card_count_list[card_cfg->need_card2_id] -= card_cfg->need_card2_num;
 			consume_card_id_set.insert(card_cfg->need_card2_id);
 		}
 
 		if (card_cfg->need_card3_num > 0)
 		{
 			m_param.card_count_list[card_cfg->need_card3_id] -= card_cfg->need_card3_num;
 			consume_card_id_set.insert(card_cfg->need_card3_id);
 		}
 	}
 
 	// 给予
 	m_param.zuhe_level_list[zuhe_id] = 1;
 
 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CARDZU);
 	m_role->NoticeNum(noticenum::NT_CARDZU_ACTIVE_ZUHE_SUCC);
 
 	// notify change
 	this->OnNotifyChange(zuhe_id, &consume_card_id_set);
 
 	gamelog::g_log_cardzu.printf(LL_INFO, "OnActiveZuheReq user %d %s, zuhe_id %d", m_role->GetUID(), m_role->GetName(), zuhe_id);
 
 	this->CheckMaxActiveCardzu(false);
}

void Cardzu::OnUpgradeZuheReq(int zuhe_id, int target_level)
{
 	if (zuhe_id < 0 || zuhe_id > CARDZU_MAX_ZUHE_ID)
 	{
 		return;
 	}
 
 	int zuhe_level = m_param.zuhe_level_list[zuhe_id];
 	if (zuhe_level <= 0)
 	{
 		m_role->NoticeNum(errornum::EN_CARDZU_HAS_NOT_ACTIVED);
 		return;
 	}
 
 	if (zuhe_level >= target_level)
 	{
 		return;
 	}
 
 	const CardzuZuheConfig *zuhe_cfg = LOGIC_CONFIG->GetCardzuCfg().GetZuheCfg(zuhe_id);
 	if (NULL == zuhe_cfg)
 	{
 		return;
 	}
 
 	if (target_level > zuhe_cfg->zuhe_max_level)
 	{
 		return;
 	}
 
 	int upgrade_need_total_lingli = 0;
 	for (int cur_level = zuhe_level; cur_level < target_level; ++ cur_level)
 	{
 		const CardzuZuheConfig::AttributeConfig *attr_cfg = LOGIC_CONFIG->GetCardzuCfg().GetZuheAttributeCfg(zuhe_id, cur_level);
 		if (NULL == attr_cfg)
 		{
 			return;
 		}
 
 		upgrade_need_total_lingli += attr_cfg->upgrade_need_lingli;
 	}
 
 	if (m_param.lingli < upgrade_need_total_lingli)
 	{
 		m_role->NoticeNum(errornum::EN_CARDZU_UPGRADE_ZUHE_LINGLI_LIMIT);
 		return;
 	}
 
 	// 消耗
 	int old_lingli = m_param.lingli;
 	m_param.lingli -= upgrade_need_total_lingli;
 
 	// 给予
 	int old_zuhe_level = m_param.zuhe_level_list[zuhe_id];
 	m_param.zuhe_level_list[zuhe_id] = target_level;
 
 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CARDZU);
 	m_role->NoticeNum(noticenum::NT_CARDZU_UPGRADE_ZUHE_SUCC);
 
 	this->OnNotifyChange(zuhe_id);
 
 	gamelog::g_log_cardzu.printf(LL_INFO, "OnActiveZuheReq user %d %s, zuhe_id %d, old_lingli %d, new_lingli %d, old_zuhe_level %d, new_zuhe_level %d,",
 		m_role->GetUID(), m_role->GetName(), zuhe_id, old_lingli, m_param.lingli, old_zuhe_level, m_param.zuhe_level_list[zuhe_id]);
}

void Cardzu::OnNotifyChange(int zuhe_id, ChangeCardSet *change_card_list)
{
 	static Protocol::SCCardzuChangeNotify cdcn;
 	cdcn.lingli = m_param.lingli;
 	memcpy(cdcn.today_coin_buy_card_times, m_param.today_coin_buy_card_times, sizeof(m_param.today_coin_buy_card_times));
	memcpy(cdcn.today_gold_bind_buy_card_times, m_param.today_gold_bind_buy_card_times, sizeof(m_param.today_gold_bind_buy_card_times));
 
 	cdcn.change_zuhe_id = zuhe_id;
 	if (zuhe_id >= 0 && zuhe_id <= CARDZU_ZUHE_MAX_LEVEL)
 	{
 		cdcn.change_zuhe_level = m_param.zuhe_level_list[zuhe_id];
 	}
 
 	cdcn.change_card_count = 0;
 
 	if (NULL != change_card_list)
 	{
 		for (ChangeCardSetIt it = change_card_list->begin(); it != change_card_list->end() && 
 			cdcn.change_card_count < Protocol::CARDZU_BATCH_CHOUCARD_TIMES; ++ it)
 		{
 			short card_id = *it;
 
 			if (card_id >= 0 && card_id <= CARDZU_MAX_CARD_ID)
 			{
 				cdcn.change_card_list[cdcn.change_card_count].card_id = card_id;
 				cdcn.change_card_list[cdcn.change_card_count].count = m_param.card_count_list[card_id];
 				++ cdcn.change_card_count;
 			}
 		}
 	}
 
 	int len = sizeof(cdcn) - sizeof(cdcn.change_card_list[0]) * (Protocol::CARDZU_BATCH_CHOUCARD_TIMES - cdcn.change_card_count);
 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cdcn, len);
}

void Cardzu::CheckMaxActiveCardzu(bool is_init_check)
{
 	int cur_active_max_cardzu_id = 0;
 
 	for (int next_cardzu_id = 1; next_cardzu_id < CARDZU_TYPE_MAX_COUNT; ++ next_cardzu_id)
 	{
 		int active_zuhe_num = 0;
 
 		CardzuConfig::CardzuZuheSet *zuhe_set = LOGIC_CONFIG->GetCardzuCfg().GetZuheSet(cur_active_max_cardzu_id);
 		if (NULL != zuhe_set)
 		{
 			for (CardzuConfig::CardzuZuheSetIt it = zuhe_set->begin(); it != zuhe_set->end(); ++ it)
 			{
 				if (m_param.zuhe_level_list[*it] > 0)
 				{
 					++ active_zuhe_num;
 				}
 			}
 
 			int next_cardzu_need_active_zuhe_num = LOGIC_CONFIG->GetCardzuCfg().GetCardzuNeedActiveCount(next_cardzu_id);
 			if (active_zuhe_num < next_cardzu_need_active_zuhe_num)
 			{
 				break;
 			}
 
 			cur_active_max_cardzu_id = next_cardzu_id;
 		}
 	}
 
 	if (m_active_max_cardzu_id < cur_active_max_cardzu_id)
 	{
 		m_active_max_cardzu_id = cur_active_max_cardzu_id;
 
 		if(!is_init_check)	// 新激活卡组，发传闻
 		{
 			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_cardzu_active_content, 
 				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), m_active_max_cardzu_id);
 			if (length > 0) 
 			{
 				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
 			}
 		}
 	}
}

void Cardzu::OnCoinChouCard(int cardzu_id)
{
 	if (cardzu_id < 0 || cardzu_id >= CARDZU_TYPE_MAX_COUNT)
 	{
 		return;
 	}
 
 	const CardzuOtherConfig &other_cfg = LOGIC_CONFIG->GetCardzuCfg().GetOtherCfg();
 
 	int today_buy_time = m_param.today_coin_buy_card_times[cardzu_id];
 	if (today_buy_time >= other_cfg.coin_chouka_max_time)
 	{
 		m_role->NoticeNum(errornum::EN_CARDZU_CHOUKA_TIME_LIMIT);
 		return;
 	}
 
 	if (m_role->GetSublock()->CheckHasLock())
 	{
 		return;
 	}
 
 	if (m_role->GetKnapsack()->IsLock())
 	{
 		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
 		return;
 	}
 
 	if (!m_role->GetKnapsack()->GetMoney()->AllCoinMoreThan(other_cfg.choucard_need_coin))
 	{
 		m_role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
 		return;
 	}
 
 	const CardzuCardConfig *new_card = NULL;
 	if (m_param.coin_choucard_less_times[cardzu_id] >= 10) // 10次没抽到3星以上，强制抽一个三星以上的卡
 	{
 		new_card = LOGIC_CONFIG->GetCardzuCfg().GetRandomCard(CARDZU_CHOUCARD_TYPE_COIN, cardzu_id, 3, CARDZU_CARD_STAR_MAX_COUNT);
 	}
 	else
 	{
 		new_card = LOGIC_CONFIG->GetCardzuCfg().GetRandomCard(CARDZU_CHOUCARD_TYPE_COIN, cardzu_id, 1, CARDZU_CARD_STAR_MAX_COUNT);
 	}
 
 	if (NULL == new_card || new_card->card_id < 0 || new_card->card_id > CARDZU_MAX_CARD_ID)
 	{
 		return;
 	}
 
 	if (new_card->star_count < 3)
 	{
 		++ m_param.coin_choucard_less_times[cardzu_id];
 	}
 	else
 	{
 		m_param.coin_choucard_less_times[cardzu_id] = 0;
 	}
 
 	// 消耗
 	if (!m_role->GetKnapsack()->GetMoney()->UseCoin(other_cfg.choucard_need_coin, "CoinChouCard"))
 	{
 		m_role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
 		return;
 	}
 	++ m_param.today_coin_buy_card_times[cardzu_id];
 
 	// 生效
 	int old_lingli = m_param.lingli;
 	int old_card_num = m_param.card_count_list[new_card->card_id];
 
 	++ m_param.card_count_list[new_card->card_id];
 	m_param.lingli += other_cfg.coin_choucard_lingli;
 
 	m_role->NoticeNum(noticenum::NT_CARDZU_BUY_LINGLI_SUCC);
 
 	ChangeCardSet card_list;
 	card_list.insert(new_card->card_id);
 	this->OnNotifyChange(-1, &card_list);
 
 	gamelog::g_log_cardzu.printf(LL_INFO, "OnCoinChouCard user %d %s, cardzu_id %d, chou_card_id %d, old_card_num %d, new_card_num %d, old_lingli %d, new_lingli %d,",
 		m_role->GetUID(), m_role->GetName(), cardzu_id, new_card->card_id, old_card_num, m_param.card_count_list[new_card->card_id], old_lingli, m_param.lingli);
}

void Cardzu::OnGoldChouCard(int cardzu_id)
{
 	if (cardzu_id < 0 || cardzu_id >= CARDZU_TYPE_MAX_COUNT)
 	{
 		return;
 	}
 
 	const CardzuOtherConfig &other_cfg = LOGIC_CONFIG->GetCardzuCfg().GetOtherCfg();
 
 	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(other_cfg.choucard_need_gold))
 	{
 		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
 		return;
 	}
 
 	const CardzuCardConfig *new_card = NULL;
 	if (0 == m_param.has_first_gold_choucard[cardzu_id]) // 第一次元宝单抽，强制抽一个三星以上的卡
 	{
 		new_card = LOGIC_CONFIG->GetCardzuCfg().GetRandomCard(CARDZU_CHOUCARD_TYPE_GOLD, cardzu_id, 3, CARDZU_CARD_STAR_MAX_COUNT);
 	}
 	else
 	{
 		new_card = LOGIC_CONFIG->GetCardzuCfg().GetRandomCard(CARDZU_CHOUCARD_TYPE_GOLD, cardzu_id, 1, CARDZU_CARD_STAR_MAX_COUNT);
 	}
 
 	if (NULL == new_card || new_card->card_id < 0 || new_card->card_id > CARDZU_MAX_CARD_ID)
 	{
 		return;
 	}
 
 	// 消耗
 	if (!m_role->GetKnapsack()->GetMoney()->UseGold(other_cfg.choucard_need_gold, "OnGoldChouCard"))
 	{
 		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
 		return;
 	}
 
 	m_param.has_first_gold_choucard[cardzu_id] = 1;
 
 	// 生效
 	int old_lingli = m_param.lingli;
 	int old_card_num = m_param.card_count_list[new_card->card_id];
 
 	++ m_param.card_count_list[new_card->card_id];
 	m_param.lingli += other_cfg.gold_choucard_lingli;
 
 	ChangeCardSet card_list;
 	card_list.insert(new_card->card_id);
 	this->OnNotifyChange(-1, &card_list);
 
 	m_role->NoticeNum(noticenum::NT_CARDZU_BUY_LINGLI_SUCC);
 
 	gamelog::g_log_cardzu.printf(LL_INFO, "OnGoldChouCard user %d %s, cardzu_id %d, chou_card_id %d, old_card_num %d, new_card_num %d, old_lingli %d, new_lingli %d,",
 		m_role->GetUID(), m_role->GetName(), cardzu_id, new_card->card_id, old_card_num, m_param.card_count_list[new_card->card_id], old_lingli, m_param.lingli);
}

void Cardzu::OnGoldBatchChouCard(int cardzu_id)
{
 	if (cardzu_id < 0 || cardzu_id >= CARDZU_TYPE_MAX_COUNT)
 	{
 		return;
 	}
 
 	const CardzuOtherConfig &other_cfg = LOGIC_CONFIG->GetCardzuCfg().GetOtherCfg();

	int choucard_need_gold = other_cfg.choucard_batch_need_gold;
	int consume_item_id = other_cfg.choucard_batch_need_item_id;
 
 	int more_than_3star_num = 0; // 三颗星或以上的卡牌数，下同
 	int more_than_4star_num = 0;
 	int more_than_5star_num = 0;
 
 	std::vector<int> card_id_list;
 
 	for (int i = 0; i < 10; ++ i)
 	{
 		const CardzuCardConfig *new_card = NULL;
 
 		if (more_than_3star_num >= 5) // 至多5个3★以上
 		{
 			new_card = LOGIC_CONFIG->GetCardzuCfg().GetRandomCard(CARDZU_CHOUCARD_TYPE_GOLD_BATCH, cardzu_id, 1, 2);
 		}
 		else if (more_than_4star_num >= 3) // 至多3个4★以上
 		{
 			new_card = LOGIC_CONFIG->GetCardzuCfg().GetRandomCard(CARDZU_CHOUCARD_TYPE_GOLD_BATCH, cardzu_id, 1, 3);
 		}
 		else if (more_than_5star_num >= 2) // 至多2个5★以上
 		{
 			new_card = LOGIC_CONFIG->GetCardzuCfg().GetRandomCard(CARDZU_CHOUCARD_TYPE_GOLD_BATCH, cardzu_id, 1, 4);
 		}
 		else
 		{
 			if (9 == i && more_than_3star_num <= 0)
 			{
 				new_card = LOGIC_CONFIG->GetCardzuCfg().GetRandomCard(CARDZU_CHOUCARD_TYPE_GOLD_BATCH, cardzu_id, 3, CARDZU_CARD_STAR_MAX_COUNT);
 			}
 			else
 			{
 				new_card = LOGIC_CONFIG->GetCardzuCfg().GetRandomCard(CARDZU_CHOUCARD_TYPE_GOLD_BATCH, cardzu_id, 1, CARDZU_CARD_STAR_MAX_COUNT);
 			}
 		}
 
 		if (NULL == new_card)
 		{
 			return;
 		}
 
 		if (new_card->star_count >= 3) ++more_than_3star_num;
 		if (new_card->star_count >= 4) ++more_than_4star_num;
 		if (new_card->star_count >= 5) ++more_than_5star_num;
 
 		card_id_list.push_back(new_card->card_id);
 	}

	//消耗
	if (!m_role->GetKnapsack()->ConsumeItem(consume_item_id, 1, "OnItemBatchChouCard"))
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(choucard_need_gold, "OnGoldBatchChouCard"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}
 
 	// 生效
 	int old_lingli = m_param.lingli;
 	m_param.lingli += other_cfg.batch_gold_choucard_lingli;
 
 	ChangeCardSet change_card_id_list;
 	std::string card_idlist_str;
 
 	for (std::vector<int>::const_iterator it = card_id_list.begin(); it != card_id_list.end(); ++ it)
 	{
 		++ m_param.card_count_list[*it];
 		change_card_id_list.insert(*it);
 
 		static char tmp_str[32];
 		SNPRINTF(tmp_str, sizeof(tmp_str), "%d, ", *it);
 		card_idlist_str += tmp_str;
 	}
 
 	m_role->NoticeNum(noticenum::NT_CARDZU_BUY_LINGLI_SUCC);
 
 	this->OnNotifyChange(-1, &change_card_id_list);
 
 	gamelog::g_log_cardzu.printf(LL_INFO, "OnGoldBatchChouCard user %d %s, cardzu_id %d, old_lingli %d, new_lingli %d, chou_card_idlist [%s]",
 		m_role->GetUID(), m_role->GetName(), cardzu_id, old_lingli, m_param.lingli, card_idlist_str.c_str());
}

void Cardzu::OnGoldBindChouCard(int cardzu_id)
{
	if (cardzu_id < 0 || cardzu_id >= CARDZU_TYPE_MAX_COUNT)
	{
		return;
	}

	const CardzuOtherConfig &other_cfg = LOGIC_CONFIG->GetCardzuCfg().GetOtherCfg();

	int today_buy_time = m_param.today_gold_bind_buy_card_times[cardzu_id];
	if (today_buy_time >= other_cfg.gold_bind_chouka_max_time)
	{
		m_role->NoticeNum(errornum::EN_CARDZU_CHOUKA_TIME_LIMIT);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldBindMoreThan(other_cfg.choucard_need_gold))
	{
		m_role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
		return;
	}

	const CardzuCardConfig *new_card = NULL;
	if (0 == m_param.has_first_gold_bind_choucard[cardzu_id]) // 第一次绑钻单抽，强制抽一个三星以上的卡
	{
		new_card = LOGIC_CONFIG->GetCardzuCfg().GetRandomCard(CARDZU_CHOUCARD_TYPE_GOLD_BIND, cardzu_id, 3, CARDZU_CARD_STAR_MAX_COUNT);
	}
	else
	{
		new_card = LOGIC_CONFIG->GetCardzuCfg().GetRandomCard(CARDZU_CHOUCARD_TYPE_GOLD_BIND, cardzu_id, 1, CARDZU_CARD_STAR_MAX_COUNT);
	}

	if (NULL == new_card || new_card->card_id < 0 || new_card->card_id > CARDZU_MAX_CARD_ID)
	{
		return;
	}

	// 消耗
	if (!m_role->GetKnapsack()->GetMoney()->UseGoldBind(other_cfg.choucard_need_gold_bind, "OnGoldBindChouCard"))
	{
		m_role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
		return;
	}
	++ m_param.today_gold_bind_buy_card_times[cardzu_id];

	m_param.has_first_gold_bind_choucard[cardzu_id] = 1;

	// 生效
	int old_lingli = m_param.lingli;
	int old_card_num = m_param.card_count_list[new_card->card_id];

	++ m_param.card_count_list[new_card->card_id];
	m_param.lingli += other_cfg.gold_bind_choucard_lingli;

	ChangeCardSet card_list;
	card_list.insert(new_card->card_id);
	this->OnNotifyChange(-1, &card_list);

	m_role->NoticeNum(noticenum::NT_CARDZU_BUY_LINGLI_SUCC);

	gamelog::g_log_cardzu.printf(LL_INFO, "OnGoldBindChouCard user %d %s, cardzu_id %d, chou_card_id %d, old_card_num %d, new_card_num %d, old_lingli %d, new_lingli %d,",
		m_role->GetUID(), m_role->GetName(), cardzu_id, new_card->card_id, old_card_num, m_param.card_count_list[new_card->card_id], old_lingli, m_param.lingli);
}

