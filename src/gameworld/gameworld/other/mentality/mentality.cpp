#include "mentality.hpp"

#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/activitydef.hpp"
#include "gameworld/world.h"
#include "gameworld/gamelog.h"
#include "scene/scene.h"

#include "engineadapter.h"
#include "globalconfig/globalconfig.h"
#include "other/event/eventhandler.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectbuff.hpp"

#include "protocal/msgitem.h"

#include "config/logicconfigmanager.hpp"
#include "other/vip/vipconfig.hpp"
#include "config/mentalityconfig.hpp"
#include "config/shuxingdanconfig.hpp"
#include "item/knapsack.h"
#include "other/vip/vip.hpp"
#include "other/capability/capability.hpp"
#include "other/sublock/sublock.hpp"

Mentality::Mentality() 
	: m_role(NULL), m_train_cd_next_time(0), m_curr_trained_mentality_type(0), m_flag(0), mentality_total_level(0)
{
	memset(m_wuxing_level_list, 0, sizeof(m_wuxing_level_list));
	memset(m_shuxingdan_list, 0, sizeof(m_shuxingdan_list));
}

Mentality::~Mentality()
{

} 

void Mentality::Init(Role *role, const MentalityParam &param)
{
	m_role = role;

	m_train_cd_next_time = param.train_cd_next_time;
	m_curr_trained_mentality_type = param.curr_trained_mentality_type;
	m_flag = param.flag;
	
	memcpy(m_mentality_list, param.mentality_list, sizeof(m_mentality_list));
	memcpy(m_wuxing_level_list, param.wuxing_level_list, sizeof(m_wuxing_level_list));
	memcpy(m_shuxingdan_list, param.shuxingdan_list, sizeof(m_shuxingdan_list));
}

void Mentality::GetOtherInitParam(MentalityParam *param)
{
	param->train_cd_next_time = m_train_cd_next_time;
	param->curr_trained_mentality_type = m_curr_trained_mentality_type;
	param->flag = m_flag;

	memcpy(param->mentality_list, m_mentality_list, sizeof(param->mentality_list));
	memcpy(param->wuxing_level_list, m_wuxing_level_list, sizeof(param->wuxing_level_list));
	memcpy(param->shuxingdan_list, m_shuxingdan_list, sizeof(param->shuxingdan_list));
}

void Mentality::GetMentalityInfo(MentalityItem mentality_list[MENTALITY_TYPE_MAX])
{
	for (int i = 0; i < MENTALITY_TYPE_MAX; ++i)
	{
		mentality_list[i] = m_mentality_list[i];
	}
}

void Mentality::SendMentalityInfo()	
{
 	static Protocol::SCMentalityList ml;
 	ml.cd_next_time = m_train_cd_next_time;
 	ml.curr_train_type = m_curr_trained_mentality_type;
 	ml.flag = m_flag;
 	ml.reserve_sh = 0;
 	memcpy(ml.mentality_list, m_mentality_list, sizeof(ml.mentality_list));
 	memcpy(ml.wuxing_level_list, m_wuxing_level_list, sizeof(ml.wuxing_level_list));
 	memcpy(ml.shuxingdan_list, m_shuxingdan_list, sizeof(ml.shuxingdan_list));
 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&ml, sizeof(ml));
}

void Mentality::Update(unsigned int now_second)
{
 	if (0 != m_train_cd_next_time && now_second >= m_train_cd_next_time)
 	{
 		m_train_cd_next_time = 0;
 
 		this->SendMentalityInfo();
 	}
}

