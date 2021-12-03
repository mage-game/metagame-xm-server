#include "roleworldevent.hpp"
#include "servercommon/worldeventdef.hpp"
#include "protocal/msgworldevent.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "global/worldevent/worldeventmanager.hpp"
#include "config/logicconfigmanager.hpp"
#include "global/worldevent/worldeventconfig.hpp"
#include "item/itemextern.hpp"
#include "servercommon/errornum.h"
#include "item/itempool.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"

RoleWorldEvent::RoleWorldEvent() : m_role(NULL), m_light_word_flag(0), m_find_npc_flag(0), m_curr_find_npc_id(0), m_fetch_reward_flag(0)
{
	memset(m_score_list, 0, sizeof(m_score_list));
}

RoleWorldEvent::~RoleWorldEvent()
{

}

void RoleWorldEvent::Init(Role *role, int fetch_reward_flag, int find_npc_score, int kill_rand_boss_score, int shenmozhixi_score, int xukongzhimen_score, 
						  int quanmintafang_score, int jiqingpk_score, int tiannvsanhua_score, short find_npc_flag, char light_world_flag,  char curr_find_npc_id)
{
	m_role = role;

	m_fetch_reward_flag = fetch_reward_flag;
	m_find_npc_flag = find_npc_flag;
	m_light_word_flag = light_world_flag;
	m_curr_find_npc_id = curr_find_npc_id;

	if (m_curr_find_npc_id <= 0)
	{
		LOGIC_CONFIG->GetWorldEventCfg().RandFindNpcId(m_find_npc_flag, &m_curr_find_npc_id);
	}

	m_score_list[WORLD_EVENT_TYPE_FIND_NPC] = find_npc_score;
	m_score_list[WORLD_EVENT_TYPE_KILL_RANDOM_BOSS] = kill_rand_boss_score;
	m_score_list[WORLD_EVENT_TYPE_SHENMOZHIXI] = shenmozhixi_score;
	m_score_list[WORLD_EVENT_TYPE_XUKONGZHIMEN] = xukongzhimen_score;
	m_score_list[WORLD_EVENT_TYPE_QUANMINTAFANG] = quanmintafang_score;
	m_score_list[WORLD_EVENT_TYPE_JIQINGPK] = jiqingpk_score;
	m_score_list[WORLD_EVENT_TYPE_TIANNVSANHUA] = tiannvsanhua_score;
}

void RoleWorldEvent::GetInitParam(int *fetch_reward_flag, int *find_npc_score, int *kill_rand_boss_score, int *shenmozhixi_score, int *xukongzhimen_score, 
								  int *quanmintafang_score, int *jiqingpk_score, int *tiannvsanhua_score, short *find_npc_flag, char *light_word_flag, char *curr_find_npc_id)
{
	*fetch_reward_flag = m_fetch_reward_flag;
	*find_npc_flag = m_find_npc_flag;
	*light_word_flag = m_light_word_flag;
	*curr_find_npc_id = m_curr_find_npc_id;

	*find_npc_score = m_score_list[WORLD_EVENT_TYPE_FIND_NPC];
	*kill_rand_boss_score = m_score_list[WORLD_EVENT_TYPE_KILL_RANDOM_BOSS];
	*shenmozhixi_score = m_score_list[WORLD_EVENT_TYPE_SHENMOZHIXI];
	*xukongzhimen_score = m_score_list[WORLD_EVENT_TYPE_XUKONGZHIMEN];
	*quanmintafang_score = m_score_list[WORLD_EVENT_TYPE_QUANMINTAFANG];
	*jiqingpk_score = m_score_list[WORLD_EVENT_TYPE_JIQINGPK];
	*tiannvsanhua_score = m_score_list[WORLD_EVENT_TYPE_TIANNVSANHUA];
}