void Mentality::ClearTrainMentalityCD()
{
 	if (m_curr_trained_mentality_type < 0 || m_curr_trained_mentality_type >= MENTALITY_TYPE_MAX) return;
 	if (m_mentality_list[m_curr_trained_mentality_type].mentality_level >= MAX_MENTALITY_BASE_LEVEL) return;
 
 	const MentalityBaseItemConfig *mbic = LOGIC_CONFIG->GetMentalityConfig().GetTrainMentalityBaseItem(m_mentality_list[m_curr_trained_mentality_type].mentality_level);
 	if (NULL == mbic) 
 	{
 		return;
 	}
 
 	if (m_role->GetLevel() < mbic->rolelevel_limit)
 	{
 		m_role->NoticeNum(errornum::EN_MENTYALITY_LEVEL_LIMIT);
 		return;
 	}
 
 	if (0 != (m_flag & MentalityParam::FLAG_CLEAR_CD))
 	{
 		m_role->NoticeNum(errornum::EN_MENTALITY_NO_NEED_CLEAR_CD);
 		return;
 	}
 
 	if (EngineAdapter::Instance().Time() >= m_train_cd_next_time)
 	{
 		m_role->NoticeNum(errornum::EN_MENTALITY_NO_NEED_CLEAR_CD);
 		return;
 	}
 
 	if (!m_role->GetKnapsack()->GetMoney()->UseGold(LOGIC_CONFIG->GetMentalityConfig().GetOtherCfg().clear_cd_cost, "ClearMentalityCD"))
 	{
 		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
 		return;
 	}
 
 	m_train_cd_next_time = 0;
 
 	this->SendMentalityInfo();
}

void Mentality::TrainMentality()
{
 	if (m_role->GetSublock()->CheckHasLock()) return;
 
 	if (m_curr_trained_mentality_type < 0 || m_curr_trained_mentality_type >= MENTALITY_TYPE_MAX) return;
 	if (0 != m_train_cd_next_time) return;
 	if (m_mentality_list[m_curr_trained_mentality_type].mentality_level >= MAX_MENTALITY_BASE_LEVEL) return;
 
 	const MentalityBaseItemConfig *mbic = LOGIC_CONFIG->GetMentalityConfig().GetTrainMentalityBaseItem(m_mentality_list[m_curr_trained_mentality_type].mentality_level);
 	if (NULL == mbic) return;
 
 	{
 		// 检查
 		if (m_role->GetLevel() < mbic->rolelevel_limit)
 		{
 			m_role->NoticeNum(errornum::EN_MENTYALITY_LEVEL_LIMIT);
 			return;
 		}
 
 		if (m_role->GetYuanli() < mbic->need_yuanli)
 		{
 			m_role->NoticeItemLack(ZHENQI_VIRTUAL_ITEM_ID, mbic->need_yuanli - m_role->GetYuanli());
 			return;
 		}
 	}
 	
 	{
 		// 消耗
 		if (!m_role->AddYuanli(-mbic->need_yuanli, "MentalityTrain")) return;
 
 		if (0 == (m_flag & MentalityParam::FLAG_CLEAR_CD))
 		{
 			m_train_cd_next_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + mbic->cd_time; 
 		}
 		else
 		{
 			m_train_cd_next_time = 0; 
 		}
 	}
 
 	{
 		++ m_mentality_list[m_curr_trained_mentality_type].mentality_level;
 
 		UNSTD_STATIC_CHECK(MENTALITY_TYPE_MAX > 0);
 		UNSTD_STATIC_CHECK(MAX_MENTALITY_BASE_LEVEL / MENTALITY_TYPE_MAX > 0);
 		if (MENTALITY_TYPE_MAX - 1 == m_curr_trained_mentality_type && (m_mentality_list[MENTALITY_TYPE_MAX - 1].mentality_level % 10 == 0))
 		{
 			int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mentality_upgrade_max, UidToInt(m_role->GetUserId()), m_role->GetName(), 
 				m_role->GetCamp(), m_mentality_list[MENTALITY_TYPE_MAX - 1].mentality_level, m_mentality_list[MENTALITY_TYPE_MAX - 1].mentality_level);
 			if (str_len > 0)
 			{
 				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_CENTER_NOTICE_3);
 			}
 		}
 
 		m_curr_trained_mentality_type = (m_curr_trained_mentality_type + 1) % MENTALITY_TYPE_MAX;
 
 		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MENTALITY);
 	}
 
 	this->SendMentalityInfo();
 
 	//EventHandler::Instance().OnTrainMentality(m_role, 1);
 
 	gamelog::g_log_mentality.printf(LL_INFO, "[Mentality] Train Mentality: [user_id:%d user_name:%s role_level[%d], cur_mentality_type:%d, cur_mentality_level:%d]",
 		UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetLevel(), m_curr_trained_mentality_type, m_mentality_list[m_curr_trained_mentality_type].mentality_level);
}

void Mentality::ClearMentalityCD()
{
 	int vip_level = m_role->GetVip()->GetVipLevel();
 	int can_buy = LOGIC_CONFIG->GetVipConfig().GetAuthParam(vip_level, VAT_FB_MENTALITY_CLEAN_CD);
 	
 	if (0 == can_buy)
 	{
 		m_role->NoticeNum(errornum::EN_VIP_LEVEL_NOT_ENOUGH);
 		return;
 	}
 
 	if (0 != (m_flag & MentalityParam::FLAG_CLEAR_CD))
 	{
 		return;
 	}
 
 	const int cd_interval_per_gold = GLOBALCONFIG->GetClearMentalityCDIntervalPerGold();
 	if (cd_interval_per_gold <= 0) return;
 
 	if (!m_role->GetKnapsack()->GetMoney()->UseGold(cd_interval_per_gold, "MentalityClearCD"))
 	{
 		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
 		return;
 	}
 
 	m_flag |= MentalityParam::FLAG_CLEAR_CD;
 	m_train_cd_next_time = 0;
 
 	this->SendMentalityInfo();
 
 	m_role->NoticeNum(noticenum::NT_MENTALITY_SPEED_SUCC);
 
 	gamelog::g_log_mentality.printf(LL_INFO, "[Mentality] ClearMentalityCD [user_id:%d user_name:%s consume_gold:%d]",
 		m_role->GetUID(), m_role->GetName(), cd_interval_per_gold);
}

void Mentality::UpgradeMentalityGengu(int type, bool is_auto_buy, bool use_protect_item)
{
 	if (m_role->GetSublock()->CheckHasLock()) return;
 
 	if (type < 0 || type >= MENTALITY_TYPE_MAX || m_mentality_list[type].gengu_level >= MAX_MENTALITY_GENGU_LEVEL) return;
 	
//  	if (m_mentality_list[type].Invalid())
//  	{
//  		m_role->NoticeNum(errornum::EN_MENTALITY_MENTALITY_LEVEL_ZERO);
//  		return;
//  	}
//  
 	Knapsack *knapsack = m_role->GetKnapsack();
 	if (knapsack->IsLock())
 	{
 		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
 		return;
 	}
 
 	const MentalityGenguItemConfig *mgic = LOGIC_CONFIG->GetMentalityConfig().GetUpgradeMentalityGenguItem(m_mentality_list[type].gengu_level);
 	if (NULL == mgic) return;

	if (use_protect_item && (mgic->protect_item_id <= 0 || mgic->protect_item_num <= 0))
	{
		m_role->NoticeNum(errornum::EN_MENTALITY_NOT_NEED_PROTECT_ITEM);
		return;
	}
 
 	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
 	{
 		// 获取可消耗的背包材料列表
 		short stuff_count = 0;
 
 		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
 		stuff_list[stuff_count].item_id = mgic->upgrade_itemid;
 		stuff_list[stuff_count].num = 1;
 		stuff_list[stuff_count].buyable = is_auto_buy;
 		++ stuff_count;
 
 		if (use_protect_item)
 		{
 			stuff_list[stuff_count].item_id = mgic->protect_item_id;
 			stuff_list[stuff_count].num = mgic->protect_item_num;
 			stuff_list[stuff_count].buyable = false;
 			++ stuff_count;
 		}
 		
 		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
 		{
 			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
 			return;
 		}
 	}

	if (!knapsack->GetMoney()->AllCoinMoreThan(mgic->need_coin))
	{
		m_role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
		return;
	}

	// 消耗
	if (mgic->need_coin > 0)
	{
		if (!knapsack->GetMoney()->UseAllCoin(mgic->need_coin, "MentalityUpgradeGengu")) return;
	}

	if (!ItemExtern::ConsumeItem(m_role, consume_list, "Mentality::UpgradeMentalityGengu"))
	{
		return;
	}
 
 	//bool is_level_up = false;
 
 	{
 		// 给予
 		int succ_add_percent = 0;
 
 		// 检查祝福
 
 		int succ_rate = mgic->succ_rate * (100 + succ_add_percent) / 100;
 
 		if (RandomNum(100) < succ_rate)
 		{
 			//is_level_up = true;
 
 			bool is_before_upgrade_boardcast = LOGIC_CONFIG->GetMentalityConfig().IsGenguLevelBoardcast(this->GetMinGenguLevel());
 
 			m_mentality_list[type].gengu_level ++;
 
 			if (m_mentality_list[type].gengu_max_level < m_mentality_list[type].gengu_level)
 			{
 				m_mentality_list[type].gengu_max_level = m_mentality_list[type].gengu_level;
 
 				if (m_mentality_list[type].gengu_max_level >= 10)
 				{
 					// 单个根骨传闻
 					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_succnotice_mentility_lift, 
 						m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), type, (int)m_mentality_list[type].gengu_level);
 					if (length > 0) 
 					{
 						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
 					}
 				}
 
 				{
 					// 全身根骨传闻 
 					bool is_after_upgrade_boardcast = LOGIC_CONFIG->GetMentalityConfig().IsGenguLevelBoardcast(this->GetMinGenguLevel());
 					if (!is_before_upgrade_boardcast && is_after_upgrade_boardcast)
 					{
 						int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_gengu_broadcast, UidToInt(m_role->GetUserId()), m_role->GetName(), 
 							m_role->GetCamp(), m_mentality_list[type].gengu_max_level, m_mentality_list[type].gengu_max_level);
 						if (str_len > 0)
 						{
 							if (m_mentality_list[type].gengu_max_level >= 5)
 							{
 								World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_CENTER_NOTICE_3);
 							}
 							else
 							{
 								World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_CENTER_NOTICE_3);
 							}
 						}
 					}
 				}
 			}
 		}
 		else
 		{
 			if (!use_protect_item && m_mentality_list[type].gengu_level > 0)
 			{
 				if (m_mentality_list[type].gengu_level > mgic->protect_level)
 				{
 					-- m_mentality_list[type].gengu_level;
 				}
 			}
 
 			m_role->NoticeNum(errornum::EN_MENTALITY_UPGRADE_GENGU_FAIL);
 		}
 	}
 
 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MENTALITY);
 
 	//EventHandler::Instance().OnMentalityGenguLevelUp(m_role, is_level_up);
 
 	this->SendMentalityInfo();
 
 	gamelog::g_log_mentality.printf(LL_INFO, "[Mentality] MentalityUpgradeGengu: [user_id:%d user_name:%s role_level:%d, gengu_type:%d, gengu_level:%d, gengu_max_level:%d, consume_gold:%d]",
 		UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetLevel(), type, m_mentality_list[type].gengu_level, m_mentality_list[type].gengu_max_level, consume_list.need_gold_num);
}