void RoleWorldEvent::OnDayChange(int old_dayid, int now_dayid)
{
// 	if (old_dayid != now_dayid)
// 	{
// 		int curr_event_type = WorldEventManager::Instance().GetEventType();
// 
// 		for(int event_type = WORLD_EVENT_TYPE_INVALID + 1; event_type < curr_event_type; ++ event_type)
// 		{
// 			const WorldEventRewardConfig::ConfigItem *reward_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetRewardCfg(event_type);
// 			if (NULL == reward_cfg)
// 			{
// 				continue;
// 			}
// 
// 			int curr_role_score = m_score_list[event_type];
// 			int reward_factor = 0;
// 			int curr_reward_flag = 0;
// 
// 			for (int reward_idx = 0; reward_idx < WORLD_EVENT_REWARD_GRADE_COUNT; ++ reward_idx)
// 			{
// 				int check_reward_flag_idx = event_type * WORLD_EVENT_REWARD_GRADE_COUNT + reward_idx;
// 
// 				if (0 == (m_fetch_reward_flag & (1 << check_reward_flag_idx)))
// 				{
// 					if (curr_role_score >= reward_cfg->need_score_list[reward_idx] && reward_cfg->reward_factor_list[reward_idx] > 0)
// 					{
// 						curr_reward_flag |= (1 << check_reward_flag_idx);
// 						reward_factor += reward_cfg->reward_factor_list[reward_idx];
// 					}
// 				}
// 			}
// 
// 			if (0 != reward_factor && 0 != curr_reward_flag)
// 			{
// 				const WorldEventTypeConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetWorldEventCfg().GetEventTypeCfg(event_type);
// 				if (NULL != cfg_item)
// 				{
// 					// 奖励
// 					int reward_coin = static_cast<int>(cfg_item->base_coin * reward_factor);
// 					int reward_xianhun = static_cast<int>(cfg_item->base_xianhun * reward_factor);
// 					int reward_zhenqi = static_cast<int>(cfg_item->base_zhenqi * reward_factor);
// 
// 					ItemConfigData reward_item = cfg_item->base_item;
// 					reward_item.num = static_cast<int>(reward_item.num * reward_factor);
// 
// 					static MailContentParam contentparam; contentparam.Reset();
// 					contentparam.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_BIND_COIN] = reward_coin;
// 					contentparam.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_XIANHUN] = reward_xianhun;
// 					contentparam.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_YUANLI] = reward_zhenqi;
// 
// 					const ItemBase *item_base = ITEMPOOL->GetItem(reward_item.item_id);
// 					if (NULL != item_base)
// 					{
// 						if (reward_item.num > item_base->GetPileLimit())
// 						{
// 							reward_item.num = item_base->GetPileLimit();
// 						}
// 
// 						contentparam.item_list[0].item_id = reward_item.item_id;
// 						contentparam.item_list[0].num = reward_item.num;
// 						contentparam.item_list[0].is_bind = reward_item.is_bind;
// 						contentparam.item_list[0].invalid_time = item_base->CalInvalidTime();	
// 					}
// 
// 					int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_world_event_event_reward_content);
// 					if (length > 0)
// 					{
// 						MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
// 
// 						int old_reward_flag = m_fetch_reward_flag;
// 
// 						m_fetch_reward_flag |= curr_reward_flag;
// 
// 						{
// 							//日志
// 							gamelog::g_log_world_event.printf(LL_INFO, "OnResetData::user[%d, %s], event_type[%d], old_reward_flag[%d], new_reward_flag[%d]"
// 								"reward_coin[%d], reward_xianhun[%d], reward_zhenqi[%d], reward_item[%s]", 
// 								m_role->GetUID(), m_role->GetName(), event_type, old_reward_flag, m_fetch_reward_flag, 
// 								reward_coin, reward_xianhun, reward_zhenqi, ItemConfigDataLog(&reward_item, NULL));
// 
// 							ROLE_LOG_QUICK10(LOG_TYPE_WORLD_EVENT_REWARD, m_role, m_role->GetLevel(), curr_event_type, NULL, NULL, reward_item.item_id, reward_item.num, 0, 0);
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
}

void RoleWorldEvent::AddScore(int event_type, int score)
{
// 	if (event_type <= WORLD_EVENT_TYPE_INVALID || event_type >= WORLD_EVENT_TYPE_MAX || score <= 0)
// 	{
// 		return;
// 	}
// 
// 	long long tmp_score = m_score_list[event_type] + score;
// 
// 	if (tmp_score < 0) tmp_score = 0;
// 	if (tmp_score > MAX_WORLD_EVENT_SCORE) tmp_score = MAX_WORLD_EVENT_SCORE;
// 
// 	m_score_list[event_type] = static_cast<int>(tmp_score);
// 	WorldEventManager::Instance().AddScore(event_type, score);
// 
// 	this->SendInfo();
}

void RoleWorldEvent::SetFindNpc(int npc_index) 
{
// 	m_find_npc_flag |= (1 << npc_index);
// 
// 	LOGIC_CONFIG->GetWorldEventCfg().RandFindNpcId(m_find_npc_flag, &m_curr_find_npc_id);
// 
// 	this->SendInfo();
// 	WorldEventManager::Instance().GetNpcLocateInfo(m_role, m_curr_find_npc_id);
}