void Mentality::YijianTishengMentality()
{
 	if (m_role->GetSublock()->CheckHasLock()) return;
 
 	if (0 == (m_flag & MentalityParam::FLAG_CLEAR_CD))	// 只有清除了CD才能
 	{
 		return;
 	}
 
 	int mentality_upgrade_max_count = MAX_MENTALITY_UPGRADE_ONCE - m_curr_trained_mentality_type;
 	
 	int round_count = 0, total_yuanli = 0;
 	while (round_count < mentality_upgrade_max_count)
 	{
 		if (m_curr_trained_mentality_type < 0 || m_curr_trained_mentality_type >= MENTALITY_TYPE_MAX) break;
 		if (m_mentality_list[m_curr_trained_mentality_type].mentality_level >= MAX_MENTALITY_BASE_LEVEL) break;
 
 		const MentalityBaseItemConfig *mbic = LOGIC_CONFIG->GetMentalityConfig().GetTrainMentalityBaseItem(m_mentality_list[m_curr_trained_mentality_type].mentality_level);
 		if (NULL == mbic) break;
 
 		if (m_role->GetLevel() < mbic->rolelevel_limit)
 		{
 			if (round_count <= 0) m_role->NoticeNum(errornum::EN_MENTYALITY_LEVEL_LIMIT);
 			break;
 		}
 
 		int need_yuanli = mbic->need_yuanli;
 
 		if (m_role->GetYuanli() < total_yuanli + need_yuanli)
 		{
 			if (round_count <= 0) 
 			{
 				m_role->NoticeItemLack(ZHENQI_VIRTUAL_ITEM_ID, total_yuanli + need_yuanli - m_role->GetYuanli());
 			}
 			break;
 		}
 
 		total_yuanli += need_yuanli;
 
 		++ m_mentality_list[m_curr_trained_mentality_type].mentality_level;
 
 		UNSTD_STATIC_CHECK(MENTALITY_TYPE_MAX > 0);
 		UNSTD_STATIC_CHECK(MAX_MENTALITY_BASE_LEVEL / MENTALITY_TYPE_MAX > 0);
 		if (MENTALITY_TYPE_MAX - 1 == m_curr_trained_mentality_type && (m_mentality_list[MENTALITY_TYPE_MAX - 1].mentality_level % 20 == 0))
 		{
 			int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_mentality_upgrade_max, UidToInt(m_role->GetUserId()), m_role->GetName(), 
 				m_role->GetCamp(), m_mentality_list[MENTALITY_TYPE_MAX - 1].mentality_level, m_mentality_list[MENTALITY_TYPE_MAX - 1].mentality_level);
 			if (str_len > 0)
 			{
 				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_CENTER_NOTICE_3);
 			}
 		}
 
 		m_curr_trained_mentality_type = (m_curr_trained_mentality_type + 1) % MENTALITY_TYPE_MAX;
 
 		++ round_count;
 	}
 
 	if (total_yuanli > 0) 
 	{
 		m_train_cd_next_time = 0; 
 
 		m_role->AddYuanli(-total_yuanli, "YijianTishengMentality");
 	}
 
 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MENTALITY);
 
 	//EventHandler::Instance().OnTrainMentality(m_role, round_count);
 
 	Protocol::SCMentalityYijianTishengAck yjts_ack;
 	yjts_ack.tisheng_count = (mentality_upgrade_max_count == MAX_MENTALITY_UPGRADE_ONCE) ? round_count : round_count - 1;
 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&yjts_ack, sizeof(Protocol::SCMentalityYijianTishengAck));
 
 	this->SendMentalityInfo();
 
 	gamelog::g_log_mentality.printf(LL_INFO, "[Mentality] YijianTishengMentality: [user_id:%d user_name:%s role_level:%d cur_mentality_type:%d, cur_mentality_level:%d]",
 		UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetLevel(), m_curr_trained_mentality_type, m_mentality_list[m_curr_trained_mentality_type].mentality_level);
}

void Mentality::OnMentalityUplevelWuxing(int wx_index, bool is_auto_buy)
{
// 	if (wx_index < 0 || wx_index >= MENTALITY_WUXING_MAX_COUNT)
// 	{
// 		return;
// 	}
// 
// 	if (m_role->GetSublock()->CheckHasLock())
// 	{
// 		return;
// 	}
// 
// 	if (m_role->GetKnapsack()->IsLock())
// 	{
// 		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
// 		return;
// 	}
// 
// 	int old_wx_level = m_wuxing_level_list[wx_index];
// 
// 	if (old_wx_level >= MENTALITY_WUXING_MAX_LEVEL)
// 	{
// 		m_role->NoticeNum(errornum::EN_MENTALITY_WUXING_LEVEL_LIMIT);
// 		return;
// 	}
// 
// 	const MentalityWuxingConfig *wx_cfg = LOGIC_CONFIG->GetMentalityConfig().GetWuxingCfg(wx_index, old_wx_level);
// 	if (NULL == wx_cfg)
// 	{
// 		return;
// 	}
// 
// 	if (mentality_total_level < wx_cfg->need_total_mentality_level)
// 	{
// 		m_role->NoticeNum(errornum::EN_MENTALITY_WUXING_JINGMAI_LIMIT);
// 		return;
// 	}
// 
// 	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
// 	{
// 		// 检查
// 		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
// 		stuff_list[0].item_id = wx_cfg->uplevel_stuff_id;
// 		stuff_list[0].num = wx_cfg->uplevel_stuff_num;
// 		stuff_list[0].buyable = is_auto_buy;
// 
// 		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
// 		{
// 			return;
// 		}
// 
// 		if (consume_list.count <= 0 && consume_list.need_gold_num <= 0) 
// 		{
// 			return;
// 		}
// 
// 		if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(consume_list.need_gold_num))
// 		{
// 			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 			return;
// 		}
// 	}
// 
// 	{
// 		// 消耗
// 		if (consume_list.count > 0 && !m_role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "MentalityWuxingUplevel")) 
// 		{
// 			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
// 			return;
// 		}
// 
// 		long long consume_gold = 0, consume_gold_bind = 0;
// 		if (consume_list.need_gold_num > 0 && !m_role->GetKnapsack()->GetMoney()->UseAllGold(consume_list.need_gold_num, "MentalityWuxingUplevel", true, &consume_gold, &consume_gold_bind))
// 		{
// 			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 			return;
// 		}
// 
// 		EventHandler::Instance().OnShopBuyItem(m_role, consume_list.buy_count, consume_list.buy_list, consume_gold, consume_gold_bind);
// 	}
// 
// 	{
// 		// 结算
// 		int succ_rand_val = RandomNum(100);
// 		if (succ_rand_val < wx_cfg->succ_rate)
// 		{
// 			++ m_wuxing_level_list[wx_index];
// 			m_role->NoticeNum(noticenum::NT_MENTALITY_WUXING_UPLEVEL_SUCC);
// 		}
// 		else
// 		{
// 			int reduce_rand_val = RandomNum(100);
// 			if  (m_wuxing_level_list[wx_index] > 0 && reduce_rand_val < wx_cfg->reduce_rate)
// 			{
// 				-- m_wuxing_level_list[wx_index];
// 			}
// 
// 			m_role->NoticeNum(noticenum::NT_MENTALITY_WUXING_UPLEVEL_FAIL);
// 		}
// 	}
// 
// 	if (old_wx_level != m_wuxing_level_list[wx_index])
// 	{
// 		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MENTALITY);
// 	}
// 
// 	this->SendMentalityInfo();
// 
// 	gamelog::g_log_mentality.printf(LL_INFO, "[Mentality] OnMentalityUplevelWuxing : [user_id:%d user_name:%s role_level[%d], wuxing_type[%d], old_level[%d], new_level[%d]",
// 		UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetLevel(), wx_index, old_wx_level, m_wuxing_level_list[wx_index]);
}