void RoleWorldEvent::FetchReward()
{
// 	int curr_event_type = WorldEventManager::Instance().GetEventType();
// 	if (curr_event_type <= WORLD_EVENT_TYPE_INVALID || curr_event_type >= WORLD_EVENT_TYPE_MAX)
// 	{
// 		return;
// 	}
// 
// 	UNSTD_STATIC_CHECK(WORLD_EVENT_TYPE_MAX * WORLD_EVENT_REWARD_GRADE_COUNT <= sizeof(m_fetch_reward_flag) * 8);
// 
// 	const WorldEventRewardConfig::ConfigItem *reward_cfg = LOGIC_CONFIG->GetWorldEventCfg().GetRewardCfg(curr_event_type);
// 	if (NULL == reward_cfg)
// 	{
// 		return;
// 	}
// 
// 	int curr_role_score = m_score_list[curr_event_type];
// 	int reward_factor = 0;
// 	int curr_reward_flag = 0;
// 
// 	for (int i = 0; i < WORLD_EVENT_REWARD_GRADE_COUNT; ++ i)
// 	{
// 		int check_reward_flag_idx = curr_event_type * WORLD_EVENT_REWARD_GRADE_COUNT + i;
// 
// 		if (0 == (m_fetch_reward_flag & (1 << check_reward_flag_idx)))
// 		{
// 			if (curr_role_score >= reward_cfg->need_score_list[i] && reward_cfg->reward_factor_list[i] > 0)
// 			{
// 				curr_reward_flag |= (1 << check_reward_flag_idx);
// 				reward_factor += reward_cfg->reward_factor_list[i];
// 			}
// 		}
// 	}
// 
// 	if (0 == reward_factor || 0 == curr_reward_flag)
// 	{
// 		m_role->NoticeNum(errornum::EN_WORLD_EVENT_NOT_REWARD_TO_FETCH);
// 		return;
// 	}
// 
// 	const WorldEventTypeConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetWorldEventCfg().GetEventTypeCfg(curr_event_type);
// 	if (NULL == cfg_item)
// 	{
// 		return;
// 	}
// 
// 	if (NULL != ITEMPOOL->GetItem(cfg_item->base_item.item_id))
// 	{
// 		if (!m_role->GetKnapsack()->HaveSpace())
// 		{
// 			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
// 			return;
// 		}
// 	}
// 
// 	// 消耗
// 	int old_fetch_reward_flag = m_fetch_reward_flag;
// 
// 	m_fetch_reward_flag |= curr_reward_flag;
// 
// 	// 奖励
// 	int reward_coin = static_cast<int>(cfg_item->base_coin * reward_factor);
// 	int reward_xianhun = static_cast<int>(cfg_item->base_xianhun * reward_factor);
// 	int reward_zhenqi = static_cast<int>(cfg_item->base_zhenqi * reward_factor);
// 
// 	ItemConfigData reward_item = cfg_item->base_item;
// 	reward_item.num = static_cast<int>(reward_item.num * reward_factor);
// 
// 	if (reward_coin)
// 	{
// 		m_role->GetKnapsack()->GetMoney()->AddCoinBind(reward_coin, "WorldEventReward");
// 	}
// 
// 	if (reward_xianhun > 0)
// 	{
// 		m_role->AddXianhun(reward_xianhun, "WorldEventReward");
// 	}
// 
// 	if (reward_zhenqi > 0)
// 	{
// 		m_role->AddYuanli(reward_zhenqi, "WorldEventReward");
// 	}
// 
// 	const ItemBase *item_base = ITEMPOOL->GetItem(cfg_item->base_item.item_id);
// 	if (NULL != item_base)
// 	{
// 		if (reward_item.num > item_base->GetPileLimit())
// 		{
// 			reward_item.num = item_base->GetPileLimit();
// 		}
// 
// 		m_role->GetKnapsack()->Put(reward_item, PUT_REASON_WORLD_EVENT_REWARD);
// 	}
// 
// 	this->SendInfo();
// 
// 	{
// 		//日志
// 		gamelog::g_log_world_event.printf(LL_INFO, "FetchReward:: user[%d, %s], level[%d], event_type[%d], old_fetch_flag[%d], new_fetch_flag[%d], "
// 			"fetch_coin[%d], fetch_xianhun[%d], fetch_zhenqi[%d], fetch_item[%s]", 
// 			m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), curr_event_type, old_fetch_reward_flag, m_fetch_reward_flag,
// 			reward_coin, reward_xianhun, reward_zhenqi, ItemConfigDataLog(&reward_item, NULL));
// 
// 		ROLE_LOG_QUICK10(LOG_TYPE_WORLD_EVENT_REWARD, m_role, m_role->GetLevel(), curr_event_type, NULL, NULL, reward_item.item_id, reward_item.num, 0, 0);
// 	}
}