void Mentality::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attr_temp.Reset();

		int mentality_min_level = MAX_MENTALITY_BASE_LEVEL, gengu_min_level = MAX_MENTALITY_GENGU_LEVEL;
		mentality_total_level = 0;

		for (int i = 0; i < MENTALITY_TYPE_MAX; i++)
		{
			if (!m_mentality_list[i].Invalid())
			{
				if (mentality_min_level > m_mentality_list[i].mentality_level)
				{
					mentality_min_level = m_mentality_list[i].mentality_level;
				}

				mentality_total_level += m_mentality_list[i].mentality_level;

				LOGIC_CONFIG->GetMentalityConfig().GetMentalityBaseAttr(i, m_mentality_list[i].mentality_level, m_attr_temp, m_role->GetProf());
			}
			else
			{
				mentality_min_level = 0;
			}

			if (!m_mentality_list[i].GenguInvalid())
			{
				if (gengu_min_level > m_mentality_list[i].gengu_max_level)
				{
					gengu_min_level = m_mentality_list[i].gengu_max_level;
				}

				LOGIC_CONFIG->GetMentalityConfig().GetGenguBaseAttr(i, m_mentality_list[i].gengu_max_level, m_attr_temp, m_role->GetProf());
			}
			else
			{
				gengu_min_level = 0;
			}
		}

		LOGIC_CONFIG->GetMentalityConfig().GetMentalityTitleAttr(m_attr_temp, mentality_min_level, m_role->GetProf());
		LOGIC_CONFIG->GetMentalityConfig().GetGenguTitleAttr(m_attr_temp, gengu_min_level, m_role->GetProf());

// 		for (int wx_index = 0; wx_index < MENTALITY_WUXING_MAX_COUNT; ++ wx_index)
// 		{
// 			const static int WUXING_COUNT_PER_MENTALITY_TYPE = 5;	// 每种根骨有5个五行球
// 
// 			// 五行属性加成
// 			UNSTD_STATIC_CHECK(35 == MENTALITY_WUXING_MAX_COUNT);
// 			UNSTD_STATIC_CHECK(7 == MENTALITY_TYPE_MAX);
// 			UNSTD_STATIC_CHECK(WUXING_COUNT_PER_MENTALITY_TYPE == (MENTALITY_WUXING_MAX_COUNT / MENTALITY_TYPE_MAX));
// 
// 			const static int wuxing_index_attr_type_map[MENTALITY_TYPE_MAX] = 
// 			{
// 				CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU,
// 				CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG,
// 				CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI,
// 				CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN,
// 				CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI,
// 				CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP,
// 				CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI,
// 			};
// 
// 			const MentalityWuxingConfig *wx_cfg = LOGIC_CONFIG->GetMentalityConfig().GetWuxingCfg(wx_index, m_wuxing_level_list[wx_index]);
// 			if (NULL != wx_cfg && wx_cfg->attr_val > 0)
// 			{
// 				int attr_index = wx_index / WUXING_COUNT_PER_MENTALITY_TYPE;
// 				if (attr_index >= 0 && attr_index < MENTALITY_TYPE_MAX)
// 				{
// 					int attr_type = wuxing_index_attr_type_map[attr_index];
// 
// 					m_attr_temp.m_attrs[attr_type] += wx_cfg->attr_val;
// 				}
// 			}
// 		}

		// 属性丹
		for (int i = 0; i < MENTALITY_SHUXINGDAN_MAX_TYPE; ++ i)
 		{
			const int num = m_shuxingdan_list[i];
 			if (num > 0)
 			{
 				const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_XIULIAN, i);
 
 				if (NULL != cfg_item)
 				{
 					m_attr_temp.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp * num;
 					m_attr_temp.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji * num;
 					m_attr_temp.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu * num;
 					m_attr_temp.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg_item->mingzhong * num;
 					m_attr_temp.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg_item->shanbi * num;
 				}
 			}
 		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_MENTALITY, m_attr_temp);
	}

	base_add.Add(m_attr_temp.m_attrs, true);
}

int Mentality::GetTotalGenguMaxLevel()
{
	int total_gengu_max_level = 0;

	for (int i = 0; i < MENTALITY_TYPE_MAX; i++)
	{
		total_gengu_max_level +=  m_mentality_list[i].gengu_max_level;
	}

	return total_gengu_max_level;
}

int Mentality::GetMinGenguLevel()
{
	int min_level = MAX_MENTALITY_GENGU_LEVEL;

	for (int i = 0; i < MENTALITY_TYPE_MAX; i++)
	{
		if (m_mentality_list[i].gengu_max_level < min_level)
		{
			min_level = m_mentality_list[i].gengu_max_level;
		}
	}

	return min_level;
}

int Mentality::GetMinMentalityBaseLevel()
{
	int min_level = MAX_MENTALITY_BASE_LEVEL;

	for (int i = 0; i < MENTALITY_TYPE_MAX; i++)
	{
		if (m_mentality_list[i].mentality_level < min_level)
		{
			min_level = m_mentality_list[i].mentality_level;
		}
	}

	return min_level;
}

int Mentality::GMUpgradeGenguTest(int type, int target_level, int *coin_cost, int *gold_cost)
{
	return -1;
}

void Mentality::GMResetGengu(int type, int level)
{
	if (type < 0 || type >= MENTALITY_TYPE_MAX) return;

	m_mentality_list[type].gengu_level = level;
	m_mentality_list[type].gengu_max_level = level;
}

void Mentality::GMSetMentalityLevel(int level)
{
	for (int i = 0; i < MENTALITY_TYPE_MAX; ++i)
	{
		m_mentality_list[i].mentality_level = level;
	}

	this->SendMentalityInfo();
}


bool Mentality::OnUseShuxingdan(int slot_idx, int use_count)
{
 	if (slot_idx < 0 || slot_idx >= MENTALITY_SHUXINGDAN_MAX_TYPE || use_count <= 0) 
 	{
 		return false;
 	}
 
 	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_XIULIAN, slot_idx);
 	if (NULL == cfg_item)
 	{
 		return false;
 	}
 
 	if (m_shuxingdan_list[slot_idx] + use_count > cfg_item->use_limit_num)
 	{
 		m_role->NoticeNum(errornum::EN_SHUXINGDAN_USE_LIMIT_NUM);
 		return false;
 	}
 
 	m_shuxingdan_list[slot_idx] += use_count;
 
 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MENTALITY);
 	this->SendMentalityInfo();
 
 	return true;
}