void RoleWorldEvent::LightWord()
{
// 	UNSTD_STATIC_CHECK(WORLD_EVENT_TITLE_WORD_MAX_NUM <= (int)sizeof(m_light_word_flag) * 8);
// 
// 	if (WORLD_EVENT_TYPE_SHENMOZHIXI != WorldEventManager::Instance().GetEventType())
// 	{
// 		return;
// 	}
// 
// 	int total_add_score = 0; char curr_light_flag = 0;
// 
// 	int word_max_count = LOGIC_CONFIG->GetWorldEventCfg().GetLightWordCount();
// 	for (int word_seq = 0; word_seq < word_max_count; ++ word_seq)
// 	{
// 		const WorldEventLightWordConfig::ConfigItem *cfg_item = LOGIC_CONFIG->GetWorldEventCfg().GetLightWordCfg(word_seq);
// 		if (NULL == cfg_item)
// 		{
// 			continue;
// 		}
// 
// 		if (0 != (m_light_word_flag & (1 << word_seq)))
// 		{
// 			continue;
// 		}
// 
// 		// 检查消耗
// 		static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
// 		{
// 			ItemExtern::ItemConsumeConfig comsume_stuff;
// 			comsume_stuff.item_id = cfg_item->stuff_id;
// 			comsume_stuff.num = cfg_item->stuff_count;
// 			comsume_stuff.buyable = false;
// 
// 			if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, &comsume_stuff, &consume_list, false))
// 			{
// 				continue;
// 			}
// 		}
// 
// 		// 消耗
// 		{
// 			if (!m_role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "WorldEventLightWord")) 
// 			{
// 				continue;
// 			}
// 		}
// 
// 		++ total_add_score;
// 		curr_light_flag |= (1 << word_seq);
// 
// 		{
// 			//日志
// 			gamelog::g_log_world_event.printf(LL_INFO, "LightWordConsume::user[%d, %s], light_word_seq[%d], consume_item[item_id=%d, item_num=%d]", 
// 				m_role->GetUID(), m_role->GetName(), word_seq, cfg_item->stuff_id, cfg_item->stuff_count);
// 		}
// 	}
// 
// 	if (total_add_score > 0 || 0 != curr_light_flag)
// 	{
// 		m_light_word_flag |= curr_light_flag;
// 		this->AddScore(WORLD_EVENT_TYPE_SHENMOZHIXI, total_add_score);
// 		this->SendInfo();
// 
// 
// 		{
// 			//日志
// 			gamelog::g_log_world_event.printf(LL_INFO, "LightWordResult::user[%d, %s], curr_light_flag[%d], now_light_flag[%d], add_score[%d], new_score[%d]", 
// 				m_role->GetUID(), m_role->GetName(), curr_light_flag, m_light_word_flag, total_add_score, m_score_list[WORLD_EVENT_TYPE_SHENMOZHIXI]);
// 		}
// 	}
// 	else
// 	{
// 		m_role->NoticeNum(errornum::EN_WORLD_EVENT_LIGHT_WORD_STUFF_LIMIT);
// 	}
// 
// 	this->CheckLightWorld();
}

void RoleWorldEvent::SendInfo()
{
// 	Protocol::SCWorldEventRoleInfo weri;
// 	weri.light_word_flag = m_light_word_flag;
// 	weri.curr_find_npc_id = m_curr_find_npc_id;
// 	weri.reserve_sh = 0;
// 	weri.fetch_reward_flag = m_fetch_reward_flag;
// 	weri.find_npc_flag = m_find_npc_flag;
// 	memcpy(weri.event_score_list, m_score_list, sizeof(weri.event_score_list));
// 	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&weri, sizeof(weri));
}

void RoleWorldEvent::CheckLightWorld()
{
// 	int word_max_count = LOGIC_CONFIG->GetWorldEventCfg().GetLightWordCount();
// 
// 	bool light_all_word = true;
// 	for (int i = 0; i < word_max_count; ++ i)
// 	{
// 		if (0 == (m_light_word_flag & (1 << i)))
// 		{
// 			light_all_word = false;
// 			break;
// 		}
// 	}
// 
// 	if (light_all_word)
// 	{
// 		m_light_word_flag = 0;
// 		this->SendInfo();
// 
// 		{
// 			 //日志
// 			gamelog::g_log_world_event.printf(LL_INFO, "CheckLightWorld : user[%d, %s]", m_role->GetUID(), m_role->GetName());
// 		}
// 	}
}

